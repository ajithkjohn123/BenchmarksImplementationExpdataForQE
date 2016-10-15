#ifndef __CONASSGN_H
#define __CONASSGN_H

#include "COperand.h"
#include "CExpression.h"
#include <list>
#include <iostream>

using namespace std;

class CExpression;

class CConSignAssgnStmt
{
protected:
	COperand *lhs;
	string delay_mechanism;
public:
	CConSignAssgnStmt();
	CConSignAssgnStmt(COperand *p_lhs, string dm);
	COperand *getLHS();
	string getDelay();
	virtual void print();
	virtual string getSelExpr();
	virtual CExpression* getOriginalSelExpr();
	virtual list<CExpression*> getValues();
	virtual list<CExpression*> getChoices();
};

#endif
