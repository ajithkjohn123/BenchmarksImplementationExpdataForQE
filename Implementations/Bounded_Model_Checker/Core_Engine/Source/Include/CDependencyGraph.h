#ifndef __CDEPENDENCYGRAPH_H
#define __CDEPENDENCYGRAPH_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <set>

#include "structure.h"

#include "CEntity.h"
#include "CExpression.h"
#include "CProcess.h"
#include "CSeqStmt.h"

#include "COSNode.h"
#include "CBasicBlock.h"

#include "RangeValue.h"

#include "CSymbSimulator.h"

using namespace std;


extern map <string, map<string, SymTabEntry> > SymbolTable;

extern vector<CEntity> Entities;

//extern map <string, map<string, RangeValue> > SymbolicValues;

struct ltstr
{
	  bool operator()(CSensitiveElement s1, CSensitiveElement s2) const
	  {
		  	if(s1.name < s2.name)
		  	{
		  		return true;
		  	}
		  	else if(s1.upper < s2.upper)
		  	{
		  		return true;	
		  	}
		  	else if(s1.lower < s2.lower)
		  	{
		  		return true;
		  	}
		  	else
		  	{
		  		return false;
		  	}
		  	
	  }
};

class CDependencyGraph
{
private:
	void createGraphForConSignAssgn(list<CConSignAssgnStmt *> &conAssignNodes, string EntName);
	map<CSensitiveElement, list<node *>, ltstr> affected;
	map<string, list<CSenNode*> > affecting;

	map<string, list<CSenNode*> > affecting_and_sensitive; // Newly added field to indicate the signals to which a node is sensitive 

	map<node *, CBasicBlock* > nodePartOfBasicBlock;
	map<CBasicBlock*, list<node *> > neighbors;
	
	map<string, list<int> > atoms;
	
	map<node*, set<string> >  nodeIdentifiers;
	

	void getOrientedStmts(CSeqStmt* stmt_ptr, list<OrientedStmt*> & ostmts);
	void createGraphForProcesses(list<CProcess *> & processNodes, string EntName);
	
	void createGraphForInstances(list<CInstance *> &instanceNodes, string EntName);
	
	void print_sen_list(list<CSensitiveElement*> sensitivityList);
	void printGraph();

	
	CPort& getPort(list<CPort> & ports, string name);
	
	void initialize();
	void find_nodes_with_zero_indegree(list<node *> &);
	void adjust_indegrees(list<node *> &);
	

	void FormSymbolicStructure(string name);
	
public:
	list<struct node*> nodes;
	list<CBasicBlock*> basicblocks;
	
	CDependencyGraph()
	{
		nodes.clear();
	}

	void createDependencyGraph(CEntity &ent);
	
	void printDependencyGraph();
	void printOrientedStmts();
	void createDependencies();
	void createDependenciesBB();
	
	
	void formBasicBlocks();
	void formBasicBlocks1();
	set<node *> identifyStartsEnds();
	void identifyStartsEnds1();
	void formBlocks(set<node *> &startnodes);
	
	list<CSenNode*> createSenNodeList(list<CSensitiveElement*> senList, node* n, string EntName);
	bool isComflicting(const CSensitiveElement &se1, const  CSensitiveElement &se2);
	list<CSensitiveElement*> createSenList(list<COperand*>& senList, string EntName);

	
	void topologicalSorting(list<node *> &sortedNodes);
	
	list<CBasicBlock*> &getBasicBlocks();
	void printBB();
	map<string, list<int> >& getAtoms();
	
	void FindAtoms();
	
	map<string, list<CSenNode*> > &getAllSenList()
	{
		 return affecting;
	}

	map<string, list<CSenNode*> > &getAffecting_And_Sensitive()
	{
		 return affecting_and_sensitive;
	}
	
	map<node *, CBasicBlock* > &GetNodePartOfBasicBlock()
	{
		return nodePartOfBasicBlock;
	}
	
	void createSensitivityList();
	
	
	
	
// 	static void readEntArchMap(char *filename, map<string, string> &EntArch);
// 	static void createGraph(char *outfile);
// 	static void printSymbolTable();

	// New functions added

	list<CSenNode*> createSenNodeListForProcess(list<CSensitiveElement*> senList, node* n, string EntName, list<COperand*> sen_list1);
	bool signalPresentInSensitivityList(CSensitiveElement* sen_element, list<COperand*> sen_list);

	void getSensitivityListConAssignStmt(CConSignAssgnStmt* proc_node, list<CSensitiveElement*>  &sensitivityList);
	void getSensitivityListCompInstStmt(CInstance * proc_node, list<CSensitiveElement*>  &sensitivityList);
	void getSensitivityListProcessStmt(CProcess * proc_node, list<CSensitiveElement*>  &sensitivityList, string EntName);
	list<node *> arrangeInTriggeredOrder(list<node *> originalNodes, string EntName);
	bool Triggering(node *first, node *second, string EntName);
	void arrangeProcessInBBInTriggeredOrder(string EntName);
	void displayNode(node *x);

};

#endif
