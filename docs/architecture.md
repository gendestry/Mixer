# Mixer / LightEngine — how it works

This document explains how the lighting engine is put together and how a value
you set travels from your code all the way to a DMX cable. It describes the code
**as it exists today** (no pools/presets yet). Diagrams live in
[`architecture.puml`](./architecture.puml).

If you only remember two things, remember these:

1. **Two identities for a fixture.** While the show is *running* we use
   `shared_ptr<Fixture>` (a live object). When something is *stored* (a cue, an
   effect recipe) we use the fixture's **FID** (a `uint16_t` id) instead. FIDs
   are stable and serializable; pointers are not. Conversion happens only at the
   store/recall boundary.
2. **Nothing is remembered in the DMX buffer.** The engine is *non-tracking*:
   every frame it wipes all the DMX buffers to zero and rebuilds the entire look
   from scratch (playback, then programmer, then effects). So values are *pushed
   through* the fixtures every frame — they are never "left" in the buffer.

---

## The big picture

Data flows top to bottom. Each layer only knows about the one below it.

```
Engine                     orchestrates everything, owns the pieces
  ├── Patch                owns universes + the FID -> fixture map
  │     ├── FixtureLibrary catalogue of fixture "personalities" (definitions)
  │     └── Universe(s)    a 512-byte DMX buffer + the fixtures placed in it
  │           └── Fixture  a stack of Parameters, wired to the universe buffer
  │                 └── Parameter   a typed view over a slice of DMX bytes
  ├── Programmer           the live editing layer (what you're changing now)
  ├── Sequence(s)          playback: ordered lists of stored Cues
  ├── FixtureGroup(s)      named selections of fixtures (for effects/programmer)
  └── DMXOutput            sends the universe buffers over the network (sACN)
```

---

## The layers, bottom to top

### 1. Channel / Type / Field — the vocabulary

(`Fixture/Parameters/Channel.h`)

The lowest-level words the whole system shares:

- **`Type`** — *what kind of attribute*: `DIMMER`, `COLOR`, `POSITION`,
  `VDIMMER`. They're bit flags so a fixture can be described as `COLOR | DIMMER`.
- **`Field`** — *the role of one physical DMX channel*: `R`, `G`, `B`, `W`, `A`,
  `PAN`, `TILT`, `DIMMER`.
- **`Channel`** — *where one channel physically lives*: its field, byte offset,
  and width (1 byte = 8-bit, 2 bytes = 16-bit).

### 2. Descriptor — the layout of one parameter

(`Fixture/Parameters/Descriptor.h`)

A `Descriptor` says "this parameter is a COLOR, spans 3 bytes, and its channels
are R@0, G@1, B@2." It's a **flyweight**: identical layouts are *interned* once
in a `DescriptorStore` and shared by every fixture that uses them. This is why
patching a hundred identical fixtures doesn't duplicate a hundred layouts.

### 3. Parameter — a view over DMX bytes

(`Fixture/Parameters/Parameter.h`)

A `Parameter` **owns no bytes**. It holds:

- a pointer to its shared `Descriptor` (the layout),
- a pointer into the universe's DMX buffer (`m_bytes`),
- its base offset in that buffer.

When you call `parameter.setColor(rgb)`, it looks up the R/G/B channels in the
descriptor and writes normalized values straight into the universe buffer. **All
values crossing the API are normalized floats in `[0,1]`** — the parameter
converts to raw DMX bytes on write.

`VDimmerParameter` is a special parameter: a *virtual dimmer*. Fixtures that have
color but no dimmer channel get one automatically; it scales the color channels
to fake an intensity, using zero extra DMX footprint.

### 4. Fixture — a stack of parameters

(`Fixture/Fixture.h`, `src/Fixture/Fixture.cpp`)

A `Fixture` is one patched light. It's a stack of `Parameter`s laid out
contiguously, plus identity: a `name`, a **`fid`** (global fixture id), and the
universe it lives in. It keeps two views of its parameters:

- `m_parameters` — in layout order,
- `m_byType` — indexed by capability (`map<Type, vector<ParamPtr>>`), so
  "give me all COLOR parameters" is instant.

The convenience writers fan a value out to the relevant parameters:

```cpp
void Fixture::setColor(const RGB& color) {
    for (auto& p : parameters(Type::COLOR))   // every color param
        p->setColor(color);
}
void Fixture::setIntensity(float norm) {
    if (has(Type::DIMMER))  ... real dimmer
    else                    ... virtual dimmer
}
```

> Note: a fixture *can* hold several parameters of the same type (e.g. multiple
> COLOR params). Today `setColor` writes the **same** color to all of them —
> there's no per-cell addressing yet.

**How a parameter gets bound to the buffer:** at *patch* time (not edit time).
When a fixture is placed into a universe, the universe assigns it a start address
and hands it the buffer pointer; the fixture propagates both down to every
parameter. From then on each parameter points at its exact slice of the universe
buffer.

### 5. Universe — a 512-byte DMX frame

(`DMX/Universe.h`)

A `Universe` *is* a `FragmentedStorage` of fixtures — it owns the raw
`array<uint8_t, 512>` buffer and the logic that packs fixtures into free channel
slots. Its extra jobs: wire every placed fixture's parameters to its buffer, and
keep a name→fixtures index. `blackout()` zeroes the buffer (called at the start
of every frame).

### 6. Patch — the source of truth for what's patched

(`Engine/Components/Patch.h`)

`Patch` owns the `FixtureLibrary`, all the `Universe`s, and the crucial
**`map<FID, FixturePtr>`** — the one place that answers "what is fixture #7?"
`patch("LED Par", universe, amount)` copies a library definition into a universe
(which wires it) and assigns sequential FIDs. It also tracks which universes are
*dirty* (changed) so output knows what to send.

`getFixture(fid)` / `getFixtures(fids)` are the **FID → live fixture** resolvers
— the bridge used every time something stored is recalled.

---

## The two "identities" in detail

| | Live layer | Stored layer |
|---|---|---|
| Fixture identity | `shared_ptr<Fixture>` | `uint16_t` FID |
| Used by | `Programmer` selection, `FixtureGroup`, running `Effect` | `Cue`, effect `Spec` |
| Why | fast, direct writes | stable, serializable, survives re-patch |

Conversion points:
- **Store:** `Programmer::makeCue()` turns `map<FixturePtr,Values>` into
  `map<FID,Values>`; effects report their target as `Spec.fids`.
- **Recall:** `Patch::getFixture(fid)` turns FIDs back into live fixtures.

---

## FixtureGroup — a named selection

(`DMX/FixtureGroup.h`)

A `FixtureGroup` is a bag of `shared_ptr<Fixture>` (it does **not** own the
fixtures — the universes do). It also caches the members' parameters bucketed by
`Type`, so an effect can ask for "every color parameter in this group" in one
call. Groups are used two ways: as reusable named selections on the `Engine`, and
as the target an `Effect` owns.

---

## Programmer — the live editing layer

(`Engine/Programmer.h`, `src/Engine/Programmer.cpp`)

This is the "console" you interact with: build a selection, set values, and they
accumulate until you clear or store. It renders on top of playback every frame.

**State:**

| Field | Meaning |
|---|---|
| `m_selection` (`vector<FixturePtr>`) | *who* you're editing right now (ordered, deduped) |
| `m_values` (`map<FixturePtr, Values>`) | *what* you've set — the accumulated look |
| `m_driven` (`set<FixturePtr>`) | every fixture the programmer has ever touched |
| `m_effects` (`EffectEngine`) | live effects running on selections |
| `m_latched` (`bool`) | have you edited since the last `select()`? |

**Selection semantics (the `m_latched` trick):**

Every edit (`setColor`, `setIntensity`, `addEffect`) calls `touch()` →
`m_latched = true`. The next `select()` checks it:

```cpp
void select(FixtureGroup& group) {
    if (m_latched) { m_selection.clear(); m_latched = false; }  // start fresh
    addToSelection(group);                                       // else add
}
```

- Select, select, select (no edits) → **additive** (union).
- Set a value, then select → **fresh** selection (but `m_values` is kept!).

That "kept `m_values`" is what lets you program fixtures 1–6 blue, then move on to
7–12 red, and have both remembered.

**Setting values** writes into `m_values` per selected fixture and marks them
driven:

```cpp
void setColor(const RGB& c) {
    for (auto& f : m_selection) { m_values[f].color = c; m_driven.insert(f); }
    touch();
}
```

Note `m_values` is **not** bound to parameters. It's plain data. It only reaches
DMX when `apply()` pushes it through the fixtures (below).

**Rendering — `apply(TimeContext)`**, called by the Engine each frame:

```cpp
1. for f in m_driven:  f->setColor(white)          // default touched fixtures
2. for (f, v) in m_values:                          // overlay programmed values
       if v.color     f->setColor(*v.color)
       if v.intensity f->setIntensity(*v.intensity)
3. m_effects.update(t)                              // effects on top (latest wins)
```

Step 1 exists so a selected-but-uncolored fixture shows white instead of a stale
value from another layer.

**Storing — `makeCue(number)`** snapshots the live state into a serializable
`Cue`: `m_values` (keyed by FixturePtr) → `cue.values` (keyed by FID), and each
live effect → its `Spec` recipe.

**`clear()`** wipes everything (the Clear button).

---

## Effects

(`Effects/*.h`, `src/Effects/*.cpp`)

- **`Effect`** — a unit of behavior re-evaluated every frame. It **owns its
  target `FixtureGroup` by value**, so it's self-contained. `apply(t)` writes its
  contribution; `spec()` reports its current recipe.
- Concrete effects: `ColorEffect` (constant color), `DimmerEffect` (constant
  level), `ColorFade` (ping-pong between two colors), `DimmerChase` (a traveling
  intensity wave shaped by a curve).
- **`Spec`** — the *serializable recipe* of an effect: its `kind`, target `fids`,
  and parameters. This is what a `Cue` stores.
- **`EffectFactory::build(spec, group)`** — turns a `Spec` back into a live
  `Effect`. One representation, both directions: `Effect → Spec` (store) and
  `Spec → Effect` (recall).
- **`EffectEngine`** — owns a list of effects and `update()`s them in order.
- **`TimeContext`** — the timing passed to every effect (`elapsed`, `dt`, `tick`,
  and a `phase(hz)` helper). Animation is in real seconds, so it's frame-rate
  independent.

---

## Show / playback

(`Show/Cue.h`, `Show/Sequence.h`, `src/Show/Sequence.cpp`)

- **`Cue`** — a stored, recallable look. `values` are per-FID (stable), `effects`
  are `Spec` recipes. Also has fade/delay timing.
- **`Sequence`** — an ordered list of cues (a playback list / executor).
  `store(cue)` adds or replaces by number; `go(patch)` advances to the next cue
  and **instantiates** its effects (builds live `Effect`s from the `Spec`s via the
  factory, resolving FIDs through the patch); `apply(t, patch)` renders the active
  cue every frame.

---

## Output

(`Engine/Components/DMXOutput.h`)

`DMXOutput` holds one sACN sender per universe (created lazily) and streams the
universe buffers onto the network. It reads each `Universe`'s 512-byte frame
directly (zero copy). `sendAll(patch)` sends every universe each frame.

---

## Engine — the orchestrator

(`Engine/Engine.h`, `src/Engine/Engine.cpp`)

`Engine` owns the `Patch`, `DMXOutput`, `Programmer`, the named `FixtureGroup`s,
and the `Sequence`s, and exposes the high-level API: **define → patch → group →
program → store → go → update**.

The heart is `update()`, called once per frame:

```cpp
void Engine::update() {
    // advance clock, build TimeContext (elapsed, dt, tick)
    m_patch.blackout();                       // 1. wipe every universe to zero
    for (seq) seq.apply(ctx, m_patch);        // 2. playback layer
    m_programmer.apply(ctx);                  // 3. programmer on top (wins)
    for (fixture) fixture->applyVirtualDimmers(); // 4. compose vdimmers once
    m_output.sendAll(m_patch);                // 5. stream over sACN
    m_patch.clearDirty();
}
```

That's the non-tracking model in five steps: clear, playback, programmer,
compose, send — rebuilt from scratch every frame.

---

## Follow one color from click to cable

1. You call `programmer.setColor(blue)`. It writes `Values{color: blue}` into
   `m_values[fixture]` and marks the fixture driven. **No DMX yet.**
2. Next frame, `Engine::update()` runs. It blacks out the buffers, then calls
   `programmer.apply(ctx)`.
3. `apply()` calls `fixture->setColor(blue)`.
4. `Fixture::setColor` loops its COLOR parameters and calls
   `parameter->setColor(blue)`.
5. `Parameter::setColor` writes normalized R/G/B into its slice of the
   **universe buffer** (the binding set up at patch time).
6. `Engine` composes virtual dimmers, then `DMXOutput::sendAll` reads the
   universe buffer and streams it as sACN.
7. The light turns blue. Next frame, repeat from step 2.

---

## Glossary

- **FID** — Fixture ID, a stable `uint16_t`. The stored identity of a fixture.
- **Descriptor** — the shared (flyweight) layout of a parameter.
- **Parameter** — a typed view over a slice of the DMX buffer; owns no bytes.
- **Virtual dimmer (VDIMMER)** — fake intensity for color-only fixtures, done by
  scaling the color channels.
- **Non-tracking** — every frame is a complete look built from scratch; the
  buffer is wiped first.
- **Spec** — the serializable recipe of an effect (used by cues).
- **sACN** — the network protocol used to stream DMX (Streaming ACN / E1.31).
