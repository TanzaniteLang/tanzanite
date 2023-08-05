#pragma once

#include <tokens.hpp>
#include <string>
#include <vector>
#include <map>

extern std::map<std::string, Tanzanite::Tokens::TokenTypes> tznTokens;
using Tanzanite::Tokens::Token;

namespace Tanzanite::Lexer {
    class Lexer {
        private:
            std::string text;
            int pos;
            int len;
            Tanzanite::Tokens::TokenLocation location;

        public:
            Lexer(std::string str): text(str), len(str.length()) {
                this->pos = 0;
                this->location.line = 1;
                this->location.col = 0;
            }
            char ReadChar();
            void StepBack();
            void StepBack(int count);
            void SkipBlank();
            void SkipComment();
            Token handleThrees(char current);
            Token handleTwos(char current);
            Token ConsumeIdentifier();
            Token ConsumeNumber();
            Token ConsumeString();
            Token ConsumeTick();
            Token ConsumeChar();
            Token GenerateToken();
    };
}
