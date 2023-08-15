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

if 7 + 7 > 10 then
    hey = "cs"
elsif 4 + 7 < 4
    lol = "xd"
else then
    lmao = "tiramify"
end
)");
    
    p.parse();
    return 0;
}
