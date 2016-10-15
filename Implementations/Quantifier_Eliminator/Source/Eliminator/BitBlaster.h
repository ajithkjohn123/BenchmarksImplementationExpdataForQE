/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/




#ifndef _BITBLASTER_H
#define	_BITBLASTER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
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

// Declarations for LMDD:BDD
extern int bdd_index;
extern map<string, int> NameIndexMap;
extern map<int, string> IndexNameMap;
// Declarations for LMDD:BDD end here


extern map<DAGNode*, DAGNode*> BlastedDAGToOriginalDAGMapForPredicates; // Maps a dag node in 
// the blasted dag corresponding to a predicate in the original dag to the dag node for the predicate


// BASIC FUNCTIONS TO PERFORM BIT-BLASTING


// Internal function called by the function getBitBlastedDAG in BitBlaster.cc
// Given a bit-vector formula rooted at "node", returns the bits of the propositional
// logic formual obtained by blasting "node", as a vector<DAGNode *>
vector<DAGNode *> getBitBlastedDAGRecursively(DAGNode * node);



// Given a bit-vector formula, return the proposotional
// formula obtained by blasting it
DAGNode* getBitBlastedDAG(DAGNode *node);



// Get label table which gives unique labels to the nodes in the 
// dag for the propositional logic formula rooted at "node".
void getLabelTable(DAGNode *node, map<DAGNode*, string> &LabelTable);



// Internal function called by the function getLabelTable
void getLabelTableRecursively(DAGNode *node, unsigned long long int &label_count, map<DAGNode*, string> &LabelTable);




// Get CNF for the propositional logic formula rooted at "node" 
// in terms of the labels in the "LabelTable"
void getCNF(DAGNode *node, map<DAGNode*, string> &LabelTable, set<string> &CNF);



// Internal function called by function getCNFRecursively
void getCNFRecursively(DAGNode *node, map<DAGNode*, string> &LabelTable, set<string> &CNF);




// This function checks if \phi' \and \forall VarSetToElim.(\neg \phi) where 
// \phi' \equiv \exists VarSetToElim.(\phi). 
// "DAG_to_blast" here represents \phi' \and (\neg \phi), 
// returns true if \phi' \and \forall VarSetToElim.(\neg \phi); false otherwise
bool checkForExactnessInOtherDirection(DAGNode *DAG_to_blast, set<string> VarSetToElim, set<string> VarSetRemaining, map<string, int> WidthTable);





// Call QBF solver to solve \exists ExQuantifiedVars1. \forall 
// UniversallyQuantifiedVars. \exists ExQuantifiedVars2.CNF where CNF 
// represents the equisatisfiable CNF for some propositional logic formula
// return true if unsat; false if sat 
bool checkQBFUnsat(set<string> &CNF, int NumVars, set<string> &ExQuantifiedVars1, set<string> &UniversallyQuantifiedVars, set<string> &ExQuantifiedVars2);



// returns 1 if "BitsChild1" is a vector containing true/false;
// returns 2 if "BitsChild2" is a vector containing true/false;
// returns 0 otherwise.
int getConstantArray(vector<DAGNode*> BitsChild1, vector<DAGNode*> BitsChild2);


// Show the given vector of DAGNode*
void showVector(vector<DAGNode*> MyVector, string WhoAmI);


// Get the operands of the addition in "OperandsOfAddition"
void getOperandsOfAddition(vector <DAGNode*> Variable, vector <DAGNode*> Constant, vector< vector <DAGNode*> > &OperandsOfAddition);


// Return the sum of the vectors "BitsChild1" and "BitsChild2"
vector <DAGNode*> getSumOfArrays(vector <DAGNode*> &BitsChild1, vector <DAGNode*> &BitsChild2);




// Check if "BitBlastedDAG" is the correct bit-blasted version of "DAG_to_blast"
// return true if exact; false otherwise
bool checkForExactnessOfBlasting(DAGNode* DAG_to_blast, map<string, int> &WidthTable, DAGNode *BitBlastedDAG);



/* The following two functions take a dag A, bit-blast it to get B. Now given exists(X, A) in bv-level, we have exists({x1,...,xt}, B(x1,...,xn)) in bit-level. Now convert B to equisatisfiable CNF with internal labels l1,...,lm. So exists({x1,...,xt}, B(x1,...,xn)) <=> exists({x1,...,xt,l1,...,lm}, B'(x1,...,xn,l1,...,lm)) where B' is the eq.sat transformation of B. Now print exists({x1,...,xt,l1,...,lm}, B'(x1,...,xn,l1,...,lm)) in QDimacs format */



// Function to write exists({x1,...,xt,l1,...,lm}, B'(x1,...,xn,l1,...,lm)) in QDimacs format 
// in file blasted_qualmode.cnf 
void writeInQDimacs(set<string> &CNF, int NumVars, set<string> &ExQuantifiedVars1, set<string> &ExQuantifiedVars2);



// Function to create B' from A as described previously 
// in file blasted_qualmode.cnf
void bitBlastAndQuantify(DAGNode *DAG_to_blast, set<string> VarSetToElim, map<string, int> WidthTable);


// The following two functions are for applying CUDD based ex. quantifier elimination
// Note that these are obselete functions, and in future, use the corresponding functions
// in DDManager.cpp. 


void bitBlastAndQuantifyUsingCDD(DAGNode *DAG_to_blast, set<string> VarSetToElim, map<string, int> WidthTable);
void quantifyAtBitLevel(DAGNode *node, set<string> &VarSetToElim);


// FUNCTIONS FOR UNSATISFIABLE CORE FINDING



// Given "node" which is an unsat bv-logic formula, this function returns the unsat core
// in "UnsatCoreSignals".
// This is achieved by bit-blasting + using a SAT solver which returns unsat core
// This is the first version of the function to achieve this
void blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignals(DAGNode *node, map<string, int> &MyWidthTable, set<string> &UnsatCoreSignals);


// Given the unsat. core in file "corefilename" in a format returned by SAT solver,
// get them in a set of strings "CoreClauses"
void getCoreClauses(set<string> &CNF, string corefilename, set<string> &CoreClauses);


// Function to parse a given string and collect some words
void parse_print_string(char *line_char, set<string> &UnNeeded);



// Get LabelTable and ReverseLabelTable which give unique labels to the nodes in the 
// dag and vice-versa for the propositional logic formula rooted at "node".
// Similar to the function getLabelTable
void getBirectionalLabelTable(DAGNode *node, map<DAGNode*, string> &LabelTable, map<string, DAGNode*> &ReverseLabelTable);



// Internal function called by getBirectionalLabelTable
void getBirectionalLabelTableRecursively(DAGNode *node, unsigned long long int &label_count, map<DAGNode*, string> &LabelTable, map<string, DAGNode*> &ReverseLabelTable);



// Get the support of the clauses in "CoreClauses" in set "SupportOfCoreClauses"
void getSupportOfCoreClauses(set<string> &CoreClauses, set<string> &SupportOfCoreClauses);



// Get the variable from the literal 
// literal can be variable or -variable
string getVariableFromLiteral(string Literal);



// Given -i, return i; given i, return -i
string negateLiteral(string Var);



// Get the set of variables (support) in the given clause "Clause" 
void splitClause(string Clause, set<string> &Variables);


// Get the bit-level support of core clauses
void getBitLevelSupportOfCoreClauses(set<string> &SupportOfCoreClauses, map<string, DAGNode*> &ReverseLabelTable,  set<string> &BitLevelSupportOfCoreClauses);


// Get the bit-vector level support of core clauses
void getBitVectorLevelSupportOfCoreClauses(set<string> &BitLevelSupportOfCoreClauses, set<string> &UnsatCoreSignals);


// Same as blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignals
// except that the assignment part is written more efficiently
void blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvanced(DAGNode *node, map<string, int> &MyWidthTable, set<string> &OriginalBits, set<string> &NegatedBits, set<string> &UnsatCoreSignals);


// Get bit-index table, which maps name of variable and bdd index
void getBit_Index_Table(map<DAGNode*, string> &LabelTable,  map<string, string> &Bit_Index_Table);


// Function to change the CNF by attaching the assignment part to it
void attachAssignmentPartToCNF(set<string> &CNF, set<string> &OriginalBits, set<string> &NegatedBits, map<string, string> &Bit_Index_Table);


// Get reverse bit-index table, which maps bdd index and variable name
void getReverse_Bit_Index_Table(map<string, string> &Bit_Index_Table, map<string, string> &Reverse_Bit_Index_Table);


// The latest version of getBitLevelSupportOfCoreClauses
void getBitLevelSupportOfCoreClausesAdvanced(set<string> &CoreClauses, map<string, string> &Reverse_Bit_Index_Table,  set<string> &BitLevelSupportOfCoreClauses);


// Latest version of the function getCoreClauses
void getCoreClausesAdvanced(string corefilename, set<string> &CoreClauses);


// Trim leading spaces from the string char *line_char
string trimLeadingSpaces(char *line_char);


// Same as blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignals
// except that 
// 1) The assignment part is written more efficiently
// 2) run_till_fix_modified.pl is used instead of zminimal
// This is the final function to achieve this functionality
void blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficient(DAGNode *node, map<string, int> &MyWidthTable, set<string> &OriginalBits, set<string> &NegatedBits, set<string> &UnsatCoreSignals);


// Remove trivial clauses from CNF
void removeTrivialClauses(set<string> &CNF);


// Return true if "Clause" is trivial; false otherwise.
// A clause is trivial if a variable is present in both original and negated versions.
bool trivialClause(string Clause);


// return true if "DAG_to_blast" is unsat; return false otherwise.
// Satisfiability is checked by blasting followed by QBF solving.
bool checkSatUsingQBFSolver(DAGNode *DAG_to_blast, map<string, int> WidthTable);


// Call QBF solver to solve \exists ExQuantifiedVars. CNF where CNF 
// represents the equisatisfiable CNF for some propositional logic formula
// return true if unsat; false if sat
bool checkQBFUnsat(set<string> &CNF, int NumVars, set<string> &ExQuantifiedVars);


// Function to add add sufficient no: of zeros as msbs to vector Bits to make it of size width
void zeroPadding(vector <DAGNode*> &Bits, int width);


// This function checks if 
// \exists RemainingVars \forall VarSetToElim.(\neg \phi)  is unsat
// "DAG_to_blast" here represents \phi 
// returns true if \exists RemainingVars \forall VarSetToElim.(\neg \phi)  is unsat ; false otherwise
bool checkQBFSolverEffectiveness(DAGNode *DAG_to_blast, set<string> &VarSetToElim, set<string> &VarSetRemaining, map<string, int> &WidthTable);




//  Version of  blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficient
// for a given dag. There is no assignment part here
void blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAG(DAGNode *node, map<string, int> &MyWidthTable, set<DAGNode*> &CoreConstraints);


// Get the blasted dag level support of core clauses
void getBlastedDAGLevelSupportOfCoreClauses(set<string> &SupportOfCoreClauses, map<string, DAGNode*> &ReverseLabelTable,  set<DAGNode*> &BlastedDAGLevelSupportOfCoreClauses);



// Get the original dag level support of core clauses
void getOriginalDAGLevelSupportOfCoreClauses(set<DAGNode*> &BlastedDAGLevelSupportOfCoreClauses,  set<DAGNode*> &OriginalDAGLevelSupportOfCoreClauses);



//  Version of  blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAG
// The SAT solver used is picosat, which is more efficient that zchaff
void blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAGUsingPicoSAT(DAGNode *node, map<string, int> &MyWidthTable, set<DAGNode*> &CoreConstraints);


// corefilename is teh list of variables used in the unsat core generated by picosat
// Just read it, and put it in SupportOfCoreClauses
void getSupportOfCoreClausesFromPicoSAT(string corefilename, set<string> &SupportOfCoreClauses);



// Bit-blast the given dag and print the dag in DIMACS format in the given file
bool blastAndOutputInGivenFile(DAGNode *DAG_to_blast, map<string, int> &WidthTable, string blasted_output_file);


// returns true if the bvmul appears as constant * variable, where constant is the 
// maximum constant with the given width; returns false otherwise
bool ItIsMinusVariable(DAGNode* node);


// Internal function called by the function getBitBlastedDAG in BitBlaster.cc
// Given a bit-vector formula rooted at "node", returns the bits of the propositional
// logic formual obtained by blasting "node", as a vector<DAGNode *>
// This implements bit-blasting similar to the way bit-blasting is implemented in STP
const vector<DAGNode *> BBForm(DAGNode * node, map<DAGNode*, vector<DAGNode*> > &BBFormMem);


//bit-blasts bvule node; left and right are blasted children
DAGNode* BBbvule(const vector<DAGNode*>& left_, const vector<DAGNode*>& right_, bool is_bvlt);


//function to do blasting of + operator
void BBPlus(vector<DAGNode*> &sum, const vector<DAGNode*>& y, DAGNode* cin);


// return the majority function of a, b, and c
DAGNode* Majority(DAGNode* a, DAGNode* b, DAGNode* c);


//function to do blasting of * operator
vector<DAGNode*> BBMult(const vector<DAGNode*> &x, const vector<DAGNode*> &y,  DAGNode* Child1, DAGNode *Child2);


// Left shift  within fixed field inserting zeros at LSB.
// Writes result into first argument.
void BBLShift(vector<DAGNode*> &x, unsigned int shift);


//return false vector of given size
vector<DAGNode*> FalseVector(int bitWidth);


//function to do blasting of x+1
vector<DAGNode*> BBInc(const vector<DAGNode*>& x);


//function to do negate all bits
vector<DAGNode*> BBNeg(const vector<DAGNode*>& x);


// version of function zeroPadding with return
vector<DAGNode*> zeroPaddingWithRet(const vector <DAGNode*> &Bits, int width);



// Find the labels of the independent variables; This is needed for Kuldeep
void findLabelsOfIndependentVariables(map<DAGNode*, string> &LabelTable, set<string> &independent_variables, set<string> &LabelsOfIndependentVariables);



// In getBitBlastedDAG, the argument should be a formula. 
// This is a general version of getBitBlastedDAG that takes 
// a bit-vector term or formula
const vector<DAGNode *> getBitBlastedDAG_General(DAGNode *node);


// Function to show blasted bits of a node
void showBlastedBits(DAGNode* node, vector<DAGNode*> &Bits);

#endif	/* _BITBLASTER_H */
