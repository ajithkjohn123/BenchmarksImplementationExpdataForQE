#ifndef __CPOSTPROCESS_H
#define __CPOSTPROCESS_H

#include <vector>
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


using namespace std;

#define t_Identifier 364

//extern map <string, <string, list<int> > > SymbolTable;

class CPrePostProcessor
{
public:
	static void process(vector<CEntity> entityList);

};

#endif
