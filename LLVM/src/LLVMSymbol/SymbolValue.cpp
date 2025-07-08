//
// Created by PengXinyang on 24-11-24.
//

#include "LLVMSymbol/SymbolValue.h"

#include "generate/LLVMGenerate.h"
#include "LLVMSymbol/SymbolCalculate.h"
#include "type/IRName.h"
#include "type/irType/IRArray.h"
#include "value/architecture/ConstValue.h"

SymbolValue* SymbolValue::instance = nullptr;
SymbolValue::SymbolValue() {
    now_symbol_table = LLVMGenerate::getInstance()->getNowSymbolTable();
    generate = LLVMGenerate::getInstance();
}

SymbolValue *SymbolValue::getInstance() {
    if(instance == nullptr) {
        instance = new SymbolValue();
    }
    return instance;
}

/*Symbol *SymbolValue::ConnectSymbolValue(TreeNode *root) {
    if(root == nullptr) { return nullptr; }
    if(root->word.word=="<ConstDef>") return ConnectConstDef(root);
    if(root->word.word=="<VarDef>") return ConnectVarDef(root);
    if(root->word.word=="<FuncDef>") return ConnectFuncDef(root);
    if(root->word.word=="MainFuncDef") return ConnectMainFuncDef(root);
    if(root->word.word=="FuncFParam") return ConnectFuncFParam(root);
    return nullptr;
}*/

vector<Value *> SymbolValue::genConstValuesIR(TreeNode *root) {
    vector<Value *> result;
    if(root->word.word == "<ConstInitVal>") {
        for(const auto son:root->sonNode) {
            Value*  value = generate->generateLLVMIR(son);
            if(value) {
                result.push_back(value);
            }
        }
    }
    return result;
}
vector<Value *> SymbolValue::genValuesIR(TreeNode *root) {
    vector<Value *> result;
    if(root->word.word == "<InitVal>") {
        for(const auto son:root->sonNode) {
            Value*  value = generate->generateLLVMIR(son);
            if(value) {
                result.push_back(value);
            }
        }
    }
    return result;
}