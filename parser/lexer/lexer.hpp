#pragma once

#include <tokens.hpp>
#include <string>
#include <vector>
#include <map>

extern std::map<std::string, Tanzanite::Tokens::TokenTypes> tznTokens;

namespace Tanzanite::Lexer {
    class Lexer {
        private:
            std::string text;
            int pos;
            int len;
            Tanzanite::Tokens::TokenLocation location;

        public:
            Lexer(std::string str): text(str), len(str.length()) {
                this->location.line = 1;
                this->location.col = 0;
            }
            char ReadChar();
            void StepBack();
            void SkipBlank();
            void SkipComment();
            Tanzanite::Tokens::Token ConsumeIdentifier();
            Tanzanite::Tokens::Token ConsumeNumber();
            Tanzanite::Tokens::Token ConsumeString();
            Tanzanite::Tokens::Token ConsumeChar();
            Tanzanite::Tokens::Token GenerateToken();
    };
}
