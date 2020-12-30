
%{
/****************************************************************************
myparser.y
ParserWizard generated YACC file.
需要修改，
1.结点的生成等
2.函数的声明里面的最后一个complex statment加上一个return的判断
3.有关变量的声明，这里似乎只是判断在最开头变量的声明。

Date: 2020年12月30日
****************************************************************************/
#include "common.h"
#include "lexer.h"
// #include "tree.h"
// #include "symbol.h"
// #include "typecheck.h"
// #include "gen_code.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


extern int yylex(void);

static Node* savedTree;
static Node* p;
static Node* t;
extern Node* root;
extern Node* yylval;

static BucketList st;

extern int line;
extern char* yytext;
int i;
int st_location;
ExprType currType;
%}

/////////////////////////////////////////////////////////////////////////////
// declarations section

// attribute type
%include {
#ifndef YYSTYPE
#define YYSTYPE Node*
#endif
}

%token IF
%token THEN
%token ELSE
%token WHILE
%token FOR
%token SWITCH
%token CASE
%token BREAK
%token DEFAULT
%token RETURN
%token INPUT
%token PRINT

%token INT
%token CHAR
%token FLOAT
%token DOUBLE
%token BOOL
%token VOID
%token CONST_INT
%token CONST_CHAR
%token CONST_FLOAT
%token CONST_DOUBLE

%token ID
%token NUM_Int
%token NUM_Double
%token Char
%token A

%token LP
%token RP
%token LS
%token RS
%token LB
%token RB
%token COLON
%token SEMICOLON
%token COMMA
%token ASSIGN
%token L_SHIFT
%token R_SHIFT
%token AND
%token OR
%token XOR
%token NOT
%token EQ
%token NE
%token LE
%token GE
%token LT
%token GT
%token ADD
%token SUB
%token MUL
%token DIV
%token MOD
%token BIT_AND
%token BIT_OR
%token SELF_ADD
%token SELF_SUB

%right SEMICOLON
%left COMMA
%right ASSIGN
%left OR
%left AND
%left XOR BIT_AND BIT_OR
%left EQ NE
%left LE GE LT GT
%left L_SHIFT R_SHIFT
%left ADD SUB
%left MUL DIV MOD
%left INT FLOAT CHAR VOID
%left NOT
%right RP RS RB
%left LP LS LB
%nonassoc SELF_ADD SELF_SUB

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

program				: declaration_list
						{
							printf("program   ");
							$$ = newroot()
							$$ = newStmtNode(CompK);
							$$->child[0] = $1;
							$$->parseName = "program";
							$$->attr.name = " ";
							$$->type = Void;
							savedTree = $$;
						}
					;
			
declaration_list	: declaration_list declaration
						{
							printf("declaration_list   ");
							$$ = newStmtNode(CompK);
							p = $1;
							i = 0;
							if (p)
							{
								while (p->sibling)
								{
									$$->child[i] = p;
									i++;
									p = p->sibling;
								}
								$$->child[i] = p;
								p->sibling = $2;
								$$->child[i+1] = $2;
							}
							else {
								$$->child[0] = $2;
							}
							$$->parseName = "declaration_list";
							$$->attr.name = " ";			
						}
					| /* empty */
						{
							printf("declaration_list   ");
							$$ = newStmtNode(CompK);
							$$->child[0] = NULL;
							$$->parseName = "declaration_list";
							$$->attr.name = " ";
						}
					;

declaration			: var_declaration
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("declaration   ");
							$$ = $1;
						}
					| fun_declaration
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("declaration   ");
							$$ = $1;
						}
					| fun_defination
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("declaration   ");
							$$ = $1;
						}
					;
			
var_declaration		: type_specifier ID_list SEMICOLON
						{
							printf("var_declaration   ");
							$$ = newDeclNode(VarK);
							for (t = $2; t!=NULL; t=t->sibling)
							{
								t->type = $1->type;
							}
							$$->child[0] = $1;
							$$->child[1] = $2;
							$$->parseName = "var_declaration";
							$$->attr.name = " ";
							
							// symbol table
						}
					| type_specifier ID LS NUM_Int RS SEMICOLON
					;

type_specifier		: INT
						{
							printf("INT   ");
							$$ = newExprNode(TypeK);
							$$->type = Integer;
							$$->parseName = "type_specifier";
							$$->attr.name = "int";
							currType = Integer;
						}
					| CHAR
						{
							printf("CHAR   ");
							$$ = newExprNode(TypeK);
							$$->type = Charactor;
							$$->parseName = "type_specifier";
							$$->attr.name = "char";
							currType = Charactor;
						}
					| DOUBLE
						{
							printf("DOUBLE   ");
							$$ = newExprNode(TypeK);
							$$->type = Double;
							$$->parseName = "type_specifier";
							$$->attr.name = "double";
							currType = Double;
						}
					| FLOAT
						{
							printf("FLOAT   ");
							$$ = newExprNode(TypeK);
							$$->type = Double;
							$$->parseName = "type_specifier";
							$$->attr.name = "float";
							currType = Float;
						}
					| BOOL
						{
							printf("BOOL   ");
							$$ = newExprNode(TypeK);
							$$->type = Boolean;
							$$->parseName = "type_specifier";
							$$->attr.name = "bool";
							currType = Boolean;
						}
					| VOID
						{
							printf("VOID   ");
							$$ = newExprNode(TypeK);
							$$->type = Void;
							$$->parseName = "type_specifier";
							$$->attr.name = "void";
							currType = Void;
						}
					| CONST_INT
						{
							printf("CONST_INT   ");
							$$ = newExprNode(TypeK);
							$$->type = Void;
							$$->parseName = "type_specifier";
							$$->attr.name = "const_int";
							currType = Const_Int;
						}
					| CONST_CHAR
						{
							printf("CONST_CHAR   ");
							$$ = newExprNode(TypeK);
							$$->type = Void;
							$$->parseName = "type_specifier";
							$$->attr.name = "const_char";
							currType = Const_Char;
						}
					| CONST_FLOAT
						{
							printf("CONST_FLOAT   ");
							$$ = newExprNode(TypeK);
							$$->type = Void;
							$$->parseName = "type_specifier";
							$$->attr.name = "const_float";
							currType = Const_Float;
						}
					| CONST_DOUBLE
						{
							printf("CONST_DOUBLE   ");
							$$ = newExprNode(TypeK);
							$$->type = Void;
							$$->parseName = "type_specifier";
							$$->attr.name = "const_double";
							currType = Const_Double;
						}
					;

fun_type_specifier	: INT
						{
							printf("INT   ");
							$$ = newExprNode(TypeK);
							$$->type = Integer;
							$$->parseName = "fun_type_specifier";
							$$->attr.name = "int";
							currType = Integer;
						}
					| CHAR
						{
							printf("CHAR   ");
							$$ = newExprNode(TypeK);
							$$->type = Charactor;
							$$->parseName = "fun_type_specifier";
							$$->attr.name = "char";
							currType = Charactor;
						}
					| DOUBLE
						{
							printf("DOUBLE   ");
							$$ = newExprNode(TypeK);
							$$->type = Double;
							$$->parseName = "fun_type_specifier";
							$$->attr.name = "double";
							currType = Double;
						}
					| FLOAT
						{
							printf("FLOAT   ");
							$$ = newExprNode(TypeK);
							$$->type = Float;
							$$->parseName = "fun_type_specifier";
							$$->attr.name = "float";
							currType = Float;
						}						
					| BOOL
						{
							printf("BOOL   ");
							$$ = newExprNode(TypeK);
							$$->type = Boolean;
							$$->parseName = "fun_type_specifier";
							$$->attr.name = "bool";
							currType = Boolean;
						}
					| VOID
						{
							printf("VOID   ");
							$$ = newExprNode(TypeK);
							$$->type = Void;
							$$->parseName = "fun_type_specifier";
							$$->attr.name = "void";
							currType = Void;
						}
					| /* empty */
						{
							printf("NULL   ");
							$$ = newExprNode(TypeK);
							$$->type = Void;
							$$->parseName = "fun_type_specifier";
							$$->attr.name = "empty(main)";
							currType = Void;
						}
					;

ID_list				: ID_list COMMA ID_lookup
						{
							printf("ID_list   ");
							$$ = newStmtNode(CompK);
							p = $1;
							if (p)
							{
								while (p->sibling)
								{
									$$->child[i] = p;
									i++;
									p = p->sibling;
								}
								$$->child[i] = p;
								p->sibling = $3;
								$$->child[i+1] = $3;
							}
							else {
								$$->child[0] = $3;
							}
							$$->parseName = "ID_list";
							$$->attr.name = " ";
						}
					| ID_list COMMA assign_decl
						{
							printf("ID_list(assign)   ");
							$$ = newStmtNode(CompK);
							p = $1;
							if (p)
							{
								while (p->sibling)
								{
									$$->child[i] = p;
									i++;
									p = p->sibling;
								}
								$$->child[i] = p;
								p->sibling = $3;
								$$->child[i+1] = $3;
							}
							else {
								$$->child[0] = $3;
							}
							$$->parseName = "ID_list";
							$$->attr.name = " ";					
						}
					| assign_decl
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("ID_list(assign)   ");
							$$ = $1;
						}
					| ID_lookup
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("ID_list   ");
							$1->sibling = NULL;
							$$ = $1;
						}
					;

assign_decl			: ID_lookup ASSIGN expression // 注意是decl所以用ID_lookup
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("assign_decl   ");
							$$ = newStmtNode(AssignK);
							$$->child[0] = $1;
							$$->child[1] = $3;
							$$->parseName = "assign_decl";
							$$->attr.name = "=";
						}
					;

ID_lookup			: ID 	// 声明时使用lookup
						{
							printf("ID_lookup   ");
							$$ = newExprNode(IdK);
							$$->parseName = "ID";
							$$->attr.name = $1->attr.name;
							$$->type = currType;
							if (st_lookup($1->attr.name) == -1)
								/* 未在表中，则视为新的定义，在表中存储于位置+1 */
								st_insert($1->attr.name, line, st_location++, currType);
							else
								/* 已在表中，则增加新的位置 */ 
								st_insert($1->attr.name, line, 0, currType);
						}
					;

ID_get				: ID 	// 调用时使用get, 在类型检查部分要负责返出变量类型
						{
							printf("ID_get   ");
							$$ = newExprNode(IdK);
							$$->parseName = "ID";
							$$->attr.name = $1->attr.name;
							if (st_lookup($1->attr.name) == -1)
								/* 未在表中，报“未定义”错误 */
								printf("Error. %s has not been declared yet.\n", $1->attr.name);
							else
							{
								st = st_get($1->attr.name);
								if (st->type==Const_Int || st->type==Const_Char || st->type==Const_Float || st->type==Const_Double)
									$$->type = Const_init;
								else
									$$->type = st->type;
							}
						}
					;

fun_declaration		: type_specifier ID_lookup LP params RP SEMICOLON
						{
							printf("fun_declaration   ");
							$$ = newDeclNode(FunK);
							t = $2;
							t->type = $1->type;
							$$->child[0] = $1;
							$$->child[1] = $2;
							$$->child[2] = $4;
							$$->parseName = "fun_declaration";
							$$->attr.name = " ";
						}
					;

fun_defination		: fun_type_specifier ID_lookup LP params RP compound_stmt //考虑加入返回值的判断
						{
							printf("fun_defination   ");
							$$ = newDeclNode(FunK);
							t = $2;
							t->type = $1->type;
							$$->child[0] = $1;
							$$->child[1] = $2;
							$$->child[2] = $4;
							$$->child[3] = $6;		//返回值的语句在这里面
							$$->type = $6->type;
							$$->parseName = "fun_defination";
							$$->attr.name = $2->attr.name;
						}
					;
					
params				: params param_list
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("params   ");
							$$ = $2;
						}
					| /* empty */
						{
							$$ = newExprNode(TypeK);
							printf("params(empty)   ");
							$$->parseName = "params";
							$$->attr.name = "empty";

							// symbol table
						}
					;

param_list			: param_list COMMA param
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("param_list   ");
							p = $1;
							while (p->sibling)
							{
								p = p->sibling;
							}
							p->sibling = $2;
							$$ = $1;
						}
					| param
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("param_list   ");
							$$ = $1;					
						}
					;

param				: type_specifier ID_lookup
						{
							printf("param   ");
							$$ = newExprNode(ParamK);
							$$->type = $1->type;
							$2->type = $1->type;
							$$->child[0] = $1;
							$$->child[1] = $2;
							$$->parseName = "param";
							$$->attr.name = " ";
						}
					| type_specifier ID_lookup LS RS
						{
							printf("param   ");
							$$ = newExprNode(ParamK);
							$$->type = $1->type;
							$2->type = $1->type;
							$$->child[0] = $1;
							$$->child[1] = $2;
							$$->parseName = "param";
							$$->attr.name = " ";
						}
					;

local_declaration	: local_declaration var_declaration
						{
							printf("local_declaration   ");
							$$ = newDeclNode(CompK);
							p = $1;
							i = 0;
							if (p)
							{
								while (p->sibling)
								{
									$$->child[i] = p;
									i++;
									p = p->sibling;
								}
								$$->child[i] = p;
								p->sibling = $2;
								$$->child[i+1] = $2;
							}
							else {
								$$->child[0] = $2;
							}
							$$->parseName = "local_declaration";
							$$->attr.name = " ";
						}
					| /* empty */
						{
							printf("local_declaration_empty   ");
							$$ = newDeclNode(CompK);
							$$->child[0] = NULL;
							$$->parseName = "local_declaration_empty";
							$$->attr.name = " ";
							
							// symbol table
						}
					;

statement_list		: statement_list statement
						{
							printf("statement_list   ");
							$$ = newStmtNode(CompK);
							p = $1;
							i = 0;
							if (p)
							{
								while (p->sibling)
								{
									$$->child[i] = p;
									i++;
									p = p->sibling;
								}
								$$->child[i] = p;
								p->sibling = $2;
								$$->child[i+1] = $2;
							}
							else {
								$$->child[0] = $2;
							}
							$$->parseName = "statement_list";
							$$->attr.name = " ";
						}
					| /* empty */
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement_list   ");
							$$ = NULL;
						}
					;

statement			: expression_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| compound_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| if_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| while_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| for_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| assign_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| return_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| fun_call
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| input_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					| print_stmt
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("statement   ");
							$$ = $1;
						}
					;

expression_stmt		: expression SEMICOLON
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("expression_stmt   ");
							$$ = $1;
						}
					| SEMICOLON
						{
							printf("expression_stmt   ");
							$$ = newStmtNode(CompK);
							$$->child[0] = NULL;
							$$->parseName = "expression_list_empty";
							$$->attr.name = "empty";
						}
					;

compound_stmt		: LB local_declaration statement_list return_stmt RB  //局部变量的声明
						{
							printf("compound_stmt   ");
							$$ = newStmtNode(CompK);
							$$->child[0] = $2;
							$$->child[1] = $3;
							$$->child[2] = $4;
							$$->parseName = "statement";
							$$->attr.name = "compound";
							$$->type = $4->type;
							// symbol table
						}
					;

if_stmt				: IF LP expression RP statement
						{
							printf("if_stmt   ");
							$$ = newStmtNode(IfK);
							$$->child[0] = $3;
							$$->child[1] = $5;
							$$->parseName = "statement";
							$$->attr.name = "if";
						}
					| IF LP expression RP statement ELSE statement
						{
							printf("if_else   ");
							$$ = newStmtNode(IfK);
							$$->child[0] = $3;
							$$->child[1] = $5;
							$$->child[2] = $7;
							$$->parseName = "statement";
							$$->attr.name = "if&else";
						}
					;

while_stmt			: WHILE LP expression RP LB statement_list RB
						{
							printf("while_stmt   ");
							$$ = newStmtNode(WhileK);
							$$->child[0] = $3;
							$$->child[1] = $6;
							$$->parseName = "statement";
							$$->attr.name = "while";
						}
					;

for_stmt			: FOR LP assign_stmt SEMICOLON expression_list SEMICOLON assign_stmt RP statement_list
						{
							printf("for_stmt   ");
							$$ = newStmtNode(ForK);
							$$->child[0] = $3;
							$$->child[1] = $5;
							$$->child[2] = $7;
							$$->child[3] = $9;
							$$->parseName = "statement";
							$$->attr.name = "for";
						}
					| FOR LP expression_list SEMICOLON expression_list SEMICOLON expression_list RP statement_list
						{
							printf("for_stmt   ");
							$$ = newStmtNode(ForK);
							$$->child[0] = $3;
							$$->child[1] = $5;
							$$->child[2] = $7;
							$$->child[3] = $9;
							$$->parseName = "statement";
							$$->attr.name = "for";
						}
					| FOR LP assign_stmt SEMICOLON expression_list SEMICOLON expression_list RP statement_list
						{
							printf("for_stmt   ");
							$$ = newStmtNode(ForK);
							$$->child[0] = $3;
							$$->child[1] = $5;
							$$->child[2] = $7;
							$$->child[3] = $9;
							$$->parseName = "statement";
							$$->attr.name = "for";
						}	
					;

expression_list		: expression
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("expression_list   ");
							$$ = $1;
						}
					| /* empty */
						{
							printf("expression_list   ");
							$$ = newStmtNode(CompK);
							$$->child[0] = NULL;
							$$->parseName = "expression_list_empty";
							$$->attr.name = "empty";
						}
					;
input_stmt			: INPUT LP ID_get RP
						{
							printf("input_stmt   ");
							$$ = newStmtNode(InputK);
							$$->child[0] = $3;
							$$->parseName = "statement";
							$$->attr.name = "input";
						}
 					;

print_stmt			: PRINT LP expression RP
						{
							printf("print_stmt   ");
							$$ = newStmtNode(PrintK);
							$$->child[0] = $3;
							$$->parseName = "statement";
							$$->attr.name = "print";
						}
					;

assign_stmt			: ID_get ASSIGN expression
						{
							printf("assign_stmt   ");
							$$ = newStmtNode(AssignK);
							$$->child[0] = $1;
							$$->child[1] = $3;
							$$->parseName = "assign_stmt";
							$$->attr.name = "ASSIGN_op";
						}
					;

return_stmt			: RETURN SEMICOLON
						{
							printf("return_stmt   ");
							$$ = newStmtNode(ReturnK);
							$$->type = Void;
						}
					| RETURN expression SEMICOLON
						{
							printf("return_stmt   ");
							$$ = newStmtNode(ReturnK);
							$$->child[0] = $2;
							$$->parseName = "return_stmt";
							$$->attr.name = " ";
							$$->type = $2->type;
						}
					;

expression			: expression relop expression
						{
							printf("expression   ");
							$$ = newExprNode(OpK);
							$$->child[0] = $1;
							$$->child[1] = $3;
							$$->attr.name = $2->attr.name;
							$$->type = $2->type;
							$$->parseName = "expression";
						}
					| SUB expression
						{
							printf("NOT   ");
							$$ = newExprNode(OpK);
							$$->child[0] = $2;
							$$->attr.name = $2->attr.name;
							$$->parseName = "expression";
						}
					| expression SELF_ADD
						{
							printf("SELF_ADD   ");
							$$ = newExprNode(OpK);
							$$->child[0] = $1;
							$$->attr.name = $1->attr.name;
							$$->parseName = "expression";							
						}
					| expression SELF_SUB
						{
							printf("SELF_SUB   ");
							$$ = newExprNode(OpK);
							$$->child[0] = $1;
							$$->attr.name = $1->attr.name;
							$$->parseName = "expression";							
						}
					| expression addop expression
						{
							printf("expression   ");
							$$ = newExprNode(OpK);
							$$->child[0] = $1;
							$$->child[1] = $3;
							$$->attr.name = $2->attr.name;
							$$->type = $1->type;
							$$->parseName = "expression";
						}
					| expression mulop expression
						{
							printf("expression   ");
							$$ = newExprNode(OpK);
							$$->child[0] = $1;
							$$->child[1] = $3;
							$$->attr.name = $2->attr.name;
							$$->type = $1->type;
							$$->parseName = "expression";
						}
					| expression
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("expression   ");
							$$ = $1;
						}
					| LP expression RP
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("expression   ");
							$$ = $2;
						}
					| factor
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("term   ");
							$$ = $1;
						}
					;
					
relop				: EQ
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("==   ");
							$$->parseName = "relop";
							$$->attr.name = "EQ_op";
							$$->type = Boolean;
						}
					| NE
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("!=   ");
							$$->parseName = "relop";
							$$->attr.name = "NE_op";
							$$->type = Boolean;
						}
					| LE
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("<=   ");
							$$->parseName = "relop";
							$$->attr.name = "LE_op";
							$$->type = Boolean;
						}
					| GE
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf(">=   ");
							$$->parseName = "relop";
							$$->attr.name = "GE_op";
							$$->type = Boolean;
						}
					| LT
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("<   ");
							$$->parseName = "relop";
							$$->attr.name = "LT_op";
							$$->type = Boolean;
						}
					| GT
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf(">   ");
							$$->parseName = "relop";
							$$->attr.name = "GT_op";
							$$->type = Boolean;
						}
					| AND
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("&&   ");
							$$->parseName = "relop";
							$$->attr.name = "AND_op";
							$$->type = Boolean;
						}
					| OR
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("||   ");
							$$->parseName = "relop";
							$$->attr.name = "OR_op";
							$$->type = Boolean;
						}
					| XOR
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("^   ");
							$$->parseName = "relop";
							$$->attr.name = "XOR_op";
							$$->type = Integer;
						}
					| L_SHIFT
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("<<   ");
							$$->parseName = "relop";
							$$->attr.name = "L_SHIFTt_op";
							$$->type = Integer;
						}
					| R_SHIFT
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf(">>   ");
							$$->parseName = "relop";
							$$->attr.name = "R_SHIFTt_op";
							$$->type = Integer;
						}
					| BIT_AND
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("&   ");
							$$->parseName = "relop";
							$$->attr.name = "BIT_AND_op";
							$$->type = Integer;
						}
					| BIT_OR
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("|   ");
							$$->parseName = "relop";
							$$->attr.name = "BIT_OR_op";
							$$->type = Integer;
						}
					| MOD
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("%   ");
							$$->parseName = "relop";
							$$->attr.name = "MOD_op";
							$$->type = Integer;
						}
					;

addop				: ADD
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("+   ");
							$$->parseName = "addop";
							$$->attr.name = "ADD_op";
							// $$->type = ADD;
						}
					| SUB
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("-   ");
							$$->parseName = "addop";
							$$->attr.name = "SUB_op";
						}
					;
					
mulop				: MUL
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("*   ");
							$$->parseName = "mulop";
							$$->attr.name = "MUL_op";
						}
					| DIV
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("/   ");
							$$->parseName = "mulop";
							$$->attr.name = "DIV_op";
						}
					;

factor				: LP expression RP
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("factor   ");
							$$ = $2;
						}
					| ID_get
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("factor   ");
							$$ = $1;
						}
					| NUM_Int
						{
							printf("factor   ");
							$$ = newExprNode(ConstK);
							$$->type = Integer;
							$$->parseName = "NUM_Int";
							$$->attr.name = $1->attr.name;
						}
					| NUM_Double
						{
							printf("factor   ");
							$$ = newExprNode(ConstK);
							$$->type = Double;
							$$->parseName = "NUM_Double";
							$$->attr.name = $1->attr.name;
						}
					| Char
						{
							printf("factor   ");
							$$ = newExprNode(ConstK);
							$$->type = Integer;
							$$->parseName = "Char";
							$$->attr.name = $1->attr.name;
						}
					| A
						{
							printf("factor   ");
							$$ = newExprNode(ConstK);
							$$->type = Charactor;
							$$->parseName = "'a'";
							$$->attr.name = "'a'";
						}
					;					

fun_call			: ID_get LP args RP
						{
							printf("fun_call   ");
							$$ = newExprNode(FunK);
							$$->child[0] = $1;
							$$->child[1] = $3;
							$$->parseName = "fun_call";
							$$->attr.name = $1->attr.name;
						}
					;

args				: arg_list
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("args   ");
							$$ = $1;
						}
					| /* empty */
						{
							printf("args_empty   ");
							$$ = newStmtNode(CompK);
							$$->parseName = "args";
							$$->attr.name = "empty";
						}
					;

arg_list			: arg_list COMMA expression
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("arg_list   ");
							p = $1;
							while (t->sibling)
							{
								t = t->sibling;
							}
							t->sibling = $2;
							$$ = $1;
						}
					| expression
						{
							$$ = (Node*) malloc(sizeof(Node));
							printf("arg_list   ");
							$$ = $1;
						}
					;

%%

/////////////////////////////////////////////////////////////////////////////
// programs section


void yyerror(char* s, int state)
{
	fprintf(stderr, "%s ", s);    
	if (state != -1)
	{
		fprintf(stderr, ": line %d, ", line);
		fprintf(stderr, "before %s \n", yytext);
	}
}
