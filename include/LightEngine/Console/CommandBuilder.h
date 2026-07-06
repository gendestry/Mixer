#pragma once

#include "LightEngine/Console/CommandAst.h"
#include "Syntax/Node.h"   // Parsing::Syntax::Node (from the synparser library)

//
// CommandBuilder: lowers a parsed CST (the `command` rule node produced by the
// synparser Engine over grammar/commands.syn) into a typed Command AST.
//
namespace LightEngine::Console
{
    class CommandBuilder
    {
        using Node = Parsing::Syntax::Node;

    public:
        [[nodiscard]] Command build(const Node& command);

    private:
        Command   buildStore(const Node& cmd, Verb verb);   // store_cmd / delete_cmd
        Command   buildSelect(const Node& select_cmd);
        Selection buildSelection(const Node& selection);
        SelTerm   buildTerm(const Node& term);
        ObjectRef buildObject(const Node& object);
        ObjType   buildObjType(const Node& objtype);
        Level     buildLevel(const Node& level);
    };
}
