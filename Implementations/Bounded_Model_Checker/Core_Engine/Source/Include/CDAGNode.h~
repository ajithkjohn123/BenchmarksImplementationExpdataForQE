/* 
 * File:   CDAGNode.h
 * Author: sourabh
 *
 * Created on 13 May, 2009, 7:07 AM
 */

//prevent multiple inclusions of the header file......

#ifndef _CDAGNODE_H
#define	_CDAGNODE_H

    #include <list>
    #include <string>
    #include <iostream>
    #include <set>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <algorithm>
    #include <assert.h> // Line added by Ajith on 24 Sep 2013

    #include "CSensitiveElement.h"

    using namespace std;

typedef unsigned long long int ULINT;

    class CDAGNode
    {
        public:
            ~CDAGNode();    //DESTRUCTOR...............
            CDAGNode();  //constructor.....with empty argument
            CDAGNode(string);  //constructor.....with 1 argument (dag node name)
            CDAGNode(string , int);  //constructor.....with 2 argument (node name and delta-time )
            CDAGNode(string , CDAGNode*);  //constructor.....for unary operators
            CDAGNode(string , CDAGNode*, CDAGNode*);  //constructor.....for creating binary operator
            CDAGNode(string , CDAGNode*, CDAGNode*, CDAGNode*);  //constructor.....for creating ternary operators

            string Get_DAGnode_value(); //returns the value of the DAG node.....

            string Get_DAGnode_delta_time_as_string();    //returns the value of the delta time in form of string

            int Get_DAGnode_delta_time_as_int();    //returns the value of the delta time in form of int

            void Append_DAGnode_inlist(CDAGNode*);
            //assuming that the DAG nodes are being accessed by many DAG nodes and appending
            //the inlist one at a time..........

            void Delete_single_entry_from_DAG_inlist(CDAGNode*);
            //delete the entry passed as the parameter from the DAG inlist......
            
            unsigned int Get_DAGnode_outlist_size();    //returns the size of the DAGnode's outlist

            unsigned int Get_DAGnode_inlist_size();    //returns the size of the DAGnode's outlist

            list<CDAGNode*>::iterator Get_DAGnode_outlist();
            //returns an iterator pointing to the begining of the outlist

            list<CDAGNode*>::iterator Get_DAGnode_inlist();
            //returns an iterator pointing to the begining of the inlist

            void Set_DAGnode_delta_time(int);
            //sets the delta time of the dag node under consideration...

            string Print_DAGnode_attributes();
            //prints the node addributes of the DAG node under consideration.....

            bool Get_DAG_node_type();   //retuns the node type of the DAG node......
                                        //(i.e. operand node or operator node)
            bool equals(CDAGNode*);
            //Return true if two dags are exactly equal. i.e. each pair of nodes
            //have exactly the same data else return false - data includes the name, delta_time

            bool less(CDAGNode*);
            //Return true if dag1 < dag2. Take pair of nodes starting from the roots.
            //If node1<node2 return true. Else take the next pair of nodes in the d.f.s order.  -
            //data includes the name, delta_time, size of the lists

            pair<int, int> getLHSRange();
            //If the dag node is an array reference say A[lower to upper], make a pair (lower,higher)
            //and return it. Else return (-1, -1)

            void size(int &, int &);  //Get the size of the dag : size = no: of nodes ang get the no: of exists

	    void getSize(int &excnt, int &count, set<CDAGNode*> &Nodes);

            void findExistsDAGs(list<CDAGNode*> &);
            //Find the sub dags which appear as exists(List, dag) and return pointers to them

            string getDAGStr();   //displays the entire DAG.....

            void getDAGNames(set<string> &);
            //get the names of non existentially quantified variables in a DAG...........

            void getExistsDAGNames(set<string> &);
            //get the names of existentially quantified variables in a DAG...........

	    bool isroot();  //checks whether the given DAGnode is a root node or not

            void mark_redendant_node_for_deletion();
	    
	    void calculateDeltaDelays(map<CSensitiveElement*, string> &delays);

	    // FUNCTIONS FOR INTEGRATING WITH QELIMINATOR ADDED FROM 30/12/09 ONWARDS

	    string Get_DAGnode_data_as_id_delta(void);
	    bool freeOfVar(string VarToElim);
	    void getConstant(int &constant);
	    ULINT convertLiteralToNumber(string literal);
	    ULINT getCoefOfChar(char c);
	    ULINT findPower16(int power);
	    ULINT convertToDecimal(string x);
            void getCoefficient(string Var, ULINT &Coefficient);
            void getConstantModified(ULINT &constant);
            bool freeOfVarModified(string VarToElim);
	    string getDAGOfProperSkeltonFormatInYicesFormat(string context);
	    string getBinaryOfChar(char c);
	    string convertHexToBitvector(string hex);
	    string convertDecimalToBitvector(string dec);
	    string integerToBinaryString(ULINT i);
	    string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required);
	    string convertDecimalToBitvectorWithProperWidth(string dec, int Width);


	    // ON 11/1/2010 FOR CHANGES OF SYMBOLIC SIMULATOR TO EXECUTE ON BENCHMARKS
	    
	    void calculateDeltaDelaysModified(map<CSensitiveElement*, string> &delays);
	    void getVariableNameFromSelect(string &MyName, int &MyDelta);
	
	    // ON 19/1/2010 FOR CONVERTING STRINGS TO PROPER FORM

	    string convertHexToBitvectorWithProperWidth(string hex, int Width);
	    string hexToBinaryString(string hex);
	    string convertLiteralToBitvectorWithProperWidth(string literal, int Width);

	    
	    // FUNCTIONS FOR CONVERTING SUB NODES TO ADD NODES ON 20/1/2010

	    void getConstantModifiedWithMinusSupported(ULINT &constant, int Sign, ULINT Mod);
	    void getCoefficientWithMinusSupported(string Var, ULINT &Coefficient, int Sign, ULINT Mod);
	    ULINT additiveInverse(ULINT number, ULINT mod);
	    ULINT addWithModulo(ULINT a, ULINT b, ULINT mod);


	    void getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, FILE *yices_fp);
	    void getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, FILE *yices_fp, int shift);
	    


	    string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, set<string> &VarsToElimSet);
	    string convertDecimalToBitvectorWithProperWidthInSMTLibFormat(string dec, int Width);
	    string convertHexToBitvectorWithProperWidthInSMTLibFormat(string hex, int Width);
	    string convertLiteralToBitvectorWithProperWidthInSMTLibFormat(string literal, int Width);

	    string getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required);
	    string convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(string dec, int Width);
	    string convertHexToBitvectorWithProperWidthInNewSMTLibFormat(string hex, int Width);
	    string convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(string literal, int Width);


	    bool evaluateTrivialCondition();
	    bool evaluateClockCondition();
	    void getDAGNamesWithCheck(set<string> &names);
	    void getDAGNamesOfDAG_WithNoExists_WithDP(set<string> &names, set<CDAGNode*> *PtrDPTable);
	    string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, map<CDAGNode*, string> &DPTable);

	    string convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(string literal, int Width);


	    string getSingleVariable();

	    int getSizeOfConstant(string constant);
	    
	    string convertBitStringToBitvectorWithProperWidthInNewSMTLibFormat(string constant, int width);

	    string binaryStringToInteger(string str);

	    //void Set_id(ULINT MyId);

	    //ULINT Get_id();

        private:
            string name;    //node name
            int delta_time;
            bool node_type; //true in case of operator node and false in case of operand type
            list<string> labels;
            list<CDAGNode*> out_list;
            list<CDAGNode*> in_list;
            bool visited;
            int cost;
            bool root ;
	    //ULINT id; // unique id for the dag nodes : can be as big as the number of dag nodes

    };


#endif	/* _CDAGNODE_H */

