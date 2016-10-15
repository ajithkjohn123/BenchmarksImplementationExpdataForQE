#include "CConSignAssgnStmt.h"
#include "COperand.h"


CConSignAssgnStmt::CConSignAssgnStmt(COperand *p_lhs, string dm)
{
	this->lhs = p_lhs;
	delay_mechanism = dm;
}


CConSignAssgnStmt::CConSignAssgnStmt()
{
	
}

COperand *CConSignAssgnStmt::getLHS()
{
	return lhs;
}

void CConSignAssgnStmt::print()
{
	cout<<endl<<"ERROR -- this function should not be called -- CConSignAssgnStmt::print()"<<endl;
}

string CConSignAssgnStmt::getSelExpr()
{
	return "";
}

CExpression* CConSignAssgnStmt::getOriginalSelExpr()
{
	return NULL;
}


list<CExpression*> CConSignAssgnStmt::getValues()
{
}

list<CExpression*> CConSignAssgnStmt::getChoices()
{
}

string CConSignAssgnStmt::getDelay()
{
	return delay_mechanism;
}

