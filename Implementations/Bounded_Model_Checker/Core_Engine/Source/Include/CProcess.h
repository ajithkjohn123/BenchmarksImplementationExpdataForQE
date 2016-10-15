#ifndef __CPROCESS_H
#define __CPROCESS_H

#include "COperand.h"
#include "structure.h"
#include <string>
#include <iostream>
#include <list>


#include "CSeqStmt.h"
#include "CParser.h"
#include "SymbolicRelation.h"

using namespace std;

class CSeqStmt;


class CProcess
{
	string name;
	list<COperand*> sensitivity_list;

	CSeqStmt *first;
public:
	CProcess(); 

	CProcess( string p_name,
		list<struct IDinfo *> &p_sensitivity_list,
		CSeqStmt *p_first);

	list<COperand*> & getSenList();

	CSeqStmt *getFirst();
	
	map<string, SymbolicRelation*> table;
	

// 	string getOperand();
// 	bool isLhsOperand();
// 	bool isRhsOperand();
// 	
// 	COperand * getLhsOperand();
// 	COperand * getRhsOperand();
// 
// 	CExpression * getLhsExpr();
// 	CExpression * getRhsExpr();
// 
// 	bool isUnary();
// 	void print(int num);
	
};

#endif
