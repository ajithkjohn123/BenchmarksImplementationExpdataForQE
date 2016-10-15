/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/



#ifndef _DD_H
#define	_DD_H

    #include <list>
    #include <string>
    #include <iostream>
    #include <set>

    #include "Eliminator.h"
    #define MAX_AP 1000
    #define MAX_NODES 900000
    #define LMI_OFFSET MAX_AP/2


    using namespace std;

    class DDNode
    {
        public:
            ~DDNode();    //DESTRUCTOR...............
            DDNode(int new_val);  //constructor.....with 1 argument (for terminal node)
            DDNode(DAGNode *new_predicate, DDNode *new_left, DDNode *new_right); 
            int getID();
	    void setID(int);
	    int getVal();
	    void setVal(int);
	    bool getMark();
	    void setMark(bool);
	    DAGNode* getPredicate();
	    void setPredicate(DAGNode*);
	    DDNode* getLeft();
	    void setLeft(DDNode*);
	    DDNode* getRight();
	    void setRight(DDNode*);
	    string getDDString();

        private:
         	int id;
		int val;
		DAGNode* predicate;
		bool mark;
		DDNode* left;
		DDNode* right;		
    };


#endif	/* _DDNODE_H */

