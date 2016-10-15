#ifndef EXPROPER_H_
#define EXPROPER_H_

#include "CExpression.h"

struct expr_oper 
{
   bool is_oper;
   union {
    COperand *oper;
    CExpression *expr;
   } exp_or_op; 
};

#endif /*EXPROPER_H_*/
