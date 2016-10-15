/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


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
#include <sys/time.h>
#include <sys/resource.h>

#include "y.tab.h"
#include "DagManager.h"
#include "DDManager.h"
#include "BitBlaster.h"
#include "ImplicationCacheTree.h"
#include "DAGEliminator.h"

#define  F_condition_chunk_size_in_lmdd_with_monniaux_to_make_it_Monniaux 10000
#define  Min_call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_for_satisfactory_truncation_to_make_it_Monniaux 1
#define  Satisfactory_truncation_in_lmdd_with_monniaux_to_make_it_Monniaux 1

using namespace std;


extern int yyparse(void);
void yyerror(char* str);

extern vector<DAGNode*> root;
extern DAGManager dm;
extern set<string> VarSetToElim;
DDManager ddm;
DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);

char* qEliminator_type=strdup("-lma");
char* Elimination_Type=strdup("-white");
char* Input_type=strdup("-smt");
string InFileName;
char* Output_type=strdup("-qelim");
int squaring_times=1;
char* Dag_Eliminator_Type=strdup("-mn");
string machine_name = "robin";
bool order_based_on_vars_to_elim = false;


time_t SMTSolver_Time_In_ImplicationCheck=0;
time_t ExSolver_Time_In_ImplicationCheck=0;
time_t Simulator_Time_In_ImplicationCheck=0;
int simplification_mode=0; // This denotes the simplification mode. If set to two, elimination is done at the time of creation of dd. i.e. inconsistent paths are not created at all. This keeps the dd always compact; but takes a lot of time in SMT solver calls. If set to one, elimination of inconsistent paths is done at selected points in the algorithm where dd can blow up. i.e. dd is allowed to blowup and then it is compressed. If set to zero, elimination of inconsistent paths is not done at all, which causes large dds but in less time

FILE *ImpCheckFile;
FILE *layer1_calls_fp;
FILE *layer2_calls_fp;
FILE *apqelim_fp;

unsigned long long int Calls_For_ImpCheck=0;
unsigned long long int Calls_For_ImpCheck_By_Simulator=0;
unsigned long long int Calls_For_ImpCheck_By_Exsolver=0;
unsigned long long int Calls_For_ImpCheck_By_SMTsolver=0;
unsigned long long int Cache_Hits_For_ImpCheck=0;
unsigned long long int Cache_Misses_For_ImpCheck=0;
unsigned long long int Calls_For_ImpCheck_By_Overlap_Checking=0;
unsigned long long int Successful_ImpCheck_Calls=0; 
unsigned long long int Failed_ImpCheck_Calls=0; 
unsigned long long int Second_Level_Cache_Hits_For_ImpCheck=0;
unsigned long long int Second_Level_Cache_Misses_For_ImpCheck=0;
unsigned long long int Show_ImpCheck_Calls=0; 

ImplicationCacheTree CacheTree;

bool make_squarings_independent=true;
bool reorderingBasedOnSharing_Enabled=true;

/* Variables to print details of apqelim */
FILE *APQElimStatusFile;
bool apqelim_status_on=true;
unsigned long long int Multivar_APQElim_Calls=0;
unsigned long long int APQElim_Calls=0;
unsigned long long int APQElim_Calls_Solved_By_QElimLMA=0;
unsigned long long int APQElim_Calls_Solved_By_DropLMDs_1=0;
unsigned long long int APQElim_Calls_Solved_By_DropLMDs=0;
unsigned long long int APQElim_Calls_Given_To_APQElim_Exp=0; 
/* Variables to print details of apqelim ends */

FILE *FILE_Blowup;
unsigned long long int apply_shrinks=0;
unsigned long long int apply_blows=0;
unsigned long long int ite_shrinks=0;
unsigned long long int ite_blows=0;
float blow_max_ite=0;
float blow_min_ite=10;
float blow_max_apply=0;
float blow_min_apply=10;

vector< set<string> > SignalClass;
map<string, int> SignalEquivalenceClass;

unsigned long long int applystepwithimpcheck_calls=0;
unsigned long long int applystepwithimpcheck_cache_non_trivial_hit=0;
unsigned long long int applystepwithimpcheck_cache_trivial_hit=0;
unsigned long long int applystepwithimpcheck_local_cache_hit=0;
unsigned long long int applyitewithimpcheck_calls=0;
unsigned long long int applyitewithimpcheck_local_cache_hit=0;

unsigned long long int applystepwithimpcheck_local_cache_max_size=0;
unsigned long long int applyitewithimpcheck_local_cache_max_size=0;
unsigned long long int applystepwithimpcheck_max_context_size=0;
unsigned long long int applyitewithimpcheck_max_context_size=0;

time_t time_in_local_hashing_in_applystepwithimpcheck=0;
time_t time_in_local_hashing_in_applyitewithimpcheck=0;

unsigned long long int applystepwithimpcheck_local_cache_trivial_hit=0;
unsigned long long int applyitewithimpcheck_local_cache_trivial_hit=0;
unsigned long long int applystepwithimpcheck_local_cache_level1_hit=0;
unsigned long long int applyitewithimpcheck_local_cache_level1_hit=0;
unsigned long long int eliminate_incons_paths_int_local_cache_level1_hit=0;
unsigned long long int max_context_size_imp_check=0;
unsigned long long int applystepwithimpcheck_external_cache_hit=0;
unsigned long long int applystepwithimpcheck_external_cache_miss=0;
unsigned long long int createitewithimpcheck_external_cache_hit=0;
unsigned long long int createitewithimpcheck_external_cache_miss=0;
unsigned long long int applystepwithimpcheck_misses=0;
unsigned long long int applyitewithimpcheck_misses=0;
unsigned long long int effect_of_reduce=0;
bool cache_tree_on=false;


// For trimming down context properly
bool trimming_context_mod_based=false;
bool trimming_context_class_based=false;
bool trimming_context_overapproximate=true; // Take care! This makes context over-approximation of the existing context. Hence, cross-checking with context will not work in general.
bool trimming_context_strategy_four=false; // Blend of class based strategy with eagerness. But exact
unsigned long long int trim_context_strategy_four_times=0;
unsigned long long int level_2_trimming_effect=0;
time_t trimming_time=0;
unsigned long long int thunsat_calls = 0;


// For Monniaux
time_t getModel_time=0;
time_t Generalize1_time=0;
time_t Generalize2_time=0;
time_t Project_time=0;
time_t Quant_Monn_time=0;
// For Monniaux ends here

// For LMDE counts
unsigned long long int Max_LMEs=0;
unsigned long long int Min_LMEs=0;
unsigned long long int Max_LMDs=0;
unsigned long long int Min_LMDs=0;
// For LMDE counts ends here

time_t time_in_layer1=0;
time_t time_in_layer2=0;

bool unrolling_mode=false;
bool printing_with_linearizing_mode=false;
bool linearization=true;


// Details needed in Layered..txt, Mon...txt, etc.
int LMECount_In_DAG=0;
int LMDCount_In_DAG=0;
int BitLevel_LMDCount_In_DAG=0;
int Initial_Dag_Size;
int Total_Vars;
int Total_Vars_to_Elim;
int Total_Bits_to_Elim;
time_t LMDD_Creation_Time;
time_t QElim_Time;
int Max_Inter_LMDD_Size;
int Number_of_autonames;

// For Layer - I

int L1_Calls=0;
time_t L1_Total_Time=0;
time_t L1_Max_Time=0;
time_t L1_Min_Time=7200;

int L1_Total_Bitwidth_of_Var_Eliminated=0;
int L1_Max_Bitwidth_of_Var_Eliminated=0;
int L1_Min_Bitwidth_of_Var_Eliminated=10000;

int L1_Calls_with_Gt_Zero_Diseqs=0;

int L1_Total_Diseqs_for_Calls_with_Gt_Zero_Diseqs=0;
int L1_Max_Diseqs_for_Calls_with_Gt_Zero_Diseqs=0;
int L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs=10000;

float L1_Total_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs=0;
float L1_Max_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs=0;
float L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs=10000;

time_t L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs=0;
time_t L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs=0;
time_t L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs=7200;

time_t L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs=0;
time_t L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs=0;
time_t L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs=7200;

// For Layer - II

int L2_Calls=0;
time_t L2_Total_Time=0;
time_t L2_Max_Time=0;
time_t L2_Min_Time=7200;

int L2_Total_Bitwidth_of_Var_Eliminated=0;
int L2_Max_Bitwidth_of_Var_Eliminated=0;
int L2_Min_Bitwidth_of_Var_Eliminated=10000;

int L2_Calls_with_Zero_Eqs=0;
int L2_Calls_with_Eqs=0;

int L2_Total_Diseqs=0;
int L2_Max_Diseqs=0;
int L2_Min_Diseqs=10000;

// New details needed for Layer -I and Layer -II

int L1_Total_Eqs_for_Calls_with_Gt_Zero_Diseqs=0;
int L1_Max_Eqs_for_Calls_with_Gt_Zero_Diseqs=0;
int L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs=10000;

int L1_Total_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs=0;
int L1_Max_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs=0;
int L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs=10000;

// New details needed for Layer -I and Layer -II ends here

// Details needed in Layered..txt, Mon...txt, etc. ends here

// Declarations for timeout
time_t time_out=900;
time_t time_out_start=0;
bool time_out_enabled=false;// timeout will happen only if this variable is true
bool timed_out=false;
// Declarations for timeout ends here


// Declarations for comparing with OmegaTest (Some of these are printed in Layered...txt also)
int NoOfCallsToAPQElim=0;
int NoOfCallsToAPQElimEasy=0;
time_t Total_Omega_Test_Time=0;
time_t Total_AP_QElim_Time=0;
bool compare_apqelim_omegatest=false;
unsigned long long Total_Omega_Test_Time_In_ms = 0;
unsigned long long Total_AP_QElim_Time_In_ms = 0;
int L1_Total_Eqs = 0;
int L1_Max_Eqs = 0;
int L1_Min_Eqs = 10000;
int L1_Total_Diseqs = 0;
int L1_Max_Diseqs = 0;
int L1_Min_Diseqs = 10000;
int L1_Total_Vars = 0;
int L1_Max_Vars = 0;
int L1_Min_Vars = 10000;
int APQElim_Total_Eqs=0;
float APQElim_Avg_Eqs=0;
int APQElim_Max_Eqs=0;
int APQElim_Min_Eqs=10000;
int APQElim_Total_Diseqs=0;
float APQElim_Avg_Diseqs=0;
int APQElim_Max_Diseqs=0;
int APQElim_Min_Diseqs=10000;
int APQElim_Total_Vars=0;
float APQElim_Avg_Vars=0;
int APQElim_Max_Vars=0;
int APQElim_Min_Vars=10000;
int APQElim_Total_VarsToElim=0;
float APQElim_Avg_VarsToElim=0;
int APQElim_Max_VarsToElim=0;
int APQElim_Min_VarsToElim=10000;
int APQElim_Total_Bitwidth=0;
float APQElim_Avg_Bitwidth=0;
int APQElim_Max_Bitwidth=0;
int APQElim_Min_Bitwidth=10000;
float APQElim_Total_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs=0;
float APQElim_Avg_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs=0;
float APQElim_Max_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs=0;
float APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs=10000;
int APQElim_Calls_GtZero_Diseqs=0;
int APQElim_Total_Eqs_for_Calls_GtZero_Diseqs=0;
float APQElim_Avg_Eqs_for_Calls_GtZero_Diseqs=0;
int APQElim_Max_Eqs_for_Calls_GtZero_Diseqs=0;
int APQElim_Min_Eqs_for_Calls_GtZero_Diseqs=10000;
int APQElim_Total_Diseqs_for_Calls_GtZero_Diseqs=0;
float APQElim_Avg_Diseqs_for_Calls_GtZero_Diseqs=0;
int APQElim_Max_Diseqs_for_Calls_GtZero_Diseqs=0;
int APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs=10000;
int APQElim_Total_Bitwidth_for_Calls_GtZero_Diseqs=0;
float APQElim_Avg_Bitwidth_for_Calls_GtZero_Diseqs=0;
int APQElim_Max_Bitwidth_for_Calls_GtZero_Diseqs=0;
int APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs=10000;
// Declarations for comparing with OmegaTest end here (Some of these are printed in Layered...txt also)

// The time details for layers in milliseconds
unsigned long long int L1_Total_Time_In_ms=0;
unsigned long long int L1_Max_Time_In_ms=0;
unsigned long long int L1_Min_Time_In_ms=100000; // This is 100 secs.
unsigned long long int L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms=0;
unsigned long long int L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms=0;
unsigned long long int L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms=100000;
unsigned long long int L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms=0;
unsigned long long int L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms=0;
unsigned long long int L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms=100000;
unsigned long long int L2_Total_Time_In_ms=0;
unsigned long long int L2_Max_Time_In_ms=0;
unsigned long long int L2_Min_Time_In_ms=100000;
// The time details for layers in milliseconds end here

// Declarations for comparing with Bit-Blasting
bool compare_apqelim_blasting=false;
unsigned long long int Total_Blasting_Time_In_ms=0;
time_t Total_Blasting_Time=0;
bool bitblasting_with_scopereduction=true;
// Declarations for comparing with Bit-Blasting end here

// Declarations for LMDD:BDD
int bdd_index=0;
map<string, int> NameIndexMap;
map<int, string> IndexNameMap;
int frame_number_arg=-1;
// Declarations for LMDD:BDD end here


// For outputting 
string output_file_for_bmc="";
string cache_file_for_bmc="";
map<int, list<int> > cache_for_bmc;
// For outputting

// For selecting the generalize2 strategy; default=2=our  strategy
int generalize2_strategy = 2;
// For selecting the generalize2 strategy; default=2=our  strategy

bool cav = false;
bool limited_options = false;

// For selecting outputting the conjunctions in SMTLib format
bool output_conjunctions = false;
// For selecting outputting the conjunctions in SMTLib format

string smt_conjunct_strategy = "apqelim";


// For evaluation and finding the unsat core
bool core_finding=false;// evaluate and find unsat. core
bool core_finding_without_evaluation=false;// find unsat. core and no evaluation prior to that
map<string, string> Model_to_Evaluate;
string model_file="";
string evaluation_result_in_core_file="";
string core_result_in_core_file="";
unsigned long long int unsatcore_duration_ms;
unsigned long long int zminimal_duration_ms;

// For evaluation and finding the unsat core ends here

// For SAT using blasting + QBF
bool sat_using_qbf=false;
// For SAT using blasting + QBF ends here

// For SAT using STP
bool sat_using_stp=false;
// For SAT using STP ends here

// For selecting SMT solver to be used internally
string internal_solver = "stp";
// For selecting SMT solver to be used internally ends here

// For details of QE for bit-vector constraints
int EquationCount_In_DAG=0;
int DisequationCount_In_DAG=0;
int BitLevel_DisequationCount_In_DAG=0;
int NonBitLevel_DisequationCount_In_DAG=0;
int EquationWithSelsConcatsCount_In_DAG=0;
int BitLevel_DisequationWithSelsConcatsCount_In_DAG=0;
int NonBitLevel_DisequationWithSelsConcatsCount_In_DAG=0;
int ImpureVarsToQElimCount=0;
int PureVarsToQElimCount=0;

// For performance analysis

bool log_for_per_analysis = true;
unsigned long long int L1_L2_Total_Time_In_ms_For_Conjunctions=0;
unsigned long long int dag2matrix_Total_Time_In_ms_For_Conjunctions=0;
unsigned long long int matrix2dag_Total_Time_In_ms_For_Conjunctions=0;
// For details of QE for bit-vector constrainsts end here


// For LMI handling

bool LMI_handling_enabled = false;
bool Octagons = false;
unsigned long long int z3_timeout_ms=0;// Timeout for z3 when used in semanticScopeReduction. Default is no timeout
bool debug_blasting = false;// debug_blasting is to check correctness of bit-blasting
bool exactnesscheck_on = false;// to check exactness of QE using STP/yices
bool exactness_checking_cvc3 = true;// to check exactness of QE using CVC3
extern int WidthInteger; // Width of integer when the input is supplied as LIA
extern bool LIA_Input; // Mode to specify that the input is LIA
extern bool LIA_Input_Signed; // Flag to indicate that the LIA input is signed
extern unsigned long long int ModWidthInteger; // 2^(WidthInteger)
extern unsigned long long int ModWidthIntegerHalf; // (2^(WidthInteger))/2
map<DAGNode*, DAGNode*> BlastedDAGToOriginalDAGMapForPredicates; // Maps a dag node in 
FILE* LMIQEOutputDataFp;// output file for printing relevant results for the LMI case
FILE* LMIQEImportantDataFp;// output file for printing contributions of different layers
// Following are the data we need 
int LMIMonniauxCounter=0;
int LMINoVarsToElim=0;
int LMINoLMIs=0;
int LMINoLMEs=0;
int LMINoLMDs=0;
int LMINoVars=0;
unsigned long long int LMITotalBits=0;
unsigned long long int LMIBitsToElim=0;
int LMIVarsInputToLayer2=0;
int LMIVarsInputToLayer3=0;
int LMIVarsInputToLayer4=0;
unsigned long long int LMILayer1Time=0;
unsigned long long int LMILayer2Time=0;
unsigned long long int LMILayer3Time=0;
unsigned long long int LMILayer4Time=0;



bool LMI_QE_using_blasting=false;
// When true, this calls Monniaux recursively on conjunctions
HashTable <string, DAGNode*> Monniaux_HT; 
map<string, int> VariableMap;
unsigned long long int Monniaux_HT_hits=0;
unsigned long long int Monniaux_HT_misses=0;

FILE* LMIQEImportantDataFourierMotzkinFp;// output file for printing contributions of different layers for the Fourier-Motzkin type calls
FILE* LMIQEImportantDataAdditionalFp;// output file for printing contributions of different layers for the calls made from Fourier-Motzkin type calls
int FMTypeConjunctionCounter=1;
int AdditionalConjunctionCounter=1;

int BlastingConjunctionCounter=0;
unsigned long long int LMIBlastingTime=0;



//Variables for collecting data on layer3
int LMILayer3Calls=0;
int LMILayer3CallsTimedOut=0;
int LMILayer3CallsFinished=0;


int LMILayer3CallsSCAchieved=0;
int LMILayer3CallsNoSC=0;
int LMILayer3CallsTrue=0;


FILE* LMIQEFinishedLayer3DataFp;

FILE* LMIQEFinishedLayer3TimedOutDataFp;
FILE* LMIQEFinishedLayer3SCAchievedDataFp;
FILE* LMIQEFinishedLayer3NoSCDataFp;
FILE* LMIQEFinishedLayer3TrueDataFp;
bool SwitchOffLayer1=false;
bool SwitchOffLayer2=false;


FILE *LMIQEImportantDataBlastingFp;
int LMIProjectBlastOriginalNoVarsToElim=0;
int LMIProjectBlastOriginalNoLMCs=0;
unsigned long long int LMIProjectBlastOriginalNoBitsToElim=0;
int LMIProjectBlastToBlastNoVarsToElim=0;
int LMIProjectBlastToBlastNoLMCs=0;
unsigned long long int LMIProjectBlastToBlastNoBitsToElim=0;


FILE *LMIQELayer1EffectivenessFp;
int LMIPostLayer1NoVarsToElim=0;
int LMIPostLayer1NoLMCs=0;
unsigned long long int LMIPostLayer1NoBitsToElim=0;


bool LMI_QE_using_OT=false;
int OTConjunctionCounter=0;
unsigned long long int LMIOTTime=0;
FILE* LMIQEImportantDataOTFp;



bool SwitchOffLayer3=true;
bool order_lmes_before_lmis=false;//For LMDDs involving LMEs, LMDs, and LMIs
bool order_lmes_before_lmis_in_normalization = false;//For LMDDs involving LMEs, LMDs, and LMIs
bool order_lmcs_for_small_lmdds_in_normalization = false;//For LMDDs involving LMEs, LMDs, and LMIs


bool use_monniaux_type_simplification_in_dd=false;// Use Monniaux type simplifications in QE using LMDDs
bool SMTCheckBeforeCombining_in_monniaux_type_simplification_in_dd=false;
DAGNode* PartialResult_In_LMDDWithMonniauxTypeSimplifications;


unsigned long long int call_LMDD_Monniaux_SingleVar_calls=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_on_zero=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_hits=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat=0;
unsigned long long int LMDD_Monniaux_SingleVar_calls=0;
time_t Time_in_SMT_calls=0;

bool check_effectiveness_of_first_smt_solver_call=false;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_misses_on_sat=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_misses_on_sat_sat_with_pi=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_misses_on_sat_unsat_with_pi=0;

unsigned long long int call_LMDD_Monniaux_SingleVar_calls_misses_with_project=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_misses_without_project=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_first_smt_calls=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_second_smt_calls=0;
time_t call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_total_time=0;
time_t call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time=0;

bool check_f_condition_in_lmdd_with_monniaux=false;
bool check_h_condition_in_lmdd_with_monniaux=false;
bool check_path_condition_in_lmdd_with_monniaux=false;
bool check_simplified_path_condition_in_lmdd_with_monniaux=false;
bool pure_lmdd_mode=false;
bool local_constraint_as_single_edge_mode=true;
DDNode* current_root_LMDD_node_in_lmdd_with_monniaux;

bool check_f_condition_in_chunks_in_lmdd_with_monniaux=true;

int f_condition_chunk_size_in_lmdd_with_monniaux=F_condition_chunk_size_in_lmdd_with_monniaux_to_make_it_Monniaux;
//prev.value was 10

unsigned long long int call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_after_chunk_projects=0;
unsigned long long int min_call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_for_satisfactory_truncation=Min_call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_for_satisfactory_truncation_to_make_it_Monniaux;
//prev.value was 100

unsigned long long int call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat_after_chunk_projects=0;

float satisfactory_truncation_in_lmdd_with_monniaux=Satisfactory_truncation_in_lmdd_with_monniaux_to_make_it_Monniaux;
//prev.value was 0.05

bool use_original_f_for_checking_in_lmdd_with_monniaux=true;

unsigned long long int call_LMDD_Monniaux_SingleVar_calls_to_simplified_path_condition=0;
unsigned long long int call_LMDD_Monniaux_SingleVar_calls_to_simplified_path_condition_sat=0;


FILE* hashingFp;


bool use_path_based_strategy_in_lmdd_monniaux=false;// Use path based strategy for QE with
// Monniuax and LMDD type simplifications
bool do_monniaux_at_start_up_in_use_path_based_strategy_in_lmdd_monniaux=true;
int monniaux_calls_at_start_up_in_use_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int project_calls_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int time_in_start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int max_path_depth_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int hit_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int zero_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int one_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int qe_lmdd_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int smt_calls_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int time_in_smt_calls_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int unsat_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;

float NoVarsToElim_To_NoVars_Ratio_For_Monniaux=0.5;//Fixed as 0.5 for all sorts benchmarks based on CAV'11 work
int NoVarsRemaining_For_Monniaux=75;//Fixed as 75 for all sorts benchmarks based on CAV'11 work 
int Lowest_Support_Nodes_For_QE_LMDD=0;
int  Highest_Level_For_QE_LMDD=2;
int Lowest_Size_For_QE_LMDD=500;//100 earlier
int select_strategy_calls=0;
int select_strategy_ddfreeofvar=0;
int select_strategy_dagresolve=0;
int select_strategy_lownodeswith_lmi_in_support=0;
int select_strategy_size=0;
int select_strategy_for_monniax=0;
int select_strategy_for_lmdd_default=0;
int select_strategy_lowlevel=0;

unsigned long long int time_spent_in_qe_lmdd=0;
FILE* PathBasedStrategyFp;
bool apply_lmebased_simplification_on_sat_path=true;
bool use_dag_instead_of_lmdd_in_smt_calls_in_path_based_strategy_in_lmdd_monniaux=true;
DAGNode* original_dag_in_path_based_strategy_in_lmdd_monniaux;
bool debug_check_in_path_based_strategy_in_lmdd_monniaux=false;


bool see_WhatWouldHappenWithTopLevelStrategy=true;
string WhatWouldHappenWithTopLevelStrategy="unknown";
bool remove_already_eliminated_variables_in_path_based_strategy = true;
unsigned long long int total_generalization_and_projection_time=0;
float avg_generalization_and_projection_time=0;
unsigned long long int total_vars_to_elim_in_projects=0;
float avg_vars_to_elim_in_projects=0;
int number_of_variables_for_which_no_elimination_needed=0;
list<string> OrderInWhichVariablesAreEliminated;
string FinalQEDoneByWhom;

unsigned long long int startup_project_calls_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int start_up_qe_lmdd_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int start_up_time_spent_in_qe_lmdd=0;
unsigned long long int free_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int dagresolve_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int nonterminal_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int computed_offpathchild_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int hashed_offpathchild_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int zero_offpathchild_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int one_offpathchild_in_path_based_strategy_in_lmdd_monniaux=0;
unsigned long long int project_calls_on_path_in_path_based_strategy_in_lmdd_monniaux=0;

unsigned long long int int_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar=0;
unsigned long long int int_time_spent_in_qe_lmdd_for_select_strategy_dagresolve=0;
unsigned long long int int_time_spent_in_qe_lmdd_for_select_strategy_lowlevel=0;
unsigned long long int int_time_spent_in_qe_lmdd_for_select_strategy_size=0;
unsigned long long int int_time_spent_in_qe_lmdd_for_select_strategy_default=0;
string select_strategy_basis="";
int int_select_strategy_calls=0;
int int_select_strategy_ddfreeofvar=0;
int int_select_strategy_dagresolve=0;
int int_select_strategy_lownodeswith_lmi_in_support=0;
int int_select_strategy_size=0;
int int_select_strategy_for_monniax=0;
int int_select_strategy_for_lmdd_default=0;
int int_select_strategy_lowlevel=0;

unsigned long long int ext_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar=0;
unsigned long long int ext_time_spent_in_qe_lmdd_for_select_strategy_dagresolve=0;
unsigned long long int ext_time_spent_in_qe_lmdd_for_select_strategy_lowlevel=0;
unsigned long long int ext_time_spent_in_qe_lmdd_for_select_strategy_size=0;
unsigned long long int ext_time_spent_in_qe_lmdd_for_select_strategy_default=0;
int ext_select_strategy_calls=0;
int ext_select_strategy_ddfreeofvar=0;
int ext_select_strategy_dagresolve=0;
int ext_select_strategy_lownodeswith_lmi_in_support=0;
int ext_select_strategy_size=0;
int ext_select_strategy_for_monniax=0;
int ext_select_strategy_for_lmdd_default=0;
int ext_select_strategy_lowlevel=0;

bool PerformProjectsInPathBasedStrategyAtLMDDLevel = false;//The projects in 
// the path based strategy are like \exists X.(h /\ C_{x} /\ C_{\neg x}), not like
// \exists X.(D_{x} /\ D_{\neg x} /\ C_{x} /\ C_{\neg x})

bool UseMonniauxEnabledAllVariableStrategy = false;// When enabled, this
// performs all variable elimination with Monniaux type simplifications turned on
bool debug_check_in_use_monniaux_enabled_all_variable_strategy = false;
//Statitics on Monniaux enabled all variable strategy
FILE* MonEnabledAllVarsFp;
unsigned long long int CallsToQEWithMonEnabledAllVarStrategy = 0;
unsigned long long int CallsToQEWithMonEnabledAllVarStrategyGlobalHashed = 0;
unsigned long long int CallsToQEWithMonEnabledAllVarStrategyOnTerminals = 0;
unsigned long long int CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsFreeOfAllVars = 0;
unsigned long long int CallsToGetSimplifiedInstance = 0;
unsigned long long int CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsWhereCallToGetSimplifiedInstanceSolved = 0;
unsigned long long int TimeSpentInGetSimplifiedInstance = 0;


bool ConvertProblemIntoDisjunctionOfTailedTriangles = false;// When enabled, the original
// QE problem on LMDDs, \exists X.(f) is converted into \exists X. (f_1 /\ C_1) \/ .... \/ \exists X. (f_n /\ C_n)
// and then each of them are solved using appropriate QE strategy
bool debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy = false;
FILE* ConversionToDisjunctionOfProblemsFp;
unsigned long long int NumberOfTailedTriangles = 0;
unsigned long long int NumberOfTailedTrianglesToQE = 0;
unsigned long long int TotalSMTTimeInConversionToDisjunctionOfTailedTriangles = 0;
unsigned long long int TotalQETimeInConversionToDisjunctionOfTailedTriangles = 0;

unsigned long long int TotalSizeOfTriangles = 0;
unsigned long long int TotalSizeOfTails = 0;
unsigned long long int TotalVarsToElimFromTailedTriangles = 0;
unsigned long long int TotalVarsEliminatedByLMEsFromTailedTriangles = 0;
unsigned long long int TotalVarsEliminatedFreeTrianglesFromTailedTriangles = 0;
unsigned long long int TotalRemainingVarsEliminatedFromTailedTriangles = 0;

unsigned long long int TimeSpentInProjectsInQE_LMDD = 0;

float AvgSizeOfTriangles = 0;
float AvgSizeOfTails = 0;
float AvgVarsToElimFromTailedTriangles = 0;
float AvgVarsEliminatedByLMEsFromTailedTriangles = 0;
float AvgVarsEliminatedFreeTrianglesFromTailedTriangles = 0;
float AvgRemainingVarsEliminatedFromTailedTriangles = 0;

HashTable <string, DDNode*> PerVariableHTForDagResolve_WithLMISupport;
//This HT stores \exists x.(f /\ e) = f'

set<string> CopyOfVarSetToElim;
map<DDNode*, pair<int, int> > PathCountsOfDDNodes;//Each entry is like 
// ddnode* ---> (#paths originating at the DDNode with LMEs in proper form containing any of the variables to be
// eliminated passing through right child, #paths originating at the DDNode with LMEs in proper form containing any of the variables to be
// eliminated passing through left child)


FILE* ConversionToDisjunctionOfProblemsWithLMEsFp;
bool ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification = false;// When enabled, the original
// QE problem on LMDDs, \exists X.(f) is converted into \exists X. (f_1 /\ C_1) \/ .... \/ \exists X. (f_n /\ C_n)
// and then each of them are solved using appropriate QE strategy similar to the case with ConvertProblemIntoDisjunctionOfTailedTriangles.
// Additionally 1) LME based simplification is applied as and when the LME is encountered and
// 2) No explicit list of tailed triangles is created.

unsigned long long int NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplificationFalse = 0;
unsigned long long int NoOfCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries = 0;
unsigned long long int NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries = 0;
unsigned long long int NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByLMEs = 0;
unsigned long long int NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByHT = 0;
unsigned long long int TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForLMEs = 0;
unsigned long long int TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForHT = 0;

unsigned long long int NonecasesInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int SATCallsInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int SATCallsInConProbDisjTailTriaWithMESimplificationWhichAreSat = 0;
unsigned long long int SATCallsInConProbDisjTailTriaWithMESimplificationWhichAreUnsat = 0;
unsigned long long int TimeInSATCallsInConProbDisjTailTriaWithMESimplification = 0;


unsigned long long int QE_LMDDCallsInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int TimeInQE_LMDDCallsInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int LeftcasesInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int RightcasesInConProbDisjTailTriaWithMESimplification = 0;
int MaximumSizeOfLMDDForQELMDDInDisjunctionStrategy = 250;
unsigned long long int TotalDDSizeInNonecasesInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int TotalCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int TotalNegCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int TotalCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification = 0;
unsigned long long int TotalNegCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification = 0;



bool LMEBasedSimplificationAsPreprocessor = false;
unsigned long long int DDSizeBeforeLMEBasedSimplificationAsPreprocessor = 0;
unsigned long long int DDSizeAfterLMEBasedSimplificationAsPreprocessor = 0;
unsigned long long int TimeInLMEBasedSimplificationAsPreprocessor = 0;
unsigned long long int TimeInSimplifyLMDDInLMEBasedSimplificationAsPreprocessor = 0;
unsigned long long int CallsToLMEBasedSimplificationAsPreprocessor = 0;
unsigned long long int HitsInLMEBasedSimplificationAsPreprocessor = 0;
unsigned long long int MissesInLMEBasedSimplificationAsPreprocessor = 0;
FILE* LMEBasedSimplificationAsPreprocessor_OutputFile;
set<string> VariablesBenefittedByLMEBasedSimplificationAsPreprocessor;
unsigned long long int TimeSpentInSMTCallsInsideDAGResolve = 0;
unsigned long long int TimeSpentInLayer1CallsInsideDAGResolve = 0;
unsigned long long int TimeSpentInHTLookUpInsideDAGResolve = 0;
unsigned long long int OriginalCallsToDAGResolve = 0;
unsigned long long int RecursiveCallsToDAGResolve = 0;
unsigned long long int RecursiveCallsToDAGResolveMissed = 0;
unsigned long long int RecursiveCallsToDAGResolveMissedNonTerminals = 0;
unsigned long long int RecursiveCallsToDAGResolveMissedNonTerminalsButFreeOfVarToElim = 0;
unsigned long long int RecursiveCallsToDAGResolveLeadingToLayer1Call = 0;
unsigned long long int RecursiveCallsToDAGResolveLeadingToLayer1CallHit = 0;
unsigned long long int TimeSpentInDDCreationInsideDAGResolve = 0;
unsigned long long int TimeSpentInDDConversionInsideDAGResolve = 0;
unsigned long long int CallsToLMEBasedSimplificationAsPreprocessorWhereLMDDIsFreeOfVar = 0;
unsigned long long int QETimeUsingLMDDs = 0;
list<int> CountsForVariablesEliminated;


HashTable <string, DAGNode*> TableForSimplifyLMCInsideSimplifyLMDD;
// hash table which stores the layer1 entries inside simplifyLMDD 


HashTable <string, DDNode*> MapForDDFromDAG;
// global hash table for DAG to DD operation
// used inside DDManager::createDDFromDAGWithoutImplicationCheck


FILE* SimplifyLMDD_OutputFile;


bool LMEBasedSimplificationInsideQELMDD = false;
FILE* QELMDD_OutputFile;
unsigned long long int call_QE_LMDD_Single_Var = 0;
unsigned long long int call_QE_LMDD_Single_Var_Hashed = 0;
unsigned long long int call_QE_LMDD_Single_Var_OnTerminalZero = 0;
unsigned long long int call_QE_LMDD_Single_Var_OnTerminalOne = 0;

unsigned long long int call_QE_LMDD_Single_Var_OnTerminalOne_Empty = 0;
unsigned long long int call_QE_LMDD_Single_Var_OnTerminalOne_Unsat = 0;
unsigned long long int call_QE_LMDD_Single_Var_OnTerminalOne_Sat = 0;

unsigned long long int call_QE_LMDD_Single_Var_FreeOfVar = 0;
unsigned long long int call_QE_LMDD_Single_Var_SimplifyLMDDCalls = 0;
unsigned long long int call_QE_LMDD_Single_Var_SimplifyLMDDCalls_Hashed = 0;
unsigned long long int call_QE_LMDD_Single_Var_SimplifyLMDDCalls_FreeOfVar = 0;

unsigned long long int call_QE_LMDD_Single_Var_TraverseDown = 0;
unsigned long long int call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var = 0;
unsigned long long int call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME = 0;
unsigned long long int call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_dd_free_of_var = 0;
unsigned long long int call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_SimplifyLMDD_calls = 0;
unsigned long long int call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_Time = 0;
unsigned long long int call_QE_LMDD_Single_Var_TraverseDown_C_bound_with_Var = 0;

set<string> VariablesBenefittedByLMEBasedSimplificationInsideQELMDD;
list<int> SizesOfEliminatedLMDDs;


bool EagerConversionToDisjunctions = false;
// when true, performs conversion to disjunctions until the # of SMT calls >= threshold t_1
// or max triangle size <= threshold t_2
int MaximumNoOfSMTCallsInEagerConversionToDisjunctions = 0;//25 initially, 50 then
int MaximumSizeOfTriangleInEagerConversionToDisjunctions = 3;//100 earlier
int SMTCallsCommittedInEagerConversionToDisjunctions = 0;
list<int> SizeOfTrianglesInEagerConversionToDisjunctions;
list<string> SatUnsatOfTrianglesInEagerConversionToDisjunctions;

bool PerformSimplifyLMDDInsidePreprocessingKeepingStructuralIsomorphism = false;


bool DemandBasedConversionToDisjunctions = false;
unsigned long long int NumberOfUnsatTailedTrianglesInDemandBasedConversionToDisjunctions = 0;
unsigned long long int NumberOfSplitTailedTrianglesInDemandBasedConversionToDisjunctions = 0;
int MaximumSizeOfTriangleInDemandBasedConversionToDisjunctions = 100;
int MaximumNoOfSMTCallsInDemandBasedConversionToDisjunctions = 0;
int SizeOfInputDDInDDBasedQE = 0;



bool ConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = false;
int MaxNoOfSMTCallsInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 5;
int MaxTriangleSizeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 20;//100 earlier
FILE* ConversionToDisjunctionOfProblemsWithSMTSolverFp;
FILE* ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp;
int NoOfInitialSMTCallsInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCallsCommitted = 0;
unsigned long long int TotalInitialSMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 0;
unsigned long long int TotalInitialQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 0;
unsigned long long int NumberOfTailedTrianglesUnsatInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 0;
unsigned long long int NumberOfTailedTrianglesToQEInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 0;
unsigned long long int TotalHierarchySMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 0;
unsigned long long int TotalHierarchyQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = 0;
string StrategyForChoosingInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = "AlwaysChooseOriginal";


bool PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = false;
time_t TimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles=450;
time_t TimeOutStart_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles=0;
bool TimedOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles=false;
time_t TimeSpentInMonniaux_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles=0;


unsigned long long int NumberOfTimesCollectedTailedTrianglesToQE = 0;
bool CollectConsecutiveSatTailedTriangles_In_ConversionToDisjunctionOfTailedTriangles = false;

bool OTAfterLayer2=false;
FILE* LMIQEImportantDataOTAfterLayer2Fp;

// For LMI handling ends here


// For blasting + outputting in DIMACS format
bool blast_and_output=false;
bool dynamic_variable_ordering_in_blast_and_output = false;
string independent_variables_file_name="";
set<string> independent_variables;
bool write_back_and_exit=false;//to write back the dag in smtlib format and exit

// For blasting + outputting as Verilog circuit
bool blast_and_output_as_verilog_circuit = false;


// Declarations for analysing the times
FILE* TimeAnalysisFourierMotzkinFp;// output file for printing the relevant data on time for Fourier-Motzkin type calls
FILE* TimeAnalysisFp;// output file for printing the relevant data on time for all calls
unsigned long long int TimePerConjunctInSMTAndGeneralize;


bool SimpleSMTChecksBeforeLayer3 = false;//perform simple SMT checks before Layer3

bool Layer2CheckUsingBDDs = false; //perform layer2 check using blasting and BDDs

unsigned long long int TotalLayer2CheckTime = 0; //Total time spent in Layer2 on checks
// For layer2 check using bdds, this includes blasting + dd creation + qe time

unsigned long long int TotalLayer2CheckTimeOnDDManipulation = 0; 
// For layer2 check using bdds, this includes dd creation + qe time

unsigned long long int TotalLayer2CheckTimeONQE = 0; 
// For layer2 check using bdds, this includes qe time

unsigned long long int TotalLayer2Checks = 0; 
// This gives the total number of layer2 checks

unsigned long long int TotalLayer2CheckTimeOnBlasting = 0;
// For layer2 check using bdds, this includes blasting

unsigned long long int TotalLayer2CheckTimeOnDDCreation = 0;
// For layer2 check using bdds, this includes dd creation

FILE* TimeLayer2ChecksFp;// output file for printing the relevant data on time for Layer2 checks
FILE* CumulativeTimeLayer2ChecksFp;// output file for printing the cumulative data on time for Layer2 checks
FILE* OutputDataLayer2ChecksFp;// output file for printing relevant data for analysis in Layer2 checks using BDDs

bool PrintLayer2Times = false;//output details of layer2 checks in LMIQEOutputDataFp
bool PrintBlastTimes = false;//output details of blasting in LMIQEOutputDataFp


// for analysis with .dat format
int layer_to_test_with_dat_file = -1;
// This function parses the lines from the file cache_file_for_bmc


// options for pre-processing
int total_number_of_constraints_to_preprocess = 0;
int total_number_of_constraints_to_preprocess_input_to_layer2 = 0;
int total_number_of_constraints_to_preprocess_input_to_layer3 = 0;
int total_number_of_variables_to_preprocess = 0;
int total_number_of_variables_to_preprocess_input_to_layer2 = 0;
int total_number_of_variables_to_preprocess_input_to_layer3 = 0;
int total_number_of_lmes_to_preprocess = 0;
int total_number_of_lmds_to_preprocess = 0;
int total_number_of_lmis_to_preprocess = 0;
unsigned long long int layer1_time_to_preprocess = 0;
unsigned long long int layer2_time_to_preprocess = 0;
// options for pre-processing end here

void parse_print_string(char *line_char, int &miss, list<int> &hits)
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
		  miss = frame;
		}
	      else
		{
		  hits.push_back(frame);
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

// This function reads the file cache_file_for_bmc, parses it, and updates the 
// map cache_for_bmc. This is used in the -black-blasting mode of QE using LMDDs i.e.
// QBV_Solve
void readcache_for_bmc()
{

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  if(cache_file_for_bmc=="")
    {
      cout<<"\nInvalid cache file in readcache_for_bmc in main.cc\n";
      exit(1);
    }

  fp = fopen(cache_file_for_bmc.c_str(), "r");
  if (fp == NULL)
    {
      cout<<"\nError while opening file "<<cache_file_for_bmc<<" for reading in main.cc\n";
      exit(1);
    }
  while ((read = getline(&line, &len, fp)) != -1) {
    string line_str(line);
    if(line_str.find("\t")!=string::npos)
      {
	int miss;
	list<int> hits;
	parse_print_string(line, miss, hits);   
	cache_for_bmc.insert(make_pair(miss, hits)); 
      }
  }
  if (line)
    {
    free(line);
    }

  fclose(fp);


  cout<<"\ncache_for_bmc\n";

  for(map<int, list<int> >::iterator it=cache_for_bmc.begin(); it!= cache_for_bmc.end(); it++)
    {
      cout<<"\nMiss : "<<it->first<<"\tHits : ";
      for(list<int>::iterator it2=(it->second).begin(); it2!=(it->second).end(); it2++)
	{
	  cout<<*it2<<"\t";
	} 
    }// end of for
}


// Function to print the details of QE from Boolean combinations of LMCs
void printDataInFiles()
{
  FILE *layer_comparison_fp = fopen("LayerComparisonData.txt", "a+");
  if(layer_comparison_fp == NULL)
    {
      cout<<"\nCould not open LayerComparisonData.txt\n";
      exit(1);
    }
  FILE *mon_lmdd_comparison_fp = fopen("MonniauxLMDDComparisonData.txt", "a+");
  if(mon_lmdd_comparison_fp == NULL)
    {
      cout<<"\nCould not open MonniauxLMDDComparisonData.txt\n";
      exit(1);
    }
  FILE *ap_qelim_fp = fopen("APQElimData.txt", "a+");
  if(ap_qelim_fp == NULL)
    {
      cout<<"\nCould not open APQElimData.txt\n";
      exit(1);
    }
  FILE *apqelim_omegatest_comparison_fp;

  if(compare_apqelim_omegatest) // Only if the comparison is enabled we need to open this file
    {
      apqelim_omegatest_comparison_fp = fopen("APQElimOmegaTestComparisonData.txt", "a+");
      if(apqelim_omegatest_comparison_fp == NULL)
	{
	  cout<<"\nCould not open APQElimOmegaTestComparisonData.txt\n";
	  exit(1);
	}
    }

  FILE *apqelim_blasting_comparison_fp;

  if(compare_apqelim_blasting) // Only if the comparison is enabled we need to open this file
    {
      apqelim_blasting_comparison_fp = fopen("APQElimBitBlastingComparisonData.txt", "a+");
      if(apqelim_blasting_comparison_fp == NULL)
	{
	  cout<<"\nCould not open APQElimBitBlastingComparisonData.txt\n";
	  exit(1);
	}
    }

  FILE *unsat_core_computation_fp;

  if(core_finding_without_evaluation || core_finding) // Only if the unsat core computation is done, we need to open this file
    {
      unsat_core_computation_fp = fopen("UnsatCoreComputationData.txt", "a+");
      if(unsat_core_computation_fp == NULL)
	{
	  cout<<"\nCould not open UnsatCoreComputationData.txt\n";
	  exit(1);
	}
    }

  float L1_Avg_Time=0;
  float L1_Avg_Bitwidth_of_Var_Eliminated=0;
  float L1_Avg_Diseqs_for_Calls_with_Gt_Zero_Diseqs=0;
  float L1_Avg_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs=0;
  float L1_Avg_Time_for_Calls_with_Gt_Zero_Diseqs=0;
  float L1_Avg_Time_for_Calls_with_Eq_Zero_Diseqs=0;
  float L2_Avg_Time=0;
  float L2_Avg_Bitwidth_of_Var_Eliminated=0;
  float L2_Avg_Diseqs=0;

  float L1_Avg_Time_In_ms=0;
  float L1_Avg_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms=0;
  float L1_Avg_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms=0;
  float L2_Avg_Time_In_ms=0;


  if(L1_Calls > 0)
    {
      L1_Avg_Bitwidth_of_Var_Eliminated =  (float)L1_Total_Bitwidth_of_Var_Eliminated/(float)L1_Calls;
      L1_Avg_Time = (float)L1_Total_Time/(float)L1_Calls;
      L1_Avg_Time_In_ms = (float)L1_Total_Time_In_ms/(float)L1_Calls;
    }

  if(L1_Calls_with_Gt_Zero_Diseqs > 0)
    {
      L1_Avg_Diseqs_for_Calls_with_Gt_Zero_Diseqs = (float)L1_Total_Diseqs_for_Calls_with_Gt_Zero_Diseqs/(float)L1_Calls_with_Gt_Zero_Diseqs;
      L1_Avg_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs = (float)L1_Total_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs/(float)L1_Calls_with_Gt_Zero_Diseqs;
      L1_Avg_Time_for_Calls_with_Gt_Zero_Diseqs = (float)L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs/(float)L1_Calls_with_Gt_Zero_Diseqs;
      L1_Avg_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms = (float)L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms/(float)L1_Calls_with_Gt_Zero_Diseqs;
    }

  if((L1_Calls - L1_Calls_with_Gt_Zero_Diseqs) > 0)
    {
      L1_Avg_Time_for_Calls_with_Eq_Zero_Diseqs = (float)L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs/((float)L1_Calls - (float)L1_Calls_with_Gt_Zero_Diseqs);
      L1_Avg_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms = (float)L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms/((float)L1_Calls - (float)L1_Calls_with_Gt_Zero_Diseqs);
    }

  if(L2_Calls > 0)
    {
      L2_Avg_Bitwidth_of_Var_Eliminated =  (float)L2_Total_Bitwidth_of_Var_Eliminated/(float)L2_Calls;
      L2_Avg_Time = (float)L2_Total_Time/(float)L2_Calls;
      L2_Avg_Diseqs = (float)L2_Total_Diseqs/(float)L2_Calls;
      L2_Avg_Time_In_ms = (float)L2_Total_Time_In_ms/(float)L2_Calls;
    }

  // We have set the minimum values as 10,000 and 7200. Set them to zeros before printing

  if(L1_Min_Time==7200)
    {
      L1_Min_Time=0;
    }
  if(L1_Min_Time_In_ms==100000)
    {
      L1_Min_Time_In_ms=0;
    }
  if(L1_Min_Bitwidth_of_Var_Eliminated==10000)
    {
      L1_Min_Bitwidth_of_Var_Eliminated=0;
    }
  if(L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs==10000)
    {
      L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs=0;
    }
  if(L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs==10000)
    {
      L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs=0;
    }
  if(L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs==7200)
    {
      L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs=0;
    }
  if(L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms==100000)
    {
      L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms=0;
    }
  if(L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs==7200)
    {
      L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs=0;
    }
  if(L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms==100000)
    {
      L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms=0;
    }
  if(L2_Min_Time==7200)
    {
      L2_Min_Time=0;
    }
  if(L2_Min_Time_In_ms==100000)
    {
      L2_Min_Time_In_ms=0;
    }
  if(L2_Min_Bitwidth_of_Var_Eliminated==10000)
    {
      L2_Min_Bitwidth_of_Var_Eliminated=0;
    }
  if(L2_Min_Diseqs==10000)
    {
      L2_Min_Diseqs=0;
    }

  // New details needed for Layer -I and Layer -II

  float L1_Avg_Eqs_for_Calls_with_Gt_Zero_Diseqs;
  float L1_Avg_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs;

  if(L1_Calls_with_Gt_Zero_Diseqs > 0)
    {
      L1_Avg_Eqs_for_Calls_with_Gt_Zero_Diseqs = (float)L1_Total_Eqs_for_Calls_with_Gt_Zero_Diseqs/(float)L1_Calls_with_Gt_Zero_Diseqs;
      L1_Avg_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs = (float)L1_Total_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs/(float)L1_Calls_with_Gt_Zero_Diseqs;
     
    }


  if(L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs==10000)
    {
      L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs=0;
    }
  if(L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs==10000)
    {
      L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs=0;
    }

  // New details needed for Layer -I and Layer -II ends here

  // Details needed for AP_QElim calls : To be printed to Layered...txt and if -compare_apqelim_omegatest is true, to be printed to APQElimOmegaTestComparisonData.txt

  if(strcmp(Elimination_Type,"-white")==0 && strcmp(Dag_Eliminator_Type, "-dd")==0)// for qualmode, apqelim calls are basically calls to apqelim_easy i.e. to L1
    {
      NoOfCallsToAPQElim = L1_Calls;
      NoOfCallsToAPQElimEasy = L1_Calls; 

      float L1_Avg_Eqs = 0;
      float L1_Avg_Diseqs = 0;
      float L1_Avg_Vars = 0;
	
      if(L1_Calls > 0)
	{
	  L1_Avg_Eqs =  (float)L1_Total_Eqs/(float)L1_Calls;
	  L1_Avg_Diseqs = (float)L1_Total_Diseqs/(float)L1_Calls;
	  L1_Avg_Vars = (float)L1_Total_Vars/(float)L1_Calls;
	}

      if(L1_Min_Eqs==10000)
	{
	  L1_Min_Eqs=0;
	}
      if(L1_Min_Diseqs==10000)
	{
	  L1_Min_Diseqs=0;
	}
      if(L1_Min_Vars==10000)
	{
	  L1_Min_Vars=0;
	}

      APQElim_Avg_Eqs = L1_Avg_Eqs;
      APQElim_Max_Eqs = L1_Max_Eqs;
      APQElim_Min_Eqs = L1_Min_Eqs;
      APQElim_Avg_Diseqs = L1_Avg_Diseqs;
      APQElim_Max_Diseqs = L1_Max_Diseqs;
      APQElim_Min_Diseqs = L1_Min_Diseqs;
      APQElim_Avg_Vars = L1_Avg_Vars;
      APQElim_Max_Vars = L1_Max_Vars;
      APQElim_Min_Vars = L1_Min_Vars;
      APQElim_Avg_VarsToElim = 1;
      APQElim_Max_VarsToElim = 1;
      APQElim_Min_VarsToElim = 1;
      APQElim_Avg_Bitwidth = L1_Avg_Bitwidth_of_Var_Eliminated;
      APQElim_Max_Bitwidth = L1_Max_Bitwidth_of_Var_Eliminated;
      APQElim_Min_Bitwidth = L1_Min_Bitwidth_of_Var_Eliminated;
      APQElim_Avg_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs = L1_Avg_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Max_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs = L1_Max_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs = L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Calls_GtZero_Diseqs = L1_Calls_with_Gt_Zero_Diseqs;

      APQElim_Avg_Eqs_for_Calls_GtZero_Diseqs = L1_Avg_Eqs_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Max_Eqs_for_Calls_GtZero_Diseqs = L1_Max_Eqs_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Min_Eqs_for_Calls_GtZero_Diseqs = L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Avg_Diseqs_for_Calls_GtZero_Diseqs = L1_Avg_Diseqs_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Max_Diseqs_for_Calls_GtZero_Diseqs = L1_Max_Diseqs_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs = L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Avg_Bitwidth_for_Calls_GtZero_Diseqs = L1_Avg_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Max_Bitwidth_for_Calls_GtZero_Diseqs = L1_Max_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs;
      APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs = L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs;
    }
  else if(strcmp(Dag_Eliminator_Type, "-mn")==0) // Monniaux
    {
      NoOfCallsToAPQElimEasy = L1_Calls; 

      if(NoOfCallsToAPQElim > 0)
	{
	  APQElim_Avg_Eqs  =  (float)APQElim_Total_Eqs/(float)NoOfCallsToAPQElim;
	  APQElim_Avg_Diseqs  = (float)APQElim_Total_Diseqs/(float)NoOfCallsToAPQElim;
	  APQElim_Avg_Vars  = (float)APQElim_Total_Vars/(float)NoOfCallsToAPQElim;
	  APQElim_Avg_VarsToElim  = (float)APQElim_Total_VarsToElim/(float)NoOfCallsToAPQElim;
	  APQElim_Avg_Bitwidth  = (float)APQElim_Total_Bitwidth/(float)NoOfCallsToAPQElim;
	}

      if(APQElim_Calls_GtZero_Diseqs > 0)
	{
	  APQElim_Avg_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs  = (float)APQElim_Total_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs/(float)APQElim_Calls_GtZero_Diseqs;
	  APQElim_Avg_Eqs_for_Calls_GtZero_Diseqs  = (float)APQElim_Total_Eqs_for_Calls_GtZero_Diseqs/(float)APQElim_Calls_GtZero_Diseqs;
	  APQElim_Avg_Diseqs_for_Calls_GtZero_Diseqs = (float)APQElim_Total_Diseqs_for_Calls_GtZero_Diseqs/(float)APQElim_Calls_GtZero_Diseqs;
	  APQElim_Avg_Bitwidth_for_Calls_GtZero_Diseqs = (float)APQElim_Total_Bitwidth_for_Calls_GtZero_Diseqs/(float)APQElim_Calls_GtZero_Diseqs;
	}



      if(APQElim_Min_Eqs==10000)
	{
	  APQElim_Min_Eqs=0;
	}
      if(APQElim_Min_Diseqs==10000)
	{
	  APQElim_Min_Diseqs=0;
	}
      if(APQElim_Min_Vars==10000)
	{
	  APQElim_Min_Vars =0;
	}
      if(APQElim_Min_VarsToElim==10000)
	{
	  APQElim_Min_VarsToElim=0;
	}
      if(APQElim_Min_Bitwidth==10000)
	{
	  APQElim_Min_Bitwidth=0;
	}
      if(APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs==10000)
	{
	  APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs=0;
	}
      if(APQElim_Min_Eqs_for_Calls_GtZero_Diseqs==10000)
	{
	  APQElim_Min_Eqs_for_Calls_GtZero_Diseqs=0;
	}
      if(APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs==10000)
	{
	  APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs=0;
	}
      if(APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs==10000)
	{
	  APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs=0;
	}

    }
 
  
  // Details needed for AP_QElim calls ends here


  string Dag_Eliminator_Type_String;
  if(strcmp(Dag_Eliminator_Type, "-mn")==0)
    {
      Dag_Eliminator_Type_String = "Monniuax";

      if(generalize2_strategy == 3)
	{
	  Dag_Eliminator_Type_String += "(LPAR_2008_Generalize2)";
	}
    }
  else
    {
      if(strcmp(Elimination_Type, "-white")==0)
	{
	  Dag_Eliminator_Type_String = "LMDD(QualMoDE)";
	  if(order_based_on_vars_to_elim)
	    {
	      Dag_Eliminator_Type_String += "(order_based_on_vars_to_elim)";
	    }

	  if(simplification_mode==1)
	    {
	      Dag_Eliminator_Type_String += "(s.mode=1)";
	
	      if(trimming_context_overapproximate)
		{
		  Dag_Eliminator_Type_String += "(t.mode=overapproximate)";
		}
	      else
		{
		  Dag_Eliminator_Type_String += "(t.mode=exact)";
		}

	    }
	  else if(simplification_mode==2)
	    {
	      Dag_Eliminator_Type_String += "(s.mode=2)";

	      if(trimming_context_overapproximate)
		{
		  Dag_Eliminator_Type_String += "(t.mode=overapproximate)";
		}
	      else
		{
		  Dag_Eliminator_Type_String += "(t.mode=exact)";
		}
	    }
	}
      else if(strcmp(Elimination_Type, "-black") == 0)
	{
	  Dag_Eliminator_Type_String = "LMDD(All_Path_QElim)";
	}
      else if(strcmp(Elimination_Type, "-gray") == 0)
	{
	  Dag_Eliminator_Type_String = "LMDD(All_Path_QElim_With_Simplications)";
	}
    }


  if(!timed_out)
    {

      fprintf(layer_comparison_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t",InFileName.c_str(), LMECount_In_DAG, LMDCount_In_DAG, BitLevel_LMDCount_In_DAG, Initial_Dag_Size, Total_Vars, Dag_Eliminator_Type_String.c_str(), QElim_Time, Max_Inter_LMDD_Size, LMDD_Creation_Time, getModel_time, Total_Vars_to_Elim, Total_Bits_to_Elim);
    }
  else
    {
      fprintf(layer_comparison_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%s\t>%d\t%d\t%d\t%d\t%d\t%d\t",InFileName.c_str(), LMECount_In_DAG, LMDCount_In_DAG, BitLevel_LMDCount_In_DAG, Initial_Dag_Size, Total_Vars, Dag_Eliminator_Type_String.c_str(), time_out, Max_Inter_LMDD_Size, LMDD_Creation_Time, getModel_time, Total_Vars_to_Elim, Total_Bits_to_Elim);
    }

  fprintf(layer_comparison_fp, "%d\t%f\t%d\t%d\t%f\t%d\t%d\t%d\t%f\t%d\t%d\t%f\t%f\t%f\t%f\t%d\t%d\t%f\t%d\t%d\t",L1_Calls, L1_Avg_Time, L1_Max_Time, L1_Min_Time, L1_Avg_Bitwidth_of_Var_Eliminated, L1_Max_Bitwidth_of_Var_Eliminated, L1_Min_Bitwidth_of_Var_Eliminated, L1_Calls_with_Gt_Zero_Diseqs, L1_Avg_Diseqs_for_Calls_with_Gt_Zero_Diseqs, L1_Max_Diseqs_for_Calls_with_Gt_Zero_Diseqs, L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs, L1_Avg_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs, L1_Max_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs, L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs, L1_Avg_Time_for_Calls_with_Gt_Zero_Diseqs, L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs, L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs, L1_Avg_Time_for_Calls_with_Eq_Zero_Diseqs, L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs, L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs);

  fprintf(layer_comparison_fp, "%d\t%f\t%d\t%d\t%f\t%d\t%d\t%d\t%d\t%f\t%d\t%d\t%s\t",L2_Calls, L2_Avg_Time, L2_Max_Time, L2_Min_Time, L2_Avg_Bitwidth_of_Var_Eliminated, L2_Max_Bitwidth_of_Var_Eliminated, L2_Min_Bitwidth_of_Var_Eliminated, L2_Calls_with_Zero_Eqs, L2_Calls_with_Eqs, L2_Avg_Diseqs, L2_Max_Diseqs, L2_Min_Diseqs, machine_name.c_str());

  // New details needed for Layer -I and Layer -II

  fprintf(layer_comparison_fp, "%f\t%d\t%d\t%f\t%d\t%d\t",L1_Avg_Eqs_for_Calls_with_Gt_Zero_Diseqs, L1_Max_Eqs_for_Calls_with_Gt_Zero_Diseqs, L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs, L1_Avg_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs, L1_Max_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs, L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs);

  // New details needed for Layer -I and Layer -II end here

  // millisecond details for layers

  fprintf(layer_comparison_fp, "%llu\t%f\t%llu\t%llu\t%llu\t%f\t%llu\t%llu\t%llu\t%f\t%llu\t%llu\t%llu\t%f\t%llu\t%llu\n", L1_Total_Time_In_ms, L1_Avg_Time_In_ms, L1_Max_Time_In_ms, L1_Min_Time_In_ms, L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms, L1_Avg_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms, L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms, L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms, L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms, L1_Avg_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms, L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms, L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms, L2_Total_Time_In_ms, L2_Avg_Time_In_ms, L2_Max_Time_In_ms, L2_Min_Time_In_ms);

  // millisecond details for layers end here

  // Details for AP_QElim

  fprintf(ap_qelim_fp, "\n\n%s\t%s\t%s\t%d\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%f\t%f\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\n",InFileName.c_str(), Dag_Eliminator_Type_String.c_str(), machine_name.c_str(), NoOfCallsToAPQElim, APQElim_Calls_GtZero_Diseqs, NoOfCallsToAPQElimEasy, APQElim_Avg_Eqs, APQElim_Max_Eqs, APQElim_Min_Eqs, APQElim_Avg_Diseqs, APQElim_Max_Diseqs, APQElim_Min_Diseqs, APQElim_Avg_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs, APQElim_Max_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs, APQElim_Min_Eqs_Div_Diseqs_for_Calls_GtZero_Diseqs, APQElim_Avg_Vars, APQElim_Max_Vars, APQElim_Min_Vars, APQElim_Avg_VarsToElim, APQElim_Max_VarsToElim, APQElim_Min_VarsToElim, APQElim_Avg_Bitwidth, APQElim_Max_Bitwidth, APQElim_Min_Bitwidth, APQElim_Avg_Eqs_for_Calls_GtZero_Diseqs, APQElim_Max_Eqs_for_Calls_GtZero_Diseqs, APQElim_Min_Eqs_for_Calls_GtZero_Diseqs, APQElim_Avg_Diseqs_for_Calls_GtZero_Diseqs, APQElim_Max_Diseqs_for_Calls_GtZero_Diseqs, APQElim_Min_Diseqs_for_Calls_GtZero_Diseqs, APQElim_Avg_Bitwidth_for_Calls_GtZero_Diseqs, APQElim_Max_Bitwidth_for_Calls_GtZero_Diseqs, APQElim_Min_Bitwidth_for_Calls_GtZero_Diseqs);   

  // Details for AP_QElim ends here

  if(!timed_out)
    {
      fprintf(mon_lmdd_comparison_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%s\n",InFileName.c_str(), Initial_Dag_Size, Total_Vars, Total_Vars_to_Elim, Total_Bits_to_Elim, Dag_Eliminator_Type_String.c_str(), QElim_Time, Max_Inter_LMDD_Size, LMDD_Creation_Time, getModel_time, machine_name.c_str());
    }
  else
    {
      fprintf(mon_lmdd_comparison_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%s\t>%d\t%d\t%d\t%d\t%s\n",InFileName.c_str(), Initial_Dag_Size, Total_Vars, Total_Vars_to_Elim, Total_Bits_to_Elim, Dag_Eliminator_Type_String.c_str(), time_out, Max_Inter_LMDD_Size, LMDD_Creation_Time, getModel_time, machine_name.c_str());
    }

  if(compare_apqelim_omegatest) // Only if the comparison is enabled we need to write to this file
    {
      fprintf(apqelim_omegatest_comparison_fp, "\n\n%s\t%s\t%d\t%d\t%d\t%d\t%llu\t%llu\t%s\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\n",InFileName.c_str(), Dag_Eliminator_Type_String.c_str(), NoOfCallsToAPQElim, NoOfCallsToAPQElimEasy, Total_AP_QElim_Time, Total_Omega_Test_Time, Total_AP_QElim_Time_In_ms, Total_Omega_Test_Time_In_ms, machine_name.c_str(), APQElim_Avg_Eqs, APQElim_Max_Eqs, APQElim_Min_Eqs, APQElim_Avg_Diseqs, APQElim_Max_Diseqs, APQElim_Min_Diseqs, APQElim_Avg_Vars, APQElim_Max_Vars, APQElim_Min_Vars, APQElim_Avg_VarsToElim, APQElim_Max_VarsToElim, APQElim_Min_VarsToElim, APQElim_Avg_Bitwidth, APQElim_Max_Bitwidth, APQElim_Min_Bitwidth);   
    }
  if(compare_apqelim_blasting) // Only if the comparison is enabled we need to write to this file
    {
      fprintf(apqelim_blasting_comparison_fp, "\n\n%s\t%s\t%d\t%d\t%d\t%d\t%llu\t%llu\t%s\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%d\t%d\n",InFileName.c_str(), Dag_Eliminator_Type_String.c_str(), NoOfCallsToAPQElim, NoOfCallsToAPQElimEasy, Total_AP_QElim_Time, Total_Blasting_Time, Total_AP_QElim_Time_In_ms, Total_Blasting_Time_In_ms, machine_name.c_str(), APQElim_Avg_Eqs, APQElim_Max_Eqs, APQElim_Min_Eqs, APQElim_Avg_Diseqs, APQElim_Max_Diseqs, APQElim_Min_Diseqs, APQElim_Avg_Vars, APQElim_Max_Vars, APQElim_Min_Vars, APQElim_Avg_VarsToElim, APQElim_Max_VarsToElim, APQElim_Min_VarsToElim, APQElim_Avg_Bitwidth, APQElim_Max_Bitwidth, APQElim_Min_Bitwidth);   
    }

  fclose(layer_comparison_fp);
  fclose(mon_lmdd_comparison_fp);
  fclose(ap_qelim_fp);
  if(compare_apqelim_omegatest) // Only if the comparison is enabled we need to close this file
    {
      fclose(apqelim_omegatest_comparison_fp);
    }  
  if(compare_apqelim_blasting) // Only if the comparison is enabled we need to write to this file
    {
      fclose(apqelim_blasting_comparison_fp);
    }

  if(core_finding_without_evaluation || core_finding) // Only if the unsat core computation is done, we need to open this file
    {
      fprintf(unsat_core_computation_fp,"\n\n%llu\t%llu\n", unsatcore_duration_ms, zminimal_duration_ms);
      fclose(unsat_core_computation_fp);
    }

 
	   
}
 

// Function to print the details of QE from Boolean combinations of bit-vector constraints
void printDataInFilesForBitVectorCases()
{
  FILE *qelim_data_fp = fopen("QElimDataForNonLinearDAGs.txt", "a+");
  if(qelim_data_fp == NULL)
    {
      cout<<"\nCould not open QElimDataForNonLinearDAGs.txt\n";
      exit(1);
    }

  fprintf(qelim_data_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",InFileName.c_str(), EquationCount_In_DAG, DisequationCount_In_DAG, BitLevel_DisequationCount_In_DAG, NonBitLevel_DisequationCount_In_DAG, EquationWithSelsConcatsCount_In_DAG, BitLevel_DisequationWithSelsConcatsCount_In_DAG, NonBitLevel_DisequationWithSelsConcatsCount_In_DAG, Initial_Dag_Size, Total_Vars, Total_Vars_to_Elim, Total_Bits_to_Elim, PureVarsToQElimCount, ImpureVarsToQElimCount);

  fclose(qelim_data_fp);
}
 

// Function to print the details of QE from Boolean combinations of LMI constraints
void printDataInFilesForLMIs()
{
    
  FILE *LMIQELayer3DagDataFp = fopen("LMIQELayer3DagData.txt", "a+");
  if(LMIQELayer3DagDataFp == NULL)
    {
      cout<<"\nCould not open LMIQELayer3DagData.txt\n";
      exit(1);
    } 
  
  if(LMILayer3CallsTimedOut==0 && LMILayer3CallsSCAchieved==0 && LMILayer3CallsNoSC==0 && LMILayer3CallsTrue==0)
       fprintf(LMIQELayer3DagDataFp, "\n\n%s\t-\t-\t-\t-\n",InFileName.c_str());
  else
      fprintf(LMIQELayer3DagDataFp, "\n\n%s\t%d\t%d\t%d\t%d\n",InFileName.c_str(), LMILayer3CallsTimedOut, LMILayer3CallsSCAchieved, LMILayer3CallsNoSC, LMILayer3CallsTrue);
  
  
   fclose(LMIQELayer3DagDataFp);
    
    
    
    
  FILE *qelim_data_fp = fopen("LMIQEDagData.txt", "a+");
  if(qelim_data_fp == NULL)
    {
      cout<<"\nCould not open LMIQEDagData.txt\n";
      exit(1);
    }

 if(!timed_out)
    {
      fprintf(qelim_data_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d",InFileName.c_str(), Initial_Dag_Size, Total_Vars, Total_Vars_to_Elim, Total_Bits_to_Elim, QElim_Time, Max_Inter_LMDD_Size);
    }
  else
    {
      fprintf(qelim_data_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d",InFileName.c_str(), Initial_Dag_Size, Total_Vars, Total_Vars_to_Elim, Total_Bits_to_Elim, time_out, Max_Inter_LMDD_Size);
    }
  
  
  if(!LMEBasedSimplificationAsPreprocessor)
  {
  if(use_path_based_strategy_in_lmdd_monniaux)
        {
        fprintf(qelim_data_fp, "\tLMDD+Monniaux");
        }
  else if(UseMonniauxEnabledAllVariableStrategy)
        {
        fprintf(qelim_data_fp, "\tAllVars_QE_LMDD");
        }
  else if(ConvertProblemIntoDisjunctionOfTailedTriangles)
        {
        if(ConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls)
            {
            fprintf(qelim_data_fp, "\tDisjunctionBasedWithSMTSolver");
            }
        else
            {
            fprintf(qelim_data_fp, "\tDisjunctionBased");
            }
        }
   else
       {
         fprintf(qelim_data_fp, "\t%s", Dag_Eliminator_Type);
        }
  }
  else
  {
   if(use_path_based_strategy_in_lmdd_monniaux)
        {
        fprintf(qelim_data_fp, "\tLMDD+Monniaux+LMEBasedSimplificationPreProcessor");
        }
  else if(UseMonniauxEnabledAllVariableStrategy)
        {
        fprintf(qelim_data_fp, "\tAllVars_QE_LMDD+LMEBasedSimplificationPreProcessor");
        }
  else if(ConvertProblemIntoDisjunctionOfTailedTriangles)
        {
        fprintf(qelim_data_fp, "\t-dd(with conversion to tailed triangles)+LMEBasedSimplificationPreProcessor");
        }
   else
       {
         fprintf(qelim_data_fp, "\t%s+LMEBasedSimplificationPreProcessor", Dag_Eliminator_Type);
        }   
  }
                 
   fprintf(qelim_data_fp, "\t%s\n", machine_name.c_str());
                 
  
   fclose(qelim_data_fp);
}
 

// Function to collect details about the given QE problem instance
void getDataForComparison(DAGNode *node, set<string> &VarSetToElim, map<string, int> &WidthTable)
{
  Initial_Dag_Size = dm.getSizeOfDAG(node);
  set<string> Names;
  getDAGNames(node, Names);
  Total_Vars = Names.size();
  Total_Vars_to_Elim = VarSetToElim.size();
  Total_Bits_to_Elim = 0;

  for(set<string>::iterator it = VarSetToElim.begin(); it != VarSetToElim.end(); it++)
    {
      string variable = *it;
      map<string, int>::iterator wit = WidthTable.find(variable);
      if(wit == WidthTable.end())
	{
	  cout<<"\nVariable "<<variable<<" has no entry in WidthTable in function getDataForComparison in main.cc\n";
	  exit(1);
	}
      int number_of_bits = wit->second;
      Total_Bits_to_Elim += number_of_bits;
    } 
 
  
}


// Function to decide whether to go the Monniaux way or the LMDD way based on E/N and N-E
void findWhatWouldHappenWithTopLevelStrategy(DAGNode *node, set<string> &VarSetToElim)
{  
  set<string> Names;
  getDAGNames(node, Names);
  int NoVars = Names.size();
  int NoVarsToElim = VarSetToElim.size();
  
  float ratio = (float)NoVarsToElim/(float)NoVars;
  int NoVarsRemaining = NoVars - NoVarsToElim;
            
  //cout<<"\nNoVarsToElim = "<<NoVarsToElim<<endl;
  //cout<<"\nNoVars = "<<NoVars<<endl;
  //cout<<"\n(float)NoVarsToElim/(float)NoVars = "<<ratio<<endl;
  //cout<<"\nNoVarsRemaining = "<<NoVarsRemaining<<endl;
  
  if(ratio >= NoVarsToElim_To_NoVars_Ratio_For_Monniaux && NoVarsRemaining < NoVarsRemaining_For_Monniaux)
                {                
                WhatWouldHappenWithTopLevelStrategy = "monniaux";
                }
            else
                {
                WhatWouldHappenWithTopLevelStrategy = "lmdd";
                }
}

// Function to show the usage of the tool: This includes all the options
void usage()
{
  cout<<"\nUsage: Quantifier_Eliminator [option] [infile]\n";
  cout<<"\nOptions are\n";
  cout<<"\nInput's Type : -smt = SMT file which contains the dag on which quantifier elimination is to be applied, -mat = Conjunction of lmes and lmds in matrix format, -dat = Conjunction of lmes and lmds in new QE implementation's input format, -smt_conjunct = SMT file which contains the dag which is a conjunction of LMEs and LMDs for QELIM, -smt_conjunct_sat = SMT file which contains the dag which is a conjunction of LMEs and LMDs to be checked for satisfiability\n";
  cout<<"\nqEliminator's Type : -lma = Linear Modular Arithmetic based Eliminator, -ilp = Omega Test, -bit = Bit-blasting\n";
  cout<<"\nDag Eliminator Type : -mn = Monniaux's method of qelimination over dag, -dd = lmdd based qelimination over dag, -pr = Project, -l1b = Layer1_Blast, -l1ot = Layer1_OT, -l2ot = Layer2_OT, -l2ub = ProjectWithBDDBasedLayer2, -preprocess = Use Layer1 and Layer2 for preprocessing conjunctions in smtlib files\n";
  cout<<"\n-linearize = on/off: on = to linearize the input dag. i.e. to replace the non-linear operations such as selection, concatenation etc. by fresh bit-vector variables. off = no linearization applied. note that only -mn type QELIM is (partially) implemented in such cases (on by default)\n";
  cout<<"\nElimination's Type : -white = White Box Type, -black = Black Box Type, -gray = Gray Box Type, -allsmt = All-SMT Type\n";
  cout<<"\n-timeout = Specify the timeout in seconds\n";
  cout<<"\n-memoryout = Specify the memoryout in KBytes\n";
  cout<<"\nOutput's Type : -qelim = Quantifier Elimination, -umc = Squaring\n";
  cout<<"\n-impchecks2 = Use SMT solver to eliminate inconsistent paths by construction\n";
  cout<<"\n-impchecks1 = Use SMT solver to eliminate inconsistent paths after construction\n";
  cout<<"\n-impchecks0 = No inconsistent path elimination\n";
  cout<<"\n-times = i where i is the number of squarings (valid if Output's Type = -umc)\n";
  cout<<"\n-unroll = to print the input dag after unrolling the times specified in the -times parameter\n";
  cout<<"\n-linearize-and-print = to print the input dag after linearization\n";
  cout<<"\n-evaluate_and_get_core = evaluate the input dag and get it's unsatisfiable core\n";
  cout<<"\n-get_core = get the unsatisfiable core with no prior evaluation\n";
  cout<<"\n-model=model file where model file is a txt file containing an assignment of values to variables in the dag (valid only with -evaluate_and_get_core and -get_core)\n";
  cout<<"\n-evaluation_result=file name where file name is the file containing the result of evaluating the dag on assignment supplied (valid only with -evaluate_and_get_core)\n";  
  cout<<"\n-core_result=file name where file name is the file containing the signals in unsat core i.e. subset of signals in support of dag sufficient to evaluate the dag to the given value (valid only with -evaluate_and_get_core and -get_core)\n";
  cout<<"\n-order_based_on_vars_to_elim = to order the lmdes in the initial dd based on the variables to be eliminated i.e. make the lmdes which contain the variables to be eliminated upper in the initial dd (lower in the order)\n";
  cout<<"\n-compare_apqelim_omegatest = to compare the performance of AP_QElim with OmegaTest when used for quantifier elimination from conjunctions of LMDEs arising from the qualmode/Monniaux \n";
  cout<<"\n-compare_apqelim_blasting = to compare the performance of AP_QElim with bit-blasting when used for quantifier elimination from conjunctions of LMDEs arising from the qualmode/Monniaux ; Currently this calls AP_QElim and prints the inputs to bit-blaster in files\n";
  cout<<"\n-black_blasting = to perform all path quantifier elimination with BV_Solve at the terminals\n";
  cout<<"\n-use_original_generalize2 = use the original generalize2 from Monniaux's LPAR 2008 paper\n";
  cout<<"\n-print_conjunctions = print the QELIM problem at conjunction level to .smt files\n";
  cout<<"\n-smt_conjunct_strategy = give the strategy for QELIM from conjunction of LMDEs level\n";
  cout<<"\n-sat_using_qbf = return sat/unsat using bit-blasting followed by QBF solving\n";
  cout<<"\n-blast_and_output = Bit-blast the given benchmark and output in DIMACS format (Note that only the formula part is blasted. :exists part is ignored)\n";
  cout<<"\n-blast_and_output_as_verilog_circuit = Bit-blast the given benchmark and output as a Verilog circuit\n";
  cout<<"\n-dynamic_variable_ordering_in_blast_and_output = Sets dynamic variable ordering in -blast_and_output mode\n";
  cout<<"\n-independent_variables=<filename> Specify the independent variables in the benchmark in a file. Applicable only in -blast_and_output mode\n";
  cout<<"\n-write_back_and_exit= To write back the input file in SMTLIB format and then exit (only if -blast_and_output is true)\n"; 
  cout<<"\n-sat_using_stp = return sat/unsat using STP\n";
  cout<<"\n-LMI_handling_enabled = Enable handling of LMIs\n";
  cout<<"\n-LIA_Input = Input .smt file is in Linear Arithmetic over Integers. This is internally converted to a bit-vector formula with specified width\n";
  cout<<"\n-WidthInteger = Valid only when -LIA_input is true. Specifies the width of integer\n";
  cout<<"\n-Octagons = LIA Input from LinDD benchmarks. APs here are octagons\n";
  cout<<"\n-LMI_QE_using_blasting = QE from LMIs using LME based reasoning followed by blasting\n";
  cout<<"\n-LMI_QE_using_OT = QE from LMIs using LME based reasoning followed by Omega Test\n";
  cout<<"\n-z3_timeout_ms = Timeout value for z3 calls in milliseconds, when it is used for semanticScopeReduction. Default value is 0, which means no timeout. Applicable only when -LMI_handling_enabled is true\n";
  cout<<"\n-SwitchOffLayer1 =  Valid only when -LMI_handling_enabled is true. Switch-off layer1\n";
  cout<<"\n-SwitchOffLayer2 =  Valid only when -LMI_handling_enabled is true. Switch-off layer2\n";
  cout<<"\n-SwitchOffLayer3 =  Valid only when -LMI_handling_enabled is true. Switch-off layer3\n";
  cout<<"\n-solver = stp/yices (give the SMT solver to be used for sat. inside QElimination. default is stp)\n";
  cout<<"\n-order_lmes_before_lmis =  Valid only when -LMI_handling_enabled and -dd are true. To order the LMEs in the LMDD upper compared to the  LMIs(i.e. lower order for LMEs and higher order for LMIs)\n";
  cout<<"\n-order_lmes_before_lmis_in_normalization =  Valid only when -LMI_handling_enabled and -dd are true. To order the LMEs in the LMDD upper compared to the  LMIs(i.e. lower order for LMEs and higher order for LMIs) in the normalized dags\n";
  cout<<"\n-order_lmcs_for_small_lmdds_in_normalization = Valid only when -LMI_handling_enabled and -dd are true. To order the LMCs such that the LMDDs from the normalized dags are small\n";
  cout<<"\n-use_monniaux_type_simplification_in_dd = Use Monniaux type simplifications in QE using LMDDs\n";
  cout<<"\n-use_path_based_strategy_in_lmdd_monniaux = Use path based QE strategy with LMDD and Monniaux type simplifications\n";
  cout<<"\n-perform_projects_at_lmdd_level_in_path_based_strategy = Perform Projects at LMDD level in Path Based Strategy. Applicable only if -use_path_based_strategy_in_lmdd_monniaux and -dd are true (false by default)\n";
  cout<<"\n-use_monniaux_enabled_all_variable_strategy = Use Monniaux enabled all variable strategy for QE. Applicable only if -dd is true (false by default)\n";
  cout<<"\n-convert_problem_into_disjunction_of_tailed_triangles = Valid only when -dd is true. The original QE problem on LMDDs, exists X.(f) is converted into exists X. (f_1 and C_1) or .... or exists X. (f_n and C_n) and then each of them are solved using appropriate QE strategy (false by default)\n";
  cout<<"\n-perform_monniaux_at_startup_in_disjunction_of_tailed_triangles = Valid only when -dd is true and -convert_problem_into_disjunction_of_tailed_triangles is true. When true, performs a number of Monniaux type Project calls before disjunction based algorithm is started\n";
  cout<<"\n-collect_consecutive_tailed_triangles_in_disjunction_of_tailed_triangles = Valid only when -dd is true and -convert_problem_into_disjunction_of_tailed_triangles is true. When true, collects consecutive tailed triangles and calls QE_LMDD on them in a single shot\n";
  cout<<"\n-timeout_in_perform_monniaux_at_startup_in_disjunction_of_tailed_triangles = Valid only when -perform_monniaux_at_startup_in_disjunction_of_tailed_triangles is true. Sets the timeout for Monniaux type Project calls before disjunction based algorithm is started (in seconds) (450 seconds by default)\n";
  cout<<"\n-convert_problem_into_disjunction_of_tailed_triangles_with_agressive_lme_based_simplification = Valid only when -dd is true. When enabled, the original QE problem on LMDDs, exists X.(f) is converted into exists X. (f_1 and C_1) or .... or exists X. (f_n and C_n) and then each of them are solved using appropriate QE strategy similar to the case with -convert_problem_into_disjunction_of_tailed_triangles. Additionally 1) LME based simplification is applied as and when the LME is encountered and  2) No explicit list of tailed triangles is created. (false by default)\n";  
  cout<<"\n-lme_based_simplification_as_preprocessor = Valid only when -dd is true. When enabled, applies LME based simplification as a pre-processing step\n";
  cout<<"\n-perform_SimplifyLMDD_inside_preprocessing_keeping_structural_isomorphism = Valid only when -dd as well as -lme_based_simplification_as_preprocessor are true. When enabled, ensures that simplifyLMDD creates structurally isomorphic LMDD\n";
  cout<<"\n-lme_based_simplification_inside_QE_LMDD = Valid only when -dd is true. When enabled, applies LME based simplification eagerly inside QE_LMDD\n";
  cout<<"\n-eager_conversion_to_disjunctions = Valid only when -dd is true and -convert_problem_into_disjunction_of_tailed_triangles is true. When true, performs conversion to disjunctions until the number of SMT calls >= threshold t_1 or max triangle size <= threshold t_2\n";
  cout<<"\n-demand_based_conversion_to_disjunctions = Valid only when -dd is true and -convert_problem_into_disjunction_of_tailed_triangles is true. When true, performs conversion of a disjunct to further disjunctions if the number of SMT calls < threshold t_1 and disjunct size > threshold t_2 and QE from disjunct is required\n";
  cout<<"\n-maximum_additional_smt_calls_in_demand_based_conversion_to_disjunctions = Specifies the maximum number of additional SMT calls when when -demand_based_conversion_to_disjunctions is true (default = 0)\n";
  cout<<"\n-maximum_triangle_size_in_demand_based_conversion_to_disjunctions = Specifies the maximum size of triangle when when -demand_based_conversion_to_disjunctions is true (default = 100)\n";
  cout<<"\n-convert_to_disjunction_of_tailed_triangles_using_smt_calls = Valid only when -dd is true and -convert_problem_into_disjunction_of_tailed_triangles is true. When true, performs conversion to disjunctions using SMT solver calls\n";
  cout<<"\n-apply_OT_after_layer2 = Apply Omega Test after Layer2 when -LMI_QE_using_OT is true\n";
  cout<<"\n-apply_simple_SMTchecks_before_layer3 = Apply simple SMT based checks before Layer3 is applied\n";
  cout<<"\n-layer2_check_using_bdds = Perform the check in Layer2 using BDDs\n";
  cout<<"\n-exactnesscheck_on = Check exactness of the QE process\n";
  cout<<"\n-testlayer1 = Invoke only layer1; to be used only with -dat\n";
  cout<<"\n-testlayer2 = Invoke only layer2; to be used only with -dat\n";
  cout<<"\n-machine = machine's name \n";
  cout<<"\nCurrently the tool supports -compare_apqelim_omegatest and -compare_apqelim_blasting with only Dag Eliminator Type : -mn or Dag Eliminator Type : -dd and Elimination_Type : -white\n";
  cout<<"\nCurrently the tool does not support the combination qEliminator_type : -ilp, Elimination_Type : -white\n\n";
}

// Function to show the usage of the tool: This shows limited set of options 
void usage_limited()
{
  cout<<"\nUsage: Quantifier_Eliminator [option] [infile]\n\n";
  cout<<"\ninfile                        : file containing Boolean combinations of linear modular constraints in SMTLIB format";
  cout<<"\n-dag                          : dag based quantifier elimination (default)";
  cout<<"\n-dd                           : dd based quantifier elimination";
  cout<<"\n-conjunct                     : quantifier elimination from conjunction of linear modular constraints\n\n";
}

// Function to show the usage of the tool: This includes the options which were shown in the 
// executable for CAV 2011
void usage_CAV()
{
  cout<<"\nUsage\n*********\n";
  cout<<"\nall options SMTLIBfilename\n";
  cout<<"\nCurrently options are\n";
  cout<<"\n-mn : invoke dag approach of elimination (default)\n";
  cout<<"\n-dd : invoke dd approach of elimination (QuaLMoDE by default)\n";
  cout<<"\n-black : invoke BlackBox_QElim Type elimination on LMDD\n";
  cout<<"\n-black_blasting : invoke QBV_Solve on LMDD\n";
  cout<<"\n-use_original_generalize2 : invoke Monniaux_Org\n";
  cout<<"\n-smt_conjunct -smt_conjunct_strategy = strategy where strategy=apqelim | bvsolve | omega : to invoke on QELIM instances at conjunction level\n";
  cout<<"\nExample: to invoke QuaLMoDE on case1.smt, execute all -dd case1.smt\nto invoke Monniaux on case1.smt, execute all case1.smt\nto invoke BlackBox_QElim Type elimination on LMDD on case1.smt, execute all -dd -black case1.smt\nto invoke QBV_Solve on case1.smt, execute all -black_blasting case1.smt\nto invoke Monniaux_Org on case1.smt, execute all -use_original_generalize2 case1.smt\nto invoke AP_QElim on conjunction-level instance case1.smt, execute -smt_conjunct -smt_conjunct_strategy=apqelim case1.smt\nto invoke BV_Solve on conjunction-level instance case1.smt, execute -smt_conjunct -smt_conjunct_strategy=bvsolve case1.smt\nto invoke Omega Test on conjunction-level instance case1.smt, execute -smt_conjunct -smt_conjunct_strategy=omega case1.smt";

  cout<<"\nAfter execution please look at MonniauxLMDDComparisonData.txt, APQElimData.txt, LayerComparisonData.txt,  and ConjunctionBenchmarkDetails.txt to get the details of time taken by QuaLMoDE/Monniaux, details of AP_QElim calls made, details of calls to layers made, time taken by AP_QElim/BV_Solve/Omega Test respectively\n"; 
  cout<<"\nPlease look at ../readme.txt for more details\n";
}

// Function to show the present set of options set by the user
void showOptions()
{
  cout<<"\nInput_type = "<<Input_type<<endl;
  cout<<"\nqEliminator_type = "<<qEliminator_type<<endl;
  cout<<"\nElimination_Type = "<<Elimination_Type<<endl;
  cout<<"\nOutput_type = "<<Output_type<<endl;
  cout<<"\nsimplification_mode = "<<simplification_mode<<endl;
  cout<<"\nInput file = "<<InFileName<<endl;
  cout<<"\nSquaring Times = "<<squaring_times<<endl;
  cout<<"\nDag_Eliminator_Type = "<<Dag_Eliminator_Type<<endl;
  cout<<"\ncompare_apqelim_omegatest = "<<compare_apqelim_omegatest<<endl;
  cout<<"\ncompare_apqelim_blasting = "<<compare_apqelim_blasting<<endl;
}	



// Function to read the command line arguments
void readArguments(int argc, char** argv)
{

  if(argc<2) 
    {
    if(cav)
	{
      	usage_CAV();
	}
    else if(limited_options)
	{
	usage_limited();
	}
    else
	{
      	usage(); 
	}
    exit(1);
    }
  int i=1;
  while(i<=argc-2)
    {
      char* argument=argv[i];
      if(strcmp(argument,"-smt")==0 || strcmp(argument,"-dat")==0 || strcmp(argument,"-mat")==0 || strcmp(argument,"-smt_conjunct_sat")==0  || strcmp(argument,"-smt_conjunct")==0)
	{
	  Input_type = argument;
	}
      else if(strcmp(argument,"-lma")==0 || strcmp(argument,"-ilp")==0 || strcmp(argument,"-bit")==0)
	{
	  qEliminator_type = argument;
	}
      else if(strcmp(argument,"-white")==0 || strcmp(argument,"-black")==0 || strcmp(argument,"-gray")==0 || strcmp(argument,"-white_blasting")==0 || strcmp(argument,"-black_blasting")==0 || strcmp(argument,"-allsmt")==0)
	{
	  Elimination_Type = argument;
	}
      else if(strcmp(argument,"-qelim")==0 || strcmp(argument,"-umc")==0)
	{
	  Output_type = argument;
	}
      else if(strcmp(argument,"-dd")==0 || strcmp(argument,"-mn")==0  || strcmp(argument,"-pr")==0  || strcmp(argument,"-l1b")==0  || strcmp(argument,"-l1ot")==0  || strcmp(argument,"-l2ot")==0  || strcmp(argument,"-l2ub")==0 || strcmp(argument,"-preprocess")==0)
	{
	  Dag_Eliminator_Type = argument;
	}
      else if(strcmp(argument,"-impchecks2")==0)
	{
	  simplification_mode=2;
	}
      else if(strcmp(argument,"-impchecks1")==0)
	{
	  simplification_mode=1;
	}
      else if(strcmp(argument,"-impchecks0")==0)
	{
	  simplification_mode=0;
	}
      else if(strcmp(argument,"-unroll")==0)
	{
	  unrolling_mode=true;
	}
      else if(strcmp(argument,"-linearize")==0)
	{
	  printing_with_linearizing_mode=true;
	}
      else if(strcmp(argument, "-LMI_handling_enabled")==0)
      {
          LMI_handling_enabled=true;
      }
      else if(strcmp(argument, "-Octagons")==0)
      {
          Octagons = true;
      }
      else if(strcmp(argument, "-LMI_QE_using_blasting")==0)
      {
          LMI_QE_using_blasting = true;
      }
      else if(strcmp(argument, "-LMI_QE_using_OT")==0)
      {
          LMI_QE_using_OT = true;
      }
      else if(strcmp(argument, "-apply_OT_after_layer2")==0)
      {
          OTAfterLayer2 = true;
      }      
      else if(strcmp(argument, "-LIA_Input")==0)
      {
          LIA_Input=true;
      }
      else if(strcmp(argument, "-SwitchOffLayer1")==0)
      {
          SwitchOffLayer1=true;
      }
      else if(strcmp(argument, "-SwitchOffLayer2")==0)
      {
          SwitchOffLayer2=true;
      }
      else if(strcmp(argument, "-SwitchOffLayer3")==0)
      {
          SwitchOffLayer3=true;
      }
      else if(strcmp(argument, "-order_lmes_before_lmis")==0)
      {
          order_lmes_before_lmis=true;
      }
      else if(strcmp(argument, "-order_lmes_before_lmis_in_normalization")==0)
      {
          order_lmes_before_lmis_in_normalization=true;
      }   
      else if(strcmp(argument, "-order_lmcs_for_small_lmdds_in_normalization")==0)
      {
          order_lmcs_for_small_lmdds_in_normalization=true;
      }     
      else if(strcmp(argument, "-exactnesscheck_on")==0)
      {
        exactnesscheck_on=true;
      }
      else if(strcmp(argument, "-evaluate_and_get_core")==0)
	{
	  core_finding=true;
	}
      else if(strcmp(argument, "-get_core")==0)
	{
	  core_finding_without_evaluation=true;
	}
      else if(strcmp(argument, "-order_based_on_vars_to_elim")==0)
	{
	  order_based_on_vars_to_elim = true;
	}
      else if(strcmp(argument, "-compare_apqelim_omegatest")==0)
	{
	  compare_apqelim_omegatest = true;
	}
      else if(strcmp(argument, "-compare_apqelim_blasting")==0)
	{
	  compare_apqelim_blasting = true;
	}
      else if(strcmp(argument, "-use_original_generalize2")==0)
	{
	  generalize2_strategy = 3;
	}
      else if(strcmp(argument, "-testlayer1")==0)
	{
	  layer_to_test_with_dat_file = 1;
	}
      else if(strcmp(argument, "-testlayer2")==0)
	{
	  layer_to_test_with_dat_file = 2;
	}
      else if(strcmp(argument, "-print_conjunctions")==0)
	{
	  output_conjunctions = true;
	}
      else if(strcmp(argument,"-sat_using_qbf")==0)
	{
	  sat_using_qbf=true;
	}
      else if(strcmp(argument,"-blast_and_output")==0)
	{
	  blast_and_output=true;
	}
      else if(strcmp(argument,"-blast_and_output_as_verilog_circuit")==0)
	{
	  blast_and_output_as_verilog_circuit=true;
	}	
      else if(strcmp(argument,"-write_back_and_exit")==0)
	{
	  write_back_and_exit=true;
	}  
      else if(strcmp(argument,"-dynamic_variable_ordering_in_blast_and_output")==0)
      {
          dynamic_variable_ordering_in_blast_and_output = true;
      }
      else if(strcmp(argument,"-sat_using_stp")==0)
	{
	  sat_using_stp=true;
	}
      else if(strcmp(argument, "-use_monniaux_type_simplification_in_dd")==0)
	{
	  use_monniaux_type_simplification_in_dd=true;
	}
      else if(strcmp(argument, "-use_path_based_strategy_in_lmdd_monniaux")==0)
	{
	  use_path_based_strategy_in_lmdd_monniaux=true;
	} 
      else if(strcmp(argument, "-perform_projects_at_lmdd_level_in_path_based_strategy")==0)
        {
        PerformProjectsInPathBasedStrategyAtLMDDLevel = true;
        }
      else if(strcmp(argument, "-use_monniaux_enabled_all_variable_strategy")==0)
        {
        UseMonniauxEnabledAllVariableStrategy = true;
        }      
      else if(strcmp(argument, "-convert_problem_into_disjunction_of_tailed_triangles")==0)
        {
        ConvertProblemIntoDisjunctionOfTailedTriangles = true;
        }
      else if(strcmp(argument, "-perform_monniaux_at_startup_in_disjunction_of_tailed_triangles")==0)
        {
        PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = true;  
        }
      else if(strcmp(argument, "-collect_consecutive_tailed_triangles_in_disjunction_of_tailed_triangles")==0)
        {
        CollectConsecutiveSatTailedTriangles_In_ConversionToDisjunctionOfTailedTriangles = true;  
        }
      else if(strcmp(argument, "-convert_problem_into_disjunction_of_tailed_triangles_with_agressive_lme_based_simplification")==0)
        {
        ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification = true;  
        }
      else if(strcmp(argument, "-lme_based_simplification_as_preprocessor")==0)
        {
        LMEBasedSimplificationAsPreprocessor = true;  
        }   
      else if(strcmp(argument, "-lme_based_simplification_inside_QE_LMDD")==0)
        {
        LMEBasedSimplificationInsideQELMDD = true;  
        }
      else if(strcmp(argument, "-eager_conversion_to_disjunctions")==0)
        {
        EagerConversionToDisjunctions = true;  
        } 
      else if(strcmp(argument, "-demand_based_conversion_to_disjunctions")==0)
        {
        DemandBasedConversionToDisjunctions = true;  
        }  
      else if(strcmp(argument, "-convert_to_disjunction_of_tailed_triangles_using_smt_calls")==0)
        {
        ConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = true;  
        }
       else if(strcmp(argument, "-perform_SimplifyLMDD_inside_preprocessing_keeping_structural_isomorphism")==0)
        {
         PerformSimplifyLMDDInsidePreprocessingKeepingStructuralIsomorphism = true;  
        } 
      else if(strcmp(argument, "-apply_simple_SMTchecks_before_layer3")==0)
        {
         SimpleSMTChecksBeforeLayer3 = true;  
        } 
      else if(strcmp(argument, "-layer2_check_using_bdds")==0)
        {
        Layer2CheckUsingBDDs = true;  
        }
      else if(strcmp(argument,"-conjunct")==0)// -conjunct is another name for -smt_conjunct
        {
         Input_type=strdup("-smt_conjunct");
        }
      else if(strcmp(argument,"-dag")==0)// -dag is another name for -mn
        {
         Dag_Eliminator_Type=strdup("-mn");
        }

      else 
	{
	  string str_arg = argument;
	  int index = str_arg.find_last_of("=");
	  if(index!=string::npos)
	    {
	      if(str_arg.substr(0, index)=="-times")
		{
		  string times_str = str_arg.substr(index+1);	
		  squaring_times = atoi(times_str.c_str());
		}
              else if(str_arg.substr(0, index)=="-WidthInteger")
		{
		  string WidthInteger_str = str_arg.substr(index+1);	
		  WidthInteger = atoi(WidthInteger_str.c_str());
                  ModWidthInteger = findPower(WidthInteger);
                  ModWidthIntegerHalf = ModWidthInteger/2;
		}   
              else if(str_arg.substr(0, index)=="-timeout")
              {
                  string timeout_str = str_arg.substr(index+1);
                  time_out = strtoull(timeout_str.c_str(), NULL, 0);
                  time_out_start=0;
                  time_out_enabled=true;
                  timed_out=false;

	          
		 long long int time_out_long = time_out;
		//printf("\ntime_out is: %lld\n", time_out_long);

		struct rlimit r2;
		r2.rlim_cur = time_out_long;
		setrlimit(RLIMIT_CPU, &r2);

		getrlimit(RLIMIT_CPU, &r2);
		//printf("\nMaximum time is: %lld\n", (long long int)r2.rlim_cur);
              }
	      else if(str_arg.substr(0, index)=="-memoryout")
              {
                  string memoryout_str = str_arg.substr(index+1);
                  int memory_out = atoi(memoryout_str.c_str());
		  long long int memory_out_in_bytes = memory_out * 1024;
		  memory_out_in_bytes = memory_out_in_bytes * 1024;
	          
		 //printf("\nmemory_out_in_bytes is: %lld\n", memory_out_in_bytes);

		struct rlimit rl;
		rl.rlim_cur = memory_out_in_bytes;
		setrlimit(RLIMIT_AS, &rl);

		getrlimit(RLIMIT_AS, &rl);
		//printf("\nMaximum memory is: %lld\n", (long long int)rl.rlim_cur);
              }
              else if(str_arg.substr(0, index)=="-z3_timeout_ms")
              {
                  string z3_timeout_ms_str = str_arg.substr(index+1);
                  z3_timeout_ms = strtoull(z3_timeout_ms_str.c_str(), NULL, 0);
              }
	      else if(str_arg.substr(0, index)=="-machine")
		{
		  machine_name = str_arg.substr(index+1);	
		}
	      else if(str_arg.substr(0, index)=="-frame")
		{
		  string frame_str = str_arg.substr(index+1);	
		  frame_number_arg = atoi(frame_str.c_str());
		}
              else if(str_arg.substr(0, index)=="-maximum_triangle_size_in_demand_based_conversion_to_disjunctions")
                {
                  string size_str = str_arg.substr(index+1);	
		  MaximumSizeOfTriangleInDemandBasedConversionToDisjunctions = atoi(size_str.c_str());
                }
              else if(str_arg.substr(0, index)=="-maximum_additional_smt_calls_in_demand_based_conversion_to_disjunctions")
                {
                  string call_str = str_arg.substr(index+1);	
		  MaximumNoOfSMTCallsInDemandBasedConversionToDisjunctions = atoi(call_str.c_str());
                }
              else if(str_arg.substr(0, index)=="-timeout_in_perform_monniaux_at_startup_in_disjunction_of_tailed_triangles")
                {
                  string call_str = str_arg.substr(index+1);	
		  TimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = atoi(call_str.c_str());
                }
	      else if(str_arg.substr(0, index)=="-bmc_file")
		{
		  output_file_for_bmc = str_arg.substr(index+1);
		}  
	      else if(str_arg.substr(0, index)=="-bmc_cache_file")
		{
		  cache_file_for_bmc = str_arg.substr(index+1);
		} 
	      else if(str_arg.substr(0, index)=="-smt_conjunct_strategy")
		{
		  smt_conjunct_strategy = str_arg.substr(index+1);
		}   
	      else if(str_arg.substr(0, index)=="-model")
		{
		  model_file = str_arg.substr(index+1);
		}  
              else if(str_arg.substr(0, index)=="-independent_variables")
                {
                  independent_variables_file_name =  str_arg.substr(index+1);
                }
	      else if(str_arg.substr(0, index)=="-evaluation_result")
		{
		  evaluation_result_in_core_file = str_arg.substr(index+1);
		}  
	      else if(str_arg.substr(0, index)=="-core_result")
		{
		  core_result_in_core_file = str_arg.substr(index+1);
		} 
	      else if(str_arg.substr(0, index)=="-solver")
		{
		  internal_solver = str_arg.substr(index+1);
		}
	      else if(str_arg.substr(0, index)=="-linearize")
		{
		  if(str_arg.substr(index+1) == "on")
		    {
		      linearization = true;
		    }
		  else if(str_arg.substr(index+1) == "off")
		    {
		      linearization = false;
		    }
		  else
		    {
		      cout<<"\nUnknown value for -linearize in function readArguments in main.cc\n";
		      exit(1);
		    }
		} 

	      else
		{
		  if(cav)
		    {
		    usage_CAV();
		    }
		  else if(limited_options)
		    {
		    usage_limited();
		    }
		  else
                    {
		    usage(); 
		    }
			
		  exit(1);
		}
	    }
      	
	  else
	    {
		if(cav)
		    {
		    usage_CAV();
		    }
		else if(limited_options)
		    {
		    usage_limited();
		    }
		else
                    {
		    usage(); 
		    }
	      exit(1);
	    }
	}
      i++;
    }

  InFileName=strdup(argv[i]);
  if(strcmp(qEliminator_type,"-ilp")==0 && strcmp(Elimination_Type,"-white")==0) 
	{
	usage(); 
	exit(1);
	}
  if(compare_apqelim_omegatest && strcmp(Elimination_Type,"-white")!=0 && strcmp(Dag_Eliminator_Type, "-dd")==0)
    	{
	showOptions(); 
	usage(); 
	exit(1);
	}
  if(compare_apqelim_blasting && strcmp(Elimination_Type,"-white")!=0 && strcmp(Dag_Eliminator_Type, "-dd")==0)
    	{
	showOptions(); 
	usage(); 
	exit(1);
	}
}

// We have vector<DAGNode*> root which contains the formulas obtained from parsing
// Result from this function is the conjunction of these formulas
DAGNode* createConjunctionOfRoot()
{
  if(root.size()==1) return root[0];
  else
    {
      root[0] = createDAG("and",root, LOGICAL_SYMBOL, 1);
      return root[0];
    }
}

// Function to show the WidthTable
void showMap(map<string, int> &WidthTable)
{
  cout<<"\nWidth Table\n";

  for(map<string, int>::iterator wit=WidthTable.begin(); wit!=WidthTable.end(); wit++)
    {
      cout<<"\n"<<wit->first<<"\t"<<wit->second<<"\n";
    }
}

// This returns the set of variables other than the variables to be eliminated
void getRemainingVariables(map<string, int> &WidthTable, set<string> &VarSetToElim, set<string> &VarSetRemaining)
{
  for(map<string, int>::iterator wit=WidthTable.begin(); wit!=WidthTable.end(); wit++)
    {
      if(VarSetToElim.find(wit->first) == VarSetToElim.end())
	{
	VarSetRemaining.insert(wit->first);
	}
    }
}


// This function accepts QE input from a .dat file
// .dat format is supported by the new implementation of QE.
// Furthermore, layer1/layer2 is invoked on the QE input
void callLayerOnDatFormat(ifstream* InputFile, ofstream* LayerStatsFile)
{
        unsigned number_constraints;
        unsigned total_number_variables;
        unsigned total_number_eliminate;
        vector<string> total_variables;
        set<string> variables_to_eliminate;  
        
        map<string, int> WidthTable;     
        set<DAGNode*> Constraints;

        *InputFile >> number_constraints;
	*InputFile >> total_number_variables;

	unsigned sum_variable_width = 0;
	int number_equalities = 0;
	int number_disequalities = 0;
	int number_inequalities = 0;

	struct timeval start_ms, finish_ms;
        unsigned long long int duration_ms;

	for(unsigned total_var_it = 0; total_var_it < total_number_variables; total_var_it++)
		{
		string variable;
		*InputFile >> variable;
		total_variables.push_back(variable);		
		}
	
	for(unsigned total_var_it = 0; total_var_it < total_number_variables; total_var_it++)
		{
		unsigned variable_width;
		*InputFile >> variable_width;
		string variable = total_variables[total_var_it];
                    
                WidthTable.insert(make_pair(variable, variable_width));
		}

	*InputFile >> total_number_eliminate;	

	for(unsigned var_to_elim_it = 0; var_to_elim_it < total_number_eliminate; var_to_elim_it++)
		{
		string variable;
		*InputFile >> variable;
		variables_to_eliminate.insert(variable);

		unsigned variable_width = getWidthOfVariable(WidthTable, variable);
		sum_variable_width = sum_variable_width + variable_width;
		}	
        
        for(unsigned constr_it = 0; constr_it < number_constraints; constr_it++)
		{
		string constraint_type;
		unsigned constraint_width;
		unsigned number_variables;
		list<string> variables;
		list<ULINT> coefficients_left;
		ULINT constant_left;
		list<ULINT> coefficients_right;
		ULINT constant_right;

		*InputFile >> constraint_type;
		*InputFile >> constraint_width;
		*InputFile >> number_variables;

				
		for(unsigned var_it = 0; var_it < number_variables; var_it++)
			{
			string variable;
			*InputFile >> variable;
			variables.push_back(variable);
			}

		if(constraint_type == "=" || constraint_type == "!=")
			{
			string constant_left_str;
			*InputFile >> constant_left_str;
			ULINT constant_left = binaryStringToULINT(constant_left_str);
			
			for(unsigned var_it = 0; var_it < number_variables; var_it++)
				{
				string coefficient_str;
				*InputFile >> coefficient_str;
				ULINT coefficient = binaryStringToULINT(coefficient_str);

				coefficients_left.push_back(coefficient);
				}

			// let's create the dag for c1.x1 + ... + cn.xn + c0
                        DAGNode* LHS = getDAGFromVarsCoefsAndConstant(variables, coefficients_left, constant_left, constraint_width);
			assert(LHS != NULL);
                        
                        string ZeroString = "0";
                        DAGNode* RHS = createBVConstant(ZeroString, constraint_width);
                        assert(RHS != NULL);

			if(constraint_type == "=")
				{	
				number_equalities++;
				DAGNode* eqzNode = createDAG("=", LHS, RHS, LOGICAL_SYMBOL, 1);
				assert(eqzNode != NULL);
                                //cout<<endl<<"eqzNode = "<<getDAGStr(eqzNode)<<endl;
				Constraints.insert(eqzNode);
				}
			else
				{
				number_disequalities++;
				DAGNode* disEqzNode = createDAG("is_not_equal", LHS, RHS, LOGICAL_SYMBOL, 1);
				assert(disEqzNode != NULL);
                                //cout<<endl<<"disEqzNode = "<<getDAGStr(disEqzNode)<<endl;
				Constraints.insert(disEqzNode);
				}			
			}
                else if(constraint_type == "<=")
			{
			string constant_left_str;
			*InputFile >> constant_left_str;
			ULINT constant_left = binaryStringToULINT(constant_left_str);
			
			for(unsigned var_it = 0; var_it < number_variables; var_it++)
				{
				string coefficient_str;
				*InputFile >> coefficient_str;
				ULINT coefficient = binaryStringToULINT(coefficient_str);

				coefficients_left.push_back(coefficient);	
				}

			string constant_right_str;
			*InputFile >> constant_right_str;
			ULINT constant_right = binaryStringToULINT(constant_right_str);
			
			for(unsigned var_it = 0; var_it < number_variables; var_it++)
				{
				string coefficient_str;
				*InputFile >> coefficient_str;
				ULINT coefficient = binaryStringToULINT(coefficient_str);

				coefficients_right.push_back(coefficient);					
				}

			// let's create the dag for c1.x1 + ... + cn.xn + c0
                        DAGNode* LHS = getDAGFromVarsCoefsAndConstant(variables, coefficients_left, constant_left, constraint_width);
			assert(LHS != NULL);
                        
                        // let's create the dag for d1.x1 + ... + dn.xn + d0
                        DAGNode* RHS = getDAGFromVarsCoefsAndConstant(variables, coefficients_right, constant_right, constraint_width);
			assert(RHS != NULL);
                        
                        DAGNode* leqNode = createDAG("bvule", LHS, RHS, LOGICAL_SYMBOL, 1);
                        assert(leqNode != NULL);
                        //cout<<endl<<"leqNode = "<<getDAGStr(leqNode)<<endl;
			Constraints.insert(leqNode);
                                
			number_inequalities++;			
			}
		else
			{
			cout<<"\nUnknown constraint type from .dat input file\n";
			exit(-1);
			}
        }// for each constraint
        
        *LayerStatsFile << endl << InFileName << "\t" << total_number_variables << "\t" << total_number_eliminate << "\t" << sum_variable_width;
	*LayerStatsFile << "\t" << number_equalities << "\t" << number_disequalities << "\t" << number_inequalities;
        
        
        if(layer_to_test_with_dat_file == 1)
        {
            cout<<"\n\nLayer1 started\n\n";
            gettimeofday (&start_ms, NULL);

            DAGNode* ResultAfterLayer1 = applyLMILayer1(Constraints, variables_to_eliminate, WidthTable);

            gettimeofday (&finish_ms, NULL);

            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

            if(ResultAfterLayer1 == NULL)
            {
                        cout<<"\nResultAfterLayer1 == NULL! Error insideapplyLMILayer1 \n";
                        exit(-1);
            }
            else
            {   
                        //cout<<endl<<"ResultAfterLayer1 = "<<getDAGStr(ResultAfterLayer1)<<endl;
                        cout<<"\nLayer1 completed in "<<duration_ms<<" milli seconds\n";
            }
            cout<<endl;

            *LayerStatsFile << "\t" << duration_ms << "\t" << machine_name << endl;
        }
        else  if(layer_to_test_with_dat_file == 2)
        {
            cout<<"\n\nLayer2 started\n\n";
            gettimeofday (&start_ms, NULL);

            DAGNode* ResultAfterLayer2 = applyLMILayer2Recursive(Constraints, variables_to_eliminate, WidthTable);

            gettimeofday (&finish_ms, NULL);

            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

            if(ResultAfterLayer2 == NULL)
            {
                        cout<<"\nResultAfterLayer2 == NULL! Error inside applyLMILayer2Recursive\n";
                        exit(-1);
            }
            else
            {   
                        //cout<<endl<<"ResultAfterLayer2 = "<<getDAGStr(ResultAfterLayer2)<<endl;
                        cout<<"\nLayer2 completed in "<<duration_ms<<" milli seconds\n";
            }
            cout<<endl;

            *LayerStatsFile << "\t" << duration_ms << "\t" << machine_name << endl;
        }
}


// This function accepts a set of LMEs and set of LMDs from the key board, and
// then invokes QE on it using the function qEliminatorReturningDAG.
// Note that qEliminatorReturningDAG performs QE on matrix. Hence, 
// this function does QE without scope reduction
void qEliminationOnMatrix()
{
  int NoEqns, NoDisEqns, NoVars, NoVarsToElim, NoOfBits;
  ULINT Mod;
  Matrix EqCoefMat, EqConstMat; // EqCoefMat is A and EqConstMat is B in AX=B
  Matrix DisEqCoefMat, DisEqConstMat; // respectively C, D in CX <> D
	
  list<string> VarNames;
  map<string, int> WidthTable;     
    
  cin>>NoVars;
  cin>>NoVarsToElim;
  cin>>NoOfBits;
  cin>>NoEqns;
  cin>>NoDisEqns;
  
  Mod=findPower(NoOfBits);
  getEquations(EqCoefMat, EqConstMat, NoEqns, NoVars, Mod);
  getDisequations(DisEqCoefMat, DisEqConstMat, NoDisEqns, NoVars, Mod);

  cout<<"\nInitial LMDEs\n";
  showEquations(EqCoefMat, EqConstMat);
  showDisEquations(EqCoefMat, EqConstMat);
  // Fill in other details 
  for(int i=0;i<NoVars;i++)
    {
      char temp[100];
      sprintf(temp, "%d", i);
      string temp_str(temp);
      string variable = "x_";
      variable += temp_str;
      VarNames.push_back(variable);
      WidthTable.insert(make_pair(variable, NoOfBits));
    }
 
  if(strcmp(qEliminator_type, "-lma")==0)
    {
      DAGNode* result = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
      if(result==NULL)
	{
	cout<<"\nresult is true\n";
	}
      else
	{
	cout<<"\nresult = "<<getDAGStr(result)<<endl;
	}
    }
   
}

// This function accepts a set of LMEs from the key board, and
// then invokes QE on it using the function qEliminatorReturningDAG.
// Note that qEliminatorReturningDAG performs QE on matrix. Hence, 
// this function does QE without scope reduction.
// This function was used to test the function qEliminatorReturningDAG
// on some sets of LMEs from STP web site
void qEliminationOnMatrix_STP_Form()
{
  time_t el_start, el_end;
  time(&el_start);
  int NoEqns, NoDisEqns, NoVars, NoVarsToElim, NoOfBits;
  ULINT Mod;
  Matrix EqCoefMat, EqConstMat; // EqCoefMat is A and EqConstMat is B in AX=B
  Matrix DisEqCoefMat, DisEqConstMat; // respectively C, D in CX <> D
	
  list<string> VarNames;
  map<string, int> WidthTable;     
    
  cin>>NoVars;
  cin>>NoVarsToElim;
  cin>>NoOfBits;
  cin>>NoEqns;
 
  NoDisEqns=0;
  Mod=findPower(NoOfBits);
  
  getEquations(EqCoefMat, EqConstMat, NoEqns, NoVars, Mod);

  cout<<"\nInitial LMEs\n";
  showEquations(EqCoefMat, EqConstMat);
  // Fill in other details 
  for(int i=0;i<NoVars;i++)
    {
      char temp[100];
      sprintf(temp, "%d", i);
      string temp_str(temp);
      string variable = "x_";
      variable += temp_str;
      VarNames.push_back(variable);
      WidthTable.insert(make_pair(variable, NoOfBits));
    }
 
  if(strcmp(qEliminator_type, "-lma")==0)
    {
      DAGNode* result = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);

      time(&el_end);
      cout<<"\nElimination Time = "<<el_end-el_start<<" seconds \n";

      if(result==NULL)
	{
	cout<<"\nresult is true\n";
	}
      else
	{
	cout<<"\nresult = "<<getDAGStr(result)<<endl;
	}
    }
   
}

// Finds squared image given Conjunct = R(s0,s1) /\ R(s1,s2). SquaredImage = Exists(s1, Conjunct)
DAGNode* getSquaredImage(DAGNode* Conjunct, map<string, int> &WidthTable) 
{
  set<string> Names;
  set<string> VarsToElim;

  getDAGNames(Conjunct, Names);

  // We need to quantify out all signals from Conjunct which end with _1
  for(set<string>::iterator it=Names.begin(); it!=Names.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      if(index==string::npos)
	{
	  cout<<"\nVariable name without time stamp in function getSquaredImage in main.cc\n";
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

  //exit(1);
  DAGNode* SquaredImage = ddm.Quantifier_Eliminator(Conjunct, VarsToElim, WidthTable);
  return SquaredImage;
}
      

// Finds the square of R. Given R(s0, s1), square of R = R(s0,s1) \/ Rename_s2_to_s1(Exists(s1, R(s0,s1)/\R(s1,s2))). But given R(<=i)(s0,s1) square is just Rename_s2_to_s1(Exists(s1, R(<=i)(s0,s1)/\R(<=i)(s1,s2))). Hence disjunction is needed only in the first call
DAGNode* getSquare(DAGNode* TR, map<string, int> &WidthTable, int time)
{
  // TR is R(s0,s1)
  //cout<<"\nTR = "<<getDAGStr(TR)<<endl;
  DAGNode* TR_next = cloneAndShift(TR, 1); // get R(s1,s2)
  //cout<<"\nTR_next = "<<getDAGStr(TR_next)<<endl;
  DAGNode* Conjunct = createDAG("and", TR, TR_next, LOGICAL_SYMBOL, 1); // Conjunct = R(s0,s1) /\ R(s1,s2)
  //cout<<"\nConjunct = "<<getDAGStr(Conjunct)<<endl;
  DAGNode* SquaredImage = getSquaredImage(Conjunct, WidthTable); // SquaredImage = Exists(s1, R(s0,s1) /\ R(s1,s2))
  //cout<<"\nSquaredImage = "<<getDAGStr(SquaredImage)<<endl;
  DAGNode* RenamedImage = cloneWithRenamedDeltas(SquaredImage); // RenamedImage = Rename_s2_to_s1(Exists(s1, R(s0,s1)/\R(s1,s2)))
  //cout<<"\nRenamed Image = "<<getDAGStr(RenamedImage)<<endl; 
  DAGNode* Square;

  if(time==0) // first time
    {
      cout<<"\nFirst time. Disjunction needed\n";
      Square = createDAG("or", TR, RenamedImage, LOGICAL_SYMBOL, 1); // Square = R(s0,s1) \/ Rename_s2_to_s1(Exists(s1, R(s0,s1)/\R(s1,s2)))
    }
  else
    {
      cout<<"\nDisjunction not needed\n";  
      Square = RenamedImage; // Square = Rename_s2_to_s1(Exists(s1, R(s0,s1)/\R(s1,s2)))
    }


  //cout<<"\nSquare = "<<getDAGStr(Square)<<endl;
  //cout<<"\nSquaring finished\n";
  //int userchar = getchar();
  return Square;
}

// Function to add new entries into WidthTable. For example, if there's an entry x, add entries x_1,...,x_3
void getUpdatedWidthTable(map<string, int> &WidthTableOriginal, map<string, int> &WidthTable)
{
  set<string> ids_processed;
  for(map<string, int>::iterator it=WidthTableOriginal.begin(); it!=WidthTableOriginal.end(); it++)
    {
      string id_delta;
      int width;

      id_delta = it->first;
      width = it->second;

      int index = id_delta.find_last_of("_");
      if(index==string::npos)
	{
	  cout<<"\nVariable name without time stamp in function getUpdatedWidthTable in main.cc\n";
	  exit(1);
	}
      string id = id_delta.substr(0, index);

      if(ids_processed.find(id) != ids_processed.end()) // id processed. take next one
	continue;
      
      string id_0, id_1, id_2;

      id_0 = id;
      id_0 += "_";
      id_0 += "0";
      id_1 = id;
      id_1 += "_";
      id_1 += "1";
      id_2 = id;
      id_2 += "_";
      id_2 += "2";

      WidthTable.insert(make_pair(id_0, width));
      WidthTable.insert(make_pair(id_1, width));
      WidthTable.insert(make_pair(id_2, width));
      ids_processed.insert(id);
    }
}

// Function to add new entries into WidthTable. For example, if there's an entry x, add entries x_1,...,x_(max_delta)
void getUpdatedWidthTableBasedOnSquaring(map<string, int> &WidthTableOriginal, map<string, int> &WidthTable, int max_delta)
{
  set<string> ids_processed;
  for(map<string, int>::iterator it=WidthTableOriginal.begin(); it!=WidthTableOriginal.end(); it++)
    {
      string id_delta;
      int width;

      id_delta = it->first;
      width = it->second;

      int index = id_delta.find_last_of("_");
      if(index==string::npos)
	{
	  cout<<"\nVariable name without time stamp in function getUpdatedWidthTable in main.cc\n";
	  exit(1);
	}
      string id = id_delta.substr(0, index);

      if(ids_processed.find(id) != ids_processed.end()) // id processed. take next one
	continue;
      
      for(int i=0;i<=max_delta;i++)
	{
	  string new_id_delta;
	  new_id_delta = id;
	  new_id_delta += "_";
	  char temp[100];
	  sprintf(temp, "%d", i);
	  string delta_str(temp);
	  new_id_delta += delta_str;

	  WidthTable.insert(make_pair(new_id_delta, width));
	}
      ids_processed.insert(id);
    }
}

// Function to do UMC using iterative squaring: first version. Given TR and original widthtable,
// performs iterative squaring for 2 steps.
void unboundedModelChecking(DAGNode *TR, map<string, int> WidthTableOriginal)
{
  time_t umc_start, umc_end;
  time(&umc_start);

  map<string, int> WidthTable;
  // The function getUpdatedWidthTable updates the width table. Currently, it has only entries var_0, var_1 for all vars. We need to add vr_2 also. WidthTable is the recreated WidthTable
  showMap(WidthTableOriginal);
  getUpdatedWidthTable(WidthTableOriginal, WidthTable);
  showMap(WidthTable);

  int squaring_times = 2;
  DAGNode *SquaredClosure = TR;
	
  for(int i=0;i<squaring_times;i++)
    {
      SquaredClosure = getSquare(SquaredClosure, WidthTable, i);
      cout<<endl<<i+1<<" th squaring completed\n";
      //cout<<endl<<"SquaredClosure = "<<getDAGStr(SquaredClosure)<<endl;

      if(isTrue(SquaredClosure)) break;
      //int char_from_user = getchar();
    }

  cout<<"\nUMC completed\n";
  time(&umc_end);
  cout<<"\nUMC Duration = "<<umc_end-umc_start<<" seconds \n";
}


// Finds the square of R. Given R(s0, s1), square of R = Rename_s2_to_s1(Exists(s1, R(s0,s1)/\R(s1,s2)))
DAGNode* getSquareModified(DAGNode* TR, map<string, int> &WidthTable)
{
  // TR is R(s0,s1)
  //cout<<"\nTR = "<<getDAGStr(TR)<<endl;
  DAGNode* TR_next = cloneAndShift(TR, 1); // get R(s1,s2)
  //cout<<"\nTR_next = "<<getDAGStr(TR_next)<<endl;
  DAGNode* Conjunct = createDAG("and", TR, TR_next, LOGICAL_SYMBOL, 1); // Conjunct = R(s0,s1) /\ R(s1,s2)
  //cout<<"\nConjunct = "<<getDAGStr(Conjunct)<<endl;
  DAGNode* SquaredImage = getSquaredImage(Conjunct, WidthTable); // SquaredImage = Exists(s1, R(s0,s1) /\ R(s1,s2))
  //cout<<"\nSquaredImage = "<<getDAGStr(SquaredImage)<<endl;
  DAGNode* RenamedImage = cloneWithRenamedDeltas(SquaredImage); // RenamedImage = Rename_s2_to_s1(Exists(s1, R(s0,s1)/\R(s1,s2)))
  return RenamedImage;
}

// Return unit DAG. i.e. (a_1=a_0)/\.../\(x_1=x_0)
DAGNode* getUnitDAG(DAGNode *TR, map<string, int> &WidthTableOriginal)
{
  set<string> VarNamesWithDeltas;
  getDAGNames(TR, VarNamesWithDeltas);
  
  set<string> VarNames;
  for(set<string>::iterator it=VarNamesWithDeltas.begin(); it!=VarNamesWithDeltas.end(); it++)
    {
      string id_delta;
      id_delta = *it;
      int index = id_delta.find_last_of("_");
      if(index==string::npos)
	{
	  cout<<"\nVariable name without time stamp in function getUnitDAG in main.cc\n";
	  exit(1);
	}
      string id = id_delta.substr(0, index);
      VarNames.insert(id);
    }
  // VarNames contains the set of state variables
  // create Unit = /\ (x_0=x_1) for all x belong to. VarNames
  
  DAGNode* Unit=NULL;
  
  for(set<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++)
    {
      string id=*it;
      int width;

      string id_0, id_1;
      
      id_0 = id;
      id_0 += "_";
      id_0 += "0";
      
      id_1 = id;
      id_1 += "_";
      id_1 += "1";


      if(WidthTableOriginal.find(id_0)!=WidthTableOriginal.end())
	{
	  width = WidthTableOriginal[id_0];
	}
      else if(WidthTableOriginal.find(id_1)!=WidthTableOriginal.end())
	{
	  width = WidthTableOriginal[id_1];
	}
      else
	{
	  cout<<"\nError in function getUnitDAG in main.cc. No entry for "<<id_0<<" or "<<id_1<<" in WidthTable \n";
	  exit(1);
	}      

      DAGNode *bv0 = createBVVariable(id_0, width);
      DAGNode *bv1 = createBVVariable(id_1, width); 
      DAGNode *eqn = createDAG("=", bv0, bv1, LOGICAL_SYMBOL, 1);

      if(Unit==NULL) Unit=eqn;
      else
	Unit = createDAG("and", Unit, eqn, LOGICAL_SYMBOL, 1);
    }
  
  if(Unit==NULL) 
    {
      cout<<"\nUnit == NULL in function getUnitDAG in main.cc\n";
      exit(1);
    }
  return Unit;
}

// Return TR \/ Unit
DAGNode* findInitialSquaredClosure(DAGNode *TR, map<string, int> &WidthTableOriginal)
{
  DAGNode* Unit;
  Unit = getUnitDAG(TR, WidthTableOriginal);
  DAGNode* InitialSquare = createDAG("or", Unit, TR, LOGICAL_SYMBOL, 1);
  return InitialSquare;
}

// Function to do UMC using iterative squaring: second version. Given TR and original widthtable,
// performs iterative squaring for 3 steps.
void unboundedModelCheckingModified(DAGNode *TR, map<string, int> WidthTableOriginal)
{
  map<string, int> WidthTable;
  // The function getUpdatedWidthTable updates the width table. Currently, it has only entries var_0, var_1 for all vars. We need to add vr_2 also. WidthTable is the recreated WidthTable
  showMap(WidthTableOriginal);
  getUpdatedWidthTable(WidthTableOriginal, WidthTable);
  showMap(WidthTable);

  int squaring_times = 3;

  cout<<"\nTR = "<<getDAGStr(TR)<<endl;
  DAGNode *SquaredClosure = findInitialSquaredClosure(TR, WidthTableOriginal);
  cout<<"\nSquaredClosure = "<<getDAGStr(SquaredClosure)<<endl;
  
  //exit(1);
	
  for(int i=0;i<squaring_times;i++)
    {
      SquaredClosure = getSquareModified(SquaredClosure, WidthTable);
      cout<<endl<<i+1<<" th squaring completed\n";
      //cout<<endl<<"SquaredClosure = "<<getDAGStr(SquaredClosure)<<endl;

      if(isTrue(SquaredClosure)) break;
      //int char_from_user = getchar();
    }

  cout<<"\nUMC completed\n";
}

// This function performs unbounded model checking using dds. This is the third/final version for UMC using LMDDs.
// TR as a dag is taken as input and the transitive closure for the specified number of squarings is computed. 
// All the computations are done at the dd level. There is no dd-->dag-->dd conversion in between.
void unboundedModelCheckingModifiedEverythingAtDDLevel(DAGNode *TR, map<string, int> WidthTableOriginal, string infilename)
{
  time_t umc_start, umc_end;
  time(&umc_start);

  
  
  map<string, int> WidthTable;
  // The function getUpdatedWidthTable updates the width table. Currently, it has only entries var_0, var_1 for all vars. We need to add vr_2 also. WidthTable is the recreated WidthTable
  showMap(WidthTableOriginal);
  //getUpdatedWidthTable(WidthTableOriginal, WidthTable);

  int max_delta;
  if(make_squarings_independent)
    {
      max_delta = 3*squaring_times+2;
    }
  else
    {
      max_delta = squaring_times+1;
    }

  getUpdatedWidthTableBasedOnSquaring(WidthTableOriginal, WidthTable, max_delta);
  showMap(WidthTable);
    

  DAGNode* InitialClosure = TR; // InitialClosure = R(s0, s1)

  cout<<"\nInitialClosure = "<<getDAGStr(InitialClosure)<<endl;
  
  DAGNode* TransitiveClosure = ddm.getTransitiveClosure(InitialClosure, squaring_times, WidthTable, infilename);
  
  cout<<"\nUMC completed\n";

  time(&umc_end);
  cout<<"\nUMC Duration = "<<umc_end-umc_start<<" seconds \n";
}

// This function performs unbounded model checking using Monniaux's approach. 
// TR as a dag is taken as input and the transitive closure for the specified number of squarings is computed. 
void unboundedModelCheckingModifiedUsingMonniaux(DAGNode *TR, map<string, int> WidthTableOriginal, string infilename)
{
  time_t umc_start, umc_end;
  time(&umc_start);

  
  
  map<string, int> WidthTable;
  // The function getUpdatedWidthTable updates the width table. Currently, it has only entries var_0, var_1 for all vars. We need to add vr_2 also. WidthTable is the recreated WidthTable
  showMap(WidthTableOriginal);
  //getUpdatedWidthTable(WidthTableOriginal, WidthTable);

  int max_delta;
  max_delta = squaring_times+1;


  getUpdatedWidthTableBasedOnSquaring(WidthTableOriginal, WidthTable, max_delta);


  showMap(WidthTable);
    

  DAGNode* InitialClosure = TR; // InitialClosure = R(s0, s1)

  cout<<"\nInitialClosure = "<<getDAGStr(InitialClosure)<<endl;
  
  DAGNode* TransitiveClosure = getTransitiveClosure(InitialClosure, squaring_times, WidthTable, infilename);
  
  cout<<"\nUMC completed\n";

  time(&umc_end);

  cout<<"\nUMC Duration = "<<umc_end-umc_start<<" seconds \n";
}

// Given TR, unroll it, and print it in SMTLIB format
void unrolling(DAGNode *TR, map<string, int> WidthTableOriginal, string infilename)
{
    
  map<string, int> WidthTable;
  // The function getUpdatedWidthTable updates the width table. Currently, it has only entries var_0, var_1 for all vars. We need to add vr_2 also. WidthTable is the recreated WidthTable
  showMap(WidthTableOriginal);
  //getUpdatedWidthTable(WidthTableOriginal, WidthTable);

  int max_delta;
  max_delta = squaring_times+1;

  getUpdatedWidthTableBasedOnSquaring(WidthTableOriginal, WidthTable, max_delta);

  showMap(WidthTable);

  getUnrolled(TR, squaring_times, WidthTable, infilename);
}

// print details in APQElimStatusFile
void printAPQElimStatus()
{


  fprintf(APQElimStatusFile, "\n%llu\t%llu\t%llu\t%llu\t%llu", APQElim_Calls, APQElim_Calls_Solved_By_QElimLMA, APQElim_Calls_Solved_By_DropLMDs_1-APQElim_Calls_Solved_By_QElimLMA, APQElim_Calls_Solved_By_DropLMDs, APQElim_Calls_Given_To_APQElim_Exp);

  
}

// print details in FILE_Blowup file
void printBlowupStatus()
{
  fprintf(FILE_Blowup, "\n\napply blows = %llu\napply shrinks = %llu\nite blows = %llu\nite shrinks = %llu\nmax blowup in apply = %f\nmin blowup in apply = %f\nmax blowup in ite = %f\nmin blowup in ite = %f\n", apply_blows, apply_shrinks, ite_blows, ite_shrinks, blow_max_apply, blow_min_apply, blow_max_ite, blow_min_ite);
  
}

// print in layer1_calls_fp and layer2_calls_fp
void initialize_Layer_i_Calls_Files(FILE *layer1_calls_fp, FILE *layer2_calls_fp)
{
  fprintf(layer1_calls_fp, "#LMEs\t#LMDs\t#Bits in var to elim\tTime(millisecs)\n");
  fprintf(layer2_calls_fp, "#LMEs\t#LMDs\t#Bits in var to elim\tTime(millisecs)\n");
}

// print in apqelim_fp
void initialize_APQElim_File(FILE *apqelim_fp)
{
fprintf(apqelim_fp, "#LMEs\t#variables in LMEs\t#zero entries in LMEs\t#non-zero entries in LMEs\t#LMDs\t#variables in LMDs\t#zero entries in LMDs\t#non-zero entries in LMDs\ttime(ms) to solve\n");
}


// open monn_status.txt, write details in it, and close it
void printUMC_MonniauxStatus()
{

  
  FILE* MonnFile = fopen("monn_status.txt", "w");
  if(MonnFile == NULL)
    {
      cout<<"\nCould not create monn_status.txt\n";
      exit(1);
    }
  
  fprintf(MonnFile, "\n\ngetModel time = %d\nGeneralize1 time = %d\nGeneralize2 time = %d\nProject time = %d\nQuant Elim time = %d\n\n", getModel_time, Generalize1_time, Generalize2_time, Project_time, Quant_Monn_time);

 fclose(MonnFile);
  
}


// Get updated widthtable for the -black-blasting mode of LMDD based elimination
void getUpdatedWidthTableForGivenFrame(map<string, int> &WidthTableOriginal, map<string, int> &WidthTable, int myframe)
{
  set<string> ids_processed;
  for(map<string, int>::iterator it=WidthTableOriginal.begin(); it!=WidthTableOriginal.end(); it++)
    {
      string id_delta;
      int width;

      id_delta = it->first;
      width = it->second;

      int index = id_delta.find_last_of("_");
      if(index==string::npos)
	{
	  cout<<"\nVariable name without time stamp in function getUpdatedWidthTableForGivenFrame in main.cc\n";
	  exit(1);
	}
      string id = id_delta.substr(0, index);

      if(ids_processed.find(id) != ids_processed.end()) // id processed. take next one
	continue;
      
      string id_0, id_1;

      char frame_number_char[100];
      sprintf(frame_number_char,"%d", myframe);
      string frame_number_str(frame_number_char);

      char frame_number_char_p1[100];
      sprintf(frame_number_char_p1,"%d", myframe+1);
      string frame_number_str_p1(frame_number_char_p1);


      id_0 = id;
      id_0 += "_";
      id_0 += frame_number_str;
      id_1 = id;
      id_1 += "_";
      id_1 += frame_number_str_p1;

      WidthTable.insert(make_pair(id_0, width));
      WidthTable.insert(make_pair(id_1, width));
      ids_processed.insert(id);
    }
}


//get the model in the map Model_to_Evaluate by parsing model.txt
void getModelFromFile()
{
  
  if(model_file == "")
    {
      cout<<"\nNo model specified in -evaluate_and_get_core mode\n";
      exit(1);
    }


  ifstream *infile;
  infile = new ifstream();
  infile->open(model_file.c_str());

  while(!infile->eof())
    {
      string value;
      string signal;

      *infile>>value;
      if(value=="")
	break;
      cout<<"Line read is "<<value<<endl;
      int index = value.find("=");
      if(index == string::npos)
	{
	  cout<<"\nLine with no = in model.txt\n";
	  exit(1);
	}
      signal = value.substr(0, index);
      value  = value.substr(index+1);
      Model_to_Evaluate.insert(make_pair(signal, value));
    }

  infile->close();

}


// main function
int main(int argc, char** argv)
{

  time_t pgm_start_time, pgm_end_time;
  time(&pgm_start_time);
  


  ImpCheckFile = fopen("impchecks.txt", "w");
  if(ImpCheckFile == NULL)
    {
      cout<<"\nCould not create impchecks.txt\n";
      exit(1);
    }

  APQElimStatusFile = fopen("apqelim.txt", "w");
  if(APQElimStatusFile == NULL)
    {
      cout<<"\nCould not create apqelim.txt\n";
      exit(1);
    }

  FILE_Blowup = fopen("blowup.txt", "w");
  if(FILE_Blowup == NULL)
    {
      cout<<"\nCould not create blowup.txt\n";
      exit(1);
    }


  readArguments(argc, argv);
  
  extern FILE *yyin;
  FILE *fp = fopen(InFileName.c_str(), "r");
  if(fp == NULL)
    {
      cout<<"\nCould not create "<<InFileName<<"\n";
      exit(1);
    }

  yyin = fp;
 	

  if(strcmp(Input_type, "-mat")==0)// Input supplied in a matrix form 
    // Note that the program assumes that the input is in SMTLIB format.
    // Hence, to give input in the matrix format call it as
    // Quantifier_Eliminator -mat test.mat < test.mat where test.mat is the input file
    {
     string layer_1_file_name = "matrix";
     layer_1_file_name += "_layer1_call_details.txt";

     string layer_2_file_name = "matrix";
     layer_2_file_name += "_layer2_call_details.txt";
	      
     layer1_calls_fp = fopen(layer_1_file_name.c_str(), "w");
     if(layer1_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_1_file_name<<"\n";
		    exit(1);
		  } 

     layer2_calls_fp = fopen(layer_2_file_name.c_str(), "w");
     if(layer2_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_2_file_name<<"\n";
		    exit(1);
		  } 


     initialize_Layer_i_Calls_Files(layer1_calls_fp, layer2_calls_fp);


      bool stp_form=true;
      if(stp_form)
	{
	  qEliminationOnMatrix_STP_Form();
	}
      else
	{
	  qEliminationOnMatrix();
	}

      fclose(ImpCheckFile);
      fclose(APQElimStatusFile);
      fclose(FILE_Blowup);
      fclose(fp);
      fclose(layer1_calls_fp);
      fclose(layer2_calls_fp);  

      time(&pgm_end_time);
      cout<<"\nProgram Execution Time = "<<pgm_end_time - pgm_start_time<<" seconds\n";

    }
  else if(strcmp(Input_type, "-dat")==0)// Input supplied in a dat form
     // This is a form supported by the new implementation of QE
     // This option allows to compare between the new implementation
     // and this implementation
    {
        ifstream* InputFile;
	InputFile = new ifstream();
        InputFile->open(InFileName.c_str());

	ofstream* LayerStatsFile;
	LayerStatsFile = new ofstream();
        
        if(layer_to_test_with_dat_file == 1)
        {
            LayerStatsFile->open("Layer1StatiticsFromOldImplementation.txt", fstream::app);	
        }
        else if(layer_to_test_with_dat_file == 2)
        {
            LayerStatsFile->open("Layer2StatiticsFromOldImplementation.txt", fstream::app);	
        }
        else
        {
            cout<<"\nERROR!layer_to_test_with_dat_file has value -1 in main.cpp\n";
            exit(1);
        }

        callLayerOnDatFormat(InputFile, LayerStatsFile);
        
        InputFile->close();
	LayerStatsFile->close();
    }
  else if(strcmp(Input_type, "-smt_conjunct_sat")==0) // input is conjunction of bv-logic constraints
// which needs to be checked for satisfiability. We linearize this conjunction, to yield a conjunction
// of LMCs, and QE all variables which do not take part in non-linear operations. Result is a simplified
// bv-formula equisat with the original conjunction of bv-logic constraints
    {
     int Index_Dot = InFileName.find_last_of(".");
     if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
     string infilename_withno_ext = InFileName.substr(0, Index_Dot);

     string layer_1_file_name = infilename_withno_ext;
     layer_1_file_name += "_smt_conjunct_sat";
     layer_1_file_name += "_layer1_call_details.txt";

     string layer_2_file_name = infilename_withno_ext;
     layer_2_file_name += "_smt_conjunct_sat";
     layer_2_file_name += "_layer2_call_details.txt";
	      
     layer1_calls_fp = fopen(layer_1_file_name.c_str(), "w");
     if(layer1_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_1_file_name<<"\n";
		    exit(1);
		  } 

     layer2_calls_fp = fopen(layer_2_file_name.c_str(), "w");
     if(layer2_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_2_file_name<<"\n";
		    exit(1);
		  } 


     initialize_Layer_i_Calls_Files(layer1_calls_fp, layer2_calls_fp);

      extern map<string, int> WidthTable;
      yyparse();

      // conjoin all root[i] and put in root[0]
      root[0] = createConjunctionOfRoot();

      bool all_vars_to_elim=true;
      if(all_vars_to_elim)
	{
	  // create VarSetToElim : VarSetToElim is the set of all variables to eliminate
	  VarSetToElim.clear();
	  getDAGNames(root[0], VarSetToElim); // all variables selected for elimination
	}

      // check if there are impure variables in VarSetToElim. If yes, remove them from VarSetToElim with a warning
      updateVarSetToElim(VarSetToElim, root[0]);

      // Linearize the dag root[0] as we can handle only linear operations by CAV'11 work
      map<DAGNode*, DAGNode*> LinearizationTable;
      int count=0;
      root[0] = getLinearizedClone(root[0], LinearizationTable, count, WidthTable, VarSetToElim);

      DAGNode* result = ddm.QELIM_Conjunction_level(root[0], VarSetToElim, WidthTable, smt_conjunct_strategy, InFileName);
          
      if(result->Get_node_name()=="true" || result->Get_node_name()=="false") // result is true/false
	    {
	      cout<<"\nresult = "<<getDAGStr(result);
	    }
      else // result  is the simplified formula equisat. with the original one
	    {
	      // Non-linearize
	      result = getOriginalClone(result, LinearizationTable);
	      cout<<"\n\nresult = "<<getDAGStr(result)<<endl<<endl;
	    }

      printAPQElimStatus();
      
      fclose(ImpCheckFile);
      fclose(APQElimStatusFile);
      fclose(FILE_Blowup);
      fclose(fp);
      fclose(layer1_calls_fp);
      fclose(layer2_calls_fp);  


      time(&pgm_end_time);
      cout<<"\nProgram Execution Time = "<<pgm_end_time - pgm_start_time<<" seconds\n";
    }
  else if(strcmp(Input_type, "-smt_conjunct")==0) // input is conjunction of predicates for QELIM
    {
     int Index_Dot = InFileName.find_last_of(".");
     if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
     string infilename_withno_ext = InFileName.substr(0, Index_Dot);

	
    string apqelim_file_name = infilename_withno_ext;
    apqelim_file_name += "_apqelim_details.txt";

    apqelim_fp = fopen(apqelim_file_name.c_str(), "w");
    if(apqelim_fp == NULL)
		  {
		    cout<<"\nCould not create "<<apqelim_file_name<<"\n";
		    exit(1);
		  } 

     initialize_APQElim_File(apqelim_fp);

     string layer_1_file_name = infilename_withno_ext;
     layer_1_file_name += "_conjunct";
     layer_1_file_name += "_layer1_call_details.txt";

     string layer_2_file_name = infilename_withno_ext;
     layer_2_file_name += "_conjunct";
     layer_2_file_name += "_layer2_call_details.txt";
	      
     layer1_calls_fp = fopen(layer_1_file_name.c_str(), "w");
     if(layer1_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_1_file_name<<"\n";
		    exit(1);
		  } 

     layer2_calls_fp = fopen(layer_2_file_name.c_str(), "w");
     if(layer2_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_2_file_name<<"\n";
		    exit(1);
		  } 


      initialize_Layer_i_Calls_Files(layer1_calls_fp, layer2_calls_fp);

      extern map<string, int> WidthTable;
      yyparse();
      // and all root[i] and put in root[0]
      root[0] = createConjunctionOfRoot();
 
      map<DAGNode*, DAGNode*> LinearizationTable;
      int count=0;
      root[0] = getLinearizedClone(root[0], LinearizationTable, count, WidthTable, VarSetToElim);		

 
      DAGNode* result = ddm.QELIM_Conjunction_level(root[0], VarSetToElim, WidthTable, smt_conjunct_strategy, InFileName); 

      cout<<"\n\nresult = "<<getDAGStr(result)<<endl<<endl; 

      fclose(ImpCheckFile);
      fclose(APQElimStatusFile);
      fclose(FILE_Blowup);
      fclose(fp);
      fclose(layer1_calls_fp);
      fclose(layer2_calls_fp);  


      time(&pgm_end_time);
      cout<<"\nProgram Execution Time = "<<pgm_end_time - pgm_start_time<<" seconds\n";

    }
  else if(strcmp(Input_type, "-smt")==0) // The input is a Boolean combination
    {

      extern map<string, int> WidthTable;
      yyparse();
      // and all root[i] and put in root[0]
      root[0] = createConjunctionOfRoot();
      
      //cout<<"\nroot[0] = "<<getDAGStr(root[0])<<endl;
 
      if(unrolling_mode) // for unrolling the TR
	{
	  unrolling(root[0], WidthTable, InFileName);
	  exit(1);
	}

      map<DAGNode*, DAGNode*> LinearizationTable;
      int count=0;

      if(!blast_and_output_as_verilog_circuit && !core_finding && !core_finding_without_evaluation && !LMI_handling_enabled && !debug_blasting && linearization)
// should not be done in case of unsat core finding,
// LMI handling is enabled (here there's change in the function for linearization)
// should not be done in case of debugging the blasting,
// and when it is specified that linearization should not be done
	{
	  // code for linearization 
	  root[0] = getLinearizedClone(root[0], LinearizationTable, count, WidthTable, VarSetToElim);		
	}	     
      


      if(printing_with_linearizing_mode) // for linearizing the file and printing in file
	{
	  int Index_Dot = InFileName.find_last_of(".");
	  if(Index_Dot == string::npos)
	    {
	      cout<<"\nInput file name without . in the file name in main.cc\n";
	      exit(1);
	    }
	  string infilename_withno_ext = InFileName.substr(0, Index_Dot);
	  string linearized_file = infilename_withno_ext;
	  linearized_file += "_linear.smt";
	  printInputToQEliminatorInSMTLibFormatInGivenFile(root[0], WidthTable, VarSetToElim, linearized_file);
	  exit(1);
	}


      if(core_finding) // evaluate and find unsat core
	{
	  struct timeval start_ms, finish_ms;
	  gettimeofday (&start_ms, NULL);

	  map<DAGNode*, DAGNode*> SubtractionTable;
	  SubtractionTable.clear();
	  root[0] = getSubtractionReplacedClone(root[0], SubtractionTable, WidthTable);


	  getModelFromFile();
		
	  cout<<"\nModel_to_Evaluate\n";
	  for(map<string, string>::iterator mit = Model_to_Evaluate.begin(); mit != Model_to_Evaluate.end(); mit++)
	    {	
	      cout<<endl<<mit->first<<"\t"<<mit->second<<endl;
	    }

	  string result_evaluation = evaluateFunction(root[0], Model_to_Evaluate, WidthTable);

	  set<string> SignalsInUnsatCore;

	  int unsatcore_finding_strategy = 2;

	  if(unsatcore_finding_strategy == 1) // evaluation based strategy
	    {
	      findUnsatCoreEvaluationBasedStrategy(root[0], Model_to_Evaluate, WidthTable, SignalsInUnsatCore);
	    }
	  else if(unsatcore_finding_strategy == 2) // SMT based strategy
	    {

	      if(result_evaluation == "0")
		{
		  cout<<"\nresult of evaluation is "<<result_evaluation<<"\nLet's find the unsat. core\n";
				
		  string temp = findUnsatCoreSMTBasedStrategy(root[0], Model_to_Evaluate, WidthTable, SignalsInUnsatCore);
		}
	      else 	{
		cout<<"\nresult of evaluation is "<<result_evaluation<<"\nCurrently our strategy can be used to find unsat cores only for bit-vector formulas evaluating to false\n";
	      }
				
	    }
	  else
	    {
	      cout<<"\nUnknown value "<<unsatcore_finding_strategy<<" for unsatcore_finding_strategy in main.cpp\n";
	    }

	  showSet(SignalsInUnsatCore, "SignalsInUnsatCore");


	  if(evaluation_result_in_core_file == "")
	    {
	      cout<<"\nNo file specified to write the result of evaluation\n";
	      exit(1);
	    }

	  FILE *EvalCoreFile = fopen(evaluation_result_in_core_file.c_str(), "w");
	  if(EvalCoreFile == NULL)
	    {
	      cout<<"\n"<<evaluation_result_in_core_file<<" cannot be opened\n";
	      exit(1);
	    }
	  fprintf(EvalCoreFile, "%s", result_evaluation.c_str());
	  fclose(EvalCoreFile);

	  if(core_result_in_core_file == "")
	    {
	      cout<<"\nNo file specified to write the unsat. core\n";
	      exit(1);
	    }

	  FILE *CoreFile = fopen(core_result_in_core_file.c_str(), "w");
	  if(CoreFile == NULL)
	    {
	      cout<<"\n"<<core_result_in_core_file<<" cannot be opened\n";
	      exit(1);
	    }

	  for(set<string>::iterator cit = SignalsInUnsatCore.begin(); cit != SignalsInUnsatCore.end(); cit++)
	    {
	      fprintf(CoreFile, "%s\n", (*cit).c_str());
	    }

	  fclose(CoreFile);

	  gettimeofday (&finish_ms, NULL);
		
	  unsatcore_duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  unsatcore_duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

	  printDataInFiles();
		
	  exit(1);
	}


      if(core_finding_without_evaluation) // find unsat core with no prior evaluation
	{
	  struct timeval start_ms, finish_ms;
	  gettimeofday (&start_ms, NULL);


	  map<DAGNode*, DAGNode*> SubtractionTable;
	  SubtractionTable.clear();
	  root[0] = getSubtractionReplacedClone(root[0], SubtractionTable, WidthTable);

	  getModelFromFile();
		
	  cout<<"\nModel_to_Evaluate\n";
	  for(map<string, string>::iterator mit = Model_to_Evaluate.begin(); mit != Model_to_Evaluate.end(); mit++)
	    {	
	      cout<<endl<<mit->first<<"\t"<<mit->second<<endl;
	    }

	  set<string> SignalsInUnsatCore;

	  string temp = findUnsatCoreSMTBasedStrategy(root[0], Model_to_Evaluate, WidthTable, SignalsInUnsatCore);

	  showSet(SignalsInUnsatCore, "SignalsInUnsatCore");


	  if(core_result_in_core_file == "")
	    {
	      cout<<"\nNo file specified to write the unsat. core\n";
	      exit(1);
	    }

	  FILE *CoreFile = fopen(core_result_in_core_file.c_str(), "w");
	  if(CoreFile == NULL)
	    {
	      cout<<"\n"<<core_result_in_core_file<<" cannot be opened\n";
	      exit(1);
	    }

	  for(set<string>::iterator cit = SignalsInUnsatCore.begin(); cit != SignalsInUnsatCore.end(); cit++)
	    {
	      fprintf(CoreFile, "%s\n", (*cit).c_str());
	    }

	  fclose(CoreFile);

	  gettimeofday (&finish_ms, NULL);
		
	  unsatcore_duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  unsatcore_duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

	  printDataInFiles();
		
	  exit(1);
	}

      if(sat_using_qbf)// find sat/unsat using QBF solver
	{
	  bool sat_result = checkSatUsingQBFSolver(root[0], WidthTable);
	  time(&pgm_end_time);
	  cout<<"\nProgram Execution Time = "<<pgm_end_time - pgm_start_time<<endl;
	  exit(1);
	}

      if(blast_and_output_as_verilog_circuit) 
	{
	 cout<<"\nConverting to elaborated Verilog...";
	 int Index_Dot = InFileName.find_last_of(".");
         if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
        string infilename_withno_ext = InFileName.substr(0, Index_Dot);

        string  blasted_output_file = infilename_withno_ext;
        blasted_output_file += ".v";

	map<DAGNode*, DAGNode*> FunctionMap;
	convertRelationToFunctionMap(root[0], FunctionMap);
	printFunctionMapInVerilog(FunctionMap, WidthTable, blasted_output_file);
	cout<<"\nElaboration successful\n\n\n";
	exit(1);	
	}
      
      if(blast_and_output)// bit-blast the formula part and output in DIMACS format
        {
         int Index_Dot = InFileName.find_last_of(".");
         if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
        string infilename_withno_ext = InFileName.substr(0, Index_Dot);

        string  blasted_output_file = "blasted_";
        blasted_output_file  += infilename_withno_ext;
        blasted_output_file += ".cnf";
        
        int TotalWidth = 0;
        time_t duration;  
        unsigned long long int BDDCreationTime_msec;    
        
        if(linearization && LMI_handling_enabled)
         {
            if(LIA_Input)
                {
                    if(LIA_Input_Signed)// Convert each variable x to x+((2^p)/2) mod 2^p
                    {
                            root[0] = convertToUnsigned(root[0], WidthTable);
                    }
                    else
                    {
                        // Add 2^(p-1) to both the sides of the LMI  
                        cout<<"\nAdding Half Modulus to both sides\n";
                        root[0] = addHalfModulusToBothSides(root[0]);

                           // Convert constant to constant - 2^(p-1)
                          //root[0] = convertConstants(root[0], WidthTable);
                    }
                }          
                
                
          cout<<"\nLinearizing the dag\n";
          // code for linearization 
          root[0] = getLinearizedCloneWithLMISupport(root[0], LinearizationTable, count, WidthTable, VarSetToElim);	
          //cout<<"\nLinearized DAG = "<<getDAGStr(root[0])<<endl;
                
          cout<<"\nSimplifying the dag\n";
                
          root[0] = simplifyLMIDag(root[0], WidthTable);// For simplification using some rules
          //cout<<"\nSimplified DAG = "<<getDAGStr(root[0])<<endl;
                
          root[0] = quickSimplify(root[0]); // Simple version of expression simplification
        }
        else
        {
                
            //root[0] = quickSimplify(root[0]); // Simple version of expression simplification        
            
            set<string> Support;
            getDAGNames(root[0], Support);
            
            for(set<string>::iterator s_it = Support.begin(); s_it != Support.end(); s_it++)
                {
                string signal = *s_it;
                int MyWidth = getWidthOfVariable(WidthTable, signal);
                TotalWidth = TotalWidth + MyWidth;
                }
            cout<<"\nTotal number of input bits = "<<TotalWidth<<endl;
        }

	if(write_back_and_exit)
	{
		string  write_back_file  = infilename_withno_ext;
        	write_back_file += "_write_back.smt";

		FILE *write_back_file_ptr = fopen(write_back_file.c_str(), "w");

		assert(write_back_file_ptr != NULL);

		fprintf(write_back_file_ptr,"(benchmark %s\n", infilename_withno_ext.c_str());

		string dag_str = printDAGInSMTLIBFormatAsDAG_WithSupportForLMIs(root[0], WidthTable, write_back_file_ptr);

		fprintf(write_back_file_ptr, "\n:formula %s\n", dag_str.c_str());
		fprintf(write_back_file_ptr, "\n)\n");
		fclose(write_back_file_ptr);
	
		exit(1);
	}
        
        bool exit_after_width_finding = false;
        
        if(exit_after_width_finding)
            {
            
            FILE *width_fp = fopen("circuit_widths.txt", "a+");
            if(width_fp == NULL)
            {
                cout<<"\nCould not open circuit_widths.txt\n";
                exit(1);
            }           
            
            fprintf(width_fp, "\n%s\t%d", InFileName.c_str(), TotalWidth);
            fclose(width_fp);
            
            exit(1);
            }
        
        bool blast_and_bdd_create = false;
        
        if(blast_and_bdd_create)
        {    
           ddm.bitBlastAndCreateBDDInDDManager(root[0], duration, BDDCreationTime_msec);
        }
        else
        {
            bool indepent_variables_provided = true;
            
            if(indepent_variables_provided)
            {
                cout<<"\nLet us read "<< independent_variables_file_name << " into indepent_variables\n";
                
                if(independent_variables_file_name != "")
                {
                     ifstream *independent_variables_file;  
                     independent_variables_file = new ifstream();
                     independent_variables_file->open(independent_variables_file_name.c_str());
                    // Let us read independent_variables_file
                    while(!independent_variables_file->eof())
                    {
                        string variable; 
                        
                        *independent_variables_file>>variable;
                        if(variable == "")
                        {
                                break;
                        }
                    
                       independent_variables.insert(variable);
                    } // while(!independent_variables_file->eof())
                     
                   independent_variables_file->close();  
                     
                }// if(independent_variables_file_name != "")
                
            }// if(indepent_variables_provided)           
            
            blastAndOutputInGivenFile(root[0], WidthTable, blasted_output_file);           
        }
        
        
        FILE *bdd_fp = fopen("bdd_creation_details.txt", "a+");
        if(bdd_fp == NULL)
        {
                cout<<"\nCould not open bdd_creation_details.txt\n";
                exit(1);
        }           
        
        if(dynamic_variable_ordering_in_blast_and_output)
        {
            fprintf(bdd_fp, "\n%s\t%d\t%d\t%llu\tOn\n", InFileName.c_str(), TotalWidth, duration, BDDCreationTime_msec);
        }
        else
        {
            fprintf(bdd_fp, "\n%s\t%d\t%d\t%llu\tOff\n", InFileName.c_str(), TotalWidth, duration, BDDCreationTime_msec);
        }    
        
        fclose(bdd_fp);        
        
        
        exit(1);  
        }
      if(sat_using_stp)// find sat/unsat using STP
	{
	  bool sat_result = ThUnsatSingleFormulaInSMTLibFormatAsDAG(root[0], WidthTable);
	  cout<<"\nProgram Execution Time = "<<pgm_end_time - pgm_start_time<<endl;
	  exit(1);
	}
      if(strcmp(qEliminator_type,"-bit")==0)  // user has selected to apply bit-level qelimination
	{
	  bitBlastAndQuantify(root[0], VarSetToElim, WidthTable);
	  exit(1);
	}

      if(!debug_blasting){
		
	if(strcmp(Output_type,"-umc")==0)// UMC using iterative squaring
	  {
		int Index_Dot = InFileName.find_last_of(".");
		if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
		string infilename_withno_ext = InFileName.substr(0, Index_Dot);

		string Dag_Eliminator_Type_String;
		if(strcmp(Dag_Eliminator_Type, "-mn")==0)
		  {
		    Dag_Eliminator_Type_String = "_Monniuax";
		  }
		else
		  {
		    if(strcmp(Elimination_Type, "-white")==0)
		      {
			Dag_Eliminator_Type_String = "_LMDD_QualMoDE";
		      }
		    else
		      {
			Dag_Eliminator_Type_String = "_LMDD_All_Path_QElim";
		      }
		  }	

		string layer_1_file_name = infilename_withno_ext;
		layer_1_file_name += Dag_Eliminator_Type_String;
		layer_1_file_name += "_umc";
		layer_1_file_name += "_layer1_call_details.txt";

		string layer_2_file_name = infilename_withno_ext;
		layer_2_file_name += Dag_Eliminator_Type_String;
                layer_2_file_name += "_umc";
		layer_2_file_name += "_layer2_call_details.txt";
	      

		layer1_calls_fp = fopen(layer_1_file_name.c_str(), "w");
		if(layer1_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_1_file_name<<"\n";
		    exit(1);
		  } 

		layer2_calls_fp = fopen(layer_2_file_name.c_str(), "w");
		if(layer2_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_2_file_name<<"\n";
		    exit(1);
		  } 


		initialize_Layer_i_Calls_Files(layer1_calls_fp, layer2_calls_fp);

	    if(strcmp(Dag_Eliminator_Type, "-dd")==0)
	      {

		int squaring_mode = 3;
		if(squaring_mode == 1)
		  {
		    unboundedModelCheckingModified(root[0], WidthTable); // root[0] is the TR
		  }
		else if(squaring_mode == 2)
		  {
		    unboundedModelChecking(root[0], WidthTable); // root[0] is the TR
		  }
		else
		  {
		    unboundedModelCheckingModifiedEverythingAtDDLevel(root[0], WidthTable, InFileName); 
		  }
	      }// Dag_Eliminator_Type is -dd
	    else if(strcmp(Dag_Eliminator_Type, "-mn")==0)
	      {
		unboundedModelCheckingModifiedUsingMonniaux(root[0], WidthTable, InFileName);
                printUMC_MonniauxStatus();
	      }// Dag_Eliminator_Type is -mn
	    else
	      {
		cout<<"\nUnknown Dag_Eliminator_Type "<<Dag_Eliminator_Type<<endl;
		exit(1);
	      }

	    ddm.ImplicationCheckCacheStatus();
	    printBlowupStatus();

	    fclose(ImpCheckFile);
	    fclose(APQElimStatusFile);
	    fclose(FILE_Blowup);
	    fclose(fp);
      	    fclose(layer1_calls_fp);
      	    fclose(layer2_calls_fp);  


	  }// for umc

            
	else // Quantifier Elimination from Boolean combination of LMCs / bit-vector constraints
	  {

            if(linearization && LMI_handling_enabled)// Quantifier Elimination on LMCs and LMIs
            {
                
                int Index_Dot = InFileName.find_last_of(".");
		if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
		string infilename_withno_ext = InFileName.substr(0, Index_Dot);
                
                string LMIOutfileName = infilename_withno_ext;
                LMIOutfileName += "_LMEQEOutputData.txt";                
                
                LMIQEOutputDataFp = fopen(LMIOutfileName.c_str(), "w");
                if(LMIQEOutputDataFp == NULL)
                {
                    cout<<"\n"<<LMIOutfileName<<" cannot be opened\n";
                    exit(1);
                }
                
                TimeAnalysisFourierMotzkinFp = fopen("TimeAnalysisFourierMotzkin.txt", "a+");
                if(TimeAnalysisFourierMotzkinFp == NULL)
                {
                    cout<<"\nTimeAnalysisFourierMotzkin.txt cannot be opened\n";
                    exit(1);
                }
                
                TimeAnalysisFp = fopen("TimeAnalysis.txt", "a+");
                if(TimeAnalysisFp == NULL)
                {
                    cout<<"\nTimeAnalysis.txt cannot be opened\n";
                    exit(1);
                }
                
                
                TimeLayer2ChecksFp = fopen("Layer2ChecksData.txt", "a+");
                if(TimeLayer2ChecksFp == NULL)
                {
                    cout<<"\nCould not open Layer2ChecksData.txt\n";
                    exit(1);
                }
                
                if(Layer2CheckUsingBDDs)
                {
                    fprintf(TimeLayer2ChecksFp, "\n%s\tcheck with BDDs\n", InFileName.c_str());
                    fprintf(TimeLayer2ChecksFp, "\nColumn 1 = Size of C\nColumn 2 = Size of D and I\nColumn 3 = Size of blasted C\nColumn 4 = Size of blasted D and I");
                    fprintf(TimeLayer2ChecksFp, "\nColumn 5 = Size of BDD for C\nColumn 6 = Size of BDD for D and I\nColumn 7 = Size of BDD for C and D and I");
                    fprintf(TimeLayer2ChecksFp, "\nColumn 8 = Size of BDD for exists x.(C)\nColumn 9 = Size of BDD for exists x. (C and D and I)");
                    fprintf(TimeLayer2ChecksFp, "\nColumn 10 = Total time\nColumn 11 = Blasting time\nColumn 12 = BDD creation time\nColumn 13 = QE time\n");
                }
                else
                {
                    fprintf(TimeLayer2ChecksFp, "\n%s\tcheck with heuristic\n", InFileName.c_str());
                    fprintf(TimeLayer2ChecksFp, "\nColumn 1 = Check time\n");
                }
                
                fclose(TimeLayer2ChecksFp);
                
                
                OutputDataLayer2ChecksFp = fopen("Layer2ChecksUsingBDDsOutputData.txt", "a+");
                if(OutputDataLayer2ChecksFp == NULL)
                {
                    cout<<"\nCould not open Layer2ChecksUsingBDDsOutputData.txt\n";
                    exit(1);
                }
                
                if(Layer2CheckUsingBDDs)
                {
                    fprintf(OutputDataLayer2ChecksFp, "\n%s\n", InFileName.c_str());
                }
                
                fclose(OutputDataLayer2ChecksFp);              
                
                
                LMIQEImportantDataFp = fopen("LMIQEImportantData.txt", "a+");
                if(LMIQEImportantDataFp == NULL)
                {
                    cout<<"\nLMIQEImportantData.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQEImportantDataFourierMotzkinFp = fopen("LMIQEImportantDataFourierMotzkin.txt", "a+");
                if(LMIQEImportantDataFourierMotzkinFp == NULL)
                {
                    cout<<"\nLMIQEImportantDataFourierMotzkin.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQEImportantDataAdditionalFp = fopen("LMIQEImportantDataAdditional.txt", "a+");
                if(LMIQEImportantDataAdditionalFp == NULL)
                {
                    cout<<"\nLMIQEImportantDataAdditional.txt cannot be opened\n";
                    exit(1);
                }
                
                                
                LMIQEImportantDataOTFp = fopen("LMIQEImportantDataOT.txt", "a+");
                if(LMIQEImportantDataOTFp == NULL)
                {
                    cout<<"\nLMIQEImportantDataOT.txt cannot be opened\n";
                    exit(1);
                }
                
                
                LMIQEImportantDataOTAfterLayer2Fp = fopen("LMIQEImportantDataOTAfterLayer2.txt", "a+");
                if(LMIQEImportantDataOTAfterLayer2Fp == NULL)
                {
                    cout<<"\nLMIQEImportantDataOTAfterLayer2.txt cannot be opened\n";
                    exit(1);
                }
                
                
                LMIQEFinishedLayer3DataFp = fopen("LMIQEFinishedLayer3Data.txt", "a+");
                if(LMIQEFinishedLayer3DataFp == NULL)
                {
                    cout<<"\nLMIQEFinishedLayer3Data.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQEFinishedLayer3TimedOutDataFp = fopen("LMIQEFinishedLayer3TimedOutData.txt", "a+");
                if(LMIQEFinishedLayer3TimedOutDataFp == NULL)
                {
                    cout<<"\nLMIQEFinishedLayer3TimedOutData.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQEFinishedLayer3SCAchievedDataFp = fopen("LMIQEFinishedLayer3SCAchievedData.txt", "a+");
                if(LMIQEFinishedLayer3SCAchievedDataFp == NULL)
                {
                    cout<<"\nLMIQEFinishedLayer3SCAchievedData.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQEFinishedLayer3NoSCDataFp = fopen("LMIQEFinishedLayer3NoSCData.txt", "a+");
                if(LMIQEFinishedLayer3NoSCDataFp == NULL)
                {
                    cout<<"\nLMIQEFinishedLayer3NoSCData.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQEFinishedLayer3TrueDataFp = fopen("LMIQEFinishedLayer3TrueData.txt", "a+");
                if(LMIQEFinishedLayer3TrueDataFp == NULL)
                {
                    cout<<"\nLMIQEFinishedLayer3TrueData.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQEImportantDataBlastingFp = fopen("LMIQEImportantDataBlasting.txt", "a+");
                if(LMIQEImportantDataBlastingFp == NULL)
                {
                    cout<<"\nLMIQEImportantDataBlasting.txt cannot be opened\n";
                    exit(1);
                }
                
                LMIQELayer1EffectivenessFp = fopen("LMIQELayer1EffectivenessData.txt", "a+");
                if(LMIQELayer1EffectivenessFp == NULL)
                {
                    cout<<"\nLMIQELayer1EffectivenessData.txt cannot be opened\n";
                    exit(1);
                }
                
                hashingFp = fopen("hashing_in_lmdd_with_monniaux.txt", "a+");
                if(hashingFp == NULL)
                {
                    cout<<"\nhashing_in_lmdd_with_monniaux.txt cannot be opened\n";
                    exit(1);
                }
                
                
                PathBasedStrategyFp = fopen("path_based_strategy_performance.txt", "a+");
                if(PathBasedStrategyFp == NULL)
                {
                    cout<<"\npath_based_strategy_performance.txt cannot be opened\n";
                    exit(1);
                }
                
                MonEnabledAllVarsFp = fopen("MonEnabledAllVarsStrategyPerformance.txt", "a+");
                if(MonEnabledAllVarsFp == NULL)
                {
                    cout<<"\nMonEnabledAllVarsStrategyPerformance.txt cannot be opened\n";
                    exit(1);
                } 
                
                ConversionToDisjunctionOfProblemsFp = fopen("ConversionToDisjunctionOfProblems.txt", "a+");
                if(ConversionToDisjunctionOfProblemsFp == NULL)
                {
                    cout<<"\nConversionToDisjunctionOfProblems.txt cannot be opened\n";
                    exit(1);
                } 
                
                ConversionToDisjunctionOfProblemsWithLMEsFp = fopen("ConversionToDisjunctionOfProblemsWithAggressiveLMEBasedSimplification.txt", "a+");
                if(ConversionToDisjunctionOfProblemsWithLMEsFp == NULL)
                {
                    cout<<"\nConversionToDisjunctionOfProblemsWithAggressiveLMEBasedSimplification.txt cannot be opened\n";
                    exit(1);
                } 
                
                LMEBasedSimplificationAsPreprocessor_OutputFile = fopen("LMEBasedSimplificationAsPreProcessorDetails.txt", "a+");
                if(LMEBasedSimplificationAsPreprocessor_OutputFile == NULL)
                {
                    cout<<"\nLMEBasedSimplificationAsPreProcessorDetails.txt cannot be opened\n";
                    exit(1);
                } 
                
                QELMDD_OutputFile = fopen("QELMDDDetails.txt", "a+");
                if(QELMDD_OutputFile == NULL)
                {
                    cout<<"\nQELMDDDetails.txt cannot be opened\n";
                    exit(1);
                } 
                
                SimplifyLMDD_OutputFile = fopen("SimplifyLMDDDetails.txt", "a+");
                if(SimplifyLMDD_OutputFile == NULL)
                {
                    cout<<"\nSimplifyLMDDDetails.txt cannot be opened\n";
                    exit(1);
                } 
                
                ConversionToDisjunctionOfProblemsWithSMTSolverFp = fopen("ConversionToDisjunctionOfProblemsUsingSMTCalls.txt", "a+");
                if(ConversionToDisjunctionOfProblemsWithSMTSolverFp == NULL)
                {
                    cout<<"\nConversionToDisjunctionOfProblemsUsingSMTCalls.txt cannot be opened\n";
                    exit(1);
                } 
                
                ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp = fopen("HierarchyFromConversionToDisjunctionOfProblemsUsingSMTCalls.txt", "a+");
                if(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp == NULL)
                {
                    cout<<"\nHierarchyFromConversionToDisjunctionOfProblemsUsingSMTCalls.txt cannot be opened\n";
                    exit(1);
                }
                
                CumulativeTimeLayer2ChecksFp = fopen("CumulativeTimeLayer2Checks.txt", "a+");
                if(CumulativeTimeLayer2ChecksFp == NULL)
                {
                    cout<<"\nCumulativeTimeLayer2Checks.txt cannot be opened\n";
                    exit(1);
                }
                
                // files needed to call the CAV'11 work
                string Dag_Eliminator_Type_String;
		if(strcmp(Dag_Eliminator_Type, "-mn")==0)
		  {
		    Dag_Eliminator_Type_String = "_Monniuax";
		  }
		else if(strcmp(Dag_Eliminator_Type, "-pr")==0)
		  {
		    Dag_Eliminator_Type_String = "_Project";
		  }
	        else if(strcmp(Dag_Eliminator_Type, "-l1b")==0)
		  {
		    Dag_Eliminator_Type_String = "_L1Blast";
		  }
                else if(strcmp(Dag_Eliminator_Type, "-l1ot")==0)
		  {
		    Dag_Eliminator_Type_String = "_L1OT";
		  }
                else if(strcmp(Dag_Eliminator_Type, "-l2ot")==0)
		  {
		    Dag_Eliminator_Type_String = "_L2OT";
		  }
		else if(strcmp(Dag_Eliminator_Type, "-l2ub")==0)
		  {
		    Dag_Eliminator_Type_String = "_ProjectWithBDDBasedLayer2";
		  }
                else if(strcmp(Dag_Eliminator_Type, "-preprocess")==0)
		  {
		    Dag_Eliminator_Type_String = "_preprocess";
		  }
		else
		  {
		    if(strcmp(Elimination_Type, "-white")==0)
		      {
			Dag_Eliminator_Type_String = "_LMDD_QualMoDE";
		      }
		    else
		      {
			Dag_Eliminator_Type_String = "_LMDD_All_Path_QElim";
		      }
		  }	

		string layer_1_file_name = infilename_withno_ext;
		layer_1_file_name += Dag_Eliminator_Type_String;
		layer_1_file_name += "_layer1_call_details.txt";

		string layer_2_file_name = infilename_withno_ext;
		layer_2_file_name += Dag_Eliminator_Type_String;
		layer_2_file_name += "_layer2_call_details.txt";
	      

		layer1_calls_fp = fopen(layer_1_file_name.c_str(), "w");
		if(layer1_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_1_file_name<<"\n";
		    exit(1);
		  } 

		layer2_calls_fp = fopen(layer_2_file_name.c_str(), "w");
		if(layer2_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_2_file_name<<"\n";
		    exit(1);
		  } 

		initialize_Layer_i_Calls_Files(layer1_calls_fp, layer2_calls_fp);

                // files needed to call the CAV'11 work end here
                
                
                
                //cout<<"\nHere we accept LMEs, LMDs, and LMIs\n";
                if(false)
                {
                    ofstream *outfile = new ofstream();
                    //cout<<"\nOriginal DAG = "<<getDAGStr(root[0])<<endl;
                    outfile->open("original_DAGFile.txt");
                    DAGNode* node = root[0];
                    cout<<"\nSize of original dag = "<<dm.getSizeOfDAG(node)<<endl;
                     *outfile<<"\nOriginal Formula "<<endl;
                    dm.printDAG(node, outfile);
                    outfile->close();
                    //showMap(WidthTable);
                }
                
                
                if(LIA_Input)
                {
                    if(LIA_Input_Signed)// Convert each variable x to x+((2^p)/2) mod 2^p
                    {
                            root[0] = convertToUnsigned(root[0], WidthTable);
                    }
                    else
                    {
                        // Add 2^(p-1) to both the sides of the LMI  
                        cout<<"\nAdding Half Modulus to both sides\n";
                        root[0] = addHalfModulusToBothSides(root[0]);

                           // Convert constant to constant - 2^(p-1)
                          //root[0] = convertConstants(root[0], WidthTable);
                    }
                }
                
                if(false)
                { 
                    ofstream *outfile = new ofstream();
                   outfile->open("unsigned_DAGFile.txt");
                    DAGNode* node = root[0];
                    cout<<"\nSize of unsigned dag = "<<dm.getSizeOfDAG(node)<<endl;
                     *outfile<<"\nUnsigned Formula "<<endl;
                    dm.printDAG(node, outfile);
                    outfile->close();
                    //exit(1);
                    //showMap(WidthTable);
                }
                
                //cout<<"\nFirst step is linearization\n";
                // code for linearization 
                root[0] = getLinearizedCloneWithLMISupport(root[0], LinearizationTable, count, WidthTable, VarSetToElim);	
                //cout<<"\nLinearized DAG = "<<getDAGStr(root[0])<<endl;
                
                root[0] = simplifyLMIDag(root[0], WidthTable);// For simplification using some rules
                //cout<<"\nSimplified DAG = "<<getDAGStr(root[0])<<endl;
                
                getDataForComparison(root[0], VarSetToElim, WidthTable); // For collecting data about the dag to be quantified - No: of LMEs and No: of LMDs
                  
                //Let us see what will happen with top-level strategy?
                if(see_WhatWouldHappenWithTopLevelStrategy)
                {
                    findWhatWouldHappenWithTopLevelStrategy(root[0], VarSetToElim);
                    //cout<<"\nWhatWouldHappenWithTopLevelStrategy = "<<WhatWouldHappenWithTopLevelStrategy<<endl;
                }               
                
                // Code for QE
                DAGNode* result;
                if(strcmp(Dag_Eliminator_Type, "-mn")==0)// Monniaux's scheme for elimination
		  {
		    root[0] = quickSimplify(root[0]); // Simple version of expression simplification
                    
                    if(true)
                        {
                          result = Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForLMIs(root[0], VarSetToElim, WidthTable); 
                        }
                    else
                        {
                        result = Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForLMIs_With_Recursive_Splitting(root[0], VarSetToElim, WidthTable);
                        }//else of if(true)
                
                        fprintf(LMIQEOutputDataFp, "\nHits in Monniaux' hash table = %llu\nMisses in Monniaux's hash table = %llu\n", Monniaux_HT_hits, Monniaux_HT_misses);
 
		   }//if(strcmp(Dag_Eliminator_Type, "-mn")==0)
                else if(strcmp(Dag_Eliminator_Type, "-dd")==0) // QE using LMDDs
		  {
                    root[0] = quickSimplify(root[0]); // Simple version of expression simplification
                    
                    result = ddm.Quantifier_Eliminator(root[0], VarSetToElim, WidthTable);                    
                   
                } // QE using LMDDs
		else if(strcmp(Dag_Eliminator_Type, "-pr")==0 || strcmp(Dag_Eliminator_Type, "-l1b")==0 || strcmp(Dag_Eliminator_Type, "-l1ot")==0 || strcmp(Dag_Eliminator_Type, "-l2ot")==0  || strcmp(Dag_Eliminator_Type, "-l2ub")==0  || strcmp(Dag_Eliminator_Type, "-preprocess")==0) // QE using Project and its variants
		  {
                    root[0] = quickSimplify(root[0]); // Simple version of expression simplification

		    vector<DAGNode*> Conjuncts_In_Root;
		    splitConjunctionIntoVectorGeneralWithSupportForNot(root[0], Conjuncts_In_Root);
		    assert(Conjuncts_In_Root.size() != 0);
                    
		    createVariableMap(WidthTable);	

		    time_t var_elim_start_time, var_elim_end_time;
		    time(&var_elim_start_time);	
                    time_out_start = var_elim_start_time;

		    if(strcmp(Dag_Eliminator_Type, "-pr")==0)
		    {
			result = callProject(Conjuncts_In_Root, VarSetToElim, WidthTable, 0);   
		    }
                    else if(strcmp(Dag_Eliminator_Type, "-l1b")==0)
		    {
              		result = Project_Blast(Conjuncts_In_Root, VarSetToElim, WidthTable);
		    }
		    else if(strcmp(Dag_Eliminator_Type, "-l1ot")==0)
		    {
              		Project_OT(Conjuncts_In_Root, VarSetToElim, WidthTable);
		    	result = createLogicalConstant("true");
		    }
		    else if(strcmp(Dag_Eliminator_Type, "-l2ot")==0)
		    {
              		applyOTAfterLayer2(Conjuncts_In_Root, VarSetToElim, WidthTable);
		    	result = createLogicalConstant("true");
		    }
		    else if(strcmp(Dag_Eliminator_Type, "-l2ub")==0)
		    {
              		Layer2CheckUsingBDDs = true;
                    	result = callProject(Conjuncts_In_Root, VarSetToElim, WidthTable, 0);   
		    }
		    else if(strcmp(Dag_Eliminator_Type, "-preprocess")==0)
		    {
			assert(VarSetToElim.empty());
	  		getDAGNames(root[0], VarSetToElim);
			//cout<<"\nPreprocessing the formula\n";
			result = Layer1AndLayer2(Conjuncts_In_Root, VarSetToElim, WidthTable, 0);

			string qeliminated_filename = infilename_withno_ext;
			qeliminated_filename += "_preprocessed.smt";

			string qeliminated_benchmarkname = infilename_withno_ext;
			qeliminated_benchmarkname += "_preprocessed";
			printQEliminatedResultInSMTLibFormatInGivenFile(result, WidthTable, qeliminated_filename, qeliminated_benchmarkname);

			FILE* PreprocessingDetailsFp = fopen("PreprocessingDetails.txt", "a+");
		        assert(PreprocessingDetailsFp != NULL);

			fprintf(PreprocessingDetailsFp, "\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), total_number_of_variables_to_preprocess, total_number_of_constraints_to_preprocess, total_number_of_lmes_to_preprocess, total_number_of_lmds_to_preprocess, total_number_of_lmis_to_preprocess, total_number_of_variables_to_preprocess_input_to_layer2, total_number_of_constraints_to_preprocess_input_to_layer2, 
total_number_of_variables_to_preprocess_input_to_layer3, total_number_of_constraints_to_preprocess_input_to_layer3, 
layer1_time_to_preprocess, layer2_time_to_preprocess);
		       
			fclose(PreprocessingDetailsFp);
			//cout<<"\nPreprocessed formula printed in "<< qeliminated_filename <<endl;
		    }

		    time(&var_elim_end_time);

		    //if(!timed_out) cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;		
		    //else cout<<"\nQuantifier elimination timed out in "<<(var_elim_end_time - var_elim_start_time)<<" SECONDS "<<endl<<endl;

		    QElim_Time = var_elim_end_time - var_elim_start_time;
                   
                } // QE using LMDDs
		else
		  {
		    cout<<"\nUnknown Dag_Eliminator_Type "<<Dag_Eliminator_Type<<endl;
		    exit(1);
		  }
                
                 fprintf(hashingFp, "\n%s\t%llu\t%llu", InFileName.c_str(), call_LMDD_Monniaux_SingleVar_calls, call_LMDD_Monniaux_SingleVar_calls_on_zero);
                   
                 fprintf(hashingFp, "\t%llu\t%llu\t%llu", call_LMDD_Monniaux_SingleVar_calls_hits, call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat, call_LMDD_Monniaux_SingleVar_calls_misses_with_project);
                  
                 fprintf(hashingFp, "\t%llu\t%llu\t%llu", call_LMDD_Monniaux_SingleVar_calls_misses_without_project, call_LMDD_Monniaux_SingleVar_calls_first_smt_calls, call_LMDD_Monniaux_SingleVar_calls_second_smt_calls);
                                      
                 fprintf(hashingFp, "\t%d\t%d", call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_total_time, call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time);
                    
                 
                    
                 fprintf(PathBasedStrategyFp,  "\n\n%s\t%llu\t%llu", InFileName.c_str(), project_calls_in_path_based_strategy_in_lmdd_monniaux, start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux);
                   
                 fprintf(PathBasedStrategyFp, "\t%llu\t%llu", time_in_start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux, startup_project_calls_in_path_based_strategy_in_lmdd_monniaux);
                 
                 fprintf(PathBasedStrategyFp, "\t%llu\t%llu", start_up_qe_lmdd_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux, start_up_time_spent_in_qe_lmdd);
                                 
                 fprintf(PathBasedStrategyFp, "\t%llu\t%llu", max_path_depth_in_path_based_strategy_in_lmdd_monniaux, nodes_on_path_in_path_based_strategy_in_lmdd_monniaux);
                    
                                
                fprintf(PathBasedStrategyFp, "\t%llu\t%llu", hit_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux, one_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux);

                fprintf(PathBasedStrategyFp, "\t%llu\t%llu", free_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux, dagresolve_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux);
                
                fprintf(PathBasedStrategyFp, "\t%llu", nonterminal_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux);
                
                fprintf(PathBasedStrategyFp, "\t%llu", computed_offpathchild_in_path_based_strategy_in_lmdd_monniaux);
                
                fprintf(PathBasedStrategyFp, "\t%llu", hashed_offpathchild_in_path_based_strategy_in_lmdd_monniaux);
                
                fprintf(PathBasedStrategyFp, "\t%llu", zero_offpathchild_in_path_based_strategy_in_lmdd_monniaux);
                
                fprintf(PathBasedStrategyFp, "\t%llu", one_offpathchild_in_path_based_strategy_in_lmdd_monniaux);
                
                
                 fprintf(PathBasedStrategyFp, "\t%d\t%d", int_select_strategy_calls, int_select_strategy_ddfreeofvar);

                 fprintf(PathBasedStrategyFp, "\t%d", int_select_strategy_dagresolve);
                 
                 fprintf(PathBasedStrategyFp, "\t%d", int_select_strategy_lowlevel);

                 fprintf(PathBasedStrategyFp, "\t%d\t%d", int_select_strategy_size, int_select_strategy_for_monniax);

                 fprintf(PathBasedStrategyFp, "\t%d", int_select_strategy_for_lmdd_default);
                
                 // Time spent in QE_LMDD in each case
                  fprintf(PathBasedStrategyFp, "\t%llu", int_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar);

                 fprintf(PathBasedStrategyFp, "\t%llu", int_time_spent_in_qe_lmdd_for_select_strategy_dagresolve);
                 
                 fprintf(PathBasedStrategyFp, "\t%llu", int_time_spent_in_qe_lmdd_for_select_strategy_lowlevel);

                 fprintf(PathBasedStrategyFp, "\t%llu", int_time_spent_in_qe_lmdd_for_select_strategy_size);

                 fprintf(PathBasedStrategyFp, "\t%llu", int_time_spent_in_qe_lmdd_for_select_strategy_default);
                 
                
                fprintf(PathBasedStrategyFp, "\t%llu", smt_calls_on_path_in_path_based_strategy_in_lmdd_monniaux);

                fprintf(PathBasedStrategyFp, "\t%llu\t%llu", time_in_smt_calls_on_path_in_path_based_strategy_in_lmdd_monniaux, unsat_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux);

                fprintf(PathBasedStrategyFp, "\t%llu", project_calls_on_path_in_path_based_strategy_in_lmdd_monniaux);              
                  
                 
                 
                 fprintf(PathBasedStrategyFp, "\t%s", WhatWouldHappenWithTopLevelStrategy.c_str());
                 
                 //cout<<"\ntotal_generalization_and_projection_time = "<<total_generalization_and_projection_time<<endl;
                 //cout<<"\nproject_calls_in_path_based_strategy_in_lmdd_monniaux = "<<project_calls_in_path_based_strategy_in_lmdd_monniaux<<endl;
                 
                 avg_generalization_and_projection_time = (float)total_generalization_and_projection_time / (float)project_calls_in_path_based_strategy_in_lmdd_monniaux;
                 
                 //cout<<"\navg_generalization_and_projection_time = "<<avg_generalization_and_projection_time<<endl;
                 
                 
                 fprintf(PathBasedStrategyFp, "\t%f", avg_generalization_and_projection_time);     
                 
                 
                 //cout<<"\ntotal_vars_to_elim_in_projects = "<<total_vars_to_elim_in_projects<<endl;                 
                 avg_vars_to_elim_in_projects = (float)total_vars_to_elim_in_projects / (float)project_calls_in_path_based_strategy_in_lmdd_monniaux;
                 //cout<<"\navg_vars_to_elim_in_projects = "<<avg_vars_to_elim_in_projects<<endl;
                 
                 
                 fprintf(PathBasedStrategyFp, "\t%f", avg_vars_to_elim_in_projects);    
                 
                 fprintf(PathBasedStrategyFp, "\t%d", number_of_variables_for_which_no_elimination_needed);    
                 
                 fprintf(PathBasedStrategyFp, "\t<");    
                 
                 for(list<string>::iterator order_it=OrderInWhichVariablesAreEliminated.begin(); order_it != OrderInWhichVariablesAreEliminated.end(); order_it++)
                    {
                     fprintf(PathBasedStrategyFp, "%s, ", (*order_it).c_str());    
                    }
                 
                 fprintf(PathBasedStrategyFp, ">");    
                 
                 fprintf(PathBasedStrategyFp, "\t%s", FinalQEDoneByWhom.c_str());
                 
                 fprintf(PathBasedStrategyFp, "\t%f", NoVarsToElim_To_NoVars_Ratio_For_Monniaux);
                  
                 fprintf(PathBasedStrategyFp, "\t%d", NoVarsRemaining_For_Monniaux);
                 
                 fprintf(PathBasedStrategyFp, "\t%d\t%d\t%d", monniaux_calls_at_start_up_in_use_path_based_strategy_in_lmdd_monniaux+1, Highest_Level_For_QE_LMDD, Lowest_Size_For_QE_LMDD);
                         
                 
                 fprintf(PathBasedStrategyFp, "\t%llu\t%llu", total_generalization_and_projection_time, time_spent_in_qe_lmdd);
                 
                 
                 fprintf(PathBasedStrategyFp, "\t%d\t%d", ext_select_strategy_calls, ext_select_strategy_ddfreeofvar);

                 fprintf(PathBasedStrategyFp, "\t%d", ext_select_strategy_dagresolve);
                 
                 fprintf(PathBasedStrategyFp, "\t%d", ext_select_strategy_lowlevel);

                 fprintf(PathBasedStrategyFp, "\t%d\t%d", ext_select_strategy_size, ext_select_strategy_for_monniax);

                 fprintf(PathBasedStrategyFp, "\t%d", ext_select_strategy_for_lmdd_default);
                
                 // Time spent in QE_LMDD in each case
                  fprintf(PathBasedStrategyFp, "\t%llu", ext_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar);

                 fprintf(PathBasedStrategyFp, "\t%llu", ext_time_spent_in_qe_lmdd_for_select_strategy_dagresolve);
                 
                 fprintf(PathBasedStrategyFp, "\t%llu", ext_time_spent_in_qe_lmdd_for_select_strategy_lowlevel);

                 fprintf(PathBasedStrategyFp, "\t%llu", ext_time_spent_in_qe_lmdd_for_select_strategy_size);

                 fprintf(PathBasedStrategyFp, "\t%llu", ext_time_spent_in_qe_lmdd_for_select_strategy_default);
                 
                 if(use_path_based_strategy_in_lmdd_monniaux)
                 {
                      fprintf(PathBasedStrategyFp, "\tLMDD+Monniaux");
                 }
                 else
                 {
                     fprintf(PathBasedStrategyFp, "\t%s", Dag_Eliminator_Type);
                 }
                 
                 fprintf(PathBasedStrategyFp, "\t%s\n", machine_name.c_str());
                 
                 
                 

                if(check_f_condition_in_chunks_in_lmdd_with_monniaux)
                    {
                    if(use_original_f_for_checking_in_lmdd_with_monniaux)
                        {
                        fprintf(hashingFp, "\toriginal f AND NOT Pi in chunks of %d", f_condition_chunk_size_in_lmdd_with_monniaux);
                        }
                    else
                        {
                        fprintf(hashingFp, "\tf AND NOT Pi in chunks of %d", f_condition_chunk_size_in_lmdd_with_monniaux);
                        }
                    }
                else if(check_f_condition_in_lmdd_with_monniaux)
                    {
                    if(use_original_f_for_checking_in_lmdd_with_monniaux)
                        {
                        fprintf(hashingFp, "\toriginal f AND NOT Pi"); 
                        }
                    else
                        {
                        fprintf(hashingFp, "\tf AND NOT Pi"); 
                        }                       
                    }
                else if(check_h_condition_in_lmdd_with_monniaux)
                    {
                    fprintf(hashingFp, "\tE AND D AND I AND C_{NOT x} AND h AND NOT Pi");
                    }
                else if(check_path_condition_in_lmdd_with_monniaux)
                    {
                    fprintf(hashingFp, "\tE AND D AND I AND C_{NOT x} AND Path AND NOT Pi");
                    }
                else
                    {
                   fprintf(hashingFp, "\tE AND D AND I AND C_{NOT x} AND Path"); 
                    }                    
                fprintf(hashingFp, "\t%llu\t%llu\t%llu", call_LMDD_Monniaux_SingleVar_calls_misses_on_sat, call_LMDD_Monniaux_SingleVar_calls_misses_on_sat_sat_with_pi, call_LMDD_Monniaux_SingleVar_calls_misses_on_sat_unsat_with_pi); 

                fprintf(hashingFp, "\t%llu\t%llu", call_LMDD_Monniaux_SingleVar_calls_to_simplified_path_condition, call_LMDD_Monniaux_SingleVar_calls_to_simplified_path_condition_sat);

                
                fprintf(MonEnabledAllVarsFp, "\n%s\t", InFileName.c_str());
                   
                fprintf(MonEnabledAllVarsFp, "%llu\t", CallsToQEWithMonEnabledAllVarStrategy);
                
                fprintf(MonEnabledAllVarsFp, "%llu\t", CallsToQEWithMonEnabledAllVarStrategyGlobalHashed);
                
                fprintf(MonEnabledAllVarsFp, "%llu\t", CallsToQEWithMonEnabledAllVarStrategyOnTerminals);
                
                fprintf(MonEnabledAllVarsFp, "%llu\t", CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsFreeOfAllVars);
                
                fprintf(MonEnabledAllVarsFp, "%llu\t", CallsToGetSimplifiedInstance);
                
                fprintf(MonEnabledAllVarsFp, "%llu\t", CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsWhereCallToGetSimplifiedInstanceSolved);
                 
                            
                if(UseMonniauxEnabledAllVariableStrategy)
                 {
                      fprintf(MonEnabledAllVarsFp, "AllVars_QE_LMDD\t");
                 }
                 else
                 {
                     fprintf(MonEnabledAllVarsFp, "%s\t", Dag_Eliminator_Type);
                 }
                 
                fprintf(MonEnabledAllVarsFp, "%s\t", machine_name.c_str());
                
                fprintf(MonEnabledAllVarsFp, "%llu\n", TimeSpentInGetSimplifiedInstance);
                 
                 
                fprintf(ConversionToDisjunctionOfProblemsFp, "\n\n%s\t", InFileName.c_str());
                
                
                if(DemandBasedConversionToDisjunctions)
                    {
                    fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", NumberOfUnsatTailedTrianglesInDemandBasedConversionToDisjunctions);
                    
                    fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", NumberOfSplitTailedTrianglesInDemandBasedConversionToDisjunctions);
                    }
                else
                    {
                    fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", NumberOfTailedTriangles);
                    }
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", NumberOfTailedTrianglesToQE);
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", TotalSMTTimeInConversionToDisjunctionOfTailedTriangles);
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", TotalQETimeInConversionToDisjunctionOfTailedTriangles);
                 
                
                AvgSizeOfTriangles = (float)TotalSizeOfTriangles/(float)NumberOfTailedTrianglesToQE;
                AvgSizeOfTails = (float)TotalSizeOfTails/(float)NumberOfTailedTrianglesToQE;
                AvgVarsToElimFromTailedTriangles = (float)TotalVarsToElimFromTailedTriangles/(float)NumberOfTailedTrianglesToQE;
                AvgVarsEliminatedByLMEsFromTailedTriangles = (float)TotalVarsEliminatedByLMEsFromTailedTriangles/(float)NumberOfTailedTrianglesToQE;
                AvgVarsEliminatedFreeTrianglesFromTailedTriangles = (float)TotalVarsEliminatedFreeTrianglesFromTailedTriangles/(float)NumberOfTailedTrianglesToQE;
                AvgRemainingVarsEliminatedFromTailedTriangles = (float)TotalRemainingVarsEliminatedFromTailedTriangles/(float)NumberOfTailedTrianglesToQE;
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%f\t", AvgSizeOfTriangles);
                fprintf(ConversionToDisjunctionOfProblemsFp, "%f\t", AvgSizeOfTails);
                fprintf(ConversionToDisjunctionOfProblemsFp, "%f\t", AvgVarsToElimFromTailedTriangles);
                fprintf(ConversionToDisjunctionOfProblemsFp, "%f\t", AvgVarsEliminatedByLMEsFromTailedTriangles);
                fprintf(ConversionToDisjunctionOfProblemsFp, "%f\t", AvgVarsEliminatedFreeTrianglesFromTailedTriangles);
                fprintf(ConversionToDisjunctionOfProblemsFp, "%f\t", AvgRemainingVarsEliminatedFromTailedTriangles);
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", TimeSpentInProjectsInQE_LMDD);
                
                if(!LMEBasedSimplificationAsPreprocessor)
                {
                    if(ConvertProblemIntoDisjunctionOfTailedTriangles)
                    {
                     if(DemandBasedConversionToDisjunctions)
                        {
                        fprintf(ConversionToDisjunctionOfProblemsFp, "-dd+ConversionToTailedTriangles+DemandBasedTriangleCreation\t"); 
                        }                        
                      else
                        {
                        fprintf(ConversionToDisjunctionOfProblemsFp, "-dd(with conversion to tailed triangles)\t");
                        }
                    }
                    else
                    {
                     fprintf(ConversionToDisjunctionOfProblemsFp, "%s\t", Dag_Eliminator_Type);
                    }
                }
                else
                {
                     if(ConvertProblemIntoDisjunctionOfTailedTriangles)
                    {
                      fprintf(ConversionToDisjunctionOfProblemsFp, "-dd(with conversion to tailed triangles)+LMEBasedSimplificationPreProcessor\t");
                    }
                    else
                    {
                     fprintf(ConversionToDisjunctionOfProblemsFp, "%s+LMEBasedSimplificationPreProcessor\t", Dag_Eliminator_Type);
                    }
              
                }
                 
                fprintf(ConversionToDisjunctionOfProblemsFp, "%s\t", machine_name.c_str());
                 
                fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", TotalSizeOfTriangles);
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t", TotalSizeOfTails);
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%llu\t%llu\t%llu\t", CallsToQEWithMonEnabledAllVarStrategy, CallsToQEWithMonEnabledAllVarStrategyGlobalHashed, CallsToQEWithMonEnabledAllVarStrategy - CallsToQEWithMonEnabledAllVarStrategyGlobalHashed);
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%f\t", (float)CallsToQEWithMonEnabledAllVarStrategyGlobalHashed/(float)CallsToQEWithMonEnabledAllVarStrategy);
                
                
                if(false) //blows-up: a lot of data
                {
                fprintf(ConversionToDisjunctionOfProblemsFp, "\t<");    
                 
                 for(list<string>::iterator order_it=OrderInWhichVariablesAreEliminated.begin(); order_it != OrderInWhichVariablesAreEliminated.end(); order_it++)
                    {
                     fprintf(ConversionToDisjunctionOfProblemsFp, "%s, ", (*order_it).c_str());    
                    }
                 
                 fprintf(ConversionToDisjunctionOfProblemsFp, ">");    
                }
                
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "{");    
                 
                for(list<int>::iterator order_it=SizeOfTrianglesInEagerConversionToDisjunctions.begin(); order_it != SizeOfTrianglesInEagerConversionToDisjunctions.end(); order_it++)
                    {
                     fprintf(ConversionToDisjunctionOfProblemsFp, "%d,", (*order_it));    
                    }
                 
                fprintf(ConversionToDisjunctionOfProblemsFp, "}\t");    
                 
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "{");    
                 
                for(list<string>::iterator order_it=SatUnsatOfTrianglesInEagerConversionToDisjunctions.begin(); order_it !=SatUnsatOfTrianglesInEagerConversionToDisjunctions.end(); order_it++)
                    {
                     fprintf(ConversionToDisjunctionOfProblemsFp, "%s,", (*order_it).c_str());    
                    }
                 
                fprintf(ConversionToDisjunctionOfProblemsFp, "}\t");    
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%d\t", MaximumNoOfSMTCallsInDemandBasedConversionToDisjunctions);    
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%d\t", MaximumSizeOfTriangleInDemandBasedConversionToDisjunctions);    
                
                fprintf(ConversionToDisjunctionOfProblemsFp, "%d\t", SizeOfInputDDInDDBasedQE);    
                 
                if(PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles)
                    {
                    fprintf(ConversionToDisjunctionOfProblemsFp, "%d\t", TimeSpentInMonniaux_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles);    
                    }
                else
                    {
                    fprintf(ConversionToDisjunctionOfProblemsFp, "NO\t");    
                    }
                
                if(CollectConsecutiveSatTailedTriangles_In_ConversionToDisjunctionOfTailedTriangles)
                    {
                     fprintf(ConversionToDisjunctionOfProblemsFp, "%d\n", NumberOfTimesCollectedTailedTrianglesToQE);    
                    }
                else
                    {
                    fprintf(ConversionToDisjunctionOfProblemsFp, "NO\t");    
                    }
                 
                 
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "\n\n%s\t", InFileName.c_str());
                
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%d\t", MaxNoOfSMTCallsInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%d\t", MaxTriangleSizeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%d\t", NoOfInitialSMTCallsInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCallsCommitted);
                
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", TotalInitialSMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", TotalInitialQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", NumberOfTailedTrianglesUnsatInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", NumberOfTailedTrianglesToQEInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", TotalHierarchySMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", TotalHierarchyQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls);
                
                         
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%f\t", AvgSizeOfTriangles);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%f\t", AvgSizeOfTails);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%f\t", AvgVarsToElimFromTailedTriangles);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%f\t", AvgVarsEliminatedByLMEsFromTailedTriangles);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%f\t", AvgVarsEliminatedFreeTrianglesFromTailedTriangles);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%f\t", AvgRemainingVarsEliminatedFromTailedTriangles);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", TimeSpentInProjectsInQE_LMDD);
                
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "DisjunctionBasedWithSMTSolver\t", Dag_Eliminator_Type);                 
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%s\t", machine_name.c_str());
                 
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", TotalSizeOfTriangles);                
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t", TotalSizeOfTails);                
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%llu\t%llu\t%llu\t", CallsToQEWithMonEnabledAllVarStrategy, CallsToQEWithMonEnabledAllVarStrategyGlobalHashed, CallsToQEWithMonEnabledAllVarStrategy - CallsToQEWithMonEnabledAllVarStrategyGlobalHashed);
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%f\t", (float)CallsToQEWithMonEnabledAllVarStrategyGlobalHashed/(float)CallsToQEWithMonEnabledAllVarStrategy);
                            
                
                
                list<string>::iterator satunsat_order_it=SatUnsatOfTrianglesInEagerConversionToDisjunctions.begin();
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "{");    
                for(list<int>::iterator order_it=SizeOfTrianglesInEagerConversionToDisjunctions.begin(); order_it != SizeOfTrianglesInEagerConversionToDisjunctions.end(); order_it++)
                    {
                     fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "(%d,%s),", (*order_it), (*satunsat_order_it).c_str());  
                     satunsat_order_it++;
                    }                 
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "}\t");
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%d\t", SizeOfInputDDInDDBasedQE);    
                fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverFp, "%s\n", StrategyForChoosingInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls.c_str());    
                 
                 
                 
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "\n\n%s\t", InFileName.c_str());                   
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplification);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplificationFalse);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", NoOfCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries/(float)NoOfCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries);
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByLMEs/(float)NoOfCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByHT/(float)NoOfCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForLMEs);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForHT);
                                 
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", NonecasesInConProbDisjTailTriaWithMESimplification);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", SATCallsInConProbDisjTailTriaWithMESimplification);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", SATCallsInConProbDisjTailTriaWithMESimplificationWhichAreSat);
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", SATCallsInConProbDisjTailTriaWithMESimplificationWhichAreUnsat);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", TimeInSATCallsInConProbDisjTailTriaWithMESimplification);
               
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", QE_LMDDCallsInConProbDisjTailTriaWithMESimplification);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", TimeInQE_LMDDCallsInConProbDisjTailTriaWithMESimplification);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", CallsToQEWithMonEnabledAllVarStrategy);
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", CallsToQEWithMonEnabledAllVarStrategyGlobalHashed);                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", LeftcasesInConProbDisjTailTriaWithMESimplification);
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%llu\t", RightcasesInConProbDisjTailTriaWithMESimplification);      
                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)TotalDDSizeInNonecasesInConProbDisjTailTriaWithMESimplification/(float)NonecasesInConProbDisjTailTriaWithMESimplification);  
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)TotalCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification/(float)NonecasesInConProbDisjTailTriaWithMESimplification);  
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)TotalNegCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification/(float)NonecasesInConProbDisjTailTriaWithMESimplification);  
                
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)TotalCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification/(float)NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplificationFalse);  
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%f\t", (float)TotalNegCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification/(float)NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplificationFalse);  
               
                
                if(ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification)
                 {
                      fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "Disjunction+LMESimplification\t");
                 }
                 else
                 {
                     fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%s\t", Dag_Eliminator_Type);
                 }
                 
                fprintf(ConversionToDisjunctionOfProblemsWithLMEsFp, "%s\t", machine_name.c_str());
                
                
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "\n\n%s\t", InFileName.c_str());                   
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", DDSizeBeforeLMEBasedSimplificationAsPreprocessor);                
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", DDSizeAfterLMEBasedSimplificationAsPreprocessor);                
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", TimeInLMEBasedSimplificationAsPreprocessor);                
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", TimeInSimplifyLMDDInLMEBasedSimplificationAsPreprocessor);   
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", CallsToLMEBasedSimplificationAsPreprocessor);                
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", HitsInLMEBasedSimplificationAsPreprocessor);   
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", MissesInLMEBasedSimplificationAsPreprocessor);
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", OriginalCallsToDAGResolve);   
                
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "{");                     
                for(set<string>::iterator var_it=VariablesBenefittedByLMEBasedSimplificationAsPreprocessor.begin(); var_it != VariablesBenefittedByLMEBasedSimplificationAsPreprocessor.end(); var_it++)
                    {
                     fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%s,", (*var_it).c_str());    
                    }                 
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "}\t");   
                
                if(false) //blows-up: a lot of data
                    {
                
                    fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "<");                     
                    for(list<string>::iterator order_it=OrderInWhichVariablesAreEliminated.begin(); order_it != OrderInWhichVariablesAreEliminated.end(); order_it++)
                        {
                        fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%s,", (*order_it).c_str());    
                        }                    
                    fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, ">\t");    
                    }
                
                
                fprintf(SimplifyLMDD_OutputFile, "\n\n%s\t", InFileName.c_str());  
                
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", RecursiveCallsToDAGResolve); 
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", RecursiveCallsToDAGResolveMissed); 
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", RecursiveCallsToDAGResolveMissedNonTerminals);
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", RecursiveCallsToDAGResolveMissedNonTerminalsButFreeOfVarToElim);
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", RecursiveCallsToDAGResolveLeadingToLayer1Call); 
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", RecursiveCallsToDAGResolveLeadingToLayer1CallHit);
               
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", TimeSpentInHTLookUpInsideDAGResolve); 
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", TimeSpentInSMTCallsInsideDAGResolve);   
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", TimeSpentInLayer1CallsInsideDAGResolve);  
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", TimeSpentInDDConversionInsideDAGResolve);
                fprintf(SimplifyLMDD_OutputFile, "%llu\t", TimeSpentInDDCreationInsideDAGResolve);  
                
                
                
                
                if(use_path_based_strategy_in_lmdd_monniaux)
                    {
                    fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "LMDD+Monniaux+LMEBasedSimplificationPreProcessor\t");
                    fprintf(SimplifyLMDD_OutputFile, "LMDD+Monniaux+LMEBasedSimplificationPreProcessor\t");
                    }
               else if(UseMonniauxEnabledAllVariableStrategy)
                    {
                    fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "AllVars_QE_LMDD+LMEBasedSimplificationPreProcessor\t");
                    fprintf(SimplifyLMDD_OutputFile, "AllVars_QE_LMDD+LMEBasedSimplificationPreProcessor\t");
                    }
                else if(ConvertProblemIntoDisjunctionOfTailedTriangles)
                    {
                    fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "-dd(with conversion to tailed triangles)+LMEBasedSimplificationPreProcessor\t");
                    fprintf(SimplifyLMDD_OutputFile, "-dd(with conversion to tailed triangles)+LMEBasedSimplificationPreProcessor\t");
                    }
                else
                    {
                    fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%s+LMEBasedSimplificationPreProcessor\t", Dag_Eliminator_Type);
                    fprintf(SimplifyLMDD_OutputFile, "%s+LMEBasedSimplificationPreProcessor\t", Dag_Eliminator_Type);
                    }
                
                 
               fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%s\t", machine_name.c_str());
               fprintf(SimplifyLMDD_OutputFile, "%s\t", machine_name.c_str());
                
              fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", CallsToLMEBasedSimplificationAsPreprocessorWhereLMDDIsFreeOfVar); 
              fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%llu\t", QETimeUsingLMDDs);
              
              
               if(false) //blows-up: a lot of data
                {
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "<");                     
                for(list<int>::iterator order_it=CountsForVariablesEliminated.begin(); order_it != CountsForVariablesEliminated.end(); order_it++)
                    {
                     fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%d,", (*order_it));    
                    }                 
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, ">\t");    
               }
                
                unsigned long long int SumOfCountsForVariablesEliminated = 0;
                for(list<int>::iterator order_it=CountsForVariablesEliminated.begin(); order_it != CountsForVariablesEliminated.end(); order_it++)
                    {
                     SumOfCountsForVariablesEliminated = SumOfCountsForVariablesEliminated + (*order_it);
                    }   
                fprintf(LMEBasedSimplificationAsPreprocessor_OutputFile, "%f\n", (float)SumOfCountsForVariablesEliminated/(float)CountsForVariablesEliminated.size());                
                
                
                fprintf(QELMDD_OutputFile, "\n\n%s\t", InFileName.c_str());  
                
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_Hashed); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_OnTerminalZero); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_OnTerminalOne); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_OnTerminalOne_Empty); 
                
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_OnTerminalOne_Unsat); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_OnTerminalOne_Sat); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_FreeOfVar); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_SimplifyLMDDCalls); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_SimplifyLMDDCalls_Hashed); 
                
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_SimplifyLMDDCalls_FreeOfVar); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_TraverseDown); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_dd_free_of_var); 
                
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_SimplifyLMDD_calls); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_Time); 
                fprintf(QELMDD_OutputFile, "%llu\t", call_QE_LMDD_Single_Var_TraverseDown_C_bound_with_Var); 
                
                
                fprintf(QELMDD_OutputFile, "{");                     
                for(set<string>::iterator var_it=VariablesBenefittedByLMEBasedSimplificationInsideQELMDD.begin(); var_it != VariablesBenefittedByLMEBasedSimplificationInsideQELMDD.end(); var_it++)
                    {
                     fprintf(QELMDD_OutputFile, "%s,", (*var_it).c_str());    
                    }                 
                fprintf(QELMDD_OutputFile, "}\t");   
                
                
                fprintf(QELMDD_OutputFile, "<");                     
                for(list<string>::iterator var_it=OrderInWhichVariablesAreEliminated.begin(); var_it != OrderInWhichVariablesAreEliminated.end(); var_it++)
                    {
                     fprintf(QELMDD_OutputFile, "%s,", (*var_it).c_str());    
                    }                 
                fprintf(QELMDD_OutputFile, ">\t");   
                
                
                fprintf(QELMDD_OutputFile, "<");                     
                for(list<int>::iterator var_it=SizesOfEliminatedLMDDs.begin(); var_it != SizesOfEliminatedLMDDs.end(); var_it++)
                    {
                     fprintf(QELMDD_OutputFile, "%d,", (*var_it));    
                    }                 
                fprintf(QELMDD_OutputFile, ">\t"); 
                
                fprintf(QELMDD_OutputFile, "<");                     
                for(list<int>::iterator order_it=CountsForVariablesEliminated.begin(); order_it != CountsForVariablesEliminated.end(); order_it++)
                    {
                     fprintf(QELMDD_OutputFile, "%d,", (*order_it));    
                    }                 
                fprintf(QELMDD_OutputFile, ">\t");    
               
                
                if(LMEBasedSimplificationInsideQELMDD)
                    {
                    fprintf(QELMDD_OutputFile, "%s+LMEBasedSimplificationInside\t", Dag_Eliminator_Type);
                    }
                else
                    {
                    fprintf(QELMDD_OutputFile, "%s\t", Dag_Eliminator_Type);
                    }                
                 
               fprintf(QELMDD_OutputFile, "%s\t", machine_name.c_str());
               
               
               if(Layer2CheckUsingBDDs)
                {
                   if(LIA_Input)
                        {
                        fprintf(CumulativeTimeLayer2ChecksFp, "\n%s\t%s\tBDDs\t%llu\t%llu\t%llu\t%d\t%llu\n", InFileName.c_str(), Dag_Eliminator_Type, TotalLayer2CheckTimeOnBlasting, TotalLayer2CheckTimeOnDDCreation, TotalLayer2CheckTimeONQE, WidthInteger, TotalLayer2Checks);
                        }
                   else
                        {
                         fprintf(CumulativeTimeLayer2ChecksFp, "\n%s\t%s\tBDDs\t%llu\t%llu\t%llu\t-\t%llu\n", InFileName.c_str(), Dag_Eliminator_Type, TotalLayer2CheckTimeOnBlasting, TotalLayer2CheckTimeOnDDCreation, TotalLayer2CheckTimeONQE, TotalLayer2Checks);
                         }
                }
               else
               {
                   if(LIA_Input)
                        {
                        fprintf(CumulativeTimeLayer2ChecksFp, "\n%s\t%s\tHeuristic\t%llu\t-\t-\t%d\t%llu\n", InFileName.c_str(), Dag_Eliminator_Type, TotalLayer2CheckTime, WidthInteger, TotalLayer2Checks);
                        }
                   else
                        {
                        fprintf(CumulativeTimeLayer2ChecksFp, "\n%s\t%s\tHeuristic\t%llu\t-\t-\t-\t%llu\n", InFileName.c_str(), Dag_Eliminator_Type, TotalLayer2CheckTime, TotalLayer2Checks);
                        }
               }
                
                printDataInFilesForLMIs();
                
                // Non-linearize
                //cout<<"\nNon-Linearization starts "<<endl;
		bool output_result = false;
	        if(output_result)
		{
			string qeliminated_filename = infilename_withno_ext;
			qeliminated_filename += "_eliminated.smt";

			string qeliminated_benchmarkname = infilename_withno_ext;
			qeliminated_benchmarkname += "_eliminated";
			printQEliminatedResultInSMTLibFormatInGivenFile(result, WidthTable, qeliminated_filename, qeliminated_benchmarkname);
		}
                
		result = getOriginalClone(result, LinearizationTable);
                
                //cout<<"\nNon-Linearization ends "<<endl;
                
                //cout<<"\nNon-Linearized Result = "<<getDAGStr(result)<<endl;
                
                fclose(LMIQEImportantDataBlastingFp);
                
                fclose(LMIQEFinishedLayer3TimedOutDataFp);
                fclose(LMIQEFinishedLayer3SCAchievedDataFp);
                fclose(LMIQEFinishedLayer3NoSCDataFp);
                fclose(LMIQEFinishedLayer3TrueDataFp);
                fclose(LMIQELayer1EffectivenessFp);
                
                 fclose(LMIQEFinishedLayer3DataFp);                 
                 fclose(LMIQEImportantDataOTFp);
                 fclose(LMIQEImportantDataFourierMotzkinFp);
                 fclose(LMIQEImportantDataAdditionalFp);
                 fclose(LMIQEOutputDataFp);
                 fclose(LMIQEImportantDataFp);
                 fclose(layer1_calls_fp);
		 fclose(layer2_calls_fp);
                 fclose(hashingFp);
                 fclose(PathBasedStrategyFp);
                 fclose(MonEnabledAllVarsFp);
                 fclose(ConversionToDisjunctionOfProblemsFp);
                 fclose(ConversionToDisjunctionOfProblemsWithLMEsFp);
                 fclose(LMEBasedSimplificationAsPreprocessor_OutputFile);
                 fclose(SimplifyLMDD_OutputFile);
                 fclose(QELMDD_OutputFile);
                 fclose(ConversionToDisjunctionOfProblemsWithSMTSolverFp);
                 fclose(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp);
                 fclose(LMIQEImportantDataOTAfterLayer2Fp);
                 fclose(TimeAnalysisFourierMotzkinFp);
                 fclose(TimeAnalysisFp);
                 fclose(CumulativeTimeLayer2ChecksFp);
                 
                 
                if(!exactnesscheck_on) 
			{                       
			exit(1);
			}

		// checking for exactness in one direction	        

		bool exact1;

		if(internal_solver == "yices")
		  {
		    cout<<"\nThe functionality of checking exactness is not built with Yices. Exiting...";
                     exit(1);
		  }
		else
		  {
		    exact1 = checkForExactnessInOneDirectionUsingSTP_With_LMI_Support(root[0], result, WidthTable);
		  }	
                
                if(!exact1)
			{
		  	cout<<"\nQuantifier Elimination is Inexact\n";
                        exit(1);
			}
                
                // checking for exactness in the other direction

		bool exact2; 
                // using bit-blasting
		
                DAGNode* not_root;
	
		not_root =  createDAG("not", root[0], LOGICAL_SYMBOL, 1);

		DAGNode* DAG_to_blast = createDAG("and", result, not_root, LOGICAL_SYMBOL, 1); 

		//cout<<"\nDAG_to_blast = "<<getDAGStr(DAG_to_blast)<<endl;

		set<string> VarSetRemaining;
	
		getRemainingVariables(WidthTable, VarSetToElim, VarSetRemaining);

		exact2 = checkForExactnessInOtherDirection(DAG_to_blast, VarSetToElim, VarSetRemaining, WidthTable);
		
		if(exact1 && exact2)
			{
		  	cout<<"\nQuantifier Elimination is Exact\n";
			}
		else
			{
		  	cout<<"\nQuantifier Elimination is Inexact\n";
                        }
                
                
                
                
                
                exit(1);

            }// Quantifier Elimination on LMCs and LMIs ends here
            else if(linearization)// Quantifier Elimination on LMCs
	      {

		int Index_Dot = InFileName.find_last_of(".");
		if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
		string infilename_withno_ext = InFileName.substr(0, Index_Dot);


		string Dag_Eliminator_Type_String;
		if(strcmp(Dag_Eliminator_Type, "-mn")==0)
		  {
		    Dag_Eliminator_Type_String = "_Monniuax";
		  }
		else
		  {
		    if(strcmp(Elimination_Type, "-white")==0)
		      {
			Dag_Eliminator_Type_String = "_LMDD_QualMoDE";
		      }
		    else
		      {
			Dag_Eliminator_Type_String = "_LMDD_All_Path_QElim";
		      }
		  }	

		string layer_1_file_name = infilename_withno_ext;
		layer_1_file_name += Dag_Eliminator_Type_String;
		layer_1_file_name += "_layer1_call_details.txt";

		string layer_2_file_name = infilename_withno_ext;
		layer_2_file_name += Dag_Eliminator_Type_String;
		layer_2_file_name += "_layer2_call_details.txt";
	      

		layer1_calls_fp = fopen(layer_1_file_name.c_str(), "w");
		if(layer1_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_1_file_name<<"\n";
		    exit(1);
		  } 

		layer2_calls_fp = fopen(layer_2_file_name.c_str(), "w");
		if(layer2_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_2_file_name<<"\n";
		    exit(1);
		  } 


		initialize_Layer_i_Calls_Files(layer1_calls_fp, layer2_calls_fp);


		DAGNode* result;

		getLMDECounts(root[0], WidthTable); // For collecting data about the dag to be quantified - No: of LMEs and No: of LMDs

		getDataForComparison(root[0], VarSetToElim, WidthTable); // For collecting data about the dag to be quantified - No: of LMEs and No: of LMDs
  
		if(strcmp(Dag_Eliminator_Type, "-dd")==0) // QE using LMDDs
		  {
		    root[0] = quickSimplify(root[0]); // Simple version of expression simplification
		
		    result = ddm.Quantifier_Eliminator(root[0], VarSetToElim, WidthTable);

		    time(&pgm_end_time);
		
		    cout<<"\nProgram Execution Time = "<<pgm_end_time - pgm_start_time<<endl;

		    if(strcmp(Elimination_Type, "-black_blasting") == 0)// This part applies for only the QBV_Solve reported in CAV'11
		      {
                        cout<<"\nLinearizationTable\n";
			for(map<DAGNode*, DAGNode*>::iterator it=LinearizationTable.begin(); it!=LinearizationTable.end(); it++)
			  {
			    cout<<endl<<getDAGStr(it->first)<<"\t"<<getDAGStr(it->second)<<endl;
			  }	

			// Non-linearize
		    	result = getOriginalClone(result, LinearizationTable);

		    	cout<<"\n\nafter non-linearization, result = "<<getDAGStr(result)<<endl<<endl;

                        if(frame_number_arg==-1)
			  {
			    cout<<"\nInvalid frame number\n";
			    exit(1);
			  }

		        if(output_file_for_bmc=="")
			  {
			    cout<<"\nInvalid output_file_for_bmc\n";
			    exit(1);
			  }

			readcache_for_bmc(); // read cache_for_bmc

                        map<int, list<int> >::iterator cfbit = cache_for_bmc.find(frame_number_arg);
			if(cfbit == cache_for_bmc.end())
			  {
			    cout<<"\nall called for hit frame. exiting\n";
			    exit(1);
			  }

			list<int> hit_frames = cfbit->second;

			map<int, int> DeltaMap;
			DeltaMap.insert(make_pair(0, frame_number_arg));
      			DeltaMap.insert(make_pair(1, frame_number_arg+1));
                        DAGNode* result_shifted = cloneWithRenamedDeltasWithDeltaMap(result, DeltaMap); 

			map<string, int> UpdatedWidthTable;
                        getUpdatedWidthTableForGivenFrame(WidthTable, UpdatedWidthTable, frame_number_arg);

			if(frame_number_arg==0)
			  {
			    printOutputOfQEliminatorInSMTLibFormatInGivenFile(result_shifted, UpdatedWidthTable, output_file_for_bmc,frame_number_arg);
			  }
			else
			  {
			    printOutputOfQEliminatorInSMTLibFormatInGivenFileForNonZeroFrame(result_shifted, UpdatedWidthTable, output_file_for_bmc,frame_number_arg);
			  }

                        for(list<int>::iterator hfit=hit_frames.begin(); hfit!=hit_frames.end(); hfit++)
			  {
			    int myframe = *hfit;
			    map<int, int> DeltaMap;
			    DeltaMap.insert(make_pair(0, myframe));
			    DeltaMap.insert(make_pair(1, myframe+1));
			    DAGNode* result_shifted = cloneWithRenamedDeltasWithDeltaMap(result, DeltaMap); 

			    map<string, int> UpdatedWidthTable;
			    getUpdatedWidthTableForGivenFrame(WidthTable, UpdatedWidthTable, myframe);


			    if(myframe==0)
			      {
				printOutputOfQEliminatorInSMTLibFormatInGivenFile(result_shifted, UpdatedWidthTable, output_file_for_bmc,myframe);
			      }
			    else
			      {
				printOutputOfQEliminatorInSMTLibFormatInGivenFileForNonZeroFrame(result_shifted, UpdatedWidthTable, output_file_for_bmc,myframe);
			      }
			  }
			
			
		      }//if(strcmp(Elimination_Type, "-black_blasting") == 0) ends here
		  }// if(strcmp(Dag_Eliminator_Type, "-dd")==0) ends here
		else if(strcmp(Dag_Eliminator_Type, "-mn")==0)// QE using Monniaux's approach
		  {
		    //cout<<"\nroot[0] = "<<getDAGStr(root[0])<<endl;
		    root[0] = quickSimplify(root[0]); // Simple version of expression simplification
		
		    //cout<<"\nroot[0] = "<<getDAGStr(root[0])<<endl;
		    result = Monniaux_Quantifier_Eliminator_With_TimeRecording(root[0], VarSetToElim, WidthTable);
		  }
		else
		  {
		    cout<<"\nUnknown Dag_Eliminator_Type "<<Dag_Eliminator_Type<<endl;
		    exit(1);
		  }

                result = quickSimplify(result); // Simple version of expression simplification
	      
		cout<<"\nNumber of autonames = "<<LinearizationTable.size()<<endl;
		Number_of_autonames = LinearizationTable.size();

		ddm.ImplicationCheckCacheStatus(); 
		printAPQElimStatus();

		printDataInFiles();

		fclose(ImpCheckFile);
		fclose(APQElimStatusFile);
		fclose(layer1_calls_fp);
		fclose(layer2_calls_fp);
	        fclose(FILE_Blowup);
	        fclose(fp);

	    
		cout<<"\nresult = "<<getDAGStr(result)<<endl;

		if(!exactnesscheck_on) 
			{
			exit(1);
			}

		// checking for exactness in one direction	        

		bool exact1;

		if(internal_solver == "yices")
		  {
		    exact1 = checkForExactnessInOneDirection(root[0], result, WidthTable);
		  }
		else
		  {
		    exact1 = checkForExactnessInOneDirectionUsingSTP(root[0], result, WidthTable);
		  }	
                
                if(!exact1)
			{
		  	cout<<"\nQuantifier Elimination is Inexact\n";
                        exit(1);
			}

		// checking for exactness in the other direction

		bool exact2; 

		if(exactness_checking_cvc3) // using CVC3
		  {
		    exact2 = checkForExactnessInOtherDirectionUsingCVC3(root[0], result, WidthTable, VarSetToElim);
		  }
	
		else // using bit-blasting
		  {
		    DAGNode* not_root;
	
		    not_root =  createDAG("not", root[0], LOGICAL_SYMBOL, 1);

		    DAGNode* DAG_to_blast = createDAG("and", result, not_root, LOGICAL_SYMBOL, 1); 

		    cout<<"\nDAG_to_blast = "<<getDAGStr(DAG_to_blast)<<endl;

		    set<string> VarSetRemaining;
	
		    getRemainingVariables(WidthTable, VarSetToElim, VarSetRemaining);

		    exact2 = checkForExactnessInOtherDirection(DAG_to_blast, VarSetToElim, VarSetRemaining, WidthTable);
		  }

		if(exact1 && exact2)
			{
		  	cout<<"\nQuantifier Elimination is Exact\n";
			}
		else
			{
		  	cout<<"\nQuantifier Elimination is Inexact\n";
			}
	      }// if(linearization) ends here. i.e. QE for LMCs ends here

	    else // QE for bit-vector constraints
	      {
		int Index_Dot = InFileName.find_last_of(".");
		if(Index_Dot == string::npos)
		  {
		    cout<<"\nInput file name without . in the file name in main.cc\n";
		    exit(1);
		  }
		string infilename_withno_ext = InFileName.substr(0, Index_Dot);


		string Dag_Eliminator_Type_String;
		if(strcmp(Dag_Eliminator_Type, "-mn")==0)
		  {
		    Dag_Eliminator_Type_String = "_Monniuax";
		  }
		else
		  {
		    if(strcmp(Elimination_Type, "-white")==0)
		      {
			Dag_Eliminator_Type_String = "_LMDD_QualMoDE";
		      }
		    else
		      {
			Dag_Eliminator_Type_String = "_LMDD_All_Path_QElim";
		      }
		  }	

		string layer_1_file_name = infilename_withno_ext;
		layer_1_file_name += Dag_Eliminator_Type_String;
		layer_1_file_name += "_layer1_call_details.txt";

		string layer_2_file_name = infilename_withno_ext;
		layer_2_file_name += Dag_Eliminator_Type_String;
		layer_2_file_name += "_layer2_call_details.txt";
	      

		layer1_calls_fp = fopen(layer_1_file_name.c_str(), "w");
		if(layer1_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_1_file_name<<"\n";
		    exit(1);
		  } 

		layer2_calls_fp = fopen(layer_2_file_name.c_str(), "w");
		if(layer2_calls_fp == NULL)
		  {
		    cout<<"\nCould not create "<<layer_2_file_name<<"\n";
		    exit(1);
		  } 


		initialize_Layer_i_Calls_Files(layer1_calls_fp, layer2_calls_fp);


		getEquationsDisequationsCounts(root[0], WidthTable); // For collecting data about the dag to be quantified - No: of equations and number of disequations
		getDataForComparison(root[0], VarSetToElim, WidthTable); // For collecting data about the dag to be quantified - dag size, #vars, #vars to elim, #bits to elim
		set<string> ImpureVarsToQElim;
		set<string> PureVarsToQElim;
		updateVarSetToElim(VarSetToElim, root[0], ImpureVarsToQElim, PureVarsToQElim);
		
		DAGNode* result;

		if(strcmp(Dag_Eliminator_Type, "-mn")==0)// Monniaux's scheme for elimination
		  {
		    root[0] = quickSimplify(root[0]); // Simple version of expression simplification
		    result = Monniaux_Quantifier_Eliminator_With_TimeRecording_WithSupportForSelsConcats(root[0], VarSetToElim, WidthTable);

		    exit(1);
		  }
		else
		  {
		    cout<<"\nUnknown Dag_Eliminator_Type "<<Dag_Eliminator_Type<<endl;
		    exit(1);
		  }

		printDataInFilesForBitVectorCases();

		fclose(layer1_calls_fp);
		fclose(layer2_calls_fp);

	      }
	  } // for else of umc
      }// for !debug_blasting ends here
      else // for debug_blasting : for checking the exactness of bit-blasting
	{
                    
	  DAGNode* DAG_to_blast = root[0]; 

	  cout<<"\nDAG_to_blast = "<<getDAGStr(DAG_to_blast)<<endl;

	  set<string> VarSetRemaining;
	
	  getRemainingVariables(WidthTable, VarSetToElim, VarSetRemaining);

	  bool exact2 = checkForExactnessInOtherDirection(DAG_to_blast, VarSetToElim, VarSetRemaining, WidthTable);
	} // debug_blasting

    }

  else

    {
     if(cav)
	    {
	    usage_CAV();
	    }
    else if(limited_options)
	    {
	    usage_limited();
	    }
    else
            {
	    usage(); 
	    }
 

      exit(1);
   }


  return 0;
}


