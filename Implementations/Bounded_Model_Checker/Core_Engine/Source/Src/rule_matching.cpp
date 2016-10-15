#include "CExprSimpStructure.h"

extern map<string, list<Rule*> > RewriteRules;
extern map<string, bool> ValidSubstrings;
extern map<Rule*, int> RulesWithPriority;
extern map<Rule*, Term*> RulesWithCondition;

map<Rule*, list<struct CExprSimpNode *> > MatchedRules;
map<int, list<Rule*> > MatchedRulesWithPriority;

map<int, list< AffectedNodes* > > AffectedArea;

map<struct CExprSimpNode *, list< int > > NodeAffectedNo;
list< AffectedNodes* > AllAffectedArea;

map<struct CExprSimpNode *, set<Rule*> > NodesDone;
vector<string> operators;

map<struct CExprSimpNode *, set<AffectedNodes*> >NodeAff;

string evaluateGuardCondition(map<argument *, struct CExprSimpNode*> correspondance, Term* lhs);


bool is_oper(string expr)
{
	for(int i=0; i<operators.size(); i++)
	{
		if(expr == operators[i])
		{
			return true;
		}
	}

	return false;
}

/********************************************************************/

bool find_in_labels(string name, list<string> &labels)
{
	list<string>::iterator istr;
	for(istr = labels.begin(); istr != labels.end() ;istr++)
	{
		if(name == (*istr))
		{
			return true;
		}
	}

	return false;
}

bool is_same(string avalue, string nname)
{

  /* This was the earlier code : Changing 1'b1 to true and 1'b0 to false may not work */
	if(avalue == "1" && nname == "1'b1")
	{
		return true;
	}
	else if(avalue == "0" && nname == "1'b0")
	{
		return true;
 	}

// 	if(avalue == "1" && nname == "true")
// 	{
// 		return true;
// 	}
// 	else if(avalue == "0" && nname == "false")
// 	{
// 		return true;
// 	}

	return false;
}

void get_same_nodes(Term* lhs, struct CExprSimpNode *r, map <string, list<argument *> >&  same_nodes,
			map <argument *, struct CExprSimpNode*>&  correspondance)
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
			get_same_nodes((*ia)->var_or_term.r, (*iout), same_nodes, correspondance);
			//cout<<"get_correspondance5"<<endl;
		}
		else
		{
			//cout<<"get_correspondance3"<<endl;
			string temp = (*ia)->var_or_term.v->name + "_" + (*ia)->var_or_term.v->id_or_value;
			//cout<<"get_correspondance4"<<endl;
			correspondance[(*ia)]=(*iout);

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

bool check_matching(Term* lhs,struct CExprSimpNode *r)
{
	list<argument *>::iterator ia = lhs->args.begin();
	list<struct CExprSimpNode*>::iterator iout = r->out_list.begin();

	for(int i=0; i < lhs->args.size(); i++)
	{
		if((*ia)->is_term)
		{
			//cout<<"again rule"<<endl;
			if( !check_matching((*ia)->var_or_term.r, (*iout)))
			{
				return false;
			}
		}
		else
		{
			//cout<<"why why "<<(*ia)->var_or_term.v->name;

			//cout<<"  "<<(*ia)->var_or_term.v->id_or_value;
			//cout<<"  "<< (*ia)->var_or_term.v->id_or_value;

			//cout<<"  "<< (*((*iout)->labels.begin())) << endl;

			if((*ia)->var_or_term.v->name == "c" && ((*ia)->var_or_term.v->id_or_value == "x" || (*ia)->var_or_term.v->id_or_value == "y") && (*((*iout)->labels.begin())) == "c")
			{
				//cout<<"Any const"<<endl;
			}
			else if((*ia)->var_or_term.v->name == "c" && (*((*iout)->labels.begin())) == "c" )
			{
				//cout<<"Fixed const::"<<(*ia)->var_or_term.v->id_or_value;
				//cout<<"    "<<(*iout)->name <<endl;
				if(!is_same((*ia)->var_or_term.v->id_or_value, (*iout)->name))
				{
					return false;
				}
			}
			else if(!find_in_labels((*ia)->var_or_term.v->name,(*iout)->labels))
			{
				//cout<<"Any var"<<endl;
				return false;
			}
		}

		iout++;
		ia++;
	}
	return true;
}

/********************************************************************/

bool is_valid_substring(string t)
{
	if(ValidSubstrings.find(t) != ValidSubstrings.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void addMatchedRule(Rule* mrule, struct CExprSimpNode* r)
{
	//cout<<"----------------"<<endl;
	//cout<<"Priority:"<<RulesWithPriority[mrule]<<" Node: "<<r->name<<r<<endl;
	//cout<<"Size:"<<MatchedRulesWithPriority[RulesWithPriority[mrule]].size() <<endl;
	//cout<<"----------------"<<endl;

	if(MatchedRules.find(mrule) != MatchedRules.end())
	{
		list<struct CExprSimpNode *>& rules = MatchedRules[mrule];
		list<struct CExprSimpNode *>::iterator irule = rules.begin();

		for(; irule != rules.end() ; irule++)
		{
			if((*irule) == r)
			{
				return;
			}
		}
	}

	MatchedRules[mrule].push_back(r);
	MatchedRulesWithPriority[RulesWithPriority[mrule]].push_back(mrule);
}

bool is_similar(map <string, list<argument *> >&  same_nodes,
			map <argument *, struct CExprSimpNode*>&  correspondance)
{
	map <string, list<argument *> >::iterator isn = same_nodes.begin();

	while(isn != same_nodes.end())
	{
		list<argument *> largs = (*isn).second;
		list<argument *>::iterator il = largs.begin();

		argument *first = (*il);

		while(il != largs.end())
		{
			if(correspondance[first] != correspondance[*il])
			{
				return false;
			}

			il++;
		}

		isn++;
	}

	return true;
}

void get_lhs_edges(Term* lhs,struct CExprSimpNode *r, list< pair<string, string> > & lhsArgEdges,
	list< pair<CExprSimpNode*, CExprSimpNode*> > & lhsEdges,map< string, CExprSimpNode* >& allnodes)
{
	list<argument *>::iterator ia = lhs->args.begin();
	list<struct CExprSimpNode*>::iterator iout = r->out_list.begin();

	for(int i=0; i < lhs->args.size(); i++)
	{
		if((*ia)->is_term)
		{
			//cout<<"again rule"<<endl;
			get_lhs_edges((*ia)->var_or_term.r, (*iout), lhsArgEdges, lhsEdges, allnodes);
		}
		else
		{
			string arg1 = lhs->function_name;
			string arg2 = (*ia)->var_or_term.v->name + "_" + (*ia)->var_or_term.v->id_or_value;

			lhsArgEdges.push_back(make_pair(arg1, arg2));
			lhsEdges.push_back(make_pair(r, *iout));
			allnodes[arg2] = *iout;
		}

		iout++;
		ia++;
	}
}

void get_rhs_edges(Term* rhs, list< pair<string, string> > & rhsArgEdges)
{
	list<argument *>::iterator ia = rhs->args.begin();


	for(int i=0; i < rhs->args.size(); i++)
	{
		if((*ia)->is_term)
		{
			//cout<<"again rule"<<endl;
			get_rhs_edges((*ia)->var_or_term.r, rhsArgEdges);
		}
		else
		{
			string arg1 = rhs->function_name;
			string arg2 = (*ia)->var_or_term.v->name + "_" + (*ia)->var_or_term.v->id_or_value;

			rhsArgEdges.push_back(make_pair(arg1, arg2));
		}

		ia++;
	}

}

void delete_preserved_edges(list< pair<string, string> >&lhsArgEdges,
							list< pair<CExprSimpNode*, CExprSimpNode*> >& lhsEdges,
							list< pair<string, string> >& rhsArgEdges)
{
	list< pair<string, string> >::iterator ilae = lhsArgEdges.begin();
	list< pair<CExprSimpNode*, CExprSimpNode*> >::iterator ile = lhsEdges.begin();
	list< pair<string, string> >::iterator irae  = rhsArgEdges.begin();


	for(; irae  != rhsArgEdges.end(); irae++)
	{
		for(;ilae != lhsArgEdges.end()
			;ilae++,ile++)
		{
			if(*irae == *ilae)
			{
				ilae= lhsArgEdges.erase(ilae);
				ile = lhsEdges.erase(ile);
				ilae--;
				ile--;
			}
		}
	}
}

bool check_if_present(int size, AffectedNodes* af)
{
	list< AffectedNodes* > &lafs = AffectedArea[size];
	list< AffectedNodes* >::iterator iter1;

	for(iter1 = lafs.begin(); iter1 != lafs.end(); iter1++)
	{
		if(af->root == (*iter1)->root && af->ra == (*iter1)->ra )
		{
			return true;
		}
	}

	return false;
}

void copyEdges(list< pair<CExprSimpNode*, CExprSimpNode*> > lst1, list< pair<CExprSimpNode*, CExprSimpNode*> >& lst2)
{
	list< pair<CExprSimpNode*, CExprSimpNode*> >::iterator ile = lst1.begin();

	for(;ile != lst1.end() ;ile++)
	{
		lst2.push_back(*ile);
	}
}

void get_nodes(Term* hs, list< string >& hnodes)
{
	if(!is_oper(hs->function_name))
	{
		hnodes.push_back(hs->function_name+"_"+hs->id_or_value);
		return;
	}

	list<argument *>::iterator ia = hs->args.begin();


	for(int i=0; i < hs->args.size(); i++)
	{
		if((*ia)->is_term)
		{
			//cout<<"again rule"<<endl;
			get_nodes((*ia)->var_or_term.r, hnodes);
		}
		else
		{
			string arg2 = (*ia)->var_or_term.v->name + "_" + (*ia)->var_or_term.v->id_or_value;

			hnodes.push_back(arg2);
		}

		ia++;
	}
}

void delete_nodes(list< string >& lhsnodes, list< string >& rhsnodes, list< string >& deletednodes)
{
	list< string >::iterator inode;
	for(inode = lhsnodes.begin(); inode != lhsnodes.end(); inode++)
	{
		list< string >::iterator inode2;
		for(inode2 = rhsnodes.begin(); inode2 != rhsnodes.end(); inode2++)
		{
			if((*inode) == (*inode2))
			{
				deletednodes.push_back(*inode);
			}
		}
	}
}

void fill_affected_area(Rule* mrule, struct CExprSimpNode* r, map <string, list<argument *> > &same_nodes)
{
// 	if(NodesDone.find(r) != NodesDone.end())
// 	{
// 		return;
// 	}

	list< string > lhsnodes;
	list< string > rhsnodes;
	list< string > deletednodes;

	map< string, CExprSimpNode* > allnodes;


	get_nodes(mrule->lhs, lhsnodes);
	get_nodes(mrule->rhs, rhsnodes);
	delete_nodes(lhsnodes, rhsnodes, deletednodes);

	list< pair<string, string> > lhsArgEdges;
	list< pair<CExprSimpNode*, CExprSimpNode*> > lhsEdges;
	list< pair<CExprSimpNode*, CExprSimpNode*> > lhsEdgesBackup;
	list< pair<string, string> > rhsArgEdges;

	get_lhs_edges(mrule->lhs, r,lhsArgEdges,lhsEdges, allnodes);

	//cout<<"after get_lhs_edges"<<endl;
	//cout<< "lhsArgEdges"<<lhsArgEdges.size()<<endl;
	//cout<< "lhsEdges"<<lhsEdges.size()<<endl;
	//cout<< "lhsEdges"<<rhsArgEdges.size()<<endl;


	get_rhs_edges(mrule->rhs,rhsArgEdges);
	//cout<<"after get_rhs_edges"<<endl;
	//cout<< "lhsArgEdges"<<lhsArgEdges.size()<<endl;
	//cout<< "lhsEdges"<<lhsEdges.size()<<endl;
	//cout<< "lhsEdges"<<rhsArgEdges.size()<<endl;

	copyEdges(lhsEdges,lhsEdgesBackup);

	delete_preserved_edges(lhsArgEdges,lhsEdges,rhsArgEdges);
	//cout<<"after delete_preserved_edges"<<endl;
	//cout<< "lhsArgEdges"<<lhsArgEdges.size()<<endl;
	//cout<< "lhsEdges"<<lhsEdges.size()<<endl;
	//cout<< "lhsEdges"<<rhsArgEdges.size()<<endl;

	list< pair<string, string> >::iterator ilae = lhsArgEdges.begin();
	list< pair<CExprSimpNode*, CExprSimpNode*> >::iterator ile = lhsEdges.begin();

	int size = lhsArgEdges.size();

	AffectedNodes* af = new AffectedNodes(mrule, r, lhsEdges, lhsEdgesBackup);

	if(check_if_present(size, af))
	{
		//cout<< "///////not added"<<endl;
		delete af;
		return;
	}

	AffectedArea[size].push_back(af);

	NodeAffectedNo[r].push_back(size);

	AllAffectedArea.push_back(af);

	NodeAff[r].insert(af);

	list< string >::iterator inode;
	int sum = r->out_list.size();
	for(inode = deletednodes.begin(); inode != deletednodes.end(); inode++)
	{
		struct CExprSimpNode* result = allnodes[*inode];
		if(result->in_list.size() == 1)
		{
			sum = sum + result->cost;
		}
	}

	r->cost = sum;

/*	if(!is_oper(mrule->rhs->function_name ))
	{
		
		struct CExprSimpNode* result = allnodes[mrule->rhs->function_name+"_"+mrule->rhs->id_or_value];
		if(result->in_list.size() == 1)
		{
			r->cost = lhsEdges.size();
		}
	}
	else
	{
		r->cost = lhsEdges.size();
	}*/
}

void add_labels_1(struct CExprSimpNode* r)
{
	list<struct CExprSimpNode*> ::iterator iter_ch = r->out_list.begin();

	list<string> labels_ch1 = (*iter_ch)->labels;
	list<string> ::iterator iter1 = labels_ch1.begin();

	for(iter1 = labels_ch1.begin(); iter1 != labels_ch1.end(); iter1++)
	{
		string temp = r->name;
		temp = temp + ",";
		temp = temp + (*iter1);

		if(is_valid_substring(temp))
		{
			(r->labels).push_back(temp);
		}

		if(RewriteRules.find(temp) != RewriteRules.end())
		{
			list<Rule*> t_rules = RewriteRules[temp];
			list<Rule*>::iterator il;

			for(il= t_rules.begin() ; il!= t_rules.end() ;il++)
			{
				//cout<<"*******checking*******"<<endl;

				map <string, list<argument *> >  same_nodes;
				map <argument *, struct CExprSimpNode*>  correspondance;
				get_same_nodes((*il)->lhs, r, same_nodes, correspondance);

				if(check_matching((*il)->lhs,r) && is_similar(same_nodes, correspondance))
				{
					addMatchedRule(*il, r);
					fill_affected_area(*il, r, same_nodes);


					NodesDone[r].insert(*il);
					//cout<<"*******added*******"<<endl;
				}
			}
		}
	}
}

void tokenize_string_into_set(string term, set<string> &tokens)
{
	int st = 0;
	int index = term.find(",",st);
	
	while(index != string::npos)
	{
		tokens.insert(term.substr(st, index - st));
		//cout<<term.substr(st, index - st)<<endl;
		st = index + 1;
		index = term.find(",",st);
	}
	
	//tokens.insert(term.substr(st));
	//cout<<term.substr(st)<<endl;
}

void show_correspondance(map<argument *, struct CExprSimpNode*> correspondance)
{
  cout<<"\n\nCORRESPONDENCE\n\n";
  
  map<argument *, struct CExprSimpNode*>::iterator it;

  for(it = correspondance.begin(); it != correspondance.end(); it++)
    {
      cout<<(it->first)<<"\t" <<(it->first)->get_string()<<"\t"<<it->second<<endl;
    }
 
}


struct CExprSimpNode* findMatchingNodeFromCorrespondance(map<argument *, struct CExprSimpNode*> correspondance, argument *arg)
{
  string arg_name = arg->get_string();

  map<argument *, struct CExprSimpNode*>::iterator it;

  for(it = correspondance.begin(); it != correspondance.end(); it++)
    {
       
      //cout<<(it->first)<<"\t" <<(it->first)->get_string()<<"\t"<<it->second<<endl;

      if((it->first)->get_string() == arg_name)
	return it->second;
     
    }
  return NULL;
}
 

vector<string> getArguments(map<argument *, struct CExprSimpNode*> correspondance, list<argument*> args)
{
  list<argument*>::iterator args_it;
  vector<string> ArgsVector;

  for(args_it = args.begin(); args_it != args.end(); args_it++)
    {
      argument *arg = (*args_it);
      string arg_str;
      if(arg->is_term)
	{
	  //cout<<"\nTerm\n"; 
	  Term *t = arg->var_or_term.r;
	  arg_str = evaluateGuardCondition(correspondance, t);
	}
      else
	{
	  //cout<<"\nNot a term\n"; 
	  //show_correspondance(correspondance);
	 //cout<<"\narg = "<<arg<<endl;  
	 //cout<<"\narg->get_string() = "<<arg->get_string()<<endl;  
	 //cout<<"\n(*args_it) = "<<(*args_it)<<endl;  
	 //cout<<"\n(*args_it)->get_string() = "<<(*args_it)->get_string()<<endl; 

	 struct CExprSimpNode* match = findMatchingNodeFromCorrespondance(correspondance, arg);
	 if(match == NULL)  // constant
	   {
	     //cout<<"\nConstant\n";
	     arg_str = ((arg->var_or_term).v)->name;
	     //cout<<"\nAdds "<<arg_str<<endl;
	   }
	 else
	   {
	     //cout<<"\nVariable\n";
	     arg_str = "";
	     match->getnodeVars(arg_str);
	     //cout<<"\nAdds "<<arg_str<<endl;
	   }
	}
      ArgsVector.push_back(arg_str);
    }
  return ArgsVector;
}
    
string makeVarString(string source)
{
  return source;
}

string intersectStringSets(string arg1, string arg2)
{
  set<string> arg1_set, arg2_set, intersect_set;
  string intersect_string;

  tokenize_string_into_set(arg1, arg1_set);
  tokenize_string_into_set(arg2, arg2_set);
  set_intersection(arg1_set.begin(), arg1_set.end(), arg2_set.begin(), arg2_set.end(),inserter(intersect_set, intersect_set.begin()));
  
  if(intersect_set.empty())
    return "empty";
  else
    {
      set<string>::iterator set_it;
      intersect_string = "";
      for(set_it = intersect_set.begin(); set_it != intersect_set.end(); set_it++)
	{
	  intersect_string += (*set_it) + ",";
	}
      return intersect_string;
    }
}


bool isHexConstant(char c)
{
  if('0' <= c && c <= '9')
    return true;
  if('a' <= c && c <= 'f')
    return true;
  if('A' <= c && c <= 'F')
    return true;
  return false;
}

bool isHexNumber(string s)
{
  if(s[0] == 'x' && s[1] == '"' && isHexConstant(s[2]))
    return true;
  return false;
}


bool isLiteral(string s)
{
  if(s[0] == '\'' && '0' <= s[1] && s[1] <= '9')
    return true;
  return false;
}

bool isConstant(string s)
{
  if(isHexNumber(s) || isLiteral(s)) return true;
  return false;
}


void convertToProperForm(string &s)
{
  if(s[1] =='1' && s[0] =='\'' && s[2] =='\'') 
    s="1"; 
  else if(s[1] =='0' && s[0] =='\'' && s[2] =='\'')  
    s="0"; 
  else if(s[0]=='x')
    {
      s.erase(s.begin(), s.begin()+2);
      s.erase(s.end()-1, s.end());
   }
  else
    {
      cout<<"\nUnknown constant "<<s<< "inside function convertToProperForm in rule_matching.cpp\n";
      exit(1);
    }
}

bool constantsAreEqual(string const1, string const2)
{
  convertToProperForm(const1);
  convertToProperForm(const2);

  //cout<<"\nconst1 = "<<const1<<"\tconst2 = "<<const2<<endl;

  if(const1==const2) return true;
  return false;
}
  
  

string evaluateGuardCondition(map<argument *, struct CExprSimpNode*> correspondance, Term* condition)
{
  vector<string> ArgsVector;

  if(condition->function_name == "List_Equal") 
    {
      ArgsVector = getArguments(correspondance, condition->args);
      //cout<<"\nArgsVector[0] = "<<ArgsVector[0]<<",ArgsVector[1] = "<<ArgsVector[1]<<endl;
      if(ArgsVector[0] == ArgsVector[1])
	{
	  //cout<<"\nEqual returns true\n";  
	return "true";
	}
      else 
	{
	  //cout<<"\nEqual returns false\n";  
	return "false";
	}
      }

  if(condition->function_name == "LOG_NOT") 
    {
     ArgsVector = getArguments(correspondance, condition->args);
     if(ArgsVector[0] == "true")
       {
	 //cout<<"\nEqual returns false\n";   
	return "false";
       }
     else if(ArgsVector[0] == "false")
       {
	 //cout<<"\nEqual returns true\n"; 
	return "true";
       }
     else if(ArgsVector[0] == "X")
       {
	 //cout<<"\nEqual returns X\n"; 
	return "X";
       }
     else
	{
	  cout<<"\nUnknown value "<<ArgsVector[0]<<"for  ArgsVector[0] inside getGuard. Exiting ...";
	  exit(1);
	  return "false";
	}
     } 

   if(condition->function_name == "List_Intersect")
     {
      ArgsVector = getArguments(correspondance, condition->args);
      //cout<<"\nList_Intersect returns "<< intersectStringSets(ArgsVector[0], ArgsVector[1]) <<endl;
      return intersectStringSets(ArgsVector[0], ArgsVector[1]);
    }
     
   if(condition->function_name == "List_mk")
     {
      ArgsVector = getArguments(correspondance, condition->args);
      //cout<<"\nList_mk returns "<< makeVarString(ArgsVector[0])<<endl;
      return makeVarString(ArgsVector[0]);
    }

   if(condition->function_name == "Log_Equal")
     {
      ArgsVector = getArguments(correspondance, condition->args);

      //cout<<"\nLog_Equal inside condition part :\n";
      //cout<<"\nArgsVector[0] = "<<ArgsVector[0]<<"\tArgsVector[1] = "<<ArgsVector[1]<<endl;
      
      int index;
      string Arg1Temp, Arg2Temp;
      string Arg1, Arg2;

      index=ArgsVector[0].find(",");
      Arg1Temp=ArgsVector[0].substr(index+1);
      Arg1=ArgsVector[0].substr(0, index);

      index=ArgsVector[1].find(",");
      Arg2Temp=ArgsVector[1].substr(index+1);
      Arg2=ArgsVector[1].substr(0, index);
      
      //cout<<"\nArg1Temp = "<<Arg1Temp<<"\tArg2Temp = "<<Arg2Temp<<endl;
      
      // Log_Equal currently makes sense only for hexadecimal constants. Else the result is X
      
      if( Arg1Temp!="" || Arg2Temp!="")
	{
	  //cout<<"\nX returned\n";
	  return "X";
	}
      
      // now we know that both ArgsVector[0], ArgsVector[1] are varaibles/constants
      // checking if they are variables

     
	
      //if(!(ArgsVector[0][0] == 'x' && ArgsVector[0][1] == '"' && isHexConstant(ArgsVector[0][2])) || !(ArgsVector[1][0] == 'x' && ArgsVector[1][1] == '"' && isHexConstant(ArgsVector[1][2]) ))

      /* Old check
      if(  (!isHexNumber(ArgsVector[0]) || !isHexNumber(ArgsVector[1])) &&
	   (!isLiteral(ArgsVector[0]) || !isLiteral(ArgsVector[1])) )
	{
	  //cout<<"\nX returned\n";
	  return "X";
	}
      
      // now we are sure that both are hex constants
      
      if(ArgsVector[0] == ArgsVector[1]) // Arguments are hex constants which are matching
	{
	  //cout<<"\ntrue returned\n";
	  return "true";
	  } */

      //cout<<"\nArg1 = "<<Arg1<<"\tArg2 = "<<Arg2<<endl;
      
      if(!isConstant(Arg1) || !isConstant(Arg2))
	{
	  //cout<<"\nX returned\n";
	  return "X";
	}

      if(constantsAreEqual(Arg1, Arg2))
	{
	  //cout<<"\ntrue returned\n";
	  return "true";
	} 


      else // Arguments are hex constants which are mis-matching
	{
	  //cout<<"\nfalse returned\n";
	  return "false";
	}
    }

   if(condition->function_name == "IsHexConst")
     {
      ArgsVector = getArguments(correspondance, condition->args);

      //cout<<"\nIsHexConst inside condition part :\n";
      //cout<<"\nArgsVector[0] = "<<ArgsVector[0]<<endl;
      
      int index;
      string Arg1Temp;

      index=ArgsVector[0].find(",");
      Arg1Temp=ArgsVector[0].substr(index+1);
     
      //cout<<"\nArg1Temp = "<<Arg1Temp<<endl;
      
      if( Arg1Temp!="")
	{
	  //cout<<"\nfalse returned\n";
	  return "false";
	}
      
      // now we know that both ArgsVector[0] is a varaible/constant
      // checking if it is a hex constant

      
      //if(ArgsVector[0][0] == 'x' && ArgsVector[0][1] == '"' && isHexConstant(ArgsVector[0][2]) )

      if(isHexNumber(ArgsVector[0]) || isLiteral(ArgsVector[0]))
	{
	  //cout<<"\ntrue returned\n";
	  return "true";
	}
      
      else 
	{
	  //cout<<"\nfalse returned\n";
	  return "false";
	}
    }


      
   cout<<"\nUnknown operator "<<condition->function_name<<" appears in condition part\n";
   exit(1);


}
      
      

bool getGuard(map<argument *, struct CExprSimpNode*> correspondance, Rule* il)
{
  if(RulesWithCondition.empty()) // No rule is there with condition

    return true;

  if(RulesWithCondition.find(il) == RulesWithCondition.end()) // il is not a rule with condition

    return true;

  else
    {
      Term* condition = RulesWithCondition[il];
      string BoolValue = evaluateGuardCondition(correspondance, condition);
      //cout<<"\nBoolValue = "<<BoolValue<<endl;
      if(BoolValue == "true")
	return true;
      else if(BoolValue == "false")
	return false;
      else if(BoolValue == "X")
	return false;
      else
	{
	  cout<<"\nUnknown value "<<BoolValue<<" for BoolValue inside getGuard. Exiting ...";
	  exit(1);
	  //return false;
	}
    }
}  

void add_labels_2(struct CExprSimpNode* r)
{
	list<struct CExprSimpNode*> ::iterator iter_ch = r->out_list.begin();

	list<string> labels_ch1 = (*iter_ch)->labels;
	list<string> ::iterator iter1 = labels_ch1.begin();

	iter_ch++;
	list<string> labels_ch2 = (*iter_ch)->labels;
	list<string> ::iterator iter2 = labels_ch2.begin();

	for(iter1 = labels_ch1.begin(); iter1 != labels_ch1.end(); iter1++)
	{
		for(iter2 = labels_ch2.begin(); iter2 != labels_ch2.end(); iter2++)
		{
			string temp = r->name;
			temp = temp + ",";
			temp = temp + (*iter1);
			temp = temp + ",";
			temp = temp + (*iter2);
			
			//cout<<"\ntemp inside add_labels_2 = "<<temp<<endl<<endl;

			if(is_valid_substring(temp))
			{
				(r->labels).push_back(temp);
			}

			if(RewriteRules.find(temp) != RewriteRules.end())
			{

			  //cout<<"\nInside if(RewriteRules.find(temp) != RewriteRules.end())"<<endl;

				list<Rule*> t_rules = RewriteRules[temp];
				list<Rule*>::iterator il;

				for(il= t_rules.begin() ; il!= t_rules.end() ;il++)
				{
				  //cout<<"*******checking*******"<<endl;

					map <string, list<argument *> >  same_nodes;
					map <argument *, struct CExprSimpNode*>  correspondance;
					get_same_nodes((*il)->lhs, r, same_nodes, correspondance);
					
					//show_correspondance(correspondance);
					bool Guard = getGuard(correspondance, *il);
					//if(Guard)
					//  cout<<"\nGuard = True\n";
					//else 
					//  cout<<"\nGuard = False\n";

					if(check_matching((*il)->lhs,r) && is_similar(same_nodes, correspondance) && Guard)
					{
						addMatchedRule(*il, r);
						fill_affected_area(*il, r, same_nodes);
						NodesDone[r].insert(*il);
						//cout<<"*******added*******"<<endl;
					}
				}
			}
		}
	}
}

void add_labels_3(struct CExprSimpNode* r)
{
	list<struct CExprSimpNode*> ::iterator iter_ch = r->out_list.begin();

	list<string> labels_ch1 = (*iter_ch)->labels;
	list<string> ::iterator iter1 = labels_ch1.begin();

	iter_ch++;
	list<string> labels_ch2 = (*iter_ch)->labels;
	list<string> ::iterator iter2 = labels_ch2.begin();

	iter_ch++;
	list<string> labels_ch3 = (*iter_ch)->labels;
	list<string> ::iterator iter3 = labels_ch3.begin();

	for(iter1 = labels_ch1.begin(); iter1 != labels_ch1.end(); iter1++)
	{
	  //cout<<"LOOP1"<<endl;

		for(iter2 = labels_ch2.begin(); iter2 != labels_ch2.end(); iter2++)
		{
		  //cout<<"LOOP2"<<endl;

			for(iter3 = labels_ch3.begin(); iter3 != labels_ch3.end(); iter3++)
			{
			  //cout<<"LOOP3"<<endl;

				string temp = r->name;
				temp = temp + ",";
				temp = temp + (*iter1);
				temp = temp + ",";
				temp = temp + (*iter2);
				temp = temp + ",";
				temp = temp + (*iter3);

			

				if(is_valid_substring(temp))
				{
				  //cout<<"::VALID::"<<endl;
					(r->labels).push_back(temp);
				}

				if(RewriteRules.find(temp) != RewriteRules.end())
				{
				  //cout<<"\nInside if(RewriteRules.find(temp) != RewriteRules.end())"<<endl;
					list<Rule*> t_rules = RewriteRules[temp];
					list<Rule*>::iterator il;

					for(il= t_rules.begin() ; il!= t_rules.end() ;il++)
					{
					  //cout<<"*******checking*******"<<endl;

						map <string, list<argument *> >  same_nodes;
						map <argument *, struct CExprSimpNode*>  correspondance;
						get_same_nodes((*il)->lhs, r, same_nodes, correspondance);

						if(check_matching((*il)->lhs,r) && is_similar(same_nodes, correspondance))
						{
							addMatchedRule(*il, r);
							fill_affected_area(*il, r, same_nodes);
							NodesDone[r].insert(*il);
							//cout<<"*******added*******"<<endl;
						}
					}
				}
			}
		}
	}
}

void calculate_cost(struct CExprSimpNode* r)
{

}

void process_node(struct CExprSimpNode* r)
{
  //cout<<"Name  = " << r->name<<endl;
	char* n = strdup((r->name).c_str());
	if(is_oper(r->name))
	{
		(r->labels).push_back("e");
		//cout<<"oper::"<<r->name<<endl;
		switch((r->out_list).size())
		{
			case 1:
			  //cout<<"args::"<<1<<endl;
				add_labels_1(r);
			break;

			case 2:
			  //cout<<"args::"<<2<<endl;
				add_labels_2(r);
			break;

			case 3:
			  //cout<<"args::"<<3<<endl;
				add_labels_3(r);
			break;
		}
	}
	else if('0' <= n[0] && n[0] <= '9')
	{
	  //cout<<"c"<<endl;
		(r->labels).push_back("c");
		r->cost = 0;
		
	}

	// NEWLY ADDED ON 27/01/2010 

	/*

	ADDING THIS MAY CAUSE A LOT OF CHANGES IN THE EXISTING RULES : HENCE LET US KEEP THEM AS EXPRESSIONS FOR THE TIME BEING

	else if(n[0] == 'x' && n[1] == '"' && '0' <= n[2] && n[2] <= '9') // Hex constant
	{
	  cout<<"c"<<endl;
		(r->labels).push_back("c");
		r->cost = 0;
		
	}

	else if(n[0] == '\'' && '0' <= n[1] && n[1] <= '9') // Literal constant
	{
	  cout<<"c"<<endl;
		(r->labels).push_back("c");
		r->cost = 0;
		
	}
	*/
	// NEWLY ADDED ON 27/01/2010  ENDS HERE

	else
	{
	  //cout<<"e"<<endl;
		(r->labels).push_back("e");
		r->cost = 0;
	}
}


void match_rule(struct CExprSimpNode* r)
{
// 	//cout<<endl<<"-----------------------------------------"<<endl;
// 	//cout<<"Label = " << r->label<<endl;
	if(r->labels.size() != 0)
	{
		return;
	}

	//	cout<<"Name  = " << r->name<<endl;

 	//cout<<"Children = ";
	list<struct CExprSimpNode*> ::iterator iter;

	for(iter = r->out_list.begin(); iter != r->out_list.end(); iter++)
	{
		match_rule(*iter);
		//cout<<"   "<<(*iter);
	}

	process_node(r);
}


// Clear the existing labels and recompute the labels of all nodes connected to the node r
void match_rule_after_clearing(struct CExprSimpNode* r)
{
// 	//cout<<endl<<"-----------------------------------------"<<endl;
// 	//cout<<"Label = " << r->label<<endl;
	if(r->labels.size() != 0)
	{
		r->labels.clear();
	}

	//cout<<"Name  = " << r->name<<endl;

// 	//cout<<"Children = ";
	list<struct CExprSimpNode*> ::iterator iter;

	for(iter = r->out_list.begin(); iter != r->out_list.end(); iter++)
	{
		match_rule_after_clearing(*iter);
// 		//cout<<"   "<<(*iter);
	}

	process_node(r);
}


void init_oper()
{
	operators.push_back("LOG_NOT");
	operators.push_back( "Bit_Neg");
	operators.push_back( "Red_And");
	operators.push_back( "Red_Or");
	operators.push_back( "Red_Xor");
	operators.push_back( "Sub");
	operators.push_back( "Log_Equal");
	operators.push_back( "CaseZ_Equal");
	operators.push_back( "Wild_Equal");
	operators.push_back( "Case_Equal");
	operators.push_back( "Log_Inequal");
	operators.push_back( "Grt_than");
	operators.push_back( "Grt_Equal");
	operators.push_back( "Less_than");
	operators.push_back( "Less_equal");
	operators.push_back( "Lft_Shift");
	operators.push_back( "Add");
	operators.push_back( "Log_And");
	operators.push_back( "Log_Or");
	operators.push_back( "Bit_And");
	operators.push_back( "Bit_Or");
	operators.push_back( "Bit_Xor");
	operators.push_back( "Bit_XNor");
	operators.push_back( "Ternary");
	operators.push_back( "Concat");
	operators.push_back( "Replic");
	operators.push_back( "Sel");
	operators.push_back( "PosEdge");
	operators.push_back( "NegEdge");
	operators.push_back( "AnyEdge");
	operators.push_back( "Event_Or");
	operators.push_back( "Edge_cond");
	operators.push_back( "LUB");
	operators.push_back( "LFP");
	operators.push_back( "Exists");
}


void rule_matching(struct CExprSimpNode* ExprSimpRoot)
{
  //cout<<endl<<"-----------------------------------------------------------------------";
  //cout<<endl<<"                           rule_matching";
  //cout<<endl<<"-----------------------------------------------------------------------"<<endl;
	init_oper();
	match_rule(ExprSimpRoot);
}


void print_labels(struct CExprSimpNode* r)
{
	list<struct CExprSimpNode*> ::iterator iter;

	for(iter = r->out_list.begin(); iter != r->out_list.end(); iter++)
	{
		print_labels(*iter);
	}

	cout<<endl<<"--------------------  labels for each node  ---------------------------"<<endl;
	cout<<"Names = " << r->name<<endl;
	cout<<"Labels::"<<endl;

	list<string> ::iterator iter1 ;

	for(iter1 = r->labels.begin(); iter1 != r->labels.end(); iter1++)
	{
		cout<<"  "<< (*iter1);
	}
}
