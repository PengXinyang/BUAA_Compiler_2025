#include "./Lexer/include/FileProcess.h"
#include "./Lexer/include/Lexer.h"
#include<iostream>

#include "GenerateIR.h"
#include "MipsGenerate.h"
#include "OptimizeIR.h"
#include "OptimizeMips.h"
#include "Config/include/ConfigOptimizer.h"
#include "ErrorHandle/include/ErrorPrint.h"
#include "Lexer/include/TokenType.h"
#include "optimize/LLVMOptimizerInit.h"
#include "Parser/include/Parser.h"
#include "Symbol/include/SymbolHandle.h"
using namespace std;
string source;

int main() {
    //优化开关
    ConfigOptimizer::setConfigOptimize(true);
    ConfigOptimizer::initConfigOptimizer();
    LLVMOptimizerInit::setIsOptimize(true);
    LLVMOptimizerInit::setIsConstFold(true);

    source = FileProcess::readFileIntoString("testfile.txt");
    //下面是词法分析处理程序
    TokenType* tokenTypeMain = TokenType::getTokenTypeInstance();
    Lexer *lexer = Lexer::getLexerInstance();
    lexer->setCharPtr(const_cast<char *>(source.c_str()));
    lexer->handleSource();
    lexer->printRight();
    //下面是语法分析处理程序
    Parser* parser = Parser::getParserInstance();
    parser->ParserHandle();
    parser->printParser();
    //下面是符号表分析程序
    SymbolHandle* symbol_handle = SymbolHandle::getSymbolHandleInstance();
    symbol_handle->handleSymbol();
    //如果有错误，输出错误
    ErrorPrint::printfError();
    GenerateIR* generate_ir = GenerateIR::getInstance();
    if(!ErrorPrint::isError) {
        symbol_handle->printSymbol();
        generate_ir->generateLLVMIR();
        if(ConfigOptimizer::isConfigOptimize()) {
            //执行优化
            OptimizeIR* optimize_ir = OptimizeIR::getInstance();
            optimize_ir->runOptimizeIR();
        }
        generate_ir->printLLVMIR();
        Module* module = generate_ir->getModule();
        //MipsGenerate* mips_generate = MipsGenerate::getInstance(new Module());
        if(ConfigOptimizer::isConfigOptimize()) {
            //执行后端优化
            OptimizeMips* optimize_mips = OptimizeMips::getInstance();
            optimize_mips->runOptimizeMips();
        }
        MipsGenerate* mips_generate = MipsGenerate::getInstance(module);
        mips_generate->generateMips();
        mips_generate->printMips();
    }
    //删除类
    Lexer::deleteLexerInstance();
    TokenType::deleteTokenTypeInstance();
    return 0;
}
