// Parses one Mixer command line (grammar/command_input.txt) against
// grammar/commands.syn and prints the typed Command AST. No evaluation yet -
// this demonstrates lexing -> CST -> typed AST.

#include <iostream>
#include <string>
#include <vector>

#include "Tokenizer/Parser.h"
#include "Syntax/GrammarParser.h"
#include "Syntax/Engine.h"

#include "LightEngine/Console/CommandBuilder.h"

#ifndef MIXER_GRAMMAR_DIR
#define MIXER_GRAMMAR_DIR "grammar"
#endif

int main()
{
    const std::string dir = MIXER_GRAMMAR_DIR;

    // --- 1. Lex the input into tokens ------------------------------------
    Parsing::Tokenizer::Parser lexer(dir + "/commands.tokens");
    lexer.parse(dir + "/command_input.txt");

    std::vector<Parsing::Tokenizer::Token> tokens;
    for (auto& t : lexer.getTokens())
        if (!t.ignore) tokens.push_back(t);

    std::cout << "Tokens:\n";
    for (auto& t : tokens)
        std::cout << "  " << t.name << " '" << t.value << "'\n";

    // --- 2. Load the grammar & parse into a CST --------------------------
    auto grammar = Parsing::Syntax::GrammarParser::parseFile(dir + "/commands.syn");
    Parsing::Syntax::Engine engine(grammar, tokens);
    auto cst = engine.parse(grammar.startRule);
    if (!cst)
    {
        std::cerr << "parse failed";
        if (const auto* t = engine.furthestToken())
            std::cerr << " near '" << t->value << "' (token #" << engine.furthestPos() << ")";
        std::cerr << "\n";
        return 1;
    }

    // --- 3. Lower the CST into the typed Command AST ---------------------
    LightEngine::Console::CommandBuilder builder;
    LightEngine::Console::Command cmd = builder.build(*cst);

    std::cout << "\nAST:\n";
    cmd.print(2);
    return 0;
}
