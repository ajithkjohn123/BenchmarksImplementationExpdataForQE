#ifndef __CPARSER_H
#define __CPARSER_H

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>

#include <math.h>

#include "CPort.h"
#include "CAtom.h"
#include "CLiteral.h"
#include "structure.h"
#include "idinfo.h"
#include "CSeqStmt.h"
//#include "CCaseStmt.h"
#include "CJunctionNode.h"

#include "CEntity.h"

#include "CPrePostProcessor.h"

#include "CDAGNode.h"

using namespace std;

/*#define t_Identifier 372 */

extern map <string, map<string, SymTabEntry> > SymbolTable;

class CEntity;

extern vector<CEntity> Entities;

class CPort;

class CParser
{
public:
	static void printPort(list<CPort> port_list);

	static COperand*  createOperand(struct IDinfo*);

	static COperand*  createOperand(string name);

	static CExpression* createExpression(string oper, struct expr_oper *op1, struct expr_oper *op2, bool is_unary);

	static struct expr_oper* convertToExpression(struct expr_oper *op);

	static CSeqStmt * getLast(CSeqStmt *first);

	static void printExpression(CExpression * expr, int num);

	static void printOperand(COperand *op);

	static struct expr_oper* createExpression(char *name);

	static CSeqStmt *createAndSetJunctionNode(CSeqStmt *cond_stmt);

	static void insertPortToSymbolTable(string entity_name, list<CPort> port_list);

	static int getEntity(string entity_name);

	static bool isConstFolding(struct expr_oper *op1, struct expr_oper *op2);

	static COperand* doConstFolding(string oper, struct expr_oper *op1, struct expr_oper *op2);
	
	static CExpression* createOthers();
	
};

#endif

