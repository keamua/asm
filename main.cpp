#include"main.tab.hh"
#include"common.h"
#include<iostream>
using std::cout;
using std::endl;
TreeNode *root=nullptr;
int main (int argc,char **argv)
{
    yyparse();
    if(root){//若存在语法树结点
        root->genNodeId(root);//将整棵语法树赋予id
        root->printAST(root);//打印相关信息
    }
    return 0;
}
int yyerror(char const* message)
{
  cout << message << endl;
  return -1;
}
