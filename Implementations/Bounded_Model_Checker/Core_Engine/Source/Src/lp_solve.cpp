#include <vector>
#include <map>
#include <string>
#include <utility>

#include <iostream>
#include <fstream>

#include "CExprSimpStructure.h"


using namespace std;



// class gr_node
// {
// public:
// 	int index;
// 	int cost;
// 	gr_node():cost(0),index(-1){}
// 	gr_node(int ind, int t):cost(t),index(ind){}
// };

extern vector<gr_node*> ISNodes;
extern map< pair<int,int>, bool> ISEdges;

// void init()
// {
// // 	n = new gr_node(0,400);/* 0 */
// // 	ISNodes.push_back(n);
// 
// 	ISNodes.clear();
// 
// 	ISEdges.clear();
// 
// 	gr_node* n = new gr_node(0,5, NULL);/* 1 */
// 	ISNodes.push_back(n);
// 
// 	n = new gr_node(1,2, NULL);/* 2 */
// 	ISNodes.push_back(n);
// 	
// 	n = new gr_node(2,4, NULL);/* 3 */
// 	ISNodes.push_back(n);
// 
// 	n = new gr_node(3,5, NULL);/* 4 */
// 	ISNodes.push_back(n);
// 
// 	n = new gr_node(4,10, NULL);/* 5 */
// 	ISNodes.push_back(n);
// 
// // 	n = new gr_node(6,250);/* 6 */
// // 	ISNodes.push_back(n);
// 
// 	ISEdges.insert(make_pair(make_pair(1, 2), true) );
// 
// // 	ISEdges.insert(make_pair(make_pair(0, 2), true) );
// // 	ISEdges.insert(make_pair(make_pair(1, 2), true) );
// 	ISEdges.insert(make_pair(make_pair(2, 3), true) );
// 
// 	ISEdges.insert(make_pair(make_pair(2, 1), true) );
// // 	ISEdges.insert(make_pair(make_pair(2, 0), true) );
// // 	ISEdges.insert(make_pair(make_pair(2, 1), true) );
// 	ISEdges.insert(make_pair(make_pair(3, 2), true) );
// }

vector<gr_node*> result;

int lp_solve()
{
//	init();
	fstream file_op("ip.txt",ios::out);

	//cout<<"before"<<endl;

        file_op<<"max:  ";

	result.clear();

	int  length = ISNodes.size();

	for(int i=0;i<length-1;i++)
	{
		file_op<<" " << ISNodes[i]->cost << " x" << ISNodes[i]->index << "+";
	}

	file_op<<" "<<ISNodes[length-1]->cost << " x" << ISNodes[length-1]->index<<";";
	
	file_op<<endl;
	file_op<<endl;

	for(int i=0;i<length;i++)
	{
		file_op<<" " << " x" << ISNodes[i]->index << "  <= 1;"<<endl;
	}

	file_op<<endl;
	file_op<<endl;


	for(int i=0;i<length;i++)
	{
		for(int j=0;j<length;j++)
		{
			if(ISEdges.find( make_pair(i, j) ) != ISEdges.end())
			{
				file_op<<" " << " x" << ISNodes[i]->index << " + " << " x" << ISNodes[j]->index << "  <= 1;"<<endl;
			}
		}
	}

	ISNodes.clear();

	ISEdges.clear();

	file_op.close();

	system("~/lp_solve/lp_solve ip.txt > ip.out");

	fstream file_ip("ip.out",ios::in);

	while(!file_ip.eof())
	{
		char line[256];
		file_ip.getline(line, 255);

		if(line[0] == 'x' && line[strlen(line)-1] == '1')
		{
			char *num = &line[1];
			char *last = &line[1];

			while(*last != ' ') last++;

			*last = '\0';

			int index = atoi(num);
			//cout<<"  "<<index<<endl;

			result.push_back(ISNodes[index]);
		}
	}

	return 0;
}
