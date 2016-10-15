%{
#include <stdio.h>
#include <list>
#include <map>
#include <stack>

#include <string>
#include <iostream>
#include <algorithm>

#include "idinfo.h"
#include "structure.h"

#include "CIdentifier.h"
#include "CEntity.h"
#include "CPort.h"
#include "CParser.h"
#include "CCondConSignAssgnStmt.h"
#include "CSelConSignAssgnStmt.h"

#include "CAssgnStmt.h"
#include "CSeqStmt.h"
#include "CCaseStmt.h"
#include "CProcess.h"
#include "CWaitStmt.h"
#include "CWhileStmt.h"
#include "CInstance.h"

#include <string.h>
#include <stdlib.h>

#define t_ENUM 9999
#define type_function 9998

/*#include "y.tab.h"*/

using namespace std;


void yyerror(char *);
extern "C"
{
	int yylex(void);  
	int yywrap()
        {
                return 1;
        }
}


list<CPort> genportList;
list<CPort> portList, genericList;

list<string> idList;
list<struct IDinfo *> senList;
list<string> idWaitList;

list< pair_upper_lower > rangeList;

extern map <string, map<string, SymTabEntry> > SymbolTable;

extern map<string, string> EntArch;

list<CExpression*> condchoicesList;
list<CExpression*> valuesList;

list<CExpression*> paramList;
list<COperand*> formalList;

list<CSeqStmt *> SeqStmtList;

struct expr_oper * FunctionArg;

int ProcessCount=0;

// list<CPort> bufferPorts;

list<list<CExpression*> > st_condchoicesList;
list<list<CExpression*> > st_valuesList;
list<list<CSeqStmt*> > st_SeqStmtList;

list<CInstance *> instantNodes;
list<CProcess *> processNodes;
list<CConSignAssgnStmt *> conAssignNodes;

vector<CEntity> Entities;

map<string, SymTabEntry> IDEntries;

string ArchitectureEntity;


%}

%union{
  struct IDinfo *ds_Id;
  struct pair_arch_entity *archEntPair;

  int value;

  struct pair_upper_lower *boundPair;
  struct expr_oper *exprOper; 

  struct pair_waveform_expression *waveformExpression;

  struct sequential_statement *seqStmt;
}

%token
t_ACCESS
t_AFTER
t_ALIAS
t_ALL
t_AND
t_ARCHITECTURE
t_ARRAY
t_ASSERT
t_ATTRIBUTE
t_BEGIN
t_BLOCK
t_BODY
t_BUFFER
t_BUS
t_CASE
t_COMPONENT
t_CONFIGURATION
t_CONSTANT
t_DISCONNECT
t_DOWNTO
t_ELSE
t_ELSIF
t_END
t_ENTITY
t_EXIT
t_FILE
t_FOR
t_FUNCTION
t_GENERATE
t_GENERIC
t_GUARDED
t_IF
t_IN
t_INOUT
t_IS
t_LABEL
t_LIBRARY

t_LINKAGE
t_LOOP
t_MAP
t_NAND
t_NEW
t_NEXT
t_NOR
t_NULL
t_OF
t_REGISTER
t_ON
t_OPEN
t_OR
t_OTHERS
t_OUT
t_PACKAGE
t_PORT
t_PROCEDURE
t_PROCESS
t_RANGE
t_RECORD
t_REGISTERport
t_REPORT
t_RETURN
t_SELECT
t_SEVERITY
t_SIGNAL
t_SUBTYPE
t_THEN
t_TO
t_TRANSPORT
t_TYPE
t_UNITS
t_UNTIL
t_USE
t_VARIABLE
t_WAIT
t_WHEN
t_WHILE
t_WITH
t_XOR
t_XNOR

t_INERTIAL



/* VHDL binary operators
  ======================= */

%nonassoc t_EQSym t_NESym t_LTSym t_LESym t_GTSym t_GESym
%left t_SLL t_SRL t_SLA t_SRA t_ROL t_ROR
%left t_Plus t_Minus t_Ampersand
%left MED_PRECEDENCE
%left t_Star t_Slash t_MOD t_REM


%nonassoc t_DoubleStar t_ABS t_NOT MAX_PRECEDENCE

/* misc syms */
/*t_Space */
%token t_Apostrophe
t_LeftParen
t_RightParen
t_Comma
t_VarAsgn
t_Colon
t_Semicolon



t_Arrow
t_Box
t_Bar
t_Dot

%token  < ds_Id > t_Identifier
t_AbstractLit
t_CharacterLit

t_StringLit
t_BitStringLit


%type <ds_Id> entity_start literal mark subtype_indic target name sensitivity_list_2 designator sel_name suffix wait_sensitivity_list_2 name2 ifts_name enumeration_literal constrained_array_definition subtype_indic1 type_definition attribute_name physical_literal_no_default

%type <value> mode interf_element_2 interf_element_1 direction object_class opts opts_2 t_TRANSPORT t_INERTIAL

%type <exprOper> primary relation expr interf_element_4 signal_decl_2 and_relation or_relation xor_relation wavefrm  wavefrm_element wavefrm_element_1 choices choice while_scheme loop_stat_2 iteration_scheme wait_stat_2 wait_stat_3 association_list_2 association_element actual_part constant_decl_1 variable_decl_1 wavefrm_1 wavefrm_2

%type <boundPair> range_spec discrete_range gen_association_element gen_association_list_2 discrete_range1 index_constraint_2

%type <archEntPair> arch_start

%type <waveformExpression> condal_wavefrms_2 sel_wavefrms_2

%type <seqStmt> seq_stats seq_stat seq_stats_1 seq_stats_2 case_stat if_stat signal_assign_stat variable_assign_stat wait_stat loop_stat null_stat

%%
start: design_file
;design_file     : design_unit_list
;design_unit_list: design_unit
 | design_unit_list design_unit  

;designator : t_Identifier
	{
		$$ = $1;
	}	
 | t_StringLit  

;literal    : t_AbstractLit  
	{
		$$ = $1;	
	}
 | t_CharacterLit  
	{
		$$ = $1;	
	}
 | t_BitStringLit
	{
		$$ = $1;	
	}
 | physical_literal_no_default  
	{
		$$ = $1;
	}
 | t_NULL  
	{
		struct IDinfo* tn = (struct IDinfo*) malloc(sizeof(struct IDinfo));	

		tn->len=strlen("null");
		tn->name= strdup("null");
		tn->type= t_StringLit;
	
		tn->upper = NULL;
		tn->lower = NULL;
		tn->unit = NULL;

		$$ = tn;	
	}


;enumeration_literal  : t_CharacterLit
	{
		$$ = $1;
	}
 | t_Identifier
	{
		$$ = $1;
	}  

;physical_literal     : physical_literal_1 t_Identifier  
;physical_literal_1   :  
 | t_AbstractLit  
;physical_literal_no_default    : t_AbstractLit t_Identifier  
	{
		$1->unit = $2->name;
		$$ = $1;
	}


;idf_list : t_Identifier 
	{
		idList.push_back(($1)->name);
	}
 | idf_list t_Comma t_Identifier 
	{
		idList.push_back(($3)->name);
	}


/*------------------------------------------
--  Desing Unit
--------------------------------------------*/

;design_unit     : context_list lib_unit  
;context_list    :  
 | context_list context_item  

;lib_unit    : entity_decl  
 | config_decl  
 | package_decl  
 | arch_body  
 | package_body  

;context_item    : lib_clause  
 | use_clause  

;lib_clause  : t_LIBRARY idf_list t_Semicolon  
	{
		idList.clear();
	}

;use_clause : t_USE sel_list t_Semicolon  

;sel_list : sel_name		
 | sel_list t_Comma sel_name	

/*------------------------------------------
--  Library Units
--------------------------------------------*/

;entity_decl : entity_start entity_decl_1 entity_decl_2
entity_decl_3 entity_decl_4 t_END entity_decl_41 entity_decl_5 t_Semicolon 
	{
		string name = ($1)->name;
		//printf ("\n*****Entity found %s****\n", name.c_str());
		//e1.setName(($1)->name);

		//cout<<"\n----port list---\n";
		//CParser::printPort(portList);
		CParser::insertPortToSymbolTable(($1)->name, portList);

		//cout<<"\n----generic list---"<<endl;
		//CParser::printPort(genericList);
		
		CParser::insertPortToSymbolTable(name, portList);
		CParser::insertPortToSymbolTable(name, genericList);

		CEntity e1(name, portList, genericList);

		Entities.push_back(e1);

		portList.clear();
		genericList.clear();
		idList.clear();
		
	}

;entity_start: t_ENTITY t_Identifier t_IS	
	{
		$$ = $2;
	}
;entity_decl_5 :  
 | t_Identifier	
;entity_decl_4 :  
 | t_BEGIN concurrent_stats  
;entity_decl_3 :  
 | entity_decl_3 entity_decl_6  
;entity_decl_6 : entity_decltve_item  
;entity_decl_2 :
	{
		portList.clear();
	}
 | t_PORT interf_list t_Semicolon  
	{
		portList.splice(portList.begin(), genportList);
		genportList.clear();
	}

;entity_decl_41:
 | t_ENTITY	

;entity_decl_1 :
	{
		genericList.clear();
	}
 | t_GENERIC interf_list t_Semicolon  
	{
		genericList.splice(genericList.begin(), genportList);
		
		genportList.clear();
	}


;arch_body : arch_start arch_body_1 t_BEGIN concurrent_stats t_END arch_body_11
arch_body_2 t_Semicolon 
	{
		string ent  = $1->entity_name; 
		string arch = $1->arch_name;
			//cout<<EntArch[ent]<<"***"<<arch<<endl;
			int hui;
			//cin>>hui;

//		if(EntArch[ent] == arch)
//		{
			//cout<<"--------------------------"<<endl;
			//cout<<"got matching entity arch"<<endl;
			//cout<<"--------------------------"<<endl;
		
			int index = CParser::getEntity(ent);
			//cout<<"--------------------------"<<endl;
			//cout<<index<<endl;
			//cout<<"--------------------------"<<endl;

			Entities[index].setProcesses(processNodes);
			//cout<<"--------------------------"<<endl;
			//cout<<processNodes.size()<<endl;
			//cout<<"--------------------------"<<endl;

			Entities[index].setConAssgnStmt(conAssignNodes);
			//cout<<"--------------------------"<<endl;
			//cout<<conAssignNodes.size()<<endl;
			//cout<<"--------------------------"<<endl;

			Entities[index].setInstances(instantNodes);
			//cout<<"--------------------------"<<endl;
			//cout<<instantNodes.size()<<endl;
			//cout<<"--------------------------"<<endl;

//			map<string, SymTabEntry> ids = SymbolTable[ent];
//			copy(IDEntries.begin(), IDEntries.end(), SymbolTable[ent].end());

			map<string, SymTabEntry> t_IDEntries;
			t_IDEntries = SymbolTable[ent];

			SymbolTable.erase(ent);
	
			t_IDEntries.insert(IDEntries.begin(), IDEntries.end());
			
			SymbolTable.insert(make_pair(ent, t_IDEntries));
			
			IDEntries.clear();
			t_IDEntries.clear();
			processNodes.clear();
			conAssignNodes.clear();
			instantNodes.clear();
//		}
//		else
//		{
//			// delete everything
//		}

	}
;arch_start : t_ARCHITECTURE t_Identifier t_OF t_Identifier t_IS 
	{
		struct pair_arch_entity *pae = (struct pair_arch_entity *) 
				malloc(sizeof(struct pair_arch_entity));
		pae->arch_name = strdup(($2)->name);
		pae->entity_name = strdup(($4)->name);
		$$ = pae;
		ArchitectureEntity = pae->entity_name;
	}

;arch_body_11 :
;arch_body_11 : t_ARCHITECTURE
;arch_body_2  :
;arch_body_2  : t_Identifier  
;arch_body_1  :  
;arch_body_1  : arch_body_1 arch_body_3  
;arch_body_3  : block_decltve_item  

;config_decl : config_start config_decl_1 block_config t_END config_decl_2 t_Semicolon
 /* ;config_start : t_CONFIGURATION t_Identifier t_OF mark t_IS 
    replaced: */
;config_start : t_CONFIGURATION t_Identifier t_OF t_Identifier t_IS 
;config_decl_2    :  
;config_decl_2    : t_Identifier  
;config_decl_1    :  
;config_decl_1    : config_decl_1 config_decl_3  
;config_decl_3    : config_decltve_item  

;package_decl  : package_start package_decl_1 t_END package_decl_2 t_Semicolon  
;package_start : t_PACKAGE t_Identifier t_IS 
;package_decl_2     :  
;package_decl_2     : t_Identifier  
;package_decl_1     :  
;package_decl_1     : package_decl_1 package_decl_3  
;package_decl_3     : package_decltve_item  

;package_body    : pack_body_start package_body_1 t_END package_body_2 t_Semicolon  
;pack_body_start : t_PACKAGE t_BODY t_Identifier t_IS 
;package_body_2  :  
;package_body_2  : t_Identifier  
;package_body_1  :  
;package_body_1  : package_body_1 package_body_3  
;package_body_3  : package_body_decltve_item  

/*------------------------------------------
--  Declarative Item
--------------------------------------------*/
;common_decltve_item   : type_decl  
	{
	}

;common_decltve_item   : subtype_decl  
	{
		
	}

;common_decltve_item   : constant_decl  
	{
		
	}

;common_decltve_item   : file_decl  
;common_decltve_item   : alias_decl  
;common_decltve_item   : subprog_decl  
;common_decltve_item   : use_clause  

;entity_decltve_item   : common_decltve_item  
;entity_decltve_item   : subprog_body  
;entity_decltve_item   : attribute_decl  
;entity_decltve_item   : attribute_spec  
;entity_decltve_item   : disconnection_spec  
;entity_decltve_item   : signal_decl  

;block_decltve_item    : common_decltve_item  
;block_decltve_item    : subprog_body  
;block_decltve_item    : comp_decl  
	{
		
	}

;block_decltve_item    : attribute_decl  
;block_decltve_item    : attribute_spec  
;block_decltve_item    : config_spec  
;block_decltve_item    : disconnection_spec  
;block_decltve_item    : signal_decl  
	{
		
	}

;package_decltve_item  : common_decltve_item  
;package_decltve_item  : comp_decl  
;package_decltve_item  : attribute_decl  
;package_decltve_item  : attribute_spec  
;package_decltve_item  : disconnection_spec  
;package_decltve_item  : signal_decl  

;package_body_decltve_item  : common_decltve_item  
;package_body_decltve_item  : subprog_body  

;subprog_decltve_item    : common_decltve_item  
;subprog_decltve_item    : subprog_body  
;subprog_decltve_item    : attribute_decl  
;subprog_decltve_item    : attribute_spec  
;subprog_decltve_item    : variable_decl  

;procs_decltve_item  : common_decltve_item  
;procs_decltve_item  : subprog_body  
;procs_decltve_item  : attribute_decl  
;procs_decltve_item  : attribute_spec  
;procs_decltve_item  : variable_decl  

;config_decltve_item : attribute_spec  
;config_decltve_item : use_clause  
/*------------------------------------------
--  Subprograms
--------------------------------------------*/

;subprog_decl    : subprog_spec t_Semicolon  

;subprog_spec  : t_PROCEDURE t_Identifier subprog_spec_1  
;subprog_spec  : t_FUNCTION designator subprog_spec_2 t_RETURN mark  
;subprog_spec_2     :  
;subprog_spec_2     : interf_list  
;subprog_spec_1     :  
;subprog_spec_1     : interf_list  

;subprog_body : subprog_spec t_IS subprog_body_1 t_BEGIN seq_stats t_END subprog_body_2 t_Semicolon  
;subprog_body_2    :  
;subprog_body_2    : designator  
;subprog_body_1    :  
;subprog_body_1    : subprog_body_1 subprog_body_3  
;subprog_body_3    : subprog_decltve_item  

/*--------------------------------------------------
--  Interface Lists and Associaton Lists
----------------------------------------------------*/

;interf_list  : t_LeftParen interf_element interf_list_1 t_RightParen  
;interf_list_1     :  
;interf_list_1     : interf_list_1 interf_list_2  
;interf_list_2     : t_Semicolon interf_element  

;interf_element    : interf_element_1 idf_list t_Colon interf_element_2 subtype_indic interf_element_3 interf_element_4  
	{
		//cout<<1<<endl;
		struct expr_oper *val=NULL;
		if($7 != NULL)
		{
			if($7->is_oper)
			{
				val = CParser::convertToExpression($7);
			}
			else
			{
				val = $7;
			}
		}

		string type = $5->name;
			
		int objClass = $1;
			
		list<string> upper;
		list<string> lower;
		list< pair_upper_lower >::iterator irange;
			
		for (irange=rangeList.begin(); 
		irange!=rangeList.end(); 
		irange++)
		{
			upper.push_back((*irange).upper_name);
			lower.push_back((*irange).lower_name);
		}

		list<string>::iterator iterID;
		for (iterID=idList.begin(); 
		iterID!=idList.end(); 
		iterID++)
		{	
			list<string> t_upper;
			list<string> t_lower;

			list<string>::iterator iterList;

			for (iterList = upper.begin();
				iterList != upper.end();
				iterList++)
			{
				t_upper.push_back(*iterList);
			}
			for (iterList = lower.begin();
				iterList != lower.end();
				iterList++)
			{
				t_lower.push_back(*iterList);
			}

			CPort p;
			p.setName((*iterID).c_str());
			p.setMode($4);
			if(val!=NULL)
			{
				p.setValue(val->exp_or_op.expr);
			}
			else
			{
				p.setValue(NULL);
			}
			p.setUpper(t_upper);
			p.setLower(t_lower);
			p.setType(type);

			//cout<<"  "<<(*iterID);
			genportList.push_back(p);
		}

		idList.clear();
		rangeList.clear();
		//cout<<2<<endl;
	}
;interf_element_4  : 
	{
		$$ = NULL;
	} 
;interf_element_4  : t_VarAsgn expr  
	{
		$$ = $2;
	}
;interf_element_3  :  
;interf_element_3  : t_BUS
;interf_element_2  :  
	{
		$$ = t_IN;
	}
;interf_element_2  : mode  
	{
		$$ = $1;
	}

;interf_element_1  :  
	{
		$$ = t_SIGNAL;
	}
;interf_element_1  : object_class  
	{
		$$ = $1;
	}
;mode  : t_IN  
	{
		$$ = t_IN;
	}
;mode  : t_OUT 
	{
		$$ = t_OUT;
	}
;mode  : t_INOUT  
	{
		$$ = t_INOUT;
	}
;mode  : t_BUFFER  
	{
		$$ = t_BUFFER;
	}
;mode  : t_LINKAGE
	{
		$$ = t_LINKAGE;
	}
  

;association_list     : t_LeftParen association_element association_list_1 t_RightParen
	{
		//cout<<"association_list     : t_LeftParen association_element association_list_1 t_RightParen" <<endl;

		
		struct expr_oper *assoc_ele;
		if($2->is_oper)
		{
			assoc_ele = CParser::convertToExpression($2);
		}
		else
		{
			assoc_ele = $2;
		}

		paramList.push_front(assoc_ele->exp_or_op.expr);
	}
;association_list_1   :  
;association_list_1   : association_list_1 association_list_2  
	{
		struct expr_oper *assoc_ele;
		if($2->is_oper)
		{
			assoc_ele = CParser::convertToExpression($2);
		}
		else
		{
			assoc_ele = $2;
		}

		paramList.push_back(assoc_ele->exp_or_op.expr);
	}

;association_list_2   : t_Comma association_element  
	{
		$$ = $2;
	}

;gen_association_list : t_LeftParen gen_association_element gen_association_list_1 t_RightParen  
	{
		rangeList.push_front(*($2));
	}

;gen_association_list_1    :  
;gen_association_list_1    : gen_association_list_1 gen_association_list_2  
	{
		rangeList.push_back(*($2));
	}

;gen_association_list_2    : t_Comma gen_association_element  
	{
		$$ = $2;
	}


;association_element  : formal_part t_Arrow actual_part  
	{
		$$ = $3;
	}

;association_element  : actual_part
	{
		$$ = $1;
	}
  

/* changed ;gen_association_element   : association_element  */
;gen_association_element   : expr
	{
		FunctionArg = $1;
		struct pair_upper_lower *pul = (struct pair_upper_lower*) 
						malloc(sizeof(pair_upper_lower));

		if($1->is_oper)
		{
			struct expr_oper *eo = $1;
			
			CLiteral* oper = (CLiteral*)(eo->exp_or_op).oper;
			
			pul->upper_name=strdup((oper->getValue()).c_str());
	
			pul->upper_type=oper->getType();
	
			pul->lower_name=strdup((oper->getValue()).c_str());
	
			pul->lower_type=oper->getType();
		}

		$$ = pul;
	}
;gen_association_element   : discrete_range1  
	{
		$$ = $1;
	}

;formal_part     : name	
	{
	  
	  
	  COperand* op =  CParser::createOperand($1->name); 
	  //COperand* op =  CParser::createOperand($1); //This was the earlier line
		//cout<<"\n\nHERE2\n\n"; //New
		//op->print(); // New
		formalList.push_back(op);
	}  

;actual_part     : expr  
	{
		$$ = $1;
	}
;actual_part     : t_OPEN  

/*--------------------------------------------------
--  Names and Expressions
----------------------------------------------------*/

;mark  : t_Identifier	
	{
		$$ = $1;
	}
;mark  : sel_name  
	{
		$$ = $1;
	}


;expr : and_relation
	{
		$$ = $1;
		//cout<< "expr : and_relation" <<endl;
	}
  
;expr : or_relation
	{
		$$ = $1;
	}
  
;expr : xor_relation
	{
		$$ = $1;
	}
  
;expr : relation  
	{
		$$ = $1;
	}

;expr : relation t_NAND relation  
	{
		CExpression* expr =  CParser::createExpression
					("nand", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;expr : relation t_NOR relation  
	{
		CExpression* expr =  CParser::createExpression
					("nor", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;expr : relation t_XNOR relation  
	{
		CExpression* expr =  CParser::createExpression
					("xnor", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;and_relation    : relation t_AND relation  
	{
		CExpression* expr =  CParser::createExpression
					("and", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
		//cout<< "relation t_AND relation  " << endl;
	}

;and_relation    : and_relation t_AND relation  
	{
		CExpression* expr =  CParser::createExpression
					("and", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;or_relation     : relation t_OR relation  
	{
		CExpression* expr =  CParser::createExpression
					("or", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;or_relation     : or_relation t_OR relation  
	{
		CExpression* expr =  CParser::createExpression
					("or", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;xor_relation    : relation t_XOR relation  
	{
		CExpression* expr =  CParser::createExpression
					("xor", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;xor_relation    : xor_relation t_XOR relation  
	{
		CExpression* expr =  CParser::createExpression
					("xor", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}


/* ;relation   : unary_operator primary   */
;relation   : primary  
	{
		$$ = $1;
		//cout<<";relation   : primary"<<endl;
	}

;relation   : t_Plus primary  %prec MED_PRECEDENCE
	{
		CExpression* expr =  CParser::createExpression
					("+", $2, NULL, true);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : t_Minus primary %prec MED_PRECEDENCE
	{
		CExpression* expr =  CParser::createExpression
					("-", $2, NULL, true);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : t_ABS primary   
	{
		CExpression* expr =  CParser::createExpression
					("abs", $2, NULL, true);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : t_NOT primary   
	{
		CExpression* expr =  CParser::createExpression
					("not", $2, NULL, true);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : primary t_DoubleStar primary
	{
		struct expr_oper *eo = (struct expr_oper*)malloc(sizeof(struct expr_oper));

		;
		if(CParser::isConstFolding($1, $3))
		{
			COperand* oper =  CParser::doConstFolding("**", $1, $3);

			eo->is_oper = true;
			(eo->exp_or_op).oper = oper;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
					("**", $1, $3, false);
			
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
		}

		$$ = eo;
	}

/* ;    relation : relation binary_operator primary */

;relation   : relation t_MOD relation
	{
		CExpression* expr =  CParser::createExpression
					("mod", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_REM relation
	{
		CExpression* expr =  CParser::createExpression
					("rem", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_Ampersand relation
	{
		CExpression* expr =  CParser::createExpression
					("&", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_Star relation
	{
		struct expr_oper *eo = (struct expr_oper*)malloc(sizeof(struct expr_oper));

		;
		if(CParser::isConstFolding($1, $3))
		{
			COperand* oper =  CParser::doConstFolding("*", $1, $3);

			eo->is_oper = true;
			(eo->exp_or_op).oper = oper;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
					("*", $1, $3, false);
			
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
		}

		$$ = eo;
	}
;relation   : relation t_Plus relation
	{
		struct expr_oper *eo = (struct expr_oper*)malloc(sizeof(struct expr_oper));

		;
		if(CParser::isConstFolding($1, $3))
		{
			COperand* oper =  CParser::doConstFolding("+", $1, $3);

			eo->is_oper = true;
			(eo->exp_or_op).oper = oper;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
					("+", $1, $3, false);
			
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
		}

		$$ = eo;
	}
;relation   : relation t_Minus relation
	{
		struct expr_oper *eo = (struct expr_oper*)malloc(sizeof(struct expr_oper));

		;
		if(CParser::isConstFolding($1, $3))
		{
			COperand* oper =  CParser::doConstFolding("-", $1, $3);

			eo->is_oper = true;
			(eo->exp_or_op).oper = oper;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
					("-", $1, $3, false);
			
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
		}

		$$ = eo;
	}
;relation   : relation t_LESym relation
	{
		CExpression* expr =  CParser::createExpression
					("<=", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_GESym relation
	{
		CExpression* expr =  CParser::createExpression
					(">=", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_LTSym relation
	{
		CExpression* expr =  CParser::createExpression
					("<", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_GTSym relation
	{
		CExpression* expr =  CParser::createExpression
					(">", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_EQSym relation
	{
		CExpression* expr =  CParser::createExpression
					("=", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_NESym relation
	{
		CExpression* expr =  CParser::createExpression
					("/=", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}
;relation   : relation t_Slash relation
	{
		struct expr_oper *eo = (struct expr_oper*)malloc(sizeof(struct expr_oper));

		;
		if(CParser::isConstFolding($1, $3))
		{
			COperand* oper =  CParser::doConstFolding("/", $1, $3);

			eo->is_oper = true;
			(eo->exp_or_op).oper = oper;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
					("/", $1, $3, false);
			
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
		}

		$$ = eo;
	}

/******************     Newly added shift operators - START     **************************/
;relation   : relation t_SLL relation
	{
		CExpression* expr =  CParser::createExpression
					("sll", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;relation   : relation t_SRL relation
	{
		CExpression* expr =  CParser::createExpression
					("srl", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;relation   : relation t_SLA relation
	{
		CExpression* expr =  CParser::createExpression
					("sla", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;relation   : relation t_SRA relation
	{
		CExpression* expr =  CParser::createExpression
					("sra", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;relation   : relation t_ROL relation
	{
		CExpression* expr =  CParser::createExpression
					("rol", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}

;relation   : relation t_ROR relation
	{
		CExpression* expr =  CParser::createExpression
					("ror", $1, $3, false);

		struct expr_oper *eo = (struct expr_oper*) 
		malloc(sizeof(struct expr_oper));

		eo->is_oper = false;
		(eo->exp_or_op).expr = expr;
		$$ = eo;
	}


/******************     Newly added shift operators - END     **************************/

;primary    : name  
	{
//********************changed	
		//cout<<"###########"<<endl;
		//cout<<$1->name<<endl;		
		//cout<<"###########"<<endl;
		string ap = "'";

		if(ap == $1->name)
		{
			//cout<<"ap "<<endl;
			struct expr_oper *nm = (struct expr_oper*) malloc(sizeof(struct expr_oper));

			nm->is_oper = true;
			nm->exp_or_op.oper = CParser::createOperand($1->lower);
			struct expr_oper *dsg = (struct expr_oper*) malloc(sizeof(struct expr_oper));
			dsg->is_oper = true;

			dsg->exp_or_op.oper = CParser::createOperand($1->upper);
			struct expr_oper *attr = (struct expr_oper*) malloc(sizeof(struct expr_oper));

			attr->is_oper = false;
			attr->exp_or_op.expr = CParser::createExpression("'", nm, dsg, false);

			$$ = attr;
		}
		else if($1->type == type_function)
		{
			//cout<<"type_function "<<endl;
			struct expr_oper *nm = (struct expr_oper*) malloc(sizeof(struct expr_oper));

			nm->is_oper = true;
			nm->exp_or_op.oper = CParser::createOperand($1->name);


			struct expr_oper *attr = (struct expr_oper*) malloc(sizeof(struct expr_oper));

			attr->is_oper = false;
			attr->exp_or_op.expr = CParser::createExpression("function", nm, FunctionArg, false);

			$$ = attr;
		}
		else
		{
			//cout<<"else 1 "<<endl;
			COperand* op =  CParser::createOperand($1);
			struct expr_oper *eo = (struct expr_oper*) 
				malloc(sizeof(struct expr_oper));
			eo->is_oper = true;
			(eo->exp_or_op).oper = op;
			$$ = eo;
			//cout<<"else  2"<<endl;
		}
		//cout<<";primary    : name  "<<endl;
	}

;primary    : literal  
	{
		COperand* op =  CParser::createOperand($1);
		struct expr_oper *eo = (struct expr_oper*) 
			malloc(sizeof(struct expr_oper));
		eo->is_oper = true;
		(eo->exp_or_op).oper = op;
		$$ = eo;
	}

;primary    : aggregate  
;primary    : qualified_expr  
;primary    : allocator  
;primary    : t_LeftParen expr t_RightParen  
	{
		$$ = $2;
	}

/*************changed***************/	


/***********************************/	

;name  : mark  
	{
		$$ = $1;
	}
;name  : name2  
	{
		$$ = $1;
	}

;name2 : t_StringLit

;name2 : attribute_name  

	{
		$$ = $1;
	}

;name2 : ifts_name  
	{
		$$ = $1;
	}


;sel_name   : name t_Dot suffix  
	{
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		string str = $1->name;
		str = str + ".";
		str = str + $3->name;

		id->len  = str.length();
		id->name = strdup(str.c_str());
		id->type = t_Identifier;

		$$ = id;
	}

;suffix     : designator  
	{
		$$ = $1;
	}
;suffix     : t_CharacterLit
	{
		$$ = $1;
	}

;suffix     : t_ALL
	{
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		string str = "all";

		id->len  = str.length();
		id->name = strdup(str.c_str());
		id->type = t_Identifier;

		$$ = id;
	}

;ifts_name  : mark gen_association_list  
	{
		//cout<<"yayayayayaya"<<endl;
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	


		id->len=strlen($1->name);
		id->name= $1->name;


/**************************************/
// TODO - Fix thois bug, we have to search in IDEntries as well as in port list.

// FIXED - TESTING IS REMAINING

		//cout<< "// FIXED - TESTING IS REMAINING " <<endl;
		//cout<<SymbolTable[ArchitectureEntity].size()<<"  "<< $1->name <<endl;
		//cout<< "// FIXED - TESTING IS REMAINING " <<endl;

		if(IDEntries.find($1->name) == IDEntries.end() 
		&& SymbolTable[ArchitectureEntity].find($1->name) == SymbolTable[ArchitectureEntity].end())
		{
			id->type= type_function;
			//cout<< "id->type= type_function; " <<endl;
		}
		else
		{	
			//cout<< "id->type= t_Identifier; " <<endl;
			id->type= t_Identifier;
		}
		
		//id->type= t_Identifier;
		
/******************************/		

		list< pair_upper_lower >::iterator irange;
		irange=rangeList.begin();

	  	id->upper = strdup((*irange).upper_name );
	  	id->lower = strdup((*irange).lower_name );
	  	id->unit = NULL;

		rangeList.clear();

		$$ = id;
	}
;ifts_name  : name2 gen_association_list  

;attribute_name  : mark t_Apostrophe t_Identifier  

	{

		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		id->len=strlen("'");
	  	id->name= strdup("'");
	  	id->type= t_Identifier;

	  	id->upper = strdup($3->name);
	  	id->lower = strdup($1->name);

		$$ = id;
	}

;attribute_name  : name2 t_Apostrophe t_Identifier  
;attribute_name : mark t_Apostrophe t_RANGE
;attribute_name : name2 t_Apostrophe t_RANGE

;aggregate  : element_association_list2 t_RightParen  
;aggregate  : t_LeftParen choices t_Arrow expr t_RightParen  
	{
			//cout<<"nanananana"<<endl;
	}

;element_association_list2 : t_LeftParen element_association t_Comma element_association  
;element_association_list2 : element_association_list2 t_Comma element_association  

;qualified_expr : mark t_Apostrophe t_LeftParen expr t_RightParen  
;qualified_expr : mark t_Apostrophe aggregate  


;allocator  : t_NEW mark mark allocator_1  
;allocator  : t_NEW mark allocator_2  
;allocator  : t_NEW qualified_expr  
;allocator_2     :  
;allocator_2     : gen_association_list  
;allocator_1     :  
;allocator_1     : gen_association_list  


/*--------------------------------------------------
--  Element Association and Choices
----------------------------------------------------*/

;element_association  : choices t_Arrow expr  
;element_association  : expr  

;choices    : choice choices_1  
		{
			$$ = $1;
		}
;choices_1  :  
;choices_1  : choices_1 choices_2  
;choices_2  : t_Bar choice  

;choice     : expr  
		{
			$$ = $1;
		}
;choice     : discrete_range1  
		{
		//cout<< "yayayaya" <<endl;
			struct pair_upper_lower *pul = $1;
			struct IDinfo *low, *high;
	  		low = (struct IDinfo*) malloc(sizeof(struct IDinfo));	

	  		high = (struct IDinfo*) malloc(sizeof(struct IDinfo));	

	  		low->name = pul->lower_name;
	  		low->type = pul->lower_type;
			
	  		high->name = pul->upper_name;
	  		high->type = pul->upper_type;

			struct expr_oper *low_eo = (struct expr_oper*) malloc(sizeof(struct expr_oper));

			struct expr_oper *high_eo = (struct expr_oper*) malloc(sizeof(struct expr_oper));

			low_eo->is_oper = true;
			high_eo->is_oper = true; 

			low_eo->exp_or_op.oper = CParser::createOperand
					(low);
			high_eo->exp_or_op.oper = CParser::createOperand
					(high); 
			


			CExpression* expr =  CParser::createExpression
						("range_in", low_eo, high_eo, false);

			struct expr_oper *eo = (struct expr_oper*) 
			malloc(sizeof(struct expr_oper));

			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
			$$ = eo;
		}

;choice     : t_OTHERS  
		{
			struct expr_oper *eo = CParser::createExpression("others"); 
			$$ = eo;
		}
/*--------------------------------------------------
--	Type Declarations
----------------------------------------------------*/

;type_decl     : t_TYPE t_Identifier t_IS type_definition t_Semicolon 
	{
		//////////// I have removed imcomplete type defiction by remove 
		/*type_decl_1   : */

		string idtype = $4->name;

		if($4->upper == NULL && $4->lower == NULL && idtype == "enum")
		{
			SymTabEntry entry;
			string name = ($2)->name;
			entry.id_type = "enum";
			entry.data_type = "enum";
			entry.value = NULL;
			entry.rangeindex.splice(entry.rangeindex.begin(), idList);
		
			IDEntries[name]=entry;
		}
		else if($4->upper == NULL && $4->lower == NULL && idtype == "range")
		{
			SymTabEntry entry;
			string name = ($2)->name;
			entry.id_type = "range";
			entry.data_type = "range";
			entry.value = NULL;

			list<string> rlist;
			list<pair_upper_lower> :: iterator irange=rangeList.begin(); 
			 
			rlist.push_back((*irange).lower_name);
			rlist.push_back((*irange).upper_name);

			entry.rangeindex.splice(entry.rangeindex.begin(), rlist);
			
			IDEntries[name]=entry;
		}
		else if($4->upper != NULL && $4->lower != NULL)
		{
			SymTabEntry entry;
			string name = ($2)->name;
			entry.id_type = "array";
			entry.data_type = idtype;
			entry.value = NULL;
			entry.rangeindex.push_back($4->lower);
			entry.rangeindex.push_back($4->upper);
			
			IDEntries[name]=entry;
		}
		else
		{
			cout<<"unsupport type of "<<($2)->name<<endl;
			exit(1);
		}
		idList.clear();
		rangeList.clear();
	}

;type_definition : enumeration_type_definition  
	{
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		
		id->len=strlen("enum");
	  	id->name= strdup("enum");
		id->upper = NULL;
    		id->lower = NULL;

		$$ = id;
	}

;type_definition : range_constraint
	{
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		
		id->len=strlen("range");
	  	id->name= strdup("range");
		id->upper = NULL;
    		id->lower = NULL;

		$$ = id;
	}
  
;type_definition : physical_type_definition  
;type_definition : unconstrained_array_definition  

;type_definition : constrained_array_definition
	{
		$$ = $1;
	}

;type_definition : record_type_definition  
;type_definition : access_type_definition  
;type_definition : file_type_definition  

;enumeration_type_definition    : t_LeftParen enumeration_literal enumeration_type_definition_1 t_RightParen  
	{
		idList.push_front(($2)->name);
	}
	
;enumeration_type_definition_1  :  
;enumeration_type_definition_1  : enumeration_type_definition_1 enumeration_type_definition_2  
;enumeration_type_definition_2  : t_Comma enumeration_literal  
	{
		idList.push_back(($2)->name);
	}

;physical_type_definition : range_constraint t_UNITS base_unit_decl
physical_type_definition_1 t_END t_UNITS 
;physical_type_definition_1     :  
;physical_type_definition_1     : physical_type_definition_1 physical_type_definition_2  
;physical_type_definition_2     : secondary_unit_decl  

;base_unit_decl     : t_Identifier t_Semicolon  

;secondary_unit_decl     : t_Identifier t_EQSym physical_literal t_Semicolon  

;unconstrained_array_definition : t_ARRAY t_LeftParen
index_subtype_definition unconstrained_array_definition_1 t_RightParen t_OF
subtype_indic 
;unconstrained_array_definition_1    :  
;unconstrained_array_definition_1    : unconstrained_array_definition_1 unconstrained_array_definition_2  
;unconstrained_array_definition_2    : t_Comma index_subtype_definition  

;index_subtype_definition  : mark t_RANGE t_Box  

;constrained_array_definition   : t_ARRAY index_constraint t_OF subtype_indic  
	{
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		
		id->len=strlen($4->name);
	  	id->name= $4->name;
	  	id->type= t_Identifier;

		list< pair_upper_lower >::iterator irange;
		irange=rangeList.begin();

	  	id->upper = strdup((*irange).upper_name );
	  	id->lower = strdup((*irange).lower_name );

		rangeList.clear();

		$$ = id;
	}

;record_type_definition : t_RECORD element_decl record_type_definition_1 t_END
t_RECORD 
;record_type_definition_1  :  
;record_type_definition_1  : record_type_definition_1 record_type_definition_2  
;record_type_definition_2  : element_decl  

;element_decl  : idf_list t_Colon subtype_indic t_Semicolon  

;access_type_definition    : t_ACCESS subtype_indic  

;file_type_definition : t_FILE t_OF mark  

/*--------------------------------------------------
--  Subtypes and Constraints
----------------------------------------------------*/

;subtype_decl  : t_SUBTYPE t_Identifier t_IS subtype_indic t_Semicolon  
	{
		string idtype = $4->name;

		SymTabEntry entry;
		string name = ($2)->name;
		entry.id_type = idtype;
		entry.data_type = idtype;
		entry.value = NULL;
		//entry.rangeindex.splice(entry.rangeindex.begin(), idList);
		entry.rangeindex.push_back($4->lower);
		entry.rangeindex.push_back($4->upper);


		IDEntries[name]=entry;

		idList.clear();
		rangeList.clear();	
	}

;subtype_indic   : mark subtype_indic_1  
	{
//			$$ = $1;
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		
		id->len=strlen($1->name);
	  	id->name= $1->name;
	  	id->type= t_Identifier;

		list< pair_upper_lower >::iterator irange;
		irange=rangeList.begin();

	  	id->upper = strdup((*irange).upper_name );
	  	id->lower = strdup((*irange).lower_name );

		//rangeList.clear();

		$$ = id;

	}

;subtype_indic   : subtype_indic1  
;subtype_indic_1 :  
;subtype_indic_1 : gen_association_list  

;subtype_indic1  : mark mark range_constraint  
;subtype_indic1  : mark range_constraint  
	{
//			$$ = $1;
		struct IDinfo* id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		
		id->len=strlen($1->name);
	  	id->name= $1->name;
	  	id->type= t_Identifier;

		list< pair_upper_lower >::iterator irange;
		irange=rangeList.begin();

	  	id->upper = strdup((*irange).upper_name );
	  	id->lower = strdup((*irange).lower_name );

		rangeList.clear();

		$$ = id;
	}

;subtype_indic1  : mark mark subtype_indic1_1  
;subtype_indic1_1     :  
;subtype_indic1_1     : gen_association_list  

;range_constraint     : t_RANGE range_spec  
		{
			rangeList.clear();
			rangeList.push_back(*($2));
		}

;index_constraint     : t_LeftParen discrete_range index_constraint_1 t_RightParen  
		{
			rangeList.push_front(*($2));
		}
;index_constraint_1   :  
;index_constraint_1   : index_constraint_1 index_constraint_2  
		{
			rangeList.push_back(*($2));
		}
;index_constraint_2   : t_Comma discrete_range  
		{
			$$ = $2;
		}

;discrete_range  : subtype_indic  
;discrete_range  : range_spec  
		{
			$$ = $1;
		}

;discrete_range1 : subtype_indic1  
;discrete_range1 : expr direction expr  
		{
		
			//cout<<"*/*/*/*/"<<endl;
			struct pair_upper_lower *pul = (struct pair_upper_lower*) malloc(sizeof(pair_upper_lower));;
			if($2 == t_TO)
			{
				
				pul->upper_name=strdup((((CLiteral*)($3->exp_or_op.oper))->getValue()).c_str());

				pul->upper_type=
					($3->exp_or_op.oper)->getType();

				//cout<<"#######"<<pul->upper_name<<endl;


				pul->lower_name=strdup((((CLiteral*)($1->exp_or_op.oper))->getValue()).c_str());

				pul->lower_type=
					($1->exp_or_op.oper)->getType();

				//cout<<"#######"<<pul->lower_name<<endl;
			}
			else
			{
				pul->upper_name=strdup((((CLiteral*)($1->exp_or_op.oper))->getValue()).c_str());

				pul->upper_type=
					($1->exp_or_op.oper)->getType();;

				//cout<<"#######"<<pul->upper_name<<endl;

				pul->lower_name=strdup((((CLiteral*)($3->exp_or_op.oper))->getValue()).c_str());

				pul->lower_type=
					($3->exp_or_op.oper)->getType();

				//cout<<"#######"<<pul->lower_name<<endl;
			}
			$$ = pul;
		}

;range_spec  : attribute_name  
;range_spec  : expr direction expr  
	{

		//cout<<"//////////////////"<<endl;
		struct pair_upper_lower *pul = (struct pair_upper_lower*) malloc(sizeof(pair_upper_lower));;
		if($2 == t_TO)
		{
			
			pul->upper_name=strdup((((CLiteral*)($3->exp_or_op.oper))->getValue()).c_str());

			//cout<<"#######"<<pul->upper_name<<endl;

			pul->upper_type=
				($3->exp_or_op.oper)->getType();


			pul->lower_name=strdup((((CLiteral*)($1->exp_or_op.oper))->getValue()).c_str());

			pul->lower_type=
				($1->exp_or_op.oper)->getType();

			//cout<<"#######"<<pul->lower_name<<endl;
		}
		else
		{
			pul->upper_name=strdup((((CLiteral*)($1->exp_or_op.oper))->getValue()).c_str());

			pul->upper_type=
				($1->exp_or_op.oper)->getType();;

			//cout<<"#######"<<pul->upper_name<<endl;

			pul->lower_name=strdup((((CLiteral*)($3->exp_or_op.oper))->getValue()).c_str());

			pul->lower_type=
				($3->exp_or_op.oper)->getType();

			//cout<<"#######"<<pul->lower_name<<endl;
		}
		$$ = pul;
	}

;direction  : t_TO
	{
		$$ = t_TO;
	}
;direction  : t_DOWNTO  
	{
		$$ = t_DOWNTO;
	}

/*--------------------------------------------------
--  Objects, Aliases, Files, Disconnections
----------------------------------------------------*/

;constant_decl : t_CONSTANT idf_list t_Colon subtype_indic constant_decl_1 t_Semicolon  
	{
		//cout<<10<<endl;
		struct expr_oper *value = NULL;
		if($5 != NULL)
		{
			if($5->is_oper)
			{
				value = CParser::convertToExpression($5);
			}
			else
			{
				value = $5;
			}
		}

		list<string>::iterator id;
		for (id=idList.begin(); 
		id!=idList.end(); 
		id++)
		{
			list<string> t_range;
			//cout<<112<<endl;
			if(rangeList.size())
			{
				list< pair_upper_lower >::iterator irange = rangeList.begin();
	
				//copy(rangeList.begin(), rangeList.end(), t_range);
	
				t_range.push_back((*irange).lower_name);
				t_range.push_back((*irange).upper_name);
			}

			SymTabEntry entry;
			string name = (*id);
			entry.id_type = "constant";
			entry.data_type = $4->name;
			if(value!=NULL)
			{
				entry.value = value->exp_or_op.expr;
			}
			else
			{
				entry.value = NULL;
			}

			
			entry.rangeindex.splice(entry.rangeindex.begin(), t_range);

			IDEntries[name]=entry;
		}

		idList.clear();
		rangeList.clear();
		//cout<<20<<endl;
		
	}

;constant_decl_1    :  
	{
		$$=NULL;
	}

;constant_decl_1    : t_VarAsgn expr  
	{
		$$ = $2;
	}


;signal_decl   : t_SIGNAL idf_list t_Colon subtype_indic signal_decl_1 signal_decl_2 t_Semicolon  
	{
		//cout<<11<<endl;
		struct expr_oper *value = NULL;
		if($6 != NULL)
		{
			if($6->is_oper)
			{
				value = CParser::convertToExpression($6);
			}
			else
			{
				value = $6;
			}
		}
		//cout<<111<<endl;
		list<string>::iterator id;
		for (id=idList.begin(); 
		id!=idList.end(); 
		id++)
		{
			list<string> t_range;
			//cout<<112<<endl;
			if(rangeList.size())
			{
				list< pair_upper_lower >::iterator irange = rangeList.begin();
	
				//copy(rangeList.begin(), rangeList.end(), t_range);
	
				t_range.push_back((*irange).lower_name);
				t_range.push_back((*irange).upper_name);
			}
			else
			{
				t_range.push_back("0");
				t_range.push_back("0");
			}

			//cout<<113<<endl;
			SymTabEntry entry;
			string name = (*id);
			entry.id_type = "signal";
			entry.data_type = $4->name;
			
			if(value!=NULL)
			{
				entry.value = value->exp_or_op.expr;
			}
			else
			{
				entry.value = NULL;
			}

			entry.rangeindex.splice(entry.rangeindex.begin(), t_range);

			IDEntries[name]=entry;

//			upper.push_back((*irange).upper_name);
//			lower.push_back((*irange).lower_name);
		}

		idList.clear();
		rangeList.clear();
		//cout<<21<<endl;	
		
/*		int objClass = t_SIGNAL;
		
		list<string> upper;
		list<string> lower;
		list< pair_upper_lower >::iterator irange;
		
		for (irange=rangeList.begin(); 
		irange!=rangeList.end(); 
		irange++)
		{
			upper.push_back((*irange).upper_name);
			lower.push_back((*irange).lower_name);
		}
		
		list<string>::iterator iterID;
		for (iterID=idList.begin(); 
		iterID!=idList.end(); 
		iterID++)
		{	
			CPort p;
			p.setName((*iterID).c_str());
			p.setMode(objClass);
			p.setValue(val);
			p.setUpper(upper);
			p.setLower(lower);
			p.setType(type);

			//cout<<"  "<<(*iterID);
			genportList.push_back(p);
		}
*/
	}

;signal_decl_2 :  
	{
		$$=NULL;
	}
;signal_decl_2 : t_VarAsgn expr  
	{
		$$ = $2;
	}
;signal_decl_1 :  
;signal_decl_1 : signal_kind  

;variable_decl : t_VARIABLE idf_list t_Colon subtype_indic variable_decl_1 t_Semicolon
	{
		//cout<<999999999<<endl;
		struct expr_oper *value = NULL;
		if($5 != NULL)
		{
			//cout<<3333<<endl;
			if($5->is_oper)
			{
				//cout<<66<<endl;
				value = CParser::convertToExpression($5);
			}
			else
			{
				//cout<<88<<endl;
				value = $5;
			}
		}
		//cout<<888888888<<endl;
		list<string>::iterator id;
		for (id=idList.begin(); 
		id!=idList.end(); 
		id++)
		{
			list<string> t_range;
			//cout<<112<<endl;
			if(rangeList.size())
			{
				list< pair_upper_lower >::iterator irange = rangeList.begin();
	
				//copy(rangeList.begin(), rangeList.end(), t_range);
	
				t_range.push_back((*irange).lower_name);
				t_range.push_back((*irange).upper_name);
			}
			else
			{
				t_range.push_back("0");
				t_range.push_back("0");
			}

			SymTabEntry entry;
			string name = (*id);
			entry.id_type = "variable";
			entry.data_type = $4->name;


			if(value!=NULL)
			{
				entry.value = value->exp_or_op.expr;
			}
			else
			{
				entry.value = NULL;
			}

			//entry.value = value->exp_or_op.expr;
			entry.rangeindex.splice(entry.rangeindex.begin(), t_range);

			IDEntries[name]=entry;

//			upper.push_back((*irange).upper_name);
//			lower.push_back((*irange).lower_name);
		}

		idList.clear();
		rangeList.clear();
	}
  
;variable_decl_1    :  
	{
		$$=NULL;
	}
;variable_decl_1    : t_VarAsgn expr  
	{
		$$ = $2;
	}

;object_class    : t_CONSTANT  
	{
		$$ = t_CONSTANT;
	}

;object_class    : t_SIGNAL  
	{
		$$ = t_SIGNAL;
	}

;object_class    : t_VARIABLE  
	{
		$$ = t_VARIABLE;
	}

;signal_kind     : t_BUS  
;signal_kind     : t_REGISTER  

;alias_decl    : t_ALIAS t_Identifier t_Colon subtype_indic t_IS name t_Semicolon  

;file_decl     : t_FILE t_Identifier t_Colon subtype_indic t_IS file_decl_1 expr t_Semicolon  
;file_decl_1   :  
;file_decl_1   : mode  

;disconnection_spec    : t_DISCONNECT signal_list t_Colon mark t_AFTER expr t_Semicolon  

;signal_list     : name signal_list_1  
;signal_list     : t_OTHERS  
;signal_list     : t_ALL  
;signal_list_1   :  
;signal_list_1   : signal_list_1 signal_list_2  
;signal_list_2   : t_Comma name  

/*--------------------------------------------------
--  Attribute Declarations and Specifications
----------------------------------------------------*/

;attribute_decl     : t_ATTRIBUTE t_Identifier t_Colon mark t_Semicolon  

;attribute_spec   : t_ATTRIBUTE t_Identifier t_OF entity_spec t_IS expr t_Semicolon  

;entity_spec : entity_name_list t_Colon entity_class  

;entity_name_list     : designator entity_name_list_1  
;entity_name_list     : t_OTHERS  
;entity_name_list     : t_ALL  
;entity_name_list_1   :  
;entity_name_list_1   : entity_name_list_1 entity_name_list_2  
;entity_name_list_2   : t_Comma designator  

;entity_class    : t_ENTITY
;entity_class    : t_ARCHITECTURE  
;entity_class    : t_PACKAGE 
;entity_class    : t_CONFIGURATION
;entity_class    : t_COMPONENT
;entity_class    : t_LABEL  
;entity_class    : t_TYPE
;entity_class    : t_SUBTYPE  
;entity_class    : t_PROCEDURE  
;entity_class    : t_FUNCTION  
;entity_class    : t_SIGNAL
;entity_class    : t_VARIABLE
;entity_class    : t_CONSTANT  

/*--------------------------------------------------
--  Schemes
----------------------------------------------------*/

;generation_scheme    : if_scheme  
;generation_scheme    : for_scheme  

;iteration_scheme     : for_scheme  
;iteration_scheme     : while_scheme  

;if_scheme  : t_IF expr  

;for_scheme : t_FOR t_Identifier t_IN discrete_range  
	{
		//printf("\n$$$$Please run the preprocessor before running the parser.$$$$\n");
	}

;while_scheme    : t_WHILE expr  
	{
		$$ = $2;
	}

/*--------------------------------------------------
--  Concurrent Statements
----------------------------------------------------*/

;concurrent_stats     : concurrent_stats_1  
;concurrent_stats_1   :  
;concurrent_stats_1   : concurrent_stats_1 concurrent_stats_2  
;concurrent_stats_2   : concurrent_stat  

;concurrent_stat : block_stat  
 | concurrent_assertion_stat  
 | concurrent_procedure_call  
 | concurrent_signal_assign_stat  
 | comp_inst_stat  
 | generate_stat  
 | procs_stat  

;block_stat : t_Identifier t_Colon t_BLOCK block_stat_1 block_stat_2
block_stat_3 block_stat_4 t_BEGIN concurrent_stats t_END t_BLOCK block_stat_5
t_Semicolon 
;block_stat_5    :  
;block_stat_5    : t_Identifier  
;block_stat_4    :  
;block_stat_4    : block_stat_4 block_stat_6  
;block_stat_6    : block_decltve_item  
;block_stat_3    :  
;block_stat_3    : t_PORT interf_list t_Semicolon block_stat_7  
;block_stat_7    :  
;block_stat_7    : t_PORT t_MAP association_list t_Semicolon  
;block_stat_2    :  
;block_stat_2    : t_GENERIC interf_list t_Semicolon block_stat_8  
;block_stat_8    :  
;block_stat_8    : t_GENERIC t_MAP association_list t_Semicolon  
;block_stat_1    :  
;block_stat_1    : t_LeftParen expr t_RightParen  

;comp_inst_stat   : t_Identifier t_Colon mark t_GENERIC t_MAP association_list comp_inst_stat_1 t_Semicolon
	{
		CInstance *cinst = new 
		CInstance($1->name, $3->name, paramList, formalList);

		instantNodes.push_back(cinst);
		paramList.clear();
		cinst->print();
	}
;comp_inst_stat   : t_Identifier t_Colon mark t_PORT t_MAP association_list t_Semicolon  
	{
		CInstance *cinst = new 
		CInstance($1->name, $3->name, paramList, formalList);
		instantNodes.push_back(cinst);
		paramList.clear();
		formalList.clear();
		cinst->print();
	}

;comp_inst_stat_1 :  
;comp_inst_stat_1 : t_PORT t_MAP association_list  

;concurrent_assertion_stat : t_Identifier t_Colon assertion_stat  
;concurrent_assertion_stat : assertion_stat  

;concurrent_procedure_call : t_Identifier t_Colon procedure_call_stat  
;concurrent_procedure_call : procedure_call_stat  

;concurrent_signal_assign_stat    : t_Identifier t_Colon condal_signal_assign  
;concurrent_signal_assign_stat    : condal_signal_assign  
;concurrent_signal_assign_stat    : t_Identifier t_Colon sel_signal_assign  
;concurrent_signal_assign_stat    : sel_signal_assign  

;condal_signal_assign  : target t_LESym opts condal_wavefrms t_Semicolon  
	{
		//cout<<"-------transport"<< t_TRANSPORT <<"  "<<$3 <<endl;
		string delay_mechanism;
		if($3 ==  t_TRANSPORT)
		{
			//cout<<"?????????transport"<<endl;
			delay_mechanism = "transport";
		}
		else if($3 == t_INERTIAL)
		{
			//cout<<"?????????inertial"<<endl;
			delay_mechanism = "inertial";
		}
		//cout<<"-------transportaaaaaaaa"<<endl;
		COperand* lhs =  CParser::createOperand($1);
		//cout<<"-------transportiiiiiiiii"<<endl;	
		CConSignAssgnStmt *ccass = new 
		CCondConSignAssgnStmt(lhs, condchoicesList, valuesList, delay_mechanism);
		//cout<<"-------transportbbbbbbb"<<endl;
		ccass->print();

		conAssignNodes.push_back(ccass);

		condchoicesList.clear();
		valuesList.clear();
		//cout<<"-------transport111111"<<endl;
	}

;condal_wavefrms     : condal_wavefrms_1 wavefrm  
	{ 
		//cout<<"condal_wavefrms     : condal_wavefrms_1 wavefrm  - START" <<endl;
		struct expr_oper *wf;
		if($2->is_oper)
		{
			wf = CParser::convertToExpression($2);
		}
		else
		{
			wf = $2;
		}
		
		//cout<<"1 " <<endl;

		valuesList.push_back((wf->exp_or_op).expr);

		struct IDinfo *others;
		others = (struct IDinfo*) malloc(sizeof(struct IDinfo));	

		//cout<<"2 " <<endl;
		
/*		others->name = strdup("others");;
		others->type = t_OTHERS;
		
		cout<<"2a " <<endl;*/
		struct expr_oper *others_eo = (struct expr_oper*) malloc(sizeof(struct expr_oper));
		//cout<<"2b " <<endl;
		others_eo->is_oper = true;
		//cout<<"2c " <<endl;
		others_eo->exp_or_op.oper = CParser::createOperand
				("others");
		//cout<<"3 " <<endl;
			
		CExpression* expr =  CParser::createExpression
					("others", others_eo, NULL, true);

		condchoicesList.push_back(expr);
		//cout<<"condal_wavefrms     : condal_wavefrms_1 wavefrm  " <<endl;
	}

;condal_wavefrms_1   :  
;condal_wavefrms_1   : condal_wavefrms_1 condal_wavefrms_2  
	{
		condchoicesList.push_back(($2->expr->exp_or_op).expr);
		valuesList.push_back(($2->wf->exp_or_op).expr);
		
		//cout<< "condal_wavefrms_1   : condal_wavefrms_1 condal_wavefrms_2  " <<endl;
	}
;condal_wavefrms_2   : wavefrm t_WHEN expr t_ELSE  
	{
		struct pair_waveform_expression *pwe;

		pwe = (struct pair_waveform_expression*)
		malloc(sizeof(struct pair_waveform_expression));
			
		struct expr_oper *wf;
		struct expr_oper *e;
		if($1->is_oper)
		{
			wf = CParser::convertToExpression($1);
		}
		else
		{
			wf = $1;
		}

		if($3->is_oper)
		{
			e = CParser::convertToExpression($3);
		}
		else
		{
			e = $3;
		}
	
		pwe->wf = wf;
		pwe->expr = e;

		$$ = pwe;
	}


;wavefrm   : wavefrm_element wavefrm_1
	{
		if($2 == NULL)
		{
			$$ = $1;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
						(",", $1, $2, false);
	
			struct expr_oper *eo = (struct expr_oper*) 
			malloc(sizeof(struct expr_oper));
	
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
			$$ = eo;
		}
		//cout<< "wavefrm   : wavefrm_element wavefrm_1 " <<endl;
	}
;wavefrm_1 :  
	{
		$$ = NULL;
	}
;wavefrm_1 : wavefrm_1 wavefrm_2  
	{
		if($1 == NULL)
		{
			$$ = $2;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
						(",", $2, $1, false);
	
			struct expr_oper *eo = (struct expr_oper*) 
			malloc(sizeof(struct expr_oper));
	
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
			$$ = eo;
		}
		
	}

;wavefrm_2 : t_Comma wavefrm_element  
	{
		$$ = $2;
	}

;wavefrm_element     : expr wavefrm_element_1  
	{
		if($2 == NULL)
		{
			$$ = $1;
		}
		else
		{
			CExpression* expr =  CParser::createExpression
						("after", $1, $2, false);
	
			struct expr_oper *eo = (struct expr_oper*) 
			malloc(sizeof(struct expr_oper));
	
			eo->is_oper = false;
			(eo->exp_or_op).expr = expr;
			$$ = eo;
		}
		//cout<< "wavefrm_element     : expr wavefrm_element_1" <<endl;
	}

;wavefrm_element_1   :  
	{
		$$ = NULL;
	}
;wavefrm_element_1   : t_AFTER expr  
	{
		$$ = $2;
	}

;target     : name  
	{
		//cout<<"target     : name"<<endl;
		$$ = $1;
	}
;target     : aggregate 
	{
		//cout<<"target     : aggregate "<<endl;
	} 

;opts    : opts_1 opts_2 
	{
		$$ = $2;
		//cout<<"opts    : opts_1 opts_2 "<<$$ <<endl;
	} 
;opts_2  :  
	{
		$$ = -1;
	}
;opts_2  : t_TRANSPORT 
	{
		
		$$ = t_TRANSPORT;
		//cout<<";opts_2  : t_TRANSPORT "<<$$ <<endl;
	}
;opts_2  : t_INERTIAL
	{
		$$ = t_INERTIAL;
	} 
;opts_1  :  
;opts_1  : t_GUARDED  

;sel_signal_assign : t_WITH expr t_SELECT target t_LESym opts sel_wavefrms t_Semicolon 
	{
		string delay_mechanism;
		if($6 ==  t_TRANSPORT)
		{
			delay_mechanism = "transport";
		}
		else if($6 == t_INERTIAL)
		{
			delay_mechanism = "inertial";
		}

		struct expr_oper *expr;
		if($2->is_oper)
		{
			expr = CParser::convertToExpression($2);
		}
		else
		{
			expr = $2;
		}

		COperand* lhs =  CParser::createOperand($4);
		CConSignAssgnStmt *ccass = new 
		CSelConSignAssgnStmt(lhs,
				(expr->exp_or_op).expr, 
				condchoicesList, 
				valuesList,
				delay_mechanism);

		ccass->print();

		conAssignNodes.push_back(ccass);

		condchoicesList.clear();
		valuesList.clear();

	}

;sel_wavefrms   : sel_wavefrms_1 wavefrm t_WHEN choices  
	{
		struct pair_waveform_expression *pwe;

		pwe = (struct pair_waveform_expression*)
		malloc(sizeof(struct pair_waveform_expression));
			
		struct expr_oper *wf;
		struct expr_oper *e;
		if($2->is_oper)
		{
			wf = CParser::convertToExpression($2);
		}
		else
		{
			wf = $2;
		}

		if($4->is_oper)
		{
			e = CParser::convertToExpression($4);
		}
		else
		{
			e = $4;
		}

		condchoicesList.push_back((e->exp_or_op).expr);
		valuesList.push_back((wf->exp_or_op).expr);
	}

;sel_wavefrms_1 :  
;sel_wavefrms_1 : sel_wavefrms_1 sel_wavefrms_2  
	{
		condchoicesList.push_back(($2->expr->exp_or_op).expr);
		valuesList.push_back(($2->wf->exp_or_op).expr);
	}

;sel_wavefrms_2 : wavefrm t_WHEN choices t_Comma  
	{
		struct pair_waveform_expression *pwe;

		pwe = (struct pair_waveform_expression*)
		malloc(sizeof(struct pair_waveform_expression));
			
		struct expr_oper *wf;
		struct expr_oper *e;
		if($1->is_oper)
		{
			wf = CParser::convertToExpression($1);
		}
		else
		{
			wf = $1;
		}

		if($3->is_oper)
		{
			e = CParser::convertToExpression($3);
		}
		else
		{
			e = $3;
		}
	
		pwe->wf = wf;
		pwe->expr = e;

		$$ = pwe;
	}

;generate_stat : t_Identifier t_Colon generation_scheme t_GENERATE
concurrent_stats t_END t_GENERATE generate_stat_1 t_Semicolon 
;generate_stat_1 :  
;generate_stat_1 : t_Identifier  

;procs_stat    : t_Identifier t_Colon procs_stat1  
;procs_stat    : procs_stat1  

;procs_stat1 : t_PROCESS procs_stat1_1 procs_stat1_2 t_BEGIN seq_stats t_END
t_PROCESS procs_stat1_3 t_Semicolon 
	{
	
		CProcess* proc_stmt;
		if($5 != NULL)
		{
			proc_stmt = new CProcess("", senList, $5->stmt);
		}
		else
		{
			proc_stmt = new CProcess("", senList, NULL);
		}
	
		processNodes.push_back(proc_stmt);
	
		senList.clear();	
	}
;procs_stat1_3 :  
;procs_stat1_3 : t_Identifier  
;procs_stat1_2 :  
;procs_stat1_2 : procs_stat1_2 procs_stat1_4  
;procs_stat1_4 : procs_decltve_item  
;procs_stat1_1 :  
;procs_stat1_1 : t_LeftParen sensitivity_list t_RightParen  

;sensitivity_list     : name sensitivity_list_1
	{
		senList.push_front(($1));
	}
 
;sensitivity_list_1   :  
;sensitivity_list_1   : sensitivity_list_1 sensitivity_list_2  
	{
		senList.push_back(($2));
	}
;sensitivity_list_2   : t_Comma name  
	{
		$$ = $2;
	}

/*-------------------------------------------------------------------
	Newly added by nachiket
*/
;wait_sensitivity_list     : name wait_sensitivity_list_1
	{
		idWaitList.push_front(($1)->name);
	}
 
;wait_sensitivity_list_1   :  
;wait_sensitivity_list_1   : wait_sensitivity_list_1 wait_sensitivity_list_2  
	{
		idWaitList.push_back(($2)->name);
	}
;wait_sensitivity_list_2   : t_Comma name  
	{
		$$ = $2;
	}

/*--------------------------------------------------
--  Sequential Statements
----------------------------------------------------*/

;seq_stats     : seq_stats_1  
	{
	        
		$$ = $1;
		
	}

;seq_stats_1   :
	{
		$$ = NULL;
	}  
;seq_stats_1   : seq_stats_1 seq_stats_2
	{

		if($1 == NULL)
		{
			$$= $2;
		}
		else
		{

			CSeqStmt*last = CParser::getLast($1->stmt);

			last->setNext($2->stmt);

			$$ = $1;
		}

	}

;seq_stats_2   : seq_stat  
	{
		$$ = $1;
	}

;seq_stat : assertion_stat  
;seq_stat : case_stat  
	{
		$$ = $1;
	}
;seq_stat : exit_stat  
;seq_stat : if_stat  
	{
		$$ = $1;
	}
;seq_stat : loop_stat  
	{
		$$ = $1;
	}

;seq_stat : next_stat  
;seq_stat : null_stat  
	{
		$$ = $1;
	}
;seq_stat : procedure_call_stat  
;seq_stat : return_stat  
;seq_stat : signal_assign_stat  
	{
	  
		$$ = $1;
	  
	}
;seq_stat : variable_assign_stat  
	{
		$$ = $1;
	}
;seq_stat : wait_stat  
	{
		$$ = $1;
	}

;assertion_stat  : t_ASSERT expr assertion_stat_1 assertion_stat_2 t_Semicolon  
;assertion_stat_2     :  
;assertion_stat_2     : t_SEVERITY expr  
;assertion_stat_1     :  
;assertion_stat_1     : t_REPORT expr  

;case_stat  : t_CASE {
		st_condchoicesList.push_front(condchoicesList);
		st_SeqStmtList.push_front(SeqStmtList);

		condchoicesList.clear();
		SeqStmtList.clear();
	}
expr t_IS case_stat_alternative case_stat_1 t_END t_CASE t_Semicolon  
	{
		struct sequential_statement *seq_stmt = (struct sequential_statement *) 
				malloc(sizeof(struct sequential_statement));

		struct expr_oper *expr;
		if($3->is_oper)
		{
			expr = CParser::convertToExpression($3);
		}
		else
		{
			expr = $3;
		}

		CSeqStmt *case_stmt = new 
					CCaseStmt(expr->exp_or_op.expr,
						condchoicesList,
						SeqStmtList);

		seq_stmt->stmt = case_stmt;


		CSeqStmt *junction_node = CParser::createAndSetJunctionNode(case_stmt);

		condchoicesList.clear();
		SeqStmtList.clear();

		case_stmt->print();

		$$ = seq_stmt;

		condchoicesList.splice(condchoicesList.begin(), st_condchoicesList.front());
		st_condchoicesList.pop_front();
		
		SeqStmtList.splice(SeqStmtList.begin(), st_SeqStmtList.front());
		st_SeqStmtList.pop_front();
	}
;case_stat_1     :  
;case_stat_1     : case_stat_1 case_stat_2  
;case_stat_2     : case_stat_alternative  

;case_stat_alternative     : t_WHEN choices t_Arrow seq_stats  
	{
		struct expr_oper *e;

		if($2->is_oper)
		{
			e = CParser::convertToExpression($2);
		}
		else
		{
			e = $2;
		}

		condchoicesList.push_back(e->exp_or_op.expr);
		SeqStmtList.push_back($4->stmt);
	}

;exit_stat  : t_EXIT exit_stat_1 exit_stat_2 t_Semicolon  
;exit_stat_2     :  
;exit_stat_2     : t_WHEN expr  
;exit_stat_1     :  
;exit_stat_1     : t_Identifier  

;if_stat    : t_IF {
                //cout<<"\n\nIF started\n\n";
		//cout<<"\n\nInitializaton Started\n\n";
		st_condchoicesList.push_front(condchoicesList);
		st_SeqStmtList.push_front(SeqStmtList);

		condchoicesList.clear();
		SeqStmtList.clear();
		//cout<<"\n\nInitializaton Finished\n\n";
	}
expr  t_THEN seq_stats  if_stat_1  if_stat_2  t_END t_IF t_Semicolon  
	{

	        //cout<<"\n\ncondn THEN ... started\n\n";
		struct sequential_statement *seq_stmt = (struct sequential_statement *) 
				malloc(sizeof(struct sequential_statement));

		struct expr_oper *e;

		//cout<<"\n\nParsing codn part of IF started\n\n";

		if($3->is_oper)
		{
			e = CParser::convertToExpression($3);
		}
		else
		{
			e = $3;
		}

		condchoicesList.push_front(e->exp_or_op.expr);

		//cout<<"\n\nParsing stmt part of IF started\n\n";

		SeqStmtList.push_front($5->stmt);

		CSeqStmt *if_stmt = new 
					CCaseStmt(NULL,
						condchoicesList,
						SeqStmtList);

		seq_stmt->stmt = if_stmt;


		CSeqStmt *junction_node = CParser::createAndSetJunctionNode(seq_stmt->stmt);

		condchoicesList.clear();
		SeqStmtList.clear();
		if_stmt->print();

		$$ = seq_stmt;
		condchoicesList.splice(condchoicesList.begin(), st_condchoicesList.front());
		st_condchoicesList.pop_front();
		
		SeqStmtList.splice(SeqStmtList.begin(), st_SeqStmtList.front());
		st_SeqStmtList.pop_front();
	}

;if_stat_2  :  
;if_stat_2  : t_ELSE seq_stats  
	{
	       //cout<<"\n\nELSE started\n\n";
	       //char *else_ptr = (char*)malloc(5);
	       //strcpy(else_ptr,"else");
	        struct expr_oper *e = CParser::createExpression("else");
		//struct expr_oper *e = CParser::createExpression(else_ptr);
		//cout<<"\n\nSt1 finished\n\n";
		condchoicesList.push_back(e->exp_or_op.expr);
		//cout<<"\n\nSt2 finished\n\n";
		SeqStmtList.push_back($2->stmt);
		//cout<<"\n\nELSE finished\n\n";
	}

;if_stat_1  :  
;if_stat_1  : if_stat_1 if_stat_3  
;if_stat_3  : t_ELSIF expr t_THEN seq_stats  
	{
	        //cout<<"\n\nELSIF started\n\n";
		struct expr_oper *e;

		if($2->is_oper)
		{
			e = CParser::convertToExpression($2);
		}
		else
		{
			e = $2;
		}

		condchoicesList.push_back(e->exp_or_op.expr);
		SeqStmtList.push_back($4->stmt);
		//cout<<"\n\nELSIF finished\n\n";
	}


;loop_stat  : loop_stat_1 loop_stat_2 t_LOOP seq_stats t_END t_LOOP loop_stat_3 t_Semicolon  
	{
		struct sequential_statement *seq_stmt = (struct sequential_statement *) 
				malloc(sizeof(struct sequential_statement));

		struct expr_oper *e;

		if($2->is_oper)
		{
			e = CParser::convertToExpression($2);
		}
		else
		{
			e = $2;
		}

		CSeqStmt *loop_stmt = new CWhileStmt(e->exp_or_op.expr, $4->stmt);

		seq_stmt->stmt = loop_stmt;

		CSeqStmt *junction_node = CParser::createAndSetJunctionNode(seq_stmt->stmt);

		loop_stmt->print();

		$$ = seq_stmt;
	}
;loop_stat_3     :  
;loop_stat_3     : t_Identifier  
;loop_stat_2     :  
;loop_stat_2     : iteration_scheme
	{
		$$ = $1;
	}  
;loop_stat_1     :  
;loop_stat_1     : t_Identifier t_Colon  

;next_stat  : t_NEXT next_stat_1 next_stat_2 t_Semicolon  
;next_stat_2     :  
;next_stat_2     : t_WHEN expr  
;next_stat_1     :  
;next_stat_1     : t_Identifier  

;null_stat  : t_NULL t_Semicolon  
	{
		struct sequential_statement *seq_stmt = (struct sequential_statement *) 
					malloc(sizeof(struct sequential_statement));;

		CSeqStmt *null_stmt = new CSeqStmt();
		seq_stmt->stmt = null_stmt;

		$$ = seq_stmt;
	}


;procedure_call_stat  : name t_Semicolon  

;return_stat     : t_RETURN return_stat_1 t_Semicolon  
;return_stat_1   :  
;return_stat_1   : expr  

;signal_assign_stat    : target t_LESym signal_assign_stat_1 wavefrm t_Semicolon  
		{
		        
			struct sequential_statement *seq_stmt = (struct sequential_statement *) 
					malloc(sizeof(struct sequential_statement));;

			struct expr_oper *wf;
			if($4->is_oper)
			{
				wf = CParser::convertToExpression($4);
			}
			else
			{
				wf = $4;
			}

			COperand* op =  CParser::createOperand($1);

		//	printf("\nTarget\n");
		//	op->print();
		//	printf("\n****  <=  ****\n");
		//	CParser::printExpression(wf->exp_or_op.expr,0);
		//	wf->exp_or_op.expr->print(0);

			CSeqStmt *assign_stmt = new CAssgnStmt(op, wf->exp_or_op.expr, SIGNAL_ASSIGNMENT);
			seq_stmt->stmt = assign_stmt;
			
			$$ = seq_stmt;	
			assign_stmt->print();

			//printf("\n***Signal assignment statement found***\n");
		}
;signal_assign_stat_1  :  
;signal_assign_stat_1  : t_TRANSPORT  

;variable_assign_stat  : target t_VarAsgn expr t_Semicolon  
		{
			struct sequential_statement *seq_stmt = (struct sequential_statement *) 
					malloc(sizeof(struct sequential_statement));;

			struct expr_oper *wf;
			if($3->is_oper)
			{
				wf = CParser::convertToExpression($3);
			}
			else
			{
				wf = $3;
			}

			COperand* op =  CParser::createOperand($1);


			CSeqStmt *assign_stmt = new CAssgnStmt(op, wf->exp_or_op.expr, VAR_ASSIGNMENT);
			seq_stmt->stmt = assign_stmt;
			$$ = seq_stmt;	
			assign_stmt->print();

//			printf("\n***Variable assignment statement found***\n");
		}


;wait_stat  : t_WAIT wait_stat_1 wait_stat_2 wait_stat_3 t_Semicolon  
	{
		struct sequential_statement *seq_stmt = (struct sequential_statement *) 
					malloc(sizeof(struct sequential_statement));;

		CSeqStmt *wait_stmt = new CWaitStmt($3==NULL?NULL:$3->exp_or_op.expr, 
							$4==NULL?NULL:$4->exp_or_op.expr, 
							idWaitList);

		seq_stmt->stmt = wait_stmt;
		$$ = seq_stmt;	

		idWaitList.clear();

		$$ = seq_stmt;
		
		wait_stmt->print();
	}
;wait_stat_3     :  
	{
		$$ = NULL;	
	}
;wait_stat_3     : t_FOR expr  
	{
		struct expr_oper *e;

		if($2->is_oper)
		{
			e = CParser::convertToExpression($2);
			//cout<<"//////////"<<endl;
			e->exp_or_op.expr->print(0);
			//cout<<"//////////"<<endl;
		}
		else
		{
			e = $2;
			//cout<<"//////////"<<endl;
			e->exp_or_op.oper->print();
			//cout<<"//////////"<<endl;
		}

		$$ = e;	
	}
;wait_stat_2     :  
	{
		$$ = NULL;	
	}
;wait_stat_2     : t_UNTIL expr  
	{
		struct expr_oper *e;

		if($2->is_oper)
		{
			e = CParser::convertToExpression($2);
		}
		else
		{
			e = $2;
		}

		$$ = e;	
	}
;wait_stat_1     :  
	{
		idWaitList.clear();	
	}
;wait_stat_1     : t_ON wait_sensitivity_list
/*--------------------------------------------------
--  Components and Configurations
----------------------------------------------------*/

;comp_decl     : t_COMPONENT t_Identifier comp_decl_1 comp_decl_2 t_END t_COMPONENT t_Semicolon  
;comp_decl     : t_COMPONENT t_Identifier t_IS comp_decl_1 comp_decl_2 t_END t_COMPONENT t_Semicolon  
;comp_decl_2   :  
;comp_decl_2   : t_PORT interf_list t_Semicolon  
;comp_decl_1   :  
;comp_decl_1   : t_GENERIC interf_list t_Semicolon  

;block_config  : t_FOR block_spec block_config_1 block_config_2 t_END t_FOR t_Semicolon  
;block_config_2     :  
;block_config_2     : block_config_2 block_config_3  
;block_config_3     : config_item  
;block_config_1     :  
;block_config_1     : block_config_1 block_config_4  
;block_config_4     : use_clause  

;block_spec  : name  

;config_item   : block_config  
;config_item   : comp_config  

;comp_config   : t_FOR comp_spec comp_config_1 comp_config_2 t_END t_FOR t_Semicolon  
;comp_config_2 :  
;comp_config_2 : block_config  
;comp_config_1 :  
;comp_config_1 : t_USE binding_indic t_Semicolon  

;config_spec : t_FOR comp_spec t_USE
binding_indic t_Semicolon 

;comp_spec   : inst_list t_Colon mark  

;inst_list   : idf_list  
;inst_list   : t_ALL  
;inst_list   : t_OTHERS  

;binding_indic   : entity_aspect binding_indic_1 binding_indic_2  
;binding_indic_2 :  
;binding_indic_2 : t_PORT t_MAP association_list  
;binding_indic_1 :  
;binding_indic_1 : t_GENERIC t_MAP association_list  

;entity_aspect   : t_ENTITY name  
;entity_aspect   : t_CONFIGURATION mark  
;entity_aspect : t_OPEN 
;
%%
extern FILE* yyout;

void yyerror(char* str)
{
  fprintf( yyout, "ERROR: %s\n", str );
}

/************************************************************
* RESOLVED BUGS
* J.Gaisler reported:
*       Range attributes in FOR loops are not supported 
*	Unary minus not supported in second argument to relational exp. 
* Thorsten Groetker
*	precedence and associtivity of operators
*	(formal=>actual) was permitted in gen_associatin_list
*		and thus in subtype_indic e.g. 
*************************************************************
* OTHER KNOWN PROBLEMS:
* the grammar allows expressions: 3*-5 thats pretty nice but illegal 
* in VHDL - only 3*(-5) is permitted from LRM.
*************************************************************/

