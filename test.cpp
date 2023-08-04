#include <cstdio>
#include <parser.hpp>

int main() {
    Tanzanite::Parser::Parser p(R"(xd: String = 64
)");
    
    p.parse();
    p.print();
    return 0;
}
