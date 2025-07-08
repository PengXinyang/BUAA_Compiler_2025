//
// Created by Peng Xinyang on 24-9-20.
//

#include "../include/Lexer.h"
#include <map>

#include "../../ErrorHandle/include/ErrorPrint.h"
#include "../include/TokenType.h"

TokenType* tokenTypeN = TokenType::getTokenTypeInstance();
map<string,string> tokenTypeMap = tokenTypeN->getTokenTypeMap();
int i=0;

Lexer*Lexer::lexerInstance = new Lexer();
void Lexer::setCharPtr(char *source) {
	charPtr = source;
}

Lexer *Lexer::getLexerInstance() {
    if (lexerInstance == nullptr) {
        lexerInstance = new Lexer();
    }
    return lexerInstance;
}
void Lexer::deleteLexerInstance() {
    if(lexerInstance) {
        delete lexerInstance;
        lexerInstance = nullptr;
    }
}

string Lexer::getToken() {
	return token;
}
Word Lexer::handleNext() {
	handle_next();
	Word word = Word(tokenType, token,lineNum,i);
	return word;
}
void Lexer::handleSource() {
	while(charPtr && *charPtr) {
		Word p = handleNext();
		if(!p.word_type.empty()&&!p.word.empty()) {
			tokenMap[i++] = p;
		}
	}
}

//输出正确的结果
void Lexer::printRight() {
	if(lineWrong.empty()) {
		FILE*fp = fopen("lexer.txt", "w");
		for(const auto&[fst,snd]:tokenMap) {
			fprintf(fp,"%s %s\n",snd.word_type.c_str(),snd.word.c_str());
		}
		fclose(fp);
	}
}
//输出错误的结果
void Lexer::printWrong() {
	if(!lineWrong.empty()) {
		FILE*fp = fopen("error.txt", "w");
	for(const auto &[fst, snd] : lineWrong) {
		fprintf(fp, "%d %c\n",fst,snd);
	}
	fclose(fp);
	}
}

//获取tokenMap
map<int, Word> Lexer::getWordMap() {
	return tokenMap;
}


//读字符过程
void Lexer::getChar() {
	Char = *charPtr;
	charPtr++;
}
//清空token
void Lexer::clearToken() {
	string str="";
	token.swap(str);
	tokenType="";
}
//判断是否为空格
bool Lexer::isSpace() {
	return Char == ' ';
}
//判断是否为换行符
bool Lexer::isNewLine() {
	return Char == '\n';
}
//判断是否为制表符
bool Lexer::isTab() {
	return Char == '\t';
}
//判断是否为字母或下划线
bool Lexer::isLetter() {
	return (Char >= 'a' && Char <= 'z') || (Char >= 'A' && Char <= 'Z') || Char == '_';
}
//判断是否为数字
bool Lexer::isDigit() {
	return Char >= '0' && Char <= '9';
}
//判断是否为逗号
bool Lexer::isComma() {
	return Char == ',';
}
//判断是否为双引号，用于处理常字符串
bool Lexer::isDoubleQuo() {
	return Char == '\"';
}
//判断是否为单引号，用于处理字符常量
bool Lexer::isSingleQuo() {
	return Char == '\'';
}
//判断是否为分号
bool Lexer::isSemi() {
	return Char == ';';
}
//判断是否为等号
bool Lexer::isEqu() {
	return Char == '=';
}
//判断是否为非号
bool Lexer::isNot() {
	return Char == '!';
}
//判断是否为小于号
bool Lexer::isLess() {
	return Char == '<';
}
//判断是否为大于号
bool Lexer::isGreater() {
	return Char == '>';
}
//判断是否为加号
bool Lexer::isPlus() {
	return Char == '+';
}
//判断是否为减号
bool Lexer::isMinus() {
	return Char == '-';
}
//判断是否为除号
bool Lexer::isDivi() {
	return Char == '/';
}
//判断是否为星号
bool Lexer::isStar() {
	return Char == '*';
}
//判断是否为百分号
bool Lexer::isMod() {
	return Char == '%';
}
//判断是否为左括号
bool Lexer::isLpar() {
	return Char == '(';
}
//判断是否为右括号
bool Lexer::isRpar() {
	return Char == ')';
}
//判断是否为左中括号
bool Lexer::isLBrack() {
	return Char == '[';
}
//判断是否为右中括号
bool Lexer::isRBrack() {
	return Char == ']';
}
//判断是否为左大括号
bool Lexer::isLBrace() {
	return Char == '{';
}
//判断是否为右大括号
bool Lexer::isRBrace() {
	return Char == '}';
}
//判断是否为单个'&'符
bool Lexer::isAnd() {
	return Char == '&';
}
//判断是否为单个'|'符
bool Lexer::isOr() {
	return Char == '|';
}
//每次调用把Char字符与token字符数组中的字符串连接
void Lexer::catToken() {
	token += Char;
}
//字符指针回退一个
void Lexer::retract() {
	charPtr--;
}
//查找token是不是保留字
//有个小漏洞：变量名是Ident，IntConst等
int Lexer::reserver() {
	if(tokenTypeMap.find(token)!=tokenTypeMap.end()) {
		return 1;
	}
	else if(token=="Ident" || token == "IntConst" || token == "StringConst" || token == "CharConst") {
		return 1;
	}
	else return 0;
}
//将token数字字符串转成整数
long long Lexer::transNum(const string &token) {
	return stoll(token);
}
//错误处理
void Lexer::lexerError() {
	lineWrong[lineNum] = 'a';
	ErrorPrint::printError(ErrorCategory::illegal_symbol, lineNum);
}

//词法分析程序
int Lexer::handle_next() {//返回类别码
	clearToken();
	getChar();//读入第一个字符
	TokenType* tokenTypeN = TokenType::getTokenTypeInstance();
	tokenTypeMap = tokenTypeN->getTokenTypeMap();
	while (isSpace() || isTab() || Char=='\r')
		getChar();/* 读取字符，跳过空格、换行和Tab */
	if(isNewLine()) {
		/*这一行已经读完*/
		lineNum++;
	}
	else if (isLetter()) {  /* 判断当前字符是否是一个字母*/
		while (isLetter() || isDigit()) { /* 将字符拼接成字符串*/
			catToken();
			getChar();
		}
		retract();/* 指针后退一个字符*/
		int resultValue = reserver();/* resultValue是查找保留字的返回值 */
		if (resultValue == 0) tokenType = tokenTypeMap["Ident"];/* resultValue=0，token中的字符串为标识符 */
		else tokenType = tokenTypeMap[token];/* 否则token中的字符串为保留字*/
	}
	else if (isDigit()) {/* 判断当前字符是否是一个数字*/
		while (isDigit()) {/* 将字符拼接成整数*/
			catToken();
			getChar();
		}
		retract();
		num = transNum(token);/* 将token中的字符串转换成整数*/
		tokenType = tokenTypeMap["IntConst"];/* 此时识别的单词是整数*/
	}
	else if(isDoubleQuo()) {/*当前读到了双引号，说明这很可能是字符串*/
		catToken();
		getChar();
		while(Char!='\"' || (*(charPtr-2))=='\\' && (*(charPtr-3))!='\\') {
			//当前字符不是双引号，或者是双引号但是前一位是转义字符，说明这个双引号属于字符串
			catToken();
			getChar();
		}
		catToken();
		getChar();//读取后一个双引号的空格
		//这里指针需要回退，因为此时指针指在双引号后面两位，应退回一位。
		retract();
		tokenType = tokenTypeMap["StringConst"];
	}
	else if(isSingleQuo()) {/*读到了单引号，很可能是字符常量*/
		catToken();
		getChar();
		while(Char!='\'' || (*(charPtr-2))=='\\' && (*(charPtr-3))!='\\') {
			catToken();
			getChar();
		}
		catToken();
		getChar();//读取后一个单引号的下一位
		//这里指针需要回退，因为此时指针指在双引号后面两位，应退回一位。
		retract();
		tokenType = tokenTypeMap["CharConst"];
	}
	/*开始读双分界符，不包括条件运算符*/
	else if(isEqu()) {
		//读到了一个等号，有可能是双等于号
		catToken();
		getChar();
		if (isEqu()) {
			catToken();
			tokenType = tokenTypeMap["=="];/*判断是否为双等于符号*/
		}
		else {
			retract();
			tokenType = tokenTypeMap["="];
		}
	}
	else if(isNot()) {
		//读到了一个非号，有可能是不等于号
		catToken();
		getChar();
		if (isEqu()) {
			catToken();
			tokenType = tokenTypeMap["!="];
		}/*判断是否为不等于符号*/
		else {
			retract();
			tokenType = tokenTypeMap["!"];
		}
	}
	else if(isLess()) {
		//读到了一个小于号，有可能是小于等于号
		catToken();
		getChar();
		if (isEqu()) {
			catToken();
			tokenType = tokenTypeMap["<="];/*判断是否为小于等于符号*/
		}
		else {
			retract();
			tokenType = tokenTypeMap["<"];
		}
	}
	else if(isGreater()) {
		catToken();
		getChar();
		if (isEqu()) {
			catToken();
			tokenType = tokenTypeMap[">="];
		}
		else {
			retract();
			tokenType = tokenTypeMap[">"];
		}
	}
	/*双分界符结束，读单分界符*/
	else if (isPlus()) {
		catToken();
		tokenType = tokenTypeMap["+"];/*判断当前字符是否为加号*/
	}
	else if (isMinus()) {
		catToken();
		tokenType = tokenTypeMap["-"];/*判断当前字符是否为减号*/
	}
	else if (isStar()) {
		catToken();
		tokenType = tokenTypeMap["*"];/*判断是否为星号*/
	}
	else if (isMod()) {
		catToken();
		tokenType = tokenTypeMap["%"];/*判断是否为%*/
	}
	else if (isLpar()) {
		catToken();
		tokenType = tokenTypeMap["("];/*判断是否为左括号*/
	}
	else if (isRpar()) {
		catToken();
		tokenType = tokenTypeMap[")"];/*判断是否为右括号*/
	}
	else if (isLBrace()) {
		catToken();
		tokenType = tokenTypeMap["{"];/*判断是否为左大括号*/
	}
	else if (isRBrace()) {
		catToken();
		tokenType = tokenTypeMap["}"];/*判断是否为右大括号*/
	}
	else if(isLBrack()) {
		catToken();
		tokenType = tokenTypeMap["["];/*判断是否为左中括号*/
	}
	else if (isRBrack()) {
		catToken();
		tokenType = tokenTypeMap["]"];/*判断是否为右中括号*/
	}
	else if (isComma()) {
		catToken();
		tokenType = tokenTypeMap[","];/*判断是否为逗号*/
	}
	else if (isSemi()) {
		catToken();
		tokenType = tokenTypeMap[";"];/*判断是否为分号*/
	}
	/*单分界符结束，接下来处理&&和||*/
	else if(isAnd()) {
		catToken();
		getChar();
		if(isAnd()) {
			catToken();
			tokenType = tokenTypeMap["&&"];
		}
		else {
			retract();
			lexerError();
			Word word = Word(tokenTypeMap["&&"],"&&",lineNum,i);
			tokenMap[i++] = word;
		}
	}
	else if(isOr()) {
		catToken();
		getChar();
		if(isOr()) {
			catToken();
			tokenType = tokenTypeMap["||"];
		}
		else {
			retract();
			lexerError();
			Word word = Word(tokenTypeMap["||"],"||",lineNum,i);
			tokenMap[i++] = word;
		}
	}
	/*处理除号和注释*/
	else if (isDivi()) {
		catToken();
		getChar();
		if(isDivi()) {
			//两个'/'，说明本行是注释,直接读到'\n'；如果注释是最后一行，没有换行符，那么跳出循环。
			while(!isNewLine()) {
				if((*charPtr) == 0) break;
				getChar();
			}
			//如果当前是换行符，则指针回退一位，便于下一次处理时读到换行符，让行数+1
			//如果注释是最后一行，没有换行符
			if(isNewLine()) retract();
		}
		else if (isStar()) {/*判断是否为星号*/
			/*处理注释，可能为多行，读到换行符则行号+1*/
			do {
				do {
					getChar();
					if(isNewLine()) {lineNum++;}
				} while (!isStar());
				//此时说明已经读到了下一个星号，如果接下来是'/'那么多行注释结束，否则仍然处于注释状态
				//如果全为*
				do {
					getChar();
					if(isNewLine()) {lineNum++;}
					//读到'/'，多行注释结束
					else if (isDivi()) return 0;/*注释处理完毕*/
				} while (isStar());
				//否则，前面的*也属于注释，多行注释没有结束，继续读。
			} while (!isStar());
		}
		else {
			//说明只是除号
			retract();
			tokenType = tokenTypeMap["/"];/*不是注释，当前单词为除号*/
		}
	}
	return 0;
}