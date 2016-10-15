#ifndef __CATOM_H
#define __CATOM_H

#include <string>
#include <string.h>
#include "COperand.h"
#include "CExpression.h"
#include <map>
#include <list>
#include <set>
#include <cctype>

#include "structure.h"

#include "ProcessSExpressions.h"

using namespace std;

struct SymTabEntry;


extern map <string, map<string, SymTabEntry> > SymbolTable;

class COperand;

class VariableRangeConstraint
{
public:
	pair<int, int> range;
	CDAGNode * constraint;	
	
	VariableRangeConstraint(int lower, int upper, CDAGNode * p_constraint)
	{
		range.first = lower;
		range.second = upper;
		constraint = p_constraint;
	}
};

class CAtom:public COperand
{
	string name;
	string upper;
	string lower;
	string unit;
//	CExpression *upper;
//	CExpression *lower;

	list<ProcessSExpressions*>::iterator IsPresent (list<ProcessSExpressions*> & SymbolicExpressions, CAtom* toBeSearched);

public:
	CAtom();
	~CAtom();
	CAtom(int type, string name, string upper, string lower, string unit="");
//	CAtom(int type, string name, string upper, string lower);
//	string getName();
	virtual void print();
	void setUpper(string p_upper);
	void setLower(string p_lower);

	string getUpper( );
	string getLower( );

	bool isEnumValue(map<string, SymTabEntry> &ids, string name);
	CDAGNode* getSymbolicExpressionsForVariable(CAtom* atomName,list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta);
	
		
	virtual string getOper();
	virtual int adjustRange(string entityName); 
	virtual void setLowerBits(int numberofbits);
	virtual bool unAdjusted();
	virtual string getName();
	virtual CDAGNode* getDAG(string entName, int delta, set<string>& setRelation);
	virtual CDAGNode* getSymbolicExpressions(string entName, int delta, set<string>& setRelation, list<ProcessSExpressions*> & SymbolicExpressions); // Added on 9/9/08
};

#endif
