#ifndef __CASSGNSTMT_H
#define __CASAGNSTMT_H

#include "CSeqStmt.h"
#include "CExpression.h"
#include "COperand.h"

#include "structure.h"
#include "CParser.h"

#include <algorithm>

using namespace std;




class CAssgnStmt:public CSeqStmt
{
	int type_of_assignment;
	// varible or signal

	COperand *lhs;
	CExpression* rhs;
	
	list<ProcessSExpressions*>::iterator IsPresent (list<ProcessSExpressions*> & ostmts, COperand* toBeSearched);

public:
	CAssgnStmt();
	CAssgnStmt(COperand *lhs, CExpression* rhs, int type);
	virtual void print();
	virtual void createNode(FILE *fp, string parent);
	virtual void adjustStmt(string entityName);	
	virtual void getChangedLHS(list<CSensitiveElement*>& sensitivityList);
	
	virtual void getOutputs(list<ProcessSExpressions*> & SymbolicExpressions);  // Added on 5/9/08
	virtual void getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta); // Added on 9/9/08
	virtual void getChangedRHS(list<COperand*>& sensitivityList);

	virtual void createEdgeInArgDepGraph(Graph *ArgDepGraph);
	bool sePresentInSensitveList(list<CSensitiveElement*> sensitivityList, CSensitiveElement* se);
};


#endif
