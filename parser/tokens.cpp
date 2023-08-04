#include "tokens.hpp"

#include <map>
#include <string>

using Tanzanite::Tokens::TokenTypes;

std::map<std::string, TokenTypes> tznTokens {
    // operators
    {"=", TokenTypes::Assing},
    {"==", TokenTypes::Equals},
    {"+", TokenTypes::Plus},
    {"+=", TokenTypes::PlusAssign},
    {"-", TokenTypes::Minus},
    {"-=", TokenTypes::MinusAssign},
    {"*", TokenTypes::Asterisk},
    {"*=", TokenTypes::AsteriskAssign},
    {"**", TokenTypes::DoubleAsterisk},
    {"**=", TokenTypes::DoubleAsteriskAssign},
    {"/", TokenTypes::Slash},
    {"/=", TokenTypes::SlashAssign},
    {"//", TokenTypes::DoubleSlash},
    {"//=", TokenTypes::DoubleSlashAssign},
    {"%", TokenTypes::Modulo},
    {"%=", TokenTypes::ModuloAssign},
    {"!", TokenTypes::Bang},
    {"!=", TokenTypes::NotEquals},
    {"~", TokenTypes::Tilda},
    {"~=", TokenTypes::TildaAssign},
    {"&", TokenTypes::Ampersand},
    {"&=", TokenTypes::Ampersand},
    {"&&", TokenTypes::And},
    {"|", TokenTypes::Pipe},
    {"|=", TokenTypes::PipeAssign},
    {"||", TokenTypes::Or},
    {"|>", TokenTypes::PipeTo},
    {"^", TokenTypes::Caret},
    {"^=", TokenTypes::CaretAssign},
    {"<", TokenTypes::Less},
    {"<=", TokenTypes::LessEquals},
    {"<<", TokenTypes::LeftShift},
    {"<<=", TokenTypes::LeftShiftAssign},
    {"<=>", TokenTypes::Spaceship},
    {">", TokenTypes::Greater},
    {">=", TokenTypes::GreaterEquals},
    {">>", TokenTypes::RightShift},
    {">>=", TokenTypes::RightShiftAssign},
    // Satements
    {"if", TokenTypes::If},
    {"else", TokenTypes::Else},
    {"elsif", TokenTypes::Elsif},
    {"for", TokenTypes::For},
    {"while", TokenTypes::While},
    {"until", TokenTypes::Until},
    {"unless", TokenTypes::Unless},
    {"break", TokenTypes::Break},
    {"next", TokenTypes::Next},
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
    {"begin", TokenTypes::Begin},
    {"in", TokenTypes::In},
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
    {"then", TokenTypes::Then},
    {"\n", TokenTypes::Blank}
};
