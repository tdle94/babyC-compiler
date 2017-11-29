#ifndef YOUR_CODE_H
#define YOUR_CODE_H
#include "stddef.h"
#include "stdlib.h"

// A counter to keep track of basic block
typedef struct BasicBlock BB;
struct BasicBlock {
	unsigned begin;		// begin of a block
};
static int blkCounter = 0;
BB blockKeeper[50];
// generate random character
static inline unsigned myrandomlt26() {
   long l;
   do { l = random(); } while (l>=(RAND_MAX/100)*100);
   return (unsigned)(l % 100);
}



// counter for virtual register
static int virtualReg = 0;
static int base = 0;
static int offset = 4;

typedef enum{ASTNODE_ARITH_OP, ASTNODE_LOGIC_OP, ASTNODE_COMPARE,
	         ASTNODE_ASSIGN, ASTNODE_IDENT, ASTNODE_NUM,
			  ASTNODE_IF, ASTNODE_WHILE, ASTNODE_IFELSE} ASTNodeType;

// Define all operation types (add, mult, etc) here.
typedef enum{ADD_OP, MULT_OP, DIV_OP, SUB_OP,
	         OR_OP, AND_OP, GT_OP, GE_OP, LT_OP,
			 LE_OP, EQ_OP, NE_OP, LOADI_OP, LOADAI_OP, STOREAI_OP, CBR_OP} ASTOp;

// generic node that has every field
typedef struct ASTNode ASTNode;
struct ASTNode{
	ASTNodeType type;
    ASTOp op; // The actual operation (add, mult, etc)
	int num;  //Need to store the actual value for number nodes
	char *name; //Need to store the actual variable name for ident nodes
	int base;
	int offset;
	char *logicOp;
	ASTNode* next;
	ASTNode* left; // Left child
	ASTNode* right; // Right child
};

// activation record pointer aka stack pointer
ASTNode *sp[50];

// Add functions to create the different kinds of ASTNodes
// These functions are called by the code embedded in the grammar.
// Here are some samples:
ASTNode* CreateNumNode(int num);
ASTNode* CreateIdentNode(char*);
ASTNode* CreateIdentLeftNode(char*);
ASTNode* CreateStatementListNode(ASTNode* st, ASTNode* stList);
ASTNode* AddDeclarationList(ASTNode*, ASTNode*);
ASTNode* CreateStatementListNode(ASTNode*, ASTNode*);
ASTNode* CreateAssignmentNode(ASTNode*, ASTNode*);
ASTNode* CreateAddNode(ASTNode*, ASTNode*);
ASTNode* CreateSubNode(ASTNode*, ASTNode*);
ASTNode* CreateMultNode(ASTNode*, ASTNode*);
ASTNode* CreateDivNode(ASTNode*, ASTNode*);
ASTNode* CreateWhileNode(ASTNode*, ASTNode*);
ASTNode* CreateIfNode(ASTNode*, ASTNode*);
ASTNode* CreateIfElseNode(ASTNode*, ASTNode*, ASTNode*);
ASTNode* CreateOrNode(ASTNode*, ASTNode*);
ASTNode* CreateAndNode(ASTNode*, ASTNode*);
ASTNode* CreateCmpNode(ASTNode*, ASTNode*, ASTNode*);
ASTNode* CreateOpNode(char*);
unsigned long HashFunc(char *);
char *GetVar(char*);

// Need a function to add a declaration to your symbol table
void AddDeclaration(char*);

/* These are the function that generates ILOC code after the construction of the AST */
void GenerateILOC(ASTNode*);
void Emit(ASTOp, int *, int *, int *);
int GetNextReg();
int Expr(ASTNode*);		// generate expression code
int GetCurrentReg();
int SearchRarp(char*);
void CreateRarp(ASTNode*);
#endif
