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

void TreeNode::addChild(TreeNode* node)
{
	if(this->child==nullptr)
	{
		this->child=node;
	}
	else
	{
		TreeNode *p=child;
		while(p->sibling!=nullptr)
		{
			p=p->sibling;
		}
		p->sibling=node;
	}
};
void TreeNode:: addSibling(TreeNode* node)
{
	TreeNode *p=this;
	while(p->sibling)
		p=p->sibling;
	p->sibling=node; 
};
int num=0;
void TreeNode:: genNodeId(TreeNode *node)
{
	
	if (node) {
		node->nodeID=num;
		num++;;//调用操作结点数据的函数方法
        genNodeId(node->child);//访问该结点的左孩子
        genNodeId(node->sibling);//访问该结点的右孩子
    }
    //如果结点为空，返回上一层
};//从根节点开始逐个赋Id 实现方式同学们可以自行修改

void TreeNode:: printAST(TreeNode *node)
{
	if (node) {
		printNodeInfo(node);//调用操作结点数据的函数方法
        printAST(node->child);//访问该结点的左孩子
        printAST(node->sibling);//访问该结点的右孩子
    }

}
;//打印语法树结点
/***
* 以下的几个函数皆为在printAST过程中辅助输出使用
* 同学们可以根据需要自己使用其他方法
***/
void TreeNode:: printNodeInfo(TreeNode *node)
{
	string info=node->nodeTypeInfo(node);
	cout<<"ID:"<<node->nodeID<<"          TYPE:"<<info<<endl<<endl;

}
;
void TreeNode:: printNodeConnection(TreeNode *node)
{
	if(node->child&&node->sibling)
		cout<<"child:"<<node->child->nodeID<<"sibling:"<<node->sibling->nodeID<<endl;
	else if(node->child&&!node->sibling)
		cout<<"child:"<<node->child->nodeID<<endl;
	else if(!node->child&&node->sibling)
		cout<<"sibling:"<<node->sibling->nodeID<<endl;
	else if(!node->child&&!node->sibling)
		cout<<"no connection below this node!"<<endl;
}
;
string TreeNode:: nodeTypeInfo(TreeNode *node)
{
	string typeinfo="unrecognize";
	if(node->nodeType==NODE_STMT)
		typeinfo="NODE_STMT";
	switch(node->nodeType)
	{
		case '0' :typeinfo="NODE_CONST";break;
		case '1' :typeinfo="NODE_BOOL";break;
		case '2' :typeinfo="NODE_VAR";break;
		case '3' :typeinfo="NODE_EXPR";break;
		case '4' :typeinfo="NODE_TYPE";break;
		case '5' :typeinfo="NODE_STMT";break;
		case '6' :typeinfo="NODE_PROG";break;
		case '7' :typeinfo="NODE_OP";break;		
		default :break;
   }
	if(node->nodeType=NODE_STMT)
		switch(node->stmtType)
	{
		case '0' :typeinfo="STMT_IF";break;
		case '1' :typeinfo="STMT_WHILE";break;
		case '2' :typeinfo="STMT_DECL";break;
		case '3' :typeinfo="STMT_ASSIGN";break;
		case '4' :typeinfo="STMT_PRINTF";break;
		case '5' :typeinfo="STMT_SCANF";break;		
		default :break;
   }
	if(node->nodeType=NODE_OP)
		switch(node->opType)
	{
		case '0' :typeinfo="OP_EQUAL";break;
		case '1' :typeinfo="OP_NOT";break;
		case '2' :typeinfo="OP_ADD";break;	
		default :break;
   }
	if(node->nodeType=NODE_VAR)
		switch(node->varType)
	{
		case '0' :typeinfo="VAR_INTEGER";break;
		case '1' :typeinfo="VAR_VOID";break;	
		default :break;
   }
	if(node->stmtType==STMT_IF)
		typeinfo="STMT_IF";
	return typeinfo;
	
}
;
TreeNode::TreeNode(NodeType type)
{
	this->child=nullptr;
	this->sibling=nullptr;
	this->var_name="";
	this->nodeType=type;
};


/*
int main()
{
	TreeNode *node=new TreeNode(NODE_STMT);
	node->var_name="alksjdklasjdkl";
	cout<<"ok";
	TreeNode *T=new TreeNode(NODE_STMT);

	TreeNode *S=new TreeNode(NODE_STMT);
	S->var_name="ssss";
	T->addChild(node);
	T->addChild(S);
	cout<<T->child->var_name;
	cout<<T->child->sibling->var_name;
	cout<<"ok";
	/*
		TreeNode * a;
		TreeNode * b;
		TreeNode * c;
		TreeNode * d;
		TreeNode * e;
		TreeNode * f;
		TreeNode * g;
		TreeNode * h;
	a=(TreeNode*)malloc(sizeof(TreeNode));
	b=(TreeNode*)malloc(sizeof(TreeNode));
	c=(TreeNode*)malloc(sizeof(TreeNode));
	d=(TreeNode*)malloc(sizeof(TreeNode));
	e=(TreeNode*)malloc(sizeof(TreeNode));
	f=(TreeNode*)malloc(sizeof(TreeNode));
	g=(TreeNode*)malloc(sizeof(TreeNode));
	h=(TreeNode*)malloc(sizeof(TreeNode));
		T->addChild(a);
		T->addChild(b);
		T->addChild(c);
		T->addChild(d);
		T->addChild(e);
		T->addChild(f);
		T->addChild(g);
	*/

	T->genNodeId(T);
	T->printAST(T);
	
	return 0;

}


*/
/*
int main()
{
	TreeNode *T;
	T=(TreeNode*)malloc(sizeof(TreeNode));
    T->nodeID=1;
	T->nodeType=NODE_STMT;
	T->stmtType=STMT_IF;
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
*/
