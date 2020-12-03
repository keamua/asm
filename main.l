%option noyywrap
%{
    #include"common.h"
    #include"main.tab.hh"
%}
DIGIT [0-9]
INTEGER [0-9]+
LETTER [a-zA-Z]
ID ({LETTER}|_)({LETTER}|_|{DIGIT})* 

EOL (\r\n|\n)
WHITE [\t ]

STRING \"[^\"\n]*\"

SGPS \/\/.*\n
COMMENT \/\*(?:[^\*]|\*+[^\/\*])*\*+\/



%%

"true" {
    TreeNode *node = new TreeNode(NODE_BOOL);
    node->bool_val = true;
    yylval = node;
    return TRUE;
}
"false" {
    TreeNode *node = new TreeNode(NODE_BOOL);
    node->bool_val = false;
    yylval = node;
    return FALSE;
}

"int" return INT;
"void" return VOID;
"char" return CHAR;

"if" return IF;
"while" return WHILE;
"else" return ELSE;
"for"  return FOR;

"printf" return PRINTF;
"scanf" return SCANF;

"=" return ASSIGN;
"+=" return ASSIGN;
"-=" return ASSIGN;

"-" return MINUS;
"+" return ADD;
"%" return MOD;
"++" return AONE;

"&&" return AND;
"||" return OR;
"!=" return NE;
"<"	return LT;
"<=" return LE;
">"	return GT;
">=" return GE;





"!" return NOT;
"==" return EQUAL;

"&" return REFER;
"," return COMMA;
";" return SEMICOLON;
"(" return LPAREN;
")" return RPAREN;
"{" return LBRACE;
"}" return RBRACE;

{INTEGER} {
    TreeNode *node = new TreeNode(NODE_CONST);
    node->int_val = string(yytext);
    yylval = node;
    return INTEGER;
}
{ID} {
    TreeNode *node = new TreeNode(NODE_VAR);
    node->var_name = string(yytext);
    yylval = node;
    return ID;
}
{EOL}
{WHITE}
{SGPS} {
    TreeNode *node = new TreeNode(NODE_COMM);
    node->comm = string(yytext);
    yylval = node;
    return COMMENT;
}
{COMMENT} {
    TreeNode *node = new TreeNode(NODE_COMM);
    node->comm = string(yytext);
    yylval = node;
    return COMMENT;
}


{STRING}   {
    TreeNode *node = new TreeNode(NODE_STRING);
    node->comm = string(yytext);
    yylval = node;
    return STRING;
}
%%
