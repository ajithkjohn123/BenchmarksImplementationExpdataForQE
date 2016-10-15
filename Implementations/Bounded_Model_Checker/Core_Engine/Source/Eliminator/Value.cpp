/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


	#include "Value.h"

	using namespace std;

	Value::Value()
	{
		unique_node_identifier = " ";

		node_type.data_type = UNKNOWN_DATA_TYPE;
		node_type.width = IMMATERIAL_WIDTH;

		node_result.type_of_value.data_type = UNKNOWN_DATA_TYPE;
		node_result.type_of_value.width = IMMATERIAL_WIDTH;
	}

	Value:: ~Value()
	{
		//empty destructor...............
	}

	Value* Value::copy_value()
	{
		Value *copied_val = new Value();
		copied_val->set_unique_node_identifier(unique_node_identifier);
		copied_val->set_node_type(node_type);
		copied_val->set_node_result(node_result);
		return copied_val;
	}

	void Value::set_unique_node_identifier(string id)
	{
		unique_node_identifier = id;
	}

	string Value::get_unique_node_identifier()
	{
		return unique_node_identifier;
	}

	void Value::set_node_type(Type nd_type)
	{
		node_type = nd_type;
	}

	Type Value::get_node_type()
	{
		return node_type;
	}

	void Value::set_node_result(Result nd_result)
	{
		node_result = nd_result;
	}

	Result Value::get_node_result()
	{
		return node_result;
	}

