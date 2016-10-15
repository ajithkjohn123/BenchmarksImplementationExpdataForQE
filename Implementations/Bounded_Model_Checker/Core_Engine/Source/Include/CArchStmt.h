#ifndef CARCHSTMT_H_
#define CARCHSTMT_H_

#include "CProcess.h"
#include "CConSignAssgnStmt.h"
#include "CInstance.h"

#define CONCURRENT_ASSIGNMENT_STMT 0
#define PROCESS_STMT 1
#define COMPONENT_INSTANTIANTION_STMT 2

class CProcess;
class CConSignAssgnStmt;
class CInstance;

class CArchStmt
{
public:
	int type; // CONCURRENT_ASSIGNMENT_STMT PROCESS_STMT or COMPONENT_INSTANTIANTION_STMT
	union 
	{
		CInstance * instance_stmt;
 		CProcess * process_stmt;
 		CConSignAssgnStmt * conc_assign_stmt;
	} stmt;
	
public:
	CArchStmt(	int type, 
				CConSignAssgnStmt * conc_assign_stmt,
 				CProcess * process_stmt,
				CInstance * instance_stmt )
	{
		this->type = type;
		switch(type)
		{
			case CONCURRENT_ASSIGNMENT_STMT:
				this->stmt.conc_assign_stmt = conc_assign_stmt;
			break;
	
			case PROCESS_STMT:
				this->stmt.process_stmt = process_stmt;
			break;
			
			case COMPONENT_INSTANTIANTION_STMT:
				this->stmt.instance_stmt = instance_stmt;
			break;
		}
	}
	
	int getType()
	{
		return type;
	}
	
};
#endif /*CARCHSTMT_H_*/
