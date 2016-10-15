#ifndef __CWHILESTMT_H
#define __CWHILESTMT_H

#include "CSeqStmt.h"
#include "CExpression.h"

class CWhileStmt:public CSeqStmt
{
	CExpression* condition;
	CSeqStmt *body;

public:
	CWhileStmt();
	CWhileStmt(CExpression* condition,
	CSeqStmt *body);

	virtual void print();
	virtual list<CSeqStmt *> getBranches();
	//virtual void getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts);
	virtual void getOutputs(list<ProcessSExpressions*> & SymbolicExpressions); // Modified on 5/9/08
	virtual void getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta); // Added on 9/9/08

/*	CSeqStmt *getNext();
	CSeqStmt();
	setNext(CSeqStmt *n);
	CSeqStmt(CSeqStmt *n);*/
};


#endif
