#include "COperand.h"

COperand::COperand()
{
}

COperand::~COperand()
{
}

COperand::COperand(int type, string unit)
{
	this->type = type;
	this->unit = unit;
}

int COperand::getType()
{
	return type;
}

void COperand::print()
{
	cout<<endl<<"ERROR -- this function should not be called -- COperand::print()"<<endl;
}

string COperand::getOper()
{
}

string COperand::getName()
{

}

int COperand::adjustRange(string entityName)
{

} 

bool COperand::unAdjusted()
{
	return false;
}

void COperand::setLowerBits(int numberofbits)
{

}

CDAGNode* COperand::getDAG(string entName, int previous, set<string>& setRelation)
{

}

// Added on 9/9/08

CDAGNode* COperand::getSymbolicExpressions(string entName, int delta, set<string>& setRelation,  list<ProcessSExpressions*> & SymbolicExpressions)
{

}

// End of Additions on 9/9/08

