#include "CEntity.h"

CEntity::CEntity()
{
/*	instantNodes.clear();
	instantNodes.clear();
	instantNodes.clear();
	inPorts.clear();
	outPorts.clear();
	inoutPorts.clear();
	bufferPorts.clear();
	name = "";*/
	ref_count = 0;
}

CEntity::CEntity(string p_name,
 	list<CPort> p_ports,
 	list<CPort> p_gens
 	)
{
	name = p_name;
	ports.splice(ports.begin(), p_ports);;
	gens.splice(gens.begin(), p_gens);;
	
	ref_count = 0;
}

void CEntity::setProcesses(list<CProcess *>& p_processNodes)
{
	processNodes.splice(processNodes.begin(), p_processNodes);;
}

void CEntity::setConAssgnStmt(list<CConSignAssgnStmt *>& p_conAssignNodes)
{
	conAssignNodes.splice(conAssignNodes.begin(), p_conAssignNodes);;
}

void CEntity::setInstances(list<CInstance *>& p_instantNodes)
{
	instanceNodes.splice(instanceNodes.begin(), p_instantNodes);;
}

void CEntity::setName(char*p_name)
{
    name = p_name;
}

string CEntity::getName()
{
    return name;
}

list<CInstance *> & CEntity::getInstances()
{
	return instanceNodes;
}

list<CPort> & CEntity::getPorts()
{
	return ports;
}

list<CConSignAssgnStmt *>& CEntity::getConAssgnStmt( )
{
	return conAssignNodes;
}

list<CProcess *>& CEntity::getProcesses()
{
	return processNodes;
}

map<string, SymbolicRelation*>& CEntity::getTable()
{
	return table;
}

void CEntity::setRefCount(int count)
{
	ref_count = count;
}

int CEntity::getRefCount()
{
	return ref_count;
}



