#pragma once

#include "tokens.hpp"
#include <string>

namespace Tanzanite::Lexer {
    class Lexer {
        private:
            std::string text;
            int pos;
            int len;
            Tanzanite::Tokens::TokenLocation location;

        public:
            Lexer(std::string str): text(str), len(str.length()) {}
            char ReadChar();
            void StepBack();
            void SkipWhite();
            void ConsumeIdentifier();
            void ConsumeNumber();
            void ConsumeString();
            void ConsumeChar();
            Tanzanite::Tokens::Token GenerateToken();
    };
}
