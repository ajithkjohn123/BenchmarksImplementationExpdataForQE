#ifndef CSENSITIVEELEMENT_H_
#define CSENSITIVEELEMENT_H_

#include <map>

using namespace std;

class CSensitiveElement
{
public:
	string name;
	string upper;
	string lower;

	CSensitiveElement()
	{
	}
	
	CSensitiveElement(string name, string upper, string lower)
	{
		this->name = name;
		this->upper = upper;
		this->lower = lower;		
	}	
	
	bool operator==(CSensitiveElement se)
	{
		if(	this->name == se.name &&
			this->upper == se.upper &&
			this->lower == se.lower)
		{
			return true;
		}
		
		return false;
	}
	
	void printCSensitiveElement()
	{
	  cout<<name<<"("<<lower<<","<<upper<<")";
	}
};



#endif /*CSENSITIVEELEMENT_H_*/
