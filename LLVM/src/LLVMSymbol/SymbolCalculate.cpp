//
// Created by PengXinyang on 24-11-25.
//

#include "../../include/LLVMSymbol/SymbolCalculate.h"

#include "../../../Symbol/include/Symbol.h"
#include "generate/LLVMGenerate.h"

SymbolCalculate* SymbolCalculate::instance;
SymbolCalculate* SymbolCalculate::getInstance() {
    if (!instance) {
        instance = new SymbolCalculate();
    }
    return instance;
}
int SymbolCalculate::calculate(TreeNode *root) {
    if(root==nullptr) return 0;
    if(root->word.word=="<Exp>"||root->word.word=="<ConstExp>") {
        return calculate(root->sonNode[0]);
    }
    if(root->word.word=="<AddExp>") {
        return calculateAddExp(root);
    }
    if(root->word.word=="<MulExp>") {
        return calculateMulExp(root);
    }
    if(root->word.word=="<UnaryExp>") {
        return calculateUnaryExp(root);
    }
    if(root->word.word=="<PrimaryExp>") {
        return calculatePrimaryExp(root);
    }
    if(root->word.word=="<Number>" || root->word.word=="<Character>") {
        return calculateNumberOrChar(root);
    }
    if(root->word.word=="<LVal>") {
        return calculateLVal(root);
    }
    return 1;
}

vector<int> SymbolCalculate::calculateInitVal(TreeNode *root) {
    vector<int> initVal;
    if(root->word.word=="<InitVal>") {
        for(auto* sonNode : root->sonNode) {
            if(sonNode->word.word=="<Exp>") {
                initVal.push_back(calculate(sonNode));
            }
        }
    }
    return initVal;
}

vector<int> SymbolCalculate::calculateConstInitVal(TreeNode *root) {
    vector<int> initVal;
    if(root->word.word=="<ConstInitVal>") {
        for(auto* sonNode : root->sonNode) {
            if(sonNode->word.word=="<ConstExp>") {
                initVal.push_back(calculate(sonNode));
            }
        }
    }
    return initVal;
}

int SymbolCalculate::calculateMulExp(TreeNode *root) {
    int mulExp = calculate(root->sonNode[0]);
    for(int i=1;i<root->son_num;++i) {
        //i是奇数，一定是符号；++i就是对应要算的下一项
        if(root->sonNode[i]->word.word=="*") {
            mulExp*=calculate(root->sonNode[++i]);
        }else if(root->sonNode[i]->word.word=="/") {
            int num = calculate(root->sonNode[++i]);
            if(num == 0) num = 1;
            mulExp/=num;
        }else if(root->sonNode[i]->word.word=="%") {
            int num = calculate(root->sonNode[++i]);
            if(num == 0) num = 1;
            mulExp%=num;
        }
    }
    return mulExp;
}

int SymbolCalculate::calculateAddExp(TreeNode *root) {
    int addExp = calculate(root->sonNode[0]);
    for(int i=1;i<root->son_num;++i) {
        if(root->sonNode[i]->word.word=="+") {
            addExp+=calculate(root->sonNode[++i]);
        }else if(root->sonNode[i]->word.word=="-") {
            addExp-=calculate(root->sonNode[++i]);
        }
    }
    return addExp;
}

int SymbolCalculate::calculateUnaryExp(TreeNode *root) {
    int ans = 0;
    TreeNode* son = root->sonNode[0];
    if(son->word.word=="<UnaryOp>") {
        string type = son->sonNode[0]->word.word;
        if(type=="+") {
            ans = calculate(root->sonNode[1]);
        }else if(type=="-") {
            ans = - calculate(root->sonNode[1]);
        }else if(type=="!") {
            ans = calculate(root->sonNode[1]) == 0? 1:0;
        }
    }else if(son->word.word=="<PrimaryExp>") {
        ans = calculatePrimaryExp(son);
    }
    return ans;
}

int SymbolCalculate::calculatePrimaryExp(TreeNode *root) {
    int ans = 0;
    if(root->sonNode[0]->word.word=="<Number>" || root->sonNode[0]->word.word=="<Character>" || root->sonNode[0]->word.word=="<LVal>") {
        ans = calculate(root->sonNode[0]);
    }else ans = calculate(root->sonNode[1]);
    return ans;
}

int SymbolCalculate::calculateNumberOrChar(TreeNode *root) {
    if(root->word.word=="<Number>") {
        return stoi(root->sonNode[0]->word.word);
    }
    if(root->word.word=="<Character>") {
        int num = root->sonNode[0]->word.word[1];
        if(num == '\\') {
            //看看有没有下一个字符
            if(root->sonNode[0]->word.word.length() > 2) {
                char c = root->sonNode[0]->word.word[2];
                //根据c的单词决定num的值
                switch(c) {
                    case 'a': num = 7;break;
                    case 'b': num = 8;break;
                    case 't': num = 9;break;
                    case 'n': num = 10;break;
                    case 'v': num = 11;break;
                    case 'f': num = 12;break;
                    case '\"': num = 34;break;
                    case '\'': num = 39;break;
                    case '\\': num = 92;break;
                    case '0': num = 0;break;
                    default: num = 92;
                }
            }
        }
        return num;
    }
    return 0;
}

// LVal → Ident ['[' Exp ']']
//由于这个类的计算是计算常数的，所以
int SymbolCalculate::calculateLVal(TreeNode *root) {
    //获取Ident符号，由于是求值，不是定义，所以需要找全局符号表
    Symbol* symbol = LLVMGenerate::getInstance()
    ->getNowSymbolTable()
    ->get_symbol_in_all_table(root->sonNode[0]->word,root->sonNode[0]->word.line_num);
    if(symbol == nullptr) { return 0; }
    vector<int> args;
    for(int i=1;i<root->son_num;++i) {
        if(root->sonNode[i]->word.word=="<Exp>") {
            //实际上根据文法，最多只有一个Exp
            args.push_back(calculate(root->sonNode[i]));
        }
    }
    if(symbol->is_var) {
        //只是一般变量
        return symbol->var_value;
    }
    //否则，是一维数组
    if(!symbol->array_values.empty()) {
        int index = args[0];
        return symbol->array_values[index];
    }
    //如果是char，可能要根据string读
    if(symbol->btype == 1) {
        if(!symbol->const_string.empty()) {
            int index = args[0];
            //TODO:处理转义字符
            return symbol->const_string[index];
        }
    }
    return 0;
}
