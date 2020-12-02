#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>

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
		printNodeConnection(node);
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
	cout<<"ID:"<<node->nodeID<<"          TYPE:"<<info<<endl;

}
;
void TreeNode:: printNodeConnection(TreeNode *node)
{
	if(node->child&&node->sibling)
		cout<<"child:"<<node->child->nodeID<<"sibling:"<<node->sibling->nodeID<<endl<<endl;
	else if(node->child&&!node->sibling)
		cout<<"child:"<<node->child->nodeID<<endl<<endl;
	else if(!node->child&&node->sibling)
		cout<<"sibling:"<<node->sibling->nodeID<<endl<<endl;
	else if(!node->child&&!node->sibling)
		cout<<"no connection below this node!"<<endl<<endl;
}
;
string TreeNode:: nodeTypeInfo(TreeNode *node)
{
	string typeinfo="unrecognize";
	if(node->nodeType==NODE_CONST)
	{
		typeinfo="NODE_CONST";
		typeinfo+="     valve=";
		typeinfo+=node->int_val;
	}
	if(node->nodeType==NODE_BOOL)
	{
		typeinfo="NODE_BOOL";
	}
	if(node->nodeType==NODE_VAR)
	{
		typeinfo="NODE_VAR";
		typeinfo+="     idname:";
		typeinfo+=node->var_name;
	}
	if(node->nodeType==NODE_EXPR)
		typeinfo="NODE_EXPR";			
	if(node->nodeType==NODE_TYPE)
	{
		typeinfo="NODE_TYPE";
		if(node->varType==VAR_INTEGER){typeinfo="VAR_INTEGER";}
		if(node->varType==VAR_VOID){typeinfo="VAR_VOID";}
		if(node->varType==VAR_CHAR){typeinfo="VAR_CHAR";}
	}
	if(node->nodeType==NODE_STMT)
	{
		typeinfo="NODE_STMT";
		if(node->stmtType==STMT_IF){typeinfo="STMT_IF";}
		if(node->stmtType==STMT_WHILE){typeinfo="STMT_WHILE";}
		if(node->stmtType==STMT_DECL){typeinfo="STMT_DECL";}
		if(node->stmtType==STMT_ASSIGN){typeinfo="STMT_ASSIGN";}
		if(node->stmtType==STMT_PRINTF){typeinfo="STMT_PRINTF";}
		if(node->stmtType==STMT_SCANF){typeinfo="STMT_SCANF";}
		if(node->stmtType==STMT_FOR){typeinfo="STMT_FOR";}
		if(node->stmtType==STMT_FUNC){typeinfo="new type to be complate ";}
	}
	if(node->nodeType==NODE_PROG)
		typeinfo="NODE_PROG";
	if(node->nodeType==NODE_OP)
	{
		typeinfo="NODE_OP";
		if(node->opType==OP_EQUAL){typeinfo="OP_EQUAL";}
		if(node->opType==OP_ADD){typeinfo="OP_ADD";}
		if(node->opType==OP_NOT){typeinfo="OP_NOT";}
		if(node->opType==OP_MOD){typeinfo="OP_MOD";}
		if(node->opType==OP_GT){typeinfo="OP_GT";}
		if(node->opType==OP_LE){typeinfo="OP_LE";}
		if(node->opType==OP_LT){typeinfo="OP_LT";}
		if(node->opType==OP_AND){typeinfo="OP_AND";}
		if(node->opType==OP_OR){typeinfo="OP_OR";}
	}				
	if(node->nodeType==NODE_STRING)
	{
		typeinfo="NODE_STRING";
		typeinfo+="     String_content";
		typeinfo+=node->comm;
	}
		
	if(node->nodeType==NODE_COMM)
	{
		typeinfo="NODE_COMM";
		typeinfo+="     COMMENT_content";
		typeinfo+=node->comm;
	}
	if(node->nodeType==NODE_FUNC)
		typeinfo="NODE_FUNC";	
	return typeinfo;
	
}
;
TreeNode::TreeNode(NodeType type)
{
	this->child=nullptr;
	this->sibling=nullptr;
	this->var_name="";
	this->comm="";
	this->bool_val=true;
	this->int_val="";
	this->nodeType=type;
};
