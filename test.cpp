#include <cstdio>
#include <parser.hpp>

int main() {
    Tanzanite::Parser::Parser p(R"(
# declarations
# declarations must have return type!
def foo: Void; end
def foo2(): Void; end

def foo3: Void
end

def foo4(): Void
end

# declarations with params
def baz a: Int : Void; end
def baz2(a: Int): Void; end

def baz3 a: Int : Void
end

def baz4(a: Int): Void
end

# declarations with params and default values
def bar a: Int = 7 : Void; end
def bar2(a: Int = 7): Void; end

def bar3 a: Int = 7 : Void
end

def bar4(a: Int = 7, b: Char): Void
end
)");
    
    p.parse();
    return 0;
}
