#include "tree.h"

extern int lineno;

int NodeNum = 0;
int tempNum = 0;
int strNum  = 0;
bool decl_flag = false;
string str_table[16];

int Zero = 0;

TreeNode::TreeNode(NodeType nodeType) {
    nodeID = 0;
    tempID = 0;
    strID = 0;
    line = lineno;

    type = NOTYPE;       
    info = "";
    label.begin_label = label.false_label = label.next_label =
        label.true_label = "";

    this->nodeType = nodeType;
    stmtType = NO_STMT;    
    exprType = NO_EXPR;
    declType = NO_DECL;
    opType = NO_OP;

    int_val = 0;
    bool_val = false;
    str_val = "";
    char_val = "";
    var_name = "";

    for(int i = 0; i < MAXCHILDREN; i++) {
        children[i] = nullptr;
    }
//    sibling = nullptr;
    
}

void TreeNode::addChild(TreeNode *t) {
    int i = 0;
    while (children[i]){
        i++;        
    }
    children[i] = t;
}

void TreeNode::addSibling(TreeNode *t) {
    if(sibling) {
        TreeNode *p = sibling;
        while(p->sibling) {
            p = p->sibling;
        }
        p->sibling = t;
    } 
    else {
        sibling = t;
    }
}

void TreeNode::genNodeId() {
    nodeID = NodeNum++;
    int i = 0;
    while(children[i]) {
        children[i]->genNodeId();
        i++;
    }
    if(sibling)
        sibling->genNodeId();
}

void TreeNode::printNodeInfo() {
    cout<<"nodeID:"<<nodeID<<"    lineno:"<<line<<"    node_type： "<<nttoa(nodeType).c_str();
    switch(nodeType) {
        case STMT_NODE: cout<<"    StmtType:"<<sttoa(stmtType).c_str()<<endl;   break;
        case EXPR_NODE: cout<<"    ExprType:"<<eptoa(exprType).c_str()<<endl;   break;
        case DECL_NODE: cout<<"    DeclType:"<<dltoa(declType).c_str()<<endl;   break;
        case NO_NODE:   cout<<"    No node Type."<<endl;                        break;
        default:    break;
    }
}

void TreeNode::printAST() {
    //printNodeInfo();
    //printNodeConnection();
    //cout<<endl;
    if(exprType == OP_EXPR) {
        if(opType == ASSIGN_OP) {
            tempID = tempNum;
            tempNum++;
        } 
        else {
            if(children[0]->exprType == OP_EXPR) {
                children[0]->tempID = tempNum - 1;
                tempNum++;
            }
            if(children[1] != NULL && children[1]->exprType == OP_EXPR) {
                children[1]->tempID = tempNum - 1;
                tempNum++;
            }
        }
    }
    if(exprType == CONST_EXPR && (type == CHARACTOR || type == STRINGt)) {
        str_table[strNum] = str_val;
        strID = strNum;
        strNum++;
    }
    if(nodeType == DECL_NODE)
        decl_flag = true;
    int i = 0;
    while(children[i]) {
        children[i]->printAST();
        i++;
    }
    type_check();
    if(sibling)
        sibling->printAST();
}

void TreeNode::printNodeConnection() {
    if(!children[0] && !sibling)
        cout<<"This node has no connection!";
    else if (children[0]){
        cout<<"children: ";
        for(int i = 0; children[i]; i++) {
            cout<<"@"<<children[i]->nodeID<<" ";
            TreeNode *p = children[i]->sibling;
            while(p) {
                cout<<"@"<<p->nodeID<<" ";
                p = p->sibling;
            }
        }
    }
    else if(sibling){
        cout<<"    sibling: ";
        TreeNode *p = sibling;
        while (p)
        {
            cout<<"@"<<p->nodeID<<" ";
            p = p->sibling;
        }
    }
    cout<<endl;
}

void TreeNode::type_check() {
    //int i = 0;
    //TreeNode *t = children[1];
    //TreeNode *p = children[1];
    switch(nodeType) {
    case STMT_NODE:
        switch(stmtType) {
        case IF_STMT:
            if(children[0]->type != BOOLEAN) {
                cerr << "Bad boolean type at line: " << line << endl;
            }
            break;
        case WHILE_STMT:
            if(children[0]->type != BOOLEAN) {
                cerr << "Bad boolean type at line: " << line << endl;
            }
            break;
        case FOR_STMT:
            if(children[0]->children[1]->type != BOOLEAN && children[0]->children[1]->exprType != NULL_EXPR) {
                cerr << "Bad boolean type at line: " << line << endl;
            }
            break;
        case ASSIGN_STMT:
            if (children[0]->type != children[1]->type)
            {
                cerr << "Bad assign at line: " << line << endl;
            }
            break;
            /*           
        case INOUT_STMT:
            if (children[1]){
                if (children[0]->type != STRINGt || children[1]->type != VAR){
                    cerr << "Bad printf at line: " << line << endl;
                }
            }            
            else if (children[0]->type != STRINGt){
                cerr << "Bad printf at line: " << line << endl;
            }
            break; 
            */
        default:
            break;
        }
        break;
    case EXPR_NODE:
        switch (exprType){
        case OP_EXPR:
            switch(opType) {
            case ADD_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad add at line: " << line << endl;
                type = children[0]->type;
                break;
            case SUB_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad sub at line: " << line << endl;
                type = children[0]->type;
                break;
            case MUL_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad mul at line: " << line << endl;
                type = children[0]->type;
                break;
            case DIV_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad div at line: " << line << endl;
                type = children[0]->type;
                break;
            case MOD_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad mod at line: " << line << endl;
                type = children[0]->type;
                break;
            case SADD_OP:
                if(children[0]->type != INTEGERt)
                    cerr << "bad ++ at line: " << line << endl;
                type = children[0]->type;
                break;
            case SSUB_OP:
                if(children[0]->type != INTEGERt)
                    cerr << "bad -- at line: " << line << endl;
                type = children[0]->type;
                break;
            case POS_OP:
                if(children[0]->type != INTEGERt)
                    cerr << "bad + at line: " << line << endl;
                type = children[0]->type;
                break;
            case NEG_OP:
                if(children[0]->type != INTEGERt)
                    cerr << "bad - at line: " << line << endl;
                type = children[0]->type;
                break;        
            case ASSIGN_OP:
                if(children[0]->type != children[1]->type)
                    cerr << "bad = at line: " << line << endl;
                type = children[0]->type;
                break;
            case ADDE_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad += at line: " << line << endl;
                type = children[0]->type;
                break;
            case MULE_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad *= at line: " << line << endl;
                type = children[0]->type;
                break;
            case SUBE_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad -= at line: " << line << endl;
                type = children[0]->type;
                break;
            case EQUAL_OP:
                if(children[0]->type != children[1]->type)
                    cerr << "bad == at line: " << line << endl;
                type = BOOLEAN;
                break;
            case GT_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad > at line: " << line << endl;
                type = BOOLEAN;
                break;
            case GE_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad >= at line: " << line << endl;
                type = BOOLEAN;
                break;
            case LT_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad < at line: " << line << endl;
                type = BOOLEAN;
                break;
            case LE_OP:
                if(children[0]->type != children[1]->type ||
                   children[0]->type != INTEGERt)
                    cerr << "bad <= at line: " << line << endl;
                type = BOOLEAN;
                break;
            case NE_OP:
                if(children[0]->type != children[1]->type)
                    cerr << "bad != at line: " << line << endl;
                type = BOOLEAN;
                break;
            case NOT_OP:
                if(children[0]->type != BOOLEAN)
                    cerr << "bad ! at line: " << line << endl;
                type = BOOLEAN;
                break;
            case AND_OP:
                if(children[0]->type != BOOLEAN)
                    cerr << "bad && at line: " << line << endl;
                type = BOOLEAN;
                break;
            case OR_OP:
                if(children[0]->type != BOOLEAN)
                    cerr << "bad || at line: " << line << endl;
                type = BOOLEAN;
                break;    
            default:
                break;
            }
            break;

        case ID_EXPR:{
            int i = SymbolsTable.lookup(var_name);
            if(decl_flag) {
                type = (Type)SymbolsTable.table[i].type;
            } 
            else {
                // 对于其他ID节点，如果没有在符号表中找到或者没有被声明(即defined为false)，则说明其未被声明，或其在声明之前使用
                if(i != -1 && SymbolsTable.table[i].defined)
                    type = (Type)SymbolsTable.table[i].type;
                else
                    cerr << "variable not define at line: " << line << endl;
            }
            }
            break;            
        default:
            break;
        }        
        break;
    case DECL_NODE:
        switch (declType){
            case VAR_DECL:{
                TreeNode *t = children[1];
                while(t){
                    int i = 0;
                    if(t->exprType == OP_EXPR) { 
                        if((i = SymbolsTable.lookup(t->children[0]->var_name)) != -1) {
                            if(!SymbolsTable.table[i].defined)
                                SymbolsTable.table[i].defined = true;
                            else
                                cerr << "variable repeat define at line: " << line << endl;
                        }
                    } 
                    else { 
                        if((i = SymbolsTable.lookup(t->var_name)) != -1) {
                            if(!SymbolsTable.table[i].defined)
                                SymbolsTable.table[i].defined = true;
                            else
                                cerr << "variable repeat define at line: " << line << endl;
                            }
                    }
                    t = t->sibling;
                }
                decl_flag = false;
                }
                break;    
            case FUNC_DECL:
                if(children[0]->type != children[3]->type)
                    cerr << "bad return at line: " << line << endl;
                type = children[0]->type;
                break;   
            default:
                break;
        }    
    default:
        break;        
    }

}

string TreeNode::nttoa(NodeType type) {
    switch(type) {
        case DECL_NODE:         return "DECL_NODE";         break;
        case EXPR_NODE:         return "EXPR_NODE";         break;
        case STMT_NODE:         return "STMT_NODE";         break;
        case NO_NODE:           return "NO_NODE";           break;    
        default:                return "";                  break;
    }
}
string TreeNode::sttoa(StmtType type) {
    switch(type) {
        case IF_STMT:           return "if";                break;
        case WHILE_STMT:        return "while";             break;
        case ASSIGN_STMT:       return "assign";            break;
        case PRINTF_STMT:       return "print";             break;
        case SCANF_STMT:        return "scanf";             break;
        case FOR_STMT:          return "for";               break;
        case RETURN_STMT:       return "return";            break;
        case COMP_STMT:         return "compound_stmt";     break;
        case INOUT_STMT:        return "inout_stmt";        break;        
        default:                return "";                  break;
    }
}
string TreeNode::dltoa(DeclType type) {
    switch(type) {
        case VAR_DECL:          return "var_decl";          break;
        case FUNC_DECL:         return "func_decl";         break;
        default:                return "";                  break;
    }
}
string TreeNode::eptoa(ExprType type) {
    switch(type) {
        case OP_EXPR:           return "op_expr";           break;
        case ID_EXPR:           return "id_expr";           break;
        case TYPE_EXPR:         return "type_expr";         break;
        case PARAM_EXPR:        return "param_expr";        break;
        case CONST_EXPR:        return "const_expr";        break;
        case NULL_EXPR:         return "null_expr";         break;
        default:                return "";                  break;
    }
}
string TreeNode::optoa(OpType type) {
    switch(type) {
        case EQUAL_OP:          return "==";                break;
        case GT_OP:             return ">";                 break;
        case GE_OP:             return ">=";                break;
        case LT_OP:             return "<";                 break;
        case LE_OP:             return "<=";                break;
        case NE_OP:             return "!=";                break;
        case AND_OP:            return "&&";                break;
        case OR_OP:             return "||";                break;
        case NOT_OP:            return "!";                 break;
        case ADD_OP:            return "+";                 break;
        case SUB_OP:            return "-";                 break;
        case POS_OP:            return "+";                 break;    
        case NEG_OP:            return "-";                 break;
        case MUL_OP:            return "*";                 break;
        case DIV_OP:            return "/";                 break;
        case MOD_OP:            return "%";                 break;
        case SADD_OP:           return "++";                break;
        case SSUB_OP:           return "--";                break;
        case ASSIGN_OP:         return "=";                 break;
        case ADDE_OP:           return "+=";                break;
        case SUBE_OP:           return "-=";                break;
        case MULE_OP:           return "*=";                break;
        default:                return "";                  break;
    }
}
string TreeNode::tytoa(Type type) {
    switch(type) {
        case INTEGERt:           return "int";               break;
        case VOIDt:              return "void";              break;
        case BOOLEAN:           return "bool";              break;
        case CHARACTOR:         return "char";              break;
        case STRINGt:            return "string";            break;
        case VAR:               return "variable";          break;
        case CONSTt:             return "constant";          break;        
        case NOTYPE:            return "notype";            break;
        default:                return "";                  break;
    }
}

void tree::get_temp_var(TreeNode *t) {
    if(t->nodeType != EXPR_NODE)
        return;
    if(t->opType < ADD_OP || t->opType > NO_OP)
        return;
    t->tempID = temp_var_seq;
    tree::temp_var_seq++;
}



string tree::new_label(void) {
    char tmp[20];
    sprintf(tmp, "label%d", label_seq);
    label_seq++;
    return tmp;
}

void tree::stmt_get_label(TreeNode *t) {
    TreeNode *e = t->children[0];
    TreeNode *s1 = t->children[1];
    TreeNode *s2 = t->children[2];
    TreeNode *s3 = t->children[3];
    switch(t->stmtType) {
    case COMP_STMT:
        TreeNode *last;
        TreeNode *p;
        for(p = t->children[0];
            p->nodeType == DECL_NODE;
            p = p->sibling)
            ;

        for(; p; p = p->sibling) {
            last = p;
            recursive_get_label(p);
        }

        t->label.next_label = last->label.next_label;
        if(t->sibling)
            t->sibling->label.begin_label = t->label.next_label;
        break;
    case WHILE_STMT:
        if(t->label.begin_label == "")
            t->label.begin_label = new_label();
        s1->label.next_label = t->label.begin_label;

        s1->label.begin_label = e->label.true_label = new_label();

        if(t->label.next_label == "")
            t->label.next_label = new_label();
        e->label.false_label = t->label.next_label;
        if(t->sibling)
            t->sibling->label.begin_label = t->label.next_label;

        recursive_get_label(e);
        recursive_get_label(s1);
        break;
    case FOR_STMT:
        s3 = s1;
        s1 = e->children[1];
        s2 = e->children[2];
        e = e->children[0];
        if(t->label.begin_label == "")
            t->label.begin_label = new_label();
        s1->label.true_label = new_label();
        if(t->label.next_label == "")
            t->label.next_label = new_label();
        s3->label.next_label = t->label.begin_label;
        s3->label.begin_label = s1->label.true_label;
        s1->label.false_label = t->label.next_label;
        if(t->sibling)
            t->sibling->label.begin_label = t->label.next_label;

        recursive_get_label(s1);
        recursive_get_label(s3);
        break;
    case IF_STMT:
        if(s2 == NULL) { // if(e) s1
            if(e->label.true_label == "")
                e->label.true_label = new_label();
            if(t->label.next_label == "")
                t->label.next_label = new_label();
            e->label.false_label = t->label.next_label;
            s1->label.begin_label = e->label.true_label;
            s1->label.next_label = t->label.next_label;
        } else { // if(e) s1  else s2
            if(e->label.true_label == "")
                e->label.true_label = new_label();
            if(t->label.next_label == "")
                t->label.next_label = new_label();
            e->label.false_label = new_label();
            s1->label.begin_label = e->label.true_label;
            s1->label.next_label = t->label.next_label;
            s2->label.begin_label = e->label.false_label;
            s2->label.next_label = t->label.next_label;
        }
        recursive_get_label(e);
        recursive_get_label(s1);
        if(s2)
            recursive_get_label(s2);
        break;
    }
}

void tree::expr_get_label(TreeNode *t) {
    if(t->type != BOOLEAN)
        return;
    TreeNode *e1 = t->children[0];
    TreeNode *e2 = t->children[1];
    switch(t->opType) {
    case AND_OP:
        if(e1->label.true_label == "")
            e1->label.true_label = new_label();
        if(t->label.true_label == "")
            t->label.true_label = new_label();
        if(t->label.false_label == "")
            t->label.false_label = new_label();
        e2->label.true_label = t->label.true_label;
        e1->label.false_label = e2->label.false_label = t->label.false_label;
        break;
    case OR_OP:
        if(t->label.true_label == "")
            t->label.true_label = new_label();
        if(t->label.false_label == "")
            t->label.false_label = new_label();
        e1->label.false_label = new_label();
        e2->label.false_label = t->label.false_label;
        e1->label.true_label = e2->label.true_label = t->label.true_label;
        break;
    case NOT_OP:
        if(t->label.true_label == "")
            t->label.true_label = new_label();
        if(t->label.false_label == "")
            t->label.false_label = new_label();
        e1->label.false_label = t->label.true_label;
        e1->label.true_label = t->label.false_label;
        break;
    }
    if(e1)
        recursive_get_label(e1);
    if(e2)
        recursive_get_label(e2);
}

void tree::recursive_get_label(TreeNode *t) {
    if(t != NULL) {
        if(t->nodeType == STMT_NODE|| t->nodeType == DECL_NODE)
            stmt_get_label(t);
        else if(t->exprType == OP_EXPR )
            expr_get_label(t);
    }
}

void tree::get_label(void) {
    TreeNode *p = root->children[0];
    for(; p->nodeType == DECL_NODE; p = p->sibling)
        ;
    p->label.begin_label = "main";
    while(p) {
        recursive_get_label(p);
        p = p->sibling;
    }
}

void tree::stmt_gen_code(TreeNode *t) {
    int i = 0;
    int j = 0;
    int k = 0;
    TreeNode *e = t->children[0];
    TreeNode *s1 = t->children[1];
    TreeNode *s2 = t->children[2];
    TreeNode *s3 = t->children[3];
    switch(t->stmtType) {
    case IF_STMT:
        if(s2 == NULL) {
            recursive_gen_code(e);
            cout << e->label.true_label << ":" << endl;
            recursive_gen_code(s1);
        } else {
            recursive_gen_code(e);
            cout << e->label.true_label << ":" << endl;
            recursive_gen_code(s1);
            cout << "\tJMP " << t->label.next_label << endl;
            cout << e->label.false_label << ":" << endl;
            recursive_gen_code(s2);
        }
        if(t->label.next_label != "")
            cout << t->label.next_label << ":" << endl;
        break;
    case WHILE_STMT:
        if(t->label.begin_label != "")
            cout << t->label.begin_label << ":" << endl;
        recursive_gen_code(e);
        cout << e->label.true_label << ":" << endl;
        recursive_gen_code(s1);
        cout << "\tJMP " << t->label.begin_label << endl;
        if(t->label.next_label != "")
            cout << t->label.next_label << ":" << endl;
        break;
    case FOR_STMT:
        s3 = s1;
        s1 = e->children[1];
        s2 = e->children[2];
        e = e->children[0];
        recursive_gen_code(e);
        cout << t->label.begin_label << ":" << endl;
        recursive_gen_code(s1);
        cout << s1->label.true_label << ":" << endl;
        recursive_gen_code(s3);
        recursive_gen_code(s2);
        cout << "\tJMP " << t->label.begin_label << endl;

        if(t->label.next_label != "")
            cout << t->label.next_label << ":" << endl;
        break;
    case COMP_STMT:
        for(TreeNode *p = t->children[0]; p; p = p->sibling)
            recursive_gen_code(p);
        cout << endl;
        break;
    case SCANF_STMT:
        s1 = e->children[1];
        while(s1) {
            i++;
            s1 = s1->sibling;
        }
        for(j = i; j > 0; j--) {
            k = j - 1;
            s2 = e->children[1];
            for(k; k > 0; k--) {
                s2 = s2->sibling;
            }
            cout << "\tpushl $_" << s2->children[0]->var_name << endl;
        }
        cout << "\tpushl $STR" << e->children[0]->strID << endl;
        cout << "\tcall scanf" << endl;
        cout << "\taddl $" << 4 * i + 4 << ", %esp" << endl;
        break;
    case PRINTF_STMT:
        if(e->declType == FUNC_DECL) { // printf("", n)
            s1 = e->children[1];
            while(s1) {
                i++;
                s1 = s1->sibling;
            }
            for(j = i; j > 0; j--) {
                k = j - 1;
                s2 = e->children[1];
                for(k; k > 0; k--) {
                    s2 = s2->sibling;
                }
                if(s2->exprType == OP_EXPR)
                    cout << "\tpushl t" << s2->tempID << endl;
                else
                    cout << "\tpushl _" << s2->var_name << endl;
            }
            cout << "\tpushl $STR" << e->children[0]->strID << endl;
            cout << "\tcall printf" << endl;
            cout << "\taddl $" << 4 * i + 4 << ", %esp" << endl;
        } else {
            cout << "\tpushl $STR" << e->strID << endl;
            cout << "\tcall printf" << endl;
            cout << "\taddl $" << 4 << ", %esp" << endl;
        }
        break;
    default:
        break;
    }
    if (t->nodeType == DECL_NODE){
        while(s1) {
            if(s1->exprType == OP_EXPR)
                recursive_gen_code(s1);
            s1 = s1->sibling;
        }
    }    
}

void tree::expr_gen_code(TreeNode *t) {
    if(t->opType != AND_OP && t->opType != OR_OP && t->opType != NOT_OP)
        for(int i = 0; i < MAXCHILDREN, t->children[i] != NULL; i++)
            recursive_gen_code(t->children[i]);
    TreeNode *e1 = t->children[0];
    TreeNode *e2 = t->children[1];
    switch(t->opType) {
    case ASSIGN_OP:
        cout << "\tmovl ";
        if(e2->exprType == ID_EXPR) {
            cout << "_" << e2->var_name;
        } else if(e2->exprType == CONST_EXPR) {
            if(e2->type == INTEGERt)
                cout << "$" << e2->int_val;
            else if(e2->type == CHARACTOR)
                cout << "STR" << e2->strID;
        } else {
            cout << "t" << e2->tempID;
        }
        cout << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        cout << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case ADDE_OP:
        cout << "\tmovl _" << e1->var_name << ", %eax" << endl;
        cout << "\taddl ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        cout << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case SUBE_OP:
        cout << "\tmovl _" << e1->var_name << ", %eax" << endl;
        cout << "\tsubl ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        cout << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case ADD_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\taddl ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        break;
    case SUB_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tsubl ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        break;
    case MUL_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\timull ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        break;
    case DIV_OP:
        cout << "\tXOR %edx, %edx" << endl;
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp %edx, %eax" << endl; // 除0
        cout << "\tjge O" << Zero << endl;
        cout << "\tmovl $0, %edx" << endl;
        cout << "\tnot %edx" << endl;
        cout << "O" << Zero << ":" << endl;
        Zero++;
        cout << "\tmovl ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %ebx" << endl;

        cout << "\tidiv %ebx" << endl;
        cout << "\tXOR %edx, %edx" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        break;
    case MOD_OP:
        cout << "\tXOR %edx, %edx" << endl;
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp %edx, %eax" << endl; // 除0
        cout << "\tjge O" << Zero << endl;
        cout << "\tmovl $0, %edx" << endl;
        cout << "\tnot %edx" << endl;
        cout << "O" << Zero << ":" << endl;
        Zero++;
        cout << "\tmovl ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %ebx" << endl;

        cout << "\tidiv %ebx" << endl;
        cout << "\tXOR %eax, %eax" << endl;
        cout << "\tmovl %edx, t" << t->tempID << endl;
        break;
    case POS_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        break;
    case NEG_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\timul $-1, %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        break;
    case SADD_OP:
        cout << "\tmovl _" << e1->var_name << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        cout << "\taddl $1, %eax" << endl;
        cout << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case SSUB_OP:
        cout << "\tmovl _" << e1->var_name << ", %eax" << endl;
        cout << "\tmovl %eax, t" << t->tempID << endl;
        cout << "\tsubl $1, %eax" << endl;
        cout << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case EQUAL_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR) {
            if(e1->type == INTEGERt)
                cout << "$" << e1->int_val;
            else if(e1->type == CHARACTOR)
                cout << "STR" << e1->strID;
        } else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR) {
            if(e2->type == INTEGERt)
                cout << "$" << e2->int_val;
            else if(e2->type == CHARACTOR)
                cout << "STR" << e2->strID;
        } else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tje " << t->label.true_label << endl;
        cout << "\tjmp " << t->label.false_label << endl;
        break;
    case NE_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR) {
            if(e1->type == INTEGERt)
                cout << "$" << e1->int_val;
            else if(e1->type == CHARACTOR)
                cout << "STR" << e1->strID;
        } else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR) {
            if(e2->type == INTEGERt)
                cout << "$" << e2->int_val;
            else if(e2->type == CHARACTOR)
                cout << "STR" << e2->strID;
        } else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tjne " << t->label.true_label << endl;
        cout << "\tjmp " << t->label.false_label << endl;
        break;
    case GT_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR && e1->type == INTEGERt)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR && e2->type == INTEGERt)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tjg " << t->label.true_label << endl;
        cout << "\tjmp " << t->label.false_label << endl;
        break;
    case GE_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR && e1->type == INTEGERt)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR && e2->type == INTEGERt)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tjge " << t->label.true_label << endl;
        cout << "\tjmp " << t->label.false_label << endl;
        break;
    case LT_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR && e1->type == INTEGERt)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR && e2->type == INTEGERt)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tjl " << t->label.true_label << endl;
        cout << "\tjmp " << t->label.false_label << endl;
        break;
    case LE_OP:
        cout << "\tmovl ";
        if(e1->exprType == ID_EXPR)
            cout << "_" << e1->var_name;
        else if(e1->exprType == CONST_EXPR && e1->type == INTEGERt)
            cout << "$" << e1->int_val;
        else
            cout << "t" << e1->tempID;
        cout << ", %eax" << endl;
        cout << "\tcmp ";
        if(e2->exprType == ID_EXPR)
            cout << "_" << e2->var_name;
        else if(e2->exprType == CONST_EXPR && e2->type == INTEGERt)
            cout << "$" << e2->int_val;
        else
            cout << "t" << e2->tempID;
        cout << ", %eax" << endl;
        cout << "\tjle " << t->label.true_label << endl;
        cout << "\tjmp " << t->label.false_label << endl;
        break;
    case AND_OP:
        recursive_gen_code(e1);
        cout << e1->label.true_label << ":" << endl;
        recursive_gen_code(e2);
        break;
    case OR_OP:
        recursive_gen_code(e1);
        cout << e1->label.false_label << ":" << endl;
        recursive_gen_code(e2);
        break;
    case NOT_OP:
        recursive_gen_code(e1);
        break;
    }
}

void tree::recursive_gen_code( TreeNode *t) {
    if(t != NULL) {
        if(t->nodeType == STMT_NODE || t->nodeType == DECL_NODE)
            stmt_gen_code(t);
        else if(t->exprType == OP_EXPR)
            expr_gen_code(t);
    }
}
//对声明语句的输出：type list； list -> expr | list , expr , expr -> id | id = expr
void tree::gen_decl(TreeNode *t) {
    for(TreeNode *p = t->children[1]; p; p = p->sibling) {
        if(p->exprType == ID_EXPR && p->type == INTEGERt) {
            cout << "_" << p->var_name << ":" << endl;
        } else if(p->exprType == ID_EXPR && p->type == CHARACTOR) {
            cout << "_" << p->var_name << ":" << endl;
        } else if(p->exprType == OP_EXPR && p->opType == ASSIGN_OP) {
            cout << "_" << p->children[0]->var_name << ":" << endl;
        }
        cout << "\t.zero\t4" << endl;
        cout << "\t.align\t4" << endl;
    }
}

void tree::gen_header(){
    cout<<"\t.386\n";
    cout<<"\t.model flat, stdcall\n";
    cout<<"\toption casemap :none\n";
    cout<<"\tinclude c:\\masm32\\include\\msvcrt.inc\n";
    cout<<"\tincludelib c:\\masm32\\lib\\msvcrt.lib\n";

}

void tree::gen_code() {
    //gen_header();
    cout << "\t.section .bss" << endl;
    TreeNode *p = root->children[0];
    while(p) {
        if(p->nodeType == STMT_NODE && p->stmtType == COMP_STMT)
            p = p->children[0];   
        if(p->nodeType == DECL_NODE && p->declType == VAR_DECL)
            gen_decl(p);
        p = p->sibling;
    }

    for(int i = 0; i < tempNum; i++) {
        cout << "t" << i << ":" << endl;
        cout << "\t.zero\t4" << endl;
        cout << "\t.align\t4" << endl;
    }
    cout << endl;

    cout << "\t.section .rodata" << endl;
    for(int i = 0; i < strNum; i++) {
        cout << "STR" << i << ":" << endl;
        cout << "\t.string\t" << str_table[i] << "" << endl;
        cout << "\t.align\t4" << endl;
    }

    cout << endl;
    cout << "\t.section .text" << endl;
    cout << "\t.globl main" << endl << endl;
    cout << "main:" << endl;

    p = root->children[0];

    while(p) {
        recursive_gen_code(p);
        p = p->sibling;
    }

    if(root->label.next_label != "")
        cout << root->label.next_label << ":" << endl;
    cout << "\tret" << endl;
}