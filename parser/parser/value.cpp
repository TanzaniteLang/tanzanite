#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

// AstNodes
using Tanzanite::AstNodes::ValueNode;

namespace Tanzanite::Parser {
    // value end delimiters
    // based on expression but everything is well defined
    // if there is any operator, the token right after must exist
    // if there is function call but rules
    // - if in array -> must be in ()
    // - if as function parameter -> must be in ()
    // function returns BlockNode that must be unpacked
    AstNode* Parser::parseValue() {
        BlockNode *node = new BlockNode();
        // begin must be either ( or identifier or value
        Token begin = this->lex.GenerateToken();
        switch (begin.type) {
            case TokenTypes::Char:
            case TokenTypes::Int:
            case TokenTypes::Float:
            case TokenTypes::Bool: {
                node->addLine(new ValueNode(begin.text, &begin));
                while (true) {
                    AstNode *maybeOperator = this->parseOperator();
                    if (!maybeOperator) {
                        Token hm = this->lex.GenerateToken();
                        if (hm.type == TokenTypes::Identifier) {
                            bustError("Invalid operator got", hm.text + " instead");
                        }
                        this->lex.StepBack(hm.text.length());
                        break;
                    }
                    const auto& nodes = ((BlockNode*) maybeOperator)->getLines();
                    for (const auto& n : nodes) {
                        node->addLine(n);
                    }
                }
                break;
            } case TokenTypes::Identifier:
                bustError("Variables can't be used yet", "");
                break;
            case TokenTypes::String:
                node->addLine(new ValueNode(begin.text, &begin));
                break;
            default:
                bustError("Not a value or identifier", begin.text);
        }

        return node;
    }
}
