#ifndef PROCESSSEXPRESSIONS_H_
#define PROCESSSEXPRESSIONS_H_

#include <list>
#include <string>

//#include "CDAGNode.h"

using namespace std;

class COperand;

class CDAGNode;

class ProcessSExpressions
{
	COperand *lhs;
	CDAGNode* value;
	
		
public:
	ProcessSExpressions(COperand *p_lhs, CDAGNode* p_value)
	{
		lhs = p_lhs;
		value = p_value;
	}
	
	void addValue(CDAGNode* p_value)
	{
		value = p_value;
	}
	COperand *getLhs()
	{
		return lhs;
	}
	CDAGNode* getValue()
	{
		return value;
	}
	

};

#endif 
