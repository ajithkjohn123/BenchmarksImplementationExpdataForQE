#include "CHelper.h"

int main()
{
  	CHelper MANAGER;

	// for (x_0=0b001)
	CDAGNode *x0 = MANAGER.createDAG("x", 0);
	CDAGNode *y0 = MANAGER.createDAG("y", 0);
  	CDAGNode *e0 = MANAGER.createDAG("=", x0, y0);
///*
	//for X[5](x_0=0b001)

	CDAGNode *i0 = MANAGER.createDAG("5");
	CDAGNode *f0 = MANAGER.createDAG("X",i0 ,e0);
//*/
//
/*
	//for G(x_0=0b001)[5]

	CDAGNode *i0 = MANAGER.createDAG("5");
	CDAGNode *f0 = MANAGER.createDAG("G",e0 ,i0);
//
*/
//
/*
	//for F(x_0=0b001)[5]

	CDAGNode *i0 = MANAGER.createDAG("5");
	CDAGNode *f0 = MANAGER.createDAG("F",e0 ,i0);
//
*/
	//for printing in string
	cout<<"original formula : "<<f0->getDAGStr()<<endl;
	MANAGER.printGraphInDotFormat(f0, "formula0.dot");

	//for calling for unrolling
	CDAGNode *f1 = MANAGER.callUnrolledClone(f0);

	//for printing in string
	cout<<"modified formula : "<<f1->getDAGStr()<<endl;
	MANAGER.printGraphInDotFormat(f1, "formula1.dot");

	return 0;
}
