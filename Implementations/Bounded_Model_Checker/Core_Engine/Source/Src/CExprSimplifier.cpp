#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vector>
#include <map>
#include <string>
#include <utility>

#include <iostream>

#include <fstream>

#include <time.h>


#include "CExprSimpStructure.h"


using namespace std;

#define CLK_TCK 2992630000

#define HELP -1
#define FCFS 1
#define PRIO 2
#define AREA 3

#define PRIO_LEVEL 6

#define MAX_AFFECT_NODES 4


extern vector<string> ExprSimpGraph_nodes;
extern vector< pair<int,int> > ExprSimpEdges;
extern map<string, struct CExprSimpNode *> Subexpressions; // Is this needed?

extern map<Rule*, list<struct CExprSimpNode *> > MatchedRules;
extern map<int, list<Rule*> > MatchedRulesWithPriority;
extern map<int, list< AffectedNodes* > > AffectedArea;
extern map<struct CExprSimpNode *, set<Rule*> > NodesDone;


extern list< AffectedNodes* > AllAffectedArea;

extern map<string, bool> ValidSubstrings;

extern struct CExprSimpNode* ExprSimpRoot; // Is this needed?
extern int count1; // Is this needed?

extern map<string, list<Rule*> > RewriteRules;

bool is_oper(string expr);

/*****************************************************/
void init();
void CreateRules(string RuleFile);
void print_rules();


void rule_matching(struct CExprSimpNode* ExprSimpRoot);
void print_labels(struct CExprSimpNode* r);


void increamental_update(struct CExprSimpNode * r);
struct CExprSimpNode * take_action(Rule* pRule, list<struct CExprSimpNode *>& matched_nodes);
void match_rule_after_clearing(struct CExprSimpNode* r);
/*****************************************************/

//FILE* fpRulesApplied = fopen("RulesApplied.txt","w");


void print_map()
{
	//cout<<endl<<"-----------------------------------------------------------------------";
	//cout<<endl<<"                               MAP                                     ";
	//cout<<endl<<"-----------------------------------------------------------------------"<<endl;

	map<string,struct CExprSimpNode *>::iterator iter;
	int c = 0;

	for( iter = Subexpressions.begin(); iter != Subexpressions.end(); iter++)
	{
		//cout << "Expr: " << iter->first << "\tAddr: " << iter->second << endl;
		c++;
	}

	//cout<<endl<<"------------final size------------------------------------------------";
	//cout<<endl<<c<<endl;
	//cout<<endl<<"-----------------------------------------------------------------------";
}

map<struct CExprSimpNode*, bool> Graph;
int COUNT;



void create_node(fstream &file_op, struct CExprSimpNode* r)
{
// 	if(r->visited == true)
// 	{
// 		return;
// 	}

	if(Graph.find(r) != Graph.end())
	{
		
		return;
		
	}

	file_op<<"n"<<r<<"[label=\""<< r->name <<"\"];"<<endl;

	Graph.insert(make_pair(r,true));
	COUNT++;

// 	r->visited = true;

	list<struct CExprSimpNode*>::iterator iter ;

	for(iter=r->out_list.begin(); iter != r->out_list.end(); iter++)
	{
		file_op<<"n"<<r<<"->"<<"n"<<(*iter)<<";"<<endl;
	}

	for(iter=r->out_list.begin(); iter != r->out_list.end(); iter++)
	{
		create_node(file_op, *iter);
	}
}


void reset_flag(struct CExprSimpNode* r)
{
	r->visited = false;

	list<struct CExprSimpNode*>::iterator iter ;
	for(iter=r->out_list.begin(); iter != r->out_list.end(); iter++)
	{
		reset_flag(*iter);
	}
}

void create_entire_graph(string name)
{
	COUNT = 0;
	Graph.clear();
	fstream file_op(name.c_str(),ios::out);

	//cout<<"before"<<endl;

        file_op<<"digraph G {"<<endl;

	create_node(file_op, ExprSimpRoot);

	//cout<<"after"<<endl;

	file_op<<"}"<<endl;

	cout<<"NODES======"<<COUNT<<endl;

//	reset_flag(ExprSimpRoot);

	file_op.close();
}

void  print_nodes()
{
	//cout<<endl<<"-----------------------------------------------------------------------";
	//cout<<endl<<endl<<endl<<ExprSimpGraph_nodes.size()<<endl;

	for(int i=0; i<ExprSimpGraph_nodes.size(); i++)
	{
		//cout<<ExprSimpGraph_nodes[i]<<endl;
	}
}

void  print_edges()
{
	//cout<<ExprSimpEdges.size()<<endl;
	vector< pair<int,int> > ::iterator iedge;

	for(iedge=ExprSimpEdges.begin(); iedge != ExprSimpEdges.end(); iedge++)
	{
		//cout<< (*iedge).first << " " << (*iedge).second <<endl;
	}
}


void print_valid_strings()
{
	//cout<<"*** Valid Strings ***"<<endl;
	map<string, bool >::iterator imr = ValidSubstrings.begin();
	while(imr != ValidSubstrings.end())
	{
		//cout<<(*imr).first<<endl;
		imr++;
	}
}

void print_usage()
{
	//cout<<endl<<"--------------------------------------------------------------------"<<endl;
	//cout<<"Usage: ./exprsimp expressionfile [options]"<<endl;
	//cout<<endl;
	//cout<<"options"<<endl;
	//cout<<"   -h         Help"<<endl;

	//cout<<"   -f         First come first serve (Default)"<<endl;
	//cout<<"   -p         Priority based"<<endl;
	//cout<<"   -a         Area affected based"<<endl;
	//cout<<endl;
	//cout<<"NOTE: -f -p -a switches are mutually exclusive"<<endl;
	//cout<<"--------------------------------------------------------------------"<<endl;
	//cout<<endl;
}

int get_strategy(string arg)
{
	if(arg == "-h")
	{
		return HELP;
	}
	else if(arg == "-f")
	{
		return FCFS;
	}
	else if(arg == "-p")
	{
		return PRIO;
	}
	else if(arg == "-a")
	{
		return AREA;
	}
}

void print_macthed_rules()
{
	cout<<endl<<"********************************************************************"<<endl;

	cout<<endl<<"Total rules matched:"<<MatchedRules.size()<<endl;

	cout<<endl<<"********************************************************************"<<endl;

	map<struct CExprSimpNode *, set<Rule*> >::iterator nn =  NodesDone.begin();
	for(; nn !=  NodesDone.end() ; nn++)
	{
		if((*nn).second.size()<2)
		{
			continue;
		}
		cout<<(*nn).first<<" =>  ";
		set<Rule*>::iterator rr = (*nn).second.begin();


		for( ; rr != (*nn).second.end() ; rr++)
		{
			cout<<(*rr)->get_string()<<endl;
		}

		cout<<endl;
	}

	map<Rule*, list<struct CExprSimpNode *> >::iterator imr = MatchedRules.begin();

	while(imr != MatchedRules.end())
	{

		imr->first->print();
		
		// Newly added code 

		cout<<"\n\nRule is "<<imr->first->get_string()<<endl<<endl;

		// Newly added code ends

		cout<<endl<<"*****"<<endl;

		list<struct CExprSimpNode *>::iterator imn = imr->second.begin();
		
		// Newly added code 

		//if(imn == imr->second.end()) cout<<"\n\nThe second part is empty!!!\n\n";

		// Newly added code ends
		while(imn != imr->second.end())
		{
			cout<<*imn<<endl;
			cout<<"Content("<<*imn<<") = "<<(*imn)->getCExprSimpNodeStr()<<endl<<endl; // Newly added
			imn++;
		}

		cout<<endl<<"*****************************"<<endl;
		imr++;
	}
}

void strategy_fcfs()
{
	while(MatchedRules.size())
	{
		map<Rule*, list<struct CExprSimpNode *> >::iterator imr = MatchedRules.begin();

		if(imr->second.size() == 0)
		{
			MatchedRules.erase(imr);
			return;
		}

		
		//fprintf(fpRulesApplied,"%s\n",(imr->first->get_string()).c_str());

		//cout<<"\n\nRule applied is "<<imr->first->get_string()<<endl<<endl; // Newly added

		struct CExprSimpNode * changed = take_action(imr->first, imr->second);

		//cout<<"\n\nThe new node after application = "<<changed->getCExprSimpNodeStr()<<endl;

		//cout<<"\n\nAfter Applying this Rule, DAG becomes \n"; // Newly added

		//cout<<ExprSimpRoot->getCExprSimpNodeStr()<<"\n\n"; // Newly added

		//ExprSimpRoot->printDAGStructure(); // Newly added to check the DAG structure
		


		if(changed != NULL)
		{
		  //cout<<"result node ==>"<<changed<<"  "<<changed->name<<endl;
		  //cout<<"loop2"<<endl;
			changed->labels.clear();
			
			// Recomputing the labels of all nodes connected to the changed node : Newly added 
			for(list<struct CExprSimpNode*> ::iterator iter = changed->out_list.begin(); iter != changed->out_list.end(); iter++)
			  {
			    match_rule_after_clearing(*iter);
			    // 		//cout<<"   "<<(*iter);
			  }
			increamental_update(changed); 
		}
		else
		{
		  //cout<<"result node ==> NULL"<<endl;
		}


		if(imr->second.size() == 0)
		{
		  //cout<<"loop3"<<endl;
			MatchedRules.erase(imr);
		}

		//print_macthed_rules();
	}
}

pair<int, Rule*> getRuleByPriority()
{
	for(int i=1; i<PRIO_LEVEL; i++)
	{
		if(MatchedRulesWithPriority.find(i) != MatchedRulesWithPriority.end())
		{
			list<Rule*> lrules = MatchedRulesWithPriority[i];
			return make_pair(i, *(lrules.begin()));
		}
	}
}

void delete_rule_priority(int priority, Rule* rp)
{

	list<Rule*> &lrules = MatchedRulesWithPriority[priority];
	//cout<<"delete_rule_priority0   "<<lrules.size()<<endl;

	list<Rule*>::iterator ir = lrules.begin();

	for(; ir != lrules.end() ; ir++)
	{
		if(*ir == rp)
		{
			lrules.erase(ir);
			break;
		}
	}

//	lrules.remove(rp);
	//cout<<"delete_rule_priority1   "<<lrules.size() <<endl;
	if(lrules.size() == 0)
	{
		//cout<<"delete_rule_priority2"<<endl;
		MatchedRulesWithPriority.erase(priority);
	}
}

void print_macthed_rules_with_priority()
{
	cout<<endl<<"********************************************************************"<<endl;
	cout<<endl<<"Total rules matched:"<<MatchedRulesWithPriority.size()<<endl;
	cout<<endl<<"********************************************************************"<<endl;
	
	for(int i=0; i<PRIO_LEVEL; i++)
	{
		cout<<"priority:   "<<i<<endl;
		if(MatchedRulesWithPriority.find(i) != MatchedRulesWithPriority.end())
		{

			list<Rule*> lrules = MatchedRulesWithPriority[i];
			list<Rule*>::iterator ii =  lrules.begin();
			cout<<"size:   "<<lrules.size()<<endl;
			for(;  ii !=  lrules.end(); ii++)
			{
				(*ii)->print();
				cout<<endl<<"-------------------------------------------"<<endl;
			}
		}
	}
}

void strategy_prio()
{
	while(MatchedRulesWithPriority.size())
	{
		//cout<<"$$You are here000$$"<<endl;
		pair<int, Rule*> pair_r = getRuleByPriority();
		//cout<<"$$You are here111$$"<<endl;
		map<Rule*, list<struct CExprSimpNode *> >::iterator imr = MatchedRules.find(pair_r.second);
		//cout<<"$$You are here222$$"<<endl;

		if(imr == MatchedRules.end())
		{
			delete_rule_priority(pair_r.first, pair_r.second);
			//cout<<"$$You are here2$$"<<endl;
			print_macthed_rules_with_priority();
			continue;
		}

		if(imr->second.size() == 0)
		{
			delete_rule_priority(pair_r.first, pair_r.second);
			MatchedRules.erase(imr);
			//cout<<"$$You are here$$"<<endl;
			print_macthed_rules_with_priority();
			continue;
		}


		//cout<<"$$You are here333$$"<<endl;

		struct CExprSimpNode * changed = take_action(imr->first, imr->second);

		//cout<<"strategy_prio1"<<endl;

		if(changed != NULL)
		{
			//cout<<"result node ==>"<<changed<<"  "<<changed->name<<endl;
			//cout<<"strategy_prio2"<<endl;
			changed->labels.clear();

			// Recomputing the labels of all nodes connected to the changed node : Newly added 
			for(list<struct CExprSimpNode*> ::iterator iter = changed->out_list.begin(); iter != changed->out_list.end(); iter++)
			  {
			    match_rule_after_clearing(*iter);
			    // 		//cout<<"   "<<(*iter);
			  }
			increamental_update(changed);
			delete_rule_priority(pair_r.first, pair_r.second);
		}
		else
		{
			//cout<<"result node ==> NULL"<<endl;
		}

		if(imr->second.size() == 0)
		{
			//cout<<"strategy_prio3"<<endl;
			MatchedRules.erase(imr);
		}
		print_macthed_rules_with_priority();
	}
}

bool is_compitable(set < pair<CExprSimpNode*, CExprSimpNode*> > &setEdges, AffectedNodes* affnode)
{
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> > nodes = affnode->afEdges;
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> >::iterator inode = nodes.begin();
	for(; inode != nodes.end(); inode++)
	{
		if(setEdges.find(*inode) != setEdges.end())
		{
			//cout<<"return false"<<endl;
			return false;
		}
		//cout<<"return NO false"<<endl;
	}

	nodes = affnode->lhsEdges;
	inode = nodes.begin();
	for(; inode != nodes.end(); inode++)
	{
		if(setEdges.find(*inode) != setEdges.end())
		{
			//cout<<"return false"<<endl;
			return false;
		}
		//cout<<"return NO false"<<endl;
	}

	return true;
}

void add_to_set(set < pair<CExprSimpNode*, CExprSimpNode*> > &setEdges, AffectedNodes* affnode)
{
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> > &nodes = affnode->afEdges;
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> >::iterator inode = nodes.begin();
	for(; inode != nodes.end(); inode++)
	{
		setEdges.insert(setEdges.begin(), *inode);
	}

}

void form_graph();
extern vector<gr_node*> result;

int ind_set();

int lp_solve();

void get_non_interfearing(list <pair<Rule*, CExprSimpNode*> > &noninterfering)
{
	//cout<<"AffectedArea.size() = "<<AffectedArea.size()<<endl;

	form_graph();

	lp_solve();

	vector<gr_node*>::iterator ii = result.begin();

	for(; ii != result.end(); ii++)
	{
		AffectedNodes* af = (*ii)->af;
		noninterfering.push_back(make_pair(af->ra, af->root));
	}

}

/*void get_non_interfearing(list <pair<Rule*, CExprSimpNode*> > &noninterfering)
{
	//cout<<"AffectedArea.size() = "<<AffectedArea.size()<<endl;

	form_graph();

	ind_set();

	vector<gr_node*>::iterator ii = result.begin();

	for(; ii != result.end(); ii++)
	{
		AffectedNodes* af = (*ii)->af;
		noninterfering.push_back(make_pair(af->ra, af->root));
	}

}

void get_non_interfearing(list <pair<Rule*, CExprSimpNode*> > &noninterfering)
{
	set < pair<CExprSimpNode*, CExprSimpNode*> > setEdges;
	cout<<"AffectedArea.size() = "<<AffectedArea.size()<<endl;

	for(int i=MAX_AFFECT_NODES; i>=0; i--)
	{
		cout<<"i = "<<i<<endl;
		if(AffectedArea.find(i) != AffectedArea.end())
		{
			cout<<"i = "<<i<<endl;
			list< AffectedNodes* > &affs = AffectedArea[i];
			list< AffectedNodes* >::iterator iaff = affs.begin();

			for(; iaff != affs.end() ; iaff++)
			{
				cout<<"loop"<<endl;
				if(is_compitable(setEdges, *iaff))
				{
					cout<<"loop1"<<endl;
					add_to_set(setEdges, *iaff);
					noninterfering.push_back(make_pair((*iaff)->ra, (*iaff)->root));
				}
			}
		}
	}
}

*/

void print_affected_area()
{
   //cout<<"AffectedArea.size() "<<AffectedArea.size()<<endl;

	for(int i=MAX_AFFECT_NODES; i>=0; i--)
	{
		//cout<<"i = "<<i<<endl;
		if(AffectedArea.find(i) != AffectedArea.end())
		{
			list< AffectedNodes* > &affs = AffectedArea[i];
			list< AffectedNodes* >::iterator iaff = affs.begin();

			for(; iaff != affs.end() ; iaff++)
			{
				//cout<<"Rule:: "<<(*iaff)->ra->get_string()<<endl;
				//cout<<"node: "<<(*iaff)->root<<endl;
			}
		}
	}
}

void strategy_area()
{
	//cout<<"########## "<<endl;

	while(AllAffectedArea.size())
	{
		list <pair<Rule*, CExprSimpNode*> > noninterfering;
		//cout<<"noninterfering.size() = "<<noninterfering.size()<<endl;
		get_non_interfearing(noninterfering);

		AllAffectedArea.clear();
		//cout<<"noninterfering.size() = "<<noninterfering.size()<<endl;
		list< pair<Rule*, CExprSimpNode*> >::iterator iaff = noninterfering.begin();

		for(; iaff != noninterfering.end() ; iaff++)
		{
			list<struct CExprSimpNode *> node;
			node.push_back(iaff->second);
			struct CExprSimpNode * changed = take_action(iaff->first, node);

			if(changed != NULL)
			{
				//cout<<"result node ==>"<<changed<<"  "<<changed->name<<endl;
				//cout<<"loop2"<<endl;
				changed->labels.clear();
				
				// Recomputing the labels of all nodes connected to the changed node : Newly added 
			   for(list<struct CExprSimpNode*> ::iterator iter = changed->out_list.begin(); iter != changed->out_list.end(); iter++)
				  {
				    match_rule_after_clearing(*iter);
				    // 		//cout<<"   "<<(*iter);
				  }
				increamental_update(changed);
			}
			else
			{
				//cout<<"result node ==> NULL"<<endl;
			}
		}

		print_affected_area();
	}


}

void print_interfearing_area()
{
	list< AffectedNodes* >::iterator iter1 = AllAffectedArea.begin();

	//cout<<endl<<"AllAffectedArea size"<<AllAffectedArea.size()<<endl;
	
	for(;iter1 != AllAffectedArea.end(); iter1++)
	{
	        //cout<<"Rule1::"<< (*iter1)->ra->get_string()<<endl;
		list< AffectedNodes* >::iterator iter2 = AllAffectedArea.begin();
		for(;iter2 != AllAffectedArea.end(); iter2++)
		{
		        //cout<<"Rule2::"<< (*iter2)->ra->get_string()<<endl;
			if((*iter1)!= (*iter2) && (*iter1)->include(*iter2))
			{
			       //cout<<"***Conflict found***"<<endl;

			       //cout<<"between\n rule1::"<< (*iter1)->ra->get_string()<<endl;
								
			        //cout<<"at node "<<(*iter1)->root<<endl;
				//(*iter1)->root->print();

				//cout<<"and\n rule2"<<(*iter2)->ra->get_string()<<endl;
				
				//cout<<"at node "<<(*iter2)->root<<endl;
				//(*iter2)->root->print();
			}
		}
	
	}
	//cout<<endl<<"end of story"<<endl;
}
/*******************/
vector<gr_node*> ISNodes;
map< pair<int,int>, bool> ISEdges;

void form_graph()
{
	list< AffectedNodes* >::iterator iter1 = AllAffectedArea.begin();
	int i=0;
	
	for(;iter1 != AllAffectedArea.end(); iter1++)
	{
		
		gr_node* n = new gr_node(i,(*iter1)->root->cost, (*iter1));
		ISNodes.push_back(n);

		list< AffectedNodes* >::iterator iter2 = AllAffectedArea.begin();
		int j = 0;
		for(;iter2 != AllAffectedArea.end(); iter2++)
		{
			if((*iter1)!= (*iter2) && (*iter1)->include(*iter2))
			{
				//cout<<i << "--" <<j <<endl;
				ISEdges.insert(make_pair(make_pair(i, j), true) );
				ISEdges.insert(make_pair(make_pair(j, i), true) );
			}
			j++;
		}
		i++;
	}
}
/********************/
/*
int main(int argc, char** argv)
{
	char* infilename;
	char* outfilename;
	char* mapfilename;
	int i=2;

	clock_t start,end;
	double timeelapsed;

	
	start= clock();

	int strategy;

	if(argc < 2)
	{
		//cout<<endl<<"Too less arguments..."<<endl;
		print_usage();
		exit(2);
	}


	strategy = get_strategy(argv[1]);

	if(HELP == strategy)
	{
		print_usage();
		exit(0);
	}


	if (argc>1)
	{
		infilename=(char*)malloc(sizeof(char)*strlen(argv[1])+1);
		strcpy(infilename, argv[1]);
		if ((yyin=fopen(argv[1],"r"))==NULL)
		{
			yyerror("unable to open inputfile");
		}
	}
	else
	{
		infilename=" ";
	}

	if(argc == 2)
	{
		strategy = FCFS;
	}
	else
	{
		strategy = get_strategy(argv[2]);
	}

	if(HELP == strategy)
	{
		print_usage();
		exit(0);
	}

	yyparse();

	create_entire_graph("graph_before.dot");
	init();
	cout<<"\n\nInit Over\n\n";
	CreateRules();
	cout<<"\n\nCreateRules Over\n\n";
	print_rules();

	//print_valid_strings();

	rule_matching(ExprSimpRoot);
	//print_labels(ExprSimpRoot);

	cout<<endl<<"After rule_matching..."<<endl;

	switch(strategy)
	      {
		case FCFS:
			print_macthed_rules();
			strategy_fcfs();
		break;

		case PRIO:
			print_macthed_rules_with_priority();
			strategy_prio();
		break;

		case AREA:
			//cout<<endl<<"1"<<endl;
			print_interfearing_area();
			//cout<<endl<<"2"<<endl;
			print_affected_area();
			//cout<<endl<<"3"<<endl;
			strategy_area();
		break;
	      }
	    
	cout<<endl<<"After strategy..."<<endl;
	
	create_entire_graph("graph_after.dot");

	end = clock();
	timeelapsed = ((double)(end-start))/CLK_TCK;///CLOCKS_PER_SEC;
	cout<<"Timetaken::"<<timeelapsed<<endl;

	return 0;
}*/

// Internal data structures are cleared here before triggering the expression simplifier
void initializeDataStructures()
{
MatchedRules.clear();
MatchedRulesWithPriority.clear();
AffectedArea.clear();
NodesDone.clear();
AllAffectedArea.clear();
ValidSubstrings.clear();
RewriteRules.clear();
} 

void ExpressionSimplifier(string RuleFile)
{

       initializeDataStructures(); // newly added
       //cout<<"\ninitialization of datastructures over\n";

	int i=2;

	clock_t start,end;
	double timeelapsed;

	
	start= clock();

	int strategy = FCFS; // To be modified later

	//create_entire_graph("graph_before.dot");
	init();
	//cout<<"\n\nInit Over\n\n";
	CreateRules(RuleFile);
	//cout<<"\n\nCreateRules Over\n\n";
	//print_rules();

	//print_valid_strings();

	//print_macthed_rules(); //newly added

	rule_matching(ExprSimpRoot);
	//print_labels(ExprSimpRoot);

	//cout<<endl<<"After rule_matching..."<<endl;

	switch(strategy)
	      {
		case FCFS:
		  //print_macthed_rules();
			strategy_fcfs();
		break;

		case PRIO:
		       //print_macthed_rules_with_priority();
			strategy_prio();
		break;

		case AREA:
		        //cout<<endl<<"1"<<endl;
			print_interfearing_area();
			//cout<<endl<<"2"<<endl;
			print_affected_area();
			//cout<<endl<<"3"<<endl;
			strategy_area();
		break;
	      }
	    
	//cout<<endl<<"After strategy..."<<endl;
	
	//create_entire_graph("graph_after.dot");

	end = clock();
	timeelapsed = ((double)(end-start))/CLK_TCK;///CLOCKS_PER_SEC;
	//cout<<"Timetaken::"<<timeelapsed<<endl;

	return;
}

