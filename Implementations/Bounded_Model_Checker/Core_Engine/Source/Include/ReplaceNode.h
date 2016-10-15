#ifndef REPLACENODE_H_
#define REPLACENODE_H_

#include "CDAGNode.h"

class CDAGNode;

class ReplaceNode
{
public:
	CDAGNode *old_node;
	CDAGNode * new_node;
	ReplaceNode(CDAGNode *old_node1, CDAGNode * new_node1)
	{
		old_node = old_node1;
		new_node = new_node1;
	}
	
};

#endif /*REPLACENODE_H_*/
