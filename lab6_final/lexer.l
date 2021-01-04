%option noyywrap
%{
    #include"common.h"
    #include"main.tab.hh"
    int lineno=1;
    extern symbol_table SymbolsTable;
%}

INTEGER (0|[1-9][0-9]*)
ID [[:alpha:]_][[:alpha:][:digit:]_]*

EOL (\r\n|\n)
WHITE [\t ]

BlockComm \/\*(?:[^\*]|\*+[^\/\*])*\*+\/
LineComm \/\/.*\n?

ConstStr (\".*\")
ConstChar (\'.+\')

%%

{EOL} {
    ++lineno;
}
{LineComm} {
    TreeNode *node = new TreeNode(STMT_NODE);
//    node->exprType = CONST_EXPR;
    node->str_val = string(yytext);
    node->info = "comment";
//    node->type = STRINGt;
    yylval = node;
    return COMMENT;
}
{BlockComm} {
    TreeNode *node = new TreeNode(STMT_NODE);
    node->str_val = string(yytext);
    node->info = "comment";
    string str = string(yytext);
    int index = 0; 
    while((index=str.find("\n",index)) < str.length()){
		++lineno;
		index++;
	}
    yylval = node;
    return COMMENT;
}

"true" {
    TreeNode *node = new TreeNode(EXPR_NODE);
    node->exprType = OP_EXPR;
    node->bool_val = true;
    node->str_val = "true";
    node->info = "true";
    node->type= BOOLEAN;
    yylval = node;
    return TRUE;
}
"false" {
    TreeNode *node = new TreeNode(EXPR_NODE);
    node->exprType = OP_EXPR;
    node->bool_val = false;
    node->str_val = "false";
    node->info = "false";
    node->type= BOOLEAN;
    yylval = node;
    return FALSE;
}

"int" return INT;
"void" return VOID;
"char" return CHAR;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
"string" return STRING;
"bool" return BOOL;
"const" return CONST;

"if" return IF;
"while" return WHILE;
"else" return ELSE;
"for" return FOR;

"main" return MAIN;


"return" return RETURN;
"printf" return PRINTF;
"scanf" return SCANF;

"=" return ASSIGN;
"+=" return ADDE;
"-=" return SUBE;
"*=" return MULE;

"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"%" return MOD;
"++" return SADD;
"--" return SSUB;

"==" return EQUAL;
"!=" return NE;
">" return GT;
">=" return GE;
"<" return LT;
"<=" return LE;
"!" return NOT;

"&&" return AND;
"||" return OR;

"&" return QUOTE;

"(" return LPAREN;
")" return RPAREN;
"[" return LBRACK;
"]" return RBRACK;
"{" return LBRACE;
"}" return RBRACE;

";" return SEMICOLON;
"," return COMMA;
"." return DOT;
":" return COLON;
"->" return POT;


{INTEGER} {
    TreeNode *node = new TreeNode(EXPR_NODE);
    node->exprType = CONST_EXPR;
    node->int_val = atoi(yytext);
    node->str_val = string(yytext);
    node->info = "Interger";
    node->type = INTEGERt;
    yylval = node;
    return INTEGER;
}
{ID} {
    TreeNode *node = new TreeNode(EXPR_NODE);
    node->exprType = ID_EXPR;
    node->var_name = string(yytext);
    node->type = VAR;
    node->info = "ID";
//    int i = SymbolsTable.lookup(node->var_name);
//    if(i != -1){
//        node->type=(Type)SymbolsTable.table[i].type;
//    }
    yylval = node;
    return ID;
}
{ConstStr} {
    TreeNode *node = new TreeNode(EXPR_NODE);
    node->exprType = CONST_EXPR;
    node->str_val = string(yytext);
    node->info = "ConstString";
    node->type = STRINGt;
    yylval = node;
    return ConstStr;
}
{ConstChar} {
    TreeNode *node = new TreeNode(EXPR_NODE);
    node->exprType = CONST_EXPR;

    string s = string(yytext);
    node->char_val = s.at(1);
    node->str_val = "\"";
    node->str_val += node->char_val;
    if(s.at(1)=='\\')
        node->str_val += s.at(2);
    node->str_val += "\"";
    
    node->info = "ConstChar";
    node->type = CHARACTOR;
    yylval = node;
    return ConstChar;
}

{WHITE}

%%