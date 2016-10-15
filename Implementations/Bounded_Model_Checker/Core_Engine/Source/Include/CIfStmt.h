#ifndef __CIFSTMT_H
#define __CIFSTMT_H

#include "CSeqStmt.h"
#include "CExpression.h"

class CIfStmt:public CSeqStmt
{
	CExpression* condition;
	CSeqStmt *true_branch;
	CSeqStmt *false_branch;
public:
	CIfStmt();
	virtual void print();
	virtual void createNode(FILE *fp, string parent);
	virtual void adjustStmt(string entityName);

	//virtual void getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts);
	
	virtual void getChangedLHS(list<CSensitiveElement*>& sensitivityList);

	virtual void getOutputs(list<ProcessSExpressions*> & SymbolicExpressions); // Added on 5/9/08

	virtual void getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta); // Added on 9/9/08

/*	CSeqStmt *getNext();
	CSeqStmt();
	setNext(CSeqStmt *n);
	CSeqStmt(CSeqStmt *n);*/

	virtual void getChangedRHS(list<COperand*>& sensitivityList);
	virtual void createEdgeInArgDepGraph(Graph *ArgDepGraph);
};


#endif
