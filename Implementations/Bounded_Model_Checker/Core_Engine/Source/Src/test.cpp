#include <list>
#include <string>
#include <iostream>

using namespace std;

class c1
{
	string str;
	int value;
public:
	c1(string s, int val)
	{
		str = s;
		value = val;
	}
	c1& operator=(c1& obj)
	{
		this->str = obj.str;
		this->value = obj.value;
		return *this;
	}
};

int main(void)

{

list<c1> l;
l.push_back(c1("abc1", 1));
l.push_back(c1("abc2", 2));
l.push_back(c1("abc3", 3));
l.push_back(c1("abc4", 4));

list<c1> m;

m=l;
list<c1>::iterator iter;

m.clear();

for (iter=l.begin(); iter!=l.end(); iter++)
{
	cout<<"  "<<(*iter);
}
cout<<endl;
for (iter=m.begin(); iter!=m.end(); iter++)
{
	cout<<"  "<<(*iter);
}
cout<<endl;
return(0);

}

