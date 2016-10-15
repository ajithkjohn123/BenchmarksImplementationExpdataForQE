#ifndef __CEXPSIMPSTRUCTURE_H
#define __CEXPSIMPSTRUCTURE_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <set>

using namespace std;

class CExprSimpNode
{
public:
	string name;
	list<string> labels;
	list<struct CExprSimpNode*> out_list;
	list<struct CExprSimpNode*> in_list;
	bool visited;
	int cost;
	CExprSimpNode()
	{
		name = "";
		labels.clear();
		out_list.clear();
		in_list.clear();
		visited = false;
		cost = 0;
	}

	CExprSimpNode(string n, string l)
	{
		name = n;
		labels.push_back(l);
		out_list.clear();
		in_list.clear();
		visited = false;
		cost = 0;
	}
	
	void getnodeVars(string &arg_str) 
	{
	  //cout<<"\n"<<name<<"called\n";
	  if(out_list.begin() == out_list.end()) // consider only the terminal nodes
	    {
	      arg_str += name;
	      arg_str += ",";
	    }
	  else
	    {
	      list<struct CExprSimpNode*>::iterator iout = out_list.begin();

	      for(; iout != out_list.end(); iout++)
		      {
			(*iout)->getnodeVars(arg_str);
		      }
	    }
	}

	string getCExprSimpNodeStr()
	{
		string dag_str = name;

		if(out_list.begin() == out_list.end()) // consider only the terminal nodes
		  {
		    return dag_str;
		  }

		dag_str += "(";

		list<struct CExprSimpNode*>::iterator iout = out_list.begin();

		for(; iout != out_list.end(); iout++)
		{
			dag_str += (*iout)->getCExprSimpNodeStr() + ",";
		}

		dag_str += ")";

		return dag_str;
	}

	void computeDAGSize(set <struct CExprSimpNode*> &addresses, int &size)
	  {
	    
	    if(!addresses.empty())
	      {
		//cout<<"\nNon-empty\n";
		
		if(addresses.find(this) != addresses.end())
		  {
		    //cout<<"\nAlready traversed\n";
		    return;
		  }
		else
		  {
		    //cout<<"\nNew node\n";

		    //cout<<"\nNode is "<<this<<", which contains "<<this->getCExprSimpNodeStr()<<endl;

		    size++; 

		    addresses.insert(this);
		    
		    list<struct CExprSimpNode*>::iterator iout = out_list.begin();

		    for(; iout != out_list.end(); iout++)
		      {
			(*iout)->computeDAGSize(addresses, size);
		      }
		  }
	      }
	    
	    else
		 {
		   //cout<<"\nNew node\n";

		   //cout<<"\nNode is "<<this<<", which contains "<<this->getCExprSimpNodeStr()<<endl;
		   
		    size++; 
		    
		    addresses.insert(this);
		    
		    list<struct CExprSimpNode*>::iterator iout = out_list.begin();

		    for(; iout != out_list.end(); iout++)
		      {
			(*iout)->computeDAGSize(addresses, size);
		      }
		  }
	    
	  }


	void computeTreeSize(int &size)
	  {
	    size++; 
	    
	    list<struct CExprSimpNode*>::iterator iout = out_list.begin();

	    for(; iout != out_list.end(); iout++)
	      {
		(*iout)->computeTreeSize(size);
	      }
		 
	}

	void getDepthsOfExistsNodes(list<int> &depths, int depth)
	  {
	    if(name == "Exists") // Exists node encountered
		      {
			cout<<"\ndepth = "<<depth<<"\n";
			depths.push_back(depth);
		      }
	    
	    list<struct CExprSimpNode*>::iterator iout = out_list.begin();
	    
	    for(; iout != out_list.end(); iout++)
	        {
		  (*iout)->getDepthsOfExistsNodes(depths,depth+1);
			
		}
   
	  }


	void getDepthMap(map<string, vector<int> > &DepthMap, int depth)
	  {
	    if(name == "exprList") // get the list of ex.quantified variables and insert into ExQuantifySet
		      {
			list<CExprSimpNode*>::iterator it;
			
			for(it = out_list.begin();it != out_list.end(); it++)
			  {
			    // Check if the present depth is less than the new depth. If yes depth should be the new depth
			    if(DepthMap.find((*it)->name) != DepthMap.end())
			      {
				DepthMap[(*it)->name].push_back(depth);
				 
			      }
			    else
			      {
				vector<int> NewDepthMapEntry;
				NewDepthMapEntry.push_back(depth);	
				DepthMap.insert(make_pair((*it)->name,NewDepthMapEntry));
			      }
			  }
		      }


	    
	    list<struct CExprSimpNode*>::iterator iout = out_list.begin();
	    
	    for(; iout != out_list.end(); iout++)
	        {
		  (*iout)->getDepthMap(DepthMap,depth+1);
			
		}

	    
	  }


	void getDepthScopeMap(map<string, list< pair<int, int> > > &DepthScopeMap, int depth, CExprSimpNode* parent)
	  {
	    if(name == "exprList") 
		      {
			// get the size(scope)

			int size;

			// parent shuld be "exists" node. 

			if(parent->name == "Exists")
			  {
			    list<CExprSimpNode*>::iterator it = (parent->out_list).begin();
			    it++; 
			    // it now points to SE scoped under exists
			    set <struct CExprSimpNode*> addresses;
			    size = 0;
			    (*it)->computeDAGSize(addresses,size);
			    // size now contains size of the SE
			  }
			else
			  {
			    cout<<"\nERROR in the function getDepthScopeMap!!!Pointer to exprList in the DAG from a node other than Exists node. Exiting...";
			    exit(1);
			  }
			// size(scope) computed
			    

			list<CExprSimpNode*>::iterator it;
			
			for(it = out_list.begin();it != out_list.end(); it++)
			  {
			    // Check if the variable is already there in the DepthScopeMap
			    if(DepthScopeMap.find((*it)->name) != DepthScopeMap.end())
			      {
				DepthScopeMap[(*it)->name].push_back(make_pair(depth,size));
				 
			      }
			    else
			      {
				list< pair<int, int> > NewDepthScopeMapEntry;
				NewDepthScopeMapEntry.push_back(make_pair(depth,size));	
				DepthScopeMap.insert(make_pair((*it)->name,NewDepthScopeMapEntry));
			      }
			  }
		      }


	    
	    list<struct CExprSimpNode*>::iterator iout = out_list.begin();
	    
	    for(; iout != out_list.end(); iout++)
	        {
		  (*iout)->getDepthScopeMap(DepthScopeMap,depth+1, this);
			
		}

	    
	  }


	// Find the total no: of nodes under the scope of an ex.quantified variable .
	//  scope = dag_scope (size of dag underneath)
	void getTotalScopeofVariable(string variable, int &dag_scope, set <struct CExprSimpNode*> &addresses, CExprSimpNode* parent)
	  {
	    list<CExprSimpNode*>::iterator it;

	    if(name == "exprList") 
		      {
			//cout<<"\nScopes of "<<variable<<" are "<<dag_scope<<","<<tree_scope<<endl;
				
			if(parent->name != "Exists")
			  {
			    cout<<"\nERROR in the function getTotalScopeMap!!!Pointer to exprList in the DAG from a node other than Exists node. Exiting...";
			    exit(1);
			  }

			// Check if the variable is there in the exprList
			bool VarPresent = false;
			for(it = out_list.begin();it != out_list.end(); it++)
			  {
			    if(((*it)->name) == variable)
			      {
				VarPresent = true;
				break;
			      }
			  }
			
			if(VarPresent == true)
			  {
			    // set the dag_scope, tree_scope
			    it = (parent->out_list).begin();
			    it++; 
			    
			    // it now points to SE scoped under exists
			    (*it)->computeDAGSize(addresses,dag_scope);
			    //(*it)->computeTreeSize(tree_scope);
			    // scopes found
			  }
			//cout<<"\nScopes of "<<variable<<" are "<<dag_scope<<","<<tree_scope<<endl;
		      }
			    
	    list<struct CExprSimpNode*>::iterator iout = out_list.begin();
	    
	    for(; iout != out_list.end(); iout++)
	        {
		  (*iout)->getTotalScopeofVariable(variable, dag_scope, addresses, this);
			
		}
   
	  }

	// Get the ex.quantified variables
	void getExVarNames(set<string> &ExVarNames)
	  {
	    if(name == "exprList") // get the list of ex.quantified variables and insert into ExVarNames
		      {
			list<CExprSimpNode*>::iterator it;
			
			for(it = out_list.begin();it != out_list.end(); it++)
			  {
			    // Check if the variable is present in the ExVarNames. If no, add into ExVarNames
			    if(ExVarNames.find((*it)->name) == ExVarNames.end())
			      {
				ExVarNames.insert((*it)->name);
			      }
			  }
		      }
	    
	    list<struct CExprSimpNode*>::iterator iout = out_list.begin();
	    
	    for(; iout != out_list.end(); iout++)
	        {
		  (*iout)->getExVarNames(ExVarNames);
			
		}
	    
	  }



	// Find the total no: of nodes under the scope of each ex.quantified variable
	void getTotalScopeMap(map<string, int> &TotalScopeMap)
	  {
	    // Get the existentially quatified variables
	    set<string> ExVarNames;
	    set<string>::iterator it;
	    
	    getExVarNames(ExVarNames);

	    for(it = ExVarNames.begin();it != ExVarNames.end(); it++)
	      {
		set <struct CExprSimpNode*> addresses;
		int dag_scope = 0; //tree_scope = 0;
		
		getTotalScopeofVariable(*it, dag_scope, addresses, NULL);	 
		
		TotalScopeMap.insert(make_pair((*it),dag_scope));
	      }
	  }

	void getDAGAddresses(set <struct CExprSimpNode*>  &addresses)
	  {
	    
	    if(!addresses.empty())
	      {
		//cout<<"\nNon-empty\n";
		
		if(addresses.find(this) != addresses.end())
		  {
		    //cout<<"\nAlready traversed\n";
		    return;
		  }
		else
		  {
		    addresses.insert(this);
		    
		    list<struct CExprSimpNode*>::iterator iout = out_list.begin();

		    for(; iout != out_list.end(); iout++)
		      {
			(*iout)->getDAGAddresses(addresses);
		      }
		  }
	      }
	    
	    else
		 {
		   
		    addresses.insert(this);
		    
		    list<struct CExprSimpNode*>::iterator iout = out_list.begin();

		    for(; iout != out_list.end(); iout++)
		      {
			(*iout)->getDAGAddresses(addresses);
		      }
		  }
	    
	  }

	/* Function same as getDAGAddresses with additional check for duplicate nodes - Not needed */
	
/* 	void getDAGAddressesNew(set <struct CExprSimpNode*>  &addresses) */
/* 	  { */
	    
/* 	    if(!addresses.empty()) */
/* 	      { */
/* 		//cout<<"\nNon-empty\n"; */
		
/* 		if(addresses.find(this) != addresses.end()) */
/* 		  { */
/* 		    //cout<<"\nAlready traversed\n"; */
/* 		    return; */
/* 		  } */
/* 		else */
/* 		  { */
/* 		    // new node. Checking if there already exists a node with the same data as this */
		    
/* 		    //cout<<"\nNew node\n"; */
		    
/* 		    for(set <struct CExprSimpNode*>::iterator  addr_it = addresses.begin(); addr_it != addresses.end(); addr_it++) */
/* 		      { */
/* 			if((*addr_it)->getCExprSimpNodeStr() == this->getCExprSimpNodeStr()) */
			  
/* 			  { */
/* 			    cout<<"\nThere is already a node" <<(*addr_it)<<" with the same data as "<<this<<" . Hence exiting\n"; */
/* 			    exit(1); */
/* 			  } */
		
/* 		      } */
			  
			

/* 		    addresses.insert(this); */
		    
/* 		    list<struct CExprSimpNode*>::iterator iout = out_list.begin(); */

/* 		    for(; iout != out_list.end(); iout++) */
/* 		      { */
/* 			(*iout)->getDAGAddressesNew(addresses); */
/* 		      } */
/* 		  } */
/* 	      } */
	    
/* 	    else */
/* 		 { */
		   
/* 		    addresses.insert(this); */
		    
/* 		    list<struct CExprSimpNode*>::iterator iout = out_list.begin(); */

/* 		    for(; iout != out_list.end(); iout++) */
/* 		      { */
/* 			(*iout)->getDAGAddressesNew(addresses); */
/* 		      } */
/* 		  } */
	    
/* 	  } */


	void printDAGStructure()
	  {
	    set <struct CExprSimpNode*> addresses;  

	    getDAGAddresses(addresses);

	  /*   cout<<"\nDAG STRUCTURE\n*****************\n"; */

/* 	    // Print the addresses, in_list, out_list */

/* 	    for(set <struct CExprSimpNode*>::iterator addr_it = addresses.begin(); addr_it != addresses.end(); addr_it++) */
/* 	      { */
/* 		cout<<"\n"<<(*addr_it)<<"\n"<<"####################"<<endl; */

/* 		cout<<"\nContent = "<<(*addr_it)->getCExprSimpNodeStr(); */

/* 		cout<<"\nOut List = "; */

/* 		list<struct CExprSimpNode*>::iterator iout = ((*addr_it)->out_list).begin(); */

/* 		for(; iout != ((*addr_it)->out_list).end(); iout++) */
/* 		      { */
/* 			cout<<(*iout)<<","; */
/* 		      } */

/* 		cout<<"\nIn List = "; */

/* 		list<struct CExprSimpNode*>::iterator iin = ((*addr_it)->in_list).begin(); */

/* 		for(; iin != ((*addr_it)->in_list).end(); iin++) */
/* 		      { */
/* 			cout<<(*iin)<<","; */
/* 		      } */
/* 	      } */

	    // Check the correctness of the DAG -- DAG is correct if for all nodes n , for all o. , o is in out_list(a) => a is in in_list(o).. Exiting when this is not true

	    for(set <struct CExprSimpNode*>::iterator addr_it = addresses.begin(); addr_it != addresses.end(); addr_it++)
	      {
		list<struct CExprSimpNode*>::iterator iout = ((*addr_it)->out_list).begin();

		for(; iout != ((*addr_it)->out_list).end(); iout++)
		      {
			list<struct CExprSimpNode*> InList = (*iout)->in_list;
			
			if( InList.empty())
			  {
			    cout<<"\n"<<(*iout) <<" is there in the out list of "<<(*addr_it)<<", but "<< (*addr_it) <<" is not there in the in list of "<<(*iout);
			    exit(1);
			  }
			else 
			  {
			    bool found = false;

			    for(list<struct CExprSimpNode*>::iterator InList_it = InList.begin(); InList_it != InList.end(); InList_it++)
			      {
				if( (*InList_it) == (*addr_it))
				  { found = true; break;}
			      }

			    if(found == false)
			      {
				cout<<"\n"<<(*iout) <<" is there in the out list of "<<(*addr_it)<<", but "<< (*addr_it) <<" is not there in the in list of "<<(*iout);
				exit(1);
			      }
			  } 
		      }

	
	      }

	    cout<<"\n\nDAG STRUCTURE IS CORRECT\n\n";
	    
	   
	  }


};

struct node_info
{
	char* expr;
	CExprSimpNode* node_ptr;
};

struct argument;

class Variable
{
public:
	string name;
	string id_or_value;

	Variable(){}
	Variable(string n, string v)
	{
		name = n;
		id_or_value = v;
	}
};

class Term
{
public:
	string function_name;
	string id_or_value;
	list<argument *> args;

	Term(){}
	Term(string fname, string iov, list<argument *>& as)
	{
		function_name = fname;
		args.splice(args.begin(), as);
		id_or_value = iov;
	}

	void print();

	string get_string();
};

struct argument
{
	bool is_term;
	union
	{
		Term *r;
		Variable * v;
	} var_or_term;

	void print()
	{
		if(is_term)
		{
			cout<<"Rule:"<<endl;
			Term *temp = (var_or_term.r);
			temp->print();
		}
		else
		{
			cout<<"var_name:"<<var_or_term.v->name<<endl;
			cout<<"var_id_or_value:"<<var_or_term.v->id_or_value<<endl;
		}
	}

	string get_string()
	{
		if(is_term)
		{
			Term *temp = (var_or_term.r);
			return temp->get_string();
		}
		else
		{
			return var_or_term.v->name + "_" +var_or_term.v->id_or_value;
		}

	}
};


class Rule
{
public:
	Term *lhs;
	Term *rhs;
	Rule(Term *p_lhs, Term *p_rhs)
	{
		lhs = p_lhs;
		rhs = p_rhs;
	}

	void print();
	string get_string()
	{
		string str_lhs = lhs->get_string();
		string str_rhs = rhs->get_string();

		return str_lhs + " --> " + str_rhs ;

	}
};


class AffectedNodes
{
public:
	Rule *ra;
	struct CExprSimpNode* root;
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> > afEdges;
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> > lhsEdges;

	AffectedNodes(Rule *mrule, struct CExprSimpNode* r, 
			list < pair<struct CExprSimpNode*, struct CExprSimpNode *> > affs,
			list < pair<struct CExprSimpNode*, struct CExprSimpNode *> > lhss
			)
	{
		ra = mrule;
		root = r;
		afEdges.splice(afEdges.begin(), affs);
		lhsEdges.splice(lhsEdges.begin(), lhss);
	}

	bool include(AffectedNodes *af);
};

class gr_node
{
public:
	int index;
	int cost;
	AffectedNodes *af;
	gr_node():cost(0),index(-1){}
	gr_node(int ind, int t, AffectedNodes *a):cost(t),index(ind),af(a)
	{
	}
};



#endif



