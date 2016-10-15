#include "CExprSimpStructure.h"

#include "math.h"
#include "string.h"


extern map<string, int> functionArgs;
extern map<string, list<Rule*> > RewriteRules;
extern map<Rule*, list<struct CExprSimpNode *> > MatchedRules;
extern map<struct CExprSimpNode *, set<Rule*> > NodesDone;
extern struct CExprSimpNode* ExprSimpRoot;

extern map<struct CExprSimpNode *, list< int > > NodeAffectedNo;
extern map<int, list< AffectedNodes* > > AffectedArea;

extern list< AffectedNodes* > AllAffectedArea;

extern map<struct CExprSimpNode *, set<AffectedNodes*> >NodeAff;



void process_node(struct CExprSimpNode * r);


// bool find_in_labels(string name, list<string> &labels)
// {
// 	list<string>::iterator istr;
// 	for(istr = labels.begin(); istr != labels.end() ;istr++)
// 	{
// 		if(name == (*istr))
// 		{
// 			return true;
// 		}
// 	}
//
// 	return false;
// }
//
// bool is_same(string avalue, string nname)
// {
// 	if(avalue == "1" && nname == "1'b1")
// 	{
// 		return true;
// 	}
// 	else if(avalue == "0" && nname == "1'b0")
// 	{
// 		return true;
// 	}
//
// 	return false;
// }
//
// bool check_matching(Term* lhs,struct CExprSimpNode *r)
// {
// 	list<argument *>::iterator ia = lhs->args.begin();
// 	list<struct CExprSimpNode*>::iterator iout = r->out_list.begin();
//
// 	for(int i=0; i < lhs->args.size(); i++)
// 	{
// 		if((*ia)->is_term)
// 		{
// 			//cout<<"again rule"<<endl;
// 			if( !check_matching((*ia)->var_or_term.r, (*iout)))
// 			{
// 				return false;
// 			}
// 		}
// 		else
// 		{
// 			//cout<<"why why "<<(*ia)->var_or_term.v->name;
//
// 			//cout<<"  "<<(*ia)->var_or_term.v->id_or_value;
// 			//cout<<"  "<< (*ia)->var_or_term.v->id_or_value;
//
// 			//cout<<"  "<< (*((*iout)->labels.begin())) << endl;
//
// 			if((*ia)->var_or_term.v->name == "c" && ((*ia)->var_or_term.v->id_or_value == "x" || (*ia)->var_or_term.v->id_or_value == "y") && (*((*iout)->labels.begin())) == "c")
// 			{
// 				//cout<<"Any const"<<endl;
// 			}
// 			else if((*ia)->var_or_term.v->name == "c" && (*((*iout)->labels.begin())) == "c" )
// 			{
// 				//cout<<"Fixed const::"<<(*ia)->var_or_term.v->id_or_value;
// 				//cout<<"    "<<(*iout)->name <<endl;
// 				if(!is_same((*ia)->var_or_term.v->id_or_value, (*iout)->name))
// 				{
// 					return false;
// 				}
// 			}
// 			else if(!find_in_labels((*ia)->var_or_term.v->name,(*iout)->labels))
// 			{
// 				//cout<<"Any var"<<endl;
// 				return false;
// 			}
// 		}
//
// 		iout++;
// 		ia++;
// 	}
// 	return true;
// }

void get_correspondance(Term* lhs, struct CExprSimpNode *r, map <string, struct CExprSimpNode *> &correspondance,
						map <struct CExprSimpNode *, struct CExprSimpNode *>  &parent_to_be_deleted,
						map <string, list<argument *> >&  same_nodes)
{
	list<argument *>::iterator ia = lhs->args.begin();
	list<struct CExprSimpNode*>::iterator iout = r->out_list.begin();

	//cout<<"get_correspondance1"<<endl;

	for(int i=0; i < lhs->args.size(); i++)
	{
		//cout<<"get_correspondance1"<<endl;
		if((*ia)->is_term)
		{
			//cout<<"get_correspondance2"<<endl;
			get_correspondance((*ia)->var_or_term.r, (*iout), correspondance, parent_to_be_deleted, same_nodes);
			//cout<<"get_correspondance5"<<endl;
		}
		else
		{
			//cout<<"get_correspondance3"<<endl;
			string temp = (*ia)->var_or_term.v->name + "_" + (*ia)->var_or_term.v->id_or_value;
			
			//cout<<"\nInserting ("<<temp<<", "<<(*iout)<<"into correspondance\n";
			//cout<<"\nInserting ("<<(*iout)<<", "<<r<<"into parent_to_be_deleted\n";
			
			//correspondance.insert(make_pair(temp, (*iout) ));
			correspondance.insert(pair<string, struct CExprSimpNode*>(temp, (*iout) ));
			parent_to_be_deleted.insert(make_pair((*iout), r));
			//cout<<"get_correspondance4"<<endl;

			if(same_nodes.find(temp) != same_nodes.end())
			{
				same_nodes[temp].push_back(*ia);
			}
			else
			{
				list<argument *> a;
				a.push_back(*ia);
				same_nodes.insert(make_pair(temp,a));
			}

		}

		//cout<<"get_correspondance6"<<endl;
		ia++;
		iout++;
	}
}



string binarytodecimal(const string & rhs)
{
	long sum = 0;
	for (int i=0;i <rhs.length();i++)
	{
		if (rhs[i] == '0')
		{
			sum *= 2;
			continue;
		}
		else if (rhs[i] == '1')
		{
			sum = (sum*2) + 1;
			continue;
		}
		else
		{
			break;
		}
	}

	stringstream ss;
	ss << sum;
	return ss.str();
}



int hextodecimal(char *bin)

{

     char *ptr;//pointer to a char

     int value = 0; // the value of the current hex char pointed to

     int sum = 0;// the running sum

     int power = -1;//power is what base 16 is raised to starts at -1 to count for 0

     ptr = bin;//set the pointer to point at start of string ie 0xabcd

     ptr += 2;//first digit after the 0x ie a


     while(*ptr != (char)NULL)//while pointer is not null count a new char
     {
          ptr++;
          power++;
     }

     ptr = &bin[2];//sets pointer to point at first value after 0x ie a

     while(*ptr != (char)NULL)//while pointer is not equal to null
     {
          switch(*ptr)

          {

			  case '0': value = 0; break;

			  case '1': value = 1; break;

			  case '2': value = 2; break;

			  case '3': value = 3; break;

			  case '4': value = 4; break;

			  case '5': value = 5; break;

			  case '6': value = 6; break;

			  case '7': value = 7; break;

			  case '8': value = 8; break;

			  case '9': value = 9; break;

			  case 'a': value = 10; break;

			  case 'b': value = 11; break;

			  case 'c': value = 12; break;

			  case 'd': value = 13; break;

			  case 'e': value = 14; break;

			  case 'f': value = 15; break;

			  default: printf("Input Error not 0-9 or a-f"); exit(1);

          }


          sum += pow(16,power)*value;//this calculates the total until pointer points to null

          ptr++;//moves the pointer along to the next char

          power--;//decrements the power

     }

     printf("\nHex: 0x%x, Decimal: %d ", sum, sum);

     return sum;
}




int get_int_value(string value)
{
	int result;

	int index = value.find("'");

	string temp;

	switch(value.at(index+1))
	{
		case 'b':
		case 'B':
			temp = binarytodecimal(value.substr(index+2));
			result = atoi(temp.c_str());
		break;

		case 'd':
		case 'D':
			//cout<<"to be converted ::"<< value.substr(index+2) <<endl;
			temp = value.substr(index+2);
			result = atoi(temp.c_str());
		break;

		case 'h':
		case 'H':
			char ctemp[50];
			strcpy(ctemp, value.substr(index+2).c_str());
			result = hextodecimal(ctemp);
		break;
	}

	return result;
}

void delete_and_add(list<struct CExprSimpNode *>& node_list, struct CExprSimpNode * remove, struct CExprSimpNode * add)
{
	list<struct CExprSimpNode *>::iterator iter;

	//cout<<"\nremove = "<<remove<<endl;
	//cout<<"\nadd = "<<add<<endl;
	//cout<<"\nnode_list\n";
	//for(iter = node_list.begin(); iter != node_list.end(); iter++)
	//{
	//        
	//  cout<<(*iter)<<"\t";
	//}


	//cout<<"delete_and_add1"<<endl;

	for(iter = node_list.begin(); iter != node_list.end(); iter++)
	{
	        
	  //cout<<"delete_and_add2"<<endl;
		if((*iter) == remove)
		{
			break;
		}
		//cout<<"delete_and_add3"<<endl;
	}

	if(iter != node_list.end())
	  {
	    list<struct CExprSimpNode *>::iterator iter1 = node_list.erase(iter);
	    //cout<<"delete_and_add4"<<endl;
	    // 	if(iter1 != node_list.begin())
	    // 	{
	    // 		iter1--;
	    // 	}
	    node_list.insert(iter1, add);
	  }
	else
	  {
	    node_list.push_back(add);
	  }
	
	//cout<<"delete_and_add5"<<endl;
}

void copy_list(list<struct CExprSimpNode*> &slist, list<struct CExprSimpNode*> &dlist)
{
	list<struct CExprSimpNode *>::iterator iout;

	for(iout = slist.begin(); iout != slist.end(); iout++)
	{
		dlist.push_back(*iout);
	}
}

struct CExprSimpNode *evalalate(struct CExprSimpNode *r, Term* rhs, map <string, struct CExprSimpNode *> &correspondance)
{
	list<argument *>::iterator iarg1 = rhs->args.begin();

	string oper = (*iarg1)->var_or_term.r->function_name;


	list<argument *>::iterator iarg = (*iarg1)->var_or_term.r->args.begin();



	int no_of_args = functionArgs[oper];

	//cout<<"evalalate1   "<<oper<<"    "<<no_of_args <<endl;

	vector<int> args;

	for(int i =0; i<no_of_args; i++)
	{
		//cout<<"entry"<<endl;
		//cout<<"evalalate2::"<<(*iarg)->var_or_term.v->name + "_" + (*iarg)->var_or_term.v->id_or_value<<endl;
		struct CExprSimpNode *n = correspondance[(*iarg)->var_or_term.v->name + "_" + (*iarg)->var_or_term.v->id_or_value];
		//cout<<"evalalate3"<<endl;
		int val = get_int_value(n->name);
		//cout<<"evalalate4  "<<val<<endl;
		args.push_back(val);
		iarg++;
	}

	//cout<<"evalalate5"<<endl;

	int result;
	if(oper == "Add")
	{
		result = args[0] + args[1];
	}
	else if(oper == "Sub")
	{
		result = args[0] - args[1];
	}

	//cout<<"evalalate6"<<endl;

	//TODO function
	//string str_val = convert_to_string(result);

	//string str_val<<bin << result;

	char temp[50];

	sprintf(temp, "5d'%d",result);

	string str_val = temp ;

	CExprSimpNode *n_ptr = new CExprSimpNode();
	n_ptr->name = str_val;

	copy_list(r->in_list, n_ptr->in_list);

	list<struct CExprSimpNode *>::iterator iin;

	for(iin = n_ptr->in_list.begin(); iin != n_ptr->in_list.end(); iin++)
	{
		delete_and_add((*iin)->out_list, r, n_ptr);
	}

	return n_ptr;
}


void remove_edges(struct CExprSimpNode * r)
{
	list<struct CExprSimpNode *>::iterator iout;

	//cout<<"remove_edges1"<<endl;

	for(iout = r->out_list.begin(); iout != r->out_list.end(); iout++)
	{
		//cout<<"remove_edges2"<<endl;
		(*iout)->in_list.remove(r);
	}
}

void clear_other_nodes(map <string, list<argument *> >& same_nodes)
{
	map <string, list<argument *> >::iterator iout = same_nodes.begin();

	while(iout != same_nodes.end() )
	{
		if( (*iout).second.size() == 1)
		{
			same_nodes.erase(iout);
		}
		else
		{
			iout++;
		}
	}
}

void remove_from_all_matched_rules(struct CExprSimpNode *n)
{
	set<Rule*>::iterator in = NodesDone[n].begin();

	//cout<<"remove_from_all_matched_rules1"<<endl;

	for(; in != NodesDone[n].end() ;in++)
	{
		//cout<<"remove_from_all_matched_rules2"<<endl;
		MatchedRules[*in].remove(n);
	}

	list<int>::iterator iter = NodeAffectedNo[n].begin();

	for(; iter != NodeAffectedNo[n].end(); iter++)
	{
		list< AffectedNodes* > &lafs = AffectedArea[*iter];
		list< AffectedNodes* >::iterator iter1;
	
		for(iter1 = lafs.begin(); iter1 != lafs.end(); iter1++)
		{
			if(n == (*iter1)->root )
			{
				iter1 = lafs.erase(iter1);
				iter1--;
			}
		}
	}

	set<AffectedNodes*>::iterator iaff = NodeAff[n].begin();

	for(; iaff != NodeAff[n].end(); iaff++)
	{
		AllAffectedArea.remove(*iaff);
	}

	NodesDone.erase(n);
	NodeAffectedNo.erase(n);
}


void show_correspondance_parent_to_be_deleted(map <string, struct CExprSimpNode *> correspondance, map <struct CExprSimpNode *, struct CExprSimpNode *>  parent_to_be_deleted)
{
  cout<<"\n\nCORRESPONDENCE\n\n";
  
  map <string, struct CExprSimpNode *>::iterator it;

  for(it = correspondance.begin(); it != correspondance.end(); it++)
    {
      cout<<it->first<<"\t"<<it->second<<"\t"<<(it->second)->getCExprSimpNodeStr()<<endl;
    }

  cout<<"\n\nparent_to_be_deleted\n\n";
  
  map <struct CExprSimpNode *, struct CExprSimpNode *>::iterator it1;

  for(it1 = parent_to_be_deleted.begin(); it1 != parent_to_be_deleted.end(); it1++)
    {
      cout<<it1->first<<"\t"<<it1->second<<endl;
    }
}

map <struct CExprSimpNode *, struct CExprSimpNode *> modify_parent_to_be_deleted(map <struct CExprSimpNode *, struct CExprSimpNode *> parent_to_be_deleted, struct CExprSimpNode *r)
{
  map <struct CExprSimpNode *, struct CExprSimpNode *> new_parent_to_be_deleted;

  for(map <struct CExprSimpNode *, struct CExprSimpNode *>::iterator it1 = parent_to_be_deleted.begin(); it1 != parent_to_be_deleted.end(); it1++)
    {
      if(it1->second == r)
	new_parent_to_be_deleted.insert(make_pair(it1->first, r));
    }
  return new_parent_to_be_deleted;
}



struct CExprSimpNode* recursiveConstruct(Term *rhs, map <string, struct CExprSimpNode *> correspondance, map <struct CExprSimpNode *, struct CExprSimpNode *> parent_to_be_deleted)
{  
  //cout<<"\nrhs->function_name = "<<rhs->function_name<<endl;

		int no_of_args = functionArgs[rhs->function_name];

		CExprSimpNode *n_ptr = new CExprSimpNode();
		n_ptr->name = rhs->function_name;

		list<argument *>::iterator iarg = rhs->args.begin();
		list<struct CExprSimpNode *> args;
		for(int i =0; i<no_of_args; i++)
		{

		  if((*iarg)->is_term)
		    {
		      //cout<<"\nChild is term\n";
		      struct CExprSimpNode *n = recursiveConstruct((*iarg)->var_or_term.r, correspondance, parent_to_be_deleted);
		      args.push_back(n);
		      n->in_list.push_back(n_ptr);
		      
		      }
		  else
			{
			  //cout<<"\nChild is not a term\n";
			//show_correspondance_parent_to_be_deleted(correspondance, parent_to_be_deleted);

			struct CExprSimpNode *n = correspondance[(*iarg)->var_or_term.v->name + "_" + (*iarg)->var_or_term.v->id_or_value];
			//cout<<"corres="<<(*iarg)->var_or_term.v->name + "_" + (*iarg)->var_or_term.v->id_or_value<<endl;
			//cout<<"address="<<n;

			args.push_back(n);
			
			
			if(parent_to_be_deleted.empty())
			  {
			    //cout<<"\n"<<n_ptr<<" is added into the in list of "<<n<<endl;  
			  n->in_list.push_back(n_ptr);
			  }
			
			else if(parent_to_be_deleted.find(n) == parent_to_be_deleted.end())
			  {
			    //cout<<"\n"<<n_ptr<<" is added into the in list of "<<n<<endl;
			  n->in_list.push_back(n_ptr);
			  }
			
			else
			  {
			    //cout<<"\n"<<parent_to_be_deleted[n]<<"is deleted from and "<<n_ptr<<" is added into the in list of "<<n<<endl;  
			  delete_and_add(n->in_list, parent_to_be_deleted[n], n_ptr); 
			  }
			
			//delete_and_add(n->in_list, parent_to_be_deleted[n], n_ptr); OLD CODE
			
			}
		iarg++;
		}

		
		copy_list(args, n_ptr->out_list);
		
		return n_ptr;
}



struct CExprSimpNode * take_action(Rule* pRule, list<struct CExprSimpNode *>& matched_nodes)
{
	list<struct CExprSimpNode *>::iterator imn = matched_nodes.begin();
	struct CExprSimpNode *r = (*imn);


	//	cout<<"Hi";
	//	 cout<<"\n\nApplied at node "<<r<<endl<<endl;
// 	cout<<"\n\nApplied at node "<<r<<endl<<endl;
// 	cout<<"\n\nApplied at node "<<r<<endl<<endl;
// 	cout<<"\n\nApplied at node "<<r<<endl<<endl;
// 	cout<<"\n\nApplied at node "<<r<<endl<<endl;
// 	cout<<"\n\nApplied at node "<<r<<endl<<endl;

	// Newly added to debug
	if(false)
	  {
	    cout<<"\n\nApplied at node "<<r<<" with name = "<<r->name<<endl<<endl; // Newly added
	    if(r->name=="Log_Equal")
	      {
		cout<<"\nChildren...\n";
		list<struct CExprSimpNode*>::iterator iout = r->out_list.begin();
		cout<<(*iout)->name<<"\n";
		iout++;
		cout<<(*iout)->name<<"\n";
	      }
	  }
	    

	//	cout<<"\n\nApplied at node with content = "<<r->getCExprSimpNodeStr()<<endl<<endl; // Newly added

// 	cout<<"\n\nApplied at node "<<r<<" with content = "<<r->getCExprSimpNodeStr()<<endl<<endl; // Newly added
// 	cout<<"\n\nApplied at node "<<r<<" with content = "<<r->getCExprSimpNodeStr()<<endl<<endl; // Newly added
// 	cout<<"\n\nApplied at node "<<r<<" with content = "<<r->getCExprSimpNodeStr()<<endl<<endl; // Newly added
// 	cout<<"\n\nApplied at node "<<r<<" with content = "<<r->getCExprSimpNodeStr()<<endl<<endl; // Newly added
// 	cout<<"\n\nApplied at node "<<r<<" with content = "<<r->getCExprSimpNodeStr()<<endl<<endl; // Newly added


//	//cout<<"take_action1 == "<<rule<<endl;

// 	if(RewriteRules.find(rule) == RewriteRules.end())
// 	{
// 		//cout<<"--NO RULE FOUND ERROR--"<<endl;
// 	}
//
// 	pair<Rule*, Rule*> pRule = RewriteRules[rule];

	Term* lhs = pRule->lhs;
	Term* rhs = pRule->rhs;

	//cout<<"take_action2 : lhs->function_name =  "<<lhs->function_name<<", rhs->function_name =   "<<rhs->function_name<<endl;

	map <string, struct CExprSimpNode *>  correspondance;
	map <struct CExprSimpNode *, struct CExprSimpNode *>  parent_to_be_deleted;
	map <string, list<argument *> >  same_nodes;


	//cout<<"take_action3"<<endl;

	get_correspondance(lhs, r, correspondance, parent_to_be_deleted, same_nodes);

	//show_correspondance_parent_to_be_deleted(correspondance, parent_to_be_deleted);

	parent_to_be_deleted = modify_parent_to_be_deleted(parent_to_be_deleted, r);

	// parent_to_be_deleted currently contains nodes other than r - node to replaced. Nodes other than r should not be deleted. Hence removing these entries in modify_parent_to_be_deleted

	//show_correspondance_parent_to_be_deleted(correspondance, parent_to_be_deleted);
	
      

// 	clear_other_nodes(same_nodes);

	//cout<<"take_action4"<<endl;

// 	if(!check_matching(lhs,r))
// 	{
// 		//cout<<"take_action5"<<endl;
// 		matched_nodes.erase(imn);
// 		//cout<<"take_action5.1"<<endl;
// 		return NULL;
// 	}

	//cout<<"take_action6"<<endl;

	struct CExprSimpNode *result;

	// Newly added

	//cout<<"\nrhs->function_name = "<<rhs->function_name<<endl;

	// Newly added ends here


/*	if(functionArgs.find(rhs->function_name) == functionArgs.end())
	{
		//cout<<"eval"<<endl;
		result = evalalate(r, rhs, correspondance);
	}*/
	if(functionArgs.find(rhs->function_name) == functionArgs.end())
	{
	  //cout<<"take_action10"<<endl;
	  
	  // Following line argument *arg = (*(rhs->args.begin())); commented on 1/2/2010 as arg is not at all used and is meaingless if rhs is not a function
	  
	  //argument *arg = (*(rhs->args.begin()));
		
	  //cout<<"\nArgument to correspondance = "<<rhs->function_name + "_" + rhs->id_or_value<<endl;
		
		 // Following check for presence of arg_to_correspondance in correspondence is added on 1/2/2010 as if rhs is c_1 say in rule ite,e_1,e_1-->c_1 if e_1=e_2, correspondance[c_1] is correspondance.end()
		

		string arg_to_correspondance = rhs->function_name;
		arg_to_correspondance += "_";
		arg_to_correspondance += rhs->id_or_value;
		
		//cout<<"\nArgument to correspondance = "<<arg_to_correspondance<<endl;
		

		map <string, struct CExprSimpNode *>::iterator correspondance_it = correspondance.find(arg_to_correspondance);

		//cout<<"\nSearching done\n";

		if(correspondance_it != correspondance.end())
		  {
		    //cout<<"\ncorrespondance_it != correspondance.end()\n";
		  result = correspondance_it->second;
		  }
		else 
		  {
		    if(arg_to_correspondance == "c_1")
		      {
			result = new CExprSimpNode();
			result->name = "1'b1"; 
		      }
		    else if(arg_to_correspondance == "c_0")
		      {
			result = new CExprSimpNode();
			result->name = "1'b0"; 
		      }
		    else
		      {
			cout<<"\nResult which is matching with "<<arg_to_correspondance<<" cannot be created inside function take_action inside file incr_update.cpp\n";
			exit(1);
		      }
		  }

		//cout<<"\nresult = "<<result<<" with content = "<<result->getCExprSimpNodeStr()<<endl;

		//cout<<"take_action12::"<<r->in_list.size() <<"  "<<r->out_list.size()<<endl;

		// Inlist of result is
		// cout<<"\n\nresult's InList contains\n\n";
// 		list<struct CExprSimpNode *>::iterator ResultInListIt;
// 		for(ResultInListIt = result->in_list.begin(); ResultInListIt != result->in_list.end(); ResultInListIt++)
// 			{
// 			  cout<<(*ResultInListIt)<<"\t";
// 			}
// 		cout<<"\nOVER\n";

		//result->in_list.clear(); // Why to clear it?? : result is an existing node referred by other nodes - hence deleting it's in list is incorrect
		copy_list(r->in_list, result->in_list);

		// Inlist of result is
	// 	cout<<"\n\nAfter clearing and copying r's in list, result's InList contains\n\n";
		
// 		for(ResultInListIt = result->in_list.begin(); ResultInListIt != result->in_list.end(); ResultInListIt++)
// 			{
// 			  cout<<(*ResultInListIt)<<"\t";
// 			}
// 		cout<<"\nOVER\n";
		
		//set <struct CExprSimpNode*> addresses;
		//int size = 0;

		//ExprSimpRoot->computeDAGSize(addresses, size);
				


		if(r->in_list.size() != 0)
		{
			list<struct CExprSimpNode *>::iterator iin;
			for(iin = r->in_list.begin(); iin != r->in_list.end(); iin++)
			{
				//cout<<"for1333"<<endl;
			       
				delete_and_add((*iin)->out_list, r, result);
			}
		}
		//cout<<"take_action11"<<endl;
	}
	else
	{
	  //cout<<"take_action7"<<endl;
		
	  //cout<<"\nrhs->function_name = "<<rhs->function_name<<endl;
	  
		int no_of_args = functionArgs[rhs->function_name];

		CExprSimpNode *n_ptr = new CExprSimpNode();
		n_ptr->name = rhs->function_name;

		list<argument *>::iterator iarg = rhs->args.begin();
		list<struct CExprSimpNode *> args;
		for(int i =0; i<no_of_args; i++)
		{

		  if((*iarg)->is_term)
		    {
		      //cout<<"\nChild is term\n";
		      //show_correspondance_parent_to_be_deleted(correspondance, parent_to_be_deleted);
		      struct CExprSimpNode *n = recursiveConstruct((*iarg)->var_or_term.r, correspondance, parent_to_be_deleted);
		      
		      //cout<<"\nNode created by recursiveConstruct is "<<n<<" which contains "<<n->getCExprSimpNodeStr()<<endl;
		      args.push_back(n);
		      n->in_list.push_back(n_ptr);
		      
		      }
		   else
			{

			  //cout<<"\nChild is not a term\n";
			struct CExprSimpNode *n = correspondance[(*iarg)->var_or_term.v->name + "_" + (*iarg)->var_or_term.v->id_or_value];
			
			//cout<<"\nNode n is "<<n<<" which contains "<<n->getCExprSimpNodeStr()<<endl;
			//cout<<"take_action9.1"<<endl;
			//cout<<"corres="<<(*iarg)->var_or_term.v->name + "_" + (*iarg)->var_or_term.v->id_or_value<<endl <<"address"<<n;
			args.push_back(n);
			//cout<<"take_action9.2"<<endl;
			
			if(parent_to_be_deleted.empty())
			  {
			    //cout<<"\n"<<n_ptr<<" is added into the in list of "<<n<<endl;  
			  n->in_list.push_back(n_ptr);
			  }
			
			else if(parent_to_be_deleted.find(n) == parent_to_be_deleted.end())
			  {
			    //cout<<"\n"<<n_ptr<<" is added into the in list of "<<n<<endl;  
			  n->in_list.push_back(n_ptr);
			  }
			
			else
			  {
			    //cout<<"\n"<<parent_to_be_deleted[n]<<"is deleted from and "<<n_ptr<<" is added into the in list of "<<n<<endl;  
			  delete_and_add(n->in_list, parent_to_be_deleted[n], n_ptr); 
			  }
			
			//delete_and_add(n->in_list, parent_to_be_deleted[n], n_ptr);  OLD CODE
			//cout<<"take_action23"<<endl;
			}
		iarg++;
		}

		//cout<<"args::"<<args.size()<<endl;
		//cout<<"take_action10"<<endl;
		copy_list(args, n_ptr->out_list);
		//cout<<"take_action11"<<endl;
		copy_list(r->in_list, n_ptr->in_list);
		//cout<<"take_action12"<<endl;
		//cout<<"take_action12::"<<n_ptr->in_list.size() <<"  "<<n_ptr->out_list.size()<<endl;
		if(n_ptr->in_list.size() != 0)
		{
			list<struct CExprSimpNode *>::iterator iin;
			for(iin = n_ptr->in_list.begin(); iin != n_ptr->in_list.end(); iin++)
			{
				//cout<<"for1333"<<endl;
				delete_and_add((*iin)->out_list, r, n_ptr);
			}
		}
		//cout<<"take_action8"<<endl;
		result = n_ptr;
	}

	//matched_nodes.erase(imn);

	remove_from_all_matched_rules(r);

	remove_edges(r);

	if(r == ExprSimpRoot)
	{
		ExprSimpRoot = result;
	}

	//cout<<"take_action9"<<endl;
	return result;
}

bool comp_str(string s1, string s2)
{
	if(s1.compare(s2) == 0)
	{
		return true;
	}
	else
	{
		return true;
	}
}

int comp_str_sort (string s1, string s2)
{
	return s1.compare(s2);
}


bool is_lists_equal(list<string> list1, list<string> list2)
{

	list<string> temp = list1;
	list<string>::iterator l2;

	for(l2 = list2.begin(); l2 != list2.end(); l2++)
	{
		temp.remove(*l2);
	}

	if(temp.size() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}


}

// bool is_lists_equal(list<string> list1, list<string> list2)
// {
// 	list<string>::iterator l1;
// 	list<string>::iterator l2;
//
// 	for(l1 = list1.begin(), l2 = list2.begin(); l1 != list1.end(); l1++, l2++)
// 	{
// 		if((*l1) == (*l2))
// 		{
// 			return false;
// 		}
// 	}
//
// 	return true;
// }

void copy_labels(list<string> &list1, list<string> &list2)
{
	list<string>::iterator l1;

	for(l1 = list1.begin(); l1 != list1.end(); l1++)
	{
		list2.push_back(*l1);
	}
}

void increamental_update(struct CExprSimpNode * r)
{
	//cout<<"#### in  increamental_update####"<<endl;
	list<string> labels_temp;
	copy_labels(r->labels, labels_temp);

	//cout<<"#### nooooo ####"<<endl;

	if(r->labels.size() == labels_temp.size() && is_lists_equal(r->labels, labels_temp))
	{
		//cout<<"#### yesss####"<<endl;
	}


	r->labels.clear();

	//cout<<"\n\nRecalculating lables of "<<r<<endl<<endl;

	process_node(r);

	
	r->labels.sort();
	labels_temp.sort();

	if(r->labels.size() == labels_temp.size() && is_lists_equal(r->labels, labels_temp))
	{
	  //cout<<"#### no need to recurse####"<<endl;
	}
	else
	{
	  //cout<<"#### recursing ####"<<endl;

		list<struct CExprSimpNode*>::iterator in;
		for(in = r->in_list.begin(); in != r->in_list.end(); in++)
		{
		  //cout<<"\n\nRecalculating lables of "<<(*in)<<endl<<endl;
			increamental_update(*in);
		}
	}
}
