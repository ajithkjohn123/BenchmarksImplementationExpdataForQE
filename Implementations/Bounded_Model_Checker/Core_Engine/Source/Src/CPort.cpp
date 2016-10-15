#include "CPort.h"
#include "y.tab.h"

CPort::CPort()
{
}
void CPort::setName(string p_name)
{
	name = p_name;
}

void CPort::setMode(int p_mode)
{
    mode = p_mode;
}

void CPort::setUpper(list<string> &p_upper)
{
    upperbound.splice(upperbound.begin(), p_upper);
}
void CPort::setLower(list<string> &p_lower)
{
    lowerbound.splice(lowerbound.begin(), p_lower);
}
void CPort::setType(string p_type)
{
    type = p_type;
}

void CPort::setValue(CExpression* p_value)
{
	value = p_value;
}

string CPort::getName()
{
	return name;
}

CExpression*  CPort::getValue()
{
	return value;
}

int CPort::getMode()
{
	return mode;
}

list<string> CPort::getUpper()
{
	return upperbound;
}

list<string> CPort::getLower()
{
	return lowerbound;
}

string CPort::getType()
{
	return type;
}
string CPort::getModeStr()
{
	switch(mode)
	{
		case t_IN:
			return "in";
		break;

		case t_OUT:
			return "out";
		break;

		case t_INOUT:
			return "inout";
		break;

		case t_BUFFER:
			return "buffer";
		break;

		case t_LINKAGE:
			return "linkage";
		break;
	}
}

string CPort::getStart()
{
	if(lowerbound.size() != 0)
	{
		return (*lowerbound.begin());
	}
	else
	{
		return "0";
	}
}

string CPort::getEnd()
{
	if(lowerbound.size() != 0)
	{
		return (*upperbound.begin());
	}
	else
	{
		return "0";
	}
//	return (*upperbound.begin());
}


