#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

// AstNodes
using Tanzanite::AstNodes::IfNode;
using Tanzanite::AstNodes::ValueNode;

namespace Tanzanite::Parser {
    AstNode* Parser::parseIf(bool isUnless) {
        IfNode* node = new IfNode((ValueNode*) this->parseValue(), isUnless);
        Token maybeThen = this->lex.GenerateToken();
        if (maybeThen.type == TokenTypes::Then) {
            this->parseBody(node->getBody());
        } else {
            this->lex.StepBack(maybeThen.text.length());
            this->parseBody(node->getBody());
        }

        return node;
    }

    AstNode* Parser::parseCondition(Token tk) {
        AstNode *node = nullptr;
        switch (tk.type) {
            case TokenTypes::If:
            case TokenTypes::Unless:
                node = this->parseIf(tk.type == TokenTypes::Unless);
                break;
            default:
                bustError("Invalid condition token", tk.text);
                break;
        }

        return node;
    }
}
