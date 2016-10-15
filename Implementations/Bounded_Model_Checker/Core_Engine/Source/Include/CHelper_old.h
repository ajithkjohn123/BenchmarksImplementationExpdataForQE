#ifndef CHELPER_H_
#define CHELPER_H_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>

#include <math.h>

/*#include "CPort.h"
#include "CAtom.h"
#include "CLiteral.h"
#include "structure.h"
#include "idinfo.h"
#include "CSeqStmt.h"
#include "CCaseStmt.h"
#include "CJunctionNode.h"

#include "CEntity.h"

#include "CPrePostProcessor.h"*/

#include "CDAGNode.h"

using namespace std;

class CDAGNode;

class CHelper
{
public:
	static vector<int> findAtomsOfLHS(vector< pair<int,int> > &atom_ranges, int lower, int upper);
	
	static vector<int> findAtomsOfRHS(vector< pair<int,int> > &atom_ranges, int lower, int upper);

	static CDAGNode* createDAG(string oper, CDAGNode* lhs);

	static CDAGNode* createDAG(string oper, CDAGNode* lhs, CDAGNode* rhs);

	static CDAGNode* createDAG(string oper, CDAGNode* c1, CDAGNode* c2, CDAGNode* c3);
	
	static CDAGNode* createDAG(string oper, CDAGNode* c1, int r1, int r2);
	
	static CDAGNode* createDAG(string oper, int index);
	
	static CDAGNode* createDAG(string name);
		
	static CDAGNode* CHelper::createDAG(string name, int lower, int upper, int index);
	
};

#endif /*CHELPER_H_*/
