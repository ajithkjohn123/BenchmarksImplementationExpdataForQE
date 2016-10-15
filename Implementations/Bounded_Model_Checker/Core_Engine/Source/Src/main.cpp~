#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "y.tab.h"

#include "CIdentifier.h"
#include "CEntity.h"

#include "CPrePostProcessor.h"
#include "CSymbSimulator.h"

#include "time.h" // newly added to find the execution times

#include "DagManager.h"
#include "DDManager.h"
#include "BitBlaster.h"
#include "ImplicationCacheTree.h"
#include "DAGEliminator.h"
#include "cudd.h"

#include <sys/time.h>
#include <list>
#include <map>
#include <string>
#include <cassert>

//#define DEBUG

#define  F_condition_chunk_size_in_lmdd_with_monniaux_to_make_it_Monniaux 10000
#define  Min_call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_for_satisfactory_truncation_to_make_it_Monniaux 1
#define  Satisfactory_truncation_in_lmdd_with_monniaux_to_make_it_Monniaux 1

using namespace std;


extern int yydebug;
extern FILE *yyin, *yyout;

extern int MVL_LINNUM;

extern int yyparse(void);
void yyerror(char* str);

map <string, map<string, SymTabEntry> > SymbolTable;
  // entity  var_name    SymTabEntry

map<string, string> EntArch;

// command line options with default values

int debug_mode=0;
int no_of_simulation_cycles=7;
int qelimination_on=0;
int TimesOfUnrolling=-1;
int Slicing=1;
string solver_for_bmc = "smtlib"; // Two options currently : "smtlib" will print in SMTLib format and "yices" will print in Yices format
bool exit_after_vcgeneration = false; // Exit from the program after vcgeneration
bool exit_after_verification = false; // Exit from the program after verification - do not show the results
bool elimination_of_inconsistent_paths = true; // Eliminate inconsistent paths in the LMDD after quantifier elimination 
bool qelim_bit_level = false; 
bool refinement_on = false; 
int no_of_cegar_iterations = 1;
bool psl_property_on_cycles = false; // if true interpret the PSL property as happening on clock cycles, o/w interpret as happening on clock changes
// by default, on cycles
bool preprocessor_off = true; // call the preprocessor initially
bool clock_based_simplification_enabled = false; // for clock based simplification
bool TRNeeedInFile = false; // for generation of transition relation in a file
bool exit_after_tr_generation = false; // exit after generation of TR
string quantifier_eliminator = "qelmdd";

// command line options end here

time_t program_start_time;

CHelper MANAGER;
DDManager ddm;
DAGManager dm;
BVManager* bvm = new BVManager();
DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);

char* infilename;
char* propertyfile;
char* original_infilename;
string TRFileName;

string solver_timeout_seconds = "1800";


time_t ssimulation_time;
time_t qe_time=0;
time_t yices_time_inside_cegar=0;
time_t unsat_core_time=0;


struct timeval program_start_time_ms, program_end_time_ms;
unsigned long long int ssimulation_time_ms;
unsigned long long int qe_time_ms=0;
unsigned long long int yices_time_inside_cegar_ms=0;
unsigned long long int unsat_core_time_ms=0;
unsigned long long int program_duration_ms;
unsigned long long int unsat_core_time_total_ms=0;
int all_call_number=0; // stores the number of calls to all from applyQuantifierElimination i.e. CEGAR loop
int unsat_core_hash_hits=0;
int unsat_core_hash_misses=0; // number of hits and misses in the hash table for unsat core computation
int es_ht_hash_hits=0;
int es_ht_hash_misses=0; // number of hits and misses in the hash table for expression simplification
int qualmode_ht_hash_hits=0;
int qualmode_ht_hash_misses=0; // number of hits and misses in the hash table for qualmode
int inlining_ht_hash_hits=0;
int inlining_ht_hash_misses=0; // number of hits and misses in the hash table for inilining

int GlobalAutonameCount=0;// number of autonames created while linearizing the dag before creating LMDD out of it

unsigned long long int cvc_generation_from_se_time=0;
unsigned long long int total_cegar_time=0;
int cegar_iteration = 1;
unsigned long long int total_avc_generation_time=0;
unsigned long long int total_expose_time=0;
unsigned long long int total_qe_time=0;
unsigned long long int simulation_ms = 0;

map<int, set<string> > RefinementDetails;

FILE *log_file;

bool logging_enabled = true;


// New options for QE

int WidthInteger=0;
bool LIA_Input = false;
bool LIA_Input_Signed = false;
unsigned long long int ModWidthInteger=1;
unsigned long long int ModWidthIntegerHalf=0;
bool code_to_handle_assumptions = true;

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

// New options for QE ends here

void writeToLogFile(string mydata)
{
  log_file = fopen("log.dat", "a");

  if(log_file == NULL) {cout<<"\nCannot open file log.dat\n"; exit(1);}

  fprintf(log_file, "%s", mydata.c_str());

  fclose(log_file);
}  

void CallYices()
{
  
  cout<<"\nVerification condition given to solver...";

  if(logging_enabled)
	  {
	    writeToLogFile("\n\nVerification Condition Given to Solver. Please Wait...\n\n");
	  }


  string yices_command = "yices -e -tc -tm ";
  yices_command += solver_timeout_seconds;
  yices_command += " yices_input.ys > yices_output.txt";

  #ifdef DEBUG
  cout << endl << yices_command << endl;
  #endif

  system(yices_command.c_str());

  FILE *result_file = fopen("yices_output.txt", "r");

  if(result_file == NULL) {cout<<"\nCannot open file yices_output.txt\n"; exit(1);}

  char result[100];
  fscanf(result_file, "%s", result);
  fclose(result_file);
  string result_str(result);

  if(result_str=="unsat") 
    {
      cout<<"\nVHDL design holds the property\n";

      if(logging_enabled)
	  {
	    writeToLogFile("\nVHDL design holds the property\n");
	  }


    }
  else if(result_str=="sat") // CEX obtained in file yices_output.txt
    {

      cout<<"\nVHDL design does not hold the property\n";

      if(logging_enabled)
	  {
	    writeToLogFile("\nVHDL design does not hold the property\n");
	  }

      bool display_cex = true;

      if(display_cex)
	{
		string command = "cexAnalyzer < ";
		string property_file_name(propertyfile);
		command += property_file_name;
		system(command.c_str()); 
		// This 1) convert CEX obtained in file yices_output.txt to generate CEX in VCD format in vcd_form.vcd			
		// and 2) generates the causality set in countersignals.txt 
		
		string envName = "TCL_FILES";
        	char *path_name = getenv(envName.c_str());
		assert(path_name != NULL); 
		string tcl_file(path_name);
        	tcl_file += "/des1.tcl";
		command = "gtkwave -T ";
		command += tcl_file;
		command += " vcd_form.vcd";
		system(command.c_str()); // This displays the cex with causality set highlighted
	}	
      else
	{	
	      	system("cex2vcd"); // convert CEX obtained in file yices_output.txt to generate CEX in VCD format in vcd_form.vcd
	}      
    }

  else
    {

      cout<<"\nUnknown result from the solver. Most probably the solver has timed out!!! Please decrease the bound in the property\n";

      if(logging_enabled)
	  {
	    writeToLogFile("\nUnknown result from the solver. Most probably the solver has timed out!!! Please decrease the bound in the property\n");
	  }


      #ifdef DEBUG
      cout<<"\nUnknown returned from SMT Solver in function CallYices in main.cpp\n"; 
      exit(1);
      #endif
    }
}



void CallYicesAndStopAfterVCDFileGeneration()
{
  
  cout<<"\nVerification condition given to solver...";

  string yices_command = "yices -e -tc -tm ";
  yices_command += solver_timeout_seconds;
  yices_command += " yices_input.ys > yices_output.txt";

  system(yices_command.c_str());

  FILE *result_file = fopen("yices_output.txt", "r");

  if(result_file == NULL) {cout<<"\nCannot open file yices_output.txt\n"; exit(1);}

  char result[100];
  fscanf(result_file, "%s", result);
  fclose(result_file);
  string result_str(result);

  // Let's create the result file names
  string infilename_str(original_infilename);
  string propertyfile_str(propertyfile);
  string infilename_str_without_extension;
  string propertyfile_str_without_extension;

  int index_of_dot = infilename_str.find_last_of(".");
  if(index_of_dot == string::npos)
  {
	infilename_str_without_extension = infilename_str;
  }
  else
  {
	infilename_str_without_extension = infilename_str.substr(0, index_of_dot);
  }

  index_of_dot = propertyfile_str.find_last_of(".");
  if(index_of_dot == string::npos)
  {
	propertyfile_str_without_extension = propertyfile_str;
  }
  else
  {
	propertyfile_str_without_extension = propertyfile_str.substr(0, index_of_dot);
  }

  string sat_result_file = infilename_str_without_extension;
  sat_result_file += "_";
  sat_result_file += propertyfile_str_without_extension;
  sat_result_file += "_";
  sat_result_file += "result.txt";

  string cex_result_file = infilename_str_without_extension;
  cex_result_file += "_";
  cex_result_file += propertyfile_str_without_extension;
  cex_result_file += "_";
  cex_result_file += "cex.vcd";
  

  if(result_str=="unsat") 
    {
      FILE *sat_result_file_fp = fopen(sat_result_file.c_str(), "w");
      assert(sat_result_file_fp != NULL);
      fprintf(sat_result_file_fp,"unsat\n");
      fclose(sat_result_file_fp); 

      cout<<"\nVHDL design holds the property\n"; 
      if(logging_enabled)
	  {
	    writeToLogFile("\nVHDL design holds the property\n");
	  }          
    }
  else if(result_str=="sat") // CEX obtained in file yices_output.txt
    {
      FILE *sat_result_file_fp = fopen(sat_result_file.c_str(), "w");
      assert(sat_result_file_fp != NULL);
      fprintf(sat_result_file_fp,"sat\n");
      fclose(sat_result_file_fp);

      string command = "cexAnalyzer < ";
      string property_file_name(propertyfile);
      command += property_file_name;
      system(command.c_str()); 
      // This 1) convert CEX obtained in file yices_output.txt to generate CEX in VCD format in vcd_form.vcd			
      // and 2) generates the causality set in countersignals.txt 
		
      command = "cp vcd_form.vcd ";
      command += cex_result_file;
      system(command.c_str()); 

      cout<<"\nVHDL design does not hold the property\n"; 
      if(logging_enabled)
	  {
	    writeToLogFile("\nVHDL design does not hold the property\n");
	  }         
                 
    }

  else
    {
      FILE *sat_result_file_fp = fopen(sat_result_file.c_str(), "w");
      assert(sat_result_file_fp != NULL);
      fprintf(sat_result_file_fp,"unknown\n");
      fclose(sat_result_file_fp);      

      cout<<"\nUnknown result from the solver. Most probably the solver has timed out!!!\n";
      if(logging_enabled)
	  {
	    writeToLogFile("\nUnknown result from the solver. Most probably the solver has timed out!!! Please decrease the bound in the property\n");
	  }
    
    }
}




void createFrameSignalMap(set<string> &Signals, map<int, set<string> > &FrameSignalMap)
{
  for(set<string>::iterator it1 = Signals.begin(); it1 != Signals.end(); it1++)
	{
	  string id_delta = *it1;
	  int index = id_delta.find_last_of("_");
	  string id = id_delta.substr(0, index);
	  int delta = atoi(id_delta.substr(index+1).c_str());

	  map<int, set<string> > :: iterator fit = FrameSignalMap.find(delta);

	  if(fit == FrameSignalMap.end()) // no entry for delta
	    {
	      set<string> SignalsAtThisFrame;

	      SignalsAtThisFrame.insert(id_delta);
					
	      FrameSignalMap.insert(make_pair(delta, SignalsAtThisFrame));
	    }
	  else
	    {
	      (fit->second).insert(id_delta);
	    }  

	}// for
  
}//function

void printTimeData()
{
  FILE *time_file = fopen("time.txt", "a+");

  if(time_file == NULL) {cout<<"\nCannot open file time.txt\n"; exit(1);}

  fprintf(time_file,"\n\nVHDL File = %s\nProperty File = %s\nTotal duration = %llu\nParsing + CDFG generation + Symbolic simulation time = %llu\nTime to generate concrete verification condition from symbolic expressions = %llu\nTotal CEGAR time (This mainly includes the following three times) = %llu\nNumber of CEGAR iterations = %d\nTotal time spent to generate the abstract verification condition inside the refinement loop = %llu\nTotal time spent in Yices calls inside the refinement loop = %llu\nTotal time spent in finding the signals to be exposed inside the refinement loop = %llu\nTotal quantifier elimination time (This includes time spent by Qualmode + time for exp.simplification etc. to convert the dag to prop. skelton format)= %llu\nTotal Qualmode tome=%llu\nTotal time spent by unsat core computation (all) in CEGAR loop = %llu\nTotal time spent by unsat core computation (all + preparing to convert to input format of all such as exp. simpl. ) in CEGAR loop = %llu\nTotal time spent in concrete-valued evaluation/simulation in CEGAR loop = %llu\n\nNumber of calls to all for unsat core computation = %d\nNumber of hits in unsat core computation hash table = %d\nNumber of misses in unsat core computation hash table = %d\nNumber of hits in expression simplification hash table = %d\nNumber of misses in expression simplification hash table = %d\nNumber of hits in QuaLMoDE cache = %d\nNumber of misses in QuaLMoDE cache = %d\nNumber of hits in Inlining cache = %d\nNumber of misses in Inlining cache = %d\n", strdup(infilename), strdup(propertyfile), program_duration_ms, ssimulation_time_ms, cvc_generation_from_se_time, total_cegar_time, cegar_iteration, total_avc_generation_time, yices_time_inside_cegar_ms, total_expose_time, total_qe_time, qe_time_ms, unsat_core_time_ms, unsat_core_time_total_ms, simulation_ms, all_call_number, unsat_core_hash_hits, unsat_core_hash_misses, es_ht_hash_hits, es_ht_hash_misses, qualmode_ht_hash_hits, qualmode_ht_hash_misses, inlining_ht_hash_hits, inlining_ht_hash_misses);

  fprintf(time_file,"\nSignals exposed\n");
  for(map<int, set<string> > :: iterator it = RefinementDetails.begin(); it != RefinementDetails.end(); it++)
    {
      fprintf(time_file, "\n\nCEAGR iteration = %d\n", it->first);

      set<string> Signals = it->second;

      map<int, set<string> > FrameSignalMap;

      createFrameSignalMap(Signals, FrameSignalMap);


      for(map<int, set<string> > :: iterator it1 = FrameSignalMap.begin(); it1 != FrameSignalMap.end(); it1++)
	{
	  fprintf(time_file, "\n\nframe = %d\t", it1->first);

	  set<string> SignalsAtThisFrame = it1->second;

	  for(set<string>::iterator it2 = SignalsAtThisFrame.begin(); it2 != SignalsAtThisFrame.end(); it2++)
	    {
	      fprintf(time_file, "%s\t", (*it2).c_str());
	    }
	}
      
    }  
  fclose(time_file);
}


void usage()
{
  cout<<"\nUsage: Bounded_Model_Checker options bounded-psl-property-file vhdl-file\n";
  cout<<"\noptions are\n";
  cout<<"\n-abstraction-on : Execute with quantifier elimination on \n-abstraction-off : Execute with quantifier elimination off  (default)";
  cout<<"\n-quantifier-eliminator=qelmdd/qesmt/qecombined: Select the quantifier elimination algorithm (default is qelmdd)";
  cout<<"\n-unrollings=integer : Maximum number of BMC unrollings";
  //cout<<"\n-slicing-on : Execute with slicing on \n-slicing-off : Execute with slicing off  (default)";
  //cout<<"\n-solver=yices/smtlib : Generate the verification condition in yices/smtlib format (default is yices)";
  cout<<"\n-exit-after-vcgeneration : Exit after verification condition generation \n-continue-after-vcgeneration : Continue after verification condition generation  (default)";
  //cout<<"\n-qelim-bit-level : Do quantifier elimination at bit-level (off by default)";
  //cout<<"\n-refinement-on : Switch on refinement in the CEGAR loop \n-refinement-off : Switch off refinement in the CEGAR loop. i.e. exit after verification condition generation using the initial abstract transition relation (default)";
  //cout<<"\n-cegar-iterations=integer : Maximum number of iterations in the CEGAR loop (default is 1)";
  //cout<<"\n-solver-timeout=integer : Timeout for the solver in seconds (default is 1800)";
  //cout<<"\n-pre-processing-off : Switch off pre-processing of VHDL design (on by default)";
  //cout<<"\n-enable-clock-based-simplification : Simplify transition relation assuming that the design is positive edge triggered and clock's name is clk (off by default)";
  cout<<"\n-tr-needed-in-file : Print the transition relation in a file (off by default)\n\n";
  //cout<<"\n-exit_after_tr_generation : Exit after generating the transition relation (off by default)";
  //cout<<"\n-psl-property-on-cycles : In the PSL property, each step is a clock cycle (default)\n-psl-property-on-changes : In the PSL property, each step is a change in clock\n\n";
}

void showOptions()
	{
	  cout<<"\nDebug mode = "<<debug_mode;
	  cout<<"\nNumber of simulation cycles = "<<no_of_simulation_cycles;
	  cout<<"\nQElimination_on = "<<qelimination_on;
	  cout<<"\nTimesOfUnrolling = "<<TimesOfUnrolling;
	  cout<<"\nSlicing = "<<Slicing;
	  cout<<"\nSolver = "<<solver_for_bmc;
	  cout<<"\nexit_after_vcgeneration = "<<exit_after_vcgeneration; 
          cout<<"\nexit_after_verification = "<<exit_after_verification;
	  cout<<"\nelimination_of_inconsistent_paths = "<<elimination_of_inconsistent_paths;
	  cout<<"\nqelim_bit_level = "<<qelim_bit_level;
	  cout<<"\nrefinement_on = "<<refinement_on;
	  cout<<"\nNumber of CEGAR iterations = "<<no_of_cegar_iterations;
	  cout<<"\nPSL property on cycles = "<<psl_property_on_cycles; 
          cout<<"\nsolver_timeout_seconds = "<<solver_timeout_seconds;
	  cout<<"\nVHDL file = "<<infilename;
	  cout<<"\npreprocessor_off = "<<preprocessor_off;
	  cout<<"\nProperty file = "<<propertyfile<<endl;
	}	

void readArguments(int argc, char** argv)
{

if(argc<3) {usage(); exit(1);}
int i=1;
while(i<=argc-3)
	{
        char* argument=argv[i];
	if(strcmp(argument,"-debug-on")==0)
		{
		  debug_mode=1;
		}
	else if(strcmp(argument,"-debug-off")==0)
		{
		  debug_mode=0;
		}
	else if(strcmp(argument,"-abstraction-on")==0)
		{
		 qelimination_on=1;
		}
	else if(strcmp(argument,"-abstraction-off")==0)
		{
		 qelimination_on=0;
		}
	else if(strcmp(argument,"-slicing-on")==0)
		{
		 Slicing=1;
		}
	else if(strcmp(argument,"-slicing-off")==0)
		{
		  Slicing=0;
		}
	else if(strcmp(argument,"-exit-after-vcgeneration")==0)
		{
		 exit_after_vcgeneration = true;
		}
	else if(strcmp(argument,"-continue-after-vcgeneration")==0)
		{
		 exit_after_vcgeneration = false;
		}
        else if(strcmp(argument,"-exit-after-verification")==0)
		{
		 exit_after_verification = true;
		}
	else if(strcmp(argument,"-continue-after-verification")==0)
		{
		 exit_after_verification = false;
		}
	else if(strcmp(argument,"-inconsistent-path_elim-on")==0)
		{
		 elimination_of_inconsistent_paths = true;
		}
	else if(strcmp(argument,"-inconsistent-path_elim-off")==0)
		{
		 elimination_of_inconsistent_paths = false;
		}
	else if(strcmp(argument,"-qelim-bit-level")==0)
		{
		  qelim_bit_level = true;
		}
	else if(strcmp(argument,"-refinement-on")==0)
		{
		 refinement_on = true;
		}
	else if(strcmp(argument,"-refinement-off")==0)
		{
		 refinement_on = false;
		}
	else if(strcmp(argument,"-psl-property-on-cycles")==0)
		{
		 psl_property_on_cycles = true;
		}
	else if(strcmp(argument,"-psl-property-on-changes")==0)
		{
		 psl_property_on_cycles = false;
		}
	else if(strcmp(argument,"-pre-processing-off")==0)
		{
		 preprocessor_off = true;
		}
	else if(strcmp(argument,"-enable-clock-based-simplification")==0)
		{
		 clock_based_simplification_enabled = true;
		}
	else if(strcmp(argument,"-tr-needed-in-file")==0)
		{
		 TRNeeedInFile = true;
		}
	else if(strcmp(argument,"-exit_after_tr_generation")==0)
		{
		 exit_after_tr_generation = true;
		}	
 	else 
		{
		string str_arg = argument;
		int index = str_arg.find_last_of("=");
      		if(index!=string::npos)
		  {
		    if(str_arg.substr(0, index)=="-simulation-cycles")
			{
			string times_str = str_arg.substr(index+1);	
			no_of_simulation_cycles = atoi(times_str.c_str());
			}
		    if(str_arg.substr(0, index)=="-unrollings")
			{
			string times_str = str_arg.substr(index+1);	
			TimesOfUnrolling = atoi(times_str.c_str());
			}
                    else if(str_arg.substr(0, index)=="-solver")
			{
			solver_for_bmc = str_arg.substr(index+1);	
			}
		    else if(str_arg.substr(0, index)=="-quantifier-eliminator")
			{
			quantifier_eliminator = str_arg.substr(index+1);	
			}
		    else if(str_arg.substr(0, index)=="-solver-timeout")
			{
			solver_timeout_seconds = str_arg.substr(index+1);
			}
		    else if(str_arg.substr(0, index)=="-cegar-iterations")
		        {
			string no_of_cegar_iterations_str = str_arg.substr(index+1);
			no_of_cegar_iterations = atoi(no_of_cegar_iterations_str.c_str());
			}
		    else
			{
			usage();
			exit(1);
			}
		  }
      	
		else
			{
			usage();
                	exit(1);
			}
		}
	i++;
	}

propertyfile=strdup(argv[i]);

i++;

infilename=strdup(argv[i]);
original_infilename = infilename;

}


int main(int argc, char** argv)
{

  //time_t program_start_time;
        time_t t2;
        time(&program_start_time);
	gettimeofday (&program_start_time_ms, NULL);

 	char* outfilename;

	// flushing the log file

	
	log_file = fopen("log.dat", "w+");

	if(log_file == NULL) {cout<<"\nCannot open file log.dat\n"; exit(1);}

	fclose(log_file);

	// flushing the log file ends here


	

// 	int i=7;

  /* missing parameter check */
    
  /* yydebug=1; */


  /* open files */
  /* input file */

// 	debug_mode = atoi(argv[1]);

// 	cout<<"\nDebug Mode"<<debug_mode<<"\n"; // 0 - Debug mode, 1- Real mode

// 	no_of_simulation_cycles = atoi(argv[2]);

// 	qelimination_on = atoi(argv[3]);

// 	cout<<"\nQuantifier Elimination"<<qelimination_on<<"\n"; // 0 - Quantifier Elimination switched off , 1- Quantifier Elimination switched on
	
// 	TimesOfUnrolling = atoi(argv[4]);

// 	cout<<"\nTimesOfUnrolling = "<<TimesOfUnrolling<<"\n"; // 0 - Quantifier Elimination switched off , 1- Quantifier Elimination switched on

// 	Slicing = atoi(argv[5]);

// 	cout<<"\nSlicing = "<<Slicing<<"\n"; // 0 - Slicing switched off , 1- Slicing switched on


// 	// getting the property file
	
// 	propertyfile = strdup(argv[6]);
	
// 	cout<<"\nProperty file = "<<propertyfile<<"\n"; 

	
	
	
// 	CPrePostProcessor::readEntArchMap(mapfilename, EntArch);

	/*outfilename = strdup(argv[argc-1]);

	if ((yyout=fopen(outfilename, "w"))==NULL)
	{
		yyerror("could not open outputfile\n");
		} */


	readArguments(argc, argv);

	// Let's create the TR file name
	string infilename_str(original_infilename);
	string infilename_str_without_extension;
	int index_of_dot = infilename_str.find_last_of(".");
	if(index_of_dot == string::npos)
	{
		infilename_str_without_extension = infilename_str;
	}
	else
	{
		infilename_str_without_extension = infilename_str.substr(0, index_of_dot);
	}
	TRFileName = infilename_str_without_extension;
	TRFileName += ".smt";

        #ifdef DEBUG
	showOptions();
	#endif

	if(!preprocessor_off)
	{
		cout<<"\nPreprocessing VHDL file ...";

		string command = "SymSimuPreProcess ";
		string infilename_str(infilename);
		command += infilename_str;
		system(command.c_str()); 

		cout<<"\nPreprocessing of VHDL file successful";

		infilename = "preprocessed.vhd";
	}	
	

	yyout=stdout;

	cout<<"\nParsing VHDL file ...";

	if(logging_enabled)
	  {
	    writeToLogFile("\n\nParsing of VHDL File Started. Please Wait...\n\n");
	  }

	if ((yyin=fopen(infilename, "r"))==NULL)
	  {
	    yyerror("unable to open inputfile");
	    exit(1);
	  }    
	
	yyparse();


	#ifdef DEBUG
	cout<<"\n\nPrinting SymbolTable\n\n";
	#endif
	
	#ifdef DEBUG
	CPrePostProcessor::printSymbolTable();
	#endif

	#ifdef DEBUG
	cout<<"\n\nSymbolTable Printed\n\n";
	#endif

	#ifdef DEBUG
	cout<<"\n\nPostprocessing Started\n\n";
	#endif
	
	CPrePostProcessor::postprocess();

	#ifdef DEBUG
	cout<<"\n\nPostprocessing Successful\n\n";
	#endif

	cout<<"\nParsing of VHDL file successful";

	if(logging_enabled)
	  {
	    writeToLogFile("\n\nParsing of VHDL File Successful\n\n");
	  }



	CSymbSimulator SS;
	
	cout<<"\nSymbolically simulating the design...";
	fflush(stdout);

	if(logging_enabled)
	  {
	    writeToLogFile("\n\nSymbolic Simulation Started. Please Wait...\n\n");
	  }


	SS.execute();

	//cout<<"\n\nSymbolic Simulation Successful\n\n";

	if(logging_enabled)
	  {
	    //writeToLogFile("\n\nSymbolic Simulation Successful\n\n");
	  }



//   /* open files */
//   /* input file */
//   if (argc>1){
//     infilename=(char*)malloc(sizeof(char)*strlen(argv[1])+1);
//     strcpy(infilename, argv[1]);
//     if ((yyin=fopen(argv[1],"r"))==NULL){
//       yyerror("unable to open inputfile");
//     }    
//     argc--;
//     argv++;
//   }else{
//     infilename=" ";
//   }
//   /* output file */
//   if (argc>1){
//     if ((yyout=fopen(argv[1],"w"))==NULL){
//       yyerror("could not open outputfile\n");
//     }
//   }
// 
//   yyparse();

	time(&t2);

	#ifdef DEBUG
	cout<<"\n\nEXECUTION TIME  = "<< (t2 - program_start_time) <<" SECONDS "<<endl<<endl;
	#endif

       gettimeofday(&program_end_time_ms, NULL);
       program_duration_ms = program_end_time_ms.tv_sec * 1000 + program_end_time_ms.tv_usec / 1000;
       program_duration_ms -= program_start_time_ms.tv_sec * 1000 + program_start_time_ms.tv_usec / 1000;
         

	#ifdef DEBUG
	printTimeData();
	#endif

	if(!exit_after_tr_generation && !exit_after_vcgeneration && !refinement_on) // if CEGAR loop is ON, then answer comes out from the CEGAR loop
        {
		if(!exit_after_verification)
		  {
		    //system("yices -tc yices_input.ys");
		    CallYices();
		    time(&t2);

		    #ifdef DEBUG
		    cout<<"\n\nEXECUTION TIME + YICES CALL TIME = "<< (t2 - program_start_time) <<" SECONDS "<<endl<<endl;
		    #endif
		  }
		else
		  {
		  	CallYicesAndStopAfterVCDFileGeneration();
		  }
	}

	return 0;
}
