#include "CWaitStmt.h"

CWaitStmt::CWaitStmt()
{
}

CWaitStmt::CWaitStmt(CExpression* p_condition,
	CExpression* p_timeout,
	list<string> p_sensitivity)

{
	condition = p_condition;
	timeout = p_timeout;
	sensitivity.splice(sensitivity.begin(), p_sensitivity);

}


void CWaitStmt::print()
{
	//printf("\n*** wait statement ***\n");
	if(timeout != NULL)
	{
		//cout<<"Time out: "<<endl;	
		//timeout->print(0);
	}

	if(condition != NULL)
	{
		//cout<<endl<<"Condition: "<<endl;
		//condition->print(0);
	}
	list<string>::iterator isen;


	//cout<<endl<<"Sensitivity: "<<endl;

	for(	isen = sensitivity.begin();
		isen != sensitivity.end();
		isen++)
	{
		//cout<<(*isen)+"  ";	
	}

	//cout<<endl;

	if(this->next != NULL)
	{
		//this->next->print();
	}
}

void CWaitStmt::createNode(FILE *fp, string parent)
{
	fprintf(fp, "seqstmt%u [label=\"WAIT\"];\n",this); 
	fprintf(fp, "%s -> seqstmt%u;\n",parent.c_str(), this); 

	if(timeout != NULL)
	{
		fprintf(fp, "timeout%u [label=\"timeout--%s\"];\n",this, timeout->getOrgExpr().c_str() ); 
		fprintf(fp, "seqstmt%u -> timeout%u;\n",this, this); 
	}

	if(condition != NULL)
	{
		fprintf(fp, "condition%u [label=\"condition--%s\"];\n",this, condition->getOrgExpr().c_str() ); 
		fprintf(fp, "seqstmt%u -> condition%u;\n",this, this); 
	}

	if(sensitivity.size() != 0)
	{
		list<string>::iterator isen;
		string lst;
		for(	isen = sensitivity.begin();
			isen != sensitivity.end();
			isen++)
		{
			lst = lst + (*isen) + " " ;
		}
	
		fprintf(fp, "sen%u [label=\"sensitivity--%s\"];\n",this, lst.c_str() ); 
		fprintf(fp, "seqstmt%u -> sen%u;\n",this, this); 
	}

	char new_parent[50];
	sprintf(new_parent, "seqstmt%u", this);

	if(this->next != NULL)
	{
		this->next->createNode(fp, new_parent);
	}
}

void CWaitStmt::adjustStmt(string entityName)
{
	if(timeout != NULL)
	{
		timeout->getAndAdjustExpression(entityName); 
	}

	if(condition != NULL)
	{
		condition->getAndAdjustExpression(entityName); 
	}

	if(this->next != NULL)
	{
		this->next->adjustStmt(entityName);
	}
}

//void CWaitStmt::getOrientedStmts(CExpression* cond, list<OrientedStmt*> & ostmts)
//{
//}


void CWaitStmt::getChangedLHS(list<CSensitiveElement*>& sensitivityList)
{
}

// Added on 5/9/08

void CWaitStmt::getOutputs(list<ProcessSExpressions*> & SymbolicExpressions)
{
}

// End of additions

// Added on 9/9/08

void CWaitStmt::getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta)
{
}

// End of additions

void CWaitStmt::getChangedRHS(list<COperand*>& sensitivityList)
{
}

void CWaitStmt::createEdgeInArgDepGraph(Graph *ArgDepGraph)
{
}
