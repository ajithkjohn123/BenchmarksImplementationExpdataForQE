
#include <list>
#include <string>
#include <iostream>
#include <set>


using namespace std;

class CDAGNode;

class ExistsNodesWithProfits
{
 public :
   
 CDAGNode *Address; //Address of the exists node in tree
 int Depth; // Depth of the exists node in the tree starting from the root of the tree

 // Let we have plucked out the sub tree with root from the exists node. First we find the depths of the exists nodes in the subtree after converting the subtree to dag. These depths are in   PreAbstractionDepths

 list<int> PreAbstractionDepths; 
 
 // Conversion of the subtree to dag is just for finding the depths. Now we apply the abstraction and equivalent reductions at the subtree. Again convert this to dag for finding the depths. These depths are in   PostAbstractionDepths

 list<int> PostAbstractionDepths;
 
       ExistsNodesWithProfits(CDAGNode *MyAddress, int MyDepth)
	{
	  Address = MyAddress;
	  Depth = MyDepth;
	}

 	ExistsNodesWithProfits(CDAGNode *MyAddress, int MyDepth, list<int> MyPreAbstractionDepths, list<int> MyPostAbstractionDepths)
	{
	  Address = MyAddress;
	  Depth = MyDepth;
	  PreAbstractionDepths.splice(PreAbstractionDepths.begin(),MyPreAbstractionDepths);
	  PostAbstractionDepths.splice(PostAbstractionDepths.begin(),MyPostAbstractionDepths);
	  
	}

	void displayExistsNodesWithProfits()
	{
	  list<int>::iterator PostAbstractionDepths_it;
	  list<int>::iterator PreAbstractionDepths_it;
	  
	  cout<<"\nAddress = "<<Address;
	  cout<<"\nDepth = "<<Depth;

	  cout<<"\nPreAbstractionDepths = ";

	  for(PreAbstractionDepths_it = PreAbstractionDepths.begin(); PreAbstractionDepths_it != PreAbstractionDepths.end(); PreAbstractionDepths_it++)
	    {cout<<(*PreAbstractionDepths_it)<<"\t";}

	  
	  cout<<"\nPostAbstractionDepths = ";

	  for(PostAbstractionDepths_it = PostAbstractionDepths.begin(); PostAbstractionDepths_it != PostAbstractionDepths.end(); PostAbstractionDepths_it++)
	    {cout<<(*PostAbstractionDepths_it)<<"\t";}

	}
};
