#pragma once

#include "lexer.hpp"
#include "tokens.hpp"
#include <ast/nodes.hpp>
#include <map>
#include <string>
#include <vector>

using Tanzanite::AstNodes::AstNode;
using Tanzanite::AstNodes::BlockNode;
using Tanzanite::AstNodes::FunctionNode;
using Tanzanite::Tokens::Token;

namespace Tanzanite::Parser {
    void bustError(std::string msg, std::string info);
    std::string stringifyType(Token tk);
    bool checkNumericalType(const Token& type);

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
            void parseBody(BlockNode *node);
            AstNode* parseFunction(Token tk);
            AstNode* parseCondition(Token tk);
            AstNode* parseVariable(Token val, bool checkForEnd=true);
            AstNode* parseValue();
            AstNode* parseIf(bool isUnless);
            AstNode* parseElsif(bool isNegated);
            bool checkType(const Token& type);
            void skipSemicolon(bool blanks);
            // util methods
            void registerIdentifier(std::string name, Identifier i);
            void handleFunctionTop(FunctionNode* node);
            AstNode* parseOperator();
    };
}
