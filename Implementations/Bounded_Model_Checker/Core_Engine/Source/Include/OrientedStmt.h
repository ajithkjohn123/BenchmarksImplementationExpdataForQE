#ifndef ORIENTEDSTMT_H_
#define ORIENTEDSTMT_H_

#include <list>
#include <string>

#include "CExpression.h"

using namespace std;

class COperand;

class CExpression;

class OrientedStmt
{
	COperand *lhs;
	list<CExpression*> values;
	list<CExpression*> conditions;
		
public:
	OrientedStmt(	COperand *p_lhs,
					list<CExpression*> p_values,
					list<CExpression*> p_conditions
				)
	{
		lhs = p_lhs;
		values.splice(values.begin(), p_values);;
		conditions.splice(conditions.begin(), p_conditions);;
	}
	
	void addValues(list<CExpression*> p_values, list<CExpression*> p_conditions)
	{
		values.splice(values.end(), p_values);;
		conditions.splice(conditions.end(), p_conditions);;
	}
	COperand *getLhs()
	{
		return lhs;
	}
	list<CExpression*>& getValues()
	{
		return values;
	}
	
	list<CExpression*>& getConditions()
	{
		return conditions;
	}
	
};

#endif /*ORIENTEDSTMT_H_*/
