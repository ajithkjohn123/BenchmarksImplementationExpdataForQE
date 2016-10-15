#ifndef CBASICBLOCK_H_
#define CBASICBLOCK_H_
#include <list>
#include <map>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <set>

#include <algorithm>

/*#include "structure.h"

#include "CEntity.h"
#include "CExpression.h"
#include "CProcess.h"
#include "CSeqStmt.h"
*/
#include "COSNode.h"
#include "ConditionRelation.h"


using namespace std;

struct ltdag
{
	  bool operator()(CDAGNode dag1, CDAGNode dag2) const
	  {
	  		return dag1.less(&dag2);
	  }
};

class CBasicBlock
{
private:
	void createGraphForConSignAssgn(list<CConSignAssgnStmt *> &conAssignNodes);
	map<string, list<node *> > affected;
	map<node *, list<string> > affecting;
	
	
	
	list<CSensitiveElement*> sensitivityList;
	
	void getOrientedStmts(CSeqStmt* stmt_ptr, list<OrientedStmt*> & ostmts);
	void createGraphForProcesses(list<CProcess *> & processNodes);
	
	set<CSensitiveElement*> input_identifiers; 
	
	set<CSensitiveElement*> output_identifiers;
	
	set<CSensitiveElement*> identifiers;
	

	
	

public:
	list<node*> block_nodes;
	list<CBasicBlock*> in_list;
	list<CBasicBlock*> out_list;
	
	map<CDAGNode, CDAGNode*, ltdag> mapRelation;
	
	//map<string, list<RelationInputOutput*> > table;
	map<string, SymbolicRelation*> table;

	list<CDAGNode*> constraints;
	
	map<string, list<ConditionRelation*> >mapSignalDelta; 
	
	CBasicBlock()
	{
		block_nodes.clear();
		in_list.clear();
		out_list.clear();
		sensitivityList.clear();
	}

	CBasicBlock(list<node*> &p_block_nodes,list<CBasicBlock*> &p_in_list,list<CBasicBlock*> &p_out_list)
	{
		block_nodes.splice(block_nodes.end(), p_block_nodes);
		in_list.splice(in_list.end(), p_in_list);
		out_list.splice(out_list.end(), p_out_list);
	}
	
	CBasicBlock(list<node*> &p_block_nodes)
	{
		block_nodes.splice(block_nodes.end(), p_block_nodes);
		in_list.clear();
		out_list.clear();
	}
	
	void addBlockNodes(list<node*> p_block_nodes)
	{
		block_nodes.splice(block_nodes.end(), p_block_nodes);
	}
	
	
	void addToIncoming(CBasicBlock* b)
	{
		in_list.push_back(b);
	}
	
	void addToOutgoing(CBasicBlock* b)
	{
		out_list.push_back(b);
	}
	
	void addTosensitivityList(CSensitiveElement* b)
	{
		sensitivityList.push_back(b);
	}
	
	void addInputIdentifiers(set<CSensitiveElement*>& p_identifiers)
	{
		input_identifiers.insert(p_identifiers.begin(), p_identifiers.end());
		addIdentifiers(p_identifiers);
	}
	
	//map<string, list<RelationInputOutput*> >& getTable()
	map<string, SymbolicRelation*>& getTable()
	{
		return table;
	}
	
	void addOutputIdentifiers(set<CSensitiveElement*>& p_identifiers)
	{
		output_identifiers.insert(p_identifiers.begin(), p_identifiers.end());
		addIdentifiers(p_identifiers);
	}
	
	void addIdentifiers(set<CSensitiveElement*>& p_identifiers)
	{
		identifiers.insert(p_identifiers.begin(), p_identifiers.end());
	}
	
	set<CSensitiveElement*>& get_Input_identifiers()
	{
		return input_identifiers;
	}
	
	set<CSensitiveElement*>& get_Output_identifiers()
	{
		return output_identifiers;
	}
	
	set<CSensitiveElement*>& get_identifiers()
	{
		return identifiers;
	}


/*	void createDependencyGraph(CEntity &ent);
	
	void printDependencyGraph();
	void printOrientedStmts();
	void createDependencies();
	
	void formBasicBlocks();
	void identifyStartsEnds();
	void formBlocks();*/
	
// 	static void readEntArchMap(char *filename, map<string, string> &EntArch);
// 	static void createGraph(char *outfile);
// 	static void printSymbolTable();

		// Function added on 29/08/09

	void display()
	  {
	   set<CSensitiveElement*>::iterator it;
	  
	   cout<<"\nIdentifiers\n";
	   
	   for(it=identifiers.begin(); it!= identifiers.end(); it++)
	     cout<<(*it)->name<<"("<<(*it)->lower<<","<<(*it)->upper<<")";

	   cout<<"\nInput Identifiers\n";
	   
	  for(it=input_identifiers.begin(); it!= input_identifiers.end(); it++)
	     cout<<(*it)->name<<"("<<(*it)->lower<<","<<(*it)->upper<<")";

	  cout<<"\nOutput Identifiers\n";

	  for(it=output_identifiers.begin(); it!= output_identifiers.end(); it++)
	     cout<<(*it)->name<<"("<<(*it)->lower<<","<<(*it)->upper<<")";


	  list<CSensitiveElement*>::iterator list_it;

	   cout<<"\nSensitivity List\n";

	   for(list_it=sensitivityList.begin(); list_it!= sensitivityList.end(); list_it++)
	     cout<<(*list_it)->name<<"("<<(*list_it)->lower<<","<<(*list_it)->upper<<")";

	  map<string, list<node *> >::iterator affected_it;
	  map<node *, list<string> >::iterator  affecting_it;

	  cout<<"\nAffected Map\n";

	  for(affected_it=affected.begin(); affected_it != affected.end(); affected_it++)
	    {
	      cout<<(*affected_it).first;
	      list<node*>::iterator node_it = (*affected_it).second.begin();
	      for(; node_it != (*affected_it).second.end(); node_it++)
		{
		  cout<<"\t"<<(*node_it)->name;
		}
	    }

	  
	 cout<<"\nAffecting Map\n";

	  for(affecting_it=affecting.begin(); affecting_it != affecting.end(); affecting_it++)
	    {
	      cout<<(*affecting_it).first->name;
	      list<string>::iterator string_it = (*affecting_it).second.begin();
	      for(; string_it != (*affecting_it).second.end(); string_it++)
		{
		  cout<<"\t"<<(*string_it);
		}
	    }

	  }



};
#endif 
