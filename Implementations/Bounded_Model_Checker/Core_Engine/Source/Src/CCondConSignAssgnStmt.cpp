#include "CCondConSignAssgnStmt.h"

CCondConSignAssgnStmt::CCondConSignAssgnStmt()
{
}

CCondConSignAssgnStmt::CCondConSignAssgnStmt(COperand *lhs, 
			list<CExpression*> &condList,
			list<CExpression*> &exprList,
			string dm)
			:CConSignAssgnStmt(lhs, dm)
{
	this->conditions.splice(this->conditions.begin(), condList);
	this->values.splice(this->values.begin(), exprList);
}

void CCondConSignAssgnStmt::print()
{
	//printf("\n*** concurrent conditional signal assignment ***\n");
	//cout<<endl<<"Target: "<<endl;
	//lhs->print();
	
	//cout<<endl<<"lhs->getName: "<<lhs->getName()<<endl;
	//cout<<endl<<"lhs->getOper: "<<lhs->getOper()<<endl;
	list<CExpression*>::iterator iterCond;
	list<CExpression*>::iterator iterVal;
	int i;
	for(i=1, iterCond = conditions.begin(), iterVal = values.begin();
	iterCond != conditions.end() && iterVal != values.end();
	iterCond++,iterVal++,i++)
	{
		//cout<<"Condition_"<<i<<":"<<endl;
		//(*iterCond)->print(0);

		//cout<<"Value_"<<i<<":"<<endl;
		//(*iterVal)->print(0);
	}
}

string CCondConSignAssgnStmt::getSelExpr()
{
	return "";
}

CExpression* CCondConSignAssgnStmt::getOriginalSelExpr()
{
	return NULL;
}

list<CExpression*> CCondConSignAssgnStmt::getValues()
{
	return values;
}

list<CExpression*> CCondConSignAssgnStmt::getChoices()
{
	return conditions;
}




