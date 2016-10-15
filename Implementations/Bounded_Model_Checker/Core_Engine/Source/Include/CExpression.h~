#ifndef __CEXPRESSION_H
#define __CEXPRESSION_H

#include "CAtom.h"
//#include "CParser.h"
#include "CSensitiveElement.h"
#include <string>
#include <iostream>
#include <list>
#include <set>


//#include "CDAGNode.h"
#include "CHelper.h"
/*#include "y.tab.h"*/


using namespace std;

/*#define t_Identifier 372 */

class CExpression
{
	string oper;
	bool is_operator1;
	bool is_operator2;
	bool is_unary;

	union
	{
		CExpression *expr_lhs;
		COperand *op_lhs;
	} lhs;

	union rhs
	{
		CExpression *expr_rhs;
		COperand *op_rhs;
	} rhs;

public:
	CExpression(); 

	CExpression( 	string oper, 
			bool is_op1, struct expr_oper *op1,
			bool is_op2, struct expr_oper *op2,
			bool is_unary);

	string getOperand();
	bool isLhsOperand();
	bool isRhsOperand();
	
	COperand * getLhsOperand();
	COperand * getRhsOperand();

	CExpression * getLhsExpr();
	CExpression * getRhsExpr();

	bool isUnary();
	void print(int num);

	string getOrgExpr();
	
	int getAndAdjustExpression(string entityName);
	void getAndAdjustExpressionCondition(string entityName);
	
	void getSensitivityList(list<CSensitiveElement*>& sensitivityList);
	
	CDAGNode* getDAG(string entName, int delta, set<string>& setRelation);
	
	CDAGNode* getSymbolicExpressions(string entName, int delta, set<string>& setRelation, list<ProcessSExpressions*> & SymbolicExpressions);// Added on 9/9/08

	void getChangedRHS(list<COperand*>& sensitivityList);
};

#endif
