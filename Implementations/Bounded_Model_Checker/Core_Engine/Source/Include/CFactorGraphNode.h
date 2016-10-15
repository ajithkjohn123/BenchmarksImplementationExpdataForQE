#ifndef CFACTORGRAPHNODE_H_
#define CFACTORGRAPHNODE_H_

#include <list>
#include <string>
#include <iostream>
#include <set>

#include "CDAGNode.h"

using namespace std;

class CFactorGraphNode
{
	public:
		int type;
		int count; //Used in scope reduction 2
		bool marked; //Used in scope reduction 3 
		list<CFactorGraphNode*> list_of_edges;
		string name;
		CDAGNode *exp;
		
		CFactorGraphNode()
		{
			count = 0;
			marked = false;
			list_of_edges.clear();
			name = "";
			exp = NULL;
		}
		
		
};


#endif /*CFACTORGRAPHNODE_H_*/
