#include "CAssgnStmt.h"

CAssgnStmt::CAssgnStmt()
{
}

CAssgnStmt::CAssgnStmt(COperand *p_lhs, CExpression* p_rhs, int type):CSeqStmt(NULL)
{
	lhs = p_lhs;
	rhs = p_rhs;
	type_of_assignment = type;
}

void CAssgnStmt::print()
{
	if(type_of_assignment == SIGNAL_ASSIGNMENT)
	{
		//cout<<endl<<"\n***Signal assignment statement***\n"<<endl;
	}
	else
	{
		//cout<<endl<<"\n***Variable assignment statement***\n"<<endl;
	}

	//cout<<endl<<"LHS: "<<endl;
	//lhs->print();

	//cout<<endl<<"RHS: "<<endl;
	//rhs->print(0);

	if(this->next != NULL)
	{
		this->next->print();
	}

}

void CAssgnStmt::createNode(FILE *fp, string parent)
{
	if(type_of_assignment == SIGNAL_ASSIGNMENT)
	{
		fprintf(fp, "seqstmt%u [label=\"%s SIG_ASSGN %s\"];\n",this, lhs->getOper().c_str(), rhs->getOrgExpr().c_str() ); 
	}
	else
	{
		fprintf(fp, "seqstmt%u [label=\"%s VAR_ASSGN %s\"];\n",this, lhs->getOper().c_str(), rhs->getOrgExpr().c_str() ); 
	}

	fprintf(fp, "%s -> seqstmt%u;\n",parent.c_str(), this); 
	
	char new_parent[50];

	sprintf(new_parent, "seqstmt%u", this);

	if(this->next != NULL)
	{
		this->next->createNode(fp, new_parent);
	}
}

void CAssgnStmt::adjustStmt(string entityName)
{
	int numberofbits = 0;
	numberofbits = rhs->getAndAdjustExpression(entityName);

	if(numberofbits != -1)
	{
	  if(lhs->unAdjusted())
	    {
		lhs->setLowerBits(numberofbits);
	    }
	}
	else
	{
		lhs->adjustRange(entityName);
	}

	if(this->next != NULL)
	{
		this->next->adjustStmt(entityName);
	}
}

list<ProcessSExpressions*>::iterator CAssgnStmt::IsPresent (list<ProcessSExpressions*> & ostmts, COperand * toBeSearched)
{
	list<ProcessSExpressions*>::iterator  ios;
	for(ios = ostmts.begin();ios != ostmts.end();ios++)
	{
		if(toBeSearched->getName() == (*ios)->getLhs()->getName() && 
			((CAtom*)toBeSearched)->getLower() == ((CAtom*)((*ios)->getLhs()))->getLower() &&
			((CAtom*)toBeSearched)->getUpper() == ((CAtom*)((*ios)->getLhs()))->getUpper())
		{
			return ios;
		}	
	}
	
	return ostmts.end();
}


bool CAssgnStmt::sePresentInSensitveList(list<CSensitiveElement*> sensitivityList, CSensitiveElement* se)
{
  
  for(list<CSensitiveElement*>::iterator sensitivityList_it = sensitivityList.begin(); sensitivityList_it != sensitivityList.end(); sensitivityList_it++)
    {
      if( (se->name == (*sensitivityList_it)->name) && (se->lower == (*sensitivityList_it)->lower) && (se->upper == (*sensitivityList_it)->upper) )
	{
	  return true;
	}
    }

  return false;
}
 

void CAssgnStmt::getChangedLHS(list<CSensitiveElement*>& sensitivityList)
{
	CSensitiveElement* se = new CSensitiveElement(((CAtom*)lhs)->getName(), ((CAtom*)lhs)->getUpper(), ((CAtom*)lhs)->getLower());

	
	//if( !sePresentInSensitveList(sensitivityList, se))
	  sensitivityList.push_back(se);
	
	if(this->next != NULL)
	{
		this->next->getChangedLHS(sensitivityList);
	}
}

// Added on 5/9/08

void CAssgnStmt::getOutputs(list<ProcessSExpressions*> & SymbolicExpressions)
{

	COperand *lhs1 = lhs;

	list<ProcessSExpressions*>::iterator it = IsPresent(SymbolicExpressions, lhs1);
	

	if(it == SymbolicExpressions.end())
	{
		ProcessSExpressions *os = new ProcessSExpressions(lhs1, NULL);
		SymbolicExpressions.push_back(os);
	}
	
	//cout<<"\n\n*******This Node is************\n";

	//this->print(); 

	//cout<<"\n*******************Output= "<<temp<<"\n\n";
	
	
	if(this->next != NULL)
	{
	   
		(this->next)->getOutputs(SymbolicExpressions);
	}
	
}

// End of additions on 5/9/08


// Added on 9/9/08

void CAssgnStmt::getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta)
{

	COperand *lhs1 = lhs;

	CExpression *rhs1= rhs;

	set <string> setRelation; // Temporarily Added - To be removed later

	list<ProcessSExpressions*>::iterator it = IsPresent(SymbolicExpressions, lhs1);
	
	CDAGNode *SymbExpForRHSInProcess = rhs1->getSymbolicExpressions(entName,delta,setRelation,SymbolicExpressions);


	//cout<<"\n\n****LHS******\t"<<temp;

	//cout<<"\n*******RHS*****\t"<<SymbExpForRHSInProcess->getDAGStr()<<endl;

	if(it != SymbolicExpressions.end())
	{
		(*it)->addValue(SymbExpForRHSInProcess);
	}
	else
	{
	  cout<<"\nError in getSymbolicExpressions. Exiting";
	  exit(1);
	}
		
	//cout<<"\n\n*******This Node is************\n";

	//this->print(); 

	//cout<<"\n*******************\n";
	
	
	if(this->next != NULL)
	{
	   
		(this->next)->getSymbolicExpressions(SymbolicExpressions,entName,delta);
	}
	
}

// End of additions on 9/9/08


// On 29/9/08

void CAssgnStmt::getChangedRHS(list<COperand*>& sensitivityList)
{
  (this->rhs)->getChangedRHS(sensitivityList);

	if(this->next != NULL)
	{
		this->next->getChangedRHS(sensitivityList);
	}
}

// End of additions on 16/9/08

void CAssgnStmt::createEdgeInArgDepGraph(Graph *ArgDepGraph)
{
  COperand *lhs1 = lhs;
  list<COperand*> sensitivityList;
  
  rhs->getChangedRHS(sensitivityList);

  list<COperand*>::iterator sensitivityList_it;

  for(sensitivityList_it = sensitivityList.begin();sensitivityList_it != sensitivityList.end(); sensitivityList_it++)
    {
      ArgDepGraph->AddEdge(((CAtom*)(*sensitivityList_it))->getName(), ((CAtom*)lhs1)->getName());
    }

  if(this->next != NULL)
	{
	this->next->createEdgeInArgDepGraph(ArgDepGraph);
	}
  
}
