#include "CSeqStmt.h"
CSeqStmt *CSeqStmt::getNext()
{
	return next;
}

void CSeqStmt::setNext(CSeqStmt *n)
{
	next = n;
}

CSeqStmt::CSeqStmt(CSeqStmt *n)
{
	next = n;
}

CSeqStmt::CSeqStmt()
{
	next = NULL;
}

void CSeqStmt::print()
{
	//cout<<"*** null statement ***"<<endl;
}

list<CSeqStmt *> CSeqStmt::getBranches()
{
}

void CSeqStmt::createNode(FILE *fp, string parent)
{
}

void CSeqStmt::adjustStmt(string entityName)
{
}

/*void CSeqStmt::getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts)
{
}*/

void CSeqStmt::getChangedLHS(list<CSensitiveElement*>& sensitivityList)
{
	
}


// Added on 5/9/08

void CSeqStmt::getOutputs(list<ProcessSExpressions*> & SymbolicExpressions)
{
}

// End of additions


// Added on 9/9/08

void CSeqStmt::getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta)
{
}

// End of additions

// Added on 30/9/08

void CSeqStmt::getChangedRHS(list<COperand*>& sensitivityList)
{
	
}

// End of additions

void CSeqStmt::createEdgeInArgDepGraph(Graph *ArgDepGraph)
{
}