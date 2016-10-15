/* 	
*	Property Translator - PARSER
*	Author: Nikhil Saxena
*/
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <iostream>
	#include "CHelper.h"   
	void pperror(char *);
	int pplex(void);	
	extern CHelper MANAGER;
	extern CDAGNode* root;
%}
%union{
	char* str;
	int iVal;
	class CDAGNode* nPtr;
};
%type <nPtr> formula
%type <nPtr> predicate
%type <nPtr> expression
%token <str> NEW_LINE 
%right <str> IMPLIES
%right <str> NOT
%right <str> X F G N
%right <str> SEQUENCE
%left <str> SELECT
%left <str> OR
%left <str> AND
%left <str> OP
%left <str> CONCAT ARITHOP BV_BIN_OP
%right <str> BV_NOT
%token <str> BVCONSTT IDENTIFIER
%token <iVal> INTEGER
%token <str> L_BRACKET R_BRACKET L_BRACE R_BRACE L_PAREN R_PAREN

%%
program:
	formula	NEW_LINE			{ 
							root = $1;
							return 0;
						}
	;
formula:
	predicate				{	
							$$ = $1;
						}
	| L_PAREN formula R_PAREN		{	
							$$ = $2; 	
						}
	| NOT formula				{ 
							CDAGNode *n_f  = MANAGER.createDAG($1,$2);
							$$ = n_f; 
						}
	| formula AND formula	  		{ 
							CDAGNode *f_and  = MANAGER.createDAG($2,$1,$3);
							$$ = f_and; 
						}
	| formula OR formula  			{ 
							CDAGNode *f_or  = MANAGER.createDAG($2,$1,$3);
							$$ = f_or; 
						}
	| formula IMPLIES formula		{ 
							CDAGNode *f_imp  = MANAGER.createDAG($2,$1,$3);
							$$ = f_imp; 
						}
	| formula SEQUENCE formula		{ 	
							char temp[50];
							sprintf(temp,"%d",1);
							string temp1(temp);
							CDAGNode *i  = MANAGER.createDAG(temp1);
							CDAGNode *f_x  = MANAGER.createDAG("X",i,$3);
							CDAGNode *f_and  = MANAGER.createDAG("&&",$1,f_x);
							$$ = f_and; 
						}
	| X L_BRACKET INTEGER R_BRACKET formula	{ 
							char temp[50];
							sprintf(temp,"%d",$3);
							string temp1(temp);
							CDAGNode *i  = MANAGER.createDAG(temp1);
							CDAGNode *f_x  = MANAGER.createDAG("X",i,$5);
							$$ = f_x; 
						}
	| G formula L_BRACKET INTEGER R_BRACKET { 
							char temp[50];
							sprintf(temp,"%d",$4);
							string temp1(temp);
							CDAGNode *i  = MANAGER.createDAG(temp1);
							CDAGNode *f_g  = MANAGER.createDAG("G",$2,i);
							$$ = f_g;
						}
	| F formula L_BRACKET INTEGER R_BRACKET { 
							char temp[50];
							sprintf(temp,"%d",$4);
							string temp1(temp);
							CDAGNode *i  = MANAGER.createDAG(temp1);
							CDAGNode *f_f  = MANAGER.createDAG("F",$2,i);
							$$ = f_f;
						}
	| N formula L_BRACKET INTEGER R_BRACKET {	
							char temp[50];
							sprintf(temp,"%d",$4);
							string temp1(temp);
							CDAGNode *i  = MANAGER.createDAG(temp1);
							CDAGNode *n_f  = MANAGER.createDAG("!",$2);
							CDAGNode *f_g  = MANAGER.createDAG("G",n_f,i);
							$$ = f_g;
						}
    	;	
predicate:
    	expression OP expression		{		
							CDAGNode *pred  = MANAGER.createDAG($2,$1,$3);
							$$ = pred; 
						}
	;
expression:
	L_PAREN expression R_PAREN		{	
							$$ = $2;
						}
	| IDENTIFIER			  	{
							CDAGNode *id  = MANAGER.createDAG($1,0);
							$$ = id; 
						}
	| BVCONSTT				{
							CDAGNode *bvc  = MANAGER.createDAG($1);
							$$ = bvc; 
						}
	| INTEGER				{
							char temp[50];
							sprintf(temp,"%d",$1);
							string temp1(temp);
							CDAGNode *i  = MANAGER.createDAG(temp1);
							$$ = i; 
						}
	| expression L_BRACKET INTEGER SELECT INTEGER R_BRACKET {
							CDAGNode *sel  = MANAGER.createDAG($4,$1,$3,$5);
							$$ = sel; 
							
						}
	| expression CONCAT expression		{
							CDAGNode *con  = MANAGER.createDAG($2,$1,$3);
							$$ = con; 
						}
	| expression BV_BIN_OP expression	{
							CDAGNode *bv_bin  = MANAGER.createDAG($2,$1,$3);
							$$ = bv_bin; 
						}
	| BV_NOT expression			{
							CDAGNode *bv_un  = MANAGER.createDAG($1,$2);
							$$ = bv_un; 
						}
	| expression ARITHOP expression		{
							CDAGNode *arith  = MANAGER.createDAG($2,$1,$3);
							$$ = arith; 
						}
	;
%%
void pperror(char *s) {
    fprintf(stderr, "%s\n", s);
    cout<<"Exiting.."<<endl;
    exit(1);
}
