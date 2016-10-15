#include "CPrePostProcessor.h"

string CurrentEntityName;

void CPrePostProcessor::setCompInst()
{
	for(int i=0; i<Entities.size();i++)
	{
		CEntity &ent = Entities[i];

		list<CInstance *> instanceNodes = ent.getInstances();
		list<CInstance *>::iterator iNodes = instanceNodes.begin();

		for(	iNodes = instanceNodes.begin();
			iNodes != instanceNodes.end();
			iNodes++
		   )
		{
			int index = CParser::getEntity((*iNodes)->getEntName());
			(*iNodes)->setIndex(index);
		}
	}
}

void CPrePostProcessor::AdjustRangesInstances(list<CInstance *> &instanceNodes)
{
	list<CInstance *>::iterator iNodes = instanceNodes.begin();

	// New code added

	list<CInstance *>::iterator iNodes2 = instanceNodes.begin();

	//cout <<"\n\nINSTANCE NODES AND THEIR ACTUAL PARAMETERS BEFORE POST PROCESSING\n\n";

	for(	iNodes2 = instanceNodes.begin();
		iNodes2 != instanceNodes.end();
		iNodes2++
		)
	{
	        //cout <<"\nPARAMETERS\n";
		list<CExpression*> params= (*iNodes2)->getActualParam();
		list<CExpression*> ::iterator iparam= params.begin();

		for(	iparam = params.begin();
			iparam != params.end();
			iparam++
		)
		{
		  set<string> setRelation;
		  //cout<<(*iparam)->getDAG("",0,setRelation)->getDAGStr()<<endl;
		}
	}

	//cout<<endl<<endl;

	// New code added terminated


	for(	iNodes = instanceNodes.begin();
		iNodes != instanceNodes.end();
		iNodes++
		)
	{
	        
		list<CExpression*> params= (*iNodes)->getActualParam();
		list<CExpression*> ::iterator iparam= params.begin();

		for(	iparam = params.begin();
			iparam != params.end();
			iparam++
		)
		{
		  (*iparam)->getAndAdjustExpression(CurrentEntityName);
		}
	}

		iNodes = instanceNodes.begin();

	//cout <<"\n\nINSTANCE NODES AND THEIR PARAMETERS\n\n";

	for(	iNodes = instanceNodes.begin();
		iNodes != instanceNodes.end();
		iNodes++
		)
	{
	        //cout <<"\nPARAMETERS\n";
		list<COperand*> params= (*iNodes)->getFormalParam();
		list<COperand*> ::iterator iparam= params.begin();

		for(	iparam = params.begin();
			iparam != params.end();
			iparam++
		)
		{
		  //(*iparam)->print();
		}
		}
}


void CPrePostProcessor::AdjustRangesConSignAssgn(list<CConSignAssgnStmt *> &conAssignNodes)
{
	list<CConSignAssgnStmt *>::iterator iNodes = conAssignNodes.begin();

	for(	iNodes = conAssignNodes.begin();
		iNodes != conAssignNodes.end();
		iNodes++
		)
	{
		COperand *lhs = (*iNodes)->getLHS();

		list<CExpression*> values = (*iNodes)->getValues();
		list<CExpression*> choices = (*iNodes)->getChoices();

		list<CExpression*> ::iterator ichoice = choices.begin();
		list<CExpression*> ::iterator ivalue = values.begin();

		int numberofbits = 0;
		for(	;
			ichoice != choices.end();
			ichoice++,ivalue++
		)
		{
			// Added by Ajith On 12 Sep '13
			//cout<<"\nChoice \n";
			//(*ichoice)->print(0);
			//cout<<"\nValue \n";
			//(*ivalue)->print(0);

			//cout<<"\nCalling getAndAdjustExpression on choice\n";
			// Added by Ajith On 12 Sep '13 ends here

			(*ichoice)->getAndAdjustExpression(CurrentEntityName);

			numberofbits = (*ivalue)->getAndAdjustExpression(CurrentEntityName);
		}

		if(numberofbits != -1)
		  {
		
		    if(lhs->unAdjusted())
		      {       
		        //cout<<"\n\nInside numberofbits != -1 && lhs->unAdjusted()\n\n";
  
			lhs->setLowerBits(numberofbits);
		      }
		  }
		else
		  {
		    //cout<<"\n\nInside NOT(numberofbits != -1 && lhs->unAdjusted())\n\n";
		    
		    lhs->adjustRange(CurrentEntityName);
		  }
	}
}

void CPrePostProcessor::AdjustRangesProcesses(list<CProcess *>& processNodes)
{
	list<CProcess *>::iterator iNodes = processNodes.begin();

	for(	iNodes = processNodes.begin();
		iNodes != processNodes.end();
		iNodes++
		)
	{
		if((*iNodes)->getFirst() != NULL)
		{
			//cout<<"***Process stmt***"<<endl;
			(*iNodes)->getFirst()->print();
			(*iNodes)->getFirst()->adjustStmt(CurrentEntityName);
		}
		
		list<COperand*> & senList = (*iNodes)->getSenList();
		list<COperand*>  ::iterator isen;

		for(isen = senList.begin();
			isen != senList.end();
			isen++)
		{
			(*isen)->adjustRange(CurrentEntityName);
		}		
	}	
}


void CPrePostProcessor::AdjustRanges()
{
	for(int i=0; i<Entities.size();i++)
	{
		CEntity &ent = Entities[i];

		CurrentEntityName =  ent.getName();
		//cout<<"Processing entity"<<CurrentEntityName;

		list<CInstance *> &instanceNodes = ent.getInstances();
		AdjustRangesInstances(instanceNodes);
		//cout<<"Instances"<<instanceNodes.size()<<endl;

		list<CConSignAssgnStmt *> &conAssignNodes = ent.getConAssgnStmt();
		AdjustRangesConSignAssgn(conAssignNodes);
		//cout<<"conAssignNodes"<<conAssignNodes.size()<<endl;

		list<CProcess *> &processNodes = ent.getProcesses();
		AdjustRangesProcesses(processNodes);
		//cout<<"processed"<<processNodes.size()<<endl;
	}
}

void CPrePostProcessor::postprocess()
{
	setCompInst();
	//cout<<"done postprocess"<<endl;
	AdjustRanges();
	//cout<<"done AdjustRanges"<<endl;
}

void CPrePostProcessor::readEntArchMap(char *filename, map<string, string> &EntArch)
{
	FILE *fp = fopen(filename, "r");

	if (fp==NULL)
	{
		cout<<"unable to open mapping file"<<endl;
	}    


	char lin[80];

// 	cout<<endl;
// 
// 	fgets ( lin, 80, fp);
// 	string line1 = lin;
// 	cout<<"found string " <<line1<<endl;


	while( (fgets ( lin, 80, fp)) != NULL)
	{
		string line = lin;
		//cout<<"found string " <<line<<"***********"<<endl;
		line = line.substr(0, line.length() - 1);
		
		int index = line.find("#");
//		cout<<"found at index " <<index<<endl;
		
		string entity = line.substr(0, index);
		string arch   = line.substr(index + 1);
		EntArch[entity] = arch;
		//cout<<"arch is " <<arch<<"***********"<<endl;
		//cout<<"--------------------------"<<endl;
		//cout<<entity<<"**"<<arch<<endl;
		//cout<<"--------------------------"<<endl;
	}

	map<string, string>::iterator iter = EntArch.begin();

	while(iter != EntArch.end())
	{
		//cout<< iter->first<<"--"<<iter->second <<endl;
		iter++;
	}
// 	cout<<"enter2"<<endl;
// 	int hui;
// 	cin>>hui;

	
}


void CPrePostProcessor::creatInstances(FILE *fp, list<CInstance *> &instanceNodes)
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
		//cout<<1<<endl;
		fprintf(fp, "cis%d [shape=record,label=\"%s| %s | %d\"];\n", p, (*iNodes)->getInstName().c_str(), (*iNodes)->getEntName().c_str(), 
		(*iNodes)->getIndex());

		if(p==0)
		{
			fprintf(fp, "Comp_Insts -> cis%d;\n", p);
		}
		else
		{
			fprintf(fp, "cis%d -> cis%d;\n", p-1, p);
		}

		list<CExpression*> params= (*iNodes)->getActualParam();
		list<CExpression*> ::iterator iparam= params.begin();

		list<COperand*> formals= (*iNodes)->getFormalParam();
		list<COperand*> ::iterator iformal = formals.begin();

		fprintf(fp, "param%d [shape=record,label=\"",p);

		if(formals.size() != 0)
		{
			for(	iparam = params.begin();
				iparam != params.end();
				iparam++,iformal++
			)
			{
				fprintf(fp, "%s=>%s|", (*iformal)->getOper().c_str(),
								(*iparam)->getOrgExpr().c_str()) ;
			}
		}
		else
		{
			for(	iparam = params.begin();
				iparam != params.end();
				iparam++
			)
			{
				fprintf(fp, "%s|", (*iparam)->getOrgExpr().c_str()) ;
			}

		}
		fprintf(fp, "\"];\n");

		fprintf(fp, "cis%d -> param%d;\n", p, p);

		p++;
	}
}

void CPrePostProcessor::creatConSignAssgn(FILE *fp, list<CConSignAssgnStmt *> &conAssignNodes)
{
	int p = 0;
	list<CConSignAssgnStmt *>::iterator iNodes = conAssignNodes.begin();

	for(	iNodes = conAssignNodes.begin();
		iNodes != conAssignNodes.end();
		iNodes++
		)
	{
		COperand *lhs = (*iNodes)->getLHS();
		fprintf(fp, "csa%d [label=\"%s|%s|%s\"];\n", p, (lhs->getName()).c_str(), (*iNodes)->getSelExpr().c_str(), (*iNodes)->getDelay().c_str());

		if(p==0)
		{
			fprintf(fp, "ConSignAssgns -> csa%d;\n", p);
		}
		else
		{
			fprintf(fp, "csa%d -> csa%d;\n", p-1, p);
		}

		list<CExpression*> values = (*iNodes)->getValues();
		list<CExpression*> choices = (*iNodes)->getChoices();

		list<CExpression*> ::iterator ichoice = choices.begin();
		list<CExpression*> ::iterator ivalue = values.begin();
		//cout<<"you are here... sizes "<<choices.size() <<"  "<<values.size() <<endl;

		int q=0;
		for(	;
			ichoice != choices.end();
			ichoice++,ivalue++
		)
		{
			fprintf(fp, "choicevalue%d%d [shape=record,label=\"%s|%s\"];\n",p,q,(*ichoice)->getOrgExpr().c_str(),(*ivalue)->getOrgExpr().c_str());
			if(q==0)
			{
				fprintf(fp, "csa%d -> choicevalue%d%d;\n", p, p, q);
			}
			else
			{
				fprintf(fp, "choicevalue%d%d -> choicevalue%d%d;\n", p, q-1, p, q);
			}
			q++;
			//cout<<endl;
		}

		p++;
	}
}

void CPrePostProcessor::creatProcesses(FILE *fp, list<CProcess *> &processNodes)
{
	int p = 0;
	list<CProcess *>::iterator iNodes = processNodes.begin();

	for(	iNodes = processNodes.begin();
		iNodes != processNodes.end();
		iNodes++
		)
	{
		fprintf(fp, "process%d;\n", p);

		fprintf(fp, "Processes -> process%d;\n", p);

		list<COperand*> & senList = (*iNodes)->getSenList();
		list<COperand*>  ::iterator isen;

		fprintf(fp, "senlist%d [shape=record,label=\"",p);

		for(	isen = senList.begin();
			isen != senList.end();
			isen++
		)
		{
			fprintf(fp, "%s(%s:%s)|", (*isen)->getName().c_str(), 
										((CAtom*)(*isen))->getLower().c_str(), 
										((CAtom*)(*isen))->getUpper().c_str()) ;
		}

		fprintf(fp, "\"];\n");

		fprintf(fp, "process%d -> senlist%d;\n", p, p);

		char parent[50];
		sprintf(parent, "senlist%d", p);

		if((*iNodes)->getFirst() != NULL)
		{
			(*iNodes)->getFirst()->createNode(fp,parent);
		}

// 		int q=0;
// 		for(	;
// 			isen != senList.end();
// 			isen++,ivalue++
// 		)
// 		{
// 			fprintf(fp, "choicevalue%d%d [shape=record,label=\"%s|%s\"];\n",p,q,(*ichoice)->getOrgExpr().c_str(),(*ivalue)->getOrgExpr().c_str());
// 			if(q==0)
// 			{
// 				fprintf(fp, "csa%d -> choicevalue%d%d;\n", p, p, q);
// 			}
// 			else
// 			{
// 				fprintf(fp, "choicevalue%d%d -> choicevalue%d%d;\n", p, q-1, p, q);
// 			}
// 			q++;
// 		}

		p++;
	}
}

void CPrePostProcessor::creatports(FILE *fp, list<CPort>& ports)
{
	list<CPort>::iterator iport;

	int p=0;
	for(iport = ports.begin();
		iport != ports.end();
		iport++
		)
	{

		fprintf(fp, "port%d [shape=record,label=\"%s| %s | %s | %s | %s| %s\"];\n", p, iport->getName().c_str(), iport->getType().c_str(), 
		iport->getModeStr().c_str(), iport->getStart().c_str(), iport->getEnd().c_str(), iport->getValue()==NULL?"":(iport->getValue()->getOrgExpr()).c_str());
		if(p==0)
		{
			fprintf(fp, "Ports -> port%d;\n", p);
		}
		else
		{
			fprintf(fp, "port%d -> port%d;\n", p-1, p);
		}
		p++;
	}
}

void CPrePostProcessor::createGraph(char *outprefix)
{
	for(int i=0; i<Entities.size();i++)
	{
		CEntity &ent = Entities[i];

		char rootName[25];
		sprintf(rootName, "%s_%d", ent.getName().c_str(), i);

		string filename = outprefix;
		filename = filename + ent.getName() + ".dot";

		FILE *fp = fopen(filename.c_str(), "w");

		fprintf(fp, "digraph Entity%d {\n", i);
		fprintf(fp, "%s [shape=tripleoctagon,color=lightblue,style=filled];\n", rootName);

		fprintf(fp, "Ports[shape=box,peripheries=2];\n");
		fprintf(fp, "ConSignAssgns[shape=box,peripheries=2];\n");
		fprintf(fp, "Processes[shape=box,peripheries=2];\n");
		fprintf(fp, "Comp_Insts[shape=box,peripheries=2];\n");

		fprintf(fp, "%s -> Ports;\n", rootName);
		fprintf(fp, "%s -> ConSignAssgns;\n", rootName);
		fprintf(fp, "%s -> Processes;\n", rootName);
		fprintf(fp, "%s -> Comp_Insts;\n", rootName);

		list<CPort> &ports = ent.getPorts();
		creatports(fp, ports);
		//cout<<ports.size()<<endl;

		list<CInstance *> &instanceNodes = ent.getInstances();
		creatInstances(fp, instanceNodes);
		//cout<<"instanceNodes"<<instanceNodes.size()<<endl;

		list<CConSignAssgnStmt *> &conAssignNodes = ent.getConAssgnStmt();
		creatConSignAssgn(fp, conAssignNodes);
		//cout<<"conAssignNodes"<<conAssignNodes.size()<<endl;

		list<CProcess *> &processNodes = ent.getProcesses();
		creatProcesses(fp, processNodes);
		//cout<<"processNodes"<<processNodes.size()<<endl;

		fprintf(fp, "}");

		fclose(fp);
	}

}

void CPrePostProcessor::printSymbolTable()
{
	map <string, map<string, SymTabEntry> >::iterator ist;
	
	for (ist=SymbolTable.begin(); ist != SymbolTable.end(); ++ist) 
	{
		//cout<<endl;
		//cout << "***********************************"<< endl;
        	//cout << "Name of Entity: "<< ist->first << endl;
		//cout << "************************************"<< endl;

		map<string, SymTabEntry>::iterator ien;
		for (ien = (ist->second).begin(); ien != (ist->second).end(); ++ien) 
		{
			//cout << "Name: "<< ien->first << endl;
			//cout << "ID type: " << ien->second.id_type<<endl;
			//cout << "Data type: " << ien->second.data_type<<endl;
			string value = ien->second.value==NULL?"":ien->second.value->getOrgExpr();
			//cout << "Value: " << value <<endl;
			//cout << "Range: " << endl;
			list<string>::iterator ir;
			for (ir = (ien->second).rangeindex.begin(); ir != (ien->second).rangeindex.end(); ++ir) 
			{
				//cout<<" " << (*ir);
			}
			//cout <<endl<< "-----------------------------------"<< endl;
		}

		//cout << "***********************************"<< endl;
	}
}

