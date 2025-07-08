//
// Created by PengXinyang on 24-9-20.
//

#include "../include/TokenType.h"
using namespace std;

TokenType::TokenType() {
    tokenTypeMap["Ident"] = "IDENFR";
    tokenTypeMap["IntConst"] = "INTCON";
    tokenTypeMap["StringConst"] = "STRCON";
    tokenTypeMap["CharConst"] = "CHRCON";
    tokenTypeMap["main"] = "MAINTK";
    tokenTypeMap["const"] = "CONSTTK";
    tokenTypeMap["int"] = "INTTK";
    tokenTypeMap["char"] = "CHARTK";
    tokenTypeMap["break"] = "BREAKTK";
    tokenTypeMap["continue"] = "CONTINUETK";
    tokenTypeMap["if"] = "IFTK";
    tokenTypeMap["else"] = "ELSETK";
    tokenTypeMap["!"] = "NOT";
    tokenTypeMap["&&"] = "AND";
    tokenTypeMap["||"] = "OR";
    tokenTypeMap["for"] = "FORTK";
    tokenTypeMap["getint"] = "GETINTTK";
    tokenTypeMap["getchar"] = "GETCHARTK";
    tokenTypeMap["printf"] = "PRINTFTK";
    tokenTypeMap["return"] = "RETURNTK";
    tokenTypeMap["+"] = "PLUS";
    tokenTypeMap["-"] = "MINU";
    tokenTypeMap["void"] = "VOIDTK";
    tokenTypeMap["*"] = "MULT";
    tokenTypeMap["/"] = "DIV";
    tokenTypeMap["%"] = "MOD";
    tokenTypeMap["<"] = "LSS";
    tokenTypeMap["<="] = "LEQ";
    tokenTypeMap[">"] = "GRE";
    tokenTypeMap[">="] = "GEQ";
    tokenTypeMap["=="] = "EQL";
    tokenTypeMap["!="] = "NEQ";
    tokenTypeMap["="] = "ASSIGN";
    tokenTypeMap[";"] = "SEMICN";
    tokenTypeMap[","] = "COMMA";
    tokenTypeMap["("] = "LPARENT";
    tokenTypeMap[")"] = "RPARENT";
    tokenTypeMap["["] = "LBRACK";
    tokenTypeMap["]"] = "RBRACK";
    tokenTypeMap["{"] = "LBRACE";
    tokenTypeMap["}"] = "RBRACE";
}

TokenType* TokenType::g_pTokenType = new TokenType();

TokenType* TokenType::getTokenTypeInstance() {
    if(g_pTokenType == nullptr) {
        g_pTokenType = new TokenType();
    }
    return g_pTokenType;
}
void TokenType::deleteTokenTypeInstance() {
    if(g_pTokenType) {
        delete g_pTokenType;
        g_pTokenType = nullptr;
    }
}

map<string,string> TokenType::getTokenTypeMap() {
    return tokenTypeMap;
}

string TokenType::getTokenType(const string& token) {
    return tokenTypeMap[token];
}
