//
// Created by PengXinyang on 24-9-29.
//

#include "../include/ParserTree.h"
#include<cstdio>
TreeNode *ParserTree::catchTree(TreeNode* root, const Word &word) {
    auto* son = new TreeNode(word);
    son->father = root;
    root->sonNode.push_back(son);
    root->son_num++;
    if(root->type=="char" && son->type=="int") {
        //语法树上涉及强制类型转换
        root->type = "int";
        return root;
    }
    if(!root->type.empty()) {
        //根节点本身有类型，并且已经不会强制类型转换了，直接跳出
        return root;
    }
    if(!son->type.empty()){
        root->type = son->type;
    }
    return root;
}

TreeNode *ParserTree::createTree(const Word &word) {
    auto* root = new TreeNode(word);
    return root;
}

TreeNode *ParserTree::catchTree(TreeNode *root, TreeNode *son) {
    son->father = root;
    root->sonNode.push_back(son);
    root->son_num++;
    if(root->type=="char" && son->type=="int") {
        //语法树上涉及强制类型转换
        root->type = "int";
        return root;
    }
    if(!root->type.empty()) {
        //根节点本身有类型，并且已经不会强制类型转换了，直接跳出
        return root;
    }
    if(!son->type.empty()){
        root->type = son->type;
    }
    return root;
}
void ParserTree::printTree(FILE* fp, const TreeNode *root) {
    if (root != nullptr) {
        if(root->son_num==0) {
            //没有孩子，是子节点，要输出类型和名字
            fprintf(fp,"%s %s\n",root->word.word_type.c_str(),root->word.word.c_str());
        }
        else {
            for (int i = 0; i < root->son_num; i++) {
                printTree(fp,root->sonNode[i]);
            }
            if(root->word.word!="<BlockItem>"&&root->word.word!="<Decl>"&&root->word.word!="<Btype>") {
                fprintf(fp,"%s\n", root->word.word.c_str());
            }
        }
    }
}

TreeNode* ParserTree::adjustTree(TreeNode *root, const Word &word) {
    /*语法树的调整
     * 本来，对于左递归文法，是直接转成循环处理
     * 但是循环处理后，构建语法树会发现他们都作为同一类在同一层
     * 比如AddExp = AddExp + MulExp
     * 循环后，树AddExp下面一层有多个MulExp，每两个之间还有运算符
     * 而根据题目，题目希望建的树就是AddExp + MulExp这三部分
     * 所以要调整树，从右往左
     */
    TreeNode *parent = createTree(root->word);
    TreeNode *p = parent;
    for(int i=root->son_num-1; i>=0; i-=2) {
        if(i!=0) {
            auto* sonWord = new TreeNode(word);
            p->sonNode.push_back(sonWord);
            sonWord->father = p;
            p->sonNode.push_back(root->sonNode[i-1]);
            root->sonNode[i-1]->father = p;
            p->son_num+=2;
        }
        p->sonNode.push_back(root->sonNode[i]);
        root->sonNode[i]->father = p;
        p->son_num++;
        p=p->sonNode[0];
    }
    delete root;
    return parent;
}

// 递归函数，查找子树中的所有叶子节点
void ParserTree::getLeafNodes(TreeNode *root, vector<TreeNode *> &leafNodes) {
    // 如果当前节点为空，直接返回
    if (root == nullptr) return;

    // 如果当前节点是叶子节点，加入结果
    if (root->son_num == 0) {
        leafNodes.push_back(root);
    } else {
        // 否则递归查找子节点
        for (TreeNode* child : root->sonNode) {
            getLeafNodes(child, leafNodes);
        }
    }
}

