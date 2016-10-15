#ifndef __SELCONASSGN_H
#define __SELCONASSGN_H

#include "CConSignAssgnStmt.h"
#include "CExpression.h"

#include <list>
#include <iostream>

using namespace std;

class CSelConSignAssgnStmt:public CConSignAssgnStmt
{
	CExpression *selection_expression;
	list<CExpression*> choices;
	list<CExpression*> values;
public:
	CSelConSignAssgnStmt();
	CSelConSignAssgnStmt(COperand *p_lhs,
			CExpression *selection_expression,
			list<CExpression*> &choices,
			list<CExpression*> &values,
			string dm);

	virtual void print();
	virtual string getSelExpr();
	virtual CExpression* getOriginalSelExpr();
	virtual list<CExpression*> getValues();
	virtual list<CExpression*> getChoices();

};

#endif
