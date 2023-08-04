#include <cstdio>
#include <lexer/lexer.hpp>
#include <tokens.hpp>

using Tanzanite::Lexer::Lexer;
using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

void printToken(Token tk) {
    printf("Token (%s)\t{col: %d, line: %d}\n", tk.text.c_str(),
            tk.location.col, tk.location.line);
}

int main() {
    Lexer lex(R"(
< < <  <<  <<=
)");
    Token tk = lex.GenerateToken();
    while (tk.type != TokenTypes::Eof) {
        printToken(tk);
        tk = lex.GenerateToken();
    }
    printToken(tk);

    return 0;
}
