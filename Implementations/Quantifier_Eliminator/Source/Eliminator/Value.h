/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/

#ifndef _VALUE_H
#define	_VALUE_H

#include "Oprs.h"
#include "macros.h"	

using namespace std;
class Value
{
    public:

	void set_unique_node_identifier(string);
	string get_unique_node_identifier();

	void set_node_type(Type);
	Type get_node_type();

	void set_node_result(Result);
	Result get_node_result();

	//constructors
	Value();
	Value *copy_value();
	~Value();
	
    private:

	//new field of the particular DAG node......
	string unique_node_identifier;
	Type node_type;
        Result node_result;

};


#endif	/* _VALUE_H */

