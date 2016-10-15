
/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/



#include "DAGEliminator.h"

// Internal function called by getAtomicPredicates 
// to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
void getAtomicPredicatesInternal(DAGNode* node, set<DAGNode*> &APSet, set<DAGNode*> &AlreadyEncountered)
{

  if(AlreadyEncountered.find(node)!=AlreadyEncountered.end()) // node is already encountered. return
    {
      return;
    }
              
  if(node->Get_node_name()=="and" || node->Get_node_name()=="or" || node->Get_node_name()=="if_then_else" || node->Get_node_name()=="not") // call getAtomicPredicatesInternal on each child
    {
      list<DAGNode*>::iterator iout = node->Get_outlist_start();
      unsigned int outlist_size = node->Get_outlist_size();

      for(int i=0;i<outlist_size;i++)
	{
	  getAtomicPredicatesInternal(*iout, APSet, AlreadyEncountered);
	  iout++;
	}
    }
  else
    {
      if(node->Get_node_name()=="=" || node->Get_node_name()=="is_not_equal" ||  node->Get_node_name()== "equivalent" ||  node->Get_node_name()== "not_equal_to") 
	{
	  APSet.insert(node);
	}
      else if(node->Get_outlist_size()>0)// illegal node
	{
	  cout<<"\nIllegal node "<<node->Get_node_name()<<" encountered inside function getAtomicPredicatesInternal in file DAGEliminator.h\n";
	  exit(1);
	}
	    
    }
  AlreadyEncountered.insert(node);
}



// Function to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
// Implemented with dynamic programming
void getAtomicPredicates(DAGNode* node, set<DAGNode*> &APSet)
{
  set<DAGNode*> AlreadyEncountered;
  AlreadyEncountered.clear();
  getAtomicPredicatesInternal(node, APSet, AlreadyEncountered);
}


// Print the dag rooted at "f" in SMTLIB format, call STP to (i) check if it is sat/unsat and set the 
// return value accordingly, and (ii) if sat, get the model in result_thunsat_single_formula.txt
bool ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG(DAGNode* f, map<string, int> &WidthTable)
{
  FILE *smt_file = fopen("thunsat_single_formula.smt", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.smt\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  string dag_str = printDAGInSMTLIBFormatAsDAG(f, WidthTable, smt_file);

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);

  system("stp -p --SMTLIB1 thunsat_single_formula.smt > result_thunsat_single_formula.txt"); //call to STP

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG in DAGEliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}


  ifstream *infile;
  infile = new ifstream();
  infile->open("result_thunsat_single_formula.txt");
  
  string s="unknown";

  while(!infile->eof())
    {
 	
      *infile>>s;
      if(s=="")
	break;
    }

  infile->close();

  if(s=="unsat") {
		//cout<<"\ns==unsat\n";
		return true;
		}
		
  else if(s=="sat") {
		    //cout<<"\ns==sat\n";
		    return false;
		    }
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG in DAGEliminator.cpp\n"; exit(1);
    }
}


// Given a dag rooted at "node", this function checks if dag is sat/unsat using STP
// If unsat return true. 
// If sat, get the model in the map Model by parsing the CEX returned from STP
bool getModelUsingSTP(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  Model.clear();
  if(ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG(node, WidthTable)) // node is unsat; return true
    {
      return true;
    }

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function getModelUsingSTP in DAGEliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}

  // node is sat; CEX is in result_thunsat_single_formula.txt
  getModelFromSTPCEXFile(Model);
  //showModel(Model);
	
  time(&end);
  duration = end - start;
  getModel_time = getModel_time + duration;

  return false;
}
  

// Given a dag rooted at "node", this function checks if dag is sat/unsat using STP/yices
// If unsat return true. 
// If sat, get the model in the map Model by parsing the CEX returned from STP/yices
bool getModelGeneral(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
if(internal_solver == "yices")
	{
	return getModel(node, Model, WidthTable);
	}
else if(internal_solver == "stp")
	{
	return getModelUsingSTP(node, Model, WidthTable);
	}
else	
	{
	cout<<"\nUnknown solver in getModelGeneral\n";
	exit(1);
	}
}

// Print the dag rooted at "f" in yices format, call yices to (i) check if it is sat/unsat and set the 
// return value accordingly, and (ii) if sat, get the model in result_thunsat_single_formula.txt
bool ThUnsatSingleFormulaWithCEXFile(DAGNode* f, map<string, int> &WidthTable)
{
  FILE *smt_file = fopen("thunsat_single_formula.ys", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(f, WidthTable);


  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  string command;
  command = "yices -tc -e ";

  char tout_char[100];
  sprintf(tout_char,"%d",time_out);
  string tout_str(tout_char);

  command += "-tm ";
  command += tout_str;
  command += " thunsat_single_formula.ys > result_thunsat_single_formula.txt";
  system(command.c_str());

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function ThUnsatSingleFormulaWithCEXFile in DAGEliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}



  //system("yices -tc -e thunsat_single_formula.ys > result_thunsat_single_formula.txt");
  FILE *result_smt_file = fopen("result_thunsat_single_formula.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_single_formula.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function ThUnsatSingleFormulaWithCEXFile\n"; exit(1);
    }
}



// Given a dag rooted at "node", this function checks if dag is sat/unsat using yices.
// If unsat return true. 
// If sat, get the model in the map Model by parsing the CEX returned from yices
bool getModel(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  Model.clear();
  if(ThUnsatSingleFormulaWithCEXFile(node, WidthTable)) // node is unsat; return true
    {
      return true;
    }

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function getModel in DAGEliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}

  // node is sat; CEX is in result_thunsat_single_formula.txt
  getModelFromFile(Model);
  //showModel(Model);
	
  time(&end);
  duration = end - start;
  getModel_time = getModel_time + duration;

  return false;
}
  


// Implements Generalize1 from Monniaux's LPAR 2008 paper
// Given a variable ---> value map Model, evaluates the APs in APSet.
// The APs which are evaluated to true are pushed into Constraints
// The APs which are evaluated to false are negated and the negation is pushed into Constraints
void Generalize1(map<string, string> &Model, DAGNode* F, set<DAGNode*> &APSet, map<string, int> &WidthTable, vector<DAGNode*> &Constraints)
{
time_t start;
time_t end;
time_t duration;

time(&start); 


  Constraints.clear();

  for(set<DAGNode*>::iterator ait=APSet.begin(); ait!=APSet.end(); ait++)
    {
      if(checkEntailment(*ait, WidthTable, Model)) // Model entails *ait
	{
	  Constraints.push_back(*ait);
	}
      else // Model entails negation of *ait
	{
	 Constraints.push_back(negateConstraint(*ait));
	}
     }

  time(&end);
  duration = end - start;
  Generalize1_time = Generalize1_time + duration;
}

// Implementation of Generalize2 strategy in Monniaux's LPAR 2008 work
// Removes unnecessary constraints from "Constraints" using an SMT-Solver (STP/yices)
void Generalize2_Monniaux(vector<DAGNode*> &Constraints, DAGNode* F, map<string, int> &WidthTable)
{
 
time_t start;
time_t end;
time_t duration;

time(&start); 

set<DAGNode*> AlreadyDone;
AlreadyDone.clear();


  while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<Constraints.size();i++)
	{
          //cout<<"\nTaking ap "<<getDAGStr(Constraints[i])<<endl;
          if(AlreadyDone.find(Constraints[i])!=AlreadyDone.end()) // We have already checked if /\(Constraints\Constraints[i]) => F is true
		{
		//cout<<"\nAlready Done\n";
		continue;
		}
	  if(ThUnsatMonniaux(F, Constraints, i, WidthTable)) // returns true if /\(Constraints\Constraints[i]) => F. i.e. Constraints[i] can be dropped
	    {
	      //cout<<"\nThis can be removed\n";
	      Constraints.erase(Constraints.begin()+i);
	      fixpoint_reached=false;
	      break;
	    }
	//cout<<"\nThis cannot be removed\n";
	AlreadyDone.insert(Constraints[i]);
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here

  time(&end);
  duration = end - start;
  Generalize2_time = Generalize2_time + duration;
}


// Given two sets of DAGNode*, "ReducedModelChild1" and "ReducedModelChild2", assign in 
// "ReducedModelChild" the set with minimum cardinality.
void findSmaller(set<DAGNode*> &ReducedModelChild1, set<DAGNode*> &ReducedModelChild2, set<DAGNode*> &ReducedModel)
{
  if(ReducedModelChild1.size() < ReducedModelChild2.size()) // ReducedModelChild1 is smaller
	{
	ReducedModel = ReducedModelChild1;
	}
  else
	{
	ReducedModel = ReducedModelChild2;
	}
}

// Implementation of Generalize2 strategy in our CAV 2011 work
// Removes unnecessary constraints from "Constraints" using an evaluation based strategy
void Generalize2_Strategy2(vector<DAGNode*> &Constraints, DAGNode* F)
{
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

set<DAGNode*> Model;
set<DAGNode*> ReducedModel;
for(int i=0;i<Constraints.size(); i++)
    {
      Model.insert(Constraints[i]);
    }

map<DAGNode*, pair< char, set<DAGNode*> > > DPTable;
DPTable.clear();
char ret_value = Generalize2_Strategy2_Internal(F, Model, ReducedModel, DPTable);
if(ret_value != 't')
	{
	cout<<"\nError in Generalize2_Strategy2 in DAGEliminator.cpp\n";
	exit(1);
	}
Constraints.clear();
for(set<DAGNode*>::iterator it=ReducedModel.begin();it!=ReducedModel.end(); it++)
    {
      Constraints.push_back(*it);
    }

  time(&end);
  duration = end - start;
  Generalize2_time = Generalize2_time + duration;


}

// Internal function called by function Generalize2_Strategy2
// Return value indicates the result of evaluating the dag node rooted at "F"
// using the model "Model". The reduced model, i.e. model with unnecessary constraints
// dropped is in "ReducedModel"
char Generalize2_Strategy2_Internal(DAGNode* F, set<DAGNode*> &Model, set<DAGNode*> &ReducedModel, map<DAGNode*, pair< char, set<DAGNode*> > > &DPTable)
{

  map<DAGNode*, pair< char, set<DAGNode*> > > ::iterator dpit = DPTable.find(F);
 
  if(dpit!=DPTable.end()) // F already encountered
	{
        //cout<<"\nDPTable Hit\n";
	ReducedModel = (dpit->second).second;
	return (dpit->second).first;
	}

  else { // encountered first time

  char ret_value;
 
  int OutlistSize = F->Get_outlist_size();

  string Name = F->Get_node_name();
 
  ReducedModel.clear();

  if(Name == "=" || Name == "is_not_equal") // LME/LMD
    {
      
      if( Model.find(F) !=  Model.end()) // Label as it is, is in  Model. Hence node is true
	
	{
	ReducedModel.insert(F); 
	ret_value = 't';
	}
      
      else // Label appears in the negated form
	{
	ReducedModel.insert(negateConstraint(F)); 
	ret_value = 'f'; 
	}
    }
  else if(OutlistSize != 0)// Internal node
    {
      
      if(Name == "and" && OutlistSize == 2) // binary and
	{
	  set<DAGNode*> ReducedModelChild1;
          set<DAGNode*> ReducedModelChild2;
 
	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild1, DPTable);
	  
	  iout++;

	  char child2 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild2, DPTable);

	  if(child1 == 'f' && child2 == 'f')
	    
	    { 
		findSmaller(ReducedModelChild1, ReducedModelChild2, ReducedModel); 
		ret_value = 'f';
	   }

	  else if(child1 == 'f')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 'f';
	    }
	    
	  else if(child2 == 'f')
	    
	    { 
		ReducedModel = ReducedModelChild2; 
		ret_value = 'f';
	    }

	  else if(child1 == 't' && child2 == 't')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin())); 
		ret_value = 't';
	    }
	    
	  else
	    { 
              cout<<"\nIllegal combination in and in function Generalize2_Strategy2_Internal in DAGEliminator.cpp\n";
	      exit(1);
	    }
	  
	}
      else if(Name == "or" && OutlistSize == 2) // binary or
	{
	  set<DAGNode*> ReducedModelChild1;
	  set<DAGNode*> ReducedModelChild2;

	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild1, DPTable);

	  iout++;

	  char child2 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild2, DPTable);

	  if(child1 == 't' && child2 == 't')

	    { 
		findSmaller(ReducedModelChild1, ReducedModelChild2, ReducedModel); 
		ret_value = 't';
	   }
	    
	   else if(child1 == 't')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 't';
	    }
	    
	  else if(child2 == 't')
	    
	    { 
		ReducedModel = ReducedModelChild2; 
		ret_value = 't';
	    }

	  else if(child1 == 'f' && child2 == 'f')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	    }
	    
	  else
	    { 
              cout<<"\nIllegal combination in or in function Generalize2_Strategy2_Internal in DAGEliminator.cpp\n";
	      exit(1);
	    }

	  
	}
      else if(Name == "if_then_else")
	{
	  set<DAGNode*> ReducedModelChild1;
	  set<DAGNode*> ReducedModelChild2;
	  set<DAGNode*> ReducedModelChild3; 

	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild1, DPTable);

	  iout++;

	  char child2 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild2, DPTable);

	  iout++;

	  char child3 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild3, DPTable);

	  if(child1 == 't' && child2 == 't')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 't';
	   }
	
	  else if(child1 == 'f' && child3 == 't')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild3.begin(), ReducedModelChild3.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 't';
	   }
	  
	  else if(child1 == 't' && child2 == 'f')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	   }
	
	  else if(child1 == 'f' && child3 == 'f')
	    
	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild3.begin(), ReducedModelChild3.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	   }

	  else if(child2 == 'f' && child3 == 'f')

	    { 
		set_union(ReducedModelChild2.begin(), ReducedModelChild2.end(), ReducedModelChild3.begin(), ReducedModelChild3.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	    }
	  
	  
	  else
	    { 
              cout<<"\nIllegal combination in ite in function Generalize2_Strategy2_Internal in DAGEliminator.cpp\n";
	      exit(1);
	    }
	}


     else if(Name == "not")
	{
	  set<DAGNode*> ReducedModelChild1;

	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild1, DPTable);

	  if(child1 == 't')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 'f';
	    }
	    
	  else if(child1 == 'f')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 't';
	    }

	  else
	    { 
              cout<<"\nIllegal combination in not in function Generalize2_Strategy2_Internal in DAGEliminator.cpp in node  "<<getDAGStr(F)<<"\n";
	      exit(1);
	    }

	  
	}
      else if(Name == "and" && OutlistSize > 2) // and with more than 2 children
	{
	  vector< set<DAGNode*> > ReducedModelChildren;
	  vector<char> ResultChildren;
 
	  list<DAGNode*>::iterator iout = F->Get_outlist_start();
	  for(int i=0;i<OutlistSize;i++)
	    {
	      set<DAGNode*> ReducedModelChild;

	      char child = Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild, DPTable);
	      ReducedModelChildren.push_back(ReducedModelChild);
	      ResultChildren.push_back(child);

	      iout++;
	    }

	  ret_value = Generalize2_Strategy2_Internal_For_And_With_Multiple_Children(ReducedModelChildren, ResultChildren, ReducedModel);
	}

      else if(Name == "or" && OutlistSize > 2) // or with more than 2 children
	{
	  vector< set<DAGNode*> > ReducedModelChildren;
	  vector<char> ResultChildren;
 
	  list<DAGNode*>::iterator iout = F->Get_outlist_start();
	  for(int i=0;i<OutlistSize;i++)
	    {
	      set<DAGNode*> ReducedModelChild;

	      char child = Generalize2_Strategy2_Internal(*iout,  Model, ReducedModelChild, DPTable);
	      ReducedModelChildren.push_back(ReducedModelChild);
	      ResultChildren.push_back(child);

	      iout++;
	    }

	  ret_value = Generalize2_Strategy2_Internal_For_Or_With_Multiple_Children(ReducedModelChildren, ResultChildren, ReducedModel);
	}
      else
	{
	  cout<<"\nUnknown node "<<Name<<" in function Generalize2_Strategy2_Internal in DAGEliminator.cpp\n";
	  exit(1);
	}
    }// internal node

   else // leaf
   	{
	if(Name=="true")
		{
		ret_value = 't';
		}
	else if(Name=="false")
		{
		ret_value = 'f';
		}
	else
		{
		cout<<"\nUnknown node "<<Name<<" in function Generalize2_Strategy2_Internal in DAGEliminator.cpp\n";
		exit(1);
		}
	}// leaf

   DPTable.insert(make_pair(F, make_pair(ret_value, ReducedModel)));
   return ret_value;
}// DPTable miss

}// Function ends 

	  


// Check sat/unsat of /\(Constraints\Constraints[drop_this]) /\ ~F, and
// return true/false accordingly
bool ThUnsatMonniaux(DAGNode *F, vector<DAGNode*> &Constraints, int drop_this, map<string, int> &WidthTable)
{
  DAGNode *root = NULL;

  for(int i=0;i<Constraints.size();i++)
    {
      if(i!=drop_this) // drop the constraint drop_this
	{
	  if(root==NULL) 
	    {
	      root=Constraints[i]; 
	    }
	  else 
	    {
	      root=createDAG("and", root, Constraints[i], LOGICAL_SYMBOL, 1);
	    }
	}
    }// for ends here

  if(root==NULL)
    {
      cout<<"\nroot becomes NULL in function ThUnsatMonniaux in DAGEliminator.cpp\n";
      exit(1);
    }

  // root is now /\(Constraints\Constraints[i])
  // get ~F

  DAGNode *Neg_F = createDAG("not", F, LOGICAL_SYMBOL, 1);

  // get /\(Constraints\Constraints[i]) /\ ~F
  
  root = createDAG("and", root, Neg_F, LOGICAL_SYMBOL, 1);

  // check for sat.

  bool result = ThUnsatSingleFormula(root, WidthTable);

  // deleteDAG(root); This is needed later on to delete the unneeded dag

  return result;
}
  


// This function finds \exists VarsToElim. (/\ Constraints) and returns it
// This has options to compare QE using QE_LMC with QE using BV_Solve and Omegatest
DAGNode* callAPResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  if(output_conjunctions)
	{
	//createConjunctionAndPrintInSMTLibFormat(Constraints, VarsToElim, WidthTable);
	createConjunctionsAccordingToModAndPrintInSMTLibFormat(Constraints, VarsToElim, WidthTable);
	}


  if(compare_apqelim_omegatest) // we need to compare between AP_QElim and Omegatest times, but continue with the dag computed by AP_QElim. But the comparison is done inside Eliminator.cpp. We just need to call QEliminatorForMultipleVariablesUsingLMAandILPwithScopeReductionReturningDAG  with compare_apqelim_omegatest = true
    {

      NoOfCallsToAPQElim++; // call to AP_QElim encountered


      set<DAGNode*> Constraints_Set;
      for(int i=0; i<Constraints.size(); i++)
	{
	Constraints_Set.insert(Constraints[i]);
	}

      getAP_QElimDetailsAtDAGLevel(Constraints_Set, VarsToElim, WidthTable);


      DAGNode* ret_value = quickSimplify(QEliminatorForMultipleVariablesUsingLMAandILPwithScopeReductionReturningDAG(Constraints_Set, VarsToElim, WidthTable));

      return ret_value;


    }
  else if(bitblasting_with_scopereduction && compare_apqelim_blasting) // we need to compare between AP_QElim and blasting times, but continue with the dag computed by AP_QElim. But the comparison is done inside Eliminator.cpp. We just need to call QEliminatorForMultipleVariablesUsingLMAandBitBlastingwithScopeReductionReturningDAG
    {

      NoOfCallsToAPQElim++; // call to AP_QElim encountered


      set<DAGNode*> Constraints_Set;
      for(int i=0; i<Constraints.size(); i++)
	{
	Constraints_Set.insert(Constraints[i]);
	}

      getAP_QElimDetailsAtDAGLevel(Constraints_Set, VarsToElim, WidthTable);


      DAGNode* ret_value = quickSimplify(QEliminatorForMultipleVariablesUsingLMAandBitBlastingwithScopeReductionReturningDAG(Constraints_Set, VarsToElim, WidthTable));

      return ret_value;


    }
  else
    {
      time_t start;
      time_t end;
      time_t duration;

      time(&start); 

      NoOfCallsToAPQElim++; // call to AP_QElim encountered

      // copy vector Constraints to set Constraints_Set

      set<DAGNode*> Constraints_Set;

      for(int i=0;i<Constraints.size(); i++)
	{
	  Constraints_Set.insert(Constraints[i]);
	}
  
      set<DAGNode*> Temp_Set; // only required to satisfy the fact that APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG requires a set as second argument. APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG unions first and second sets to get a set say 'S' and does qunatifier elimination on this. Hence, it is fine to keep first argument as Constraints_Set and second argument as Temp_Set (which is empty)

      getAP_QElimDetailsAtDAGLevel(Constraints_Set, VarsToElim, WidthTable);

      DAGNode* ret_value = quickSimplify(APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(Constraints_Set, Temp_Set, VarsToElim, WidthTable));

      time(&end);
      duration = end - start;
      Project_time = Project_time + duration;
  
      return ret_value;
    }
}
  
	  
// Implementation of procedure Monniaux presented in our CAV'11 work
// Given a dag rooted at "F" and a set of variables "VarSetToElim", this
// finds \exists VarsToElim. (F) and returns it
DAGNode* Monniaux_Quantifier_Eliminator(DAGNode *F, set<string> VarSetToElim, map<string, int> &WidthTable)
{

  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  vector<DAGNode*> Constraints;

  set<DAGNode*> APSet;
  getAtomicPredicates(F, APSet);

  map<string, string> Model;
  DAGNode *H = F;
  DAGNode *O = createLogicalConstant("false");
  int iteration =  1;
  
  while(!getModelGeneral(H, Model, WidthTable))
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function Monniaux_Quantifier_Eliminator in DAGEliminator.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return O;
	}

      cout<<"\nFinding the "<<iteration<<" th cube\n";
      Generalize1(Model, F, APSet, WidthTable, Constraints);
      //showAPVector("Constraints", Constraints);

      if(generalize2_strategy==2) // Our strategy
	{
      	Generalize2_Strategy2(Constraints, F);
	}
      else if(generalize2_strategy==3) // LPAR 2008 strategy
	{
      	Generalize2_Monniaux(Constraints, F, WidthTable);
	}
      else
	{
	cout<<"\nUnknown value for generalize2_strategy2 in Monniaux_Quantifier_Eliminator in DAGEliminator.cpp\n";
	exit(1);
	}

      //showAPVector("Constraints", Constraints);
      //char user_char = getchar();
      DAGNode *Pi = callAPResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(Constraints, VarSetToElim, WidthTable);
      //cout<<"\nPi = "<<getDAGStr(Pi)<<endl;
      O = createDAG("or", O, Pi, LOGICAL_SYMBOL, 1);
      //cout<<"\nO = "<<getDAGStr(O)<<endl;
      H = createDAG("and", H, createDAG("not", Pi, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);
      //cout<<"\nH = "<<getDAGStr(H)<<endl;
     iteration++;
     }

  time(&end);
  duration = end - start;
  Quant_Monn_time = Quant_Monn_time + duration;
  
  return O;

}

// Finds the variables to eliminate from the dag. 
// This is done by finding the set of variables in the dag rooted at "dag" 
// such that their delta times are 1 in "VarsToElim"
void getVarsToElimInGetTransitiveClosure(DAGNode* dag, set<string> &VarsToElim) 
{
  set<string> Names;
  getDAGNames(dag, Names); 

  // We need to quantify out all signals from Conjunct which end with _1
  for(set<string>::iterator it=Names.begin(); it!=Names.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function getVarsToElimInGetTransitiveClosure in DAGEliminator.cpp\n";
		    exit(1);
		  }
      string id = id_delta.substr(0, index);
      string delta_str = id_delta.substr(index+1);	
      if(delta_str=="1")
	{
	  VarsToElim.insert(id_delta);
	}
    }

  cout<<"\nVarsToElim\n";
  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      cout<<*it<<endl;
    }

  cout<<"\nNo: of vars to eliminate / No: of vars = "<<VarsToElim.size()<<"/"<<Names.size()<<endl;
}
      

// Get the transitive closure for the given transitive relation for the the number 
// of times in "squaring_times", and return it
DAGNode* getTransitiveClosure(DAGNode *TR, int squaring_times, map<string, int> &WidthTable, string infilename)
{

  DAGNode *PresentClosure_Dag = TR;

  checkForPropSkelton(PresentClosure_Dag);

  cout<<"\n\nDag's structure confirmed to be propositional\n\n";

  cout<<"\nNumber of nodes in the initial dag = "<<getSize(PresentClosure_Dag)<<endl;

  DAGNode* InitialClosure_Dag = PresentClosure_Dag;

  DAGNode *new_dag, *old_dag;

  for(int squaring_counter=0; squaring_counter<squaring_times; squaring_counter++)
    {

      old_dag = PresentClosure_Dag;

      cout<<"\nStarting the "<<squaring_counter+1<<" th squaring ...\n";
      cout<<"\nNumber of nodes in the PresentClosure_Dag = "<<getSize(PresentClosure_Dag)<<endl;
      //cout<<"\nPresentClosure_Dag = "<<getDAGStr(PresentClosure_Dag)<<endl;

      map<int, int> DeltaMap;

      DeltaMap.insert(make_pair(0, 1));
      DeltaMap.insert(make_pair(1, 2));
  
      DAGNode* PresentClosure_Dag_Shifted = cloneWithRenamedDeltasWithDeltaMap(PresentClosure_Dag, DeltaMap); 
      cout<<"\nNumber of nodes in the PresentClosure_Dag_Shifted = "<<getSize(PresentClosure_Dag_Shifted)<<endl;
      //cout<<"\nPresentClosure_Dag_Shifted = "<<getDAGStr(PresentClosure_Dag_Shifted)<<endl;

  

      DAGNode* Conjunct=createDAG("and", PresentClosure_Dag, PresentClosure_Dag_Shifted, LOGICAL_SYMBOL, 1);
      cout<<"\nNumber of nodes in Conjunct = "<<getSize(Conjunct)<<endl;
      //cout<<"\nConjunct = "<<getDAGStr(Conjunct)<<endl;

 
      set<string> VarSetToElim;

      getVarsToElimInGetTransitiveClosure(Conjunct, VarSetToElim);
	
      //char userchar = getchar();

      time_t var_elim_start_time, var_elim_end_time;
      time(&var_elim_start_time);

      DAGNode* Conjunct_Eliminated;

      bool input_needed_in_smtfile_format=true;
      if(input_needed_in_smtfile_format)
	{
	printInputToQEliminatorInSMTLibFormatWithCount(Conjunct, WidthTable, VarSetToElim, infilename);
	}


      Conjunct_Eliminated = Monniaux_Quantifier_Eliminator(Conjunct, VarSetToElim, WidthTable);
    
      time(&var_elim_end_time);
      cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;

      cout<<"\nNumber of nodes in Conjunct_Eliminated = "<<getSize(Conjunct_Eliminated)<<endl;
      //cout<<"\nConjunct_Eliminated = "<<getDAGStr(Conjunct_Eliminated)<<endl;

      DAGNode*  Conjunct_Eliminated_Renamed;

      DeltaMap.clear();

      DeltaMap.insert(make_pair(2, 1));

      Conjunct_Eliminated_Renamed = cloneWithRenamedDeltasWithDeltaMap(Conjunct_Eliminated, DeltaMap); 
      cout<<"\nNumber of nodes in Conjunct_Eliminated_Renamed = "<<getSize(Conjunct_Eliminated_Renamed)<<endl;
      //cout<<"\nConjunct_Eliminated_Renamed = "<<getDAGStr(Conjunct_Eliminated_Renamed)<<endl;

      PresentClosure_Dag  = createDAG("or", Conjunct_Eliminated_Renamed, InitialClosure_Dag, LOGICAL_SYMBOL, 1);

      cout<<"\nNumber of nodes in PresentClosure_Dag = "<<getSize(PresentClosure_Dag)<<endl;

      new_dag = PresentClosure_Dag;

      bool fixpoint_check=false;

      if(fixpoint_check)
	{
	  if(fixpointReached(old_dag, new_dag, WidthTable))
	    {
	      break;
	    }
	}
    }

  cout<<"\nSquarings finished\n";

  //system("top > Monn_ram.txt");

  return PresentClosure_Dag;

}

// returns true if fixpoint is reached in iterative squaring; false otherwise
// fixpoint is reached if old_dag = new_dag
bool fixpointReached(DAGNode *old_dag, DAGNode *new_dag, map<string, int> &WidthTable)
{

  DAGNode *cross_check = createDAG("is_not_equal", old_dag, new_dag, LOGICAL_SYMBOL, 1);

  if(ThUnsatSingleFormula(cross_check, WidthTable))
    {
      cout<<"\nFix point reached\n"; 
      return true;

    }
  else
    {
      cout<<"\nFix point is not reached\n";
      return false;
    }
 
}


// Calls the function Monniaux_Quantifier_Eliminator, and records the 
// time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording(DAGNode *F, set<string> VarSetToElim, map<string, int> &WidthTable)  
{
  time_t var_elim_start_time, var_elim_end_time;
  time(&var_elim_start_time);

  time_out_start = var_elim_start_time;
  
  DAGNode* result = Monniaux_Quantifier_Eliminator(F, VarSetToElim, WidthTable);

  time(&var_elim_end_time);

  if(!timed_out)
	{
  	//cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
  	//cout<<"\nSize of output dag = "<<getSize(result)<<endl;
	}
  else
	{
	//cout<<"\nQuantifier elimination timed out in "<<(var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
	}

  QElim_Time = var_elim_end_time - var_elim_start_time;

  return result;
  
}


// Function called from Generalize2_Strategy2_Internal to handle and node with
// more than two children
// returns 't' or 'f' depending on how the children gets evaluated
char Generalize2_Strategy2_Internal_For_And_With_Multiple_Children(vector< set<DAGNode*> > &ReducedModelChildren, vector<char> &ResultChildren, set<DAGNode*> &ReducedModel)
{
  // take each ResultChildren[i]. check if it is 'f'. if yes, we know that we should return 'f'. But it can happen that there is some other ResultChildren[j] which is also 'f' such that ReducedModelChildren[j].size()<ReducedModelChildren[i].size(). In this case, it is better to return 'f' with ReducedModelChildren[j] as the ReducedModel

  map<int, int> Children_Size_With_Index;

  for(int i=0;i<ResultChildren.size();i++)
    {
      if(ResultChildren[i]=='f')
	{
	  Children_Size_With_Index.insert(make_pair(ReducedModelChildren[i].size(), i));
	}
    }
  
  // Now, we have in Children_Size_With_Index the set of indices of all children with their sizes. Return the entry with the smallest size. i.e. the top-most entry

  map<int, int>::iterator it = Children_Size_With_Index.begin();

  if(it!=Children_Size_With_Index.end()) // There is at least one entry in the map
    {
      int index = it->second;

      ReducedModel = ReducedModelChildren[index];

      return 'f';
    }

  // all are 'true'. Hence take union of all sets in ReducedModelChildren and return true

  ReducedModel.clear();

  for(int i=0;i<ReducedModelChildren.size();i++)
    {
      set_union(ReducedModel.begin(), ReducedModel.end(), ReducedModelChildren[i].begin(), ReducedModelChildren[i].end(), inserter(ReducedModel, ReducedModel.begin())); 
    }

  return 't';
}

// Function called from Generalize2_Strategy2_Internal to handle or node with
// more than two children
// returns 't' or 'f' depending on how the children gets evaluated
char Generalize2_Strategy2_Internal_For_Or_With_Multiple_Children(vector< set<DAGNode*> > &ReducedModelChildren, vector<char> &ResultChildren, set<DAGNode*> &ReducedModel)
{

  // take each ResultChildren[i]. check if it is 't'. if yes, we know that we should return 't'. But it can happen that there is some other ResultChildren[j] which is also 't' such that ReducedModelChildren[j].size()<ReducedModelChildren[i].size(). In this case, it is better to return 't' with ReducedModelChildren[j] as the ReducedModel

  map<int, int> Children_Size_With_Index;

  for(int i=0;i<ResultChildren.size();i++)
    {
      if(ResultChildren[i]=='t')
	{
	  Children_Size_With_Index.insert(make_pair(ReducedModelChildren[i].size(), i));
	}
    }
  
  // Now, we have in Children_Size_With_Index the set of indices of all children with their sizes. Return the entry with the smallest size. i.e. the top-most entry

  map<int, int>::iterator it = Children_Size_With_Index.begin();

  if(it!=Children_Size_With_Index.end()) // There is at least one entry in the map
    {
      int index = it->second;

      ReducedModel = ReducedModelChildren[index];

      return 't';
    }


  // all are 'false'. Hence take union of all sets in ReducedModelChildren and return false

  ReducedModel.clear();

  for(int i=0;i<ReducedModelChildren.size();i++)
    {
      set_union(ReducedModel.begin(), ReducedModel.end(), ReducedModelChildren[i].begin(), ReducedModelChildren[i].end(), inserter(ReducedModel, ReducedModel.begin())); 
    }

  return 'f';
}



// Unrolls the relation "TR", "squaring_times" #of times, and prints the unrolled relation 
// in SMTLIB format
// Used when the command line option -unrolling is true
void getUnrolled(DAGNode *TR, int squaring_times, map<string, int> &WidthTable, string infilename)
{

  DAGNode *Conjunct = TR;

 
  for(int squaring_counter=1; squaring_counter<=squaring_times; squaring_counter++)
    {

      map<int, int> DeltaMap;

      DeltaMap.insert(make_pair(0, squaring_counter));
      DeltaMap.insert(make_pair(1, squaring_counter+1));
  
      DAGNode* TR_Shifted = cloneWithRenamedDeltasWithDeltaMap(TR, DeltaMap); 

      Conjunct=createDAG("and", Conjunct, TR_Shifted, LOGICAL_SYMBOL, 1);
    }

  cout<<"\nNumber of nodes in Conjunct = "<<getSize(Conjunct)<<endl;
  //cout<<"\nConjunct = "<<getDAGStr(Conjunct)<<endl;

 
  set<string> VarSetToElim;

  getVarsToElimInUnrolling(Conjunct, VarSetToElim, squaring_times);

  bool input_needed_in_smtfile_format=true;
  if(input_needed_in_smtfile_format)
    {
      printInputToQEliminatorInSMTLibFormatWithCount(Conjunct, WidthTable, VarSetToElim, infilename);
    }
    
}

// Select the variables to be eliminated in the -unrolling mode in "VarsToElim"
// Presently we select all the variables as variables to eliminate
// Used when the command line option -unrolling is true
void getVarsToElimInUnrolling(DAGNode* dag, set<string> &VarsToElim, int times) 
{
  set<string> Names;
  getDAGNames(dag, Names); 

  
  for(set<string>::iterator it=Names.begin(); it!=Names.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function getVarsToElimInGetTransitiveClosure in DAGEliminator.cpp\n";
		    exit(1);
		  }
      string id = id_delta.substr(0, index);
      string delta_str = id_delta.substr(index+1);
      int delta = atoi(delta_str.c_str());
      if(delta>=0) // This can be changed
	{
	  VarsToElim.insert(id_delta);
	}
    }

  cout<<"\nVarsToElim\n";
  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      cout<<*it<<endl;
    }

  cout<<"\nNo: of vars to eliminate / No: of vars = "<<VarsToElim.size()<<"/"<<Names.size()<<endl;
}
      	
// Get the details at the QE_LMC level, before QE_LMC is called
void getAP_QElimDetailsAtDAGLevel(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  //showAPSet("Constraints", Constraints);
  //showSet(VarsToElim,"VarsToElim");

  unsigned int number_of_lmes, number_of_lmds, number_of_vars, number_of_vars_to_elim, number_of_bits;
  
  set<DAGNode*> LMEs;
  set<DAGNode*> LMDs;
  
  seggregateIntoLMEsAndLMDs(Constraints, LMEs, LMDs, WidthTable);

  //showAPSet("LMEs", LMEs);
  //showAPSet("LMDs", LMDs);

  number_of_lmes = LMEs.size();
  number_of_lmds = LMDs.size();

  set<string> Support;
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;
     set<string> Support_Constraint;
     getDAGNames(Constraint, Support_Constraint);
     set<string> NewSet;
     set_union(Support_Constraint.begin(), Support_Constraint.end(), Support.begin(), Support.end(),inserter(NewSet, NewSet.begin())); 
     Support = NewSet;
   }
  number_of_vars = Support.size();

  set<string> VarsToElimInSupport;
  set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));
  number_of_vars_to_elim = VarsToElimInSupport.size();


  number_of_bits = 0;
  for(set<string>::iterator it=VarsToElimInSupport.begin(); it!=VarsToElimInSupport.end(); it++)
    {
      string var_to_elim = *it;
      map<string, int>::iterator wit = WidthTable.find(var_to_elim);
      if(wit == WidthTable.end())
	{
	  cout<<"\nVariable "<<var_to_elim<<" has no entry in WidthTable in function getAP_QElimDetailsAtDAGLevel in DAGEliminator.cpp\n";
	  exit(1);
	}
      number_of_bits += wit->second;
    }

  // number_of_lmes, number_of_lmds, number_of_vars, number_of_vars_to_elim, number_of_bits found

  //cout<<"\nnumber_of_lmes = "<<number_of_lmes<<", number_of_lmds = "<<number_of_lmds<<", number_of_vars = "<<number_of_vars<<", number_of_vars_to_elim = "<<number_of_vars_to_elim<<", number_of_bits = "<<number_of_bits<<endl;

  APQElim_Total_Eqs += number_of_lmes;
    
  if(number_of_lmes > APQElim_Max_Eqs)
	    {
	     APQElim_Max_Eqs  = number_of_lmes;
	    }

  if(number_of_lmes < APQElim_Min_Eqs)
	    {
	    APQElim_Min_Eqs = number_of_lmes;
	    }

   APQElim_Total_Diseqs += number_of_lmds;
  
  if(number_of_lmds > APQElim_Max_Diseqs)
	    {
	    APQElim_Max_Diseqs  = number_of_lmds;
	    }

  if(number_of_lmds < APQElim_Min_Diseqs)
	    {
	    APQElim_Min_Diseqs  = number_of_lmds;
	    }

   APQElim_Total_Vars  += number_of_vars;
  
  if(number_of_vars > APQElim_Max_Vars)
	    {
	     APQElim_Max_Vars = number_of_vars;
	    }

  if(number_of_vars < APQElim_Min_Vars)
	    {
	    APQElim_Min_Vars  = number_of_vars;
	    }

  APQElim_Total_VarsToElim  += number_of_vars_to_elim;
  
  if(number_of_vars_to_elim > APQElim_Max_VarsToElim)
	    {
	    APQElim_Max_VarsToElim  = number_of_vars_to_elim;
	    }

  if(number_of_vars_to_elim < APQElim_Min_VarsToElim)
	    {
	    APQElim_Min_VarsToElim  = number_of_vars_to_elim;
	    }

  APQElim_Total_Bitwidth  += number_of_bits;
  
  if(number_of_bits > APQElim_Max_Bitwidth)
	    {
	    APQElim_Max_Bitwidth  = number_of_bits;
	    }

  if(number_of_bits < APQElim_Min_Bitwidth)
	    {
	    APQElim_Min_Bitwidth  = number_of_bits;
	    }


  if(number_of_lmds > 0) // This is a call with #LMDs > 0
    {
      APQElim_Calls_GtZero_Diseqs++;
      float Eqs_Div_Diseqs = (float)number_of_lmes/(float)number_of_lmds;

      

      APQElim_Total_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs += Eqs_Div_Diseqs;
      if(Eqs_Div_Diseqs > APQElim_Max_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs)
	{
	  APQElim_Max_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs = Eqs_Div_Diseqs;
	}

      if(Eqs_Div_Diseqs < APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs)
	{
	  APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs = Eqs_Div_Diseqs;
	}

      
       APQElim_Total_Eqs_for_Calls_GtZero_Diseqs += number_of_lmes;
    
      if(number_of_lmes > APQElim_Max_Eqs_for_Calls_GtZero_Diseqs)
	{
	  APQElim_Max_Eqs_for_Calls_GtZero_Diseqs  = number_of_lmes;
	}

      if(number_of_lmes < APQElim_Min_Eqs_for_Calls_GtZero_Diseqs)
	{
	 APQElim_Min_Eqs_for_Calls_GtZero_Diseqs  = number_of_lmes;
	}

      APQElim_Total_Diseqs_for_Calls_GtZero_Diseqs += number_of_lmds;
  
      if(number_of_lmds > APQElim_Max_Diseqs_for_Calls_GtZero_Diseqs)
	{
	  APQElim_Max_Diseqs_for_Calls_GtZero_Diseqs  = number_of_lmds;
	}

      if(number_of_lmds < APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs)
	{
	  APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs  = number_of_lmds;
	}

      APQElim_Total_Bitwidth_for_Calls_GtZero_Diseqs += number_of_bits;
  
      if(number_of_bits > APQElim_Max_Bitwidth_for_Calls_GtZero_Diseqs)
	{
	 APQElim_Max_Bitwidth_for_Calls_GtZero_Diseqs  = number_of_bits;
	}

      if(number_of_bits < APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs)
	{
	  APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs  = number_of_bits;
	}



    }// if(number_of_lmds>0) ends here

}


// Functions added from 22 July 2011 onwards to implement Monniaux style QE on dags with LMCs and sels, concats

// Calls the function Monniaux_Quantifier_Eliminator_WithSupportForSelsConcats, 
// and records the time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForSelsConcats(DAGNode *F, set<string> VarSetToElim, map<string, int> &WidthTable)  
{
  time_t var_elim_start_time, var_elim_end_time;
  time(&var_elim_start_time);

  time_out_start = var_elim_start_time;
  
  DAGNode* result = Monniaux_Quantifier_Eliminator_WithSupportForSelsConcats(F, VarSetToElim, WidthTable);

  time(&var_elim_end_time);

  if(!timed_out)
	{
  	cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
  	cout<<"\nSize of output dag = "<<getSize(result)<<endl;
	}
  else
	{
	cout<<"\nQuantifier elimination timed out in "<<(var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
	}

  QElim_Time = var_elim_end_time - var_elim_start_time;

  return result;
  
}

// Version of Monniaux which handles equalities, disequalities with +, *, selections, and concatenations
// Given a dag rooted at "F" and a set of variables "VarSetToElim", this
// finds \exists VarsToElim. (F) and returns it
DAGNode* Monniaux_Quantifier_Eliminator_WithSupportForSelsConcats(DAGNode *F, set<string> VarSetToElim, map<string, int> &WidthTable)
{

  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  vector<DAGNode*> Constraints;

  set<DAGNode*> APSet;
  getAtomicPredicates(F, APSet);

  //showAPSet("APSet", APSet);

  map<string, string> Model;
  DAGNode *H = F;
  DAGNode *O = createLogicalConstant("false");
  int iteration =  1;

  
  while(!getModelGeneral_WithSupportForSelsConcats(H, Model, WidthTable))
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function Monniaux_Quantifier_Eliminator_WithSupportForSelsConcats in DAGEliminator.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return O;
	}

      //cout<<"\nSatisfiable! See result_thunsat_single_formula.txt for the model\n";

      cout<<"\nFinding the "<<iteration<<" th cube\n";

      //char c = getchar();

           
      Generalize1_WithSupportForSelsConcats(Model, F, APSet, WidthTable, Constraints);
      
      showAPVector("Constraints After Generalize1", Constraints);

      if(generalize2_strategy==2) // Our strategy
	{
      	Generalize2_Strategy2(Constraints, F);
	}
      else
	{
	cout<<"\nUnknown value for generalize2_strategy2 in Monniaux_Quantifier_Eliminator_WithSupportForSelsConcats in DAGEliminator.cpp\n";
	exit(1);
	}

      showAPVector("Constraints After Generalize2", Constraints);
            
      DAGNode *Pi = QE_Using_LMA_Bruttomesso_Blasting_ReturningDAG(Constraints, VarSetToElim, WidthTable);

      //cout<<"\nPi = "<<getDAGStr(Pi)<<endl;
      O = createDAG("or", O, Pi, LOGICAL_SYMBOL, 1);
      //cout<<"\nO = "<<getDAGStr(O)<<endl;
      H = createDAG("and", H, createDAG("not", Pi, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);
      //cout<<"\nH = "<<getDAGStr(H)<<endl;
     iteration++;
     }

  time(&end);
  duration = end - start;
  Quant_Monn_time = Quant_Monn_time + duration;
  
  return O;

}

// Version of getModelGeneral which handles equalities, disequalities with +, *, selections, and concatenations
bool getModelGeneral_WithSupportForSelsConcats(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
if(internal_solver == "yices")
	{
	return getModel_WithSupportForSelsConcats(node, Model, WidthTable);
	}
else	
	{
	cout<<"\nUnknown solver in getModelGeneral_WithSupportForSelsConcats\n";
	exit(1);
	}
}

// Version of getModel which handles equalities, disequalities with +, *, selections, and concatenations
bool getModel_WithSupportForSelsConcats(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  Model.clear();
  if(ThUnsatSingleFormulaWithCEXFile_WithSupportForSelsConcats(node, WidthTable)) // node is unsat; return true
    {
      return true;
    }

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function getModel_WithSupportForSelsConcats in DAGEliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}

  // node is sat; CEX is in result_thunsat_single_formula.txt
  getModelFromFile(Model);
  //showModel(Model);
	
  time(&end);
  duration = end - start;
  getModel_time = getModel_time + duration;

  return false;
}

// Version of ThUnsatSingleFormulaWithCEXFile which handles equalities, 
// disequalities with +, *, selections, and concatenations
bool ThUnsatSingleFormulaWithCEXFile_WithSupportForSelsConcats(DAGNode* f, map<string, int> &WidthTable)
{
  FILE *smt_file = fopen("thunsat_single_formula.ys", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(f, WidthTable);


  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  string command;
  command = "yices -tc -e ";

  char tout_char[100];
  sprintf(tout_char,"%d",time_out);
  string tout_str(tout_char);

  command += "-tm ";
  command += tout_str;
  command += " thunsat_single_formula.ys > result_thunsat_single_formula.txt";
  system(command.c_str());

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function ThUnsatSingleFormulaWithCEXFile_WithSupportForSelsConcats in DAGEliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}



  //system("yices -tc -e thunsat_single_formula.ys > result_thunsat_single_formula.txt");
  FILE *result_smt_file = fopen("result_thunsat_single_formula.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_single_formula.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function ThUnsatSingleFormulaWithCEXFile_WithSupportForSelsConcats\n"; exit(1);
    }
}

// Version of Generalize1 which handles equalities, 
// disequalities with +, *, selections, and concatenations
void Generalize1_WithSupportForSelsConcats(map<string, string> &Model, DAGNode* F, set<DAGNode*> &APSet, map<string, int> &WidthTable, vector<DAGNode*> &Constraints)
{
time_t start;
time_t end;
time_t duration;

time(&start); 


  Constraints.clear();

  for(set<DAGNode*>::iterator ait=APSet.begin(); ait!=APSet.end(); ait++)
    {
      if(checkEntailment_WithSupportForSelsConcats(*ait, WidthTable, Model)) // Model entails *ait
	{
	  //cout<<"\nConstraint "<<getDAGStr(*ait)<<" is true\n";
	  Constraints.push_back(*ait);
	}
      else // Model entails negation of *ait
	{
	  //cout<<"\nConstraint "<<getDAGStr(*ait)<<" is false\n";
	 Constraints.push_back(negateConstraint(*ait));
	}
     }

  time(&end);
  duration = end - start;
  Generalize1_time = Generalize1_time + duration;
}

// Function to return exists VarsToElim. (/\ Constraints) given (i) a set of variables VarsToElim ,and
// (ii) a set of constraints Constraints which are equalities, disequalities with +, *, selections, and concatenations.
// QE is done with Nelson-Oppen purification followed by our CAV'11 work, Sharygyna and Bruttomesso's ICCAD'08
// work, and bit-blasting + CUDD for the remaining variables
DAGNode* QE_Using_LMA_Bruttomesso_Blasting_ReturningDAG(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  bool output_conjunctions_in_smtlib_format = true;
  static int output_file_count = 0;
  output_file_count++;


  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;

  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);

  //showAPSet("Free_Constraints", Free_Constraints);
  showAPSet("Bound_Constraints", Bound_Constraints);


  if(output_conjunctions_in_smtlib_format)
    {
      createConjunctionAndPrintInSMTLibFormatInFileWithGivenPreName(Bound_Constraints, VarsToElim, WidthTable, "conjunction_", output_file_count);
    }


  cout<<"\nApplying Nelson-Oppen style purification\n";

  set<DAGNode*> Bound_Constraints_After_Purification;
  applyNOpurification(Bound_Constraints, WidthTable, Bound_Constraints_After_Purification, VarsToElim);

  showAPSet("Bound_Constraints_After_Purification", Bound_Constraints_After_Purification);
  showSet(VarsToElim, "VarsToElim");


  DAGNode* ResultOfQE;

  bool initial_implementation = false; // In this implementation, constraints are split into arithmetic-constraints and core-constraints. arithmetic-constraints are LMDEs and core constraints are constraints with sel, concat, =, is_not_equal. QE of arithmetic-constraints done with QE_LMC and QE of other constraints with blasting+CUDD

  if(initial_implementation)
    {
      // Now let us just split the Bound_Constraints_After_Purification into arithmetic-constraints and core-constraints

      set<DAGNode*> ArithmeticConstraints;
      set<DAGNode*> CoreConstraints;

      for(set<DAGNode*>::iterator it=Bound_Constraints_After_Purification.begin(); it!=Bound_Constraints_After_Purification.end(); it++)
	{
	  DAGNode *Constraint = *it;
	  if(findTypeOfConstraint(Constraint) == "arithmetic")
	    {
	      ArithmeticConstraints.insert(Constraint);
	    }
	  else if(findTypeOfConstraint(Constraint) == "core")
	    {
	      CoreConstraints.insert(Constraint);
	    }
	  else
	    {
	      cout<<"\nUnknown constraint type in function applyNOpurification in Eliminator.cpp\n";
	      exit(1);
	    }
	}

      showAPSet("ArithmeticConstraints", ArithmeticConstraints);
      showAPSet("CoreConstraints", CoreConstraints);

      // Now let us just split the VarsToElim into ArithVarsToElim (variables to elim occuring only in ArithmeticConstraints), CoreVarsToElim  (variables to elim occuring only in CoreConstraints) and CommonVarsToElim  (variables to elim occuring both in ArithmeticConstraints and CoreConstraints)

      set<string> SupportOfArithmeticConstraints;
      set<string> SupportOfCoreConstraints;

      for(set<DAGNode*>::iterator it=ArithmeticConstraints.begin(); it!=ArithmeticConstraints.end(); it++)
	{
	  getDAGNames(*it, SupportOfArithmeticConstraints);
	}

      for(set<DAGNode*>::iterator it=CoreConstraints.begin(); it!=CoreConstraints.end(); it++)
	{
	  getDAGNames(*it, SupportOfCoreConstraints);
	}

      set<string> ArithVarsToElim;
      set<string> CoreVarsToElim;
      set<string> CommonVarsToElim;

      for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
	{
	  if(SupportOfArithmeticConstraints.find(*it) != SupportOfArithmeticConstraints.end() && SupportOfCoreConstraints.find(*it) != SupportOfCoreConstraints.end())
	    {
	      CommonVarsToElim.insert(*it);
	    }
	  else if(SupportOfArithmeticConstraints.find(*it) != SupportOfArithmeticConstraints.end() && SupportOfCoreConstraints.find(*it) == SupportOfCoreConstraints.end())
	    {
	      ArithVarsToElim.insert(*it);
	    }
	  else if(SupportOfArithmeticConstraints.find(*it) == SupportOfArithmeticConstraints.end() && SupportOfCoreConstraints.find(*it) != SupportOfCoreConstraints.end())
	    {
	      CoreVarsToElim.insert(*it);
	    }
	  else
	    {
	      cout<<"\nVariable "<<*it<<" is appearing neither in arithmetic nor in core constraints. No need to eliminate it\n";
	    }
	}// for

      showSet(ArithVarsToElim, "ArithVarsToElim");
      showSet(CoreVarsToElim, "CoreVarsToElim");
      showSet(CommonVarsToElim, "CommonVarsToElim");

      // Let us call QE_LMC to perform QE from arithmetic constraints

      showAPSet("ArithmeticConstraints", ArithmeticConstraints);
      set<DAGNode*> Temp_Set; // only required to satisfy the fact that APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG requires a set as second argument. APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG unions first and second sets to get a set say 'S' and does qunatifier elimination on this. Hence, it is fine to keep first argument as ArithmeticConstraints and second argument as Temp_Set (which is empty)

      DAGNode* ArithPartAfterQE = quickSimplify(APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(ArithmeticConstraints, Temp_Set, ArithVarsToElim, WidthTable));

      cout<<endl<<"ArithPartAfterQE = "<<getDAGStr(ArithPartAfterQE)<<endl;




      DAGNode* CorePartAfterQE = quickSimplify(Trivial_Bruttomesso(CoreConstraints, CoreVarsToElim, WidthTable)); // Presently CorePartAfterQE is just the conjunction of constraints in CoreConstraints

      cout<<endl<<"CorePartAfterQE = "<<getDAGStr(CorePartAfterQE)<<endl;
	
      DAGNode* InputToBlasting = quickSimplify(createDAG("and", ArithPartAfterQE, CorePartAfterQE, LOGICAL_SYMBOL, 1));
 
      cout<<endl<<"InputToBlasting = "<<getDAGStr(InputToBlasting)<<endl;

      set<string> VarsToElimByBlasting;

      set_union(CoreVarsToElim.begin(), CoreVarsToElim.end(), CommonVarsToElim.begin(), CommonVarsToElim.end(),inserter(VarsToElimByBlasting, VarsToElimByBlasting.begin()));

      ResultOfQE = ddm.qElimUsingBlasting(InputToBlasting, VarsToElimByBlasting, WidthTable);

      cout<<endl<<"ResultOfQE = "<<getDAGStr(ResultOfQE)<<endl;
    }

  else // // In this implementation, constraints are split into arithmetic-constraints, core-constraints and hard-constraints. arithmetic-constraints are LMDEs, core constraints are constraints with sel, concat, = and  hard constraints are constraints with sel, concat, is_not_equal. QE of arithmetic-constraints done with QE_LMC, QE of core constraints with Bruttomesso approach and final QE with blasting + CUDD
    {
      // Now let us just split the Bound_Constraints_After_Purification into arithmetic-constraints, core-constraints, and hard-constraints

      set<DAGNode*> ArithmeticConstraints;
      set<DAGNode*> CoreConstraints;
      set<DAGNode*> HardConstraints;

      for(set<DAGNode*>::iterator it=Bound_Constraints_After_Purification.begin(); it!=Bound_Constraints_After_Purification.end(); it++)
	{
	  DAGNode *Constraint = *it;
	  if(findTypeOfConstraint(Constraint) == "arithmetic")
	    {
	      ArithmeticConstraints.insert(Constraint);
	    }
	  else if(findTypeOfConstraint(Constraint) == "core" && Constraint->Get_node_name() == "=" )
	    {
	      CoreConstraints.insert(Constraint);
	    }
  	  else if(findTypeOfConstraint(Constraint) == "core" && Constraint->Get_node_name() == "is_not_equal" )
	    {
	      HardConstraints.insert(Constraint);
	    }

	  else
	    {
	      cout<<"\nUnknown constraint type in function applyNOpurification in Eliminator.cpp\n";
	      exit(1);
	    }
	}

      showAPSet("ArithmeticConstraints", ArithmeticConstraints);
      showAPSet("CoreConstraints", CoreConstraints);
      showAPSet("HardConstraints", HardConstraints);
      


      // Now let us just split the VarsToElim into ArithVarsToElim (variables to elim occuring only in ArithmeticConstraints), CoreVarsToElim  (variables to elim occuring only in CoreConstraints) and CommonVarsToElim  (other variables to eliminate)

      set<string> SupportOfArithmeticConstraints;
      set<string> SupportOfCoreConstraints;
      set<string> SupportOfHardConstraints;

      for(set<DAGNode*>::iterator it=ArithmeticConstraints.begin(); it!=ArithmeticConstraints.end(); it++)
	{
	  getDAGNames(*it, SupportOfArithmeticConstraints);
	}

      for(set<DAGNode*>::iterator it=CoreConstraints.begin(); it!=CoreConstraints.end(); it++)
	{
	  getDAGNames(*it, SupportOfCoreConstraints);
	}

      for(set<DAGNode*>::iterator it=HardConstraints.begin(); it!=HardConstraints.end(); it++)
	{
	  getDAGNames(*it, SupportOfHardConstraints);
	}


      set<string> ArithVarsToElim;
      set<string> CoreVarsToElim;
      set<string> OtherVarsToElim;

      for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
	{
	  if(SupportOfArithmeticConstraints.find(*it) != SupportOfArithmeticConstraints.end() && SupportOfCoreConstraints.find(*it) == SupportOfCoreConstraints.end()  && SupportOfHardConstraints.find(*it) == SupportOfHardConstraints.end()) // *it appears only in ArithmeticConstraints
	    {
	      ArithVarsToElim.insert(*it);
	    }
	  else if(SupportOfArithmeticConstraints.find(*it) == SupportOfArithmeticConstraints.end() && SupportOfCoreConstraints.find(*it) != SupportOfCoreConstraints.end() && SupportOfHardConstraints.find(*it) == SupportOfHardConstraints.end()) // *it appears only in CoreConstraints
	    {
	      CoreVarsToElim.insert(*it);
	    }
	  else if(SupportOfArithmeticConstraints.find(*it) == SupportOfArithmeticConstraints.end() && SupportOfCoreConstraints.find(*it) == SupportOfCoreConstraints.end() && SupportOfHardConstraints.find(*it) == SupportOfHardConstraints.end()) // *it does not appear any where
	    {
	      cout<<"\nVariable "<<*it<<" is not appearing in 1) arithmetic 2) core or 3) hard constraints. No need to eliminate it\n";
	    }

	  else // All other variables need to be eliminated by blasting + CUDD
	    {
	      OtherVarsToElim.insert(*it);
	    }
	}// for

      showSet(ArithVarsToElim, "ArithVarsToElim");
      showSet(CoreVarsToElim, "CoreVarsToElim");
      showSet(OtherVarsToElim, "OtherVarsToElim");

      // Let us call QE_LMC to perform QE from arithmetic constraints

      showAPSet("ArithmeticConstraints", ArithmeticConstraints);
      set<DAGNode*> Temp_Set; // only required to satisfy the fact that APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG requires a set as second argument. APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG unions first and second sets to get a set say 'S' and does qunatifier elimination on this. Hence, it is fine to keep first argument as ArithmeticConstraints and second argument as Temp_Set (which is empty)

      DAGNode* ArithPartAfterQE = quickSimplify(APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(ArithmeticConstraints, Temp_Set, ArithVarsToElim, WidthTable));

      cout<<endl<<"ArithPartAfterQE = "<<getDAGStr(ArithPartAfterQE)<<endl;

      DAGNode* CorePartAfterQE = quickSimplify(Bruttomesso(CoreConstraints, CoreVarsToElim, WidthTable)); 

      cout<<endl<<"CorePartAfterQE = "<<getDAGStr(CorePartAfterQE)<<endl;

      DAGNode* HardPart = getConjunctionFromSet(HardConstraints); // Get the conjunction of constraints in HardConstraints

      cout<<endl<<"HardPart = "<<getDAGStr(HardPart)<<endl;
	
      DAGNode* InputToBlasting = quickSimplify(createDAG("and", createDAG("and", ArithPartAfterQE, CorePartAfterQE, LOGICAL_SYMBOL, 1), HardPart, LOGICAL_SYMBOL, 1));
 

      if(OtherVarsToElim.empty()) // No variables for QE
	{
	  ResultOfQE = InputToBlasting;
	}
      else
	{
	  cout<<endl<<"InputToBlasting = "<<getDAGStr(InputToBlasting)<<endl;

	  if(output_conjunctions_in_smtlib_format)
	    {
	      PrintInSMTLibFormatInFileWithGivenPreName(InputToBlasting, OtherVarsToElim, WidthTable, "blasting_", output_file_count);
	    }

	  ResultOfQE = ddm.qElimUsingBlasting(InputToBlasting, OtherVarsToElim, WidthTable);
	}

      cout<<endl<<"ResultOfQE = "<<getDAGStr(ResultOfQE)<<endl;

    }

  DAGNode *FreePart=NULL;
  // FreePart is the conjunction of the constraints in Free_Constraints
  for(set<DAGNode*>::iterator it=Free_Constraints.begin(); it!=Free_Constraints.end(); it++)
    {
      if(FreePart==NULL)
	{
	  FreePart=*it;
	}
      else
	{
	  FreePart = createDAG("and",FreePart, *it, LOGICAL_SYMBOL, 1);
	}
    }
	  
  if(FreePart == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      FreePart = dm.createNode("true",(void*)v);
    }

  cout<<endl<<"FreePart = "<<getDAGStr(FreePart)<<endl;

  DAGNode* FinalResult; // FinalResult = conjunction of constraints in Free_Constraints /\ ResultOfQE

  FinalResult = quickSimplify(createDAG("and",FreePart, ResultOfQE, LOGICAL_SYMBOL, 1));

  cout<<endl<<"FinalResult = "<<getDAGStr(FinalResult)<<endl;

  return FinalResult;
 
}

// Function to apply Nelson - Oppen purification to a set of constraints "Bound_Constraints".
// The result is obtained in FinalPurifiedConstraints.
void applyNOpurification(set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable, set<DAGNode*> &FinalPurifiedConstraints, set<string> &VarsToElim)
{
int FreshVars_Count=0;
map<DAGNode*, DAGNode*> InverseFreshVars_Map; //Map from dag node to dag node for fresh variable 

for(set<DAGNode*>::iterator it=Bound_Constraints.begin(); it!=Bound_Constraints.end(); it++)
	{
	DAGNode *Constraint = *it;
	set<DAGNode*> PurifiedConstraints;

	//cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;

	NOpurification(Constraint, WidthTable, PurifiedConstraints, FreshVars_Count, InverseFreshVars_Map);

	//showAPSet("PurifiedConstraints", PurifiedConstraints);

	set_union(FinalPurifiedConstraints.begin(), FinalPurifiedConstraints.end(), PurifiedConstraints.begin(), PurifiedConstraints.end(),inserter(FinalPurifiedConstraints, FinalPurifiedConstraints.begin()));
	}

//cout<<"\nWe have created "<<FreshVars_Count<<" pure variables\n. Adding them to VarsToElim\n";

for(int count_index=1; count_index <= FreshVars_Count; count_index++)
	{
	string new_name = "pure_";
	char char_count[10] = " ";
	sprintf(char_count , "%d", count_index);
	string string_count(char_count);
	new_name += string_count;
	VarsToElim.insert(new_name);
	}
}

// Function to apply Nelson - Oppen purification to a constraint "Constraint".
// The result is obtained in PurifiedConstraints
void NOpurification(DAGNode *Constraint, map<string, int> &WidthTable, set<DAGNode*> &PurifiedConstraints, int &FreshVars_Count, map<DAGNode*, DAGNode*> &InverseFreshVars_Map)
{
set<DAGNode*> ImpureNodes;
findImpureNodes(Constraint, ImpureNodes);

if(ImpureNodes.empty())//All nodes are pure, constraint is pure
	{
	  //cout<<"\nConstraint is pure\n";
	PurifiedConstraints.insert(Constraint);
	return;
	}

set<DAGNode*> NewConstraints;
DAGNode* PuredConstraint = createPuredClone(Constraint, NewConstraints, ImpureNodes, WidthTable, FreshVars_Count, InverseFreshVars_Map);
set<DAGNode*> NewConstraintsPured;

for(set<DAGNode*>::iterator it=NewConstraints.begin(); it!=NewConstraints.end(); it++)
	{
	DAGNode* MyConstraint = *it;
	set<DAGNode*> MyConstraintPured;
	NOpurification(MyConstraint, WidthTable, MyConstraintPured, FreshVars_Count, InverseFreshVars_Map);

	set_union(NewConstraintsPured.begin(), NewConstraintsPured.end(), MyConstraintPured.begin(), MyConstraintPured.end(),inserter(NewConstraintsPured, NewConstraintsPured.begin()));
	}

PurifiedConstraints.insert(PuredConstraint);
set_union(PurifiedConstraints.begin(), PurifiedConstraints.end(), NewConstraintsPured.begin(), NewConstraintsPured.end(),inserter(PurifiedConstraints, PurifiedConstraints.begin()));
}

// Return the type of constraint; "nil" | "core" | "arithmetic"
string findTypeOfConstraint(DAGNode* Constraint)
	{
	 int OutlistSize = Constraint->Get_outlist_size();
	 string Name = Constraint->Get_node_name();

	if(OutlistSize == 0)
		{
		return "nil";
		}
	if(Name == "select" || Name == "concat")
		{
		return "core";
		}
	if(Name == "bvadd" || Name == "bvmul" || Name == "bvsub")
		{
		return "arithmetic";
		}
	if(Name == "=" || Name == "is_not_equal")
		{
		list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
		DAGNode *LHS = *iout;
		iout++;
		DAGNode *RHS = *iout;	
		
		string type_constraint_left = findTypeOfConstraint(LHS);
		string type_constraint_right = findTypeOfConstraint(RHS);
	
		if(type_constraint_left != "nil")//left has some type; return it.
			{
			return type_constraint_left;
			}
		else if(type_constraint_right != "nil")//right has some type; return it.
			{
			return type_constraint_right;
			}
		else // both sides do not have type; return "arithmetic"
			{
			return "arithmetic";
			}
		}

	cout<<"\nUnsupported operator "<<Name<<" in function findTypeOfConstraint in Eliminator.cpp\n";
	exit(1);
	}

// Find the set of impure nodes under the dag rooted at "Constraint"
void findImpureNodes(DAGNode* Constraint, set<DAGNode*> &ImpureNodes)
	{
	// First find the type of Constraint
	string type_constraint = findTypeOfConstraint(Constraint);
	// Find nodes in Constraint which are not matching with type of Constraint except the root
	// Let us go to the left and right to avoid the root
	list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
	DAGNode *LHS = *iout;
	iout++;
	DAGNode *RHS = *iout;
	
	getImpureNodes(LHS, type_constraint, ImpureNodes);
	getImpureNodes(RHS, type_constraint, ImpureNodes);
	}

// Find the set of impure nodes under the dag rooted at "node"
// which is of type "type_constraint"
void getImpureNodes(DAGNode* node, string type_constraint, set<DAGNode*> &ImpureNodes)
	{
	unsigned int outlist_size = node->Get_outlist_size();
	
	// leaf nodes are always pure
	
	if(node->Get_outlist_size() != 0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = node->Get_outlist_start();
            unsigned int outlist_size = node->Get_outlist_size();

	    // check if the node's type is matching with type_constraint. If yes go down. Else add it into ImpureNodes
	
	   string type_node = findTypeOfConstraint(node); 

           if(type_node != type_constraint) // node's type is not matching with type_constraint
		{
		ImpureNodes.insert(node);
		}
	   else // go down
		{
		switch(outlist_size)
            		{
                 	case 2:{
				getImpureNodes(*iout, type_constraint, ImpureNodes);
				iout++;
				getImpureNodes(*iout, type_constraint, ImpureNodes);
				break;
				}
			case 3: {
				getImpureNodes(*iout, type_constraint, ImpureNodes);
				iout++;
				getImpureNodes(*iout, type_constraint, ImpureNodes);
				iout++;
				getImpureNodes(*iout, type_constraint, ImpureNodes);
				break;
				}
			default:
				{
				cout<<"\nUnexpected operator in function getImpureNodes in Eliminator.cpp\n";
				exit(1);
		      		}
                	}//switch
		}//else
	}//Impure node
}//function ends here

// Given a dag rooted at "parent", return a clone which is purified.
// The new equalities which are created during the purification are
// added into the set "NewConstraints"
DAGNode* createPuredClone(DAGNode* parent, set<DAGNode*> &NewConstraints, set<DAGNode*> &ImpureNodes, map<string, int> &WidthTable, int &FreshVars_Count, map<DAGNode*, DAGNode*> &InverseFreshVars_Map)
    {
        DAGNode* clone;

	if(ImpureNodes.find(parent) != ImpureNodes.end())// parent is impure
		{
		// check if we have created a fresh variable for this node
		map<DAGNode*, DAGNode*>::iterator iit = InverseFreshVars_Map.find(parent);
		if(iit != InverseFreshVars_Map.end()) // yes, we have
			{
			clone = iit->second;
			}
		else // no, we need to create
			{
			// what is the name of the node? size etc.?
			string new_name = "pure_";
			FreshVars_Count++;
			char char_count[10] = " ";
			sprintf(char_count , "%d", FreshVars_Count);
			string string_count(char_count);
			new_name += string_count;

			int width = getWidthOfNode(parent, WidthTable);

			Value *v = new Value();
			Type *t = new Type();
	 		t->data_type = BITVECTOR_SYMBOL;
			t->width = width;
			v->set_node_type(*t);
			clone = dm.createNode (new_name,(void*)v);

			InverseFreshVars_Map.insert(make_pair(parent, clone)); // inserted into InverseFreshVars_Map 
			WidthTable.insert(make_pair(new_name, width));

			// create new constraint clone = parent
			DAGNode* NewEquation = createDAG("=", clone, parent, LOGICAL_SYMBOL, 1);
			NewConstraints.insert(NewEquation);
			}// no, we need to create
		}// parent is impure
	else // parent is pure. go down
	{
	// Get the size of outlist of parent
	unsigned int outlist_size = parent->Get_outlist_size();
	
                   
        if(outlist_size>0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = createPuredClone(*iout, NewConstraints, ImpureNodes, WidthTable, FreshVars_Count, InverseFreshVars_Map);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = createPuredClone(*iout, NewConstraints, ImpureNodes, WidthTable, FreshVars_Count, InverseFreshVars_Map);
		    iout++;
                    child_2 = createPuredClone(*iout, NewConstraints, ImpureNodes, WidthTable, FreshVars_Count, InverseFreshVars_Map);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

                    child_1 = createPuredClone(*iout, NewConstraints, ImpureNodes, WidthTable, FreshVars_Count, InverseFreshVars_Map);
		    iout++;
                    child_2 = createPuredClone(*iout, NewConstraints, ImpureNodes, WidthTable, FreshVars_Count, InverseFreshVars_Map);
		    iout++;
                    child_3 = createPuredClone(*iout, NewConstraints, ImpureNodes, WidthTable, FreshVars_Count, InverseFreshVars_Map);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    Children.push_back(child_3);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function createPuredClone in Eliminator.cpp\n";
		 exit(1);
		}

	    }//switch
	}//if an operator node
	else // leaf node
	  {
	   clone = dm.createNode(parent->Get_node_name(), parent->getValue());	             
	  }// leaf node
	}//parent is pure. go down
        return clone;
    }// function 


// Returns the conjunction of the constraints in "CoreConstraints"
// In that sense, it is same as DAGEliminator::getConjunctionFromSet, but
// this function is kept as it has a different set of parameters compared
// to the function DAGEliminator::getConjunctionFromSet.
DAGNode* Trivial_Bruttomesso(set<DAGNode*> &CoreConstraints, set<string> &CoreVarsToElim, map<string, int> &WidthTable)
{
  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in CoreConstraints
  for(set<DAGNode*>::iterator it=CoreConstraints.begin(); it!=CoreConstraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
	  
  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }
  return resultDAG;
}

// Returns exists CoreVarsToElim. (/\ CoreConstraints) using 
// Bruttomesso and Sharygyna's ICCAD 2008 work
DAGNode* Bruttomesso(set<DAGNode*> &CoreConstraints, set<string> &CoreVarsToElim, map<string, int> &WidthTable)
{

  if(CoreVarsToElim.empty())// Just return conjunction of constraints in the set CoreConstraints
    {
      return Trivial_Bruttomesso(CoreConstraints, CoreVarsToElim, WidthTable);
    }

  // Convert each constraint to concatenation normal form
  // Do this...


  //cout<<"\n\nChecking if the core constraints are in concatenation normal form.....\n\n";

  // Ensure that each constraint is in concatenation normal form
  for(set<DAGNode*>::iterator it=CoreConstraints.begin(); it!=CoreConstraints.end(); it++)
    {
      DAGNode* Constraint = *it;

      //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;

      if(!constraintInConcatenationNormalForm(Constraint))
	{
	  cout<<"\nConstraint "<<getDAGStr(Constraint)<<" is not in concatenation normal form in function Bruttomesso in Eliminator.cpp\n";
	  exit(1);
	}
      else
	{
	  //cout<<"\nConstraint "<<getDAGStr(Constraint)<<" is in concatenation normal form\n";
	}
    }//for
  
 // We are here because, we know that each core constraint is in concatenation normal form
 // Let us convert each core constraint in concatenation normal form into conjunction of equalities over simple terms

  set<DAGNode*> EqualitiesOverSimpleTerms;

  for(set<DAGNode*>::iterator it=CoreConstraints.begin(); it!=CoreConstraints.end(); it++)
    {
      DAGNode* Constraint = *it;

      //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;

      if(bothSidesAreSimpleTerms(Constraint)) // both sides are simple terms => Constraint is already an equality over simple terms
	{
	  EqualitiesOverSimpleTerms.insert(Constraint);
	}
      else
	{
	  set<DAGNode*> NewEqualities;
	  getNewEqualitiesFromConstraint(Constraint, WidthTable, NewEqualities);
	  set_union(EqualitiesOverSimpleTerms.begin(), EqualitiesOverSimpleTerms.end(), NewEqualities.begin(), NewEqualities.end(), inserter(EqualitiesOverSimpleTerms, EqualitiesOverSimpleTerms.begin())); 
	}
    }//for

  showAPSet("EqualitiesOverSimpleTerms", EqualitiesOverSimpleTerms);
  
  // Let us convert the equalities over simple terms to equalities over terms based on the coarsest base

  set<DAGNode*> EqualitiesOverSimpleTermsOnCoarsetBase;
  getEqualitiesOverSimpleTermsOnCoarsetBase(EqualitiesOverSimpleTerms, WidthTable, EqualitiesOverSimpleTermsOnCoarsetBase);
  
  showAPSet("EqualitiesOverSimpleTermsOnCoarsetBase", EqualitiesOverSimpleTermsOnCoarsetBase);

  // Now we have converted core-constraints into equality constraints. Let us use Layer 1 to eliminate the variables.
  // Linearize each constraint

  set<DAGNode*> EqualitiesOverSimpleTermsOnCoarsetBaseLinearized;
  map<DAGNode*, DAGNode*> LinearizationTable;
  int auto_count = 0;
  set<string> AutoVarsToElim;
  
  for(set<DAGNode*>::iterator it=EqualitiesOverSimpleTermsOnCoarsetBase.begin(); it!=EqualitiesOverSimpleTermsOnCoarsetBase.end(); it++)
    {
      DAGNode* Constraint = *it;
      DAGNode* LinearizedConstraint = getSimpliedLinearizedCloneWithChangeInVarsToElim(Constraint, LinearizationTable, auto_count, WidthTable, CoreVarsToElim, AutoVarsToElim);
      cout<<"\nConstraint = "<<getDAGStr(Constraint)<<"\t LinearizedConstraint = "<<getDAGStr(LinearizedConstraint)<<endl;
      EqualitiesOverSimpleTermsOnCoarsetBaseLinearized.insert(LinearizedConstraint);
    }

  showSet(AutoVarsToElim, "AutoVarsToElim");
  // Let us call Layer1 for QE

  set<string> VarSetToElimByBruttomesso;
  set_union(CoreVarsToElim.begin(),CoreVarsToElim.end(), AutoVarsToElim.begin(), AutoVarsToElim.end(), inserter(VarSetToElimByBruttomesso, VarSetToElimByBruttomesso.begin()));

  DAGNode* LinearizedDAGAfterQE = quickSimplify(APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(EqualitiesOverSimpleTermsOnCoarsetBaseLinearized, VarSetToElimByBruttomesso, WidthTable));

  //cout<<"\nLinearizedDAGAfterQE = "<<getDAGStr(LinearizedDAGAfterQE)<<endl;

  DAGNode* DAGAfterQE = getOriginalClone(LinearizedDAGAfterQE, LinearizationTable);

  //cout<<"\nDAGAfterQE = "<<getDAGStr(DAGAfterQE)<<endl;

  return DAGAfterQE;
}


// Returns true if "Constraint" is in concatenation normal form; returns false otherwise 
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of concatenation normal form
bool constraintInConcatenationNormalForm(DAGNode *Constraint)
{
  // A constraint is in concatenation normal form if both the sides of it are concatenation of simple terms

  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  DAGNode *LHS = *iout;
  iout++;
  DAGNode *RHS = *iout;

  if(!constraintIsConcatenationOfSimpleTerms(LHS))
    {
      //cout<<"\n"<<getDAGStr(LHS)<<" is not concatenation of simple terms\n";
      return false;
    }
  else if(!constraintIsConcatenationOfSimpleTerms(RHS))
    {
      //cout<<"\n"<<getDAGStr(RHS)<<" is not concatenation of simple terms\n";
      return false;
    }
	
  return true;
}

// Returns true if "Constraint" is a concatenation of simple terms; returns false otherwise 
// simple term ::= variable | constant | select(variable, constant, constant)
bool constraintIsConcatenationOfSimpleTerms(DAGNode *Constraint)
{
  unsigned int outlist_size = Constraint->Get_outlist_size();
  if(outlist_size == 0)
    {
      return true;
    }//constants, variables

  else if(Constraint->Get_node_name() == "select")
    {
      if(simpleSelect(Constraint))
	{
	  return true;
	}
      else
	{
	  return false;
	}
    }//select
  else if(Constraint->Get_node_name() == "concat")
    {
      // check if it is a concatenation of simple terms

      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
      DAGNode *Child1 = *iout;
      iout++;
      DAGNode *Child2 = *iout;

      if( constraintIsConcatenationOfSimpleTerms(Child1) && constraintIsConcatenationOfSimpleTerms(Child2) )
	{
	  return true;
	}
      else
	{
	  return false;
	}
      
    }//concat
  else
    {
      cout<<"\nUnsupported operator "<<Constraint->Get_node_name()<<" in function constraintIsConcatenationOfSimpleTerms in Eliminator.cpp\n";
      exit(1);
    }
}

// Returns true if "Constraint" is a simple select; returns false otherwise 
// simple select ::= select(variable, constant, constant)
bool simpleSelect(DAGNode *Constraint)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  DAGNode *Child1 = *iout;

  // if Child1 is variable or constant, select is simple

  unsigned int child1_outlist_size = Child1->Get_outlist_size();
  if(child1_outlist_size == 0)
    {
      return true;
    }//constants, variables

  return false;
  
}

// Returns true if both LHS and RHS of constraint rooted at "Constraint" are simple terms; returns false otherwise 
// simple term ::= variable | constant | select(variable, constant, constant)
bool bothSidesAreSimpleTerms(DAGNode *Constraint)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  DAGNode *LHS = *iout;
  iout++;
  DAGNode *RHS = *iout;

  if(!SimpleTerm(LHS))
    {
      //cout<<"\n"<<getDAGStr(LHS)<<" is not a simple term\n";
      return false;
    }
  else if(!SimpleTerm(RHS))
    {
      //cout<<"\n"<<getDAGStr(RHS)<<" is not a simple term\n";
      return false;
    }
	
  return true;
}

// Returns true if "Constraint" is a simple term; returns false otherwise 
// simple term ::= variable | constant | select(variable, constant, constant)
bool SimpleTerm(DAGNode *Constraint)
{
  unsigned int outlist_size = Constraint->Get_outlist_size();
  if(outlist_size == 0)
    {
      return true;
    }//constants, variables

  else if(Constraint->Get_node_name() == "select")
    {
      if(simpleSelect(Constraint))
	{
	  return true;
	}
      else
	{
	  return false;
	}
    }//select

  return false;
}


// Given a a constraint "Constraint" which is LHS = RHS where both LHS and RHS are
// concatenations of simple terms, generate new equalities from it
// Let LHS = RHS is s1@...@sn = t1@...@tn. New equalities are {s1=t1,...,sn=tn}
void getNewEqualitiesFromConstraint(DAGNode* Constraint, map<string, int> &WidthTable, set<DAGNode*> &NewEqualities)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  DAGNode *LHS = *iout;
  iout++;
  DAGNode *RHS = *iout;

  if(isVariable(LHS))
    {
      string variable_name = LHS->Get_node_name();
      int variable_width = getWidthOfNode(LHS, WidthTable);

      list<DAGNode*> ChildrenOfConcat;
      list<int> SizesOfChildrenOfConcat;

      getChildrenOfConcatAndGetSizes(RHS, ChildrenOfConcat, SizesOfChildrenOfConcat, WidthTable);

      list<int>::iterator it;
      
      //cout<<endl<<"ChildrenOfConcat"<<endl;
      //showAPList(ChildrenOfConcat);

      //cout<<endl<<"SizesOfChildrenOfConcat"<<endl;
      //it=SizesOfChildrenOfConcat.begin();
      //for(;it!=SizesOfChildrenOfConcat.end(); it++)
      //{
      //	  cout<<endl<<*it<<endl;
      //	}


      if(mismatchInSizes(SizesOfChildrenOfConcat, variable_width))
	{
	  cout<<"\nMismatch in sizes in function getNewEqualitiesFromConstraint in Eliminator.cpp\n";
	  exit(1);
	}


      SizesOfChildrenOfConcat.reverse();
      ChildrenOfConcat.reverse();

      it=SizesOfChildrenOfConcat.begin();
      list<DAGNode*>::iterator cit=ChildrenOfConcat.begin();
      int my_position =  0;

      for(;it!=SizesOfChildrenOfConcat.end(); it++)
	{
	  int child_size = *it;
	  
	  DAGNode* Child1 = createBVVariable(variable_name, child_size);

	  int low_position = my_position;
	  int high_position = low_position + child_size - 1;
	  my_position = high_position + 1;

	  char low_position_char[100], high_position_char[100];
	  sprintf(low_position_char, "%d", low_position);
	  sprintf(high_position_char, "%d", high_position);
	  
	  string low_position_str(low_position_char);
	  string high_position_str(high_position_char);

	  Value *v1 = new Value();
	  Type t1;
	  t1.data_type = INTEGER_CONSTANT;
	  t1.width = IMMATERIAL_WIDTH;
	  v1->set_node_type(t1);
	  DAGNode* Child2 = dm.createNode(low_position_str, (void*)v1);
	  
	  Value *v2 = new Value();
	  Type t2;
	  t2.data_type = INTEGER_CONSTANT;
	  t2.width = IMMATERIAL_WIDTH;
	  v2->set_node_type(t2);
	  DAGNode* Child3 = dm.createNode(high_position_str, (void*)v2);


	  DAGNode *lhs = createDAG("select", Child1, Child2, Child3, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	  DAGNode *rhs = *cit;

	  DAGNode *eqn = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);

	  NewEqualities.insert(eqn);
	  
	  cit++;
	}//for
    }// isVariable(LHS)
  
  // Write similar code for isVariable(RHS)
  else if(isVariable(RHS))
    {
      string variable_name = RHS->Get_node_name();
      int variable_width = getWidthOfNode(RHS, WidthTable);

      list<DAGNode*> ChildrenOfConcat;
      list<int> SizesOfChildrenOfConcat;

      getChildrenOfConcatAndGetSizes(LHS, ChildrenOfConcat, SizesOfChildrenOfConcat, WidthTable);

      list<int>::iterator it;

      //cout<<endl<<"ChildrenOfConcat"<<endl;
      //showAPList(ChildrenOfConcat);

      //cout<<endl<<"SizesOfChildrenOfConcat"<<endl;
      //it=SizesOfChildrenOfConcat.begin();
      //for(;it!=SizesOfChildrenOfConcat.end(); it++)
      //{
      //	  cout<<endl<<*it<<endl;
      //	}

      if(mismatchInSizes(SizesOfChildrenOfConcat, variable_width))
	{
	  cout<<"\nMismatch in sizes in function getNewEqualitiesFromConstraint in Eliminator.cpp\n";
	  exit(1);
	}


      SizesOfChildrenOfConcat.reverse();
      ChildrenOfConcat.reverse();

      it=SizesOfChildrenOfConcat.begin();
      list<DAGNode*>::iterator cit=ChildrenOfConcat.begin();
      int my_position =  0;

      for(;it!=SizesOfChildrenOfConcat.end(); it++)
	{
	  int child_size = *it;
	  
	  DAGNode* Child1 = createBVVariable(variable_name, child_size);

	  int low_position = my_position;
	  int high_position = low_position + child_size - 1;
	  my_position = high_position + 1;


	  char low_position_char[100], high_position_char[100];
	  sprintf(low_position_char, "%d", low_position);
	  sprintf(high_position_char, "%d", high_position);
	  
	  string low_position_str(low_position_char);
	  string high_position_str(high_position_char);

	  Value *v1 = new Value();
	  Type t1;
	  t1.data_type = INTEGER_CONSTANT;
	  t1.width = IMMATERIAL_WIDTH;
	  v1->set_node_type(t1);
	  DAGNode* Child2 = dm.createNode(low_position_str, (void*)v1);
	  
	  Value *v2 = new Value();
	  Type t2;
	  t2.data_type = INTEGER_CONSTANT;
	  t2.width = IMMATERIAL_WIDTH;
	  v2->set_node_type(t2);
	  DAGNode* Child3 = dm.createNode(high_position_str, (void*)v2);


	  DAGNode *lhs = createDAG("select", Child1, Child2, Child3, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	  DAGNode *rhs = *cit;

	  DAGNode *eqn = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);

	  NewEqualities.insert(eqn);
	  
	  cit++;
	}//for
    }// isVariable(RHS)
  
  else
    {
      cout<<"\nFunction getNewEqualitiesFromConstraint in Eliminator.cpp cannot process "<<getDAGStr(Constraint)<<endl;
      exit(1);
    }
}

// Given a dag node rooted at "Constraint", get the children of "Constraint" in 
// "ChildrenOfConcat", and the sizes of children of "Constraint" in "SizesOfChildrenOfConcat" 
void getChildrenOfConcatAndGetSizes(DAGNode* Constraint, list<DAGNode*> &ChildrenOfConcat, list<int> &SizesOfChildrenOfConcat, map<string, int> &WidthTable)
{
  if(Constraint->Get_node_name() != "concat")
    {
      ChildrenOfConcat.push_back(Constraint);
      SizesOfChildrenOfConcat.push_back(getWidthOfNode(Constraint, WidthTable));
    }//!concat
  else //concat
    {
      // go to left and right

      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
      DAGNode *Child1 = *iout;
      iout++;
      DAGNode *Child2 = *iout;

      getChildrenOfConcatAndGetSizes(Child1, ChildrenOfConcat, SizesOfChildrenOfConcat, WidthTable);
      getChildrenOfConcatAndGetSizes(Child2, ChildrenOfConcat, SizesOfChildrenOfConcat, WidthTable);
      
    }//concat
}


// return true if the sum of elements in "SizesOfChildrenOfConcat" is "variable_width";
// return false otherwise 
bool mismatchInSizes(list<int> &SizesOfChildrenOfConcat, int variable_width)
{
  int sum =  0;
  list<int>::iterator it=SizesOfChildrenOfConcat.begin();
  for(;it!=SizesOfChildrenOfConcat.end(); it++)
	{
	  sum = sum + *it;
	}

  if(sum != variable_width)
    {
      return true;
    }
  return false;
}


// Given a set of equalities over simple terms, "EqualitiesOverSimpleTerms",
// generate a set of equalities over simple terms on coarsest base in "EqualitiesOverSimpleTermsOnCoarsetBase"
void getEqualitiesOverSimpleTermsOnCoarsetBase(set<DAGNode*> &EqualitiesOverSimpleTerms, map<string, int> &WidthTable,  set<DAGNode*> &EqualitiesOverSimpleTermsOnCoarsetBase)
{	
// Finding the coarset base
// get the initial base

map<string, set<int> > Base;
for(set<DAGNode*>::iterator it=EqualitiesOverSimpleTerms.begin(); it!=EqualitiesOverSimpleTerms.end(); it++)
    {
	DAGNode* Equality = *it;
	getExtractionsAndUpdateBase(Equality, WidthTable, Base);
     }

//cout<<"\nInitial Base\n";
//showBase(Base);

// Computing the coarset base
// Take each equality, check if Base(left side variable) = Base(right side variable). If yes, do nothing. If no, change
// base using this equality and the equalities prior to it

// It is easy to have the equalities in a vector to do this

 vector<DAGNode*> EqualitiesOverSimpleTermsVector;

 for(set<DAGNode*>::iterator it=EqualitiesOverSimpleTerms.begin(); it!=EqualitiesOverSimpleTerms.end(); it++)
    {
      EqualitiesOverSimpleTermsVector.push_back(*it);
    }

 bool initial_base_changed = false;

 for(int i=0; i<EqualitiesOverSimpleTermsVector.size(); i++)
   {
     // take each constraint 
     DAGNode* Equality = EqualitiesOverSimpleTermsVector[i];
     //cout<<"\nEquality = "<<getDAGStr(Equality)<<endl;
     if(!equalityRespectsBase(Equality, Base, WidthTable))
       {
	 cout<<"\nEquality "<<getDAGStr(Equality)<<" does not respect the base in function getEqualitiesOverSimpleTermsOnCoarsetBase in Eliminator.cpp\n";
	 initial_base_changed = true;
	 exit(1);
       }
     //cout<<"\nEquality respects the base\n";
	 
   }// for ends here
 // We have the coarset base here
 // If coarset base = initial base, no need to recompute the constraints
 // Else recompute the constraints

 if(initial_base_changed) // Initial base is changed
   {
     cout<<"\nInitial base is changed in function getEqualitiesOverSimpleTermsOnCoarsetBase in Eliminator.cpp\n";
     exit(1);
   }
 else // No change in initial base
   {
     bool old_implementation = false;

     if(old_implementation)// If coarset base = initial base, the constraints are not recomputed. But this is wrong. Consider x[1:5]=y[1:5]/\ x[1:1]=a /\ x[2:2]=b /\  y[1:1]=c /\ y[2:2]=d. coarset base = initial base here is x-->{1,2,3,5},y-->{1,2,3,5}. x[1:5]=y[1:5] to be reconstructed as x[1:1]=y[1:1] /\ x[2:2]=y[2:2]/\ x[3:5]=y[3:5]
       {     
	 for(set<DAGNode*>::iterator it=EqualitiesOverSimpleTerms.begin(); it!=EqualitiesOverSimpleTerms.end(); it++)
	   {
	     EqualitiesOverSimpleTermsOnCoarsetBase.insert(*it);
	   }
       }
     else
       {
	 for(set<DAGNode*>::iterator it=EqualitiesOverSimpleTerms.begin(); it!=EqualitiesOverSimpleTerms.end(); it++)
	   {
	     if(needToBeReconstructed(*it, WidthTable, Base))
	       {
		 cout<<"\nEquality "<<getDAGStr(*it)<<" is to be reconstructed\n";
		 set<DAGNode*> ReconstructedEqualities;
		 ReconstructEqualities(*it, WidthTable, Base, ReconstructedEqualities);
		 set_union(EqualitiesOverSimpleTermsOnCoarsetBase.begin(), EqualitiesOverSimpleTermsOnCoarsetBase.end(), ReconstructedEqualities.begin(), ReconstructedEqualities.end(), inserter(EqualitiesOverSimpleTermsOnCoarsetBase, EqualitiesOverSimpleTermsOnCoarsetBase.begin())); 
		 //exit(1);
	       }
	     else
	       {
		 EqualitiesOverSimpleTermsOnCoarsetBase.insert(*it);
	       }
	   }// for
	 
       }// else of if(old_implementation)
   }//else of if(initial_base_changed) 
 

}

// Given a constraint rooted at "Constraint", update the "Base" using the constraint.
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of "Base".
void getExtractionsAndUpdateBase(DAGNode* Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base)
{
if(Constraint->Get_node_name() == "=" || Constraint->Get_node_name() == "is_not_equal")
	{
	list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
	getExtractionsAndUpdateBase(*iout, WidthTable, Base);
	iout++;
	getExtractionsAndUpdateBase(*iout, WidthTable, Base);
	}
else if(Constraint->Get_node_name() == "select")
	{
	list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

	string variable = (*iout)->Get_node_name();

	iout++;
	string low_str = (*iout)->Get_node_name();
	int low = atoi(low_str.c_str());
	int low_slice = low;

	iout++;
	string high_str = (*iout)->Get_node_name();
	int high = atoi(high_str.c_str());
	int high_slice = high + 1;

	int width;
	map<string, int>::iterator wit = WidthTable.find(variable);

	if(wit == WidthTable.end())
		{
		cout<<"\nNo entry for variable "<<variable<<" in width table in function getExtractionsAndUpdateBase in Eliminator.cpp\n";
		exit(1);
		}
	width = wit->second;

	if(high_slice < width && low_slice > 0) // update Base
		{
		map<string, set<int> >::iterator bit = Base.find(variable);
		if(bit == Base.end()) // no entry for variable in Base
			{
			set<int> Slices;
			Slices.insert(high_slice);
			Slices.insert(low_slice);
			Base.insert(make_pair(variable, Slices));
			}
		else // entry exists for variable
			{
			(bit->second).insert(high_slice);
			(bit->second).insert(low_slice);
			}

		}// if(high_slice < width && low_slice > 0) 
	else if(high_slice < width)
		{
		map<string, set<int> >::iterator bit = Base.find(variable);
		if(bit == Base.end()) // no entry for variable in Base
			{
			set<int> Slices;
			Slices.insert(high_slice);
			Base.insert(make_pair(variable, Slices));
			}
		else // entry exists for variable
			{
			(bit->second).insert(high_slice);
			}

		}// if(high_slice < width) 
	else if(low_slice > 0)
		{
		map<string, set<int> >::iterator bit = Base.find(variable);
		if(bit == Base.end()) // no entry for variable in Base
			{
			set<int> Slices;
			Slices.insert(low_slice);
			Base.insert(make_pair(variable, Slices));
			}
		else // entry exists for variable
			{
			(bit->second).insert(low_slice);
			}

		}// if(low_slice > 0)
		
	}// else if(Constraint->Get_node_name() == "select")
}// function ends here

// returns true if the given equality rooted at "Constraint" respects (does not contradict) 
// the existing "Base"; returns false otherwise.
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of "Base".
bool equalityRespectsBase(DAGNode* Constraint, map<string, set<int> > &Base, map<string, int> &WidthTable)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  DAGNode *LHS = *iout;
  iout++;
  DAGNode *RHS = *iout;

  string left_variable = getVariableFromDAG(LHS);
  string right_variable = getVariableFromDAG(RHS);

  if(left_variable == "" || right_variable == "") // left/right is a constant. no problem
    {
      return true;
    }
  else
    {
      map<string, set<int> >::iterator left_entry = Base.find(left_variable);
      map<string, set<int> >::iterator right_entry = Base.find(right_variable);
      
      set<int> Left_Slices;
      set<int> Right_Slices;

      if(left_entry != Base.end()) 
	{
	  // Let the left part is like x(2:5) where x has width 8 and slicing is {2,4,6}. But 
         // the part of slice of interest here is only {4}
	  Left_Slices = left_entry->second;
	  if(LHS->Get_node_name() == "select") 
		{
		list<DAGNode*>::iterator liout = LHS->Get_outlist_start();
		liout++;
	        string low_str = (*liout)->Get_node_name();
		int low = atoi(low_str.c_str());

		liout++;
		string high_str = (*liout)->Get_node_name();
		int high = atoi(high_str.c_str());
		

		trimSlices(Left_Slices, high, low);
		}

	}


      if(right_entry != Base.end()) 
	{ // Let the right part is like x(2:5) where x has width 8 and slicing is {2,4,6}. But 
         // the part of slice of interest here is only {4} 
	  Right_Slices = right_entry->second;
	  if(RHS->Get_node_name() == "select") 
		{
		list<DAGNode*>::iterator riout = RHS->Get_outlist_start();
		riout++;
		string low_str = (*riout)->Get_node_name();
		int low = atoi(low_str.c_str());

		riout++;
		string high_str = (*riout)->Get_node_name();
		int high = atoi(high_str.c_str());
		

		trimSlices(Right_Slices, high, low);
		}

	}

//       cout<<"\nLeft_Slices\n";
//       for(set<int>::iterator it = Left_Slices.begin(); it != Left_Slices.end(); it++)
// 		{
// 		cout<<*it<<"\t";	
// 		}

//       cout<<"\nRight_Slices\n";
//       for(set<int>::iterator it = Right_Slices.begin(); it != Right_Slices.end(); it++)
// 		{
// 		cout<<*it<<"\t";	
// 		}

      if(Left_Slices == Right_Slices)// This is a stricter condition. It can be like x[3:5]=y[0:2] also. Let Slices(x[3:5]) = {4} and Slices(x[0:2]) = {1} Slices are still different. But x[3:5]=y[0:2] respects the Base.
	{
	  return true;
	}
      else
	{
	  return false;
	}

    }// else
}

// show the existing "Base"
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of "Base".
void showBase(map<string, set<int> > &Base)
{
for(map<string, set<int> >::iterator bit = Base.begin(); bit != Base.end(); bit++)
	{
	cout<<"\n"<<bit->first<<endl;
	for(set<int>::iterator it = (bit->second).begin(); it != (bit->second).end(); it++)
		{
		cout<<*it<<"\t";	
		}

	}
}

// Given a simple term rooted at "Constraint", return
// the variable name. Return "" if it is a constant.
string getVariableFromDAG(DAGNode *Constraint)
{
  if(isConstant(Constraint))
    {
      return "";
    }

  else if(isVariable(Constraint))
    {
      return Constraint->Get_node_name();
    }

  else if(Constraint->Get_node_name() == "select")
    {
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
      DAGNode *Child1 = *iout;
      return Child1->Get_node_name();
    }
  else
    {
      cout<<"\nUnsupported operator "<<Constraint->Get_node_name()<<" in function getVariableFromDAG  in Eliminator.cpp\n";
      exit(1);
    }
}

// Changes the "Slices" by removing entries from "Slices" which are > high or <=  low
void trimSlices(set<int> &Slices, int high, int low)
{
set<int> Slices_Trimmed;
for(set<int>::iterator it = Slices.begin(); it != Slices.end(); it++)
	{
	if(*it <= high && *it > low)
		{
		Slices_Trimmed.insert(*it);
		}
	}
Slices = Slices_Trimmed;
}

// Checks if the constraint rooted at "Constraint" needs to be reconstructed as per the "Base"
bool needToBeReconstructed(DAGNode *Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  DAGNode *LHS = *iout;
  iout++;
  DAGNode *RHS = *iout;

  set<int> Left_Slices;
  set<int> Right_Slices;

  findSlices(LHS, WidthTable, Base, Left_Slices);
  findSlices(RHS, WidthTable, Base, Right_Slices);

  if(Left_Slices.empty() && Right_Slices.empty()) // There are no slices inside
    {
      return false;// No need to recreate
    }
  else
    {
      return true;
    }

}

// Find the set of slices in the simple term rooted at "Constraint"
void findSlices(DAGNode *Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base, set<int> &Slices)
{

  if(isConstant(Constraint)) // constant. no slices
    {
      return;
    }
  else if(isVariable(Constraint))
    {
      map<string, set<int> >::iterator entry = Base.find(Constraint->Get_node_name());

      if(entry != Base.end()) 
	{
	  Slices = entry->second;
	}
    }
  else // simple extraction
    {
      list<DAGNode*>::iterator liout = Constraint->Get_outlist_start();
      map<string, set<int> >::iterator entry = Base.find((*liout)->Get_node_name());

      if(entry != Base.end()) 
	{
	  Slices = entry->second;
	  liout++;
	  string low_str = (*liout)->Get_node_name();
	  int low = atoi(low_str.c_str());

	  liout++;
	  string high_str = (*liout)->Get_node_name();
	  int high = atoi(high_str.c_str());
		

	  trimSlices(Slices, high, low);
	}
    }// simple extraction
}// function ends here

// Reconstruct the equality rooted at "Constraint" according to "Base", and push back the 
// reconstructed equality into "ReconstructedEqualities"
void ReconstructEqualities(DAGNode *Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base, set<DAGNode*> &ReconstructedEqualities)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  DAGNode *LHS = *iout;
  iout++;
  DAGNode *RHS = *iout;

  string left_variable = getVariableFromDAG(LHS);
  string right_variable = getVariableFromDAG(RHS);

  int width_left_variable = getWidthOfVariable(WidthTable, left_variable);
  int width_right_variable = getWidthOfVariable(WidthTable, right_variable);

  
  set<int> Left_Slices;
  set<int> Right_Slices;

  findSlices(LHS, WidthTable, Base, Left_Slices);
  findSlices(RHS, WidthTable, Base, Right_Slices);

  if(Left_Slices != Right_Slices)
    {
      cout<<"\nSlices are not matching in function ReconstructEqualities in Eliminator.cpp\n";
      exit(1);
    }
  else if(Left_Slices.empty())
    {
      cout<<"\nEmpty slice in function ReconstructEqualities in Eliminator.cpp\n";
      exit(1);
    }
  else
    {
      set<int> Slices = Left_Slices;
      set<int>::iterator it = Slices.begin();
      int low = getLowIndexOfExtractionNode(LHS);
      int high;
      
      while(true)
	{
	  if(it !=  Slices.end())
	    {
	      high = (*it)-1;

	      DAGNode* LeftSide = createSelectionNode(left_variable, width_left_variable, low, high);
	      DAGNode* RightSide = createSelectionNode(right_variable, width_right_variable, low, high);
	      DAGNode* NewEquation = createDAG("=", LeftSide, RightSide, LOGICAL_SYMBOL, 1);
	      ReconstructedEqualities.insert(NewEquation);
	    } 
	  else
	    {
	      high = (getHighIndexOfExtractionNode(LHS));

	      DAGNode* LeftSide = createSelectionNode(left_variable, width_left_variable, low, high);
	      DAGNode* RightSide = createSelectionNode(right_variable, width_right_variable, low, high);
	      DAGNode* NewEquation = createDAG("=", LeftSide, RightSide, LOGICAL_SYMBOL, 1);
	      ReconstructedEqualities.insert(NewEquation);

	      break;
	    }
	  low = high + 1;
	  it++;
	}// while(true)
      
    }// else of if(Left_Slices != Right_Slices)

  showAPSet("ReconstructedEqualities", ReconstructedEqualities);

}// function ends here

// return the low index of extraction node
int getLowIndexOfExtractionNode(DAGNode *Constraint)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  iout++;
  string low_str = (*iout)->Get_node_name();
  int low = atoi(low_str.c_str());
  return low;
}

// return the high index of extraction node
int getHighIndexOfExtractionNode(DAGNode *Constraint)
{
  list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
  iout++;
  iout++;
  string hi_str = (*iout)->Get_node_name();
  int hi = atoi(hi_str.c_str());
  return hi;
}


// Functions to implement Monniaux style QE on dags with LMCs and sels, concats end here

// Functions added from 13 November 2011 onwards to implement Monniaux style QE on dags with LMCs and LMIs

// Internal function called by getAtomicPredicates_WithSupportForLMIs 
// to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
// such that the atomic predicates can be LMIs
void getAtomicPredicates_WithSupportForLMIsInternal(DAGNode* node, set<DAGNode*> &APSet, set<DAGNode*> &AlreadyEncountered)
{

  if(AlreadyEncountered.find(node)!=AlreadyEncountered.end()) // node is already encountered. return
    {
      return;
    }
              
  if(node->Get_node_name()=="and" || node->Get_node_name()=="or" || node->Get_node_name()=="if_then_else" || node->Get_node_name()=="not") // call getAtomicPredicatesInternal on each child
    {
      list<DAGNode*>::iterator iout = node->Get_outlist_start();
      unsigned int outlist_size = node->Get_outlist_size();

      for(int i=0;i<outlist_size;i++)
	{
	  getAtomicPredicates_WithSupportForLMIsInternal(*iout, APSet, AlreadyEncountered);
	  iout++;
	}
    }
  else
    {
      if(node->Get_node_name()=="=" || node->Get_node_name()=="is_not_equal" ||  node->Get_node_name()== "equivalent" ||  node->Get_node_name()== "not_equal_to") //for LMCs
	{
	  APSet.insert(node);
	}
      else if(node->Get_node_name()=="bvult" || node->Get_node_name()=="bvugt" ||  node->Get_node_name()== "bvule" ||  node->Get_node_name()== "bvuge") //for LMIs
	{
	  APSet.insert(node);
	}
      
      else if(node->Get_outlist_size()>0)// illegal node
	{
	  cout<<"\nIllegal node "<<node->Get_node_name()<<" encountered inside function getAtomicPredicates_WithSupportForLMIsInternal in file DAGEliminator.h\n";
	  exit(1);
	}
	    
    }
  AlreadyEncountered.insert(node);
}



// Function to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
// Implemented with dynamic programming such that the atomic predicates can be LMIs
void getAtomicPredicates_WithSupportForLMIs(DAGNode* node, set<DAGNode*> &APSet)
{
  set<DAGNode*> AlreadyEncountered;
  AlreadyEncountered.clear();
  getAtomicPredicates_WithSupportForLMIsInternal(node, APSet, AlreadyEncountered);
}


// Version of Generalize1_WithSupportForSelsConcats with support for LMIs also
void Generalize1_WithSupportForSelsConcats_WithSupportForLMIs(map<string, string> &Model, DAGNode* F, set<DAGNode*> &APSet, map<string, int> &WidthTable, vector<DAGNode*> &Constraints)
{
time_t start;
time_t end;
time_t duration;

time(&start); 


  Constraints.clear();

  for(set<DAGNode*>::iterator ait=APSet.begin(); ait!=APSet.end(); ait++)
    {
      //cout<<"\nConstraint "<<getDAGStr(*ait)<<" is taken\n";
      if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(*ait, WidthTable, Model)) // Model entails *ait
	{
	  //cout<<"\nConstraint "<<getDAGStr(*ait)<<" is true\n";
	  Constraints.push_back(*ait);
	}
      else // Model entails negation of *ait
	{
	  //cout<<"\nConstraint "<<getDAGStr(*ait)<<" is false\n";
	 Constraints.push_back(negateConstraint_With_LMISupport(*ait));
	}
     }

  time(&end);
  duration = end - start;
  Generalize1_time = Generalize1_time + duration;
}


// Version of Generalize2_Strategy2 with support for LMIs
void Generalize2_Strategy2_WithLMISupport(vector<DAGNode*> &Constraints, DAGNode* F)
{
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

set<DAGNode*> Model;
set<DAGNode*> ReducedModel;
for(int i=0;i<Constraints.size(); i++)
    {
      Model.insert(Constraints[i]);
    }

map<DAGNode*, pair< char, set<DAGNode*> > > DPTable;
DPTable.clear();
char ret_value = Generalize2_Strategy2_WithLMISupport_Internal(F, Model, ReducedModel, DPTable);
if(ret_value != 't')
	{
        cout<<"\nError in Generalize2_Strategy2_WithLMISupport in DAGEliminator.cpp. Return value from Generalize2_Strategy2_WithLMISupport_Internal is  "<<ret_value<<"\n";
	exit(1);
	}
Constraints.clear();
for(set<DAGNode*>::iterator it=ReducedModel.begin();it!=ReducedModel.end(); it++)
    {
      Constraints.push_back(*it);
    }

  time(&end);
  duration = end - start;
  Generalize2_time = Generalize2_time + duration;


}


// Version of Generalize2_Strategy2_Internal with LMI support
char Generalize2_Strategy2_WithLMISupport_Internal(DAGNode* F, set<DAGNode*> &Model, set<DAGNode*> &ReducedModel, map<DAGNode*, pair< char, set<DAGNode*> > > &DPTable)
{

  map<DAGNode*, pair< char, set<DAGNode*> > > ::iterator dpit = DPTable.find(F);
 
  if(dpit!=DPTable.end()) // F already encountered
	{
        //cout<<"\nDPTable Hit\n";
	ReducedModel = (dpit->second).second;
	return (dpit->second).first;
	}

  else { // encountered first time

  char ret_value;
 
  int OutlistSize = F->Get_outlist_size();

  string Name = F->Get_node_name();
 
  ReducedModel.clear();

  if(Name == "=" || Name == "is_not_equal"|| Name == "bvugt"|| Name == "bvult"|| Name == "bvuge"|| Name == "bvule") // LME/LMD/LMI
    {
      
      if( Model.find(F) !=  Model.end()) // Label as it is, is in  Model. Hence node is true
	
	{
	ReducedModel.insert(F); 
	ret_value = 't';
	}
      
      else // Label appears in the negated form
	{
	ReducedModel.insert(negateConstraint_With_LMISupport(F)); 
	ret_value = 'f'; 
	}
    }
  else if(OutlistSize != 0)// Internal node
    {
      
      if(Name == "and" && OutlistSize == 2) // binary and
	{
	  set<DAGNode*> ReducedModelChild1;
          set<DAGNode*> ReducedModelChild2;
 
	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild1, DPTable);
	  
	  iout++;

	  char child2 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild2, DPTable);

	  if(child1 == 'f' && child2 == 'f')
	    
	    { 
		findSmaller(ReducedModelChild1, ReducedModelChild2, ReducedModel); 
		ret_value = 'f';
	   }

	  else if(child1 == 'f')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 'f';
	    }
	    
	  else if(child2 == 'f')
	    
	    { 
		ReducedModel = ReducedModelChild2; 
		ret_value = 'f';
	    }

	  else if(child1 == 't' && child2 == 't')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin())); 
		ret_value = 't';
	    }
	    
	  else
	    { 
              cout<<"\nIllegal combination in and in function Generalize2_Strategy2_WithLMISupport_Internal in DAGEliminator.cpp\n";
	      exit(1);
	    }
	  
	}
      else if(Name == "or" && OutlistSize == 2) // binary or
	{
	  set<DAGNode*> ReducedModelChild1;
	  set<DAGNode*> ReducedModelChild2;

	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild1, DPTable);

	  iout++;

	  char child2 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild2, DPTable);

	  if(child1 == 't' && child2 == 't')

	    { 
		findSmaller(ReducedModelChild1, ReducedModelChild2, ReducedModel); 
		ret_value = 't';
	   }
	    
	   else if(child1 == 't')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 't';
	    }
	    
	  else if(child2 == 't')
	    
	    { 
		ReducedModel = ReducedModelChild2; 
		ret_value = 't';
	    }

	  else if(child1 == 'f' && child2 == 'f')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	    }
	    
	  else
	    { 
              cout<<"\nIllegal combination in or in function Generalize2_Strategy2_WithLMISupport_Internal in DAGEliminator.cpp\n";
	      exit(1);
	    }

	  
	}
      else if(Name == "if_then_else")
	{
	  set<DAGNode*> ReducedModelChild1;
	  set<DAGNode*> ReducedModelChild2;
	  set<DAGNode*> ReducedModelChild3; 

	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild1, DPTable);

	  iout++;

	  char child2 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild2, DPTable);

	  iout++;

	  char child3 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild3, DPTable);

	  if(child1 == 't' && child2 == 't')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 't';
	   }
	
	  else if(child1 == 'f' && child3 == 't')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild3.begin(), ReducedModelChild3.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 't';
	   }
	  
	  else if(child1 == 't' && child2 == 'f')

	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild2.begin(), ReducedModelChild2.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	   }
	
	  else if(child1 == 'f' && child3 == 'f')
	    
	    { 
		set_union(ReducedModelChild1.begin(), ReducedModelChild1.end(), ReducedModelChild3.begin(), ReducedModelChild3.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	   }

	  else if(child2 == 'f' && child3 == 'f')

	    { 
		set_union(ReducedModelChild2.begin(), ReducedModelChild2.end(), ReducedModelChild3.begin(), ReducedModelChild3.end(), inserter(ReducedModel, ReducedModel.begin()));  
		ret_value = 'f';
	    }
	  
	  
	  else
	    { 
              cout<<"\nIllegal combination in ite in function Generalize2_Strategy2_WithLMISupport_Internal in DAGEliminator.cpp\n";
	      exit(1);
	    }
	}


     else if(Name == "not")
	{
	  set<DAGNode*> ReducedModelChild1;

	  list<DAGNode*>::iterator iout = F->Get_outlist_start();

	  char child1 =  Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild1, DPTable);

	  if(child1 == 't')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 'f';
	    }
	    
	  else if(child1 == 'f')
	    
	    { 
		ReducedModel = ReducedModelChild1; 
		ret_value = 't';
	    }

	  else
	    { 
              cout<<"\nIllegal combination in not in function Generalize2_Strategy2_WithLMISupport_Internal in DAGEliminator.cpp in node  "<<getDAGStr(F)<<"\n";
	      exit(1);
	    }

	  
	}
      else if(Name == "and" && OutlistSize > 2) // and with more than 2 children
	{
	  vector< set<DAGNode*> > ReducedModelChildren;
	  vector<char> ResultChildren;
 
	  list<DAGNode*>::iterator iout = F->Get_outlist_start();
	  for(int i=0;i<OutlistSize;i++)
	    {
	      set<DAGNode*> ReducedModelChild;

	      char child = Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild, DPTable);
	      ReducedModelChildren.push_back(ReducedModelChild);
	      ResultChildren.push_back(child);

	      iout++;
	    }

	  ret_value = Generalize2_Strategy2_Internal_For_And_With_Multiple_Children(ReducedModelChildren, ResultChildren, ReducedModel);
	}

      else if(Name == "or" && OutlistSize > 2) // or with more than 2 children
	{
	  vector< set<DAGNode*> > ReducedModelChildren;
	  vector<char> ResultChildren;
 
	  list<DAGNode*>::iterator iout = F->Get_outlist_start();
	  for(int i=0;i<OutlistSize;i++)
	    {
	      set<DAGNode*> ReducedModelChild;

	      char child = Generalize2_Strategy2_WithLMISupport_Internal(*iout,  Model, ReducedModelChild, DPTable);
	      ReducedModelChildren.push_back(ReducedModelChild);
	      ResultChildren.push_back(child);

	      iout++;
	    }

	  ret_value = Generalize2_Strategy2_Internal_For_Or_With_Multiple_Children(ReducedModelChildren, ResultChildren, ReducedModel);
	}
      else
	{
	  cout<<"\nUnknown node "<<Name<<" in function Generalize2_Strategy2_WithLMISupport_Internal in DAGEliminator.cpp\n";
	  exit(1);
	}
    }// internal node

   else // leaf
   	{
	if(Name=="true")
		{
		ret_value = 't';
		}
	else if(Name=="false")
		{
		ret_value = 'f';
		}
	else
		{
		cout<<"\nUnknown node "<<Name<<" in function Generalize2_Strategy2_WithLMISupport_Internal in DAGEliminator.cpp\n";
		exit(1);
		}
	}// leaf

   DPTable.insert(make_pair(F, make_pair(ret_value, ReducedModel)));
   return ret_value;
}// DPTable miss

}// Function ends 



// Function to return exists VarsToElim. (/\ Constraints) given (i) a set of variables VarsToElim ,and
// (ii) a set of constraints Constraints which are LMIs..
// QE is done with bit-blasting + CUDD
DAGNode* QE_Using_Blasting_WithSupportForLMIs(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
   //showAPVector("Constraints", Constraints);
 //showSet(VarsToElim, "VarsToElim"); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
  
  DAGNode* InputToBlasting = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
           Bound_Constraints_Vector.push_back(*it); 
        }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}  
  
  
  DAGNode* ResultOfQE = ddm.qElimUsingBlasting(InputToBlasting, VarsToElim, WidthTable);
  //cout<<"\nResultOfQE = "<<getDAGStr(ResultOfQE)<<endl;
 
  DAGNode* FinalResult; // FinalResult = conjunction of constraints in Free_Constraints /\ ResultOfQE
  DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
  FinalResult = quickSimplify(createDAG("and",FreePart, ResultOfQE, LOGICAL_SYMBOL, 1));

  //cout<<endl<<"FinalResult = "<<getDAGStr(FinalResult)<<endl;
  return FinalResult;
}

// Call semantic scope reduction using unsat core computation
// Here Phi = /\ of constraints in Constraints
// Result is in 1) partition returned in PartitionedConjunction, 2) return false if timed out; else true
 bool semanticScopeReduction(DAGNode* Phi, set<DAGNode*> &Constraints,  set<string> &VarsToElim, map<string, int> &WidthTable, set< set<DAGNode*> > &PartitionedConjunction)
 {
     
     bool keep_partitions_non_intersecting = false;
     bool check_for_proper_partition = true;
     bool cross_check_unsat_core = false;
     bool original_z3_constraint = true;
     string solver = "picosat";
     
    
     
     //cout<<"\nPhi = "<<getDAGStr(Phi)<<endl;
     
     //showAPSet("Constraints", Constraints);
     
     map<int, DAGNode*> ConstraintMap;
     map<DAGNode*, int> ReverseConstraintMap;
     
     initializeConstraintMaps(ConstraintMap, ReverseConstraintMap, Constraints); 
     // ConstraintMap associates a unique number with each constraint
     // ReverseConstraintMap is the reverse mapping of it
          
     //showConstraintMaps(ConstraintMap, ReverseConstraintMap);
     
       
     set<string> VarsToRemain;
     
     getVariablesToRemain(Phi, VarsToElim,  VarsToRemain);
     
     //showSet(VarsToElim, "VarsToElim");
     
     //showSet(VarsToRemain, "VarsToRemain");
     
     DAGNode* NegPhi = createDAG("not", Phi, LOGICAL_SYMBOL, 1); // NegPhi = \neg Phi
             
     map<string, string> YMap;   // Model of forall VarsToElim. \neg Phi     
     
     string result_from_z3;
     
     result_from_z3 = getModelForQBVF_UsingZ3_WithSupportForLMIs(NegPhi, YMap,  WidthTable,  VarsToElim,  VarsToRemain);
                 
     if(result_from_z3 == "unsat") 
     {
         // forall VarsToElim. \neg Phi  is unsat i.e. \exists VarsToElim. Phi is valid
         //cout<<"\nexists X. Phi is valid\n";
         fprintf(LMIQEOutputDataFp, "\nexists X. Phi is valid\n");
         //cout<<"\nPress any key to consider the next exists x_i. A\n";
         //char c=getchar(); 
         
         PartitionedConjunction.clear(); // result is true
         return true;// result can be trusted
     }
     else if(result_from_z3 == "unknown")
     {
         // forall VarsToElim. \neg Phi  is unknown
         //cout<<"\nexists X. Phi is unknown. z3 timed out\n";
          fprintf(LMIQEOutputDataFp, "\nexists X. Phi is unknown. z3 timed out\n");
         //cout<<"\nPress any key to consider the next exists x_i. A\n";
         //char c=getchar(); 
         return false;// result can't be trusted
     }
     else // result_from_z3 is sat
     {
        //cout<<"\nexists X. Phi is not valid\n"; 
        //cout<<"\nPress any key to continue\n";
        //char c=getchar(); 
        
        vector< set<int> > CorePartitions;
        set<int> RemainingPartition;
        
        DAGNode* PhiPrime = cloneUsingValueMap(Phi, YMap);
        
        //cout<<"\nPhiPrime = "<<getDAGStr(PhiPrime)<<endl;
        
        set<DAGNode*> CoreConstraints;
        
        if(solver=="zchaff")
        {
           blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAG(PhiPrime, WidthTable, CoreConstraints); 
        }
        else if(solver=="picosat")
        {
            blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAGUsingPicoSAT(PhiPrime, WidthTable, CoreConstraints); 
        }
        else
        {
            cout<<"\nUnknown solver "<<solver<<" inside function semanticScopeReduction in DAGEliminator.cpp\n";
            exit(1);
        }
        
        
        
        //showAPSet("CoreConstraints", CoreConstraints);
        
        if(cross_check_unsat_core)
        {
           DAGNode* temp = getConjunctionFromSet(CoreConstraints);
           if(!ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(temp, WidthTable))
           {
               cout<<"\nUnsat core incorrect inside function semanticScopeReduction in DAGEliminator.cpp\n";
               exit(1);
           }
           else
           {
                //cout<<"\nUnsat core correct. Press any key to continue";
               //char c=getchar();               
           }
        }
        
        set<int> CoreConstraintIndices;
        set<int> OtherConstraintIndices;
        
        getCoreConstraintIndices(ReverseConstraintMap, CoreConstraints, CoreConstraintIndices, OtherConstraintIndices, YMap);
               
        
        if(check_for_proper_partition)
        {
            if(OtherConstraintIndices.empty())// Unsat core has the entire \phi
            {
                //cout<<"\nUnsat core includes the entire Phi in function semanticScopeReduction in DAGEliminator.cpp\n";
                
                //cout<<"\nLet us try to find another unsat core. Press any key to continue";
                //char c = getchar();
               
                // try to find another unsat. core
                tryToFindUnsatCore(Constraints, YMap, WidthTable, CoreConstraintIndices, OtherConstraintIndices, ReverseConstraintMap, check_for_proper_partition, cross_check_unsat_core, solver);
                
                if(OtherConstraintIndices.empty())// Unsat core still has the entire \phi
                        {
                        //cout<<"\nNo simplification possible\n";
                        //cout<<"\nPress any key to consider the next exists x_i. A\n";
                        //char c=getchar();  
                        
                        PartitionedConjunction.insert(Constraints); // no simplification. Hence input = output
                        return true;     
                        }// Unsat core still has the entire \phi
            }// Unsat core  has the entire \phi
        }// end of if(check_for_proper_partition)
        
        //cout<<"\nForming the partitions based on the constraint indices obtained\n";
        
        CorePartitions.push_back(CoreConstraintIndices);
        RemainingPartition = OtherConstraintIndices;
        
        //showPartition(CorePartitions, RemainingPartition);
        
        // Forming the z3 problem instance
        
        bool loop_entry;
        
        set<string> UnivQuantVars;
        set<string> ExistQuantVars;
        DAGNode *Condition;
        DAGNode *PreventedYMaps;
        
        if(original_z3_constraint) // Try the constraint in original form
        {
            
            Condition = createConstraintFromPartition(CorePartitions, RemainingPartition, ConstraintMap, Constraints, VarsToElim,  WidthTable, NegPhi,  UnivQuantVars, ExistQuantVars);

            //cout<<"\nCondition = "<<getDAGStr(Condition)<<endl;
            //showSet(UnivQuantVars, "UnivQuantVars");
            //showSet(ExistQuantVars, "ExistQuantVars");
            
            YMap.clear();
            
            result_from_z3 = getModelForQBVF_UsingZ3_WithSupportForLMIs(Condition, YMap,  WidthTable,  UnivQuantVars,  ExistQuantVars);
            
            if(result_from_z3 == "unsat")
            {
                loop_entry = false;
            }
            else if(result_from_z3 == "unknown")
            {
               //cout<<"\nexists X. Phi is unknown. z3 timed out\n";
               //cout<<"\nPress any key to consider the next exists x_i. A\n";
               fprintf(LMIQEOutputDataFp, "\nexists X. Phi is unknown. z3 timed out\n");
               //char c=getchar(); 
               return false;
            }
            else // result_from_z3 is sat
            {
                loop_entry = true;
            }
            
            
        }
        else // Try the constraint in simplified form
        {
            DAGNode *YMapDAG = createDAGFromYMap(YMap, WidthTable); // dag is like (y0=v0) /\ .... /\ (yn=vn)
            DAGNode *PreventedYMaps =  createDAG("not", YMapDAG, LOGICAL_SYMBOL, 1); // It is like ~( (y0=v0) /\ .... /\ (yn=vn) )
            
            YMap.clear();
            PreventedYMaps = checkSimplifiedZ3Constraint(NegPhi, PreventedYMaps, WidthTable, YMap, CorePartitions, RemainingPartition, ConstraintMap, Constraints, VarsToElim, VarsToRemain, result_from_z3);
            // set result_from_z3 = sat if we want loop entry; unsat/unknown otherwise. Also return YMap, PreventedYMaps properly
            
            if(result_from_z3 == "unsat")
            {
                loop_entry = false;
            }
            else if(result_from_z3 == "unknown")
            {
               //cout<<"\nexists X. Phi is unknown. z3 timed out\n";
               fprintf(LMIQEOutputDataFp, "\nexists X. Phi is unknown. z3 timed out\n");
               //cout<<"\nPress any key to consider the next exists x_i. A\n";
               //char c=getchar(); 
               return false;
            }
            else // result_from_z3 is sat
            {
                loop_entry = true;
            }
        }
        
        int loop_counter = 1;
        
        while(loop_entry)
            {
            // \exists ExistQuantVars. \forall UnivQuantVars. Condition  is sat 
            //cout<<"\nLoop iteration "<<loop_counter<<". exists ExistQuantVars. forall UnivQuantVars. Condition  is sat  \n";
            
            //cout<<"\nPress any key to continue";
            
            //char c = getchar();
            
            DAGNode* PhiPrimeInLoop = cloneUsingValueMap(Phi, YMap);
        
            //cout<<"\nPhiPrimeInLoop = "<<getDAGStr(PhiPrimeInLoop)<<endl;
        
            set<DAGNode*> CoreConstraintsInLoop;
            
            if(solver=="zchaff")
            {
                blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAG(PhiPrimeInLoop, WidthTable, CoreConstraintsInLoop); 
            }
            else if(solver=="picosat")
            {
                blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAGUsingPicoSAT(PhiPrimeInLoop, WidthTable, CoreConstraintsInLoop); 
            }
            else
            {
                cout<<"\nUnknown solver "<<solver<<" inside function semanticScopeReduction in DAGEliminator.cpp\n";
                exit(1);
            }
                   
            
            
            if(cross_check_unsat_core)
            {
            DAGNode* temp = getConjunctionFromSet(CoreConstraintsInLoop);
            if(!ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(temp, WidthTable))
                {
                cout<<"\nUnsat core incorrect inside function semanticScopeReduction in DAGEliminator.cpp\n";
                exit(1);
                }
            else
                {
                cout<<"\nUnsat core correct. Press any key to continue";
               //char c=getchar();               
                }
            }
        
            //showAPSet("CoreConstraintsInLoop", CoreConstraintsInLoop);
        
            set<int> CoreConstraintIndicesInLoop;
            set<int> OtherConstraintIndicesInLoop;
        
            getCoreConstraintIndices(ReverseConstraintMap, CoreConstraintsInLoop, CoreConstraintIndicesInLoop, OtherConstraintIndicesInLoop, YMap);
             
            
            
            if(check_for_proper_partition)
            {
                if(OtherConstraintIndicesInLoop.empty())// Unsat core has the entire \phi
                {
                //cout<<"\nUnsat core includes the entire Phi in function semanticScopeReduction in DAGEliminator.cpp\n";
                
                //cout<<"\nLet us try to find another unsat core. Press any key to continue";
                //char c = getchar();
               
                // try to find another unsat. core
                tryToFindUnsatCore(Constraints, YMap, WidthTable, CoreConstraintIndicesInLoop, OtherConstraintIndicesInLoop, ReverseConstraintMap, check_for_proper_partition, cross_check_unsat_core, solver);
                
                 if(OtherConstraintIndicesInLoop.empty())// Unsat core still has the entire \phi
                        {
                        //cout<<"\nNo simplification possible\n";
                        //cout<<"\nPress any key to consider the next exists x_i. A\n";
                        //char c=getchar();    
                        
                        PartitionedConjunction.insert(Constraints);
                        return true;     
                        }// Unsat core still has the entire \phi
                }// Unsat core has the entire \phi
            } // end of if(check_for_proper_partition)
            
            
            //cout<<"\nWe need to refine the partitions based on new constraint indices obtained\n";
            
            //showPartition(CorePartitions, RemainingPartition);
            
            if(keep_partitions_non_intersecting)
            {

                refinePartition(CorePartitions, RemainingPartition, CoreConstraintIndicesInLoop); 

                if(RemainingPartition.empty() && CorePartitions.size()==1)// There's only a single partition; exit from loop
                    {
                    //cout<<"\nThere's only a single partition; exiting from loop\n";
                    break;
                    }
            }
            else //partitions can be intersecting. There is always simplication except when unsat core is the entire \phi
            {
                refineIntersectingPartition(CorePartitions, RemainingPartition, CoreConstraintIndicesInLoop); 
            }
        
            //showPartition(CorePartitions, RemainingPartition);
            
            //cout<<"\nPress any key to continue";
            //c = getchar();
            
           if(original_z3_constraint) // Try the constraint in original form
            {
                 UnivQuantVars.clear();
                 ExistQuantVars.clear();
                
                Condition = createConstraintFromPartition(CorePartitions, RemainingPartition, ConstraintMap, Constraints, VarsToElim,  WidthTable, NegPhi,  UnivQuantVars, ExistQuantVars);

                //cout<<"\nCondition = "<<getDAGStr(Condition)<<endl;
                //showSet(UnivQuantVars, "UnivQuantVars");
                //showSet(ExistQuantVars, "ExistQuantVars");
                YMap.clear();
                
                               
                result_from_z3 = getModelForQBVF_UsingZ3_WithSupportForLMIs(Condition, YMap,  WidthTable,  UnivQuantVars,  ExistQuantVars);
            
                if(result_from_z3 == "unsat")
                {
                    loop_entry = false;
                }
                else if(result_from_z3 == "unknown")
                {
                    //cout<<"\nexists X. Phi is unknown. z3 timed out\n";
                    fprintf(LMIQEOutputDataFp, "\nexists X. Phi is unknown. z3 timed out\n");
                    //cout<<"\nPress any key to consider the next exists x_i. A\n";
                    //char c=getchar(); 
                    return false;
                }
                else // result_from_z3 is sat
                {
                    loop_entry = true;
                }
            
            }
            else // Try the constraint in simplified form
            {
                DAGNode *YMapDAG = createDAGFromYMap(YMap, WidthTable); // dag is like (y0=v0) /\ .... /\ (yn=vn)
                DAGNode *PreventedYMaps =  createDAG("not", YMapDAG, LOGICAL_SYMBOL, 1); // It is like ~( (y0=v0) /\ .... /\ (yn=vn) )
            
                YMap.clear();
                PreventedYMaps = checkSimplifiedZ3Constraint(NegPhi, PreventedYMaps, WidthTable, YMap, CorePartitions, RemainingPartition, ConstraintMap, Constraints, VarsToElim, VarsToRemain, result_from_z3);
                // set result_from_z3 = sat if we want loop entry; unsat/unknown otherwise. Also return YMap, PreventedYMaps properly
            
                if(result_from_z3 == "unsat")
                {
                    loop_entry = false;
                }
                else if(result_from_z3 == "unknown")
                {
                    cout<<"\nexists X. Phi is unknown. z3 timed out\n";
                    cout<<"\nPress any key to consider the next exists x_i. A\n";
                    //char c=getchar(); 
                    return false;
                }
                else // result_from_z3 is sat
                {
                    loop_entry = true;
                }
            }// else ends here
            
            loop_counter++;
           }// loop ends here
        
       // exists ExistQuantVars. \forall UnivQuantVars. Condition  is unsat 
       //cout<<"\nexists ExistQuantVars. forall UnivQuantVars. Condition  is unsat  for the given set of partitions\n"; 
       //showPartition(CorePartitions, RemainingPartition);
       //cout<<"\nPress any key to consider the next exists x_i. A\n";
       //c=getchar();  
       
       createPartitionedConjunction(ConstraintMap, CorePartitions, RemainingPartition, PartitionedConjunction);
       
       
       bool remove_trivial_partitions = true;
       if(remove_trivial_partitions)
       {
          set< set<DAGNode*> > PartitionedConjunctionWithoutTrivialPartitions;
          
          for(set< set< DAGNode* > >::iterator par_conj_it = PartitionedConjunction.begin(); par_conj_it != PartitionedConjunction.end(); par_conj_it++)
          {
              set<DAGNode*> Partition_Set = *par_conj_it;
              DAGNode* Partition_Conjunction = getConjunctionFromSet(Partition_Set);
              
              //cout<<"\nPartition_Conjunction = "<<getDAGStr(Partition_Conjunction)<<endl;
              
              set<string> VarsToRemainInPartition_Conjunction;
              getVariablesToRemain(Partition_Conjunction, VarsToElim,  VarsToRemainInPartition_Conjunction);
     
              DAGNode* Neg_Partition_Conjunction = createDAG("not", Partition_Conjunction, LOGICAL_SYMBOL, 1); 
              map<string, string> YMap_Partition_Conjunction;   // Model of forall VarsToElim. \neg Partition_Conjunction     
     
              string result_from_z3_for_partition;
              result_from_z3_for_partition = getModelForQBVF_UsingZ3_WithSupportForLMIs(Neg_Partition_Conjunction, YMap_Partition_Conjunction,  WidthTable,  VarsToElim,  VarsToRemainInPartition_Conjunction);
              
              //cout<<"\nresult_from_z3_for_partition =  "<<result_from_z3_for_partition<<endl;
              
              if(result_from_z3_for_partition != "unsat")
                {
                  PartitionedConjunctionWithoutTrivialPartitions.insert(Partition_Set);
                }//if
          }//for
          
          PartitionedConjunction.clear();
          PartitionedConjunction = PartitionedConjunctionWithoutTrivialPartitions;
       } //if(remove_trivial_partitions)
          
        
       return true; 
     }// result_from_z3 is sat
     
 }

 
 

// This returns the set of variables other than the variables to be eliminated
void getVariablesToRemain(DAGNode* Phi, set<string> &VarSetToElim, set<string> &VarSetRemaining)
{
  set<string> PhiSupport;
  
  getDAGNames(Phi, PhiSupport);
          
  for(set<string>::iterator it=PhiSupport.begin(); it!=PhiSupport.end(); it++)
    {
      if(VarSetToElim.find(*it) == VarSetToElim.end())
	{
	VarSetRemaining.insert(*it);
	}
    }
}


// Initialize the ConstraintMaps. 
// ConstraintMap associates a unique number with each constraint
// ReverseConstraintMap is the reverse mapping
void initializeConstraintMaps(map<int, DAGNode*> &ConstraintMap, map<DAGNode*, int> &ReverseConstraintMap, set<DAGNode*> &Constraints)
{
int index = 1;

 for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     ConstraintMap.insert(make_pair(index, *it));
     ReverseConstraintMap.insert(make_pair(*it, index));
     index++;
    }
 
}



// show the ConstraintMaps. 
// ConstraintMap associates a unique number with each constraint
// ReverseConstraintMap is the reverse mapping
void showConstraintMaps(map<int, DAGNode*> &ConstraintMap, map<DAGNode*, int> &ReverseConstraintMap)
{

 cout<<"\n\nConstraint Map\n***************\n\n";   
 for(map<int, DAGNode*>::iterator it=ConstraintMap.begin(); it!=ConstraintMap.end(); it++)
    {
     cout<<it->first<<"\t"<<getDAGStr(it->second)<<endl;
     }
 
 cout<<"\n\nReverse Constraint Map\n***************\n\n";   
 for(map<DAGNode*, int>::iterator it=ReverseConstraintMap.begin(); it!=ReverseConstraintMap.end(); it++)
    {
     cout<<getDAGStr(it->first)<<"\t"<<it->second<<endl;
     }
 
}


// Print the dag rooted at "dag" in CVC3 SMTLIB format as "forall VarsToElim. dag".
// call z3 to (i) check if it is sat/unsat and set the  return value accordingly. return value = sat | unsat | unknown
//, and (ii) if sat, get the model in result_conjunction.txt
string ThUnsatSingleFormulaWithCEXFileInCVC3StyleSMTLibFormat_WithLMISupport(DAGNode* dag, map<string, int> &WidthTable, set<string> &VarsToElim, set<string> &VarsToRemain)
{
  FILE *check_file = fopen("conjunction.smt", "w");

  if(check_file == NULL) 
   {cout<<"\nCannot create conjunction.smt\n"; exit(1);}

  fprintf(check_file,"(benchmark conjunction\n:logic QF_UFBV[32]\n");


  list<string> VarsToRemainList;
  
  for(set<string>::iterator it=VarsToRemain.begin(); it!=VarsToRemain.end(); it++)
    {
      VarsToRemainList.push_back(*it);
    }
  
  
  writeBVDeclarationsInSMTLibFormatWithLookup(check_file,  WidthTable,  VarsToRemainList);


  string original_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(dag, WidthTable, VarsToElim);
  

  string exists_str = getExistsString(VarsToElim,WidthTable);

  
  fprintf(check_file,"\n:formula  (forall %s %s)", exists_str.c_str(), original_str.c_str()); 
  
  fprintf(check_file,"\n)\n");
  
  fclose(check_file);
  
  
  // Creating timeout file for z3
  
  
  
  FILE *tout_file = fopen("timeout_z3.txt", "w");

  if(tout_file == NULL) 
   {cout<<"\nCannot create timeout_z3.txt in function ThUnsatSingleFormulaWithCEXFileInCVC3StyleSMTLibFormat_WithLMISupport in DAGEliminator.cpp\n"; exit(1);}
  
  fprintf(tout_file, "SOFT_TIMEOUT=");
  
  fprintf(tout_file, "%llu", z3_timeout_ms);
  
  fclose(tout_file);
  

	
  system("z3 -smt -m  -ini:timeout_z3.txt conjunction.smt > result_conjunction.txt");
 
   cout<<"\nZ3 called\n";

  FILE *result_check_file = fopen("result_conjunction.txt", "r");
  
  if(result_check_file == NULL) 
   {cout<<"\nCannot create result_conjunction.txt\n"; exit(1);}


  char result[100];
  string result_str = "";
  
  while(result_str != "sat" && result_str != "unsat" && result_str != "unknown")
  {
       fscanf(result_check_file, "%s", result);
       result_str = result;
  }
  
  fclose(result_check_file);
  
  cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat")
    {
      cout<<"\nz3 returns unsat\n";
      return result_str;
     }      
  else if(result_str=="sat")
    { 
      cout<<"\nz3 returns sat\n";
      return result_str;
    }
  else if(result_str=="unknown")
    { 
      cout<<"\nz3 returns unknown\n";
      return result_str;
    }
  else
    {
      cout<<"\nUnidentified result "<<result_str<<" from z3 in function ThUnsatSingleFormulaWithCEXFileInCVC3StyleSMTLibFormat_WithLMISupport in DAGEliminator.cpp\n";;
      exit(1);
    }


}




// Print the dag rooted at "dag" in CVC3 SMTLIB format as "forall VarsToElim. dag".
// call z3 to (i) check if it is sat/unsat and set the  return value accordingly. return value can be sat | unsat | unknown
//, and (ii) if sat, get the model in the map Model
string getModelForQBVF_UsingZ3_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable, set<string> &VarSetToElim, set<string> &VarsToRemain)
{
  Model.clear();
  
  string result = ThUnsatSingleFormulaWithCEXFileInCVC3StyleSMTLibFormat_WithLMISupport(node, WidthTable, VarSetToElim, VarsToRemain);
  
  if(result == "unsat" || result == "unknown") // node is unsat/unknown; return it
    {
      return result;
    }

  // node is sat; CEX is in result_conjunction.txt
  getModelFromZ3CEXFile(Model);
  //showModel(Model);
  return result;
}



// Get the model in the variable --> value map Model 
// by parsing the CEX returned from z3 in the file result_conjunction.txt
void getModelFromZ3CEXFile(map<string, string> &Model)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("result_conjunction.txt");
  
  string s;
  string signal;
  string value;

  for(int count=0;!infile->eof(); count++)
    {
 	
      *infile>>s;
      if(s=="")
	break;
      //cout<<"Line read is "<<s<<endl;
      if(s=="sat" || s=="unsat") // sat/unsat line
	{
	  break;
	}
      else if(count%3==0) // signal line
	{
          signal = s;
	  continue;
	}
      else if(count%3==1) // -> line
	{
	  
	  continue;
	}
      else // value line
	{
	  value  = s;

	  int start_pos = value.find("v");
	  value  = value.substr(start_pos+1);
	 
          
	  Model.insert(make_pair(signal, value));
	}
    }

  infile->close();

}


// Get indices of core constraints and other constraints in CoreConstraintIndices, OtherConstraintIndices respectively
void getCoreConstraintIndices(map<DAGNode*, int> &ReverseConstraintMap, set<DAGNode*> &CoreConstraints, set<int> &CoreConstraintIndices, set<int> &OtherConstraintIndices, map<string, string> &YMap)
{
    for(map<DAGNode*, int>::iterator it=ReverseConstraintMap.begin(); it != ReverseConstraintMap.end(); it++)
    {
        DAGNode* dag = cloneUsingValueMap(it->first, YMap);
        set<DAGNode*>::iterator cit = CoreConstraints.find(dag);
        if(cit != CoreConstraints.end()) // dag is a core constraint. Insert its index into CoreConstraintIndices
        {
            CoreConstraintIndices.insert(it->second);
        }
        else
        {
            OtherConstraintIndices.insert(it->second) ;
        }        
    }
    //showSet(CoreConstraintIndices, "CoreConstraintIndices");
    //showSet(OtherConstraintIndices, "OtherConstraintIndices");
}


// Show the partitions
void showPartition(vector < set<int> > &CorePartitions, set<int> &RemainingPartition)
{
    cout<<"\nCore Partitions\n";
    int i=1;
    for(vector< set<int> >::iterator it=CorePartitions.begin(); it != CorePartitions.end(); it++)
    {
        set<int> ThisPartition = *it;
        cout<<"\nPartition "<<i<<endl;
        showSet(ThisPartition, "Partition");
         i++;       
    }
    
    showSet(RemainingPartition, "RemainingPartition");
}



// Create the constraint from the parition
DAGNode* createConstraintFromPartition(vector< set<int> > &CorePartitions, set<int> &RemainingPartition, map<int, DAGNode*> &ConstraintMap, set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode *NegPhi, set<string> &UnivQuantVars, set<string> &ExistQuantVars)
{
    bool remaining_partition_can_be_empty = true;
    
    DAGNode* Condition = NULL;
    int partition_index=1;
    
    //cout<<"\n Remaining Partition\n";
    
   set<DAGNode*> ConstraintsFromRemainingPartition;
   getConstraintsFromParticularPartition(RemainingPartition, ConstraintMap, ConstraintsFromRemainingPartition);
   DAGNode* Conjunction = getConjunctionFromSet(ConstraintsFromRemainingPartition);

   
   if(!remaining_partition_can_be_empty)
   {
        if(isTrue(Conjunction))
        {
       cout<<"Empty partition in function createConstraintFromPartition in DAGEliminator.cpp\n";
       exit(1);
        }
   }  
  

   DAGNode* ClonedConjunction = cloneAndShiftWithWidthTableUpdate(Conjunction, partition_index, VarsToElim, WidthTable);

   //cout<<"\nCloned Conjunction "<<getDAGStr(ClonedConjunction)<<endl;

   if(Condition==NULL)
        {
        Condition = ClonedConjunction;
        }
   else
        {
        Condition = createDAG("and", Condition, ClonedConjunction, LOGICAL_SYMBOL, 1);
        }

   partition_index++;     
    
    
    for(vector< set<int> >::iterator it=CorePartitions.begin(); it != CorePartitions.end(); it++)
    {
        //cout<<"\n Partition "<<partition_index<<endl;
        
       set<int> ThisPartition = *it;
       set<DAGNode*> ConstraintsFromThisPartition;
       getConstraintsFromParticularPartition(ThisPartition, ConstraintMap, ConstraintsFromThisPartition);
       Conjunction = getConjunctionFromSet(ConstraintsFromThisPartition);
       
       if(isTrue(Conjunction))
       {
           cout<<"Empty partition in function createConstraintFromPartition in DAGEliminator.cpp\n";
           exit(1);
       }

       ClonedConjunction = cloneAndShiftWithWidthTableUpdate(Conjunction, partition_index, VarsToElim, WidthTable);
       
       //cout<<"\nCloned Conjunction "<<getDAGStr(ClonedConjunction)<<endl;
       
       if(Condition==NULL)
            {
            Condition = ClonedConjunction;
            }
       else
            {
            Condition = createDAG("and", Condition, ClonedConjunction, LOGICAL_SYMBOL, 1);
            }
               
       partition_index++;        
    }
    
   DAGNode* ClonedNegPhi = cloneAndShiftWithWidthTableUpdate(NegPhi, partition_index, VarsToElim, WidthTable); 
   
   //cout<<"\nCloned NegPhi "<<getDAGStr(ClonedNegPhi)<<endl;
   
   if(Condition==NULL)
            {
            Condition = ClonedNegPhi;
            }
   else
            {
            Condition = createDAG("and", Condition, ClonedNegPhi, LOGICAL_SYMBOL, 1);
            }
   if(Condition==NULL)
   {
       cout<<"\nError in function createConstraintFromPartition in Eliminator.cpp. Condition becoming NULL\n";
       exit(1);
   }
   // Condition obtained
   // Obtain UnivQuantVars and ExistQuantVars
   
   for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
        string variable = *it;
               
        string new_name = variable;
	new_name += "_";
	char temp[100];
	sprintf(temp, "%d", partition_index); // partition_index here is the index of variables to elim in \neg Phi
	string delta_str = temp;
	new_name += delta_str;
        
        UnivQuantVars.insert(new_name);
    }
   
  getVariablesToRemain(Condition, UnivQuantVars, ExistQuantVars); 
  
  return Condition;
                
}


// Get the constraints from a partition
void getConstraintsFromParticularPartition(set<int> &ThisPartition, map<int, DAGNode*> &ConstraintMap, set<DAGNode*> &ConstraintsFromThisPartition)
{
    for(set<int>::iterator it=ThisPartition.begin(); it != ThisPartition.end(); it++)
    {
        int index = *it;
        map<int, DAGNode*>::iterator cit = ConstraintMap.find(index);
        if(cit == ConstraintMap.end())
        {
            cout<<"\nThere is no entry for "<<index<<" in ConstraintMap. Error in function getConstraintsFromParticularPartition in DAGEliminator.cpp\n";
            exit(1);
        }
        DAGNode *dag = cit->second;
        ConstraintsFromThisPartition.insert(dag);
    }
}


// Function to refine the existing partition based on the new unsat. core
void refinePartition(vector< set<int> > &CorePartitions, set<int> &RemainingPartition, set<int> &CoreConstraintIndicesInLoop)
{
    // Let us call CorePartitions as \pi, RemainingPartition as \alpha, and CoreConstraintIndicesInLoop as \beta
    set<int> BetaMinusAlpha;
    set_difference(CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), RemainingPartition.begin(), RemainingPartition.end(),
		     inserter(BetaMinusAlpha, BetaMinusAlpha.begin()));
    
    if(BetaMinusAlpha.empty()) // \beta subset of \alpha
    {
        CorePartitions.push_back(CoreConstraintIndicesInLoop);// \pi = \pi \union \beta
        set<int> AlphaMinusBeta;
        set_difference(RemainingPartition.begin(), RemainingPartition.end(), CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), 
		     inserter(AlphaMinusBeta, AlphaMinusBeta.begin()));
        RemainingPartition = AlphaMinusBeta;// \alpha = \alpha \minus \beta
    }
    else
    {
        set<int> BetaInterAlpha;
        set_intersection(CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), RemainingPartition.begin(), RemainingPartition.end(),
		     inserter(BetaInterAlpha, BetaInterAlpha.begin()));
        
        if(BetaInterAlpha.empty()) 
        {
            set<int> PartitionsIntersectingNewUnsatcore;
            findPartitionsIntersectingNewUnsatcore(CorePartitions, CoreConstraintIndicesInLoop, PartitionsIntersectingNewUnsatcore);
            
            if(PartitionsIntersectingNewUnsatcore.empty())// \beta does not intersect with any \pi_i. Then why \neg ( BetaMinusAlpha.empty()))
            {
                cout<<"\nInfeasibe partition arrived in function refinePartition in DAGEliminator.cpp\n";
                exit(1);
            }
            else if(PartitionsIntersectingNewUnsatcore.size()==1) // \beta intersects with exactly one \pi_i
            {
                // do nothing
                return;
            }
            else // \beta intersects with more than one \pi_i
            {
                set<int> NewPartition; // NewPartition = union of all these intersecting partitions
                for(int i=0; i<CorePartitions.size(); i++)
                {
                    if(PartitionsIntersectingNewUnsatcore.find(i) != PartitionsIntersectingNewUnsatcore.end()) // i is a partition
                        // intersecting with \beta
                    {
                        set_union(NewPartition.begin(), NewPartition.end(), CorePartitions[i].begin(), CorePartitions[i].end(),
		     inserter(NewPartition, NewPartition.begin()));
                    }
                }// end for
                
                vector< set<int> > NewCorePartitions; 
                // NewCorePartitions = NewPartition \union all existing partitions not intesecting with \beta
                
                NewCorePartitions.push_back(NewPartition);
                
                for(int i=0; i<CorePartitions.size(); i++)
                {
                    if(PartitionsIntersectingNewUnsatcore.find(i) == PartitionsIntersectingNewUnsatcore.end()) // i is not a partition
                        // intersecting with \beta
                    {
                        NewCorePartitions.push_back(CorePartitions[i]);
                    }
                }// end for
                
                CorePartitions = NewCorePartitions;
            }// end of else of \beta intersects with more than one \pi_i
            
        }// end of  if(BetaInterAlpha.empty()) 
        else
        {
           set<int> PartitionsIntersectingNewUnsatcore;
            findPartitionsIntersectingNewUnsatcore(CorePartitions, CoreConstraintIndicesInLoop, PartitionsIntersectingNewUnsatcore);
            
            if(PartitionsIntersectingNewUnsatcore.empty())// \beta does not intersect with any \pi_i. Then why \neg ( BetaMinusAlpha.empty()))
            {
                cout<<"\nInfeasibe partition arrived in function refinePartition in DAGEliminator.cpp\n";
                exit(1);
            }
            else // \beta intersects with one or more \pi_i
            {
                set<int> NewPartition; // NewPartition = union of all these intersecting partitions and \beta
                for(int i=0; i<CorePartitions.size(); i++)
                {
                    if(PartitionsIntersectingNewUnsatcore.find(i) != PartitionsIntersectingNewUnsatcore.end()) // i is a partition
                        // intersecting with \beta
                    {
                        set_union(NewPartition.begin(), NewPartition.end(), CorePartitions[i].begin(), CorePartitions[i].end(),
		     inserter(NewPartition, NewPartition.begin()));
                    }
                }// end for
                 set_union(NewPartition.begin(), NewPartition.end(), CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(),
		     inserter(NewPartition, NewPartition.begin()));
                
                vector< set<int> > NewCorePartitions; 
                // NewCorePartitions = NewPartition \union all existing partitions not intesecting with \beta
                
                NewCorePartitions.push_back(NewPartition);
                
                for(int i=0; i<CorePartitions.size(); i++)
                {
                    if(PartitionsIntersectingNewUnsatcore.find(i) == PartitionsIntersectingNewUnsatcore.end()) // i is not a partition
                        // intersecting with \beta
                    {
                        NewCorePartitions.push_back(CorePartitions[i]);
                    }
                }// end for
                
            CorePartitions = NewCorePartitions; 
            
            set<int> AlphaMinusBeta;
            set_difference(RemainingPartition.begin(), RemainingPartition.end(), CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), 
		     inserter(AlphaMinusBeta, AlphaMinusBeta.begin()));
            RemainingPartition = AlphaMinusBeta;
        }// end of \beta intersects with one or more \pi_i
            
    }// end of else of if(BetaInterAlpha.empty())
   
}// end of else of if(BetaMinusAlpha.empty())
    
}// end of function

    

// Inter = Indices of elements in \pi such that \pi[i] \intersect \Beta \equiv \nonempty    
void findPartitionsIntersectingNewUnsatcore(vector< set<int> > &Pi, set<int> &Beta,  set<int> &Inter)
{
for(int i=0; i<Pi.size(); i++)
         {
        set<int> Pi_i = Pi[i];
        set<int> Pi_i_Inter_Beta;
        set_intersection(Pi_i.begin(), Pi_i.end(), Beta.begin(), Beta.end(), 
                 inserter(Pi_i_Inter_Beta, Pi_i_Inter_Beta.begin()));

        if(!Pi_i_Inter_Beta.empty()) // \pi[i] \intersect \Beta \equiv \nonempty  
            {
            Inter.insert(i);    
            }

        }// end for
}


// Function to refine the existing partition based on the new unsat. core, where the partitions can be intersecting
void refineIntersectingPartition(vector< set<int> > &CorePartitions, set<int> &RemainingPartition, set<int> &CoreConstraintIndicesInLoop)
{
    // Let us call CorePartitions as \pi, RemainingPartition as \alpha, and CoreConstraintIndicesInLoop as \beta
    set<int> BetaMinusAlpha;
    set_difference(CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), RemainingPartition.begin(), RemainingPartition.end(),
		     inserter(BetaMinusAlpha, BetaMinusAlpha.begin()));
    
    if(BetaMinusAlpha.empty()) // \beta subset of \alpha
    {
        CorePartitions.push_back(CoreConstraintIndicesInLoop);// \pi = \pi \union \beta
        set<int> AlphaMinusBeta;
        set_difference(RemainingPartition.begin(), RemainingPartition.end(), CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), 
		     inserter(AlphaMinusBeta, AlphaMinusBeta.begin()));
        RemainingPartition = AlphaMinusBeta;// \alpha = \alpha \minus \beta
    }
    else
    {
        set<int> BetaInterAlpha;
        set_intersection(CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), RemainingPartition.begin(), RemainingPartition.end(),
		     inserter(BetaInterAlpha, BetaInterAlpha.begin()));
        
        if(BetaInterAlpha.empty()) 
        {
            set<int> PartitionsIntersectingNewUnsatcore;
            findPartitionsIntersectingNewUnsatcore(CorePartitions, CoreConstraintIndicesInLoop, PartitionsIntersectingNewUnsatcore);
            
            if(PartitionsIntersectingNewUnsatcore.empty())// \beta does not intersect with any \pi_i. Then why \neg ( BetaMinusAlpha.empty()))
            {
                cout<<"\nInfeasibe partition arrived in function refinePartition in DAGEliminator.cpp\n";
                exit(1);
            }
            else if(PartitionsIntersectingNewUnsatcore.size()==1) // \beta intersects with exactly one \pi_i
            {
                // do nothing
                return;
            }
            else // \beta intersects with more than one \pi_i
            {
                CorePartitions.push_back(CoreConstraintIndicesInLoop);// \pi = \pi \union \beta              
                
            }// end of else of \beta intersects with more than one \pi_i
            
        }// end of  if(BetaInterAlpha.empty()) 
        else
        {
           set<int> PartitionsIntersectingNewUnsatcore;
            findPartitionsIntersectingNewUnsatcore(CorePartitions, CoreConstraintIndicesInLoop, PartitionsIntersectingNewUnsatcore);
            
            if(PartitionsIntersectingNewUnsatcore.empty())// \beta does not intersect with any \pi_i. Then why \neg ( BetaMinusAlpha.empty()))
            {
                cout<<"\nInfeasibe partition arrived in function refinePartition in DAGEliminator.cpp\n";
                exit(1);
            }
            else // \beta intersects with one or more \pi_i
            {
            CorePartitions.push_back(CoreConstraintIndicesInLoop);// \pi = \pi \union \beta     
            
            set<int> AlphaMinusBeta;
            set_difference(RemainingPartition.begin(), RemainingPartition.end(), CoreConstraintIndicesInLoop.begin(), CoreConstraintIndicesInLoop.end(), 
		     inserter(AlphaMinusBeta, AlphaMinusBeta.begin()));
            RemainingPartition = AlphaMinusBeta;// \alpha = \alpha \minus \beta
        }// end of \beta intersects with one or more \pi_i
            
    }// end of else of if(BetaInterAlpha.empty())
   
}// end of else of if(BetaMinusAlpha.empty())
    
}// end of function


// Try to find another unsat. core
void tryToFindUnsatCore(set<DAGNode*> &Constraints, map<string, string> &YMap, map<string, int> &WidthTable, set<int> &CoreConstraintIndices, set<int> &OtherConstraintIndices, map<DAGNode*, int> &ReverseConstraintMap, bool check_for_proper_partition, bool cross_check_unsat_core, string solver)
{
    
    // We have /\ Constraints
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* ThisConstraint = *it;
        set<DAGNode*> ConstraintToRemove;
        ConstraintToRemove.insert(ThisConstraint);
        set<DAGNode*> ReducedConstraints;
        set_difference(Constraints.begin(), Constraints.end(), ConstraintToRemove.begin(), ConstraintToRemove.end(),
		     inserter(ReducedConstraints, ReducedConstraints.begin()));
    
        DAGNode* Phi = getConjunctionFromSet(ReducedConstraints);
        DAGNode* PhiPrime = cloneUsingValueMap(Phi, YMap);
        //cout<<"\nPhiPrime = "<<getDAGStr(PhiPrime)<<endl;
        
        // Let us check if PhiPrime is unsat. If yes, proceed. Else start another iteration of the loop
        
        if(!ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(PhiPrime, WidthTable)) // PhiPrime is sat
        {
           //cout<<"\nRemoving "<<getDAGStr(ThisConstraint)<<" from Constraints makes it sat under the given YMap\n";
           //cout<<"\nPress any key to remove next constraint and see\n";
           //char c = getchar();
           continue;
        }
        
        // PhiPrime is unsat
        set<DAGNode*> CoreConstraints;
        
        
         if(solver=="zchaff")
        {
           blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAG(PhiPrime, WidthTable, CoreConstraints); 
        }
        else if(solver=="picosat")
        {
            blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAGUsingPicoSAT(PhiPrime, WidthTable, CoreConstraints); 
        }
        else
        {
            cout<<"\nUnknown solver "<<solver<<" inside function semanticScopeReduction in DAGEliminator.cpp\n";
            exit(1);
        }
        
        //showAPSet("CoreConstraints", CoreConstraints);
        
        if(cross_check_unsat_core)
            {
            DAGNode* temp = getConjunctionFromSet(CoreConstraints);
            if(!ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(temp, WidthTable))
                {
                cout<<"\nUnsat core incorrect inside function tryToFindUnsatCore in DAGEliminator.cpp\n";
                exit(1);
                }
            else
                {
                cout<<"\nUnsat core correct. Press any key to continue";
                //char c=getchar();               
                }
            }
        
    CoreConstraintIndices.clear();
    OtherConstraintIndices.clear();

    getCoreConstraintIndices(ReverseConstraintMap, CoreConstraints, CoreConstraintIndices, OtherConstraintIndices, YMap);

    if(check_for_proper_partition)
        {
        if(OtherConstraintIndices.empty())// Unsat core has the entire \phi
            {
            //cout<<"\nUnsat core includes the entire Phi in function tryToFindUnsatCore in DAGEliminator.cpp\n";
            //cout<<"\nLet us try to find another unsat core. Press any key to continue";
            //char c = getchar(); 
            continue;
            }
        else
            {
            //cout<<"\nProper unsat core obtained\n";
            break;
            }
        }// if(check_for_proper_partition)
    }// for loop ends here
    
    if(OtherConstraintIndices.empty())// Unsat core has the entire \phi
        {
        //cout<<"\nAll unsat cores under the given YMap have the entire Phi\n";
        return;     
        }
}
    

// Check simplified version of z3 constraint
// Sets loop_entry_str properly; returns changed PreventedYMaps
DAGNode* checkSimplifiedZ3Constraint(DAGNode* NegPhi, DAGNode* PreventedYMaps, map<string, int> &WidthTable, map<string, string> &YMap, vector< set<int> > &CorePartitions, set<int> &RemainingPartition, map<int, DAGNode*> &ConstraintMap, set<DAGNode*> &Constraints, set<string> &VarsToElim,  set<string> &VarsToRemain, string &loop_entry_str)
{
    
    while(true)
    {
      // First check if \forall x. NegPhi /\ PreventedYMaps is sat. If no, set loop_entry = false and return. Else get the YMap
        DAGNode* Condition = createDAG("and", NegPhi,  PreventedYMaps, LOGICAL_SYMBOL, 1);
        
        //cout<<"\nCondition given to z3 = "<<getDAGStr(Condition)<<endl;
        
        YMap.clear();       
        
        string condition_result = getModelForQBVF_UsingZ3_WithSupportForLMIs(Condition, YMap, WidthTable, VarsToElim, VarsToRemain);
        
        if(condition_result == "unsat") // \forall x. NegPhi /\ PreventedYMaps is unsat
        {
            cout<<"\nforall x. NegPhi & PreventedYMaps is unsat in function checkSimplifiedZ3Constraint in DAGEliminator.cpp\n";
            cout<<"\nPress any key to continue\n";
            //char c = getchar();
            loop_entry_str = condition_result;
            return PreventedYMaps;
        }
        else if(condition_result == "unknown") // \forall x. NegPhi /\ PreventedYMaps is unknown
        {
            cout<<"\nforall x. NegPhi & PreventedYMaps is unknown in function checkSimplifiedZ3Constraint in DAGEliminator.cpp\n";
            cout<<"\nPress any key to continue\n";
            //char c = getchar();
            loop_entry_str = condition_result;
            return PreventedYMaps;
        }
        else // \forall x. NegPhi /\ PreventedYMaps is sat. Model in YMap
        {
            // Check if /\ Partition_i under YMap is sat. If all are sat exit from loop. else continue the loop
            
            bool remaining_partition_can_be_empty = true;
            
            int partition_index=1;
    
            //cout<<"\nRemaining Partition\n";
    
            set<DAGNode*> ConstraintsFromRemainingPartition;
            getConstraintsFromParticularPartition(RemainingPartition, ConstraintMap, ConstraintsFromRemainingPartition);
            DAGNode* Conjunction = getConjunctionFromSet(ConstraintsFromRemainingPartition);

   
            if(!remaining_partition_can_be_empty)
                {
                if(isTrue(Conjunction))
                    {
                    cout<<"Empty partition in function checkSimplifiedZ3Constraint in DAGEliminator.cpp\n";
                    exit(1);
                    }
                }  
  
            DAGNode* ClonedConjunction = cloneAndShiftWithWidthTableUpdate(Conjunction, partition_index, VarsToElim, WidthTable);
            //cout<<"\nCloned Conjunction "<<getDAGStr(ClonedConjunction)<<endl;
            
            DAGNode* ClonedConjunctionPrime = cloneUsingValueMap(ClonedConjunction, YMap);
            //cout<<"\nCloned Conjunction Prime "<<getDAGStr(ClonedConjunctionPrime)<<endl;
             
             // check if ClonedConjunctionPrime is sat
            
            if(ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(ClonedConjunctionPrime, WidthTable)) // ClonedConjunctionPrime is unsat
                {
                // repeat the loop to find another YMap. but modify the PreventedMaps
                cout<<"\nGiven YMap makes the remaining partition unsat. Let us find another YMap\n";
                //cout<<"\nPress any key to continue\n";
                //char c = getchar();
                
                DAGNode *YMapDAG = createDAGFromYMap(YMap, WidthTable); // dag is like (y0=v0) /\ .... /\ (yn=vn)
                PreventedYMaps =  createDAG("and", PreventedYMaps, createDAG("not", YMapDAG, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1); // It is like PreventedYMaps /\ ~( (y0=v0) /\ .... /\ (yn=vn) )
                YMap.clear();
                continue;
                }

           partition_index++;  
           
           bool find_another_ymap = false;   
    
            for(vector< set<int> >::iterator it=CorePartitions.begin(); it != CorePartitions.end(); it++)
                {
                //cout<<"\nPartition "<<partition_index<<endl;
        
                set<int> ThisPartition = *it;
                set<DAGNode*> ConstraintsFromThisPartition;
                getConstraintsFromParticularPartition(ThisPartition, ConstraintMap, ConstraintsFromThisPartition);
                Conjunction = getConjunctionFromSet(ConstraintsFromThisPartition);
       
                if(isTrue(Conjunction))
                    {
                        cout<<"Empty partition in function createConstraintFromPartition in DAGEliminator.cpp\n";
                        exit(1);
                    }

                ClonedConjunction = cloneAndShiftWithWidthTableUpdate(Conjunction, partition_index, VarsToElim, WidthTable);
       
                //cout<<"\nCloned Conjunction "<<getDAGStr(ClonedConjunction)<<endl;
                
                DAGNode* ClonedConjunctionPrime = cloneUsingValueMap(ClonedConjunction, YMap);
                
                //cout<<"\nCloned Conjunction Prime"<<getDAGStr(ClonedConjunctionPrime)<<endl;
             
                 // check if ClonedConjunctionPrime is sat
            
                if(ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(ClonedConjunctionPrime, WidthTable)) // ClonedConjunctionPrime is unsat
                    {
                    // repeat the loop, but modify the PreventedMaps
                    cout<<"\nGiven YMap makes the partition "<<partition_index<<" unsat. Let us find another YMap\n";
                    //cout<<"\nPress any key to continue\n";
                    //char c = getchar();
                
                    find_another_ymap = true;
                    
                    DAGNode *YMapDAG = createDAGFromYMap(YMap, WidthTable); // dag is like (y0=v0) /\ .... /\ (yn=vn)
                    PreventedYMaps =  createDAG("and", PreventedYMaps, createDAG("not", YMapDAG, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1); // It is like PreventedYMaps /\ ~( (y0=v0) /\ .... /\ (yn=vn) )
                    YMap.clear();
                    
                    break; // go out of for loop
                    }
               
                partition_index++;        
             }// for loop ends here
           
           if(find_another_ymap)
           {
               continue; // start again another iteration of while(true)
           }
    
           else
           {
               cout<<"\nGiven YMap makes all the partitions sat. Let us continue with this YMap\n";
               cout<<"\nPress any key to continue\n";
               //char c = getchar();
               
               loop_entry_str = "sat";
               return PreventedYMaps;
           }
           
            
        } // \forall x. NegPhi /\ PreventedYMaps is sat. Model in YMap
    }// while(true) ends here   
        
}// Function ends here
           


// Given YMap signal ---> value and widthtable, create a dag (signal0=value0) /\ .... /\ (signaln=valuen)
DAGNode* createDAGFromYMap(map<string, string> &YMap, map<string, int> &WidthTable)
{
    DAGNode* Condition = NULL;
    
    for(map<string, string>::iterator it = YMap.begin(); it != YMap.end(); it++)
    {
        string signal = it->first;
                
        string value_width = it->second;
        int width_start_pos = value_width.find("[");
        string value  = value_width.substr(0, width_start_pos);
        
        map<string, int>::iterator wit = WidthTable.find(signal);
        if(wit == WidthTable.end())
        {
            cout<<"\nNo entry for the signal "<<signal<<" in WidthTable in function createDAGFromYMap in DAGEliminator.cpp\n";
            exit(1);
        }
        int width = wit->second;
        
        DAGNode *lhs = createBVVariable(signal, width);
        DAGNode *rhs = createBVConstant(value, width);
        
        DAGNode* eqn = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);
        
	if(Condition==NULL) 
        {
            Condition=eqn;             
        }
        else
        {
          Condition = createDAG("and", Condition, eqn, LOGICAL_SYMBOL, 1);
        }
    }// for loop ends here
    
    return Condition;
        
}// function ends here
    



// Call syntactic scope reduction
// Here Phi = /\ of constraints in Constraints
void syntacticScopeReduction(set<DAGNode*> &Constraints,  set<string> &VarsToElim, map<string, int> &WidthTable)
     {
      showSet(VarsToElim,"VarsToElim");
      showAPSet("Constraints", Constraints);
      
      map<DAGNode*, set<string> > Constraint_VarsToElim_Map;
      map<string, set<DAGNode*> > VarToElim_Constraints_Map;
      map<set<DAGNode*>, set<string> > Constraints_VarsToElim_Map;

      // Create Constraint_VarsToElim_Map
      for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
        {
         DAGNode *Constraint = *it;
         set<string> Support;
         getDAGNames(Constraint, Support); // Support of Constraint obtained
         set<string> VarsToElimInSupport;
         set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));

         Constraint_VarsToElim_Map.insert(make_pair(Constraint, VarsToElimInSupport));
        }

      // Showing Constraint_VarsToElim_Map

     //  cout<<"\nConstraint_VarsToElim_Map\n";

     //  for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
     //    {
     //     DAGNode *Constraint = it->first;
     //     set<string> Support = it->second;
     //     cout<<"\nConstraint = "<<getDAGStr(Constraint)<<"\t";
     //    showSet(Support, "Support");
     //   }

      // Create VarToElim_Constraints_Map

      bool Constraints_Free_Of_VarsToElim = true;

      for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
        {
         DAGNode *Constraint = it->first;
         set<string> Support = it->second;

         if(!Support.empty())
            {
            Constraints_Free_Of_VarsToElim = false;
            }

         for(set<string>::iterator it2=Support.begin(); it2!=Support.end(); it2++)
            {
            string Variable = *it2;
            map<string, set<DAGNode*> >::iterator vcm_it = VarToElim_Constraints_Map.find(Variable);
            if(vcm_it != VarToElim_Constraints_Map.end())
                    {
                    (vcm_it->second).insert(Constraint);
                    }
            else
                    {
                    set<DAGNode*> Constraints;
                    Constraints.insert(Constraint);
                    VarToElim_Constraints_Map.insert(make_pair(Variable, Constraints));
                    }
            } // for ends here 
         }// for ends here

      if(Constraints_Free_Of_VarsToElim) // All constraints free of VarsToElim
            {
             cout<<"\nAll constraints free of variables to be eliminated\n";
            return;
            }

      // Showing VarToElim_Constraints_Map

      //cout<<"\nVarToElim_Constraints_Map\n";

      // for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
      //  {
      //    string VarToElim = it->first;
      //    set<DAGNode*> Constraints = it->second;
      //    cout<<"\nVarToElim = "<<VarToElim<<"\t";
      //    showAPSet("Constraints", Constraints);
      //  }

      // Create Constraints_VarsToElim_Map

      for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
        {
         string VarToElim = it->first;
         set<DAGNode*> Constraints = it->second;
         map<set<DAGNode*>, set<string> >::iterator cvm_it = Constraints_VarsToElim_Map.find(Constraints);
         if(cvm_it != Constraints_VarsToElim_Map.end())
            {
            (cvm_it->second).insert(VarToElim);
            }
        else	
            {
            set<string> Support;
            Support.insert(VarToElim);
            Constraints_VarsToElim_Map.insert(make_pair(Constraints, Support));
            }
        }

      // Showing Constraints_VarsToElim_Map

      //cout<<"\nConstraints_VarsToElim_Map\n";

      //for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
      //   {
      //    set<DAGNode*> Constraints = it->first;
      //    set<string>  VarsToElimInConstraints = it->second;
     //     showAPSet("Constraints", Constraints);
     //    showSet(VarsToElimInConstraints, "VarsToElimInConstraints");
     // }
        
        
   map<set<DAGNode*>, set<string> > Constraints_VarsToElim_Map_Final;
   map<set<DAGNode*>, set<string> > Constraints_VarsToElim_Map_Temp;
   
   for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
         {
          set<DAGNode*> Constraints = it->first;
          set<string>  VarsToElimInConstraints = it->second;
          
          if(Constraints.size()==1) // moving entries with size(LHS)==1 to the final map
          {
              Constraints_VarsToElim_Map_Final.insert(make_pair(Constraints, VarsToElimInConstraints));
          }
          else // We will look at the entries with size(LHS) > 1
          {
              Constraints_VarsToElim_Map_Temp.insert(make_pair(Constraints, VarsToElimInConstraints));
          }
      }
   
   
   //showConstraintsVarsToElimMap("Constraints_VarsToElim_Map_Final", Constraints_VarsToElim_Map_Final);
   
   //showConstraintsVarsToElimMap("Constraints_VarsToElim_Map_Temp", Constraints_VarsToElim_Map_Temp);
   
    
   while(Constraints_VarsToElim_Map_Temp.size() > 1)
   {
       
       //showConstraintsVarsToElimMap("Constraints_VarsToElim_Map_Temp", Constraints_VarsToElim_Map_Temp);
       
       map<set<DAGNode*>, set<string> >::iterator minlhs_it = findEntryWithMinLHS(Constraints_VarsToElim_Map_Temp);
       
       //cout<<"\nMininum set is\n";
       //set<DAGNode*> TempSet = minlhs_it->first;
       //showAPSet("Constraints", TempSet);
       //showSet(minlhs_it->second, "VarsToElimInConstraints");
       
       Constraints_VarsToElim_Map_Final.insert(make_pair(minlhs_it->first, minlhs_it->second));
       
       recreateConstraints_VarsToElim_Map(Constraints_VarsToElim_Map_Temp, minlhs_it);
   }
   
   
   // Copy the last entry of Constraints_VarsToElim_Map_Temp into Constraints_VarsToElim_Map_Final
   
   map<set<DAGNode*>, set<string> >::iterator it = Constraints_VarsToElim_Map_Temp.begin();
   Constraints_VarsToElim_Map_Final.insert(make_pair(it->first, it->second));
   
   
   showConstraintsVarsToElimMap("Constraints_VarsToElim_Map_Final", Constraints_VarsToElim_Map_Final);
   
   //showConstraintsVarsToElimMap("Constraints_VarsToElim_Map_Temp", Constraints_VarsToElim_Map_Temp);
   
   
   // Let us just observe the result of giving the leaf level constraints to SemanticScopeReduction
   // Note that, we are not reading back the results
   
      

    for(map<set<DAGNode*>, set<string> >::iterator it = Constraints_VarsToElim_Map_Final.begin(); it != Constraints_VarsToElim_Map_Final.end(); it++)
     {
        set<DAGNode*> MyConstraints = it->first;
        set<string> MyVarsToElim = it->second;


        showAPSet("MyConstraints", MyConstraints); 
        showSet(MyVarsToElim, "MyVarsToElim"); 

        if(MyConstraints.size()==1)// semanticScopeReduction can be applied
        {
            cout<<"\nSemantic Scope Reduction can be applied as the conjunction is at the leaf level\n";
            cout<<"\nPress any key to continue\n";
            //char c=getchar();

            DAGNode* Conjunction_MyConstraints = getConjunctionFromSet(MyConstraints);

            set< set<DAGNode*> > ScopeReduced_MyConstraints;
            bool result_trustworthy =   semanticScopeReduction(Conjunction_MyConstraints, MyConstraints, MyVarsToElim, WidthTable, ScopeReduced_MyConstraints);

            if(!result_trustworthy)
              {
                  cout<<"\nz3 timed out during computation. Result is cannot be trusted\n";                          
              }
              else
              {
                  cout<<"\nScopeReduced_MyConstraints\n";
                  for(set< set<DAGNode*> >::iterator sdagit = ScopeReduced_MyConstraints.begin(); sdagit != ScopeReduced_MyConstraints.end(); sdagit++)
                  {
                    set<DAGNode*> MyDAGSet = *sdagit;
                    showAPSet("ScopeReduced_MyConstraint", MyDAGSet); 
                    }//for
              }// else

              cout<<"\nPress any key to continue\n";
              //c=getchar();
        }// if(MyConstraints.size()==1)
        else if(!properSubsetOfMyConstraintsExistsInMap(MyConstraints, Constraints_VarsToElim_Map_Final))// semanticScopeReduction can be applied
        {
            cout<<"\nSemantic Scope Reduction can be applied as the conjunction is at the leaf level\n";
            cout<<"\nPress any key to continue\n";
            //char c=getchar();

            DAGNode* Conjunction_MyConstraints = getConjunctionFromSet(MyConstraints);

            set< set<DAGNode*> > ScopeReduced_MyConstraints;
            bool result_trustworthy =   semanticScopeReduction(Conjunction_MyConstraints, MyConstraints, MyVarsToElim, WidthTable, ScopeReduced_MyConstraints);

            if(!result_trustworthy)
              {
                  cout<<"\nz3 timed out during computation. Result is cannot be trusted\n";                          
              }
            else
              {
                  cout<<"\nScopeReduced_MyConstraints\n";
                  for(set< set<DAGNode*> >::iterator sdagit = ScopeReduced_MyConstraints.begin(); sdagit != ScopeReduced_MyConstraints.end(); sdagit++)
                  {
                    set<DAGNode*> MyDAGSet = *sdagit;
                    showAPSet("ScopeReduced_MyConstraint", MyDAGSet);                             
                  }//for
              }// else

              cout<<"\nPress any key to continue\n";
              //c=getchar();
        }//  else if(!properSubsetOfMyConstraintsExistsInMap(MyConstraints, Constraints_VarsToElim_Map_Final))
        else// semantic scope reduction cannot be applied
        {
            cout<<"\nSemantic Scope Reduction cannot be applied as the conjunction is not at the leaf level\n";
            cout<<"\nPress any key to continue\n";
            //char c=getchar();
        }

    }// for ends here
            
   
 }



// refine ConstraintsVarsToElimMap
void refineConstraintsVarsToElimMap(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map,  map<set<DAGNode*>, set<string> >::iterator remove_it)
{
    map<set<DAGNode*>, set<string> > New_Constraints_VarsToElim_Map;
    
     for(map<set<DAGNode*>, set<string> >::iterator it = Constraints_VarsToElim_Map.begin(); it != Constraints_VarsToElim_Map.end(); it++)
     {
         if(it != remove_it)
         {
             set<DAGNode*> RemoveLHS = remove_it->first;
             set<DAGNode*> LHS = it->first;
             
             if(checkIntersecting(RemoveLHS, LHS)) // remove RemoveLHS
             {
                 //cout<<"\nIntersecting\n";
                 set<DAGNode*> NewConstraints;
                 set_difference(LHS.begin(), LHS.end(), RemoveLHS.begin(),  RemoveLHS.end(),inserter(NewConstraints, NewConstraints.begin()));
                 
                 // First check if there's already an entry for NewConstraints
                 map<set<DAGNode*>, set<string> >::iterator check_it = New_Constraints_VarsToElim_Map.find(NewConstraints);
                 if(check_it == New_Constraints_VarsToElim_Map.end())// No
                 {
                  New_Constraints_VarsToElim_Map.insert(make_pair(NewConstraints, it->second));   
                 }
                 else
                 {
                     set<string> NewVarsToElim;
                     set<string> Existing1 = it->second;
                     set<string> Existing2 = check_it->second;
                     
                     set_union(Existing1.begin(), Existing1.end(), Existing2.begin(),  Existing2.end(),inserter(NewVarsToElim, NewVarsToElim.begin()));
                     New_Constraints_VarsToElim_Map[NewConstraints] = NewVarsToElim;
                 }
                 
             }// merge
             else // just insert into the new map
             {
                 //cout<<"\nNon-Intersecting\n";
                 set<DAGNode*> NewConstraints = LHS;
                  // First check if there's already an entry for NewConstraints
                 map<set<DAGNode*>, set<string> >::iterator check_it = New_Constraints_VarsToElim_Map.find(NewConstraints);
                 if(check_it == New_Constraints_VarsToElim_Map.end())// No
                 {
                  New_Constraints_VarsToElim_Map.insert(make_pair(NewConstraints, it->second));   
                 }
                 else
                 {
                     set<string> NewVarsToElim;
                     set<string> Existing1 = it->second;
                     set<string> Existing2 = check_it->second;
                     
                     set_union(Existing1.begin(), Existing1.end(), Existing2.begin(),  Existing2.end(),inserter(NewVarsToElim, NewVarsToElim.begin()));
                     New_Constraints_VarsToElim_Map[NewConstraints] = NewVarsToElim;
                 }// else                 
             }// just insert into the new map
         }// if
     }// for
    
   Constraints_VarsToElim_Map = New_Constraints_VarsToElim_Map;  
}



// Return location in Constraints_VarsToElim map with minimum sized LHS
map<set<DAGNode*>, set<string> >::iterator findEntryWithMinLHS(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map)
{
    map<set<DAGNode*>, set<string> >::iterator my_location = Constraints_VarsToElim_Map.begin();
        
     int minsize = (my_location->first).size();
     
     for(map<set<DAGNode*>, set<string> >::iterator it = Constraints_VarsToElim_Map.begin(); it != Constraints_VarsToElim_Map.end(); it++)
     {
         if( minsize >  (it->first).size() )
         {
             my_location = it;
             minsize = (it->first).size();
         }
     }
     
     return my_location;
}


// recreate the constraints to vars to elim map
void recreateConstraints_VarsToElim_Map(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map, map<set<DAGNode*>, set<string> >::iterator minlhs_it)
{
    map<set<DAGNode*>, set<string> > New_Constraints_VarsToElim_Map;
    
     for(map<set<DAGNode*>, set<string> >::iterator it = Constraints_VarsToElim_Map.begin(); it != Constraints_VarsToElim_Map.end(); it++)
     {
         if(it != minlhs_it)
         {
             set<DAGNode*> MinLHS = minlhs_it->first;
             set<DAGNode*> LHS = it->first;
             
             if(checkIntersecting(MinLHS, LHS)) // merge
             {
                 //cout<<"\nIntersecting\n";
                 set<DAGNode*> NewConstraints;
                 set_union(MinLHS.begin(), MinLHS.end(), LHS.begin(),  LHS.end(),inserter(NewConstraints, NewConstraints.begin()));
                 New_Constraints_VarsToElim_Map.insert(make_pair(NewConstraints, it->second));
             }// merge
             else // just insert into the new map
             {
                 //cout<<"\nNon-Intersecting\n";
                 New_Constraints_VarsToElim_Map.insert(make_pair(it->first, it->second));
             }// just insert into the new map
         }// if
     }// for
    
   Constraints_VarsToElim_Map = New_Constraints_VarsToElim_Map; 
    
}// eof



// returns true if Set1 is intersecting with Set2; false otherwise
bool checkIntersecting(set<DAGNode*> &Set1, set<DAGNode*> &Set2)
{
 set<DAGNode*> InterSet;
 set_intersection(Set1.begin(), Set1.end(), Set2.begin(), Set2.end(),inserter(InterSet, InterSet.begin()));
 if(!InterSet.empty()) return true;
 return false;
}


// show the ConstraintsVarsToElimMap
void showConstraintsVarsToElimMap(string WhoAmI, map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map)
{
      cout<<"\n\n";

      cout<<WhoAmI<<"\n**********\n\n";

      for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
         {
          set<DAGNode*> Constraints = it->first;
          set<string>  VarsToElimInConstraints = it->second;
          showAPSet("Constraints", Constraints);
          showSet(VarsToElimInConstraints, "VarsToElimInConstraints");
      }
}


// create the partitioned conjunction as per the partitions
void createPartitionedConjunction(map<int, DAGNode*> &ConstraintMap, vector< set<int> > &CorePartitions, set<int> &RemainingPartition, set< set<DAGNode*> > &PartitionedConjunction)
{
    set<DAGNode*> ConstraintsFromThisPartition;
     
    getConstraintsFromParticularPartition(RemainingPartition,  ConstraintMap,  ConstraintsFromThisPartition);
    PartitionedConjunction.insert(ConstraintsFromThisPartition);
    
    for(int i=0; i<CorePartitions.size(); i++)
    {
      ConstraintsFromThisPartition.clear();
      set<int> MyPartition = CorePartitions[i];
      
     getConstraintsFromParticularPartition(MyPartition,  ConstraintMap,  ConstraintsFromThisPartition);
     PartitionedConjunction.insert(ConstraintsFromThisPartition);  
    }
    
}


// return true if there exists a proper subset of MyConstraints in the LHS of Constraints_VarsToElim_Map_Final
bool properSubsetOfMyConstraintsExistsInMap(set<DAGNode*> &MyConstraints,  map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map)
{
 for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
         {
          set<DAGNode*> Constraints = it->first;
                    
          if(isProperSubset(Constraints, MyConstraints))
            {
              return true;
            }// if          
        }// for
 return false;
}


// returns true if Set1 is a proper subset of Set2; false otherwise
// Set1 is a proper subset of Set2 <=> (Set1 \ Set2 = empty set) and (Set2 \ Set1 != empty set)
bool isProperSubset(set<DAGNode*> &Set1, set<DAGNode*> &Set2)
{
 set<DAGNode*> Set1DiffSet2;
 set<DAGNode*> Set2DiffSet1;
 
 set_difference(Set1.begin(), Set1.end(), Set2.begin(), Set2.end(),inserter(Set1DiffSet2, Set1DiffSet2.begin()));
 set_difference(Set2.begin(), Set2.end(), Set1.begin(), Set1.end(),inserter(Set2DiffSet1, Set2DiffSet1.begin()));
 
 if(Set1DiffSet2.empty() && !Set2DiffSet1.empty()) return true;
 return false;
}





// Given a set of LMIs A and vars to elim X, it tries to 1) synthesize LMEs from the LMIs, and compute exists X. A
// 2) Drop LMIs using the dropping idea
DAGNode* LMEBasedSimplificationAndLMIDropping(set<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable, DAGNode* &QuantifiedPart)
{
  struct timeval layer1_start_ms, layer1_finish_ms;
  struct timeval layer2_start_ms, layer2_finish_ms;
  
  
  
    
    
 vector<DAGNode*> BVULE_Vector;// set of constraints in Constraints of the form <=
 vector<DAGNode*> BVUGT_Vector;// Set of constraints in Constraints of the form >
 
 for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
        {
           DAGNode* MyConstraint = *it;
           if(MyConstraint->Get_node_name()=="bvule")
           {
               BVULE_Vector.push_back(MyConstraint);
           }
           else if(MyConstraint->Get_node_name()=="bvugt")
           {
               BVUGT_Vector.push_back(MyConstraint);
           }
           else
           {
             cout<<"\nUnknown predicate "<<MyConstraint->Get_node_name()<<" inside function LMEBasedSimplification in DAGEliminator.cpp\n";
             exit(1);
           }             
           
        }//for ends here
 
 
 //showAPVector("BVULE_Vector", BVULE_Vector);
 //showAPVector("BVUGT_Vector", BVUGT_Vector);
      

set<DAGNode*> AlreadyDone;
AlreadyDone.clear();

vector< pair<DAGNode*, DAGNode*> > SimplifiableLMIPairs;

while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<BVUGT_Vector.size();i++)
	{
          DAGNode* ugt = BVUGT_Vector[i];
          //cout<<"\nTaking LMI "<<getDAGStr(ugt)<<endl;
          if(AlreadyDone.find(ugt)!=AlreadyDone.end()) // We have already checked if there exists a pair for ugt
		{
		//cout<<"\nAlready Done\n";
		continue;
		}
          
          for(int j=0; j<BVULE_Vector.size(); j++)
          {
              DAGNode* ule = BVULE_Vector[j];
              
              if(simplifiableLMIPairFound(ugt, ule))
              {
                  //cout<<"\nMatches with "<<getDAGStr(ule)<<endl;
                  //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
                  SimplifiableLMIPairs.push_back(make_pair(ule, ugt));
                  BVUGT_Vector.erase(BVUGT_Vector.begin()+i);
                  BVULE_Vector.erase(BVULE_Vector.begin()+j);
                  fixpoint_reached=false;
                  break;
              }
          }
	
        if(!fixpoint_reached)// BVUGT_Vector is changed. Restart the loop
        {
            //cout<<"\nStarting from the beginning of BVUGT_Vector\n";
            break;
        }
        
        //cout<<"\nNo matching ule constraint found\n";
	AlreadyDone.insert(ugt);
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here


vector<DAGNode*> LME_Vector;

synthesizeLMEsFromLMIs(SimplifiableLMIPairs, LME_Vector, WidthTable);

//cout<<"\nThe problem we need to solve is\n";

//showSet(VarsToElim, "VarsToElim");
//showAPVector("BVULE_Vector", BVULE_Vector);
//showAPVector("BVUGT_Vector", BVUGT_Vector);
//showAPVector("LME_Vector", LME_Vector);

showAPVector("LMEs synthesized from LMIs", LME_Vector, LMIQEOutputDataFp);

DAGNode *ConjunctionOfLMIs = NULL;

 for(int i=0;i<BVULE_Vector.size();i++)
	{
          DAGNode* ule = BVULE_Vector[i];
          if(ConjunctionOfLMIs == NULL)
          {
              ConjunctionOfLMIs = ule;
          }
          else
          {
             ConjunctionOfLMIs=createDAG("and",  ConjunctionOfLMIs, ule, LOGICAL_SYMBOL, 1); 
          }
     }// for

 for(int i=0;i<BVUGT_Vector.size();i++)
	{
          DAGNode* ugt = BVUGT_Vector[i];
          if(ConjunctionOfLMIs == NULL)
          {
              ConjunctionOfLMIs = ugt;
          }
          else
          {
           ConjunctionOfLMIs=createDAG("and",  ConjunctionOfLMIs, ugt, LOGICAL_SYMBOL, 1);    
          }
     }// for

if(ConjunctionOfLMIs == NULL)
    {
    //cout<<"\nAll LMIs are converted to LMEs by the function LMEBasedSimplification in DAGEliminatir.cpp\n";
    
    fprintf(LMIQEOutputDataFp, "\nAll LMIs are converted to LMEs\n");
    
     set<DAGNode*> LME_Set;
    for(int i=0; i<LME_Vector.size(); i++)
    {
        LME_Set.insert(LME_Vector[i]);
    }
    //showAPSet("LME_Set", LME_Set);
     
     gettimeofday (&layer1_start_ms, NULL); 
     
    DAGNode* QEConjunctionOfLMEs = APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(LME_Set,  VarsToElim,  WidthTable);
    
    
    
    gettimeofday (&layer1_finish_ms, NULL);
    LMILayer1Time = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
    LMILayer1Time -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
    
    
    
    //cout<<"\nQEConjunctionOfLMEs = "<<getDAGStr(QEConjunctionOfLMEs)<<endl;
    //fprintf(LMIQEOutputDataFp, "\nResult of QE = %s\n", (getDAGStr(QEConjunctionOfLMEs)).c_str());
    DAGNode* QEDag = QEConjunctionOfLMEs;
    QuantifiedPart = createLogicalConstant("true");
    
    
    
    return QEDag;
    }
else
    {
    /* We have \exists VarsToElim.(\alpha/\ \beta) here, where \alpha = ConjunctionOfLMIs  and 
     \beta = conjunction of LMEs in LME_Vector  */
    
    //cout<<"\nConjunctionOfLMIs = "<<getDAGStr(ConjunctionOfLMIs)<<endl;
    
    gettimeofday (&layer1_start_ms, NULL); 
    
    /* Simplify \alpha to \alpha' using \beta */
    DAGNode* InlinedConjunctionOfLMIs = inlineLMIConjunctionUsingLMEs(ConjunctionOfLMIs, VarsToElim, LME_Vector, WidthTable);
    
    getNumberOfVarsToElimInLayer2Input(InlinedConjunctionOfLMIs, VarsToElim);
    
    
    //cout<<"\nInlinedConjunctionOfLMIs = "<<getDAGStr(InlinedConjunctionOfLMIs)<<endl;
    //fprintf(LMIQEOutputDataFp, "\nConjunctionOfLMIs after simplification by LMEs = %s\n", (getDAGStr(InlinedConjunctionOfLMIs)).c_str());
    
    vector<DAGNode*> InlinedLMI_Vector;
    splitConjunctionIntoLMIVector(InlinedConjunctionOfLMIs, InlinedLMI_Vector);
    //showAPVector("InlinedLMI_Vector", InlinedLMI_Vector);
    set<DAGNode*> InlinedLMI_Set;
    for(int i=0; i<InlinedLMI_Vector.size(); i++)
    {
        InlinedLMI_Set.insert(InlinedLMI_Vector[i]);
    }
    //showAPSet("InlinedLMI_Set", InlinedLMI_Set);
    
    
    set<DAGNode*> LME_Set;
    for(int i=0; i<LME_Vector.size(); i++)
    {
        LME_Set.insert(LME_Vector[i]);
    }
    //showAPSet("LME_Set", LME_Set);
    
    /* Finding \exists VarsToElim.\beta = \beta' This is always possible since all coefficients in \alpha are odd */
    DAGNode* QEConjunctionOfLMEs = APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(LME_Set,  VarsToElim,  WidthTable);
    //fprintf(LMIQEOutputDataFp, "\nResult of QE from conjunction of LMEs = %s\n", (getDAGStr(QEConjunctionOfLMEs)).c_str());
    
    
    
    gettimeofday (&layer1_finish_ms, NULL);
    LMILayer1Time = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
    LMILayer1Time -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
    
    
    /* Finding \exists VarsToElim.\alpha' = \phi1 \and \exists VarsToElim. \phi2 */
    DAGNode* QEInlinedLMIConjunction=NULL; //\phi1
    DAGNode* QuantifiedInlinedLMIConjunction=NULL;// \phi2
    
    DAGNode* QEDag; // Final QE dag  
    
    //dropLMIsForMultipleVariables(InlinedLMI_Set,  VarsToElim,  WidthTable, QEInlinedLMIConjunction, QuantifiedInlinedLMIConjunction);
    
    gettimeofday (&layer2_start_ms, NULL); 
    
    dropLMIsForMultipleVariablesRecursive(InlinedLMI_Set,  VarsToElim,  WidthTable, QEInlinedLMIConjunction, QuantifiedInlinedLMIConjunction);
    
    
    gettimeofday (&layer2_finish_ms, NULL);
    LMILayer2Time = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
    LMILayer2Time -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;    
    
    
    
    if(isTrue(QuantifiedInlinedLMIConjunction))//\phi2 is true. Hence QE is done
        {
        QEDag = createDAG("and", QEConjunctionOfLMEs, QEInlinedLMIConjunction, LOGICAL_SYMBOL, 1);// result is \beta'/\ \phi1
        QuantifiedPart = QuantifiedInlinedLMIConjunction;
        fprintf(LMIQEOutputDataFp, "\nDropping simplies the part of the problem to true\n");
        return QEDag;
        }
    else
        {
        QEDag = createDAG("and", QEConjunctionOfLMEs, QEInlinedLMIConjunction, LOGICAL_SYMBOL, 1);// result is \beta'/\ \phi1
        QuantifiedPart = QuantifiedInlinedLMIConjunction;
        //cout<<"\nQuantifier-free part = "<<getDAGStr(QEDag)<<endl;
        fprintf(LMIQEOutputDataFp, "\nDropping could not simplify the part of the problem to true\n");
        //fprintf(LMIQEOutputDataFp, "\nWe need to eliminate quantifiers from %s\n", (getDAGStr(QuantifiedInlinedLMIConjunction)).c_str());
         //cout<<"\nWe need to eliminate quantifiers from "<<getDAGStr(QuantifiedInlinedLMIConjunction)<<endl;
        
        
        getNumberOfVarsToElimInLayer3Input(QuantifiedInlinedLMIConjunction, VarsToElim);
        
        
        return QEDag;
        //exit(1);
        }
    }//else
  
}// end of function


// Check if the given pair of LMIs can be simplified into an LME
bool simplifiableLMIPairFound(DAGNode* ugt, DAGNode* ule)
{
    list<DAGNode*>::iterator iout;
    iout = ugt->Get_outlist_start();
    DAGNode *ugt_LHS = *iout;
    iout++;
    DAGNode *ugt_RHS = *iout;	
    
    iout = ule->Get_outlist_start();
    DAGNode *ule_LHS = *iout;
    iout++;
    DAGNode *ule_RHS = *iout;	
    
    if(ugt_LHS != ule_LHS)
    {
        return false;
    }
    
    if(!isConstant(ugt_RHS) || !isConstant(ule_RHS))
    {
        return false;
    }
    
    string ugt_constant_str = ugt_RHS->Get_node_name();
    long long ugt_constant =  strtoull(ugt_constant_str.c_str(), NULL, 0);
    
    string ule_constant_str = ule_RHS->Get_node_name();
    long long ule_constant =  strtoull(ule_constant_str.c_str(), NULL, 0);
    
    if(ugt_constant == ule_constant-1)
    {
        return true;
    }
    
    return false;
}




// Given an "InitialLME" and variable in "VarToElim" convert the "InitialLME" to the form VarToElim = OtherVariable + Constant
// Applicable only for octagons
DAGNode* simplifyLMEToProperForm(DAGNode* InitialLME, map<string, int> &WidthTable, string VarToElim)
{

// Write in matrix form

int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

// Get the list of variables,  NoVars, NoVarsToElim
list<string> VarNames;
set<string> names;

names.clear();
names.insert(VarToElim);
attachSetToList(names, VarNames);

names.clear();
getDAGNames(InitialLME, names);
attachSetToList(names, VarNames);
NoVars=VarNames.size();
NoVarsToElim=1;

set<DAGNode*> Constraints;
Constraints.insert(InitialLME);

// Get the equations and disequations
bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

//showEquations(EqCoefMat, EqConstMat);


if(NoVars != 2)
    {
    cout<<"\nYou are not supposed to call the function simplifyLMEToProperForm in DAGEliminator.cpp on systems other than octagons\n";
    exit(1);
    }

if(EqCoefMat.empty() || EqConstMat.empty())
    {
    cout<<"\nMatrices are empty in the function simplifyLMEToProperForm in DAGEliminator.cpp\n";
    exit(1);
    }

ULINT Coefficient1 = EqCoefMat[0][0];
ULINT Coefficient2 = EqCoefMat[0][1];
ULINT Constant = EqConstMat[0][0];

if(Coefficient1%2==0)
    {
    cout<<"\nCoefficient becomes even in the function simplifyLMEToProperForm in DAGEliminator.cpp. You are not supposed to call this function on systems other than octagons\n";
    exit(1);
    }

if(Coefficient2%2==0)
    {
    cout<<"\nCoefficient becomes even in the function simplifyLMEToProperForm in DAGEliminator.cpp. You are not supposed to call this function on systems other than octagons\n";
    exit(1);
    }

Coefficient2 = aInverse(Coefficient2, ModWidthInteger);

Coefficient2 = mul(Coefficient2, mInverse(Coefficient1, ModWidthInteger), ModWidthInteger);
Constant = mul(Constant, mInverse(Coefficient1, ModWidthInteger), ModWidthInteger);
Coefficient1 = mul(Coefficient1, mInverse(Coefficient1, ModWidthInteger), ModWidthInteger);

//cout<<"\nCoefficient1 = "<<Coefficient1<<" , Coefficient2 = "<<Coefficient2<<" , Constant = "<<Constant<<endl;


// Let us create the simplified DAG now

DAGNode *lhs;
DAGNode *rhs;
DAGNode *term1;
DAGNode *term2;
DAGNode *term3;

list<string>::iterator vit=VarNames.begin();

if(Coefficient1!=0) // no need to add terms with zero coefficients
    {
    string varname = *vit;
    DAGNode *variable = createBVVariable(varname, WidthInteger);



    if(Coefficient1 == 1)
        {
        term1 = variable;
        }
    else
        {
        char coeff[20];
        sprintf(coeff, "%llu", Coefficient1);
        string coeff_str(coeff);
        DAGNode *coefficient = createBVConstant(coeff_str, WidthInteger);
        term1 = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
        }
     }

 vit++;

 if(Coefficient2!=0) // no need to add terms with zero coefficients
    {
    string varname = *vit;
    DAGNode *variable = createBVVariable(varname, WidthInteger);



    if(Coefficient2 == 1)
        {
        term2 = variable;
        }
    else
        {
        char coeff[20];
        sprintf(coeff, "%llu", Coefficient2);
        string coeff_str(coeff);
        DAGNode *coefficient = createBVConstant(coeff_str, WidthInteger);
        term2 = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
        }
     }

   char coeff[20];
   sprintf(coeff, "%llu", Constant);
   string coeff_str(coeff);
   term3 = createBVConstant(coeff_str, WidthInteger);


   lhs=term1;

   if(Constant==0)
    {
       rhs = term2;
    }
   else
    {
       rhs=createDAG("bvadd", term2, term3, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
    }


   DAGNode* SimplifiedLME = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);
   //cout<<"\nSimplifiedLME = "<<getDAGStr(SimplifiedLME)<<endl;

   return SimplifiedLME;
}



// Eliminate quantifiers in ConjunctionOfLMIs using the vector of LMEs LME_Vector
DAGNode* inlineLMIConjunctionUsingLMEs(DAGNode* ConjunctionOfLMIs, set<string> &VarsToElim, vector<DAGNode*> &LME_Vector, map<string, int> &WidthTable)
{
    for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
        string VarToElim = *it;
        
        //cout<<"\nVarToElim = "<<VarToElim<<endl;
        
        DAGNode *LMESelected = NULL; // LME selected from LME_Vector to elminate VarToElim
        int location_LMESelected = -1;
        
        for(int i=0; i<LME_Vector.size(); i++)
        {
            //cout<<"\nTrying LME "<<getDAGStr(LME_Vector[i])<<endl;
            set<string> SupportLME;
            getDAGNames(LME_Vector[i], SupportLME);
            
            if(SupportLME.find(VarToElim) != SupportLME.end())
            {
                LMESelected = LME_Vector[i];
                location_LMESelected = i;
                break;
            }
        }// for ends here
        
        if(LMESelected != NULL)
        {
            //cout<<"\nWe have selected the LME "<<getDAGStr(LMESelected)<<" for eliminating the vaiable "<<VarToElim<<endl;
            DAGNode* LMESelectedInProperForm = simplifyLMEToProperForm(LMESelected, WidthTable, VarToElim);
            
            //cout<<"\nLMESelectedInProperForm = "<<getDAGStr(LMESelectedInProperForm)<<endl;
            
            ULINT Constant_In_LMESelectedInProperForm = 0;
            list<DAGNode*>::iterator LMESelectedInProperForm_iout = LMESelectedInProperForm->Get_outlist_start();
            LMESelectedInProperForm_iout++;
            DAGNode* LMESelectedInProperForm_RHS = *(LMESelectedInProperForm_iout);
            getConstantModified(LMESelectedInProperForm_RHS, Constant_In_LMESelectedInProperForm);
            //cout<<"\nConstant_In_LMESelectedInProperForm = "<<Constant_In_LMESelectedInProperForm<<endl;
            
            if(Constant_In_LMESelectedInProperForm == 0)
                {                    
                ConjunctionOfLMIs = cloneUsingSingleLME(ConjunctionOfLMIs,  VarToElim, LMESelectedInProperForm);
                //cout<<"\nConjunctionOfLMIs after inlining =  "<<getDAGStr(ConjunctionOfLMIs)<<endl;
                }
            else
                {  
                //cout<<"\nConjunctionOfLMIs before inlining =  "<<getDAGStr(ConjunctionOfLMIs)<<endl;
                //cout<<"\nVarToElim = "<<VarToElim<<endl;
                ConjunctionOfLMIs = cloneUsingSingleLMEExpression(ConjunctionOfLMIs,  VarToElim, LMESelectedInProperForm);
                //cout<<"\nConjunctionOfLMIs after inlining =  "<<getDAGStr(ConjunctionOfLMIs)<<endl;
                
                }           
            
        }
        else
        {
            //cout<<"\nWe cannot select any LME for eliminating the variable "<<VarToElim<<endl;
        }        
        
    }// for ends here
    
    return ConjunctionOfLMIs;
}


// Given a dag node "node", variable to eliminate "VarToElim", and LME "VarToElim = RHS",
// replace dag nodes for "VarToElim" by RHS
DAGNode* cloneUsingSingleLME(DAGNode *node,  string VarToElim, DAGNode *LME)
{
    map<DAGNode*, DAGNode*> DPTable;
    return cloneUsingSingleLMEInternal(node, VarToElim, LME, DPTable);
}

// Called from cloneUsingSingleLME
DAGNode* cloneUsingSingleLMEInternal(DAGNode *parent,  string VarToElim, DAGNode *LME,  map<DAGNode*, DAGNode*> &DPTable)
{
    map<DAGNode*, DAGNode*>::iterator dit=DPTable.find(parent);

   if(dit!=DPTable.end()) // Hit in DPTable
    {
    return dit->second;
    }        

    DAGNode* clone;
    // Get the size of outlist of parent
    unsigned int outlist_size = parent->Get_outlist_size();


    if(outlist_size>0)  //i.e. if an operator node
    {
        list<DAGNode*>::iterator iout = parent->Get_outlist_start();

        switch(outlist_size)
        {
            case 1:
            {
                DAGNode* child_1 ;

                child_1 = cloneUsingSingleLMEInternal(*iout, VarToElim, LME, DPTable);

                vector<DAGNode *> Children;
                Children.push_back(child_1);
                clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                break;
            }
            case 2:
            {
                DAGNode* child_1 ;
                DAGNode* child_2 ;

                child_1 = cloneUsingSingleLMEInternal(*iout, VarToElim, LME, DPTable);
                iout++;
                child_2 = cloneUsingSingleLMEInternal(*iout, VarToElim, LME, DPTable);

                vector<DAGNode *> Children;
                Children.push_back(child_1);
                Children.push_back(child_2);
                clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                break;
            }
            case 3:
            {
                DAGNode* child_1 ;
                DAGNode* child_2 ;
                DAGNode* child_3 ;

                child_1 = cloneUsingSingleLMEInternal(*iout, VarToElim, LME, DPTable);
                iout++;
                child_2 = cloneUsingSingleLMEInternal(*iout, VarToElim, LME, DPTable);
                iout++;
                child_3 = cloneUsingSingleLMEInternal(*iout, VarToElim, LME, DPTable);

                vector<DAGNode *> Children;
                Children.push_back(child_1);
                Children.push_back(child_2);
                Children.push_back(child_3);
                clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                break;
            }
          default:
            {
              cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function cloneUsingSingleLMEInternal in Eliminator.cpp\n";
             exit(1);
            }

        }
    }
    else // leaf node
      {
        string id_delta = parent->Get_node_name();
        Value* MyValue = (Value*)(parent->getValue());
        Type MyType = MyValue->get_node_type();
        int MyWidth = MyType.width;
        int MyDataType = MyType.data_type;

        if(id_delta == VarToElim && MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
          {
            // create the clone
            list<DAGNode*>::iterator lme_out = LME->Get_outlist_start();
            DAGNode *LHS = *lme_out;
            lme_out++;
            DAGNode *RHS = *lme_out;	
            clone = RHS;
            }
        else if(id_delta != VarToElim && MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
            {
             clone = dm.createNode(parent->Get_node_name(), parent->getValue());
            }          
        else // constant
          {
            clone = dm.createNode(parent->Get_node_name(), parent->getValue());
          }

      }// leaf node

    DPTable.insert(make_pair(parent, clone));
    return clone;
}


// Given a set of LMIs and a set of variables to be eliminated, this function calls
// dropLMIsForMultipleVariables repeatedly as long as it drops variables
void dropLMIsForMultipleVariablesRecursive(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction)
{
    set<string> OriginalVarsToElim;
    set<string> NewVarsToElim;
    set<DAGNode*> Bound_Constraints;
    DAGNode* QEInlinedLMIConjunctionInLoop;
    DAGNode* QuantifiedInlinedLMIConjunctionInLoop;
    
    
    QEInlinedLMIConjunction = NULL;
    OriginalVarsToElim = VarsToElim;
    Bound_Constraints = Constraints;
    
    
    
    while(true)
    {
        QEInlinedLMIConjunctionInLoop = NULL;
        QuantifiedInlinedLMIConjunctionInLoop = NULL;
        
        dropLMIsForMultipleVariables(Bound_Constraints, OriginalVarsToElim, WidthTable, QEInlinedLMIConjunctionInLoop, QuantifiedInlinedLMIConjunctionInLoop);
        
        if(QEInlinedLMIConjunction==NULL)
            {
            QEInlinedLMIConjunction = QEInlinedLMIConjunctionInLoop;
            }
        else
            {
            QEInlinedLMIConjunction = createDAG("and", QEInlinedLMIConjunctionInLoop, QEInlinedLMIConjunction, LOGICAL_SYMBOL, 1);
            }
        // Compute NewVarsToElim
        if(isTrue(QuantifiedInlinedLMIConjunctionInLoop))// QE is done already
        {
            //cout<<"\nQuantifiedInlinedLMIConjunctionInLoop is True. Hence exiting from loop\n";
            break;
        }
        
        //cout<<"\nQuantifiedInlinedLMIConjunctionInLoop is not True\n";
        set<string> Support;
        getDAGNames(QuantifiedInlinedLMIConjunctionInLoop, Support);
        NewVarsToElim.clear();
        set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(NewVarsToElim, NewVarsToElim.begin()));
        
        //showSet(OriginalVarsToElim, "OriginalVarsToElim");
        //showSet(NewVarsToElim, "NewVarsToElim");
        
        if(setsEqual(NewVarsToElim, OriginalVarsToElim))
        {
            //cout<<"\nOriginalVarsToElim ==  NewVarsToElim\n";
            break;
        }
        
        //cout<<"\nOriginalVarsToElim !=  NewVarsToElim\n";
        OriginalVarsToElim = NewVarsToElim;
        Bound_Constraints.clear();
	splitConjunctionIntoLMISet(QuantifiedInlinedLMIConjunctionInLoop, Bound_Constraints);        
    }// while(true)
    
    QuantifiedInlinedLMIConjunction = QuantifiedInlinedLMIConjunctionInLoop;
    
}// end of function





// Given a set of LMIs and a set of variables to be eliminated, this function applies
// dropping procedure for LMIs on a variable by variable basis.
void dropLMIsForMultipleVariables(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction)
{

  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  

  //showSet(VarsToElim,"VarsToElim");
  //showAPSet("Constraints", Constraints);



  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;

      //cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints);

      QElim_Bound_Constraints.clear();
      
      bool single_lmi_pair_case = false;
      
      if(single_lmi_pair_case)
      {     
          if(Bound_Constraints.size()==1 || Bound_Constraints.size()==2)// dropping is now implemented only for a single LMI pair
            {
              DAGNode* Constraint_LHS=NULL;
              DAGNode* Low=NULL;
              DAGNode* High=NULL;
              if(singleLMIPair(Bound_Constraints, Constraint_LHS, Low, High)) // check if either 1) we have a single LMI 2^k2.c2.x+s \bowtie d or
                  // 2) we have a pair of LMIs 2^k2.c2.x+s \bowtie_1 d1, 2^k2.c2.x+s \bowtie_2 d2 where \bowtie \in {bvule, bvugt},
                  //  \bowtie_1,  \bowtie_2 \in  {bvule, bvugt}, and are different , and d, d_1, d_2 are constants
                {
                  dropSingleLMIPair(Bound_Constraints, Constraint_LHS, Low, High, VariableToElim, QElim_Bound_Constraints);
                }
              else
              {
                  cout<<"\nThe pair of LMIs is not a single LMI pair\n";
                  exit(1);
              }// else
            }// if
          else if(Bound_Constraints.size()>2)
          {
              cout<<"\nDropping is not implemented currently for more than two constraints\n";
              exit(1);
          }      
      }      
      else
      {
          vector< pair<DAGNode*, DAGNode*> > LMIPairs;
          identifyLMIPairs(Bound_Constraints, LMIPairs);
          //showLMIPairs(LMIPairs);
          dropMultipleLMIPairs(Bound_Constraints, LMIPairs,  VariableToElim, QElim_Bound_Constraints); 
      }
      
      //showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      //showAPSet("Constraints", Constraints);
    }// for ends here
  
  // Here "Constraints" has all the constraints. Just extract the constraints which contain the varstoelim and the others
  vector<DAGNode*> Constraints_Vector;
  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
  {
      Constraints_Vector.push_back(*it);
  }
  
  set<DAGNode*> Free_Constraints_Final;
  set<DAGNode*> Bound_Constraints_Final;
  
  getFreeAndBoundConstraintsForMultipleVariables(Constraints_Vector, VarsToElim, Free_Constraints_Final, Bound_Constraints_Final, WidthTable);
  
  for(set<DAGNode*>::iterator it=Free_Constraints_Final.begin(); it != Free_Constraints_Final.end(); it++)
    {
      if(QEInlinedLMIConjunction==NULL)
      {
          QEInlinedLMIConjunction = *it;
      }
      else
      {
          QEInlinedLMIConjunction = createDAG("and", QEInlinedLMIConjunction, *it, LOGICAL_SYMBOL, 1);
      }
    }// for
  
   for(set<DAGNode*>::iterator it=Bound_Constraints_Final.begin(); it != Bound_Constraints_Final.end(); it++)
    {
      if(QuantifiedInlinedLMIConjunction==NULL)
      {
          QuantifiedInlinedLMIConjunction = *it;
      }
      else
      {
          QuantifiedInlinedLMIConjunction = createDAG("and", QuantifiedInlinedLMIConjunction, *it, LOGICAL_SYMBOL, 1);
      }
    }// for
  
   if(QEInlinedLMIConjunction == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      QEInlinedLMIConjunction = dm.createNode("true",(void*)v);
    }
  
   if(QuantifiedInlinedLMIConjunction == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      QuantifiedInlinedLMIConjunction = dm.createNode("true",(void*)v);
    }

  }// end of function


// check if either 1) we have a single LMI 2^k2.c2.x+s \bowtie d or
// 2) we have a pair of LMIs 2^k2.c2.x+s \bowtie_1 d1, 2^k2.c2.x+s \bowtie_2 d2 where \bowtie \in {bvule, bvugt},
//  \bowtie_1,  \bowtie_2 \in  {bvule, bvugt}, and are different , and d, d_1, d_2 are constants
// returns true if (1) or (2) is true; else returns false
// Also sets Constraint_LHS = 2^k2.c2.x+s, Low = d1/d/NULL, and High = d2/d/NULL
bool singleLMIPair(set<DAGNode*> &Bound_Constraints, DAGNode* &Constraint_LHS, DAGNode* &Low, DAGNode* &High)
{    
    DAGNode* ule=NULL;
    DAGNode *ugt=NULL;
    
    if(Bound_Constraints.size()==1)
    {
        set<DAGNode*>::iterator it = Bound_Constraints.begin();
        DAGNode* Constraint = *it;
        
        if(Constraint->Get_node_name()=="bvule")
        {
            ule = Constraint;
        }
        else if(Constraint->Get_node_name()=="bvugt")
        {
            ugt = Constraint;
        }
        else
        {
            cout<<"\nConstraint "<<getDAGStr(Constraint)<<" other than bvule/bvugt encountered in function singleLMIPair in DAGEliminator.cpp\n";
            return false;
        }
        
        
                
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
         if(!isConstant(RHS))
        {
            cout<<"\nConstraint "<<getDAGStr(Constraint)<<" has RHS as a non-constant in function singleLMIPair in DAGEliminator.cpp\n"; 
            return false;
        } 
        
        if(ule != NULL)
        {
            High = RHS;
            Low = NULL;
            Constraint_LHS = LHS;
            cout<<"\nConstraint_LHS = "<<getDAGStr(Constraint_LHS)<<" , High = "<<getDAGStr(High)<<endl;
        }
        else
        {
           High = NULL;
           Low = RHS;
           Constraint_LHS = LHS; 
           cout<<"\nConstraint_LHS = "<<getDAGStr(Constraint_LHS)<<" , Low = "<<getDAGStr(Low)<<endl;
        }
        
        return true;
        
    }
    else if(Bound_Constraints.size()==2)
    {
        set<DAGNode*>::iterator it = Bound_Constraints.begin();
        DAGNode* Constraint1 = *it;
        it++;
        DAGNode* Constraint2 = *it;
        
        if(Constraint1->Get_node_name()=="bvule" && Constraint2->Get_node_name()=="bvugt")
        {
            ule = Constraint1;
            ugt = Constraint2;
        }
        else if(Constraint1->Get_node_name()=="bvugt" && Constraint2->Get_node_name()=="bvule")
        {
            ule = Constraint2;
            ugt = Constraint1;
        }
        else
        {
            cout<<"\nConstraint other than bvule/bvugt encountered in function singleLMIPair in DAGEliminator.cpp\n";
            return false;
        }
               
                
        list<DAGNode*>::iterator iout;
        iout = ule->Get_outlist_start();
        DAGNode *LHS_ule = *iout;
        iout++;
        DAGNode *RHS_ule = *iout;	
        
         if(!isConstant(RHS_ule))
        {
            cout<<"\nConstraint "<<getDAGStr(ule)<<" has RHS as a non-constant in function singleLMIPair in DAGEliminator.cpp\n"; 
            return false;
        }   
        
        iout = ugt->Get_outlist_start();
        DAGNode *LHS_ugt = *iout;
        iout++;
        DAGNode *RHS_ugt = *iout;	
        
         if(!isConstant(RHS_ugt))
        {
            cout<<"\nConstraint "<<getDAGStr(ugt)<<" has RHS as a non-constant in function singleLMIPair in DAGEliminator.cpp\n"; 
            return false;
        }
        
        if(LHS_ule != LHS_ugt)
        {
            cout<<"\nConstraints "<<getDAGStr(ule)<<" ,"<<getDAGStr(ugt)<<" have different LHSs in function singleLMIPair in DAGEliminator.cpp\n"; 
            return false;  
        }
        
        High = RHS_ule;
        Low = RHS_ugt;
        Constraint_LHS = LHS_ule;
        return true;
    }
    else
    {
        cout<<"\nYou are not supposed to call the function singleLMIPair in DAGEliminator.cpp with more than two constraints\n";
        exit(1);
    }
}// end of function


// Given 2^k2.c2.x in Constraint_LHS, and two dags LHS, RHS which contain 
// numbers, apply dropping using the counting argument
void dropSingleLMIPair(set<DAGNode*> &Bound_Constraints, DAGNode *Constraint_LHS, DAGNode *Low, DAGNode *High, string VariableToElim, set<DAGNode*> &QElim_Bound_Constraints)
{
       
    ULINT Coefficient;
    getCoefficientCorrected(Constraint_LHS, VariableToElim, Coefficient);// guaranteed to contain bvmul
    
    if(Coefficient%2==0)// Coefficient is even
    {
        cout<<"\nEven coefficient encountered in function dropSingleLMIPair in Eliminator.cpp\n";
        exit(1);
    }
       
    ULINT l2, u2, delta;
    
    if(Low == NULL)
    {
        l2 = 0;
    }
    else
    {
        string Low_str = Low->Get_node_name();
        l2 = strtoull(Low_str.c_str(), NULL, 0)+1;// Note that constraint is >. To make it l2 we need to add 1
    }
    
    if(High == NULL)
    {
        u2 = ModWidthInteger-1;
    }
    else
    {
        string High_str = High->Get_node_name();
        u2 = strtoull(High_str.c_str(), NULL, 0);
    }
    
    delta = u2-l2+1;
    
    // Coefficient is odd. Hence k2 = 0
    // We already know that p=k1=WidthInteger
    // Hence the check \floor(\delta / 2^(p-k1+k2)) >= 1 reduces to \delta >= 1
 
    if(delta >=1)// We can drop the Bound_Constraints
        // Note that this always happens, unless u2 < l2, given k2 = 0, and p=k1=WidthInteger
    {
        cout<<"\ndelta = "<<delta<<endl;
        cout<<"\n All the Bound_Constraints are dropped\n";
        QElim_Bound_Constraints.clear();
    }
    else
    {
        QElim_Bound_Constraints = Bound_Constraints;
    }
}



// Given a vector of ule, ugt constraints, apply dropping using the counting argument
// Here assumptions are p=k1=ModWidthInteger and k2=0
void dropMultipleLMIPairs(set<DAGNode*> &Bound_Constraints, vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, set<DAGNode*> &QElim_Bound_Constraints)
{
    if(Bound_Constraints.empty())
    {
        QElim_Bound_Constraints.clear();
        return;
    }    
    
    vector<long long int> Deltas;
        
    for(int i=0; i<LMIPairs.size(); i++)
    {
        DAGNode *ule = LMIPairs[i].first;
        DAGNode *ugt = LMIPairs[i].second;
        
        DAGNode *High=NULL;
        DAGNode *Low=NULL;
        DAGNode *Constraint_LHS=NULL;
        
        if(ule == NULL)
          {
            list<DAGNode*>::iterator iout;
            iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = NULL;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ugt; 
        }        
        else if(ugt==NULL)
            {
            list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
        
            High = RHS_ule;
            Low = NULL;
            Constraint_LHS = LHS_ule; 
            } 
        else if(ule != NULL && ugt != NULL)
        {
           list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
             iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = RHS_ule;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ule;  
        }
        else
        {
            cout<<"\nule and ugt both NULL in function dropMultipleLMIPairs in DAGEliminator.cpp\n";
            exit(1);
        }
        
        ULINT Coefficient;
        getCoefficientCorrected(Constraint_LHS, VariableToElim, Coefficient);// guaranteed to contain bvmul
        //cout<<"\nConstraint_LHS = "<<getDAGStr(Constraint_LHS)<<endl;
    
        if(Coefficient%2==0)// Coefficient is even
        {
            cout<<"\nEven coefficient encountered in function dropMultipleLMIPairs in DAGEliminator.cpp\n";
            exit(1);
        }
       
        long long int l2, u2;
        long long int delta;
    
        if(Low == NULL)
        {
            l2 = 0;
        }
        else
        {
            string Low_str = Low->Get_node_name();
            l2 = strtoull(Low_str.c_str(), NULL, 0)+1;// Note that constraint is >. To make it l2 we need to add 1
        }
    
        if(High == NULL)
        {
            u2 = ModWidthInteger-1;
        }
        else
        {
            string High_str = High->Get_node_name();
            u2 = strtoull(High_str.c_str(), NULL, 0);
        }
    
        delta = u2-l2+1;
        
        //cout<<"\ndelta = "<<delta<<endl;
    
        Deltas.push_back(delta);       
    }// for
    
    //Check the condition 
    long long int SumDeltas = 0;
    
    for(int i=0; i<Deltas.size(); i++)
    {
        SumDeltas += Deltas[i];
    }
    
    long long int ToSubtract;
    
    ToSubtract = (Deltas.size()-1)*ModWidthInteger;
    
    //cout<<"\nSumDeltas = "<<SumDeltas<<"\tToSubtract = "<<ToSubtract<<endl;
    
    long long int Eta = SumDeltas - ToSubtract;   
 
    if(Eta >=1)// We can drop the Bound_Constraints
    {
        //cout<<"\nEta  = "<<Eta<<endl;
        //cout<<"\n All the Bound_Constraints are dropped\n";
        QElim_Bound_Constraints.clear();
    }
    else
    {
        //cout<<"\nEta  = "<<Eta<<endl;
        //cout<<"\n None of the Bound_Constraints are dropped\n";
        QElim_Bound_Constraints = Bound_Constraints;
    }
}// end of function


//Create ule, ugt pairs from Constraints
 void identifyLMIPairs(set<DAGNode*> &Constraints, vector<pair<DAGNode*, DAGNode*> > &LMIPairs)
{
vector<DAGNode*> BVULE_Vector;// set of constraints in Constraints of the form <=
vector<DAGNode*> BVUGT_Vector;// Set of constraints in Constraints of the form >

for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
    {
       DAGNode* MyConstraint = *it;
       if(MyConstraint->Get_node_name()=="bvule")
       {
           BVULE_Vector.push_back(MyConstraint);
       }
       else if(MyConstraint->Get_node_name()=="bvugt")
       {
           BVUGT_Vector.push_back(MyConstraint);
       }
       else
       {
         cout<<"\nUnknown predicate "<<MyConstraint->Get_node_name()<<" inside function identifyLMIPairs in DAGEliminator.cpp\n";
         exit(1);
       }             

    }//for ends here


//showAPVector("BVULE_Vector", BVULE_Vector);
//showAPVector("BVUGT_Vector", BVUGT_Vector);

while(BVUGT_Vector.size()>0)
{
      DAGNode* ugt = BVUGT_Vector[0];
      //cout<<"\nTaking LMI "<<getDAGStr(ugt)<<endl;
      
      bool match_found=false;
      
      for(int j=0; j<BVULE_Vector.size(); j++)
      {
          DAGNode* ule = BVULE_Vector[j];

          if(LMIPairFound(ugt, ule))
          {
              //cout<<"\nMatches with "<<getDAGStr(ule)<<endl;
              //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
              LMIPairs.push_back(make_pair(ule, ugt));
              BVUGT_Vector.erase(BVUGT_Vector.begin());
              BVULE_Vector.erase(BVULE_Vector.begin()+j);
              match_found=true;
              break;
          }
      }

    if(!match_found)
        {
        //cout<<"\nNo matching ule constraint found. Removing the LMI\n";
        DAGNode* ule = NULL;
        LMIPairs.push_back(make_pair(ule, ugt));
        BVUGT_Vector.erase(BVUGT_Vector.begin());       
        }
    }// while ends here


for(int j=0; j<BVULE_Vector.size(); j++)
      {
          DAGNode* ule = BVULE_Vector[j];
          //cout<<"\nRemoving the LMI "<<getDAGStr(ule)<<"\n";
          DAGNode* ugt = NULL;
          LMIPairs.push_back(make_pair(ule, ugt));
    }//for
 }// end of function
 
  
 

// Given a dag node "node", variable to eliminate "VarToElim", and LME "VarToElim = RHS+Constant",
// replace dag nodes for "VarToElim" by "RHS+Constant", and return the simplified dag
DAGNode* cloneUsingSingleLMEExpression(DAGNode *node,  string VarToElim, DAGNode *LME)
{
    list<DAGNode*>::iterator LME_iout = LME->Get_outlist_start();
    LME_iout++;
    DAGNode* LME_RHS = *LME_iout;
            
    ULINT ConstantInLME_RHS=0;
    getConstantModified(LME_RHS, ConstantInLME_RHS);
    set<string> SupportLME_RHS;
    getDAGNames(LME_RHS, SupportLME_RHS);
    if(SupportLME_RHS.size()>1)
        {
        cout<<"\nMore than one variable in the support of "<<getDAGStr(LME_RHS)<<". System is not a system of octagons\n";
        exit(1);
        }
    string VariableInLME_RHS;
    set<string>::iterator SupportLME_RHS_it = SupportLME_RHS.begin();
    VariableInLME_RHS = *SupportLME_RHS_it;
    
    
    HashTable<string, DAGNode*> DPTable;
    HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
    DAGNode* ret = cloneUsingSingleLMEExpressionInternal(node, VarToElim, VariableInLME_RHS, ConstantInLME_RHS, PtrDPTable);
    return ret;
}

// Called from cloneUsingSingleLMEExpression
DAGNode* cloneUsingSingleLMEExpressionInternal(DAGNode *node,  string VarToElim, string VariableInLME_RHS, ULINT ConstantInLME_RHS, HashTable<string, DAGNode*> *DPTable)
{
  DAGNode* clone;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;
  
  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }
//cout<<"\nCache Miss for node "<<node<<"\n";
// Create the converted dag in clone
string MyName = node->Get_node_name();
Value* MyValue = (Value*)(node->getValue());
Type MyType = MyValue->get_node_type();
int MyWidth = MyType.width;
int MyDataType = MyType.data_type;

 if(MyName == "bvule" || MyName == "bvugt" || MyName == "bvuge" || MyName == "bvult") // bvule/bvugt/bvuge/bvult encountered
    {
     set<string> Support_node;
     getDAGNames(node, Support_node);
     
     if(Support_node.find(VarToElim) == Support_node.end()) //node free of VarToElim. Just make the same clone
            {
              list<DAGNode*>::iterator it = node->Get_outlist_start();
              vector<DAGNode *> Children;

              for(int i=0; i<node->Get_outlist_size(); i++)
                {
                DAGNode *child = cloneUsingSingleLMEExpressionInternal(*it, VarToElim, VariableInLME_RHS, ConstantInLME_RHS, DPTable);
                Children.push_back(child);
                it++;
                }
              
                clone = dm.createNode(MyName,Children, node->getValue());
            }//  if(Support_node.find(VarToElim) == Support.end()) 
     else
            {
            clone = cloneLMIUsingLME(node, VarToElim, VariableInLME_RHS, ConstantInLME_RHS);
            } // else
    }// bvule/bvugt ends here                   
 else if(node->Get_outlist_size() > 0)  // other operator nodes
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> Children;

      for(int i=0; i<node->Get_outlist_size(); i++)
              {
                DAGNode *child = cloneUsingSingleLMEExpressionInternal(*it, VarToElim, VariableInLME_RHS, ConstantInLME_RHS, DPTable);
                Children.push_back(child);
                it++;
              }
              
      clone = dm.createNode(MyName,Children, node->getValue());
    }	
 else if(node->Get_outlist_size()==0) // leaf level node
    {
       clone = dm.createNode(MyName, node->getValue());
    }// leaf level node
  else // unhandled operator
    {
      cout<<"\nIllegal operator "<<MyName<<" encountered in function cloneUsingSingleLMEExpressionInternal( in DAGEliminator.cpp\n";
      exit(1);
    }
  
  result = DPTable->hashtable_insert(key, clone);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside function cloneUsingSingleLMEExpressionInternal(  in DAGEliminator.cpp\n";
      exit(1);
    }
  return clone;
}


// Given an LMI c1.x1+....+cn.xn+c \bowtie d in "node", a variable xi in "VarToElim",
// a variable y in "VariableInLME_RHS", and a constant k in "ConstantInLME_RHS", 
// create an LMI which is c1.x1+....+cn.xn+c \bowtie d, simplified using xi=y+k. i.e
// return c1.x1+...+ci.y+...+cn.xn+c+k.ci \bowtie d
DAGNode* cloneLMIUsingLME(DAGNode* node, string VarToElim, string VariableInLME_RHS, ULINT ConstantInLME_RHS)
{
     string LMIType = node->Get_node_name();
     list<DAGNode*>::iterator iout = node->Get_outlist_start();
     DAGNode* LHS = *iout;
     iout++;
     DAGNode* RHS = *iout;
    
    map<string, ULINT> VariableCoefficientMap;
    ULINT ExistingConstant;
    
    obtainVariableCoefficientMap(LHS, VariableCoefficientMap, ExistingConstant);
    
    ULINT ModifiedConstant;
    map<string, ULINT>::iterator it=VariableCoefficientMap.find(VarToElim);
    ULINT CoefficientOfVarToElim = it->second;
    ModifiedConstant = add(ExistingConstant, mul(CoefficientOfVarToElim, ConstantInLME_RHS, ModWidthInteger), ModWidthInteger);
    
        
    // Now recreate the LHS part from VariableCoefficientMap and ModifiedConstant
    
    list<string> Vars;
    list<ULINT> Coefs;
    
    for(map<string, ULINT>::iterator vit=VariableCoefficientMap.begin(); vit != VariableCoefficientMap.end(); vit++)
    {
        if(vit->first == VarToElim)
            {
             Vars.push_back(VariableInLME_RHS);
            }
        else
            {
            Vars.push_back(vit->first);
            }
        Coefs.push_back(vit->second);
    }
    
    DAGNode* ModifiedLHS = getDAGFromVarsCoefsAndConstant(Vars, Coefs, ModifiedConstant, WidthInteger);
    
    vector<DAGNode*> Children;
    Children.push_back(ModifiedLHS);
    Children.push_back(RHS);
    
    DAGNode* clone = dm.createNode(LMIType,Children, node->getValue());
    
    return clone;
}


// Get #of vars to elim in input of layer2 of LMI QE algorithm
void getNumberOfVarsToElimInLayer2Input(DAGNode* Constraint, set<string> &VarsToElim)
{
    set<string> Support;
    set<string> CommonVars;
    
    getDAGNames(Constraint, Support);
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMIVarsInputToLayer2 = CommonVars.size();    
}


// Get #of vars to elim in input of layer3 of LMI QE algorithm
void getNumberOfVarsToElimInLayer3Input(DAGNode* Constraint, set<string> &VarsToElim)
{
    set<string> Support;
    set<string> CommonVars;
    
    getDAGNames(Constraint, Support);
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMIVarsInputToLayer3 = CommonVars.size();    
}


// Get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
// in the respective global variables, and print them
void getRelevantLMIDetails(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
    LMIMonniauxCounter++;
    
    set<string> Support;
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode *Constraint = *it;
        
        getDAGNames(Constraint, Support);
        
        string ConstraintType = Constraint->Get_node_name();
        
        if(ConstraintType == "=" )
            LMINoLMEs++;
        else if(ConstraintType == "is_not_equal" )
            LMINoLMDs++;
        else if(ConstraintType == "bvule" ||  ConstraintType == "bvugt" || ConstraintType == "bvult" || ConstraintType == "bvuge")
            LMINoLMIs++;
        else
        {
            cout<<"\nUnknown constraint type "<<ConstraintType<<" in function getRelevantLMIDetails in DAGEliminator.cpp\n";
            exit(1);
        }
    }// for
    
    set<string> CommonVars;
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMINoVarsToElim = CommonVars.size();
    LMINoVars = Support.size();
    
    LMITotalBits = 0;
    
    for(set<string>::iterator it=Support.begin(); it != Support.end(); it++)
        {
        string signal = *it;
        map<string, int>::iterator wit =WidthTable.find(signal);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for "<<signal<<" in the WidthTable in function getRelevantLMIDetails in DAGEliminator.cpp\n";
            exit(1);
            }
        LMITotalBits += wit->second;        
        }

    LMIBitsToElim = 0;
    
    for(set<string>::iterator it=CommonVars.begin(); it != CommonVars.end(); it++)
        {
        string signal = *it;
        map<string, int>::iterator wit =WidthTable.find(signal);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for "<<signal<<" in the WidthTable in function getRelevantLMIDetails in DAGEliminator.cpp\n";
            exit(1);
            }
        LMIBitsToElim += wit->second;        
        }
    
fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), LMIMonniauxCounter, LMINoLMIs, LMINoLMEs, LMINoLMDs, LMINoVars, LMINoVarsToElim, LMITotalBits, LMIBitsToElim);
    
}//function





// Function which implements our QE methods on conjunction of LMIs. 
// This function can be used when the input conjunction has LMIs, LMEs, and LMDs
DAGNode* lmiQEUsingOurMethodsGeneral(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  // Initialize the post-layer1 details
  //initializeProjectPostLayer1Details();
    
  // Initialize all the conjunction-level parameters to zero
   initializeConjunctionLevelParameters();  
    
  //showAPVector("Constraints", Constraints);
  showSet(VarsToElim, "VarsToElim"); 
  // showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  showAPSet("Free_Constraints", Free_Constraints);
  showAPSet("Bound_Constraints", Bound_Constraints);
  
  //showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  //showAPSet("Bound_ConDAGNode* Quantified_Bound_Part;
    
  
  // get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
  // in the respective global variables and write in the file
  getRelevantLMIDetails(Bound_Constraints, VarsToElim, WidthTable);
  
  
  DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
            {
             Bound_Constraints_Vector.push_back(*it); 
            }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
  
  // Let us split the bound constraints into LMEs, LMIs, and LMDs
  
  set<DAGNode*> LMESet;
  set<DAGNode*> LMISet;
  set<DAGNode*> LMDSet;
  
  
  for(set<DAGNode*>::iterator it=Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
  {
      DAGNode* Constraint = *it;
      
      if(Constraint->Get_node_name()=="=")
      {
          LMESet.insert(Constraint);
      }
      else if(Constraint->Get_node_name()=="is_not_equal")
      {
          LMDSet.insert(Constraint);
      }
      else if(Constraint->Get_node_name()=="bvule" || Constraint->Get_node_name()=="bvuge"  || Constraint->Get_node_name()=="bvult" || Constraint->Get_node_name()=="bvugt")
      {
          LMISet.insert(Constraint);
      }
      else
      {
          cout<<"\nUnknown constraint type "<<Constraint->Get_node_name()<<" in function lmiQEUsingOurMethodsGeneral in DAGEliminator.cpp\n";
          exit(1);
      }
  }//for
      
  
  if(LMISet.empty())// No LMIs. Use CAV'11 work
  {
      struct timeval layer1_start_ms, layer1_finish_ms;
      gettimeofday (&layer1_start_ms, NULL);      
      
      
      cout<<"\nNo LMIs appear in the conjunction\n";
      
      set<DAGNode*> LMELMDSet;
      set_union(LMESet.begin(),  LMESet.end(), LMDSet.begin(), LMDSet.end(), inserter(LMELMDSet, LMELMDSet.begin())); 
      vector<DAGNode*> LMELMDVector;
      
      for(set<DAGNode*>::iterator it=LMELMDSet.begin(); it != LMELMDSet.end(); it++)
        {
        LMELMDVector.push_back(*it);
        }
      
      showAPVector("LMELMDVector", LMELMDVector);
      //exit(1);
		
     DAGNode* QFreePart = callAPResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(LMELMDVector, VarsToElim, WidthTable);
     
     cout<<"\nQFreePart = "<<getDAGStr(QFreePart)<<endl;
     
     DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
     
     cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
          
     DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
     
     
     gettimeofday (&layer1_finish_ms, NULL);
     LMILayer1Time = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
     LMILayer1Time -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;
     
     fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2, LMIVarsInputToLayer3, LMIVarsInputToLayer4, LMILayer1Time, LMILayer2Time, LMILayer3Time, LMILayer4Time);
     //fprintf(LMIQELayer1EffectivenessFp, "\n%s\t%d\t%d\t%d\t%llu\t%d\t%d\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMINoLMIs+LMINoLMDs+LMINoLMEs, LMINoVarsToElim, LMIBitsToElim, LMIPostLayer1NoLMCs, LMIPostLayer1NoVarsToElim, LMIPostLayer1NoBitsToElim);
      
     return resultDAG;
  }
  
  else
  {
      cout<<"\nLMIs appear in the conjunction\n";
      showAPSet("LMESet", LMESet);
      showAPSet("LMDSet", LMDSet);
      showAPSet("LMISet", LMISet);
      //char c = getchar();
      
      DAGNode* ResultSimplification = QEFromGeneralConjunctionMultipleVariables(LMESet, LMDSet, LMISet, VarsToElim, WidthTable);
      cout<<"\nResultSimplification = "<<getDAGStr(ResultSimplification)<<endl;
     
      DAGNode *QFreePart=createLogicalConstant("true");
      DAGNode *BoundPart=createLogicalConstant("true");
      
      if(!isTrue(ResultSimplification))
        {
        divideConjunctionToFreeAndBoundParts(ResultSimplification, VarsToElim, WidthTable, QFreePart, BoundPart);
        }
      
      if(isTrue(BoundPart))
        {
        DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
        cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
        DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
        
        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2, LMIVarsInputToLayer3, LMIVarsInputToLayer4, LMILayer1Time, LMILayer2Time, LMILayer3Time, LMILayer4Time);
        //fprintf(LMIQELayer1EffectivenessFp, "\n%s\t%d\t%d\t%d\t%llu\t%d\t%d\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMINoLMIs+LMINoLMDs+LMINoLMEs, LMINoVarsToElim, LMIBitsToElim, LMIPostLayer1NoLMCs, LMIPostLayer1NoVarsToElim, LMIPostLayer1NoBitsToElim);
    
        return resultDAG;
        }
      else
        {
          cout<<"\nWe need to eliminate quantifiers from "<<getDAGStr(BoundPart)<<endl;
          
          //char c = getchar();
          
          set<DAGNode*> Quantified_Bound_Part_Constraints;
          splitConjunctionIntoLMISet(BoundPart, Quantified_Bound_Part_Constraints);
          
          //getRelevantProjectPostLayer1Details(Quantified_Bound_Part_Constraints, VarsToElim, WidthTable);
          //fprintf(LMIQELayer1EffectivenessFp, "\n%s\t%d\t%d\t%d\t%llu\t%d\t%d\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMINoLMIs+LMINoLMDs+LMINoLMEs, LMINoVarsToElim, LMIBitsToElim, LMIPostLayer1NoLMCs, LMIPostLayer1NoVarsToElim, LMIPostLayer1NoBitsToElim);
    
          
          set< set<DAGNode*> > ScopeReducedDAGs;
          
          struct timeval layer3_start_ms, layer3_finish_ms;
	 gettimeofday (&layer3_start_ms, NULL);    
          
         bool result_trustworthy;
         
         if(SwitchOffLayer3)
         {
             result_trustworthy =  false;
         }
         else
         {        
            result_trustworthy =   semanticScopeReduction(BoundPart, Quantified_Bound_Part_Constraints, VarsToElim, WidthTable, ScopeReducedDAGs);  
         }
          
         gettimeofday (&layer3_finish_ms, NULL);
   	 LMILayer3Time = layer3_finish_ms.tv_sec * 1000 + layer3_finish_ms.tv_usec / 1000;
   	 LMILayer3Time -= layer3_start_ms.tv_sec * 1000 + layer3_start_ms.tv_usec / 1000;
         
            
          if(!result_trustworthy)
              {
                 cout<<"\nz3 timed out during computation. Result is cannot be trusted\n";
                 cout<<"\nWe need to eliminate quantifiers from "<<getDAGStr(BoundPart)<<endl;
                 
                    LMILayer3CallsTimedOut++;
                
                    int LMCCountInInputConjunction;
                    int VarsCountInInputConjunction;
                    int VarsToElimCountInInputConjunction;
                    int PartitionsInOutputConjunction;
                    int MaxPartitionSizeInOutputConjunction;
                    int MinPartitionSizeInOutputConjunction;
                    float AvgPartitionSizeInOutputConjunction;
                   
                    extractLayer3Data(false, Quantified_Bound_Part_Constraints, VarsToElim, ScopeReducedDAGs, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction);
                   
                    fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t0\t%d\t%d\t%d\t-\t-\t-\t-\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, LMILayer3Time);
                    fprintf(LMIQEFinishedLayer3TimedOutDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t-\t-\t-\t-\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, LMILayer3Time);
                 
                     bool CallLayer4Local=true;
                     
                     if(CallLayer4Local)
                     {
                        //char c = getchar();
                          struct timeval layer4_start_ms, layer4_finish_ms;
                          gettimeofday (&layer4_start_ms, NULL);    
                         
                         
                        Octagons = true; //Use the LinDD work
                        vector<DAGNode*> Quantified_Bound_Part_Constraints_Vector;
                        copySetToVector(Quantified_Bound_Part_Constraints, Quantified_Bound_Part_Constraints_Vector);
                        convertStrictLMIsToWeakLMIs(Quantified_Bound_Part_Constraints_Vector, WidthTable);
                        BoundPart = getConjunctionFromVector(Quantified_Bound_Part_Constraints_Vector);
                                
                        DAGNode* RemainingResult = expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(BoundPart, VarsToElim, WidthTable, 0);  
                        DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
                        cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
                        DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
                        resultDAG = createDAG("and", resultDAG, RemainingResult, LOGICAL_SYMBOL, 1);
                        cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;
                        //c = getchar();
                        Octagons = false; 
                        
                        gettimeofday (&layer4_finish_ms, NULL);
                        LMILayer4Time = layer4_finish_ms.tv_sec * 1000 + layer4_finish_ms.tv_usec / 1000;
                        LMILayer4Time -= layer4_start_ms.tv_sec * 1000 + layer4_start_ms.tv_usec / 1000;
                        
                        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2, LMIVarsInputToLayer3, LMIVarsInputToLayer4, LMILayer1Time, LMILayer2Time, LMILayer3Time, LMILayer4Time);
                                           
                        return resultDAG;
                      }
              }
              else
              {
                   LMILayer3CallsFinished++;
                   
                   int LMCCountInInputConjunction;
                   int VarsCountInInputConjunction;
                   int VarsToElimCountInInputConjunction;
                   int PartitionsInOutputConjunction;
                   int MaxPartitionSizeInOutputConjunction;
                   int MinPartitionSizeInOutputConjunction;
                   float AvgPartitionSizeInOutputConjunction;
                   
                   extractLayer3Data(true, Quantified_Bound_Part_Constraints, VarsToElim, ScopeReducedDAGs, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction);
                   
                   
                   
                    if(ScopeReducedDAGs.size()==0)
                        {
                        LMILayer3CallsTrue++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t3\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3Time);
                        fprintf(LMIQEFinishedLayer3TrueDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3Time);
                         }
                   else if(ScopeReducedDAGs.size()==1)
                        {
                        LMILayer3CallsNoSC++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t2\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3Time);
                        fprintf(LMIQEFinishedLayer3NoSCDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3Time);
                        }
                   else
                        {
                         LMILayer3CallsSCAchieved++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t1\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3Time);
                        fprintf(LMIQEFinishedLayer3SCAchievedDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), LMIMonniauxCounter, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3Time);
                        }
           
             
                  cout<<"\nScopeReducedDAGs\n";
                  
                  int partition_number=1;
                  for(set< set<DAGNode*> >::iterator sdagit = ScopeReducedDAGs.begin(); sdagit != ScopeReducedDAGs.end(); sdagit++)
                  {
                    cout<<"\nPartition "<<partition_number<<endl;
                    set<DAGNode*> MyDAGSet = *sdagit;
                    showAPSet("Partition", MyDAGSet);  
                    partition_number++;
                  }//for    
                  
                  if(ScopeReducedDAGs.empty())// Scopereduction reduces Bound_Part to true
                  {
                     DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
                    cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
                    DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
                    
                   fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2, LMIVarsInputToLayer3, LMIVarsInputToLayer4, LMILayer1Time, LMILayer2Time, LMILayer3Time, LMILayer4Time);
                    
                    //c=getchar();
                    return resultDAG; 
                  }
                  else
                  {
                      cout<<"\nWe still need to eliminate quantifiers \n";
                      exit(1);
                  }//else
              }// else          
       }//else of   if(isTrue(BoundPart))       
  }//else of  if(LMISet.empty())
}//function




// Given a set of LMEs, LMDs, and LMIs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode*
DAGNode* QEFromGeneralConjunctionMultipleVariables(set<DAGNode*> &E, set<DAGNode*> &D,  set<DAGNode*> &I, set<string> VarsToElim, map<string, int> &WidthTable)
{

  //cout<<"\nAPResolveModifiedWithMultipleVariablesUsingLMA called\n";

  set<DAGNode*> Constraints;
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  DAGNode* QElim_Bound_Constraint;

  showSet(VarsToElim,"VarsToElim");

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));
  set_union(Constraints.begin(), Constraints.end(), I.begin(), I.end(),inserter(Constraints, Constraints.begin()));

  showAPSet("Constraints", Constraints);



  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;

      cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      showAPSet("Free_Constraints", Free_Constraints);
      showAPSet("Bound_Constraints", Bound_Constraints);

      QElim_Bound_Constraints.clear();
      QElim_Bound_Constraint = NULL; // stands for true

      if(Bound_Constraints.size()>0)
	{
          set<DAGNode*> LMESet;
          set<DAGNode*> LMDSet;
          set<DAGNode*> LMISet;
          
          splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
          
          if(LMISet.empty()) // no LMIs. Use CAV'11 work to eliminate this variable
              // Elimination guaranteed
            {
              cout<<"\nLMIs do not appear in the conjunction\n";
              
              struct timeval layer1_start_ms, layer1_finish_ms;
              gettimeofday (&layer1_start_ms, NULL);      
              
              
              QElim_Bound_Constraint = callMatrixQEliminator(Bound_Constraints, VariableToElim, WidthTable);
              
              gettimeofday (&layer1_finish_ms, NULL);
              unsigned long long int LMILayer1TimeHere = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
              LMILayer1TimeHere -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;
              
              LMILayer1Time += LMILayer1TimeHere;
            }
          else
            {
              cout<<"\nLMIs appear in the conjunction\n";
              showAPSet("LMESet", LMESet);
              showAPSet("LMDSet", LMDSet);
              showAPSet("LMISet", LMISet);
              
              if(LMESet.empty())
                {
                  cout<<"\nLMESet is empty. Only option is to drop\n";
                  //char c = getchar();
                  
                  set<DAGNode*> MostConstrainingLME;
                  MostConstrainingLME.clear(); 
                  
                  int WidthOfVarToElim;
                  map<string, int>::iterator wit = WidthTable.find(VariableToElim);
                  if(wit == WidthTable.end())
                    {
                    cout<<"\nNo entry for variable "<<VariableToElim<<" in the widthtable in function QEFromGeneralConjunctionMultipleVariables in DAGEliminator.cpp\n";
                    exit(1);
                    }
                  WidthOfVarToElim = wit->second;
                  
                  // Using layer2 - elimination is not guaranteed
                  // Increase LMIVarsInputToLayer2 by one
                  LMIVarsInputToLayer2++;
                  
                  struct timeval layer2_start_ms, layer2_finish_ms;
                  gettimeofday (&layer2_start_ms, NULL);     
                  
                  QElim_Bound_Constraint = dropUnconstrainingDisequationsAndInequations(MostConstrainingLME, LMDSet, LMISet, VariableToElim, WidthOfVarToElim);
                  
                  gettimeofday (&layer2_finish_ms, NULL);
                  unsigned long long int LMILayer2TimeHere = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
                  LMILayer2TimeHere -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;
              
                  LMILayer2Time += LMILayer2TimeHere;
                  
                  
                  if(true)
                    {
                    set<string> SupportOfQElim_Bound_Constraint;
                    getDAGNames(QElim_Bound_Constraint, SupportOfQElim_Bound_Constraint);
                    if(SupportOfQElim_Bound_Constraint.find(VariableToElim)!=SupportOfQElim_Bound_Constraint.end())// result still has var to elim
                        // We need to call Layer3
                        {
                        cout<<"\nDropping could not finish the QE\n" ;
                        //Increase LMIVarsInputToLayer3 by one
                        
                        if(!SwitchOffLayer3)
                            LMIVarsInputToLayer3++;
                        else
                            LMIVarsInputToLayer4++;
                        }
                    }//if(true)
                  
                }//if(LMESet.empty())
              else// LMEBased simplification for LMDs and LMIs
                {
                  
                  // Using layer1 - elimination is not guaranteed
                                    
                  struct timeval layer1_start_ms, layer1_finish_ms;
                  gettimeofday (&layer1_start_ms, NULL);     
                  
                  QElim_Bound_Constraint = callMatrixQEliminatorForLMCsAndLMIs(LMESet, LMDSet, LMISet, VariableToElim, WidthTable);
                  
                  
                  gettimeofday (&layer1_finish_ms, NULL);
                  unsigned long long int LMILayer1TimeHere = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
                  LMILayer1TimeHere -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;
              
                  LMILayer1Time += LMILayer1TimeHere;
                  
                  
                  if(true)
                    {
                    set<string> SupportOfQElim_Bound_Constraint;
                    getDAGNames(QElim_Bound_Constraint, SupportOfQElim_Bound_Constraint);
                    if(SupportOfQElim_Bound_Constraint.find(VariableToElim)!=SupportOfQElim_Bound_Constraint.end())// result still has var to elim
                    // We need to call Layer2
                        {
                        cout<<"\nLME based reasoning could not finish the QE\n" ;
                         //Increase LMIVarsInputToLayer2 by one
                        LMIVarsInputToLayer2++;
                        }  
                  }//if(true)
                }
            }//else     
	  
	}// if 

      if(QElim_Bound_Constraint != NULL) // stands for true
	{
          
          if(!isTrue(QElim_Bound_Constraint))
                {
                cout<<"\nQElim_Bound_Constraint = "<<getDAGStr(QElim_Bound_Constraint)<<"\n";
                splitConjunctionIntoSetGeneral(QElim_Bound_Constraint, QElim_Bound_Constraints);
                }
	}

      showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      showAPSet("Constraints", Constraints);
    }

  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
  
    if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    resultDAG = quickSimplify(resultDAG);

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    return resultDAG;
}



// Function to drop LMIs and LMDs given \exists x. (Conjunction of LMEs /\ Conjunction of LMDs /\ Conjunction of LMIs) mod 2^WidthOfVarToElim
DAGNode* dropUnconstrainingDisequationsAndInequations(set<DAGNode*> &LME, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet, string VarToElim,  int WidthOfVarToElim)
{
set<DAGNode*> Constraints;
set<DAGNode*> Phi;
set<DAGNode*> Xi;

// Constraints <--- E \union D \union I
set_union(LME.begin(), LME.end(), LMDSet.begin(), LMDSet.end(), inserter(Constraints, Constraints.begin()));   
set_union(Constraints.begin(), Constraints.end(), LMISet.begin(), LMISet.end(), inserter(Constraints, Constraints.begin()));  
showAPSet("Constraints", Constraints);

// \phi <---- E
set_union(Phi.begin(), Phi.end(), LME.begin(), LME.end(), inserter(Phi, Phi.begin()));   
showAPSet("Phi", Phi);

// \xi <--- Costraints \ \phi
set_difference(Constraints.begin(), Constraints.end(), Phi.begin(), Phi.end(), inserter(Xi, Xi.begin()));   
showAPSet("Xi", Xi);

while(!setsEqual(Phi, Constraints))
    {
    cout<<"\nPhi neq Constraints\n";
    DAGNode *AdditionalConstraint = createLogicalConstant("true");
    //The AdditionalConstraint is for bvugt constraints. They can be dropped, but we need to extract
    // these additional constraints from them
    if(isExtForLMDsAndLMIs(Phi, Xi, VarToElim, WidthOfVarToElim, AdditionalConstraint)) // \exists x. (Xi /\ Phi) \equiv \exists x. (Phi)
        {
        if(isTrue(AdditionalConstraint))
            {
             if(Phi.empty())
                    return createLogicalConstant("true");
             else
                    return getConjunctionFromSet(Phi);
            }// if(isTrue(AdditionalConstraint)) ends here
        else
            {
            cout<<"\nAdditionalConstraint = "<<getDAGStr(AdditionalConstraint)<<endl;
              if(Phi.empty())
                    return AdditionalConstraint;
             else
                    return createDAG("and", AdditionalConstraint, getConjunctionFromSet(Phi),  LOGICAL_SYMBOL, 1);
            }// else of if(isTrue(AdditionalConstraint)) ends here
        }//if
    else
        {
        set<DAGNode*> Pi;
        findConstraintsWithMaximumKValueForLMDsAndLMIs(Xi, VarToElim, Pi);
        set_union(Phi.begin(), Phi.end(), Pi.begin(), Pi.end(), inserter(Phi, Phi.begin()));  // \phi <---- \phi \union \pi 
        }//else
    
    }//while

cout<<"\nPhi = Constraints\n";
return getConjunctionFromSet(Phi);
}//function


// Find the set of constraints with maximum k value among a set of LMIs an LMDs
void findConstraintsWithMaximumKValueForLMDsAndLMIs(set<DAGNode*> &Xi, string VarToElim, set<DAGNode*> &Pi)
{
    map<DAGNode*, ULINT> KValueMap;
    
    ULINT MaxKValue = 0;
    
    for(set<DAGNode*>::iterator it=Xi.begin(); it != Xi.end(); it++)
    {
        DAGNode* Constraint = *it;
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;
        
        ULINT Coefficient;
        getCoefficientCorrected(LHS, VarToElim, Coefficient);// Assumption here is that variables
        // appear only on the LHS of LMI
        
        ULINT MyKValue = powerOfTwo(Coefficient);
        
        if(MyKValue > MaxKValue)
            {
            MaxKValue = MyKValue;
            }
        
        KValueMap.insert(make_pair(Constraint, MyKValue));
    }//for
    
    for(map<DAGNode*, ULINT>::iterator it=KValueMap.begin(); it != KValueMap.end(); it++)
    {
        if(it->second == MaxKValue)
        {
           Pi.insert(it->first); 
        }//if
    }//for
}


// Find maximum k-value from a set of constraints
int findMaximumKValue(set<DAGNode*> &Phi, string VarToElim, int WidthOfVarToElim)
{
    if(Phi.empty())// \phi empty. k1=WidthOfVarToElim
    {
        return WidthOfVarToElim;
    }
    else
    {
        cout<<"\nPhi not empty in function findMaximumKValue in DAGEliminator.cpp. This is not implemented\n";
        exit(1);
    }
}

// isExt function for LMIs and LMDs
bool isExtForLMDsAndLMIs(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, DAGNode*  &AdditionalConstraint)
{
    int k1 = findMaximumKValue(Phi, VarToElim, WidthOfVarToElim);
    
    cout<<"\nk1 = "<<k1<<endl;
    
    ULINT ModWidthOfVarToElim = findPower(WidthOfVarToElim);
    
    cout<<"\nWidthOfVarToElim = "<<WidthOfVarToElim<<", ModWidthOfVarToElim = "<<ModWidthOfVarToElim<<endl;
    
    set<DAGNode*> LMESet;
    set<DAGNode*> LMDSet;
    set<DAGNode*> LMISet;
    
    splitConstraintsIntoLMELMDLMISets(Xi, LMESet, LMDSet, LMISet);
    // Note that LMESet here is empty always
    
    if(!LMDSet.empty())
    {
    //cout<<"\nLMDSet is not empty in the function isExtForLMDsAndLMIs in DAGEliminator.cpp. This needs to be implemented\n";
    bool extend = isExtForLMDsAndLMIsWithLMDs(Phi, Xi, VarToElim, WidthOfVarToElim, AdditionalConstraint, k1, ModWidthOfVarToElim, LMESet, LMDSet, LMISet);
    //char c = getchar();
    return extend;
    }
    
    // Let us just ensure that the RHS of all LMIs are constants
    
    set<DAGNode*> ReplacableLMIs;
    
    for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* lmi = *it;
        if(!lmiRHSConstant(lmi))
        {
            cout<<"\nRHS of the LMI "<<getDAGStr(lmi)<<" is not a constant in function  isExtForLMDsAndLMIs in DAGEliminator.cpp. This needs to be implemented\n";
            
            if(lmi->Get_node_name()=="bvugt")
                {
                cout<<"\nLet us try to replace this by a tighter one for dropping. But we need to add a new LMI free of "<<VarToElim<<"\n";
                
                bool replacable = checkIfLMIIsReplacable(lmi, LMISet); // An lmi is replacable if there does not exist any other LMI with the same LHS and opposite sign
                
                if(replacable)
                    {
                    ReplacableLMIs.insert(lmi);
                    
                    
                    //NewConstraint is lmi's RHS \neq (2^p)-1
                    DAGNode *NewConstraint = getNewConstraintFromBVUGTConstraint(lmi, ModWidthOfVarToElim, WidthOfVarToElim);
                    // Conjunct the NewConstraint with the AdditionalConstraint
                    if(isTrue(AdditionalConstraint))
                        {
                        AdditionalConstraint = NewConstraint;
                        }
                    else
                        {
                        AdditionalConstraint = createDAG("and", AdditionalConstraint, NewConstraint,  LOGICAL_SYMBOL, 1);
                        }
                    }
                else
                    {
                    cout<<"\nLMI cannot be replaced in function isExtForLMDsAndLMIs in DAGEliminator.cpp.\n";
                    exit(1);
                    }//else
                }
            else if(lmi->Get_node_name()=="bvule")
                {
                cout<<"\nLet us try to replace this by a tighter one for dropping\n";
                
                bool replacable = checkIfLMIIsReplacable(lmi, LMISet); // An lmi is replacable if there does not exist any other LMI with the same LHS and opposite sign
                
                if(replacable)
                    {
                    ReplacableLMIs.insert(lmi);
                    }
                else
                    {
                    cout<<"\nLMI cannot be replaced in function isExtForLMDsAndLMIs in DAGEliminator.cpp.\n";
                    exit(1);
                    }//else
                }//else if
            else
                {
                cout<<"\nUnknown LMI type "<<lmi->Get_node_name()<<" in function isExtForLMDsAndLMIs in DAGEliminator.cpp.\n";
                exit(1);
                }//else
            
        }//if
    }//for
   
    
   if(!ReplacableLMIs.empty()) 
        {
         showAPSet("ReplacableLMIs", ReplacableLMIs);
         replaceLMIsByTighterOnes(ReplacableLMIs, WidthOfVarToElim, LMISet);
         showAPSet("LMISet after replacement", LMISet);
         //char c = getchar();
         }
   // Let us form LMI Pairs 
   vector<pair<DAGNode*, DAGNode*> > LMIPairs; 
   identifyLMIPairs(LMISet, LMIPairs);
   showLMIPairs(LMIPairs); 
   long long int CombinationsPermitted = findCombinationsPermittedByLMIs(LMIPairs,  VarToElim, k1, WidthOfVarToElim, ModWidthOfVarToElim);
   
   if(CombinationsPermitted >= 1)
   {
       return true;
   }

   return false;
}


// An lmi is replacable if there does not exist any other LMI with the same LHS and opposite sign
bool checkIfLMIIsReplacable(DAGNode* lmi, set<DAGNode*> &LMISet) 
{
   for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* mylmi = *it;
        if(lmi != mylmi && lhsMatching(lmi, mylmi) && lmi->Get_node_name() != mylmi->Get_node_name())
            return false;      
    }//for
   return true;    
}
                

// check if the LHSs of the LMIs are matching
bool lhsMatching(DAGNode *first, DAGNode* second) 
{
    list<DAGNode*>::iterator iout;
    iout = first->Get_outlist_start();
    DAGNode *first_LHS = *iout;
   
    iout = second->Get_outlist_start();
    DAGNode *second_LHS = *iout;
    
    if(first_LHS == second_LHS)
    {
        return true;
    }
    return false;
}


// Function which returns the #of ways to engineer the k1-k2 part given a set of LMI pairs, k1, and a variable to eliminate
long long int findCombinationsPermittedByLMIs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, int k1, ULINT WidthOfVarToElim, ULINT ModWidthOfVarToElim)
{
   // Find the delta, k of each LMI pair
    vector<long long int> Deltas;
    vector<ULINT> KValues;
        
    for(int i=0; i<LMIPairs.size(); i++)
    {
        //cout<<"\nReached here at beginning of loop\n";
        DAGNode *ule = LMIPairs[i].first;
        DAGNode *ugt = LMIPairs[i].second;
        
        DAGNode *High=NULL;
        DAGNode *Low=NULL;
        DAGNode *Constraint_LHS=NULL;
        
        if(ule == NULL)
          {
            //cout<<"\nInside if(ule == NULL)\n";
            
            list<DAGNode*>::iterator iout;
            iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = NULL;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ugt; 
        }        
        else if(ugt==NULL)
            {
            list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
        
            High = RHS_ule;
            Low = NULL;
            Constraint_LHS = LHS_ule; 
            } 
        else if(ule != NULL && ugt != NULL)
        {
           list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
             iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = RHS_ule;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ule;  
        }
        else
        {
            cout<<"\nule and ugt both NULL in function findCombinationsPermittedByLMIs in DAGEliminator.cpp\n";
            exit(1);
        }
        
        //cout<<"\noutside if-else structure\n";
        //cout<<"\nConstraint_LHS = "<<getDAGStr(Constraint_LHS)<<endl;
        
        ULINT Coefficient;
        getCoefficientCorrected(Constraint_LHS, VariableToElim, Coefficient);
        
        //cout<<"\nCoefficient = "<<Coefficient<<endl;
        
        ULINT MyKValue = powerOfTwo(Coefficient);
        KValues.push_back(MyKValue);
               
        //cout<<"\nMyKValue = "<<MyKValue<<endl;
       
       
        long long int l2, u2;
        long long int delta;
    
        if(Low == NULL)
        {
            l2 = 0;
        }
        else
        {
            string Low_str = Low->Get_node_name();
            l2 = strtoull(Low_str.c_str(), NULL, 0)+1;// Note that constraint is >. To make it l2 we need to add 1
        }
    
        if(High == NULL)
        {
            u2 = ModWidthOfVarToElim-1;
        }
        else
        {
            string High_str = High->Get_node_name();
            u2 = strtoull(High_str.c_str(), NULL, 0);
        }
    
        delta = u2-l2+1;
        
        //cout<<"\ndelta = "<<delta<<endl;
    
        Deltas.push_back(delta);       
    }// for
    
    
    // Check if all k values are the same
    ULINT MyKValue = KValues[0];
    
    for(int i=0; i<KValues.size(); i++)
        {
        if(MyKValue != KValues[i])
            {
            cout<<"\nAll k-values are not the same in the function findCombinationsPermittedByLMIs in DAGEliminator.cpp\n";
            exit(1);
            }
        }
    
    // All k-values are the same. Let the k-value be k2
    ULINT k2 = KValues[0];
    
    //Let us return \floor (\delta_i/2^(p-k1+k2)) - (n-1).2^(k1-k2)
    ULINT FirstPart=0;
    
    for(int i=0; i<Deltas.size(); i++)
    {
        ULINT Denominator = findPower(WidthOfVarToElim-k1+k2);
        ULINT Numerator = Deltas[i];
        
        cout<<"\nNumerator = "<<Numerator<<"\tDenominator = "<<Denominator<<endl;
        
        double quotient = floor(Numerator/Denominator);
        FirstPart += (ULINT)quotient;
    }
    
    long long int SignedFirstPart = (long long int)FirstPart;
    
    long long int SecondPart;
    
    SecondPart = (Deltas.size()-1)*findPower(k1-k2);
    
    cout<<"\nSignedFirstPart = "<<SignedFirstPart<<"\tSecondPart = "<<SecondPart<<endl;
    
    long long int CombinationsPermitted = SignedFirstPart - SecondPart;
    
    //cout<<"\nCombinationsPermitted = "<<CombinationsPermitted<<endl;
    
    return CombinationsPermitted;
   
}// end of function





// Recreate LMIset by replacing LMIs of the form <= with variables in RHS by LMIs which are tighter with constant on the RHS
void replaceLMIsByTighterOnes(set<DAGNode*> &ReplacableLMIs, int WidthOfVarToElim, set<DAGNode*> &LMISet)
{
    set<DAGNode*> NewLMISet;
    
    for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* LMI = *it;
        if(ReplacableLMIs.find(LMI)==ReplacableLMIs.end()) // LMI is not in Replacable set
        {
            NewLMISet.insert(LMI);
        }
        else //LMI is replacable
        {
            DAGNode* NewLMI;
            
            if(LMI->Get_node_name()=="bvule")
                {
                NewLMI = findNewTighterLMI(LMI, WidthOfVarToElim);
                }
            else if(LMI->Get_node_name()=="bvugt")
                {
                NewLMI = findNewTighterLMIForBVUGT(LMI, WidthOfVarToElim);
                }
            else
                {
                cout<<"\nUnknown LMI type "<<LMI->Get_node_name()<<" in function replaceLMIsByTighterOnes in DAGEliminator.cpp\n";
                exit(1);
                }
            NewLMISet.insert(NewLMI);
        }
    }//for
    
    LMISet.clear();
    LMISet = NewLMISet;
}//function


// Recreate LMI of the form <= with variables in RHS by LMIs which are tighter with constant on the RHS
DAGNode* findNewTighterLMI(DAGNode* LMI,  int WidthOfVarToElim)
{
    string LMIType = LMI->Get_node_name();
    // Obtain the RHS of LMI
    
    list<DAGNode*>::iterator iout;
    iout = LMI->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
    
    // Obtain the coefficients of the variables and the constant of the RHS part
    // i.e. given c1.x1+...+cn.xn+d, obtain {x1-->c1,...,xn--->cn} and d
    
    map<string, ULINT> VariableCoefficientMap;
    ULINT ConstantPart;
    obtainVariableCoefficientMap(RHS, VariableCoefficientMap, ConstantPart);
    
    // Find minimum k-value of the coefficient part
    ULINT MinimumCoefK = WidthOfVarToElim;
    
    for(map<string, ULINT>::iterator it=VariableCoefficientMap.begin();  it!=VariableCoefficientMap.end(); it++)
        {
        ULINT Coefficient = it->second;
        ULINT MyKValue = powerOfTwo(Coefficient); 
        if(MyKValue < MinimumCoefK)
            {
            MinimumCoefK = MyKValue;
            }
        }//for
    
    ConstantPart = ConstantPart % (findPower(MinimumCoefK));
    char ConstantPart_char[10];
    sprintf(ConstantPart_char, "%llu", ConstantPart);
    string ConstantPart_str(ConstantPart_char);
    // LHS <= ConstantPart is a tighter LMI
    DAGNode *NewRHS = createBVConstant(ConstantPart_str, WidthOfVarToElim); 
    
    DAGNode *NewLMI = createDAG(LMIType, LHS, NewRHS, LOGICAL_SYMBOL, 1);
    return NewLMI;
}


void divideConjunctionToFreeAndBoundParts(DAGNode *dag, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QFreePart, DAGNode* &BoundPart)
{
    set<DAGNode*> Conjuncts;
    splitConjunctionIntoSetGeneral(dag, Conjuncts);
    
    set<DAGNode*> Free_Constraints;
    set<DAGNode*> Bound_Constraints;
    
    Free_Constraints.clear();
    Bound_Constraints.clear();
    
    vector<DAGNode*> Conjuncts_Vector;
    for(set<DAGNode*>::iterator it=Conjuncts.begin(); it != Conjuncts.end(); it++)
        {
        Conjuncts_Vector.push_back(*it);
        }
    
    getFreeAndBoundConstraintsForMultipleVariables(Conjuncts_Vector, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
    
    QFreePart = getConjunctionFromSet(Free_Constraints);
    BoundPart = getConjunctionFromSet(Bound_Constraints);
}

//NewConstraint is lmi's RHS \neq (2^p)-1
DAGNode* getNewConstraintFromBVUGTConstraint(DAGNode *LMI, ULINT ModWidthOfVarToElim, ULINT WidthOfVarToElim)
{
    list<DAGNode*>::iterator iout;
    iout = LMI->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
    
    ULINT MaxValue = ModWidthOfVarToElim - 1;
    char MaxValue_char[10];
    sprintf(MaxValue_char, "%llu", MaxValue);
    string MaxValue_str(MaxValue_char);
    DAGNode *MaxValue_dag = createBVConstant(MaxValue_str, WidthOfVarToElim);
    
    DAGNode *NewConstraint = createDAG("is_not_equal", RHS, MaxValue_dag, LOGICAL_SYMBOL, 1);
    return NewConstraint;
}


// Recreate LMI of the form > with variables in RHS by LMIs which are tighter with constant on the RHS
DAGNode* findNewTighterLMIForBVUGT(DAGNode* LMI,  int WidthOfVarToElim)
{
    string LMIType = LMI->Get_node_name();
    // Obtain the RHS of LMI
    
    list<DAGNode*>::iterator iout;
    iout = LMI->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
    
    ULINT ModWidthOfVarToElim = findPower(WidthOfVarToElim);
    
    ULINT MaxValue = ModWidthOfVarToElim - 2;
    char MaxValue_char[10];
    sprintf(MaxValue_char, "%llu", MaxValue);
    string MaxValue_str(MaxValue_char);
     
    // LHS > ModWidthOfVarToElim - 2 is a tighter LMI
    DAGNode *NewRHS = createBVConstant(MaxValue_str, WidthOfVarToElim);     
    DAGNode *NewLMI = createDAG(LMIType, LHS, NewRHS, LOGICAL_SYMBOL, 1);
    return NewLMI;
}

// Splitting and QE using Fourier-Motzkin for multiple variables
DAGNode* expensiveLayersOnMultipleVariable(DAGNode* Phi, vector<string> VarsToElim, map<string, int> &WidthTable)
{
 if(VarsToElim.size()==1)// Only a single variable to be eliminated-Terminating case
    {
     set<DAGNode*> Constraints;
     
     splitConjunctionIntoSetGeneral(Phi, Constraints);
     
      string VariableToElim = VarsToElim[0];     
      //cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      set<DAGNode*> Free_Constraints;
      set<DAGNode*> Bound_Constraints;
      
      Free_Constraints.clear();
      Bound_Constraints.clear();

      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints);
      

       set<DAGNode*> LMESet;
       set<DAGNode*> LMDSet;
       set<DAGNode*> LMISet;
       
       splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
       
       DAGNode* QElimBoundPart;
       
       if(!LMESet.empty())
       {
         QElimBoundPart = callMatrixQEliminatorForLMCsAndLMIs(LMESet, LMDSet, LMISet, VariableToElim, WidthTable);
         set<string> QElimBoundPart_Support;
         getDAGNames(QElimBoundPart, QElimBoundPart_Support);
         if(QElimBoundPart_Support.find(VariableToElim) != QElimBoundPart_Support.end())
         {
             cout<<"\nError in function in DAGEliminator.cpp. Even coefficient encountered for "<<VariableToElim<<endl;
             exit(1);
         }//if
       }//if 
       else
       {
         QElimBoundPart = expensiveLayersOnSingleVariable(Bound_Constraints, VariableToElim, WidthTable);
         
          if(checkTimeOut())
            {
            // Time Out !!!
            cout<<"\nWarning!!!The function expensiveLayersOnMultipleVariable in DAGEliminator.cpp has timed out. Its result is not exact\n";
            timed_out = true; // timed_out flag set
            return createLogicalConstant("true");
            }
       }    
      
      //cout<<"\nQElimBoundPart = "<<getDAGStr(QElimBoundPart)<<endl;
      
      DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
      //cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
      
      DAGNode* ResultDAG = createDAG("and", QElimBoundPart, FreePart, LOGICAL_SYMBOL, 1);
     
      return ResultDAG;      
    }//if(VarsToElimInSupport.size()==1)
 else // recursing case
 {
     set<DAGNode*> Constraints;
     
     splitConjunctionIntoSetGeneral(Phi, Constraints);
     
     string VariableToElim = VarsToElim[0];     
     cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      set<DAGNode*> Free_Constraints;
      set<DAGNode*> Bound_Constraints;
      
      Free_Constraints.clear();
      Bound_Constraints.clear();

      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints);
      
      
       set<DAGNode*> LMESet;
       set<DAGNode*> LMDSet;
       set<DAGNode*> LMISet;
       
       splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
       
       showAPSet("LMESet", LMESet);
       showAPSet("LMDSet", LMDSet);
       showAPSet("LMISet", LMISet);
       
       DAGNode* QElimBoundPart;
       
       if(!LMESet.empty())
       {
         cout<<"\nLMESet is not empty\n";  
           
         QElimBoundPart = callMatrixQEliminatorForLMCsAndLMIs(LMESet, LMDSet, LMISet, VariableToElim, WidthTable);
         set<string> QElimBoundPart_Support;
         getDAGNames(QElimBoundPart, QElimBoundPart_Support);
         if(QElimBoundPart_Support.find(VariableToElim) != QElimBoundPart_Support.end())
         {
             cout<<"\nError in function in DAGEliminator.cpp. Even coefficient encountered for "<<VariableToElim<<endl;
             exit(1);
         }//if
       }//if 
       else
       {
       QElimBoundPart = expensiveLayersOnSingleVariable(Bound_Constraints, VariableToElim, WidthTable);
       
        if(checkTimeOut())
            {
            // Time Out !!!
            cout<<"\nWarning!!!The function expensiveLayersOnMultipleVariable in DAGEliminator.cpp has timed out. Its result is not exact\n";
            timed_out = true; // timed_out flag set
            return createLogicalConstant("true");
            }
       }
       
      cout<<"\nQElimBoundPart = "<<getDAGStr(QElimBoundPart)<<endl;
      
      DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
      cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
      
     //QElimBoundPart is a disjunction of conjunctions
     set<DAGNode*> QElimBoundPart_Components;
     // Get the individual component conjunctions in QElimBoundPart
     getComponentsOfDisjunction(QElimBoundPart, QElimBoundPart_Components);
     
     showAPSet("QElimBoundPart_Components", QElimBoundPart_Components);
     
     VarsToElim.erase(VarsToElim.begin()+0);
     
     cout<<"\nElimination of  "<<VariableToElim<<" is over \n";
     
     DAGNode* ResultDAG = NULL;
     
     cout<<"\nCalling expensiveLayersOnMultipleVariable on the individual problem instances\n";
     
     for(set<DAGNode*>::iterator it=QElimBoundPart_Components.begin();  it!=QElimBoundPart_Components.end(); it++)
     {
         DAGNode *Component = *it;
         DAGNode *ComponentWithFreePart = createDAG("and",Component, FreePart, LOGICAL_SYMBOL, 1);
         DAGNode *QFreeComponentWithFreePart = expensiveLayersOnMultipleVariable(ComponentWithFreePart, VarsToElim, WidthTable);
         
         if(checkTimeOut())
            {
            // Time Out !!!
            cout<<"\nWarning!!!The function expensiveLayersOnMultipleVariable in DAGEliminator.cpp has timed out. Its result is not exact\n";
            timed_out = true; // timed_out flag set
            return createLogicalConstant("true");
            }
         
         
         
         if(ResultDAG==NULL)
             ResultDAG = QFreeComponentWithFreePart;
         else
             ResultDAG = createDAG("or", ResultDAG, QFreeComponentWithFreePart, LOGICAL_SYMBOL, 1);
     }
    
     return ResultDAG;
     
 }//else ends here
}//function ends here


// Get the components of disjunction
void getComponentsOfDisjunction(DAGNode *node, set<DAGNode*> &Components)
{
    if(node->Get_node_name()=="or")
    {
        list<DAGNode*>::iterator iout;
        for(iout = node->Get_outlist_start(); iout!=node->Get_outlist_end(); iout++)
        {
         getComponentsOfDisjunction(*iout, Components);   
        }
    }// if(node->Get_node_name()=="or")
    else
    {
       Components.insert(node) ;
    }
}//function ends


// Applying splitting and Fourier-Motzkin on \exists VariableToElim. (/\Constraints)
DAGNode* expensiveLayersOnSingleVariable(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable)
{
   if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function expensiveLayersOnSingleVariable in DAGEliminator.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}
    
    
    int WidthOfVarToElim;
    ULINT ModOfVarToElim;
    
    map<string, int>::iterator wit = WidthTable.find(VarToElim);
    if(wit == WidthTable.end())
        {
        cout<<"\nNo entry for the variable "<<VarToElim<<" in WidthTable in function expensiveLayersOnSingleVariable in DAGEliminator.cpp\n";
        exit(1);
        }
    WidthOfVarToElim = wit->second;
    ModOfVarToElim = findPower(WidthOfVarToElim);
    
    //cout<<"\nWidthOfVarToElim = "<<WidthOfVarToElim<<endl;
    //cout<<"\nModOfVarToElim = "<<ModOfVarToElim<<endl;
    
    vector<DAGNode*> Constraints_Vector;
    vector< vector<DAGNode*> > SplittedConstraints;
    
    // Let us ensure that we have only <= constraints
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* Constraint = *it;
        if(Constraint->Get_node_name() != "bvule")
        {
            cout<<"\nUnsupported constraint "<<getDAGStr(Constraint)<<" in function expensiveLayersOnSingleVariable in DAGEliminator.cpp\n";
            exit(1);
        }
    }//for
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        Constraints_Vector.push_back(*it);        
    }
    
    ULINT CoefOfVarToElim;
    ULINT AIOfCoefOfVarToElim;
    
    for(int constraint_index=0; constraint_index < Constraints_Vector.size(); constraint_index++)
    {
        DAGNode *Constraint = Constraints_Vector[constraint_index];
        
        string side_of_variable = findSideOfVariableInConstraint(Constraint, VarToElim);
        //cout<<"\nside_of_variable = "<<side_of_variable<<endl;
        
        bool both_sides_implemented = true;
        if(!both_sides_implemented)
        {
               if(side_of_variable == "both")
                {
                 cout<<"\nThe variable "<<VarToElim<<" appears on both sides of "<<getDAGStr(Constraint)<<" in function expensiveLayersOnSingleVariable in DAGEliminator.cpp"<<endl;
                exit(1);
                }//if(side_of_variable == "both")
        }
        
        if(side_of_variable == "none")
        {
            cout<<"\nThe variable "<<VarToElim<<" does not appear on any side of "<<getDAGStr(Constraint)<<" in function expensiveLayersOnSingleVariable in DAGEliminator.cpp"<<endl;
            exit(1);
        }//if(side_of_variable == "both")
        
        string type_of_coefficient = "original";
        if(constraint_index==0)//first constraint
        {
            getCoefOfVarToElimAndAIOfCoefOfVarToElim(Constraint, VarToElim, side_of_variable, ModOfVarToElim, CoefOfVarToElim, AIOfCoefOfVarToElim);
            //cout<<"\nCoefOfVarToElim = "<<CoefOfVarToElim<<", AIOfCoefOfVarToElim = "<<AIOfCoefOfVarToElim<<endl;
        }//if(constr_index==0)
        else
        {
            type_of_coefficient = findTypeOfCoefficient(Constraint, VarToElim, side_of_variable, CoefOfVarToElim, AIOfCoefOfVarToElim);
            //cout<<"\ntype_of_coefficient = "<<type_of_coefficient<<endl;
        }// else of if(constr_index==0)
        
             
        vector<DAGNode*> SplittedVector;
        
        
        if(side_of_variable == "left" || side_of_variable == "right")
        {
           obtainSplittedVector(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector);
         }
        else if(side_of_variable == "both")
        {
          obtainSplittedVectorForLMIsWithBothSides(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector);
        }
        else
        {
          cout<<"\nUnexpected side_of_variable "<<side_of_variable<<" in function expensiveLayersOnSingleVariable in DAGEliminator.cpp\n";
          exit(1);  
        }
         SplittedConstraints.push_back(SplittedVector);        
    }//for ends
    
    
    // Let us apply distributive law to SplittedConstraints in order to obatin the constraints in DNF
    // Note that this portion can be improved using Monniuax style to obtain DNF
    
    vector<DAGNode*> DNF;
    
    bool obtain_DNF_using_distributivity = true;
    
    if(obtain_DNF_using_distributivity)
    {
        obtainDNFUsingDistributivity(SplittedConstraints, DNF);
    }
    else
    {
        cout<<"\nError in function expensiveLayersOnSingleVariable in DAGEliminator.cpp.  Presently the only way to obtain DNF is using distributivity\n";
        exit(1);
    }
    
    //showAPVector("DNF", DNF);
    //cout<<"\nDNF.size() == "<<DNF.size()<<endl;
    
    set<DAGNode*> QEliminatedDNF;
    eliminateQuantifiersUsingFourierMotzkin(DNF, VarToElim, WidthTable, QEliminatedDNF);    
    
    // Take disjunction of constraints in QEliminatedDNF 
   DAGNode* resultDAG;
   resultDAG = getDisjunctionFromSet(QEliminatedDNF);
   //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;
   return resultDAG;
    
}//function ends



// Version of obtainSplittedVector which works on LMIs with variable to be eliminated on both sides
void obtainSplittedVectorForLMIsWithBothSides(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector)
{    
    list<string> VarNames;
    VarNames.push_back(VarToElim);
    set<string> Support;
    getDAGNames(Constraint, Support);
    attachSetToList(Support, VarNames);
    
    //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *S = *iout;
    iout++;
    DAGNode *T = *iout;	
    
    getCoefsAndConstsModified(S, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(T, VarNames, T_Coef_Mat, T_Const_Mat);
    
    //showList(VarNames, "VarNames");
    //cout<<"\nS_Coef_Mat\n";
    //showMatrix(S_Coef_Mat);
    //cout<<"\nS_Const_Mat\n";
    //showMatrix(S_Const_Mat);
    //cout<<"\nT_Coef_Mat\n";
    //showMatrix(T_Coef_Mat);
    //cout<<"\nT_Const_Mat\n";
    //showMatrix(T_Const_Mat);
    
    Matrix E_Coef_Mat, E_Const_Mat, F_Coef_Mat, F_Const_Mat;
    Matrix Var_Coef_Mat, Var_Const_Mat;
    ULINT E, F;
    
    copyMatrix(S_Coef_Mat, E_Coef_Mat);
    copyMatrix(S_Const_Mat, E_Const_Mat);
    nullifyColumn(E_Coef_Mat, 0);
    E = E_Const_Mat[0][0];
    
    copyMatrix(T_Coef_Mat, F_Coef_Mat);
    copyMatrix(T_Const_Mat, F_Const_Mat);
    nullifyColumn(F_Coef_Mat, 0);
    F = F_Const_Mat[0][0];
    
    copyMatrix(S_Coef_Mat, Var_Coef_Mat);
    nullifyColumnsExceptOne(Var_Coef_Mat, 0);
    getNullMatrix(Var_Const_Mat, 1, 1);
    
    
      if(IsNullMatrix(E_Coef_Mat)==0 || IsNullMatrix(F_Coef_Mat)==0)
    {
        cout<<"\nDifference between the LHS and RHS of "<<getDAGStr(Constraint)<<" is not a constant\n";
        exit(1);
    }  
    
    if(E == 0 && F==0)
    {
        cout<<"\nNo need to reconstruct "<<getDAGStr(Constraint)<<" as both the LHS and RHS are the same\n";
        exit(1);
    }
    
    if(E > F)
    {
        if(F==0)
        {
            DAGNode* ReconstructedConstraint;
            aInverseOfMatrix(E_Const_Mat, ModOfVarToElim);
            vector<string> LMITypes;
            LMITypes.push_back("bvule");
            ReconstructedConstraint = quickSimplify(getDAGFromInequations(E_Coef_Mat, E_Const_Mat, Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
            //cout<<"\nReconstructedConstraint = "<<getDAGStr(ReconstructedConstraint)<<endl;
            //cout<<"\nPress any key to continue\n";
            //char c = getchar();
            obtainSplittedVector(ReconstructedConstraint, VarToElim, WidthTable, "right", type_of_coefficient, ModOfVarToElim, SplittedVector);
         }// if(F==0)
        else
        {
            vector<DAGNode*> SplittedVector1;
            vector<DAGNode*> SplittedVector2;
            
            aInverseOfMatrix(E_Const_Mat, ModOfVarToElim);
            
            ULINT AI_of_F = aInverse(F, ModOfVarToElim);
            ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
            Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
            getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
            getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
            AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
            
            vector<string> LMITypes;
            LMITypes.push_back("bvule");
            DAGNode* ReconstructedConstraint1 = quickSimplify(getDAGFromInequations(E_Coef_Mat, E_Const_Mat, Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
            //cout<<"\nReconstructedConstraint1 = "<<getDAGStr(ReconstructedConstraint1)<<endl;
            DAGNode* ReconstructedConstraint2 = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
            //cout<<"\nReconstructedConstraint2 = "<<getDAGStr(ReconstructedConstraint2)<<endl;
            
            //cout<<"\nPress any key to continue\n";
            //char c = getchar();
            obtainSplittedVector(ReconstructedConstraint1, VarToElim, WidthTable, "right", type_of_coefficient, ModOfVarToElim, SplittedVector1);
            obtainSplittedVector(ReconstructedConstraint2, VarToElim, WidthTable, "left", type_of_coefficient, ModOfVarToElim, SplittedVector2);
            
            vector< vector<DAGNode*> > SplittedConstraints;
            SplittedConstraints.push_back(SplittedVector1);
            SplittedConstraints.push_back(SplittedVector2);
           
            bool obtain_DNF_using_distributivity = true;
    
            if(obtain_DNF_using_distributivity)
                {
                obtainDNFUsingDistributivity(SplittedConstraints, SplittedVector);
                }
            else
                {
                cout<<"\nError in function obtainSplittedVectorForLMIsWithBothSides in DAGEliminator.cpp.  Presently the only way to obtain DNF is using distributivity\n";
                exit(1);
                }
         }// else of if(F==0)
    }// if(E > F)
    else
    {
        if(E==0)
        {
                     
            ULINT AI_of_F = aInverse(F, ModOfVarToElim);
            ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
            Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
            getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
            getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
            AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
            
            vector<string> LMITypes;
            LMITypes.push_back("bvule");

            DAGNode* ReconstructedConstraint = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
            //cout<<"\nReconstructedConstraint = "<<getDAGStr(ReconstructedConstraint)<<endl;
         
            //cout<<"\nPress any key to continue\n";
            //char c = getchar();
            obtainSplittedVector(ReconstructedConstraint, VarToElim, WidthTable, "left", type_of_coefficient, ModOfVarToElim, SplittedVector);
         }// if(E==0)
        else
        {
            vector<DAGNode*> SplittedVector1;
            vector<DAGNode*> SplittedVector2;
            
            aInverseOfMatrix(E_Const_Mat, ModOfVarToElim);
            
            ULINT AI_of_F = aInverse(F, ModOfVarToElim);
            ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
            Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
            getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
            getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
            AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
            
            vector<string> LMITypes;
            LMITypes.push_back("bvule");
            DAGNode* ReconstructedConstraint1 = quickSimplify(getDAGFromInequations(E_Coef_Mat, E_Const_Mat, Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
            //cout<<"\nReconstructedConstraint1 = "<<getDAGStr(ReconstructedConstraint1)<<endl;
            DAGNode* ReconstructedConstraint2 = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
            //cout<<"\nReconstructedConstraint2 = "<<getDAGStr(ReconstructedConstraint2)<<endl;
            
            //cout<<"\nPress any key to continue\n";
            //char c = getchar();
            obtainSplittedVector(ReconstructedConstraint1, VarToElim, WidthTable, "right", type_of_coefficient, ModOfVarToElim, SplittedVector1);
            obtainSplittedVector(ReconstructedConstraint2, VarToElim, WidthTable, "left", type_of_coefficient, ModOfVarToElim, SplittedVector2);
            
           for(int i=0; i<SplittedVector1.size(); i++)
            {
                SplittedVector.push_back(SplittedVector1[i]);
            }
           for(int i=0; i<SplittedVector2.size(); i++)
            {
                SplittedVector.push_back(SplittedVector2[i]);
            }
         }// else of if(E==0)
    }//else of if(E > F)
}//end of function


// Split the Constraint into disjunctions based on the OVF idea
void obtainSplittedVector(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector)
{    
    list<string> VarNames;
    VarNames.push_back(VarToElim);
    set<string> Support;
    getDAGNames(Constraint, Support);
    attachSetToList(Support, VarNames);
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *S = *iout;
    iout++;
    DAGNode *T = *iout;	
    
    getCoefsAndConstsModified(S, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(T, VarNames, T_Coef_Mat, T_Const_Mat);
    
    //showList(VarNames, "VarNames");
    //cout<<"\nS_Coef_Mat\n";
    //showMatrix(S_Coef_Mat);
    //cout<<"\nS_Const_Mat\n";
    //showMatrix(S_Const_Mat);
    //cout<<"\nT_Coef_Mat\n";
    //showMatrix(T_Coef_Mat);
    //cout<<"\nT_Const_Mat\n";
    //showMatrix(T_Const_Mat);
    
    Matrix Delta_Coef_Mat, Delta_Const_Mat, AIDelta_Coef_Mat, AIDelta_Const_Mat;
    
    
    if(side_of_variable=="left")
    {
        copyMatrix(S_Coef_Mat, Delta_Coef_Mat);
        copyMatrix(S_Const_Mat, Delta_Const_Mat);
        nullifyColumn(Delta_Coef_Mat, 0);
        aInverseOfMatrix(Delta_Coef_Mat, ModOfVarToElim);
        aInverseOfMatrix(Delta_Const_Mat, ModOfVarToElim);
    
        copyMatrix(Delta_Coef_Mat, AIDelta_Coef_Mat);
        aInverseOfMatrix(AIDelta_Coef_Mat, ModOfVarToElim);
        copyMatrix(S_Const_Mat, AIDelta_Const_Mat);
    }
    else  if(side_of_variable=="right")
    {
        copyMatrix(T_Coef_Mat, Delta_Coef_Mat);
        copyMatrix(T_Const_Mat, Delta_Const_Mat);
        nullifyColumn(Delta_Coef_Mat, 0);
        aInverseOfMatrix(Delta_Coef_Mat, ModOfVarToElim);
        aInverseOfMatrix(Delta_Const_Mat, ModOfVarToElim);
    
        copyMatrix(Delta_Coef_Mat, AIDelta_Coef_Mat);
        aInverseOfMatrix(AIDelta_Coef_Mat, ModOfVarToElim);
        copyMatrix(T_Const_Mat, AIDelta_Const_Mat);
    }
    else
    {
         cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function obtainSplittedVector in DAGEliminator.cpp\n";
          exit(1);
    }
    
    //cout<<"\nDelta_Coef_Mat\n";
    //showMatrix(Delta_Coef_Mat);
    //cout<<"\nDelta_Const_Mat\n";
    //showMatrix(Delta_Const_Mat);
    
    Matrix TPlusDelta_Coef_Mat, TPlusDelta_Const_Mat;
    Matrix SPlusDelta_Coef_Mat, SPlusDelta_Const_Mat;
    Matrix DeltaPlusOne_Coef_Mat, DeltaPlusOne_Const_Mat;
    Matrix DeltaMinusOne_Coef_Mat, DeltaMinusOne_Const_Mat;
    Matrix TPlusDeltaPlusOne_Coef_Mat, TPlusDeltaPlusOne_Const_Mat;
    Matrix SPlusDeltaMinusOne_Coef_Mat, SPlusDeltaMinusOne_Const_Mat;
    
    Matrix AITPlusDelta_Coef_Mat, AITPlusDelta_Const_Mat;
    Matrix AISPlusDelta_Coef_Mat, AISPlusDelta_Const_Mat;
    Matrix AIDeltaPlusOne_Coef_Mat, AIDeltaPlusOne_Const_Mat;
    Matrix AIDeltaMinusOne_Coef_Mat, AIDeltaMinusOne_Const_Mat;
    Matrix AITPlusDeltaPlusOne_Coef_Mat, AITPlusDeltaPlusOne_Const_Mat;
    Matrix AISPlusDeltaMinusOne_Coef_Mat, AISPlusDeltaMinusOne_Const_Mat;
    
    Matrix One_Coef_Mat, One_Const_Mat, MinusOne_Coef_Mat, MinusOne_Const_Mat;
    Matrix Zero_Coef_Mat, Zero_Const_Mat;
    
    getNullMatrix(One_Coef_Mat, 1, VarNames.size());
    getNullMatrix(MinusOne_Coef_Mat, 1, VarNames.size());
    getNullMatrix(One_Const_Mat, 1, 1);
    One_Const_Mat[0][0]=1;
    getNullMatrix(MinusOne_Const_Mat, 1, 1);
    MinusOne_Const_Mat[0][0]=ModOfVarToElim-1;
    getNullMatrix(Zero_Coef_Mat, 1, VarNames.size());
    getNullMatrix(Zero_Const_Mat, 1, 1);
    
    // Finding S+Delta
    copyMatrix(S_Coef_Mat, SPlusDelta_Coef_Mat);
    copyMatrix(S_Const_Mat, SPlusDelta_Const_Mat);
    matAdd(SPlusDelta_Coef_Mat, Delta_Coef_Mat, ModOfVarToElim);
    matAdd(SPlusDelta_Const_Mat, Delta_Const_Mat, ModOfVarToElim);
    
    // Finding T+Delta
    copyMatrix(T_Coef_Mat, TPlusDelta_Coef_Mat);
    copyMatrix(T_Const_Mat, TPlusDelta_Const_Mat);
    matAdd(TPlusDelta_Coef_Mat, Delta_Coef_Mat, ModOfVarToElim);
    matAdd(TPlusDelta_Const_Mat, Delta_Const_Mat, ModOfVarToElim);
    
     // Finding Delta+1
    copyMatrix(Delta_Coef_Mat, DeltaPlusOne_Coef_Mat);
    copyMatrix(Delta_Const_Mat, DeltaPlusOne_Const_Mat);
    matAdd(DeltaPlusOne_Coef_Mat, One_Coef_Mat, ModOfVarToElim);
    matAdd(DeltaPlusOne_Const_Mat, One_Const_Mat, ModOfVarToElim);
    
    // Finding Delta-1
    copyMatrix(Delta_Coef_Mat, DeltaMinusOne_Coef_Mat);
    copyMatrix(Delta_Const_Mat, DeltaMinusOne_Const_Mat);
    matAdd(DeltaMinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
    matAdd(DeltaMinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
    
    
    // Finding T+Delta+1
    copyMatrix(TPlusDelta_Coef_Mat, TPlusDeltaPlusOne_Coef_Mat);
    copyMatrix(TPlusDelta_Const_Mat, TPlusDeltaPlusOne_Const_Mat);
    matAdd(TPlusDeltaPlusOne_Coef_Mat, One_Coef_Mat, ModOfVarToElim);
    matAdd(TPlusDeltaPlusOne_Const_Mat, One_Const_Mat, ModOfVarToElim);
    
    
    
    // Finding S+Delta-1
    copyMatrix(SPlusDelta_Coef_Mat, SPlusDeltaMinusOne_Coef_Mat);
    copyMatrix(SPlusDelta_Const_Mat, SPlusDeltaMinusOne_Const_Mat);
    matAdd(SPlusDeltaMinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
    matAdd(SPlusDeltaMinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
    
    
    // Finding AI(S+Delta)
    copyMatrix(SPlusDelta_Coef_Mat, AISPlusDelta_Coef_Mat);
    copyMatrix(SPlusDelta_Const_Mat, AISPlusDelta_Const_Mat);
    aInverseOfMatrix(AISPlusDelta_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AISPlusDelta_Const_Mat, ModOfVarToElim);
    
    // Finding AI(T+Delta)
    copyMatrix(TPlusDelta_Coef_Mat, AITPlusDelta_Coef_Mat);
    copyMatrix(TPlusDelta_Const_Mat, AITPlusDelta_Const_Mat);
    aInverseOfMatrix(AITPlusDelta_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AITPlusDelta_Const_Mat, ModOfVarToElim);
      
     // Finding AI(Delta+1)
    copyMatrix(DeltaPlusOne_Coef_Mat, AIDeltaPlusOne_Coef_Mat);
    copyMatrix(DeltaPlusOne_Const_Mat, AIDeltaPlusOne_Const_Mat);
    aInverseOfMatrix(AIDeltaPlusOne_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AIDeltaPlusOne_Const_Mat, ModOfVarToElim);
    
     // Finding AI(Delta-1)
    copyMatrix(DeltaMinusOne_Coef_Mat, AIDeltaMinusOne_Coef_Mat);
    copyMatrix(DeltaMinusOne_Const_Mat, AIDeltaMinusOne_Const_Mat);
    aInverseOfMatrix(AIDeltaMinusOne_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AIDeltaMinusOne_Const_Mat, ModOfVarToElim);
    
    
    // Finding AI(T+Delta+1)
    copyMatrix(TPlusDeltaPlusOne_Coef_Mat, AITPlusDeltaPlusOne_Coef_Mat);
    copyMatrix(TPlusDeltaPlusOne_Const_Mat, AITPlusDeltaPlusOne_Const_Mat);
    aInverseOfMatrix(AITPlusDeltaPlusOne_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AITPlusDeltaPlusOne_Const_Mat, ModOfVarToElim);
    
    // Finding AI(S+Delta-1)
    copyMatrix(SPlusDeltaMinusOne_Coef_Mat, AISPlusDeltaMinusOne_Coef_Mat);
    copyMatrix(SPlusDeltaMinusOne_Const_Mat, AISPlusDeltaMinusOne_Const_Mat);
    aInverseOfMatrix(AISPlusDeltaMinusOne_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AISPlusDeltaMinusOne_Const_Mat, ModOfVarToElim);
    
    
    //cout<<"\nTPlusDelta_Coef_Mat\n"; showMatrix(TPlusDelta_Coef_Mat);
    //cout<<"\nTPlusDelta_Const_Mat\n"; showMatrix(TPlusDelta_Const_Mat);
    
    //cout<<"\nSPlusDelta_Coef_Mat\n"; showMatrix(SPlusDelta_Coef_Mat);
    //cout<<"\nSPlusDelta_Const_Mat\n"; showMatrix(SPlusDelta_Const_Mat);
    
    //cout<<"\nDeltaPlusOne_Coef_Mat\n"; showMatrix(DeltaPlusOne_Coef_Mat);
    //cout<<"\nDeltaPlusOne_Const_Mat\n"; showMatrix(DeltaPlusOne_Const_Mat);
    
    //cout<<"\nDeltaMinusOne_Coef_Mat\n"; showMatrix(DeltaMinusOne_Coef_Mat);
    //cout<<"\nDeltaMinusOne_Const_Mat\n"; showMatrix(DeltaMinusOne_Const_Mat);
    
    //cout<<"\nTPlusDeltaPlusOne_Coef_Mat\n"; showMatrix(TPlusDeltaPlusOne_Coef_Mat);
    //cout<<"\nTPlusDeltaPlusOne_Const_Mat\n"; showMatrix(TPlusDeltaPlusOne_Const_Mat);
    
    //cout<<"\nSPlusDeltaMinusOne_Coef_Mat\n"; showMatrix(SPlusDeltaMinusOne_Coef_Mat);
    //cout<<"\nSPlusDeltaMinusOne_Const_Mat\n"; showMatrix(SPlusDeltaMinusOne_Const_Mat);
        
    
    //cout<<"\nAITPlusDelta_Coef_Mat\n"; showMatrix(AITPlusDelta_Coef_Mat);
    //cout<<"\nAITPlusDelta_Const_Mat\n"; showMatrix(AITPlusDelta_Const_Mat);
    
    //cout<<"\nAISPlusDelta_Coef_Mat\n"; showMatrix(AISPlusDelta_Coef_Mat);
    //cout<<"\nAISPlusDelta_Const_Mat\n"; showMatrix(AISPlusDelta_Const_Mat);
    
    //cout<<"\nAIDeltaPlusOne_Coef_Mat\n"; showMatrix(AIDeltaPlusOne_Coef_Mat);
    //cout<<"\nAIDeltaPlusOne_Const_Mat\n"; showMatrix(AIDeltaPlusOne_Const_Mat);
    
    //cout<<"\nAIDeltaMinusOne_Coef_Mat\n"; showMatrix(AIDeltaMinusOne_Coef_Mat);
    //cout<<"\nAIDeltaMinusOne_Const_Mat\n"; showMatrix(AIDeltaMinusOne_Const_Mat);
    
    //cout<<"\nAITPlusDeltaPlusOne_Coef_Mat\n"; showMatrix(AITPlusDeltaPlusOne_Coef_Mat);
    //cout<<"\nAITPlusDeltaPlusOne_Const_Mat\n"; showMatrix(AITPlusDeltaPlusOne_Const_Mat);
    
    //cout<<"\nAISPlusDeltaMinusOne_Coef_Mat\n"; showMatrix(AISPlusDeltaMinusOne_Coef_Mat);
    //cout<<"\nAISPlusDeltaMinusOne_Const_Mat\n"; showMatrix(AISPlusDeltaMinusOne_Const_Mat);    
    
    DAGNode *DeltaEqZero, *OneLteDelta, *DeltaLteTPlusDelta, *SPlusDeltaLteTPlusDelta;
    DAGNode *SPlusDeltaLteDeltaMinusOne, *DeltaLteSPlusDelta, *TPlusDeltaLteDeltaMinusOne;
    DAGNode *TPlusDeltaLteSPlusDeltaMinusOne, *TPlusDeltaPlusOneLteSPlusDelta;
    
    vector<string> LMITypes;
    LMITypes.push_back("bvule");
    
    DeltaEqZero = quickSimplify(getDAGFromEqs(Delta_Coef_Mat, AIDelta_Const_Mat, VarNames, ModOfVarToElim));
    //cout<<"\nDeltaEqZero = "<<getDAGStr(DeltaEqZero)<<endl;
    
    OneLteDelta = quickSimplify(getDAGFromInequations(One_Coef_Mat, One_Const_Mat, Delta_Coef_Mat, Delta_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nOneLteDelta = "<<getDAGStr(OneLteDelta)<<endl;
    
    DeltaLteTPlusDelta = quickSimplify(getDAGFromInequations(Delta_Coef_Mat, Delta_Const_Mat, TPlusDelta_Coef_Mat, TPlusDelta_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nDeltaLteTPlusDelta = "<<getDAGStr(DeltaLteTPlusDelta)<<endl;
    
    SPlusDeltaLteTPlusDelta = quickSimplify(getDAGFromInequations(SPlusDelta_Coef_Mat, SPlusDelta_Const_Mat, TPlusDelta_Coef_Mat, TPlusDelta_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nSPlusDeltaLteTPlusDelta = "<<getDAGStr(SPlusDeltaLteTPlusDelta)<<endl;
    
    SPlusDeltaLteDeltaMinusOne = quickSimplify(getDAGFromInequations(SPlusDelta_Coef_Mat, SPlusDelta_Const_Mat, DeltaMinusOne_Coef_Mat, DeltaMinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nSPlusDeltaLteDeltaMinusOne = "<<getDAGStr(SPlusDeltaLteDeltaMinusOne)<<endl;
    
    DeltaLteSPlusDelta = quickSimplify(getDAGFromInequations(Delta_Coef_Mat, Delta_Const_Mat, SPlusDelta_Coef_Mat, SPlusDelta_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nDeltaLteSPlusDelta = "<<getDAGStr(DeltaLteSPlusDelta)<<endl;
    
    TPlusDeltaLteDeltaMinusOne = quickSimplify(getDAGFromInequations(TPlusDelta_Coef_Mat, TPlusDelta_Const_Mat, DeltaMinusOne_Coef_Mat, DeltaMinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nTPlusDeltaLteDeltaMinusOne = "<<getDAGStr(TPlusDeltaLteDeltaMinusOne)<<endl;
    
    TPlusDeltaLteSPlusDeltaMinusOne = quickSimplify(getDAGFromInequations(TPlusDelta_Coef_Mat, TPlusDelta_Const_Mat, SPlusDeltaMinusOne_Coef_Mat, SPlusDeltaMinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nTPlusDeltaLteSPlusDeltaMinusOne = "<<getDAGStr(TPlusDeltaLteSPlusDeltaMinusOne)<<endl;
    
    TPlusDeltaPlusOneLteSPlusDelta = quickSimplify(getDAGFromInequations(TPlusDeltaPlusOne_Coef_Mat, TPlusDeltaPlusOne_Const_Mat, SPlusDelta_Coef_Mat, SPlusDelta_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nTPlusDeltaPlusOneLteSPlusDelta = "<<getDAGStr(TPlusDeltaPlusOneLteSPlusDelta)<<endl;
    
    DAGNode *AITPlusDeltaLteAISPlusDelta;
    DAGNode *AIDeltaMinusOneLteAISPlusDelta, *AISPlusDeltaLteAIDelta;
    DAGNode *AISPlusDeltaMinusOneLteAITPlusDelta, *AISPlusDeltaLteAITPlusDeltaPlusOne;    
    
    AITPlusDeltaLteAISPlusDelta = quickSimplify(getDAGFromInequations(AITPlusDelta_Coef_Mat, AITPlusDelta_Const_Mat, AISPlusDelta_Coef_Mat, AISPlusDelta_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nAITPlusDeltaLteAISPlusDelta = "<<getDAGStr(AITPlusDeltaLteAISPlusDelta)<<endl;
    
    AIDeltaMinusOneLteAISPlusDelta = quickSimplify(getDAGFromInequations(AIDeltaMinusOne_Coef_Mat, AIDeltaMinusOne_Const_Mat, AISPlusDelta_Coef_Mat, AISPlusDelta_Const_Mat,  VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nAIDeltaMinusOneLteAISPlusDelta = "<<getDAGStr(AIDeltaMinusOneLteAISPlusDelta)<<endl;
    
    AISPlusDeltaLteAIDelta = quickSimplify(getDAGFromInequations(AISPlusDelta_Coef_Mat, AISPlusDelta_Const_Mat, AIDelta_Coef_Mat, AIDelta_Const_Mat,  VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nAISPlusDeltaLteAIDelta = "<<getDAGStr(AISPlusDeltaLteAIDelta)<<endl;    
    
    AISPlusDeltaMinusOneLteAITPlusDelta = quickSimplify(getDAGFromInequations(AISPlusDeltaMinusOne_Coef_Mat, AISPlusDeltaMinusOne_Const_Mat, AITPlusDelta_Coef_Mat, AITPlusDelta_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nAISPlusDeltaMinusOneLteAITPlusDelta = "<<getDAGStr(AISPlusDeltaMinusOneLteAITPlusDelta)<<endl;
        
    AISPlusDeltaLteAITPlusDeltaPlusOne = quickSimplify(getDAGFromInequations(AISPlusDelta_Coef_Mat, AISPlusDelta_Const_Mat, AITPlusDeltaPlusOne_Coef_Mat, AITPlusDeltaPlusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
    //cout<<"\nAISPlusDeltaLteAITPlusDeltaPlusOne = "<<getDAGStr(AISPlusDeltaLteAITPlusDeltaPlusOne)<<endl;
    
    //cout<<"\nside_of_variable = "<<side_of_variable<<", type_of_coefficient = "<<type_of_coefficient<<endl;
    
    if(type_of_coefficient == "original")
    {
        
        if(isTrue(DeltaEqZero)) // Delta=0 is true. The only constraint is s<=t
        {
            SplittedVector.push_back(Constraint);
        }
        else
        {
            if(!isFalse(DeltaEqZero)) //Delta = 0 is not False. Insert Delta=0
            {
               SplittedVector.push_back(DeltaEqZero); 
            }
            
            DAGNode *Constraint1, *Constraint2, *Constraint3;
            vector<DAGNode*> Constraint1_Children;
            vector<DAGNode*> Constraint2_Children;
            vector<DAGNode*> Constraint3_Children;
            
             if(!isFalse(OneLteDelta)) //1<=Delta is not False. Form and insert the constraints
            {
               if(!isTrue(OneLteDelta))
               {
                   Constraint1_Children.push_back(OneLteDelta);
                   Constraint2_Children.push_back(OneLteDelta);
                   Constraint3_Children.push_back(OneLteDelta);
               }
               Constraint1_Children.push_back(DeltaLteTPlusDelta);
               Constraint1_Children.push_back(SPlusDeltaLteTPlusDelta);
               
               Constraint2_Children.push_back(SPlusDeltaLteDeltaMinusOne);
               Constraint2_Children.push_back(SPlusDeltaLteTPlusDelta);
               
               Constraint3_Children.push_back(DeltaLteSPlusDelta);
               Constraint3_Children.push_back(TPlusDeltaLteDeltaMinusOne);
               
               if(side_of_variable == "left")
               {
                   Constraint3_Children.push_back(TPlusDeltaPlusOneLteSPlusDelta);
               }
               else if(side_of_variable == "right")
               {
                   Constraint3_Children.push_back(TPlusDeltaLteSPlusDeltaMinusOne);
               }
               else
               {
                   cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function obtainSplittedVector in DAGEliminator.cpp\n";
                   exit(1);
               }
               
               Constraint1 = NULL;
               for(int i=0; i<Constraint1_Children.size(); i++)
                {
                  if(Constraint1==NULL) 
                  {
                      Constraint1 = Constraint1_Children[i];
                  }
                  else
                  {
                      Constraint1 = createDAG("and", Constraint1_Children[i], Constraint1, LOGICAL_SYMBOL, 1);
                  }
                }//for
               
               Constraint2 = NULL;
               for(int i=0; i<Constraint2_Children.size(); i++)
                {
                  if(Constraint2==NULL) 
                  {
                      Constraint2 = Constraint2_Children[i];
                  }
                  else
                  {
                      Constraint2 = createDAG("and", Constraint2_Children[i], Constraint2, LOGICAL_SYMBOL, 1);
                  }
                }//for
               
               Constraint3 = NULL;
               for(int i=0; i<Constraint3_Children.size(); i++)
                {
                  if(Constraint3==NULL) 
                  {
                      Constraint3 = Constraint3_Children[i];
                  }
                  else
                  {
                      Constraint3 = createDAG("and", Constraint3_Children[i], Constraint3, LOGICAL_SYMBOL, 1);
                  }
                }//for
               
               SplittedVector.push_back(Constraint1); 
               SplittedVector.push_back(Constraint2); 
               SplittedVector.push_back(Constraint3);                
            }//if(!isFalse(OneLteDelta))
            
        }//else of if(isTrue(DeltaEqZero))
        
    }// if(type_of_coefficient == "original")
    
    else if(type_of_coefficient == "negated")
    {
        
        if(isTrue(DeltaEqZero)) // Delta=0 is true. The only constraint is s<=t
        {
            SplittedVector.push_back(Constraint);
        }
        else
        {
            if(!isFalse(DeltaEqZero)) //Delta = 0 is not False. Insert Delta=0
            {
               SplittedVector.push_back(DeltaEqZero); 
            }
            
            DAGNode *Constraint1, *Constraint2, *Constraint3;
            vector<DAGNode*> Constraint1_Children;
            vector<DAGNode*> Constraint2_Children;
            vector<DAGNode*> Constraint3_Children;
            
             if(!isFalse(OneLteDelta)) //1<=Delta is not False. Form and insert the constraints
            {
               if(!isTrue(OneLteDelta))
               {
                   Constraint1_Children.push_back(OneLteDelta);
                   Constraint2_Children.push_back(OneLteDelta);
                   Constraint3_Children.push_back(OneLteDelta);
               }
               Constraint1_Children.push_back(DeltaLteTPlusDelta);
               Constraint1_Children.push_back(AITPlusDeltaLteAISPlusDelta);
               
               Constraint2_Children.push_back(AIDeltaMinusOneLteAISPlusDelta);
               Constraint2_Children.push_back(AITPlusDeltaLteAISPlusDelta);
               
               Constraint3_Children.push_back(AISPlusDeltaLteAIDelta);
               Constraint3_Children.push_back(TPlusDeltaLteDeltaMinusOne);
               
               if(side_of_variable == "left")
               {
                   Constraint3_Children.push_back(AISPlusDeltaLteAITPlusDeltaPlusOne);
               }
               else if(side_of_variable == "right")
               {
                   Constraint3_Children.push_back(AISPlusDeltaMinusOneLteAITPlusDelta);
               }
               else
               {
                   cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function obtainSplittedVector in DAGEliminator.cpp\n";
                   exit(1);
               }
               
               Constraint1 = NULL;
               for(int i=0; i<Constraint1_Children.size(); i++)
                {
                  if(Constraint1==NULL) 
                  {
                      Constraint1 = Constraint1_Children[i];
                  }
                  else
                  {
                      Constraint1 = createDAG("and", Constraint1_Children[i], Constraint1, LOGICAL_SYMBOL, 1);
                  }
                }//for
               
               Constraint2 = NULL;
               for(int i=0; i<Constraint2_Children.size(); i++)
                {
                  if(Constraint2==NULL) 
                  {
                      Constraint2 = Constraint2_Children[i];
                  }
                  else
                  {
                      Constraint2 = createDAG("and", Constraint2_Children[i], Constraint2, LOGICAL_SYMBOL, 1);
                  }
                }//for
               
               Constraint3 = NULL;
               for(int i=0; i<Constraint3_Children.size(); i++)
                {
                  if(Constraint3==NULL) 
                  {
                      Constraint3 = Constraint3_Children[i];
                  }
                  else
                  {
                      Constraint3 = createDAG("and", Constraint3_Children[i], Constraint3, LOGICAL_SYMBOL, 1);
                  }
                }//for
               
               SplittedVector.push_back(Constraint1); 
               SplittedVector.push_back(Constraint2); 
               SplittedVector.push_back(Constraint3);                
            }//if(!isFalse(OneLteDelta))
            
        }//else of if(isTrue(DeltaEqZero))
        
    }// else if(type_of_coefficient == "negated")
    
    else
     {
     cout<<"\nIllegal value "<<type_of_coefficient<<" for type_of_coefficient in function obtainSplittedVector in DAGEliminator.cpp\n";
     exit(1);
     }    
    
    //showAPVector("SplittedVector", SplittedVector);   
   
}// end of function


// Obtain DNF from a constraint of the form (a1 \/ ... \/ an) /\ ...../\ (x1 \/ ...\/ xn) using distributivity
void obtainDNFUsingDistributivity(vector< vector<DAGNode*> > &SplittedConstraints, vector<DAGNode*> &DNF)
{
    while(SplittedConstraints.size() >= 2)
    {
        vector<DAGNode*> Splitted1 = SplittedConstraints[0];
        vector<DAGNode*> Splitted2 = SplittedConstraints[1];
        
        vector<DAGNode*> NewSplitted;
        applyDistributiveLaw(Splitted1, Splitted2, NewSplitted);
        
        SplittedConstraints.erase(SplittedConstraints.begin()+0);
        SplittedConstraints.erase(SplittedConstraints.begin()+1);
        
        SplittedConstraints.push_back(NewSplitted);
    }
    
    // Here SplittedConstraints.size() <2
    
    if(SplittedConstraints.size() <=  0)
    {
        cout<<"\nError in function obtainDNFUsingDistributivity in DAGEliminator.cpp\n";
        exit(1);
    }
    
  // Here SplittedConstraints.size() = 1
    
    DNF = SplittedConstraints[0];    
}


// Given A=(a1 \/ ... \/ an) and B=(b1 \/ ...\/ bn), obtain C as (a1b1 \/ ... \/ anbn)
void applyDistributiveLaw(vector<DAGNode*> &A, vector<DAGNode*> &B, vector<DAGNode*> &C)
{
    for(int i=0; i<A.size(); i++)
        for(int j=0; j<B.size(); j++)
        {
            DAGNode* Conjunct1 = A[i];
            DAGNode* Conjunct2 = B[j];
            
            DAGNode* NewConjunct = createDAG("and", Conjunct1, Conjunct2, LOGICAL_SYMBOL, 1);
            C.push_back(NewConjunct);
        }
}


// Eliminate quantifiers from the vector of conjunctions using Fourier-Motzkin style QE
void eliminateQuantifiersUsingFourierMotzkin(vector<DAGNode*> &DNF, string VarToElim, map<string, int> &WidthTable, set<DAGNode*> &QEliminatedDNF)
{
    for(int i=0; i<DNF.size(); i++)
    {
        DAGNode* Conjunction = DNF[i];
        
        //cout<<"\nConjunction = "<<getDAGStr(Conjunction)<<endl;
        
        set<string> Support;
        getDAGNames(Conjunction, Support);
        
        if(Support.find(VarToElim)==Support.end())// Conjunction free of VarToElim
        {
         //cout<<"\nConjunction free of "<<VarToElim<<endl;
         QEliminatedDNF.insert(Conjunction);
        }
        else
        {
         //cout<<"\nConjunction has "<<VarToElim<<endl;
         set<DAGNode*> Free_Constraints;
         set<DAGNode*> Bound_Constraints;
         set<DAGNode*> Constraints;
         
         splitConjunctionIntoSetGeneral(Conjunction, Constraints);         
         getFreeAndBoundConstraints(Constraints, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
         
         //showAPSet("Bound_Constraints", Bound_Constraints);
         //showAPSet("Free_Constraints", Free_Constraints);
         
         DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
         
         //cout<<"\nFree_Part = "<<getDAGStr(Free_Part)<<endl;
         
         DAGNode* QE_Bound_Part = applyFourierMotzkin(Bound_Constraints, VarToElim, WidthTable);
         
         //cout<<"\nQE_Bound_Part = "<<getDAGStr(QE_Bound_Part)<<endl;
         
         DAGNode* NewConjunction = createDAG("and", Free_Part, QE_Bound_Part, LOGICAL_SYMBOL, 1);
         
         //cout<<"\nNewConjunction = "<<getDAGStr(NewConjunction)<<endl;
         
         QEliminatedDNF.insert(NewConjunction);
        }//else
    }//for
}//function ends here


// Returns the disjunction of the constraints in "Constraints"
DAGNode* getDisjunctionFromSet(set<DAGNode*> &Constraints)
{
  DAGNode *resultDAG=NULL;
  // result is the disjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("or",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
	  
  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("false",(void*)v);
    }
  return resultDAG;
}


// Recreate BVUGT constraints by BVULE constraints
DAGNode* recreateBVUGTConstraints(DAGNode* Phi, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  set<DAGNode*> Constraints;
  splitConjunctionIntoSetGeneral(Phi, Constraints);
  //showAPSet("Constraints", Constraints);
  
  set<DAGNode*> RecreatedConstraints;
  
  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
  {
      DAGNode* Constraint = *it;
      
      //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
      
      if(Constraint->Get_node_name()=="=" || Constraint->Get_node_name()=="is_not_equal"|| Constraint->Get_node_name()=="bvule")
      {
          // This is fine
          //cout<<"\nNo need to recreate the constraint\n";
          RecreatedConstraints.insert(Constraint);
      }//if
      else if(Constraint->Get_node_name()=="bvugt")
      {
          //cout<<"\nWe need to recreate the constraint\n";
          DAGNode* RecreatedConstraint1;
          DAGNode* RecreatedConstraint2;
          
          getRecreatedBVUGTConstraints(Constraint, WidthTable, RecreatedConstraint1, RecreatedConstraint2);
          
          //cout<<"\nRecreatedConstraint1 = "<<getDAGStr(RecreatedConstraint1)<<endl;
          //cout<<"\nRecreatedConstraint2 = "<<getDAGStr(RecreatedConstraint2)<<endl;
          
          if(isFalse(RecreatedConstraint1) || isFalse(RecreatedConstraint1))//False
          {
              cout<<"\nResult becoming FALSE in function recreateBVUGTConstraints in DAGEliminator.cpp\n";
              exit(1);
          }
          if(!isTrue(RecreatedConstraint1))
          {
              RecreatedConstraints.insert(RecreatedConstraint1);
          }
          if(!isTrue(RecreatedConstraint2))
          {
              RecreatedConstraints.insert(RecreatedConstraint2);
          }
      }//else if
  }//for
  
  DAGNode* RecreatedDAG = getConjunctionFromSet(RecreatedConstraints);
  //cout<<"\nRecreatedDAG = "<<getDAGStr(RecreatedDAG)<<endl;
  return RecreatedDAG;

}//function


// Recreate BVUGT constraint Constraint as RecreatedConstraint1 /\ RecreatedConstraint2
// where RecreatedConstraint1, RecreatedConstraint2 are BVULE constraint
void getRecreatedBVUGTConstraints(DAGNode *Constraint, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint1, DAGNode* &RecreatedConstraint2)
{
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
    
    set<string> Support_Constraint;
    getDAGNames(Constraint, Support_Constraint);
    if(Support_Constraint.empty())
    {
        cout<<"\nEmpty support for constraint in function getRecreatedBVUGTConstraints in DAGEliminator.cpp\n";
        exit(1);
    }
    set<string>::iterator it = Support_Constraint.begin();
    string Variable = *it;
    map<string, int>::iterator wit = WidthTable.find(Variable);
    if(wit == WidthTable.end())
    {
        cout<<"\nNo entry for the variable "<<Variable<<" in WidthTable in function getRecreatedBVUGTConstraints in DAGEliminator.cpp\n";
        exit(1);
    }
    int WidthOfVar = wit->second;
    ULINT ModWidthOfVar = findPower(WidthOfVar);
    
    ULINT MaxValue = ModWidthOfVar- 1;
    char MaxValue_char[10];
    sprintf(MaxValue_char, "%llu", MaxValue);
    string MaxValue_str(MaxValue_char);
    DAGNode *MaxValue_dag = createBVConstant(MaxValue_str, WidthOfVar);
    
    DAGNode *NewConstraint = quickSimplify(createDAG("is_not_equal", RHS, MaxValue_dag, LOGICAL_SYMBOL, 1));
    
    if(isTrue(NewConstraint))
    {
        RecreatedConstraint1 = NewConstraint;
    }
    else
    {
        ULINT MaxValueLessOne = ModWidthOfVar- 2;
        char MaxValueLessOne_char[10];
        sprintf(MaxValueLessOne_char, "%llu", MaxValueLessOne);
        string MaxValueLessOne_str(MaxValueLessOne_char);
        DAGNode *MaxValueLessOne_dag = createBVConstant(MaxValueLessOne_str, WidthOfVar);
    
        RecreatedConstraint1 = quickSimplify(createDAG("bvule", RHS, MaxValueLessOne_dag, LOGICAL_SYMBOL, 1));
    }
    
    list<string> VarNames;
    attachSetToList(Support_Constraint, VarNames);
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;
    
    getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
    getUnitMatrix(Unit_Const_Mat, 1, 1);
    
    matAdd(T_Const_Mat, Unit_Const_Mat, ModWidthOfVar);
    
    vector<string> LMITypes;
    LMITypes.push_back("bvule");
    
    RecreatedConstraint2 = quickSimplify(getDAGFromInequations(T_Coef_Mat, T_Const_Mat, S_Coef_Mat, S_Const_Mat, VarNames, ModWidthOfVar, LMITypes));
    
}


// Call syntactic scope reduction. Now apply semantic scope reduction followed by Fourier-Motzkin style
// QE on the scope reduced parts and create the result
DAGNode* syntacticScopeReductionWithQE(DAGNode *Phi,  set<string> &VarsToElim, map<string, int> &WidthTable)
     {
      bool recreate_bvugt_constraints = true; 
      
      if(recreate_bvugt_constraints)
        {
            //cout<<"\nRecreating the BVUGT constraints\n";
            //cout<<"\nOriginal Phi = "<<getDAGStr(Phi)<<endl;
            Phi = recreateBVUGTConstraints(Phi, VarsToElim, WidthTable);
            //cout<<"\nRecreated Phi = "<<getDAGStr(Phi)<<endl;
            //char c = getchar();
            //fprintf(LMIQEOutputDataFp, "\nBVUGT constraints recreated\n");
        }  
    
    set<DAGNode*> Constraints;

    //showSet(VarsToElim,"VarsToElim");
    splitConjunctionIntoSetGeneral(Phi, Constraints);
    //showAPSet("Constraints", Constraints);

     
    map<DAGNode*, set<string> > Constraint_VarsToElim_Map;
    map<string, set<DAGNode*> > VarToElim_Constraints_Map;
    map<set<DAGNode*>, set<string> > Constraints_VarsToElim_Map;

      // Create Constraint_VarsToElim_Map
      for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
        {
         DAGNode *Constraint = *it;
         set<string> Support;
         getDAGNames(Constraint, Support); // Support of Constraint obtained
         set<string> VarsToElimInSupport;
         set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));

         Constraint_VarsToElim_Map.insert(make_pair(Constraint, VarsToElimInSupport));
        }

      // Showing Constraint_VarsToElim_Map

     //  cout<<"\nConstraint_VarsToElim_Map\n";

     //  for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
     //    {
     //     DAGNode *Constraint = it->first;
     //     set<string> Support = it->second;
     //     cout<<"\nConstraint = "<<getDAGStr(Constraint)<<"\t";
     //    showSet(Support, "Support");
     //   }

      // Create VarToElim_Constraints_Map

      bool Constraints_Free_Of_VarsToElim = true;

      for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
        {
         DAGNode *Constraint = it->first;
         set<string> Support = it->second;

         if(!Support.empty())
            {
            Constraints_Free_Of_VarsToElim = false;
            }

         for(set<string>::iterator it2=Support.begin(); it2!=Support.end(); it2++)
            {
            string Variable = *it2;
            map<string, set<DAGNode*> >::iterator vcm_it = VarToElim_Constraints_Map.find(Variable);
            if(vcm_it != VarToElim_Constraints_Map.end())
                    {
                    (vcm_it->second).insert(Constraint);
                    }
            else
                    {
                    set<DAGNode*> Constraints;
                    Constraints.insert(Constraint);
                    VarToElim_Constraints_Map.insert(make_pair(Variable, Constraints));
                    }
            } // for ends here 
         }// for ends here

      if(Constraints_Free_Of_VarsToElim) // All constraints free of VarsToElim
            {
             //cout<<"\nAll constraints free of variables to be eliminated\n";
            return createLogicalConstant("true");
            }

      // Showing VarToElim_Constraints_Map

      //cout<<"\nVarToElim_Constraints_Map\n";

      // for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
      //  {
      //    string VarToElim = it->first;
      //    set<DAGNode*> Constraints = it->second;
      //    cout<<"\nVarToElim = "<<VarToElim<<"\t";
      //    showAPSet("Constraints", Constraints);
      //  }

      // Create Constraints_VarsToElim_Map

      for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
        {
         string VarToElim = it->first;
         set<DAGNode*> Constraints = it->second;
         map<set<DAGNode*>, set<string> >::iterator cvm_it = Constraints_VarsToElim_Map.find(Constraints);
         if(cvm_it != Constraints_VarsToElim_Map.end())
            {
            (cvm_it->second).insert(VarToElim);
            }
        else	
            {
            set<string> Support;
            Support.insert(VarToElim);
            Constraints_VarsToElim_Map.insert(make_pair(Constraints, Support));
            }
        }
      

      // Showing Constraints_VarsToElim_Map
      
       showConstraintsVarsToElimMap("Constraints_VarsToElim_Map", Constraints_VarsToElim_Map);


       while(Constraints_VarsToElim_Map.size() > 1)
       {

           //showConstraintsVarsToElimMap("Constraints_VarsToElim_Map", Constraints_VarsToElim_Map);

           map<set<DAGNode*>, set<string> >::iterator minlhs_it = findEntryWithMinLHS(Constraints_VarsToElim_Map);

           set<DAGNode*> SelectedConstraints = minlhs_it->first;
           //showAPSet("SelectedConstraints", SelectedConstraints);
           set<string> VarsToElimInSelectedConstraints = minlhs_it->second;
           //showSet(VarsToElimInSelectedConstraints, "VarsToElimInSelectedConstraints");
           
           //cout<<"\nLet us eliminate variables from this\n";           
           
           DAGNode* ResultOfQEFromSelectedConstraints = applySemanticScopeReductionAndExpensiveLayers(SelectedConstraints, VarsToElimInSelectedConstraints, WidthTable);
            
           
           if(checkTimeOut())
                    {
                    // Time Out !!!
                    cout<<"\nWarning!!!The function syntacticScopeReductionWithQE in DAGEliminator.cpp has timed out. It's result is unknown\n";
                    timed_out = true; // timed_out flag set
                    return createLogicalConstant("true"); // Let's temporarily treat it as sat in the called function
                    }              

           
           
           recreateConstraints_VarsToElim_MapWithQE(Constraints_VarsToElim_Map, minlhs_it, ResultOfQEFromSelectedConstraints);
       }
   
   // Showing Constraints_VarsToElim_Map
     
   //showConstraintsVarsToElimMap("Constraints_VarsToElim_Map", Constraints_VarsToElim_Map); 
      
   DAGNode *resultDAG;
   
   map<set<DAGNode*>, set<string> >::iterator it = Constraints_VarsToElim_Map.begin();
   
   set<DAGNode*> Final_Constraints = it->first;
   
   resultDAG = getConjunctionFromSet(Final_Constraints);
   
   return resultDAG;
   
 }


// Apply semantic scope reduction followed by expensive layers
// on \exists VarsToElim. (/\ Constraints)
// Note that some of the constraints in Constraints may be disjunctions
// Hence we need to convert /\Constraints to DNF and then call the layers on each,
// and then take disjunction of the results
DAGNode* applySemanticScopeReductionAndExpensiveLayers(set<DAGNode*> &Constraints,  set<string> &VarsToElim, map<string, int> &WidthTable)
{     
    vector< vector<DAGNode*> > SplittedConstraints;
    vector<DAGNode*> DNF;
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* Constraint = *it;
        
        //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
        // Constraint can be a disjunction in general. Hence split it into individual components initially
        vector<DAGNode*> SplittedVector;
        
        set<DAGNode*> SplittedSet;
        // Get the individual component conjunctions in Constraint
        getComponentsOfDisjunction(Constraint, SplittedSet);
        copySetToVector(SplittedSet, SplittedVector);
        SplittedConstraints.push_back(SplittedVector);
    }
    
    // Let us apply distributive law to SplittedConstraints in order to obatin the constraints in DNF
    // Note that this portion can be improved using Monniuax style to obtain DNF
    bool obtain_DNF_using_distributivity = true;    
    if(obtain_DNF_using_distributivity)
    {
        obtainDNFUsingDistributivity(SplittedConstraints, DNF);
    }
    else
    {
        cout<<"\nError in function applySemanticScopeReductionAndExpensiveLayers in DAGEliminator.cpp.  Presently the only way to obtain DNF is using distributivity\n";
        exit(1);
    }
    
    //showAPVector("DNF", DNF);
    //cout<<"\nDNF.size() == "<<DNF.size()<<endl;
    
    set<DAGNode*> QEliminatedDNF;
    
    // The problem now is \exists VarsToElim.(DNF)
    
    for(int i=0; i<DNF.size(); i++)
    {
        DAGNode* Quantified_Bound_Part = DNF[i];
        
        //cout<<"\nConstraint to be given to exp. layers = "<<getDAGStr(Quantified_Bound_Part)<<endl;
        
        set<DAGNode*> Quantified_Bound_Part_Constraints;
        
        splitConjunctionIntoSetGeneral(Quantified_Bound_Part, Quantified_Bound_Part_Constraints);
          
        set< set<DAGNode*> > ScopeReducedDAGs;
        
        bool result_trustworthy =   semanticScopeReduction(Quantified_Bound_Part, Quantified_Bound_Part_Constraints, VarsToElim, WidthTable, ScopeReducedDAGs);  
        
        
         if(!result_trustworthy)            
              {
                    vector<DAGNode*> Quantified_Bound_Part_Constraints_Vector;
                    
                    copySetToVector(Quantified_Bound_Part_Constraints, Quantified_Bound_Part_Constraints_Vector);
             
                   set<DAGNode*> Free_Constraints_From_Quantified_Bound_Part;
                   set<DAGNode*> Bound_Constraints_From_Quantified_Bound_Part;

                   getFreeAndBoundConstraintsForMultipleVariables(Quantified_Bound_Part_Constraints_Vector, VarsToElim, Free_Constraints_From_Quantified_Bound_Part, Bound_Constraints_From_Quantified_Bound_Part, WidthTable);
             
                   DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints_From_Quantified_Bound_Part);
                   
                   DAGNode* Bound_Part = getConjunctionFromSet(Bound_Constraints_From_Quantified_Bound_Part);
             
                   //cout<<"\nCalling expensive layers for QE from "<<getDAGStr(Bound_Part)<<endl;
                   
                   set<DAGNode*> SplittedSet;
                   
                   if(!isTrue(Bound_Part))
                   {
                       DAGNode *ResultFromExpLayer =quickSimplify(expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(Bound_Part, VarsToElim, WidthTable, 0));
  
                       
                        if(checkTimeOut())
                        {
                        // Time Out !!!
                        cout<<"\nWarning!!!The function applySemanticScopeReductionAndExpensiveLayers in DAGEliminator.cpp has timed out. Its result is not exact\n";
                        timed_out = true; // timed_out flag set
                        return createLogicalConstant("true");
                        }
                       
                       
                       //cout<<"\nResultFromExpLayer = "<<getDAGStr(ResultFromExpLayer)<<endl;
                   
                       set<DAGNode*> SplittedSetInitial;
                       // Get the individual component conjunctions in Constraint
                   
                      getComponentsOfDisjunction(ResultFromExpLayer, SplittedSetInitial);
                      
                      if(isTrue(Free_Part))
                      {
                         SplittedSet = SplittedSetInitial; 
                      }
                      else
                      {
                          for(set<DAGNode*>::iterator it=SplittedSetInitial.begin(); it != SplittedSetInitial.end(); it++)
                          {
                              DAGNode *Temp  = createDAG("and", *it, Free_Part, LOGICAL_SYMBOL, 1);
                              SplittedSet.insert(Temp);
                          }//for
                      }//else
                   }//if(!isTrue(Bound_Part))         
                   else
                   {
                       SplittedSet.insert(Free_Part);
                   }                 
                  
                  set_union(QEliminatedDNF.begin(), QEliminatedDNF.end(), SplittedSet.begin(), SplittedSet.end(), inserter(QEliminatedDNF, QEliminatedDNF.begin())); 
	     }//if(!result_trustworthy)
         else if(!ScopeReducedDAGs.empty())
                {
                    
                    for(set< set<DAGNode*> >::iterator it=ScopeReducedDAGs.begin(); it != ScopeReducedDAGs.end(); it++)
                            {
                            set<DAGNode*> MyScopeReducedSet;
                            MyScopeReducedSet = *it;
                            DAGNode *Quantified_Bound_Part = getConjunctionFromSet(MyScopeReducedSet);
                            
                            //cout<<"\nScopeReduced Conjunction = "<<getDAGStr(Quantified_Bound_Part)<<endl;
        
                            if(!isTrue(Quantified_Bound_Part))
                                     {
                                        vector<DAGNode*> Quantified_Bound_Part_Constraints_Vector;
                                        copySetToVector(Quantified_Bound_Part_Constraints, Quantified_Bound_Part_Constraints_Vector);
                                                  
                                       set<DAGNode*> Free_Constraints_From_Quantified_Bound_Part;
                                       set<DAGNode*> Bound_Constraints_From_Quantified_Bound_Part;

                                       getFreeAndBoundConstraintsForMultipleVariables(Quantified_Bound_Part_Constraints_Vector, VarsToElim, Free_Constraints_From_Quantified_Bound_Part, Bound_Constraints_From_Quantified_Bound_Part, WidthTable);

                                       DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints_From_Quantified_Bound_Part);

                                       DAGNode* Bound_Part = getConjunctionFromSet(Bound_Constraints_From_Quantified_Bound_Part);

                                       //cout<<"\nCalling expensive layers for QE from "<<getDAGStr(Bound_Part)<<endl;

                                       set<DAGNode*> SplittedSet;

                                       if(!isTrue(Bound_Part))
                                       {
                                           DAGNode *ResultFromExpLayer =quickSimplify(expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(Bound_Part, VarsToElim, WidthTable, 0));

                                            if(checkTimeOut())
                                                    {
                                                    // Time Out !!!
                                                    cout<<"\nWarning!!!The function applySemanticScopeReductionAndExpensiveLayers in DAGEliminator.cpp has timed out. It's result is unknown\n";
                                                    timed_out = true; // timed_out flag set
                                                    return createLogicalConstant("true"); // Let's temporarily treat it as sat in the called function
                                                    }                                       
                                           
                                           
                                           //cout<<"\nResultFromExpLayer = "<<getDAGStr(ResultFromExpLayer)<<endl;

                                           set<DAGNode*> SplittedSetInitial;
                                           // Get the individual component conjunctions in Constraint

                                          getComponentsOfDisjunction(ResultFromExpLayer, SplittedSetInitial);

                                          if(isTrue(Free_Part))
                                          {
                                             SplittedSet = SplittedSetInitial; 
                                          }
                                          else
                                          {
                                              for(set<DAGNode*>::iterator it=SplittedSetInitial.begin(); it != SplittedSetInitial.end(); it++)
                                              {
                                                  DAGNode *Temp  = createDAG("and", *it, Free_Part, LOGICAL_SYMBOL, 1);
                                                  SplittedSet.insert(Temp);
                                              }//for
                                          }//else
                                       }//if(!isTrue(Bound_Part))         
                                       else
                                       {
                                           SplittedSet.insert(Free_Part);
                                       }  
                                       
                                                 
                                set_union(QEliminatedDNF.begin(), QEliminatedDNF.end(), SplittedSet.begin(), SplittedSet.end(), inserter(QEliminatedDNF, QEliminatedDNF.begin())); 
                                }//if(!isTrue(MyConjunction))
                          }//for
            }//else if(!ScopeReducedDAGs.empty())
    }//for each constraint in DNF
    
   DAGNode* resultDAG;
   resultDAG = getDisjunctionFromSet(QEliminatedDNF);
   //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;
   return resultDAG;
}



// Version of recreateConstraints_VarsToElim_Map, which  is called from syntacticScopeReductionWithQE
void recreateConstraints_VarsToElim_MapWithQE(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map, map<set<DAGNode*>, set<string> >::iterator minlhs_it, DAGNode *ResultOfQE)
{
    map<set<DAGNode*>, set<string> > New_Constraints_VarsToElim_Map;
    
     for(map<set<DAGNode*>, set<string> >::iterator it = Constraints_VarsToElim_Map.begin(); it != Constraints_VarsToElim_Map.end(); it++)
     {
         if(it != minlhs_it)
         {
             set<DAGNode*> MinLHS = minlhs_it->first;
             set<DAGNode*> LHS = it->first;
             
             if(checkIntersecting(MinLHS, LHS)) // merge
             {
                 //cout<<"\nIntersecting\n";
                 //cout<<"\nExisting Formula Part\n";
                 //showAPSet("LHS", LHS);
                 
                 set<DAGNode*> NewConstraints;
                 set_difference(LHS.begin(), LHS.end(), MinLHS.begin(),  MinLHS.end(),inserter(NewConstraints, NewConstraints.begin()));
                 NewConstraints.insert(ResultOfQE);
                 
                 //cout<<"\nNew Formula Part\n";
                 //showAPSet("NewConstraints", NewConstraints);
                 
                 New_Constraints_VarsToElim_Map.insert(make_pair(NewConstraints, it->second));
             }// merge
             else // just insert into the new map
             {
                 //cout<<"\nNon-Intersecting\n";
                 //cout<<"\nExisting Formula Part\n";
                 //showAPSet("LHS", LHS);
                 
                 New_Constraints_VarsToElim_Map.insert(make_pair(it->first, it->second));
             }// just insert into the new map
         }// if
     }// for
    
   Constraints_VarsToElim_Map = New_Constraints_VarsToElim_Map; 
    
}// eof



// Get #of vars to elim in input of layer4 of LMI QE algorithm
void getNumberOfVarsToElimInLayer4Input(DAGNode* Constraint, set<string> &VarsToElim)
{
    set<string> Support;
    set<string> CommonVars;
    
    getDAGNames(Constraint, Support);
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMIVarsInputToLayer4 = CommonVars.size();    
}
    
    
// Initialize all the conjunction-level parameters to zero
void initializeConjunctionLevelParameters()
{
LMINoVarsToElim=0;
LMINoLMIs=0;
LMINoLMEs=0;
LMINoLMDs=0;
LMINoVars=0;
LMITotalBits=0;
LMIBitsToElim=0;
LMIVarsInputToLayer2=0;
LMIVarsInputToLayer3=0;
LMIVarsInputToLayer4=0;
LMILayer1Time=0;
LMILayer2Time=0;
LMILayer3Time=0;
LMILayer4Time=0;
}


// Version of isExtForLMDsAndLMIs when there are LMIs
bool isExtForLMDsAndLMIsWithLMDs(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, DAGNode*  &AdditionalConstraint, int k1, ULINT ModWidthOfVarToElim, set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet)
{
    // Let us just ensure that the RHS of all LMIs are constants
    
    set<DAGNode*> ReplacableLMIs;
    
    for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* lmi = *it;
        if(!lmiRHSConstant(lmi))
        {
            cout<<"\nRHS of the LMI "<<getDAGStr(lmi)<<" is not a constant in function  isExtForLMDsAndLMIs in DAGEliminator.cpp. This needs to be implemented\n";
            
            if(lmi->Get_node_name()=="bvugt")
                {
                cout<<"\nLet us try to replace this by a tighter one for dropping. But we need to add a new LMI free of "<<VarToElim<<"\n";
                
                bool replacable = checkIfLMIIsReplacable(lmi, LMISet); // An lmi is replacable if there does not exist any other LMI with the same LHS and opposite sign
                
                if(replacable)
                    {
                    ReplacableLMIs.insert(lmi);
                    
                    
                    //NewConstraint is lmi's RHS \neq (2^p)-1
                    DAGNode *NewConstraint = getNewConstraintFromBVUGTConstraint(lmi, ModWidthOfVarToElim, WidthOfVarToElim);
                    // Conjunct the NewConstraint with the AdditionalConstraint
                    if(isTrue(AdditionalConstraint))
                        {
                        AdditionalConstraint = NewConstraint;
                        }
                    else
                        {
                        AdditionalConstraint = createDAG("and", AdditionalConstraint, NewConstraint,  LOGICAL_SYMBOL, 1);
                        }
                    }
                else
                    {
                    cout<<"\nLMI cannot be replaced in function isExtForLMDsAndLMIs in DAGEliminator.cpp.\n";
                    exit(1);
                    }//else
                }
            else if(lmi->Get_node_name()=="bvule")
                {
                cout<<"\nLet us try to replace this by a tighter one for dropping\n";
                
                bool replacable = checkIfLMIIsReplacable(lmi, LMISet); // An lmi is replacable if there does not exist any other LMI with the same LHS and opposite sign
                
                if(replacable)
                    {
                    ReplacableLMIs.insert(lmi);
                    }
                else
                    {
                    cout<<"\nLMI cannot be replaced in function isExtForLMDsAndLMIs in DAGEliminator.cpp.\n";
                    exit(1);
                    }//else
                }//else if
            else
                {
                cout<<"\nUnknown LMI type "<<lmi->Get_node_name()<<" in function isExtForLMDsAndLMIs in DAGEliminator.cpp.\n";
                exit(1);
                }//else
            
        }//if
    }//for
   
    
   if(!ReplacableLMIs.empty()) 
        {
         showAPSet("ReplacableLMIs", ReplacableLMIs);
         replaceLMIsByTighterOnes(ReplacableLMIs, WidthOfVarToElim, LMISet);
         showAPSet("LMISet after replacement", LMISet);
         //char c = getchar();
         }
   // Let us form LMI Pairs 
   vector<pair<DAGNode*, DAGNode*> > LMIPairs; 
   identifyLMIPairs(LMISet, LMIPairs);
   showLMIPairs(LMIPairs); 
   long long int CombinationsPermitted = findCombinationsPermittedByLMIs(LMIPairs,  VarToElim, k1, WidthOfVarToElim, ModWidthOfVarToElim);
   long long int CombinationsDisallowed = findCombinationsDisallowedByLMDs(LMDSet,  VarToElim, k1, WidthOfVarToElim, ModWidthOfVarToElim);
   
   
   cout<<"\nCombinationsPermitted = "<<CombinationsPermitted<<endl;
   cout<<"\nCombinationsDisallowed = "<<CombinationsDisallowed<<endl;
   
   
   
   if(CombinationsPermitted > CombinationsDisallowed)
   {
       return true;
   }

   return false;
} 


// Find # of combinations disallowed LMDs while dropping conjunction of LMIs and LMDs
 long long int findCombinationsDisallowedByLMDs(set<DAGNode*> &LMDSet,  string VarToElim, int k1, int WidthOfVarToElim, ULINT ModWidthOfVarToElim)
 {
     long long int CombinationsDisallowed=0;
     
     for(set<DAGNode*>::iterator it=LMDSet.begin(); it != LMDSet.end(); it++)
     {
         DAGNode* LMD = *it;
         
         list<DAGNode*>::iterator iout;
         iout = LMD->Get_outlist_start();
         DAGNode *LHS = *iout;
         iout++;
         DAGNode *RHS = *iout;
         
         ULINT LHS_Coefficient;
         getCoefficientCorrected(LHS, VarToElim, LHS_Coefficient);
         
         ULINT RHS_Coefficient;
         getCoefficientCorrected(RHS, VarToElim, RHS_Coefficient);
         
         
         if(LHS_Coefficient ==0 && RHS_Coefficient == 0)
         {
             cout<<"\nBoth LHS and RHS are free of variable "<<VarToElim<<" in "<<getDAGStr(LMD)<<endl;
             cout<<"\nError in function findCombinationsDisallowedByLMIs in DAGEliminator.cpp\n";
             exit(1);
         }
         
          if(LHS_Coefficient !=0 && RHS_Coefficient != 0)
         {
             cout<<"\nBoth LHS and RHS have the variable "<<VarToElim<<" in "<<getDAGStr(LMD)<<endl;
             cout<<"\nError in function findCombinationsDisallowedByLMIs in DAGEliminator.cpp\n";
             exit(1);
         }
         
         if(LHS_Coefficient!=0)
         {
         cout<<"\nLHS_Coefficient = "<<LHS_Coefficient<<endl;
         ULINT MyKValue = powerOfTwo(LHS_Coefficient);
         ULINT MyContribution = findPower(MyKValue);
         CombinationsDisallowed += MyContribution;
         }
         
         if(RHS_Coefficient!=0)
         {
         cout<<"\nRHS_Coefficient = "<<RHS_Coefficient<<endl;
         ULINT MyKValue = powerOfTwo(RHS_Coefficient);
         ULINT MyContribution = findPower(MyKValue);
         CombinationsDisallowed += MyContribution;
         }
     }//for
     
     return CombinationsDisallowed;
 }
 
 
 
 
 
 // Functions added from 12 Jan 2012 onwards to implement variant of Monniaux on LMIs which is  recursive

// Calls the function Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting, 
// and records the time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForLMIs_With_Recursive_Splitting(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable)  
{
  time_t var_elim_start_time, var_elim_end_time;
  time(&var_elim_start_time);

  time_out_start = var_elim_start_time;
  
  createVariableMap(WidthTable);
  //showVariableMap();
  
  int project_depth = 0;
  
  DAGNode* result = callMonniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(F, VarSetToElim, WidthTable, project_depth);

  time(&var_elim_end_time);

  if(!timed_out)
	{
  	cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
  	cout<<"\nSize of output dag = "<<getSize(result)<<endl;
	}
  else
	{
	cout<<"\nQuantifier elimination timed out in "<<(var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
	}

  QElim_Time = var_elim_end_time - var_elim_start_time;

  return result;
  
}


// create VariableMap
void createVariableMap(map<string, int> &WidthTable)
{
    int variable_index = 1;
    for(map<string, int>::iterator it=WidthTable.begin(); it != WidthTable.end(); it++)
    {
        string variable = it->first;
        VariableMap.insert(make_pair(variable, variable_index));
        variable_index++;
    }
}


// show the variable map
void showVariableMap()
{
    cout<<"\nVariableMap\n***********\n";
    for(map<string, int>::iterator it=VariableMap.begin(); it !=VariableMap.end(); it++)
    {
        string variable = it->first;
        int variable_index = it->second;
        
        cout<<endl<<variable<<"\t"<<variable_index;
    }
    cout<<endl;
}




// calls  Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting with dynamic programming
DAGNode* callMonniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth)
{
  // Check in HashTable  
 DAGNode* Existing  = getEntryFromMonniaux_HT(F, VarSetToElim);

  if(Existing != NULL)
    {
      Monniaux_HT_hits++;
      cout<<"\nHit in Monniaux_HT in function callMonniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting\n";
      return Existing;
    }
 
 Monniaux_HT_misses++;
 cout<<"\nMiss in Monniaux_HT in function callMonniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting\n";
 //Call Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting
 DAGNode* Result = Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(F, VarSetToElim, WidthTable, project_depth);
 
 //Insert into HashTable
 insertEntryIntoMonniaux_HT(F, VarSetToElim, Result);
 
 return Result;    
}




// Function to get entries from Monniaux_HT
DAGNode* getEntryFromMonniaux_HT(DAGNode* F, set<string> &VarSetToElim)
{
  //cout<<"\ngetEntryFromMonniaux_HT called on "<<F<<"\n" ;
  // Creating the hash key

  string key = "";
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", F);
  temp_key = temp_char;
  key += temp_key;

  for(set<string>::iterator it = VarSetToElim.begin(); it != VarSetToElim.end(); it++)
    {
      string Variable = *it;
      map<string, int>::iterator vit = VariableMap.find(Variable);
      if(vit == VariableMap.end())
        {
        cout<<"\nNo entry for "<<Variable<<" in VariableMap in function getEntryFromMonniaux_HT in DAGEliminator.cpp\n";
        exit(1);
        }
      
      int variable_index = vit->second;
      
      sprintf(temp_char , "%d", variable_index);
      temp_key = temp_char;
      key += temp_key;
    }
  
  //cout<<"\nHash key = "<<key<<endl;

 
  // Hash key created

  HTStatusValue<DAGNode*> result = Monniaux_HT.hashtable_search(key);
  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}



// Function to insert entries into Monniaux_HT
void insertEntryIntoMonniaux_HT(DAGNode* F, set<string> &VarSetToElim, DAGNode* value)
{
   //cout<<"\ninsertEntryIntoMonniaux_HT called on "<<F<<"\n" ; 
  // Creating the hash key

  string key = "";
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", F);
  temp_key = temp_char;
  key += temp_key;

  for(set<string>::iterator it = VarSetToElim.begin(); it != VarSetToElim.end(); it++)
    {
      string Variable = *it;
      map<string, int>::iterator vit = VariableMap.find(Variable);
      if(vit == VariableMap.end())
        {
        cout<<"\nNo entry for "<<Variable<<" in VariableMap in function insertEntryIntoMonniaux_HT in DAGEliminator.cpp\n";
        exit(1);
        }
      
      int variable_index = vit->second;
      
      sprintf(temp_char , "%d", variable_index);
      temp_key = temp_char;
      key += temp_key;
    }

 //cout<<"\nHash key = "<<key<<endl;
 
  // Hash key created
  // Check if entry already exists

  HTStatusValue<DAGNode*> result = Monniaux_HT.hashtable_search(key);
  
  if(result.success())
    {
      // Do nothing
      if(false)
        {
        // Problem: Results can be semantically same - but syntactically different
        DAGNode* ExistingValue = result.GetValue();
        if(ExistingValue != value)
            {
            cout<<"\nError inside function insertEntryIntoMonniaux_HT in DAGEliminator.cpp.\n";
            cout<<"Result already exists for this subproblem which is different from the new result\n";
            exit(1);
            }
        }
    }

  else
    {
    result = Monniaux_HT.hashtable_insert(key, value);
    if(!result.success())
        {
        cout<<"\nHash Insert Failure Inside Function insertEntryIntoMonniaux_HT in DAGEliminator.cpp\n";
        exit(1);
        }
  }//else of if(result.success())
}


// Version of Monniaux_Quantifier_Eliminator_WithSupportForLMIs which is recursive
DAGNode* Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth)
{

  bool EnableLmiQEUsingOurMethods = !LMI_QE_using_blasting && !LMI_QE_using_OT;
  
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  vector<DAGNode*> Constraints;

  set<DAGNode*> APSet;
  getAtomicPredicates_WithSupportForLMIs(F, APSet);

  //showAPSet("APSet", APSet);
  
  map<string, string> Model;
  DAGNode *H = F;
  DAGNode *O = createLogicalConstant("false");
  int iteration =  1;
  

  while(!getModelGeneral_WithSupportForLMIs(H, Model, WidthTable))
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting in DAGEliminator.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return O;
	}

      //cout<<"\nSatisfiable! See result_thunsat_single_formula.txt for the model\n";

      cout<<"\nFinding the "<<iteration<<" th cube\n";
      fprintf(LMIQEOutputDataFp, "\nFinding the %d th cube\n", iteration);

     //char c = getchar();

           
      Generalize1_WithSupportForSelsConcats_WithSupportForLMIs(Model, F, APSet, WidthTable, Constraints);
      
      //showAPVector("Constraints After Generalize1", Constraints);

      if(generalize2_strategy==2) // Our strategy
	{
      	Generalize2_Strategy2_WithLMISupport(Constraints, F);
	}
      else
	{
	cout<<"\nUnknown value for generalize2_strategy2 in Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting in DAGEliminator.cpp\n";
	exit(1);
	}

     //showAPVector("Constraints After Generalize2", Constraints);
      
      DAGNode *Pi;
      
      if(EnableLmiQEUsingOurMethods)
      {
          
          if(Octagons)//LinDD cases
            {
                Pi = call_lmiQEUsingOurMethods_With_Recursive_Splitting(Constraints, VarSetToElim, WidthTable, project_depth);
                //cout<<"\nPress any key to continue\n";
                //char c = getchar();
                bool cross_check = false;
                if(cross_check)
                {
                      cout<<"\nPi = "<<getDAGStr(Pi)<<endl;
                      if(ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(Pi, WidthTable)) // Pi is unsat
                        {
                            cout<<"\nPi unsat\n";
                            exit(1);
                        }
                      else
                      {
                          cout<<"\nPi sat\n";
                      }
                }//if(cross_check)
                
          }
          else
          {
              Pi = lmiQEUsingOurMethodsGeneral(Constraints, VarSetToElim, WidthTable);
              //cout<<"\nPress any key to continue\n";
              //char c = getchar();
          }
      }// if(EnableLmiQEUsingOurMethods)
      else if(LMI_QE_using_blasting)
      {
          struct timeval blasting_start_ms, blasting_finish_ms;
          
         gettimeofday (&blasting_start_ms, NULL); 
          
         if(Octagons)//LinDD cases
            {
            Pi = lmiQEUsingLMEBasedReasoningAndBlasting(Constraints, VarSetToElim, WidthTable);
            }
         else
         {
             Pi = lmiQEUsingLMEBasedReasoningAndBlastingGeneral(Constraints, VarSetToElim, WidthTable);
         }        
     
        gettimeofday (&blasting_finish_ms, NULL);
    
        unsigned long long int LocalTime = blasting_finish_ms.tv_sec * 1000 + blasting_finish_ms.tv_usec / 1000;
        LocalTime -= blasting_start_ms.tv_sec * 1000 + blasting_start_ms.tv_usec / 1000; 
        LMIBlastingTime += LocalTime;
        BlastingConjunctionCounter++;
        
        fprintf(LMIQEImportantDataBlastingFp, "\n\n%s\t%d\t%d\t%d\t%llu", InFileName.c_str(), BlastingConjunctionCounter, LMIProjectBlastOriginalNoLMCs, LMIProjectBlastOriginalNoVarsToElim, LMIProjectBlastOriginalNoBitsToElim);
        fprintf(LMIQEImportantDataBlastingFp, "\t%d\t%d\t%d\n", LMIProjectBlastToBlastNoLMCs, LMIProjectBlastToBlastNoVarsToElim, LMIProjectBlastToBlastNoBitsToElim);      
      }//else if(LMI_QE_using_blasting)
      else if(LMI_QE_using_OT)
      {
          struct timeval ot_start_ms, ot_finish_ms;
          unsigned long long int LocalTime;
          
          gettimeofday (&ot_start_ms, NULL); 
          
          if(Octagons)//LinDD cases
            {
            lmiQEUsingLMEBasedReasoningAndOTest(Constraints, VarSetToElim, WidthTable);
            
            gettimeofday (&ot_finish_ms, NULL);
    
            LocalTime = ot_finish_ms.tv_sec * 1000 + ot_finish_ms.tv_usec / 1000;
            LocalTime -= ot_start_ms.tv_sec * 1000 + ot_start_ms.tv_usec / 1000; 
            LMIOTTime += LocalTime;
            
            
            Pi = call_lmiQEUsingOurMethods_With_Recursive_Splitting(Constraints, VarSetToElim, WidthTable, project_depth);
            }
         else
          {
           lmiQEUsingLMEBasedReasoningAndOTestGeneral(Constraints, VarSetToElim, WidthTable);
           
           gettimeofday (&ot_finish_ms, NULL);
    
            LocalTime = ot_finish_ms.tv_sec * 1000 + ot_finish_ms.tv_usec / 1000;
            LocalTime -= ot_start_ms.tv_sec * 1000 + ot_start_ms.tv_usec / 1000; 
            LMIOTTime += LocalTime;
           
           Pi = lmiQEUsingOurMethodsGeneral(Constraints, VarSetToElim, WidthTable);
          }        
          
       
        OTConjunctionCounter++;
        
        fprintf(LMIQEImportantDataOTFp, "\n%s\t%d\t%llu\n", InFileName.c_str(), OTConjunctionCounter,LocalTime);
      }//else if(LMI_QE_using_OT)
      else
      {
          cout<<"\nUnspecified QE method in function Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting in DAGEliminator.cpp\n";
          exit(1);
      }//else

      //cout<<"\nPi = "<<getDAGStr(Pi)<<endl;
      O = createDAG("or", O, Pi, LOGICAL_SYMBOL, 1);
      //cout<<"\nO = "<<getDAGStr(O)<<endl;
      H = createDAG("and", H, createDAG("not", Pi, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);
      //cout<<"\nH = "<<getDAGStr(H)<<endl;
     iteration++;
     }

  time(&end);
  duration = end - start;
  Quant_Monn_time = Quant_Monn_time + duration;
  
  
  return O;

}




// calls lmiQEUsingOurMethods_With_Recursive_Splitting  with dynamic programming
DAGNode* call_lmiQEUsingOurMethods_With_Recursive_Splitting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
    
 DAGNode* F = getConjunctionFromVector(Constraints);
 
 //cout<<"\nF in call_lmiQEUsingOurMethods_With_Recursive_Splitting = "<<getDAGStr(F)<<endl;
 //showSet(VarsToElim, "VarsToElim");
    
  // Check in HashTable  
 DAGNode* Existing  = getEntryFromMonniaux_HT(F, VarsToElim);

  if(Existing != NULL)
    {
      Monniaux_HT_hits++;
      cout<<"\nHit in Monniaux_HT in call_lmiQEUsingOurMethods_With_Recursive_Splitting\n";
      //cout<<"\nExisting in call_lmiQEUsingOurMethods_With_Recursive_Splitting = "<<getDAGStr(Existing)<<endl;
      return Existing;
    }
 
 Monniaux_HT_misses++;
 cout<<"\nMiss in Monniaux_HT in call_lmiQEUsingOurMethods_With_Recursive_Splitting\n";
 
 //Call lmiQEUsingOurMethods_With_Recursive_Splitting
 DAGNode* Result = lmiQEUsingOurMethods_With_Recursive_Splitting(Constraints, VarsToElim, WidthTable, project_depth);
 
 //Insert into HashTable
 insertEntryIntoMonniaux_HT(F, VarsToElim, Result);
 
 return Result;    
}





// Version of lmiQEUsingOurMethods with recursive splitting
DAGNode* lmiQEUsingOurMethods_With_Recursive_Splitting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
  bool dropping_using_qbf_solving=false;
  bool CallSemanticScopeReduction=false;
  bool CallLMEBasedSimplification = true;
  bool CallSyntacticScopeReduction=false;
  bool CallExpensiveLayers = true;
  bool CallSyntacticScopeReductionOnTimedOutCases=false;
  
  
  //showAPVector("Constraints", Constraints);
 //showSet(VarsToElim, "VarsToElim"); 

 // Local versions of the conjunction-level data ends here
int PresentConjunctionNumberLocal=0;
int LMINoVarsToElimLocal=0;
int LMINoLMIsLocal=0;
int LMINoLMEsLocal=0;
int LMINoLMDsLocal=0;
int LMINoVarsLocal=0;
unsigned long long int LMITotalBitsLocal=0;
unsigned long long int LMIBitsToElimLocal=0;
int LMIVarsInputToLayer2Local=0;
int LMIVarsInputToLayer3Local=0;
int LMIVarsInputToLayer4Local=0;
unsigned long long int LMILayer1TimeLocal=0;
unsigned long long int LMILayer2TimeLocal=0;
unsigned long long int LMILayer3TimeLocal=0;
unsigned long long int LMILayer4TimeLocal=0;
 // Local versions of the conjunction-level data ends here 



   //showAPVector("Constraints", Constraints);
   convertStrictLMIsToWeakLMIs(Constraints, WidthTable);
   //showAPVector("Constraints", Constraints);
   
   //char c = getchar();
  
   
  
  showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
  
  showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);
  
  
  // get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
  // in the respective global variables
  getRelevantLMIDetails_With_Recursive_Splitting(Bound_Constraints, VarsToElim, WidthTable, LMINoVarsToElimLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMITotalBitsLocal, LMIBitsToElimLocal, PresentConjunctionNumberLocal);
  
  
  DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
           Bound_Constraints_Vector.push_back(*it); 
        }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
  if(dropping_using_qbf_solving)
  {
      vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
           Bound_Constraints_Vector.push_back(*it); 
        }
      
       createConjunctionAndCheckQBFSolverEffectivess(Bound_Constraints_Vector, VarsToElim, WidthTable);       
       
  }
  
  if(CallLMEBasedSimplification)
  {
      DAGNode* Quantified_Bound_Part;
      DAGNode* Simplified_Bound_Part;
      
      if(!SwitchOffLayer1)
        {
        Simplified_Bound_Part = LMEBasedSimplificationAndLMIDropping_ForWeakLMIs(Bound_Constraints, VarsToElim, WidthTable, Quantified_Bound_Part, LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMILayer1TimeLocal, LMILayer2TimeLocal);
        }
      else
        {
        Quantified_Bound_Part =   getConjunctionFromSet(Bound_Constraints);
        Simplified_Bound_Part = createLogicalConstant("true");
        }
      
      DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
      
      if(isTrue(Quantified_Bound_Part))
      {
          DAGNode* resultDAG;
          resultDAG = createDAG("and", Simplified_Bound_Part, Free_Part, LOGICAL_SYMBOL, 1);
          
          fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
          
          if(project_depth == 0)
            {
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);        
               FMTypeConjunctionCounter++;
          }
          else
            {
             fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
            AdditionalConjunctionCounter ++; 
          }
          
                 
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
      }
      else
      {
          DAGNode* PartialResultDAG;
          PartialResultDAG = createDAG("and", Simplified_Bound_Part, Free_Part, LOGICAL_SYMBOL, 1);
          //cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
          //cout<<"\nWe still need to eliminate quantifiers from "<<getDAGStr(Quantified_Bound_Part)<<endl;
          
          set<DAGNode*> Quantified_Bound_Part_Constraints;
          splitConjunctionIntoLMISet(Quantified_Bound_Part, Quantified_Bound_Part_Constraints);
          
           
          
          set< set<DAGNode*> > ScopeReducedDAGs;
          
          
         struct timeval layer3_start_ms, layer3_finish_ms;
	 gettimeofday (&layer3_start_ms, NULL);          
          
         bool result_trustworthy;
         
         if(SwitchOffLayer3)
         {
             result_trustworthy =   false;
         }
         else
         {         
            result_trustworthy =   semanticScopeReduction(Quantified_Bound_Part, Quantified_Bound_Part_Constraints, VarsToElim, WidthTable, ScopeReducedDAGs);  
         }
          
         gettimeofday (&layer3_finish_ms, NULL);
   	 LMILayer3TimeLocal = layer3_finish_ms.tv_sec * 1000 + layer3_finish_ms.tv_usec / 1000;
   	 LMILayer3TimeLocal -= layer3_start_ms.tv_sec * 1000 + layer3_start_ms.tv_usec / 1000;
          
          LMILayer3Calls++;
        
          
          if(!result_trustworthy)
              {
              
              
              if(project_depth == 0)
                {
                LMILayer3CallsTimedOut++;
                
                 int LMCCountInInputConjunction;
                 int VarsCountInInputConjunction;
                 int VarsToElimCountInInputConjunction;
                 int PartitionsInOutputConjunction;
                 int MaxPartitionSizeInOutputConjunction;
                 int MinPartitionSizeInOutputConjunction;
                 float AvgPartitionSizeInOutputConjunction;
                   
                 extractLayer3Data(false, Quantified_Bound_Part_Constraints, VarsToElim, ScopeReducedDAGs, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction);
                   
                 fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t0\t%d\t%d\t%d\t-\t-\t-\t-\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, LMILayer3TimeLocal);
                 fprintf(LMIQEFinishedLayer3TimedOutDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t-\t-\t-\t-\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, LMILayer3TimeLocal);
                }
              
                 //cout<<"\nz3 timed out during computation. Result is cannot be trusted\n";
                 fprintf(LMIQEOutputDataFp, "\nz3 timed out during computation. Result is cannot be trusted\n");
                 
                 //fprintf(LMIQEImportantDataFp, "(unfinished)");
                 
                 if(CallExpensiveLayers)
                    {
                     fprintf(LMIQEOutputDataFp, "\nCalling the expensive layers\n"); 
                     
                     cout<<"\nCalling expensive layers for QE from "<<getDAGStr(Quantified_Bound_Part)<<endl;
                     
                     fprintf(LMIQEOutputDataFp, "\nCalling expensive layers for QE from %s\n", getDAGStr(Quantified_Bound_Part).c_str());
                     
                    DAGNode *RemainingResult;
                    
                    if(CallSyntacticScopeReductionOnTimedOutCases) //Perform syntactic scope reduction
                        // combined with semantic scope reduction and expensive layers
                    {
                        
                        getNumberOfVarsToElimInLayer4Input_With_Recursive_Splitting(Quantified_Bound_Part, VarsToElim, LMIVarsInputToLayer4Local);
                        
                        struct timeval layer4_start_ms, layer4_finish_ms;
                        gettimeofday (&layer4_start_ms, NULL);  
                        
                       RemainingResult = syntacticScopeReductionWithQE(Quantified_Bound_Part, VarsToElim, WidthTable);
                       
                       
                        if(checkTimeOut())
                        {
                         fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         
                         if(project_depth == 0)
                         {
                             
                          if(false)
                            {
                            fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                            fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                            FMTypeConjunctionCounter++;
                            }
                         }
                         else
                         {
                          fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                          fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);                        
                          AdditionalConjunctionCounter++;
                         }
                         
                         // Time Out !!!
                        cout<<"\nWarning!!!The function lmiUsingOurMethods in DAGEliminator.cpp has timed out. It's result is unknown\n";
                        timed_out = true; // timed_out flag set
                        return createLogicalConstant("true"); // Let's temporarily treat it as sat in the called function
                        }           
                       
                       gettimeofday (&layer4_finish_ms, NULL);
                       LMILayer4TimeLocal = layer4_finish_ms.tv_sec * 1000 + layer4_finish_ms.tv_usec / 1000;
                       LMILayer4TimeLocal -= layer4_start_ms.tv_sec * 1000 + layer4_start_ms.tv_usec / 1000;
                       
                        fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                       
                        if(project_depth==0)
                        {
                          fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                          fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);            
                          FMTypeConjunctionCounter++;
                        }
                        else
                        {
                          fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                          fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);            
                         AdditionalConjunctionCounter++;  
                        }
                        
                     }
                    else // Directly call the expensive layers on \exists X.Phi
                    {
                       struct timeval layer4_start_ms, layer4_finish_ms;
                       gettimeofday (&layer4_start_ms, NULL);    
                       
                       getNumberOfVarsToElimInLayer4Input_With_Recursive_Splitting(Quantified_Bound_Part, VarsToElim, LMIVarsInputToLayer4Local);
                        
                        
                      RemainingResult = expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(Quantified_Bound_Part, VarsToElim, WidthTable, project_depth);  
                     
                      gettimeofday (&layer4_finish_ms, NULL);
                      LMILayer4TimeLocal = layer4_finish_ms.tv_sec * 1000 + layer4_finish_ms.tv_usec / 1000;
                      LMILayer4TimeLocal -= layer4_start_ms.tv_sec * 1000 + layer4_start_ms.tv_usec / 1000;
                      
                      if(checkTimeOut())
                        {
                         fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         
                         if(project_depth == 0)
                         {
                             if(false)
                                {
                                fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                                fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                                FMTypeConjunctionCounter++;
                                }
                         }
                         else
                         {
                           fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                           fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                          AdditionalConjunctionCounter++;
                         }
                         // Time Out !!!
                        cout<<"\nWarning!!!The function lmiQEUsingOurMethods_With_Recursive_Splitting in DAGEliminator.cpp has timed out. Its result is not exact\n";
                        timed_out = true; // timed_out flag set
                        return createLogicalConstant("true");
                        }
                      
                        fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                
                        if(project_depth==0)
                        {
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         FMTypeConjunctionCounter++;
                        }
                        else
                        {
                         fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                        AdditionalConjunctionCounter++;
                        }
                        
                    }                   
                    
                    DAGNode* resultDAG;
                    resultDAG = createDAG("and", PartialResultDAG, RemainingResult, LOGICAL_SYMBOL, 1);
                    return resultDAG;
                    }// if(CallExpensiveLayers)
                 else
                    {
                    exit(1); 
                    }// else of  if(CallExpensiveLayers)
              }
              else
              {
              
               if(project_depth == 0)
                {
                   LMILayer3CallsFinished++;
                   
                   int LMCCountInInputConjunction;
                   int VarsCountInInputConjunction;
                   int VarsToElimCountInInputConjunction;
                   int PartitionsInOutputConjunction;
                   int MaxPartitionSizeInOutputConjunction;
                   int MinPartitionSizeInOutputConjunction;
                   float AvgPartitionSizeInOutputConjunction;
                   
                   extractLayer3Data(true, Quantified_Bound_Part_Constraints, VarsToElim, ScopeReducedDAGs, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction);
                   
                   
                   
                    if(ScopeReducedDAGs.size()==0)
                        {
                        LMILayer3CallsTrue++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t3\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        fprintf(LMIQEFinishedLayer3TrueDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                         }
                   else if(ScopeReducedDAGs.size()==1)
                        {
                        LMILayer3CallsNoSC++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t2\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        fprintf(LMIQEFinishedLayer3NoSCDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        }
                   else
                        {
                         LMILayer3CallsSCAchieved++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t1\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        fprintf(LMIQEFinishedLayer3SCAchievedDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        }
                }
                  //cout<<"\nScopeReducedDAGs\n";
                  fprintf(LMIQEOutputDataFp, "\nPartitioned DAGs\n");
                  int partition_number=1;
                  for(set< set<DAGNode*> >::iterator sdagit = ScopeReducedDAGs.begin(); sdagit != ScopeReducedDAGs.end(); sdagit++)
                  {
                    fprintf(LMIQEOutputDataFp, "\nPartition %d\n", partition_number);
                    set<DAGNode*> MyDAGSet = *sdagit;
                    showAPSet("Partition", MyDAGSet, LMIQEOutputDataFp);  
                    partition_number++;
                    //showAPSet("ScopeReducedDAGs", MyDAGSet);  
                  }//for
                  
                  if(ScopeReducedDAGs.empty())//Other part true
                    {
                      
                        fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

                        if(project_depth==0)
                        {
                        fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                        FMTypeConjunctionCounter++;
                        }
                        else
                        {
                         fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         AdditionalConjunctionCounter++;                        
                        }
                        
                        return PartialResultDAG;
                    }
                  else
                    {
                      //cout<<"\nQE needs to done\n";
                      fprintf(LMIQEOutputDataFp, "\nQE needs to done\n");
                      //fprintf(LMIQEImportantDataFp, "(unfinished)");
                      
                      if(CallExpensiveLayers)
                        {
                        fprintf(LMIQEOutputDataFp, "\nCalling the expensive layers\n");   
                        
                        getNumberOfVarsToElimInLayer4Input_With_Recursive_Splitting(Quantified_Bound_Part, VarsToElim, LMIVarsInputToLayer4Local);
                                                
                        struct timeval layer4_start_ms, layer4_finish_ms;
                        gettimeofday (&layer4_start_ms, NULL);  
                        
                        
                        DAGNode *RemainingResult = expensiveLayersOnSetOfScopeReducedDAGs_With_Recursive_Splitting(ScopeReducedDAGs , VarsToElim, WidthTable, project_depth);
                        
                        
                        if(checkTimeOut())
                        {
                         fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

                         if(project_depth == 0)
                         {
                             if(false)
                                {
                                 fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                                 fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                                 FMTypeConjunctionCounter++;
                                }
                         }
                         else
                         {
                          fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                          fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                          AdditionalConjunctionCounter++;
                         }
                         
                         
                          // Time Out !!!
                        cout<<"\nWarning!!!The function lmiUsingOurMethods_With_Recursive_Splitting in DAGEliminator.cpp has timed out. Its result is unknown\n";
                        timed_out = true; // timed_out flag set
                        return createLogicalConstant("true"); // Let's temporarily treat it as sat in the called function
                        }              
                        
                        
                        
                       gettimeofday (&layer4_finish_ms, NULL);
                       LMILayer4TimeLocal = layer4_finish_ms.tv_sec * 1000 + layer4_finish_ms.tv_usec / 1000;
                       LMILayer4TimeLocal -= layer4_start_ms.tv_sec * 1000 + layer4_start_ms.tv_usec / 1000;
        
                        fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

                        if(project_depth == 0) 
                        {
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         FMTypeConjunctionCounter++; 
                        }
                        else
                        {
                        fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                        AdditionalConjunctionCounter++;
                        }
                        
                        
                        DAGNode* resultDAG;
                        
                        //cout<<"\nRemainingResult = "<<getDAGStr(RemainingResult)<<endl;
                        //cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
                        
                        resultDAG = createDAG("and", PartialResultDAG, RemainingResult, LOGICAL_SYMBOL, 1);
                        
                        //cout<<"\nCalling expensive layers completed\n";
                        //cout<<"\nFinal Result of QE = "<<getDAGStr(resultDAG)<<endl;
                        //cout<<"\nPress any key to continue\n";
                        //char c = getchar();
                        
                        return resultDAG;
                        }// if(CallExpensiveLayers)
                    else
                        {
                        exit(1); 
                        }// else of  if(CallExpensiveLayers)
                    }
                  
              }// else          
      }// else
            
  }// if(CallLMEBasedSimplification)
  
  if(CallSemanticScopeReduction)
  {
      // Presently the problem is \exists VarsToElim. (/\ Bound_Constraints)
      
      bool SingleVariableSmanticScopeReduction = false;
      
      // Let's convert this to individual problems on each variable to be eliminted if
      // SingleVariableSmanticScopeReduction == true, and let us call
      // semanticScopeReduction on each of the conjunct
      
      if(SingleVariableSmanticScopeReduction)
      {
          map<string, set<DAGNode*> > VariableLevelConstraints;
          for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
          {
              string Variable = *it;
              set<DAGNode*> BoundConstraintsForVariable;
              set<DAGNode*> FreeConstraintsForVariable;
              getFreeAndBoundConstraints(Bound_Constraints, Variable, FreeConstraintsForVariable, BoundConstraintsForVariable, WidthTable);
              
              if(!BoundConstraintsForVariable.empty())
              {
                  VariableLevelConstraints.insert(make_pair(Variable, BoundConstraintsForVariable));
              }
          }
          
          cout<<"\nVariableLevelConstraints\n***************************\n";
           for(map<string, set<DAGNode*> >::iterator it =VariableLevelConstraints.begin(); it != VariableLevelConstraints.end(); it++)
          {
              string Variable = it->first;
              set<DAGNode*> BoundConstraintsForVariable = it->second;
              cout<<"\nVarToElim = "<<Variable<<endl;
              showAPSet("Constraints With VarToElim", BoundConstraintsForVariable);
          }
          // Call semanticScopeReduction on each of these
          
                   
           for(map<string, set<DAGNode*> >::iterator it =VariableLevelConstraints.begin(); it != VariableLevelConstraints.end(); it++)
          {
              string Variable = it->first;
              set<DAGNode*> BoundConstraintsForVariable = it->second;
              
              set<string> MyVarsToElim;
              MyVarsToElim.insert(Variable);
              
              DAGNode* Conjunction_BoundConstraintsForVariable = getConjunctionFromSet(BoundConstraintsForVariable);
              
              
              set< set<DAGNode*> > ScopeReducedDAGs;
              bool result_trustworthy =   semanticScopeReduction(Conjunction_BoundConstraintsForVariable, BoundConstraintsForVariable, MyVarsToElim, WidthTable, ScopeReducedDAGs);
              
              if(!result_trustworthy)
              {
                  cout<<"\nz3 timed out during computation. Result is cannot be trusted\n";
              }
              else
              {
                  cout<<"\nScopeReducedDAGs\n";
                  for(set< set<DAGNode*> >::iterator sdagit = ScopeReducedDAGs.begin(); sdagit != ScopeReducedDAGs.end(); sdagit++)
                  {
                    set<DAGNode*> MyDAGSet = *sdagit;
                    showAPSet("ScopeReducedDAGs", MyDAGSet);  
                  }//for
              }// else
              
              cout<<"\nPress any key to continue\n";
              char c=getchar();
          }// for
                    
      }// if(SingleVariableSmanticScopeReduction)
      else
      {
        set< set<DAGNode*> > ScopeReducedDAGs;
        bool result_trustworthy =   semanticScopeReduction(Phi, Bound_Constraints, VarsToElim, WidthTable, ScopeReducedDAGs);  
        if(!result_trustworthy)
              {
                 cout<<"\nz3 timed out during computation. Result is cannot be trusted\n";
              }
              else
              {
                  cout<<"\nScopeReducedDAGs\n";
                  for(set< set<DAGNode*> >::iterator sdagit = ScopeReducedDAGs.begin(); sdagit != ScopeReducedDAGs.end(); sdagit++)
                  {
                    set<DAGNode*> MyDAGSet = *sdagit;
                    showAPSet("ScopeReducedDAGs", MyDAGSet);  
                  }//for
              }// else
      }// else of if(SingleVariableSmanticScopeReduction)
      
  }//if(CallSemanticScopeReduction)
  
  if(CallSyntacticScopeReduction)
  {
      syntacticScopeReduction(Bound_Constraints,  VarsToElim, WidthTable);
  }//if(CallSyntacticScopeReduction)
  
  exit(1);
}// end of function
    





// Version of LMEBasedSimplificationAndLMIDropping which works on BVULE and BVUGE constraints
DAGNode* LMEBasedSimplificationAndLMIDropping_ForWeakLMIs(set<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable, DAGNode* &QuantifiedPart, int &LMIVarsInputToLayer2Local, int &LMIVarsInputToLayer3Local, unsigned long long int &LMILayer1TimeLocal, unsigned long long int &LMILayer2TimeLocal)
{
  struct timeval layer1_start_ms, layer1_finish_ms;
  struct timeval layer2_start_ms, layer2_finish_ms;
  
  
  
    
    
 vector<DAGNode*> BVULE_Vector;// set of constraints in Constraints of the form <=
 vector<DAGNode*> BVUGT_Vector;// Set of constraints in Constraints of the form >=
 
 for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
        {
           DAGNode* MyConstraint = *it;
           if(MyConstraint->Get_node_name()=="bvule")
           {
               BVULE_Vector.push_back(MyConstraint);
           }
           else if(MyConstraint->Get_node_name()=="bvuge")
           {
               BVUGT_Vector.push_back(MyConstraint);
           }
           else
           {
             cout<<"\nUnknown predicate "<<MyConstraint->Get_node_name()<<" inside function LMEBasedSimplificationAndLMIDropping_ForWeakLMIs in DAGEliminator.cpp\n";
             exit(1);
           }             
           
        }//for ends here
 
 
 //showAPVector("BVULE_Vector", BVULE_Vector);
 //showAPVector("BVUGT_Vector", BVUGT_Vector);
      

set<DAGNode*> AlreadyDone;
AlreadyDone.clear();

vector< pair<DAGNode*, DAGNode*> > SimplifiableLMIPairs;

while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<BVUGT_Vector.size();i++)
	{
          DAGNode* ugt = BVUGT_Vector[i];
          //cout<<"\nTaking LMI "<<getDAGStr(ugt)<<endl;
          if(AlreadyDone.find(ugt)!=AlreadyDone.end()) // We have already checked if there exists a pair for ugt
		{
		//cout<<"\nAlready Done\n";
		continue;
		}
          
          for(int j=0; j<BVULE_Vector.size(); j++)
          {
              DAGNode* ule = BVULE_Vector[j];
              
              if(simplifiableLMIPairFound_ForWeakLMIs(ugt, ule))
              {
                  //cout<<"\nMatches with "<<getDAGStr(ule)<<endl;
                  //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
                  SimplifiableLMIPairs.push_back(make_pair(ule, ugt));
                  BVUGT_Vector.erase(BVUGT_Vector.begin()+i);
                  BVULE_Vector.erase(BVULE_Vector.begin()+j);
                  fixpoint_reached=false;
                  break;
              }
          }
	
        if(!fixpoint_reached)// BVUGT_Vector is changed. Restart the loop
        {
            //cout<<"\nStarting from the beginning of BVUGT_Vector\n";
            break;
        }
        
        //cout<<"\nNo matching ule constraint found\n";
	AlreadyDone.insert(ugt);
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here


vector<DAGNode*> LME_Vector;

synthesizeLMEsFromLMIs(SimplifiableLMIPairs, LME_Vector, WidthTable);

//cout<<"\nThe problem we need to solve is\n";

//showSet(VarsToElim, "VarsToElim");
//showAPVector("BVULE_Vector", BVULE_Vector);
//showAPVector("BVUGT_Vector", BVUGT_Vector);
//showAPVector("LME_Vector", LME_Vector);


showAPVector("LMEs synthesized from LMIs", LME_Vector, LMIQEOutputDataFp);

DAGNode *ConjunctionOfLMIs = NULL;

 for(int i=0;i<BVULE_Vector.size();i++)
	{
          DAGNode* ule = BVULE_Vector[i];
          if(ConjunctionOfLMIs == NULL)
          {
              ConjunctionOfLMIs = ule;
          }
          else
          {
             ConjunctionOfLMIs=createDAG("and",  ConjunctionOfLMIs, ule, LOGICAL_SYMBOL, 1); 
          }
     }// for

 for(int i=0;i<BVUGT_Vector.size();i++)
	{
          DAGNode* ugt = BVUGT_Vector[i];
          if(ConjunctionOfLMIs == NULL)
          {
              ConjunctionOfLMIs = ugt;
          }
          else
          {
           ConjunctionOfLMIs=createDAG("and",  ConjunctionOfLMIs, ugt, LOGICAL_SYMBOL, 1);    
          }
     }// for

if(ConjunctionOfLMIs == NULL)
    {
    //cout<<"\nAll LMIs are converted to LMEs by the function LMEBasedSimplification in DAGEliminatir.cpp\n";
    
    fprintf(LMIQEOutputDataFp, "\nAll LMIs are converted to LMEs\n");
    
     set<DAGNode*> LME_Set;
    for(int i=0; i<LME_Vector.size(); i++)
    {
        LME_Set.insert(LME_Vector[i]);
    }
    //showAPSet("LME_Set", LME_Set);
     
     gettimeofday (&layer1_start_ms, NULL); 
     
    DAGNode* QEConjunctionOfLMEs = APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(LME_Set,  VarsToElim,  WidthTable);
    
    gettimeofday (&layer1_finish_ms, NULL);
    LMILayer1TimeLocal = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
    LMILayer1TimeLocal -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    


    //cout<<"\nQEConjunctionOfLMEs = "<<getDAGStr(QEConjunctionOfLMEs)<<endl;
    //fprintf(LMIQEOutputDataFp, "\nResult of QE = %s\n", (getDAGStr(QEConjunctionOfLMEs)).c_str());
    DAGNode* QEDag = QEConjunctionOfLMEs;
    QuantifiedPart = createLogicalConstant("true");   
    
    
    return QEDag;
    }
else
    {
    /* We have \exists VarsToElim.(\alpha/\ \beta) here, where \alpha = ConjunctionOfLMIs  and 
     \beta = conjunction of LMEs in LME_Vector  */
    
    //cout<<"\nConjunctionOfLMIs = "<<getDAGStr(ConjunctionOfLMIs)<<endl;
    
    
    gettimeofday (&layer1_start_ms, NULL); 
    
    /* Simplify \alpha to \alpha' using \beta */
    DAGNode* InlinedConjunctionOfLMIs = inlineLMIConjunctionUsingLMEs(ConjunctionOfLMIs, VarsToElim, LME_Vector, WidthTable);
    
    getNumberOfVarsToElimInLayer2Input_With_Recursive_Splitting(InlinedConjunctionOfLMIs, VarsToElim, LMIVarsInputToLayer2Local);
    
    
    //cout<<"\nInlinedConjunctionOfLMIs = "<<getDAGStr(InlinedConjunctionOfLMIs)<<endl;
    //fprintf(LMIQEOutputDataFp, "\nConjunctionOfLMIs after simplification by LMEs = %s\n", (getDAGStr(InlinedConjunctionOfLMIs)).c_str());
    
    vector<DAGNode*> InlinedLMI_Vector;
    splitConjunctionIntoLMIVector(InlinedConjunctionOfLMIs, InlinedLMI_Vector);
    //showAPVector("InlinedLMI_Vector", InlinedLMI_Vector);
    set<DAGNode*> InlinedLMI_Set;
    for(int i=0; i<InlinedLMI_Vector.size(); i++)
    {
        InlinedLMI_Set.insert(InlinedLMI_Vector[i]);
    }
    //showAPSet("InlinedLMI_Set", InlinedLMI_Set);
    
    
    set<DAGNode*> LME_Set;
    for(int i=0; i<LME_Vector.size(); i++)
    {
        LME_Set.insert(LME_Vector[i]);
    }
    //showAPSet("LME_Set", LME_Set);
    
    /* Finding \exists VarsToElim.\beta = \beta' This is always possible since all coefficients in \alpha are odd */
    DAGNode* QEConjunctionOfLMEs = APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(LME_Set,  VarsToElim,  WidthTable);
    fprintf(LMIQEOutputDataFp, "\nResult of QE from conjunction of LMEs = %s\n", (getDAGStr(QEConjunctionOfLMEs)).c_str());
    
    
    
    gettimeofday (&layer1_finish_ms, NULL);
    LMILayer1TimeLocal = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
    LMILayer1TimeLocal -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
    
    
    /* Finding \exists VarsToElim.\alpha' = \phi1 \and \exists VarsToElim. \phi2 */
    DAGNode* QEInlinedLMIConjunction=NULL; //\phi1
    DAGNode* QuantifiedInlinedLMIConjunction=NULL;// \phi2
    
    DAGNode* QEDag; // Final QE dag  
    
    //dropLMIsForMultipleVariables(InlinedLMI_Set,  VarsToElim,  WidthTable, QEInlinedLMIConjunction, QuantifiedInlinedLMIConjunction);
    
    gettimeofday (&layer2_start_ms, NULL); 
    
    dropLMIsForMultipleVariablesRecursive_ForWeakLMIs(InlinedLMI_Set,  VarsToElim,  WidthTable, QEInlinedLMIConjunction, QuantifiedInlinedLMIConjunction);
    
    
    gettimeofday (&layer2_finish_ms, NULL);
    LMILayer2TimeLocal = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
    LMILayer2TimeLocal -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;    
    
    
    
    if(isTrue(QuantifiedInlinedLMIConjunction))//\phi2 is true. Hence QE is done
        {
        QEDag = createDAG("and", QEConjunctionOfLMEs, QEInlinedLMIConjunction, LOGICAL_SYMBOL, 1);// result is \beta'/\ \phi1
        QuantifiedPart = QuantifiedInlinedLMIConjunction;
        fprintf(LMIQEOutputDataFp, "\nDropping simplies the part of the problem to true\n");
        return QEDag;
        }
    else
        {
        QEDag = createDAG("and", QEConjunctionOfLMEs, QEInlinedLMIConjunction, LOGICAL_SYMBOL, 1);// result is \beta'/\ \phi1
        QuantifiedPart = QuantifiedInlinedLMIConjunction;
        //cout<<"\nQuantifier-free part = "<<getDAGStr(QEDag)<<endl;
        fprintf(LMIQEOutputDataFp, "\nDropping could not simplify the part of the problem to true\n");
        //fprintf(LMIQEOutputDataFp, "\nWe need to eliminate quantifiers from %s\n", (getDAGStr(QuantifiedInlinedLMIConjunction)).c_str());
         //cout<<"\nWe need to eliminate quantifiers from "<<getDAGStr(QuantifiedInlinedLMIConjunction)<<endl;
        
        
        getNumberOfVarsToElimInLayer3Input_With_Recursive_Splitting(QuantifiedInlinedLMIConjunction, VarsToElim, LMIVarsInputToLayer3Local);
        
        
        return QEDag;
        //exit(1);
        }
    }//else
  
}// end of function




// Version of simplifiableLMIPairFound which works on BVULE and BVUGE constraints
bool simplifiableLMIPairFound_ForWeakLMIs(DAGNode* ugt, DAGNode* ule)
{
    list<DAGNode*>::iterator iout;
    iout = ugt->Get_outlist_start();
    DAGNode *ugt_LHS = *iout;
    iout++;
    DAGNode *ugt_RHS = *iout;	
    
    iout = ule->Get_outlist_start();
    DAGNode *ule_LHS = *iout;
    iout++;
    DAGNode *ule_RHS = *iout;	
    
    if(ugt_LHS != ule_LHS)
    {
        return false;
    }
    
    if(!isConstant(ugt_RHS) || !isConstant(ule_RHS))
    {
        return false;
    }
    
    string ugt_constant_str = ugt_RHS->Get_node_name();
    long long ugt_constant =  strtoull(ugt_constant_str.c_str(), NULL, 0);
    
    string ule_constant_str = ule_RHS->Get_node_name();
    long long ule_constant =  strtoull(ule_constant_str.c_str(), NULL, 0);
    
    if(ugt_constant == ule_constant)
    {
        return true;
    }
    
    return false;
}




// Version of dropLMIsForMultipleVariablesRecursive for BVULE and BVUGE constraints
void dropLMIsForMultipleVariablesRecursive_ForWeakLMIs(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction)
{
    set<string> OriginalVarsToElim;
    set<string> NewVarsToElim;
    set<DAGNode*> Bound_Constraints;
    DAGNode* QEInlinedLMIConjunctionInLoop;
    DAGNode* QuantifiedInlinedLMIConjunctionInLoop;
    
    
    QEInlinedLMIConjunction = NULL;
    OriginalVarsToElim = VarsToElim;
    Bound_Constraints = Constraints;
    
    
    
    while(true)
    {
        QEInlinedLMIConjunctionInLoop = NULL;
        QuantifiedInlinedLMIConjunctionInLoop = NULL;
        
        dropLMIsForMultipleVariables_ForWeakLMIs(Bound_Constraints, OriginalVarsToElim, WidthTable, QEInlinedLMIConjunctionInLoop, QuantifiedInlinedLMIConjunctionInLoop);
        
        if(QEInlinedLMIConjunction==NULL)
            {
            QEInlinedLMIConjunction = QEInlinedLMIConjunctionInLoop;
            }
        else
            {
            QEInlinedLMIConjunction = createDAG("and", QEInlinedLMIConjunctionInLoop, QEInlinedLMIConjunction, LOGICAL_SYMBOL, 1);
            }
        // Compute NewVarsToElim
        if(isTrue(QuantifiedInlinedLMIConjunctionInLoop))// QE is done already
        {
            //cout<<"\nQuantifiedInlinedLMIConjunctionInLoop is True. Hence exiting from loop\n";
            break;
        }
        
        //cout<<"\nQuantifiedInlinedLMIConjunctionInLoop is not True\n";
        set<string> Support;
        getDAGNames(QuantifiedInlinedLMIConjunctionInLoop, Support);
        NewVarsToElim.clear();
        set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(NewVarsToElim, NewVarsToElim.begin()));
        
        //showSet(OriginalVarsToElim, "OriginalVarsToElim");
        //showSet(NewVarsToElim, "NewVarsToElim");
        
        if(setsEqual(NewVarsToElim, OriginalVarsToElim))
        {
            //cout<<"\nOriginalVarsToElim ==  NewVarsToElim\n";
            break;
        }
        
        //cout<<"\nOriginalVarsToElim !=  NewVarsToElim\n";
        OriginalVarsToElim = NewVarsToElim;
        Bound_Constraints.clear();
	splitConjunctionIntoLMISet(QuantifiedInlinedLMIConjunctionInLoop, Bound_Constraints);        
    }// while(true)
    
    QuantifiedInlinedLMIConjunction = QuantifiedInlinedLMIConjunctionInLoop;
    
}// end of function





// Version of dropLMIsForMultipleVariables for BVUGE, BVULE constraints
void dropLMIsForMultipleVariables_ForWeakLMIs(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction)
{

  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  

  //showSet(VarsToElim,"VarsToElim");
  //showAPSet("Constraints", Constraints);



  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;

      //fprintf(LMIQEOutputDataFp, "\nVariableToElim = %s\n", VariableToElim.c_str());
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);

      QElim_Bound_Constraints.clear();
      
      vector< pair<DAGNode*, DAGNode*> > LMIPairs;
      identifyLMIPairs_ForWeakLMIs(Bound_Constraints, LMIPairs);
      //writeLMIPairs(LMIPairs);
      dropMultipleLMIPairs_ForWeakLMIs(Bound_Constraints, LMIPairs,  VariableToElim, QElim_Bound_Constraints); 
            
      //showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      //showAPSet("Constraints", Constraints);
    }// for ends here
  
  // Here "Constraints" has all the constraints. Just extract the constraints which contain the varstoelim and the others
  vector<DAGNode*> Constraints_Vector;
  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
  {
      Constraints_Vector.push_back(*it);
  }
  
  set<DAGNode*> Free_Constraints_Final;
  set<DAGNode*> Bound_Constraints_Final;
  
  getFreeAndBoundConstraintsForMultipleVariables(Constraints_Vector, VarsToElim, Free_Constraints_Final, Bound_Constraints_Final, WidthTable);
  
  for(set<DAGNode*>::iterator it=Free_Constraints_Final.begin(); it != Free_Constraints_Final.end(); it++)
    {
      if(QEInlinedLMIConjunction==NULL)
      {
          QEInlinedLMIConjunction = *it;
      }
      else
      {
          QEInlinedLMIConjunction = createDAG("and", QEInlinedLMIConjunction, *it, LOGICAL_SYMBOL, 1);
      }
    }// for
  
   for(set<DAGNode*>::iterator it=Bound_Constraints_Final.begin(); it != Bound_Constraints_Final.end(); it++)
    {
      if(QuantifiedInlinedLMIConjunction==NULL)
      {
          QuantifiedInlinedLMIConjunction = *it;
      }
      else
      {
          QuantifiedInlinedLMIConjunction = createDAG("and", QuantifiedInlinedLMIConjunction, *it, LOGICAL_SYMBOL, 1);
      }
    }// for
  
   if(QEInlinedLMIConjunction == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      QEInlinedLMIConjunction = dm.createNode("true",(void*)v);
    }
  
   if(QuantifiedInlinedLMIConjunction == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      QuantifiedInlinedLMIConjunction = dm.createNode("true",(void*)v);
    }

  }// end of function

 
 
 // Version of dropMultipleLMIPairs on BVULE, BVUGE constraints
void dropMultipleLMIPairs_ForWeakLMIs(set<DAGNode*> &Bound_Constraints, vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, set<DAGNode*> &QElim_Bound_Constraints)
{
    if(Bound_Constraints.empty())
    {
        QElim_Bound_Constraints.clear();
        return;
    }
    if(LMIPairs.size()==0)
    {
        QElim_Bound_Constraints = Bound_Constraints;
        return;
    }
    
    vector<long long int> Deltas;
        
    for(int i=0; i<LMIPairs.size(); i++)
    {
        DAGNode *ule = LMIPairs[i].first;
        DAGNode *ugt = LMIPairs[i].second;
        
        DAGNode *High=NULL;
        DAGNode *Low=NULL;
        DAGNode *Constraint_LHS=NULL;
        
        if(ule == NULL)
          {
            list<DAGNode*>::iterator iout;
            iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = NULL;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ugt; 
        }        
        else if(ugt==NULL)
            {
            list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
        
            High = RHS_ule;
            Low = NULL;
            Constraint_LHS = LHS_ule; 
            } 
        else if(ule != NULL && ugt != NULL)
        {
           list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
             iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = RHS_ule;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ule;  
        }
        else
        {
            cout<<"\nule and ugt both NULL in function dropMultipleLMIPairs_ForWeakLMIs in DAGEliminator.cpp\n";
            exit(1);
        }
        
        ULINT Coefficient;
        getCoefficientCorrected(Constraint_LHS, VariableToElim, Coefficient);// guaranteed to contain bvmul
        //cout<<"\nConstraint_LHS = "<<getDAGStr(Constraint_LHS)<<endl;
    
        if(Coefficient%2==0)// Coefficient is even
        {
            cout<<"\nEven coefficient encountered in function dropMultipleLMIPairs_ForWeakLMIs in DAGEliminator.cpp\n";
            exit(1);
        }
       
        long long int l2, u2;
        long long int delta;
    
        if(Low == NULL)
        {
            l2 = 0;
        }
        else
        {
            string Low_str = Low->Get_node_name();
            l2 = strtoull(Low_str.c_str(), NULL, 0);// Note that constraint is >=
        }
    
        if(High == NULL)
        {
            u2 = ModWidthInteger-1;
        }
        else
        {
            string High_str = High->Get_node_name();
            u2 = strtoull(High_str.c_str(), NULL, 0);
        }
    
        delta = u2-l2+1;
        
        //cout<<"\ndelta = "<<delta<<endl;
    
        Deltas.push_back(delta);       
    }// for
    
    //Check the condition 
    long long int SumDeltas = 0;
    
    for(int i=0; i<Deltas.size(); i++)
    {
        SumDeltas += Deltas[i];
    }
    
    long long int ToSubtract;
    
    ToSubtract = (Deltas.size()-1)*ModWidthInteger;
    
    //fprintf(LMIQEOutputDataFp, "\nSumDeltas = %d, ToSubtract =%d\n", SumDeltas, ToSubtract);
    
    long long int Eta = SumDeltas - ToSubtract;   
 
    if(Eta >=1)// We can drop the Bound_Constraints
    {
        //cout<<"\nEta  = "<<Eta<<endl;
        //fprintf(LMIQEOutputDataFp, "\n All the Bound_Constraints are dropped\n");
        QElim_Bound_Constraints.clear();
    }
    else
    {
        //cout<<"\nEta  = "<<Eta<<endl;
        //fprintf(LMIQEOutputDataFp, "\n None of the Bound_Constraints are dropped\n");
        QElim_Bound_Constraints = Bound_Constraints;
    }
}// end of function



// Convert strict LMIs in Constraints into weak LMIs
void convertStrictLMIsToWeakLMIs(vector<DAGNode*> &Constraints, map<string, int> &WidthTable)
{
    vector<DAGNode*> NewConstraints;
    
    for(int i=0; i<Constraints.size(); i++)
    {
        DAGNode* Constraint = Constraints[i];
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
        string LMIType = Constraint->Get_node_name();
               
        if(LMIType == "bvule" || LMIType == "bvuge")//already weak
        {
         DAGNode* NewConstraint;
         
         recreateWeakConstraint(Constraint, WidthTable, NewConstraint);
         // This helps in proper ordering of terms
         
         NewConstraints.push_back(NewConstraint);   
        }
        else//strict LMI
        {
            DAGNode* NewConstraint;
            
            if(LMIType=="bvugt") //LHS > RHS
            {
                 DAGNode* RecreatedConstraint1;
                 DAGNode* RecreatedConstraint2;
                 
                 set<string> TempSet;
                 
                 getRecreatedBVUGTConstraintsModified(LHS, RHS, WidthTable, RecreatedConstraint1, RecreatedConstraint2, TempSet);
                 
                 if(!isTrue(RecreatedConstraint1))
                 {
                   NewConstraints.push_back(RecreatedConstraint1);    
                 }
               NewConstraints.push_back(RecreatedConstraint2);    
            }//if(LMIType=="bvugt")
            else //We have LHS < RHS. Recreate using RHS > LHS
            {
                 DAGNode* RecreatedConstraint1;
                 DAGNode* RecreatedConstraint2;
                 
                 set<string> TempSet;
                 getRecreatedBVUGTConstraintsModified(RHS, LHS, WidthTable, RecreatedConstraint1, RecreatedConstraint2, TempSet);
                 
                 if(!isTrue(RecreatedConstraint1))
                 {
                   NewConstraints.push_back(RecreatedConstraint1);    
                 }
               NewConstraints.push_back(RecreatedConstraint2);    
            }//else of if(LMIType=="bvugt")
        } // else of strict LMI
    }//for
    
    
    Constraints = NewConstraints;
    
}//function



// Applying Splitting and Fourier-Motzkin on split dags with recursive splitting
DAGNode* expensiveLayersOnSetOfScopeReducedDAGs_With_Recursive_Splitting(set< set<DAGNode*> > &ScopeReducedDAGs , set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
    DAGNode *F = createLogicalConstant("true");
    
    int set_index=1;
    
    for(set< set<DAGNode*> >::iterator it=ScopeReducedDAGs.begin(); it != ScopeReducedDAGs.end(); it++)
    {
        fprintf(LMIQEOutputDataFp, "\nTaking the scope reduced set %d\n", set_index);
        
        set<DAGNode*> MyScopeReducedSet;
        MyScopeReducedSet = *it;
        DAGNode *MyConjunction = getConjunctionFromSet(MyScopeReducedSet);
        
        if(!isTrue(MyConjunction))
        {
            //cout<<"\nMyConjunction = "<<getDAGStr(MyConjunction)<<endl;
            vector<DAGNode*> MyConjunctionVector;
            copySetToVector(MyScopeReducedSet, MyConjunctionVector);
            DAGNode *Pi = call_expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(MyConjunctionVector, VarsToElim, WidthTable, project_depth);
            
            
             if(checkTimeOut())
            {
            // Time Out !!!
            cout<<"\nWarning!!!The function expensiveLayersOnSetOfScopeReducedDAGs_With_Recursive_Splitting in DAGEliminator.cpp has timed out. Its result is not exact\n";
            timed_out = true; // timed_out flag set
            return createLogicalConstant("true");
            }
            
            fprintf(LMIQEOutputDataFp, "\nQE from the scope reduced set %d is finished\n", set_index);
            //fprintf(LMIQEOutputDataFp, "\nResult of QE from scope reduced set %d = %s\n", set_index, getDAGStr(Pi).c_str());
            
            if(isTrue(F))
                {
                F = Pi;
                }
            else
             {
                F=createDAG("and", F, Pi, LOGICAL_SYMBOL, 1);
              }
        }//if(!isTrue(MyConjunction))
        set_index++;
    }//for
    return F;
}//function



// calls expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting  with dynamic programming
DAGNode* call_expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{    
 DAGNode* F = getConjunctionFromVector(Constraints);
 
 cout<<"\nF before call to expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting: "<<getDAGStr(F)<<endl;
 cout<<"\nF =  "<<F<<endl;
 showSet(VarsToElim, "VarsToElim");
    
  // Check in HashTable  
 DAGNode* Existing  = getEntryFromMonniaux_HT(F, VarsToElim);

  if(Existing != NULL)
    {
      Monniaux_HT_hits++;
      cout<<"\nHit in call_expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting\n";
      return Existing;
    }
 
 Monniaux_HT_misses++;
 cout<<"\nMiss in call_expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting\n";
 
 //Call lmiQEUsingOurMethods_With_Recursive_Splitting
 DAGNode* Result = expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(F, VarsToElim, WidthTable, project_depth);
 
 cout<<"\nF after call to expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting: "<<getDAGStr(F)<<endl;
 cout<<"\nF =  "<<F<<endl;
 showSet(VarsToElim, "VarsToElim");
 
 //Insert into HashTable
 insertEntryIntoMonniaux_HT(F, VarsToElim, Result);
 
 return Result;    
}





// Apply splitting and Fourier-Motzkin on each conjunction with recursive splitting and Monniaux calling
DAGNode* expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(DAGNode* Phi, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
 cout<<"\nCalling expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting on "<<getDAGStr(Phi)<<endl;
 showSet(VarsToElim, "VarsToElim");
 
 bool convert_BVUGE_to_BVULE=true;
 
 if(convert_BVUGE_to_BVULE)
    {
    cout<<"\nOriginal Phi = "<<getDAGStr(Phi)<<endl;
    Phi = convertBVUGEConstraintsToBVULE(Phi, WidthTable);
    cout<<"\nConverted Phi = "<<getDAGStr(Phi)<<endl;
    }
 
  set<DAGNode*> Constraints;
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QE_Bound_Constraints;
  
  
  splitConjunctionIntoSetGeneral(Phi, Constraints);
  //showAPSet("Constraints", Constraints);

  set<string> Support_Phi;
  set<string> VarsToElimInSupport;
  getDAGNames(Phi, Support_Phi);
  set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_Phi.begin(), Support_Phi.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));

  showSet(VarsToElimInSupport, "Variables to eliminate", LMIQEOutputDataFp); 
  showSet(VarsToElimInSupport,"VarsToElimInSupport");
  
  set<string> VarsToElimNeedOfSplitting;
  
  for(set<string>::iterator it = VarsToElimInSupport.begin(); it != VarsToElimInSupport.end(); it++)
    {
    string variable = *it;
    cout<<"\nvariable = "<<variable<<endl;
    showAPSet("Constraints", Constraints);
    
    if(constraintsAlreadyInNormalForm(Constraints, variable, WidthTable))
        {
        cout<<"\nThe constraints are already in normal form\n";
        Free_Constraints.clear();
        Bound_Constraints.clear();
        getFreeAndBoundConstraints(Constraints, variable, Free_Constraints, Bound_Constraints, WidthTable);
    
        DAGNode* resultOfFM = applyFourierMotzkin(Bound_Constraints, variable, WidthTable);
        
        cout<<"\nresultOfFM = "<<getDAGStr(resultOfFM)<<endl;
        
        QE_Bound_Constraints.clear();
        splitConjunctionIntoSetGeneral(resultOfFM, QE_Bound_Constraints);
        
        Constraints.clear();
        set_union(Free_Constraints.begin(), Free_Constraints.end(), QE_Bound_Constraints.begin(), QE_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));
	}
    else
        {   
          cout<<"\nThe constraints are not in normal form\n";
          VarsToElimNeedOfSplitting.insert(variable); 
        }
    }//for
 
  showSet(VarsToElimNeedOfSplitting, "VarsToElimNeedOfSplitting");
  
  if(VarsToElimNeedOfSplitting.empty())// all variables eliminated
    {
    cout<<"\nVarsToElimNeedOfSplitting.empty()\n";
    DAGNode* resultDAG = getConjunctionFromSet(Constraints);
    return resultDAG;
    }
  
  
  Free_Constraints.clear();
  Bound_Constraints.clear();
  
  vector<DAGNode*> Constraints_Vector;

  copySetToVector(Constraints, Constraints_Vector);
  getFreeAndBoundConstraintsForMultipleVariables(Constraints_Vector, VarsToElimNeedOfSplitting, Free_Constraints, Bound_Constraints, WidthTable);

  
  DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
  
  if(Bound_Constraints.empty())//No bound part
    {
      return FreePart;
    }
  
  DAGNode* BoundPart = getConjunctionFromSet(Bound_Constraints);
  DAGNode* QFree_BoundPart;
  bool intermediate_hashing = true;
  
  if(!intermediate_hashing)//no intermediate hashing
  {
 
    set<string>::iterator it = VarsToElimNeedOfSplitting.begin();
    string VariableSelected = *it;
    cout<<"\nVariable selected for elimination = "<<VariableSelected<<endl;
  
    DAGNode* NormalForm;
    
    bool single_variable_normalization = false;
    
    if(single_variable_normalization)
        {
        NormalForm = quickSimplify_WithLMISupport(convertToNormalForm(Bound_Constraints, VariableSelected, WidthTable));
        cout<<"\nNormalForm = "<<getDAGStr(NormalForm)<<endl;
        }
    else
        {
        set<string> VariablesSelectedForNormalization;
        selectVariablesForNormalization(Bound_Constraints, VarsToElimNeedOfSplitting, VariablesSelectedForNormalization, WidthTable);
        NormalForm = quickSimplify_WithLMISupport(convertToNormalFormForMultipleVariables(Bound_Constraints, VariablesSelectedForNormalization, WidthTable));
        cout<<"\nNormalForm = "<<getDAGStr(NormalForm)<<endl;        
        }
  
   
    cout<<"\nCalling Monniaux to eliminate quantifiers from NormalForm\n";
  
    DAGNode* QE_NormalForm = quickSimplify_WithLMISupport(callMonniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(NormalForm, VarsToElimNeedOfSplitting, WidthTable, project_depth+1));
  
    cout<<"\nQE_NormalForm = "<<getDAGStr(QE_NormalForm)<<endl;
    
    QFree_BoundPart = QE_NormalForm;
  }
  else //first check if \exists VarsToElimNeedOfSplitting. BoundPart is already in the hash table
  {
        // Check in HashTable  
        DAGNode* Existing  = getEntryFromMonniaux_HT(BoundPart, VarsToElimNeedOfSplitting);

        if(Existing != NULL)
            {
             Monniaux_HT_hits++;
            cout<<"\nHit in Monniaux_HT in expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting\n";
              //cout<<"\nExisting in call_lmiQEUsingOurMethods_With_Recursive_Splitting = "<<getDAGStr(Existing)<<endl;
            QFree_BoundPart =  Existing;
            }//if(Existing != NULL)
        else
            {
            Monniaux_HT_misses++;
            cout<<"\nMiss in Monniaux_HT in expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting\n";
 
            //Compute the result
            set<string>::iterator it = VarsToElimNeedOfSplitting.begin();
            string VariableSelected = *it;
            cout<<"\nVariable selected for elimination = "<<VariableSelected<<endl;
  
            DAGNode* NormalForm;
            
            
            bool single_variable_normalization = false;
    
            if(single_variable_normalization)
                {
                NormalForm = quickSimplify_WithLMISupport(convertToNormalForm(Bound_Constraints, VariableSelected, WidthTable));
                cout<<"\nNormalForm = "<<getDAGStr(NormalForm)<<endl;
                }
            else
                {
                set<string> VariablesSelectedForNormalization;
                selectVariablesForNormalization(Bound_Constraints, VarsToElimNeedOfSplitting, VariablesSelectedForNormalization, WidthTable);
                NormalForm = quickSimplify_WithLMISupport(convertToNormalFormForMultipleVariables(Bound_Constraints, VariablesSelectedForNormalization, WidthTable));
                cout<<"\nNormalForm = "<<getDAGStr(NormalForm)<<endl;
                //exit(1);
                }  
  
           cout<<"\nCalling Monniaux to eliminate quantifiers from NormalForm\n";
  
            DAGNode* QE_NormalForm = quickSimplify_WithLMISupport(callMonniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(NormalForm, VarsToElimNeedOfSplitting, WidthTable, project_depth+1));
  
            cout<<"\nQE_NormalForm = "<<getDAGStr(QE_NormalForm)<<endl;
    
            QFree_BoundPart = QE_NormalForm;
 
            //Insert into HashTable
            insertEntryIntoMonniaux_HT(BoundPart, VarsToElimNeedOfSplitting, QE_NormalForm);
        }//else
  }
  
  //exit(1);
  
  DAGNode* resultDAG = createDAG("and", FreePart, QFree_BoundPart, LOGICAL_SYMBOL, 1);
   
  return resultDAG;
}//function



// Convert the BVUGE Constraints to BVULE
DAGNode* convertBVUGEConstraintsToBVULE(DAGNode* Phi, map<string, int> &WidthTable)
{
   vector<DAGNode*> Constraints;
   splitConjunctionIntoLMIVector(Phi, Constraints);
   vector<DAGNode*> NewConstraints;
    
    for(int i=0; i<Constraints.size(); i++)
    {
        DAGNode* Constraint = Constraints[i];
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
        string LMIType = Constraint->Get_node_name();
               
        if(LMIType == "bvule")//already bvule
        {
         DAGNode* NewConstraint;         
         recreateWeakConstraint(Constraint, WidthTable, NewConstraint);
         // This helps in proper ordering of terms
         NewConstraints.push_back(NewConstraint);   
        }
        else if(LMIType == "bvuge")//bvuge constraint
        {
         DAGNode* NewConstraint;
         recreateWeakConstraintFromComponents(RHS, LHS, "bvule", WidthTable, NewConstraint);
         //recreate LHS >= RHS as RHS <= LHS
         NewConstraints.push_back(NewConstraint);   
        } // bvuge type
        else
        {
         cout<<"\nUnsupported LMIType "<<LMIType<<" in function convertBVUGEConstraintsToBVULE in DAGEliminator.cpp\n";
         exit(1);
        }
    }//for
    
    
DAGNode* resultDAG = getConjunctionFromVector(NewConstraints);
return resultDAG;
    
}//function



// convert the constraints in Constraints to normal form for the variable VarToElim, using splitting
DAGNode* convertToNormalForm(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable)
{
   if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function convertToNormalForm in DAGEliminator.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}
    
    
    int WidthOfVarToElim;
    ULINT ModOfVarToElim;
    
    map<string, int>::iterator wit = WidthTable.find(VarToElim);
    if(wit == WidthTable.end())
        {
        cout<<"\nNo entry for the variable "<<VarToElim<<" in WidthTable in function convertToNormalForm in DAGEliminator.cpp\n";
        exit(1);
        }
    WidthOfVarToElim = wit->second;
    ModOfVarToElim = findPower(WidthOfVarToElim);
    
    //cout<<"\nWidthOfVarToElim = "<<WidthOfVarToElim<<endl;
    //cout<<"\nModOfVarToElim = "<<ModOfVarToElim<<endl;
    
    vector<DAGNode*> Constraints_Vector;
    vector< vector<DAGNode*> > SplittedConstraints;
    
    // Let us ensure that we have only <= constraints
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* Constraint = *it;
        if(Constraint->Get_node_name() != "bvule")
        {
            cout<<"\nUnsupported constraint "<<getDAGStr(Constraint)<<" in function convertToNormalForm in DAGEliminator.cpp\n";
            exit(1);
        }
    }//for
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        Constraints_Vector.push_back(*it);        
    }
    
    ULINT CoefOfVarToElim;
    ULINT AIOfCoefOfVarToElim;
    bool first_constraint_encountered=false;
    
    for(int constraint_index=0; constraint_index < Constraints_Vector.size(); constraint_index++)
    {
        DAGNode *Constraint = Constraints_Vector[constraint_index];
        //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
        
        string side_of_variable = findSideOfVariableInConstraint(Constraint, VarToElim);
        //cout<<"\nside_of_variable = "<<side_of_variable<<endl;
        
        bool both_sides_implemented = true;
        if(!both_sides_implemented)
        {
               if(side_of_variable == "both")
                {
                 cout<<"\nThe variable "<<VarToElim<<" appears on both sides of "<<getDAGStr(Constraint)<<" in function convertToNormalForm in DAGEliminator.cpp"<<endl;
                exit(1);
                }//if(side_of_variable == "both")
        }
        
        if(side_of_variable == "none")
        {
            //cout<<"\nThe variable "<<VarToElim<<" does not appear on any side of "<<getDAGStr(Constraint)<<" in function convertToNormalForm in DAGEliminator.cpp"<<endl;
            
            vector<DAGNode*> SplittedVector;
            SplittedVector.push_back(Constraint);
            SplittedConstraints.push_back(SplittedVector);  
            continue;
        }//if(side_of_variable == "both")
        
        string type_of_coefficient = "original";
        if(!first_constraint_encountered)//first constraint
        {
            getCoefOfVarToElimAndAIOfCoefOfVarToElim(Constraint, VarToElim, side_of_variable, ModOfVarToElim, CoefOfVarToElim, AIOfCoefOfVarToElim);
            //cout<<"\nCoefOfVarToElim = "<<CoefOfVarToElim<<", AIOfCoefOfVarToElim = "<<AIOfCoefOfVarToElim<<endl;
            first_constraint_encountered = true;
        }//if(constr_index==0)
        else
        {
            type_of_coefficient = findTypeOfCoefficient(Constraint, VarToElim, side_of_variable, CoefOfVarToElim, AIOfCoefOfVarToElim);
            //cout<<"\ntype_of_coefficient = "<<type_of_coefficient<<endl;
        }// else of if(constr_index==0)
        
             
        vector<DAGNode*> SplittedVector;
        
        
        if(side_of_variable == "left" || side_of_variable == "right")
        {
           obtainSplittedVectorMadeEfficient(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector);
         }
        else if(side_of_variable == "both")
        {
            obtainSplittedVectorForLMIsWithBothSidesMadeEfficient(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector);
         }
        else
        {
          cout<<"\nUnexpected side_of_variable "<<side_of_variable<<" in function convertToNormalForm in DAGEliminator.cpp\n";
          exit(1);  
        }
        
        
      if(SplittedVector.size() > 0)  
      {
          SplittedConstraints.push_back(SplittedVector);
      }        
    }//for ends
 
 vector<DAGNode*> Final_Vector;
 
 for(int i=0; i<SplittedConstraints.size(); i++)
    {
      vector<DAGNode*> TempVector = SplittedConstraints[i];
      DAGNode* TempDAG = getDisjunctionFromVector(TempVector);
      Final_Vector.push_back(TempDAG);
    }
 
 DAGNode* resultDAG = getConjunctionFromVector(Final_Vector);
 return resultDAG;
}   
    
    
    
 


// Version of obtainSplittedVector, but with lesser number of disjuncts
void obtainSplittedVectorMadeEfficient(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector)
{    
    list<string> VarNames;
    VarNames.push_back(VarToElim);
    set<string> Support;
    getDAGNames(Constraint, Support);
    attachSetToList(Support, VarNames);
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *S = *iout;
    iout++;
    DAGNode *T = *iout;	
    
    getCoefsAndConstsModified(S, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(T, VarNames, T_Coef_Mat, T_Const_Mat);
    
    //showList(VarNames, "VarNames");
    //cout<<"\nS_Coef_Mat\n";
    //showMatrix(S_Coef_Mat);
    //cout<<"\nS_Const_Mat\n";
    //showMatrix(S_Const_Mat);
    //cout<<"\nT_Coef_Mat\n";
    //showMatrix(T_Coef_Mat);
    //cout<<"\nT_Const_Mat\n";
    //showMatrix(T_Const_Mat);
    
    Matrix e1_Coef_Mat, e1_Const_Mat, AIe1_Coef_Mat, AIe1_Const_Mat;
    Matrix e2_Coef_Mat, e2_Const_Mat;
    Matrix K_Coef_Mat, K_Const_Mat;
    
    
    if(side_of_variable=="left")
    {
        copyMatrix(S_Coef_Mat, K_Coef_Mat);
        nullifyColumnsExceptOne(K_Coef_Mat, 0);
        getNullMatrix(K_Const_Mat, 1, 1);
        
        if(type_of_coefficient == "negated")
            {
            aInverseOfMatrix(K_Coef_Mat, ModOfVarToElim);
            }
        
        copyMatrix(S_Coef_Mat, e1_Coef_Mat);
        copyMatrix(S_Const_Mat, e1_Const_Mat);
        nullifyColumn(e1_Coef_Mat, 0);
        
        copyMatrix(e1_Coef_Mat, AIe1_Coef_Mat);
        copyMatrix(e1_Const_Mat, AIe1_Const_Mat);
        aInverseOfMatrix(AIe1_Coef_Mat, ModOfVarToElim);
        aInverseOfMatrix(AIe1_Const_Mat, ModOfVarToElim);
    
        copyMatrix(T_Coef_Mat, e2_Coef_Mat);
        copyMatrix(T_Const_Mat, e2_Const_Mat);        
    }
    else  if(side_of_variable=="right")
    {
        copyMatrix(T_Coef_Mat, K_Coef_Mat);
        nullifyColumnsExceptOne(K_Coef_Mat, 0);
        getNullMatrix(K_Const_Mat, 1, 1);
        
         if(type_of_coefficient == "negated")
            {
            aInverseOfMatrix(K_Coef_Mat, ModOfVarToElim);
            }
        
        copyMatrix(T_Coef_Mat, e1_Coef_Mat);
        copyMatrix(T_Const_Mat, e1_Const_Mat);
        nullifyColumn(e1_Coef_Mat, 0);
        
        copyMatrix(e1_Coef_Mat, AIe1_Coef_Mat);
        copyMatrix(e1_Const_Mat, AIe1_Const_Mat);
        aInverseOfMatrix(AIe1_Coef_Mat, ModOfVarToElim);
        aInverseOfMatrix(AIe1_Const_Mat, ModOfVarToElim);
    
        copyMatrix(S_Coef_Mat, e2_Coef_Mat);
        copyMatrix(S_Const_Mat, e2_Const_Mat);     
    }
    else
    {
         cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function obtainSplittedVectorMadeEfficient in DAGEliminator.cpp\n";
          exit(1);
    }
    
    //cout<<"\ne1_Coef_Mat\n";
    //showMatrix(e1_Coef_Mat);
    //cout<<"\ne1_Const_Mat\n";
    //showMatrix(e1_Const_Mat);
    //cout<<"\nAIe1_Coef_Mat\n";
    //showMatrix(AIe1_Coef_Mat);
    //cout<<"\nAIe1_Const_Mat\n";
    //showMatrix(AIe1_Const_Mat);
    //cout<<"\ne2_Coef_Mat\n";
    //showMatrix(e2_Coef_Mat);
    //cout<<"\ne2_Const_Mat\n";
    //showMatrix(e2_Const_Mat);
    //cout<<"\nK_Coef_Mat\n";
    //showMatrix(K_Coef_Mat);
    //cout<<"\nK_Const_Mat\n";
    //showMatrix(K_Const_Mat);
    
    
    Matrix e1MinusOne_Coef_Mat, e1MinusOne_Const_Mat;
    Matrix AIe1MinusOne_Coef_Mat, AIe1MinusOne_Const_Mat;
    Matrix AIe1Pluse2_Coef_Mat, AIe1Pluse2_Const_Mat;
    Matrix AIOfAIe1MinusOne_Coef_Mat, AIOfAIe1MinusOne_Const_Mat;
    Matrix AIOfAIe1Pluse2_Coef_Mat, AIOfAIe1Pluse2_Const_Mat;
        
    Matrix One_Coef_Mat, One_Const_Mat, MinusOne_Coef_Mat, MinusOne_Const_Mat;
    Matrix Zero_Coef_Mat, Zero_Const_Mat;
    
    getNullMatrix(One_Coef_Mat, 1, VarNames.size());
    getNullMatrix(MinusOne_Coef_Mat, 1, VarNames.size());
    getNullMatrix(One_Const_Mat, 1, 1);
    One_Const_Mat[0][0]=1;
    getNullMatrix(MinusOne_Const_Mat, 1, 1);
    MinusOne_Const_Mat[0][0]=ModOfVarToElim-1;
    getNullMatrix(Zero_Coef_Mat, 1, VarNames.size());
    getNullMatrix(Zero_Const_Mat, 1, 1);
    
    // Finding e1-1
    copyMatrix(e1_Coef_Mat, e1MinusOne_Coef_Mat);
    copyMatrix(e1_Const_Mat, e1MinusOne_Const_Mat);
    matAdd(e1MinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
    matAdd(e1MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
    
    // Finding AI(e1)-1
    copyMatrix(AIe1_Coef_Mat, AIe1MinusOne_Coef_Mat);
    copyMatrix(AIe1_Const_Mat, AIe1MinusOne_Const_Mat);
    matAdd(AIe1MinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
    matAdd(AIe1MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
    
     // Finding AI(e1)+e2
    copyMatrix(AIe1_Coef_Mat, AIe1Pluse2_Coef_Mat);
    copyMatrix(AIe1_Const_Mat, AIe1Pluse2_Const_Mat);
    matAdd(AIe1Pluse2_Coef_Mat, e2_Coef_Mat, ModOfVarToElim);
    matAdd(AIe1Pluse2_Const_Mat, e2_Const_Mat, ModOfVarToElim);
    
    // Finding AI(AI(e1)-1)
    copyMatrix(AIe1MinusOne_Coef_Mat, AIOfAIe1MinusOne_Coef_Mat);
    copyMatrix(AIe1MinusOne_Const_Mat, AIOfAIe1MinusOne_Const_Mat);
    aInverseOfMatrix(AIOfAIe1MinusOne_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AIOfAIe1MinusOne_Const_Mat, ModOfVarToElim);
    
     // Finding AI(AI(e1)+e2)
    copyMatrix(AIe1Pluse2_Coef_Mat, AIOfAIe1Pluse2_Coef_Mat);
    copyMatrix(AIe1Pluse2_Const_Mat, AIOfAIe1Pluse2_Const_Mat);
    aInverseOfMatrix(AIOfAIe1Pluse2_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AIOfAIe1Pluse2_Const_Mat, ModOfVarToElim);
    
    
    //cout<<"\ne1MinusOne_Coef_Mat\n"; showMatrix(e1MinusOne_Coef_Mat);
    //cout<<"\ne1MinusOne_Const_Mat\n"; showMatrix(e1MinusOne_Const_Mat);
    
    //cout<<"\nAIe1MinusOne_Coef_Mat\n"; showMatrix(AIe1MinusOne_Coef_Mat);
    //cout<<"\nAIe1MinusOne_Const_Mat\n"; showMatrix(AIe1MinusOne_Const_Mat);
    
    //cout<<"\nAIe1Pluse2_Coef_Mat\n"; showMatrix(AIe1Pluse2_Coef_Mat);
    //cout<<"\nAIe1Pluse2_Const_Mat\n"; showMatrix(AIe1Pluse2_Const_Mat);
    
    //cout<<"\nAIOfAIe1MinusOne_Coef_Mat\n"; showMatrix(AIOfAIe1MinusOne_Coef_Mat);
    //cout<<"\nAIOfAIe1MinusOne_Const_Mat\n"; showMatrix(AIOfAIe1MinusOne_Const_Mat);
    
    //cout<<"\nAIOfAIe1Pluse2_Coef_Mat\n"; showMatrix(AIOfAIe1Pluse2_Coef_Mat);
    //cout<<"\nAIOfAIe1Pluse2_Const_Mat\n"; showMatrix(AIOfAIe1Pluse2_Const_Mat);
    
       
    DAGNode *e1GteOne, *e1MinusOneGtee2, *kxLteAIe1MinusOne, *kxGteAIe1Pluse2;
    DAGNode *e1Ltee2, *kxGteAIe1, *kxLteAIe1Pluse2;
    DAGNode *kxLtee1, *kxGteAIOfAIe1Pluse2;
    DAGNode *kxGteAIOfAIe1MinusOne, *kxLteAIOfAIe1Pluse2;
    
    vector<string> LMITypesBVULE;
    LMITypesBVULE.push_back("bvule");
    
    vector<string> LMITypesBVUGE;
    LMITypesBVUGE.push_back("bvuge");
    
    
    e1GteOne = quickSimplify(getDAGFromInequations(e1_Coef_Mat, e1_Const_Mat, One_Coef_Mat, One_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
    //cout<<"\ne1GteOne = "<<getDAGStr(e1GteOne)<<endl;
    
    e1MinusOneGtee2 = quickSimplify(getDAGFromInequations(e1MinusOne_Coef_Mat, e1MinusOne_Const_Mat, e2_Coef_Mat, e2_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
    //cout<<"\ne1MinusOneGtee2 = "<<getDAGStr(e1MinusOneGtee2)<<endl;
    
    kxLteAIe1MinusOne = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIe1MinusOne_Coef_Mat, AIe1MinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
    //cout<<"\nkxLteAIe1MinusOne = "<<getDAGStr(kxLteAIe1MinusOne)<<endl;
    
    kxGteAIe1Pluse2 = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIe1Pluse2_Coef_Mat, AIe1Pluse2_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
    //cout<<"\nkxGteAIe1Pluse2 = "<<getDAGStr(kxGteAIe1Pluse2)<<endl;
    
    e1Ltee2 = quickSimplify(getDAGFromInequations(e1_Coef_Mat, e1_Const_Mat, e2_Coef_Mat, e2_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
    //cout<<"\ne1Ltee2 = "<<getDAGStr(e1Ltee2)<<endl;
    
    kxGteAIe1 = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIe1_Coef_Mat, AIe1_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
    //cout<<"\nkxGteAIe1Ltekx = "<<getDAGStr(kxGteAIe1)<<endl;
    
    kxLteAIe1Pluse2 = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIe1Pluse2_Coef_Mat, AIe1Pluse2_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
    //cout<<"\nkxLteAIe1Pluse2 = "<<getDAGStr(kxLteAIe1Pluse2)<<endl;
   
    kxLtee1 = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, e1_Coef_Mat, e1_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
    //cout<<"\nkxLtee1 = "<<getDAGStr(kxLtee1)<<endl;
    
    kxGteAIOfAIe1Pluse2 = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIOfAIe1Pluse2_Coef_Mat, AIOfAIe1Pluse2_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
    //cout<<"\nkxGteAIOfAIe1Pluse2 = "<<getDAGStr(kxGteAIOfAIe1Pluse2)<<endl;
    
    kxGteAIOfAIe1MinusOne = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIOfAIe1MinusOne_Coef_Mat, AIOfAIe1MinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
    //cout<<"\nkxGteAIOfAIe1MinusOne = "<<getDAGStr(kxGteAIOfAIe1MinusOne)<<endl;
    
    kxLteAIOfAIe1Pluse2 = quickSimplify(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIOfAIe1Pluse2_Coef_Mat, AIOfAIe1Pluse2_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
    //cout<<"\nkxLteAIOfAIe1Pluse2 = "<<getDAGStr(kxLteAIOfAIe1Pluse2)<<endl;
    
        
    //cout<<"\nside_of_variable = "<<side_of_variable<<", type_of_coefficient = "<<type_of_coefficient<<endl;
    
    if(type_of_coefficient == "original")
    {
       
            if(side_of_variable == "right")
               {
                   vector<DAGNode*> Constraint1_Vector;
                   Constraint1_Vector.push_back(e1GteOne);
                   Constraint1_Vector.push_back(e1MinusOneGtee2);
                   Constraint1_Vector.push_back(kxLteAIe1MinusOne);
                   
                   DAGNode* Constraint1 = getConjunctionFromVector(Constraint1_Vector);
                   SplittedVector.push_back(Constraint1);
                   SplittedVector.push_back(kxGteAIe1Pluse2);
               }
            else if(side_of_variable == "left")
               {
                   vector<DAGNode*> Constraint1_Vector;
                   Constraint1_Vector.push_back(e1GteOne);
                   Constraint1_Vector.push_back(e1Ltee2);
                   Constraint1_Vector.push_back(kxGteAIe1);
                   
                   DAGNode* Constraint1 = getConjunctionFromVector(Constraint1_Vector);
                   SplittedVector.push_back(Constraint1);
                   SplittedVector.push_back(kxLteAIe1Pluse2);
               }
               else
               {
                   cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function obtainSplittedVectorMadeEfficient in DAGEliminator.cpp\n";
                   exit(1);
               }
    }
    else if(type_of_coefficient == "negated")
    {
       
            if(side_of_variable == "right")
               {
                   vector<DAGNode*> Constraint1_Vector;
                   Constraint1_Vector.push_back(e1GteOne);
                   Constraint1_Vector.push_back(e1MinusOneGtee2);
                   Constraint1_Vector.push_back(kxGteAIOfAIe1MinusOne);
                   
                   DAGNode* Constraint1 = getConjunctionFromVector(Constraint1_Vector);
                   SplittedVector.push_back(Constraint1);
                   SplittedVector.push_back(kxLteAIOfAIe1Pluse2);
               }
            else if(side_of_variable == "left")
               {
                   vector<DAGNode*> Constraint1_Vector;
                   Constraint1_Vector.push_back(e1GteOne);
                   Constraint1_Vector.push_back(e1Ltee2);
                   Constraint1_Vector.push_back(kxLtee1);
                   
                   DAGNode* Constraint1 = getConjunctionFromVector(Constraint1_Vector);
                   SplittedVector.push_back(Constraint1);
                   SplittedVector.push_back(kxGteAIOfAIe1Pluse2);
               }
               else
               {
                   cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function obtainSplittedVectorMadeEfficient in DAGEliminator.cpp\n";
                   exit(1);
               }
    }
    else
    {
     
     cout<<"\nIllegal value "<<type_of_coefficient<<" for type_of_coefficient in function obtainSplittedVectorMadeEfficient in DAGEliminator.cpp\n";
     exit(1);
     
    }    
    
    //showAPVector("SplittedVector", SplittedVector);   
   
}// end of function




// Version of obtainSplittedVectorForLMIsWithBothSides with lesser number of disjuncts
void obtainSplittedVectorForLMIsWithBothSidesMadeEfficient(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector)
{    
    list<string> VarNames;
    VarNames.push_back(VarToElim);
    set<string> Support;
    getDAGNames(Constraint, Support);
    attachSetToList(Support, VarNames);
    
    cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *S = *iout;
    iout++;
    DAGNode *T = *iout;	
    
    getCoefsAndConstsModified(S, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(T, VarNames, T_Coef_Mat, T_Const_Mat);
    
    //showList(VarNames, "VarNames");
    //cout<<"\nS_Coef_Mat\n";
    //showMatrix(S_Coef_Mat);
    //cout<<"\nS_Const_Mat\n";
    //showMatrix(S_Const_Mat);
    //cout<<"\nT_Coef_Mat\n";
    //showMatrix(T_Coef_Mat);
    //cout<<"\nT_Const_Mat\n";
    //showMatrix(T_Const_Mat);
    
    Matrix E_Coef_Mat, E_Const_Mat, F_Coef_Mat, F_Const_Mat;
    Matrix Var_Coef_Mat, Var_Const_Mat;
    Matrix AIVar_Coef_Mat, AIVar_Const_Mat;
    ULINT E, F;
    
    copyMatrix(S_Coef_Mat, E_Coef_Mat);
    copyMatrix(S_Const_Mat, E_Const_Mat);
    nullifyColumn(E_Coef_Mat, 0);
    E = E_Const_Mat[0][0];
    
    copyMatrix(T_Coef_Mat, F_Coef_Mat);
    copyMatrix(T_Const_Mat, F_Const_Mat);
    nullifyColumn(F_Coef_Mat, 0);
    F = F_Const_Mat[0][0];
    
    copyMatrix(S_Coef_Mat, Var_Coef_Mat);
    nullifyColumnsExceptOne(Var_Coef_Mat, 0);
    getNullMatrix(Var_Const_Mat, 1, 1);
    
    copyMatrix(Var_Coef_Mat, AIVar_Coef_Mat);
    copyMatrix(Var_Const_Mat, AIVar_Const_Mat);
    aInverseOfMatrix(AIVar_Coef_Mat, ModOfVarToElim);
    aInverseOfMatrix(AIVar_Const_Mat, ModOfVarToElim);
    
    if(IsNullMatrix(E_Coef_Mat)==0 || IsNullMatrix(F_Coef_Mat)==0)
    {
        cout<<"\nDifference between the LHS and RHS of "<<getDAGStr(Constraint)<<" is not a constant in function obtainSplittedVectorForLMIsWithBothSidesMadeEfficient in DAGEliminator.cpp\n";
        exit(1);
    }  
    
    if(E == 0 && F==0)
    {
        cout<<"\nNo need to reconstruct "<<getDAGStr(Constraint)<<" as both the LHS and RHS are the same in function obtainSplittedVectorForLMIsWithBothSidesMadeEfficient in DAGEliminator.cpp\n";
        return;
    }
    
    if(E > F)
    {
        if(F==0)
        {
            DAGNode* ReconstructedConstraint;
            
            if(type_of_coefficient=="original")
                {
                aInverseOfMatrix(E_Const_Mat, ModOfVarToElim);
                vector<string> LMITypes;
                LMITypes.push_back("bvuge");
                ReconstructedConstraint = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, E_Coef_Mat, E_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                }
            else if(type_of_coefficient=="negated")
                {
                vector<string> LMITypes;
                LMITypes.push_back("bvule");
                ReconstructedConstraint = quickSimplify(getDAGFromInequations(AIVar_Coef_Mat, AIVar_Const_Mat, E_Coef_Mat, E_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                }
            else
                {
                cout<<"\nUnknown value "<<type_of_coefficient<<" for type_of_coefficient in function obtainSplittedVectorForLMIsWithBothSidesMadeEfficient in DAGEliminator.cpp\n";
                exit(1);
                }
            
            SplittedVector.push_back(ReconstructedConstraint);            
         }// if(F==0)
        else
        {
            DAGNode* ReconstructedConstraint1;
            DAGNode* ReconstructedConstraint2;
            DAGNode* ReconstructedConstraint;
            
             if(type_of_coefficient=="original")
                {
                aInverseOfMatrix(E_Const_Mat, ModOfVarToElim);
            
                ULINT AI_of_F = aInverse(F, ModOfVarToElim);
                ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
                Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
                getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
                getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
                AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
            
                vector<string> LMITypes;
                LMITypes.push_back("bvuge");
                ReconstructedConstraint1 = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, E_Coef_Mat, E_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
                LMITypes.clear();
                LMITypes.push_back("bvule");
                ReconstructedConstraint2 = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
             }
             else if(type_of_coefficient=="negated")
                {
                          
                ULINT F_Plus_One = add(F, 1, ModOfVarToElim);
                Matrix F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat;
                getNullMatrix(F_Plus_One_Coef_Mat, 1, VarNames.size());
                getNullMatrix(F_Plus_One_Const_Mat, 1, 1);
                F_Plus_One_Const_Mat[0][0] = F_Plus_One;
            
                vector<string> LMITypes;
                LMITypes.push_back("bvuge");
                ReconstructedConstraint1 = quickSimplify(getDAGFromInequations(AIVar_Coef_Mat, AIVar_Const_Mat, F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
                LMITypes.clear();
                LMITypes.push_back("bvule");
                ReconstructedConstraint2 = quickSimplify(getDAGFromInequations(AIVar_Coef_Mat, AIVar_Const_Mat, E_Coef_Mat, E_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
             }
            else
                {
                cout<<"\nUnknown value "<<type_of_coefficient<<" for type_of_coefficient in function obtainSplittedVectorForLMIsWithBothSidesMadeEfficient in DAGEliminator.cpp\n";
                exit(1);
                }
            
           ReconstructedConstraint=createDAG("and", ReconstructedConstraint1, ReconstructedConstraint2, LOGICAL_SYMBOL, 1); 
           SplittedVector.push_back(ReconstructedConstraint);      
         }// else of if(F==0)
    }// if(E > F)
    else
    {
        if(E==0)
        {                    
            ULINT AI_of_F = aInverse(F, ModOfVarToElim);
            ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
            Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
            getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
            getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
            AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
            
            Matrix F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat;
            copyMatrix(AI_of_F_Minus_One_Coef_Mat, F_Plus_One_Coef_Mat);
            copyMatrix(AI_of_F_Minus_One_Const_Mat, F_Plus_One_Const_Mat);
            aInverseOfMatrix(F_Plus_One_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(F_Plus_One_Const_Mat, ModOfVarToElim);
            
            DAGNode* ReconstructedConstraint;
            
             if(type_of_coefficient=="original")
                {
                vector<string> LMITypes;
                LMITypes.push_back("bvule");

                ReconstructedConstraint = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
                }
             else if(type_of_coefficient=="negated")
                {
                vector<string> LMITypes;
                LMITypes.push_back("bvuge");

                ReconstructedConstraint = quickSimplify(getDAGFromInequations(AIVar_Coef_Mat, AIVar_Const_Mat, F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
                }
            else
                {
                cout<<"\nUnknown value "<<type_of_coefficient<<" for type_of_coefficient in function obtainSplittedVectorForLMIsWithBothSidesMadeEfficient in DAGEliminator.cpp\n";
                exit(1);
                }
            SplittedVector.push_back(ReconstructedConstraint);   
          }// if(E==0)
        else
        {
            DAGNode* ReconstructedConstraint1;
            DAGNode* ReconstructedConstraint2;
            
             if(type_of_coefficient=="original")
                {
                aInverseOfMatrix(E_Const_Mat, ModOfVarToElim);
            
                ULINT AI_of_F = aInverse(F, ModOfVarToElim);
                ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
                Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
                getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
                getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
                AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
            
                vector<string> LMITypes;
                LMITypes.push_back("bvuge");
                ReconstructedConstraint1 = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, E_Coef_Mat, E_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
                LMITypes.clear();
                LMITypes.push_back("bvule");
                ReconstructedConstraint2 = quickSimplify(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
             }
             else if(type_of_coefficient=="negated")
                {
                          
                ULINT F_Plus_One = add(F, 1, ModOfVarToElim);
                Matrix F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat;
                getNullMatrix(F_Plus_One_Coef_Mat, 1, VarNames.size());
                getNullMatrix(F_Plus_One_Const_Mat, 1, 1);
                F_Plus_One_Const_Mat[0][0] = F_Plus_One;
            
                vector<string> LMITypes;
                LMITypes.push_back("bvuge");
                ReconstructedConstraint1 = quickSimplify(getDAGFromInequations(AIVar_Coef_Mat, AIVar_Const_Mat, F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
                LMITypes.clear();
                LMITypes.push_back("bvule");
                ReconstructedConstraint2 = quickSimplify(getDAGFromInequations(AIVar_Coef_Mat, AIVar_Const_Mat, E_Coef_Mat, E_Const_Mat, VarNames, ModOfVarToElim, LMITypes));
             }
            else
                {
                cout<<"\nUnknown value "<<type_of_coefficient<<" for type_of_coefficient in function obtainSplittedVectorForLMIsWithBothSidesMadeEfficient in DAGEliminator.cpp\n";
                exit(1);
                }
            
           SplittedVector.push_back(ReconstructedConstraint1);     
           SplittedVector.push_back(ReconstructedConstraint1);    
         }// else of if(E==0)
    }//else of if(E > F)
    
    showAPVector("SplittedVector", SplittedVector);
}//end of function




// Version of getNumberOfVarsToElimInLayer2Input s.t. change is made in the parameters
void getNumberOfVarsToElimInLayer2Input_With_Recursive_Splitting(DAGNode* Constraint, set<string> &VarsToElim, int &LMIVarsInputToLayer2Local)
{
    set<string> Support;
    set<string> CommonVars;
    
    getDAGNames(Constraint, Support);
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMIVarsInputToLayer2Local = CommonVars.size();    
}


// Version of getNumberOfVarsToElimInLayer3Input s.t. change is made in the parameters
void getNumberOfVarsToElimInLayer3Input_With_Recursive_Splitting(DAGNode* Constraint, set<string> &VarsToElim, int &LMIVarsInputToLayer3Local)
{
    set<string> Support;
    set<string> CommonVars;
    
    getDAGNames(Constraint, Support);
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMIVarsInputToLayer3Local = CommonVars.size();    
}


// Version of getNumberOfVarsToElimInLayer4Input s.t. change is made in the parameters
void getNumberOfVarsToElimInLayer4Input_With_Recursive_Splitting(DAGNode* Constraint, set<string> &VarsToElim, int &LMIVarsInputToLayer4Local)
{
    set<string> Support;
    set<string> CommonVars;
    
    getDAGNames(Constraint, Support);
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMIVarsInputToLayer4Local = CommonVars.size();    
}
    



// QE from conjunction of LMIs using LME based simplification followed by bit-blasting and CUDD
DAGNode* lmiQEUsingLMEBasedReasoningAndBlasting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  bool CallLMEBasedSimplification = true;
  
   showAPVector("Constraints", Constraints);
   convertStrictLMIsToWeakLMIs(Constraints, WidthTable);
   showAPVector("Constraints", Constraints);   
  
  showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
  
  showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);
  
  
   DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
           Bound_Constraints_Vector.push_back(*it); 
        }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
    
  if(CallLMEBasedSimplification)
  {
      DAGNode* Quantified_Bound_Part;
      DAGNode* Simplified_Bound_Part = LMEBasedSimplification_ForWeakLMIs(Bound_Constraints, VarsToElim, WidthTable, Quantified_Bound_Part);
      DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
      
      if(isTrue(Quantified_Bound_Part))
      {
          DAGNode* resultDAG;
          resultDAG = createDAG("and", Simplified_Bound_Part, Free_Part, LOGICAL_SYMBOL, 1);
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
      }
      else
      {
          DAGNode* PartialResultDAG;
          PartialResultDAG = createDAG("and", Simplified_Bound_Part, Free_Part, LOGICAL_SYMBOL, 1);
          //cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
          //cout<<"\nWe still need to eliminate quantifiers from "<<getDAGStr(Quantified_Bound_Part)<<endl;
          
         set<string> Quantified_Bound_Part_Support;
         set<string> CommonVars;
    
        getDAGNames(Quantified_Bound_Part, Quantified_Bound_Part_Support);
        set_intersection(VarsToElim.begin(), VarsToElim.end(), Quantified_Bound_Part_Support.begin(), Quantified_Bound_Part_Support.end(),inserter(CommonVars, CommonVars.begin()));

          
         vector<DAGNode*> Quantified_Bound_Part_Constraints;
         splitConjunctionIntoLMIVector(Quantified_Bound_Part, Quantified_Bound_Part_Constraints);
         
         cout<<"\nPress any key to start bit-blasting based QE\n";
         char c = getchar();
          
         DAGNode* ResultFromCUDD = QE_Using_Blasting_WithSupportForLMIs(Quantified_Bound_Part_Constraints, CommonVars, WidthTable);

         DAGNode* resultDAG;
         resultDAG = createDAG("and", PartialResultDAG, ResultFromCUDD, LOGICAL_SYMBOL, 1);
         //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
         return resultDAG;
      }
  }//if(CallLMEBasedSimplification)
}// end of function


// Apply LMEBasedSimplification on weak LMIs
DAGNode* LMEBasedSimplification_ForWeakLMIs(set<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable, DAGNode* &QuantifiedPart)
{
    
 vector<DAGNode*> BVULE_Vector;// set of constraints in Constraints of the form <=
 vector<DAGNode*> BVUGT_Vector;// Set of constraints in Constraints of the form >=
 
 for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
        {
           DAGNode* MyConstraint = *it;
           if(MyConstraint->Get_node_name()=="bvule")
           {
               BVULE_Vector.push_back(MyConstraint);
           }
           else if(MyConstraint->Get_node_name()=="bvuge")
           {
               BVUGT_Vector.push_back(MyConstraint);
           }
           else
           {
             cout<<"\nUnknown predicate "<<MyConstraint->Get_node_name()<<" inside function LMEBasedSimplification_ForWeakLMIs in DAGEliminator.cpp\n";
             exit(1);
           }             
           
        }//for ends here
 
 
 //showAPVector("BVULE_Vector", BVULE_Vector);
 //showAPVector("BVUGT_Vector", BVUGT_Vector);
      

set<DAGNode*> AlreadyDone;
AlreadyDone.clear();

vector< pair<DAGNode*, DAGNode*> > SimplifiableLMIPairs;

while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<BVUGT_Vector.size();i++)
	{
          DAGNode* ugt = BVUGT_Vector[i];
          //cout<<"\nTaking LMI "<<getDAGStr(ugt)<<endl;
          if(AlreadyDone.find(ugt)!=AlreadyDone.end()) // We have already checked if there exists a pair for ugt
		{
		//cout<<"\nAlready Done\n";
		continue;
		}
          
          for(int j=0; j<BVULE_Vector.size(); j++)
          {
              DAGNode* ule = BVULE_Vector[j];
              
              if(simplifiableLMIPairFound_ForWeakLMIs(ugt, ule))
              {
                  //cout<<"\nMatches with "<<getDAGStr(ule)<<endl;
                  //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
                  SimplifiableLMIPairs.push_back(make_pair(ule, ugt));
                  BVUGT_Vector.erase(BVUGT_Vector.begin()+i);
                  BVULE_Vector.erase(BVULE_Vector.begin()+j);
                  fixpoint_reached=false;
                  break;
              }
          }
	
        if(!fixpoint_reached)// BVUGT_Vector is changed. Restart the loop
        {
            //cout<<"\nStarting from the beginning of BVUGT_Vector\n";
            break;
        }
        
        //cout<<"\nNo matching ule constraint found\n";
	AlreadyDone.insert(ugt);
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here


vector<DAGNode*> LME_Vector;

synthesizeLMEsFromLMIs(SimplifiableLMIPairs, LME_Vector, WidthTable);

//cout<<"\nThe problem we need to solve is\n";

//showSet(VarsToElim, "VarsToElim");
//showAPVector("BVULE_Vector", BVULE_Vector);
//showAPVector("BVUGT_Vector", BVUGT_Vector);
//showAPVector("LME_Vector", LME_Vector);


showAPVector("LMEs synthesized from LMIs", LME_Vector, LMIQEOutputDataFp);

DAGNode *ConjunctionOfLMIs = NULL;

 for(int i=0;i<BVULE_Vector.size();i++)
	{
          DAGNode* ule = BVULE_Vector[i];
          if(ConjunctionOfLMIs == NULL)
          {
              ConjunctionOfLMIs = ule;
          }
          else
          {
             ConjunctionOfLMIs=createDAG("and",  ConjunctionOfLMIs, ule, LOGICAL_SYMBOL, 1); 
          }
     }// for

 for(int i=0;i<BVUGT_Vector.size();i++)
	{
          DAGNode* ugt = BVUGT_Vector[i];
          if(ConjunctionOfLMIs == NULL)
          {
              ConjunctionOfLMIs = ugt;
          }
          else
          {
           ConjunctionOfLMIs=createDAG("and",  ConjunctionOfLMIs, ugt, LOGICAL_SYMBOL, 1);    
          }
     }// for

if(ConjunctionOfLMIs == NULL)
    {
    //cout<<"\nAll LMIs are converted to LMEs by the function LMEBasedSimplification in DAGEliminatir.cpp\n";
    
    fprintf(LMIQEOutputDataFp, "\nAll LMIs are converted to LMEs\n");
    
    set<DAGNode*> LME_Set;
    for(int i=0; i<LME_Vector.size(); i++)
    {
        LME_Set.insert(LME_Vector[i]);
    }
    //showAPSet("LME_Set", LME_Set);
    DAGNode* QEConjunctionOfLMEs = APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(LME_Set,  VarsToElim,  WidthTable);
    //cout<<"\nQEConjunctionOfLMEs = "<<getDAGStr(QEConjunctionOfLMEs)<<endl;
    //fprintf(LMIQEOutputDataFp, "\nResult of QE = %s\n", (getDAGStr(QEConjunctionOfLMEs)).c_str());
    DAGNode* QEDag = QEConjunctionOfLMEs;
    QuantifiedPart = createLogicalConstant("true");
    
    return QEDag;
    }
else
    {
    /* We have \exists VarsToElim.(\alpha/\ \beta) here, where \alpha = ConjunctionOfLMIs  and 
     \beta = conjunction of LMEs in LME_Vector  */
    
    //cout<<"\nConjunctionOfLMIs = "<<getDAGStr(ConjunctionOfLMIs)<<endl;
    
    /* Simplify \alpha to \alpha' using \beta */
    DAGNode* InlinedConjunctionOfLMIs = quickSimplify_WithLMISupport(inlineLMIConjunctionUsingLMEs(ConjunctionOfLMIs, VarsToElim, LME_Vector, WidthTable));
    
    //cout<<"\nInlinedConjunctionOfLMIs = "<<getDAGStr(InlinedConjunctionOfLMIs)<<endl;
    //fprintf(LMIQEOutputDataFp, "\nConjunctionOfLMIs after simplification by LMEs = %s\n", (getDAGStr(InlinedConjunctionOfLMIs)).c_str());
    
      
    set<DAGNode*> LME_Set;
    for(int i=0; i<LME_Vector.size(); i++)
    {
        LME_Set.insert(LME_Vector[i]);
    }
    //showAPSet("LME_Set", LME_Set);
    
    /* Finding \exists VarsToElim.\beta = \beta' This is always possible since all coefficients in \alpha are odd */
    DAGNode* QEConjunctionOfLMEs = APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(LME_Set,  VarsToElim,  WidthTable);
    //fprintf(LMIQEOutputDataFp, "\nResult of QE from conjunction of LMEs = %s\n", (getDAGStr(QEConjunctionOfLMEs)).c_str());
    
    DAGNode* QEDag = QEConjunctionOfLMEs;
    QuantifiedPart = InlinedConjunctionOfLMIs;
    
    return QEDag;
    }//else
}//end of function


// Write vector< pair<DAGNode*, DAGNode*> > LMIPairs in file
void writeLMIPairs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs)
{
    fprintf(LMIQEOutputDataFp, "\nLMIPairs\n**********\n");
    for(int i=0; i<LMIPairs.size(); i++)
    {
       if(LMIPairs[i].first==NULL) 
       {
          fprintf(LMIQEOutputDataFp, "NULL\t%s\n", getDAGStr(LMIPairs[i].second).c_str()); 
       }
       else if(LMIPairs[i].second==NULL)
       {
          fprintf(LMIQEOutputDataFp, "%s\tNULL\n", getDAGStr(LMIPairs[i].first).c_str()); 
       }
       else
       {
           fprintf(LMIQEOutputDataFp, "%s\t%s\n", getDAGStr(LMIPairs[i].first).c_str(), getDAGStr(LMIPairs[i].second).c_str());
       }     
        
    }//for
}// end of function


// Select the variables for normalizaton
void selectVariablesForNormalization(set<DAGNode*> &Bound_Constraints, set<string> &VarsToElim, set<string> &Normalized, map<string, int> &WidthTable)
{
    map<string, set<string> > Neighbours;
    
    for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
        string variable = *it;
        //cout<<"\nvariable = "<<variable<<endl;
        
        set<DAGNode*> BoundConstraintsForVariable;
        set<DAGNode*> FreeConstraintsForVariable;
        getFreeAndBoundConstraints(Bound_Constraints, variable, FreeConstraintsForVariable, BoundConstraintsForVariable, WidthTable);
          
        set<string> Support_BoundConstraintsForVariable;
        for(set<DAGNode*>::iterator cit=BoundConstraintsForVariable.begin(); cit != BoundConstraintsForVariable.end(); cit++)
        {
            getDAGNames(*cit, Support_BoundConstraintsForVariable);
        }
        
       set<string> MySelf;
       MySelf.insert(variable);
       
       set<string> MyNeighbours;
       set_difference(Support_BoundConstraintsForVariable.begin(), Support_BoundConstraintsForVariable.end(), MySelf.begin(), MySelf.end(),
		     inserter(MyNeighbours, MyNeighbours.begin()));
       
       //showSet(MyNeighbours, "MyNeighbours");       
       Neighbours.insert(make_pair(variable, MyNeighbours));        
    }//for
    
    set<string> Non_Normalized;
    for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
        string variable = *it;
        //cout<<"\nvariable = "<<variable<<endl;
        
        if(Non_Normalized.find(variable) == Non_Normalized.end())
        {
            Normalized.insert(variable);
            //showSet(Normalized, "Normalized");
            map<string, set<string> >::iterator nit = Neighbours.find(variable);
            if(nit == Neighbours.end())
            {
                cout<<"\nNo neighbours for "<<variable<<" in function selectVariablesForNormalization in DAGEliminator.cpp\n";
                exit(1);
            }
           set<string> MyNeighbours = nit->second;
           set_union(Non_Normalized.begin(), Non_Normalized.end(), MyNeighbours.begin(), MyNeighbours.end(),
		     inserter(Non_Normalized, Non_Normalized.begin()));
           //showSet(Non_Normalized, "Non_Normalized");
        }//if(Non-Normalized.find(variable) == Non-Normalized.end())        
    }//for  
    
    //showSet(Normalized, "Normalized");
    //showSet(Non_Normalized, "Non_Normalized");
    
}//function
       



// Version of convertToNormalForm for multiple variables
DAGNode* convertToNormalFormForMultipleVariables(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
   if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function convertToNormalFormForMultipleVariables in DAGEliminator.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}  
    
    
    vector<DAGNode*> Constraints_Vector;
    vector< vector<DAGNode*> > SplittedConstraints;
    
    // Let us ensure that we have only <= constraints
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* Constraint = *it;
        if(Constraint->Get_node_name() != "bvule")
        {
            cout<<"\nUnsupported constraint "<<getDAGStr(Constraint)<<" in function convertToNormalFormForMultipleVariables in DAGEliminator.cpp\n";
            exit(1);
        }
    }//for
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        Constraints_Vector.push_back(*it);        
    }
    
    map<DAGNode*, string> ConstraintVarToElimMap;
    
    for(int constraint_index=0; constraint_index < Constraints_Vector.size(); constraint_index++)
    {
        DAGNode *Constraint = Constraints_Vector[constraint_index];
        cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
        set<string> Support;
        
        getDAGNames(Constraint, Support);
        set<string> MyVariables;
        set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),
		     inserter(MyVariables, MyVariables.begin()));
        
         if(MyVariables.size() == 0)
            {
            cout<<"\nNo variables to normalize\n";
            continue;
            }
        
        if(MyVariables.size() > 1)
            {
            cout<<"\nVariables to normalize is multiple in function convertToNormalFormForMultipleVariables in DAGEliminator.cpp\n";
            exit(1);
            }
        string MyVariable;
        set<string>::iterator it = MyVariables.begin();
        MyVariable = *it;
        cout<<"\nMyVariable = "<<MyVariable<<endl;        
        ConstraintVarToElimMap.insert(make_pair(Constraint, MyVariable));
    }
    
    map<string, bool> ConstraintEncountered;
    for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
        string MyVariable = *it;
        ConstraintEncountered.insert(make_pair(MyVariable, false));
    }   
    
    map<string, ULINT> CoefsOfVariable;
    map<string, ULINT> AICoefsOfVariable;
    
    
    for(int constraint_index=0; constraint_index < Constraints_Vector.size(); constraint_index++)
    {
        DAGNode *Constraint = Constraints_Vector[constraint_index];
        cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
        
        map<DAGNode*, string>::iterator it=ConstraintVarToElimMap.find(Constraint);
        if(it == ConstraintVarToElimMap.end())
        {
            cout<<"\nConstraint free of vars to elim\n";
            vector<DAGNode*> SplittedVector;
            SplittedVector.push_back(Constraint);
            SplittedConstraints.push_back(SplittedVector);  
            continue;
        }
        
        string VarToElim = it->second;
        cout<<"\nVarToElim = "<<VarToElim<<endl;
        
        
        int WidthOfVarToElim;
        ULINT ModOfVarToElim;
        
    
        map<string, int>::iterator wit = WidthTable.find(VarToElim);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for the variable "<<VarToElim<<" in WidthTable in function convertToNormalFormForMultipleVariables in DAGEliminator.cpp\n";
            exit(1);
            }
        WidthOfVarToElim = wit->second;
        ModOfVarToElim = findPower(WidthOfVarToElim);
    
        cout<<"\nWidthOfVarToElim = "<<WidthOfVarToElim<<endl;
        cout<<"\nModOfVarToElim = "<<ModOfVarToElim<<endl;
        
        string side_of_variable = findSideOfVariableInConstraint(Constraint, VarToElim);
        cout<<"\nside_of_variable = "<<side_of_variable<<endl;
        
        bool both_sides_implemented = true;
        if(!both_sides_implemented)
        {
               if(side_of_variable == "both")
                {
                 cout<<"\nThe variable "<<VarToElim<<" appears on both sides of "<<getDAGStr(Constraint)<<" in function convertToNormalFormForMultipleVariables in DAGEliminator.cpp"<<endl;
                exit(1);
                }//if(side_of_variable == "both")
        }
        
        if(side_of_variable == "none")
        {
            //cout<<"\nThe variable "<<VarToElim<<" does not appear on any side of "<<getDAGStr(Constraint)<<" in function convertToNormalForm in DAGEliminator.cpp"<<endl;
            
            vector<DAGNode*> SplittedVector;
            SplittedVector.push_back(Constraint);
            SplittedConstraints.push_back(SplittedVector);  
            continue;
        }//if(side_of_variable == "both")
        
        string type_of_coefficient = "original";
        
        map<string, bool>::iterator cit = ConstraintEncountered.find(VarToElim);
        if(cit == ConstraintEncountered.end())
            {
            cout<<"\nNo entry for the variable "<<VarToElim<<" in ConstraintEncountered in function convertToNormalFormForMultipleVariables in DAGEliminator.cpp\n";
            exit(1);
            }
        bool first_constraint_encountered = cit->second;
        
        if(!first_constraint_encountered)//first constraint
        {
            cout<<"\n"<<VarToElim<<" is encountered the first time\n";
            ULINT CoefOfVarToElim;
            ULINT AIOfCoefOfVarToElim;
            getCoefOfVarToElimAndAIOfCoefOfVarToElim(Constraint, VarToElim, side_of_variable, ModOfVarToElim, CoefOfVarToElim, AIOfCoefOfVarToElim);
            cout<<"\nCoefOfVarToElim = "<<CoefOfVarToElim<<", AIOfCoefOfVarToElim = "<<AIOfCoefOfVarToElim<<endl;
            CoefsOfVariable.insert(make_pair(VarToElim, CoefOfVarToElim));
            AICoefsOfVariable.insert(make_pair(VarToElim, AIOfCoefOfVarToElim));
            ConstraintEncountered[VarToElim] = true;
        }//if(constr_index==0)
        else
        {
            cout<<"\n"<<VarToElim<<" is not encountered the first time\n";
            ULINT CoefOfVarToElim;
            ULINT AIOfCoefOfVarToElim;
            
            map<string, ULINT>::iterator coef_it = CoefsOfVariable.find(VarToElim);
            if(coef_it == CoefsOfVariable.end())
                {
                cout<<"\nNo entry for "<<VarToElim<<" in CoefsOfVariable in function convertToNormalFormForMultipleVariable in DAGEliminator.cpp\n";
                exit(1);
                }
            CoefOfVarToElim = coef_it->second;
            
            map<string, ULINT>::iterator aicoef_it = AICoefsOfVariable.find(VarToElim);
            if(aicoef_it == AICoefsOfVariable.end())
                {
                cout<<"\nNo entry for "<<VarToElim<<" in AICoefsOfVariable in function convertToNormalFormForMultipleVariable in DAGEliminator.cpp\n";
                exit(1);
                }
            AIOfCoefOfVarToElim = aicoef_it->second;
            
            
            type_of_coefficient = findTypeOfCoefficient(Constraint, VarToElim, side_of_variable, CoefOfVarToElim, AIOfCoefOfVarToElim);
            cout<<"\ntype_of_coefficient = "<<type_of_coefficient<<endl;
        }// else of if(constr_index==0)
        
             
        vector<DAGNode*> SplittedVector;
        
        
        if(side_of_variable == "left" || side_of_variable == "right")
        {
           obtainSplittedVectorMadeEfficient(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector);
         }
        else if(side_of_variable == "both")
        {
            obtainSplittedVectorForLMIsWithBothSidesMadeEfficient(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector);
         }
        else
        {
          cout<<"\nUnexpected side_of_variable "<<side_of_variable<<" in function convertToNormalFormForMultipleVariable in DAGEliminator.cpp\n";
          exit(1);  
        }
        
      showAPVector("SplittedVector", SplittedVector);
      
      if(SplittedVector.size()>0)
        {
        SplittedConstraints.push_back(SplittedVector);        
        }
    }//for ends
 
 vector<DAGNode*> Final_Vector;
 
 for(int i=0; i<SplittedConstraints.size(); i++)
    {
      vector<DAGNode*> TempVector = SplittedConstraints[i];
      DAGNode* TempDAG = getDisjunctionFromVector(TempVector);
      Final_Vector.push_back(TempDAG);
    }
 
 DAGNode* resultDAG = getConjunctionFromVector(Final_Vector);
 return resultDAG;
}   
    



// Version of lmiQEUsingLMEBasedReasoningAndBlasting which works on dags 
// with LMIs, LMDs, and LMEs
DAGNode* lmiQEUsingLMEBasedReasoningAndBlastingGeneral(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  bool CallLMEBasedSimplification = true;
  
   showAPVector("Constraints", Constraints);
   //convertStrictLMIsToWeakLMIs(Constraints, WidthTable);
   //showAPVector("Constraints", Constraints);   
  
  //showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  showSet(VarsToElim, "VarsToElim"); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  showAPSet("Free_Constraints", Free_Constraints);
  showAPSet("Bound_Constraints", Bound_Constraints);
  
  //showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  //showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);
  
  initializeProjectBlastDetails();
  getRelevantProjectBlastOriginalDetails(Bound_Constraints, VarsToElim, WidthTable);  
  
   DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
           Bound_Constraints_Vector.push_back(*it); 
        }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
    
  if(CallLMEBasedSimplification)
  {
      DAGNode* Quantified_Bound_Part;
      DAGNode* Simplified_Bound_Part;
      
      // Let us split the bound constraints into LMEs, LMIs, and LMDs
  
        set<DAGNode*> LMESet;
        set<DAGNode*> LMISet;
        set<DAGNode*> LMDSet;  
  
        for(set<DAGNode*>::iterator it=Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
            DAGNode* Constraint = *it;
      
            if(Constraint->Get_node_name()=="=")
                {
                LMESet.insert(Constraint);
                }
            else if(Constraint->Get_node_name()=="is_not_equal")
                {
                LMDSet.insert(Constraint);
                }
            else if(Constraint->Get_node_name()=="bvule" || Constraint->Get_node_name()=="bvuge"  || Constraint->Get_node_name()=="bvult" || Constraint->Get_node_name()=="bvugt")
                {
                LMISet.insert(Constraint);
                }
            else
                {
                cout<<"\nUnknown constraint type "<<Constraint->Get_node_name()<<" in function lmiQEUsingLMEBasedReasoningAndBlastingGeneral in DAGEliminator.cpp\n";
                exit(1);
                }
        }//for
      
  
     if(LMISet.empty())// No LMIs. Use CAV'11 work
        {      
        cout<<"\nNo LMIs appear in the conjunction\n";
      
        set<DAGNode*> LMELMDSet;
        set_union(LMESet.begin(),  LMESet.end(), LMDSet.begin(), LMDSet.end(), inserter(LMELMDSet, LMELMDSet.begin())); 
        vector<DAGNode*> LMELMDVector;
      
        for(set<DAGNode*>::iterator it=LMELMDSet.begin(); it != LMELMDSet.end(); it++)
            {
            LMELMDVector.push_back(*it);
            }
      
        showAPVector("LMELMDVector", LMELMDVector);
        DAGNode* QFreePart = callAPResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(LMELMDVector, VarsToElim, WidthTable);
        cout<<"\nQFreePart = "<<getDAGStr(QFreePart)<<endl;
        DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
        cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
        DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
        return resultDAG;
        }//No LMIs
     else
        {
       DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
         
      cout<<"\nLMIs appear in the conjunction\n";
      showAPSet("LMESet", LMESet);
      showAPSet("LMDSet", LMDSet);
      showAPSet("LMISet", LMISet);
            
      DAGNode* ResultSimplification;
      
      ResultSimplification = LMEBasedSimplification_General(LMESet, LMDSet, LMISet, VarsToElim, WidthTable);
      cout<<"\nResultSimplification = "<<getDAGStr(ResultSimplification)<<endl;
     
      DAGNode *QFreePart=createLogicalConstant("true");
      DAGNode *BoundPart=createLogicalConstant("true");
      
      if(!isTrue(ResultSimplification))
        {
        divideConjunctionToFreeAndBoundParts(ResultSimplification, VarsToElim, WidthTable, QFreePart, BoundPart);
        }      
      if(isTrue(BoundPart))
        {
        DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
        cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
        DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
        return resultDAG;
        }
      else
        {
          DAGNode* PartialResultDAG;
          PartialResultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
          cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
          cout<<"\nWe still need to eliminate quantifiers from "<<getDAGStr(BoundPart)<<endl;
          
         set<string> BoundPart_Support;
         set<string> CommonVars;
    
        getDAGNames(BoundPart, BoundPart_Support);
        set_intersection(VarsToElim.begin(), VarsToElim.end(), BoundPart_Support.begin(), BoundPart_Support.end(),inserter(CommonVars, CommonVars.begin()));

          
         vector<DAGNode*> BoundPart_Constraints;
         splitConjunctionIntoLMIVector(BoundPart, BoundPart_Constraints);
         
         //cout<<"\nPress any key to start bit-blasting based QE\n";
         //char c = getchar();
          
         getRelevantProjectBlastToBlastDetails(BoundPart_Constraints, CommonVars, WidthTable);
         
         DAGNode* ResultFromCUDD = QE_Using_Blasting_WithSupportForLMIs(BoundPart_Constraints, CommonVars, WidthTable);

         DAGNode* resultDAG;
         resultDAG = createDAG("and", PartialResultDAG, ResultFromCUDD, LOGICAL_SYMBOL, 1);
         //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
         return resultDAG;
      }//else of if(isTrue(BoundPart))
     }//else of if(LMISet.empty())
  }// if(CallLMEBasedSimplification)
}//end of function




// Version of LMEBasedSimplification which is more general
DAGNode* LMEBasedSimplification_General(set<DAGNode*> &E, set<DAGNode*> &D,  set<DAGNode*> &I, set<string> VarsToElim, map<string, int> &WidthTable)
{

  //cout<<"\nAPResolveModifiedWithMultipleVariablesUsingLMA called\n";

  set<DAGNode*> Constraints;
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  DAGNode* QElim_Bound_Constraint;

  showSet(VarsToElim,"VarsToElim");

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));
  set_union(Constraints.begin(), Constraints.end(), I.begin(), I.end(),inserter(Constraints, Constraints.begin()));

  showAPSet("Constraints", Constraints);

  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;

      cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      showAPSet("Free_Constraints", Free_Constraints);
      showAPSet("Bound_Constraints", Bound_Constraints);

      QElim_Bound_Constraints.clear();
      QElim_Bound_Constraint = NULL; // stands for true

      if(Bound_Constraints.size()>0)
	{
          set<DAGNode*> LMESet;
          set<DAGNode*> LMDSet;
          set<DAGNode*> LMISet;
          
          splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
          
          if(LMISet.empty()) // no LMIs. Use CAV'11 work to eliminate this variable
              // Elimination guaranteed
            {
              cout<<"\nLMIs do not appear in the conjunction\n";
                
              QElim_Bound_Constraint = callMatrixQEliminator(Bound_Constraints, VariableToElim, WidthTable);
              
             }
          else
            {
              cout<<"\nLMIs appear in the conjunction\n";
              showAPSet("LMESet", LMESet);
              showAPSet("LMDSet", LMDSet);
              showAPSet("LMISet", LMISet);
              
              if(LMESet.empty())
                {
                  cout<<"\nLMESet is empty. Only option is to blast\n";                
                  QElim_Bound_Constraint = getConjunctionFromSet(Bound_Constraints);
                  
                }//if(LMESet.empty())
              else// LMEBased simplification for LMDs and LMIs
                {                 
                  // Using layer1 - elimination is not guaranteed                                    
                  QElim_Bound_Constraint = callMatrixQEliminatorForLMCsAndLMIs(LMESet, LMDSet, LMISet, VariableToElim, WidthTable);
                               
                }//else if(LMESet.empty())
            }//else if(LMISet.empty()) 	  
	}//  if(Bound_Constraints.size()>0)

      if(QElim_Bound_Constraint != NULL) // stands for true
	{
          
          if(!isTrue(QElim_Bound_Constraint))
                {
                cout<<"\nQElim_Bound_Constraint = "<<getDAGStr(QElim_Bound_Constraint)<<"\n";
                splitConjunctionIntoSetGeneral(QElim_Bound_Constraint, QElim_Bound_Constraints);
                }
	}

      showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      showAPSet("Constraints", Constraints);
    }//for

  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
  
    if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    resultDAG = quickSimplify(resultDAG);

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    return resultDAG;
}


// Extract relevant details of Layer3
void extractLayer3Data(bool z3_finished, set<DAGNode*> &Constraints, set<string> &VarsToElim, set< set<DAGNode*> > &ScopeReducedDAGs, int &LMCCountInInputConjunction, int &VarsCountInInputConjunction, int &VarsToElimCountInInputConjunction, int &PartitionsInOutputConjunction, int &MaxPartitionSizeInOutputConjunction, int &MinPartitionSizeInOutputConjunction, float &AvgPartitionSizeInOutputConjunction)
{
    
    LMCCountInInputConjunction = Constraints.size();

    set<string> Support;
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
        {
            DAGNode *Constraint = *it;        
            getDAGNames(Constraint, Support);
        }// for

    set<string> CommonVars;
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
        
    
    VarsToElimCountInInputConjunction = CommonVars.size();
    VarsCountInInputConjunction = Support.size();
    
    if(z3_finished)
    {
      if(ScopeReducedDAGs.empty())  
      {
         PartitionsInOutputConjunction = 0;
         MaxPartitionSizeInOutputConjunction=0;
         MinPartitionSizeInOutputConjunction=0;
         AvgPartitionSizeInOutputConjunction=0;
      }
      else
      {
      PartitionsInOutputConjunction = ScopeReducedDAGs.size();
      
      set< set<DAGNode*> >::iterator it = ScopeReducedDAGs.begin();
      MaxPartitionSizeInOutputConjunction= (*it).size();
      MinPartitionSizeInOutputConjunction=  (*it).size();
      AvgPartitionSizeInOutputConjunction=  (*it).size();
      
      for(; it !=  ScopeReducedDAGs.end(); it++)
        {
          if((*it).size() > MaxPartitionSizeInOutputConjunction)
          {
            MaxPartitionSizeInOutputConjunction = (*it).size();            
          }
          if((*it).size() < MinPartitionSizeInOutputConjunction)
          {
            MinPartitionSizeInOutputConjunction = (*it).size();            
          }
           AvgPartitionSizeInOutputConjunction += (*it).size();
        }//for
      AvgPartitionSizeInOutputConjunction /= PartitionsInOutputConjunction;
      
      /* There's some bug here. Avg. is not coming out properly */      
      }//else    
    }// if(z3_finished)
}//function end




// Initialize the Project_Blast detail
void initializeProjectBlastDetails()
{
LMIProjectBlastOriginalNoVarsToElim=0;
LMIProjectBlastOriginalNoLMCs=0;
LMIProjectBlastOriginalNoBitsToElim=0;
LMIProjectBlastToBlastNoVarsToElim=0;
LMIProjectBlastToBlastNoLMCs=0;
LMIProjectBlastToBlastNoBitsToElim=0;
}



// Get the relevant details of Project_Blast calls before LME based reasoning
void getRelevantProjectBlastOriginalDetails(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
    LMIProjectBlastOriginalNoLMCs=Constraints.size(); 
    
    set<string> Support;
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode *Constraint = *it;        
        getDAGNames(Constraint, Support);
    }// for
    
    set<string> CommonVars;
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
     LMIProjectBlastOriginalNoVarsToElim = CommonVars.size();
   

    LMIProjectBlastOriginalNoBitsToElim = 0;
    
    for(set<string>::iterator it=CommonVars.begin(); it != CommonVars.end(); it++)
        {
        string signal = *it;
        map<string, int>::iterator wit =WidthTable.find(signal);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for "<<signal<<" in the WidthTable in function getRelevantProjectBlastOriginalDetails in DAGEliminator.cpp\n";
            exit(1);
            }
        LMIProjectBlastOriginalNoBitsToElim += wit->second;        
        }
    
    
}//function




// Get the relevant details of Project_Blast calls after LME based reasoning
void getRelevantProjectBlastToBlastDetails(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
    LMIProjectBlastToBlastNoLMCs=Constraints.size(); 
    
    set<string> Support;
    for(int i=0; i<Constraints.size(); i++)
    {
        DAGNode *Constraint = Constraints[i];    
        getDAGNames(Constraint, Support);
    }// for
    
    set<string> CommonVars;
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMIProjectBlastToBlastNoVarsToElim= CommonVars.size();
   

    LMIProjectBlastToBlastNoBitsToElim= 0;
    
    for(set<string>::iterator it=CommonVars.begin(); it != CommonVars.end(); it++)
        {
        string signal = *it;
        map<string, int>::iterator wit =WidthTable.find(signal);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for "<<signal<<" in the WidthTable in function getRelevantProjectBlastOriginalDetails in DAGEliminator.cpp\n";
            exit(1);
            }
         LMIProjectBlastToBlastNoBitsToElim += wit->second;        
        }
    
    
}//function



// Get the relevant details of Project calls after LME based reasoning
void getRelevantProjectPostLayer1Details(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
    LMIPostLayer1NoLMCs = Constraints.size(); 
    
    set<string> Support;
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode *Constraint = *it;
        getDAGNames(Constraint, Support);
    }// for
    
    set<string> CommonVars;
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
   LMIPostLayer1NoVarsToElim = CommonVars.size();
   

   LMIPostLayer1NoBitsToElim = 0;
    
    for(set<string>::iterator it=CommonVars.begin(); it != CommonVars.end(); it++)
        {
        string signal = *it;
        map<string, int>::iterator wit =WidthTable.find(signal);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for "<<signal<<" in the WidthTable in function getRelevantProjectPostLayer1Details in DAGEliminator.cpp\n";
            exit(1);
            }
         LMIPostLayer1NoBitsToElim += wit->second;        
        }
    
    
}//function



// Initialize the Project calls details post Layer1
void initializeProjectPostLayer1Details()
{
LMIPostLayer1NoVarsToElim=0;
LMIPostLayer1NoLMCs=0;
LMIPostLayer1NoBitsToElim=0;
}





// QE from conjunction of LMIs using LME based simplification followed by Omega Test
// Note that there' no return DAG. Just to comapre the performance
void lmiQEUsingLMEBasedReasoningAndOTest(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  bool CallLMEBasedSimplification = true;
  
   showAPVector("Constraints", Constraints);
   convertStrictLMIsToWeakLMIs(Constraints, WidthTable);
   showAPVector("Constraints", Constraints);   
  
  showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
  
  showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);
  
  
   DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
           Bound_Constraints_Vector.push_back(*it); 
        }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
    
  if(CallLMEBasedSimplification)
  {
      DAGNode* Quantified_Bound_Part;
      DAGNode* Simplified_Bound_Part = LMEBasedSimplification_ForWeakLMIs(Bound_Constraints, VarsToElim, WidthTable, Quantified_Bound_Part);
      DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
      
      if(isTrue(Quantified_Bound_Part))
      {
          DAGNode* resultDAG;
          resultDAG = createDAG("and", Simplified_Bound_Part, Free_Part, LOGICAL_SYMBOL, 1);
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          //return resultDAG;
      }
      else
      {
          DAGNode* PartialResultDAG;
          PartialResultDAG = createDAG("and", Simplified_Bound_Part, Free_Part, LOGICAL_SYMBOL, 1);
          //cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
          //cout<<"\nWe still need to eliminate quantifiers from "<<getDAGStr(Quantified_Bound_Part)<<endl;
          
         set<string> Quantified_Bound_Part_Support;
         set<string> CommonVars;
    
        getDAGNames(Quantified_Bound_Part, Quantified_Bound_Part_Support);
        set_intersection(VarsToElim.begin(), VarsToElim.end(), Quantified_Bound_Part_Support.begin(), Quantified_Bound_Part_Support.end(),inserter(CommonVars, CommonVars.begin()));

          
         vector<DAGNode*> Quantified_Bound_Part_Constraints;
         splitConjunctionIntoLMIVector(Quantified_Bound_Part, Quantified_Bound_Part_Constraints);
         
         cout<<"\nPress any key to start Omega Test based QE\n";
         char c = getchar();
         
         // Get the list of variables,  NoVars, NoVarsToElim
            list<string> VarNames;
            attachSetToList(CommonVars, VarNames);
            int OTNoVarsToElim = CommonVars.size();
            attachSetToList(Quantified_Bound_Part_Support, VarNames);     
            
            int OTBits = 0;//Max. width of any variable in the Quantified_Bound_Part
            
            for(set<string>::iterator it=Quantified_Bound_Part_Support.begin();  it!=Quantified_Bound_Part_Support.end(); it++)
                {
                string MyVariable = *it;
                map<string, int>::iterator wit = WidthTable.find(MyVariable);
                if(wit == WidthTable.end())
                    {
                    cout<<"\nVariable "<<MyVariable<<" has no entry in WidthTable in function lmiQEUsingLMEBasedReasoningAndOTest in DAGEliminator.cpp\n";
                    exit(1);
                    }
                
                if(OTBits < wit->second)
                    {
                    OTBits = wit->second;
                    }        
                }//for
            
            ULINT OTMod = findPower(OTBits);            
            
            cout<<"\nBefore calling Omega Test\n";
            cout<<"\nQuantified_Bound_Part = "<<getDAGStr(Quantified_Bound_Part)<<endl;
            cout<<"\nOTNoVarsToElim = "<<OTNoVarsToElim<<endl;
            cout<<"\nOTMod = "<<OTMod<<endl;
            showList(VarNames, "VarNames");
                        
            DAGNode* TempDAG = qEliminator_ILP_ReturningDAG_WithLMISupport(Quantified_Bound_Part, OTNoVarsToElim, OTMod, VarNames, WidthTable);
      }//else
  }//if(CallLMEBasedSimplification)
}// end of function




// Version of lmiQEUsingLMEBasedReasoningAndOTest which works on dags 
// with LMIs, LMDs, and LMEs
void lmiQEUsingLMEBasedReasoningAndOTestGeneral(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
  bool CallLMEBasedSimplification = true;
  
   showAPVector("Constraints", Constraints);
   //convertStrictLMIsToWeakLMIs(Constraints, WidthTable);
   //showAPVector("Constraints", Constraints);   
  
  //showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  showSet(VarsToElim, "VarsToElim"); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  showAPSet("Free_Constraints", Free_Constraints);
  showAPSet("Bound_Constraints", Bound_Constraints);
  
  //showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  //showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);
  
  //initializeProjectBlastDetails();
  //getRelevantProjectBlastOriginalDetails(Bound_Constraints, VarsToElim, WidthTable);  
  
   DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
           Bound_Constraints_Vector.push_back(*it); 
        }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
    
  if(CallLMEBasedSimplification)
  {
      DAGNode* Quantified_Bound_Part;
      DAGNode* Simplified_Bound_Part;
      
      // Let us split the bound constraints into LMEs, LMIs, and LMDs
  
        set<DAGNode*> LMESet;
        set<DAGNode*> LMISet;
        set<DAGNode*> LMDSet;  
  
        for(set<DAGNode*>::iterator it=Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
        {
            DAGNode* Constraint = *it;
      
            if(Constraint->Get_node_name()=="=")
                {
                LMESet.insert(Constraint);
                }
            else if(Constraint->Get_node_name()=="is_not_equal")
                {
                LMDSet.insert(Constraint);
                }
            else if(Constraint->Get_node_name()=="bvule" || Constraint->Get_node_name()=="bvuge"  || Constraint->Get_node_name()=="bvult" || Constraint->Get_node_name()=="bvugt")
                {
                LMISet.insert(Constraint);
                }
            else
                {
                cout<<"\nUnknown constraint type "<<Constraint->Get_node_name()<<" in function lmiQEUsingLMEBasedReasoningAndOTestGeneral in DAGEliminator.cpp\n";
                exit(1);
                }
        }//for
      
  
     if(LMISet.empty())// No LMIs. Use CAV'11 work
        {      
        cout<<"\nNo LMIs appear in the conjunction\n";
      
        set<DAGNode*> LMELMDSet;
        set_union(LMESet.begin(),  LMESet.end(), LMDSet.begin(), LMDSet.end(), inserter(LMELMDSet, LMELMDSet.begin())); 
        vector<DAGNode*> LMELMDVector;
      
        for(set<DAGNode*>::iterator it=LMELMDSet.begin(); it != LMELMDSet.end(); it++)
            {
            LMELMDVector.push_back(*it);
            }
      
        showAPVector("LMELMDVector", LMELMDVector);
        DAGNode* QFreePart = callAPResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(LMELMDVector, VarsToElim, WidthTable);
        cout<<"\nQFreePart = "<<getDAGStr(QFreePart)<<endl;
        DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
        cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
        DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);       
        }//No LMIs
     else
        {
       DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
         
      cout<<"\nLMIs appear in the conjunction\n";
      showAPSet("LMESet", LMESet);
      showAPSet("LMDSet", LMDSet);
      showAPSet("LMISet", LMISet);
            
      DAGNode* ResultSimplification;
      
      ResultSimplification = LMEBasedSimplification_General(LMESet, LMDSet, LMISet, VarsToElim, WidthTable);
      cout<<"\nResultSimplification = "<<getDAGStr(ResultSimplification)<<endl;
     
      DAGNode *QFreePart=createLogicalConstant("true");
      DAGNode *BoundPart=createLogicalConstant("true");
      
      if(!isTrue(ResultSimplification))
        {
        divideConjunctionToFreeAndBoundParts(ResultSimplification, VarsToElim, WidthTable, QFreePart, BoundPart);
        }      
      if(isTrue(BoundPart))
        {
        DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
        cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
        DAGNode* resultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);        
        }
      else
        {
          DAGNode* PartialResultDAG;
          PartialResultDAG = createDAG("and", QFreePart, FreePart, LOGICAL_SYMBOL, 1);
          cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
          cout<<"\nWe still need to eliminate quantifiers from "<<getDAGStr(BoundPart)<<endl;
          
         set<string> BoundPart_Support;
         set<string> CommonVars;
    
        getDAGNames(BoundPart, BoundPart_Support);
        set_intersection(VarsToElim.begin(), VarsToElim.end(), BoundPart_Support.begin(), BoundPart_Support.end(),inserter(CommonVars, CommonVars.begin()));

          
         vector<DAGNode*> BoundPart_Constraints;
         splitConjunctionIntoLMIVector(BoundPart, BoundPart_Constraints);
         
         //cout<<"\nPress any key to start bit-blasting based QE\n";
         //char c = getchar();
          
         //getRelevantProjectBlastToBlastDetails(BoundPart_Constraints, CommonVars, WidthTable);
         
         // Get the list of variables,  NoVars, NoVarsToElim
            list<string> VarNames;
            attachSetToList(CommonVars, VarNames);
            int OTNoVarsToElim = CommonVars.size();
            attachSetToList(BoundPart_Support, VarNames);     
            
            int OTBits = 0;//Max. width of any variable in the BoundPart
            
            for(set<string>::iterator it=BoundPart_Support.begin();  it!=BoundPart_Support.end(); it++)
                {
                string MyVariable = *it;
                map<string, int>::iterator wit = WidthTable.find(MyVariable);
                if(wit == WidthTable.end())
                    {
                    cout<<"\nVariable "<<MyVariable<<" has no entry in WidthTable in function lmiQEUsingLMEBasedReasoningAndOTestGeneral in DAGEliminator.cpp\n";
                    exit(1);
                    }
                
                if(OTBits < wit->second)
                    {
                    OTBits = wit->second;
                    }        
                }//for
            
            ULINT OTMod = findPower(OTBits);            
            
            cout<<"\nBefore calling Omega Test\n";
            cout<<"\nQuantified_Bound_Part = "<<getDAGStr(BoundPart)<<endl;
            cout<<"\nOTNoVarsToElim = "<<OTNoVarsToElim<<endl;
            cout<<"\nOTMod = "<<OTMod<<endl;
            showList(VarNames, "VarNames");
                        
           DAGNode* TempDAG = qEliminator_ILP_ReturningDAG_WithLMISupport(BoundPart, OTNoVarsToElim, OTMod, VarNames, WidthTable);
                  
      }//else of if(isTrue(BoundPart))
     }//else of if(LMISet.empty())
  }// if(CallLMEBasedSimplification)
}//end of function




/* Final set of functions for QE from Boolean combinations of LMEs, LMDs, and LMIs */
/* Added on 26 Jan 2012 onwards */

// Calls the function Monniaux_Quantifier_Eliminator_WithSupportForLMIs, 
// and records the time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForLMIs(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable)  
{
  time_t var_elim_start_time, var_elim_end_time;
  time(&var_elim_start_time);

  time_out_start = var_elim_start_time;
  
  createVariableMap(WidthTable);
  //showVariableMap();
  // VariableMap associates a unique integer with each varaible
  // This helps in creating the hash key for \exists X. (F)
  
  int project_depth = 0;
  
  DAGNode* result = callMonniaux_Quantifier_Eliminator_WithSupportForLMIs(F, VarSetToElim, WidthTable, project_depth);

  time(&var_elim_end_time);

  if(!timed_out)
	{
  	//cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
  	//cout<<"\nSize of output dag = "<<getSize(result)<<endl;
	}
  else
	{
	//cout<<"\nQuantifier elimination timed out in "<<(var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;
	}

  QElim_Time = var_elim_end_time - var_elim_start_time;

  return result;
  
}



// Calls  Monniaux_Quantifier_Eliminator_WithSupportForLMIs with dynamic programming
DAGNode* callMonniaux_Quantifier_Eliminator_WithSupportForLMIs(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth)
{
  // Check in HashTable  
 DAGNode* Existing  = getEntryFromMonniaux_HT(F, VarSetToElim);

  if(Existing != NULL)
    {
      Monniaux_HT_hits++;
      //cout<<"\nHit in Monniaux_HT in function callMonniaux_Quantifier_Eliminator_WithSupportForLMIs\n";
      return Existing;
    }
 
 Monniaux_HT_misses++;
 //cout<<"\nMiss in Monniaux_HT in function callMonniaux_Quantifier_Eliminator_WithSupportForLMIs\n";
 //Call Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting
 DAGNode* Result = Monniaux_Quantifier_Eliminator_WithSupportForLMIs(F, VarSetToElim, WidthTable, project_depth);
 
 //Insert into HashTable
 insertEntryIntoMonniaux_HT(F, VarSetToElim, Result);
 
 return Result;    
}


// Function to perform \exists X.\Phi where Phi is a conjunction of LMEs, LMDs, and LMIs
DAGNode* Monniaux_Quantifier_Eliminator_WithSupportForLMIs(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth)
{   
  time_t start;
  time_t end;
  time_t duration;
  
  struct timeval fun_start_ms, fun_finish_ms;
  unsigned long long int fun_duration_ms;
  
  struct timeval cubefind_start_ms, cubefind_finish_ms;
  
  // new code added on 19 july 2015 starts here
   struct timeval generalize2_start_ms, generalize2_finish_ms;
   unsigned long long int generalize2_duration_ms; 
   double contribution_generalize2;
   int number_of_aps_before_generalize2, number_of_aps_after_generalize2;
   // new code added on 19 july 2015 ends here
   
  time(&start); 

  vector<DAGNode*> Constraints;

  set<DAGNode*> APSet;
  getAtomicPredicates_WithSupportForLMIs(F, APSet);

  //showAPSet("APSet", APSet);
  
  map<string, string> Model;
  DAGNode *H = F;
  DAGNode *O = createLogicalConstant("false");
  int iteration =  1;
  
  struct timeval smt_start_ms, smt_finish_ms;
  unsigned long long int SMTTime;
  gettimeofday (&smt_start_ms, NULL); 
  
  gettimeofday (&cubefind_start_ms, NULL); 

  while(!getModelGeneral_WithSupportForLMIs(H, Model, WidthTable))
    {
      
      gettimeofday (&smt_finish_ms, NULL);
      SMTTime = smt_finish_ms.tv_sec * 1000 + smt_finish_ms.tv_usec / 1000;
      SMTTime -= smt_start_ms.tv_sec * 1000 + smt_start_ms.tv_usec / 1000;     
      //cout<<"\nTime for SMT call = "<< SMTTime<<" milli seconds\n";
      
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function Monniaux_Quantifier_Eliminator_WithSupportForLMIs in DAGEliminator.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return O;
	}

      //cout<<"\nSatisfiable! See result_thunsat_single_formula.txt for the model\n";

      //cout<<"\nFinding the "<<iteration<<" th cube\n";
      fprintf(LMIQEOutputDataFp, "\nFinding the %d th cube\n", iteration);

     //char c = getchar();
      
       gettimeofday (&fun_start_ms, NULL);

           
      Generalize1_WithSupportForSelsConcats_WithSupportForLMIs(Model, F, APSet, WidthTable, Constraints);
      
      //showAPVector("Constraints After Generalize1", Constraints);
      // new code added on 19 july 2015 starts here
      gettimeofday (&generalize2_start_ms, NULL); 
      number_of_aps_before_generalize2 = Constraints.size();
      // new code added on 19 july 2015 ends  here

      if(strcmp(Elimination_Type, "-allsmt") == 0)
        {
        // do nothing  
        }
      else if(generalize2_strategy==2) // Our strategy
	{
      	Generalize2_Strategy2_WithLMISupport(Constraints, F);
	}
      else
	{
	Generalize2_Monniaux(Constraints, F, WidthTable);
	}
      
      // new code added on 19 july 2015 starts here
      number_of_aps_after_generalize2 = Constraints.size();
      int number_of_aps_dropped = number_of_aps_before_generalize2 - number_of_aps_after_generalize2;
      assert(number_of_aps_dropped >= 0);
      contribution_generalize2 = (double)number_of_aps_dropped/(double)number_of_aps_before_generalize2;
      
      gettimeofday (&generalize2_finish_ms, NULL); 
      generalize2_duration_ms = generalize2_finish_ms.tv_sec * 1000 + generalize2_finish_ms.tv_usec / 1000;
      generalize2_duration_ms -= generalize2_start_ms.tv_sec * 1000 + generalize2_start_ms.tv_usec / 1000;  
      // new code added on 19 july 2015 ends here
      
       //showAPVector("Constraints After Generalize2", Constraints);
      gettimeofday (&cubefind_finish_ms, NULL); 
      TimePerConjunctInSMTAndGeneralize = cubefind_finish_ms.tv_sec * 1000 + cubefind_finish_ms.tv_usec / 1000;
      TimePerConjunctInSMTAndGeneralize -= cubefind_start_ms.tv_sec * 1000 + cubefind_start_ms.tv_usec / 1000;    
      
      
      DAGNode *Pi;
      
      if(LMI_QE_using_blasting)
      {
         struct timeval blasting_start_ms, blasting_finish_ms;          
         gettimeofday (&blasting_start_ms, NULL); 
         
         Pi = Project_Blast(Constraints, VarSetToElim, WidthTable);
         
        gettimeofday (&blasting_finish_ms, NULL);    
        unsigned long long int LocalTime = blasting_finish_ms.tv_sec * 1000 + blasting_finish_ms.tv_usec / 1000;
        LocalTime -= blasting_start_ms.tv_sec * 1000 + blasting_start_ms.tv_usec / 1000; 
        LMIBlastingTime += LocalTime;
        BlastingConjunctionCounter++;        
      }
      else if(LMI_QE_using_OT)
      {
          struct timeval ot_start_ms, ot_finish_ms;
          unsigned long long int LocalTime;
          unsigned long long int LocalOTTime, LocalProjectTime;
          
          
          if(project_depth == 0)//Original call: Call Project_OT
              {
              gettimeofday (&ot_start_ms, NULL); 
              
              if(OTAfterLayer2)
                {
                //cout<<"\nLet's call applyOTAfterLayer2\n";
                applyOTAfterLayer2(Constraints, VarSetToElim, WidthTable);
                }
              else
                {
                Project_OT(Constraints, VarSetToElim, WidthTable);
                }
              
              gettimeofday (&ot_finish_ms, NULL);
              LocalTime = ot_finish_ms.tv_sec * 1000 + ot_finish_ms.tv_usec / 1000;
              LocalTime -= ot_start_ms.tv_sec * 1000 + ot_start_ms.tv_usec / 1000; 
              LocalOTTime = LocalTime;
              LMIOTTime += LocalTime;
              }
          
          gettimeofday (&ot_start_ms, NULL); 
           
          Pi = callProject(Constraints, VarSetToElim, WidthTable, project_depth);   
          
          gettimeofday (&ot_finish_ms, NULL);
          LocalTime = ot_finish_ms.tv_sec * 1000 + ot_finish_ms.tv_usec / 1000;
          LocalTime -= ot_start_ms.tv_sec * 1000 + ot_start_ms.tv_usec / 1000; 
          LocalProjectTime = LocalTime;
             
          OTConjunctionCounter++;        
         
      }//else if(LMI_QE_using_OT)
      else
      {
          struct timeval pr_start_ms, pr_finish_ms;
          unsigned long long int LocalTime;
          
          gettimeofday (&pr_start_ms, NULL); 
          
                         
          Pi = callProject(Constraints, VarSetToElim, WidthTable, project_depth);      
          
          gettimeofday (&pr_finish_ms, NULL);
          LocalTime = pr_finish_ms.tv_sec * 1000 + pr_finish_ms.tv_usec / 1000;
          LocalTime -= pr_start_ms.tv_sec * 1000 + pr_start_ms.tv_usec / 1000;    
          
          
          if(project_depth == 0)
            {
            total_vars_to_elim_in_projects = total_vars_to_elim_in_projects + VarSetToElim.size();
            project_calls_in_path_based_strategy_in_lmdd_monniaux++;
            // Please do not get confused by the name of the variable. It is just a counter
            }
          
          //cout<<"\nTime for Project = "<< LocalTime<<" milli seconds\n";
      }
      
      gettimeofday (&fun_finish_ms, NULL);
      fun_duration_ms = fun_finish_ms.tv_sec * 1000 + fun_finish_ms.tv_usec / 1000;
      fun_duration_ms -= fun_start_ms.tv_sec * 1000 + fun_start_ms.tv_usec / 1000;
      
       if(project_depth == 0)
            {
            total_generalization_and_projection_time = total_generalization_and_projection_time + fun_duration_ms;
            }
      
      if(checkTimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles())
            {
                // Time out for the monniaux calls at start-up
                cout<<"\nThe initial Monniaux style calls have timed out!\n";
                TimedOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = true; // timed_out flag set
                return O;
            }

      //cout<<"\nPi = "<<getDAGStr(Pi)<<endl;
      O = createDAG("or", O, Pi, LOGICAL_SYMBOL, 1);
      //cout<<"\nO = "<<getDAGStr(O)<<endl;
      
      if(strcmp(Elimination_Type, "-allsmt") == 0)
      {
          DAGNode *ConjunctionOfConstraints = getConjunctionFromVector(Constraints);
          assert(ConjunctionOfConstraints != NULL);
          H = createDAG("and", H, createDAG("not", ConjunctionOfConstraints, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);
      }
      else
      {
        H = createDAG("and", H, createDAG("not", Pi, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);
      }
      //cout<<"\nH = "<<getDAGStr(H)<<endl;
     iteration++;
     
     gettimeofday (&smt_start_ms, NULL); 
     
     gettimeofday (&cubefind_start_ms, NULL); 
     
     }

  time(&end);
  duration = end - start;
  Quant_Monn_time = Quant_Monn_time + duration;
  
  
  return O;

}

// calls Project  with dynamic programming
DAGNode* callProject(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
    
 DAGNode* F = getConjunctionFromVector(Constraints);
 
 //cout<<"\nF in callProject = "<<getDAGStr(F)<<endl;
 //showSet(VarsToElim, "VarsToElim");
    
  // Check in HashTable  
 DAGNode* Existing  = getEntryFromMonniaux_HT(F, VarsToElim);

  if(Existing != NULL)
    {
      Monniaux_HT_hits++;
      //cout<<"\nHit in Monniaux_HT in callProject\n";
      //cout<<"\nExisting in call_lmiQEUsingOurMethods_With_Recursive_Splitting = "<<getDAGStr(Existing)<<endl;
      return Existing;
    }
 
 Monniaux_HT_misses++;
 //cout<<"\nMiss in Monniaux_HT in callProject\n";
 
 //Call lmiQEUsingOurMethods_With_Recursive_Splitting
 DAGNode* Result = Project(Constraints, VarsToElim, WidthTable, project_depth);
 
 //cout<<"\nCalling  insertEntryIntoMonniaux_HT from callProject\n";
 
 //Insert into HashTable
 insertEntryIntoMonniaux_HT(F, VarsToElim, Result);
 
 return Result;    
}

// Project function which computes \exists X. \pi where X is a set of variables
// and \pi is a conjunction of LMEs, LMDs, and LMIs
DAGNode* Project(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
// Local versions of the conjunction-level data start here
int PresentConjunctionNumberLocal=0;
int LMINoVarsToElimLocal=0;
int LMINoLMIsLocal=0;
int LMINoLMEsLocal=0;
int LMINoLMDsLocal=0;
int LMINoVarsLocal=0;
unsigned long long int LMITotalBitsLocal=0;
unsigned long long int LMIBitsToElimLocal=0;
int LMIVarsInputToLayer2Local=0;
int LMIVarsInputToLayer3Local=0;
int LMIVarsInputToLayer4Local=0;
unsigned long long int LMILayer1TimeLocal=0;
unsigned long long int LMILayer2TimeLocal=0;
unsigned long long int LMILayer3TimeLocal=0;
unsigned long long int LMILayer4TimeLocal=0;
unsigned long long int CubeFindingTimeLocal=TimePerConjunctInSMTAndGeneralize;
unsigned long long int SimpleSMTChecksTimeBeforeLayer3=0;

 // Local versions of the conjunction-level data end here  

  // Step 1: Convert strict LMIs to weak LMIs
   //showAPVector("Original Constraints", Constraints);
   convertStrictLMIsToWeakLMIs_WithLMDESupport(Constraints, WidthTable, VarsToElim);
   convertLMDEsToProperForm(Constraints, WidthTable);
   //showAPVector("Constraints after converting strict LMIs to weak LMIs and LMDEs to proper form", Constraints);
   
   // Step 2: Synthesize LMEs from LMIs
   
   tryToSynthesizeLMEsFromLMIs(Constraints,  VarsToElim,  WidthTable);
   //showAPVector("Constraints after LME synthesis", Constraints);      
  
  //showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showSet(VarsToElim, "VarsToElim");
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
 
  DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
  
  //showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  //showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);
  
  
  // get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
  // in the respective global variables
  getRelevantLMIDetails_With_Recursive_Splitting(Bound_Constraints, VarsToElim, WidthTable, LMINoVarsToElimLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMITotalBitsLocal, LMIBitsToElimLocal, PresentConjunctionNumberLocal);
  
  
  DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
            {
           Bound_Constraints_Vector.push_back(*it); 
            }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
      //Apply Layer1      
      
      DAGNode* Simplified_Bound_Part;
      
      if(!SwitchOffLayer1)
        {
          
         struct timeval layer1_start_ms, layer1_finish_ms;                  
         gettimeofday (&layer1_start_ms, NULL); 
    
         Simplified_Bound_Part = applyLMILayer1(Bound_Constraints, VarsToElim, WidthTable);
        
        gettimeofday (&layer1_finish_ms, NULL);
        LMILayer1TimeLocal = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
        LMILayer1TimeLocal -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
        }
      else
        {
        Simplified_Bound_Part =   getConjunctionFromSet(Bound_Constraints);
        }
      
      LMIVarsInputToLayer2Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part, VarsToElim);           
      
      if(LMIVarsInputToLayer2Local == 0) //Layer1 is successful in eliminating all the quantifiers
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, Simplified_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
          
          
          fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
          
          
          if(project_depth == 0)
            {
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);        
               
               
               fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);        
                             
               
               FMTypeConjunctionCounter++;
             }
          else
            {
             fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
            AdditionalConjunctionCounter ++; 
            }         
          
          //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
       }// if of  Layer1 is successful in eliminating all the quantifiers
      
      
      //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
      //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
      //cout<<"\nResult of Layer1 = "<<getDAGStr(Simplified_Bound_Part)<<endl;      
      
      // Call Layer2
                  
      DAGNode* QFree_Bound_Part;
      DAGNode* Quantified_Bound_Part;
      
      // Simplified_Bound_Part is the conjunction of 
      // QFree_Bound_Part and Quantified_Bound_Part
      // Former quantifier free and the latter quantified
      // First let us split Simplified_Bound_Part into QFree_Bound_Part and Quantified_Bound_Part
      
      convertToBoundAndFreeParts(Simplified_Bound_Part, VarsToElim, QFree_Bound_Part, Quantified_Bound_Part, WidthTable);
      //cout<<"\nQFree_Bound_Part in result of Layer1 = "<<getDAGStr(QFree_Bound_Part)<<endl;
      //cout<<"\nQuantified_Bound_Part in result of Layer1 = "<<getDAGStr(Quantified_Bound_Part)<<endl;
      
      // Apply Layer2
      
      // First split the conjunction Quantified_Bound_Part into set of atomic predicates
      set<DAGNode*>  Quantified_Bound_Part_Constraint_Set;
      splitConjunctionIntoSetGeneral(Quantified_Bound_Part, Quantified_Bound_Part_Constraint_Set);
      
      
      DAGNode* Simplified_Bound_Part_After_Layer2;
      if(!SwitchOffLayer2)
        {
          
         struct timeval layer2_start_ms, layer2_finish_ms;                  
         gettimeofday (&layer2_start_ms, NULL); 
    
         Simplified_Bound_Part_After_Layer2 = applyLMILayer2Recursive(Quantified_Bound_Part_Constraint_Set, VarsToElim, WidthTable);
         //cout<<"\nSimplified_Bound_Part_After_Layer2 = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;
         
        Simplified_Bound_Part_After_Layer2 = singleLMEElimination(Simplified_Bound_Part_After_Layer2);
        //cout<<"\nSimplified_Bound_Part_After_Layer2 after singleLMEElimination = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;
        
        gettimeofday (&layer2_finish_ms, NULL);
        LMILayer2TimeLocal = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
        LMILayer2TimeLocal -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;    
        }
      else
        {
        Simplified_Bound_Part_After_Layer2 =   getConjunctionFromSet(Quantified_Bound_Part_Constraint_Set);
        }
      
      LMIVarsInputToLayer3Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part_After_Layer2, VarsToElim);           
      
      if(LMIVarsInputToLayer3Local == 0) //Layer2 is successful in eliminating all the quantifiers
          // Effectively Layer2 did: \exists VarToElim.(Quantified_Bound_Part) \equiv True
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, QFree_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
          
          
          fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
       
          
          
          if(project_depth == 0)
            {
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);        
               
               
               fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);        
                             
               
               FMTypeConjunctionCounter++;
             }
          else
            {
             fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
            AdditionalConjunctionCounter ++; 
            }         
          
          //cout<<"\nLMILayer2TimeLocal = "<<LMILayer2TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer3Local = "<<LMIVarsInputToLayer3Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
       }// if of  Layer2 is successful in eliminating all the quantifiers
      
      
      //cout<<"\nLMILayer2TimeLocal = "<<LMILayer2TimeLocal<<endl;
      //cout<<"\nLMIVarsInputToLayer3Local = "<<LMIVarsInputToLayer3Local<<endl;
      //cout<<"\nResult of Layer2 = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;      
      
      //Calling Layer 3
      
      DAGNode *InputToLayer3 = Simplified_Bound_Part_After_Layer2;
      
      DAGNode* PartialResultDAG;
      PartialResultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, QFree_Bound_Part, LOGICAL_SYMBOL, 1));
      //cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
      //cout<<"\nWe still need to eliminate quantifiers from "<<getDAGStr(InputToLayer3)<<endl;

      set<DAGNode*> InputToLayer3_Constraints;
      splitConjunctionIntoSetGeneral(InputToLayer3, InputToLayer3_Constraints);
      
      set< set<DAGNode*> > ScopeReducedDAGs;//Output of Layer3 is a set of scope reduced dags           

     bool result_trustworthy;
         
     if(SwitchOffLayer3)
     {
         result_trustworthy =   false;
     }
     else
     { 
         
        struct timeval layer3_start_ms, layer3_finish_ms;
        gettimeofday (&layer3_start_ms, NULL);     
        
        result_trustworthy =   semanticScopeReduction(InputToLayer3, InputToLayer3_Constraints, VarsToElim, WidthTable, ScopeReducedDAGs);  
        
        gettimeofday (&layer3_finish_ms, NULL);
        LMILayer3TimeLocal = layer3_finish_ms.tv_sec * 1000 + layer3_finish_ms.tv_usec / 1000;
        LMILayer3TimeLocal -= layer3_start_ms.tv_sec * 1000 + layer3_start_ms.tv_usec / 1000;

        LMILayer3Calls++;
     }
     
     if(!result_trustworthy)
              {              
              
              if(project_depth == 0)
                {
                LMILayer3CallsTimedOut++;
                
                 int LMCCountInInputConjunction;
                 int VarsCountInInputConjunction;
                 int VarsToElimCountInInputConjunction;
                 int PartitionsInOutputConjunction;
                 int MaxPartitionSizeInOutputConjunction;
                 int MinPartitionSizeInOutputConjunction;
                 float AvgPartitionSizeInOutputConjunction;
                   
                 extractLayer3Data(false, InputToLayer3_Constraints, VarsToElim, ScopeReducedDAGs, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction);
                   
                 fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t0\t%d\t%d\t%d\t-\t-\t-\t-\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, LMILayer3TimeLocal);
                 fprintf(LMIQEFinishedLayer3TimedOutDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t-\t-\t-\t-\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, LMILayer3TimeLocal);
                }
              
              
                if(SimpleSMTChecksBeforeLayer3)
                {   
                    fprintf(LMIQEOutputDataFp, "\nPerforming simple checks on %s\n", getDAGStr(InputToLayer3).c_str());
                    
                    set<string> VarsToElim_In_InputToLayer3;
                    
                    set<string> Support_InputToLayer3;
                    getDAGNames(InputToLayer3, Support_InputToLayer3);
                    
                    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_InputToLayer3.begin(), Support_InputToLayer3.end(),inserter(VarsToElim_In_InputToLayer3, VarsToElim_In_InputToLayer3.begin()));
                                        
                    showSet(VarsToElim_In_InputToLayer3, "VarsToElim_In_InputToLayer3", LMIQEOutputDataFp);
                    
                    struct timeval checks_start_ms, checks_finish_ms;
                    gettimeofday (&checks_start_ms, NULL);    
                    
                    string Result_Checks = simpleSMTChecks(InputToLayer3, VarsToElim_In_InputToLayer3, WidthTable);
                    
                    gettimeofday (&checks_finish_ms, NULL);
                    SimpleSMTChecksTimeBeforeLayer3 = checks_finish_ms.tv_sec * 1000 + checks_finish_ms.tv_usec / 1000;
                    SimpleSMTChecksTimeBeforeLayer3 -= checks_start_ms.tv_sec * 1000 + checks_start_ms.tv_usec / 1000;
                    
                    fprintf(LMIQEOutputDataFp, "\nResult of the checks is %s\n", Result_Checks.c_str());
                    
                    if(Result_Checks == "valid" || Result_Checks == "unsat")//exists VarsToElim. (InputToLayer3) is True
                        // or exists VarsToElim. (InputToLayer3) is False
                    {
                      fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                      fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
               
               
                      fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                      fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
               
               
                      if(project_depth==0)
                        {
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         
                         
                         fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
                        
                         
                         FMTypeConjunctionCounter++;
                        }
                    else
                        {
                         fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                     
                         AdditionalConjunctionCounter++;
                        }
               
                    if(Result_Checks == "valid")//exists VarsToElim. (InputToLayer3) is True
                        {
                           return PartialResultDAG;
                        }
                    else//if(Result_Checks == "unsat")
                        {
                          return createLogicalConstant("false");
                        }                    
                    }//if(Result_Checks == "valid" || Result_Checks == "unsat")
                } //if(SimpleSMTChecksBeforeLayer3)           
              
              
                 //cout<<"\nz3 timed out during computation. Result cannot be trusted\n";
                 //fprintf(LMIQEOutputDataFp, "\nz3 timed out during computation. Result is cannot be trusted\n");                 
                 //fprintf(LMIQEOutputDataFp, "\nCalling the expensive layers\n");                      
                 //cout<<"\nCalling expensive layers for QE from "<<getDAGStr(InputToLayer3)<<endl;                     
                 fprintf(LMIQEOutputDataFp, "\nCalling expensive layers for QE from %s\n", getDAGStr(InputToLayer3).c_str());
                                  
                 //Calling Layer4                 
                 
                 DAGNode* InputToLayer4 = InputToLayer3;
                 LMIVarsInputToLayer4Local = findNumberOfVarsToElimRemaining(InputToLayer4, VarsToElim);  
                     
                 DAGNode *RemainingResult;                  
                
                 struct timeval layer4_start_ms, layer4_finish_ms;
                 gettimeofday (&layer4_start_ms, NULL);    
                                  
                 
                 vector<DAGNode*> InputToLayer4_Vector;
                 splitConjunctionIntoVectorGeneral(InputToLayer4, InputToLayer4_Vector);
                 
                 unsigned long long int FMTime=0;
                 unsigned long long int NormalizationTime=0;
                 unsigned long long int MonTime=0;
                 
                 RemainingResult = call_applyLMILayer4(InputToLayer4_Vector, VarsToElim, WidthTable, project_depth, FMTime, NormalizationTime, MonTime);  
                     
                 gettimeofday (&layer4_finish_ms, NULL);
                 LMILayer4TimeLocal = layer4_finish_ms.tv_sec * 1000 + layer4_finish_ms.tv_usec / 1000;
                 LMILayer4TimeLocal -= layer4_start_ms.tv_sec * 1000 + layer4_start_ms.tv_usec / 1000;
                 
                  //cout<<"\nLMILayer4TimeLocal = "<<LMILayer4TimeLocal<<endl;
                      
                 if(checkTimeOut())
                      {
                      fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                      fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                       
                      
                      fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                      fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
                     
                         if(project_depth == 0)
                         {
                             
                           if(false)
                                {
                                fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                                fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                                
                                
                                fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                                fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
                                
                                
                                FMTypeConjunctionCounter++;
                                }
                         }
                         else
                         {
                           fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                           fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                          AdditionalConjunctionCounter++;
                         }
                         // Time Out !!!
                      cout<<"\nWarning!!!The function Project in Eliminator.cpp has timed out. Its result is not exact\n";
                      timed_out = true; // timed_out flag set
                      return createLogicalConstant("true");
                     }
                 
                 if(checkTimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles())
                    {
                     fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                     fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                     
                                          
                     fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                     fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
                   
                     
                     if(project_depth == 0)
                           {
                           if(false)
                                {
                                 fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                                 fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                                 
                                 
                                 
                                 fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                                 fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
                                 
                                 
                                 FMTypeConjunctionCounter++;
                                }
                            }
                     else
                           {
                           fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                           fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                          AdditionalConjunctionCounter++;
                           }                  
                     
                    // Time out for the monniaux calls at start-up
                    cout<<"\nThe initial Monniaux style calls have timed out!\n";
                    TimedOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = true; // timed_out flag set
                    return createLogicalConstant("false");
                    }
                      
               fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
               
               
               fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
               
               
               if(project_depth==0)
                        {
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         
                         
                         fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, CubeFindingTimeLocal, SimpleSMTChecksTimeBeforeLayer3);
                        
                         
                         FMTypeConjunctionCounter++;
                        }
               else
                        {
                         fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                        AdditionalConjunctionCounter++;
                        }
               
               DAGNode* resultDAG;
               resultDAG = createDAG("and", PartialResultDAG, RemainingResult, LOGICAL_SYMBOL, 1);
               return resultDAG;
              }//if(!result_trustworthy)
     else //result is trust worthy
              {
              
               if(project_depth == 0)
                    {
                   LMILayer3CallsFinished++;
                   
                   int LMCCountInInputConjunction;
                   int VarsCountInInputConjunction;
                   int VarsToElimCountInInputConjunction;
                   int PartitionsInOutputConjunction;
                   int MaxPartitionSizeInOutputConjunction;
                   int MinPartitionSizeInOutputConjunction;
                   float AvgPartitionSizeInOutputConjunction;
                   
                   extractLayer3Data(true, InputToLayer3_Constraints, VarsToElim, ScopeReducedDAGs, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction);
                   
                   
                   
                    if(ScopeReducedDAGs.size()==0)
                        {
                        LMILayer3CallsTrue++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t3\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        fprintf(LMIQEFinishedLayer3TrueDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                         }
                   else if(ScopeReducedDAGs.size()==1)
                        {
                        LMILayer3CallsNoSC++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t2\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        fprintf(LMIQEFinishedLayer3NoSCDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        }
                   else
                        {
                         LMILayer3CallsSCAchieved++;
                        fprintf(LMIQEFinishedLayer3DataFp, "\n\n%s\t%d\t1\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        fprintf(LMIQEFinishedLayer3SCAchievedDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%llu\n", InFileName.c_str(), PresentConjunctionNumberLocal, LMCCountInInputConjunction, VarsCountInInputConjunction, VarsToElimCountInInputConjunction, PartitionsInOutputConjunction, MaxPartitionSizeInOutputConjunction, MinPartitionSizeInOutputConjunction, AvgPartitionSizeInOutputConjunction, LMILayer3TimeLocal);
                        }
                    }
               
                  cout<<"\nScopeReducedDAGs\n";
                  fprintf(LMIQEOutputDataFp, "\nPartitioned DAGs\n");
                  int partition_number=1;
                  for(set< set<DAGNode*> >::iterator sdagit = ScopeReducedDAGs.begin(); sdagit != ScopeReducedDAGs.end(); sdagit++)
                  {
                    fprintf(LMIQEOutputDataFp, "\nPartition %d\n", partition_number);
                    cout<<"\nPartition "<<partition_number<<endl;
                    set<DAGNode*> MyDAGSet = *sdagit;
                    //showAPSet("Partition", MyDAGSet, LMIQEOutputDataFp);  
                    partition_number++;
                    //showAPSet("ScopeReducedDAGs", MyDAGSet);  
                  }//for
                  
                  //Calling Layer4
                  
                  if(ScopeReducedDAGs.empty())//Other part true
                    {
                      
                        fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

                        if(project_depth==0)
                        {
                        fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                        FMTypeConjunctionCounter++;
                        }
                        else
                        {
                         fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         AdditionalConjunctionCounter++;                        
                        }
                        
                        return PartialResultDAG;
                    }
                  else //scope reduced set is not empty
                    {
                      cout<<"\nQE needs to done\n";
                      fprintf(LMIQEOutputDataFp, "\nQE needs to done\n");
                      //fprintf(LMIQEImportantDataFp, "(unfinished)");
                      
                        fprintf(LMIQEOutputDataFp, "\nCalling the expensive layers\n"); 
                        
                        LMIVarsInputToLayer4Local = LMIVarsInputToLayer3Local;  // Layer3 has just reduced the scopes of quantifiers
                     
                        DAGNode *RemainingResult;                  
                
                        struct timeval layer4_start_ms, layer4_finish_ms;
                        gettimeofday (&layer4_start_ms, NULL);    
                                  
                        
                        RemainingResult = call_applyLMILayer4OnSetOfDAGs(ScopeReducedDAGs , VarsToElim, WidthTable, project_depth);  
                     
                        gettimeofday (&layer4_finish_ms, NULL);
                        LMILayer4TimeLocal = layer4_finish_ms.tv_sec * 1000 + layer4_finish_ms.tv_usec / 1000;
                        LMILayer4TimeLocal -= layer4_start_ms.tv_sec * 1000 + layer4_start_ms.tv_usec / 1000;
                        
                        
                        if(checkTimeOut())
                        {
                         fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

                         if(project_depth == 0)
                            {
                             if(false)
                                {
                                 fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                                 fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                                 FMTypeConjunctionCounter++;
                                }
                            }
                         else
                            {
                            fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                            fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                            AdditionalConjunctionCounter++;
                            }                        
                         
                          // Time Out !!!
                        cout<<"\nWarning!!!The function Project in DAGEliminator.cpp has timed out. Its result is unknown\n";
                        timed_out = true; // timed_out flag set
                        return createLogicalConstant("true"); // Let's temporarily treat it as sat in the called function
                        }              
                        
                              
                        fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

                        if(project_depth == 0) 
                        {
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         FMTypeConjunctionCounter++; 
                        }
                        else
                        {
                        fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                        fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                        AdditionalConjunctionCounter++;
                        }
                        
                        
                        DAGNode* resultDAG;
                        
                        //cout<<"\nRemainingResult = "<<getDAGStr(RemainingResult)<<endl;
                        //cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
                        
                        resultDAG = createDAG("and", PartialResultDAG, RemainingResult, LOGICAL_SYMBOL, 1);
                        
                        //cout<<"\nCalling expensive layers completed\n";
                        //cout<<"\nFinal Result of QE = "<<getDAGStr(resultDAG)<<endl;
                        //cout<<"\nPress any key to continue\n";
                        //char c = getchar();
                        
                        return resultDAG;
                        }//else of if(ScopeReducedDAGs.empty())
              }// else of if(!result_trust_worthy) 
}// end of function


// Functions to perform the Layer4: Normalization + Fourier-Motzkin

// Performs Layer4 on a set of DAGs : Corrected Version
DAGNode* call_applyLMILayer4OnSetOfDAGs(set< set<DAGNode*> > &ScopeReducedDAGs , set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
    DAGNode *F = createLogicalConstant("true");    
    int set_index=1;
    
    for(set< set<DAGNode*> >::iterator it=ScopeReducedDAGs.begin(); it != ScopeReducedDAGs.end(); it++)
    {
        //fprintf(LMIQEOutputDataFp, "\nTaking the scope reduced set %d\n", set_index);
        //cout<<"\nTaking the scope reduced set "<<set_index<<endl;
        
        set<DAGNode*> MyScopeReducedSet;
        MyScopeReducedSet = *it;
        DAGNode *MyConjunction = getConjunctionFromSet(MyScopeReducedSet);
        
        if(!isTrue(MyConjunction))
        {
            //cout<<"\nMyConjunction = "<<getDAGStr(MyConjunction)<<endl;
            vector<DAGNode*> MyConjunctionVector;
            copySetToVector(MyScopeReducedSet, MyConjunctionVector);
            
            unsigned long long int FMTime=0;
            unsigned long long int NormalizationTime=0;
            unsigned long long int MonTime=0;
            
            DAGNode *Pi = call_applyLMILayer4(MyConjunctionVector, VarsToElim, WidthTable, project_depth, FMTime, NormalizationTime, MonTime);
            
            
             if(checkTimeOut())
            {
            // Time Out !!!
            cout<<"\nWarning!!!The function applyLMILayer4OnSetOfDAGs in DAGEliminator.cpp has timed out. Its result is not exact\n";
            timed_out = true; // timed_out flag set
            return createLogicalConstant("true");
            }
            
            //fprintf(LMIQEOutputDataFp, "\nQE from the scope reduced set %d is finished\n", set_index);
           //cout<<"\nQE from the scope reduced set "<<set_index<<" is finished\n";
            
            //fprintf(LMIQEOutputDataFp, "\nResult of QE from scope reduced set %d = %s\n", set_index, getDAGStr(Pi).c_str());
            //cout<<"\nResult of QE from scope reduced set "<<set_index<<" = "<<getDAGStr(Pi)<<endl;
            
            if(isTrue(F))
                {
                F = Pi;
                }
            else
                {
                F=createDAG("and", F, Pi, LOGICAL_SYMBOL, 1);
                }
        }//if(!isTrue(MyConjunction))
        set_index++;
    }//for
    return F;
}//function



// calls Layer4 on a dag  with dynamic programming : Corrected Version
DAGNode* call_applyLMILayer4(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth, unsigned long long int &FMTime, unsigned long long int &NormalizationTime, unsigned long long int &MonTime)
{    
 DAGNode* F = getConjunctionFromVector(Constraints);
 
 //cout<<"\nF before call to applyLMILayer4: "<<getDAGStr(F)<<endl;
 //cout<<"\nF =  "<<F<<endl;
 //showSet(VarsToElim, "VarsToElim");
    
  // Check in HashTable  
 DAGNode* Existing  = getEntryFromMonniaux_HT(F, VarsToElim);

 if(Existing != NULL)
    {
      Monniaux_HT_hits++;
      cout<<"\nHit in call_applyLMILayer4\n";
      return Existing;
    }
 
 Monniaux_HT_misses++;
 cout<<"\nMiss in call_applyLMILayer4\n";
 
 if(project_depth == 0)
                    {
                     set<string> VarsToElim_In_F;
                    
                    set<string> Support_F;
                    getDAGNames(F, Support_F);                    
                    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_F.begin(), Support_F.end(),inserter(VarsToElim_In_F, VarsToElim_In_F.begin()));
                                      
                     attachInputToLayer3ToConstraintsFile(VarsToElim_In_F, F);
                    }
 
 //call applyLMILayer4
 DAGNode* Result = applyLMILayer4(F, VarsToElim, WidthTable, project_depth, FMTime, NormalizationTime, MonTime);
 
 //cout<<"\nF after call to applyLMILayer4: "<<getDAGStr(F)<<endl;
 //cout<<"\nF =  "<<F<<endl;
 //showSet(VarsToElim, "VarsToElim");
 //cout<<"\nResult after call to applyLMILayer4: "<<getDAGStr(Result)<<endl;
 
 //cout<<"\nCalling insertEntryIntoMonniaux_HT from call_applyLMILayer4\n";
 
 //Insert into HashTable
 insertEntryIntoMonniaux_HT(F, VarsToElim, Result);
 
 return Result;    
}



// apply Layer4 on \exists VarsToElim. Phi
// Note that presently only constraints of the form kx1+kx2+...+kxn+d \bowtie kx1+kx2+...+kxn+e
// are handled, where k \in \{ +c, -c \} and \bowtie \in \{<=, => \}
DAGNode* applyLMILayer4(DAGNode* Phi, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth, unsigned long long int &FMTime, unsigned long long int &NormalizationTime, unsigned long long int &MonTime)
{
 //cout<<"\nCalling applyLayer4 on "<<getDAGStr(Phi)<<endl;
 //showSet(VarsToElim, "VarsToElim");
 
 // Convert >= constraints to <=.
 // Also recreate all the constraints with a proper variable ordering
 Phi = convertBVUGEConstraintsToBVULECorrected(Phi, WidthTable);
 //cout<<"\nConverted Phi = "<<getDAGStr(Phi)<<endl;
  
  set<DAGNode*> Constraints;
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QE_Bound_Constraints;
  
  
  splitConjunctionIntoSetGeneral(Phi, Constraints);
  //showAPSet("Constraints Input to Layer4", Constraints);

  set<string> Support_Phi;
  set<string> VarsToElimInSupport;
  getDAGNames(Phi, Support_Phi);
  set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_Phi.begin(), Support_Phi.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));

  //showSet(VarsToElimInSupport, "Variables to eliminate", LMIQEOutputDataFp); 
  //showSet(VarsToElimInSupport,"VarsToElimInSupport");
  
  struct timeval fm_start_ms, fm_finish_ms;
  gettimeofday (&fm_start_ms, NULL);    
  
  set<string> VarsToElimNeedOfSplitting;
  
  for(set<string>::iterator it = VarsToElimInSupport.begin(); it != VarsToElimInSupport.end(); it++)
    {
    string variable = *it;
    //cout<<"\nvariable = "<<variable<<endl;
    //showAPSet("Constraints", Constraints);
    
    if(constraintsAlreadyInNormalForm(Constraints, variable, WidthTable))
        {
        //cout<<"\nThe constraints are already in normal form\n";
        Free_Constraints.clear();
        Bound_Constraints.clear();
        getFreeAndBoundConstraints(Constraints, variable, Free_Constraints, Bound_Constraints, WidthTable);
    
        DAGNode* resultOfFM;
        
        if(simpleFourierMotzkinCanBeApplied(Bound_Constraints, variable, WidthTable))
        {
           resultOfFM = applyFourierMotzkin(Bound_Constraints, variable, WidthTable);
        }
        else
        {
           list<DAGNode*> TempAPListForDD; //applicable for -dd
            
           resultOfFM = quickSimplify_WithLMISupport(applyFourierMotzkinForEvenCoefficients(Bound_Constraints, variable, WidthTable, TempAPListForDD));
           
           if(!checkIfConjunctionOfLMCs(resultOfFM))
            {
            cout<<"\nIn function applyLMILayer4 in DAGEliminator.cpp: Fourier-Motzkin results in splitting. Normalization is not implemented on splitted dags\n";
            exit(1);
            }
        }       
        
        //cout<<"\nresultOfFM = "<<getDAGStr(resultOfFM)<<endl;
        
        QE_Bound_Constraints.clear();
        splitConjunctionIntoSetGeneral(resultOfFM, QE_Bound_Constraints);
        
        Constraints.clear();
        set_union(Free_Constraints.begin(), Free_Constraints.end(), QE_Bound_Constraints.begin(), QE_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));
	}
    else
        {   
          //cout<<"\nThe constraints are not in normal form\n";
          VarsToElimNeedOfSplitting.insert(variable); 
        }
    }//for
  
  gettimeofday (&fm_finish_ms, NULL);
  FMTime = fm_finish_ms.tv_sec * 1000 + fm_finish_ms.tv_usec / 1000;
  FMTime -= fm_start_ms.tv_sec * 1000 + fm_start_ms.tv_usec / 1000;
 
  //showSet(VarsToElimNeedOfSplitting, "VarsToElimNeedOfSplitting");
  
  if(VarsToElimNeedOfSplitting.empty())// all variables eliminated
    {
    //cout<<"\nVarsToElimNeedOfSplitting.empty()\n";
    DAGNode* resultDAG = getConjunctionFromSet(Constraints);
    return resultDAG;
    }
  
  
  // We need to eliminate VarsToElimNeedOfSplitting from /\Constraints
  Free_Constraints.clear();
  Bound_Constraints.clear();
  
  vector<DAGNode*> Constraints_Vector;

  copySetToVector(Constraints, Constraints_Vector);
  getFreeAndBoundConstraintsForMultipleVariables(Constraints_Vector, VarsToElimNeedOfSplitting, Free_Constraints, Bound_Constraints, WidthTable);

  
  DAGNode* FreePart = getConjunctionFromSet(Free_Constraints);
  
  if(Bound_Constraints.empty())//No bound part
    {
      return FreePart;
    }
  
  // We need to eliminate VarsToElimNeedOfSplitting from /\Bound_Constraints
  DAGNode* BoundPart = getConjunctionFromSet(Bound_Constraints);
  DAGNode* QFree_BoundPart;
  
  //first check if \exists VarsToElimNeedOfSplitting. BoundPart is already in the hash table
  
// Check in HashTable  
DAGNode* Existing  = getEntryFromMonniaux_HT(BoundPart, VarsToElimNeedOfSplitting);

if(Existing != NULL)
    {
     Monniaux_HT_hits++;
    //cout<<"\nHit in Monniaux_HT in applyLMILayer4\n";
      //cout<<"\nExisting in call_lmiQEUsingOurMethods_With_Recursive_Splitting = "<<getDAGStr(Existing)<<endl;
    QFree_BoundPart =  Existing;
    }//if(Existing != NULL)
else
    {
    Monniaux_HT_misses++;
    //cout<<"\nMiss in Monniaux_HT in applyLMILayer4\n";

    //Compute the result
    set<string>::iterator it = VarsToElimNeedOfSplitting.begin();
    string VariableSelected = *it;
    //cout<<"\nVariable selected for elimination = "<<VariableSelected<<endl;
    
    struct timeval norm_start_ms, norm_finish_ms;
    gettimeofday (&norm_start_ms, NULL);   

    DAGNode* NormalForm;

    set<string> VariablesSelectedForNormalization;
    selectVariablesForNormalization(Bound_Constraints, VarsToElimNeedOfSplitting, VariablesSelectedForNormalization, WidthTable);
    
    list<DAGNode*> TemporaryAPList; //This  is useful only in the LMDD case
    
    NormalForm = quickSimplify_WithLMISupport(convertToNormalFormForMultipleVariablesCorrected(Bound_Constraints, VariablesSelectedForNormalization, WidthTable, TemporaryAPList));
       
    //cout<<"\nNormalForm = "<<getDAGStr(NormalForm)<<endl;
    
   showSet(VariablesSelectedForNormalization, "VariablesSelectedForNormalization", LMIQEOutputDataFp);
   showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp); 
   fprintf(LMIQEOutputDataFp, "\nNormal form = %s\n", getDAGStr(NormalForm).c_str());  


  gettimeofday (&norm_finish_ms, NULL);
  NormalizationTime = norm_finish_ms.tv_sec * 1000 + norm_finish_ms.tv_usec / 1000;
  NormalizationTime -= norm_start_ms.tv_sec * 1000 + norm_start_ms.tv_usec / 1000;
    
    //cout<<"\nCalling Monniaux to eliminate quantifiers from NormalForm\n";
  
  struct timeval mon_start_ms, mon_finish_ms;
  gettimeofday (&mon_start_ms, NULL);   

   DAGNode* QE_NormalForm = quickSimplify_WithLMISupport(callMonniaux_Quantifier_Eliminator_WithSupportForLMIs(NormalForm, VarsToElimNeedOfSplitting, WidthTable, project_depth+1));

    //cout<<"\nQE_NormalForm = "<<getDAGStr(QE_NormalForm)<<endl;

  gettimeofday (&mon_finish_ms, NULL);
  MonTime = mon_finish_ms.tv_sec * 1000 + mon_finish_ms.tv_usec / 1000;
  MonTime -= mon_start_ms.tv_sec * 1000 + mon_start_ms.tv_usec / 1000;
    
    QFree_BoundPart = QE_NormalForm;

    //Insert into HashTable
    insertEntryIntoMonniaux_HT(BoundPart, VarsToElimNeedOfSplitting, QE_NormalForm);

   }//else of if(Existing != NULL)
  
    
  DAGNode* resultDAG = createDAG("and", FreePart, QFree_BoundPart, LOGICAL_SYMBOL, 1);
   
  return resultDAG;
}//function



// Project_Blast function which computes \exists X. \pi where X is a set of variables
// and \pi is a conjunction of LMEs, LMDs, and LMIs using Layer1 followed by blasting + CUDD
DAGNode* Project_Blast(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
// Local versions of the conjunction-level data start here
int PresentConjunctionNumberLocal=0;
int LMINoVarsToElimLocal=0;
int LMINoLMIsLocal=0;
int LMINoLMEsLocal=0;
int LMINoLMDsLocal=0;
int LMINoVarsLocal=0;
unsigned long long int LMITotalBitsLocal=0;
unsigned long long int LMIBitsToElimLocal=0;
int LMIVarsInputToLayer2Local=0;
unsigned long long int LMILayer1TimeLocal=0;
unsigned long long int LMILayer2TimeLocal=0;
 // Local versions of the conjunction-level data end here  

// Step 1: Convert strict LMIs to weak LMIs
   //showAPVector("Original Constraints", Constraints);
   convertStrictLMIsToWeakLMIs_WithLMDESupport(Constraints, WidthTable, VarsToElim);
   convertLMDEsToProperForm(Constraints, WidthTable);
   //showAPVector("Constraints after converting strict LMIs to weak LMIs and LMDEs to proper form", Constraints);
   
// Step 2: Synthesize LMEs from LMIs
   
  tryToSynthesizeLMEsFromLMIs(Constraints,  VarsToElim,  WidthTable);
 //showAPVector("Constraints after LME synthesis", Constraints);        
  
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showSet(VarsToElim, "VarsToElim");
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
 
  DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
  
  
  // get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
  // in the respective global variables
  getRelevantLMIDetails_With_Recursive_Splitting(Bound_Constraints, VarsToElim, WidthTable, LMINoVarsToElimLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMITotalBitsLocal, LMIBitsToElimLocal, PresentConjunctionNumberLocal);
  
  
  DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
            {
           Bound_Constraints_Vector.push_back(*it); 
            }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
  //Apply Layer1      
      
  DAGNode* Simplified_Bound_Part;
      
  if(!SwitchOffLayer1)
        {
          
         struct timeval layer1_start_ms, layer1_finish_ms;                  
         gettimeofday (&layer1_start_ms, NULL); 
    
         Simplified_Bound_Part = applyLMILayer1(Bound_Constraints, VarsToElim, WidthTable);
        
        gettimeofday (&layer1_finish_ms, NULL);
        LMILayer1TimeLocal = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
        LMILayer1TimeLocal -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
        }
 else
        {
        Simplified_Bound_Part =   getConjunctionFromSet(Bound_Constraints);
        }
      
 LMIVarsInputToLayer2Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part, VarsToElim);           
      
 if(LMIVarsInputToLayer2Local == 0) //Layer1 is successful in eliminating all the quantifiers
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, Simplified_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataBlastingFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataBlastingFp, "\t%d\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMILayer1TimeLocal, LMILayer2TimeLocal);
          
          //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
       }// if Layer1 is successful in eliminating all the quantifiers
      
      
 //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
 //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
 //cout<<"\nResult of Layer1 = "<<getDAGStr(Simplified_Bound_Part)<<endl;      
      
 // Call Layer2: Blasting + CUDD
                  
 DAGNode* QFree_Bound_Part;
 DAGNode* Quantified_Bound_Part;
      
 // Simplified_Bound_Part is the conjunction of 
 // QFree_Bound_Part and Quantified_Bound_Part
 // Former quantifier free and the latter quantified
 // First let us split Simplified_Bound_Part into QFree_Bound_Part and Quantified_Bound_Part
      
 convertToBoundAndFreeParts(Simplified_Bound_Part, VarsToElim, QFree_Bound_Part, Quantified_Bound_Part, WidthTable);
 //cout<<"\nQFree_Bound_Part in result of Layer1 = "<<getDAGStr(QFree_Bound_Part)<<endl;
 //cout<<"\nQuantified_Bound_Part in result of Layer1 = "<<getDAGStr(Quantified_Bound_Part)<<endl;
      
 
      
 // First split the conjunction Quantified_Bound_Part into set of atomic predicates
 set<DAGNode*>  Quantified_Bound_Part_Constraint_Set;
 splitConjunctionIntoSetGeneral(Quantified_Bound_Part, Quantified_Bound_Part_Constraint_Set);
 vector<DAGNode*>  Quantified_Bound_Part_Constraint_Vector;
 copySetToVector(Quantified_Bound_Part_Constraint_Set, Quantified_Bound_Part_Constraint_Vector);
      
 set<string> Quantified_Bound_Part_Support;
 set<string> CommonVars;
    
  getDAGNames(Quantified_Bound_Part, Quantified_Bound_Part_Support);
  set_intersection(VarsToElim.begin(), VarsToElim.end(), Quantified_Bound_Part_Support.begin(), Quantified_Bound_Part_Support.end(),inserter(CommonVars, CommonVars.begin()));

  struct timeval layer2_start_ms, layer2_finish_ms;                  
  gettimeofday (&layer2_start_ms, NULL); 
      
  DAGNode* ResultFromCUDD = QE_Using_Blasting_WithSupportForLMIs(Quantified_Bound_Part_Constraint_Vector, CommonVars, WidthTable);

  gettimeofday (&layer2_finish_ms, NULL);
  LMILayer2TimeLocal = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
  LMILayer2TimeLocal -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;    
      
  DAGNode* resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, QFree_Bound_Part, LOGICAL_SYMBOL, 1));
     
  resultDAG = quickSimplify_WithLMISupport(createDAG("and", resultDAG, ResultFromCUDD, LOGICAL_SYMBOL, 1)); 
      
  fprintf(LMIQEImportantDataBlastingFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
  fprintf(LMIQEImportantDataBlastingFp,  "\t%d\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMILayer1TimeLocal, LMILayer2TimeLocal);
          
        
  //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
  return resultDAG;
}// end of function



// Project_OT function which computes \exists X. \pi where X is a set of variables
// and \pi is a conjunction of LMEs, LMDs, and LMIs using Layer1 followed by conversion to ILA + Omega Test
DAGNode* Project_OT(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
// Local versions of the conjunction-level data start here
int PresentConjunctionNumberLocal=0;
int LMINoVarsToElimLocal=0;
int LMINoLMIsLocal=0;
int LMINoLMEsLocal=0;
int LMINoLMDsLocal=0;
int LMINoVarsLocal=0;
unsigned long long int LMITotalBitsLocal=0;
unsigned long long int LMIBitsToElimLocal=0;
int LMIVarsInputToLayer2Local=0;
unsigned long long int LMILayer1TimeLocal=0;
unsigned long long int LMILayer2TimeLocal=0;
 // Local versions of the conjunction-level data end here  

// Step 1: Convert strict LMIs to weak LMIs
//showAPVector("Original Constraints", Constraints);
 convertStrictLMIsToWeakLMIs_WithLMDESupport(Constraints, WidthTable, VarsToElim);
 convertLMDEsToProperForm(Constraints, WidthTable);
//showAPVector("Constraints after converting strict LMIs to weak LMIs and LMDEs to proper form", Constraints);
   
// Step 2: Synthesize LMEs from LMIs
   
  tryToSynthesizeLMEsFromLMIs(Constraints,  VarsToElim,  WidthTable);
 //showAPVector("Constraints after LME synthesis", Constraints);        
  
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showSet(VarsToElim, "VarsToElim");
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
 
  DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
  
  
  // get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
  // in the respective global variables
  getRelevantLMIDetails_With_Recursive_Splitting(Bound_Constraints, VarsToElim, WidthTable, LMINoVarsToElimLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMITotalBitsLocal, LMIBitsToElimLocal, PresentConjunctionNumberLocal);
  
  
  DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
            {
           Bound_Constraints_Vector.push_back(*it); 
            }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
  //Apply Layer1      
      
  DAGNode* Simplified_Bound_Part;
      
  if(!SwitchOffLayer1)
        {
          
         struct timeval layer1_start_ms, layer1_finish_ms;                  
         gettimeofday (&layer1_start_ms, NULL); 
    
        Simplified_Bound_Part = applyLMILayer1(Bound_Constraints, VarsToElim, WidthTable);
        
        gettimeofday (&layer1_finish_ms, NULL);
        LMILayer1TimeLocal = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
        LMILayer1TimeLocal -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
        }
 else
        {
        Simplified_Bound_Part =   getConjunctionFromSet(Bound_Constraints);
        }
      
 LMIVarsInputToLayer2Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part, VarsToElim);           
      
 if(LMIVarsInputToLayer2Local == 0) //Layer1 is successful in eliminating all the quantifiers
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, Simplified_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataOTFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataOTFp, "\t%d\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMILayer1TimeLocal, LMILayer2TimeLocal);
          
          //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
       }// if Layer1 is successful in eliminating all the quantifiers
      
      
 //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
 //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
 //cout<<"\nResult of Layer1 = "<<getDAGStr(Simplified_Bound_Part)<<endl;      
      
 // Call Layer2: Conversion to ILA + Omega Test
 
 DAGNode* QFree_Bound_Part;
 DAGNode* Quantified_Bound_Part;
      
 // Simplified_Bound_Part is the conjunction of 
 // QFree_Bound_Part and Quantified_Bound_Part
 // Former quantifier free and the latter quantified
 // First let us split Simplified_Bound_Part into QFree_Bound_Part and Quantified_Bound_Part
      
 convertToBoundAndFreeParts(Simplified_Bound_Part, VarsToElim, QFree_Bound_Part, Quantified_Bound_Part, WidthTable);
 //cout<<"\nQFree_Bound_Part in result of Layer1 = "<<getDAGStr(QFree_Bound_Part)<<endl;
 //cout<<"\nQuantified_Bound_Part in result of Layer1 = "<<getDAGStr(Quantified_Bound_Part)<<endl;
      
 set<string> Quantified_Bound_Part_Support;
 set<string> CommonVars;
    
 getDAGNames(Quantified_Bound_Part, Quantified_Bound_Part_Support);
 set_intersection(VarsToElim.begin(), VarsToElim.end(), Quantified_Bound_Part_Support.begin(), Quantified_Bound_Part_Support.end(),inserter(CommonVars, CommonVars.begin()));

 // Get the list of variables,  NoVars, NoVarsToElim
 list<string> VarNames;
 attachSetToList(CommonVars, VarNames);
 int OTNoVarsToElim = CommonVars.size();
 attachSetToList(Quantified_Bound_Part_Support, VarNames);     
            
 int OTBits = 0;//Max. width of any variable in the Quantified_Bound_Part
            
 for(set<string>::iterator it=Quantified_Bound_Part_Support.begin();  it!=Quantified_Bound_Part_Support.end(); it++)
                {
                string MyVariable = *it;
                map<string, int>::iterator wit = WidthTable.find(MyVariable);
                if(wit == WidthTable.end())
                    {
                    cout<<"\nVariable "<<MyVariable<<" has no entry in WidthTable in function Project_OT in DAGEliminator.cpp\n";
                    exit(1);
                    }
                
                if(OTBits < wit->second)
                    {
                    OTBits = wit->second;
                    }        
                }//for
            
   ULINT OTMod = findPower(OTBits);            
            
   //cout<<"\nBefore calling Omega Test\n";
   //cout<<"\nQuantified_Bound_Part = "<<getDAGStr(Quantified_Bound_Part)<<endl;
   //cout<<"\nOTNoVarsToElim = "<<OTNoVarsToElim<<endl;
   //cout<<"\nOTMod = "<<OTMod<<endl;
   //showList(VarNames, "VarNames");
   
   // Only the call is important
   
  struct timeval layer2_start_ms, layer2_finish_ms;                  
  gettimeofday (&layer2_start_ms, NULL); 
   
  DAGNode* TempDAG = qEliminator_ILP_ReturningDAG_WithLMISupport(Quantified_Bound_Part, OTNoVarsToElim, OTMod, VarNames, WidthTable);
 
  gettimeofday (&layer2_finish_ms, NULL);
  LMILayer2TimeLocal = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
  LMILayer2TimeLocal -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;    
      
  
  fprintf(LMIQEImportantDataOTFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
  fprintf(LMIQEImportantDataOTFp,  "\t%d\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMILayer1TimeLocal, LMILayer2TimeLocal);
  
  
}// end of function



// Variant of Project_OT which applies Omega Test after Layer2
DAGNode* applyOTAfterLayer2(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
// Local versions of the conjunction-level data start here
int PresentConjunctionNumberLocal=0;
int LMINoVarsToElimLocal=0;
int LMINoLMIsLocal=0;
int LMINoLMEsLocal=0;
int LMINoLMDsLocal=0;
int LMINoVarsLocal=0;
unsigned long long int LMITotalBitsLocal=0;
unsigned long long int LMIBitsToElimLocal=0;
int LMIVarsInputToLayer2Local=0;
int LMIVarsInputToLayer3Local=0;
int DummyLayerVarsInput=0;
unsigned long long int LMILayer1TimeLocal=0;
unsigned long long int LMILayer2TimeLocal=0;
unsigned long long int LMILayer3TimeLocal=0;
unsigned long long int DummyLayerTimeLocal=0;
// Local versions of the conjunction-level data end here  

// Step 1: Convert strict LMIs to weak LMIs
//showAPVector("Original Constraints", Constraints);
 convertStrictLMIsToWeakLMIs_WithLMDESupport(Constraints, WidthTable, VarsToElim);
 convertLMDEsToProperForm(Constraints, WidthTable);
//showAPVector("Constraints after converting strict LMIs to weak LMIs and LMDEs to proper form", Constraints);
   
// Step 2: Synthesize LMEs from LMIs
   
  tryToSynthesizeLMEsFromLMIs(Constraints,  VarsToElim,  WidthTable);
 //showAPVector("Constraints after LME synthesis", Constraints);        
  
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showSet(VarsToElim, "VarsToElim");
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
 
  DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
  
  
  // get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
  // in the respective global variables
  getRelevantLMIDetails_With_Recursive_Splitting(Bound_Constraints, VarsToElim, WidthTable, LMINoVarsToElimLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMITotalBitsLocal, LMIBitsToElimLocal, PresentConjunctionNumberLocal);
  
  
  DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
            {
           Bound_Constraints_Vector.push_back(*it); 
            }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
  //Apply Layer1      
      
  DAGNode* Simplified_Bound_Part;
      
  if(!SwitchOffLayer1)
        {
          
         struct timeval layer1_start_ms, layer1_finish_ms;                  
         gettimeofday (&layer1_start_ms, NULL); 
    
        Simplified_Bound_Part = applyLMILayer1(Bound_Constraints, VarsToElim, WidthTable);
        
        gettimeofday (&layer1_finish_ms, NULL);
        LMILayer1TimeLocal = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
        LMILayer1TimeLocal -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
        }
 else
        {
        Simplified_Bound_Part =   getConjunctionFromSet(Bound_Constraints);
        }
      
 LMIVarsInputToLayer2Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part, VarsToElim);           
      
 if(LMIVarsInputToLayer2Local == 0) //Layer1 is successful in eliminating all the quantifiers
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, Simplified_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataOTAfterLayer2Fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataOTAfterLayer2Fp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, DummyLayerVarsInput, LMIVarsInputToLayer3Local, LMILayer1TimeLocal, LMILayer2TimeLocal, DummyLayerTimeLocal, LMILayer3TimeLocal);
          
          //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
       }// if Layer1 is successful in eliminating all the quantifiers
      
      
     //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
    //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
    //cout<<"\nResult of Layer1 = "<<getDAGStr(Simplified_Bound_Part)<<endl; 
 
    // Call Layer2
                  
      DAGNode* QFree_Bound_Part;
      DAGNode* Quantified_Bound_Part;
      
      // Simplified_Bound_Part is the conjunction of 
      // QFree_Bound_Part and Quantified_Bound_Part
      // Former quantifier free and the latter quantified
      // First let us split Simplified_Bound_Part into QFree_Bound_Part and Quantified_Bound_Part
      
      convertToBoundAndFreeParts(Simplified_Bound_Part, VarsToElim, QFree_Bound_Part, Quantified_Bound_Part, WidthTable);
      //cout<<"\nQFree_Bound_Part in result of Layer1 = "<<getDAGStr(QFree_Bound_Part)<<endl;
      //cout<<"\nQuantified_Bound_Part in result of Layer1 = "<<getDAGStr(Quantified_Bound_Part)<<endl;
      
      // Apply Layer2
      
      // First split the conjunction Quantified_Bound_Part into set of atomic predicates
      set<DAGNode*>  Quantified_Bound_Part_Constraint_Set;
      splitConjunctionIntoSetGeneral(Quantified_Bound_Part, Quantified_Bound_Part_Constraint_Set);
      
      
      DAGNode* Simplified_Bound_Part_After_Layer2;
      if(!SwitchOffLayer2)
        {
          
         struct timeval layer2_start_ms, layer2_finish_ms;                  
         gettimeofday (&layer2_start_ms, NULL); 
    
         Simplified_Bound_Part_After_Layer2 = applyLMILayer2Recursive(Quantified_Bound_Part_Constraint_Set, VarsToElim, WidthTable);
         //cout<<"\nSimplified_Bound_Part_After_Layer2 = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;
         
        Simplified_Bound_Part_After_Layer2 = singleLMEElimination(Simplified_Bound_Part_After_Layer2);
        //cout<<"\nSimplified_Bound_Part_After_Layer2 after singleLMEElimination = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;
        
        gettimeofday (&layer2_finish_ms, NULL);
        LMILayer2TimeLocal = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
        LMILayer2TimeLocal -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;    
        }
      else
        {
        Simplified_Bound_Part_After_Layer2 =   getConjunctionFromSet(Quantified_Bound_Part_Constraint_Set);
        }
      
      LMIVarsInputToLayer3Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part_After_Layer2, VarsToElim);           
      
      if(LMIVarsInputToLayer3Local == 0) //Layer2 is successful in eliminating all the quantifiers
          // Effectively Layer2 did: \exists VarToElim.(Quantified_Bound_Part) \equiv True
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, QFree_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataOTAfterLayer2Fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataOTAfterLayer2Fp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, DummyLayerVarsInput, LMIVarsInputToLayer3Local, LMILayer1TimeLocal, LMILayer2TimeLocal, DummyLayerTimeLocal, LMILayer3TimeLocal);
                             
          //cout<<"\nLMILayer2TimeLocal = "<<LMILayer2TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer3Local = "<<LMIVarsInputToLayer3Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          return resultDAG;
       }// if of  Layer2 is successful in eliminating all the quantifiers
      
      
      //cout<<"\nLMILayer2TimeLocal = "<<LMILayer2TimeLocal<<endl;
      //cout<<"\nLMIVarsInputToLayer3Local = "<<LMIVarsInputToLayer3Local<<endl;
      //cout<<"\nResult of Layer2 = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;      
      
      //Calling Layer 3
      
        DAGNode *InputToLayer3 = Simplified_Bound_Part_After_Layer2;
            
      
        // Call Layer3: Conversion to ILA + Omega Test

        set<string> InputToLayer3_Support;
        set<string> CommonVars;

        getDAGNames(InputToLayer3, InputToLayer3_Support);
        set_intersection(VarsToElim.begin(), VarsToElim.end(), InputToLayer3_Support.begin(), InputToLayer3_Support.end(),inserter(CommonVars, CommonVars.begin()));

         // Get the list of variables,  NoVars, NoVarsToElim
        list<string> VarNames;
        attachSetToList(CommonVars, VarNames);
        int OTNoVarsToElim = CommonVars.size();
        attachSetToList(InputToLayer3_Support, VarNames);     

        int OTBits = 0;//Max. width of any variable in InputToLayer3

        for(set<string>::iterator it=InputToLayer3_Support.begin();  it!=InputToLayer3_Support.end(); it++)
                    {
                    string MyVariable = *it;
                    map<string, int>::iterator wit = WidthTable.find(MyVariable);
                    if(wit == WidthTable.end())
                        {
                        cout<<"\nVariable "<<MyVariable<<" has no entry in WidthTable in function applyOTAfterLayer2 in DAGEliminator.cpp\n";
                        exit(1);
                        }

                    if(OTBits < wit->second)
                        {
                        OTBits = wit->second;
                        }        
                    }//for

        ULINT OTMod = findPower(OTBits);            

       cout<<"\nBefore calling Omega Test\n";
       cout<<"\nInputToLayer3 = "<<getDAGStr(InputToLayer3)<<endl;
       cout<<"\nOTNoVarsToElim = "<<OTNoVarsToElim<<endl;
       cout<<"\nOTMod = "<<OTMod<<endl;
       showList(VarNames, "VarNames");

       // Only the call is important

      struct timeval layer3_start_ms, layer3_finish_ms;                  
      gettimeofday (&layer3_start_ms, NULL); 

      DAGNode* TempDAG = qEliminator_ILP_ReturningDAG_WithLMISupport(InputToLayer3, OTNoVarsToElim, OTMod, VarNames, WidthTable);

      gettimeofday (&layer3_finish_ms, NULL);
      LMILayer3TimeLocal = layer3_finish_ms.tv_sec * 1000 + layer3_finish_ms.tv_usec / 1000;
      LMILayer3TimeLocal -= layer3_start_ms.tv_sec * 1000 + layer3_start_ms.tv_usec / 1000;    


      fprintf(LMIQEImportantDataOTAfterLayer2Fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
      
      DummyLayerVarsInput = LMIVarsInputToLayer3Local;
      
      fprintf(LMIQEImportantDataOTAfterLayer2Fp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, DummyLayerVarsInput, LMIVarsInputToLayer3Local, LMILayer1TimeLocal, LMILayer2TimeLocal, DummyLayerTimeLocal, LMILayer3TimeLocal); 
  
}// end of function



// Tries to find \exists VarsToElim. (formula) in the following way:
// 1) If VarsToElim = support(formula), then if formula is unsat then \exists VarsToElim. (formula) is unsat, and
//     if formula is sat then \exists VarsToElim. (formula) is valid.
// 2) Else check if \exists VarsToElim. (formula) is valid.
string simpleSMTChecks(DAGNode* formula, set<string> &VarsToElim, map<string, int> &WidthTable)
{
     set<string> VarsToRemain;     
     getVariablesToRemain(formula, VarsToElim,  VarsToRemain);
     
     //showSet(VarsToElim, "VarsToElim");     
     //showSet(VarsToRemain, "VarsToRemain");
    
     if(VarsToRemain.empty())
     {
        cout<<"\nVarsToRemain is empty\n"; 
         
        map<string, string> TempModel;
        
        bool ConditionIsFalse = getModelGeneral_WithSupportForLMIs(formula, TempModel, WidthTable);
        
                       
        if(ConditionIsFalse)
            {
            cout<<"\nexists X. (formula) is unsat\n";            
            return "unsat";
            }
        else
            {
            cout<<"\nexists X. (formula) is valid\n";            
            return "valid";
            }
     }//if(VarsToRemain.empty())
     else
     {
        cout<<"\nVarsToRemain is non-empty\n";  
         
        DAGNode* NegPhi = createDAG("not", formula, LOGICAL_SYMBOL, 1); // NegPhi = \neg formula
             
        map<string, string> YMap;   // Model of forall VarsToElim. \neg Phi     
     
        string result_from_z3;
     
        result_from_z3 = getModelForQBVF_UsingZ3_WithSupportForLMIs(NegPhi, YMap,  WidthTable,  VarsToElim,  VarsToRemain);
        
                         
        if(result_from_z3 == "unsat") 
            {
              cout<<"\nexists X. (formula) is valid\n";            
              return "valid";  
            }
        else
            {
             cout<<"\nNot clear if exists X. (formula) is valid/unsat\n";            
             return result_from_z3;
            }
     }//if(!VarsToRemain.empty())
}//end of function


// Checks if \exists VarToElim. (Phi /\ Xi) \equiv \exists VarToElim. (Phi) using BDDs
bool isExtForLMDsAndLMIsUsingBDDs(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, map<string, int> &WidthTable, unsigned long long int &BlastTimeLocal, unsigned long long int &DDTimeLocal, unsigned long long int &QETimeLocal)
{
    DAGNode* PhiDag = getConjunctionFromSet(Phi);    
    DAGNode* XiDag = getConjunctionFromSet(Xi);
    
    bool print_to_Layer2ChecksUsingBDDsOutputData = false;
    
    if(print_to_Layer2ChecksUsingBDDsOutputData)
        {
        OutputDataLayer2ChecksFp = fopen("Layer2ChecksUsingBDDsOutputData.txt", "a+");
        if(OutputDataLayer2ChecksFp == NULL)
                {
                    cout<<"\nCould not open Layer2ChecksUsingBDDsOutputData.txt\n";
                    exit(1);
                }
    
        fprintf(OutputDataLayer2ChecksFp, "\nVarToElim = %s\n", VarToElim.c_str());
        fprintf(OutputDataLayer2ChecksFp, "\nPhiDag = %s\n", getDAGStr(PhiDag).c_str());
        fprintf(OutputDataLayer2ChecksFp, "\nXiDag = %s\n", getDAGStr(XiDag).c_str());
        fclose(OutputDataLayer2ChecksFp);
      }
    
    if(ddm.quantifiedFormulaeAreEquivalentUsingBlastingAndCudd(PhiDag, XiDag, VarToElim, WidthTable, BlastTimeLocal, DDTimeLocal, QETimeLocal))
    {
        return true;
    }
    else
    {
        return false;
    }
}
