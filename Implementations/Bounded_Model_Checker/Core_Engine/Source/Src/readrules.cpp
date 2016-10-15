#include<string>
#include<vector>
#include<list>
#include<map>
#include<fstream>

#include "CExprSimpStructure.h"

using namespace std;

class Rule;

map<string, list<Rule*> > RewriteRules;

map<string, int> functionArgs;

map<string, bool> ValidSubstrings;

map<Rule*, int> RulesWithPriority;

map<Rule*, Term*> RulesWithCondition;

vector<string> tokenize_term(string term)
{
	vector<string> tokens;
	int st = 0;
	int index = term.find(",",st);
	
	while(index != string::npos)
	{
		tokens.push_back(term.substr(st, index - st));
		//cout<<term.substr(st, index - st)<<endl;
		st = index + 1;
		index = term.find(",",st);
	}
	
	tokens.push_back(term.substr(st));
	//cout<<term.substr(st)<<endl;
	
	return tokens;
}

void partitionLine(string line, int &priority, string& lhs, string& rhs, string &condition)
{

	char *str;
	str = strdup(line.c_str());
	char delims[] = " \t";
	char *result = NULL;
	string arrow = "-->";

	result = strtok( str, delims );
	priority = atoi(result);
	//printf( "result is \"%s\"\n", result );

	result = strtok( NULL, delims );
	lhs = result;
	//printf( "result is \"%s\"\n", result );

	result = strtok( NULL, delims );
	//printf( "result is \"%s\"\n", result );

	result = strtok( NULL, delims );
	rhs = result;
	//printf( "result is \"%s\"\n", result );

	result = strtok( NULL, delims );

	if(result != NULL)
	  {
	    // printf( "result is \"%s\"\n", result );

	    result = strtok( NULL, delims );
	    condition = result;
	    //printf( "result is \"%s\"\n", result );
	  }
	else
	  {
	    condition = "";
	  }
}

vector<string> parseTerm(string var)
{
	vector<string> tok_var;
	int index = var.find("_", 0);

	tok_var.push_back(var.substr(0, index));
	tok_var.push_back(var.substr(index + 1));

	return tok_var;
}

bool isFunction(string name)
{
	if(functionArgs.find(name) != functionArgs.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Term * getTerm(vector<string> tokens, int start, int &end, string &str_l_rule)
{
	/*(eval, bit_and, x, y)*/
	string sub_str, id_or_value;
	string function_name = tokens[start];
	list<argument *> args;
	int no_args;
	int st_arg;

	////cout<<"getRule1"<<endl;

	if(functionArgs.find(function_name) != functionArgs.end())
	{
		////cout<<"getRule2"<<endl;
		no_args = functionArgs[function_name];
		id_or_value = "";

		st_arg = start+1;
	}
	else
	{
		//cout<<"getRule4"<<endl;
		vector<string> tok_var;
		tok_var = parseTerm(function_name);

		
		function_name = tok_var[0];
		id_or_value = tok_var[1];
		no_args = 0;
	}

	sub_str = function_name ;

	for(int i=1; i<= no_args; i++)
	{
		//cout<<"getRule5"<<endl;
		sub_str = sub_str + ",";
		int e;
		if(isFunction(tokens[st_arg]))
		{

			////cout<<"getRule6"<<endl;
			string s1;
			Term *ra = getTerm(tokens, st_arg, e, s1);
			struct argument* a = (struct argument*) malloc(sizeof(struct argument));
			a->is_term = true;
			a->var_or_term.r = ra;
			args.push_back(a);
			sub_str = sub_str + s1;
		}
		else
		{
			//cout<<"getRule7::"<<tokens[st_arg]<<endl;
			vector<string> tok_var;
			tok_var = parseTerm(tokens[st_arg]);
			Variable *va = new Variable(tok_var[0], tok_var[1]);
			////cout<<"getRule7.1"<<endl;
			struct argument* a = (struct argument*) malloc(sizeof(struct argument));
			a->is_term = false;
			a->var_or_term.v = va;
			////cout<<"getRule7.2"<<endl;

			e = st_arg + 1;
			args.push_back(a);
			sub_str = sub_str + tok_var[0];
			////cout<<"getRule8"<<endl;
		}

		st_arg = e;
	}

	Term *t = new Term(function_name, id_or_value, args);

	end = st_arg;

	ValidSubstrings.insert(make_pair(sub_str, true));

	str_l_rule = sub_str;

	return t;
}



Term * getCondition(vector<string> tokens, int start, int &end, string &str_l_rule)
{
	/*(eval, bit_and, x, y)*/
	string sub_str, id_or_value;
	string function_name = tokens[start];
	list<argument *> args;
	int no_args;
	int st_arg;

	////cout<<"getRule1"<<endl;

	if(functionArgs.find(function_name) != functionArgs.end())
	{
		////cout<<"getRule2"<<endl;
		no_args = functionArgs[function_name];
		id_or_value = "";

		st_arg = start+1;
	}
	else
	{
		//cout<<"getRule4"<<endl;
		vector<string> tok_var;
		tok_var = parseTerm(function_name);

		
		function_name = tok_var[0];
		id_or_value = tok_var[1];
		no_args = 0;
	}

	sub_str = function_name ;

	for(int i=1; i<= no_args; i++)
	{
		//cout<<"getRule5"<<endl;
		sub_str = sub_str + ",";
		int e;
		if(isFunction(tokens[st_arg]))
		{

			////cout<<"getRule6"<<endl;
			string s1;
			Term *ra = getCondition(tokens, st_arg, e, s1);
			struct argument* a = (struct argument*) malloc(sizeof(struct argument));
			a->is_term = true;
			a->var_or_term.r = ra;
			args.push_back(a);
			sub_str = sub_str + s1;
		}
		else
		{
			//cout<<"getRule7::"<<tokens[st_arg]<<endl;
			vector<string> tok_var;
			tok_var = parseTerm(tokens[st_arg]);
			Variable *va = new Variable(tok_var[0], tok_var[1]);
			////cout<<"getRule7.1"<<endl;
			struct argument* a = (struct argument*) malloc(sizeof(struct argument));
			a->is_term = false;
			a->var_or_term.v = va;
			////cout<<"getRule7.2"<<endl;

			e = st_arg + 1;
			args.push_back(a);
			sub_str = sub_str + tok_var[0];
			////cout<<"getRule8"<<endl;
		}

		st_arg = e;
	}

	Term *t = new Term(function_name, id_or_value, args);

	end = st_arg;

	str_l_rule = sub_str;

	return t;
}


void init()
{
	functionArgs.insert(make_pair("LOG_NOT", 1));
	functionArgs.insert(make_pair( "Bit_Neg", 1));
	functionArgs.insert(make_pair( "Red_And", 1));
	functionArgs.insert(make_pair( "Red_Or", 1));
	functionArgs.insert(make_pair( "Red_Xor", 1));
	functionArgs.insert(make_pair( "Sub", 2));
	functionArgs.insert(make_pair( "Log_Equal", 2));
	functionArgs.insert(make_pair( "CaseZ_Equal", 2));
	functionArgs.insert(make_pair( "Wild_Equal", 2));
	functionArgs.insert(make_pair( "Case_Equal", 2));
	functionArgs.insert(make_pair( "Log_Inequal", 2));
	functionArgs.insert(make_pair( "Grt_than", 2));
	functionArgs.insert(make_pair( "Grt_Equal", 2));
	functionArgs.insert(make_pair( "Less_than", 2));
	functionArgs.insert(make_pair( "Less_equal", 2));
	functionArgs.insert(make_pair( "Lft_Shift", 1));
	functionArgs.insert(make_pair( "Add", 2));
	functionArgs.insert(make_pair( "Log_And", 2));
	functionArgs.insert(make_pair( "Log_Or", 2));
	functionArgs.insert(make_pair( "Bit_And", 2));
	functionArgs.insert(make_pair( "Bit_Or", 2));
	functionArgs.insert(make_pair( "Bit_Xor", 2));
	functionArgs.insert(make_pair( "Bit_XNor", 2));
	functionArgs.insert(make_pair( "Ternary", 3));
	functionArgs.insert(make_pair( "Concat", 2));
	functionArgs.insert(make_pair( "Replic", 2));
	functionArgs.insert(make_pair( "Sel", 3)); 
	functionArgs.insert(make_pair( "PosEdge", 2));
	functionArgs.insert(make_pair( "NegEdge", 2));
	functionArgs.insert(make_pair( "AnyEdge", 2));
	functionArgs.insert(make_pair( "Event_Or", 2));
	functionArgs.insert(make_pair( "Edge_cond", 2));
	functionArgs.insert(make_pair( "LUB", 2));
	functionArgs.insert(make_pair( "LFP", 2));
	functionArgs.insert(make_pair( "Mul", 2));
	
	// New function added for "Exists"
	functionArgs.insert(make_pair( "Exists", 2));
	// New function added for conditional replacement
	functionArgs.insert(make_pair( "List_Equal", 2));
	functionArgs.insert(make_pair( "List_Intersect", 2));
	functionArgs.insert(make_pair( "List_mk", 1));
	functionArgs.insert(make_pair( "IsHexConst", 1));
}


void printRulesWithCondition()
{
	map<Rule*, Term*>::iterator ir;

	//cout <<"No of rules with condition ::"<< RulesWithCondition.size()<<endl;

	for(ir= RulesWithCondition.begin() ; ir!= RulesWithCondition.end() ;ir++)
	{
		Rule* t_rule = (*ir).first;
		Term* t_condition = (*ir).second;
		
		//cout<<"\nEntry\n";
		//cout<<"\nRule\n";
		//cout<<t_rule->get_string();
		//cout<<"\nCondition\n";
		//cout<<t_condition->get_string();
	}
}


void showFile(string RuleFile)
{
  cout<<"\nShowing "<<RuleFile<<endl;
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(RuleFile.c_str(), "r");
  if (fp == NULL)
    {
      cout<<"\nError while opening file "<<RuleFile.c_str()<<" for reading\n";
      return;
    }
  while ((read = getline(&line, &len, fp)) != -1) {
    printf("Retrieved line of length %zu :\n", read);
    printf("%s", line);
  }
  fclose(fp);
}


void CreateRules(string RuleFile)
{

  bool show_file = false;
  if(show_file)
    {
      showFile(RuleFile);// for debugging
    }



	fstream file_op(RuleFile.c_str(),ios::in);

	char str[2000];

	//cout<<"\nFile open\n";

	while(!file_op.eof())
	{
		file_op.getline(str,2000);

		string line = str;

		//cout<<"\nline obtained\n";

		if(line == "")
		{
		  //cout<<"\nNull line\n";
		  break;
		}

		//cout <<"doing "<< str <<endl;

		string lhs,rhs,condition;
		int priority;

		partitionLine(line, priority, lhs, rhs, condition);

		//cout<<"\nLHS = "<<lhs<<",RHS = "<<rhs<<"condition = "<<condition<<endl;
		
		vector<string> l_tokens = tokenize_term(lhs);
		
		vector<string> r_tokens = tokenize_term(rhs);
		
		int el, er;
		string str_l_rule;
		string str_r_rule;
		
		Term *t_lhs = getTerm(l_tokens, 0, el, str_l_rule);
		
		Term *t_rhs = getTerm(r_tokens, 0, er, str_r_rule);

		Rule *rc = new Rule(t_lhs, t_rhs);
		
		//cout <<"LHS STR::"<< str_l_rule<<endl;

		//cout <<"doing "<< rc->get_string() <<endl;

	//	ValidSubstrings.insert(make_pair(str_l_rule, true));
	
		if(RewriteRules.find(str_l_rule) == RewriteRules.end())
		{
		  //cout<<"\nRewriteRules.find(str_l_rule) == RewriteRules.end()\n";
			list<Rule*> t_rules;
			t_rules.push_back(rc);
			RewriteRules.insert(make_pair(str_l_rule, t_rules));
			
		}
		else
		  {
		    //cout<<"\nRewriteRules.find(str_l_rule) != RewriteRules.end()\n";
			RewriteRules[str_l_rule].push_back(rc);
			
		}


		RulesWithPriority.insert(make_pair(rc, priority));

		if(condition != "")
		  {
		    int ec;
		    
		    string str_c_rule;

		    vector<string> c_tokens = tokenize_term(condition);
		    
		    Term *t_cond = getCondition(c_tokens, 0, er, str_c_rule);
		    
		    //cout <<"\nCONDITION STR::"<< str_c_rule<<endl;

		    //cout <<"\nCONDITION IS"<<endl;

		    //cout<<t_cond->get_string()<<endl;

		    RulesWithCondition.insert(make_pair(rc, t_cond));
		  }
		    
	}

	printRulesWithCondition();

	file_op.close();
}

void print_rules()
{
	map<string, list<Rule*> >::iterator ir ;

	cout <<"No of rules::"<< RewriteRules.size()<<endl;

	for(ir= RewriteRules.begin() ; ir!= RewriteRules.end() ;ir++)
	{
		list<Rule*> t_rules = (*ir).second;
		list<Rule*>::iterator il;

		cout<<"***************************************"<<endl;
		cout <<"LHS STR::"<< (*ir).first<<endl;
		cout<<"***************************************"<<endl;


		for(il= t_rules.begin() ; il!= t_rules.end() ;il++)
		{
			cout<<"*******Rule*******"<<endl;

			(*il)->print();
		}
	}
}


