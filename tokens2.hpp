#ifndef __TOKENS_H__
#define __TOKENS_H__

enum class Token {
    GREATER_EQUAL,
    NOT,
    OR,
    OpAdd,
    CloseBracket,
    Eof,
    LESSTHAN,
    Hexa,
    NOTEQUAL,
    doubleEqual,
    OpSub,
    IntConst,
    OpMod,
    Bin,
    LESS_EQUAL,
    Ident,
    OpenPar,
    Comillas,
    EQUAL,
    SemiColon,
    OpenBracket,
    Comma,
    ClosePar,
    AND,
    OpDiv,
    OpMul,
    OpenCurly,
    GREATER,
    CloseCurly,
    KwPrint,
    KwRead,
    KwIf,
    KwReturn,
    KwRef,
    KwInt,
    KwVoid,
    KwClass,
    KwWhile,
    KwElse,
};
#endif