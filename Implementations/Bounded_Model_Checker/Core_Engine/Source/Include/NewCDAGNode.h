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



using namespace std;

//class CExpression;
//class CEntity;

class CDAGNode
{
public:
	string name;
	int delta_time;
	bool delta_variable; // Denotes a node with delta time set  
	
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
		delta_variable = false;
	}

	CDAGNode(string n, string l)
	{
		name = n;
		labels.push_back(l);
		out_list.clear();
		in_list.clear();
		visited = false;
		cost = 0;
		delta_variable = false;
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
		delta_variable = true;
        }
	
	string getDAGStr()
	{
		string dag_str = name;


		if(delta_variable == true)
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


	void getDAGNames(set<string> &names)
	{
		string dag_str = name;


		if(delta_variable == true)
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
		clone->delta_time = delta_variable == false?delta_time:delta_time + shift;
		nodeShiftednode.insert(make_pair(this, clone));
		
		if(delta_variable == true)
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
		if (name != dag2->name 
			|| delta_time != dag2->delta_time 
			|| out_list.size() != dag2->out_list.size())
		{
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
		
		if(delta_variable == true && Graph.find(id_delta) != Graph.end())
		{
			return;
		}
	
		count++;
	
		if(delta_variable == true)
		{
			Graph.insert(make_pair(id_delta,true));
		}
	
		list<CDAGNode*>::iterator iout = out_list.begin();

		for(; iout != out_list.end(); iout++)
		{
			(*iout)->size(excnt, count, Graph);
		}
	}

	string getDAGStrInSMTLibFormat()
	{
		string dag_str;


		if(delta_variable == true)
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



	
	
};

#endif /*CDAGNODE_H_*/
