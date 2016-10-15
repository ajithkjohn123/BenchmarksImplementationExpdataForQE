


/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#ifndef _ELIMINATOR_H
#define	_ELIMINATOR_H


#include "math.h"
#include "DagManager.h"
#include "Value.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <sys/time.h>
	
using namespace std;

extern DAGManager dm;

extern char* qEliminator_type;
extern FILE *ImpCheckFile;

extern FILE *layer1_calls_fp;
extern FILE *layer2_calls_fp;

extern FILE *apqelim_fp;

typedef unsigned long long int ULINT;

/* Variables to print details of apqelim */
extern FILE *APQElimStatusFile;
extern bool apqelim_status_on;
extern unsigned long long int Multivar_APQElim_Calls;
extern unsigned long long int APQElim_Calls;
extern unsigned long long int APQElim_Calls_Solved_By_QElimLMA;
extern unsigned long long int APQElim_Calls_Solved_By_DropLMDs_1;
extern unsigned long long int APQElim_Calls_Solved_By_DropLMDs;
extern unsigned long long int APQElim_Calls_Given_To_APQElim_Exp; 
extern unsigned long long int Max_LMEs;
extern unsigned long long int Min_LMEs;
extern unsigned long long int Max_LMDs;
extern unsigned long long int Min_LMDs;
extern time_t time_in_layer1;
extern time_t time_in_layer2;
/* Variables to print details of apqelim ends */

// Details needed in Layered..txt, Mon...txt, etc.
extern int LMECount_In_DAG;
extern int LMDCount_In_DAG;
extern int BitLevel_LMDCount_In_DAG;

// For Layer - I

extern int L1_Calls;
extern time_t L1_Total_Time;
extern time_t L1_Max_Time;
extern time_t L1_Min_Time;

extern int L1_Total_Bitwidth_of_Var_Eliminated;
extern int L1_Max_Bitwidth_of_Var_Eliminated;
extern int L1_Min_Bitwidth_of_Var_Eliminated;

extern int L1_Calls_with_Gt_Zero_Diseqs;

extern int L1_Total_Diseqs_for_Calls_with_Gt_Zero_Diseqs;
extern int L1_Max_Diseqs_for_Calls_with_Gt_Zero_Diseqs;
extern int L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs;

extern float L1_Total_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs;
extern float L1_Max_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs;
extern float L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs;

extern time_t L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs;
extern time_t L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs;
extern time_t L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs;

extern time_t L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs;
extern time_t L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs;
extern time_t L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs;

// For Layer - II

extern int L2_Calls;
extern time_t L2_Total_Time;
extern time_t L2_Max_Time;
extern time_t L2_Min_Time;

extern int L2_Total_Bitwidth_of_Var_Eliminated;
extern int L2_Max_Bitwidth_of_Var_Eliminated;
extern int L2_Min_Bitwidth_of_Var_Eliminated;

extern int L2_Calls_with_Zero_Eqs;
extern int L2_Calls_with_Eqs;

extern int L2_Total_Diseqs;
extern int L2_Max_Diseqs;
extern int L2_Min_Diseqs;

extern int L1_Total_Eqs_for_Calls_with_Gt_Zero_Diseqs;
extern int L1_Max_Eqs_for_Calls_with_Gt_Zero_Diseqs;
extern int L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs;

extern int L1_Total_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs;
extern int L1_Max_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs;
extern int L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs;

// Details needed in Layered..txt, Mon...txt, etc. ends here

// Declarations for timeout
extern time_t time_out;
extern time_t time_out_start;
extern bool timed_out;
extern bool time_out_enabled;
// Declarations for timeout ends here

// The time details for layers in milliseconds
extern unsigned long long int L1_Total_Time_In_ms;
extern unsigned long long int L1_Max_Time_In_ms;
extern unsigned long long int L1_Min_Time_In_ms; 
extern unsigned long long int L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms;
extern unsigned long long int L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms;
extern unsigned long long int L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms;
extern unsigned long long int L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms;
extern unsigned long long int L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms;
extern unsigned long long int L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms;
extern unsigned long long int L2_Total_Time_In_ms;
extern unsigned long long int L2_Max_Time_In_ms;
extern unsigned long long int L2_Min_Time_In_ms;
// The time details for layers in milliseconds end here

// Declarations for comparing with OmegaTest
extern time_t Total_Omega_Test_Time;
extern time_t Total_AP_QElim_Time;
extern bool compare_apqelim_omegatest;
extern unsigned long long Total_Omega_Test_Time_In_ms;
extern unsigned long long Total_AP_QElim_Time_In_ms;
extern int L1_Total_Eqs;
extern int L1_Max_Eqs;
extern int L1_Min_Eqs;
extern int L1_Total_Diseqs;
extern int L1_Max_Diseqs;
extern int L1_Min_Diseqs;
extern int L1_Total_Vars;
extern int L1_Max_Vars;
extern int L1_Min_Vars;
// Declarations for comparing with OmegaTest end here

// Declarations for comparing with Bit-blasting
extern bool compare_apqelim_blasting;
extern unsigned long long int Total_Blasting_Time_In_ms;
extern time_t Total_Blasting_Time;
extern bool bitblasting_with_scopereduction;
extern string InFileName;
extern char* Elimination_Type;
extern char* Dag_Eliminator_Type;
// Declarations for comparing with Bit-blasting

extern string internal_solver;

// For details of QE for bit-vector constraints
extern int EquationCount_In_DAG;
extern int DisequationCount_In_DAG;
extern int BitLevel_DisequationCount_In_DAG;
extern int NonBitLevel_DisequationCount_In_DAG;
extern int EquationWithSelsConcatsCount_In_DAG;
extern int BitLevel_DisequationWithSelsConcatsCount_In_DAG;
extern int NonBitLevel_DisequationWithSelsConcatsCount_In_DAG;
extern int ImpureVarsToQElimCount;
extern int PureVarsToQElimCount;
// For details of QE for bit-vector constrainsts end here

// For performance analysis
extern bool log_for_per_analysis;
extern unsigned long long int L1_L2_Total_Time_In_ms_For_Conjunctions;
extern unsigned long long int dag2matrix_Total_Time_In_ms_For_Conjunctions;
extern unsigned long long int matrix2dag_Total_Time_In_ms_For_Conjunctions;

// For LMI handling
extern time_t getModel_time;
extern bool LMI_handling_enabled;
extern int WidthInteger;
extern unsigned long long int ModWidthInteger; // 2^(WidthInteger)
extern unsigned long long int ModWidthIntegerHalf; // (2^(WidthInteger))/2

extern int LMIMonniauxCounter;
extern int FMTypeConjunctionCounter;
extern int AdditionalConjunctionCounter;


extern FILE* LMIQEImportantDataFourierMotzkinFp;// output file for printing contributions of different layers for the Fourier-Motzkin type calls
extern FILE* LMIQEImportantDataAdditionalFp;// output file for printing contributions of different layers for the calls made from Fourier-Motzkin type calls
extern FILE* LMIQEOutputDataFp;// output file for printing relevant results for the LMI case
extern FILE* LMIQEImportantDataFp;// output file for printing contributions of different layers


extern bool SwitchOffLayer1;
extern bool SwitchOffLayer2;

extern bool order_lmes_before_lmis_in_normalization;
extern bool order_lmcs_for_small_lmdds_in_normalization;

extern time_t Time_in_SMT_calls;
extern bool pure_lmdd_mode;
extern bool local_constraint_as_single_edge_mode;

extern unsigned long long int total_generalization_and_projection_time;
extern float avg_generalization_and_projection_time;
extern unsigned long long int project_calls_in_path_based_strategy_in_lmdd_monniaux;
extern unsigned long long int total_vars_to_elim_in_projects;
extern float avg_vars_to_elim_in_projects;

extern set<string> CopyOfVarSetToElim;

extern bool PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles;
extern time_t TimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles;
extern time_t TimeOutStart_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles;
extern bool TimedOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles;
extern time_t TimeSpentInMonniaux_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles;


extern bool SimpleSMTChecksBeforeLayer3;

extern bool Layer2CheckUsingBDDs;
extern unsigned long long int TotalLayer2CheckTime;
extern FILE* TimeLayer2ChecksFp;
extern FILE* OutputDataLayer2ChecksFp;
extern unsigned long long int TotalLayer2CheckTimeOnDDManipulation;
extern unsigned long long int TotalLayer2CheckTimeONQE;
extern unsigned long long int TotalLayer2Checks;
extern FILE* CumulativeTimeLayer2ChecksFp;
extern bool PrintLayer2Times;
extern bool PrintBlastTimes;
extern unsigned long long int TotalLayer2CheckTimeOnBlasting;
extern unsigned long long int TotalLayer2CheckTimeOnDDCreation;
// For LMI handling ends here


typedef vector< vector<ULINT> > Matrix;
typedef vector<ULINT> Row; 


// Declarations of functions defined in DAGEliminator.cpp

bool isExtForLMDsAndLMIsUsingBDDs(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, map<string, int> &WidthTable, unsigned long long int &BlastTimeLocal, unsigned long long int &DDTimeLocal, unsigned long long int &QETimeLocal);

// Declarations of functions defined in DAGEliminator.cpp ends here


// DECLARATIONS OF THE FUNCTIONS FOR MATRIX LEVEL ELIMINATION

// return Additive Inverse of the first input modulo the second input
ULINT aInverse(ULINT number, ULINT mod);

// return Multiplicative Inverse of the first input modulo the second input
ULINT mInverse(ULINT number, ULINT mod);

// return a+b modulo mod
ULINT add(ULINT a, ULINT b, ULINT mod);

// return a-b modulo mod
ULINT sub(ULINT a, ULINT b, ULINT mod);

// return a*b modulo mod
ULINT mul(ULINT a, ULINT b, ULINT mod);

// return a/b modulo mod. Note than a/b mod is defined only if b has a MI.i.e. if b is odd
ULINT div(ULINT a, ULINT b, ULINT mod);

// Show the matrix
void showMatrix(Matrix &MyMatrix);

// Get a matrix from key board
void getMatrix(Matrix &MyMatrix, int rows, int cols);

// Show Equalities 
void showEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix);

// Nullify column indexed by  SelColumn 
void nullifyColumn(Matrix &A, int SelColumn);

//  Function to push back the the selectedIndexth row into NewMatrix.
void copyRow(Matrix &MyMatrix, int selectedIndex, Matrix &MyRow);

//  Function to copy the the selectedIndexth column into MyCoulmnMatrix. Initially MyColumnMatrix is empty
void copyColumn(Matrix &MyMatrix, int selectedIndex, Matrix &MyColumnMatrix);

//Convert MyMatrix to AI(MyMatrix)
void aInverseOfMatrix(Matrix &MyMatrix, ULINT Mod);

// Convert MyMatrix to MyMatrix*Scalar
void scalarMatMultiply(Matrix &MyMatrix, ULINT Scalar, ULINT Mod);

// Multiply matrixes A and B to get C
void matMultiply(Matrix &A, Matrix &B, Matrix &C, ULINT Mod);

// A = A + B where A, B are matrixes
void matAdd(Matrix &A, Matrix &B, ULINT Mod);

// A = A - B where A, B are matrixes
void matSub(Matrix &A, Matrix &B, ULINT Mod);

// Delete a row from matrix indexed by Index
void deleteRow(Matrix &A, int Index);

// Delete a column from matrix indexed by Index
void deleteColumn(Matrix &A, int Index);

// return the power of 2 in number. Eg: if number=4, return 2
ULINT powerOfTwo(ULINT number);

// return 2 ^ number
ULINT findPower(ULINT number);

// Add RowMatrix as a new row into MyMatrix
void addRow(Matrix &MyMatrix, Matrix &RowMatrix);

// Here MyMatrix is a column matrix. This function adds element as the last element of MyMatrix
void addElementToColumnMatrix(Matrix &MyMatrix, ULINT element);

// Check if MyMatrix is a null matrix
int IsNullMatrix(Matrix &MyMatrix);


// Given matrix A and mod, generate matrix B s.t. element in B = mod - corresponding element in A
void convertMatrixToDifferenceOfPowerOf2(Matrix &A, ULINT mod, Matrix &B);

// Check if selected equation is solvable
int equationSolvable(Matrix &MyMatrix, ULINT Const, ULINT Mod);

// Find GCD of a set of numbers
ULINT findGCD(set<ULINT> &numbers);

// return gcd of a and b
ULINT GCD(ULINT a, ULINT b);

//  Function to copy the MyMatrix into MyMatrixCopy
void copyMatrix(Matrix &MyMatrix, Matrix &MyMatrixCopy);

// Show the disequations
void showDisEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix);

// Divide column indexed by  SelColumn by power(2,SelectedPower) if divisible. If not divisible set to zero
void divideColumnByPowerOf2IfDivisible(Matrix &EqCoefMat, int SelColumn, ULINT SelectedPower);

// Search MyVector and return index of element
int searchVector(vector<int> &MyVector, int element);

// Get the index of the equation with any odd coef for variable indexed by VarToElimIndex
void getEquationWithOddCoefForGivenVariable(Matrix &EqCoefMat, int VarToElimIndex, int &EqnToSubstIndex);

// Get the index of the equation with least even coef for variable indexed by VarToElimIndex
void getEquationWithLeastEvenCoefForGivenVariable(Matrix &EqCoefMat, int VarToElimIndex, int &EqnToSubstIndex, ULINT Mod);

// In the column indexed by SelColumn if the entry is divisible by power(2,SelectedPower),
// then make it zero (we have eliminated the variable indexed by SelColumn from it), else keep it
void modifyDisEqCoefMat(Matrix &DisEqCoefMat, int SelColumn, ULINT SelectedPower);


// Get the matrix which is the element-wise maximum of matrices in arg1 and arg2
void getMaximumMatrix(Matrix &A, Matrix &B, Matrix &C);

// Declaration of fns for dropping disequalities
void dropUnconstrainingDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod) ;


//  Function to copy the the non-zero elements in the selectedIndexth column into MyMap. Initially MyMap is empty. 
// first field of MyMap is the row index and the second field is the element MyMatrix[first field][selectedIndexth]
// if MyMatrix[first field][selectedIndexth] != 0
void copyColumnToMap(Matrix &MyMatrix, int selectedIndex, map<int, ULINT> &MyMap);


// Function to convert the mapped elements of MyMap to powers of 2. 
void convertCoefToPowerOf2(map<int, ULINT> &MyMap);

// Function to convert the mapped elements of MyMap to number-element
void subtractMappedElementsFromNumber(map<int, ULINT> &MyMap, ULINT number);

// Function to convert the mapped elements of MyMap to element - number
void subtractNumberFromMappedElements(map<int, ULINT> &MyMap, ULINT number);

// Function to find the maximum of the mapped elements of MyMap
ULINT getMaximumOfMappedElement(map<int, ULINT> &MyMap);


// Function to show MyMap
int showMap(map<int, ULINT> &MyMap);


// Eliminate entries in MyMap with mapped element = zero
void eliminateEntriesWithZeroMappedElements(map<int, ULINT> &MyMap);

// Function to find the minimum of the mapped elements of MyMap
ULINT getMinimumOfMappedElement(map<int, ULINT> &MyMap);


// Return the value of bit at location loc in the value count
ULINT getTypeOfConstraint(ULINT count, ULINT loc);


// Add all the rows in YourMatrix as a new rows into MyMatrix
void addRows(Matrix &MyMatrix, Matrix &YourMatrix);


// Eliminate the disequations free of the variable to be eliminated from the disequations in
// DisEqCoefMat, DisEqConstMat, and copy them in DisEqFreeOfVarToElimCoefMat, DisEqFreeOfVarToElimConstMat
void eliminateDisequationsFreeOfVarToElimAndCopyInNewMatrix(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int VarToElimIndex, Matrix &DisEqFreeOfVarToElimCoefMat, Matrix &DisEqFreeOfVarToElimConstMat);



// return true if the column indexed by index is all zeros in A
bool columnIsNull(Matrix &A,int index);


// This function finds the disequations in which there are no variables left to eliminate, removes them from set of disequations and copies them in a new matrices
void eliminateDisequationsFreeOfAllVarsToElimAndCopyInNewMatrix(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarToElim, Matrix &DisEqFreeOfAllVarsToElimCoefMat, Matrix &DisEqFreeOfAllVarsToElimConstMat);

// show MyVector
void showVector(vector<int> &MyVector);

// Print Equalities
void printEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix, FILE *fp);

// Print the disequations 
void printDisEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix, FILE *fp) ;


// The function which implements the Layer 1 in CAV'11 paper
void qEliminatorForEquationsAndDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod);


// Get two matrices from an input file : coefficient and constant matrices
// Both have same no: of rows and no: of columns of coefficient matrix is cols
// constant matrix has 1 column : But constant is (last entry - second last entry)
void getEquations(Matrix &MyMatrixCoef, Matrix &MyMatrixConst, int rows, int cols, ULINT Mod);



// Function to eliminate a single variable from a conjunction of LMEs and LMDs.
// Returns the result as a DAG
DAGNode* qEliminatorReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable);


// The function which implements the LME based splitting scheme described in the CAV'11 paper
// Returns the result as a DAG
DAGNode* splitDisequationsReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable);


// Creates a dag from a conjunction of disequalities specified as matrices, and returns it
DAGNode* getDAGFromDisEqs(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, list<string> &VarNames, ULINT Mod);


// Creates a dag from a conjunction of equalities specified as matrices, and returns it
DAGNode* getDAGFromEqs(Matrix &EqCoefMat, Matrix &EqConstMat, list<string> &VarNames, ULINT Mod); 


// Creates a dag from an equality specified as matrices, and returns it
DAGNode* getEquationDAGFromEqn(ULINT SelectedCoef, Matrix &SelectedEqn, ULINT SelectedConst, list<string> &VarNames, string VarToElim, ULINT Mod);


// Given number in form 2^k*c return c Eg: if number=4, return 1
ULINT getOddPart(ULINT number);

// This function eliminates duplicate entries from a set of equations/disequations and recreates a new set of constraints
void deleteDuplicateConstraints(Matrix &CoefMat, Matrix &ConstMat, Matrix &NewCoefMat, Matrix &NewConstMat);


// This function adds a new constraint into the set of constraints
void addConstraint(Matrix &NewCoefMat, Matrix &NewConstMat, Row &LHS, ULINT RHS);


// Checks if the constraint LHS op RHS is already present in the set of constraints
bool constraintPresent(Matrix &NewCoefMat, Matrix &NewConstMat, Row &LHS, ULINT RHS);


// Check if two rows are the same
bool rowsEqual(Row &row1, Row &row2);


// Eliminates trivial LMEs and LMDs from the given conjunction of LMEs and LMDs
void eliminateTrivialEquationsAndDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat);


// return true if the row indexed by index is all zeros in A; false otherwise
bool rowIsNull(Matrix &A,int index);


// Function which implements Layer 3 in CAV'11 paper. This takes a conjunction of LMEs and LMDs
// and drops LMDs which are implied by the conjunction of LMEs and the remaining LMDs
void eliminateImpliedDisequations(Matrix EqCoefMat, Matrix EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, ULINT Mod, map<string, int> WidthTable, list<string> VarNames);


// Check if the LMD at the row SelectedIndex is implied by LMEs and the other LMDs. Return true if this happens; false otherwise
bool disEquationImpliedByOthers(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, ULINT Mod, map<string, int> WidthTable, int SelectedIndex, list<string> VarNames);


// Function to write the declarations of the variables in WidthTable which are appearing in the list "VarNames",
// in Yices format in the file yices_file
void writeBVDeclarationsInYicesFormatWithLookup(FILE *yices_file, map<string, int> WidthTable, list<string> VarNames);


// Function to get a rows X cols null matrix in NullMatrix
void getNullMatrix(Matrix &NullMatrix, int rows, int cols);


// Function to get a rows X cols unit matrix in UnitMatrix
void getUnitMatrix(Matrix &UnitMatrix, int rows, int cols);


// Makes destination = Source1 concatenat Source2 
// destination is rows X cols matrix where Source1 is a rows X cols1 
// and Sourc2 is a rows X cols2 matrix and cols = cols1 + cols2
void concatenateMatrices(Matrix Source1, Matrix Source2, Matrix &Destination);


// Checks how many non-zero entries are there in the matrix MyMatrix
// Returns the non-zero element if there is only a single non-zero element
// returns -1 otherwise
int getSingleNonZeroEntry(Matrix MyMatrix);


// Get two matrices from an input file : coefficient and constant matrices
// Both have same no: of rows and no: of columns of coefficient matrix is cols
// constant matrix has 1 column : But constant is (last entry - second last entry)
void getDisequations(Matrix &MyMatrixCoef, Matrix &MyMatrixConst, int rows, int cols, ULINT Mod);



// The final version of the function to drop unconstraining LMDs. i.e. function implementing Layer 2 of the CAV'11 paper
void dropUnconstrainingDisequationsModified(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod);


//  Function to copy the zero elements in the selectedIndexth column into CoreLMDs. Initially CoreLMDs is empty
void initializeCoreLMDs(Matrix &MyMatrix, int selectedIndex, set<int> &CoreLMDs);


// Given map<int, ULINT> CoefsOfVarToElimEqualities, it returns minium of the second field
// of CoefsOfVarToElimEqualities if CoefsOfVarToElimEqualities is non-empty, and powerOfTwo(Mod)
// if CoefsOfVarToElimEqualities is empty
ULINT findMinimumKValue(map<int, ULINT> &CoefsOfVarToElimEqualities, ULINT Mod);



//Eliminates unconstraining LMDs. Called by dropUnconstrainingDisequationsModified
void eliminateDisequalitiesModified(ULINT KEqn, map<int, ULINT> &KValuesForDisEquations, set<int> &CoreLMDs);


// returns true if set A = set B; false otherwise
bool setsEqual(set<int> &A, set<int> &B);


// Function getLeastConstrLMDs in CAV'11 paper.
// Given a set of LMDs D, and set of LMDs core,
// returns the least constrained set of LMDs from D
set<int> getLeastConstrLMDs(set<int> &D, set<int> &core, map<int, ULINT> &KValuesForDisEquations);



// The final version of isExt in CAV'11 paper
bool isExtModified(set<int> &core, set<int> &D, ULINT KEqn, map<int, ULINT> &KValuesForDisEquations);


// FUNCTIONS TO PERFORM DAG-LEVEL OPERATIONS


// Functions to create a dagnode with label, children, data_type, and width as specified
DAGNode* createDAG(string name, vector<DAGNode *> children, int data_type, int width);
DAGNode* createDAG(string name, DAGNode *child1, int data_type, int width);
DAGNode* createDAG(string name, DAGNode *child1, DAGNode *child2, int data_type, int width);
DAGNode* createDAG(string name, DAGNode *child1, DAGNode *child2, DAGNode *child3, int data_type, int width);


// Function to create a dagnode for a bit-vector variable with label and width as specified
DAGNode* createBVVariable(string name, int width);


// Function to create a dagnode for a bit-vector constant with label and width as specified
DAGNode* createBVConstant(string constant, int width);


// Function to create a dagnode for a logical variable with label as specified
DAGNode* createLogicalVariable(string name);


// Function to create a dagnode for a logical constant with label as specified
DAGNode* createLogicalConstant(string constant);


// Function to return the content of the dag as a string
string getDAGStr(DAGNode *node);


// Function to check if the dag node "parent" has propositional structure
// i.e. the predicates should appear only at the leaves
// exit from the program otherwise
void checkForPropSkelton(DAGNode* parent);


// return true if MyName is a logical operator or constant; false otherwise
bool Logical(string MyName);


// return true if MyName is a logical operator; false otherwise
bool LogicalOperator(string MyName);


// Function to find the support of the dag in the parameter node
// Example : if node is x+y+z, function should make names = {x, y, z}
void getDAGNames(DAGNode *root, set<string> &names);


// Function called from getDAGNames to get the support : note that this function
// works with dynamic programming
void getDAGNamesInternal(DAGNode *node, set<string> &names, set<DAGNode*> &Encountered);


// Function called from getDAGNames to get the support
void getDAGNamesInternalNoDP(DAGNode *node, set<string> &names);


// return 1 if element appears in MyList; false otherwise
int searchList(list<string> MyList, string element);


// append the content of MySet to MyList
void attachSetToList(set<string> MySet, list<string> &MyList);


// return the #bits in the constraint root
int getNoOfBitsInConstraint(DAGNode* root, map<string, int> &WidthTable);


// convert a number specified as string into ULINT, and return it
ULINT convertLiteralToNumber(string literal);


// convert a hexadecimal number to decimal (ULINT) and return it
ULINT convertToDecimal(string x);


// Given c1.x1 + .... + cn.xn + c0, get c0, and put it in constant
void getConstantModified(DAGNode* node, ULINT &constant);


// Given c1.x1 + .... + cn.xn + c0, and xi, get ci, and put it in Coefficient
void getCoefficient(DAGNode *node, string Var, ULINT &Coefficient);


// Given an LME, recreate it as 2^k.x = t, where x is the first one in the lexicographic
// ordering, and t is free of x
DAGNode* recreateEquationInProperForm(DAGNode* equation, map<string, int> &WidthTable);


// Function to return the content of a dag (in propositional skeleton format) in Yices format
// Note that this function exits if the argument dag has concats in it.
// If the argument dag has concats in it, please use the function 
// callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats
string callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(DAGNode *root, map<string, int> WidthTable);


// Function to create a table WidthTableForNodes, which maps a dag node to its width
void createWidthTableForOperators(DAGNode *node, map<DAGNode*, int> &WidthTableForNodes, map<string, int> WidthTable);


// Find maximum of i and j
int findMaximum(int i, int j);


// return the minimum width required to represent a Number
// Number is a string here
int findMinimumWidthRequired(string Number);


// return the minimum width required to represent a Number
// Number is ULINT here
int findMinimumWidthRequiredForNumber(ULINT number);


// Function to adjust the widths in WidthTableForNodes. This function parses the dagnode "node", and
// changes the entries in WidthTableForNodes according to the contents of "node"
void propogateWidths(DAGNode *node, map<DAGNode*, int> &WidthTableForNodes, int WidthFromParent);


// Function to traverse the dag node "node" and return the content of a dag 
//(in propositional skeleton format) in Yices format
// This is called by callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable
string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required);


// Function to return the 4-bit binary representation, given a hexadecimal
// number in parameter "c"
string getBinaryOfChar(char c);


// Function to convert a hex string into Yices binary format, and return it
string convertHexToBitvectorWithProperWidth(string hex, int Width);


// Function to convert a hex string into a binary string
string hexToBinaryString(string hex);


// convert a literal which is '1' or '0' into Yices format, and return it
string convertLiteralToBitvectorWithProperWidth(string literal, int Width);


// Function to convert a decimal string into Yices binary format, and return it
string convertDecimalToBitvectorWithProperWidth(string dec, int Width);

// Function to convert a decimal number into a binary string
string integerToBinaryString(ULINT i);

// Function to write the declarations of the variables in WidthTable in Yices format in the file "smt_file"
void writeBVDeclarationsInYicesFormat(FILE *smt_file, map<string, int> WidthTable);


// returns true if f is unsat; false otherwise
// In future, use this function whenever there is a need to check
// satisfiability of an arbitrary formula
bool ThUnsatSingleFormula(DAGNode* f, map<string, int> WidthTable);


// returns true if the dag node "node" is free of the variable VarToElim; false otherwise
bool freeOfVarModified(DAGNode* node, string VarToElim);



// Function to show a given set of integers
void showSet(set<int> &MySet, string WhoAmI);


// Function to show a given set of strings
void showSet(set<string> &MySet, string WhoAmI);


// shows the list of string "names"
void showList(list<string> names, string WhoAmI);

// returns true if the dag node "predicate" is free of a set of variables DropVars; false otherwise
bool freeOfVars(DAGNode *predicate, set<string> &DropVars);


// returns true if exists(X, original) => eliminated i.e. original \and \neg eliminated is unsat; false otherwise
bool checkForExactnessInOneDirection(DAGNode* original, DAGNode* eliminated, map<string, int> WidthTable);


// Here "node" represents a propositional logic formula
// This function returns the content of "node" as a string in Yices format
string getBooleanDagAsString(DAGNode *node, string context);


// Updates the set VarSetToElim depending on the content of the node "parent"
void updateVarSetToElim(set<string> &VarSetToElim, DAGNode *parent, string new_name);


// returns true if "parent" is a non-linear multiplication; false otherwise
bool nonLinearMult(DAGNode *parent);


// linearize the dag in "parent". i.e. replace the nodes containing non-linear operations such as
// concat, select etc. by dag nodes which are fresh variables, and then return the linearized dag
// This function does not support LMIs
DAGNode* getLinearizedClone(DAGNode* parent, map<DAGNode*, DAGNode*> &LinearizationTable, int &count, map<string, int> &WidthTable, set<string> &VarSetToElim);


// Version of getLinearizedClone with support for LMIs
DAGNode* getLinearizedCloneWithLMISupport(DAGNode* parent, map<DAGNode*, DAGNode*> &LinearizationTable, int &count, map<string, int> &WidthTable, set<string> &VarSetToElim);


// return the negation of the LMC
// eg: negateConstraint(x+y=z) is x+y<>z, and vice-versa
DAGNode* negateConstraint(DAGNode* C);


// Find the width of the node "node" and return it
int getWidthOfNode(DAGNode *node, map<string, int> &WidthTable);


// Function to update the set VarSetToElim. This function removes the impure
// variables i.e. variables which cannot be eliminated by the CAV'11 paper based
// work from the set VarSetToElim
void updateVarSetToElim(set<string> &VarSetToElim, DAGNode *parent);



// Function newly added on 14th June 2010 onwards

// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// and a number shift, create a copy of parent with indexes of the variable nodes changed
// as new index = existing index + shift
// This is used in iterative squaring
DAGNode* cloneAndShift(DAGNode* parent, int shift);


// This function extracts the APs from a DAG node "parent"
// APs from the condition part of the ites are inserted before the APs from the then and the else parts
void getAPsForDAGInPropSkelton(DAGNode* parent, list<DAGNode*> &APList);


// Shows a list of dag nodes
void showAPList(list<DAGNode*> &APList);


// Check if a dag node is already present in the APList. 
// Return true if yes; false otherwise
bool searchAPList(list<DAGNode*> &APList, DAGNode *target);


// checks if the given node is true
bool isTrue(DAGNode *node);


// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// create a copy of parent with indexes of the variable nodes changed
// as new index = 2 if existing index = 1, and new index = existing index otherwise.
// This is used in iterative squaring
DAGNode* cloneWithRenamedDeltas(DAGNode* parent);



// Called by recreateEquationInProperForm to recreate an LME as 2^k.x = t, 
// where x is the first one in the lexicographic
// ordering, and t is free of x 
DAGNode* recreateDAGFromEqs(Matrix EqCoefMat, Matrix EqConstMat, list<string> VarNames, ULINT Mod);



// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// create a copy of parent with indexes of the variable nodes changed
// based on the DeltaMap
// NB : Note that a version of this function cloneWithRenamedDeltasWithDeltaMap,
// making use of dynamic programming is avaiable
DAGNode* createShiftedLMEBasedOnDeltaMap(DAGNode* parent, map<int, int> &DeltaMap);


// Function to write declarations in SMTLib format
void writeBVDeclarationsInSMTLibFormat(FILE *smt_file, map<string, int> WidthTable);


// Function to return the content of a dag (in propositional skeleton format) in SMTLIB format
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(DAGNode *root, map<string, int> WidthTable);


// Function to traverse the dag node "node" and return the content of a dag (in propositional skeleton format) in SMTLIB format
// This is called by callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required);


// Function to convert a decimal string into SMTLIB binary format, and return it
string convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(string dec, int Width);


// Function to convert a hex string into SMTLIB binary format, and return it
string convertHexToBitvectorWithProperWidthInNewSMTLibFormat(string hex, int Width);


// Function to convert a literal into SMTLIB binary format, and return it
string convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(string literal, int Width);


// Function to do QELIM from a conjunction of LMEs and LMDs using Layer 1 for deciding sat of the conjunction of LMEs and LMDs. 
// Should be called with  NoVarsToElim equal to number of variables
// Returns 'u' if unsat, 's' if sat, 'x' if unknown
char solveForEquationsAndDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod);


// check if the set of LMEs and LMDs is sat/unsat syntactically (trivial check)
// Returns 'u' if unsat, 's' if sat, 'x' if unknown
char syntacticCheckForSatisfiability(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat);


// check if the set of LMEs sat/unsat syntactically (trivial check)
// Returns true if unsat, false if sat/unknown
bool setOfEquationsSyntacticallyUnsat(Matrix &MyCoefMatrix, Matrix &MyConstMatrix);


// check if the set of LMDs sat/unsat syntactically  (trivial check)
// Returns true if unsat, false if sat/unknown
bool setOfDisequationsSyntacticallyUnsat(Matrix &MyCoefMatrix, Matrix &MyConstMatrix);


// Function to do QELIM from a conjunction of LMEs and LMDs for deciding sat of the conjunction of LMEs and LMDs. 
// Should be called with  NoVarsToElim equal to number of variables
// This uses only Layers 1&2 as the deeper layers are expensive
// Returns 'u' if unsat, 's' if sat, 'x' if unknown
char qEliminatorForSolving(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod);


// Given map<int, ULINT> CoefsOfVarToElimEqualities, it returns maximum of the second field
// of CoefsOfVarToElimEqualities if CoefsOfVarToElimEqualities is non-empty, and zero
// if CoefsOfVarToElimEqualities is empty
ULINT findMaximumKValue(map<int, ULINT> &CoefsOfVarToElimDisEqualities);




// Implements synthesizeLMEs in CAV'11 paper
// This synthesizes new LMEs from the given problem instance, which simplifies the given instance
// Returns true if eager splitting is not required; false otherwise
bool bitLevelReasoning(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int VarToElimIndex, ULINT Mod);


// returns true if there exists an LME e in E s.t. e is like 2.lhs = 2.rhs, and LMD d is like lhs <> rhs; false otherwise
bool constrainingEquationExists(Matrix &EqCoefMat, Matrix &EqConstMat, Row &DisEqCoef, ULINT DisEqConst, ULINT Mod);


// called by constrainingEquationExists to check if LME e is like 2.lhs = 2.rhs, and LMD d is like lhs <> rhs.
// returns true if exists; false otherwise
bool equationPairOfDisequation(Row &EqCoef, ULINT EqConst, Row &DisEqCoef, ULINT DisEqConst, ULINT Mod);



// Function to implement the eager splitting scheme in CAV'11 paper
DAGNode* eagerSplitReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable);



// show a row matrix
void showRow(Row &MyRow);


// return x ^ number
ULINT findPowerOfAnyNumber(ULINT x, ULINT number);


// Function which actually does the eager splitting
DAGNode* eagerSplitInternalReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable);



// This function finds the LMDs free of the variable to be eliminated before splitting,
// and copies them in DisEqFreeOfSplittingCoefMat, DisEqFreeOfSplittingConstMat
void eliminateDisequationsFreeOfSplittingAndCopyInNewMatrix(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int VarToElimIndex, Matrix &DisEqFreeOfSplittingCoefMat, Matrix &DisEqFreeOfSplittingConstMat, ULINT KEqn);



// Return the value of bit at location loc in the value count when expressed with base as Base
ULINT getTypeOfConstraintForEagerSplitting(ULINT count, ULINT loc, ULINT Base);



// change the global variables Max_LMEs, Min_LMEs, Max_LMDs, and Min_LMDs
// using NoOfEqs and NoOfDiseqs
void getLME_LMD_Counts(int NoOfEqs,int NoOfDiseqs);


// check if there exists LMDs which contain any of the variables to be eliminated
// returns true if exists; false otherwise
bool findEliminationDone(Matrix DisEqCoefMat, int NoVarsToElim);


// updates the global variable APQElim_Calls
void update_APQElim_Calls();


// updates the global variable Multivar_APQElim_Calls
void update_Multivar_APQElim_Calls();


// updates the global variable APQElim_Calls_Solved_By_QElimLMA
void update_APQElim_Calls_Solved_By_QElimLMA(Matrix DisEqCoefMat, int NoVarsToElim);


// updates the global variable APQElim_Calls_Solved_By_DropLMDs_1
void update_APQElim_Calls_Solved_By_DropLMDs_1(Matrix DisEqCoefMat, int NoVarsToElim);


// updates the global variable APQElim_Calls_Solved_By_DropLMDs
void update_APQElim_Calls_Solved_By_DropLMDs(Matrix DisEqCoefMat, int NoVarsToElim);


// updates the global variable APQElim_Calls_Given_To_APQElim_Exp
void update_APQElim_Calls_Given_To_APQElim_Exp(Matrix DisEqCoefMat, int NoVarsToElim);


// Splits the dag node which is a conjunction of APs into a set of APs in Conjuncts   
void splitConjunctionIntoSet(DAGNode* Constraint, set<DAGNode*> &Conjuncts);


// show a set of DAGNodes
void showAPSet(string WhoamI, set<DAGNode*> &APSet);



// Given a set of DAGNodes, which are LMCs, convert them into matrix representation
// A pair of matrices for LMEs, and another pair for LMDs is generated
bool getEquationsAndDisequations(set<DAGNode*> &Constraints, list<string> &VarNames, int &NoOfBits, int &NoOfEqs, int &NoOfDisEqs, Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, map<string, int> &WidthTable);



// Given a set of LMCs and a variable to be eliminated, this function first converts the 
// LMCs into matrix representation, and then calls the matrix based QEliminator, and 
// returns the result as a DAGNode*
DAGNode* callMatrixQEliminator(set<DAGNode*> &Constraints, string VariableToElim, map<string, int> &WidthTable);



// Given a set of LMCs in "Constraints" and a variable to be eliminated, generate two sets of constraints - 
// A set of constraints free of the variable to be eliminated and another set of constraints which contain
// the variable to be eliminated
void getFreeAndBoundConstraints(set<DAGNode*> &Constraints, string Variable, set<DAGNode*> &Free_Constraints, set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable);



// Given a set of LMCs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode*
DAGNode* APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> &WidthTable);



// Split a cojunction of LMCs in "Constraint" into a set of LMEs E and a set of LMDs D
void splitConjunctionIntoSetsWithSupportForNot(DAGNode* Constraint, set<DAGNode*> &E, set<DAGNode*> &D);



// Function to simplify the dag node "node" based on some simple syntactic rules
DAGNode*  quickSimplify(DAGNode *node);


// Function called by quickSimplify to simplify the dag node "node" based on some simple syntactic rules  
// Returns "node" with the syntactic simplification rules applied
DAGNode*  quickSimplifyInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable);


// Return true if "node" is a constant; false otherwise  
bool isConstant(DAGNode *node);



// Given a previously linearized dag and a linearization table based on which linearization was done,
// return a copy of the dag with the linear nodes replaced by the original nodes 
DAGNode* getOriginalClone(DAGNode* node, map<DAGNode*, DAGNode*> &LinearizationTable);


// Search the LinearizationTable for a particular node and return the iterator
map<DAGNode*, DAGNode*>::iterator findInLinearizationTable(map<DAGNode*, DAGNode*> &LinearizationTable, DAGNode* clone);



// Function to write declarations in SMTLIB format for the set of variables in the set "Vars"
void writeBVDeclarationsInSMTLibFormatForSelectedVars(FILE *smt_file, map<string, int> WidthTable, set<string> &Vars);


// Print the given conjunction "root" in SMTLIB format in file "outfile_name" where
// "outfile_name" = "infile_name" without extension + "_simplified.smt"
void printConjunctInSMTLibFormat(DAGNode *root, map<string, int> &WidthTable, string infile_name, string &outfile_name);


// Here "node" is a selection node. This function returns
// the width of selection node which is,
// given, sel(x, 1, 3), return 3
int getWidthOfSelectionNode(DAGNode *node);


// return true if "node" is a bit-vector constant; false otherwise
bool isBVConstant(DAGNode *node);


// ImpureVars are variables occuring in non-linear operations
// This function traverse the dag "node" and gets the impure variables
// in the set ImpureVars
void getImpureVariables(DAGNode *node, set<string> &ImpureVars);


// Function to update the set VarSetToElim. This function removes the impure
// variables i.e. variables which cannot be eliminated by the CAV'11 paper based
// work from the set VarSetToElim
void updateVarSetToElim(set<string> &VarSetToElim, DAGNode *node);



// Print the given string "result" in SMTLIB format in file "outfile_name" where
// "outfile_name" = "infile_name" without extension + "_simplified.smt"
void printTrivialConjunctInSMTLibFormat(string result, string infile_name, string &outfile_name);



// Function to decide sat of conjunction of LMEs and LMDs by QE. 
// Calls getEquationsAndDisequations for dag ---> matrix conversion and
// qEliminatorForSolving for sat. checking from conjunction of LMEs and LMDs represented as matrices
// Returns 'u' if unsat, 's' if sat, 'x' if unknown  
char satCheckByExQuantifierElimination(DAGNode *root, set<string> &VarsToElim, map<string, int> &WidthTable);



// Print conjunction of LMCs in E, D, and u_AP in an SMTLIB file; name of the SMTLIB file
// is decided in the function printConjunctInSMTLibFormatWithCount
void printImplicationCheckInSMTFile(set<DAGNode*> &E, set<DAGNode*> &D, DAGNode *u_AP, int Show_ImpCheck_Calls, map<string, int> &WidthTable);




// Print the given conjunction "root" in SMTLIB format. The file name 
// is decided as "disjunct_count".smt and is put in a separate directory
void printConjunctInSMTLibFormatWithCount(DAGNode *root, map<string, int> &WidthTable, int disjunct_count);



// Print the given conjunction "root" in SMTLIB format with the variables to eliminate
// The file name is decided as "infilename_withno_ext"+"_square"+count+".smt"
void printInputToQEliminatorInSMTLibFormatWithCount(DAGNode *root, map<string, int> &WidthTable, set<string> &VarSetToElim, string infilename);



// return a single variable from the support of "node";
// rerturn "" if the support is empty
string getSingleVariable(DAGNode *node);




// Returns the value of the LMDE rooted at "node" 
// if it is evaluated using the variable ---> value map "Variable_Value_Map".
// Note that value here is a decimal number
// Return value of type "bvatom" is the bit-vector result. The parameter "boolean_result" is set to 
// true/false if the return value is Boolean
bvatom lmdeEvaluator(DAGNode *node, int width, map<string, ULINT> &Variable_Value_Map, bool &boolean_result);




// returns the value of E/\D/\Negated_u_AP if it is evaluated using the variable ---> value map "Variable_Value_Map"
bool conjunctionOfLMDEEvaluator(set<DAGNode*> &E, set<DAGNode*> &D, DAGNode *Negated_u_AP, int width, map<string, ULINT> &Variable_Value_Map);



// checks if E/\D/\Negated_u_AP is unsat/sat by simulating the dag
// returns
//   's' if sat
//   'u' if unsat
//   'x' if it cannot find
char implicationCheckBySimulation(set<DAGNode*> &E, set<DAGNode*> &D, DAGNode *Negated_u_AP, map<string, int> &WidthTable);


// show the Variable_Value_Map
void showVariable_Value_Map(string WhoamI, map<string, ULINT> &Variable_Value_Map);


// Returns result of evaluating the the LMC "lmde" using the variable--->value map Model
bool checkEntailment(DAGNode *lmde, map<string, int> &WidthTable, map<string, string> &Model);



// Same as the function lmdeEvaluator except that, 
// In the value ---> variable map "Variable_Value_Map", value is a binary string
bvatom lmdeEvaluatorWithBinaryModel(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result);



// returns true if eliminated => exists(X, original) i.e. eliminated \and (\forall X.(\neg original)) is unsat; false otherwise
bool checkForExactnessInOtherDirectionUsingCVC3(DAGNode* original, DAGNode* eliminated, map<string, int> WidthTable, set<string> &VarSetToElim);


// Given a set of strings VarsToElim, return it as \exists variable_1....\exists variable_n
// in the SMTLIB old format (Note that CVC3 we have uses the old SMTLIB format)
string  getExistsString(set<string> &VarsToElim, map<string, int> &WidthTable);


// Function to return the content of a dag (in propositional skeleton format) in old SMTLIB format
// Note that the CVC3 we have uses the old SMTLIB format
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(DAGNode *root, map<string, int> WidthTable, set<string> &VarSetToElim);


// Function to traverse the dag node "node" and return the content of a dag (in propositional skeleton format) in old SMTLIB format
// This is called by callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style
// Note that the CVC3 we have uses the old SMTLIB format
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, set<string> &VarSetToElim);


// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// create a copy of parent with indexes of the variable nodes changed
// based on the DeltaMap; This function uses dynamic programming
DAGNode* cloneWithRenamedDeltasWithDeltaMap(DAGNode* parent, map<int, int> &DeltaMap);


// Function to create clone with delta maps. Called from cloneWithRenamedDeltasWithDeltaMap
DAGNode* cloneWithRenamedDeltasWithDeltaMapInternal(DAGNode* parent, map<int, int> &DeltaMap, map<DAGNode*, DAGNode*> &DPTable);


// Function called by getSize. Gets size of dag rooted at "v" in variable "Size"
void getSizeInternal(DAGNode *v, int &Size, set<DAGNode*> &DPTable);


// Get size of a dag; uses dynamic programming
int getSize(DAGNode *dag);


// Simplify and node with multiple children and return the simplified node
// Called by quickSimplifyInternal
DAGNode* simplifyAndWithMultipleChildren(vector<DAGNode*> &Children);


// Simplify or node with multiple children  and return the simplified node
// Called by quickSimplifyInternal
DAGNode* simplifyOrWithMultipleChildren(vector<DAGNode*> &Children);


// Find bit-level LMDs among the LMDs in LMDSet, and put them in BitLevelLMDSet 
void getBitLevelLMDs(set<DAGNode*> &LMDSet, map<string, int> &WidthTable, set<DAGNode*> &BitLevelLMDSet);


// Get the #LMEs, #LMDs, #bit-level LMDs in the dag "node", and
// set the global variables LMECount_In_DAG, LMDCount_In_DAG, and
// BitLevel_LMDCount_In_DAG
void getLMDECounts(DAGNode* node, map<string, int> &WidthTable);


// Function called by getLMDECounts to get the details
// Given dag rooted at "node", this function finds the set of LMEs and the set of LMDs in "node"
void getLMEsAndLMDsInternal(DAGNode* node, set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, char status, set<string> &AlreadyEncountered);



// Get the number of bits in the variable to be eliminated in number_of_bits
// Note that the variable to be eliminated is VarNames[0]
void getNumberOfBitsInVarToElim(unsigned int &number_of_bits, list<string> &VarNames, map<string, int> &WidthTable);



// Print the details needed per call in files layer1_calls_fp and layer2_calls_fp, and set global variables
// according to the details of the call
void print_Layer_i_Calls(unsigned int number_of_lmes, unsigned int number_of_lmds, unsigned int number_of_bits, int time_in_layer, int layer, unsigned long long int duration_layer_ms);


// Get the #LMEs and #LMDs in the input to Layer 1
void getNumberOfLMEsNumberOfLMDs_Layer_1(unsigned int &number_of_lmes, unsigned int &number_of_lmds, Matrix &EqCoefMat, Matrix &DisEqCoefMat);


// Get the #LMEs and #LMDs in the input to Layer 2
void getNumberOfLMEsNumberOfLMDs_Layer_2(unsigned int &number_of_lmes, unsigned int &number_of_lmds, Matrix &EqCoefMat, Matrix &DisEqCoefMat, int NoVarsToElim);


// Print the given formula rooted at "root" in SMTLIB format (with the additional :exists specifier) in file "file_name"
// In the future use this function to express a given arbitrary formula in SMTLIB format
// when the result is needed in a tree format
void printInputToQEliminatorInSMTLibFormatInGivenFile(DAGNode *root, map<string, int> &WidthTable, set<string> &VarSetToElim, string filename);


// Return true if the global variable "time_out mode" is true and time out has happened;
// Return false otherwise
bool checkTimeOut();




// Function to do the same work as done by Ganesh & Dill
// Given a conjunction of LMCs as matrices, this substitutes the occurances of the odd variables
// to be eliminated using Ganesh & Dill's work
// Effect : The matrices are changed
void qEliminatorForEquationsAndDisequationsOnlyForOddCoefficients(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod);


// Function implementing the procedure BV_Solve in CAV'11 paper
// Given a conjunction of LMCs as matrices, this substitutes the occurances of the odd variables
// to be eliminated using Ganesh & Dill's work, and employs bit-blasting + QE at bit-level
// Result is NULL always (Although the result is generated, it is not returned)
DAGNode* qEliminatorReturningDAGOnlyForOddCoefficientsFollowedByBitBlasting(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable);


// This function finds the equations in which there are no variables left to eliminate, removes them from set of equations and copies them in a new matrices
void eliminateEquationsFreeOfAllVarsToElimAndCopyInNewMatrix(Matrix &EqCoefMat, Matrix &EqConstMat, int NoVarsToElim, Matrix &EqFreeOfAllVarsToElimCoefMat, Matrix &EqFreeOfAllVarsToElimConstMat);



// Get set of variables to be eliminated "VarSetToElim" from list of variables "VarNames"
// A variable in "VarNames" is to be eliminated if its index <= NoVarsToElim
// Called by qEliminatorReturningDAGOnlyForOddCoefficientsFollowedByBitBlasting
void getVarSetToElimFromVarNames(list<string> &VarNames, int NoVarsToElim, set<string> &VarSetToElim);


// Given a set of constraints and a set of variables to be eliminated,
// this function calls OmegaTest with scope reduction as well as QE_LMC (APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG), and
// returns the result from APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG
DAGNode* QEliminatorForMultipleVariablesUsingLMAandILPwithScopeReductionReturningDAG(set<DAGNode*> Constraints, set<string> VarsToElim, map<string, int> &WidthTable);



// Given a set of LMEs and LMDs, and a set of variables to be eliminated, 
// perform QE using OmegaTest. return value is always TRUE dag node
DAGNode* callOmegaTestReturningDAG(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> &WidthTable);



// Set global variables according to the details per call of QE_LMC
void getAP_QElimDetails(unsigned int number_of_lmes, unsigned int number_of_lmds, unsigned int number_of_vars);


// Get the #variables in a set of LMEs and set of LMDs
void getNumberOfVars(unsigned int &number_of_vars, Matrix &EqCoefMat, Matrix &DisEqCoefMat);


// Seggregate a set of LMCs into (i) a set of LMEs and (ii) a set of LMDs, and
// recreate the bit-level LMDs as LMEs in the form 2^k.x = t where x is the lowest
// in the lexicographic ordering, and t is free of x
void seggregateIntoLMEsAndLMDs(set<DAGNode*> &Constraints_Set, set<DAGNode*> &LMEs, set<DAGNode*> &LMDs, map<string, int> &WidthTable);



// recreate a bit-level LMD as LME in the form 2^k.x = t where x is the lowest
// in the lexicographic ordering, and t is free of x. Return the LME as dag node*
DAGNode* recreateEquationFromBitLevelDisequationInProperForm(DAGNode* disequation, map<string, int> WidthTable);



// Given a set of constraints and a set of variables to be eliminated,
// this function calls bit-blasting + QE at bit-level with scope reduction 
// as well as QE_LMC (APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG), and
// returns the result from APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG
DAGNode* QEliminatorForMultipleVariablesUsingLMAandBitBlastingwithScopeReductionReturningDAG(set<DAGNode*> Constraints, set<string> VarsToElim, map<string, int> &WidthTable);



// Given a set of LMEs and LMDs, and a set of variables to be eliminated, 
// perform QE using GD layer + bit-blasting + QE at bit-level. return value is always TRUE dag node
// This effectively implements BV_Solve in CAV'11 paper (But note that the result is always TRUE,
// hence this function was used only in the initial performance measurement experiments. The 
// function which was actually used in the CAV'11 experiments to implement BV_Solve is 
// DDManager::APResolveModifiedWithMultipleVariablesUsingLMAWithBlasting)
DAGNode* callBitBlastingReturningDAG(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> &WidthTable);


// Given a conjunction of LMCs as matrices, this substitutes the occurances of the odd variables
// to be eliminated using Ganesh & Dill's work.
// The result has two parts Phi1 which is free of variables to be eliminated, and
// Phi2 which contains the variables to be eliminated
// Phi1 is returned, and Phi2 is a parameter
DAGNode* qEliminatorReturningDAGOnlyForOddCoefficients(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable, DAGNode* &Phi2);




// Same as qEliminatorReturningDAGOnlyForOddCoefficients except that the result is 
// returned as Phi1 and Phi2
DAGNode* qEliminatorReturningDAGOnlyForOddCoefficientsReturningSingleDAG(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable);



// Given a set of constraints, converts them into matrices and 
// qEliminatorReturningDAGOnlyForOddCoefficientsReturningSingleDAG on the matrices
// returns the result from qEliminatorReturningDAGOnlyForOddCoefficientsReturningSingleDAG
DAGNode* callMatrixQEliminatorForGDLayer(set<DAGNode*> &Constraints, string VariableToElim, map<string, int> &WidthTable);



// Given a set of constraints and a set of variables to be eliminated, this function applies
// scope reduction and callMatrixQEliminatorForGDLayer to each variable
DAGNode* APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(set<DAGNode*> Constraints, set<string> VarsToElim, map<string, int> &WidthTable);


// Print the formula rooted at "root" in SMTLIB format in the file file name with 
// special handling of labels of root nodes. Labels of root nodes are printed as
// abstraction_"root_number_str"
void printOutputOfQEliminatorInSMTLibFormatInGivenFile(DAGNode *root, map<string, int> &WidthTable, string file_name, int root_number);


// Same as printOutputOfQEliminatorInSMTLibFormatInGivenFile except that 
// only a selected set of variables are declared
void printOutputOfQEliminatorInSMTLibFormatInGivenFileForNonZeroFrame(DAGNode *root, map<string, int> &WidthTable, string file_name, int root_number);




// create conjunction of the given set of constraints and print in SMTLIB format
void createConjunctionAndPrintInSMTLibFormat(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);



// Function to get QE_LMC details when QE_LMC is directly called by the user
// This function is called by the function DDManager::QELIM_Conjunction_level
void getAP_QElimDetailsAtDAGLevel_For_QELIM_Conjunctions(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, unsigned int &number_of_lmes, unsigned int &number_of_lmds, unsigned int &number_of_vars, unsigned int &number_of_vars_to_elim, unsigned int &number_of_bits);




// Given a set of LMCs in "Constraints" and a set of variables to be eliminated,
// create conjunctions of constraints of the same bit-width and give to the function 
// createConjunctionAndPrintInSMTLibFormat
void createConjunctionsAccordingToModAndPrintInSMTLibFormat(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// evaluateFunction can be used to evaluate dags which are in the following form
// dag ::= ite(dag1, dag, dag) | bit-vector | dag1
// dag1 ::= dag1 /\ dag1 | dag1 \/ dag1 | !dag1 | lmde
// Thus the result is always a bit-vector (returned as a string)
// This calls evaluateFunction_Internal which gets the result as bvatom.
// The result is converted to string by this function
string evaluateFunction(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable);



// Called by evaluateFunction as explained above
bvatom evaluateFunction_Internal(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, map<DAGNode*, bvatom> &DPTable);



// Function which calls lmdeEvaluatorModified to evaluate an LMDE
bvatom call_lmdeEvaluatorReturningBV(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable);



// lmdeEvaluatorModified is lmdeEvaluatorWithBinaryModel except that, 
// it handles simple selections of the form variable[integer:integer]
bvatom lmdeEvaluatorModified(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result);


// Return decimal digit corresponding to a given hex digit
ULINT getCoefOfChar(char c);


// Return power^16
ULINT findPower16(int power);


// Return decimal number corresponding to a hex string
ULINT hexStringToULINT(string x);


// Same as evaluateFunction except that the function also returns a subset of the support of dag
// which is sufficient to evaluate dag to the specific value (in "ProblematicSignalsAtPreviousStateForSignal")
string findUnsatCoreEvaluationBasedStrategy(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal);



// Function called by findUnsatCoreEvaluationBasedStrategy to achieve the functionality described
// in the synopsys of findUnsatCoreEvaluationBasedStrategy
bvatom findUnsatCoreEvaluationBasedStrategy_Internal(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal, map<DAGNode*, pair<bvatom, set<string> > > &DPTable);



// Same as call_lmdeEvaluatorReturningBV except that it also finds the support of
// the LMDE rooted at "node" in "ProblematicSignalsAtPreviousStateForSignal"
bvatom call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal);



// ReducedModel is the smaller one of ReducedModelChild1 and ReducedModelChild2
void findSmallerModel(set<string> &ReducedModelChild1, set<string> &ReducedModelChild2, set<string> &ReducedModel);



// create the dag for ("variable" = "value") and return it
DAGNode* getEquationDAGFromAssignment(string variable, int width, string value);


// Given a dag rooted at "node" and a variable --> value map "Value_Map", find unsat. core for
// the formula (node \and \and(variable_i = value_i)) where i=0 to sizeof(Value_Map)-1 
// Unsat core is computed in ProblematicSignalsAtPreviousStateForSignal. The return string is "Hi" always.
string findUnsatCoreSMTBasedStrategy(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal);



// Internal function called by getDescendents to get the set of descendents of a node
void getDescendentsInternal(DAGNode *node, set<DAGNode*> &InitialDescendents);


// Get the set of descendents of the dag node "node"
void getDescendents(DAGNode *node, set<DAGNode*> &Descendents);


// Given a dag rooted at "parent", create a dag where the subtractions are replaced by additions
// with additive inverses
DAGNode* getSubtractionReplacedClone(DAGNode* parent, map<DAGNode*, DAGNode*> &SubtractionTable, map<string, int> &WidthTable);


// Copy the set of strings to a list of strings
void copySetToList(set<string> MySet, list<string> &MyList);




// Given a dag node rooted at "parent" where the label of the root is "bvsub", return 
// a node where the "bvsub" is replaced with "bvadd" with additive inverse
DAGNode* recreateSubtractionDAG(DAGNode* parent, map<string, int> &WidthTable);


// Given lists {c1,...,cn}, {x1,...,xn} and c0, create dag for c1.x1 + ... + cn.xn + c0, and return it
DAGNode* getDAGFromVarsCoefsAndConstant(list<string> Vars, list<ULINT> Coefs, ULINT SelectedConst, int width);



// Function getConstantModified with support for bvsub nodes.
// bvsub is treated as bvadd with the additive inverse
void getConstantModifiedWithMinusSupported(DAGNode* node, ULINT &constant, int Sign, ULINT Mod);



// Function getCoefficient with support for bvsub nodes.
// bvsub is treated as bvadd with the additive inverse.
void getCoefficientWithMinusSupported(DAGNode* node, string Var, ULINT &Coefficient, int Sign, ULINT Mod);



// Return the width of an LMDE node with selection operator
int getWidthOfLMDEWithSelects(DAGNode *node, map<string, int> MyWidthTable);



// Returns true if f is unsat; false otherwise using STP
// Here the dag is printed in SMTLIB format as dag itself - with commmon sub-expression sharing
// Called by ThUnsatSingleFormula
bool ThUnsatSingleFormulaInSMTLibFormatAsDAG(DAGNode* f, map<string, int> &WidthTable);



// Function to print the dag "root" in SMTLIB format as dag itself - with commmon sub-expression sharing
// Called by ThUnsatSingleFormulaInSMTLibFormatAsDAG
// In the future use this function to express a given arbitrary formula in SMTLIB format
// when the result is needed in a dag format
string printDAGInSMTLIBFormatAsDAG(DAGNode *root, map<string, int> &WidthTable, FILE *fp);



// Modified version of the function convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat,
// where the width is not one.
string convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(string literal, int width);



// Function to print the dag "root" in SMTLIB format as dag itself - with commmon sub-expression sharing
// Called by printDAGInSMTLIBFormatAsDAG
string printInSMTLibFormatModified(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<DAGNode*, string> &LabelTable, FILE *fp, map<string, int> &WidthTable, int &TempCount, map<DAGNode*, string> &TypeTableOfITENodes);



// Modified version of the function createWidthTableForOperators with dynamic programming
void createWidthTableForOperatorsWithDP(DAGNode *tree, map<DAGNode*, int> &WidthTableForNodes, map<string, int> &WidthTable);


// Modified version of the function propogateWidths function with dynamic programming 
void propogateWidthsWithDP(DAGNode *tree, map<DAGNode*, int> &WidthTableForNodes, int WidthFromParent, set<DAGNode*> &DPTable);


// Function to set properly, the types of the nodes in the dag rooted at "tree" 
string setTypesWithDP(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfITENodes);


// Function called by the function setTypesWithDP to set the types of the nodes 
string setTypesWithDPInternal(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfITENodes, HashTable<string, string> *PtrHTable);


// Function to write the declarations in SMTLIB format for the selected set of 
// variables in the list "VarNames"
void writeBVDeclarationsInSMTLibFormatWithLookup(FILE *smt_file, map<string, int> &WidthTable, list<string> &VarNames);



// Returns true if exists(X, original) => eliminated i.e. original \and \neg eliminated is unsat; false otherwise
// Underlying SMT-Solver used is STP, not Yices.
bool checkForExactnessInOneDirectionUsingSTP(DAGNode* original, DAGNode* eliminated, map<string, int> &WidthTable);



// Get the #equations, #disequations, etc. in the dag "node", and
// set global variables accordingly
void getEquationsDisequationsCounts(DAGNode* node, map<string, int> &WidthTable);



// Function to obtain the set of equations and the set of disequations in the dag node rooted at "node"
void getEquationsAndDisequationsInternal(DAGNode* node, set<DAGNode*> &EquationSet, set<DAGNode*> &DisequationSet, char status, set<string> &AlreadyEncountered);



// Function to put the bit-level disequations in "DisequationSet" into "BitLevelDisequationSet"
void getBitLevelDisequations(set<DAGNode*> &DisequationSet, map<string, int> &WidthTable, set<DAGNode*> &BitLevelDisequationSet);



// This function sets the variable has_selects_concats to true if the dag
// rooted at "node" has selects / concats inside it
void checkForSelectsConcats(DAGNode* node, bool &has_selects_concats);



// Same as updateVarSetToElim(set<string> &VarSetToElim, DAGNode *node) except that
// the result is obtained in parameters ImpureVarsToQElim and PureVarsToQElim
void updateVarSetToElim(set<string> &VarSetToElim, DAGNode *node, set<string> &ImpureVarsToQElim, set<string> &PureVarsToQElim);



// Same as callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with support for concatenation 
// Use this function in the argument dag has concats in it
string callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(DAGNode *root, map<string, int> &WidthTable);


// Modified version of getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with handling of concat (&) added : Presently & is handled in the same way as in SSimulator/CDAGNode.cpp::getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable
string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<string, int> &WidthTable);


// Returns true if the "node" is a variable; false otherwise
bool isVariable(DAGNode *node);


// Returns true if the node is a concatenation of variables such as v1 @ ... @ vn
bool isVariableConcatenation(DAGNode *node);


// Same as propogateWidths with support for concats
void propogateWidths_WithSupportForSelsConcats(DAGNode *node, map<DAGNode*, int> &WidthTableForNodes, int WidthFromParent);


// lmdeEvaluatorModified_With_Concat_Support is lmdeEvaluatorWithBinaryModel except that,
// it handles selections of the form expression[integer:integer], and concatenations
bvatom lmdeEvaluatorModified_With_Concat_Support(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result, map<string, int> &WidthTable);



// Same as checkEntailment with support for selects and concats
bool checkEntailment_WithSupportForSelsConcats(DAGNode *constraint, map<string, int> &WidthTable, map<string, string> &Model);


// Given a set of LMCs in "Constraints" and a set of variables to be eliminated, generate two sets of constraints - 
// A set of constraints free of the variables to be eliminated and another set of constraints which contain
// the variables to be eliminated
void getFreeAndBoundConstraintsForMultipleVariables(vector<DAGNode*> &Constraints, set<string> &Variables, set<DAGNode*> &Free_Constraints, set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable);



// Same as getLinearizedClone except 
// 1) it handles only selection operator
// 2) if the selection operator x[2:3] is found, and  x is a variable to be eliminated,
//    then the fresh variable replacing x[2:3] is also added to the set of variables to be eliminated
DAGNode* getSimpliedLinearizedCloneWithChangeInVarsToElim(DAGNode* parent, map<DAGNode*, DAGNode*> &LinearizationTable, int &count, map<string, int> &WidthTable, set<string> &VarSetToElim, set<string> &AutoVarsToElim);



// Function to dump the contents of a given dag rooted at "resultDAG" in
// a given file (Note that although from the function name, it appears that the 
// conjunction is printed in SMTLIB format, really it is just dumped as text file)
void PrintInSMTLibFormatInFileWithGivenPreName(DAGNode* resultDAG, set<string> &VarsToElim, map<string, int> &WidthTable, string file_pre_name, int file_counter);


// Function to create a conjunction of constraints in "Constraints", and dump in
// a given file (Note that although from the function name, it appears that the 
// conjunction is printed in SMTLIB format, really it is just dumped as text file)
void createConjunctionAndPrintInSMTLibFormatInFileWithGivenPreName(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, string file_pre_name, int file_counter);



// Find the width of the variable from the WidthTable, and return it
// If there is no entry for the variable in WidthTable, exit from the program with a message
int getWidthOfVariable(map<string, int> &WidthTable, string variable);




// Create a selection node with given name, low index, high index, and width, and return it
DAGNode* createSelectionNode(string variable, int width, int low, int high);




// Function to get a dag rooted at "root" which is of the form
// c1*x1+ ... +cn*xn+c0 as AX+B.
// This function converts this to [c1 ... cn] and [c0], 
// pushes [c1 ... cn] into A and [c0] into B
// If ci is 1, it may be omitted. i.e. 1.x1+1.x2 can appear as x1+x2 as well
void getCoefsAndConstsModified(DAGNode *root, list<string> &VarNames, Matrix &A, Matrix &B);



// Given a dag rooted at "root" which is of the form c1*x1+ ... +cn*xn+c0,
// this function generates the map {x1-->c1,...,xn--->cn}
// If ci is 1, it may be omitted. i.e. 1.x1+1.x2 can appear as x1+x2 as well
// precondition: Initialize "VariableCoefficientMap" to {x1-->1,...,xn--->1}
// The function obtainVariableCoefficientMap performs this initialization and
// then calls updateVariableCoefficientMap
void updateVariableCoefficientMap(DAGNode *node, map<string, ULINT> &VariableCoefficientMap);


// Functions to implement Monniaux style QE on dags with LMIs start here


// Same as callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with support for LMIs
// Use this function if the argument dag has LMIs in it
string callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(DAGNode *root, map<string, int> &WidthTable);


// Version of createWidthTableForOperatorsWithDP with support for LMIs
void createWidthTableForOperatorsWithDP_WithSupportForLMIs(DAGNode *tree, map<DAGNode*, int> &WidthTableForNodes, map<string, int> &WidthTable);



// Modified version of getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with handling of LMIs
string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<string, int> &WidthTable);



// Version of printDAGInSMTLIBFormatAsDAG with support for LMIs
string printDAGInSMTLIBFormatAsDAG_WithSupportForLMIs(DAGNode *root, map<string, int> &WidthTable, FILE *fp);



// Version of setTypesWithDP with support for LMIs
string setTypesWithDP_WithSupportForLMIs(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfNodes);



// Version of setTypesWithDPInternal with support for LMIs
string setTypesWithDPInternal_WithSupportForLMIs(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfNodes, HashTable<string, string> *PtrHTable);



// Version of printInSMTLibFormatModified with support for LMIs
string printInSMTLibFormatModified_WithSupportForLMIs(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<DAGNode*, string> &LabelTable, FILE *fp, map<string, int> &WidthTable, int &TempCount, map<DAGNode*, string> &TypeTableOfNodes);



//  Version of checkEntailment_WithSupportForSelsConcats with support for LMIs also
bool checkEntailment_WithSupportForSelsConcats_WithLMISupport(DAGNode *constraint, map<string, int> &WidthTable, map<string, string> &Model);



//  Version of lmdeEvaluatorModified_With_Concat_Support with support for LMIs also
bvatom lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result, map<string, int> &WidthTable);



// negateConstraint with support for LMIs
DAGNode* negateConstraint_With_LMISupport(DAGNode* C);



// checkForExactnessInOneDirectionUsingSTP with support for LMIs
bool checkForExactnessInOneDirectionUsingSTP_With_LMI_Support(DAGNode* original, DAGNode* eliminated, map<string, int> &WidthTable);



//  Version of ThUnsatSingleFormulaInSMTLibFormatAsDAG with support for LMIs
bool ThUnsatSingleFormulaInSMTLibFormatAsDAG_With_LMI_Support(DAGNode* f, map<string, int> &WidthTable);



// callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable with support for LMIs
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(DAGNode *root, map<string, int> WidthTable);



// getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable with support for LMIs
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required);



// Function to convert each variable x to x+((2^p)/2) mod 2^p,
// where 2^p is ModWidthInteger and (2^p)/2 is ModWidthIntegerHalf
DAGNode*  convertToUnsigned(DAGNode *node, map<string, int> &WidthTable);


// Function called by  convertToUnsigned
// Returns "node" with the conversion applied
DAGNode*  convertToUnsignedInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable, map<string, int> &WidthTable);


// Function to convert variable x to  x+((2^p)/2) mod 2^p
DAGNode* convertVariableNodeToUnsigned(DAGNode *node, map<string, int> &WidthTable);


// Function to convert the term c.x to  c.x+c.((2^p)/2) mod 2^p
DAGNode* convertMultiplyNodeToUnsigned(DAGNode *node, map<string, int> &WidthTable);



// create conjunction of the given set of constraints and 
// call the function checkForExactnessInOtherDirection in BitBlaster.cpp
void createConjunctionAndCheckQBFSolverEffectivess(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);




// Version of getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style with support for LMIs
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, set<string> &VarSetToElim);


// Version of callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style with support for LMIs
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(DAGNode *root, map<string, int> &WidthTable, set<string> &VarSetToElim);



// Given a dag node "parent" and a map "ValueMap" from variable names to valueindecimal[width]
// for some vaiables, create a copy of parent with variable nodes replaced by value[width] nodes
// for the variables present in the "ValueMap".
// This function uses dynamic programming
DAGNode* cloneUsingValueMap(DAGNode* parent, map<string, string> &ValueMap);



// Function to create clone with value map. Called from cloneUsingValueMap
DAGNode* cloneUsingValueMapInternal(DAGNode* parent, map<string, string> &ValueMap, map<DAGNode*, DAGNode*> &DPTable);



// Given a dag node "parent", a "shift", and "VarsToElim",
// create a copy of parent with occurances of variables v in "VarsToElim" changed as
// v_shift; This function uses dynamic programming
DAGNode* cloneAndShift(DAGNode* parent, int shift, set<string> &VarsToElim);



// Function called from cloneAndShift
DAGNode* cloneAndShiftInternal(DAGNode* parent,  int shift,  set<string> &VarsToElim, map<DAGNode*, DAGNode*> &DPTable);



// Insert new entries into the width table after cloneAndShif function
void updateWidthTableAftercloneAndShift(map<string, int> &WidthTable, int shift, set<string> &VarsToElim);


// Call cloneAndShift followed by updateWidthTableAftercloneAndShift
DAGNode* cloneAndShiftWithWidthTableUpdate(DAGNode* Conjunction, int shift, set<string> &VarsToElim, map<string, int> &WidthTable);



// Function called by  convertConstants
// Returns "node" with the conversion applied
DAGNode*  convertConstantsInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable, bool bvmul_encountered);


// Function to convert each constant to constant - 2^(p-1)
// where 2^p is ModWidthInteger and 2^(p-1) is ModWidthIntegerHalf
DAGNode*  convertConstants(DAGNode *node);



// Convert "bvconstant" to "bvconstant - ModWIdthIntegerHalf"
string subtractHalfMod(string bvconstant);


// Function to convert each side of LMI p \bowtie q to p+2^(p-1) \bowtie q+2^(p-1)
// where  2^(p-1) is ModWidthIntegerHalf
DAGNode*  addHalfModulusToBothSides(DAGNode *node);



// Function called by  addHalfModulusToBothSides
// Returns "node" with the conversion applied
DAGNode*  addHalfModulusToBothSidesInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable);



// LHS is a1.x1 +a2.x2 + .... +an.xn 
// Convert this to a1.x1 +a2.x2 + .... +an.xn+ModWidthIntegerHalf
DAGNode* addHalfModulusToLHS(DAGNode *LHS);



// RHS is a constant c 
// Convert this to c+ModWidthIntegerHalf
DAGNode* addHalfModulusToRHS(DAGNode *RHS);



// Convert "bvconstant" to "(bvconstant + ModWIdthIntegerHalf) Mod ModWIdthInteger"
string addHalfMod(string bvconstant);



// Version of   splitConjunctionIntoSet where APs are LMIs
void splitConjunctionIntoLMIVector(DAGNode* Constraint, vector<DAGNode*> &Conjuncts);


// Version of splitConjunctionIntoLMIVector except that set is used in place of vector
void splitConjunctionIntoLMISet(DAGNode* Constraint, set<DAGNode*> &Conjuncts);


// Version of setsEqual for strings
bool setsEqual(set<string> &A, set<string> &B);


// Given a dag "root" of the form c1.x1+...+cn.xn+c0, obtain the map xi--->ci 
// in VariableCoefficientMap, and c0 in ConstantPart
void obtainVariableCoefficientMap(DAGNode* root, map<string, ULINT> &VariableCoefficientMap, ULINT &ConstantPart);


// Function to show a given set of strings in a given file
void showSet(set<string> &MySet, string WhoAmI, FILE *fp);


// show a set of DAGNodes in a given file
void showAPSet(string WhoamI, set<DAGNode*> &APSet, FILE *fp);


// Version of splitConjunctionIntoLMISet except that we need to take all APs, not just LMIs
void splitConjunctionIntoSetGeneral(DAGNode* Constraint, set<DAGNode*> &Conjuncts);



// Split the given set of constraints into sets of LMEs, LMDs, and LMIs
void splitConstraintsIntoLMELMDLMISets(set<DAGNode*> &Constraints, set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet);



// Creates a dag from a conjunction of inequations specified as matrices, and returns it	
DAGNode* getDAGFromInequations(Matrix &InEqLHSCoefMat, Matrix &InEqLHSConstMat, Matrix &InEqRHSCoefMat, Matrix &InEqRHSConstMat, list<string> &VarNames, ULINT Mod, vector<string> &LMITypes);



// The function simplifies LMIs using LMEs
DAGNode* qEliminatorForInequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &InEqLHSCoefMat, Matrix &InEqLHSConstMat, Matrix &InEqRHSCoefMat, Matrix &InEqRHSConstMat,int NoVarsToElim, ULINT Mod, vector<string> &LMITypes, list<string> &VarNames);


// Given a set of DAGNodes, which are LMIs and LMEs, convert them into matrix representation
// A pair of matrices for LMEs, and four matrices for LMIs, and one vector for types of LMIs are generated
bool getEquationsAndInequations(set<DAGNode*> &LMESet, set<DAGNode*> &LMISet, list<string> &VarNames, int &NoOfBits, int &NoOfEqs, int &NoOfInEqs, Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &InEqLHSCoefMat, Matrix &InEqLHSConstMat, Matrix &InEqRHSCoefMat, Matrix &InEqRHSConstMat, map<string, int> &WidthTable, vector<string> &LMITypes);



// Given a set of LMIs and LMEs and a variable to be eliminated, this function first converts them
//  into matrix representation, and then calls the matrix based QEliminator to simplify the LMIs
DAGNode* callMatrixQEliminatorForLMIs(set<DAGNode*> &LMESet, set<DAGNode*> &LMISet, string VariableToElim, map<string, int> &WidthTable);



// Given a set of LMCs, LMIs and a variable to be eliminated, this function first converts the
// LMCs, LMIs into matrix representation, and then calls the matrix based LME based simplification
DAGNode* callMatrixQEliminatorForLMCsAndLMIs(set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet, string VariableToElim, map<string, int> &WidthTable);


// Version of setsEqual for DAGNode*
bool setsEqual(set<DAGNode*> &A, set<DAGNode*> &B);


// Modification of getCoefficient which can operate on any dag.
// Remember that getCoefficient operates on dags of the form c1.x1 + .... + cn.xn + c0
void getCoefficientCorrected(DAGNode *node, string Var, ULINT &Coefficient);


// checks if the given node is false
bool isFalse(DAGNode *node);


// Show a vector of DAGNode* nodes
void showAPVector(string WhoamI, vector<DAGNode*> &APVector);



// Show a vector of DAGNode* nodes in given file
void showAPVector(string WhoamI, vector<DAGNode*> &APVector, FILE* fp);


// Nullify all columns except the one indexed by  SelColumn 
void nullifyColumnsExceptOne(Matrix &A, int SelColumn);


// Copy the contents of the set to vector
void copySetToVector(set<DAGNode*> &MySet, vector<DAGNode*> &MyVector);


// Version of quickSimplify which handles LMI operators also
DAGNode*  quickSimplify_WithLMISupport(DAGNode *node);


// Function called by quickSimplify_WithLMISupport to simplify the dag node "node" based on some simple syntactic rules  
// Returns "node" with the syntactic simplification rules applied
DAGNode*  quickSimplify_WithLMISupportInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable);


// Evaluates an LMI expression where the operands are constants
char booleanLMIExpression(string LMIType, DAGNode* LHS, DAGNode* RHS);



// Given a set of constraints, converts them into matrices and then
// qEliminatorReturningDAGOnlyWithLayer1CAV11 is called on the matrices
// qEliminatorReturningDAGOnlyWithLayer1CAV11 is the layer1 of CAV'11 work
DAGNode* callMatrixQEliminatorOnlyWithLayer1CAV11(set<DAGNode*> &Constraints, string VariableToElim, map<string, int> &WidthTable, DAGNode* &PivotEquation);



// Apply Layer1 of CAV'11 on the matrices
DAGNode* qEliminatorReturningDAGOnlyWithLayer1CAV11(Matrix &EqCoefMatPassed, Matrix &EqConstMatPassed, Matrix &DisEqCoefMatPassed, Matrix &DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> &VarNames, map<string, int> &WidthTable, DAGNode* &Phi1, DAGNode* &Phi2, DAGNode* &PivotEquation);



// Version of  qEliminatorForEquationsAndDisequations, which returns the pivot LME
void qEliminatorForEquationsAndDisequationsWhichReturnsPivotEquation(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod, Matrix &MyPivotRow, ULINT &MyPivotCoef);


// returns true if the dag node "node" contains the variable VarToElim; false otherwise
bool isBound(DAGNode* node, string VarToElim);



// Find minimum of i and j
ULINT findMinimum(ULINT i, ULINT j);


// Find maximum of i and j
ULINT findMaximum(ULINT i, ULINT j);


// Version of splitConjunctionIntoSetGeneral, except that the output is obtained in a vector
void splitConjunctionIntoVectorGeneral(DAGNode* Constraint, vector<DAGNode*> &Conjuncts);


// Given an equation, convert it to the form c1.x1+....+cn.xn = dn
DAGNode* recreateEquationInOriginalForm(DAGNode* equation, map<string, int> &WidthTable);



// Convert strict LMIs in Constraints into weak LMIs
void convertStrictLMIsToWeakLMIs(vector<DAGNode*> &Constraints, map<string, int> &WidthTable);


// Recreate the constraint with proper ordering of terms
void recreateWeakConstraint(DAGNode *Constraint, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint);


// Slightly modified version of getRecreatedBVUGTConstraints
// Recreate LHS > RHS 
void getRecreatedBVUGTConstraintsModified(DAGNode *LHS, DAGNode *RHS, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint1, DAGNode* &RecreatedConstraint2, set<string> &VarsToElim);




// Convert the LMDEs to the form 2^k.x \bowtie t and do not change the LMIs
void convertLMDEsToProperForm(vector<DAGNode*> &Constraints, map<string, int> &WidthTable);


// Version of simplifiableLMIPairFound_ForWeakLMIs which works on any weak constraints
bool simplifiableLMIPairFound_ForWeakLMIs_Advanced(DAGNode* uge, DAGNode* ule);



// Try to synthesize LMEs from LMIs 
void tryToSynthesizeLMEsFromLMIs(vector<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable);



// synthesize LMEs from LMIs 
void synthesizeLMEsFromLMIs(vector< pair<DAGNode*, DAGNode*> > &SimplifiableLMIPairs, vector<DAGNode*> &LMEs, map<string, int> &WidthTable);



// Returns the conjunction of the constraints in "Constraints"
DAGNode* getConjunctionFromSet(set<DAGNode*> &Constraints);



// Version of getRelevantLMIDetails s.t. there's no printing and 
// change is made in the variables which are passed
void getRelevantLMIDetails_With_Recursive_Splitting(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int &LMINoVarsToElimLocal, int &LMINoLMIsLocal, int &LMINoLMEsLocal, int &LMINoLMDsLocal, int &LMINoVarsLocal, unsigned long long int &LMITotalBitsLocal, unsigned long long int &LMIBitsToElimLocal, int &PresentConjunctionNumberLocal);



// Given a set of LMEs, LMDs, and LMIs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode* using Layer1
// Layer1: CAV'11 work / LME based simplification
DAGNode* applyLMILayer1(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Given a dag Phi and a set of variables to  be eliminated, find the # of variables 
// to be eliminated still remaining
int findNumberOfVarsToElimRemaining(DAGNode* Phi, set<string> &VarsToElim);



// Check if the RHS of the given LMI is a constant
bool lmiRHSConstant(DAGNode* lmi);


// Check if the given pair of LMIs can be put as a pair
bool LMIPairFound(DAGNode* ugt, DAGNode* ule);


// Show vector< pair<DAGNode*, DAGNode*> > LMIPairs
void showLMIPairs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs);


//Version of identifyLMIPairs for BVULE, BVUGE constraints
 void identifyLMIPairs_ForWeakLMIs(set<DAGNode*> &Constraints, vector<pair<DAGNode*, DAGNode*> > &LMIPairs);
 
  // Given a set of LMIs and a set of variables to be eliminated, this function calls
// applyLMILayer2 repeatedly as long as it drops variables
DAGNode* applyLMILayer2Recursive(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);


// Given a set of LMCs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode* using Layer2
DAGNode* applyLMILayer2(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable);




// Version of dropUnconstrainingDisequationsAndInequations,
// which is corrected to handle any conjunction of LMEs, LMDs, and LMIs
DAGNode* dropUnconstrainingDisequationsAndInequationsCorrected(set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet, string VarToElim,  int WidthOfVarToElim, ULINT ModOfVarToElim, map<string, int> &WidthTable);


// Corrected version of findConstraintsWithMaximumKValueForLMDsAndLMIs
void findConstraintsWithMaximumKValueForLMDsAndLMIsCorrected(set<DAGNode*> &Xi, string VarToElim, set<DAGNode*> &Pi, ULINT ModOfVarToElim);


// Find k-value of the constraint
ULINT findKValue(DAGNode* Constraint, string VarToElim, ULINT ModOfVarToElim);


// Find minimum k-value from a set of constraints
ULINT findMinimumKValue(set<DAGNode*> &Phi, string VarToElim, ULINT WidthOfVarToElim, ULINT ModOfVarToElim);


// Corrected version of isExtLMDsAndLMIs
// Checks if \exists VarToElim. (Phi /\ Xi) \equiv \exists VarToElim. (Phi) using counting argument
bool isExtForLMDsAndLMIsCorrected(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, ULINT ModOfVarToElim);


// Corrected version of findCombinationsDisallowedByLMDs
 long long int findCombinationsDisallowedByLMDsCorrected(set<DAGNode*> &LMDSet,  string VarToElim, ULINT ModOfVarToElim);

 
 // Check if InputDag is a conjunction of LMEs
 // If yes, return true node
 // Else return the original node
 DAGNode* singleLMEElimination(DAGNode* InputDag);
 
 


 // Recreate LMIs with terms on both sides as tighter constraints with constants on RHS
DAGNode* findTighterSimplerConstraint(string LMIType, DAGNode* LHS, DAGNode* RHS,  ULINT ModOfVarToElim, int WidthOfVarToElim);



// Version of findCombinationsPermittedByLMIs which works on weak LMIs
long long int findCombinationsPermittedByLMIs_OnWeakLMIs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, int k1, ULINT WidthOfVarToElim, ULINT ModWidthOfVarToElim);



// Version of convertStrictLMIsToWeakLMIs with support for LMEs and LMDs
void convertStrictLMIsToWeakLMIs_WithLMDESupport(vector<DAGNode*> &Constraints, map<string, int> &WidthTable, set<string> &VarsToElim);



// Given a set of constraints in "Constraints" and a set of variables in "Variables",
// get the set of constraints free of variables in "Variables" in "Free_Constraints", 
// and the remaining constraints in "Bound_Constraints"
void getFreeAndBoundConstraintsForMultipleVariables(set<DAGNode*> &Constraints, set<string> &Variables, set<DAGNode*> &Free_Constraints, set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable);




// Convert Simplified_Bound_Part into conjunction of QFree_Bound_Part and Quantified_Bound_Part
// Quantified_Bound_Part has VarsToElim
// QFree_Bound_Part is free of VarsToElim 
void convertToBoundAndFreeParts(DAGNode* Simplified_Bound_Part, set<string> &VarsToElim, DAGNode* &QFree_Bound_Part, DAGNode* &Quantified_Bound_Part, map<string, int> &WidthTable);



// Version of convertBVUGEConstraintsToBVULE s.t. Phi 
// can contain equalities, disequalities also.
// Also recreate all the constraints with a proper variable ordering
DAGNode* convertBVUGEConstraintsToBVULECorrected(DAGNode* Phi, map<string, int> &WidthTable);


// Version of recreateWeakConstraint except that recreation happens from components
void recreateWeakConstraintFromComponents(DAGNode *LHS, DAGNode *RHS, string LMIType, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint);



// Check if each constraint is either 1) free of variable or
// 2) Does not have variable on both sides and appears as k.v <= RHS or LHS <= k.v (note that k should 
// be the same for all constraints) where k is an odd number
bool constraintsAlreadyInNormalForm(set<DAGNode*> &Constraints, string variable, map<string, int> &WidthTable);



// Checks if the simple version of Fourier-Mozkin can be applied for QE
// from /\Constraints.
// Simple version of Fourier-Motzkin can be applied if
// all occurances of variable in the Constraints are odd
bool simpleFourierMotzkinCanBeApplied(set<DAGNode*> &Constraints, string variable,  map<string, int> &WidthTable);



// Eliminate quantifiers from the conjunction using Fourier-Motzkin style QE  where the variable has odd coefficient
DAGNode* applyFourierMotzkin(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable);


// Corrected version of convertToNormalForm for multiple variables.
// We found some bugs before reporting to CAV'12.
// These bugs are corrected here.
DAGNode* convertToNormalFormForMultipleVariablesCorrected(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, list<DAGNode*> &APListForNormalizedDD);




// Normalize the constraints of the form ((+/-).kx+e_1 <= e_2) and (e_1 <= (+/-).kx+e_2)
void normalizeConstraintsWithVariableOnOneSide(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector, list<DAGNode*> &APListForNormalizedDD);



// Normalize the constraints of the form ((+/-).kx+e_1 <= (+/-).kx+e_2)
void normalizeConstraintsWithVariableOnBothSides(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector, list<DAGNode*> &APListForNormalizedDD);



// Normalize LMDs of the form kx+e_1 \neq e_2 and -kx+e_1 \neq e_2
void normalizeLMDs(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector, list<DAGNode*> &APListForNormalizedDD);



// Find the side in which variable appears in Constraint
// It can be left, right, none, or both
string findSideOfVariableInConstraint(DAGNode* Constraint, string VarToElim);


// Find the coefficient of the VarToElim in Constraint, and its additive inverse
void getCoefOfVarToElimAndAIOfCoefOfVarToElim(DAGNode *Constraint, string VarToElim, string side_of_variable, ULINT ModOfVarToElim, ULINT &CoefOfVarToElim, ULINT &AIOfCoefOfVarToElim);



// Checks if the coefficient of the variable to be  eliminated is the same/additive inverse
// of the coefficients which previously appeared. Returns original/negated according to it
string findTypeOfCoefficient(DAGNode* Constraint, string VarToElim, string side_of_variable, ULINT CoefOfVarToElim, ULINT AIOfCoefOfVarToElim);


// Returns the conjunction of the constraints in "Constraints"
DAGNode* getConjunctionFromVector(vector<DAGNode*> &Constraints);


// Returns the disjunction of the constraints in "Constraints"
DAGNode* getDisjunctionFromVector(vector<DAGNode*> &Constraints);


// Get the LHS, RHS, and name of the given LMI node
void getChildrenAndNameOfLMINode(DAGNode* LMI, DAGNode* &LHS, DAGNode* &RHS, string &Name);

// Function to simplify the dag node "node" 
// using the rule t_1 <= c_1 \wedge \neg (t_1 <= c_1-1) ---> t_1 = c_1
DAGNode*  simplifyLMIDag(DAGNode *node, map<string, int> &WidthTable);


// Function called by simplifyLMIDag 
DAGNode*  simplifyLMIDagInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable, map<string, int> &WidthTable);


// Function called by getOriginalClone
DAGNode* getOriginalClone_Internal(DAGNode* node, map<DAGNode*, DAGNode*> &LinearizationTable, HashTable <string, DAGNode*> *PtrMapForNonLinearization);


// Push back LMC into APList if (1) LMC is not true/false and (2) LMC is already present
void pushBackIntoAPList(list<DAGNode*> &APList, DAGNode* LMC);


// Eliminate quantifiers from the conjunction using Fourier-Motzkin style QE where the variable has even coefficient
DAGNode* applyFourierMotzkinForEvenCoefficients(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable, list<DAGNode*> &APListForDD);


// Find result of \exists VarToElim. (First_LHS <= First_RHS)
DAGNode* applyFourierMotzkinForEvenCoefficientsForSingleLMI(DAGNode* First_LHS, DAGNode* First_RHS, string VarToElim, map<string, int> &WidthTable, list<DAGNode*> &APListForDD);


// Find result of \exists VarToElim. (First_LHS <= First_RHS)\wedge (First_RHS <= Second_RHS)
DAGNode* applyFourierMotzkinForEvenCoefficientsForLMIPairs(DAGNode* First_LHS, DAGNode* First_RHS, DAGNode* Second_RHS, string VarToElim, map<string, int> &WidthTable, list<DAGNode*> &APListForDD);

// Check if dag is a conjunction of LMCs. Return true if yes, false otherwise
bool checkIfConjunctionOfLMCs(DAGNode* Constraint);


// Version of simplifiableLMIPairFound_ForWeakLMIs which works on LMIs of the same type
bool simplifiableLMIPairFound_ForWeakLMIs_Advanced_With_Same_Operator(DAGNode* first, DAGNode* second);



// Copy the list of strings to a set of strings
void copyListToSet(list<string> &MyList, set<string> &MySet);


// Version of getModelGeneral which handles equalities, disequalities with inequations
bool getModelGeneral_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);


// Version of getModel which handles equalities, disequalities with inequations
bool getModel_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);


//  Version of ThUnsatSingleFormulaWithCEXFile which handles equalities, disequalities with inequations
bool ThUnsatSingleFormulaWithCEXFile_WithSupportForLMIs(DAGNode* f, map<string, int> &WidthTable);


// Version of ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG with support for LMIs
bool ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(DAGNode* f, map<string, int> &WidthTable);


// Version of getModelUsingSTP with support for LMIs
bool getModelUsingSTP_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable);


// Get the model in the variable --> value map Model 
// by parsing the CEX returned from yices in the file result_thunsat_single_formula.txt
void getModelFromFile(map<string, string> &Model);


// Display the variable --> value map "Model"
void showModel(map<string, string> &Model);

// Get the model in the variable --> value map Model 
// by parsing the CEX returned from STP in the file result_thunsat_single_formula.txt
void getModelFromSTPCEXFile(map<string, string> &Model);


// Show the number of zero, non-zero elements in the matrix
void countZeroNonZeroElements(Matrix &MyMatrix);


// Faster version of freeOfVars
// A problem with this is that even if predicate has something like 0*x+..., it will
// say that predicate is not free of x
bool freeOfVarsEfficient(DAGNode *predicate, set<string> &DropVars);


//check if MyPredicate is an LME with odd coefficient for any of the variables to be eliminated
bool predicateIsLMEWithOddCoefForSomeVarToElim(DAGNode* MyPredicate);


//Version of predicateIsLMEWithOddCoefForSomeVarToElim with the following differences
//1) Variables to be eliminated is a parameter; not a global variable CopyOfVarSetToElim
//2) Sets OddVariableToEliminate as the variable to be eliminated with odd coefficient in MyPredicate 
bool predicateIsLMEWithOddCoefForSomeVarToElim(DAGNode* MyPredicate, set<string> &VarSetToElim, string &OddVariableToEliminate);


// Faster version of freeOfVarModified
// A problem with this is that even if node has something like 0*x+..., it will
// say that node is not free of x
bool freeOfVarModifiedEfficient(DAGNode* node, string VarToElim);



// Return true if the global variable "PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles" is true 
// and time out has happened;
// Return false otherwise
bool checkTimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles();



// Function to drop simple implied LMIs from LMISet, before applying Layer2
// Result is obtianed in LMISetAfterDropping
void dropSimpleImpliedLMIs(set<DAGNode*> &LMISet, set<DAGNode*> &LMISetAfterDropping, int WidthOfVarToElim);



// There is a file "ConstraintsToLayer3.txt".
// Attach \exists VarsToElim. (Conjunction) to this.
void attachInputToLayer3ToConstraintsFile(set<string> &VarsToElim, DAGNode* Conjunction);


// Checks if \exists VarToElim. (Phi /\ Xi) \equiv \exists VarToElim. (Phi) 
// either using Counting Lemma or using BDDs, depending on the argument
// Layer2CheckUsingBDDs
bool isExtForLMDsAndLMIsGeneral(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, ULINT ModOfVarToElim, map<string, int> &WidthTable);


// Return the type of the given dagnode
Type getNodeType(DAGNode *node);

// End of functions

#endif	/* _ELIMINATOR_H */
