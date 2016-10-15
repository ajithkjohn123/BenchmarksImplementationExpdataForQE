#include "CCaseStmt.h"

extern CHelper MANAGER;

CCaseStmt::CCaseStmt()
{
}

CCaseStmt::CCaseStmt(CExpression* p_case_expr,
	list<CExpression*> p_case_choices,
	list<CSeqStmt *> p_case_statements)
{
	case_expr = p_case_expr;

	case_choices.splice(case_choices.begin(), p_case_choices);

	case_statements.splice(case_statements.begin(), p_case_statements);
}

void CCaseStmt::print()
{
	if(case_expr!=NULL)
	{
		//printf("\n*** sequential case statement ***\n");
		//cout<<endl<<"Expression: "<<endl;
		//case_expr->print(0);
	}
	else
	{
		//printf("\n*** sequential if statement ***\n");
	}

	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;
	int i;
	for(i=1, iterCond = case_choices.begin(), iterStmt = case_statements.begin();
	iterCond != case_choices.end() && iterStmt != case_statements.end();
	iterCond++,iterStmt++,i++)
	{
		//cout<<"Choice_"<<i<<":"<<endl;
		//(*iterCond)->print(0);

		//cout<<"Stmt_branch_"<<i<<":"<<endl;
		//(*iterStmt)->print();
	}
	
	if(this->next != NULL)
	{
		//this->next->print();
	}

}

list<CSeqStmt *> CCaseStmt::getBranches()
{
	return case_statements;
}

void CCaseStmt::createNode(FILE *fp, string parent)
{
	if(case_expr!=NULL)
	{
		fprintf(fp, "seqstmt%u [label=\"case -- %s\"];\n",this, case_expr->getOrgExpr().c_str()); 
	}
	else
	{
		fprintf(fp, "seqstmt%u [label=\"if\"];\n",this); 
	}

	fprintf(fp, "%s -> seqstmt%u;\n",parent.c_str(), this); 

	char new_parent[50];
	sprintf(new_parent, "seqstmt%u", this);
	
	int i;
	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;

	for(i=1, iterCond = case_choices.begin(), iterStmt = case_statements.begin();
	iterCond != case_choices.end() && iterStmt != case_statements.end();
	iterCond++,iterStmt++,i++)
	{
		fprintf(fp, "ch%u%d [shape=box,label=\"%s\"];\n",this,i, (*iterCond)->getOrgExpr().c_str());

		if(i==1)
		{
			fprintf(fp, "seqstmt%u -> ch%u%d;\n",this,this,i); 
		}
		else
		{
			fprintf(fp, "ch%u%d -> ch%u%d;\n",this,i-1,this,i);
		}

		char new_parent1[50];
		sprintf(new_parent1, "ch%u%d", this, i);

		(*iterStmt)->createNode(fp, new_parent1);
	}

	if(this->next != NULL)
	{
		this->next->createNode(fp, new_parent);
	}

}


void CCaseStmt::adjustStmt(string entityName)
{
	if(case_expr!=NULL)
	{
		//cout<<"case expr"<<endl;
		case_expr->getAndAdjustExpression(entityName); 
	}

	int i;
	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;
	//cout<<"after"<<endl;
	for(i=1, iterCond = case_choices.begin(), iterStmt = case_statements.begin();
	iterCond != case_choices.end() && iterStmt != case_statements.end();
	iterCond++,iterStmt++,i++)
	{
		//cout<<"condition"<<endl;
		(*iterCond)->getAndAdjustExpressionCondition(entityName);
		//cout<<"value"<<endl;

		(*iterStmt)->adjustStmt(entityName);
	}

	if(this->next != NULL)
	{
		//cout<<"next"<<endl;
		this->next->adjustStmt(entityName);
	}
}



void CCaseStmt::getChangedLHS(list<CSensitiveElement*>& sensitivityList)
{
	int i;
	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;
	//cout<<"after"<<endl;
	for(i=1, iterCond = case_choices.begin(), iterStmt = case_statements.begin();
	iterCond != case_choices.end() && iterStmt != case_statements.end();
	iterCond++,iterStmt++,i++)
	{
		(*iterStmt)->getChangedLHS(sensitivityList);
	}

	if(this->next != NULL)
	{
		this->next->getChangedLHS(sensitivityList);
	}
}


// Added on 5/9/08


void CCaseStmt::getOutputs(list<ProcessSExpressions*> & SymbolicExpressions)
{
       	int i;
	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;

	//cout<<"\nInside Case::getOutputs\n";

	for(i=1, iterCond = case_choices.begin(), iterStmt = case_statements.begin();
	iterCond != case_choices.end() && iterStmt != case_statements.end();
	iterCond++,iterStmt++,i++)
	{
		(*iterStmt)->getOutputs(SymbolicExpressions);
	}


	if(this->next != NULL)
	{
		this->next->getOutputs(SymbolicExpressions);
	}
}

// End of additions



// Added on 9/9/08


void CCaseStmt::getSymbolicExpressions(list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta)
{
	list<CSeqStmt*> statements = case_statements;
	list<CExpression*> choices = case_choices;

	list<ProcessSExpressions*> TempSymbolicExpressions, FinalSymbolicExpressions;
	
	set <string> setRelation; // Temporarily Added - To be removed later

	list<ProcessSExpressions*>::iterator symb_exp_it,final_symb_exp_it,temp_symb_exp_it;


	statements.reverse();
	choices.reverse();
	 	
	CExpression* caseExpr ;

	if(case_expr!=NULL)
	{
		caseExpr = case_expr;
	}

	int i;
	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;
	
	//cout<<"after"<<endl;

	//TempSymbolicExpressions = SymbolicExpressions;

	for(symb_exp_it = SymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end(); symb_exp_it++) 

		  {
		    map<CDAGNode*,CDAGNode*> nodeShiftednode;
		    set<string> setRelation;
                    CDAGNode *tempCDAGNode;

		    if((*symb_exp_it)->getValue() != NULL)
		      tempCDAGNode = MANAGER.cloneAndShift((*symb_exp_it)->getValue(), 0);
		    else
		      tempCDAGNode = NULL;

		    nodeShiftednode.clear();
		    ProcessSExpressions *os = new ProcessSExpressions((*symb_exp_it)->getLhs(), tempCDAGNode);
		    TempSymbolicExpressions.push_back(os);
		  }

	//FinalSymbolicExpressions = SymbolicExpressions;
	
		for(symb_exp_it = SymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end(); symb_exp_it++) 

		  {
		    map<CDAGNode*,CDAGNode*> nodeShiftednode;
		    set<string> setRelation;
		    CDAGNode *tempCDAGNode;

		    if((*symb_exp_it)->getValue() != NULL)
		      tempCDAGNode = MANAGER.cloneAndShift((*symb_exp_it)->getValue(), 0);
		    else
		      tempCDAGNode = NULL;
 
		    nodeShiftednode.clear();
		    ProcessSExpressions *os = new ProcessSExpressions((*symb_exp_it)->getLhs(), tempCDAGNode);
		    FinalSymbolicExpressions.push_back(os);
		  }

		//cout<<"\n\nPrinting final symbolic expressions\n";
		
		//for(final_symb_exp_it = FinalSymbolicExpressions.begin(); final_symb_exp_it != FinalSymbolicExpressions.end(); final_symb_exp_it++) 

		// {
		//    if((*final_symb_exp_it)->getValue() != NULL)
		//      cout<<endl<<(*final_symb_exp_it)->getValue()->getDAGStr();
		//    else
		//      cout<<"NULL";
		//  }

		//cout<<"\n\nPrinting Initial symbolic expressions\n";
		
		//for(final_symb_exp_it = TempSymbolicExpressions.begin(); final_symb_exp_it != TempSymbolicExpressions.end(); final_symb_exp_it++) 

		//  {
		//    if((*final_symb_exp_it)->getValue() != NULL)
		//      cout<<endl<<(*final_symb_exp_it)->getValue()->getDAGStr();
		//    else
		//      cout<<"NULL";
		//    
		//  }

		//cout<<"\n\nPrinting Symbolic expressions\n";
		
		//for(final_symb_exp_it = SymbolicExpressions.begin(); final_symb_exp_it != SymbolicExpressions.end(); final_symb_exp_it++) 

		// {
		//   if((*final_symb_exp_it)->getValue() != NULL)
		//      cout<<endl<<(*final_symb_exp_it)->getValue()->getDAGStr();
		//   else
		//      cout<<"NULL";
		//    
		//      }

		//cout<<"\n\nLoop Started\n\n";

	for(i=1, iterCond = choices.begin(), iterStmt = statements.begin();
	iterCond != choices.end() && iterStmt !=statements.end();
	iterCond++,iterStmt++,i++)
	{
	        //SymbolicExpressions = TempSymbolicExpressions;

	        for(symb_exp_it = SymbolicExpressions.begin(), temp_symb_exp_it = TempSymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end() && temp_symb_exp_it != TempSymbolicExpressions.end(); symb_exp_it++, temp_symb_exp_it++) 

		  {
		    map<CDAGNode*,CDAGNode*> nodeShiftednode;
		    set<string> setRelation;
		    CDAGNode *tempCDAGNode;

		    if((*temp_symb_exp_it)->getValue() != NULL)
		      tempCDAGNode = MANAGER.cloneAndShift((*temp_symb_exp_it)->getValue(), 0);
		    else
		      tempCDAGNode = NULL;
 
		    nodeShiftednode.clear();

		    (*symb_exp_it)->addValue(tempCDAGNode);
		    
		  }

		CDAGNode *final_cond,*op1,*op2,*temp1,*temp2;
		
		
		if(case_expr!=NULL)
		{
	
		  op1 = (case_expr)->getSymbolicExpressions(entName,delta,setRelation,SymbolicExpressions); 

		  if( (*iterCond)->getOperand() == "others" || (*iterCond)->getOperand() == "else" )
		    {final_cond = MANAGER.createDAG("true");}
		  else
		    {op2 = (*iterCond)->getSymbolicExpressions(entName,delta,setRelation,SymbolicExpressions);
		    final_cond = MANAGER.createDAG("=", op1, op2);}
		}
		else
		{
		  if( (*iterCond)->getOperand() == "others" || (*iterCond)->getOperand() == "else" )
		    {final_cond = MANAGER.createDAG("true");}
		  else
		    {final_cond = (*iterCond)->getSymbolicExpressions(entName,delta,setRelation,SymbolicExpressions); }
		}
	
		//cout<<"\nCondition Inside Case="<<final_cond->getDAGStr()<<endl;
		
		(*iterStmt)->getSymbolicExpressions(SymbolicExpressions,entName,delta);


		//cout<<"\nPrinting Initial Symbolic Expressions\n";
		
		//for(symb_exp_it = TempSymbolicExpressions.begin(); symb_exp_it != TempSymbolicExpressions.end(); symb_exp_it++) 

		//  {
		//    cout<<endl<<(*symb_exp_it)->getValue()->getDAGStr()<<endl;
		//  }


		//cout<<"\nPrinting Symbolic Expressions Of If Part\n";
		
		//for(symb_exp_it = SymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end(); symb_exp_it++) 

		// {
		//   cout<<"\t"<<(*symb_exp_it)->getValue()->getDAGStr()<<endl;
		// }
		

		//cout<<"\n\nCreating final symbolic expressions\n";
		
		for(symb_exp_it = SymbolicExpressions.begin(), final_symb_exp_it = FinalSymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end() && final_symb_exp_it != FinalSymbolicExpressions.end(); symb_exp_it++, final_symb_exp_it++) 

		  {
		    temp1 = (*symb_exp_it)->getValue();
		    temp2 = (*final_symb_exp_it)->getValue();

		    // cout<<"\nNew SE\t"<<temp1->getDAGStr();
		    //cout<<"\nExisting SE\t"<<temp2->getDAGStr();
		    
		    (*final_symb_exp_it)->addValue(MANAGER.createDAG("ite", final_cond, temp1, temp2));
		  }

		
		//cout<<"\n\nPrinting final symbolic expressions\n";
		
		//for(final_symb_exp_it = FinalSymbolicExpressions.begin(); final_symb_exp_it != FinalSymbolicExpressions.end(); final_symb_exp_it++) 

		// {
		//    cout<<endl<<(*final_symb_exp_it)->getValue()->getDAGStr();
		//  }

		//cout<<"\n\nPrinting Initial symbolic expressions\n";
		
		//for(final_symb_exp_it = TempSymbolicExpressions.begin(); final_symb_exp_it != TempSymbolicExpressions.end(); final_symb_exp_it++) 

		//  {
		//    cout<<endl<<(*final_symb_exp_it)->getValue()->getDAGStr();
		//  }

		//cout<<"\n\nPrinting Symbolic expressions\n";
		
		//for(final_symb_exp_it = SymbolicExpressions.begin(); final_symb_exp_it != SymbolicExpressions.end(); final_symb_exp_it++) 

		//  {
		//    cout<<endl<<(*final_symb_exp_it)->getValue()->getDAGStr();
		//  }

		//cout<<"\n\nOne pass completed\n\n";
		
	}

	//SymbolicExpressions = FinalSymbolicExpressions;

	 for(symb_exp_it = SymbolicExpressions.begin(), final_symb_exp_it = FinalSymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end() && final_symb_exp_it != FinalSymbolicExpressions.end(); symb_exp_it++, final_symb_exp_it++) 

		  {
		    map<CDAGNode*,CDAGNode*> nodeShiftednode;
		    set<string> setRelation;
		    CDAGNode *tempCDAGNode;

		    if((*final_symb_exp_it)->getValue() != NULL)
		      tempCDAGNode = MANAGER.cloneAndShift((*final_symb_exp_it)->getValue(), 0);
		    else
		      tempCDAGNode = NULL;
 
		    nodeShiftednode.clear();

		    (*symb_exp_it)->addValue(tempCDAGNode);
		    
		  }


	if(this->next != NULL)
	{
	        //cout<<"next"<<endl;
		this->next->getSymbolicExpressions(SymbolicExpressions,entName,delta);
	}
}


// End of additions


// Additions on 30/9/08

void CCaseStmt::getChangedRHS(list<COperand*>& sensitivityList)
{
	int i;
	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;
	//cout<<"after"<<endl;
	
	if(case_expr != NULL)
	  case_expr->getChangedRHS(sensitivityList);

	for(i=1, iterCond = case_choices.begin(), iterStmt = case_statements.begin();
	iterCond != case_choices.end() && iterStmt != case_statements.end();
	iterCond++,iterStmt++,i++)
	{
	        (*iterCond)->getChangedRHS(sensitivityList);
		(*iterStmt)->getChangedRHS(sensitivityList);
	}

	if(this->next != NULL)
	{
		this->next->getChangedRHS(sensitivityList);
	}
}

// End of additions

void CCaseStmt::createEdgeInArgDepGraph(Graph *ArgDepGraph)
{
  
	int i;
	list<CExpression*>::iterator iterCond;
	list<CSeqStmt *>::iterator iterStmt;
	//	cout<<"after"<<endl;
	
	//	if(case_expr != NULL)
	//  case_expr->getChangedRHS(sensitivityList);

	for(i=1, iterCond = case_choices.begin(), iterStmt = case_statements.begin();
	iterCond != case_choices.end() && iterStmt != case_statements.end();
	iterCond++,iterStmt++,i++)
	{
	        //(*iterCond)->getChangedRHS(sensitivityList);
		(*iterStmt)->createEdgeInArgDepGraph(ArgDepGraph);
	}

	if(this->next != NULL)
	{
		this->next->createEdgeInArgDepGraph(ArgDepGraph);
	}
}


