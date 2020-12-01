#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
using namespace std;

using std::cerr;
using std::cout;
using std::endl;
using std::string;

#include "tree.h"

void TreeNode:: addChild(TreeNode* node)
{
	if(!this->child)
		this->child=node;
	else
	{
		TreeNode *p=child;
		while(p->sibling)
		{
			p=p->sibling;
		}
		p->sibling=node;
	}
};
void TreeNode:: addSibling(TreeNode* node)
{
	TreeNode *p=this->sibling;
	while(p->sibling)
		p=p->sibling;
	p->sibling=node; 
};
void TreeNode:: genNodeId(TreeNode *node)
{
	if (node) {
		node->nodeID++;//调用操作结点数据的函数方法
        genNodeId(node->child);//访问该结点的左孩子
        genNodeId(node->sibling);//访问该结点的右孩子
    }
    //如果结点为空，返回上一层
};//从根节点开始逐个赋Id 实现方式同学们可以自行修改

void TreeNode:: printAST(TreeNode *node)
{
	cout<<111<<endl;
	if (node) {
		printNodeInfo();//调用操作结点数据的函数方法
        printAST(node->child);//访问该结点的左孩子
        printAST(node->sibling);//访问该结点的右孩子
    }

}
;//打印语法树结点
/***
* 以下的几个函数皆为在printAST过程中辅助输出使用
* 同学们可以根据需要自己使用其他方法
***/
void TreeNode:: printNodeInfo()
{
	string info=this->nodeTypeInfo();
	cout<<"ID:"<<this->nodeID<<"TYPE:"<<info<<endl;

}
;
void TreeNode:: printNodeConnection()
{
	if(this->child&&this->sibling)
		cout<<"child:"<<this->child->nodeID<<"sibling:"<<this->sibling->nodeID<<endl;
	else if(this->child&&!this->sibling)
		cout<<"child:"<<this->child->nodeID<<endl;
	else if(!this->child&&this->sibling)
		cout<<"sibling:"<<this->sibling->nodeID<<endl;
	else if(!this->child&&!this->sibling)
		cout<<"no connection below this node!"<<endl;
}
;
string TreeNode:: nodeTypeInfo()
{
	string typeinfo;
	switch(this->nodeType)
	{
		case '0' :typeinfo="NODE_CONST";break;
		case '1' :typeinfo="NODE_BOOL";break;
		case '2' :typeinfo="NODE_VAR";break;
		case '3' :typeinfo="NODE_EXPR";break;
		case '4' :typeinfo="NODE_TYPE";break;
		case '5' :typeinfo="NODE_STMT";break;
		case '6' :typeinfo="NODE_PROG";break;
		case '7' :typeinfo="NODE_OP";break;		
		default :cout << "error node_type!" << endl;
   }
	if(this->nodeType=NODE_STMT)
		switch(this->stmtType)
	{
		case '0' :typeinfo="STMT_IF";break;
		case '1' :typeinfo="STMT_WHILE";break;
		case '2' :typeinfo="STMT_DECL";break;
		case '3' :typeinfo="STMT_ASSIGN";break;
		case '4' :typeinfo="STMT_PRINTF";break;
		case '5' :typeinfo="STMT_SCANF";break;		
		default :cout << "error stmt_type!" << endl;
   }
	if(this->nodeType=NODE_OP)
		switch(this->opType)
	{
		case '0' :typeinfo="OP_EQUAL";break;
		case '1' :typeinfo="OP_NOT";break;
		case '2' :typeinfo="OP_ADD";break;
		case '3' :typeinfo="OP_SUB";break;
		case '4' :typeinfo="OP_MUL";break;
		case '5' :typeinfo="OP_DIV";break;		
		default :cout << "error op_type!" << endl;
   }
	if(this->nodeType=NODE_VAR)
		switch(this->varType)
	{
		case '0' :typeinfo="VAR_INTEGER";break;
		case '1' :typeinfo="VAR_VOID";break;	
		default :cout << "error var_type!" << endl;
   }
	return typeinfo;
	
}
;
TreeNode::TreeNode(NodeType type)
{
	this->child=nullptr;
	this->sibling=nullptr;
	nodeType=type;
};

int main()
{
	TreeNode *T;
	T=(TreeNode*)malloc(sizeof(TreeNode));
    T->nodeID=1;
    T->child=(TreeNode*)malloc(sizeof(TreeNode));
    T->sibling=(TreeNode*)malloc(sizeof(TreeNode));
  
    T->child->nodeID=2;
    T->child->child=(TreeNode*)malloc(sizeof(TreeNode));
    T->child->sibling=(TreeNode*)malloc(sizeof(TreeNode));
    T->child->sibling->nodeID=5;
    T->child->sibling->child=NULL;
    T->child->sibling->sibling=NULL;
    T->sibling->nodeID=3;
    T->sibling->child=(TreeNode*)malloc(sizeof(TreeNode));
    T->sibling->child->nodeID=6;
    T->sibling->child->child=NULL;
    T->sibling->child->sibling=NULL;
    T->sibling->sibling=(TreeNode*)malloc(sizeof(TreeNode));
    T->sibling->sibling->nodeID=7;
    T->sibling->sibling->child=NULL;
    T->sibling->sibling->sibling=NULL;
    T->child->child->nodeID=4;
    T->child->child->child=NULL;
    T->child->child->sibling=NULL;
	T->genNodeId(T);
	T->printAST(T);
	return 0;

}
