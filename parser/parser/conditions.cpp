#include <ast/nodes.hpp>
#include <tokens.hpp>
#include <parser.hpp>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

// AstNodes
using Tanzanite::AstNodes::ConditionNode;
using Tanzanite::AstNodes::IfNode;
using Tanzanite::AstNodes::ElsifNode;
using Tanzanite::AstNodes::ElseNode;
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

    AstNode* Parser::parseElsif(bool isUnless) {
        ElsifNode* node = new ElsifNode((ValueNode*) this->parseValue(), isUnless);
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
        ConditionNode *node = new ConditionNode();
        bool parsedBegining = false;
        bool isNegated = false;
        bool stop = false;
        while (true) {
            switch (tk.type) {
                case TokenTypes::If:
                case TokenTypes::Unless:
                    if (parsedBegining) {
                        bustError("Starting if condition again?", "");
                        stop = true;
                        break;
                    }
                    isNegated = tk.type == TokenTypes::Unless;
                    node->addToChain(this->parseIf(isNegated));
                    break;
                case TokenTypes::Elsif:
                    if (!parsedBegining) {
                        bustError("If condition cannot start with elsif!", "");
                        stop = true;
                        break;
                    }
                    node->addToChain(this->parseElsif(isNegated));
                    break;
                case TokenTypes::Else: {
                    if (!parsedBegining) {
                        bustError("If condition cannot start with else!", "");
                        stop = true;
                        break;
                    }
                    Token maybeThen = this->lex.GenerateToken();
                    ElseNode *n = new ElseNode();
                    if (maybeThen.type == TokenTypes::Then) {
                        this->parseBody(n->getBody());
                    } else {
                        this->lex.StepBack(maybeThen.text.length());
                        this->parseBody(n->getBody());
                    }
                    node->addToChain(n);
                    stop = true;
                    break;
                } case TokenTypes::Eof:
                    stop = true;
                    break;
                default:
                    this->lex.StepBack(tk.text.length());
                    stop = true;
                    break;
            }

            if (stop) break;

            tk = this->lex.GenerateToken();
        }

        return node;
    }
}
