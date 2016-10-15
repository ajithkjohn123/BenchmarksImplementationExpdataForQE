#ifndef CONDITIONSVALUES_H_
#define CONDITIONSVALUES_H_

#include "CDAGNode.h"

class ConditionsValues
{
public:
	vector<int> delta;
	vector<CDAGNode*> values;
	vector<CDAGNode*> conditions;
	vector< set<string> > setConditions;
	vector< vector<pair<int, int> > > ranges_covered;
	
	ConditionsValues()
	{
		delta.clear();
		values.clear();
		conditions.clear();
	}
	//ports.splice(ports.begin(), p_ports);;
	ConditionsValues(	vector<int> adelta,
						vector<CDAGNode*> avalues,
						vector<CDAGNode*> aconditions)
	{
/*		delta.splice(delta.begin(), adelta);
		values.splice(values, avalues);
		conditions.splice(conditions, aconditions);*/
	}
	
	
};

#endif /*CONDITIONSVALUES_H_*/
