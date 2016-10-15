#ifndef __CINSTANCE_H
#define __CINSTANCE_H

#include "CSeqStmt.h"
#include "CExpression.h"
#include "COperand.h"
#include <string>
#include <iostream>
#include <list>

using namespace std;

class CExpression;
class CEntity;

class CInstance
{
	list<COperand*> formal_param;
	list<CExpression*> actual_param;
	string instance_name;
	string entity_name;
	int index;
public:
	CInstance();

	CInstance(string p_instance_name,
		string p_entity_name,
		list<CExpression*> p_actual_param,
		list<COperand*> p_formal_param);

	virtual void print();

	string getEntName();
	string getInstName();
	int getIndex();

	void setIndex(int ind);

	list<CExpression*>& getActualParam();
	list<COperand*>& getFormalParam();

/*	CSeqStmt *getNext();
	CSeqStmt();
	setNext(CSeqStmt *n);
	CSeqStmt(CSeqStmt *n);*/
};


#endif
