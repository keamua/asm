#ifndef TREE_H
#define TREE_H
#define MAXCHILDREN 4
#include "symbol.h"
#include <string>
#include <iostream>
using namespace std;

extern symbol_table SymbolsTable;

enum NodeType{
    STMT_NODE,
    DECL_NODE,
    EXPR_NODE,
    NO_NODE
};


enum StmtType {
    COMP_STMT,
    IF_STMT,
    WHILE_STMT,
    FOR_STMT,
    ASSIGN_STMT,       
    PRINTF_STMT,
    SCANF_STMT,
    RETURN_STMT,
    INOUT_STMT, 
    NO_STMT
};

enum DeclType{
    VAR_DECL,
    FUNC_DECL,
    NO_DECL
};

enum ExprType{
    OP_EXPR,
    ID_EXPR,
    TYPE_EXPR,
    PARAM_EXPR,
    CONST_EXPR,
    NULL_EXPR,
    NO_EXPR,
    VAR_EXPR
};

enum OpType {
    EQUAL_OP,
    GT_OP,
    GE_OP,
    LT_OP,
    LE_OP,
    NE_OP,

    AND_OP,
    OR_OP,
    NOT_OP,

    ADD_OP,
    SUB_OP,
    MUL_OP,
    DIV_OP,
    MOD_OP,

    POS_OP,
    NEG_OP,    
    SADD_OP,
    SSUB_OP,

    ASSIGN_OP,
    ADDE_OP,
    SUBE_OP,
    MULE_OP,

    QUOTE_OP,
    NO_OP
};

enum Type {
    INTEGERt,
    BOOLEAN,
    CHARACTOR,
    STRINGt,
    CONSTt,
    VAR,
    VOIDt,    
    NOTYPE
};

struct Label {
    string true_label;
    string false_label;
    string begin_label;
    string next_label;
};

struct TreeNode {
    int nodeID;    
    int tempID;
    int strID;      
    int line;
    string info; 
    Label label;

    NodeType nodeType; 
    StmtType stmtType; 
    ExprType exprType;
    DeclType declType;

    OpType   opType;
    int     int_val;   
    bool   bool_val; 
    string  str_val;
    string char_val;
    string var_name;

    Type type;

    TreeNode *children[MAXCHILDREN];
    TreeNode *sibling;

    void addChild(TreeNode *);
    void addSibling(TreeNode *);

    void genNodeId(); 

    void printAST(); 
    void printNodeInfo();
    void printNodeConnection();
    string nodeTypeInfo();

    

    TreeNode(NodeType type);

    string nttoa(NodeType type);
    string sttoa(StmtType type);
    string eptoa(ExprType type);
    string dltoa(DeclType type);
    string optoa(OpType type);
    string tytoa(Type type);

    void type_check();
};

class tree {
  public:
    TreeNode *root;
    int node_seq = 0;
    int temp_var_seq = 0;
    int label_seq = 0;
    int const_str_seq = 0;

    void get_temp_var(TreeNode *t);
    void get_const_str(TreeNode *t);
    string new_label(void);
    void recursive_get_label(TreeNode *t);
    void stmt_get_label(TreeNode *t);
    void expr_get_label(TreeNode *t);

    void gen_header();
    void gen_decl(TreeNode *t);
    void recursive_gen_code(TreeNode *t);
    void stmt_gen_code(TreeNode *t);
    void expr_gen_code(TreeNode *t);

    void get_label(void);
    void gen_code();
    tree(TreeNode *node) {
        root = node;
        temp_var_seq = root->tempID;
        const_str_seq = root->strID;
    }
};
#endif