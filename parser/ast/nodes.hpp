#pragma once

#include <string>
#include <tokens.hpp>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

namespace Tanzanite::AstNodes {
    class AstNode {
        protected:
            std::string nodeName;
        public:
            std::string stringify() {
                return this->nodeName;
            }
    };

    class VariableDeclaration: public AstNode {
        protected:
            std::string variableName;
            std::string variableType;
        public:
            VariableDeclaration(Token name, Token type) {
                this->nodeName = "VariableDeclaration";
                this->variableName = name.text;
                this->variableType = type.text;
            }
    };

    class VariableInitialization: public VariableDeclaration {
        private:
            std::string variableValue;
        public:
            VariableInitialization(VariableDeclaration decl, Token value): VariableDeclaration(decl) {
                this->variableValue = value.text;
                this->nodeName = "VariableInitialization";
            }
    };

    class VariableDefinition: public AstNode {
        private:
            std::string variableName;
            std::string variableType;
            std::string variableValue;
        public:
            VariableDefinition(Token name, Token value) {
                this->nodeName = "VariableDefinition";
                this->variableName = name.text;
                switch (value.type) {
                    case TokenTypes::String:
                        this->variableType = "String";
                        break;
                    case TokenTypes::Char:
                        this->variableType = "Char";
                        break;
                    case TokenTypes::Int:
                        this->variableType = "Int";
                        break;
                    case TokenTypes::Float:
                        this->variableType = "Float";
                        break;
                    case TokenTypes::Bool:
                        this->variableType = "Bool";
                        break;
                    default:
                        this->variableType = "INVALID";
                        break;
                }
            }
    };
}
