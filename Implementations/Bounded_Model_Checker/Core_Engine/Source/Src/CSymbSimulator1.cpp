#include "CSymbSimulator.h"

//map <string, map<string, ConditionsValues> > SymbolicValues;


//CSymbSimulator::vector<CDependencyGraph*> AllDepenGraphs;

void CSymbSimulator::execute()
{
	list<int> sortedIndices;
	topologicalSorting(sortedIndices);
	sortedIndices.reverse();
	
	list<int>::iterator isi = sortedIndices.begin();
	
	for(; isi != sortedIndices.end(); isi++)
	{
		CEntity &ent = Entities[*isi];

		CDependencyGraph*  dg= new CDependencyGraph();
		
		dg->createDependencyGraph(ent);
		
//		AllDepenGraphs.push_back(dg);
		
//		dg->FindAtoms();
		
		//affectedBasicBlocks.clear();
		
		
		//TODO
		//dg->formBasicBlocks();
		dg->formBasicBlocks1();
		dg->printBB();
		
		map<string, list<CSenNode*> > &affecting = dg->getAllSenList();
		map<node *, CBasicBlock* > &nodePartOfBasicBlock = dg->GetNodePartOfBasicBlock();
		
		
		//TODO
		CreateTableForProcesses(ent);
		
		
		PerformSymbolicSimulationOfBasicBlocks(dg, ent, affecting,
											 nodePartOfBasicBlock);	
											 
		
		printTableBasicBlocks(dg);	
		
		
		
		
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
		
		
		affectedBasicBlocks.clear();
		PerformSymbolicSimulationOverall(dg, ent, affecting,
											 nodePartOfBasicBlock);	
											 
		 printTableOverall(ent);
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
	
	int totalNodes = Entities.size();
	
	list<int> nodesWithZeroIndegree;

	int size = nodesWithZeroIndegree.size();
	nodesWithZeroIndegree.clear();
	
	while(totalNodes != sortedIndices.size())
	{
		find_nodes_with_zero_indegree(nodesWithZeroIndegree);
		
		adjustRefCount(nodesWithZeroIndegree);

		sortedIndices.merge(nodesWithZeroIndegree);
		
		nodesWithZeroIndegree.clear();		
		cout<< "sortedNodes  "<<sortedIndices.size()<<endl;
	}
}

void CSymbSimulator::printTableOverall(CEntity& ent)
{
	map< string, list<RelationInputOutput*> >& table = ent.getTable();
	
	map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
	
	cout<<"**************************************"<<endl;
	for(;itab != table.end(); itab++)
	{
		cout<<"---------------------------"<<endl;
		string output_identifier = itab->first;
		list<RelationInputOutput*> & entries = itab->second;
		cout<< "ID::  " << output_identifier<<endl;
	
		for(list<RelationInputOutput*>::iterator irio = entries.begin();
			irio != entries.end();
			irio++)
		{
			RelationInputOutput* rio = *irio;
			cout<< "Time:  " << rio->delta_time<<endl;
			cout<< "Relation:  " << rio->relation->getDAGStr()<<endl;
		}
	}
	
	cout<< "constraints are :  " <<endl;
	list<CDAGNode*>::iterator icon = ent.constraints.begin();
	for(;icon != ent.constraints.end(); icon++)
	{
		cout<< (*icon)->getDAGStr()<<endl;
	}				
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
	cout<<"insertToAffectedBB"<<endl;
	list<AffBlocksAndDelta*>::iterator result;
	bool found = false;

	for(result = affectedBasicBlocks.begin(); result != affectedBasicBlocks.end(); result++)
	{	
		cout<<"*-*-*-*-*"<<endl;
		cout<< delta+1 << "  "<< (*result)->delta<<endl;
		cout<< bb << "  "<< (*result)->bb<<endl;
		cout<<"*-*-*-*-*"<<endl;
		
		if(delta+1 == (*result)->delta && bb == (*result)->bb)
		{
			if((*result)->condition->getDAGStr().find(condition->getDAGStr()) != std::npos)
			{
				(*result)->condition = CHelper::createDAG("or", condition, (*result)->condition);
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
		cout<<"*-------------*"<< endl;
		cout<<"adding to AffBlocksAndDelta"<< endl;
		cout<<condition->getDAGStr()<<endl;
		cout<<delta+1<<endl;
		cout<<bb<<endl;
		cout<<"*-------------*"<<endl;
		
		AffBlocksAndDelta *abd = new AffBlocksAndDelta(delta+1, bb, condition);
		affectedBasicBlocks.insert(result, abd);		
	}
}

CSensitiveElement* CSymbSimulator::getSenEleforInput(set<CSensitiveElement*>& input_identifiers, 
										string identifier)
{
	set<CSensitiveElement*>::iterator ise;
	for(ise = input_identifiers.begin(); ise != input_identifiers.end(); ise++)
	{
		if((*ise)->name == identifier)
		{
			return *ise;
		}
	}
}


void CSymbSimulator::createNextSignal(map<string, ConditionsValues> &SymbolicQueues, CEntity& ent, string  identifier,
					map<string, list<CSenNode*> > &affecting, 
					map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
	vector<CDAGNode*> values;
	vector<CDAGNode*> conditions;
	
	char *temp = strdup(identifier.c_str());
	
	SymbolicQueues[identifier].delta.push_back(1);
	SymbolicQueues[identifier].values.push_back(CHelper::createDAG(identifier, 1));
	SymbolicQueues[identifier].conditions.push_back(CHelper::createDAG("true"));
	
//	rv.values.push_back(values);
//	rv.conditions.push_back(conditions);
	
//	rv.values[1] = values;
//	rv.conditions[1] = conditions;
	
	cout<<affectedBasicBlocks.size()<<endl;
	

	if(affecting.find(identifier) != affecting.end())
	{
		
		list<CSenNode*> &sennodelist = affecting[identifier];
		
		cout<<"ID: "<<identifier<<" size = "<<sennodelist.size()<<endl;
		list<CSenNode*>::iterator ien;
		for (ien = sennodelist.begin(); ien != sennodelist.end(); ++ien) 
		{
			CBasicBlock* bb = nodePartOfBasicBlock[(*ien)->n];
			cout<<affectedBasicBlocks.size()<<endl;
			
			CSensitiveElement* se = getSenEleforInput(bb->get_Input_identifiers(), identifier);
			
			set<string> setCondition;

			CDAGNode* triggering_condition = calculateEventCondition(1, SymbolicQueues[identifier], se, setCondition);	
			
			insertToAffectedBB(1, bb, triggering_condition);
		
			cout<<affectedBasicBlocks.size()<<endl;
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
		
		cout<<"creating one delta at one time "<<(*ist)->name<<endl;

		createNextSignal(SymbolicQueues, ent, (*ist)->name, affecting, nodePartOfBasicBlock);
		
		
/*		ConditionsValues rv;
		
		vector< pair<int, int> > atom_ranges;
		vector<CDAGNode*> values;
		vector<CDAGNode*> conditions;
		
		CDAGNode*v = CHelper::createDAG(*ist);
		CDAGNode*c = CHelper::createDAG("true");
		
		cout<<v->getDAGStr()<<endl;
		cout<<c->getDAGStr()<<endl;
		
	
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
		values.push_back(CHelper::createDAG("select", CHelper::createDAG(id_name), start, end));
		conditions.push_back(CHelper::createDAG("true"));
		start = end+1;
	}
	
	irange++;
	
	int upper = atoi((*irange).c_str());;
	atom_ranges.push_back(make_pair(start, upper));
	values.push_back(CHelper::createDAG("select", CHelper::createDAG(id_name), start, upper));
	conditions.push_back(CHelper::createDAG("true"));
	
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
	set <CSensitiveElement*> ::iterator ist;
	
	for (ist=identifiers.begin(); ist != identifiers.end(); ++ist) 
	{
		ConditionsValues rv;
		

		vector<CDAGNode*> values;
		vector<CDAGNode*> conditions;
		
		char *t = strdup((*ist)->name.c_str());
		
		cout<< *ist <<endl;
		
		int lower = atoi((*ist)->lower.c_str());
		int upper = atoi((*ist)->upper.c_str());
		
		CDAGNode*v = CHelper::createDAG((*ist)->name, 0);
		CDAGNode*c = CHelper::createDAG("true");
		
		cout<<v->getDAGStr()<<endl;
		cout<<c->getDAGStr()<<endl;
		
	
		rv.delta.push_back(0);
		rv.values.push_back(v);
		rv.conditions.push_back(c);
		
		SymbolicQueues.insert(make_pair((*ist)->name, rv));
	}	
}

void CSymbSimulator::printTableBasicBlocks(CDependencyGraph* dg)
{
	list<CBasicBlock *> basicblocks = dg->getBasicBlocks();
	
	list<CBasicBlock *> ::iterator ibb = basicblocks.begin();
	
	map<string, bool> previously_changed;
	int i = 1;
	
	
	for(;ibb != basicblocks.end(); ibb++)
	{
		map< string, list<RelationInputOutput*> >& table = (*ibb)->getTable();
		
		map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
		
		cout<<"**************************************"<<endl;
		for(;itab != table.end(); itab++)
		{
			cout<<"---------------------------"<<endl;
			string output_identifier = itab->first;
			list<RelationInputOutput*> & entries = itab->second;
			cout<< "ID::  " << output_identifier<<endl;
		
			for(list<RelationInputOutput*>::iterator irio = entries.begin();
				irio != entries.end();
				irio++)
			{
				RelationInputOutput* rio = *irio;
				cout<< "Time:  " << rio->delta_time<<endl;
				cout<< "Relation:  " << rio->relation->getDAGStr()<<endl;
			}
			
			//cout<<"- - - - - - - - - - - - - - - - - "<<endl;
			
			//deltas_new.push_back((*irio)->delta_time + current);
		}		
	}	
}	


void CSymbSimulator::PerformSymbolicSimulationOfBasicBlocks (CDependencyGraph* dg, CEntity& ent,
					map<string, list<CSenNode*> > &affecting, 
					map<node *, CBasicBlock* > &nodePartOfBasicBlock)
{
	list<CBasicBlock *> basicblocks = dg->getBasicBlocks();
	
	list<CBasicBlock *> ::iterator ibb = basicblocks.begin();
	
	map<string, bool> previously_changed;
	int i = 1;
	
	
	for(;ibb != basicblocks.end(); ibb++)
	{
		map<string, ConditionsValues> SymbolicQueues;
		
		createValuesAtZeroTime(SymbolicQueues, ent, (*ibb)->get_identifiers());
		//createValuesAtZeroTime(SymbolicQueues, ent, (*ibb)->output_identifiers);
		
		createValuesAtOneTime(SymbolicQueues, ent, (*ibb)->get_Input_identifiers(), 
					affecting, nodePartOfBasicBlock);	
		
		PerformSymbolicSimulation(*ibb, ent, SymbolicQueues);
		
		createTableForBB(*ibb, SymbolicQueues);
	}
}	

void CSymbSimulator::getIdentifiers(set<CSensitiveElement*> &identifiers, 
									set<CSensitiveElement*> &input_identifiers, 
									set<CSensitiveElement*> &output_identifiers, 
									CEntity& ent)
{
	map<string, SymTabEntry> &idMap = SymbolTable[ent.getName()];
	
	map<string, SymTabEntry>::iterator iter = idMap.begin();
	
	for(; iter != idMap.end() ; iter++)
	{
		string name = iter->first;
		list<string>::iterator ir;
		ir = (iter->second).rangeindex.begin();
		string lower = *ir; 
		++ir;

		string upper = *ir;
		
		CSensitiveElement *se = new CSensitiveElement(name, upper, lower);
		 
		identifiers.insert(se);
		
		if(iter->second.id_type == "port::out")
		{
			output_identifiers.insert(se);
			cout<<"output = "<< iter->first <<endl;
		}
		
		if(iter->second.id_type == "port::in")
		{
			
			input_identifiers.insert(se);
			cout<<"input = "<< iter->first <<endl;
		}
	}
}

void CSymbSimulator::printSymbolicQueues(map<string, ConditionsValues>& SymbolicQueues)
{
	map< string, ConditionsValues >::iterator isymb = SymbolicQueues.begin();
	
	cout<<"***SymbolicQueues**"<<endl;
	
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

void CSymbSimulator::createTableOverall(CEntity& ent, 
					map<string, ConditionsValues> &SymbolicQueues, 
					set<CSensitiveElement*>& output_identifiers)
{
	set<CSensitiveElement*>:: iterator iout = output_identifiers.begin(); 
	
	// creating table for each output
	for(;iout != output_identifiers.end(); iout++)
	{
		ConditionsValues &cv = SymbolicQueues[(*iout)->name];
		
		vector<int>::iterator  idel = cv.delta.begin();
		vector<CDAGNode*>::iterator ival = cv.values.begin();
		vector<CDAGNode*>::iterator  icond = cv.conditions.begin();
		
		CDAGNode* temp = CHelper::createDAG("true");
		
		int i;
		
		cout<< temp->getDAGStr()<<endl;
		cout<< cv.conditions[0]->getDAGStr()<<endl;
		
		for(i=1 ;i < cv.delta.size(); i++)
		{
			int delta_time = cv.delta[i];
			
			//TODO
			//CDAGNode * dag1 = CHelper::createDAG("equivalent", cv.values[i], cv.values[i-1]);
			//CDAGNode * dag2 = CHelper::createDAG("ite", cv.conditions[i], cv.values[i], dag1);
			
			cout<<cv.values[i]->getDAGStr()<<endl;
			
			// TODO - we have to get relation and not just value
			// CDAGNode * dag1 = bb->mapRelation[*(cv.values[i])];
			CDAGNode * dag1 = cv.values[i];
			// remove this
			 
			RelationInputOutput *rio = new RelationInputOutput(delta_time, dag1);
			
			ent.table[(*iout)->name].push_back(rio);
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
	
	map<string, list<ConditionRelation*> >mapSignalDelta; 
	
	getIdentifiers(identifiers, input_identifiers, output_identifiers, ent);
	
	map<string, ConditionsValues> SymbolicQueues;
		
	createValuesAtZeroTime(SymbolicQueues, ent, identifiers);
	
	createValuesAtOneTime(SymbolicQueues, ent, input_identifiers, 
				affecting, nodePartOfBasicBlock);	
	
	
	int i = 0;
	while(!affectedBasicBlocks.empty() && i < 40)
	{
		AffBlocksAndDelta * abd = affectedBasicBlocks.front();
		affectedBasicBlocks.pop_front();
		
		PerformSymbolicSimulationBB(abd, ent, SymbolicQueues, mapSignalDelta);
		i++;
		
	}
	
	createTableOverall(ent, SymbolicQueues, output_identifiers);
	printSymbolicQueues(SymbolicQueues);
}	

void CSymbSimulator::PerformSymbolicSimulationBB(AffBlocksAndDelta* abd, CEntity& ent, map<string, ConditionsValues> &SymbolicQueues, map<string, list<ConditionRelation*> >& mapSignalDelta)
{
	int current = abd->delta;
	CBasicBlock *bb = abd->bb;
	CDAGNode *prev_condition = abd->condition;
	
	cout<<prev_condition->getDAGStr()<<endl;
	
	map< string, list<RelationInputOutput*> >& table = bb->getTable();
	
	map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
	
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
			}
			else
			{
				map<CDAGNode*,CDAGNode*> nodeShiftednode;
				relation_shifted = relation->cloneAndShift(NULL, nodeShiftednode, shift, setRelation);
				nodeShiftednode.clear();
			}
			
			cout<<relation_shifted->getDAGStr()<<endl;
			
			ent.constraints.push_back(relation_shifted);
			//CDAGNode* condition_shifted = condition->cloneAndShift(NULL, nodeShiftednode);
			
			CDAGNode* out_dag = CHelper::createDAG(output_identifier, current + delta_time - 2);
			
			cout<<out_dag->getDAGStr()<<endl;
			
			char str_current[20];
			
			sprintf(str_current, "%d", current + delta_time - 2);
			
			string id_delta = output_identifier + str_current;
			
			ConditionRelation *cr = new ConditionRelation(prev_condition, relation_shifted);
			
			setRelation.erase(id_delta);
			
			cr->setRelation.insert(setRelation.begin(), setRelation.end());
			
			mapSignalDelta[id_delta].push_back(cr);
			
			insertToSymbolicQueue(current + delta_time - 2, out_dag, prev_condition, SymbolicQueues[output_identifier]);
			
			list<CBasicBlock *> ::iterator iout = bb->out_list.begin();
			int sz1 = bb->out_list.size();
			
			for(;iout != bb->out_list.end(); iout++)
			{
				CSensitiveElement* se = getSenEleforInput((*iout)->get_Input_identifiers(), output_identifier);
				set<string> setCondition;
				
				CDAGNode* triggering_condition = calculateEventCondition(current + delta_time - 2, SymbolicQueues[output_identifier], se, setCondition);
				
				insertToAffectedBB(current + delta_time - 2, *iout, triggering_condition);
			}
		}
	}
}


void CSymbSimulator::createTableForBB(CBasicBlock* bb, map<string, ConditionsValues> &SymbolicQueues)
{
	set<CSensitiveElement*>:: iterator iout = bb->get_Output_identifiers().begin(); 
	
	// creating table for each output
	for(;iout != bb->get_Output_identifiers().end(); iout++)
	{
		ConditionsValues &cv = SymbolicQueues[(*iout)->name];
		
		vector<int>::iterator  idel = cv.delta.begin();
		vector<CDAGNode*>::iterator ival = cv.values.begin();
		vector<CDAGNode*>::iterator  icond = cv.conditions.begin();
		
		CDAGNode* temp = CHelper::createDAG("true");
		
		int i;
		
		cout<< temp->getDAGStr()<<endl;
		cout<< cv.conditions[0]->getDAGStr()<<endl;
		
		// skipping initial changes
		/*for(i = 0; i < cv.delta.size(); i++)
		{
			if(!temp->equals(cv.conditions[i]))
			{
				break;
			}
		}*/
		
		//++i;
		
		// creating RelationInputOutput entries for each entry in symbolic queue 
		// starting from second
		
		for(i=1 ;i < cv.delta.size(); i++)
		{
			int delta_time = cv.delta[i];
			
			//TODO
			//CDAGNode * dag1 = CHelper::createDAG("equivalent", cv.values[i], cv.values[i-1]);
			//CDAGNode * dag2 = CHelper::createDAG("ite", cv.conditions[i], cv.values[i], dag1);
			
			// remove this
			map<CDAGNode, CDAGNode*> ::iterator iter = bb->mapRelation.begin();
			for(; iter != bb->mapRelation.end() ; iter++)
			{
				CDAGNode temp = (iter->first);
				
				
				cout<<temp.getDAGStr()<<endl;
				cout<<iter->second->getDAGStr()<<endl;
				cout<<"---"<<endl;				
			}
			
			cout<<cv.values[i]->getDAGStr()<<endl;
			
			CDAGNode * dag1 = bb->mapRelation[*(cv.values[i])];
			// remove this
			 
			RelationInputOutput *rio = new RelationInputOutput(delta_time, dag1);
			
			bb->table[(*iout)->name].push_back(rio);
		}
	}
}

void CSymbSimulator::insertToSymbolicQueue(int delta_time, CDAGNode* relation_shifted, CDAGNode* condition, ConditionsValues& cv)
{
	cout<<"inserting..."<<endl;
	cout<<relation_shifted->getDAGStr()<<endl;
	cout<<condition->getDAGStr()<<endl;
	
	vector<int>::iterator  idel = cv.delta.begin();
	vector<CDAGNode*>::iterator ival = cv.values.begin();
	vector<CDAGNode*>::iterator  icond = cv.conditions.begin();
	
	bool already_presnt = false;
	
	for(;idel != cv.delta.end(); idel++, ival++, icond++)
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
		/*TODO*/
	}
	else
	{
		cv.delta.insert(idel, delta_time);
		cv.values.insert(ival, relation_shifted);
		cv.conditions.insert(icond, condition);		
	}
}

CDAGNode * CSymbSimulator::calculateEventCondition(int delta_time, ConditionsValues& cv, CSensitiveElement* se, set<string>& setCondition)
{
/*	cv.delta;
	cv.values;
	cv.conditions;*/
	
	int lower = atoi(se->lower.c_str());
	int upper = atoi(se->upper.c_str()); 	
	
	CDAGNode* temp = CHelper::createDAG("true");
	
	int size = cv.delta.size();
	
	int i;
	
	for(i = 0; i < cv.delta.size()-1; i++)
	{
		if(!temp->equals(cv.conditions[i]))
		{
			break;
		}
	}
	
	CDAGNode * dag1 = NULL;
	
	CDAGNode * dag_true = CHelper::createDAG("select", cv.values[i], lower, upper);
	
	if(i == 0)
	{
		CDAGNode * dag_false = CHelper::createDAG("select", cv.values[i-1], lower, upper);
		dag1 = CHelper::createDAG("ite", cv.conditions[i], dag_true, dag_false);
	}
	else
	{
		dag1 = dag_true;		
	}
	
	cout<<dag1->getDAGStr()<<endl;
	
	for(;i < cv.delta.size()-1; i++)
	{
		CDAGNode * dag_true = CHelper::createDAG("select", cv.values[i], lower, upper);
		
		CDAGNode * dag2 = CHelper::createDAG("ite", cv.conditions[i], dag_true, dag1);
		dag1 = dag2;
	}
	
	CDAGNode * dag_current = CHelper::createDAG("select", cv.values[cv.delta.size()-1], lower, upper);
	cout<<dag_current->getDAGStr()<<endl;
	
	
	CDAGNode * dag3 = CHelper::createDAG("is_not_equal", dag_current, dag1);
	cout<<dag3->getDAGStr()<<endl;
	
	CDAGNode * final_expression = CHelper::createDAG("and", cv.conditions[cv.delta.size()-1], dag3);
	cout<<final_expression->getDAGStr()<<endl;
	
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
				
				insertToAffectedBB(1, *iout, triggering_condition);
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
	
	list<int> deltas;
	
	deltas.push_back(1);
	
	for(;inode != sortedNodes.end();inode++)
	{
		switch((*inode)->arch_stmt->getType())
		{
			case CONCURRENT_ASSIGNMENT_STMT:
				 deltas = PerformSymbolicSimulationConAssign(bb, (*inode)->arch_stmt->stmt.conc_assign_stmt, ent, SymbolicQueues, deltas);
			break;
	
			case PROCESS_STMT:
				deltas= PerformSymbolicSimulationProcess(bb, (*inode)->arch_stmt->stmt.process_stmt, ent, SymbolicQueues, deltas);
			break;
			
			case COMPONENT_INSTANTIANTION_STMT:
				deltas = PerformSymbolicSimulationCompInstant(bb, (*inode)->arch_stmt->stmt.instance_stmt, ent, SymbolicQueues, deltas);
			break;
		}
	}
}

list<int> CSymbSimulator::PerformSymbolicSimulationConAssign(CBasicBlock * bb, CConSignAssgnStmt * conc_assign_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, list<int> &deltas)
{
	list<int> deltas_new;
	
	set<string> setRelation;
	set<string> setRelation1;
	
	while(deltas.size() != 0)
	{
		int current = deltas.front();
	    deltas.pop_front();
	    
		COperand *lhs = conc_assign_stmt->getLHS();
		CDAGNode *final_expression = NULL;
		CDAGNode * dag1 = NULL;
	
		list<CExpression*> values = (conc_assign_stmt)->getValues();
		list<CExpression*> choices = (conc_assign_stmt)->getChoices();
	
		list<CExpression*> ::iterator ichoice = choices.begin();
		list<CExpression*> ::iterator ivalue = values.begin();
		cout<<"you are here... sizes "<<choices.size() <<"  "<<values.size() <<endl;
		
		if((*ichoice)->getOperand() == "others")
		{
			dag1 = (*ivalue)->getDAG(ent.getName(), current, setRelation);
			cout<<dag1->getDAGStr()<<endl;
		}
		else if(choices.size() == 1)
		{
			if(conc_assign_stmt->getSelExpr() != "")
			{
				CDAGNode *sel_expr = CHelper::createDAG(conc_assign_stmt->getSelExpr());
				dag1 = CHelper::createDAG("=", sel_expr, (*ichoice)->getDAG(ent.getName(), current, setRelation) );
			}
			else
			{
				dag1 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
			}
			
			final_expression = CHelper::createDAG("ite", dag1, (*ivalue)->getDAG(ent.getName(), current, setRelation), lhs->getDAG(ent.getName(), current, setRelation));
			cout<<final_expression->getDAGStr()<<endl;
		}
		else
		{
			list<CExpression*> ::reverse_iterator ichoice = choices.rbegin();
			list<CExpression*> ::reverse_iterator ivalue = values.rbegin();
			
			if((*ichoice)->getOperand() == "others")
			{
				dag1 = (*ivalue)->getDAG(ent.getName(), current, setRelation);
			}	
			else
			{
				CDAGNode * dag2 = NULL;
				if(conc_assign_stmt->getSelExpr() != "")
				{
					CDAGNode *sel_expr = CHelper::createDAG(conc_assign_stmt->getSelExpr());
					dag2 = CHelper::createDAG("=", sel_expr, (*ichoice)->getDAG(ent.getName(), current, setRelation) );
				}
				else
				{
					dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
				}
				
				dag1 = CHelper::createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), lhs->getDAG(ent.getName(), current, setRelation));
			}
				
			for(;ichoice != choices.rend(); ichoice++,ivalue++)
			{
				CDAGNode * dag2 = NULL;
				if(conc_assign_stmt->getSelExpr() != "")
				{
					CDAGNode *sel_expr = CHelper::createDAG(conc_assign_stmt->getSelExpr());
					dag2 = CHelper::createDAG("=", sel_expr, (*ichoice)->getDAG(ent.getName(), current, setRelation) );
				}
				else
				{
					dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
				}
				
				CDAGNode * dag3 = CHelper::createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), dag1);
				dag1 = dag3;
			}
		}
		
		CDAGNode* lhs_dag = lhs->getDAG(ent.getName(), current+1, setRelation1);
		
		final_expression = CHelper::createDAG("equivalent", lhs_dag, dag1);
		cout<<final_expression->getDAGStr()<<endl;
		
		CDAGNode* true_cond = CHelper::createDAG("true");
		
		bb->constraints.push_back(final_expression);
		
		ConditionRelation* cr = new ConditionRelation(true_cond, final_expression);
		cr->setRelation.insert(setRelation.begin(), setRelation.end());
		
		char curr_delta[5];
		sprintf(curr_delta, "%d", current+1);
		
		string id_delta = lhs->getName() + curr_delta;;
		
		bb->mapSignalDelta[id_delta].push_back(cr);
		
		//think
		CDAGNode* ddd = CHelper::createDAG(lhs->getName(), current+1);
		bb->mapRelation[*ddd] = final_expression;
		
		SymbolicQueues[lhs->getName()].delta.push_back(current+1);
		SymbolicQueues[lhs->getName()].values.push_back(ddd);
		
		//TODO
		SymbolicQueues[lhs->getName()].conditions.push_back(true_cond);
	
		deltas_new.push_back(current + 1);
		deltas_new.sort();
		deltas_new.unique();
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
	
	vector<int> atom_indices = CHelper::findAtomsOfLHS(atom_ranges, lower, upper);
	
	int i = 0;
	int start = 0;
	for(;i < atom_indices.size(); i++)
	{
		int index = atom_indices[i];
		int end = atom_ranges[index].second - atom_ranges[index].first + start;
		char temp[20];
		sprintf(temp, "%d", start);
		CDAGNode *st_dag = CHelper::createDAG(temp);

		sprintf(temp, "%d", end);
		CDAGNode *en_dag = CHelper::createDAG(temp);

		// TODO
		// int current = 1;			
		values[current][index] = CHelper::createDAG("select", final_expression, st_dag, en_dag);
		start = end+1;
	}*/
}

list<int>  CSymbSimulator::PerformSymbolicSimulationProcess(CBasicBlock * bb, CProcess * process_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, list<int> &deltas)
{
	list<int> deltas_new;
	
	while(deltas.size() != 0)
	{
		int current = deltas.front();
	    deltas.pop_front();
	    	
		map< string, list<RelationInputOutput*> >& table = process_stmt->table;
		
		map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
		
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
				}
				else
				{
					map<CDAGNode*,CDAGNode*> nodeShiftednode;
					relation_shifted = relation->cloneAndShift(NULL, nodeShiftednode, shift, setRelation);
					nodeShiftednode.clear();
				}
				
				cout<<relation_shifted->getDAGStr()<<endl;
				
				ent.constraints.push_back(relation_shifted);
				//CDAGNode* condition_shifted = condition->cloneAndShift(NULL, nodeShiftednode);
				
				CDAGNode* out_dag = CHelper::createDAG(output_identifier, current + delta_time - 2);
				
				cout<<out_dag->getDAGStr()<<endl;
				
				char str_current[20];
				
				sprintf(str_current, "%d", current + delta_time - 2);
				
				string id_delta = output_identifier + str_current;
				
				CDAGNode* true_cond = CHelper::createDAG("true");
				
				ConditionRelation *cr = new ConditionRelation(true_cond, relation_shifted);
				
				setRelation.erase(id_delta);
				
				cr->setRelation.insert(setRelation.begin(), setRelation.end());
				
				bb->mapSignalDelta[id_delta].push_back(cr);
				
				insertToSymbolicQueue(current + delta_time - 2, out_dag, true_cond, SymbolicQueues[output_identifier]);
				
				deltas_new.push_back((*irio)->delta_time + current);
			}				
		}		
	}
	
	return deltas_new;	
}

list<int>  CSymbSimulator::PerformSymbolicSimulationCompInstant(CBasicBlock * bb, CInstance * instance_stmt, CEntity &ent, map<string, ConditionsValues> &SymbolicQueues, list<int> &deltas)
{
	list<int> deltas_new;
	
	while(deltas.size() != 0)
	{
		int current = deltas.front();
	    deltas.pop_front();
	    	
		int index = instance_stmt->getIndex();
		CEntity &comp = Entities[index];
		
		map< string, list<RelationInputOutput*> >& table = comp.getTable();
		
		map< string, list<RelationInputOutput*> >::iterator itab = table.begin();
		
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
				}
				else
				{
					map<CDAGNode*,CDAGNode*> nodeShiftednode;
					relation_shifted = relation->cloneAndShift(NULL, nodeShiftednode, shift, setRelation);
					nodeShiftednode.clear();
				}
				
				cout<<relation_shifted->getDAGStr()<<endl;
				
				ent.constraints.push_back(relation_shifted);
				//CDAGNode* condition_shifted = condition->cloneAndShift(NULL, nodeShiftednode);
				
				CDAGNode* out_dag = CHelper::createDAG(output_identifier, current + delta_time - 2);
				
				cout<<out_dag->getDAGStr()<<endl;
				
				char str_current[20];
				
				sprintf(str_current, "%d", current + delta_time - 2);
				
				string id_delta = output_identifier + str_current;
				
				CDAGNode* true_cond = CHelper::createDAG("true");
				
				ConditionRelation *cr = new ConditionRelation(true_cond, relation_shifted);
				
				setRelation.erase(id_delta);
				
				cr->setRelation.insert(setRelation.begin(), setRelation.end());
				
				bb->mapSignalDelta[id_delta].push_back(cr);
				
				insertToSymbolicQueue(current + delta_time - 2, out_dag, true_cond, SymbolicQueues[output_identifier]);
				
				deltas_new.push_back((*irio)->delta_time + current);
			}				
		}		
	}
	
	return deltas_new;
}

void CSymbSimulator::createConstraint(OrientedStmt* os, map<string, list<RangeConstraint*> > &mapRV, CDAGNode *dag)
{
	if(mapRV.find(os->getLhs()->getName()) != mapRV.end())
	{
		list<RangeConstraint*>& listRV = mapRV[os->getLhs()->getName()];
		list<RangeConstraint*>::iterator irv;
		
		int low_new = atoi(((CAtom*)(os->getLhs()))->getLower().c_str());
		int upp_new = atoi(((CAtom*)(os->getLhs()))->getUpper().c_str());
		
		for(irv = listRV.begin(); irv != listRV.end(); ++irv)
		{
			int low_old = (*irv)->range.first;
			int upp_old = (*irv)->range.second;

			if(low_new <= low_old && upp_old <= upp_new)
			{
				irv = listRV.erase(irv);
				// same or total spanning
			}
			else if(low_new == low_old && upp_old >= upp_new)
			{
				// old    5            15
				// new    5       12
				pair<int, int> upper_range = make_pair(upp_new+1, upp_old);
				
				CDAGNode *dag_upp =	CHelper::createDAG("select", 
													(*irv)->constraint, 
													upp_new+1 - low_old, 
													upp_old - low_old);
													
				RangeConstraint *rc_upp = new RangeConstraint(upp_new+1, upp_old, dag_upp);
				listRV.push_back(rc_upp);
				
				irv = listRV.erase(irv);
				// inside
			}	
			else if(low_new >= low_old && upp_old == upp_new)
			{
				// old    5            15
				// new          8      15
				pair<int, int> lower_range = make_pair(low_old, low_new-1);
				CDAGNode *dag_low =	CHelper::createDAG("select", 
													(*irv)->constraint, 
													low_old - low_old, 
													low_new-1 - low_old);
													
				RangeConstraint *rc_low = new RangeConstraint(low_old, low_new-1, dag_low);
				
				listRV.push_back(rc_low);
				
				irv = listRV.erase(irv);
				
				// inside
			}	
			else if(low_new > low_old && upp_old > upp_new)
			{
				// old    5            15
				// new        8    12
				pair<int, int> lower_range = make_pair(low_old, low_new-1);
				pair<int, int> upper_range = make_pair(upp_new+1, upp_old);

				CDAGNode *dag_low =	CHelper::createDAG("select", 
													(*irv)->constraint, 
													low_old - low_old, 
													low_new-1 - low_old);
													
				RangeConstraint *rc_low = new RangeConstraint(low_old, low_new-1, dag_low);


				CDAGNode *dag_upp =	CHelper::createDAG("select", 
													(*irv)->constraint, 
													upp_new+1 - low_old, 
													upp_old - low_old);
													
				RangeConstraint *rc_upp = new RangeConstraint(upp_new+1, upp_old, dag_upp);
				
				listRV.push_back(rc_upp);
				listRV.push_back(rc_low);

				irv = listRV.erase(irv);
				// inside
			}	
			else if(low_new >= low_old && low_new <= upp_old)
			{
				CDAGNode *dag_low =	CHelper::createDAG("select", 
													(*irv)->constraint, 
													low_old - low_old, 
													low_new-1 - low_old);
													
				RangeConstraint *rc_low = new RangeConstraint(low_old, low_new-1, dag_low);

				listRV.push_back(rc_low);

				irv = listRV.erase(irv);
				// lower is inside
			}	
			else if(upp_new >= low_old && upp_new <= upp_old)
			{
				CDAGNode *dag_upp =	CHelper::createDAG("select", 
													(*irv)->constraint, 
													upp_new+1 - low_old, 
													upp_old - low_old);
													
				RangeConstraint *rc_upp = new RangeConstraint(upp_new+1, upp_old, dag_upp);

				listRV.push_back(rc_upp);
				
				irv = listRV.erase(irv);
				// upper is inside
			}
		}
		
		//TODO
//		CDAGNode *dag = CHelper::createDAG(os->getLhs()->getName());
		
		RangeConstraint *rc = new RangeConstraint(low_new, upp_new, dag);
		listRV.push_back(rc);
	}
}

CDAGNode *CSymbSimulator::createDAGforOS(OrientedStmt* os, int current, CEntity &ent)
{
	list<CExpression*> ::iterator ichoice_other = os->getConditions().begin();
	list<CExpression*> ::iterator ivalue_other = os->getValues().begin();
	
	CDAGNode * dag1 = NULL;
	
	set<string> setRelation;
	set<string> setRelation1;

	for(;ichoice_other != os->getConditions().end(); ichoice_other++,ivalue_other++)
	{
		if((*ichoice_other)->getOperand() == "others")
		{
			dag1 = (*ivalue_other)->getDAG(ent.getName(), current, setRelation);
			ichoice_other = os->getConditions().erase(ichoice_other);
			ivalue_other = os->getValues().erase(ivalue_other);
			break;
		}	
	}

	for(;ichoice_other != os->getConditions().end(); ichoice_other++,ivalue_other++)
	{
		if((*ichoice_other)->getOperand() == "others")
		{
			CDAGNode * dag3 = CHelper::createDAG("OR", (*ivalue_other)->getDAG(ent.getName(), current, setRelation), dag1);
			dag1 = dag3;
			ichoice_other = os->getConditions().erase(ichoice_other);
			ivalue_other = os->getValues().erase(ivalue_other);
		}	
	}
	
	
	list<CExpression*> ::reverse_iterator ichoice = os->getConditions().rbegin();
	list<CExpression*> ::reverse_iterator ivalue = os->getValues().rbegin();
	
	if(dag1 == NULL)
	{
		CDAGNode * dag2 = NULL;
		dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
		dag1 = CHelper::createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), os->getLhs()->getDAG(ent.getName(), current, setRelation));
	}	

	for(;ichoice != os->getConditions().rend(); ichoice++,ivalue++)
	{
		CDAGNode * dag2 = NULL;
		dag2 = (*ichoice)->getDAG(ent.getName(), current, setRelation);
		
		CDAGNode * dag3 = CHelper::createDAG("ite", dag2, (*ivalue)->getDAG(ent.getName(), current, setRelation), dag1);
		dag1 = dag3;
	}
	
	CDAGNode* lhs_dag = os->getLhs()->getDAG(ent.getName(), current+1, setRelation1);
	
	CDAGNode * final_expression = CHelper::createDAG("equivalent", lhs_dag, dag1);
	cout<<final_expression->getDAGStr()<<endl;	
	
	return final_expression;
}


void CSymbSimulator::CreateTableForProcesses(CEntity &ent)
{
	list<CProcess *> &processNodes = ent.getProcesses();
	string entName = ent.getName();
	
	list<CProcess *>::iterator iNodes;

	for(iNodes = processNodes.begin();
		iNodes != processNodes.end();
		iNodes++
		)
	{
		list<string> ::iterator isen;

		CSeqStmt *stmt_ptr = (*iNodes)->getFirst();
		
		list<OrientedStmt*> ostmts;
		list<CSensitiveElement*> sensitivityList;
		
		if(stmt_ptr != NULL)
		{
			stmt_ptr->getOrientedStmts(NULL, ostmts);
		}

		map<string, list<RangeConstraint*> > mapRV;
		list<OrientedStmt*>::iterator  ios;
		for(ios = ostmts.begin(); ios != ostmts.end(); ++ios)
		{
			CDAGNode* dag = createDAGforOS(*ios, 1, ent);
			createConstraint(*ios, mapRV, dag);
		}
	}
}

