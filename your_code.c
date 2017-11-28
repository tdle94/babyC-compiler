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
    node->op = LOADI_OP;
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
    node->op = LOADAI_OP;
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

    return node;
}

ASTNode *CreateSubNode(ASTNode *expr, ASTNode *term)
{

    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = expr;
    node->right = term;
    node->op = SUB_OP;      // ASTOp
    node->type = ASTNODE_ARITH_OP;  // ASTNodeType

    return node;
}

ASTNode *CreateMultNode(ASTNode *term, ASTNode *factor)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = term;
    node->right = factor;
    node->op = MULT_OP;
    node->type = ASTNODE_ARITH_OP;

    return node;
}

ASTNode *CreateDivNode(ASTNode *term, ASTNode *factor)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = term;
    node->right = factor;
    node->op = DIV_OP;
    node->type = ASTNODE_ARITH_OP;

    return node;
}

ASTNode *CreateAssignmentNode(ASTNode *lhs, ASTNode *expr)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->right = expr;
    node->left = lhs;
    node->op = STOREAI_OP;
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
    node->op = CBR_OP;      // compare branch
    return node;
}

ASTNode *CreateIfNode(ASTNode *cond, ASTNode *stList)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = cond;
    node->right = stList;
    node->type = ASTNODE_IF;
    node->op = CBR_OP;

    return node;
}

ASTNode *CreateIfElseNode(ASTNode *cond, ASTNode *stList1, ASTNode *stList2)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = cond;
    node->right = stList1;
    node->next = stList2;
    node->type = ASTNODE_IFELSE;
    node->op = CBR_OP;

    return node;
}

ASTNode *CreateOrNode(ASTNode *cond, ASTNode *lTerm)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = cond;
    node->right = lTerm;
    node->type = ASTNODE_LOGIC_OP;
    node->logicOp = "||";
    node->op = OR_OP;

    return node;
}

ASTNode *CreateAndNode(ASTNode *lTerm, ASTNode *lFactor)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = lTerm;
    node->right = lFactor;
    node->type = ASTNODE_LOGIC_OP;
    node->logicOp = "&&";
    node->op = AND_OP;

    return node;
}


ASTNode *CreateOpNode(char *op)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!strcmp("<=", op))
        node->op = LE_OP;
    else if (!strcmp("<", op))
        node->op = LT_OP;
    else if (!strcmp(">=", op))
        node->op = GE_OP;
    else if (!strcmp(">", op))
        node->op = GT_OP;
    else if (!strcmp("!=", op))
        node->op = NE_OP;
    else
        node->op = EQ_OP;
    return node;
}

ASTNode *CreateCmpNode(ASTNode *lExpr, ASTNode *Op, ASTNode *rExpr)
{
    // ASTNode *node = malloc(sizeof(ASTNode));
    Op->left = lExpr;
    Op->right = rExpr;
    Op->type = ASTNODE_COMPARE;

    return Op;
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
    Expr(node);
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
        case ASTNODE_COMPARE:
            t1 = Expr(node->left);
            t2 = Expr(node->right);
            res = GetNextReg();
            Emit(node->op, &t1, &t2, &res);
            break;
        case ASTNODE_LOGIC_OP:
            t1 = Expr(node->left);
            t2 = Expr(node->right);
            res = GetNextReg();
            Emit(node->op, &t1, &t2, &res);
            break;
        case ASTNODE_WHILE:
            printf("B%d:\n", currentBlock);
            currentBlock++;
            res = Expr(node->left);
            Emit(node->op, &res, NULL, NULL);
            Expr(node->right);
            break;
        case ASTNODE_IF:
            res = Expr(node->left);
            Emit(node->op, &res, NULL, NULL);
            Expr(node->right);
            break;
        case ASTNODE_IFELSE:
            res = Expr(node->left);
            Emit(node->op, &res, NULL, NULL);
            Expr(node->right);
            Expr(node->next);
            break;
        case ASTNODE_NUM:
            res = GetNextReg();
            Emit(node->op, &node->num, NULL, &res);
            break;
        case ASTNODE_IDENT:
            res = GetNextReg();
            node->offset = SearchRarp(node->name);
            Emit(node->op, &node->base, &node->offset, &res);
            break;
        case ASTNODE_ARITH_OP:
            t1 = Expr(node->left);
            t2 = Expr(node->right);
            res = GetNextReg();
            Emit(node->op, &t1, &t2, &res);
            break;
        case ASTNODE_ASSIGN:
            res = Expr(node->right);
            CreateRarp(node);
            Emit(node->op, &res, &node->base,& node->offset);
            if (node->next == NULL) break;
            res = Expr(node->next);
            break;
        default:
            res = Expr(node->left);
            res = Expr(node->right);
            break;
    }
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

    while (i > 0)
    {
        i = (i - 4)/offset;
        if (!strcmp(sp[i]->name, var))
            return sp[i]->offset;
    }
    //printf("%d\n", virtualReg);
    return base;
}

int GetCurrentReg()
{
    return virtualReg;
}

// emit ILOC code
void Emit(ASTOp op, int *src1, int *src2, int *dest)
{
    // char beginBasicBlk[30];
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
        case SUB_OP:
            printf("sub r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case OR_OP:
            printf("or r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case AND_OP:
            printf("and r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case GT_OP:
            printf("cmp_GT r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case GE_OP:
            printf("cmp_GE r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case LT_OP:
            printf("cmp_LT r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case LE_OP:
            printf("cmp_LE r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case EQ_OP:
            printf("cmp_EQ r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case NE_OP:
            printf("cmp_NE r%d, r%d -> r%d\n", *src1, *src2, *dest);
            break;
        case CBR_OP:
            printf("cbr r%d -> B%d, B%d\n", *src1, currentBlock, currentBlock+1);
            break;
        default:
            break;
    }
}
