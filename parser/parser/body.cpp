#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

// AstNodes
using Tanzanite::AstNodes::InvalidNode;

namespace Tanzanite::Parser {
    void Parser::parseBody(BlockNode *node) {
        Token cur = this->lex.GenerateToken();

        while (cur.type != TokenTypes::End) {
            switch (cur.type) {
                case TokenTypes::Def:
                case TokenTypes::Fun:
                    bustError("Function cannot be defined here", "This is not C compliant");
                    break;
                case TokenTypes::Blank:
                    // this->nodes.push_back(new AstNodes::Emt());
                    break;
                case TokenTypes::If:
                case TokenTypes::Unless:
                    node->addLine(this->parseCondition(cur));
                    break;
                case TokenTypes::Identifier:
                    node->addLine(this->parseVariable(cur));
                    break;
                default:
                    node->addLine(new InvalidNode());
            }
            cur = this->lex.GenerateToken();

        }
    }
}
