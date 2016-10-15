#include "CHelper.h"


vector<int>	CHelper::findAtomsOfLHS(vector< pair<int,int> > &atom_ranges, int lower, int upper)
{
	vector<int> atom_indices;
	vector< pair<int,int> >::iterator irange = atom_ranges.begin();
	
	int i = 0;
	for(;i < atom_ranges.size(); i++)
	{
		if((*irange).first == lower)
		{
			//atom_indices.push_back(i);
			break;
		}
	}

	for(;i < atom_ranges.size(); i++)
	{
		atom_indices.push_back(i);
		
		if((*irange).second == upper)
		{
			break;
		}
	}
	
	return atom_indices;
}

vector<int>	CHelper::findAtomsOfRHS(vector< pair<int,int> > &atom_ranges, int lower, int upper)
{
	vector<int> atom_indices;
	vector< pair<int,int> >::iterator irange = atom_ranges.begin();
	
	int i = 0;
	for(;i < atom_ranges.size(); i++)
	{
		if(atom_ranges[i].first <= lower && atom_ranges[i].second >= lower)
		{
			//atom_indices.push_back(i);
			break;
		}
	}

	for(;i < atom_ranges.size(); i++)
	{
		atom_indices.push_back(i);
		
		if(atom_ranges[i].first <= upper && atom_ranges[i].second >= upper)
		{
			break;
		}
	}
	
	return atom_indices;
}

CDAGNode* CHelper::createDAG(string oper, CDAGNode* lhs)
{
	CDAGNode* dag_node = new CDAGNode();
	
	dag_node->name = oper;
	
	dag_node->out_list.push_back(lhs);
	lhs->in_list.push_back(dag_node);

	return dag_node;
}

CDAGNode* CHelper::createDAG(string oper, CDAGNode* lhs, CDAGNode* rhs)
{
	CDAGNode* dag_node = new CDAGNode();
	
	dag_node->name = oper;
	
	dag_node->out_list.push_back(lhs);
	lhs->in_list.push_back(dag_node);

	dag_node->out_list.push_back(rhs);
	rhs->in_list.push_back(dag_node);

	return dag_node;
}

CDAGNode* CHelper::createDAG(string oper, CDAGNode* c1, CDAGNode* c2, CDAGNode* c3)
{
	CDAGNode* dag_node = new CDAGNode();
	
	dag_node->name = oper;
	
	dag_node->out_list.push_back(c1);
	c1->in_list.push_back(dag_node);

	dag_node->out_list.push_back(c2);
	c2->in_list.push_back(dag_node);

	dag_node->out_list.push_back(c3);
	c3->in_list.push_back(dag_node);

	return dag_node;	
}

CDAGNode* CHelper::createDAG(string oper, CDAGNode* c1, int r1, int r2)
{
	CDAGNode* dag_node = new CDAGNode();
	
	char temp[20];
	sprintf(temp, "%d", r1);
	CDAGNode *st_dag = createDAG(temp);
	
	sprintf(temp, "%d", r2);
	CDAGNode *en_dag = createDAG(temp);
	
	dag_node->name = oper;
	
	dag_node->out_list.push_back(c1);
	c1->in_list.push_back(dag_node);

	dag_node->out_list.push_back(st_dag);
	st_dag->in_list.push_back(dag_node);

	dag_node->out_list.push_back(en_dag);
	en_dag->in_list.push_back(dag_node);

	return dag_node;		
}


CDAGNode* CHelper::createDAG(string name)
{
	CDAGNode* dag_node = new CDAGNode();
	
	dag_node->name = name;

	return dag_node;	
}

CDAGNode* CHelper::createDAG(string name, int index)
{
	CDAGNode* dag_node = new CDAGNode();
	
	dag_node->name = name;
	dag_node->delta_time = index;
	dag_node->delta_variable = true;
	
	return dag_node;		
}

CDAGNode* CHelper::createDAG(string name, int lower, int upper, int index)
{
	CDAGNode* id = createDAG(name, index);;
	
	CDAGNode* dag_node = createDAG("select", id,  lower, upper);
	
	return dag_node;		
}
