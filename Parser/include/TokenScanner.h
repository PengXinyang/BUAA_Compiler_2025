//
// Created by PengXinyang on 24-9-28.
//

#ifndef SCANNER_H
#define SCANNER_H
#include "../../Lexer/include/Lexer.h"


class TokenScanner {
public:
    static TokenScanner* getTokenScannerInstance();
    //读当前单词
    Word readNowWord();
    //预读k个单词
    Word* preReadWords(int k);
    //回退一个单词，通常是错误处理
    void retractWord();
    //获得当前单词
    Word getNowWord();
    //获得预读单词
    Word* getPreWords();
    int position = 0;//记录当前所在单词位置
private:
    TokenScanner();
    static TokenScanner* instance;
    Lexer* lexer;
    Word nowWord;//当前读的单词
    Word preRead[30];//预读的单词
    //单词map
    map<int,Word> wordMap;
};



#endif //SCANNER_H
