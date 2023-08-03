#include "tokens.hpp"

#include <map>
#include <string>

using Tanzanite::Tokens::TokenTypes;

std::map<std::string, TokenTypes> tznTokens {
    // operators
    {"=", TokenTypes::Assing},
    {"+", TokenTypes::Plus},
    {"-", TokenTypes::Minus},
    {"*", TokenTypes::Asterisk},
    {"/", TokenTypes::Slash},
    {"%", TokenTypes::Modulo},
    {"!", TokenTypes::Bang},
    {"~", TokenTypes::Tilda},
    {"&", TokenTypes::Ampersand},
    {"|", TokenTypes::Pipe},
    {"^", TokenTypes::Caret},
    {"<", TokenTypes::LessThan},
    {">", TokenTypes::GreaterThan},
    // Satements
    {"if", TokenTypes::If},
    {"else", TokenTypes::Else},
    {"elsif", TokenTypes::Elsif},
    {"for", TokenTypes::For},
    {"while", TokenTypes::While},
    {"until", TokenTypes::Until},
    {"unless", TokenTypes::Unless},
    {"break", TokenTypes::Break},
    {"case", TokenTypes::Case},
    {"when", TokenTypes::When},
    {"true", TokenTypes::Bool},
    {"false", TokenTypes::Bool},
    {"def", TokenTypes::Def},
    {"fun", TokenTypes::Fun},
    {"nil", TokenTypes::Nil},
    {"return", TokenTypes::Return},
    {"sizeof", TokenTypes::Sizeof},
    {"with", TokenTypes::With},
    // delimiters
    {"?", TokenTypes::QuestionMark},
    {".", TokenTypes::Do},
    {",", TokenTypes::Comma},
    {":", TokenTypes::Colon},
    {";", TokenTypes::Semicolon},
    {"(", TokenTypes::LBracket},
    {")", TokenTypes::RBracket},
    {"[", TokenTypes::LSquareBracket},
    {"]", TokenTypes::RSquareBracket},
    {"{", TokenTypes::LSquiglyBracket},
    {"}", TokenTypes::RSquiglyBracket},
    {"do", TokenTypes::Do},
    {"end", TokenTypes::End},
    {"\n", TokenTypes::Blank}
};
