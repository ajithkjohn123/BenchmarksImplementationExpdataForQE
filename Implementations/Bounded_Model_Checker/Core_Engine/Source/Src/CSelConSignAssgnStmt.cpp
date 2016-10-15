#include "CSelConSignAssgnStmt.h"


CSelConSignAssgnStmt::CSelConSignAssgnStmt()
{
}

CSelConSignAssgnStmt::CSelConSignAssgnStmt(COperand *lhs,
			CExpression *sel_exp,
			list<CExpression*> &choicesList,
			list<CExpression*> &valuesList,
			string dm)
			:CConSignAssgnStmt(lhs, dm)
{
	selection_expression = sel_exp;
	this->choices.splice(this->choices.begin(), choicesList);
	this->values.splice(this->values.begin(), valuesList);
}

void CSelConSignAssgnStmt::print()
{
	//printf("\n*** concurrent select signal assignment ***\n");
	//cout<<endl<<"Target: "<<endl;
	//lhs->print();

	//cout<<endl<<"Selection expression: "<<endl;
	//selection_expression->print(0);

	list<CExpression*>::iterator iterCond;
	list<CExpression*>::iterator iterVal;
	int i;
	for(i=1, iterCond = choices.begin(), iterVal = values.begin();
	iterCond != choices.end() && iterVal != values.end();
	iterCond++,iterVal++,i++)
	{
		//cout<<"Choice_"<<i<<":"<<endl;
		//(*iterCond)->print(0);

		//cout<<"Value_"<<i<<":"<<endl;
		//(*iterVal)->print(0);
	}
}

string CSelConSignAssgnStmt::getSelExpr()
{
	return selection_expression->getOrgExpr();
}

CExpression* CSelConSignAssgnStmt::getOriginalSelExpr()
{
	return selection_expression;
}


list<CExpression*> CSelConSignAssgnStmt::getValues()
{
	return values;
}

list<CExpression*> CSelConSignAssgnStmt::getChoices()
{
	return choices;
}


