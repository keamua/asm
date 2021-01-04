%{
    #include "common.h"
    extern TreeNode * root;
    int yylex();
    int yyerror( char const * );
    extern int lineno;
    extern symbol_table SymbolsTable;
%}
%defines

%start program

%token ID INTEGER ConstStr ConstChar COMMENT
%token IF ELSE WHILE FOR MAIN RETURN 
%token INT VOID CHAR STRING BOOL CONST
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMICOLON COMMA DOT COLON POT
%token TRUE FALSE
%token ASSIGN ADDE SUBE MULE
%token ADD SUB MUL DIV MOD
%token EQUAL NE GT GE LT LE
%token AND OR NOT
%token PRINTF SCANF

%right COMMA
%right ASSIGN ADDE SUBE MULE
%left AND OR
%left EQUAL NE
%left GT GE LT LE
%left ADD SUB
%left MUL DIV MOD
%left SADD SSUB
%right NOT NEG QUOTE POS
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE 
%%
program
    : statements {
        root = new TreeNode(NO_NODE);
//        root->stmtType = COMP_STMT;
        root->info = "root";
        root->addChild($1);
    }
    ;
statements
    : statement {
        $$ = $1;
    }
    | statements statement{
        $$ = $1;
        $$->addSibling($2);
    }
//    | statement COMMENT{
//        $$ = $1;
//    }    
    ;
statement
    : var_declare {
        $$ = $1;
    }
//    | func_declare {
//        $$ = $1;
//    }    
    | instruction {
        $$ = $1;
    }
    | if_else {
        $$ = $1;
    }
    | while {
        $$ = $1;
    }
    | for {
        $$ = $1;
    }
    | COMMENT {
        $$ = $1;
    }    //多行注释
    | type MAIN LPAREN RPAREN compound_stmt {
//        TreeNode *node=new TreeNode(DECL_NODE);
//        node->declType = FUNC_DECL;
//        node->type = $1->type;
//        node->info = "func_main_declare";   
//        node->addChild($1);
//        node->addChild($5);
        $$ = $5;
    }
    ;
compound_stmt
    : LBRACE statements RBRACE {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = COMP_STMT;
        node->info = "compound_stmt";
        node->type = $2->type;
        node->addChild($2);
        $$ = node;
    }
    /*
    | LBRACE statements return SEMICOLON RBRACE {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = COMP_STMT;
        node->info = "compound_stmt";
        node->type = $3->type;
        node->addChild($2);
        node->addChild($3);
        $$ = node;
    }
    */
    ;
if_else
    : IF bool_statment compound_stmt %prec LOWER_THEN_ELSE {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = IF_STMT;
//        node->type = BOOLEAN;
        node->info = "if";
        node->addChild($2);
        node->addChild($3);
        $$ = node;
    }
    | IF bool_statment compound_stmt ELSE compound_stmt {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = IF_STMT;
        node->info = "if_else";
//        node->type = BOOLEAN;
        node->addChild($2);
        node->addChild($3);
        node->addChild($5);
        $$ = node;
    }
    | IF bool_statment compound_stmt ELSE if_else {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = IF_STMT;
        node->info = "if_else_if";
//        node->type = BOOLEAN;
        node->addChild($2);
        node->addChild($3);
        node->addChild($5);
        $$ = node;
    }
    ;    
while
    : WHILE bool_statment compound_stmt {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = WHILE_STMT;
        node->info = "while";
//        node->type = BOOLEAN;
        node->addChild($2);
        node->addChild($3);
        $$ = node;
    }
    ;
for
    : FOR LPAREN for_statment RPAREN compound_stmt {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = FOR_STMT;
        node->info = "for";
//        node->type = BOOLEAN;
        node->addChild($3);
        node->addChild($5);
        $$ = node;
    }
    ;
for_statment
    : var_declare bool_expr SEMICOLON expr {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = NO_STMT;
//        node->type = BOOLEAN;
        node->info = "for_statment";
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        $$ = node;
    }
    | expr SEMICOLON bool_expr SEMICOLON expr{
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = NO_STMT;
//        node->type = BOOLEAN;
        node->info = "for_statment";
        node->addChild($1);
        node->addChild($3);
        node->addChild($5);
        $$ = node;
    }
    ;
bool_statment
    : LPAREN bool_expr RPAREN {
        $$ = $2;
    }
    ;
var_declare
    : type id_list SEMICOLON {
        TreeNode *node = new TreeNode(DECL_NODE);
        node->declType = VAR_DECL;
        TreeNode *t = $2;
        while(t){
            if(t->exprType == OP_EXPR){
                t->children[0]->type = $1->type;
                if(SymbolsTable.lookup(t->children[0]->var_name) == -1)
                    SymbolsTable.insert(t->children[0]->var_name, t->children[0]->type);                    
//                else
//                    cerr<<"bad var_declare(repeat) at line :"<< node->line <<endl;                        
            }else{
                t->type = $1->type;
                if(SymbolsTable.lookup(t->var_name) == -1)
                    SymbolsTable.insert(t->var_name, t->type);
//                else
//                    cerr<<"bad var_declare(repeat) at line :"<< node->line <<endl;    
            }
            t = t->sibling;
        }
        node->info = "var_declare";
        node->addChild($1);
        node->addChild($2);
        $$ = node;
    }
    ;
id_list
    : id_list COMMA expr {
        $$ = $1;
        $$->addSibling($3);
    }
    | expr {
        $$ = $1;
    }
    ;
/*        
func_declare
    : type ID LPAREN param_list RPAREN SEMICOLON{
        TreeNode *node=new TreeNode(DECL_NODE);
        node->declType = FUNC_DECL;
        $2->type = $1->type;
        node->type = $1->type;
        node->info = "func_declare";
        if(SymbolsTable.lookup($2->var_name) == -1)
            SymbolsTable.insert($2->var_name, $2->type);
        else
            cerr<<"bad func_declare(repeat) at line :"<< node->line <<endl;    
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        $$ = node;
    }
    | type ID LPAREN param_list RPAREN compound_stmt {
        TreeNode *node=new TreeNode(DECL_NODE);
        node->declType = FUNC_DECL;
        $2->type = $1->type;
        node->type = $1->type;
        node->info = "func_declare";
        if(SymbolsTable.lookup($2->var_name) == -1)
            SymbolsTable.insert($2->var_name, $2->type);
        else
            cerr<<"bad func_declare(repeat) at line :"<< node->line <<endl;    
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        node->addChild($6);
        $$ = node;
    }
    ;
param_list
    : param_list COMMA type ID {
        $$ = $1;
        $4->type = $3->type;
        $$->addSibling($4);
        if(SymbolsTable.lookup($4->var_name) == -1)
            SymbolsTable.insert($4->var_name, $4->type);
        else
            cerr<<"bad func_declare's param(repeat) at line :"<< $$->line <<endl; 
    }
    | type ID {
        $$ = $2;
        $2->type = $1->type;
        if(SymbolsTable.lookup($2->var_name) == -1)
            SymbolsTable.insert($2->var_name, $2->type);
        else
            cerr<<"bad func_declare's param(repeat) at line :"<< $$->line <<endl; 
    }
    |  {
        TreeNode *node=new TreeNode(EXPR_NODE);
        node->exprType = NULL_EXPR; 

        $$ = node;
    }
    ;
*/    
instruction
    : expr SEMICOLON {
        $$ = $1;  
    }    
    | printf SEMICOLON {
        $$ = $1;
    }
    | scanf SEMICOLON {
        $$ = $1;
    }
    | return SEMICOLON {
        $$ = $1;
    }
    ;
printf
    : PRINTF LPAREN expr RPAREN {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = PRINTF_STMT;
        node->info = "printf";
        node->addChild($3);
        $$ = node;
    }
    ;
scanf
    : SCANF LPAREN expr RPAREN {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = SCANF_STMT;
        node->info = "scanf";
        node->addChild($3);
        $$ = node;
    }
    ;
return
    : RETURN expr {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = RETURN_STMT;
        node->type = $2->type;
        node->info = "return_expr";
        node->addChild($2);
        $$ = node;
    }
    ;    
bool_expr
    : TRUE {
        $$ = $1;
    }
    | FALSE {
        $$ = $1;
    }
    | expr {
        $$ = $1;
    }
    | expr EQUAL expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = EQUAL_OP;
        node->info = "==";
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | expr GT expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = GT_OP;
        node->info = ">";
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | expr GE expr {
        TreeNode *node=new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = GE_OP;
        node->info = ">=";
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | expr LT expr {
        TreeNode *node=new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->info = "<";
        node->opType = LT_OP;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr LE expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = LE_OP;
        node->info = "<=";
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | expr NE expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = NE_OP;
        node->info = "!=";
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | NOT bool_expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->info = "!";
        node->opType = NOT_OP;
        node->addChild($2);
        $$ = node;        
    }
    | bool_expr AND bool_expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->info = "&&";
        node->opType = AND_OP;
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | bool_expr OR bool_expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->info = "||";
        node->opType = OR_OP;
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | LPAREN bool_expr RPAREN{
        $$ = $2;
    }
    |  {
        TreeNode *node=new TreeNode(EXPR_NODE);
        node->exprType = NULL_EXPR; 
        $$ = node;
    }
    ;
expr
    : ID {
        $$ = $1;
    }
    | INTEGER {
        $$ = $1;
    }
    | ConstStr {
        $$ = $1;
    }
    | ConstChar {
        $$ = $1;
    }
    | ConstStr COMMA id_list {
        TreeNode *node = new TreeNode(STMT_NODE);
        node->stmtType = INOUT_STMT;
        node->exprType = OP_EXPR;
        node->declType = FUNC_DECL;
        /*
        TreeNode *t = $3;
        while(t){
            if(t->exprType == OP_EXPR){
                if(SymbolsTable.lookup(t->children[0]->var_name) == -1)
                    cerr<<"bad id(not declaim) at line :"<< t->line <<endl;       
            }else{
                if(SymbolsTable.lookup(t->var_name) == -1)
                    cerr<<"bad id(not declaim) at line :"<< t->line <<endl;
            }
            t = t->sibling;
        }
        */
        node->addChild($1);
        node->addChild($3);
        $$ = node;
    }
    | expr ADD expr {
        TreeNode *node=new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = ADD_OP;
        node->info = "+";
        node->addChild($1);
        node->addChild($3);
        $$ = node;   
    }
    | expr SUB expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = SUB_OP;
        node->info = "-";
        node->addChild($1);
        node->addChild($3);
        $$ = node;   
    }
    | expr MUL expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = MUL_OP;
        node->info = "*";
        node->addChild($1);
        node->addChild($3);
        $$ = node;   
    }
    | expr DIV expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = DIV_OP;
        node->info = "/";
        node->addChild($1);
        node->addChild($3);
        $$ = node;   
    }
    | expr MOD expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = MOD_OP;
        node->info = "%";
        node->addChild($1);
        node->addChild($3);
        $$ = node;   
    }
    | ADD expr %prec POS {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = POS_OP;
        node->info = "+";
        node->addChild($2);
        $$ = node;
    }
    | SUB expr %prec NEG {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = NEG_OP;
        node->info = "-";
        node->addChild($2);
        $$ = node;
    }
    | QUOTE ID {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = QUOTE_OP;
        node->info = "&";
        node->addChild($2);
//        if(SymbolsTable.lookup($2->var_name) == -1)
//            cerr<<"bad id(not declaim) at line :"<< node->line <<endl;
        $$ = node;
    }
    | ID SADD {
        TreeNode *node=new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = SADD_OP;
        node->info = "++";
        node->addChild($1);
//        if(SymbolsTable.lookup($1->var_name) == -1)
//            cerr<<"bad id(not declaim) at line :"<< node->line <<endl;
        $$ = node;
    }
    | ID SSUB {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = SSUB_OP;
        node->info = "--";
        node->addChild($1);
//        if(SymbolsTable.lookup($1->var_name) == -1)
//            cerr<<"bad id(not declaim) at line :"<< node->line <<endl;
        $$ = node;
    }
    | ID ASSIGN expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = ASSIGN_OP;
        node->info = "=";
        node->addChild($1);
        node->addChild($3);
//        if(SymbolsTable.lookup($1->var_name) == -1)
//            cerr<<"bad id(not declaim) at line :"<< node->line <<endl;
        $$=node;
    }
    | ID ADDE expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = ADDE_OP;
        node->info = "+=";
        node->addChild($1);
        node->addChild($3);
//        if(SymbolsTable.lookup($1->var_name) == -1)
//            cerr<<"bad id(not declaim) at line :"<< node->line <<endl;
        $$ = node;  
    }
    | ID MULE expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = MULE_OP;
        node->info = "*=";
        node->addChild($1);
        node->addChild($3);
//        if(SymbolsTable.lookup($1->var_name) == -1)
//            cerr<<"bad id(not declaim) at line :"<< node->line <<endl;
        $$ = node;  
    }
    | ID SUBE expr {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = OP_EXPR;
        node->opType = SUBE_OP;
        node->info = "-=";
        node->addChild($1);
        node->addChild($3);
//        if(SymbolsTable.lookup($1->var_name) == -1)
//            cerr<<"bad id(not declaim) at line :"<< node->line <<endl;
        $$ = node;  
    }
    | LPAREN expr RPAREN{
        $$ = $2;
    }
    | /* empty */ {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = NULL_EXPR;
        node->type = VOIDt;
        $$ = node;
    }
    ;
type
    : INT {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = TYPE_EXPR;
        node->info = "int";
        node->type = INTEGERt;
        $$=node;
    }
    | VOID {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = TYPE_EXPR;
        node->info = "void";
        node->type = VOIDt;
        $$=node;         
    }
    | CHAR {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = TYPE_EXPR;
        node->info = "char";
        node->type = CHARACTOR;
        $$=node;   
    }
    | BOOL {
        TreeNode *node = new TreeNode(EXPR_NODE);
        node->exprType = TYPE_EXPR;
        node->info = "bool";
        node->type = BOOLEAN;
        $$=node;   
    }
    ;

%%