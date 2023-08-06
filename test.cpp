#include <cstdio>
#include <parser.hpp>

int main() {
    Tanzanite::Parser::Parser p(R"(
xd: String
lol = 69
nice: Float = 3.14
x = 5 + 0 // 7 + 4 + 3
)");
    
    p.parse();
    return 0;
}
