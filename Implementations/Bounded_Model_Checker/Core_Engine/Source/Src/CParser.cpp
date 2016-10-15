#include "CParser.h"
#include "y.tab.h"

void CParser::printPort(list<CPort> portList)
{
	//printf("\nin CParser::printPort()\n");

	list<CPort>::iterator iport;
	for (iport=portList.begin(); 
	iport!=portList.end(); 
	iport++)
	{
		//cout<<"Name: "<<(*iport).getName()<<endl;
		//cout<<"Mode: "<<(*iport).getModeStr()<<endl;
		//cout<<"Type: "<<(*iport).getType()<<endl;
		fflush(stdout);	
		string value = (*iport).getValue()==NULL?"":((*iport).getValue())->getOrgExpr();	
		//cout<<"Value: "<<value<<endl;
		fflush(stdout);		
		list<string> uList =  (*iport).getUpper();
		list<string> lList =  (*iport).getLower();

		list<string>::iterator iterList;

		for (iterList = uList.begin();
			iterList != uList.end();
			iterList++)
		{
			//cout<<"  "<<(*iterList);
		}
		
		//cout<<endl;
		for (iterList = lList.begin();
			iterList != lList.end();
			iterList++)
		{
			//cout<<"  "<<(*iterList);
		}
		//out<<endl;
 		fflush(stdout);		
	}	
}

COperand* CParser::createOperand(struct IDinfo* id)
{
  //cout<<"\nCParser::createOperand(struct IDinfo* id) called\n";
  //cout<<"\nid->type  = "<<id->type<<endl;
  //cout<<"\nt_Identifier = "<<t_Identifier<<endl;
  //string x = id->name; // Temporary
  //cout<<"\nChecking Over1\n";
  //if(id->type == t_Identifier) cout<<"\nIdentifier\n";   else cout<<"\nLiteral\n"; 
  //cout<<"\nChecking Over2\n";
	if(id->type == t_Identifier)
	{
	  //cout<<"\nID type is identifier\t";
	  //cout<<"\nID type is identifier\t";
	  //cout<<"\nID type is identifier\t";
	  //cout<<"\nID type is identifier\t";
	  //cout<<"\nID type is identifier\t";
	  //cout<<"\nID type is identifier\t";

		string name = id->name;

		//cout<<"\nHere1\t";

		string upper = "-1";

		//cout<<"\nHere1.1\t";

		string lower = "-1";

		//cout<<"\nHere1.2\t";

		string unit = "";

		//cout<<"\nHere2\t";
		//cout<<"\nHere2\t";
		//cout<<"\nHere2\t";
		//cout<<"\nHere2\t";
		//cout<<"\nHere2\t";
		//cout<<"\nHere2\t";
		//cout<<"\nHere2\t";
		//cout<<"\nHere2\t";

		if(id->upper != NULL)
		{
		  //cout<<"\nHere2.1\t";
		  //cout<<"\nHere2.1\t";
		  //cout<<"\nHere2.1\t";
		  //cout<<"\nHere2.1\t";
		  //cout<<"\nHere2.1\t";
		  //cout<<"\nHere2.1\t";
		  //cout<<"\n"<<id->upper<<"\t";
		  //cout<<"\n"<<id->upper<<"\t";
		  //cout<<"\n"<<id->upper<<"\t";
		  //cout<<"\n"<<id->upper<<"\t";
		  //cout<<"\n"<<id->upper<<"\t";

			upper = id->upper;

			//cout<<"\nHere2.2\t";
			//cout<<"\nHere2.2\t";
			//cout<<"\nHere2.2\t";
			//cout<<"\nHere2.2\t";
			//cout<<"\nHere2.2\t";
			//cout<<"\nHere2.2\t";
		}
		
		//cout<<"\nHere3\t";
		//cout<<"\nHere3\t";
		//cout<<"\nHere3\t";
		//cout<<"\nHere3\t";
		//cout<<"\nHere3\t";
		//cout<<"\nHere3\t";

		if(id->lower != NULL)
		{
			lower = id->lower;
		}

		//cout<<"\nHere4\t";
	// 	cout<<"\nHere4\t";
// 		cout<<"\nHere4\t";
// 		cout<<"\nHere4\t";
// 		cout<<"\nHere4\t";

		if(id->unit != NULL)
		{
			unit = id->unit;
		}

	// 	cout<<"\nHere5\t";
// 		cout<<"\nHere5\t";
// 		cout<<"\nHere5\t";
// 		cout<<"\nHere5\t";
// 		cout<<"\nHere5\t";
// 		cout<<"\nHere5\t";

		CAtom * at = new CAtom(id->type, name, lower, upper, unit);
		
		// cout<<"\nHere6\t";
// 		cout<<"\nHere6\t";
// 		cout<<"\nHere6\t";
// 		cout<<"\nHere6\t";
// 		cout<<"\nHere6\t";
// 		cout<<"\nHere6\t";

		//CAtom * at = new CAtom(id->type, name, lower, upper); // Newly Changed

	// 	cout<<"\nHere7\t";
// 		cout<<"\nHere7\t";
// 		cout<<"\nHere7\t";
// 		cout<<"\nHere7\t";
// 		cout<<"\nHere7\t";
// 		cout<<"\nHere7\t";
		return at;
	}
	else
	{
	   //cout<<"\nID type is literal\n";
	   //cout<<"name is "<<id->name<<"\n";

		string unit = "";

		/*if(id->unit != NULL)
		{
			unit = id->unit;
		}*/

		CLiteral * lt = new CLiteral (id->type, id->name, unit);
		return lt;
	}
}

COperand*  CParser::createOperand(string name)
{

        //cout<<"\nCParser::createOperand(string name) called\n";
	string upper = "-1";
	string lower = "-1";

	//cout<<"\nname = "<<name<<", lower = "<<lower<<", upper = "<<upper<<",t_Identifier = "<<t_Identifier<<endl; 

	CAtom * at = new CAtom(t_Identifier, name, lower, upper);
	//cout<<"\n"<<name<<" created \n"; // newly added
	return at;
}

bool CParser::isConstFolding(struct expr_oper *op1, struct expr_oper *op2)
{
	bool is_op1 = op1->is_oper;
	bool is_op2 = op2->is_oper;

	if(is_op1 && is_op2)
	{
		if(((op1->exp_or_op).oper)->getType()==t_Identifier || 
			((op2->exp_or_op).oper)->getType()==t_Identifier)
		{
			return false;
		}
		else
		{
			return true;
		}
		
	}	

	return false;
}

COperand* CParser::doConstFolding(string oper, struct expr_oper *op1, struct expr_oper *op2)
{
	bool is_op1 = op1->is_oper;
	bool is_op2 = op2->is_oper;

	CLiteral * l1 = (CLiteral *)((op1->exp_or_op).oper);
	CLiteral * l2 = (CLiteral *)((op2->exp_or_op).oper); 


	int i1 = atoi((l1->getValue()).c_str());
	int i2 = atoi((l2->getValue()).c_str());

	int result;

	if(oper == "*")
	{
		result = i1 * i2;
	}
	else if(oper == "+")
	{
		result = i1 + i2;
	}
	else if(oper == "-")
	{
		result = i1 - i2;
	}
	else if(oper == "/")
	{
		result = i1 / i2;
	}
	else if(oper == "**")
	{
		result = (int)pow(i1,i2);
	}


	char temp[50];

	sprintf(temp, "%d",result);

	CLiteral * lt = new CLiteral(((op1->exp_or_op).oper)->getType(), temp);
	return lt;

}

CExpression* CParser::createExpression(string oper, struct expr_oper *op1, struct expr_oper *op2, bool is_unary)
{
	bool is_op1 = op1->is_oper;
	bool is_op2;

	if(op2 != NULL)
	{
		is_op2 = op2->is_oper;
	}

	CExpression * expr = new CExpression(	oper, 
						is_op1, op1,
						is_op2, op2, 
						is_unary);
	return expr;


}

/*CExpression* createExpression(string oper, CExpression* op1, CExpression* op2, bool is_unary)
{
	
}*/


struct expr_oper* CParser::convertToExpression(struct expr_oper *op)
{
	CExpression * expr = new CExpression(	"operand", 
						true, op,
						true, NULL, 
						true);
	struct expr_oper *eo = (struct expr_oper*) 
	malloc(sizeof(struct expr_oper));

	eo->is_oper = false;
	(eo->exp_or_op).expr = expr;

	return eo;
}

CSeqStmt * CParser::getLast(CSeqStmt *first)
{

	CSeqStmt *iter = first;

	while(iter->getNext() != NULL)
	{

		iter = iter->getNext();
	}

	return iter;
}

struct expr_oper * CParser::createExpression(char *name)
{
	struct IDinfo *others;
	others = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	others->unit = NULL;
	others->type = t_Identifier;
	others->upper = NULL;//new
	others->lower = NULL;//new

	//cout<<"\n\nstrdup started\n\n";

	others->name = strdup(name);

	//cout<<"\n\nstrdup finished\n\n";

	struct expr_oper *others_eo = (struct expr_oper*) malloc(sizeof(struct expr_oper));

	others_eo->is_oper = true;

	//cout<<"\n\ncreateOperand called\n\n";

	others_eo->exp_or_op.oper = CParser::createOperand(others);

	
	//cout<<"\n\ncreateOperand finished\n\n";

	CExpression* expr =  CParser::createExpression
				(name, others_eo, NULL, true);

	struct expr_oper *eo = (struct expr_oper*) 
	malloc(sizeof(struct expr_oper));

	eo->is_oper = false;
	(eo->exp_or_op).expr = expr;

	return eo;
}

CSeqStmt *CParser::createAndSetJunctionNode(CSeqStmt*cond_stmt)
{
	CSeqStmt *junction_node = new CJunctionNode(cond_stmt);
	list<CSeqStmt *> stmtList = cond_stmt->getBranches();
	list<CSeqStmt *>::iterator iterList;
	for(iterList=stmtList.begin(); iterList !=stmtList.end(); iterList++)
	{
		CSeqStmt *last = getLast((*iterList));
		last->setNext(junction_node);
	}

	return junction_node;
}


int CParser::getEntity(string entity_name)
{
	for(int i=0; i< Entities.size(); i++)
	{
		if(Entities[i].getName() == entity_name)
		{
			return i;
		}
	}
	return -1;
}

void CParser::insertPortToSymbolTable(string entity_name, list<CPort> port_list)
{
	//map<string, list<int> > port_map;

	map<string, SymTabEntry> IDEntries;

	//cout<<"0000"<<endl;

	list<CPort>::iterator iport;
	for (iport=port_list.begin(); 
	iport!=port_list.end(); 
	iport++)
	{
		//cout<<11111111<<(*iport).getName() <<endl;
		SymTabEntry entry;
		string name = (*iport).getName();
		entry.id_type = "port::" + (*iport).getModeStr();
		entry.data_type = (*iport).getType();
		entry.value = (*iport).getValue();

		list<string> uList =  (*iport).getUpper();
		list<string> lList =  (*iport).getLower();

		if(uList.size() == 0 && lList.size() == 0)
		{
			//cout<<"*************inside*********************"<<endl;
			entry.rangeindex.push_back("0");
			entry.rangeindex.push_back("0");
		}
		else
		{
			entry.rangeindex.push_back(*(lList.begin()));
			entry.rangeindex.push_back(*(uList.begin()));
		}

		IDEntries.insert(make_pair(name, entry));
	}

	//cout<<333<<endl;

	SymbolTable.insert(make_pair(entity_name, IDEntries));
	//cout<<444<<endl;
	CPrePostProcessor::printSymbolTable();
	//cout<<555<<endl;
}

CExpression* CParser::createOthers()
{
	struct IDinfo *others;
	others = (struct IDinfo*) malloc(sizeof(struct IDinfo));	

	others->name = strdup("others");
	//others->type = t_OTHERS;

	struct expr_oper *others_eo = (struct expr_oper*) malloc(sizeof(struct expr_oper));

	others_eo->is_oper = true;

	others_eo->exp_or_op.oper = CParser::createOperand
			(others);

		
	CExpression* expr =  CParser::createExpression
				("others", others_eo, NULL, true);

	return expr;	
}












