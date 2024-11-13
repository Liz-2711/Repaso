#ifndef __TOKENS_H__
#define __TOKENS_H__

enum class Token {
    GREATER_EQUAL,
    LESSTHAN,
    NOT,
    OR,
    OpMod,
    OpAdd,
    Eof,
    NOTEQUAL,
    OpSub,
    IntConst,
    CloseBracket,
    LESS_EQUAL,
    Ident,
    OpenPar,
    Comillas,
    EQUAL,
    SemiColon,
    OpenBracket,
    Comma,
    ClosePar,
    GREATER,
    CloseCurly,
    AND,
    OpDiv,
    OpenCurly,
    OpMul,
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