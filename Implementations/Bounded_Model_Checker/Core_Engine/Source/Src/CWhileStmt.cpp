#include "CWhileStmt.h"

CWhileStmt::CWhileStmt()
{
}

CWhileStmt::CWhileStmt(CExpression* p_condition, CSeqStmt *p_body)
{
	condition = p_condition;
	body = p_body;
}


void CWhileStmt::print()
{
	//printf("\n*** loop statement ***\n");
	if(condition != NULL)
	{
		//cout<<endl<<"Condition: "<<endl;
		//condition->print(0);
	}

	//body->print();

	if(this->next != NULL)
	{
		//this->next->print();
	}
}

list<CSeqStmt *> CWhileStmt::getBranches()
{
	list<CSeqStmt *> temp_list;
	temp_list.push_back(body);

	return temp_list;
}

void CWhileStmt::createNode(FILE *fp, string parent)
{
	if(condition != NULL)
	{
		fprintf(fp, "seqstmt%u [label=\"while -- %s\"];\n",this, condition->getOrgExpr().c_str()); 

		fprintf(fp, "%s -> seqstmt%u;\n",parent.c_str(), this); 

	}

	char new_parent[50];

	sprintf(new_parent, "seqstmt%u", this);

	body->createNode(fp, new_parent);

	if(this->next != NULL)
	{
		this->next->createNode(fp, new_parent);
	}
}

void CWhileStmt::adjustStmt(string entityName)
{
	condition->getAndAdjustExpression(entityName); 

	body->adjustStmt(entityName);

	if(this->next != NULL)
	{
		this->next->adjustStmt(entityName);
	}
}

//void CWhileStmt::getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts)
//{
//}

void CWhileStmt::getChangedLHS(list<CSensitiveElement*>& sensitivityList)
{
}

// Added on 5/9/08

void CWhileStmt::getOutputs(list<ProcessSExpressions*> & SymbolicExpressions)
{
}

// End of additions

//Added on 9/9/08

void CWhileStmt::getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta)
{
}

// End of additions


void CWhileStmt::getChangedRHS(list<COperand*>& sensitivityList)
{
}

void CWhileStmt::createEdgeInArgDepGraph(Graph *ArgDepGraph)
{
}


