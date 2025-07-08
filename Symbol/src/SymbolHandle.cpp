//
// Created by PengXinyang on 24-10-12.
//

#include "../include/SymbolHandle.h"

#include "../../ErrorHandle/include/ErrorPrint.h"

SymbolHandle* SymbolHandle::instance;
SymbolHandle::SymbolHandle() {
    Parser* parser = Parser::getParserInstance();
    paserTree = parser->getPaserTree();
    symbol_table_id = 0;
}

SymbolHandle *SymbolHandle::getSymbolHandleInstance() {
    if(instance == nullptr) {
        instance = new SymbolHandle();
    }
    return instance;
}

void SymbolHandle::handleSymbol() {
    if(paserTree==nullptr) {
        paserTree = Parser::getParserInstance()->getPaserTree();
    }
    SymbolCompUnit(paserTree);
}

void SymbolHandle::printSymbol() {
    FILE*fp = fopen("symbol.txt","w");
    if(fp!=nullptr&&GlobalSymbolTable!=nullptr) {
        GlobalSymbolTable->print_all_symbol_table(fp);
    }
    fclose(fp);
}

shared_ptr<SymbolTable> SymbolHandle::getSymbolTable() {
    return GlobalSymbolTable;
}


shared_ptr<SymbolTable> SymbolHandle::createSymbolTable() {
    shared_ptr<SymbolTable> symbol_table = nullptr;
    if(GlobalSymbolTable!=nullptr) {
        const auto father_table =nowSymbolTable;//当前所在的符号表，应当作为父亲符号表
        symbol_table = make_shared<SymbolTable>(*new SymbolTable(++symbol_table_id, father_table));
        father_table->children_ptr.push_back(symbol_table);
        nowSymbolTable = symbol_table;
        //如果当前在函数内，给这个符号表标记一个函数名
        nowSymbolTable->func_token = func_token;
    }else {
        //如果主符号表是空的，说明此时还没有创建全局符号表，否则直到退出时符号表栈才消失
        symbol_table = make_shared<SymbolTable>(*new SymbolTable());
        GlobalSymbolTable = symbol_table;
        nowSymbolTable = symbol_table;
        symbol_table_id+=1;
    }
    return symbol_table;
}

void SymbolHandle::SymbolCompUnit(TreeNode *root) {
    //处理整个编译单元
    //此时的root是整个语法树的根节点
    shared_ptr<SymbolTable> symbol_table = createSymbolTable();
    vector<TreeNode*> son_nodes = root->sonNode;
    for(const auto son: son_nodes) {
        if(son->word.word=="<Decl>") SymbolDecl(son);
        else if(son->word.word=="<FuncDef>") SymbolFuncDef(son);
        else if(son->word.word=="<MainFuncDef>") SymbolMainFuncDef(son);
    }
}

void SymbolHandle::SymbolDecl(TreeNode *root) {
    //这个是处理声明的，对于声明而言，一定是变量或数组，当然也包括const，因为不涉及Block的增加，所以不用创建新的符号表
    vector<TreeNode*> son_nodes = root->sonNode;
    for(const auto son: son_nodes) {
        if(son->word.word=="<ConstDecl>") SymbolConstDecl(son);
        else if(son->word.word=="<VarDecl>") SymbolVarDecl(son);
    }
}

void SymbolHandle::SymbolConstDecl(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    bool is_const = true;//第0个节点是const
    int btype = SymbolBtype(son_nodes[1]);//第1个节点是btype,0是int 1是char
    //从这里开始，就是ConstDef。
    for(int i=2;i<root->son_num;i+=2) {
        //i+=2是因为要隔开中间的逗号
        if(son_nodes[i]->word.word=="<ConstDef>") {
            SymbolConstDef(son_nodes[i],is_const,btype);
        }
    }
}

int SymbolHandle::SymbolBtype(const TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    return son_nodes[0]->word.word=="char"?1:0;//0是int 1是char
}

void SymbolHandle::SymbolConstDef(TreeNode *root, bool is_const, int btype) {
    vector<TreeNode*> son_nodes = root->sonNode;
    Word Ident = son_nodes[0]->word;
    string token = Ident.word;
    //注意，这里token可能会存在重定义的错误
    if(nowSymbolTable->is_in_table(token)) {
        //如果这个单词在当前符号表已经找到
        ErrorPrint::printError(ErrorCategory::redefine,Ident.line_num);
        token = "";//将token赋为空，后续处理不赋值
        //TODO:这里存在一个问题，暂时不清楚如果重定义，后面是否还需要分析
        for(TreeNode* son: son_nodes) {
            if(son->word.word=="<ConstExp>") {
                SymbolConstExp(son,token);
            }else if(son->word.word=="<ConstInitVal>") {
                SymbolConstInitVal(son,token);
            }
        }
        return;
    }
    if(son_nodes[1]->word.word == "[") {
        //说明是数组
        //TODO:dim维度由ConstExp决定，可以等之后求出来，当前只需要符号表暂时还不需要
        int dim = 0;
        nowSymbolTable->add_array_symbol(Ident,btype,dim,is_const);
        if(son_nodes[2]->word.word=="<ConstExp>") {
            SymbolConstExp(son_nodes[2],token);//第二个一定是ConstExp
        }
        //接下来，读到等号，因为右括号可能不存在
        if(son_nodes[3]->word.word == "]") {
            //第5个是ConstInitVal
            SymbolConstInitVal(son_nodes[5],token);
        }else {
            //没有右括号，所以只有五个节点，其中4是ConstInitVal
            SymbolConstInitVal(son_nodes[4],token);
        }
        //TODO:参数的值与ConstInitVal有关
    }else {
        //否则，这不是数组
        //TODO:参数的值与ConstInitVal有关
        int var_value = 0;
        nowSymbolTable->add_var_symbol(Ident,btype,var_value,is_const);
        //第二个节点是ConstInitVal
        SymbolConstInitVal(son_nodes[2],token);
    }
}

void SymbolHandle::SymbolConstInitVal(TreeNode *root, const string &token) {
    vector<TreeNode*> son_nodes = root->sonNode;
    if(son_nodes[0]->word.word_type=="STRCON") {
        //说明是StringConst，自然就是字符数组
        string strCon = son_nodes[0]->word.word;
        nowSymbolTable->set_char_array_value(token,strCon);
    }else if(son_nodes[0]->word.word=="{") {
        //说明是第二种形式，'{' [ ConstExp { ',' ConstExp } ] '}'
        for(int i=1;i<root->son_num-1;i+=2) {
            //从第1个开始，+=2为了跳过“，”
            if(son_nodes[i]->word.word=="<ConstExp>") {
                SymbolConstExp(son_nodes[i],token);
            }
        }
    }else {
        //第一种形式，ConstExp
        SymbolConstExp(son_nodes[0],token);
    }
}
void SymbolHandle::SymbolVarDecl(TreeNode *root) {
    bool is_const = false;
    vector<TreeNode*> son_nodes = root->sonNode;
    int btype = SymbolBtype(son_nodes[0]);
    for(int i=1;i< root->son_num;i+=2) {
        SymbolVarDef(son_nodes[i],is_const,btype);
    }
}

void SymbolHandle::SymbolVarDef(TreeNode *root, bool is_const, int btype) {
    vector<TreeNode*> son_nodes = root->sonNode;
    //第0位一定是Ident
    Word Ident = son_nodes[0]->word;
    string token = Ident.word;
    if(nowSymbolTable->is_in_table(token)) {
        ErrorPrint::printError(ErrorCategory::redefine,Ident.line_num);
        token = "";
        for(const auto son: son_nodes) {
            if(son->word.word=="<ConstExp>") {
                SymbolConstExp(son,token);
            }else if(son->word.word=="<InitVal>") {
                SymbolInitVal(son,token);
            }
        }
        return;
    }
    //第1位如果是左括号，那么一定是数组，否则一定是变量
    //注意，如果是单个变量，比如int x，那么它只有一个子节点
    if(root->son_num>1 && son_nodes[1]->word.word=="[") {
        int dim = 0;
        nowSymbolTable->add_array_symbol(Ident,btype,dim,is_const);
        //下面分析ConstExp
        SymbolConstExp(son_nodes[2],token);
        //下面有关右中括号，可能本身缺失，同时在变量定义中，等号也有可能不存在。
        //为了方便起见，直接查等于号，等于号只有可能是第3/4位，也有可能不存在
        if(root->son_num>=5) {
            //防止数组越界
            if(son_nodes[4]->word.word=="=") {
                //第4位是等于号，说明第5位必须是InitVal
                SymbolInitVal(son_nodes[5],token);
            }else if(son_nodes[3]->word.word=="=") {
                //第4位是InitVal
                SymbolInitVal(son_nodes[4],token);
            }
        }
    }else {
        //不是数组，是变量
        int var_value = 0;
        nowSymbolTable->add_var_symbol(Ident,btype,var_value,is_const);
        //下一位有可能是等于号
        if(root->son_num>=2) {
            if(son_nodes[1]->word.word=="=") {
                //第2位是InitVal
                SymbolInitVal(son_nodes[2],token);
            }
        }
    }
}

void SymbolHandle::SymbolInitVal(TreeNode *root, const string &token) {
    vector<TreeNode*> son_nodes = root->sonNode;
    if(son_nodes[0]->word.word_type=="STRCON") {
        //说明是StringConst，自然就是字符数组
        string strInit = son_nodes[0]->word.word;
        nowSymbolTable->set_char_array_value(token,strInit);
    }else if(son_nodes[0]->word.word=="{") {
        //说明是第二种形式，'{' [ Exp { ',' Exp } ] '}'
        for(int i=1;i<root->son_num-1;i+=2) {
            //从第1个开始，+=2为了跳过“，”
            if(son_nodes[i]->word.word=="<Exp>") {
                SymbolExp(son_nodes[i],token);
            }
        }
    }else {
        //第一种形式，Exp
        SymbolExp(son_nodes[0],token);
    }
}

void SymbolHandle::SymbolFuncDef(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    int func_type = SymbolFuncType(son_nodes[0]);
    Word Ident = son_nodes[1]->word;
    string token = Ident.word;
    func_token = token;
    if(nowSymbolTable->is_in_table(token)) {
        ErrorPrint::printError(ErrorCategory::redefine,Ident.line_num);
        /*TODO:函数重定义，后面的该怎么分析？
         *TODO:感觉没办法直接分析，毕竟不存在函数Ident这个符号*/
        return;
    }
    bool is_func = true;
    vector<int> params_type;
    //注意，右小括号可能不存在
    //进入循环后，参数属于下一个符号表，所以要建立新表
    createSymbolTable();
    for(int i=3;i<root->son_num;++i) {
        if(son_nodes[i]->word.word=="<FuncFParams>") {
            //必须是FuncFParams才进入
            SymbolFuncFParams(son_nodes[i],params_type);
        }
    }
    int params_num = static_cast<int>(params_type.size());
    //加入函数的声明，函数声明此时在父符号表，主要是为了方便添加“函数是否有返回值”
    nowSymbolTable->father_ptr->add_func_symbol(Ident,func_type,params_num,params_type);
    //由于最后一个节点一定是Block，所以直接这样调用即可
    SymbolBlock(son_nodes[root->son_num-1],is_func);
    //退出Block后，将当前的符号表退回到父符号表
    nowSymbolTable=nowSymbolTable->father_ptr;
    Symbol*symbol = nowSymbolTable->get_symbol(token);
    //TODO：根据文法，对于需要有返回值的函数，应当看最后一句话是不是return；只要有return就行，不需要考虑 return 语句是否有返回值
    TreeNode* Block = son_nodes[root->son_num-1];
    //最后一个是大括号，所以如果有BlockItem，那么一定是倒数第二个
    TreeNode* last_block_item = Block->sonNode[Block->son_num-2];
    if(last_block_item->word.word=="<BlockItem>") {
        //说明这就是最后一个BlockItem
        for(const auto son: last_block_item->sonNode) {
            if(son->word.word=="<Stmt>") {
                //判断语句的第一个单词是不是return
                if(son->sonNode[0]->word.word=="return") {
                    symbol->is_return = true;
                }else symbol->is_return = false;
            }
        }
    }
    //g类错误，如果函数有返回值，那么报错函数结尾的}所在行号
    if(symbol->is_return==false && (symbol->func_type==0 || symbol->func_type==1) ) {
        //注意，最后一个节点是Block，Block的最后一个节点是右大括号
        TreeNode* Block = son_nodes[root->son_num-1];
        ErrorPrint::printError(ErrorCategory::return_lack,
            Block->sonNode[Block->son_num-1]->word.line_num);
    }
    //出去函数了，需要将当前的SymbolHandle的函数名置为空
    func_token = "";
}

void SymbolHandle::SymbolMainFuncDef(TreeNode *root) {
    //主函数，区别在于，main不用进符号表，也没有参数，直接创建符号表进入Block就行
    vector<TreeNode*> son_nodes = root->sonNode;
    bool is_func = true;
    is_main = true;//进入main函数
    func_token="main";
    createSymbolTable();
    //由于最后一个节点一定是Block，所以直接这样调用即可
    SymbolBlock(son_nodes[root->son_num-1],is_func);
    //退出Block后，将当前的符号表退回到父符号表
    nowSymbolTable=nowSymbolTable->father_ptr;
    //TODO：根据文法，对于需要有返回值的函数，应当看最后一句话是不是return；只要有return就行，不需要考虑 return 语句是否有返回值
    TreeNode* Block = son_nodes[root->son_num-1];
    //最后一个是大括号，所以如果有BlockItem，那么一定是倒数第二个
    TreeNode* last_block_item = Block->sonNode[Block->son_num-2];
    if(last_block_item->word.word=="<BlockItem>") {
        //说明这就是最后一个BlockItem
        for(const auto son: last_block_item->sonNode) {
            if(son->word.word=="<Stmt>") {
                if(son->sonNode[0]->word.word=="return") {
                    is_main_return = true;
                }else is_main_return = false;
            }
        }
    }
    //main函数如果没有返回值
    if(!is_main_return) {
        //注意，最后一个节点是Block，Block的最后一个节点是右大括号
        ErrorPrint::printError(ErrorCategory::return_lack,
            Block->sonNode[Block->son_num-1]->word.line_num);
    }
    is_main=false;//退出main函数
    func_token="";//函数名置为空
}

int SymbolHandle::SymbolFuncType(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    if(son_nodes[0]->word.word=="int") {return 0;}
    if(son_nodes[0]->word.word=="char") {return 1;}
    if(son_nodes[0]->word.word=="void") {return 2;}
    printf("Error in SymbolFuncType\n");
    return -1;
}

void SymbolHandle::SymbolFuncFParams(TreeNode *root, vector<int>& params_type) {
    vector<TreeNode*> son_nodes = root->sonNode;
    for(int i=0;i<root->son_num;i+=2) {
        if(son_nodes[i]->word.word=="<FuncFParam>") {
            int func_type = SymbolFuncFParam(son_nodes[i]);
            if(func_type>=0) {
                //不是-1，说明没有符号表重复，可以添加
                params_type.push_back(func_type);
            }
        }
    }
}

int SymbolHandle::SymbolFuncFParam(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    int btype = SymbolBtype(son_nodes[0]);
    Word Ident = son_nodes[1]->word;
    string token = Ident.word;
    if(nowSymbolTable->is_in_table(token)) {
        ErrorPrint::printError(ErrorCategory::redefine,Ident.line_num);
        return -1;
    }
    bool is_array = false;
    //否则，没有冲突，应当看第2个是不是中括号，如果是那么就是数组形参，否则只是一般的变量形参
    if(root->son_num>=3 && son_nodes[2]->word.word=="[") {
        is_array = true;
        nowSymbolTable->add_func_param_symbol(Ident,btype,is_array);
    }else {
        nowSymbolTable->add_func_param_symbol(Ident,btype,is_array);
    }
    if(!is_array) {
        //不是数组，但是在代码生成涉及到强制类型转换的问题，所以还需要记录是什么类型
        return btype;
    }
    //btype 0是int 1是char
    return btype == 0?2:3;
}

void SymbolHandle::SymbolBlock(TreeNode *root, bool is_func) {
    //如果不是函数产生的，那么需要自己建一个符号表
    vector<TreeNode*> son_nodes = root->sonNode;
    if(!is_func) createSymbolTable();
    //注意，Block能推出0个或多个BlockItem,所以要使用循环
    for(const auto& son_node : son_nodes) {
        if(son_node->word.word=="<BlockItem>") {
            SymbolBlockItem(son_node);
        }
    }
    //如果不是函数，那么在这里就要退出符号表，回到父节点
    if(!is_func) nowSymbolTable=nowSymbolTable->father_ptr;
}

void SymbolHandle::SymbolBlockItem(TreeNode *root) {
    //只有可能两种节点：Decl，Stmt
    vector<TreeNode*> son_nodes = root->sonNode;
    for(TreeNode * son_node : son_nodes) {
        if(son_node->word.word=="<Decl>") {
            SymbolDecl(son_node);
        }else if(son_node->word.word=="<Stmt>") {
            SymbolStmt(son_node);
        }
    }
}
//TODO:Stmt之后的内容
void SymbolHandle::SymbolStmt(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    //多个情况，根据子树的第0个节点，确定如何处理。
    if(son_nodes[0]->word.word=="<LVal>") {
        int is_const_l_val = 0;
        //lVal第一个是当前标识符的值，第二个代表如果是数组，那么数组的标号
        pair<int,int> lVal = SymbolLVal(son_nodes[0], &is_const_l_val);
        //LVal分析的标识符，如果没有重定义，则需要考虑是不是h错误：给常量赋值
        if(is_const_l_val == 1) {
            //son_nodes[0]是LVal；LVal的第0个子节点是Ident
            ErrorPrint::printError(ErrorCategory::const_error,
                son_nodes[0]->sonNode[0]->word.line_num);
        }
        if(son_nodes[2]->word.word=="<Exp>") {
            //获取LVal的标识符token
            string token = son_nodes[0]->sonNode[0]->word.word;
            int exp = 0;
            if(son_nodes[0]->sonNode[0]->word.word_type=="IDENFR" && is_const_l_val==0) {
                //注意，这里不再进入Exp处理赋值，直接在外面LVal解决
                exp = SymbolExp(son_nodes[2],"");
            }else {
                //否则，本身就是未定义的或者是常量，不应当处理赋值
                SymbolExp(son_nodes[2],"");
            }
        }
    }
    else if(son_nodes[0]->word.word=="<Exp>") {
        SymbolExp(son_nodes[0],"");
    }
    else if(son_nodes[0]->word.word=="<Block>") {
        SymbolBlock(son_nodes[0],false);
    }
    else if(son_nodes[0]->word.word=="if") {
        //第二个节点是Cond
        SymbolCond(son_nodes[2]);
        //第3个可能是右小括号，也可能因为缺失右小括号，就是Stmt
        //直接用循环判断更好
        int num = 3;
        while(num<root->son_num) {
            if(son_nodes[num]->word.word=="<Cond>") {
                SymbolCond(son_nodes[num]);
            }else if(son_nodes[num]->word.word=="<Stmt>") {
                SymbolStmt(son_nodes[num]);
            }
            num++;
        }
    }
    else if(son_nodes[0]->word.word=="for") {
        //这里面需要注意，最少有6个节点，但是对于最多的情况，ForStmt是第7位，所以需要处理
        int num = 2;
        is_in_for++;
        while(num<root->son_num) {
            if(son_nodes[num]->word.word=="<ForStmt>") {
                SymbolForStmt(son_nodes[num]);
            }else if(son_nodes[num]->word.word=="<Cond>") {
                SymbolCond(son_nodes[num]);
            }else if(son_nodes[num]->word.word=="<Stmt>") {
                SymbolStmt(son_nodes[num]);
            }
            num++;
        }
        is_in_for--;
    }
    else if(son_nodes[0]->word.word=="break" || son_nodes[0]->word.word=="continue") {
        if(is_in_for<=0) {
            //不在循环里面
            ErrorPrint::printError(ErrorCategory::break_continue_not_loop,
                son_nodes[0]->word.line_num);
        }
    }
    else if(son_nodes[0]->word.word=="return") {
        //TODO:对于有返回值的函数，只需要考虑函数末尾有没有return
        //也就是说，在return所在的Stmt结点，它没有右侧的兄弟节点
        //注意，应当是沿着函数的Block的最右侧路线走，最后一个Stmt是return
        TreeNode* father = root->father;
        //注意：函数的声明不是“父符号表最后一个符号”，因为像if, for这些可以产生更多的子符号表
        Symbol* func_symbol = nowSymbolTable->father_ptr->get_symbol_in_all_table(func_token);
        //TODO：注：由于文法要求，必须看最后一句是不是return，所以为了判断方便，我调整在FuncDef部分处理
        if(is_main) {
            is_main_return = true;
        }
        //否则，说明是一般的函数，父符号表中最后一个符号就是函数的符号
        else {
            if(func_symbol->is_func) {
                if(func_symbol->func_type!=2) {
                    //不是void函数
                    nowSymbolTable->father_ptr->set_is_return(func_symbol,true);
                }
                else {
                    //是void函数，
                    //如果出现了return，且后面一个节点是Exp报错
                    //报错行号是return的行号
                    //如果只有一个son_node,说明return没有分号，这个不是void的错误，是缺少分号，所以要保证子节点个数大于1
                    if(root->son_num > 1 && son_nodes[1]->word.word=="<Exp>") {
                        ErrorPrint::printError(ErrorCategory::return_not_match,
                        son_nodes[0]->word.line_num);
                    }
                }
            }
        }
        //分析Exp的值
        for(int i=0;i<root->son_num;i++) {
            if(son_nodes[i]->word.word=="<Exp>") {
                int exp = SymbolExp(son_nodes[i],"");
                if(func_symbol!=nullptr && func_symbol->is_func) {
                    nowSymbolTable->father_ptr->set_func_return(func_symbol,exp);
                }
            }
        }
        /*if(father) {
            if(father->sonNode[father->son_num-1]==root) {
                //如果父亲的最后一个节点，就是root，那么说明root没有兄弟节点
                //这就说明，root的return是最后一句话
                //如果是main函数，记录有返回值
                if(is_main) is_main_return = true;
                else {
                    if(func_symbol->is_func) {
                        if(func_symbol->func_type!=2) {
                            //不是void函数
                            nowSymbolTable->father_ptr->set_is_return(func_symbol,true);
                        }
                        else {
                            //是void函数，出现了return，报错
                            //报错行号是return的行号
                            ErrorPrint::printError(ErrorCategory::return_not_match,
                                son_nodes[0]->word.line_num);
                        }
                    }
                    //分析Exp的值
                    for(int i=0;i<root->son_num;i++) {
                        if(son_nodes[i]->word.word=="<Exp>") {
                            int exp = SymbolExp(son_nodes[i],"");
                            nowSymbolTable->father_ptr->set_func_return(func_symbol,exp);
                        }
                    }
                }
            }
        }*/
    }
    else if(son_nodes[0]->word.word=="printf") {
        //如果是printf，进行最后一句话
        //这个是要处理，StringConst和Exp个数的匹配问题
        //其实，由于Exp的结点在语法分析已经建立完成，所以直接查Exp节点个数即可
        //第二个子节点一定是StringConst
        string StringConst = son_nodes[2]->word.word;
        //查询字符串有几个%，就代表后面要有几个Exp
        //TODO:注意，格式字符串只包含%d和%c，其余不统计
        int percent_num = 0, exp_num = 0;
        int len = StringConst.length();
        for(int i=0;i<len-1;i++) {
            if(StringConst[i]=='%' && (StringConst[i+1]=='d'||StringConst[i+1]=='c')) {
                percent_num++;
            }
        }
        //接下来分析后面的Exp，同时统计Exp的个数
        for(TreeNode* son_node:son_nodes) {
            if(son_node->word.word=="<Exp>") {
                exp_num++;
                SymbolExp(son_node,"");
            }
        }
        if(exp_num!=percent_num) {
            //两个不相等，报错，参数个数不匹配
            ErrorPrint::printError(ErrorCategory::format_num_not_match,
                son_nodes[0]->word.line_num);
        }
    }
    else if(son_nodes[0]->word.word!=";"){
        //可能是语法树建立出现了错误
        printf("Error in SymbolHandle::SymbolStmt or Paser::Stmt\n");
    }
}

void SymbolHandle::SymbolForStmt(TreeNode *root) {
    //三个节点
    vector<TreeNode*> son_nodes = root->sonNode;
    int is_const_l_val = 0;
    pair<int,int> lVal = SymbolLVal(son_nodes[0], &is_const_l_val);
    //LVal分析的标识符，如果没有重定义，则需要考虑是不是h错误：给常量赋值
    if(is_const_l_val == 1) {
        //son_nodes[0]是LVal；LVal的第0个子节点是Ident
        ErrorPrint::printError(ErrorCategory::const_error,
            son_nodes[0]->sonNode[0]->word.line_num);
    }
    if(son_nodes[2]->word.word=="<Exp>") {
        //获取LVal的标识符token
        string token = son_nodes[0]->sonNode[0]->word.word;
        int exp = 0;
        if(son_nodes[0]->sonNode[0]->word.word_type=="IDENFR" && is_const_l_val==0) {
            exp = SymbolExp(son_nodes[2],token);
        }else {
            //否则，本身就是非法的，不应当处理赋值
            SymbolExp(son_nodes[2],"");
        }
    }
}

int SymbolHandle::SymbolExp(TreeNode *root, const string &token) {
    //Exp → AddExp
    return SymbolAddExp(root->sonNode[0],token);
}

void SymbolHandle::SymbolCond(TreeNode *root) {
    SymbolLOrExp(root->sonNode[0]);
}

pair<int,int> SymbolHandle::SymbolLVal(TreeNode *root, int* type) {
    //分析LVal
    vector<TreeNode*> son_nodes = root->sonNode;
    Word Ident = son_nodes[0]->word;
    string token = Ident.word;
    //bool is_array = false;//判断这个LVal是不是数组
    //接下来要判断，这个Ident是否定义过
    if(!nowSymbolTable->is_in_all_table(token)) {
        *type = 2;
        //如果不在当前符号表和所有的父符号表，那么报错
        ErrorPrint::printError(ErrorCategory::undefined,Ident.line_num);
        token = "";
        //不论LVal是否有误，都分析Exp
        for(const auto &son_node:son_nodes) {
            if(son_node->word.word=="<Exp>") {
                SymbolExp(son_node,token);
            }
        }
        return {0,-1};//因为未定义，所以返回0
    }
    Symbol* symbol = nowSymbolTable->get_symbol_in_all_table(token);
    if(symbol!=nullptr) {
        if(symbol->is_const) {
            *type = 1;
        }else *type = 0;
    }
    //不论LVal是否有误，都分析Exp
    int exp = -1;
    for(const auto &son_node:son_nodes) {
        if(son_node->word.word=="<Exp>") {
            exp = SymbolExp(son_node,token);
        }
    }
    //TODO:其实应当返回Ident符号对应的值，这里之后需要修改

    if(symbol!=nullptr) {
        if(symbol->is_array) {
            //如果是数组
            if(symbol->btype==0) {
                //说明是int数组
                //注意，在函数中，由于数组的定义可以是形参，元素个数默认为0
                if(symbol->array_values.empty() || exp >= symbol->array_values.size()) {return {-1,exp};}
                return {symbol->array_values[exp],exp};
            }
            if(symbol->btype==1) {
                if(symbol->array_values.empty() || exp >= symbol->array_values.size()) {return {-1,exp};}
                return {symbol->array_values[exp],exp};
            }
        }
        else if(symbol->is_var) {
            //说明是变量
            return {symbol->var_value,exp};
        }
    }
    printf("Error in SymbolHandle::SymbolLVal\n");
    return {-1,-1};
}

int SymbolHandle::SymbolPrimaryExp(TreeNode *root, const string& token) {
    vector<TreeNode*> son_nodes = root->sonNode;
    //直接用for循环遍历子节点，然后分类讨论去判断
    for(const auto &son_node:son_nodes) {
        if(son_node->word.word=="<Exp>") {
            return SymbolExp(son_node,token);
        }else if(son_node->word.word=="<LVal>") {
            //TODO:如何把LVal的值赋给token对应的符号
            int is_const_l_val = -1;
            pair<int,int> lVal = SymbolLVal(son_node,&is_const_l_val);
            return lVal.first;
        }else if(son_node->word.word=="<Number>") {
            int number = SymbolNumber(son_node);
            //TODO:处理如何赋值，因为实际上不是直接赋值，而是要考虑运算的结果
            //TODO:以及，对于数组元素的个数，也需要考虑
            nowSymbolTable->set_var_value(token,number);
            return number;
        }else if(son_node->word.word=="<Character>") {
            char character = SymbolCharacter(son_node);
            //TODO:处理如何赋值，因为实际上不是直接赋值，而是要考虑运算的结果
            nowSymbolTable->set_var_value(token,character);
            return character;
        }
    }
    printf("Error in SymbolHandle::SymbolPrimaryExp\n");
    return -1;//出现了错误
}

int SymbolHandle::SymbolNumber(TreeNode *root) {
    if(root->word.word=="<Number>") {return stoi(root->sonNode[0]->word.word);}
    return root->sonNode[0]->word.word[1];
}

char SymbolHandle::SymbolCharacter(TreeNode *root) {
    return root->sonNode[0]->word.word[1];
}

int SymbolHandle::SymbolUnaryExp(TreeNode *root, const string &token) {
    vector<TreeNode*> son_nodes = root->sonNode;
    char op = '+';
    for(int i=0;i<root->son_num;++i) {
        const auto& son_node = son_nodes[i];
        if(son_node->word.word=="<PrimaryExp>") {
            return SymbolPrimaryExp(son_node,token);
        }else if(son_node->word.word=="<UnaryOp>") {
            op = SymbolUnaryOp(son_node);
        }else if(son_node->word.word=="<UnaryExp>") {
            if(op=='+') return SymbolUnaryExp(son_node,token);
            if(op=='-') return - SymbolUnaryExp(son_node,token);
            if(op=='!') return ! SymbolUnaryExp(son_node,token);
        }else if(son_node->word.word_type=="IDENFR") {
            //这里进入函数调用的环节，要考虑函数参数不匹配的问题
            /*
             *对于调用函数时，参数类型不匹配一共有以下几种情况的不匹配：
             *传递数组给变量。
             *传递变量给数组。
             *传递 char 型数组给 int 型数组。
             *传递 int 型数组给 char 型数组
             */
            Word Ident = son_node->word;
            string func_token = Ident.word;
            if(!nowSymbolTable->is_in_all_table(func_token)) {
                //如果都不再，直接报错，返回-1简单处理
                //TODO:复杂处理，不一定用得上，先不做了
                ErrorPrint::printError(ErrorCategory::undefined,Ident.line_num);
                return -1;//由于文法保证不会在同一行出现多个错误，所以不用再考虑
            }
            //TODO:函数调用后返回值的确认
            //在之前的符号分析中，函数是形参，没有值（默认为0），所以运算的结果肯定有问题
            Symbol* func_symbol = nowSymbolTable->get_symbol_in_all_table(func_token);
            int param_num = func_symbol->param_num;
            auto param_type = func_symbol->param_type;
            for(int j=2;j<root->son_num;++j) {
                if(son_nodes[j]->word.word=="<FuncRParams>") {
                    //每次分析FuncRParams，返回它的类型(数组还是变量)
                    //判断个数是否满足，以及类型是否匹配
                    vector<int> r_param_type = SymbolFuncRParams(son_nodes[j],func_token);
                    //如果参数个数不对
                    if(r_param_type.size()!=param_num) {
                        ErrorPrint::printError(ErrorCategory::param_num_not_match,
                            Ident.line_num);
                    }else if(r_param_type!=param_type) {
                        //对于相同位置，如果分别是0，1，那么属于强制类型转换，不报错
                        //为了方便判断，将0和1全部置为0
                        //如果内容不对，即参数类型不匹配
                        for(int k = 0;k<param_num;++k) {
                            if(r_param_type[k] == 1) r_param_type[k] = 0;
                            if(param_type[k] == 1) param_type[k] = 0;
                        }
                        if(r_param_type!=param_type) {
                            ErrorPrint::printError(ErrorCategory::param_type_not_match,
                                Ident.line_num);
                        }
                    }else {
                        //TODO:没有问题，应当正常返回函数值，以后再处理
                        return func_symbol->func_value;
                    }
                }
            }
            break;
        }
    }
    return -1;
}

char SymbolHandle::SymbolUnaryOp(TreeNode *root) {
    return root->sonNode[0]->word.word[0];
}

vector<int> SymbolHandle::SymbolFuncRParams(TreeNode *root, const string &func_token) {
    vector<int> r_params;
    for(const auto& son_node:root->sonNode) {
        //son node一定是Exp
        if(son_node->word.word==",") continue;
        SymbolExp(son_node,"");
        vector<TreeNode*> leaf_nodes;
        ParserTree::getLeafNodes(son_node,leaf_nodes);
        //根据Exp的叶子节点，由于这里Exp是实参，
        //那么如果Exp的叶子节点有左中括号，说明一定是数据
        //同时，找第一个Ident，如果没有左中括号且Ident是数组，那么说明传参确实是数组
        int flag_ident = 0;//标记是否找到了第一个Ident
        int left_bracket = 0;//标记有没有左括号
        string token;
        Symbol* symbol = nullptr;
        int is_con_var = -1;
        for(const auto& leaf_node:leaf_nodes) {
            if(leaf_node->word.word=="[" || leaf_node->word.word_type=="STRCON") {
                //一定是数据，传的是数组元素，普通的变量
                left_bracket = 1;
                break;
            }
            if(leaf_node->word.word_type=="IDENFR" && flag_ident == 0) {
                flag_ident = 1;
                token = leaf_node->word.word;
                //如果能找到token，说明它有定义，自然有符号
                if(nowSymbolTable->is_in_all_table(token)) {
                    symbol = nowSymbolTable->get_symbol_in_all_table(token);
                }
            }
            else if(leaf_node->word.word_type=="INTCON" ) {
                //说明这是int变量，不是数组
                is_con_var = 0;
            }else if(leaf_node->word.word_type=="CHRCON") {
                //说明这是char变量，不是数组
                is_con_var = 1;
            }
        }
        if (left_bracket) {
            //有左括号，一定是数据，传的是数组元素，普通的变量
            r_params.push_back(0);
        }else {
            //否则，根据symbol的情况采取措施
            //TODO:注意，这里少考虑一种情况，如果实参有符号Exp未定义，那么添加的vector肯定会变少，很可能在一行出现两个错误
            if(is_con_var==0 || is_con_var==1) {
                //这个符号是参数var，不是数组
                r_params.push_back(is_con_var);
            }
            else if(symbol != nullptr) {
                int btype = symbol->btype;
                int type = symbol->type;
                if(type == 0 || symbol->is_var) {
                    //这个符号是参数var，不是数组
                    r_params.push_back(symbol->btype);
                }else if(type == 1) {
                    //2是int数组,3是char数组
                    r_params.push_back(type + btype + 1);
                }else if(type == 2) {
                    //2是函数，函数返回的是int，char,void
                    if(symbol->func_type == 0) {
                        //是int的变量
                        r_params.push_back(0);
                    }else if(symbol->func_type == 1) {
                        //是char的变量
                        r_params.push_back(1);
                    }else {
                        //确实调用了，但是void类型函数，和参数类型不匹配
                        r_params.push_back(-1);
                    }
                }
            }
        }
    }
    return r_params;
}

int SymbolHandle::SymbolMulExp(TreeNode *root, const string &token) {
    vector<TreeNode*> son_nodes = root->sonNode;
    char op = '*';
    int exp1 = 0;
    int exp2 = 0;
    //MulExp节点数一定是1或3
    if(root->son_num==1) {
        //只有UnaryExp
        return SymbolUnaryExp(son_nodes[0],token);
    }
    if(root->son_num==3) {
        exp1 = SymbolMulExp(son_nodes[0],token);
        op = son_nodes[1]->word.word[0];
        exp2 = SymbolUnaryExp(son_nodes[2],token);
        if(op=='*') return exp1 + exp2;
        if(op=='/') {
            //TODO:这里赋值先这样处理，如果分母为0返回-1；之后需要更加精细的处理
            if(exp2 == 0) return -1;
            return exp1 / exp2;
        }
        if(op=='%') {
            if(exp2 == 0) return -1;
            return exp1 % exp2;
        }
    }
    printf("Error in SymbolHandle::SymbolMulExp\n");
    return -1;
}

int SymbolHandle::SymbolAddExp(TreeNode *root, const string &token) {
    vector<TreeNode*> son_nodes = root->sonNode;
    char op = '+';
    int exp1 = 0;
    int exp2 = 0;
    //MulExp节点数一定是1或3
    if(root->son_num==1) {
        //只有MulExp
        return SymbolMulExp(son_nodes[0],token);
    }
    if(root->son_num==3) {
        exp1 = SymbolAddExp(son_nodes[0],token);
        op = son_nodes[1]->word.word[0];
        exp2 = SymbolMulExp(son_nodes[2],token);
        if(op=='+') return exp1 + exp2;
        if(op=='-') return exp1 - exp2;
    }
    printf("Error in SymbolHandle::SymbolAddExp\n");
    return -1;
}

int SymbolHandle::SymbolRelExp(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    int exp1 = 0;
    int exp2 = 0;
    string op = "<";
    if(root->son_num==1) {
        return SymbolAddExp(son_nodes[0],"");
    }
    if(root->son_num==3) {
        exp1 = SymbolRelExp(son_nodes[0]);
        op = son_nodes[1]->word.word;
        exp2 = SymbolAddExp(son_nodes[2],"");
        if(op=="<") return exp1 < exp2;
        if(op==">") return exp1 > exp2;
        if(op=="<=") return exp1 <= exp2;
        if(op==">=") return exp1 >= exp2;
    }
    printf("Error in SymbolHandle::SymbolRelExp\n");
    return -1;
}

int SymbolHandle::SymbolEqExp(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    int exp1 = 0;
    int exp2 = 0;
    string op = "==";
    if(root->son_num==1) {
        return SymbolRelExp(son_nodes[0]);
    }
    if(root->son_num==3) {
        exp1 = SymbolEqExp(son_nodes[0]);
        op = son_nodes[1]->word.word;
        exp2 = SymbolRelExp(son_nodes[2]);
        if(op=="==") return exp1 == exp2;
        if(op=="!=") return exp1 != exp2;
    }
    printf("Error in SymbolHandle::SymbolEqExp\n");
    return -1;
}

int SymbolHandle::SymbolLAndExp(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    int exp1 = 0;
    int exp2 = 0;
    if(root->son_num==1) {
        return SymbolEqExp(son_nodes[0]);
    }
    if(root->son_num==3) {
        exp1 = SymbolLAndExp(son_nodes[0]);
        exp2 = SymbolEqExp(son_nodes[2]);
        return exp1 && exp2;
    }
    printf("Error in SymbolHandle::SymbolLAndExp\n");
    return -1;
}

int SymbolHandle::SymbolLOrExp(TreeNode *root) {
    vector<TreeNode*> son_nodes = root->sonNode;
    int exp1 = 0;
    int exp2 = 0;
    if(root->son_num==1) {
        return SymbolLAndExp(son_nodes[0]);
    }
    if(root->son_num==3) {
        exp1 = SymbolLOrExp(son_nodes[0]);
        exp2 = SymbolLAndExp(son_nodes[2]);
        return exp1 || exp2;
    }
    printf("Error in SymbolHandle::SymbolLOrExp\n");
    return -1;
}

int SymbolHandle::SymbolConstExp(TreeNode *root, const string &token) {
    return SymbolAddExp(root->sonNode[0],token);
}
