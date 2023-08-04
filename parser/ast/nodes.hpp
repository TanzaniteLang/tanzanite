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
            std::string stringify() {
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
            std::string type;
            std::string text;
        public:
            ValueNode(Token token, std::string *type);
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

    class FunctionParamNode: public ValueNode {
        public:
            FunctionParamNode(Token token, std::string *type): ValueNode(token, type) {
                this->nodeName = "FunctionParamNode";
            }
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
