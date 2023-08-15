#pragma once

#include <cstdio>
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
            Token tk;
        public:
            ValueNode(std::string val, Token *tok): value(val) {
                if (tok) this->tk = *tok;
                this->nodeName = "ValueNode";
            }

            std::string stringify() override {
                return this->getValue();
            }

            Token getToken() { return this->tk; }

            std::string getValue() { return this->value; }
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

    class VariableNode: public AstNode {
        private:
            std::string type;
            std::string name;
            AstNode* value;
        public:
            VariableNode(Token* token, std::string *type, AstNode *value);
            std::string getType() { return this->type; }
            std::string getName() { return this->name; }
            AstNode *getValue() { return this->value; }
            bool isDeclaration() { return this->value == nullptr; }

            std::string stringify() override {
                return getName() + ", " + getType();
            }
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

    class ConditionNode: public AstNode {
        private:
            std::vector<AstNode*> chain;
        public:
            ConditionNode() {
                this->nodeName = "ConditionNode";
            }

            void addToChain(AstNode *condition) { this->chain.push_back(condition); }
            std::vector<AstNode*>& getChain() { return this->chain; }
    };

    class IfNode: public AstNode {
        private:
            ValueNode* val;
            BlockNode body;
            bool negated;
        public:
            IfNode(ValueNode *val, bool isUnless) {
                this->nodeName = "IfNode";
                this->val = val;
                this->negated = isUnless;
            }

            bool isUnless() { return this->negated; }

            AstNode *getValue() { return this->val; }
            BlockNode* getBody() { return &this->body; }
    };

    class ElsifNode: public AstNode {
        private:
            ValueNode* val;
            BlockNode body;
            bool negated;
        public:
            ElsifNode(ValueNode *val, bool negatedByUnless) {
                this->nodeName = "ElsifNode";
                this->val = val;
                this->negated = negatedByUnless;
            }

            bool isNegated() { return this->negated; }

            BlockNode* getBody() { return &this->body; }
            AstNode *getValue() { return this->val; }
    };

    class ElseNode: public AstNode {
        private:
            BlockNode body;
        public:
            ElseNode() {
                this->nodeName = "ElseNode";
            }

            BlockNode* getBody() { return &this->body; }
    };

    class FunctionNode: public AstNode {
        private:
            std::string name;
            std::string returnType;
            std::map<std::string, VariableNode*> params;
            BlockNode body;
            bool cFunc;
        public:
            FunctionNode(std::string name, bool isCFunc) {
                this->name = name;
                this->nodeName = "FunctionNode";
                this->cFunc = isCFunc;
            }

            void setReturnType(std::string type) {
                this->returnType = type;
            }

            void addParam(std::string name, VariableNode *node) {
                this->params[name] = node;
            }

            BlockNode* getBody() { return &this->body; }

            bool hasBody() { return this->body.getLines().size(); }
            bool isCFunc() { return this->cFunc; }

            std::string getReturnType() { return this->returnType; }
            std::string getName() { return this->name; }
            std::map<std::string, VariableNode*> getParams() { return this->params; }
    };
}
