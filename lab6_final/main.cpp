#include "main.tab.hh"
#include "common.h"
#include <iostream>
#include <fstream>
using namespace std;
TreeNode *root = nullptr;
symbol_table SymbolsTable;
int main ()
{
    yyparse();
    if(root){//若存在语法树结点
        //root->genNodeId();//将整棵语法树赋予id
        root->printAST();//打印相关信息
        
        cerr<<endl;

        tree _tree(root);
        _tree.get_label();
        _tree.gen_code();
    }
}
int yyerror(char const* message)
{
    cout << message << endl;
    return -1;
}