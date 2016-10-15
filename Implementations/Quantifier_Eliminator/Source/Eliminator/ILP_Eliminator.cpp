/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#include "ILP_Eliminator.h"

// Given \exists X. (E /\ D), perform QE using ILP
// Always NULL is returned
DAGNode* qEliminator_ILP_ReturningDAG(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, list<string> VarNames, map<string, int> WidthTable)
{
  // Delete the duplicate contraints

  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  //cout<<"\nShowing original set of constraints\n";
  //showEquations(EqCoefMatPassed, EqConstMatPassed); // Show the equations
  //showDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed); // Show the disequations


  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  
  //cout<<"\nShowing duplicate eliminated set of constraints\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations


  // Create DAG for /\of LMEs and /\of LMDs
  
  DAGNode *DagFromEqs, *DagFromDiseqs;
  DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, VarNames, Mod);
  DagFromDiseqs = getDAGFromDisEqs(DisEqCoefMat, DisEqConstMat, VarNames, Mod);

  DAGNode *InputDag;
  if(DagFromEqs != NULL && DagFromDiseqs != NULL)
    InputDag = createDAG("and", DagFromEqs, DagFromDiseqs, LOGICAL_SYMBOL, 1);

  else if(DagFromEqs != NULL && DagFromDiseqs == NULL)
    InputDag = DagFromEqs;

  else if(DagFromEqs == NULL && DagFromDiseqs != NULL)
    InputDag = DagFromDiseqs;
  
  else
    {
      cout<<"\nEmpty matrices passed to function qEliminator_ILP_ReturningDAG\n";
      exit(1);
    }

  // For debugging
  //ofstream *outfile = new ofstream();
  //outfile->open("debug_ilp.txt");
  //*outfile<<"\nOriginal DAG\n";
  //dm.printDAG(InputDag, outfile);
  // For debugging ends here

  FILE *ot_input = fopen("omega_input.txt", "w");
  if(ot_input == NULL)
    {
      cout<<"\nCould not create file omega_input.txt for writing\n";
    }
  
  // label the bvadd, bvmul nodes
  map<DAGNode*, string> OperatorTable;
  getOperatorTable(InputDag, OperatorTable);

  //cout<<"\nLabel Table\n";
  map<DAGNode*, string>::iterator OperatorTable_it = OperatorTable.begin();
  for(;OperatorTable_it != OperatorTable.end();OperatorTable_it++)
    {
      if((*OperatorTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
      if((*OperatorTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
      //cout<<"\n"<<(*OperatorTable_it).first<<"\t"<<((*OperatorTable_it).first)->Get_node_name()<<"\t"<<(*OperatorTable_it).second; 
    }
  //cout<<"\n";	

  writePreludeOfOmegaTest(ot_input, NoVarsToElim, VarNames, WidthTable, OperatorTable);
  getOmegaTestInput(InputDag, OperatorTable, ot_input, Mod);
  writeConclusionOfOmegaTest(ot_input, WidthTable, VarNames);
  fclose(ot_input);

  cout<<"\nExecuting oc omega_input.txt\n";

  system("oc omega_input.txt");

  return NULL;
}


// Given "node" which points to the root of a DAG, get OperatorTable,
// which associates a label name with each DAG node
void getOperatorTable(DAGNode *node, map<DAGNode*, string> &OperatorTable)
{

  unsigned long long int sum_count = 1;
  unsigned long long int product_count = 1;
 
  bool reset_visited = dm.ResetVisitedFlagOfDAGNodes();

  if(reset_visited == false)
    {
      cout<<"\nCould not reset visited flag of all DAG nodes\n";
      exit(1);
    }

  getOperatorTableRecursively(node, sum_count, product_count, OperatorTable);
}

// Function internally called by getOperatorTable
void getOperatorTableRecursively(DAGNode *node, unsigned long long int &sum_count, unsigned long long int &product_count, map<DAGNode*, string> &OperatorTable)
{
  string MyName = node->Get_node_name();

  if(node->Get_visited_flag() == true)
    return;

  node->Set_visited_flag();
 
  string Label;
  char count[100];

  if(MyName == "bvadd")
    {
      sprintf(count,"%llu",sum_count);
      Label = "s_";
      Label += count;
      sum_count++;
      OperatorTable.insert (pair<DAGNode*, string>(node, Label));
    }
  else if(MyName == "bvmul")
    {
      sprintf(count,"%llu",product_count);
      Label = "p_";
      Label += count;
      product_count++;
      OperatorTable.insert (pair<DAGNode*, string>(node, Label));
    }


  list<DAGNode*>::iterator it = node->Get_outlist_start(); 
  for(unsigned int i=0; i<node->Get_outlist_size(); i++, it++)
    {
      getOperatorTableRecursively(*it, sum_count, product_count, OperatorTable);
    }
}

// Write the preclude of the omega test input file "ot_input"
void writePreludeOfOmegaTest(FILE *ot_input, int NoVarsToElim, list<string> VarNames, map<string, int> WidthTable, map<DAGNode*, string> OperatorTable)
{
  fprintf(ot_input, "{");
  fprintf(ot_input, "[");
  
  int count=0;
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++, count++)
    {
      if(count==VarNames.size()-1) // *it is the last variable which is not to be eliminated : no comma
	{
	  fprintf(ot_input, "%s", (*it).c_str());
	}
      else if(count>=NoVarsToElim) // *it is a variable which is not to be eliminated
	{
	 fprintf(ot_input, "%s", (*it).c_str());
	 fprintf(ot_input, ", ");
	}
    }
   fprintf(ot_input, "] : exists(");

   for(map<DAGNode*, string>::iterator it=OperatorTable.begin(); it!=OperatorTable.end(); it++)
     {
       fprintf(ot_input, "%s", (it->second).c_str());
       fprintf(ot_input, ", ");
     }

  count=0;
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++, count++)
    {
      if(count==NoVarsToElim-1) // *it is the last variable which is to be eliminated : no comma
	{
	  fprintf(ot_input, "%s", (*it).c_str());
	}
      else if(count<NoVarsToElim) // *it is a variable which is to be eliminated
	{
	 fprintf(ot_input, "%s", (*it).c_str());
	 fprintf(ot_input, ", ");
	}
    }
   
  fprintf(ot_input, " : ");

}
  

// Write the conclusion of the omega test input file "ot_input"
void writeConclusionOfOmegaTest(FILE *ot_input, map<string, int> WidthTable, list<string> VarNames)
{
  int count=0;
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++, count++)
    {
      string name = *it;
      int width = WidthTable[name];
      ULINT maximum = findPower(width)-1;
      int minimum = 0;
      fprintf(ot_input, "\n\n%s >= %d && %s <= %llu", name.c_str(), minimum, name.c_str(), maximum);

      if(count!=VarNames.size()-1) fprintf(ot_input, " && ");
    }
  fprintf(ot_input, ")}; ");
  
}
  

// Write the details of the DAG node "node" in the omega test input file "ot_input"
void getOmegaTestInput(DAGNode *node, map<DAGNode*, string> OperatorTable, FILE *ot_input, ULINT Mod)
{

  bool reset_visited = dm.ResetVisitedFlagOfDAGNodes();
  if(reset_visited == false)
    {
      cout<<"\nCould not reset visited flag of all dag nodes\n";
      exit(1);
    }
  
  HashTable <string, string> TableForOmegaTest;
  HashTable <string, string> *PtrTableForOmegaTest = &TableForOmegaTest;
  string ret_value = getOmegaTestInputRecursively(node, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
}



// Function internally called by getOmegaTestInput
// The string which is returned is used internally, but not by the caller getOmegaTestInput
string getOmegaTestInputRecursively(DAGNode *node, map<DAGNode*, string> OperatorTable, FILE *ot_input, HashTable <string, string> *PtrTableForOmegaTest, ULINT Mod)
{
  if(node->Get_visited_flag() == true)
	{  
        // code for memoization can be added here
	// Creating the hash key

	  string key;
	  char temp_char[10];

	  sprintf(temp_char , "%x", node);
	  key = temp_char;
	  
	  HTStatusValue<string> result = PtrTableForOmegaTest->hashtable_search(key);
	  if(result.success())
	    {
	      return result.GetValue();
	    }
	  else
	    {
	      cout<<"\nnode was visited but, no entry in function getOmegaTestInputRecursively\n";
	      exit(1);
	    }
	}


  string ret_value;

  node->Set_visited_flag();

  if(node->Get_outlist_size()==0) // leaf level
    {
      ret_value = node->Get_node_name();
    }

  else
  {
  string MyName = node->Get_node_name();

  if(MyName == "bvmul") 
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);

      ULINT constant = strtoull(child1_name.c_str(), NULL, 0);
      string label = OperatorTable[node];

      char Mod_char[100];
      sprintf(Mod_char , "%llu", Mod);
      string Mod_string = Mod_char;

      sprintf(Mod_char , "%llu", Mod-1);
      string Mod_dec_string = Mod_char;

      char Range_char[100];
      sprintf(Range_char , "%llu", Mod-1);
      string Range_string = Range_char;
      
      // Print to file
      
      if(child1_name == "0")
	{
	  ret_value="0";
	}
      else
	{
	  if(child1_name == "1") child1_name=""; // multiplication by 1

	  ret_value=child1_name;
	  ret_value+=child2_name;
	  ret_value+="-";
	  ret_value+=Mod_string;
	  ret_value+=label;
	}

      fprintf(ot_input, "\n\n%s <= %s && %s <= %s && ", ret_value.c_str(), Mod_dec_string.c_str(), label.c_str(), Range_string.c_str());
    }

  else if(MyName == "bvadd") 
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);

      string label = OperatorTable[node];

      char Mod_char[100];
      sprintf(Mod_char , "%llu", Mod);
      string Mod_string = Mod_char;

      sprintf(Mod_char , "%llu", Mod-1);
      string Mod_dec_string = Mod_char;

      string sum;
      sum = child1_name;
      sum += "+";
      sum += child2_name; 

      string corrected_sum;
      corrected_sum = sum;
      corrected_sum += "-";
      corrected_sum += Mod_string;

      // Print to file

      ret_value=label;
      fprintf(ot_input, "\n\n((%s <= %s && %s = %s) || (%s > %s && %s = %s)) && ", sum.c_str(), Mod_dec_string.c_str(), ret_value.c_str(), sum.c_str(), sum.c_str(), Mod_dec_string.c_str(), ret_value.c_str(), corrected_sum.c_str());
    }

  else if(MyName == "=") 
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s = %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }

  else if(MyName == "is_not_equal") 
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s != %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }

  else if(MyName == "and") 
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInputRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      ret_value="";
    }

  else 
    {
      cout<<"\nUnknown operator "<<MyName<<" inside function getOmegaTestInputRecursively\n";
      exit(1);
    }
  }
      
  
  // Inserting into the hashtable
  // Creating the hash key

  string key;
  char temp_char[10];

  sprintf(temp_char , "%x", node);
  key = temp_char;
	  
  HTStatusValue<string> result = PtrTableForOmegaTest->hashtable_search(key);
  
  if(result.success())
    {
      cout<<"\nInside function getOmegaTestInputRecursively, entry already exists for this node. Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = PtrTableForOmegaTest->hashtable_insert(key, ret_value);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function getOmegaTestInputRecursively\n";
      exit(1);
    }

  return ret_value; 

}



// Versions of the functions above modified s.t.
// LMIs are handled, No passing by value/matrix

// Given \exists X. (E /\ D /\ I), perform QE using ILP
// Always NULL is returned
DAGNode* qEliminator_ILP_ReturningDAG_WithLMISupport(DAGNode* InputDag, int NoVarsToElim, ULINT Mod, list<string> &VarNames, map<string, int> &WidthTable)
{ 
  FILE *ot_input = fopen("omega_input.txt", "w");
  if(ot_input == NULL)
    {
      cout<<"\nCould not create file omega_input.txt for writing\n";
    }
  
  // label the bvadd, bvmul nodes
  map<DAGNode*, string> OperatorTable;
  getOperatorTable(InputDag, OperatorTable);

  //cout<<"\nLabel Table\n";
  map<DAGNode*, string>::iterator OperatorTable_it = OperatorTable.begin();
  for(;OperatorTable_it != OperatorTable.end();OperatorTable_it++)
    {
      if((*OperatorTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
      if((*OperatorTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
      //cout<<"\n"<<(*OperatorTable_it).first<<"\t"<<((*OperatorTable_it).first)->Get_node_name()<<"\t"<<(*OperatorTable_it).second; 
    }
  //cout<<"\n";	

  writePreludeOfOmegaTestWithNoPassingByValue(ot_input, NoVarsToElim, VarNames, WidthTable, OperatorTable);
  getOmegaTestInput_WithLMISupport(InputDag, OperatorTable, ot_input, Mod);
  writeConclusionOfOmegaTestWithNoPassingByValue(ot_input, WidthTable, VarNames);
  fclose(ot_input);
  
  cout<<"\nWriting to Omega Test file completed\n";
  //char c = getchar();

  cout<<"\nExecuting oc omega_input.txt\n";

  system("oc omega_input.txt");
  
  //c = getchar();

  return NULL;
}



// Version of writePreludeOfOmegaTest with no passing by value
void writePreludeOfOmegaTestWithNoPassingByValue(FILE *ot_input, int NoVarsToElim, list<string> &VarNames, map<string, int> &WidthTable, map<DAGNode*, string> &OperatorTable)
{
  fprintf(ot_input, "{");
  fprintf(ot_input, "[");
  
  int count=0;
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++, count++)
    {
      if(count==VarNames.size()-1) // *it is the last variable which is not to be eliminated : no comma
	{
	  fprintf(ot_input, "%s", (*it).c_str());
	}
      else if(count>=NoVarsToElim) // *it is a variable which is not to be eliminated
	{
	 fprintf(ot_input, "%s", (*it).c_str());
	 fprintf(ot_input, ", ");
	}
    }
   fprintf(ot_input, "] : exists(");

   for(map<DAGNode*, string>::iterator it=OperatorTable.begin(); it!=OperatorTable.end(); it++)
     {
       fprintf(ot_input, "%s", (it->second).c_str());
       fprintf(ot_input, ", ");
     }

  count=0;
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++, count++)
    {
      if(count==NoVarsToElim-1) // *it is the last variable which is to be eliminated : no comma
	{
	  fprintf(ot_input, "%s", (*it).c_str());
	}
      else if(count<NoVarsToElim) // *it is a variable which is to be eliminated
	{
	 fprintf(ot_input, "%s", (*it).c_str());
	 fprintf(ot_input, ", ");
	}
    }
   
  fprintf(ot_input, " : ");

}
  

// Version of writeConclusionOfOmegaTest with no passing by value
void writeConclusionOfOmegaTestWithNoPassingByValue(FILE *ot_input, map<string, int> &WidthTable, list<string> &VarNames)
{
  int count=0;
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++, count++)
    {
      string name = *it;
      int width = WidthTable[name];
      ULINT maximum = findPower(width)-1;
      int minimum = 0;
      fprintf(ot_input, "\n\n%s >= %d && %s <= %llu", name.c_str(), minimum, name.c_str(), maximum);

      if(count!=VarNames.size()-1) fprintf(ot_input, " && ");
    }
  fprintf(ot_input, ")}; ");
  
}
  

// Version of getOmegaTestInput with support for LMIs
void getOmegaTestInput_WithLMISupport(DAGNode *node, map<DAGNode*, string> &OperatorTable, FILE *ot_input, ULINT Mod)
{

  bool reset_visited = dm.ResetVisitedFlagOfDAGNodes();
  if(reset_visited == false)
    {
      cout<<"\nCould not reset visited flag of all dag nodes in function getOmegaTestInput_WithLMISupport in ILP_Eliminator.cpp\n";
      exit(1);
    }
  
  HashTable <string, string> TableForOmegaTest;
  HashTable <string, string> *PtrTableForOmegaTest = &TableForOmegaTest;
  string ret_value = getOmegaTestInput_WithLMISupportRecursively(node, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
}



// Function internally called by getOmegaTestInput_WithLMISupport
string getOmegaTestInput_WithLMISupportRecursively(DAGNode *node, map<DAGNode*, string> &OperatorTable, FILE *ot_input, HashTable <string, string> *PtrTableForOmegaTest, ULINT Mod)
{
  if(node->Get_visited_flag() == true)
	{  
        // code for memoization can be added here
	// Creating the hash key

	  string key;
	  char temp_char[10];

	  sprintf(temp_char , "%x", node);
	  key = temp_char;
	  
	  HTStatusValue<string> result = PtrTableForOmegaTest->hashtable_search(key);
	  if(result.success())
	    {
	      return result.GetValue();
	    }
	  else
	    {
	      cout<<"\nnode was visited but, no entry in function getOmegaTestInput_WithLMISupportRecursively in ILP_Eliminator.cpp\n";
	      exit(1);
	    }
	}


  string ret_value;

  node->Set_visited_flag();

  if(node->Get_outlist_size()==0) // leaf level
    {
      ret_value = node->Get_node_name();
    }

  else
  {
  string MyName = node->Get_node_name();

  if(MyName == "bvmul") 
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);

      ULINT constant = strtoull(child1_name.c_str(), NULL, 0);
      string label = OperatorTable[node];

      char Mod_char[100];
      sprintf(Mod_char , "%llu", Mod);
      string Mod_string = Mod_char;

      sprintf(Mod_char , "%llu", Mod-1);
      string Mod_dec_string = Mod_char;

      char Range_char[100];
      sprintf(Range_char , "%llu", Mod-1);
      string Range_string = Range_char;
      
      // Print to file
      
      if(child1_name == "0")
	{
	  ret_value="0";
	}
      else
	{
	  if(child1_name == "1") child1_name=""; // multiplication by 1

	  ret_value=child1_name;
	  ret_value+=child2_name;
	  ret_value+="-";
	  ret_value+=Mod_string;
	  ret_value+=label;
	}

      fprintf(ot_input, "\n\n%s <= %s && %s <= %s && ", ret_value.c_str(), Mod_dec_string.c_str(), label.c_str(), Range_string.c_str());
    }

  else if(MyName == "bvadd") 
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);

      string label = OperatorTable[node];

      char Mod_char[100];
      sprintf(Mod_char , "%llu", Mod);
      string Mod_string = Mod_char;

      sprintf(Mod_char , "%llu", Mod-1);
      string Mod_dec_string = Mod_char;

      string sum;
      sum = child1_name;
      sum += "+";
      sum += child2_name; 

      string corrected_sum;
      corrected_sum = sum;
      corrected_sum += "-";
      corrected_sum += Mod_string;

      // Print to file

      ret_value=label;
      fprintf(ot_input, "\n\n((%s <= %s && %s = %s) || (%s > %s && %s = %s)) && ", sum.c_str(), Mod_dec_string.c_str(), ret_value.c_str(), sum.c_str(), sum.c_str(), Mod_dec_string.c_str(), ret_value.c_str(), corrected_sum.c_str());
    }
  else if(MyName == "bvule")
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s <= %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }
  else if(MyName == "bvuge")
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s >= %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }
  else if(MyName == "bvult")
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s < %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }
  else if(MyName == "bvugt")
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s > %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }
  else if(MyName == "=") 
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s = %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }

  else if(MyName == "is_not_equal") 
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      
      
      // Print to file
      fprintf(ot_input, "\n\n%s != %s && ", child1_name.c_str(), child2_name.c_str());
      ret_value="";
    }

  else if(MyName == "and") 
    {
      
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      string child1_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      it++;
      string child2_name = getOmegaTestInput_WithLMISupportRecursively(*it, OperatorTable, ot_input, PtrTableForOmegaTest, Mod);
      ret_value="";
    }

  else 
    {
      cout<<"\nUnknown operator "<<MyName<<" inside function getOmegaTestInput_WithLMISupportRecursively in ILP_Eliminator.cpp\n";
      exit(1);
    }
  }
      
  
  // Inserting into the hashtable
  // Creating the hash key

  string key;
  char temp_char[10];

  sprintf(temp_char , "%x", node);
  key = temp_char;
	  
  HTStatusValue<string> result = PtrTableForOmegaTest->hashtable_search(key);
  
  if(result.success())
    {
      cout<<"\nInside function getOmegaTestInput_WithLMISupportRecursively in ILP_Eliminator.cpp, entry already exists for this node. Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = PtrTableForOmegaTest->hashtable_insert(key, ret_value);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function getOmegaTestInput_WithLMISupportRecursively in ILP_Eliminator.cpp\n";
      exit(1);
    }

  return ret_value; 

}








