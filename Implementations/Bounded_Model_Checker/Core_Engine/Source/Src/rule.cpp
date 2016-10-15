#include "CExprSimpStructure.h"

void Rule::print()
{
	return;
	cout<<"**LHS**"<<endl;
	lhs->print();
	cout<<"**RHS**"<<endl;
	rhs->print();
}


void Term::print()
{
	return;
	cout<<"Function name::"<<function_name<<endl;
	cout<<"Id or Value::"<<id_or_value<<endl;
	
	list<argument *>::iterator iarg;
	int i=1;
	for(iarg= args.begin() ; iarg!= args.end() ;iarg++)
	{
		cout<<"Arg"<<i<<"::"<<endl;
		argument* x=*iarg;
		x->print();  

		i++;
	}
}


string Term::get_string()
{
	string temp = function_name;

	if(id_or_value != "")
	{
		temp = temp + "_" + id_or_value;
	}
	
	list<argument *>::iterator iarg;
	for(iarg= args.begin() ; iarg!= args.end() ;iarg++)
	{
		argument* x=*iarg;

		temp = temp + "," + x->get_string();
	}

	return temp;
}

bool AffectedNodes::include(AffectedNodes *af)
{
	//cout<<1<<endl;
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> >::iterator inode1 = afEdges.begin();
	list < pair<struct CExprSimpNode*, struct CExprSimpNode *> >::iterator inode2 = af->afEdges.begin();

	//cout<<2<<endl;
	for( ; inode1 != afEdges.end(); inode1++)
	{
		//cout<<3<<endl;
		inode2 = af->afEdges.begin();
		for( ; inode2 != af->afEdges.end(); inode2++)
		{
			//cout<<4<<endl;
			if((*inode1).first == (*inode2).first &&(*inode1).second == (*inode2).second )
			{
				//cout<<"hiiiiiiiiiiiii"<<endl;
			}

			if(*inode1 == *inode2)
			{
				return true;
			}
		}
	}

	inode1 = lhsEdges.begin();
	inode2 = af->afEdges.begin();

	for( ; inode1 != lhsEdges.end(); inode1++)
	{
		//cout<<5<<endl;
		inode2 = af->afEdges.begin();
		for( ; inode2 != af->afEdges.end(); inode2++)
		{
			//cout<<6<<endl;
			if((*inode1).first == (*inode2).first &&(*inode1).second == (*inode2).second )
			{
				//cout<<"byeeeeeeeee"<<endl;
			}

			if(*inode1 == *inode2)
			{
				return true;
			}
		}
	}


	return false;
}


