#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

// AstNodes
using Tanzanite::AstNodes::InvalidNode;

namespace Tanzanite::Parser {
    void Parser::parseBody(BlockNode *node) {
        int end_counter = 0;
        Token cur = this->lex.GenerateToken();

        while (!(cur.type == TokenTypes::End && end_counter == 0)) {
            switch (cur.type) {
                case TokenTypes::Def:
                case TokenTypes::Fun:
                    bustError("Function cannot be defined inside function", "This is not C compliant");
                    break;
                case TokenTypes::Blank:
                    // this->nodes.push_back(new AstNodes::Emt());
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
