#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

// AstNodes
using Tanzanite::AstNodes::ValueNode;
using Tanzanite::AstNodes::VariableNode;

namespace Tanzanite::Parser {
    AstNode* Parser::parseVariable(Token val, bool checkForEnd) {
        Token next = this->lex.GenerateToken();
        AstNode *retNode = nullptr;

        switch (next.type) {
            case TokenTypes::Colon: {
                Token type = this->lex.GenerateToken();
                if (!this->checkType(type)) {
                    return nullptr;
                }
                next = this->lex.GenerateToken();
                if (next.type == TokenTypes::Assing) {
                    AstNode *node = this->parseValue();
                    retNode = new VariableNode(&val, &type.text, node);
                } else {
                    this->lex.StepBack(next.text.length());
                    retNode = new VariableNode(&val, &type.text, nullptr);
                }
                break;
            } case TokenTypes::Assing: {
                AstNode *node = this->parseValue();
                auto const &nodes = ((BlockNode *) node)->getLines();
                ValueNode *value = (ValueNode*) nodes[0];
                std::string strType = stringifyType(value->getToken());

                retNode = new VariableNode(&val, &strType, node);
                break;
            } default:
                bustError("Invalid token! Expected = or : but got", next.text + " instead");
        }
        if (checkForEnd) {
            Token isEnd = this->lex.GenerateToken();
            if (isEnd.type != TokenTypes::Semicolon && isEnd.type != TokenTypes::Blank) {
                bustError("Expected new line or ; but got", isEnd.text);
                this->lex.StepBack(isEnd.text.length());
            }
        }
        return retNode;
    }

}
