// Inspiration took from https://github.com/xHyroM/frog/blob/old/frog/src/lexer/mod.rs

#include "lexer.hpp"
#include "tokens.hpp"
#include <cctype>
#include <vector>

using Tanzanite::Tokens::Token;
using Tanzanite::Tokens::TokenTypes;

extern std::map<std::string, Tanzanite::Tokens::TokenTypes> tznTokens;

namespace Tanzanite::Lexer {
    char Lexer::ReadChar() {
        if (this->pos - 1 == this->len) return 0;
        char return_value = this->text[this->pos];
        this->pos++;

        return return_value;
    }
    
    void Lexer::StepBack() {
        this->pos--;
    }

    void Lexer::SkipBlank() {
        char current = this->ReadChar();
        while (current == ' ' || current == '\t') {
            current = this->ReadChar();
        }
        this->StepBack();
    }

    void Lexer::SkipComment() {
        char current = this->ReadChar();
        while (current != '\n') current = this->ReadChar();
    }

    Token Lexer::ConsumeIdentifier() {
        Token tkn;
        tkn.type = TokenTypes::Identifier;
        std::string val = "";

        char consumed = this->ReadChar();

        while (isdigit(consumed) || isalpha(consumed) || consumed == '_' || consumed == '@') {
            val += consumed;
            consumed = this->ReadChar();
        }

        this->StepBack();
        if (tznTokens.find(val) != tznTokens.end()) tkn.type = tznTokens[val];
        tkn.location = this->location;
        tkn.text = val;
        return tkn;
    }

    Token Lexer::ConsumeNumber() {
        Token tkn;
        std::string val = "";
        bool is_float = false;

        char consumed = this->ReadChar();

        while (isdigit(consumed) || consumed == '.') {
            val += consumed;
            consumed = this->ReadChar();
            if (consumed == '.') is_float = true;
        }

        this->StepBack();
        tkn.type = is_float ? TokenTypes::Float : TokenTypes::Int;
        tkn.location = this->location;
        tkn.text = val;
        return tkn;
    }

    Token Lexer::ConsumeString() {
        Token tkn;
        tkn.type = TokenTypes::String;

        char consumed = this->ReadChar();
        std::string str = "";
        str += consumed;

        while (true) {
            consumed = this->ReadChar();
            if (consumed == 0) break;
            if (consumed == '\\' && this->ReadChar() == '"') {
                str += "\\\"";
                continue;
            }
            str += consumed;
            if (consumed == '"') break;
        }

        tkn.text = str;
        tkn.location = this->location;
        return tkn;
    }

    Token Lexer::ConsumeChar() {
        Token tkn;
        tkn.type = TokenTypes::Char;

        std::string val = "";
        for (int i = 0; i < 3; i++) {
            val += this->ReadChar();
        }

        tkn.text = val;
        tkn.location = this->location;
        return tkn;
    }

    Token Lexer::handleTwos(char current) {
        Token tkn;

        char next = this->ReadChar();

        switch (current) {
            case '=':
                if (next == '=') {
                    tkn.text = "==";
                    tkn.type = TokenTypes::Equals;
                    tkn.location = this->location;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Assing;
                tkn.text = "=";
                tkn.location = this->location;
                break;
            case '+':
                if (next == '=') {
                    tkn.text = "+=";
                    tkn.type = TokenTypes::PlusAssign;
                    tkn.location = this->location;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "+";
                tkn.location = this->location;
                tkn.type = TokenTypes::Plus;
                break;
            case '-':
                if (next == '=') {
                    tkn.text = "-=";
                    tkn.type = TokenTypes::MinusAssign;
                    tkn.location = this->location;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "-";
                tkn.location = this->location;
                tkn.type = TokenTypes::Minus;
                break;
            case '%':
                if (next == '=') {
                    tkn.text = "%=";
                    tkn.type = TokenTypes::ModuloAssign;
                    tkn.location = this->location;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "%";
                tkn.location = this->location;
                tkn.type = TokenTypes::Modulo;
                break;
            case '!':
                if (next == '=') {
                    tkn.text = "!=";
                    tkn.type = TokenTypes::NotEquals;
                    tkn.location = this->location;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Bang;
                tkn.text = "!";
                tkn.location = this->location;
                break;
            case '~':
                if (next == '=') {
                    tkn.text = "~=";
                    tkn.type = TokenTypes::TildaAssign;
                    tkn.location = this->location;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "~";
                tkn.location = this->location;
                tkn.type = TokenTypes::Tilda;
                break;
            case '^':
                if (next == '=') {
                    tkn.text = "^=";
                    tkn.type = TokenTypes::CaretAssign;
                    tkn.location = this->location;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Caret;
                tkn.text = "^";
                tkn.location = this->location;
                break;
        }

        return tkn;
    }

    Token Lexer::GenerateToken() {
        this->SkipBlank();
        Token tkn;
        char tok = this->ReadChar();

        switch (tok) {
            case '=': // =, ==
                tkn = this->handleTwos(tok);
                break;
            case '+': // +, +=
                tkn = this->handleTwos(tok);
                break;
            case '-': // -, -=
                tkn = this->handleTwos(tok);
                break;
            case '*': // *, *=, **, **=
                tkn.type = TokenTypes::Asterisk;
                tkn.text = "*";
                tkn.location = this->location;
                break;
            case '/': // /, /=, //, //=
                tkn.type = TokenTypes::Slash;
                tkn.text = "/";
                tkn.location = this->location;
                break;
            case '#':
                this->SkipComment();
                tkn = this->GenerateToken();
                break;
            case '%': // %, %=
                tkn = this->handleTwos(tok);
                break;
            case '!': // !, !=
                tkn = this->handleTwos(tok);
                break;
            case '~': // ~, ~=
                tkn = this->handleTwos(tok);
                break;
            case '&': // &, &=, &&
                tkn.type = TokenTypes::Ampersand;
                tkn.text = "&";
                tkn.location = this->location;
                break;
            case '|': // |, |=, ||, |>
                tkn.type = TokenTypes::Pipe;
                tkn.text = "|";
                tkn.location = this->location;
                break;
            case '^': // ^, ^=
                tkn = this->handleTwos(tok);
                break;
            case '<': // <, <=, <<, <<=
                tkn.type = TokenTypes::Less;
                tkn.text = "<";
                tkn.location = this->location;
                break;
            case '>': // >, >=, >>, >>=
                tkn.type = TokenTypes::Greater;
                tkn.text = ">";
                tkn.location = this->location;
                break;
            case '?':
                tkn.type = TokenTypes::QuestionMark;
                tkn.text = "?";
                tkn.location = this->location;
                break;
            case '.':
                tkn.type = TokenTypes::Dot;
                tkn.text = ".";
                tkn.location = this->location;
                break;
            case ',':
                tkn.type = TokenTypes::Comma;
                tkn.text = ",";
                tkn.location = this->location;
                break;
            case ':':
                tkn.type = TokenTypes::Colon;
                tkn.text = ":";
                tkn.location = this->location;
                break;
            case ';':
                tkn.type = TokenTypes::Semicolon;
                tkn.text = ";";
                tkn.location = this->location;
                break;
            case '(':
                tkn.type = TokenTypes::LBracket;
                tkn.text = "(";
                tkn.location = this->location;
                break;
            case ')':
                tkn.type = TokenTypes::RBracket;
                tkn.text = ")";
                tkn.location = this->location;
                break;
            case '[':
                tkn.type = TokenTypes::LSquareBracket;
                tkn.text = "[";
                tkn.location = this->location;
                break;
            case ']':
                tkn.type = TokenTypes::RSquareBracket;
                tkn.text = "]";
                tkn.location = this->location;
                break;
            case '{':
                tkn.type = TokenTypes::LSquiglyBracket;
                tkn.text = "{";
                tkn.location = this->location;
                break;
            case '}':
                tkn.type = TokenTypes::RSquiglyBracket;
                tkn.text = "}";
                tkn.location = this->location;
                break;
            case '\n':
                tkn.type = TokenTypes::Blank;
                tkn.text = "\\n";
                tkn.location = this->location;
                break;
            case '"':
                this->StepBack();
                tkn = this->ConsumeString();
                break;
            case '\'':
                this->StepBack();
                tkn = this->ConsumeChar();
                break;
            case 0:
                tkn.location = this->location;
                tkn.text = "EOF";
                tkn.type = TokenTypes::Eof;
                break;
            default:
                if (isalpha(tok) || tok == '_' || tok == '@') {
                    this->StepBack();
                    tkn = this->ConsumeIdentifier();
                } else if (isdigit(tok)) {
                    this->StepBack();
                    tkn = this->ConsumeNumber();
                } else {
                    tkn.location = this->location;
                    tkn.text = "";
                    tkn.type = TokenTypes::Illegal;
                }
        }

        return tkn;
    }
}
