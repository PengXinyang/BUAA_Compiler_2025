//
// Created by PengXinyang on 24-11-23.
//

#include "value/architecture/user/Function.h"

#include <sstream>

#include "generate/MipsCell.h"
#include "optimize/LLVMOptimizerInit.h"
#include "optimize/MemToReg.h"
#include "optimize/data_structure/DominantTree.h"
#include "optimize/Tool/ActiveVarAnalysis.h"
#include "Register/RegisterController.h"
#include "Register/RegisterDistribute.h"
#include "Register/RegisterTool.h"
#include "structure/text/MipsBlock.h"
#include "type/IRName.h"
#include "type/irType/IRBlock.h"

Function::Function(const string& name, IRType* returnType):GlobalValue(new IRBlock("function"),name){
    this->returnType = returnType;
    basicBlocks = vector<BasicBlock*>();
    params = vector<Param*>();
    valueRegisterMap = unordered_map<Value*, Register*>();
    if(!LLVMOptimizerInit::isOptimize()) {
        IRName::addFunction(this);
    }
    //开启优化，添加控制流图
    cfgGraph = new CfgGraph(this);
    dominantTree = new DominantTree(this);
    activeVarAnalysis = new ActiveVarAnalysis(this);
}

void Function::addParam(Param *param) {
    this->params.push_back(param);
    param->setFunc(this);
}
void Function::addBasicBlock(BasicBlock *basicBlock, int index) {
    if(index>=0 && index < basicBlocks.size()) {
        basicBlocks.insert(basicBlocks.begin()+index,basicBlock);
    }
    else {
        basicBlocks.push_back(basicBlock);
    }
    basicBlock->setFunctionParent(this);
}

string Function::toLLVM() {
    /**
     * 示例：
     * define dso_local i32 @func() {
     *     store i32 2, i32* @a
     *     ret i32 1
     * }
     */
    ostringstream os;
    //首先添加第一行前缀部分
    //注意，”@“包含在了value_name中
    os<<"define dso_local "<<returnType->toLLVM()<<" "<<value_name<<"(";
    //添加函数的参数
    const int param_num = static_cast<int>(params.size());
    for(int i=0;i<param_num;i++) {
        os<<params[i]->toLLVM();
        if(i<param_num-1) os<<", ";
    }
    //添加参数后到第一行最右侧
    os<<") {\n";
    const int basic_block_num = static_cast<int>(basicBlocks.size());
    for(int i=0;i<basic_block_num;i++) {
        os<<basicBlocks[i]->toLLVM();
        if(i<basic_block_num-1) os<<"\n\n";
    }
    os<<"\n}";
    return os.str();
}

void Function::generateMIPS() {
    new MipsBlock(value_name.substr(1));
    MipsCell::resetFunction(this);
    int num = min(3,static_cast<int>(params.size()));
    for(int i=0;i<num;++i) {
        //前三个参数只需要压栈，由a1-a3寄存器保存
        RegisterController::allocateRegister(
            params[i],
            Register::getRegister(Register::regTransform(static_cast<int>(RegisterName::$a0) + i+1))
        );
        RegisterTool::moveValue(params[i]);
    }
    for(int i=3;i<params.size();++i) {
        RegisterTool::moveValue(params[i]);
    }
    for(const auto basicBlock:basicBlocks) {
        basicBlock->generateMIPS();
    }
}
void Function::DCEBlockInstruction() {
    for(const auto basicBlock:basicBlocks) {
        basicBlock->DCEBlockInstruction();
    }
}

void Function::buildCfgGraph() const {
    if(cfgGraph) {
        cfgGraph->buildCFG();
    }
}

CfgGraph *Function::getCfgGraph() const {
    return cfgGraph;
}

DominantTree *Function::getDominantTree() const {
    return dominantTree;
}

void Function::buildDominantTree() const {
    if(dominantTree) {
        //生成支配集
        dominantTree->generateDominateBlocks();
        //生成支配树
        dominantTree->generateDominantTree();
        //生成支配边界
        dominantTree->generateDominateEdge();
    }
}

void Function::insertPhiInstruction() {
    //首先，设置起始基本块为函数的第一个基本块
    MemToReg::setFirstBlock(basicBlocks[0]);
    //遍历所有基本块，插入phi函数
    for(const auto basicBlock:basicBlocks) {
        basicBlock->insertPhiInstruction();
    }
}

void Function::deletePhiInstruction() {
    //复制副本
    auto basic_blocks = vector(basicBlocks.begin(), basicBlocks.end());
    for(const auto basic_block:basic_blocks) {
        basic_block->PhiToPC();
    }
    for(const auto basic_block:basicBlocks) {
        basic_block->PCToMove();
    }
}

void Function::activeAnalysis() const {
    if(activeVarAnalysis) {
        //首先，让所有基本块构建use-def集
        activeVarAnalysis->generateUseDef();
        //然后，构建in-out集
        activeVarAnalysis->generateInOut();
    }
}

ActiveVarAnalysis *Function::getActiveVarAnalysis() const {
    return activeVarAnalysis;
}

void Function::registerDistribute() {
    RegisterDistribute::setRegisterToValueMap(unordered_map<Register*, Value*>());
    RegisterDistribute::setValueToRegisterMap(unordered_map<Value*, Register*>());
    RegisterDistribute::distributeInBasicBlock(basicBlocks[0]);
    valueRegisterMap = RegisterDistribute::getValueToRegisterMap();
}

string Function::printRegisterDistribute() {
    ostringstream oss;
    for(const auto [fst, snd] : valueRegisterMap) {
        oss<<fst->value_name<<" 分配为 "<<snd->toMips()<<"\n\t\t";
    }
    return oss.str();
}
