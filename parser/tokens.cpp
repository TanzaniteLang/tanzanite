#include "tokens.hpp"

#include <map>
#include <string>

using Tanzanite::TokenTypes;

std::map<std::string, Tanzanite::TokenTypes> tokens {
    // operators
    {"+", TokenTypes::Operator},
    {"-", TokenTypes::Operator},
    {"*", TokenTypes::Operator},
    {"/", TokenTypes::Operator},
    {"%", TokenTypes::Operator},
    {"**", TokenTypes::Operator},
    {"//", TokenTypes::Operator},
    {"!", TokenTypes::Operator},
    {"~", TokenTypes::Operator},
    {"<<", TokenTypes::Operator},
    {">>", TokenTypes::Operator},
    {"&", TokenTypes::Operator},
    {"|", TokenTypes::Operator},
    {"^", TokenTypes::Operator},
    {"==", TokenTypes::Operator},
    {"!=", TokenTypes::Operator},
    {"<", TokenTypes::Operator},
    {"<=", TokenTypes::Operator},
    {">", TokenTypes::Operator},
    {">=", TokenTypes::Operator},
    {"<=>", TokenTypes::Operator},
    {"&&", TokenTypes::Operator},
    {"||", TokenTypes::Operator},
    {"=", TokenTypes::Operator},
    {"+=", TokenTypes::Operator},
    {"-=", TokenTypes::Operator},
    {"/=", TokenTypes::Operator},
    {"//=", TokenTypes::Operator},
    {"%=", TokenTypes::Operator},
    {"|=", TokenTypes::Operator},
    {"&=", TokenTypes::Operator},
    {"^=", TokenTypes::Operator},
    {"**=", TokenTypes::Operator},
    {">>=", TokenTypes::Operator},
    {"<<=", TokenTypes::Operator},
    {"||=", TokenTypes::Operator},
    {"&&=", TokenTypes::Operator},
    {"|>", TokenTypes::Operator},
    // keywords
    {"if", TokenTypes::Keyword},
    {"else", TokenTypes::Keyword},
    {"elsif", TokenTypes::Keyword},
    {"for", TokenTypes::Keyword},
    {"while", TokenTypes::Keyword},
    {"until", TokenTypes::Keyword},
    {"unless", TokenTypes::Keyword},
    {"do", TokenTypes::Keyword},
    {"end", TokenTypes::Keyword},
    {"break", TokenTypes::Keyword},
    {"case", TokenTypes::Keyword},
    {"when", TokenTypes::Keyword},
    {"true", TokenTypes::Keyword},
    {"false", TokenTypes::Keyword},
    {"def", TokenTypes::Keyword},
    {"fun", TokenTypes::Keyword},
    {"nil", TokenTypes::Keyword},
    {"return", TokenTypes::Keyword},
    {"sizeof", TokenTypes::Keyword},
    {"with", TokenTypes::Keyword},
    // delimiters
    {".", TokenTypes::Delimiter},
    {",", TokenTypes::Delimiter},
    {";", TokenTypes::Delimiter},
    {"(", TokenTypes::Delimiter},
    {")", TokenTypes::Delimiter},
    {"[", TokenTypes::Delimiter},
    {"]", TokenTypes::Delimiter},
    {"{", TokenTypes::Delimiter},
    {"}", TokenTypes::Delimiter},
};
