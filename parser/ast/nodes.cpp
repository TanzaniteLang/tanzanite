#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <string>

using Tanzanite::Tokens::TokenTypes;

namespace Tanzanite::AstNodes {
    VariableNode::VariableNode(Token *token, std::string *type, AstNode *value) {
        this->nodeName = "VariableNode";
        this->value = value;
        if (token != nullptr) this->name = token->text;
        else this->name = "";

        if (type != nullptr) this->type = *type;
        else {
            switch (token->type) {
                case TokenTypes::String:
                    this->type = "String";
                    break;
                case TokenTypes::Char:
                    this->type = "Char";
                    break;
                case TokenTypes::Int:
                    this->type = "Int";
                    break;
                case TokenTypes::Float:
                    this->type = "Float";
                    break;
                case TokenTypes::Bool:
                    this->type = "Bool";
                    break;
                default:
                    this->type = "unknown";
            }
        }
    }
}
