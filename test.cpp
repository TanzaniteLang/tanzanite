#include <cstdio>
#include <parser.hpp>

int main() {
    Tanzanite::Parser::Parser p(R"(
hm: String
hey = "lol"
krasna: Int = 69

def lol1(nice = 69): Void
    hm: String
    hey = "lol"
    krasna: Int = 69
end

def lol2(nice: String = "idk", hey: Int = 504, f = 3.14): Void
end

def lol3 nice: String, lol = 69: Void
end
)");
    
    p.parse();
    return 0;
}
