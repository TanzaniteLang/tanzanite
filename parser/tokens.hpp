#pragma once

#include <string>

namespace Tanzanite::Tokens {
    enum class TokenTypes {
        Eof = -1,
        Whitespace,
        Illegal,

        // Identifier and literals
        Identifier,
        String,
        Char,
        Int,
        Float,
        Bool,
        Nil,

        // Statement
        Assing,
        If,
        Else,
        Elsif,
        For,
        While,
        Unless,
        Until,
        Break,
        Case,
        When,
        With,

        // Operators
        Plus,
        Minus,
        Asterisk,
        Slash,
        Modulo,
        Bang,
        Tilda,
        Ampersand,
        Pipe,
        Caret,
        LessThan,
        GreaterThan,

        // Delimiters
        QuestionMark,
        Dot,
        Comma,
        Colon,
        Semicolon,
        LBracket,
        RBracket,
        LSquareBracket,
        RSquareBracket,
        LSquiglyBracket,
        RSquiglyBracket,
        Do,
        End,
        Blank,

        // Reserved keywords
        Def,
        Fun,
        Return,
        Sizeof,
    };

    typedef struct {
        int line;
        int col;
    } TokenLocation;

    typedef struct {
        TokenTypes type;
        std::string text;
        TokenLocation location;
    } Token;
}
