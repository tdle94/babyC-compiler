%{
	#include <stdio.h>
	#include "your_code.h"

// The parser needs to call the scanner to get the next token
	extern int yylex();

// The error function
	extern int yyerror(const char *);

// The ASTNode root
    extern ASTNode* gASTRoot;
%}

//Put any initialization code here
%initial-action
{

}
%error-verbose

%token LE "<="
%token GE ">="
%token EQ "=="
%token NE "!="
%token OR "||"
%token AND "&&"
%token MAIN "main"
%token INT "int"
%token IF "if"
%token ELSE "else"
%token WHILE "while"


%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

//Define the types for the grammar attributes ($$, $1, $2, ...)


%union
{
	struct ASTNode* node; // Most $$ values will be ASTNodes
	int num; // Integer numbers
	char* string; // Strings for identifiers
}



//Specify the type for each token. Only needed for IDENT and NUM, because they are the only ones that have actual regexp rules
%token <string> IDENT
%token <num> NUM


//Specify the type for each non-terminal in the grammar. Here are some samples:
%type <node> If
%type <node> While
%type <node> DeclarationList
%type <node> Declaration
%type <node> StatementList
%type <node> Statement
%type <node> Assignment
%type <node> LHS
%type <node> Expr
%type <node> Term
%type <node> Factor
%type <node> Condition
%type <node> Lterm
%type <node> Lfactor
%type <node> Compare
%type <node> Op
%type <node> Code


// Add the rest of the types for the grammar's symbols


%%

// Write the grammar for BabyC, and write an embedded action for each production. Here are some samples for you:

Goal: "main" '(' ')' '{' DeclarationList StatementList '}'	{gASTRoot=$6;} // Store the AST root node in gASTRoot
;

DeclarationList: {$$=NULL;}
				| Declaration DeclarationList {$$ = AddDeclarationList($1, $2);}    // Note that a DeclarationList may be empty
;

Declaration: "int" IDENT ';' {AddDeclaration($2); printf("Processing declaration of %s\n", $2);}
;

StatementList: {$$ = NULL;}
               | Statement StatementList	{$$ = CreateStatementListNode($1,$2); printf("Adding a Statement to a statement list \n");}
;

Statement:	Assignment 		{$$ = $1;}
			| If			{$$ = $1;}
			| While			{$$ = $1;}
;

Assignment: LHS '=' Expr ';'			{$$ = CreateAssignmentNode($1, $3); printf("Creating Assignment node \n");}
;

LHS:  IDENT					{$$ = CreateIdentLeftNode($1); printf("Creating left-hand IDENT node for %s\n", $1);}
;

Expr: Term 					{$$ = $1;}
	  | Expr '+' Term 		{$$ = CreateAddNode($1, $3); puts("Creating add node");}
	  | Expr '-' Term			{$$ = CreateSubNode($1, $3); puts("Creating Subtraction node");}
;

Term: Factor 				{$$ = $1;}
	  | Term '*' Factor  		{$$ = CreateMultNode($1, $3); puts("Creating Multiplication node");}
	  | Term '/' Factor		{$$ = CreateDivNode($1, $3); puts("Creating div node");}
;

Factor: IDENT 		{$$ = CreateIdentNode($1); printf("Creating IDENT node for %s\n", $1);}
	| NUM 		{$$ = CreateNumNode($1); printf("Creating NUM node for %d\n", $1);}
	| '('Expr')'	{$$ = $2; puts("Creating Expression node in parentheses")}
;


If: IF '('Condition')' Code ELSE Code				{$$ = CreateIfElseNode($3, $5, $7); puts("Creating if else node");}
	| IF '('Condition')' Code				{$$ = CreateIfNode($3, $5); puts("Creating if node");}
;

Code: Statement								{$$ = $1;}
	  | '{'StatementList'}'					{$$ = $2;}
;



Condition: Condition OR Lterm {$$ = CreateOrNode($1, $3); puts("Creating OR node");}
		   | Lterm			  {$$ = $1;}
;

Lterm: Lterm AND Lfactor 	{$$ = CreateAndNode($1, $3); puts("Creating AND node");}
	   | Lfactor			{$$ = $1;}
;


Lfactor: Compare			{$$ = $1;}
;

Compare: Expr Op Expr		{$$ = CreateCmpNode($1, $2, $3); puts("Creating Compare node");}
;

Op: GE 		{$$ = CreateOpNode(">=");}
	| LE	{$$ = CreateOpNode("<=");}
	| NE	{$$ = CreateOpNode("!=");}
	| EQ	{$$ = CreateOpNode("==");}
	| '>'	{$$ = CreateOpNode(">");}
	| '<'	{$$ = CreateOpNode("<");}
;



While: WHILE '('Condition')' '{'StatementList'}' {$$ = CreateWhileNode($3, $6); puts("Creating while node")}
;



%%
