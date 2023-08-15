#include "ast/nodes.hpp"
#include "tokens.hpp"
#include <cstdio>
#include <iterator>
#include <parser.hpp>
#include <string>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

// AST Nodes
using Tanzanite::AstNodes::InvalidNode;

namespace Tanzanite::Parser {
    std::string stringifyType(Token tk) {
        switch (tk.type) {
            case TokenTypes::String:
                return "String";
            case TokenTypes::Char:
                return "Char";
            case TokenTypes::Int:
                return "Int";
            case TokenTypes::Float:
                return "Float";
            case TokenTypes::Bool:
                return "Bool";
            default:
                return "unknown";
        }
    }

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
                    this->nodes.push_back(this->parseFunction(tk));
                    break;
                case TokenTypes::Blank:
                    // this->nodes.push_back(new AstNodes::Emt());
                    break;
                case TokenTypes::If:
                case TokenTypes::Unless:
                    this->nodes.push_back(this->parseCondition(tk));
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

    void Parser::registerIdentifier(std::string name, Identifier i) {
        this->globalScope[name] = i;
    }
 
    void Parser::skipSemicolon(bool blanks) {
        Token cur = this->lex.GenerateToken();
        while (cur.type == TokenTypes::Semicolon || (blanks && cur.type == TokenTypes::Blank)) {
            cur = this->lex.GenerateToken();
        }
        this->lex.StepBack(cur.text.length());
    }
}
