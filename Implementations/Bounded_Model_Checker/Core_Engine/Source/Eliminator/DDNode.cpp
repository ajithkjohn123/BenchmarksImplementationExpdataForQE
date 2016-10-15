/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#include "DDNode.h"

DDNode :: ~DDNode()  //destructor for the DDNode
{
}

DDNode :: DDNode(int new_val)  //constructor.....with 1 argument 
{	
	val = new_val;
	predicate = NULL;
	left = NULL;
	right = NULL;
	mark = false;
	id = -1; 
}


DDNode :: DDNode(DAGNode *new_predicate, DDNode *new_left, DDNode *new_right)  
{
  if(new_predicate == NULL) { cout<<"\nConstructor DDNode called with NULL predicate\n"; exit(1);} 

	val = -1;
	predicate = new_predicate;
	left = new_left;
	right = new_right;  
	mark = false;
	id = -1; 
}



int DDNode :: getID() {return this->id;}
void DDNode :: setID(int new_id) {this->id=new_id;}
int DDNode :: getVal() {return this->val;}
void DDNode :: setVal(int new_val) {this->val=new_val;}
bool DDNode :: getMark() {return this->mark;}
void DDNode :: setMark(bool new_mark) {this->mark=new_mark;}
DAGNode* DDNode :: getPredicate() {return this->predicate;}
void DDNode :: setPredicate(DAGNode* new_predicate) {this->predicate=new_predicate;}
DDNode* DDNode :: getLeft() {return this->left;}
void DDNode :: setLeft(DDNode* new_left) {this->left=new_left;}
DDNode* DDNode :: getRight() {return this->right;}
void DDNode :: setRight(DDNode* new_right) {this->right=new_right;}

string DDNode :: getDDString()
{
  string dag_str;
  char temp[10];

  if(predicate == NULL) // terminal nodes
    {
      
      sprintf(temp, "%d", this->val);
      dag_str= "val=";
      dag_str+=temp;
      sprintf(temp, "%d", this->id);
      dag_str+=",id=";
      dag_str+=temp;
      sprintf(temp, "%d", this->mark);
      dag_str+=",mark=";
      dag_str+=temp;
      return dag_str;
    }
  
  dag_str = "ite( ";
  dag_str += "content=";
  dag_str += getDAGStr(this->predicate);
  sprintf(temp, "%d", this->id);
  dag_str+=",id=";
  dag_str+=temp;
  sprintf(temp, "%d", this->mark);
  dag_str+=",mark=";
  dag_str+=temp;
  dag_str += " ,";
  dag_str += (this->left)->getDDString();
  dag_str += " ,";
  dag_str += (this->right)->getDDString();
  dag_str += " )";

  return dag_str;
}
