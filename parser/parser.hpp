#pragma once

#include "lexer/lexer.hpp"
#include "tokens.hpp"
#include <ast/nodes.hpp>
#include <string>
#include <vector>

using Tanzanite::AstNodes::AstNode;
using Tanzanite::Tokens::Token;

namespace Tanzanite::Parser {
    class Parser {
        private:
            Tanzanite::Lexer::Lexer lex;
            std::vector<AstNode*> nodes;
        public:
            Parser(std::string text): lex(text) {}
            void parse();
            AstNode *varOrFunction(Token tkn);
            void print();
    };
}
