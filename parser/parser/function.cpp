#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

// AstNodes
using Tanzanite::AstNodes::VariableNode;

namespace Tanzanite::Parser {
    // first handle declaration cases
    // can be either Colon, LBracket or Indentifier
    // If it is bracket, await RBracket
    // If it is Indentifier it means it's parameter
    // If it is Colon, it means it has 0 params and return type is next
    void Parser::handleFunctionTop(FunctionNode* node) {
        bool expect_end_bracket = false;

        Token current = this->lex.GenerateToken();
        while (current.type != TokenTypes::Blank && current.type != TokenTypes::Semicolon) {
            switch (current.type) {
                case TokenTypes::LBracket:
                    expect_end_bracket = true;
                    break;
                case TokenTypes::RBracket:
                    if (expect_end_bracket) {
                        expect_end_bracket = false;
                    } else {
                        bustError("Useless ) ", "");
                        expect_end_bracket = false;
                    }
                    break;
                case TokenTypes::Colon:
                    if (!expect_end_bracket) {
                        Token retType = this->lex.GenerateToken();
                        node->setReturnType(retType.text);
                    } else {
                        bustError("Bracket not closed", "");
                    }
                    break;
                case TokenTypes::Identifier: {
                    VariableNode *var = (VariableNode*) this->parseVariable(current, false);
                    if (!var) {
                        bustError("Unable to parse function parameter", "Skip!");
                        break;
                    }
                    Token next = this->lex.GenerateToken();
                    if (next.type != TokenTypes::Comma && next.type != TokenTypes::Colon) {
                        if (expect_end_bracket && next.type != TokenTypes::RBracket) {
                            bustError("Expected , : or ) got", next.text);
                        }
                    
                    }
                    node->addParam(var->getName(), var);
                    this->lex.StepBack(next.text.length());
                    break;
                } case TokenTypes::Comma:
                    break;
                default:
                    continue;
            }
            current = this->lex.GenerateToken();
        }
    }

    AstNode* Parser::parseFunction(Token tk) {
        Token name = this->lex.GenerateToken();
        if (name.type != TokenTypes::Identifier) return nullptr;

        bool isCFunc = tk.type == TokenTypes::Fun ? true : false;
        FunctionNode *node = new FunctionNode(name.text, isCFunc);

        this->handleFunctionTop(node);
        this->skipSemicolon(true);
        Token next = this->lex.GenerateToken();

        if (next.type != TokenTypes::End) {
            this->lex.StepBack(next.text.length());
            this->parseBody(node->getBody());
        }
        return node;
    }
}
