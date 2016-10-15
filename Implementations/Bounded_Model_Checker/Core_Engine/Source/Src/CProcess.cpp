#include "CProcess.h"
CProcess::CProcess()
{
}

CProcess::CProcess( string p_name,
		list<struct IDinfo *> &p_sensitivity_list,
		CSeqStmt *p_first)
{
	name = p_name;
	first = p_first;
	
	list<struct IDinfo *> ::iterator isen = p_sensitivity_list.begin(); 
	for(; isen != p_sensitivity_list.end(); isen++)
	{
		COperand* op =  CParser::createOperand(*isen);
		sensitivity_list.push_back(op);
	}
}

list<COperand*> & CProcess::getSenList()
{
	return sensitivity_list;
}

CSeqStmt *CProcess::getFirst()
{
	return first;
}
