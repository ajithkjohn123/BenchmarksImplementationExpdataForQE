#include "CAtom.h"

extern CHelper MANAGER;

CAtom::CAtom()
{
}

CAtom::~CAtom()
{
}

CAtom::CAtom(int type, string name, string lower, string upper, string unit):COperand(type,unit)
{
	this->name = name;
	this->lower = lower;
	this->upper = upper;
	//cout<<"\n"<<name<<" created \n"; // newly added
}



void CAtom::print()
{
	//cout<<"OP::     "<<name<<endl;
	//cout<<"Lower: "<<lower<<"\t"<<"Upper: "<<upper<<endl;
}

string CAtom::getOper()
{
	string oper;
	oper = name + "(" + lower + ":" + upper +")"+unit;
	return oper;
}

string CAtom::getName()
{
	return name;
}

bool CAtom::unAdjusted()
{
	if(lower == "-1" && upper == "-1" )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CAtom::isEnumValue(map<string, SymTabEntry> &ids, string name)
{
	map<string, SymTabEntry>::iterator iter = ids.begin();
	for(iter = ids.begin();iter != ids.end();iter++)
	{
		if((*iter).second.id_type == "enum" )
		{
			list<string>::iterator iri;
			for(iri = (*iter).second.rangeindex.begin();iri != (*iter).second.rangeindex.end();iri++)
			{
				if(name == *iri)
				{
					return true;
				}
			}
		}
	}
}

int CAtom::adjustRange(string entityName)
{
	int noofbits=0;

	//cout<<"atom name = "<<name<<endl;
	if(unAdjusted())
	{
		map<string, SymTabEntry> &ids = SymbolTable[entityName];
		if(ids.find(name) == ids.end())
		{
			if(isEnumValue(ids, name))
			{
				lower = "-2";
				upper = "-2";	
			}
			//cout<<"dddddddddddddddd"<<endl;
			return -1;
		}

		SymTabEntry ste = ids[name];
		
		list<string>::iterator ilist = ste.rangeindex.begin();

		lower = (*ilist);
		ilist++;
		upper = (*ilist);		

		int u = atoi(upper.c_str());
		int l = atoi(lower.c_str());
		noofbits = u - l;
		
	}
	else
	{
		int u = atoi(upper.c_str());
		int l = atoi(lower.c_str());
		noofbits = u - l;
		
	}
	return noofbits+1;
}

void CAtom::setLowerBits(int numberofbits)
{
	char t_upper[20];
	sprintf(t_upper, "%d", numberofbits-1);
	lower = "0";
	upper = t_upper;
} 


string CAtom::getUpper( )
{
	return upper;
}

string CAtom::getLower( )
{
	return lower;	
}

CDAGNode* CAtom::getDAG(string entName, int delta, set<string>& setRelation)
{
	CDAGNode * final_expression = NULL;
	int low = atoi(lower.c_str());
	
	int up = atoi(upper.c_str()); 
	
	char *temp1 = strdup(name.c_str());
	
	CDAGNode * dag1 = NULL;
	
	char curr_delta[5];
	
	
	if(SymbolTable[entName][this->name].id_type == "variable")
	{
		dag1= MANAGER.createDAG(this->name, delta);
		sprintf(curr_delta, "%d", delta);
	}
	else
	{
		dag1= MANAGER.createDAG(this->name, delta);
		sprintf(curr_delta, "%d", delta);
	}
	
	setRelation.insert(name+"_"+curr_delta);
	
	char temp[20];
	sprintf(temp, "%d", low);
	CDAGNode *st_dag = MANAGER.createDAG(temp);

	sprintf(temp, "%d", up);
	CDAGNode *en_dag = MANAGER.createDAG(temp);
	
	final_expression = MANAGER.createDAG("select", dag1, st_dag, en_dag);
	
	return final_expression;
}


list<ProcessSExpressions*>::iterator CAtom::IsPresent (list<ProcessSExpressions*> & SymbolicExpressions, CAtom* toBeSearched)
{
	list<ProcessSExpressions*>::iterator  ios;
	for(ios = SymbolicExpressions.begin();ios != SymbolicExpressions.end();ios++)
	{
		if(toBeSearched->getName() == (*ios)->getLhs()->getName() && 
			((CAtom*)toBeSearched)->getLower() == ((CAtom*)((*ios)->getLhs()))->getLower() &&
			((CAtom*)toBeSearched)->getUpper() == ((CAtom*)((*ios)->getLhs()))->getUpper())
		{
			return ios;
		}	
	}
	
	return SymbolicExpressions.end();
}


list<VariableRangeConstraint*> sortVariableRangeConstraint(list<VariableRangeConstraint*> original_entries)
{
  list<VariableRangeConstraint*>::iterator original_entries_it1 =  original_entries.begin();
  list<VariableRangeConstraint*>::iterator original_entries_it2 =  original_entries.begin();
  list<VariableRangeConstraint*> sorted_list;

  while(original_entries.begin() != original_entries.end())
    {
      int low_first = (*original_entries_it1)->range.first;

      for(original_entries_it2 =  original_entries.begin();original_entries_it2!=original_entries.end();original_entries_it2++)
	{
	  int low_second = (*original_entries_it2)->range.first;
	  if(low_second > low_first)
	    original_entries_it1 = original_entries_it2;
	}
      sorted_list.push_back(new VariableRangeConstraint((*original_entries_it1)->range.first, (*original_entries_it1)->range.second,(*original_entries_it1)->constraint));
      original_entries_it1 = original_entries.erase(original_entries_it1);
      original_entries_it1 =  original_entries.begin();
    }
  return sorted_list;
}
			 
  

CDAGNode* CAtom::getSymbolicExpressionsForVariable(CAtom *atomName,list<ProcessSExpressions*> & SymbolicExpressions, string entName, int delta)
{
  list<ProcessSExpressions*>::iterator it = SymbolicExpressions.begin();
  list<ProcessSExpressions*> VarSymbolicExpressions;
  map<string, list<VariableRangeConstraint*> > mapRV;

  string varName = atomName->name;

  map<string, SymTabEntry> &ids = SymbolTable[entName];

  SymTabEntry ste = ids[varName];
		
  list<string>::iterator ilist = ste.rangeindex.begin();

  string lower = (*ilist);
  ilist++;
  string upper = (*ilist);		

  int u = atoi(upper.c_str());
  int l = atoi(lower.c_str());

  //cout<<"\nU="<<u<<"\tL="<<l<<endl;
  
  set<string> setRelation; 

  VariableRangeConstraint *rc = new VariableRangeConstraint(l, u, MANAGER.createDAG(varName,delta));
  mapRV[varName].push_back(rc); // Initializing the map 

  for(;it != SymbolicExpressions.end();it++)
	{
	  // Each pass  will find an element in the map

		if(varName == (*it)->getLhs()->getName())
		{
		  // Each pass  will find an element in the map whose name = varName

		  int low_new = atoi(((CAtom*)((*it)->getLhs()))->getLower().c_str());
		  int upp_new = atoi(((CAtom*)((*it)->getLhs()))->getUpper().c_str());
	
		  VariableRangeConstraint *rc = new VariableRangeConstraint(low_new, upp_new, (*it)->getValue());
	
		  if(mapRV.find(varName) != mapRV.end())
		    {
		      list<VariableRangeConstraint*>& listRV = mapRV[varName];
		      list<VariableRangeConstraint*>::iterator irv;
		      
		      irv = listRV.begin();

		      while(irv != listRV.end())
			{
			int low_old = (*irv)->range.first;
			int upp_old = (*irv)->range.second;

			//cout<<"\nLN="<<low_new<<"UN="<<upp_new<<"LO="<<low_old<<"UO="<<upp_old;

			if(low_new <= low_old && upp_old <= upp_new)

			  {
			    //cout<<"\nReached in low_new <= low_old && upp_old <= upp_new\n";
			  
				irv = listRV.erase(irv);
				// same or total spanning
			  }
			else if(low_new == low_old && upp_old >= upp_new)
			{
				// old    5            15
				// new    5       12
			  
			      //cout<<"\nReached in low_new == low_old && upp_old >= upp_new\n";

				pair<int, int> upper_range = make_pair(upp_new+1, upp_old);

				CDAGNode *dag_upp =	MANAGER.createDAG("select",
													(*irv)->constraint,
													upp_new+1,
													upp_old);

				//cout<<"\ndag_upp=\t"<<dag_upp->getDAGStr()<<endl;

				VariableRangeConstraint *rc_upp = new VariableRangeConstraint(upp_new+1, upp_old, dag_upp);
				listRV.push_back(rc_upp);

				irv = listRV.erase(irv);
				// inside
			}
			else if(low_new >= low_old && upp_old == upp_new)
			{
				// old    5            15
				// new          8      15

			       //cout<<"\nReached in low_new >= low_old && upp_old == upp_new\n";

				pair<int, int> lower_range = make_pair(low_old, low_new-1);
				CDAGNode *dag_low =	MANAGER.createDAG("select",
													(*irv)->constraint,
													low_old,
													low_new-1);

				VariableRangeConstraint *rc_low = new VariableRangeConstraint(low_old, low_new-1, dag_low);

				listRV.push_back(rc_low);

				irv = listRV.erase(irv);

				// inside
			}
			else if(low_new > low_old && upp_old > upp_new)
			{
				// old    5            15
				// new        8    12

			       //cout<<"\nReached in low_new > low_old && upp_old > upp_new\n";

			       
				pair<int, int> lower_range = make_pair(low_old, low_new-1);
				pair<int, int> upper_range = make_pair(upp_new+1, upp_old);

				CDAGNode *dag_low =	MANAGER.createDAG("select",
													(*irv)->constraint,
													low_old,
													low_new-1);

				VariableRangeConstraint *rc_low = new VariableRangeConstraint(low_old, low_new-1, dag_low);


				CDAGNode *dag_upp =	MANAGER.createDAG("select",
													(*irv)->constraint,
													upp_new+1,
													upp_old);

				VariableRangeConstraint *rc_upp = new VariableRangeConstraint(upp_new+1, upp_old, dag_upp);

				listRV.push_back(rc_upp);
				listRV.push_back(rc_low);

				irv = listRV.erase(irv);
				// inside
			}
			else if(low_new >= low_old && low_new <= upp_old)
			{

			  //cout<<"\nReached in low_new >= low_old && low_new <= upp_old\n";

				CDAGNode *dag_low =	MANAGER.createDAG("select",
													(*irv)->constraint,
													low_old,
													low_new-1);

				VariableRangeConstraint *rc_low = new VariableRangeConstraint(low_old, low_new-1, dag_low);

				listRV.push_back(rc_low);

				irv = listRV.erase(irv);
				// lower is inside
			}
			else if(upp_new >= low_old && upp_new <= upp_old)
			{

			  //cout<<"\nReached in upp_new >= low_old && upp_new <= upp_old\n";

				CDAGNode *dag_upp =	MANAGER.createDAG("select",
													(*irv)->constraint,
													upp_new+1,
													upp_old);

				//cout<<"\nDAG for dag_upp is \t"<<dag_upp->getDAGStr()<<endl;

				VariableRangeConstraint *rc_upp = new VariableRangeConstraint(upp_new+1, upp_old, dag_upp);

				listRV.push_back(rc_upp);

				irv = listRV.erase(irv);
				// upper is inside
			}
			else
			  {
			    ++irv;
			  }
		}

		//TODO
//		CDAGNode *dag = MANAGER.createDAG(os->getLhs()->getName());

		listRV.push_back(rc);
		    }
		  else
		    {
		      //string name = os->getLhs()->getName();
		      mapRV[varName].push_back(rc);
		    }
		  
		  

			
		}	
	}

  // At this point mapRV contains the SEs for different parts of the variable in map form. 
  // We just need to form a DAG out of it.

     CDAGNode *dag;

      map<string, list<VariableRangeConstraint*> >::iterator itab = mapRV.begin();

      for(;itab != mapRV.end(); itab++)
	{
		
	        string output_identifier = itab->first;
		list<VariableRangeConstraint*> original_entries = itab->second;

		// Temp code
		list<VariableRangeConstraint*>::iterator temp =  original_entries.begin();
		
		//cout<<"\nPrinting bv-ranges of "<<output_identifier;

		for(;temp!=original_entries.end();temp++) 
			{
			//cout<<endl<<(*temp)->range.first<<","<<(*temp)->range.second;
			}
		// ends here

		list<VariableRangeConstraint*>::iterator original_entries_it1 =  original_entries.begin();
		list<VariableRangeConstraint*>::iterator original_entries_it2 =  original_entries.begin();
		list<VariableRangeConstraint*> entries;

	
		while(original_entries.begin() != original_entries.end())
		  {
		    int low_first = (*original_entries_it1)->range.first;

		    for(original_entries_it2 =  original_entries.begin();original_entries_it2!=original_entries.end();original_entries_it2++)
		      {
			int low_second = (*original_entries_it2)->range.first;
			if(low_second > low_first)
			  original_entries_it1 = original_entries_it2;
		      }
		    entries.push_back(new VariableRangeConstraint((*original_entries_it1)->range.first, (*original_entries_it1)->range.second,(*original_entries_it1)->constraint));
		    original_entries_it1 = original_entries.erase(original_entries_it1);
		    original_entries_it1 =  original_entries.begin();
		  }

		// Temp code
		temp =  entries.begin();
		
		//cout<<"\nPrinting bv-ranges of Sorted"<<output_identifier;

		for(;temp!=entries.end();temp++) 
		{
			//cout<<endl<<(*temp)->range.first<<","<<(*temp)->range.second;
		}
		// ends here

		

		list<VariableRangeConstraint*>::iterator irio = entries.begin();
		dag = MANAGER.createDAG("select",(*irio)->constraint,(*irio)->range.first, (*irio)->range.second);
		
			
		// create lhs_dag here.......

		//lhs_dag = MANAGER.createDAG(output_identifier, (*irio)->range.first, (*irio)->range.second, 2);
		//dag = MANAGER.createDAG("equivalent", lhs_dag, (*irio)->constraint);

		irio++;
		
		for(; irio != entries.end(); irio++)
		{
		       // create lhs_dag here also.....
		       //lhs_dag = MANAGER.createDAG(output_identifier, (*irio)->range.first, (*irio)->range.second, 2);
		        dag = MANAGER.createDAG("concat", dag, MANAGER.createDAG("select",(*irio)->constraint,(*irio)->range.first, (*irio)->range.second));
			
		}
	
	}

      return dag;
	

  
}

CDAGNode* CAtom::getSymbolicExpressions(string entName, int delta, set<string>& setRelation, list<ProcessSExpressions*> & SymbolicExpressions)
{
	CDAGNode * final_expression = NULL;
	int low = atoi(lower.c_str());
	
	int up = atoi(upper.c_str()); 
	
	char *temp1 = strdup(name.c_str());
	
	CDAGNode * dag1 = NULL;
	
	char curr_delta[5];
	
	
	if(SymbolTable[entName][this->name].id_type == "variable")
	{
	        //cout<<"\nWe are here......\n";
		/*	list<ProcessSExpressions*>::iterator it = IsPresent(SymbolicExpressions,this);
		cout<<"\nWe are Before if......\n"; 
		if(it != SymbolicExpressions.end())
		  {
		    cout<<"\nWe are Inside if......\n"; 
		    if((*it)->getValue()==NULL)
		      cout<<"NULL";
		    else
		      cout<<"Value=\t"<<(*it)->getValue()->getDAGStr()<<endl;
		    dag1 = (*it)->getValue();
		  }
		else
		  {
		    cout<<"\nError in CAtom::getSymbolicExpressions. Exiting";
		    exit(1);
		    }*/
			
		dag1 = getSymbolicExpressionsForVariable(this,SymbolicExpressions,entName,delta);

		// dag1 is the dag for the entire variable 

	        sprintf(curr_delta, "%d", delta+1);
	}
	else
	{
		dag1= MANAGER.createDAG(this->name, delta);
		sprintf(curr_delta, "%d", delta);
	}
	
	setRelation.insert(name+"_"+curr_delta);
	
	char temp[20];
	sprintf(temp, "%d", low);
	CDAGNode *st_dag = MANAGER.createDAG(temp);

	sprintf(temp, "%d", up);
	CDAGNode *en_dag = MANAGER.createDAG(temp);
	
	final_expression = MANAGER.createDAG("select", dag1, st_dag, en_dag);
	
	return final_expression;
}


