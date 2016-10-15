#include "CIfStmt.h"

CIfStmt::CIfStmt()
{
}

void CIfStmt::print()
{
	//cout<<"***THIS IS IF STATEMENT***"<<endl;
	if(this->next != NULL)
	{
		//this->next->print();
	}
}

void CIfStmt::createNode(FILE *fp, string parent)
{
}

void CIfStmt::adjustStmt(string entityName)
{
	//cout<<"condition"<<endl;
	condition->getAndAdjustExpression(entityName); 
	//cout<<"true"<<endl;
	true_branch->adjustStmt(entityName);
	//cout<<"false"<<endl;
	false_branch->adjustStmt(entityName);
	//cout<<"next"<<endl;
	if(this->next != NULL)
	{
		this->next->adjustStmt(entityName);
	}
}

//void CIfStmt::getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts)
//{
//}

void CIfStmt::getChangedLHS(list<CSensitiveElement*>& sensitivityList)
{
}

// Added on 5/9/08

void CIfStmt::getOutputs(list<ProcessSExpressions*> & SymbolicExpressions)
{
}

// End of additions

// Added on 9/9/08

void CIfStmt::getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta)
{
}

// End of additions



void CIfStmt::getChangedRHS(list<COperand*>& sensitivityList)
{
}

void CIfStmt::createEdgeInArgDepGraph(Graph *ArgDepGraph)
{
}


