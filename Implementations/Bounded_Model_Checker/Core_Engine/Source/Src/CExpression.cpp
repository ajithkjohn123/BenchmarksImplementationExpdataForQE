#include "CExpression.h"
#include "y.tab.h"

extern CHelper MANAGER;

CExpression::CExpression()
{

}

void CExpression::getSensitivityList(list<CSensitiveElement*>& sensitivityList)
{
	CExpression * expr = this;

	if(expr->isLhsOperand())
	{
		COperand *op = expr->getLhsOperand();
		if(op->getType() == t_Identifier && op->getName() != "others")
		{
			CSensitiveElement* se = new CSensitiveElement(((CAtom*)op)->getName(), ((CAtom*)op)->getUpper(), ((CAtom*)op)->getLower());
			sensitivityList.push_back(se);
		}
	}
	else
	{
		(expr->getLhsExpr())->getSensitivityList(sensitivityList);
	}


	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
//			sensitivityList.push_back(expr->getOperand());
			COperand *op = expr->getRhsOperand();
			
			if(op->getType() == t_Identifier && op->getName() != "others")
			{
				CSensitiveElement* se = new CSensitiveElement(((CAtom*)op)->getName(), ((CAtom*)op)->getUpper(), ((CAtom*)op)->getLower());
				sensitivityList.push_back(se);
			}
		}
		else
		{
			(expr->getRhsExpr())->getSensitivityList(sensitivityList);
		}
	}
}


void CExpression::getChangedRHS(list<COperand*>& sensitivityList)
{
	CExpression * expr = this;

	if(expr->isLhsOperand())
	{
		COperand *op = expr->getLhsOperand();
		if(op->getType() == t_Identifier && op->getName() != "others")
		{
			sensitivityList.push_back(op);
		}
	}
	else
	{
		(expr->getLhsExpr())->getChangedRHS(sensitivityList);
	}


	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
//			sensitivityList.push_back(expr->getOperand());
			COperand *op = expr->getRhsOperand();
			
			if(op->getType() == t_Identifier && op->getName() != "others")
			{
				sensitivityList.push_back(op);
			}
		}
		else
		{
			(expr->getRhsExpr())->getChangedRHS(sensitivityList);
		}
	}
}



CExpression::CExpression( string oper, 
			bool is_op1, struct expr_oper *op1,
			bool is_op2, struct expr_oper *op2,
			bool is_unary)
{
	this->oper = oper;
	this->is_operator1 = is_op1;
	this->is_operator2 = is_op2;
	this->is_unary = is_unary;

	if(is_op1)
	{
		this->lhs.op_lhs = op1->exp_or_op.oper;
	}
	else
	{
		this->lhs.expr_lhs = op1->exp_or_op.expr;

	}

	if(!is_unary)
	{
		if(is_op2)
		{
			this->rhs.op_rhs = op2->exp_or_op.oper;
		}
		else
		{
			this->rhs.expr_rhs = op2->exp_or_op.expr;
	
		}
	}
}

string CExpression::getOperand()
{
	return oper;
}

bool CExpression::isLhsOperand()
{
	return is_operator1;
}

bool CExpression::isRhsOperand()
{
	return is_operator2;
}

COperand * CExpression::getLhsOperand()
{
	return lhs.op_lhs;
}

COperand * CExpression::getRhsOperand()
{
	return rhs.op_rhs;
}

CExpression * CExpression::getLhsExpr()
{
	return lhs.expr_lhs;
}

CExpression * CExpression::getRhsExpr()
{
	return rhs.expr_rhs;
}

bool CExpression::isUnary()
{
	return is_unary;
}

void CExpression::print(int num)
{
	CExpression * expr = this;

	cout<<endl<<"[[ expression "<<num<<endl;
	
	cout<<"Operand: "<<expr->getOperand()<<endl;
	if(expr->isLhsOperand())
	{
		COperand *op = expr->getLhsOperand();
		cout<<"LHS:"<<endl;
		op->print();
	}
	else
	{
		cout<<"LHS:      expression"<<(num + 1)<<endl;
		(expr->getLhsExpr())->print(num+1);
		
		
	}

	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
			COperand *op = expr->getRhsOperand();
			cout<<"RHS:"<<endl;
			op->print();
		}
		else
		{
			cout<<"RHS:      expression"<<(num + 2)<<endl;
			
			(expr->getRhsExpr())->print(num+2);
		}
	}

	cout<<endl<<"]]"<<endl;
}

string CExpression::getOrgExpr()
{
	CExpression * expr = this;

	string str_expr = "(";

	if(expr->isUnary() && (expr->getOperand() != "operand") )
	{
		str_expr = str_expr + "\\" + expr->getOperand() + " ";
	}
	
	if(expr->isLhsOperand())
	{
		COperand *op = expr->getLhsOperand();
		str_expr = str_expr + op->getOper() + " ";
	}
	else
	{
		str_expr = str_expr + (expr->getLhsExpr())-> getOrgExpr() + " ";
	}

	if(!expr->isUnary())
	{
		str_expr = str_expr + "\\"  + expr->getOperand() + " ";
	}
	

	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
			COperand *op = expr->getRhsOperand();
			str_expr = str_expr + op->getOper() + " ";
		}
		else
		{
			str_expr = str_expr + (expr->getRhsExpr())-> getOrgExpr() + " ";
		}
	}

	str_expr = str_expr + ")";

	return str_expr;
}

void CExpression::getAndAdjustExpressionCondition(string entityName)
{
	CExpression * expr = this;
	//cout<<"getAndAdjustExpressionCondition - start"<<endl;
	if(expr->isLhsOperand())
	{
		//cout<<"expr->isLhsOperand()"<<endl;
		COperand *op = expr->getLhsOperand();
		op->adjustRange(entityName); 
	}
	else
	{
		//cout<<"else1"<<endl;
		(expr->getLhsExpr())->getAndAdjustExpression(entityName);
	}

	//cout<<"outside1"<<endl;

	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
			//cout<<"expr->isRhsOperand()"<<endl;
			COperand *op = expr->getRhsOperand();
			op->adjustRange(entityName); 
		}
		else
		{
			//cout<<"else2"<<endl;
			(expr->getRhsExpr())->getAndAdjustExpression(entityName);
		}
	}

		//cout<<"getAndAdjustExpressionCondition - end"<<endl;
}

int CExpression::getAndAdjustExpression(string entityName)
{
	CExpression * expr = this;
	int nob_lhs = 0, nob_rhs = 0;

	if(expr->isLhsOperand())
	{
		//cout<<"expr->isLhsOperand()"<<endl;
		COperand *op = expr->getLhsOperand();
		nob_lhs = op->adjustRange(entityName); 
	}
	else
	{
		//cout<<"else1"<<endl;
		nob_lhs = (expr->getLhsExpr())->getAndAdjustExpression(entityName);
	}

	//cout<<"outside1"<<endl;
	if(expr->isUnary())
	{
		return nob_lhs;
	}
	//cout<<"outside2"<<endl;
	
	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
			//cout<<"expr->isRhsOperand()"<<endl;
			COperand *op = expr->getRhsOperand();
			nob_rhs = op->adjustRange(entityName); 
		}
		else
		{
			//cout<<"else2"<<endl;
			nob_rhs = (expr->getRhsExpr())->getAndAdjustExpression(entityName);
		}
	}

	//cout<<"fffffffffffff"<<endl;
	if(nob_rhs == nob_lhs)
	{
		//cout<<"nob_rhs == nob_lhs"<<endl;
		return nob_rhs;
	}
	else if(nob_rhs == -1 && nob_lhs != -1)
	{
		//cout<<"nob_rhs == -1 && nob_lhs != -1"<<endl;
		return nob_lhs;
	}
	else if (nob_rhs != -1 && nob_lhs == -1)
	{
		//cout<<"nob_rhs != -1 && nob_lhs == -1"<<endl;
		return nob_rhs;
	}
	else
	{
		// Added by Ajith On 12 Sep '13
		string WhoamI = expr->getOperand();
		if(WhoamI == "and" || WhoamI =="nand" || WhoamI == "nor" || WhoamI == "xnor" || WhoamI == "or" || WhoamI == "xor" || WhoamI == "not" || WhoamI == "<=" || WhoamI == ">=" || WhoamI == "<" || WhoamI == ">" || WhoamI == "=" || WhoamI == "/=") //Logical operator
		{
			//cout<<"logical operator"<<endl;
			return 1;
		}
		else
		{			
		// Added by Ajith On 12 Sep '13 ends here
			cout<< "ERROR - left hand side and right hand side bits are mismatching in "<<endl;
			this->print(0);
			exit(1);
		}
	}
}

/*CDAGNode* CExpression::getDAG(string entName, int previous, map<string, bool> &previously_changed)
{
	CDAGNode* expr_dag = NULL;
	CDAGNode* lhs_dag = NULL;
	CDAGNode* rhs_dag = NULL;
	
	CExpression* expr = this;

	if(expr->isLhsOperand())
	{
		COperand *op = expr->getLhsOperand();
		if(previously_changed.find(op->getName()) == previously_changed.end())
		{
			lhs_dag = op->getDAG(entName, previous);
		}
		else
		{
			lhs_dag = op->getDAG(entName, previous+1);	
		}
	}
	else
	{
		
		lhs_dag = (expr->getLhsExpr())-> getDAG(entName, previous, previously_changed);
	}
	
	if(expr->isUnary() && (expr->getOperand() != "operand") )
	{
		expr_dag =  MANAGER.createDAG(expr->getOperand(), lhs_dag); 
	}
	else if(expr->isUnary() && (expr->getOperand() == "operand"))
	{
		expr_dag = lhs_dag; 
	}
	

	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
			COperand *op = expr->getRhsOperand();
			
			if(previously_changed.find(op->getName()) == previously_changed.end())
			{
				rhs_dag = op->getDAG(entName, previous);
			}
			else
			{
				rhs_dag = op->getDAG(entName, previous+1);	
			}
		}
		else
		{
			rhs_dag = (expr->getRhsExpr())-> getDAG(entName, previous, previously_changed);
		}
		
		expr_dag =  MANAGER.createDAG(expr->getOperand(), lhs_dag, rhs_dag); 
	}

	return expr_dag;	
}*/

CDAGNode* CExpression::getDAG(string entName, int delta, set<string>& setRelation)
{
	CDAGNode* expr_dag = NULL;
	CDAGNode* lhs_dag = NULL;
	CDAGNode* rhs_dag = NULL;
	
	CExpression* expr = this;
	
	if(expr->getOperand() == "'")
	{
		if(expr->getRhsOperand()->getName() == "event")
		{
			lhs_dag = (expr->getLhsOperand())-> getDAG(entName, delta, setRelation);
			rhs_dag = (expr->getLhsOperand())-> getDAG(entName, delta-1, setRelation);
			expr_dag = MANAGER.createDAG("not_equal_to", lhs_dag, rhs_dag);
		}
		else if(expr->getRhsOperand()->getName() == "lastvalue")
		{
			expr_dag = (expr->getRhsExpr())-> getDAG(entName, delta-1, setRelation);
		}
		else
		{
			cout<< "Non-supported attribute"<< expr->getRhsOperand() <<endl;
			exit(1);
		}
		
		return expr_dag;
	}

	if(expr->isLhsOperand())
	{
		COperand *op = expr->getLhsOperand();
		lhs_dag = op->getDAG(entName, delta, setRelation);
	}
	else
	{
		lhs_dag = (expr->getLhsExpr())-> getDAG(entName, delta, setRelation);
	}
	
	if(expr->isUnary() && (expr->getOperand() != "operand") )
	{
		expr_dag =  MANAGER.createDAG(expr->getOperand(), lhs_dag); 
	}
	else if(expr->isUnary() && (expr->getOperand() == "operand"))
	{
		expr_dag = lhs_dag; 
	}
	

	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
			COperand *op = expr->getRhsOperand();
			rhs_dag = op->getDAG(entName, delta, setRelation);
		}
		else
		{
			rhs_dag = (expr->getRhsExpr())-> getDAG(entName, delta, setRelation);
		}
		
		expr_dag =  MANAGER.createDAG(expr->getOperand(), lhs_dag, rhs_dag); 
	}

	return expr_dag;	
}




CDAGNode* CExpression::getSymbolicExpressions(string entName, int delta, set<string>& setRelation,  list<ProcessSExpressions*> & SymbolicExpressions)
{
	CDAGNode* expr_dag = NULL;
	CDAGNode* lhs_dag = NULL;
	CDAGNode* rhs_dag = NULL;
	
	CExpression* expr = this;
	
	if(expr->getOperand() == "'")
	{
		if(expr->getRhsOperand()->getName() == "event")
		{
			lhs_dag = (expr->getLhsOperand())-> getSymbolicExpressions(entName, delta, setRelation, SymbolicExpressions);
			rhs_dag = (expr->getLhsOperand())-> getSymbolicExpressions(entName, delta-1, setRelation, SymbolicExpressions);
			expr_dag = MANAGER.createDAG("not_equal_to", lhs_dag, rhs_dag);
		}
		else if(expr->getRhsOperand()->getName() == "lastvalue")
		{
			expr_dag = (expr->getRhsExpr())-> getSymbolicExpressions(entName, delta-1, setRelation, SymbolicExpressions);
		}
		else
		{
			cout<< "Non-supported attribute"<< expr->getRhsOperand() <<endl;
			exit(1);
		}
		
		return expr_dag;
	}

	if(expr->isLhsOperand())
	{
		COperand *op = expr->getLhsOperand();
		lhs_dag = op->getSymbolicExpressions(entName, delta, setRelation, SymbolicExpressions);
	}
	else
	{
		lhs_dag = (expr->getLhsExpr())-> getSymbolicExpressions(entName, delta, setRelation, SymbolicExpressions);
	}
	
	if(expr->isUnary() && (expr->getOperand() != "operand") )
	{
		expr_dag =  MANAGER.createDAG(expr->getOperand(), lhs_dag); 
	}
	else if(expr->isUnary() && (expr->getOperand() == "operand"))
	{
		expr_dag = lhs_dag; 
	}
	

	if(!expr->isUnary())
	{
		if(expr->isRhsOperand())
		{
			COperand *op = expr->getRhsOperand();
			rhs_dag = op->getSymbolicExpressions(entName, delta, setRelation, SymbolicExpressions);
		}
		else
		{
			rhs_dag = (expr->getRhsExpr())-> getSymbolicExpressions(entName, delta, setRelation, SymbolicExpressions);
		}
		
		expr_dag =  MANAGER.createDAG(expr->getOperand(), lhs_dag, rhs_dag); 
	}

	return expr_dag;	
}



