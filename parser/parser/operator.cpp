#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

// AstNodes
using Tanzanite::AstNodes::ValueNode;
using Tanzanite::AstNodes::OperatorNode;

namespace Tanzanite::Parser {
    AstNode* Parser::parseOperator() {
        Token op = this->lex.GenerateToken();
        switch (op.type) {
            case TokenTypes::Equals:
            case TokenTypes::NotEquals:
            case TokenTypes::Plus:
            case TokenTypes::Minus:
            case TokenTypes::Asterisk:
            case TokenTypes::DoubleAsterisk:
            case TokenTypes::Slash:
            case TokenTypes::DoubleSlash:
            case TokenTypes::Modulo:
            case TokenTypes::Ampersand:
            case TokenTypes::Or:
            case TokenTypes::Pipe:
            case TokenTypes::PipeTo:
            case TokenTypes::Caret:
            case TokenTypes::LeftShift:
            case TokenTypes::RightShift:
            case TokenTypes::Less:
            case TokenTypes::LessEquals:
            case TokenTypes::Greater:
            case TokenTypes::GreaterEquals:
            case TokenTypes::Spaceship: {
                Token nextVal = this->lex.GenerateToken();
                if (!checkNumericalType(nextVal)) return nullptr;

                BlockNode *node = new BlockNode();
                node->addLine(new OperatorNode(op.text));
                node->addLine(new ValueNode(nextVal.text, nullptr));

                return node;
            } default:
                this->lex.StepBack(op.text.length());
                return nullptr;
        }
    }

}
