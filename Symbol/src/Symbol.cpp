//
// Created by PengXinyang on 24-10-12.
//

#include <utility>

#include "../include/Symbol.h"

#include <bits/locale_facets_nonio.h>

#include "../../Lexer/include/Lexer.h"
#include "type/IRName.h"
#include "type/irType/IRArray.h"
#include "type/irType/IRChar.h"
#include "type/irType/IRInt.h"
#include "type/irType/IRVoid.h"
#include "value/architecture/ConstValue.h"
#include "value/architecture/user/GlobalVariable.h"

Symbol::Symbol() = default;
SymbolTable::SymbolTable() = default;
SymbolTable::SymbolTable(const int table_id, const shared_ptr<SymbolTable> &father_ptr) {
    this->table_id = table_id;
    this->father_ptr = father_ptr;
    this->layer = father_ptr->layer + 1;
}

bool Symbol::is_in_global_table() const {
    //全局符号表的id一定是1
    return table_id == 1;
}
IRType *Symbol::get_ir_type() {
    if(ir_type!=nullptr) return ir_type;
    if(btype == 0 || func_type == 0) {
        //说明是int
        ir_type = new IRInt();
    }else if(btype == 1 || func_type == 1) {
        //说明是char
        ir_type = new IRChar();
    }else if(func_type == 2) {
        //说明是void
        ir_type = new IRVoid();
    }
    return ir_type;
}

void Symbol::connect_value() {
    if(is_in_global_table()) {
        //说明在全局符号表中，应当标记为全局符号
        //全局符号的命名需要加前缀
        IRType *ir_type = get_ir_type();
        if(is_const) {
            if(is_var) {
                //说明是单元素常量
                //在这里先不处理值，之后遍历语法树的时候再处理
                string value_num = to_string(var_value);
                value = new ConstValue(ir_type,value_num);
            }
            else if(is_array) {
                //说明是数组
                vector<int> dims(2);dims[0] = dim;
                auto *ir_array = new IRArray(ir_type,dims);
                //将数组和常量字符串赋到init_var中
                auto* init_var = new InitVar(ir_type,array_values,const_string);
                value = new GlobalVariable(ir_array,IRName::getGlobalVariableName(token),init_var,true);
            }
        }else if(is_var) {
            //说明是变量，并且是全局变量，全局变量赋值原则上是常值
            vector<int> initValues;
            initValues.push_back(var_value);
            auto* init_var = new InitVar(ir_type,initValues,"");
            value = new GlobalVariable(ir_type,IRName::getGlobalVariableName(token),init_var);
        }else if(is_array) {
            //说明是全局数组
            vector<int> dims(2);dims[0] = dim;
            auto *ir_array = new IRArray(ir_type,dims);
            //将数组和常量字符串赋到init_var中
            auto* init_var = new InitVar(ir_type,array_values,const_string);
            value = new GlobalVariable(ir_array,IRName::getGlobalVariableName(token),init_var);
        }else if(is_func) {
            //说明是函数
            value = new Function(IRName::getFunctionName(token),ir_type);
        }
    }
    //TODO: 对于局部变量，涉及到存储，分配等指令，计划在生成LLVM指令部分再确定Value
    /*else {
        //说明不是全局符号表，那么根据文法，在一般符号表中，只能出现变量和数组的符号
        if(is_var) {
            //是局部变量，局部变量需要保存当前所在的函数信息
        }else if(is_func_param) {
            //是函数参数，主要是数组类型在这里要赋成指针类型
        }else if(is_array) {
            //数组类型，但也属于局部
        }
    }*/
}

void SymbolTable::print_symbol_table(FILE* fp) {
    for(const auto &token:token_vector) {
        string identType;
        if(symbol_table[token].is_func) {
            //如果是函数
            string bType;
            if(symbol_table[token].func_type==0) {
                bType = "Int";
            }else if(symbol_table[token].func_type==1) {
                bType = "Char";
            }else if(symbol_table[token].func_type==2) {
                bType = "Void";
            }
            identType = bType+"Func";
        }
        else {
            //如果不是函数，那么就是const，类型，和数组的组合
            string bType = symbol_table[token].btype==1?"Char":"Int";//0是int 1是char
            string Const = symbol_table[token].is_const?"Const":"";
            string Array = symbol_table[token].is_array?"Array":"";
            identType = Const+bType;
            identType.append(Array);
        }
        fprintf(fp,"%d %s %s\n",table_id,token.c_str(),identType.c_str());
    }
}

void SymbolTable::print_all_symbol_table(FILE *fp) {
    print_symbol_table(fp);
    for(const auto& children: children_ptr) {
        children->print_all_symbol_table(fp);
    }
}

bool SymbolTable::is_in_table(const string& token) {
    if(symbol_table.find(token) == symbol_table.end()) {
        return false;
    }
    return true;
}

bool SymbolTable::is_in_all_table(const string &token) {
    if(is_in_table(token)) {return true;}
    if(father_ptr==nullptr){ return false;}//当前节点没有，父节点为空，说明找到头了，确实没有这个符号
    return father_ptr->is_in_all_table(token);//当前表没有，父节点不为空，递归向上找
}

Symbol* SymbolTable::get_symbol(const string &token) {
    if(is_in_table(token)) {
        return &symbol_table[token];
    }
    return nullptr;
}

Symbol *SymbolTable::get_symbol_in_all_table(const string &token) {
    if(is_in_table(token)) {
        return &symbol_table[token];
    }
    if(father_ptr==nullptr) {return nullptr;}
    return father_ptr->get_symbol_in_all_table(token);
}

Symbol *SymbolTable::get_symbol_in_all_table(const string &token, const string &token_known) {
    //使用token_vector，找known_token之前的符号
    auto it_token_known = find(token_vector.begin(),token_vector.end(),token_known);
    auto it_token = find(token_vector.begin(),it_token_known,token);
    if(it_token!=token_vector.end()) {
        //说明可以找到，直接返回
        return &symbol_table[token];
    }
    //没有找到，查找前面的表。前面的表没有限制，因为代码生成时所有符号肯定在前面出现过
    if(father_ptr==nullptr) {return nullptr;}
    return father_ptr->get_symbol_in_all_table(token);
}

Symbol *SymbolTable::get_symbol_in_all_table(const Word& word, int line_num) {
    Symbol* symbol = nullptr;
    string token = word.word;
    if(is_in_table(token)) {
        //如果在当前的表中，需要判断它的行号有没有超
        symbol =  &symbol_table[token];
        //如果查到的符号行号大于要求的行号，说明它的定义在目标行之后，
        //我们应当找父符号表定义的符号
        if(symbol->lineNum>line_num) {
            symbol = nullptr;
        }
        else if(symbol->lineNum==line_num) {
            //TODO：行号相等，由于存在同一行先使用后定义符号的情况，需要进一步排查。
            //这个时候，由于列号小，说明不应该找局部的符号
            if(word.pos<symbol->pos) {
                symbol = nullptr;
            }
        }
    }
    if(symbol!=nullptr) {
        //说明当前表中的符号，在该行之前已经定义
        return  symbol;
    }
    if(father_ptr==nullptr) {return nullptr;}
    return father_ptr->get_symbol_in_all_table(token);
}


Symbol *SymbolTable::get_last_symbol() {
    if(token_vector.empty()) {return nullptr;}
    return &symbol_table[token_vector.back()];
}

bool SymbolTable::is_in_var_table(const string &token) {
    if(is_in_table(token)) {
        if(const Symbol* symbol = get_symbol(token); symbol != nullptr && symbol->is_var) {
            return true;
        }
    }
    return false;
}
bool SymbolTable::is_in_func_table(const string &token) {
    if(is_in_table(token)) {
        if(const Symbol* symbol = get_symbol(token); symbol != nullptr && symbol->is_func) {
            return true;
        }
    }
    return false;
}

bool SymbolTable::is_in_array_table(const string &token) {
    if(is_in_table(token)) {
        if(const Symbol* symbol = get_symbol(token); symbol != nullptr && symbol->is_array) {
            return true;
        }
    }
    return false;
}

bool SymbolTable::is_const(const string &token) {
    if(is_in_table(token)) {
        return symbol_table[token].is_const;
    }
    if(father_ptr==nullptr) {
        return false;
    }
    return father_ptr->is_const(token);
}

void SymbolTable::add_symbol(const string& token, Symbol &symbol) {
    token_vector.push_back(token);
    //符号表的id就是添加顺序，也就是符号map的元素个数，从0开始
    symbol.symbol_id = static_cast<int>(symbol_table.size());
    symbol.table_id = table_id;
    symbol_table[token] = symbol;
}

void SymbolTable::add_var_symbol(const Word& word, const int btype, const int var_value, const bool is_const) {
    Symbol symbol;
    symbol.token = word.word;
    symbol.lineNum = word.line_num;
    symbol.pos = word.pos;
    symbol.btype = btype;
    symbol.var_value = var_value;
    symbol.is_const = is_const;
    symbol.is_var = true;
    symbol.type = 0;
    add_symbol(symbol.token, symbol);
}

void SymbolTable::add_func_symbol(const Word& word, const int func_type, const int param_num, const vector<int> &param_type) {
    Symbol symbol;
    symbol.token = word.word;
    symbol.lineNum = word.line_num;
    symbol.pos = word.pos;
    symbol.func_type = func_type;
    symbol.param_num = param_num;
    symbol.param_type = param_type;
    symbol.type = 2;
    symbol.is_func = true;
    add_symbol(symbol.token, symbol);
}

void SymbolTable::add_func_param_symbol(const Word &word, const int btype, const bool is_array) {
    Symbol symbol;
    symbol.token = word.word;
    symbol.lineNum = word.line_num;
    symbol.pos = word.pos;
    symbol.btype = btype;
    symbol.is_array = is_array;
    symbol.is_var = !is_array;//是数组就不是var，是var就不是数组
    symbol.type = is_array?1:0;
    symbol.is_func_param = true;
    add_symbol(symbol.token, symbol);
}

void SymbolTable::add_array_symbol(const Word& word, const int btype, const int dim, const bool is_const) {
    Symbol symbol;
    symbol.token = word.word;
    symbol.lineNum = word.line_num;
    symbol.pos = word.pos;
    symbol.dim = dim;
    symbol.btype = btype;
    symbol.is_const = is_const;
    symbol.is_array = true;
    symbol.type = 1;
    add_symbol(symbol.token, symbol);
}

//TODO:这里需要判断一下，是在本符号表找符号，还是在所有符号表中找符号
void SymbolTable::set_var_value(const string &token, const int var_value) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_var) {
        symbol->var_value = var_value;
    }
}

void SymbolTable::set_int_array_value(const string &token, const vector<int> &array_value) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_array) {
        if(symbol->btype ==0) {
            symbol->array_values = array_value;
        }
    }
}

void SymbolTable::set_int_array_value(const string &token, const int array_value) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_array) {
        if(symbol->btype ==0) {
            symbol->array_values.push_back(array_value);
        }
    }
}

void SymbolTable::set_char_array_value(const string &token, const string &array_value) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_array) {
        if(symbol->btype ==1) {
            symbol->const_string = array_value;
        }
    }
}

void SymbolTable::set_char_array_value(const string &token, const char array_value) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_array) {
        if(symbol->btype ==1) {
            symbol->array_values.push_back(array_value);
        }
    }
}

void SymbolTable::set_array_dim(const string &token, const int dim) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_array) {
        symbol->dim = dim;
    }
}

void SymbolTable::set_is_return(const string &token, bool is_return) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_func) {
        symbol->is_return = is_return;
    }
}

void SymbolTable::set_is_return(Symbol *func_symbol, const bool is_return) {
    if(func_symbol != nullptr && func_symbol->is_func) {
        func_symbol->is_return = is_return;
    }
}

void SymbolTable::set_func_return(const string &token, int func_value) {
    Symbol* symbol = get_symbol_in_all_table(token);
    if(symbol != nullptr && symbol->is_func) {
        symbol->func_value = func_value;
    }
}

void SymbolTable::set_func_return(Symbol *func_symbol, int func_value) {
    if(func_symbol != nullptr && func_symbol->is_func) {
        func_symbol->func_value = func_value;
    }
}
