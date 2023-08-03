#include <cstdio>
#include <lexer.hpp>
#include <tokens.hpp>

using Tanzanite::Lexer::Lexer;
using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

void printToken(Token tk) {
    printf("Token (%s)\n", tk.text.c_str());
}

int main() {
    Lexer lex(R"(
class Customer
   @@no_of_customers = 0
   def initialize(id, name, addr)
      @cust_id = id
      # @cust_name = name
      puts "comment test"
      @cust_addr = addr
   end
end
)");
    Token tk = lex.GenerateToken();
    while (tk.type != TokenTypes::Eof) {
        printToken(tk);
        tk = lex.GenerateToken();
    }
    printToken(tk);

    return 0;
}
