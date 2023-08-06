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
using Tanzanite::AstNodes::ValueNode;
using Tanzanite::AstNodes::VariableNode;
using Tanzanite::AstNodes::OperatorNode;
using Tanzanite::AstNodes::FunctionNode;

namespace Tanzanite::Parser {
    void bustError(std::string msg, std::string info) {
        if (info.length()) {
            printf("\033[31;1mBUSTED\033[0m: %s => %s\n", msg.c_str(), info.c_str());
        } else {
            printf("\033[31;1mBUSTED\033[0m: %s\n", msg.c_str());
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
                        bustError("Bracket not closed", "");
                    }
                    break;
                case TokenTypes::Identifier: {
                    VariableNode *var = (VariableNode*) this->parseVariable(current);
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

    bool checkNumericalType(const Token& type) {
        const std::string& t = type.text;

        switch (type.type) {
            case TokenTypes::Int:
            case TokenTypes::Char:
            case TokenTypes::Float:
            case TokenTypes::Bool:
                return true;
            case TokenTypes::LBracket:
                bustError("Brackets soon get implementation", "");
                return false;
            default:
                if (type.type == TokenTypes::Identifier) {
                    bustError("Custom types not supported yet", type.text);
                } else {
                    bustError("Type cannot be used with operators", t);
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
            case TokenTypes::Char:
            case TokenTypes::Int:
            case TokenTypes::Float:
            case TokenTypes::Bool: {
                node->addLine(new ValueNode(begin.text));
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
                node->addLine(new ValueNode(begin.text));
                break;
            default:
                bustError("Not a value or identifier", begin.text);
        }

        return node;
    }

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
                node->addLine(new ValueNode(nextVal.text));

                return node;
            }
            default:
                this->lex.StepBack(op.text.length());
                return nullptr;
        }
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
                if (next.type == TokenTypes::Assing) {
                    AstNode *node = this->parseValue();
                    return new VariableNode(&val, &type.text, node);
                } else {
                    this->lex.StepBack(next.text.length());
                    return new VariableNode(&val, &type.text, nullptr);
                }
                break;
            } case TokenTypes::Assing: {
                AstNode *node = this->parseValue();
                return new VariableNode(&val, nullptr, node);
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

        printf("Fn name: %s\n", node->getName().c_str());
        auto params = node->getParams();
        for (const auto& param : params) {
            printf("Fn param: %s\tType: %s\t HasDefault?: %d\n",
                    param.first.c_str(), param.second->getType().c_str(),
                    param.second->isDeclaration() ? false : true);
        }

        return node;
    }
}
