#ifndef __CPOSTPROCESS_H
#define __CPOSTPROCESS_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include "CPort.h"
#include "CAtom.h"
#include "CLiteral.h"
#include "structure.h"
#include "idinfo.h"
#include "CSeqStmt.h"
#include "CEntity.h"
#include "CParser.h"

#include "stdio.h"

#include "CConSignAssgnStmt.h"
#include "CInstance.h"
#include "CProcess.h"




using namespace std;

class CPort;
class CEntity;
class CProcess;

/*#define t_Identifier 372 */

extern map <string, map<string, SymTabEntry> > SymbolTable;

extern vector<CEntity> Entities;



class CPrePostProcessor
{
	static void AdjustRangesConSignAssgn(list<CConSignAssgnStmt *> &conAssignNodes);
	static void AdjustRangesInstances(list<CInstance *> &instanceNodes);
	static void AdjustRangesProcesses(list<CProcess *>& processNodes);

	static void creatInstances(FILE *fp, list<CInstance *> &instanceNodes);
	static void creatConSignAssgn(FILE *fp, list<CConSignAssgnStmt *> &conAssignNodes);
	static void creatProcesses(FILE *fp, list<CProcess *> &processNodes);
	static void creatports(FILE *fp, list<CPort>& ports);

	static void setCompInst();
	static void AdjustRanges();

public:
	static void postprocess();
	static void readEntArchMap(char *filename, map<string, string> &EntArch);
	static void createGraph(char *outfile);
	static void printSymbolTable();
};

#endif
