#include "ast/nodes.hpp"
#include "tokens.hpp"
#include <cstdio>
#include <parser.hpp>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

namespace Tanzanite::Parser {
    void Parser::parse() {
        Token tk = this->lex.GenerateToken();
        switch (tk.type) {
            case TokenTypes::Identifier:
                this->nodes.push_back(this->varOrFunction(tk));
                break;
            default:
                printf("Unknown yet!\n");
        }
    }
    
    void Parser::print() {
        for (const auto &node : this->nodes) {
            if (!node) printf("nullptr\n");
            printf("%s\n", node->stringify().c_str());
        }
    }

    AstNode* Parser::varOrFunction(Token tkn) {
        // name = val
        // name: type = val
        // name arg arg
        Token next = this->lex.GenerateToken();

        if (next.type == TokenTypes::Eof) {
            // err
        }

        if (next.type == TokenTypes::Identifier) {
            return nullptr;
        }

        if (next.type == TokenTypes::Colon) {
            Token type = this->lex.GenerateToken();
            Token isAssign = this->lex.GenerateToken();

            auto decl = new AstNodes::VariableDeclaration(tkn, type);

            if (isAssign.type == TokenTypes::Assing) {
                return new AstNodes::VariableInitialization(*decl, this->lex.GenerateToken());
            }
        }

        if (next.type == TokenTypes::Assing) {

            return new AstNodes::VariableDefinition(tkn, this->lex.GenerateToken());
        }

        return nullptr;
    }
}
