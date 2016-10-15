#ifndef RELATIONINPUTOUTPUT_H_
#define RELATIONINPUTOUTPUT_H_

#include "CDAGNode.h"


class RelationInputOutput
{
public:
	int delta_time;
	CDAGNode* relation;
	CDAGNode* condition;	
	vector<pair<int, int> > ranges_covered;
	set<string> setRelation;
	
	RelationInputOutput(int delta_time1, CDAGNode* relation1, CDAGNode* condition1,  
						vector<pair<int, int> > &ranges_covered1, set<string> &setRelation1)
	{
		delta_time = delta_time1;
		relation = relation1;
		condition = condition1;	
		setRelation.insert(setRelation1.begin(), setRelation1.end());
		ranges_covered = ranges_covered1;
	}
};



#endif /*RELATIONINPUTOUTPUT_H_*/
