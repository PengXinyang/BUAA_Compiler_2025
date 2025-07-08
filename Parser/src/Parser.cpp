//
// Created by PengXinyang on 24-9-28.
//

#include "../include/Parser.h"

#include "../include/ParserTree.h"
#include "../../ErrorHandle/include/ErrorPrint.h"
void Parser::ParserHandle() {
    tokenScanner = TokenScanner::getTokenScannerInstance();
    ParserRoot = CompUnit();
}
void Parser::printParser() {
    if(!ErrorPrint::isError) {
        //说明没有错误
        FILE* fp = fopen("parser.txt", "w");
        ParserTree::printTree(fp,ParserRoot);
        fclose(fp);
    }
}
Parser* Parser::instance = nullptr;
Parser *Parser::getParserInstance() {
    if(instance==nullptr) {
        instance = new Parser();
    }
    return instance;
}

TreeNode *Parser::getPaserTree(){
    return ParserRoot;
}

TreeNode *Parser::CompUnit() {
    Word word;
    word.word="<CompUnit>";
    //创建Parser的根节点，最后连接完要返回这个节点
    TreeNode* root = ParserTree::createTree(word);
    //预读单词,考虑到First集合，选择预读3位
    preWords = tokenScanner->preReadWords(3);
    while(preWords[0].word=="const" || preWords[2].word!="(") {
        //第一位是const或第3位不是左括号，说明一定不是函数
        ParserTree::catchTree(root,Decl());
        //预读单词,考虑到First集合，选择预读3位
        preWords = tokenScanner->preReadWords(3);
    }
    while(preWords[1].word!="main") {
        //第2个不是main，那么只能是一般的函数
        ParserTree::catchTree(root,FuncDef());
        preWords = tokenScanner->preReadWords(3);
    }
    //剩下的，应当是main函数
    ParserTree::catchTree(root,MainFuncDef());
    return root;
}

TreeNode *Parser::Decl() {
    Word word;
    word.word="<Decl>";
    TreeNode* root = ParserTree::createTree(word);
    //预读，看第一位是不是const
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word=="const") {
        ParserTree::catchTree(root,ConstDecl());
    }
    else {
        //不是常量声明，那就是变量声明
        ParserTree::catchTree(root,VarDecl());
    }
    return root;
}

TreeNode *Parser::ConstDecl() {
    Word word;
    word.word="<ConstDecl>";
    //创建ConstDecl的根节点，最后连接完要返回这个节点
    TreeNode* root = ParserTree::createTree(word);
    now_word = tokenScanner->readNowWord();//当前单词是const
    ParserTree::catchTree(root, now_word);//将const连到树上
    //识别Btype并连接
    ParserTree::catchTree(root,Btype());
    //识别ConstDef并连接
    ParserTree::catchTree(root,ConstDef());
    //接下来，可能进入{, ConstDef}循环，先预读一位
    preWords = tokenScanner->preReadWords(1);
    while(preWords[0].word == ",") {
        now_word = tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //识别ConstDef并连接
        ParserTree::catchTree(root,ConstDef());
        //将下一个单词预读入
        preWords = tokenScanner->preReadWords(1);
    }
    //预读一位，判断是不是分号，如果不是就报错
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word!=";") {
        //如果下一个单词不是分号，说明出现了错误
        ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
    }
    else {
        //最后一个是分号，需要连接上
        now_word = tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    return root;
}

TreeNode *Parser::Btype() {
    Word word;
    word.word = "<Btype>";
    TreeNode *btypeRoot = ParserTree::createTree(word);
    now_word=tokenScanner->readNowWord();
    if(now_word.word=="int"||now_word.word=="char") {
        //说明是Btype
        ParserTree::catchTree(btypeRoot,now_word);
    }
    return btypeRoot;
}

TreeNode *Parser::ConstDef() {
    Word word;
    word.word="<ConstDef>";
    TreeNode* root = ParserTree::createTree(word);
    now_word=tokenScanner->readNowWord();
    if(now_word.word_type=="IDENFR") {
        //第一个一定是标识符
        ParserTree::catchTree(root, now_word);
        now_word=tokenScanner->readNowWord();
        if(now_word.word=="[") {
            //左括号
            ParserTree::catchTree(root, now_word);
            ParserTree::catchTree(root,ConstExp());
            //预读1位，判断是不是右括号
            preWords = tokenScanner->preReadWords(1);
            if(preWords[0].word!="]") {
                //不是右括号，需要报k型错误
                ErrorPrint::printError(ErrorCategory::r_bracket_lack,now_word.line_num);
                //再读，下一位应当是等号
                now_word=tokenScanner->readNowWord();
            }
            else {
                //是右括号，连接
                now_word = tokenScanner->readNowWord();
                ParserTree::catchTree(root, now_word);
                //再读，下一位应当是等号
                now_word=tokenScanner->readNowWord();
            }
        }
        if(now_word.word=="=") {
            //赋值号
            ParserTree::catchTree(root, now_word);
        }
        //常量赋值
        ParserTree::catchTree(root,ConstInitVal());
    }
    return root;
}

TreeNode *Parser::ConstInitVal() {
    Word word;
    word.word="<ConstInitVal>";
    TreeNode* root = ParserTree::createTree(word);
    //预读，判断下一个单词是什么类型
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word_type=="STRCON") {
        //说明是stringConst
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    else if(preWords[0].word=="{") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        preWords = tokenScanner->preReadWords(1);
        if(preWords[0].word=="}") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }
        else {
            //不是右括号，一定是ConstExp
            ParserTree::catchTree(root,ConstExp());
            //如果是逗号，就进循环
            now_word=tokenScanner->readNowWord();
            while(now_word.word==",") {
                ParserTree::catchTree(root, now_word);
                ParserTree::catchTree(root,ConstExp());
                now_word=tokenScanner->readNowWord();
            }
            //now_word是右大括号，直接加入
            ParserTree::catchTree(root, now_word);
        }
    }
    else {
        ParserTree::catchTree(root,ConstExp());
    }
    return root;
}

TreeNode *Parser::VarDecl() {
    Word word;
    word.word="<VarDecl>";
    TreeNode* root = ParserTree::createTree(word);
    ParserTree::catchTree(root,Btype());
    ParserTree::catchTree(root,VarDef());
    //预读，看看是不是逗号
    preWords = tokenScanner->preReadWords(1);
    while(preWords[0].word==",") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,VarDef());
        preWords = tokenScanner->preReadWords(1);
    }
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word!=";") {
        ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
    }
    else {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    return root;
}

TreeNode *Parser::VarDef() {
    Word word;
    word.word="<VarDef>";
    TreeNode* root = ParserTree::createTree(word);
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root, now_word);
    //预读，有可能是左括号
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word=="[") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,ConstExp());
        preWords = tokenScanner->preReadWords(1);
        if(preWords[0].word!="]") {
            ErrorPrint::printError(ErrorCategory::r_bracket_lack,now_word.line_num);
        }
        else {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
            preWords = tokenScanner->preReadWords(1);//预读，判断下一个是不是赋值符号
        }
    }
    if(preWords[0].word=="=") {
        //有等号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,InitVal());
    }
    return root;
}

TreeNode *Parser::InitVal() {
    Word word;
    word.word="<InitVal>";
    TreeNode* root = ParserTree::createTree(word);
    //先预读，判断是不是字符串常量
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word_type=="STRCON") {
        //说明是stringConst
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    else if(preWords[0].word=="{") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        preWords = tokenScanner->preReadWords(1);
        if(preWords[0].word=="}") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }
        else {
            //不是右括号，一定是Exp
            ParserTree::catchTree(root,Exp());
            //如果是逗号，就进循环
            now_word=tokenScanner->readNowWord();
            while(now_word.word==",") {
                ParserTree::catchTree(root, now_word);
                ParserTree::catchTree(root,Exp());
                now_word=tokenScanner->readNowWord();
            }
            //now_word是右大括号，直接加入
            ParserTree::catchTree(root, now_word);
        }
    }
    else {
        ParserTree::catchTree(root,Exp());
    }
    return root;
}

TreeNode *Parser::FuncDef() {
    Word word;
    word.word="<FuncDef>";
    TreeNode* root = ParserTree::createTree(word);
    //添加FuncType
    ParserTree::catchTree(root,FuncType());
    //添加ident
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root,now_word);
    //添加小括号，左右
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root,now_word);
    //预读，判断是不是右小括号
    //如果不是右小括号，也不是FuncFParams的first集，那么就说明缺少右小括号，报错
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word==")") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    else if(preWords[0].word=="int"||preWords[0].word=="char") {
        //说明是形参的First集
        ParserTree::catchTree(root,FuncFParams());
        //预读，看下一位是不是右括号
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word==")") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }
        else {
            //报错
            ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
        }
    }
    else {
        //报错
        ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
    }
    //右小括号后面是Block;无论有没有右小括号，都要继续进行
    ParserTree::catchTree(root,Block());
    return root;
}

TreeNode *Parser::MainFuncDef() {
    Word word;
    word.word="<MainFuncDef>";
    TreeNode* root = ParserTree::createTree(word);
    for(int i=0;i<3;++i) {
        //int,main,( 没有异议
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    //预读，判断右括号
    preWords = tokenScanner->preReadWords(1);
    if(preWords[0].word==")") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    else {
        //报错
        ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
    }
    //右小括号后面是Block;无论有没有右小括号，都要继续进行
    ParserTree::catchTree(root,Block());
    return root;
}

TreeNode *Parser::FuncType() {
    Word word;
    word.word = "<FuncType>";
    TreeNode *btypeRoot = ParserTree::createTree(word);
    now_word=tokenScanner->readNowWord();
    if(now_word.word=="int"||now_word.word=="char"||now_word.word=="void") {
        //说明是Btype
        ParserTree::catchTree(btypeRoot,now_word);
    }
    return btypeRoot;
}

TreeNode *Parser::FuncFParams() {
    Word word;
    word.word="<FuncFParams>";
    TreeNode* root = ParserTree::createTree(word);
    ParserTree::catchTree(root,FuncFParam());
    //预读，看是不是逗号
    preWords=tokenScanner->preReadWords(1);
    while(preWords[0].word==",") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,FuncFParam());
        preWords=tokenScanner->preReadWords(1);
    }
    return root;
}

TreeNode *Parser::FuncFParam() {
    Word word;
    word.word="<FuncFParam>";
    TreeNode* root = ParserTree::createTree(word);
    ParserTree::catchTree(root,Btype());
    preWords=tokenScanner->preReadWords(1);
    if(preWords[0].word_type=="IDENFR") {
        //是标识符
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //判断是不是左中括号
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word=="[") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
            preWords=tokenScanner->preReadWords(1);
            if(preWords[0].word=="]") {
                now_word=tokenScanner->readNowWord();
                ParserTree::catchTree(root, now_word);
            } else {
                //缺少右中括号
                ErrorPrint::printError(ErrorCategory::r_bracket_lack,now_word.line_num);
            }
        }
    }
    else printf("函数参数FuncFParam出现错误\n");
    return root;
}

TreeNode *Parser::Block() {
    Word word;
    word.word="<Block>";
    TreeNode* root = ParserTree::createTree(word);
    preWords=tokenScanner->preReadWords(1);
    if(preWords[0].word=="{") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //预读，看是不是右括号
        preWords=tokenScanner->preReadWords(1);
        while(preWords[0].word!="}") {
            //不是右括号，应当是BlockItem
            ParserTree::catchTree(root, BlockItem());
            //继续预读
            preWords=tokenScanner->preReadWords(1);
        }
        //接下来一定是右大括号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
    }
    else {
        printf("Block语句块左大括号出现错误\n");
    }
    return root;
}

TreeNode *Parser::BlockItem() {
    Word word;
    word.word="<BlockItem>";
    TreeNode* root = ParserTree::createTree(word);
    preWords=tokenScanner->preReadWords(1);
    if(preWords[0].word=="const" || preWords[0].word=="int" || preWords[0].word=="char") {
        //说明是声明
        ParserTree::catchTree(root,Decl());
    } else {
        //否则是语句
        ParserTree::catchTree(root,Stmt());
    }
    return root;
}

TreeNode *Parser::Stmt() {
    Word word;
    word.word="<Stmt>";
    TreeNode* root = ParserTree::createTree(word);
    preWords=tokenScanner->preReadWords(2);
    //如果第一个单词是ident，并且第二个不是左括号那么就是LVal
    //因为如果第二个是左括号，说明这是函数值调用，隶属于[Exp];这一项
    //有一个问题：LVal可以推出Exp，同时Exp可以推出LVal，两者差别在于有没有等号，所以考虑回溯
    if(preWords[0].word_type=="IDENFR") {
        //准备回溯，首先假设是左值赋值表达式
        //pos记录预处理前的位置
        int pos = tokenScanner->position;
        TreeNode* predict = LVal();
        //预读，看是不是赋值等号
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word=="=") {
            //说明假设没有问题，是左值表达式赋值
            ParserTree::catchTree(root,predict);
            //接下来应当是赋值符号=
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root,now_word);
            preWords=tokenScanner->preReadWords(1);
            if(preWords[0].word=="getint" || preWords[0].word=="getchar") {
                //这个单词可以直接连树上，并且下一个单词是左小括号，也可以直接连
                now_word=tokenScanner->readNowWord();
                ParserTree::catchTree(root, now_word);
                now_word=tokenScanner->readNowWord();
                ParserTree::catchTree(root,now_word);
                preWords=tokenScanner->preReadWords(1);
                if(preWords[0].word!=")") {
                    //如果没有右小括号，报错
                    ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
                }
                else {
                    now_word=tokenScanner->readNowWord();
                    ParserTree::catchTree(root, now_word);
                    //预读，判断分号
                    preWords=tokenScanner->preReadWords(1);
                }
                if(preWords[0].word!=";") {
                    //缺少分号
                    ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
                }
                else {
                    now_word=tokenScanner->readNowWord();
                    ParserTree::catchTree(root, now_word);
                }
            }
            else {
                //应当是Exp
                ParserTree::catchTree(root,Exp());
                //预读，判断是不是分号
                preWords=tokenScanner->preReadWords(1);
                if(preWords[0].word!=";") {
                    //缺少分号
                    ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
                }
                else {
                    now_word=tokenScanner->readNowWord();
                    ParserTree::catchTree(root, now_word);
                }
            }
        }else {
            //说明其实是Exp，应当回溯
            tokenScanner->position = pos;
            preWords=tokenScanner->preReadWords(1);
            if(preWords[0].word==";") {
                now_word=tokenScanner->readNowWord();
                ParserTree::catchTree(root, now_word);
            }else {
                //连接Exp
                ParserTree::catchTree(root,Exp());
                preWords=tokenScanner->preReadWords(1);
                //Exp只有一次，所以预读判断是不是分号
                if(preWords[0].word==";") {
                    now_word=tokenScanner->readNowWord();
                    ParserTree::catchTree(root, now_word);
                }else {
                    ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
                }
            }
        }
    }
    //如果是括号，那么是Block
    else if(preWords[0].word=="{") {
        ParserTree::catchTree(root,Block());
    }
    //如果是if语句
    else if(preWords[0].word=="if") {
        //读if
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //读左括号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root,now_word);
        //连接Cond
        ParserTree::catchTree(root,Cond());
        //预读，判断右小括号
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word!=")") {
            ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
        }else {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }
        ParserTree::catchTree(root,Stmt());
        //预读，看有没有else
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word=="else") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
            ParserTree::catchTree(root,Stmt());
        }
    }
    //如果是for语句
    else if(preWords[0].word=="for") {
        //读for
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //读左括号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root,now_word);
        //预读一个单词，判断是不是ForStmt首字符集
        //根据定义，ForStmt首字符集和LVal一致
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word_type=="IDENFR") {
            //说明是ForStmt
            ParserTree::catchTree(root,ForStmt());
        }
        //连接分号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //由于Cond难判断First集，我决定预读，看下一位是不是分号
        //如果是分号，说明Cond没有写，否则就是Cond
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word!=";") {
            //不是分号，是Cond
            ParserTree::catchTree(root,Cond());
        }
        //连接分号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //预读单词，判断是不是ForStmt
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word_type=="IDENFR") {
            //说明是ForStmt
            ParserTree::catchTree(root,ForStmt());
        }
        //读右括号，根据文法定义，for循环的右括号不会出错
        //保险起见还是先判一下错？代码竞速的环节可以考虑优化一下？
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word!=")") {
            ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
        }
        else {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }
        //连接Stmt
        ParserTree::catchTree(root,Stmt());
    }
    //break和continue
    else if(preWords[0].word=="break" || preWords[0].word=="continue") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //预读，判断有没有分号
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word==";") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }else {
            //没有分号，报错
            ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
        }
    }
    //return语句
    else if(preWords[0].word=="return") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //预读，判断是不是Exp
        //如果直接是分号，那么说明return结束
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word==";") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }else {
            //否则，应当是exp
            ParserTree::catchTree(root,Exp());
            //Exp结束后，预读判断下一位有没有分号
            preWords=tokenScanner->preReadWords(1);
            if(preWords[0].word==";") {
                now_word=tokenScanner->readNowWord();
                ParserTree::catchTree(root, now_word);
            }else {
                //没有分号，报错
                ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
            }
        }
    }
    //printf语句
    else if(preWords[0].word=="printf") {
        //读printf
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //读左小括号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //读StringConst
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root,now_word);
        //预读，如果是逗号要进循环
        preWords=tokenScanner->preReadWords(1);
        while(preWords[0].word==",") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
            //连接Exp
            ParserTree::catchTree(root,Exp());
            //预读
            preWords=tokenScanner->preReadWords(1);
        }
        //判断预读是不是右小括号，否则报错
        if(preWords[0].word==")") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
            //预读，接下来判断是不是分号
            preWords=tokenScanner->preReadWords(1);
        }else {
            ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
        }
        if(preWords[0].word==";") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }else {
            //不是分号，报错
            ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
        }
    }
    //如果不是上述内容，应当隶属于[Exp];
    else {
        if(preWords[0].word==";") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }else {
            //连接Exp
            ParserTree::catchTree(root,Exp());
            preWords=tokenScanner->preReadWords(1);
            //Exp只有一次，所以预读判断是不是分号
            if(preWords[0].word==";") {
                now_word=tokenScanner->readNowWord();
                ParserTree::catchTree(root, now_word);
            }else {
                ErrorPrint::printError(ErrorCategory::semicolon_lack,now_word.line_num);
            }
        }
    }
    return root;
}

TreeNode *Parser::ForStmt() {
    Word word;
    word.word = "<ForStmt>";
    TreeNode* root = ParserTree::createTree(word);
    //连接LVal
    ParserTree::catchTree(root,LVal());
    //连接赋值等号
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root, now_word);
    //连接Exp
    ParserTree::catchTree(root,Exp());
    return root;
}

TreeNode *Parser::Exp() {
    Word word;
    word.word = "<Exp>";
    TreeNode* root = ParserTree::createTree(word);
    //连接AddExp
    ParserTree::catchTree(root,AddExp());
    return root;
}

TreeNode *Parser::Cond() {
    Word word;
    word.word = "<Cond>";
    TreeNode* root = ParserTree::createTree(word);
    //连接LOrExp
    ParserTree::catchTree(root,LOrExp());
    return root;
}

TreeNode *Parser::LVal() {
    Word word;
    word.word = "<LVal>";
    TreeNode* root = ParserTree::createTree(word);
    //第一个是ident，直接连接
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root, now_word);
    //预读，判断是否为左中括号
    preWords=tokenScanner->preReadWords(1);
    if(preWords[0].word=="[") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //连接Exp
        ParserTree::catchTree(root,Exp());
        //预读，检查有没有右中括号
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word=="]") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }else {
            //缺少右中括号，报错
            ErrorPrint::printError(ErrorCategory::r_bracket_lack,now_word.line_num);
        }
    }
    return root;
}

TreeNode *Parser::PrimaryExp() {
    Word word;
    word.word = "<PrimaryExp>";
    TreeNode* root = ParserTree::createTree(word);
    //预读，根据预读情况分类
    preWords=tokenScanner->preReadWords(2);
    if(preWords[0].word=="(") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //连接Exp
        ParserTree::catchTree(root,Exp());
        //预读，判断有没有右小括号
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word==")") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }else {
            //缺少右小括号
            ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
        }
    }
    //如果是ident，就是lVal
    else if(preWords[0].word_type=="IDENFR" || preWords[1].word=="[") {
        //连接LVal
        //TODO：第二个判断条件是为了避免类似于 a[1[0]这种错误调用的
        ParserTree::catchTree(root,LVal());
    }
    //接下来考虑Number
    else if(preWords[0].word_type=="INTCON") {
        //连接Number
        ParserTree::catchTree(root,Number());
    }
    //Character
    else if(preWords[0].word_type=="CHRCON") {
        //连接Character
        ParserTree::catchTree(root,Character());
    }
    else {
        printf("PrimaryExp可能出现错误\n");
    }
    return root;
}

TreeNode *Parser::Number() {
    Word word;
    word.word = "<Number>";
    TreeNode* root = ParserTree::createTree(word);
    //直接连接
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root, now_word);
    return root;
}

TreeNode *Parser::Character() {
    Word word;
    word.word = "<Character>";
    TreeNode* root = ParserTree::createTree(word);
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root, now_word);
    return root;
}

//一元表达式
TreeNode *Parser::UnaryExp() {
    Word word;
    word.word = "<UnaryExp>";
    TreeNode* root = ParserTree::createTree(word);
    //由于PrimaryExp 和函数部分都存在Ident,所以需要预读两位
    preWords=tokenScanner->preReadWords(2);
    if(preWords[0].word_type=="IDENFR" && preWords[1].word=="(") {
        //说明这是函数
        //连接Ident
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //连接左括号
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        //预读，看下一位是右括号还是实参
        preWords=tokenScanner->preReadWords(1);
        if(preWords[0].word!=")") {
            //应当是实参
            ParserTree::catchTree(root, FuncRParams());
            //预读，判断下一位是不是右括号
            preWords=tokenScanner->preReadWords(1);
        }
        if(preWords[0].word==")") {
            now_word=tokenScanner->readNowWord();
            ParserTree::catchTree(root, now_word);
        }else {
            //没有小括号，报错
            ErrorPrint::printError(ErrorCategory::r_parenthesis_lack,now_word.line_num);
        }
    }
    else if(preWords[0].word=="+"||preWords[0].word=="-"||preWords[0].word=="!") {
        //连接一元运算符
        ParserTree::catchTree(root, UnaryOp());
        //右递归，连接UnaryExp
        ParserTree::catchTree(root, UnaryExp());
    }
    else {
        //说明是PrimaryExp
        ParserTree::catchTree(root, PrimaryExp());
    }
    return root;
}

TreeNode *Parser::UnaryOp() {
    Word word;
    word.word = "<UnaryOp>";
    TreeNode* root = ParserTree::createTree(word);
    now_word=tokenScanner->readNowWord();
    ParserTree::catchTree(root, now_word);
    return root;
}

TreeNode *Parser::FuncRParams() {
    Word word;
    word.word = "<FuncRParams>";
    TreeNode* root = ParserTree::createTree(word);
    //连接Exp
    ParserTree::catchTree(root,Exp());
    //预读，判断是不是逗号，进循环
    preWords=tokenScanner->preReadWords(1);
    while(preWords[0].word==",") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,Exp());
        preWords=tokenScanner->preReadWords(1);
    }
    return root;
}

//以下内容涉及到左递归，需要进行改造
TreeNode *Parser::MulExp() {
    Word word;
    word.word = "<MulExp>";
    TreeNode* root = ParserTree::createTree(word);
    /* UnaryExp {('*' | '/' | '%') UnaryExp} */
    //先假设是UnaryExp，如果分析完之后后面有乘除符号，就说明假设错误，需要回溯
    //考虑到回溯对时间的消耗，不如手动建树
    int pos = tokenScanner->position;
    //TreeNode* right = UnaryExp();
    ParserTree::catchTree(root,UnaryExp());
    //预读
    preWords=tokenScanner->preReadWords(1);
    //判断，如果是这些符号，说明应当，按照MulExp处理
    while(preWords[0].word=="*"||preWords[0].word=="/"||preWords[0].word=="%") {
        //回退
        //tokenScanner->position = pos;
        //按照Exp处理
        //ParserTree::catchTree(root,MulExp());
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,UnaryExp());
        preWords=tokenScanner->preReadWords(1);
    }
    root = ParserTree::adjustTree(root,word);
    return root;
}

TreeNode *Parser::AddExp() {
    Word word;
    word.word = "<AddExp>";
    TreeNode* root = ParserTree::createTree(word);
    /*MulExp {('+' | '−') MulExp}*/
    //连接MulExp
    ParserTree::catchTree(root,MulExp());
    //预读
    preWords=tokenScanner->preReadWords(1);
    //循环
    while(preWords[0].word=="+"||preWords[0].word=="-") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,MulExp());
        preWords=tokenScanner->preReadWords(1);
    }
    root = ParserTree::adjustTree(root,word);
    return root;
}

TreeNode *Parser::RelExp() {
    Word word;
    word.word = "<RelExp>";
    TreeNode* root = ParserTree::createTree(word);
    /*AddExp {('<' | '>' | '<=' | '>=') AddExp }*/
    //连接AddExp
    ParserTree::catchTree(root,AddExp());
    //预读
    preWords=tokenScanner->preReadWords(1);
    while(preWords[0].word=="<"||preWords[0].word==">"||preWords[0].word=="<="||preWords[0].word==">=") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,AddExp());
        preWords=tokenScanner->preReadWords(1);
    }
    root = ParserTree::adjustTree(root,word);
    return root;
}

TreeNode *Parser::EqExp() {
    Word word;
    word.word = "<EqExp>";
    TreeNode* root = ParserTree::createTree(word);
    /*RelExp {('==' | '!=') RelExp}*/
    //连接RelExp
    ParserTree::catchTree(root,RelExp());
    preWords=tokenScanner->preReadWords(1);
    while(preWords[0].word=="=="||preWords[0].word=="!=") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,RelExp());
        preWords=tokenScanner->preReadWords(1);
    }
    root = ParserTree::adjustTree(root,word);
    return root;
}

TreeNode *Parser::LAndExp() {
    Word word;
    word.word = "<LAndExp>";
    TreeNode* root = ParserTree::createTree(word);
    /*EqExp {'&&' EqExp}*/
    ParserTree::catchTree(root,EqExp());
    preWords=tokenScanner->preReadWords(1);
    while(preWords[0].word=="&&") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,EqExp());
        preWords=tokenScanner->preReadWords(1);
    }
    root = ParserTree::adjustTree(root,word);
    return root;
}

TreeNode *Parser::LOrExp() {
    Word word;
    word.word = "<LOrExp>";
    TreeNode* root = ParserTree::createTree(word);
    /*LAndExp {'||' LAndExp}*/
    ParserTree::catchTree(root,LAndExp());
    preWords=tokenScanner->preReadWords(1);
    while(preWords[0].word=="||") {
        now_word=tokenScanner->readNowWord();
        ParserTree::catchTree(root, now_word);
        ParserTree::catchTree(root,LAndExp());
        preWords=tokenScanner->preReadWords(1);
    }
    root = ParserTree::adjustTree(root,word);
    return root;
}

TreeNode *Parser::ConstExp() {
    Word word;
    word.word="<ConstExp>";
    TreeNode* root = ParserTree::createTree(word);
    ParserTree::catchTree(root,AddExp());
    return root;
}
