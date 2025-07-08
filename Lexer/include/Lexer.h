//
// Created by Peng Xinyang on 24-9-20.
//

#ifndef LEXER_H
#define LEXER_H
#include <algorithm>
#include <map>
#include <string>
#include <utility>
using namespace std;
struct Word {
    string word;//单词名字
    int line_num;//行号
    int pos;
    string word_type;//单词类型
    Word(){word = "", line_num = 0; word_type = "";pos = -1;}
    Word(string word_type,string word,int lin_num,int pos) {
        this->word = move(word);
        this->word_type = move(word_type);
        this->line_num = lin_num;
        this->pos = pos;
    }
};

class Lexer {
public:
    static Lexer* getLexerInstance();
    static void deleteLexerInstance();
    //初始化，让指针指向源代码source
    void setCharPtr(char*source);
    //处理下一个单词
    Word handleNext();
    //处理的具体过程
    int handle_next();
    //完全处理所有的单词
    void handleSource();
    //获取读取的单词
    string getToken();
    //输出正确的内容
    void printRight();
    //输出错误的内容
    void printWrong();
    //错误处理
    void lexerError();
    //获取tokenMap
    map<int,Word> getWordMap();
private:
    static Lexer* lexerInstance;
    //存储单词类别和单词值
    map<int,Word> tokenMap;
    //行号+错误码
    map<int,char> lineWrong;
    string token;//当前处理的单词
    char Char = ' ';//存取当前读进的字符
    long long num = 0;//存入当前读入的整型变量
    char* charPtr = nullptr;//字符指针
    int lineNum = 1;//行数
    string tokenType;
    //读字符过程
    void getChar();
    //清空token
    void clearToken();
    //判断是否为空格
    bool isSpace();
    //判断是否为换行符
    bool isNewLine();
    //判断是否为制表符
    bool isTab();
    //判断是否为字母
    bool isLetter();
    //判断是否为数字
    bool isDigit();
    //判断是否为逗号
    bool isComma();
    //判断是否为双引号，用于处理常字符串
    bool isDoubleQuo();
    //判断是否为单引号，用于处理字符常量
    bool isSingleQuo();
    //判断是否为分号
    bool isSemi();
    //判断是否为等号
    bool isEqu();
    //判断是否为非号
    bool isNot();
    //判断是否为小于号
    bool isLess();
    //判断是否为大于号
    bool isGreater();
    //判断是否为加号
    bool isPlus();
    //判断是否为减号
    bool isMinus();
    //判断是否为除号
    bool isDivi();
    //判断是否为星号
    bool isStar();
    //判断是否为百分号
    bool isMod();
    //判断是否为左括号
    bool isLpar();
    //判断是否为右括号
    bool isRpar();
    //判断是否为左中括号
    bool isLBrack();
    //判断是否为右中括号
    bool isRBrack();
    //判断是否为左大括号
    bool isLBrace();
    //判断是否为右大括号
    bool isRBrace();
    //判断是否为单个'&'符
    bool isAnd();
    //判断是否为单个'|'符
    bool isOr();
    //每次调用把Char字符与token字符数组中的字符串连接
    void catToken();
    //字符指针回退一个
    void retract();
    //查找token是不是保留字
    //有个小漏洞：变量名是Ident，IntConst等
    int reserver();
    //将token数字字符串转成整数
    long long transNum(const string &token);
};



#endif //LEXER_H
