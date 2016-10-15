#ifndef __CID_H
#define __CID_H

#include <vector>
#include <string>
#include "COperand.h"

using namespace std;


class CIdentifier:public COperand
{
	string name;
public:
	CIdentifier();
	~CIdentifier();
};

#endif
