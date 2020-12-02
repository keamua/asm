#ifndef TREE_H
#define TREE_H
#include<iostream>
#include<string>
#include<queue>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

enum NodeType{
    NODE_CONST,
    NODE_BOOL,
    NODE_VAR,
    NODE_EXPR,
    NODE_TYPE,
    NODE_STMT,
    NODE_PROG,
    NODE_OP,
    NODE_FUNC,   //函数
    NODE_COMM,   //注释
    NODE_STRING  //字符串
};

enum StmtType{
    STMT_IF,
    STMT_WHILE,
    STMT_DECL,
    STMT_ASSIGN,
    STMT_PRINTF,
    STMT_SCANF,
    STMT_FUNC,     //函数
    STMT_FOR
};

enum OpType{
    OP_EQUAL,
    OP_NOT,
    OP_ADD,
    OP_MOD,   //模运算
    OP_GT,   //大于
    OP_LE,   //小于等于
    OP_LT,     //小于
    OP_OR,    //或
    OP_AND    //与
};

enum VarType{
    VAR_INTEGER,
    VAR_VOID,
    VAR_CHAR      //新增类型char。
};

struct TreeNode {
    int nodeID;
    NodeType nodeType;
    TreeNode* child  ;
    TreeNode* sibling ;

    void addChild(TreeNode *);
    void addSibling(TreeNode *);

    void genNodeId(TreeNode *);//从根节点开始逐个赋Id 实现方式同学们可以自行修改

    void printAST(TreeNode *);//打印语法树结点
    /***
     * 以下的几个函数皆为在printAST过程中辅助输出使用
     * 同学们可以根据需要自己使用其他方法
    ***/
    void printNodeInfo(TreeNode *);
    void printNodeConnection(TreeNode *);
    string nodeTypeInfo(TreeNode *);

    string int_val;
    bool bool_val;
    StmtType stmtType;
    OpType opType;
    VarType varType;
    string var_name;

    string comm; //注释

    TreeNode(NodeType type);
};
#endif
