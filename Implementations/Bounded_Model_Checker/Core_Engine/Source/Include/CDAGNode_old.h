#ifndef CDAGNODE_H_
#define CDAGNODE_H_

#include <list>
#include <string>
#include <iostream>
#include <set>

/*#include "CExpression.h"
#include "ReplaceNode.h"
#include "CDAGNode.h"
#include "CEntity.h"*/

#include "CSensitiveElement.h"
#include "CProfit.h"

#define MIN_DELTA INT_MIN

/* INT_MIN is a symbolic constant known to the compiler which indicates the smallest value of the integer. The minimum value of delta is fixed as INT_MIN. This is the delta time set for CDAGNode type nodes which correspond to operators like "select", "+" etc. Hence care should be taken that delta time of operator nodes should not be operated up on */

using namespace std;

//class CExpression;
//class CEntity;

class CDAGNode
{
public:
	string name;
	int delta_time;
	
	list<string> labels;
	list<CDAGNode*> out_list;
	list<CDAGNode*> in_list;
	bool visited;
	int cost;
	CDAGNode()
	{
		name = "";
		labels.clear();
		out_list.clear();
		in_list.clear();
		visited = false;
		cost = 0;
		delta_time = MIN_DELTA;
	}

	CDAGNode(string n, string l)
	{
		name = n;
		labels.push_back(l);
		out_list.clear();
		in_list.clear();
		visited = false;
		cost = 0;
		delta_time = MIN_DELTA;
	}

       CDAGNode(string n,int dtime)
        {
                name = n;
                labels.clear();
                out_list.clear();
                in_list.clear();
                visited = false;
                cost = 0;
                delta_time = dtime;
        }
	
	string getDAGStr()
	{
		string dag_str = name;


		if(delta_time != MIN_DELTA)
		{
			char temp[10];
			sprintf(temp, "%d", delta_time);

			char *t = strdup(dag_str.c_str());

			dag_str += "_";

			dag_str += temp;

			return dag_str;
		}

		dag_str += "(";

		list<CDAGNode*>::iterator iout = out_list.begin();

		for(; iout != out_list.end(); iout++)
		{
			dag_str += (*iout)->getDAGStr() + ",";
		}

		dag_str += ")";

		return dag_str;
	}


	/*void getDAGNames(set<string> &names)
	{
		string dag_str = name;

		if(delta_time != MIN_DELTA)
		{
			char temp[10];

			sprintf(temp, "%d", delta_time);

			char *t = strdup(dag_str.c_str());

			dag_str += "_";

			dag_str += temp;

			names.insert(dag_str);
		}

		list<CDAGNode*>::iterator iout = out_list.begin();

		for(; iout != out_list.end(); iout++)
		{
			(*iout)->getDAGNames(names);
		}

	}*/

	void getDAGNames(set<string> &names)
	{
		string dag_str = name;

		if(dag_str == "exists")

		  {
		    set<string> ExistsVarSet;
		    set<string> VarSet;
		    
		    list<CDAGNode*>::iterator iout = out_list.begin();

		    (*iout)->getDAGNames(ExistsVarSet);
		    iout++;
		    (*iout)->getDAGNames(VarSet);
		    
		    set_difference(VarSet.begin(), VarSet.end(), ExistsVarSet.begin(), ExistsVarSet.end(),inserter(names, names.begin()));
		  }

		else
		  {
		    if(delta_time != MIN_DELTA)
		      {
			char temp[10];

			sprintf(temp, "%d", delta_time);

			char *t = strdup(dag_str.c_str());

			dag_str += "_";

			dag_str += temp;

			names.insert(dag_str);
		      }

		    list<CDAGNode*>::iterator iout = out_list.begin();

		    for(; iout != out_list.end(); iout++)
		      {
			(*iout)->getDAGNames(names);
		      }
		  }

	}


	void getExistsDAGNames(set<string> &ExistsVarSet) // get the existentially qunatified  variable names (only the name - not sel(name, index, index) from the DAG 
	{
		string dag_str = name;

		if(dag_str == "exists")

		  {
		    set<string> NewVarSet;
		    		    
		    list<CDAGNode*>::iterator iout = out_list.begin();

		    (*iout)->getDAGNames(NewVarSet);

		    // add NewVarSet to ExistsVarSet
		    set_union(NewVarSet.begin(), NewVarSet.end(), ExistsVarSet.begin(), ExistsVarSet.end(),inserter(ExistsVarSet, ExistsVarSet.begin()));
		    iout++;

		    (*iout)->getExistsDAGNames(ExistsVarSet);
		    	    
		  }

		else
		  {
		    list<CDAGNode*>::iterator iout = out_list.begin();

		    for(; iout != out_list.end(); iout++)
		      {
			(*iout)->getExistsDAGNames(ExistsVarSet);
		      }
		  }

	}
	
	CDAGNode* cloneAndShift(CDAGNode* parent, map<CDAGNode*,CDAGNode*> &nodeShiftednode, int shift, set<string> &setRelation)
	{
		if(nodeShiftednode.find(this) != nodeShiftednode.end())
		{
			if(parent != NULL)
			{
				nodeShiftednode[this]->in_list.push_back(parent);
			}
			
			return nodeShiftednode[this];
		}
		
		CDAGNode* clone = new CDAGNode();

		clone->name = this->name;
		clone->delta_time = delta_time == MIN_DELTA?delta_time:delta_time + shift;
		nodeShiftednode.insert(make_pair(this, clone));
		
		if(delta_time != MIN_DELTA)
		{
			char str_current[20];
			
			sprintf(str_current, "%d", delta_time + shift);
			
			string id_delta = this->name + "_" + str_current;
			
			setRelation.insert(id_delta);
		}
		
		list<CDAGNode*>::iterator iout = out_list.begin();
		
		for(; iout != out_list.end(); iout++)
		{
			CDAGNode* child = (*iout)->cloneAndShift(clone, nodeShiftednode, shift, setRelation);
			clone->out_list.push_back(child);
		}
		
		if(parent != NULL)
		{
			clone->in_list.push_back(parent);
		}
	
		return clone;	
	}
	
	bool equals(CDAGNode* dag2)
	{
	  //cout<<"\nDAG1->NAME="<<name<<",DAG2->NAME="<<dag2->name<<endl;
		if (name != dag2->name 
			|| delta_time != dag2->delta_time 
			|| out_list.size() != dag2->out_list.size())
		{
		  //cout<<"\nFALSE\n";
			return false;
		}
		
		list<CDAGNode*>::iterator iout1 = out_list.begin();
		list<CDAGNode*>::iterator iout2 = dag2->out_list.begin();
		
		bool ret_value = true;
		
		for(; iout1 != out_list.end(); iout1++, iout2++ )
		{
			ret_value = ret_value & (*iout1)->equals(*iout2);
		}
		
		return ret_value;			
	}	
	
	bool less(CDAGNode *dag2)
	{
	  	if(name < dag2->name)
	  	{
	  		return true;
	  	}
	  	else if(delta_time < dag2->delta_time )
	  	{
	  		return true;	
	  	}
	  	else if(out_list.size() < dag2->out_list.size())
	  	{
	  		return true;
	  	}
	  	else
	  	{
	 		list<CDAGNode*>::iterator iout1 = out_list.begin();
			list<CDAGNode*>::iterator iout2 = dag2->out_list.begin();
			
			bool ret_value = true;
			
			for(; iout1 != out_list.end(); iout1++, iout2++ )
			{
				if((*iout1)->less(*iout2))
				{
					return true;
				}
			}
	  	}
	  	
	  	return false;
	}		
	
	pair<int, int> getLHSRange()
	{
		if(name != "equivalent")
		{
			cout<<"ERROR1"<<endl;
			return make_pair(-1, -1);
		}
		
		list<CDAGNode*>::iterator iout1 = out_list.begin();

		if((*iout1)->name != "select")
		{
			cout<<"ERROR2"<<endl;
			return make_pair(-1, -1);
		}
		
		list<CDAGNode*>::iterator iout2 = (*iout1)->out_list.begin();
		
		iout2++;
		int lower = atoi((*iout2)->name.c_str());
		
		iout2++;
		int upper = atoi((*iout2)->name.c_str());

		return make_pair(lower, upper);		
	}
	
	void size(int &excnt, int &count, map<string, bool> Graph)
	{
		if(name == "exists")
		{
			excnt++;
		}
		char str_current[20];

		sprintf(str_current, "%d", delta_time);

		string id_delta = name + "_" + str_current;
		
		if(delta_time!= MIN_DELTA && Graph.find(id_delta) != Graph.end())
		{
			return;
		}
	
		count++;
	
		if(delta_time != MIN_DELTA)
		{
			Graph.insert(make_pair(id_delta,true));
		}
	
		list<CDAGNode*>::iterator iout = out_list.begin();

		for(; iout != out_list.end(); iout++)
		{
			(*iout)->size(excnt, count, Graph);
		}
	}

	void findTreeSize(int &count)
	{
		
		count++;
	
		
		list<CDAGNode*>::iterator iout = out_list.begin();

		for(; iout != out_list.end(); iout++)
		{
			(*iout)->findTreeSize(count);
		}
	}

	string getDAGStrInSMTLibFormat()
	{
		string dag_str;


		if(delta_time != MIN_DELTA)
		{
		       dag_str = name;

		         
			char temp[10];

			sprintf(temp, "%d", delta_time);

			dag_str += "_";

			dag_str += temp;

			return dag_str;
		}
		
		else
		  {

		    cout<<"\nNAME=\t"<<name<<endl;

		    if(name == "select")
		      {
			dag_str = "(bv-extract ";

			list<CDAGNode*>::iterator iout = out_list.begin();

			iout++;

			iout++;

			dag_str += (*iout)->getDAGStrInSMTLibFormat() + " ";

			iout = out_list.begin();

			iout++;

			dag_str += (*iout)->getDAGStrInSMTLibFormat() + " ";

			iout = out_list.begin();

			dag_str += (*iout)->getDAGStrInSMTLibFormat();

			dag_str += ")";

			return dag_str;
		      }

		    else if(name == "exists")
		      {
			dag_str = "(exists (";

			set<string> ExistsVarSet;
			set<string>::iterator ExistsVarSet_it;

			getExistsDAGNames(ExistsVarSet); // Get the list of ex.quantified variables

			for(ExistsVarSet_it = ExistsVarSet.begin(); ExistsVarSet_it != ExistsVarSet.end(); ExistsVarSet_it++)
			  dag_str += (*ExistsVarSet_it) + " ";
			
			dag_str += ") ( ";
			
			list<CDAGNode*>::iterator iout = out_list.begin();

			iout++; //iout now points to the formula part
			
			dag_str += (*iout)->getDAGStrInSMTLibFormat() + " )";
			
			dag_str += ")";
			
			return dag_str;
		      }

		    else if(name == "is_not_equal")
		      {
			dag_str = "(not (= ";

			list<CDAGNode*>::iterator iout = out_list.begin();

			for(; iout != out_list.end(); iout++)
			  {
			    dag_str += (*iout)->getDAGStrInSMTLibFormat() + " ";
			  }

			dag_str += "))";

			return dag_str;
		      }
		    else if ((name == "equal") || (name == "concat") || (name == "and") || (name == "or") || (name == "not") || (name == "ite"))
		      {
			if(name == "equal")
			  
			  dag_str = "(= ";

			else if(name == "concat")
			  
			  dag_str = "(bv-concat ";

			else if((name == "and") || (name == "or") || (name == "not") || (name == "ite"))
			  
			    dag_str = "(" +  name + " ";
			
			list<CDAGNode*>::iterator iout = out_list.begin();

			for(; iout != out_list.end(); iout++)
			  {
			    dag_str += (*iout)->getDAGStrInSMTLibFormat() + " ";
			  }

			dag_str += ")";

			return dag_str;
		      
		      }
		    else
		      {
			dag_str = name;
			
			return dag_str;
		      }
		  }
	}


	/*	void calculateDeltaDelays(map<CSensitiveElement*, string> &delays)
	  {
	    	list<CDAGNode*>::iterator iout = out_list.begin();

		if(name == "select")
		{
			
		  string signal_name = (*iout)->name;
		  int signal_delta_time = (*iout)->delta_time;
		  iout++;
		  string signal_lower = (*iout)->name;
		  iout++;
		  string signal_upper = (*iout)->name;
		  
		  map<CSensitiveElement*, string>::iterator delays_it;
		  
		  for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
		    {
		      if( ((delays_it->first)->name == signal_name) && ((delays_it->first)->lower == signal_lower) && ((delays_it->first)->upper == signal_upper) && delays_it->second == "") 
			{
			  char temp[20];
			  sprintf(temp,"%d",signal_delta_time);
			  delays_it->second = temp;
			  cout<<"\n\nIN IF PART\n\n";
			}

		      else if( ((delays_it->first)->name == signal_name) && ((delays_it->first)->lower == signal_lower) && ((delays_it->first)->upper == signal_upper) && (signal_delta_time > atoi((delays_it->second).c_str())) ) 
			{
			  char temp[20];
			  sprintf(temp,"%d",signal_delta_time);
			  delays_it->second = temp;
                          cout<<"\n\nIN ELSE PART\n\n";
			}
		 
		    }
		}

		for(; iout != out_list.end(); iout++)
		{
			(*iout)->calculateDeltaDelays(delays);
		}

		} */

	/* This is the modified calculateDeltaDelays function. The previous calculateDeltaDelays function is given above (commented). The difference is this. 

	Let we have an input x(0:10). It may be appearing in the symbolic expression as x(0:4) and x(5:10). Hence what this modified function does is that, it takes each variable from the symbolic expression, checks if there is an input with input's name = variable's name and input's range = variable's range. If yes, the variable's delta value is read. If a match is not found for the variable, it checks if there is an input with input's name = variable's name. If yes, the variable's delta value is read. Else the variable's delta value is not read */

	void calculateDeltaDelays(map<CSensitiveElement*, string> &delays)
	  {
	    	list<CDAGNode*>::iterator iout = out_list.begin();

		if(name == "select")
		{
			
		  string signal_name = (*iout)->name;
		  int signal_delta_time = (*iout)->delta_time;
		  iout++;
		  string signal_lower = (*iout)->name;
		  iout++;
		  string signal_upper = (*iout)->name;
		  
		  map<CSensitiveElement*, string>::iterator delays_it;

		  bool match_found = false;
		  
		  for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
		    {
		      if( ((delays_it->first)->name == signal_name) && ((delays_it->first)->lower == signal_lower) && ((delays_it->first)->upper == signal_upper))
			{
			  if(delays_it->second == "")

			    {
			      char temp[20];
			      sprintf(temp,"%d",signal_delta_time);
			      delays_it->second = temp;
			      //cout<<"\n\nIN IF PART\n\n";
			    }

			  else if( signal_delta_time > atoi((delays_it->second).c_str()) )
			    {
			      char temp[20];
			      sprintf(temp,"%d",signal_delta_time);
			      delays_it->second = temp;
			      //cout<<"\n\nIN ELSE PART\n\n";
			    }
			  match_found = true;
			  break;
			}
		    }

		  if(!match_found)
		    {
		       for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
			 {
			   if((delays_it->first)->name == signal_name)
			     {
			       if(delays_it->second == "")

				 {
				   char temp[20];
				   sprintf(temp,"%d",signal_delta_time);
				   delays_it->second = temp;
				   //cout<<"\n\nIN IF PART\n\n";
				 }

			       else if( signal_delta_time > atoi((delays_it->second).c_str()) )
				 {
				   char temp[20];
				   sprintf(temp,"%d",signal_delta_time);
				   delays_it->second = temp;
				   //cout<<"\n\nIN ELSE PART\n\n";
				 }
			       match_found = true;
			       break;
			     }
			 }
		    }
			  
		 
		}
		

		for(; iout != out_list.end(); iout++)
		{
			(*iout)->calculateDeltaDelays(delays);
		}

	  }

	/* Function to replace each occurance of name_delta(lower:upper) by the DAG rhs */

	

	void replaceSubDag(string name, int delta, string lower, string upper, CDAGNode* rhs) 
	{
	list<CDAGNode*>::iterator irootout = out_list.begin();
	for(; irootout != out_list.end(); irootout++)
	   {
	   CDAGNode* currentNode = (*irootout);
	   if(currentNode->name == "select")
	        {
		list<CDAGNode*>::iterator iselectout = (currentNode->out_list).begin();
		string signal_name = (*iselectout)->name;
		int signal_delta_time = (*iselectout)->delta_time;
		iselectout++;
		string signal_lower = (*iselectout)->name;
		iselectout++;
		string signal_upper = (*iselectout)->name;

		if(name == signal_name && delta == signal_delta_time && lower == signal_lower && upper == signal_upper)
		    {
		      // cout<<"\n\nMATCH FOUND!!";
		     irootout = out_list.erase(irootout);

		     // creating clone of rhs to assign to out_list at irrotout
		     map<CDAGNode*,CDAGNode*> temp1;
		     set<string> temp2;
		     CDAGNode* rhs_copy = rhs->cloneAndShift(NULL, temp1, 0, temp2);
		     // clone created 
		
		     irootout = out_list.insert(irootout,rhs_copy);
		     }
		else
		  {
		   currentNode->replaceSubDag(name, delta, lower, upper, rhs);
		  }
 
		
		}
	   else
	     {
	       currentNode->replaceSubDag(name, delta, lower, upper, rhs);
	     }

	   }
	
	}
	


	string getDAGStrInExprSimpFormat()
	{
		string dag_str;


		if(delta_time != MIN_DELTA)
		{
		       dag_str = name;

		         
			char temp[10];

			sprintf(temp, "%d", delta_time);

			dag_str += "_";

			dag_str += temp;

			return dag_str;
		}
		
		else
		  {

		    //cout<<"\nNAME=\t"<<name<<endl;

		    if(name == "exists")
		      {
			// TO DO
			list<CDAGNode*>::iterator it_wr = out_list.begin();
			dag_str = "Exists(List(";
			dag_str += (*it_wr)->getDAGStrInExprSimpFormat();
			dag_str += "),";
			it_wr++;
			dag_str += (*it_wr)->getDAGStrInExprSimpFormat();
			dag_str += ")";
			return dag_str;
			
		      } 
		    else if(name == "comma") // "comma" is the operator separating individual variables in the variable's list in exists
		      {
			list<CDAGNode*>::iterator it_wr = out_list.begin();

			dag_str = "";
			dag_str += (*it_wr)->getDAGStrInExprSimpFormat();
			dag_str += ",";
			it_wr++;
			dag_str += (*it_wr)->getDAGStrInExprSimpFormat();
			return dag_str;
		      }

		    else
		      {
			if(name == "select")
			  {
			    dag_str = "Sel";
			  }

			else if(name == "concat")
			  {
			    dag_str =  "Concat";
			  }
			else if(name == "not")
			  {
			    dag_str =  "LOG_NOT";
			  }
			else if(name == "and")
			  {
			    dag_str =  "Log_And";
			  }
			else if(name == "or")
			  {
			    dag_str =  "Log_Or";
			  } 
			else if(name == "ite")
			  {
			    dag_str =  "Ternary";
			  } 
			else if(name == "equivalent")
			  {
			    dag_str =  "Log_Equal";
			  } 
			else if(name == "is_not_equal")
			  {
			    dag_str =  "Log_Inequal";
			  } 
			else
			  {
			    dag_str = name;
			  }

		    	list<CDAGNode*>::iterator it_wr = out_list.begin();
			list<CDAGNode*>::iterator it_ch = out_list.begin();
			it_ch++;

			dag_str += "(";
			while(it_wr != out_list.end())
			  {
			    if(it_ch != out_list.end())
			      {
			       dag_str += (*it_wr)->getDAGStrInExprSimpFormat() + ",";
			       it_ch++;
			      }
			    else
			      {
				dag_str += (*it_wr)->getDAGStrInExprSimpFormat();
			      }
			    it_wr++;
			  }

			dag_str += ")";

			return dag_str;
		      }
		  }
	}


	void findExistsDAGs(list<CDAGNode*> &ExistsList)
	  {
 
	    string dag_str = name;

	    if(dag_str == "exists")

		  {
		    cout<<"\n"<<dag_str<<"\n";
		    ExistsList.push_back(this);
		  }

	    
	    list<CDAGNode*>::iterator iout = out_list.begin();

	    for(; iout != out_list.end(); iout++)
	      {
		(*iout)->findExistsDAGs(ExistsList);
	      }
	  

	  }

	void findExistsDAGsWithDepths(list<ExistsNodesWithProfits*> &ExistsList, int depth)
	  {
 
	    string dag_str = name;

	    if(dag_str == "exists")

		  {
		    //cout<<"\n"<<dag_str<<"\n";
		    ExistsNodesWithProfits* NewNode = new ExistsNodesWithProfits(this, depth);
		    ExistsList.push_back(NewNode);
		  }

	    
	    list<CDAGNode*>::iterator iout = out_list.begin();

	    for(; iout != out_list.end(); iout++)
	      {
		(*iout)->findExistsDAGsWithDepths(ExistsList, depth+1);
	      }
	  

	  }


	void replaceTree(CDAGNode* lhs, CDAGNode* rhs) // Replace occurance of lhs by rhs. There is only one occurance of lhs in the tree 
	{
	list<CDAGNode*>::iterator irootout = out_list.begin();
	for(; irootout != out_list.end(); irootout++)
	   {
	     if( (*irootout) == lhs)
	       {
		 cout<<"\n\nMATCH FOUND!!";
		 irootout = out_list.erase(irootout);
		 irootout = out_list.insert(irootout,rhs);
		 return;
	     }
	    
	   else
	     {
	       (*irootout)->replaceTree(lhs, rhs);
	     }

	   }
	}

	
	
};

#endif /*CDAGNODE_H_*/
