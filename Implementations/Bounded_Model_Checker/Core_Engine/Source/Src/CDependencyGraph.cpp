#include "CDependencyGraph.h"
#include "y.tab.h"

void CDependencyGraph::createDependencyGraph(CEntity &ent)
{
// 	list<CInstance *> &instanceNodes = ent.getInstances();
// 	createGraphForInstances(fp, instanceNodes);
// 	cout<<"instanceNodes"<<instanceNodes.size()<<endl;

	string EntName = ent.getName();

	list<CConSignAssgnStmt *> &conAssignNodes = ent.getConAssgnStmt();
	createGraphForConSignAssgn(conAssignNodes, EntName);
	//cout<<"conAssignNodes"<<conAssignNodes.size()<<endl;
	
	
	
	

	list<CProcess *> &processNodes = ent.getProcesses();
	createGraphForProcesses(processNodes, EntName);
	//cout<<"processNodes"<<processNodes.size()<<endl;
	
	
	list<CInstance *> &instanceNodes = ent.getInstances();
	createGraphForInstances(instanceNodes, EntName);
	//cout<<"instanceNodes"<<instanceNodes.size()<<endl;
	
	

//	FormSymbolicStructure(EntName);
	
	//printDependencyGraph();
	//cout<<"**************************************************"<<endl;
	//printOrientedStmts();
	
	createDependencies();
	
	//printGraph();
		
	

	

}

void CDependencyGraph::FormSymbolicStructure(string EntName)
{
/*	map<string, list<int> >::iterator iatom;
	
	for(iatom = atoms.begin();iatom != atoms.end();iatom++)
	{
		string id_name = iatom->first;
		list<int>::iterator ib = iatom->second.begin();
		
		vector< pair<int, int> > atom_ranges;
		
		list<string>::iterator irange = SymbolTable[EntName][id_name].rangeindex.begin();
		
		int start = atoi((*irange).c_str());
		for(;ib != iatom->second.end();ib++)
		{
			int end = *ib;
			atom_ranges.push_back(make_pair(start, end));
			start = end+1;
		}
		
		irange++;
		
		int upper = atoi((*irange).c_str());;
		atom_ranges.push_back(make_pair(start, upper));

		RangeValue rv;
		rv.atom_ranges = atom_ranges;
		
		if(SymbolicValues.find(EntName) != SymbolicValues.end())
		{
			map<string, RangeValue> &id_symbValues = SymbolicValues[EntName];
			id_symbValues[id_name] = rv;
		}
		else
		{
			map<string, RangeValue> symbValues;
			symbValues[id_name] = rv;
			SymbolicValues[EntName] = symbValues;
		}
	}*/
}

void CDependencyGraph::FindAtoms()
{
	map<CSensitiveElement, list<node*> > ::iterator iaffed;
	int size = affected.size();
	
	CSensitiveElement se;
	
/*	se.name = "d";
	se.lower = "0";
	se.upper = "5";
	
	int n = affected[se].size();*/
	
	for(iaffed = affected.begin(); iaffed != affected.end(); iaffed++)
	{
		CSensitiveElement se = iaffed->first;
		//cout<< se.name<< " ";
		//cout<< se.lower<< " ";
		//cout<< se.upper<< " ";
		//cout<<endl;		
	}
		
	
	
	
	//map<CSensitiveElement, list<node*> > ::iterator iaffed;
	//int size = affected.size();
	for(iaffed = affected.begin(); iaffed != affected.end(); iaffed++)
	{
		CSensitiveElement se = iaffed->first;
		
		int lower = atoi(se.lower.c_str());
		int upper = atoi(se.upper.c_str());
		
		list<int> &atomList = atoms[se.name];
		
		list<int> :: iterator ifind; 
		ifind = find(atomList.begin(), atomList.end(), lower-1);
		if(lower && ifind == atomList.end())
		{
			atomList.push_back(lower-1);
		}

		ifind = find(atomList.begin(), atomList.end(), upper);
		if(upper && ifind == atomList.end())
		{
			atomList.push_back(upper);
		}
	}
	
	map<string, list<int> > ::iterator iatom = atoms.begin();
	for(iatom = atoms.begin(); iatom != atoms.end(); iatom++)
	{
		char *temp = strdup(iatom->first.c_str());
		iatom->second.sort();
	}
}


void CDependencyGraph::initialize()
{
	list<node *> ::iterator inode = nodes.begin();
	for(;inode != nodes.end();inode++)
	{
		int size = (*inode)->in_list.size();
		(*inode)->setCount((*inode)->in_list.size());
	}
}

void CDependencyGraph::adjust_indegrees(list<node *> &nodesWithZeroIndegree)
{
	list<node *> ::iterator inode = nodesWithZeroIndegree.begin();
	for(;inode != nodesWithZeroIndegree.end();inode++)
	{
		(*inode)->setCount(-1);
		
		list<node *> ::iterator iout = (*inode)->out_list.begin();
		for(;iout != (*inode)->out_list.end();iout++)
		{
			int size = (*iout)->getCount() - 1;
			(*iout)->setCount((*iout)->getCount() - 1);
		}
	}
}

void CDependencyGraph::find_nodes_with_zero_indegree(list<node *> &nodesWithZeroIndegree)
{
	list<node *> ::iterator inode = nodes.begin();
	for(;inode != nodes.end();inode++)
	{
		if((*inode)->getCount() == 0)
		{
			nodesWithZeroIndegree.push_back(*inode);
		}
	}
}


void CDependencyGraph::topologicalSorting(list<node *> &sortedNodes)
{
	int totalNodes = nodes.size();
	initialize();
	
	list<node *> nodesWithZeroIndegree;
//	find_nodes_with_zero_indegree(nodesWithZeroIndegree);

//	list<node *> ::iterator inode = nodesWithZeroIndegree.begin();
//	for(;inode != nodesWithZeroIndegree.end();inode++)
//	{
//		sortedNodes.push_back(*inode);
//	}

//	copy(nodesWithZeroIndegree.begin(), nodesWithZeroIndegree.end(), sortedNodes.end());
//	adjust_indegrees(nodesWithZeroIndegree);

	int size = nodesWithZeroIndegree.size();
	nodesWithZeroIndegree.clear();
	
	while(totalNodes != sortedNodes.size())
	{
		find_nodes_with_zero_indegree(nodesWithZeroIndegree);
		
		list<node *> ::iterator inode = nodesWithZeroIndegree.begin();
		for(;inode != nodesWithZeroIndegree.end();inode++)
		{
			sortedNodes.push_back(*inode);
		}

		adjust_indegrees(nodesWithZeroIndegree);
		
		nodesWithZeroIndegree.clear();		
		//cout<< "sortedNodes  "<<sortedNodes.size()<<endl;
	}
}


void CDependencyGraph::printGraph()
{
	list<node *> ::iterator inode = nodes.begin();
	for(;inode != nodes.end();inode++)
	{
		//cout<<*inode<<" --->  ";
		
		list<node *> ::iterator iout = (*inode)->out_list.begin();
		for(;iout != (*inode)->out_list.end();iout++)
		{
			//cout<< "   "<<*iout;
		}
		
		//cout<<endl;
	}	
	
}

set<node *> CDependencyGraph::identifyStartsEnds()
{
	set<node *> startnodes;
	list<node *> ::iterator inode = nodes.begin();
	for(;inode != nodes.end();inode++)
	{
		node * temp = *inode;
		if((*inode)->in_list.size()>1 || (*inode)->start == true)
		{
			(*inode)->start = true;
			startnodes.insert(*inode);
		}
		
		if((*inode)->out_list.size()>1 || (*inode)->end == true)
		{
			(*inode)->end = true;
			
			list<node *> ::iterator iout = (*inode)->out_list.begin();
			for(;iout != (*inode)->out_list.end();iout++)
			{
				(*iout)->start = true;
				startnodes.insert(*iout);
			}
		}
	}	
	
	return startnodes;
}

void CDependencyGraph::identifyStartsEnds1()
{
	set<node *> startnodes;
	list<node *> ::iterator inode = nodes.begin();
	for(;inode != nodes.end();inode++)
	{
		(*inode)->start = true;
	}	
	
}

void CDependencyGraph::printBB()
{
	list<CBasicBlock *> ::iterator ibb = basicblocks.begin();
	for(;ibb != basicblocks.end();ibb++)
	{
		CBasicBlock* bb = *ibb;
		//cout<<"BB:: "<<*ibb;
	
		list<node *> ::iterator inode = (*ibb)->block_nodes.begin();
		//cout<<"Nodes inside:: " ;
		
		for(;inode != (*ibb)->block_nodes.end();inode++)
		{
			//cout<<"   "<<*inode;
		}
		
		//cout<<endl;
		
		//cout<<"RHS ids are:: " ;
		set<CSensitiveElement*> &Input_identifiers = bb->get_Input_identifiers(); 
		set<CSensitiveElement*> &output_identifiers = bb->get_Output_identifiers();
		
		set<CSensitiveElement*>::iterator iter = Input_identifiers.begin();
		for(; iter != Input_identifiers.end() ; iter++)
		{
			//cout<<""<< (*iter)->name<<endl;
		}
		
		//cout<<endl;
		//cout<<"LHS ids are:: " ;
		iter = output_identifiers.begin();
		for(; iter != output_identifiers.end() ; iter++)
		{
			//cout<<""<<(*iter)->name<<endl;
		}
		
		//cout<<"outlist:: " ;
		
			
		list<CBasicBlock *> ::iterator iout = (*ibb)->out_list.begin();
		
		for(;iout != (*ibb)->out_list.end();iout++)
		{
			//cout<<"   "<<*iout;
		}
		//cout<<endl;
				
	}
}

void CDependencyGraph::formBlocks(set<node *> &startnodes)
{
	set<node *> ::iterator inode = startnodes.begin();
	for(;inode != startnodes.end();inode++)
	{
		list<node *> block_nodes;
		//set<string> identifiers;
		CBasicBlock *bb = new CBasicBlock();
		
		if(((*inode)->start && (*inode)->end) || (*inode)->out_list.size() == 0)
		{
			block_nodes.push_back(*inode);
			
			bb->addInputIdentifiers((*inode)->identifiers);
//			copy(, identifiers.begin());

			nodePartOfBasicBlock[*inode] = bb;
			bb->addOutputIdentifiers((*inode)->output_identifiers);
			
			
			neighbors[bb] = (*inode)->out_list;
		}
		else
		{
			block_nodes.push_back(*inode);
			bb->addInputIdentifiers((*inode)->identifiers);
//			copy((*inode)->identifiers.begin(), (*inode)->identifiers.end(), identifiers.begin());

			nodePartOfBasicBlock[*inode] = bb;
			
			
			node *next = *((*inode)->out_list.begin());
			node *previous = *inode;
			
			while(1)
			{
				if(next->start)
				{
					bb->addOutputIdentifiers(previous->output_identifiers);
					neighbors[bb] = previous->out_list;
					break;
				}
				
				if(next->end)
				{
					bb->addIdentifiers(previous->output_identifiers);
					
					nodePartOfBasicBlock[next] = bb;
					bb->addOutputIdentifiers((next)->output_identifiers);
					
					neighbors[bb] = next->out_list;
					
					block_nodes.push_back(next);
					bb->addIdentifiers((next)->identifiers);
					//copy((next)->identifiers.begin(), (next)->identifiers.end(), identifiers.begin());
					
					break;
				}
				
				bb->addIdentifiers(previous->output_identifiers);
				
				nodePartOfBasicBlock[next] = bb;
				
				//neighbors[bb] = next->out_list;
				
				block_nodes.push_back(next);
				bb->addIdentifiers((next)->identifiers);
				//copy((next)->identifiers.begin(), (next)->identifiers.end(), identifiers.begin());
				
				previous = next;
				next = *(next->out_list.begin());
				
			}
			
/*			while(!next->start && !next->end)
			{
				block_nodes.push_back(next);
				nodePartOfBasicBlock[next] = bb;
				next = *(next->out_list.begin());
			}
*/
			
//			nodePartOfBasicBlock[next] = bb;
//			neighbors[bb] = next->out_list;
		}
		
		bb->addBlockNodes(block_nodes);
		
		basicblocks.push_back(bb);
	}
}

void CDependencyGraph::formBasicBlocks1()
{
	set<node *> startnodes ;
	
	identifyStartsEnds1();
	
	startnodes.insert(nodes.begin(), nodes.end());
	
	int size1 = nodes.size();
	
	
	int size2 = startnodes.size();
	
	
	
	
	formBlocks(startnodes);
	printBB();
	createDependenciesBB();
	
}


void CDependencyGraph::formBasicBlocks()
{
	set<node *> startnodes = identifyStartsEnds();
	
	int size = startnodes.size();
	
	formBlocks(startnodes);
	printBB();
	createDependenciesBB();
	
}

void CDependencyGraph::createDependenciesBB()
{
	list<CBasicBlock*> ::iterator ibb;
	
	for(ibb = basicblocks.begin();ibb != basicblocks.end();ibb++)
	{
//		list<node *>::iterator inode = (*ibb)->block_nodes.begin();
		list<node *>::iterator inode = neighbors[*ibb].begin();
		
		//cout<<"**********"<<endl;
		//cout<< *ibb<<endl;
		//cout<<"---"<<endl;
		
		for(;inode != neighbors[*ibb].end();inode++)
		{
			//cout<< *inode<<endl;
					
			CBasicBlock* bb = nodePartOfBasicBlock[*inode];
			//bb->addTosensitivityList();
			
			bb->addToIncoming(*ibb);
			(*ibb)->addToOutgoing(bb);
		}
	}
	
	
}

void CDependencyGraph::createSensitivityList()
{
	map<string, list<CSenNode*> >::iterator iaff = affecting.begin();
	
	for(;iaff != affecting.end(); iaff++)
	{
		list<CSenNode*> ::iterator ise;
			
		for(ise = affecting[iaff->first].begin();
			ise != affecting[iaff->first].end();
			ise++)
		{
			CBasicBlock * bb = nodePartOfBasicBlock[(*ise)->n];
			bb->addTosensitivityList((*ise)->se);
		}
	}
}

bool CDependencyGraph::isComflicting(const CSensitiveElement &se1, const CSensitiveElement &se2)
{
	int u1 = atoi(se1.upper.c_str());
	int l1 = atoi(se1.lower.c_str());

	int u2 = atoi(se2.upper.c_str());
	int l2 = atoi(se2.lower.c_str());
	
	
	if(l1 > u2 || l2 > u1)
	{
		return false;
	}

	return true;
	
}


void CDependencyGraph::createDependencies()
{
	map<CSensitiveElement, list<node*> > ::iterator iaffed;
	for(iaffed = affected.begin();iaffed != affected.end();iaffed++)
	{
		//cout<< iaffed->first.name <<endl;
	}
	//map<CSensitiveElement, list<node*> > ::iterator iaffed;
	for(iaffed = affected.begin();iaffed != affected.end();iaffed++)
	{
		char *temp = strdup(iaffed->first.name.c_str());
		int size = affecting.size();
		
		if(affecting.find(iaffed->first.name) != affecting.end())
		{
			char *temp = strdup(iaffed->first.name.c_str());
			list<CSenNode*> &senNodeList = affecting[iaffed->first.name];
			list<node*> ::iterator inode = iaffed->second.begin();
			
			for(inode = iaffed->second.begin();inode != iaffed->second.end();inode++)
			{
				list<CSenNode*> ::iterator isn;

				for(isn = senNodeList.begin();isn != senNodeList.end();isn++)
				{
					node *n1 = *inode;
					node *n2 = (*isn)->n;
					if(n1 != n2 && isComflicting(iaffed->first, *((*isn)->se)))
					{
						((*isn)->n)->addToIncoming(*inode);
						(*inode)->addToOutgoing((*isn)->n);
					}
				}
			}
		}
	}
}

void CDependencyGraph::printOrientedStmts()
{
	list<struct node*>::iterator in = nodes.begin();
	for(	in = nodes.begin();
		in != nodes.end();
		in++
		)
	{
		list<OrientedStmt*>::iterator  ios;
		for(ios = (*in)->ostmts.begin();ios != (*in)->ostmts.end();ios++)
		{
			//cout<<"LHS = "<< (*ios)->getLhs()->getName()<<endl;
			
			list<CExpression*>& values = (*ios)->getValues();
			list<CExpression*>& conditions = (*ios)->getConditions();
			
			list<CExpression*> ::iterator icond = conditions.begin();
			list<CExpression*> ::iterator ivalue = values.begin();

			for(int i=1	;
				icond != conditions.end();
				icond++,ivalue++,i++
				)
			{
				//cout<<"cond"<<i<<endl;
				//cout<<(*icond)->getOrgExpr()<<endl;
				//cout<<"value"<<i<<endl;
				//cout<<(*ivalue)->getOrgExpr()<<endl;
			}			
		}
	}	
}

void CDependencyGraph::printDependencyGraph()
{
	map<CSensitiveElement, list<node *> >::iterator iaffed ;
	
	int size = affected.size();
	
	for(iaffed = affected.begin(); iaffed != affected.end(); iaffed++)
	{
		//cout<<"Id:: "<<iaffed->first.name << "--" << iaffed->first.upper << " , " << iaffed->first.lower <<endl;
		list<node *>::iterator inode = iaffed->second.begin();
		
		//cout<<"Nodes:: ";
		
		for(inode = iaffed->second.begin();
			inode != iaffed->second.end();
			inode++
			)
		{
			//cout<< *inode<<endl;
						
			list<CSenNode*> ::iterator ise;
			//cout<<"Sensitivity :: ";
				
/*			for(ise = affecting[iaffed->first.name].begin();
				ise != affecting[iaffed->first.name].end();
				ise++)
			{
		//			cout<<(*ise)->name << "(" << (*ise)->upper << " , " << (*ise)->lower << ")"<<"   ";
			}*/
//			cout<<"Id:: "<<inode->name << "--" << iaffed->first.upper << " , " << iaffed->first.lower <<endl;
		}
		
		//cout<<"------------------------------"<<endl;
		//cout<<endl;
	}
	
	//cout<<"###################################"<<endl;
	//cout<<endl;
	
	map<string, list<CSenNode*> >::iterator iaffing ;
	
	for(	iaffing = affecting.begin();
		iaffing != affecting.end();
		iaffing++
		)
	{
		//cout<<"Key:: "<<iaffing->first<< endl;
		list<CSenNode*>::iterator inode = iaffing->second.begin();
		
		//cout<<"SenNodes:: ";
		
		for(inode = iaffing->second.begin();
			inode != iaffing->second.end();
			inode++
			)
		{
			//cout<< (*inode)->se->name <<"--"<< (*inode)->n <<endl;
		}
		
		//cout<<"------------------------------"<<endl;
		//cout<<endl;
	}
	
}

void CDependencyGraph::print_sen_list(list<CSensitiveElement*> sensitivityList)
{
	list<CSensitiveElement*> ::iterator isen = sensitivityList.begin();
	for(;isen != sensitivityList.end();isen++)
	{
		//cout<<(*isen)->name<<" ( "<<(*isen)->lower <<" ,  "<<(*isen)->upper<<")"<<endl;
		
	}
}

list<CSenNode*> CDependencyGraph::createSenNodeList(list<CSensitiveElement*> senList, node* n, string EntName)
{
	// TODO - we have to change parser to handle sensitity list a(0), a(3 downto 0)
	list<CSenNode*> senNodeList;
	
	list<CSensitiveElement*> ::iterator isen = senList.begin();
	for(; isen != senList.end(); isen++)
	{
		CSenNode* sn = new CSenNode(n, *isen);
//		senNodeList.push_back(sn);
		char *temp = strdup((*isen)->name.c_str());
		affecting_and_sensitive[(*isen)->name].push_back(sn); // newly added
		affecting[(*isen)->name].push_back(sn);
		nodeIdentifiers[n].insert((*isen)->name);
		n->identifiers.insert(*isen);
		//cout<<(*isen)->name<<endl;
		//cout<<n->identifiers.size()<<endl;
		if(SymbolTable[EntName][(*isen)->name].id_type == "port::in")
		{
			n->start = true;
		}
		
	}
	
	return senNodeList;
}

void CDependencyGraph::createGraphForConSignAssgn(list<CConSignAssgnStmt *> &conAssignNodes, string EntName)
{
	list<CConSignAssgnStmt *>::iterator iNodes = conAssignNodes.begin();

	for(iNodes = conAssignNodes.begin();iNodes != conAssignNodes.end();	iNodes++)
	{
		COperand *lhs = (*iNodes)->getLHS();
		
		list<CExpression*> values1;
		list<CExpression*> conditions1;		
		
		CExpression* SelExpression = (*iNodes)->getOriginalSelExpr();
		list<CExpression*> values = (*iNodes)->getValues();
		list<CExpression*> choices = (*iNodes)->getChoices();
		

		list<CExpression*> ::iterator ichoice = choices.begin();
		list<CExpression*> ::iterator ivalue = values.begin();
		//cout<<"you are here... sizes "<<choices.size() <<"  "<<values.size() <<endl;

		list<CSensitiveElement*> sensitivityList;

		//fflush(stdout);

		
		if(SelExpression != NULL)
		  {//cout<<"\nSelection Expression = "<<SelExpression->getOrgExpr();
		    SelExpression->getSensitivityList(sensitivityList); }
		else
		  {//cout<<"\nSelection Expression = NULL";
		  }

		for(; ichoice != choices.end(); ichoice++,ivalue++)
		{
			(*ivalue)->getSensitivityList(sensitivityList);
			values1.push_back(*ivalue);
			
			(*ichoice)->getSensitivityList(sensitivityList);
			conditions1.push_back(*ichoice);
		}
		
		OrientedStmt *os = new OrientedStmt(lhs, values1, conditions1);
		
		node *n = new node(os);
		
		char * temp = strdup(lhs->getName().c_str());
		
		//cout<<"Node:: "<<n<<":: "<<(lhs->getName())<<" "<<(*iNodes)->getSelExpr().c_str()<<endl;
		print_sen_list(sensitivityList);
		
		CSensitiveElement *se = new CSensitiveElement(((CAtom*)lhs)->getName(), ((CAtom*)lhs)->getUpper(), ((CAtom*)lhs)->getLower());
		
		list<CSenNode*> senNodeList = createSenNodeList(sensitivityList, n,EntName);
		
		
		CArchStmt * arch_stmt = new CArchStmt(CONCURRENT_ASSIGNMENT_STMT, *iNodes, NULL, NULL);
		
		n->addStmt(arch_stmt);
		
		
		//affected[se].push_back(n);
		
		
		if(affected.find(*se) != affected.end())
		{
			affected[*se].push_back(n);
		}
		else
		{
			list<node *> nodes;
			nodes.push_back(n);
			affected.insert(make_pair(*se, nodes));
		}
		
		//cout<<SymbolTable[EntName][((CAtom*)lhs)->getName()].id_type<<endl;
		if(SymbolTable[EntName][((CAtom*)lhs)->getName()].id_type == "port::out")
		{
			n->end = true;
		}
		
		
/*		if(SymbolTable[EntName][((CAtom*)lhs)->getName()].id_type == "port::in")
		{
			n->start = true;
		}*/
		
		n->output_identifiers.insert(se);	
		
		nodes.push_back(n);

		nodeIdentifiers[n].insert(se->name);
	}
}


void CDependencyGraph::getSensitivityListConAssignStmt(CConSignAssgnStmt* proc_node, list<CSensitiveElement*>  &sensitivityList)
{
	CExpression* SelExpression = proc_node->getOriginalSelExpr();
	list<CExpression*> values = proc_node->getValues();
	list<CExpression*> choices = proc_node->getChoices();
		

	list<CExpression*> ::iterator ichoice = choices.begin();
	list<CExpression*> ::iterator ivalue = values.begin();
	

	if(SelExpression != NULL)
	  {//cout<<"\nSelection Expression = "<<SelExpression->getOrgExpr();
	   SelExpression->getSensitivityList(sensitivityList); }
	else
	  {//cout<<"\nSelection Expression = NULL";
	  }

	for(; ichoice != choices.end(); ichoice++,ivalue++)
		{
			(*ivalue)->getSensitivityList(sensitivityList);
			
			(*ichoice)->getSensitivityList(sensitivityList);
			
		}
}

void CDependencyGraph::getSensitivityListCompInstStmt(CInstance * proc_node, list<CSensitiveElement*>  &sensitivityList)
{

		list<CExpression*> params= proc_node->getActualParam();
		list<CExpression*> ::iterator iparam= params.begin();

		list<COperand*> formals= proc_node->getFormalParam();
		list<COperand*> ::iterator iformal = formals.begin();

//		fprintf(fp, "param%d [shape=record,label=\"",p);

	
		list<CSensitiveElement*> lhsList;
		
		list<CPort> &ports = Entities[proc_node->getIndex()].getPorts(); 

		list<CPort>::iterator iport;
	
		if(formals.size() != 0)
		{
			
			for(iparam = params.begin();
				iparam != params.end();
				iparam++,iformal++)
			{
			        //cout<<(*iformal)->getName()<<endl;
				CPort& port = getPort(ports, (*iformal)->getName());
				if(port.getMode() == t_IN || port.getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(sensitivityList);
				}

				if(port.getMode() == t_OUT || port.getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(lhsList);
				}
			}
		}
		else
		{
			iport = ports.begin();
			for(iparam = params.begin();
				iparam != params.end();
				iparam++,iport++)
			{
				if((*iport).getMode() == t_IN || (*iport).getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(sensitivityList);
				}
				
				if((*iport).getMode() == t_OUT || (*iport).getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(lhsList);
				}
				
			}

		}
}

void CDependencyGraph::getSensitivityListProcessStmt(CProcess * proc_node, list<CSensitiveElement*>  &sensitivityList, string EntName)
{
  
	list<COperand*>  sen_list1; 

	sen_list1 = proc_node->getSenList();

	sensitivityList = createSenList(sen_list1, EntName);
}


void CDependencyGraph::getOrientedStmts(CSeqStmt* stmt_ptr, list<OrientedStmt*> & ostmts)
{

}

list<CSensitiveElement*> CDependencyGraph::createSenList(list<COperand*>& senList, string EntName)
{
	list<CSensitiveElement*> sensitivityList;
	set<string> setDone;
	
	list<COperand*> ::iterator isen = senList.begin();
	for(; isen != senList.end(); isen++)
	{
		string lower = ((CAtom*)(*isen))->getLower();

		string upper = ((CAtom*)(*isen))->getUpper();
		
		//cout<<(*isen)->getName()+ upper + lower<<endl;
		if(setDone.find((*isen)->getName()+ upper + lower) == setDone.end())
		{
			CSensitiveElement* se = new CSensitiveElement((*isen)->getName(), upper, lower);
			sensitivityList.push_back(se);
			setDone.insert((*isen)->getName()+ upper + lower);
		}
	}
	
	return sensitivityList;
}

// Newly added functions

list<CSenNode*> CDependencyGraph::createSenNodeListForProcess(list<CSensitiveElement*> senList, node* n, string EntName, list<COperand*> sen_list1)
{
	// TODO - we have to change parser to handle sensitity list a(0), a(3 downto 0)
	list<CSenNode*> senNodeList;
	
	list<CSensitiveElement*> ::iterator isen = senList.begin();
	for(; isen != senList.end(); isen++)
	{
		CSenNode* sn = new CSenNode(n, *isen);
//		senNodeList.push_back(sn);
		char *temp = strdup((*isen)->name.c_str());

		if(signalPresentInSensitivityList((*isen),sen_list1))
		  affecting_and_sensitive[(*isen)->name].push_back(sn);

		affecting[(*isen)->name].push_back(sn);
		nodeIdentifiers[n].insert((*isen)->name);
		n->identifiers.insert(*isen);
		//cout<<(*isen)->name<<endl;
		//cout<<n->identifiers.size()<<endl;
		if(SymbolTable[EntName][(*isen)->name].id_type == "port::in")
		{
			n->start = true;
		}
		
	}
	
	return senNodeList;
}

bool CDependencyGraph::signalPresentInSensitivityList(CSensitiveElement* sen_element, list<COperand*> sen_list)
{
  list<COperand*>::iterator sen_list_it = sen_list.begin();

  for(;sen_list_it != sen_list.end(); sen_list_it++)
    {
      if( (((CAtom*)(*sen_list_it))->getLower() == sen_element->lower) && (((CAtom*)(*sen_list_it))->getUpper() == sen_element->upper) && ((*sen_list_it)->getName() == sen_element->name) )
	return true;
    }
  return false;
}



void CDependencyGraph::createGraphForProcesses(list<CProcess *> & processNodes, string EntName)
{
	list<CProcess *>::iterator iNodes = processNodes.begin();

	for(iNodes = processNodes.begin();
		iNodes != processNodes.end();
		iNodes++
		)
	{
		
		list<string> ::iterator isen;

		CSeqStmt *stmt_ptr = (*iNodes)->getFirst();
		
		node *n = new node();
		list<OrientedStmt*> ostmts;
		list<CSensitiveElement*> sensitivityList;
		list<COperand*> sen_list1, sen_list2; 
		list<COperand*>::iterator sen_list2_it;
		
		if(stmt_ptr != NULL)
		{
			stmt_ptr->getChangedLHS(sensitivityList);
		}

		//cout<<"Node::(Process) "<<n<<":: "<<endl;
		
		list<CSensitiveElement*>::iterator  ios;
		
		set<string> setDone;

		//cout<<"\n\nReached here ....";

		for(ios = sensitivityList.begin();ios != sensitivityList.end();ios++)
		  {
		    //(*ios)->printCSensitiveElement();
		  }
		
		//cout<<"\n\nReached here 0....";
		
		ios = sensitivityList.begin();

		while(ios != sensitivityList.end())
		{

		       //(*ios)->printCSensitiveElement();
		       //cout<<"\n\nReached here 01....";


			if(affected.find(*(*ios)) != affected.end())
			  {
			    affected[*(*ios)].push_back(n);
			  }
			else
			  {
			    list<node *> nodes;
			    nodes.push_back(n);
			    affected.insert(make_pair(*(*ios), nodes));
			  }

			//affected[*(*ios)].push_back(n);
			
			
			//cout<<"\n\nReached here 02....";
			nodeIdentifiers[n].insert((*ios)->name);
			//cout<<"\n\nReached here 03....";
			
			if(SymbolTable[EntName][(*ios)->name].id_type == "port::out")
			{
				n->end = true;
			}
			
			if(SymbolTable[EntName][(*ios)->name].id_type == "port::in")
			{
				n->start = true;
			}
		
			if(setDone.find((*ios)->name + (*ios)->upper + (*ios)->lower) == setDone.end())
			{
				n->output_identifiers.insert(*ios);	
				setDone.insert((*ios)->name + (*ios)->upper + (*ios)->lower);
			}
			//cout<<"\n\nReached here 0L....";
			ios++;
			//cout<<"\n\nReached here 0L2....";
		}
		
		//cout<<"\n\nReached here 1....";

		/* Newly added on 16/9/08 */

		sensitivityList.clear();

		if(stmt_ptr != NULL)
		{
			stmt_ptr->getChangedRHS(sen_list1);
		}

		//cout<<"\n\nReached here 2....";

		/* Removing variables from the sensitivity list */

		sen_list2_it = sen_list1.begin();

		while(sen_list2_it != sen_list1.end())
		  {

		    if(SymbolTable[EntName][((CAtom*)(*sen_list2_it))->getName()].id_type == "variable")
		      {
			sen_list2_it = sen_list1.erase(sen_list2_it);
		      }
		    else
		      {
			sen_list2_it++;
		      }
		   
		  }

		//cout<<"\n\nReached here 3....";
		
		sen_list2 = (*iNodes)->getSenList();

		sen_list2_it = sen_list2.begin();

		for(; sen_list2_it != sen_list2.end(); sen_list2_it++)
		  {
		    sen_list1.push_back(*sen_list2_it);
		  }
		
		sen_list2_it = sen_list1.begin();

		/*cout<<"\n\nInputs of Process\n\n";

		

		for(; sen_list2_it != sen_list1.end(); sen_list2_it++)
		  {
		    cout<<"\n\nName = "<<((CAtom*)(*sen_list2_it))->getName()<<"\t"<<"upper = "<<((CAtom*)(*sen_list2_it))->getUpper()<<"\t"<<"lower = "<<((CAtom*)(*sen_list2_it))->getLower();
		    }*/

		
		sensitivityList = createSenList(sen_list1, EntName);

		//list<CSenNode*> senNodeList = createSenNodeList(sensitivityList, n,EntName);

		list<CSenNode*> senNodeList = createSenNodeListForProcess(sensitivityList, n, EntName, sen_list2);
		
		CArchStmt * arch_stmt = new CArchStmt(PROCESS_STMT, NULL, *iNodes, NULL);
		
		n->addStmt(arch_stmt);
		
		nodes.push_back(n);
	}	
}


CPort& CDependencyGraph::getPort(list<CPort> & ports, string name)
{
	list<CPort>::iterator iport;

	for(iport = ports.begin(); iport != ports.end(); iport++)
	{
		if((*iport).getName() == name)
		{
			return *iport;
		}
	}	
}


void CDependencyGraph::createGraphForInstances(list<CInstance *> &instanceNodes, string EntName)
{
	int p = 0;
	list<CInstance *>::iterator iNodes = instanceNodes.begin();

	//cout<<"--------------------------"<<endl;
	//cout<<instanceNodes.size()<<endl;
	//cout<<"--------------------------"<<endl;

	for(	iNodes = instanceNodes.begin();
		iNodes != instanceNodes.end();
		iNodes++
		)
	{
		list<CExpression*> params= (*iNodes)->getActualParam();
		list<CExpression*> ::iterator iparam= params.begin();

		list<COperand*> formals= (*iNodes)->getFormalParam();
		list<COperand*> ::iterator iformal = formals.begin();

//		fprintf(fp, "param%d [shape=record,label=\"",p);

		list<CSensitiveElement*> sensitivityList;
		list<CSensitiveElement*> lhsList;
		
		list<CPort> &ports = Entities[(*iNodes)->getIndex()].getPorts(); 

		list<CPort>::iterator iport;
	
		if(formals.size() != 0)
		{
			
			for(iparam = params.begin();
				iparam != params.end();
				iparam++,iformal++)
			{
				//cout<<(*iformal)->getName()<<endl;
				CPort& port = getPort(ports, (*iformal)->getName());
				if(port.getMode() == t_IN || port.getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(sensitivityList);
				}

				if(port.getMode() == t_OUT || port.getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(lhsList);
				}
			}
		}
		else
		{
			iport = ports.begin();
			for(iparam = params.begin();
				iparam != params.end();
				iparam++,iport++)
			{
				if((*iport).getMode() == t_IN || (*iport).getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(sensitivityList);
				}
				
				if((*iport).getMode() == t_OUT || (*iport).getMode() == t_INOUT)
				{
					(*iparam)->getSensitivityList(lhsList);
				}
				
			}

		}
		
		node *n = new node();
		
		list<CSenNode*> senNodeList = createSenNodeList(sensitivityList, n, EntName);
		
		list<CSensitiveElement*> ::iterator ilhs = lhsList.begin();
		set<string> setDone;
		for(; ilhs != lhsList.end(); ilhs++)
		{
			//cout<<"output = "<< (*ilhs)->name <<endl;
			//CSensitiveElement se_obj = ;
			
			list<node *> nn;
			nn.push_back(n);
			
			affected.insert(make_pair(*(*ilhs), nn));
			//affected[*(*ilhs)].push_back(n);
			nodeIdentifiers[n].insert((*ilhs)->name);
			
			if(SymbolTable[EntName][(*ilhs)->name].id_type == "port::out")
			{
				n->end = true;
			}
			
			if(SymbolTable[EntName][(*ilhs)->name].id_type == "port::in")
			{
				n->start = true;
			}
			
			if(setDone.find((*ilhs)->name + (*ilhs)->upper + (*ilhs)->lower) == setDone.end())
			{
				n->output_identifiers.insert(*ilhs);
				setDone.insert((*ilhs)->name + (*ilhs)->upper + (*ilhs)->lower);
			}
			
		}
		
		CArchStmt * arch_stmt = new CArchStmt(COMPONENT_INSTANTIANTION_STMT, NULL, NULL, *iNodes);
		
		n->addStmt(arch_stmt);
		nodes.push_back(n);
	}	
}

list<CBasicBlock*> & CDependencyGraph::getBasicBlocks()
{
	return  basicblocks;
}

map<string, list<int> >& CDependencyGraph::getAtoms()
{
	return atoms;
}


// Function to check whether node 'first' triggers node 'second'

bool CDependencyGraph::Triggering(node *first, node *second, string EntName)

{
  // Finding outputs of first node

set<CSensitiveElement*> affected_first = first -> output_identifiers;

 // Finding sensitity list of second node

list<CSensitiveElement*> sensitivityList;

switch(second->arch_stmt->getType())
		{
			case CONCURRENT_ASSIGNMENT_STMT:
				 getSensitivityListConAssignStmt(second->arch_stmt->stmt.conc_assign_stmt, sensitivityList);
				 break;

			case PROCESS_STMT:
			        getSensitivityListProcessStmt(second->arch_stmt->stmt.process_stmt, sensitivityList, EntName);
				break;

			case COMPONENT_INSTANTIANTION_STMT:
			        getSensitivityListCompInstStmt(second->arch_stmt->stmt.instance_stmt, sensitivityList);
				break;
		}

 set<CSensitiveElement*>::iterator affected_first_it = affected_first.begin();

 bool flag = false;
 
 for(;affected_first_it != affected_first.end(); affected_first_it++)
   {
     list<CSensitiveElement*>::iterator sensitivityList_it = sensitivityList.begin();

     for(;sensitivityList_it != sensitivityList.end(); sensitivityList_it++)
       {
	 if( ((*affected_first_it)->name == (*sensitivityList_it)->name) &&  ((*affected_first_it)->upper == (*sensitivityList_it)->upper) && ((*affected_first_it)->lower == (*sensitivityList_it)->lower) ) 
	   {flag = true;break;}
       }
	
     if(flag == true) break;
   }
	   
 return flag;
} 


list<node *> CDependencyGraph::arrangeInTriggeredOrder(list<node *> originalNodes, string EntName)
{
  
  list<node *> sortedNodes; 
  
  list<node *> ::iterator iOrigNode = originalNodes.begin();

  sortedNodes.push_back(*iOrigNode);

  iOrigNode = originalNodes.erase(iOrigNode);
  
  list<node *> ::iterator iSortedNode = sortedNodes.begin();

  
  while(originalNodes.begin() != originalNodes.end())
    {
      list<node *> ::iterator iOrigNode = originalNodes.begin();

      bool flag = false;

      while(iOrigNode != originalNodes.end())
	
	{
    
	  if(Triggering((*iSortedNode),(*iOrigNode),EntName))
	
	    {
	      sortedNodes.push_back(*iOrigNode);
	      iOrigNode = originalNodes.erase(iOrigNode);
	      flag = true;
	      break;
	    }
	  else
	    {
	      iOrigNode++;
	    }
	}

      if(flag == false) break;
      
      iSortedNode++;
    }

  if(originalNodes.begin() != originalNodes.end())

    {
      cout<<"\n\nERROR! THERE EXISTS SOME STATEMENT WHICH IS NEVER TRIGGERED...EXITING\n\n";
      exit(1);
    }

  return sortedNodes;	  
  
}

void CDependencyGraph::arrangeProcessInBBInTriggeredOrder(string EntName)
{

list<CBasicBlock*>::iterator  basicblocks_it;

 //cout<<"\nBBs and nodes inside them before rearranging\n";

 for(basicblocks_it = basicblocks.begin(); basicblocks_it != basicblocks.end(); basicblocks_it++)

   {
     //cout<<"\nBasic Block\n";
     list<node *> originalNodes = (*basicblocks_it)->block_nodes;
     list<node*>:: iterator it;

     for(it=originalNodes.begin(); it!=originalNodes.end(); it++)
	{
        //displayNode(*it);
	}
   }

 for(basicblocks_it = basicblocks.begin(); basicblocks_it != basicblocks.end(); basicblocks_it++)

   {
     (*basicblocks_it)->block_nodes = arrangeInTriggeredOrder((*basicblocks_it)->block_nodes, EntName);
   }

 //cout<<"\nBBs and nodes inside them after rearranging\n";

 for(basicblocks_it = basicblocks.begin(); basicblocks_it != basicblocks.end(); basicblocks_it++)

   {
     //cout<<"\nBasic Block\n";
     list<node *> originalNodes = (*basicblocks_it)->block_nodes;
     list<node*>:: iterator it;

     for(it=originalNodes.begin(); it!=originalNodes.end(); it++)
	{
        //displayNode(*it);
	}
   }
 
}

void CDependencyGraph::displayNode(node *x)
{

switch(x->arch_stmt->getType())
		{
			case CONCURRENT_ASSIGNMENT_STMT:
			         cout<<"\nCONCURRENT_ASSIGNMENT_STMT\n";
			         x->arch_stmt->stmt.conc_assign_stmt->getLHS()->print();
				 break;

			case PROCESS_STMT:
			        cout<<"\nProcess Statement\n";
				break;

			case COMPONENT_INSTANTIANTION_STMT:
			        cout<<"\nComp Inst Statement\n";
				break;
		}
}



