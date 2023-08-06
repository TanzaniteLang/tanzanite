#pragma once

#include <map>
#include <string>
#include <tokens.hpp>
#include <vector>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

namespace Tanzanite::AstNodes {
    class AstNode {
        protected:
            std::string nodeName;
        public:
            virtual std::string stringify() {
                return this->nodeName;
            }
    };

    class InvalidNode: public AstNode {
        public:
            InvalidNode() {
                this->nodeName = "InvalidNode";
            }
    };

    class ValueNode: public AstNode {
        private:
            std::string value;
        public:
            ValueNode(std::string val): value(val) {
                this->nodeName = "ValueNode";
            }

            std::string stringify() override {
                return this->getValue();
            }

            std::string getValue() { return this->value; }
    };

    class VariableNode: public AstNode {
        private:
            std::string type;
            std::string text;
        public:
            VariableNode(Token* token, std::string *type);
            std::string getType() { return this->type; }
            std::string getText() { return this->text; }
    };

    class BlockNode: public AstNode {
        private:
            std::vector<AstNode*> lines;
        public:
            BlockNode() {
                this->nodeName = "BlockNode";
            }

            void addLine(AstNode* node) {
                this->lines.push_back(node);
            }

            std::vector<AstNode*> getLines() { return this->lines; }
    };

    class BracketNode: protected BlockNode {
        public:
            BracketNode() {
                this->nodeName = "BracketNode";
            }

            void addNode(AstNode* node) {
                this->addLine(node);
            }
            std::vector<AstNode*> getNodes() { return this->getLines(); }
    };

    class ExpressionNode: public BracketNode {
        public:
            ExpressionNode() {
                this->nodeName = "ExpressionNode";
            }
    };

    class FunctionParamNode: public VariableNode {
        public:
            FunctionParamNode(Token *token, std::string *type): VariableNode(token, type) {
                this->nodeName = "FunctionParamNode";
            }
    };

    class OperatorNode: public AstNode {
        private:
            std::string opr;
        public:
            OperatorNode(std::string operat): opr(operat) {
                this->nodeName = "OperatorNode";
            }

            std::string stringify() override {
                return this->getOperator();
            }

            std::string getOperator() { return this->opr; }
    };

    class FunctionNode: public AstNode {
        private:
            std::string name;
            std::string returnType;
            std::map<std::string, FunctionParamNode*> params;
            BlockNode body;
        public:
            FunctionNode(std::string name) {
                this->name = name;
                this->nodeName = "FunctionNode";
            }

            void setReturnType(std::string type) {
                this->returnType = type;
            }

            void addParam(std::string name, FunctionParamNode *node) {
                this->params[name] = node;
            }

            BlockNode* getBody() { return &this->body; }

            std::string getReturnType() { return this->returnType; }
            std::string getName() { return this->name; }
            std::map<std::string, FunctionParamNode*> getParams() { return this->params; }
    };
}
