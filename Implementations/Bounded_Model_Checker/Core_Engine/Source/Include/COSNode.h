#ifndef COSNODE_H_
#define COSNODE_H_


#include <list>
#include <map>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <set>

#include "CSensitiveElement.h"

#include "CExpression.h"
#include "CArchStmt.h"

using namespace std;

class CExpression;



class node
{
public:
	string name;
	list<class node*> out_list;
	list<class node*> in_list;
	
	list<OrientedStmt*> ostmts;
	CArchStmt * arch_stmt;
	
	int count;
	
	set <CSensitiveElement*> identifiers;
	set<CSensitiveElement*> output_identifiers;
	

	//CConSignAssgnStmt *stmt;

	bool start;
	bool end;
	node()
	{
		name = "";
		out_list.clear();
		in_list.clear();
		start = false;
		end = false;
		arch_stmt=NULL;
	}

	node(OrientedStmt *p_stmt)
	{
		name = "";
		out_list.clear();
		in_list.clear();
		start = false;
		end = false;
		ostmts.push_back(p_stmt);
		arch_stmt = NULL;
	}
	
	void addStmt(OrientedStmt *p_stmt)
	{
		ostmts.push_back(p_stmt);
	}
	
	void addStmt(CArchStmt *p_arch_stmt)
	{
		this->arch_stmt = p_arch_stmt;
	}
	
	
	void addStmts(list<OrientedStmt*>& p_stmts)
	{
		ostmts.splice(ostmts.end(), p_stmts);
	}
	
	void addToIncoming(node* n)
	{
		in_list.push_back(n);
	}
	
	void addToOutgoing(node* n)
	{
		out_list.push_back(n);
	}
	
	int getCount()
	{
		return count;
	}
	
	void setCount(int c)
	{
		count = c;
	}

};

class CSenNode
{
public:
	node* n;
	CSensitiveElement *se;
	CSenNode(node* n, CSensitiveElement *se)
	{
		this->n = n;
		this->se = se;
	}
};

#endif /*COSNODE_H_*/

/*
 class OrientedStmt
{
	COperand *lhs;
	list<CExpression*> values;
	list<CExpression*> conditions;
		
public:
	OrientedStmt(	COperand *p_lhs,
					list<CExpression*> p_values,
					list<CExpression*> p_conditions
				)
	{
		lhs = p_lhs;
		values.splice(values.begin(), p_values);;
		conditions.splice(conditions.begin(), p_conditions);;
	}
	
	void addValues(list<CExpression*> p_values, list<CExpression*> p_conditions)
	{
		values.splice(values.end(), p_values);;
		conditions.splice(conditions.end(), p_conditions);;
	}
};

class node
{
public:
	string name;
	list<class node*> out_list;
	list<class node*> in_list;
	
	list<OrientedStmt*> ostmts;

	CConSignAssgnStmt *stmt;

	bool visited;
	node()
	{
		name = "";
		out_list.clear();
		in_list.clear();
		visited = false;
	}

	node(OrientedStmt *p_stmt)
	{
		name = "";
		out_list.clear();
		in_list.clear();
		visited = false;
		ostmts.push_back(p_stmt);
	}
	
	void addStmt(OrientedStmt *p_stmt)
	{
		ostmts.push_back(p_stmt);
	}
	
	void addStmts(list<OrientedStmt*>& p_stmts)
	{
		ostmts.splice(ostmts.end(), p_stmts);
	}
};
 
 */
