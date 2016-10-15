#ifndef __CJUNCTIONNODE_H
#define __CJUNCTIONNODE_H

#include "CSeqStmt.h"

class CSeqStmt;
class ProcessSExpressions;

class CJunctionNode:public CSeqStmt
{
	CSeqStmt* parent;
	bool traversed;
public:
	CJunctionNode(CSeqStmt* p_parent);
	CJunctionNode();
	virtual void print();
	virtual void createNode(FILE *fp, string parent);
	virtual void adjustStmt(string entityName);
	//virtual void getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts);
	
	virtual void getChangedLHS(list<CSensitiveElement*>& sensitivityList);
	
	virtual void getOutputs(list<ProcessSExpressions*> & SymbolicExpressions); // Modified on 5/9/08
	
	virtual void getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta); // On 9/9/08
	
	virtual void getChangedRHS(list<COperand*>& sensitivityList);

	virtual void createEdgeInArgDepGraph(Graph *ArgDepGraph);
};


#endif
