
%{
/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "idinfo.h"

#include "DagManager.h"
#include "macros.h"
#include "Oprs.h"
#include "helping_functions.h"
#include "Value.h"
#define YYDEBUG 1
#define t_ENUM 9999
#define MAX_DELETED_COUNT 5000

using namespace std;


unsigned int node_counter = 0;

void yyerror(char *);
 int yylex(void);  
 int yywrap()
 {
   return 1;
 }
struct node_pair
{
	int first;
	int second;
};


class NamedExprPair
{
 public:
  string symbol;
  DAGNode *node;
  NamedExprPair(string symbol_in, DAGNode *node_in)
    {
      symbol = symbol_in;
      node = node_in;
    }
};

 vector<NamedExprPair> v_named_exprs;
class VectorOfExpressions
{
 public:
  vector<DAGNode *> v_expressions;
  VectorOfExpressions(vector<DAGNode* > v_expressions_to_push)
    {
      for(int i=0;i<v_expressions_to_push.size();i++)
        {
          v_expressions.push_back(v_expressions_to_push[i]);
        }
    }
  VectorOfExpressions() //A do nothing constructor..
    {
    }

};

vector<DAGNode *> assign_expressions(
                                          class VectorOfExpressions *v_exprs_in
                                          )
{
  vector<DAGNode *>v_exprs;
  for(int i=0;i<v_exprs_in->v_expressions.size();i++)
    {
      v_exprs.push_back(v_exprs_in->v_expressions[i]);
    }

  return v_exprs;
}

int WidthInteger=0;
bool LIA_Input = false;
bool LIA_Input_Signed = false;
unsigned long long int ModWidthInteger=1;
unsigned long long int ModWidthIntegerHalf=0;
bool code_to_handle_assumptions = true;



string shiftNonNegativeToPositiveSide(string bvconstant)
{
unsigned long long int number=strtoull(bvconstant.c_str(), NULL, 0);
number = ModWidthIntegerHalf+number;
char number_char[10];
sprintf(number_char, "%llu", number);
string number_str(number_char);
return number_str;
}

string shiftNegativeToPositiveSide(string bvconstant)
{
long long int original_number=strtoll(bvconstant.c_str(), NULL, 0)*-1;
original_number = (signed)ModWidthIntegerHalf+original_number;

if(original_number < 0)
    {
    cout<<"\nError in function shiftNegativeToPositiveSide in expr1.y\n";
    exit(1);
    }

unsigned long long int number = original_number;
char number_char[10];
sprintf(number_char, "%llu", number);
string number_str(number_char);
return number_str;
}

string getAdditiveInverse(string bvconstant)
{
unsigned long long int number=strtoull(bvconstant.c_str(), NULL, 0);

if(number<0 || number>=ModWidthInteger) 
    {cout<<"\nNumber "<<number<<" more than maximum mod or less than zero inside function getAdditiveInverse in expr1.y . Exiting...\n"; exit(1);}

unsigned long long int AInumber;
if(number==0) 
    {
    AInumber = 0;
    }
else
    {
    AInumber = ModWidthInteger-number;
    }
char AInumber_char[10];
sprintf(AInumber_char, "%llu", AInumber);
string AInumber_str(AInumber_char);
return AInumber_str;
}

void checkIfPositiveNumberIsFitting(string bvconstant)
{
unsigned long long int number=strtoull(bvconstant.c_str(), NULL, 0);
unsigned long long int times = 2*(number+1);

if(times > ModWidthInteger) 
    {cout<<"\nNumber "<<number<<" is not representable using "<<WidthInteger<<" bits.  Error in function checkIfPositiveNumberIsFitting  in expr1.y . Exiting...\n"; exit(1);}
}

void checkIfNegativeNumberIsFitting(string bvconstant)
{
unsigned long long int number=strtoull(bvconstant.c_str(), NULL, 0);
unsigned long long int times = 2*number;

if(times > ModWidthInteger) 
    {cout<<"\nNumber  ~"<<number<<" is not representable using "<<WidthInteger<<" bits.  Error in function checkIfNegativeNumberIsFitting  in expr1.y . Exiting...\n"; exit(1);}
}

vector<string> graph_nodes;
vector< pair<int,int> > edges;

map<string, int> decl_types_list;

map < DAGNode*, DAGNode* > assumption_table;

 DAGManager dm(true,false);
vector<DAGNode*> root;
int count1 = 0;
int count2 = 0;
int globalnewhashcounter=0;
int globalnewnodenamecounter=0;

set<string> VarSetToElim;
map<string, int> WidthTable;
ofstream *outfile = new ofstream("parser_debug_file.txt");

int deleted_node_count=0;




%}


%union{
  struct IDinfo *ds_Id;
  class DAGNode *dagnode;
  class VectorOfExpressions *vector_of_expressions;
  int value;
  class Type *type_ptr;
}

%token <value> LOG_NOT Log_And Log_Or Ite Define Bool Real Int BitVector BV_ADD BV_MUL Benchmark Extrafuns Formula Ifthenelse Extract Concat BV_OR Let BV_AND BV_SHL BV_LSHR ZERO_EXTEND BV_NOT BV_SUB BV_UDIV BV_ULT BV_XOR IFF SIGN_EXTEND Flet Extrapreds BV_NEG BV_SLE BV_ULE BV_UGT Assumption BV_UGE BV_SGE Exists
%token <value> Assert Check Define_Type UType t_EQSym t_NESym t_LTSym 
%token <value> t_LESym t_GTSym t_GESym t_Plus t_Minus t_Ampersand t_UnaryMinus
%token <value> t_Star t_Slash t_LeftParen t_RightParen t_doubleColon
%token <value> t_Colon t_Semicolon t_Arrow t_Box t_Bar t_Dot t_LeftBrack t_RightBrack

%token  < ds_Id > t_Identifier t_AbstractLit t_BvLit_Dec t_BoolLit t_Var

%type  <type_ptr>  yices_basic_types

%type <dagnode> expr 
%type <vector_of_expressions> exprlist 
%%

 /*------------------Grammar----------------------------------------*/


start : t_LeftParen Benchmark t_Identifier bench_attribute t_RightParen
	{
	int garbage_nodes = dm.collect_garbage();
	/*cout<<"Successfully collected "<<garbage_nodes<<" number of garbage nodes\n";*/
	outfile->close();
	} 
	;

bench_attribute : fun_declaration | formula_declaration | pred_declaration | assumption_declaration | elimination_declaration | fun_declaration bench_attribute | formula_declaration bench_attribute | pred_declaration bench_attribute | assumption_declaration bench_attribute | elimination_declaration bench_attribute;

fun_declaration : t_Colon Extrafuns t_LeftParen fun_symb_decl t_RightParen;

pred_declaration : t_Colon Extrapreds t_LeftParen pred_symb_decl t_RightParen;

elimination_declaration : t_Colon Exists t_LeftParen  id_list t_RightParen;

id_list : t_Identifier {VarSetToElim.insert($1->name);}
           | id_list t_Identifier  {VarSetToElim.insert($2->name);} ;

elimination_id_list : t_LeftParen t_Identifier Int t_RightParen 
                                     {
                                         if(!LIA_Input) 
                                            {
                                            cout<<"\nInput is in Linear Arithmetic over Integers. Please set the parameters -LIA_Input and -WidthInteger properly\n";
                                            exit(1);
                                            }

                                          if(WidthInteger==0) 
                                            {
                                            cout<<"\nPlease set the parameters -WidthInteger properly\n";
                                            exit(1);
                                            } 

                                         Value *v = new Value();
                                         Type *t = new Type();
                                         t->data_type = BITVECTOR_SYMBOL;
                                         t->width = WidthInteger;	
                                         
                                         node_counter++;
                                         string node_id = "unique_node_no_" + integerToString(node_counter);

                                         v->set_node_type(*t);
                                         v->set_unique_node_identifier(node_id);			 

                                         DAGNode *node = dm.createNode ($2->name,(void*)v);

                                         /*cout<<"\nWidth of bitvector "<<$2->name<<" is "<<t->width<<endl;*/
                                         /*cout<<"\nInserting it into the width table"<<endl;*/


                                         WidthTable.insert(make_pair($2->name, t->width));
                                         
                                         /*cout<<"\nInserting it into VarSetToElim"<<endl;*/
                                         
                                        VarSetToElim.insert($2->name);
                                       }
           | elimination_id_list  t_LeftParen t_Identifier Int t_RightParen 
                                        {
                                         if(!LIA_Input) 
                                            {
                                            cout<<"\nInput is in Linear Arithmetic over Integers. Please set the parameters -LIA_Input and -WidthInteger properly\n";
                                            exit(1);
                                            }

                                          if(WidthInteger==0) 
                                            {
                                            cout<<"\nPlease set the parameters -WidthInteger properly\n";
                                            exit(1);
                                            } 

                                         Value *v = new Value();
                                         Type *t = new Type();
                                         t->data_type = BITVECTOR_SYMBOL;
                                         t->width = WidthInteger;	
                                         
                                         node_counter++;
                                         string node_id = "unique_node_no_" + integerToString(node_counter);

                                         v->set_node_type(*t);
                                         v->set_unique_node_identifier(node_id);			 

                                         DAGNode *node = dm.createNode ($3->name,(void*)v);

                                         /*cout<<"\nWidth of bitvector "<<$3->name<<" is "<<t->width<<endl;*/
                                         /*cout<<"\nInserting it into the width table"<<endl;*/


                                         WidthTable.insert(make_pair($3->name, t->width));
                                         
                                         /*cout<<"\nInserting it into VarSetToElim"<<endl;*/
                                         
                                        VarSetToElim.insert($3->name);
                                       }
                                        ;


fun_symb_decl : t_LeftParen t_Identifier yices_basic_types t_LeftBrack t_AbstractLit t_RightBrack t_RightParen
		  { 
			 Value *v = new Value();
			 Type *t = $3;

			 int width = atoi($5->name);	
			 t->width = width;
			 node_counter++;
			 string node_id = "unique_node_no_" + integerToString(node_counter);
			 
			 v->set_node_type(*t);
			 v->set_unique_node_identifier(node_id);			 
			 
			 DAGNode *node = dm.createNode ($2->name,(void*)v);

			 /*cout<<"\nWidth of bitvector "<<$2->name<<" is "<<$5->name<<endl;*/
			 /*cout<<"\nInserting it into the width table"<<endl;*/

			 		 
			 WidthTable.insert(make_pair($2->name, width));

			 /*cout<<"  Matched type_define_constructs\n";*/
			 
		       }
                       |
                       t_LeftParen t_Identifier yices_basic_types t_RightParen
                    { 
                         if(!LIA_Input) 
                            {
                            cout<<"\nInput is in Linear Arithmetic over Integers. Please set the parameters -LIA_Input and -WidthInteger properly\n";
                            exit(1);
                            }

                          if(WidthInteger==0) 
                            {
                            cout<<"\nPlease set the parameters -WidthInteger properly\n";
                            exit(1);
                            } 

			 Value *v = new Value();
			 Type *t = $3;

			 int width = WidthInteger;	
			 t->width = width;
			 node_counter++;
			 string node_id = "unique_node_no_" + integerToString(node_counter);
			 
			 v->set_node_type(*t);
			 v->set_unique_node_identifier(node_id);			 
			 
			 DAGNode *node = dm.createNode ($2->name,(void*)v);

			 /*cout<<"\nWidth of bitvector "<<$2->name<<" is "<<t->width<<endl;*/
			 /*cout<<"\nInserting it into the width table"<<endl;*/

			 		 
			 WidthTable.insert(make_pair($2->name, width));

			 /*cout<<"  Matched type_define_constructs\n";*/
			 
		       }


                        ;

pred_symb_decl : t_LeftParen t_Identifier t_RightParen
		
			{ 
			 Value *v = new Value();
			 Type *t = new Type();
		    	t->data_type = BITVECTOR_SYMBOL;
		    	t->width = 1;
			
			 node_counter++;
			 string node_id = "unique_node_no_" + integerToString(node_counter);
			 
			 v->set_node_type(*t);
			 v->set_unique_node_identifier(node_id);			 
			 
			 DAGNode *node = dm.createNode ($2->name,(void*)v);

			 /*cout<<"\nWidth of bitvector "<<$2->name<<" is 1"<<endl;*/
			 /*cout<<"\nInserting it into the width table"<<endl;*/

			 int width = 1;			 
			 WidthTable.insert(make_pair($2->name, width));

			 /*cout<<"  Matched type_define_constructs\n";*/
			 
		       };


yices_basic_types : 
                  Bool { 
		    /*cout<<"  Matched basic type Bool\n";*/
		    Type *node_type = new Type();
		    node_type->data_type = LOGICAL_SYMBOL;
		    node_type->width = IMMATERIAL_WIDTH;
		    $$ = node_type;
		    
		  } 
                  | 
                  Real { 
		    /*cout<<"  Matched basic type Real\n";*/
		    Type *node_type = new Type();
		    node_type->data_type = REAL_SYMBOL;
		    node_type->width = IMMATERIAL_WIDTH;
		    $$ = node_type;		    
		  } 
                  | 
                  Int { 
		    /*cout<<"  Matched basic type Int. Converting Int to a bit-vector of size "<<WidthInteger<<" bits \n";*/
                    Type *node_type = new Type();
		    node_type->data_type = BITVECTOR_SYMBOL;
		    node_type->width = IMMATERIAL_WIDTH;
		    $$ = node_type;		    
		  }
                   |
		   BitVector
		   {
		    /*cout<<"  Matched basic type Bitvector\n";*/
		    Type *node_type = new Type();
		    node_type->data_type = BITVECTOR_SYMBOL;
		    node_type->width = IMMATERIAL_WIDTH;
		    $$ = node_type;

		   }

;

formula_declaration : t_Colon Formula expr 
			{
			root.push_back($3);
			/*cout<<"\nFormula declaration is parsed\n";*/
			}
                        |
                        t_Colon Formula t_LeftParen  Exists elimination_id_list expr t_RightParen
                        {
                        root.push_back($6);
			/*cout<<"\nFormula declaration is parsed\n";*/
                        }
                        ;

assumption_declaration : t_Colon Assumption expr 
			{
                        if(code_to_handle_assumptions)
                            {
                             assumption_table.insert (pair < DAGNode*, DAGNode* > (dm.getIThChildOfNode ($3, 0), dm.getIThChildOfNode ($3, 1)));
                             }

			/*cout<<"\nAssumption declaration is parsed\n";*/
			};

expr : t_LeftParen LOG_NOT exprlist t_RightParen 
              { 
	       /*cout<<"  Matched an expr\n";*/
		Value *v = new Value();
		Type t;
		t.data_type = LOGICAL_OPERATOR;
		t.width = IMMATERIAL_WIDTH;
		node_counter++;
		string node_id = "unique_node_no_" + integerToString(node_counter);
			 
		v->set_node_type(t);
		v->set_unique_node_identifier(node_id);			 
		
	       vector <DAGNode *> v_operands = assign_expressions($3);
	       DAGNode * node = dm.createNode("not",v_operands,(void*)v);
	       $$ = node;
	      }

;

expr : t_LeftParen Log_And exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/
	       vector <DAGNode *> v_operands = assign_expressions($3);
		Value *v = new Value();
		Type t;
		t.data_type = LOGICAL_OPERATOR;
		t.width = IMMATERIAL_WIDTH;
		node_counter++;
		string node_id = "unique_node_no_" + integerToString(node_counter);
			 
		v->set_node_type(t);
		v->set_unique_node_identifier(node_id);			 
		
	       DAGNode * node = dm.createNode("and",v_operands,(void*)v);
	       

	       $$ = node;
	     }
;

expr : t_LeftParen Log_Or exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/
	       vector <DAGNode *> v_operands = assign_expressions($3);
		Value *v = new Value();
		Type t;
		t.data_type = LOGICAL_OPERATOR;
		t.width = IMMATERIAL_WIDTH;
		node_counter++;
		string node_id = "unique_node_no_" + integerToString(node_counter);
			 
		v->set_node_type(t);
		v->set_unique_node_identifier(node_id);			 
		
	       DAGNode * node = dm.createNode("or",v_operands,(void*)v);

	       $$ = node;
	     }
;


expr : t_LeftParen Ifthenelse exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/
	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);
		DAGNode * node = dm.createNode("if_then_else",v_operands,v_operands[1]->getValue());

	       $$ = node;
	     }
;


expr : t_LeftParen Ite exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/
	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);
		DAGNode * node = dm.createNode("ite",v_operands,v_operands[1]->getValue());

	       $$ = node;
	     }
;

expr : t_LeftParen t_EQSym exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/
	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);
		Value *v = new Value();
		Type t;
		t.data_type = LOGICAL_OPERATOR;
		t.width = IMMATERIAL_WIDTH;
		node_counter++;
		string node_id = "unique_node_no_" + integerToString(node_counter);
			 
		v->set_node_type(t);
		v->set_unique_node_identifier(node_id);			 
		
	       DAGNode * node = dm.createNode("=",v_operands, (void*)v);


	       $$ = node;
	     }
;


expr : t_LeftParen t_LTSym exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvult",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen t_GTSym exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvugt",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen t_GESym exprlist t_RightParen 
             { 
	      /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvuge",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen t_LESym exprlist t_RightParen 
             { 
	      /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvule",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen t_Plus exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvadd",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen t_Minus exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvsub",v_operands,(void*)new_v);

	       $$ = node;
	     }
;



expr : t_LeftParen t_Star exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvmul",v_operands,(void*)new_v);

	       $$ = node;
	     }
;



expr : t_LeftParen BV_ADD exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvadd",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen BV_XOR exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvxor",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen IFF exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("iff",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen BV_SUB exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvsub",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen BV_UDIV exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvudiv",v_operands,(void*)new_v);

	       $$ = node;
	     }
;



expr : t_LeftParen BV_MUL exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvmul",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen BV_ULT exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvult",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen BV_UGT exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvugt",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen BV_ULE exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvule",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen BV_UGE exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvuge",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen BV_SGE exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvsge",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen BV_SLE exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvsle",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen Concat exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("concat",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen Extract t_LeftBrack t_AbstractLit t_Colon t_AbstractLit t_RightBrack exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr Extract "<<$4->name<<", "<<$6->name<<"\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($8);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);

	       int new_sel_handling=1;

	       if(new_sel_handling==1)
		 {
		   // newly added
		   Value *v1 = new Value();
		   Type t1;
		   t1.data_type = INTEGER_CONSTANT;
		   t1.width = IMMATERIAL_WIDTH;
		   node_counter++;
		   node_id = "unique_node_no_" + integerToString(node_counter);
			 
		   v1->set_node_type(t1);
		   v1->set_unique_node_identifier(node_id);
		
		   DAGNode* high_sel = dm.createNode($4->name, (void*)v1);

		   Value *v2 = new Value();
		   Type t2;
		   t2.data_type = INTEGER_CONSTANT;
		   t2.width = IMMATERIAL_WIDTH;
		   node_counter++;
		   node_id = "unique_node_no_" + integerToString(node_counter);
			 
		   v2->set_node_type(t2);
		   v2->set_unique_node_identifier(node_id);
		
		   DAGNode* low_sel = dm.createNode($6->name, (void*)v2);

		   vector<DAGNode*> Children;

		   if(v_operands.size()!=1)
		     {
		       cout<<"\nError in extract in parsing\n";
		       exit(1);
		     }
		   Children.push_back(v_operands[0]);
		   Children.push_back(low_sel);
		   Children.push_back(high_sel);
		   

		   
		   string extract_name = "select"; // stored as sel(x, low, high)
		   DAGNode * node = dm.createNode(extract_name,Children,(void*)new_v);
		   $$ = node;
		 }
	       // newly added ends here
	       else
		 {

		   string high($4->name);
		   string low($6->name);

		   string extract_name = "select ";
		   extract_name += high;
		   extract_name += " ";
		   extract_name += low;

		   DAGNode * node = dm.createNode(extract_name,v_operands,(void*)new_v);

		   $$ = node;
		 }
	     }
;


expr : t_LeftParen BV_OR exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvor",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen BV_AND exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvand",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen BV_SHL exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvshl",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen BV_LSHR exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvlshr",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen BV_NOT exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvnot",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen BV_NEG exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($3);

	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = IMMATERIAL_WIDTH;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("bvneg",v_operands,(void*)new_v);

	       $$ = node;
	     }
;

expr : t_LeftParen ZERO_EXTEND t_LeftBrack t_AbstractLit t_RightBrack exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($6);
	
		int width = atoi($4->name);
	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = width;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("zero_extend",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen SIGN_EXTEND t_LeftBrack t_AbstractLit t_RightBrack exprlist t_RightParen 
             { 
	       /*cout<<"  Matched an expr\n";*/

	       vector <DAGNode *> v_operands;
	       v_operands = assign_expressions($6);
	
		int width = atoi($4->name);
	       
	       Type t;
	       node_counter++;
	       string node_id = "unique_node_no_" + integerToString(node_counter);
	       t.data_type = BITVECTOR_OPERATOR;
	       t.width = width;

	       Value *new_v = new Value();
	       new_v->set_node_type(t);
	       new_v->set_unique_node_identifier(node_id);


	       DAGNode * node = dm.createNode("sign_extend",v_operands,(void*)new_v);

	       $$ = node;
	     }
;


expr : t_LeftParen Let t_LeftParen t_Var expr t_RightParen expr t_RightParen 
             { 
               /*cout<<"  Matched a let expr\n";*/
		
		DAGNode *to_replace;
		if((to_replace = dm.getNodeWithLabel($4->name)) == NULL)
			{
			cout<<"\nError\n";
	    		to_replace = dm.createNode($4->name);
			}
	  	if(to_replace==NULL)
	    		cout<<"ERROR !! Found a null node \n";
	       
	       DAGNode *node_replace_by = $5; 

		*outfile<<"\ndag is originally \n";
		dm.printDAG($7, outfile);
		*outfile<<"\nReplacing occurances of \n";
		dm.printDAG(to_replace, outfile);
		*outfile<<"\nby\n";
		dm.printDAG(node_replace_by, outfile);
		*outfile<<"\ndag becomes\n";
		

	       bool success = dm.ReplaceLeafByDAG(to_replace, node_replace_by); 
	       
	       if(!success)
		 {
		   cout<<"\nError in ReplaceLeafByDAG call in expr1.y. Look at the LOG file\n";
		   exit(1);
		 }

		dm.printDAG($7, outfile);

		success = dm.deleteDAG(to_replace); 
	       
	       if(!success)
		 {
		   cout<<"\nError in deleteDAG call in expr1.y. Look at the LOG file\n";
		   exit(1);
		 }
	       deleted_node_count++;
	       if(deleted_node_count>=MAX_DELETED_COUNT)
			{
			int garbage_nodes = dm.collect_garbage();
			cout<<"Successfully collected "<<garbage_nodes<<" number of garbage nodes\n";
			deleted_node_count=0;
			}
		

	       $$ = $7;

	     }
;


expr : t_LeftParen Flet t_LeftParen t_Var expr t_RightParen expr t_RightParen 
             { 
               /*cout<<"  Matched a flet expr\n";*/
		
		DAGNode *to_replace;
		if((to_replace = dm.getNodeWithLabel($4->name)) == NULL)
			{
			cout<<"\nError\n";
	    		to_replace = dm.createNode($4->name);
			}
	  	if(to_replace==NULL)
	    		cout<<"ERROR !! Found a null node \n";
	       
	       DAGNode *node_replace_by = $5; 

		*outfile<<"\ndag is originally \n";
		dm.printDAG($7, outfile);
		*outfile<<"\nReplacing occurances of \n";
		dm.printDAG(to_replace, outfile);
		*outfile<<"\nby\n";
		dm.printDAG(node_replace_by, outfile);
		*outfile<<"\ndag becomes\n";
		

	       bool success = dm.ReplaceLeafByDAG(to_replace, node_replace_by);
	       
	       if(!success)
		 {
		   cout<<"\nError in ReplaceLeafByDAG call in expr1.y. Look at the LOG file\n";
		   exit(1);
		 }

		dm.printDAG($7, outfile);

		success = dm.deleteDAG(to_replace); 
				
	       if(!success)
		 {
		   cout<<"\nError in deleteDAG call in expr1.y. Look at the LOG file\n";
		   exit(1);
		 }

	       deleted_node_count++;
	       if(deleted_node_count>=MAX_DELETED_COUNT)
			{
			int garbage_nodes = dm.collect_garbage();
			cout<<"Successfully collected "<<garbage_nodes<<" number of garbage nodes\n";
			deleted_node_count=0;
			}
		
		

	       $$ = $7;

	     }
;


exprlist: expr { 
                /*cout<<"  Matched an exprlist with expr\n";*/
	       vector<DAGNode *> v;
	       v.push_back($1);
	       class VectorOfExpressions *s1=new VectorOfExpressions(v);
	       $$=s1;
	
	       }
             |
          exprlist expr
	     {
	       
	       vector<DAGNode *> v;
	       v=assign_expressions($1);
	       v.push_back($2);
	       class VectorOfExpressions *s1=new VectorOfExpressions(v);
	       $$=s1;
	       /*cout<<"  Matched an exprlist with expr exprlist\n";*/

	     }
;



expr     : t_Identifier
	{
	  //Create a leaf node here
	  /*cout<<"  Found an identifier "<<$1->name<<endl;*/
	  DAGNode * node;
	  
	  if((node = dm.getNodeWithLabel($1->name)) == NULL)
	    node = dm.createNode($1->name);
	  if(node==NULL)
	    cout<<"ERROR !! Found a null node \n";
	  $$ = node;

          if(code_to_handle_assumptions)
                {
                map < DAGNode*, DAGNode* >::iterator assum_it;
                assum_it = assumption_table.find (node);
                if (assum_it != assumption_table.end()) 
                    {
                    $$ = assum_it->second;
                    }
                 }
	}

        |
	
	t_Var
	{
	  //Create a leaf node here
	  /*cout<<"  Found a var "<<$1->name<<endl;*/
	  DAGNode * node;
	  
	  if((node = dm.getNodeWithLabel($1->name)) == NULL)
	    node = dm.createNode($1->name);
	  if(node==NULL)
	    cout<<"ERROR !! Found a null node \n";
	  $$ = node;
	}

	|

        t_AbstractLit
	{
        if(LIA_Input) /* Input is LIA. All integer constants should be treated as bit-vector constants with the specified width */
            {
              /*cout<<"  Found an abstract literal "<<$1->name<<". Converting this to a bit-vector constant of width "<<WidthInteger<<endl;*/

              string bvconstant = ($1->name);
              
              checkIfPositiveNumberIsFitting(bvconstant);

               if(LIA_Input_Signed)
                    {
                    bvconstant = shiftNonNegativeToPositiveSide(bvconstant);
                    }

              int width = WidthInteger;

              /*cout<<"\n"<<bvconstant<<"\t"<<width<<"\n";*/

              DAGNode *node;

              if((node = dm.getNodeWithLabel(bvconstant)) == NULL)
              {
                    Value *v = new Value();
                    Type t;
                    t.data_type = BITVECTOR_CONSTANT;
                    t.width = width;
                    node_counter++;
                    string node_id = "unique_node_no_" + integerToString(node_counter);

                    v->set_node_type(t);
                    v->set_unique_node_identifier(node_id);			 

                    node = dm.createNode(bvconstant, (void*)v);
              }
              if(node==NULL)
                cout<<"  ERROR !! Found a null node \n";

              $$ = node;
        }
        else /* Normal bit-vector input. Abstract literal should be treated as abstract literals itself */
        {
          /*cout<<"  Found a abstract literal "<<$1->name<<endl;*/


          DAGNode *node;

          if((node = dm.getNodeWithLabel($1->name)) == NULL)
          {
                Value *v = new Value();
                Type t;
                t.data_type = INTEGER_CONSTANT;
                t.width = IMMATERIAL_WIDTH;
                node_counter++;
                string node_id = "unique_node_no_" + integerToString(node_counter);

                v->set_node_type(t);
                v->set_unique_node_identifier(node_id);			 

                node = dm.createNode($1->name, (void*)v);
          }
          if(node==NULL)
            cout<<"  ERROR !! Found a null node \n";

          $$ = node;
          }
	}

         |

        t_BvLit_Dec t_LeftBrack t_AbstractLit t_RightBrack
	{
	  /*cout<<"  Found a bitvector literal "<<$1->name<<" in decimal base "<<endl;*/
	  string bvconstant = ($1->name);
          

	  bvconstant = bvconstant.substr(2);
	  int width = atoi($3->name);

	  /*cout<<"\n"<<bvconstant<<"\t"<<width<<"\n";*/
	  
	  DAGNode *node;
	  
	  if((node = dm.getNodeWithLabel(bvconstant)) == NULL)
	  {
	    	Value *v = new Value();
		Type t;
		t.data_type = BITVECTOR_CONSTANT;
		t.width = width;
		node_counter++;
		string node_id = "unique_node_no_" + integerToString(node_counter);
			 
		v->set_node_type(t);
		v->set_unique_node_identifier(node_id);			 
		
	    	node = dm.createNode(bvconstant, (void*)v);
	  }
	  if(node==NULL)
	    cout<<"  ERROR !! Found a null node \n";

	  $$ = node;
	}

        |

        t_BoolLit 
	{
	  /*cout<<"  Found a boolean literal "<<$1->name<<endl;*/

	  
	  DAGNode *node;
	  
	  if((node = dm.getNodeWithLabel($1->name)) == NULL)
	  {
	    	Value *v = new Value();
		Type t;
		t.data_type = LOGICAL_CONSTANT;
		t.width = IMMATERIAL_WIDTH;
		node_counter++;
		string node_id = "unique_node_no_" + integerToString(node_counter);
			 
		v->set_node_type(t);
		v->set_unique_node_identifier(node_id);			 
		
	    	node = dm.createNode($1->name, (void*)v);
	  }
	  if(node==NULL)
	    cout<<"  ERROR !! Found a null node \n";

	  $$ = node;
	}
        
        |

        t_LeftParen t_UnaryMinus t_AbstractLit t_RightParen
	{
        if(LIA_Input) /* Input is LIA. All integer constants should be treated as bit-vector constants with the specified width */
            {
              /*cout<<"  Found an abstract literal ~"<<$3->name<<". Converting this to a bit-vector constant of width "<<WidthInteger<<endl;*/

              string bvconstant = ($3->name);

              checkIfNegativeNumberIsFitting(bvconstant);

              if(LIA_Input_Signed)
                    {
                    bvconstant = shiftNegativeToPositiveSide(bvconstant);
                    }
               else   
                    {
                    bvconstant = getAdditiveInverse(bvconstant);
                    }

              int width = WidthInteger;

              /*cout<<"\n"<<bvconstant<<"\t"<<width<<"\n";*/

              DAGNode *node;

              if((node = dm.getNodeWithLabel(bvconstant)) == NULL)
              {
                    Value *v = new Value();
                    Type t;
                    t.data_type = BITVECTOR_CONSTANT;
                    t.width = width;
                    node_counter++;
                    string node_id = "unique_node_no_" + integerToString(node_counter);

                    v->set_node_type(t);
                    v->set_unique_node_identifier(node_id);			 

                    node = dm.createNode(bvconstant, (void*)v);
              }
              if(node==NULL)
                cout<<"  ERROR !! Found a null node \n";

              $$ = node;
        }
        else /* Normal bit-vector input. Not allowed */
        {
          cout<<"\n~ operator is not a valid bit-vector operator. Please set -LIA_Input and -WidthInteger parameters if you want to give LIA input\n";
          exit(1);
          }
	}



;
%%
extern FILE* yyout;

void yyerror(char* str)
{
  fprintf( yyout, "ERROR from parser: %s\n", str );
}

//actually the following function has to be defined the expression simplifier..................
void * copy_dag_addtional_fields(void *fields_to_copy) 
{
	Value *value_to_copy = (Value*)fields_to_copy;
	Value *copied_value = value_to_copy->copy_value();
	return (void*)copied_value;
}

/*
int main(int argc, char** argv)
{
  

        extern FILE *yyin;
	FILE *fp = fopen(argv[1], "r");
	yyin = fp;
	yyparse();
	return 0;
}*/
