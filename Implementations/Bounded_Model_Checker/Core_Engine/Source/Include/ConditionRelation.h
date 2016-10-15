#ifndef CONDITIONRELATION_H_
#define CONDITIONRELATION_H_

#include <string>

#include <iostream>
#include <set>

using namespace std;


class ConditionRelation
{
public:
	CDAGNode * condition;	
	CDAGNode * relation;	
	set<string> setCondition;
	set<string> setRelation;
	vector<pair<int, int> > ranges_covered;
	
	
	ConditionRelation(CDAGNode * p_condition, CDAGNode * p_relation)
	{
		condition = p_condition;
		relation = p_relation;
	}
};


#endif /*CONDITIONRELATION_H_*/
