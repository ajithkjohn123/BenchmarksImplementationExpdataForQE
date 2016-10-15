/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/



#ifndef _DAGELIMINATOR_H
#define	_DAGELIMINATOR_H

#include <string>
#include <iostream>
#include <sys/time.h>

#include "DDManager.h"
#include "ILP_Eliminator.h"

#include <fstream>
#include <list>
#include <set>
#include <map>
    
using namespace std;

extern DAGManager dm;
extern DDManager ddm;

// For Monniaux
extern time_t getModel_time;
extern time_t Generalize1_time;
extern time_t Generalize2_time;
extern time_t Project_time;
extern time_t Quant_Monn_time;
extern time_t QElim_Time;
// For Monniaux ends here

// Declarations for timeout
extern time_t time_out;
extern time_t time_out_start;
extern bool timed_out;
// Declarations for timeout ends here

// Declarations for comparing with OmegaTest (Some of these are printed in Layered...txt also)
extern int NoOfCallsToAPQElim;
extern bool compare_apqelim_omegatest;

// These are across all calls to AP_QElim
extern int APQElim_Total_Eqs;
extern float APQElim_Avg_Eqs;
extern int APQElim_Max_Eqs;
extern int APQElim_Min_Eqs;
extern int APQElim_Total_Diseqs;
extern float APQElim_Avg_Diseqs;
extern int APQElim_Max_Diseqs;
extern int APQElim_Min_Diseqs;
extern int APQElim_Total_Vars;
extern float APQElim_Avg_Vars;
extern int APQElim_Max_Vars;
extern int APQElim_Min_Vars;
extern int APQElim_Total_VarsToElim;
extern float APQElim_Avg_VarsToElim;
extern int APQElim_Max_VarsToElim;
extern int APQElim_Min_VarsToElim;
extern int APQElim_Total_Bitwidth;
extern float APQElim_Avg_Bitwidth;
extern int APQElim_Max_Bitwidth;
extern int APQElim_Min_Bitwidth;

// These are across calls to AP_QElim with #LMDs > 0
extern float APQElim_Total_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs;
extern float APQElim_Avg_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs;
extern float APQElim_Max_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs;
extern float APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs;
extern int APQElim_Calls_GtZero_Diseqs;
extern int APQElim_Total_Eqs_for_Calls_GtZero_Diseqs;
extern float APQElim_Avg_Eqs_for_Calls_GtZero_Diseqs;
extern int APQElim_Max_Eqs_for_Calls_GtZero_Diseqs;
extern int APQElim_Min_Eqs_for_Calls_GtZero_Diseqs;
extern int APQElim_Total_Diseqs_for_Calls_GtZero_Diseqs;
extern float APQElim_Avg_Diseqs_for_Calls_GtZero_Diseqs;
extern int APQElim_Max_Diseqs_for_Calls_GtZero_Diseqs;
extern int APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs;
extern int APQElim_Total_Bitwidth_for_Calls_GtZero_Diseqs;
extern float APQElim_Avg_Bitwidth_for_Calls_GtZero_Diseqs;
extern int APQElim_Max_Bitwidth_for_Calls_GtZero_Diseqs;
extern int APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs;

// Declarations for comparing with OmegaTest end here (Some of these are printed in Layered...txt also)

// Declarations for comparing with Bit-blasting
extern bool bitblasting_with_scopereduction;
extern bool compare_apqelim_blasting;
// Declarations for comparing with Bit-blasting end here

// For selecting the generalize2 strategy; default=2=our  strategy
extern int generalize2_strategy;
// For selecting the generalize2 strategy; default=2=our  strategy

extern bool output_conjunctions;


// For LMI handling
extern unsigned long long int z3_timeout_ms;
// Following are the data we need 
extern string InFileName;

extern int LMINoVarsToElim;
extern int LMINoLMIs;
extern int LMINoLMEs;
extern int LMINoLMDs;
extern int LMINoVars;
extern unsigned long long int LMITotalBits;
extern unsigned long long int LMIBitsToElim;
extern int LMIVarsInputToLayer2;
extern int LMIVarsInputToLayer3;
extern int LMIVarsInputToLayer4;
extern unsigned long long int LMILayer1Time;
extern unsigned long long int LMILayer2Time;
extern unsigned long long int LMILayer3Time;
extern unsigned long long int LMILayer4Time;;
extern bool Octagons;


extern HashTable <string, DAGNode*> Monniaux_HT; 
extern map<string, int> VariableMap;

extern bool LMI_QE_using_blasting;

extern unsigned long long int Monniaux_HT_hits;
extern unsigned long long int Monniaux_HT_misses;



extern int BlastingConjunctionCounter;
extern unsigned long long int LMIBlastingTime;
extern FILE* LMIQEImportantDataBlastingFp;


//Variables for collecting data on layer3
extern int LMILayer3Calls;
extern int LMILayer3CallsTimedOut;
extern int LMILayer3CallsFinished;
extern int LMILayer3CallsSCAchieved;
extern int LMILayer3CallsNoSC;
extern int LMILayer3CallsTrue;
extern FILE* LMIQEFinishedLayer3DataFp;

extern FILE* LMIQEFinishedLayer3TimedOutDataFp;
extern FILE* LMIQEFinishedLayer3SCAchievedDataFp;
extern FILE* LMIQEFinishedLayer3NoSCDataFp;
extern FILE* LMIQEFinishedLayer3TrueDataFp;



extern FILE *LMIQEImportantDataBlastingFp;
extern int LMIProjectBlastOriginalNoVarsToElim;
extern int LMIProjectBlastOriginalNoLMCs;
extern unsigned long long int LMIProjectBlastOriginalNoBitsToElim;
extern int LMIProjectBlastToBlastNoVarsToElim;
extern int LMIProjectBlastToBlastNoLMCs;
extern unsigned long long int LMIProjectBlastToBlastNoBitsToElim;


extern FILE *LMIQELayer1EffectivenessFp;
extern int LMIPostLayer1NoVarsToElim;
extern int LMIPostLayer1NoLMCs;
extern unsigned long long int LMIPostLayer1NoBitsToElim;

extern bool LMI_QE_using_OT;
extern int OTConjunctionCounter;
extern unsigned long long int LMIOTTime;
extern FILE* LMIQEImportantDataOTFp;
extern FILE* LMIQEConjunctionLevelOTFp;

extern bool SwitchOffLayer3;

extern bool OTAfterLayer2;
extern FILE* LMIQEImportantDataOTAfterLayer2Fp;

extern int total_number_of_constraints_to_preprocess;
extern int total_number_of_constraints_to_preprocess_input_to_layer2;
extern int total_number_of_constraints_to_preprocess_input_to_layer3;
extern int total_number_of_variables_to_preprocess;
extern int total_number_of_variables_to_preprocess_input_to_layer2;
extern int total_number_of_variables_to_preprocess_input_to_layer3;
extern int total_number_of_lmes_to_preprocess;
extern int total_number_of_lmds_to_preprocess;
extern int total_number_of_lmis_to_preprocess;
extern unsigned long long int layer1_time_to_preprocess;
extern unsigned long long int layer2_time_to_preprocess;
// For LMI handling ends here

// FUNCTIONS TO IMPLEMENT MONNIAUX STYLE QE ON BOOLEAN COMBINATIONS OF LMCs



// Internal function called by getAtomicPredicates 
// to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
void getAtomicPredicatesInternal(DAGNode* node, set<DAGNode*> &APSet, set<DAGNode*> &AlreadyEncountered);




// Function to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
// Implemented with dynamic programming
void getAtomicPredicates(DAGNode* node, set<DAGNode*> &APSet);



// Given a dag rooted at "node", this function checks if dag is sat/unsat using yices.
// If unsat return true. 
// If sat, get the model in the map Model by parsing the CEX returned from yices
bool getModel(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);



// Implementation of procedure Monniaux presented in our CAV'11 work
// Given a dag rooted at "F" and a set of variables "VarSetToElim", this
// finds \exists VarsToElim. (F) and returns it
DAGNode* Monniaux_Quantifier_Eliminator(DAGNode *root, set<string> VarSetToElim, map<string, int> &WidthTable);




// Print the dag rooted at "f" in yices format, call yices to (i) check if it is sat/unsat and set the 
// return value accordingly, and (ii) if sat, get the model in result_thunsat_single_formula.txt
bool ThUnsatSingleFormulaWithCEXFile(DAGNode* f, map<string, int> &WidthTable);




// Implements Generalize1 from Monniaux's LPAR 2008 paper
// Given a variable ---> value map Model, evaluates the APs in APSet.
// The APs which are evaluated to true are pushed into Constraints
// The APs which are evaluated to false are negated and the negation is pushed into Constraints
void Generalize1(map<string, string> &Model, DAGNode* F, set<DAGNode*> &APSet, map<string, int> &WidthTable, vector<DAGNode*> &Constraints);


// Check sat/unsat of /\(Constraints\Constraints[drop_this]) /\ ~F, and
// return true/false accordingly
bool ThUnsatMonniaux(DAGNode *F, vector<DAGNode*> &Constraints, int drop_this, map<string, int> &WidthTable);



// This function finds \exists VarsToElim. (/\ Constraints) and returns it
// This has options to compare QE using QE_LMC with QE using BV_Solve and Omegatest
DAGNode* callAPResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



// Finds the variables to eliminate from the dag. 
// This is done by finding the set of variables in the dag rooted at "dag" 
// such that their delta times are 1 in "VarsToElim"
void getVarsToElimInGetTransitiveClosure(DAGNode* dag, set<string> &VarsToElim);



// Get the transitive closure for the given transitive relation for the the number 
// of times in "squaring_times", and return it
DAGNode* getTransitiveClosure(DAGNode *TR, int squaring_times, map<string, int> &WidthTable, string infilename);



// returns true if fixpoint is reached in iterative squaring; false otherwise
// fixpoint is reached if old_dag = new_dag
bool fixpointReached(DAGNode *old_dag, DAGNode *new_dag, map<string, int> &WidthTable);



// Internal function called by function Generalize2_Strategy2
// Return value indicates the result of evaluating the dag node rooted at "F"
// using the model "Model". The reduced model, i.e. model with unnecessary constraints
// dropped is in "ReducedModel"
char Generalize2_Strategy2_Internal(DAGNode* F, set<DAGNode*> &Model, set<DAGNode*> &ReducedModel, map<DAGNode*, pair< char, set<DAGNode*> > > &DPTable);



// Implementation of Generalize2 strategy in our CAV 2011 work
// Removes unnecessary constraints from "Constraints" using an evaluation based strategy
void Generalize2_Strategy2(vector<DAGNode*> &Constraints, DAGNode* F);



// Given two sets of DAGNode*, "ReducedModelChild1" and "ReducedModelChild2", assign in 
// "ReducedModelChild" the set with minimum cardinality.
void findSmaller(set<DAGNode*> &ReducedModelChild1, set<DAGNode*> &ReducedModelChild2, set<DAGNode*> &ReducedModel);



// Calls the function Monniaux_Quantifier_Eliminator, and records the 
// time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording(DAGNode *F, set<string> VarSetToElim, map<string, int> &WidthTable);


// Function called from Generalize2_Strategy2_Internal to handle and node with
// more than two children
// returns 't' or 'f' depending on how the children gets evaluated 
char Generalize2_Strategy2_Internal_For_And_With_Multiple_Children(vector< set<DAGNode*> > &ReducedModelChildren, vector<char> &ResultChildren, set<DAGNode*> &ReducedModel);



// Function called from Generalize2_Strategy2_Internal to handle or node with
// more than two children
// returns 't' or 'f' depending on how the children gets evaluated
char Generalize2_Strategy2_Internal_For_Or_With_Multiple_Children(vector< set<DAGNode*> > &ReducedModelChildren, vector<char> &ResultChildren, set<DAGNode*> &ReducedModel);



// Unrolls the relation "TR", "squaring_times" #of times, and prints the unrolled relation 
// in SMTLIB format
// Used when the command line option -unrolling is true
void getUnrolled(DAGNode *TR, int squaring_times, map<string, int> &WidthTable, string infilename);



// Select the variables to be eliminated in the -unrolling mode in "VarsToElim"
// Presently we select all the variables as variables to eliminate
// Used when the command line option -unrolling is true
void getVarsToElimInUnrolling(DAGNode* dag, set<string> &VarsToElim, int times);



// Implementation of Generalize2 strategy in Monniaux's LPAR 2008 work
// Removes unnecessary constraints from "Constraints" using an SMT-Solver (STP/yices)
void Generalize2_Monniaux(vector<DAGNode*> &Constraints, DAGNode* F, map<string, int> &WidthTable);



// Get the details at the QE_LMC level, before QE_LMC is called
void getAP_QElimDetailsAtDAGLevel(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



// Print the dag rooted at "f" in SMTLIB format, call STP to (i) check if it is sat/unsat and set the 
// return value accordingly, and (ii) if sat, get the model in result_thunsat_single_formula.txt
bool ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG(DAGNode* f, map<string, int> &WidthTable);



// Given a dag rooted at "node", this function checks if dag is sat/unsat using STP
// If unsat return true. 
// If sat, get the model in the map Model by parsing the CEX returned from STP
bool getModelUsingSTP(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);



// Given a dag rooted at "node", this function checks if dag is sat/unsat using STP/yices
// If unsat return true. 
// If sat, get the model in the map Model by parsing the CEX returned from STP/yices
bool getModelGeneral(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);

// FUNCTIONS TO IMPLEMENT MONNIAUX STYLE QE ON DAGs WITH LMCs, SELs, AND CONCATs


// Calls the function Monniaux_Quantifier_Eliminator_WithSupportForSelsConcats, 
// and records the time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForSelsConcats(DAGNode *F, set<string> VarSetToElim, map<string, int> &WidthTable);



// Version of Monniaux which handles equalities, disequalities with +, *, selections, and concatenations
// Given a dag rooted at "F" and a set of variables "VarSetToElim", this
// finds \exists VarsToElim. (F) and returns it
DAGNode* Monniaux_Quantifier_Eliminator_WithSupportForSelsConcats(DAGNode *F, set<string> VarSetToElim, map<string, int> &WidthTable);


// Version of getModelGeneral which handles equalities, disequalities with +, *, selections, and concatenations
bool getModelGeneral_WithSupportForSelsConcats(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);



// Version of getModel which handles equalities, disequalities with +, *, selections, and concatenations
bool getModel_WithSupportForSelsConcats(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);



// Version of ThUnsatSingleFormulaWithCEXFile which handles equalities, 
// disequalities with +, *, selections, and concatenations
bool ThUnsatSingleFormulaWithCEXFile_WithSupportForSelsConcats(DAGNode* f, map<string, int> &WidthTable);



// Version of Generalize1 which handles equalities, 
// disequalities with +, *, selections, and concatenations
void Generalize1_WithSupportForSelsConcats(map<string, string> &Model, DAGNode* F, set<DAGNode*> &APSet, map<string, int> &WidthTable, vector<DAGNode*> &Constraints);



// Function to return exists VarsToElim. (/\ Constraints) given (i) a set of variables VarsToElim ,and
// (ii) a set of constraints Constraints which are equalities, disequalities with +, *, selections, and concatenations.
// QE is done with Nelson-Oppen purification followed by our CAV'11 work, Sharygyna and Bruttomesso's ICCAD'08
// work, and bit-blasting + CUDD for the remaining variables
DAGNode* QE_Using_LMA_Bruttomesso_Blasting_ReturningDAG(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);




// Function to apply Nelson - Oppen purification to a set of constraints "Bound_Constraints".
// The result is obtained in FinalPurifiedConstraints.
void applyNOpurification(set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable, set<DAGNode*> &FinalPurifiedConstraints, set<string> &VarsToElim);



// Function to apply Nelson - Oppen purification to a constraint "Constraint".
// The result is obtained in PurifiedConstraints
void NOpurification(DAGNode *Constraint, map<string, int> &WidthTable, set<DAGNode*> &PurifiedConstraints, int &FreshVars_Count, map<DAGNode*, DAGNode*> &InverseFreshVars_Map);



// Return the type of constraint; "nil" | "core" | "arithmetic"
string findTypeOfConstraint(DAGNode* Constraint);



// Find the set of impure nodes under the dag rooted at "Constraint"
void findImpureNodes(DAGNode* Constraint, set<DAGNode*> &ImpureNodes);



// Find the set of impure nodes under the dag rooted at "node"
// which is of type "type_constraint"
void getImpureNodes(DAGNode* node, string type_constraint, set<DAGNode*> &ImpureNodes);



// Given a dag rooted at "parent", return a clone which is purified.
// The new equalities which are created during the purification are
// added into the set "NewConstraints"
DAGNode* createPuredClone(DAGNode* parent, set<DAGNode*> &NewConstraints, set<DAGNode*> &ImpureNodes, map<string, int> &WidthTable, int &FreshVars_Count, map<DAGNode*, DAGNode*> &InverseFreshVars_Map);



// Returns the conjunction of the constraints in "CoreConstraints"
// In that sense, it is same as DAGEliminator::getConjunctionFromSet, but
// this function is kept as it has a different set of parameters compared
// to the function DAGEliminator::getConjunctionFromSet.
DAGNode* Trivial_Bruttomesso(set<DAGNode*> &CoreConstraints, set<string> &CoreVarsToElim, map<string, int> &WidthTable);


// Returns exists CoreVarsToElim. (/\ CoreConstraints) using 
// Bruttomesso and Sharygyna's ICCAD 2008 work
DAGNode* Bruttomesso(set<DAGNode*> &CoreConstraints, set<string> &CoreVarsToElim, map<string, int> &WidthTable);




// Returns true if "Constraint" is in concatenation normal form; returns false otherwise 
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of concatenation normal form
bool constraintInConcatenationNormalForm(DAGNode *Constraint);



// Returns true if "Constraint" is a concatenation of simple terms; returns false otherwise 
// simple term ::= variable | constant | select(variable, constant, constant)
bool constraintIsConcatenationOfSimpleTerms(DAGNode *Constraint);



// Returns true if "Constraint" is a simple select; returns false otherwise 
// simple select ::= select(variable, constant, constant)
bool simpleSelect(DAGNode *Constraint);



// Returns true if both LHS and RHS of constraint rooted at "Constraint" are simple terms; returns false otherwise 
// simple term ::= variable | constant | select(variable, constant, constant)
bool bothSidesAreSimpleTerms(DAGNode *Constraint);



// Returns true if "Constraint" is a simple term; returns false otherwise 
// simple term ::= variable | constant | select(variable, constant, constant)
bool SimpleTerm(DAGNode *Constraint);



// Given a a constraint "Constraint" which is LHS = RHS where both LHS and RHS are
// concatenations of simple terms, generate new equalities from it
// Let LHS = RHS is s1@...@sn = t1@...@tn. New equalities are {s1=t1,...,sn=tn}
void getNewEqualitiesFromConstraint(DAGNode* Constraint, map<string, int> &WidthTable, set<DAGNode*> &NewEqualities);



// Given a dag node rooted at "Constraint", get the children of "Constraint" in 
// "ChildrenOfConcat", and the sizes of children of "Constraint" in "SizesOfChildrenOfConcat" 
void getChildrenOfConcatAndGetSizes(DAGNode* Constraint, list<DAGNode*> &ChildrenOfConcat, list<int> &SizesOfChildrenOfConcat, map<string, int> &WidthTable);




// return true if the sum of elements in "SizesOfChildrenOfConcat" is "variable_width";
// return false otherwise 
bool mismatchInSizes(list<int> &SizesOfChildrenOfConcat, int variable_width);



// Given a set of equalities over simple terms, "EqualitiesOverSimpleTerms",
// generate a set of equalities over simple terms on coarsest base in "EqualitiesOverSimpleTermsOnCoarsetBase"
void getEqualitiesOverSimpleTermsOnCoarsetBase(set<DAGNode*> &EqualitiesOverSimpleTerms, map<string, int> &WidthTable,  set<DAGNode*> &EqualitiesOverSimpleTermsOnCoarsetBase);



// Given a constraint rooted at "Constraint", update the "Base" using the constraint.
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of "Base".
void getExtractionsAndUpdateBase(DAGNode* Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base);



// show the existing "Base"
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of "Base".
void showBase(map<string, set<int> > &Base);



// returns true if the given equality rooted at "Constraint" respects (does not contradict) 
// the existing "Base"; returns false otherwise.
// Refer Bruttomesso and Sharygyna's ICCAD 2008 work for definition of "Base".
bool equalityRespectsBase(DAGNode* Equality, map<string, set<int> > &Base, map<string, int> &WidthTable);



// Given a simple term rooted at "Constraint", return
// the variable name. Return "" if it is a constant.
string getVariableFromDAG(DAGNode *Constraint);



// Changes the "Slices" by removing entries from "Slices" which are > high or <=  low
void trimSlices(set<int> &Slices, int high, int low);



// Checks if the constraint rooted at "Constraint" needs to be reconstructed as per the "Base"
bool needToBeReconstructed(DAGNode *Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base);



// Find the set of slices in the simple term rooted at "Constraint"
void findSlices(DAGNode *Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base, set<int> &Slices);


// Reconstruct the equality rooted at "Constraint" according to "Base", and push back the 
// reconstructed equality into "ReconstructedEqualities"
void ReconstructEqualities(DAGNode *Constraint, map<string, int> &WidthTable, map<string, set<int> > &Base, set<DAGNode*> &ReconstructedEqualities);


// return the low index of extraction node
int getLowIndexOfExtractionNode(DAGNode *Constraint);



// return the high index of extraction node
int getHighIndexOfExtractionNode(DAGNode *Constraint);

// Functions to implement Monniaux style QE on dags with LMCs and sels, concats end here


// Functions to implement Monniaux style QE on dags with LMIs start here


// Internal function called by getAtomicPredicates_WithSupportForLMIs 
// to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
// such that the atomic predicates can be LMIs
void getAtomicPredicates_WithSupportForLMIsInternal(DAGNode* node, set<DAGNode*> &APSet, set<DAGNode*> &AlreadyEncountered);


// Function to get the atomic predicates in a dag rooted at "node" given in propositional skelton form
// Implemented with dynamic programming such that the atomic predicates can be LMIs
void getAtomicPredicates_WithSupportForLMIs(DAGNode* node, set<DAGNode*> &APSet);


// Version of Generalize1_WithSupportForSelsConcats with support for LMIs also
void Generalize1_WithSupportForSelsConcats_WithSupportForLMIs(map<string, string> &Model, DAGNode* F, set<DAGNode*> &APSet, map<string, int> &WidthTable, vector<DAGNode*> &Constraints);


// Version of Generalize2_Strategy2 with support for LMIs
void Generalize2_Strategy2_WithLMISupport(vector<DAGNode*> &Constraints, DAGNode* F);


// Version of Generalize2_Strategy2_Internal with LMI support
char Generalize2_Strategy2_WithLMISupport_Internal(DAGNode* F, set<DAGNode*> &Model, set<DAGNode*> &ReducedModel, map<DAGNode*, pair< char, set<DAGNode*> > > &DPTable);


// Function to return exists VarsToElim. (/\ Constraints) given (i) a set of variables VarsToElim ,and
// (ii) a set of constraints Constraints which are LMIs..
// QE is done with bit-blasting + CUDD
DAGNode* QE_Using_Blasting_WithSupportForLMIs(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



 // Call semantic scope reduction using unsat core computation
// Here Phi = /\ of constraints in Constraints
// Result is in 1) partition returned in PartitionedConjunction, 2) return false if timed out; else true
 bool semanticScopeReduction(DAGNode* Phi, set<DAGNode*> &Constraints,  set<string> &VarsToElim, map<string, int> &WidthTable, set< set<DAGNode*> > &PartitionedConjunction);

 
 
 // This returns the set of variables other than the variables to be eliminated
void getVariablesToRemain(DAGNode* Phi, set<string> &VarSetToElim, set<string> &VarSetRemaining);



// Initialize the ConstraintMaps. 
// ConstraintMap associates a unique number with each constraint
// ReverseConstraintMap is the reverse mapping
void initializeConstraintMaps(map<int, DAGNode*> &ConstraintMap, map<DAGNode*, int> &ReverseConstraintMap, set<DAGNode*> &Constraints);



// show the ConstraintMaps. 
// ConstraintMap associates a unique number with each constraint
// ReverseConstraintMap is the reverse mapping
void showConstraintMaps(map<int, DAGNode*> &ConstraintMap, map<DAGNode*, int> &ReverseConstraintMap);



// Print the dag rooted at "dag" in CVC3 SMTLIB format as "forall VarsToElim. dag".
// call z3 to (i) check if it is sat/unsat and set the  return value accordingly. return value = sat | unsat | unknown
//, and (ii) if sat, get the model in result_conjunction.txt
string ThUnsatSingleFormulaWithCEXFileInCVC3StyleSMTLibFormat_WithLMISupport(DAGNode* dag, map<string, int> &WidthTable, set<string> &VarSetToElim, set<string> &VarsToRemain);



// Print the dag rooted at "dag" in CVC3 SMTLIB format as "forall VarsToElim. dag".
// call z3 to (i) check if it is sat/unsat and set the  return value accordingly. return value can be sat | unsat | unknown
//, and (ii) if sat, get the model in the map Model
string getModelForQBVF_UsingZ3_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable, set<string> &VarSetToElim, set<string> &VarsToRemain);



// Get the model in the variable --> value map Model 
// by parsing the CEX returned from z3 in the file result_conjunction.txt
void getModelFromZ3CEXFile(map<string, string> &Model);



// Get indices of core constraints and other constraints in CoreConstraintIndices, OtherConstraintIndices respectively
void getCoreConstraintIndices(map<DAGNode*, int> &ReverseConstraintMap, set<DAGNode*> &CoreConstraints, set<int> &CoreConstraintIndices, set<int> &OtherConstraintIndices, map<string, string> &YMap);



// Show the partition
void showPartition(vector < set<int> > &CorePartitions, set<int> &RemainingPartition);



// Create the constraint from the parition
DAGNode* createConstraintFromPartition(vector< set<int> > &CorePartitions, set<int> &RemainingPartition, map<int, DAGNode*> &ConstraintMap, set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode *NegPhi, set<string> &UnivQuantVars, set<string> &ExistQuantVars);



// Get the constraints from a partition
void getConstraintsFromParticularPartition(set<int> &ThisPartition, map<int, DAGNode*> &ConstraintMap, set<DAGNode*> &ConstraintsFromThisPartition);


// Function to refine the existing partition based on the new unsat. core
void refinePartition(vector< set<int> > &CorePartitions, set<int> &RemainingPartition, set<int> &CoreConstraintIndicesInLoop);


// Inter = Indices of elements in \pi such that \pi[i] \intersect \Beta \equiv \nonempty    
void findPartitionsIntersectingNewUnsatcore(vector< set<int> > &Pi, set<int> &Beta,  set<int> &Inter);


// Function to refine the existing partition based on the new unsat. core, where the partitions can be intersecting
void refineIntersectingPartition(vector< set<int> > &CorePartitions, set<int> &RemainingPartition, set<int> &CoreConstraintIndicesInLoop);


// Try to find another unsat. core
void tryToFindUnsatCore(set<DAGNode*> &Constraints, map<string, string> &YMap, map<string, int> &WidthTable, set<int> &CoreConstraintIndices, set<int> &OtherConstraintIndices, map<DAGNode*, int> &ReverseConstraintMap, bool check_for_proper_partition, bool cross_check_unsat_core, string solver);


// Check simplified version of z3 constraint
// Sets loop_entry_str properly; returns changed PreventedYMaps
DAGNode* checkSimplifiedZ3Constraint(DAGNode* NegPhi, DAGNode* PreventedYMaps, map<string, int> &WidthTable, map<string, string> &YMap, vector< set<int> > &CorePartitions, set<int> &RemainingPartition, map<int, DAGNode*> &ConstraintMap, set<DAGNode*> &Constraints, set<string> &VarsToElim,  set<string> &VarsToRemain, string &loop_entry_str);


// Given YMap signal ---> value and widthtable, create a dag (signal0=value0) /\ .... /\ (signaln=valuen)
DAGNode* createDAGFromYMap(map<string, string> &YMap, map<string, int> &WidthTable);


// Call syntactic scope reduction
// Here Phi = /\ of constraints in Constraints
 void syntacticScopeReduction(set<DAGNode*> &Constraints,  set<string> &VarsToElim, map<string, int> &WidthTable);
 
 
 
 // show the ConstraintsVarsToElimMap
void showConstraintsVarsToElimMap(string WhoAmI, map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map);



// Return location in Constraints_VarsToElim map with minimum sized LHS
map<set<DAGNode*>, set<string> >::iterator findEntryWithMinLHS(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map);



// recreate the constraints to vars to elim map
void recreateConstraints_VarsToElim_Map(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map, map<set<DAGNode*>, set<string> >::iterator minlhs_it);


// returns true if Set1 is intersecting with Set2; false otherwise
bool checkIntersecting(set<DAGNode*> &Set1, set<DAGNode*> &Set2);


// show the ConstraintsVarsToElimMap
void showConstraintsVarsToElimMap(string WhoAmI, map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map);


// create the partitioned conjunction as per the partitions
void createPartitionedConjunction(map<int, DAGNode*> &ConstraintMap, vector< set<int> > &CorePartitions, set<int> &RemainingPartition, set< set<DAGNode*> > &PartitionedConjunction);


// return true if there exists a proper subset of MyConstraints in the LHS of Constraints_VarsToElim_Map_Final
bool properSubsetOfMyConstraintsExistsInMap(set<DAGNode*> &MyConstraints,  map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map);


// returns true if Set1 is a proper subset of Set2; false otherwise
// Set1 is a proper subset of Set2 <=> (Set1 \ Set2 = empty set) and (Set2 \ Set1 != empty set)
bool isProperSubset(set<DAGNode*> &Set1, set<DAGNode*> &Set2);



// refine ConstraintsVarsToElimMap
void refineConstraintsVarsToElimMap(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map,  map<set<DAGNode*>, set<string> >::iterator remove_it);



// Given a set of LMIs A and vars to elim X, it tries to 1) synthesize LMEs from the LMIs, and compute exists X. A
// 2) Drop LMIs using the dropping idea
DAGNode* LMEBasedSimplificationAndLMIDropping(set<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable, DAGNode* &QuantifiedPart);



// Check if the given pair of LMIs can be simplified into an LME
bool simplifiableLMIPairFound(DAGNode* ugt, DAGNode* ule);




// Given an "InitialLME" and variable in "VarToElim" convert the "InitialLME" to the form VarToElim = OtherVariable + Constant
// Applicable only for octagons
DAGNode* simplifyLMEToProperForm(DAGNode* InitialLME, map<string, int> &WidthTable, string VarToElim);



// Eliminate quantifiers in ConjunctionOfLMIs using the vector of LMEs LME_Vector
DAGNode* inlineLMIConjunctionUsingLMEs(DAGNode* ConjunctionOfLMIs, set<string> &VarsToElim, vector<DAGNode*> &LME_Vector, map<string, int> &WidthTable);


// Given a dag node "node", variable to eliminate "VarToElim", and LME "VarToElim = RHS",
// replace dag nodes for "VarToElim" by RHS
DAGNode* cloneUsingSingleLME(DAGNode *node,  string VarToElim, DAGNode *LME);


// Called from cloneUsingSingleLME
DAGNode* cloneUsingSingleLMEInternal(DAGNode *parent,  string VarToElim, DAGNode *LME,  map<DAGNode*, DAGNode*> &DPTable);



// Given a set of LMIs and a set of variables to be eliminated, this function applies
// dropping procedure for LMIs on a variable by variable basis.
void dropLMIsForMultipleVariables(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction);


// check if either 1) we have a single LMI 2^k2.c2.x+s \bowtie d or
// 2) we have a pair of LMIs 2^k2.c2.x+s \bowtie_1 d1, 2^k2.c2.x+s \bowtie_2 d2 where \bowtie \in {bvule, bvugt},
//  \bowtie_1,  \bowtie_2 \in  {bvule, bvugt}, and are different , and d, d_1, d_2 are constants
// returns true if (1) or (2) is true; else returns false
// Also sets Constraint_LHS = 2^k2.c2.x+s, Low = d1/d/NULL, and High = d2/d/NULL
bool singleLMIPair(set<DAGNode*> &Bound_Constraints, DAGNode* &Constraint_LHS, DAGNode* &Low, DAGNode* &High);


// Given 2^k2.c2.x in Constraint_LHS, and two dags LHS, RHS which contain 
// numbers, apply dropping using the counting argument
void dropSingleLMIPair(set<DAGNode*> &Bound_Constraints, DAGNode *Constraint_LHS, DAGNode *Low, DAGNode *High, string VariableToElim, set<DAGNode*> &QElim_Bound_Constraints);



// Given a vector of ule, ugt constraints, apply dropping using the counting argument
// Here assumptions are p=k1=ModWidthInteger and k2=0
void dropMultipleLMIPairs(set<DAGNode*> &Bound_Constraints, vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, set<DAGNode*> &QElim_Bound_Constraints);



//Create ule, ugt pairs from Constraints
 void identifyLMIPairs(set<DAGNode*> &Constraints, vector<pair<DAGNode*, DAGNode*> > &LMIPairs);
 
 
 

// Given a set of LMIs and a set of variables to be eliminated, this function calls
// dropLMIsForMultipleVariables repeatedly as long as it drops variables
void dropLMIsForMultipleVariablesRecursive(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction);


// Given a dag node "node", variable to eliminate "VarToElim", and LME "VarToElim = RHS+Constant",
// replace dag nodes for "VarToElim" by "RHS+Constant", and return the simplified dag
DAGNode* cloneUsingSingleLMEExpression(DAGNode *node,  string VarToElim, DAGNode *LME);


// Called from cloneUsingSingleLMEExpression
DAGNode* cloneUsingSingleLMEExpressionInternal(DAGNode *node,  string VarToElim, string VariableInLME_RHS, ULINT ConstantInLME_RHS, HashTable<string, DAGNode*> *DPTable);



// Given an LMI c1.x1+....+cn.xn+c \bowtie d in "node", a variable xi in "VarToElim",
// a variable y in "VariableInLME_RHS", and a constant k in "ConstantInLME_RHS", 
// create an LMI which is c1.x1+....+cn.xn+c \bowtie d, simplified using xi=y+k. i.e
// return c1.x1+...+ci.y+...+cn.xn+c+k.ci \bowtie d
DAGNode* cloneLMIUsingLME(DAGNode* node, string VarToElim, string VariableInLME_RHS, ULINT ConstantInLME_RHS);



// Get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
// in the respective global variables
void getRelevantLMIDetails(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



// Get #of vars to elim in input of layer2 of LMI QE algorithm
void getNumberOfVarsToElimInLayer2Input(DAGNode* Constraint, set<string> &VarsToElim);



// Get #of vars to elim in input of layer3 of LMI QE algorithm
void getNumberOfVarsToElimInLayer3Input(DAGNode* Constraint, set<string> &VarsToElim);




// Function which implements our QE methods on conjunction of LMIs. 
// This function can be used when the input conjunction has LMIs, LMEs, and LMDs
DAGNode* lmiQEUsingOurMethodsGeneral(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);




// Given a set of LMEs, LMDs, and LMIs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode*
DAGNode* QEFromGeneralConjunctionMultipleVariables(set<DAGNode*> &E, set<DAGNode*> &D,  set<DAGNode*> &I, set<string> VarsToElim, map<string, int> &WidthTable);



// Function to drop LMIs and LMDs given \exists x. (Conjunction of LMEs /\ Conjunction of LMDs /\ Conjunction of LMIs) mod 2^WidthOfVarToElim
DAGNode* dropUnconstrainingDisequationsAndInequations(set<DAGNode*> &LME, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet, string VarToElim,  int WidthOfVarToElim);



// Find the set of constraints with maximum k value among a set of LMIs an LMDs
void findConstraintsWithMaximumKValueForLMDsAndLMIs(set<DAGNode*> &Xi, string VarToElim, set<DAGNode*> &Pi);



// Find maximum k-value from a set of constraints
int findMaximumKValue(set<DAGNode*> &Phi, string VarToElim, int WidthOfVarToElim);



// isExt function for LMIs and LMDs
bool isExtForLMDsAndLMIs(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, DAGNode*  &AdditionalConstraint);



// Function which returns the #of ways to engineer the k1-k2 part given a set of LMI pairs, k1, and a variable to eliminate
long long int findCombinationsPermittedByLMIs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, int k1, ULINT WidthOfVarToElim, ULINT ModWidthOfVarToElim);




// An lmi is replacable if there does not exist any other LMI with the same LHS and opposite sign
bool checkIfLMIIsReplacable(DAGNode* lmi, set<DAGNode*> &LMISet) ;



// check if the LHSs of the LMIs are matching
bool lhsMatching(DAGNode *first, DAGNode* second) ;


// Recreate LMIset by replacing LMIs of the form <= with variables in RHS by LMIs which are tighter with constant on the RHS
void replaceLMIsByTighterOnes(set<DAGNode*> &ReplacableLMIs, int WidthOfVarToElim, set<DAGNode*> &LMISet);


// Recreate LMI of the form <= with variables in RHS by LMIs which are tighter with constant on the RHS
DAGNode* findNewTighterLMI(DAGNode* LMI,  int WidthOfVarToElim);



void divideConjunctionToFreeAndBoundParts(DAGNode *dag, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QFreePart, DAGNode* &BoundPart);


//NewConstraint is lmi's RHS \neq (2^p)-1
DAGNode* getNewConstraintFromBVUGTConstraint(DAGNode *LMI, ULINT ModWidthOfVarToElim, ULINT WidthOfVarToElim);


// Recreate LMI of the form > with variables in RHS by LMIs which are tighter with constant on the RHS
DAGNode* findNewTighterLMIForBVUGT(DAGNode* LMI,  int WidthOfVarToElim);



// Applying splitting and Fourier-Motzkin on \exists VariableToElim. (/\Constraints)
DAGNode* expensiveLayersOnSingleVariable(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable);




// Split the Constraint into disjunctions based on the OVF idea
void obtainSplittedVector(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector);



// Obtain DNF from a constraint of the form (a1 \/ ... \/ an) /\ ...../\ (x1 \/ ...\/ xn) using distributivity
void obtainDNFUsingDistributivity(vector< vector<DAGNode*> > &SplittedConstraints, vector<DAGNode*> &DNF);



// Given A=(a1 \/ ... \/ an) and B=(b1 \/ ...\/ bn), obtain C as (a1b1 \/ ... \/ anbn)
void applyDistributiveLaw(vector<DAGNode*> &A, vector<DAGNode*> &B, vector<DAGNode*> &C);



// Eliminate quantifiers from the vector of conjunctions using Fourier-Motzkin style QE
void eliminateQuantifiersUsingFourierMotzkin(vector<DAGNode*> &DNF, string VarToElim, map<string, int> &WidthTable, set<DAGNode*> &QEliminatedDNF);



// Returns the disjunction of the constraints in "Constraints"
DAGNode* getDisjunctionFromSet(set<DAGNode*> &Constraints);


// Recreate BVUGT constraints by BVULE constraints
DAGNode* recreateBVUGTConstraints(DAGNode* Phi, set<string> &VarsToElim, map<string, int> &WidthTable);


// Recreate BVUGT constraint Constraint as RecreatedConstraint1 /\ RecreatedConstraint2
// where RecreatedConstraint1, RecreatedConstraint2 are BVULE constraint
void getRecreatedBVUGTConstraints(DAGNode *Constraint, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint1, DAGNode* &RecreatedConstraint2);


// Get the components of disjunction
void getComponentsOfDisjunction(DAGNode *node, set<DAGNode*> &Components);


// Splitting and QE using Fourier-Motzkin for multiple variables
DAGNode* expensiveLayersOnMultipleVariable(DAGNode* Phi, vector<string> VarsToElim, map<string, int> &WidthTable);


// Version of obtainSplittedVector which works on LMIs with variable to be eliminated on both sides
void obtainSplittedVectorForLMIsWithBothSides(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector);


// Call syntactic scope reduction. Now apply semantic scope reduction followed by Fourier-Motzkin style
// QE on the scope reduced parts and create the result
DAGNode* syntacticScopeReductionWithQE(DAGNode *Phi,  set<string> &VarsToElim, map<string, int> &WidthTable);


// Apply semantic scope reduction followed by expensive layers
// on \exists VarsToElim. (/\ Constraints)
// Note that some of the constraints in Constraints may be disjunctions
// Hence we need to convert /\Constraints to DNF and then call the layers on each,
// and then take disjunction of the results
DAGNode* applySemanticScopeReductionAndExpensiveLayers(set<DAGNode*> &Constraints,  set<string> &VarsToElim, map<string, int> &WidthTable);



// Version of recreateConstraints_VarsToElim_Map, which  is called from syntacticScopeReductionWithQE
void recreateConstraints_VarsToElim_MapWithQE(map<set<DAGNode*>, set<string> > &Constraints_VarsToElim_Map, map<set<DAGNode*>, set<string> >::iterator minlhs_it, DAGNode *ResultOfQE);


// Get #of vars to elim in input of layer4 of LMI QE algorithm
void getNumberOfVarsToElimInLayer4Input(DAGNode* Constraint, set<string> &VarsToElim);


// Initialize all the conjunction-level parameters to zero
void initializeConjunctionLevelParameters();


// Find # of combinations disallowed LMDs while dropping conjunction of LMIs and LMDs
long long int findCombinationsDisallowedByLMDs(set<DAGNode*> &LMDSet,  string VarToElim, int k1, int WidthOfVarToElim, ULINT ModWidthOfVarToElim);



// Version of isExtForLMDsAndLMIs when there are LMIs
bool isExtForLMDsAndLMIsWithLMDs(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, DAGNode*  &AdditionalConstraint, int k1, ULINT ModWidthOfVarToElim, set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet);




// Calls the function Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting, 
// and records the time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForLMIs_With_Recursive_Splitting(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable);



// create VariableMap
void createVariableMap(map<string, int> &WidthTable);


// show the variable map
void showVariableMap();


// calls  Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting with dynamic programming
DAGNode* callMonniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth);


// Function to get entries from Monniaux_HT
DAGNode* getEntryFromMonniaux_HT(DAGNode* F, set<string> &VarSetToElim);


// Function to insert entries into Monniaux_HT
void insertEntryIntoMonniaux_HT(DAGNode* F, set<string> &VarSetToElim, DAGNode* R);


// Version of Monniaux_Quantifier_Eliminator_WithSupportForLMIs which is recursive
DAGNode* Monniaux_Quantifier_Eliminator_WithSupportForLMIs_With_Recursive_Splitting(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth);


// calls lmiQEUsingOurMethods_With_Recursive_Splitting  with dynamic programming
DAGNode* call_lmiQEUsingOurMethods_With_Recursive_Splitting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);


// Version of lmiQEUsingOurMethods with recursive splitting
DAGNode* lmiQEUsingOurMethods_With_Recursive_Splitting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);



// Version of LMEBasedSimplificationAndLMIDropping which works on BVULE and BVUGE constraints
DAGNode* LMEBasedSimplificationAndLMIDropping_ForWeakLMIs(set<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable, DAGNode* &QuantifiedPart, int &LMIVarsInputToLayer2Local, int &LMIVarsInputToLayer3Local, unsigned long long int &LMILayer1TimeLocal, unsigned long long int &LMILayer2TimeLocal);

// Version of simplifiableLMIPairFound which works on BVULE and BVUGE constraints
bool simplifiableLMIPairFound_ForWeakLMIs(DAGNode* ugt, DAGNode* ule);


// Version of dropLMIsForMultipleVariablesRecursive for BVULE and BVUGE constraints
void dropLMIsForMultipleVariablesRecursive_ForWeakLMIs(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction);


// Version of dropLMIsForMultipleVariables for BVUGE, BVULE constraints
void dropLMIsForMultipleVariables_ForWeakLMIs(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, DAGNode* &QEInlinedLMIConjunction, DAGNode* &QuantifiedInlinedLMIConjunction);



 // Version of dropMultipleLMIPairs on BVULE, BVUGE constraints
void dropMultipleLMIPairs_ForWeakLMIs(set<DAGNode*> &Bound_Constraints, vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, set<DAGNode*> &QElim_Bound_Constraints);




// Applying Splitting and Fourier-Motzkin on split dags with recursive splitting
DAGNode* expensiveLayersOnSetOfScopeReducedDAGs_With_Recursive_Splitting(set< set<DAGNode*> > &ScopeReducedDAGs , set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);


// calls expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting  with dynamic programming
DAGNode* call_expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);


// Apply splitting and Fourier-Motzkin on each conjunction with recursive splitting
DAGNode* expensiveLayersOnScopeReducedDAG_With_Recursive_Splitting(DAGNode* Phi, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);


// Convert the BVUGE Constraints to BVULE
DAGNode* convertBVUGEConstraintsToBVULE(DAGNode* Phi, map<string, int> &WidthTable);


// convert the constraints in Constraints to normal form for the variable VarToElim, using splitting
DAGNode* convertToNormalForm(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable);


// Version of obtainSplittedVector, but with lesser number of disjuncts
void obtainSplittedVectorMadeEfficient(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector);

// Version of obtainSplittedVectorForLMIsWithBothSides with lesser number of disjuncts
void obtainSplittedVectorForLMIsWithBothSidesMadeEfficient(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector);



// Version of getNumberOfVarsToElimInLayer2Input s.t. change is made in the parameters
void getNumberOfVarsToElimInLayer2Input_With_Recursive_Splitting(DAGNode* Constraint, set<string> &VarsToElim, int &LMIVarsInputToLayer2Local);


// Version of getNumberOfVarsToElimInLayer3Input s.t. change is made in the parameters
void getNumberOfVarsToElimInLayer3Input_With_Recursive_Splitting(DAGNode* Constraint, set<string> &VarsToElim, int &LMIVarsInputToLayer3Local);


// Version of getNumberOfVarsToElimInLayer4Input s.t. change is made in the parameters
void getNumberOfVarsToElimInLayer4Input_With_Recursive_Splitting(DAGNode* Constraint, set<string> &VarsToElim, int &LMIVarsInputToLayer4Local);


// QE from conjunction of LMIs using LME based simplification followed by bit-blasting and CUDD
DAGNode* lmiQEUsingLMEBasedReasoningAndBlasting(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Apply LMEBasedSimplification on weak LMIs
DAGNode* LMEBasedSimplification_ForWeakLMIs(set<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable, DAGNode* &QuantifiedPart);


// Write vector< pair<DAGNode*, DAGNode*> > LMIPairs in file
void writeLMIPairs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs);


// Select the variables for normalizaton
void selectVariablesForNormalization(set<DAGNode*> &Bound_Constraints, set<string> &VarsToElim, set<string> &Normalized, map<string, int> &WidthTable);


// Version of convertToNormalForm for multiple variables
DAGNode* convertToNormalFormForMultipleVariables(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



// Version of lmiQEUsingLMEBasedReasoningAndBlasting which works on dags 
// with LMIs, LMDs, and LMEs
DAGNode* lmiQEUsingLMEBasedReasoningAndBlastingGeneral(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Version of LMEBasedSimplification which is more general
DAGNode* LMEBasedSimplification_General(set<DAGNode*> &E, set<DAGNode*> &D,  set<DAGNode*> &I, set<string> VarsToElim, map<string, int> &WidthTable);


// Extract relevant details of Layer3
void extractLayer3Data(bool z3_finished, set<DAGNode*> &Constraints, set<string> &VarsToElim, set< set<DAGNode*> > &ScopeReducedDAGs, int &LMCCountInInputConjunction, int &VarsCountInInputConjunction, int &VarsToElimCountInInputConjunction, int &PartitionsInOutputConjunction, int &MaxPartitionSizeInOutputConjunction, int &MinPartitionSizeInOutputConjunction, float &AvgPartitionSizeInOutputConjunction);


// Get the relevant details of Project_Blast calls before LME based reasoning
void getRelevantProjectBlastOriginalDetails(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Get the relevant details of Project_Blast calls after LME based reasoning
void getRelevantProjectBlastToBlastDetails(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Initialize the Project_Blast detail
void initializeProjectBlastDetails();


// Get the relevant details of Project calls after LME based reasoning
void getRelevantProjectPostLayer1Details(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Initialize the Project calls details post Layer1
void initializeProjectPostLayer1Details();


// QE from conjunction of LMIs using LME based simplification followed by Omega Test
// Note that there' no return DAG. Just to comapre the performance
void lmiQEUsingLMEBasedReasoningAndOTest(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Version of lmiQEUsingLMEBasedReasoningAndOTest which works on dags 
// with LMIs, LMDs, and LMEs
void lmiQEUsingLMEBasedReasoningAndOTestGeneral(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);




/* Final set of functions for QE from Boolean combinations of LMEs, LMDs, and LMIs */
/* Added on 26 Jan 2012 onwards */

// Calls the function Monniaux_Quantifier_Eliminator_WithSupportForLMIs, 
// and records the time taken by the function
DAGNode* Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForLMIs(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable);


// Calls  Monniaux_Quantifier_Eliminator_WithSupportForLMIs with dynamic programming
DAGNode* callMonniaux_Quantifier_Eliminator_WithSupportForLMIs(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth);


// Function to perform \exists X.\Phi where Phi is a conjunction of LMEs, LMDs, and LMIs
DAGNode* Monniaux_Quantifier_Eliminator_WithSupportForLMIs(DAGNode *F, set<string> &VarSetToElim, map<string, int> &WidthTable, int project_depth);


// calls Project  with dynamic programming
DAGNode* callProject(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);



// Project function which computes \exists X. \pi where X is a set of variables
// and \pi is a conjunction of LMEs, LMDs, and LMIs
DAGNode* Project(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);



// Performs Layer4 on a set of DAGs : Corrected Version
DAGNode* call_applyLMILayer4OnSetOfDAGs(set< set<DAGNode*> > &ScopeReducedDAGs , set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);




// calls Layer4 on a dag  with dynamic programming : Corrected Version
DAGNode* call_applyLMILayer4(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth, unsigned long long int &FMTime, unsigned long long int &NormalizationTime, unsigned long long int &MonTime);






// apply Layer4 on \exists VarsToElim. Phi
// Note that presently only constraints of the form kx1+kx2+...+kxn+d \bowtie kx1+kx2+...+kxn+e
// are handled, where k \in \{ +c, -c \} and \bowtie \in \{<=, => \}. Also
// the Fourier-Motzkin style elimination is applied only when k is odd.
DAGNode* applyLMILayer4(DAGNode* Phi, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth, unsigned long long int &FMTime, unsigned long long int &NormalizationTime, unsigned long long int &MonTime);





// Project_Blast function which computes \exists X. \pi where X is a set of variables
// and \pi is a conjunction of LMEs, LMDs, and LMIs using Layer1 followed by blasting + CUDD
DAGNode* Project_Blast(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);




// Project_OT function which computes \exists X. \pi where X is a set of variables
// and \pi is a conjunction of LMEs, LMDs, and LMIs using Layer1 followed by conversion to ILA + Omega Test
DAGNode* Project_OT(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



// Variant of Project_OT which applies Omega Test after Layer2
DAGNode* applyOTAfterLayer2(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



// Tries to find \exists VarsToElim. (formula) in the following way:
// 1) If VarsToElim = support(formula), then if formula is unsat then \exists VarsToElim. (formula) is unsat, and
//     if formula is sat then \exists VarsToElim. (formula) is valid.
// 2) Else check if \exists VarsToElim. (formula) is valid.
string simpleSMTChecks(DAGNode* formula, set<string> &VarsToElim, map<string, int> &WidthTable);




// Checks if \exists VarToElim. (Phi /\ Xi) \equiv \exists VarToElim. (Phi) using BDDs
bool isExtForLMDsAndLMIsUsingBDDs(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, map<string, int> &WidthTable, unsigned long long int &BlastTimeLocal, unsigned long long int &DDTimeLocal, unsigned long long int &QETimeLocal);


// Function which computes \exists X. \pi using Layer1 and Layer2, 
// where X is a set of variables in \pi
// and \pi is a conjunction of LMEs, LMDs, and LMIs
// Helps in solving
DAGNode* Layer1AndLayer2(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth);

#endif	/* _DAGELIMINATOR_H */

