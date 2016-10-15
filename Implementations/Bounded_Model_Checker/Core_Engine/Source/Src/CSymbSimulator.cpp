#include "CSymbSimulator.h"
#include "y.tab.h"


//map <string, map<string, ConditionsValues> > SymbolicValues;


//CSymbSimulator::vector<CDependencyGraph*> AllDepenGraphs;

extern char* propertyfile;
extern CHelper MANAGER;
extern DDManager ddm;
extern DAGManager dm;
extern int no_of_simulation_cycles;
extern int debug_mode;
extern int qelimination_on;
extern int TimesOfUnrolling;
extern int Slicing;
extern time_t program_start_time;
extern string solver_for_bmc;
extern time_t ssimulation_time;
extern bool qelim_bit_level;
extern bool refinement_on; 
extern int no_of_cegar_iterations;
extern bool psl_property_on_cycles;
extern string quantifier_eliminator;
time_t total_qe_time_secs = 0;

bool real_cycle_present; // Is a real cycle present?
bool structural_cycle_present; // Is a structural cycle present?
string top_level_entity; // Stores the name of the top-level entity
//FILE* inline_fp = fopen("InliningResults.txt","w");
//FILE* fpDebugFile = fopen("DebugForExQuantifiedVars.txt","w");

int qualmode_call_number=0; // stores the number of calls to Qualmode from applyQuantifierElimination
extern int all_call_number; // stores the number of calls to all from applyQuantifierElimination

extern time_t yices_time_inside_cegar;
extern time_t unsat_core_time;


extern struct timeval program_start_time_ms;
extern unsigned long long int ssimulation_time_ms;
extern unsigned long long int yices_time_inside_cegar_ms;
extern unsigned long long int unsat_core_time_ms;
extern unsigned long long int unsat_core_time_total_ms;


map<string, list< map<string, string> > > UnsatCoreFindingHashTable;
extern int unsat_core_hash_hits;
extern int unsat_core_hash_misses; // number of hits and misses in the hash table for unsat core computation

extern unsigned long long int cvc_generation_from_se_time;
extern unsigned long long int total_cegar_time;
extern int cegar_iteration;
extern unsigned long long int total_avc_generation_time;
extern unsigned long long int total_expose_time;
extern unsigned long long int total_qe_time;
extern unsigned long long int simulation_ms;
extern int es_ht_hash_hits;
extern int es_ht_hash_misses; // number of hits and misses in the hash table for expression simplification
extern int qualmode_ht_hash_hits;
extern int qualmode_ht_hash_misses; // number of hits and misses in the hash table for qualmode
extern map<int, set<string> > RefinementDetails;

extern bool clock_based_simplification_enabled; // for clock based simplification
extern bool TRNeeedInFile; // for generation of transition relation in a file
extern bool exit_after_tr_generation; // exit after generation of TR

// For expression simplification

vector<string> ExprSimpGraph_nodes;
vector< pair<int,int> > ExprSimpEdges;
map<string, struct CExprSimpNode *> Subexpressions;
list<node_info *> exList;
struct CExprSimpNode* ExprSimpRoot;
int count1 = 0;
int count2 = 0;
void ExpressionSimplifier(string RuleFile);
void create_entire_graph(string name);
HashTable <string, CDAGNode*> ES_HT;// Expression simplifier's hash table
HashTable <string, CDAGNode*> Qualmode_HT;// Qualmode's hash table. Note that hashing is on callQualmode.

// Declarations from QE
extern FILE* LMIQEOutputDataFp;
extern FILE* LMIQEImportantDataFp;
extern FILE* LMIQEImportantDataFourierMotzkinFp;
extern FILE* LMIQEImportantDataAdditionalFp;
extern FILE* LMIQEFinishedLayer3DataFp;
extern FILE* LMIQEFinishedLayer3TimedOutDataFp;
extern FILE* LMIQEFinishedLayer3SCAchievedDataFp;
extern FILE* LMIQEFinishedLayer3NoSCDataFp;
extern FILE* LMIQEFinishedLayer3TrueDataFp;
extern FILE* LMIQEImportantDataOTFp;
extern FILE* hashingFp;
extern FILE* PathBasedStrategyFp;
extern FILE* MonEnabledAllVarsFp;
extern FILE* ConversionToDisjunctionOfProblemsFp;
extern FILE* ConversionToDisjunctionOfProblemsWithLMEsFp;
extern FILE* LMEBasedSimplificationAsPreprocessor_OutputFile;
extern FILE* SimplifyLMDD_OutputFile;
extern FILE* QELMDD_OutputFile;
extern FILE* ConversionToDisjunctionOfProblemsWithSMTSolverFp;
extern FILE* ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp;
extern FILE* LMIQEImportantDataOTAfterLayer2Fp;
extern FILE* TimeAnalysisFourierMotzkinFp;
extern FILE* TimeAnalysisFp;
extern FILE* TimeLayer2ChecksFp;
extern FILE* CumulativeTimeLayer2ChecksFp;
extern FILE* OutputDataLayer2ChecksFp;
extern FILE* log_file;
extern FILE* ImpCheckFile;
extern FILE* layer1_calls_fp;
extern FILE* layer2_calls_fp;
extern FILE* FILE_Blowup;
extern FILE* LMIQEImportantDataBlastingFp;
extern FILE* LMIQELayer1EffectivenessFp;

int qe_call_counter = 0;

void CSymbSimulator::execute()
{
         time_t t2;
	list<int> sortedIndices;

        //cout<<"\n\n**************************************\n\nCSymbSimulator::execute started\n\n******************************\n\n";

	topologicalSorting(sortedIndices);

	list<int>::iterator isi = sortedIndices.begin();

	top_level_entity = Entities[*isi].getName();

	sortedIndices.reverse();

	#ifdef DEBUG 
	cout<<"\n\nOrder of processing the entities\n\n";
	#endif

	isi = sortedIndices.begin();
        
                                                                                                                   
        for(; isi != sortedIndices.end(); isi++)
        {
                CEntity &ent = Entities[*isi];
                
		#ifdef DEBUG                                                                           
                cout<<ent.getName()<<endl;
		#endif
	}
                                                                                                                             
	

	isi = sortedIndices.begin();

	for(; isi != sortedIndices.end(); isi++)
	{
		CEntity &ent = Entities[*isi];

		CDependencyGraph*  dg= new CDependencyGraph();
		
		#ifdef DEBUG 
		cout<<"\n\n**************************************\n\nStarting Symbolic Simulation of Entity\n";

		cout<<ent.getName()<<endl;

		cout<<"\n\n*************************************\n\n";	
		#endif	
		

		dg->createDependencyGraph(ent);
		

//		AllDepenGraphs.push_back(dg);

//		dg->FindAtoms();

		//affectedBasicBlocks.clear();

		//cout<<"\n\nForming basic blocks\n";


		dg->formBasicBlocks();
		//dg->formBasicBlocks1();
		
		// At this point, the order of the nodes in BB is the order in which they appear in the dependency graph.
		// They should be ordered according to the order in which they will be triggered 

		//cout<<"\n\narrangeProcessInBBInTriggeredOrder\n";

		dg->arrangeProcessInBBInTriggeredOrder(ent.getName());
		
		#ifdef DEBUG
		dg->printBB();
		#endif

		//map<string, list<CSenNode*> > &affecting = dg->getAllSenList();
		
		map<string, list<CSenNode*> > &affecting = dg->getAffecting_And_Sensitive();

		map<string, list<CSenNode*> >::iterator affecting_it;

		/*cout<<"\nAffecting Map\n";

		for(affecting_it=affecting.begin(); affecting_it != affecting.end(); affecting_it++)
		  {
		    cout<<endl<<endl<<(*affecting_it).first<<endl;
		    list<CSenNode*>::iterator csennode_it = (*affecting_it).second.begin();
		    for(; csennode_it != (*affecting_it).second.end(); csennode_it++)
		      {
			cout<<endl<<"\tCSENNODE->NODE->NAME "<<(*csennode_it)->n->name<<" CSENNODE->CSENELMNT->NAME "<<(*csennode_it)->se->name<<endl;
		      }
		 }*/

		map<node *, CBasicBlock* > &nodePartOfBasicBlock = dg->GetNodePartOfBasicBlock();

		// Checking for cycles : Structural and Real - Kaustubh's algorithm

		check_for_cycles(ent);
		
		if(real_cycle_present)
		  {
		  #ifdef DEBUG
		  cout<<"\nReal cycle present in the entity "<<ent.getName()<<endl;
		  #endif
		  }

		else if(structural_cycle_present)
		  { 
	          #ifdef DEBUG
		  cout<<"\nStructural cycle present in the entity "<<ent.getName()<<". But no real cycle is present\n" ;
	          #endif
		  }			
		else
		  {
		  #ifdef DEBUG
		  cout<<"\nNeither real nor structural cycle present in the entity "<<ent.getName()<<endl ;
		  #endif
		  }
		  
		
		  

		//TODO

		
		//cout<<"\n\n**************************************\n\nCreating Table for Processes\n\n******************************\n\n";
		
		

		CreateTableForProcesses(ent);

		
		#ifdef DEBUG
		cout<<"\n\n**************************************\n\nPerforming Symbolic Simulation Of Basic Blocks\n\n******************************\n\n";
		#endif

		PerformSymbolicSimulationOfBasicBlocks(dg, ent, affecting, nodePartOfBasicBlock); 
	//cout<<"\n\n**************************************\n\nPrinting Tables for Basic Blocks\n\n******************************\n\n";


	        //printTableBasicBlocks(dg);

	
	//	affecting = dg->getAllSenList();
	//	nodePartOfBasicBlock = dg->GetNodePartOfBasicBlock();


//		createValuesAtZeroTime(ent, dg->getAtoms());
//		printSymbolicValues(ent, 0);

//		advanceDeltaTime(ent);


/*		if(/cycle is present in dg/ 1)
		{


//			createValuesAtOneTime(ent, affecting, nodePartOfBasicBlock);
			printSymbolicValues(ent, 1);

			advanceDeltaTime(ent);

			dg->createSensitivityList();

			PerformCyclicSymbolicSimulation(dg->getBasicBlocks(), ent);
		}
		else
		{
			map<node *, CBasicBlock* > &nodePartOfBasicBlock = dg->GetNodePartOfBasicBlock();

//			createValuesAtOneTime(ent, affecting, nodePartOfBasicBlock);
			printSymbolicValues(ent, 1);

			advanceDeltaTime(ent);

			list<node *> sortedNodes;
			dg->topologicalSorting(sortedNodes);

			//PerformSymbolicSimulation(sortedNodes, ent, 1, 1);
		}*/


		

	affectedBasicBlocksForOverall.clear();
		
        
	//cout<<"\n\n**************************************\n\nPerforming Overall Symbolic Simulation\n\n******************************\n\n";
	

	/*cout<<"\nAffecting Map\n";

	for(affecting_it=affecting.begin(); affecting_it != affecting.end(); affecting_it++)
	  {
		    cout<<endl<<endl<<(*affecting_it).first<<endl;
		    list<CSenNode*>::iterator csennode_it = (*affecting_it).second.begin();
		    for(; csennode_it != (*affecting_it).second.end(); csennode_it++)
		      {
			cout<<endl<<"\tCSENNODE->NODE->NAME "<<(*csennode_it)->n->name<<" CSENNODE->CSENELMNT->NAME "<<(*csennode_it)->se->name<<endl;
		      }
		      }*/


	PerformSymbolicSimulationOverall(dg, ent, affecting, nodePartOfBasicBlock);

        
	//cout<<"\n\n**************************************\n\nPrinting Overall Tables\n\n******************************\n\n";
	


	//if(ent.getName() == top_level_entity)
	//  {
	//    printTableOverall(ent);
	//  }

	 //removeTransientsFromTable(ent); // removes trasients from the table 

	#ifdef DEBUG
	printTableOverall(ent);
	#endif

	if(debug_mode==1)
		{
		cout<<"\nDebug Mode"<<debug_mode<<"\n";
		simplifyTable(ent); // Function to replace the table entry of each output of the entity by entity name + output name + delta time. This is useful for debugging
		printTableOverall(ent);
		}

	// call VC generator for the top level entity

	// FOLLOWING CALL TO VC Generation is commented on 21/1/2010 so that Qunatifier Eliminator can be tested
	
	
	
	if(true)
	  {

	    if(ent.getName() == top_level_entity)
			{

			  // Recording S.Simulation time

			  time(&t2);

			  #ifdef DEBUG
			  cout<<"\n\nS.S. TIME TIME  = "<< (t2 - program_start_time) <<" SECONDS "<<endl<<endl;
			  #endif			 

			  ssimulation_time = t2 - program_start_time;

			  struct timeval ssimulation_finish_time_ms;
			  gettimeofday (&ssimulation_finish_time_ms, NULL);
			  ssimulation_time_ms = ssimulation_finish_time_ms.tv_sec * 1000 + ssimulation_finish_time_ms.tv_usec / 1000;
			  ssimulation_time_ms -= program_start_time_ms.tv_sec * 1000 + program_start_time_ms.tv_usec / 1000;	




			  cout<<"\nSymbolic simulation successful";
			  // Recording S.Simulation time ends here

			  // Code for VC generation
			  cout<<"\nGenerating the verification condition...";
			  fflush(stdout);

			  if(Slicing==1) 
			    {
			      int ret = VCGenerator_With_Slicing(ent, sortedIndices);
// 			      cout<<"\nret = "<<ret<<endl;
// 			      cout<<"\nVCGenerator_With_Slicing finished\n";
// 			      cout<<"\nVCGenerator_With_Slicing finished\n";
// 			      cout<<"\nVCGenerator_With_Slicing finished\n";
// 			      cout<<"\nVCGenerator_With_Slicing finished\n";
// 			      cout<<"\nVCGenerator_With_Slicing finished\n";
			      //cout<<"\nVCGenerator_With_Slicing finished\n";
			    }
			  
			  else VCGenerator(ent);   

			  cout<<"\nVerification condition generation successful";

			  if(qelimination_on == 1)
			  {
			  	cout<<"\nQuantifier elimination details: algo="<< quantifier_eliminator <<", time=" << total_qe_time_secs<< " secs\n";
			  }
		          else
			  {
				cout<<endl;	
			  }

			  cout<<"\nVerification condition in verification_condition.smt\n\n";
			}
	  }
	
	#ifdef DEBUG
	cout<<"\nSymbolic simulation of entity finished\n";
        #endif
	}
}

/*
	void CSymbSimulator::string getExistentialExp()
	{
		string dag_str = name;

		cout<<"Inside getDAGStr()"<<endl<<endl;
		cout<<"dag_str="<<dag_str<<"  "<<"delta_time="<<delta_time<<endl;
		if(delta_time != -1)
		{
			cout<<endl<<"delta_time != -1 so entering 'if'"<<endl;
			char temp[10];
			sprintf(temp, "%d", delta_time);

			char *t = strdup(dag_str.c_str());

			dag_str += "_";

			dag_str += temp;

			cout<<"exiting getDAGStr() from inside 'if' with dag_str="<< dag_str <<endl<<endl;
			return dag_str;
		}
		cout<<endl<<"Outside 'if' as delta_time == -1"<<endl;

		dag_str += "(";

		list<CDAGNode*>::iterator iout = out_list.begin();

		for(; iout != out_list.end(); iout++)
		{
			cout<<"Making a call to getDAGStr() from 'for'"<<endl;
			dag_str += (*iout)->getDAGStr() + ",";
			cout<<endl<<"Inside 'for' in getDAGStr() with dag_str="<< dag_str <<endl;
		}

		dag_str += ")";
		cout<<"exiting getDAGStr() after 'for', dag_str="<< dag_str <<endl<<endl;
		return dag_str;
	}
*/
void CSymbSimulator::simplifyTable(CEntity& ent)
{
  //map< string, list<RelationInputOutput*> >& table = ent.getTable();
                                                                                                                             
  //map< string, list<RelationInputOutput*> >::iterator itab = table.begin();

   map< string, SymbolicRelation*>& table = ent.getTable();
                                                                                                                             
   map< string, SymbolicRelation*>::iterator itab = table.begin();
           
                                                                                                                             
	string entityname = ent.getName();  // Entity name obtained                                
                                                                                           
       for(;itab != table.end(); itab++)
      {
            string output_identifier = itab->first; // Output name obtained
            //list<RelationInputOutput*> & entries = itab->second; // List for the output obtained
	    SymbolicRelation* entries = itab->second; // SE Obtained
                                                                                                          
            
	    string name = entityname+"_"+output_identifier; 

            entries->relation = MANAGER.createDAG(name, 0); //new CDAGNode(name, 0);
	                
      }
}

void CSymbSimulator::adjustRefCount(list<int> &nodesWithZeroIndegree)
{
	list<int> ::iterator inode = nodesWithZeroIndegree.begin();
	for(;inode != nodesWithZeroIndegree.end();inode++)
	{
		Entities[*inode].setRefCount(-1);

		CEntity &ent = Entities[*inode];
		list<CInstance *> &instanceNodes = ent.getInstances();

		list<CInstance *>::iterator iNodes = instanceNodes.begin();

		for(	iNodes = instanceNodes.begin();
			iNodes != instanceNodes.end();
			iNodes++
			)
		{
			Entities[(*iNodes)->getIndex()].setRefCount(Entities[(*iNodes)->getIndex()].getRefCount() - 1);
 		}
	}
}

void CSymbSimulator::find_nodes_with_zero_indegree(list<int> &nodesWithZeroIndegree)
{
	for(int i=0; i<Entities.size();i++)
	{
		CEntity &ent = Entities[i];

		if(ent.getRefCount() == 0)
		{
		        //cout<<"\n"<<i<<"\t"<<ent.getName();
			nodesWithZeroIndegree.push_back(i);
		}
	}
}

void CSymbSimulator::setRefCounts()
{
	for(int i=0; i<Entities.size();i++)
	{
		CEntity &ent = Entities[i];
		list<CInstance *> &instanceNodes = ent.getInstances();

		list<CInstance *>::iterator iNodes = instanceNodes.begin();

		for(	iNodes = instanceNodes.begin();
			iNodes != instanceNodes.end();
			iNodes++
			)
		{
			Entities[(*iNodes)->getIndex()].setRefCount(Entities[(*iNodes)->getIndex()].getRefCount() + 1);
 		}
	}

}

void CSymbSimulator::topologicalSorting(list<int>& sortedIndices)
{
	setRefCounts();
	list<int>::iterator isi;

	int totalNodes = Entities.size();

	list<int> nodesWithZeroIndegree;

	int size = nodesWithZeroIndegree.size();
	nodesWithZeroIndegree.clear();

	while(totalNodes != sortedIndices.size())
	{
		//cout<<"\n\nPrinting Nodes with Zero Indegree\n";
		
		find_nodes_with_zero_indegree(nodesWithZeroIndegree);

		adjustRefCount(nodesWithZeroIndegree);
		
		isi = nodesWithZeroIndegree.begin();

       for(; isi != nodesWithZeroIndegree.end(); isi++)
        {
               sortedIndices.push_back(*isi);
         }
		
		//cout<<"\n\nPrinting Nodes with Zero Indegree\n";
		
		 //isi = nodesWithZeroIndegree.begin();

       //for(; isi != nodesWithZeroIndegree.end(); isi++)
        //{
         //       CEntity &ent = Entities[*isi];
                                                                                  
         //      cout<<ent.getName()<<endl;
       // }
        
        

		//sortedIndices.merge(nodesWithZeroIndegree);

		nodesWithZeroIndegree.clear();
		//cout<< "sortedNodes  "<<sortedIndices.size()<<endl;
		//cout<<"\n\nSorted Nodes\n\n";

	 //isi = sortedIndices.begin();

       //for(; isi != sortedIndices.end(); isi++)
        //{
        //        CEntity &ent = Entities[*isi];
                                                                                  
        //       cout<<ent.getName()<<endl;
       // }
	}

	//cout<<"\n\nOrder After Topological Sorting\n\n";

	 isi = sortedIndices.begin();

       for(; isi != sortedIndices.end(); isi++)
        {
                CEntity &ent = Entities[*isi];
                                                                             
               //cout<<ent.getName()<<endl;
        }

}

void CSymbSimulator::printTableOverall(CEntity& ent)
{
  //map< string, list<RelationInputOutput*> >& table = ent.getTable();

  //map< string, list<RelationInputOutput*> >::iterator itab = table.begin();

   map< string, SymbolicRelation*>& table = ent.getTable();

  map< string, SymbolicRelation*>::iterator itab = table.begin();

	
  cout<<"\n\nI AM HERE!!!"<<endl;
	cout<<"######Entity##########"<< ent.getName()<<"#########################################################################"<<endl;
	cout<<"**************************************"<<endl;
	for(;itab != table.end(); itab++)
	{
		cout<<"---------------------------"<<endl;
		string output_identifier = itab->first;
		//list<RelationInputOutput*> & entries = itab->second;
		SymbolicRelation* entries = itab->second;

		cout<< "ID::  " << output_identifier<<endl;
		cout<<"Relation:  "<< entries->relation->getDAGStr()<<endl;

		// NEW FUNCTION CALL ADDED TO SEE THE DAG STRUCTURE OF THE SE FOR EACH OUTPUT

		//MANAGER.printGraphInDotFormat(entries->relation, output_identifier+".dot" );

		  // NEW FUNCTION CALL ENDS

		map<string, bool> Graph;
		int count = 0;
		int excnt = 0;
		entries->relation->size(excnt, count);

		cout<< "Relation size:  " << count<<endl;
		cout<< "Number of exists:  " <<excnt <<endl;

		cout<<"Map:  "<<endl;
		map<CSensitiveElement*, string> delay_map = entries->delays;
		map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin();
		
		for(; delay_map_it != delay_map.end(); delay_map_it++)
			{
			  cout<< "Input: ";
			  (*delay_map_it).first->printCSensitiveElement();
			  cout<<"\t";
			  cout<< "Time:  " << (*delay_map_it).second<<endl;
			}
			

	}
     cout<<"########################################################################################################################\n"<<endl;
}




void CSymbSimulator::advanceDeltaTime(CEntity& ent)
{
/*	cout<<endl;

	map<string, ConditionsValues> & mapRV = SymbolicValues[ent.getName()];

	map<string, ConditionsValues>::iterator ien;
	for (ien = mapRV.begin(); ien != mapRV.end(); ++ien)
	{
		ConditionsValues &rv = ien->second;

		rv.values.push_back(rv.values[rv.values.size()-1]);
		rv.conditions.push_back(rv.conditions[rv.conditions.size()-1]);
	}*/
}

void CSymbSimulator::insertToAffectedBB(int delta, CBasicBlock* bb, CDAGNode* condition, set<string> &setCondition)
{
	//cout<<"insertToAffectedBB"<<endl;
	list<AffBlocksAndDelta*>::iterator result;
	bool found = false;

	for(result = affectedBasicBlocks.begin(); result != affectedBasicBlocks.end(); result++)
	{
		//cout<<"*-*-*-*-*"<<endl;
		//cout<< delta+1 << "  "<< (*result)->delta<<endl;
		//cout<< bb << "  "<< (*result)->bb<<endl;
		//cout<<"*-*-*-*-*"<<endl;

		if(delta+1 == (*result)->delta && bb == (*result)->bb)
		{
			if((*result)->condition->getDAGStr().find(condition->getDAGStr()) == string::npos)
			{
				(*result)->condition = MANAGER.createDAG("or", condition, (*result)->condition);
				(*result)->setCondition.insert(setCondition.begin(), setCondition.end());
			}
			found = true;
			break;
		}

		if(delta+1 < (*result)->delta)
		{
			break;

		}
	}

	if(!found)
	{
		//cout<<"*-------------*"<< endl;
		//cout<<"adding to AffBlocksAndDelta"<< endl;
		//cout<<condition->getDAGStr()<<endl;
		//cout<<delta+1<<endl;
		//cout<<bb<<endl;
		//cout<<"*-------------*"<<endl;

		AffBlocksAndDelta *abd = new AffBlocksAndDelta(delta+1, bb, condition, setCondition);
		affectedBasicBlocks.insert(result, abd);
	}
}

CSensitiveElement* CSymbSimulator::getSenEleforInput(set<CSensitiveElement*>& input_identifiers,
										string identifier)
{
        //cout<< "\n\ngetSenEleforInput Started\n\n";
	set<CSensitiveElement*>::iterator ise;
	for(ise = input_identifiers.begin(); ise != input_identifiers.end(); ise++)
	{
	  //cout<< "\n\nInside for of getSenEleforInput\n\n";
		if((*ise)->name == identifier)
		{       //cout<< "\n\nInside if of getSenEleforInput\n\n";
			return *ise;
		}
	}
	//cout<< "\n\ngetSenEleforInput Terminated\n\n";
	return NULL;
}


void CSymbSimulator::createNextSignal(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, string  identifier,
					map<string, list<CSenNode*> > &affecting,
					map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
	vector<CDAGNode*> values;
	vector<CDAGNode*> conditions;
	set<string> emptySetCondition;

	char *temp = strdup(identifier.c_str());
	

	SymbolicQueues[identifier].delta.push_back(1);
	SymbolicQueues[identifier].values.push_back(MANAGER.createDAG(identifier, 1));
	SymbolicQueues[identifier].conditions.push_back(MANAGER.createDAG("true"));
	SymbolicQueues[identifier].setConditions.push_back(emptySetCondition);
	
	int size = SymbolicQueues[identifier].ranges_covered.size();
	SymbolicQueues[identifier].ranges_covered.push_back(SymbolicQueues[identifier].ranges_covered[size-1]);

//	rv.values.push_back(values);
//	rv.conditions.push_back(conditions);

//	rv.values[1] = values;
//	rv.conditions[1] = conditions;

	//cout<<"Affected BB size = "<<affectedBasicBlocks.size()<<endl;


	if(affecting.find(identifier) != affecting.end())
	{

		list<CSenNode*> &sennodelist = affecting[identifier];

		//cout<<"ID: "<<identifier<<" size = "<<sennodelist.size()<<endl;
		list<CSenNode*>::iterator ien;
		for (ien = sennodelist.begin(); ien != sennodelist.end(); ++ien)
		{
			CBasicBlock* bb = nodePartOfBasicBlock[(*ien)->n];

			//cout<<"\nDisplaying Basic Block sensitive to "<<identifier<<endl;

			//bb->display();

			//cout<<"\nAffected BB size inside for = "<<affectedBasicBlocks.size()<<endl;

			CSensitiveElement* se = getSenEleforInput(bb->get_Input_identifiers(), identifier);  

			//if(se==NULL) cout<<"\n\nse is NULL\n\n";
			//else cout<<"\n\nse is non NULL\n\n";

			//cout<<"\nChecking "<<identifier<<" in input id list of Basic Block, and the Sen.Elmt. of identifier is : ";

			//cout<<se->name<<"("<<se->lower<<","<<se->upper<<")";

			//cout<<"\n\nIdentifier";

			//cout<<identifier<<endl<<endl;

			set<string> setCondition;

			CDAGNode* triggering_condition = calculateEventCondition(1, SymbolicQueues[identifier], se, setCondition);

			//cout<<"\nTriggering Condn : "<<triggering_condition->getDAGStr();

			insertToAffectedBB(1, bb, triggering_condition, setCondition);

			//displayAffectedBB();

			//cout<<affectedBasicBlocks.size()<<endl;
		}
	}
}


/*void CSymbSimulator::createValuesAtOneTime(CEntity& ent, map<string, list<CSenNode*> > &affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
	cout<<endl;

	map<string, SymTabEntry> & idMap = SymbolTable[ent.getName()];

	map<string, SymTabEntry>::iterator ien;
	for (ien = idMap.begin(); ien != idMap.end(); ++ien)
	{
		if(ien->second.id_type == "port::in" || ien->second.id_type == "port::inout" )
		{
			ConditionsValues &rv = SymbolicValues[ent.getName()][ien->first];

			createNextSignal(ien->first, rv, affecting, nodePartOfBasicBlock);
		}
	}
}*/

void CSymbSimulator::createValuesAtOneTime(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, set<CSensitiveElement*> &input_identifiers,
					map<string, list<CSenNode*> > &affecting,
					map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
	set <CSensitiveElement*> ::iterator ist;

	map<string, SymTabEntry> & idMap = SymbolTable[ent.getName()];

	for (ist=input_identifiers.begin(); ist != input_identifiers.end(); ++ist)
	{
/*		if(idMap[*ist].id_type == "port::in" || idMap[*ist].id_type == "port::inout" )
		{
			ConditionsValues &rv = SymbolicQueues[*ist];

			createNextSignal(SymbolicQueues, ent, *ist, affecting, nodePartOfBasicBlock);
		}
		else
		{
			ConditionsValues &rv = SymbolicQueues[*ist];

			rv.values.push_back(rv.values[rv.values.size()-1]);
			rv.conditions.push_back(rv.conditions[rv.conditions.size()-1]);
		}*/

		ConditionsValues &rv = SymbolicQueues[(*ist)->name];

		//cout<<"\n\nCreating one delta at one time "<<(*ist)->name<<endl<<endl;

		createNextSignal(SymbolicQueues, ent, (*ist)->name, affecting, nodePartOfBasicBlock);


/*		ConditionsValues rv;

		vector< pair<int, int> > atom_ranges;
		vector<CDAGNode*> values;
		vector<CDAGNode*> conditions;

		CDAGNode*v = MANAGER.createDAG(*ist);
		CDAGNode*c = MANAGER.createDAG("true");

		//cout<<v->getDAGStr()<<endl;
		//cout<<c->getDAGStr()<<endl;


//		rv.atom_ranges = atom_ranges;
		rv.values.push_back(v);
		rv.conditions.push_back(c);

		SymbolicQueues[*ist] = rv;*/
	}
}


void CSymbSimulator::formSymbolicStructureWithAtoms(string EntName, string id_name, list<int> &atoms)
{
/*	list<int>::iterator ib = atoms.begin();

	vector< pair<int, int> > atom_ranges;
	vector<CDAGNode*> values;
	vector<CDAGNode*> conditions;

	list<string>::iterator irange = SymbolTable[EntName][id_name].rangeindex.begin();

	int start = atoi((*irange).c_str());
	for(; ib != atoms.end(); ib++)
	{
		int end = *ib;
		atom_ranges.push_back(make_pair(start, end));
		values.push_back(MANAGER.createDAG("select", MANAGER.createDAG(id_name), start, end));
		conditions.push_back(MANAGER.createDAG("true"));
		start = end+1;
	}

	irange++;

	int upper = atoi((*irange).c_str());;
	atom_ranges.push_back(make_pair(start, upper));
	values.push_back(MANAGER.createDAG("select", MANAGER.createDAG(id_name), start, upper));
	conditions.push_back(MANAGER.createDAG("true"));

	ConditionsValues rv;
	rv.atom_ranges = atom_ranges;
	rv.values.push_back(values);
	rv.conditions.push_back(conditions);

	if(SymbolicValues.find(EntName) != SymbolicValues.end())
	{
		map<string, ConditionsValues> &id_symbValues = SymbolicValues[EntName];
		id_symbValues[id_name] = rv;
	}
	else
	{
		map<string, ConditionsValues> symbValues;
		symbValues[id_name] = rv;
		SymbolicValues[EntName] = symbValues;
	}	*/
}

void CSymbSimulator::printSymbolicValues(CEntity& ent, int current)
{
/*	map <string, map<string, ConditionsValues> >::iterator ist;

	for (ist=SymbolicValues.begin(); ist != SymbolicValues.end(); ++ist)
	{
		cout<< "*** "<<  ist->first  << " ***" <<endl;
		map<string, ConditionsValues>::iterator ien;
		for (ien = (ist->second).begin(); ien != (ist->second).end(); ++ien)
		{
			cout<< "-- "<<  ien->first  << " --" <<endl;

			ConditionsValues &rv = ien->second;

			int size = rv.atom_ranges.size();
			for(int i=0; i < size; i++)
			{
				cout<< "atom_range( "<< rv.atom_ranges[i].first<< " , "<< rv.atom_ranges[i].second  << ")"<<endl;
				cout<< "---------"<<endl;
				CDAGNode * temp = rv.values[current][i];
				cout<< rv.values[current][i]->getDAGStr()<<endl;

				temp = rv.conditions[current][i];
				cout<< rv.conditions[current][i]->getDAGStr() <<endl;
			}

			cout<< "**********************************************" <<endl;
			cout<<endl;
		}
	}	*/
}


void CSymbSimulator::createValuesAtZeroTime(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, set<CSensitiveElement*> &identifiers)
{
  //cout<<"\n\ncreateValuesAtZeroTime Started\n\n";
        
	set <CSensitiveElement*> ::iterator ist;

	for (ist=identifiers.begin(); ist != identifiers.end(); ++ist)
	{
		ConditionsValues rv;


		vector<CDAGNode*> values;
		vector<CDAGNode*> conditions;
		set<string> emptySetConditions;

		char *t = strdup((*ist)->name.c_str());

		//cout<< *ist <<endl;

		int lower = atoi((*ist)->lower.c_str());
		int upper = atoi((*ist)->upper.c_str());

		CDAGNode*v = MANAGER.createDAG((*ist)->name, 0);
		CDAGNode*c = MANAGER.createDAG("true");

		//cout<<v->getDAGStr()<<endl;
		//cout<<c->getDAGStr()<<endl;
		
		vector<pair<int,int> > ranges;
		ranges.push_back(make_pair(lower, upper));


		rv.delta.push_back(0);
		rv.values.push_back(v);
		rv.conditions.push_back(c);
		rv.setConditions.push_back(emptySetConditions);
		rv.ranges_covered.push_back(ranges);
		

		SymbolicQueues.insert(make_pair((*ist)->name, rv));
	}

//cout<<"\n\ncreateValuesAtZeroTime Finished\n\n";
}

void CSymbSimulator::printTableBasicBlocks(CDependencyGraph* dg)
{
	list<CBasicBlock *> basicblocks = dg->getBasicBlocks();

	list<CBasicBlock *> ::iterator ibb = basicblocks.begin();

	map<string, bool> previously_changed;
	int i = 1;
	int bbcount=1;
	
	cout<<"\n\nI AM HERE!!!"<<endl;
	cout<<"############################################################################################################"<<endl;
	for(;ibb != basicblocks.end(); ibb++)
	{
	  //map< string, list<RelationInputOutput*> >& table = (*ibb)->getTable();

	  //map< string, list<RelationInputOutput*> >::iterator itab = table.begin();

	  map< string, SymbolicRelation*>& table = (*ibb)->getTable();
	  map< string, SymbolicRelation*>::iterator itab = table.begin();

		cout<<"***************************************************"<<endl;
		cout<<"Table for Basic Block"<<bbcount<<endl;
		cout<<"****************************************************"<<endl; 
		
		for(;itab != table.end(); itab++)
		{
			cout<<"---------------------------"<<endl;
			string output_identifier = itab->first;
			//list<RelationInputOutput*> & entries = itab->second;
			SymbolicRelation* entries = itab->second;
			cout<< "ID::  " << output_identifier<<endl;
			cout<<"Relation:  "<< entries->relation->getDAGStr()<<endl;
			cout<<"Map:  "<<endl;
			map<CSensitiveElement*, string> delay_map = entries->delays;
			map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin();

			/*for(list<RelationInputOutput*>::iterator irio = entries.begin();
				irio != entries.end();
				irio++)
			{
				RelationInputOutput* rio = *irio;
				cout<< "Time:  " << rio->delta_time<<endl;
				cout<< "Condition: "<< rio->condition->getDAGStr()<<endl;
				cout<<"Relation:  "<< rio->relation->getDAGStr()<<endl;
				map<string, bool> Graph;
				int count = 0;
				//rio->relation->size(count, Graph);
				int excnt = 0;
				rio->relation->size(excnt, count, Graph);
				cout<< "Relation size:  " << count <<endl;
				cout<< "Number of exists:   "<<excnt <<endl;
				}*/

			for(; delay_map_it != delay_map.end(); delay_map_it++)
			{
			  cout<< "Input: ";
			  (*delay_map_it).first->printCSensitiveElement();
			  cout<<"\t";
			  cout<< "Time:  " << (*delay_map_it).second<<endl;
			}
			

			//cout<<"- - - - - - - - - - - - - - - - - "<<endl;

			//deltas_new.push_back((*irio)->delta_time + current);
		}
		bbcount = bbcount+1;
	}
	cout<<"#############################################################################################################\n"<<endl;
}


void CSymbSimulator::PerformSymbolicSimulationOfBasicBlocks(CDependencyGraph* dg, CEntity& ent,
					map<string, list<CSenNode*> > &affecting,
					map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
	list<CBasicBlock *> basicblocks = dg->getBasicBlocks();

	list<CBasicBlock *> ::iterator ibb = basicblocks.begin();

	map<string, bool> previously_changed;
	int i = 1;
	int bb_count=0;

	for(;ibb != basicblocks.end(); ibb++) {bb_count++;}

	#ifdef DEBUG
	cout<<"\nNo:of basic blocks = "<<bb_count;
	#endif

	//cout<<"\nInside PerformSymbolicSimu of Basic Blocks\n\n";

	ibb = basicblocks.begin();

	bb_count=1;

	for(;ibb != basicblocks.end(); ibb++)
	{
	       //cout<<"\nInside loop of PerformSymbolicSimu of Basic Block"<<bb_count<<"\n\n";

		map<string, ConditionsValues> SymbolicQueues;

		createValuesAtZeroTime(SymbolicQueues, ent, (*ibb)->get_identifiers());
		//createValuesAtZeroTime(SymbolicQueues, ent, (*ibb)->output_identifiers);

		createValuesAtOneTime(SymbolicQueues, ent, (*ibb)->get_Input_identifiers(),
					affecting, nodePartOfBasicBlock);

		PerformSymbolicSimulation(*ibb, ent, SymbolicQueues);
		
		map<string, FinalRelationWithDelta*> final_expressions;

		calculateExpressionsForBB(*ibb, SymbolicQueues, final_expressions, ent);

		#ifdef DEBUG
		printSymbolicQueues(SymbolicQueues); 
		#endif

		createTableForBB(*ibb, SymbolicQueues, final_expressions, affecting, nodePartOfBasicBlock);
		
		bb_count ++;
	}
	}

	/*void CSymbSimulator::calculateExpressionsForBB(CBasicBlock *bb, map<string, ConditionsValues>& SymbolicQueues, 
									map<string, FinalRelation*> & final_expressions, CEntity &ent)
	{
		map<string, list<ConditionRelation*> >::iterator imsd = bb->mapSignalDelta.begin();
		
		for(;imsd != bb->mapSignalDelta.end();imsd++)
		{
			string id_delta = imsd->first;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
			
			
			int current = atoi(id_delta.substr(index+1).c_str());
			int previous;
					
			ConditionsValues &cv = SymbolicQueues[id];
			int size = cv.delta.size();

			for(int i=0; i < size; i++)
			{
				if(cv.delta[i] == current)
				{
					previous = cv.delta[i-1];
					break;
				}
			}
			
			list<ConditionRelation*> &listCR = imsd->second;
			
			vector<pair<int, int> >rel_ranges;
			
			CDAGNode *final_dag = NULL;
			set<string> setCondRel;
			
			if(listCR.size() == 1)
			{
				list<ConditionRelation*>::iterator icr = listCR.begin();
				
				pair<int, int> range = (*icr)->relation->getLHSRange();
				rel_ranges.push_back(range);
				
				final_dag = (*icr)->relation;
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
			}
			else
			{
				list<ConditionRelation*>::iterator icr = listCR.begin();
				
				pair<int, int> range = (*icr)->relation->getLHSRange();
				
				CDAGNode * dag2 = (*icr)->relation;
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
				
				rel_ranges.push_back(range);
				icr++;
				
				for(;icr != listCR.end(); icr++)
				{
					pair<int, int> range = (*icr)->relation->getLHSRange();
					
					CDAGNode * dag3 = (*icr)->relation;
					setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
		
					dag2 = MANAGER.createDAG("and", dag2, dag3);
					
					rel_ranges.push_back(range);
				}
				
				final_dag = dag2;
			}
			
			vector<pair<int, int> >remain_ranges;
			
			SymTabEntry &ste = SymbolTable[ent.getName()][id];
			
			list<string>::iterator irange;
			irange = ste.rangeindex.begin();
			
			int start = atoi((*irange).c_str());
			
			irange++;
			
			int last = atoi((*irange).c_str());
				
			getRemainingRanges(rel_ranges, remain_ranges, last);
			
			CDAGNode * current_dag = MANAGER.createDAG(id, current);
			CDAGNode * previous_dag = MANAGER.createDAG(id, previous);
			
			setCondRel.insert(previous_dag->getDAGStr());
			
			for(int i=0; i < remain_ranges.size(); i++)
			{
				pair<int, int> range = remain_ranges[i];
				
				CDAGNode * lhs = MANAGER.createDAG("select", current_dag, range.first, range.second);
				CDAGNode * rhs = MANAGER.createDAG("select", previous_dag, range.first, range.second);
				
				CDAGNode * dag1 = MANAGER.createDAG("equivalent", lhs, rhs);
				
				final_dag = MANAGER.createDAG("and", final_dag, dag1);
			}
			
			//cout<<final_dag->getDAGStr()<<endl;	
			
			FinalRelation* fr = new FinalRelation(final_dag, setCondRel);
			
			final_expressions[id_delta] = fr;
		}
	}*/
	void CSymbSimulator::calculateExpressionsForBB(CBasicBlock *bb, map<string, ConditionsValues>& SymbolicQueues, 
									map<string, FinalRelationWithDelta*> & final_expressions, CEntity &ent)
	{

	  //cout<<"\n\ncalculateExpressionsForBB STARTED\n\n";

	  //cout<<"\n\nmapSignalDelta IS \n\n";

		map<string, list<ConditionRelation*> >::iterator imsd;

		/*imsd = bb->mapSignalDelta.begin();

		for(;imsd != bb->mapSignalDelta.end();imsd++)
		{
		    string id = imsd->first;

		    cout<<"\n\nID: "<<id;

		    list<ConditionRelation*> &listCR = imsd->second;

		    list<ConditionRelation*>::iterator listCR_it = listCR.begin();

		    for(;listCR_it != listCR.end(); listCR_it++)
		      {
			ConditionRelation* cr = (*listCR_it);

			cout<<"\nCondition : "<<cr->condition->getDAGStr();
			cout<<"\nRelation : "<<cr->relation->getDAGStr();
		      }
		 }*/

		
		imsd = bb->mapSignalDelta.begin();
		
		for(;imsd != bb->mapSignalDelta.end();imsd++)
		{
		        //string id_delta = imsd->first;
			//int index = id_delta.find_last_of("_");
			//string id = id_delta.substr(0, index);
			
			
			//int current = atoi(id_delta.substr(index+1).c_str());
			//int previous;
					
			/*ConditionsValues &cv = SymbolicQueues[id];
			int size = cv.delta.size();

			for(int i=0; i < size; i++)
			{
				if(cv.delta[i] == current)
				{
					previous = cv.delta[i-1];
					break;
				}
				}*/

		        string id_delta = imsd->first;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);

		        //string id = imsd->first;
			
			list<ConditionRelation*> &listCR = imsd->second;
			
			vector<pair<int, int> >rel_ranges;
			
			CDAGNode *final_dag = NULL;
			set<string> setCondRel;
			
			if(listCR.size() == 1)
			{
				list<ConditionRelation*>::iterator icr = listCR.begin();
				
				//pair<int, int> range = (*icr)->ranges_covered;
				rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end());
				
				final_dag = (*icr)->relation;
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
			}
			else
			{

			  // THIS PART MAY BE CHANGED AFTER WARDS - THIS IS THE PLACE WHERE WE ARE FINDINING THAT THERE IS (C1, R1), (C2, R2) FOR THE SAME DELTA. HERE WE ARE ALWAYS CHOOSING (C1, R1), AS I FEEL THAT BOTH (C1, R1) AND (C2, R2) SHOULD BE THE SAME

			  list<ConditionRelation*>::iterator icr = listCR.begin();
				
			  //pair<int, int> range = (*icr)->ranges_covered;
			  rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end());
				
			  final_dag = (*icr)->relation;
			  setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
			     

			  /*list<ConditionRelation*>::iterator icr = listCR.begin();
				
				pair<int, int> range = (*icr)->relation->getLHSRange();
				
				CDAGNode * dag2 = (*icr)->relation;
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
				
				//rel_ranges.push_back(range);
				rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end());
				
				icr++;
				
				for(;icr != listCR.end(); icr++)
				{
					pair<int, int> range = (*icr)->relation->getLHSRange();
					
					CDAGNode * dag3 = (*icr)->relation;
					setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
		
					dag2 = MANAGER.createDAG("and", dag2, dag3);
					
					//rel_ranges.push_back(range);
					rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end());
					
				}
				
				final_dag = dag2; */
			}

			// Now we have in final_dag the SE of the intermediate variable/output at a delta time equal to  delta time of the last queue entry for the intermediate variable/output say 'n'. We need to shift it by 'n'

			// Finding n

			ConditionsValues &cv = SymbolicQueues[id]; // SQ for id
			
			int max_del_index = cv.delta.size()-1;

			int delta_time = cv.delta[max_del_index];

			setCondRel.clear();

			final_dag->getDAGNames(setCondRel);
			
			FinalRelationWithDelta* fr = new FinalRelationWithDelta(delta_time, final_dag, setCondRel, rel_ranges);

			//cout<<"\n\nSYMBOLIC EXPRESSIONS FOR "<<id<<" : "<<final_dag->getDAGStr();

			final_expressions[id] = fr;
			
		}
	}
	/*
	void CSymbSimulator::calculateExpressionsForOverall(map<string, ConditionsValues>& SymbolicQueues, 
												map<string, list<ConditionRelation*> >& mapSignalDelta,
												map<string, FinalRelation*> & final_expressions, CEntity& ent)
	{
		map<string, list<ConditionRelation*> >::iterator imsd = mapSignalDelta.begin();
		
		for(;imsd != mapSignalDelta.end();imsd++)
		{
			string id_delta = imsd->first;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
			
			int current = atoi(id_delta.substr(index+1).c_str());
			int previous, current_index;
					
			ConditionsValues &cv = SymbolicQueues[id];
			int size = cv.delta.size();

			for(int i=0; i < size; i++)
			{
				if(cv.delta[i] == current)
				{
					current_index = i;
					previous = cv.delta[i-1];
					break;
				}
			}
			
			CDAGNode *final_dag = NULL;
			set<string> setCondRel;
			
			list<ConditionRelation*> &listCR = imsd->second;
			
	//		vector<pair<int, int> >rel_ranges;
			
			if(listCR.size() == 1)
			{
				list<ConditionRelation*>::iterator icr = listCR.begin();
				
	//			pair<int, int> range = (*icr)->relation->getLHSRange();
	//			rel_ranges.push_back(range);
				
				CDAGNode * dag1 = MANAGER.createDAG("equivalent", cv.values[current_index], cv.values[current_index-1]);
				final_dag = MANAGER.createDAG("ite", cv.conditions[current_index], (*icr)->relation, dag1);
				setCondRel.insert(cv.setConditions[current_index].begin(), cv.setConditions[current_index].end());
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
			}
			else
			{
				list<ConditionRelation*>::iterator icr = listCR.begin();
				
				//pair<int, int> range = (*icr)->relation->getLHSRange();
				
				// CDAGNode * lhs = MANAGER.createDAG("select", cv.values[current_index], range.first, range.second);
				// CDAGNode * rhs = MANAGER.createDAG("select", , range.first, range.second);
				
				CDAGNode * dag1 = MANAGER.createDAG("equivalent", cv.values[current_index], cv.values[current_index-1]);
				CDAGNode * dag2 = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);
				setCondRel.insert((*icr)->setCondition.begin(), (*icr)->setCondition.end());
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
				
				//rel_ranges.push_back(range);
				icr++;
				
				for(;icr != listCR.end(); icr++)
				{
					CDAGNode * dag3 = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag2);
					setCondRel.insert((*icr)->setCondition.begin(), (*icr)->setCondition.end());
					setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());

					dag2 = dag3;
					//rel_ranges.push_back(range);
				}
				
				final_dag = dag2;
			}
			
			//cout<<final_dag->getDAGStr()<<endl;
			
			FinalRelation* fr = new FinalRelation(final_dag, setCondRel);
			
			final_expressions[id_delta] = fr;
		}
	}*/

	void CSymbSimulator::calculateExpressionsForOverall(map<string, ConditionsValues>& SymbolicQueues, map<string, list<ConditionRelation*> >& mapSignalDelta, map<string, FinalRelationWithDelta*> & final_expressions, CEntity& ent)
	{

	        //cout<<"\n\ncalculateExpressionsForOverall STARTED\n\n";

		//cout<<"\n\nmapSignalDelta IS \n\n";

		map<string, list<ConditionRelation*> >::iterator imsd;

		
		/*
		imsd = mapSignalDelta.begin();

		for(;imsd != mapSignalDelta.end();imsd++)
		{
		    string id = imsd->first;

		    cout<<"\n\nID: "<<id;

		    list<ConditionRelation*> &listCR = imsd->second;

		    list<ConditionRelation*>::iterator listCR_it = listCR.begin();

		    for(;listCR_it != listCR.end(); listCR_it++)
		      {
			ConditionRelation* cr = (*listCR_it);

			cout<<"\nCondition : "<<cr->condition->getDAGStr();
			cout<<"\nRelation : "<<cr->relation->getDAGStr();
		      }
		 }*/
		
		for(imsd = mapSignalDelta.begin();imsd != mapSignalDelta.end();imsd++)
		{
		      /*string id_delta = imsd->first;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);

			cout<<"\n\nID_DELTA is :"<<id_delta<<endl<<endl;
			
			int current = atoi(id_delta.substr(index+1).c_str());
			int previous, current_index;
					
			ConditionsValues &cv = SymbolicQueues[id];
			int size = cv.delta.size();

			for(int i=0; i < size; i++)
			{
				if(cv.delta[i] == current)
				{
					current_index = i;
					previous = cv.delta[i-1];
					break;
				}
				}*/
		        
		        string id_delta = imsd->first;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
		        
			ConditionsValues &cv = SymbolicQueues[id];

		        int current_index = cv.delta.size()-1;

			int delta_time = cv.delta[current_index];

			
			CDAGNode *final_dag = NULL;

			set<string> setCondRel;
			
			list<ConditionRelation*> &listCR = imsd->second;
			
			vector<pair<int, int> >rel_ranges;
			
			if(listCR.size() == 1)
			{
				list<ConditionRelation*>::iterator icr = listCR.begin();
				
				//pair<int, int> range = (*icr)->relation->getLHSRange();
				rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end() );
				
				CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				CDAGNode * dag1 = MANAGER.createDAG("equivalent", new_id, old_id);

				//cout<<"\n\ndag1 = "<<dag1->getDAGStr()<<endl<<endl;
				
				for(int i=1; i < (*icr)->ranges_covered.size(); i++)
				{
					CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * dag3 = MANAGER.createDAG("equivalent", new_id, old_id);
					
					dag1 = MANAGER.createDAG("and", dag3, dag1);
					
				}
				
	//			CDAGNode * dag1 = MANAGER.createDAG("equivalent", new_id, old_id);
	//			final_dag = MANAGER.createDAG("ite", cv.conditions[current_index], (*icr)->relation, dag1);

				// Newly added on 20th July 2009

				//cout<<"\n\nCondition is = "<<((*icr)->condition)->getDAGStr()<<"\n\n";

				if( ((*icr)->condition)->Get_DAGnode_value() == "true")
				  
				  final_dag = (*icr)->relation;
				
				else

				  final_dag = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);

				// Newly added on 20th July 2009. Earlier it was just final_dag = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);

								
				//cout<<"\n\nFINAL DAG INSIDE if(listCR.size() == 1) IS "<<final_dag->getDAGStr()<<endl<<endl;
	//			setCondRel.insert(cv.setConditions[current_index].begin(), cv.setConditions[current_index].end());
				setCondRel.insert((*icr)->setCondition.begin(), (*icr)->setCondition.end());
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
			}
			else
			{
			   // THIS PART MAY BE CHANGED AFTER WARDS - THIS IS THE PLACE WHERE WE ARE FINDINING THAT THERE IS (C1, R1), (C2, R2) FOR THE SAME DELTA. HERE WE ARE ALWAYS CHOOSING (C1, R1), AS I FEEL THAT BOTH (C1, R1) AND (C2, R2) SHOULD BE THE SAME
			  
			
			  //cout<<"\n\nINSIDE ELSE OF if(listCR.size() == 1) "<<endl<<endl;

			      list<ConditionRelation*>::iterator icr = listCR.begin();
				
				CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				CDAGNode * dag1 = MANAGER.createDAG("equivalent", new_id, old_id);
				
				for(int i=1; i < (*icr)->ranges_covered.size(); i++)
				{
					CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * dag3 = MANAGER.createDAG("equivalent", new_id, old_id);
					
					dag1 = MANAGER.createDAG("and", dag3, dag1);
					
				}


				// Newly added on 20th July 2009

				//cout<<"\n\nCondition is = "<<((*icr)->condition)->getDAGStr()<<"\n\n";

				CDAGNode * dag2;

				if( ((*icr)->condition)->Get_DAGnode_value() == "true")
				  
				  dag2 = (*icr)->relation;
				
				else
				  
				  dag2 = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);

				// Newly added on 20th July 2009 ends here. Earlier it was CDAGNode * dag2 = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);

				setCondRel.insert((*icr)->setCondition.begin(), (*icr)->setCondition.end());
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());
				
				//rel_ranges.push_back(range);
				rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end() );
				icr++;
				
				for(;icr != listCR.end(); icr++)
				{
					//pair<int, int> range = (*icr)->relation->getLHSRange();
					
					CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * dag1 = MANAGER.createDAG("equivalent", new_id, old_id);
					
					for(int i=1; i < (*icr)->ranges_covered.size(); i++)
					{
						CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
						//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
						CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
						CDAGNode * dag3 = MANAGER.createDAG("equivalent", new_id, old_id);
						
						dag1 = MANAGER.createDAG("and", dag3, dag1);
						
					}

					// Newly added on 20th July 2009

					//cout<<"\n\nCondition is = "<<((*icr)->condition)->getDAGStr()<<"\n\n";

					CDAGNode * dag3;

					if( ((*icr)->condition)->Get_DAGnode_value() == "true")
				  
					  dag3 = (*icr)->relation;
				
					else
					  
					  dag3 = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);
					  

					// Newly added on 20th July 2009 ends here. Earlier it was CDAGNode * dag3 = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);
					
					
					setCondRel.insert((*icr)->setCondition.begin(), (*icr)->setCondition.end());
					setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());

					
					dag2 = MANAGER.createDAG("and", dag2, dag3);
					
					//rel_ranges.push_back(range);
					rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end());
				}
				
				
				// WHY COND FROM S. QUEUE IS NEEDED? NOT NEEDED

				//final_dag = MANAGER.createDAG("ite", cv.conditions[current_index], dag2);
				//setCondRel.insert(cv.setConditions[current_index].begin(), cv.setConditions[current_index].end());
				
				
				final_dag = dag2; // final_dag is obtained here

				//cout<<"\n\nFINAL DAG INSIDE ELSE OF if(listCR.size() == 1) IS "<<final_dag->getDAGStr()<<endl<<endl;


			        /*list<ConditionRelation*>::iterator icr = listCR.begin();
				
				//pair<int, int> range = (*icr)->relation->getLHSRange();
				rel_ranges.insert(rel_ranges.begin(), (*icr)->ranges_covered.begin(), (*icr)->ranges_covered.end() );
				
				CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
				CDAGNode * dag1 = MANAGER.createDAG("equivalent", new_id, old_id);

				//cout<<"\n\ndag1 = "<<dag1->getDAGStr()<<endl<<endl;
				
				for(int i=1; i < (*icr)->ranges_covered.size(); i++)
				{
					CDAGNode * new_id = MANAGER.createDAG("select", cv.values[current_index], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					//CDAGNode * old_id = MANAGER.createDAG("select", cv.values[current_index-1], (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * old_id = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), (*icr)->ranges_covered[0].first, (*icr)->ranges_covered[0].second);
					CDAGNode * dag3 = MANAGER.createDAG("equivalent", new_id, old_id);
					
					dag1 = MANAGER.createDAG("and", dag3, dag1);
					
				}
				
	//			CDAGNode * dag1 = MANAGER.createDAG("equivalent", new_id, old_id);
	//			final_dag = MANAGER.createDAG("ite", cv.conditions[current_index], (*icr)->relation, dag1);
				final_dag = MANAGER.createDAG("ite", (*icr)->condition, (*icr)->relation, dag1);
				
				
				
				//setCondRel.insert(cv.setConditions[current_index].begin(), cv.setConditions[current_index].end());
				setCondRel.insert((*icr)->setCondition.begin(), (*icr)->setCondition.end());
				setCondRel.insert((*icr)->setRelation.begin(), (*icr)->setRelation.end());*/

				

				

			}
			
			vector<pair<int, int> >remain_ranges;
			SymTabEntry &ste = SymbolTable[ent.getName()][id];
			
			list<string>::iterator irange;
			irange = ste.rangeindex.begin();
			
			irange++;
			
			int last = atoi((*irange).c_str());
				
			getRemainingRanges(rel_ranges, remain_ranges, last);
			
			for(int j=0; j < remain_ranges.size(); j++)
			{
				pair<int, int> range = remain_ranges[j];
				
				CDAGNode * lhs = MANAGER.createDAG("select", cv.values[current_index], range.first, range.second);
				//CDAGNode * rhs = MANAGER.createDAG("select", cv.values[current_index-1], range.first, range.second);
				CDAGNode * rhs = MANAGER.createDAG("select", getPreviousIdDelta(cv.values[current_index]), range.first, range.second);

				CDAGNode * dag1 = MANAGER.createDAG("equivalent", lhs, rhs);
				
				final_dag = MANAGER.createDAG("and", final_dag, dag1);
			}
			
			//cout<<final_dag->getDAGStr()<<endl;
			
			//FinalRelation* fr = new FinalRelation(final_dag, setCondRel, rel_ranges);
			
			//final_expressions[id_delta] = fr;

			setCondRel.clear();

			final_dag->getDAGNames(setCondRel);
			
			FinalRelationWithDelta* fr = new FinalRelationWithDelta(delta_time, final_dag, setCondRel, rel_ranges);

			//cout<<"\n\nSYMBOLIC EXPRESSIONS FOR "<<id<<" : "<<final_dag->getDAGStr()<<endl<<endl;

			final_expressions[id] = fr;
		}
	}


	void CSymbSimulator::getIdentifiers(set<CSensitiveElement*> &identifiers,
										set<CSensitiveElement*> &input_identifiers,
										set<CSensitiveElement*> &output_identifiers,
										set<CSensitiveElement*> &intermediate_identifiers,CEntity& ent)
	{
	        //cout<<"\n\nInside getIdentifiers\n\n"; 

		map<string, SymTabEntry> &idMap = SymbolTable[ent.getName()];

		map<string, SymTabEntry>::iterator iter = idMap.begin();

		for(; iter != idMap.end() ; iter++)
		{
			string name = iter->first;
			//cout<<name<<endl;
			list<string>::iterator ir;
			ir = (iter->second).rangeindex.begin();
			
			string lower;
			string upper;
			
			if(ir != (iter->second).rangeindex.end())
			{
				lower = *ir;
				++ir;
				upper = *ir;
			}
			else
			{
				lower = "-1";
				upper = "-1";
			}
			
			CSensitiveElement *se = new CSensitiveElement(name, upper, lower);

			//cout<<"\n\n";

			//se->printCSensitiveElement();

			//cout<<"\n\n";

			identifiers.insert(se);

			if(iter->second.id_type == "port::out")
			{
				output_identifiers.insert(se);
				//cout<<"output = "<< iter->first <<endl;
			}

			else if(iter->second.id_type == "port::in")
			{

				input_identifiers.insert(se);
				//cout<<"input = "<< iter->first <<endl;
			}
			else
			{
			
				 intermediate_identifiers.insert(se);
                                //cout<<"intermediate = "<< iter->first <<endl;
                        }

		}
	}

	void CSymbSimulator::printSymbolicQueues(map<string, ConditionsValues>& SymbolicQueues)
	{
		map< string, ConditionsValues >::iterator isymb = SymbolicQueues.begin();

		cout<<"\n\n********************************\n\n***SymbolicQueues**\n\n**************************************\n\n"<<endl;

		for(;isymb != SymbolicQueues.end(); isymb++)
		{
			ConditionsValues &cv = isymb->second;

			int size = cv.delta.size();

			cout<<"**************"<<endl;
			for(int i=0; i < size; i++)
			{
				cout<<cv.delta[i] <<endl;
				cout<<cv.values[i]->getDAGStr()<<endl;
				cout<<cv.conditions[i]->getDAGStr()<<endl;
				cout<<"---------"<<endl;
			}
		}

	}

	void CSymbSimulator::getRemainingRanges(vector<pair<int, int> >& rel_ranges, vector<pair<int, int> >&remain_ranges, int last)
	{
		sort(rel_ranges.begin(), rel_ranges.end(), compare_pair());

		int start = 0;
		for(int i =0; i<rel_ranges.size(); i++)
		{
			if(start==rel_ranges[i].first)
			{
				start = rel_ranges[i].second+1;
				continue;	
			}
			else 
			{
				remain_ranges.push_back(make_pair(start, rel_ranges[i].first-1));
			}
		}
		
		if(start <= last)
		{
			remain_ranges.push_back(make_pair(start, last));
		}
	}


      //  	void CSymbSimulator::convertIteToAnds(list<CDAGNode*> &node_list, CDAGNode *root)
// 	  {
	        
// 	    string dagName = root->name;

// 	    //cout<<"\nDAG NAME="<<dagName<<endl;

// 	    if(dagName == "and")
	      
// 	      {
// 		list<CDAGNode*>::iterator iout = root->out_list.begin();

// 		for(; iout != root->out_list.end(); iout++)
// 		{
// 		  //node_list.push_back((*iout));

// 		  convertIteToAnds(node_list,(*iout));
// 		}
// 	      }
// 	    else
// 	      {
// 		node_list.push_back(root);
// 	      }

// 	  } 

 //        CDAGNode* CSymbSimulator::convertIteToFunction(CDAGNode *root)
// 	  {
// 	    CDAGNode* final_dag;

// 	    list<CDAGNode*> node_list;

// 	    convertIteToAnds(node_list,root);

// 	    // Printing the initial list

// 	    //cout<<"\n\nPRINTING THE INITIAL LIST\n\n";

// 	    list<CDAGNode*>::iterator iout; 
	    
// 	    //for(iout = node_list.begin();iout != node_list.end(); iout++)
// 	    // {
// 	    //	cout<<endl<<endl<<(*iout)->getDAGStr();
// 	    //	
// 	    //  }

// 	    iout  = node_list.begin();

// 	    bool match_found=false;

// 	    while(iout != node_list.end())
// 	      {
// 		match_found = false;

// 		if((*iout)->name == "ite")
// 		  {
// 		   list<CDAGNode*>::iterator child  = (*iout)->out_list.begin(); 
// 		   CDAGNode *cond,*stat1,*stat2;

// 		   cond = (*child);
// 		   ++child;
// 		   stat1 = (*child);
// 		   ++child;
// 		   stat2 = (*child);
		   
// 		   if((stat1->name == "equivalent") && (stat2->name == "equivalent"))
// 		     {
// 		      list<CDAGNode*>::iterator stat1_child  = stat1->out_list.begin(); 
// 		      list<CDAGNode*>::iterator stat2_child  = stat2->out_list.begin(); 
// 		      CDAGNode *lhs1,*lhs2,*rhs1,*rhs2;
		      
// 		      lhs1 = (*stat1_child);
// 		      lhs2 = (*stat2_child);
// 		      ++stat1_child; ++stat2_child;
// 		      rhs1 = (*stat1_child);
// 		      rhs2 = (*stat2_child);
		      
// 		      if((lhs1->name == lhs2->name))  
// 			{
// 			  node_list.push_back(MANAGER.createDAG("equivalent",lhs1,MANAGER.createDAG("ite",cond,rhs1,rhs2)));
// 			  iout = node_list.erase(iout);
// 			  match_found = true;
// 			}
// 		     }
// 		  }

// 		if(match_found == false)
// 		  ++iout;
		
// 	      }

// 	    // Printing the list

// 	    //cout<<"\n\nPRINTING THE LIST\n\n";

// 	    //iout  = node_list.begin();
	    
// 	    //for(;iout != node_list.end(); iout++)
// 	    //  {
// 	    //	cout<<endl<<endl<<(*iout)->getDAGStr();
// 	    //	
// 	    //  }
	      
			  
// 	    iout  = node_list.begin();
	    
// 	    final_dag = (*iout);

// 	    iout++;

// 	    for(;iout != node_list.end(); iout++)
// 	      {
// 		final_dag = MANAGER.createDAG("and",final_dag,(*iout));
		
// 	      }
	      

// 	    return final_dag;
	    
// 	  }


// CDAGNode* CSymbSimulator::Inlining(CDAGNode *root, set<string> setExQuantify)
// 	  {
// 	    //cout<<"\n\nInlining Starts\n\n";

// 	    //cout<<"\n\nConverting to list of functions\n\n";

// 	    CDAGNode* final_dag;

// 	    list<CDAGNode*> node_list;

// 	    list<CDAGNode*>::iterator iout;

// 	    convertIteToAnds(node_list,root);

// 	    //Printing the initial list

// 	    // cout<<"\n\nPRINTING THE INITIAL LIST\n\n";

// // 	    iout  = node_list.begin();
	    
// // 	    for(;iout != node_list.end(); iout++)
// // 	     {
// // 	    	cout<<endl<<endl<<(*iout)->getDAGStr();
		
// // 	     }

// 	    //No Inlining needed if there is only a single constraint

// 	    if(node_list.size() <= 1)
// 	      {
// 		cout<<"\nNo Inlining needed as there is only a single constraint\n";
// 		return root;
// 	      }	    
	    

// 	    iout  = node_list.begin();

// 	    bool match_found=false;

// 	    while(iout != node_list.end())
// 	      {
// 		match_found = false;

// 		if((*iout)->name == "ite")
// 		  {
// 		   list<CDAGNode*>::iterator child  = (*iout)->out_list.begin(); 
// 		   CDAGNode *cond,*stat1,*stat2;

// 		   cond = (*child);
// 		   ++child;
// 		   stat1 = (*child);
// 		   ++child;
// 		   stat2 = (*child);
		   
// 		   if((stat1->name == "equivalent") && (stat2->name == "equivalent"))
// 		     {
// 		      list<CDAGNode*>::iterator stat1_child  = stat1->out_list.begin(); 
// 		      list<CDAGNode*>::iterator stat2_child  = stat2->out_list.begin(); 
// 		      CDAGNode *lhs1,*lhs2,*rhs1,*rhs2;
		      
// 		      lhs1 = (*stat1_child);
// 		      lhs2 = (*stat2_child);
// 		      ++stat1_child; ++stat2_child;
// 		      rhs1 = (*stat1_child);
// 		      rhs2 = (*stat2_child);
		      
// 		      if((lhs1->name == lhs2->name))  
// 			{
// 			  node_list.push_back(MANAGER.createDAG("equivalent",lhs1,MANAGER.createDAG("ite",cond,rhs1,rhs2)));
// 			  iout = node_list.erase(iout);
// 			  match_found = true;
// 			}
// 		     }
// 		  }

// 		if(match_found == false)
// 		  ++iout;
		
// 	      }

// 	    //Printing the list

// 	    // cout<<"\n\nPrinting the list of functions\n\n";

// // 	    iout  = node_list.begin();
	    
// // 	    for(;iout != node_list.end(); iout++)
// // 	     {
// // 	    	cout<<endl<<endl<<(*iout)->getDAGStr();
	    	
// // 	     }
	    
// 	    //cout<<"\n\nConverting to list of functions ends\n\n";
// 	    //cout<<"\n\nActual inlining starts\n\n";
	    
// 	    set<string>::iterator setExQuantify_it;
// 	    for(setExQuantify_it = setExQuantify.begin(); setExQuantify_it != setExQuantify.end(); setExQuantify_it++)
// 	      {
// 		string element = *(setExQuantify_it);
// 		//cout<<"\n\nVariable to be inlined is "<<element;
// 		int index = element.find_last_of("_");
// 		string element_name = element.substr(0, index); 
// 		int element_delta = atoi(element.substr(index+1).c_str()); 

// 		string element_lower, element_upper;
// 		CDAGNode* target;
// 		match_found=false;

		
// 		//cout<<"\n\nTaking each function from the list of functions\n\n";
// 		iout  = node_list.begin();
// 		for(;iout != node_list.end(); iout++)
// 		  {
// 		    CDAGNode* function_root = (*iout);
// 		    if(function_root->name == "equivalent")
// 		      {
// 		      list<CDAGNode*>::iterator equivalent_child  = function_root->out_list.begin(); 
// 		      CDAGNode *lhs,*rhs;
		      
// 		      lhs = (*equivalent_child);
// 		      ++equivalent_child;
// 		      rhs = (*equivalent_child);

// 		      if(lhs->name == "select")  
// 			{
// 			  list<CDAGNode*>::iterator iselectout = (lhs->out_list).begin();
// 			  string signal_name = (*iselectout)->name;
// 			  int signal_delta = (*iselectout)->delta_time;
			  
			  
// 			  if(element_name == signal_name && element_delta == signal_delta)
// 			    {
// 			      //cout<<"\n\nMatch found !!!\n\n";
// 			      iselectout++;    
// 			      element_lower = (*iselectout)->name;
// 			      iselectout++;
// 			      element_upper = (*iselectout)->name;
// 			      target = rhs;
// 			      iout = node_list.erase(iout); // delete the matching entry from the list as it is no longer needed
// 			      match_found = true;
// 			      break;
// 			    }
// 			}
// 		      if(rhs->name == "select")  
// 			{
// 			  list<CDAGNode*>::iterator iselectout = (rhs->out_list).begin();
// 			  string signal_name = (*iselectout)->name;
// 			  int signal_delta = (*iselectout)->delta_time;
			  
			  
// 			  if(element_name == signal_name && element_delta == signal_delta)
// 			    {
// 			      //cout<<"\n\nMatch found !!!\n\n";
// 			      iselectout++;    
// 			      element_lower = (*iselectout)->name;
// 			      iselectout++;
// 			      element_upper = (*iselectout)->name;
// 			      target = lhs;
// 			      iout = node_list.erase(iout); // delete the matching entry from the list as it is no longer needed
// 			      match_found = true;
// 			      break;
// 			    }
// 			}
// 		      }
// 		  }

// 		if(match_found == true)
// 		  {
// 		    //cout<<"\n\nReplacing each occurance of "<<element<<" by "<<target->getDAGStr()<<endl<<endl;

// 		    iout  = node_list.begin();
// 		    for(;iout != node_list.end(); iout++)
// 		      {
// 			//cout<<"\n\nFunction Before Inlining : "<<(*iout)->getDAGStr()<<endl<<endl;
// 			(*iout)->replaceSubDag(element_name, element_delta, element_lower, element_upper, target);
// 			//cout<<"\n\nFunction After Inlining : "<<(*iout)->getDAGStr()<<endl<<endl;
// 		      }
// 		  }
// 	      }
		    
	    
// 	    //cout<<"\n\nActual inlining finishes\n\n";

// 	    //Printing the inlined list

// 	    // cout<<"\n\nPrinting the inlined list of functions\n\n";

// // 	    iout  = node_list.begin();
	    
// // 	    for(;iout != node_list.end(); iout++)
// // 	     {
// // 	    	cout<<endl<<endl<<(*iout)->getDAGStr();
	    	
// // 	    }

// 	    //cout<<"\n\nConverting the list of inlined functions to DAG\n\n";
  
// 	    iout  = node_list.begin();
// 	    final_dag = (*iout);
// 	    iout++;

// 	    for(;iout != node_list.end(); iout++)
// 	      {
// 		final_dag = MANAGER.createDAG("and",final_dag,(*iout));
		
// 	      }
	      

// 	    return final_dag;
	    
// 	  }
	  

// FUNCTIONS PERFORMING INLINING



/*void CSymbSimulator::performInling(CEntity& ent)
{
	map< string, list<RelationInputOutput*> >& table = ent.getTable();

	map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
	
	for(;itab != table.end(); itab++)
	{
		string output_identifier = itab->first;
		list<RelationInputOutput*> & entries = itab->second;
		
		for(list<RelationInputOutput*>::iterator irio = entries.begin();
			irio != entries.end();
			irio++)
		{
			RelationInputOutput* rio = *irio;
			list<CDAGNode*> listOfExists;
			rio->relation->getExistentialExp(listOfExists);
		}
	}
}


void CSymbSimulator::getExistentialExp(list<CDAGNode*>& listOfExists)
{
	if(name == "exists")
	{
		listOfExists.push_back(this);
	}
	else
	{
		list<CDAGNode*>::iterator iout = out_list.begin();
	
		for(; iout != out_list.end(); iout++)
		{
			(*iout)->getExistentialExp(listOfExists);		
		}	
	}
}*/

/*===============================================================================================
NOTE: This function will take set of strings which contains variables to be existentially quantified out and pointer to DAG. [Assume "exists" in the following form: exists(set of strings,pointer to DAG)]

This function will call function "findPlacesOfInlining" which is defined next.*/
//If there is some problem, check line no. (1563,1564), (1590,1591), (1610,1611)

// 	void CSymbSimulator::call_findPlacesOfInlining(set<string> setExQuantify,CDAGNode *node)
// 	{
// 	        clock_t start_time;
// 		list<CDAGNode*> listOfPlacesToBeReplaced;
// 		list<CDAGNode*> adjustAndWhere;
// 		list<CDAGNode*> adjustAndBy;
// 		list<CDAGNode*> replaceBy;
// 		map<CDAGNode*,CDAGNode*> nodeShiftednode;
// 		set<string> setRelation;
// 		set<string> quantifiedout;
// 		set<string> setExQuantify_ordered;
// 		set<string> setExQuantify_temp;
// 		list<int> deltas;
// 		bool is_var_quantified=false;
// 		bool valAssignedToRelaceBy = false;
		
// 		// Adding the code for ordering variables to be existentially quantified out
		
// 		//int index = name.find_last_of("_");
// 		//int delta = atoi(name.substr(index+1).c_str());
		
// 		setExQuantify_temp = setExQuantify;
		
// 		/*
// 		while(setExQuantify_temp.size() != 0)
// 		{
// 			string var;
// 			deltas.clear();
// 			set<string>::iterator existQuantiOut = setExQuantify_temp.begin();
			
// 			cout<<endl<<"here1 in call_findPlacesOfInlining"<<endl;
			
// 			for(; existQuantiOut != setExQuantify_temp.end(); existQuantiOut++)
// 			{
// 				string variable = *existQuantiOut;
// 				cout<<endl<<"variable ="<<variable<<endl;
// 				int index = variable.find_last_of("_");
// 				var = variable.substr(0,index).c_str();
			
// 				cout<<"here2 in call_findPlacesOfInlining"<<endl;
				
// 				set<string>::iterator existQuantiOut1 = setExQuantify_temp.begin();
// 				for(; existQuantiOut1 != setExQuantify_temp.end(); existQuantiOut1++)
// 				{
// 					string variable1 = *existQuantiOut1;
// 					int index = variable1.find_last_of("_");
// 					string currvar = variable1.substr(0,index).c_str();
					
// 					cout << "currvar=" << currvar << endl;
					
// 					if(var == currvar)
// 					{
// 						int delta = atoi(variable1.substr(index+1).c_str());
// 						deltas.push_back(delta);
// 						setExQuantify_temp.erase(*existQuantiOut1);
// 						//existQuantiOut = setExQuantify_temp.begin();
// 					}	
// 				}
				
// 				deltas.sort();	
				
// 				cout<<"here3 in call_findPlacesOfInlining"<<endl;
				
// 				list<int>::iterator a = deltas.begin();
// 				for(; a != deltas.end(); a++)
// 				{
// 					string temp = var ;
// 					temp +=  "_";
// 					temp += (*a);	
// 					setExQuantify_ordered.insert(temp);
// 				}
				
// 				cout<<"here4 in call_findPlacesOfInlining"<<endl;
// 			}
			
			
				
			
// 		}
// 		/*
// 		cout << endl << endl << "ORDERED VARIABLES:::" << endl << endl; 
// 		set<string>::iterator pqr = setExQuantify_ordered.begin();
		
// 		for(; pqr != setExQuantify_ordered.end(); pqr++)
// 		{
// 			cout << "variable=" << (*pqr) << endl;
// 		}
// 		*/
		
// 		set<string>::iterator existQuantiOut = setExQuantify.begin();

// 		start_time = clock();
		
// 		for(; existQuantiOut != setExQuantify.end(); existQuantiOut++)
// 		{
// 			listOfPlacesToBeReplaced.clear();
// 			adjustAndWhere.clear();
// 			adjustAndBy.clear();
// 			nodeShiftednode.clear();
// 			setRelation.clear();
// 			replaceBy.clear();
// 			is_var_quantified=false;
// 			valAssignedToRelaceBy = false;
			
// 			string currExistQuantiVar = *existQuantiOut;
			
// 			//cout << endl << endl<< "TRYING TO QUANTIFY=" << currExistQuantiVar << endl << endl;
// 			findPlacesOfInlining(currExistQuantiVar,node,listOfPlacesToBeReplaced,adjustAndWhere,adjustAndBy,replaceBy,valAssignedToRelaceBy);
			
			
// 			if(replaceBy.size() != 0)
// 			{	
// 				list<CDAGNode*>::iterator replace = listOfPlacesToBeReplaced.begin();
// 				for(; replace != listOfPlacesToBeReplaced.end(); replace++)
// 				{
// 					CDAGNode* a = new CDAGNode();
					
// 					list<CDAGNode*>::iterator i = replaceBy.begin();
// 					a = MANAGER.cloneAndShift((*i), 0);
					
// 					**replace = *a;
// 					is_var_quantified=true;
// 				}
// 			}
			
// 			if(is_var_quantified)
// 			{
// 				quantifiedout.insert(currExistQuantiVar);	
// 			}
			
// 			nodeShiftednode.clear();
// 			setRelation.clear();
			
// 			if(adjustAndBy.size() != 0 && adjustAndWhere.size() == 0)
// 			{
// 				list<CDAGNode*>::iterator adjustby = adjustAndBy.begin();
// 				for(; adjustby != adjustAndBy.end(); adjustby++)
// 				{
// 					*node = **adjustby;	
// 				}
// 			}
// 			else
// 			{
// 				list<CDAGNode*>::iterator adjustby = adjustAndBy.begin();
// 				for(; adjustby != adjustAndBy.end(); adjustby++)
// 				{
// 					list<CDAGNode*>::iterator adjustwhere = adjustAndWhere.begin();
// 					for(; adjustwhere != adjustAndWhere.end(); adjustwhere++)
// 					{
// 						CDAGNode* a = new CDAGNode(); 
// 						a = MANAGER.cloneAndShift((*adjustby), 0);
// 						**adjustwhere = *a;
// 					}
// 				}
// 			}
// 			//cout << endl << endl<< "DAG after replacing " << currExistQuantiVar << "=" << node->getDAGStr() << endl << endl;
// 		}

// 		cout<<"\n\nTime needed for first loop = "<< ((double)clock()-start_time)/CLOCKS_PER_SEC <<endl<<endl;

// 		start_time = clock();
		
// 		if(quantifiedout.size() == setExQuantify.size())
// 		{
// 			list<CDAGNode*>::iterator remove_exists = node->in_list.begin();
// 			for(; remove_exists != node->in_list.end(); remove_exists++)
// 			{	
// 				if(node->name == "exists")
// 				{
// 					list<CDAGNode*>::iterator remove_exists1 = (*remove_exists)->in_list.begin();
// 					for(; remove_exists1 != (*remove_exists)->in_list.end(); remove_exists1++)
// 					{	
// 						nodeShiftednode.clear();
// 						setRelation.clear();
						
// 						CDAGNode* a = new CDAGNode();
// 						a = MANAGER.cloneAndShift(node, 0);
// 						**remove_exists1 = *a;
// 					}
// 				}
// 				else
// 				{
// 					continue;
// 				}
// 			}			
			
// 		}

// 		cout<<"\n\nTime needed for second loop = "<< ((double)clock()-start_time)/CLOCKS_PER_SEC <<endl<<endl;
		
// 	}


// /*==================================================================================================================
//   NOTE: This function will actually do inlining.*/	

// 	void CSymbSimulator::findPlacesOfInlining(string currExistQuantiVar,CDAGNode* node,list<CDAGNode*>& listOfPlacesToBeReplaced,list<CDAGNode*>& adjustAndWhere,list<CDAGNode*>& adjustAndBy,list<CDAGNode*>& replaceBy,bool& valAssignedToRelaceBy)
// 	{	
// 		//cout << endl << endl << "HERE BEFORE  'IF'" << endl << endl;
// 		if(valAssignedToRelaceBy == false)
// 		{
// 			//cout << endl << endl << "HERE INSIDE 'IF'" << endl << endl; 
// 			if(node->name == "and")
// 			{
			
// 				list<CDAGNode*>::iterator i = node->out_list.begin();
		
// 				for(; i != node->out_list.end(); i++)
// 				{	
// 					if(valAssignedToRelaceBy == false)
// 					{
					
// 						if((*i)->name == "equivalent")
// 						{
// 							int count=0;
// 							bool is_currExistQuantiVar_present = false;
							
// 							//cout << endl << "(*i)->getDAGStr()=" << (*i)->getDAGStr() << endl; 
						
// 							list<CDAGNode*>::iterator j = (*i)->out_list.begin();
			
// 							for(; j != (*i)->out_list.end(); j++)
// 							{
// 								if((*j)->name == "select")
// 								{ 
// 									++count;
									
// 									list<CDAGNode*>::iterator k = (*j)->out_list.begin();
							
// 									if((*k)->getDAGStr() == currExistQuantiVar)
// 									{
// 										is_currExistQuantiVar_present=true;			
// 									}
// 								}
// 							}
							
// 							//cout << endl << "here1" << endl;
					
// 							if((count == 2) && is_currExistQuantiVar_present)
// 							{
// 								//cout << endl << "(count == 2) && is_currExistQuantiVar_present" << endl;
								
// 								j = (*i)->out_list.begin();
						
// 								for(; j != (*i)->out_list.end(); j++)
// 								{
// 									if((*j)->name == "select")
// 									{
// 										list<CDAGNode*>::iterator k = (*j)->out_list.begin();
									
// 										//cout << endl << "(*k)->getDAGStr()=" << (*k)->getDAGStr() << endl;
// 										//cout << endl << "currExistQuantiVar=" << currExistQuantiVar << endl;
// 										if((*k)->getDAGStr() == currExistQuantiVar)
// 										{
// 											listOfPlacesToBeReplaced.push_back(*j);			
// 										}
// 										else
// 										{
// 											//continue;
// 											replaceBy.push_back(*j);
// 											valAssignedToRelaceBy = true;	
// 										}	
// 									}
// 								}
// 							}
// 							else if((count == 1) && is_currExistQuantiVar_present)
// 							{
// 								//cout << endl << "(count == 1) && is_currExistQuantiVar_present" << endl;
// 								j = (*i)->out_list.begin();
			
// 								for(; j != (*i)->out_list.end(); j++)
// 								{
// 									if((*j)->name == "select")
// 									{
// 										continue;
// 									}
// 									else
// 									{	
// 										replaceBy.push_back(*j);
// 										valAssignedToRelaceBy = true;
// 									}
// 								} 
						
// 								j = (*i)->in_list.begin();
						
// 								for(; j != (*i)->in_list.end(); j++)
// 								{
// 									if((*j)->name == "and")
// 									{
// 										list<CDAGNode*>::iterator k = (*j)->out_list.begin();
									
// 										for(; k != (*j)->out_list.end(); k++)
// 										{	
// 											bool counter = false;	
						
// 											if((*k)->name == "equivalent")
// 											{
// 												list<CDAGNode*>::iterator l = (*k)->out_list.begin();
										
// 												for(; l != (*k)->out_list.end(); l++)
// 												{
// 													if((*l)->name == "select")
// 													{
// 														list<CDAGNode*>::iterator z = (*l)->out_list.begin();
										
// 														for(; z != (*l)->out_list.end(); z++)
// 														{
// 															if((*z)->getDAGStr() == currExistQuantiVar)
// 															{
// 																counter=true;
// 															}
// 														}	
// 													}	
// 												}
										
// 												if(counter == false)
// 												{
// 													adjustAndBy.push_back(*k);
	
// 													if((*j)->in_list.size() !=  0)
// 													{
// 														list<CDAGNode*>::iterator l = (*j)->in_list.begin();
										
// 														for(; l != (*j)->in_list.end(); l++)
// 														{
// 															list<CDAGNode*>::iterator k = (*l)->out_list.begin();
// 															for(; k != (*l)->out_list.end(); k++)
// 															{
// 																if((*k)->name == "and")
// 																{
// 																	adjustAndWhere.push_back(*k);
// 																}
// 															}
// 														}
// 													}
// 												}
										
// 											}
// 											else
// 											{
// 												adjustAndBy.push_back(*k);
										
// 												list<CDAGNode*>::iterator l = (*j)->in_list.begin();
										
// 												for(; l != (*j)->in_list.end(); l++)
// 												{
// 													if((*l)->name == "and")
// 													{
// 														adjustAndWhere.push_back(*l);
// 													}
// 												}
// 											}
// 										}
// 									}
// 									else
// 									{
// 										continue;	
// 									}
// 								}
						
// 								remainingReplacePlaces(node,currExistQuantiVar,listOfPlacesToBeReplaced);
// 							}
// 							else if(count == 1 && !(is_currExistQuantiVar_present))
// 							{
// 								//cout << endl << "(count == 1) && !(is_currExistQuantiVar_present)" << endl;
// 								j = (*i)->out_list.begin();
			
// 								for(; j != (*i)->out_list.end(); j++)
// 								{
// 									if((*j)->name == "select")
// 									{
// 										continue;
// 									}
// 									else
// 									{	
// 										additionalReplacePlaces(*j,currExistQuantiVar,listOfPlacesToBeReplaced);
// 									}
// 								}
// 							}
// 							//else 
// 							//{
// 								//remainingReplacePlaces(node,currExistQuantiVar,listOfPlacesToBeReplaced);	
// 							//}
// 						}
// 						else if((*i)->name == "and")
// 						{
// 							findPlacesOfInlining(currExistQuantiVar,*i,listOfPlacesToBeReplaced,adjustAndWhere,adjustAndBy,replaceBy,valAssignedToRelaceBy);
// 						}
// 						//else if((*i)->name == "select")
// 						//{
// 							//remainingReplacePlaces(*i,currExistQuantiVar,listOfPlacesToBeReplaced);
// 						//}
// 					}
// 				}
// 			}
// 		}
// 		//else 
// 		//{
// 			additionalReplacePlaces(node,currExistQuantiVar,listOfPlacesToBeReplaced);
// 		//}
// 	}


	
// 	void CSymbSimulator::additionalReplacePlaces(CDAGNode *node,string currExistQuantiVar,list<CDAGNode*>& listOfPlacesToBeReplaced)
// 	{
// 		if(node->getDAGStr() == currExistQuantiVar)
// 		{
// 			list<CDAGNode*>::iterator j = node->in_list.begin();
// 			for(; j != node->in_list.end(); j++)
// 			{
// 				if((*j)->name == "select")
// 				{
// 					list<CDAGNode*>::iterator p = (*j)->in_list.begin();
// 					for(; p != (*j)->in_list.end(); p++)
// 					{
// 						listOfPlacesToBeReplaced.push_back(*j);
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			list<CDAGNode*>::iterator j = node->out_list.begin();
// 			for(; j != node->out_list.end(); j++)
// 			{
// 				additionalReplacePlaces(*j,currExistQuantiVar,listOfPlacesToBeReplaced);
// 			}
// 		}	
// 	}

// 	void CSymbSimulator::remainingReplacePlaces(CDAGNode *node,string currExistQuantiVar,list<CDAGNode*>& listOfPlacesToBeReplaced)
// 	{	
// 		if((node)->getDAGStr() == currExistQuantiVar)
// 		{
// 			list<CDAGNode*>::iterator j = node->in_list.begin();
// 			for(; j != node->in_list.end(); j++)
// 			{
// 				if((*j)->name == "select")
// 				{
// 					list<CDAGNode*>::iterator p = (*j)->in_list.begin();
// 					for(; p != (*j)->in_list.end(); p++)
// 					{
// 						listOfPlacesToBeReplaced.push_back(*j);
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			list<CDAGNode*>::iterator j = node->out_list.begin();

// 			for(; j != node->out_list.end(); j++)
// 			{
// 				remainingReplacePlaces(*j,currExistQuantiVar,listOfPlacesToBeReplaced);
// 			}
// 		}	
// 	}


/*******************************************************************************************************************
								Functions for Inlining over
********************************************************************************************************************/

/*******************************************************************************************************************
								Functions for Scope Reduction
********************************************************************************************************************/
// 	void CSymbSimulator::listOfFunctionNodesForFactorGraph(CDAGNode* node,list<CDAGNode*>& listOfFunctionNodes)
// 	{
// 		if(node->name == "and")
// 		{
// 			list<CDAGNode*>::iterator i = node->out_list.begin();
			
// 			for(; i != node->out_list.end(); i++)
// 			{
// 				if((*i)->name == "and")
// 				{
// 					listOfFunctionNodesForFactorGraph(*i,listOfFunctionNodes);
// 				}
// 				else
// 				{
// 					listOfFunctionNodes.push_back(*i);
// 				}
// 			}
// 		}
// 	}

// 	void CSymbSimulator::listOfVariables(CDAGNode* node,set<string>& setOfVariables)
// 	{
// 		if(node->name == "select")
// 		{
// 			list<CDAGNode*>::iterator i = node->out_list.begin();
			
// 			if((*i)->name == "select")
// 			{
// 				list<CDAGNode*>::iterator j = (*i)->out_list.begin();
			
// 				for(; j != (*i)->out_list.end(); j++)
// 				{
// 					listOfVariables(*j,setOfVariables);
// 				}	
// 			}
// 			else
// 			{
// 				string variableNodeName = (*i)->getDAGStr();
// 				//cout << "variaable name=" << variableNodeName << endl;
// 				setOfVariables.insert(variableNodeName);
// 			} 
// 		}
// 		else
// 		{
// 			list<CDAGNode*>::iterator i = node->out_list.begin();
			
// 			for(; i != node->out_list.end(); i++)
// 			{
// 				listOfVariables(*i,setOfVariables);
// 			}
// 		}	
// 	}

// 	CDAGNode* CSymbSimulator::scopeReduction(set<string> setExQuantify,CDAGNode* node)
// 	{
// 	  CDAGNode* SReducedDAG = NULL;

// 		if(setExQuantify.size() > 1)
// 		{
// 			list<CDAGNode*> listOfFunctionNodes_DAGNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes_temp;
// 			list<CFactorGraphNode*> listOfVariableNodes_ordered;
// 			list<CFactorGraphNode*> listOfFunctionNodes;
// 			list<CFactorGraphNode*> listOfNewFunNodes;
// 			list<CFactorGraphNode*> listOfFunctionNodes_floating;
// 			set<string> variableSetForFunctionNode;
			
// 			// It was found that several nodes which are unneeded gets added to listOfNewFunNodes unnecessarily. This is to filter out only the needed ones. Added on 18/03/09

// 			set<CFactorGraphNode*> SetOfNewFunNodes_Unneeded; 

// 			//Creating variable nodes of a factor graph
// 			set<string>:: iterator a = setExQuantify.begin();
// 			for(; a != setExQuantify.end(); a++)
// 			{
// 				CFactorGraphNode* varNode = new CFactorGraphNode();
// 				varNode->type = 0;
// 				varNode->name = (*a);
// 				listOfVariableNodes.push_back(varNode);
// 			}
			
// 			//Creating function nodes of a factor graph
// 			listOfFunctionNodesForFactorGraph(node,listOfFunctionNodes_DAGNodes);
			
// 			if(listOfFunctionNodes_DAGNodes.size() == 0)
// 			{
// 			        // cout << endl << "***********************"<< endl << endl;
// 			        // cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// 			        // cout << endl << "***********************"<< endl << endl;
				
// 				CDAGNode* temp6 = getDAGFromStringSet(setExQuantify); // setExQuantify is non-empty here 
// 				SReducedDAG = MANAGER.createDAG("exists", temp6, node); 

// 				// cout << endl << "***********************"<< endl << endl;
// 				// cout << endl << "VALUE RETURNED"<< endl << endl;
// 				// cout << endl << "***********************"<< endl << endl;
// 				return SReducedDAG;	
// 			}
			
// 			// cout << endl << "***********************"<< endl << endl;
// 			// cout << endl << "SCOPE REDUCTION POSSIBLE"<< endl << endl;
// 			// cout << endl << "***********************"<< endl << endl;

// 			list<CDAGNode*>:: iterator b = listOfFunctionNodes_DAGNodes.begin();
// 			for(; b != listOfFunctionNodes_DAGNodes.end(); b++)
// 			{
// 				CFactorGraphNode* funNode = new CFactorGraphNode();
// 				funNode->type = 1;
// 				funNode->name = (*b)->getDAGStr();
// 				funNode->exp = *b;
				
// 				//Creating edges in the above nodes
// 				variableSetForFunctionNode.clear();
// 				//cout << "(*b)->getDAGStr()=" << (*b)->getDAGStr() << endl;
// 				listOfVariables(*b,variableSetForFunctionNode);//Getting the set of variables in a constraint
				
// 				set<string>::iterator d = setExQuantify.begin();
// 				for(; d != setExQuantify.end(); d++)
// 				{
// 					 set<string>::iterator e = variableSetForFunctionNode.begin();
// 					for(; e != variableSetForFunctionNode.end(); e++)
// 					{
// 						if((*d) == (*e))
// 						{
// 							list<CFactorGraphNode*>::iterator f = listOfVariableNodes.begin();
// 							for(; f != listOfVariableNodes.end(); f++)
// 							{
// 								if((*e) == (*f)->name)
// 								{
// 									(*f)->list_of_edges.push_back(funNode);
// 									funNode->list_of_edges.push_back(*f);
// 								}
// 							}
// 						}
// 					}
// 				}
				
// 				listOfFunctionNodes.push_back(funNode);
// 			}


// 			/* Newly added - Finding FunctionNodes with empty list_of_edges */

			
// 			list<CFactorGraphNode*>::iterator it;
					
// 			for(it = listOfFunctionNodes.begin(); it != listOfFunctionNodes.end(); ++it)
// 			  {
// 			    CFactorGraphNode* temp = (*it);
// 			    if( (temp->list_of_edges).empty() )
// 			      {
				
// 				listOfFunctionNodes_floating.push_back(temp);
// 			      }
		
// 			  }


// 			// Displaying the factor graph
// 			/*
// 			list<CFactorGraphNode*>::iterator Var_it;
// 			list<CFactorGraphNode*>::iterator Fun_it;

// 			cout<<"\n\nFactor Graph is\n\n";

// 			for(Var_it = listOfVariableNodes.begin(); Var_it != listOfVariableNodes.end(); Var_it++)
// 			  {
// 			    cout<<"\n\nVariable Node : "<<(*Var_it)->name<<endl;
			    
// 			    if( ((*Var_it)->list_of_edges).empty() )
// 			      {
// 				cout<<"\nNot connected to any node\n";
// 			      }
// 			    else
// 			      {
// 				cout<<"\nConnected to the following nodes\n";

// 				for(Fun_it = ((*Var_it)->list_of_edges).begin(); Fun_it != ((*Var_it)->list_of_edges).end(); Fun_it++)	
// 				  {
// 				    cout<<endl<<(*Fun_it)->name<<endl;
// 				  }
// 			      }
// 			    }*/


			
// 			listOfVariableNodes_temp = listOfVariableNodes;
			
// 			while(listOfVariableNodes_temp.size() != 0)
// 			{
// 				//cout << "listOfVariableNodes_temp.size()=" << listOfVariableNodes_temp.size() << endl;
// 				CFactorGraphNode* min = new CFactorGraphNode();
// 				bool deleted = false;
				
// 				list<CFactorGraphNode*>::iterator h = listOfVariableNodes_temp.begin();
// 				min = (*h); //Intialization
// 				for(; h != listOfVariableNodes_temp.end(); h++)
// 				{
// 					if((*h)->list_of_edges.size() <= min->list_of_edges.size())
// 					{
// 						min = (*h);
// 					}
// 				}
				
// 				//cout << "min=" << min->name << endl;
// 				listOfVariableNodes_ordered.push_back(min);
				
// 				list<CFactorGraphNode*>::iterator i = listOfVariableNodes_temp.begin();
// 				while(i != listOfVariableNodes_temp.end())
// 				{
// 					if(((*i)->name == min->name) && (deleted == false))
// 					{
// 						if((++i) == listOfVariableNodes_temp.end())
// 						{
// 							--i;
// 							--i;
// 							listOfVariableNodes_temp.pop_back();	
// 						}
// 						else
// 						{
// 							--i;
// 							i = listOfVariableNodes_temp.erase(i);	
// 						}
						
// 						deleted = true;
// 					}
// 					else
// 					{
// 						++i;
// 						//break;	
// 					}
// 				}
// 				//cout << "here3" << endl;
// 			}
			
// 			/*cout << endl << endl << "listOfVariableNodes_ordered.size()=" << listOfVariableNodes_ordered.size() << endl;
// 			cout << endl << endl << "listOfVariableNodes.size()=" << listOfVariableNodes.size() << endl;
			
			
// 			list<CFactorGraphNode*>::iterator j = listOfVariableNodes_ordered.begin();
// 			for(; j != listOfVariableNodes_ordered.end(); j++)
// 			{
// 				cout << endl << endl << "variable name=" << (*j)->name << endl;
// 				cout << "list_of_edges.size()=" << (*j)->list_of_edges.size() << endl;	
// 			}*/
			

		
// 			// Displaying the factor graph

// 			/*
// 			cout<<"\n\nFactor Graph is\n\n";

// 			for(Var_it = listOfVariableNodes.begin(); Var_it != listOfVariableNodes.end(); Var_it++)
// 			  {
// 			    cout<<"\n\nVariable Node : "<<(*Var_it)->name<<endl;
			    
// 			    if( ((*Var_it)->list_of_edges).empty() )
// 			      {
// 				cout<<"\nNot connected to any node\n";
// 			      }
// 			    else
// 			      {
// 				cout<<"\nConnected to the following nodes\n";

// 				for(Fun_it = ((*Var_it)->list_of_edges).begin(); Fun_it != ((*Var_it)->list_of_edges).end(); Fun_it++)	
// 				  {
// 				    cout<<endl<<(*Fun_it)->name<<endl;
// 				  }
// 			      }
// 			  }*/

					
// 			/*For clone_And_Shift */

// 			map<CDAGNode*,CDAGNode*> temp8;
// 			set<string> temp9;
			
		
// 			/* Merging part */
			
// 			list<CFactorGraphNode*>::iterator k = listOfVariableNodes_ordered.begin();
// 			//for(; k != listOfVariableNodes_ordered.end(); k++)
// 			while(k != listOfVariableNodes_ordered.end())
// 			{
// 			       //cout << endl << endl << "PROCESSING ELEMENT=" << (*k)->name << endl << endl;
				
// 				int degree, degree_temp;
// 				degree = degree_temp = (*k)->list_of_edges.size();
// 				int noOfAnd = (degree-1);
				
// 				CFactorGraphNode* newFunNode = new CFactorGraphNode();
// 				set<CFactorGraphNode*> setOfVarNodes;
				
// 				CDAGNode* exp = NULL;

// 				string newName = "exists(";
// 				newName += (*k)->name;
// 				newName += ",";
				
// 				while(noOfAnd != 0)
// 				{
// 					newName += "and(";
// 					--noOfAnd;
// 				}
				
// 				list<CFactorGraphNode*>::iterator l = (*k)->list_of_edges.begin();
// 				while((degree != 0) && (l != (*k)->list_of_edges.end()))
// 				{
// 				  SetOfNewFunNodes_Unneeded.insert(*l); // Newly added

// 					 if(degree == degree_temp)
// 					{
// 						newName += (*l)->name;
// 						newName += ",";
// 						degree--;
// 					}
// 					else
// 					{
// 						newName += (*l)->name;
// 						newName += ")";
// 						newName += ",";
// 						degree--;
// 					}

// 					 /* Newly added - for creating the CDAGNode* exp corresponding to newName */ 

// 					 if(exp == NULL)
// 					   {
					     
// 					     //CDAGNode *temp6 = MANAGER.cloneAndShift((*l)->exp, 0);
// 					     //temp9.clear();
// 					     //exp = temp6;
// 					     exp = (*l)->exp;
// 					   }
// 					 else
// 					   {
// 					     //CDAGNode *temp6 = MANAGER.cloneAndShift((*l)->exp, 0);
// 					     //temp9.clear();
// 					     CDAGNode *temp6 = ((*l)->exp);
// 					     exp = MANAGER.createDAG("and", temp6, exp);
// 					   }

// 					 /* Newly added - for creating the CDAGNode* exp corresponding to newName Ends Here */ 
					
// 					list<CFactorGraphNode*>:: iterator m = (*l)->list_of_edges.begin();
// 					for(; m != (*l)->list_of_edges.end(); m++)
// 					{
// 						if(!((*k)->name == (*m)->name))
// 						{
// 							setOfVarNodes.insert(*m);
// 						}
// 					}
					
// 					++l;
// 				} 

// 				/* Newly added - for creating the "exits" part of CDAGNode* exp corresponding to newName */ 

// 				//CDAGNode* temp6 = MANAGER.createDAG((*k)->name);

// 				set<string> setExQuantifyTemp;

// 				setExQuantifyTemp.insert((*k)->name);

			
// 				CDAGNode* temp6 = getDAGFromStringSet(setExQuantifyTemp); // setExQuantifyTemp is non-empty due to the previous insert
				
				
// 				exp = MANAGER.createDAG("exists", temp6, exp);

// 				/* Newly added - for creating the "exits" part of CDAGNode* exp corresponding to newName - Ends Here */ 
				
// 				/* For displaying setOfVarNodes */

// 				/*
// 				set<CFactorGraphNode*>:: iterator y = setOfVarNodes.begin();
// 				for(; y != setOfVarNodes.end(); y++)
// 				{
// 					list<CFactorGraphNode*>:: iterator x = (*y)->list_of_edges.begin();
// 					for(; x != (*y)->list_of_edges.end(); x++)
// 					{
// 						cout << "its list_of_edges=" << (*x)->name << endl;
// 					}
// 				}
// 				*/
				
// 				newName += ")";
				
// 				//cout << endl << endl << "newName  =  " << newName << endl << endl;

// 				//cout << endl << endl << "exp  =  " << exp->getDAGStr() << endl << endl;
				
// 				newFunNode->name = newName;

// 				newFunNode->exp = exp;
				
// 				CFactorGraphNode* tempNode = (*k);
// 				k = listOfVariableNodes_ordered.erase(k);

// 				/* For displaying setOfVarNodes */
				
// 				/*
// 				set<CFactorGraphNode*>:: iterator z = setOfVarNodes.begin();
// 				for(; z != setOfVarNodes.end(); z++)
// 				{
// 					cout<< "element=" << (*z)->name << endl;
// 				}
// 				*/
				
// 				set<CFactorGraphNode*>:: iterator n = setOfVarNodes.begin();
// 				for(; n != setOfVarNodes.end(); n++)
// 				{
// 					list<CFactorGraphNode*>::iterator o = (*n)->list_of_edges.begin();
// 					while(o != (*n)->list_of_edges.end())
// 					{
// 						/*
// 						list<CFactorGraphNode*>::iterator q = (*k)->list_of_edges.begin();
// 						for(; q != (*k)->list_of_edges.end(); q++)
// 						{
// 							cout << endl<<"(*q)->name=" << (*q)->name << endl;
// 						}
// 						*/
// 						bool deleted = false;
						 
// 						list<CFactorGraphNode*>::iterator p = tempNode->list_of_edges.begin();
// 						for(; p != tempNode->list_of_edges.end(); p++)
// 						{
// 							if(deleted == false)
// 							{
// 								if((*o)->name == (*p)->name) 
// 								{
// 									if((++o) == (*n)->list_of_edges.end())
// 									{
// 										--o;
// 										--o;
// 										(*n)->list_of_edges.pop_back();	
// 									}
// 									else
// 									{
// 										--o;
// 										o = (*n)->list_of_edges.erase(o);
// 									}
// 									deleted = true;
// 								}
// 							}
// 						}
						
// 						if(deleted == false)
// 						{
// 							++o;
// 						}
// 					}
					
// 					(*n)->list_of_edges.push_back(newFunNode);
// 					newFunNode->list_of_edges.push_back(*n);
					
					
					
// 					//listOfNewFunNodes.push_back(newFunNode);
// 				}
// 				//cout << endl << endl << "newFunNode->name=" << newFunNode->name << endl << endl;
// 				listOfNewFunNodes.push_back(newFunNode);
// 			}
			
// 			/* Newly added - for debugging */

// 			//cout<<"\n\nSize of listOfNewFunNodes = "<<listOfNewFunNodes.size()<<endl<<endl;
// 			//cout<<"\n\nlistOfNewFunNodes"<<endl<<endl;

// 			//for(list<CFactorGraphNode*>::iterator list_it = listOfNewFunNodes.begin(); list_it != listOfNewFunNodes.end(); list_it++)
// 			//	  {cout<<"\n\n"<<((*list_it)->exp)->getDAGStr()<<endl<<endl;}

// 			//cout<<"\n\nlistOfNewFunNodes which are needed finally"<<endl<<endl;

// 			//for(list<CFactorGraphNode*>::iterator list_it = listOfNewFunNodes.begin(); list_it != listOfNewFunNodes.end(); list_it++)
// 			//  {if(SetOfNewFunNodes_Unneeded.find(*list_it) == SetOfNewFunNodes_Unneeded.end()) cout<<"\n\n"<<((*list_it)->exp)->getDAGStr()<<endl<<endl;}

// 			//cout << "WITH===>";

// 			//for(it = listOfFunctionNodes_floating.begin(); it != listOfFunctionNodes_floating.end(); ++it)
// 			//  {
// 			//   CFactorGraphNode* temp = (*it);
// 			//   cout<<"\n\n"<<temp->name<<"\n\n";
			  
// 			//  }
			

// 			/* Newly added - for debugging ends */
			

// 			for(it = listOfNewFunNodes.begin(); it != listOfNewFunNodes.end(); ++it)
// 			  {
// 			    if(SetOfNewFunNodes_Unneeded.find(*it) == SetOfNewFunNodes_Unneeded.end())
// 			      {
// 				if(SReducedDAG == NULL)
// 				  {
// 				    SReducedDAG = (*it)->exp;
// 				  }
// 				else
// 				  {
// 				    SReducedDAG = MANAGER.createDAG("and",(*it)->exp,SReducedDAG);
// 				  }
// 			      }
			   
// 			  }


// 			for(it = listOfFunctionNodes_floating.begin(); it != listOfFunctionNodes_floating.end(); ++it)
// 			  {
// 			    CFactorGraphNode* temp = (*it);

// 			    if(SReducedDAG == NULL)
// 			      {
// 				SReducedDAG = temp->exp;
// 			      }
// 			    else
// 			      {
// 				SReducedDAG = MANAGER.createDAG("and",temp->exp,SReducedDAG);
// 			      }
			   
// 			  }

	
// 		}
// 		else
// 		{
// 			cout << endl << endl << "As only one variable to be existentially quantified out, so 'NO SCOPE REDUCTION'" << endl <<endl;	
// 			//string existQuant = getListString(setExQuantify);
// 			//CDAGNode* temp6 = MANAGER.createDAG(existQuant);
			
// 			if(setExQuantify.empty()) // Can be empty also. Depends on the call
// 			  SReducedDAG = node;
// 			else
// 			  {
// 			    CDAGNode* temp6 = getDAGFromStringSet(setExQuantify);
// 			    SReducedDAG = MANAGER.createDAG("exists", temp6, node); 
// 			  }
// 		}

// 		if(SReducedDAG == NULL) {cout<<"\n\nERROR in function scopeReduction! Return value becomes NULL.\n"; exit(1);}

// 		return SReducedDAG;

		
// 	}


// CDAGNode* CSymbSimulator::scopeReduction_Original_On18May09(set<string> setExQuantify,CDAGNode* node)
// 	{
// 	  CDAGNode* SReducedDAG = NULL;

// 		if(setExQuantify.size() > 1)
// 		{
// 			list<CDAGNode*> listOfFunctionNodes_DAGNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes_temp;
// 			list<CFactorGraphNode*> listOfVariableNodes_ordered;
// 			list<CFactorGraphNode*> listOfFunctionNodes;
// 			list<CFactorGraphNode*> listOfNewFunNodes;
// 			list<CFactorGraphNode*> listOfFunctionNodes_floating;
// 			set<string> variableSetForFunctionNode;
			
// 			//Creating variable nodes of a factor graph
// 			set<string>:: iterator a = setExQuantify.begin();
// 			for(; a != setExQuantify.end(); a++)
// 			{
// 				CFactorGraphNode* varNode = new CFactorGraphNode();
// 				varNode->type = 0;
// 				varNode->name = (*a);
// 				listOfVariableNodes.push_back(varNode);
// 			}
			
// 			//Creating function nodes of a factor graph
// 			listOfFunctionNodesForFactorGraph(node,listOfFunctionNodes_DAGNodes);
			
// 			if(listOfFunctionNodes_DAGNodes.size() == 0)
// 			{
// 			        // cout << endl << "***********************"<< endl << endl;
// // 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// // 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// // 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// // 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// // 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// // 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// // 				cout << endl << "***********************"<< endl << endl;
// 				//string existQuant = getListString(setExQuantify);
// 				//CDAGNode* temp6 = MANAGER.createDAG(existQuant);
// 				CDAGNode* temp6 = getDAGFromStringSet(setExQuantify); // setExQuantify is non-empty here 
// 				SReducedDAG = MANAGER.createDAG("exists", temp6, node); 
// 				// cout << endl << "***********************"<< endl << endl;
// // 				cout << endl << "VALUE RETURNED"<< endl << endl;
// // 				cout << endl << "VALUE RETURNED"<< endl << endl;
// // 				cout << endl << "VALUE RETURNED"<< endl << endl;
// // 				cout << endl << "VALUE RETURNED"<< endl << endl;
// // 				cout << endl << "VALUE RETURNED"<< endl << endl;
// // 				cout << endl << "VALUE RETURNED"<< endl << endl;
// // 				cout << endl << "***********************"<< endl << endl;
// 				return SReducedDAG;	
// 			}
			
// 			// cout << endl << "***********************"<< endl << endl;
// // 			cout << endl << "SCOPE REDUCTION POSSIBLE"<< endl << endl;
// // 			cout << endl << "SCOPE REDUCTION POSSIBLE"<< endl << endl;
// // 			cout << endl << "SCOPE REDUCTION POSSIBLE"<< endl << endl;
// // 			cout << endl << "SCOPE REDUCTION POSSIBLE"<< endl << endl;
// // 			cout << endl << "SCOPE REDUCTION POSSIBLE"<< endl << endl;
// // 			cout << endl << "SCOPE REDUCTION POSSIBLE"<< endl << endl;
// // 			cout << endl << "***********************"<< endl << endl;

// 			list<CDAGNode*>:: iterator b = listOfFunctionNodes_DAGNodes.begin();
// 			for(; b != listOfFunctionNodes_DAGNodes.end(); b++)
// 			{
// 				CFactorGraphNode* funNode = new CFactorGraphNode();
// 				funNode->type = 1;
// 				funNode->name = (*b)->getDAGStr();
// 				funNode->exp = *b;
				
// 				//Creating edges in the above nodes
// 				variableSetForFunctionNode.clear();
// 				//cout << "(*b)->getDAGStr()=" << (*b)->getDAGStr() << endl;
// 				listOfVariables(*b,variableSetForFunctionNode);//Getting the set of variables in a constraint
				
// 				set<string>::iterator d = setExQuantify.begin();
// 				for(; d != setExQuantify.end(); d++)
// 				{
// 					 set<string>::iterator e = variableSetForFunctionNode.begin();
// 					for(; e != variableSetForFunctionNode.end(); e++)
// 					{
// 						if((*d) == (*e))
// 						{
// 							list<CFactorGraphNode*>::iterator f = listOfVariableNodes.begin();
// 							for(; f != listOfVariableNodes.end(); f++)
// 							{
// 								if((*e) == (*f)->name)
// 								{
// 									(*f)->list_of_edges.push_back(funNode);
// 									funNode->list_of_edges.push_back(*f);
// 								}
// 							}
// 						}
// 					}
// 				}
				
// 				listOfFunctionNodes.push_back(funNode);
// 			}


// 			/* Newly added - Finding FunctionNodes with empty list_of_edges */


				


// 			//cout<<"\n\nFunctions with empty and non-empty lists\n\n";

// 			list<CFactorGraphNode*>::iterator it;

			

// 			for(it = listOfFunctionNodes.begin(); it != listOfFunctionNodes.end(); ++it)
// 			  {
// 			    CFactorGraphNode* temp = (*it);
// 			    if( (temp->list_of_edges).empty() )
// 			      {
// 				cout<<"\n\nEmpty List :   "<<temp->name<<"\n\n";
// 				cout<<"\n\nEmpty List :   "<<(temp->exp)->getDAGStr()<<"\n\n";
// 				listOfFunctionNodes_floating.push_back(temp);
// 			      }
// 			    else
// 			      {
// 				cout<<"\n\nNon Empty List :   "<<temp->name<<"\n\n";
// 				cout<<"\n\nNon Empty List :   "<<(temp->exp)->getDAGStr()<<"\n\n";
// 			      }
// 			  }


		

// 			/* Newly added - Finding FunctionNodes with empty list_of_edges Ends here */
			      
			
			
// 			/*
// 			list<CFactorGraphNode*>::iterator c = listOfFunctionNodes.begin();
// 			for(; c != listOfFunctionNodes.end(); c++)
// 			{
// 				cout << endl << endl << "function name=" << (*c)->name << endl;
// 				cout << "list_of_edges.size()=" << (*c)->list_of_edges.size() << endl;	
// 			}
			
// 			list<CFactorGraphNode*>::iterator g = listOfVariableNodes.begin();
// 			for(; g != listOfVariableNodes.end(); g++)
// 			{
// 				cout << endl << endl << "variable name=" << (*g)->name << endl;
// 				cout << "list_of_edges.size()=" << (*g)->list_of_edges.size() << endl;	
// 			}
// 			*/


// 			// cout << endl << "***********************"<< endl << endl;
// // 			cout << endl << "HERE1"<< endl << endl;
// // 			cout << endl << "HERE1"<< endl << endl;
// // 			cout << endl << "HERE1"<< endl << endl;
// // 			cout << endl << "HERE1"<< endl << endl;
// // 			cout << endl << "HERE1"<< endl << endl;
// // 			cout << endl << "HERE1"<< endl << endl;
// // 			cout << endl << "***********************"<< endl << endl;
			
// 			listOfVariableNodes_temp = listOfVariableNodes;
			
// 			while(listOfVariableNodes_temp.size() != 0)
// 			{
// 				//cout << "listOfVariableNodes_temp.size()=" << listOfVariableNodes_temp.size() << endl;
// 				CFactorGraphNode* min = new CFactorGraphNode();
// 				bool deleted = false;
				
// 				list<CFactorGraphNode*>::iterator h = listOfVariableNodes_temp.begin();
// 				min = (*h); //Intialization
// 				for(; h != listOfVariableNodes_temp.end(); h++)
// 				{
// 					if((*h)->list_of_edges.size() <= min->list_of_edges.size())
// 					{
// 						min = (*h);
// 					}
// 				}
				
// 				//cout << "min=" << min->name << endl;
// 				listOfVariableNodes_ordered.push_back(min);
				
// 				list<CFactorGraphNode*>::iterator i = listOfVariableNodes_temp.begin();
// 				while(i != listOfVariableNodes_temp.end())
// 				{
// 					if(((*i)->name == min->name) && (deleted == false))
// 					{
// 						if((++i) == listOfVariableNodes_temp.end())
// 						{
// 							--i;
// 							--i;
// 							listOfVariableNodes_temp.pop_back();	
// 						}
// 						else
// 						{
// 							--i;
// 							i = listOfVariableNodes_temp.erase(i);	
// 						}
						
// 						deleted = true;
// 					}
// 					else
// 					{
// 						++i;
// 						//break;	
// 					}
// 				}
// 				//cout << "here3" << endl;
// 			}
			
// 			//cout << endl << endl << "listOfVariableNodes_ordered.size()=" << listOfVariableNodes_ordered.size() << endl;
// 			//cout << endl << endl << "listOfVariableNodes.size()=" << listOfVariableNodes.size() << endl;
			
// 			/*
// 			list<CFactorGraphNode*>::iterator j = listOfVariableNodes_ordered.begin();
// 			for(; j != listOfVariableNodes_ordered.end(); j++)
// 			{
// 				cout << endl << endl << "variable name=" << (*j)->name << endl;
// 				cout << "list_of_edges.size()=" << (*j)->list_of_edges.size() << endl;	
// 			}
// 			*/

// 			/* The following decls. and loop are temporary */

// 			/* Newly added - Finding FunctionNodes with empty list_of_edges */

// 			cout<<"\n\nDisplaying the Functions\n\n";

// 			for(it = listOfFunctionNodes.begin(); it != listOfFunctionNodes.end(); ++it)
// 			  {
// 			    CFactorGraphNode* temp = (*it);
// 			    cout<<"\n\nFunction :   "<<temp->name<<"\n\n";
// 			    cout<<"\n\nFunction :   "<<(temp->exp)->getDAGStr()<<"\n\n";
			   
// 			  }

// 			/* Newly added - Finding FunctionNodes with empty list_of_edges Ends here */

// 			// cout << endl << "***********************"<< endl << endl;
// // 			cout << endl << "HERE2"<< endl << endl;
// // 			cout << endl << "HERE2"<< endl << endl;
// // 			cout << endl << "HERE2"<< endl << endl;
// // 			cout << endl << "HERE2"<< endl << endl;
// // 			cout << endl << "HERE2"<< endl << endl;
// // 			cout << endl << "HERE2"<< endl << endl;
// // 			cout << endl << "***********************"<< endl << endl;
			
// 			/* Newly added - for clone_And_Shift */

// 			map<CDAGNode*,CDAGNode*> temp8;
// 			set<string> temp9;
			
// 			/* Newly added - for clone_And_Shift Ends Here */
			
// 			list<CFactorGraphNode*>::iterator k = listOfVariableNodes_ordered.begin();
// 			//for(; k != listOfVariableNodes_ordered.end(); k++)
// 			while(k != listOfVariableNodes_ordered.end())
// 			{
// 				//cout << endl << endl << "PROCESSING ELEMENT=" << (*k)->name << endl << endl;
				
// 				int degree, degree_temp;
// 				degree = degree_temp = (*k)->list_of_edges.size();
// 				int noOfAnd = (degree-1);
				
// 				CFactorGraphNode* newFunNode = new CFactorGraphNode();
// 				set<CFactorGraphNode*> setOfVarNodes;
				
// 				CDAGNode* exp = NULL;

// 				string newName = "exists(";
// 				newName += (*k)->name;
// 				newName += ",";
				
// 				while(noOfAnd != 0)
// 				{
// 					newName += "and(";
// 					--noOfAnd;
// 				}
				
// 				list<CFactorGraphNode*>::iterator l = (*k)->list_of_edges.begin();
// 				while((degree != 0) && (l != (*k)->list_of_edges.end()))
// 				{
// 					 if(degree == degree_temp)
// 					{
// 						newName += (*l)->name;
// 						newName += ",";
// 						degree--;
// 					}
// 					else
// 					{
// 						newName += (*l)->name;
// 						newName += ")";
// 						newName += ",";
// 						degree--;
// 					}

// 					 /* Newly added - for creating the CDAGNode* exp corresponding to newName */ 

// 					 if(exp == NULL)
// 					   {
					     
// 					     //CDAGNode *temp6 = MANAGER.cloneAndShift((*l)->exp, 0);
// 					     //temp9.clear();
// 					     //exp = temp6;
// 					     exp = (*l)->exp;
// 					   }
// 					 else
// 					   {
// 					     //CDAGNode *temp6 = MANAGER.cloneAndShift((*l)->exp, 0);
// 					     //temp9.clear();
// 					     CDAGNode *temp6 = ((*l)->exp);
// 					     exp = MANAGER.createDAG("and", temp6, exp);
// 					   }

// 					 /* Newly added - for creating the CDAGNode* exp corresponding to newName Ends Here */ 
					
// 					list<CFactorGraphNode*>:: iterator m = (*l)->list_of_edges.begin();
// 					for(; m != (*l)->list_of_edges.end(); m++)
// 					{
// 						if(!((*k)->name == (*m)->name))
// 						{
// 							setOfVarNodes.insert(*m);
// 						}
// 					}
					
// 					++l;
// 				} 

// 				/* Newly added - for creating the "exits" part of CDAGNode* exp corresponding to newName */ 

// 				//CDAGNode* temp6 = MANAGER.createDAG((*k)->name);

// 				set<string> setExQuantifyTemp;

// 				setExQuantifyTemp.insert((*k)->name);

			
// 				CDAGNode* temp6 = getDAGFromStringSet(setExQuantifyTemp); // setExQuantifyTemp is non-empty due to the previous insert
				
				
// 				exp = MANAGER.createDAG("exists", temp6, exp);

// 				/* Newly added - for creating the "exits" part of CDAGNode* exp corresponding to newName - Ends Here */ 
				
// 				/*
// 				set<CFactorGraphNode*>:: iterator y = setOfVarNodes.begin();
// 				for(; y != setOfVarNodes.end(); y++)
// 				{
// 					list<CFactorGraphNode*>:: iterator x = (*y)->list_of_edges.begin();
// 					for(; x != (*y)->list_of_edges.end(); x++)
// 					{
// 						cout << "its list_of_edges=" << (*x)->name << endl;
// 					}
// 				}
// 				*/
				
// 				newName += ")";
				
// 				//cout << endl << endl << "newName  =  " << newName << endl << endl;

// 				//cout << endl << endl << "exp  =  " << exp->getDAGStr() << endl << endl;
				
// 				newFunNode->name = newName;

// 				newFunNode->exp = exp;
				
// 				CFactorGraphNode* tempNode = (*k);
// 				k = listOfVariableNodes_ordered.erase(k);
				
// 				/*
// 				set<CFactorGraphNode*>:: iterator z = setOfVarNodes.begin();
// 				for(; z != setOfVarNodes.end(); z++)
// 				{
// 					cout<< "element=" << (*z)->name << endl;
// 				}
// 				*/
				
// 				set<CFactorGraphNode*>:: iterator n = setOfVarNodes.begin();
// 				for(; n != setOfVarNodes.end(); n++)
// 				{
// 					list<CFactorGraphNode*>::iterator o = (*n)->list_of_edges.begin();
// 					while(o != (*n)->list_of_edges.end())
// 					{
// 						/*
// 						list<CFactorGraphNode*>::iterator q = (*k)->list_of_edges.begin();
// 						for(; q != (*k)->list_of_edges.end(); q++)
// 						{
// 							cout << endl<<"(*q)->name=" << (*q)->name << endl;
// 						}
// 						*/
// 						bool deleted = false;
						 
// 						list<CFactorGraphNode*>::iterator p = tempNode->list_of_edges.begin();
// 						for(; p != tempNode->list_of_edges.end(); p++)
// 						{
// 							if(deleted == false)
// 							{
// 								if((*o)->name == (*p)->name) 
// 								{
// 									if((++o) == (*n)->list_of_edges.end())
// 									{
// 										--o;
// 										--o;
// 										(*n)->list_of_edges.pop_back();	
// 									}
// 									else
// 									{
// 										--o;
// 										o = (*n)->list_of_edges.erase(o);
// 									}
// 									deleted = true;
// 								}
// 							}
// 						}
						
// 						if(deleted == false)
// 						{
// 							++o;
// 						}
// 					}
					
// 					(*n)->list_of_edges.push_back(newFunNode);
// 					newFunNode->list_of_edges.push_back(*n);
					
					
					
// 					//listOfNewFunNodes.push_back(newFunNode);
// 				}
// 				//cout << endl << endl << "newFunNode->name=" << newFunNode->name << endl << endl;
// 				listOfNewFunNodes.push_back(newFunNode);
// 			}
			
// 			/* Newly added - for debugging */

// 			cout<<"\n\nSize of listOfNewFunNodes = "<<listOfNewFunNodes.size()<<endl<<endl;
// 			cout<<"\n\nlistOfNewFunNodes"<<endl<<endl;

// 			for(list<CFactorGraphNode*>::iterator list_it = listOfNewFunNodes.begin(); list_it != listOfNewFunNodes.end(); list_it++)
// 			  {cout<<"\n\n"<<((*list_it)->exp)->getDAGStr()<<endl<<endl;}

// 			/* Newly added - for debugging ends */
			
			
// 			list<CFactorGraphNode*>::iterator w = listOfNewFunNodes.end();
// 			--w;
// 			CFactorGraphNode* finalExp = (*w);
// 			cout << endl << endl << "SCOPE REDUCED EXPRESSION===>" << finalExp->name << endl << endl;
// 			cout << endl << endl << "SCOPE REDUCED EXPRESSION FROM DAG ===>" << (finalExp->exp)->getDAGStr() << endl << endl;
			
// 			cout << "WITH===>";

// 			for(it = listOfFunctionNodes_floating.begin(); it != listOfFunctionNodes_floating.end(); ++it)
// 			  {
// 			   CFactorGraphNode* temp = (*it);
// 			   cout<<"\n\n"<<temp->name<<"\n\n";
			  
// 			  }
			
// 			///* Newly added - for clone_And_Shift */

// 			//map<CDAGNode*,CDAGNode*> temp8;
// 			//set<string> temp9;
			
// 			///* Newly added - for clone_And_Shift Ends Here */

// 			// cout << endl << "***********************"<< endl << endl;
// // 			cout << endl << "HERE3"<< endl << endl;
// // 			cout << endl << "HERE3"<< endl << endl;
// // 			cout << endl << "HERE3"<< endl << endl;
// // 			cout << endl << "HERE3"<< endl << endl;
// // 			cout << endl << "HERE3"<< endl << endl;
// // 			cout << endl << "HERE3"<< endl << endl;
// // 			cout << endl << "***********************"<< endl << endl;

// 			/* Newly added - for final DAG creation */

// 			if(finalExp->exp != NULL)
// 					   {
					     
// 					     //CDAGNode *temp6 = MANAGER.cloneAndShift(finalExp->exp, 0);
// 					     //temp9.clear();
// 					     CDAGNode *temp6 = (finalExp->exp);
// 					     SReducedDAG = temp6;
// 					   }

// 			for(it = listOfFunctionNodes_floating.begin(); it != listOfFunctionNodes_floating.end(); ++it)
// 			  {
// 			    CFactorGraphNode* temp = (*it);

// 			    if(SReducedDAG == NULL)
// 			      {
// 				SReducedDAG = temp->exp;
// 			      }
// 			    else
// 			      {
// 				SReducedDAG = MANAGER.createDAG("and",temp->exp,SReducedDAG);
// 			      }
			   
// 			  }

// 			// cout << endl << "***********************"<< endl << endl;
// // 			cout << endl << "HERE4"<< endl << endl;
// // 			cout << endl << "HERE4"<< endl << endl;
// // 			cout << endl << "HERE4"<< endl << endl;
// // 			cout << endl << "HERE4"<< endl << endl;
// // 			cout << endl << "HERE4"<< endl << endl;
// // 			cout << endl << "HERE4"<< endl << endl;
// // 			cout << endl << "***********************"<< endl << endl;


// 		}
// 		else
// 		{
// 			cout << endl << endl << "As only one variable to be existentially quantified out, so 'NO SCOPE REDUCTION'" << endl <<endl;	
// 			//string existQuant = getListString(setExQuantify);
// 			//CDAGNode* temp6 = MANAGER.createDAG(existQuant);
			
// 			if(setExQuantify.empty()) // Can be empty also. Depends on the call
// 			  SReducedDAG = node;
// 			else
// 			  {
// 			    CDAGNode* temp6 = getDAGFromStringSet(setExQuantify);
// 			    SReducedDAG = MANAGER.createDAG("exists", temp6, node); 
// 			  }
// 		}

// 		return SReducedDAG;

// 		/* Newly added - for final DAG creation Ends Here */
// 	}


// /*******************************************************************************************************************
// 								Functions for Scope Reduction over
// ********************************************************************************************************************/

// /*******************************************************************************************************************
// 								Functions for Scope Reduction (2)
// ********************************************************************************************************************/

// 	void CSymbSimulator::scopeReduction2(set<string> setExQuantify,CDAGNode* node)
// 	{
// 		if(setExQuantify.size() > 1)
// 		{
// 			bool marked = false,node_marked = false;
// 			int count = 0;	
	    
// 			list<CDAGNode*> listOfFunctionNodes_DAGNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes_temp;
// 			list<CFactorGraphNode*> listOfVariableNodes_ordered;
// 			list<CFactorGraphNode*> listOfFunctionNodes;
// 			list<CFactorGraphNode*> listOfNewFunNodes;
// 			set<string> variableSetForFunctionNode;
		
// 		//Creating variable nodes of a factor graph
// 			set<string>:: iterator a = setExQuantify.begin();
// 			for(; a != setExQuantify.end(); a++)
// 			{
// 				CFactorGraphNode* varNode = new CFactorGraphNode();
// 				varNode->type = 0;
// 				varNode->name = (*a);
// 				listOfVariableNodes.push_back(varNode);
// 			}
			
// 			//Creating function nodes of a factor graph
// 			listOfFunctionNodesForFactorGraph(node,listOfFunctionNodes_DAGNodes);
			
// 			if(listOfFunctionNodes_DAGNodes.size() == 0)
// 			{
// 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// 				return;	
// 			}
			
// 			list<CDAGNode*>:: iterator b = listOfFunctionNodes_DAGNodes.begin();
// 			for(; b != listOfFunctionNodes_DAGNodes.end(); b++)
// 			{
// 				CFactorGraphNode* funNode = new CFactorGraphNode();
// 				funNode->type = 1;
// 				funNode->name = (*b)->getDAGStr();
				
// 				//Creating edges in the above nodes
// 				variableSetForFunctionNode.clear();
// 				//cout << "(*b)->getDAGStr()=" << (*b)->getDAGStr() << endl;
// 				listOfVariables(*b,variableSetForFunctionNode);//Getting the set of variables in a constraint
				
// 				set<string>::iterator d = setExQuantify.begin();
// 				for(; d != setExQuantify.end(); d++)
// 				{
// 					 set<string>::iterator e = variableSetForFunctionNode.begin();
// 					for(; e != variableSetForFunctionNode.end(); e++)
// 					{
// 						if((*d) == (*e))
// 						{
// 							list<CFactorGraphNode*>::iterator f = listOfVariableNodes.begin();
// 							for(; f != listOfVariableNodes.end(); f++)
// 							{
// 								if((*e) == (*f)->name)
// 								{
// 									(*f)->list_of_edges.push_back(funNode);
// 									funNode->list_of_edges.push_back(*f);
// 								}
// 							}
// 						}
// 					}
// 				}
				
// 				listOfFunctionNodes.push_back(funNode);
// 			}
		
// 			/*
// 			list<CFactorGraphNode*>::iterator c = listOfFunctionNodes.begin();
// 			for(; c != listOfFunctionNodes.end(); c++)
// 			{
// 				cout << endl << endl << "function name=" << (*c)->name << endl;
// 				cout << "list_of_edges.size()=" << (*c)->list_of_edges.size() << endl;	
// 			}
			
// 			list<CFactorGraphNode*>::iterator g = listOfVariableNodes.begin();
// 			for(; g != listOfVariableNodes.end(); g++)
// 			{
// 				cout << endl << endl << "variable name=" << (*g)->name << endl;
// 				cout << "list_of_edges.size()=" << (*g)->list_of_edges.size() << endl;	
// 			}
// 			*/
			
// 			listOfVariableNodes_temp = listOfVariableNodes;
			
// 			//Get the value of "count" for each node in listOfVariableNodes;
// 			list<CFactorGraphNode*>::iterator outer = listOfVariableNodes_temp.begin();
// 			for(; outer != listOfVariableNodes_temp.end(); outer++)
// 			{
// 				count = 0;	
// 				marked = false;
				
// 				list<CFactorGraphNode*>::iterator inner = listOfVariableNodes_temp.begin();
// 				for(; inner != listOfVariableNodes_temp.end(); inner++)
// 				{
// 					if((*outer)->name == (*inner)->name)
// 					{
// 						continue;
// 					}
// 					else 
// 					{
// 						list<CFactorGraphNode*>::iterator edges_outer = (*outer)->list_of_edges.begin();
// 						for(; edges_outer != (*outer)->list_of_edges.end(); edges_outer++)
// 						{
// 							node_marked = false;
// 							list<CFactorGraphNode*>::iterator edges_inner = (*inner)->list_of_edges.begin();
// 							for(; edges_inner != (*inner)->list_of_edges.end(); edges_inner++)
// 							{
// 								if(((*edges_outer)->name == (*edges_inner)->name) && (node_marked == false))
// 								{
// 									count++;
// 									node_marked = true;
// 									marked = true;	
// 								}
// 							}
// 						}
// 					}
// 				}	
				
// 				if(marked == true)
// 				{
// 					(*outer)->count = count;
// 				}				
// 			}
		
		
		
		
// 			while(listOfVariableNodes_temp.size() != 0)
// 			{
// 				//cout << "listOfVariableNodes_temp.size()=" << listOfVariableNodes_temp.size() << endl;
// 				CFactorGraphNode* min = new CFactorGraphNode();
// 				bool deleted = false;
				
// 				list<CFactorGraphNode*>::iterator h = listOfVariableNodes_temp.begin();
// 				min = (*h); //Intialization
// 				for(; h != listOfVariableNodes_temp.end(); h++)
// 				{
// 					if((*h)->count <= min->count)
// 					{
// 						min = (*h);
// 					}
// 				}
				
// 				//cout << "min=" << min->name << endl;
// 				listOfVariableNodes_ordered.push_back(min);
				
// 				list<CFactorGraphNode*>::iterator i = listOfVariableNodes_temp.begin();
// 				while(i != listOfVariableNodes_temp.end())
// 				{
// 					if(((*i)->name == min->name) && (deleted == false))
// 					{
// 						if((++i) == listOfVariableNodes_temp.end())
// 						{
// 							--i;
// 							--i;
// 							listOfVariableNodes_temp.pop_back();	
// 						}
// 						else
// 						{
// 							--i;
// 							i = listOfVariableNodes_temp.erase(i);	
// 						}
						
// 						deleted = true;
// 					}
// 					else
// 					{
// 						++i;
// 						//break;	
// 					}
// 				}
// 				//cout << "here3" << endl;
// 			}
		
// 			//cout << endl << endl << "listOfVariableNodes_ordered.size()=" << listOfVariableNodes_ordered.size() << endl;
// 			//cout << endl << endl << "listOfVariableNodes.size()=" << listOfVariableNodes.size() << endl;
			
// 			/*
// 			list<CFactorGraphNode*>::iterator j = listOfVariableNodes_ordered.begin();
// 			for(; j != listOfVariableNodes_ordered.end(); j++)
// 			{
// 				cout << endl << endl << "variable name=" << (*j)->name << endl;
// 				cout << "count=" << (*j)->count << endl;	
// 			}
// 			*/
			
// 			list<CFactorGraphNode*>::iterator k = listOfVariableNodes_ordered.begin();
// 			//for(; k != listOfVariableNodes_ordered.end(); k++)
// 			while(k != listOfVariableNodes_ordered.end())
// 			{
// 				//cout << endl << endl << "PROCESSING ELEMENT=" << (*k)->name << endl << endl;
				
// 				int degree, degree_temp;
// 				degree = degree_temp = (*k)->list_of_edges.size();
// 				int noOfAnd = (degree-1);
				
// 				CFactorGraphNode* newFunNode = new CFactorGraphNode();
// 				set<CFactorGraphNode*> setOfVarNodes;
				
// 				string newName = "exists(";
// 				newName += (*k)->name;
// 				newName += ",";
				
// 				while(noOfAnd != 0)
// 				{
// 					newName += "and(";
// 					--noOfAnd;
// 				}
				
// 				list<CFactorGraphNode*>::iterator l = (*k)->list_of_edges.begin();
// 				while((degree != 0) && (l != (*k)->list_of_edges.end()))
// 				{
// 					 if(degree == degree_temp)
// 					{
// 						newName += (*l)->name;
// 						newName += ",";
// 						degree--;
// 					}
// 					else
// 					{
// 						newName += (*l)->name;
// 						newName += ")";
// 						newName += ",";
// 						degree--;
// 					}
					
// 					list<CFactorGraphNode*>:: iterator m = (*l)->list_of_edges.begin();
// 					for(; m != (*l)->list_of_edges.end(); m++)
// 					{
// 						if(!((*k)->name == (*m)->name))
// 						{
// 							setOfVarNodes.insert(*m);
// 						}
// 					}
					
// 					++l;
// 				} 
				
// 				/*
// 				set<CFactorGraphNode*>:: iterator y = setOfVarNodes.begin();
// 				for(; y != setOfVarNodes.end(); y++)
// 				{
// 					list<CFactorGraphNode*>:: iterator x = (*y)->list_of_edges.begin();
// 					for(; x != (*y)->list_of_edges.end(); x++)
// 					{
// 						cout << "its list_of_edges=" << (*x)->name << endl;
// 					}
// 				}
// 				*/
				
// 				newName += ")";
				
// 				//cout << endl << endl << "newName=" << newName << endl << endl;
				
// 				newFunNode->name = newName;
				
// 				CFactorGraphNode* tempNode = (*k);
// 				k = listOfVariableNodes_ordered.erase(k);
				
// 				/*
// 				set<CFactorGraphNode*>:: iterator z = setOfVarNodes.begin();
// 				for(; z != setOfVarNodes.end(); z++)
// 				{
// 					cout<< "element=" << (*z)->name << endl;
// 				}
// 				*/
				
// 				set<CFactorGraphNode*>:: iterator n = setOfVarNodes.begin();
// 				for(; n != setOfVarNodes.end(); n++)
// 				{
// 					list<CFactorGraphNode*>::iterator o = (*n)->list_of_edges.begin();
// 					while(o != (*n)->list_of_edges.end())
// 					{
// 						/*
// 						list<CFactorGraphNode*>::iterator q = (*k)->list_of_edges.begin();
// 						for(; q != (*k)->list_of_edges.end(); q++)
// 						{
// 							cout << endl<<"(*q)->name=" << (*q)->name << endl;
// 						}
// 						*/
// 						bool deleted = false;
						 
// 						list<CFactorGraphNode*>::iterator p = tempNode->list_of_edges.begin();
// 						for(; p != tempNode->list_of_edges.end(); p++)
// 						{
// 							if(deleted == false)
// 							{
// 								if((*o)->name == (*p)->name) 
// 								{
// 									if((++o) == (*n)->list_of_edges.end())
// 									{
// 										--o;
// 										--o;
// 										(*n)->list_of_edges.pop_back();	
// 									}
// 									else
// 									{
// 										--o;
// 										o = (*n)->list_of_edges.erase(o);
// 									}
// 									deleted = true;
// 								}
// 							}
// 						}
						
// 						if(deleted == false)
// 						{
// 							++o;
// 						}
// 					}
					
// 					(*n)->list_of_edges.push_back(newFunNode);
// 					newFunNode->list_of_edges.push_back(*n);
					
					
					
// 					//listOfNewFunNodes.push_back(newFunNode);
// 				}
// 				//cout << endl << endl << "newFunNode->name=" << newFunNode->name << endl << endl;
// 				listOfNewFunNodes.push_back(newFunNode);
// 			}
			
// 			list<CFactorGraphNode*>::iterator w = listOfNewFunNodes.end();
// 			--w;
// 			CFactorGraphNode* finalExp = (*w);
// 			cout << endl << endl << "SCOPE REDUCED EXPRESSION (2)===>" << finalExp->name << endl << endl;
// 		}
// 		else
// 		{
// 			cout << endl << endl << "As only one variable to be existentially quantified out, so 'NO SCOPE REDUCTION'" << endl <<endl;	
// 		}
// 	}
	
// /**********************************************************************************************************************************
//  *                                   Function for Scope reduction 2 OVER 
//  * ********************************************************************************************************************************/
 
 /*******************************************************************************************************************
								Functions for Scope Reduction (3)
********************************************************************************************************************/
// 	void CSymbSimulator::scopeReduction3(set<string> setExQuantify,CDAGNode* node)
// 	{
// 		if(setExQuantify.size() > 1)
// 		{
// 			list<CDAGNode*> listOfFunctionNodes_DAGNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes;
// 			list<CFactorGraphNode*> listOfVariableNodes_temp;
// 			list<CFactorGraphNode*> listOfVariableNodes_ordered;
// 			list<CFactorGraphNode*> listOfFunctionNodes;
// 			list<CFactorGraphNode*> listOfNewFunNodes;
// 			set<string> variableSetForFunctionNode;
			
// 			//Creating variable nodes of a factor graph
// 			set<string>:: iterator a = setExQuantify.begin();
// 			for(; a != setExQuantify.end(); a++)
// 			{
// 				CFactorGraphNode* varNode = new CFactorGraphNode();
// 				varNode->type = 0;
// 				varNode->name = (*a);
// 				listOfVariableNodes.push_back(varNode);
// 			}
			
// 			//Creating function nodes of a factor graph
// 			listOfFunctionNodesForFactorGraph(node,listOfFunctionNodes_DAGNodes);
			
// 			if(listOfFunctionNodes_DAGNodes.size() == 0)
// 			{
// 				cout << endl << "SCOPE REDUCTION NOT POSSIBLE"<< endl << endl;
// 				return;	
// 			}
			
// 			list<CDAGNode*>:: iterator b = listOfFunctionNodes_DAGNodes.begin();
// 			for(; b != listOfFunctionNodes_DAGNodes.end(); b++)
// 			{
// 				CFactorGraphNode* funNode = new CFactorGraphNode();
// 				funNode->type = 1;
// 				funNode->name = (*b)->getDAGStr();
				
// 				//Creating edges in the above nodes
// 				variableSetForFunctionNode.clear();
// 				//cout << "(*b)->getDAGStr()=" << (*b)->getDAGStr() << endl;
// 				listOfVariables(*b,variableSetForFunctionNode);//Getting the set of variables in a constraint
				
// 				set<string>::iterator d = setExQuantify.begin();
// 				for(; d != setExQuantify.end(); d++)
// 				{
// 					 set<string>::iterator e = variableSetForFunctionNode.begin();
// 					for(; e != variableSetForFunctionNode.end(); e++)
// 					{
// 						if((*d) == (*e))
// 						{
// 							list<CFactorGraphNode*>::iterator f = listOfVariableNodes.begin();
// 							for(; f != listOfVariableNodes.end(); f++)
// 							{
// 								if((*e) == (*f)->name)
// 								{
// 									(*f)->list_of_edges.push_back(funNode);
// 									funNode->list_of_edges.push_back(*f);
// 								}
// 							}
// 						}
// 					}
// 				}
				
// 				listOfFunctionNodes.push_back(funNode);
// 			}

		
// 			/*
// 			list<CFactorGraphNode*>::iterator c = listOfFunctionNodes.begin();
// 			for(; c != listOfFunctionNodes.end(); c++)
// 			{
// 				cout << endl << endl << "function name=" << (*c)->name << endl;
// 				cout << "list_of_edges.size()=" << (*c)->list_of_edges.size() << endl;	
// 			}
			
// 			list<CFactorGraphNode*>::iterator g = listOfVariableNodes.begin();
// 			for(; g != listOfVariableNodes.end(); g++)
// 			{
// 				cout << endl << endl << "variable name=" << (*g)->name << endl;
// 				cout << "list_of_edges.size()=" << (*g)->list_of_edges.size() << endl;	
// 			}
// 			*/
			
// 			listOfVariableNodes_temp = listOfVariableNodes;
			
// 			while(listOfVariableNodes_temp.size() >= 1)
// 			{
// 				//Order the variables
// 				listOfVariableNodes_ordered.clear();
// 				while(listOfVariableNodes_temp.size() != 0)
// 				{
// 					//cout << "listOfVariableNodes_temp.size()=" << listOfVariableNodes_temp.size() << endl;
// 					CFactorGraphNode* min = new CFactorGraphNode();
// 					bool deleted = false;
				
// 					list<CFactorGraphNode*>::iterator h = listOfVariableNodes_temp.begin();
// 					min = (*h); //Intialization
// 					for(; h != listOfVariableNodes_temp.end(); h++)
// 					{
// 						if((*h)->list_of_edges.size() <= min->list_of_edges.size())
// 						{
// 							min = (*h);
// 						}
// 					}
				
// 					//cout << "min=" << min->name << endl;
// 					listOfVariableNodes_ordered.push_back(min);
				
// 					list<CFactorGraphNode*>::iterator i = listOfVariableNodes_temp.begin();
// 					while(i != listOfVariableNodes_temp.end())
// 					{
// 						if(((*i)->name == min->name) && (deleted == false))
// 						{
// 							if((++i) == listOfVariableNodes_temp.end())
// 							{
// 								--i;
// 								--i;
// 								listOfVariableNodes_temp.pop_back();	
// 							}
// 							else
// 							{
// 								--i;
// 								i = listOfVariableNodes_temp.erase(i);	
// 							}
						
// 							deleted = true;
// 						}
// 						else
// 						{
// 							++i;
// 							//break;	
// 						}
// 					}
// 					//cout << "here3" << endl;
// 				}
			
// 				//cout << endl << endl << "listOfVariableNodes_ordered.size()=" << listOfVariableNodes_ordered.size() << endl;
// 				//cout << endl << endl << "listOfVariableNodes_temp.size()=" << listOfVariableNodes_temp.size() << endl;
				
// 				/*
// 				list<CFactorGraphNode*>::iterator j = listOfVariableNodes_ordered.begin();
// 				for(; j != listOfVariableNodes_ordered.end(); j++)
// 				{
// 					cout << endl << endl << "variable name=" << (*j)->name << endl;
// 					cout << "list_of_edges.size()=" << (*j)->list_of_edges.size() << endl;	
// 				}
// 				*/
			
// 				list<CFactorGraphNode*>::iterator k = listOfVariableNodes_ordered.begin();
// 				//for(; k != listOfVariableNodes_ordered.end(); k++)
// 				while(k != listOfVariableNodes_ordered.end())
// 				{
// 					//cout << endl << endl << "PROCESSING ELEMENT=" << (*k)->name << endl << endl;
// 					bool already_marked = false;
// 					list<CFactorGraphNode*>::iterator ab = (*k)->list_of_edges.begin();
// 					for(; ab != (*k)->list_of_edges.end(); ab++)
// 					{
// 						if((*ab)->marked == true)
// 						{
// 							already_marked = true;
// 						}
// 					}
					
// 					//cout << endl << endl << "already_marked == " << already_marked << endl << endl;
// 					if(already_marked == false)
// 					{
// 						int degree, degree_temp;
// 						degree = degree_temp = (*k)->list_of_edges.size();
// 						int noOfAnd = (degree-1);
						
// 						CFactorGraphNode* newFunNode = new CFactorGraphNode();
// 						set<CFactorGraphNode*> setOfVarNodes;
						
// 						string newName = "exists(";
// 						newName += (*k)->name;
// 						newName += ",";
						
// 						while(noOfAnd != 0)
// 						{
// 							newName += "and(";
// 							--noOfAnd;
// 						}
						
// 						list<CFactorGraphNode*>::iterator l = (*k)->list_of_edges.begin();
// 						while((degree != 0) && (l != (*k)->list_of_edges.end()))
// 						{
// 							 if(degree == degree_temp)
// 							{
// 								newName += (*l)->name;
// 								newName += ",";
// 								degree--;
// 							}
// 							else
// 							{
// 								newName += (*l)->name;
// 								newName += ")";
// 								newName += ",";
// 								degree--;
// 							}
							
// 							list<CFactorGraphNode*>:: iterator m = (*l)->list_of_edges.begin();
// 							for(; m != (*l)->list_of_edges.end(); m++)
// 							{
// 								if(!((*k)->name == (*m)->name))
// 								{
// 									setOfVarNodes.insert(*m);
// 								}
// 							}
							
// 							++l;
// 						} 
						
// 						/*
// 						set<CFactorGraphNode*>:: iterator y = setOfVarNodes.begin();
// 						for(; y != setOfVarNodes.end(); y++)
// 						{
// 							list<CFactorGraphNode*>:: iterator x = (*y)->list_of_edges.begin();
// 							for(; x != (*y)->list_of_edges.end(); x++)
// 							{
// 								cout << "its list_of_edges=" << (*x)->name << endl;
// 							}
// 						}
// 						*/
						
// 						newName += ")";
						
// 						//cout << endl << endl << "newName=" << newName << endl << endl;
						
// 						newFunNode->name = newName;
// 						newFunNode->marked = true; //IMPORTANT:::Indicates that newly created node is marked
						
// 						CFactorGraphNode* tempNode = (*k);
// 						k = listOfVariableNodes_ordered.erase(k);
						
// 						/*
// 						set<CFactorGraphNode*>:: iterator z = setOfVarNodes.begin();
// 						for(; z != setOfVarNodes.end(); z++)
// 						{
// 							cout<< "element=" << (*z)->name << endl;
// 						}
// 						*/
						
// 						set<CFactorGraphNode*>:: iterator n = setOfVarNodes.begin();
// 						for(; n != setOfVarNodes.end(); n++)
// 						{
// 							list<CFactorGraphNode*>::iterator o = (*n)->list_of_edges.begin();
// 							while(o != (*n)->list_of_edges.end())
// 							{
// 								/*
// 								list<CFactorGraphNode*>::iterator q = (*k)->list_of_edges.begin();
// 								for(; q != (*k)->list_of_edges.end(); q++)
// 								{
// 									cout << endl<<"(*q)->name=" << (*q)->name << endl;
// 								}
// 								*/
// 								bool deleted = false;
								 
// 								list<CFactorGraphNode*>::iterator p = tempNode->list_of_edges.begin();
// 								for(; p != tempNode->list_of_edges.end(); p++)
// 								{
// 									if(deleted == false)
// 									{
// 										if((*o)->name == (*p)->name) 
// 										{
// 											if((++o) == (*n)->list_of_edges.end())
// 											{
// 												--o;
// 												--o;
// 												(*n)->list_of_edges.pop_back();	
// 											}
// 											else
// 											{
// 												--o;
// 												o = (*n)->list_of_edges.erase(o);
// 											}
// 											deleted = true;
// 										}
// 									}
// 								}
								
// 								if(deleted == false)
// 								{
// 									++o;
// 								}
// 							}
							
// 							(*n)->list_of_edges.push_back(newFunNode);
// 							newFunNode->list_of_edges.push_back(*n);
							
							
							
// 							//listOfNewFunNodes.push_back(newFunNode);
// 						}
// 						//cout << endl << endl << "newFunNode->name=" << newFunNode->name << endl << endl;
// 						listOfNewFunNodes.push_back(newFunNode);
// 					}
// 					else
// 					{
// 						k++;	
// 					}
// 				}
				
// 				listOfVariableNodes_temp = listOfVariableNodes_ordered;
// 				//Again set "marked=false" for next iteration
// 				list<CFactorGraphNode*>:: iterator bc = listOfVariableNodes_temp.begin();
// 				for(; bc != listOfVariableNodes_temp.end(); bc++)
// 				{
// 					list<CFactorGraphNode*>:: iterator cd = (*bc)->list_of_edges.begin();
// 					for(; cd != (*bc)->list_of_edges.end(); cd++)
// 					{
// 						(*cd)->marked = false;		
// 					}
// 				}
// 			}
			
// 			list<CFactorGraphNode*>::iterator w = listOfNewFunNodes.end();
// 			--w;
// 			CFactorGraphNode* finalExp = (*w);
// 			cout << endl << endl << "SCOPE REDUCED EXPRESSION (3)===>" << finalExp->name << endl << endl;
// 		}
// 		else
// 		{
// 			cout << endl << endl << "As only one variable to be existentially quantified out, so 'NO SCOPE REDUCTION'" << endl <<endl;	
// 		}
// 	}

/**********************************************************************************************************************************
 *                                   Function for Scope reduction 3 OVER 
 * ********************************************************************************************************************************/




	void CSymbSimulator::createTableOverall(CEntity& ent, map<string, ConditionsValues> &SymbolicQueues, 
						set<CSensitiveElement*>& output_identifiers,
						set<CSensitiveElement*>& input_identifiers,
						set<CSensitiveElement*>& intermediate_identifiers,
						map<string, list<ConditionRelation*> >& mapSignalDelta,
						map<string, FinalRelationWithDelta*> & final_expressions)

	{
	  clock_t start_time;

	  //cout<<"\n\ncreateTableOverall Started\n\n";

	  map<string, FinalRelationWithDelta*>::iterator  final_expressions_it;

	  //cout<<"\n\nFinal Expression contains\n\n";

	  //for(final_expressions_it = final_expressions.begin(); final_expressions_it != final_expressions.end(); final_expressions_it++)
	    
	  //  {
	  //    string element = (*final_expressions_it).first;
	  //    cout<<"\t"<<element;
	  //  }

	  //cout<<"\n\nFinal Expression is Finished\n\n";

	  
	  
	 set<CSensitiveElement*>:: iterator iout = output_identifiers.begin();

		// creating table for each output
	for(;iout != output_identifiers.end(); iout++)
		{
		        

			ConditionsValues &cv = SymbolicQueues[(*iout)->name];

			vector<int>::iterator  idel = cv.delta.begin();
			vector<CDAGNode*>::iterator ival = cv.values.begin();
			vector<CDAGNode*>::iterator  icond = cv.conditions.begin();
			vector< set<string> >::iterator isetc = cv.setConditions.begin();
			
					
			int i = cv.delta.size()-1;

			int delta_time = cv.delta[i];

			CDAGNode* present_condition = cv.conditions[i];

			string id_delta = cv.values[i]->getDAGStr();

			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index); // id is now x
			int delta = atoi(id_delta.substr(index+1).c_str()); // Note that delta = delta_time

			string needed_output = id;

			//cout<<"\n\nNeeded Output = "<<id<<endl<<endl;

			if(final_expressions.find(id) == final_expressions.end())
				{
				  //cout<<"\n\nOutput "<<id<<" is not connected to any input \n\n";
				  continue;
				 }

			FinalRelationWithDelta* fr = final_expressions[id];
	
			map<CDAGNode*,CDAGNode*> temp1;
			set<string> temp2;

			CDAGNode* final_dag = fr->final_relation; 

			//cout<<"\n\nFINAL DAG = "<<final_dag->getDAGStr()<<endl<<endl;

			//cout<<"\n\nFinding the variables whose constraints are to be added\n\n";
			
			set<string> setCondRel;
			(fr->final_relation)->getDAGNames(setCondRel);
			
			set<string> setDone;
			
			while(!setCondRel.empty())
			  {			        
				string element = *(setCondRel.begin());

				//cout<<"\n\nElement = "<<element<<endl<<endl;

				setCondRel.erase(element);

				index = element.find_last_of("_");
				id = element.substr(0, index); 
				delta = atoi(element.substr(index+1).c_str()); 

				//if(delta < 2) continue; // No need not addd constraints for elements with delta time at 0 and 1
				// This is added temporarily. Should be removed after allowing -ve deltas

				if(needed_output == id) continue;
				
				if(!is_intermediate(element, intermediate_identifiers))
				{
				  continue;
			
				}
				
				if(final_expressions.find(id) == final_expressions.end())
				{
				 continue;
				}

				//if(!setDone.empty())
				//  {
				//    if(!(setDone.find(id) == setDone.end()))
				//      {
				//	continue;
				//      }
				//  }

				if(findElementWithLargerOrSameDelta(setDone, element))
				  {
				    //cout<<"\n\nElement With Larger Or Same Delta Already Exists\n\n";
				    continue;
				  }
				//setDone.insert(id);
				
				//setDone.insert(element);

				addElementToSetDone(setDone, element);
				
				FinalRelationWithDelta* fr_ele = final_expressions[id];

				int shift =  delta - fr_ele->delta;

				//cout<<"\n\nShift for "<<element<<" is "<<shift;

				CDAGNode *newConstraint = MANAGER.cloneAndShift(fr_ele->final_relation, shift);

				temp1.clear();

				//cout<<"\n\nNew Constraint is "<<newConstraint->getDAGStr()<<endl<<endl;

				set<string> setNew, setNewReduced;
				set<string>::iterator setNew_it;
			
				newConstraint->getDAGNames(setNew);
				
				
				//if(final_dag == NULL)
				//  final_dag = newConstraint;
				//else
				//  final_dag = MANAGER.createDAG("and", final_dag, newConstraint);
				
				//cout<<"\n\nfinal_dag is "<<final_dag->getDAGStr()<<endl<<endl;

				for(setNew_it = setNew.begin(); setNew_it != setNew.end(); setNew_it++)

				  	{
					  string new_element = *(setNew_it);
					  //cout<<"\n\nnew_element = "<<new_element<<endl<<endl;
					  if(!(setCondRel.find(new_element) == setCondRel.end()))
					    {
					      //cout<<"\n\nAlready added\n\n";
					     }
					  else
					    {
					      //int new_index = new_element.find_last_of("_");
					      //string new_id = new_element.substr(0, new_index); 
					      //if(!(setDone.find(new_id) == setDone.end()))
					      //	{
					      //	  cout<<"\n\nAlready added\n\n";
					      //	}
					      //else
					      //	{
					      //	  cout<<"\n\nNew One\n\n";
					      //	  setNewReduced.insert(new_element);
					      //	}

					      if(findElementWithLargerOrSameDelta(setDone, new_element))
					      	{
					      	  //cout<<"\n\nElement With Larger Or Same Delta Already Exists\n\n";
					      	}
					      else
					      	{
					      	  //cout<<"\n\nNew One\n\n";
					      	  setNewReduced.insert(new_element);
						}

					    }

					}

				//cout<<"\n\nsetNewReduced Computed\n\n";

				setCondRel.insert(setNewReduced.begin(), setNewReduced.end());

				//cout<<"\n\nsetNew added to setCondRel\n\n";
			
				//cout<<final_dag->getDAGStr()<<endl;
				
				//set<string> setDiffer;

  				//set_difference(fr->setFinalRelation.begin(), fr->setFinalRelation.end(), setDone.begin(), setDone.end(),inserter(setDiffer, setDiffer.begin()));
                 				
				//cout<<fr->setFinalRelation.size()<<endl;
                 
				//cout<<setCondRel.size()<<endl;
                 
                 				
				//setCondRel.insert(setDiffer.begin(), setDiffer.end());
                 
 				//cout<<setCondRel.size()<<endl;
                 
				/*set<string>::iterator iset = fr->setFinalRelation.begin();
				for(; iset != fr->setFinalRelation.end(); iset++)
				{
					if(setDone.find() != s)
				}*/
			  }

			//cout<<"\n\nLoop for finding the variables whose constraints are to be added\n\n";

			//cout<<"\n\nLoop for adding additional constraints starting\n\n";
			
			for(set<string>::iterator setDone_it = setDone.begin(); setDone_it != setDone.end(); setDone_it++)

				  	{
					  string element = *(setDone_it);
					  //cout<<"\n\nelement = "<<element<<endl<<endl;
					  index = element.find_last_of("_");
					  id = element.substr(0, index); 
					  delta = atoi(element.substr(index+1).c_str()); 
					  
					  FinalRelationWithDelta* fr_ele = final_expressions[id];
					  int shift =  delta - fr_ele->delta;
					  //cout<<"\n\nShift for "<<element<<" is "<<shift;
					  
					  CDAGNode *newConstraint = MANAGER.cloneAndShift(fr_ele->final_relation, shift);
					  temp1.clear();
					  
					  //cout<<"\n\nNew Constraint is "<<newConstraint->getDAGStr()<<endl<<endl;
					  
					  if(final_dag == NULL)
					      final_dag = newConstraint;
					  else
					      final_dag = MANAGER.createDAG("and", final_dag, newConstraint);
									  
					}

			//cout<<"\n\nLoop for adding additional constraints over\n\n";

			if(ent.getName() == top_level_entity)
			  {
			    //cout<<"\n\nAdding additional constraints on inputs for the top-level entity\n\n";

			    //final_dag = addAdditionalConstraints(final_dag, input_identifiers);

			    //cout<<"\n\nDAG After Adding additional constraints on inputs for the top-level entity \n"<<final_dag->getDAGStr()<<"\n\n";
			    final_dag = final_dag; // No additional constraints are needed
			    
			  }

			//cout<<"\n\nShifting final_dag by " << -1*delta_time <<"\n\n";

			//cout<<"\n\nFinal DAG before shifting == "<<final_dag->getDAGStr()<<endl<<endl;

			//cout<<"\n\nShifting final_dag by " << -1*delta_time <<"\n\n";

			CDAGNode* final_shifted_dag = MANAGER.cloneAndShift(final_dag, -1*delta_time);

			temp1.clear();

			//cout<<"\n\nFinal DAG after shifting == "<<final_shifted_dag->getDAGStr()<<endl<<endl;

			// Getting all variables in final_shifted_dag

			if(qelimination_on==1 && Slicing==0) // This is the mode in which we are doing quantifier elimination along with symbolic simulation
			  {

				set<string> setVars, setExQuantify, setExQuantifySecond;
				set<string>::iterator setVars_it, setExQuantify_it;

				if(final_shifted_dag == NULL) {
				  //cout<<"\n\nFINAL DAG NULL\n\n";
				  exit(1); }

				final_shifted_dag->getDAGNames(setVars);

				//cout<<"\n\nFINAL DAG IS :\t"<<final_shifted_dag->getDAGStr()<<endl<<endl;

				//cout<<"\n\nLIST OF ELEMENTS\n\n";

				//for(setVars_it = setVars.begin(); setVars_it != setVars.end(); setVars_it++)

				//  	{
				//	  string element = *(setVars_it);
				//	  cout<<"\n\nelement = "<<element<<endl<<endl;
				//	}
				
				//cout<<"\n\n ELEMENTS\n\n";

				for(setVars_it = setVars.begin(); setVars_it != setVars.end(); setVars_it++)

				  	{
					  string element = *(setVars_it);

					  //cout<<"\n\nelement = "<<element<<endl<<endl;

					  if(is_intermediate(element, intermediate_identifiers))
						{
						  //cout<<"\nIntermediate\n";
						  setExQuantify.insert(element);				
						}
					}

				setVars.clear();

				//cout<<"\n\nLoop for obtaining ex.variables over\n\n";

				string existQuant = getListString(setExQuantify);

				//string existQuant = "Hi"; // Temp
			
				//cout <<"\nEx.Quant. List\n";

				//cout<<existQuant;

				
				/* CODE ADDED ON 2nd Feb 09 STRATS HERE */

				
				if(!setExQuantify.empty())
				
				  {
				    //cout<<"\nVARIABLES TO ELIMINATE = "<<existQuant<<endl;
				    final_shifted_dag = MANAGER.createDAG("exists", getDAGFromStringSet(setExQuantify), final_shifted_dag);
				    
				  }
				else
				  {
				    //cout<<"\nNO VARIABLES TO ELIMINATE = "<<endl;
				  }
			  }
			

			      CDAGNode* final_shifted_inlined_sr_dag;

				if(ent.getName() == top_level_entity)
				  {

				    //cout<<"\nOriginal DAG = "<<final_shifted_dag->getDAGStr()<<endl;
				   		    
				    // Finding the size of dag
				    set<CDAGNode*> TempSet;
				    int excnt=0, count=0;
				    TempSet.clear();
				    
				    final_shifted_dag->getSize(excnt, count, TempSet);

				    //cout<<"\nSize of present dag = "<<count<<endl;

				    // code to find size of dag ends here

				    //cout<<"\nCalling Inlining"<<endl;

				    bool InliningOn = true;
				    
				    CDAGNode *temp;
				    
				    if(InliningOn)
				      {
					temp = MANAGER.Global_Inlining(final_shifted_dag);
				      }
				    else
				      {
					temp = final_shifted_dag;
				      }
				    
				    //cout<<"\nInlining Done"<<endl;

				    //cout<<"\nCallig Expression Simplifier"<<endl;
				    
				    //cout<<"\nAfter inlining = "<<temp->getDAGStr()<<endl;


				    temp = convertExpressionWithExistsToProperFormula(temp);

				    //cout<<"\nAfter Expression Simplification  = "<<temp->getDAGStr()<<endl;
				    
				    //cout<<"\nExpression Simplification Done"<<endl;
				    

				    // Finding the size of dag

				    excnt=0;
				    count=0;
				    TempSet.clear();
				    
				    temp->getSize(excnt, count, TempSet);

				    //cout<<"\nSize of present dag = "<<count<<endl;

				  
				    // Finding the APTable and showing it

				    set<CDAGNode*> APTableOfDag;
				    MANAGER.getAPsForDAGInPropSkelton(temp, APTableOfDag);

				    //cout<<"\nAP TABLE\n**********\n";

				    //cout<<"\nSize of APTable = "<<APTableOfDag.size()<<endl;
				    
				    for(set<CDAGNode*>::iterator aptab_it=APTableOfDag.begin();aptab_it!=APTableOfDag.end();aptab_it++)
				      {
					//cout<<"\n"<<(*aptab_it)->getDAGStr()<<endl;
				      }

				    // code to find size of dag ends here

				   

				    //temp = MANAGER.Global_Scope_reduction(temp);
				    //cout<<"\nAfter Scope_reduction  = "<<temp->getDAGStr()<<endl;

	      
				    //if(true) exit(1); // Temporary

				    


				   // Get the widths of the variables

				   map<string, int> WidthTable;

				   getWidthTable(temp, top_level_entity, WidthTable); // To get the widths of the variables


				   
				   // Remove the unneeded selections

				   //cout<<"\nRemoving unneeded selections\n";
				   
				   temp = removeUnneededSelects(temp, WidthTable);

				   //cout<<"\nDag after removing unneeded selections = "<<temp->getDAGStr()<<endl;

				   temp = simplifyFormulaWithExists(temp, "rules_simplify.txt");
				   
				   //cout<<"\nDag after simplification = "<<temp->getDAGStr()<<endl;

				   

				   // Removing the unneeded selections completed


				    // Finding the size of dag

				    excnt=0;
				    count=0;
				    TempSet.clear();
				    
				    temp->getSize(excnt, count, TempSet);

				    //cout<<"\nSize of present dag = "<<count<<endl;

				  
				    // Finding the APTable and showing it

				    APTableOfDag.clear();
				    MANAGER.getAPsForDAGInPropSkelton(temp, APTableOfDag);

				    //cout<<"\nAP TABLE\n**********\n";

				    //cout<<"\nSize of APTable = "<<APTableOfDag.size()<<endl;
				    
				    for(set<CDAGNode*>::iterator aptab_it=APTableOfDag.begin();aptab_it!=APTableOfDag.end();aptab_it++)
				      {
					//cout<<"\n"<<(*aptab_it)->getDAGStr()<<endl;
				      }

				    // code to find size of dag ends here

				    //if(true) exit(1); // Temporary


				   temp = Quantifier_Eliminator_With_Redundant_Selection_Removal(temp, WidthTable);

				    //cout<<"\nCalling the Qunatifier Eliminator Finished\n";				    
				    
				    final_shifted_inlined_sr_dag = temp; 



				    // final_shifted_inlined_sr_dag = final_shifted_dag;
			    
				    
				  }
				
				else
				  {
				   			    
				    
				    // The below line is commented to add quantifier elimination at all levels
				    //final_shifted_inlined_sr_dag = final_shifted_dag;
				    
				     CDAGNode *temp = MANAGER.Global_Inlining(final_shifted_dag);
				    

				     //cout<<"\nAfter inlining = "<<temp->getDAGStr()<<endl;


				    temp = convertExpressionWithExistsToProperFormula(temp);

				    //cout<<"\nAfter Expression Simplification  = "<<temp->getDAGStr()<<endl;


				    //temp = MANAGER.Global_Scope_reduction(temp);
				    

				    //cout<<"\nAfter Scope_reduction  = "<<temp->getDAGStr()<<endl;

				    //cout<<"\nCalling the Qunatifier Eliminator\n";

				    // Get the widths of the variables

				    map<string, int> WidthTable;

				    getWidthTable(temp, top_level_entity, WidthTable);

				       
				    
				    

				    // Remove the unneeded selections

				   //cout<<"\nRemoving unneeded selections\n";
				   
				   temp = removeUnneededSelects(temp, WidthTable);

				   //cout<<"\nDag after removing unneeded selections = "<<temp->getDAGStr()<<endl;

				   temp = simplifyFormulaWithExists(temp, "rules_simplify.txt");
				   
				   //cout<<"\nDag after simplification = "<<temp->getDAGStr()<<endl;





				    temp = Quantifier_Eliminator_With_Redundant_Selection_Removal(temp, WidthTable);

				    //cout<<"\nCalling the Qunatifier Eliminator Finished\n";

				    //exit(1); // Temporary

				    
				    final_shifted_inlined_sr_dag = temp;  

				    
				    
				    //final_shifted_inlined_sr_dag = final_shifted_dag;
				    
				    
				    
				  }

				
				/* CODE ADDED ON 2nd Feb 09 ENDS HERE */


				/* COMMENTING ON 2nd Feb 09 STRATS HERE


				fprintf(inline_fp, "\n\nEntity Name = %s\n",(ent.getName()).c_str());
				fprintf(inline_fp, "\n\nOutput Name = %s\n",needed_output.c_str());

				if(existQuant == "")
				  {
				    fprintf(inline_fp, "\nNo Intermediate Variables\n");
				  }
				else
				  {
				    fprintf(inline_fp, "\nOriginal List of Intermediate Variables\n");
				    fprintf(inline_fp, "\n%s\n",existQuant.c_str());
				    fprintf(inline_fp, "\nNo: of Intermediate Variables = %d\n",setExQuantify.size());
				  }
			


				// TEMPORARY CODE ADDED ON 26th Dec 08 

				// THIS CODE IS TEMPORARY UNTIL INLINING IS CORRECTED. COMMENT THIS AND UNCOMMENT "CODE FOR INLINING" WHEN WE ADD INLINING 

				//CDAGNode *final_shifted_inlined_dag = final_shifted_dag;

				//if(existQuant != "")
				//	  {
				
				//	CDAGNode* exist_quant_dag = MANAGER.createDAG(existQuant);
					
				//	final_shifted_inlined_dag = MANAGER.createDAG("exists", exist_quant_dag, final_shifted_inlined_dag);
				//	}

				// TEMPORARY CODE ENDS
				
				
				// CODE FOR INLINING
				
				
				CDAGNode *final_shifted_inlined_dag = MANAGER.cloneAndShift(final_shifted_dag, 0);
			
				//cout<<"\n\nDAG before inling is "<<temp->getDAGStr();

				//CDAGNode *final_shifted_inlined_dag = convertIteToFunction(temp);

				//cout<<"\nHere2"<<temp<<"\n";

				//if(final_shifted_inlined_dag == NULL) cout<<"\nNULL\n";

				//cout<<"\n\nDAG for inling is : "<<final_shifted_inlined_dag->getDAGStr();

				//cout<<"\n\nEx.variables to be eliminated:";

				//for(setExQuantify_it = setExQuantify.begin(); setExQuantify_it != setExQuantify.end(); setExQuantify_it++)

				//  	{
				//	  string element = *(setExQuantify_it);
				//
				//	  cout<<"\t"<<element;
				//	}


				
				
				start_time = clock();	
			

				//call_findPlacesOfInlining(setExQuantify,final_shifted_inlined_dag);

				final_shifted_inlined_dag = Inlining(final_shifted_inlined_dag, setExQuantify);

				cout<<"\n\nTime needed for inlining = "<< ((double)clock()-start_time)/CLOCKS_PER_SEC <<endl<<endl;

				//cout<<"\n\nDAG after inling is :"<<final_shifted_inlined_dag->getDAGStr();

				// Getting all variables in final_shifted_inlined_dag

				final_shifted_inlined_dag->getDAGNames(setVars);

							
				for(setVars_it = setVars.begin(); setVars_it != setVars.end(); setVars_it++)

				  	{
					  string element = *(setVars_it);

					  if(is_intermediate(element, intermediate_identifiers))
						{
							setExQuantifySecond.insert(element);				
						}
					}

				setVars.clear();

				//set<string> setDiffer2;

  				//set_difference(setExQuantify.begin(), setExQuantify.end(), setExQuantifySecond.begin(), setExQuantifySecond.end(), inserter(setDiffer2, setDiffer2.begin()));

				
				//existQuant = getListString(setDiffer2);
				
				//cout<<"\n\nEx.variables eliminated:\t"<<existQuant;

				//cout<<"\n\nNo: of Ex.variables eliminated:\t"<<setDiffer2.size();

				//cout<<"\n\nNo: of Ex.variables still to be eliminated:\t"<<setExQuantifySecond.size();

				existQuant = getListString(setExQuantifySecond);

				if(existQuant == "")
				  {
				    fprintf(inline_fp, "\nNo Intermediate Variables\n");
				  }

				else
				  {
				    fprintf(inline_fp, "\nRemaining List of Intermediate Variables after Inlining\n");
				    fprintf(inline_fp, "\n%s\n",existQuant.c_str());
				    fprintf(inline_fp, "\nNo: of Intermediate Variables Remainining = %d\n",setExQuantifySecond.size());
				  }

				// final_shifted_inlined_dag CONTAINS THE SHIFTED INLINED DAG without the ("exists", remaining variables, part. Remaining variables are in setExQuantifySecond

				// INLINING CODE ENDS HERE

			

				// CALLING NACHIKET'S EXPRESSION SIMPLIFIER : TEMPORARY NOW

				if(existQuant != "")
				  {
				    cout<<"\n\nInput to NACHIKET'S EXPRESSION SIMPLIFIER\n\n";
                                    CDAGNode* Input_CDAGNode = MANAGER.createDAG("exists", getDAGFromStringSet(setExQuantifySecond), final_shifted_inlined_dag); 
				    cout<<"\nBefore Conversion to Expr. simplifier Format\n";
				    cout<<"\n"<<Input_CDAGNode->getDAGStr()<<endl;
				    
				    ExprSimpRoot = convertCDAGNodeToCExprSimpNode(Input_CDAGNode);

				    cout<<"\nAfter Conversion to Expr. simplifier Format\n";
				    cout<<"\n"<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;
				    cout<<"\nDAG Size = "<<findSizeOfDAG(ExprSimpRoot)<<endl;

				    //CDAGNode *Output_CDAGNode = convertCExprSimpNodeToCDAGNode(ExprSimpRoot);

				    //cout<<"\nAfter Conversion to Expr. simplifier Format in CDAGNode* format\n";
				    //cout<<"\n"<<Output_CDAGNode->getDAGStr()<<endl;
				    
				    ExpressionSimplifier();
				    
				    cout<<"\nAfter Simplification\n";
				    cout<<"\n"<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;
				    cout<<"\nDAG Size = "<<findSizeOfDAG(ExprSimpRoot)<<endl;

				    CDAGNode *Output_CDAGNode = convertCExprSimpNodeToCDAGNode(ExprSimpRoot);

				    cout<<"\nResult of Expr. simplifier\n";
				    cout<<"\n"<<Output_CDAGNode->getDAGStr()<<endl;

				  }
				

				// CALLING NACHIKET'S EXPRESSION SIMPLIFIER ENDS HERE : TEMPORARY NOW

				// SCOPE REDUCTION CODE STARTS HERE

				CDAGNode* final_shifted_inlined_sr_dag;

				if(existQuant == "")
				  {
				    final_shifted_inlined_sr_dag = final_shifted_inlined_dag;
				  }
				else
				  {
				    map<CDAGNode*,CDAGNode*> temp8;

				    set<string> temp9;
				
				    CDAGNode *temp6 = MANAGER.cloneAndShift(final_shifted_inlined_dag, 0);
				    CDAGNode *temp7 = convertIteToFunction(temp6);
				    if(temp7 == NULL) cout<<"\nNULL\n";

				    cout<<"\n\nDAG for scope reduction is : "<<temp7->getDAGStr();
				
				    cout << endl<< endl << "setExQuantifySecond.size()=" << setExQuantifySecond.size() << endl;
				
				    final_shifted_inlined_sr_dag = scopeReduction(setExQuantifySecond,temp7);
				  }

				//cout<<"\n\nSR1 OVER\n\n";


				
				COMMENTING ON 2nd FEB 2009 ENDS HERE   */
				


				/* TEMPORARILY COMMENTING THE OTHER TWO SREDUCTION STRATEGIES */

				//************************************

				/*

				map<CDAGNode*,CDAGNode*> temp18;

				set<string> temp19;
				
				CDAGNode *temp16 = MANAGER.cloneAndShift(final_shifted_inlined_dag_for_sr, 0);
				CDAGNode *temp17 = convertIteToFunction(temp16);
				if(temp17 == NULL) cout<<"\nNULL\n";
				
				
				scopeReduction2(setExQuantifySecond,temp17);

				cout<<"\n\nSR2 OVER\n\n";

				*/
				
				//*****************************************

				/*

				map<CDAGNode*,CDAGNode*> temp28;

				set<string> temp29;
				
				CDAGNode *temp26 = MANAGER.cloneAndShift(final_shifted_inlined_dag_for_sr, 0);
				CDAGNode *temp27 = convertIteToFunction(temp26);
				if(temp27 == NULL) cout<<"\nNULL\n";
				
				scopeReduction3(setExQuantifySecond,temp27);

				cout<<"\n\nSR3 OVER\n\n";

				*/

				


				// SCOPE REDUCTION CODE ENDS HERE
				
				
			
				
			
			setDone.clear();

			final_shifted_inlined_sr_dag->getDAGNames(setDone);

			map<CSensitiveElement*, string> delays; // Delay with each input

			initializeDeltaMapForEntity(delays, input_identifiers);

			
			// THE FOLLOWING LINE IS COMMENTED AND NEW LINE ADDED ON 11/1/2010 AS BUG IN calculateDeltaDelays FOUND WHILE TESTING : SEE readme.txt
			
			//final_shifted_inlined_sr_dag->calculateDeltaDelays(delays); // Calculate delta time of each input from the dag for SE

			
			final_shifted_inlined_sr_dag->calculateDeltaDelaysModified(delays); // Calculate delta time of each input from the dag for SE


			// Finding the delays by substracting the delta time of the input from the delta time of the output

			map<CSensitiveElement*, string>::iterator delays_it;

			//cout<<"\n\nDELAYS\n\n";
		  
			//for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
			//  {
			//    cout<<"name = "<<delays_it->first->name<<"\t"<<delays_it->first->lower<<"\t"<<delays_it->first->upper<<"\tdelay="<<delays_it->second<<endl;
			//  }

			//cout<<"\n\nDelta Time = "<<delta_time;

			for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
			  {
			     char temp[20];
			    //sprintf(temp, "%d", delta_time - atoi(delays_it->second.c_str()));

			    // For inputs which are present in the sensitivity list; but not in the symbolic expression, the delay is set as the delta time of the component; for others it is the absolute value of the delay
			    
			    if(delays_it->second == "") 
			      {
				sprintf(temp, "%d", delta_time); 
			      }
			    else
			      {
				sprintf(temp, "%d", -1*atoi(delays_it->second.c_str()));
			      }
				
			    delays_it->second = temp;
			  }
			
			//cout<<"\n\nDELAYS\n\n";
		  
			//for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
			//  {
			//    cout<<"name = "<<delays_it->first->name<<"\t"<<delays_it->first->lower<<"\t"<<delays_it->first->upper<<"\tdelay="<<delays_it->second<<endl;
			//  }

						
			SymbolicRelation *rio = new SymbolicRelation(delays, final_shifted_inlined_sr_dag, fr->ranges_covered, setDone);

			ent.table[(*iout)->name] = rio;

			//RelationInputOutput *rio = new RelationInputOutput(delta_time, final_shifted_inlined_sr_dag, present_condition, fr->ranges_covered, setDone);

			//ent.table[(*iout)->name].push_back(rio);
		}
	
	


/* THIS PART IS FOR CONC. ASSIGNMENTS LIKE "X<='0'" */

	       iout = output_identifiers.begin();

		// checking table for each output
		for(;iout != output_identifiers.end(); iout++)
		  {
		    if(ent.table.find((*iout)->name) == ent.table.end())
		      {
		      //cout<<"\nTable for "<<(*iout)->name<<" is empty!!";
		      list<CConSignAssgnStmt *> &conAssignNodes = ent.getConAssgnStmt();
		      list<CConSignAssgnStmt *>::iterator conAssignNodes_iter = conAssignNodes.begin();
		      for(;conAssignNodes_iter != conAssignNodes.end(); conAssignNodes_iter++)
		       {
			 if(((*conAssignNodes_iter)->getLHS()->getName()==(*iout)->name))
			   {
			     //cout<<"\nMatch Found!!\n";

			     COperand* lhs_operand = (*conAssignNodes_iter)->getLHS();
			     
			     list<CExpression*> conAssign_values = (*conAssignNodes_iter)->getValues();
			     list<CExpression*> conAssign_choices = (*conAssignNodes_iter)->getChoices();
			     list<CSensitiveElement*> conAssign_sensitivityList;

			     list<CExpression*>::iterator conAssign_choices_iter = conAssign_choices.begin();
			     list<CExpression*>::iterator conAssign_values_iter = conAssign_values.begin();

			     if((*conAssign_choices_iter)->getOperand() == "others")
				  {
				    set<string> temp;
				    (*conAssign_values_iter)->getSensitivityList(conAssign_sensitivityList);

				    if(conAssign_sensitivityList.begin()==conAssign_sensitivityList.end())
				      {
					set<string> setDone;
					CDAGNode* rhs_dag_conAssign = (*conAssign_values_iter)->getDAG(ent.getName(), 0, temp);
					//cout<<dag1->getDAGStr()<<endl;
					CDAGNode* lhs_dag_conAssign = lhs_operand->getDAG(ent.getName(), 0, temp);

					CDAGNode* final_dag = MANAGER.createDAG("equivalent", lhs_dag_conAssign , rhs_dag_conAssign);
					//cout<<final_dag->getDAGStr()<<endl;

					
					int low = atoi(((CAtom*)lhs_operand)->getLower().c_str());
					int up = atoi(((CAtom*)lhs_operand)->getUpper().c_str());
					//cout<<"\nLow="<<low<<",Up="<<up;
					pair<int, int> range(low, up);
					vector<pair<int, int> > ranges_covered;
					ranges_covered.push_back(range);

					final_dag->getDAGNames(setDone);

					/* Setting delays */

					map<CSensitiveElement*, string> delays; // Delay with each input

					initializeDeltaMapForEntity(delays, input_identifiers);

					map<CSensitiveElement*, string>::iterator delays_it;

					//cout<<"\n\nDELAYS\n\n";
		  
					for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
					  {
					    //cout<<"name = "<<delays_it->first->name<<"\t"<<delays_it->first->lower<<"\t"<<delays_it->first->upper<<"\tdelay="<<delays_it->second<<endl;
					  }

					//cout<<"\n\nDelta Time = 1\n\n";

					for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
					  {
					    delays_it->second = "1";
					  }
			
					//cout<<"\n\nDELAYS\n\n";
		  
					for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
					  {
					    //cout<<"name = "<<delays_it->first->name<<"\t"<<delays_it->first->lower<<"\t"<<delays_it->first->upper<<"\tdelay="<<delays_it->second<<endl;
					  }

					/* Adding to the table */

					SymbolicRelation *rio = new SymbolicRelation(delays, final_dag, ranges_covered, setDone);

					ent.table[(*iout)->name] = rio;
				      }
				  }
			   }
		       }
		      
		
		      }  
		  }   


		
		    
	  
		    }



void CSymbSimulator::PerformSymbolicSimulationOverall(CDependencyGraph* dg,
					CEntity& ent,
					map<string, list<CSenNode*> > &affecting,
					map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
	set<CSensitiveElement*> identifiers;
	set<CSensitiveElement*> input_identifiers;
	set<CSensitiveElement*> output_identifiers;
	set<CSensitiveElement*> intermediate_identifiers;

	map<string, list<ConditionRelation*> >mapSignalDelta;
	
	getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers, ent);

	 set<CSensitiveElement*>::iterator input_identifiers_it;

	 //cout<<"\n\nINPUT IDENTIFIERS\n\n";

	 //for(input_identifiers_it = input_identifiers.begin(); input_identifiers_it != input_identifiers.end(); input_identifiers_it++)
	 //  {
      
	 //    (*input_identifiers_it)->printCSensitiveElement();
	 //    cout<<"\n";
	 //  }


	 //cout<<"\n\nAdding the basic blocks as the vertices of bb graph\n\n";

	 Graph_BB BBTriggeringGraph;

	 if(structural_cycle_present && !real_cycle_present)
	   {
	     initializeBBTriggeringGraph(BBTriggeringGraph, dg->getBasicBlocks());
	   }

	map<string, ConditionsValues> SymbolicQueues;

	createValuesAtZeroTime(SymbolicQueues, ent, identifiers);

	createValuesAtOneTimeForOverall(SymbolicQueues, ent, input_identifiers,
				affecting, nodePartOfBasicBlock, BBTriggeringGraph);


	int i = 1;
//	int no_of_simulation_cycles = 6;
	int current_time = 1;
	int max_input_time = 1;

	//cout<<"\n\nDISPLAYING AFF.B.B BEFORE ENTERING LOOP\n\n";

	//displayAffectedBB();

	//	cout<<endl<<endl;

	// NOTE : 40 IS THE LIMIT PUT ON THE NO: OF DELTAS FOR ANY CIRCUIT, SINCE THE SE'S BEYOND SOME LIMIT MAY BE REALLY PROHIBITIVE IN SIZE
	
	while(!affectedBasicBlocksForOverall.empty() && i < 40)
	{
		AffBlocksAndDeltaWithInput * abd = affectedBasicBlocksForOverall.front();
		affectedBasicBlocksForOverall.pop_front();
		if(current_time == abd->delta)
		{
		}
		else
		{
			i++;
			current_time = abd->delta;
			if((real_cycle_present == true) && (i > no_of_simulation_cycles))
			{
			  //cout<<"\n\ni > no_of_simulation_cycles in cyclic circuit...Hence exiting from the loop\n\n";
			  break;
			}
		}
		
		if(max_input_time < current_time)
		{
			max_input_time = current_time;
		}

		PerformSymbolicSimulationBB(abd, ent, SymbolicQueues, mapSignalDelta, affecting, nodePartOfBasicBlock, BBTriggeringGraph);
//		i++;

	}

	if(i==40)

	  {
	    //cout<<"\n\nSYMBOLIC SIMULATION DONE UP TO THE MAXIMUM LIMIT 40\n\n";
	  }
	
	//printSymbolicQueues(SymbolicQueues);

	//map<string, FinalRelation*>  final_expressions;

	map<string, FinalRelationWithDelta*> final_expressions;
	
	calculateExpressionsForOverall(SymbolicQueues, mapSignalDelta, final_expressions, ent); 

	// The call to the fn: createNonExistingConstraints seems to be unneeded
	
	//createNonExistingConstraints(SymbolicQueues, final_expressions, max_input_time, ent);

	createTableOverall(ent, SymbolicQueues, output_identifiers, input_identifiers, intermediate_identifiers, mapSignalDelta, final_expressions);

	#ifdef DEBUG
	printSymbolicQueues(SymbolicQueues);
	#endif
}

void CSymbSimulator::createNonExistingConstraints(map<string, ConditionsValues> &SymbolicQueues, map<string, FinalRelation*> & final_expressions, int max_input_time, CEntity &ent)
{
	map< string, ConditionsValues >::iterator isymb = SymbolicQueues.begin();
	
	for(;isymb != SymbolicQueues.end(); isymb++)
	{
		string idname = isymb->first;
		ConditionsValues &cv = isymb->second;

		int size = cv.delta.size();
		int current_time = 1;

		for(int i=1; i < size; i++)
		{
		  //cout <<"\n\ni="<<i<<endl;
			if(current_time < cv.delta[i])
			{
				for(int j=current_time; j < cv.delta[i] || j<max_input_time ;j++)
				{
					CDAGNode * new_id = MANAGER.createDAG(idname, j);
					CDAGNode * dag1 = MANAGER.createDAG("equivalent", new_id, cv.values[i]);
					set<string> setCondRel;
					setCondRel.insert(cv.values[i]->getDAGStr());
					
					SymTabEntry &ste = SymbolTable[ent.getName()][idname];
		
					list<string>::iterator irange;
					irange = ste.rangeindex.begin();
					
					int start = atoi((*irange).c_str());
					irange++;
					
					int last = atoi((*irange).c_str());
					
					pair<int, int> ranges(start, last);
					
					vector<pair<int, int> >rel_ranges;
					rel_ranges.push_back(ranges);
					
						
					FinalRelation* fr = new FinalRelation(dag1, setCondRel, rel_ranges);
					
					final_expressions.insert(make_pair(new_id->getDAGStr(), fr));
				}
				
				current_time = cv.delta[i];
			}
		}
	}
}

void CSymbSimulator::PerformSymbolicSimulationBB(AffBlocksAndDeltaWithInput* abd, CEntity& ent, map<string, ConditionsValues> &SymbolicQueues, map<string, list<ConditionRelation*> >& mapSignalDelta, map<string, list<CSenNode*> > &affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock, Graph_BB &BBTriggeringGraph)
{
	int current = abd->delta;
	CBasicBlock *bb = abd->bb;
	CSensitiveElement* Changed_Input = abd->input;


	//cout<<"\n\nTaking the Basic Block triggered at "<<current<<" with input ";
	//Changed_Input->printCSensitiveElement();
	//cout<<endl<<endl;

	/* We are changing the condn. to be inserted at the queue */
	//CDAGNode *prev_condition = abd->condition;
	//set<string>& prev_setCondition = abd->setCondition;

	//cout<<prev_condition->getDAGStr()<<endl;

	CDAGNode *prev_condition;

	set<string> prev_setCondition;

	set<CSensitiveElement*> input_identifiers = bb->get_Input_identifiers();

	

	//cout<<"\n\nInput identifiers\n\n";

	//for(set<CSensitiveElement*>::iterator input_identifiers_it = input_identifiers.begin(); input_identifiers_it != input_identifiers.end(); input_identifiers_it++)
	//  {
	//    (*input_identifiers_it)->printCSensitiveElement();
	//    cout<<endl;
	//  }

	
	// Newly added on 20th July 2009

	bool IsTriggeringConditionTrue = checkIfTriggeringConditionIsTrue(bb);	

	// Newly added on 20th July 2009 ends here


	
	map< string, SymbolicRelation*>& table = bb->getTable();

	map< string, SymbolicRelation*>::iterator itab = table.begin();

	for(;itab != table.end(); itab++)
	{

		string output_identifier = itab->first;
		SymbolicRelation* entry = itab->second;

		//cout<<"\n\n"<<output_identifier<<" is changed\n\n";


		CDAGNode* relation = entry->relation;
		
		//cout<<"\n\nSE for "<<output_identifier<<" is " <<relation->getDAGStr()<<" at delta time 0\n";

		map<CSensitiveElement*, string> delay_map = entry->delays;

		//CDAGNode* condition = (*irio)->condition;
		set<string> setRelation;

		/* current is the delta time at which this BB was triggered with change in Changed_Input. But we do not know the delta time of this entry. It is to be computed like this.
		   We know that BB was triggered by change in Changed_Input. We can find from the delay_map, the delta time for a change in Changed_Input to affect the output. Hence, 
		   delta time of this entry i.e.present_delta_time = current + delay_map(Changed_Input)
		   The relation's delta time is 0.
		   Hence to get the final relation we should shift relation by present_delta_time */

		

		int present_delta_time = current + findDeltaTimeOfInput(delay_map, Changed_Input);
		int shift = present_delta_time;

		//cout<<"\n\nDelta time of this entry is " <<present_delta_time<<".   Hence shifting relation by "<<shift<<endl<<endl;

		CDAGNode* relation_shifted;

		if(shift == 0)
		{
			relation_shifted = relation;
		}
		else
		{
			map<CDAGNode*,CDAGNode*> nodeShiftednode;
			relation_shifted = MANAGER.cloneAndShift(relation, shift);
			nodeShiftednode.clear();
		}

		//cout<<"\n\nShifted SE for "<<output_identifier<<" is "<<relation_shifted->getDAGStr()<<endl<<endl;

		ent.constraints.push_back(relation_shifted); // shifted SE added to constraints of the entity

		//CDAGNode* out_dag = MANAGER.createDAG(output_identifier, current + delta_time - 2);

		CDAGNode* out_dag = MANAGER.createDAG(output_identifier, present_delta_time);

		//cout<<"\n\nADDING  "<<out_dag->getDAGStr()<<" into s.queue of "<<output_identifier<<"at time "<<current + delta_time - 2<<endl;

		char str_current[20];

		sprintf(str_current, "%d", present_delta_time);

		string id_delta = output_identifier + "_" + str_current;

		

		// Newly added on 20th July 2009

		if(IsTriggeringConditionTrue) // First node of BB is component instantiation Or ConcSigAssgnment Or a combinational process statement. Hence triggering condition is true
		  
		  prev_condition = MANAGER.createDAG("true");

		else

		  prev_condition = getTriggeringConditionForOverall(delay_map, prev_setCondition, Changed_Input, current); 


		// Newly added on 20th July 2009 ends. Earlier this was just prev_condition = getTriggeringConditionForOverall(delay_map, prev_setCondition, Changed_Input, current); The if-else is newly added


		ConditionRelation *cr = new ConditionRelation(prev_condition, relation_shifted);

		setRelation.erase(id_delta);

		cr->setRelation.insert(setRelation.begin(), setRelation.end());
		cr->ranges_covered = entry->ranges_covered;

		//mapSignalDelta[id_delta].push_back(cr);

		updateMapSignalDelta(mapSignalDelta, cr, output_identifier, present_delta_time, id_delta);

		//mapSignalDelta[output_identifier].push_back(cr); // We need to consider only the output name; delta is not needed as only the last entry is sufficient

		
		//cout<<"\n\nADDING  "<<out_dag->getDAGStr()<<" into s.queue of "<<id_delta<<" at time "<<present_delta_time<<" with condition "<<prev_condition->getDAGStr()<<endl<<endl;
			

		//insertToSymbolicQueue(current + delta_time - 2, out_dag, prev_condition, prev_setCondition, SymbolicQueues[output_identifier], (*irio)->ranges_covered);

		

		insertToSymbolicQueue(present_delta_time, out_dag, prev_condition, prev_setCondition, SymbolicQueues[output_identifier], entry->ranges_covered);

		//list<CBasicBlock *> ::iterator iout = bb->out_list.begin(); identification of BBs is wrong!!!

		// Finding the basic blocks sensitive to the output_identifier

		list<CSenNode*> newly_affectedNodes = affecting[output_identifier];

		list<CSenNode*>::iterator newly_affectedNodes_it = newly_affectedNodes.begin();
			
		list<CBasicBlock*> newly_affectedBBs;

		for(;newly_affectedNodes_it != newly_affectedNodes.end(); newly_affectedNodes_it++)
			  {
			    newly_affectedBBs.push_back(nodePartOfBasicBlock[(*newly_affectedNodes_it)->n]);
			  }
			

		//cout<<"\nNEW BBs Triggered\n";

		int sz1 = newly_affectedBBs.size();

		list<CBasicBlock *> ::iterator iout = newly_affectedBBs.begin();

		for(;iout != newly_affectedBBs.end(); iout++)
			  {
			    //cout<<"\nDisplaying BB\n";

			    //(*iout)->display();
			    
			    //cout<<"\n\noutput id " << output_identifier <<endl;
			     CSensitiveElement* se = getSenEleforInput((*iout)->get_Input_identifiers(), output_identifier);
			     set<string> setCondition;
				
			     if(se == NULL)
				{
					continue;
				}

			     insertToAffectedBBForOverall(present_delta_time, se, (*iout), bb, BBTriggeringGraph);

			  }
		}
}


bool CSymbSimulator::is_output_input(string element, set<CSensitiveElement*> &output_identifiers, set<CSensitiveElement*> &input_identifiers)
{

        int index = element.find_last_of("_");
	string name = element.substr(0, index); 

	set<CSensitiveElement*>::iterator iid = output_identifiers.begin();
	for(; iid != output_identifiers.end(); iid++)
	{
		if(name == (*iid)->name)
		{
			return true;
		}
	}

	iid = input_identifiers.begin();
	for(; iid != input_identifiers.end(); iid++)
	{
		if(name == (*iid)->name)
		{
			return true;
		}
	}
	
	return false;
}

bool CSymbSimulator::is_intermediate(string element, set<CSensitiveElement*> &intermediate_identifiers)
{
        int index = element.find_last_of("_");
	string name = element.substr(0, index); 
	

        set<CSensitiveElement*>::iterator iid = intermediate_identifiers.begin();
        for(; iid != intermediate_identifiers.end(); iid++)
	  {     
	        
                if(name == (*iid)->name)
                {
                        return true;
                }
        }
                                                                  
                                                                                              
        return false;
}


bool CSymbSimulator::presentInmapSignalDelta(map<string, list<ConditionRelation*> > mapSignalDelta, string target)
{

  map<string, list<ConditionRelation*> >::iterator  mapSignalDelta_it;

 for(mapSignalDelta_it = mapSignalDelta.begin(); mapSignalDelta_it != mapSignalDelta.end(); mapSignalDelta_it++)

    {
      if(target == mapSignalDelta_it->first)
	return true;
    }
 return false;
}


void CSymbSimulator::createTableForBB(CBasicBlock* bb, map<string, ConditionsValues> &SymbolicQueues, map<string, FinalRelationWithDelta*>& final_expressions, map<string, list<CSenNode*> > affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{

        clock_t start_time;
	set<CSensitiveElement*>:: iterator iout;

	//cout<<"\nCreating table for each output\n";

	//iout = bb->get_Output_identifiers().begin();

	//for(; iout != bb->get_Output_identifiers().end(); iout++)
	//  {
	//  cout<<"\n\nOutput is \t"<<(*iout)->name;  
	//  cout<<endl;
	//  }

	//cout<<endl<<endl;

	/*map<string, FinalRelationWithDelta*>::iterator  final_expressions_it;

	  cout<<"\n\nFinal Expression contains\n\n";

	  for(final_expressions_it = final_expressions.begin(); final_expressions_it != final_expressions.end(); final_expressions_it++)
	    
	    {
	      string element = (*final_expressions_it).first;
	      cout<<element<<"\t";
	      cout<<((*final_expressions_it).second)->delta<<"\t"<<(((*final_expressions_it).second)->final_relation)->getDAGStr()<<endl<<endl;
	    }

	    cout<<"\n\nFinal Expression is Finished\n\n";*/


	   

	// creating table for each output

	for(iout = bb->get_Output_identifiers().begin();iout != bb->get_Output_identifiers().end(); iout++)
	{

	   //cout<<"\n\nOutput is \t"<<(*iout)->name;

	  	ConditionsValues &cv = SymbolicQueues[(*iout)->name];

		
		vector<int>::iterator  idel = cv.delta.begin();
		vector<CDAGNode*>::iterator ival = cv.values.begin();
		vector<CDAGNode*>::iterator  icond = cv.conditions.begin();


		// creating RelationInputOutput entries for each entry in symbolic queue
		// starting from second

		// Now we need to create RelationInputOutput entries for the last  entry in symbolic queue

	        // point i to the last delta

		int i = cv.delta.size()-1;

		//for(int i=1 ;i < cv.delta.size(); i++)
		//{
			int delta_time = cv.delta[i];

			CDAGNode* present_condition = cv.conditions[i];

			string id_delta = cv.values[i]->getDAGStr(); // Now name is x_3

			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index); // id is now x
			
			string needed_output = id;

			int delta = atoi(id_delta.substr(index+1).c_str()); // Note that delta = delta_time

			//	if(!presentInmapSignalDelta(bb->mapSignalDelta, cv.values[i]->getDAGStr()))
			  
			//  { cout <<endl<<endl<<cv.values[i]->getDAGStr()<<" not present in mapSignalDelta\n\n"; continue;}

			// FinalRelation* fr = final_expressions[cv.values[i]->getDAGStr()];

			FinalRelationWithDelta* fr = final_expressions[id];

			//MANAGER.createDAG("ite", cv.conditions[i], cv.values[i], dag1);

			// final_dag is this SE shifted by delta

			map<CDAGNode*,CDAGNode*> temp1;
			set<string> temp2;

			//CDAGNode* final_dag = MANAGER.cloneAndShift(fr->final_relation, delta_time);

			//temp1.clear();

			CDAGNode* final_dag = fr->final_relation;

			//cout<<"\n\nFinal Relation->name = "<<(fr->final_relation)->name<<" Final Relation->delta_time= "<<(fr->final_relation)->delta_time<<endl;
			
			set<string> setCondRel;
			(fr->final_relation)->getDAGNames(setCondRel);
			set<string> setDone;
			
			
			//cout<<"\n\nFinal Relation\n\n";

			//cout<<"\n"<<(fr->final_relation)->getDAGStr()<<endl;

			//cout<<"\n\nsetCondRel\n\n";

			//for(set<string>::iterator setCondRel_it=setCondRel.begin(); setCondRel_it != setCondRel.end(); setCondRel_it++) 
			//  cout<<(*setCondRel_it)<<endl;

			//cout<<"\n\nFinding the variables whose constraints are to be added\n\n";
							
			while(!setCondRel.empty())
			  {			        
				string element = *(setCondRel.begin());

				//cout<<"\n\nElement = "<<element<<endl<<endl;

				setCondRel.erase(element);

				index = element.find_last_of("_");
				id = element.substr(0, index); 
				delta = atoi(element.substr(index+1).c_str()); 

				//if(delta < 2) continue; // No need not addd constraints for elements with delta time at 0 and 1
				// This is added temporarily. Should be removed after allowing -ve deltas

				if(needed_output == id) 
				  {
				    //cout<<"\n\nSame as needed output\n\n";
				    continue;
				  }
				
				if(is_output_input(element, bb->get_Output_identifiers(), bb->get_Input_identifiers()) )
				{
				  //cout<<"\n\nNot an intermediate variable\n\n";
				  continue;
				}
				
				if(final_expressions.find(id) == final_expressions.end())
				{
				  //cout<<"\n\nNo expression present\n\n";
				 continue;
				}

				//if(!setDone.empty())
				//  {
				//   if(!(setDone.find(id) == setDone.end()))
				//      {
				//	cout<<"\n\nAlready added\n\n";
				//	continue;
				//     }
				//  }

				//setDone.insert(id);

				if(findElementWithLargerOrSameDelta(setDone, element))
				  {
				    //cout<<"\n\nElement With Larger Or Same Delta Already Exists\n\n";
				    continue;
				  }
				
								
				addElementToSetDone(setDone, element);

				FinalRelationWithDelta* fr_ele = final_expressions[id];

				int shift =  delta - fr_ele->delta;

				//cout<<"\n\nShift for "<<element<<" is "<<shift;

				CDAGNode *newConstraint = MANAGER.cloneAndShift(fr_ele->final_relation, shift);

				temp1.clear();

				//cout<<"\n\nNew Constraint is "<<newConstraint->getDAGStr()<<endl<<endl;

				set<string> setNew, setNewReduced;
				set<string>::iterator setNew_it;
			
				
				newConstraint->getDAGNames(setNew);
				
				//if(final_dag == NULL)
				//  final_dag = newConstraint;
				//else
				//  final_dag = MANAGER.createDAG("and", final_dag, newConstraint);
				
				for(setNew_it = setNew.begin(); setNew_it != setNew.end(); setNew_it++)

				  	{
					  
					  string new_element = *(setNew_it);
					  //cout<<"\n\nnew_element = "<<new_element<<endl<<endl;
					  if(!(setCondRel.find(new_element) == setCondRel.end()))
					    {
					      //cout<<"\n\nAlready added\n\n";
					     }
					  else
					    {

					  //    int new_index = new_element.find_last_of("_");
					  //    string new_id = new_element.substr(0, new_index); 
					  //    if(!(setDone.find(new_id) == setDone.end()))
					  //	{
					  //	  cout<<"\n\nAlready added\n\n";
					  //	}
					  //    else
					  //	{
					  //	  setNewReduced.insert(new_element);
					  //	}
					      
					      if(findElementWithLargerOrSameDelta(setDone, new_element))
					      	{
					      	  //cout<<"\n\nElement With Larger Or Same Delta Already Exists\n\n";
					      	}
					      else
					      	{
					      	  //cout<<"\n\nNew One\n\n";
					      	  setNewReduced.insert(new_element);
					      	}
					    }
					 
					 
					}

				//cout<<"\n\nAdding setNewReduced to setCondRel\n\n";

				setCondRel.insert(setNewReduced.begin(), setNewReduced.end());
			
				//cout<<final_dag->getDAGStr()<<endl;
				
				//set<string> setDiffer;

  				//set_difference(fr->setFinalRelation.begin(), fr->setFinalRelation.end(), setDone.begin(), setDone.end(),inserter(setDiffer, setDiffer.begin()));
                 				
				//cout<<fr->setFinalRelation.size()<<endl;
                 
				//cout<<setCondRel.size()<<endl;
                 
                 				
				//setCondRel.insert(setDiffer.begin(), setDiffer.end());
                 
 				//cout<<setCondRel.size()<<endl;
                 
				/*set<string>::iterator iset = fr->setFinalRelation.begin();
				for(; iset != fr->setFinalRelation.end(); iset++)
				{
					if(setDone.find() != s)
				}*/
			  }
			
			//cout<<"\n\nLoop for finding the variables whose constraints are to be added finishwd\n\n";

			//cout<<"\n\nLoop for adding additional constraints starting\n\n";
			
			for(set<string>::iterator setDone_it = setDone.begin(); setDone_it != setDone.end(); setDone_it++)

				  	{
					  string element = *(setDone_it);
					  //cout<<"\n\nelement = "<<element<<endl<<endl;
					  index = element.find_last_of("_");
					  id = element.substr(0, index); 
					  delta = atoi(element.substr(index+1).c_str()); 
					  
					  FinalRelationWithDelta* fr_ele = final_expressions[id];
					  int shift =  delta - fr_ele->delta;
					  //cout<<"\n\nShift for "<<element<<" is "<<shift;
					  
					  CDAGNode *newConstraint = MANAGER.cloneAndShift(fr_ele->final_relation, shift);
					  temp1.clear();
					  
					  //cout<<"\n\nNew Constraint is "<<newConstraint->getDAGStr()<<endl<<endl;
					  
					  if(final_dag == NULL)
					      final_dag = newConstraint;
					  else
					      final_dag = MANAGER.createDAG("and", final_dag, newConstraint);
									  
					}

			//cout<<"\n\nLoop for adding additional constraints over\n\n";

			//cout<<"\n\nDAG before shifting == "<<final_dag->getDAGStr()<<endl<<endl;

			CDAGNode* final_shifted_dag = MANAGER.cloneAndShift(final_dag, -1*delta_time);

			temp1.clear();

			//cout<<"\n\nDAG after shifting == "<<final_shifted_dag->getDAGStr()<<endl<<endl;

			CDAGNode* final_shifted_inlined_sr_dag;

			


			/* CURRENTLY WE ARE DOING QUANTIFIER ELIMINATION ONLY AT ENTITY LEVEL. Hence irrespective of qelimination_on we always do not collect variables to quantify out here */

			if(false) // Earlier the codn was qelimination_on == 1
			  {
			 
			set<string> setVars, setExQuantify, setExQuantifySecond;
			set<string>::iterator setVars_it, setExQuantify_it;

			final_shifted_dag->getDAGNames(setVars);
				
			for(setVars_it = setVars.begin(); setVars_it != setVars.end(); setVars_it++)

			  	{
				  string element = *(setVars_it);
				  //cout<<"\n\nelement = "<<element;
				  if(!is_output_input(element, bb->get_Output_identifiers(), bb->get_Input_identifiers()))
				        {
					 //cout<<"\nIntermediate\n";
					setExQuantify.insert(element);				
					}
				}

			setVars.clear();

			//cout<<"\n\nLoop for obtaining ex.variables over\n\n";

			string existQuant = getListString(setExQuantify);

			//string existQuant = "Hi"; 
			
			//cout <<"\nEx.Quant. List\n";

			//cout<<existQuant;

			final_shifted_inlined_sr_dag = final_shifted_dag;

			if(existQuant != "")
				  {
			
				 final_shifted_inlined_sr_dag = MANAGER.createDAG("exists", getDAGFromStringSet(setExQuantify), final_shifted_inlined_sr_dag);  
				 } 
			  }
			else
			  {
			   final_shifted_inlined_sr_dag = final_shifted_dag;
			  }

					
			/* According to the latest scheme, we are not doing any simplification at BB or entity level - only at top level. Hence commenting this */
			
			// CODE FOR INLINING
				
			/* Keeping a copy of original DAG */

			/*

			CDAGNode *final_shifted_inlined_dag = MANAGER.cloneAndShift(final_shifted_dag, 0);

			//cout<<"\n\nDAG before inling is "<<temp->getDAGStr();

			//CDAGNode *final_shifted_inlined_dag = convertIteToFunction(temp);

			//cout<<"\nHere2"<<temp<<"\n";

			//if(final_shifted_inlined_dag == NULL) cout<<"\nNULL\n";

			//cout<<"\n\nDAG for inling is : "<<final_shifted_inlined_dag->getDAGStr();

			cout<<"\n\nEx.variables to be eliminated:";

			for(setExQuantify_it = setExQuantify.begin(); setExQuantify_it != setExQuantify.end(); setExQuantify_it++)

			  	{
				  string element = *(setExQuantify_it);

				  cout<<"\t"<<element;
				}

			start_time = clock();	

			//call_findPlacesOfInlining(setExQuantify,final_shifted_inlined_dag);

			final_shifted_inlined_dag = Inlining(final_shifted_inlined_dag, setExQuantify);

			cout<<"\n\nTime needed for inlining = "<< ((double)clock()-start_time)/CLOCKS_PER_SEC <<endl<<endl;

			//cout<<"\n\nDAG after inling is :"<<final_shifted_inlined_dag->getDAGStr();

			// Getting all variables in final_shifted_inlined_dag

			final_shifted_inlined_dag->getDAGNames(setVars);

							
			for(setVars_it = setVars.begin(); setVars_it != setVars.end(); setVars_it++)

			  	{
					  string element = *(setVars_it);
					  if(!is_output_input(element, bb->get_Output_identifiers(), bb->get_Input_identifiers()))
					    {
					      setExQuantifySecond.insert(element);				
					    }
				}

			setVars.clear();

			//set<string> setDiffer2;

			//set_difference(setExQuantify.begin(), setExQuantify.end(), setExQuantifySecond.begin(), setExQuantifySecond.end(),inserter(setDiffer2, setDiffer2.begin()));

				
			//existQuant = getListString(setDiffer2);
				
			//cout<<"\n\nEx.variables eliminated:\t"<<existQuant;

			//cout<<"\n\nNo: of Ex.variables eliminated:\t"<<setDiffer2.size();

			//cout<<"\n\nNo: of Ex.variables still to be eliminated:\t"<<setExQuantifySecond.size();

			existQuant = getListString(setExQuantifySecond);
				
			// final_shifted_inlined_dag CONTAINS THE SHIFTED INLINED DAG without the ("exists", remaining variables, part. Remaining variables are in setExQuantifySecond

			// INLINING CODE ENDS HERE
				
			// SCOPE REDUCTION CODE STARTS HERE

			CDAGNode* final_shifted_inlined_sr_dag;

			if(existQuant == "")
			  {
			    final_shifted_inlined_sr_dag = final_shifted_inlined_dag;
			  }
			else
			  {
			    map<CDAGNode*,CDAGNode*> temp8;

			    set<string> temp9;
				
			    CDAGNode *temp6 = MANAGER.cloneAndShift(final_shifted_inlined_dag, 0);
			    CDAGNode *temp7 = convertIteToFunction(temp6);
			    if(temp7 == NULL) cout<<"\nNULL\n";

			    cout<<"\n\nDAG for scope reduction is : "<<temp7->getDAGStr();
			
			    cout << endl<< endl << "setExQuantifySecond.size()=" << setExQuantifySecond.size() << endl;
			
			    final_shifted_inlined_sr_dag = scopeReduction(setExQuantifySecond,temp7);
			  }

			//cout<<"\n\nSR1 OVER\n\n";

			*/

			/* TEMPORARILY COMMENTING THE OTHER TWO SREDUCTION STRATEGIES */

			//************************************

			/*

			map<CDAGNode*,CDAGNode*> temp18;

			set<string> temp19;
				
			CDAGNode *temp16 = MANAGER.cloneAndShift(final_shifted_inlined_dag_for_sr, 0);
			CDAGNode *temp17 = convertIteToFunction(temp16);
			if(temp17 == NULL) cout<<"\nNULL\n";
				
				
			scopeReduction2(setExQuantifySecond,temp17);

			cout<<"\n\nSR2 OVER\n\n";

			*/
				
			//*****************************************

			/*

			map<CDAGNode*,CDAGNode*> temp28;

			set<string> temp29;
				
			CDAGNode *temp26 = MANAGER.cloneAndShift(final_shifted_inlined_dag_for_sr, 0);
			CDAGNode *temp27 = convertIteToFunction(temp26);
			if(temp27 == NULL) cout<<"\nNULL\n";
				
			scopeReduction3(setExQuantifySecond,temp27);

			cout<<"\n\nSR3 OVER\n\n";

			*/

				


			// SCOPE REDUCTION CODE ENDS HERE
					
				

			setDone.clear();

			final_shifted_inlined_sr_dag->getDAGNames(setDone);

			
			map<CSensitiveElement*, string> delays; // Delay with each input

			initializeDeltaMapForBB(delays, bb, affecting, nodePartOfBasicBlock);


			// THE FOLLOWING LINE IS COMMENTED AND NEW LINE ADDED ON 11/1/2010 AS BUG IN calculateDeltaDelays FOUND WHILE TESTING : SEE readme.txt

			//final_shifted_inlined_sr_dag->calculateDeltaDelays(delays); // Calculate delta time of each input from the dag for SE

			final_shifted_inlined_sr_dag->calculateDeltaDelaysModified(delays); // Calculate delta time of each input from the dag for SE


			// Finding the delays by substracting the delta time from the delta time of the output

			map<CSensitiveElement*, string>::iterator delays_it;

			for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
			  {
			    char temp[20];
			    //sprintf(temp, "%d", delta_time - atoi(delays_it->second.c_str()));

			    // For inputs which are present in the sensitivity list; but not in the symbolic expression, the delay is set as the delta time of the component; for others it is the absolute value of the delay
			    
			    if(delays_it->second == "") 
			      {
				sprintf(temp, "%d", delta_time); 
			      }
			    else
			      {
				sprintf(temp, "%d", -1*atoi(delays_it->second.c_str()));
			      }
				
			    delays_it->second = temp;
			   }
			
			//cout<<"\n\nDELAYS\n\n";
		  
			//for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
			//  {
			//    cout<<"name = "<<delays_it->first->name<<"\t"<<delays_it->first->lower<<"\t"<<delays_it->first->upper<<"\tdelay="<<delays_it->second<<endl;
			//  }

						
			//RelationInputOutput *rio = new RelationInputOutput(delta_time, final_dag, present_condition, fr->ranges_covered, setDone);
			SymbolicRelation *rio = new SymbolicRelation(delays, final_shifted_inlined_sr_dag, fr->ranges_covered, setDone);

			
			//bb->table[(*iout)->name].push_back(rio);

			bb->table[(*iout)->name] = rio;
	
	}

	//cout<<"\n\nReached Here2\n\n";
}


string CSymbSimulator::getListString(set<string> &setExQuantify)
{
	string existQuant;
	
	//cout<<endl<<"\nHere0";
	
	set<string>::iterator iset = setExQuantify.begin();

	//cout<<endl<<"\nHere1";
	
	if(iset == setExQuantify.end())
	{
	  //cout<<endl<<"\nHere3";
		return "";
	}

	//cout<<endl<<"\nHere2";

	existQuant = *iset;

	//cout<<endl<<existQuant;
	
	iset++;
	
	for(; iset != setExQuantify.end(); iset++)
	{
		existQuant += ","+ *iset;
		//cout<<endl<<existQuant;
	}
	
	return existQuant;
}


void CSymbSimulator::insertToSymbolicQueue(int delta_time, CDAGNode* relation_shifted, CDAGNode* condition, set<string>& setCondition, ConditionsValues& cv, vector<pair<int, int> > ranges_covered )
{
  //cout<<"inserting..."<<endl;
  //cout<<relation_shifted->getDAGStr()<<endl;
  //cout<<condition->getDAGStr()<<endl;

	vector<int>::iterator  idel = cv.delta.begin();
	vector<CDAGNode*>::iterator ival = cv.values.begin();
	vector<CDAGNode*>::iterator  icond = cv.conditions.begin();
	vector< set<string> >::iterator isetc = cv.setConditions.begin();
	vector<vector<pair<int, int> > >::iterator irange = cv.ranges_covered.begin();

	bool already_presnt = false;

	for(;idel != cv.delta.end(); idel++, ival++, icond++, isetc++, irange++)
	{
		if(delta_time == *idel)
		{
			already_presnt = true;
			break;
		}

		if(delta_time < *idel)
		{
			break;
		}
	}

	if(already_presnt)
	{

	  //cout<<"\n\nAN ENTRY IS ALREADY PRESENT AT THE SAME DELTA\n\n";
	  
	  /*There is an entry already present*/
	  /* Check if the range of the new entry is already present in cv.ranges_covered. If yes, do nothing. Else, attach the condition of the new entry to the existing condn */

	  /* vector<pair<int, int> > present_range = (*irange);
	  
	  bool flag = false;

	  for(int i=0; i<present_range.size(); i++)
	    { 
	      if( (present_range[i].first == ranges_covered[0].first) && (present_range[i].second == ranges_covered[0].second))
		flag = true;
	    }

	  if(flag == false)
	    cout<<"\n\nALREADY PRESENT. HENCE NO ACTION\n\n";
	  else
	    {
	      cout<<"\n\nALREADY PRESENT. HOWEVER THE RANGES ARE DIFFERENT. HENCE MODIFYING THE CONDITION\n\n";
	      (*icond) = MANAGER.createDAG("or", condition, (*icond));
	      (*irange).insert(ranges_covered[0]);
	      }*/

	}
	else
	{
		cv.delta.insert(idel, delta_time);
		cv.values.insert(ival, relation_shifted);
		cv.conditions.insert(icond, condition);
		cv.setConditions.insert(isetc, setCondition);
		cv.ranges_covered.insert(irange, ranges_covered);
	}
}

CDAGNode * CSymbSimulator::calculateEventCondition(int delta_time, ConditionsValues& cv, CSensitiveElement* se, set<string>& setCondition)
{
/*	cv.delta;
	cv.values;
	cv.conditions;*/

	int lower = atoi(se->lower.c_str());
	int upper = atoi(se->upper.c_str());

	CDAGNode* temp = MANAGER.createDAG("true");

	int size = cv.delta.size();

	int i;

	bool exitDueToBreak = false;

	//cout<<"\n\nHere1";
	
	for(i = 0; i < cv.delta.size()-1; i++)
	{
		if(!temp->equals(cv.conditions[i]))
		{
			exitDueToBreak = true;
			break;
		}
	}

	//cout<<"\n\nHere2";

	CDAGNode * dag1 = NULL;
	CDAGNode * dag_true = NULL;

	//cout<<"\n\nHere3";
	
	if(exitDueToBreak)
	{
		dag_true = MANAGER.createDAG("select", cv.values[i], lower, upper);
		//cout<<"\n\nHere3,if 1";
		setCondition.insert(cv.values[i]->getDAGStr());
		//cout<<"\n\nHere3,if 2";
	}
	else
	{
		dag_true = MANAGER.createDAG("select", cv.values[i-1], lower, upper);
		//cout<<"\n\nHere3,else 1";
		setCondition.insert(cv.values[i-1]->getDAGStr());
		//cout<<"\n\nHere3,else 2";
	}
	

	//cout<<"\n\nHere4";

	if(i == 0)
	{
		CDAGNode * dag_false = MANAGER.createDAG("select", cv.values[i-1], lower, upper);
		setCondition.insert(cv.values[i-1]->getDAGStr());

		dag1 = MANAGER.createDAG("ite", cv.conditions[i], dag_true, dag_false);
		
		setCondition.insert(cv.setConditions[i].begin(), cv.setConditions[i].end());

	}
	else
	{
		dag1 = dag_true;
	}

	//cout<<"\n\nHere5";

	//cout<<dag1->getDAGStr()<<endl;

	for(;i < cv.delta.size()-1; i++)
	{
		CDAGNode * dag_true = MANAGER.createDAG("select", cv.values[i], lower, upper);
		setCondition.insert(cv.values[i]->getDAGStr());

		CDAGNode * dag2 = MANAGER.createDAG("ite", cv.conditions[i], dag_true, dag1);
		setCondition.insert(cv.setConditions[i].begin(), cv.setConditions[i].end());


		dag1 = dag2;
	}

	//cout<<"\n\nHere6";

	CDAGNode * dag_current = MANAGER.createDAG("select", cv.values[cv.delta.size()-1], lower, upper);
	setCondition.insert(cv.values[cv.delta.size()-1]->getDAGStr());

	//cout<<dag_current->getDAGStr()<<endl;


	CDAGNode * dag3 = MANAGER.createDAG("is_not_equal", dag_current, dag1);
	//cout<<dag3->getDAGStr()<<endl;

	CDAGNode * final_expression = MANAGER.createDAG("and", cv.conditions[cv.delta.size()-1], dag3);
	setCondition.insert(cv.setConditions[cv.delta.size()-1].begin(), cv.setConditions[cv.delta.size()-1].end());

	//cout<<final_expression->getDAGStr()<<endl;

	//cout<<"\n\nHere7";

	return final_expression;
}




void CSymbSimulator::PerformCyclicSymbolicSimulation(list<CBasicBlock*> &basicblocks, CEntity& ent)
{
	//TODO
	int TIMES_TO_SIMULATE = 10;
	list<AffBlocksAndDelta*> affectedBasicBlocksNew;


	for(int i = 2; i< TIMES_TO_SIMULATE; i++)
	{
		list<AffBlocksAndDelta *> ::iterator ibb = affectedBasicBlocks.begin();
		int sz = affectedBasicBlocks.size();

		for(;ibb != affectedBasicBlocks.end(); ibb++)
		{
			CDAGNode *final_condition = calculateConditions((*ibb)->bb, i);
			//PerformSymbolicSimulation((*ibb)->block_nodes, ent, i, i - 1);

			list<CBasicBlock *> ::iterator iout = (*ibb)->bb->out_list.begin();
			int sz1 = (*ibb)->bb->out_list.size();

			CDAGNode* triggering_condition ;//= calculateEventCondition(1, SymbolicQueues[identifier]);

			for(;iout != (*ibb)->bb->out_list.end(); iout++)
			{

				//insertToAffectedBB(1, *iout, triggering_condition);
			}
		}

		int size = affectedBasicBlocksNew.size();

		affectedBasicBlocks.clear();
		affectedBasicBlocks = affectedBasicBlocksNew;

		size = affectedBasicBlocks.size();
		affectedBasicBlocksNew.clear();

		printSymbolicValues(ent, i);
		advanceDeltaTime(ent);
	}
}

CDAGNode * CSymbSimulator::calculateConditions(CBasicBlock *bb, int current)
{
	CDAGNode *final_condition = NULL;



	return final_condition;
}


void CSymbSimulator::PerformSymbolicSimulation(CBasicBlock * bb, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues)
{
	list<node *> &sortedNodes = bb->block_nodes;

	list<node *> ::iterator inode = sortedNodes.begin();

	map<string, bool> previously_changed;

	//list<int> deltas;

	map<int, list<string> > deltas;

	//deltas.push_back(1);

	initializeDeltasForBBSimulation(bb, deltas);

	//showDeltasForBBSimulation(deltas);

	//cout<<"\n\nPerformSymbolicSimulation\n\n";

	for(;inode != sortedNodes.end();inode++)
	{
		switch((*inode)->arch_stmt->getType())
		{
			case CONCURRENT_ASSIGNMENT_STMT:
			        deltas = PerformSymbolicSimulationConAssign(bb, (*inode)->arch_stmt->stmt.conc_assign_stmt, ent, SymbolicQueues, deltas);
			break;

			case PROCESS_STMT:
			       //cout<<"\n\nPerformSymbolicSimulation of Process Statement\n\n";
				deltas= PerformSymbolicSimulationProcess(bb, (*inode)->arch_stmt->stmt.process_stmt, ent, SymbolicQueues, deltas);
			break;

			case COMPONENT_INSTANTIANTION_STMT:
				deltas = PerformSymbolicSimulationCompInstant(bb, (*inode)->arch_stmt->stmt.instance_stmt, ent, SymbolicQueues, deltas);
			break;
		}
	}
}

map<int, list<string> > CSymbSimulator::PerformSymbolicSimulationConAssign(CBasicBlock * bb, CConSignAssgnStmt * conc_assign_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, map<int, list<string> >  &deltas)
{
         //list<int> deltas_new;
         map<int, list<string> > deltas_new;

	set<string> setRelation;
	set<string> setRelation1;

	//cout<<"\n\nInside Perf.Symb.Simu.Con.Assign\n\n";

	while(deltas.size() != 0)
	{
	        //int current = deltas.front();
	        //deltas.pop_front();

	        int current;

		list<string> InputsAtCurrent;

		getInputsAtSmallestDelta(deltas, current, InputsAtCurrent);

		//cout<<"\n\nDeltas for BBSimulation after deleting the least entry i.e. at "<<current<<endl;

		//showDeltasForBBSimulation(deltas);

	  
		COperand *lhs = conc_assign_stmt->getLHS();
		CDAGNode *final_expression = NULL;
		CDAGNode * dag1 = NULL;

		list<CExpression*> values = (conc_assign_stmt)->getValues();
		list<CExpression*> choices = (conc_assign_stmt)->getChoices();

		list<CExpression*> ::iterator ichoice = choices.begin();
		list<CExpression*> ::iterator ivalue = values.begin();
		//cout<<"you are here... sizes "<<choices.size() <<"  "<<values.size() <<endl;

		if((*ichoice)->getOperand() == "others")
		{
		  //cout<<"\n\nInside if ichoce -> getOperand = others\n";
			dag1 = (*ivalue)->getDAG(ent.getName(), current, setRelation);
			//cout<<dag1->getDAGStr()<<endl;
		}
		else if(choices.size() == 1)
		{
		  //cout<<"\n\nInside choices.size()=1\n";
		  //cout<<"\nSele.Expression=\t"<<conc_assign_stmt->getSelExpr()<<endl;
			/*if(conc_assign_stmt->getSelExpr() != "")
			{
				CDAGNode *sel_expr = MANAGER.createDAG(conc_assign_stmt->getSelExpr());
				dag1 = MANAGER.createDAG("=", sel_expr, (*ichoice)->getDAG(ent.getName(), current, setRelation) );
				cout<<"\ndag1=\t"<<dag1->getDAGStr()<<endl;
				}*/
			if(conc_assign_stmt->getOriginalSelExpr() != NULL)
			{
				CDAGNode *sel_expr = conc_assign_stmt->getOriginalSelExpr()->getDAG(ent.getName(), current, setRelation);
				dag1 = MANAGER.createDAG("=", sel_expr, (*ichoice)->getDAG(ent.getName(), current, setRelation) );
				//cout<<"\ndag1=\t"<<dag1->getDAGStr()<<endl;
				}
			else
			{
				dag1 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
				//cout<<"\ndag1=\t"<<dag1->getDAGStr()<<endl;
			}

			final_expression = MANAGER.createDAG("ite", dag1, (*ivalue)->getDAG(ent.getName(), current, setRelation), lhs->getDAG(ent.getName(), current, setRelation));
			//cout<<final_expression->getDAGStr()<<endl;
		}
		else
		{
			list<CExpression*> ::reverse_iterator ichoice = choices.rbegin();
			list<CExpression*> ::reverse_iterator ivalue = values.rbegin();

			//cout<<"\n\nInside Else.....\n\n";
		
			if((*ichoice)->getOperand() == "others")
			{
				dag1 = (*ivalue)->getDAG(ent.getName(), current, setRelation);
				//cout<<"\n\nErraneous Others\t"<<dag1->getDAGStr()<<endl;
			}
			else
			{
				CDAGNode * dag2 = NULL;
				if(conc_assign_stmt->getSelExpr() != "")
				{
					CDAGNode *sel_expr = MANAGER.createDAG(conc_assign_stmt->getSelExpr());
					dag2 = MANAGER.createDAG("=", sel_expr, (*ichoice)->getDAG(ent.getName(), current, setRelation) );
				}
				else
				{
					dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
				}

				dag1 = MANAGER.createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), lhs->getDAG(ent.getName(), current, setRelation));
			}

			ichoice++; ivalue++;			

			for(;ichoice != choices.rend(); ichoice++,ivalue++)
			{
				CDAGNode * dag2 = NULL;
				//cout<<"\n\nSelection Expression\t"<<conc_assign_stmt->getSelExpr()<<endl;
				if(conc_assign_stmt->getSelExpr() != "")
				{
					CDAGNode *sel_expr = MANAGER.createDAG(conc_assign_stmt->getSelExpr());
					dag2 = MANAGER.createDAG("=", sel_expr, (*ichoice)->getDAG(ent.getName(), current, setRelation) );
					//cout<<"\ndag2 selExpr!null =\t"<<dag2->getDAGStr()<<endl;
				}
				else
				{
	
					dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
					//cout<<"\ndag2 =\t"<<dag2->getDAGStr()<<endl;
				}

				CDAGNode * dag3 = MANAGER.createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), dag1);
				dag1 = dag3;
			}
		}

		//cout<<"\n\nLHS of Conc. Assignment is \n\n";

		lhs->print();

		CDAGNode* lhs_dag = lhs->getDAG(ent.getName(), current+1, setRelation1);

		final_expression = MANAGER.createDAG("equivalent", lhs_dag, dag1);
		//cout<<final_expression->getDAGStr()<<endl;

		CDAGNode* true_cond = MANAGER.createDAG("true");

		bb->constraints.push_back(final_expression);
		
		int low = atoi(((CAtom*)lhs)->getLower().c_str());
		int up = atoi(((CAtom*)lhs)->getUpper().c_str());
		pair<int, int> range(low, up);

		ConditionRelation* cr = new ConditionRelation(true_cond, final_expression);
		cr->setRelation.insert(setRelation.begin(), setRelation.end());
		cr->ranges_covered.push_back(range);

		char curr_delta[5];
		sprintf(curr_delta, "%d", current+1);

		string id_delta = lhs->getName() + "_" + curr_delta;
		//char *temp = strdup(id_delta.c_str());

		string id = lhs->getName();

		//bb->mapSignalDelta[id_delta].push_back(cr);

		//bb->mapSignalDelta[id].push_back(cr);

		updateMapSignalDelta(bb->mapSignalDelta, cr, id, current+1, id_delta);

		//think
		CDAGNode* ddd = MANAGER.createDAG(lhs->getName(), current+1);
		bb->mapRelation[*ddd] = final_expression;

		SymbolicQueues[lhs->getName()].delta.push_back(current+1);
		SymbolicQueues[lhs->getName()].values.push_back(ddd);

		//TODO
		SymbolicQueues[lhs->getName()].conditions.push_back(true_cond);
		
		set<string> emptySet;
		SymbolicQueues[lhs->getName()].setConditions.push_back(emptySet);
		SymbolicQueues[lhs->getName()].ranges_covered.push_back(cr->ranges_covered);
		

		//deltas_new.push_back(current + 1);
		//deltas_new.sort();
		//deltas_new.unique();

		deltas_new[current + 1].push_back(lhs->getName());

		//cout<<"\n\nDeltas for BBSimulation after inserting "<<lhs->getName()<<" at "<<current+1<<endl;

		//showDeltasForBBSimulation(deltas_new);
	}

	return deltas_new;
}

void CSymbSimulator::assignToSymbolicValues(COperand *lhs, CDAGNode *final_expression, CEntity &ent, int current)
{
/*	CAtom *at = (CAtom*) lhs;

	int lower = atoi(at->getLower().c_str());
	int upper = atoi(at->getUpper().c_str());

	string id_name = at->getName();
	string ent_name = ent.getName();

	ConditionsValues &rv = SymbolicValues[ent_name][id_name];
//	vector< pair<int,int> > &atom_ranges = rv.atom_ranges;
	vector < vector<CDAGNode*> > &values = rv.values;

	vector<int> atom_indices = MANAGER.findAtomsOfLHS(atom_ranges, lower, upper);

	int i = 0;
	int start = 0;
	for(;i < atom_indices.size(); i++)
	{
		int index = atom_indices[i];
		int end = atom_ranges[index].second - atom_ranges[index].first + start;
		char temp[20];
		sprintf(temp, "%d", start);
		CDAGNode *st_dag = MANAGER.createDAG(temp);

		sprintf(temp, "%d", end);
		CDAGNode *en_dag = MANAGER.createDAG(temp);

		// TODO
		// int current = 1;
		values[current][index] = MANAGER.createDAG("select", final_expression, st_dag, en_dag);
		start = end+1;
	}*/
}

map<int, list<string> >  CSymbSimulator::PerformSymbolicSimulationProcess(CBasicBlock * bb, CProcess * process_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, map<int, list<string> > &deltas)
{
  //list<int> deltas_new;
  map<int, list<string> > deltas_new;

	while(deltas.size() != 0)
	{
	  //int current = deltas.front()+1;
	  //	deltas.pop_front();

		int current;

		list<string> InputsAtCurrent;

		getInputsAtSmallestDelta(deltas, current, InputsAtCurrent);

		//cout<<"\n\nDeltas for BBSimulation after deleting the least entry i.e. at "<<current<<endl;

		//showDeltasForBBSimulation(deltas);


		//map< string, list<RelationInputOutput*> >& table = process_stmt->table;

		map< string, SymbolicRelation*>& table = process_stmt->table;

		//map< string, list<RelationInputOutput*> >::iterator itab = table.begin();

		map< string, SymbolicRelation*>::iterator itab = table.begin();

		for(;itab != table.end(); itab++)
		{

			string output_identifier = itab->first;
			//list<RelationInputOutput*> & entries = itab->second;
			SymbolicRelation* Symb_Exp = itab->second;

			//for(list<RelationInputOutput*>::iterator irio = entries.begin();
			//	irio != entries.end();
			//	irio++)
			//	{

			//	int delta_time = (*irio)->delta_time;
			//	CDAGNode* relation = (*irio)->relation;
				//CDAGNode* condition = (*irio)->condition;

			
			CDAGNode* relation = Symb_Exp->relation;
				set<string> setRelation;

				int shift = current;

				//cout<<"\n\nSE for Process = "<<relation->getDAGStr()<<endl;

				CDAGNode* relation_shifted;

				//cout<<"\nShift = "<<shift<<"\n\n"; 

				if(shift == 0)
				{
					relation_shifted = relation;
					//setRelation.insert((*irio)->setRelation.begin(), (*irio)->setRelation.end());
					setRelation.insert(Symb_Exp->setRelation.begin(), Symb_Exp->setRelation.end());
					//cout<<setRelation.size()<<endl;
				}
				else
				{
					map<CDAGNode*,CDAGNode*> nodeShiftednode;
					relation_shifted = MANAGER.cloneAndShift(relation, shift);
					nodeShiftednode.clear();
				}

				//cout<<"\n\nShifted SE for Process = "<<relation_shifted->getDAGStr()<<endl;

				//cout<<relation_shifted->getDAGStr()<<endl;

				ent.constraints.push_back(relation_shifted);
				

				CDAGNode* out_dag = MANAGER.createDAG(output_identifier, current + 1);

				//cout<<out_dag->getDAGStr()<<endl;

				char str_current[20];

				sprintf(str_current, "%d", current + 1);

				string id_delta = output_identifier + "_" + str_current;
				
				string id = output_identifier;

				CDAGNode* true_cond = MANAGER.createDAG("true");

				ConditionRelation *cr = new ConditionRelation(true_cond, relation_shifted);

				//setRelation.erase(id_delta);

				setRelation.erase(id);

				cr->setRelation.insert(setRelation.begin(), setRelation.end());
				//cr->ranges_covered = (*irio)->ranges_covered;
				cr->ranges_covered = Symb_Exp->ranges_covered;

				//bb->mapSignalDelta[id_delta].push_back(cr);

				//bb->mapSignalDelta[id].push_back(cr);

				updateMapSignalDelta(bb->mapSignalDelta, cr, id, current + 1, id_delta);
				
				set<string> empty_set;

				//insertToSymbolicQueue(current + delta_time - 2, out_dag, true_cond, empty_set, SymbolicQueues[output_identifier], (*irio)->ranges_covered);

				insertToSymbolicQueue(current + 1, out_dag, true_cond, empty_set, SymbolicQueues[output_identifier], Symb_Exp->ranges_covered);

				//deltas_new.push_back(current + delta_time - 2);
				
				//deltas_new.sort();
				//deltas_new.unique();
				//	}

				deltas_new[current + 1].push_back(output_identifier);

				//cout<<"\n\nDeltas for BBSimulation after inserting "<<output_identifier<<" at "<<current + 1<<endl;

				//showDeltasForBBSimulation(deltas_new);
				
		}
	}

	return deltas_new;
}

void CSymbSimulator::getactualFormalRealtion(CInstance * instance_stmt, map<string, CExpression*>& actualFormal)
{
	list<CExpression*> params= (instance_stmt)->getActualParam();
	list<CExpression*> ::iterator iparam= params.begin();

	list<COperand*> formals= (instance_stmt)->getFormalParam();
	list<COperand*> ::iterator iformal = formals.begin();

	if(formals.size() != 0)
	{
		for(	iparam = params.begin();
			iparam != params.end();
			iparam++,iformal++
		)
		{
			actualFormal[(*iformal)->getName()] = (*iparam);
		}
	}
	else
	{
		int index = instance_stmt->getIndex();
		CEntity &comp = Entities[index];
		
		list<CPort>& ports = comp.getPorts();
		list<CPort>::iterator iport = ports.begin();

		for(iparam = params.begin();
			iparam != params.end();
			iparam++, iport++
		)
		{
			actualFormal[iport->getName()] = (*iparam);
		}
	}	
}

/*void CSymbSimulator::delete_and_add(ReplaceNode* rn)
{
	CDAGNode *old_node = rn->old_node;
	CDAGNode * new_node = rn->new_node;
	
	cout<<old_node->getDAGStr()<<endl;
	cout<<new_node->getDAGStr()<<endl;
	
	list<CDAGNode *> &node_list = old_node->in_list;
	list<CDAGNode *>::iterator iter;

	for(iter = node_list.begin(); iter != node_list.end(); iter++)
	{
		list<CDAGNode *>::iterator iter_inner;
		
		for(iter_inner= (*iter)->out_list.begin();
			iter_inner != (*iter)->out_list.end();
			iter_inner++)
		{
			if((*iter_inner) == old_node)
			{
				break;
			}
		}
		list<CDAGNode *>::iterator iter1 = (*iter)->out_list.erase(iter_inner);

		(*iter)->out_list.insert(iter1, new_node);
	}


}*/

/*list<int>  CSymbSimulator::PerformSymbolicSimulationCompInstant(CBasicBlock * bb, CInstance * instance_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, list<int> &deltas)
{
	list<int> deltas_new;
	
	int index = instance_stmt->getIndex();
	CEntity &comp = Entities[index];

	map< string, list<RelationInputOutput*> >& table = comp.getTable();

	map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
	
	map<string, CExpression*> actualFormal;
	
	getactualFormalRealtion(instance_stmt, actualFormal);
	

	while(deltas.size() != 0)
	{
		int current = deltas.front()+1;
	    deltas.pop_front();

		for(;itab != table.end(); itab++)
		{
			string output_identifier = itab->first;
			list<RelationInputOutput*> & entries = itab->second;

			for(list<RelationInputOutput*>::iterator irio = entries.begin();
				irio != entries.end();
				irio++)
			{
				int delta_time = (*irio)->delta_time;
				CDAGNode* relation = (*irio)->relation;
				//CDAGNode* condition = (*irio)->condition;
				set<string> setRelation;

				int shift = current - 2;

				cout<<relation->getDAGStr()<<endl;

				CDAGNode* relation_shifted;

				if(shift == 0)
				{
					relation_shifted = relation;
					list<ReplaceNode*> replaceNodes;
					replaceNodes.clear();
					
					replace(relation, actualFormal, setRelation, ent, replaceNodes);
					
					for(list<ReplaceNode*>::iterator irn = replaceNodes.begin();
						irn != replaceNodes.end();
						irn++)
					{
						delete_and_add(*irn);
					}
				}
				else
				{
					map<CDAGNode*,CDAGNode*> nodeShiftednode;
					relation_shifted = MANAGER.cloneAndShift(relation, shift);
					
					list<ReplaceNode*> replaceNodes;
					replace(relation_shifted, actualFormal, setRelation, ent, replaceNodes);

					for(list<ReplaceNode*>::iterator irn = replaceNodes.begin();
						irn != replaceNodes.end();
						irn++)
					{
						delete_and_add(*irn);
					}
					
					
					nodeShiftednode.clear();
				}

				cout<<relation_shifted->getDAGStr()<<endl;

				ent.constraints.push_back(relation_shifted);
			
				CDAGNode* out_dag = MANAGER.createDAG(output_identifier, current + delta_time - 2);

				cout<<out_dag->getDAGStr()<<endl;

				char str_current[20];

				sprintf(str_current, "%d", current + delta_time - 2);

				string id_delta = output_identifier + "_" + str_current;

				CDAGNode* true_cond = MANAGER.createDAG("true");

				ConditionRelation *cr = new ConditionRelation(true_cond, relation_shifted);

				setRelation.erase(id_delta);

				cr->setRelation.insert(setRelation.begin(), setRelation.end());
				cr->ranges_covered = (*irio)->ranges_covered;

				bb->mapSignalDelta[id_delta].push_back(cr);

				set<string> empty_set;

				insertToSymbolicQueue(current + delta_time - 2, out_dag, true_cond, empty_set, SymbolicQueues[output_identifier], (*irio)->ranges_covered);

				deltas_new.push_back((*irio)->delta_time + current);
			}
		}
	}

	return deltas_new;
}*/

map<int, list<string> >  CSymbSimulator::PerformSymbolicSimulationCompInstant(CBasicBlock * bb, CInstance * instance_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, map<int, list<string> > &deltas)
{
  //list<int> deltas_new;

  clock_t start_time;

  map<int, list<string> > deltas_new;

        map<string,CDAGNode*>::iterator irn; // changed on 5/9/08        
	
	int index = instance_stmt->getIndex();

	CEntity &comp = Entities[index];

	//map< string, list<RelationInputOutput*> >& table = comp.getTable();

	//map< string, list<RelationInputOutput*> >::iterator itab = table.begin();

	map<string, SymbolicRelation*>& table = comp.getTable();

	map<string, CExpression*> actualFormal;

        string ExQuantifyList; // added on 5/9/08

	set <string>ExQuantifySet; // Set of ex.quant. variables to be elimiated added on 26/12/08

	int ExQuantifyCount, ExQuantifyIndex; // added on 5/9/08
	
	getactualFormalRealtion(instance_stmt, actualFormal);
	

	while(deltas.size() != 0)
	{
	  //int current = deltas.front()+1;
	  //deltas.pop_front();

	  int current;

	  list<string> InputsAtCurrent;

	  getInputsAtSmallestDelta(deltas, current, InputsAtCurrent);

	  //cout<<"\n\nDeltas for BBSimulation after deleting the least entry i.e. at "<<current<<endl;

	  //showDeltasForBBSimulation(deltas);

	  for(list<string>::iterator InputsAtCurrent_it = InputsAtCurrent.begin(); InputsAtCurrent_it != InputsAtCurrent.end(); InputsAtCurrent_it++)
	    {
	      string Actual_TriggeringInput = (*InputsAtCurrent_it); // Actual Input triggering the comp inst

	      //cout<<"\n\nActual Triggering Input = "<<Actual_TriggeringInput<<"....Finding the formal input corresponding to this"<<endl<<endl;

	      string Formal_TriggeringInput = findFormalTriggeringInput(actualFormal, Actual_TriggeringInput, ent);

	      //cout<<"\n\nFormal Triggering Input = "<<Formal_TriggeringInput<<endl<<endl;

	      for(map<string, SymbolicRelation*>::iterator itab = table.begin();itab != table.end(); itab++)
		{
			string output_identifier = itab->first;

			//cout<<"\n\nOUTPUT IS "<<output_identifier<<endl<<endl;

			//list<RelationInputOutput*> & entries = itab->second;

			SymbolicRelation* entry = itab->second;

			/*for(list<RelationInputOutput*>::iterator irio = entries.begin();
				irio != entries.end();
				irio++)
			{*/

				map<CSensitiveElement*, string> delay_map = entry->delays;

				// THIS LINE CDAGNode* relation = entry->relation; IS TEMPORARILY ADDED WHEN renameExVariablesInTree WAS COMMENTED

				//CDAGNode* relation = entry->relation;

				
				CDAGNode* relation = createCloneWithInternalVarsRenamed(entry->relation, comp.getName(), instance_stmt) ; // Replacing internal variables in the instance by entityname_instancename_variablename
				
				//int entry_time = entry->delta;

				
				int delay_time = findDeltaTimeOfInputString(delay_map, Formal_TriggeringInput);
		

				int present_time = current + delay_time;

				//int shift = present_time - entry_time;

				int shift = present_time;

				set<string> setRelation;
				
				//cout<<"\n\nRelaton is "<<relation->getDAGStr()<<endl<<endl;
				
				//cout<<"\n\nThis was valid at "<<entry_time<<" where as we need relation at "<<present_time<<". Hence shifting by "<<shift<<endl<<endl;
				//cout<<"\n\nThis was valid at time 0 where as we need relation at "<<present_time<<". Hence shifting by "<<shift<<endl<<endl;

				CDAGNode* relation_shifted;

				map<string, CDAGNode*> replaceNodes;

				map<CDAGNode*, CDAGNode*> replaceDAGNodes;// Newly added on 30th June 2009 for expression manager

			       if(shift == 0)
				{

				  //cout<<"\n\nNo shift. \n\n";
					relation_shifted = relation;
					//map<string, CDAGNode*> replaceNodes;
					replaceNodes.clear();
					
					replaceDAGNodes.clear();// Newly added on 30th June 2009 for expression manager

					replace(relation_shifted, actualFormal, setRelation, ent, replaceNodes);

					
					for(irn = replaceNodes.begin();
						irn != replaceNodes.end();
						irn++)
					{
					        //string id_delta1 = irn->first;
						//int index = id_delta1.find_last_of("_");
						//string id = id_delta1.substr(0, index);
						//int del = atoi(id_delta1.substr(index+1).c_str());
						
						//CDAGNode* dag1 = MANAGER.createDAG("equivalent", irn->second, MANAGER.createDAG(id, del));;
						//relation_shifted = MANAGER.createDAG("and", dag1, relation_shifted);
						//delete_and_add(*irn);

						string id_delta1 = irn->first;
						int index = id_delta1.find_last_of("_");
						string id = id_delta1.substr(0, index);
						int del = atoi(id_delta1.substr(index+1).c_str());

						// Finding the indices of formal arguments

						// COMMENTED FOR FINAL INTEGRATION ON 17th June 09

						//list<CDAGNode*> actual_out_list = (irn->second)->out_list;
						//list<CDAGNode*>::iterator actual_out_list_it = actual_out_list.begin();

						//actual_out_list_it++;
						//int formal_lower = atoi(((*actual_out_list_it)->name).c_str());
						//actual_out_list_it++;
						//int formal_upper = atoi(((*actual_out_list_it)->name).c_str());
						
						// COMMENTED FOR FINAL INTEGRATION ON 17th June 09 ENDS HERE
						
						list<CDAGNode*>::iterator actual_out_list_it = (irn->second)->Get_DAGnode_outlist();
						
						actual_out_list_it++;
						int formal_lower = atoi(((*actual_out_list_it)->Get_DAGnode_value()).c_str());
						actual_out_list_it++;
						int formal_upper = atoi(((*actual_out_list_it)->Get_DAGnode_value()).c_str());
						
						int formal_size = formal_upper - formal_lower;

						replaceDAGNodes.insert(pair<CDAGNode*,CDAGNode*>(MANAGER.createDAG(id, 0, formal_size, del), irn->second));
						
						// The following lines are commented on 30th June 2009 for expression manager


						//CDAGNode* dag1 = MANAGER.createDAG("equivalent", MANAGER.createDAG(id, 0, formal_size, del), irn->second);
						//relation_shifted = MANAGER.createDAG("and", dag1, relation_shifted);
						
					}

					    

                                        //cout<<"\n\nVars to be ex.quantified are \n";
                                        ExQuantifyList="";

                                        ExQuantifyCount=0;

					ExQuantifySet.clear();

                                        for(irn = replaceNodes.begin();
                                                irn != replaceNodes.end();
                                                irn++) {ExQuantifyCount++;}

                                        //cout<<"\n\nNo: of Vars to be ex.quantified is"<<ExQuantifyCount<<endl;

                                        ExQuantifyIndex=0;

                                        for(irn = replaceNodes.begin();
                                                irn != replaceNodes.end();
                                                irn++)
                                        {

                                                string id_delta1 = irn->first;
                                                int index = id_delta1.find_last_of("_");
                                                string id = id_delta1.substr(0, index);
                                                string del = id_delta1.substr(index+1).c_str();
						string element = id+"_"+del;

						ExQuantifySet.insert(element);

                                                //ExQuantifyList = ExQuantifyList+id+"_"+del;
						
                                                //if(ExQuantifyIndex < ExQuantifyCount-1)
                                                //  ExQuantifyList = ExQuantifyList+",";

                                                //ExQuantifyIndex++;
                                                //cout<<"\n\nExQuantifyList ="<<ExQuantifyList;
                                        }

					ExQuantifyList = getListString(ExQuantifySet);

                                        
					//cout<<"\n\nVariables To be Eliminated are \t"<<ExQuantifyList<<endl;

					//cout<<"\nMap is \n";

					for(map<CDAGNode*, CDAGNode*>::iterator replaceDAGNodes_it = replaceDAGNodes.begin(); replaceDAGNodes_it != replaceDAGNodes.end(); replaceDAGNodes_it++)
					  {
					    //cout<<"\n"<<(replaceDAGNodes_it->first)->getDAGStr()<<"\t"<<(replaceDAGNodes_it->second)->getDAGStr()<<endl;
					  }
					
					// TEMPORARY CODE ADDED ON 26th Dec 08, ENABLED AGAIN ON 3rd June 09

					// THIS CODE IS TEMPORARY UNTIL INLINING IS CORRECTED. COMMENT THIS WHEN INLINING IS CORRECTED

					//if(ExQuantifyList != "")
					//{

					//  CDAGNode* exist_quant_dag = getDAGFromStringSet(ExQuantifySet);
				
					//  relation_shifted = MANAGER.createDAG("exists", exist_quant_dag, relation_shifted);
					//}

					// TEMPORARY CODE ENDS

					// INLINING CODE : THIS IS COMMENTED AS WE ARE MODIFYING INLINING
				
					//map<CDAGNode*,CDAGNode*> temp1;
						
					//set<string> temp2;

					//CDAGNode *temp3 = MANAGER.cloneAndShift(relation_shifted, 0);

					//start_time = clock();

					//temp3 = Inlining(temp3, ExQuantifySet);

					//cout<<"\n\nTime needed for inlining = "<< ((double)clock()-start_time)/CLOCKS_PER_SEC <<endl<<endl;

					//cout<<"\n\nDAG after inling is :"<<temp3->getDAGStr();
					//cout << "\n\n INLINING OVER	" << endl;

					//relation_shifted = temp3; 

					

					// INLINING CODE ENDS


					// CODE ADDED ON 30TH JUNE 2009 AFTER DEVELOPING EXPRESSION MANAGER

					//if(relation_shifted == NULL)  cout<<"\n\nOriginal dag before replacement is NULL\n";
					//else cout<<"\n\nOriginal dag before replacement "<<relation_shifted->getDAGStr();
					
					relation_shifted = MANAGER.cloneAndReplace(relation_shifted, replaceDAGNodes);
					
					//if(relation_shifted == NULL)  cout<<"\n\nDag after replacement is NULL\n";
					//else cout<<"\n\nDag after replacement "<<relation_shifted->getDAGStr();					

					// CODE ADDED ON 30TH JUNE 2009 AFTER DEVELOPING EXPRESSION MANAGER
				}
				else
				{
					map<CDAGNode*,CDAGNode*> nodeShiftednode;

					relation_shifted = MANAGER.cloneAndShift(relation, shift);

					//cout<<"\n\nRelation Shifted\n"<<relation_shifted->getDAGStr()<<endl<<endl;

					replaceNodes.clear();
					
					replaceDAGNodes.clear();// Newly added on 30th June 2009 for expression manager

					replace(relation_shifted, actualFormal, setRelation, ent, replaceNodes);
					
					
					for(irn = replaceNodes.begin();
						irn != replaceNodes.end();
						irn++)
					{
					        //string id_delta1 = irn->first;
						//int index = id_delta1.find_last_of("_");
						//string id = id_delta1.substr(0, index);
						//int del = atoi(id_delta1.substr(index+1).c_str());
						
						//CDAGNode* dag1 = MANAGER.createDAG("equivalent", irn->second, MANAGER.createDAG(id, del));
						//relation_shifted = MANAGER.createDAG("and", dag1, relation_shifted);
						//delete_and_add(*irn);

					        string id_delta1 = irn->first;
						int index = id_delta1.find_last_of("_");
						string id = id_delta1.substr(0, index);
						int del = atoi(id_delta1.substr(index+1).c_str());

						// Finding the indices of formal arguments

						// COMMENTED ON 17th June 09 FOR FINAL INTEGRATION WITH DAG MANAGER

						//list<CDAGNode*> actual_out_list = (irn->second)->out_list;
						//list<CDAGNode*>::iterator actual_out_list_it = actual_out_list.begin();
						
						//actual_out_list_it++;
						//int formal_lower = atoi(((*actual_out_list_it)->name).c_str());
						//actual_out_list_it++;
						//int formal_upper = atoi(((*actual_out_list_it)->name).c_str());

						// COMMENTED ON 17th June 09 FOR FINAL INTEGRATION WITH DAG MANAGER

						
						list<CDAGNode*>::iterator actual_out_list_it = (irn->second)->Get_DAGnode_outlist();
						
						actual_out_list_it++;
						int formal_lower = atoi(((*actual_out_list_it)->Get_DAGnode_value()).c_str());
						actual_out_list_it++;
						int formal_upper = atoi(((*actual_out_list_it)->Get_DAGnode_value()).c_str());
						
						int formal_size = formal_upper - formal_lower;

						replaceDAGNodes.insert(pair<CDAGNode*,CDAGNode*>(MANAGER.createDAG(id, 0, formal_size, del), irn->second));
						
						// The following lines are commented on 30th June 2009 for expression manager

						
						//CDAGNode* dag1 = MANAGER.createDAG("equivalent", MANAGER.createDAG(id, 0, formal_size, del), irn->second);
						//relation_shifted = MANAGER.createDAG("and", dag1, relation_shifted);
						
					}

					

                                        //cout<<"\n\nVars to be ex.quantified are \n";
                                        ExQuantifyList="";

                                        ExQuantifyCount=0;

                                        for(irn = replaceNodes.begin();
                                                irn != replaceNodes.end();
                                                irn++) {ExQuantifyCount++;}

                                        //cout<<"\n\nNo: of Vars to be ex.quantified is"<<ExQuantifyCount<<endl;

                                        ExQuantifyIndex=0;

                                        for( irn = replaceNodes.begin();
                                                irn != replaceNodes.end();
                                                irn++)
                                        {
                                                string id_delta1 = irn->first;
                                                int index = id_delta1.find_last_of("_");
                                                string id = id_delta1.substr(0, index);
                                                string del = id_delta1.substr(index+1).c_str();
                                                string element = id+"_"+del;

						ExQuantifySet.insert(element);


						//ExQuantifyList = ExQuantifyList+id+"_"+del;

                                                //if(ExQuantifyIndex < ExQuantifyCount-1)
                                                //  ExQuantifyList = ExQuantifyList+",";

                                                //ExQuantifyIndex++;

                                                //cout<<"\n\nExQuantifyList ="<<ExQuantifyList;
					 }

					

					ExQuantifyList = getListString(ExQuantifySet);

                                        
					//cout<<"\n\nVariables To be Eliminated are \t"<<ExQuantifyList<<endl;

					//cout<<"\nMap is \n";

					for(map<CDAGNode*, CDAGNode*>::iterator replaceDAGNodes_it = replaceDAGNodes.begin(); replaceDAGNodes_it != replaceDAGNodes.end(); replaceDAGNodes_it++)
					  {
					    //cout<<"\n"<<(replaceDAGNodes_it->first)->getDAGStr()<<"\t"<<(replaceDAGNodes_it->second)->getDAGStr()<<endl;
					  }

					// TEMPORARY CODE ADDED ON 26th Dec 08, ENABLED AGAIN ON 3rd June 09

					// THIS CODE IS TEMPORARY UNTIL INLINING IS CORRECTED. COMMENT THIS WHEN INLINING IS CORRECTED


					//if(ExQuantifyList != "")
					//{

					//CDAGNode* exist_quant_dag = getDAGFromStringSet(ExQuantifySet);  
										
					//relation_shifted = MANAGER.createDAG("exists", exist_quant_dag, relation_shifted);
					//}

					// TEMPORARY CODE ENDS
				
					// INLINING CODE : THIS IS COMMENTED AS WE ARE MODIFYING INLINING
						
					//cout<<"\n\nDAG for inling is : "<<relation_shifted->getDAGStr();

					//map<CDAGNode*,CDAGNode*> temp1;
						
					//set<string> temp2;

					//CDAGNode *temp3 = MANAGER.cloneAndShift(relation_shifted, 0);

					//start_time = clock();

					//temp3 = Inlining(temp3, ExQuantifySet);

					//cout<<"\n\nTime needed for inlining = "<< ((double)clock()-start_time)/CLOCKS_PER_SEC <<endl<<endl;


					//cout<<"\n\nDAG after inling is :"<<temp3->getDAGStr();
					//cout << "\n\n INLINING OVER	" << endl;

					//relation_shifted = temp3;

					
                      
					// INLINING CODE ENDS

					
					// CODE ADDED ON 30TH JUNE 2009 AFTER DEVELOPING EXPRESSION MANAGER
					
					//if(relation_shifted == NULL)  cout<<"\n\nOriginal dag before replacement is NULL\n";
					//else cout<<"\n\nOriginal dag before replacement "<<relation_shifted->getDAGStr();
					
					relation_shifted = MANAGER.cloneAndReplace(relation_shifted, replaceDAGNodes);

					//if(relation_shifted == NULL)  cout<<"\n\nDag after replacement is NULL\n";
					//else cout<<"\n\nDag after replacement "<<relation_shifted->getDAGStr();
					
					// CODE ADDED ON 30TH JUNE 2009 AFTER DEVELOPING EXPRESSION MANAGER

					
                                       			
					nodeShiftednode.clear();
				}


				//cout<<"\n\nRelation Shifted After Replacing Formals by Actuals\n\n";
				
				//cout<<relation_shifted->getDAGStr()<<endl;
				
				if(debug_mode==1)
				
					{
					  //cout<<"\n\nInstance name\n";
				
					  //cout<<instance_stmt->getInstName()<<endl;
				
					//relation_shifted = MANAGER.createDAG(instance_stmt->getInstName()+"_"+relation_shifted->getDAGStr());
					
					relation_shifted = MANAGER.createDAG(instance_stmt->getInstName() + "_" + relation_shifted->Get_DAGnode_value(), relation_shifted->Get_DAGnode_delta_time_as_int());

					//cout<<"\n\nModified Constraints\n\n";
				
					//cout<<relation_shifted->getDAGStr()<<endl;
					}
				

				ent.constraints.push_back(relation_shifted);
				//CDAGNode* condition_shifted = condition->cloneAndShift(NULL, nodeShiftednode);


				char str_current[20];

				//sprintf(str_current, "%d", current + delta_time - 2);

				sprintf(str_current, "%d", present_time);
				
				//string id_delta = replaceNodes[output_identifier + "_" + str_current]->getDAGStr();

				//cout<<"\nOutput_identifier = \t"<<output_identifier;

				CExpression* exp = actualFormal[output_identifier];

				//if(exp == NULL) cout<<"\t Exp. For it is NULL";
				//else cout<<"\t Exp. For it is NON NULL";
				
				string id_delta= exp->getLhsOperand()->getName() + "_" + str_current;

				//cout<<id_delta<<endl;
				
				//CDAGNode* out_dag = MANAGER.createDAG(exp->getLhsOperand()->getName(), current + delta_time - 2);

				CDAGNode* out_dag = MANAGER.createDAG(exp->getLhsOperand()->getName(), present_time);
				
				int lower = atoi(((CAtom*)(exp->getLhsOperand()))->getLower().c_str());
				int upper = atoi(((CAtom*)(exp->getLhsOperand()))->getUpper().c_str());
				
				pair<int, int> range(lower, upper);
				vector<pair<int, int> > ranges_covered;
				
				ranges_covered.push_back(range);
				
				CDAGNode* true_cond = MANAGER.createDAG("true");

				ConditionRelation *cr = new ConditionRelation(true_cond, relation_shifted);

				setRelation.erase(id_delta);

				cr->setRelation.insert(setRelation.begin(), setRelation.end());

				//cr->ranges_covered = (*irio)->ranges_covered;

				//cr->ranges_covered = entry->ranges_covered;

				cr->ranges_covered = ranges_covered;

				updateMapSignalDelta(bb->mapSignalDelta, cr, exp->getLhsOperand()->getName(), present_time, id_delta);

				//bb->mapSignalDelta[id_delta].push_back(cr);

				set<string> empty_set;

				//insertToSymbolicQueue(current + delta_time - 2, out_dag, true_cond, empty_set, SymbolicQueues[exp->getLhsOperand()->getName()], ranges_covered);
				
				insertToSymbolicQueue(present_time, out_dag, true_cond, empty_set, SymbolicQueues[exp->getLhsOperand()->getName()], ranges_covered);
				
				deltas_new[present_time].push_back(exp->getLhsOperand()->getName());

				//cout<<"\n\nDeltas for BBSimulation after inserting "<<exp->getLhsOperand()->getName()<<" at "<<present_time<<endl;

				//showDeltasForBBSimulation(deltas_new);

				//deltas_new.push_back(current + delta_time - 2);
				
				//deltas_new.sort();
				//deltas_new.unique();
		}
		
		
	    }

	}

	return deltas_new;
}

void CSymbSimulator::InitializeRV(list<ProcessSExpressions*> &SymbolicExpressions, map<string, list<RangeConstraint*> > &mapRV, string entName)
{
   // Newly added code to obtain SEs of bv's inside process properly

  string VarName;

  list<ProcessSExpressions*>::iterator SymbolicExpressions_it =  SymbolicExpressions.begin();

  for(;SymbolicExpressions_it != SymbolicExpressions.end(); SymbolicExpressions_it++)

    {

      VarName = (*SymbolicExpressions_it)->getLhs()->getName();

      if(mapRV[VarName].begin() == mapRV[VarName].end())

	{
	  set<string> setConstraint;

	  map<string, SymTabEntry> &ids = SymbolTable[entName];

	  SymTabEntry ste = ids[VarName];
		
	  list<string>::iterator ilist = ste.rangeindex.begin();

	  string lower = (*ilist);
	  ilist++;
	  string upper = (*ilist);		

	  int u = atoi(upper.c_str());
	  int l = atoi(lower.c_str());

	  RangeConstraint *rc = new RangeConstraint(l, u, MANAGER.createDAG(VarName,1),setConstraint);

	  mapRV[VarName].push_back(rc); // Initializing the map 
	}
    }

}
  

void CSymbSimulator::createConstraint(ProcessSExpressions* os, map<string, list<RangeConstraint*> > &mapRV, CDAGNode *dag, set<string>& setConstraint, string entName)
{       

        int low_new = atoi(((CAtom*)(os->getLhs()))->getLower().c_str());
	int upp_new = atoi(((CAtom*)(os->getLhs()))->getUpper().c_str());
	
	RangeConstraint *rc = new RangeConstraint(low_new, upp_new, dag, setConstraint);
	
	if(mapRV.find(os->getLhs()->getName()) != mapRV.end())
	{
		list<RangeConstraint*>& listRV = mapRV[os->getLhs()->getName()];
		list<RangeConstraint*>::iterator irv;
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

				RangeConstraint *rc_upp = new RangeConstraint(upp_new+1, upp_old, dag_upp, (*irv)->setConstraint);
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

				RangeConstraint *rc_low = new RangeConstraint(low_old, low_new-1, dag_low, (*irv)->setConstraint);

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

				RangeConstraint *rc_low = new RangeConstraint(low_old, low_new-1, dag_low, (*irv)->setConstraint);


				CDAGNode *dag_upp =	MANAGER.createDAG("select",
													(*irv)->constraint,
													upp_new+1,
													upp_old);

				RangeConstraint *rc_upp = new RangeConstraint(upp_new+1, upp_old, dag_upp, (*irv)->setConstraint);

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

				RangeConstraint *rc_low = new RangeConstraint(low_old, low_new-1, dag_low, (*irv)->setConstraint);

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

				RangeConstraint *rc_upp = new RangeConstraint(upp_new+1, upp_old, dag_upp, (*irv)->setConstraint);

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
		mapRV[os->getLhs()->getName()].push_back(rc);
	}
}

/*CDAGNode *CSymbSimulator::createDAGforOS(ProcessSExpressions* os, int current, CEntity &ent, 
										set<string>& setConstraint)
{
	list<CExpression*> ::iterator ichoice_other = os->getConditions().begin();
	list<CExpression*> ::iterator ivalue_other = os->getValues().begin();

	CDAGNode * dag1 = NULL;

	set<string> setRelation;
	set<string> setRelationlhs;
	
	
	
	// all others conditions are ORed together and erased after creating DAG 
	for(;ichoice_other != os->getConditions().end(); ichoice_other++,ivalue_other++)
	{
		cout<<(*ichoice_other)->getOperand()<<endl;
		if((*ichoice_other)->getOperand() == "else" || (*ichoice_other)->getOperand() == "others" )
		{
			dag1 = (*ivalue_other)->getDAG(ent.getName(), current, setRelation);
			ichoice_other = os->getConditions().erase(ichoice_other);
			ivalue_other = os->getValues().erase(ivalue_other);
			break;
		}
	}

	for(;ichoice_other != os->getConditions().end(); ichoice_other++,ivalue_other++)
	{
		cout<<(*ichoice_other)->getOperand()<<endl;
		if((*ichoice_other)->getOperand() == "else" || (*ichoice_other)->getOperand() == "others" )
		{
			CDAGNode * dag3 = MANAGER.createDAG("OR", (*ivalue_other)->getDAG(ent.getName(), current, setRelation), dag1);
			dag1 = dag3;
			ichoice_other = os->getConditions().erase(ichoice_other);
			ivalue_other = os->getValues().erase(ivalue_other);
		}
	}


	
	list<CExpression*> ::reverse_iterator ichoice = os->getConditions().rbegin();
	list<CExpression*> ::reverse_iterator ivalue = os->getValues().rbegin();

	//if no other condition
	if(dag1 == NULL)
	{
		CDAGNode * dag2 = NULL;
		cout<<(*ichoice)->getOrgExpr()<<endl;
		dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
		
		dag1 = MANAGER.createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), os->getLhs()->getDAG(ent.getName(), current, setRelation));
		
		// increment pointers
		ichoice++;
		ivalue++;
		
	}

	for(;ichoice != os->getConditions().rend(); ichoice++,ivalue++)
	{
		CDAGNode * dag2 = NULL;
		dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);

		CDAGNode * dag3 = MANAGER.createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), dag1);
		dag1 = dag3;
	}

	CDAGNode* lhs_dag = os->getLhs()->getDAG(ent.getName(), current+1, setRelationlhs);

	CDAGNode * final_expression = MANAGER.createDAG("equivalent", lhs_dag, dag1);
	cout<<final_expression->getDAGStr()<<endl;
	
	setConstraint.insert(setRelation.begin(), setRelation.end());
	
	cout<< "createDAGforOS" <<endl;
	cout<<final_expression->getDAGStr()<<endl;
	cout<<setConstraint.size()<<endl;
	

	return final_expression;
}*/


void CSymbSimulator::CreateTableForProcesses(CEntity &ent)
{
	list<CProcess *> &processNodes = ent.getProcesses();
	string entName = ent.getName();
	list<CProcess *>::iterator iNodes;

	// Added on 9/9/08
	
	//list<COperand*,CDAGNode*> SymbolicExpressions;
	//list<COperand*,CDAGNode*>::iterator symb_exp_it;

	// End of additions on 9/9/08


//	cout<<"\n\n***********************************\n\nCreating Tables for Processes\n\n**************************\n\n";

	for(iNodes = processNodes.begin();
		iNodes != processNodes.end();
		iNodes++
		)
	{
		list<string> ::iterator isen;

		CSeqStmt *stmt_ptr = (*iNodes)->getFirst();

		list<ProcessSExpressions*> SymbolicExpressions;
		list<ProcessSExpressions*>::iterator symb_exp_it;

		list<CSensitiveElement*> sensitivityList;

		// Added on 5/9/08

		// list<COperand*>;

		// list<COperand*>::iterator outputs_it;

                if(stmt_ptr != NULL)
                {
                        stmt_ptr->getOutputs(SymbolicExpressions);
                }

		// cout<<"\n\nOutputs Of the Process are....\n\n";

		//outputs.sort();

		//outputs.unique();

                //for(outputs_it = outputs.begin();
                //outputs_it != outputs.end();
                //outputs_it++) {cout<<(*outputs_it)<<endl;}

                //cout<<"\n\nOutputs Of the Process are finished....\n\n";

		// Additions on 9/9/08

		//cout<<"\n\nInitializing the map SymbolicExpressios with output=output\n\n";
		
		for(symb_exp_it = SymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end(); symb_exp_it++) 

		  {
		    set<string> setRelation; 
		    (*symb_exp_it)->addValue((*symb_exp_it)->getLhs()->getDAG(entName, 1, setRelation));
		  }

		//for(outputs_it = outputs.begin();
                //outputs_it != outputs.end();
                //outputs_it++)

		//  {
		//    SymbolicExpressions[(*outputs_it)] = MANAGER.createDAG((*outputs_it),1);
		//  }

		//cout<<"\n\nObtaining Symbolic Expressions for signals and variables inside process\n\n";

		if(stmt_ptr != NULL)
                {
                        stmt_ptr->getSymbolicExpressions(SymbolicExpressions,entName,1);
                }

		//cout<<"\n\nPrinting Symbolic Expressions for signals and variables inside process\n";

		for(symb_exp_it = SymbolicExpressions.begin(); symb_exp_it != SymbolicExpressions.end(); symb_exp_it++) 

		  {
		    //cout<<endl<<"\t"<<(*symb_exp_it)->getValue()->getDAGStr()<<endl;
		  }


                // End of Additions


		//if(stmt_ptr != NULL)
		//	{
		//		stmt_ptr->getOrientedStmts(NULL, ostmts);
		//	}

		map<string, list<RangeConstraint*> > mapRV;
		list<ProcessSExpressions*>::iterator  ios;

		set<string> setConstraint;

		InitializeRV(SymbolicExpressions,mapRV,entName);
		
		//cout<<"\n\nPrinting Final Symbolic Expressions for signals and variables inside process\n";

		for(ios = SymbolicExpressions.begin(); ios != SymbolicExpressions.end(); ++ios)
		{
			set<string> setConstraint;
			set<string> temp;

			//CDAGNode* lhs_dag = (*ios)->getLhs()->getDAG(ent.getName(), 2, temp);

			(*ios)->getValue()->getDAGNames(setConstraint);

			//CDAGNode* dag = MANAGER.createDAG("equivalent", lhs_dag, (*ios)->getValue());

			CDAGNode* dag = (*ios)->getValue();
			//cout<<"\n"<<dag->getDAGStr()<<endl;
			createConstraint(*ios, mapRV, dag, setConstraint, ent.getName());
		}
		createTableForProcesses(mapRV, *iNodes);
	}
}


void CSymbSimulator::createTableForProcesses(map<string, list<RangeConstraint*> > &mapRV, CProcess *proc)
{
	map<string, list<RangeConstraint*> >::iterator itab = mapRV.begin();
	
	//cout<<"\n\n#########################################################################################################\n\n";
	
	//cout<<"Table for Process\n\n";

	for(;itab != mapRV.end(); itab++)
	{
		set<string> setConstraint;
		CDAGNode *lhs_dag;
		set<string>::iterator setConstraint_it; // newly added
		string output_identifier = itab->first;
		list<RangeConstraint*> & entries = itab->second;
		vector<pair<int, int> > ranges_covered;

		// Temp code

		//cout<<"\nRanges of "<<output_identifier<<endl;

		list<RangeConstraint*>::iterator temp = entries.begin();
		for(;temp!=entries.end();temp++)
		  {
		    //cout<<(*temp)->range.first<<","<<(*temp)->range.second<<endl;
		  }

		// Temp code ends

		
		list<RangeConstraint*>::iterator irio = entries.begin();
		CDAGNode *dag = (*irio)->constraint ;
		setConstraint.insert((*irio)->setConstraint.begin(), (*irio)->setConstraint.end());
		
		ranges_covered.push_back((*irio)->range);

		// create lhs_dag here.......

		lhs_dag = MANAGER.createDAG(output_identifier, (*irio)->range.first, (*irio)->range.second, 2);
		dag = MANAGER.createDAG("equivalent", lhs_dag, (*irio)->constraint);

		/* ADDED ON 29/12/2009 FOR INTEGRATION WITH QELIM. BUT THIS WILL REDUCE INLININING CHANCES */

		//cout<<"\ndag = "<<dag->getDAGStr()<<endl;
		//dag = MANAGER.convertEquationToProperForm(dag);
		//cout<<"\ndag after converting to proper form = "<<dag->getDAGStr()<<endl;

                /* ADDED ON 29/12/2009 FOR INTEGRATION WITH QELIM ENDS */

		irio++;
		
		for(; irio != entries.end(); irio++)
		{
		       // create lhs_dag here also.....
		        lhs_dag = MANAGER.createDAG(output_identifier, (*irio)->range.first, (*irio)->range.second, 2);

			CDAGNode *new_dag = MANAGER.createDAG("equivalent", lhs_dag, (*irio)->constraint);

			/* ADDED ON 29/12/2009 FOR INTEGRATION WITH QELIM. BUT THIS WILL REDUCE INLININING CHANCES */

			//cout<<"\nnew_dag = "<<new_dag->getDAGStr()<<endl;
			//new_dag = MANAGER.convertEquationToProperForm(new_dag);
			//cout<<"\nnew_dag after converting to proper form = "<<new_dag->getDAGStr()<<endl;

			/* ADDED ON 29/12/2009 FOR INTEGRATION WITH QELIM ENDS */
			
		       
			dag = MANAGER.createDAG("and",new_dag, dag);
			setConstraint.insert((*irio)->setConstraint.begin(), (*irio)->setConstraint.end());
			ranges_covered.push_back((*irio)->range);
		}
		
		//cout<<"\n***********************************************************\n";
		//cout<< "createTableForProcesses" <<endl;
		//cout<<"\n\nTable for output "<<output_identifier<<"\n\n"; 
		//cout<<"Original Symbolic Expression at delta time 1 :\n";
		//cout<<dag->getDAGStr()<<endl;
		//cout<<setConstraint.size()<<endl;
		
		for(setConstraint_it = setConstraint.begin(); setConstraint_it != setConstraint.end(); setConstraint_it++)
		{
		  //cout<<(*setConstraint_it)<<endl;
		}

		//cout<<"\nSymbolic Expression at delta time 0 :\n";

		map<CDAGNode*,CDAGNode*> temp1;
		set<string> temp2;
		CDAGNode *shifted_dag = MANAGER.cloneAndShift(dag, -1);
		temp1.clear();

		setConstraint.clear();
		shifted_dag->getDAGNames(setConstraint);

		//cout<<shifted_dag->getDAGStr()<<endl;
		//cout<<setConstraint.size()<<endl;
		
		for(setConstraint_it = setConstraint.begin(); setConstraint_it != setConstraint.end(); setConstraint_it++)
		{
		  //cout<<(*setConstraint_it)<<endl;
		}


		//cout<<"\n***********************************************************\n";

		SymbolicRelation* r = new SymbolicRelation(shifted_dag, ranges_covered, setConstraint); 
		
		//proc->table[output_identifier].push_back(r);
		
		proc->table[output_identifier] = r;
	}
	
	//cout<<"\n\n#########################################################################################################\n\n";
}

/*
void CSymbSimulator::replace(CDAGNode* dag_node, map<string, CExpression*> &actualFormal, set<string> &setRelation, CEntity &ent, list<ReplaceNode*> &replaceNodes)
{
	if(actualFormal.find(dag_node->name) != actualFormal.end())
	{
		CDAGNode* tobereplaced = actualFormal[dag_node->name]->getDAG(ent.getName(), dag_node->delta_time, setRelation);
		
		char temp[5];
		sprintf(temp, "%d", dag_node->delta_time);
		string id_delta = dag_node->name + "_" + temp;
		setRelation.erase(id_delta);

		ReplaceNode *rn = new ReplaceNode(dag_node, tobereplaced);
		replaceNodes.push_back(rn);
	}
	
	list<CDAGNode*>::iterator iout = dag_node->out_list.begin();
	
	for(; iout != dag_node->out_list.end(); iout++)
	{
		replace(*iout, actualFormal, setRelation, ent, replaceNodes);
	}
}*/

void CSymbSimulator::replace(CDAGNode* dag_node, map<string, CExpression*> &actualFormal, set<string> &setRelation, CEntity &ent, map<string, CDAGNode*> &replaceNodes)
{
	if(actualFormal.find(dag_node->Get_DAGnode_value()) != actualFormal.end())
	{
		char temp[5];
		sprintf(temp, "%d", dag_node->Get_DAGnode_delta_time_as_int());
		string id_delta = dag_node->Get_DAGnode_value() + "_" + temp;
				
		if(replaceNodes.find(id_delta) == replaceNodes.end())
		{
			CDAGNode* tobereplaced = actualFormal[dag_node->Get_DAGnode_value()]->getDAG(ent.getName(), dag_node->Get_DAGnode_delta_time_as_int(), setRelation);
			replaceNodes[id_delta]= tobereplaced;
		}
	}
	
	list<CDAGNode*>::iterator iout = dag_node->Get_DAGnode_outlist();
	
	for(unsigned int i=0; i < dag_node->Get_DAGnode_outlist_size(); i++, iout++)
	{
		replace(*iout, actualFormal, setRelation, ent, replaceNodes);
	}
}


string CSymbSimulator::findFormalTriggeringInput(map<string, CExpression*> actualFormal, string Actual_TriggeringInput, CEntity &ent)
{
  bool found = false;
  string formal;
  for(map<string, CExpression*>::iterator actualFormal_it = actualFormal.begin(); actualFormal_it != actualFormal.end(); actualFormal_it++)
    {
      CExpression* temp = (*actualFormal_it).second;
      
      /*set<string> setRelation;
      int delta = 10; // Some arbitrary delta
      CDAGNode* tempDAG = temp->getDAG(ent.getName(), delta, setRelation);
      cout<<"\n\nACTUAL ARGUMENT IS "<<tempDAG->getDAGStr()<<endl<<endl;
      set<string> names;
      tempDAG->getDAGNames(names);
      if(names.size() > 1) 
	{
	  cout<<"\n\nERROR! ACTUAL ARGUMENT "<<tempDAG->getDAGStr()<<" CONTAINS MORE THAN ONE SIGNAL\n\n";
	  exit(1);
	}
      set<string>::iterator names_it = names.begin();
      string id_delta = (*names_it);
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      cout<<"\n\nNAME = "<<id<<endl<<endl;*/

      string id = temp->getLhsOperand()->getName();

      if(id == Actual_TriggeringInput)
	{
	  formal = (*actualFormal_it).first;
	  found = true;
	  break;
	}
    }  
  if (found == true)
    return formal;
  else
    {
     //cout<<"\n\nERROR! ACTUAL ARGUMENT "<<Actual_TriggeringInput<<" IS NOT MAPPING WITH ANY FORMAL ARGUMENT\n\n"; 
     exit(1);
     return "";
     }
}
      


/* TRASIENT REMOVAL IS NO LONGER REQUIRED IN THE PRESENT SCHEME */


/*
void CSymbSimulator::removeTransientsFromTable(CEntity& ent)
{

  cout<<"\n\nInside removeTransientsFromTable\n\n";

        map< string, list<RelationInputOutput*> >& table = ent.getTable();
                                                                                                                             
        map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
                                                                                                                             
	string entityname = ent.getName();  // Entity name obtained                                
                                                                                           
       for(;itab != table.end(); itab++)
      {
            string output_identifier = itab->first; // Output name obtained
            list<RelationInputOutput*> & entries = itab->second; // List for the output obtained
                                                                                                                            
            for(list<RelationInputOutput*>::iterator irio = entries.begin();
                       irio != entries.end();
                       irio++)
               {
                       RelationInputOutput* rio = *irio;
                       int delta_time = rio->delta_time;
		       CDAGNode *spec = rio->condition;
		      
		       

		       // This part is temporary
 
		       if(isUnsat(spec, entityname))   
		          cout<<"\n\nspec->getDAGStr() IS UNSAT\n\n";
		       else
		         cout<<"\n\nspec->getDAGStr() IS SAT\n\n";

                }
       }
}

*/


/* This function should return true if the argument is unsat and false if the argument is sat */

// bool CSymbSimulator::isUnsat(CDAGNode* spec, string EntName)
// {

//        cout<<"\n\nInside isUnsat\n\n";


// 	map<CDAGNode*,CDAGNode*> temp_map;

// 	set<string> temp_string;

// 	CDAGNode *copy = MANAGER.cloneAndShift(spec, 0);

// 	FILE *smt_file;

// 	if((smt_file=fopen("smt_file.smt","w"))==NULL)
// 	  {
// 	    cout<<"\nError while trying to open file smt_file.smt for writing. Exiting...";
// 	    exit(1);
// 	  }
	
// 	fprintf(smt_file,"(benchmark smt_file.smt\n");
	        
// 	writeBVDeclarationsIntoSMTLib(copy,smt_file,EntName);

// 	//string finalString = removeParentheses(copy->getDAGStrInSMTLibFormat());

// 	string finalString = copy->getDAGStrInSMTLibFormat();

// 	fprintf(smt_file,":formula %s\n)", finalString.c_str());

// 	fclose(smt_file);

// 	return true;
// }

/* Function to remove parantheses like () */

string CSymbSimulator::removeParentheses(string target)

{

  int index = 0;
  int loc = 0;

  while(target.find("()",index) != string::npos)
    {
      loc = target.find("()",index);
      //cout<<"\nloc = "<<loc;
      target = target.erase(loc,2);
      index = loc + 2;
    }

  return target;
}




void CSymbSimulator::check_for_cycles(CEntity &ent)
{
  Graph ArgDepGraph;
  Graph BlockDepGraph;
  
  createArgDepGraph(&ArgDepGraph, ent);
  createBlockDepGraph(&BlockDepGraph, ent);
  //cout<<"\nArg Dep Graph\n";
  //ArgDepGraph.print();
  //cout<<"\nBlock Dep Graph\n";
  //BlockDepGraph.print();
  //cout<<"\nPrinting Over\n";

  set<string> ArgSCC, BlockSCC;

  //cout<<"\n\nComputing BlockSCC to check for structural cycles";
  BlockDepGraph.Tarjan(BlockSCC);
  //cout<<"\n\nBlockSCC computed";
  
  if(BlockSCC.size() != 0) // Check if there is a structural cycle (i.e. cycle in BlockSCC)

    structural_cycle_present = true; 

  else
  
    structural_cycle_present = false; 

 
  // Initialize ArgSCC and BlockSCC for real cycle detection
  // Setting ArgSCC = nil
  // BlockSCC = All vertices in BlockDepGraph

  InitializeBlockSCC(BlockSCC, ent);

  //cout<<"\n\nInitialization of Block SCC over";

  //cout<<"\n\nPrinting BlockSCC";

  //printSCCs(BlockSCC);

  int count = 0;
  
  while(true)
    {
      //cout<<"\n\nWhile loop started";


      bool flag = false;


      //cout<<"\n\nDeleting vertices from ArgDepGraph not present in BlockSCC";
      
      flag = ArgDepGraph.deleteVerticesNotPresent(BlockSCC);

      //cout<<"\n\nVertices not in BlockSCC deleted from ArgDepGraph";


      if(flag==false && count != 0)
	break;

      
      //cout<<"\n\nRecomputing ArgSCC";

      ArgDepGraph.Tarjan(ArgSCC);

      //cout<<"\n\nArgSCC recomputed";

      //cout<<"\n\nPrinting ArgSCC";

      //printSCCs(ArgSCC);



      //cout<<"\n\nDeleting vertices from BlockDepGraph not present in ArgSCC";
       
      BlockDepGraph.deleteVerticesNotPresent(ArgSCC);

      //cout<<"\n\nVertices not in ArgSCC deleted from ArgDepGraph";



      //cout<<"\n\nRecomputing BlockSCC";

      BlockDepGraph.Tarjan(BlockSCC);

      //cout<<"\n\nBlockSCC recomputed";

      //cout<<"\n\nPrinting BlockSCC";

      //printSCCs(BlockSCC);


      
      count++;
    }

  

  //cout<<"\n\nPRINTING Arg Dep Graph and Block Dep Graph after removing vertices\n\n";

  //cout<<"\nArg Dep Graph\n";
  //ArgDepGraph.print();
  //cout<<"\nBlock Dep Graph\n";
  //BlockDepGraph.print();
  //cout<<"\nPrinting Over\n";

  
  //cout<<"\n\nRecomputing ArgSCC and BlockSCC";
  ArgDepGraph.Tarjan(ArgSCC);
  BlockDepGraph.Tarjan(BlockSCC);
  //cout<<"\n\nArgSCC and BlockSCC recomputed";
  
  if(ArgSCC.size() != 0 && BlockSCC.size() != 0) // Check if there is cycle in both ArgSCC and BlockSCC

    real_cycle_present = true; 

  else
  
    real_cycle_present = false; 

}

CPort& CSymbSimulator::getPort(list<CPort> & ports, string name)
{
	list<CPort>::iterator iport;

	for(iport = ports.begin(); iport != ports.end(); iport++)
	{
		if((*iport).getName() == name)
		{
			return *iport;
		}
	}	
}


void CSymbSimulator::createEdgeInDepGraphForInstances(list<CInstance *> &instanceNodes, string EntName, Graph *DepGraph)
{
	int p = 0;
	list<CInstance *>::iterator iNodes = instanceNodes.begin();

	//cout<<"--------------------------"<<endl;
	//cout<<instanceNodes.size()<<endl;
	//cout<<"--------------------------"<<endl;

	for(	iNodes = instanceNodes.begin();
		iNodes != instanceNodes.end();
		iNodes++
		)
	{
		list<CExpression*> params= (*iNodes)->getActualParam();
		list<CExpression*> ::iterator iparam= params.begin();

		list<COperand*> formals= (*iNodes)->getFormalParam();
		list<COperand*> ::iterator iformal = formals.begin();

//		fprintf(fp, "param%d [shape=record,label=\"",p);

		list<CSensitiveElement*> sensitivityList;
		list<CSensitiveElement*> lhsList;
		
		list<CPort> &ports = Entities[(*iNodes)->getIndex()].getPorts(); 

		list<CPort>::iterator iport;
	
		if(formals.size() != 0)
		{
			
			for(iparam = params.begin();
				iparam != params.end();
				iparam++,iformal++)
			{
				//cout<<(*iformal)->getName()<<endl;
				CPort& port = getPort(ports, (*iformal)->getName());
				if(port.getMode() == t_IN || port.getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(sensitivityList);
				}

				if(port.getMode() == t_OUT || port.getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(lhsList);
				}
			}
		}
		else
		{
			iport = ports.begin();
			for(iparam = params.begin();
				iparam != params.end();
				iparam++,iport++)
			{
				if((*iport).getMode() == t_IN || (*iport).getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(sensitivityList);
				}
				
				if((*iport).getMode() == t_OUT || (*iport).getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(lhsList);
				}
				
			}

		}
		
		
		list<CSensitiveElement*> ::iterator ilhs = lhsList.begin();
		list<CSensitiveElement*>::iterator sensitivityList_it;
		
		for(; ilhs != lhsList.end(); ilhs++)
		{
		for(sensitivityList_it = sensitivityList.begin();sensitivityList_it != sensitivityList.end(); sensitivityList_it++)
		  {
		    DepGraph->AddEdge((*sensitivityList_it)->name, (*ilhs)->name);
		  }
		}
		
	}	
}


void CSymbSimulator::createArgDepGraph(Graph *ArgDepGraph, CEntity &ent)
{
	string EntName = ent.getName();

	set<CSensitiveElement*> identifiers;
	CSensitiveElement* identifier;
	set<CSensitiveElement*> input_identifiers;
	set<CSensitiveElement*> output_identifiers;
	set<CSensitiveElement*> intermediate_identifiers;

	// create the vertexes
	getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers, ent);

	set<CSensitiveElement*>::iterator identifiers_it = identifiers.begin();

	for(; identifiers_it != identifiers.end();  identifiers_it++)
	  {
	    identifier = (*identifiers_it);
	    //cout<<"\nIdentifier="<<identifier->name;
	    ArgDepGraph->AddVertex(identifier->name);
	  }
	// create edgdes
	// conc. sig. assignments

	list<CConSignAssgnStmt *> &conAssignNodes = ent.getConAssgnStmt();
	list<CConSignAssgnStmt *>::iterator iNodes;

	for(iNodes = conAssignNodes.begin();iNodes != conAssignNodes.end();	iNodes++)
	{
		COperand *lhs = (*iNodes)->getLHS();
		
		list<CExpression*> values1;
		list<CExpression*> conditions1;		
		
		CExpression* SelExpression = (*iNodes)->getOriginalSelExpr();
		list<CExpression*> values = (*iNodes)->getValues();
		list<CExpression*> choices = (*iNodes)->getChoices();
		

		list<CExpression*> ::iterator ichoice = choices.begin();
		list<CExpression*> ::iterator ivalue = values.begin();
		//cout<<"you are here... sizes "<<choices.size() <<"  "<<values.size() <<endl;

		list<CSensitiveElement*> sensitivityList;

		//fflush(stdout);

		
		if(SelExpression != NULL)
		  {//cout<<"\nSelection Expression = "<<SelExpression->getOrgExpr();
		    SelExpression->getSensitivityList(sensitivityList); }
		else
		  { //cout<<"\nSelection Expression = NULL";
		   }

		for(; ichoice != choices.end(); ichoice++,ivalue++)
		{
			(*ivalue)->getSensitivityList(sensitivityList);
			(*ichoice)->getSensitivityList(sensitivityList);
		}

		list<CSensitiveElement*>::iterator sensitivityList_it;

		for(sensitivityList_it = sensitivityList.begin();sensitivityList_it != sensitivityList.end(); sensitivityList_it++)
		  {
		    ArgDepGraph->AddEdge((*sensitivityList_it)->name, ((CAtom*)lhs)->getName());
		  }
	}
	
	// process statements

	list<CProcess *> &processNodes = ent.getProcesses();
	list<CProcess *>::iterator pNodes = processNodes.begin();

	for(pNodes = processNodes.begin();
		pNodes != processNodes.end();
		pNodes++
		)
	  {
	    CSeqStmt *stmt_ptr = (*pNodes)->getFirst();
	    if(stmt_ptr != NULL)
		{
		  stmt_ptr->createEdgeInArgDepGraph(ArgDepGraph);
		}
	    
	  }

	// comp.instantiations

	list<CInstance *> &instanceNodes = ent.getInstances();
	createEdgeInDepGraphForInstances(instanceNodes, EntName, ArgDepGraph);
}

void CSymbSimulator::createBlockDepGraph(Graph *BlockDepGraph, CEntity &ent)
{
      
	string EntName = ent.getName();

	set<CSensitiveElement*> identifiers;
	CSensitiveElement* identifier;
	set<CSensitiveElement*> input_identifiers;
	set<CSensitiveElement*> output_identifiers;
	set<CSensitiveElement*> intermediate_identifiers;

	// create the vertexes
	getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers, ent);

	set<CSensitiveElement*>::iterator identifiers_it = identifiers.begin();

	for(; identifiers_it != identifiers.end();  identifiers_it++)
	  {
	    identifier = (*identifiers_it);
	    //cout<<"\nIdentifier="<<identifier->name;
	    BlockDepGraph->AddVertex(identifier->name);
	  }
	// create edgdes
	
	// conc. sig. assignments

	list<CConSignAssgnStmt *> &conAssignNodes = ent.getConAssgnStmt();
	list<CConSignAssgnStmt *>::iterator iNodes;

	for(iNodes = conAssignNodes.begin();iNodes != conAssignNodes.end();	iNodes++)
	{
		COperand *lhs = (*iNodes)->getLHS();
		
		list<CExpression*> values1;
		list<CExpression*> conditions1;		
		
		CExpression* SelExpression = (*iNodes)->getOriginalSelExpr();
		list<CExpression*> values = (*iNodes)->getValues();
		list<CExpression*> choices = (*iNodes)->getChoices();
		

		list<CExpression*> ::iterator ichoice = choices.begin();
		list<CExpression*> ::iterator ivalue = values.begin();
		//cout<<"you are here... sizes "<<choices.size() <<"  "<<values.size() <<endl;

		list<CSensitiveElement*> sensitivityList;

		//fflush(stdout);

		
		if(SelExpression != NULL)
		  {//cout<<"\nSelection Expression = "<<SelExpression->getOrgExpr();
		    SelExpression->getSensitivityList(sensitivityList); }
		else
		  {//cout<<"\nSelection Expression = NULL";
		  }

		for(; ichoice != choices.end(); ichoice++,ivalue++)
		{
			(*ivalue)->getSensitivityList(sensitivityList);
			(*ichoice)->getSensitivityList(sensitivityList);
		}

		list<CSensitiveElement*>::iterator sensitivityList_it;

		for(sensitivityList_it = sensitivityList.begin();sensitivityList_it != sensitivityList.end(); sensitivityList_it++)
		  {
		    BlockDepGraph->AddEdge((*sensitivityList_it)->name, ((CAtom*)lhs)->getName());
		  }
	}
	
	// process statements

	list<CProcess *> &processNodes = ent.getProcesses();
	list<CProcess *>::iterator pNodes = processNodes.begin();
	

	for(pNodes = processNodes.begin();
		pNodes != processNodes.end();
		pNodes++
		)
	  {
	    list<CSensitiveElement*> lhsList;
	    list<CSensitiveElement*>::iterator lhsList_it;
	    list<COperand*> sensitivityList;
	    list<COperand*>::iterator sensitivityList_it;
	    
	   
	    sensitivityList = (*pNodes)->getSenList();
	    CSeqStmt *stmt_ptr = (*pNodes)->getFirst();

	    if(stmt_ptr != NULL)
		{
		  stmt_ptr->getChangedLHS(lhsList);
		  
		 for(lhsList_it = lhsList.begin(); lhsList_it != lhsList.end(); lhsList_it++)
		   {
		     for(sensitivityList_it = sensitivityList.begin();sensitivityList_it != sensitivityList.end(); sensitivityList_it++)
		       {
			 BlockDepGraph->AddEdge(((CAtom*)(*sensitivityList_it))->getName(), (*lhsList_it)->name);
		       }
		   }
		  
		}
	    
	  }


	
	// comp.instantiations

	list<CInstance *> &instanceNodes = ent.getInstances();
	createEdgeInDepGraphForInstances(instanceNodes, EntName, BlockDepGraph);
}


void  CSymbSimulator::InitializeBlockSCC(set<string> &BlockSCC, CEntity &ent)
{
  BlockSCC.clear();

	string EntName = ent.getName();

	string element="";

	set<CSensitiveElement*> identifiers;
	CSensitiveElement* identifier;
	set<CSensitiveElement*> input_identifiers;
	set<CSensitiveElement*> output_identifiers;
	set<CSensitiveElement*> intermediate_identifiers;

	getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers, ent);

	set<CSensitiveElement*>::iterator identifiers_it = identifiers.begin();

	for(; identifiers_it != identifiers.end();  identifiers_it++)
	  {
	    identifier = (*identifiers_it);
	    //cout<<"\nIdentifier="<<identifier->name;
	    if(element == "")
	      element = identifier->name;
	    else
	      element = element + " " + identifier->name;
	  }
	BlockSCC.insert(element);

}

void CSymbSimulator::printSCCs(set<string> Components)
{
  set<string>::iterator Components_it;

  for(Components_it = Components.begin(); Components_it != Components.end(); Components_it++)
    {
      //cout<<"\n\nSCC :"<<(*Components_it);
      }
}


void CSymbSimulator::initializeBBTriggeringGraph(Graph_BB &BBTriggeringGraph, list<CBasicBlock*> BasicBlocks)
{
  list<CBasicBlock*>::iterator BasicBlocks_it;

  for(BasicBlocks_it = BasicBlocks.begin(); BasicBlocks_it != BasicBlocks.end(); BasicBlocks_it++)
    {
      bool NodeExists = BBTriggeringGraph.AddVertex((*BasicBlocks_it)); 
      if(!NodeExists)
	{
	  //cout<<"\n\nWARNING!!!TRYING TO ENTER "<<(*BasicBlocks_it)<<" MORE THAN ONCE INTO BB GRAPH\n\n";
	}
    }
}

CDAGNode* CSymbSimulator::getTriggeringCondition(set<CSensitiveElement*> input_identifiers, set<string> &prev_setCondition, int time)
{
  CDAGNode *condition = NULL;
  CSensitiveElement* element;
  CDAGNode *atom1, *atom2;

  prev_setCondition.clear();

  set<CSensitiveElement*>::iterator input_identifiers_it = input_identifiers.begin();

  for(; input_identifiers_it != input_identifiers.end(); input_identifiers_it++)
	  {
	    element = (*input_identifiers_it);
	    int low = atoi(element->lower.c_str());
	    int up = atoi(element->upper.c_str());
	    
	    if(condition == NULL)
	      {
		atom1 = MANAGER.createDAG(element->name, low, up, time);
		atom2 = MANAGER.createDAG(element->name, low, up, time-1);
		condition = MANAGER.createDAG("is_not_equal",atom1,atom2);
		prev_setCondition.insert(atom1->getDAGStr());
		prev_setCondition.insert(atom2->getDAGStr());
	      }
	    else
	      {
		atom1 = MANAGER.createDAG(element->name, low, up, time);
		atom2 = MANAGER.createDAG(element->name, low, up, time-1);
		condition = MANAGER.createDAG("or", condition, MANAGER.createDAG("is_not_equal", atom1, atom2));
		prev_setCondition.insert(atom1->getDAGStr());
		prev_setCondition.insert(atom2->getDAGStr());
	      }
	    }
  return condition;
}
  
  
CDAGNode* CSymbSimulator::getPreviousIdDelta(CDAGNode *current)
{
  //cout<<"\n\ncurrent = "<<current->getDAGStr()<<endl<<endl;
  string id_delta = current->getDAGStr();
  int index = id_delta.find_last_of("_");
  string id = id_delta.substr(0, index);
  int current_delta = atoi(id_delta.substr(index+1).c_str());
  //cout<<"\n\ncurrent_delta = "<<current_delta;
  CDAGNode *previous = MANAGER.createDAG(id, current_delta-1); //new CDAGNode(id, current_delta-1);
  //cout<<"\n\nprevious = "<<previous->getDAGStr()<<endl<<endl;
  return previous;
}


/* Functions to change the Symbolic Simulation Scheme */


void CSymbSimulator::initializeDeltaMapForBB (map<CSensitiveElement*, string> &delays, CBasicBlock* thisBB, map<string, list<CSenNode*> > affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
  set<CSensitiveElement*> inputIdentifiers = thisBB->get_Input_identifiers();

  set<CSensitiveElement*>::iterator inputIdentifiers_it;

  //cout<<"\n\nINPUT IDENTIFIERS\n\n";

  //for(inputIdentifiers_it = inputIdentifiers.begin(); inputIdentifiers_it != inputIdentifiers.end(); inputIdentifiers_it++)
  //  {
  //    
  //    (*inputIdentifiers_it)->printCSensitiveElement();
  //    cout<<"\n";
  //  }

  for(inputIdentifiers_it = inputIdentifiers.begin(); inputIdentifiers_it != inputIdentifiers.end(); inputIdentifiers_it++)
    {

      string identifier = (*inputIdentifiers_it)->name;

      if(affecting.find(identifier) != affecting.end())
	{

		list<CSenNode*> &sennodelist = affecting[identifier];

		//cout<<"ID: "<<identifier<<" size = "<<sennodelist.size()<<endl;
		list<CSenNode*>::iterator ien;
		for (ien = sennodelist.begin(); ien != sennodelist.end(); ++ien)
		{
			CBasicBlock* bb = nodePartOfBasicBlock[(*ien)->n];

			if(bb == thisBB )
			  {
			    //cout<<"\n\nthisBB is sensitive to "<<identifier<<". Inserting into delay map\n\n";
			    CSensitiveElement* NewElement = new CSensitiveElement((*inputIdentifiers_it)->name, (*inputIdentifiers_it)->upper, (*inputIdentifiers_it)->lower);
			    if(isPresentInDeltaMap(delays, NewElement) == 0)
			      delays.insert( pair<CSensitiveElement*, string> (NewElement, ""));
			  }
		}
	}
      
    }
}


void CSymbSimulator::initializeDeltaMapForEntity (map<CSensitiveElement*, string> &delays, set<CSensitiveElement*> inputIdentifiers)
{
  

  set<CSensitiveElement*>::iterator inputIdentifiers_it;

  //cout<<"\n\nINPUT IDENTIFIERS\n\n";

  for(inputIdentifiers_it = inputIdentifiers.begin(); inputIdentifiers_it != inputIdentifiers.end(); inputIdentifiers_it++)
    {
      
      //(*inputIdentifiers_it)->printCSensitiveElement();
      //cout<<"\n";
    }

  for(inputIdentifiers_it = inputIdentifiers.begin(); inputIdentifiers_it != inputIdentifiers.end(); inputIdentifiers_it++)
    {

     delays.insert( pair<CSensitiveElement*, string> (new CSensitiveElement((*inputIdentifiers_it)->name, (*inputIdentifiers_it)->upper, (*inputIdentifiers_it)->lower), ""));
     }
}

  
/* These functions may not be necessary */

void CSymbSimulator::createValuesAtOneTimeForOverall(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, set<CSensitiveElement*> &input_identifiers, map<string, list<CSenNode*> > &affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock, Graph_BB &BBTriggeringGraph)
{
	set <CSensitiveElement*> ::iterator ist;

	map<string, SymTabEntry> & idMap = SymbolTable[ent.getName()];

	for (ist=input_identifiers.begin(); ist != input_identifiers.end(); ++ist)
	{
/*		if(idMap[*ist].id_type == "port::in" || idMap[*ist].id_type == "port::inout" )
		{
			ConditionsValues &rv = SymbolicQueues[*ist];

			createNextSignal(SymbolicQueues, ent, *ist, affecting, nodePartOfBasicBlock, BBTriggeringGraph);
		}
		else
		{
			ConditionsValues &rv = SymbolicQueues[*ist];

			rv.values.push_back(rv.values[rv.values.size()-1]);
			rv.conditions.push_back(rv.conditions[rv.conditions.size()-1]);
		}*/

		ConditionsValues &rv = SymbolicQueues[(*ist)->name];

		//cout<<"\n\nCreating one delta at one time "<<(*ist)->name<<endl<<endl;

		createNextSignalForOverall(SymbolicQueues, ent, (*ist)->name, affecting, nodePartOfBasicBlock, BBTriggeringGraph);


/*		ConditionsValues rv;

		vector< pair<int, int> > atom_ranges;
		vector<CDAGNode*> values;
		vector<CDAGNode*> conditions;

		CDAGNode*v = MANAGER.createDAG(*ist);
		CDAGNode*c = MANAGER.createDAG("true");

		cout<<v->getDAGStr()<<endl;
		cout<<c->getDAGStr()<<endl;


//		rv.atom_ranges = atom_ranges;
		rv.values.push_back(v);
		rv.conditions.push_back(c);

		SymbolicQueues[*ist] = rv;*/
	}
}



void CSymbSimulator::createNextSignalForOverall(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, string  identifier,
					map<string, list<CSenNode*> > &affecting,
					map<node *, CBasicBlock* > &nodePartOfBasicBlock, Graph_BB &BBTriggeringGraph)
{
	vector<CDAGNode*> values;
	vector<CDAGNode*> conditions;
	set<string> emptySetCondition;

	char *temp = strdup(identifier.c_str());
	

	SymbolicQueues[identifier].delta.push_back(1);
	SymbolicQueues[identifier].values.push_back(MANAGER.createDAG(identifier, 1));
	SymbolicQueues[identifier].conditions.push_back(MANAGER.createDAG("true"));
	SymbolicQueues[identifier].setConditions.push_back(emptySetCondition);
	
	int size = SymbolicQueues[identifier].ranges_covered.size();
	SymbolicQueues[identifier].ranges_covered.push_back(SymbolicQueues[identifier].ranges_covered[size-1]);

//	rv.values.push_back(values);
//	rv.conditions.push_back(conditions);

//	rv.values[1] = values;
//	rv.conditions[1] = conditions;

	//cout<<"Affected BB size = "<<affectedBasicBlocksForOverall.size()<<endl;


	if(affecting.find(identifier) != affecting.end())
	{

		list<CSenNode*> &sennodelist = affecting[identifier];

		//cout<<"ID: "<<identifier<<" size = "<<sennodelist.size()<<endl;
		list<CSenNode*>::iterator ien;
		for (ien = sennodelist.begin(); ien != sennodelist.end(); ++ien)
		{
			CBasicBlock* bb = nodePartOfBasicBlock[(*ien)->n];

			//cout<<"\nDisplaying Basic Block sensitive to "<<identifier<<endl;

			//bb->display();

			//cout<<"\nAffected BB size inside for = "<<affectedBasicBlocksForOverall.size()<<endl;

			CSensitiveElement* se = getSenEleforInput(bb->get_Input_identifiers(), identifier);  

			//if(se==NULL) cout<<"\n\nse is NULL\n\n";
			//else cout<<"\n\nse is non NULL\n\n";

			//cout<<"\nChecking "<<identifier<<" in input id list of Basic Block, and the Sen.Elmt. of identifier is : ";

			//cout<<se->name<<"("<<se->lower<<","<<se->upper<<")";

			//cout<<"\n\nIdentifier";

			//cout<<identifier<<endl<<endl;

			//set<string> setCondition;

			//CDAGNode* triggering_condition = calculateEventCondition(1, SymbolicQueues[identifier], se, setCondition);

			//cout<<"\nTriggering Condn : "<<triggering_condition->getDAGStr();

			insertToAffectedBBForOverall(1, se, bb, NULL, BBTriggeringGraph);

			//displayAffectedBB();

			//cout<<affectedBasicBlocksForOverall.size()<<endl;
		}
	}
}



void CSymbSimulator::insertToAffectedBBForOverall(int delta, CSensitiveElement* input, CBasicBlock* bb, CBasicBlock* source, Graph_BB &BBTriggeringGraph)
{
       //cout<<"insertToAffectedBBForOverall"<<endl;

	if(structural_cycle_present && !real_cycle_present && source != NULL)
	  {
	   BBTriggeringGraph.AddEdge(source, bb);

	   if(BBTriggeringGraph.checkCycles()==1)
	     {
	       //cout<<"\n\nNo need to retrigger the basic block. No need to add it to AffectedBBForOverall\n\n";
	       BBTriggeringGraph.deleteEdge(source, bb);
	       return;
	     }
	  }

	//bool found = false;

	list<AffBlocksAndDeltaWithInput*>::iterator result;
	for(result = affectedBasicBlocksForOverall.begin(); result != affectedBasicBlocksForOverall.end(); result++);
	  /*  {
	    	if(delta == (*result)->delta && bb == (*result)->bb)
		{
		  cout<<"\n\nENTRY ALREADY EXISTS IN AffectedBBForOverall. NEW ENTRY NOT INSERTED\n\n";
		  found = true;
		  break;
		}
		}*/
		  
	// If not found, insert at the rear

	  //	if(!found)
	  // {
	    AffBlocksAndDeltaWithInput *abd = new AffBlocksAndDeltaWithInput(delta, input, bb);
	    affectedBasicBlocksForOverall.insert(result, abd);
	    // }

}


CDAGNode* CSymbSimulator::getTriggeringConditionForOverall(map<CSensitiveElement*, string> delay_map, set<string> &prev_setCondition, CSensitiveElement* Changed_Input, int time)

{
  CDAGNode *condition;
  CSensitiveElement* element;
  CDAGNode *atom1, *atom2;
  
  prev_setCondition.clear();

  element = Changed_Input;
  int low = atoi(element->lower.c_str());
  int up = atoi(element->upper.c_str());
	    
  atom1 = MANAGER.createDAG(element->name, low, up, time);
  atom2 = MANAGER.createDAG(element->name, low, up, time-1);
  condition = MANAGER.createDAG("is_not_equal",atom1,atom2);
  prev_setCondition.insert(atom1->getDAGStr());
  prev_setCondition.insert(atom2->getDAGStr());
  
  int input_delta_time = findDeltaTimeOfInput(delay_map, Changed_Input);

  for(map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin(); delay_map_it != delay_map.end(); delay_map_it++)
	  {
	    element = (*delay_map_it).first;
	    int low = atoi(element->lower.c_str());
	    int up = atoi(element->upper.c_str());

	    string element_delta_time_string = (*delay_map_it).second;

	    int element_delta_time = atoi(element_delta_time_string.c_str());
	    
	    if(element->name != Changed_Input->name)
	      {
		int element_shift_wrt_changed_input = element_delta_time -  input_delta_time;
		int element_time_wrt_time =  time - element_shift_wrt_changed_input;

		atom1 = MANAGER.createDAG(element->name, low, up,element_time_wrt_time);
		atom2 = MANAGER.createDAG(element->name, low, up, element_time_wrt_time-1);
		condition = MANAGER.createDAG("or", condition, MANAGER.createDAG("is_not_equal", atom1, atom2));
		prev_setCondition.insert(atom1->getDAGStr());
		prev_setCondition.insert(atom2->getDAGStr());
	      }
	    }
  return condition;
}
  

int CSymbSimulator::findDeltaTimeOfInput(map<CSensitiveElement*, string> delay_map, CSensitiveElement* Input)
{
  int element_delta_time;
  string element_delta_time_string;
  CSensitiveElement* element;


  //cout<<"\n\nDELAY MAP\n";

  //for(map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin(); delay_map_it != delay_map.end(); delay_map_it++)
  //{
  //			  cout<< "Input: ";
  //			  (*delay_map_it).first->printCSensitiveElement();
  //			  cout<<"\t";
  //			  cout<< "Time:  " << (*delay_map_it).second<<endl;
  //			}


  for(map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin(); delay_map_it != delay_map.end(); delay_map_it++)
	  {
	    element = (*delay_map_it).first;
	    if(element->name == Input->name && element->upper == Input->upper && element->lower == Input->lower)
	      {
	       element_delta_time_string = (*delay_map_it).second;
	       element_delta_time = atoi(element_delta_time_string.c_str());
	       return element_delta_time;
	      }
	    }

  //cout<<"\n\nERROR! ";
  //Input->printCSensitiveElement();
  //cout<<" not found in delay map. Exiting.....";
  exit(1);
  return -1;
}

int CSymbSimulator::isPresentInDeltaMap(map<CSensitiveElement*, string> delay_map, CSensitiveElement* Input)
{
  CSensitiveElement* element;
 for(map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin(); delay_map_it != delay_map.end(); delay_map_it++)
	  {
	    element = (*delay_map_it).first;
	    if(element->name == Input->name && element->upper == Input->upper && element->lower == Input->lower)
	      {
	       return 1;
	      }
	    }

  return 0;
}


void CSymbSimulator::updateMapSignalDelta(map<string, list<ConditionRelation*> >& mapSignalDelta, ConditionRelation *cr, string output_identifier, int present_delta_time, string output_identifier_with_delta)
{
  map<string, list<ConditionRelation*> >::iterator mapSignalDelta_it;
  int flag = 0;

  /*for(mapSignalDelta_it = mapSignalDelta.begin(); mapSignalDelta_it != mapSignalDelta.end(); mapSignalDelta_it++)
    {
      string id_delta = (*mapSignalDelta_it).first;
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index); 
      int delta = atoi(id_delta.substr(index+1).c_str()); 
      
      if((id == output_identifier) && (present_delta_time == delta))  // Entry at the same time. Append the new entry
	{
	  (*mapSignalDelta_it).second.push_back(cr);
	  flag = 1;
	  break;
	}
      else if(id == output_identifier)
	{
	  mapSignalDelta.erase(mapSignalDelta_it); // erasing like this is wrong. change this when uncommented
	  mapSignalDelta[output_identifier_with_delta].push_back(cr);
	  flag = 1;
	  break;
	}
    }
  
  if(flag == 0)
  mapSignalDelta[output_identifier_with_delta].push_back(cr); */



  //cout<<"\n\nRANGES OF CR TO BE INSERTED\n\n";

  //for(int i=0; i<cr->ranges_covered.size(); i++)
  //  cout<<"Lower = "<<cr->ranges_covered[i].first<<",Upper = "<<cr->ranges_covered[i].second<<endl;
  

  for(mapSignalDelta_it = mapSignalDelta.begin(); mapSignalDelta_it != mapSignalDelta.end(); mapSignalDelta_it++)
    {
      string id_delta = (*mapSignalDelta_it).first;
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index); 
      int delta = atoi(id_delta.substr(index+1).c_str()); 

            
      if((id == output_identifier) && (present_delta_time == delta))  // Entry at the same time. Don't insert the new entry
	{
	  list<ConditionRelation*> existing_cr = (*mapSignalDelta_it).second;
	  
	  for(list<ConditionRelation*>::iterator existing_cr_it = existing_cr.begin(); existing_cr_it != existing_cr.end(); existing_cr_it++)
	    {
	      if( (cr->ranges_covered[0].first == (*existing_cr_it)->ranges_covered[0].first) && (cr->ranges_covered[0].second == (*existing_cr_it)->ranges_covered[0].second))
		  {
		    //cout<<"\n\nRANGES MATCHING!!!NEW ENTRY IS NOT INSERTED\n\n";
		    flag = 1;
		    break;
		  }
	    }
	  
	}

    }
  
  if(flag == 0)
  mapSignalDelta[output_identifier_with_delta].push_back(cr);

  


  /*

  

  // Insert at the rear

  for(mapSignalDelta_it = mapSignalDelta.begin(); mapSignalDelta_it != mapSignalDelta.end(); mapSignalDelta_it++);
  mapSignalDelta[output_identifier_with_delta].push_back(cr);*/


  
}


void CSymbSimulator::initializeDeltasForBBSimulation(CBasicBlock* bb, map<int, list<string> > &deltas)
{
  set<CSensitiveElement*> input_identifiers = bb->get_Input_identifiers();

  for(set<CSensitiveElement*>::iterator it = input_identifiers.begin(); it != input_identifiers.end(); it++)
    {
      deltas[1].push_back((*it)->name);
    }
}

void CSymbSimulator::showDeltasForBBSimulation(map<int, list<string> > deltas)
{
  //cout<<"\n\nDELTAS IN BB SIMULATION\n\n";
  
  for(map<int, list<string> >::iterator it = deltas.begin(); it != deltas.end(); it++)
    {
      //cout<<(*it).first<<"\t";

      list<string> ids = (*it).second;
      for(list<string>::iterator ids_it = ids.begin(); ids_it != ids.end(); ids_it++)
	{
	//cout<<(*ids_it)<<"\t";
	}

      //cout<<"\n";
    }
}
  
// Find the entry in deltas with the least delta time, copy it in the arguments, and delete that location
void CSymbSimulator::getInputsAtSmallestDelta(map<int, list<string> > &deltas, int &current, list<string> &InputsAtCurrent)
{
  //cout<<"\n\nInside getInputsAtSmallestDelta\n\n";

  map<int, list<string> > deltas_new;
  map<int, list<string> >::iterator it = deltas.begin();
  current = (*it).first;
  

  for(; it != deltas.end(); it++)
    {
      if(current > (*it).first)
	{
	  current = (*it).first;
	}
    }

  InputsAtCurrent = deltas[current];

 for(it = deltas.begin(); it != deltas.end(); it++)
    {
      if((*it).first != current)
	{
	  deltas_new[(*it).first]=(*it).second;
	}
    }
 
  deltas = deltas_new;
  
}
  
  
int CSymbSimulator::findDeltaTimeOfInputString(map<CSensitiveElement*, string> delay_map, string Input)
{
  int element_delta_time;
  string element_delta_time_string;
  CSensitiveElement* element;

  //cout<<"\n\nDELAY MAP\n";

  //for(map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin(); delay_map_it != delay_map.end(); delay_map_it++)
  //			{
  //			  cout<< "Input: ";
  //			  (*delay_map_it).first->printCSensitiveElement();
  //			  cout<<"\t";
  //			  cout<< "Time:  " << (*delay_map_it).second<<endl;
  //			}
  //

  for(map<CSensitiveElement*, string>::iterator delay_map_it = delay_map.begin(); delay_map_it != delay_map.end(); delay_map_it++)
	  {
	    element = (*delay_map_it).first;
	    if(element->name == Input)
	      {
	       element_delta_time_string = (*delay_map_it).second;
	       element_delta_time = atoi(element_delta_time_string.c_str());
	       return element_delta_time;
	      }
	    }

  //cout<<"\n\nERROR! ";
  //cout<<Input<<" not found in delay map. Exiting.....";
  exit(1);
  return -1;
}


CDAGNode* CSymbSimulator::addAdditionalConstraints(CDAGNode *final_dag, set<CSensitiveElement*> input_identifiers)
{
  
  set<string> setCondRel;
  final_dag->getDAGNames(setCondRel);
  CDAGNode *new_final_dag = final_dag;
  			
  while(!setCondRel.empty())
    {
      
      string element = *(setCondRel.begin());
      
      //cout<<"\n\nElement = "<<element<<endl<<endl;

      setCondRel.erase(element);
      
      int index = element.find_last_of("_");
      string id = element.substr(0, index); 
      int delta = atoi(element.substr(index+1).c_str()); 

      CSensitiveElement* se = getSenEleforInput(input_identifiers, id);
      
      if(se != NULL && delta >= 2)
	{
	  CDAGNode *temp1 = MANAGER.createDAG(se->name, atoi((se->lower).c_str()), atoi((se->upper).c_str()), delta);
	  CDAGNode *temp2 = MANAGER.createDAG(se->name, atoi((se->lower).c_str()), atoi((se->upper).c_str()), 1);
	  CDAGNode *temp3 = MANAGER.createDAG("equivalent", temp1, temp2);
	  new_final_dag = MANAGER.createDAG("and", new_final_dag, temp3);
	}
    }

  return new_final_dag;
}


bool CSymbSimulator::findElementWithLargerOrSameDelta(set<string> setDone, string new_element)
{
  int new_index = new_element.find_last_of("_");
  string new_id = new_element.substr(0, new_index); 
  int new_delta = atoi(new_element.substr(new_index+1).c_str()); 

  string element;
  int index;
  string id;
  int delta;

  set<string>::iterator setDone_it;

  for(setDone_it = setDone.begin(); setDone_it != setDone.end(); setDone_it++)
	  {
	    element = (*setDone_it);
	    index = element.find_last_of("_");
	    id = element.substr(0, index); 
	    delta = atoi(element.substr(index+1).c_str()); 

	    if(new_id == id)
	      {
		if(new_delta <= delta)
		  {
		    return true;
		  }
	      }
	  }
  return false;
}
	    
void CSymbSimulator::addElementToSetDone(set<string> &setDone, string new_element)
{
  int new_index = new_element.find_last_of("_");
  string new_id = new_element.substr(0, new_index); 
  int new_delta = atoi(new_element.substr(new_index+1).c_str()); 

  string element;
  int index;
  string id;
  int delta;

  set<string>::iterator setDone_it;

  for(setDone_it = setDone.begin(); setDone_it != setDone.end(); setDone_it++)
	  {
	    element = (*setDone_it);
	    index = element.find_last_of("_");
	    id = element.substr(0, index); 
	    delta = atoi(element.substr(index+1).c_str()); 

	    if(new_id == id)
	      {
		setDone.erase(setDone_it);
		break;
	      }
	  }

  setDone.insert(new_element);
}


CDAGNode* CSymbSimulator::getDAGFromStringSet(set<string> &setExQuantify)
{
        CDAGNode* dagptr=NULL;
	
	set<string>::iterator iset = setExQuantify.begin();

		
	if(iset == setExQuantify.end())
	{
	  //cout<<"\n\nWARNING!!!FUNCTION getDAGFromStringSet returns NULL pointer. Take care in calling function\n\n";
	  return NULL;
	}

	
	string id_delta = *iset;
	int index = id_delta.find_last_of("_");
	string id = id_delta.substr(0, index); 
	int delta = atoi(id_delta.substr(index+1).c_str()); 


	dagptr = MANAGER.createDAG(id, delta);

	iset++;
	
	for(; iset != setExQuantify.end(); iset++)
	{
	        id_delta = *iset;
		index = id_delta.find_last_of("_");
		id = id_delta.substr(0, index); 
		delta = atoi(id_delta.substr(index+1).c_str()); 
		dagptr = MANAGER.createDAG("comma", dagptr, MANAGER.createDAG(id, delta));
		
	}
	
	return dagptr;
}


// struct node_info* CSymbSimulator::convertTreeToDAG(CDAGNode *tree)
// {
//   string name = tree->name;
//   int delta_time = tree->delta_time;
//   list<CDAGNode*> out_list = tree->out_list;

//   //cout<<"\nNAME = "<<name<<endl;

//    if(delta_time != MIN_DELTA)
//     {
//       struct node_info *ni_ptr = (struct node_info *) malloc(sizeof(struct node_info));

//       char temp[10];
//       sprintf(temp, "%d", delta_time);
//       name += "_";
//       name += temp;

//       if(Subexpressions.find(name) != Subexpressions.end())
// 		{
// 		        //cout<<"found"<<endl;
// 			ni_ptr->expr = strdup(name.c_str());
// 			ni_ptr->node_ptr = Subexpressions.find(name)->second;
// 		}
//       else
// 		{
// 			count2++;
// 			//cout<<"NOT found"<<endl;
// 			CExprSimpNode *n_ptr = new CExprSimpNode();

// 			n_ptr->name = name;
// 			ni_ptr->expr = strdup(name.c_str());
// 			ni_ptr->node_ptr = n_ptr;

// 			Subexpressions.insert(make_pair(ni_ptr->expr, ni_ptr->node_ptr));
// 			//cout<<"NOT found2"<<endl;
// 		}
//       count1+=1;
//       //cout<<"\n"<<name<<" finished "<<endl;
//       return ni_ptr;
//     }

//   else
//     {
//        if(name == "exists")
// 		      {
			
// 			struct node_info *ni_ptr = (struct node_info *) malloc(sizeof(struct node_info));

// 			list<CDAGNode*>::iterator it = out_list.begin();
// 			//struct node_info *child1 = convertTreeToDAG(*it);
// 			it++;
// 			struct node_info *child2 = convertTreeToDAG(*it);
// 			it = out_list.begin();
// 			struct node_info *child1 = convertTreeToDAG(*it);
			
			
// 			struct node_info *ni_ptr_list;
// 			string toBeSearched;

// 			if(child1 == NULL)
// 			  {
// 			    ni_ptr_list = (struct node_info *) malloc(sizeof(struct node_info));
// 			    toBeSearched = "(";
// 			    toBeSearched =  toBeSearched + "exprList";
// 			    toBeSearched = toBeSearched + ",";
		
// 			    list<node_info *>::iterator iter = exList.begin();
// 			    for(; iter != exList.end(); iter++)
// 			      {
// 				toBeSearched = toBeSearched + (*iter)->expr + ",";
// 			      }
		
// 			    toBeSearched = toBeSearched + ")";

// 			    if(Subexpressions.find(toBeSearched) != Subexpressions.end())
// 			      {
			
// 				ni_ptr_list->expr = strdup(toBeSearched.c_str());
// 				ni_ptr_list->node_ptr = Subexpressions.find(toBeSearched)->second;
// 			      }
// 			    else
// 			      {
// 				count2++;
// 				CExprSimpNode *n_ptr_list = new CExprSimpNode();
// 				n_ptr_list->name = "exprList";

// 				iter = exList.begin();
// 				for(; iter != exList.end(); iter++)
// 				  {
// 				    (n_ptr_list->out_list).push_back((*iter)->node_ptr);
// 				    (((*iter)->node_ptr)->in_list).push_back(n_ptr_list);
// 				  }

// 				ni_ptr_list->expr = strdup(toBeSearched.c_str());
// 				ni_ptr_list->node_ptr = n_ptr_list;

// 				Subexpressions.insert(make_pair(ni_ptr_list->expr, ni_ptr_list->node_ptr));
// 			      }
		
// 			    exList.clear();
// 			  }
// 			else
// 			  {
// 			    ni_ptr_list = (struct node_info *) malloc(sizeof(struct node_info));
// 			    toBeSearched = "(";
// 			    toBeSearched =  toBeSearched + "exprList";
// 			    toBeSearched = toBeSearched + ",";
// 			    toBeSearched = toBeSearched + child1->expr;
// 			    toBeSearched = toBeSearched + ")";

// 			    if(Subexpressions.find(toBeSearched) != Subexpressions.end())
// 			      {
			
// 				ni_ptr_list->expr = strdup(toBeSearched.c_str());
// 				ni_ptr_list->node_ptr = Subexpressions.find(toBeSearched)->second;
// 			      }
// 			    else
// 			      {
// 				count2++;
// 				CExprSimpNode *n_ptr_list = new CExprSimpNode();
// 				n_ptr_list->name = "exprList";

// 				(n_ptr_list->out_list).push_back(child1->node_ptr);
// 				((child1->node_ptr)->in_list).push_back(n_ptr_list);
				

// 				ni_ptr_list->expr = strdup(toBeSearched.c_str());
// 				ni_ptr_list->node_ptr = n_ptr_list;

// 				Subexpressions.insert(make_pair(ni_ptr_list->expr, ni_ptr_list->node_ptr));
// 			      }
// 			  }


// 			toBeSearched = "(";
// 			toBeSearched =  toBeSearched + "Exists";
		
// 			toBeSearched = toBeSearched + ",";
// 			toBeSearched = toBeSearched + ni_ptr_list->expr;
// 			toBeSearched = toBeSearched + ",";
// 			toBeSearched = toBeSearched + child2->expr;

// 			toBeSearched = toBeSearched + ")";

// 			if(Subexpressions.find(toBeSearched) != Subexpressions.end())
// 			  {
			
// 			    ni_ptr->expr = strdup(toBeSearched.c_str());
// 			    ni_ptr->node_ptr = Subexpressions.find(toBeSearched)->second;
// 			  }
// 			else
// 			  {
// 			    count2++;
// 			    CExprSimpNode *n_ptr = new CExprSimpNode();
// 			    n_ptr->name = "Exists";

// 			    (n_ptr->out_list).push_back(ni_ptr_list->node_ptr);
// 			    (n_ptr->out_list).push_back(child2->node_ptr);

// 			    ((ni_ptr_list->node_ptr)->in_list).push_back(n_ptr);
// 			    ((child2->node_ptr)->in_list).push_back(n_ptr);

// 			    ni_ptr->expr = strdup(toBeSearched.c_str());
// 			    ni_ptr->node_ptr = n_ptr;

// 			    Subexpressions.insert(make_pair(ni_ptr->expr, ni_ptr->node_ptr));
// 			  }
// 			count1+=1;
// 			//cout<<"\n"<<name<<" finished "<<endl;
// 			return ni_ptr;
// 		      }
		
	
//       else if(name == "comma") // "comma" is the operator separating individual variables in the variable's list in exists
// 		      {
			
// 			list<CDAGNode*>::iterator it = out_list.begin();
// 			struct node_info *child1 = convertTreeToDAG(*it);
// 			it++;
// 			struct node_info *child2 = convertTreeToDAG(*it);
			
// 			if(child1 != NULL)
// 			  exList.push_front(child1);
// 			if(child2 != NULL)
// 			  exList.push_front(child2);

// 			count1+=1;
// 			//cout<<"\n"<<name<<" finished "<<endl;
// 			return NULL;
// 		      }
      
//       else 
	
// 		      {
// 			struct node_info *ni_ptr = (struct node_info *) malloc(sizeof(struct node_info));
			
// 			if(name == "select")
// 			  {
// 			    name = "Sel";
// 			  }

// 			else if(name == "concat")
// 			  {
// 			    name =  "Concat";
// 			  }
// 			else if(name == "not")
// 			  {
// 			    name =  "LOG_NOT";
// 			  }
// 			else if(name == "and")
// 			  {
// 			    name =  "Log_And";
// 			  }
// 			else if(name == "or")
// 			  {
// 			    name =  "Log_Or";
// 			  } 
// 			else if(name == "ite")
// 			  {
// 			    name =  "Ternary";
// 			  } 
// 			else if(name == "equivalent")
// 			  {
// 			    name =  "Log_Equal";
// 			  }
// 			else if(name == "=")
// 			  {
// 			    name =  "Log_Equal";
// 			  }
// 			else if(name == "is_not_equal")
// 			  {
// 			    name =  "Log_Inequal";
// 			  } 
// 			else if(name == "not_equal_to")
// 			  {
// 			    name =  "Log_Inequal";
// 			  } 
// 			else if(name == "-")
// 			  {
// 			    name =  "Sub";
// 			  } 
// 			else if(name == "+")
// 			  {
// 			    name =  "Add";
// 			  } 
// 			else if(name == ">")
// 			  {
// 			    name =  "Grt_than";
// 			  } 
// 			else if(name == "<")
// 			  {
// 			    name =  "Less_than";
// 			  } 
// 			else if(name == ">=")
// 			  {
// 			    name =  "Grt_Equal";
// 			  } 
// 			else if(name == "<=")
// 			  {
// 			    name =  "Less_Equal";
// 			  } 
			
		
// 			else if(out_list.begin() == out_list.end())
// 			  {
// 			    //cout<<"\nNumber "<<name<<" inside the function convertTreeToDAG in the file CExprSimpStructure.h\n";

// 			    if(name == "true")
// 			      name = "1'b1";
// 			    if(name == "false")
// 			      name = "1'b0";
// 			  }
// 			else
// 			  {
// 			    cout<<"\nUnknown function "<<name<<" inside the function convertTreeToDAG in the file CSymbSimulator.cpp...Exiting\n";
// 			    exit(1);
// 			  }


// 			if(name == "LOG_NOT") // Unary operators
// 			  {
// 			    list<CDAGNode*>::iterator it = out_list.begin();
			    
// 			    struct node_info *child1 = convertTreeToDAG(*it);
			    
			    
// 			    string toBeSearched = "(";
// 			    toBeSearched =  toBeSearched + name;
// 			    toBeSearched = toBeSearched + ",";
// 			    toBeSearched = toBeSearched + child1->expr;
// 			    toBeSearched = toBeSearched + ")";
			    
// 			    if(Subexpressions.find(toBeSearched) != Subexpressions.end())
// 			      {
			
// 				ni_ptr->expr = strdup(toBeSearched.c_str());
// 				ni_ptr->node_ptr = Subexpressions.find(toBeSearched)->second;
// 			      }
// 			    else
// 			      {
// 				count2++;
// 				CExprSimpNode *n_ptr = new CExprSimpNode();
// 				n_ptr->name = name;

// 				(n_ptr->out_list).push_back(child1->node_ptr);
// 				((child1->node_ptr)->in_list).push_back(n_ptr);

// 				ni_ptr->expr = strdup(toBeSearched.c_str());
// 				ni_ptr->node_ptr = n_ptr;

// 				Subexpressions.insert(make_pair(ni_ptr->expr, ni_ptr->node_ptr));
// 			      }
// 			  }
			
// 			else if(name == "Concat" || name == "Log_Or" || name == "Log_And" || name == "Log_Equal" || name == "Log_Inequal" ||name == "Sub" || name == "Add"  || name == "Grt_than" || name == "Less_than" || name == "Grt_Equal" || name == "Less_Equal" ) //binary operators
// 			  {
// 			    list<CDAGNode*>::iterator it = out_list.begin();
// 			    struct node_info *child1 = convertTreeToDAG(*it);
// 			    it++;
// 			    struct node_info *child2 = convertTreeToDAG(*it);

			    
// 			    string toBeSearched = "(";
// 			    toBeSearched =  toBeSearched + name;
// 			    toBeSearched = toBeSearched + ",";
// 			    toBeSearched = toBeSearched + child1->expr;
// 			    toBeSearched = toBeSearched + ",";
// 			    toBeSearched = toBeSearched + child2->expr;
// 			    toBeSearched = toBeSearched + ")";

// 			    if(Subexpressions.find(toBeSearched) != Subexpressions.end())
// 			      {
			
// 				ni_ptr->expr = strdup(toBeSearched.c_str());
// 				ni_ptr->node_ptr = Subexpressions.find(toBeSearched)->second;
// 			      }
// 			    else
// 			      {
// 				count2++;
// 				CExprSimpNode *n_ptr = new CExprSimpNode();
// 				n_ptr->name = name;

// 				(n_ptr->out_list).push_back(child1->node_ptr);
// 				(n_ptr->out_list).push_back(child2->node_ptr);

// 				((child1->node_ptr)->in_list).push_back(n_ptr);
// 				((child2->node_ptr)->in_list).push_back(n_ptr);

// 				ni_ptr->expr = strdup(toBeSearched.c_str());
// 				ni_ptr->node_ptr = n_ptr;

// 				Subexpressions.insert(make_pair(ni_ptr->expr, ni_ptr->node_ptr));
// 			      }
// 			  }
// 			else if(name == "Ternary" || name == "Sel")// operators with three arguments 
// 			  {
// 			    list<CDAGNode*>::iterator it = out_list.begin();
// 			    struct node_info *child1 = convertTreeToDAG(*it);
// 			    it++;
// 			    struct node_info *child2 = convertTreeToDAG(*it);
// 			    it++;
// 			    struct node_info *child3 = convertTreeToDAG(*it);
			    
// 			    //cout<<"\nBefore toBeSearched\n";
// 			    //cout<<"\nBefore toBeSearched\n";
// 			    //cout<<"\nBefore toBeSearched\n";
// 			    //cout<<"\nBefore toBeSearched\n";

// 			    string toBeSearched = "(";
// 			    toBeSearched =  toBeSearched + name;
// 			    toBeSearched = toBeSearched + ",";
			    
// 			    //cout<<"\nHere1\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere1\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere1\t"<<toBeSearched<<"\n";

// 			    toBeSearched = toBeSearched + child1->expr;

// 			    //cout<<"\nHere2\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere2\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere2\t"<<toBeSearched<<"\n";

// 			    toBeSearched = toBeSearched + ",";
// 			    toBeSearched = toBeSearched + child2->expr;

// 			    //cout<<"\nHere3\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere3\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere3\t"<<toBeSearched<<"\n";

// 			    toBeSearched = toBeSearched + ",";
// 			    toBeSearched = toBeSearched + child3->expr;
// 			    toBeSearched = toBeSearched + ")";

// 			    //cout<<"\nHere4\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere4\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nHere4\t"<<toBeSearched<<"\n";

			    

// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";
// 			    //cout<<"\nAfter toBeSearched\t"<<toBeSearched<<"\n";

			    

// 			    if(Subexpressions.find(toBeSearched) != Subexpressions.end())
// 			      {
			
// 				ni_ptr->expr = strdup(toBeSearched.c_str());
// 				ni_ptr->node_ptr = Subexpressions.find(toBeSearched)->second;
// 			      }
// 			    else
// 			      {
// 				count2++;
// 				CExprSimpNode *n_ptr = new CExprSimpNode();
// 				n_ptr->name = name;

// 				//cout<<"\nBefore Pushbacks On Outlist\n";
// 				//cout<<"\nBefore Pushbacks On Outlist\n";
// 				//cout<<"\nBefore Pushbacks On Outlist\n";
// 				//cout<<"\nBefore Pushbacks On Outlist\n";			

// 				(n_ptr->out_list).push_back(child1->node_ptr);
// 				(n_ptr->out_list).push_back(child2->node_ptr);
// 				(n_ptr->out_list).push_back(child3->node_ptr);

// 				//cout<<"\nBefore Pushbacks On Inlist\n";
// 				//cout<<"\nBefore Pushbacks On Inlist\n";
// 				//cout<<"\nBefore Pushbacks On Inlist\n";
// 				//cout<<"\nBefore Pushbacks On Inlist\n";	

// 				((child1->node_ptr)->in_list).push_back(n_ptr);
// 				((child2->node_ptr)->in_list).push_back(n_ptr);
// 				((child3->node_ptr)->in_list).push_back(n_ptr);

// 				//cout<<"\nBefore Final Corrections\n";
// 				//cout<<"\nBefore Final Corrections\n";
// 				//cout<<"\nBefore Final Corrections\n";
// 				//cout<<"\nBefore Final Corrections\n";

// 				ni_ptr->expr = strdup(toBeSearched.c_str());
// 				ni_ptr->node_ptr = n_ptr;

// 				Subexpressions.insert(make_pair(ni_ptr->expr, ni_ptr->node_ptr));
// 			      }

			    
// 			  }

// 			else // constants
// 			  {
			     
// 			     if(Subexpressions.find(name) != Subexpressions.end())
// 			       {
// 				 //cout<<"found"<<endl;
// 				 ni_ptr->expr = strdup(name.c_str());
// 				 ni_ptr->node_ptr = Subexpressions.find(name)->second;
// 			       }
// 			     else
// 			       {
// 				 count2++;
// 				 //cout<<"NOT found"<<endl;
// 				 CExprSimpNode *n_ptr = new CExprSimpNode();

// 				 n_ptr->name = name;
// 				 ni_ptr->expr = strdup(name.c_str());
// 				 ni_ptr->node_ptr = n_ptr;

// 				 Subexpressions.insert(make_pair(ni_ptr->expr, ni_ptr->node_ptr));
// 				 //cout<<"NOT found2"<<endl;
// 			       }
// 			  }
// 			count1+=1;
// 			//cout<<"\n"<<name<<" finished "<<endl;
// 			//cout<<"\n"<<name<<" finished "<<endl;
// 			//cout<<"\n"<<name<<" finished "<<endl;
// 			//cout<<"\n"<<name<<" finished "<<endl;
// 			return ni_ptr;
// 		      }
			

//     }

   
				
		
// }
	
// CExprSimpNode* CSymbSimulator::convertCDAGNodeToCExprSimpNode(CDAGNode *tree)
// {
//   struct node_info* ni_ptr = convertTreeToDAG(tree);
//   return ni_ptr->node_ptr;
// }

// CDAGNode* CSymbSimulator::convertCExprSimpNodeToCDAGNode(CExprSimpNode *dag)
// {
//   string name = dag->name;
//   list<CExprSimpNode*> out_list = dag->out_list;

//   if(out_list.begin() == out_list.end()) // Terminal node
//     {
//       if(name.find("_") == string::npos) // Constant
// 	{
// 	  //cout<<"\nConstant = "<<name<<endl;
// 	  if(name == "1'b1")
// 	    name = "true";
// 	  if(name == "1'b0")
// 	    name = "false";
// 	  //cout<<"\nConstant = "<<name<<endl; 
// 	  return MANAGER.createDAG(name); 
// 	}
//       else // Variable
// 	{
// 	  int index = name.find_last_of("_");
// 	  string id = name.substr(0, index); 
// 	  int delta = atoi(name.substr(index+1).c_str()); 
// 	  return MANAGER.createDAG(id, delta);
// 	}
//     }
//   else
//     {
//       if(name == "exprList") // "exprList" should be converted to cascaded "comma"s
// 		      {
// 			set<string> setExQuantify;
// 			list<CExprSimpNode*>::iterator it;

// 			for(it = out_list.begin();it != out_list.end(); it++)
// 			  setExQuantify.insert((*it)->name);

// 			if(setExQuantify.empty())
// 			  {
// 			    cout<<"\nExists operator with empty list obtained from expression simplifier...Exiting\n";
// 			    exit(1);
// 			  }

// 			return getDAGFromStringSet(setExQuantify);
// 		      }
      
//       else 
	
// 		      {
// 			if(name == "Sel")
// 			  {
// 			    name = "select";
// 			  }

// 			else if(name == "Concat")
// 			  {
// 			    name =  "concat";
// 			  }
// 			else if(name == "LOG_NOT")
// 			  {
// 			    name =  "not";
// 			  }
// 			else if(name == "Log_And")
// 			  {
// 			    name =  "and";
// 			  }
// 			else if(name == "Log_Or")
// 			  {
// 			    name =  "or";
// 			  } 
// 			else if(name == "Ternary")
// 			  {
// 			    name =  "ite";
// 			  } 
// 			else if(name == "Log_Equal")
// 			  {
// 			    name =  "equivalent";
// 			  } 
// 			else if(name == "Log_Inequal")
// 			  {
// 			    name =  "is_not_equal";
// 			  } 
// 			else if(name == "Exists")
// 			  {
// 			    name =  "exists";
// 			  } 
// 			else if(name == "Add")
// 			  {
// 			    name =  "+";
// 			  } 
// 			else if(name == "Sub")
// 			  {
// 			    name =  "-";
// 			  } 
// 			else if(name == "Grt_than")
// 			  {
// 			    name =  ">";
// 			  } 
// 			else if(name == "Less_than")
// 			  {
// 			    name =  "<";
// 			  } 
// 			else if(name == "Grt_Equal")
// 			  {
// 			    name =  ">=";
// 			  } 
// 			else if(name == "Less_Equal")
// 			  {
// 			    name =  "<=";
// 			  } 
			
// 			else
// 			  {
// 			    cout<<"\nUnknown function "<<name<<" inside the function convertCExprSimpNodeToCDAGNode in the file CSymbSimulator.cpp...Exiting\n";
// 			    exit(1);
// 			  }


// 			if(name == "not") // Unary operators
// 			  {
// 			    list<CExprSimpNode*>::iterator it = out_list.begin();
			    
// 			    CDAGNode* child1 = convertCExprSimpNodeToCDAGNode(*it);

// 			    return MANAGER.createDAG(name, child1);
// 			  }
			    
// 			else if(name == "concat" || name == "or" || name == "and" || name == "equivalent" || name == "is_not_equal" || name == "exists" || name == "+" || name == "-" || name == ">" || name == "<" || name == ">=" || name == "<=") //binary operators
// 			  {
// 			    list<CExprSimpNode*>::iterator it = out_list.begin();
			    
// 			    CDAGNode* child1 = convertCExprSimpNodeToCDAGNode(*it);
// 			    it++;
// 			    CDAGNode* child2 = convertCExprSimpNodeToCDAGNode(*it);

// 			    return MANAGER.createDAG(name, child1, child2);

// 			   }
// 			else if(name == "ite" || name == "select")// operators with three arguments 
// 			  {
// 			    list<CExprSimpNode*>::iterator it = out_list.begin();
// 			    CDAGNode* child1 = convertCExprSimpNodeToCDAGNode(*it);
// 			    it++;
// 			    CDAGNode* child2 = convertCExprSimpNodeToCDAGNode(*it);
// 			    it++;
// 			    CDAGNode* child3 = convertCExprSimpNodeToCDAGNode(*it);
			    
// 			    return MANAGER.createDAG(name, child1, child2, child3);
// 			   }

			
// 		      }
			

//     }
		
	
// }

// int CSymbSimulator::findSizeOfDAG(struct CExprSimpNode* x)
// {
//   set <struct CExprSimpNode*> addresses;
//   int size = 0;

//   x->computeDAGSize(addresses,size);

//   return size;
// }

// // Here x should be exists(comma(...comma(x,y)...),SE)
// // This takes the list to be quantified into a set S and then calls Inlining(S,SE)
				    
// CDAGNode* CSymbSimulator::callInlining(CDAGNode *dag)
// {
//   //cout<<"\nDAG = "<<dag->getDAGStr()<<endl;

//   list<CDAGNode*>::iterator iout = dag->out_list.begin();
//   set<string> VarSet, VarSet2, OriginalVarSet;
//   (*iout)->getDAGNames(VarSet);
//   iout++;
//   CDAGNode *RHS = (*iout);
   
//   for(set<string>::iterator VarSet_it = VarSet.begin(); VarSet_it != VarSet.end(); VarSet_it++)

// 	 {
// 	   string element = *(VarSet_it);
// 	   //cout<<"\nelement = "<<element<<endl;
// 	   OriginalVarSet.insert(element);
// 	 }
  
//   //cout<<"\nOriginal = "<<RHS->getDAGStr()<<endl;

//   RHS = Inlining(RHS, VarSet);

//   //cout<<"\nInlined = "<<RHS->getDAGStr()<<endl;

//   VarSet.clear();
//   RHS->getDAGNames(VarSet);
  				
//   for(set<string>::iterator VarSet_it = VarSet.begin(); VarSet_it != VarSet.end(); VarSet_it++)

// 	 {
// 	   string element = *(VarSet_it);

// 	   //cout<<"\nelement = "<<element<<endl;
	   
// 	   if(OriginalVarSet.find(element) != OriginalVarSet.end())
// 	     {
// 	       VarSet2.insert(element);				
// 	     }
// 	 }

//   if(VarSet2.empty())
//     return RHS; // all variables removed by inlining
//   else
//     return MANAGER.createDAG("exists", getDAGFromStringSet(VarSet2), RHS);  
  
// }

// // This function goes into the tree pointed by me.
// // It looks for exists nodes. If found, tries to inline it, otherwise returns the node as such
// // Ultimately returns a new tree (with all fresh nodes) which is a copy of me with exists nodes replaced with inlined ones
// // If you are calling this function with me = SE with no parent then parent = NULL

// CDAGNode* CSymbSimulator::cloneAndShiftWithInlining(CDAGNode* parent, CDAGNode *me)
// 	{
	  
// 	 CDAGNode* clone = new CDAGNode();

// 	 clone->name = me->name;
// 	 clone->delta_time = me->delta_time;
				
// 	 list<CDAGNode*>::iterator iout = me->out_list.begin();
		
// 	 for(; iout != me->out_list.end(); iout++)
// 		{
// 		  CDAGNode*  child = cloneAndShiftWithInlining(clone, (*iout));
// 		  clone->out_list.push_back(child);
// 		}

// 	 // clone is ready here
	
// 	if(clone->name == "exists")
// 	    {
// 	      clone = callInlining(clone);
// 	    } 
	
// 	if(parent != NULL)
// 		{
// 			clone->in_list.push_back(parent);
// 		}
	
// 		return clone;	
// 	}



// CDAGNode* CSymbSimulator::completeInlining(CDAGNode* parent, CDAGNode *me)
// 	{
// 	  //cout<<"NAME = "<<me->name<<endl;

// 	 list<CDAGNode*>::iterator iout = me->out_list.begin();
// 	 while(iout != me->out_list.end())
// 		{
// 		   cout<<"*iout = "<<(*iout)<<",It is = "<<(*iout)->getDAGStr()<<endl<<endl;
  
// 		  CDAGNode*  child = completeInlining(me, (*iout));
		 		    
// 		  if((*iout)->name == "exists")
// 		    {
// 		      cout<<"child = "<<child<<",It is = "<<child->getDAGStr()<<endl;

// 		      cout<<"*iout = "<<(*iout)<<",It is = "<<(*iout)->getDAGStr()<<endl<<endl;
// 		      iout = me->out_list.erase(iout);

// 		      cout<<"*iout = "<<(*iout)<<",It is = "<<(*iout)->getDAGStr()<<endl<<endl;
// 		      me->out_list.insert(iout,child);

// 		      cout<<"*iout = "<<(*iout)<<",It is = "<<(*iout)->getDAGStr()<<endl<<endl;
		      
// 		    }
// 		  iout++;
// 		}

// 	if(me->name == "exists")
// 	    {
// 	      CDAGNode* InlinedMe = callInlining(me);

// 	      cout<<"InlinedMe = "<<InlinedMe->getDAGStr()<<endl;

// 	      if(parent != NULL)
// 		{
// 			InlinedMe->in_list.push_back(parent);
			
// 		}
// 	      return InlinedMe;
// 	    }
// 	else
// 	  return me;
	  
// 	}




// // Here x should be exists(comma(...comma(x,y)...),SE)
// // This takes the list to be quantified into a set S and then calls scopeReduction(S,SE)
// // Ultimately returns a new tree (with all fresh nodes) which is a scope reduced version of the parameter dag
				    
// CDAGNode* CSymbSimulator::callScopeReduction(CDAGNode *dag)
// {
//   list<CDAGNode*>::iterator iout = dag->out_list.begin();
//   set<string> VarSet, VarSet2;
//   set<string>::iterator VarSet_it;
//   (*iout)->getDAGNames(VarSet);
//   iout++;
//   CDAGNode *RHS = (*iout);

//   cout<<"\n\nVarSet\n\n";

//   for(VarSet_it = VarSet.begin(); VarSet_it != VarSet.end(); VarSet_it++)
//     cout<<"\t"<<(*VarSet_it);

//   cout<<"\n\nRHS = "<<RHS->getDAGStr()<<endl<<endl;
   
//   CDAGNode* SreducedDag = scopeReduction(VarSet,RHS);

//   cout<<"\n\nOutput of Scope Reducer = "<<SreducedDag->getDAGStr()<<endl<<endl;
   
//   return SreducedDag;  
// }




// // This function goes into the tree pointed by me.
// // It looks for exists nodes. If found, tries to scope reduce it, otherwise returns the node as such
// // Ultimately returns a new tree (with all fresh nodes) which is a copy of me with exists nodes replaced with scope reduced ones
// // If you are calling this function with me = SE with no parent then parent = NULL
// CDAGNode* CSymbSimulator::cloneAndShiftWithScopeReduction(CDAGNode* parent, CDAGNode *me)
// 	{
// 	  CDAGNode* clone = new CDAGNode();

// 	  clone->name = me->name;
// 	  clone->delta_time = me->delta_time;
				
// 	  list<CDAGNode*>::iterator iout = me->out_list.begin();
		
// 	  for(; iout != me->out_list.end(); iout++)
// 		{
// 		  CDAGNode*  child = cloneAndShiftWithScopeReduction(clone, (*iout));
// 		  clone->out_list.push_back(child);
// 		}
	  
// 	  // clone is ready here
	
// 	if(clone->name == "exists")
// 	    {
// 	      cout<<"\n\nInput to Scope Reducer = "<<clone->getDAGStr()<<endl<<endl;
// 	      clone = callScopeReduction(clone);
// 	      cout<<"\n\nOutput of Scope Reducer = "<<clone->getDAGStr()<<endl<<endl;
// 	    } 
	  
// 	  if(parent != NULL)
// 		{
// 		  clone->in_list.push_back(parent);
// 		}
	
// 	return clone;
// 	}

// // Note that dag is NOT deleted here

// CDAGNode* CSymbSimulator::simplifyExpression(CDAGNode *dag)
// {
//   Subexpressions.clear();

//  //  cout<<"\nBEFORE CONVERSION\n";
// //   cout<<"\nBEFORE CONVERSION\n";
// //   cout<<"\nBEFORE CONVERSION\n";
// //   cout<<"\nBEFORE CONVERSION\n";
// //   cout<<"\nBEFORE CONVERSION\n";
  

//   ExprSimpRoot = convertCDAGNodeToCExprSimpNode(dag);



//    cout<<"\nBEFORE GIVING TO EXPRESSION SIMPLIFIER = "<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;
// //   cout<<"\nBEFORE GIVING TO EXPRESSION SIMPLIFIER\n";
// //   cout<<"\nBEFORE GIVING TO EXPRESSION SIMPLIFIER\n";
// //   cout<<"\nBEFORE GIVING TO EXPRESSION SIMPLIFIER\n";
// //   cout<<"\nBEFORE GIVING TO EXPRESSION SIMPLIFIER\n";
  
  

//   ExpressionSimplifier();

//    cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER = "<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;
//    cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER\n";
//    cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER\n";
//    cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER\n";
//    cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER\n";
//    cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER\n";

//   CDAGNode *simplified_dag = convertCExprSimpNodeToCDAGNode(ExprSimpRoot);

//   cout<<"\nAFTER CONVERSION = "<<simplified_dag->getDAGStr()<<"\n";
//   cout<<"\nAFTER CONVERSION\n";
//   cout<<"\nAFTER CONVERSION\n";
//   cout<<"\nAFTER CONVERSION\n";
//   cout<<"\nAFTER CONVERSION\n";

//   Subexpressions.clear();

//   // Delete the DAG

//   deleteDAG(ExprSimpRoot); // Temporarily deleted


//   cout<<"\nAFTER DELETION\n";
//   cout<<"\nAFTER DELETION\n";
//   cout<<"\nAFTER DELETION\n";
//   cout<<"\nAFTER DELETION\n";
//   cout<<"\nAFTER DELETION\n";
 
//   return simplified_dag;
// }


// CDAGNode* CSymbSimulator::equivalentReduction(CDAGNode *dag, string OutputName)
// {
//   bool Same;
//   CDAGNode *inlined_dag, *sreduced_dag, *simplified_dag, *original_dag;
//   int size;
  
//   map<CDAGNode*,CDAGNode*> temp1;
//   set<string> temp2;

//   size = findSizeOfTreeAndPrintTreeInDAGForm(dag, "SE_Before_Simplification.dat", "Before Simplification", OutputName);
//   //cout<<"\nSize of the SE before simplification = "<<size<<endl;
  
//   /* OLD LOOP */

//   /* 
//   do
//     {
//       cout<<"\nLOOP STARTED\n";
     

//       original_dag = dag;
//       //cout<<"\nDAG = "<<dag->getDAGStr()<<endl;
      
      
      
//       inlined_dag = cloneAndShiftWithInlining(NULL, dag);
      
//       //cout<<"\nINLINED DAG = "<<inlined_dag->getDAGStr()<<endl;
      
      

//       //simplified_dag = simplifyExpression(inlined_dag);
//       //cout<<"\nSIMPLIFIED DAG = "<<simplified_dag->getDAGStr()<<endl;
    

//       //sreduced_dag = cloneAndShiftWithScopeReduction(NULL, simplified_dag); // Temporarily commented
//       //cout<<"\nSCOPE REDUCED DAG = "<<sreduced_dag->getDAGStr()<<endl;
      
      
//       sreduced_dag = inlined_dag;
      
      
//       dag = sreduced_dag;
//       Same = dag->equals(original_dag);

//     }while(!Same);

//   size = findSizeOfTreeAndPrintTreeInDAGForm(dag, "SE_After_Simplification.dot", "After Simplification", OutputName);
//   //cout<<"\nSize of the SE after simplification = "<<size<<endl;*/

//   /* MODIFIED LOOP */
//   /* THIS ALSO DOES NOT RESULT IN EXECUTION OF fit : BUT, THIS LOOKS REASONABLE */
  
//    do
//     {
//       cout<<"\nLOOP STARTED\n";
//       // cout<<"\nLOOP STARTED\n";
//       // cout<<"\nLOOP STARTED\n";
//       // cout<<"\nLOOP STARTED\n";

//       original_dag = MANAGER.cloneAndShift(dag, 0);

//       //findSizeOfTree(original_dag, "TreeSize.dat", "\nSIZE OF DAG = ");

     
//       cout<<"\nORIGINAL DAG = "<<original_dag->getDAGStr()<<endl;

//       temp1.clear();
     
//       inlined_dag = cloneAndShiftWithInlining(NULL, dag);



//       Same = dag->equals(inlined_dag);

//       if(Same)
// 	cout<<"\nInlined DAG = Original DAG\n";
//       else
// 	cout<<"\nNOT(Inlined DAG = Original DAG)\n";



//       //findSizeOfTree(inlined_dag, "TreeSize.dat", "SIZE OF INLINED DAG = ");

//       cout<<"\nINLINED DAG = "<<inlined_dag->getDAGStr()<<endl;

//       deleteTree(dag);

//       cout<<"\nINLINED\n";
//       //cout<<"\nINLINED\n";
//       //cout<<"\nINLINED\n";
//       //cout<<"\nINLINED\n";

//       simplified_dag = simplifyExpression(inlined_dag);

//       cout<<"\nSIMPLIFIED\n";
//       cout<<"\nSIMPLIFIED\n";
//       cout<<"\nSIMPLIFIED\n";
//       cout<<"\nSIMPLIFIED\n";


//       Same = inlined_dag->equals(simplified_dag);

//       if(Same)
// 	cout<<"\nSimplified DAG = Inlined DAG\n";
//       else
// 	cout<<"\nNOT(Simplified DAG = Inlined DAG)\n";



//       deleteTree(inlined_dag);

//       //findSizeOfTree(simplified_dag, "TreeSize.dat", "SIZE OF SIMPLIFIED DAG = ");

//       cout<<"\nSIMPLIFIED DAG = "<<simplified_dag->getDAGStr()<<endl;

//       cout<<"\nSIMPLIFIED\n";
//       //cout<<"\nSIMPLIFIED\n";
//       //cout<<"\nSIMPLIFIED\n";
//       //cout<<"\nSIMPLIFIED\n";

//       sreduced_dag = cloneAndShiftWithScopeReduction(NULL, simplified_dag); 

      
//       Same = simplified_dag->equals(sreduced_dag);

//       if(Same)
// 	cout<<"\nScope Reduced DAG = Simplified DAG\n";
//       else
// 	cout<<"\nNOT(Scope Reduced DAG = Simplified DAG)\n";




//       //findSizeOfTree(sreduced_dag, "TreeSize.dat", "SIZE OF SCOPE REDUCED DAG = ");

//       cout<<"\nSREDUCED DAG = "<<sreduced_dag->getDAGStr()<<endl;

//       deleteTree(simplified_dag);

//       cout<<"\nSREDUCED\n";
//       //cout<<"\nSREDUCED DAG = "<<sreduced_dag->getDAGStr()<<endl;
//       //cout<<"\nORIGINAL DAG = "<<original_dag->getDAGStr()<<endl;
      
//       Same = original_dag->equals(sreduced_dag);

//       if(Same)
// 	cout<<"\nOriginal DAG = Scope Reduced DAG\n";
//       else
// 	cout<<"\nNOT(Original DAG = Scope Reduced DAG)\n";



//       cout<<"\nCOMPARED\n";
//       //cout<<"\nCOMPARED\n";
//       //cout<<"\nCOMPARED\n";
//       //cout<<"\nCOMPARED\n";

//       dag = sreduced_dag;

//       deleteTree(original_dag);

//     }while(!Same);

//    cout<<"\nTHE FINAL DAG WITH NO ABSTRACTION WITH QUANTIFIERS AT MAXIMUM DEPTH\n*****************************************************\n\n";
//    //cout<<"\nEXITED\n";
//    //cout<<"\nEXITED\n";
//    //cout<<"\nEXITED\n";
//    cout<<dag->getDAGStr()<<endl<<endl;

//   size = findSizeOfTreeAndPrintTreeInDAGForm(dag, "SE_After_Simplification.dot", "After Simplification", OutputName);
//   //cout<<"\nSize of the SE after simplification = "<<size<<endl;
//   //cout<<"\nSize of the SE after simplification = "<<size<<endl;
//   //cout<<"\nSize of the SE after simplification = "<<size<<endl;
//   //cout<<"\nSize of the SE after simplification = "<<size<<endl;



//    /* MODIFIED LOOP : THERE IS SOME PROBLEM WITH completeInlining. DON'T WASTE MUCH TIME!!!THIS IS ALSO NOT A SOLUTION */
//   /*

//    do
//     {
//       cout<<"\nLOOP STARTED\n";
//       cout<<"\nLOOP STARTED\n";
//       cout<<"\nLOOP STARTED\n";
//       cout<<"\nLOOP STARTED\n";

//       original_dag = MANAGER.cloneAndShift(dag, 0);

//       temp1.clear();
     
//       inlined_dag = completeInlining(NULL, dag);

//       cout<<"\nINLINED\n";
//       cout<<"\nINLINED\n";
//       cout<<"\nINLINED\n";
//       cout<<"\nINLINED\n";

//       cout<<"\nORIGINAL DAG = "<<original_dag->getDAGStr()<<endl;

//       cout<<"\nINLINED DAG = "<<inlined_dag->getDAGStr()<<endl;
      

//       simplified_dag = simplifyExpression(inlined_dag);

//       cout<<"\nSIMPLIFIED\n";
//       cout<<"\nSIMPLIFIED\n";
//       cout<<"\nSIMPLIFIED\n";
//       cout<<"\nSIMPLIFIED\n";

//       cout<<"\nSIMPLIFIED DAG = "<<simplified_dag->getDAGStr()<<endl;

//       sreduced_dag = cloneAndShiftWithScopeReduction(NULL, simplified_dag); 

//       deleteTree(simplified_dag);

      
//       cout<<"\nSREDUCED DAG = "<<sreduced_dag->getDAGStr()<<endl;
//       cout<<"\nORIGINAL DAG = "<<original_dag->getDAGStr()<<endl;
      
//       Same = original_dag->equals(sreduced_dag);

//       cout<<"\nCOMPARED\n";
//       cout<<"\nCOMPARED\n";
//       cout<<"\nCOMPARED\n";
//       cout<<"\nCOMPARED\n";
//       cout<<"\nCOMPARED\n";
//       cout<<"\nCOMPARED\n";
//       cout<<"\nCOMPARED\n";
//       cout<<"\nCOMPARED\n";

//       dag = sreduced_dag;

//       deleteTree(original_dag);

//     }while(!Same);

//    cout<<"\nEXITED\n";
//    cout<<"\nEXITED\n";
//    cout<<"\nEXITED\n";
//    cout<<"\nEXITED\n";
//    cout<<"\nDAG = "<<dag->getDAGStr()<<endl;

//   size = findSizeOfTreeAndPrintTreeInDAGForm(dag, "SE_After_Simplification.dot", "Debug_After_Simplification.dot");
//   cout<<"\nSize of the SE after simplification = "<<size<<endl;
//   cout<<"\nSize of the SE after simplification = "<<size<<endl;
//   cout<<"\nSize of the SE after simplification = "<<size<<endl;
//   cout<<"\nSize of the SE after simplification = "<<size<<endl;*/

  

//   //findProfits(dag, OutputName); // Find the profits : commented on 10th March 2009. This was working fine for debounce_grp8.vhd on 10th March 2009. See Readme/readme.txt to see why we commented this

//   getAbstractedExpression(dag, OutputName); // Get the abstracted SE
 
//   return dag;
// }
	
	    
// int CSymbSimulator::findSizeOfTreeAndPrintTreeInDAGForm(CDAGNode* tree, string name, string Where, string OutputName)
// {
//   int size;

//   Subexpressions.clear();

//   ExprSimpRoot = convertCDAGNodeToCExprSimpNode(tree);

//   // if(name != "")
//   // {
//   //  create_entire_graph(name);// prints the current DAG in CExprRoot into file name
//   // }

//   size = findSizeOfDAG(ExprSimpRoot);

//   //FILE *fpDebugFile;

//   //if((fpDebugFile = fopen(DebugFile.c_str(),"a")) != NULL)

//   fprintf(fpDebugFile,"\n*******************************************************\n"); 
  
//   fprintf(fpDebugFile,"Output = %s\t%s", OutputName.c_str(), Where.c_str()); 

//   fprintf(fpDebugFile,"\nSize = %d\n",size); 

//   //fclose(fpDebugFile);

//   exQuantifierDepths(ExprSimpRoot);

//   Subexpressions.clear();

//   // Delete the DAG

//   deleteDAG(ExprSimpRoot); // Temporarily deleted
  
//   return size;
// }

// This function creats a new tree which is a copy of the tree rooted at relation with existential variables substituted as
// name --> entityname_instancename_name
// very NB : The function returns the same tree(not a copy) in case there are no intermediate variables 

// CDAGNode* CSymbSimulator::renameExVariablesInTree(CDAGNode *relation, string entity, string instance)
// {
//      set<string> OriginalVarSet, ModifiedVarSet;
//      relation->getExistsDAGNames(OriginalVarSet); // List of ex.quantified variables

//      if(OriginalVarSet.empty()) // No ex.quantified vars
//        return relation;
//      else
//        {
// 	 set<string>::iterator str_it, str_it2;
     
// 	 // New list of ex.quantified variables created
// 	 for(str_it=OriginalVarSet.begin(); str_it!=OriginalVarSet.end(); str_it++)
// 	   {
// 	     string element = (*str_it); // Important attach at the front; at the back we have the delta
// 	     if(entity == "")
// 	       element = instance+"__"+element;
// 	     else if(instance == "")
// 	       element = entity+"__"+element; 
// 	     else
// 	       element = entity + "__" + instance + "__" + element;
// 	     ModifiedVarSet.insert(element);
// 	   }

// 	 map<string, string> RenameMap;

// 	 for(str_it=OriginalVarSet.begin(), str_it2=ModifiedVarSet.begin(); str_it!=OriginalVarSet.end() ; str_it++, str_it2++)
// 	   {
// 	     RenameMap.insert(make_pair((*str_it),(*str_it2)));
// 	   }
     
// 	 CDAGNode* renamed_relation = cloneAndShiftWithRename(NULL, relation, RenameMap);

// 	 return renamed_relation;
     
//     }
// }
     
     
    
// CDAGNode* CSymbSimulator::cloneAndShiftWithRename(CDAGNode* parent, CDAGNode* me, map<string,string> RenameMap)
// 	{
	
// 		CDAGNode* clone = new CDAGNode();

// 		clone->name = me->name;

// 		clone->delta_time = me->delta_time;

// 		if(clone->delta_time != MIN_DELTA)
// 		{
// 			char str_delta_time[20];
			
// 			sprintf(str_delta_time, "%d", clone->delta_time);
			
// 			string id_delta = clone->name + "_" + str_delta_time;

// 			if(RenameMap.find(id_delta) != RenameMap.end())

// 			  {
// 			    string id_delta_new = RenameMap[id_delta];
			    
// 			    int index = id_delta_new.find_last_of("_");
			    
// 			    string name_new = id_delta_new.substr(0, index); 
			    
// 			    int delta_new = atoi(id_delta_new.substr(index+1).c_str()); 

// 			    clone->name = name_new;
			    
// 			    clone->delta_time = delta_new;
// 			  }
			
// 		}
		
// 		list<CDAGNode*>::iterator iout = me->out_list.begin();
		
// 		for(; iout != me->out_list.end(); iout++)
// 		{
// 			CDAGNode* child = cloneAndShiftWithRename(clone,(*iout), RenameMap);
// 			clone->out_list.push_back(child);
// 		}
		
// 		if(parent != NULL)
// 		{
// 			clone->in_list.push_back(parent);
// 		}
	
// 		return clone;	
// 	}

// void CSymbSimulator::exQuantifierDepths(CExprSimpNode *dag)
// {
//   map<string, vector<int> > DepthMap;
//   map<string, vector<int> >::iterator DepthMap_it;


//   //FILE *fpDebugFile;

//   //if((fpDebugFile = fopen(DebugFile.c_str(),"a")) != NULL)

//   //{
//     fprintf(fpDebugFile,"\nEx. quantified variables and their depths\n"); 
     
//     dag->getDepthMap(DepthMap,0);
    
//     int i;
    
//     // for(i=1,DepthMap_it = DepthMap.begin(); DepthMap_it != DepthMap.end(); DepthMap_it++,i++)
// //       {
// // 	fprintf(fpDebugFile,"\n%d)Variable = %s\tDepth = %d",i,(*DepthMap_it).first.c_str(),(*DepthMap_it).second);
// //       }

//      for(i=1,DepthMap_it = DepthMap.begin(); DepthMap_it != DepthMap.end(); DepthMap_it++,i++)
//        {
//  	fprintf(fpDebugFile,"\n%d)Variable = %s\tDepth = ",i,(*DepthMap_it).first.c_str());
	
// 	for(vector<int>::iterator depth_it = ((*DepthMap_it).second).begin(); depth_it != ((*DepthMap_it).second).end(); depth_it++)
// 	  {fprintf(fpDebugFile,"%d, ",(*depth_it));
// 	  }
// 	fprintf(fpDebugFile,"\n");

//        }

//      //  fclose(fpDebugFile);
//     //}
// }

// void CSymbSimulator::deleteDAG(struct CExprSimpNode* dag) 
// 	{
// 	  list<struct CExprSimpNode*>::iterator iout = dag->out_list.begin();

	 
// 	  while(1)
// 	    {
// 	      if(dag->out_list.empty()) break;
// 	      else deleteDAG(*(dag->out_list.begin()));
// 	    }

// 	  // cout<<"\n\ndeleteDAG("<<dag->name<<")1";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")1";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")1";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")1";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")1";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")1";
	  
// 	  list<struct CExprSimpNode*> dag_in_list =  dag->in_list;

// 	 //  cout<<"\n\ndeleteDAG("<<dag->name<<")2";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")2";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")2";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")2";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")2";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")2";
	  
	  
// 	  list<struct CExprSimpNode*>::iterator dag_in_list_it = dag_in_list.begin();

// 	 //  cout<<"\n\ndeleteDAG("<<dag->name<<")3";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")3";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")3";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")3";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")3";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")3";
	  

// 	  for(; dag_in_list_it != dag_in_list.end(); dag_in_list_it++) 
// 		      {
// 			// cout<<"\n\ndeleteDAG("<<dag->name<<")4";
// // 			cout<<"\n\ndeleteDAG("<<dag->name<<")4";
// // 			cout<<"\n\ndeleteDAG("<<dag->name<<")4";
// // 			cout<<"\n\ndeleteDAG("<<dag->name<<")4";
// // 			cout<<"\n\ndeleteDAG("<<dag->name<<")4";
// // 			cout<<"\n\ndeleteDAG("<<dag->name<<")4";
// 			if(!(((*dag_in_list_it)->out_list).empty()))
// 			  {
// 			    // cout<<"\n\ndeleteDAG("<<dag->name<<")5";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")5";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")5";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")5";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")5";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")5";
// 			  (*dag_in_list_it)->out_list.remove(dag);
// 			    // cout<<"\n\ndeleteDAG("<<dag->name<<")6";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")6";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")6";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")6";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")6";
// // 			    cout<<"\n\ndeleteDAG("<<dag->name<<")6";
// 			  }
// 		      }
	    
// 	 //  cout<<"\n\ndeleteDAG("<<dag->name<<")7";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")7";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")7";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")7";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")7";
// // 	  cout<<"\n\ndeleteDAG("<<dag->name<<")7";
	  

// 	    free(dag);
	  
// 	}  


// void CSymbSimulator::deleteTree(CDAGNode* tree) 
// 	{
// 	  list<CDAGNode*>::iterator iout = tree->out_list.begin();

// 	  for(; iout != tree->out_list.end(); iout++) // Go down
// 	  	      {
// 	  		deleteTree(*iout);
// 	  	      }

		      

// 	    free(tree);
	  
// 	    }  

// void CSymbSimulator::findSizeOfTree(CDAGNode* tree, string DebugFile, string message)
// {
//   int size = 0;

//   tree->findTreeSize(size);

//   FILE *fpDebugFile;

//   if((fpDebugFile = fopen(DebugFile.c_str(),"a")) != NULL)
//     {
//       fprintf(fpDebugFile,"\n");
//       fprintf(fpDebugFile,message.c_str());
//       fprintf(fpDebugFile,"%d",size);
//       fprintf(fpDebugFile," NODES");
//       // fprintf(fpDebugFile," NODES  = ");
//       //fprintf(fpDebugFile,"%d",size*sizeof(CDAGNode)); 
//       //fprintf(fpDebugFile," BYTES"); 
//     }

//   fclose(fpDebugFile); 
// }


// void CSymbSimulator::findProfits(CDAGNode *tree, string OutputName)
// {
//   list<CDAGNode*> ExistsList;
//   map<CDAGNode*,CDAGNode*> temp1;
//   set<string> temp2, VarSet;
//   string existQuant;
//   list<CDAGNode*>::iterator ExistsList_it;
//   CDAGNode *original, *cloned, *abstracted, *inlined, *original_abstracted, *simplified, *sreduced;
//   bool Same;
//   int i, size, AbstractableCount = 0,  NonAbstractableCount = 0,;

//   cout<<"\nFINDING PROFITS\n*****************\n";

//   fprintf(fpDebugFile,"\n\nPROFITS AFTER INTRODUCING ONE STEP ABSTRACTION\n###################################################\n"); 

//   fprintf(fpDebugFile,"Profit measures of introducing an abstraction at Exists({ex. variable1,...,ex. variablen}, R1 & R2) are\nDifference in depths and scopes of ex. variablei's where\ndepth = depth of the ex. variablei from the root of the dag\nscope = size of the dag on the RHS of the exists node to which this ex. variablei is attached\n#########################################################################\n "); 
  
//   tree->findExistsDAGs(ExistsList);

//   for(i=1,ExistsList_it = ExistsList.begin(); ExistsList_it != ExistsList.end(); ExistsList_it++,i++)
//     {
      
//       original = (*ExistsList_it);

//       cout<<"\nOriginal Address = "<<original<<endl;

//       if(andAbstractionFeasible(original))
// 	{
// 	   cout<<"\n\nDirect Abstraction Applicable Here"<<endl<<endl;

// 	  AbstractableCount++;

// 	  // Printing 

// 	  VarSet.clear();

// 	  list<CDAGNode*>::iterator original_left = (original->out_list).begin();

// 	  (*original_left)->getDAGNames(VarSet); // Getting the variable names from the exists part of original DAG
      
// 	  existQuant = getListString(VarSet);

// 	  fprintf(fpDebugFile,"\n\nAt Exists node %d with Exists List as %s\n",i,existQuant.c_str()); 

// 	  fprintf(fpDebugFile,"\n******************************************************************\n"); 

// 	  fprintf(fpDebugFile,"\n\nCurrent Depths and Scopes\n"); 


// 	  // Printing the profits

// 	  printProfits(original);
       

// 	  // Printing Over

// 	  cloned = MANAGER.cloneAndShift(original, 0);

// 	  temp1.clear();

// 	  cout<<"\nCloned = "<<cloned->getDAGStr()<<endl;

// 	  abstracted = abstractionOnAnd(cloned);  // duplicate should not be created here

// 	  //abstracted = cloned;

// 	  cout<<"\nAbstracted = "<<abstracted->getDAGStr()<<endl;

// 	  do
// 	    {
// 	      cout<<"\nLOOP STARTED\n";
	  
// 	      original_abstracted = MANAGER.cloneAndShift(abstracted, 0);

// 	      temp1.clear();

// 	      cout<<"\nORIGINAL  = "<<original_abstracted->getDAGStr()<<endl;

// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";


// 	      inlined = cloneAndShiftWithInlining(NULL, original_abstracted);

// 	      cout<<"\nINLINED = "<<inlined->getDAGStr()<<endl;

// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";

// 	      deleteTree(abstracted);

// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";

// 	      simplified = simplifyExpression(inlined);

// 	      deleteTree(inlined);

// 	      cout<<"\nSIMPLIFIED = "<<simplified->getDAGStr()<<endl;

// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";

	      
// 	      sreduced = cloneAndShiftWithScopeReduction(NULL, simplified); 

// 	      cout<<"\nSREDUCED = "<<sreduced->getDAGStr()<<endl;
	  
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";

	     
// 	      deleteTree(simplified);

// 	      Same = original_abstracted->equals(sreduced);

// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";

	    
      
// 	      abstracted = sreduced;

// 	      deleteTree(original_abstracted);

// 	    }while(!Same);
	
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";

// 	  cout<<"\nFINAL = "<<abstracted->getDAGStr()<<endl;

// 	  // Printing the profits

// 	  fprintf(fpDebugFile,"\n\nDepths and Scopes after Abstraction and Simplification\n"); 

// 	  printProfits(abstracted);
            
// 	  deleteTree(abstracted);
   
      
// 	}
//       else if(andAbstractionFeasibleModified(original))
// 	{

// 	  cout<<"\n\nModified Abstraction Applicable Here"<<endl<<endl;

// 	  // abstraction on and is possible, but not direct. don't print anything
// 	  AbstractableCount++;

// 	   // Printing 

// 	  VarSet.clear();

// 	  list<CDAGNode*>::iterator original_left = (original->out_list).begin();

// 	  (*original_left)->getDAGNames(VarSet); // Getting the variable names from the exists part of original DAG
      
// 	  existQuant = getListString(VarSet);

// 	  fprintf(fpDebugFile,"\n\nAt Exists node %d with Exists List as %s\n",i,existQuant.c_str()); 

// 	  fprintf(fpDebugFile,"\n******************************************************************\n"); 

// 	  fprintf(fpDebugFile,"\n\nCurrent Depths and Scopes\n"); 


// 	  // Printing the profits

// 	  printProfits(original);
       

// 	  // Printing Over

// 	  cloned = MANAGER.cloneAndShift(original, 0);

// 	  temp1.clear();

// 	  cout<<"\nCloned = "<<cloned->getDAGStr()<<endl;

// 	  abstracted = abstractionOnAndModified(cloned);  // duplicate should not be created here

// 	  //abstracted = cloned;

// 	  cout<<"\nAbstracted = "<<abstracted->getDAGStr()<<endl;

// 	  do
// 	    {
// 	      cout<<"\nLOOP STARTED\n";
	  
// 	      original_abstracted = MANAGER.cloneAndShift(abstracted, 0);

// 	      temp1.clear();

// 	      cout<<"\nORIGINAL  = "<<original_abstracted->getDAGStr()<<endl;

// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";


// 	      inlined = cloneAndShiftWithInlining(NULL, original_abstracted);

// 	      cout<<"\nINLINED = "<<inlined->getDAGStr()<<endl;

// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";

	    
// 	      deleteTree(abstracted);

// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";

// 	      simplified = simplifyExpression(inlined);

// 	      deleteTree(inlined);

// 	      cout<<"\nSIMPLIFIED = "<<simplified->getDAGStr()<<endl;

// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";

// 	      sreduced = cloneAndShiftWithScopeReduction(NULL, simplified); 

// 	      cout<<"\nSREDUCED = "<<sreduced->getDAGStr()<<endl;
	  
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";

	    
// 	      deleteTree(simplified);

// 	      Same = original_abstracted->equals(sreduced);

// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";

	        
// 	      abstracted = sreduced;

// 	      deleteTree(original_abstracted);

// 	    }while(!Same);
	
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";

// 	  cout<<"\nFINAL = "<<abstracted->getDAGStr()<<endl;

// 	  // Printing the profits

// 	  fprintf(fpDebugFile,"\n\nDepths and Scopes after Abstraction and Simplification\n"); 

// 	  printProfits(abstracted);
            
// 	  deleteTree(abstracted); 
   
	
// 	  }
	  
//       else 
// 	{
// 	  // abstraction on and is not possible..don't print anything
// 	  NonAbstractableCount++;
// 	}
	  
//     }

//   fprintf(fpDebugFile,"\n\n******************************************************************\n");

//   fprintf(fpDebugFile,"\nNo: of abstractable exists nodes = %d, No: of non abstractable exists nodes = %d\n", AbstractableCount, NonAbstractableCount); 

//   fprintf(fpDebugFile,"\n******************************************************************\n");
// }

// // Function to apply exists(L, R1/\R2) --> and(exists(L,R1),exists(L,R2)) where tree is exists(L, R1/\R2)
// CDAGNode* CSymbSimulator::abstractionOnAnd(CDAGNode *tree)
// {
//   map<CDAGNode*,CDAGNode*> temp1;
//   set<string> temp2;

//   list<CDAGNode*> out_list = tree->out_list;
//   list<CDAGNode*>::iterator out_list_it =  out_list.begin();

//   CDAGNode *ExistsPart = (*out_list_it);
//   CDAGNode *ExistsPartCopy  = MANAGER.cloneAndShift(ExistsPart, 0);

//   temp1.clear();

//   out_list_it++;
//   CDAGNode *TreePart = (*out_list_it);
   
//   if(TreePart->name == "and")
//     {
//       list<CDAGNode*> and_out_list = TreePart->out_list;
//       list<CDAGNode*>::iterator and_out_list_it =  and_out_list.begin();

//       CDAGNode *LHS = (*and_out_list_it);
//       and_out_list_it++;
//       CDAGNode *RHS = (*and_out_list_it);
      
//       return MANAGER.createDAG("and", MANAGER.createDAG("exists",ExistsPart,LHS), MANAGER.createDAG("exists",ExistsPartCopy,RHS));
//     }

//   else
//     {
//       return tree;
//     }
// }
  

 

// void CSymbSimulator::exQuantifierDepthsScopes(CExprSimpNode *dag)
// {
//   map<string, list< pair<int, int> > > DepthScopeMap;
//   map<string, list< pair<int, int> > >::iterator DepthScopeMap_it;


//   //FILE *fpDebugFile;

//   //if((fpDebugFile = fopen(DebugFile.c_str(),"a")) != NULL)

//   //{
//   //  fprintf(fpDebugFile,"\nEx. quantified variables, their depths and scopes\n"); 
     
//     dag->getDepthScopeMap(DepthScopeMap,0, NULL);
    
//     int i;
    
//     // for(i=1,DepthMap_it = DepthMap.begin(); DepthMap_it != DepthMap.end(); DepthMap_it++,i++)
// //       {
// // 	fprintf(fpDebugFile,"\n%d)Variable = %s\tDepth = %d",i,(*DepthMap_it).first.c_str(),(*DepthMap_it).second);
// //       }

//      for(i=1,DepthScopeMap_it = DepthScopeMap.begin(); DepthScopeMap_it != DepthScopeMap.end(); DepthScopeMap_it++,i++)
//        {
//  	fprintf(fpDebugFile,"\n%d)Variable = %s\t",i,(*DepthScopeMap_it).first.c_str());
	
// 	for(list< pair<int, int> >::iterator depthscope_it = ((*DepthScopeMap_it).second).begin(); depthscope_it != ((*DepthScopeMap_it).second).end(); depthscope_it++)
// 	  {fprintf(fpDebugFile,"at depth %d with scope %d  ",(*depthscope_it).first, (*depthscope_it).second);
// 	  }
// 	fprintf(fpDebugFile,"\n");

//        }

//      //  fclose(fpDebugFile);
//     //}
// }


// void CSymbSimulator::exQuantifierTotalScopes(CExprSimpNode *dag)
// {
//   map<string, int> TotalScopeMap;
//   map<string, int>::iterator TotalScopeMap_it;


//     dag->getTotalScopeMap(TotalScopeMap);
    
//     int i;
    
//     for(i=1,TotalScopeMap_it = TotalScopeMap.begin(); TotalScopeMap_it != TotalScopeMap.end(); TotalScopeMap_it++,i++)
//        {
//  	fprintf(fpDebugFile,"\n%d)Variable = %s, Total Scope (Total DAG Size of all DAGs Ris attached to all occurances of Exists({...,ex.variablei,...}, Ri) = %d\n",i,(*TotalScopeMap_it).first.c_str(), (*TotalScopeMap_it).second);
// 	}

   
// }


// void CSymbSimulator::printProfits(CDAGNode* tree)
// {
//   Subexpressions.clear();

//   ExprSimpRoot = convertCDAGNodeToCExprSimpNode(tree);

//   fprintf(fpDebugFile,"\nIndividual Depths and Scopes\n");

//   exQuantifierDepthsScopes(ExprSimpRoot); // Print the depths and scopes

//   fprintf(fpDebugFile,"\nTotal Scopes\n");

//   exQuantifierTotalScopes(ExprSimpRoot); // Print the total scopes

//   Subexpressions.clear();

//   deleteDAG(ExprSimpRoot);
 
// }


// bool CSymbSimulator::andAbstractionFeasible(CDAGNode* tree)
// {
//   cout<<"\nChecking if AND abstraction is feasible\n";
//    list<CDAGNode*>::iterator children_it = (tree->out_list).begin();

//    children_it++;

//    if((*children_it)->name == "and")
//      {
//      cout<<"\nRight child is and\n";
//      return true;
//      }
//    else
//      {
//        cout<<"\nRight child is "<<(*children_it)->name<<"\n";
//      return false;
//      }
// }


// CDAGNode* CSymbSimulator::splitPresentExists(CDAGNode* exp, set<string> &setExQuantifyTemp)
// {
//   if(exp->name != "exists")
//     return exp;
//   else
//     {
//       list<CDAGNode*>::iterator children_it = (exp->out_list).begin();
//       CDAGNode* NameList = (*children_it);

//       set<string> NameSet;
//       set<string>::iterator NameSet_it;

//       NameList->getDAGNames(NameSet);

//       for(NameSet_it = NameSet.begin(); NameSet_it != NameSet.end(); NameSet_it++)
// 	setExQuantifyTemp.insert((*NameSet_it));
      
//       children_it++;

//       return (*children_it);
//     }
      
// }

// bool CSymbSimulator::andAbstractionFeasibleModified(CDAGNode* tree)
// {
//   cout<<"\nChecking if AND abstraction is feasible\n";
//    list<CDAGNode*>::iterator children_it = (tree->out_list).begin();

//    children_it++;

//    if((*children_it)->name == "and")
//      {
//       cout<<"\nRight child is "<<(*children_it)->name<<"\n";
//      return true;
//      }
//    else
//      {
     
//        if((*children_it)->name == "exists")
// 	 {
// 	   return andAbstractionFeasibleModified((*children_it));
// 	 }
//        else
// 	 {
// 	   cout<<"\nRight child is "<<(*children_it)->name<<"\n";
// 	   return false;
// 	 }
//      }
// }

// CDAGNode* CSymbSimulator::findAndAbstractionFeasibleNode(CDAGNode* tree)
// {
//   //cout<<"\nChecking if AND abstraction is feasible\n";
//    list<CDAGNode*>::iterator children_it = (tree->out_list).begin();

//    children_it++;

//    if((*children_it)->name == "and")
//      {
//       cout<<"\nRight child is "<<(*children_it)->name<<"\n";
//      return (*children_it);
//      }
//    else
//      {
     
//        if((*children_it)->name == "exists")
// 	 {
// 	   return findAndAbstractionFeasibleNode(*children_it);
// 	 }
//        else
// 	 {
// 	   cout<<"\nERROR!"<<(*children_it)->name<<" which is a node other than and encountered in abstractionOnAndWithExistsPart\n";
// 	   exit(1);
	  
// 	 }
//      }
// }


// // Function to apply exists(L, R1/\R2) --> and(exists(L,R1),exists(L,R2)) where TreePart is R1/\R2 and ExistsPart is L
// CDAGNode* CSymbSimulator::abstractionOnAndWithExistsPart(CDAGNode *TreePart, CDAGNode *ExistsPart)
// {
//   map<CDAGNode*,CDAGNode*> temp1;
//   set<string> temp2;

//   CDAGNode *ExistsPartCopy  = MANAGER.cloneAndShift(ExistsPart, 0);

//   temp1.clear();

//   if(TreePart->name == "and")
//     {
//       list<CDAGNode*> and_out_list = TreePart->out_list;
//       list<CDAGNode*>::iterator and_out_list_it =  and_out_list.begin();

//       CDAGNode *LHS = (*and_out_list_it);
//       and_out_list_it++;
//       CDAGNode *RHS = (*and_out_list_it);
      
//       return MANAGER.createDAG("and", MANAGER.createDAG("exists",ExistsPart,LHS), MANAGER.createDAG("exists",ExistsPartCopy,RHS));
//     }

//   else
//     {
//       cout<<"\nERROR!"<<TreePart->name<<" which is a node other than and encountered in abstractionOnAndWithExistsPart\n";
//       exit(1);
//     }
// }
  

// // Function to apply exists(L1, exists(L2, R1/\R2)) --> exists(L2,and(exists(L1,R1),exists(L1,R2))) where tree is exists(L1, exists(L2, R1/\R2))
// CDAGNode* CSymbSimulator::abstractionOnAndModified(CDAGNode *tree)
// {

//   CDAGNode* AbstractionApplicableNode = findAndAbstractionFeasibleNode(tree); // find the AND node where abstraction is applicable
//   // Get the ExistsPart

//   cout<<"\n\nAbstractionApplicableNode = "<<AbstractionApplicableNode<<endl;

//   list<CDAGNode*> out_list = tree->out_list;
//   list<CDAGNode*>::iterator out_list_it =  out_list.begin();
//   CDAGNode *ExistsPart = (*out_list_it);

//   cout<<"\n\nExistsPart = "<<ExistsPart<<endl;

//   CDAGNode *AfterAbstractionNode = abstractionOnAndWithExistsPart(AbstractionApplicableNode, ExistsPart); // AfterAbstractionNode is node after applying abstraction at AbstractionApplicableNode with ExistsPart pushed in

//   return AfterAbstractionNode;
// }


// void CSymbSimulator::getAbstractedExpression(CDAGNode *tree, string OutputName) // Get the abstracted SE
// {
//   list<ExistsNodesWithProfits*> ExistsList;
//   list<ExistsNodesWithProfits*>::iterator ExistsList_it;

//   map<CDAGNode*,CDAGNode*> temp1;
//   set<string> temp2;
  
 
//   CDAGNode *original, *cloned, *abstracted, *inlined, *original_abstracted, *simplified, *sreduced;
//   bool Same;
//   int i, size, AbstractableCount = 0,  NonAbstractableCount = 0,;

//   tree->findExistsDAGsWithDepths(ExistsList,0); // Find the addresses of the exists nodes and their depths
//   // Note that we are finding here the tree depths; not dag depths

//   for(i=1,ExistsList_it = ExistsList.begin(); ExistsList_it != ExistsList.end(); ExistsList_it++,i++)
//     {
      
//       original = (*ExistsList_it)->Address;

//       cout<<"\nOriginal Address = "<<original<<endl;

//       cout<<"\nOriginal = "<<original->getDAGStr()<<endl;

//       if(andAbstractionFeasible(original))
// 	{
// 	   cout<<"\n\nDirect Abstraction Applicable Here"<<endl<<endl;

// 	  AbstractableCount++;

// 	  // Get the current depths of the exists nodes inside the subtree rooted at "original". By depths, we mean dag depths. For this, we obtain the dag for original and get depths of the exists nodes in it as a set

// 	  list<int> depths;

// 	  list<int>::iterator depths_it;

// 	  // Insert this into PreAbstractionDepths of the current exists node

// 	  getDepthsOfExitsNodesBeforeAbstraction(original, depths);

// 	  cout<<"\nDEPTHS OF EXISTS NODES INSIDE "<<original->getDAGStr()<<"\n";

// 	  for(depths_it = depths.begin(); depths_it != depths.end();  depths_it++)
// 	    {
// 	      cout<<"\t"<<(*depths_it);
// 	    }


// 	  ((*ExistsList_it)->PreAbstractionDepths).splice(((*ExistsList_it)->PreAbstractionDepths).begin(), depths);

// 	  // Main abstraction and equivalent reduction part starts here

// 	  cloned = MANAGER.cloneAndShift(original, 0);

// 	  temp1.clear();

// 	  cout<<"\nCloned = "<<cloned->getDAGStr()<<endl;

// 	  abstracted = abstractionOnAnd(cloned);  // duplicate should not be created here

// 	  //abstracted = cloned;

// 	  cout<<"\nAbstracted = "<<abstracted->getDAGStr()<<endl;

// 	  do
// 	    {
// 	      cout<<"\nLOOP STARTED\n";
	  
// 	      original_abstracted = MANAGER.cloneAndShift(abstracted, 0);

// 	      temp1.clear();

// 	      cout<<"\nORIGINAL  = "<<original_abstracted->getDAGStr()<<endl;

// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";


// 	      inlined = cloneAndShiftWithInlining(NULL, original_abstracted);

// 	      cout<<"\nINLINED = "<<inlined->getDAGStr()<<endl;

// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";

// 	      deleteTree(abstracted);

// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";

// 	      simplified = simplifyExpression(inlined);

// 	      deleteTree(inlined);

// 	      cout<<"\nSIMPLIFIED = "<<simplified->getDAGStr()<<endl;

// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";

	      
// 	      sreduced = cloneAndShiftWithScopeReduction(NULL, simplified); 

// 	      cout<<"\nSREDUCED = "<<sreduced->getDAGStr()<<endl;
	  
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";

	     
// 	      deleteTree(simplified);

// 	      Same = original_abstracted->equals(sreduced);

// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";

	    
      
// 	      abstracted = sreduced;

// 	      deleteTree(original_abstracted);

// 	    }while(!Same);
	
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";

// 	  cout<<"\nFINAL = "<<abstracted->getDAGStr()<<endl;

// 	  // Get the current depths of the exists nodes inside the subtree rooted at "abstracted". By depths, we mean dag depths. For this, we obtain the dag for abstracted and get depths of the exists nodes in it as a set

// 	  //set<int> depths; already declared earlier

// 	  depths.clear();

// 	  getDepthsOfExitsNodes(abstracted, depths);

// 	  cout<<"\nDEPTHS OF EXISTS NODES INSIDE "<<abstracted->getDAGStr()<<"\n";

// 	  for(depths_it = depths.begin(); depths_it != depths.end();  depths_it++)
// 	    {
// 	      cout<<"\t"<<(*depths_it);
// 	    }
	  
// 	  // Insert this into PostAbstractionDepths of the current exists node

// 	  ((*ExistsList_it)->PostAbstractionDepths).splice(((*ExistsList_it)->PostAbstractionDepths).begin(), depths);

	    
// 	  deleteTree(abstracted);

// 	  cout<<"\nEXISTS NODE IS\n";

// 	  (*ExistsList_it)->displayExistsNodesWithProfits();
      
// 	}
//       else if(andAbstractionFeasibleModified(original))
// 	{

// 	  cout<<"\n\nModified Abstraction Applicable Here"<<endl<<endl;

// 	  // abstraction on and is possible, but not direct. don't print anything
// 	  AbstractableCount++;

// 	  // Get the current depths of the exists nodes inside the subtree rooted at "original". By depths, we mean dag depths. For this, we obtain the dag for original and get depths of the exists nodes in it as a set

// 	  list<int> depths;

// 	  list<int>::iterator depths_it;

// 	  // Insert this into PreAbstractionDepths of the current exists node

// 	  getDepthsOfExitsNodesBeforeAbstraction(original, depths);

// 	  cout<<"\nDEPTHS OF EXISTS NODES INSIDE "<<original->getDAGStr()<<"\n";

// 	  for(depths_it = depths.begin(); depths_it != depths.end();  depths_it++)
// 	    {
// 	      cout<<"\t"<<(*depths_it);
// 	    }

	  
// 	  ((*ExistsList_it)->PreAbstractionDepths).splice(((*ExistsList_it)->PreAbstractionDepths).begin(), depths);


// 	  // Main abstraction and equivalent reduction part starts here
	  

// 	  cloned = MANAGER.cloneAndShift(original, 0);

// 	  temp1.clear();

// 	  cout<<"\nCloned = "<<cloned->getDAGStr()<<endl;

// 	  abstracted = abstractionOnAndModified(cloned);  // duplicate should not be created here

// 	  //abstracted = cloned;

// 	  cout<<"\nAbstracted = "<<abstracted->getDAGStr()<<endl;

// 	  do
// 	    {
// 	      cout<<"\nLOOP STARTED\n";
	  
// 	      original_abstracted = MANAGER.cloneAndShift(abstracted, 0);

// 	      temp1.clear();

// 	      cout<<"\nORIGINAL  = "<<original_abstracted->getDAGStr()<<endl;

// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";
// 	      cout<<"\nCALLING INLINING\n";


// 	      inlined = cloneAndShiftWithInlining(NULL, original_abstracted);

// 	      cout<<"\nINLINED = "<<inlined->getDAGStr()<<endl;

// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";
// 	      cout<<"\nINLINED\n";

	    
// 	      deleteTree(abstracted);

// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";
// 	      cout<<"\nDELETED\n";

// 	      simplified = simplifyExpression(inlined);

// 	      deleteTree(inlined);

// 	      cout<<"\nSIMPLIFIED = "<<simplified->getDAGStr()<<endl;

// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";
// 	      cout<<"\nSIMPLIFIED\n";

// 	      sreduced = cloneAndShiftWithScopeReduction(NULL, simplified); 

// 	      cout<<"\nSREDUCED = "<<sreduced->getDAGStr()<<endl;
	  
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";
// 	      cout<<"\nSREDUCED\n";

	    
// 	      deleteTree(simplified);

// 	      Same = original_abstracted->equals(sreduced);

// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";
// 	      cout<<"\nSame = "<<Same<<"\n";

	        
// 	      abstracted = sreduced;

// 	      deleteTree(original_abstracted);

// 	    }while(!Same);
	
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";
// 	  cout<<"\nBefore Final\n";

// 	  cout<<"\nFINAL = "<<abstracted->getDAGStr()<<endl;

// 	  // Get the current depths of the exists nodes inside the subtree rooted at "abstracted". By depths, we mean dag depths. For this, we obtain the dag for abstracted and get depths of the exists nodes in it as a set

// 	  //set<int> depths;

// 	  depths.clear();

// 	  getDepthsOfExitsNodes(abstracted, depths);

// 	  cout<<"\nDEPTHS OF EXISTS NODES INSIDE "<<abstracted->getDAGStr()<<"\n";

// 	  for(depths_it = depths.begin(); depths_it != depths.end();  depths_it++)
// 	    {
// 	      cout<<"\t"<<(*depths_it);
// 	    }
	  
// 	  // Insert this into PostAbstractionDepths of the current exists node

// 	  ((*ExistsList_it)->PostAbstractionDepths).splice(((*ExistsList_it)->PostAbstractionDepths).begin(), depths);
     
// 	  deleteTree(abstracted); 

// 	  cout<<"\nEXISTS NODE IS\n";

// 	  (*ExistsList_it)->displayExistsNodesWithProfits();
   	
// 	  }
	  
//       else 
// 	{
// 	  // abstraction on and is not possible..don't print anything
// 	  NonAbstractableCount++;
// 	}
	  
//     }

//   cout<<"\n\n******************************************************************\n";

//   cout<<"\nNo: of abstractable exists nodes = "<<AbstractableCount<<", No: of non abstractable exists nodes = "<<NonAbstractableCount<<endl; 
//   cout<<"\n\n******************************************************************\n";

//   for(i=0,ExistsList_it = ExistsList.begin(); ExistsList_it != ExistsList.end(); ExistsList_it++,i++)
//     {
//       cout<<"\nExists Node "<<i+1<<"\n";
//       (*ExistsList_it)->displayExistsNodesWithProfits();
//     } 

// }

// void CSymbSimulator::getDepthsOfExitsNodes(CDAGNode *tree, list<int> &depths)
// {
 
//   Subexpressions.clear();

//   ExprSimpRoot = convertCDAGNodeToCExprSimpNode(tree);

//   ExprSimpRoot->getDepthsOfExistsNodes(depths, 0); // Get the depths of the exists nodes as a set

//   Subexpressions.clear();

//   deleteDAG(ExprSimpRoot);
 
// }


// void CSymbSimulator::getDepthsOfExitsNodesBeforeAbstraction(CDAGNode *tree, list<int> &depths)
// {
//   // It can happen that tree is like exists(a, exists(b, exists(c, R1/\R2))) 
//   // We may want to compute depths od exists in tree for finding profit of applying abstraction at exists(a,...)
//   // Clearly we need not include b, c here as exists(a, exists(b, exists(c, R1/\R2))) = exists(b, exists(c, exists(a, R1/\R2))) 
//   // Depths here is 0 (for a) + depths from R1/\R2 which is the body
  


//   CDAGNode *body = findAndAbstractionFeasibleNode(tree); 
    
//   Subexpressions.clear();

//   depths.push_back(0); // As the root is exists(var, ....)

//   ExprSimpRoot = convertCDAGNodeToCExprSimpNode(body);

//   ExprSimpRoot->getDepthsOfExistsNodes(depths, 0); // Get the depths of the exists nodes as a set

//   Subexpressions.clear();

//   deleteDAG(ExprSimpRoot);
 
// }


bool CSymbSimulator::checkIfTriggeringConditionIsTrue(CBasicBlock *bb)
{
  list<node*> block_nodes = bb->block_nodes;
  list<node*>::iterator block_nodes_it = block_nodes.begin(); // First node of BB taken
  
  node *First = (*block_nodes_it);
  CArchStmt * Arch_stmt = First->arch_stmt;
  int NodeType = Arch_stmt->type;
  

  if(NodeType == 2) // Component Instantiation
    return true;
  else if(NodeType == 1) // Process statement
    {
      // Get the sensitivity list of the process statement
      CProcess* ProcessStmt = Arch_stmt->stmt.process_stmt;
      list<COperand*> SensitivityList = ProcessStmt->getSenList();

      // Get the variables referred by the process statement
      CSeqStmt *stmt_ptr = ProcessStmt->getFirst();
      list<COperand*> InputList;
      InputList.clear();
      if(stmt_ptr != NULL)
		{
			stmt_ptr->getChangedRHS(InputList);
		}
      if(listEqual(InputList,SensitivityList)) // combinational process statement
	return true;
      else
	return false;
    }
  else if(NodeType == 0) // Concurrent signal assignment statement : They have any way sensitivity list = list of inputs. Hence trigerring condition is true
    return true; 
}
  
  
bool CSymbSimulator::listEqual(list<COperand*> InputList, list<COperand*> SensitivityList)
{
  set<COperand*> SetDiffer;
  string Name1, Name2, Upper1, Upper2, Lower1, Lower2;
  bool match_found;
  list<COperand*>::iterator InputList_it;
  list<COperand*>::iterator SensitivityList_it;


  for(InputList_it=InputList.begin(); InputList_it!=InputList.end(); InputList_it++)
    {
      ((CAtom*)(*InputList_it))->print();

      Name1 = ((CAtom*)(*InputList_it))->getName();
      Upper1 = ((CAtom*)(*InputList_it))->getUpper();
      Lower1 = ((CAtom*)(*InputList_it))->getLower();

      //cout<<"\n\nName1 = "<<Name1<<", Upper1 = "<<Upper1<<", Lower1 = "<<Lower1<<endl<<endl;

      match_found = false;

      for(SensitivityList_it=SensitivityList.begin(); SensitivityList_it!=SensitivityList.end(); SensitivityList_it++)
	{
	   ((CAtom*)(*SensitivityList_it))->print();
	   Name2 = ((CAtom*)(*SensitivityList_it))->getName();
	   Upper2 = ((CAtom*)(*SensitivityList_it))->getUpper();
	   Lower2 = ((CAtom*)(*SensitivityList_it))->getLower();

	   //cout<<"\n\nName2 = "<<Name2<<", Upper2 = "<<Upper2<<", Lower2 = "<<Lower2<<endl<<endl;

	   if(Name1==Name2 && Upper1==Upper2 && Lower1==Lower2)
	     {match_found=true; break;}
	}
      if(match_found == false)
	SetDiffer.insert(*InputList_it);
    }

  
  if(SetDiffer.empty())
    return true;
  else
    return false;
}


/*Type CSymbSimulator::getVariableType(string VarName, string EntName)
{
  
	map <string, map<string, SymTabEntry> >::iterator ist;
	
	for (ist=SymbolTable.begin(); ist != SymbolTable.end(); ++ist) 
	{
		cout<<endl;
		cout << "***********************************"<< endl;
        	cout << "Name of Entity: "<< ist->first << endl;
		cout << "************************************"<< endl;

		map<string, SymTabEntry>::iterator ien;
		for (ien = (ist->second).begin(); ien != (ist->second).end(); ++ien) 
		{
			cout << "Name: "<< ien->first << endl;
			cout << "ID type: " << ien->second.id_type<<endl;
			cout << "Data type: " << ien->second.data_type<<endl;
			string value = ien->second.value==NULL?"":ien->second.value->getOrgExpr();
			cout << "Value: " << value <<endl;
			cout << "Range: " << endl;
			list<string>::iterator ir;
			for (ir = (ien->second).rangeindex.begin(); ir != (ien->second).rangeindex.end(); ++ir) 
			{
				cout<<" " << (*ir);
			}
			cout <<endl<< "-----------------------------------"<< endl;
		}

		cout << "***********************************"<< endl;
	}
}*/

// Replacing internal variables in the instance by entityname_instancename_variablename

void CSymbSimulator::showSet(set<string> me, string WhoAmI)
{
//cout<<"\n"<<WhoAmI<<"\n*****\n\n";
set<string>::iterator me_it;

for(me_it=me.begin(); me_it!=me.end(); me_it++)
	{
	//cout<<(*me_it)<<"\n";
	}
}

CDAGNode* CSymbSimulator::createCloneWithInternalVarsRenamed(CDAGNode *relation, string EntName, CInstance * instance_stmt) 
{
  string InstName=instance_stmt->getInstName();

  // First find the internal variables
  // Internal variables = All variables in relation - variables corresponding to formal arguments
  // First get the formal arguments
  
  set<string> Formal_Arg_Names, Variables, OriginalInternalVarSet, ModifiedInternalVarSet;

  list<COperand*> formals= (instance_stmt)->getFormalParam();
  if(formals.size() != 0)
    {
      //cout<<"\nNOTE: formals.size() != 0\n";
      for(list<COperand*> ::iterator iformal = formals.begin(); iformal != formals.end(); iformal++)
	Formal_Arg_Names.insert((*iformal)->getName());
    }
  else
    { 
      //cout<<"\nNOTE: formals.size() == 0\n";
      int index = instance_stmt->getIndex();
      CEntity &comp = Entities[index];
		
      list<CPort>& ports = comp.getPorts();
      for(list<CPort>::iterator iport = ports.begin();iport != ports.end();  iport++)
	Formal_Arg_Names.insert(iport->getName());
    }
  
  //showSet(Formal_Arg_Names, "Formal_Arg_Names");
  
  relation->getDAGNames(Variables); // Set of all variables in relation
  
  // Create Internal variables = All variables in relation - variables corresponding to formal arguments

  set<string>::iterator it, it2;
  
  for(it=Variables.begin(); it!=Variables.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index); 
      
      if(Formal_Arg_Names.find(id) == Formal_Arg_Names.end()) // id is not a formal argument
	{
	  OriginalInternalVarSet.insert(id_delta);
	}
    }

  //showSet(OriginalInternalVarSet, "OriginalInternalVarSet");

   // Create new names for internal variables

   for(it=OriginalInternalVarSet.begin(); it!=OriginalInternalVarSet.end(); it++)
	   {
	     string element = (*it); // Important : Attach at the front; at the back we have the delta
	     if(EntName == "")
	       element = InstName+"__"+element;
	     else if(InstName == "")
	       element = EntName+"__"+element; 
	     else
	       element = EntName + "__" + InstName + "__" + element;
	     ModifiedInternalVarSet.insert(element);
	   }

   //showSet(ModifiedInternalVarSet, "ModifiedInternalVarSet");

   map<string, string> RenameMap;

   for(it=OriginalInternalVarSet.begin(), it2=ModifiedInternalVarSet.begin(); it!=OriginalInternalVarSet.end() ; it++, it2++)
	   {
	     RenameMap.insert(make_pair((*it),(*it2)));
	   }
     

   CDAGNode* renamed_relation = MANAGER.cloneWithRename(relation, RenameMap);

   
   return renamed_relation;
   
}

void CSymbSimulator::VCGenerator(CEntity &ent)
{

cout<<"\nGenerating transition relation ...";

FILE* yices_fp = fopen("yices_input.ys","w");

if(yices_fp==NULL)
  {
    //cout<<"\nCannot open yices_input.ys\n";
    exit(1);
  }


set<CDAGNode*> SEsOfAllOutputs;
set<string> Outputs;
CDAGNode *TR;
CDAGNode *UnrolledTR;
 string property; 

 map<string, string> ReplacedVarsMap; // While creating the TR, we are substituting some occurances of internal vars by new vars. This table keeps track of them

 int X_name_count=0; // This keeps track of no:of new vars created

getSEsOfAllOutputsInASet(SEsOfAllOutputs, Outputs, ent);



// Code added on 22 Oct 2010 to get conjunction of delta-TR in a file
bool delta_transition_relation_needed_in_file=true;

if(delta_transition_relation_needed_in_file)
    {
      getDeltaTR(SEsOfAllOutputs, ent);
    }
// Code added on 22 Oct 2010 to get conjunction of delta-TR in a file ends here



 #ifdef DEBUG
 cout<<"\nCreating the transition relation\n";
 #endif

TR = createTransitionRelation(SEsOfAllOutputs, Outputs, ent, ReplacedVarsMap, X_name_count);

 #ifdef DEBUG
 cout<<"\nTransition relation created\n";
 #endif

 //cout<<"\nTR = "<<TR->getDAGStr()<<endl;

 #ifdef DEBUG
 cout<<"\nPreparing the dag in Yices printable format\n";
 #endif

 //cout<<"\nTR = "<<TR->getDAGStr()<<endl;

 TR = prepareDAGForOutputtingInYices(TR, ent.getName(), ReplacedVarsMap);
 
 #ifdef DEBUG
 cout<<"\nPreparing the dag in Yices printable format ends\n";

 cout<<"\nUnrolling and printing in Yices format\n";
 #endif

 //cout<<"\nTR = "<<TR->getDAGStr()<<endl;


 if(clock_based_simplification_enabled)
   {
     #ifdef DEBUG
     cout<<"\nDoing clock based simplification of TR\n";
     #endif

     TR = MANAGER.cloneWithClockBasedSimplification(TR); // Note that this is only possible with the clck based assumption
   }




 #ifdef DEBUG
 cout<<"\nDoing simplification of TR by elimination of trivial disequations\n";
 #endif

 TR = MANAGER.cloneWithTrivialDisequationsEliminated(TR); 

 #ifdef DEBUG
 cout<<"\nEliminating temporary variables created during TR creation from TR\n";
 #endif
 //cout<<"\nTR before elimination of temporary variables = "<<TR->getDAGStr()<<endl;

 TR = eliminateTemporaryVariablesOfTRCreation(TR); // This is always possible

 //cout<<"\nTR after eliminating temporary variables = "<<TR->getDAGStr()<<endl;

 
  #ifdef DEBUG
  cout<<"\nEliminating trivial equalities from TR\n";
  #endif

 
  TR = MANAGER.cloneWithTrivialEquationsEliminated(TR); // In the TR formed, there are trivial equalities of the form x=x.  There is a function eliminateTrivialEqualities in this file which removes x=x type equations. But, this wants TR as a conjunction of realtions which does not happen when Qelimination is ON. Function cloneWithTrivialEquationsEliminated in CHelper.cpp replaces x=x by true

  //cout<<"\nTR after eliminating trivial equalities = "<<TR->getDAGStr()<<endl;
 

  //cout<<"\nTR after simplifications = "<<TR->getDAGStr()<<endl;


 // The following piece of code to first unroll the TR and then print in Yices format is commented. This can cause huge final dags after unrolling. Hence presently, we are calling a function which unrolls and prints in steps without explicitely creating the copies


 /*
cout<<"\nUnrolling\n";
cout<<"\nTimesOfUnrolling = "<<TimesOfUnrolling<<endl;
UnrolledTR = MANAGER.unrollTransitionRelation(TR, TimesOfUnrolling);
cout<<"\nUnrolling done"<<endl;

 cout<<"\nWriting to Yices format\n"; 

// Following line changed on 25/1/2010
//writeInYicesFile(UnrolledTR, yices_fp, ent.getName(), ReplacedVarsMap);
 writeInYicesFileModified(UnrolledTR, yices_fp, ent.getName(), ReplacedVarsMap);

 cout<<"\nWriting to Yices format done\n"; 
 */

  bool apply_quick_simplify = true;
  if(apply_quick_simplify)
  {
	//cout<<"\nCalling quickSimplify on TR\n";
	//cout<<"\nTR before calling quickSimplify = "<<TR->getDAGStr()<<endl;
	map<string, string> SimplificationTable;
  	TR = MANAGER.callQuickSimplify(TR, SimplificationTable);
	//cout<<"\nTR after calling quickSimplify = "<<TR->getDAGStr()<<endl;
  }


  bool apply_and_flattening = true;
  if(apply_and_flattening)
  {
	//cout<<"\nCalling flattening on TR\n";
	//cout<<"\nTR before and_flattening = "<<TR->getDAGStr()<<endl;
	TR = MANAGER.recreateAfterAndFlattening(TR);
	//cout<<"\nTR after and_flattening = "<<TR->getDAGStr()<<endl;
  }

  /* Function call added on 14 June 2010 to print the transition relation in SMTLib format */

  if(TRNeeedInFile)
    {
      //cout<<"\nTR needed in file\n";
  
      TR = convertExpressionToProperFormula(TR, "rules_dnf.txt"); // This is also required
      TR = convertExpressionToProperFormula(TR, "rules_dnf.txt"); // This is also required
      TR = convertExpressionToProperFormula(TR, "rules_simplify.txt"); 

      map<string, int> WidthTableTR;
      getWidthTableForTR(TR, ent.getName(), WidthTableTR);

      map<CDAGNode*, CDAGNode*> SubtractionTable;
      SubtractionTable.clear();
      TR = MANAGER.getSubtractionReplacedClone(TR, SubtractionTable, WidthTableTR);
      
      MANAGER.printTRInFile(TR, WidthTableTR);
    }


cout<<"\nTransition relation generation successful";

  if(exit_after_tr_generation)
   {
	cout<<"\n\n\n";
	return;	
   }

cout<<"\nParsing property...";

  bool property_translator_on=true;
  if(property_translator_on) {
    callPropertyTranslator(ent); // This function calls the property translator to write property.txt file
  }

cout<<"\nParsing of property successful";

cout<<"\nGenerating verification condition ...";
  

 property = readProperty();

 if(TimesOfUnrolling == -1) // TimesOfUnrolling is still default; let's figure it out
 {
 	set<string> PropertySupport;
 	getPropertySupport(PropertySupport);
   	
	for(set<string>::iterator sup_it = PropertySupport.begin(); sup_it != PropertySupport.end(); sup_it++)
	{
		string support_signal = *sup_it;
		int index_dt = support_signal.find_last_of("_");
		string dt_string = support_signal.substr(index_dt+1);
		int dt_int = atoi(dt_string.c_str());

		if(dt_int > TimesOfUnrolling)
		{
			TimesOfUnrolling = dt_int;
		}
	} 

	cout << "\nTimes of unrolling = " << TimesOfUnrolling;
	// char c = getchar();
 }

 assert(TimesOfUnrolling > 0);

 UnrollTR_and_Print_in_Yices_Format(TR, yices_fp, ent.getName(), ReplacedVarsMap, property);

 cout<<"\nVerification condition generation successful";
 
 //cout<<"\nUnrolling and printing in Yices format ends\n";
 
}

  
void CSymbSimulator::getSEsOfAllOutputsInASet(set<CDAGNode*> &SEsOfAllOutputs, set<string> &Outputs, CEntity &ent)
{

  map< string, SymbolicRelation*>& table = ent.getTable();

  map< string, SymbolicRelation*>::iterator itab = table.begin();

  for(;itab != table.end(); itab++)
	{
	  string output = itab->first;
	  Outputs.insert(output);
	  SymbolicRelation* entries = itab->second;
	  SEsOfAllOutputs.insert(entries->relation);
	  }
}

CDAGNode* CSymbSimulator::createTransitionRelation(set<CDAGNode*> SEsOfAllOutputs, set<string> Outputs, CEntity &ent, map<string, string> &ReplacedVarsMap, int &X_name_count)
{
  
  CDAGNode* TR=NULL;
  CDAGNode* TRForOneOutput;
  set<CDAGNode*>::iterator it;
  set<string>::iterator it2;

  for(it=SEsOfAllOutputs.begin(), it2=Outputs.begin();it!=SEsOfAllOutputs.end();it++, it2++)
    {
      TRForOneOutput=createTransitionRelationForGivenOutput(*it, *it2, ent, ReplacedVarsMap, X_name_count);

      //cout << "\nTRForOneOutput == " << TRForOneOutput->getDAGStr() << endl;

      if(TR==NULL)
	TR=TRForOneOutput;
      else
	TR=MANAGER.createDAG("and", TR, TRForOneOutput);
    }
  return TR;
}

int CSymbSimulator::getMaximum(set<int> MySet)
{
  int maximum=0;
  
  for(set<int>::iterator it=MySet.begin(); it!=MySet.end(); it++)
    if(*it>maximum) maximum=*it;
  return maximum;
}

void CSymbSimulator::getNamesFromCSensitiveElementSet(set<CSensitiveElement*> identifiers, set<string> &id_names)
{
  for(set<CSensitiveElement*>::iterator it = identifiers.begin(); it!=identifiers.end(); it++)
    id_names.insert((*it)->name);
}

void CSymbSimulator::showMap(map<string, int> me, string WhoAmI)
{
cout<<"\n"<<WhoAmI<<"\n*****\n\n";
 map<string, int>::iterator me_it;

for(me_it=me.begin(); me_it!=me.end(); me_it++)
	{
	  cout<<me_it->first<<"\t"<<me_it->second<<"\n";
	}
}


CDAGNode* CSymbSimulator::createTransitionRelationForGivenOutput(CDAGNode* SE, string output, CEntity &ent, map<string, string> &ReplacedVarsMap, int &X_name_count)
{

  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);

  set<string> input_id_names, output_inter_id_names;

  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names);
  getNamesFromCSensitiveElementSet(output_identifiers, output_inter_id_names);

  if(qelimination_on==0 || (qelimination_on==1 && Slicing==1)) // if qelimination_on==0, then SE can contain internal variables from the top level entity. When qelimination_on==1 && Slicing==1 also, we are creating TR from concrete SEs only
     {
       getNamesFromCSensitiveElementSet(intermediate_identifiers, output_inter_id_names);
     }

  //cout<<"\nIdentifiers\n";
  
  //showSet(input_id_names, "input_id_names");
  //showSet(output_inter_id_names, "output_inter_id_names");


  if(qelimination_on==0 || (qelimination_on==1 && Slicing==1)) // if qelimination_on==0, then SE can contain variables from lower levels also. They should also be treated as internal signals. . When qelimination_on==1 && Slicing==1 also, we are creating TR from concrete SEs only
    {
      set<string> inter_id_names_from_lower_levels;
      get_inter_id_names_from_lower_levels(SE, input_id_names, output_inter_id_names, inter_id_names_from_lower_levels);
      showSet(inter_id_names_from_lower_levels, "inter_id_names_from_lower_levels");
      attachSet(output_inter_id_names, inter_id_names_from_lower_levels);
    }
  
  //cout<<"\nNow Identifiers Is\n";

  //showSet(input_id_names, "input_id_names");
  //showSet(output_inter_id_names, "output_inter_id_names");



  int shift, MinDeltaDelay;
  set<int> DeltasIncluded, NewDeltas;
  CDAGNode *final, *temp;
  

  //cout<<"\nInitially we have "<<SE->getDAGStr()<<endl;
  //cout<<"\nSteps in obtaining TR\n";

  MinDeltaDelay=0;
  MANAGER.findMinDeltaDelay(SE, MinDeltaDelay);
  shift=-1*MinDeltaDelay;
  DeltasIncluded.insert(shift);
  

  //cout<<"\nShifting SE by "<<shift<<"and storing in final\n";
  temp = MANAGER.cloneAndShift(SE, shift);
  final = temp;
  //cout<<"we have SE for "<<output<<" at time "<<shift<<" as "<<temp->getDAGStr()<<endl;
  //cout<<"\nwe have final SE for "<<output<<" as "<<final->getDAGStr()<<endl;

  NewDeltas.clear();
  //MANAGER.getNewDeltas(temp, output, DeltasIncluded, NewDeltas); // Get all delta times of the variable output from dag temp and put them in NewDeltas provided the delta time is >0 and is not already included in NewDeltas

  MANAGER.getNewDeltasModified(temp, output_inter_id_names, DeltasIncluded, NewDeltas); // Get all delta times of the variable output from dag temp and put them in NewDeltas provided the delta time is >0 and is not already included in NewDeltas
  
  while(!NewDeltas.empty())
    {
      shift = getMaximum(NewDeltas);
      DeltasIncluded.insert(shift);
      temp = MANAGER.cloneAndShift(SE, shift);
      final = MANAGER.createDAG("and", final, temp);
      //cout<<"\nWe have SE for "<<output<<" at time "<<shift<<" as "<<temp->getDAGStr()<<endl;
      
      //cout<<"\nShifting SE by "<<shift<<"and conjuncting with that in final\n";
      // cout<<"\nwe have final SE for "<<output<<" as "<<final->getDAGStr()<<endl;

      NewDeltas.clear();
      //MANAGER.getNewDeltas(temp, output, DeltasIncluded, NewDeltas);
      MANAGER.getNewDeltasModified(temp, output_inter_id_names, DeltasIncluded, NewDeltas);

    }

 

  //cout<<"\nPushing deltas of inputs to limits 0 and 1\n";

  final = MANAGER.cloneWithDeltasAtLimit(final, 0, 1, input_id_names); // input variables with dtime >= 1 becomes 1 and dtime <=0 becomes 0. 
  //cout<<"\nAfter pushDeltasToLimits  We have final SE for "<<output<<" as "<<final->getDAGStr()<<endl;
  
   map<string, int> MaxDeltaMap;

   for(set<string>::iterator it=output_inter_id_names.begin(); it!=output_inter_id_names.end() ; it++)
	   {
	     int MaxDeltaDelay=0;
	     MANAGER.findMaxDeltaDelay(final, *it, MaxDeltaDelay);
	     MaxDeltaMap.insert(make_pair((*it),MaxDeltaDelay));
	   }
   
   //showMap(MaxDeltaMap, "MaxDeltaMap");
     

   map<CDAGNode*,CDAGNode*> DPTable;
   DPTable.clear();
   final = MANAGER.cloneWithReplaceVariablesModified(final, MaxDeltaMap, X_name_count, DPTable, ReplacedVarsMap); 

// Take the dag in final.Now for each variable, check to see if it is an intermediate/output variable. If yes, and it's dtime>0 && dtime< max delta, replace the variable's name by X_countgenerated and delta time by 0. ie. X_count_0 is a new variable replacing the existing variable

   //cout<<"nAfter replaceVariables,  We have final SE for "<<output<<" as "<<final->getDAGStr()<<endl;

   //cout<<"\nPushing deltas of outputs and internal signals to limits 0 and 1\n";

   final = MANAGER.cloneWithDeltasAtLimit(final, 0, 1, output_inter_id_names); // output or intermediate variables with dtime >0 becomes 1 and dtime <=0 becomes 0

   //cout<<"We have final SE for "<<output<<" as "<<final->getDAGStr()<<endl;
   
   return final;
}
  

void CSymbSimulator::writeBVDeclarationsInYicesFormat(CDAGNode* root, FILE *smt_file, string TopLevelEntity, map<string, string> ReplacedVarsMap)

{

  //cout<<"\n\nInside writeBVDeclarationsIntoSMTLib\n\n";

  //cout<<"\n\nReplacedVarsMap\n\n";

  for(map<string, string>::iterator ReplacedVarsMap_it = ReplacedVarsMap.begin();  ReplacedVarsMap_it != ReplacedVarsMap.end(); ReplacedVarsMap_it++)
    {
      //cout<<endl<<ReplacedVarsMap_it->first<<"\t"<<ReplacedVarsMap_it->second<<endl;
    }




  map<string, int> VarsAndSizes;
  map<string, int>::iterator VarsAndSizes_it;
     
  set<string> VarsFromDag;

  string Name, EntityName, lower, upper, Temp;

  int index, l, u, size;

  SymTabEntry ste;
  
  root->getDAGNames(VarsFromDag);

  set<string>::iterator VarsFromDag_it =  VarsFromDag.begin();

  

  for(;VarsFromDag_it != VarsFromDag.end(); VarsFromDag_it++)
    {
      Name = *VarsFromDag_it;

      index = Name.find_last_of("_");
      Name = Name.substr(0, index);  // Here we have splitted the id_delta into id and delta and Name here is id


      // If variable is newly introduced map it back to original variable
      
      map<string, string>::iterator rvm_it = ReplacedVarsMap.find(Name);

      if(rvm_it != ReplacedVarsMap.end()) // variable is a temporary variable introduced while TR creation
	{
	  Name = rvm_it->second;
	}


      if(Name.rfind("__") == string::npos) // Variable is from the current entity
	{
	  EntityName = TopLevelEntity;
	}

      else
	{
	  index = Name.rfind("__");
	  Temp = Name.substr(0, index);
	  Name = Name.substr(index+2); 
	  
	  index = Temp.rfind("__");
	  EntityName = Name.substr(0, index); 

	  if(EntityName.rfind("__") != string::npos)
	    {
	      index = EntityName.rfind("__");
	      EntityName = EntityName.substr(index+2); 
	    }
	}
   
      map<string, SymTabEntry> &ids = SymbolTable[EntityName];

      ste = ids[Name];

      list<string>::iterator ilist = ste.rangeindex.begin();

      lower = -1;

      upper = -1;

     if(ilist != ste.rangeindex.end())

           lower = (*ilist);

      ilist++;

     if(ilist != ste.rangeindex.end())
	
	upper = (*ilist);		

      u = atoi(upper.c_str());

      l = atoi(lower.c_str());

      size = u - l + 1;

      //cout<<"\n"<<*VarsFromDag_it<<"\t"<<size<<endl;

      VarsAndSizes.insert(make_pair((*VarsFromDag_it),size));     
    }


  for(VarsAndSizes_it=VarsAndSizes.begin(); VarsAndSizes_it!=VarsAndSizes.end(); VarsAndSizes_it++)
        {

	  fprintf(smt_file,"(define %s",(VarsAndSizes_it->first).c_str());
	  fprintf(smt_file," :: (bitvector %d))\n",VarsAndSizes_it->second);
	}
 
}

void CSymbSimulator::writeInYicesFile(CDAGNode* root, FILE *fp, string TopLevelEntity, map<string, string> ReplacedVarsMap)
{
// Write the declarations of the variables
writeBVDeclarationsInYicesFormat(root,fp, TopLevelEntity, ReplacedVarsMap);
// Get the dag part in Yices format and print it

// Following line commented and new added on 25/1/10

string dag_str = root->getDAGOfProperSkeltonFormatInYicesFormat("bool"); 

fprintf(fp, "\n(assert %s )\n", dag_str.c_str());
fprintf(fp, "\n(check)\n");
}


CDAGNode* CSymbSimulator::convertExpressionToProperFormula(CDAGNode *dag, string RuleFile)
{
  bool hashing_enabed = true;

  if(hashing_enabed)
    {
      // check if already exists in hash table
      string key;
      
      char temp_char[10];
      sprintf(temp_char , "%x", dag);
      string dag_addr(temp_char);
      key = dag_addr + RuleFile;
      // key is formed

      // check if already exists
      HTStatusValue<CDAGNode*> result = ES_HT.hashtable_search(key);
      if(result.success()) // Exists in HT
	{
	  es_ht_hash_hits++;
	  return result.GetValue();
	}
      
      es_ht_hash_misses++;

      //cout<<"\nconvertExpressionToProperFormula called with RuleFile = "<<RuleFile<<endl;

      ExprSimpRoot = MANAGER.convertCDAGNodeToCExprSimpNode(dag);

      //cout<<"\nBEFORE GIVING TO EXPRESSION SIMPLIFIER = "<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;

      ExpressionSimplifier(RuleFile);

      //cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER = "<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;
 
      CDAGNode *simplified_dag = MANAGER.convertCExprSimpNodeToCDAGNode(ExprSimpRoot);

      //cout<<"\nAFTER CONVERSION = "<<simplified_dag->getDAGStr()<<"\n";

      result = ES_HT.hashtable_insert(key, simplified_dag);
   
      if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function CSymbSimulator::convertExpressionToProperFormula\n";
	  exit(1);
	}     
 
      return simplified_dag;
      
    }
  else // hashing disabled
    { 
  
      //Subexpressions.clear();

      //cout<<"\nconvertExpressionToProperFormula called with RuleFile = "<<RuleFile<<endl;

      ExprSimpRoot = MANAGER.convertCDAGNodeToCExprSimpNode(dag);

      //cout<<"\nBEFORE GIVING TO EXPRESSION SIMPLIFIER = "<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;

      ExpressionSimplifier(RuleFile);

      //cout<<"\nAFTER GIVING TO EXPRESSION SIMPLIFIER = "<<ExprSimpRoot->getCExprSimpNodeStr()<<endl;
 
      CDAGNode *simplified_dag = MANAGER.convertCExprSimpNodeToCDAGNode(ExprSimpRoot);

      //cout<<"\nAFTER CONVERSION = "<<simplified_dag->getDAGStr()<<"\n";

      //Subexpressions.clear();

      // Delete the DAG

      //deleteDAG(ExprSimpRoot); // Temporarily commented

      //cout<<"\nAFTER DELETION\n";
 
      return simplified_dag;
    }
}

CDAGNode* CSymbSimulator::convertExpressionToProperFormulaComplete(CDAGNode *dag, string RuleFile)
{
  bool Same;
  CDAGNode *simplified_dag, *original_dag;
 
  
  original_dag = dag;


  while(1)
    {
          
      cout<<"\nORIGINAL DAG = "<<original_dag->getDAGStr()<<endl;

      simplified_dag = convertExpressionToProperFormula(original_dag, RuleFile);

      if(original_dag->equals(simplified_dag))
      {
	cout<<"\nOriginal DAG = Simplified DAG\n";
	break;
      }
      else
	cout<<"\nNOT(Original DAG = Simplified DAG)\n";
      
    }

   cout<<"\nTHE FINAL DAG\n*****************************************************\n\n";
   
   cout<<simplified_dag->getDAGStr()<<endl<<endl;

   return simplified_dag;

}

CDAGNode* CSymbSimulator::convertExpressionWithExistsToProperFormula(CDAGNode *dag)
{
  string value = dag->Get_DAGnode_value();
  if(value != "exists") return dag;
  else
    {
      list<CDAGNode*>::iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *exists_part = *iout;
      iout++;
      CDAGNode *dag_part = *iout;

      // Check if dag_part is free of all variables to be eliminated

      set<string>  VarsToElim;
      set<string>  VarsPresent;
      set<string>  VarsCommon;
  
      exists_part->getDAGNames(VarsToElim);
      dag_part->getDAGNames(VarsPresent);

      set_intersection(VarsToElim.begin(), VarsToElim.end(), VarsPresent.begin(), VarsPresent.end(),inserter(VarsCommon, VarsCommon.begin()));
      
      if(VarsCommon.empty())
	{
	  //cout<<"\nNo variables left to eliminate\n";
	  return dag_part;
	}
      
      //cout<<"\nCalling convertExpressionToProperFormula\n";

      CDAGNode *simplified_dag_part = convertExpressionToProperFormula(dag_part, "rules_dnf.txt");
      simplified_dag_part = convertExpressionToProperFormula(simplified_dag_part, "rules_dnf.txt");


      //cout<<"\nCalling convertExpressionToProperFormula Done\n";

      //CDAGNode *simplified_dag_part = convertExpressionToProperFormulaComplete(dag_part, "rules_dnf.txt");
      
      CDAGNode *new_dag = MANAGER.createDAG("exists", exists_part, simplified_dag_part);
      return new_dag;
    }
}
  



void CSymbSimulator::deleteDAG(struct CExprSimpNode* dag) 
	{
	  list<struct CExprSimpNode*>::iterator iout = dag->out_list.begin();

	 
	  while(1)
	    {
	      if(dag->out_list.empty()) break;
	      else deleteDAG(*(dag->out_list.begin()));
	    }

	  
	  list<struct CExprSimpNode*> dag_in_list =  dag->in_list;

	  
	  list<struct CExprSimpNode*>::iterator dag_in_list_it = dag_in_list.begin();


	  for(; dag_in_list_it != dag_in_list.end(); dag_in_list_it++) 
		      {
			if(!(((*dag_in_list_it)->out_list).empty()))
			  {
			  (*dag_in_list_it)->out_list.remove(dag);
			  }
		      }
	    
	    free(dag);
	}  




// This function takes as input, the name of a variable and returns it's width looking at the Symbol Table
int CSymbSimulator::getWidthOfVariable(string variable, string TopLevelEntity)
{  
    

  string Name, EntityName, lower, upper, Temp;

  int index, l, u, size;

  SymTabEntry ste;
  
  //cout<<"\nGetting the width\n";

  //cout<<"\nOriginally Variable = "<<variable<<endl;

  Name = variable;

  index = Name.find_last_of("_");
  Name = Name.substr(0, index);  // Here we have splitted the id_delta into id and delta and Name here is id

  //cout<<"\nOriginally Name = "<<Name<<endl;


  //cout<<"\nAfter mapping, Name = "<<Name<<endl;


  if(Name.rfind("__") == string::npos) // Variable is from the current entity
    {
      //cout<<"\nVariable from the current entity\n";
	  
      EntityName = TopLevelEntity;

      //cout<<"\nEntityName = "<<EntityName<<endl;
    }

  else
    {

      //cout<<"\nVariable from the child entity\n";

      index = Name.rfind("__");
      Temp = Name.substr(0, index);
      Name = Name.substr(index+2); 

      //cout<<"\nName = "<<Name<<endl;
      //cout<<"\nTemp = "<<Temp<<endl;
	  
      index = Temp.rfind("__");
      EntityName = Temp.substr(0, index); 

      //cout<<"\nEntityName = "<<EntityName<<endl;

      if(EntityName.rfind("__") != string::npos)
	{
	  //cout<<"\nExctracting out the instance information\n";

	  index = EntityName.rfind("__");
	  EntityName = EntityName.substr(index+2); 

	  //cout<<"\nEntityName = "<<EntityName<<endl;
	}
    }
   

  //cout<<"\nFinally,Name = "<<Name<<"\tEntityName = "<<EntityName<<endl;

  map<string, SymTabEntry> &ids = SymbolTable[EntityName];

  ste = ids[Name];

  list<string>::iterator ilist = ste.rangeindex.begin();

  lower = -1;

  upper = -1;

  if(ilist != ste.rangeindex.end())

    lower = (*ilist);

  ilist++;

  if(ilist != ste.rangeindex.end())
	
    upper = (*ilist);		

  u = atoi(upper.c_str());

  l = atoi(lower.c_str());

  size = u - l + 1;

  //cout<<"\n"<<variable<<"\t"<<size<<endl;

  return size;
}


void CSymbSimulator::getWidthTable(CDAGNode* dag, string TopLevelEntity, map<string, int> &WidthTable)
{
  set<string> Variables;

  dag->getExistsDAGNames(Variables);
  dag->getDAGNames(Variables);

  for(set<string>::iterator it = Variables.begin();  it != Variables.end(); it++)
    {
      string variable = *it;
      int width = getWidthOfVariable(variable, TopLevelEntity);
      WidthTable.insert(make_pair(variable, width));
    }
}



void CSymbSimulator::getWidthTableForFinalUnrolledSE(CDAGNode* root, string TopLevelEntity, map<string, string> ReplacedVarsMap, map<string, int> &WidthTable)

{

  //cout<<"\n\nInside getWidthTableForFinalUnrolledSE\n\n";

  //cout<<"\n\nReplacedVarsMap\n\n";

  //for(map<string, string>::iterator ReplacedVarsMap_it = ReplacedVarsMap.begin();  ReplacedVarsMap_it != ReplacedVarsMap.end(); ReplacedVarsMap_it++)
  //  {
  //    cout<<endl<<ReplacedVarsMap_it->first<<"\t"<<ReplacedVarsMap_it->second<<endl;
  //  }


    
  set<string> VarsFromDag;

  string Name, EntityName, lower, upper, Temp;

  int index, l, u, size;

  SymTabEntry ste;
  
  root->getDAGNames(VarsFromDag);

  set<string>::iterator VarsFromDag_it =  VarsFromDag.begin();

  

  for(;VarsFromDag_it != VarsFromDag.end(); VarsFromDag_it++)
    {
      Name = *VarsFromDag_it;

      index = Name.find_last_of("_");
      Name = Name.substr(0, index);  // Here we have splitted the id_delta into id and delta and Name here is id

      //cout<<"\nOriginally Name = "<<Name<<endl;


      // If variable is newly introduced map it back to original variable
      
      map<string, string>::iterator rvm_it = ReplacedVarsMap.find(Name);

      if(rvm_it != ReplacedVarsMap.end()) // variable is a temporary variable introduced while TR creation
	{
	  Name = rvm_it->second;
	}


      //cout<<"\nAfter mapping, Name = "<<Name<<endl;


      if(Name.rfind("__") == string::npos) // Variable is from the current entity
	{
	  //cout<<"\nVariable from the current entity\n";
	  
	  EntityName = TopLevelEntity;

	  //cout<<"\nEntityName = "<<EntityName<<endl;
	}

      else
	{

	  //cout<<"\nVariable from the child entity\n";

	  index = Name.rfind("__");
	  Temp = Name.substr(0, index);
	  Name = Name.substr(index+2); 

	  //cout<<"\nName = "<<Name<<endl;
	  //cout<<"\nTemp = "<<Temp<<endl;
	  
	  index = Temp.rfind("__");
	  EntityName = Temp.substr(0, index); 

	  //cout<<"\nEntityName = "<<EntityName<<endl;

	  if(EntityName.rfind("__") != string::npos)
	    {
	      //cout<<"\nExctracting out the instance information\n";

	      index = EntityName.rfind("__");
	      EntityName = EntityName.substr(index+2); 

	      //cout<<"\nEntityName = "<<EntityName<<endl;
	    }
	}
   

      //cout<<"\nFinally,Name = "<<Name<<"\tEntityName = "<<EntityName<<endl;

      map<string, SymTabEntry> &ids = SymbolTable[EntityName];

      ste = ids[Name];

      list<string>::iterator ilist = ste.rangeindex.begin();

      lower = -1;

      upper = -1;

     if(ilist != ste.rangeindex.end())

           lower = (*ilist);

      ilist++;

     if(ilist != ste.rangeindex.end())
	
	upper = (*ilist);		

      u = atoi(upper.c_str());

      l = atoi(lower.c_str());

      size = u - l + 1;

      //cout<<"\n"<<*VarsFromDag_it<<"\t"<<size<<endl;

      WidthTable.insert(make_pair((*VarsFromDag_it),size));     
    }

}

void CSymbSimulator::writeBVDeclarationsInYicesFormatModified(FILE *smt_file, map<string, int> WidthTable)

{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {

	  fprintf(smt_file,"(define %s",(it->first).c_str());
	  fprintf(smt_file," :: (bitvector %d))\n",it->second);
	}
 
}


void CSymbSimulator::writeInYicesFileModified(CDAGNode* root, FILE *fp, string TopLevelEntity, map<string, string> ReplacedVarsMap)
{

  map<string, int> WidthTable;


  cout<<"\nGetting the width table and printing in Yices\n";

  //get the WidthTable
  getWidthTableForFinalUnrolledSE(root, TopLevelEntity, ReplacedVarsMap, WidthTable);


  // Write the declarations of the variables
  writeBVDeclarationsInYicesFormatModified(fp, WidthTable);


  //cout<<"\nWidth table\n";
  //map<string, int>::iterator wit =  WidthTable.begin();
  //for(;wit !=  WidthTable.end(); wit++)
  //  {
  //    cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
  //  }


  //cout<<"\nFINAL SE ORIGINALLY\n";

  //cout<<"\n"<<root->getDAGStr()<<endl;


  //cout<<"\nPreprocessing the dah before printing in Yices format\n";


  //root = MANAGER.getPreprocessedClone(root);  

  //cout<<"\nAfter renaming predicates, SE = "<<root->getDAGStr()<<"\n";

 

  //cout<<"\nRemoving unneeded selections\n";

  //set<CDAGNode*> UnneededSelects;
  
  //int width = MANAGER.findUnneededSelections(root, UnneededSelects, false, WidthTable);

  
  //cout<<"\nUnneeded Selects are\n";

  //for(set<CDAGNode*>::iterator it = UnneededSelects.begin(); it != UnneededSelects.end(); it++)
  //  {
  //    cout<<"\n"<<*it<<"\t"<<(*it)->getDAGStr()<<endl;
  //   }
  

  //root = MANAGER.getSelectionsRemovedClone(root, UnneededSelects);

  //cout<<"\nSE after removing unneeded selections = "<<root->getDAGStr()<<endl;


  // cout<<"\nNow the SE is preprocessed. Getting it in Yices format\n";

  
  // Get the dag part in Yices format and print it
  
  // Replacing this with function which directly prints to file rather than storing it intermediately ON 27/01/2010

  //string dag_str = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(root, WidthTable);
  //fprintf(fp, "\n(assert %s )\n", dag_str.c_str());
  //fprintf(fp, "\n(check)\n");

  cout<<"\nPrinting the dag in Yices\n";

  fprintf(fp, "\n(assert ");
  MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile(root, WidthTable, fp);
  fprintf(fp, ")\n(check)\n");
  
  fclose(fp);
}


CDAGNode* CSymbSimulator::prepareDAGForOutputtingInYices(CDAGNode *root, string TopLevelEntity, map<string, string> ReplacedVarsMap)
{

  map<string, int> WidthTable;

  //get the WidthTable
  getWidthTableForFinalUnrolledSE(root, TopLevelEntity, ReplacedVarsMap, WidthTable);

  root = MANAGER.getPreprocessedClone(root);  

  //cout<<"\nAfter renaming predicates, SE = "<<root->getDAGStr()<<"\n";
  
  //cout<<"\nRemoving unneeded selections\n";

  set<CDAGNode*> UnneededSelects;
  
  int width = MANAGER.findUnneededSelections(root, UnneededSelects, false, WidthTable);

  
  //cout<<"\nUnneeded Selects are\n";

  //for(set<CDAGNode*>::iterator it = UnneededSelects.begin(); it != UnneededSelects.end(); it++)
  //  {
  //    cout<<"\n"<<*it<<"\t"<<(*it)->getDAGStr()<<endl;
  //   }
  

  root = MANAGER.getSelectionsRemovedClone(root, UnneededSelects);

  //cout<<"\nSE after removing unneeded selections = "<<root->getDAGStr()<<endl;


  // cout<<"\nNow the SE is preprocessed. Getting it in Yices format\n";
  
  return root;
}



void CSymbSimulator::UnrollTR_and_Print_in_Yices_Format(CDAGNode* root, FILE *fp, string TopLevelEntity, map<string, string> ReplacedVarsMap, string property)
{

  map<string, int> WidthTable_TR;


  //cout<<"\nGetting the width table and printing in Yices\n";

  //get the WidthTable
  getWidthTableForFinalUnrolledSE(root, TopLevelEntity, ReplacedVarsMap, WidthTable_TR);


  // get the width table for the unrolled TR
  map<string, int> WidthTable;
  getWidthTableForUnrolledTR(WidthTable_TR, WidthTable);
  


  // Write the declarations of the variables
  writeBVDeclarationsInYicesFormatModified(fp, WidthTable);


  //cout<<"\nWidth table\n";
  //map<string, int>::iterator wit =  WidthTable.begin();
  //for(;wit !=  WidthTable.end(); wit++)
  //  {
  //    cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
  //  }


  //cout<<"\nUnrolling the TR and printing in Yices\n";

  MANAGER.UnrollTR_and_Print_in_Yices_Format(root, WidthTable, fp, TimesOfUnrolling, property);
    
  //cout<<"\nUnrolling the TR and printing in Yices finished\n";

  fclose(fp);
}


void CSymbSimulator::getWidthTableForUnrolledTR(map<string, int> WidthTable_TR, map<string, int> &WidthTable){
  set<string> ids_processed;

  for(map<string, int>::iterator it = WidthTable_TR.begin(); it != WidthTable_TR.end(); it++)
    {
      
      string id_delta = it->first;
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);

      if(ids_processed.find(id) != ids_processed.end())
	continue;

      ids_processed.insert(id);

      int width = it->second;

      for(int i=0; i<=TimesOfUnrolling; i++)
	{
	  char delta_char[10];
	  sprintf(delta_char, "%d", i);
	  string delta_string(delta_char);
	  
	  string new_id_delta = id + "_" + delta_string;
	  
	  WidthTable.insert(make_pair(new_id_delta, width));
	}
    }
}
	  
			

void CSymbSimulator::get_inter_id_names_from_lower_levels(CDAGNode *SE, set<string> input_id_names, set<string> output_inter_id_names, set<string> &inter_id_names_from_lower_levels)      
{
  set<string> names;
  SE->getDAGNames(names);
  
  for(set<string>::iterator it = names.begin(); it != names.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      string name = id_delta.substr(0, index);
      
      if(input_id_names.find(name) == input_id_names.end() &&  output_inter_id_names.find(name) == output_inter_id_names.end()) // name is neither in input variable names nor in internal/output variable names
	{
	  cout<<"\n"<<name<<" is coming from lower level entities. Hence it is internal\n";
	  inter_id_names_from_lower_levels.insert(name);
	}
    }
}
 

// union source set to destination set 
void CSymbSimulator::attachSet(set<string> &destination, set<string> source)
{
  
for(set<string>::iterator it = source.begin(); it != source.end(); it++)
    {
      destination.insert(*it);
    }
}



CDAGNode* CSymbSimulator::removeUnneededSelects(CDAGNode *dag, map<string, int> WidthTable)
{
  string value = dag->Get_DAGnode_value();
  if(value != "exists") return dag;
  else
    {
      list<CDAGNode*>::iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *exists_part = *iout;
      iout++;
      CDAGNode *dag_part = *iout;

      set<CDAGNode*> UnneededSelects;

      int width = MANAGER.findUnneededSelectionsModified(dag_part, UnneededSelects, false, WidthTable, false);
       

      //cout<<"\nUnneeded Selects are\n";

      for(set<CDAGNode*>::iterator it = UnneededSelects.begin(); it != UnneededSelects.end(); it++)
	{
	  //cout<<"\n"<<*it<<"\t"<<(*it)->getDAGStr()<<endl;
	}


      dag_part = MANAGER.getSelectionsRemovedClone(dag_part, UnneededSelects);

      //cout<<"\nDAG after removing unneeded selections = "<<dag_part->getDAGStr()<<endl;

      CDAGNode *new_dag = MANAGER.createDAG("exists", exists_part, dag_part);

      return new_dag;
    }
}



CDAGNode* CSymbSimulator::simplifyFormulaWithExists(CDAGNode *dag, string RuleFile)
{
  string value = dag->Get_DAGnode_value();
  if(value != "exists") return dag;
  else
    {
      list<CDAGNode*>::iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *exists_part = *iout;
      iout++;
      CDAGNode *dag_part = *iout;
      
      CDAGNode *simplified_dag_part = convertExpressionToProperFormula(dag_part, RuleFile);

      CDAGNode *new_dag = MANAGER.createDAG("exists", exists_part, simplified_dag_part);
      return new_dag;
    }
}




/*******Newly added functions on 25.04.2010 to add slicing ********************************/


int CSymbSimulator::VCGenerator_With_Slicing(CEntity &ent, list<int> sortedIndices)
{
struct timeval cvc_generation_from_se_time_start_ms, cvc_generation_from_se_time_end_ms;
struct timeval total_cegar_time_start_ms, total_cegar_time_end_ms;
struct timeval total_avc_generation_time_start_ms, total_avc_generation_time_end_ms;
struct timeval total_expose_time_start_ms, total_expose_time_end_ms;


gettimeofday(&cvc_generation_from_se_time_start_ms, NULL); 


map<string, CDAGNode*> SEsOfAllOutputs;
map<string, CDAGNode*> TRMAP;
map<string, CDAGNode*> TRMAP_Proper;

CDAGNode* TR=NULL;

set<string> InterestedOPs;
string property; 
map<string, string> ReplacedVarsMap; // While creating the TR, we are substituting some occurances of internal vars by new vars. This table keeps track of them

int X_name_count=0; // This keeps track of no:of new vars created

 getSEsOfAllOutputsInAMap(SEsOfAllOutputs, ent);

 //cout<<"\nCreating the transition relation map\n";

 getTransitionRelationMap(SEsOfAllOutputs, ent, ReplacedVarsMap, X_name_count, TRMAP);

 //cout<<"\nTransition relation map created\n";

//   cout<<"\nTR MAP\n";
 
//   for(map<string, CDAGNode*>::iterator it=TRMAP.begin(); it!=TRMAP.end(); it++)
//     {
//       cout<<it->first<<"\n"<<(it->second)->getDAGStr()<<endl<<endl;
//     }

 
 //cout<<"\nPreparing the dags in TRMAP in Yices printable format\n";
 
 for(map<string, CDAGNode*>::iterator it=TRMAP.begin(); it!=TRMAP.end(); it++)
   {
    CDAGNode *TR_Proper =  prepareDAGForOutputtingInYices(it->second, ent.getName(), ReplacedVarsMap);
    TRMAP_Proper.insert(make_pair(it->first, TR_Proper));
   }

//   cout<<"\nTR MAP PROPER\n";
 
//   for(map<string, CDAGNode*>::iterator it=TRMAP_Proper.begin(); it!=TRMAP_Proper.end(); it++)
//     {
//       cout<<it->first<<"\n"<<(it->second)->getDAGStr()<<endl<<endl;
//     }

 bool property_translator_on=true;
  if(property_translator_on) {
    callPropertyTranslator(ent); // create property.txt by calling the property translator
  }

 property = readProperty();


 set<string> PropertySupport;
 bool support_property_reading_on=true;
 if(support_property_reading_on)
   {
     getPropertySupport(PropertySupport);
   }

 

 // InterestedOPs contains the set of outputs at different frames referred in the property
 getInterestedOPs(InterestedOPs);

 

//  cout<<"\nInterestedOPs\n";
//  for(set<string>::iterator it1=InterestedOPs.begin(); it1!=InterestedOPs.end(); it1++)
//    {
//      cout<<*it1<<"\t";
//    }





//  cout<<"\nTR MAP PROPER\n";
 

//  cout<<"\nSize of TRMAP_Proper = "<<TRMAP_Proper.size()<<endl;
//  map<string, CDAGNode*>::iterator it=TRMAP_Proper.begin();
//  cout<<it->first<<"\n"<<(it->second)->getDAGStr()<<endl<<endl;
//  it++;


  
 


//  cout<<"\nSize of TRMAP_Proper = "<<TRMAP_Proper.size()<<endl;
 int i=0;
 for(map<string, CDAGNode*>::iterator it=TRMAP_Proper.begin(); it!=TRMAP_Proper.end(); it++)
   {
//      cout<<it->first<<"\n"<<(it->second)->getDAGStr()<<endl<<endl;

     if(TR == NULL) TR=it->second;
     else TR = MANAGER.createDAG("and", it->second, TR); 
     i++;

     if(i>=TRMAP_Proper.size()) break; // This strange way of exiting from the loop is required due to some problem with STL maps. But the earlier loop on TRMAP_Proper is working. Why???
   }

 

 //cout<<"\nTR = "<<TR->getDAGStr()<<endl;

 // Simplify the TR by 1) clock based simplification, 2) eliminate the X variables by inlining

 //cout<<"\nDoing clock based simplification of TR\n";

 TR = MANAGER.cloneWithClockBasedSimplification(TR); // Note that this is only possible with the clck based assumption

 //cout<<"\nTR after clock based simplication = "<<TR->getDAGStr()<<endl;

 //cout<<"\nEliminating temporary variables created during TR creation from TR\n";

 //cout<<"\nTR before eliminating temporary variables = "<<TR->getDAGStr()<<endl;

 TR = eliminateTemporaryVariablesOfTRCreation(TR); // This is always possible
 
 //cout<<"\nTR after eliminating temporary variables = "<<TR->getDAGStr()<<endl;

 //if(true) exit(1);

  //cout<<"\nEliminating trivial equalities from TR\n";

 TR = eliminateTrivialEqualities(TR); // In the TR formed, there are trivial equalities of the form x=x. Eliminate them

 //cout<<"\nTR after eliminating trivial equalities = "<<TR->getDAGStr()<<endl;
 

 //cout<<"\nTR after simplifications = "<<TR->getDAGStr()<<endl;

 printTRDetails(ent, TR); 

  
 
 map<string, set<string> > DepMapFinal;
 MANAGER.getDepMapFinal(TR, DepMapFinal);

 //cout<<"\nDepMapFinal Obtained\n";
//  for(map<string, set<string> >::iterator it=DepMapFinal.begin();it!=DepMapFinal.end();it++)
//     {
//       string signal = it->first;
//       set<string> friends = it->second;
//       cout<<"\nSignal = "<<signal<<endl;
//       cout<<"Friends\n";
//       for(set<string>::iterator it1=friends.begin(); it1!=friends.end(); it1++)
// 	cout<<*it1<<"\t";
//       cout<<"\n";      
//     }

  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
  set<string> input_id_names;
  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names);

  map<string, set<string> > TrimmedDepMapFinal;
  for(map<string, set<string> >::iterator it=DepMapFinal.begin();it!=DepMapFinal.end();it++)
    {
      string signal = it->first;
      int index = signal.find_last_of("_");
      string signal_name = signal.substr(0, index);

      if(signal_name[0]=='X') continue;

      int frame = atoi(signal.substr(index+1).c_str());
      if(frame == 0) continue;

      set<string> friends = it->second;
      set<string> CloseFriends;
      for(set<string>::iterator it1=friends.begin(); it1!=friends.end(); it1++)
	{
	  string friend1 = *it1;
	  int index = friend1.find_last_of("_");
	  string friend1_name = friend1.substr(0, index);

	  if(input_id_names.find(friend1_name)!=input_id_names.end()) continue;
	  if(friend1[0]=='X') continue;
	  if(friend1==signal) continue;

	  CloseFriends.insert(friend1);
	}

      TrimmedDepMapFinal.insert(make_pair(signal, CloseFriends));
    }

 //cout<<"\nTrimmedDepMapFinal Obtained\n";
//  for(map<string, set<string> >::iterator it=TrimmedDepMapFinal.begin();it!=TrimmedDepMapFinal.end();it++)
//     {
//       string signal = it->first;
//       set<string> friends = it->second;
//       cout<<"\nSignal = "<<signal<<endl;
//       cout<<"Close Friends\n";
//       for(set<string>::iterator it1=friends.begin(); it1!=friends.end(); it1++)
// 	cout<<*it1<<"\t";
//       cout<<"\n";      
//     }

 
 //cout<<"\nCalculating InterestedOPsFinal\n";

 
 set<string> InterestedOPsFinal;

 if(false)
   {
 for(set<string>::iterator it=InterestedOPs.begin(); it!=InterestedOPs.end(); it++)
   {
     string output = *it;
     set<string> FP;
     //cout<<"\nCalculating FP for output = "<<output<<endl;
     getFixPoint(TrimmedDepMapFinal, output, FP);
     //cout<<"\nCalculating FP for output = "<<output<<" finished "<<endl;
     set_union(InterestedOPsFinal.begin(), InterestedOPsFinal.end(), FP.begin(), FP.end(),inserter(InterestedOPsFinal, InterestedOPsFinal.begin()));
   }
   }
 else
   {
     getFixPointOfSet(TrimmedDepMapFinal, InterestedOPs, InterestedOPsFinal);
   }
 


 //cout<<"\nInterestedOPsFinal Obtained\n";
//  for(set<string>::iterator it1=InterestedOPsFinal.begin(); it1!=InterestedOPsFinal.end(); it1++)
//    {
//      cout<<*it1<<"\t";
//    }

 map<int, set<string> > FRAMEMAP;
 for(set<string>::iterator it1=InterestedOPsFinal.begin(); it1!=InterestedOPsFinal.end(); it1++)
   {
     string signal = *it1;
     int index = signal.find_last_of("_");
     string signal_name = signal.substr(0, index);
     int frame = atoi(signal.substr(index+1).c_str());
     
     map<int, set<string> >::iterator fit = FRAMEMAP.find(frame);
     if(fit == FRAMEMAP.end())
       {
	 set<string> signals;
	 signals.insert(signal_name);
	 FRAMEMAP.insert(make_pair(frame, signals));
       }
     else
       {
	 (fit->second).insert(signal_name);
       }
   }

 //cout<<"\nFRAMEMAP Obtained\n";
//  for(map<int, set<string> >::iterator it = FRAMEMAP.begin(); it != FRAMEMAP.end(); it++)
//    {
//      cout<<it->first<<"\t";
//      for(set<string>::iterator it1=(it->second).begin(); it1!=(it->second).end(); it1++)
//        {
// 	 cout<<*it1<<"\t";
//        }
//      cout<<endl;
//    }

 

 vector<string> hierarchy;
 obtainHierarchy(hierarchy, ent, "");
 hierarchy.push_back(ent.getName());

 
 //cout<<"\nHierarchy Obtained\n";
//  for(int i=0; i<hierarchy.size(); i++)
//    cout<<hierarchy[i]<<endl;


 map<string, map<string, CDAGNode*> > InstanceFunctionMap;
 createInstanceFunctionMap(InstanceFunctionMap, hierarchy, TR, ent);


 //cout<<"\nInstanceFunctionMap Obtained\n";
//  for(map<string, map<string, CDAGNode*> >::iterator it=InstanceFunctionMap.begin(); it!=InstanceFunctionMap.end(); it++)
//    {
//      cout<<"\nInstance = "<<it->first<<endl;
//      for(map<string, CDAGNode*>::iterator it2=(it->second).begin(); it2!=(it->second).end(); it2++)
//        {
// 	 cout<<"\nsignal = "<<it2->first<<" , rhs = "<<(it2->second)->getDAGStr()<<"\n";
//        }
//      cout<<endl;
//    }

 

 if(!refinement_on || qelimination_on == 0)
   {
     createFrames_and_Print_in_Yices_Format_Modified(hierarchy, FRAMEMAP, InstanceFunctionMap, ent, property, sortedIndices, PropertySupport);
     //cout<<"\nVCGenerator_With_Slicing finished\n";
     //cout<<"\nWriting in Yices over\n";
     //exit(1);
   }
 else // enter here only if refinement_on and qelimination_on == 1
   {
     cout<<"\nEntering the CEGAR loop\n";

     
     map<string, int> WidthTableForCVC;

     vector<CDAGNode*> UnrolledTRs; 

     CDAGNode* CVC = generateConcreteVerificationCondition(hierarchy, FRAMEMAP, InstanceFunctionMap, ent, property, sortedIndices, WidthTableForCVC, UnrolledTRs);// generate the concrete verification condition without property i.e. R1/\.../\Rn in CVC with R1,...,Rn in UnrolledTRs
     
     //cout<<"\nConcrete Verification Condition = "<<CVC->getDAGStr()<<endl;

     gettimeofday(&cvc_generation_from_se_time_end_ms, NULL); 
     cvc_generation_from_se_time = cvc_generation_from_se_time_end_ms.tv_sec * 1000 + cvc_generation_from_se_time_end_ms.tv_usec / 1000;
     cvc_generation_from_se_time -= cvc_generation_from_se_time_start_ms.tv_sec * 1000 + cvc_generation_from_se_time_start_ms.tv_usec / 1000;

     gettimeofday(&total_cegar_time_start_ms, NULL);

     while(true) // CEGAR loop
       {
	 
	 cout<<"\nStarting the CEGAR loop "<<cegar_iteration<<endl;

	 //cout<<"\nPress any key to continue \n";

	 //char c = getchar();

	 unsigned long long int total_avc_generation_time_duration_ms;
	 gettimeofday(&total_avc_generation_time_start_ms, NULL);
	 
	 createFrames_and_Print_in_Yices_Format_Modified(hierarchy, FRAMEMAP, InstanceFunctionMap, ent, property, sortedIndices, PropertySupport); // write the abstract verification condition in yices_input.ys

	 gettimeofday(&total_avc_generation_time_end_ms, NULL);
         total_avc_generation_time_duration_ms = total_avc_generation_time_end_ms.tv_sec * 1000 + total_avc_generation_time_end_ms.tv_usec / 1000;
         total_avc_generation_time_duration_ms -= total_avc_generation_time_start_ms.tv_sec * 1000 + total_avc_generation_time_start_ms.tv_usec / 1000;        
	 total_avc_generation_time = total_avc_generation_time + total_avc_generation_time_duration_ms;

	 cout<<"\n\nVerification Condition yices_input.ys given to solver. Please Wait...\n\n";

	 time_t yices_time_inside_cegar_start, yices_time_inside_cegar_end, yices_time_inside_cegar_duration;
	 time(&yices_time_inside_cegar_start);

	 struct timeval yices_time_inside_cegar_start_ms, yices_time_inside_cegar_end_ms;
	 unsigned long long int yices_time_inside_cegar_duration_ms;
	 gettimeofday(&yices_time_inside_cegar_start_ms, NULL);


	 system("yices -e -tc -tm 1800 yices_input.ys > yices_output.txt");

	gettimeofday(&yices_time_inside_cegar_end_ms, NULL);

        yices_time_inside_cegar_duration_ms = yices_time_inside_cegar_end_ms.tv_sec * 1000 + yices_time_inside_cegar_end_ms.tv_usec / 1000;
        yices_time_inside_cegar_duration_ms -= yices_time_inside_cegar_start_ms.tv_sec * 1000 + yices_time_inside_cegar_start_ms.tv_usec / 1000;
        
	yices_time_inside_cegar_ms = yices_time_inside_cegar_ms + yices_time_inside_cegar_duration_ms;	
	

	 time(&yices_time_inside_cegar_end);

	 yices_time_inside_cegar_duration = yices_time_inside_cegar_end - yices_time_inside_cegar_start;
	 yices_time_inside_cegar = yices_time_inside_cegar + yices_time_inside_cegar_duration;


	 FILE *result_file = fopen("yices_output.txt", "r");

	 if(result_file == NULL) {cout<<"\nCannot open file yices_output.txt\n"; exit(1);}

	 char result[100];
	 fscanf(result_file, "%s", result);
	 fclose(result_file);
	 string result_str(result);

	 if(result_str=="unsat") // VHDL design holds the property
	   {
	     cout<<"\nVHDL design holds the property\n";

	     break;
	   }
	 else if(result_str=="sat") // CEX obtained in file yices_output.txt
	   {

	     cout<<"\nVHDL design does not hold the property. Analyzing the counterexample\n";

	     map<string, string> Model;

	     getModelFromFile(Model);

	     cout<<"\nThe CEX \n";

	     for(map<string, string>::iterator mit = Model.begin(); mit!=Model.end(); mit++)
	       {
		 cout<<endl<<mit->first<<"\t"<<mit->second<<endl;
	       }

	     set<string> SetOfSignalsTobeExposed;

	     int strategy_to_find_signals_tobe_exposed = 2;

	     bool real;

	     if(strategy_to_find_signals_tobe_exposed == 1) // 1 means the strategy is
	       // stopping at the first mismatch between observed value and value in CEX
	       // of any output signal and going back from there using the simple dag
	       // evaluation based strategy to find smaller cores
	       {
		 real = evaluateConcreteVerificationConditionAndFindSetOfSignalsTobeExposed(CVC, UnrolledTRs, WidthTableForCVC, Model, ent, SetOfSignalsTobeExposed);
	       }
	     else if(strategy_to_find_signals_tobe_exposed == 2) // 2 means the strategy is
	       // evaluating the property using the values obtained from the CEX
	       //  and going back from all these places using the unsat cores to find smaller cores
	       {
		 unsigned long long int total_expose_time_duration_ms;
		 gettimeofday(&total_expose_time_start_ms, NULL);
	 
		 real = evaluateConcreteVerificationConditionAndFindSetOfSignalsTobeExposedPropertyGuided(CVC, UnrolledTRs, WidthTableForCVC, Model, ent, SetOfSignalsTobeExposed);

		 gettimeofday(&total_expose_time_end_ms, NULL);
		 total_expose_time_duration_ms = total_expose_time_end_ms.tv_sec * 1000 + total_expose_time_end_ms.tv_usec / 1000;
		 total_expose_time_duration_ms -= total_expose_time_start_ms.tv_sec * 1000 + total_expose_time_start_ms.tv_usec / 1000;        
		 total_expose_time = total_expose_time + total_expose_time_duration_ms;
	       }
	     else
	       {
		 cout<<"\nUnknown value "<<strategy_to_find_signals_tobe_exposed<<" for strategy_to_find_signals_tobe_exposed in CSymbSimulator::VCGenerator_With_Slicing\n";
		 exit(1);
	       }
		 

	     if(real) // CEX is real
	       {
		 cout<<"\nVHDL design does not hold the property. Please look at the counterexample\n";

		 string command = "cex2vcd";
		 
		 system(command.c_str()); // convert CEX obtained in file yices_output.txt to generate CEX in VCD format in vcd_form.vcd

		 break;
	       }
	     else // CEX is spurious
	       {
		 refinement(SetOfSignalsTobeExposed, ent);
	       }

	   }
	 else // Unknown from yices
	   {

	     cout<<"\nUnknown result from the solver. Most probably the solver has timed out!!! Please decrease the bound in the property\n"; 
	     exit(1);
	   }


	 if(cegar_iteration >= no_of_cegar_iterations)
	   {
	     cout<<"\nWe have reached the maximum number of CEGAR iterations.\n";
	     cout<<"\nPlease increase the number of CEGAR iterations\n";
	     break;
	   }
	 
	 cegar_iteration++;

       }// CEGAR loop ends here


     gettimeofday(&total_cegar_time_end_ms, NULL); 
     total_cegar_time = total_cegar_time_end_ms.tv_sec * 1000 + total_cegar_time_end_ms.tv_usec / 1000;
     total_cegar_time -= total_cegar_time_start_ms.tv_sec * 1000 + total_cegar_time_start_ms.tv_usec / 1000;

   }//refinement_on ends here

  return 1;

}



void CSymbSimulator :: getFixPointOfSet(map<string, set<string> > &DepMap, set<string> output, set<string> &FixPoint)
{
  
  set_union(FixPoint.begin(), FixPoint.end(), output.begin(), output.end(),inserter(FixPoint, FixPoint.begin()));
  
  while(1){
    set<string> NewState;
    getImage(DepMap, FixPoint, NewState);
    set<string> NewFixPoint;
    set_union(FixPoint.begin(), FixPoint.end(), NewState.begin(), NewState.end(),inserter(NewFixPoint, NewFixPoint.begin()));
    if(setsEqual(FixPoint, NewFixPoint)) break;
    set_union(FixPoint.begin(), FixPoint.end(), NewFixPoint.begin(), NewFixPoint.end(),inserter(FixPoint, FixPoint.begin()));    
  }
}




void CSymbSimulator :: getFixPoint(map<string, set<string> > &DepMap, string output, set<string> &FixPoint)
{
  
  //cout<<"\noutput = "<<output<<endl;
  FixPoint.insert(output);
  
  while(1){
    set<string> NewState;
    getImage(DepMap, FixPoint, NewState);
    set<string> NewFixPoint;
    set_union(FixPoint.begin(), FixPoint.end(), NewState.begin(), NewState.end(),inserter(NewFixPoint, NewFixPoint.begin()));
    if(setsEqual(FixPoint, NewFixPoint)) break;
    set_union(FixPoint.begin(), FixPoint.end(), NewFixPoint.begin(), NewFixPoint.end(),inserter(FixPoint, FixPoint.begin()));    
  }
}

void CSymbSimulator :: getImage(map<string, set<string> > &DepMap, set<string> &FixPoint, set<string> &NewState)
{
  for(set<string>::iterator it=FixPoint.begin(); it!=FixPoint.end(); it++)
    {
      string signal = *it;
      //cout<<"\nsignal = "<<signal<<endl;
      int index = signal.find_last_of("_");
      string signal_name = signal.substr(0, index);
      int frame = atoi(signal.substr(index+1).c_str());
      
      if(frame<=1) continue;

      string signal_to_check = signal_name;
      signal_to_check += "_";
      signal_to_check += "1";

      //cout<<"\nsignal_to_check = "<<signal_to_check<<endl;

      map<string, set<string> > ::iterator it2 = DepMap.find(signal_to_check);
      if(it2!=DepMap.end())
	{
	  set<string> Image = it2->second;
	  set<string> ProperImage;
	  
	  for(set<string>::iterator it3=Image.begin(); it3!=Image.end(); it3++)
	    {
	     string friend1 = *it3;

	     //cout<<"\nfriend1 = "<<friend1<<endl;
	     
	     int index = friend1.find_last_of("_");
	     string friend1_name = friend1.substr(0, index); 
	     int friend1_frame = atoi(friend1.substr(index+1).c_str());

	     int newframe;
	     if(friend1_frame == 1)  newframe = frame;
	     else newframe = frame-1;

	     char temp[100];
	     sprintf(temp, "%d", newframe);
	     string dt(temp);
	     friend1_name += "_";
	     friend1_name += dt;

	     //cout<<"\nfriend1_name = "<<friend1_name<<endl;

	     ProperImage.insert(friend1_name);
	    }
	  set_union(ProperImage.begin(), ProperImage.end(), NewState.begin(), NewState.end(),inserter(NewState, NewState.begin()));
	}
    }
}

bool CSymbSimulator :: setsEqual(set<string> &FixPoint, set<string> &NewFixPoint)
{
 set<string> Set1, Set2;
 set_difference(FixPoint.begin(), FixPoint.end(), NewFixPoint.begin(), NewFixPoint.end(),inserter(Set1, Set1.begin()));
 set_difference(NewFixPoint.begin(), NewFixPoint.end(), FixPoint.begin(), FixPoint.end(),inserter(Set2, Set2.begin()));
 if(Set1.empty() && Set2.empty()) return true;
 return false;
}
 


void CSymbSimulator::getSEsOfAllOutputsInAMap(map<string, CDAGNode*> &SEsOfAllOutputs, CEntity &ent)
{

  map< string, SymbolicRelation*>& table = ent.getTable();

  map< string, SymbolicRelation*>::iterator itab = table.begin();

  for(;itab != table.end(); itab++)
	{
	  string output = itab->first;
	  SymbolicRelation* entries = itab->second;
	  CDAGNode *SE = entries->relation;
	  SEsOfAllOutputs.insert(make_pair(output, SE));
	  }
}


void CSymbSimulator::getTransitionRelationMap(map<string, CDAGNode*> &SEsOfAllOutputs, CEntity &ent, map<string, string> &ReplacedVarsMap, int &X_name_count, map<string, CDAGNode*> &TRMAP)
{
  
  CDAGNode* TRForOneOutput;
  map<string, CDAGNode*>::iterator it;

  for(it=SEsOfAllOutputs.begin();it!=SEsOfAllOutputs.end();it++)
    {
      TRForOneOutput=createTransitionRelationForGivenOutput(it->second, it->first, ent, ReplacedVarsMap, X_name_count);
      TRMAP.insert(make_pair(it->first, TRForOneOutput));
    }
}


// Currently InterestedOPs are read from a file outputs.txt
void CSymbSimulator::getInterestedOPs(set<string> &InterestedOPs)
{
  bool old_implementation = false;
  if(old_implementation)
  {
	  ifstream *infile;
	  infile = new ifstream();
	  infile->open("outputs.txt");
	  while(!infile->eof())
	    {
	      string s;
	      *infile>>s;
	      if(s=="")
		break;
	      cout<<"Line read is "<<s<<endl;
	      InterestedOPs.insert(s);
	    }
	  infile->close();
  }
  else
  {
	  ifstream *infile;
	  infile = new ifstream();
	  infile->open("outputs-signals.txt");
	  while(!infile->eof())
	    {
	      string s;
	      *infile>>s;
	      if(s=="")
		break;
	      //cout<<"Line read is "<<s<< " " << endl;

	      for(int i=1; i<=TimesOfUnrolling;i++)
		{
		  string signal_index=s;
		  char index[10];
		  sprintf(index, "%d", i);
		  string index_str(index);
		  signal_index += "_";
		  signal_index += index_str;
		  InterestedOPs.insert(signal_index);
		}	      
	    }
	  infile->close();
		
  }

}


void CSymbSimulator::obtainHierarchy(vector<string> &hierarchy, CEntity &ent, string prefix)
{
  //cout<<"\nprefix = "<<prefix<<endl;
  list<CInstance *> &instanceNodes = ent.getInstances();
  list<CInstance *>::iterator iNodes = instanceNodes.begin();

  for(iNodes = instanceNodes.begin(); iNodes != instanceNodes.end(); iNodes++)
    {
      string instance = prefix;
      instance+=(*iNodes)->getEntName();
      instance+="__";
      instance+=(*iNodes)->getInstName();
      
      string myname=instance;
      myname+="__";
      int index = (*iNodes)->getIndex();
      CEntity &comp = Entities[index];
      //cout<<"\nmyname = "<<myname<<endl;
      //cout<<"\ninstance = "<<instance<<endl;
      obtainHierarchy(hierarchy, comp, myname);

      hierarchy.push_back(instance);
    }
}
  

 void CSymbSimulator::getInstanceNameAndSignalName(string &NeededInstance, string &NeededSignal, string signal)
 {
  int index = signal.rfind("__");
  if(index==string::npos)
    {
      NeededSignal = signal;
      NeededInstance = "";
    }
  else
    {
      NeededInstance = signal.substr(0, index);
      NeededSignal = signal.substr(index+2);
   }
}



CDAGNode* CSymbSimulator::eliminateTemporaryVariablesOfTRCreation(CDAGNode *TR)
{
  set<string> Vars, VarsToElim;
  
  TR->getDAGNames(Vars);

  for(set<string>::iterator it=Vars.begin(); it!=Vars.end(); it++)
   {
     string signal = *it;
     if(signal[0]=='X') VarsToElim.insert(signal);
   }
  
  //cout<<"\nVarsToElim\n";
  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
   {
     //cout<<*it<<endl;
   }
  
  if(VarsToElim.empty())
    return TR;
  else
    {
      TR = MANAGER.createDAG("exists", getDAGFromStringSet(VarsToElim), TR);
      TR = MANAGER.Global_Inlining(TR);

      // This is needed due to some small bug in Inlining code : Given exists(x, x=f(y) /\ R(x,c)), it returns exists(x, R(f(y),c)). Hence just return the right part of exists
      if(TR->Get_DAGnode_value()!= "exists") return TR;
      else
	{
	  list<CDAGNode*>::iterator iout = TR->Get_DAGnode_outlist();
	  iout++;
	  TR = *iout;
	  return TR;
	}
    }
}


CDAGNode* CSymbSimulator::eliminateTrivialEqualities(CDAGNode *TR)
{
  CDAGNode *SimplifiedTR=NULL;

  map<string, CDAGNode*> FunctionMap;
  set<CDAGNode*> SimplifiedFunctions;

  MANAGER.createFunctionMap(TR, FunctionMap);
  
  for(map<string, CDAGNode*>::iterator it=FunctionMap.begin(); it!=FunctionMap.end(); it++)
    {
      // Take the function
      CDAGNode *Function = it->second;
      // Take lhs and rhs
      list<CDAGNode*>::iterator iout = Function->Get_DAGnode_outlist();
      CDAGNode* lhs = *iout;
      iout++;
      CDAGNode *rhs = *iout;
      if(lhs!=rhs) 
	SimplifiedFunctions.insert(Function);
    }
      
  for(set<CDAGNode*>::iterator it=SimplifiedFunctions.begin(); it!=SimplifiedFunctions.end(); it++)
    {
      if(SimplifiedTR==NULL) SimplifiedTR=*it;
      else SimplifiedTR=MANAGER.createDAG("and", SimplifiedTR, *it);
    }
  return SimplifiedTR;
}
  

void CSymbSimulator::createInstanceFunctionMap(map<string, map<string, CDAGNode*> > &InstanceFunctionMap, vector<string> &hierarchy, CDAGNode *TR, CEntity &ent)
{
  map<string, CDAGNode*> FunctionMap;
  MANAGER.createFunctionMap(TR, FunctionMap);

//   cout<<"\nFunction Map\n";
//   for(map<string, CDAGNode*>::iterator it=FunctionMap.begin(); it!=FunctionMap.end(); it++)
//        {
// 	 cout<<"\nsignal = "<<it->first<<" , rhs = "<<(it->second)->getDAGStr()<<"\n";
// 	}
  
  //cout<<"\nCreating the InstanceFunctionMap\n";

  int i=0;
  for(; i<hierarchy.size()-1; i++)
    {
      string instance = hierarchy[i];
      map<string, CDAGNode*> InstanceMap;
      for(map<string, CDAGNode*>::iterator it=FunctionMap.begin(); it!=FunctionMap.end(); it++)
	{
	  string signal = it->first;
	  int index = signal.find_last_of("_");
	  string signal_name = signal.substr(0, index);
	  string delta_string = signal.substr(index+1);

	  if(delta_string!="1")
	    {
	      cout<<"\nWarning!Illegal transition function for "<<signal<<" inside CSymbSimulator::createInstanceFunctionMap\n";
	      continue;
	    }


	  CDAGNode *Function = it->second;
	  
	  
	  if(signal_name.find(instance)!=string::npos) // signal's name contains instance's name => signal is with in this instance
	    InstanceMap.insert(make_pair(signal_name, Function));
	}
      
      InstanceFunctionMap.insert(make_pair(instance, InstanceMap));
    }     

  // Every signal is included in the top-level

  string instance = hierarchy[i];

  map<string, CDAGNode*> InstanceMap;
  for(map<string, CDAGNode*>::iterator it=FunctionMap.begin(); it!=FunctionMap.end(); it++)
	{
	  string signal = it->first;
	  int index = signal.find_last_of("_");
	  string signal_name = signal.substr(0, index);

	  string delta_string = signal.substr(index+1);

	  if(delta_string!="1")
	    {
	      cout<<"\nWarning!Illegal transition function for "<<signal<<" inside CSymbSimulator::createInstanceFunctionMap\n";
	      continue;
	    }

	  

	  CDAGNode *Function = it->second;
	  InstanceMap.insert(make_pair(signal_name, Function));
	}
      
 InstanceFunctionMap.insert(make_pair(instance, InstanceMap));
  
}


void CSymbSimulator::findInterestedSEsOfInstance(map<string, CDAGNode*> &InterestedSEsOfInstance, string InstanceName, set<string> InterestedSet, map<string, map<string, CDAGNode*> > &InstanceFunctionMap)
{
  map<string, CDAGNode*> SEsOfInstance;

  SEsOfInstance=InstanceFunctionMap[InstanceName];
  
  for(map<string, CDAGNode*>::iterator it=SEsOfInstance.begin(); it != SEsOfInstance.end(); it++)
    {
      string signal=it->first;
      CDAGNode *Function = it->second;
      
      if(InterestedSet.find(signal)!=InterestedSet.end()) //  signal is interested
	InterestedSEsOfInstance.insert(make_pair(signal, Function));
    }
}
 


void CSymbSimulator::getWidthTableWithDP(CDAGNode* dag, string TopLevelEntity, map<string, int> &WidthTable)
{
  set<string> Variables;
   
  set<CDAGNode*> DPTable;
  dag->getDAGNamesOfDAG_WithNoExists_WithDP(Variables, &DPTable);
  DPTable.clear();

  for(set<string>::iterator it = Variables.begin();  it != Variables.end(); it++)
    {
      string variable = *it;
      int width = getWidthOfVariable(variable, TopLevelEntity);
      WidthTable.insert(make_pair(variable, width));
    }
}




void CSymbSimulator::createFrames_and_Print_in_Yices_Format_Modified(vector<string> &hierarchy, map<int, set<string> > &FRAMEMAP, map<string, map<string, CDAGNode*> > &InstanceFunctionMap, CEntity &ent, string property, list<int> sortedIndices, set<string> &PropertySupport)
{
  map<string, list<int> > Blast_Table;
  map<string, list<int> > *Ptr_Blast_Table = &Blast_Table;// Table for blasting scheme

  set<int> Blast_Table_Hits;
  set<int> Blast_Table_Misses;

  string yices_file;
  yices_file = "verification_condition.smt";

  if(qelim_bit_level)
    {
      FILE* yices_fp = fopen(yices_file.c_str(),"w");
      if(yices_fp==NULL)
	{
	  cout<<"\nCannot open " << yices_file << endl;
	  exit(1);
	}
      if(solver_for_bmc=="smtlib")
	{
	  fprintf(yices_fp,"(benchmark vc\n");
	}  
      fclose(yices_fp);

      readBlast_Table_Hits_Misses(Blast_Table_Hits, Blast_Table_Misses);
    }



  HashTable <string, CDAGNode*> HTAcrossFrames;
  HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames = &HTAcrossFrames;

  map<int, CDAGNode*> FRAME_SEMAP;


  map<string, string> SimplificationTable;

  getSimplificationTable(SimplificationTable);

  //showSimplificationTable(SimplificationTable);

  for(map<int, set<string> >::iterator it=FRAMEMAP.begin(); it!=FRAMEMAP.end(); it++)
    {
      map<string, CDAGNode*> InstanceMap;
      
      set<string> InterestedSet = it->second;

      
      //cout<<"\nOutputs interested in frame # "<<it->first<<" are \n";
      for(set<string>::iterator it2=InterestedSet.begin();it2!=InterestedSet.end();it2++)
	{
	  //cout<<*it2<<"\t";
	}
      //cout<<"\n";

      
      for(int i=0; i<hierarchy.size(); i++)
	{
	  string InstanceName = hierarchy[i];

	  CDAGNode *SEOfInstance=NULL;

	  //cout<<"\nInstanceName = "<<InstanceName<<endl;

	  map<string, CDAGNode*> InterestedSEsOfInstance;
	 
	  findInterestedSEsOfInstance(InterestedSEsOfInstance, InstanceName, InterestedSet, InstanceFunctionMap);

	  if(InterestedSEsOfInstance.empty()) // No signals of this instance are interested in the current frame. Hence write true in the InstanceMap. 
	    {
	      //cout<<"\nInterestedSignalsOfInstance is empty\n";
	      SEOfInstance = MANAGER.createDAG("true");
	      //cout<<"\nSEInstance = "<<SEOfInstance->getDAGStr()<<endl;
	    }
	  else //Else go down and construct it properly
	    {
	      
	      //cout<<"\nInterestedSEsOfInstance"<<endl;
	      //for(map<string, CDAGNode*>::iterator sit=InterestedSEsOfInstance.begin(); sit!=InterestedSEsOfInstance.end(); sit++)
	      //	{cout<<endl<<sit->first<<"\t"<<(sit->second)->getDAGStr();}

	      //cout<<endl;
	      //cout<<"\nTaking each of them\n";

	    for(map<string, CDAGNode*>::iterator it3=InterestedSEsOfInstance.begin(); it3!=InterestedSEsOfInstance.end(); it3++)
	       {
		 string signal = it3->first;

		 //cout<<"\nsignal "<<signal<<"\n";

		 string NeededInstance, NeededSignal;
		 getInstanceNameAndSignalName(NeededInstance, NeededSignal, signal);

		 //cout<<"\nNeededInstance "<<NeededInstance<<", NeededSignal = "<<NeededSignal<<"\n";
		 
		 if(NeededInstance==InstanceName || NeededInstance=="") // from this instance. Pick up from InterestedSEsOfEntity. NeededInstance=="" is for signals at the top-level
		   {
		     //cout<<"\nFrom this entity\n";
		     if(SEOfInstance==NULL)
		       SEOfInstance=it3->second;
		     else
		       SEOfInstance=MANAGER.createDAG("and", SEOfInstance, it3->second);
		     //cout<<"\nSEInstance = "<<SEOfInstance->getDAGStr()<<endl;
		   }
		 else // from lower level
		   {
		     //cout<<"\nFrom some lower level entity\n";
		     map<string, CDAGNode*>::iterator InstanceMap_it = InstanceMap.find(NeededInstance);
		     if(InstanceMap_it == InstanceMap.end())
		       {
			 cout<<"\nIt seems instances are not traversed in proper order\n";
			 exit(1);
		       }
		     else
		       {

			  CDAGNode *SE = InstanceMap_it->second;
			  //cout<<"\nSE = "<<SE->getDAGStr()<<endl;

			 if(SEOfInstance==NULL)
			   SEOfInstance=SE;
			 else
			   SEOfInstance=MANAGER.createDAG("and", SEOfInstance, SE);
			 //cout<<"\nSEInstance = "<<SEOfInstance->getDAGStr()<<endl;
			 
			 
		       } 
		   }
	       }
	    }

	  if(qelimination_on==1) // We should eliminate internal variables of the instance before eliminating
	    {

	      if(!qelim_bit_level)// QElimination using QuaLMoDE
		{
		  set<string> VarsToElim;
		  chooseVarsToElim(VarsToElim, InstanceName, ent, sortedIndices, SEOfInstance, it->first);
		  //showSet(VarsToElim, "VarsToElim");
		  //cout<<"\nInstanceName = "<<InstanceName<<endl;
	      
		  //cout<<"\nSEOfInstance = "<<SEOfInstance<<endl;
		  CDAGNode* Existing  = getEntryFromHTAcrossFramesModified(SEOfInstance, VarsToElim, Ptr_HTAcrossFrames);
		  CDAGNode* SE_Orig = SEOfInstance;

		  if(Existing != NULL)
		    {
		      //cout<<"\nTable hit\n";
		      //cout<<"\nResult returned = "<<createDAGFromDD(Existing, MANAGER)->getDAGStr()<<endl;
		      SEOfInstance = Existing;
		      appendEntryIntoBlast_Table(SE_Orig, VarsToElim, it->first-1, Ptr_Blast_Table);
		    }
		  else
		    {
		      //cout<<"\nTable miss\n";
		      CDAGNode* SEOfInstance_Orig = SEOfInstance;
		      //cout<<"\nSEOfInstance = "<<SEOfInstance->getDAGStr()<<endl;
		      SEOfInstance = applyQuantifierElimination(SEOfInstance, VarsToElim, ent, it->first-1);
		      //insertEntryIntoHTAcrossFrames(InstanceName, VarsToElim, SEOfInstance, Ptr_HTAcrossFrames);
		      //cout<<"\nSEOfInstance After QElim = "<<SEOfInstance->getDAGStr()<<endl;

		      insertEntryIntoHTAcrossFramesModified(SEOfInstance_Orig, VarsToElim, SEOfInstance, Ptr_HTAcrossFrames);
		      insertEntryIntoBlast_Table(SEOfInstance_Orig, VarsToElim, it->first-1, Ptr_Blast_Table);
		    }
		}//if(!qelim_bit_level) ends here
	      else // QElimination using QuaLMoDE with Blasting
		{
		  set<string> VarsToElim;
		  chooseVarsToElim(VarsToElim, InstanceName, ent, sortedIndices, SEOfInstance, it->first);
		  //showSet(VarsToElim, "VarsToElim");
		  //cout<<"\nInstanceName = "<<InstanceName<<endl;
	      
		  //cout<<"\nSEOfInstance = "<<SEOfInstance<<endl;
		  //CDAGNode* Existing  = getEntryFromHTAcrossFramesModified(SEOfInstance, VarsToElim, Ptr_HTAcrossFrames);
		  //CDAGNode* SE_Orig = SEOfInstance;

		  if(Blast_Table_Hits.find(it->first-1)!=Blast_Table_Hits.end())// This is a hit
		    {
		      //cout<<"\nTable hit for "<<it->first-1<<"\n";
		      //cout<<"\nResult returned = "<<createDAGFromDD(Existing, MANAGER)->getDAGStr()<<endl;
		      //SEOfInstance = Existing;
		      //appendEntryIntoBlast_Table(SE_Orig, VarsToElim, it->first, Ptr_Blast_Table);

		      SEOfInstance = MANAGER.createDAG("abstraction", 0);
		    }
		  else
		    {
		      //cout<<"\nTable miss\n";
		      //cout<<"\nTable miss for "<<it->first-1<<"\n";
		      CDAGNode* SEOfInstance_Orig = SEOfInstance;
		      //cout<<"\nSEOfInstance = "<<SEOfInstance->getDAGStr()<<endl;
		      SEOfInstance = applyQuantifierElimination(SEOfInstance, VarsToElim, ent, it->first-1);
		      //insertEntryIntoHTAcrossFrames(InstanceName, VarsToElim, SEOfInstance, Ptr_HTAcrossFrames);
		      //cout<<"\nSEOfInstance After QElim = "<<SEOfInstance->getDAGStr()<<endl;

		      //insertEntryIntoHTAcrossFramesModified(SEOfInstance_Orig, VarsToElim, SEOfInstance, Ptr_HTAcrossFrames);
		      //insertEntryIntoBlast_Table(SEOfInstance_Orig, VarsToElim, it->first, Ptr_Blast_Table);
		    }
		}// QElimination using QuaLMoDE with Blasting ends here
	    }

	  else // convertExpressionToProperFormula is called to convert the dag to propositional skelton format since our procedure to print in Yices can handle only dags in propositional skelton format : Note that this is not the correct solution --> we need to make the procedure CHelper::printInYicesFormat such that it prints dags which are not in prop.skelton format also 
	    { 
	      set<string> VarsToElim;
	      VarsToElim.clear();
	      //cout<<"\nSEOfInstance = "<<SEOfInstance<<endl;
	      CDAGNode* Existing  = getEntryFromHTAcrossFramesModified(SEOfInstance, VarsToElim, Ptr_HTAcrossFrames);
	      if(Existing != NULL)
		{
		  //cout<<"\nTable hit\n";
		  //cout<<"\nResult returned = "<<createDAGFromDD(Existing, MANAGER)->getDAGStr()<<endl;
		  SEOfInstance = Existing;
		}
	      else
		{
		  //cout<<"\nTable miss\n";
		  CDAGNode* SEOfInstance_Orig = SEOfInstance;
		  SEOfInstance = convertExpressionToProperFormula(SEOfInstance, "rules_proper.txt"); // rules_proper.txt has only those rules to convert dag to prop.skelton format : Ideally speaking, we need to apply only the simplification rules here
		  SEOfInstance = convertExpressionToProperFormula(SEOfInstance, "rules_proper.txt"); 
		  insertEntryIntoHTAcrossFramesModified(SEOfInstance_Orig, VarsToElim, SEOfInstance, Ptr_HTAcrossFrames);
		}	     
	    }

	  //cout<<"\nInserting into Instance Map\n";
	  InstanceMap.insert(make_pair(InstanceName, SEOfInstance));
	}
     
      FRAME_SEMAP.insert(make_pair(it->first, InstanceMap[ent.getName()]));
    }


//   cout<<"\nFRAME_SEMAP\n***************\n";
//   for(map<int, CDAGNode*>::iterator it=FRAME_SEMAP.begin(); it!=FRAME_SEMAP.end(); it++)
//     {
//       cout<<"\n"<<it->first<<"\n"<<(it->second)->getDAGStr()<<endl;
//     }
//   cout<<"\n******************\n";

  

  //cout<<"\nPrinting in Yices file\n***************\n";
  //cout<<"\nCreating the width tables and TRs\n***************\n";

  //vector<map<string, int> > WidthTables;

  map<string, int> WidthTable;
  vector<CDAGNode*> UnrolledTRs;
  
  for(map<int, CDAGNode*>::iterator it=FRAME_SEMAP.begin(); it!=FRAME_SEMAP.end(); it++)
    {
      int frame = it->first;
      CDAGNode *TR = it->second;
            
      //cout<<"\nShifting properly before printing\n";

      TR = MANAGER.callCloneAndShiftWithDP(TR, frame-1);

      //cout<<"\nShifted TR = "<<TR->getDAGStr()<<endl;

      TR = MANAGER.callQuickSimplify(TR, SimplificationTable);

      //cout<<"\nAfter property-based simplification  = "<<TR->getDAGStr()<<endl;
      

      bool convert_tr_to_proper_form=false;
      if(convert_tr_to_proper_form)
	  {
	    TR = convertExpressionToProperFormula(TR, "rules_dnf.txt"); 
	    //cout<<"\nAfter conversion to prop skelton  = "<<TR->getDAGStr()<<endl;
	  }

      




      //cout<<"\nPrinting in Yices format\n";

      // Function to print in Yices format. Note : print variables also

      
      //cout<<"\nGetting the width table and printing in Yices\n";

      //get the declarations

      getWidthTableWithDP(TR, ent.getName(), WidthTable);


      UnrolledTRs.push_back(TR);
 
    }


  //cout<<"\nWidth tables and TRs created\n***************\n";
  //cout<<"\nWriting in Yices file\n***************\n";
  

  //cout<<"\nWriting the formula\n***************\n";
  

  

  int TempCount=0;

  vector<string> RootStrings;

  map<CDAGNode*, string> LabelTable;

  FILE *yices_fp;

  if(qelim_bit_level)
    {
      yices_fp = fopen(yices_file.c_str(),"a+");
    }
  else
    {
      yices_fp = fopen(yices_file.c_str(),"w");
    }

  if(yices_fp==NULL)
  {
    cout<<"\nCannot open " << yices_file << endl;
    exit(1);
  }


  if(!qelim_bit_level)
    {
      if(solver_for_bmc=="smtlib")
	{
	  fprintf(yices_fp,"(benchmark vc\n");
	}
    }
  
  time_t unroll_begin_time, unroll_over_time, unroll_start_time, unroll_end_time;

  time(&unroll_begin_time);

  set<string> SupportOfUnrolling; // Support of the unrolled portion

  for(unsigned int i=0; i<UnrolledTRs.size(); i++)
    {
      time(&unroll_start_time);

     //cout<<"\nWriting Frame "<<i<<"\n";
     
      // Finding the size of dag
     set<CDAGNode*> TempSet;
     int excnt=0, count=0;
     TempSet.clear();
     UnrolledTRs[i]->getSize(excnt, count, TempSet);
     //cout<<"\nSize of UnrolledTRs["<<i<<"]  = "<<count<<endl;

     
     //cout<<"\nObtaining the Yices string\n";
     string yices_str = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(UnrolledTRs[i], WidthTable, TempCount, yices_fp, LabelTable, SupportOfUnrolling); 
     //cout<<"\nYices string obtained\n";

     RootStrings.push_back(yices_str);

     //cout<<"\nFrame "<<i<<" written\n";


     time(&unroll_end_time);
     //cout<<"\nUnrolling Time "<<unroll_end_time-unroll_start_time<<" seconds \n";


    }

  time(&unroll_over_time);
  //cout<<"\nTotal Unrolling Time "<<unroll_begin_time-unroll_over_time<<" seconds \n";

  //showSet(SupportOfUnrolling, "SupportOfUnrolling");
  //showSet(PropertySupport, "PropertySupport");

  set<string> UndeclaredVariablesInProperty;

  set_difference(PropertySupport.begin(), PropertySupport.end(), SupportOfUnrolling.begin(), SupportOfUnrolling.end(),inserter(UndeclaredVariablesInProperty, UndeclaredVariablesInProperty.begin()));

  //showSet(UndeclaredVariablesInProperty, "UndeclaredVariablesInProperty");


  if(solver_for_bmc=="yices")
    {
      // declare the remaining variables

      for(set<string>::iterator uvit = UndeclaredVariablesInProperty.begin(); uvit != UndeclaredVariablesInProperty.end(); uvit++)
        {
	  string variable = *uvit;
	  int width = getWidthOfVariable(variable, ent.getName());

	  fprintf(yices_fp,"(define %s",variable.c_str());
	  fprintf(yices_fp," :: (bitvector %d))\n",width);
	}
      
      

      string final_string="\n(assert (and ";
      for(unsigned int i=0; i<RootStrings.size(); i++){
	final_string += RootStrings[i];
	final_string += " ";
      }
  
      final_string += property;
      final_string += "))\n";
  
      fprintf(yices_fp,"%s", final_string.c_str());
  
 
      fprintf(yices_fp,"\n(check)\n");
    }

  else if(solver_for_bmc=="smtlib")
    {

      // declare the remaining variables

      for(set<string>::iterator uvit = UndeclaredVariablesInProperty.begin(); uvit != UndeclaredVariablesInProperty.end(); uvit++)
        {
	  string variable = *uvit;
	  int width = getWidthOfVariable(variable, ent.getName());

	  fprintf(yices_fp,":extrafuns ((%s",variable.c_str());
	  fprintf(yices_fp," BitVec[%d]))\n",width);

	}


      string final_string="\n:formula (and ";
      for(unsigned int i=0; i<RootStrings.size(); i++){
	final_string += RootStrings[i];
	final_string += " ";
      }
  
      final_string += property;
      final_string += ")\n";
  
      fprintf(yices_fp,"%s", final_string.c_str());
  
 
      fprintf(yices_fp,"\n)\n");
    }
  else
    {
      cout<<"\nUnknown solver for bmc "<<solver_for_bmc<<endl;
      exit(1);
    }

  fclose(yices_fp);

  //cout<<"\nYices file closed\n";


  //   cout<<"\nPrinting Yices file finished\n";
//   cout<<"\nPrinting Yices file finished\n";
//   cout<<"\nPrinting Yices file finished\n";
//   cout<<"\nPrinting Yices file finished\n";

  if(!qelim_bit_level)
    {

      FILE* blast_table_fp = fopen("blast_table.txt","w");

      if(blast_table_fp==NULL)
	{
	  cout<<"\nCannot open blast_table.txt\n";
	  exit(1);
	}

      for(map<string, list<int> >::iterator btit=Blast_Table.begin(); btit!=Blast_Table.end(); btit++)
	{
	  list<int> Frames = btit->second;
      
	  for(list<int>::iterator lit=Frames.begin(); lit!=Frames.end(); lit++)
	    {
	      fprintf(blast_table_fp,"%d\t", *lit);
	    }

	  fprintf(blast_table_fp,"\n");
	}

      fclose(blast_table_fp);
    }


}



CDAGNode* CSymbSimulator::applyQuantifierElimination(CDAGNode *root, set<string> &VarsToElim, CEntity &ent, int frame_number)
{
  struct timeval total_qe_time_start_ms, total_qe_time_end_ms;
  unsigned long long int total_qe_time_duration_ms;
  gettimeofday(&total_qe_time_start_ms, NULL);
 

  if(root == NULL) {
    cout<<"\n\nFINAL DAG NULL in function CSymbSimulator::applyQuantifierElimination\n\n";
    exit(1); }

  if(!VarsToElim.empty())
				
    {
      
      //showSet(VarsToElim, "VarsToElim");
      //cout<<"\nroot = "<<root->getDAGStr()<<endl;
      
      root = MANAGER.createDAG("exists", getDAGFromStringSet(VarsToElim), root);
				    
    }
  else
    {
      //cout<<"\nNO VARIABLES TO ELIMINATE = "<<endl;
    }

  //cout<<"\nBefore inlining = "<<root->getDAGStr()<<endl;
	
  CDAGNode *temp;
  bool InliningEnabled = true;
  if(InliningEnabled)
    {
      temp = MANAGER.Global_Inlining(root);
    }
  else
    {
      temp = root;
    }

  
				    

  //cout<<"\nAfter inlining = "<<temp->getDAGStr()<<endl;


  temp = convertExpressionWithExistsToProperFormula(temp);

  //cout<<"\nAfter Expression Simplification  = "<<temp->getDAGStr()<<endl;


  //cout<<"\nGetting widths of the variables\n";

  // Get the widths of the variables

  map<string, int> WidthTable;

  getWidthTable(temp, ent.getName(), WidthTable);


  // Remove the unneeded selections

  //cout<<"\nRemoving unneeded selections\n";
				   
  temp = removeUnneededSelects(temp, WidthTable);

  //cout<<"\nDag after removing unneeded selections = "<<temp->getDAGStr()<<endl;

  temp = simplifyFormulaWithExists(temp, "rules_simplify.txt");
				   
  //cout<<"\nDag after simplification = "<<temp->getDAGStr()<<endl;


  if(!qelim_bit_level)  
    {
      
      bool make_calls_to_qualmode_independent = true;

      if(make_calls_to_qualmode_independent) // give different names to the variables in the dags in
	// each call to Qualmode. This avoids order problems arising out of common variables existing
	// between calls to Qualmode.
	{

	  bool hashing_enabled = true;

	  if(hashing_enabled)
	    {
	      temp = callQualmode(temp, WidthTable);
	    }
	  else
	    {

	      map<string, string> RenameMap;
	  
	      map<string, string> ReverseRenameMap;

	      map<string, int> RenamedWidthTable;
      
	      createRenameMaps(temp, RenameMap, ReverseRenameMap);

	      createRenamedWidthTable(WidthTable, RenamedWidthTable);


	      //cout<<"\nOriginal DAG = "<<temp->getDAGStr()<<endl;


	      CDAGNode *RenamedDAG = MANAGER.cloneWithRename(temp, RenameMap);

	      //cout<<"\nRenamedDAG = "<<RenamedDAG->getDAGStr()<<endl;

	        
	      CDAGNode *RenamedTemp = Quantifier_Eliminator_With_Redundant_Selection_Removal(RenamedDAG, RenamedWidthTable);


	      //cout<<"\nRenamedTemp = "<<RenamedTemp->getDAGStr()<<endl;

	      temp = MANAGER.cloneWithRename(RenamedTemp, ReverseRenameMap);

	      //cout<<"\nFinal DAG = "<<temp->getDAGStr()<<endl;

	      qualmode_call_number++;
	    }// else of hashing_enabled


	}// if of make_calls_to_qualmode_independent

      else
	{
	  temp = Quantifier_Eliminator_With_Redundant_Selection_Removal(temp, WidthTable);
	}// else of make_calls_to_qualmode_independent

    }// if(!qelim_bit_level) 
  else
    {
      temp = Qualmode_Blasting(temp, WidthTable, frame_number);
    }// else of if(!qelim_bit_level) 

  //cout<<"\nCalling the Qunatifier Eliminator Finished\n";

  //cout<<"\nDag after Qunatifier Elimination  = "<<temp->getDAGStr()<<endl;


  gettimeofday(&total_qe_time_end_ms, NULL);
  total_qe_time_duration_ms = total_qe_time_end_ms.tv_sec * 1000 + total_qe_time_end_ms.tv_usec / 1000;
  total_qe_time_duration_ms -= total_qe_time_start_ms.tv_sec * 1000 + total_qe_time_start_ms.tv_usec / 1000;        
  total_qe_time = total_qe_time + total_qe_time_duration_ms;

  
  return temp;  


}


void CSymbSimulator::getEntityHandler(CEntity &MyEntity, list<int> sortedIndices, string EntityName)
{
  list<int>::iterator isi = sortedIndices.begin();
  bool flag=false;
                                                                                               
  for(; isi != sortedIndices.end(); isi++)
    {
      MyEntity = Entities[*isi];
                                                                                           
      if(MyEntity.getName()==EntityName) 
	{
	  flag=true;
	  break;
	}
    }
  
  if(!flag)
    {
      cout<<"\nNo handler found for entity "<<EntityName<<" in function CSymbSimulator::getEntityHandler\n";
      exit(1);
   }
}

void CSymbSimulator::chooseVarsToElim(set<string> &VarsToElim, string InstanceName, CEntity &ent, list<int> sortedIndices, CDAGNode *dag, int FrameNumber)
{
  // This function chooses the variables to eliminate. Currently it returns the set of all internal variables of the entity
  set<string> intermediate_names;

  set<string> input_names, output_names;

  if(InstanceName == ent.getName()) // we are at top-level
    {
      set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
      getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
      getNamesFromCSensitiveElementSet(intermediate_identifiers, intermediate_names);

      // newly added
       getNamesFromCSensitiveElementSet(input_identifiers, input_names);
       getNamesFromCSensitiveElementSet(output_identifiers, output_names);
      
    }

  else
    {
      CEntity MyEntity;
      string EntityName;
      int index = InstanceName.rfind("__");
      EntityName = InstanceName.substr(0, index);
      index = EntityName.rfind("__");
      if(index!=string::npos)
	{
	  EntityName = EntityName.substr(index+2);
	}
      getEntityHandler(MyEntity, sortedIndices, EntityName);

      set<string> temp_intermediate_names;

      set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
      getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,MyEntity);
      getNamesFromCSensitiveElementSet(intermediate_identifiers, temp_intermediate_names);

      // Attach InstanceName at the front to get actual name

      for(set<string>::iterator it=temp_intermediate_names.begin();it!=temp_intermediate_names.end();it++)
	{
	  string name = *it;
	  string new_name = InstanceName;
	  new_name += "__";
	  new_name += name;
	  intermediate_names.insert(new_name);
	}

      // newly added
      getNamesFromCSensitiveElementSet(input_identifiers, input_names);
      getNamesFromCSensitiveElementSet(output_identifiers, output_names);

    }

  //cout<<"\nIntermediate names of instance "<<InstanceName<<endl;
  //showSet(intermediate_names, "intermediate_names");

  //cout<<"\nInput names of instance "<<InstanceName<<endl;
  //showSet(input_names, "input_names");

  //cout<<"\nOutput names of instance "<<InstanceName<<endl;
  //showSet(output_names, "output_names");

  // added on 14.5.2010 
  set<string> UnquantifiedSignals;
  getUnquantifiedSignals(UnquantifiedSignals);
  //showSet(UnquantifiedSignals, "UnquantifiedSignals");

  //cout << "\nUnquantifiedSignals\n";
  //for(set<string>::iterator it=UnquantifiedSignals.begin(); it!=UnquantifiedSignals.end(); it++)
  //{
  //	cout << *it << endl;
  //}
  // added on 14.5.2010  ends 

  //cout<<"\nSignal names\n";


  set<string> Vars;
  dag->getDAGNames(Vars);
  for(set<string>::iterator it=Vars.begin(); it!=Vars.end(); it++)
    {
      string signal = *it;
      int index = signal.find_last_of("_");
      string signal_name = signal.substr(0, index);
      int delta = atoi(signal.substr(index+1).c_str());

      if(intermediate_names.find(signal_name)!=intermediate_names.end())
	{ 

	  int frame = delta + FrameNumber - 1;

	  char frame_char[50];
	  sprintf(frame_char, "%d", frame);
	  string frame_str(frame_char);

	  string signal_frame;
	  
	  signal_frame = signal_name;
	  signal_frame += "_";
	  signal_frame += frame_str;


	  if(UnquantifiedSignals.find(signal_name) == UnquantifiedSignals.end())
	    {
	        VarsToElim.insert(signal);
	        //cout<<signal<<"inserted\n";

            }
	}

      // newly added
      string OriginalSignalName = signal_name;
      index = signal_name.rfind("__");
      if(index != string::npos) signal_name = signal_name.substr(index+2);

      //cout<<signal_name<<endl;

      if(input_names.find(signal_name)==input_names.end() && output_names.find(signal_name)==output_names.end())// not an input id, not an output id
	{
	  int frame = delta + FrameNumber - 1;

	  char frame_char[50];
	  sprintf(frame_char, "%d", frame);
	  string frame_str(frame_char);

	  string signal_frame;
	  
	  signal_frame = OriginalSignalName;
	  signal_frame += "_";
	  signal_frame += frame_str;

	  //cout<<"\nsignal_frame = "<<signal_frame<<endl;

	  if(UnquantifiedSignals.find(signal_name)==UnquantifiedSignals.end())
		    {
		      VarsToElim.insert(signal);
		      //cout<<signal<<"inserted\n";
		    }
	}

	
    }
  
}


void CSymbSimulator::insertEntryIntoHTAcrossFrames(string InstanceName, set<string> VarsToElim, CDAGNode *OutputDag, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames)
{
  // Creating the hash key

  string key;

  key = InstanceName;

  for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }


  // Hash key created
  // Check if entry already exists

  HTStatusValue<CDAGNode*> result = Ptr_HTAcrossFrames->hashtable_search(key);
  
  if(result.success())
    {
      cout<<"\nInside function CSymbSimulator::insertEntryIntoHTAcrossFrames. Entry already exists for this.  Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = Ptr_HTAcrossFrames->hashtable_insert(key, OutputDag);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function CSymbSimulator::insertEntryIntoHTAcrossFrames\n";
      exit(1);
    }
}



CDAGNode* CSymbSimulator::getEntryFromHTAcrossFrames(string InstanceName, set<string> VarsToElim, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames)
{
  // Creating the hash key

  string key;

  key = InstanceName;

  for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }


  // Hash key created

  HTStatusValue<CDAGNode*> result = Ptr_HTAcrossFrames->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}




void CSymbSimulator::insertEntryIntoHTAcrossFramesModified(CDAGNode *SEOfInstance, set<string> VarsToElim, CDAGNode *OutputDag, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames)
{
  // Creating the hash key

  char temp[100];
  string key;

  sprintf(temp, "%x", SEOfInstance);
  string temp_key = temp;
  key=temp_key;

  for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }


  // Hash key created
  // Check if entry already exists

  HTStatusValue<CDAGNode*> result = Ptr_HTAcrossFrames->hashtable_search(key);
  
  if(result.success())
    {
      cout<<"\nInside function CSymbSimulator::insertEntryIntoHTAcrossFrames. Entry already exists for this.  Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = Ptr_HTAcrossFrames->hashtable_insert(key, OutputDag);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function CSymbSimulator::insertEntryIntoHTAcrossFrames\n";
      exit(1);
    }
}



CDAGNode* CSymbSimulator::getEntryFromHTAcrossFramesModified(CDAGNode *SEOfInstance, set<string> VarsToElim, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames)
{

  // Creating the hash key

  char temp[100];
  string key;

  sprintf(temp, "%x", SEOfInstance);
  string temp_key = temp;
  key=temp_key;

  for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }


  // Hash key created

  HTStatusValue<CDAGNode*> result = Ptr_HTAcrossFrames->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}





// Currently InterestedOPs are read from a file outputs.txt
void CSymbSimulator::getSimplificationTable(map<string, string> &SimplificationTable)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("inputs.txt");
  while(!infile->eof())
    {
      string s;
      *infile>>s;
      if(s=="")
	break;
      cout<<"Line read is "<<s<<endl;
      
      int equal_pos = s.find("=");
      string signal_name = s.substr(0, equal_pos);
      cout<<"signal_name "<<signal_name<<endl;
      string value  = s.substr(equal_pos+1);
      cout<<"value "<<value<<endl;

      SimplificationTable.insert(make_pair(signal_name, value));
    }

  infile->close();

}


void CSymbSimulator::showWidthTable(map<string, int> &WidthTable)
{
  cout<<"\nWidth Table\n************\n";
  for(map<string, int>::iterator it=WidthTable.begin(); it!=WidthTable.end(); it++)
    {
      cout<<it->first<<"\t"<<it->second<<endl;
    }
}


void CSymbSimulator::showSimplificationTable(map<string, string> &SimplificationTable)
{
  cout<<"\nSimplificationTable\n";

  
  for(map<string, string>::iterator tit = SimplificationTable.begin(); tit != SimplificationTable.end(); tit++)
	{
	  cout<<"\nSignal = "<<tit->first<<"\tValue = "<<tit->second<<endl;
	}
}
      
 
// Signals which should not be quantified out are read from signals.txt
void CSymbSimulator::getUnquantifiedSignals(set<string> &UnquantifiedSignals)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("signals.txt");
  while(!infile->eof())
    {
      string s;
      *infile>>s;
      if(s=="")
	break;
      //cout<<"Line read is "<<s<<endl;
      UnquantifiedSignals.insert(s);
    }
  infile->close();

}


/* Function added on 14 June 2010 for helping in printing the transition relation in SMTLib format */

void CSymbSimulator::getWidthTableForTR(CDAGNode* TR, string TopLevelEntity, map<string, int> &WidthTableTR)
{

  set<string> Vars;
  TR->getDAGNames(Vars);

  for(set<string>::iterator it=Vars.begin(); it!=Vars.end(); it++)
    {
      string id_delta = *it;
      int width = getWidthOfVariable(id_delta, TopLevelEntity);
      WidthTableTR.insert(make_pair(id_delta, width));
    }
}



string CSymbSimulator::readProperty()
{
  string property="";

  FILE* prop_fp = fopen("property.txt","r");
  if(prop_fp == NULL) 
    {
      cout<<"\nNo property file \n";
      return property;
    }
  
  char c;
  c=fgetc(prop_fp);
  while(c!=EOF)
    {
      property += c;
      c=fgetc(prop_fp);
    }

  fclose(prop_fp);
  
  //cout<<"\nProperty = "<<property<<endl;
  return property;
}


/* Function added on 7 July 2010 for integrating with property translator and GUI */


void  CSymbSimulator::callPropertyTranslator(CEntity &ent)
{
  // create and print the width table
  // get the input & output identifiers of the top-level entity
set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
set<string> input_id_names, output_id_names, inter_id_names;
getNamesFromCSensitiveElementSet(input_identifiers, input_id_names);  
getNamesFromCSensitiveElementSet(output_identifiers, output_id_names);
getNamesFromCSensitiveElementSet(intermediate_identifiers, inter_id_names); 

 map<string, int> WidthTable;
 for(set<string>::iterator it=input_id_names.begin(); it!=input_id_names.end(); it++)
   {
     string Name = *it;

     map<string, SymTabEntry> &ids = SymbolTable[ent.getName()];

     SymTabEntry ste = ids[Name];

     list<string>::iterator ilist = ste.rangeindex.begin();

     string lower, upper;

     if(ilist != ste.rangeindex.end()){

       lower = (*ilist);
     }
     else
       {
	 cout<<"\nEntry for "<<Name<<" not found in symbol table in function CSymbSimulator::callPropertyTranslator\n";
	 exit(1);
       }

     ilist++;

     if(ilist != ste.rangeindex.end()){
       upper = (*ilist);		
     }
     else
       {
	 cout<<"\nEntry for "<<Name<<" not found in symbol table in function CSymbSimulator::callPropertyTranslator\n";
	 exit(1);
       }

     int u = atoi(upper.c_str());

     int l = atoi(lower.c_str());

     int width = u - l + 1;

     WidthTable.insert(make_pair(Name, width));

   }

 for(set<string>::iterator it=output_id_names.begin(); it!=output_id_names.end(); it++)
   {
     string Name = *it;

     map<string, SymTabEntry> &ids = SymbolTable[ent.getName()];

     SymTabEntry ste = ids[Name];

     list<string>::iterator ilist = ste.rangeindex.begin();


     string lower, upper;

     if(ilist != ste.rangeindex.end()){

       lower = (*ilist);
     }
     else
       {
	 cout<<"\nEntry for "<<Name<<" not found in symbol table in function CSymbSimulator::callPropertyTranslator\n";
	 exit(1);
       }

     ilist++;

     if(ilist != ste.rangeindex.end()){
       upper = (*ilist);		
     }
     else
       {
	 cout<<"\nEntry for "<<Name<<" not found in symbol table in function CSymbSimulator::callPropertyTranslator\n";
	 exit(1);
       }


     int u = atoi(upper.c_str());

     int l = atoi(lower.c_str());

     int width = u - l + 1;

     WidthTable.insert(make_pair(Name, width));

   }


 for(set<string>::iterator it=inter_id_names.begin(); it!=inter_id_names.end(); it++)
   {
     string Name = *it;

     map<string, SymTabEntry> &ids = SymbolTable[ent.getName()];

     SymTabEntry ste = ids[Name];

     list<string>::iterator ilist = ste.rangeindex.begin();


     string lower, upper;

     if(ilist != ste.rangeindex.end()){

       lower = (*ilist);
     }
     else
       {
	 continue;
       }

     ilist++;

     if(ilist != ste.rangeindex.end()){
       upper = (*ilist);		
     }
     else
       {
	 continue;
       }


     int u = atoi(upper.c_str());

     int l = atoi(lower.c_str());

     int width = u - l + 1;

     WidthTable.insert(make_pair(Name, width));

   }

 FILE *wtable_fp;
 if ((wtable_fp=fopen("widthtable.txt", "w"))==NULL)
		{
		  cout<<"\nUnable open 	widthtable.txt\n";
		  exit(1);
		}    
	
 
 for(map<string, int>::iterator it=WidthTable.begin(); it!=WidthTable.end(); it++)
   {
     fprintf(wtable_fp, "%s-%d\n", (it->first).c_str(), it->second);
   }
 fclose(wtable_fp);

 
 string file_to_check_for_presence = "property.txt";
 deleteFileIfPresent(file_to_check_for_presence);

 
 string command;
 if(psl_property_on_cycles)
 {
	 command="propTransCycleBased < ";
	 command += propertyfile;
	 command += " -";
	 command += solver_for_bmc;
	 command += " -width_file=widthtable.txt";
	 command += " -output_file=property.txt";
	 command += " -support_file=property_support.txt";
 }
 else
 {
	 command="propTrans < ";
	 command += propertyfile;
	 command += " ";
	 command += solver_for_bmc;	 
	 command += " -output_file=property.txt";
	 command += " -support_file=property_support.txt";
 }
 system(command.c_str());

 bool file_is_present = checkIfFileIsPresent(file_to_check_for_presence);
 if(!file_is_present)
 {
	cout << "\nParse-error in input bounded PSL property\n";
	exit(1);
 }

}


bool CSymbSimulator::checkIfFileIsPresent(string file_to_check_for_presence)
{
	FILE *fp = fopen((char*) file_to_check_for_presence.c_str(), "r");
        if (fp == NULL)
        {
            return false;
        }
	else
	{
		fclose(fp);
		return true;
	}
}
   


void CSymbSimulator::deleteFileIfPresent(string file_to_check_for_presence)
{
	bool file_is_present = checkIfFileIsPresent(file_to_check_for_presence);
	if(file_is_present)
	{
		string command = "rm ";
		command += file_to_check_for_presence;

		system(command.c_str());		
	}
}  

// 

void CSymbSimulator::getDeltaTR(set<CDAGNode*> &SEsOfAllOutputs, CEntity &ent)
{

  //cout<<"\nInside getDeltaTR\n";
  
  CDAGNode *DeltaTR=NULL;
  
  for(set<CDAGNode*>::iterator it=SEsOfAllOutputs.begin();it!=SEsOfAllOutputs.end(); it++)
    {
      CDAGNode *DeltaTR_i = *it;

      //cout<<"\nInitially DeltaTR_i = "<<DeltaTR_i->getDAGStr()<<endl;

      map<string, string> ReplacedVarsMap;

      DeltaTR_i = prepareDAGForOutputtingInYices(DeltaTR_i, ent.getName(), ReplacedVarsMap);

      //cout<<"\nAfter preparing to output, DeltaTR_i = "<<DeltaTR_i->getDAGStr()<<endl;
      
      DeltaTR_i = convertExpressionToProperFormula(DeltaTR_i, "rules_dnf.txt"); // This is also required

      DeltaTR_i = convertExpressionToProperFormula(DeltaTR_i, "rules_dnf.txt"); // This is also required

      DeltaTR_i = convertExpressionToProperFormula(DeltaTR_i, "rules_simplify.txt"); 

      //cout<<"\nIn prop.skelton format, DeltaTR_i = "<<DeltaTR_i->getDAGStr()<<endl;

      if(DeltaTR==NULL)
	{
	  DeltaTR=DeltaTR_i;
	}
      else
	{
	  DeltaTR = MANAGER.createDAG("and", DeltaTR, DeltaTR_i);
	}

    }

      map<string, int> WidthTableDeltaTR;
      getWidthTableForTR(DeltaTR, ent.getName(), WidthTableDeltaTR);

//       map<CDAGNode*, CDAGNode*> SubtractionTable;
//       SubtractionTable.clear();
//       TR = MANAGER.getSubtractionReplacedClone(TR, SubtractionTable, WidthTableTR);
            
      if(false)
      {
	MANAGER.printDeltaTRInFile(DeltaTR, WidthTableDeltaTR);
      }

}


void CSymbSimulator::insertEntryIntoBlast_Table(CDAGNode *SEOfInstance, set<string> VarsToElim, int frame, map<string, list<int> > *Ptr_Blast_Table)
{
  // Creating the hash key

  char temp[100];
  string key;

  sprintf(temp, "%x", SEOfInstance);
  string temp_key = temp;
  key=temp_key;

  for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }


  // Hash key created
  // Check if entry already exists

  map<string, list<int> >::iterator it = Ptr_Blast_Table->find(key);
  if(it!=Ptr_Blast_Table->end())
    {
      cout<<"\nEntry already exists in CSymbSimulator::insertEntryIntoBlast_Table\n";
      exit(1);
    }

  list<int> FrameNos;
  FrameNos.push_back(frame);
  Ptr_Blast_Table->insert(make_pair(key, FrameNos)); 
  
}


void CSymbSimulator::appendEntryIntoBlast_Table(CDAGNode *SEOfInstance, set<string> VarsToElim, int frame, map<string, list<int> > *Ptr_Blast_Table)
{
  // Creating the hash key

  char temp[100];
  string key;

  sprintf(temp, "%x", SEOfInstance);
  string temp_key = temp;
  key=temp_key;

  for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }


  // Hash key created
  // Check if entry already exists

  map<string, list<int> >::iterator it = Ptr_Blast_Table->find(key);
  if(it==Ptr_Blast_Table->end())
    {
      cout<<"\nEntry does not exist in CSymbSimulator::appendEntryIntoBlast_Table\n";
      exit(1);
    }

  (it->second).push_back(frame);
  
}


void CSymbSimulator::readBlast_Table_Hits_Misses(set<int> &Blast_Table_Hits, set<int> &Blast_Table_Misses)
{

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  string infilename = "blast_table.txt";
  fp = fopen(infilename.c_str(), "r");
  if (fp == NULL)
    {
      cout<<"\nError while opening file "<<infilename<<" for reading\n";
      exit(1);
    }
  while ((read = getline(&line, &len, fp)) != -1) {
    //printf("Retrieved line of length %zu :\n", read);
    //printf("%s", line);

    string line_str(line);
    if(line_str.find("\t")!=string::npos)
	  {
	    //cout<<line_str;
	    parse_print_string(line, Blast_Table_Hits, Blast_Table_Misses);    
	  }
   }
  if (line)
    free(line);

  fclose(fp);


  cout<<"\nBlast_Table_Hits\n";
  for(set<int>::iterator it=Blast_Table_Hits.begin(); it!=Blast_Table_Hits.end(); it++)
    {
      cout<<*it<<endl;
    }

  cout<<"\nBlast_Table_Misses\n";
  for(set<int>::iterator it=Blast_Table_Misses.begin(); it!=Blast_Table_Misses.end(); it++)
    {
      cout<<*it<<endl;
    } 
}


void CSymbSimulator::parse_print_string(char *line_char, set<int> &Blast_Table_Hits, set<int> &Blast_Table_Misses)
{
  
  int i=1;
  
  int index=0;

  char word[500];

  int word_index = 0;
  
  while(line_char[index]!='\0')
	  {
	    
	    char read = line_char[index];

	    if(read == '\t' || read == ' ' || read == '\n')
	      {
		if(word_index==0) // empty word
		  {
		    index++;
		  }
		else
		  {
		    word[word_index] = '\0';
		    
		    int frame = atoi(word);

		    if(i==1)
		      {
			Blast_Table_Misses.insert(frame);
		      }
		    else
		      {
			Blast_Table_Hits.insert(frame);
		      }

		    i++;

		    word_index = 0;

		    index++;
		  }
	      }
	    else
	      {
		word[word_index] = line_char[index];

		word_index++;

		index++;
	      }
	      
	    
	  }
}




/* FUNCTIONS ADDED ON 31-01-2011 ONWARDS - FOR IMPLEMENTING REFINEMENT */

/* Function to return the concrete verification condition (CVC): Similar to the function "createFrames_and_Print_in_Yices_Format_Modified" 
   except that 1) it returns the CVC as DAG as well as in UnrolledTRs and WidthTable for the concrete verification condition 2) qelimination_on==0 

Note that CVC here is just conjunction of UnrolledTRs. property is not included
*/

CDAGNode* CSymbSimulator::generateConcreteVerificationCondition(vector<string> &hierarchy, map<int, set<string> > &FRAMEMAP, map<string, map<string, CDAGNode*> > &InstanceFunctionMap, CEntity &ent, string property, list<int> sortedIndices, map<string, int> &WidthTableForCVC, vector<CDAGNode*> &UnrolledTRs)
{
  HashTable <string, CDAGNode*> HTAcrossFrames;
  HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames = &HTAcrossFrames;

  map<int, CDAGNode*> FRAME_SEMAP;


  map<string, string> SimplificationTable;

  getSimplificationTable(SimplificationTable);

  //showSimplificationTable(SimplificationTable);

  for(map<int, set<string> >::iterator it=FRAMEMAP.begin(); it!=FRAMEMAP.end(); it++)
    {
      map<string, CDAGNode*> InstanceMap;
      
      set<string> InterestedSet = it->second;

      
      //cout<<"\nOutputs interested in frame # "<<it->first<<" are \n";
      for(set<string>::iterator it2=InterestedSet.begin();it2!=InterestedSet.end();it2++)
	{
	  //cout<<*it2<<"\t";
	}
      //cout<<"\n";

      
      for(int i=0; i<hierarchy.size(); i++)
	{
	  string InstanceName = hierarchy[i];

	  CDAGNode *SEOfInstance=NULL;

	  //cout<<"\nInstanceName = "<<InstanceName<<endl;

	  map<string, CDAGNode*> InterestedSEsOfInstance;
	 
	  findInterestedSEsOfInstance(InterestedSEsOfInstance, InstanceName, InterestedSet, InstanceFunctionMap);

	  if(InterestedSEsOfInstance.empty()) // No signals of this instance are interested in the current frame. Hence write true in the InstanceMap. 
	    {
	      //cout<<"\nInterestedSignalsOfInstance is empty\n";
	      SEOfInstance = MANAGER.createDAG("true");
	      //cout<<"\nSEInstance = "<<SEOfInstance->getDAGStr()<<endl;
	    }
	  else //Else go down and construct it properly
	    {
	      
	      //cout<<"\nInterestedSEsOfInstance"<<endl;
	      //for(map<string, CDAGNode*>::iterator sit=InterestedSEsOfInstance.begin(); sit!=InterestedSEsOfInstance.end(); sit++)
	      //	{cout<<endl<<sit->first<<"\t"<<(sit->second)->getDAGStr();}

	      //cout<<endl;
	      //cout<<"\nTaking each of them\n";

	    for(map<string, CDAGNode*>::iterator it3=InterestedSEsOfInstance.begin(); it3!=InterestedSEsOfInstance.end(); it3++)
	       {
		 string signal = it3->first;

		 //cout<<"\nsignal "<<signal<<"\n";

		 string NeededInstance, NeededSignal;
		 getInstanceNameAndSignalName(NeededInstance, NeededSignal, signal);

		 //cout<<"\nNeededInstance "<<NeededInstance<<", NeededSignal = "<<NeededSignal<<"\n";
		 
		 if(NeededInstance==InstanceName || NeededInstance=="") // from this instance. Pick up from InterestedSEsOfEntity. NeededInstance=="" is for signals at the top-level
		   {
		     //cout<<"\nFrom this entity\n";
		     if(SEOfInstance==NULL)
		       SEOfInstance=it3->second;
		     else
		       SEOfInstance=MANAGER.createDAG("and", SEOfInstance, it3->second);
		     //cout<<"\nSEInstance = "<<SEOfInstance->getDAGStr()<<endl;
		   }
		 else // from lower level
		   {
		     //cout<<"\nFrom some lower level entity\n";
		     map<string, CDAGNode*>::iterator InstanceMap_it = InstanceMap.find(NeededInstance);
		     if(InstanceMap_it == InstanceMap.end())
		       {
			 cout<<"\nIt seems instances are not traversed in proper order\n";
			 exit(1);
		       }
		     else
		       {

			  CDAGNode *SE = InstanceMap_it->second;
			  //cout<<"\nSE = "<<SE->getDAGStr()<<endl;

			 if(SEOfInstance==NULL)
			   SEOfInstance=SE;
			 else
			   SEOfInstance=MANAGER.createDAG("and", SEOfInstance, SE);
			 //cout<<"\nSEInstance = "<<SEOfInstance->getDAGStr()<<endl;
			 
			 
		       } 
		   }
	       }
	    }//Else go down and construct it properly ends here

	  set<string> VarsToElim;
	  VarsToElim.clear();
	  //cout<<"\nSEOfInstance = "<<SEOfInstance<<endl;
	  CDAGNode* Existing  = getEntryFromHTAcrossFramesModified(SEOfInstance, VarsToElim, Ptr_HTAcrossFrames);
	  if(Existing != NULL)
	    {
	      //cout<<"\nTable hit\n";
	      //cout<<"\nResult returned = "<<createDAGFromDD(Existing, MANAGER)->getDAGStr()<<endl;
	      SEOfInstance = Existing;
	    }
	  else
	    {
	      //cout<<"\nTable miss\n";
	      CDAGNode* SEOfInstance_Orig = SEOfInstance;
	      SEOfInstance = convertExpressionToProperFormula(SEOfInstance, "rules_proper.txt"); // rules_proper.txt has only those rules to convert dag to prop.skelton format : Ideally speaking, we need to apply only the simplification rules here
	      SEOfInstance = convertExpressionToProperFormula(SEOfInstance, "rules_proper.txt"); 
	      insertEntryIntoHTAcrossFramesModified(SEOfInstance_Orig, VarsToElim, SEOfInstance, Ptr_HTAcrossFrames);
	    }	     
	  //cout<<"\nInserting into Instance Map\n";
	  InstanceMap.insert(make_pair(InstanceName, SEOfInstance));
	} // for(int i=0; i<hierarchy.size(); i++) ends here
     
      FRAME_SEMAP.insert(make_pair(it->first, InstanceMap[ent.getName()]));
    }//for(map<int, set<string> >::iterator it=FRAMEMAP.begin(); it!=FRAMEMAP.end(); it++) ends here


//   cout<<"\nFRAME_SEMAP\n***************\n";
//   for(map<int, CDAGNode*>::iterator it=FRAME_SEMAP.begin(); it!=FRAME_SEMAP.end(); it++)
//     {
//       cout<<"\n"<<it->first<<"\n"<<(it->second)->getDAGStr()<<endl;
//     }
//   cout<<"\n******************\n";

  

  cout<<"\nCreating the concrete verification condition and the width table\n";

  for(map<int, CDAGNode*>::iterator it=FRAME_SEMAP.begin(); it!=FRAME_SEMAP.end(); it++)
    {
      int frame = it->first;
      CDAGNode *TR = it->second;
            
      //cout<<"\nShifting properly before printing\n";

      TR = MANAGER.callCloneAndShiftWithDP(TR, frame-1);

      //cout<<"\nShifted TR = "<<TR->getDAGStr()<<endl;

      TR = MANAGER.callQuickSimplify(TR, SimplificationTable);

      //cout<<"\nAfter property-based simplification  = "<<TR->getDAGStr()<<endl;
      

      bool convert_tr_to_proper_form=false;
      if(convert_tr_to_proper_form)
	  {
	    TR = convertExpressionToProperFormula(TR, "rules_dnf.txt"); 
	    //cout<<"\nAfter conversion to prop skelton  = "<<TR->getDAGStr()<<endl;
	  }

     //get the width table for this frame

      getWidthTableWithDP(TR, ent.getName(), WidthTableForCVC);

      UnrolledTRs.push_back(TR);
    }


  CDAGNode *CVC = NULL;
  for(unsigned int i=0; i<UnrolledTRs.size(); i++)
    {
      cout<<"\nTaking the TR number "<<i<<endl;
      CDAGNode *TR = UnrolledTRs[i];

      //cout<<"\nTR = "<<TR->getDAGStr()<<endl;

      if(CVC == NULL)
	{
	  CVC = TR;
	}
      else
	{
	  CVC = MANAGER.createDAG("and", CVC, TR);
	}
    }

  if(CVC == NULL)
    {
      cout<<"\nERROR! Concrete verification condition becomes NULL in function CSymbSimulator::generateConcreteVerificationCondition\n";
      exit(1);
    }

  return CVC;

}// Function ends here



//get the model in the map Model by parsing the CEX returned from the solver in the file yices_output.txt
void CSymbSimulator::getModelFromFile(map<string, string> &Model)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("yices_output.txt");
  
  string s;
  string signal;
  string value;

  for(int count=0;!infile->eof(); count++)
    {
 	
      *infile>>s;
      if(s=="")
	break;
      //cout<<"Line read is "<<s<<endl;
      if(count==0) // sat/unsat line
	{
	  continue;
	}
      if(count%3==1) // (= line
	{
	  continue;
	}
      if(count%3==2) // signal line
	{
	  signal = s;
	  continue;
	}
      else // value line
	{
	  value  = s;

	  int start_pos = value.find("b");
	  value  = value.substr(start_pos+1);
	  int end_pos = value.find(")");
	  value  = value.substr(0,end_pos);

	  Model.insert(make_pair(signal, value));
	}
    }

  infile->close();

}

// evaluate CVC using Model : note that CVC = conjunction of dags in UnrolledTRs
bool CSymbSimulator::evaluateConcreteVerificationConditionAndFindSetOfSignalsTobeExposed(CDAGNode* CVC, vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<string, string> &Model, CEntity &ent, set<string> &SetOfSignalsTobeExposed)
{

  bool CEXIsReal = true;

  MANAGER.showMap(Model, "Model");

  trimLeadingZeros(Model);
  
  cout<<"\nModel after trimming laeding zeros\n";

  MANAGER.showMap(Model, "Model");
  
  extendVariable_Value_Map(CVC, Model, WidthTableForCVC, ent);

  cout<<"\nAfter extension,\n";

  MANAGER.showMap(Model, "Model");

  //char  c = getchar();

  map<int, map<string, string> > StateWiseModel;

  initializeStateWiseModel(StateWiseModel, Model);

  showStateWiseModel(StateWiseModel);

  int SpuriousState;

  set<string> MismatchedSignals;

  for(unsigned int i=0; i<UnrolledTRs.size(); i++)
    {
      cout<<"\nTaking the TR number "<<i<<","<<i+1<<endl; 

      CDAGNode *TR = UnrolledTRs[i];

      map<int, map<string, string> >::iterator sit = StateWiseModel.find(i);

      if(sit == StateWiseModel.end())
	{
	  cout<<"\nNo entry for state "<<i<<" in StateWiseModel\n";
	  exit(1);
	}

      map<string, string> Model_i = sit->second;

      sit = StateWiseModel.find(i+1);

      if(sit == StateWiseModel.end())
	{
	  cout<<"\nNo entry for state "<<i+1<<" in StateWiseModel\n";
	  exit(1);
	}
      
      map<string, string> Model_i_p_1 = sit->second;

      MANAGER.showMap(Model_i, "Model i");
      MANAGER.showMap(Model_i_p_1, "Model i+1");

      cout<<"\nGetting the internal and output signals at state "<<i+1<<endl;

      map<string, string> Model_From_Simulation;

      getModelFromSimulation(TR, Model_i, Model_i_p_1, Model_From_Simulation, WidthTableForCVC, ent, i);

      get_Mismatch_Between_Observed_And_Simulation_Models(Model_i_p_1, Model_From_Simulation, MismatchedSignals, ent);

      if(MismatchedSignals.empty()) // No mismatch between observed signals and the signals obtained from simulation
	{
	  updateStateWiseModel(StateWiseModel, Model_From_Simulation, i+1);// As Model_i_p_1 and Model_From_Simulation are matching, Model_i_p_1 is a subset of Model_From_Simulation. Hence insert entries from Model_From_Simulation into StateWiseModel
	}
      else
	{
	  cout<<"\nMismatch at state "<<i+1<<endl;
	  MANAGER.showSet(MismatchedSignals, "MismatchedSignals");
	  CEXIsReal = false;	  
	  SpuriousState = i+1;
	  break;
	}

    }// end for


  if(CEXIsReal)
    {
      cout<<"\nCounterexample is real\n";
      return true;
    }
  else
    {
      cout<<"\nCounterexample is spurious\n";
      cout<<"\nLet us try to find the signals to be exposed\n";

      findSetOfSignalsTobeExposed(CVC, UnrolledTRs, WidthTableForCVC, StateWiseModel, ent, SpuriousState, MismatchedSignals, SetOfSignalsTobeExposed);

      showSet(SetOfSignalsTobeExposed, "SetOfSignalsTobeExposed");      

      return false;
    }

}// function ends here


// find the signals which need to be exposed and concatenate them in signals.txt
void CSymbSimulator::refinement(set<string> &SetOfSignalsToBeExposed, CEntity &ent)
{
  MANAGER.showSet(SetOfSignalsToBeExposed, "SetOfSignalsToBeExposed");

  bool limit_exposition_to_new_signals = true;
  
  if(limit_exposition_to_new_signals)
    {
      set<string> SetOfNewSignalsToBeExposed;

      // get the input & output identifiers of the top-level entity
      set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
      getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
      set<string> output_id_names;
      getNamesFromCSensitiveElementSet(output_identifiers, output_id_names);// output_id_names obtained

      set<string> PresentUnquantifiedSignals;
      getUnquantifiedSignals(PresentUnquantifiedSignals);// Present set of signals in signals.txt
      

      for(set<string>::iterator it = SetOfSignalsToBeExposed.begin(); it != SetOfSignalsToBeExposed.end(); it++)
	{
	  string id_delta = *it;
	  int index = id_delta.find_last_of("_");
	  string id = id_delta.substr(0, index);

	  if( (output_id_names.find(id) == output_id_names.end()) && (PresentUnquantifiedSignals.find(id_delta) == PresentUnquantifiedSignals.end())) // signal is not an output signal and is not there in signals.txt
	    {
	      SetOfNewSignalsToBeExposed.insert(id_delta);
	    }//if 
	}// for

      SetOfSignalsToBeExposed = SetOfNewSignalsToBeExposed;

    }// if(limit_observation_to_output_signals)


  cout<<"\nAfter limiting to unexposed internal signals\n";

  MANAGER.showSet(SetOfSignalsToBeExposed, "SetOfSignalsToBeExposed");

  if(SetOfSignalsToBeExposed.empty())
    {
      cout<<"\nSetOfSignalsToBeExposed is empty in CSymbSimulator::refinement\n";
      exit(1);
    }


  //char c =  getchar();

  // Writing into RefinementDetails

  RefinementDetails.insert(make_pair(cegar_iteration, SetOfSignalsToBeExposed));

  // Writing into RefinementDetails done
  
  
  FILE* signals_fp = fopen("signals.txt","a+");

  for(set<string>::iterator it = SetOfSignalsToBeExposed.begin(); it != SetOfSignalsToBeExposed.end(); it++)
    {
      string signal = *it;
      // concatenate \nsignal into signals_fp

      fprintf(signals_fp, "\n%s", signal.c_str());
    }  

  fclose(signals_fp);

  // concatenate them in signals.txt

  

}


void CSymbSimulator::extendVariable_Value_Map(CDAGNode *CVC, map<string, string> &Model, map<string, int> &WidthTableForCVC, CEntity &ent)
{
  set<string> Support;
  CVC->getDAGNames(Support);


  // get the input & output identifiers of the top-level entity
  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
  set<string> input_id_names, output_id_names;
  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names);  
  getNamesFromCSensitiveElementSet(output_identifiers, output_id_names);
  

  srand((unsigned)time(0));
  
  for(set<string>::iterator it = Support.begin(); it != Support.end(); it++)
    {
      string variable = *it;
      map<string, string>::iterator mit = Model.find(variable);
      
      if(mit == Model.end())
	{
	  cout<<endl<<"variable "<<variable<<" does not occure in Model\n";

	  // split variable into name and delta
	  // if delta = 0, i.e. intial state; assign random value to it
	  // else if name is an input/output variable, assign random value to it

	  bool choose_value = false;

	  int index = variable.find_last_of("_");
	  string id = variable.substr(0, index);
	  int delta = atoi(variable.substr(index+1).c_str());

	  if(delta == 0)
	    {
	      choose_value = true;
	    }
	  else
	    {
	      if(input_id_names.find(id)!=input_id_names.end())
		{
		  choose_value = true;
		}
	      else if(output_id_names.find(id)!=output_id_names.end())
		{
		  choose_value = true;
		}
		  
	    }



	  if(choose_value)
	    {
	      map<string, int>::iterator wit = WidthTableForCVC.find(variable);

	      if(wit == WidthTableForCVC.end())
		{
		  cout<<endl<<"variable "<<variable<<" has no entry in WidthTableForCVC in CSymbSimulator::extendVariable_Value_Map\n";
		  exit(1);
		}

	      int width = wit->second;

	      unsigned long long int Range = findPower(width);
	  
	      unsigned long long int random_no = rand()% Range;

	      string bin_value = MANAGER.ULINTToBinaryString(random_no); // binary value with minimum required width

	      cout<<endl<<"variable "<<variable<<" given the value "<<bin_value<<" \n";

	      Model.insert(make_pair(variable, bin_value));
	    }
	  else
	    {
	      cout<<endl<<"variable "<<variable<<" is not assigned a radom value \n"; 
	    }
	}// if(mit == Model.end())	  
	  
    }// for each variable in Support  
}


void CSymbSimulator::initializeStateWiseModel(map<int, map<string, string> > &StateWiseModel, map<string, string> &Model)
{
  for(map<string, string>::iterator it = Model.begin(); it != Model.end(); it++)
    {
      string signal = it->first;
      string value = it->second;

      int index = signal.find_last_of("_");
      string id = signal.substr(0, index);
      int state = atoi(signal.substr(index+1).c_str());

      map<int, map<string, string> >::iterator sit = StateWiseModel.find(state);
      if(sit == StateWiseModel.end()) // no map present for state
	{
	  // create map
	  map<string, string> Model_For_State;
	  Model_For_State.insert(make_pair(signal, value));
	  // insert map into StateWiseModel
	  StateWiseModel.insert(make_pair(state, Model_For_State));
	}
      else
	{ // map already exists. hence, just insert into the map
	  (sit->second).insert(make_pair(signal, value));
	}
    }// end for
}// end function


void CSymbSimulator::showStateWiseModel(map<int, map<string, string> > &StateWiseModel)
{
  cout<<"\nStateWiseModel\n";

  for(map<int, map<string, string> >::iterator it = StateWiseModel.begin(); it != StateWiseModel.end(); it++)
    {
      int state = it->first;
      map<string, string> Model = it->second;

      cout<<"\nState = "<<state<<endl;

      for(map<string, string>::iterator it2 = Model.begin(); it2 != Model.end(); it2++)
	{
	  string signal = it2->first;
	  string value = it2->second;

	  cout<<"\nSignal = "<<signal<<"\tValue = "<<value<<endl;	  
	}// end for

    }// end for
}// end function



void CSymbSimulator::getModelFromSimulation(CDAGNode *TR, map<string, string> &Model_i, map<string, string> &Model_i_p_1, map<string, string> &Model_From_Simulation, map<string, int> &WidthTableForCVC, CEntity &ent, int frame_number)
{
  if(TR == NULL)
    {
      cout<<"\n TR = NULL in function CSymbSimulator::getModelFromSimulation\n";
      exit(1);
    }

  //cout<<"\nTR = "<<TR->getDAGStr()<<endl;

  map<string, CDAGNode*> FunctionMap;
  MANAGER.createFunctionMap(TR, FunctionMap);

  //cout<<"\nFunctionMap\n";
  
  for(map<string, CDAGNode*>::iterator it=FunctionMap.begin(); it!=FunctionMap.end(); it++)
    {
      string signal = it->first;
      // Take the function
      CDAGNode *Function = it->second;

      //cout<<"\nSignal = "<<signal<<endl;
      //cout<<"\nFunction = "<<Function->getDAGStr()<<endl;
    }


   map<string, string> Variable_Value_Map; // This is the union of Model_i and Model_i_p_1
   getVariable_Value_Map(Variable_Value_Map, Model_i, Model_i_p_1);
   //MANAGER.showMap(Variable_Value_Map, "Variable_Value_Map");

   // Let us first find the order in which the functions should be evaluated

   list<string> OrderOfEvaluationForSignals;

   getOrderOfEvaluationForSignals(OrderOfEvaluationForSignals, FunctionMap, ent, frame_number);

   for(list<string>::iterator lit = OrderOfEvaluationForSignals.begin(); lit != OrderOfEvaluationForSignals.end(); lit++)
     {
       // Evaluate each function using the Variable_Value_Map

       string signal = *lit;

       map<string, CDAGNode*>::iterator it = FunctionMap.find(signal);

       if(it == FunctionMap.end())
	 {
	   cout<<"\nNo entry for "<<signal<<" in FunctionMap in CSymbSimulator::getModelFromSimulation\n";
	   exit(1);
	 }

       // Take the function
       CDAGNode *Function = it->second;

       //cout<<"\nSignal = "<<signal<<endl;
       //cout<<"\nFunction = "<<Function->getDAGStr()<<endl;

       string value;
       value = MANAGER.evaluateFunction(Function, Variable_Value_Map, WidthTableForCVC);
       //cout<<"\nvalue = "<<value<<endl;

       map<string, string>::iterator vv_it = Variable_Value_Map.find(signal);

       if(vv_it != Variable_Value_Map.end())
	 {
	   cout<<"\nEntry exists for "<<signal<<" in Variable_Value_Map in CSymbSimulator::getModelFromSimulation\n";
	   exit(1);
	 }

       Variable_Value_Map.insert(make_pair(signal, value));
       
       Model_From_Simulation.insert(make_pair(signal, value));
     }
}


void CSymbSimulator::getVariable_Value_Map(map<string, string> &Variable_Value_Map, map<string, string> &Model_i, map<string, string> &Model_i_p_1)
{
  for(map<string, string>::iterator it = Model_i.begin(); it != Model_i.end(); it++)
    {
      Variable_Value_Map.insert(make_pair(it->first, it->second));
    }
  for(map<string, string>::iterator it = Model_i_p_1.begin(); it != Model_i_p_1.end(); it++)
    {
      Variable_Value_Map.insert(make_pair(it->first, it->second));
    }

}



void CSymbSimulator::get_Mismatch_Between_Observed_And_Simulation_Models(map<string, string> &Observed_Model, map<string, string> &Model_From_Simulation, set<string> &MismatchedSignals, CEntity &ent)
{
  // for each signal in Model_From_Simulation, check if the value is matching with the observed value

  for(map<string, string>::iterator it = Model_From_Simulation.begin(); it != Model_From_Simulation.end(); it++)
    {
      string signal = it->first;
      string value_simulated = it->second;
      
      map<string, string>::iterator it2 = Observed_Model.find(signal);
      if(it2 == Observed_Model.end())
	{
	  cout<<"\nThe signal "<<signal<<" is not observed\n";
	  continue;
	}
      
      string value_observed = it2->second;
      if(value_observed != value_simulated) // mismatch between value observed and simulated
	{
	  cout<<"\nMismatch\n";

	  cout<<"\nvalue_observed = "<<value_observed<<endl;
	  cout<<"\nvalue_simulated = "<<value_simulated<<endl;

	  MismatchedSignals.insert(signal);
	}
    }// for ends here

  bool limit_observation_to_output_signals = true;
  
  if(limit_observation_to_output_signals)
    {
      set<string> MismatchedOutputSignals;

      // get the input & output identifiers of the top-level entity
      set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
      getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
      set<string> output_id_names;
      getNamesFromCSensitiveElementSet(output_identifiers, output_id_names);

      for(set<string>::iterator it = MismatchedSignals.begin(); it != MismatchedSignals.end(); it++)
	{
	  string id_delta = *it;
	  int index = id_delta.find_last_of("_");
	  string id = id_delta.substr(0, index);

	  if(output_id_names.find(id) != output_id_names.end()) // id is an output signal
	    {
	      MismatchedOutputSignals.insert(id_delta);
	    }//if 
	}// for

      MismatchedSignals = MismatchedOutputSignals;

    }// if(limit_observation_to_output_signals)

      
}

void CSymbSimulator::updateStateWiseModel(map<int, map<string, string> > &StateWiseModel, map<string, string> &Model_From_Simulation, int state)
{
// Add each entry from Model_From_Simulation into StateWiseModel

  for(map<string, string>::iterator it = Model_From_Simulation.begin(); it != Model_From_Simulation.end(); it++)
    {
      string signal = it->first;
      string value = it->second;
      
      map<int, map<string, string> >::iterator sit = StateWiseModel.find(state);
      if(sit == StateWiseModel.end()) // no map present for state
	{
	  // create map
	  map<string, string> Model_For_State;
	  Model_For_State.insert(make_pair(signal, value));
	  // insert map into StateWiseModel
	  StateWiseModel.insert(make_pair(state, Model_For_State));
	}
      else
	{ // map already exists. hence, just insert into the map
	  (sit->second).insert(make_pair(signal, value));
	}
      
    }// for ends here
}


void CSymbSimulator::findSetOfSignalsTobeExposed(CDAGNode *CVC, vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<int, map<string, string> > &StateWiseModel, CEntity &ent, int SpuriousState, set<string> &ProblematicSignalsAtSpuriousState, set<string> &SetOfSignalsTobeExposed)
{
  while(SpuriousState > 0 && !ProblematicSignalsAtSpuriousState.empty())// SpuriousState should be >0 as state_0 is initialized as well as there are some problematic signals
    {
      CDAGNode *TR = UnrolledTRs[SpuriousState-1];

      map<int, map<string, string> >::iterator sit = StateWiseModel.find(SpuriousState-1);

      if(sit == StateWiseModel.end())
	{
	  cout<<"\nNo entry for state "<<SpuriousState-1<<" in StateWiseModel\n";
	  exit(1);
	}

      map<string, string> Model_i = sit->second;

      sit = StateWiseModel.find(SpuriousState);

      if(sit == StateWiseModel.end())
	{
	  cout<<"\nNo entry for state "<<SpuriousState<<" in StateWiseModel\n";
	  exit(1);
	}

      map<string, string> Model_i_p_1 = sit->second;

      
      cout<<"\nTR = "<<TR->getDAGStr()<<endl;

      MANAGER.showMap(Model_i, "Model i");
      MANAGER.showMap(Model_i_p_1, "Model i+1");
      MANAGER.showSet(ProblematicSignalsAtSpuriousState, "ProblematicSignalsAtSpuriousState");

      set<string> ProblematicSignalsAtPreviousState;
      cout<<"\nLet us find what are the problematic signals at the previous state\n";
      cout<<"\nThey are the signals at state "<<SpuriousState-1<<" which are unobserved and which cause any of the ProblematicSignalsAtSpuriousState to take their values\n";

      for(set<string>::iterator it = ProblematicSignalsAtSpuriousState.begin(); it != ProblematicSignalsAtSpuriousState.end(); it++)
	{
	  string signal = *it;
	  set<string> ProblematicSignalsAtPreviousStateForSignal;
	  
	  cout<<"\nLet us find the problematic signals at "<<SpuriousState-1<<" which are unobserved and which cause "<<signal<<" to take it's value\n";

	  findProblematicSignalsAtPreviousStateForSignal(TR, Model_i, Model_i_p_1, signal, WidthTableForCVC, ProblematicSignalsAtPreviousStateForSignal, ent);
	  
	  set_union(ProblematicSignalsAtPreviousState.begin(), ProblematicSignalsAtPreviousState.end(), ProblematicSignalsAtPreviousStateForSignal.begin(), ProblematicSignalsAtPreviousStateForSignal.end(),inserter(ProblematicSignalsAtPreviousState, ProblematicSignalsAtPreviousState.begin()));
	}

      set_union(SetOfSignalsTobeExposed.begin(), SetOfSignalsTobeExposed.end(), ProblematicSignalsAtPreviousState.begin(), ProblematicSignalsAtPreviousState.end(),inserter(SetOfSignalsTobeExposed, SetOfSignalsTobeExposed.begin()));
        
      ProblematicSignalsAtSpuriousState = ProblematicSignalsAtPreviousState;
      
      SpuriousState--;
      
    }// while ends here
  
      
}


void CSymbSimulator::findProblematicSignalsAtPreviousStateForSignal(CDAGNode *TR, map<string, string> &Model_i, map<string, string> &Model_i_p_1, string signal, map<string, int> &WidthTableForCVC, set<string> &ProblematicSignalsAtPreviousStateForSignal, CEntity &ent)
{
  if(TR == NULL)
    {
      cout<<"\n TR = NULL in function CSymbSimulator::findProblematicSignalsAtPreviousStateForSignal\n";
      exit(1);
    }

  cout<<"\nTR = "<<TR->getDAGStr()<<endl;

  map<string, CDAGNode*> FunctionMap;
  MANAGER.createFunctionMap(TR, FunctionMap);

  
  map<string, CDAGNode*>::iterator fit = FunctionMap.find(signal);
  
  if(fit == FunctionMap.end())
    {
      cout<<"\nNo function for signal "<<signal<<" in CSymbSimulator::findProblematicSignalsAtPreviousStateForSignal\n";
      exit(1);
    }
  
  CDAGNode *Function = fit->second;

   map<string, string> Variable_Value_Map; // This is the union of Model_i and Model_i_p_1
   getVariable_Value_Map(Variable_Value_Map, Model_i, Model_i_p_1);
   MANAGER.showMap(Variable_Value_Map, "Variable_Value_Map");

   cout<<"\nLet us find the signals responsible to evaluate "<<signal<<" to the specific value in "<<Function->getDAGStr()<<endl;

   string evaluated_value = MANAGER.findProblematicSignalsAtPreviousStateForSignal(Function, Variable_Value_Map, WidthTableForCVC, ProblematicSignalsAtPreviousStateForSignal);

   // get the input identifiers of the top-level entity
  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
  set<string> input_id_names;
  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names); 

  set<string> ProblematicSignalsAtPreviousStateForSignalModified;

  for(set<string>::iterator it = ProblematicSignalsAtPreviousStateForSignal.begin(); it != ProblematicSignalsAtPreviousStateForSignal.end(); it++)
    {
      string signal = *it;
      int index = signal.find_last_of("_");
      string id = signal.substr(0, index);
      int delta = atoi(signal.substr(index+1).c_str());

      if(input_id_names.find(id)==input_id_names.end()) // signal is not an input
		{
		  ProblematicSignalsAtPreviousStateForSignalModified.insert(signal);
		}
    }
  
  ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalModified;
  
  showSet(ProblematicSignalsAtPreviousStateForSignal, "ProblematicSignalsAtPreviousStateForSignal");
}

// Given a model, this function returns a modified
// model s.t. the values in the model have no leading zeros
void CSymbSimulator::trimLeadingZeros(map<string, string> &Model)
{
  map<string, string> NewModel;

  for(map<string, string>::iterator it = Model.begin(); it != Model.end(); it++)
    {
      string signal = it->first;
      string value = it->second;

      string trimmed_value;

      if(value.length()==1)
	{
	  trimmed_value = value;
	}
      else
	{
	 int index = value.find_first_of("1"); // get index of first 1

	 if(index != string::npos)// There is 1
	   {
	     trimmed_value = value.substr(index);
	   }
	 else // all zeros
	   {
	     trimmed_value = "0";
	   }
	}
      
      NewModel.insert(make_pair(signal, trimmed_value)); 
    }// for ends

  Model = NewModel;
}


void CSymbSimulator::updateStateWiseModelBasedOnModel_From_Simulation(map<int, map<string, string> > &StateWiseModel, map<string, string> &Model_From_Simulation, int state)
{
// Add each entry from Model_From_Simulation into StateWiseModel

  for(map<string, string>::iterator it = Model_From_Simulation.begin(); it != Model_From_Simulation.end(); it++)
    {
      string signal = it->first;
      string value = it->second;
      
      map<int, map<string, string> >::iterator sit = StateWiseModel.find(state);
      if(sit == StateWiseModel.end()) // no map present for state
	{
	  // create map
	  map<string, string> Model_For_State;
	  Model_For_State.insert(make_pair(signal, value));
	  // insert map into StateWiseModel
	  StateWiseModel.insert(make_pair(state, Model_For_State));
	}
      else
	{ // map already exists. hence, just insert into the map
	  // before inserting, just see if entry for signal is there already

	  map<string, string>::iterator mit = (sit->second).find(signal);

	  if(mit == (sit->second).end()) // no entry exists for signal; hence add new entry
	    {
	      (sit->second).insert(make_pair(signal, value));
	    }
	  else// entry exists for signal; hence change existing entry
	    {
	      cout<<"\nstate = "<<state<<"\nsignal = "<<signal<<"\nvalue = "<<value<<endl;
	      (sit->second)[signal] = value;
	    }
	}// else ends here
      
    }// for ends here
}




// evaluate CVC using Model : note that CVC = conjunction of dags in UnrolledTRs, return if CEX is real/spurious and set of signals
// to be exposed
bool CSymbSimulator::evaluateConcreteVerificationConditionAndFindSetOfSignalsTobeExposedPropertyGuided(CDAGNode* CVC, vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<string, string> &Model, CEntity &ent, set<string> &SetOfSignalsTobeExposed)
{
  unsigned long long int simulation_duration_ms;
  struct timeval simulation_start_ms, simulation_end_ms;
  gettimeofday(&simulation_start_ms, NULL);



  //MANAGER.showMap(Model, "Model");

  trimLeadingZeros(Model);

  cout<<"\nModel after trimming leading zeros\n";

  //MANAGER.showMap(Model, "Model");

  eliminateAssignmentsToInternalSignals(Model, ent);

  cout<<"\nModel after eliminating assignments to internal/output signals at time > 0\n";

  //MANAGER.showMap(Model, "Model");

  extendVariable_Value_Map_Modified(CVC, Model, WidthTableForCVC, ent);

  cout<<"\nAfter extension,\n";

  //MANAGER.showMap(Model, "Model");

  map<int, map<string, string> > StateWiseModel;

  initializeStateWiseModel(StateWiseModel, Model);

  showStateWiseModel(StateWiseModel);

  int SpuriousState;

  set<string> MismatchedSignals;

  for(unsigned int i=0; i<UnrolledTRs.size(); i++)
    {
      cout<<"\nTaking the TR number "<<i<<","<<i+1<<endl; 

      CDAGNode *TR = UnrolledTRs[i];

      map<int, map<string, string> >::iterator sit = StateWiseModel.find(i);

      map<string, string> Model_i;

      map<string, string> Model_i_p_1;

      if(sit != StateWiseModel.end())
	{
	  Model_i = sit->second; // Gives the signals in prev. state
	}

      sit = StateWiseModel.find(i+1);

      if(sit != StateWiseModel.end())
	{
	  Model_i_p_1 = sit->second; // Gives the input signals in this state
	}

      if(Model_i.empty() && Model_i_p_1.empty())
	{
	  cout<<"\nModel to be used for simulation is empty in function CSymbSimulator::evaluateConcreteVerificationConditionAndFindSetOfSignalsTobeExposedPropertyGuided\n";
	  exit(1);
	}
      

      cout<<"\nGetting the internal and output signals at state "<<i+1<<endl;

      map<string, string> Model_From_Simulation;

      getModelFromSimulation(TR, Model_i, Model_i_p_1, Model_From_Simulation, WidthTableForCVC, ent, i);

      updateStateWiseModelBasedOnModel_From_Simulation(StateWiseModel, Model_From_Simulation, i+1);// Insert entries from Model_From_Simulation into StateWiseModel  

      //showStateWiseModel(StateWiseModel);
    
    }// end for


  cout<<"\nFinally\n";

  showStateWiseModel(StateWiseModel);


  gettimeofday(&simulation_end_ms, NULL);
  simulation_duration_ms = simulation_end_ms.tv_sec * 1000 + simulation_end_ms.tv_usec / 1000;
  simulation_duration_ms -= simulation_start_ms.tv_sec * 1000 + simulation_start_ms.tv_usec / 1000;
  simulation_ms = simulation_ms + simulation_duration_ms;	



  set<string> SupportOfUnsatCore;

  bool CEXIsReal = evaluateAndGetSupportOfUnsatCoreOfProperty(StateWiseModel, SupportOfUnsatCore);

  if(CEXIsReal)
    {
      cout<<"\nCounterexample is real\n";

      return true;
    }
  else
    {
      cout<<"\nCounterexample is spurious\n";

      cout<<"\nLet us try to find the signals to be exposed\n";


      MANAGER.showSet(SupportOfUnsatCore, "SupportOfUnsatCore");

      //char c = getchar();


      findSetOfSignalsTobeExposedPropertyGuided(UnrolledTRs, WidthTableForCVC, StateWiseModel, ent, SupportOfUnsatCore, SetOfSignalsTobeExposed);

      showSet(SetOfSignalsTobeExposed, "SetOfSignalsTobeExposed");

      
      //char c = getchar();


      return false;
      } 

  

}// function ends here



bool CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfProperty(map<int, map<string, string> > &StateWiseModel, set<string> &SupportOfUnsatCore)
{

  unsigned long long int unsat_core_time_total_duration_ms;
  struct timeval unsat_core_time_total_start_ms, unsat_core_time_total_end_ms;
  gettimeofday(&unsat_core_time_total_start_ms, NULL);


  bool result_evaluation;

  bool trivial = false;

  if(trivial)
    {
      cout<<"\nPlease write result of evaluation of property as true/fale in result_evaluation.txt and unsat. core of property (if it is false) in in unsatcore_evaluation.txt\n";
      cout<<"\nPress any key to continue\n";
      char c =  getchar();  
    }
  else
    {



      // convert property in BPSL into a valid SMTLib file (in property_in_proper_smtlib.txt)
      string command;

      if(psl_property_on_cycles)
 	{
		 command="propTransCycleBased < ";
		 command += propertyfile;
		 command += " -proper_smtlib";
		 command += " -width_file=widthtable.txt";
		 command += " -output_file=property_in_proper_smtlib.txt";
 	}
 	else
 	{
	      	command="propTrans < ";
	      	command += propertyfile;
	      	command += " proper_smtlib";
	      	command += " -output_file=property_in_proper_smtlib.txt";
	}


      system(command.c_str());

      // write StateWiseModel into model_evaluation.txt
      printStateWiseModel(StateWiseModel, "model_evaluation.txt");


      // before calling let's delete the existing result files
      
      system("rm result_evaluation.txt");

      system("rm unsatcore_evaluation.txt");

      bool inputs_to_all_needed = false;

      if(inputs_to_all_needed)
	{
	  char all_call_number_char[100];
	  sprintf(all_call_number_char, "%d", all_call_number);
	  string all_call_number_str(all_call_number_char);

	  string dag_file = "formula_";
	  dag_file += all_call_number_str;
	  dag_file += ".txt";

	  string model_file = "model_";
	  model_file += all_call_number_str;
	  model_file += ".txt";

	  string command1 = "cp property_in_proper_smtlib.txt ";
	  command1 += dag_file;

	  string command2 = "cp model_evaluation.txt ";
	  command2 += model_file;

	  system(command1.c_str());
	  system(command2.c_str());

	  all_call_number++;
	}
      

      time_t unsat_core_time_start, unsat_core_time_end, unsat_core_time_duration;
      time(&unsat_core_time_start);


      unsigned long long int unsat_core_time_duration_ms;
      struct timeval unsat_core_time_start_ms, unsat_core_time_end_ms;
      gettimeofday(&unsat_core_time_start_ms, NULL);


      // call system("all ....") with proper arguments
      // get results in result_evaluation.txt and unsatcore_evaluation.txt
      command = "all -evaluate_and_get_core -model=model_evaluation.txt -evaluation_result=result_evaluation.txt -core_result=unsatcore_evaluation.txt property_in_proper_smtlib.txt";
      system(command.c_str());


      gettimeofday(&unsat_core_time_end_ms, NULL);
      unsat_core_time_duration_ms = unsat_core_time_end_ms.tv_sec * 1000 + unsat_core_time_end_ms.tv_usec / 1000;
      unsat_core_time_duration_ms-= unsat_core_time_start_ms.tv_sec * 1000 + unsat_core_time_start_ms.tv_usec / 1000;
      unsat_core_time_ms = unsat_core_time_ms + unsat_core_time_duration_ms;	




      time(&unsat_core_time_end);
      unsat_core_time_duration = unsat_core_time_end - unsat_core_time_start;
      unsat_core_time = unsat_core_time + unsat_core_time_duration;
      
      
    }
  
  // read result_evaluation.txt and get the result in result_evaluation

  FILE *result_evaluation_file = fopen("result_evaluation.txt", "r");

  if(result_evaluation_file == NULL) 
   {cout<<"\nCannot open result_evaluation.txt\n"; exit(1);}


  char result[100];
  fscanf(result_evaluation_file, "%s", result);
  fclose(result_evaluation_file);
  string result_str(result);

  cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="0")
    {
      
      result_evaluation = false;
    }
      
  else if(result_str=="1")
    { 
      result_evaluation = true;
    }
  else
    {
      cout<<"\nUnknown result "<<result_str<<" from all\n";
      exit(1);
    }


  // reading unsatcore_evaluation.txt into SupportOfUnsatCore

  SupportOfUnsatCore.clear();
  ifstream *infile;
  infile = new ifstream();
  infile->open("unsatcore_evaluation.txt");


  if(!(infile->is_open()))
	{
	  cout<<"\nFile unsatcore_evaluation.txt cannot be opened in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfProperty\n";
	  exit(1);
	}



  while(!infile->eof())
    {
      string s;
      *infile>>s;
      if(s=="")
	break;
      cout<<"Line read is "<<s<<endl;
      SupportOfUnsatCore.insert(s);
    }
  infile->close();


  if(!result_evaluation && SupportOfUnsatCore.empty()) // CEX is spurious and unsat core is empty
    {
      cout<<"\nEmpty Unsat core in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfProperty\n";
      exit(1);
    }


  gettimeofday(&unsat_core_time_total_end_ms, NULL);
  unsat_core_time_total_duration_ms = unsat_core_time_total_end_ms.tv_sec * 1000 + unsat_core_time_total_end_ms.tv_usec / 1000;
  unsat_core_time_total_duration_ms-= unsat_core_time_total_start_ms.tv_sec * 1000 + unsat_core_time_total_start_ms.tv_usec / 1000;
  unsat_core_time_total_ms = unsat_core_time_total_ms + unsat_core_time_total_duration_ms;	


  return result_evaluation;
   
}

int CSymbSimulator::findLastSpuriousState(set<string> &ProblematicSignals)
{
  int maximum = 0;

  for(set<string>::iterator it = ProblematicSignals.begin(); it != ProblematicSignals.end(); it++)
    {
      string signal = *it;
      int index = signal.find_last_of("_");
      string id = signal.substr(0, index);
      int delta = atoi(signal.substr(index+1).c_str());

      if(maximum < delta)
	{
	  maximum = delta;
	}
    }
  return maximum;      
}


void CSymbSimulator::getProblematicSignals_i_p_1(set<string> &ProblematicSignals, int i_p_1, set<string> &ProblematicSignals_i_p_1)
{
  for(set<string>::iterator it = ProblematicSignals.begin(); it != ProblematicSignals.end(); it++)
    {
      string signal = *it;
      int index = signal.find_last_of("_");
      string id = signal.substr(0, index);
      int delta = atoi(signal.substr(index+1).c_str());

      if(delta == i_p_1)
	{
	  ProblematicSignals_i_p_1.insert(signal);
	}
    }
}



void CSymbSimulator::findSetOfSignalsTobeExposedPropertyGuided(vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<int, map<string, string> > &StateWiseModel, CEntity &ent, set<string> &SupportOfUnsatCore, set<string> &SetOfSignalsTobeExposed)
{

  set<string> ProblematicSignals;

  bool remove_inputs = true;

  if(remove_inputs)
    {
      // get the input identifiers of the top-level entity
      set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
      getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
      set<string> input_id_names;
      getNamesFromCSensitiveElementSet(input_identifiers, input_id_names); 

      for(set<string>::iterator it = SupportOfUnsatCore.begin(); it != SupportOfUnsatCore.end(); it++)
	{
	  string signal = *it;
	  int index = signal.find_last_of("_");
	  string id = signal.substr(0, index);
	  int delta = atoi(signal.substr(index+1).c_str());

	  if(input_id_names.find(id)==input_id_names.end()) // signal is not an input
	    {
	      ProblematicSignals.insert(signal);
	    }
	}// for
    }//if(remove_inputs)

  else
    {
      ProblematicSignals = SupportOfUnsatCore;
    }
      
  
  showSet(ProblematicSignals, "ProblematicSignals");

  //char c = getchar();

  bool old_implementation = false;

  if(old_implementation)
    {


      int LastSpuriousState = findLastSpuriousState(ProblematicSignals);// find the state with maximum delta among signals in ProblematicSignals

      for(int i=LastSpuriousState-1; i>=0; i--)
	{
	  cout<<"\nTaking the TR number "<<i<<","<<i+1<<endl; 

	  CDAGNode *TR = UnrolledTRs[i];

	  map<int, map<string, string> >::iterator sit = StateWiseModel.find(i);

	  if(sit == StateWiseModel.end())
	    {
	      cout<<"\nNo entry for state "<<i<<" in StateWiseModel\n";
	      exit(1);
	    }

	  map<string, string> Model_i = sit->second; // Gives the signals in prev. state

	  sit = StateWiseModel.find(i+1);

	  if(sit == StateWiseModel.end())
	    {
	      cout<<"\nNo entry for state "<<i+1<<" in StateWiseModel\n";
	      exit(1);
	    }
      
	  map<string, string> Model_i_p_1 = sit->second; // Gives the input signals in this state

	  MANAGER.showMap(Model_i, "Model i"); 
	  MANAGER.showMap(Model_i_p_1, "Model i+1");

	  set<string> ProblematicSignals_i_p_1;

	  getProblematicSignals_i_p_1(ProblematicSignals, i+1, ProblematicSignals_i_p_1);

	  if(ProblematicSignals_i_p_1.empty())
	    {
	      continue;
	    }

	  set<string> ProblematicSignals_i;

	  cout<<"\nLet us find what are the problematic signals at state "<<i<<"\n";
	  cout<<"\nThey are the signals at state "<<i<<" which are unobserved and which cause any of the ProblematicSignals_i_p_1 to take their values\n";

	  for(set<string>::iterator it = ProblematicSignals_i_p_1.begin(); it != ProblematicSignals_i_p_1.end(); it++)
	    {
	      string signal = *it;
	      set<string> ProblematicSignals_i_ForSignal;
	  
	      cout<<"\nLet us find the problematic signals at "<<i<<" which are unobserved and which cause "<<signal<<" to take it's value\n";

	      findProblematicSignalsAtPreviousStateForSignalPropertyGuided(TR, Model_i, Model_i_p_1, signal, WidthTableForCVC, ProblematicSignals_i, ent);
	  
	      set_union(ProblematicSignals_i.begin(), ProblematicSignals_i.end(), ProblematicSignals_i_ForSignal.begin(), ProblematicSignals_i_ForSignal.end(),inserter(ProblematicSignals_i, ProblematicSignals_i.begin()));

	 
	    }

	  set_union(ProblematicSignals.begin(), ProblematicSignals.end(), ProblematicSignals_i.begin(), ProblematicSignals_i.end(),inserter(ProblematicSignals, ProblematicSignals.begin()));
	}// end for

  
      SetOfSignalsTobeExposed = ProblematicSignals;
    }//if(old_implementation)
  else
    {
      int LastSpuriousState = findLastSpuriousState(ProblematicSignals);// find the state with maximum delta among signals in ProblematicSignals

      for(int i=LastSpuriousState-1; i>=0; i--)
	{
	  cout<<"\nTaking the TR number "<<i<<","<<i+1<<endl; 

	  CDAGNode *TR = UnrolledTRs[i];

	  map<int, map<string, string> >::iterator sit = StateWiseModel.find(i);

	  if(sit == StateWiseModel.end())
	    {
	      cout<<"\nNo entry for state "<<i<<" in StateWiseModel\n";
	      exit(1);
	    }

	  map<string, string> Model_i = sit->second; // Gives the signals in prev. state

	  sit = StateWiseModel.find(i+1);

	  if(sit == StateWiseModel.end())
	    {
	      cout<<"\nNo entry for state "<<i+1<<" in StateWiseModel\n";
	      exit(1);
	    }
      
	  map<string, string> Model_i_p_1 = sit->second; // Gives the input signals in this state

	  MANAGER.showMap(Model_i, "Model i"); 
	  MANAGER.showMap(Model_i_p_1, "Model i+1");

	  set<string> ProblematicSignals_i_p_1;

	  getProblematicSignals_i_p_1(ProblematicSignals, i+1, ProblematicSignals_i_p_1);

	  if(ProblematicSignals_i_p_1.empty())
	    {
	      continue;
	    }

	  while(!ProblematicSignals_i_p_1.empty())
	    {
	      set<string> ProblematicSignals_i;
	      set<string> NewProblematicSignalsInCurrentState;// This is the new set of signals in the state i+1 which are problematic (in the unsat core of the set of signals presently in ProblematicSignals_i_p_1)

	      cout<<"\nLet us find what are the problematic signals at state "<<i<<"\n";
	      cout<<"\nThey are the signals at state "<<i<<" which are unobserved and which cause any of the ProblematicSignals_i_p_1 to take their values\n";

	      for(set<string>::iterator it = ProblematicSignals_i_p_1.begin(); it != ProblematicSignals_i_p_1.end(); it++)
		{
		  string signal = *it;
		  set<string> ProblematicSignals_i_ForSignal;
		  set<string> NewProblematicSignalsInCurrentState_ForSignal;
	  
		  cout<<"\nLet us find the problematic signals at "<<i<<" which are unobserved and which cause "<<signal<<" to take it's value\n";

		  findProblematicSignalsAtPreviousStateForSignalPropertyGuided(TR, Model_i, Model_i_p_1, signal, WidthTableForCVC, ProblematicSignals_i_ForSignal, ent);
	  
		  set_union(ProblematicSignals_i.begin(), ProblematicSignals_i.end(), ProblematicSignals_i_ForSignal.begin(), ProblematicSignals_i_ForSignal.end(),inserter(ProblematicSignals_i, ProblematicSignals_i.begin()));

		  getNewProblematicSignalsInCurrentState_ForSignal(ProblematicSignals_i_ForSignal, i+1, NewProblematicSignalsInCurrentState_ForSignal);// NewProblematicSignalsInCurrentState_ForSignal = signals in ProblematicSignals_i_ForSignal with delta time as i+1

		  showSet(NewProblematicSignalsInCurrentState_ForSignal, "NewProblematicSignalsInCurrentState_ForSignal");

		  set_union(NewProblematicSignalsInCurrentState.begin(), NewProblematicSignalsInCurrentState.end(), NewProblematicSignalsInCurrentState_ForSignal.begin(), NewProblematicSignalsInCurrentState_ForSignal.end(),inserter(NewProblematicSignalsInCurrentState, NewProblematicSignalsInCurrentState.begin()));

		  showSet(NewProblematicSignalsInCurrentState, "NewProblematicSignalsInCurrentState");
		}

	      set_union(ProblematicSignals.begin(), ProblematicSignals.end(), ProblematicSignals_i.begin(), ProblematicSignals_i.end(),inserter(ProblematicSignals, ProblematicSignals.begin()));

	      showSet(ProblematicSignals_i, "ProblematicSignals_i");
	      showSet(ProblematicSignals, "ProblematicSignals");

	      // Now we have considered all the signals which were present in ProblematicSignals_i_p_1 
	      // Hence clear ProblematicSignals_i_p_1 

	      ProblematicSignals_i_p_1.clear();

	      set_union(NewProblematicSignalsInCurrentState.begin(), NewProblematicSignalsInCurrentState.end(), ProblematicSignals_i_p_1.begin(), ProblematicSignals_i_p_1.end(),inserter(ProblematicSignals_i_p_1, ProblematicSignals_i_p_1.begin()));

	      NewProblematicSignalsInCurrentState.clear();

	      showSet(ProblematicSignals_i_p_1, "ProblematicSignals_i_p_1");
	      
	    }//while(!ProblematicSignals_i_p_1.empty())
	  
	}// end for


  
      SetOfSignalsTobeExposed = ProblematicSignals;
    }//else of if(old_implementation)

}// end of function



void CSymbSimulator::findProblematicSignalsAtPreviousStateForSignalPropertyGuided(CDAGNode *TR, map<string, string> &Model_i, map<string, string> &Model_i_p_1, string signal, map<string, int> &WidthTableForCVC, set<string> &ProblematicSignalsAtPreviousStateForSignal, CEntity &ent)
{
  if(TR == NULL)
    {
      cout<<"\n TR = NULL in function CSymbSimulator::findProblematicSignalsAtPreviousStateForSignalPropertyGuided\n";
      exit(1);
    }

  cout<<"\nTR = "<<TR->getDAGStr()<<endl;

  map<string, CDAGNode*> FunctionMap;
  MANAGER.createFunctionMap(TR, FunctionMap);

  
  map<string, CDAGNode*>::iterator fit = FunctionMap.find(signal);
  
  if(fit == FunctionMap.end())
    {
      cout<<"\nNo function for signal "<<signal<<" in CSymbSimulator::findProblematicSignalsAtPreviousStateForSignalPropertyGuided\n";
      exit(1);
    }
  
  CDAGNode *Function = fit->second;

   map<string, string> Variable_Value_Map; // This is the union of Model_i and Model_i_p_1
   getVariable_Value_Map(Variable_Value_Map, Model_i, Model_i_p_1);
   MANAGER.showMap(Variable_Value_Map, "Variable_Value_Map");

   cout<<"\nLet us find the signals responsible to evaluate "<<signal<<" to the specific value in "<<Function->getDAGStr()<<endl;

   int unsat_core_finding_strategy = 2;

   if(unsat_core_finding_strategy == 1)// Smaller core finding using dag evaluation
     {
       string evaluated_value = MANAGER.findProblematicSignalsAtPreviousStateForSignal(Function, Variable_Value_Map, WidthTableForCVC, ProblematicSignalsAtPreviousStateForSignal);
     }
   else if(unsat_core_finding_strategy == 2)// Smaller core finding using Unsat core finding
     {
       string evaluated_value = evaluateAndGetSupportOfUnsatCoreOfGivenDAGWithHashing(signal, Function, Variable_Value_Map, WidthTableForCVC, ProblematicSignalsAtPreviousStateForSignal);
     }

   // get the input identifiers of the top-level entity
  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
  set<string> input_id_names;
  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names); 

  set<string> ProblematicSignalsAtPreviousStateForSignalModified;

  for(set<string>::iterator it = ProblematicSignalsAtPreviousStateForSignal.begin(); it != ProblematicSignalsAtPreviousStateForSignal.end(); it++)
    {
      string signal = *it;
      int index = signal.find_last_of("_");
      string id = signal.substr(0, index);
      int delta = atoi(signal.substr(index+1).c_str());

      if(input_id_names.find(id)==input_id_names.end()) // signal is not an input
		{
		  ProblematicSignalsAtPreviousStateForSignalModified.insert(signal);
		}
    }
  
  ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalModified;
  
  showSet(ProblematicSignalsAtPreviousStateForSignal, "ProblematicSignalsAtPreviousStateForSignal");
}




string CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAG(string signal, CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &SupportOfUnsatCore)
{
  
  bool old_implementation = false;

  if(old_implementation)
    {
      
      string result_str;

      // we need to remove entry for signal from Variable_Value_Map

      map<string, string> Variable_Value_Map_Signal_Removed;

      for(map<string, string>::iterator it = Variable_Value_Map.begin(); it != Variable_Value_Map.end(); it++)
	{
	  if(it->first != signal)
	    {
	      Variable_Value_Map_Signal_Removed.insert(make_pair(it->first, it->second));
	    }
	}
  
      // Write node's RHS into a valid SMTLib file(in dag_in_proper_smtlib.txt)
      if(node->Get_DAGnode_value() == "="  || node->Get_DAGnode_value() == "equivalent")
	{
	  list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
	  iout++;
	  node = *iout;
	}
      else
	{
	  cout<<"\nArgument dag is "<<node->Get_DAGnode_value()<<" i.e. not a function in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAG\n";
	  exit(1);
	}

      MANAGER.printDAGInSMTLibFormatInGivenFile(node, MyWidthTable, "dag_in_proper_smtlib.txt");
  
      // write Variable_Value_Map_Signal_Removed into model_evaluation.txt

      printVariable_Value_Map_Signal_Removed(Variable_Value_Map_Signal_Removed, "model_evaluation.txt");
      // call system("all ....") with proper arguments
      // get results in result_evaluation.txt and unsatcore_evaluation.txt


      //char c = getchar();

      system("rm result_evaluation.txt");

      system("rm unsatcore_evaluation.txt");


      time_t unsat_core_time_start, unsat_core_time_end, unsat_core_time_duration;

      time(&unsat_core_time_start);

      
      unsigned long long int unsat_core_time_duration_ms;
      struct timeval unsat_core_time_start_ms, unsat_core_time_end_ms;
      gettimeofday(&unsat_core_time_start_ms, NULL);

	
      
      string command = "all -evaluate_and_get_core -model=model_evaluation.txt -evaluation_result=result_evaluation.txt -core_result=unsatcore_evaluation.txt dag_in_proper_smtlib.txt";
      system(command.c_str());


      gettimeofday(&unsat_core_time_end_ms, NULL);
      unsat_core_time_duration_ms = unsat_core_time_end_ms.tv_sec * 1000 + unsat_core_time_end_ms.tv_usec / 1000;
      unsat_core_time_duration_ms-= unsat_core_time_start_ms.tv_sec * 1000 + unsat_core_time_start_ms.tv_usec / 1000;
      unsat_core_time_ms = unsat_core_time_ms + unsat_core_time_duration_ms;	




      time(&unsat_core_time_end);

      unsat_core_time_duration = unsat_core_time_end - unsat_core_time_start;

      unsat_core_time = unsat_core_time + unsat_core_time_duration;
      

      //c = getchar();


      // read result_evaluation.txt and get the result in result_str

      FILE *result_evaluation_file = fopen("result_evaluation.txt", "r");

      if(result_evaluation_file == NULL) 
	{cout<<"\nCannot open result_evaluation.txt\n"; exit(1);}


      char result[100];
      fscanf(result_evaluation_file, "%s", result);
      fclose(result_evaluation_file);
      result_str = result;


      // reading unsatcore_evaluation.txt into SupportOfUnsatCore

      SupportOfUnsatCore.clear();
      ifstream *infile;
      infile = new ifstream();
      infile->open("unsatcore_evaluation.txt");


      if(!(infile->is_open()))
	{
	  cout<<"\nFile unsatcore_evaluation.txt cannot be opened in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAG\n";
	  exit(1);
	}



      while(!infile->eof())
	{
	  string s;
	  *infile>>s;
	  if(s=="")
	    break;
	  cout<<"Line read is "<<s<<endl;
	  SupportOfUnsatCore.insert(s);
	}
      infile->close();


      if(SupportOfUnsatCore.empty())
	{
	  cout<<"\nEmpty Unsat core in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAG\n";
	  exit(1);
	}



      return result_str;
    }
  else
    {

      unsigned long long int unsat_core_time_total_duration_ms;
      struct timeval unsat_core_time_total_start_ms, unsat_core_time_total_end_ms;
      gettimeofday(&unsat_core_time_total_start_ms, NULL);


      
      string result_str;

      string value_of_signal;

      

      // we need to remove entry for signal from Variable_Value_Map

      map<string, string> Variable_Value_Map_Signal_Removed;

      for(map<string, string>::iterator it = Variable_Value_Map.begin(); it != Variable_Value_Map.end(); it++)
	{
	  if(it->first != signal)
	    {
	      Variable_Value_Map_Signal_Removed.insert(make_pair(it->first, it->second));
	    }
	  else
	    {
	      value_of_signal = it->second;
	    }
	}

      // Convert value of the signal into decimal

      unsigned long long int value_of_signal_in_decimal = binaryStringToULINT(value_of_signal);
      char value_of_signal_in_decimal_char[100];
      sprintf(value_of_signal_in_decimal_char, "%llu", value_of_signal_in_decimal);
      string value_of_signal_in_decimal_string(value_of_signal_in_decimal_char);

      
      

      // Convert node into proper prop. skelton format

      node = convertExpressionToProperFormula(node, "rules_dnf.txt"); 
      node = convertExpressionToProperFormula(node, "rules_dnf.txt"); 
      node = convertExpressionToProperFormula(node, "rules_simplify.txt"); 

      // Get dag for signal <> value_of_signal

      string id_delta = signal;
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      string delta = id_delta.substr(index+1);
      int delta_int = atoi(delta.c_str());

      CDAGNode *variable = MANAGER.createDAG(id, delta_int);

      CDAGNode *constant = MANAGER.createDAG(value_of_signal_in_decimal_string);
      
      CDAGNode *DisEquationPart = MANAGER.createDAG("is_not_equal", variable, constant); 

      // node = node /\ DisEquationPart

      node = MANAGER.createDAG("and", node, DisEquationPart);


      cout<<"\nnode before printing in SMTLIB format = "<<node->getDAGStr()<<endl;

  
      // Write into a valid SMTLib file(in dag_in_proper_smtlib.txt)

      MANAGER.printDAGInSMTLibFormatInGivenFile(node, MyWidthTable, "dag_in_proper_smtlib.txt");
  
      // write Variable_Value_Map_Signal_Removed into model_evaluation.txt

      printVariable_Value_Map_Signal_Removed(Variable_Value_Map_Signal_Removed, "model_evaluation.txt");
      // call system("all ....") with proper arguments
      // get results in result_evaluation.txt and unsatcore_evaluation.txt


      //char c = getchar();

      system("rm unsatcore_evaluation.txt");


      //cout<<"\nCalling all....Press any key to continue\n";
      //char c = getchar();


      bool inputs_to_all_needed = false;

      if(inputs_to_all_needed)
	{
	  char all_call_number_char[100];
	  sprintf(all_call_number_char, "%d", all_call_number);
	  string all_call_number_str(all_call_number_char);

	  string dag_file = "formula_";
	  dag_file += all_call_number_str;
	  dag_file += ".txt";

	  string model_file = "model_";
	  model_file += all_call_number_str;
	  model_file += ".txt";

	  string command1 = "cp dag_in_proper_smtlib.txt ";
	  command1 += dag_file;

	  string command2 = "cp model_evaluation.txt ";
	  command2 += model_file;

	  system(command1.c_str());
	  system(command2.c_str());

	  all_call_number++;
	}


      time_t unsat_core_time_start, unsat_core_time_end, unsat_core_time_duration;

      time(&unsat_core_time_start);


      unsigned long long int unsat_core_time_duration_ms;
      struct timeval unsat_core_time_start_ms, unsat_core_time_end_ms;
      gettimeofday(&unsat_core_time_start_ms, NULL);


      string command = "all -get_core -model=model_evaluation.txt  -core_result=unsatcore_evaluation.txt dag_in_proper_smtlib.txt";
      system(command.c_str());

      gettimeofday(&unsat_core_time_end_ms, NULL);
      unsat_core_time_duration_ms = unsat_core_time_end_ms.tv_sec * 1000 + unsat_core_time_end_ms.tv_usec / 1000;
      unsat_core_time_duration_ms-= unsat_core_time_start_ms.tv_sec * 1000 + unsat_core_time_start_ms.tv_usec / 1000;
      unsat_core_time_ms = unsat_core_time_ms + unsat_core_time_duration_ms;	


      time(&unsat_core_time_end);

      unsat_core_time_duration = unsat_core_time_end - unsat_core_time_start;

      unsat_core_time = unsat_core_time + unsat_core_time_duration;



      //c = getchar();

      // reading unsatcore_evaluation.txt into SupportOfUnsatCore

      SupportOfUnsatCore.clear();
      ifstream *infile;
      infile = new ifstream();
      infile->open("unsatcore_evaluation.txt");

      if(!(infile->is_open()))
	{
	  cout<<"\nFile unsatcore_evaluation.txt cannot be opened in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAG\n";
	  exit(1);
	}

      while(!infile->eof())
	{
	  string s;
	  *infile>>s;
	  if(s=="")
	    break;
	  cout<<"Line read is "<<s<<endl;
	  SupportOfUnsatCore.insert(s);
	}
      infile->close();


      if(SupportOfUnsatCore.empty())
	{
	  cout<<"\nEmpty Unsat core in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAG\n";
	  exit(1);
	}



      gettimeofday(&unsat_core_time_total_end_ms, NULL);
      unsat_core_time_total_duration_ms = unsat_core_time_total_end_ms.tv_sec * 1000 + unsat_core_time_total_end_ms.tv_usec / 1000;
      unsat_core_time_total_duration_ms-= unsat_core_time_total_start_ms.tv_sec * 1000 + unsat_core_time_total_start_ms.tv_usec / 1000;
      unsat_core_time_total_ms = unsat_core_time_total_ms + unsat_core_time_total_duration_ms;	


      return result_str;
    }
   
}


void CSymbSimulator::printStateWiseModel(map<int, map<string, string> > &StateWiseModel, string filename)
{
  FILE *MyFile = fopen(filename.c_str(), "w");

  if(MyFile == NULL)
    {
      cout<<"\nCannot open "<<filename<<" in CSymbSimulator::printStateWiseModel\n";
      exit(1);
    }


  cout<<"\nStateWiseModel\n";

  for(map<int, map<string, string> >::iterator it = StateWiseModel.begin(); it != StateWiseModel.end(); it++)
    {
      int state = it->first;
      map<string, string> Model = it->second;

      cout<<"\nState = "<<state<<endl;

      for(map<string, string>::iterator it2 = Model.begin(); it2 != Model.end(); it2++)
	{
	  string signal = it2->first;
	  string value = it2->second;

	  fprintf(MyFile, "%s=%s\n", signal.c_str(), value.c_str());	  
	}// end for

    }// end for

  fclose(MyFile);
  
}// end function


void CSymbSimulator::printVariable_Value_Map_Signal_Removed(map<string, string> &Variable_Value_Map_Signal_Removed, string filename)
{
  FILE *MyFile = fopen(filename.c_str(), "w");

  if(MyFile == NULL)
    {
      cout<<"\nCannot open "<<filename<<" in CSymbSimulator::printVariable_Value_Map_Signal_Removed\n";
      exit(1);
    }

  for(map<string, string>::iterator it2 = Variable_Value_Map_Signal_Removed.begin(); it2 != Variable_Value_Map_Signal_Removed.end(); it2++)
    {
      string signal = it2->first;
      string value = it2->second;

      fprintf(MyFile, "%s=%s\n", signal.c_str(), value.c_str());	  
    }// end for

  fclose(MyFile);
  
}// end function



void CSymbSimulator::createRenameMaps(CDAGNode *dag, map<string, string> &RenameMap, map<string, string> &ReverseRenameMap)
{
  set<string> Variables;

  dag->getExistsDAGNames(Variables);
  dag->getDAGNames(Variables);

  for(set<string>::iterator it = Variables.begin();  it != Variables.end(); it++)
    {
      // get id_delta
      string id_delta = *it;
      // split into id and delta
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      string delta = id_delta.substr(index+1);
      // get qualmode_call_number as string
      char qualmode_call_number_char[100];
      sprintf(qualmode_call_number_char, "%d", qualmode_call_number);
      string qualmode_call_number_str(qualmode_call_number_char);

      // newid = id_qualmode_call_number as string
      string newid = id;
      newid += "_";
      newid += qualmode_call_number_str;
      // create newid_delta 
      string newid_delta = newid;
      newid_delta += "_";
      newid_delta += delta;   

      RenameMap.insert(make_pair(id_delta, newid_delta));
      ReverseRenameMap.insert(make_pair(newid_delta, id_delta));
    }
}


void CSymbSimulator::createRenamedWidthTable(map<string, int> &WidthTable, map<string, int> &RenamedWidthTable)
{

  for(map<string, int>::iterator it = WidthTable.begin();  it != WidthTable.end(); it++)
    {
      // get id_delta
      string id_delta = it->first;
      // get width
      int width = it->second;
    
      // split id_delta into id and delta
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      string delta = id_delta.substr(index+1);
      // get qualmode_call_number as string
      char qualmode_call_number_char[100];
      sprintf(qualmode_call_number_char, "%d", qualmode_call_number);
      string qualmode_call_number_str(qualmode_call_number_char);

      // newid = id_qualmode_call_number as string
      string newid = id;
      newid += "_";
      newid += qualmode_call_number_str;
      // create newid_delta 
      string newid_delta = newid;
      newid_delta += "_";
      newid_delta += delta;   

      RenamedWidthTable.insert(make_pair(newid_delta, width));
    }
}


/**
   Function that allows translation of binary strings to unsinged long long integers

**/
unsigned long long int CSymbSimulator::binaryStringToULINT(string str)
{
  int length=str.size();
  unsigned long long int num=0;
  unsigned long long int base=1;
  for(int i=length-1;i>=0;i--)
    {
	if((str[i]!='1') && (str[i]!='0'))
	{
	  cout<<"Unable to convert binary string to integer: "<<str<<" in CSymbSimulator::binaryStringToULINT"<<endl;
	  exit(1);
	}
      if(str[i]=='1')
        {
          num=num+base;
        }
      base=base*2;
    }
  return num;


}



string CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAGWithHashing(string signal, CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &SupportOfUnsatCore)
{
  
  bool hashing_enabled = true;

  if(hashing_enabled)
    {
      cout<<"\nsignal = "<<signal<<endl;
      cout<<"\nnode = "<<node->getDAGStr()<<endl;

      
      int index = signal.find_last_of("_");
      string id = signal.substr(0, index);
      int delta = atoi(signal.substr(index+1).c_str());
      int myshift = delta - 1;
      cout<<"\nmyshift = "<<myshift<<endl;
      
      string signal_renamed;
      CDAGNode *node_renamed;
      
      signal_renamed = id;
      signal_renamed += "_1";
      node_renamed = MANAGER.callCloneAndShiftWithDP(node, -1*myshift);
      
      cout<<"\nsignal_renamed = "<<signal_renamed<<endl;
      cout<<"\nnode_renamed = "<<node_renamed->getDAGStr()<<endl;


      map<string, string> Variable_Value_Map_Renamed;

      for(map<string, string>::iterator mit=Variable_Value_Map.begin(); mit!=Variable_Value_Map.end(); mit++)
	{
	  string variable = mit->first;
	  string value = mit->second;
	  string variable_renamed;
	 
	  int v_index = variable.find_last_of("_");
	  string v_id = variable.substr(0, v_index);
	  int v_delta = atoi(variable.substr(v_index+1).c_str());

	  string new_delta;

	  if(v_delta == myshift+1)
	    {
	      new_delta = "1";
	    }
	  else if(v_delta == myshift)
	    {
	      new_delta = "0";
	    } 
	  else
	    {
	      cout<<"\nInvalid v_delta "<<v_delta<<" in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAGWithHashing\n";
	      exit(1);
	    }
	  
	  variable_renamed = v_id;
	  variable_renamed += "_";
	  variable_renamed += new_delta;
  
	  Variable_Value_Map_Renamed.insert(make_pair(variable_renamed, value));
	}// for ends here
      
      cout<<"\nVariable_Value_Map_Renamed\n";

      for(map<string, string>::iterator mit=Variable_Value_Map_Renamed.begin(); mit!=Variable_Value_Map_Renamed.end(); mit++)
	{
	  string variable = mit->first;
	  string value = mit->second;
	  cout<<"\nvariable = "<<variable<<"\tvalue = "<<value<<endl;
	}// for ends here

      map<string, string> Variable_Value_Map_Renamed_Signal_Removed;

      string value_of_signal;

      // Variable_Value_Map_Renamed_Signal_Removed = Variable_Value_Map_Renamed \ {signal_renamed}

      for(map<string, string>::iterator it = Variable_Value_Map_Renamed.begin(); it != Variable_Value_Map_Renamed.end(); it++)
	{
	  if(it->first != signal_renamed)
	    {
	      Variable_Value_Map_Renamed_Signal_Removed.insert(make_pair(it->first, it->second));
	    }
	  else
	    {
	      value_of_signal = it->second;
	    }
	}//for ends here

      // Convert value of the signal_renamed into decimal

      unsigned long long int value_of_signal_in_decimal = binaryStringToULINT(value_of_signal);
      char value_of_signal_in_decimal_char[100];
      sprintf(value_of_signal_in_decimal_char, "%llu", value_of_signal_in_decimal);
      string value_of_signal_in_decimal_string(value_of_signal_in_decimal_char);

      // Get dag for signal_renamed <> value_of_signal i.e. DisEquationPart
      CDAGNode *variable = MANAGER.createDAG(id, 1);
      CDAGNode *constant = MANAGER.createDAG(value_of_signal_in_decimal_string);
      CDAGNode *DisEquationPart = MANAGER.createDAG("is_not_equal", variable, constant); 


      // Hashing : Check if already exists in the hash table

      set<string> SupportOfUnsatCoreRenamed;

      string result_str;

      bool exists_in_hash_table = false;

      exists_in_hash_table = checkIfEntryExistsInUnsatCoreHashTable(node_renamed, DisEquationPart, Variable_Value_Map_Renamed_Signal_Removed, SupportOfUnsatCoreRenamed);

      if(exists_in_hash_table)
	{
	  cout<<"\nHit in UnsatCoreHashTable\n";
	  unsat_core_hash_hits++;
	}
	  
      else
	{
	  cout<<"\nMiss in UnsatCoreHashTable\n";
	  unsat_core_hash_misses++;
	  result_str = evaluateAndGetSupportOfUnsatCoreOfGivenDAGOperatingWithHashing(node_renamed, DisEquationPart, Variable_Value_Map_Renamed_Signal_Removed, MyWidthTable, SupportOfUnsatCoreRenamed);

	  insertEntryInToUnsatCoreHashTable(node_renamed, DisEquationPart, Variable_Value_Map_Renamed_Signal_Removed, SupportOfUnsatCoreRenamed);

	  showUnsatCoreFindingHashTable();
	}      

      for(set<string>::iterator sit=SupportOfUnsatCoreRenamed.begin(); sit!=SupportOfUnsatCoreRenamed.end(); sit++)
	{
	  string variable = *sit;

	  string variable_renamed;
	  int v_index = variable.find_last_of("_");
	  string v_id = variable.substr(0, v_index);
	  int v_delta = atoi(variable.substr(v_index+1).c_str());

	  int new_delta;

	  if(v_delta == 1)
	    {
	      new_delta = myshift+1;
	    }
	  else if(v_delta == 0)
	    {
	      new_delta = myshift;
	    } 
	  else
	    {
	      cout<<"\nInvalid v_delta "<<v_delta<<" in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAGWithHashing\n";
	      exit(1);
	    }
	  
	  variable_renamed = v_id;
	  variable_renamed += "_";

	  char new_delta_char[10];
	  sprintf(new_delta_char, "%d", new_delta);
	  string new_delta_str(new_delta_char);
	  
	  variable_renamed += new_delta_str;
  
	  SupportOfUnsatCore.insert(variable_renamed);	  
	}  


      MANAGER.showSet(SupportOfUnsatCore,  "SupportOfUnsatCore");      

      return result_str;
    }
  else
    {
      string result_str = evaluateAndGetSupportOfUnsatCoreOfGivenDAG(signal, node, Variable_Value_Map, MyWidthTable, SupportOfUnsatCore);
      return result_str;
    }
   
}




string CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAGOperatingWithHashing(CDAGNode *node, CDAGNode *DisEquationPart, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &SupportOfUnsatCore)
{
      unsigned long long int unsat_core_time_total_duration_ms;
      struct timeval unsat_core_time_total_start_ms, unsat_core_time_total_end_ms;
      gettimeofday(&unsat_core_time_total_start_ms, NULL);
      
      string result_str;

      string value_of_signal;

      

      // Convert node into proper prop. skelton format

      node = convertExpressionToProperFormula(node, "rules_dnf.txt"); 
      node = convertExpressionToProperFormula(node, "rules_dnf.txt"); 
      node = convertExpressionToProperFormula(node, "rules_simplify.txt"); 

     // node = node /\ DisEquationPart

      node = MANAGER.createDAG("and", node, DisEquationPart);


      //cout<<"\nnode before printing in SMTLIB format = "<<node->getDAGStr()<<endl;

  
      // Write into a valid SMTLib file(in dag_in_proper_smtlib.txt)

      MANAGER.printDAGInSMTLibFormatInGivenFile(node, MyWidthTable, "dag_in_proper_smtlib.txt");
  
      // write Variable_Value_Map_Signal_Removed into model_evaluation.txt

      printVariable_Value_Map_Signal_Removed(Variable_Value_Map, "model_evaluation.txt");
      // call system("all ....") with proper arguments
      // get results in result_evaluation.txt and unsatcore_evaluation.txt


      //char c = getchar();

      system("rm unsatcore_evaluation.txt");


      //cout<<"\nCalling all....Press any key to continue\n";
      //char c = getchar();


      bool inputs_to_all_needed = false;

      if(inputs_to_all_needed)
	{
	  char all_call_number_char[100];
	  sprintf(all_call_number_char, "%d", all_call_number);
	  string all_call_number_str(all_call_number_char);

	  string dag_file = "formula_";
	  dag_file += all_call_number_str;
	  dag_file += ".txt";

	  string model_file = "model_";
	  model_file += all_call_number_str;
	  model_file += ".txt";

	  string command1 = "cp dag_in_proper_smtlib.txt ";
	  command1 += dag_file;

	  string command2 = "cp model_evaluation.txt ";
	  command2 += model_file;

	  system(command1.c_str());
	  system(command2.c_str());

	  all_call_number++;
	}


      time_t unsat_core_time_start, unsat_core_time_end, unsat_core_time_duration;

      time(&unsat_core_time_start);


      unsigned long long int unsat_core_time_duration_ms;
      struct timeval unsat_core_time_start_ms, unsat_core_time_end_ms;
      gettimeofday(&unsat_core_time_start_ms, NULL);


      string command = "all -get_core -model=model_evaluation.txt  -core_result=unsatcore_evaluation.txt dag_in_proper_smtlib.txt";
      system(command.c_str());

      gettimeofday(&unsat_core_time_end_ms, NULL);
      unsat_core_time_duration_ms = unsat_core_time_end_ms.tv_sec * 1000 + unsat_core_time_end_ms.tv_usec / 1000;
      unsat_core_time_duration_ms-= unsat_core_time_start_ms.tv_sec * 1000 + unsat_core_time_start_ms.tv_usec / 1000;
      unsat_core_time_ms = unsat_core_time_ms + unsat_core_time_duration_ms;	


      time(&unsat_core_time_end);

      unsat_core_time_duration = unsat_core_time_end - unsat_core_time_start;

      unsat_core_time = unsat_core_time + unsat_core_time_duration;



      //c = getchar();

      // reading unsatcore_evaluation.txt into SupportOfUnsatCore

      SupportOfUnsatCore.clear();
      ifstream *infile;
      infile = new ifstream();
      infile->open("unsatcore_evaluation.txt");

      if(!(infile->is_open()))
	{
	  cout<<"\nFile unsatcore_evaluation.txt cannot be opened in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAGOperatingWithHashing\n";
	  exit(1);
	}

      while(!infile->eof())
	{
	  string s;
	  *infile>>s;
	  if(s=="")
	    break;
	  cout<<"Line read is "<<s<<endl;
	  SupportOfUnsatCore.insert(s);
	}
      infile->close();


      if(SupportOfUnsatCore.empty())
	{
	  cout<<"\nEmpty Unsat core in CSymbSimulator::evaluateAndGetSupportOfUnsatCoreOfGivenDAGOperatingWithHashing\n";
	  exit(1);
	}



      gettimeofday(&unsat_core_time_total_end_ms, NULL);
      unsat_core_time_total_duration_ms = unsat_core_time_total_end_ms.tv_sec * 1000 + unsat_core_time_total_end_ms.tv_usec / 1000;
      unsat_core_time_total_duration_ms-= unsat_core_time_total_start_ms.tv_sec * 1000 + unsat_core_time_total_start_ms.tv_usec / 1000;
      unsat_core_time_total_ms = unsat_core_time_total_ms + unsat_core_time_total_duration_ms;	


      return result_str;

}


void CSymbSimulator::insertEntryInToUnsatCoreHashTable(CDAGNode *node_renamed, CDAGNode *DisEquationPart, map<string, string> &Variable_Value_Map_Renamed_Signal_Removed, set<string> &SupportOfUnsatCoreRenamed)
{
  char temp[100];
  string key, temp_key;

  sprintf(temp, "%x", node_renamed);
  temp_key = temp;
  key=temp_key;

  sprintf(temp, "%x", DisEquationPart);
  temp_key = temp;
  key+=temp_key;


  map<string, string> Core_Value_Map;
  
  for(map<string, string>::iterator vit = Variable_Value_Map_Renamed_Signal_Removed.begin(); vit != Variable_Value_Map_Renamed_Signal_Removed.end(); vit++)
    {
      if(SupportOfUnsatCoreRenamed.find(vit->first) != SupportOfUnsatCoreRenamed.end())//vit->first is a core signal. Hence insert it
	{
	  Core_Value_Map.insert(make_pair(vit->first, vit->second));
	}
    }
  
  map<string, list< map<string, string> > >::iterator it = UnsatCoreFindingHashTable.find(key);

  if(it == UnsatCoreFindingHashTable.end())// No entry exists for key
    {
      // create a new entry
      list< map<string, string> > ValueMapList;
      ValueMapList.push_back(Core_Value_Map);
      UnsatCoreFindingHashTable.insert(make_pair(key, ValueMapList));
    }
  else // Entry exists for key
    {
      (it->second).push_back(Core_Value_Map);
    }

}



bool CSymbSimulator::checkIfEntryExistsInUnsatCoreHashTable(CDAGNode *node_renamed, CDAGNode *DisEquationPart, map<string, string> &Variable_Value_Map_Renamed_Signal_Removed, set<string> &SupportOfUnsatCoreRenamed)
{
  char temp[100];
  string key, temp_key;

  sprintf(temp, "%x", node_renamed);
  temp_key = temp;
  key=temp_key;

  sprintf(temp, "%x", DisEquationPart);
  temp_key = temp;
  key+=temp_key;


  map<string, list< map<string, string> > >::iterator it = UnsatCoreFindingHashTable.find(key);

  if(it == UnsatCoreFindingHashTable.end())// No entry exists for key
    {
      // (node_renamed, DisEquationPart) is encountered for the
      // first time
      return false;
    }
  else // Entry exists for key
    {
      // (node_renamed, DisEquationPart) is already encountered
      list< map<string, string> > ValueMapList = it->second;
      
      for(list< map<string, string> >::iterator vit = ValueMapList.begin(); vit != ValueMapList.end(); vit++)
	{
	  map<string, string> ValueMap = *vit;
	  if(firstVMapImpliesSecondVMap(ValueMap, Variable_Value_Map_Renamed_Signal_Removed))// set of entries in first map is a subset of the set of entries in the second map
	    {
	      getSupportOfVMap(ValueMap, SupportOfUnsatCoreRenamed);
	      return true;
	    }
	}

      return false;
      
    }// else ends here

}


// returns true if set of entries in first map is a subset of the set of entries in the second map
// returns false otherwise
bool CSymbSimulator::firstVMapImpliesSecondVMap(map<string, string> &FirstValueMap, map<string, string> &SecondValueMap)
{
  for(map<string, string>::iterator it=FirstValueMap.begin(); it!=FirstValueMap.end(); it++)
    {
      string signal = it->first;
      string value = it->second;

      map<string, string>::iterator vit = SecondValueMap.find(signal);
      
      if(vit == SecondValueMap.end())// no entry for signal
	{
	  return false;
	}
      if(vit->second != value)// value for signal is different
	{
	  return false;
	}
    }// for
  return true;// for all signals in FirstValueMap, entry exists
  //in SecondValueMap and the entries are matching
}


void CSymbSimulator::getSupportOfVMap(map<string, string> &ValueMap, set<string> &SupportOfUnsatCoreRenamed)
{
  for(map<string, string>::iterator it=ValueMap.begin(); it!=ValueMap.end(); it++)
    {
      SupportOfUnsatCoreRenamed.insert(it->first);
    }// for ends here

}


void CSymbSimulator::showUnsatCoreFindingHashTable()
{
  cout<<"\nUnsatCoreFindingHashTable"<<endl;

  for(map<string, list< map<string, string> > >::iterator it = UnsatCoreFindingHashTable.begin(); it !=  UnsatCoreFindingHashTable.end(); it++)
    {
      cout<<"\nkey = "<<it->first;
      list< map<string, string> > ValueMapList = it->second;
      
      cout<<"\nValueMapList\n";

      for(list< map<string, string> >::iterator vit = ValueMapList.begin(); vit != ValueMapList.end(); vit++)
	{
	  cout<<"\nValueMap\n";

	  map<string, string> ValueMap = *vit;
	  
	  for(map<string, string>::iterator mit = ValueMap.begin(); mit != ValueMap.end(); mit++)
	    {
	      cout<<mit->first<<"\t"<<mit->second<<endl;
	    }// for map
	  
	}// for list
    
    }// for map
}



// Signals which are in the support of property read from property_support.txt
void CSymbSimulator::getPropertySupport(set<string> &PropertySupport)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("property_support.txt");
  while(!infile->eof())
    {
      string s;
      *infile>>s;
      if(s=="")
	break;
      //cout<<"Line read is "<<s<<endl;
      PropertySupport.insert(s);
    }

  infile->close();

}


void CSymbSimulator::getOrderOfEvaluationForSignals(list<string> &OrderOfEvaluationForSignals, map<string, CDAGNode*> &FunctionMap, CEntity &ent, int frame_number)
{
  // get the input identifiers of the top-level entity
  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
  set<string> input_id_names;
  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names);// input_id_names obtained

  map<string, set<string> > SignalDependencyTable;

  
  for(map<string, CDAGNode*>::iterator fit = FunctionMap.begin(); fit != FunctionMap.end(); fit++)
    {
      string signal = fit->first;
      CDAGNode *Function = fit->second;

      set<string> Support;
      Function->getDAGNames(Support);
      
      set<string> LimitedSupport;
      getLimitedSupportFromSupport(Support, input_id_names, signal, frame_number, LimitedSupport);// LimitedSupport = Support \ {this signal, signals at delta time frame_number, input signals}

      SignalDependencyTable.insert(make_pair(signal, LimitedSupport));   
    }

 //  cout<<"\nSignalDependencyTable\n";
//   for(map<string, set<string> >::iterator fit = SignalDependencyTable.begin(); fit != SignalDependencyTable.end(); fit++)
//     {
//       string signal = fit->first;
//       set<string> Support;
//       Support = fit->second;
//       cout<<"\nsignal = "<<signal<<endl;
//       showSet(Support, "Support");
//     }

  
  map<string, set<string> > NewSignalDependencyTable;
  set<string> AddedSignals;
  
  while(!SignalDependencyTable.empty())
    {
      for(map<string, set<string> >::iterator fit = SignalDependencyTable.begin(); fit != SignalDependencyTable.end(); fit++)
	{
	  string signal = fit->first;
	  set<string> Support;
	  Support = fit->second;

	  //cout<<"\nsignal = "<<signal<<endl;
	  //showSet(Support, "Support");


	  set<string> RemainingSignals;

	  set_difference(Support.begin(), Support.end(), AddedSignals.begin(), AddedSignals.end(),inserter(RemainingSignals, RemainingSignals.begin()));

	  //showSet(RemainingSignals, "RemainingSignals");

	  if(RemainingSignals.empty())// All signals in the dependency list of this signal are already added
	    {
	      OrderOfEvaluationForSignals.push_back(signal);
	      AddedSignals.insert(signal);
	    }
	  else
	    {
	     NewSignalDependencyTable.insert(make_pair(signal, Support));    
	    }
	}// for each signal in SignalDependencyTable

      SignalDependencyTable = NewSignalDependencyTable;

      NewSignalDependencyTable.clear();


      // cout<<"\nSignalDependencyTable\n";
//       for(map<string, set<string> >::iterator it2 = SignalDependencyTable.begin(); it2 != SignalDependencyTable.end(); it2++)
// 	{
// 	  string signal = it2->first;
// 	  set<string> Support;
// 	  Support = it2->second;
// 	  cout<<"\nsignal = "<<signal<<endl;
// 	  showSet(Support, "Support");
// 	}
 
    }// while(!SignalDependencyTable.empty())


//   cout<<"\nOrderOfEvaluationForSignals\n";
//   for(list<string>::iterator it = OrderOfEvaluationForSignals.begin(); it != OrderOfEvaluationForSignals.end(); it++)
//     {
//       cout<<endl<<*it<<endl;
//     }
  
}

// LimitedSupport = Support \ {signals at delta time frame_number, input signals}
void CSymbSimulator::getLimitedSupportFromSupport(set<string> &Support, set<string> &input_id_names, string signal, int frame_number, set<string> &LimitedSupport)
{
  for(set<string>::iterator it = Support.begin(); it != Support.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      int delta = atoi(id_delta.substr(index+1).c_str());

      if(delta != frame_number && input_id_names.find(id)==input_id_names.end() && id_delta != signal)
	{
	  LimitedSupport.insert(id_delta);
	}// if
    }// for
}


void CSymbSimulator::eliminateAssignmentsToInternalSignals(map<string, string> &Model, CEntity &ent)
{
  // get the input identifiers of the top-level entity
  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
  set<string> input_id_names;
  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names); 

  map<string, string> NewModel;
  
  for(map<string, string>::iterator it = Model.begin(); it != Model.end(); it++)
    {
      string variable = it->first;

      bool choose_value = false;

      int index = variable.find_last_of("_");
      string id = variable.substr(0, index);
      int delta = atoi(variable.substr(index+1).c_str());

      if(delta == 0)
	{
	  choose_value = true;
	}
      else if(input_id_names.find(id)!=input_id_names.end())
	{
	  choose_value = true;
	}
	    



      if(choose_value)
	{
	  NewModel.insert(make_pair(variable, it->second));
	}
      else
	{
	  //cout<<endl<<"Value for variable "<<variable<<"removed from model\n"; 
	}
    }// for each variable in Model

  Model = NewModel;
}



void CSymbSimulator::extendVariable_Value_Map_Modified(CDAGNode *CVC, map<string, string> &Model, map<string, int> &WidthTableForCVC, CEntity &ent)
{
  //cout<<"\nCVC = "<<CVC->getDAGStr()<<endl; // Temporary



  set<string> Support;
  CVC->getDAGNames(Support);


  // get the input identifiers of the top-level entity
  set<CSensitiveElement*> identifiers, input_identifiers, output_identifiers, intermediate_identifiers;
  getIdentifiers(identifiers, input_identifiers, output_identifiers, intermediate_identifiers,ent);
  set<string> input_id_names;
  getNamesFromCSensitiveElementSet(input_identifiers, input_id_names);  
  

  srand((unsigned)time(0));
  
  for(set<string>::iterator it = Support.begin(); it != Support.end(); it++)
    {
      string variable = *it;
      map<string, string>::iterator mit = Model.find(variable);
      
      if(mit == Model.end())
	{
	  //cout<<endl<<"variable "<<variable<<" does not occure in Model\n";

	  // split variable into name and delta
	  // if delta = 0, i.e. intial state; assign random value to it
	  // else if name is an input variable, assign random value to it

	  bool choose_value = false;

	  int index = variable.find_last_of("_");
	  string id = variable.substr(0, index);
	  int delta = atoi(variable.substr(index+1).c_str());

	  if(delta == 0)
	    {
	      choose_value = true;
	    }
	  else if(input_id_names.find(id)!=input_id_names.end())
	    {
	      choose_value = true;
	    }
	    



	  if(choose_value)
	    {
	      map<string, int>::iterator wit = WidthTableForCVC.find(variable);

	      if(wit == WidthTableForCVC.end())
		{
		  cout<<endl<<"variable "<<variable<<" has no entry in WidthTableForCVC in CSymbSimulator::extendVariable_Value_Map_Modified\n";
		  exit(1);
		}

	      int width = wit->second;

	      unsigned long long int Range = findPower(width);
	  
	      unsigned long long int random_no = rand()% Range;

	      string bin_value = MANAGER.ULINTToBinaryString(random_no); // binary value with minimum required width

	      //cout<<endl<<"variable "<<variable<<" given the value "<<bin_value<<" \n";

	      Model.insert(make_pair(variable, bin_value));
	    }
	  else
	    {
	      //cout<<endl<<"variable "<<variable<<" is not assigned a radom value \n"; 
	    }
	}// if(mit == Model.end())	  
	  
    }// for each variable in Support  
}


void CSymbSimulator::getNewProblematicSignalsInCurrentState_ForSignal(set<string> &ProblematicSignals_i_ForSignal, int current, set<string> &NewProblematicSignalsInCurrentState_ForSignal)
{
  for(set<string>::iterator it = ProblematicSignals_i_ForSignal.begin(); it != ProblematicSignals_i_ForSignal.end(); it++)
    {
      string id_delta = *it;

      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      int delta = atoi(id_delta.substr(index+1).c_str());
      
      if(delta == current)
	{
	  NewProblematicSignalsInCurrentState_ForSignal.insert(id_delta);
	}
      
    }//for
}//function


CDAGNode* CSymbSimulator::callQualmode(CDAGNode *dag, map<string, int> &WidthTable)
{

  // The first step is to recreate the dag exists(X, DAG) by removing from X, the variables
  // not in support(DAG)

  bool recreation_of_dag = true;

  if(recreation_of_dag)
    {
      string value = dag->Get_DAGnode_value();
      if(value != "exists") return dag;
      else
	{
	  list<CDAGNode*>::iterator iout = dag->Get_DAGnode_outlist();
	  CDAGNode *exists_part = *iout;
	  iout++;
	  CDAGNode *dag_part = *iout;

	  // Check if dag_part is free of all variables to be eliminated

	  set<string>  VarsToElim;
	  set<string>  VarsPresent;
	  set<string>  VarsCommon;
  
	  exists_part->getDAGNames(VarsToElim);
	  dag_part->getDAGNames(VarsPresent);

	  set_intersection(VarsToElim.begin(), VarsToElim.end(), VarsPresent.begin(), VarsPresent.end(),inserter(VarsCommon, VarsCommon.begin()));
      
	  if(VarsCommon.empty())
	    {
	      //cout<<"\nNo variables left to eliminate\n";
	      return dag_part;
	    }
      
	  // recreate the dag
	  dag = MANAGER.createDAG("exists", getDAGFromStringSet(VarsCommon), dag_part);
	}
    }// if(recreation_of_dag)


  // check if already exists in hash table
  string key;
      
  char temp_char[10];
  sprintf(temp_char , "%x", dag);
  string dag_addr(temp_char);
  key = dag_addr;
  // key is formed

  // check if already exists
  HTStatusValue<CDAGNode*> result = Qualmode_HT.hashtable_search(key);
  if(result.success()) // Exists in HT
    {
      qualmode_ht_hash_hits++;
      return result.GetValue();
    }
      
  qualmode_ht_hash_misses++;

  map<string, string> RenameMap;
	  
  map<string, string> ReverseRenameMap;

  map<string, int> RenamedWidthTable;
      
  createRenameMaps(dag, RenameMap, ReverseRenameMap);

  createRenamedWidthTable(WidthTable, RenamedWidthTable);


  //cout<<"\nOriginal DAG before QuaLMoDE = "<<dag->getDAGStr()<<endl;


  CDAGNode *RenamedDAG = MANAGER.cloneWithRename(dag, RenameMap);

  //cout<<"\nRenamedDAG = "<<RenamedDAG->getDAGStr()<<endl;

	        
  CDAGNode *RenamedTemp = Quantifier_Eliminator_With_Redundant_Selection_Removal(RenamedDAG, RenamedWidthTable);


  //cout<<"\nRenamedTemp = "<<RenamedTemp->getDAGStr()<<endl;

  CDAGNode *qeliminated_dag = MANAGER.cloneWithRename(RenamedTemp, ReverseRenameMap);

  //cout<<"\nFinal DAG = "<<temp->getDAGStr()<<endl;

  qualmode_call_number++;

  result = Qualmode_HT.hashtable_insert(key, qeliminated_dag);
   
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function CSymbSimulator::callQualmode\n";
      exit(1);
    }     


  return qeliminated_dag;
}




/* FUNCTIONS ADDED ON 31-01-2011 ONWARDS - FOR IMPLEMENTING REFINEMENT END HERE*/


/* FUNCTIONS ADDDED ON 18-08-2016 ONWARDS - TO INTEGRATE WITH THE LATEST QE */

CDAGNode* CSymbSimulator::Quantifier_Eliminator_With_Redundant_Selection_Removal(CDAGNode* formula_with_exists, map<string, int> &WidthTable)
{
	if(formula_with_exists->Get_DAGnode_value() != "exists")
    	{
      		//cout<<"\nNo variables to eliminate. Return\n";
      		return formula_with_exists;
    	}
	else
	{
		
 		map<CDAGNode*, CDAGNode*> LinearizationTable;
		map<CDAGNode*, CDAGNode*> SubtractionTable;
  		int AutoNameCount = 0;

		// Separating the formula_with_exists into exists part and dag part
		set<string> VarsToElimOriginal;
  		set<string> VarsToElimRenamed;
  		formula_with_exists->getExistsDAGNames(VarsToElimOriginal);
  		list<CDAGNode*>::iterator iout = formula_with_exists->Get_DAGnode_outlist();
  		iout++;
  		CDAGNode *DagPart = *iout;

  		bool calling_quicksimplify_before_dd_creation = true;
		if(calling_quicksimplify_before_dd_creation)
		{
			map<string, string> QuickSimplificationTable;
		      	DagPart = MANAGER.callQuickSimplify(DagPart, QuickSimplificationTable);
		}

		// Linearizing the dag part

		//cout<<"\nVarsToElimOriginal\n";
		set<string>::iterator vit =  VarsToElimOriginal.begin();
		for(;vit !=  VarsToElimOriginal.end(); vit++)
		{
			//cout<<*vit<<endl;
		}

  		//cout<<"\nDag part = "<<DagPart->getDAGStr()<<"\n";
		DagPart = MANAGER.getPreprocessedClone(DagPart);
		//cout<<"\nDag part after replacement of predicates = "<<DagPart->getDAGStr()<<"\n";

		//cout<<"\nWidth table\n";
  		map<string, int>::iterator wit =  WidthTable.begin();
  		for(;wit !=  WidthTable.end(); wit++)
    		{
      			//cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
    		}


		//cout<<"\nGetting the linearized dag\n";
		bool make_autonames_independent = true;
		CDAGNode* LinearDag;
  		if(make_autonames_independent)
    		{
      			LinearDag = MANAGER.getLinearizedCloneWithGlobalCount(DagPart, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
    		}
  		else
    		{
      			LinearDag = MANAGER.getLinearizedClone(DagPart, LinearizationTable, AutoNameCount, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
    		}
		//cout<<"\nLinearDag = "<<LinearDag->getDAGStr()<<endl;

  		//cout<<"\nLinearization table\n";
  		map<CDAGNode*, CDAGNode*>::iterator lit =  LinearizationTable.begin();
  		for(;lit !=  LinearizationTable.end(); lit++)
    		{
      			//cout<<"\n"<<(lit->first)->getDAGStr()<<"\t"<<(lit->second)->getDAGStr()<<endl;
    		}

		//cout<<"\nWidth table\n";
  		wit =  WidthTable.begin();
  		for(;wit !=  WidthTable.end(); wit++)
    		{
      			//cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
    		}


		SubtractionTable.clear();
  		LinearDag = MANAGER.getSubtractionReplacedClone(LinearDag, SubtractionTable, WidthTable);
  		//cout<<"\nAfter replacing subtractions by additions, LinearDag = "<<LinearDag->getDAGStr()<<endl;

		//cout<<"\nVarsToElimRenamed\n";
  		vit =  VarsToElimRenamed.begin();
  		for(;vit !=  VarsToElimRenamed.end(); vit++)
    		{
      			//cout<<*vit<<endl;
    		}


  		// Creating VarsToElim
		list<string> VarsToElim;
		set<string> names;
  		LinearDag->getDAGNames(names);
  		for(set<string>::iterator it=names.begin(); it != names.end(); it++)
    		{
			string name = *it;
      			//cout<<"\nname = "<<name<<endl;
      			if( VarsToElimOriginal.find(name) != VarsToElimOriginal.end() || VarsToElimRenamed.find(name) != VarsToElimRenamed.end())	
			{
				VarsToElim.push_back(name);
			}
    		}

		//cout<<"\nVarsToElim\n";
		for(list<string>::iterator vars_it = VarsToElim.begin();vars_it !=  VarsToElim.end(); vars_it++)
    		{
      			//cout<<*vars_it<<endl;
    		}

		set<string> VarSetToElim;
		for(list<string>::iterator vars_it = VarsToElim.begin();vars_it !=  VarsToElim.end(); vars_it++)
    		{
      			VarSetToElim.insert(*vars_it);
    		}

  		//cout<<"\nVarSetToElim\n";
  		for(set<string>::iterator vars_it = VarSetToElim.begin();vars_it !=  VarSetToElim.end(); vars_it++)
    		{
      			//cout<<*vars_it<<endl;
    		}

		MANAGER.checkForPropSkeltonModified(LinearDag);
		//cout<<"\n\nDag's structure confirmed to be propositional\n\n";


  		bool input_needed = true;
		if(input_needed) 
    		{
		       //cout<<"\nLinearDag = "<<LinearDag->getDAGStr()<<endl;
		       //cout<<"\nVarSetToElim\n";
		       for(set<string>::iterator vars_it = VarSetToElim.begin();vars_it !=  VarSetToElim.end(); vars_it++)
			{
				//cout<<*vars_it<<endl;
			}
			MANAGER.printQEliminatorResultsInFile(LinearDag, WidthTable, VarsToElim);
		}

		// HERE
		// 1) Convert from CDAGNode* to DagNode*
		DAGNode* LinearDag_for_qe = convertCDAGNodeToDAGNode(LinearDag, WidthTable);
		assert(LinearDag_for_qe != NULL);
		//cout << "\nLinearDag_for_qe = " << getDAGStr(LinearDag_for_qe);
		
		//cout<<"\nVarSetToElim\n";
		for(set<string>::iterator vars_it = VarSetToElim.begin();vars_it !=  VarSetToElim.end(); vars_it++)
		{
			//cout<<*vars_it<<endl;
		}

		// 2) Call QE
		//cout<<"\nQuantifier eliminator called ...";

		time_t qe_start_time;
		time_t qe_end_time;

		time(&qe_start_time);
		
		// Setting options
		qe_call_counter++;
		DAGNode* LinearDag_after_qe;
		LMI_handling_enabled = true;
		order_lmes_before_lmis_in_normalization = true;
		string trname_to_print = "TR_";
		char qe_call_counter_chr[100];
  		sprintf(qe_call_counter_chr,"%d",qe_call_counter);
  		string qe_call_counter_str = qe_call_counter_chr;
		trname_to_print += qe_call_counter_str;
		InFileName = trname_to_print;

		openFilesBeforeQE();

		if(quantifier_eliminator == "qecombined")
		{
			//cout << "\nqecombined\n";
			ConvertProblemIntoDisjunctionOfTailedTriangles = true;
			PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = true;
			TimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = 20;
			
			LinearDag_after_qe = ddm.Quantifier_Eliminator(LinearDag_for_qe, VarSetToElim, WidthTable);
		}
		else if(quantifier_eliminator == "qesmt")
		{
			//cout << "\nqesmt\n";
			LinearDag_after_qe = Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForLMIs(LinearDag_for_qe, VarSetToElim, WidthTable);
		}
		else if(quantifier_eliminator == "qelmdd")
		{
			//cout << "\nqelmdd\n";
			LinearDag_after_qe = ddm.Quantifier_Eliminator(LinearDag_for_qe, VarSetToElim, WidthTable);
		}
		else
		{
			cout << "\nUnanticipated quantifier_eliminator " << quantifier_eliminator << endl;
			assert(false);
		}

		assert(LinearDag_after_qe != NULL);
		closeFilesAfterQE();

		time(&qe_end_time);
		total_qe_time_secs = total_qe_time_secs + qe_end_time - qe_start_time;

		//cout << "\nLinearDag_after_qe = " << getDAGStr(LinearDag_after_qe);
		
		// 3) Convert result from DagNode* to CDAGNode*
		CDAGNode* dag_eliminated;
		dag_eliminated = convertDAGNodeToCDAGNode(LinearDag_after_qe, WidthTable);
		//cout << "\ndag_eliminated = " << dag_eliminated->getDAGStr();
				
	
  		bool calling_quicksimplify = true;
		if(calling_quicksimplify)
    		{
      			map<string, string> QuickSimplificationTable;
      			dag_eliminated = MANAGER.callQuickSimplify(dag_eliminated, QuickSimplificationTable);
    		}

  		// CALL To CHECK EXACTNESS

		if(false)
   		{
			bool exact = MANAGER.checkForExactness( LinearDag, dag_eliminated, WidthTable, VarsToElim);
			if(exact)
        		{
          			cout<<"\nExact\n";
			}
        		else
			{
          			cout<<"\nInexact\n";
          			exit(1);
			}
    		}
      
  		//  CALL To CHECK EXACTNESS ENDS HERE

  		//cout<<"\nNon linearizing the dag\n";
		CDAGNode* dag_eliminated_orig;
		if(!LinearizationTable.empty())
    		{
      			//cout<<"\nLinearization table\n";
      			lit =  LinearizationTable.begin();
      			for(;lit !=  LinearizationTable.end(); lit++)
        		{
          			//cout<<"\n"<<(lit->first)->getDAGStr()<<"\t"<<(lit->second)->getDAGStr()<<endl;
        		}
			dag_eliminated_orig = MANAGER.getOriginalClone(dag_eliminated, LinearizationTable);
    		}
		else
    		{
      			dag_eliminated_orig = dag_eliminated;
    		}

		//cout<<"\ndag_eliminated_orig = "<<dag_eliminated_orig->getDAGStr()<<endl;
		
  		return dag_eliminated_orig; 
	}
}



/* FUNCTIONS ADDDED ON 18-08-2016 ONWARDS - TO INTEGRATE WITH THE LATEST QE ENDS HERE */



CDAGNode* CSymbSimulator::Qualmode_Blasting(CDAGNode* formula_with_exists, map<string, int> &WidthTable, int frame_number)
{
	cout << "\nCSymbSimulator::Qualmode_Blasting is not implemented yet!!\n";
	exit(1);
}


DAGNode* CSymbSimulator::convertCDAGNodeToDAGNode(CDAGNode* LinearDag, map<string, int> &WidthTable)
{
	map<CDAGNode*, DAGNode*> DPTable;
	DPTable.clear();
	DAGNode* LinearDag_DAG = convertCDAGNodeToDAGNode_recur(LinearDag, WidthTable, DPTable);
	return LinearDag_DAG;
}


DAGNode* CSymbSimulator::convertCDAGNodeToDAGNode_recur(CDAGNode* LinearDag, map<string, int> &WidthTable, map<CDAGNode*, DAGNode*> &DPTable)
{
	map<CDAGNode*, DAGNode*>::iterator it = DPTable.find(LinearDag);
	if(it != DPTable.end())
	{
		//cout << "\nHT hit!\n";
		return it->second;
	}
	else
	{
		//cout << "\nHT miss!\n";
		DAGNode* LinearDag_DAG;

		string name = LinearDag->Get_DAGnode_value();
	  	int delta_time = LinearDag->Get_DAGnode_delta_time_as_int();
 		bool ntype = LinearDag->Get_DAG_node_type();
		//cout << "\nname = " << name << "\tdelta_time = " << delta_time << endl;
		string dag_str;

		if(!ntype) // variable
		{
		    
			char temp[10];
		      	sprintf(temp, "%d", delta_time);
			dag_str = name;
		      	dag_str += "_";
		      	dag_str += temp;

			int width;
			map<string, int>::iterator wit = WidthTable.find(dag_str);
			assert(wit != WidthTable.end());
			width = wit->second;

			LinearDag_DAG = createBVVariable(dag_str, width);
		}
		else if(LinearDag->Get_DAGnode_outlist_size() == 0) //constant
		{
	  		//cout<<"\nNumber "<<name<<" inside the function convertTreeToDAG in the file CExprSimpStructure.h\n";
			if(name == "true" || name == "1")
			{
				dag_str = "true";
				LinearDag_DAG = createLogicalConstant(dag_str);
			}
			else if(name == "false" || name == "0")
			{
				dag_str = "false";
				LinearDag_DAG = createLogicalConstant(dag_str);
			}
			else if(name == "'1'")
			{
				dag_str = "1";
				int width = 1;
				LinearDag_DAG = createBVConstant(dag_str, width);
			}
			else if(name == "'0'")
			{
				dag_str = "0";
				int width = 1;
				LinearDag_DAG = createBVConstant(dag_str, width);
			}
			else
			{
				assert(name[0] == 'x');
				
				string hex = name;
				hex.erase(hex.begin(), hex.begin()+2);
  				hex.erase(hex.end()-1, hex.end());
  				// now hex is in proper hexa decimal form
  				ULINT number = convertToDecimal(hex);

				char temp[100];
				sprintf(temp, "%d", number);
  				string dag_str(temp);

				int width;
				width = hex.size()*4;
				
				LinearDag_DAG = createBVConstant(dag_str, width);	
			}
		}
		else
    		{
     			if(name == "and" || name == "or" || name == "not")
			{
	  			dag_str = name;
				
			}
			else if(name == "ite")
			{
				dag_str = "if_then_else";
			}
			else if(name == "equivalent" || name == "=")
			{
	  			dag_str =  "=";
			}
			else if(name == "is_not_equal" || name == "not_equal_to")
			{
				dag_str = "is_not_equal";
			}
			else if(name == "<=")
		  	{
		    		dag_str = "bvule";
		  	}
			else if(name == ">=")
		  	{
		  	  	dag_str = "bvuge";
		  	}
			else if(name == "<")
		  	{
		    		dag_str = "bvult";
		  	}
			else if(name == ">")
		  	{
		    		dag_str = "bvugt";
		  	}
			else if(name == "*")
	      		{
				dag_str = "bvmul";
			}
			else if(name == "+")
	      		{
				dag_str = "bvadd";
			}
			else
	 		{
	 			assert(false);
			}
		
			list<CDAGNode*>::iterator iout = LinearDag->Get_DAGnode_outlist();
    			unsigned int outlist_size = LinearDag->Get_DAGnode_outlist_size();
    
    			switch(outlist_size)
      			{
      				case 1:
				{
				  	DAGNode* child1 = convertCDAGNodeToDAGNode_recur(*iout, WidthTable, DPTable);
					LinearDag_DAG = createDAG(dag_str, child1, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
			  	
				  	break;
				}
				case 2:
				{
				  	DAGNode* child1 = convertCDAGNodeToDAGNode_recur(*iout, WidthTable, DPTable);
					iout++;
					DAGNode* child2 = convertCDAGNodeToDAGNode_recur(*iout, WidthTable, DPTable);
		
					if(dag_str == "is_not_equal")
					{
						LinearDag_DAG = createDAG("not", createDAG("=", child1, child2, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH), BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
					}
					else
					{
						LinearDag_DAG = createDAG(dag_str, child1, child2, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
					}					
			  	
				  	break;

				}
				case 3:
				{
				  	DAGNode* child1 = convertCDAGNodeToDAGNode_recur(*iout, WidthTable, DPTable);
					iout++;
					DAGNode* child2 = convertCDAGNodeToDAGNode_recur(*iout, WidthTable, DPTable);
					iout++;
					DAGNode* child3 = convertCDAGNodeToDAGNode_recur(*iout, WidthTable, DPTable);
					LinearDag_DAG = createDAG(dag_str, child1, child2, child3, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
			  	
				  	break;
				  	
				}
			       default: 
				{
				 	assert(false);
				}
      			   }

		}

		DPTable.insert(make_pair(LinearDag, LinearDag_DAG));
		return LinearDag_DAG;
	}

	
}


void CSymbSimulator::openFilesBeforeQE()
{

	LMIQEOutputDataFp = fopen("LMIQEOutputData.txt", "a+");
	assert(LMIQEOutputDataFp != NULL);

	LMIQEImportantDataFp = fopen("LMIQEImportantData.txt", "a+");
	assert(LMIQEImportantDataFp != NULL);

	LMIQEImportantDataFourierMotzkinFp = fopen("LMIQEImportantDataFourierMotzkin.txt", "a+");
	assert(LMIQEImportantDataFourierMotzkinFp != NULL);

	LMIQEImportantDataAdditionalFp = fopen("LMIQEImportantDataAdditional.txt", "a+");
	assert(LMIQEImportantDataAdditionalFp != NULL);

	//extern FILE* LMIQEFinishedLayer3DataFp;
	//extern FILE* LMIQEFinishedLayer3TimedOutDataFp;
	//extern FILE* LMIQEFinishedLayer3SCAchievedDataFp;
	//extern FILE* LMIQEFinishedLayer3NoSCDataFp;
	//extern FILE* LMIQEFinishedLayer3TrueDataFp;
	//extern FILE* LMIQEImportantDataOTFp;
	//extern FILE* hashingFp;
	//extern FILE* PathBasedStrategyFp;
	//extern FILE* MonEnabledAllVarsFp;
	//extern FILE* ConversionToDisjunctionOfProblemsFp;
	//extern FILE* ConversionToDisjunctionOfProblemsWithLMEsFp;
	//extern FILE* LMEBasedSimplificationAsPreprocessor_OutputFile;
	//extern FILE* SimplifyLMDD_OutputFile;
	//extern FILE* QELMDD_OutputFile;
	//extern FILE* ConversionToDisjunctionOfProblemsWithSMTSolverFp;
	//extern FILE* ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp;
	//extern FILE* LMIQEImportantDataOTAfterLayer2Fp;

	TimeAnalysisFourierMotzkinFp = fopen("TimeAnalysisFourierMotzkin.txt", "a+");
	assert(TimeAnalysisFourierMotzkinFp != NULL);

	TimeAnalysisFp = fopen("TimeAnalysis.txt", "a+");
	assert(TimeAnalysisFp != NULL);

	TimeLayer2ChecksFp = fopen("TimeLayer2Checks.txt", "a+");
	assert(TimeLayer2ChecksFp != NULL);

	CumulativeTimeLayer2ChecksFp = fopen("CumulativeTimeLayer2Checks.txt", "a+");
	assert(CumulativeTimeLayer2ChecksFp != NULL);

	//extern FILE* OutputDataLayer2ChecksFp;

	log_file = fopen("log_file.txt", "a+");
	assert(log_file != NULL);
	
	ImpCheckFile = fopen("ImpCheckFile.txt", "a+");
	assert(ImpCheckFile != NULL);
	
	layer1_calls_fp = fopen("layer1_calls.txt", "a+");
	assert(layer1_calls_fp != NULL);

	layer2_calls_fp = fopen("layer2_calls.txt", "a+");
	assert(layer2_calls_fp != NULL);

	//FILE *apqelim_fp;
	//FILE *APQElimStatusFile;

	FILE_Blowup = fopen("FILE_Blowup.txt", "a+");
	assert(FILE_Blowup != NULL);

	LMIQEImportantDataBlastingFp = fopen("LMIQEImportantDataBlasting.txt", "a+");
	assert(LMIQEImportantDataBlastingFp != NULL);

	LMIQELayer1EffectivenessFp = fopen("LMIQELayer1Effectiveness.txt", "a+");
	assert(LMIQELayer1EffectivenessFp != NULL);

}

void CSymbSimulator::closeFilesAfterQE()
{
	
	fclose(LMIQEOutputDataFp);

	fclose(LMIQEImportantDataFp);

	fclose(LMIQEImportantDataFourierMotzkinFp);

	fclose(LMIQEImportantDataAdditionalFp);

	fclose(TimeAnalysisFourierMotzkinFp);

	fclose(TimeAnalysisFp);

	//fclose(TimeLayer2ChecksFp);

	fclose(CumulativeTimeLayer2ChecksFp);

	fclose(log_file);
	
	fclose(ImpCheckFile);
	
	fclose(layer1_calls_fp);

	fclose(layer2_calls_fp);

	fclose(FILE_Blowup);

	fclose(LMIQEImportantDataBlastingFp);

	fclose(LMIQELayer1EffectivenessFp);
}


CDAGNode* CSymbSimulator::convertDAGNodeToCDAGNode(DAGNode* dag, map<string, int> &WidthTable)
{
	map<DAGNode*, CDAGNode*> DPTable;
	DPTable.clear();
	CDAGNode* dag_changed = convertDAGNodeToCDAGNode_recur(dag, WidthTable, DPTable);
	return dag_changed;
}


CDAGNode* CSymbSimulator::convertDAGNodeToCDAGNode_recur(DAGNode* dag, map<string, int> &WidthTable, map<DAGNode*, CDAGNode*> &DPTable)
{
	map<DAGNode*, CDAGNode*>::iterator it = DPTable.find(dag);
	if(it != DPTable.end())
	{
		//cout << "\nHT hit!\n";
		return it->second;
	}
	else
	{
		//cout << "\nHT miss!\n";
		CDAGNode* dag_changed;

		Value* MyValue = (Value*)(dag->getValue());
		Type MyType = MyValue->get_node_type();
      		//int MyWidth = MyType.width;
      		int MyDataType = MyType.data_type;

		if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && dag->Get_outlist_size() == 0)  //variable
 	       	{
			//cout << "\nvariable\n";

			string id_delta = dag->Get_node_name();
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index); // id is now x
			int delta = atoi(id_delta.substr(index+1).c_str()); // Note that delta = delta_time				
			dag_changed = MANAGER.createDAG(id, delta);
			assert(dag_changed != NULL);
		}
		else if(dag->Get_outlist_size() == 0) //constant
		{
			//cout << "\nconstant\n";

	  		string dag_str = dag->Get_node_name();
			dag_changed = MANAGER.createDAG(dag_str);
			assert(dag_changed != NULL);
		}
		else // operation
    		{
			string dag_str = dag->Get_node_name();
			string operator_name;

     			if(dag_str == "if_then_else")
			{
				operator_name = "ite";
			}
			else if(dag_str == "=")
			{
	  			operator_name =  "equivalent";
			}
			else if(dag_str == "bvule")
		  	{
		    		operator_name =  "<=";
		  	}
			else if(dag_str == "bvuge")
		  	{
				operator_name =  ">=";
		  	}
			else if(dag_str == "bvult")
		  	{
		    		operator_name =  "<";
		  	}
			else if(dag_str == "bvugt")
		  	{
				operator_name =  ">";
		  	}
			else if(dag_str == "bvmul")
	      		{
				operator_name = "*";
			}
			else if(dag_str == "bvadd")
	      		{
				operator_name = "+";
			}
			else if(dag_str == "and")
	      		{
				operator_name = "and";
			}
			else if(dag_str == "or")
	      		{
				operator_name = "or";
			}
			else if(dag_str == "not")
	      		{
				operator_name = "not";
			}
			else if(dag_str == "is_not_equal")
	      		{
				operator_name = "is_not_equal";
			}
			else
	 		{
				cout << "\nUnexpected operator " << dag_str << endl;
	 			assert(false);
			}

			//cout << "\noperation: " << operator_name << endl;
		
			list<DAGNode*>::iterator iout = dag->Get_outlist_start();
    			unsigned int outlist_size = dag->Get_outlist_size();
    
    			switch(outlist_size)
      			{
      				case 1:
				{
				  	CDAGNode* child1 = convertDAGNodeToCDAGNode_recur(*iout, WidthTable, DPTable);
					assert(child1 != NULL);
					dag_changed = MANAGER.createDAG(operator_name, child1);
					assert(dag_changed != NULL);					
			  	
				  	break;
				}
				case 2:
				{
					CDAGNode* child1 = convertDAGNodeToCDAGNode_recur(*iout, WidthTable, DPTable);
					assert(child1 != NULL);
					iout++;

					CDAGNode* child2 = convertDAGNodeToCDAGNode_recur(*iout, WidthTable, DPTable);
					assert(child2 != NULL);

					dag_changed = MANAGER.createDAG(operator_name, child1, child2);
					assert(dag_changed != NULL);

				  	break;

				}
				case 3:
				{
				  	CDAGNode* child1 = convertDAGNodeToCDAGNode_recur(*iout, WidthTable, DPTable);
					assert(child1 != NULL);
					iout++;

					CDAGNode* child2 = convertDAGNodeToCDAGNode_recur(*iout, WidthTable, DPTable);
					assert(child2 != NULL);
					iout++;

					CDAGNode* child3 = convertDAGNodeToCDAGNode_recur(*iout, WidthTable, DPTable);
					assert(child3 != NULL);

					dag_changed = MANAGER.createDAG(operator_name, child1, child2, child3);
					assert(dag_changed != NULL);

				  	break;
				  	
				}
			       default: 
				{
				 	assert(false);
				}
 			  }//switch ends here
		}// operation ends here

		DPTable.insert(make_pair(dag, dag_changed));
		return dag_changed;
	}// hash-table miss ends here
}// function ends here



void CSymbSimulator::printTRDetails(CEntity &ent, CDAGNode* transition_relation)
{
	cout << "\nTransition relation details: ";

	int transition_relation_excnt = 0;
	int transition_relation_size = 0;
	transition_relation->size(transition_relation_excnt, transition_relation_size);
	cout << "size=" << transition_relation_size;

	set<string> support_transition_relation;
	transition_relation->getDAGNames(support_transition_relation);
	
	int total_width = 0;

	map<string, int> WidthTable;
	getWidthTable(transition_relation, ent.getName(), WidthTable);

  	for(set<string>::iterator it = support_transition_relation.begin(); it != support_transition_relation.end(); it++)
    	{
		string name = *it;
		map<string, int>::iterator wit = WidthTable.find(name);
		assert(wit != WidthTable.end());
		
		total_width = total_width + wit->second;
      	}

	cout << ", vars=" << support_transition_relation.size() << ", bits=" << total_width;
}