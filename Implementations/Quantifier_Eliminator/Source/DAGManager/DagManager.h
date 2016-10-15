#ifndef DAG_MANAGER_H
#define DAG_MANAGER_H

#include "HashTable_WithStandardKeys.h"

#include <vector>
#include <string>
#include <fstream>
#include <list>
#include <queue>
#include <map>
using namespace std;


/*---Forward declaration of FRIEND DAGManager ---*/
class DAGManager;



class DAGNode
{

  /*---------------Friends------------------*/
  /**
   * Note here that the class DAGManager has been made friend to DAGNode. This allows the class DAGManager to access private members of DAGNode.
   * Thus we get rid of a number of getty and setty methods for the DAGNode class, since it is highely unlikely that anybody else other than the DAGManager will ever need to have access to thsese fields.
   *
   **/
  friend class DAGManager;

  /**
   * This is a friend function, friend from both DAGNode and DAGManager classes.
   * The purpose of this function is to convert a pointer to a hex "string" object.
   */
  friend string addressToString(void *ptr);


  /*-------------Private Fields.
    Note that these are accessible to friends in the above section------------*/

 private:


  string label;            //the label, in restricted mode, the label may not be unique
  int reference_count;     //Reference Count of the nodes
  void *value;             //Allow the user to store a arbitrary value
                           //If the user is not storing any value, then the DAGManager stores a null in it..
  string signature;        //internal signature, to be used in restricted mode in DAGManager
  list<DAGNode *> in_list; //list of incident edges
  list<DAGNode *> out_list; //list of outgoing edges
  bool visited;     //is this node visited? 
  bool isShared;    //is this node shared?
  bool isRoot;      //is this node a root node?
  bool isLeaf;      //is this node a leaf node?
  int temp_id;     //identifier assigned to a shared node for naming..
  int copy_count; //Count how many times a DAGNode was copied


  //    next_root and prev_root are used for maintaining a list of root nodes of DAG
  //For Non-Root Nodes, these values must be NULL
  DAGNode *next_root;
  DAGNode *prev_root;



  //    next_node is used for maintaining a list of all nodes of DAG

  //The last node created by DAGManager will have this pointer NULL
  DAGNode *next_node;

 public:


  ~DAGNode();
  string getLabel();
  

 public:


  //Constructors for the DAGNode. These differ on the number parameters only
  DAGNode(void* value_to_set = NULL);


//function declerations added by sourabh moitra on 3/1/10

	string Get_node_name();     

        unsigned int Get_outlist_size();        

        list<DAGNode*>::iterator Get_outlist_start();        

        list<DAGNode*>::iterator Get_outlist_end();        

        unsigned int Get_inlist_size();        

        list<DAGNode*>::iterator Get_inlist_start();        

        list<DAGNode*>::iterator Get_inlist_end();        

        void Set_visited_flag();
        
        bool Get_visited_flag();        

        void Clear_visited_flag();
        

//END of function declerations added by sourabh moitra on 3/1/10




 public:
  DAGNode(string label_to_assign, void *value_to_set = NULL);
  DAGNode(string label_to_assign, vector<DAGNode*> v_operands, void *value_to_set = NULL);


  //Allow the user to traverse the out_list of the DAGNode
  int getSizeOfOutList();
  

  //Get iterator to the begining of outlist
  list <DAGNode *> :: iterator getOutListBeginIterator();
  //Get iterator to the end of outlist
  list <DAGNode *> :: iterator getOutListEndIterator();



  //Read the value stored in the DAGNode
  void *getValue();

  //Write the value in the DAGNode
  bool setValue(void *value_to_set);

  //set the visited flag -- only for the external traversals 

  bool assignVisitedFlag(bool assign_to);
  bool isCopied();
};







class DAGManager
{


  /*
    DAGManager takes care for all the private fields of the DAGNodes.
    Setting right value of reference count is responsibility of DAGManager.
    DAGNodes are always created using pointers, except some header nodes.
    Make sure that correct reference count is set in *DAGManager*
  */
 private:
  ofstream *dagmanager_log;
  HashTable <string, DAGNode *> ht;
  bool restricted_mode;
  static int dagmanager_instance_number;
  bool enable_logging;
  int copy_prefix_counter;
  /*
    Statically generated head node and tail pointer for maintaining list of root nodes in the DAG
  */

  DAGNode head_roots;  //This is a special node. This is never deleted. It does not appear on any list. No reference count for it.
  DAGNode *tail_roots;


  /*
    Maintain a list of all the nodes that were ever generated. This list is useful for traversing the DAG in O(n)
  */
  DAGNode head_nodes;   //This is a special node. This is never deleted. It does not appear on any list. No reference count for it.
  DAGNode *tail_nodes;


  /**
   * dagnode_count is the count of all DAGNodes that were ever generated
   * This function simply returns this count to caller
   * This count may also include garbage nodes
   *
   * Minimum dagnode_count is 2, corresponding to the two header nodes of linked lists
   * of root nodes and dag nodes
   */
  int dagnode_count;

  //just count how many bytes were used!!
  unsigned long int memory_usage;


  //Friends
  friend string addressToString(void *ptr);











  /**
   * The public API of the DAG Manager. Most of the functions in this
   * API have a recursive helper, which is private.  The functions are
   * listed in this file in a order such that the set of associated
   * functions are grouped together.  See the documentation for more
   * details.
   *
   */



  /*Section 1 : Constructor and destructor
    The restricted mode flag stands for 

    If the flag is set, the DAGManager uses internal mechanisms for
    generating signatures and takes care for sharing of similar node using
    it..
    If not set, the lable of every node should be unique. This is
    responsibility of the client. The DAGManager does not implement the
    mechanisms of signauture generation, etc. and it uses the lable of the
    DAGNode itself as the signature*/

 public:
  DAGManager(bool restricted_mode_flag =true , bool enable_logging_flag =true);
  ~DAGManager();

 private:
  void LOG(string message);



  /* Section 2: Create The DAG*/
 public:
  //create DAGNode
  DAGNode *createNode(string label,void* value=NULL);
  DAGNode *createNode(string label, vector<DAGNode*> v_children,void* value=NULL);


  //add and remove edges between nodes
  bool addEdge(DAGNode *parent, DAGNode *child);
  int removeEdge(DAGNode *parent, DAGNode *child);
  bool addNodeAsIthChild(DAGNode *parent, DAGNode *child, int i);
  //create the DAG by reading a file..
  bool buildDAGFromFile(ifstream *infile);
 private:
  //create signature of a node, restricted mode only
  inline string createSignature(string label,vector<DAGNode*> v_operands);
  inline string createSignature(string label);

  //recompute signature of a node when the signature of the node changes..
  bool RecomputeSignatureOfANode(DAGNode *node);
  //delete a non-root node from the list of root nodes, also called by deleteOperandsOfANodeFromRootsList
  bool deleteNodeFromRootsList(DAGNode *node_to_delete);

  //a new root node has been created, add it to the list of roots
  inline bool insertNodeInRootsList(DAGNode *node);

  //every node that is created is to be inserted in this list
  inline bool insertNodeInNodesList(DAGNode *node);

  //count the number of bytes used!!
  inline void countBytesUsedByANodeInMemoryUsage(DAGNode *node);

















  
  /*Section 3 : Delete the DAG*/
 public:
  //delete a DAG
  bool deleteDAG(DAGNode *node);
 private:
  //recursive helper for the deletion
  bool deleteDAGRecursively(DAGNode *node, DAGNode *parent);













  /* Section 4: Copy the DAG*/
 public:
  //copy DAG without values
  DAGNode *copyDAG(DAGNode *node);
  //copy DAG with values
  DAGNode *copyDAGWithValues(DAGNode *node, void * (*copyfunc)(void *value_to_copy) );

 private:
  //Recursively copy the DAG without values
  DAGNode *copy(DAGNode *node, string copy_prefix);

  //Recursively copy the DAG with values
  DAGNode *copyWithValues(DAGNode *node, string copy_prefix, void * (*copyfunc)(void *value_to_copy));















  /*Section 5 : Query the DAG */
 public:
  vector<DAGNode *> getVectorOfDescendentNodesInDFSOrder(DAGNode *node);
  vector<DAGNode *> getVectorOfDescendentNodesInBFSOrder(DAGNode *node);
  vector<DAGNode *> getVectorOfAncestorNodesInDFSOrder(DAGNode *node);
  vector<DAGNode *> getVectorOfAncestorNodesInBFSOrder(DAGNode *node);

  //het the list of all root nodes
  vector<DAGNode *> getVectorOfLeafNodes(DAGNode *node);
  //Get the list of all root nodes maintained by the DAGManager
  vector<DAGNode *> getVectorOfRootNodes();

  //Get i th child of a node
  //Same effect can be achieved by traversing the outlist of the node using iterators obtained by getOutListBeginIterator() and getOutListEndIterator();
  DAGNode *getIThChildOfNode(DAGNode *node, int i);
  //just obtain a handle to a leaf node with given label, useful when you know that the node was already created and you don't want to create it again
  DAGNode *getNodeWithLabel(string label);
  vector<DAGNode *> getNodesAtIthLevel(DAGNode *root,int i);
  int getHeightOfDAGWithMinimumPaths(DAGNode *root);
 private:  
  vector<DAGNode *> getChildrenInDFSRecursively(DAGNode * node);
  vector<DAGNode *> getParentsInDFSRecursively(DAGNode * node);
  //Recursively collect the leaf nodes of the DAG
  vector<DAGNode*> collectLeavesRecursively(DAGNode* node);












  /*Section 6: Some more Query operations. */
 public:
  //Test if the DAG is a Tree (has 0 shared nodes)
  bool isDAGATree(DAGNode *root);

  //Test if node_to_test is parent of parent_of
  bool isParent(DAGNode *node_to_test, DAGNode *parent_of);

  //Test if node_to_test is child of child_of
  bool isChild(DAGNode *node_to_test, DAGNode *child_of);

  //Test if node_to_test is ancestor of ancestor_of
  bool isAncestor(DAGNode *node_to_test, DAGNode *ancestor_of);

  //Test if node_to_test is descendent of descendent_of
  bool isDescendent(DAGNode *node_to_test, DAGNode *descendent_of);








  /*Section 7A: DAG Manipulation*/
 public:
  bool ResetVisitedFlagOfDAGNodes(); 

 private:
  bool ResetVisitedFlagOfDAG(DAGNode *node); 
  bool ResetVisitedFlagOfDAGRecursively(DAGNode *node);

  //traverse the list of all DAG nodes and clear the visited flag of entire DAG. Do this when you are not sure about the operation of ResetVisitedFlagOfDAG
  bool ResetVisitedFlagOfAllDAGNodes();

  //delete entry of a node from hashtable. does not delete the node itself, just deletes its entry from hashtable
  bool deleteANodeFromHashTable(DAGNode *node);


  //mark the shared nodes in DAG (although the DAG maintains the shared flag, redo it!!)
  int markSharedNodes(DAGNode *node);
  bool markSharedNodesRecursively(DAGNode *node, int &counter);


  //delete the incident edges of the node = Clear the in-list of this node and delete corresponding entry from the out-list of its parents
  bool deleteIncidentEdges(DAGNode *node);


  //when a node becomes root, its children are no more root, so mark them as non-root
  bool deleteOperandsOfANodeFromRootsList(DAGNode *node);

  //we cannot delete a node from the DAG directly. Need some steps to do that depending upon the values of flags in that node..
  bool deleteANodeFromDAG(DAGNode *node);















  /*Section 7B: Advanced DAG Manipulation*/
 public:
  bool ReplaceLeafByDAG(DAGNode *toReplace, DAGNode *replaceBy);
  bool ReplaceChild(DAGNode *parent, DAGNode *child_to_replace, DAGNode *child_replace_by);
  bool MergeEquivalentNodes(DAGNode *toReplace, DAGNode *replaceBy);











  
  /*Section 8: This is the garbage collector. Call it on the DAGManager object*/
 public:
  int collect_garbage();












  /*Section 9: Get Statistics from the DAGManager */

 public:
  //Count the number of nodes in a DAG  viewed as Tree
  int getSizeOfDAGAsTree(DAGNode *node);

  //Count number of nodes in a DAG viewed as DAG
  int getSizeOfDAG(DAGNode *node);

  //Get the count of root nodes internally maintained by the DAGManager
  int getCountOfRootNodes();

  //get the number of collisions that happened in the hashtable
  int getCollisionsInHashTable();

  //Count the number of memory bytes used by DAGManager
  unsigned long int getMemoryUsage();

  //Count all DAGNodes that were ever generated in the DAGManager
  int getCountOfAllDAGNodesInDAGManager()
  {
    return dagnode_count;
  }

 private: 
  //Recursively count the nodes in the DAG viewed as DAG
  int getCountOfNodesInDAGRecursively(DAGNode *node);
  









  /*section 10: Print the DAG*/


  //Useful for printing the DAG


  
  /*--General printing routines.. Prints DAG to file*/
 public:
  bool printDAG(DAGNode *node, ofstream *outfile);
  bool printDAGAsTreeToFile(DAGNode *node, ofstream *outfile );
 private:
  bool printDAGRecursively(DAGNode *node, bool isRoot, ofstream *outfile);
  bool assignTempIDsAndBuildSharedNodesQ(DAGNode *node, int &temp_id_counter, vector<DAGNode *> &shared_nodes_queue);
  bool printSharedNodesFromQueue(ofstream *outfile, vector<DAGNode *> &shared_nodes_queue);

 public:
  bool printDAGAsDOTFile(DAGNode *node, ofstream *outfile);

 private:

  bool printDAGAsDOTFileRecursively(DAGNode *node, ofstream *outfile);

  

};

#endif
