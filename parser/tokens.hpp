#pragma once

namespace Tanzanite {
    enum class TokenTypes {
        Eof = -1,
        Illegal,
        Identifier,
        Operator,
        Keyword,
        Delimiter,
        Value
    };
}
