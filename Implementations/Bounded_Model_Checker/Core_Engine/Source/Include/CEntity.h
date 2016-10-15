#ifndef __CENTITY_H
#define __CENTITY_H
#include <vector>
#include <list>
#include <string>

#include "CProcess.h"
#include "CConSignAssgnStmt.h"
#include "CInstance.h"
#include "CPort.h"
#include "CDAGNode.h"


using namespace std;

class SymbolicRelation;
class CPort;
class CInstance;
class CProcess;
class CConSignAssgnStmt;
class CDAGNode;

class CEntity
{
private:
	string name;
 	list<CPort> ports;
 	list<CPort> gens;

 	list<CInstance *> instanceNodes;
 	list<CProcess *> processNodes;
 	list<CConSignAssgnStmt *> conAssignNodes;
 	int ref_count;
 	
public:

	//map<string, list<RelationInputOutput*> > table;

	map<string, SymbolicRelation*> table;

	list<CDAGNode*> constraints;

	CEntity();
	void setName(char*p_name);
	string getName();
	CEntity(string p_name,
 		list<CPort> p_ports,
 		list<CPort> p_gens);

	void setProcesses(list<CProcess *>& p_processNodes);
	void setConAssgnStmt(list<CConSignAssgnStmt *>& p_conAssignNodes);
	void setInstances(list<CInstance *>& p_instantNodes);

	list<CInstance *> & getInstances();
	list<CConSignAssgnStmt *>& getConAssgnStmt();
	list<CProcess *>& getProcesses();
	list<CPort> & getPorts();
	
	//map<string, list<RelationInputOutput*> >& getTable();
	map<string, SymbolicRelation*>& getTable();

	void setRefCount(int count);
	int getRefCount();
};




#endif
