#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

//
// CommandAst: the typed AST for the Mixer command language (see
// grammar/commands.syn). The parser produces a homogeneous CST; CommandBuilder
// lowers it into these nodes. Data only, plus a debug print().
//
namespace LightEngine::Console
{
    enum class ObjType { Fixture, Group, Color, Cue, Preset };

    [[nodiscard]] inline const char* objName(ObjType t)
    {
        switch (t)
        {
            case ObjType::Fixture: return "Fixture";
            case ObjType::Group:   return "Group";
            case ObjType::Color:   return "Color";
            case ObjType::Cue:     return "Cue";
            case ObjType::Preset:  return "Preset";
        }
        return "?";
    }

    // "Group 1", or a bare "1" (which defaults to Fixture).
    struct ObjectRef
    {
        ObjType type = ObjType::Fixture;
        int     id   = 0;
    };

    enum class SelOp { Add, Sub };   // + / -

    // One selection term: a single object or a range "1 Thru 10".
    struct SelTerm
    {
        ObjectRef                from;
        std::optional<ObjectRef> thru;   // range end; null = single object
    };

    // term (op term)* ; the first term is implicitly Add.
    struct Selection
    {
        std::vector<std::pair<SelOp, SelTerm>> terms;
    };

    // "@ 50" or "@ Full"
    struct Level
    {
        bool  full    = false;
        float percent = 0.0f;   // 0..100 (ignored when full == true)
    };

    enum class Verb { Store, Delete, Select };

    struct Command
    {
        Verb                     verb = Verb::Select;
        std::optional<ObjectRef> target;      // Store/Delete: where it lands
        std::optional<Selection> selection;   // Select: what is selected
        std::optional<Level>     level;       // Select: optional @ level

        void print(int indent = 0) const;
    };
}
