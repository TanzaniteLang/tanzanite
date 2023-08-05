#include "ast/nodes.hpp"
#include "tokens.hpp"
#include <cstdio>
#include <parser.hpp>
#include <string>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

// AST Nodes
using Tanzanite::AstNodes::InvalidNode;
using Tanzanite::AstNodes::BlockNode;
using Tanzanite::AstNodes::FunctionNode;
using Tanzanite::AstNodes::FunctionParamNode;

namespace Tanzanite::Parser {
    void bustError(std::string msg, std::string info) {
        if (info.length()) {
            printf("\e[31;1mBUSTED\e[0m: %s => %s\n", msg.c_str(), info.c_str());
        } else {
            printf("\e[31;1mBUSTED\e[0m: %s\n", msg.c_str());
        }
    }

    void Parser::parse() {
        Token tk = this->lex.GenerateToken();
        while (tk.type != TokenTypes::Eof) {
            switch (tk.type) {
                case TokenTypes::Def:
                case TokenTypes::Fun:
                    this->nodes.push_back(this->parseFunction());
                    break;
                case TokenTypes::Blank:
                    // this->nodes.push_back(new AstNodes::Emt());
                    break;
                case TokenTypes::Identifier:
                    this->nodes.push_back(this->parseVariable(tk));
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
                        // error
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

                        this->checkType(type); // blank type test

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
                        bustError("Invalid token?", next.text);
                    }
                    break;
                } case TokenTypes::Comma:
                    break;
                default:
                    continue;
            }
            current = this->lex.GenerateToken();
        }
    }

    bool Parser::checkType(const Token& type) {
        const std::string& t = type.text;

        if (t == "String") return true;
        else if (t == "Char") return true;
        else if (t == "Int") return true;
        else if (t == "Float") return true;
        else if (t == "Bool") return true;
        else {
            if (type.type == TokenTypes::Identifier) {
                bustError("Custom types not supported yet", type.text);
            } else {
                bustError("Unknown type", t);
            }
        }

        return false;
    }

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
            case TokenTypes::String:
                break;
            case TokenTypes::Char:
                break;
            case TokenTypes::Int:
                break;
            case TokenTypes::Float:
                break;
            case TokenTypes::Bool:
                break;
            case TokenTypes::Identifier:
                break;
            default:
                bustError("Not a value or identifier", begin.text);
        }

        return node;
    }

    AstNode* Parser::parseVariable(Token val) {
        Token next = this->lex.GenerateToken();
        switch (next.type) {
            case TokenTypes::Colon: {
                Token type = this->lex.GenerateToken();
                if (!this->checkType(type)) {
                    return new InvalidNode();
                }
                next = this->lex.GenerateToken();
                if (next.type == TokenTypes::Semicolon || next.type == TokenTypes::Blank) {
                    return new InvalidNode();
                } else if (next.type == TokenTypes::Assing) {
                    AstNode *node = this->parseValue();
                }
                break;
            } case TokenTypes::Assing: {
                AstNode *node = this->parseValue();
                break;
            } default:
                bustError("Invalid token! Expected = or : but got", next.text + " instead");
                return new InvalidNode();
        }
        // xd: String
        // xd = "Xd"
        // xd: String = if "77".to_i.to_s == "77" ? 5 : 69
        return new InvalidNode();
    }
    
    AstNode* Parser::parseFunction() {
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
