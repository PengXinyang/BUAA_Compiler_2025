//
// Created by PengXinyang on 24-9-28.
//

#include "../include/TokenScanner.h"

TokenScanner*TokenScanner::instance = nullptr;
TokenScanner *TokenScanner::getTokenScannerInstance() {
    if(instance==nullptr) {
        instance = new TokenScanner();
    }
    return instance;
}
TokenScanner::TokenScanner() {
    lexer = Lexer::getLexerInstance();
    wordMap = lexer->getWordMap();
}
//获取当前位置的word，然后位置+1
Word TokenScanner::readNowWord() {
    nowWord = wordMap[position++];
    return nowWord;
}

//预读k个单词，只是预读，不正式读，所以位置指针不变
Word* TokenScanner::preReadWords(int k) {
    for(int i=0; i<k; i++) {
        preRead[i] = wordMap[position+i];
    }
    return preRead;
}

//回退一位单词
void TokenScanner::retractWord() {
    position = position-1;
    nowWord = wordMap[position-1];
}

//获得当前单词
Word TokenScanner::getNowWord() {
    return nowWord;
}
//获得预读单词
Word *TokenScanner::getPreWords() {
    return preRead;
}
