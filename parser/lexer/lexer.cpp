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
        this->location.col++;

        return return_value;
    }
    
    void Lexer::StepBack() {
        this->pos--;
        this->location.col--;
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
        tkn.location = this->location;
        std::string val = "";

        char consumed = this->ReadChar();

        while (isdigit(consumed) || isalpha(consumed) || consumed == '_' || consumed == '@') {
            val += consumed;
            consumed = this->ReadChar();
        }

        this->StepBack();
        if (tznTokens.find(val) != tznTokens.end()) tkn.type = tznTokens[val];
        tkn.text = val;
        return tkn;
    }

    Token Lexer::ConsumeNumber() {
        Token tkn;
        tkn.location = this->location;
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
        tkn.text = val;
        return tkn;
    }

    Token Lexer::ConsumeString() {
        Token tkn;
        tkn.type = TokenTypes::String;

        tkn.location = this->location;
        char consumed = this->ReadChar();
        std::string str = "";
        str += consumed;

        while (true) {
            consumed = this->ReadChar();
            char next = this->ReadChar();
            if (consumed == 0) break;
            if (consumed == '\\' && next == '"') {
                str += "\\\"";
                continue;
            }
            this->StepBack();
            str += consumed;
            if (consumed == '"') break;
        }

        tkn.text = str;
        return tkn;
    }

    Token Lexer::ConsumeTick() {
        Token tkn;
        tkn.type = TokenTypes::String;

        tkn.location = this->location;
        char consumed = this->ReadChar();
        std::string str = "";
        str += consumed;

        while (true) {
            consumed = this->ReadChar();
            char next = this->ReadChar();
            if (consumed == 0) break;
            if (consumed == '\\' && next == '`') {
                str += "\\`";
                continue;
            }
            this->StepBack();
            str += consumed;
            if (consumed == '`') break;
        }

        tkn.text = str;
        return tkn;
    }

    Token Lexer::ConsumeChar() {
        Token tkn;
        tkn.type = TokenTypes::Char;
        tkn.location = this->location;

        std::string val = "";
        for (int i = 0; i < 3; i++) {
            val += this->ReadChar();
        }

        if (val[0] == val[1]) {
            tkn.text = "INVALID_CHAR";
            tkn.type = TokenTypes::Eof;
            return tkn;
        }

        tkn.text = val;
        return tkn;
    }

    Token Lexer::handleTwos(char current) {
        Token tkn;

        tkn.location = this->location;
        tkn.location.col--;
        char next = this->ReadChar();

        switch (current) {
            case '=':
                if (next == '=') {
                    tkn.text = "==";
                    tkn.type = TokenTypes::Equals;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Assing;
                tkn.text = "=";
                break;
            case '+':
                if (next == '=') {
                    tkn.text = "+=";
                    tkn.type = TokenTypes::PlusAssign;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "+";
                tkn.type = TokenTypes::Plus;
                break;
            case '-':
                if (next == '=') {
                    tkn.text = "-=";
                    tkn.type = TokenTypes::MinusAssign;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "-";
                tkn.type = TokenTypes::Minus;
                break;
            case '%':
                if (next == '=') {
                    tkn.text = "%=";
                    tkn.type = TokenTypes::ModuloAssign;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "%";
                tkn.type = TokenTypes::Modulo;
                break;
            case '!':
                if (next == '=') {
                    tkn.text = "!=";
                    tkn.type = TokenTypes::NotEquals;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Bang;
                tkn.text = "!";
                break;
            case '~':
                if (next == '=') {
                    tkn.text = "~=";
                    tkn.type = TokenTypes::TildaAssign;
                    return tkn;
                }
                this->StepBack();
                tkn.text = "~";
                tkn.type = TokenTypes::Tilda;
                break;
            case '^':
                if (next == '=') {
                    tkn.text = "^=";
                    tkn.type = TokenTypes::CaretAssign;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Caret;
                tkn.text = "^";
                break;
            case '&':
                if (next == '=') {
                    tkn.text = "&=";
                    tkn.type = TokenTypes::AmpersandAssign;
                    return tkn;
                } else if (next == '&') {
                    tkn.text = "&&";
                    tkn.type = TokenTypes::And;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Ampersand;
                tkn.text = "&";
                break;
            case '|':
                if (next == '=') {
                    tkn.text = "|=";
                    tkn.type = TokenTypes::PipeAssign;
                    return tkn;
                } else if (next == '|') {
                    tkn.text = "||";
                    tkn.type = TokenTypes::Or;
                    return tkn;
                } else if (next == '>') {
                    tkn.text = "|>";
                    tkn.type = TokenTypes::PipeTo;
                    return tkn;
                }
                this->StepBack();
                tkn.type = TokenTypes::Pipe;
                tkn.text = "|";
                break;
        }

        return tkn;
    }

    Token Lexer::handleThrees(char current) {
        Token tkn;

        tkn.location = this->location;
        tkn.location.col--;
        char next = this->ReadChar();
        char nnext = this->ReadChar();
        switch (current) {
            case '*':
                if (next == '=') {
                    tkn.text = "*=";
                    tkn.type = TokenTypes::AsteriskAssign;
                    this->StepBack();
                    return tkn;
                } else if (next == '*') {
                    tkn.text = "**";
                    tkn.type = TokenTypes::DoubleAsterisk;
                    if (nnext == '=') {
                        tkn.text = "**=";
                        tkn.type = TokenTypes::DoubleAsteriskAssign;
                    } else this->StepBack();
                    return tkn;
                }
                this->StepBack();
                this->StepBack();
                tkn.type = TokenTypes::AsteriskAssign;
                tkn.text = "*";
                break;
            case '/':
                if (next == '=') {
                    tkn.text = "/=";
                    tkn.type = TokenTypes::SlashAssign;
                    this->StepBack();
                    return tkn;
                } else if (next == '/') {
                    tkn.text = "//";
                    tkn.type = TokenTypes::DoubleSlash;
                    if (nnext == '=') {
                        tkn.text = "//=";
                        tkn.type = TokenTypes::DoubleSlashAssign;
                    } else this->StepBack();
                    return tkn;
                }
                this->StepBack();
                this->StepBack();
                tkn.type = TokenTypes::Slash;
                tkn.text = "/";
                break;
            case '<':
                if (next == '=') {
                    tkn.text = "<=";
                    tkn.type = TokenTypes::LessEquals;
                    if (nnext == '>') {
                        tkn.text = "<=>";
                        tkn.type = TokenTypes::Spaceship;
                    } else this->StepBack();
                    return tkn;
                } else if (next == '<') {
                    tkn.text = "<<";
                    tkn.type = TokenTypes::LeftShift;
                    if (nnext == '=') {
                        tkn.text = "<<=";
                        tkn.type = TokenTypes::LeftShiftAssign;
                    } else this->StepBack();
                    return tkn;
                }
                this->StepBack();
                this->StepBack();
                tkn.type = TokenTypes::Less;
                tkn.text = "<";
                break;
            case '>':
                if (next == '=') {
                    tkn.text = ">=";
                    tkn.type = TokenTypes::GreaterEquals;
                    this->StepBack();
                    return tkn;
                } else if (next == '>') {
                    tkn.text = ">>";
                    tkn.type = TokenTypes::RightShift;
                    if (nnext == '=') {
                        tkn.text = ">>=";
                        tkn.type = TokenTypes::RightShiftAssign;
                    } else this->StepBack();
                    return tkn;
                }
                this->StepBack();
                this->StepBack();
                tkn.type = TokenTypes::Greater;
                tkn.text = ">";
                break;
        }

        return tkn;
    }

    Token Lexer::GenerateToken() {
        this->SkipBlank();
        Token tkn;
        tkn.location = this->location;
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
                tkn = this->handleThrees(tok);
                break;
            case '/': // /, /=, //, //=
                tkn = this->handleThrees(tok);
                break;
            case '#':
                this->SkipComment();
                tkn.type = TokenTypes::Blank;
                tkn.text = "\\n";
                this->location.col = 0;
                this->location.line++;
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
                tkn = this->handleTwos(tok);
                break;
            case '|': // |, |=, ||, |>
                tkn = this->handleTwos(tok);
                break;
            case '^': // ^, ^=
                tkn = this->handleTwos(tok);
                break;
            case '<': // <, <=, <<, <<=, <=>
                tkn = this->handleThrees(tok);
                break;
            case '>': // >, >=, >>, >>=
                tkn = this->handleThrees(tok);
                break;
            case '?':
                tkn.type = TokenTypes::QuestionMark;
                tkn.text = "?";
                break;
            case '.':
                tkn.type = TokenTypes::Dot;
                tkn.text = ".";
                break;
            case ',':
                tkn.type = TokenTypes::Comma;
                tkn.text = ",";
                break;
            case ':':
                tkn.type = TokenTypes::Colon;
                tkn.text = ":";
                break;
            case ';':
                tkn.type = TokenTypes::Semicolon;
                tkn.text = ";";
                break;
            case '(':
                tkn.type = TokenTypes::LBracket;
                tkn.text = "(";
                break;
            case ')':
                tkn.type = TokenTypes::RBracket;
                tkn.text = ")";
                break;
            case '[':
                tkn.type = TokenTypes::LSquareBracket;
                tkn.text = "[";
                break;
            case ']':
                tkn.type = TokenTypes::RSquareBracket;
                tkn.text = "]";
                break;
            case '{':
                tkn.type = TokenTypes::LSquiglyBracket;
                tkn.text = "{";
                break;
            case '}':
                tkn.type = TokenTypes::RSquiglyBracket;
                tkn.text = "}";
                break;
            case '\n':
                tkn.type = TokenTypes::Blank;
                tkn.text = "\\n";
                this->location.col = 0;
                this->location.line++;
                break;
            case '"':
                this->StepBack();
                tkn = this->ConsumeString();
                break;
            case '`':
                this->StepBack();
                tkn = this->ConsumeTick();
                break;
            case '\'':
                this->StepBack();
                tkn = this->ConsumeChar();
                break;
            case 0:
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
                    tkn.text = "";
                    tkn.text += tok;
                    tkn.type = TokenTypes::Illegal;
                }
        }

        return tkn;
    }
}
