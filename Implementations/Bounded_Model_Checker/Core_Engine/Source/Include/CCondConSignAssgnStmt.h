#ifndef __CONDCONASSGN_H
#define __CONDCONASSGN_H

#include "CConSignAssgnStmt.h"
#include "CExpression.h"

#include <list>
#include <iostream>

using namespace std;

class CCondConSignAssgnStmt:public CConSignAssgnStmt
{
	list<CExpression*> conditions;
	list<CExpression*> values;
public:
	CCondConSignAssgnStmt(); 

	CCondConSignAssgnStmt(	COperand *lhs, 
				list<CExpression*> &condList,
				list<CExpression*> &exprList,
				string dm);
	virtual void print();
	virtual string getSelExpr();
	virtual CExpression* getOriginalSelExpr();
	virtual list<CExpression*> getValues();
	virtual list<CExpression*> getChoices();

};

#endif
