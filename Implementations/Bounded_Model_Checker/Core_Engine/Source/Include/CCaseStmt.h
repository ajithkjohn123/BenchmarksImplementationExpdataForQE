#ifndef __CCASESTMT_H
#define __CCASESTMT_H

#include "CSeqStmt.h"
#include "CExpression.h"

#include "CParser.h"

#include <list>

using namespace std;

class CSeqStmt;
class ProcessSExpressions;



class CCaseStmt:public CSeqStmt
{
	CExpression* case_expr;

	list<CExpression*> case_choices;
	list<CSeqStmt *> case_statements;

public:
	CCaseStmt();
	CCaseStmt(CExpression* p_case_expr,
	list<CExpression*> p_case_choices,
	list<CSeqStmt *> p_case_statements);

	virtual void print();

	virtual list<CSeqStmt *> getBranches();
	virtual void createNode(FILE *fp, string parent);
	virtual void adjustStmt(string entityName);
	
	virtual void getChangedLHS(list<CSensitiveElement*>& sensitivityList);
	
/*	CSeqStmt();
	setNext(CSeqStmt *n);
	CSeqStmt(CSeqStmt *n);*/

	virtual void getOutputs(list<ProcessSExpressions*> & SymbolicExpressions); // Added on 5/9/08
	virtual void getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta); // Added on 9/9/08
	virtual void getChangedRHS(list<COperand*>& sensitivityList);
	virtual void createEdgeInArgDepGraph(Graph *ArgDepGraph);
};


#endif
