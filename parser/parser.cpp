#include "ast/nodes.hpp"
#include "tokens.hpp"
#include <cstdio>
#include <parser.hpp>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

// AST Nodes
using Tanzanite::AstNodes::InvalidNode;
using Tanzanite::AstNodes::FunctionNode;
using Tanzanite::AstNodes::FunctionParamNode;

namespace Tanzanite::Parser {
    void Parser::parse() {
        Token tk = this->lex.GenerateToken();
        while (tk.type != TokenTypes::Eof) {
            switch (tk.type) {
                case TokenTypes::Def:
                case TokenTypes::Fun:
                    this->nodes.push_back(this->parseFunction(tk));
                    break;
                case TokenTypes::Blank:
                    // this->nodes.push_back(new AstNodes::NewLine());
                    break;
                default:
                    this->nodes.push_back(new InvalidNode());
            }
            tk = this->lex.GenerateToken();
        }
    }

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
                    expect_end_bracket = false;
                    break;
                case TokenTypes::Colon:
                    if (!expect_end_bracket) {
                        Token retType = this->lex.GenerateToken();
                        node->setReturnType(retType.text);
                    }
                    break;
                case TokenTypes::Identifier: {
                    // This can be either : or =
                    Token next = this->lex.GenerateToken();
                    if (next.type == TokenTypes::Colon) {
                        Token type = this->lex.GenerateToken();
                        // now this can be tricky, because there can be either , or : or =
                        // if there is , - just do nothing
                        // if there is = add default value
                        next = this->lex.GenerateToken();
                        // add param but with empty default value
                        Token empty;
                        empty.text = "";
                        empty.type = TokenTypes::Blank;

                        if (next.type == TokenTypes::Assing) {
                            next = this->lex.GenerateToken();
                            node->addParam(current.text, new FunctionParamNode(next, new std::string(type.text)));
                        } else {
                            node->addParam(current.text, new FunctionParamNode(empty, new std::string(type.text)));
                            current = next;
                            continue;
                        }
                    } else if (next.type == TokenTypes::Assing) {
                        Token value = this->lex.GenerateToken();
                        node->addParam(current.text, new FunctionParamNode(value, nullptr));
                    } else {
                        // Raise exception
                    }
                    break;
                }
                case TokenTypes::Comma:
                    break;
                default:
                    continue;
            }
            current = this->lex.GenerateToken();
        }
    }
    
    AstNode* Parser::parseFunction(Token current) {
        Token name = this->lex.GenerateToken();
        if (name.type != TokenTypes::Identifier) return nullptr;

        FunctionNode *node = new FunctionNode(name.text);

        this->handleFunctionTop(node);

        printf("Name: %s\n", node->getName().c_str());
        auto params = node->getParams();
        for (const auto& param : params) {
            printf("Param: %s\tType: %s\t HasDefault?: %d\n",
                    param.first.c_str(), param.second->getType().c_str(),
                    param.second->getText().length() ? 1 : 0);
        }

        return node;
    }
}
