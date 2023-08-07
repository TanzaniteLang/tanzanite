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

namespace Tanzanite::Parser {
    enum class IdentifierType {
        Variable,
        Function
    };

    typedef struct {
        AstNode* node;
        IdentifierType type;
    } Identifier;

    std::string stringifyType(Token tk);

    class Parser {
        private:
            Tanzanite::Lexer::Lexer lex;
            std::vector<AstNode*> nodes;
            std::map<std::string, Identifier> globalScope;
        public:
            Parser(std::string text): lex(text) {}
            void parse();
            AstNode* parseFunction();
            AstNode* parseVariable(Token val);
            AstNode* parseValue();
            bool checkType(const Token& type);
            // util methods
            void handleFunctionTop(FunctionNode* node);
            AstNode* parseOperator();
    };
}
