#pragma once

#include "lexer/lexer.hpp"
#include "tokens.hpp"
#include <ast/nodes.hpp>
#include <map>
#include <string>
#include <vector>

using Tanzanite::AstNodes::AstNode;
using Tanzanite::AstNodes::FunctionNode;
using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

namespace Tanzanite::Parser {
    typedef struct {
        AstNode* node;
        TokenTypes type;
    } Identifier;

    class Parser {
        private:
            Tanzanite::Lexer::Lexer lex;
            std::vector<AstNode*> nodes;
            std::map<std::string, Identifier> globalScope;
        public:
            Parser(std::string text): lex(text) {}
            void parse();
            AstNode* parseFunction(Token current);
            // util methods
            void handleFunctionTop(FunctionNode* node);
    };
}
