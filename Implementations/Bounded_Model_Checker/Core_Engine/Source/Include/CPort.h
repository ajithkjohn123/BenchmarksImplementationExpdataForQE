#ifndef __CPORT_H
#define __CPORT_H

#include <list>
#include <string>
#include "CExpression.h"

using namespace std;

/*
#define t_IN 289
#define t_INOUT 290
#define t_OUT 308
#define t_BUFFER 269
#define t_LINKAGE 294 */

class CExpression;

class CPort
{
    string name;
    int mode;
    list<string> upperbound;
    list<string> lowerbound;
    string type;
    CExpression* value;

 public:
    void setName(string p_name);
    void setMode(int p_mode);
    void setUpper(list<string> &p_upper);
    void setLower(list<string> &p_lower);
    void setType(string p_type);
    void setValue(CExpression* p_value);

    CExpression* getValue();
    string getName();
    int getMode();
    string getModeStr();

    list<string>  getUpper();
    list<string>  getLower();
    string getType();

    string getStart();
    string getEnd();


    CPort();


};

#endif
