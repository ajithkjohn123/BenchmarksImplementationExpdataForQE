#include "DagManager.h"
#include "helping_functions.h"
#include <map>
#include<iostream>
using namespace std;





/**
 * Synopsis: Constructor for the DAGManager
 *
 * Description: Construct the DAGManager instance. Opens the log files
 * and sets up various lists, counters, etc. The log of every instance
 * of DAGManager is written to a new file. This is useful for the
 * purpose of debugging to the client of the DAGManager.
 *
 *
 **/

int DAGManager :: dagmanager_instance_number = 0;

DAGManager :: DAGManager(bool restricted_mode_flag, bool enable_logging_flag)
{
  dagmanager_log = new ofstream();
  dagmanager_instance_number++;
  copy_prefix_counter = 0;
  dagnode_count = 0;
  string log_file_name = "DAGManager_Instance" + integerToString(dagmanager_instance_number)+".log";
  dagmanager_log->open(log_file_name.c_str());
  //cout<<"DAGManager instance " <<dagmanager_instance_number<<" created \n";
  //cout<<"Check the log in the file "<<log_file_name<<endl;
  LOG("DAGManager instance " + integerToString(dagmanager_instance_number)+ " created \n");
  LOG("Check the log in the file " + log_file_name + "\n");
  
  LOG("Creating instance of DAG Manager in");
  if(restricted_mode_flag)
    LOG(" Restricted mode\n");
  else
    LOG(" Unique labels mode\n");
  //Initial setup for the list of root nodes
  head_roots.next_root = NULL;
  head_roots.prev_root = NULL;
  tail_roots = &head_roots;

  //Initial setup for the list of all nodes that will be generated in the DAG!
  head_nodes.next_node = NULL;
  tail_nodes = &head_nodes;
  restricted_mode = restricted_mode_flag;
  enable_logging = enable_logging_flag;
  LOG("Created DAGManager\n");
}













/**
 * Synopsis : Destructor for the DAGManager
 *
 * Description : It will dellocate all the memory that was allocated by the DAGManager
 * Check out for the error message by this guy to detect memory leaks
 */

DAGManager :: ~DAGManager()
{
  //Free up all the resources that are allocated
  LOG("Invoking the destructor \n");



  //THIS IS NOT GARBAGE COLLECTION
  DAGNode *current_node = head_nodes.next_node;
  int counter =0;
  LOG("Freeing the memory allocated \n");
  while(current_node!= NULL)
    {
      DAGNode *node_to_delete = current_node;
      current_node = current_node->next_node;
      delete(node_to_delete);
      counter ++;
    }
  LOG("Finished freeing the memory allocated \n");
  
  if(dagnode_count != counter)
    {
      LOG("ERROR!! Failed to reclaim memory allocated to all dag nodes \n");
      LOG(integerToString(dagnode_count)+" number of nodes were created\n");
      LOG(integerToString(counter)+" number of nodes were reclaimed\n");
      LOG("I could not recollect the memory allocated to "+ integerToString(dagnode_count-counter) +" nodes\n");
      LOG("Please look for leaking memory, or there may be a bug in the DAGManager \n");
    }
  else
    {
      LOG(integerToString(dagnode_count)+" number of nodes were created\n");
      LOG(integerToString(counter)+" number of nodes were reclaimed\n");
      LOG("Successfully reclaimed memory allocated to all dag nodes \n");
    }
  LOG("Closing the log file\n");
  dagmanager_log->close();
  //cout<<"Destroyed the DAGManager\n";
}









/**
 * Synopsis: Create a node with a given label and no children
 *
 * Description: Creates a new DAG Node with given label and no children. 
 * In restricted mode, this function returns an old node, marking it as being shared (isShared =true) if the node with the specified label exists the DAG.
 * In user-defined labels mode, this function returns an error if the node with the same label exists in the DAG.
 *
 * 
 * Otherwise it creates a new node, marking it as unshared (isShared = false)
 * All flags have valid values on the node returned.
 *
 * Return value: DAGNode * to the node created, NULL if it fails
 *
 * See Also : CreateNode(string label, vector<DAGNode *> v_children, void *value)
 *            buildDAGFromFile
 */

DAGNode * DAGManager :: createNode(
                                   string label,
				   void *value //NULL by default, see .h file
                                   )
{

  /*The moment you create a new node successfully, you should add it to the list of nodes created. If you are returning an old node, then no need to do this.
    Maintaining this list is responsibility of DAGManager, not DAGNode!!
  */
  if( label=="" )
    return NULL;

  if(restricted_mode)
    {
      LOG("Creating a leaf node with label "+label+"\n");
      string signature = createSignature(label);
      HTStatusValue <DAGNode *> search_signature = ht.hashtable_search(signature);
      if(search_signature.success())
        {
          // a node with this key exists in the hashtable
          //This is not a new node....
          //This node is a shared node in the DAG!!
          DAGNode *result_node = search_signature.GetValue();
          result_node->isShared = true;
	  result_node->reference_count ++;
          //When this node is returned, whoever holds it has a pointer to this node
          //Therefore its reference count should be increased by 1
	  LOG("Found a node with label "+label+ " in the DAG\n");
	  LOG("Sharing the requested node\n");
	  //Cannot say anything on visited flag here
	  LOG("Created node successfully\n");
          return result_node;
        }
      else
        {
	  LOG("Creating a new DAGNode \n");
          DAGNode *result_node = new DAGNode(label, value);
          result_node->isShared = false;
	  dagnode_count ++;
          result_node->reference_count = 1; //somebody is holding a handle to this node!!
	  result_node->signature = signature;
	  //count memory
	  countBytesUsedByANodeInMemoryUsage(result_node);

          HTStatusValue <DAGNode *> insert_result = ht.hashtable_insert(signature, result_node);
          if(insert_result.success())
            {
              result_node->signature = signature;
              //This is a new node and this is a root. A new node is always a root
              bool addedToRootsList = insertNodeInRootsList(result_node);
              bool addedToNodesList = insertNodeInNodesList(result_node);
	      LOG("Created node successfully\n");
              return result_node;
            }
          else
	    {
	      LOG("ERROR!! Failed Creating a node with label "+label+"\n");
	      return NULL;
	    }
        }
    }
  else if(!restricted_mode) //user-defined labels mode
    {
      //In this case, the label of the node itself is its signature
      LOG("Creating a leaf node with label "+label+"\n");
      string signature = createSignature(label);
      HTStatusValue <DAGNode *> search_signature = ht.hashtable_search(signature);
      if(search_signature.success())
        {
	  LOG("Found a node with label "+label+ " in the DAG\n");
	  LOG("ERROR!! Failed creating new node with label "+label+"\n");
          return NULL;
        }
      else
        {
	  LOG("Creating a new DAGNode \n");
          DAGNode *result_node = new DAGNode(label, value);
	  dagnode_count ++;
          result_node->isShared = false;
          result_node->reference_count = 1;
	  result_node->signature = signature;
	  //count memory
	  countBytesUsedByANodeInMemoryUsage(result_node);

          HTStatusValue <DAGNode *> insert_result = ht.hashtable_insert(signature, result_node);
          if(insert_result.success())
            {
              //This is a new node and this is a root
              bool addedToRootsList = insertNodeInRootsList(result_node);
              bool addedToNodesList = insertNodeInNodesList(result_node);
	      LOG("Created node successfully\n");
              return result_node;
            }
          else
	    {
	      LOG("ERROR!! Failed Creating a node with label "+label+"\n");
	      return NULL;
	    }
        }


    }

}














/**
 * Synopsis: Create a node with specified operands (i.e. a non-leaf node )
 *
 * Description: Creates a new DAG Node with specified operands. 
 * In restricted mode, this function returns an old node, marking it as being shared (isShared =true) if the node with the same signature exists the DAG.
 * In user-defined labels mode, it returns NULL if a node with same label (but perhaps a different set of children) exitst in the DAG
 * Otherwise it creates a new node, marking it as unshared (isShared = false)
 * All flags have valid values on the node returned.
 *
 * Return value: DAGNode * to the node created, NULL if it fails
 *
 * See Also : DAGManager::CreateNode
 */


DAGNode * DAGManager ::  createNode(
                                    string label,
                                    vector<DAGNode*> v_children,
				    void *value //NULL by default, see .h file
                                    )
{
  if(restricted_mode)
    {
      if( label=="" )
        return NULL;
      int num_operands = v_children.size();
      LOG("Creating a non-leaf node with label "+label+"\n");
      string signature = createSignature(label, v_children); //This is a nonleaf
      HTStatusValue <DAGNode *> search_signature = ht.hashtable_search(signature);
      if(search_signature.success())
        {
          // a node with this key exists in the hashtable
          //This node is a shared node in the DAG!!
	  LOG("Sharing a non-leaf node with label\n");
          DAGNode *result_node = search_signature.GetValue();
          result_node->isShared = true;
	  result_node->reference_count ++;
          //When this node is returned, whoever holds it has a pointer to this node
          //Therefore its reference count should be increased by 1
	  LOG("Created a non-leaf node with label "+label+"\n");
          return result_node;
        }
      else
        {
	  LOG("Creating a new non-leaf node with label "+label+"\n");
          DAGNode *result_node = new DAGNode(label, v_children, value);
	  dagnode_count ++;
          result_node->isShared = false;
          result_node->isLeaf = false;
          result_node->reference_count = 1;
	  result_node->signature = signature;
	  //count memory
	  countBytesUsedByANodeInMemoryUsage(result_node);

          HTStatusValue <DAGNode *> insert_result = ht.hashtable_insert(signature, result_node);
          if(insert_result.success())
            {
              result_node->signature = signature;
              //This is a new node and this is a root
              bool addedToRootsList = insertNodeInRootsList(result_node);
              bool addedToNodesList = insertNodeInNodesList(result_node);
              for(int i=0;i<num_operands;i++)
                v_children[i]->reference_count ++;
	      LOG("Created a non-leaf node with label "+label+"\n");
              return result_node;
            }
          else
	    {
	      LOG("ERROR!! Failed Creating a non-leaf node with label "+label+"\n");
	      return NULL;
	    }
        }
    }

  else if(!restricted_mode)
    {
      if( label=="" )
        return NULL;
      int num_operands = v_children.size();
      //In this case, the label of the node itself is its signature
      string signature = createSignature(label);
      HTStatusValue <DAGNode *> search_signature = ht.hashtable_search(signature);
      if(search_signature.success())
        {
	  LOG("ERROR!! Failed Creating a non-leaf node with label "+label+"\n");
          return NULL;
        }
      else
        {
          LOG("Creating a non-leaf in user-defined labels  mode \n");
          DAGNode *result_node = new DAGNode(label,v_children,value);
	  dagnode_count ++;
          result_node->isShared = false;
          result_node->reference_count = 1;
	  result_node->signature = signature;
	  //count memory
	  countBytesUsedByANodeInMemoryUsage(result_node);

          HTStatusValue <DAGNode *> insert_result = ht.hashtable_insert(signature, result_node);
          if(insert_result.success())
            {
              //This is a new node and this is a root
              bool addedToRootsList = insertNodeInRootsList(result_node);
              bool addedToNodesList = insertNodeInNodesList(result_node);
	      //increment the reference count of the children of this node
              for(int i=0;i<num_operands;i++)
                v_children[i]->reference_count ++;
	      LOG("Created a non-leaf node with label "+label+"\n");
              return result_node;
            }
          else
	    {
	      LOG("ERROR!! Failed Creating a non-leaf node with label "+label+"\n");
	      return NULL;
	    }
        }

    }
}




/**
 * Synopsis: Byte counter
 *
 * Description: This function just sums up the number of bytes of information contained in a DAGNode.
 */

inline void DAGManager :: countBytesUsedByANodeInMemoryUsage(DAGNode *node)
{
  memory_usage += sizeof(DAGNode);
  memory_usage += node->label.size();
  memory_usage += node->out_list.size()*sizeof(DAGNode *);
  memory_usage += node->signature.size();

}




/**
 *Synopsis:  The nodes in the DAG keep adding or getting deleted. The isShared flag on a node may carry inconsistent value in such cases.
 * Use this function to Re-Identify and Re-Mark the shared nodes
 *
 * Description: This function marks the shared nodes in the DAG by performing a DFS. Every node that is visited more than once is marked as shared.
 *
 * Precondition: None!!
 * Postcondition: Ensures that visited flag of all DAGNodes is set at the end. All flags of all DAGNodes have consistent value
 * 
 * Return value: true if it succeeds, false otherwise
 *
 * See Also : markSharedNodesRecursively
 */
int DAGManager ::  markSharedNodes(DAGNode *node)
{

  /*-------
    If there are some delete operations on the DAG, then there is a chance that the node that was marked as shared now becomes unshared.
    There are two things that can be done on this...
    1. A node having isShared=true && reference_count >1 is indeed a shared node AND
    A node having isShared=true && reference_count <=1 is not shared.
    This logic relies heavily on the correctness of reference count code!!
    2. Rerun the algorithm to mark the shared nodes...
    This is a better way certainly....
    -------*/

  if(node == NULL)
    return -1;


  LOG("Marking the Shared Nodes..\n");
  //Resetting visited flag of all nodes is needed here in order to ensure correctness
  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();
  if(reset_visited == false)
    return -1;

  int count = 0;
  LOG("Marking now\n");
  bool mark_shared = markSharedNodesRecursively(node, count);
  LOG("Finished marking the Shared Nodes..\n");
  return count;
}






/**
 * Synopsis: Mark the shared nodes in the DAG
 *
 * Description: When the DAG is JUST created, the shared nodes are marked correctly. However, the DAG may undergo a number of delete operations or rewrite operations, which may not be able to maintain the consistency of isShared flag of the DAG nodes. This function marks shared nodes in the DAG.
 *
 * Arguments: A non-null handle to a DAGNode, A reference to an integer that counts number of shared nodes. (Create an integer and set it to 0, pass its reference here)
 *
 * Return value: true if it succeeds, false otherwise
 *
 * Side Effects: Argument count finally contains the number of nodes that are marked as shared
 */


bool DAGManager ::  markSharedNodesRecursively(DAGNode *node, int &count)
{

  if(node == NULL)
    return false;

  if(node->visited == true)
    {
      count ++;
      node->isShared = true;
      return true;
    }
  node->visited = true;
  int out_list_size = node->out_list.size();
  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      bool marked_child = markSharedNodesRecursively(*it, count);
      if(marked_child == false)
	{
	  LOG("ERROR!! Failed marking shared nodes recursively\n");
	  return false;
	}
    }

  return true;
}







/**
 * Synopsis: Print the DAG
 *
 * Description: Prints the DAG to stdout..Since this function prints the DAG with an explicit indication of the shared nodes, it is a 4 step process
 * 1- Mark the shared nodes
 * 2- Assign temperory names to shared nodes
 * 3- print the root expression
 * 4- print expressions of shared nodes
 * In order to print the expressions of shared nodes, it uses shared_nodes_queue in which it temperorily queues up the shared nodes, while marking them
 *
 * Return value: false if it cannot print the DAG
 *               true on success
 *
 * See Also: printDAGRecursively, markSharedNodes, ResetVisitedFlagOfDAG, assignTempIDsAndBuildSharedNodesQ, printSharedNodesFromQueue
 */


bool DAGManager :: printDAG(DAGNode *node , ofstream *outfile)
{

  LOG("Printing the DAG \n");
  //markSharedNodes has no precondition
  int mark_shared_nodes_count = markSharedNodes(node);
  if(mark_shared_nodes_count == -1)
    {
      LOG("ERROR!! Failed marking shared nodes\n"); 
      return false;
    }



  //Since assignTempIDsAndBuild* requires visited flag of DAG be resetted
  //After a call to markSharedNodes, we are sure about the consistency of the visited flag of DAG
  //So invoke ResetVisitedFlagOfDAG
  bool reset_visited = ResetVisitedFlagOfDAG(node);
  if(reset_visited == false)
    {
      LOG("ERROR!! Failed resetting visited flag of DAG\n"); 
      return false;
    }

  vector<DAGNode *> shared_nodes_queue;
  int temp_id_counter = 1;
  bool assign_temp_ids = assignTempIDsAndBuildSharedNodesQ(node, temp_id_counter, shared_nodes_queue);


  *outfile<<"root := ";
  bool print_root_expr = printDAGRecursively(node, true, outfile);
  if(print_root_expr == false)
    {
      LOG("ERROR!! Failed printing the expression of the Root node of the DAG \n"); 
      return false;
    }
  *outfile<<endl;


  bool print_shared_expressions = printSharedNodesFromQueue(outfile, shared_nodes_queue);
  if(print_shared_expressions==false)
    {
      LOG("ERROR!! Failed printing shared nodes from queue\n"); 
      return false;
    }
  LOG("Successfully printed the DAG\n");
  return true;
}









/**
 * Synopsis: Assign temperory ids to shared nodes
 *
 * Description: This is a function used during the common subexpression identification process which is used to mark the shared nodes with temperory identifiers. It is recursive.
 *
 * Precondition: Requires the visited flag of DAGNodes be resetted
 *
 * Return value: false if it cannot print the DAG
 *               true on success
 * 
 *See Also: printDAG, printDAGRecursively, markSharedNodes, ResetVisitedFlagOfDAG, printSharedNodesFromQueue
 */

bool DAGManager ::  assignTempIDsAndBuildSharedNodesQ(DAGNode *node, int &temp_id_counter, vector<DAGNode *> &shared_nodes_queue)
{

  LOG("Assigning the temporary IDs anb building the shared nodes queue\n");
  /*--Visited flags are reset. Shared flags are on */
  if((!(node->visited) && node->isShared))
    {
      node->temp_id = temp_id_counter;
      temp_id_counter ++;
      node->visited = true;
      shared_nodes_queue.push_back(node);
    }
  else if(node->visited)
    {
      return true;
    }
  node->visited = true;
  int num_operands = node->out_list.size();
  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      bool assign_ids = assignTempIDsAndBuildSharedNodesQ(*it, temp_id_counter, shared_nodes_queue);
      if(assign_ids == false)
	{
	  LOG("ERROR!! Failed to assign temporary ids to DAGnodes"); 
        return false;
	}
    }
  LOG("Successfully assigned the temporary IDs anb building the shared nodes queue\n");
  return true;
}







/**
 * Synopsis: Print the expressions of shared nodes
 *
 * Description: Prints the expressions of shared nodes to stdout from shared_nodes_queue
 *
 * Return value: false if it cannot print the DAG
 *               true on success
 *
 * See Also: printDAG, printDAGRecursively, markSharedNodes, ResetVisitedFlagOfDAG, assignTempIDsAndBuildSharedNodesQ
 */

bool DAGManager :: printSharedNodesFromQueue(ofstream *outfile, vector<DAGNode *> & shared_nodes_queue)
{
  LOG("Printing the shared nodes from the queue\n");
  int num_shared_nodes = shared_nodes_queue.size();
  for(int i=0;i<num_shared_nodes;i++)
    {
      *outfile<<"$"<<shared_nodes_queue[i]->getLabel()<<"_"<<shared_nodes_queue[i]->temp_id<<" :=  ";
      bool print_expression = printDAGRecursively(shared_nodes_queue[i], true, outfile);
      *outfile<<endl;
      if(!print_expression)
	{
	  LOG("ERROR!! Failed printing a node from shared nodes queue\n"); 
	  return false;
	}
    }
  LOG("Successfully Printed the shared nodes from the queue\n");
  return true;
}




/**
 * Synopsis: Print the root expression of a DAG
 *
 * Description: Prints the root expression of a DAG to outfile, recursively
 * It is important to node that shared nodes are suffixed with their temp_ids during printing. This helps in reading back the DAG from the file.
 *
 * Return value: false if it cannot print the DAG
 *               true on success
 * 
 * See Also: printDAG, markSharedNodes, ResetVisitedFlagOfDAG, assignTempIDsAndBuildSharedNodesQ, printSharedNodesFromQueue
 */


bool DAGManager ::  printDAGRecursively(DAGNode *node, bool isRoot, ofstream *outfile)
{
  if (node == NULL)
    return false;

  if(node->isShared && !isRoot)
    {

      *outfile<<"$"<<node->getLabel()<<"_"<<node->temp_id;
      return true;
    }
  else
    {

      *outfile<<node->getLabel();
    }

  int num_operands = node->out_list.size();
  if(num_operands>0)
    {
      *outfile<<"(";
      int i=0;
      for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
        {
          bool printed_child = printDAGRecursively(*it, false, outfile);
          if(i!=num_operands-1)
            *outfile<<",";
          if(printed_child==false)
            return false;
          i++;
        }
      *outfile<<")";
    }
  return true;
}












/**
 * Synopsis : This function returns the size of the DAG, viewed as a DAG
 *
 * Arguments: Handle to a DAGNode
 *
 *Return value: 0 if it fails or a null DAG
 *              >=1 on success
 */



int DAGManager :: getSizeOfDAG(DAGNode *node)
{
  //reset visited flag of all DAGNodes to be sure about the consistency
  LOG("Obtaining the size of DAG\n");
  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();
  if(reset_visited == false)
    return 0;

  int count = getCountOfNodesInDAGRecursively(node);
  LOG("Succesfully obtainedthe size of the DAG\n");
  return count;
}












/**
 * Synopsis: This is a private function, counts number of DAG nodes viewed as a DAG
 *
 * Description: This is a recursive function. It traverses the DAG marking the nodes as visited, and keeps counting the nodes in the DAG
 *
 *Precondition: The visited flags of reachable DAGNodes should be resetted.
 *
 * Return value: Count of DAG Nodes, viewed as DAG
 *
 */
int DAGManager :: getCountOfNodesInDAGRecursively(DAGNode *node)
{
  if(node == NULL)
    return 0;
  int count = 1; //Count this node
  if(node->visited == true)
    return 0;//this node was already counted!!

  node->visited = true;
  int num_operands = node->out_list.size();
  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)

    {
      count = count+getCountOfNodesInDAGRecursively(*it);
    }
  return count;
}




/**
 * Synopsis : Find size of DAG, starting from given DAGNode viewed as a tree.
 *
 * Return value : -1, if it fails
 *                >=0, on success
 */

int  DAGManager :: getSizeOfDAGAsTree(DAGNode *node)
{
  LOG("Obtaining the size of DAG as a Tree\n");
  if(node->out_list.size() == 0)
    return 1;
  else
    {
      int size = 1;

      for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
        size += getSizeOfDAGAsTree(*it);
      LOG("Obtained the size of DAG as a Tree\n");
      return size;
    }
}







/**
 * Synopsis: this function resets the visited flags of all the DAG nodes contained in the DAG Manager by traversing the queue of DAGNodes
 *
 * Description: This function traverses the linked list of all DAGNodes created in the DAGManager starting from head_nodes and goes on resetting visited flag of every node (including the nodes that have possibly became garbage)
 *
 * Precondition: None
 *
 * Return value: true if it succeeds, false otherwise
 *
 * Complexity: O(number of nodes in the DAGManager)
 *
 * See Also: ResetVisitedFlagOfDAG
 */


bool DAGManager ::  ResetVisitedFlagOfAllDAGNodes()
{
  LOG("Resetting the visited flag of all DAG nodes by traversing the list of DAG Nodes\n");
  DAGNode *current_node = &head_nodes;
  int counter =0;
  while(current_node->next_node != NULL)
    {
      current_node = current_node->next_node;
      current_node->visited = false;
      counter ++;
    }
  if(current_node!= tail_nodes)
    {
      LOG("ERROR!! Failed to reset visited flags of all dag nodes \n");
      return false;
    }
  LOG("Successfully resetted visited flags of all dag nodes\n");
  return true;
}












/**
 * Synopsis: this function simply prints the DAG to specified file
 *
 * Description: This function is same as printDAGAsTree except that it prints it to a file
 *
 * Return value: true if it succeeds, false otherwise
 *
 *
 */


bool DAGManager ::  printDAGAsTreeToFile(
                                         DAGNode *node,
                                         ofstream *outfile
                                         )
{
  bool result;
  *outfile<<node->label;
  if(!node->isLeaf)
    {
      *outfile<<"(";
      int out_list_size = node->out_list.size();
      int i =0;
      for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
	{
	  bool result = printDAGAsTreeToFile(*it,outfile);
	  if(!(i == out_list_size -1))
	    *outfile << ",";

	  if(result == false)
	    {
	      LOG("ERROR!! Failed printing DAGAsTreeToFile\n");
	      return false;
	    }
	  i++;
	}
      *outfile<<")";
    }

  return true;

}





/**
 * Synopsis: print DAG in graphviz DOTTY format
 *
 * Description: This function prints the DAG in DOTTY format to a file. The output file can be visually observered using dotty
 *
 * Return value: true if it succeeds , false otherwise
 *
 *
 */

bool DAGManager :: printDAGAsDOTFile(DAGNode *node, ofstream *outfile)
{
  /*-------
    The nodes in the DAG that are shared are indeed marked as being shared while they are created.
    Unless there is a deleteDAG operation executed on the DAG, a node that is shared has no opportunity for becoming unshared???
    -------*/
  LOG("Printing DAG as DOT\n");
  *outfile<<"digraph G{"<<endl;

  LOG("Marking shared nodes \n");
  int mark_shared_nodes_count = markSharedNodes(node);
  if(mark_shared_nodes_count == -1)
    {
      LOG("Found shared_node_count = -1 in printDAGAsDOTFile\n");
      return false;
    }

  LOG("Resetting visited flag\n");
  bool reset_visited = ResetVisitedFlagOfDAG(node);
  if(reset_visited == false)
    {
      LOG("ERROR!! Failed resetting visited flag of DAG\n");
      return false;
    }
  LOG("Printing the DAG\n");
  bool printed_dot = printDAGAsDOTFileRecursively(node, outfile);
  if(printed_dot == false)
    {
      LOG("ERROR!! Failed printing DAGAsDot recursively\n");
      return false;
    }

  *outfile<<"}"<<endl;
  return true;
}


bool DAGManager :: printDAGAsDOTFileRecursively(DAGNode *node, ofstream *outfile)
{

  if(node == NULL)
    {
    return false;
    }
  if(node->visited==true)
    return true;
  node->visited=true;

  *outfile<<"n"<<node<<"[label=\""<<node->getLabel()<<"\\nShared: "<<node->isShared <<"\\nRoot: "<<node->isRoot<<"\\n Leaf "<<node->isLeaf<<"\\nRC:"<<node->reference_count<<"\\nCC:"<<node->copy_count<< "\"];"<<endl;
  int num_operands = node->out_list.size();

  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      *outfile<<"n"<<node<<"->"<<"n"<<*it<<endl;
    }


  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      bool printed_operands = printDAGAsDOTFileRecursively(*it, outfile);
      if(printed_operands==false)
	{
	  LOG("ERROR!! Failed call to printDAGAsDOTFileRecursively\n");
	  return false;
	}
    }
  return true;
}





/**
 * Nodes in the DAG that are not referred by anybody are garbage.
 * Use the collect_garbage method to delete these nodes from the DAG and free up the memory used by them
 * The DAGManager internally uses a reference count for every node, which is updated in every DAG operation.
 * Garbage nodes are those that have reference count as 0 and in_list.size()=0 (i.e.Number of parents=0)
 *
 * Return value: count of the number of nodes deleted (>=0) if it succeeds
 */


int DAGManager :: collect_garbage()
{

  LOG("-------------------------------------------\n");
  LOG("Collecting garbage.........................\n");
  DAGNode *current_node = head_nodes.next_node;
  DAGNode *prev_node = NULL;
  int counter =0;
  if(current_node == NULL)
    {
    LOG("Garbage collector invoked when DAGManager has 0 DAGNodes in it\n");
    return 0;
    }
  while(current_node != NULL)
    {
      if((current_node->reference_count) ==0)
        {
	  if(current_node->in_list.size() != 0)
	    {
	      LOG("ERROR!! Found a node with reference count 0, but non-zero in-list\n");
	      LOG("This node is not being deleted \n");
	      continue;
	    }
          //During deletion, prev_node does not change!!
          DAGNode *node_to_delete = current_node;
          if(prev_node == NULL)
            {
              head_nodes.next_node = current_node->next_node;
              current_node = current_node->next_node;
              LOG("Deleting first node in nodes list\n");
	      //This is the use of having a sentinel head node. you don't have to change it
              deleteANodeFromHashTable(node_to_delete);
	      memory_usage = memory_usage - node_to_delete->signature.size() - node_to_delete->label.size() - sizeof(DAGNode);
	      
	      //if this node is root, delete it from the list of roots
	      if(node_to_delete->isRoot)
		deleteNodeFromRootsList(node_to_delete);
              delete(node_to_delete);
	      dagnode_count--;
              counter ++;
            }
          else if(current_node->next_node==NULL)
            {
              prev_node->next_node = current_node->next_node;
              LOG("Deleting last node in the nodes list\n");

              deleteANodeFromHashTable(node_to_delete);
	      memory_usage = memory_usage - node_to_delete->signature.size() - node_to_delete->label.size() - sizeof(DAGNode);
	      //if this node is root, delete it from the list of roots
	      if(node_to_delete->isRoot)
		deleteNodeFromRootsList(node_to_delete);
              delete(node_to_delete);
	      dagnode_count--;
              counter ++;
	      //Once again traverse the list and reset the tail pointer
	      //Note that this was the last node in the list, and got deleted
	      //Actually there is no need to retraverse the list, since we have the handle to non-null prev node
	      LOG("Re-Setting the tail_nodes pointer ...\n");
	      tail_nodes = prev_node;
            }
          else
            {
              prev_node->next_node = current_node->next_node;
              current_node = current_node->next_node;
              LOG("Deleting middle node from the nodes list\n");

              deleteANodeFromHashTable(node_to_delete);
	      memory_usage = memory_usage - node_to_delete->signature.size() - node_to_delete->label.size() - sizeof(DAGNode);
	      //if this node is root, delete it from the list of roots
	      if(node_to_delete->isRoot)
		deleteNodeFromRootsList(node_to_delete);
              delete(node_to_delete);
	      dagnode_count--;
              counter ++;
            }

        }
      else //Current node is not to be deleted...
        {
          prev_node = current_node;
          current_node = current_node->next_node;
        }
    }
  //Reset the tail pointer...
  LOG("Successfully collected "+integerToString(counter)+" number of garbage nodes ");
  return counter;
}












/**
 * This is a private function, friend of both DAGNode and DAGManager.
 * This function converts a pointer address to a hex string.
 * This function is used for generating the (internal) unique key associated with each node
 *
 * Return Value : 0xabcdef123 --> "abcdef123"
 *                invalid hex number --> ""
 */

string addressToString(void *ptr)
{
  //LOG("Address of ptr is "<<ptr<<endl;
  string result="";
  //LOG("Size of void pointer is "<<sizeof(void *)<<endl;
  unsigned long int num = (unsigned long int)ptr;
  for(int i=1;i<=sizeof(void *)*2;i++)
    {
      int nump = num & 0xf;
      num = num >>4;
      switch (nump)
        {
        case 0:
          result = "0" + result;
          break;
        case 1:
          result = "1" + result;
          break;
        case 2:
          result = "2" + result;
          break;
        case 3:
          result = "3" + result;
          break;
        case 4:
          result = "4" + result;
          break;
        case 5:
          result = "5" + result;
          break;
        case 6:
          result = "6" + result;
          break;
        case 7:
          result = "7" + result;
          break;
        case 8:
          result = "8" + result;
          break;
        case 9:
          result = "9" + result;
          break;
        case 10:
          result = "a" + result;
          break;
        case 11:
          result = "b" + result;
          break;
        case 12:
          result = "c" + result;
          break;
        case 13:
          result = "d" + result;
          break;
        case 14:
          result = "e" + result;
          break;
        case 15:
          result = "f" + result;
          break;
        default:
          return ""; //This is really important. If it cannot convert a hex number to string, it will return empty string
          break;
        }
    }

  return result;

}









/**
 * Synopsis: Marks the operands of a new root node as non-root
 *
 * Description: When a new DAG is created, it will finally be
 * represented in terms of a DAGNode. This is a new root. The operands
 * of this DAG MAY or MAYNOT be roots. Since all the operands of this
 * new root are now reachable from this root, we mark the operands as
 * non root.

 * A caller may still view these operands as roots. But that is its
 * responsibility. All we care about is whether all the valid DAG is
 * still reachable from the available set of roots in the DAGManager.
 *
 * Return value: true if it succeeds, false otherwise
 *
 */


bool DAGManager :: deleteOperandsOfANodeFromRootsList(DAGNode *node)
{

  LOG("Deleting operands of a node from roots list\n");
  //Put the non-root nodes off the list...DO NOT DELETE THEM
  if(node->out_list.size()!=0)
    {
      int size = node->out_list.size();

      for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
        {
          //Delete every  child of node from roots list
          DAGNode *node_to_delete = *it;
          if(!(node_to_delete-> isRoot))
            {
              continue;
            }
          if(node_to_delete->next_root == NULL)
            {
	      //This is the last node in the list
              node_to_delete->isRoot = false;
              node_to_delete->reference_count --;
              node_to_delete->prev_root->next_root = NULL;
	      node_to_delete->next_root = NULL;
	      node_to_delete->prev_root = NULL;
        
            }
          else if(node_to_delete->prev_root == NULL)
            {
	      LOG("ERROR!! Failed deleting operands of a node from roots list\n");
	      LOG("Could not find head_roots \n");
              return false;
              //cannot happen, since there is a header node head_roots from which the list has originated. This node is never NULL!!
	      //This is the purpose of having the head node
            }
          else
            {
              node_to_delete->isRoot = false;
              node_to_delete->prev_root->next_root = node_to_delete->next_root;
              node_to_delete->next_root->prev_root = node_to_delete->prev_root;
              node_to_delete->reference_count --;
	      node_to_delete->next_root = NULL;
	      node_to_delete->prev_root = NULL;
            }

        }
    }

  LOG("Successfully Deleted operands of a node from roots list\n");
  return true;
}




/**
 * Synopsis: Delete a dag!!
 *
 * Description: This function deletes DAG beneath the specified DAG
 * node. IT DOES NOT CARE ABOUT THE NODES ABOVE THE SPECIFIED DAG
 * NODE, SO USE IT CAREFULLY. You may end up converting useful nodes
 * into garbage.  This functions solely operates by looking at the
 * reference counts of the nodes. When we say it deletes DAG below a
 * node, it really deletes (reduces reference count) of the nodes
 * below it. No nodes are actually deleted. It is a good idea to run
 * garbage collector once you have deleted a (part of) DAG.
 *
 * Arguments: See inline comments
 *
 * Return value: true if it succeeds, false otherwise
 *
 */


bool DAGManager :: deleteDAG(
			     DAGNode *node //Root of the DAG to be deleted
			     )
{
  LOG("Deleting a DAG\n");
  bool detach_result = deleteIncidentEdges(node);
  if(detach_result == false)
    {
      LOG("ERROR!! Failed deleting incident edges on a node in deleteDAG\n");
      return false;
    }
  bool delete_result = deleteDAGRecursively(node, NULL);
  if(delete_result == false)
    {
      LOG("ERROR!! Failed deleting DAG Recursively\n");
      return false;
    }
  LOG("Successfully Deleted a DAG\n");
  return true;

}



bool DAGManager :: deleteDAGRecursively(
                                        DAGNode *node, //Handle to DAG to be deleted
                                        DAGNode *parent //Handle to parent of DAG to be deleted, NULL if none exists
                                        )
{
  //How to delete DAG?
  //Check for the nodes that are shared.....!

  if(node == NULL)
    {
      //cannot delete a null node
      LOG("Node called for deletion is null\n");
      return false;
    }
  if(node->reference_count >1)
    {
      //More than one node refer to this node
      //Therefore this node must be a shared node
      //One has to mark all the shared nodes before being able to do this.......
      //No need to change the out_list of node, as it will anyway be present
      int parent_count = node->in_list.size();


      //The following for loop should erase only one parent!!!

      for(list<DAGNode*>::iterator inlist_it=node->in_list.begin();inlist_it!=node->in_list.end();inlist_it++)
        {

          if(*inlist_it == parent)
            {
              node->in_list.erase(inlist_it);
              break;// Note the break. This loop must delete only one pointer from in_list
            }
        }

      node->reference_count --;
      return true;
    }


  else if(node->reference_count == 1)
    {

      //This node is going to be deleted..
      //Why care about its out_list and in_list???


      //The out_list of its parent must be updated
      //The in_list of its children must be updated

      //This is the root of a DAG to be deleted...
      //Find its children and delete them...
      //At the end, delete this node
      int num_operands = node->out_list.size();

      //No need to change the outlist of this node as it will anyway be deleted
      for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
        {

          bool deleted_operand = deleteDAGRecursively(*it, node);
          if(deleted_operand == false)
            {
              return false;
            }
        }//end for

      ///when conducting the delete operation
      ///make sure that enough pointers are deleted from the parent
      //We cannot call delete operation directly on the node
      //We have to call a specialist routine for that matter!!
      bool delete_from_dag = deleteANodeFromDAG(node);
      if(delete_from_dag == false)
        return false;
      return true;
    }
  //  LOG("Control cannot reach here\n";
}





/**
 * Synopsis: Detailed deletion of a node from DAG
 *
 * Description: When deleting a node from dag, it may not be possible to delete it directly...This function takes care for details of deletion process. STRICTLY PRIVATE use.
 *
 * Return Value: True if it succeeds, false otherwise
 *
 */

bool DAGManager :: deleteANodeFromDAG(DAGNode *node)
{

  LOG("Deleting a node from DAG  :");
  LOG(node->label+"\n");
  //Let this node hang around as an unreferenced node
  //garbage collector will take care for its deletion
  if(node->isRoot)
    {
      //This was a root node
      //remove it from the list of root nodes
      bool deleted_from_roots = deleteNodeFromRootsList(node);

      if(deleted_from_roots == false)
        {
          LOG("Unable to delete the root\n");
          return false;
        }
    }
  node->reference_count = 0;
  LOG("Deleted a node from DAGn");
  return true;
}









/**
 * Synopsis: Get count of ROOT nodes in the DAG
 *
 * Description: This function returns the number of nodes present in the list of root nodes. Note that a caller may have more handles than the number of roots present in this list.
 * This list just contains enough root nodes from which all the VALID dag is reachable. The caller may be interpreting some more nodes as roots.
 *
 * Return value: >=0, the count of nodes on the roots list
 *
 *
 */

int DAGManager :: getCountOfRootNodes()
{
  DAGNode *current_node = head_roots.next_root;
  if(current_node == NULL)
    return 0;
  int counter = 0;
  while(current_node!=NULL)
    {
      counter++;
      current_node = current_node->next_root;
    }
  return counter;
}













/**
 * Synopsis: Removes root status of a DAGNode
 *
 * Description: Marks a node as non root by deleting it from the list
 * of root nodes maintained by the DAGManager It does not delete any
 * node, just reorganizes roots list. When a node is deleted from
 * roots list, its operands may become new roots, which is also
 * performed by this function.
 */
bool DAGManager :: deleteNodeFromRootsList(DAGNode *node_to_delete)
{


  //
  LOG("Deleting a node from roots list\n");
  if(!(node_to_delete-> isRoot))
    {
      LOG("Node to be deleted from roots list is not a root\n");
      return false;
    }
  if(node_to_delete->next_root == NULL)
    {
      node_to_delete->isRoot = false;
      node_to_delete->prev_root->next_root = NULL;
      node_to_delete->reference_count --;

      for(list<DAGNode *>::iterator it=node_to_delete->out_list.begin(); it!=node_to_delete->out_list.end();it++)
	{
	  DAGNode *current_child = *it;
	  if(current_child->in_list.size()==0)
	    insertNodeInRootsList(current_child);	    
	}
      return true;
      //This is the last node in the list
    }
  else if(node_to_delete->prev_root == NULL)
    {
      return false;
      LOG("ERROR!! Failed to hit head_roots when deleting a node from roots list\n");
      //cannot happen
    }
  else
    {
      node_to_delete->isRoot = false;
      node_to_delete->prev_root->next_root = node_to_delete->next_root;
      node_to_delete->next_root->prev_root = node_to_delete->prev_root;
      node_to_delete->reference_count --;
      for(list<DAGNode *>::iterator it=node_to_delete->out_list.begin(); it!=node_to_delete->out_list.end();it++)
	{
	  DAGNode *current_child = *it;
	  if(current_child->in_list.size()==0)
	    insertNodeInRootsList(current_child);	    
	}

      return true;
    }
  LOG("Successfully a node from roots list\n");
}






int DAGManager :: getCollisionsInHashTable()
{
  return ht.getCountOfCollisions();
}







/**
 * Synopsis: Detach a sub-DAG from a DAG
 *
 * Description: This function detaches given sub-DAG from the main DAG. Given a sub-DAG indicated by node N, it looks at all the parents of N, and deletes N from their out_list. Then it sets reference count of N to 0 and clears its in-list. This effectively detaches the sub-DAG beneath N from the DAG.
 *
 * Return value: false if it cannot detach the DAG
 *               true on success
 */




bool DAGManager :: deleteIncidentEdges(DAGNode *node)
{

  LOG("Deleting incident edges on a node\n");
  if(node==NULL)
    {
      LOG("Node is null\n");
      return false;
    }
  if(node->in_list.size()==0)
    {
      return true;
    }
  else
    {

      for(list<DAGNode*>::iterator inlist_it=node->in_list.begin();inlist_it!=node->in_list.end();inlist_it++)
        {
          DAGNode *parent_i = *inlist_it;
          for(list<DAGNode*>::iterator it=parent_i->out_list.begin();it!=parent_i->out_list.end();it++)
            {
              if((*it) == node)
                {
                  parent_i->out_list.erase(it);
                  break;
                }
            }
        }
      node->in_list.clear();
      node->reference_count = 1;
      return true;
    }
  LOG("Successfully Deleted incident edges on a node\n");
}







/**
 * synopsis: Build signature of a DAG Node
 *
 * Description: Calculating the signature of a symbol node does not involve any operand addresses. The purpose of this function is to provide a shorthand for such nodes. This is also a PRIVATE function.
 *Arguments:
 *
 * Return value:  if the label is "", then ""
 *                else a valid signature string
 */

inline string DAGManager :: createSignature(
                                            string label //label of the node to be created
                                            )
{

  vector<DAGNode *> dummy_vector;
  
  return createSignature(label, dummy_vector);
}









/**
 * synopsis: Build signature of a DAG Node
 *
 * Description: This function takes specifications of a DAGNode and builds a signature from it. This function builds signature for both leaf and non-leaf nodes. This function is PRIVATE to DAGManager
 *
 *Arguments:
 *
 * Return value:  if the label is "", then ""
 *                else a valid signature string
 */
inline string DAGManager :: createSignature(
                                            string label,                //Label of the node
                                            vector<DAGNode*> v_operands //Operands of the node
                                            )
{
  if(label == "")
    return "";
  string signature = "";
  int num_operands = v_operands.size();
  for(int i=0;i<num_operands; i++)
    {
      signature = signature + addressToString(v_operands[i]);
    }
  signature += label;
  if(num_operands==0) //this is a leaf node
    signature += '1';
  else //non-leaf node
    signature += '0';
  return signature;
}

















/**
 * Synopsis: PRIVATE function that inserts a node in the list of root nodes
 *
 * Description: The DAGManager internally maintains a list of roots of
 *DAGs. Since the DAGManager may contain more than one DAG, a list of
 *all root nodes acts as a container for all the root nodes. This
 *function inserts a node in the list of root nodes. This is a PRIVATE
 *function, and should be called on a node that is known to be
 *root. e.g. a newly created DAGNode As soon as it adds a node as a
 *root node, it will take care for making the operands of this node as
 *non-root.
 *
 * Arguments: Non-null DAGNode
 *
 * Return value: True, if it succeeds, false otherwise
 */
inline bool DAGManager :: insertNodeInRootsList(DAGNode *node)
{
  LOG("Inserting a node in roots list\n");
  if(node ==NULL)
    {
      LOG("WARNING. Attempt to add a null node to the list of roots\n");
      return false;
    }
  LOG("Inserting a node in roots list \n");
  tail_roots->next_root = node;
  node->prev_root = tail_roots;
  node->next_root = NULL;
  tail_roots = node;
  node->isRoot = true;
  bool deleted_operands = deleteOperandsOfANodeFromRootsList(node);
  if(deleted_operands == false)
    {
      LOG("Could not delete the operands of a new root node from the roots list\n");
      return false;
    }
  LOG("Successfully Inserted a node in roots list\n");
  return true;
}




/**
 * Synopsis: PRIVATE function that inserts a node in the list of nodes
 *
 * Description: The DAGManager internally maintains a list of all
 * nodes of DAG. This list allows one to traverse the DAG in O(n),
 * allows garbage collection and prevents necessity to traverse DAG as
 * tree..
 *
 * Arguments: Non-null DAGNode
 *
 * Return value: True, if it succeeds, false otherwise
 */



inline bool DAGManager :: insertNodeInNodesList(DAGNode *node)
{
  LOG("Inserting a node in nodes list\n");
  if(node == NULL)
    {
      LOG("ERROR!! Failed Inserting a node in roots list, node is null\n");
      return false;
    }
  tail_nodes->next_node = node;
  node->next_node = NULL;
  tail_nodes = node;
  LOG("Successfully Inserted a node in roots list\n");
  return true;
}






/**
 * Synopsis: Public function that adds edge between two nodes...(Unique labels mode only)
 *
 * Description: This function allows to add edge between two nodes. Note that this function is costly, as it checks for possibility of creation of cycle when adding the edge. This check is done by testing whether the node to added as child appears in the ancestor list of parent...
 *
 * Arguments: DAGNode *parent: Node to be made parent
 *            DAGNode *child: Node to be made child
 *
 * Return value: True, if it succeeds, false otherwise
 * PostCondition: In restricted mode, adding edge between two nodes will change the signature of parent. There may exist a node in hashtable with same signature, which will replace this node, if found.
 */

bool DAGManager :: addEdge(DAGNode *parent, DAGNode *child)
{
  LOG("Adding edge between two nodes \n");
  if(parent==NULL || child==NULL)
    {
      LOG("ERROR!! Failed adding edge between two nodes, either parent or child is null \n");
      return false;
    }


  vector<DAGNode *> all_parents = getVectorOfAncestorNodesInDFSOrder(parent); //DFS or BFS does not matter
  for(int i=0;i<all_parents.size();i++)
    {
      if(all_parents[i] ==child)
	{
	  LOG("Found child in the ancestors of parent, cannot add edge \n");
	  return false;
	}
    }
  parent->out_list.push_back(child);
  child->in_list.push_back(parent);
  child->reference_count ++;
  if(restricted_mode)
    {
      bool correct_sign = RecomputeSignatureOfANode(parent);
      if(!correct_sign)
	{
	  LOG("ERROR!! Failed recomputing the signature of the parent \n");
	return false;
	}
    }
  LOG("Successfully Added edge between two nodes \n");
  return true;

}





/**
 * Synopsis: Public function that removes edge between two nodes...(User-Maintained labels mode only)
 *
 * Description: This function allows to remove edge between two nodes. It will remove FIRST edge between the two nodes.
 *
 * Arguments: DAGNode *parent: Node to be made parent
 *            DAGNode *child: Node to be made child
 *
 * Return value: True, if it succeeds, false otherwise
 */

int DAGManager :: removeEdge(DAGNode *parent, DAGNode *child)
{
  LOG("Removing edge between two nodes \n");
  int edge_counter = -1;
  if(parent==NULL || child==NULL)
    {
      LOG("ERROR!! Failed adding edge between two nodes \n");
      LOG("Either parent or child is null\n");
      return edge_counter;
    }



  for(list<DAGNode*>::iterator it=parent->out_list.begin();it!=parent->out_list.end();it++)
    {
      if(*it == child)
	{
	  parent->out_list.erase(it);
	  break;
	}
    }
  for(list<DAGNode*>::iterator it=child->in_list.begin();it!=child->in_list.end();it++)
    {
      if(*it == parent)
	{
	  child->in_list.erase(it);
	  break;
	}
    }

  if(restricted_mode)
    {
      //Now signature of the parent has changed. So delete and reinsert it in the hashtable
      bool correct_sign = RecomputeSignatureOfANode(parent);
      if(!correct_sign)
	{
	  LOG("ERROR!! Failed recomputing the signature of the parent \n");
	  return -1;
	}

      child->reference_count --;
    }
  LOG("Successfully removed edge between two nodes \n");
  return edge_counter;

}







/**
 * Synopsis: Public function to allow resetting of visited flags of DAGNodes
 *
 * Description: This function is just an alias for ResetVisitedFlagOfAllDAGNodes, except that it is public. During external traversals, a client will need to reset the visited flag of DAG. However, since the client is also allowed to set/reset the visited flag, in order to ensure the correctness, we should reset the visited flag of all the DAGNodes by traversing the linked list of all nodes. Hence it is implemented this way....
 *
 *Return Value: true if it succeeds, false otherwise
 *
 * Precondition: None!!
 *
 * Postcondition: visited flag of all nodes is resetted
 */



bool DAGManager :: ResetVisitedFlagOfDAGNodes()
{
  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();
  return reset_visited;

}






/**
 * Synopsis: Private function used to reset visited flag of a DAG!
 *
 * Description: Several functions in DAGManager have a precondition that the visited flag of nodes should be resetted.
 * Use this function to reset the visited flag of nodes of a DAG.
 *
 * Arguments: Non Null DAGNode *
 *
 * Precondition: The visited flags of all nodes in the DAG must be consistent. Here, by consistent, we mean all either set or reset.
 * Note that the function operates recursively. If visited flag of a node is reset but not of its child, then this function will not work correctly. (In such cases, one should invoke the ResetVisitedFlagOfAllDAGNodes function that traverses the linked list of DAGNodes and resets their visited flag). 
 * However, when you are sure that the flags are consistent (e.g. After a call to markSharedNodes), you can bypass the overhead of resetting visited flag of all DAGNodes in the entire DAG and use this function.
 *
 * Complexity: O(number of nodes in the DAG starting from root "node")
 *
 * Return value: true if it succeeds, false otherwise
 *
 * See Also: ResetVisitedFlagOfAllDAGNodes
 */


bool DAGManager :: ResetVisitedFlagOfDAG(DAGNode *node)
{
  LOG("Resetting visited flag of DAG\n");
  if(node == NULL)
    {
      LOG("ERROR!! Failed resetting visited flag of DAG\n");
      LOG("Node is null\n");
      return false;
    }
  node->visited = true;
  return ResetVisitedFlagOfDAGRecursively(node);
}




/**
 * Synopsis: Private function used to reset visited flag of a DAG!
 *
 * Description: This recursive function actually performs resetting of the flag!!
 *
 * Arguments: Non Null DAGNode *
 *
 * Return value: true if it succeeds, false otherwise
 */

bool DAGManager :: ResetVisitedFlagOfDAGRecursively(DAGNode *node)
{
  if((node->visited == false) && (node->isShared==true))
    {
      return true;
    }
  node->visited = false;
  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      if(ResetVisitedFlagOfDAGRecursively(*it) == false)
        return false;
    }
  return true;
}








/**
 */





vector<DAGNode *> DAGManager ::getVectorOfDescendentNodesInDFSOrder(DAGNode *node)
{

  vector<DAGNode *> nodes;
  nodes.clear();
  bool reset_visited = ResetVisitedFlagOfDAG(node);
  if(reset_visited == false)
    {
      LOG("Could not reset visited flag of DAG\n");
      return nodes;
    }
  nodes = getChildrenInDFSRecursively(node);
  return nodes;
}


vector<DAGNode *> DAGManager :: getChildrenInDFSRecursively(DAGNode * node)
{
  vector<DAGNode *> nodes;
  if(node->visited == true)
    return nodes;
  node->visited = true;
  if(node->out_list.size() ==0)
    {
      nodes.push_back(node);
      return nodes;
    }
  nodes.push_back(node);
  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      vector<DAGNode *> child_nodes = getChildrenInDFSRecursively(*it);
      for(int i=0;i<child_nodes.size();i++)
        nodes.push_back(child_nodes[i]);
    }

  return nodes;
}


vector<DAGNode *> DAGManager :: getVectorOfAncestorNodesInDFSOrder(DAGNode *node)
{

  vector<DAGNode *> nodes;
  nodes.clear();
  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();
  if(reset_visited == false)
    return nodes;
  nodes = getParentsInDFSRecursively(node);
  return nodes;
}
vector<DAGNode *> DAGManager :: getParentsInDFSRecursively(DAGNode * node)
{
  vector<DAGNode *> nodes;
  if(node->visited == true)
    return nodes;
  node->visited = true;

  nodes.push_back(node);
  for(list<DAGNode*>::iterator it=node->in_list.begin();it!=node->in_list.end();it++)
    {
      vector<DAGNode *> child_nodes = getParentsInDFSRecursively(*it);
      for(int i=0;i<child_nodes.size();i++)
        nodes.push_back(child_nodes[i]);
    }
  return nodes;
}







vector<DAGNode *> DAGManager :: getVectorOfDescendentNodesInBFSOrder(DAGNode *node)
{
  list<DAGNode *> bfs_queue;
  vector<DAGNode *> bfs_nodes;

  bfs_queue.clear();
  if(node == NULL)
    return bfs_nodes;
  bfs_queue.push_back(node);
  bfs_nodes.push_back(node);
  bool reset_visited = ResetVisitedFlagOfDAG(node);
  if(reset_visited == false)
    return bfs_nodes;

  DAGNode *current_node;
  do
    {
      current_node = bfs_queue.front();
      bfs_queue.pop_front();
      if(current_node->visited == true)
        {
          continue;
        }
      current_node->visited = true;

      for(list<DAGNode*>::iterator it=current_node->out_list.begin();it!=current_node->out_list.end();it++)
        {
          if((*it)->visited == false)
            {
              bfs_queue.push_back(*it);
              bfs_nodes.push_back(*it);
            }
        }

    }
  while(bfs_queue.size() > 0) ;


  return bfs_nodes;
}




vector<DAGNode *> DAGManager :: getVectorOfAncestorNodesInBFSOrder(DAGNode *node)
{
  list<DAGNode *> bfs_queue;
  vector<DAGNode *> bfs_nodes;

  bfs_queue.clear();
  if(node == NULL)
    return bfs_nodes;
  bfs_queue.push_back(node);
  bfs_nodes.push_back(node);
  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();;
  if(reset_visited == false)
    return bfs_nodes;


  DAGNode *current_node;
  do
    {
      current_node = bfs_queue.front();
      bfs_queue.pop_front();
      if(current_node->visited == true)
        {
          bfs_queue.pop_front();
          continue;
        }
      current_node->visited = true;

      for(list<DAGNode*>::iterator it=current_node->in_list.begin();it!=current_node->in_list.end();it++)
        {

          bfs_queue.push_back(*it);
          bfs_nodes.push_back(*it);
        }

    }
    while(bfs_queue.size() > 0) ;
  return bfs_nodes;
}












/**
 * Synopsis: Public function that tells if the given DAG is a Tree
 *
 * Description: This function performs a simple test, it counts the number of shared nodes in the DAG. If they are 0, then the DAG is indeed a tree...
 *
 * Arguments: Non-null DAGNode
 *
 * Return value: True, if it succeeds, false otherwise
 */


bool DAGManager :: isDAGATree(DAGNode *node)
{
  if(node == NULL)
    return false;

  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();
  if(reset_visited == false)
    return false;


  int count_of_shared_nodes = markSharedNodes(node);
  //LOG("Value of count is "<<count<<"\n";
  if(count_of_shared_nodes == 0)
    return true;
  return false;
}




/**
 * Synopsis: Test if the node_to_test is ancestor of ancestor_of
 *
 * Description: this function obtains list of all ancestor nodes of ancestor_of and searches it if it finds the node_to_test in it
 *
 * Return value: true if it succeeds, false otherwise
 *
 */


bool DAGManager :: isAncestor(DAGNode *node_to_test, DAGNode *ancestor_of)
{
  if((node_to_test ==NULL) ||(ancestor_of ==NULL))
    {
      return false;
    }

  vector<DAGNode *> all_parents = getVectorOfAncestorNodesInDFSOrder(ancestor_of);
  for(int i=0;i<all_parents.size();i++)
    {
      if(all_parents[i] == node_to_test)
        return true;
    }
  return false;
}






/**
 * Synopsis: Test if the node_to_test is descendent of descendent_of
 *
 * Description: this function obtains list of all descendent nodes of ancestor_of and searches it if it finds the node_to_test in it. This implementation uses DFS order, but it is really immaterial.
 *
 * Return value: true if it succeeds, false otherwise
 *
 */


bool DAGManager :: isDescendent(DAGNode *node_to_test, DAGNode *descendent_of)
{

  if((node_to_test ==NULL) ||(descendent_of ==NULL))
    {
      return false;
    }

  vector<DAGNode *> all_children = getVectorOfDescendentNodesInDFSOrder(descendent_of);
  for(int i=0;i<all_children.size();i++)
    {
      if(all_children[i] == node_to_test)
        return true;
    }
  return false;

}





/**
 * Synopsis: Get the ith child of a node
 *
 * Description: this function returns handle to ith child of a node. 
 *
 * Return value: NULL if it fails, DAGNode * to the ith child if it succeeds
 *
 */


DAGNode * DAGManager :: getIThChildOfNode(DAGNode *node, int i)
{
  if(i<0)
    return NULL;
  if(node->out_list.size() == 0)
    return NULL;
  int counter = 0;
  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      if(counter == i)
        return (*it);
      counter++;
    }
  return NULL;
}



/**
 * Synopsis: Obtain handle to a node with given label
 *
 * Description: This function returns a handle to a DAGNode with given label. For restricted mode, it searches for the leaf nodes. For user defined mode, it may return a non-leaf node.
 *
 *Return value: A NULL if it cannot search the node with given specs, DAGNode * to node if it finds one
 */

DAGNode * DAGManager :: getNodeWithLabel(string label)
{


  if( label=="" )
    return NULL;
  

  string signature = "";
  if(!restricted_mode)
    {
      LOG("Searching a DAGNode with label"+label+"\n");
      signature = label;
    }
  else
    {
      LOG("Searching a leaf DAGNode with label"+label+"\n");
      signature = createSignature(label);
    }
    
  HTStatusValue <DAGNode *> search_signature = ht.hashtable_search(signature);
  if(search_signature.success())
    {
      DAGNode *result_node = search_signature.GetValue();
      return result_node;
    }
  else
    return NULL;

}

/**
 * Synopsis: Test whether a node is parent of another
 *
 * Description: This function checks if a DAGNode node_to_test is parent of the DAGNode parent_of
 *
 * Return Value: True if the test succeeds, false otherwise
 *
 *
 */

bool DAGManager :: isParent(DAGNode *node_to_test, DAGNode *parent_of)
{
  if((node_to_test ==NULL) ||(parent_of ==NULL))
    {
      return false;
    }
  for(list<DAGNode*>::iterator it=node_to_test->out_list.begin();it!=node_to_test->out_list.end();it++)
    {
      if(parent_of == (*it))
        return true;
    }
  return false;
}




/**
 * Synopsis: Test whether a node is child of another
 *
 * Description: This function checks if a DAGNode node_to_test is child of the DAGNode child_of
 *
 * Return Value: True if the test succeeds, false otherwise
 *
 *
 */

bool DAGManager :: isChild(DAGNode *node_to_test, DAGNode *child_of)
{
  if((node_to_test ==NULL) ||(child_of ==NULL))
    {
      return false;
    }

  for(list<DAGNode*>::iterator it=child_of->out_list.begin();it!=child_of->out_list.end();it++)
    {
      if(node_to_test == (*it))
        return true;
    }

  return false;
}



/**
 * Synopsis: Determine the memory consumed by the DAG
 *
 * Description: This function computes the memory consumed by the DAG(s)
 *
 * Return Value: An unsigned long integer representing number of bytes of memory used..
 *
 *
 */

unsigned long int DAGManager :: getMemoryUsage()
{
  return memory_usage;

}








/**
 * Synopsis : Delete a node from hashtable
 *
 * Description: This function deletes a node from the hashtable
 *
 *
 *
 */




bool DAGManager :: deleteANodeFromHashTable(DAGNode *node_to_delete)
{
  string signature = node_to_delete->signature;
  HTStatusValue<DAGNode *> delete_result = ht.hashtable_delete(signature);
  if(delete_result.success())
    {
      LOG("Node deleted from hashtable successfully\n");
      return true;
    }
  else
    {
      LOG("Node NOT deleted from hashtable \n");
      return false;
    }

  return true;
}





/**
 * Synopsis: Get the list of Root nodes
 *
 * Description: The DAGManager maintains a bunch of DAGs and not just a single DAG. It internally keeps track of all the roots of the DAGs from which the entire DAG is reachable. (These roots may have been returned by the DAGManager when corresponding DAGs were created...)
 * This function simply returns the list of root nodes in a vector maintained by it..
 *
 *
 * Return value: The set of roots maintained by the DAGManager in a STL vector
 *
 */
vector<DAGNode*> DAGManager :: getVectorOfRootNodes()
{
  vector<DAGNode *> roots;
  DAGNode *current_node = head_roots.next_root;

  while(current_node!=NULL)
    {
      roots.push_back(current_node);
      current_node = current_node->next_root;
    }
  return roots;
}
















/**
 * Synopsis: Add child at position i to parent
 *
 * Description: This function adds a "child" child to "parent" parent at position i
 *
 * Return value: true if it succeeds , false otherwise
 *
 * Postcondition: shared flag of nodes may be incorrect after a call to this function. Use markSharedNodes function to obtain correct values for it.
 */

bool DAGManager :: addNodeAsIthChild(DAGNode *parent, DAGNode *child,int i)
{

  if((parent ==NULL) || (child ==NULL))
    {
      return false;
    }
  if(i<0)
    {
      return false;
    }
  if(i>parent->out_list.size()+1)
    {
      return false;
    }



  if(restricted_mode)
    {
      int counter = 0;
      list<DAGNode*>::iterator it;
      for(it=parent->out_list.begin();it!=parent->out_list.end();it++)
        {
          if(counter==i)
            break;
          counter++;
        }
      parent->out_list.insert(it, child);
      child->reference_count ++;

      if(child->isRoot)
        {
          bool deleted_from_roots = deleteNodeFromRootsList(child);
          if(deleted_from_roots == false)
            {
              return false;
            }

        }
      //Signature of the parent node changed, Recompute it
      bool correct_sign = RecomputeSignatureOfANode(parent);
      if(!correct_sign)
	return false;
      return true;
    }
  else //!restricted mode
    {

      int counter = 0;
      list<DAGNode*>::iterator it;
      for(it=parent->out_list.begin();it!=parent->out_list.end();it++)
        {
          if(counter==i)
            break;
          counter++;
        }
      parent->out_list.insert(it, child);
      child->reference_count ++;

      if(child->isRoot)
        {
          bool deleted_from_roots = deleteNodeFromRootsList(child);
          if(deleted_from_roots == false)
            {
              return false;
            }

        }
      //No signatures change!!
    }
}










/**
 * Synopsis: Re-Compute the signature of a node
 *
 * Description: If the signature of a node changes, it has to be recomputed. Note that after recomputing the signature, we retain the address of the node whose signature is being recomputed, and therefore signature of its parents does not change!!
 * This function is restricted mode function, and does not work in user-defined labels mode
 *
 * Return Value: True if it succeeds, false if it fails, or when called in user-defined labels mode
 */




bool DAGManager :: RecomputeSignatureOfANode(DAGNode *node)
{
  LOG("Reomputing signature of a node \n");
  if(node == NULL)
    {
      LOG("Node is Null\n");
      return false;
    }
  if(!restricted_mode)
    {
      LOG("ERROR!! Attempt to change signature of a node is user-defined labels mode\n");
      return false;
    }

  //Delete the hashtable entry using the old signature
  string current_signature = node->signature;
  HTStatusValue<DAGNode *> delete_result = ht.hashtable_delete(current_signature);
  if(delete_result.success())
    {
      LOG("Node deleted from hashtable successfully  "+node->label+"\n");
    }
  else
    {
      LOG("ERROR!! Node NOT deleted from hashtable  "+node->label+"\n");
      return false;
    }
  
  vector<DAGNode *> v_operands;
  LOG("Creating v_operands\n");
  //create v_operands considering the newly added child 
  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      
      v_operands.push_back(*it);
    }
  
  if(v_operands.size()==0)
    LOG("WARNING!! Changing signature of a leaf node\n");

  LOG("Creating signature for new parent "+node->label+"\n");
  
  string new_signature;
  new_signature = createSignature(node->label, v_operands);

  
  HTStatusValue <DAGNode *> search_signature = ht.hashtable_search(new_signature);
  if(search_signature.success())
    {
      LOG("Found the node in the hashtable\n");
      //Replace the node by the node whose signature is equal to this node's new signature
      DAGNode *replaceBy = search_signature.GetValue();
      //Modify every parent of node to point to replace_by instead of node
      for(list<DAGNode*>::iterator it=node->in_list.begin();it!=node->in_list.end();it++)
	{
	  DAGNode *current_parent = *it;
	  for(list<DAGNode*>::iterator parentit=current_parent->out_list.begin();parentit!=current_parent->out_list.end();parentit++)
	    {
	      if(*parentit == node)
		{
		  list<DAGNode *>:: iterator to_insert_at = current_parent->out_list.erase(parentit);
		  current_parent->out_list.insert(to_insert_at, replaceBy);
		  replaceBy->in_list.push_back(*parentit);
		  break; //only one at a time!!
		}
	    }
	}

      return true;
    }
  else
    {
      LOG("Did not find the node in the hashtable\n");
      HTStatusValue <DAGNode *> insert_result = ht.hashtable_insert(new_signature, node);
      if(insert_result.success())
	{
	  LOG("Successfully inserted node in the hashtable\n");
	  node->signature = new_signature;
	  //only Leaf can change, rest all flags will retain their values
	  if(v_operands.size() == 0)
	    node->isLeaf = true;
	  return true;
	}
      else
	{
	  LOG("ERROR!! Failed inserting the node in the hashtable\n");
	  return false;
	}
    }

LOG("Successfully recomputed the signature of the node\n");
  return true;
}







/**
 *Synopsis: Logging function
 *
 * Description: The purpose of this function is to log a message to log file.
 * This has been extracted out as a seperate function just in order to allow enabling/disabling of logging process.
 * Since the logging is a file operation, it is estimated to take fair amount of time. Therefore it is customary to disable it, if one is sure about the DAG operations correctness
 * Fatal messages will still be thrown to stderr/cerr
 */

void DAGManager :: LOG(string message)
{
  if(enable_logging)
    *dagmanager_log << message;
}




/**
 * Synopsis: Public function to copy a DAG
 *
 * Description: This function allows to copy a DAG into new DAG. Note that the way new and old DAG differentiate, is based on difference in their labels. Hence it uses a "prefix", with which it modifies the lables of the original DAG. The prefix is completely internal and not visible to client otherwise.
 *
 * Arguments: Non-null DAGNode
 *
 * Return value: Handle to copied DAG's root, if suceess
 *               NULL otherwise
 *
 * See Also : copy
 *
 */


static map<unsigned long int, unsigned long int> copy_map;
DAGNode * DAGManager :: copyDAG(DAGNode *node)
{
  LOG("Copying the DAG\n");
  copy_prefix_counter ++;
  string copy_prefix = integerToString(copy_prefix_counter);

  if(node==NULL)
    {
      LOG("Node is null\n");
      return NULL;
    }
  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();
  if(reset_visited == false)
    {
      LOG("ERROR!! Failed resetting visited flag of DAG\n");
      return NULL;
    }
  copy_map.clear();
  DAGNode * copied_node = copy(node, copy_prefix);
  copy_map.clear();
  LOG("Successfully copied the DAG\n");
  return copied_node;
}



/**
 * Synopsis: PRIVATE function used during copying of DAG
 *
 * Description: This function actually performs copy of DAG by recursively traversing the original DAG.
 *
 * Arguments:
 *
 * Return value: Handle to copied DAG's root, if suceess
 *               NULL otherwise

 * See Also: copyDAG
 *
 */

DAGNode * DAGManager :: copy(
                             DAGNode *node, //(sub)DAG to copy
                             string copy_prefix //copy prefix
                             )
{
  if(node->visited == true)
    {
      return (DAGNode *) copy_map[(unsigned long int)node];

    }
  node->visited = true;
  vector<DAGNode *> operands;

  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      DAGNode *child = *it;
      operands.push_back(copy(*it,copy_prefix));
    }

  DAGNode *copied_node;
  if(operands.size()==0)
    {
      copied_node = createNode(copy_prefix + node->label);
      copied_node->copy_count = copy_prefix_counter;
      copied_node->value = node->value;//shallow copy
    }
  else
    {
      copied_node = createNode(copy_prefix + node->label, operands);
      copied_node->copy_count = copy_prefix_counter;
      copied_node->value = node->value;//shallow copy
    }
  copy_map.insert(make_pair((unsigned long int )(node), (unsigned long int)(copied_node)));
  return copied_node;

}










/**
 * Synopsis: Public function to copy a DAG with values
 *
 * Description: This function allows to copy a DAG into new DAG including the values...This function requires a function pointer that defines how should the value be copied..Note that the way new and old DAG differentiate, is based on difference in their labels. Hence it uses a "prefix", with which it modifies the lables of the original DAG. The prefix is completely internal and not visible to client otherwise.
 *
 * Arguments: Non-null DAGNode
 *
 * Return value: Handle to copied DAG's root, if suceess
 *               NULL otherwise
 */

DAGNode * DAGManager :: copyDAGWithValues(DAGNode *node,void * (*copyfunc)(void *value_to_copy) )
{
  LOG("Copying the DAG with values\n");
  string copy_prefix = integerToString(copy_prefix_counter+1);
  if(node==NULL)
    {
      LOG("Root node is null\n");
      return NULL;
    }
  bool reset_visited = ResetVisitedFlagOfAllDAGNodes();
  if(reset_visited == false)
    {
      LOG("ERROR!! Failed resetting visited flag of DAGNodes \n");
      return NULL;
    }
  copy_map.clear();
  DAGNode *copied_root = copyWithValues(node, copy_prefix, copyfunc);
  copy_map.clear();
  LOG("Successfully copied DAG with values\n");
  return copied_root;
}




/**
 * Synopsis: PRIVATE function used during copying of DAG
 *
 * Description: This function actually performs copy of DAG by recursively traversing the original DAG.
 *
 * Arguments:
 *
 * Return value: Handle to copied DAG's root, if suceess
 *               NULL otherwise
 */

DAGNode * DAGManager :: copyWithValues(
                                       DAGNode *node, //(sub)DAG to copy
                                       string copy_prefix, //copy prefix
                                       void * (*copyfunc)(void *value_to_copy)
                                       )
{
  if(node->visited == true)
    {
      return (DAGNode *)copy_map[(unsigned long int)node];
    }
  node->visited = true;
  vector<DAGNode *> operands;

  for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
    {
      DAGNode *child = *it;
      operands.push_back(copyWithValues(*it,copy_prefix, copyfunc));
    }

  DAGNode *copied_node;
  if(operands.size()==0)
    {
      copied_node = createNode(copy_prefix + node->label);
      copied_node->copy_count ++;
      copied_node->value = copyfunc(node->value);//deep copy
    }
  else
    {
      copied_node = createNode(copy_prefix + node->label, operands);
      copied_node->copy_count ++;
      copied_node->value = copyfunc(node->value);//deep copy
    }
  copy_map.insert(make_pair((unsigned long int) (node),(unsigned long int)(copied_node)));
  return copied_node;

}




/**
 * Synopsis: Public function that allows to replace a leaf by another DAG
 *
 * Description: This function allows to replace a leaf by other DAG. Note carefully the preconditions
 * 1. Both node_to_replace and node_replace_by must be non-null
 * 2. out_list of node_to_replace must b1 0. i.e. the reader must be a leaf
 * 3. in_list of node_replace_by must b1 0. i.e. the writer must be a root
 * Then this function replaces the reader by the write such that the effect is visibe in entire DAG. i.e. reader is no more part of ANY dag.
 *
 *
 * Arguments: Non-null DAGNodes toReplace and replaceBy that satisfy above written conditions
 *
 * Return value: True, if it succeeds, false otherwise
 *
 * Postcondition: shared flag of nodes may be incorrect after a call to this function. Use markSharedNodes function to obtain correct values for it.

 */

bool DAGManager :: ReplaceLeafByDAG(DAGNode *toReplace, DAGNode *replaceBy)
{

  LOG("Invoking ReplaceLeafByDAG\n");
  if((toReplace==NULL) || (replaceBy==NULL))
    {
      LOG("Either toReplace or replaceBy is NULL\n");
      return false;
    }
  if(toReplace == replaceBy)
    {
      LOG("toreplace and replaceby are equal\n");
      return true;
    }

  /*
  if(replaceBy->in_list.size() != 0) //Is this requirement still needed?????
    {
      LOG("in-list of replaceBy has size Non-0\n");
      return false;
    }
  */

  if(toReplace->out_list.size() !=0)
    {
      LOG("out-list of toReplace has size Non-0\n");
      return false;
    }



  LOG("All tests passed for replacement\n");




  LOG("Releasing the node to replace\n");
  for(list<DAGNode*>::iterator it=toReplace->in_list.begin();it!=toReplace->in_list.end();it++)
    {
      DAGNode *current_parent = *it;
      for(list<DAGNode*>::iterator parentit=current_parent->out_list.begin();parentit!=current_parent->out_list.end();parentit++)
        {
          if(*parentit == toReplace)
            {

              list<DAGNode *>:: iterator to_insert_at = current_parent->out_list.erase(parentit);
              current_parent->out_list.insert(to_insert_at, replaceBy);
              replaceBy->in_list.push_back(*it);
              break; //only one at a time!!
            }
        }
    }


  //At this stage, signature of every parent has changed.. so delete and insert every parent from the hashtable. Note that the address of parent is to be retained. So DO not delete the actual node, or create any new node, just delete the hashtable entry. (Note that the DAGNodes are created outside the hashtable and are just inserted in it. So deleting a hashtable entry does not delete the DAGNode itself!!


  if(restricted_mode)
    {
      LOG("Changing the signature of every parent ....\n");
      for(list<DAGNode*>::iterator it=replaceBy->in_list.begin();it!=replaceBy->in_list.end();it++)
	{
	  bool correct_sign = RecomputeSignatureOfANode(*it);
	  if(!correct_sign)
	    return false;
	}
    }

  //  deleteDAG(toReplace);
  LOG("Successfully replaced Leaf by DAG\n");
  return true;
}








/**
 * Synopsis: Replace child_to_replace by child_replace_by
 *
 *
 * Postcondition: shared flag of nodes may be incorrect after a call to this function. Use markSharedNodes function to obtain correct values for it.

 **/

bool DAGManager :: ReplaceChild(DAGNode *parent, DAGNode *child_to_replace, DAGNode *child_replace_by)
{

  /*          parent_node
              /   \     \
             /     \     \
            *   c_t_repl  *
                         <-------c_repl_by
 


              Replace the child c_t_repl by c_repl_by.

              In this process, signaure of only parent_node changes
              +
              There are no constraints on what child_to_replace and child_replace_by are
  */

  LOG("Replacing a child \n");
  if(parent == NULL || child_to_replace==NULL || child_replace_by ==NULL)
    {
      LOG("Either parent or child_to_replace or child_replace_by is NULL\n");
      return false;
    }

  //First check if child_to_replace is really child of node parent
  for(list<DAGNode*>::iterator parentit=parent->out_list.begin();parentit!=parent->out_list.end();parentit++)
    {
      if(*parentit == child_to_replace)
	break;
      else
	{
	  LOG("child_to_replace is not a child of parent\n");
	  return false;
	}
    }




  if(restricted_mode)
    {


      for(list<DAGNode*>::iterator parentit=parent->out_list.begin();parentit!=parent->out_list.end();parentit++)
        {
          if(*parentit==child_to_replace)
            {
              list<DAGNode*>::iterator insert_at= parent->out_list.erase(parentit);
              parent->out_list.insert(insert_at, child_replace_by);
            }
        }

      //Remove parent from the in-list of child_to_replace
      for(list<DAGNode*>::iterator it=child_to_replace->in_list.begin();it!=child_to_replace->in_list.end();it++)
        {
          if(*it==parent)
            {
              child_to_replace->in_list.erase(it);
              break;
            }
        }


      if(child_replace_by->isRoot)
        {
          bool deleted_from_roots = deleteNodeFromRootsList(child_replace_by);
          if(deleted_from_roots == false)
            {
	      LOG("Could not delete child_replace_by from list of roots\n");
              return false;
            }

        }

      //Add parent to the in-list of child_replace_by
      child_replace_by->in_list.push_back(parent);

      //Now the signature of the parent has changed!!!

      bool correct_sign = RecomputeSignatureOfANode(parent);
      if(!correct_sign)
	{
	  LOG("ERROR!! Failed recomputing signature of parent\n");
	  return false;
	}
      LOG("Successfully replaced child \n");
      return true;
    }
  else //!restricted mode
    {

      for(list<DAGNode*>::iterator parentit=parent->out_list.begin();parentit!=parent->out_list.end();parentit++)
        {
          if(*parentit==child_to_replace)
            {
              list<DAGNode*>::iterator insert_at= parent->out_list.erase(parentit);
              parent->out_list.insert(insert_at, child_replace_by);
            }
        }

      //Remove parent from the in-list of child_to_replace
      for(list<DAGNode*>::iterator it=child_to_replace->in_list.begin();it!=child_to_replace->in_list.end();it++)
        {
          if(*it==parent)
            {
              child_to_replace->in_list.erase(it);
              break;
            }
        }


      if(child_replace_by->isRoot)
        {
          bool deleted_from_roots = deleteNodeFromRootsList(child_replace_by);
          if(deleted_from_roots == false)
            {
	      LOG("Could not delete child_replace_by from list of roots\n");
              return false;
            }

        }

      //Add parent to the in-list of child_replace_by
      child_replace_by->in_list.push_back(parent);
      LOG("Successfully replaced child \n");
      return true;
    }
}









/**
 * Synopsis: Public function that allows to merge node toReplace and replaceBy
 *
 * Description: This function allows merging two functionally equivalent nodes. Note carefully the preconditions
 * 1. Both node_to_replace and node_replace_by must be non-null
 * The functional equivalance of two nodes is user determined...e.g. A DAG representing expression (a+0) is functionally equivalent to a DAG representing expression a, and one would want to replace the earlier by the later. After such replacement, he would want to see every DAG referring to DAG (a+0) as (a). This function allows this kind of merging.
 * Note that this function deletes the DAG toReplace (i.e. a+0) in its operation. External handles (references created to expression a+0) are no more valid. The DAGManager simply cannot redirect external references to point to DAG (a) because it does not have back-pointers to them. (However, all internal references will be redirected, as expected). So be careful with these handles in your code.
 *
 *
 * Arguments: Non-null DAGNodes toReplace and replaceBy that satisfy above written conditions
 *
 * Return value: True, if it succeeds, false otherwise
 *
 * Postcondition: shared flag of nodes may be incorrect after a call to this function. Use markSharedNodes function to obtain correct values for it.

 */

bool DAGManager :: MergeEquivalentNodes(DAGNode *toReplace, DAGNode *replaceBy)
{
 
  LOG("Invoking MergeEquivalentNodes\n");
  if((toReplace==NULL) || (replaceBy==NULL))
    {
      LOG("Either toReplace or replaceBy is NULL\n");
      LOG("ERROR!! Merging Failed\n");
      return false;
    }
  if(toReplace == replaceBy)
    {
      LOG("toreplace and replaceby are equal\n");
      LOG("Merging Successful\n");
      return true;
    }

  LOG("All tests passed for merging\n");

  //At this stage, what we should do is that 
  //1. Detach the toReplace node and replace it by replaceBy AT EXACTLY SAME locations in the DAG


  LOG("Releasing the node to replace and replacing it ....\n");
  for(list<DAGNode*>::iterator it=toReplace->in_list.begin();it!=toReplace->in_list.end();it++)
    {
      DAGNode *current_parent = *it;
      for(list<DAGNode*>::iterator parentit=current_parent->out_list.begin();parentit!=current_parent->out_list.end();parentit++)
        {
          if(*parentit == toReplace)
            {
              list<DAGNode *>:: iterator to_insert_at = current_parent->out_list.erase(parentit);
              current_parent->out_list.insert(to_insert_at, replaceBy);
              replaceBy->in_list.push_back(*it);
              break; //only one at a time!!
            }
        }
    }


  //At this stage, signature of every parent has changed.. so delete
  //and insert every parent from the hashtable. Note that the address
  //of parent is to be retained. So DO not delete the actual node, or
  //create any new node, just delete the hashtable entry. (Note that
  //the DAGNodes are created outside the hashtable and are just
  //inserted in it. So deleting a hashtable entry does not delete the
  //DAGNode itself!!


  if(restricted_mode)
    {
      LOG("Changing the signature of every parent ....\n");
      for(list<DAGNode*>::iterator it=replaceBy->in_list.begin();it!=replaceBy->in_list.end();it++)
	{
	  bool correct_sign = RecomputeSignatureOfANode(*it);
	  if(!correct_sign)
	    return false;
	}
    }
  LOG("Deleting the DAG toReplace\n");
  deleteDAG(toReplace);
  LOG("Successfully merged two nodes in a DAG\n");
  return true;
}










/**
 * Synopsis: Obtain height of DAG by assigning every node a height equal to length of shortest path to this node starting from given root
 *
 * Description: The height of a DAGNode is defined as the length of shortest path to this node starting from given root.
 * This function performs a BFS. It uses the fact that a node at level i+1 is reached in a BFS only when all nodes at level i are already visited.
 * Note that this is a DAG, and there are no cycles.
 *
 * Return value: An integer giving the height of DAG, with root having height 1
 * -1 if it fails
 *
 */


int DAGManager ::getHeightOfDAGWithMinimumPaths(DAGNode *root)
{
  if(root == NULL)
    {
    return -1;
    }
  bool reset_visited = ResetVisitedFlagOfDAG(root);
  if(!reset_visited)
    {
      return -1;
    }

  vector<DAGNode *> nodes_at_current_level;
  vector<DAGNode *> nodes_at_next_level;
  int current_level = 1;
  nodes_at_current_level.push_back(root);
  root->visited = true;

  //Always, elements contained in the nodes_at_current_level have visited flag true
  while(1)
    {
      vector<DAGNode *>::iterator it;
      for(it = nodes_at_current_level.begin(); it!= nodes_at_current_level.end(); it++)
	{
	  DAGNode *current_node = *it;
	  list<DAGNode *>:: iterator child_it;
	  for(child_it = current_node->out_list.begin(); child_it!= current_node->out_list.end(); child_it++)
	    {
	      DAGNode *child_to_add_to_next_level = *child_it;
	      if(child_to_add_to_next_level->visited==true)
		{
		  continue;
		}
	      else
		{
		  nodes_at_next_level.push_back(child_to_add_to_next_level);
		  child_to_add_to_next_level->visited = true;
		}
	    }
      
	}

      if(nodes_at_next_level.size() == 0)
	{
	  return current_level;
	}
      else
	{
	  current_level++;
	  nodes_at_current_level.clear();
	  for(it = nodes_at_next_level.begin(); it!= nodes_at_next_level.end(); it++)
	    {
	      nodes_at_current_level.push_back(*it);
	    }
	  nodes_at_next_level.clear();
	}
    }
  return -1;
}





/**
 * Synopsis: Private function that collects leaves
 *
 * Description: This is a private function used by getVectorOfLeafNodes that recursively traverses the DAG and collects the leaves of the DAG
 *
 * Return Value: The leaf nodes of the DAG in a STL vector
 */


vector<DAGNode*> DAGManager :: collectLeavesRecursively(DAGNode* node)
{
  vector<DAGNode *> leaves;
  leaves.clear();
  if(node->visited==true)
    {
      return leaves;
    }
  else
    {
      node->visited = true;
      if(node->isLeaf || (node->out_list.size()==0))
        {
          leaves.push_back(node);
          return leaves;
        }
      else
        {
          for(list<DAGNode*>::iterator it=node->out_list.begin();it!=node->out_list.end();it++)
            {
              vector <DAGNode *> leaves_of_children = collectLeavesRecursively(*it);
              for(int i=0;i<leaves_of_children.size();i++)
                {
                  leaves.push_back(leaves_of_children[i]);
                }
            }
          return leaves;
        }
    }
}




/**
 * Synopsis: Collect Leaves of a DAG
 *
 * Description: This function traverses the DAG pointed by node, and collects all the leaf nodes in that DAG. It returns this list of leaves. This function resets the visited flag of the DAG and invokes collectLeavesRecursively to actually collect the leaves
 *
 * Return Value: A empty/non-empty STL vector of DAGNode * which are leaves of this DAG
 */


vector<DAGNode *> DAGManager :: getVectorOfLeafNodes(DAGNode *node)
{
  vector <DAGNode *> leaves;
  bool reset_visited = ResetVisitedFlagOfDAG(node);
  if(reset_visited == false)
    return leaves;

  leaves = collectLeavesRecursively(node);
  return leaves;
}






/** Constructors of the DAGNode*/

DAGNode:: DAGNode(void *value_to_set) //Default Constructor
{
  label = "";
  //Nothing to do with reference_count here

  //Nothing to do with signature here
  //no in-list
  //no out-list
  visited = false;
  //nothing to do with shared flag here
  isRoot = true;
  isLeaf = true;
  copy_count = 0;
  value = value_to_set;
  temp_id = 0;
}

DAGNode :: DAGNode(string label_to_assign, void *value_to_set)
{
  isLeaf = true;
  label = label_to_assign;
  isRoot = true;
  visited = false;
  copy_count = 0;
  value = value_to_set;
  temp_id = 0;
}

DAGNode :: DAGNode(string label_to_assign, vector<DAGNode*> v_operands, void *value_to_set)
{

  int size = v_operands.size();
  for(int i=0;i<size;i++)
    {
      out_list.push_back(v_operands[i]);
    }
  for(int i=0;i<size;i++)
    {
      (v_operands[i]->in_list).push_back(this);
    }


  //if v_operands is empty, then this is really a leaf node
  if(out_list.size()==0)
    isLeaf = true;
  else
    isLeaf = false;
  label = label_to_assign;
  isRoot = true;
  visited = false;
  copy_count = 0;
  value = value_to_set;
  temp_id = 0;
}


/* Destructor of the DAGNode */
DAGNode :: ~DAGNode()
{
  //This is the destructor

}



/**
 * Synopsis: Read the label of the DAGNode
 *
 * Description: This is a publie function that reads the label of the node. Note that it will eliminate the copy prefix from the label of the node before returning. One can query the DAGNode with isCopied method to test if the node was copied..
 Also note that labels of nodes never change, once assigned at the time of creation.
 *
 *
 */



string DAGNode ::getLabel()
{
  if(copy_count==0)
    return this->label;
  else
    {
      string copy_prefix = integerToString(this->copy_count);
      int length = copy_prefix.size();
      return this->label.substr(this->copy_count, this->label.size()- length);
    }
}

/**
 * Synopsis: Get the size of out-list of a DAGNode
 * 
 * Description: Simply returns the size of out-list
 * 
 * Return value: integer >=0
 *
 * See Also: getOutListBeginIterator, getOutListEndIterator
 */
int DAGNode ::getSizeOfOutList()
{
  return this->out_list.size();   
}




/**
 *Synopsis: Get begin iterator on the out-list
 *
 *
 * Description: The purpose of this function is to allow the client to traverse the out-list using iterator. Client may traverse the entire DAG using these iterators.
 *
 * Return value: Valid begin iterator on the out-list if the node has non-empty out-list
 *
 * See Also: getSizeOfOutList, getOutListEndIterator
 **/

list <DAGNode *> :: iterator DAGNode :: getOutListBeginIterator()
{
  list <DAGNode *> ::iterator it;
  if(this->out_list.size()!=0)
    it = this->out_list.begin();
  return it;
}



/**
 *Synopsis: Get end iterator on the out-list
 *
 * Description: The purpose of this function is to allow the client to traverse the out-list using iterator. Client may traverse the entire DAG using these iterators.
 *
 * Return value: Valid end iterator on the out-list if the node has non-empty out-list
 *
 * See Also: getSizeOfOutList, getOutListBeginIterator
 **/

list <DAGNode *> :: iterator DAGNode :: getOutListEndIterator()
{
  list <DAGNode *> ::iterator it;
  if(this->out_list.size()!=0)
    it = this->out_list.end();
  return it;
}



/**
 *Synopsis: Get The Value associated with a node
 *
 * Description: Allows the client to read the custom value associated with the node
 *
 * Return value: A void * to the value field of the DAGNode, NULL if none exists
 *
 *See Also: setValue
 */
 
void * DAGNode :: getValue()
{
  return value;
}


/**
 *Synopsis: Set The Value associated with a node
 *
 * Description: Allows the client to write the custom value associated with the node
 *
 * Return value: A bool indicating success or failure of the operation
 *
 *See Also: setValue
 */

bool DAGNode ::setValue(void *value_to_set)
{
  value = value_to_set;
  return true;
}



/**
 * Synopsis: Set the value of visited flag of a node
 *
 * Description: Use this function in external traversals to set/reset the visited flag of the node
 *
 * Return Value: true if it succeeds. false otherwise
 *
 * See Also: DAGManager::ResetVisitedFlagOfDAGNodes
 */
bool DAGNode :: assignVisitedFlag(bool assign_to)
{
  visited = assign_to;
}



/**
 * Synopsis: Test if the DAG represented by this node is copied one .
 *
 * Description: Tests the copy_count of the node and determines if the DAG represented by this node is copied or not
 *
 * Return Value: A boolean true or false.
 */
bool DAGNode :: isCopied()
{
  return (copy_count == 0)?false : true;
}










	//source lines added by Sourabh Moitra on 3/1/10
        //member functions (i.e. the interface as required by sourabh Moitra)

        string DAGNode :: Get_node_name()
        {
            return getLabel();
        }

        unsigned int DAGNode :: Get_outlist_size()
        {
            return (unsigned int)getSizeOfOutList();
        }

        list<DAGNode*>::iterator DAGNode :: Get_outlist_start()
        {
            return getOutListBeginIterator();
        }

        list<DAGNode*>::iterator DAGNode :: Get_outlist_end()
        {
            return getOutListEndIterator();
        }

        unsigned int DAGNode :: Get_inlist_size()
        {
            return (unsigned int)in_list.size();
        }

        list<DAGNode*>::iterator DAGNode :: Get_inlist_start()
        {
            return in_list.begin();
        }

        list<DAGNode*>::iterator DAGNode :: Get_inlist_end()
        {
            return in_list.end();
        }

        void DAGNode :: Set_visited_flag()
        {
            visited = true;
        }

        bool DAGNode :: Get_visited_flag()
        {
            return visited;
        }

        void DAGNode :: Clear_visited_flag()
        {
            visited = false;
        }

/*
        string DAGNode :: Get_unique_node_identifier()
        {
            
        }

        Type DAGNode :: Get_node_type()
        {
            Type temp_type;

            
            return temp_type;
        }

        Result DAGNode :: Get_node_value()	//the result of the node evaluation......
        {
            
        }

        void DAGNode :: Set_node_value(Result new_result)
        {
            
        }
*/
        //end of modifications by Sourabh Moitra.......
