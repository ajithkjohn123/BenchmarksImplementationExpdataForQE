//#ifndef SYMBOLICRELATION_H_
//#define SYMBOLICRELATION_H_

#include "CDAGNode.h"


class SymbolicRelation
{
public:
        map<CSensitiveElement*, string> delays; // Delta delay of the output for each input - Not applicable for process stmts, as for a process stmt.,  each output is affected by inputs before one delta delay 
	CDAGNode* relation; // Symbolic expression
	vector<pair<int, int> > ranges_covered; // To be removed later 
	set<string> setRelation; // To be removed later 

	SymbolicRelation(CDAGNode* relation1, vector<pair<int, int> > &ranges_covered1, set<string> &setRelation1)
	{
	        relation = relation1;
		setRelation.insert(setRelation1.begin(), setRelation1.end());
		ranges_covered = ranges_covered1;
	}
	
	SymbolicRelation(map<CSensitiveElement*, string> delays1, CDAGNode* relation1, vector<pair<int, int> > &ranges_covered1, set<string> &setRelation1)
	{
	  
	        delays = delays1;
		relation = relation1;
		setRelation.insert(setRelation1.begin(), setRelation1.end());
		ranges_covered = ranges_covered1;
	}
};



//#endif /*RELATIONINPUTOUTPUT_H_*/
