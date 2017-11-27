#include "your_code.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Hash data structure
typedef struct
{
    int key;
    char *variable;
}data;
static data hash[16];
static int M = 16;
static int N = 0;


// Write the implementations of the functions that do the real work here
ASTNode *CreateNumNode(int num)
{
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = ASTNODE_NUM;
    node->num = num;
    return node;
}


ASTNode *CreateIdentNode(char *name)
{

    if (GetVar(name) == NULL)
    {
        printf("%s\n", "Ident not declare");
        exit(1);
    }
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = ASTNODE_IDENT;
    node->name = name;
    return node;
}

ASTNode *CreateIdentLeftNode(char *name)
{
    return CreateIdentNode(name);
}


ASTNode *CreateAddNode(ASTNode *expr, ASTNode *term)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = expr;
    node->right = term;
    node->op = ADD_OP;
    node->type = ASTNODE_ARITH_OP;
    node->logicOp = "+";

    return node;
}

ASTNode *CreateSubNode(ASTNode *expr, ASTNode *term)
{

    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = expr;
    node->right = term;
    node->op = SUB_OP;      // ASTOp
    node->type = ASTNODE_ARITH_OP;  // ASTNodeType
    node->logicOp = "-";

    return node;
}

ASTNode *CreateMultNode(ASTNode *term, ASTNode *factor)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = term;
    node->right = factor;
    node->op = MULT_OP;
    node->type = ASTNODE_ARITH_OP;
    node->logicOp = "*";

    return node;
}

ASTNode *CreateDivNode(ASTNode *term, ASTNode *factor)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = term;
    node->right = factor;
    node->op = DIV_OP;
    node->type = ASTNODE_ARITH_OP;
    node->logicOp = "/";

    return node;
}

ASTNode *CreateAssignmentNode(ASTNode *lhs, ASTNode *expr)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->right = expr;
    node->left = lhs;
    node->type = ASTNODE_ASSIGN;
    return node;
}


// Take a statement node and a statement list node and connect them together
// to form a bigger statement list node (add the statement to the statement list).
// Return a pointer to the bigger list that resulted from this linking
ASTNode *CreateStatementListNode(ASTNode *st, ASTNode *stList)
{
    //
    // if (st == NULL)
    //     st = malloc(sizeof(ASTNode));
    // if (stList == NULL)
    //     stList = malloc(sizeof(ASTNode));
    st->next = stList;

    // st = malloc(sizeof(ASTNode));
    // stList = malloc(sizeof(ASTNode));
    //  st->next = stList;
    return st;
}

ASTNode *AddDeclarationList(ASTNode *declaration, ASTNode *declarationList)
{

    declaration = malloc(sizeof(ASTNode));
    declarationList = malloc(sizeof(ASTNode));
    declaration->next = declarationList;

    return declaration;
}

ASTNode *CreateWhileNode(ASTNode *cond, ASTNode *stList)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = cond;
    node->right = stList;
    node->type = ASTNODE_WHILE;

    return node;
}

ASTNode *CreateIfNode(ASTNode *cond, ASTNode *stList)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = cond;
    node->right = stList;
    node->type = ASTNODE_IF;

    return node;
}

ASTNode *CreateIfElseNode(ASTNode *cond, ASTNode *stList1, ASTNode *stList2)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = cond;
    node->right = stList1;
    node->next = stList2;
    node->type = ASTNODE_IF;

    return node;
}

ASTNode *CreateOrNode(ASTNode *cond, ASTNode *lTerm)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = cond;
    node->right = lTerm;
    node->type = ASTNODE_LOGIC_OP;

    return node;
}

ASTNode *CreateAndNode(ASTNode *lTerm, ASTNode *lFactor)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = lTerm;
    node->right = lFactor;
    node->type = ASTNODE_LOGIC_OP;

    return node;
}

ASTNode *CreateCmpNode(ASTNode *lExpr, char *logicOp, ASTNode *rExpr)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = lExpr;
    node->right = rExpr;
    node->type = ASTNODE_COMPARE;
    node->op = logicOp;

    return node;
}


void AddDeclaration(char *name)
{
    int i;
    if (GetVar(name) != NULL)
    {
        printf("%s %s\n", "Multiple declaration of ", name);
        return;
    }

    for (i = HashFunc(name); hash[i].variable != NULL; i = (i + 1) % M)
        if (hash[i].key == i)
        {
            hash[i].variable = name;
            return;
        }
    hash[i].key = i;
    hash[i].variable = name;
    N++;
}

char *GetVar(char *lookup)
{
    int i;
    for (i = HashFunc(lookup); hash[i].variable != NULL; i = (i + 1) % M)
        if (strcmp(lookup, hash[i].variable) == 0)
            return hash[i].variable;


    return NULL;
}

unsigned long HashFunc(char *key)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash % M;
}

// GenerateILOC
void GenerateILOC(ASTNode* node)
{
    int reg = Expr(node);

    printf("%d\n", reg);
}

int GetNextReg()
{
    virtualReg++;
    return virtualReg;
}


// generate ILOC for expression nodes and return the register number
int Expr(ASTNode * node)
{
    int res,
        t1,
        t2;

    switch (node->type)
    {
        case ASTNODE_NUM:
            res = GetNextReg();
            //printf("num node => reg: %d, val: %d\n", *res, node->num);
            Emit(LOADI_OP, &node->num, NULL, &res);
            break;
        case ASTNODE_IDENT:
            res = GetNextReg();
            node->offset = SearchRarp(node->name);
            //printf("ident node => reg: %d %s %d\n", *res, node->name, node->offset);
            Emit(LOADAI_OP, &node->base, &node->offset, &res);
            break;
        case ASTNODE_ARITH_OP:
            t1 = Expr(node->left);
            //printf("t1: %d \n", *t1);
            t2 = Expr(node->right);
            //printf("t1: %d t2: %d \n", *t1, *t2);
            res = GetNextReg();
            //printf("arith node %c => reg: %d, t1: %d, t2: %d\n", node->logicOp, *res, *t1, *t2);
            Emit(node->op, &t1, &t2, &res);
            break;
        case ASTNODE_ASSIGN:
            res = Expr(node->right);
            CreateRarp(node);
            //printf("assign node => reg: %d, var: %s, base: %d, offset: %d\n", *res, node->name, node->base, node->offset);
            Emit(STOREAI_OP, &res, &node->base,& node->offset);
            if (node->next == NULL) break;
            res = Expr(node->next);
            break;
        default:
            res = Expr(node->left);
            res = Expr(node->right);
            break;
    }
    //printf("return reg: %d\n", *res);
    return res;
}

// Create an activation record pointer to that variable
void CreateRarp(ASTNode *node)
{
    node->num = node->right->num;
    node->name = node->left->name;
    node->base = 0;
    node->offset = base;
    sp[base/offset] = node;
    base = base + 4;
}

// Search for activation record pointer of the previous or current variable
int SearchRarp(char *var)
{
    int i = base;
    //printf("%d\n", sp[1]->offset);
    while (i > 0)
    {
        i = (i - 4)/offset;
        if (!strcmp(sp[i]->name, var))
            return sp[i]->offset;
    }
    return GetCurrentReg();
}

int GetCurrentReg()
{
    return virtualReg;
}

// emit ILOC code
void Emit(ASTOp op, int *src1, int *src2, int *dest)
{
    switch (op) {
        case LOADI_OP:
            printf("loadi %d -> r%d\n", *src1, *dest);
            break;
        case LOADAI_OP:
            printf("loadAI rarp, %d -> r%d\n", *src2, *dest);
            break;
        case STOREAI_OP:
            printf("storeAI r%d -> rarp, %d\n", *src1, *dest);
            break;
        case MULT_OP:
            printf("mult r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case ADD_OP:
            printf("add r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case DIV_OP:
            printf("div r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case OR_
        default:
            break;
    }
}
