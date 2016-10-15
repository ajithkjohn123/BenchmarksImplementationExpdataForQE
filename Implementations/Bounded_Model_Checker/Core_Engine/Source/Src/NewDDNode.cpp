

#include "NewDDNode.h"

DDNode :: ~DDNode()  //destructor for the DDNode
{
  //cout << endl << "DDNode destroyed" << endl ;   //to be used only during debugging.......
  //exit(1);
}

DDNode :: DDNode(int new_val)  //constructor.....with 1 argument 
{	
	//index = MAX_AP+1;
	val = new_val;
	predicate = NULL;
	left = NULL;
	right = NULL;
	mark = false;

	id = -1; // Temporary

	//in_list.clear(); // On 4/1/10
}

DDNode :: DDNode(CDAGNode *new_predicate, DDNode *new_left, DDNode *new_right)  
{
  if(new_predicate == NULL) { cout<<"\nConstructor DDNode called with NULL predicate\n"; exit(1);} 

	val = -1;
	predicate = new_predicate;
	left = new_left;
	right = new_right;  
	mark = false;

	id = -1; // Temporary

	//in_list.clear(); // On 4/1/10
}

int DDNode :: getID() {return this->id;}
void DDNode :: setID(int new_id) {this->id=new_id;}
//int DDNode :: getIndex() {return this->index;}
//void DDNode :: setIndex(int new_index) {this->index=new_index;}
int DDNode :: getVal() {return this->val;}
void DDNode :: setVal(int new_val) {this->val=new_val;}
bool DDNode :: getMark() {return this->mark;}
void DDNode :: setMark(bool new_mark) {this->mark=new_mark;}
CDAGNode* DDNode :: getPredicate() {return this->predicate;}
void DDNode :: setPredicate(CDAGNode* new_predicate) {this->predicate=new_predicate;}
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
      //sprintf(temp, "%d", this->index);
      //dag_str+=",index=";
      //dag_str+=temp;
      sprintf(temp, "%d", this->mark);
      dag_str+=",mark=";
      dag_str+=temp;
      return dag_str;
    }
  
  dag_str = "ite( ";
  dag_str += "content=";
  dag_str += (this->predicate)->getDAGStr();
  sprintf(temp, "%d", this->id);
  dag_str+=",id=";
  dag_str+=temp;
  //sprintf(temp, "%d", this->index);
  //dag_str+=",index=";
  //dag_str+=temp;
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


// New fns on 4/1/10

// Append oprnd into the in_list of this node

/*
void DDNode :: Append_DDnode_inlist(DDNode* oprnd)
{
    this->in_list.insert(oprnd);
}

void DDNode :: Delete_single_entry_from_DD_inlist(DDNode* entry_to_be_deleted)
{
    this->in_list.erase(entry_to_be_deleted);
}

unsigned int DDNode :: Get_DDnode_inlist_size()
{
    unsigned int size ;
    size = (unsigned int) this->in_list.size();
    return size;
}

set<DDNode*>::iterator DDNode :: Get_DDnode_inlist()
{
    set<DDNode*>::iterator itr = this->in_list.begin();
    return itr;
}

void DDNode :: showNode()
{

  cout<<"\n*************************";
  cout<<"\nNode = "<<this;
  if(this->predicate == NULL) // terminal nodes
    {
    cout<<"\nValue = "<< this->val;
    }
  else
    {
    cout<<"\nPredicate = "<< (this->predicate)->getDAGStr();
    }

   cout<<"\nID = "<<this->id;
   cout<<"\nIndex = "<<this->index;
   cout<<"\nMark = "<<this->mark;
 
   cout<<"\nInlist = ";
   for(set<DDNode*>::iterator itr = this->in_list.begin(); itr != this->in_list.end(); itr++)
	cout<<"\t"<<*itr;

  cout<<"\nLeft = "<<this->left<<", Right = "<<this->right;;
  cout<<"\n*************************\n";
}

*/


