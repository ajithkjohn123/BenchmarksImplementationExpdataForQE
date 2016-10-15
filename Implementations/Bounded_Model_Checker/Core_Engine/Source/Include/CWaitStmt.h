#ifndef __CWAITSTMT_H
#define __CWAITSTMT_H

#include "CSeqStmt.h"
#include "CExpression.h"

#include <string>
#include <iostream>
#include <list>

using namespace std;

class CWaitStmt:public CSeqStmt
{
	list<string> sensitivity;
	CExpression* condition;
	CExpression* timeout;

public:
	CWaitStmt();
	CWaitStmt(CExpression* p_condition,
		CExpression* p_timeout,
		list<string> p_sensitivity);

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
