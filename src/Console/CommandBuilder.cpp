#include "LightEngine/Console/CommandBuilder.h"

#include <iostream>
#include <string>

namespace
{
    using Node = Parsing::Syntax::Node;

    // First child rule-node with the given rule name (null if none).
    const Node* childRule(const Node& n, const std::string& rule)
    {
        for (const auto& k : n.kids)
            if (!k.isLeaf() && k.rule == rule) return &k;
        return nullptr;
    }

    // First leaf token with the given token name (null if none).
    const Node* childLeaf(const Node& n, const std::string& tokenName)
    {
        for (const auto& k : n.kids)
            if (k.isLeaf() && k.token->name == tokenName) return &k;
        return nullptr;
    }
}

namespace LightEngine::Console
{
    Command CommandBuilder::build(const Node& command)
    {
        // command : store_cmd | delete_cmd | select_cmd  (exactly one child rule)
        const Node& c = command.kids.front();
        if (c.rule == "store_cmd")  return buildStore(c, Verb::Store);
        if (c.rule == "delete_cmd") return buildStore(c, Verb::Delete);
        return buildSelect(c);
    }

    Command CommandBuilder::buildStore(const Node& cmd, Verb verb)
    {
        // store_cmd : STORE target ;   target : objtype NUM ;
        Command out;
        out.verb = verb;
        if (const Node* t = childRule(cmd, "target"))
        {
            ObjectRef ref;
            if (const Node* ot = childRule(*t, "objtype")) ref.type = buildObjType(*ot);
            if (const Node* num = childLeaf(*t, "NUM"))    ref.id   = std::stoi(num->token->value);
            out.target = ref;
        }
        return out;
    }

    Command CommandBuilder::buildSelect(const Node& select_cmd)
    {
        // select_cmd : selection level? ;
        Command out;
        out.verb = Verb::Select;
        if (const Node* sel = childRule(select_cmd, "selection")) out.selection = buildSelection(*sel);
        if (const Node* lvl = childRule(select_cmd, "level"))     out.level     = buildLevel(*lvl);
        return out;
    }

    Selection CommandBuilder::buildSelection(const Node& selection)
    {
        // selection : term (op term)*  -> kids: term, op, term, op, term, ...
        Selection out;
        SelOp pending = SelOp::Add;   // first term unions in
        for (const auto& k : selection.kids)
        {
            if (k.rule == "term")
            {
                out.terms.push_back({pending, buildTerm(k)});
                pending = SelOp::Add;
            }
            else if (k.rule == "op")
            {
                // op : PLUS | MINUS  -> single leaf child
                pending = k.kids.front().token->name == "MINUS" ? SelOp::Sub : SelOp::Add;
            }
        }
        return out;
    }

    SelTerm CommandBuilder::buildTerm(const Node& term)
    {
        // term : object (THRU object)?  -> one or two `object` children
        SelTerm out;
        const Node* first  = nullptr;
        const Node* second = nullptr;
        for (const auto& k : term.kids)
            if (k.rule == "object") { (first ? second : first) = &k; }

        out.from = buildObject(*first);
        if (second) out.thru = buildObject(*second);
        return out;
    }

    ObjectRef CommandBuilder::buildObject(const Node& object)
    {
        // object : objtype NUM | NUM
        ObjectRef ref;   // defaults to Fixture for a bare NUM
        if (const Node* ot = childRule(object, "objtype")) ref.type = buildObjType(*ot);
        if (const Node* num = childLeaf(object, "NUM"))    ref.id   = std::stoi(num->token->value);
        return ref;
    }

    ObjType CommandBuilder::buildObjType(const Node& objtype)
    {
        // objtype : FIXTURE | GROUP | COLOR | CUE | PRESET  -> single leaf child
        const std::string& name = objtype.kids.front().token->name;
        if (name == "GROUP")  return ObjType::Group;
        if (name == "COLOR")  return ObjType::Color;
        if (name == "CUE")    return ObjType::Cue;
        if (name == "PRESET") return ObjType::Preset;
        return ObjType::Fixture;
    }

    Level CommandBuilder::buildLevel(const Node& level)
    {
        // level : AT (FULL | NUM)
        Level out;
        if (childLeaf(level, "FULL"))
        {
            out.full = true;
            out.percent = 100.0f;
        }
        else if (const Node* num = childLeaf(level, "NUM"))
        {
            out.percent = static_cast<float>(std::stoi(num->token->value));
        }
        return out;
    }

    void Command::print(int indent) const
    {
        const std::string p(indent, ' ');
        switch (verb)
        {
            case Verb::Store:  std::cout << p << "Store\n";  break;
            case Verb::Delete: std::cout << p << "Delete\n"; break;
            case Verb::Select: std::cout << p << "Select\n"; break;
        }

        if (target)
            std::cout << p << "  target: " << objName(target->type) << " " << target->id << "\n";

        if (selection)
        {
            std::cout << p << "  selection:\n";
            for (const auto& [op, term] : selection->terms)
            {
                std::cout << p << "    " << (op == SelOp::Sub ? "- " : "+ ")
                          << objName(term.from.type) << " " << term.from.id;
                if (term.thru)
                    std::cout << " Thru " << objName(term.thru->type) << " " << term.thru->id;
                std::cout << "\n";
            }
        }

        if (level)
            std::cout << p << "  @ " << (level->full ? "Full" : std::to_string(static_cast<int>(level->percent)))
                      << "\n";
    }
}
