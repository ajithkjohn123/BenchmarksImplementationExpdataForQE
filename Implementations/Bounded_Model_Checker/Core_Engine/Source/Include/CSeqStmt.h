#ifndef __CSEQSTMT_H
#define __CSEQSTMT_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <list>


//#include "COSNode.h"
#include "ProcessSExpressions.h"
#include "CSensitiveElement.h"
#include "OrientedStmt.h"

#include "graph.h"


using namespace std;

class CJunctionNode;

class CSeqStmt
{
protected:
	CSeqStmt *next;
public:
	CSeqStmt *getNext();
	CSeqStmt();
	void setNext(CSeqStmt *n);

	CSeqStmt(CSeqStmt *n);
	virtual void print();
	virtual list<CSeqStmt *> getBranches();
	virtual void createNode(FILE *fp, string parent);
	virtual void adjustStmt(string entityName);
	
	//virtual void getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts);
	
	virtual void getChangedLHS(list<CSensitiveElement*>& sensitivityList);

	virtual void getOutputs(list<ProcessSExpressions*> & SymbolicExpressions); // Added on 5/9/08

	virtual void getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta); // Added on 9/9/08
	virtual void getChangedRHS(list<COperand*>& sensitivityList);

	virtual void createEdgeInArgDepGraph(Graph *ArgDepGraph);
};


#endif
