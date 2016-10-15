#ifndef __STRUCT_H
#define __STRUCT_H

#include "CExpression.h"
#include "COperand.h"
#include "CSeqStmt.h"

//#include "ExprOper.h"

#define SIGNAL_ASSIGNMENT 1
#define VAR_ASSIGNMENT 2

class CExpression;
class CSeqStmt;
class COperand;

struct pair_upper_lower 
{
    int upper_type;
    int lower_type;
    char *upper_name;
    char *lower_name;
};

struct pair_arch_entity 
{
    char *arch_name;
    char *entity_name;
};

struct expr_oper 
{
   bool is_oper;
   union {
    COperand *oper;
    CExpression *expr;
   } exp_or_op; 
};


struct pair_waveform_expression 
{
    struct expr_oper *wf;
    struct expr_oper *expr;
};

struct sequential_statement 
{
	CSeqStmt *stmt;
};

struct SymTabEntry 
{
	string id_type;
	string data_type;
	CExpression *value;
	list<string> rangeindex;
};


#endif




