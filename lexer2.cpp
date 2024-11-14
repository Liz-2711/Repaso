#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <sstream>
#include "lexer.hpp"

enum class State {
    Tokens_q0,
    Tokens_q1,
    Tokens_q9,
    Symbols_q0,
    Operators_q0,
    Boolean_q0,
    Boolean_q10,
    Boolean_q11,
    Boolean_q12,
    Boolean_q13,
    Boolean_q14,
    Boolean_q15,
};

Token Lexer::getNextToken()
{
    text.clear();
    State state = State::Tokens_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // 
            case State::Tokens_q0:
                if ((ch == '\n') || (ch == ' ') || (ch == '\t')) {
                    state = State::Tokens_q0;
                    ch = getNextChar();
                }
                else if (ch == EOF) {
                    return Token::Eof;
                }
                else if ((ch == '_') || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z'))) {
                    text += ch;
                    state = State::Tokens_q9;
                    ch = getNextChar();
                }
                else if ((ch >= '0') && (ch <= '9')) {
                    text += ch;
                    state = State::Tokens_q1;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton '
                    state = State::Symbols_q0;
                }
                break;
            case State::Tokens_q1:
                if ((ch >= '0') && (ch <= '9')) {
                    text += ch;
                    state = State::Tokens_q1;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::IntConst;
                }
                break;
            case State::Tokens_q9:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || (ch == '_')) {
                    text += ch;
                    state = State::Tokens_q9;
                    ch = getNextChar();
                }
               else {
                    ungetChar(ch);
                     return findKw(text);
                }
                break;
            // 
            case State::Symbols_q0:
                if (ch == '[') {
                    text += ch;
                    return Token::OpenBracket;
                }
                else if (ch == '"') {
                    text += ch;
                    return Token::Comillas;
                }
                else if (ch == ',') {
                    text += ch;
                    return Token::Comma;
                }
                else if (ch == ')') {
                    text += ch;
                    return Token::ClosePar;
                }
                else if (ch == '}') {
                    text += ch;
                    return Token::CloseCurly;
                }
                else if (ch == '(') {
                    text += ch;
                    return Token::OpenPar;
                }
                else if (ch == '{') {
                    text += ch;
                    return Token::OpenCurly;
                }
                else if (ch == ';') {
                    text += ch;
                    return Token::SemiColon;
                }
                else if (ch == ']') {
                    text += ch;
                    return Token::CloseBracket;
                }
                else {
                    // Trying next automaton '
                    state = State::Operators_q0;
                }
                break;
            // 
            case State::Operators_q0:
                if (ch == '*') {
                    text += ch;
                    return Token::OpMul;
                }
                else if (ch == '+') {
                    text += ch;
                    return Token::OpAdd;
                }
                else if (ch == '%') {
                    text += ch;
                    return Token::OpMod;
                }
                else if (ch == '-') {
                    text += ch;
                    return Token::OpSub;
                }
                else if (ch == '/') {
                    text += ch;
                    return Token::OpDiv;
                }
                else {
                    // Trying next automaton '
                    state = State::Boolean_q0;
                }
                break;
            // 
            case State::Boolean_q0:
                if (ch == '!') {
                    text += ch;
                    state = State::Boolean_q13;
                    ch = getNextChar();
                }
                else if (ch == '|') {
                    text += ch;
                    state = State::Boolean_q11;
                    ch = getNextChar();
                }
                else if (ch == '<') {
                    text += ch;
                    state = State::Boolean_q14;
                    ch = getNextChar();
                }
                else if (ch == '>') {
                    text += ch;
                    state = State::Boolean_q15;
                    ch = getNextChar();
                }
                else if (ch == '=') {
                    text += ch;
                    state = State::Boolean_q12;
                    ch = getNextChar();
                }
                else if (ch == '&') {
                    text += ch;
                    state = State::Boolean_q10;
                    ch = getNextChar();
                }
                else {
                    reportError(ch);
                    ch = getNextChar();
                    state = State::Tokens_q0;
                }
                break;
            case State::Boolean_q10:
                if (ch == '&') {
                    text += ch;
                    return Token::AND;
                }
                else {
                    reportError(ch);
                    ch = getNextChar();
                    state = State::Tokens_q0;
                }
                break;
            case State::Boolean_q11:
                if (ch == '|') {
                    text += ch;
                    return Token::OR;
                }
                else {
                    reportError(ch);
                    ch = getNextChar();
                    state = State::Tokens_q0;
                }
                break;
            case State::Boolean_q12:
                if (ch == '=') {
                    text += ch;
                    return Token::doubleEqual;
                }
                else {
                    ungetChar(ch);
                    return Token::EQUAL;
                }
                break;
            case State::Boolean_q13:
                if (ch == '=') {
                    text += ch;
                    return Token::NOTEQUAL;
                }
                else {
                    ungetChar(ch);
                    return Token::NOT;
                }
                break;
            case State::Boolean_q14:
                if (ch == '=') {
                    text += ch;
                    return Token::LESS_EQUAL;
                }
                else {
                    ungetChar(ch);
                    return Token::LESSTHAN;
                }
                break;
            case State::Boolean_q15:
                if (ch == '=') {
                    text += ch;
                    return Token::GREATER_EQUAL;
                }
                else {
                    ungetChar(ch);
                    return Token::GREATER;
                }
                break;
        }
    }

}

const char *Lexer::tokenToString(Token tk)
{
    switch (tk) {
        case Token::GREATER_EQUAL: return "GREATER_EQUAL";
        case Token::LESSTHAN: return "LESSTHAN";
        case Token::NOT: return "NOT";
        case Token::OR: return "OR";
        case Token::OpMod: return "OpMod";
        case Token::OpAdd: return "OpAdd";
        case Token::Eof: return "Eof";
        case Token::NOTEQUAL: return "NOTEQUAL";
        case Token::doubleEqual: return "doubleEqual";
        case Token::OpSub: return "OpSub";
        case Token::IntConst: return "IntConst";
        case Token::CloseBracket: return "CloseBracket";
        case Token::LESS_EQUAL: return "LESS_EQUAL";
        case Token::Ident: return "Ident";
        case Token::OpenPar: return "OpenPar";
        case Token::Comillas: return "Comillas";
        case Token::EQUAL: return "EQUAL";
        case Token::SemiColon: return "SemiColon";
        case Token::OpenBracket: return "OpenBracket";
        case Token::Comma: return "Comma";
        case Token::ClosePar: return "ClosePar";
        case Token::GREATER: return "GREATER";
        case Token::CloseCurly: return "CloseCurly";
        case Token::AND: return "AND";
        case Token::OpDiv: return "OpDiv";
        case Token::OpenCurly: return "OpenCurly";
        case Token::OpMul: return "OpMul";
        default: return "Unknown";
    }

}