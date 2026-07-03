#pragma once

//
// Umbrella include for the parameter (fixture attribute) system.
//
// A Parameter is a typed view over a slice of the universe DMX buffer; its
// layout lives in a runtime, interned Descriptor (built from a fixture-library
// file or a preset) shared across all instances of the same personality. This
// gives zero-copy writes, no per-channel hashing, a semantic API, and real
// 8/16-bit support, while still supporting fixtures known only at runtime.
//
// Split across:
//   Channel.h     - Type / Field / Channel vocabulary
//   Descriptor.h  - Descriptor + DescriptorStore (flyweight interning)
//   Parameter.h   - Parameter view + semantic API
//   VDimmer.h     - VDimmerParameter (virtual dimmer)
//   Presets.h     - Descriptor presets for hardcoded fixtures / tests
//
#include "Channel.h"
#include "Descriptor.h"
#include "Parameter.h"
#include "Presets.h"
#include "VDimmer.h"
