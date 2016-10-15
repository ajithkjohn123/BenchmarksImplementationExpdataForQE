#ifndef __CSYMBSIMULATOR_H
#define __CSYMBSIMULATOR_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>


#include "CDependencyGraph.h"
#include "CHelper.h"

#include "ConditionsValues.h"
#include "ConditionRelation.h"

#include "ReplaceNode.h"

#include "graph-bb.h"

#include "time.h" // newly added to find the execution times

#include <sys/time.h>

#include "CFactorGraphNode.h" // newly added for scope reduction

#include "DagManager.h"
#include "Eliminator.h"
#include "DDManager.h" // newly added for quantifier elimination
#include "DAGEliminator.h"


//#include "Oprs.h" // newly added on 17th Sep. 09 for type information

using namespace std;


extern map <string, map<string, SymTabEntry> > SymbolTable;

extern vector<CEntity> Entities;

//extern map <string, map<string, ConditionsValues> > SymbolicValues;

class CDependencyGraph;


struct compare_pair
{
	  bool operator()(pair<int, int> r1, pair<int, int> r2) const
	  {
		return r1.first < r2.first;
	  }
};


class AffBlocksAndDelta
{
public:
	int delta;
	CBasicBlock* bb;
	CDAGNode * condition;
	set<string> setCondition;
	
	AffBlocksAndDelta(int delta1, CBasicBlock* bb1, CDAGNode * condition1, set<string> &p_setCondition): delta(delta1)
																		, bb(bb1), 
																		condition(condition1)
	{
		setCondition.insert(p_setCondition.begin(), p_setCondition.end());
	}
	
   void displayAffBlocksAndDelta()
	  {
	    cout<<"\nDelta="<<delta;
	    cout<<"\nBB is\n";
	    bb->display();
	    cout<<"\ncondition is :";
	    if(condition != NULL) {cout<<condition->getDAGStr()<<endl;}
	    else {cout<<"condition is NULL\n";}
	  }
	    
};

class AffBlocksAndDeltaWithInput
{
public:
	int delta;
	CSensitiveElement* input;
	CBasicBlock* bb;
	
		
	AffBlocksAndDeltaWithInput(int delta1, CSensitiveElement* input1, CBasicBlock* bb1)
	{
	  delta = delta1;
	  input = input1;
	  bb = bb1;
	}
	
   void displayAffBlocksAndDeltaWithInput()
	  {
	    cout<<"\nDelta="<<delta;
	    cout<<"\nInput is\t";
	    input->printCSensitiveElement();
	    cout<<"\nBB is\n";
	    bb->display();
	    cout<<"\n\n";
	 }
	    
};

class RangeConstraint
{
public:
	pair<int, int> range;
	CDAGNode * constraint;	
	set<string> setConstraint;
	RangeConstraint(int lower, int upper, CDAGNode * p_constraint, set<string>& p_setConstraint)
	{
		range.first = lower;
		range.second = upper;
		constraint = p_constraint;
		setConstraint.insert(p_setConstraint.begin(), p_setConstraint.end());
	}
};

class FinalRelation
{
public:	
	CDAGNode * final_relation;	
	set<string> setFinalRelation;
	vector<pair<int, int> > ranges_covered;
	
	FinalRelation(CDAGNode * p_final_relation, set<string> &p_finalRelation, 
					vector<pair<int, int> > p_ranges_covered)
	{
		final_relation = p_final_relation;
		setFinalRelation.insert(p_finalRelation.begin(), p_finalRelation.end());
		ranges_covered = p_ranges_covered;
	}
};

class FinalRelationWithDelta
{
public:	
        int delta;
	CDAGNode * final_relation;	
	set<string> setFinalRelation;
	vector<pair<int, int> > ranges_covered;
	
	FinalRelationWithDelta(int p_delta, CDAGNode * p_final_relation, set<string> &p_finalRelation, 
					vector<pair<int, int> > p_ranges_covered)
	{
	        delta = p_delta;
		final_relation = p_final_relation;
		setFinalRelation.insert(p_finalRelation.begin(), p_finalRelation.end());
		ranges_covered = p_ranges_covered;
	}
};


class CSymbSimulator
{
	void createNextSignal(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, string  identifier,
							map<string, list<CSenNode*> > &affecting, 
							map<node *, CBasicBlock* > &nodePartOfBasicBlock);
	//void createNextSignal(string id_name, ConditionsValues &rv, map<string, list<CSenNode*> > &affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock);
	//set<CBasicBlock*> affectedBasicBlocks;
	list<AffBlocksAndDelta*> affectedBasicBlocks;
	list<AffBlocksAndDeltaWithInput*> affectedBasicBlocksForOverall; // newly added

	void displayAffectedBB()
	  {
	    list<AffBlocksAndDelta*>::iterator it = affectedBasicBlocks.begin();
	    cout<<"\nDisplaying affectedBasicBlocks\n"; 
	    for(;it != affectedBasicBlocks.end();it++)
	      {
		cout<<"\nAffected Basic Block \n";
	      (*it)->displayAffBlocksAndDelta();
	      }
	  }
	
	void advanceDeltaTime(CEntity& ent);
	
	void getIdentifiers(set<CSensitiveElement*> &identifiers, 
										set<CSensitiveElement*> &input_identifiers, 
										set<CSensitiveElement*> &output_identifiers, 
										set<CSensitiveElement*> &intermediate_identifiers,CEntity& ent);
	void insertToAffectedBB(int delta, CBasicBlock* bb, CDAGNode* condition, set<string> &setCondition);
	void insertToSymbolicQueue(int delta_time, CDAGNode* relation_shifted, CDAGNode* condition, set<string>& setCondition, ConditionsValues& cv, vector<pair<int, int> > ranges_covered );
	CDAGNode * calculateEventCondition(int delta_time, ConditionsValues& cv, CSensitiveElement* se, set<string>& setCondition);
	
	void printTableBasicBlocks(CDependencyGraph* dg);
	
	void printTableOverall(CEntity& ent);
	
	void simplifyTable(CEntity& ent); // Newly Added

	void printSymbolicQueues(map<string, ConditionsValues>& SymbolicQueues);
	void createTableOverall(CEntity& ent, map<string, ConditionsValues> &SymbolicQueues, 
					set<CSensitiveElement*>& output_identifiers,
					set<CSensitiveElement*>& input_identifiers,
                                        set<CSensitiveElement*>& intermediate_identifiers,
					map<string, list<ConditionRelation*> >& mapSignalDelta,
					map<string, FinalRelationWithDelta*> & final_expressions);
					
	void adjustRefCount(list<int> &nodesWithZeroIndegree);
	void find_nodes_with_zero_indegree(list<int> &nodesWithZeroIndegree);
	void setRefCounts();
	void topologicalSorting(list<int> &sortedIndices);
	
	void CreateTableForProcesses(CEntity &ent);

	void InitializeRV(list<ProcessSExpressions*> &SymbolicExpressions, map<string, list<RangeConstraint*> > &mapRV, string entName);
	
	//void createConstraint(ProcessSExpressions* os, map<string, list<RangeConstraint*> > &mapRV, CDAGNode *dag, set<string>& setConstraint);
	void createConstraint(ProcessSExpressions* os, map<string, list<RangeConstraint*> > &mapRV, CDAGNode *dag, set<string>& setConstraint, string entName);

	//CDAGNode *createDAGforOS(ProcessSExpressions* os, int current, CEntity &ent, set<string>&);
	
	CSensitiveElement* getSenEleforInput(set<CSensitiveElement*>& input_identifiers, 
										string identifier);
	
	
	void getRemainingRanges(vector<pair<int, int> >& rel_ranges, vector<pair<int, int> >&remain_ranges, int last);
	
	void calculateExpressionsForBB(CBasicBlock *bb, map<string, ConditionsValues>& SymbolicQueues, 
								map<string, FinalRelationWithDelta*> & final_expressions, CEntity& ent);
								
	void calculateExpressionsForOverall(map<string, ConditionsValues>& SymbolicQueues, 
											map<string, list<ConditionRelation*> >& mapSignalDelta,
											map<string, FinalRelationWithDelta*> & final_expressions, CEntity& ent);
	
	bool is_output_input(string element, set<CSensitiveElement*> &output_identifiers, set<CSensitiveElement*> &input_identifiers);

	 bool is_intermediate(string element, set<CSensitiveElement*> &intermediate_identifiers);

	void getactualFormalRealtion(CInstance * instance_stmt, map<string, CExpression*>& actualFormal);
	
	void createTableForProcesses(map<string, list<RangeConstraint*> > &mapRV, CProcess *proc);
	
	//void delete_and_add(ReplaceNode* rn);
	
	//void replace(CDAGNode* dag_node, map<string, CExpression*> &actualFormal, set<string> &setRelation, CEntity &ent, list<ReplaceNode*> &replaceNodes);
	
	void replace(CDAGNode* dag_node, map<string, CExpression*> &actualFormal, set<string> &setRelation, CEntity &ent, map<string, CDAGNode*> &replaceNodes);
	
	string getListString(set<string> &setExQuantify); 
	
	void createNonExistingConstraints(map<string, ConditionsValues> &SymbolicQueues, map<string, FinalRelation*> & final_expressions, int max_input_time, CEntity &ent);
	
		
public:
	vector<CDependencyGraph*> AllDepenGraphs;
	void createDependencyGraphs();
	//void PerformSymbolicSimulation(list<node *> &sortedNodes, CEntity &ent, int current, int previous);
	
	void PerformSymbolicSimulation(CBasicBlock * bb, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues);
	
	
	//list<int>   PerformSymbolicSimulationConAssign(CBasicBlock * bb, CConSignAssgnStmt * conc_assign_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, list<int> &deltas);
	//list<int>   PerformSymbolicSimulationProcess(CBasicBlock * bb, CProcess * process_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, list<int> &deltas);
	//list<int>   PerformSymbolicSimulationCompInstant(CBasicBlock * bb, CInstance * instance_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, list<int> &deltas);

	map<int, list<string> >   PerformSymbolicSimulationConAssign(CBasicBlock * bb, CConSignAssgnStmt * conc_assign_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, map<int, list<string> > &deltas);
	map<int, list<string> >   PerformSymbolicSimulationProcess(CBasicBlock * bb, CProcess * process_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, map<int, list<string> > &deltas);
	map<int, list<string> >   PerformSymbolicSimulationCompInstant(CBasicBlock * bb, CInstance * instance_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, map<int, list<string> > &deltas);
	
	void assignToSymbolicValues(COperand *lhs, CDAGNode *final_expression, CEntity &ent, int current);
	vector<int>	findAtomsOfLHS(vector< pair<int,int> > &atom_ranges, int lower, int upper);
	
	void PerformCyclicSymbolicSimulation(list<CBasicBlock*> &basic_back, CEntity &ent);
	
	void createValuesAtOneTime(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, set<CSensitiveElement*> &identifiers, 
					map<string, list<CSenNode*> > &affecting, 
					map<node *, CBasicBlock* > &nodePartOfBasicBlock);	
	
	void createValuesAtZeroTime(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, set<CSensitiveElement*> &identifiers);
	
	//void createValuesAtZeroTime(CEntity& ent, map<string, list<int> >&atoms);
	
	
	void formSymbolicStructureWithAtoms(string EntName, string id_name, list<int> &atoms);
	
	void execute();
	
	CDAGNode * calculateConditions(CBasicBlock *bb, int current);
	
	void printSymbolicValues(CEntity& ent, int current);
	
	CSymbSimulator()
	{
		affectedBasicBlocks.clear();
	}
	
	void PerformSymbolicSimulationOfBasicBlocks (CDependencyGraph* dg, CEntity& ent,
									map<string, list<CSenNode*> > &affecting, 
									map<node *, CBasicBlock* > &nodePartOfBasicBlock);
									
	void createTableForBB(CBasicBlock* bb, map<string, ConditionsValues> &SymbolicQueues, map<string, FinalRelationWithDelta*>& final_expressions, map<string, list<CSenNode*> > affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock);
	
	void PerformSymbolicSimulationOverall(CDependencyGraph* dg, CEntity& ent,
					map<string, list<CSenNode*> > &affecting, 
					map<node *, CBasicBlock* > &nodePartOfBasicBlock);
					
	//void PerformSymbolicSimulationBB(AffBlocksAndDelta* abd, CEntity& ent, map<string, ConditionsValues> &SymbolicQueues, map<string, list<ConditionRelation*> >&mapSignalDelta);

	void PerformSymbolicSimulationBB(AffBlocksAndDeltaWithInput* abd, CEntity& ent, map<string, ConditionsValues> &SymbolicQueues, map<string, list<ConditionRelation*> >& mapSignalDelta, map<string, list<CSenNode*> > &affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock, Graph_BB &BBTriggeringGraph);
	
	// DECLARATIONS OF MODIFIED FUNCTIONS TO PERFORM INLINING


	void convertIteToAnds(list<CDAGNode*> &node_list, CDAGNode *root);
	CDAGNode* convertIteToFunction(CDAGNode *root);
	CDAGNode* Inlining(CDAGNode *root, set<string> setExQuantify);
	  
	// DECLARATIONS OF MODIFIED FUNCTIONS TO PERFORM INLINING TERMINATED
	  

	// DECLARATIONS OF FUNCTIONS TO PERFORM INLINING

	void call_findPlacesOfInlining(set<string> setExQuantify,CDAGNode *node);
	void findPlacesOfInlining(string currExistQuantiVar,CDAGNode* node,list<CDAGNode*>& listOfPlacesToBeReplaced,list<CDAGNode*>& adjustAndWhere,list<CDAGNode*>& adjustAndBy,list<CDAGNode*>& replaceBy,bool& valAssignedToRelaceBy);
	void additionalReplacePlaces(CDAGNode *node,string currExistQuantiVar,list<CDAGNode*>& listOfPlacesToBeReplaced);
	void remainingReplacePlaces(CDAGNode *node,string currExistQuantiVar,list<CDAGNode*>& listOfPlacesToBeReplaced);

	// DECLARATIONS OF FUNCTIONS TO PERFORM INLINING TERMINATED

	
	list<node *> arrangeInTriggeredOrder(list<node *> originalNodes);
	bool Triggering(node *first, node *second);
	bool presentInmapSignalDelta(map<string, list<ConditionRelation*> > mapSignalDelta, string target);

	//void removeTransientsFromTable(CEntity& ent);
	bool isUnsat(CDAGNode* spec, string EntName);
	string removeParentheses(string target);


	void check_for_cycles(CEntity &ent);
	void createArgDepGraph(Graph *ArgDepGraph, CEntity &ent);
	void createBlockDepGraph(Graph *BlockDepGraph, CEntity &ent);
	void createEdgeInDepGraphForInstances(list<CInstance *> &instanceNodes, string EntName, Graph *ArgDepGraph);
	CPort& getPort(list<CPort> & ports, string name);
	void InitializeBlockSCC(set<string> &BlockSCC, CEntity &ent);
	void printSCCs(set<string> Components);
	void initializeBBTriggeringGraph(Graph_BB &BBTriggeringGraph, list<CBasicBlock*> BasicBlocks);


	CDAGNode* getTriggeringCondition(set<CSensitiveElement*> input_identifiers, set<string> &prev_setCondition, int time);
	CDAGNode* getPreviousIdDelta(CDAGNode *current);
	
	void initializeDeltaMapForBB(map<CSensitiveElement*, string> &delays, CBasicBlock* bb, map<string, list<CSenNode*> > affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock);
	void initializeDeltaMapForEntity (map<CSensitiveElement*, string> &delays, set<CSensitiveElement*> inputIdentifiers);
	void createValuesAtOneTimeForOverall(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, set<CSensitiveElement*> &input_identifiers, map<string, list<CSenNode*> > &affecting, map<node *, CBasicBlock* > &nodePartOfBasicBlock, Graph_BB &BBTriggeringGraph);
	void createNextSignalForOverall(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, string  identifier,
					map<string, list<CSenNode*> > &affecting,
					map<node *, CBasicBlock* > &nodePartOfBasicBlock, Graph_BB &BBTriggeringGraph);
	void insertToAffectedBBForOverall(int delta, CSensitiveElement* input, CBasicBlock* bb, CBasicBlock* source, Graph_BB &BBTriggeringGraph);
	CDAGNode* getTriggeringConditionForOverall(map<CSensitiveElement*, string> delay_map, set<string> &prev_setCondition, CSensitiveElement* Changed_Input, int time);
	int findDeltaTimeOfInput(map<CSensitiveElement*, string> delay_map, CSensitiveElement* Input);
	int isPresentInDeltaMap(map<CSensitiveElement*, string> delay_map, CSensitiveElement* Input);
	void updateMapSignalDelta(map<string, list<ConditionRelation*> >& mapSignalDelta, ConditionRelation *cr, string output_identifier, int present_delta_time, string output_identifier_with_delta);
	void initializeDeltasForBBSimulation(CBasicBlock* bb, map<int, list<string> > &deltas);
	void showDeltasForBBSimulation(map<int, list<string> > deltas);
	void getInputsAtSmallestDelta(map<int, list<string> > &deltas, int &current, list<string> &InputsAtCurrent);
	int findDeltaTimeOfInputString(map<CSensitiveElement*, string> delay_map, string Input);
	string findFormalTriggeringInput(map<string, CExpression*> actualFormal, string Actual_TriggeringInput, CEntity &ent);
	CDAGNode* addAdditionalConstraints(CDAGNode *final_dag, set<CSensitiveElement*> input_identifiers);
	bool findElementWithLargerOrSameDelta(set<string> setDone, string new_element);
	void addElementToSetDone(set<string> &setDone, string new_element);

	// DECLARATIONS OF FUNCTIONS TO PERFORM SCOPE REDUCTION

	void listOfFunctionNodesForFactorGraph(CDAGNode* node,list<CDAGNode*>& listOfFunctionNodes);
	void listOfVariables(CDAGNode* node,set<string>& setOfVariables);
	CDAGNode* scopeReduction(set<string> setExQuantify,CDAGNode* node);
	void scopeReduction2(set<string> setExQuantify,CDAGNode* node);
	void scopeReduction3(set<string> setExQuantify,CDAGNode* node);

	// END OF DECLARATIONS OF FUNCTIONS TO PERFORM SCOPE REDUCTION

	CDAGNode* getDAGFromStringSet(set<string> &setExQuantify);

	// DECLARATIONS OF FUNCTIONS AT INTERFACE TO EXPRESSION SIMPLIFIER

	struct node_info* convertTreeToDAG(CDAGNode *tree);
	CExprSimpNode* convertCDAGNodeToCExprSimpNode(CDAGNode *tree);
	CDAGNode* convertCExprSimpNodeToCDAGNode(CExprSimpNode *dag);
	int findSizeOfDAG(struct CExprSimpNode* x);
	CDAGNode* callInlining(CDAGNode *dag);
	CDAGNode* callScopeReduction(CDAGNode *dag);
	CDAGNode* cloneAndShiftWithInlining(CDAGNode* parent, CDAGNode *me);
	CDAGNode* cloneAndShiftWithScopeReduction(CDAGNode* parent, CDAGNode *me);
	CDAGNode* simplifyExpression(CDAGNode *dag);
	CDAGNode* equivalentReduction(CDAGNode *dag, string OutputName);
	int findSizeOfTreeAndPrintTreeInDAGForm(CDAGNode* tree, string name, string Where, string OutputName);
	CDAGNode* renameExVariablesInTree(CDAGNode *relation, string entity, string instance);
	CDAGNode* cloneAndShiftWithRename(CDAGNode* parent, CDAGNode* me, map<string,string> RenameMap);
	void exQuantifierDepths(CExprSimpNode *dag);
	void deleteDAG(struct CExprSimpNode* dag); 
	void deleteTree(CDAGNode* tree);
	CDAGNode* completeInlining(CDAGNode* parent, CDAGNode *me);
	void findSizeOfTree(CDAGNode* tree, string DebugFile, string message);
	// DECLARATIONS OF FUNCTIONS AT INTERFACE TO EXPRESSION SIMPLIFIER ENDS
 
	// DECLARATIONS OF FUNCTIONS AT INTERFACE TO FOR PROFIT FINDING
	void findProfits(CDAGNode* tree, string OutputName);
	CDAGNode* abstractionOnAnd(CDAGNode* tree);
	void exQuantifierDepthsScopes(CExprSimpNode *dag);
	void exQuantifierTotalScopes(CExprSimpNode *dag);
	void printProfits(CDAGNode* tree);
	bool andAbstractionFeasible(CDAGNode* tree);
	CDAGNode* splitPresentExists(CDAGNode* exp, set<string> &setExQuantifyTemp);
	bool andAbstractionFeasibleModified(CDAGNode* tree);
	CDAGNode* findAndAbstractionFeasibleNode(CDAGNode* tree);
	CDAGNode* abstractionOnAndWithExistsPart(CDAGNode *TreePart, CDAGNode *ExistsPart);
	CDAGNode* abstractionOnAndModified(CDAGNode *tree);
	void getAbstractedExpression(CDAGNode *dag, string OutputName);
	void getDepthsOfExitsNodes(CDAGNode *tree, list<int> &depths);
	void getDepthsOfExitsNodesBeforeAbstraction(CDAGNode *tree, list<int> &depths);
	CDAGNode* scopeReduction_Original_On18May09(set<string> setExQuantify,CDAGNode* node);  
	  
	// DECLARATIONS OF FUNCTIONS AT INTERFACE TO FOR PROFIT FINDING ENDS

	bool checkIfTriggeringConditionIsTrue(CBasicBlock *bb);
	bool listEqual(list<COperand*> InputList, list<COperand*> SensitivityList);

	
	// DECLARATIONS OF FUNCTIONS NEEDED FOR COMBINING WITH QELIMINATOR

	CDAGNode* createCloneWithInternalVarsRenamed(CDAGNode *relation, string EntName, CInstance * instance_stmt) ;
	void showSet(set<string> me, string WhoAmI);
	void VCGenerator(CEntity &ent);
	void getSEsOfAllOutputsInASet(set<CDAGNode*> &SEsOfAllOutputs, set<string> &Outputs, CEntity &ent);
	CDAGNode* createTransitionRelation(set<CDAGNode*> SEsOfAllOutputs, set<string> Outputs, CEntity &ent, map<string, string> &ReplacedVarsMap, int &X_name_count);
	int getMaximum(set<int> MySet);
	void getNamesFromCSensitiveElementSet(set<CSensitiveElement*> identifiers, set<string> &id_names);
	CDAGNode* createTransitionRelationForGivenOutput(CDAGNode* SE, string output, CEntity &ent, map<string, string> &ReplacedVarsMap, int &X_name_count);
	void showMap(map<string, int> me, string WhoAmI);
	void writeInYicesFile(CDAGNode* root, FILE *fp, string TopLevelEntity, map<string, string> ReplacedVarsMap);
	void writeBVDeclarationsInYicesFormat(CDAGNode* root, FILE *smt_file, string TopLevelEntity, map<string, string> ReplacedVarsMap);

	CDAGNode* convertExpressionToProperFormula(CDAGNode *dag, string RuleFile);
	CDAGNode* convertExpressionWithExistsToProperFormula(CDAGNode *dag);
	CDAGNode* convertExpressionToProperFormulaComplete(CDAGNode *dag, string RuleFile);
	int getWidthOfVariable(string variable, string TopLevelEntity);
	void getWidthTable(CDAGNode* dag, string TopLevelEntity, map<string, int> &WidthTable);



	// DECLARATIONS OF FUNCTIONS NEEDED FOR COMBINING COMPARING RESULTS WITH AND WITH OUT QELIMINATION

	void getWidthTableForFinalUnrolledSE(CDAGNode* root, string TopLevelEntity, map<string, string> ReplacedVarsMap, map<string, int> &WidthTable);
	void writeBVDeclarationsInYicesFormatModified(FILE *smt_file, map<string, int> WidthTable);
	void writeInYicesFileModified(CDAGNode* root, FILE *fp, string TopLevelEntity, map<string, string> ReplacedVarsMap);


	CDAGNode* prepareDAGForOutputtingInYices(CDAGNode *root, string TopLevelEntity, map<string, string> ReplacedVarsMap);
	void UnrollTR_and_Print_in_Yices_Format(CDAGNode* root, FILE *fp, string TopLevelEntity, map<string, string> ReplacedVarsMap, string property);
	void getWidthTableForUnrolledTR(map<string, int> WidthTable_TR, map<string, int> &WidthTable);
	
	void get_inter_id_names_from_lower_levels(CDAGNode *SE, set<string> input_id_names, set<string> output_inter_id_names, set<string> &inter_id_names_from_lower_levels);
	void attachSet(set<string> &destination, set<string> source);

	CDAGNode* removeUnneededSelects(CDAGNode *dag, map<string, int> WidthTable);
	CDAGNode* simplifyFormulaWithExists(CDAGNode *dag, string RuleFile);


	// FUNCTION TO READ PROPERTY

	string readProperty();


	/*******Newly added functions on 25.04.2010 to add slicing ********************************/

	int VCGenerator_With_Slicing(CEntity &ent, list<int> sortedIndices);
	void getTransitionRelationMap(map<string, CDAGNode*> &SEsOfAllOutputs, CEntity &ent, map<string, string> &ReplacedVarsMap, int &X_name_count, map<string, CDAGNode*> &TRMAP);
	void getSEsOfAllOutputsInAMap(map<string, CDAGNode*> &SEsOfAllOutputs, CEntity &ent);
	void getInterestedOPs(set<string> &InterestedOPs);
	void getFixPoint(map<string, set<string> > &DepMap, string output, set<string> &FixPoint);
	void getImage(map<string, set<string> > &DepMap, set<string> &FixPoint, set<string> &NewState);
	bool setsEqual(set<string> &FixPoint, set<string> &NewFixPoint);
	void obtainHierarchy(vector<string> &hierarchy, CEntity &ent, string prefix);
	void getInstanceNameAndSignalName(string &NeededInstance, string &NeededSignal, string signal);
	void createInstanceFunctionMap(map<string, map<string, CDAGNode*> > &InstanceFunctionMap, vector<string> &hierarchy, CDAGNode *TR, CEntity &ent);
	CDAGNode* eliminateTemporaryVariablesOfTRCreation(CDAGNode *TR);
	CDAGNode* eliminateTrivialEqualities(CDAGNode *TR);
	void findInterestedSEsOfInstance(map<string, CDAGNode*> &InterestedSEsOfInstance, string InstanceName, set<string> InterestedSet, map<string, map<string, CDAGNode*> > &InstanceFunctionMap);
	void createFrames_and_Print_in_Yices_Format_Modified(vector<string> &hierarchy, map<int, set<string> > &FRAMEMAP, map<string, map<string, CDAGNode*> > &InstanceFunctionMap, CEntity &ent, string property, list<int> sortedIndices, set<string> &PropertySupport);
	CDAGNode* applyQuantifierElimination(CDAGNode *root, set<string> &VarsToElim, CEntity &ent, int frame_number);
	void getEntityHandler(CEntity &MyEntity, list<int> sortedIndices, string EntityName);
	void chooseVarsToElim(set<string> &VarsToElim, string InstanceName, CEntity &ent, list<int> sortedIndices, CDAGNode *dag, int FrameNumber);
	void insertEntryIntoHTAcrossFrames(string InstanceName, set<string> VarsToElim, CDAGNode *OutputDag, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames);
	CDAGNode* getEntryFromHTAcrossFrames(string InstanceName, set<string> VarsToElim, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames);
	void getWidthTableWithDP(CDAGNode* dag, string TopLevelEntity, map<string, int> &WidthTable);
	void getFixPointOfSet(map<string, set<string> > &DepMap, set<string> output, set<string> &FixPoint);
	void getSimplificationTable(map<string, string> &SimplificationTable);

	void insertEntryIntoHTAcrossFramesModified(CDAGNode *SEOfInstance, set<string> VarsToElim, CDAGNode *OutputDag, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames);
	CDAGNode* getEntryFromHTAcrossFramesModified(CDAGNode *SEOfInstance, set<string> VarsToElim, HashTable <string, CDAGNode*> *Ptr_HTAcrossFrames);
	void showWidthTable(map<string, int> &WidthTable);
	void showSimplificationTable(map<string, string> &SimplificationTable);
	void getUnquantifiedSignals(set<string> &UnquantifiedSignals);
	
	/* Function added on 14 June 2010 for helping in printing the transition relation in SMTLib format */
	void getWidthTableForTR(CDAGNode* TR, string TopLevelEntity, map<string, int> &WidthTableTR);
	
	/* Function added on 7 July 2010 for integrating with property translator and GUI */
	void  callPropertyTranslator(CEntity &ent);


	void getDeltaTR(set<CDAGNode*> &SEsOfAllOutputs, CEntity &ent);


	// Added on 12-1-2011

	void insertEntryIntoBlast_Table(CDAGNode *SEOfInstance, set<string> VarsToElim, int frame, map<string, list<int> > *Ptr_Blast_Table);
       	void appendEntryIntoBlast_Table(CDAGNode *SEOfInstance, set<string> VarsToElim, int frame, map<string, list<int> > *Ptr_Blast_Table);
	void readBlast_Table_Hits_Misses(set<int> &Blast_Table_Hits, set<int> &Blast_Table_Misses);
	void parse_print_string(char *line_char, set<int> &Blast_Table_Hits, set<int> &Blast_Table_Misses);


	
	// Added on 31-1-2011 Onwards

	CDAGNode* generateConcreteVerificationCondition(vector<string> &hierarchy, map<int, set<string> > &FRAMEMAP, map<string, map<string, CDAGNode*> > &InstanceFunctionMap, CEntity &ent, string property, list<int> sortedIndices, map<string, int> &WidthTableForCVC, vector<CDAGNode*> &UnrolledTRs);
	void getModelFromFile(map<string, string> &Model);
	bool evaluateConcreteVerificationConditionAndFindSetOfSignalsTobeExposed(CDAGNode* CVC, vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<string, string> &Model, CEntity &ent, set<string> &SetOfSignalsTobeExposed);
	void refinement(set<string> &SetOfSignalsToBeExposed, CEntity &ent);
	void extendVariable_Value_Map(CDAGNode *CVC, map<string, string> &Model, map<string, int> &WidthTableForCVC, CEntity &ent);
	void initializeStateWiseModel(map<int, map<string, string> > &StateWiseModel, map<string, string> &Model);
	void showStateWiseModel(map<int, map<string, string> > &StateWiseModel);
	void getModelFromSimulation(CDAGNode *TR, map<string, string> &Model_i, map<string, string> &Model_i_p_1, map<string, string> &Model_From_Simulation, map<string, int> &WidthTableForCVC, CEntity &ent, int frame_number);
	void get_Mismatch_Between_Observed_And_Simulation_Models(map<string, string> &Observed_Model, map<string, string> &Model_From_Simulation, set<string> &MismatchedSignals, CEntity &ent);
	void updateStateWiseModel(map<int, map<string, string> > &StateWiseModel, map<string, string> &Model_From_Simulation, int mystate);
	void getVariable_Value_Map(map<string, string> &Variable_Value_Map, map<string, string> &Model_i, map<string, string> &Model_i_p_1);
	void findSetOfSignalsTobeExposed(CDAGNode *CVC, vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<int, map<string, string> > &StateWiseModel, CEntity &ent, int SpuriousState, set<string> &ProblematicSignalsAtSpuriousState, set<string> &SetOfSignalsTobeExposed);
	void findProblematicSignalsAtPreviousStateForSignal(CDAGNode *TR, map<string, string> &Model_i, map<string, string> &Model_i_p_1, string signal, map<string, int> &WidthTableForCVC, set<string> &ProblematicSignalsAtPreviousStateForSignal, CEntity &ent);
	void trimLeadingZeros(map<string, string> &Model);

	bool evaluateConcreteVerificationConditionAndFindSetOfSignalsTobeExposedPropertyGuided(CDAGNode* CVC, vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<string, string> &Model, CEntity &ent, set<string> &SetOfSignalsTobeExposed);

	void updateStateWiseModelBasedOnModel_From_Simulation(map<int, map<string, string> > &StateWiseModel, map<string, string> &Model_From_Simulation, int state);

	bool evaluateAndGetSupportOfUnsatCoreOfProperty(map<int, map<string, string> > &StateWiseModel, set<string> &SupportOfUnsatCore);

	void findSetOfSignalsTobeExposedPropertyGuided(vector<CDAGNode*> &UnrolledTRs, map<string, int> &WidthTableForCVC, map<int, map<string, string> > &StateWiseModel, CEntity &ent, set<string> &SupportOfUnsatCore, set<string> &SetOfSignalsTobeExposed);

	int findLastSpuriousState(set<string> &ProblematicSignals);

	void getProblematicSignals_i_p_1(set<string> &ProblematicSignals, int i_p_1, set<string> &ProblematicSignals_i_p_1);

	void findProblematicSignalsAtPreviousStateForSignalPropertyGuided(CDAGNode *TR, map<string, string> &Model_i, map<string, string> &Model_i_p_1, string signal, map<string, int> &WidthTableForCVC, set<string> &ProblematicSignalsAtPreviousStateForSignal, CEntity &ent);

	string evaluateAndGetSupportOfUnsatCoreOfGivenDAG(string signal, CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &SupportOfUnsatCore);

	void printStateWiseModel(map<int, map<string, string> > &StateWiseModel, string filename);

	void printVariable_Value_Map_Signal_Removed(map<string, string> &Variable_Value_Map_Signal_Removed, string filename);



	void createRenameMaps(CDAGNode *dag, map<string, string> &RenameMap, map<string, string> &ReverseRenameMap);
	void createRenamedWidthTable(map<string, int> &WidthTable, map<string, int> &RenamedWidthTable);

	unsigned long long int binaryStringToULINT(string str);

	string evaluateAndGetSupportOfUnsatCoreOfGivenDAGWithHashing(string signal, CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &SupportOfUnsatCore);

	string evaluateAndGetSupportOfUnsatCoreOfGivenDAGOperatingWithHashing(CDAGNode *node, CDAGNode *DisequationPart, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &SupportOfUnsatCore);

	void insertEntryInToUnsatCoreHashTable(CDAGNode *node_renamed, CDAGNode *DisEquationPart, map<string, string> &Variable_Value_Map_Renamed_Signal_Removed, set<string> &SupportOfUnsatCoreRenamed);

	bool checkIfEntryExistsInUnsatCoreHashTable(CDAGNode *node_renamed, CDAGNode *DisEquationPart, map<string, string> &Variable_Value_Map_Renamed_Signal_Removed, set<string> &SupportOfUnsatCoreRenamed);
	
	bool firstVMapImpliesSecondVMap(map<string, string> &FirstValueMap, map<string, string> &SecondValueMap);
	void getSupportOfVMap(map<string, string> &ValueMap, set<string> &SupportOfUnsatCoreRenamed);

	void showUnsatCoreFindingHashTable();

	void getPropertySupport(set<string> &PropertySupport);

	void getOrderOfEvaluationForSignals(list<string> &OrderOfEvaluationForSignals, map<string, CDAGNode*> &FunctionMap, CEntity &ent, int frame_number);
	void getLimitedSupportFromSupport(set<string> &Support, set<string> &input_id_names, string signal, int frame_number, set<string> &LimitedSupport);
	void eliminateAssignmentsToInternalSignals(map<string, string> &Model, CEntity &ent);
	void extendVariable_Value_Map_Modified(CDAGNode *CVC, map<string, string> &Model, map<string, int> &WidthTableForCVC, CEntity &ent);
	void getNewProblematicSignalsInCurrentState_ForSignal(set<string> &ProblematicSignals_i_ForSignal, int current, set<string> &NewProblematicSignalsInCurrentState_ForSignal);

	CDAGNode* callQualmode(CDAGNode *dag, map<string, int> &WidthTable);

	bool checkIfFileIsPresent(string file_to_check_for_presence);
	void deleteFileIfPresent(string file_to_check_for_presence);


	CDAGNode* Quantifier_Eliminator_With_Redundant_Selection_Removal(CDAGNode* formula_with_exists, map<string, int> &WidthTable);
	CDAGNode* Qualmode_Blasting(CDAGNode* formula_with_exists, map<string, int> &WidthTable, int frame_number);

	DAGNode* convertCDAGNodeToDAGNode(CDAGNode* LinearDag, map<string, int> &WidthTable);
	DAGNode* convertCDAGNodeToDAGNode_recur(CDAGNode* LinearDag, map<string, int> &WidthTable, map<CDAGNode*, DAGNode*> &DPTable);
	void openFilesBeforeQE();
	void closeFilesAfterQE();
	CDAGNode* convertDAGNodeToCDAGNode(DAGNode* dag, map<string, int> &WidthTable);
	CDAGNode* convertDAGNodeToCDAGNode_recur(DAGNode* dag, map<string, int> &WidthTable, map<DAGNode*, CDAGNode*> &DPTable);
	void printTRDetails(CEntity &ent, CDAGNode* transition_relation);

};


#endif
