#ifndef TREE_H
#define TREE_H
enum
{
	STMT_NODE = 0,
	EXPR_NODE,
	DECL_NODE
};

enum
{
	WHILE_STMT = 0,
	FOR_STMT,
	IF_STMT,
	ASSIGN_STMT,
	RETURN_STMT,
	SCANF_STMT,
	PRINT_STMT,
	COMP_STMT
};

enum
{
	TYPE_EXPR = 0,
	OP_EXPR,
    CONST_EXPR,
	ID_EXPR
};

enum
{
	VAR_DECL = 0,
	FUN_DECL
};

enum
{
	Notype = 0,
	Integer,
	Boolean,
	Charactor,
	Const_Int, 
	Const_Char, 
	Void, 
	ErrorType
};
enum 
{ 
	ADDt = 0, 
	SUBt, 
	MULt, 
	DIVt, 
	EQt, 
	NEt, 
	LEt, 
	GEt, 
	LTt, 
	GTt, 
	ANDt, 
	ORt, 
	XORt, 
	L_SHIFTt, 
	R_SHIFTt, 
	ASSIGNt, 
	MODt, 
	BIT_ANDt, 
	BIT_ORt, 
	SELF_ADDt, 
	SELF_SUBt 
};

#define MAX_CHILDREN 4


union NodeAttr {
	int op;				//逻辑运算操作符
	int vali;			//数值
	char valc;			//字符串
	int symtbl_seq;		//符号表位置，id序号
	
	NodeAttr(void) { op = 0; }
	NodeAttr(int i)	{ op = i; }
	NodeAttr(char c) { valc = c; }
};

struct Label {	//最后有四个label，我理解的label是语句之间的联系，即下一步需要执行的地方，则对于一个语句，要么有开始位置，还有下一个执行的位置，有跳转的还得有真值时的跳转位置以及非真时的跳转位置
	string true_label;		//真
	string false_label;		//假
	string begin_label;		//开始
	string next_label;		//结束
};

struct Node
{
	struct Node *children[MAX_CHILDREN];	//根据语法最多四个孩子
	struct Node *sibling;					//兄弟结点，即下一个
	int lineno;								//行号
	int kind;								//三个大类，分别是：陈述，表达式，定义
	int kind_kind;							//三个大类的小类，分别有：陈述的：while，if，for……，表达式的：+，-，*，/，>，<……，和定义的：变量，函数。
	NodeAttr attr;							//不同的类别储存的不同的信息，逻辑符，整数，字符
	int type;								//类别：整型，布尔型，无类型，字符型。
	int seq;								//序号
	int temp_var;							//临时变量，在ADD中会有
	char* parseName;						//储存语法分析时该结点的信息
	Label label;							//做的label

	void output(void);						//打印结点信息
};

class tree
{
private:
	Node *root;								//根结点
	int node_seq = 0;						//结点个数
	int temp_var_seq = 0;					//临时变量个数
	int label_seq = 0;						//label个数

private:
	void type_check(Node *t);				//检查类型
	void get_temp_var(Node *t);				//得到临时变量
	string new_label(void);					//生成新的label
	void recursive_get_label(Node *t);		//递归生成label
	void stmt_get_label(Node *t);			//陈述句的生成label
	void expr_get_label(Node *t);			//表达式的生成label
	void gen_header(ostream &out);			//生成汇编的开始那一段的代码
	void gen_decl(ostream &out, Node *t);	//生成定义的代码，
	void recursive_gen_code(ostream &out, Node *t);	//递归生成代码
	void stmt_gen_code(ostream &out, Node *t);	//陈述句生成代码
	void expr_gen_code(ostream &out, Node *t);	//表达式生成代码

public:
	Node *NewRoot(int kind, int kind_kind, NodeAttr attr, int type,
		Node *child1 = NULL, Node *child2 = NULL, Node *child3 = NULL, Node *child4 = NULL); //创建新的结点
	void get_label(void);					//生成label函数
	void gen_code(ostream &out);			//生成代码的函数
};
void printTree(Node* savedTree);		//遍历生成树的信息

#endif
