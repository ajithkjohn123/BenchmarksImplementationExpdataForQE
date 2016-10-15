#ifndef __COPERAND_H
#define __COPERAND_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <set>

#include "CDAGNode.h"
#include "CHelper.h"
#include "ConditionsValues.h"
#include "ProcessSExpressions.h"



using namespace std;

//extern map <string, map<string, RangeValue> > SymbolicValues;

class COperand
{
protected:
	string opType;// int, bit, std_logic
	int type;
	string unit;
public:
	COperand();
	~COperand();
	COperand(int type, string unit);
	int getType();
	virtual void print();
	virtual string getOper();
	virtual int adjustRange(string entityName); 
	virtual bool unAdjusted();
	virtual void setLowerBits(int numberofbits);
	virtual string getName();
	
	virtual CDAGNode* getDAG(string entName, int previous, set<string>& setRelation);

	virtual CDAGNode* getSymbolicExpressions(string entName, int delta, set<string>& setRelation, list<ProcessSExpressions*> & SymbolicExpressions); // Added on 9/9/08
	
	
};




#endif
