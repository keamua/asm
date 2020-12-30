#include <iostream>
#include <string>

using namespace ::std;

#include "tree.h"
#include "symbol.h"
//#include "lexer.hpp"
//#include "parser.hpp"

int nodecount = 0;					//结点个数
int treeLevel = 0;					//遍历层数
int tempCount = 0;					//临时变量个数
extern int lineno;					//行号
extern symbol_table symtbl;			//符号表

tree parse_tree;					//语法树

void Node::output(void)				//打印结点信息
{
	// write your own code to print the parser tree
	cout << endl;
}
void printTree(Node* currNode) 		//打印整个树的信息，先序遍历 
{
	char* transType = (char*)malloc(40);
	if (currNode != NULL)
	{
		for (int a = 0; a<treeLevel; a++)
		{
			printf("   ");
		}
		
		switch (currNode->type)
		{
			case 0: transType = "Notype"; break;
			case 1: transType = "Interge"; break;
			case 2: transType = "Boolean"; break;
			case 3: transType = "Charactor"; break;
			case 4: transType = "Const_Int"; break;
			case 5: transType = "Const_Char"; break;
			case 6: transType = "Void"; break;
			case 7: transType = "ErrorType"; break;
			default: break;
		}
		
		printf("%s [ %s ] --- { %s }\n", currNode->parseName, currNode->attr, transType);

		Node* childNode;
		childNode = (Node*)malloc(sizeof(Node));
		int childNum = 0;
		for (int i = 0; currNode->children[i] != NULL; i++)
		{
			childNum++;
		}
		treeLevel++;
		for (int j = 0; j < childNum; j++)
		{
			childNode = currNode->children[j];
			printTree(childNode);
			treeLevel--;
		}
	}
}

void tree::type_check(Node *t)		//类型检查的函数，分三类，每类检查不同
{
	if (t->kind == STMT_NODE)		//如果是陈述句的类型
	{
		if (t->kind_kind == WHILE_STMT)	//while的第一个孩子即括号里的，保证是布尔型（或者是整型？）
		{
			if (t->children[0]->type != Boolean || t->children[0]->type != Integer)
			{
				cerr << "Bad boolean type at line: " << t->lineno << endl;
			}
			else if(t->children[0]->type == Integer)
			{
				t->type = Boolean;	//整型的时候按照建立结点时是看expr的类型位integer，这里换成boolean。
			}
			else
				return;
		}
		else if(t->kind_kind == FOR_STMT) //for的第二个孩子，即判断的语句
		{
			if(t->children[1]->type != Boolean || t->children[1]->type != Integer)
			{
				cerr << "Bad boolean type at line: " << t->lineno << endl;
			}
			else if(t->children[1]->type == Integer)
			{
				t->type = Boolean;	
			}
			else
				return;
		}
		else if(t->kind_kind == IF_STMT) //if同while
		{
			if(	t->children[0]->type != Boolean || t->children[0]->type != Integer)
			{
				cerr << "Bad boolean type at line: " << t->lineno << endl;
			}
			else if(t->children[0]->type == Integer)
			{
				t->type = Boolean;	
			}
			else
				return;
		}
		else if(t->kind_kind == RETURN_STMT) //应该判断一下和函数的类型是否相同，待完成
		{
			return;
		}
		else if(t->kind_kind == ASSIGN_STMT)//赋值语句保证，等号左右两边的值类型相同，强制转换就算了吧
		{
			if(t->children[0]->type != t->children[1]->type )
			{
				cerr << "assignment of incompatible type at line: " << t->lineno << endl;
			}
			else
				return;
		}
		else
			return;
					
	}
	else if(t->kind == EXPR_NODE)	//如果是表达式的类型
	{
		if(t->kind_kind == OP_EXPR)	//逻辑运算 或者 加减乘除的表达式，左右也要一样，仍然不包括 +'a' 这样的强制转换，可以考虑实现
		{
			if(t->children[0]->type != t->children[1]->type)
			{
				cerr << "Op applied to incompatible pair at line: " << t->lineno << endl;
			}
			if(	(t->attr.op == EQt) || 
				(t->attr.op == NEt) || 
				(t->attr.op == LEt) || 
				(t->attr.op == GEt) ||
				(t->attr.op == LTt) || 
				(t->attr.op == GTt) || 
				(t->attr.op == ANDt) || 
				(t->attr.op == ORt) || 
				(t->attr.op == XORt))
				t->type = Boolean;	//是逻辑运算则布尔型
			else
				t->type = Integer;	//不是的话类型是整型
			return;
		}
		else 
		{
			return;
		}
		
	}
	else if(t->kind == DECL_NODE)	//声明类型的错误判断，对于id重复声明和使用未声明的变量在，语法分析中有实现，这里处理函数声明类型和返回值不同（未完成）。
	{
		if(t->kind_kind == FUN_DECL)
		{
			if (t->children[0]->type != t->children[3]->type)
			{
				cerr << "function's return different the claim at line: " << t->lineno << endl;
			}
			else
				return;
		}
		else
			return;
	}
	return;
}

void tree::get_temp_var(Node *t)	//获得一个产生一个临时变量，这个判断条件不太懂
{
	if (t->kind != EXPR_NODE)		//必须是表达式中的临时变量
		return;
	if (t->attr.op < ADDt || t->attr.op > DIVt)	//而且是加减乘除
		return;

	Node *arg1 = t->children[0];	//第一个参数
	Node *arg2 = t->children[1];	//第二个参数

	if (arg1->kind_kind == OP_EXPR)	//如果第一个参数是运算类型的（难道临时变量的要求只能是常数，或者说在最下级已经定义过了？）
		temp_var_seq--;				//临时变量的序号减一个
	if (arg2 && arg2->kind_kind == OP_EXPR)	//判断第二个变量
		tree::temp_var_seq--;
	t->temp_var = tree::temp_var_seq;	//在这个结点上会有一个临时变量，他的序号是这个
	tree::temp_var_seq++;	//序号增加
}

Node* tree::NewRoot(int kind, int kind_kind, NodeAttr attr, int type,
				   Node *child1, Node *child2, Node *child3, Node *child4) //生成新的树的结点
{
	Node *t = new Node;
	
	if (NULL == t)
		cerr << "Out of memory at line %d\n" << lineno;
	else
	{
		t->kind = kind;
		t->kind_kind = kind_kind;
		t->attr = attr;
		t->type = type;
		t->children[0] = child1;
		t->children[1] = child2;
		t->children[2] = child3;
		t->children[3] = child4;
		t->lineno = lineno;
		t->seq = tree::node_seq++;
		t->sibling = NULL;
		t->label.begin_label = "";
		t->label.next_label = "";
		t->label.true_label = "";
		t->label.false_label = "";
		root = t;
		type_check(t); // type check
		get_temp_var(t); // generate temp veriables
	}
	return t;
}

string tree::new_label(void) //打label
{
	char tmp[20];

	sprintf(tmp, "@%d", tree::label_seq);
	tree::label_seq++;
	return tmp;
}

void tree::stmt_get_label(Node *t)	//陈述句的打label
{
	switch (t->kind_kind)			
	{
	case COMP_STMT:					//复杂陈述句
		{
			Node *last;
			Node *p;
			for (p = t->children[0]; p->kind == DECL_NODE; p = p->sibling) ; //找到第一个不是声明语句的p

			p->label.begin_label = t->label.begin_label;	//p的开始是t的开始
			for (; p; p = p->sibling)
			{
				last = p;									//对p递归生成label，直到p是t的第一个孩子的最后一个结点
				recursive_get_label(p);
			}

			t->label.next_label = last->label.next_label;	//t的下一个label是第一个孩子最后一个结点p的下一个label
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;//t的兄弟的开始label是t的nextlabel联系起来。
		}
		break;

	case WHILE_STMT:				//while句
		{
			Node *e = t->children[0];	//两个孩子
			Node *s = t->children[1];

			if (t->label.begin_label == "")
				t->label.begin_label = new_label();
			s->label.next_label = t->label.begin_label;

			s->label.begin_label = e->label.true_label = new_label();

			if (t->label.next_label == "")
				t->label.next_label = new_label();
			e->label.false_label = t->label.next_label;
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;

			recursive_get_label(e);
			recursive_get_label(s);
		}
    /* 补充上各种类型的label生成联系 */
	}
}

void tree::expr_get_label(Node *t)  //表达式的label的生成
{
	if (t->type != Boolean)			//如果不是逻辑表达式则不需要
		return;

	Node *e1 = t->children[0];		
	Node *e2 = t->children[1];
	switch (t->attr.op)
	{
	case ANDt:	//AND类型的话
		e1->label.true_label = new_label();				//如果第一个参数的真 新开一个
		e2->label.true_label = t->label.true_label;		//第二个的真 是结点的真
		e1->label.false_label = e2->label.false_label = t->label.false_label; //e1和e2的假是结点t的假
		break;

	case ORt:
		break;
    /* 需要补充的其他的表达式类型 */
	}
	if (e1)
		recursive_get_label(e1);
	if (e2)
		recursive_get_label(e2);
}

void tree::recursive_get_label(Node *t)		//递归生成label
{
	if (t->kind == STMT_NODE)
		stmt_get_label(t);
	else if (t->kind == EXPR_NODE)
		expr_get_label(t);
}

void tree::get_label(void)
{
	Node *p = root;

	p->label.begin_label = "_start";
	recursive_get_label(p);
}

void tree::gen_header(ostream &out) //生成头文件
{
	out << "# your asm code header here" << endl;
	/*your code here*/
}

void tree::gen_decl(ostream &out, Node *t)	//生成声明的变量代码
{
    out << endl << "# define your veriables and temp veriables here" << endl;
	out << "\t.bss" << endl;
	for (; t->kind == DECL_NODE; t = t->sibling)
	{
		for (Node *p = t->children[1]; p; p = p->sibling)
			if (p->type == Integer)
				out << "_" << symtbl.getname(p->attr.symtbl_seq) << ":" << endl;
                out << "\t.zero\t4" << endl;
                out << "\t.align\t4" << endl;
	}
	
	for (int i = 0; i < temp_var_seq; i++) //临时变量的汇编声明
	{
		out << "t" <<  i << ":" << endl;
        out << "\t.zero\t4" << endl;
        out << "\t.align\t4" << endl;
	}
}

void tree::stmt_gen_code(ostream &out, Node *t) //生成代码
{
	if (t->kind_kind == COMP_STMT)
	{
		for (int i = 0; t->children[i]; i++)
		{
			recursive_gen_code(out, t->children[i]);
			for (Node *p = t->children[i]->sibling; p; p = p->sibling)
				recursive_gen_code(out, p);
		}
	}
	else if (t->kind_kind == WHILE_STMT)
	{
		if (t->label.begin_label != "")
			out << t->label.begin_label << ":" << endl;
		recursive_gen_code(out, t->children[0]);
		recursive_gen_code(out, t->children[1]);
		out << "\tjmp " << t->label.begin_label << endl;
	}
	else if (t->kind_kind == PRINT_STMT)
	{
		/* ... */
	}
    /* ... */
}

void tree::expr_gen_code(ostream &out, Node *t)
{
	Node *e1 = t->children[0];
	Node *e2 = t->children[1];
	switch (t->attr.op)
	{
	case ASSIGNt:
		break;

	case ADDt:
		out << "\tmovl $";
		if (e1->kind_kind == ID_EXPR)
			out << "_" << symtbl.getname(e1->attr.symtbl_seq);
		else if (e1->kind_kind == CONST_EXPR)
			out << e1->attr.vali;
		else out << "t" << e1->temp_var;
		out << ", %eax" <<endl;
		out << "\taddl $";
		if (e2->kind_kind == ID_EXPR)
			out << "_" << symtbl.getname(e2->attr.symtbl_seq);
		else if (e2->kind_kind == CONST_EXPR)
			out << e2->attr.vali;
		else out << "t" << e2->temp_var;
		out << ", %eax" << endl;
		out << "\tmovl %eax, $t" << t->temp_var << endl;
		break;
    case ANDt:
        out << "\t# your own code of AND operation here" << endl;
        out << "\tjl @1" << endl;
        out << "\t# your asm code of AND operation end" << endl;
	case LTt:
		break;
    /* ... */
	}
}

void tree::recursive_gen_code(ostream &out, Node *t) //递归生成代码
{
	if (t->kind == STMT_NODE)
	{
		stmt_gen_code(out, t);
	}
	else if (t->kind == EXPR_NODE && t->kind_kind == OP_EXPR)
	{
		expr_gen_code(out, t);
	}
}

void tree::gen_code(ostream &out)	//生成代码的总函数
{
	gen_header(out);
	Node *p = root->children[0];
	if (p->kind == DECL_NODE)
		gen_decl(out, p);
    out << endl << endl << "# your asm code here" << endl;
	out << "\t.text" << endl;
    out << "\t.globl _start" << endl;
	recursive_gen_code(out, root);
	if (root->label.next_label != "")
		out << root->label.next_label << ":" << endl;
	out << "\tret" << endl;
}
