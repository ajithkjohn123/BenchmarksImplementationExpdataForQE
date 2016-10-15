/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#ifndef _ILP_ELIMINATOR_H
#define	_ILP_ELIMINATOR_H

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
#include "Eliminator.h"
#include "helping_functions.h"


using namespace std;

extern vector<DAGNode*> root;
extern DAGManager dm;


// Given \exists X. (E /\ D), perform QE using ILP
// Always NULL is returned
DAGNode* qEliminator_ILP_ReturningDAG(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, list<string> VarNames, map<string, int> WidthTable);



// Given "node" which points to the root of a DAG, get OperatorTable,
// which associates a label name with each DAG node
void getOperatorTable(DAGNode *node, map<DAGNode*, string> &OperatorTable);


// Function internally called by getOperatorTable
void getOperatorTableRecursively(DAGNode *node, unsigned long long int &sum_count, unsigned long long int &product_count, map<DAGNode*, string> &OperatorTable);


// Write the preclude of the omega test input file "ot_input"
void writePreludeOfOmegaTest(FILE *ot_input, int NoVarsToElim, list<string> VarNames, map<string, int> WidthTable, map<DAGNode*, string> OperatorTable);


// Write the details of the DAG node "node" in the omega test input file "ot_input"
void getOmegaTestInput(DAGNode *node, map<DAGNode*, string> OperatorTable, FILE *ot_input, ULINT Mod);


// Function internally called by getOmegaTestInput
// The string which is returned is used internally, but not by the caller getOmegaTestInput
string getOmegaTestInputRecursively(DAGNode *node, map<DAGNode*, string> OperatorTable, FILE *ot_input, HashTable <string, string> *PtrTableForOmegaTest, ULINT Mod);



// Write the conclusion of the omega test input file "ot_input"
void writeConclusionOfOmegaTest(FILE *ot_input, map<string, int> WidthTable, list<string> VarNames);

// Given \exists X. (E /\ D /\ I), perform QE using ILP
// Always NULL is returned
DAGNode* qEliminator_ILP_ReturningDAG_WithLMISupport(DAGNode* InputDag, int NoVarsToElim, ULINT Mod, list<string> &VarNames, map<string, int> &WidthTable);


// Version of writePreludeOfOmegaTest with no passing by value
void writePreludeOfOmegaTestWithNoPassingByValue(FILE *ot_input, int NoVarsToElim, list<string> &VarNames, map<string, int> &WidthTable, map<DAGNode*, string> &OperatorTable);


// Version of writeConclusionOfOmegaTest with no passing by value
void writeConclusionOfOmegaTestWithNoPassingByValue(FILE *ot_input, map<string, int> &WidthTable, list<string> &VarNames);


// Version of getOmegaTestInput with support for LMIs
void getOmegaTestInput_WithLMISupport(DAGNode *node, map<DAGNode*, string> &OperatorTable, FILE *ot_input, ULINT Mod);


// Function internally called by getOmegaTestInput_WithLMISupport
string getOmegaTestInput_WithLMISupportRecursively(DAGNode *node, map<DAGNode*, string> &OperatorTable, FILE *ot_input, HashTable <string, string> *PtrTableForOmegaTest, ULINT Mod);



#endif	/* _ILP_ELIMINATOR_H */
