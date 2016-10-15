/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#include "DDManager.h"

// Sets the global variable "count" which denotes the # of LMEs
// created so far to -1
DDManager :: DDManager()
{
  count=-1;
}

// Returns true if an entry exists in the APMap with index = u_index. Else returns false
bool DDManager::checkIfIndexExists(int u_index)
{

  map<DAGNode*, int>::iterator APMap_it = APMap.begin();
  for(; APMap_it != APMap.end(); APMap_it++)
    {
      if(APMap_it->second == u_index) return true;
    }
  return false;
}


// Returns the index of this node
int DDManager :: getIndex(DDNode *dd)
{
DAGNode *pred = dd->getPredicate();
if(pred == NULL) // terminal node
	return MAX_AP+1;
else
	{
	if(APMap.find(pred)==APMap.end()) // no entry in APMap for the DAGNode* pred : ERROR
		{
		cout<<"\nNo entry for APMap for the predicate node "<<pred<<"in function DDManager::getIndex()\n";
		exit(1);
		}
	return APMap[pred];
	}
}

// set the index of the given predicate node pred as new_index
void DDManager::setIndex(DAGNode *pred, int new_index)
{
if(pred==NULL)
	{
	cout<<"\nDDManager::setIndex called with pred == NULL\n";
	exit(1);
	}
if(new_index>=MAX_AP+1)
	{
	cout<<"\nDDManager::setIndex called with new_index>=MAX_AP+1\n";
	exit(1);
	}
if(checkIfIndexExists(new_index)) // an entry exists in APMap with the same index : ERROR
	{
	cout<<"\nIn DDManager::setIndex, an entry exists in APMap with the given index "<<new_index<<endl;
	exit(1);
	}
if(APMap.find(pred)!=APMap.end()) // an entry already exists for the AP
	{
	cout<<"\nFunction DDManager::setIndex called to change the index of an existing AP in APMap\n";
	exit(1);
	}
else // entry does not exist for AP
	{
	APMap.insert(make_pair(pred, new_index));
	}
}
	


//to create DDNode for terminal values '1' and '0', and return the DDNode
DDNode* DDManager :: createDD(int new_val)  
{
  string key;
  
  if(new_val==0) 
    key = "0"; 
  else if(new_val==1) 
    key = "1"; 
  else {cout<<"\nERROR!createDD(int new_val) called with argument "<<new_val<<endl; exit(1);}

  // Hash key created

  HTStatusValue<DDNode*> result = DD_HT.hashtable_search(key);

  if(result.success()) // Exists in HT
    {
      return result.GetValue();
    }
  else
    {
	    
      DDNode* addr_DDnode = new DDNode(new_val);

      result = DD_HT.hashtable_insert(key, addr_DDnode);
   
      if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function DDManager :: createDD(int new_val)\n";
	  exit(1);
	}
      
      //For inserting into PathCountsOfDDNodes if ConvertProblemIntoDisjunctionOfTailedTriangles is true or
      // ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification is true
      if(ConvertProblemIntoDisjunctionOfTailedTriangles || ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification)
        {
          //addr_DDnode is a terminal. Insert (0, 0)
          PathCountsOfDDNodes.insert(make_pair(addr_DDnode, make_pair(0, 0)));
        }   
      
      
      
      return addr_DDnode;
    }
       
}


//to create DDNode for LME new_predicate, and return the DDNode
DDNode* DDManager :: createDD(DAGNode *new_predicate, map<string, int> &WidthTable)
{
  DDNode* left = createDD(0);
  DDNode *right = createDD(1);

  string key;
  char temp_char1[10], temp_char2[10], temp_char3[10];

  sprintf(temp_char1 , "%x", left);
  sprintf(temp_char2 , "%x", right);
  sprintf(temp_char3 , "%x", new_predicate);

  string opd_addr1(temp_char1);
  string opd_addr2(temp_char2);
  string pred_addr(temp_char3);

  key = pred_addr + opd_addr1 + opd_addr2; // check if new_predicate(1, 0) is there
  // Hash key created

  HTStatusValue<DDNode*> result = DD_HT.hashtable_search(key);

  if(result.success()) // Exists in HT
    {
      return result.GetValue();
    }


 if(new_predicate->Get_node_name() !="bvule" && new_predicate->Get_node_name() !="bvuge" && new_predicate->Get_node_name() !="bvult" && new_predicate->Get_node_name() !="bvugt")
        {               
         new_predicate = recreateEquationInProperForm(new_predicate, WidthTable); // convert to cannonical form        
        }
 
  char temp_char4[10];
	
  sprintf(temp_char4 , "%x", new_predicate);
  string pred_addr_new(temp_char4);

  key = pred_addr_new + opd_addr1 + opd_addr2; // check if cann(new_predicate)(1, 0) is there

  // Hash key created

  result = DD_HT.hashtable_search(key);

  if(result.success()) // Exists in HT
    {
      return result.GetValue();
    }


  // no node exists already. We need to create the node

  //cout<<"\nnode does not exist already. we need to create one\n";


  if(APMap.find(new_predicate) == APMap.end()) // predicate is not there already in APMap. Hence insert it first.
        {

	//cout<<"\nLMC "<<getDAGStr(new_predicate)<<" does not exist already. we need to insert one\n";
      
        if(order_lmes_before_lmis)
            {
            count++;
            
            int InsertLocation;
            
            // if LMI
            if(new_predicate->Get_node_name() =="bvule" || new_predicate->Get_node_name() =="bvuge" || new_predicate->Get_node_name() =="bvult"  || new_predicate->Get_node_name() =="bvugt")
                {                
                InsertLocation = count + LMI_OFFSET;                
                }
            // if LME
            else if(new_predicate->Get_node_name() =="=" || new_predicate->Get_node_name() =="equivalent")
                {
                InsertLocation = count;
                }
            else
                {
                cout<<"\nUnknown predicate type "<<new_predicate->Get_node_name()<<" in function DDManager :: createDD\n";
                exit(1);
                }
            // Insert into APMap
            
            if(InsertLocation>MAX_AP) 
                    {
                    cout<<"\nIn function  DDManager :: createDD, count has exceeded MAX_AP. Increase MAX_AP\n";
                    exit(1);
                    }
            if(checkIfIndexExists(InsertLocation)) // an entry exists in APMap with the same index : ERROR
                    {
                    cout<<"\nIn DDManager::createDD, an entry exists in APMap with the given index "<<InsertLocation<<endl;
                    exit(1);
                    }            
            APMap.insert((pair<DAGNode*,int>(new_predicate, InsertLocation)));
            }//if(order_lmes_before_lmis)
        else
            {
            count++;

            if(count>MAX_AP) 

                    {cout<<"\nIn function  DDManager :: createDD, count has exceeded MAX_AP. Increase MAX_AP\n";
                    exit(1);
                    }

            if(checkIfIndexExists(count)) // an entry exists in APMap with the same index : ERROR
                {
                cout<<"\nIn DDManager::createDD, an entry exists in APMap with the given index "<<count<<endl;
                exit(1);
                }
            
            APMap.insert((pair<DAGNode*,int>(new_predicate,count)));
        }//else of if(order_lmes_before_lmis)
     }//if(APMap.find(new_predicate) == APMap.end())

  //cout<<"\nLME exists already\n"; 
	  
  DDNode* addr_DDnode = new DDNode(new_predicate, left, right); // create the dd node

  result = DD_HT.hashtable_insert(key, addr_DDnode);
   
   if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function DDManager :: createDD(DAGNode *new_predicate)\n";
	  exit(1);
	}
  
  
  //For inserting into PathCountsOfDDNodes if ConvertProblemIntoDisjunctionOfTailedTriangles is true
  // or ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification is true
  if(ConvertProblemIntoDisjunctionOfTailedTriangles || ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification)
        {
          //check if new_predicate is an LME with odd coefficient for any of the variables to be eliminated
          if(predicateIsLMEWithOddCoefForSomeVarToElim(new_predicate)) //yes
            {
              PathCountsOfDDNodes.insert(make_pair(addr_DDnode, make_pair(1, 0)));
            }
          else //no
            {
            PathCountsOfDDNodes.insert(make_pair(addr_DDnode, make_pair(0, 0)));
            }
        }
  

  return addr_DDnode;
}

// Function to create DDNode (new_predicate, right, left) and return the DDNode.
// Assumption : new_predicate is already there in the APMap in cannonical form.
DDNode* DDManager :: createDD(DAGNode *new_predicate, DDNode* left, DDNode *right )
{
  string key;
  char temp_char1[10], temp_char2[10], temp_char3[10];

  sprintf(temp_char1 , "%x", left);
  sprintf(temp_char2 , "%x", right);
  sprintf(temp_char3 , "%x", new_predicate);

  string opd_addr1(temp_char1);
  string opd_addr2(temp_char2);
  string pred_addr(temp_char3);

  key = pred_addr + opd_addr1 + opd_addr2;

  // Hash key created

  HTStatusValue<DDNode*> result = DD_HT.hashtable_search(key);

  if(result.success()) // Exists in HT
    {
      return result.GetValue();
    }
  else // new node
    {
      
     if(APMap.find(new_predicate) == APMap.end()) // AP is not there already in the APMap
        {
  
	  cout<<"\ncreateDD(DAGNode *new_predicate, DDNode* left, DDNode *right ) called to create DDs with new predicates. If you want to create DDs with new predicates, use createDD(DAGNode *new_predicate, map<string, int> WidthTable)\n";
	  exit(1);

        }

       DDNode* addr_DDnode = new DDNode(new_predicate, left, right);

       result = DD_HT.hashtable_insert(key, addr_DDnode);
   
       if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function DDManager :: createDD(DAGNode *new_predicate, DDNode* left, DDNode *right )\n";
	  exit(1);
	}
       
       
      //For inserting into PathCountsOfDDNodes if ConvertProblemIntoDisjunctionOfTailedTriangles is true
       // or ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification is true
     if(ConvertProblemIntoDisjunctionOfTailedTriangles || ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification)
        {
         //Get the pairs from left and right
         map<DDNode*, pair<int, int> >::iterator left_it = PathCountsOfDDNodes.find(left);
         
         if(left_it == PathCountsOfDDNodes.end())//No entry for left in PathCountsOfDDNodes
            {
            cout<<"\nNo entry for "<<left<<" in PathCountsOfDDNodes in function DDManager :: createDD(DAGNode *new_predicate, DDNode* left, DDNode *right )\n";
            exit(1);
            }
         
         map<DDNode*, pair<int, int> >::iterator right_it = PathCountsOfDDNodes.find(right);
         
         if(right_it == PathCountsOfDDNodes.end())//No entry for right in PathCountsOfDDNodes
            {
            cout<<"\nNo entry for "<<right<<" in PathCountsOfDDNodes in function DDManager :: createDD(DAGNode *new_predicate, DDNode* left, DDNode *right )\n";
            exit(1);
            }
         
         int MaxRightPair = findMaximum((right_it->second).first, (right_it->second).second);
         int MaxLeftPair = findMaximum((left_it->second).first, (left_it->second).second);
         
          //check if new_predicate is an LME with odd coefficient for any of the variables to be eliminated
          if(!predicateIsLMEWithOddCoefForSomeVarToElim(new_predicate)) //no
            {
              PathCountsOfDDNodes.insert(make_pair(addr_DDnode, make_pair(MaxRightPair, MaxLeftPair)));
            }
          else //yes
            {
             //Let's first see if right is terminal 0             
             if(right->getPredicate() == NULL)//right is a terminal
                {
                 if(right->getVal() == 0)//right is terminal 0
                    {
                     PathCountsOfDDNodes.insert(make_pair(addr_DDnode, make_pair(MaxRightPair, MaxLeftPair)));
                    }
                 else//right is terminal 1
                    {
                     PathCountsOfDDNodes.insert(make_pair(addr_DDnode, make_pair(MaxRightPair + 1, MaxLeftPair)));
                    }
                }//right is a terminal
             else//right is not a terminal
                {
                PathCountsOfDDNodes.insert(make_pair(addr_DDnode, make_pair(MaxRightPair + 1, MaxLeftPair)));
                }//right is not a terminal
            }//if new_predicate is an LME with odd coefficient for any of the variables to be eliminated
        }//if(ConvertProblemIntoDisjunctionOfTailedTriangles || ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification)
       
       
       
      return addr_DDnode;
    }
}

// return the result v1->getVal() op v2->getVal()
int DDManager::evaluateOp(string op, DDNode *v1, DDNode *v2)
{
  if(v1->getPredicate()!= NULL && v2->getPredicate()!= NULL) return -1;

  int value1;
  int value2;

  if(v1->getPredicate() == NULL) value1 = v1->getVal(); else value1 = -1;
  if(v2->getPredicate() == NULL) value2 = v2->getVal(); else value2 = -1;

  if(op=="or")
    {
      if(value1 == 1 || value2 == 1) return 1;
      else if(value1 == 0 && value2 == 0) return 0;
      else return -1;
    }
  if(op=="and")
    {
      if(value1 == 0 || value2 == 0) return 0;
      else if(value1 == 1 && value2 == 1) return 1;
      else return -1;
    }
  if(op=="xor")
    {
      if(value1 == -1 || value2 == -1) return -1;
      if(value1 == value2) return 0;
      else return 1;
    }
}

// return minimum of i and j
int DDManager::findMin(int i, int j)
{
  if(i<=j) return i;
  else return j;
}

// Returns the LME corresponding to the given index.
// Exits from the program if no LME exists in the APMap for the given index.
DAGNode* DDManager::findAPFromIndex(int u_index)
{

  map<DAGNode*, int>::iterator APMap_it = APMap.begin();
  for(; APMap_it != APMap.end(); APMap_it++)
    {
      if(APMap_it->second == u_index) return APMap_it->first;
    }
  cout<<"\nError in function  DDManager::findAPFromIndex. No entry in APMap for index "<<u_index<<endl;
  exit(1);
}

// Implements Bryant's reduce function.
// Given DDNode* v, returns reduced DDNode*.
DDNode* DDManager::Reduce(DDNode *v)
{
  
  DDNode *u;
  DDNode* subgraph[MAX_NODES];
  list<DDNode*> vlist[MAX_AP+2];

  SetOfDDNodes.clear();
  Traverse(v, vlist);

  map< pair<int, int>, list<DDNode*> > Q;
  map< pair<int, int>, list<DDNode*> >::iterator Q_it;
  list<DDNode*> DDNodesWithSameKey;
  pair<int, int> old_key, key;
  int nextid = 0;


  for(int i=MAX_AP+1; i>=0; i--)
    {


      Q.clear();
      
      list<DDNode*> CurrentList = vlist[i];

      if(CurrentList.empty()) continue;

      for(list<DDNode*>::iterator it=CurrentList.begin(); it!=CurrentList.end(); it++)
        {
          u=*it;
          if(u->getPredicate() == NULL) // Terminal nodes
            {
              insertIntoQ(Q, u->getVal(), u->getVal(), u);
            }
          else if(u->getLeft()->getID() == u->getRight()->getID())
            {
              u->setID(u->getLeft()->getID()); // u is a redundant node
            }
          else
            {
             insertIntoQ(Q, u->getLeft()->getID(), u->getRight()->getID(), u);
            }
        }

      // Q is ready here. Q is sorted by construction


      old_key=make_pair(-1, -1);

      for(Q_it = Q.begin(); Q_it != Q.end(); Q_it++) // Take elements one by one
        {

          DDNodesWithSameKey=Q_it->second;

          for(list<DDNode*>::iterator samekey_it = DDNodesWithSameKey.begin(); samekey_it != DDNodesWithSameKey.end(); samekey_it++)
            {

              u = (*samekey_it);

	      // Element with minium k obtained

              key=Q_it->first;

              if (key.first==old_key.first && key.second==old_key.second)
                {
		  
                  u->setID(nextid); //{matches existing vertex}
                  
                }

              else //{unique vertex}
                {
                  
                  nextid = nextid + 1;
		  if(nextid>=MAX_NODES)
		    {
		      cout<<"\nReduce has a limit of "<<MAX_NODES<<" no: of nodes. This limit exceeded\n";
		      exit(1);
		    }

                  u->setID(nextid);
                  subgraph[nextid] = u;

                  if(u->getPredicate() != NULL)
                    {
		      u->setLeft(subgraph[(u->getLeft())->getID()]);
		      u->setRight(subgraph[(u->getRight())->getID()]);
     
                    }

                  else // Terminal nodes
                    {
                      u->setLeft(NULL);
                      u->setRight(NULL);
                    }

                  old_key.first = key.first;
                  old_key.second = key.second;
                }
            }

        }
    }

  return(subgraph[v->getID()]);

}

// Function to insert into Q. Called by Reduce()
void DDManager::insertIntoQ(map< pair<int, int>, list<DDNode*> > &Q, int key1, int key2, DDNode* u)
{

  map< pair<int, int>, list<DDNode*> >::iterator Q_it;

  Q_it = Q.find(make_pair(key1, key2));

  if(Q_it != Q.end())
    {
      // Another node with the same key and same index already exists

      (Q_it->second).push_back(u);
    }
  else
    {
      list<DDNode*> new_list;
      new_list.push_back(u);
      Q.insert(make_pair(make_pair(key1, key2), new_list));
    }
}

//Implementation of Bryant's Traverse function. Called by Reduce()
void DDManager::Traverse(DDNode *v, list<DDNode*> vlist[])
{
  //procedure Traverse(v:vertex);

  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      // Already visited
      return;
    }
 
  if(v->getPredicate()!= NULL) // {v nonterminal}
    {
      Traverse(v->getLeft(), vlist);
      Traverse(v->getRight(), vlist);
    }

  
  vlist[getIndex(v)].push_back(v);

  SetOfDDNodes.insert(v);
  
}

// Gets the # of nodes in the DD rooted at "v" in "NoOfNodesInDD"
void DDManager::getNoOfNodesInDD(DDNode *v, int &NoOfNodesInDD)
{

  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      // Already visited
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {
      if(v->getPredicate()!=NULL) getNoOfNodesInDD(v->getLeft(), NoOfNodesInDD);
      if(v->getPredicate()!=NULL) getNoOfNodesInDD(v->getRight(), NoOfNodesInDD);

      NoOfNodesInDD++; // New DDNode encountered
      SetOfDDNodes.insert(v);
    }
}


// return the DDNode for not(v1)
DDNode* DDManager::createDDNot(DDNode* v1)
{
  DDNode* result = apply("xor", v1, createDD(1));
  return result;
}


// Function to create a DD from a DAG with elimination of inconsistent paths
DDNode* DDManager::createDDFromDAGWithImplicationCheck(DAGNode *dag, map<string, int> WidthTable)
{
  //cout<<"\ncreateDDFromDAGWithImplicationCheck called with dag = "<<getDAGStr(dag)<<endl; 
  
  string dag_str = dag->Get_node_name();

  if(dag_str == "=" || dag_str == "equivalent")
    {
      //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
      return createDD(dag, WidthTable);
    }

    else if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;
      DAGNode* inverseDAG = createDAG("=", left, right, LOGICAL_SYMBOL, 1);
      DDNode* inverseDD = createDD(inverseDAG, WidthTable);
      //return createDDNot(inverseDD);
      //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
      return applyWithImplicationCheck("xor", inverseDD, createDD(1), WidthTable);
    }

  else if(dag_str == "and")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;


      DDNode* then_dd = createDDFromDAGWithImplicationCheck(left, WidthTable);
      DDNode* els_dd = createDDFromDAGWithImplicationCheck(right, WidthTable);

      //DDNode* result_dd = apply("and", then_dd, els_dd);
      DDNode* result_dd = applyWithImplicationCheck("and", then_dd, els_dd, WidthTable);

      unsigned int outlist_size = dag->Get_outlist_size();
      if(outlist_size==2) 
		{
                //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
		return result_dd;
		}
      else
	{
        iout++;
	for(int i=2;i<outlist_size;i++)
		{
                DAGNode *child = *iout; 
                DDNode* child_dd = createDDFromDAGWithImplicationCheck(child, WidthTable);
		//result_dd = apply("and", result_dd, child_dd);
                result_dd = applyWithImplicationCheck("and", result_dd, child_dd, WidthTable);
		iout++;
		}
	//cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
	return result_dd;
	} 
     	

    }
  else if(dag_str == "or")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;


      DDNode* then_dd = createDDFromDAGWithImplicationCheck(left, WidthTable);
      DDNode* els_dd = createDDFromDAGWithImplicationCheck(right, WidthTable);

      //return apply("or", then_dd, els_dd);
      //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
      return applyWithImplicationCheck("or", then_dd, els_dd, WidthTable);
    }
  else if(dag_str == "if_then_else")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *codn, *left, *right;
      codn = *iout;
      iout++;
      left =*iout;
      iout++;
      right =*iout;

      
      DDNode* then_dd = createDDFromDAGWithImplicationCheck(left, WidthTable);
      DDNode* els_dd = createDDFromDAGWithImplicationCheck(right, WidthTable);
      DDNode* codn_dd = createDDFromDAGWithImplicationCheck(codn, WidthTable);

      //return createITE(codn_dd, then_dd, els_dd);
      //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;	
      return createITEWithImplicationCheck(codn_dd, then_dd, els_dd, WidthTable);
    }
  else if(dag_str == "1" || dag_str == "true")
    {
      //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
      return createDD(1);
    }
  else if(dag_str == "0" || dag_str == "false")
    {
      //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
      return createDD(0);
    }

  // Newly added on 8/4/2010 to account for "not"
  else if(dag_str == "not")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *child;
      child = *iout;
      DDNode* inverseDD = createDD(child, WidthTable);
      //return createDDNot(inverseDD);
      //cout<<"\nReturned from createDDFromDAGWithImplicationCheck of dag = "<<getDAGStr(dag)<<endl;
      return applyWithImplicationCheck("xor", inverseDD, createDD(1), WidthTable);
    }
  else
    {
      cout<<"\ncreateDDFromDAGWithImplicationCheck called with dag name "<<dag_str<<". Exiting...\n"; exit(1);
    }
}


// Simplify ite(condn, then, els) if possible, and return the simplified DDNode*
DDNode* DDManager::evaluateITE(DDNode *condn, DDNode *then, DDNode *els)
{
  if(condn->getPredicate()!= NULL && then->getPredicate()!= NULL && els->getPredicate()!= NULL)
    {
      return NULL;
    }

  if(then->getPredicate()!= NULL && els->getPredicate()!= NULL)
    {
      if(then == els)
        return then;

      if(condn->getPredicate() == NULL)
        {
          if(condn->getVal()==1)
            return then;
          else
            return els;
        }
      return NULL;
    }


  int c, t, e;

  if(condn->getPredicate()==NULL) c=condn->getVal(); else c=-1;
  if(then->getPredicate()==NULL) t=then->getVal(); else t=-1;
  if(els->getPredicate()==NULL) e=els->getVal(); else e=-1;

  if(t==1 && e==0)
    return condn;

  if(t==1 && e==1)
    return createDD(1);

  if(t==0 && e==0)
    return createDD(0);

  if(c==1 && t==1)
    return createDD(1);

  if(c==0 && e==1)
    return createDD(1);

  if(c==1 && t==0)
    return createDD(0);

  if(c==0 && e==0)
    return createDD(0);

  return NULL;
}

// Performs \exists VarSetToElim. root  with LMDD based approach
// Implemets QuaLMoDE, All Path QE, Version of QuaLMoDE which tries to eliminate all variables to eliminate in 
// a single shot,  QBV_Solve, and
// QE from LMDDs involving LMIs
// depending on the command line options
DAGNode* DDManager::Quantifier_Eliminator(DAGNode *root, set<string> VarSetToElim, map<string, int> &WidthTable)
{

  time_t lmdd_start_time, lmdd_end_time;
  time(&lmdd_start_time);
  time_out_start = lmdd_start_time;

  // Creating VarsToElim

  list<string> VarsToElim;
  for(set<string>::iterator it=VarSetToElim.begin(); it != VarSetToElim.end(); it++)
    {
      string name = *it;
      VarsToElim.push_back(name);
    }


  copySetNew(CopyOfVarSetToElim, VarSetToElim);//Copy VarSetToElim
  //into the global variable CopyOfVarSetToElim

  //cout<<"\nVarsToElim\n";

  //for(list<string>::iterator vars_it = VarsToElim.begin();vars_it !=  VarsToElim.end(); vars_it++)
   // {
   //   cout<<*vars_it<<endl;
   // }


  DAGNode* LinearDag = root;


  checkForPropSkelton(LinearDag);

  //cout<<"\n\nDag's structure confirmed to be propositional\n\n";

  bool order_prefixing=true;

  if(order_prefixing)
    {
      list<DAGNode*> APList;

      //cout<<"\nGetting the APs in the DAG\n";

      getAPsForDAGInPropSkelton(LinearDag, APList);

      //cout<<"\nAPList\n-------\n";
      //showAPList(APList);

      
      if(order_based_on_vars_to_elim)
	{
	modifyAPListBasedOnVarsToElim(APList, VarSetToElim);
	//cout<<"\nAPList\n-------\n";
	//showAPList(APList);
	}
  
      //cout<<"\nInitializing the APMap\n";
      initializeAPMap(APList, WidthTable);
      //cout<<"\nAPMap initialized\n";
    }


    
  
  time_t initial_dd_creation_time, t2;
  time(&initial_dd_creation_time);

  

  DDNode* dd;

  if(simplification_mode==2 && !LMI_handling_enabled)
	dd = createDDFromDAGWithImplicationCheck(LinearDag, WidthTable);
  else if(simplification_mode==1 && !LMI_handling_enabled)
	{
	  //cout<<"\nCalling createDDFromDAGWithoutImplicationCheck\n";
	dd = createDDFromDAGWithoutImplicationCheck(LinearDag, WidthTable);
	//cout<<"\nSize of dd = "<<getSizeOfDD(dd)<<endl;
        //cout<<"\ndd = "<<getDAGStr(createDAGFromDD(dd))<<endl;
	dd = eliminateInconsistentPaths(dd, WidthTable);
	//cout<<"\nSize of dd after elimination of inconsistent paths = "<<getSizeOfDD(dd)<<endl;
        //cout<<"\ndd = "<<getDAGStr(createDAGFromDD(dd))<<endl;
	}
    else if(simplification_mode==0)
	{
	  //cout<<"\nCalling createDDFromDAGWithoutImplicationCheck\n";
	dd = createDDFromDAGWithoutImplicationCheck(LinearDag, WidthTable);
	//cout<<"\nSize of dd = "<<getSizeOfDD(dd)<<endl;
        //cout<<"\ndd = "<<getDAGStr(createDAGFromDD(dd))<<endl;
	}
  else
	{
	cout<<"\nInvalid simplification mode\n";
	exit(1);
	}

  time(&t2);

  cout<<"\nInitial DD created in "<< (t2 - initial_dd_creation_time) <<" seconds"<<endl;

  if(false) {printAPMapOnScreen(); }
  

  LMDD_Creation_Time = t2 - initial_dd_creation_time;

  // Find the no: of nodes in the input DD

  int NoOfDDNodes = 0;
  SetOfDDNodes.clear();
  getNoOfNodesInDD(dd, NoOfDDNodes);

  cout<<"\nNumber of nodes in the input DD = "<<NoOfDDNodes<<endl;

  Max_Inter_LMDD_Size = NoOfDDNodes;
  
  SizesOfEliminatedLMDDs.push_back(NoOfDDNodes);
  
   SizeOfInputDDInDDBasedQE = NoOfDDNodes;
  
  // For data-collection for printing 
  if(!ConvertProblemIntoDisjunctionOfTailedTriangles && !ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification)
    {
    NumberOfTailedTrianglesToQE = 1;
    TotalSizeOfTriangles = NoOfDDNodes;
    TotalVarsToElimFromTailedTriangles = VarSetToElim.size();
    }
  // Data-collection for printing 

  //showAPMap(); 

  //showDD(dd); // show the dd as a dag
  
  //cout<<"\n\nThe DD = "<<getDAGStr(createDAGFromDD(dd))<<endl;
  
 
  // Cross-checking if the DD is created properly

  bool checking_dd_creation_on = false;

  if(checking_dd_creation_on)
    {

      DAGNode* dag_back = createDAGFromDD(dd);
 
      DAGNode *cross_check = createDAG("is_not_equal", LinearDag, dag_back, LOGICAL_SYMBOL, 1);
      
      if(!LMI_handling_enabled)
        {
            if(ThUnsatSingleFormula(cross_check, WidthTable))
            {
            cout<<"\ndag = dag_back\n"; // correct
            }
          else
            {
            cout<<"\ndag <> dag_back\n";
            exit(1);
            }
      }
      else 
      {
        map<string, string> Temp_Model;
      
        bool ResultIsFalse = getModelGeneral_WithSupportForLMIs(cross_check, Temp_Model, WidthTable);
              
         if(ResultIsFalse)//unsat
              {
                 cout<<"\ndag = dag_back\n"; // correct
              }
         else
              {
                cout<<"\ndag <> dag_back\n";//incorrect
                exit(1);  
              }
        } //if(LMI_handling_enabled)     
      
     }//if(checking_dd_creation_on)

  //int userchar = getchar();


  time_t var_elim_start_time;
  time(&var_elim_start_time);

  DDNode* dd_eliminated;
  
  if(!LMI_handling_enabled)
  {

      if(strcmp(Elimination_Type, "-white") == 0)
        dd_eliminated = MultiVarQElimWithStrategy(VarsToElim, dd, WidthTable);
      else if(strcmp(Elimination_Type, "-black") == 0)
        dd_eliminated = MultiVarQElimWithNoDPNoStrategy(VarsToElim, dd, WidthTable);
      else if(strcmp(Elimination_Type, "-gray") == 0)
        dd_eliminated = All_Path_QElim_With_Simplifications(VarsToElim, dd, WidthTable);
      else if(strcmp(Elimination_Type, "-white_blasting") == 0)
        {
         dd_eliminated = QuaLMoDEWithBlasting(VarsToElim, dd, WidthTable);
        exit(1);
        }
      else if(strcmp(Elimination_Type, "-black_blasting") == 0)
        {
        DAGNode* resultOfQElim = NULL;
        MultiVarQElimWithNoDPNoStrategyWithBlasting(VarsToElim, dd, WidthTable, resultOfQElim);
        if(resultOfQElim == NULL)
                  {
                  Value *v = new Value();
                  Type *t = new Type();
                  t->data_type = LOGICAL_CONSTANT;
                  t->width = 1;
                  v->set_node_type(*t);
                  resultOfQElim = dm.createNode("true",(void*)v);
                  }
        //cout<<"\nresultOfQElim = "<<getDAGStr(resultOfQElim)<<endl;
        //printOutputOfQEliminatorInSMTLibFormatInGivenFile(resultOfQElim, WidthTable, "output_qelim.smt");
        return resultOfQElim;
        //exit(1);
        }
      else
        {
          cout<<"\nUnknown Elimination_Type "<<Elimination_Type<<endl;
          exit(1);
        }
  }//if(!LMI_handling_enabled)
  else
  {
         
      if(LMEBasedSimplificationAsPreprocessor)
        {
         struct timeval start_ms, finish_ms;
         unsigned long long int duration_ms;    
         gettimeofday (&start_ms, NULL);     
          
        //cout<<"\n\nThe DD = "<<getDAGStr(createDAGFromDD(dd))<<endl;  
        
        DDSizeBeforeLMEBasedSimplificationAsPreprocessor = NoOfDDNodes;
          
        dd = callPerformLMEBasedSimplificationAsPreprocessor(VarSetToElim, dd, WidthTable);
        
        //cout<<"\n\nThe DD after LMEBasedSimplificationAsPreprocessor = "<<getDAGStr(createDAGFromDD(dd))<<endl;  
        
        DDSizeAfterLMEBasedSimplificationAsPreprocessor = getSizeOfDD(dd);
        
        if(DDSizeAfterLMEBasedSimplificationAsPreprocessor > Max_Inter_LMDD_Size)
            {
            Max_Inter_LMDD_Size = DDSizeAfterLMEBasedSimplificationAsPreprocessor;
            }
        
        gettimeofday (&finish_ms, NULL);
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;  
        
        TimeInLMEBasedSimplificationAsPreprocessor = duration_ms;
        
        cout<<"\nLMEBasedSimplificationAsPreprocessor finished in "<<TimeInLMEBasedSimplificationAsPreprocessor<<" milliseconds\n";        
        }
      
      struct timeval qe_lmdd_start_ms, qe_lmdd_finish_ms;
      unsigned long long int qe_lmdd_duration_ms;    
      gettimeofday (&qe_lmdd_start_ms, NULL);    
      
      if(use_monniaux_type_simplification_in_dd || use_path_based_strategy_in_lmdd_monniaux)//QE using LMDDs with Monniaux type QE
      {
          
        if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            cout<<"\n\nThe DD = "<<getDAGStr(createDAGFromDD(dd))<<endl;
            }
          
      original_dag_in_path_based_strategy_in_lmdd_monniaux = LinearDag; 
      
      PartialResult_In_LMDDWithMonniauxTypeSimplifications = createLogicalConstant("false");
      
      
      createVariableMap(WidthTable); //This is a function used in Monniaux style QE
      // This is required as QuaLMoDE_WithLMISupport_WithMonniauxTypeSimplifications
      // calls Project which may call callMonniaux_Quantifier_Eliminator_WithSupportForLMIs
      
      dd_eliminated = QuaLMoDE_WithLMISupport_WithMonniauxTypeSimplifications(VarsToElim, dd, WidthTable);   
      
      //cout<<"\n\nThe dd_eliminated = "<<getDAGStr(createDAGFromDD(dd_eliminated))<<endl;      
     
      }
      else if(UseMonniauxEnabledAllVariableStrategy) //All variable elimination with Monniaux type simplifications
      {
           if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                {
                cout<<"\n\nThe DD = "<<getDAGStr(createDAGFromDD(dd))<<endl;
                }

          original_dag_in_path_based_strategy_in_lmdd_monniaux = LinearDag; 

          PartialResult_In_LMDDWithMonniauxTypeSimplifications = createLogicalConstant("false");

          createVariableMap(WidthTable); //This is a function used in Monniaux style QE
         
          dd_eliminated = call_QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(VarSetToElim, dd, WidthTable);   

           if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                {               
                cout<<"\n\nThe dd_eliminated = "<<getDAGStr(createDAGFromDD(dd_eliminated))<<endl;      
                }
      }
      else if(ConvertProblemIntoDisjunctionOfTailedTriangles || ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification) //conversion of problem into disjunction of tailed traingles
      {
          
          if(true)
            {
            printAPMap();  
            }
          
           if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\n\nThe DD = "<<getDAGStr(createDAGFromDD(dd))<<endl;
                }
           
          copySetNew(CopyOfVarSetToElim, VarSetToElim);

          original_dag_in_path_based_strategy_in_lmdd_monniaux = LinearDag; 

          PartialResult_In_LMDDWithMonniauxTypeSimplifications = createLogicalConstant("false");

          createVariableMap(WidthTable); //This is a function used in Monniaux style QE
          
          if(ConvertProblemIntoDisjunctionOfTailedTriangles)
            {
            if(ConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls)
            // The traversal for finding tailed triangles is done using SMT solver calls    
                {
                dd_eliminated = conversionToDisjunctionOfTailedTrianglesFollowedByQEUsingSMTSolverCalls(VarSetToElim, dd, WidthTable);  
                }
            else
            // The traversal for finding tailed triangles is done following the edge with max. number of LMEs    
                {
                dd_eliminated = conversionToDisjunctionOfTailedTrianglesFollowedByQE(VarSetToElim, dd, WidthTable);   
                }
            }
          else if(ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification)
            {
             dd_eliminated = call_conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(VarSetToElim, dd, WidthTable);    
            }
          else
            {
              cout<<"\nError in DDManager::Quantifier_Eliminator. Unknown option!\n";
              exit(1);
            }

           if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {               
                cout<<"\n\nThe dd_eliminated = "<<getDAGStr(createDAGFromDD(dd_eliminated))<<endl;      
                }
      }
      else //Usual LMDD type QE
      {
          
      //cout<<"\n\nThe DD = "<<getDAGStr(createDAGFromDD(dd))<<endl;    
          
          if(strcmp(Elimination_Type, "-black") == 0) // if condn added by Ajith on 19 july 2015
          {
              copySetNew(CopyOfVarSetToElim, VarSetToElim);
              dd_eliminated = call_All_Path_QElim_WithLMISupport(VarSetToElim, dd, WidthTable);    
          }
          else
          {
              dd_eliminated = QuaLMoDE_WithLMISupport(VarsToElim, dd, WidthTable);     
          }
      
      }
      
    gettimeofday (&qe_lmdd_finish_ms, NULL);
    qe_lmdd_duration_ms = qe_lmdd_finish_ms.tv_sec * 1000 + qe_lmdd_finish_ms.tv_usec / 1000;
    qe_lmdd_duration_ms -= qe_lmdd_start_ms.tv_sec * 1000 + qe_lmdd_start_ms.tv_usec / 1000;    
    QETimeUsingLMDDs = qe_lmdd_duration_ms;
      
  }

  
  time(&t2);

    if(!timed_out)
	{
	
	cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (t2 - var_elim_start_time)<<" SECONDS "<<endl<<endl;
	}
    else
	{
	cout<<"\nQuantifier elimination timed out in "<<(t2 - time_out_start)<<" SECONDS "<<endl<<endl;
	}

  NoOfDDNodes = 0;
  SetOfDDNodes.clear();
  getNoOfNodesInDD(dd_eliminated, NoOfDDNodes);
  cout<<"\nNumber of nodes in the output DD = "<<NoOfDDNodes<<endl;

  if(NoOfDDNodes > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = NoOfDDNodes;
	}


  DAGNode* dag_eliminated = createDAGFromDD(dd_eliminated);
  
  cout<<"\nDAG created from DD\n";

  //cout<<"\nResult dag = "<<getDAGStr(dag_eliminated)<<endl;
  
  if(use_monniaux_type_simplification_in_dd || use_path_based_strategy_in_lmdd_monniaux)//QE using LMDDs with Monniaux type QE
      // or QE on paths using Monniaux and LMDD type simplifications
    {
      dag_eliminated = createDAG("or", dag_eliminated, PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
      
      //cout<<"\n\ndag_eliminated = "<<getDAGStr(dag_eliminated)<<endl;      
    }
  else if(UseMonniauxEnabledAllVariableStrategy)//All variable elimination with Monniaux type simplifications
    {
      dag_eliminated = createDAG("or", dag_eliminated, PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
      
      //cout<<"\n\ndag_eliminated = "<<getDAGStr(dag_eliminated)<<endl;      
    }
  else if(ConvertProblemIntoDisjunctionOfTailedTriangles || ConvertProblemIntoDisjunctionOfTailedTrianglesWithAggressiveLMESimplification) //conversion of problem into disjunction of tailed traingles
    {
      dag_eliminated = createDAG("or", dag_eliminated, PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
      
      if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
         cout<<"\n\ndag_eliminated = "<<getDAGStr(dag_eliminated)<<endl;    
        }
    }
  

  time(&lmdd_end_time);

  QElim_Time = lmdd_end_time - lmdd_start_time;

  return dag_eliminated;

}

// Implements Bryant's apply function for binary operators on LMDDs
DDNode* DDManager::apply(string op, DDNode* v1, DDNode* v2)
{
  DDNode* result;

  bool donot_traverse_down_for_easy_cases = true;
  
  if(donot_traverse_down_for_easy_cases)
  {
      if(op == "or")
      {
          if(v1->getPredicate() == NULL)//v1 is a terminal
          {
              if(v1->getVal() == 0)// v1 is 0
              {
                  return v2;
              }
          }//if(v1->getPredicate() == NULL)
         if(v2->getPredicate() == NULL)
          {
              if(v2->getVal() == 0)
              {
                  return v1;
              }
          }// if(v2->getPredicate() == NULL)  
      }// if(op == "or")
  }//if(donot_traverse_down_for_easy_cases)
  
  
  result = applyStep(op, v1, v2);
  DDNode* reducedResult = Reduce(result);

  bool show_effect_of_reduce=false;
  if(show_effect_of_reduce)
	{
        int size_before_reduce = getSizeOfDD(result);
        int size_after_reduce = getSizeOfDD(reducedResult);
        if(size_before_reduce!=size_after_reduce)
		{
		cout<<"\nreduce effective\n";
		cout<<"\nBefore reduce = "<<size_before_reduce<<", after reduce = "<<size_after_reduce<<endl;
                effect_of_reduce = effect_of_reduce + size_before_reduce - size_after_reduce;
		}
        
	}


  // for outputting
  bool outputting_needed=false;
  if(outputting_needed)
	{
	printBlowupInApply(op, getSizeOfDD(v1),getSizeOfDD(v2),getSizeOfDD(reducedResult)); 
	}
  // for outputting ends here

  return reducedResult;
}

// The internal function called by apply function
DDNode* DDManager::applyStep(string op, DDNode* v1, DDNode* v2)
{

  DDNode* u;

  // Creating the hash key

  string key=op;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", v1);
  temp_key = temp_char;
  key += temp_key;
  sprintf(temp_char , "%x", v2);
  temp_key = temp_char;
  key += temp_key;

  // Hash key created

  HTStatusValue<DDNode*> result = TableForApply.hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }

  // TableForApply MISS

  int u_value, u_index;
  DAGNode *u_AP;

  u_value = evaluateOp(op, v1, v2); // should be evaluateOp
  if(u_value != -1)
    {
      //{create terminal vertex}

      u = createDD(u_value);
      result = TableForApply.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyStep\n";
	    exit(1);
	  }

      return u;
    }

  else //{create nonterminal and evaluate further down}
    {

      int v1_index, v2_index;
      v1_index = getIndex(v1);
      v2_index = getIndex(v2);
      DDNode *v1_low, *v1_high, *v2_low, *v2_high, *v_low1, *v_high1, *v_low2, *v_high2, *u_low, *u_high;
      v1_low = v1->getLeft();
      v1_high = v1->getRight();
      v2_low = v2->getLeft();
      v2_high = v2->getRight();

      u_index = findMin(v1_index, v2_index);
      u_AP = findAPFromIndex(u_index);

      if(v1_index == u_index)
        {
          v_low1 = v1_low;
          v_high1 = v1_high;
        }
      else
        {
          v_low1 = v1;
          v_high1 = v1;
        }

      if(v2_index == u_index)
        {
          v_low2 = v2_low;
          v_high2 = v2_high;
        }
      else
        {
          v_low2 = v2;
          v_high2 = v2;
        }
      u_low = applyStep(op, v_low1, v_low2);
      u_high = applyStep(op, v_high1, v_high2);

      u = createDD(u_AP, u_low, u_high);


      result = TableForApply.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyStep\n";
	    exit(1);
	  }

      return u;
    }
}



// Functions for direct ITE creation ADDED ON 2/1/2010

// Implements Bryant's apply function for  ite operator on LMDDs
DDNode* DDManager::createITE(DDNode* condn, DDNode* then, DDNode* els)
{
  DDNode* result;
  result = applyITE(condn, then, els);
  DDNode* reducedResult = Reduce(result);

  bool show_effect_of_reduce=false;
  if(show_effect_of_reduce)
	{
        int size_before_reduce = getSizeOfDD(result);
        int size_after_reduce = getSizeOfDD(reducedResult);
        if(size_before_reduce!=size_after_reduce)
		{
		cout<<"\nreduce effective\n";
		cout<<"\nBefore reduce = "<<size_before_reduce<<", after reduce = "<<size_after_reduce<<endl;
                effect_of_reduce = effect_of_reduce + size_before_reduce-size_after_reduce;
		}
	}

  // for outputting
  bool outputting_needed=false;
  if(outputting_needed)
	{
	printBlowupInApply("ite", getSizeOfDD(condn),getSizeOfDD(then),getSizeOfDD(els),getSizeOfDD(reducedResult)); 
	}
  // for outputting ends here

  return reducedResult;
}

// The internal function called by createITE function
DDNode* DDManager::applyITE(DDNode* condn, DDNode* then, DDNode* els)
{
  DDNode* u;

  // Creating the hash key

  // Include context also in hash key

  string key="";
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", condn);
  temp_key = temp_char;
  key += temp_key;

  sprintf(temp_char , "%x", then);
  temp_key = temp_char;
  key += temp_key;

  sprintf(temp_char , "%x", els);
  temp_key = temp_char;
  key += temp_key;


  // Hash key created

  HTStatusValue<DDNode*> result = TableForITE.hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }

  
  // TableForITE MISS

  u = evaluateITE(condn, then, els);

  if(u!=NULL)
    {
      return u;
    }

  int u_value, u_index;
  DAGNode *u_AP;

  int condn_index, then_index, else_index;
  condn_index = getIndex(condn);
  then_index = getIndex(then);
  else_index = getIndex(els);

  DDNode *condn_low, *condn_high, *then_low, *then_high, *else_low, *else_high;
  DDNode *condn_next_low, *condn_next_high, *then_next_low, *then_next_high,  *else_next_low, *else_next_high;
  DDNode *u_low, *u_high;

  condn_low = condn->getLeft();
  condn_high = condn->getRight();
  then_low = then->getLeft();
  then_high = then->getRight();
  else_low = els->getLeft();
  else_high = els->getRight();

  u_index = findMin(findMin(condn_index, then_index), else_index);
  u_AP = findAPFromIndex(u_index);

  if(condn_index == u_index)
    {
      condn_next_low = condn_low;
      condn_next_high = condn_high;
    }
  else
    {
      condn_next_low = condn;
      condn_next_high = condn;
    }

  if(then_index == u_index)
    {
      then_next_low = then_low;
      then_next_high = then_high;
    }
  else
    {
      then_next_low = then;
      then_next_high = then;
    }

  if(else_index == u_index)
    {
      else_next_low = else_low;
      else_next_high = else_high;
    }
  else
    {
      else_next_low = els;
      else_next_high = els;
    }

   // Add code for context based simplification, then evaluateITE, then the following

  u_low = applyITE(condn_next_low, then_next_low, else_next_low);
  u_high = applyITE(condn_next_high, then_next_high, else_next_high);

  u = createDD(u_AP, u_low, u_high);

  result = TableForITE.hashtable_insert(key, u);
  if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyITE\n";
	    exit(1);
	  }

  return u;
}



// Function to create a DAG from a DD
DAGNode* DDManager::createDAGFromDD(DDNode *dd)
{
  HashTable <string, DAGNode*> MapForDAGFromDD;
  HashTable <string, DAGNode*> *PtrMapForDAGFromDD = &MapForDAGFromDD;

  DAGNode* result = createDAGFromDDStep(dd, PtrMapForDAGFromDD);
  return result;
}

// Internal function called by createDAGFromDD function
DAGNode* DDManager::createDAGFromDDStep(DDNode *dd, HashTable <string, DAGNode*> *PtrMapForDAGFromDD)
{
  char temp_char[10];
  sprintf(temp_char , "%x", dd);
  string key = temp_char;
  
  // key created

  HTStatusValue<DAGNode*> ResultFromHT = PtrMapForDAGFromDD->hashtable_search(key);

  if(ResultFromHT.success())
    {
      return ResultFromHT.GetValue();
    }

  // createDAGFromDDStep was not earlier called with argument 

  DAGNode *result;
  if(dd->getPredicate()==NULL)
    {
      int val=dd->getVal();
      string constant;
      if(val==1) constant="true"; else constant="false";

      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      DAGNode *bit_node = dm.createNode(constant,(void*)v);

      result = bit_node;
    }
  else
    {
      vector<DAGNode *> ChildrenOfIte;
      ChildrenOfIte.push_back(dd->getPredicate());
      ChildrenOfIte.push_back( createDAGFromDDStep(dd->getRight(), PtrMapForDAGFromDD) );
      ChildrenOfIte.push_back( createDAGFromDDStep(dd->getLeft(), PtrMapForDAGFromDD) );

      DAGNode *ite_node = dm.createNode("if_then_else", ChildrenOfIte, ChildrenOfIte[1]->getValue());

      result = ite_node;
	
      // Note that DD for equality eq is eq(0, 1).i.e eq with left child 0 and right child 1
    }


  ResultFromHT = PtrMapForDAGFromDD->hashtable_insert(key, result);
  if(!ResultFromHT.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::createDAGFromDDStep\n";
      exit(1);
    }

  return result;
}


// Implementation of QuaLMoDE in our CAV'11 work.
// Returns \exists VarsToElim. f as DDNode*.
DDNode* DDManager::MultiVarQElimWithStrategy(list<string> VarsToElim, DDNode* f, map<string, int> WidthTable)
{
  time_t drop_time, find_time, t2;

  DDNode *result;
  result = f;

   while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function MultiVarQElimWithStrategy in DDManager.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return result;
	}
      
      showList(VarsToElim, "VarsToElim");

      /*
      bool drop_variables_enabled=false; // dropping can sometimes become inexact

      if(drop_variables_enabled){

      cout<<"\nFinding the vars that can be dropped\n";
      set<string> DropVars;

      time(&find_time);
      findDropVars(result, VarsToElim, DropVars);
      time(&t2);
      cout<<"\n\nTime spent in findDropVars  = "<< (t2 - find_time)<< "SECONDS"<<endl<<endl;

      cout<<"\nVariables selected for dropping\n";
      showSet(DropVars, "DropVars");

      if(!DropVars.empty())
        {
          cout<<"\nDropping these vars\n";

         
	  time(&drop_time);
          HashTable <string, DDNode*> MapForDropVars;
	  HashTable <string, DDNode*> *PtrMapForDropVars = &MapForDropVars;

          if(simplification_mode==2)
		result = DropVariablesWithImplicationCheck(DropVars, result, PtrMapForDropVars, WidthTable);  
	  else if(simplification_mode==1)
		{
		  //cout<<"\nCalling DropVariablesWithoutImplicationCheck\n";
		result = DropVariablesWithoutImplicationCheck(DropVars, result, PtrMapForDropVars, WidthTable);
		//cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;
		result = eliminateInconsistentPaths(result, WidthTable);
		//cout<<"\nSize of result after elimination of inconsistent paths = "<<getSizeOfDD(result)<<endl;
		}
	  else if(simplification_mode==0)
		{
		  //cout<<"\nCalling DropVariablesWithoutImplicationCheck\n";
		result = DropVariablesWithoutImplicationCheck(DropVars, result, PtrMapForDropVars, WidthTable);
		//cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;
		}
	  else
		{
		cout<<"\nInvalid simplification mode. Exiting\n"; exit(1);
		}

	  time(&t2);

          cout<<"\n\nTime spent in DropVariables  = "<< (t2 - drop_time)<<" SECONDS "<<endl<<endl;

	  //showDD(result);
          //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;

          for(set<string>::iterator it=DropVars.begin(); it != DropVars.end(); it++)
            VarsToElim.remove(*it);

          continue;
        }

      cout<<"\nNo more variables to drop\n";

      }
       */


      string VarToElim = chooseVar(result, VarsToElim);

      cout<<"\nVariable chosen to eliminate = "<<VarToElim<<endl;

      cout<<"\ncallQElim called = "<<VarToElim<<endl;

      cout<<"\nNumber of nodes in the input DD = "<<getSizeOfDD(result)<<endl;


      time(&drop_time);
      result = callQElim(VarToElim, result, WidthTable);
      time(&t2);

      cout<<"\n\nTime spent in callQElim to eliminate "<<VarToElim<<"  = "<< (t2 - drop_time)<<" SECONDS "<<endl<<endl;
      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;

      //showDD(result);
 
      int NoOfDDNodes = 0;
      SetOfDDNodes.clear();
      getNoOfNodesInDD(result, NoOfDDNodes);

      cout<<"\nNumber of nodes in the output DD = "<<NoOfDDNodes<<endl;

      if(NoOfDDNodes > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = NoOfDDNodes;
	}


      VarsToElim.remove(VarToElim); 

      //int userchar = getchar(); 
      //showAPMap();
    }

   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function MultiVarQElimWithStrategy in DDManager.cpp has timed out. It's result is not exact\n";
       timed_out = true; // timed_out flag set
       return result;
     }


  return result;
}

// Find the # of nodes in which each variable occures in the LMDD rooted at "v".
// Called from chooseVar
void DDManager::findDropVarsStep(DDNode *v, map<string, int> &VarsToElimCount)
{
  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      // Already visited
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {
      if(v->getPredicate()!=NULL)// non-terminal
        {
          findDropVarsStep(v->getLeft(), VarsToElimCount);
          findDropVarsStep(v->getRight(), VarsToElimCount);
          updateVarsToElimCount(v->getPredicate(), VarsToElimCount);
        }
      SetOfDDNodes.insert(v);
    }
}


// Efficient version of findDropVarsStep
void DDManager::findDropVarsStepEfficient(DDNode *v, map<string, int> &VarsToElimCount, HashTable <string, bool> *PtrDropVarsHT)
{
  char temp_char[10];
  sprintf(temp_char , "%x", v);
  string key = temp_char;

  // key created

  HTStatusValue<bool> ResultFromHT = PtrDropVarsHT->hashtable_search(key);

  if(ResultFromHT.success())// already encountered node. do nothing
        {
          //cout<<"\nalready encountered node. do nothing\n";
          return;
        }

  // new node  
  if(v->getPredicate()!=NULL)// non-terminal
        {
          findDropVarsStepEfficient(v->getLeft(), VarsToElimCount, PtrDropVarsHT);
          findDropVarsStepEfficient(v->getRight(), VarsToElimCount, PtrDropVarsHT);
          updateVarsToElimCount(v->getPredicate(), VarsToElimCount);
        }
  
  
  ResultFromHT = PtrDropVarsHT->hashtable_insert(key, true);
  if(!ResultFromHT.success())
        {
        cout<<"\nHash Insert Failure Inside Function DDManager:findDropVarsStepEfficient\n";
        exit(1);
        }   
}

// Updates the variable ---> # occurances map given an LME "predicate"
// Called from findDropVarsStep
void DDManager::updateVarsToElimCount(DAGNode *predicate, map<string, int> &VarsToElimCount)
{
 bool old_implementation = false;
 
 if(old_implementation)
 {    
  map<string, int>::iterator it = VarsToElimCount.begin();

  for(;it!=VarsToElimCount.end(); it++)
    {
      if(!freeOfVarModified(predicate, it->first))
        it->second = it->second+1;
    }
 }//old_implementation
 else
 {
   set<string> Predicate_Support;
   getDAGNames(predicate, Predicate_Support);
   
   for(set<string>::iterator it = Predicate_Support.begin(); it != Predicate_Support.end(); it++)
    {
    string variable = *it;
    map<string, int>::iterator mit = VarsToElimCount.find(variable);
    if(mit != VarsToElimCount.end())
        {
        mit->second = mit->second+1;
        }
    }//for
   
 }//!old_implementation
}//function ends here


// Returns the variable to be eliminated next from the given LMDD
string DDManager::chooseVar(DDNode *v, list<string> &VarsToElim)
{
  map<string, int> VarsToElimCount;

  for(list<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      string var = *it;
      VarsToElimCount.insert(make_pair(var, 0));
    }

  
  bool old_implementation = false;
  
  if(old_implementation)
  {
  SetOfDDNodes.clear();
  findDropVarsStep(v, VarsToElimCount);
  }
  else
  {    
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;    
  gettimeofday (&start_ms, NULL);    
      
  HashTable <string, bool> DropVarsHT;
  HashTable <string, bool> *PtrDropVarsHT = &DropVarsHT;
  
  findDropVarsStepEfficient(v, VarsToElimCount, PtrDropVarsHT);
  
  gettimeofday (&finish_ms, NULL);
  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;  
  
  cout<<"\nTime spent in findDropVarsStepEfficient = "<<duration_ms<<" milli seconds\n";
  }
  

  map<string, int>::iterator mit = VarsToElimCount.begin();
  map<string, int>::iterator mit_selected;
  int min;

  mit_selected = mit;
  min = mit->second;
  mit++;

  for(;mit!=VarsToElimCount.end(); mit++)
    {
      if(mit->second < min)
        {
          mit_selected = mit;
          min = mit->second;
        }
    }
  
  CountsForVariablesEliminated.push_back(mit_selected->second);
          
  return mit_selected->first;
}

// Calls the function QE1_LMDD in CAV'11 work.
// Returns the LMDD for \exists VarToElim. f.
DDNode* DDManager::callQElim(string VarToElim, DDNode* f, map<string, int> WidthTable)
{
 
  TableForDagResolve.clear();

  set<DAGNode*> E, D;
  E.clear(); D.clear();


  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;

  DDNode* result;

  if(simplification_mode==2)
	{
        cout<<"\nQElimWithDPModifiedWithImplicationCheck Started\n";
	result = QElimWithDPModifiedWithImplicationCheck(VarToElim, f, E, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
	cout<<"\nQElimWithDPModifiedWithImplicationCheck Finished\n";
	}

  else if(simplification_mode==1)
	{
	  //cout<<"\nCalling QElimWithDPModifiedWithoutImplicationCheck\n";
	result = QElimWithDPModifiedWithoutImplicationCheck(VarToElim, f, E, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
	//cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;
	result = eliminateInconsistentPaths(result, WidthTable);
	//cout<<"\nSize of result after elimination of inconsistent paths = "<<getSizeOfDD(result)<<endl;
	}
  else if(simplification_mode==0)
	{
	  //cout<<"\nCalling QElimWithDPModifiedWithoutImplicationCheck\n";
	result = QElimWithDPModifiedWithoutImplicationCheck(VarToElim, f, E, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
	//cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;
	}
  else
	{
	cout<<"\nInvalid simplification mode\n";
	exit(1);
	}
  return result;
}

// Function QE1_LMDD in CAV'11 work with elimination of inconsistent paths
// Returns the LMDD for \exists VarToElim. (f/\E/\D)
DDNode* DDManager::QElimWithDPModifiedWithImplicationCheck(string VarToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function QElimWithDPModifiedWithImplicationCheck in DDManager.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}


  //showContext(f, E, D); 

  DDNode* Existing  = getEntryFromTableForQElimModified(VarToElim, f, E, D, PtrTableForQElimModified);

  if(Existing != NULL)
    {
      //cout<<"\nEntry existing in hash table returned\n";

     //cout<<"\nsize of DD returned = "<<getSizeOfDD(Existing)<<endl;

      return Existing;
    }

  //cout<<"\nNo entry existing in hash table\n";

  if(f->getPredicate()==NULL) // Terminal node
    {
      //cout<<"\nTerminal node reached\n";

      DDNode* result;

      int value = f->getVal();

      if(value == 0)
        {
	  //cout<<"\nresult=0 returned\n";

          result = f;
        }
      else
        {

          if(E.empty() && D.empty())
            {
	      //cout<<"\nContext empty. hence result=1 returned\n";

              result = createDD(1);
            }
          else if(ThUnsat(E, D, WidthTable))
            {
	      //cout<<"\nContext inconsistent. hence result=0 returned\n";
	      
              result = createDD(0);
            }
          else
            {
	      //cout<<"\nContext consistent. APResolve invoked\n";
	      
	      result = APResolveModified(E, D, VarToElim, WidthTable, PtrTableForAPResolveModified);

	      //cout<<"\nNumber of nodes in result of APResolve = "<<getSizeOfDD(result)<<endl;
	      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;
            }
        }

      insertEntryIntoTableForQElimModified(VarToElim, f, E, D, result, PtrTableForQElimModified);

      //cout<<"\nsize of DD returned = "<<getSizeOfDD(result)<<endl;

      return result;
    }
  else
    {

      if(DAGResolveApplicable(E, D, WidthTable))
        {
	  //cout<<"\nDAGResolve is applicable\n";

          DAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable);
          if(e1 != NULL)
            {
	      //cout<<"\nEquation selected is "<<getDAGStr(e1)<<endl;

              //cout<<"\nOriginal DD before DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

	      //cout<<"\nNumber of nodes in original dd = "<<getSizeOfDD(f)<<endl;
 
              f = DagResolveModifiedAdvancedWithDynamicVariableOrdering(f, e1, VarToElim, WidthTable);

              //cout<<"\nDD after DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

	      //cout<<"\nNumber of nodes in DAGResolved dd = "<<getSizeOfDD(f)<<endl;

              //f is simplified here. Check if there exists an entry with this simplfied version of f

              DDNode* Existing  = getEntryFromTableForQElimModified(VarToElim, f, E, D, PtrTableForQElimModified);

              if(Existing != NULL)
                {
		  //cout<<"\nEntry exists for the simplified dd in hash table\n";

                 //cout<<"\nsize of DD returned = "<<getSizeOfDD(Existing)<<endl;

                  return Existing;
                }

              if(DDFreeOfVar(f, VarToElim))
                {
		  //cout<<"\nEntry does not exist for the simplified dd in hash table\n";

                  DDNode *temp;
                  if(ThUnsat(E, D, WidthTable)) temp = createDD(0);
                  else 
		    {
		      temp = APResolveModified(E, D, VarToElim, WidthTable, PtrTableForAPResolveModified);
		    }

		  //cout<<"\nNumber of nodes in result of APResolve on context = "<<getSizeOfDD(temp)<<endl;

                  //cout<<"\nAPResolve on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

                  temp = applyWithImplicationCheck("and", f, temp, WidthTable);

		  //cout<<"\nNumber of nodes in result of APResolve on context and dag resolved output = "<<getSizeOfDD(temp)<<endl;

		  insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

		  //cout<<"\ntemp i.e. result = "<<getDAGStr(createDAGFromDD(temp))<<endl;

		 //cout<<"\nsize of DD returned = "<<getSizeOfDD(temp)<<endl;

		  return temp;
                }
            }
        }

      DAGNode* C = f->getPredicate();

      //cout<<"\nLME encountered = "<<getDAGStr(C)<<endl;

      if(freeOfVarModified(C, VarToElim))
        {

	  //cout<<"\nLME free of "<<VarToElim<<endl;

          DDNode *temp;

          DDNode *thenb = QElimWithDPModifiedWithImplicationCheck(VarToElim, f->getRight(), E, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
          DDNode *elseb = QElimWithDPModifiedWithImplicationCheck(VarToElim, f->getLeft(), E, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);

          DDNode* C_dd = createDD(C, WidthTable);

	  //cout<<"\nNumber of nodes in arguments of createITE = "<<getSizeOfDD(thenb)<<"\t"<<getSizeOfDD(elseb)<<"\t"<<getSizeOfDD(C_dd)<<endl;

          temp = createITEWithImplicationCheck(C_dd, thenb, elseb, WidthTable);

	  //cout<<"\nNumber of nodes in output of createITE = "<<getSizeOfDD(temp)<<endl;

	  insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

	  //cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;

	  //cout<<"\nsize of DD returned = "<<getSizeOfDD(temp)<<endl;

          return temp;
        }
      else
        {
	  //cout<<"\nLME not free of "<<VarToElim<<endl;

          DDNode *temp;

          set<DAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C));

          DDNode *thenb = QElimWithDPModifiedWithImplicationCheck(VarToElim, f->getRight(), E_new, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
          DDNode *elseb = QElimWithDPModifiedWithImplicationCheck(VarToElim, f->getLeft(), E, D_new, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);

	  //cout<<"\nNumber of nodes in arguments of or = "<<getSizeOfDD(thenb)<<"\t"<<getSizeOfDD(elseb)<<endl;

           temp = applyWithImplicationCheck("or", thenb, elseb, WidthTable);

	   //cout<<"\nNumber of nodes in output of or = "<<getSizeOfDD(temp)<<endl;

	   insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

	  //cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;

	  //cout<<"\nsize of DD returned = "<<getSizeOfDD(temp)<<endl;

          return temp;
        }
    }
}

// Check if the instance \exists VarToElim. (f /\ E/\ D) is already encountered
// If yes, return the result; NULL otherwise
DDNode* DDManager::getEntryFromTableForQElimModified(string VarToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, HashTable <string, DDNode*> *PtrTableForQElimModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }


  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForQElimModified->hashtable_search(key);
  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}


// Return true if /\E /\ D is unsat; false otherwise
// Uses STP/yices
bool DDManager::ThUnsat(set<DAGNode*> E, set<DAGNode*> D, map<string, int> WidthTable)
{

  if(internal_solver=="stp") // use any solver supporting SMTLib format; presenly STP
	{
	return ThUnsatInSMTLibFormat(E, D, WidthTable);
	}

  thunsat_calls++;

  DAGNode *root = NULL;

  set<DAGNode*>::iterator it=E.begin();
  for(; it!=E.end(); it++)
    if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

  it=D.begin();
  for(; it!=D.end(); it++)
    if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

  FILE *smt_file = fopen("thunsat.ys", "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create thunsat.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(root, WidthTable);
  
  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  system("yices -tc thunsat.ys > result_thunsat.txt");

  FILE *result_smt_file = fopen("result_thunsat.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::ThUnsat\n"; exit(1);
    }
}


// Enter \exists VarToElim. (f /\ E/\ D) ---> result into Hash Table
void DDManager::insertEntryIntoTableForQElimModified(string VarToElim, DDNode*f, set<DAGNode*> E, set<DAGNode*> D, DDNode* value, HashTable <string, DDNode*> *PtrTableForQElimModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForQElimModified->hashtable_search(key);
  
  if(result.success())
    {
      cout<<"\nInside function DDManager::insertEntryIntoTableForQElimModified. Entry already exists for this node. Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = PtrTableForQElimModified->hashtable_insert(key, value);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForQElimModified\n";
      exit(1);
    }
}



// Copy the set<DAGNode*> source into set<DAGNode*> dest
void DDManager::copySet(set<DAGNode*> &dest, set<DAGNode*> source)
{
  set<DAGNode*>::iterator it;
  for(it=source.begin(); it!=source.end(); it++)
    dest.insert(*it);
}


// Return true if the LMDD "f" is free of the variable "VarToElim";
// Return false otherwise
bool DDManager::DDFreeOfVar(DDNode *f, string VarToElim)
{
  if(f->getPredicate()!=NULL)
    {
      DAGNode *predicate = f->getPredicate();
      if(!(freeOfVarModified(predicate, VarToElim))) return false;
      if(!DDFreeOfVar(f->getLeft(), VarToElim)) return false;
      if(!DDFreeOfVar(f->getRight(), VarToElim)) return false;
      return true;
    }
  return true;
}

// Returns true if DAGResolve (simplifyLMDD in CAV'11 work) is applicable
// i.e. returns true if set of LMEs E is non-empty or if there exists at least a single
// LMD at bit-level; returns false otherwise
bool DDManager::DAGResolveApplicable(set<DAGNode*> &E, set<DAGNode*> &D, map<string, int> &WidthTable)
{
  if(!E.empty())
    return true;

  for(set<DAGNode*>::iterator it=D.begin(); it != D.end(); it++)
    {
      DAGNode* root = (*it);
      int NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);

      if(NoOfBitsInConstraint == 1)
        return true;
    }

  return false;
}

// Find \exists VarToElim. (/\ E /\ D) and convert the result to LMDD, and return the result
DDNode* DDManager::APResolveModified(set<DAGNode*> E, set<DAGNode*> D, string VarToElim, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
   //cout<<"\nAPResolve Called\n";
   
  //showContextOfAPResolve(VarToElim, E, D);

  DDNode* Existing  = getEntryFromTableForAPResolveModified(VarToElim, E, D, PtrTableForAPResolveModified);

  if(Existing != NULL)
    {
      //cout<<"\nAPResolve's hash table hit\n";
      return Existing;
    }


  if(output_conjunctions && apqelim_status_on)
	{
        vector<DAGNode*> ConstraintsVector;
	for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
		{
		ConstraintsVector.push_back(*it);
		}
	for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
		{
		ConstraintsVector.push_back(*it);
		}

	set<string> VarsToElim;
	VarsToElim.insert(VarToElim);
	createConjunctionAndPrintInSMTLibFormat(ConstraintsVector, VarsToElim, WidthTable);
	}


  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();
  names.insert(VarToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  attachSetToList(names, VarNames);

  NoVars=VarNames.size();

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;

  if(compare_apqelim_omegatest) // we need to compare between AP_QElim and Omegatest times, but continue with the dag computed by AP_QElim
    {
      if(strcmp(qEliminator_type,"-lma")==0)
	{
	  time_t start, end, duration;

	  struct timeval start_ms, finish_ms;
	  unsigned long long int duration_ms;

	  time(&start);

	  gettimeofday (&start_ms, NULL);

	  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);

	  gettimeofday (&finish_ms, NULL);

	  time(&end);
	  
	  duration = end - start;

	  Total_AP_QElim_Time = Total_AP_QElim_Time + duration;


	  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
	  	  
	  Total_AP_QElim_Time_In_ms =  Total_AP_QElim_Time_In_ms + duration_ms;
	  

	  time(&start);

	  gettimeofday (&start_ms, NULL);
	  
	  DAGNode *tempDAG = qEliminator_ILP_ReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, VarNames, WidthTable);

	  gettimeofday (&finish_ms, NULL);

	  time(&end);
	  
	  duration = end - start;

	  Total_Omega_Test_Time = Total_Omega_Test_Time + duration;

	  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
	  	  
	  Total_Omega_Test_Time_In_ms =  Total_Omega_Test_Time_In_ms + duration_ms;

	}
      else
	{
	  cout<<"\nUnknown qEliminator_type "<<qEliminator_type<<" inside function DDManager::APResolveModified\n";
	  exit(1);
	}
    }
  else if(compare_apqelim_blasting) // we need to compare between AP_QElim and blasting, but continue with the dag computed by AP_QElim
   {
      if(strcmp(qEliminator_type,"-lma")==0)
	{

	  time_t start, end, duration;

	  struct timeval start_ms, finish_ms;
	  unsigned long long int duration_ms;

	  time(&start);

	  gettimeofday (&start_ms, NULL);

	  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
          
	  gettimeofday (&finish_ms, NULL);

	  time(&end);
	  
	  duration = end - start;

	  Total_AP_QElim_Time = Total_AP_QElim_Time + duration;


	  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
	  	  
	  Total_AP_QElim_Time_In_ms =  Total_AP_QElim_Time_In_ms + duration_ms;


	  time(&start);

	  gettimeofday (&start_ms, NULL);


	  DAGNode *tempDAG = qEliminatorReturningDAGOnlyForOddCoefficientsFollowedByBitBlasting(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);

	  gettimeofday (&finish_ms, NULL);

	  time(&end);
	  
	  duration = end - start;

	  Total_Blasting_Time = Total_Blasting_Time + duration;

	  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
	  	  
	  Total_Blasting_Time_In_ms = Total_Blasting_Time_In_ms  + duration_ms;

	}
      else
	{
	  cout<<"\nUnknown qEliminator_type "<<qEliminator_type<<" inside function callMatrixQEliminator\n";
	  exit(1);
	}
	
    }
  else
    {
      if(strcmp(qEliminator_type,"-lma")==0)
	{
	  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
	}
      else if(strcmp(qEliminator_type,"-ilp")==0)
	{

	  resultDAG = qEliminator_ILP_ReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, VarNames, WidthTable);
	}  
      else
	{
	  cout<<"\nUnknown qEliminator_type "<<qEliminator_type<<" inside function DDManager::APResolveModified\n";
	  exit(1);
	}
    }

  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }


  DDNode* resultDD;

  if(simplification_mode==2)
    {
	resultDD = createDDFromDAGWithImplicationCheck(resultDAG, WidthTable);
    }

  else if(simplification_mode==1)
    {
	resultDD = createDDFromDAGWithoutImplicationCheck(resultDAG, WidthTable);
    }
  else if(simplification_mode==0)
    {
	resultDD = createDDFromDAGWithoutImplicationCheck(resultDAG, WidthTable);
    }
  
  else
	{
	cout<<"\nInvalid simplification mode\n";
	exit(1);
	}

  insertEntryIntoTableForAPResolveModified(VarToElim, E, D, resultDD, PtrTableForAPResolveModified);

  return resultDD;

}

// checks if \exists VarToElim. (/\ E /\ D) is already encountered
// return the result if yes; return NULL otherwise
DDNode* DDManager::getEntryFromTableForAPResolveModified(string VarToElim, set<DAGNode*> E, set<DAGNode*> D, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }


  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForAPResolveModified->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// enter \exists VarToElim. (/\ E /\ D) ---> result into HashTable
void DDManager::insertEntryIntoTableForAPResolveModified(string VarToElim, set<DAGNode*> E, set<DAGNode*> D, DDNode* value, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];


  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForAPResolveModified->hashtable_search(key);


  if(result.success())
    {
      cout<<"\nInside function DDManager::insertEntryIntoTableForAPResolveModified. Entry already exists for this node. Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = PtrTableForAPResolveModified->hashtable_insert(key, value);
   
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForAPResolveModified\n";
      exit(1);
    }
}


// return true if the coefficient of VarToElim in LME "equation" is odd; return false otherwise
bool DDManager::coeffIsOdd(DAGNode *equation, string VarToElim)
{
  ULINT coefficient=1;
  getCoefficient(equation, VarToElim, coefficient); // Now we have the coefficient of VarToElim in equation
  if(coefficient % 2 != 0) return true; // coefficient is odd
  return false;
}


// Select LME/bit-level LMD with the least coefficient for the variable to be eliminated, and return it
// Implements selectLME in CAV'11 work
DAGNode* DDManager::selectEquationWithLeastCoefficientForVarToElimAdvanced(set<DAGNode*> &E, set<DAGNode*> &D, string VarToElim, map<string, int> &WidthTable)
{

  // Write in matrix form
  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames, RemainingVarNames;
  set<string> names;

  names.clear();
  names.insert(VarToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  attachSetToList(names, VarNames);

  createRemainingVarNames(VarNames, RemainingVarNames); // RemainingVarNames = VarNames without first entry

  NoVars=VarNames.size();

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  if(EqCoefMat.empty()) return NULL;

  ULINT Mod=findPower(NoOfBits);

  int EqnToSubstIndex;

  getEquationWithOddCoefForGivenVariable(EqCoefMat, 0, EqnToSubstIndex);

  if(EqnToSubstIndex == -1)
    getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, 0, EqnToSubstIndex, Mod);

  Matrix SelectedEqn;
  copyRow(EqCoefMat, EqnToSubstIndex, SelectedEqn); // selected equation copied into SelectedEqn


  ULINT SelectedCoef = SelectedEqn[0][0];


  // Currently the equation is like 2^k*cx + M = const
  // Convert this to
  // 1) 2^k*cx = AI(M) + const

  deleteColumn(SelectedEqn, 0); // SelectedEqn is M
  aInverseOfMatrix(SelectedEqn, Mod); // SelectedEqn is AI(M) now


  // Convert this to
  // 2) 2^kx = MI(c)* (AI(M) + const)

  // Now, SelectedCoef = 2^k*c
  // Get c from this

  ULINT c_part = getOddPart(SelectedCoef);

  // Get MI(c)

  ULINT MI_c_part = mInverse(c_part, Mod);

  // make SelectedCoef = 2^k

  SelectedCoef = SelectedCoef/MI_c_part;

  // SelectedEqn = SelectedEqn * MI_c_part

  scalarMatMultiply(SelectedEqn, MI_c_part, Mod);

  ULINT Constant_Part = EqConstMat[EqnToSubstIndex][0];

  // Constant_Part = Constant_Part * MI_c_part

  Constant_Part = mul(Constant_Part, MI_c_part, Mod);

  DAGNode* EquationDAG = getEquationDAGFromEqn(SelectedCoef, SelectedEqn, Constant_Part, RemainingVarNames, VarToElim, Mod);


  return EquationDAG;

}


// RemainingVarNames = VarNames without first entry
void DDManager::createRemainingVarNames(list<string> VarNames, list<string> &RemainingVarNames)
{
  list<string>::iterator it = VarNames.begin();
  if(it == VarNames.end()) return;

  it++;
  for(;it != VarNames.end(); it++)
    RemainingVarNames.push_back(*it);
}



// Implements All Path QE 
// Returns DDNode* for \exists VarsToElim. (f/\E/\D)
DDNode* DDManager::QElimWithNoDP(set<string> VarsToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, map<string, int> WidthTable)
{

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function QElimWithNoDP in DDManager.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}

  if(f->getPredicate()==NULL) // Terminal node
    {
      DDNode* result;

      int value = f->getVal();

      if(value == 0)
        {
          result = f;
        }
      else
        {

          if(E.empty() && D.empty())
            {
              result = createDD(1);
            }
          else if(ThUnsat(E, D, WidthTable))
            {
              result = createDD(0);
            }
          else
            {
	      if(strcmp(qEliminator_type,"-lma")==0)
		{
		  result = APResolveModifiedWithMultipleVariablesUsingLMA(E, D, VarsToElim, WidthTable);
		}
	      else if(strcmp(qEliminator_type,"-ilp")==0)
		{
		  result = APResolveModifiedWithMultipleVariables(E, D, VarsToElim, WidthTable);
		}
	      else
		{
		  cout<<"\nUnknown qEliminator_type in function DDManager::QElimWithNoDP\n";
		  exit(1);
		}
            }
        }

      return result;
    }
  else
    {

      DAGNode* C = f->getPredicate();

      if(freeOfVars(C, VarsToElim))
        {
          DDNode *temp;

          DDNode *thenb = QElimWithNoDP(VarsToElim, f->getRight(), E, D, WidthTable);
          DDNode *elseb = QElimWithNoDP(VarsToElim, f->getLeft(), E, D, WidthTable);

          DDNode* C_dd = createDD(C, WidthTable);

	  temp = createITE(C_dd, thenb, elseb);

          return temp;
        }
      else
        {
          DDNode *temp;

          set<DAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C));

          DDNode *thenb = QElimWithNoDP(VarsToElim, f->getRight(), E_new, D, WidthTable);
          DDNode *elseb = QElimWithNoDP(VarsToElim, f->getLeft(), E, D_new, WidthTable);

	  temp = apply("or", thenb, elseb);

          return temp;
        }
    }
}


// Implements All Path QE 
// Returns DDNode* for \exists VarsToElim. f
DDNode* DDManager::MultiVarQElimWithNoDPNoStrategy(list<string> VarsToElim, DDNode* f, map<string, int> WidthTable)
{
  set<DAGNode*> E, D;
  E.clear(); D.clear();

  set<string> VarsToElimSet;
  for(list<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
     VarsToElimSet.insert(*it); 
    }

  return QElimWithNoDP(VarsToElimSet, f, E, D, WidthTable);

}

// Returns DDNode* for \exists VarsToElim. (/\E/\D)
DDNode* DDManager::APResolveModifiedWithMultipleVariables(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> WidthTable)
{

  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();

  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
     names.insert(*it); 
    }

  attachSetToList(names, VarNames);
  NoVarsToElim=VarsToElim.size();

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  attachSetToList(names, VarNames);

  NoVars=VarNames.size();

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));



  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;

  if(strcmp(qEliminator_type,"-lma")==0)
    {
    resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
    }
  else if(strcmp(qEliminator_type,"-ilp")==0)
    {
      cout<<"\nOmega Called\n";
      resultDAG = qEliminator_ILP_ReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, VarNames, WidthTable);
    }  
  else
    {
      cout<<"\nUnknown qEliminator_type "<<qEliminator_type<<" inside function DDManager::APResolveModified\n";
      exit(1);
    }

  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }


  DDNode* resultDD;

  resultDD = createDDFromDAGWithoutImplicationCheck(resultDAG, WidthTable);

  return resultDD;

}


// Newly added on 16 June 2010

// Show the APMap
void DDManager::showAPMap()
{

  cout<<"\nAPMap\n";
  map<DAGNode*, int>::iterator APMap_it = APMap.begin();
  for(; APMap_it != APMap.end(); APMap_it++)
    {
      cout<<endl<<APMap_it->first<<"\t"<<getDAGStr(APMap_it->first)<<"\t"<<APMap_it->second;
    }
}

// This function initializes the APMap before creating the LMDD from DAG
// This helps in smaller LMDDs
void DDManager::initializeAPMap(list<DAGNode*> &APList, map<string, int> &WidthTable)
{
  bool reverse_natural_order=false; // reverse natural order normally results in larger dds 
  if(reverse_natural_order)
    APList.reverse();
  list<DAGNode*>::iterator it=APList.begin();
  for(;it!=APList.end(); it++)
    {
      DAGNode* predicate = *it;
      //cout<<"\nOriginal Predicate = "<<getDAGStr(predicate)<<endl;
      
       if(predicate->Get_node_name() !="bvule" && predicate->Get_node_name() !="bvuge" && 
          predicate->Get_node_name() !="bvult" && predicate->Get_node_name() !="bvugt" &&
          predicate->Get_node_name() !="=" && predicate->Get_node_name() !="equivalent")
        {
        cout<<"\nUnallowed predicate type "<<predicate->Get_node_name()<<" in function DDManager::initializeAPMap\n";
        exit(1);
        }
      
      if(predicate->Get_node_name() !="bvule" && predicate->Get_node_name() !="bvuge" && predicate->Get_node_name() !="bvult" && predicate->Get_node_name() !="bvugt")
        {
        predicate = recreateEquationInProperForm(predicate, WidthTable);
        //cout<<"\nRecreated Equation = "<<getDAGStr(predicate)<<endl;
        }
      
      if(APMap.find(predicate) == APMap.end()) // predicate is not there already in the APMap. Hence insert the AP
	    {
            if(order_lmes_before_lmis)
                {
                    count++;

                    int InsertLocation;

                    // if LMI
                    if(predicate->Get_node_name() =="bvule" || predicate->Get_node_name() =="bvuge" || predicate->Get_node_name() =="bvult"  || predicate->Get_node_name() =="bvugt")
                        {                
                        InsertLocation = count + LMI_OFFSET;                
                        }
                    // if LME
                    else if(predicate->Get_node_name() =="=" || predicate->Get_node_name() =="equivalent")
                        {
                        InsertLocation = count;
                        }
                    else
                        {
                        cout<<"\nUnknown predicate type "<<predicate->Get_node_name()<<" in function DDManager :: initializeAPMap\n";
                        exit(1);
                        }
                    // Insert into APMap 

                    if(InsertLocation>MAX_AP) 
                            {
                            cout<<"\nIn function  DDManager :: initializeAPMap, count has exceeded MAX_AP. Increase MAX_AP\n";
                            exit(1);
                            }
                    if(checkIfIndexExists(InsertLocation)) // an entry exists in APMap with the same index : ERROR
                            {
                            cout<<"\nIn DDManager::initializeAPMap, an entry exists in APMap with the given index "<<InsertLocation<<endl;
                            exit(1);
                            }            
                    APMap.insert((pair<DAGNode*,int>(predicate, InsertLocation)));
                }//if(order_lmes_before_lmis)
            else
                {
                  count++;
                  if(count>MAX_AP) 

                    {cout<<"\nIn function  DDManager ::initializeAPMap , count has exceeded MAX_AP. Increase MAX_AP\n";
                    exit(1);
                    }

                  if(checkIfIndexExists(count)) // an entry exists in APMap with the same index : ERROR
                    {
                      cout<<"\nIn DDManager::initializeAPMap, an entry exists in APMap with the given index "<<count<<endl;
                      exit(1);
                    }
                  APMap.insert((pair<DAGNode*,int>(predicate,count)));
               }//else of if(order_lmes_before_lmis)
	    }//if(APMap.find(predicate) == APMap.end()) 
        else // already there
	    {
	      //cout<<"\nEquation is already there\n";
	    }
    }
}




// modified version of applyStep with context based simplification using implication check (On 20 June 2010)


// evaluate the operation op on terminal nodes

// Evaluate v1->getVal() op v2->getVal() if v1 and v2 point to terminal nodes;
// Else return -1
int DDManager::evaluateOpOnTerminals(string op, DDNode *v1, DDNode *v2)
{
  if(v1->getPredicate()!= NULL || v2->getPredicate()!= NULL) return -1; // either of v1 or v2 is a non-terminal. Cannot be evaluated

  int value1;
  int value2;

  value1 = v1->getVal(); 
  value2 = v2->getVal(); 

  if(op=="or")
    {
      if(value1 == 1 || value2 == 1) return 1;
      else return 0;
    }
  if(op=="and")
    {
      if(value1 == 0 || value2 == 0) return 0;
      else return 1;
    }
  if(op=="xor")
    {
      if(value1 == value2) return 0;
      else return 1;
    }
}



// evaluate ITE if all operands are constants. Else return -1
int DDManager::evaluateITEOnTerminals(DDNode *condn, DDNode *then, DDNode *els)
{
  if(condn->getPredicate()!= NULL || then->getPredicate()!= NULL || els->getPredicate()!= NULL)
    {
      return -1;
    }

  int c, t, e;

  c=condn->getVal(); 
  t=then->getVal(); 
  e=els->getVal(); 

  if(c==1)
    return t;
  else
    return e;

}


// modified version of applyITE with context based simplification using implication check
DDNode* DDManager::applyITEWithImplicationCheck(DDNode* condn, DDNode* then, DDNode* els, set<DAGNode*> E, set<DAGNode*> D, map<string, int> &WidthTable, set<DAGNode*> &Useful_Context, map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable, map<string, DDNode*> *PtrLocalHashTable_Level_1)
{
  // Declarations for finding effect of cache  
  applyitewithimpcheck_calls++;
  bool hashing_enabled=false;
  // Declarations for finding effect of cache ends here

  // Declarations for useful context finding
  bool useful_context_finding=true;
  set<DAGNode*> Useful_E_For_Impcheck;
  set<DAGNode*> Useful_D_For_Impcheck;
  set<DAGNode*> Useful_Context_For_Child1;
  set<DAGNode*> Useful_Context_For_Child2;
  set<DAGNode*> Useful_Context_For_Impcheck;
  set<DAGNode*> Context;
  

  Useful_E_For_Impcheck.clear();
  Useful_D_For_Impcheck.clear();
  Useful_Context_For_Child1.clear();
  Useful_Context_For_Child2.clear();
  Useful_Context_For_Impcheck.clear();
  Useful_Context.clear();
  Context.clear();

  bool subset_based_local_hashing=true;
  bool level_1_local_hashing=true;
  //showContextInApplyWithImpCheck(op,v1,v2,E,D);
  // Declarations for useful context finding ends here


  DDNode* u;
  string key;
  string key_level_1;
  DAGNode *Negated_u_AP;

  if(hashing_enabled){

  // Creating the hash key

  // Include context also in hash key

  key="";
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", condn); // condition
  temp_key = temp_char;
  key += temp_key;

  sprintf(temp_char , "%x", then); // then
  temp_key = temp_char;
  key += temp_key;

  sprintf(temp_char , "%x", els); // else
  temp_key = temp_char;
  key += temp_key;


  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;  // all equations in the context
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key; // all disequations in the context
    }


  // Hash key created

  HTStatusValue<DDNode*> result = TableForITEWithImpCheck.hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  }// if(hashing_enabled)


    if(level_1_local_hashing)
    {
      // Creating the hash key
      
      key_level_1=""; // operation
      string temp_key;
      char temp_char[10];

      sprintf(temp_char , "%x", condn); // first argument DD
      temp_key = temp_char;
      key_level_1 += temp_key;

      sprintf(temp_char , "%x", then); // second argument DD
      temp_key = temp_char;
      key_level_1 += temp_key;

      sprintf(temp_char , "%x", els); // third argument DD
      temp_key = temp_char;
      key_level_1 += temp_key;


      // Hash key created
      // checking in hash table

      map<string, DDNode*>::iterator Level_1_it = PtrLocalHashTable_Level_1->find(key_level_1);

      if(Level_1_it!=PtrLocalHashTable_Level_1->end())
	{
	  // code for finding effect of cache  
	  applyitewithimpcheck_local_cache_level1_hit++;
	  // code for finding effect of cache ends here
	  return Level_1_it->second;
	}
    }// if(level_1_local_hashing)



  if(subset_based_local_hashing)
    {
      DDNode* ddnode_from_cache = getEntryFromEfficientLocalHashTableTernaryOperation(PtrLocalHashTable, condn, then, els, E, D, Useful_Context);

      if(ddnode_from_cache!=NULL)
	{
	  applyitewithimpcheck_local_cache_hit++;
	  return ddnode_from_cache;
	}
    }

  applyitewithimpcheck_misses++;
  // TableForITEWithImpCheck MISS

  int u_value, u_index;
  DAGNode *u_AP;


  u_value = evaluateITEOnTerminals(condn, then, els); // if all operands are constants, then evaluate

  if(u_value!=-1)
    {
      u = createDD(u_value);
    }
 
 else
    {
	  int condn_index, then_index, else_index;
	  condn_index = getIndex(condn);
	  then_index = getIndex(then);
	  else_index = getIndex(els);

	  DDNode *condn_low, *condn_high, *then_low, *then_high, *else_low, *else_high;
	  DDNode *condn_next_low, *condn_next_high, *then_next_low, *then_next_high,  *else_next_low, *else_next_high;
	  DDNode *u_low, *u_high;

	  condn_low = condn->getLeft();
	  condn_high = condn->getRight();
	  then_low = then->getLeft();
	  then_high = then->getRight();
	  else_low = els->getLeft();
	  else_high = els->getRight();

	  u_index = findMin(findMin(condn_index, then_index), else_index);
	  u_AP = findAPFromIndex(u_index);
          Negated_u_AP = negateConstraint(u_AP);

	  if(condn_index == u_index)
	    {
	      condn_next_low = condn_low;
	      condn_next_high = condn_high;
	    }
	  else
	    {
	      condn_next_low = condn;
	      condn_next_high = condn;
	    }

	  if(then_index == u_index)
	    {
	      then_next_low = then_low;
	      then_next_high = then_high;
	    }
	  else
	    {
	      then_next_low = then;
	      then_next_high = then;
	    }

	  if(else_index == u_index)
	    {
	      else_next_low = else_low;
	      else_next_high = else_high;
	    }
	  else
	    {
	      else_next_low = els;
	      else_next_high = els;
	    }

	  // Add code for context based simplification, then evaluateITE, then the following
          // Now we have identified the top equation, and the next low and high
      	  // apply context based simplification

      	if(callImplicationCheck(E, D, u_AP, Negated_u_AP, WidthTable)) // E/\D=>u_AP . Hence only the right matters
		{
		  //showImplicationCheck(E, D, u_AP);
		  // code for finding useful context  
                if(useful_context_finding)
		{
                if(trimming_context_mod_based)
			{
			trimContext(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_class_based)
			{
	        	trimContextAdvanced(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck);
			}
                else if(trimming_context_overapproximate)
			{
			trimContextEager(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_strategy_four)
			{
			trimContextStrategyFour(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
                else
			{
			cout<<"\nUnknown trimming mode\n"; exit(1);
			}
		}
		  // code for finding useful context ends here

                u = applyITEWithImplicationCheck(condn_next_high, then_next_high, else_next_high, E, D, WidthTable, Useful_Context_For_Child1, PtrLocalHashTable, PtrLocalHashTable_Level_1);
		}
       else if(callImplicationCheck(E, D, Negated_u_AP, u_AP, WidthTable)) // E/\D=>not(u_AP) . Hence only the left matters
		{
		  //showImplicationCheck(E, D, negateConstraint(u_AP));	

		  // code for finding useful context 
		if(useful_context_finding)
		{
                if(trimming_context_mod_based)
			{
			trimContext(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_class_based)
			{
	        	trimContextAdvanced(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck);
			}
               else if(trimming_context_overapproximate)
			{
			trimContextEager(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_strategy_four)
			{
			trimContextStrategyFour(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
                else
			{
			cout<<"\nUnknown trimming mode\n"; exit(1);
			}
		}
		  // code for finding useful context ends here

		u = applyITEWithImplicationCheck(condn_next_low, then_next_low, else_next_low, E, D, WidthTable, Useful_Context_For_Child2, PtrLocalHashTable, PtrLocalHashTable_Level_1);
		}
       else // try to evaluate first, and then if not possible, go down
		{
		u = evaluateITE(condn, then, els); // evaluateITE
  		if(u == NULL)
    			{
			 set<DAGNode*> E_new, D_new;
          		 copySet(E_new, E);
          		 E_new.insert(u_AP);
          		 copySet(D_new, D);
			 D_new.insert(Negated_u_AP);

			 if(cache_tree_on)
				{
			 	CacheTree.moveDown(u_AP);
				}
		         u_high = applyITEWithImplicationCheck(condn_next_high, then_next_high, else_next_high, E_new, D, WidthTable, Useful_Context_For_Child1, PtrLocalHashTable, PtrLocalHashTable_Level_1);
	
			 if(cache_tree_on)
				{
			 	CacheTree.moveUp();
				}


			 if(cache_tree_on)
				{
			 	CacheTree.moveDown(Negated_u_AP);
				}
      			 u_low = applyITEWithImplicationCheck(condn_next_low, then_next_low, else_next_low, E, D_new, WidthTable, Useful_Context_For_Child2, PtrLocalHashTable, PtrLocalHashTable_Level_1);

			 if(cache_tree_on)
				{
			 	CacheTree.moveUp();
				}


			 u = createDD(u_AP, u_low, u_high);
   			}
		}
	}

   if(hashing_enabled){
  HTStatusValue<DDNode*> result = TableForITEWithImpCheck.hashtable_insert(key, u);
  if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyITEWithImplicationCheck\n";
	    exit(1);
	  }
   }

   // code for finding useful context 
   // show v1,v2,useful context and u
   if(useful_context_finding)
     {
       set<DAGNode*> temp1, temp2;

       set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin()));
       set_union(Useful_E_For_Impcheck.begin(), Useful_E_For_Impcheck.end(), Useful_D_For_Impcheck.begin(), Useful_D_For_Impcheck.end(),inserter(Useful_Context_For_Impcheck, Useful_Context_For_Impcheck.begin()));
       set_union(Useful_Context_For_Child1.begin(), Useful_Context_For_Child1.end(), Useful_Context_For_Child2.begin(), Useful_Context_For_Child2.end(),inserter(temp1, temp1.begin()));
       set_union(Useful_Context_For_Impcheck.begin(), Useful_Context_For_Impcheck.end(), temp1.begin(), temp1.end(),inserter(temp2, temp2.begin()));
       set_intersection(temp2.begin(), temp2.end(), Context.begin(), Context.end(),inserter(Useful_Context, Useful_Context.begin()));

       if(Useful_Context.size() > applyitewithimpcheck_max_context_size)
	    {
	      applyitewithimpcheck_max_context_size = Useful_Context.size();
 	      //cout<<"\nContext\n************\n";
	      //showLMDEContext(Context);
 	      //cout<<"\nUseful Context\n************\n";
	      //showLMDEContext(Useful_Context);
 	    }

       //showUsefulpartInApplyWithImpeck(op, v1, v2, Useful_Context, u);
     }
   // code for finding useful context ends here

   if(level_1_local_hashing && Useful_Context.empty())
      {
	// Hash key is key_level_1
	PtrLocalHashTable_Level_1->insert(make_pair(key_level_1, u));
      }// if(level_1_local_hashing)

   else if(subset_based_local_hashing)
     {
       set<DAGNode*> Temp_Useful_Context;
       copySetNew(Temp_Useful_Context, Useful_Context);


       insertIntoEfficientLocalHashTableTernaryOperation(PtrLocalHashTable, condn, then, els, Temp_Useful_Context, u);

       if(PtrLocalHashTable->size() > applyitewithimpcheck_local_cache_max_size)
	 {
	   applyitewithimpcheck_local_cache_max_size = PtrLocalHashTable->size();
	 }
     }


  return u;
}

// Version of apply with implication checks
DDNode* DDManager::applyWithImplicationCheck(string op, DDNode* v1, DDNode* v2, map<string, int> &WidthTable)
{
DDNode* u; // result DD
string key;

bool external_hashing_enabled=true;
if(external_hashing_enabled){

  // Creating the hash key

  key=op; // operation
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", v1); // first argument DD
  temp_key = temp_char;
  key += temp_key;

  sprintf(temp_char , "%x", v2); // second argument DD
  temp_key = temp_char;
  key += temp_key;

  // Hash key created

  HTStatusValue<DDNode*> result = ExternalTableForApplyWithImpCheck.hashtable_search(key);

  if(result.success())
    {
    // code for finding effect of cache  
    applystepwithimpcheck_external_cache_hit++;
    // code for finding effect of cache ends here
    return result.GetValue();
    }
  }// if(external_hashing_enabled)

applystepwithimpcheck_external_cache_miss++;

u = applyWithImplicationCheckInternal(op, v1, v2, WidthTable);

if(external_hashing_enabled){
      HTStatusValue<DDNode*> result = ExternalTableForApplyWithImpCheck.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyWithImplicationCheck\n";
	    exit(1);
	  }
      }
return u;
}

// Version of apply's internal with implication checks
DDNode* DDManager::applyWithImplicationCheckInternal(string op, DDNode* v1, DDNode* v2, map<string, int> &WidthTable)
{
  DDNode* result;

  if(cache_tree_on)
  	{
	  CacheTree.reset(); // make context empty. i.e. make it point to the root
	}

  set<DAGNode*> E, D;
  E.clear(); D.clear();

  set<DAGNode*> Useful_Context;
  Useful_Context.clear(); 


  map<string, map<set<DAGNode*>, DDNode*> > LocalHashTable;
  map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable = &LocalHashTable;


  map<string, DDNode*> LocalHashTable_Level_1;
  map<string, DDNode*> *PtrLocalHashTable_Level_1 = &LocalHashTable_Level_1;


  result = applyStepWithImplicationCheck(op, v1, v2, E, D, WidthTable, Useful_Context, PtrLocalHashTable, PtrLocalHashTable_Level_1);

  DDNode* reducedResult = Reduce(result);

 bool show_effect_of_reduce=false;
 if(show_effect_of_reduce)
	{
        int size_before_reduce = getSizeOfDD(result);
        int size_after_reduce = getSizeOfDD(reducedResult);
        if(size_before_reduce!=size_after_reduce)
		{
		cout<<"\nreduce effective\n";
		cout<<"\nBefore reduce = "<<size_before_reduce<<", after reduce = "<<size_after_reduce<<endl;
                effect_of_reduce = effect_of_reduce + size_before_reduce-size_after_reduce;
		}
	}

  // for outputting
  bool outputting_needed=false;
  if(outputting_needed)
	{
	  string str_to_show="=> ";
	  str_to_show += op;
	  printBlowupInApply(str_to_show, getSizeOfDD(v1),getSizeOfDD(v2),getSizeOfDD(reducedResult)); 
	}

  // for outputting ends here

  return reducedResult;
}

// Version of createITE with implication checks
DDNode* DDManager::createITEWithImplicationCheck(DDNode* condn, DDNode* then, DDNode* els, map<string, int> &WidthTable)
{
DDNode* u; // result DD
string key;

bool external_hashing_enabled=true;
if(external_hashing_enabled){

  // Creating the hash key
  string temp_key;
  char temp_char[10];

  key="";

  sprintf(temp_char , "%x", condn); // first argument DD
  temp_key = temp_char;
  key += temp_key;


  sprintf(temp_char , "%x", then); // second argument DD
  temp_key = temp_char;
  key += temp_key;

  sprintf(temp_char , "%x", els); // third argument DD
  temp_key = temp_char;
  key += temp_key;

  // Hash key created

  HTStatusValue<DDNode*> result = ExternalTableForCreateITEWithImpCheck.hashtable_search(key);

  if(result.success())
    {
    // code for finding effect of cache  
    createitewithimpcheck_external_cache_hit++;
    // code for finding effect of cache ends here
    return result.GetValue();
    }
  }// if(external_hashing_enabled)

createitewithimpcheck_external_cache_miss++;

u = createITEWithImplicationCheckInternal(condn, then, els, WidthTable);

if(external_hashing_enabled){
      HTStatusValue<DDNode*> result = ExternalTableForCreateITEWithImpCheck.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::createITEWithImplicationCheck\n";
	    exit(1);
	  }
      }
return u;
}

// Version of createITE's internal with implication checks
DDNode* DDManager::createITEWithImplicationCheckInternal(DDNode* condn, DDNode* then, DDNode* els, map<string, int> &WidthTable)
{
  DDNode* result;

  if(cache_tree_on)
  {
  CacheTree.reset(); // make context empty. i.e. make it point to the root
  }

  set<DAGNode*> E, D;
  E.clear(); D.clear();

  set<DAGNode*> Useful_Context;
  Useful_Context.clear(); 


  map<string, map<set<DAGNode*>, DDNode*> > LocalHashTable;
  map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable = &LocalHashTable;

  map<string, DDNode*> LocalHashTable_Level_1;
  map<string, DDNode*> *PtrLocalHashTable_Level_1 = &LocalHashTable_Level_1;

  result = applyITEWithImplicationCheck(condn, then, els, E, D, WidthTable, Useful_Context, PtrLocalHashTable, PtrLocalHashTable_Level_1);

  DDNode* reducedResult = Reduce(result);

   bool show_effect_of_reduce=false;
  if(show_effect_of_reduce)
	{
        int size_before_reduce = getSizeOfDD(result);
        int size_after_reduce = getSizeOfDD(reducedResult);
        if(size_before_reduce!=size_after_reduce)
		{
		cout<<"\nreduce effective\n";
		cout<<"\nBefore reduce = "<<size_before_reduce<<", after reduce = "<<size_after_reduce<<endl;
                effect_of_reduce = effect_of_reduce + size_before_reduce-size_after_reduce;
		}
	}

   // for outputting
  bool outputting_needed=false;
 
  if(outputting_needed)
	{
	  string str_to_show="=> ";
	  str_to_show += "ite";
	  printBlowupInApply(str_to_show, getSizeOfDD(condn),getSizeOfDD(then),getSizeOfDD(els),getSizeOfDD(reducedResult)); 
	}

  // for outputting ends here

  return reducedResult;
}


// Show f, E, and D
void DDManager :: showContext(DDNode *f, set<DAGNode*> E, set<DAGNode*> D)
{
  cout<<endl<<"DDNode = "<<f<<endl;
  
  cout<<"Context\n";
  cout<<"LMEs\n";

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      cout<<getDAGStr(*it)<<endl;
    }

  cout<<"LMDs\n";
  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
     cout<<getDAGStr(*it)<<endl;
    }
}

// return size of the LMDD rooted at "dd"
int DDManager::getSizeOfDD(DDNode *dd)
{
  int NoOfDDNodes = 0;
  SetOfDDNodes.clear();
  getNoOfNodesInDD(dd, NoOfDDNodes);
  return NoOfDDNodes;
}

// Final version of simplifyLMDD (CAV'11 work)
// Returns simplified version of LMDD rooted at "dd" 
DDNode*  DDManager::DagResolveModifiedAdvancedWithDynamicVariableOrdering(DDNode *dd, DAGNode *equation, string VarToElim, map<string, int> WidthTable)
{

  //cout<<"\nDAGResolve called\n";

  map< pair<DDNode*, DAGNode*>, DDNode*>::iterator it = TableForDagResolve.find(make_pair(dd, equation));

  if(it != TableForDagResolve.end())
    {
      return it->second;
    }

  if(dd->getPredicate() == NULL) // Terminal
    {
      return dd;
    }

  DDNode *NewLeft = DagResolveModifiedAdvancedWithDynamicVariableOrdering(dd->getLeft(), equation, VarToElim, WidthTable);
  DDNode *NewRight = DagResolveModifiedAdvancedWithDynamicVariableOrdering(dd->getRight(), equation, VarToElim, WidthTable);

  DAGNode *MyPredicate = dd->getPredicate();

  DAGNode *NewPredicate;

  // Check if MyPredicate contains the variable to be eliminated

  if(freeOfVarModified(MyPredicate, VarToElim))
    {
      NewPredicate = MyPredicate;
    }

  else if(!coeffIsOdd(equation, VarToElim)) // check if the coefficient of variable to be eliminated is odd in the equation selected
    {
      // Ideally, this should be done before selecting the equation
      NewPredicate = MyPredicate;
    }
 
  else
    {

      // Converting equation and MyPredicate to Matrix form

      // Write in matrix form
      int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
      Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

      // Get the list of variables,  NoVars, NoVarsToElim
      list<string> VarNames;
      set<string> names;

      names.clear();
      names.insert(VarToElim);
      attachSetToList(names, VarNames);
      NoVarsToElim=1;

      names.clear();

      // we should include variables from the equation and MyPredicate in names
      getDAGNames(equation, names);
      getDAGNames(MyPredicate, names);

      attachSetToList(names, VarNames);

      NoVars=VarNames.size();

      // Now we have the list of variables,  NoVars, NoVarsToElim

      set<DAGNode*> Constraints;
      Constraints.insert(equation);
      Constraints.insert(MyPredicate);

     
      // Get the equations
      bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

      ULINT Mod=findPower(NoOfBits);

      // Now everything is ready. Just call Matrix qEliminaror

      // Checking if Constraints is False before calling the eliminator

      set<DAGNode*> TemporaryDisEqSet;
      TemporaryDisEqSet.clear();
      bool theory_unsat = ThUnsat(Constraints, TemporaryDisEqSet,WidthTable);
      if(theory_unsat)
	{
	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = LOGICAL_CONSTANT;
	  t->width = 1;
	  v->set_node_type(*t);
	  NewPredicate = dm.createNode("false",(void*)v);
	}
      else
	{
	  apqelim_status_on=false; // APQElim called inside DAGQElim is not counted
	  NewPredicate = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
	  apqelim_status_on=true;

	  if(NewPredicate == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      NewPredicate = dm.createNode("true",(void*)v);
	    }
	  else // we have a new LME
	    {
	      if(!(NewPredicate->Get_node_name()=="=") && !(NewPredicate->Get_node_name()=="equivalent")) // not an LME
		{
		  cout<<"DAGResolve creats a new node which is not an LME \n";
		  exit(1);
		}

	      NewPredicate = recreateEquationInProperForm(NewPredicate, WidthTable); // recreate in cannonical form

              //cout<<"\nNewPredicate = "<<getDAGStr(NewPredicate)<<", MyPredicate = "<<getDAGStr(MyPredicate)<<endl;

	      // Note that setting dynamic_ordering=false will not change indices of APs in APMap. Also, the predicate NewPredicate is not inserted into the APMap. Now when createDDFromDAG(NewPredicate, WidthTable) is called, it will call createDD(NewPredicate, WidthTable), which will insert NewPredicate as the predicate with the maximum index. This will normally result in larger dd sizes after DAGResolve

	      bool dynamic_ordering=true;
	      if(dynamic_ordering)
		dynamicOrdering(MyPredicate, NewPredicate); // increment the indices of the predicates in APMap above MyPredicate by 1 and insert NewPredicate just on top of MyPredicate i.e. at MyPredicate's index + 1
	    }
		
	}

    }

  // Note that in DD nodes, left is else and right is then

  DDNode* then_dd = NewRight;
  DDNode* els_dd = NewLeft;

  DDNode* codn_dd;

  if(simplification_mode==2)
    {
	codn_dd = createDDFromDAGWithImplicationCheck(NewPredicate, WidthTable);
    }
  else if(simplification_mode==1)
    {
	codn_dd = createDDFromDAGWithoutImplicationCheck(NewPredicate, WidthTable);
    }
  else if(simplification_mode==0)
    {
	codn_dd = createDDFromDAGWithoutImplicationCheck(NewPredicate, WidthTable);
    }
  else
	{
	cout<<"\nInvalid simplification mode\n";
	exit(1);
	}

  DDNode* resultDD = createITE(codn_dd, then_dd, els_dd );

  TableForDagResolve.insert(make_pair(make_pair(dd, equation), resultDD));

  return resultDD;
}

// Function which places NewPredicate next to MyPredicate
void DDManager::dynamicOrdering(DAGNode *MyPredicate, DAGNode *NewPredicate)
{
  if(APMap.find(NewPredicate)!=APMap.end()) // entry already exists in APMap for the NewPredicate : Do nothing
		{
		//cout<<"\nEntry already exists in APMap for the predicate node "<<NewPredicate<<"\n";
		return;
		}

  int MyIndex;

  if(APMap.find(MyPredicate)==APMap.end()) // no entry in APMap for the MyPredicate : ERROR
		{
		cout<<"\nNo entry for APMap for the predicate node "<<MyPredicate<<"in function DDManager::dynamicOrdering\n";
		exit(1);
		}
  MyIndex = APMap[MyPredicate];

  //cout<<"\nMyIndex = "<<MyIndex<<endl;

  incrementIndexOfLMEsInAPMAPAboveGivenIndex(MyIndex);

  setIndex(NewPredicate, MyIndex+1);
  
  //cout<<"\nNewPredicate added at "<<MyIndex+1<<endl;

  count++; // as a new predicate is added
}

// Increments the index of all LMEs above the given index
void DDManager::incrementIndexOfLMEsInAPMAPAboveGivenIndex(int MyIndex)
{
  map<DAGNode*, int>::iterator APMap_it = APMap.begin();
  for(; APMap_it != APMap.end(); APMap_it++)
    {
      if(APMap_it->second > MyIndex) 
	{
	  //cout<<"\n"<<getDAGStr(APMap_it->first)<<" has index "<<APMap_it->second<<endl;

	  int new_index  = (APMap_it->second) + 1;
	  if(new_index>=MAX_AP+1)
	    {
	      cout<<"\nnew_index>=MAX_AP+1 inside function DDManager::incrementIndexOfLMEsInAPMAPAboveGivenIndex\n";
	      exit(1);
	    }
	 //  if(checkIfIndexExists(new_index)) // an entry exists in APMap with the same index : ERROR
// 	    {
// 	      cout<<"\nIn DDManager::incrementIndexOfLMEsInAPMAPAboveGivenIndex, an entry exists in APMap with the given index "<<new_index<<endl;
// 	      exit(1);
// 	    }
	  APMap_it->second = new_index;

	  //cout<<"\nIndex of "<<getDAGStr(APMap_it->first)<<" increased to "<<APMap_it->second<<endl;
	}
	  
    }
}


// Function to display input to APResolve
void DDManager :: showContextOfAPResolve(string VarToElim, set<DAGNode*> E, set<DAGNode*> D)
{
  cout<<endl<<"VarToElim = "<<VarToElim<<endl;
  
  cout<<"Context\n";
  cout<<"LMEs\n";

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      cout<<(*it)<<"\t"<<getDAGStr(*it)<<endl;
    }

  cout<<"LMDs\n";
  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
     cout<<(*it)<<"\t"<<getDAGStr(*it)<<endl;
    }
}


// Function to create a DD from a DAG without elimination of inconsistent paths
// with dynamic programming enabled
// Note that the hash table used here is global
DDNode* DDManager::createDDFromDAGWithoutImplicationCheck(DAGNode *dag, map<string, int> &WidthTable)
{
  HashTable <string, DDNode*> *PtrMapForDDFromDAG = &MapForDDFromDAG;
  DDNode* result = createDDFromDAGWithoutImplicationCheck_Internal(dag, WidthTable, PtrMapForDDFromDAG);
  return result;
}



// Function called by createDDFromDAGWithoutImplicationCheck
DDNode* DDManager::createDDFromDAGWithoutImplicationCheck_Internal(DAGNode *dag, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrMapForDDFromDAG)
{
   
  char temp_char[10];
  sprintf(temp_char , "%x", dag);
  string key = temp_char;
  
  // key created

  HTStatusValue<DDNode*> ResultFromHT = PtrMapForDDFromDAG->hashtable_search(key);

  if(ResultFromHT.success())
    {
      //cout<<"\nHit\n";
      return ResultFromHT.GetValue();
    }  
    
  //cout<<"\nMiss\n";
  
  DDNode* result;
    
  string dag_str = dag->Get_node_name();

  if(dag_str == "=" || dag_str == "equivalent" ||  dag_str =="bvule" || dag_str == "bvuge"  || dag_str =="bvult" || dag_str =="bvugt")        
    {
      result = createDD(dag, WidthTable);
    }
 else if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;
      DAGNode* inverseDAG = createDAG("=", left, right, LOGICAL_SYMBOL, 1);
      DDNode* inverseDD = createDD(inverseDAG, WidthTable);
      result = createDDNot(inverseDD);
    }
  else if(dag_str == "and")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;


      DDNode* then_dd = createDDFromDAGWithoutImplicationCheck_Internal(left, WidthTable, PtrMapForDDFromDAG);
      DDNode* els_dd = createDDFromDAGWithoutImplicationCheck_Internal(right, WidthTable, PtrMapForDDFromDAG);

      DDNode* result_dd = apply("and", then_dd, els_dd);
      
      unsigned int outlist_size = dag->Get_outlist_size();
      if(outlist_size==2) 
        {
         result = result_dd;
        }
      else
	{
        iout++;
	for(int i=2;i<outlist_size;i++)
		{
                DAGNode *child = *iout; 
                DDNode* child_dd = createDDFromDAGWithoutImplicationCheck_Internal(child, WidthTable, PtrMapForDDFromDAG);
		result_dd = apply("and", result_dd, child_dd);
                iout++;
		}
	result = result_dd;
	}   
    }
  else if(dag_str == "or")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;


      DDNode* then_dd = createDDFromDAGWithoutImplicationCheck_Internal(left, WidthTable, PtrMapForDDFromDAG);
      DDNode* els_dd = createDDFromDAGWithoutImplicationCheck_Internal(right, WidthTable, PtrMapForDDFromDAG);

      DDNode* result_dd = apply("or", then_dd, els_dd);
      
      unsigned int outlist_size = dag->Get_outlist_size();
      if(outlist_size==2) 
        {
         result = result_dd;
        }
      else
	{
        iout++;
	for(int i=2;i<outlist_size;i++)
		{
                DAGNode *child = *iout; 
                DDNode* child_dd = createDDFromDAGWithoutImplicationCheck_Internal(child, WidthTable, PtrMapForDDFromDAG);
		result_dd = apply("or", result_dd, child_dd);
                iout++;
		}
	result = result_dd;
	}   
    }
  else if(dag_str == "if_then_else")
    {
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *codn, *left, *right;
      codn = *iout;
      iout++;
      left =*iout;
      iout++;
      right =*iout;

      
      DDNode* then_dd = createDDFromDAGWithoutImplicationCheck_Internal(left, WidthTable, PtrMapForDDFromDAG);
      DDNode* els_dd = createDDFromDAGWithoutImplicationCheck_Internal(right, WidthTable, PtrMapForDDFromDAG);
      DDNode* codn_dd = createDDFromDAGWithoutImplicationCheck_Internal(codn, WidthTable, PtrMapForDDFromDAG);

      result = createITE(codn_dd, then_dd, els_dd);      
    }
  else if(dag_str == "1" || dag_str == "true")
    {
      result = createDD(1);
    }
  else if(dag_str == "0" || dag_str == "false")
    {
      result = createDD(0);
    }
  // Newly added on 8/4/2010 to account for "not"
  else if(dag_str == "not")
    {
      //cout<<endl<<"dag = "<<getDAGStr(dag)<<endl;
      
      list<DAGNode*> :: iterator iout = dag->Get_outlist_start();
      DAGNode *child;
      child = *iout;
      
      //cout<<endl<<"child = "<<getDAGStr(child)<<endl;      
      
      //DDNode* inverseDD = createDD(child, WidthTable);
      DDNode* inverseDD = createDDFromDAGWithoutImplicationCheck_Internal(child, WidthTable, PtrMapForDDFromDAG);
      
      result = createDDNot(inverseDD);
    }
  else
    {
      cout<<"\nDDManager::createDDFromDAGWithoutImplicationCheck_Internal called with dag name "<<dag_str<<". Exiting...\n"; exit(1);
    } 
  
  
  ResultFromHT = PtrMapForDDFromDAG->hashtable_insert(key, result);
  if(!ResultFromHT.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::createDDFromDAGWithoutImplicationCheck_Internal\n";
      exit(1);
    }  
  return result;
}


// Function QE1_LMDD in CAV'11 work without elimination of inconsistent paths
// Returns the LMDD for \exists VarToElim. (f/\E/\D)
DDNode* DDManager::QElimWithDPModifiedWithoutImplicationCheck(string VarToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{


  //cout<<"\n  DDManager::QElimWithDPModifiedWithoutImplicationCheck called \n";

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function QElimWithDPModifiedWithoutImplicationCheck in DDManager.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}
  

  //showContext(f, E, D); 

  DDNode* Existing  = getEntryFromTableForQElimModified(VarToElim, f, E, D, PtrTableForQElimModified);

  if(Existing != NULL)
    {
      //cout<<"\nEntry existing in hash table returned\n";

      return Existing;
    }

  //cout<<"\nNo entry existing in hash table\n";

  if(f->getPredicate()==NULL) // Terminal node
    {
      //cout<<"\nTerminal node reached\n";

      DDNode* result;

      int value = f->getVal();

      if(value == 0)
        {
	  //cout<<"\nresult=0 returned\n";

          result = f;
        }
      else
        {

          if(E.empty() && D.empty())
            {
	      //cout<<"\nContext empty. hence result=1 returned\n";

              result = createDD(1);
            }
          else if(ThUnsat(E, D, WidthTable))
            {
	      //cout<<"\nContext inconsistent. hence result=0 returned\n";
	      
              result = createDD(0);
            }
          else
            {
	      //cout<<"\nContext consistent. APResolve invoked\n";
	      
	      result = APResolveModified(E, D, VarToElim, WidthTable, PtrTableForAPResolveModified);

	      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;
            }
        }

      insertEntryIntoTableForQElimModified(VarToElim, f, E, D, result, PtrTableForQElimModified);

      return result;
    }
  else
    {

      if(DAGResolveApplicable(E, D, WidthTable))
        {
	  //cout<<"\nDAGResolve is applicable\n";

          DAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable);
          if(e1 != NULL)
            {
	      //cout<<"\nEquation selected is "<<getDAGStr(e1)<<endl;

              //cout<<"\nOriginal DD before DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

	      //cout<<"\nSize of original dd = "<<getSizeOfDD(f)<<endl;
 
              f = DagResolveModifiedAdvancedWithDynamicVariableOrdering(f, e1, VarToElim, WidthTable);

              //cout<<"\nDD after DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

	      //cout<<"\nSize of DAGResolved dd = "<<getSizeOfDD(f)<<endl;

              //f is simplified here. Check if there exists an entry with this simplfied version of f

              DDNode* Existing  = getEntryFromTableForQElimModified(VarToElim, f, E, D, PtrTableForQElimModified);

              if(Existing != NULL)
                {
		  //cout<<"\nEntry exists for the simplified dd in hash table\n";

                  return Existing;
                }

              if(DDFreeOfVar(f, VarToElim))
                {
		  //cout<<"\nEntry does not exist for the simplified dd in hash table\n";

		  //cout<<"\nAPResolve called\n";

                  DDNode *temp;
                  if(ThUnsat(E, D, WidthTable)) temp = createDD(0);
                  else 
		    {
		      temp = APResolveModified(E, D, VarToElim, WidthTable, PtrTableForAPResolveModified);
		    }

                  //cout<<"\nAPResolve on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

                  temp = apply("and", f, temp);

		  insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

		  //cout<<"\ntemp i.e. result = "<<getDAGStr(createDAGFromDD(temp))<<endl;

		  return temp;
                }
            }
        }

      DAGNode* C = f->getPredicate();

      //cout<<"\nLME encountered = "<<getDAGStr(C)<<endl;

      if(freeOfVarModified(C, VarToElim))
        {

	  //cout<<"\nLME free of "<<VarToElim<<endl;

          DDNode *temp;

          DDNode *thenb = QElimWithDPModifiedWithoutImplicationCheck(VarToElim, f->getRight(), E, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
          DDNode *elseb = QElimWithDPModifiedWithoutImplicationCheck(VarToElim, f->getLeft(), E, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);

          DDNode* C_dd = createDD(C, WidthTable);

          temp = createITE(C_dd, thenb, elseb);

          insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

	  //cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;

          return temp;
        }
      else
        {
	  //cout<<"\nLME not free of "<<VarToElim<<endl;

          DDNode *temp;

          set<DAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C));

          DDNode *thenb = QElimWithDPModifiedWithoutImplicationCheck(VarToElim, f->getRight(), E_new, D, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
          DDNode *elseb = QElimWithDPModifiedWithoutImplicationCheck(VarToElim, f->getLeft(), E, D_new, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);

           temp = apply("or", thenb, elseb);

          insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

	  //cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;

          return temp;
        }
    }
}

// Function for elimination of inconsistent paths in the DD rooted at "dd"
// Returns the DD after elimination of inconsistent paths
DDNode* DDManager::eliminateInconsistentPaths(DDNode* dd, map<string, int> &WidthTable)
{
  DDNode* TrueNode = createDD(1);

  DDNode* result;

  if(cache_tree_on)
  	{
  	CacheTree.reset();
	}

  set<DAGNode*> E, D;
  E.clear(); D.clear();

  set<DAGNode*> Useful_Context;
  Useful_Context.clear(); 


  map<string, map<set<DAGNode*>, DDNode*> > LocalHashTable;
  map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable = &LocalHashTable;

  map<string, DDNode*> LocalHashTable_Level_1;
  map<string, DDNode*> *PtrLocalHashTable_Level_1 = &LocalHashTable_Level_1;

  result = eliminateInconsistentPathsInternal(dd, TrueNode, E, D, WidthTable, Useful_Context, PtrLocalHashTable, PtrLocalHashTable_Level_1); // This basically performs applyWithImplicationCheck("and", true, dd) but without the context-insensitive (evaluation based) simplification

  DDNode* reducedResult = Reduce(result);
  return reducedResult;
}

// Internal function called by DDManager::eliminateInconsistentPaths
// Returns the DD after elimination of inconsistent paths 
// which is reduced in the caller function DDManager::eliminateInconsistentPaths
DDNode* DDManager::eliminateInconsistentPathsInternal(DDNode* v1, DDNode* v2, set<DAGNode*> E, set<DAGNode*> D, map<string, int> &WidthTable, set<DAGNode*> &Useful_Context, map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable, map<string, DDNode*> *PtrLocalHashTable_Level_1) // Note that v2 is always true
{

  // Declarations for useful context finding
  bool useful_context_finding=true;
  set<DAGNode*> Useful_E_For_Impcheck;
  set<DAGNode*> Useful_D_For_Impcheck;
  set<DAGNode*> Useful_Context_For_Child1;
  set<DAGNode*> Useful_Context_For_Child2;
  set<DAGNode*> Useful_Context_For_Impcheck;
  set<DAGNode*> Context;
  

  Useful_E_For_Impcheck.clear();
  Useful_D_For_Impcheck.clear();
  Useful_Context_For_Child1.clear();
  Useful_Context_For_Child2.clear();
  Useful_Context_For_Impcheck.clear();
  Useful_Context.clear();
  Context.clear();

  bool subset_based_local_hashing=true;
  bool level_1_local_hashing=true;
  //showContextInApplyWithImpCheck(op,v1,v2,E,D);
  // Declarations for useful context finding ends here

  DDNode* u; // result DD

  string key_level_1;
  DAGNode *Negated_u_AP;

    if(level_1_local_hashing)
    {
      // Creating the hash key
      
      key_level_1=""; // operation
      string temp_key;
      char temp_char[10];

      sprintf(temp_char , "%x", v1); // first argument DD
      temp_key = temp_char;
      key_level_1 += temp_key;

      sprintf(temp_char , "%x", v2); // second argument DD
      temp_key = temp_char;
      key_level_1 += temp_key;


      // Hash key created
      // checking in hash table

      map<string, DDNode*>::iterator Level_1_it = PtrLocalHashTable_Level_1->find(key_level_1);

      if(Level_1_it!=PtrLocalHashTable_Level_1->end())
	{
	  // code for finding effect of cache  
	  eliminate_incons_paths_int_local_cache_level1_hit++;
	  // code for finding effect of cache ends here
	  return Level_1_it->second;
	}
    }// if(level_1_local_hashing)


    if(subset_based_local_hashing)
    {
      
      DDNode* ddnode_from_cache = getEntryFromEfficientLocalHashTableBinaryOperation(PtrLocalHashTable, v1, v2, E, D, Useful_Context);

      if(ddnode_from_cache!=NULL)
	{
	  //cout<<"\nEliminate Inconsistent Path Cache Hit\n";
	  return ddnode_from_cache;
	}
    }
    

  // Hash Table MISS

  int u_value, u_index;
  DAGNode *u_AP;

  u_value = evaluateOpOnTerminals("and", v1, v2); // if both operands are constants, evaluate and return
  if(u_value != -1)
    {
      //{create terminal vertex}
      u = createDD(u_value);
    }

  else // create nonterminal and evaluate further down 
    {

      int v1_index, v2_index;
      v1_index = getIndex(v1);
      v2_index = getIndex(v2);
      DDNode *v1_low, *v1_high, *v2_low, *v2_high, *v_low1, *v_high1, *v_low2, *v_high2, *u_low, *u_high;
      v1_low = v1->getLeft();
      v1_high = v1->getRight();
      v2_low = v2->getLeft();
      v2_high = v2->getRight();

      u_index = findMin(v1_index, v2_index);
      u_AP = findAPFromIndex(u_index);
      Negated_u_AP = negateConstraint(u_AP); 

      if(v1_index == u_index)
        {
          v_low1 = v1_low;
          v_high1 = v1_high;
        }
      else
        {
          v_low1 = v1;
          v_high1 = v1;
        }

      if(v2_index == u_index)
        {
          v_low2 = v2_low;
          v_high2 = v2_high;
        }
      else
        {
          v_low2 = v2;
          v_high2 = v2;
        }

      // Now we have identified the top equation, and the next low and high
      // apply context based simplification

      if(callImplicationCheck(E, D, u_AP, Negated_u_AP, WidthTable)) // E/\D=>u_AP . Hence only the right matters
	{
        //showImplicationCheck(E, D, u_AP);
	 if(useful_context_finding)
		{
                if(trimming_context_mod_based)
			{
			trimContext(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_class_based)
			{
	        	trimContextAdvanced(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck);
			}
                else if(trimming_context_overapproximate)
			{
			trimContextEager(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_strategy_four)
			{
			trimContextStrategyFour(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
                else
			{
			cout<<"\nUnknown trimming mode\n"; exit(1);
			}
		}

	u = eliminateInconsistentPathsInternal(v_high1, v_high2, E, D, WidthTable, Useful_Context_For_Child1, PtrLocalHashTable, PtrLocalHashTable_Level_1);
	}
      else if(callImplicationCheck(E, D, Negated_u_AP, u_AP, WidthTable)) // E/\D=>not(u_AP) . Hence only the left matters
	{
	//showImplicationCheck(E, D, negateConstraint(u_AP));
        if(useful_context_finding)
		{
                if(trimming_context_mod_based)
			{
			trimContext(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_class_based)
			{
	        	trimContextAdvanced(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck);
			}
                else if(trimming_context_overapproximate)
			{
			trimContextEager(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_strategy_four)
			{
			trimContextStrategyFour(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
                else
			{
			cout<<"\nUnknown trimming mode\n"; exit(1);
			}
		}

	u = eliminateInconsistentPathsInternal(v_low1, v_low2, E, D, WidthTable, Useful_Context_For_Child2, PtrLocalHashTable, PtrLocalHashTable_Level_1);
	}
       else // try to evaluate
	{
	set<DAGNode*> E_new, D_new;
        copySet(E_new, E);
        E_new.insert(u_AP);
        copySet(D_new, D);
	D_new.insert(Negated_u_AP);

	if(cache_tree_on)
	{
	CacheTree.moveDown(u_AP);
  	}
      	u_high = eliminateInconsistentPathsInternal(v_high1, v_high2, E_new, D, WidthTable, Useful_Context_For_Child2, PtrLocalHashTable, PtrLocalHashTable_Level_1);
	if(cache_tree_on)
	{
	CacheTree.moveUp();
	}

	if(cache_tree_on)
	{
	CacheTree.moveDown(Negated_u_AP);
	}
	u_low = eliminateInconsistentPathsInternal(v_low1, v_low2, E, D_new, WidthTable, Useful_Context_For_Child1, PtrLocalHashTable, PtrLocalHashTable_Level_1);
	if(cache_tree_on)
	{
	CacheTree.moveUp();
	}

	
	u = createDD(u_AP, u_low, u_high);
	}
    }


    // code for finding useful context 
    // show v1,v2,useful context and u
    if(useful_context_finding)
	{
	  set<DAGNode*> temp1, temp2;

	set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin()));
	set_union(Useful_E_For_Impcheck.begin(), Useful_E_For_Impcheck.end(), Useful_D_For_Impcheck.begin(), Useful_D_For_Impcheck.end(),inserter(Useful_Context_For_Impcheck, Useful_Context_For_Impcheck.begin()));
	set_union(Useful_Context_For_Child1.begin(), Useful_Context_For_Child1.end(), Useful_Context_For_Child2.begin(), Useful_Context_For_Child2.end(),inserter(temp1, temp1.begin()));
        set_union(Useful_Context_For_Impcheck.begin(), Useful_Context_For_Impcheck.end(), temp1.begin(), temp1.end(),inserter(temp2, temp2.begin()));
        set_intersection(temp2.begin(), temp2.end(), Context.begin(), Context.end(),inserter(Useful_Context, Useful_Context.begin()));

    	//showUsefulpartInApplyWithImpeck(op, v1, v2, Useful_Context, u);
	}
   // code for finding useful context ends here

    if(level_1_local_hashing && Useful_Context.empty())
      {
	// Hash key is key_level_1
	PtrLocalHashTable_Level_1->insert(make_pair(key_level_1, u));
      }// if(level_1_local_hashing)
    else if(subset_based_local_hashing)
	{

	  set<DAGNode*> Temp_Useful_Context;
	  copySetNew(Temp_Useful_Context, Useful_Context);

	  insertIntoEfficientLocalHashTableBinaryOperation(PtrLocalHashTable, v1, v2, Temp_Useful_Context, u);
	}



      return u;
}


/* Functions added in the DDmanager to perform the transitive closure computation at the dd level itself */


// create and return a dd which is isomorphic to the argument dd such that 
// in place of each LME e of the argument dd, we need e with deltas replaced according to the map
DDNode*  DDManager::createShiftedDD(DDNode *dd, map<int, int> &DeltaMap, map<string, int> &WidthTable)
{
  insertPredicatesIntoAPMapIncreateShiftedDD(dd, DeltaMap, WidthTable); // insert the shifted predicates at proper locations in the APMap

  map<DDNode*, DDNode*> CreateShiftedDDTable;
  map<DDNode*, DDNode*> *PtrCreateShiftedDDTable = &CreateShiftedDDTable;

  DDNode* resultDD = createShiftedDDInternal(dd, DeltaMap, WidthTable, PtrCreateShiftedDDTable);
  return resultDD;
}


// Internal function called by DDManager::createShiftedDD to
// create and return shifted LMDD based on the DeltaMap
DDNode*  DDManager::createShiftedDDInternal(DDNode *dd, map<int, int> &DeltaMap, map<string, int> &WidthTable, map<DDNode*, DDNode*> *PtrCreateShiftedDDTable)
{
  map<DDNode*, DDNode*>::iterator it = PtrCreateShiftedDDTable->find(dd);

  if(it != PtrCreateShiftedDDTable->end())
    {
      return it->second;
    }

  if(dd->getPredicate() == NULL) // Terminal
    {
      return dd;
    }

  // First get the newly generated LME
  
  DAGNode *MyPredicate = dd->getPredicate();

  DAGNode *NewPredicate = createShiftedLMEBasedOnDeltaMap(MyPredicate, DeltaMap);

  NewPredicate = recreateEquationInProperForm(NewPredicate, WidthTable); // recreate in cannonical form

  //cout<<"\nNewPredicate = "<<getDAGStr(NewPredicate)<<", MyPredicate = "<<getDAGStr(MyPredicate)<<endl;

  //  Insert NewPredicate into proper place in the APMap if it is not already inserted 

  bool predicates_inserted = true;
  
  if(!predicates_inserted)
    {

      bool DynamicOrdering = false;
  
      if(DynamicOrdering)
	{
	  dynamicOrdering(MyPredicate, NewPredicate); // increment the indices of the predicates in APMap above MyPredicate by 1 and insert NewPredicate just on top of MyPredicate i.e. at MyPredicate's index + 1
	}
      else
	{
	  pushBackPredicateIntoAPMap(NewPredicate); // push back the new predicate as the last predicate of the APMap
	}
    }

  // Newly generated LME inserted into proper position in the APMap


  DDNode *NewRight = createShiftedDDInternal(dd->getRight(), DeltaMap, WidthTable, PtrCreateShiftedDDTable);
  DDNode *NewLeft = createShiftedDDInternal(dd->getLeft(), DeltaMap, WidthTable, PtrCreateShiftedDDTable);
  

  
  // Note that in DD nodes, left is else and right is then

  DDNode* then_dd = NewRight;
  DDNode* els_dd = NewLeft;

  DDNode* codn_dd;

  codn_dd = createDDFromDAGWithoutImplicationCheck(NewPredicate, WidthTable);

  DDNode* resultDD = createITEWithImplicationCheck(codn_dd, then_dd, els_dd, WidthTable);

  PtrCreateShiftedDDTable->insert(make_pair(dd, resultDD));

  return resultDD;
}

// Get the support of the LMDD rooted at "v" in "VarsPresent"
void DDManager::getDDNames(DDNode *v, set<string> &VarsPresent)
{
  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      // Already visited
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {
      // New DDNode encountered
      if(v->getPredicate()!=NULL) 
	{
	  // Non-leaf node
	  getDDNames(v->getLeft(), VarsPresent);
	  getDDNames(v->getRight(), VarsPresent);

	  DAGNode* My_Predicate = v->getPredicate();
	  set<string> VariablesFromMy_Predicate;
	  getDAGNames(My_Predicate, VariablesFromMy_Predicate);
      
	  set_union(VariablesFromMy_Predicate.begin(), VariablesFromMy_Predicate.end(), VarsPresent.begin(), VarsPresent.end(),inserter(VarsPresent, VarsPresent.begin()));
	}
      
      SetOfDDNodes.insert(v);
    }
}



// Finds the variables to be eliminated from an LMDD for iterative squaring computation.
// It is done by finding the support of the LMDD, and then finding the variables from them 
// such that their delta times are 1
void DDManager::getVarsToElimInGetTransitiveClosure(DDNode* dd, set<string> &VarsToElim, int DeltaToEliminate) 
{
  char temp[100];
  sprintf(temp,"%d",DeltaToEliminate);
  string DeltaToEliminate_Str(temp); 

  set<string> Names;

  SetOfDDNodes.clear(); // clear this first
  getDDNames(dd, Names); // find the set of variables in the LMEs in the dd

  // We need to quantify out all signals from Conjunct which end with _1
  for(set<string>::iterator it=Names.begin(); it!=Names.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function DDManager::getVarsToElimInGetTransitiveClosure\n";
		    exit(1);
		  }
      string id = id_delta.substr(0, index);
      string delta_str = id_delta.substr(index+1);	
      if(delta_str==DeltaToEliminate_Str)
	{
	  VarsToElim.insert(id_delta);
	}
    }

  cout<<"\nVarsToElim\n";
  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      cout<<*it<<endl;
    }
}
      

// Get the transitive closure for the given transitive relation "TR" 
// for the number of times in "squaring_times"
DAGNode* DDManager::getTransitiveClosure(DAGNode *TR, int squaring_times, map<string, int> &WidthTable, string infilename)
{

  DAGNode *PresentClosure_Dag = TR;

  checkForPropSkelton(PresentClosure_Dag);

  cout<<"\n\nDag's structure confirmed to be propositional\n\n";

  bool order_prefixing=true;

  list<DAGNode*> APList;

  if(order_prefixing)
    {

      cout<<"\nGetting the APs in the DAG\n";

      getAPsForDAGInPropSkelton(PresentClosure_Dag, APList);

      showAPList(APList);

      if(reorderingBasedOnSharing_Enabled) // push the LMEs which are getting shared with R(s1,s2) to lesser order
	{
          map<int, int> DeltaMapForFindingSharedLMEs;

      	  DeltaMapForFindingSharedLMEs.insert(make_pair(0, 1));
	  DeltaMapForFindingSharedLMEs.insert(make_pair(1, 2));
	          
	  reorderingBasedOnSharing(APList, WidthTable, DeltaMapForFindingSharedLMEs);

	  cout<<"\nAfter Reordering Based on Sharing\n";

	  showAPList(APList);
	}

       
      cout<<"\nInitializing the APMap\n";

      initializeAPMap(APList, WidthTable);

      cout<<"\nAPMap initialized\n";
    }

  


  bool signal_class_finding=true;// for finding equivalence classes of signals and thus lmes, lmds: helps in callImplicationCheck
  if(signal_class_finding)
    {
      findSignalClass(APList);
      //showSignalClass();
      createSignalEquivalenceClass();
      showSignalEquivalenceClass();
    }
  
  

  
  time_t initial_dd_creation_time, t2;
  time(&initial_dd_creation_time);

  

  DDNode* PresentClosure_DD;
  DDNode* InitialClosure_DD;
  
  
  if(simplification_mode==2)
    {
	PresentClosure_DD = createDDFromDAGWithImplicationCheck(PresentClosure_Dag, WidthTable);
    }
  else if(simplification_mode==1)
    {
      //cout<<"\nCalling createDDFromDAGWithoutImplicationCheck\n";
      PresentClosure_DD = createDDFromDAGWithoutImplicationCheck(PresentClosure_Dag, WidthTable);
      //cout<<"\nSize of dd = "<<getSizeOfDD(PresentClosure_DD)<<endl;
      //cout<<"\nPresentClosure_DD = "<<getDAGStr(createDAGFromDD(PresentClosure_DD))<<endl;

      PresentClosure_DD = eliminateInconsistentPaths(PresentClosure_DD, WidthTable);
      //cout<<"\nSize of dd after elimination of inconsistent paths = "<<getSizeOfDD(PresentClosure_DD)<<endl;
      //cout<<"\nPresentClosure_DD = "<<getDAGStr(createDAGFromDD(PresentClosure_DD))<<endl;
    }
  else if(simplification_mode==0)
    {
      //cout<<"\nCalling createDDFromDAGWithoutImplicationCheck\n";
      PresentClosure_DD = createDDFromDAGWithoutImplicationCheck(PresentClosure_Dag, WidthTable);
      //cout<<"\nSize of dd = "<<getSizeOfDD(PresentClosure_DD)<<endl;
      //cout<<"\nPresentClosure_DD = "<<getDAGStr(createDAGFromDD(PresentClosure_DD))<<endl;
    }
  else
    {
      cout<<"\nInvalid simplification mode\n";
      exit(1);
    }

  

  time(&t2);

  cout<<"\nInitial DD created in "<< (t2 - initial_dd_creation_time) <<" seconds"<<endl;

  cout<<"\nNumber of nodes in the initial DD = "<<getSizeOfDD(PresentClosure_DD)<<endl;

  //showAPMap(); 

  InitialClosure_DD = PresentClosure_DD;

  //int userchar = getchar();

  int initial_state_counter=0;


  DDNode *old_dd;
  DDNode *new_dd;

  for(int squaring_counter=0; squaring_counter<squaring_times; squaring_counter++)
    {

      cout<<"\nStarting the "<<squaring_counter+1<<" th squaring ...\n";
      cout<<"\nNumber of nodes in the PresentClosure_DD = "<<getSizeOfDD(PresentClosure_DD)<<endl;
      //cout<<"\nPresentClosure_DD = "<<getDAGStr(createDAGFromDD(PresentClosure_DD))<<endl;
      showPredicates(PresentClosure_DD, "PresentClosure_DD");
      showAPMap();
      cout<<"\nSize of APMAP = "<<APMap.size()<<endl;

      old_dd = PresentClosure_DD;
      

      map<int, int> DeltaMap;

      if(make_squarings_independent)
	{
	  DeltaMap.insert(make_pair(initial_state_counter, initial_state_counter+1));
	  DeltaMap.insert(make_pair(initial_state_counter+1, initial_state_counter+2));
	}
      else
	{
	  DeltaMap.insert(make_pair(0, 1));
	  DeltaMap.insert(make_pair(1, squaring_counter+2));
	}
  
      DDNode* PresentClosure_DD_Shifted = createShiftedDD(PresentClosure_DD, DeltaMap, WidthTable); // shifting
      cout<<"\nNumber of nodes in the PresentClosure_DD_Shifted = "<<getSizeOfDD(PresentClosure_DD_Shifted)<<endl;
      //cout<<"\nPresentClosure_DD_Shifted = "<<getDAGStr(createDAGFromDD(PresentClosure_DD_Shifted))<<endl;
      showPredicates(PresentClosure_DD_Shifted, "PresentClosure_DD_Shifted");
      showAPMap(); 
      cout<<"\nSize of APMAP = "<<APMap.size()<<endl;
     
      showCommonPredicates(PresentClosure_DD, PresentClosure_DD_Shifted, "PresentClosure_DD", "PresentClosure_DD_Shifted");
      

      DDNode* Conjunct;

      bool impcheck_compulsory=false;

      if(impcheck_compulsory)
	{
	 Conjunct = applyWithImplicationCheck("and", PresentClosure_DD, PresentClosure_DD_Shifted, WidthTable);
	}
      else // it is found that in some cases applyWithImplicationCheck creates dds with larger size
	{
	/*if(commonPredicatesExists(PresentClosure_DD, PresentClosure_DD_Shifted))
		{
		  Conjunct = applyWithImplicationCheck("and", PresentClosure_DD, PresentClosure_DD_Shifted, WidthTable);
		}
      	else
		{*/
		  Conjunct = apply("and", PresentClosure_DD, PresentClosure_DD_Shifted);
		  //showDD(Conjunct);
		  //cout<<"\nNumber of nodes in Conjunct = "<<getSizeOfDD(Conjunct)<<endl;
		  //cout<<"\nConjunct = "<<getDAGStr(createDAGFromDD(Conjunct))<<endl;
		  //Conjunct = eliminateInconsistentPaths(Conjunct, WidthTable);
		  //showDD(Conjunct);
		  //cout<<"\nNumber of nodes in Conjunct = "<<getSizeOfDD(Conjunct)<<endl;
		  //cout<<"\nConjunct = "<<getDAGStr(createDAGFromDD(Conjunct))<<endl;
	//	}
	}

      cout<<"\nNumber of nodes in Conjunct = "<<getSizeOfDD(Conjunct)<<endl;
      //cout<<"\nConjunct = "<<getDAGStr(createDAGFromDD(Conjunct))<<endl;
      showPredicates(Conjunct, "Conjunct");
      showAPMap(); 
      cout<<"\nSize of APMAP = "<<APMap.size()<<endl;

      set<string> VarSetToElim;

      if(make_squarings_independent)
	{
	  getVarsToElimInGetTransitiveClosure(Conjunct, VarSetToElim, initial_state_counter+1);
	}
      else
	{
	  getVarsToElimInGetTransitiveClosure(Conjunct, VarSetToElim, 1);
	}

      // Temporary code for the 4th squaring

      bool tempcode_fourth_squre=false;
      
      if(tempcode_fourth_squre)
	{

	  if(squaring_counter==3) // 4th squaring
	    {
	      trimVarSetToElim(VarSetToElim);
	    }
	}

     // Temporary code for 4th squaring ends here

      list<string> VarsToElim;
      for(set<string>::iterator it=VarSetToElim.begin(); it != VarSetToElim.end(); it++)
	{
	  string name = *it;
	  VarsToElim.push_back(name);
	}


      //userchar = getchar();

      time_t var_elim_start_time;
      time(&var_elim_start_time);

      DDNode* Conjunct_Eliminated;

      bool input_needed_in_smtfile_format=true;
      if(input_needed_in_smtfile_format)
	{
        cout<<"\nPrinting the conjunct in SMTLib format\n";
        printInputToQEliminatorInSMTLibFormatWithCount(createDAGFromDD(Conjunct), WidthTable, VarSetToElim, infilename);
	}


      if(strcmp(Elimination_Type, "-white") == 0)
	{
	Conjunct_Eliminated = MultiVarQElimWithStrategy(VarsToElim, Conjunct, WidthTable);
	}
      else if(strcmp(Elimination_Type, "-black") == 0)
	{
	Conjunct_Eliminated = MultiVarQElimWithNoDPNoStrategy(VarsToElim, Conjunct, WidthTable);
	}
      else
	{
	  cout<<"\nUnknown Elimination_Type "<<Elimination_Type<<endl;
	  exit(1);
	}
    
      time(&t2);
      cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (t2 - var_elim_start_time)<<" SECONDS "<<endl<<endl;

      cout<<"\nNumber of nodes in Conjunct_Eliminated = "<<getSizeOfDD(Conjunct_Eliminated)<<endl;

      bool premature_exit=false;
      if(premature_exit && squaring_counter+1==squaring_times) // This is the last iteration
	{
	break;
	}

      //cout<<"\nConjunct_Eliminated = "<<getDAGStr(createDAGFromDD(Conjunct_Eliminated))<<endl;
      //showAPMap(); 

      //userchar = getchar();

      DDNode*  Conjunct_Eliminated_Renamed;

      DeltaMap.clear();

      if(make_squarings_independent)
	{
	  DeltaMap.insert(make_pair(initial_state_counter, initial_state_counter+3));
	  DeltaMap.insert(make_pair(initial_state_counter+2, initial_state_counter+4));

          if(reorderingBasedOnSharing_Enabled)
          //if(false)
		{
	  	Conjunct_Eliminated_Renamed = createShiftedDD_With_ReorderingBasedOnSharing_Enabled(Conjunct_Eliminated, DeltaMap, WidthTable); // renaming such that the LMEs which will get shared with the next frame are pushed down
		}
	  else
		{
		Conjunct_Eliminated_Renamed = createShiftedDD(Conjunct_Eliminated, DeltaMap, WidthTable); // renaming
		}
	}
      else
	{
	  DeltaMap.insert(make_pair(squaring_counter+2, 1));
	  Conjunct_Eliminated_Renamed = createShiftedDD(Conjunct_Eliminated, DeltaMap, WidthTable); // renaming
	}


      cout<<"\nNumber of nodes in Conjunct_Eliminated_Renamed = "<<getSizeOfDD(Conjunct_Eliminated_Renamed)<<endl;
      //cout<<"\nConjunct_Eliminated_Renamed = "<<getDAGStr(createDAGFromDD(Conjunct_Eliminated_Renamed))<<endl;
      //showAPMap(); 

      if(make_squarings_independent)
	{
	  // Here the Conjunct_Eliminated_Renamed is in s_(initial_state_counter+3) and s_(initial_state_counter+4). However, InitialClosure_DD is still in s_0 and s_1. Hence create InitialClosure_DD_Renamed which is InitialClosure_DD in s_(initial_state_counter+3) and s_(initial_state_counter+4) and then disjunct with Conjunct_Eliminated_Renamed
	  DeltaMap.clear();
	  DeltaMap.insert(make_pair(0, initial_state_counter+3));
	  DeltaMap.insert(make_pair(1, initial_state_counter+4));

	  DDNode*  InitialClosure_DD_Renamed;
	
	  if(reorderingBasedOnSharing_Enabled)
          //if(false)
		{
	  	InitialClosure_DD_Renamed = createShiftedDD_With_ReorderingBasedOnSharing_Enabled(InitialClosure_DD, DeltaMap, WidthTable); // renaming such that the LMEs which will get shared with the next frame are pushed down
		}	  
	  else
	  	{
	  	InitialClosure_DD_Renamed = createShiftedDD(InitialClosure_DD, DeltaMap, WidthTable); // renaming
		}

	  PresentClosure_DD  = applyWithImplicationCheck("or", Conjunct_Eliminated_Renamed, InitialClosure_DD_Renamed, WidthTable); // disjuncting with R(s0, s1)
	}
      else
	{
	  PresentClosure_DD  = applyWithImplicationCheck("or", Conjunct_Eliminated_Renamed, InitialClosure_DD, WidthTable); // disjuncting with R(s0, s1)
	}
      cout<<"\nNumber of nodes in PresentClosure_DD = "<<getSizeOfDD(PresentClosure_DD)<<endl;
      //cout<<"\nPresentClosure_DD = "<<getDAGStr(createDAGFromDD(PresentClosure_DD))<<endl;
      //showAPMap(); 

      new_dd = PresentClosure_DD;

      bool fixpoint_check=false;

      if(fixpoint_check)
	{
	  if(fixpointReached(old_dd, new_dd, WidthTable))
	    {
	      break;
	    }
	}

      initial_state_counter = initial_state_counter+3;

    }

  cout<<"\nSquarings finished\n";

  DAGNode* Result_Dag = createDAGFromDD(PresentClosure_DD);
  return Result_Dag;

}

// Push back a new LME "NewPredicate" into the APMap
void DDManager::pushBackPredicateIntoAPMap(DAGNode *NewPredicate)
{
  if(APMap.find(NewPredicate)!=APMap.end()) // entry already exists in APMap for the NewPredicate : Do nothing
	{
	//cout<<"\nEntry already exists in APMap for the predicate node "<<NewPredicate<<"\n";
	return;
	}
 
  if(order_lmes_before_lmis)
    {
                
        count++;

        int InsertLocation;

        // if LMI
        if(NewPredicate->Get_node_name() =="bvule" || NewPredicate->Get_node_name() =="bvuge" || NewPredicate->Get_node_name() =="bvult"  || NewPredicate->Get_node_name() =="bvugt")
            {                
            InsertLocation = count + LMI_OFFSET;                
            }
        // if LME
        else if(NewPredicate->Get_node_name() =="=" || NewPredicate->Get_node_name() =="equivalent")
            {
            InsertLocation = count;
            }
        else
            {
            cout<<"\nUnknown predicate type "<<NewPredicate->Get_node_name()<<" in function DDManager :: pushBackPredicateIntoAPMap\n";
            exit(1);
            }
        // Insert into APMap

        if(InsertLocation>MAX_AP) 
                {
                cout<<"\nIn function  DDManager :: pushBackPredicateIntoAPMap, count has exceeded MAX_AP. Increase MAX_AP\n";
                exit(1);
                }
        if(checkIfIndexExists(InsertLocation)) // an entry exists in APMap with the same index : ERROR
                {
                cout<<"\nIn DDManager::pushBackPredicateIntoAPMap, an entry exists in APMap with the given index "<<InsertLocation<<endl;
                exit(1);
                }            
        APMap.insert((pair<DAGNode*,int>(NewPredicate, InsertLocation)));
    }// if(order_lmes_before_lmis)
  else
  {
      count++;

      if(count>MAX_AP) 

        {cout<<"\nIn function  DDManager :: pushBackPredicateIntoAPMap, count has exceeded MAX_AP. Increase MAX_AP\n";
        exit(1);
        }

      if(checkIfIndexExists(count)) // an entry exists in APMap with the same index : ERROR
        {
          cout<<"\nIn DDManager::pushBackPredicateIntoAPMap, an entry exists in APMap with the given index "<<count<<endl;
          exit(1);
        }


      APMap.insert((pair<DAGNode*,int>(NewPredicate,count)));
  }//else of if(order_lmes_before_lmis)
}//function


// Get the set of LMEs in a LMDD
void DDManager::getPredicates(DDNode *v, set<DAGNode*> &PredicatesPresent)
{
  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      // Already visited
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {
      // New DDNode encountered
      if(v->getPredicate()!=NULL) 
	{
	  // Non-leaf node
	  getPredicates(v->getLeft(), PredicatesPresent);
	  getPredicates(v->getRight(), PredicatesPresent);

	  DAGNode* My_Predicate = v->getPredicate();
	  PredicatesPresent.insert(My_Predicate);
	}
      
      SetOfDDNodes.insert(v);
    }
}


// Let we want to create a shifted clone of LMDD rooted at "dd" according to DeltaMap, 
// using the function DDManager::createShiftedDD. Then we need to insert the shifted 
// predicates of "dd" in proper places in APMap to get smaller LMDD sizes. This function
// achieves this.
void DDManager::insertPredicatesIntoAPMapIncreateShiftedDD(DDNode *dd, map<int, int> &DeltaMap, map<string, int> &WidthTable)
{
  // get the predicates in dd 
  SetOfDDNodes.clear();
  set<DAGNode*> PredicatesPresent;
  getPredicates(dd, PredicatesPresent); // find the predicates
  
  map<int, DAGNode*> LocalPredicateMap;
  
  for(set<DAGNode*>::iterator it=PredicatesPresent.begin(); it!=PredicatesPresent.end(); it++)
    {
      int index;

      DAGNode* pred = *it;
      
      if(APMap.find(pred)==APMap.end()) // no entry in APMap for the DAGNode* pred : ERROR
		{
		cout<<"\nNo entry for APMap for the predicate node "<<pred<<"in function DDManager::insertPredicatesIntoAPMapIncreateShiftedDD\n";
		exit(1);
		}

      index = APMap[pred];

      DAGNode* shifted_pred;

      shifted_pred = createShiftedLMEBasedOnDeltaMap(pred, DeltaMap);

      shifted_pred = recreateEquationInProperForm(shifted_pred, WidthTable); // recreate in cannonical form
      
      LocalPredicateMap.insert(make_pair(index, shifted_pred));
    }

  // LocalPredicateMap is sorted according to the index of the original predicate pred
  // Insert the newly created predicates one by one according to the order suggested by the LocalPredicateMap

  for(map<int, DAGNode*>::iterator it=LocalPredicateMap.begin(); it!=LocalPredicateMap.end(); it++)
    {
      DAGNode* shifted_pred = it->second;

      pushBackPredicateIntoAPMap(shifted_pred);
    }
}


/* Functions related to Implication Checking */

// Display the implication check /\E/\D => u_AP on a file
void DDManager::showImplicationCheck(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP)
{
static ULINT ShowImplicationCount=0;

 DAGNode *root = NULL;
 set<DAGNode*>::iterator it=E.begin();
 for(; it!=E.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
 it=D.begin();
 for(; it!=D.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

 //cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
 ShowImplicationCount++;
 char temp[100];
 sprintf(temp, "%llu", ShowImplicationCount);
 string temp_str(temp);
 string string_to_show = temp_str;
 string_to_show += "  ";

 sprintf(temp, "%llu", Calls_For_ImpCheck);
 temp_str = temp;
 string_to_show += temp_str;
 string_to_show += "  ";
 
 if(root!=NULL)
   {

     string_to_show += getDAGStr(root);
     string_to_show += " => ";
     string_to_show += getDAGStr(u_AP);
     fprintf(ImpCheckFile, "\n%s\n", string_to_show.c_str());
   }
 else
   {
     fprintf(ImpCheckFile, "\nEmpty antecedent\n");
   }
 //cout<<endl<<string_to_show<<endl;
}


// function to call yices with time recording
void DDManager::callYicesForImplicationCheck()
{
 time_t solver_start, solver_end, solver_duration;
 time(&solver_start);
system("yices -tc thunsat_impcheck.ys > result_thunsat_impcheck.txt");
 time(&solver_end);
 solver_duration = solver_end-solver_start;
 SMTSolver_Time_In_ImplicationCheck = SMTSolver_Time_In_ImplicationCheck + solver_duration;
}


// function to call STP with time recording
void DDManager::callSTPForImplicationCheck()
{
 time_t solver_start, solver_end, solver_duration;
 time(&solver_start);
  system("stp --SMTLIB1 thunsat_impcheck.smt > result_thunsat_impcheck.txt"); //call to STP
 time(&solver_end);
 solver_duration = solver_end-solver_start;
 SMTSolver_Time_In_ImplicationCheck = SMTSolver_Time_In_ImplicationCheck + solver_duration;
}



// check if E/\D/\u_AP is unsat using STP/yices
bool DDManager::implicationCheck(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP, map<string, int> &WidthTable)
{
bool ret_value;

 DAGNode *root = NULL;
 set<DAGNode*>::iterator it=E.begin();
 for(; it!=E.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
 it=D.begin();
 for(; it!=D.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

 if(root==NULL) 
{
 cout<<"\nWarning!!Context empty in function DDManager::implicationCheck. Empty context is interpreted as true context. Hence, result depends on consequent\n"; 
 //return false;
}

 if(root==NULL)
   {
     root = u_AP;
   }
 else
   {
     root = createDAG("and", root, u_AP, LOGICAL_SYMBOL, 1);
   }


 if(internal_solver=="yices")
   {

     FILE *smt_file = fopen("thunsat_impcheck.ys", "w");

     if(smt_file == NULL)
       {cout<<"\nCannot create thunsat_impcheck.ys\n"; exit(1);}

     writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

     string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(root, WidthTable);


     ///cout<<"\nOriginal dag = "<<getDAGStr(root)<<endl;
     //cout<<"\nOriginal dag in Yices = "<<dag_str<<endl;
  
     fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
     fprintf(smt_file, "\n(check)\n");
     fclose(smt_file);

     //system("yices -tc thunsat_impcheck.ys > result_thunsat_impcheck.txt");
     callYicesForImplicationCheck();
   }
 
 else  if(internal_solver=="boolector")
   {

     FILE *smt_file = fopen("thunsat_impcheck.smt", "w");

     if(smt_file == NULL)
       {cout<<"\nCannot create thunsat_impcheck.smt\n"; exit(1);}

     fprintf(smt_file,"(benchmark test.smt\n:logic QF_BV\n");

     writeBVDeclarationsInSMTLibFormat(smt_file, WidthTable);

     string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);


     ///cout<<"\nOriginal dag = "<<getDAGStr(root)<<endl;
     //cout<<"\nOriginal dag in Yices = "<<dag_str<<endl;
  
     fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
     fprintf(smt_file, "\n)\n");
     fclose(smt_file);

     //system("yices -tc thunsat_impcheck.ys > result_thunsat_impcheck.txt");
     callBoolectorForImplicationCheck();
   }

 else  if(internal_solver=="stp")
   {

     FILE *smt_file = fopen("thunsat_impcheck.smt", "w");

     if(smt_file == NULL)
       {cout<<"\nCannot create thunsat_impcheck.smt\n"; exit(1);}

     fprintf(smt_file,"(benchmark test.smt\n");

     writeBVDeclarationsInSMTLibFormat(smt_file, WidthTable);

     string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);


     ///cout<<"\nOriginal dag = "<<getDAGStr(root)<<endl;
     //cout<<"\nOriginal dag in Yices = "<<dag_str<<endl;
  
     fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
     fprintf(smt_file, "\n)\n");
     fclose(smt_file);

     //system("yices -tc thunsat_impcheck.ys > result_thunsat_impcheck.txt");
     callSTPForImplicationCheck();
   }

 FILE *result_smt_file = fopen("result_thunsat_impcheck.txt", "r");

 if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_impcheck.txt\n"; exit(1);}

 char result[100];
 fscanf(result_smt_file, "%s", result);
 fclose(result_smt_file);
 string result_str(result);

 if(result_str=="unsat") ret_value = true;
 else if(result_str=="sat") ret_value = false;
 else
   {
     cout<<"\nUnknown returned from SMT Solver in function  DDManager::implicationCheck\n"; exit(1);
   }

return ret_value;
}

// returns true if Set1 is subset of Set2; false otherwise
// Set1 is subset of Set2 <=> (Set1 \ Set2 = empty set)
bool DDManager :: isSubSet(set<DAGNode*> &Set1, set<DAGNode*> &Set2)
{
 set<DAGNode*> DiffSet;
 set_difference(Set1.begin(), Set1.end(), Set2.begin(), Set2.end(),inserter(DiffSet, DiffSet.begin()));
 if(DiffSet.empty()) return true;
 return false;
}


// check if E/\D => u_AP is valid using a layered scheme
// Layers include: Hashing, Overlap checking, Simulation, QE, and SMT-Solver call
bool DDManager::callImplicationCheck(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP, DAGNode* Negated_u_AP, map<string, int> &WidthTable)
{
  if(E.empty() && D.empty()) return false; // Nothing in context. Hence E/\D => u_AP is not valid. return false

  Calls_For_ImpCheck++;

  // show E/\D => u_AP, key
  //showImplicationCheck(E, D, u_AP);
  //showContextOfCacheTree();
  //checkContext(E, D); // check if the context E/\D matches with the context maintained through CacheTree

  // Convert E/\D => u_AP is valid  to E/\D/\~u_AP is unsat

  // check if an entry exists in cache for E/\D/\~u_AP

  bool caching = false;
  bool tree_based_caching=true; 
  bool cross_checking_tree_based_caching=false;

  /*
    set<DAGNode*> key;

    if(caching)
    {

    // form the key
     
    for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    {
    //int local_key = getKeyFromLME(*it);
    //key.insert(local_key);
    key.insert(*it);
    }

    for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    {
    //int local_key = getKeyFromLMD(*it);
    //key.insert(local_key);
    key.insert(*it);
    }
    //int local_key = getKeyFromConstraint(Negated_u_AP);
    //key.insert(local_key);
    key.insert(Negated_u_AP);

    // fprintf(ImpCheckFile, "\n");
    // for(set<DAGNode*>::iterator it=key.begin();it!=key.end();it++)
    // 	{
    // 	fprintf(ImpCheckFile, "%x ", *it);
    // 	}
    // fprintf(ImpCheckFile, "\n");


    // Check if an entry exists with key
    int ret_value = checkInImplicationTableModified(key);
    if(ret_value==1) 
    {
    fprintf(ImpCheckFile, "\nCache Hit. true returned\n");
    Cache_Hits_For_ImpCheck++;
    return true;
    }
    if(ret_value==0) 
    { 
    fprintf(ImpCheckFile, "\nCache Hit. false returned\n");
    Cache_Hits_For_ImpCheck++;
    return false;
    }
 
    fprintf(ImpCheckFile, "\nCache Miss\n");
    Cache_Misses_For_ImpCheck++;
    } */

  string key="";

  if(caching)
    {

      if(!tree_based_caching){ // HT based hashing
	// form the key   
   	string temp_key;
   	char temp_char[50];    
 
     	for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
	  {
	    sprintf(temp_char , "%x", *it);
	    temp_key = temp_char;
	    key += temp_key;  // all equations in the context
	  }

     	for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
	  {
	    sprintf(temp_char , "%x", *it);
	    temp_key = temp_char;
	    key += temp_key;  // all disequations in the context
	  }

     	sprintf(temp_char , "%x", Negated_u_AP);
     	temp_key = temp_char;
     	key += temp_key;  // Negated_u_AP in the context

     	// Hash key created
     	HTStatusValue<bool> result = FirstImplicationTable.hashtable_search(key);
     	if(result.success())
	  {
	    //fprintf(ImpCheckFile, "\nCache Hit\n");
	    //cout<<"\nCache Hit\n";
	    Cache_Hits_For_ImpCheck++;
	    return result.GetValue();
	  }

     	//fprintf(ImpCheckFile, "\nCache Miss\n");
     	//cout<<"\nCache Miss\n";
     	Cache_Misses_For_ImpCheck++;
      }
      else // tree_based_caching
	{
	  char result_from_cache = CacheTree.decide(Negated_u_AP); 
	  if(result_from_cache == 'u')
	    {
	      Cache_Hits_For_ImpCheck++;
	      //fprintf(ImpCheckFile, "\nCache Hit\n");
		
	      // For cross-checking
	      if(cross_checking_tree_based_caching)
		{
		  if(implicationCheck(E, D, Negated_u_AP, WidthTable))
		    {
		      fprintf(ImpCheckFile,"\ncaching is correct\n");	
		    }
		  else
		    {
		      fprintf(ImpCheckFile,"\ncaching is incorrect\n");	
		      exit(1);
		    }
		}
	      // cross-checking code ends here
		
	      return true;
	    }
	  else if(result_from_cache == 's')
	    {
	      Cache_Hits_For_ImpCheck++;
	      //fprintf(ImpCheckFile, "\nCache Hit\n");

	      // For cross-checking
	      if(cross_checking_tree_based_caching)
		{
		  if(!implicationCheck(E, D, Negated_u_AP, WidthTable))
		    {
		      fprintf(ImpCheckFile,"\ncaching is correct\n");	
		    }
		  else
		    {
		      fprintf(ImpCheckFile,"\ncaching is incorrect\n");	
		      exit(1);
		    }
		}
	      // cross-checking code ends here
		
	      return false;
	    }
	  Cache_Misses_For_ImpCheck++;
   	}
    }// caching
	

  bool SolvingByOverlapChecking=true;
  bool cross_checking_overlapping_solver=false;
  bool SolvingByExQuantifierElim=true;
  bool cross_checking_qelim_solver=false;
  bool ret_value;
  char ret_value_in_char='x';
  bool show_implication_check=false;
  bool second_level_hashing=true;
  bool SolvingBySimulation=false;
  bool cross_checking_simulation_solver=false;

  if(SolvingByOverlapChecking)
    {
      ret_value_in_char = implicationCheckByOverlapChecking(E, D, Negated_u_AP);

      if(ret_value_in_char == 's')
	{
	  //fprintf(ImpCheckFile,"\nsat from overlap checker\n");
	  // For cross-checking
	  if(cross_checking_overlapping_solver)
	    {
	      if(!implicationCheck(E, D, Negated_u_AP, WidthTable))
		{
		  fprintf(ImpCheckFile,"\nex.solver = solver\n");	
		}
	      else
		{
		  fprintf(ImpCheckFile,"\nex.solver <> solver\n");	
		  exit(1);
		}
	    }
	  // cross-checking code ends here
	  ret_value = false;
	  Calls_For_ImpCheck_By_Overlap_Checking++;
	}
    }


  if(show_implication_check && ret_value_in_char=='x')
    {
      Show_ImpCheck_Calls++;
      showImplicationCheck(E, D, u_AP);
      
      //printImplicationCheckInSMTFile(E, D, u_AP, Show_ImpCheck_Calls, WidthTable);
    }

  if(ret_value_in_char=='x')
    {
      //fprintf(ImpCheckFile,"\nunknown from overlap checker\n");

      set<DAGNode*> Trimmed_E; 
      set<DAGNode*> Trimmed_D;
      Trimmed_E.clear();
      Trimmed_D.clear();
      if(trimming_context_mod_based)
	{
	  trimContext(E, D, u_AP, Trimmed_E, Trimmed_D, WidthTable); // trim down the context by removing unneeded LMEs and LMDs thus creating Trimmed_E and Trimmed_D using modulus based LME/LMD dropping
	}
      else if(trimming_context_class_based)
	{
	  trimContextAdvanced(E, D, u_AP, Trimmed_E, Trimmed_D); // trim down the context by removing unneeded LMEs and LMDs thus creating Trimmed_E and Trimmed_D using class based LME/LMD dropping : more effective and cheap
	}
      else if(trimming_context_overapproximate)
	{
	  trimContextEager(E, D, u_AP, Trimmed_E, Trimmed_D, WidthTable); // trim down the context by keeping only a single lme/lmd in the trimmed context. Note that this is over-approximate
	}
      else if(trimming_context_strategy_four)
	{
	  trimContextStrategyFour(E, D, u_AP, Trimmed_E, Trimmed_D, WidthTable);
	}
      else
	{
	  set_union(Trimmed_E.begin(), Trimmed_E.end(), E.begin(), E.end(),inserter(Trimmed_E, Trimmed_E.begin()));
	  set_union(Trimmed_D.begin(), Trimmed_D.end(), D.begin(), D.end(),inserter(Trimmed_D, Trimmed_D.begin()));
	}


      string second_level_key="";
      bool second_level_cache_hit=false;

      if(second_level_hashing) // second-level hashing is HT based
	{ 
	  // form the key   
	  string temp_key;
	  char temp_char[50];    
 
	  for(set<DAGNode*>::iterator it=Trimmed_E.begin(); it!=Trimmed_E.end(); it++)
	    {
	      sprintf(temp_char , "%x", *it);
	      temp_key = temp_char;
	      second_level_key += temp_key;  // all equations in the trimmed context
	    }

	  for(set<DAGNode*>::iterator it=Trimmed_D.begin(); it!=Trimmed_D.end(); it++)
	    {
	      sprintf(temp_char , "%x", *it);
	      temp_key = temp_char;
	      second_level_key += temp_key;  // all disequations in the trimmed context
	    }

	  sprintf(temp_char , "%x", Negated_u_AP);
	  temp_key = temp_char;
	  second_level_key += temp_key;  // Negated_u_AP

	  // Hash key created
	  HTStatusValue<bool> result = SecondLevelImplicationTable.hashtable_search(second_level_key);
	  if(result.success())
	    {
	      //fprintf(ImpCheckFile, "\nCache Hit\n");
	      //cout<<"\nCache Hit\n";
	      Second_Level_Cache_Hits_For_ImpCheck++;
	      second_level_cache_hit=true;
	      ret_value = result.GetValue();
	    }
	  else
	    {
	      //fprintf(ImpCheckFile, "\nCache Miss\n");
	      //cout<<"\nCache Miss\n";
	      Second_Level_Cache_Misses_For_ImpCheck++;
	    }
	} // second-level hashing ends here


      if(!second_level_cache_hit) 
	{

	  // code to show the implication check
	  //showImplicationCheck(E, D, u_AP);
	  //showImplicationCheck(Trimmed_E, Trimmed_D, u_AP);
          // code to find the maximum size of context relevant for implication check
          ULINT context_size_imp_check = Trimmed_E.size()+Trimmed_D.size();
	  if(context_size_imp_check > max_context_size_imp_check)
		{
                //cout<<"\nContext useful for implication check\n******************\n";
		max_context_size_imp_check = context_size_imp_check;
		//showLMEAndLMDContext(Trimmed_E, Trimmed_D);
		}
	  // code to show the implication check ends

	  if(SolvingBySimulation)
	    {
	      ret_value_in_char = implicationCheckBySimulation(Trimmed_E, Trimmed_D, Negated_u_AP, WidthTable);
	      if(ret_value_in_char == 'u')
		{
		  //fprintf(ImpCheckFile,"\nsimulation solver returned unsat\n");
			
		  // For cross-checking
		  if(cross_checking_simulation_solver)
		    {
		      if(implicationCheck(E, D, Negated_u_AP, WidthTable)) // better to cross-check with original context
			{
			  fprintf(ImpCheckFile,"\nsimulation solver = solver\n");	
			}
		      else
			{
			  fprintf(ImpCheckFile,"\nsimulation solver <> solver\n");	
			  exit(1);
			}
		    }
		  // cross-checking code ends here
		  Successful_ImpCheck_Calls++;
		  ret_value = true;
		  Calls_For_ImpCheck_By_Simulator++;
		}
	      else if(ret_value_in_char == 's')
		{
		  //fprintf(ImpCheckFile,"\nsimulation solver returned sat\n");
 
		  // For cross-checking
		  if(cross_checking_simulation_solver)
		    {
		      if(!implicationCheck(E, D, Negated_u_AP, WidthTable)) // better to cross-check with original context
			{
			  fprintf(ImpCheckFile,"\nsimulation solver = solver\n");	
			}
		      else
			{
			  fprintf(ImpCheckFile,"\nsimulation solver <> solver\n");	
			  exit(1);
			}
		    }
		  // cross-checking code ends here
		  Failed_ImpCheck_Calls++;
		  ret_value = false;
		  Calls_For_ImpCheck_By_Simulator++;
		}
	      else if(ret_value_in_char == 'x')
		{
		  //fprintf(ImpCheckFile,"\nsimulation solver returned unknown\n");
		}
	      else
		{
		  fprintf(ImpCheckFile, "\nUndefined result from simulation solver\n");
		  exit(1);
		}
        
	    }// solving by simulation ends here

	  // only if ret_value_in_char == 'x' call ex. quantifier based solver

	  if(ret_value_in_char == 'x')
	    {

	      if(SolvingByExQuantifierElim)
		{
		  ret_value_in_char = implicationCheckByExQuantifierElimination(Trimmed_E, Trimmed_D, Negated_u_AP, WidthTable);
		  if(ret_value_in_char == 'u')
		    {
		      //fprintf(ImpCheckFile,"\nex.solver returned unsat\n");
			
		      // For cross-checking
		      if(cross_checking_qelim_solver)
			{
			  if(implicationCheck(E, D, Negated_u_AP, WidthTable)) // better to cross-check with original context
			    {
			      fprintf(ImpCheckFile,"\nex.solver = solver\n");	
			    }
			  else
			    {
			      fprintf(ImpCheckFile,"\nex.solver <> solver\n");	
			      exit(1);
			    }
			}
		      // cross-checking code ends here
		      Successful_ImpCheck_Calls++;
		      ret_value = true;
		      Calls_For_ImpCheck_By_Exsolver++;
		    }
		  else if(ret_value_in_char == 's')
		    {
		      //fprintf(ImpCheckFile,"\nex.solver returned sat\n");
 
		      // For cross-checking
		      if(cross_checking_qelim_solver)
			{
			  if(!implicationCheck(E, D, Negated_u_AP, WidthTable)) // better to cross-check with original context
			    {
			      fprintf(ImpCheckFile,"\nex.solver = solver\n");	
			    }
			  else
			    {
			      fprintf(ImpCheckFile,"\nex.solver <> solver\n");	
			      exit(1);
			    }
			}
		      // cross-checking code ends here
		      Failed_ImpCheck_Calls++;
		      ret_value = false;
		      Calls_For_ImpCheck_By_Exsolver++;
		    }
		  else if(ret_value_in_char == 'x')
		    {
		      //fprintf(ImpCheckFile,"\nex.solver returned unknown\n");
		    }
		  else
		    {
		      fprintf(ImpCheckFile, "\nUndefined result from ex.solver\n");
		      exit(1);
		    }
        
		}// solving by ex.quant. elimination ends here

	  

	      if(ret_value_in_char == 'x')
		{
		  //fprintf(ImpCheckFile,"\nsolver called\n");
		  ret_value = implicationCheck(Trimmed_E, Trimmed_D, Negated_u_AP, WidthTable);
		  if(ret_value) // successful call
		    {
		      Successful_ImpCheck_Calls++;
		    }
		  else
		    {
		      Failed_ImpCheck_Calls++;
		    }
		  Calls_For_ImpCheck_By_SMTsolver++;
        	}// solving by smt solver ends here i.e. unknown from ex.solver

	    }// if(ret_value_in_char == 'x') ends here i.e. unknown from simulation solver

	 
	  if(second_level_hashing)
	    { // HT based hashing
	      HTStatusValue<bool> result = SecondLevelImplicationTable.hashtable_insert(second_level_key, ret_value);
	      if(!result.success())
		{ cout<<"\nHash Insert Failure Inside Function DDManager::callImplicationCheck\n";
		exit(1);
		}

	      if(ret_value)
		{
		  //fprintf(ImpCheckFile, "\nResult=unsat\n");
		  //showImplicationCheck(Trimmed_E, Trimmed_D, u_AP);
		  //fprintf(ImpCheckFile, "  Result=unsat\n");
		  //showImplicationCheckModified(Trimmed_E, Trimmed_D, u_AP);
		}
	      else
		{
		  //showImplicationCheck(Trimmed_E, Trimmed_D, u_AP);
		  //fprintf(ImpCheckFile, "  Result=sat\n");
		  //fprintf(ImpCheckFile, "\nResult=sat\n");
		}
	    }

	}//second-level cache miss ends here i.e. !second_level_cache_hit
 

    }// unknown from overlap checker

  if(caching)
    {
      if(!tree_based_caching)
	{ // HT based hashing
	  HTStatusValue<bool> result = FirstImplicationTable.hashtable_insert(key, ret_value);
	  if(!result.success())
	    { cout<<"\nHash Insert Failure Inside Function DDManager::callImplicationCheck\n";
	    exit(1);
	    }

	  if(ret_value)
	    {
	      //fprintf(ImpCheckFile, "\nResult=unsat\n");
	    }
	  else
	    {
	      //fprintf(ImpCheckFile, "\nResult=sat\n");
	    }
	}
      else // tree_based_caching
	{
	  CacheTree.moveDown(Negated_u_AP);
	  DAGNode *Terminal;
	  if(ret_value)
	    {
	      Terminal = createLogicalConstant("true");
	    }
	  else
	    {
	      Terminal = createLogicalConstant("false");
	    }
	  CacheTree.moveDown(Terminal);
	  CacheTree.moveUp();
	  CacheTree.moveUp();
	}
 	
    }

  //showImplicationTable();
  
  return ret_value;
}

// Print the APMap in a file "apmap.txt"
void DDManager::printAPMap()
{
  FILE* APMapFile = fopen("apmap.txt", "w");
  if(APMapFile == NULL)
    {
      cout<<"\nCould not create apmap.txt\n";
      exit(1);
    }

  fprintf(APMapFile, "\nAPMap\n");
  map<DAGNode*, int>::iterator APMap_it = APMap.begin();
  for(; APMap_it != APMap.end(); APMap_it++)
    {
      if(APMap_it->first != NULL)
        {    
        fprintf(APMapFile, "\n%s\t%d\n", getDAGStr(APMap_it->first).c_str(), APMap_it->second);
        }
    }
  fclose(APMapFile);
}

// Print the status of the implication check cache
void DDManager::ImplicationCheckCacheStatus()
{
  fprintf(ImpCheckFile, "\n\nNo: of implication checks = %llu\nNo: of cache hits = %llu\nNo: of cache misses = %llu\nNo: of misses handled by overlap checker = %llu\nNo: of second-level cache hits = %llu\nNo: of second-level cache misses = %llu\nNo: of misses handled by simulation solver = %llu\nNo: of misses handled by ex. solver = %llu\nNo: of misses handled by SMT solver = %llu\nTime spent by Simulator = %d secs\nTime spent by Ex. Solver = %d secs\nTime spent by SMT Solver = %d secs\nNo: of successful missed calls = %llu\nNo: of failed missed calls = %llu\nNo: of calls to applywithimpcheck = %llu\nNo: of trivial hits in applywithimpcheck cache=%llu\nNo: of non-trivial hits in applywithimpcheck cache=%llu\nNo: of hits in applywithimpcheck local cache=%llu\nNo: of calls to applyiteimpcheck = %llu\nNo: of hits in applyiteimpcheck local cache=%llu\nMax. size of applystep cache = %llu\nMax. size of applyite cache = %llu\nMaximum context size in applystepwithimpcheck=%llu\nMaximum context size in applyitewithimpcheck=%llu\nTime spent for searching local cache of applystepwithimpcheck = %d secs\nTime spent for searching local cache of applyitewithimpcheck = %d secs\nNo: of trivial hits in applystepwithimpcheck local cache=%llu\nNo: of trivial hits in applyitewithimpcheck cache=%llu\nNo: of hits in applystepwithimpcheck level-I local cache=%llu\nNo: of hits in applyitewithimpcheck level-I local cache=%llu\nNo: of hits in eliminate inconsistent paths level-I local cache=%llu\nMaximum context size useful for implication check =%llu\nHits in external cache of apply with implication check=%llu\nMisses from external cache of apply with implication check=%llu\nHits in external cache of createite with implication check=%llu\nMisses from external cache of createite with implication check=%llu\nMisses of applystep with implication check=%llu\nMisses of applyite with implication check=%llu\nEffect of reduce=%llu\nNumber of calls to strategy 4 for trimming = %llu\nNumber of calls to strategy 4 for trimming which are trimmed effectively by level-2 = %llu\nTime spend in trimming = %d\nNumber of calls to ThUnsat = %llu\n", Calls_For_ImpCheck, Cache_Hits_For_ImpCheck, Cache_Misses_For_ImpCheck, Calls_For_ImpCheck_By_Overlap_Checking, Second_Level_Cache_Hits_For_ImpCheck, Second_Level_Cache_Misses_For_ImpCheck, Calls_For_ImpCheck_By_Simulator, Calls_For_ImpCheck_By_Exsolver, Calls_For_ImpCheck_By_SMTsolver, Simulator_Time_In_ImplicationCheck, ExSolver_Time_In_ImplicationCheck, SMTSolver_Time_In_ImplicationCheck, Successful_ImpCheck_Calls, Failed_ImpCheck_Calls, applystepwithimpcheck_calls, applystepwithimpcheck_cache_trivial_hit, applystepwithimpcheck_cache_non_trivial_hit, applystepwithimpcheck_local_cache_hit, applyitewithimpcheck_calls, applyitewithimpcheck_local_cache_hit, applystepwithimpcheck_local_cache_max_size, applyitewithimpcheck_local_cache_max_size, applystepwithimpcheck_max_context_size, applyitewithimpcheck_max_context_size, time_in_local_hashing_in_applystepwithimpcheck, time_in_local_hashing_in_applyitewithimpcheck, applystepwithimpcheck_local_cache_trivial_hit, applyitewithimpcheck_local_cache_trivial_hit, applystepwithimpcheck_local_cache_level1_hit, applyitewithimpcheck_local_cache_level1_hit, eliminate_incons_paths_int_local_cache_level1_hit, max_context_size_imp_check, applystepwithimpcheck_external_cache_hit, applystepwithimpcheck_external_cache_miss, createitewithimpcheck_external_cache_hit, createitewithimpcheck_external_cache_miss, applystepwithimpcheck_misses, applyitewithimpcheck_misses,  effect_of_reduce, trim_context_strategy_four_times, level_2_trimming_effect, trimming_time, thunsat_calls);  
}

// function to call boolector with time recording
void DDManager::callBoolectorForImplicationCheck()
{
 time_t solver_start, solver_end, solver_duration;
 time(&solver_start);
system("boolector --smt thunsat_impcheck.smt > result_thunsat_impcheck.txt");
 time(&solver_end);
 solver_duration = solver_end-solver_start;
 SMTSolver_Time_In_ImplicationCheck = SMTSolver_Time_In_ImplicationCheck + solver_duration;
}


// checks if E/\D/\u_AP is unsat by QE using Layer1 and Layer2 (Implemented in 
// function qEliminatorForSolving in Eliminator.cpp)
// Returns 'u' for unsat, 's' for sat, 'x' for don't know
char DDManager::implicationCheckByExQuantifierElimination(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP, map<string, int> &WidthTable)
{
  time_t exsolver_start, exsolver_end, exsolver_duration;
  time(&exsolver_start);

  char ret_value;

  if(E.empty() && D.empty()) // empty context
    {
      //cout<<"\nWarning!!Context empty in function DDManager::implicationCheckByExQuantifierElimination. Empty context is interpreted as true context. Hence, result depends on consequent\n"; 
      //return 's';
    }
    

  // get the matrices
  
  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;


  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  getDAGNames(u_AP, names);

  attachSetToList(names, VarNames);

  NoVars=VarNames.size();
  
  NoVarsToElim=NoVars;

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  Constraints.insert(u_AP);

  //fprintf(ImpCheckFile, "\nConstraints\n");
  //for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
  //	{
  //	 fprintf(ImpCheckFile,"\n%s\n", getDAGStr(*it).c_str());
  //	}

  

  // Get the equations and disequations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  if(!result) // false 
    {
      fprintf(ImpCheckFile,"\nunsat returned by getEquationsAndDisequations\n");
      ret_value = 'u';
    }
  else
   {

   //fprintf(ImpCheckFile,"\nConstraints in matrix form\n");
   //printEquations(EqCoefMat, EqConstMat, ImpCheckFile);
   //printDisEquations(DisEqCoefMat, DisEqConstMat, ImpCheckFile);


   ULINT Mod=findPower(NoOfBits);

   //fprintf(ImpCheckFile, "\nMod = %llu\n", Mod);

   ret_value = qEliminatorForSolving(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);
   }

 time(&exsolver_end);
 exsolver_duration = exsolver_end-exsolver_start;
 ExSolver_Time_In_ImplicationCheck = ExSolver_Time_In_ImplicationCheck + exsolver_duration;

 return ret_value;
  
}


// checks if E/\D/\~u_AP is sat by seeing if there is overlap of variables between E/\D and ~u_AP. 
// If there is no overlap, return 's' for sat, else return 'x' for don't know
char DDManager::implicationCheckByOverlapChecking(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP)
{
set<string> NamesAntecedent;
set<string> NamesConsequent;

for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, NamesAntecedent);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, NamesAntecedent);


getDAGNames(u_AP, NamesConsequent);

set<string> intersect;

set_intersection(NamesAntecedent.begin(), NamesAntecedent.end(), NamesConsequent.begin(), NamesConsequent.end(),inserter(intersect, intersect.begin()));

if(intersect.empty()) // No common variables
	{
	return 's';
	}
return 'x';
}


// Show the context from the cache tree
void DDManager::showContextOfCacheTree()
{
  set<DAGNode*> Path;
  CacheTree.getPath(Path);
  DAGNode *root = NULL;
  set<DAGNode*>::iterator it=Path.begin();
  for(; it!=Path.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

  fprintf(ImpCheckFile, "\n%s\n", getDAGStr(root).c_str());
}

// Function to check if two sets are equal
bool  DDManager:: setsEqual(set<DAGNode*> &SetA, set<DAGNode*> &SetB)
{
 set<DAGNode*> Set1, Set2;
 set_difference(SetA.begin(), SetA.end(), SetB.begin(), SetB.end(),inserter(Set1, Set1.begin()));
 set_difference(SetB.begin(), SetB.end(), SetA.begin(), SetA.end(),inserter(Set2, Set2.begin()));
 if(Set1.empty() && Set2.empty()) return true;
 return false;
}

// Function to check if the present context is the same as the path
void DDManager::checkContext(set<DAGNode*> E, set<DAGNode*> D)
{
  set<DAGNode*> Context;
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin()));
  
  set<DAGNode*> Path;
  CacheTree.getPath(Path);


  if(setsEqual(Context, Path))
   {
     fprintf(ImpCheckFile, "\nContexts are matching\n");
   }
  else
   {
    fprintf(ImpCheckFile, "\nContexts are not matching\n");
    exit(1);
   } 
}


/* Functions related to Implication Checking end here*/

// Function to show the predicates in a dd 
void DDManager::showPredicates(DDNode *dd, string WhoAmI){
// get the predicates in dd 
  SetOfDDNodes.clear();
  set<DAGNode*> PredicatesPresent;
  getPredicates(dd, PredicatesPresent); // find the predicates
  cout<<"\nPredicates present in the DD "<<WhoAmI<<" are \n****************************\n";
  for(set<DAGNode*>::iterator it=PredicatesPresent.begin(); it!=PredicatesPresent.end(); it++){
    cout<<endl<<getDAGStr(*it)<<"\t"<<APMap[*it];
  }
  cout<<"\n****************************\n";
}


// Function to show the predicates common between dd1 and dd2
void DDManager::showCommonPredicates(DDNode *dd1, DDNode *dd2, string WhoisDD1, string WhoisDD2){
// get the predicates in dd1 
  SetOfDDNodes.clear();
  set<DAGNode*> PredicatesPresentInDD1;
  getPredicates(dd1, PredicatesPresentInDD1); // find the predicates in dd1

  SetOfDDNodes.clear();
  set<DAGNode*> PredicatesPresentInDD2;
  getPredicates(dd2, PredicatesPresentInDD2); // find the predicates in dd2

  set<DAGNode*> intersect;
  set_intersection(PredicatesPresentInDD1.begin(), PredicatesPresentInDD1.end(), PredicatesPresentInDD2.begin(), PredicatesPresentInDD2.end(),inserter(intersect, intersect.begin()));

  cout<<"\nPredicates common between "<<WhoisDD1<<" and "<<WhoisDD2<<" are \n****************************\n";
  for(set<DAGNode*>::iterator it=intersect.begin(); it!=intersect.end(); it++){
    cout<<endl<<getDAGStr(*it);
  }
  cout<<"\n****************************\n";
}



// Reorder the LMEs in APList to help in smaller LMDD sizes
void DDManager::reorderingBasedOnSharing(list<DAGNode*> &APList, map<string, int> &WidthTable, map<int, int> &DeltaMapForFindingSharedMEs)
{
  list<DAGNode*> TempAPList;
  
  for(list<DAGNode*>::iterator it=APList.begin();it!=APList.end();it++)
    {
      DAGNode *pred = recreateEquationInProperForm(*it, WidthTable);
      TempAPList.push_back(pred);
    }

  list<DAGNode*> SharedAPs;

  for(list<DAGNode*>::iterator it=TempAPList.begin(); it!=TempAPList.end(); it++)
    {
      DAGNode *pred = *it;

      //map<int, int> DeltaMap;

      //DeltaMap.insert(make_pair(0, 1));
      //DeltaMap.insert(make_pair(1, 2));
   
      DAGNode* shifted_pred;

      shifted_pred = createShiftedLMEBasedOnDeltaMap(pred, DeltaMapForFindingSharedMEs);

      shifted_pred = recreateEquationInProperForm(shifted_pred, WidthTable); // recreate in cannonical form

      // shifted_pred is the predicate which will be present in R(s1, s2). Check if it is a shared predicate i.e. already in APList

      if(searchAPList(TempAPList, shifted_pred)) // shifted_pred is present in TempAPList. Hence it is shared
	{
	  SharedAPs.push_back(shifted_pred);
	}
      
    }

  APList.clear();

  for(list<DAGNode*>::iterator it=TempAPList.begin(); it!=TempAPList.end(); it++)
    {
      DAGNode *pred = *it;

      if(!searchAPList(SharedAPs, pred)) 
	{
	  APList.push_back(pred);
	}
    }
  

  // Put SharedAPs are at the end
  
  cout<<"\nShared LMEs\n";


  for(list<DAGNode*>::iterator it=SharedAPs.begin(); it!=SharedAPs.end(); it++)
    {
      DAGNode *pred = *it;

      cout<<getDAGStr(pred)<<endl;
      
      APList.push_back(pred);
    }
}

// Return true if common predicates exist between dd1 and dd2
bool DDManager::commonPredicatesExists(DDNode *dd1, DDNode *dd2){
// get the predicates in dd1 
  SetOfDDNodes.clear();
  set<DAGNode*> PredicatesPresentInDD1;
  getPredicates(dd1, PredicatesPresentInDD1); // find the predicates in dd1

  SetOfDDNodes.clear();
  set<DAGNode*> PredicatesPresentInDD2;
  getPredicates(dd2, PredicatesPresentInDD2); // find the predicates in dd2

  set<DAGNode*> intersect;
  set_intersection(PredicatesPresentInDD1.begin(), PredicatesPresentInDD1.end(), PredicatesPresentInDD2.begin(), PredicatesPresentInDD2.end(),inserter(intersect, intersect.begin()));

  if(intersect.empty())
    {
      return false;
    }
  else
    {
      return true;
    }
}

// Function which is exactly same as createShiftedDD except that the LMEs 
// which will get shared with the next frame will be pushed down in the order
DDNode*  DDManager::createShiftedDD_With_ReorderingBasedOnSharing_Enabled(DDNode *dd, map<int, int> &DeltaMap, map<string, int> &WidthTable)
{
  insertPredicatesIntoAPMapIncreateShiftedDD_With_ReorderingBasedOnSharing_Enabled(dd, DeltaMap, WidthTable); // insert the shifted predicates at proper locations in the APMap

  map<DDNode*, DDNode*> CreateShiftedDDTable;
  map<DDNode*, DDNode*> *PtrCreateShiftedDDTable = &CreateShiftedDDTable;

  DDNode* resultDD = createShiftedDDInternal(dd, DeltaMap, WidthTable, PtrCreateShiftedDDTable);
  return resultDD;
}

// This function is exactly same as insertPredicatesIntoAPMapIncreateShiftedDD except that while inserting 
// the newly created LMEs, it pushes the shared LMEs down when the flag "reorderingBasedOnSharing_Enabled" is true
void DDManager::insertPredicatesIntoAPMapIncreateShiftedDD_With_ReorderingBasedOnSharing_Enabled(DDNode *dd, map<int, int> &DeltaMap, map<string, int> &WidthTable)
{
  // get the predicates in dd 
  SetOfDDNodes.clear();
  set<DAGNode*> PredicatesPresent;
  getPredicates(dd, PredicatesPresent); // find the predicates
  
  map<int, DAGNode*> LocalPredicateMap;
  
  for(set<DAGNode*>::iterator it=PredicatesPresent.begin(); it!=PredicatesPresent.end(); it++)
    {
      int index;

      DAGNode* pred = *it;
      
      if(APMap.find(pred)==APMap.end()) // no entry in APMap for the DAGNode* pred : ERROR
		{
		cout<<"\nNo entry for APMap for the predicate node "<<pred<<"in function DDManager::insertPredicatesIntoAPMapIncreateShiftedDD\n";
		exit(1);
		}

      index = APMap[pred];

      DAGNode* shifted_pred;

      shifted_pred = createShiftedLMEBasedOnDeltaMap(pred, DeltaMap);

      shifted_pred = recreateEquationInProperForm(shifted_pred, WidthTable); // recreate in cannonical form
      
      LocalPredicateMap.insert(make_pair(index, shifted_pred));
    }

  // LocalPredicateMap is sorted according to the index of the original predicate pred
  // Insert the newly created predicates one by one according to the order suggested by the LocalPredicateMap

  // Insert LMEs in local predicate map into a list

  list<DAGNode*> PredicateList;

  for(map<int, DAGNode*>::iterator it=LocalPredicateMap.begin(); it!=LocalPredicateMap.end(); it++)
    {
      DAGNode* pred = it->second;

      PredicateList.push_back(pred);
    }

  showAPList(PredicateList);

  if(reorderingBasedOnSharing_Enabled) // push the LMEs which are getting shared with the next frame to lesser order
	{
          // create DeltaMapForFindingSharedMEs 

	  map<int, int> DeltaMapForFindingSharedLMEs;

          createDeltaMapForFindingSharedLMEs(DeltaMap, DeltaMapForFindingSharedLMEs);
 
	  reorderingBasedOnSharing(PredicateList, WidthTable, DeltaMapForFindingSharedLMEs);

	  cout<<"\nAfter Reordering Based on Sharing\n";

	  showAPList(PredicateList);
	}

  for(list<DAGNode*>::iterator it=PredicateList.begin(); it!=PredicateList.end(); it++)
    {
      DAGNode* pred = (*it);

      pushBackPredicateIntoAPMap(pred);
    }
}

// create DeltaMap for finding shared LMEs
// Used in the function DDManager::insertPredicatesIntoAPMapIncreateShiftedDD_With_ReorderingBasedOnSharing_Enabled
void DDManager::createDeltaMapForFindingSharedLMEs(map<int, int> &DeltaMap, map<int, int> &DeltaMapForFindingSharedLMEs)
{
int initial_state_counter;

map<int, int>::iterator it=DeltaMap.begin();
int count1=it->second;
it++;
int count2=it->second;
if(count1<count2)
	{
	initial_state_counter = count1;
	}
else
	{
	initial_state_counter = count2;
	}
DeltaMapForFindingSharedLMEs.insert(make_pair(initial_state_counter, initial_state_counter+1));
DeltaMapForFindingSharedLMEs.insert(make_pair(initial_state_counter+1, initial_state_counter+2));
}



// Call QE_LMC to obtain \exists VarsToElim. (E /\ D), and convert it to LMDD
DDNode* DDManager::APResolveModifiedWithMultipleVariablesUsingLMA(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> WidthTable)
{

  DAGNode *resultDAG = APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(E, D, VarsToElim, WidthTable);
  DDNode* resultDD;
  resultDD = createDDFromDAGWithoutImplicationCheck(resultDAG, WidthTable);
  return resultDD;

}


// Functions to print information about dd blowup in createITE
void DDManager::printBlowupInApply(string op, int s1,int s2,int s3,int s)
{
  float blowup = (float)s/(float)(s1+s2+s3);
  if(blowup>blow_max_ite)
	{
	blow_max_ite=blowup;
	}
  if(blowup<blow_min_ite)
	{
	blow_min_ite=blowup;
	}

  fprintf(FILE_Blowup, "\n%s\t%d\t%d\t%d\t%d\t", op.c_str(),s1,s2,s3,s);
  if(s1+s2+s3<s)
    {
      fprintf(FILE_Blowup, "bad");
      ite_blows++;
    }
  else 
    {
     fprintf(FILE_Blowup, "good");
     ite_shrinks++;
    }
}

// Functions to print information about dd blowup in apply
void DDManager::printBlowupInApply(string op, int s1,int s2,int s)
{
  float blowup = (float)s/(float)(s1+s2);
  if(blowup>blow_max_apply)
	{
	blow_max_apply=blowup;
	}
  if(blowup<blow_min_apply)
	{
	blow_min_apply=blowup;
	}

  fprintf(FILE_Blowup, "\n%s\t%d\t%d\t%d\t", op.c_str(), s1,s2,s);
  if(s1+s2<s)
    {
      fprintf(FILE_Blowup, "bad");
      apply_blows++;
    }
  else if(s1!=1 && s2!=1) // trivial cases : always good
    {
     fprintf(FILE_Blowup, "good");
     apply_shrinks++;
    }
}


// returns true if fixpoint is reached in iterative squaring; false otherwise
bool DDManager::fixpointReached(DDNode *old_dd, DDNode *new_dd, map<string, int> &WidthTable)
{

  DAGNode* old_dag = createDAGFromDD(old_dd);

  DAGNode* new_dag = createDAGFromDD(new_dd);
 
  DAGNode *cross_check = createDAG("is_not_equal", old_dag, new_dag, LOGICAL_SYMBOL, 1);

  if(ThUnsatSingleFormula(cross_check, WidthTable))
    {
      cout<<"\nFix point reached\n"; 
      return true;

    }
  else
    {
      cout<<"\nFix point is not reached\n";
      return false;
    }
 
}


// Trim down E and D to useful context 
// Useful context has only LMEs and LMDs of #bits = #bits in u_AP
void DDManager::trimContext(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP, set<DAGNode*> &Trimmed_E, set<DAGNode*> &Trimmed_D, map<string, int> &WidthTable)
{

int NoOfBitsInConsequent=getNoOfBitsInConstraint(u_AP, WidthTable); 

for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
  {
    int NoOfBitsInLME=getNoOfBitsInConstraint(*it, WidthTable);

    if(NoOfBitsInConsequent==NoOfBitsInLME)
      {
	Trimmed_E.insert(*it);
      }
  }

for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
  {
    int NoOfBitsInLMD=getNoOfBitsInConstraint(*it, WidthTable);

    if(NoOfBitsInConsequent==NoOfBitsInLMD)
      {
	Trimmed_D.insert(*it);
      }
  }

 

 if(false)
   {

     DAGNode *root = NULL;
     set<DAGNode*>::iterator it=E.begin();
     for(; it!=E.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
     it=D.begin();
     for(; it!=D.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

     cout<<"\nContext\n************\n";
     if(root==NULL)
       {
	 cout<<"\nNULL\n";
       }
     else
       {
	 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
       }


     root = NULL;
     it=Trimmed_E.begin();
     for(; it!=Trimmed_E.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
     it=Trimmed_D.begin();
     for(; it!=Trimmed_D.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

     cout<<"\nTrimmed Context\n************\n";
     if(root==NULL)
       {
	 cout<<"\nNULL\n";
       }
     else
       {
	 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
       }
   }


}

// Set the global variable SignalClass
void DDManager::findSignalClass(list<DAGNode*> &APList)
{
  for(list<DAGNode*>::iterator it=APList.begin();it!=APList.end();it++)
    {
      set<string> SignalsWithDelta;
      set<string> Signals;
      
      //cout<<"\nDAG = "<<getDAGStr(*it)<<endl;
      
      getDAGNames(*it, SignalsWithDelta);
      createDeltaTrimmedNames(SignalsWithDelta, Signals);

      //showSet(Signals, "Signals");

      if(SignalClass.size()==0)
	{
	  //cout<<"\nPushing back Signals\n";
	  SignalClass.push_back(Signals);
	}
      else
	{
	  int int_set = findIntersectingSet(Signals);
	  if(int_set!=-1) // an intersecting set exists
	    {
	      //cout<<"\nTake union with existing thing, remove the existing thing and push the union\n";
	      set<string> NewSignals;
	      set_union(SignalClass[int_set].begin(), SignalClass[int_set].end(), Signals.begin(), Signals.end(),inserter(NewSignals, NewSignals.begin()));
	      SignalClass.erase(SignalClass.begin()+int_set);
	      SignalClass.push_back(NewSignals);
	    }
	  else
	    {
	      //cout<<"\nPushing back Signals\n";
	     SignalClass.push_back(Signals);
	    }
	}

      //showSignalClass();
	     
    }// for ends here

  int initial_size, final_size;
  do
    {
      initial_size=SignalClass.size();
      bool match_found=false;
      int i,j;
      set<string> Set1, Set2, Set3;
      for(i=0;i<SignalClass.size();i++)
	{
	for(j=i+1;j<SignalClass.size();j++)
	  {
	    //cout<<"\ni="<<i<<"\tj="<<j<<endl;
	    Set1 = SignalClass[i];
	    Set2 = SignalClass[j];
	    if(intersectionExists(Set1, Set2))
	      {
		Set3.clear();
		set_union(Set1.begin(), Set1.end(), Set2.begin(), Set2.end(),inserter(Set3, Set3.begin()));
		match_found=true;
		break;
	      }
	  }// for j
	if(match_found)
	  {
	    break;
	  }
	}// for i
      if(match_found)
	  {
	    SignalClass.erase(SignalClass.begin()+j);
	    SignalClass.erase(SignalClass.begin()+i);
	    SignalClass.push_back(Set3);
	  }
      final_size=SignalClass.size();
    }while(initial_size!=final_size);// do--while ends 
  
  //showSignalClass();
      
}

// Given "SignalsWithDelta", get "Signals" which contain 
// variables in "SignalsWithDelta" without the delta field
void DDManager::createDeltaTrimmedNames(set<string> &SignalsWithDelta, set<string> &Signals)
{
  for(set<string>::iterator it=SignalsWithDelta.begin();it!=SignalsWithDelta.end();it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function DDManager::createDeltaTrimmedNames\n";
		    exit(1);
		  }
      string id = id_delta.substr(0, index);
      Signals.insert(id);
    }
}

// Find the index of the set in SignalClass whose contents 
// share some variables with "Signals", and return it
int DDManager::findIntersectingSet(set<string> &Signals)
{
  for(int i=0;i<SignalClass.size();i++)
    {
      set<string> MySet=SignalClass[i];
      set<string> intersection;
      set_intersection(MySet.begin(), MySet.end(), Signals.begin(), Signals.end(),inserter(intersection, intersection.begin()));
      if(!intersection.empty())
	{
	  return i;
	}
    }
  return -1;
}

// show the global variable "SignalClass"
void DDManager::showSignalClass()
{
  cout<<"\nSignal Class\n";
  for(int i=0;i<SignalClass.size();i++)
    {
      set<string> MySet=SignalClass[i];
      showSet(SignalClass[i], "Class");
    }
}

// return true if "Set1" intersection "Set2" is non-empty; return false otherwise
bool DDManager::intersectionExists(set<string> &Set1, set<string> &Set2)
{
  set<string> Set3;
  set_intersection(Set1.begin(), Set1.end(), Set2.begin(), Set2.end(),inserter(Set3, Set3.begin()));
  if(!Set3.empty())//intersection exists
	{
	  return true;
	}
  return false;
}

// Set the global variable "SignalEquivalenceClass" from the global variable "SignalClass"
void DDManager::createSignalEquivalenceClass()
{
  for(int i=0;i<SignalClass.size();i++)
    {
      set<string> MySet=SignalClass[i];
      for(set<string>::iterator it=MySet.begin();it!=MySet.end();it++)
	{
	  SignalEquivalenceClass.insert(make_pair(*it, i));
	}
      
    }
}

// show the global variable "SignalEquivalenceClass"
void DDManager::showSignalEquivalenceClass()
{
  cout<<"\nSignalEquivalenceClass\n";
  for(map<string, int>::iterator it=SignalEquivalenceClass.begin();it!=SignalEquivalenceClass.end();it++)
    {
      cout<<"\n"<<it->first<<"\t"<<it->second<<"\n";
    }
}


// Trim down E and D to useful context based on equivalence classes
void DDManager::trimContextAdvanced(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP, set<DAGNode*> &Trimmed_E, set<DAGNode*> &Trimmed_D)
{

int Class_Consequent=findEquivalenceClass(u_AP); 

for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
  {
    int Class_LME=findEquivalenceClass(*it);

    if(Class_Consequent==Class_LME)
      {
	Trimmed_E.insert(*it);
      }
  }

for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
  {
    int Class_LMD=findEquivalenceClass(*it);

    if(Class_Consequent==Class_LMD)
      {
	Trimmed_D.insert(*it);
      }
    
  }

 

 if(false)
   {

     DAGNode *root = NULL;
     set<DAGNode*>::iterator it=E.begin();
     for(; it!=E.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
     it=D.begin();
     for(; it!=D.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

     cout<<"\nContext\n************\n";
     if(root==NULL)
       {
	 cout<<"\nNULL\n";
       }
     else
       {
	 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
       }


     root = NULL;
     it=Trimmed_E.begin();
     for(; it!=Trimmed_E.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
     it=Trimmed_D.begin();
     for(; it!=Trimmed_D.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

     cout<<"\nTrimmed Context\n************\n";
     if(root==NULL)
       {
	 cout<<"\nNULL\n";
       }
     else
       {
	 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
       }
   }


}


// Find the equivalence class of the given LME/LMD. 
// Equivalence class of the LME/LMD is the equivalence class of any variable in it
int DDManager::findEquivalenceClass(DAGNode *Constraint)
{
  string variable="";
  variable=getSingleVariable(Constraint);
  if(variable=="")
    {
      cout<<"\nLME/LMD "<<getDAGStr(Constraint)<<" encountered with no variable names in function DDManager::findEquivalenceClass\n";
      exit(1);
    }
  int index = variable.find_last_of("_");
  if(index==string::npos)
		  {
		    cout<<"\nVariable name "<<variable<<"without time stamp in function DDManager::findEquivalenceClass\n";
		    exit(1);
		  }
  variable = variable.substr(0, index);
  map<string, int>::iterator it=SignalEquivalenceClass.find(variable);
  if(it==SignalEquivalenceClass.end())
    {
      cout<<"\nSignal "<<variable<<" encountered with no entry in SignalEquivalenceClass in function DDManager::findEquivalenceClass\n";
      exit(1);
    }
  return it->second;
}

// Function to show the useful part of context in implication checks in apply
void DDManager::showUsefulpartInApplyWithImpeck(string op, DDNode *v1, DDNode *v2, set<DAGNode*> &Useful_Context, DDNode *u)
{
cout<<"\n#######################\n";
cout<<"\nUseful Part In ApplyWithImpcheck\n";
cout<<"\nArgument DD1 :\n"; 
cout<<getDAGStr(createDAGFromDD(v1));
cout<<"\nArgument DD2 :\n"; 
cout<<getDAGStr(createDAGFromDD(v2));
 cout<<"\nOperation = "<<op<<endl;
showAPSet("Useful_Context", Useful_Context);
cout<<"\nbecomes:\n";
cout<<getDAGStr(createDAGFromDD(u));
cout<<"\n*****************\n"; 
}

// Another version of function to show the useful part of context in implication checks in apply
void DDManager::showContextInApplyWithImpCheck(string op, DDNode *v1, DDNode *v2, set<DAGNode*> &E, set<DAGNode*> &D)
{
cout<<"\n#######################\n";
cout<<"\nContext In ApplyWithImpcheck\n";
cout<<"\nArgument DD1 :\n"; 
cout<<getDAGStr(createDAGFromDD(v1));
cout<<"\nArgument DD2 :\n"; 
cout<<getDAGStr(createDAGFromDD(v2));
 cout<<"\nOperation = "<<op<<endl;
showAPSet("E", E);
showAPSet("D", D);
cout<<"\nbecomes:\n";
cout<<"\n*****************\n"; 
}

// Display the implication check /\E/\D => u_AP on screen
void DDManager::showImplicationCheckModified(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP)
{
static ULINT ShowImplicationCount=0;

 DAGNode *root = NULL;
 set<DAGNode*>::iterator it=E.begin();
 for(; it!=E.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
 it=D.begin();
 for(; it!=D.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

 //cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
 ShowImplicationCount++;
 char temp[100];
 sprintf(temp, "%llu", ShowImplicationCount);
 string temp_str(temp);
 string string_to_show = temp_str;
 string_to_show += "  ";

 sprintf(temp, "%llu", Calls_For_ImpCheck);
 temp_str = temp;
 string_to_show += temp_str;
 string_to_show += "  ";
 
 if(root!=NULL)
   {
     string_to_show += getDAGStr(root);
     string_to_show += " => ";
     string_to_show += getDAGStr(u_AP);
     cout<<endl<<string_to_show<<endl;
   }
 else // empty context
   {
     cout<<endl<<"Context is empty : hence trivially sat"<<endl;
   }
 //cout<<endl<<string_to_show<<endl;
}

// Function to get entry from the local hash table of ApplyWithImpCheck
// returns the hit entry if hit; returns NULL in case of miss
DDNode* DDManager::getEntryFromLocalHashTableOfApplyWithImpCheck(map< pair< pair<DDNode*, DDNode*>, set<DAGNode*> >,  DDNode*> *PtrLocalHashTable, DDNode *v1, DDNode *v2, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &Useful_Context)
{
if(v1->getPredicate()== NULL && v2->getPredicate()== NULL)
    {
      return NULL;
    }

time_t start, end, duration;
time(&start);


DDNode *resultDD=NULL;

set<DAGNode*> Context;
set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin()));
  
map< pair< pair<DDNode*, DDNode*>, set<DAGNode*> >,  DDNode*>::iterator it=PtrLocalHashTable->begin();

for(;it!=PtrLocalHashTable->end();it++)
   {
     // take each entry
     DDNode *op1 = ((it->first).first).first;
     DDNode *op2 = ((it->first).first).second;
     set<DAGNode*> Present_Useful_Context = (it->first).second;
     
     
     if(v1==op1 && v2==op2 && Present_Useful_Context.empty())
       {
	 applystepwithimpcheck_local_cache_trivial_hit++;
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
     else if(v1==op1 && v2==op2 && isSubSet(Present_Useful_Context, Context))
       {
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
   }

 time(&end);
 duration = end-start;
 time_in_local_hashing_in_applystepwithimpcheck = time_in_local_hashing_in_applystepwithimpcheck + duration;



 return resultDD;
}

// Function to show the local hash table of ApplyWithImpCheck
void DDManager::showLocalHashTableOfApplyWithImpCheck(map< pair< pair<DDNode*, DDNode*>, set<DAGNode*> >,  DDNode*> *PtrLocalHashTable)
{
map< pair< pair<DDNode*, DDNode*>, set<DAGNode*> >,  DDNode*>::iterator it=PtrLocalHashTable->begin();

for(;it!=PtrLocalHashTable->end();it++)
   {
     cout<<"\nEntry\n********\n";
     
     DDNode *op1 = ((it->first).first).first;
     DDNode *op2 = ((it->first).first).second;
     set<DAGNode*> Present_Useful_Context = (it->first).second;
     DDNode *result = it->second;

     showUsefulpartInApplyWithImpeck("no matter", op1, op2, Present_Useful_Context, result);
   }
}

// Function to get entry from the local hash table of EliminateInconsistentPaths
// returns the hit entry if hit; returns NULL in case of miss
DDNode* DDManager::getEntryFromLocalHashTableOfEliminateInconsistentPaths(map< pair< DDNode*, set<DAGNode*> >,  DDNode*> *PtrLocalHashTable, DDNode *v1, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &Useful_Context)
{
if(v1->getPredicate()== NULL)
    {
      return NULL;
    }

DDNode *resultDD=NULL;

set<DAGNode*> Context;
set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin()));
  
map< pair< DDNode*, set<DAGNode*> >,  DDNode*>::iterator it=PtrLocalHashTable->begin();

for(;it!=PtrLocalHashTable->end();it++)
   {
     // take each entry
     DDNode *op1 = ((it->first).first);
     set<DAGNode*> Present_Useful_Context = (it->first).second;
     
     
     if(v1==op1 && Present_Useful_Context.empty())
       {
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
     else if(v1==op1 && isSubSet(Present_Useful_Context, Context))
       {
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
   }
 return resultDD;
}

// Function to get entry from the local hash table of ApplyITEWithImpCheck
// returns the hit entry if hit; returns NULL in case of miss
DDNode* DDManager::getEntryFromLocalHashTableOfApplyITEWithImpCheck(map< pair< pair< pair<DDNode*, DDNode*>, DDNode*> , set<DAGNode*> >,  DDNode*> *PtrLocalHashTable, DDNode *condn, DDNode *then, DDNode *els, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &Useful_Context)
{
if(condn->getPredicate()== NULL && then->getPredicate()== NULL && els->getPredicate()== NULL)
    {
      return NULL;
    }

time_t start, end, duration;
time(&start);


DDNode *resultDD=NULL;

set<DAGNode*> Context;
set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin()));
  
map< pair< pair< pair<DDNode*, DDNode*>, DDNode*> , set<DAGNode*> >,  DDNode*>::iterator it=PtrLocalHashTable->begin();

for(;it!=PtrLocalHashTable->end();it++)
   {
     // take each entry
     DDNode *present_condn = (((it->first).first).first).first;
     DDNode *present_then = (((it->first).first).first).second;
     DDNode *present_els = ((it->first).first).second;

     set<DAGNode*> Present_Useful_Context = (it->first).second;
     
     
     if(condn==present_condn && then==present_then && els==present_then && Present_Useful_Context.empty())
       {
	 applyitewithimpcheck_local_cache_trivial_hit++;
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
     else if(condn==present_condn && then==present_then && els==present_then && isSubSet(Present_Useful_Context, Context))
       {
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
   }

 time(&end);
 duration = end-start;
 time_in_local_hashing_in_applyitewithimpcheck = time_in_local_hashing_in_applyitewithimpcheck + duration;


 return resultDD;
}


// Function to show the "context" of calling the implication check and the implied LMC "u_AP"
void DDManager::showImplicationCheckContext(set<DAGNode*> &Context, DAGNode *u_AP)
{
  if(Context.size()==0)
    {
      cout<<"\nContext empty in DDManager::showImplicationCheckContext\n";
      exit(1);
    }

 DAGNode *root = NULL;
 set<DAGNode*>::iterator it=Context.begin();
 for(; it!=Context.end(); it++)
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
}

// Version of copySet where the second argument is passed by reference
void DDManager::copySetNew(set<DAGNode*> &dest, set<DAGNode*> &source)
{
  set<DAGNode*>::iterator it;
  for(it=source.begin(); it!=source.end(); it++)
    dest.insert(*it);
}

// Another version of function to show the local hash table of ApplyWithImpCheck
void DDManager::showLocalHashTableForApplyWithImpCheck(map< pair< pair<DDNode*, DDNode*>, set<DAGNode*> >,  DDNode*> *PtrLocalHashTable)
{
cout<<"\nLocalHashTableForApplyWithImpCheck\n"<<endl;
map< pair< pair<DDNode*, DDNode*>, set<DAGNode*> >,  DDNode*>::iterator it=PtrLocalHashTable->begin();

int i=0;
for(;it!=PtrLocalHashTable->end();it++)
   {
     cout<<"\nEntry "<<i+1<<" : "<<endl;
     // take each entry
     DDNode *op1 = ((it->first).first).first;
     DDNode *op2 = ((it->first).first).second;
     set<DAGNode*> Present_Useful_Context = (it->first).second;
     DDNode *result = it->second;
     
     cout<<"\n"<<op1<<"\t"<<op2<<endl;
     showAPSet("Present_Useful_Context", Present_Useful_Context);
     cout<<"Result: "<<result<<endl;

     i++;
     
   }


}


// The internal function called by applyWithImplicationCheck
// (uses with STL map for level-1 cache)
DDNode* DDManager::applyStepWithImplicationCheck(string op, DDNode* v1, DDNode* v2, set<DAGNode*> E, set<DAGNode*> D, map<string, int> &WidthTable, set<DAGNode*> &Useful_Context, map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable, map<string, DDNode*> *PtrLocalHashTable_Level_1)
{
  // Declarations for finding effect of cache  
  applystepwithimpcheck_calls++;
  bool hashing_enabled=false;
  // Declarations for finding effect of cache ends here

  // Declarations for useful context finding
  bool useful_context_finding=true;
  set<DAGNode*> Useful_E_For_Impcheck;
  set<DAGNode*> Useful_D_For_Impcheck;
  set<DAGNode*> Useful_Context_For_Child1;
  set<DAGNode*> Useful_Context_For_Child2;
  set<DAGNode*> Useful_Context_For_Impcheck;
  set<DAGNode*> Context;
  

  Useful_E_For_Impcheck.clear();
  Useful_D_For_Impcheck.clear();
  Useful_Context_For_Child1.clear();
  Useful_Context_For_Child2.clear();
  Useful_Context_For_Impcheck.clear();
  Useful_Context.clear();
  Context.clear();

  bool subset_based_local_hashing=true;
  bool level_1_local_hashing=true;
  //showContextInApplyWithImpCheck(op,v1,v2,E,D);
  // Declarations for useful context finding ends here


  DDNode* u; // result DD
  string key;
  string key_level_1;

  DAGNode* Negated_u_AP;

  if(hashing_enabled){

  // Creating the hash key

  key=op; // operation
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", v1); // first argument DD
  temp_key = temp_char;
  key += temp_key;

  sprintf(temp_char , "%x", v2); // second argument DD
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;  // all equations in the context
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key; // all disequations in the context
    }

  // Hash key created

  HTStatusValue<DDNode*> result = TableForApplyWithImpCheck.hashtable_search(key);

  if(result.success())
    {
  // code for finding effect of cache  
      if(!(E.empty() && D.empty()))
	{
      	applystepwithimpcheck_cache_non_trivial_hit++;
	}
      else
	{
	applystepwithimpcheck_cache_trivial_hit++;
	}
  // code for finding effect of cache ends here
      
      return result.GetValue();
    }
  }// if(hashing_enabled)

  if(level_1_local_hashing)
    {
      // Creating the hash key
      
      key_level_1=""; // operation
      string temp_key;
      char temp_char[10];

      sprintf(temp_char , "%x", v1); // first argument DD
      temp_key = temp_char;
      key_level_1 += temp_key;

      sprintf(temp_char , "%x", v2); // second argument DD
      temp_key = temp_char;
      key_level_1 += temp_key;


      // Hash key created
      // checking in hash table

      map<string, DDNode*>::iterator Level_1_it = PtrLocalHashTable_Level_1->find(key_level_1);

      if(Level_1_it!=PtrLocalHashTable_Level_1->end())
	{
	  // code for finding effect of cache  
	  applystepwithimpcheck_local_cache_level1_hit++;
	  // code for finding effect of cache ends here
	  return Level_1_it->second;
	}
    }// if(level_1_local_hashing)

  if(subset_based_local_hashing)
    {
      DDNode* ddnode_from_cache = getEntryFromEfficientLocalHashTableBinaryOperation(PtrLocalHashTable, v1, v2, E, D, Useful_Context);
      if(ddnode_from_cache!=NULL)
	{
	  applystepwithimpcheck_local_cache_hit++;
	  return ddnode_from_cache;
	}
    }

  applystepwithimpcheck_misses++;
  // TableForApplyWithImpCheck MISS

  int u_value, u_index;
  DAGNode *u_AP;

  u_value = evaluateOpOnTerminals(op, v1, v2); // if both operands are constants, evaluate and return
  if(u_value != -1)
    {
      //{create terminal vertex}
      u = createDD(u_value);
    }

  else // create nonterminal and evaluate further down 
    {

      int v1_index, v2_index;
      v1_index = getIndex(v1);
      v2_index = getIndex(v2);
      DDNode *v1_low, *v1_high, *v2_low, *v2_high, *v_low1, *v_high1, *v_low2, *v_high2, *u_low, *u_high;
      v1_low = v1->getLeft();
      v1_high = v1->getRight();
      v2_low = v2->getLeft();
      v2_high = v2->getRight();

      u_index = findMin(v1_index, v2_index);
      u_AP = findAPFromIndex(u_index);
      Negated_u_AP = negateConstraint(u_AP); 

      if(v1_index == u_index)
        {
          v_low1 = v1_low;
          v_high1 = v1_high;
        }
      else
        {
          v_low1 = v1;
          v_high1 = v1;
        }

      if(v2_index == u_index)
        {
          v_low2 = v2_low;
          v_high2 = v2_high;
        }
      else
        {
          v_low2 = v2;
          v_high2 = v2;
        }

      // Now we have identified the top equation, and the next low and high
      // apply context based simplification

      if(callImplicationCheck(E, D, u_AP, Negated_u_AP, WidthTable)) // E/\D=>u_AP . Hence only the right matters
	{
	  //showImplicationCheck(E, D, u_AP);

         // code for finding useful context  
          if(useful_context_finding)
		{
                if(trimming_context_mod_based)
			{
			trimContext(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_class_based)
			{
	        	trimContextAdvanced(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck);
			}
		else if(trimming_context_overapproximate)
			{
			trimContextEager(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_strategy_four)
			{
			trimContextStrategyFour(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
                else
			{
			cout<<"\nUnknown trimming mode\n"; exit(1);
			}
		}
          // code for finding useful context ends here

	u = applyStepWithImplicationCheck(op, v_high1, v_high2, E, D, WidthTable, Useful_Context_For_Child1, PtrLocalHashTable, PtrLocalHashTable_Level_1);
	}
      else if(callImplicationCheck(E, D, Negated_u_AP, u_AP, WidthTable)) // E/\D=>not(u_AP) . Hence only the left matters
	{
	  //showImplicationCheck(E, D, negateConstraint(u_AP));

        // code for finding useful context 
         if(useful_context_finding)
		{
                if(trimming_context_mod_based)
			{
			trimContext(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_class_based)
			{
	        	trimContextAdvanced(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck);
			}
		else if(trimming_context_overapproximate)
			{
			trimContextEager(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
		else if(trimming_context_strategy_four)
			{
			trimContextStrategyFour(E, D, u_AP, Useful_E_For_Impcheck, Useful_D_For_Impcheck, WidthTable);
			}
                else
			{
			cout<<"\nUnknown trimming mode\n"; exit(1);
			}
		}
       // code for finding useful context ends here

	u = applyStepWithImplicationCheck(op, v_low1, v_low2, E, D, WidthTable, Useful_Context_For_Child2, PtrLocalHashTable, PtrLocalHashTable_Level_1);
	}
       else // try to evaluate
	{
	u_value = evaluateOp(op, v1, v2); // evaluateOp
  	if(u_value != -1)
    		{
      		//{create terminal vertex}
      		u = createDD(u_value);
    		}
	else
		{
                set<DAGNode*> E_new, D_new;
          	copySet(E_new, E);
          	E_new.insert(u_AP);
          	copySet(D_new, D);
		D_new.insert(Negated_u_AP);

		if(cache_tree_on)
		{	
		CacheTree.moveDown(u_AP); // add u_AP into the context
		}
      		u_high = applyStepWithImplicationCheck(op, v_high1, v_high2, E_new, D, WidthTable, Useful_Context_For_Child2, PtrLocalHashTable, PtrLocalHashTable_Level_1);
		if(cache_tree_on)
		{
		CacheTree.moveUp(); // remove u_AP from the context
		}


		if(cache_tree_on)
		{
		CacheTree.moveDown(Negated_u_AP); // add Negated_u_AP into the context
		}
		u_low = applyStepWithImplicationCheck(op, v_low1, v_low2, E, D_new, WidthTable, Useful_Context_For_Child1, PtrLocalHashTable, PtrLocalHashTable_Level_1);
		if(cache_tree_on)
		{
		CacheTree.moveUp(); // remove Negated_u_AP from the context
		}


		u = createDD(u_AP, u_low, u_high);
		}
	}
      }

      if(hashing_enabled){
      HTStatusValue<DDNode*> result = TableForApplyWithImpCheck.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyStepWithImplicationCheck\n";
	    exit(1);
	  }
      }

    // code for finding useful context 
    // show v1,v2,useful context and u
    if(useful_context_finding)
	{
	  set<DAGNode*> temp1, temp2;

	set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin()));
	set_union(Useful_E_For_Impcheck.begin(), Useful_E_For_Impcheck.end(), Useful_D_For_Impcheck.begin(), Useful_D_For_Impcheck.end(),inserter(Useful_Context_For_Impcheck, Useful_Context_For_Impcheck.begin()));
	set_union(Useful_Context_For_Child1.begin(), Useful_Context_For_Child1.end(), Useful_Context_For_Child2.begin(), Useful_Context_For_Child2.end(),inserter(temp1, temp1.begin()));
        set_union(Useful_Context_For_Impcheck.begin(), Useful_Context_For_Impcheck.end(), temp1.begin(), temp1.end(),inserter(temp2, temp2.begin()));
        set_intersection(temp2.begin(), temp2.end(), Context.begin(), Context.end(),inserter(Useful_Context, Useful_Context.begin()));

	if(Useful_Context.size() > applystepwithimpcheck_max_context_size)
	    {
	      applystepwithimpcheck_max_context_size = Useful_Context.size();
 	      //cout<<"\nContext\n************\n";
	      //showLMDEContext(Context);
 	      //cout<<"\nUseful Context\n************\n";
	      //showLMDEContext(Useful_Context);
	    }

    	//showUsefulpartInApplyWithImpeck(op, v1, v2, Useful_Context, u);
	}
   // code for finding useful context ends here

    if(level_1_local_hashing && Useful_Context.empty())
      {
	// Hash key is key_level_1
	PtrLocalHashTable_Level_1->insert(make_pair(key_level_1, u));
      }// if(level_1_local_hashing)

    else if(subset_based_local_hashing)
	{
	  set<DAGNode*> Temp_Useful_Context;
	  copySetNew(Temp_Useful_Context, Useful_Context);


	  insertIntoEfficientLocalHashTableBinaryOperation(PtrLocalHashTable, v1, v2, Temp_Useful_Context, u);

	  if(PtrLocalHashTable->size() > applystepwithimpcheck_local_cache_max_size)
	    {
	      applystepwithimpcheck_local_cache_max_size = PtrLocalHashTable->size();
	    }

	}


      return u;
}



// Trim down E and D to useful context very eagerly 
// The trimmed context may be an over-approximation of the original context
void DDManager::trimContextEager(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP, set<DAGNode*> &Trimmed_E, set<DAGNode*> &Trimmed_D, map<string, int> &WidthTable)
{

  list<DAGNode*>::iterator iout;
  iout = u_AP->Get_outlist_start();
  DAGNode *lhs_u_AP = *iout;
  iout++;
  DAGNode *rhs_u_AP = *iout;

  // Check if there exists an LME e such that the lhs_e = lhs_u_AP. If yes, the problem is solved. return
  DAGNode *LME_Selected=NULL;
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    {
      list<DAGNode*>::iterator eiout;
      eiout = (*it)->Get_outlist_start();
      DAGNode *lhs_e = *eiout;
      eiout++;
      DAGNode *rhs_e = *eiout;
    
      if(lhs_u_AP==lhs_e)// lhs's match!!
	{
	  LME_Selected=*it;
	  break;
	}
    }
 
  if(LME_Selected!=NULL) // lme obtained
    {
      Trimmed_E.insert(LME_Selected);
    }
  else
    {
      DAGNode *LMD_Selected=NULL;
      // Check if there exists an LMD d such that either i) lhs_d = lhs_u_AP and rhs_d = rhs_u_AP or ii) lhs_d = lhs_u_AP and d is at bit-level.  If yes, the problem is solved. return
      for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
	{
	  list<DAGNode*>::iterator diout;
	  diout = (*it)->Get_outlist_start();
	  DAGNode *lhs_d = *diout;
	  diout++;
	  DAGNode *rhs_d = *diout;
    
	  if(lhs_u_AP==lhs_d)// lhs's match!!
	    {
	      if(rhs_u_AP==rhs_d) // rhs's match!!
		{
		  LMD_Selected=*it;
		  break;
		}
	      else 
		{
		  string variable=getSingleVariable(lhs_u_AP);
		  if(variable=="")
		    {
		      cout<<"\nLME/LMD "<<getDAGStr(lhs_u_AP)<<" encountered with no variable names in function DDManager::trimContextEager\n";
		      exit(1);
		    }
		  map<string, int>::iterator wit = WidthTable.find(variable);
		  if(wit==WidthTable.end())
		    {
		      cout<<"\nvariable "<<variable<<" encountered with no width in function DDManager::trimContextEager\n";
		      exit(1);
		    }
		  if(wit->second==1) // lhs_u_AP <> rhs_u_AP is a bit-level constraint
		    {
		      LMD_Selected=*it;
		      break;
		    }
		}// else
	    }// if(lhs_u_AP==lhs_d)
	}// for each LMD

      if(LMD_Selected!=NULL) // lme obtained
	{
	  Trimmed_D.insert(LMD_Selected);
	}
    }// else of lme is not selected

  if(false)
    {

      DAGNode *root = NULL;
      set<DAGNode*>::iterator it=E.begin();
      for(; it!=E.end(); it++)
	if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
      it=D.begin();
      for(; it!=D.end(); it++)
	if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

      cout<<"\nContext\n************\n";
      if(root==NULL)
	{
	  cout<<"\nNULL\n";
	}
      else
	{
	  cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
	}


      root = NULL;
      it=Trimmed_E.begin();
      for(; it!=Trimmed_E.end(); it++)
	if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
      it=Trimmed_D.begin();
      for(; it!=Trimmed_D.end(); it++)
	if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

      cout<<"\nTrimmed Context\n************\n";
      if(root==NULL)
	{
	  cout<<"\nNULL\n";
	}
      else
	{
	  cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
	}
    }


}

// show the context
void DDManager::showLMDEContext(set<DAGNode*> &Context)
{
  if(Context.empty())
    {
      cout<<"\nContext is empty\n";
      return;
    }

 DAGNode *root = NULL;
 set<DAGNode*>::iterator it=Context.begin();
 for(; it!=Context.end(); it++)
   {
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
   }

 cout<<endl<<getDAGStr(root)<<endl;
}

// show a set of LMEs and a set of LMDs
void DDManager::showLMEAndLMDContext(set<DAGNode*> &E, set<DAGNode*> &D)
{
  if(E.empty() && D.empty())
    {
      cout<<"\nContext is empty\n";
      return;
    }

 DAGNode *root = NULL;
 set<DAGNode*>::iterator it=E.begin();
 for(; it!=E.end(); it++)
   {
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
   }
 it=D.begin();
 for(; it!=D.end(); it++)
   {
   if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
   }
 

 cout<<endl<<getDAGStr(root)<<endl;
}


// get entry from the efficient local hash table for binary operation
// return NULL if there is no entry
DDNode* DDManager::getEntryFromEfficientLocalHashTableBinaryOperation(map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable, DDNode *v1, DDNode *v2, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &Useful_Context)
{
  if(v1->getPredicate()== NULL && v2->getPredicate()== NULL) // both are terminals
    {
      return NULL;
    }

time_t start, end, duration;
time(&start);


DDNode *resultDD=NULL;


 string key=""; 
 string temp_key;
 char temp_char[10];

 sprintf(temp_char , "%x", v1); // first argument DD
 temp_key = temp_char;
 key += temp_key;

 sprintf(temp_char , "%x", v2); // second argument DD
 temp_key = temp_char;
 key += temp_key;

  
map<string, map<set<DAGNode*>, DDNode*> >::iterator pit=PtrLocalHashTable->find(key);

if(pit==PtrLocalHashTable->end()) // no entry for (v1, v2)
   {
     return NULL;
   }

set<DAGNode*> Context;
set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin())); 

 map<set<DAGNode*>, DDNode*> Items = pit->second;

for(map<set<DAGNode*>, DDNode*>::iterator it = Items.begin();it!=Items.end();it++)
   {
     // take each entry
     set<DAGNode*> Present_Useful_Context = it->first;
     
     
     if(Present_Useful_Context.empty())
       {
	 applystepwithimpcheck_local_cache_trivial_hit++;
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
     else if(isSubSet(Present_Useful_Context, Context))
       {
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
   }

 time(&end);
 duration = end-start;
 time_in_local_hashing_in_applystepwithimpcheck = time_in_local_hashing_in_applystepwithimpcheck + duration;



 return resultDD;
}


// get entry from the efficient local hash table for ternary operation
// return NULL if there is no entry
DDNode* DDManager::getEntryFromEfficientLocalHashTableTernaryOperation(map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable, DDNode *condn, DDNode *then, DDNode *els, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &Useful_Context)
{
  if(condn->getPredicate()== NULL && then->getPredicate()== NULL && els->getPredicate()== NULL) // all are terminals
    {
      return NULL;
    }

time_t start, end, duration;
time(&start);


DDNode *resultDD=NULL;


 string key=""; 
 string temp_key;
 char temp_char[10];

 sprintf(temp_char , "%x", condn); // first argument DD
 temp_key = temp_char;
 key += temp_key;

 sprintf(temp_char , "%x", then); // second argument DD
 temp_key = temp_char;
 key += temp_key;

 sprintf(temp_char , "%x", els); // third argument DD
 temp_key = temp_char;
 key += temp_key;

  
map<string, map<set<DAGNode*>, DDNode*> >::iterator pit=PtrLocalHashTable->find(key);

if(pit==PtrLocalHashTable->end()) // no entry for (v1, v2)
   {
     return NULL;
   }

set<DAGNode*> Context;
set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Context, Context.begin())); 

 map<set<DAGNode*>, DDNode*> Items = pit->second;

for(map<set<DAGNode*>, DDNode*>::iterator it = Items.begin();it!=Items.end();it++)
   {
     // take each entry
     set<DAGNode*> Present_Useful_Context = it->first;
     
     
     if(Present_Useful_Context.empty())
       {
	 applystepwithimpcheck_local_cache_trivial_hit++;
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
     else if(isSubSet(Present_Useful_Context, Context))
       {
	 Useful_Context = Present_Useful_Context;
	 resultDD = it->second;
	 break;
       }
   }

 time(&end);
 duration = end-start;
 time_in_local_hashing_in_applystepwithimpcheck = time_in_local_hashing_in_applystepwithimpcheck + duration;



 return resultDD;
}

// insert into the efficient local hash table for binary operation
void DDManager::insertIntoEfficientLocalHashTableBinaryOperation(map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable, DDNode *v1, DDNode *v2, set<DAGNode*> &Useful_Context, DDNode *result)
{

 string key=""; 
 string temp_key;
 char temp_char[10];

 sprintf(temp_char , "%x", v1); // first argument DD
 temp_key = temp_char;
 key += temp_key;

 sprintf(temp_char , "%x", v2); // second argument DD
 temp_key = temp_char;
 key += temp_key;

  
map<string, map<set<DAGNode*>, DDNode*> >::iterator pit=PtrLocalHashTable->find(key);

if(pit==PtrLocalHashTable->end()) // no entry for (v1, v2). Hence make map<pair<Useful_Context, result>> and make it (key --> map<pair<Useful_Context, result>>)
   {
     map<set<DAGNode*>, DDNode*> Items;
     Items.insert(make_pair(Useful_Context, result));
     PtrLocalHashTable->insert(make_pair(key, Items));
   }
 else
   {
     (pit->second).insert(make_pair(Useful_Context, result));
   }
 
}

// insert into the efficient local hash table for ternary operation
void DDManager::insertIntoEfficientLocalHashTableTernaryOperation(map<string, map<set<DAGNode*>, DDNode*> > *PtrLocalHashTable, DDNode *condn, DDNode *then, DDNode *els, set<DAGNode*> &Useful_Context, DDNode *result)
{

 string key=""; 
 string temp_key;
 char temp_char[10];

 sprintf(temp_char , "%x", condn); // first argument DD
 temp_key = temp_char;
 key += temp_key;

 sprintf(temp_char , "%x", then); // second argument DD
 temp_key = temp_char;
 key += temp_key;

 sprintf(temp_char , "%x", els); // third argument DD
 temp_key = temp_char;
 key += temp_key;

  
map<string, map<set<DAGNode*>, DDNode*> >::iterator pit=PtrLocalHashTable->find(key);

if(pit==PtrLocalHashTable->end()) // no entry. Hence make map<pair<Useful_Context, result>> and make it (key --> map<pair<Useful_Context, result>>)
   {
     map<set<DAGNode*>, DDNode*> Items;
     Items.insert(make_pair(Useful_Context, result));
     PtrLocalHashTable->insert(make_pair(key, Items));
   }
 else
   {
     (pit->second).insert(make_pair(Useful_Context, result));
   }
 
}

// Function to trim down variables to be eliminated
void DDManager::trimVarSetToElim(set<string> &VarSetToElim)
{
set<string> VarSetToElimTemp;
set<string>::iterator it;
for(it=VarSetToElim.begin(); it!=VarSetToElim.end(); it++)
	{
	string variable = *it;
        int index = variable.find_last_of("_");
      	if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function DDManager::trimVarSetToElim\n";
		    exit(1);
		  }
      string id = variable.substr(0, index);
      cout<<id[0]<<endl;
      if(id[0]!='x' && id[0]!='y' && id!="clk")
	{
        cout<<endl<<"inserted\n";
	VarSetToElimTemp.insert(variable);
	}
       }
VarSetToElim.clear();
for(it=VarSetToElimTemp.begin(); it!=VarSetToElimTemp.end(); it++)
	{
	string variable = *it;
	VarSetToElim.insert(variable);
       }
}
	


// Trim down set of LMEs E and set of LMDs D to useful context : better version
void DDManager::trimContextStrategyFour(set<DAGNode*> E, set<DAGNode*> D, DAGNode *u_AP, set<DAGNode*> &Trimmed_E, set<DAGNode*> &Trimmed_D, map<string, int> &WidthTable)
{
  time_t trimming_start, trimming_end, trimming_duration;
  time(&trimming_start);


  trim_context_strategy_four_times++;

  bool level_1_trimming=true; // easiest way of trimming. EXACT
  bool level_2_trimming=false; // back track from the consequent to find the LMEs and bit-level LMDs affecting the consequent until either a f.p. is reached or limit in number of steps is exceeded. Successful if f.p. is reached, else we will not trim; less effective. EXACT
  bool level_2_trimming_mode2=false; // back track from the consequent to find the LMEs and LMDs affecting the consequent until either a f.p. is reached or limit in number of steps is exceeded. Successful if f.p. is reached, else we will not trim; (while selecting LMDs, select a single LMD). INEXACT; this is also not much effective - it increases the DD sizes as it over-approximates the consequent of impchecks; larger dd means more impchecks; but the impchecks are in general costly; hence more time

  bool level_2_trimming_mode3=true; // back track from the consequent to find the LMEs and LMDs affecting the consequent until either a f.p. is reached or limit in number of steps is exceeded. Successful if f.p. is reached, else we will reduce the context to empty set (while selecting LMDs, select a single LMD). EXTREMELY INEXACT

  bool level_2_trimming_mode4=false; // back track from the consequent to find the LMEs and LMDs affecting the consequent until either a f.p. is reached or limit in number of steps is exceeded. Successful if f.p. is reached, else we will reduce the context to empty set (while selecting LMDs, select all LMDs). INEXACT

  bool level_1_trimming_applied=false;
// Initially trim down using class based strategy

int Class_Consequent=findEquivalenceClass(u_AP); 

for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
  {
    int Class_LME=findEquivalenceClass(*it);

    if(Class_Consequent==Class_LME)
      {
	Trimmed_E.insert(*it);
      }
  }

for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
  {
    int Class_LMD=findEquivalenceClass(*it);

    if(Class_Consequent==Class_LMD)
      {
	Trimmed_D.insert(*it);
      }
    
  }

// Trimmed context using class based strategy in Trimmed_E, Trimmed_D

 if(level_1_trimming)
   {


     list<DAGNode*>::iterator iout;
     iout = u_AP->Get_outlist_start();
     DAGNode *lhs_u_AP = *iout;
     iout++;
     DAGNode *rhs_u_AP = *iout;

     // Check if there exists an LME e in Trimmed_E such that the lhs_e = lhs_u_AP. If yes, the problem is solved. return
     DAGNode *LME_Selected=NULL;
     for(set<DAGNode*>::iterator it=Trimmed_E.begin(); it!=Trimmed_E.end(); it++)
       {
	 list<DAGNode*>::iterator eiout;
	 eiout = (*it)->Get_outlist_start();
	 DAGNode *lhs_e = *eiout;
	 eiout++;
	 DAGNode *rhs_e = *eiout;
    
	 if(lhs_u_AP==lhs_e)// lhs's match!!
	   {
	     LME_Selected=*it;
	     break;
	   }
       }
 
     if(LME_Selected!=NULL) // lme obtained
       {
	 // existing Trimmed_E, Trimmed_D are not required
	 Trimmed_E.clear();
	 Trimmed_D.clear();
	 Trimmed_E.insert(LME_Selected);
	 level_1_trimming_applied=true;
       }
     else // no lme selected
       {
	 DAGNode *LMD_Selected=NULL;
	 // Check if there exists an LMD d such that lhs_d = lhs_u_AP and d is at bit-level.  If yes, the problem is solved. return
	 for(set<DAGNode*>::iterator it=Trimmed_D.begin(); it!=Trimmed_D.end(); it++)
	   {
	     list<DAGNode*>::iterator diout;
	     diout = (*it)->Get_outlist_start();
	     DAGNode *lhs_d = *diout;
	     diout++;
	     DAGNode *rhs_d = *diout;
    
	     if(lhs_u_AP==lhs_d)// lhs's match!!
	       {
		 string variable=getSingleVariable(lhs_u_AP);
		 if(variable=="")
		   {
		     cout<<"\nLME/LMD "<<getDAGStr(lhs_u_AP)<<" encountered with no variable names in function DDManager::trimContextStrategyFour\n";
		     exit(1);
		   }
		 map<string, int>::iterator wit = WidthTable.find(variable);
		 if(wit==WidthTable.end())
		   {
		     cout<<"\nvariable "<<variable<<" encountered with no width in function DDManager::trimContextStrategyFour\n";
		     exit(1);
		   }
		 if(wit->second==1) // lhs_d <> rhs_d is a bit-level constraint. Hence effectively it is an LME
		   {
		     LMD_Selected=*it;
		     break;
		   }
	       }// if(lhs_u_AP==lhs_d)
	   }// for each LMD

	 if(LMD_Selected!=NULL) // lmd obtained
	   {
	     // existing Trimmed_E, Trimmed_D are not required
	     Trimmed_E.clear();
	     Trimmed_D.clear();
	     Trimmed_D.insert(LMD_Selected);
	     level_1_trimming_applied=true;
	   }
       }// else of lme is not selected
   }// level 1 trimming ends here


 set<DAGNode*> Trimmed_E_2;
 set<DAGNode*> Trimmed_D_2;

 bool level_2_trimming_applied=false;
 if(level_2_trimming && !level_1_trimming_applied)
   {

     map<DAGNode*, set<string> > DAGToVarsMap;
     map<string, set<DAGNode*> > VarToLMEsMap;
     map<string, set<DAGNode*> > VarToLMDsMap;

     for(set<DAGNode*>::iterator it=Trimmed_E.begin(); it!=Trimmed_E.end(); it++)
       {
	 DAGNode* lme = *it;
	 set<string> Support;
	 getDAGNames(lme, Support);
	 DAGToVarsMap.insert(make_pair(lme, Support));
	 
	 for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	   {
	     string var=*sit;
	     map<string, set<DAGNode*> >::iterator vemapit = VarToLMEsMap.find(var);
	     if(vemapit==VarToLMEsMap.end()) // no entry for var
	       {
		 set<DAGNode*> LMEs;
		 LMEs.insert(lme);
		 VarToLMEsMap.insert(make_pair(var, LMEs));
	       }
	     else // entry already exists for var
	       {
		 (vemapit->second).insert(lme);
	       }
	   }
       }// for each lme
     
     for(set<DAGNode*>::iterator it=Trimmed_D.begin(); it!=Trimmed_D.end(); it++)
       {
	 DAGNode* lmd = *it;
	 
	 string variable=getSingleVariable(lmd);
	 if(variable=="")
	   {
	     cout<<"\nLMD "<<getDAGStr(lmd)<<" encountered with no variable names in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 map<string, int>::iterator wit = WidthTable.find(variable);
	 if(wit==WidthTable.end())
	   {
	     cout<<"\nvariable "<<variable<<" encountered with no width in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 if(wit->second!=1) // lmd is not a bit-level lmd
	   {
	     continue;
	   }
	 
	 set<string> Support;
	 getDAGNames(lmd, Support);
	 DAGToVarsMap.insert(make_pair(lmd, Support));
	 
	 for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	   {
	     string var=*sit;
	     map<string, set<DAGNode*> >::iterator vdmapit = VarToLMDsMap.find(var);
	     if(vdmapit==VarToLMDsMap.end()) // no entry for var
	       {
		 set<DAGNode*> LMDs;
		 LMDs.insert(lmd);
		 VarToLMDsMap.insert(make_pair(var, LMDs));
	       }
	     else // entry already exists for var
	       {
		 (vdmapit->second).insert(lmd);
	       }
	   }
       }// for each lmd

     bool show_maps=false;
     if(show_maps)
       {

	 DAGNode* root = NULL;
	 set<DAGNode*>::iterator eit=Trimmed_E.begin();
	 for(; eit!=Trimmed_E.end(); eit++)
	   if(root==NULL) root=*eit; else root=createDAG("and", root, *eit, LOGICAL_SYMBOL, 1);
	 set<DAGNode*>::iterator dit=Trimmed_D.begin();
	 for(; dit!=Trimmed_D.end(); dit++)
	   if(root==NULL) root=*dit; else root=createDAG("and", root, *dit, LOGICAL_SYMBOL, 1);

	 cout<<"\nTrimmed Context\n************\n";
	 if(root==NULL)
	   {
	     cout<<"\nNULL\n";
	   }
	 else
	   {
	     cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
	   }

	 cout<<"\nDAGToVarsMap\n";
	 for(map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.begin();dvmapit!=DAGToVarsMap.end();dvmapit++)
	   {
	     cout<<endl;
	     cout<<getDAGStr(dvmapit->first);
	     showSet(dvmapit->second,"");
	   }

	 cout<<"\nVarsToLMEsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.begin();vemapit!=VarToLMEsMap.end();vemapit++)
	   {
	     cout<<endl;
	     cout<<vemapit->first;
	     showAPSet("",vemapit->second);
	   }

	 cout<<"\nVarsToLMDsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.begin();vdmapit!=VarToLMDsMap.end();vdmapit++)
	   {
	     cout<<endl;
	     cout<<vdmapit->first;
	     showAPSet("",vdmapit->second);
	   }
       }
   
     // maps are created

     set<string> Vars_In_u_AP;
     getDAGNames(u_AP, Vars_In_u_AP);
     if(Vars_In_u_AP.size()==1) // possible only if there is a single variable in the support of consequent
       {
	 set<string>::iterator vars_it=Vars_In_u_AP.begin();
	 string var = *vars_it;
	 int steps=3;
	 DAGNode* newConstraint;

	 for(int i=0;i<steps;i++)
	   {
	     newConstraint=NULL;

	     map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.find(var);
	     if(vemapit != VarToLMEsMap.end()) // entry exists for var in VarsToLMEs
	       {
		 for(set<DAGNode*>::iterator dit=(vemapit->second).begin();dit!=(vemapit->second).end();dit++)
		   {
		     DAGNode *lme = *dit; // each LME which contains var is taken
		     if(Trimmed_E_2.find(lme)==Trimmed_E_2.end()) // lme is not already added to Trimmed_E_2
		       {
			 Trimmed_E_2.insert(lme);
			 newConstraint = lme;
			 break;// lme inserted; break from loop
		       }
		   }
	       }
	     
	     if(newConstraint==NULL)
	       {
		 map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.find(var);
		 if(vdmapit != VarToLMDsMap.end()) // entry exists for var in VarsToLMDs
		   {
		     for(set<DAGNode*>::iterator dit=(vdmapit->second).begin();dit!=(vdmapit->second).end();dit++)
		       {
			 DAGNode *lmd = *dit; // each LMD which contains var is taken
			 if(Trimmed_D_2.find(lmd)==Trimmed_D_2.end()) // lmd is not already added to Trimmed_D_2
			   {
			     Trimmed_D_2.insert(lmd);
			     newConstraint = lmd;
			     break;// lmd inserted; break from loop
			   }
		       }
		   }
	       }

	     if(newConstraint==NULL) // no LME/LMD found which contains var. Hence exit from the loop
	       {
		 break;
	       }
	     
	     set<string> SupportNewConstraint;
	     map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.find(newConstraint);
	     SupportNewConstraint = dvmapit->second;

	     
	     
	     if(SupportNewConstraint.size()==1) // no more friends
	       {
		 level_2_trimming_applied=true;
		 break;
	       }
	     
	     // friend exists. find it
	     string my_friend="";
	     set<string>::iterator sit = SupportNewConstraint.begin();
	     if(*sit == var)
	       {
		 sit++;
		 my_friend = *sit;
	       }
	     else
	       {
		 my_friend = *sit;
	       }

	     var = my_friend; // seeding the next iteration
	   }// for loop ends
       }// if(Vars_In_u_AP.size()==1) ends here

   
     
     if(level_2_trimming_applied)
       {
	 //cout<<"\nLevel 2 trimming applied\n";
	 // Trimmed_E_2, Trimmed_D_2 are created which needs to be copied into Trimmed_E, Trimmed_D respectively
	 // There is an advantage of level_2 trimming if Trimmed_E_2 or Trimmed_D_2 is smaller than Trimmed_E or Trimmed_D respectively
	 
	 if(Trimmed_E_2.size() < Trimmed_E.size() || Trimmed_D_2.size() < Trimmed_D.size())
	   {
	     level_2_trimming_effect++;
	   }

	 Trimmed_E.clear();
	 Trimmed_D.clear();
	 copySet(Trimmed_E, Trimmed_E_2);
	 copySet(Trimmed_D, Trimmed_D_2);
       }
     


   }// level_2_trimming



 bool level_2_trimming_mode2_applied=false;
 if(level_2_trimming_mode2 && !level_1_trimming_applied)
   {

     map<DAGNode*, set<string> > DAGToVarsMap;
     map<string, set<DAGNode*> > VarToLMEsMap;
     map<string, set<DAGNode*> > VarToLMDsMap;
     map<string, set<DAGNode*> > VarToNonBitLevelLMDsMap;

     for(set<DAGNode*>::iterator it=Trimmed_E.begin(); it!=Trimmed_E.end(); it++)
       {
	 DAGNode* lme = *it;
	 set<string> Support;
	 getDAGNames(lme, Support);
	 DAGToVarsMap.insert(make_pair(lme, Support));
	 
	 for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	   {
	     string var=*sit;
	     map<string, set<DAGNode*> >::iterator vemapit = VarToLMEsMap.find(var);
	     if(vemapit==VarToLMEsMap.end()) // no entry for var
	       {
		 set<DAGNode*> LMEs;
		 LMEs.insert(lme);
		 VarToLMEsMap.insert(make_pair(var, LMEs));
	       }
	     else // entry already exists for var
	       {
		 (vemapit->second).insert(lme);
	       }
	   }
       }// for each lme
     
     for(set<DAGNode*>::iterator it=Trimmed_D.begin(); it!=Trimmed_D.end(); it++)
       {
	 DAGNode* lmd = *it;
	 
	 string variable=getSingleVariable(lmd);
	 if(variable=="")
	   {
	     cout<<"\nLMD "<<getDAGStr(lmd)<<" encountered with no variable names in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 map<string, int>::iterator wit = WidthTable.find(variable);
	 if(wit==WidthTable.end())
	   {
	     cout<<"\nvariable "<<variable<<" encountered with no width in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 if(wit->second!=1) // lmd is not a bit-level lmd
	   {
	     set<string> Support;
	     getDAGNames(lmd, Support);
	     DAGToVarsMap.insert(make_pair(lmd, Support));
	 
	     for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	       {
		 string var=*sit;
		 map<string, set<DAGNode*> >::iterator vdmapit = VarToNonBitLevelLMDsMap.find(var);
		 if(vdmapit==VarToNonBitLevelLMDsMap.end()) // no entry for var
		   {
		     set<DAGNode*> LMDs;
		     LMDs.insert(lmd);
		     VarToNonBitLevelLMDsMap.insert(make_pair(var, LMDs));
		   }
		 else // entry already exists for var
		   {
		     (vdmapit->second).insert(lmd);
		   }
	       }
 
	   }
	 else // lmd is a bit-level lmd
	   {
	     set<string> Support;
	     getDAGNames(lmd, Support);
	     DAGToVarsMap.insert(make_pair(lmd, Support));
	 
	     for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	       {
		 string var=*sit;
		 map<string, set<DAGNode*> >::iterator vdmapit = VarToLMDsMap.find(var);
		 if(vdmapit==VarToLMDsMap.end()) // no entry for var
		   {
		     set<DAGNode*> LMDs;
		     LMDs.insert(lmd);
		     VarToLMDsMap.insert(make_pair(var, LMDs));
		   }
		 else // entry already exists for var
		   {
		     (vdmapit->second).insert(lmd);
		   }
	       }
	   }
       }// for each lmd

     bool show_maps=false;
     if(show_maps)
       {

	 DAGNode* root = NULL;
	 set<DAGNode*>::iterator eit=Trimmed_E.begin();
	 for(; eit!=Trimmed_E.end(); eit++)
	   if(root==NULL) root=*eit; else root=createDAG("and", root, *eit, LOGICAL_SYMBOL, 1);
	 set<DAGNode*>::iterator dit=Trimmed_D.begin();
	 for(; dit!=Trimmed_D.end(); dit++)
	   if(root==NULL) root=*dit; else root=createDAG("and", root, *dit, LOGICAL_SYMBOL, 1);

	 cout<<"\nTrimmed Context\n************\n";
	 if(root==NULL)
	   {
	     cout<<"\nNULL\n";
	   }
	 else
	   {
	     cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
	   }

	 cout<<"\nDAGToVarsMap\n";
	 for(map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.begin();dvmapit!=DAGToVarsMap.end();dvmapit++)
	   {
	     cout<<endl;
	     cout<<getDAGStr(dvmapit->first);
	     showSet(dvmapit->second,"");
	   }

	 cout<<"\nVarsToLMEsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.begin();vemapit!=VarToLMEsMap.end();vemapit++)
	   {
	     cout<<endl;
	     cout<<vemapit->first;
	     showAPSet("",vemapit->second);
	   }

	 cout<<"\nVarsToLMDsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.begin();vdmapit!=VarToLMDsMap.end();vdmapit++)
	   {
	     cout<<endl;
	     cout<<vdmapit->first;
	     showAPSet("",vdmapit->second);
	   }

	 cout<<"\nVarsToNonBitLevelLMDsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vdmapit=VarToNonBitLevelLMDsMap.begin();vdmapit!=VarToNonBitLevelLMDsMap.end();vdmapit++)
	   {
	     cout<<endl;
	     cout<<vdmapit->first;
	     showAPSet("",vdmapit->second);
	   }
       }
   
     // maps are created

     set<string> Vars_In_u_AP;
     getDAGNames(u_AP, Vars_In_u_AP);
     if(Vars_In_u_AP.size()==1) // possible only if there is a single variable in the support of consequent
       {
	 set<string>::iterator vars_it=Vars_In_u_AP.begin();
	 string var = *vars_it;
	 int steps=3;
	 DAGNode* newConstraint;

	 for(int i=0;i<steps;i++)
	   {
	     newConstraint=NULL;

	     map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.find(var);
	     if(vemapit != VarToLMEsMap.end()) // entry exists for var in VarsToLMEs
	       {
		 for(set<DAGNode*>::iterator dit=(vemapit->second).begin();dit!=(vemapit->second).end();dit++)
		   {
		     DAGNode *lme = *dit; // each LME which contains var is taken
		     if(Trimmed_E_2.find(lme)==Trimmed_E_2.end()) // lme is not already added to Trimmed_E_2
		       {
			 Trimmed_E_2.insert(lme);
			 newConstraint = lme;
			 break;// lme inserted; break from loop
		       }
		   }
	       }
	     
	     if(newConstraint==NULL)
	       {
		 map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.find(var);
		 if(vdmapit != VarToLMDsMap.end()) // entry exists for var in VarsToLMDs
		   {
		     for(set<DAGNode*>::iterator dit=(vdmapit->second).begin();dit!=(vdmapit->second).end();dit++)
		       {
			 DAGNode *lmd = *dit; // each LMD which contains var is taken
			 if(Trimmed_D_2.find(lmd)==Trimmed_D_2.end()) // lmd is not already added to Trimmed_D_2
			   {
			     Trimmed_D_2.insert(lmd);
			     newConstraint = lmd;
			     break;// lmd inserted; break from loop
			   }
		       }
		   }
	       }

	     if(newConstraint==NULL)
	       {
		 map<string, set<DAGNode*> >::iterator vdmapit=VarToNonBitLevelLMDsMap.find(var);
		 if(vdmapit != VarToNonBitLevelLMDsMap.end()) // entry exists for var in VarsToNonBitLevelLMDs
		   {
		     for(set<DAGNode*>::iterator dit=(vdmapit->second).begin();dit!=(vdmapit->second).end();dit++)
		       {
			 DAGNode *lmd = *dit; // each LMD which contains var is taken
			 if(Trimmed_D_2.find(lmd)==Trimmed_D_2.end()) // lmd is not already added to Trimmed_D_2
			   {
			     Trimmed_D_2.insert(lmd);
			     newConstraint = lmd;
			     break;// lmd inserted; break from loop
			   }
		       }
		   }
	       }


	     if(newConstraint==NULL) // no LME/LMD found which contains var. Hence exit from the loop
	       {
		 break;
	       }
	     
	     set<string> SupportNewConstraint;
	     map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.find(newConstraint);
	     SupportNewConstraint = dvmapit->second;

	     
	     
	     if(SupportNewConstraint.size()==1) // no more friends
	       {
		 level_2_trimming_mode2_applied=true;
		 break;
	       }
	     
	     // friend exists. find it
	     string my_friend="";
	     set<string>::iterator sit = SupportNewConstraint.begin();
	     if(*sit == var)
	       {
		 sit++;
		 my_friend = *sit;
	       }
	     else
	       {
		 my_friend = *sit;
	       }

	     var = my_friend; // seeding the next iteration
	   }// for loop ends
       }// if(Vars_In_u_AP.size()==1) ends here

   
     
     if(level_2_trimming_mode2_applied)
       {
	 //cout<<"\nLevel 2 trimming applied\n";
	 // Trimmed_E_2, Trimmed_D_2 are created which needs to be copied into Trimmed_E, Trimmed_D respectively
	 // There is an advantage of level_2 trimming if Trimmed_E_2 or Trimmed_D_2 is smaller than Trimmed_E or Trimmed_D respectively
	 
	 if(Trimmed_E_2.size() < Trimmed_E.size() || Trimmed_D_2.size() < Trimmed_D.size())
	   {
	     level_2_trimming_effect++;
	   }

	 Trimmed_E.clear();
	 Trimmed_D.clear();
	 copySet(Trimmed_E, Trimmed_E_2);
	 copySet(Trimmed_D, Trimmed_D_2);
       }
     


   }// level_2_trimming_mode2


 bool level_2_trimming_mode3_applied=false;
 if(level_2_trimming_mode3 && !level_1_trimming_applied)
   {

     map<DAGNode*, set<string> > DAGToVarsMap;
     map<string, set<DAGNode*> > VarToLMEsMap;
     map<string, set<DAGNode*> > VarToLMDsMap;
     map<string, set<DAGNode*> > VarToNonBitLevelLMDsMap;

     for(set<DAGNode*>::iterator it=Trimmed_E.begin(); it!=Trimmed_E.end(); it++)
       {
	 DAGNode* lme = *it;
	 set<string> Support;
	 getDAGNames(lme, Support);
	 DAGToVarsMap.insert(make_pair(lme, Support));
	 
	 for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	   {
	     string var=*sit;
	     map<string, set<DAGNode*> >::iterator vemapit = VarToLMEsMap.find(var);
	     if(vemapit==VarToLMEsMap.end()) // no entry for var
	       {
		 set<DAGNode*> LMEs;
		 LMEs.insert(lme);
		 VarToLMEsMap.insert(make_pair(var, LMEs));
	       }
	     else // entry already exists for var
	       {
		 (vemapit->second).insert(lme);
	       }
	   }
       }// for each lme
     
     for(set<DAGNode*>::iterator it=Trimmed_D.begin(); it!=Trimmed_D.end(); it++)
       {
	 DAGNode* lmd = *it;
	 
	 string variable=getSingleVariable(lmd);
	 if(variable=="")
	   {
	     cout<<"\nLMD "<<getDAGStr(lmd)<<" encountered with no variable names in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 map<string, int>::iterator wit = WidthTable.find(variable);
	 if(wit==WidthTable.end())
	   {
	     cout<<"\nvariable "<<variable<<" encountered with no width in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 if(wit->second!=1) // lmd is not a bit-level lmd
	   {
	     set<string> Support;
	     getDAGNames(lmd, Support);
	     DAGToVarsMap.insert(make_pair(lmd, Support));
	 
	     for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	       {
		 string var=*sit;
		 map<string, set<DAGNode*> >::iterator vdmapit = VarToNonBitLevelLMDsMap.find(var);
		 if(vdmapit==VarToNonBitLevelLMDsMap.end()) // no entry for var
		   {
		     set<DAGNode*> LMDs;
		     LMDs.insert(lmd);
		     VarToNonBitLevelLMDsMap.insert(make_pair(var, LMDs));
		   }
		 else // entry already exists for var
		   {
		     (vdmapit->second).insert(lmd);
		   }
	       }
 
	   }
	 else // lmd is a bit-level lmd
	   {
	     set<string> Support;
	     getDAGNames(lmd, Support);
	     DAGToVarsMap.insert(make_pair(lmd, Support));
	 
	     for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	       {
		 string var=*sit;
		 map<string, set<DAGNode*> >::iterator vdmapit = VarToLMDsMap.find(var);
		 if(vdmapit==VarToLMDsMap.end()) // no entry for var
		   {
		     set<DAGNode*> LMDs;
		     LMDs.insert(lmd);
		     VarToLMDsMap.insert(make_pair(var, LMDs));
		   }
		 else // entry already exists for var
		   {
		     (vdmapit->second).insert(lmd);
		   }
	       }
	   }
       }// for each lmd

     bool show_maps=false;
     if(show_maps)
       {

	 DAGNode* root = NULL;
	 set<DAGNode*>::iterator eit=Trimmed_E.begin();
	 for(; eit!=Trimmed_E.end(); eit++)
	   if(root==NULL) root=*eit; else root=createDAG("and", root, *eit, LOGICAL_SYMBOL, 1);
	 set<DAGNode*>::iterator dit=Trimmed_D.begin();
	 for(; dit!=Trimmed_D.end(); dit++)
	   if(root==NULL) root=*dit; else root=createDAG("and", root, *dit, LOGICAL_SYMBOL, 1);

	 cout<<"\nTrimmed Context\n************\n";
	 if(root==NULL)
	   {
	     cout<<"\nNULL\n";
	   }
	 else
	   {
	     cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
	   }

	 cout<<"\nDAGToVarsMap\n";
	 for(map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.begin();dvmapit!=DAGToVarsMap.end();dvmapit++)
	   {
	     cout<<endl;
	     cout<<getDAGStr(dvmapit->first);
	     showSet(dvmapit->second,"");
	   }

	 cout<<"\nVarsToLMEsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.begin();vemapit!=VarToLMEsMap.end();vemapit++)
	   {
	     cout<<endl;
	     cout<<vemapit->first;
	     showAPSet("",vemapit->second);
	   }

	 cout<<"\nVarsToLMDsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.begin();vdmapit!=VarToLMDsMap.end();vdmapit++)
	   {
	     cout<<endl;
	     cout<<vdmapit->first;
	     showAPSet("",vdmapit->second);
	   }

	 cout<<"\nVarsToNonBitLevelLMDsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vdmapit=VarToNonBitLevelLMDsMap.begin();vdmapit!=VarToNonBitLevelLMDsMap.end();vdmapit++)
	   {
	     cout<<endl;
	     cout<<vdmapit->first;
	     showAPSet("",vdmapit->second);
	   }
       }
   
     // maps are created

     set<string> Vars_In_u_AP;
     getDAGNames(u_AP, Vars_In_u_AP);
     if(Vars_In_u_AP.size()==1) // possible only if there is a single variable in the support of consequent
       {
	 set<string>::iterator vars_it=Vars_In_u_AP.begin();
	 string var = *vars_it;
	 int steps=3;
	 DAGNode* newConstraint;

	 for(int i=0;i<steps;i++)
	   {
	     newConstraint=NULL;

	     map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.find(var);
	     if(vemapit != VarToLMEsMap.end()) // entry exists for var in VarsToLMEs
	       {
		 for(set<DAGNode*>::iterator dit=(vemapit->second).begin();dit!=(vemapit->second).end();dit++)
		   {
		     DAGNode *lme = *dit; // each LME which contains var is taken
		     if(Trimmed_E_2.find(lme)==Trimmed_E_2.end()) // lme is not already added to Trimmed_E_2
		       {
			 Trimmed_E_2.insert(lme);
			 newConstraint = lme;
			 break;// lme inserted; break from loop
		       }
		   }
	       }
	     
	     if(newConstraint==NULL)
	       {
		 map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.find(var);
		 if(vdmapit != VarToLMDsMap.end()) // entry exists for var in VarsToLMDs
		   {
		     for(set<DAGNode*>::iterator dit=(vdmapit->second).begin();dit!=(vdmapit->second).end();dit++)
		       {
			 DAGNode *lmd = *dit; // each LMD which contains var is taken
			 if(Trimmed_D_2.find(lmd)==Trimmed_D_2.end()) // lmd is not already added to Trimmed_D_2
			   {
			     Trimmed_D_2.insert(lmd);
			     newConstraint = lmd;
			     break;// lmd inserted; break from loop
			   }
		       }
		   }
	       }

	     if(newConstraint==NULL)
	       {
		 map<string, set<DAGNode*> >::iterator vdmapit=VarToNonBitLevelLMDsMap.find(var);
		 if(vdmapit != VarToNonBitLevelLMDsMap.end()) // entry exists for var in VarsToNonBitLevelLMDs
		   {
		     for(set<DAGNode*>::iterator dit=(vdmapit->second).begin();dit!=(vdmapit->second).end();dit++)
		       {
			 DAGNode *lmd = *dit; // each LMD which contains var is taken
			 if(Trimmed_D_2.find(lmd)==Trimmed_D_2.end()) // lmd is not already added to Trimmed_D_2
			   {
			     Trimmed_D_2.insert(lmd);
			     newConstraint = lmd;
			     break;// lmd inserted; break from loop
			   }
		       }
		   }
	       }


	     if(newConstraint==NULL) // no LME/LMD found which contains var. Hence exit from the loop
	       {
		 break;
	       }
	     
	     set<string> SupportNewConstraint;
	     map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.find(newConstraint);
	     SupportNewConstraint = dvmapit->second;

	     
	     
	     if(SupportNewConstraint.size()==1) // no more friends
	       {
		 level_2_trimming_mode3_applied=true;
		 break;
	       }
	     
	     // friend exists. find it
	     string my_friend="";
	     set<string>::iterator sit = SupportNewConstraint.begin();
	     if(*sit == var)
	       {
		 sit++;
		 my_friend = *sit;
	       }
	     else
	       {
		 my_friend = *sit;
	       }

	     var = my_friend; // seeding the next iteration
	   }// for loop ends
       }// if(Vars_In_u_AP.size()==1) ends here

   
     
     if(level_2_trimming_mode3_applied) // level 2 mode 3 is applicable : hence copy Trimmed_E_2, Trimmed_D_2 to Trimmed_E, Trimmed_D respectively
       {
	 //cout<<"\nLevel 2 trimming applied\n";
	 // Trimmed_E_2, Trimmed_D_2 are created which needs to be copied into Trimmed_E, Trimmed_D respectively
	 // There is an advantage of level_2 trimming if Trimmed_E_2 or Trimmed_D_2 is smaller than Trimmed_E or Trimmed_D respectively
	 
	 if(Trimmed_E_2.size() < Trimmed_E.size() || Trimmed_D_2.size() < Trimmed_D.size())
	   {
	     level_2_trimming_effect++;
	   }

	 Trimmed_E.clear();
	 Trimmed_D.clear();
	 copySet(Trimmed_E, Trimmed_E_2);
	 copySet(Trimmed_D, Trimmed_D_2);
       }
     else // level 2 mode 3 is not applicable : hence clear Trimmed_E, Trimmed_D
       {
	 Trimmed_E.clear();
	 Trimmed_D.clear();
       }
     


   }// level_2_trimming_mode3




 bool level_2_trimming_mode4_applied=false;
 if(level_2_trimming_mode4 && !level_1_trimming_applied)
   {

     map<DAGNode*, set<string> > DAGToVarsMap;
     map<string, set<DAGNode*> > VarToLMEsMap;
     map<string, set<DAGNode*> > VarToLMDsMap;
     map<string, set<DAGNode*> > VarToNonBitLevelLMDsMap;

     for(set<DAGNode*>::iterator it=Trimmed_E.begin(); it!=Trimmed_E.end(); it++)
       {
	 DAGNode* lme = *it;
	 set<string> Support;
	 getDAGNames(lme, Support);
	 DAGToVarsMap.insert(make_pair(lme, Support));
	 
	 for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	   {
	     string var=*sit;
	     map<string, set<DAGNode*> >::iterator vemapit = VarToLMEsMap.find(var);
	     if(vemapit==VarToLMEsMap.end()) // no entry for var
	       {
		 set<DAGNode*> LMEs;
		 LMEs.insert(lme);
		 VarToLMEsMap.insert(make_pair(var, LMEs));
	       }
	     else // entry already exists for var
	       {
		 (vemapit->second).insert(lme);
	       }
	   }
       }// for each lme
     
     for(set<DAGNode*>::iterator it=Trimmed_D.begin(); it!=Trimmed_D.end(); it++)
       {
	 DAGNode* lmd = *it;
	 
	 string variable=getSingleVariable(lmd);
	 if(variable=="")
	   {
	     cout<<"\nLMD "<<getDAGStr(lmd)<<" encountered with no variable names in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 map<string, int>::iterator wit = WidthTable.find(variable);
	 if(wit==WidthTable.end())
	   {
	     cout<<"\nvariable "<<variable<<" encountered with no width in function DDManager::trimContextStrategyFour\n";
	     exit(1);
	   }
	 if(wit->second!=1) // lmd is not a bit-level lmd
	   {
	     set<string> Support;
	     getDAGNames(lmd, Support);
	     DAGToVarsMap.insert(make_pair(lmd, Support));
	 
	     for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	       {
		 string var=*sit;
		 map<string, set<DAGNode*> >::iterator vdmapit = VarToNonBitLevelLMDsMap.find(var);
		 if(vdmapit==VarToNonBitLevelLMDsMap.end()) // no entry for var
		   {
		     set<DAGNode*> LMDs;
		     LMDs.insert(lmd);
		     VarToNonBitLevelLMDsMap.insert(make_pair(var, LMDs));
		   }
		 else // entry already exists for var
		   {
		     (vdmapit->second).insert(lmd);
		   }
	       }
 
	   }
	 else // lmd is a bit-level lmd
	   {
	     set<string> Support;
	     getDAGNames(lmd, Support);
	     DAGToVarsMap.insert(make_pair(lmd, Support));
	 
	     for(set<string>::iterator sit=Support.begin(); sit!=Support.end(); sit++)
	       {
		 string var=*sit;
		 map<string, set<DAGNode*> >::iterator vdmapit = VarToLMDsMap.find(var);
		 if(vdmapit==VarToLMDsMap.end()) // no entry for var
		   {
		     set<DAGNode*> LMDs;
		     LMDs.insert(lmd);
		     VarToLMDsMap.insert(make_pair(var, LMDs));
		   }
		 else // entry already exists for var
		   {
		     (vdmapit->second).insert(lmd);
		   }
	       }
	   }
       }// for each lmd

     bool show_maps=false;
     if(show_maps)
       {

	 DAGNode* root = NULL;
	 set<DAGNode*>::iterator eit=Trimmed_E.begin();
	 for(; eit!=Trimmed_E.end(); eit++)
	   if(root==NULL) root=*eit; else root=createDAG("and", root, *eit, LOGICAL_SYMBOL, 1);
	 set<DAGNode*>::iterator dit=Trimmed_D.begin();
	 for(; dit!=Trimmed_D.end(); dit++)
	   if(root==NULL) root=*dit; else root=createDAG("and", root, *dit, LOGICAL_SYMBOL, 1);

	 cout<<"\nTrimmed Context\n************\n";
	 if(root==NULL)
	   {
	     cout<<"\nNULL\n";
	   }
	 else
	   {
	     cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
	   }

	 cout<<"\nDAGToVarsMap\n";
	 for(map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.begin();dvmapit!=DAGToVarsMap.end();dvmapit++)
	   {
	     cout<<endl;
	     cout<<getDAGStr(dvmapit->first);
	     showSet(dvmapit->second,"");
	   }

	 cout<<"\nVarsToLMEsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.begin();vemapit!=VarToLMEsMap.end();vemapit++)
	   {
	     cout<<endl;
	     cout<<vemapit->first;
	     showAPSet("",vemapit->second);
	   }

	 cout<<"\nVarsToLMDsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.begin();vdmapit!=VarToLMDsMap.end();vdmapit++)
	   {
	     cout<<endl;
	     cout<<vdmapit->first;
	     showAPSet("",vdmapit->second);
	   }

	 cout<<"\nVarsToNonBitLevelLMDsMap\n";
	 for(map<string, set<DAGNode*> >::iterator vdmapit=VarToNonBitLevelLMDsMap.begin();vdmapit!=VarToNonBitLevelLMDsMap.end();vdmapit++)
	   {
	     cout<<endl;
	     cout<<vdmapit->first;
	     showAPSet("",vdmapit->second);
	   }
       }// for showing maps
   
     // maps are created

     set<string> MyFriends;
     getDAGNames(u_AP, MyFriends);
     int steps=3;
     set<string> AlreadyHandled;
     
     for(int i=0;i<steps;i++)
	   {
	     set<DAGNode*> newConstraints;
	     for(set<string>::iterator vars_it=MyFriends.begin();vars_it!=MyFriends.end();vars_it++)
	       {
		 bool entry_found = false;
		 string var = *vars_it;
		 
		 if(AlreadyHandled.find(var)!=AlreadyHandled.end()) // var is already handled
		   {
		     continue;
		   }
		 
		 map<string, set<DAGNode*> >::iterator vemapit=VarToLMEsMap.find(var);
		 if(vemapit != VarToLMEsMap.end()) // entry exists for var in VarsToLMEs
		   {
		 
		     for(set<DAGNode*>::iterator dit=(vemapit->second).begin();dit!=(vemapit->second).end();dit++)
		       {
			 DAGNode *lme = *dit; // each LME which contains var is taken
			 if(Trimmed_E_2.find(lme)==Trimmed_E_2.end()) // lme is not already added to Trimmed_E_2
			   {
			     Trimmed_E_2.insert(lme);
			     newConstraints.insert(lme);
			     entry_found = true;
			     break;// lme inserted; break from loop
			   }
		       }
		   }// entry exists in VarsToLMEs map
	     
		 if(!entry_found)
		   {
		     map<string, set<DAGNode*> >::iterator vdmapit=VarToLMDsMap.find(var);
		     if(vdmapit != VarToLMDsMap.end()) // entry exists for var in VarsToLMDs
		       {
			 for(set<DAGNode*>::iterator dit=(vdmapit->second).begin();dit!=(vdmapit->second).end();dit++)
			   {
			     DAGNode *lmd = *dit; // each LMD which contains var is taken
			     if(Trimmed_D_2.find(lmd)==Trimmed_D_2.end()) // lmd is not already added to Trimmed_D_2
			       {
				 Trimmed_D_2.insert(lmd);
				 newConstraints.insert(lmd);
				 entry_found = true;
				 break;// lmd inserted; break from loop
			       }
			   }
		       }// entry exists in VarsToLMDs map
		   }// if(!entry_found)

		 if(!entry_found)
		   {
		     map<string, set<DAGNode*> >::iterator vdmapit=VarToNonBitLevelLMDsMap.find(var);
		     if(vdmapit != VarToNonBitLevelLMDsMap.end()) // entry exists for var in VarsToNonBitLevelLMDs. Add all LMDs into newConstraints
		       {
			 entry_found = true;
			 for(set<DAGNode*>::iterator dit=(vdmapit->second).begin();dit!=(vdmapit->second).end();dit++)
			   {
			     DAGNode *lmd = *dit; // each LMD which contains var is taken
			     if(Trimmed_D_2.find(lmd)==Trimmed_D_2.end()) // lmd is not already added to Trimmed_D_2
			       {
				 Trimmed_D_2.insert(lmd);
				 newConstraints.insert(lmd);// lmd inserted			     
			       }
			   }
		       }// entry exists in VarsToNonBitLevelLMDs map
		   }// if(!entry_found)

		 AlreadyHandled.insert(var); // var is handled
	       }// loop taking each variable var from MyFriends ends here


	     if(newConstraints.size()==0) // no new constraint obtained. f.p. reached
	       {
		 level_2_trimming_mode4_applied=true;
		 break;
	       }

	     set<string> MyNewFriends;
	     // obtain new friends

	     for(set<DAGNode*>::iterator nc_it=newConstraints.begin(); nc_it!=newConstraints.end(); nc_it++) // take each constraint from newConstraints
	       {
		 DAGNode* newConstraint = *nc_it;
		 set<string> SupportNewConstraint;
		 map<DAGNode*, set<string> >::iterator dvmapit=DAGToVarsMap.find(newConstraint);
		 SupportNewConstraint = dvmapit->second;
		 set_union(MyNewFriends.begin(), MyNewFriends.end(), SupportNewConstraint.begin(), SupportNewConstraint.end(),inserter(MyNewFriends, MyNewFriends.begin()));
	       }// take each constraint from newConstraints

	     MyFriends = MyNewFriends;
	   }// for loop ends

   
     if(level_2_trimming_mode4_applied) // level 2 mode 4 is applicable : hence copy Trimmed_E_2, Trimmed_D_2 to Trimmed_E, Trimmed_D respectively
       {
	 //cout<<"\nLevel 2 trimming applied\n";
	 // Trimmed_E_2, Trimmed_D_2 are created which needs to be copied into Trimmed_E, Trimmed_D respectively
	 // There is an advantage of level_2 trimming if Trimmed_E_2 or Trimmed_D_2 is smaller than Trimmed_E or Trimmed_D respectively
	 
	 if(Trimmed_E_2.size() < Trimmed_E.size() || Trimmed_D_2.size() < Trimmed_D.size())
	   {
	     level_2_trimming_effect++;
	   }

	 Trimmed_E.clear();
	 Trimmed_D.clear();
	 copySet(Trimmed_E, Trimmed_E_2);
	 copySet(Trimmed_D, Trimmed_D_2);
       }
     else // level 2 mode 4 is not applicable : hence clear Trimmed_E, Trimmed_D
       {
	 Trimmed_E.clear();
	 Trimmed_D.clear();
       }

   }// level_2_trimming_mode4
     
  
	 


 if(false)
   {

     DAGNode *root = NULL;
     set<DAGNode*>::iterator it=E.begin();
     for(; it!=E.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
     it=D.begin();
     for(; it!=D.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

     cout<<"\nContext\n************\n";
     if(root==NULL)
       {
	 cout<<"\nNULL\n";
       }
     else
       {
	 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
       }


     root = NULL;
     it=Trimmed_E.begin();
     for(; it!=Trimmed_E.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
     it=Trimmed_D.begin();
     for(; it!=Trimmed_D.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

     cout<<"\nTrimmed Context\n************\n";
     if(root==NULL)
       {
	 cout<<"\nNULL\n";
       }
     else
       {
	 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
       }



     root = NULL;
     it=Trimmed_E_2.begin();
     for(; it!=Trimmed_E_2.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
     it=Trimmed_D_2.begin();
     for(; it!=Trimmed_D_2.end(); it++)
       if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

     cout<<"\nTrimmed Context II\n************\n";
     if(root==NULL)
       {
	 cout<<"\nNULL\n";
       }
     else
       {
	 cout<<endl<<getDAGStr(root)<<" => "<<getDAGStr(u_AP)<<endl;
       }
   }

 time(&trimming_end);
 trimming_duration = trimming_end-trimming_start;
 trimming_time = trimming_time + trimming_duration;



}

// Added on 17 Nov 2010 

// Copy SourceList to DestList
void DDManager::copyList(list<DAGNode*> &DestList, list<DAGNode*> &SourceList)
{
for(list<DAGNode*>::iterator it=SourceList.begin(); it != SourceList.end(); it++)
	{
	DAGNode *AP = *it;
	DestList.push_back(AP);
	}
}

// To change APList based on VarsToElim
// Note that APList determines the order of APs in APMap, which has a crucial impact on DD sizes
void DDManager::modifyAPListBasedOnVarsToElim(list<DAGNode*> &APList, set<string> &VarsToElim)
{
list<DAGNode*> OriginalAPList;

copyList(OriginalAPList, APList); // copy APList to OriginalAPList

APList.clear();

set<DAGNode*> AddedAPs;

for(list<DAGNode*>::iterator it=OriginalAPList.begin(); it != OriginalAPList.end(); it++)
	{
	DAGNode *AP = *it;
	set<string> Support;
	getDAGNames(AP, Support);
	set<string> CommonVars;
	set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(CommonVars, CommonVars.begin()));
	if(!CommonVars.empty()) // AP has some variable to be eliminated
		{
		APList.push_back(AP);
		AddedAPs.insert(AP);
		}
	}
	
for(list<DAGNode*>::iterator it=OriginalAPList.begin(); it != OriginalAPList.end(); it++)
	{
	DAGNode *AP = *it;
	if(AddedAPs.find(AP)==AddedAPs.end()) // AP is not inserted already
		{
		APList.push_back(AP);
		AddedAPs.insert(AP);
		}
	}
}

// Added on 17 Nov 2010 ends here


// Added on 20th Nov 2010

// copy set of strings in source into destination
void DDManager::copyStringSet(set<string> &dest, set<string> source)
{
  set<string>::iterator it;
  for(it=source.begin(); it!=source.end(); it++)
    dest.insert(*it);
}


// Function to get entry from the hash table for version of QuaLMoDE, which 
// tries to eliminate all the variables in a single shot (called All_Path_QElim_With_Simplifications)
DDNode* DDManager::getEntryFromTableForAll_Path_QElim_With_Simplifications_Internal(set<string> VarsToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, HashTable <string, DDNode*> *PtrTableForAll_Path_QElim_With_Simplifications_Internal)
{
  //cout<<"\nChecking if entry exists for the following call in HashTable\n";

  //showSet(VarsToElim, "VarsToElim");
  //showContext(f, E, D);
  //cout<<"\nContent(f) = "<<getDAGStr(createDAGFromDD(f))<<endl;

  // Creating the hash key

  string key="";

   for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }

  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForAll_Path_QElim_With_Simplifications_Internal->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// Function to insert an entry into the hash table for version of QuaLMoDE, which 
// tries to eliminate all the variables in a single shot (called All_Path_QElim_With_Simplifications)
void DDManager::insertEntryIntoTableForAll_Path_QElim_With_Simplifications_Internal(set<string> VarsToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, DDNode* value, HashTable <string, DDNode*> *PtrTableForAll_Path_QElim_With_Simplifications_Internal)
{

  //cout<<"\nAdding entry into HashTable\n";

  //showSet(VarsToElim, "VarsToElim");
  //showContext(f, E, D);
  //cout<<"\nContent(f) = "<<getDAGStr(createDAGFromDD(f))<<endl;
  //cout<<"\nContent(value) = "<<getDAGStr(createDAGFromDD(value))<<endl;


  // Creating the hash key

  string key="";

   for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      key += *it;
    }

  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForAll_Path_QElim_With_Simplifications_Internal->hashtable_search(key);
  
  if(result.success())
    {
      cout<<"\nInside function DDManager::insertEntryIntoTableForAll_Path_QElim_With_Simplifications_Internal. Entry already exists for this node. Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = PtrTableForAll_Path_QElim_With_Simplifications_Internal->hashtable_insert(key, value);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForAll_Path_QElim_With_Simplifications_Internal\n";
      exit(1);
    }
}


// simplifyLMDD for the procedure All_Path_QElim_With_Simplifications
DDNode* DDManager::MultiVarDAGQElim(DDNode *f, set<DAGNode*> &E, set<DAGNode*> &D, set<string> &VarsToElim, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  set<string> EliminatedVariables;

  for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      string VarToElim = *it;

      //cout<<"\nVarToElim = "<<VarToElim<<endl;

      set<DAGNode*> Free_E;
      set<DAGNode*> Bound_E;
      set<DAGNode*> Free_D;
      set<DAGNode*> Bound_D;

      getFreeAndBoundConstraints(E, VarToElim, Free_E, Bound_E, WidthTable); // Free_E : LMEs in E free of VarToElim, Bound_E : LMEs in E bound with VarToElim
      getFreeAndBoundConstraints(D, VarToElim, Free_D, Bound_D, WidthTable); // Free_D : LMDs in D free of VarToElim, Bound_D : LMDs in D bound with VarToElim

      //showAPSet("Free_E", Free_E);
      //showAPSet("Bound_E", Bound_E);
      //showAPSet("Free_D", Free_D);
      //showAPSet("Bound_D", Bound_D);


      if(DAGResolveApplicable(Bound_E, Bound_D, WidthTable))
	{
	  //cout<<"\nDAGResolve is applicable\n";

          DAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(Bound_E, Bound_D, VarToElim, WidthTable);


          if(e1 != NULL)
	    {
	      //cout<<"\nLME Selected = "<<getDAGStr(e1)<<endl;

              f = DagResolveModifiedAdvancedWithDynamicVariableOrdering(f, e1, VarToElim, WidthTable);

	      //cout<<"\nDD after DAGResolve, f = "<<getDAGStr(createDAGFromDD(f))<<endl;

	      
	      if(DDFreeOfVar(f, VarToElim))
                {
		  //cout<<"\nf free of "<<VarToElim<<endl;

		  //cout<<"\nCalling APResolve on Bound_E, Bound_D\n";

                  DDNode *temp;
                  if(ThUnsat(Bound_E, Bound_D, WidthTable)) 
		    {
		      temp = createDD(0);
		    }
                  else 
		    {
		      temp = APResolveModified(Bound_E, Bound_D, VarToElim, WidthTable, PtrTableForAPResolveModified);
		    }

		  //cout<<"\nDD from APResolve, temp = "<<getDAGStr(createDAGFromDD(temp))<<endl;

                  f = apply("and", f, temp);

		  //cout<<"\nf and temp = "<<getDAGStr(createDAGFromDD(f))<<endl;

		  E = Free_E;
		  D = Free_D;
		  EliminatedVariables.insert(VarToElim);

		  //showSet(EliminatedVariables, "EliminatedVariables");

		}// end of if(DDFreeOfVar(f, VarToElim))
	    }// end of if(e1 != NULL)
	}// end of if(DAGResolveApplicable(Bound_E, Bound_D, WidthTable))
    }// for each variable in VarsToElim

  // VarsToElim is now VarsToElim \ EliminatedVariables;

  //cout<<"\nExited from the loop\n";

  set<string> NewVarsToElim;
  
  set_difference(VarsToElim.begin(), VarsToElim.end(), EliminatedVariables.begin(), EliminatedVariables.end(),inserter(NewVarsToElim, NewVarsToElim.begin()));

  VarsToElim.clear();

  VarsToElim = NewVarsToElim;

  //showSet(VarsToElim, "VarsToElim");

  //cout<<"\nResult of MultiVarDAGQElim = "<<getDAGStr(createDAGFromDD(f))<<endl;

  return f;
}// End of function



// Internal function called by All_Path_QElim_With_Simplifications 
DDNode* DDManager::All_Path_QElim_With_Simplifications_Internal(set<string> VarsToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForAPResolveModified, HashTable <string, DDNode*> *PtrTableForAll_Path_QElim_With_Simplifications_Internal)
{
  //showContext(f, E, D); 
  //cout<<"\ncontent(f) = "<<getDAGStr(createDAGFromDD(f))<<endl;
  //cout<<"\nAll_Path_QElim_With_Simplifications_Internal called on dd node "<<f<<endl;

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function All_Path_QElim_With_Simplifications_Internal in DDManager.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}



  DDNode* Existing  = getEntryFromTableForAll_Path_QElim_With_Simplifications_Internal(VarsToElim, f, E, D, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

  if(Existing != NULL)
    {
      //cout<<"\nEntry existing in hash table returned\n";

      return Existing;
    }


  if(f->getPredicate()==NULL) // Terminal node
    {
      DDNode* result;

      int value = f->getVal();

      if(value == 0)
        {
	  //cout<<"\n0 node reached\n";
          result = f;
        }
      else
        {
	  //cout<<"\n1 node reached\n";
          if(E.empty() && D.empty())
            {
	      //cout<<"\ncontext empty\n";
              result = createDD(1);
            }
          else if(ThUnsat(E, D, WidthTable))
            {
	      //cout<<"\ncontext inconsistent\n";
              result = createDD(0);
            }
          else
            {
	      //cout<<"\ncalling APQElim\n";
	      result = APResolveModifiedWithMultipleVariablesUsingLMA(E, D, VarsToElim, WidthTable);
            }
        }

      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;

      insertEntryIntoTableForAll_Path_QElim_With_Simplifications_Internal(VarsToElim, f, E, D, result, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

      //cout<<"\nReturning the result\n";

      return result;
    }
  else
    {
      bool apply_multivar_dagqelim=true;

      set<DAGNode*> E_Original;
      set<DAGNode*> D_Original;
      set<string> VarsToElim_Original;
      copySet(E_Original, E);
      copySet(D_Original, D);
      copyStringSet(VarsToElim_Original, VarsToElim);

      DDNode *f_new;
      
      if(apply_multivar_dagqelim)
	{
	  //cout<<"\nCalling MultiVarDAGQElim\n";

	  f_new = MultiVarDAGQElim(f, E, D, VarsToElim, WidthTable, PtrTableForAPResolveModified);

	  if(VarsToElim.empty() || f_new->getPredicate()==NULL) // all variables are eliminated or f_new has become a terminal
	   {

	     //cout<<"\nAll vars in VarsToElim are eliminated. Hence returning f\n";

	     insertEntryIntoTableForAll_Path_QElim_With_Simplifications_Internal(VarsToElim_Original, f, E_Original, D_Original, f_new, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

	     return f_new;
	   }
	}
     else
	{
	f_new = f;
	}

      // The problem here is exists(VarsToElim, E/\D/\f_new) which is equivalent to exists(VarsToElim_Original, E_Original/\D_Original/\f). Hence let's check if exists(VarsToElim, E/\D/\f_new) has an entry in the HashTable

     //cout<<"\nProblem gets simplified\n";

      Existing  = getEntryFromTableForAll_Path_QElim_With_Simplifications_Internal(VarsToElim, f_new, E, D, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

     if(Existing != NULL)
    	{
      	//cout<<"\nEntry existing in hash table returned\n";

      	return Existing;
    	}
      

      //cout<<"\nAll vars in VarsToElim are not eliminated\n";

      DAGNode* C = f_new->getPredicate();

      //cout<<"\nLME Encountered = "<<getDAGStr(C)<<endl;

      if(freeOfVars(C, VarsToElim))
        {

	  //cout<<"\nLME free of VarsToElim"<<endl;

          DDNode *temp;

          DDNode *thenb = All_Path_QElim_With_Simplifications_Internal(VarsToElim, f_new->getRight(), E, D, WidthTable, PtrTableForAPResolveModified, PtrTableForAll_Path_QElim_With_Simplifications_Internal);
          DDNode *elseb = All_Path_QElim_With_Simplifications_Internal(VarsToElim, f_new->getLeft(), E, D, WidthTable, PtrTableForAPResolveModified, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

          DDNode* C_dd = createDD(C, WidthTable);

	  temp = createITE(C_dd, thenb, elseb);

	  //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(temp))<<endl;

	  insertEntryIntoTableForAll_Path_QElim_With_Simplifications_Internal(VarsToElim_Original, f, E_Original, D_Original, temp, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

	  //cout<<"\nReturning temp\n";

          return temp;
        }
      else
        {
	  //cout<<"\nLME not free of VarsToElim"<<endl;

          DDNode *temp;

          set<DAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C));

          DDNode *thenb = All_Path_QElim_With_Simplifications_Internal(VarsToElim, f_new->getRight(), E_new, D, WidthTable, PtrTableForAPResolveModified, PtrTableForAll_Path_QElim_With_Simplifications_Internal);
          DDNode *elseb = All_Path_QElim_With_Simplifications_Internal(VarsToElim, f_new->getLeft(), E, D_new, WidthTable, PtrTableForAPResolveModified, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

	  temp = apply("or", thenb, elseb);

	  //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(temp))<<endl;

	  insertEntryIntoTableForAll_Path_QElim_With_Simplifications_Internal(VarsToElim_Original, f, E_Original, D_Original, temp, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

	 //cout<<"\nReturning temp\n";

          return temp;
        }
    }
}


// Function All_Path_QElim_With_Simplifications 
DDNode* DDManager::All_Path_QElim_With_Simplifications(list<string> VarsToElim, DDNode* f, map<string, int> WidthTable)
{
  set<DAGNode*> E, D;
  E.clear(); D.clear();

  set<string> VarsToElimSet;
  for(list<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
     VarsToElimSet.insert(*it); 
    }

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForAll_Path_QElim_With_Simplifications_Internal;
  HashTable <string, DDNode*> *PtrTableForAll_Path_QElim_With_Simplifications_Internal = &TableForAll_Path_QElim_With_Simplifications_Internal;



  //cout<<"\nOriginal dd = "<<getDAGStr(createDAGFromDD(f))<<endl;

  return All_Path_QElim_With_Simplifications_Internal(VarsToElimSet, f, E, D, WidthTable, PtrTableForAPResolveModified, PtrTableForAll_Path_QElim_With_Simplifications_Internal);

}


// Added on 20th Nov 2010 ends here

// Added on 7th Jan 2011
// Functions for LMDD:BDD start here : One constructor is already added at the beginning of this file

// Returns true if "f" which is LMDD:BDD is free of VarToElim; returns false otherwise
bool DDManager::DDWithBDDFreeOfVar(DDNode *f, string VarToElim)
{
  if(f->getPredicate()!=NULL)
    {
      DAGNode *predicate = f->getPredicate();
      if(!(freeOfVarModified(predicate, VarToElim))) return false;
      if(!DDWithBDDFreeOfVar(f->getLeft(), VarToElim)) return false;
      if(!DDWithBDDFreeOfVar(f->getRight(), VarToElim)) return false;
      return true;
    }
  return true;
}


// The function simplifyLMDD for the LMDD:BDD implementation
DDNode*  DDManager::DagResolveModifiedAdvancedWithDynamicVariableOrderingBlasting(DDNode *dd, DAGNode *equation, string VarToElim, map<string, int> WidthTable)
{

  //cout<<"\nDAGResolve called\n";

  map< pair<DDNode*, DAGNode*>, DDNode*>::iterator it = TableForDagResolve.find(make_pair(dd, equation));

  if(it != TableForDagResolve.end())
    {
      return it->second;
    }

  if(dd->getPredicate() == NULL) // Terminal
    {
      return dd;
    }

  DDNode *NewLeft = DagResolveModifiedAdvancedWithDynamicVariableOrderingBlasting(dd->getLeft(), equation, VarToElim, WidthTable);
  DDNode *NewRight = DagResolveModifiedAdvancedWithDynamicVariableOrderingBlasting(dd->getRight(), equation, VarToElim, WidthTable);

  DAGNode *MyPredicate = dd->getPredicate();

  DAGNode *NewPredicate;

  // Check if MyPredicate contains the variable to be eliminated

  if(freeOfVarModified(MyPredicate, VarToElim))
    {
      NewPredicate = MyPredicate;
    }

  else if(!coeffIsOdd(equation, VarToElim)) // check if the coefficient of variable to be eliminated is odd in the equation selected
    {
      // Ideally, this should be done before selecting the equation
      NewPredicate = MyPredicate;
    }
 
  else
    {

      // Converting equation and MyPredicate to Matrix form

      // Write in matrix form
      int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
      Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

      // Get the list of variables,  NoVars, NoVarsToElim
      list<string> VarNames;
      set<string> names;

      names.clear();
      names.insert(VarToElim);
      attachSetToList(names, VarNames);
      NoVarsToElim=1;

      names.clear();

      // we should include variables from the equation and MyPredicate in names
      getDAGNames(equation, names);
      getDAGNames(MyPredicate, names);

      attachSetToList(names, VarNames);

      NoVars=VarNames.size();

      // Now we have the list of variables,  NoVars, NoVarsToElim

      set<DAGNode*> Constraints;
      Constraints.insert(equation);
      Constraints.insert(MyPredicate);

     
      // Get the equations
      bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

      ULINT Mod=findPower(NoOfBits);

      // Now everything is ready. Just call Matrix qEliminaror

      // Checking if Constraints is False before calling the eliminator

      set<DAGNode*> TemporaryDisEqSet;
      TemporaryDisEqSet.clear();
      bool theory_unsat = ThUnsat(Constraints, TemporaryDisEqSet,WidthTable);
      if(theory_unsat)
	{
	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = LOGICAL_CONSTANT;
	  t->width = 1;
	  v->set_node_type(*t);
	  NewPredicate = dm.createNode("false",(void*)v);
	}
      else
	{
	  apqelim_status_on=false; // APQElim called inside DAGQElim is not counted
	  NewPredicate = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
	  apqelim_status_on=true;

	  if(NewPredicate == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      NewPredicate = dm.createNode("true",(void*)v);
	    }
	  else // we have a new LME
	    {
	      if(!(NewPredicate->Get_node_name()=="=") && !(NewPredicate->Get_node_name()=="equivalent")) // not an LME
		{
		  cout<<"DAGResolveBlasting creats a new node which is not an LME \n";
		  exit(1);
		}

	      NewPredicate = recreateEquationInProperForm(NewPredicate, WidthTable); // recreate in cannonical form

              //cout<<"\nNewPredicate = "<<getDAGStr(NewPredicate)<<", MyPredicate = "<<getDAGStr(MyPredicate)<<endl;

	      // Note that setting dynamic_ordering=false will not change indices of APs in APMap. Also, the predicate NewPredicate is not inserted into the APMap. Now when createDDFromDAG(NewPredicate, WidthTable) is called, it will call createDD(NewPredicate, WidthTable), which will insert NewPredicate as the predicate with the maximum index. This will normally result in larger dd sizes after DAGResolve

	      bool dynamic_ordering=true;
	      if(dynamic_ordering)
		dynamicOrdering(MyPredicate, NewPredicate); // increment the indices of the predicates in APMap above MyPredicate by 1 and insert NewPredicate just on top of MyPredicate i.e. at MyPredicate's index + 1
	    }
		
	}

    }

  // Note that in DD nodes, left is else and right is then

  DDNode* then_dd = NewRight;
  DDNode* els_dd = NewLeft;

  DDNode* codn_dd;

  if(simplification_mode==2)
    {
	codn_dd = createDDFromDAGWithImplicationCheck(NewPredicate, WidthTable);
    }
  else if(simplification_mode==1)
    {
	codn_dd = createDDFromDAGWithoutImplicationCheck(NewPredicate, WidthTable);
    }
  else if(simplification_mode==0)
    {
	codn_dd = createDDFromDAGWithoutImplicationCheck(NewPredicate, WidthTable);
    }
  else
	{
	cout<<"\nInvalid simplification mode\n";
	exit(1);
	}

  DDNode* resultDD = createITE(codn_dd, then_dd, els_dd );

  TableForDagResolve.insert(make_pair(make_pair(dd, equation), resultDD));

  return resultDD;
}

// Function to insert entries into the hash table for the 
// function QE1_LMDD for the LMDD:BDD implementation
void DDManager::insertEntryIntoTableForFastQElimBlasting(string VarToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, set<DdNode*> &BDDsInContext, DDNode* value, HashTable <string, DDNode*> *PtrTableForFastQElimBlasting)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }
  
   for(set<DdNode*>::iterator it = BDDsInContext.begin(); it != BDDsInContext.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForFastQElimBlasting->hashtable_search(key);
  
  if(result.success())
    {
      cout<<"\nInside function DDManager::insertEntryIntoTableForFastQElimBlasting. Entry already exists for this node. Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = PtrTableForFastQElimBlasting->hashtable_insert(key, value);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForFastQElimBlasting\n";
      exit(1);
    }
}

// Function to get entry from the hash table for the 
// function DDManager::APQElimBlasting for the LMDD:BDD implementation
// DDManager::APQElimBlasting performs \exists VarToElim. (E /\ D /\ BDDsInContext) 
DDNode* DDManager::getEntryFromTableForAPQElimBlasting(string VarToElim, set<DAGNode*> E, set<DAGNode*> D, set<DdNode*> &BDDsInContext, HashTable <string, DDNode*> *PtrTableForAPQElimBlasting)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

 for(set<DdNode*>::iterator it = BDDsInContext.begin(); it != BDDsInContext.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }


  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForAPQElimBlasting->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// Function to insert entries into the hash table for the 
// function DDManager::APQElimBlasting for the LMDD:BDD implementation
// DDManager::APQElimBlasting performs \exists VarToElim. (E /\ D /\ BDDsInContext) 
void DDManager::insertEntryIntoTableForAPQElimBlasting(string VarToElim, set<DAGNode*> E, set<DAGNode*> D, set<DdNode*> &BDDsInContext, DDNode* value, HashTable <string, DDNode*> *PtrTableForAPQElimBlasting)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];


  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

 for(set<DdNode*>::iterator it = BDDsInContext.begin(); it != BDDsInContext.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }
  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForAPQElimBlasting->hashtable_search(key);


  if(result.success())
    {
      cout<<"\nInside function DDManager::insertEntryIntoTableForAPQElimBlasting. Entry already exists for this node. Then why earlier search was not able to find this?\n";
      exit(1);
    }

  result = PtrTableForAPQElimBlasting->hashtable_insert(key, value);
   
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForAPQElimBlasting\n";
      exit(1);
    }
}

// Performs \exists VarToElim. (E /\ D /\ BDDsInContext) 
// Returns an LMDD:BDD as result
// Used in the LMDD:BDD implementation
DDNode* DDManager::APQElimBlasting(set<DAGNode*> E, set<DAGNode*> D, set<DdNode*> &BDDsInContext, string VarToElim, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForAPQElimBlasting)
{
   //cout<<"\nAPQElimBlasting Called\n";
   
  DDNode* Existing  = getEntryFromTableForAPQElimBlasting(VarToElim, E, D, BDDsInContext, PtrTableForAPQElimBlasting);

  if(Existing != NULL)
    {
      //cout<<"\nAPQElimBlasting's hash table hit\n";
      return Existing;
    }


   showContextOfAPResolve(VarToElim, E, D);

   // Here we write the new implementation for APQElimBlasting and test it 

  if(true) {

  DDNode* resultDD;

  if(BDDsInContext.empty())
  {
  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();
  names.insert(VarToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  attachSetToList(names, VarNames);

  NoVars=VarNames.size();

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

   DAGNode *Phi1, *Phi2;

   Phi1 = qEliminatorReturningDAGOnlyForOddCoefficients(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable, Phi2);

   cout<<"\nPhi1 = "<<getDAGStr(Phi1)<<endl;
   cout<<"\nPhi2 = "<<getDAGStr(Phi2)<<endl;

   Phi1 = quickSimplify(Phi1);
   Phi2 = quickSimplify(Phi2);

   cout<<"\nPhi1 = "<<getDAGStr(Phi1)<<endl;
   cout<<"\nPhi2 = "<<getDAGStr(Phi2)<<endl;


   if(isTrue(Phi2)) // Phi2 is true. i.e. GD is able to solve it
	{
	resultDD = createDDFromDAGWithoutImplicationCheck(Phi1, WidthTable);
	}
   else // Phi2 is not true; create BDD for Phi2 and do QElim using CUDD
	{
        set<string> VarSetToElim;
        VarSetToElim.insert(VarToElim);
	DdNode* Phi2_BDD = bitBlastAndQuantifyUsingCDDInDDManager(Phi2, VarSetToElim, WidthTable);
	} 

   }

   }

  // Following code is temporary : delete it later




  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();
  names.insert(VarToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  attachSetToList(names, VarNames);

  NoVars=VarNames.size();

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;

  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);

  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }


  DDNode* resultDD;

  resultDD = createDDFromDAGWithoutImplicationCheck(resultDAG, WidthTable);

  insertEntryIntoTableForAPQElimBlasting(VarToElim, E, D, BDDsInContext, resultDD, PtrTableForAPQElimBlasting);

  return resultDD;

}

// Function to get entries from the hash table for the 
// function QE1_LMDD for the LMDD:BDD implementation
DDNode* DDManager::getEntryFromTableForFastQElimBlasting(string VarToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, set<DdNode*> &BDDsInContext, HashTable <string, DDNode*> *PtrTableForFastQElimBlasting)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DdNode*>::iterator it = BDDsInContext.begin(); it != BDDsInContext.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForFastQElimBlasting->hashtable_search(key);
  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// The function QE1_LMDD for the LMDD:BDD implementation
DDNode* DDManager::FastQElimBlasting(string VarToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, set<DdNode*> &BDDsInContext, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForFastQElimBlasting, HashTable <string, DDNode*> *PtrTableForAPQElimBlasting)
{


  //cout<<"\n  DDManager::FastQElimBlasting called \n";

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function FastQElimBlasting in DDManager.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}
  

  //showContext(f, E, D); 

  DDNode* Existing  = getEntryFromTableForFastQElimBlasting(VarToElim, f, E, D, BDDsInContext, PtrTableForFastQElimBlasting);

  if(Existing != NULL)
    {
      //cout<<"\nEntry existing in hash table returned\n";

      return Existing;
    }

  //cout<<"\nNo entry existing in hash table\n";

  if(f->getPredicate()==NULL) // Terminal node
    {
      //cout<<"\nTerminal node reached\n";

      DDNode* result;

      int value = f->getVal();

      if(value == 0)
        {
	  //cout<<"\nresult=0 returned\n";

          result = f;
        }
      else
        {

          if(E.empty() && D.empty() && BDDsInContext.empty())
            {
	      //cout<<"\nContext empty. hence result=1 returned\n";

              result = createDD(1);
            }
          else if(ThUnsat(E, D, WidthTable))
            {
	      //cout<<"\nContext inconsistent. hence result=0 returned\n";
	      
              result = createDD(0);
            }
          else
            {
	      //cout<<"\nContext consistent. APResolve invoked\n";
	      
	      result = APQElimBlasting(E, D, BDDsInContext, VarToElim, WidthTable, PtrTableForAPQElimBlasting);

	      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;
            }
        }

      insertEntryIntoTableForFastQElimBlasting(VarToElim, f, E, D, BDDsInContext, result, PtrTableForFastQElimBlasting);

      return result;
    }
  else
    {

      if(DAGResolveApplicable(E, D, WidthTable))
        {
	  //cout<<"\nDAGResolve is applicable\n";

          DAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable);
          if(e1 != NULL)
            {
	      //cout<<"\nEquation selected is "<<getDAGStr(e1)<<endl;

              //cout<<"\nOriginal DD before DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

	      //cout<<"\nSize of original dd = "<<getSizeOfDD(f)<<endl;
 
              f = DagResolveModifiedAdvancedWithDynamicVariableOrderingBlasting(f, e1, VarToElim, WidthTable);

              //cout<<"\nDD after DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

	      //cout<<"\nSize of DAGResolved dd = "<<getSizeOfDD(f)<<endl;

              //f is simplified here. Check if there exists an entry with this simplfied version of f

              DDNode* Existing  = getEntryFromTableForFastQElimBlasting(VarToElim, f, E, D, BDDsInContext, PtrTableForFastQElimBlasting);

              if(Existing != NULL)
                {
		  //cout<<"\nEntry exists for the simplified dd in hash table\n";

                  return Existing;
                }

              if(DDWithBDDFreeOfVar(f, VarToElim))
                {
		  //cout<<"\nEntry does not exist for the simplified dd in hash table\n";

		  //cout<<"\nAPResolve called\n";

                  DDNode *temp;
                  if(ThUnsat(E, D, WidthTable)) temp = createDD(0);
                  else 
		    {
		      temp = APQElimBlasting(E, D, BDDsInContext, VarToElim, WidthTable, PtrTableForAPQElimBlasting);
		    }

                  //cout<<"\nAPResolve on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

                  temp = apply("and", f, temp);

		  insertEntryIntoTableForFastQElimBlasting(VarToElim, f, E, D, BDDsInContext, temp, PtrTableForFastQElimBlasting);

		  //cout<<"\ntemp i.e. result = "<<getDAGStr(createDAGFromDD(temp))<<endl;

		  return temp;
                }
            }
        }

      DAGNode* C = f->getPredicate();

      //cout<<"\nLME encountered = "<<getDAGStr(C)<<endl;



      if(freeOfVarModified(C, VarToElim)) // should be conjuncted with if(BDD for f is free of VarToElim)
        {

	  //cout<<"\nLME free of "<<VarToElim<<endl;

          DDNode *temp;

          DDNode *thenb = FastQElimBlasting(VarToElim, f->getRight(), E, D, BDDsInContext, WidthTable, PtrTableForFastQElimBlasting, PtrTableForAPQElimBlasting);
          DDNode *elseb = FastQElimBlasting(VarToElim, f->getLeft(), E, D, BDDsInContext, WidthTable, PtrTableForFastQElimBlasting, PtrTableForAPQElimBlasting);

          DDNode* C_dd = createDD(C, WidthTable);

          temp = createITE(C_dd, thenb, elseb);

          insertEntryIntoTableForFastQElimBlasting(VarToElim, f, E, D, BDDsInContext, temp, PtrTableForFastQElimBlasting);

	  //cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;

          return temp;
        }
      else
        {
	  //cout<<"\nLME not free of "<<VarToElim<<endl;

          DDNode *temp;

          set<DAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C));

          DDNode *thenb = FastQElimBlasting(VarToElim, f->getRight(), E_new, D, BDDsInContext, WidthTable, PtrTableForFastQElimBlasting, PtrTableForAPQElimBlasting);
          DDNode *elseb = FastQElimBlasting(VarToElim, f->getLeft(), E, D_new, BDDsInContext, WidthTable, PtrTableForFastQElimBlasting, PtrTableForAPQElimBlasting);

           temp = apply("or", thenb, elseb);

          insertEntryIntoTableForFastQElimBlasting(VarToElim, f, E, D, BDDsInContext, temp, PtrTableForFastQElimBlasting);

	  //cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;

          return temp;
        }
    }
}

// Function which calls QE1_LMDD for the LMDD:BDD implementation
// Performs \exists VarToElim. f where f is a LMDD:BDD
DDNode* DDManager::callFastQElimBlasting(string VarToElim, DDNode* f, map<string, int> WidthTable)
{
  TableForDagResolve.clear();

  //exit(1);

  set<DAGNode*> E, D;
  E.clear(); D.clear();
  set<DdNode*> BDDsInContext;
  BDDsInContext.clear();


  HashTable <string, DDNode*> TableForAPQElimBlasting;
  HashTable <string, DDNode*> *PtrTableForAPQElimBlasting = &TableForAPQElimBlasting;

  HashTable <string, DDNode*> TableForFastQElimBlasting;
  HashTable <string, DDNode*> *PtrTableForFastQElimBlasting = &TableForFastQElimBlasting;

  DDNode* result;

  cout<<"\nCalling FastQElimBlasting\n";

  

  result = FastQElimBlasting(VarToElim, f, E, D, BDDsInContext, WidthTable, PtrTableForAPQElimBlasting, PtrTableForFastQElimBlasting);

   return result;
}

// Function QuaLMoDE for the LMDD:BDD implementation
// Performs \exists VarsToElim. f where f is a LMDD:BDD
DDNode* DDManager::QuaLMoDEWithBlasting(list<string> VarsToElim, DDNode* f, map<string, int> WidthTable)
{
  time_t drop_time, find_time, t2;

  DDNode *result;
  result = f;

   while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function QuaLMoDEWithBlasting in DDManager.cpp has timed out. It's result is not exact\n";
	timed_out = true; // timed_out flag set
	return result;
	}
      
      showList(VarsToElim, "VarsToElim");
      
      string VarToElim = chooseVar(result, VarsToElim);

      cout<<"\nVariable chosen to eliminate = "<<VarToElim<<endl;

      cout<<"\ncallFastQElimBlasting called = "<<VarToElim<<endl;

      cout<<"\nNumber of nodes in the input DD = "<<getSizeOfDD(result)<<endl;


      time(&drop_time);

      result = callFastQElimBlasting(VarToElim, result, WidthTable);

      time(&t2);

      cout<<"\n\nTime spent in callFastQElimBlasting to eliminate "<<VarToElim<<"  = "<< (t2 - drop_time)<<" SECONDS "<<endl<<endl;
      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;

      //showDD(result);
 
      int NoOfDDNodes = 0;
      SetOfDDNodes.clear();
      getNoOfNodesInDD(result, NoOfDDNodes);

      cout<<"\nNumber of nodes in the output DD = "<<NoOfDDNodes<<endl;

      if(NoOfDDNodes > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = NoOfDDNodes;
	}


      VarsToElim.remove(VarToElim); 

      //int userchar = getchar(); 
      //showAPMap();
    }

   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function MultiVarQElimWithStrategy in DDManager.cpp has timed out. It's result is not exact\n";
       timed_out = true; // timed_out flag set
       return result;
     }


  return result;
}


// Given "node" which is a propositional logic formula, and "VarSetToElim"  which is a set of variables,
// this function 1) creates a BDD for "node", 2) gets the indices of the variables to be eliminated, 
// and 3) returns the BDD.
DdNode* DDManager::createBDDInDDManager(DAGNode *node, map<DAGNode*, DdNode*> &DPTable, set<string> &VarSetToElim, set<int> &BooleanVarSetToElim) // node is a propositional logic formula
{
map<DAGNode*, DdNode*>::iterator dit = DPTable.find(node);
if(dit != DPTable.end())
	{
        //cout<<"\nHashtable hit for "<<node->Get_node_name()<<endl;
	return dit->second;
	}

else
{
//cout<<"\nHashtable miss for "<<node->Get_node_name()<<endl;
DdNode *result;

if(node->Get_outlist_size() == 0) 
	{
	if(node->Get_node_name()=="true" )
		{
                if(node->Get_outlist_size() != 0) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}
	        result = Cudd_ReadOne(manager);
	        Cudd_Ref(result);
		}

	 else if(node->Get_node_name()=="false" )

		{
		 if(node->Get_outlist_size() != 0) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}
	        result = Cudd_ReadLogicZero(manager);
	        Cudd_Ref(result);

  		}
	 else // variable
		{
		int variable_index;
		map<string, int>::iterator nim_it = NameIndexMap.find(node->Get_node_name());
		if(nim_it != NameIndexMap.end()) // This variable is already encountered. Use the same index
			{
			variable_index = nim_it->second;
			string variable_name = node->Get_node_name();

			int index_of_uscore = variable_name.find_last_of("_");
			string variable_name_no_uscore = variable_name.substr(0, index_of_uscore);

			if(VarSetToElim.find(variable_name_no_uscore)!=VarSetToElim.end()) //variable_name is a variable to eliminate 
				{
				//cout<<endl<<variable_name<<" is a variable to eliminate \n";
				BooleanVarSetToElim.insert(variable_index);
				}
			}
		else // New variable; new index needed
			{
			variable_index = bdd_index;
			string variable_name = node->Get_node_name();

			NameIndexMap.insert(make_pair(variable_name, variable_index));
			IndexNameMap.insert(make_pair(variable_index, variable_name));

			
			int index_of_uscore = variable_name.find_last_of("_");
			string variable_name_no_uscore = variable_name.substr(0, index_of_uscore);

			if(VarSetToElim.find(variable_name_no_uscore)!=VarSetToElim.end()) //variable_name is a variable to eliminate 
				{
				//cout<<endl<<variable_name<<" is a variable to eliminate \n";
				BooleanVarSetToElim.insert(variable_index);
				}

			bdd_index++;
			}
		result = Cudd_bddIthVar(manager,variable_index);
		}
	
	}//if(node->Get_outlist_size() == 0) 
else
	{
	if(node->Get_node_name()=="not" )
		{
		 if(node->Get_outlist_size() != 1) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_Not(temp);
                 Cudd_Ref(result);
  		}                                           
        else if(node->Get_node_name()=="and" )
		{
		if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddAnd(manager,temp1,temp2);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,temp1);
		 //Cudd_RecursiveDeref(manager,temp2);
		}

              else if(node->Get_node_name()=="or" )  
                { 
		if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddOr(manager,temp1,temp2);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,temp1);
		 //Cudd_RecursiveDeref(manager,temp2);
		 }
             else if(node->Get_node_name()=="=" )  
                { 

                    if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}

		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
                 
                 DdNode *temp =  Cudd_bddXor(manager,temp1,temp2);         
                 Cudd_Ref(temp);
 
                 result = Cudd_Not(temp);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,temp1);
		 //Cudd_RecursiveDeref(manager,temp2);
	         //Cudd_RecursiveDeref(manager,temp);
		}

               else if(node->Get_node_name()=="exor" )  
                { 

                    if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddXor(manager,temp1,temp2);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,temp1);
		 //Cudd_RecursiveDeref(manager,temp2);
		}
	    else if(node->Get_node_name()=="if_then_else" )  
                { 

                    if(node->Get_outlist_size() != 3) 
			{
			cout<<"\nUnexpected outlist size in function createBDDInDDManager in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *condn_dd = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *then_dd = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
	         iout++;
		 DdNode *else_dd = createBDDInDDManager(*iout,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddIte(manager, condn_dd, then_dd, else_dd);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,condn_dd);
		 //Cudd_RecursiveDeref(manager,then_dd);
		 //Cudd_RecursiveDeref(manager,else_dd);
                }

	    else if(node->Get_outlist_size()>0)
	      {
		cout<<"\nUnknown node "<<node<<" with name "<<node->Get_node_name()<<" in function function createBDDInDDManager in DDManager.cpp\n";
		exit(1);
	      }
	}//if(node->Get_outlist_size() != 0) 

DPTable.insert(make_pair(node, result));

return result;

} // Miss in DPTable


}

// Returns "BooleanVarSetToElim" as a cube
DdNode* DDManager::createCubeBDDInDDManager(set<int> &BooleanVarSetToElim)
{
DdNode *temp;
DdNode *result;
DdNode *var;

result = Cudd_ReadOne(manager);
Cudd_Ref(result);

for(set<int>::iterator it=BooleanVarSetToElim.begin(); it!=BooleanVarSetToElim.end(); it++)
	{
	var = Cudd_bddIthVar(manager,*it);
	temp = Cudd_bddAnd(manager,result,var);
	Cudd_Ref(temp);
  	Cudd_RecursiveDeref(manager,result);
	result = temp;
	}
return result;
}

// Given "node" which is a propositional logic formula, and "VarSetToElim"  which is a set of variables,
// this function returns the BDD for \exists VarSetToElim. node.
DdNode* DDManager::quantifyUsingCUDDInDDManager(DAGNode *node, set<string> &VarSetToElim)
{
map<DAGNode*,DdNode*> DPTable;
set<int> BooleanVarSetToElim;

DdNode *BDD = createBDDInDDManager(node, DPTable, VarSetToElim, BooleanVarSetToElim);

//cout<<"\nBDD\n";
//Cudd_PrintDebug(manager, BDD, 0, 4);

//cout<<endl<<"NameIndexMap"<<endl;
//for(map<string, int>::iterator it=NameIndexMap.begin(); it!=NameIndexMap.end(); it++)
//	{
//	cout<<it->first<<"\t"<<it->second<<endl;
//	}
//cout<<"\nbdd_index = "<<bdd_index<<endl;

//cout<<endl<<"BooleanVarSetToElim"<<endl;
//for(set<int>::iterator it=BooleanVarSetToElim.begin(); it!=BooleanVarSetToElim.end(); it++)
//	{
//	cout<<*it<<endl;
//	}

DdNode *CubeBDD = createCubeBDDInDDManager(BooleanVarSetToElim);

//cout<<"\nCubeBDD\n";
//Cudd_PrintDebug(manager, CubeBDD, 0, 4);

DdNode *QuantifiedBDD = Cudd_bddExistAbstract(manager, BDD, CubeBDD);

// Let's examine the support

if(false)
	{
	set<string> Support;
	getSupport(QuantifiedBDD, Support);
	showSet(Support, "Support");
	}

//cout<<"\nQuantifiedBDD\n";
//Cudd_PrintDebug(manager, QuantifiedBDD, 0, 4);


return QuantifiedBDD;
}


// Given "DAG_to_blast" which is a bv-logic formula, and "VarSetToElim"  which is a set of variables,
// this function performs \exists VarSetToElim. DAG_to_blast using bit-blasting + BDD
DdNode* DDManager::bitBlastAndQuantifyUsingCDDInDDManager(DAGNode *DAG_to_blast, set<string> VarSetToElim, map<string, int> WidthTable)
{
  DAGNode* node = DAG_to_blast;

  DAGNode* BitBlastedDAG = getBitBlastedDAG(node);
  
  
  bool debug = false;
    if(debug){
  ofstream *outfile = new ofstream();
  outfile->open("blasted_DAGFile.txt");
  cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
  *outfile<<"\nBlasted Formula "<<endl;
  dm.printDAG(BitBlastedDAG, outfile);
  outfile->close();
}

  DdNode* resultBDD = quantifyUsingCUDDInDDManager(BitBlastedDAG, VarSetToElim);

  return resultBDD;
}

// Get the support of the BDD node "node" in set "Support"
void DDManager::getSupport(DdNode *node, set<string> &Support)
{
int *int_support = Cudd_SupportIndex(manager, node);
//cout<<"\nSupport\n";
//for(int i=0; i<bdd_index; i++)
//	{
//	cout<<"Support["<<i<<"] = "<<int_support[i]<<endl;
//	}
//set<string> Support;
for(int i=0; i<bdd_index; i++)
	{
	if(int_support[i]==1)
		{
		map<int, string>::iterator inmt = IndexNameMap.find(i);
		if(inmt == IndexNameMap.end())
			{
			cout<<"\nNo entry for index "<<i<<" in IndexNameMap in function DDManager::getSupport\n";
			exit(1);
			}
		Support.insert(inmt->second);
		}
	}
//showSet(Support, "Support");
}


// Functions for LMDD:BDD end here

// Functions for All path QElim with GD layer + Blasting start here

// Function internally called by DDManager::MultiVarQElimWithNoDPNoStrategyWithBlasting (which implements
// QBV_Solve)
// Obtains \exists VarsToElim. (E/\D/\f) in "FinalResult"
void DDManager::QElimWithNoDPWithBlasting(set<string> VarsToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, map<string, int> WidthTable, DAGNode* &FinalResult)
{

  if(f->getPredicate()==NULL) // Terminal node
    {
      //cout<<"\nTerminal hit\n";

      int value = f->getVal();

      if(value != 0) // 1 terminal
        {
	//cout<<"\n1 terminal hit\n";

          if(E.empty() && D.empty())
            {
              cout<<"\nContext empty in DDManager::QElimWithNoDPWithBlasting\n";
	      exit(1);

            }
          else if(!ThUnsat(E, D, WidthTable))
            {
	       //cout<<"\nConsistent path\n";
               APResolveModifiedWithMultipleVariablesUsingLMAWithBlasting(E, D, VarsToElim, WidthTable, FinalResult);
            }
        }
    }
  else
    {
      DAGNode* C = f->getPredicate();

      //cout<<"\nPredicate = "<<getDAGStr(C)<<"\n";

      set<DAGNode*> E_new, D_new;
      copySet(E_new, E);
      E_new.insert(C);
      copySet(D_new, D);
      D_new.insert(negateConstraint(C));

      QElimWithNoDPWithBlasting(VarsToElim, f->getRight(), E_new, D, WidthTable, FinalResult);
      QElimWithNoDPWithBlasting(VarsToElim, f->getLeft(), E, D_new, WidthTable, FinalResult);
    }
}

// Function implementing QBV_Solve
// Obtains \exists VarsToElim. f in "FinalResult"
void DDManager::MultiVarQElimWithNoDPNoStrategyWithBlasting(list<string> VarsToElim, DDNode* f, map<string, int> WidthTable, DAGNode* &FinalResult)
{
  set<DAGNode*> E, D;
  E.clear(); D.clear();

  set<string> VarsToElimSet;
  for(list<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
     VarsToElimSet.insert(*it); 
    }

 QElimWithNoDPWithBlasting(VarsToElimSet, f, E, D, WidthTable, FinalResult);

}

// Given a set of LMEs and LMDs, and a set of variables to be eliminated, 
// perform QE using GD Layer + bit-blasting + QE at bit-level. result is obtained in "FinalResult".
// This implements BV_Solve function which was actually used in the CAV'11 experiments
void DDManager::APResolveModifiedWithMultipleVariablesUsingLMAWithBlasting(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> WidthTable, DAGNode* &FinalResult)
{
  //cout<<"\nContext\n";
  //showAPSet("E", E);
  //showAPSet("D", D);
  //showSet(VarsToElim, "VarsToElim");

  cout<<"\nAPResolveModifiedWithMultipleVariablesUsingLMAWithBlasting called\n";

  set<DAGNode*> Constraints;
  set<DAGNode*> Constraints_Free_Of_VarsToElim; // Conjunction of this is the first part
  set<DAGNode*> Constraints_Bound_With_VarsToElim;
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));
  //showAPSet("Constraints", Constraints);

  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);

  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);

  copySet(Constraints_Free_Of_VarsToElim, Free_Constraints);
  copySet(Constraints_Bound_With_VarsToElim, Bound_Constraints);

  Free_Constraints.clear();
  Bound_Constraints.clear();

  DAGNode* BoundDAG=NULL;

  if(!Constraints_Bound_With_VarsToElim.empty())
	{
	//cout<<"\nFinding Easy Vars\n";

	if(output_conjunctions)
	{
        vector<DAGNode*> ConstraintsVector;
	for(set<DAGNode*>::iterator it = Constraints_Bound_With_VarsToElim.begin(); it != Constraints_Bound_With_VarsToElim.end(); it++)
		{
		ConstraintsVector.push_back(*it);
		}

	//createConjunctionAndPrintInSMTLibFormat(ConstraintsVector, VarsToElim, WidthTable);
	createConjunctionsAccordingToModAndPrintInSMTLibFormat(ConstraintsVector, VarsToElim, WidthTable);
	return; // don't do anything
	}



	  set<string> EasyVars;// Find variables with LME with odd coefficients
	  findVariablesWithOddCoefficients(E, WidthTable, VarsToElim, EasyVars);
	  showSet(EasyVars, "EasyVars");

	  getFreeAndBoundConstraintsForMultipleVariables(Constraints_Bound_With_VarsToElim, EasyVars, Free_Constraints, 	Bound_Constraints, WidthTable);

	  showAPSet("Harder_Constraints", Free_Constraints);
	  //showAPSet("Bound_Constraints", Bound_Constraints);

         DAGNode* ResultOfGDLayer = APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(Bound_Constraints, VarsToElim, WidthTable);

        //cout<<"\nResultOfGDLayer = "<<getDAGStr(ResultOfGDLayer)<<endl;

        DAGNode* ResultFromCUDD = qElimHarderConstraints(Free_Constraints, VarsToElim, WidthTable);

        //cout<<"\nResultFromCudd = "<<getDAGStr(ResultFromCUDD)<<endl;


      	if(isTrue(ResultOfGDLayer)) // Phi1 is true. Phi = Phi2
		{
		BoundDAG = ResultFromCUDD;
		}
   	else if(isTrue(ResultFromCUDD)) // Phi2 is true. Phi = Phi1
		{
		BoundDAG = ResultOfGDLayer;
		}
   	else // Phi = Phi1 /\ Phi2
		{
		BoundDAG = createDAG("and",ResultOfGDLayer, ResultFromCUDD, LOGICAL_SYMBOL, 1);
		} 
	}

//cout<<"\nExited from qelim loop\n";

if(BoundDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      BoundDAG = dm.createNode("true",(void*)v);
    }

DAGNode* FreeDAG=NULL;

for(set<DAGNode*>::iterator it=Constraints_Free_Of_VarsToElim.begin(); it!=Constraints_Free_Of_VarsToElim.end(); it++)
    {
      if(FreeDAG==NULL)
	{
	  FreeDAG=*it;
	}
      else
	{
	  FreeDAG = createDAG("and",FreeDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
if(FreeDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      FreeDAG = dm.createNode("true",(void*)v);
    }

DAGNode* FinalDAG = NULL;

if(isTrue(BoundDAG)) // Phi1 is true. Phi = Phi2
		{
		FinalDAG = FreeDAG;
		}
else if(isTrue(FreeDAG)) // Phi2 is true. Phi = Phi1
		{
		FinalDAG = BoundDAG;
		}
else // Phi = Phi1 /\ Phi2
		{
		FinalDAG = createDAG("and",FreeDAG, BoundDAG, LOGICAL_SYMBOL, 1);
		} 

if(isTrue(FinalDAG))
	{
	//cout<<"\nAbstraction True. Why??\n";
	//exit(1);
	}

if(FinalResult==NULL)
	{
	  FinalResult=FinalDAG;
	}
else
	{
	  FinalResult = createDAG("or",FinalResult, FinalDAG, LOGICAL_SYMBOL, 1);
	}

cout<<"\nAPResolveModifiedWithMultipleVariablesUsingLMAWithBlasting finished\n";
}

// Gets the set of variables with odd coefficients in LMEs in set "EasyVars"
void DDManager::findVariablesWithOddCoefficients(set<DAGNode*> &E, map<string, int> &WidthTable, set<string> &Vars, set<string> &EasyVars)
{
// EasyVar = Var with an LME with odd coefficient
for(set<string>::iterator it=Vars.begin(); it != Vars.end(); it++)
	{
	string Var = *it;
	if(checkIfOddConstraintExistsForVariable(E, Var, WidthTable))
		{
		EasyVars.insert(Var);
		}
	}
}

// Given a set of LMEs in "Constraints" and a variable "Var",
// return true if there exists an LME in "Constraints" such
// that the coefficient of "Var" in the LME is odd;
// return false otherwise
bool DDManager::checkIfOddConstraintExistsForVariable(set<DAGNode*> &Constraints, string Var, map<string, int> &WidthTable)
{// all constraints are LMEs
for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
	{
	DAGNode *Constraint = *it;
	if(checkIfCoefficientForVariableForLMEIsOdd(Constraint, Var, WidthTable))
		{
		//cout<<"\nCoefficient of "<<Var<<" is odd in "<<getDAGStr(Constraint)<<endl;
		return true;
		}
	}
//cout<<"\nNo constraint with odd coefficient for "<<Var<<endl;
return false;
}

// Given an LME "Constraint" and a variable "Var",
// return true if the coefficient of "Var" in the LME is odd;
// return false otherwise
bool DDManager::checkIfCoefficientForVariableForLMEIsOdd(DAGNode* Constraint, string Var, map<string, int> &WidthTable)
{
int NoOfBitsInConstraint;

NoOfBitsInConstraint=getNoOfBitsInConstraint(Constraint, WidthTable);

DAGNode *lhs, *rhs;
list<DAGNode*>::iterator out_list_it = Constraint->Get_outlist_start();
lhs = (*out_list_it);
out_list_it++;
rhs = (*out_list_it);

ULINT LocalMod=findPower(NoOfBitsInConstraint);

set<string> Names_LHS;
set<string> Names_RHS;

getDAGNames(lhs, Names_LHS);
getDAGNames(rhs, Names_RHS);

ULINT Coef_LHS, Coef_RHS, Coeff;

if(Names_LHS.find(Var) != Names_LHS.end())
        {
          Coef_LHS = 1;
          getCoefficient(lhs, Var, Coef_LHS);// get coefficient of variable var and store in Coef_LHS

        }
      else
        {
          Coef_LHS = 0;
        }

if(Names_RHS.find(Var) != Names_RHS.end())
        {
          Coef_RHS = 1;
          getCoefficient(lhs, Var, Coef_RHS);// get coefficient of variable var and store in Coef_RHS

        }
      else
        {
          Coef_RHS = 0;
        }

Coeff = sub(Coef_LHS, Coef_RHS, LocalMod);

//cout<<"\nCoefficient of "<<Var<<" in "<<getDAGStr(Constraint)<<" is "<<Coeff<<endl;

if(Coeff%2==0)
	{
	return false;
	}

return true;
}



// Given a set of constraints, and a set of variables to be eliminated, 
// perform QE from the conjunction of constraints 
// using bit-blasting + QE at bit-level. Result is returned
// There is a similar function DDManager::qElimUsingBlasting, which is
// exactly same as this function except that it takes only a single
// dag node as input. 
DAGNode* DDManager::qElimHarderConstraints(set<DAGNode*> &Constraints, set<string> &VarSetToElim, map<string, int> &WidthTable)
{
DAGNode* initialDAG=NULL;

// initialDAG is the conjunction of the constraints in Constraints
for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(initialDAG==NULL)
	{
	  initialDAG=*it;
	}
      else
	{
	  initialDAG = createDAG("and",initialDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }

if(initialDAG == NULL) 
    {
      cout<<"\ninitialDAG == NULL\n";
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      initialDAG = dm.createNode("true",(void*)v);
      return initialDAG;
    }

cout<<"\ninitialDAG non NULL\n";

bool print_input_in_smtlib_format = false;
if(print_input_in_smtlib_format)
			{
			showSet(VarSetToElim, "VarSetToElim");
			static int file_counter = 1;
  			char file_counter_char[100];
			sprintf(file_counter_char, "%d", file_counter);
			string file_counter_str(file_counter_char);
			string file_name = "input_to_blasting_";
			file_name += file_counter_str;
			file_name += ".smt";
			printInputToQEliminatorInSMTLibFormatInGivenFile(initialDAG, WidthTable, VarSetToElim, file_name);
			file_counter++;
			}

DdNode* resultDD = bitBlastAndQuantifyUsingCDDInDDManager(initialDAG, VarSetToElim, WidthTable);

DAGNode* resultDAG = NULL;

resultDAG = synthesizeBVFormulaFromBDD(resultDD, WidthTable);

if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

return resultDAG;
}

// Return DAG equivalent to the given BDD "resultDD" 
DAGNode* DDManager::synthesizeBVFormulaFromBDD(DdNode *resultDD, map<string, int> &WidthTable)
{
if(Cudd_IsConstant(resultDD)==1)
 {
	if(Cudd_V(resultDD)==1) // true BDD
		{
		return NULL;
		}
       else
	{
	cout<<"\nError in DDManager::synthesizeBVFormulaFromBDD. Result is false\n";
	exit(1);
	}
}

//cout<<"\nBDD node not a constant node\n";

DAGNode* resultDAG = createDAGFromBDD(resultDD, WidthTable);

if(Cudd_IsComplement(resultDD)==1) //complemented
  {
   resultDAG = createDAG("not", resultDAG, LOGICAL_SYMBOL, 1); 
  }
	
return resultDAG;
}


// Function to create a DAG from a BDD
DAGNode* DDManager::createDAGFromBDD(DdNode *dd, map<string, int> &WidthTable)
{
  HashTable <string, DAGNode*> MapForDAGFromBDD;
  HashTable <string, DAGNode*> *PtrMapForDAGFromBDD = &MapForDAGFromBDD;

  DAGNode* result = createDAGFromBDDStep(dd, PtrMapForDAGFromBDD, WidthTable);
  return result;
}

// Internal function called by DDManager::createDAGFromBDD
DAGNode* DDManager::createDAGFromBDDStep(DdNode *dd, HashTable <string, DAGNode*> *PtrMapForDAGFromBDD, map<string, int> &WidthTable)
{
  char temp_char[10];
  sprintf(temp_char , "%x", dd);
  string key = temp_char;
  
  // key created

  HTStatusValue<DAGNode*> ResultFromHT = PtrMapForDAGFromBDD->hashtable_search(key);

  if(ResultFromHT.success())
    {
      return ResultFromHT.GetValue();
    }

  // createDAGFromBDDStep was not earlier called with argument 

  DAGNode *result;
  
  if(Cudd_IsConstant(dd)==1)
  {
  if(Cudd_V(dd)==1)// terminal 1
    {
      string constant="true";
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      DAGNode *bit_node = dm.createNode(constant,(void*)v);

      result = bit_node;
    }
  else 
    {
      string constant="false";
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      DAGNode *bit_node = dm.createNode(constant,(void*)v);

      result = bit_node;
    }
  }
  else
    {
      vector<DAGNode *> ChildrenOfIte;
      DAGNode *Condition = getCondition(dd, WidthTable);
      ChildrenOfIte.push_back(Condition);

      DdNode *ThenDD = Cudd_T(dd);
      DdNode *ElseDD = Cudd_E(dd);
       
      ChildrenOfIte.push_back( createDAGFromBDDStep(ThenDD, PtrMapForDAGFromBDD, WidthTable) ); //Then
    
      if(Cudd_IsComplement(ElseDD)==1) //complemented
	{
         ChildrenOfIte.push_back( createDAG("not", createDAGFromBDDStep(ElseDD, PtrMapForDAGFromBDD, WidthTable), LOGICAL_SYMBOL, 1)); // Else
	}
      else
	{
         ChildrenOfIte.push_back( createDAGFromBDDStep(ElseDD, PtrMapForDAGFromBDD, WidthTable) ); // Else
	}

      DAGNode *ite_node = createDAG("if_then_else", ChildrenOfIte[0], ChildrenOfIte[1], ChildrenOfIte[2], LOGICAL_SYMBOL, 1);
      result = ite_node;
	
      // Note that DD for equality eq is eq(0, 1).i.e eq with left child 0 and right child 1
    }


  ResultFromHT = PtrMapForDAGFromBDD->hashtable_insert(key, result);
  if(!ResultFromHT.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::createDAGFromBDDStep\n";
      exit(1);
    }

  return result;
}


// Given BDD node "dd", return it as a selection expression
DAGNode* DDManager::getCondition(DdNode *dd, map<string, int> &WidthTable)
{

unsigned int MyIndex = Cudd_NodeReadIndex(dd);
map<int, string>::iterator inmt = IndexNameMap.find(MyIndex);
if(inmt == IndexNameMap.end())
	{
	cout<<"\nNo variable for index "<<MyIndex<<" in DDManager::getCondition\n";
	exit(1);
	}
string MyName = inmt->second;
int index_of_uscore = MyName.find_last_of("_");
string variable_name = MyName.substr(0, index_of_uscore);
string bit_position_str = MyName.substr(index_of_uscore+1);
//int bit_position = atoi(bit_position_str.c_str());

//cout<<"\nWhat we need is select("<<variable_name<<","<<bit_position_str<<","<<bit_position_str<<")\n";
map<string, int>::iterator wit = WidthTable.find(variable_name);
if(wit == WidthTable.end())
	{
	cout<<"\nNo entry for variable "<<variable_name<<" in WidthTable in DDManager::getCondition\n";
	exit(1);
	}

int width = wit->second;

DAGNode* Child1 = createBVVariable(variable_name, width);

Value *v1 = new Value();
Type t1;
t1.data_type = INTEGER_CONSTANT;
t1.width = IMMATERIAL_WIDTH;
v1->set_node_type(t1);
DAGNode* selection_pos = dm.createNode(bit_position_str, (void*)v1);

DAGNode* Child2 = selection_pos;

DAGNode *lhs = createDAG("select", Child1, Child2, Child2, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
DAGNode *rhs = createBVConstant("1",1);

DAGNode *eqn = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);

return eqn;
}



// Functions for All path QElim with GD layer + Blasting end here

// Functions for QELIM at conjunction level

// Given "root" which is a conjunction of LMCs, return \exists VarsToElim. root.
// Use the given strategy for QE
DAGNode* DDManager::QELIM_Conjunction_level(DAGNode *root, set<string> &VarsToElim, map<string, int> &WidthTable, string qelim_conj_level_strategy, string InFileName)
{ 
 FILE *ap_fp = fopen("ConjunctionBenchmarkDetails.txt", "a+");
 if(ap_fp == NULL)
    {
      cout<<"\nCould not open ConjunctionBenchmarkDetails.txt\n";
      exit(1);
    }

 set<DAGNode*> E;
 set<DAGNode*> D;
 splitConjunctionIntoSetsWithSupportForNot(root, E, D); // splitting the root which is a conjunction into a set of APs
 //showAPSet("E", E); 
 //showAPSet("D", D);
  
  set<DAGNode*> Constraints;
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin())); 

  unsigned int number_of_lmes, number_of_lmds, number_of_vars, number_of_vars_to_elim, number_of_bits;
 
  getAP_QElimDetailsAtDAGLevel_For_QELIM_Conjunctions(Constraints, VarsToElim, WidthTable, number_of_lmes, number_of_lmds, number_of_vars, number_of_vars_to_elim, number_of_bits);

   time_t start, end, duration;
   struct timeval start_ms, finish_ms;
   unsigned long long int duration_ms;

 
   time(&start);
   gettimeofday (&start_ms, NULL);

   DAGNode* resultDAG;

 if(qelim_conj_level_strategy == "apqelim")
	{
	resultDAG = APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(E, D, VarsToElim, WidthTable);
	}
 else if(qelim_conj_level_strategy == "omega")
	{
	resultDAG = callOmegaTestReturningDAG(E, D, VarsToElim, WidthTable);
	}
 else if(qelim_conj_level_strategy == "bvsolve")
	{
	resultDAG = NULL;
	APResolveModifiedWithMultipleVariablesUsingLMAWithBlasting(E, D, VarsToElim, WidthTable, resultDAG);	
	}
 else if(qelim_conj_level_strategy == "observe")
	{
	int number_of_vars_only_even=0;
        int num_muls = 0;
        int num_adds = 0;
	observeConstraints(E, D, VarsToElim, WidthTable, number_of_vars_only_even, num_muls, num_adds);

	FILE *ob_fp = fopen("ObservationsConjunctionsNew.txt", "a+");
 	if(ob_fp == NULL)
    		{
      		cout<<"\nCould not open ObservationsConjunctionsNew.txt\n";
      		exit(1);
    		}
	fprintf(ob_fp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", InFileName.c_str(), number_of_lmes, number_of_lmds, number_of_vars, number_of_vars_to_elim, number_of_bits, number_of_vars_only_even, num_muls, num_adds);

	fclose(ob_fp);
	
	}
 else 
	{
	cout<<"\nUnknown strategy in DDManager::QELIM_Conjunction_level\n";
	exit(1);
	}

  gettimeofday (&finish_ms, NULL);

  time(&end);

  duration = end - start;

  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

  fprintf(ap_fp, "\n\n%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu", InFileName.c_str(), qelim_conj_level_strategy.c_str(), number_of_lmes, number_of_lmds, number_of_vars, number_of_vars_to_elim, number_of_bits, duration, duration_ms);

  return resultDAG;
  
}


// Function to observe the constraints given to the function
// DDManager::QELIM_Conjunction_level
void DDManager::observeConstraints(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> WidthTable,  int &number_of_vars_only_even, int &num_muls, int &num_adds)
{
  //cout<<"\nContext\n";
  //showAPSet("E", E);
  //showAPSet("D", D);
  //showSet(VarsToElim, "VarsToElim");

  cout<<"\nobserveConstraints called\n";

  set<DAGNode*> Constraints;
  set<DAGNode*> Constraints_Free_Of_VarsToElim; // Conjunction of this is the first part
  set<DAGNode*> Constraints_Bound_With_VarsToElim;
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));
  //showAPSet("Constraints", Constraints);

  DAGNode *MyDAG = NULL;
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
	{
	if(MyDAG == NULL)
		MyDAG = *it;
	else
		MyDAG = createDAG("and",MyDAG, *it, LOGICAL_SYMBOL, 1);
	}

  findMulAdds(MyDAG, num_muls, num_adds);	
  


  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);

  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);

  copySet(Constraints_Free_Of_VarsToElim, Free_Constraints);
  copySet(Constraints_Bound_With_VarsToElim, Bound_Constraints);

  Free_Constraints.clear();
  Bound_Constraints.clear();

  DAGNode* BoundDAG=NULL;

  if(!Constraints_Bound_With_VarsToElim.empty())
	{
	//cout<<"\nFinding Easy Vars\n";

	if(output_conjunctions)
	{
        vector<DAGNode*> ConstraintsVector;
	for(set<DAGNode*>::iterator it = Constraints_Bound_With_VarsToElim.begin(); it != Constraints_Bound_With_VarsToElim.end(); it++)
		{
		ConstraintsVector.push_back(*it);
		}

	//createConjunctionAndPrintInSMTLibFormat(ConstraintsVector, VarsToElim, WidthTable);
	createConjunctionsAccordingToModAndPrintInSMTLibFormat(ConstraintsVector, VarsToElim, WidthTable);
	return; // don't do anything
	}



	  set<string> EasyVars;// Find variables with LME with odd coefficients
	  findVariablesWithOddCoefficients(E, WidthTable, VarsToElim, EasyVars);
	  showSet(EasyVars, "EasyVars");

	  getFreeAndBoundConstraintsForMultipleVariables(Constraints_Bound_With_VarsToElim, EasyVars, Free_Constraints, 	Bound_Constraints, WidthTable);

	  showAPSet("Harder_Constraints", Free_Constraints);

	if(Free_Constraints.empty()) // No harder constraints
		{
		number_of_vars_only_even = 0;
		}
	else // There are harder constraints
		{
		set<string> HardVars;
		set_difference(VarsToElim.begin(), VarsToElim.end(), EasyVars.begin(), EasyVars.end(),inserter(HardVars, HardVars.begin()));
		
		number_of_vars_only_even = HardVars.size();
		}
	
	}
	
        
	cout<<"\nobserveConstraints finished\n";
}


// Find the # of bvmul and # of bvadd nodes in the DAG rooted at "node"
void DDManager::findMulAdds(DAGNode *node, int &mul, int &add)
{
   set<DAGNode*> MyMap;
  findMulAddsInternal(node, mul, add, MyMap);
}

// Internal function called by DDManager::findMulAdds
void DDManager::findMulAddsInternal(DAGNode *node, int &mul, int &add, set<DAGNode*> &MyMap)
{
  if(MyMap.find(node)!=MyMap.end())
	{
	return;
	}

  string dag_str = node->Get_node_name();

	if(dag_str == "bvmul")
	{
	  mul++;
	}
	if(dag_str == "bvadd")
	{
	  add++;
	}

	MyMap.insert(node);
	

	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  
          for(unsigned int i=0; i<node->Get_outlist_size(); i++, iout++)
		findMulAddsInternal(*iout, mul, add, MyMap);
}




// Functions for QELIM at conjunction level end here


// Newly added on 31-03-2011 for debugging

// Show the APMap on screen
void DDManager::printAPMapOnScreen()
{
  cout<<"\nAP MAP\n";

  cout<<"\nSize of APMAP = "<<APMap.size()<<"\n";

  map<DAGNode*, int>::iterator APMap_it;

  for(APMap_it = APMap.begin(); APMap_it != APMap.end();  APMap_it++)
    {
      cout<<"\n"<<getDAGStr(APMap_it->first)<<"\t"<<APMap_it->second;
    }
}

// Newly added on 31-03-2011 for debugging ends here


// Newly added on 8 May 2011 starts here

// Return true if /\E /\ D is unsat; false otherwise
// Uses STP
bool DDManager::ThUnsatInSMTLibFormat(set<DAGNode*> E, set<DAGNode*> D, map<string, int> WidthTable)
{

  thunsat_calls++;

  DAGNode *root = NULL;

  set<DAGNode*>::iterator it=E.begin();
  for(; it!=E.end(); it++)
    if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

  it=D.begin();
  for(; it!=D.end(); it++)
    if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

  FILE *smt_file = fopen("thunsat.smt", "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create thunsat.smt\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  writeBVDeclarationsInSMTLibFormat(smt_file, WidthTable);

  string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);
  
  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");

  fclose(smt_file);

  system("stp --SMTLIB1 thunsat.smt > result_thunsat.txt"); //call to STP

  FILE *result_smt_file = fopen("result_thunsat.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::ThUnsatInSMTLibFormat\n"; exit(1);
    }
}


// Newly added on 27 June 2011 

// Given a dag rooted at "initialDAG", and a set of variables to be eliminated, 
// perform QE from "initialDAG"
// using bit-blasting + QE at bit-level. Result is returned
// There is a similar function DDManager::qElimHarderConstraints, which is
// exactly same as this function except that it takes a set of dag nodes as 
// input which are internally conjuncted and the same processing is done.
DAGNode* DDManager::qElimUsingBlasting(DAGNode* initialDAG, set<string> &VarSetToElim, map<string, int> &WidthTable)
{
DdNode* resultDD = bitBlastAndQuantifyUsingCDDInDDManager(initialDAG, VarSetToElim, WidthTable);

DAGNode* resultDAG = NULL;

resultDAG = synthesizeBVFormulaFromBDD(resultDD, WidthTable);

if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

return resultDAG;
}

// Newly added on 27 June 2011 ends here



//Newly added on 1 April 2012 onwards for QE from LMDDs 
// involving LMEs, LMDs, and LMIs


// Returns \exists VarsToElim. f as DDNode* where f is an LMDD
// involving LMEs, LMDs, and LMIs
DDNode* DDManager::QuaLMoDE_WithLMISupport(list<string> VarsToElim, DDNode* f, map<string, int> &WidthTable)
{
  time_t t1, t2;

  DDNode *result;
  result = f;

   while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function QuaLMoDE_WithLMISupport in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return result;
	}
      
      //showList(VarsToElim, "VarsToElim");
      
       time(&t1);

      string VarToElim = chooseVar(result, VarsToElim);

      cout<<"\nVariable chosen to eliminate = "<<VarToElim<<endl;
      
      fprintf(LMIQEOutputDataFp, "\nVariable chosen to eliminate = %s\n", VarToElim.c_str());          
      
      OrderInWhichVariablesAreEliminated.push_back(VarToElim);
      
      time(&t2);

      //cout<<"\n\nTime spent in finding var. to elim.  = "<< (t2 - t1)<<" SECONDS "<<endl<<endl;

      //cout<<"\nNumber of nodes in the input DD = "<<getSizeOfDD(result)<<endl;


      time(&t1);
      
      int project_depth = 0;
      
      if(DDFreeOfVar_WithDP(result, VarToElim))//result free of vartoelim
        {
          //no need to change result
          number_of_variables_for_which_no_elimination_needed++;
        }
      else
        {
         result = callQElim_WithLMISupport(VarToElim, result, WidthTable, project_depth);
        }
      
      time(&t2);

      cout<<"\n\nTime spent in callQElim_WithLMISupport to eliminate "<<VarToElim<<"  = "<< (t2 - t1)<<" SECONDS "<<endl<<endl;
      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;

      //showDD(result);
 
      int NoOfDDNodes = 0;
      SetOfDDNodes.clear();
      getNoOfNodesInDD(result, NoOfDDNodes);

      cout<<"\nNumber of nodes in the output DD = "<<NoOfDDNodes<<endl;
      
      fprintf(LMIQEOutputDataFp, "\nNumber of nodes in the output DD = %d\n", NoOfDDNodes);  

      if(NoOfDDNodes > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = NoOfDDNodes;
	}
        
      SizesOfEliminatedLMDDs.push_back(NoOfDDNodes);

      VarsToElim.remove(VarToElim); 

      //int userchar = getchar(); 
      //showAPMap();
    }

   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function QuaLMoDE_WithLMISupport in DDManager.cpp has timed out. It's result is not exact\n";
       timed_out = true; // timed_out flag set
       return result;
     }


  return result;
}

// Calls the function QElim_WithLMISupport
// Returns the LMDD for \exists VarToElim. f.
DDNode* DDManager::callQElim_WithLMISupport(string VarToElim, DDNode* f, map<string, int> &WidthTable, int project_depth)
{ 
  TableForDagResolve.clear();

  set<DAGNode*> E, D, I;
  E.clear(); D.clear(); I.clear();


  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;

  DDNode* result;
  
  //cout<<"\nCalling QElim_WithLMISupport\n";
  result = QElim_WithLMISupport(VarToElim, f, E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
  //cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;

  return result;
}



// Check if the instance \exists VarToElim. (f /\ E/\ D/\I) is already encountered
// If yes, return the result; NULL otherwise
DDNode* DDManager::getEntryFromTableForQElimModified_WithLMISupport(string VarToElim, DDNode* f, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, HashTable <string, DDNode*> *PtrTableForQElimModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = I.begin(); it != I.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForQElimModified->hashtable_search(key);
  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}



// Return true if /\ E /\ D /\ I is unsat; false otherwise
// Uses STP/yices
bool DDManager::ThUnsat(set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, map<string, int> &WidthTable)
{
  DAGNode *root = NULL;

  set<DAGNode*>::iterator it=E.begin();
  for(; it!=E.end(); it++)
    if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);

  it=D.begin();
  for(; it!=D.end(); it++)
    if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
        
  it=I.begin();
  for(; it!=I.end(); it++)
    if(root==NULL) root=*it; else root=createDAG("and", root, *it, LOGICAL_SYMBOL, 1);
  
  if(root==NULL) 
        root=createLogicalConstant("true");
  
  if(internal_solver=="stp") // use any solver supporting SMTLib format; presenly STP
	{
	return ThUnsatSingleFormulaInSMTLibFormatAsDAG_With_LMI_Support(root, WidthTable);
	}

  FILE *smt_file = fopen("thunsat.ys", "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create thunsat.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(root, WidthTable);
  
  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  system("yices -tc thunsat.ys > result_thunsat.txt");

  FILE *result_smt_file = fopen("result_thunsat.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::ThUnsat\n"; exit(1);
    }
}


// Enter \exists VarToElim. (f /\ E /\ D /\ I) ---> result into Hash Table
void DDManager::insertEntryIntoTableForQElimModified_WithLMISupport(string VarToElim, DDNode*f, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, DDNode* value, HashTable <string, DDNode*> *PtrTableForQElimModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }
  
  for(set<DAGNode*>::iterator it = I.begin(); it != I.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForQElimModified->hashtable_search(key);
  
  if(result.success())
    {
      // Do nothing
      //cout<<"\nInside function DDManager::insertEntryIntoTableForQElimModified_WithLMISupport. Entry already exists for this node. Then why earlier search was not able to find this?\n";
      //exit(1);
    }
  else
    {
     result = PtrTableForQElimModified->hashtable_insert(key, value);
     if(!result.success())
        {
          cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForQElimModified_WithLMISupport\n";
          exit(1);
        }
    }//else
}



// Show f, E, D, and I
void DDManager :: showContext(DDNode *f, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I)
{
  cout<<endl<<"DDNode = "<<f<<endl;
  
  cout<<"Context\n";
  cout<<"LMEs\n";

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      cout<<getDAGStr(*it)<<endl;
    }

  cout<<"LMDs\n";
  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
     cout<<getDAGStr(*it)<<endl;
    }
  
  cout<<"LMIs\n";
  for(set<DAGNode*>::iterator it = I.begin(); it != I.end(); it++)
    {
     cout<<getDAGStr(*it)<<endl;
    }
}



// Function to compute the LMDD for \exists VarToElim. (f/\E/\D/\I)
// where f is an LMDD involving LMEs, LMDs, and LMIs
DDNode* DDManager::QElim_WithLMISupport(string VarToElim, DDNode* f, set<DAGNode*> E, set<DAGNode*> D, set<DAGNode*> I, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
  //cout<<"\nDDManager::QElim_WithLMISupport called \n";
    call_LMDD_Monniaux_SingleVar_calls++;
    CallsToQEWithMonEnabledAllVarStrategy++;
    call_QE_LMDD_Single_Var++;
    
    struct timeval proj_start_ms, proj_finish_ms;
    unsigned long long int proj_duration_ms;

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function QElim_WithLMISupport in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}
  
  //showContext(f, E, D, I);

  DDNode* Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, PtrTableForQElimModified);

  if(Existing != NULL)
    {
      CallsToQEWithMonEnabledAllVarStrategyGlobalHashed++;
      call_QE_LMDD_Single_Var_Hashed++;
      //cout<<"\nEntry existing in hash table returned\n";
      return Existing;
    }

  //cout<<"\nNo entry existing in hash table\n";

  if(f->getPredicate()==NULL) // Terminal node
    {
      CallsToQEWithMonEnabledAllVarStrategyOnTerminals++;
      //cout<<"\nTerminal node reached\n";

      DDNode* result;

      int value = f->getVal();

      if(value == 0)
        {
          call_QE_LMDD_Single_Var_OnTerminalZero++;
	  //cout<<"\nresult=0 returned\n";
          result = f;
        }
      else
        {
          call_QE_LMDD_Single_Var_OnTerminalOne++;

          if(E.empty() && D.empty() && I.empty())
            {
              call_QE_LMDD_Single_Var_OnTerminalOne_Empty++;
	      //cout<<"\nContext empty. hence result=1 returned\n";
              result = createDD(1);
            }
          else if(ThUnsat(E, D, I, WidthTable))
            {
              call_QE_LMDD_Single_Var_OnTerminalOne_Unsat++;
	     //cout<<"\nContext inconsistent. hence result=0 returned\n";
	      result = createDD(0);
            }
          else
            {
              call_QE_LMDD_Single_Var_OnTerminalOne_Sat++;
	      //cout<<"\nContext consistent. Project_ReturningLMDD invoked\n";
              gettimeofday (&proj_start_ms, NULL);
              
              result = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
              
              gettimeofday (&proj_finish_ms, NULL);
              proj_duration_ms = proj_finish_ms.tv_sec * 1000 + proj_finish_ms.tv_usec / 1000;
              proj_duration_ms -= proj_start_ms.tv_sec * 1000 + proj_start_ms.tv_usec / 1000;
              TimeSpentInProjectsInQE_LMDD = TimeSpentInProjectsInQE_LMDD + proj_duration_ms;   
            }
        }

      insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, result, PtrTableForQElimModified);
      return result;
    }//Terminal node
  else //Non-terminal node
    {
      
      // Check if f is already free of VarToElim
       if(DDFreeOfVar_WithDP(f, VarToElim))
                        {
                          call_QE_LMDD_Single_Var_FreeOfVar++;
                            
                           CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsFreeOfAllVars++;
           
                          //cout<<"\n"<<getDAGStr(createDAGFromDD(f))<<" is free of "<<VarToElim<<"\n";
                          

                          DDNode *temp;
                          
                          if(ThUnsat(E, D, I, WidthTable)) 
                            {
                              //cout<<"\nContext inconsistent. result of Project_ReturningLMDD is false\n";
                              temp = createDD(0);
                            }
                          else 
                            {
                              //cout<<"\nContext consistent. Project_ReturningLMDD invoked\n";
                              gettimeofday (&proj_start_ms, NULL); 
                              
                              temp = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                              
                              gettimeofday (&proj_finish_ms, NULL);
                              proj_duration_ms = proj_finish_ms.tv_sec * 1000 + proj_finish_ms.tv_usec / 1000;
                              proj_duration_ms -= proj_start_ms.tv_sec * 1000 + proj_start_ms.tv_usec / 1000;
                              TimeSpentInProjectsInQE_LMDD = TimeSpentInProjectsInQE_LMDD + proj_duration_ms;   
                              
                            }

                          //cout<<"\ncallProjectReturningLMDD on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

                          temp = apply("and", f, temp);

                          insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);

                          //cout<<"\ntemp i.e. result = "<<getDAGStr(createDAGFromDD(temp))<<endl;

                          return temp;
                        }// if(DDFreeOfVar_WithDP(f, VarToElim))
      
      //cout<<"\n"<<getDAGStr(createDAGFromDD(f))<<" is not free of "<<VarToElim<<"\n";
      // Check if f can be simplified using LMEs
       CallsToGetSimplifiedInstance++;      
       
       
       struct timeval start_ms, finish_ms;
       unsigned long long int duration_ms;
       
       gettimeofday (&start_ms, NULL);
       
       if(DAGResolveApplicable(E, D, WidthTable))
                {
           
                call_QE_LMDD_Single_Var_SimplifyLMDDCalls++;
           
                  //cout<<"\nDAGResolve is applicable\n";

                  DAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable);
                  
                  if(e1 != NULL)
                    {
                      //cout<<"\nEquation selected is "<<getDAGStr(e1)<<endl;

                      //cout<<"\nOriginal DD before DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

                      //cout<<"\nSize of original dd = "<<getSizeOfDD(f)<<endl;

                      f = DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(f, e1, VarToElim, WidthTable);

                      //cout<<"\nDD after DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

                      //cout<<"\nSize of DAGResolved dd = "<<getSizeOfDD(f)<<endl;

                      //f is simplified here. Check if there exists an entry with this simplfied version of f

                      DDNode* Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, PtrTableForQElimModified);

                      if(Existing != NULL)
                        {
                          CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsWhereCallToGetSimplifiedInstanceSolved++;
                          
                          call_QE_LMDD_Single_Var_SimplifyLMDDCalls_Hashed++;
                          
                          //cout<<"\nEntry exists for the simplified dd in hash table\n";
                          return Existing;
                        }

                      if(DDFreeOfVar_WithDP(f, VarToElim))
                        {
                          CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsWhereCallToGetSimplifiedInstanceSolved++;
                          
                          
                          call_QE_LMDD_Single_Var_SimplifyLMDDCalls_FreeOfVar++;
                          
                          //cout<<"\nEntry does not exist for the simplified dd in hash table\n";
                          

                          DDNode *temp;
                          
                          if(ThUnsat(E, D, I, WidthTable)) 
                            {
                              //cout<<"\nContext inconsistent. result of Project_ReturningLMDD is false\n";
                              temp = createDD(0);
                            }
                          else 
                            {
                              //cout<<"\nContext consistent. Project_ReturningLMDD invoked\n";
                              gettimeofday (&proj_start_ms, NULL); 
                              
                              temp = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                              
                              gettimeofday (&proj_finish_ms, NULL);
                              proj_duration_ms = proj_finish_ms.tv_sec * 1000 + proj_finish_ms.tv_usec / 1000;
                              proj_duration_ms -= proj_start_ms.tv_sec * 1000 + proj_start_ms.tv_usec / 1000;
                              TimeSpentInProjectsInQE_LMDD = TimeSpentInProjectsInQE_LMDD + proj_duration_ms;   
                            }

                          //cout<<"\ncallProjectReturningLMDD on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

                          temp = apply("and", f, temp);

                          insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);

                          //cout<<"\ntemp i.e. result = "<<getDAGStr(createDAGFromDD(temp))<<endl;

                          return temp;
                        }// if(DDFreeOfVar_WithDP(f, VarToElim))
                    }//if(e1 != NULL)
                }// if(DAGResolveApplicable(E, D, WidthTable))
       

    gettimeofday (&finish_ms, NULL);
           
    duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
    duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
    
    TimeSpentInGetSimplifiedInstance = TimeSpentInGetSimplifiedInstance + duration_ms;   
           
      // Traverse down
      
    call_QE_LMDD_Single_Var_TraverseDown++;
    
     DAGNode* C = f->getPredicate();
     //cout<<"\nPredicate encountered = "<<getDAGStr(C)<<endl;

      if(freeOfVarModified(C, VarToElim))
        {
          call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var++;
          
	  //cout<<"\nPredicate free of "<<VarToElim<<endl;

          DDNode *temp;

          DDNode *thenb;
          
          if(LMEBasedSimplificationInsideQELMDD)//Do eager lme based simplification inside QE_LMDD
          {
              
              
              
              string OddVariableToEliminate;
        
             if(predicateIsLMEWithOddCoefForSomeVarToElim(C, CopyOfVarSetToElim, OddVariableToEliminate))
                {
                call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME++; 
                 
                //cout<<"\npredicateIsLMEWithOddCoefForSomeVarToElim(C, CopyOfVarSetToElim, OddVariableToEliminate)\n";
                 
                if(DDFreeOfVar_WithDP(f->getRight(), OddVariableToEliminate))//f->getRight() is already free of OddVariableToEliminate.
                    // No need to apply simplifyLMDD
                    {
                    
                    call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_dd_free_of_var++;
                    
                    thenb = QElim_WithLMISupport(VarToElim, f->getRight(), E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                    }
                else//f->getRight() is not already free of OddVariableToEliminate.
                    // Let's apply simplifyLMDD
                    {
                    
                    call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_SimplifyLMDD_calls++;
                    
                    VariablesBenefittedByLMEBasedSimplificationInsideQELMDD.insert(OddVariableToEliminate);
                    
                    //cout<<"\nf->getRight() is not free of "<<OddVariableToEliminate<<endl;
                    
                    //cout<<"\nf->getRight() before simplifyLMDD = "<<getDAGStr(createDAGFromDD(f->getRight()))<<endl;
                    
                    gettimeofday (&start_ms, NULL);
                    
                    DDNode* thenb_after_lme_simplification =  DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(f->getRight(), C, OddVariableToEliminate, WidthTable);
   
                    gettimeofday (&finish_ms, NULL);
           
                    duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
                    duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
    
                    call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_Time = call_QE_LMDD_Single_Var_TraverseDown_C_free_of_Var_ProperLME_Time  + duration_ms;                

                    //cout<<"\nf->getRight() after simplifyLMDD = "<<getDAGStr(createDAGFromDD(thenb_after_lme_simplification))<<endl;
                    
                    thenb = QElim_WithLMISupport(VarToElim, thenb_after_lme_simplification, E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                    }
              }
            else//C is not an LMI of proper form. Eager lme based simplification inside QE_LMDD is not possible using C
              {
                thenb = QElim_WithLMISupport(VarToElim, f->getRight(), E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
              }             
              
          }
          else // no eager lme based simplification inside QE_LMDD
          {
            thenb = QElim_WithLMISupport(VarToElim, f->getRight(), E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
          }
          
           DDNode *elseb = QElim_WithLMISupport(VarToElim, f->getLeft(), E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);

          DDNode* C_dd = createDD(C, WidthTable);

          temp = createITE(C_dd, thenb, elseb);

          insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);

          return temp;
        }
      else
        {
         
         call_QE_LMDD_Single_Var_TraverseDown_C_bound_with_Var++;
         
	  //cout<<"\nPredicate not free of "<<VarToElim<<endl;

          DDNode *temp, *thenb, *elseb;

         if(C->Get_node_name()=="=" || C->Get_node_name()=="equivalent")//LME
            {
            set<DAGNode*> E_new, D_new;  
              
            copySet(E_new, E);
            E_new.insert(C);
            
            copySet(D_new, D);
            D_new.insert(negateConstraint(C));            
            

            thenb = QElim_WithLMISupport(VarToElim, f->getRight(), E_new, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            elseb = QElim_WithLMISupport(VarToElim, f->getLeft(), E, D_new, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            }
          else//LMI
            {
            set<DAGNode*> I_new_then, I_new_else;
                          
            copySet(I_new_then, I);
            I_new_then.insert(C);
            
            copySet(I_new_else, I);
            I_new_else.insert(negateConstraint_With_LMISupport(C));            
            

            thenb = QElim_WithLMISupport(VarToElim, f->getRight(), E, D, I_new_then, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            elseb = QElim_WithLMISupport(VarToElim, f->getLeft(), E, D, I_new_else, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            }

           temp = apply("or", thenb, elseb);

          insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);

          return temp;
        }//else of   if(freeOfVarModified(C, VarToElim))
    }//Non-terminal node
}//function




// Find \exists VarToElim. (/\ E /\ D /\ I), convert the result to LMDD, store the result in hash table, and return the result
DDNode* DDManager::callProjectReturningLMDD(set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, string VarToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
   
  DDNode* Existing  = getEntryFromTableForAPResolveModified_WithLMISupport(VarToElim, E, D, I, PtrTableForAPResolveModified);

  if(Existing != NULL)
    {
      //cout<<"\nAPResolve's hash table hit\n";
      return Existing;
    }


  DDNode* resultDD = ProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);

  insertEntryIntoTableForAPResolveModified_WithLMISupport(VarToElim, E, D, I, resultDD, PtrTableForAPResolveModified);

  return resultDD;

}




// Find \exists VarToElim. (/\ E /\ D /\ I), convert the result to LMDD, and return the result
DDNode* DDManager::ProjectReturningLMDD(set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, string VarToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
   
      // Local versions of the conjunction-level data start here
    int PresentConjunctionNumberLocal=0;
    int LMINoVarsToElimLocal=0;
    int LMINoLMIsLocal=0;
    int LMINoLMEsLocal=0;
    int LMINoLMDsLocal=0;
    int LMINoVarsLocal=0;
    unsigned long long int LMITotalBitsLocal=0;
    unsigned long long int LMIBitsToElimLocal=0;
    int LMIVarsInputToLayer2Local=0;
    int LMIVarsInputToLayer3Local=0;
    int LMIVarsInputToLayer4Local=0;
    unsigned long long int LMILayer1TimeLocal=0;
    unsigned long long int LMILayer2TimeLocal=0;
    unsigned long long int LMILayer3TimeLocal=0;
    unsigned long long int LMILayer4TimeLocal=0;
    unsigned long long int SimpleSMTChecksTimeBeforeLayer3=0;
    // Local versions of the conjunction-level data end here  
    
    // Step 0: vector<Constraints> <--- E \union D \union I;
   set<string> VarsToElim;
   VarsToElim.insert(VarToElim);
    
    set<DAGNode*> Constraints_Set;
    vector<DAGNode*> Constraints;
    
    set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints_Set, Constraints_Set.begin()));
    set_union(Constraints_Set.begin(), Constraints_Set.end(), I.begin(), I.end(),inserter(Constraints_Set, Constraints_Set.begin()));
    
    copySetToVector(Constraints_Set, Constraints);

     // Step 1: Convert strict LMIs to weak LMIs
   //showAPVector("Original Constraints", Constraints);
   convertStrictLMIsToWeakLMIs_WithLMDESupport(Constraints, WidthTable, VarsToElim);
   convertLMDEsToProperForm(Constraints, WidthTable);
   //showAPVector("Constraints after converting strict LMIs to weak LMIs and LMDEs to proper form", Constraints);
   
   // Step 2: Synthesize LMEs from LMIs
  
   
   tryToSynthesizeLMEsFromLMIs(Constraints,  VarsToElim,  WidthTable);
   //showAPVector("Constraints after LME synthesis", Constraints);      
  
  //showSet(VarsToElim, "VarsToElim", LMIQEOutputDataFp); 
  
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  getFreeAndBoundConstraintsForMultipleVariables(Constraints, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
  
  //showSet(VarsToElim, "VarsToElim");
  //showAPSet("Free_Constraints", Free_Constraints);
  //showAPSet("Bound_Constraints", Bound_Constraints);
 
  DAGNode* Free_Part = getConjunctionFromSet(Free_Constraints);
  
  //showAPSet("Free_Constraints", Free_Constraints, LMIQEOutputDataFp);
  //showAPSet("Bound_Constraints", Bound_Constraints, LMIQEOutputDataFp);
  
  
  // get details on #LMIs, #vars, #vars to elim, #bits, #bits to elim and set
  // in the respective global variables
  getRelevantLMIDetails_With_Recursive_Splitting(Bound_Constraints, VarsToElim, WidthTable, LMINoVarsToElimLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMITotalBitsLocal, LMIBitsToElimLocal, PresentConjunctionNumberLocal);
  
  
  DAGNode* Phi = getConjunctionFromSet(Bound_Constraints);
  
  if(output_conjunctions && project_depth == 0)
	{
        vector<DAGNode*> Bound_Constraints_Vector;
        
        for(set<DAGNode*>::iterator it = Bound_Constraints.begin(); it != Bound_Constraints.end(); it++)
            {
           Bound_Constraints_Vector.push_back(*it); 
            }
	createConjunctionAndPrintInSMTLibFormat(Bound_Constraints_Vector, VarsToElim, WidthTable);
	}
  
      //Apply Layer1 
  
     //cout<<"\nLayer 1 called\n";
      
      DAGNode* Simplified_Bound_Part;
      
      if(!SwitchOffLayer1)
        {
          
         struct timeval layer1_start_ms, layer1_finish_ms;                  
         gettimeofday (&layer1_start_ms, NULL); 
    
         Simplified_Bound_Part = applyLMILayer1(Bound_Constraints, VarsToElim, WidthTable);
        
        gettimeofday (&layer1_finish_ms, NULL);
        LMILayer1TimeLocal = layer1_finish_ms.tv_sec * 1000 + layer1_finish_ms.tv_usec / 1000;
        LMILayer1TimeLocal -= layer1_start_ms.tv_sec * 1000 + layer1_start_ms.tv_usec / 1000;    
        }
      else
        {
        Simplified_Bound_Part =   getConjunctionFromSet(Bound_Constraints);
        }
      
      LMIVarsInputToLayer2Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part, VarsToElim);           
      
      if(LMIVarsInputToLayer2Local == 0) //Layer1 is successful in eliminating all the quantifiers
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, Simplified_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
          
          
          fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, SimpleSMTChecksTimeBeforeLayer3);
        
          
          if(project_depth == 0)
            {
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);        
               
               
               fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, SimpleSMTChecksTimeBeforeLayer3);        
               
               
               
               FMTypeConjunctionCounter++;
             }
          else
            {
             fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
            AdditionalConjunctionCounter ++; 
            }         
          
         //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          
          
          return createDDFromDAGWithoutImplicationCheck(resultDAG, WidthTable);
       }// if of  Layer1 is successful in eliminating all the quantifiers
      
      
      //cout<<"\nLMILayer1TimeLocal = "<<LMILayer1TimeLocal<<endl;
      //cout<<"\nLMIVarsInputToLayer2Local = "<<LMIVarsInputToLayer2Local<<endl;
      //cout<<"\nResult of Layer1 = "<<getDAGStr(Simplified_Bound_Part)<<endl;      
      
      // Call Layer2
                  
      DAGNode* QFree_Bound_Part;
      DAGNode* Quantified_Bound_Part;
      
      // Simplified_Bound_Part is the conjunction of 
      // QFree_Bound_Part and Quantified_Bound_Part
      // Former quantifier free and the latter quantified
      // First let us split Simplified_Bound_Part into QFree_Bound_Part and Quantified_Bound_Part
      
      convertToBoundAndFreeParts(Simplified_Bound_Part, VarsToElim, QFree_Bound_Part, Quantified_Bound_Part, WidthTable);
      //cout<<"\nQFree_Bound_Part in result of Layer1 = "<<getDAGStr(QFree_Bound_Part)<<endl;
      //cout<<"\nQuantified_Bound_Part in result of Layer1 = "<<getDAGStr(Quantified_Bound_Part)<<endl;
      
      // Apply Layer2
      
      //cout<<"\nLayer 2 called\n";
      
      // First split the conjunction Quantified_Bound_Part into set of atomic predicates
      set<DAGNode*>  Quantified_Bound_Part_Constraint_Set;
      splitConjunctionIntoSetGeneral(Quantified_Bound_Part, Quantified_Bound_Part_Constraint_Set);
      
      
      DAGNode* Simplified_Bound_Part_After_Layer2;
      if(!SwitchOffLayer2)
        {
          
         struct timeval layer2_start_ms, layer2_finish_ms;                  
         gettimeofday (&layer2_start_ms, NULL); 
    
         Simplified_Bound_Part_After_Layer2 = applyLMILayer2Recursive(Quantified_Bound_Part_Constraint_Set, VarsToElim, WidthTable);
         //cout<<"\nSimplified_Bound_Part_After_Layer2 = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;
         
        Simplified_Bound_Part_After_Layer2 = singleLMEElimination(Simplified_Bound_Part_After_Layer2);
        //cout<<"\nSimplified_Bound_Part_After_Layer2 after singleLMEElimination = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;
        
        gettimeofday (&layer2_finish_ms, NULL);
        LMILayer2TimeLocal = layer2_finish_ms.tv_sec * 1000 + layer2_finish_ms.tv_usec / 1000;
        LMILayer2TimeLocal -= layer2_start_ms.tv_sec * 1000 + layer2_start_ms.tv_usec / 1000;    
        }
      else
        {
        Simplified_Bound_Part_After_Layer2 =   getConjunctionFromSet(Quantified_Bound_Part_Constraint_Set);
        }
      
      LMIVarsInputToLayer3Local = findNumberOfVarsToElimRemaining(Simplified_Bound_Part_After_Layer2, VarsToElim);   
      
       if(LMIVarsInputToLayer3Local == 0) //Layer2 is successful in eliminating all the quantifiers
          // Effectively Layer2 did: \exists VarToElim.(Quantified_Bound_Part) \equiv True
        { 
          DAGNode* resultDAG; //Result of QE 
          resultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, QFree_Bound_Part, LOGICAL_SYMBOL, 1));
          
          fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
          
          
          fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, SimpleSMTChecksTimeBeforeLayer3);
         
          
          
          if(project_depth == 0)
            {
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);        
               
               
               fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, SimpleSMTChecksTimeBeforeLayer3);        
              
               
               
               FMTypeConjunctionCounter++;
             }
          else
            {
             fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
            AdditionalConjunctionCounter ++; 
            }         
          
          //cout<<"\nLMILayer2TimeLocal = "<<LMILayer2TimeLocal<<endl;
          //cout<<"\nLMIVarsInputToLayer3Local = "<<LMIVarsInputToLayer3Local<<endl;
          //cout<<"\nResult of QE = "<<getDAGStr(resultDAG)<<endl;
          
          return createDDFromDAGWithoutImplicationCheck(resultDAG, WidthTable);
       }// if of  Layer2 is successful in eliminating all the quantifiers
      
      
     //cout<<"\nLMILayer2TimeLocal = "<<LMILayer2TimeLocal<<endl;
     //cout<<"\nLMIVarsInputToLayer3Local = "<<LMIVarsInputToLayer3Local<<endl;
     //cout<<"\nResult of Layer2 = "<<getDAGStr(Simplified_Bound_Part_After_Layer2)<<endl;   
     
     // Calling Layer4: Note that we do not have Layer3 now
     
    DAGNode *InputToLayer3 = Simplified_Bound_Part_After_Layer2;
      
    DAGNode* PartialResultDAG;
    PartialResultDAG = quickSimplify_WithLMISupport(createDAG("and", Free_Part, QFree_Bound_Part, LOGICAL_SYMBOL, 1));
    //cout<<"\nPartialResultDAG = "<<getDAGStr(PartialResultDAG)<<endl;
    //cout<<"\nWe still need to eliminate quantifiers from "<<getDAGStr(InputToLayer3)<<endl;    
    
    if(SimpleSMTChecksBeforeLayer3)
                {   
                    fprintf(LMIQEOutputDataFp, "\nPerforming simple checks on %s\n", getDAGStr(InputToLayer3).c_str());
                    
                    set<string> VarsToElim_In_InputToLayer3;
                    
                    set<string> Support_InputToLayer3;
                    getDAGNames(InputToLayer3, Support_InputToLayer3);
                    
                    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_InputToLayer3.begin(), Support_InputToLayer3.end(),inserter(VarsToElim_In_InputToLayer3, VarsToElim_In_InputToLayer3.begin()));
                                        
                    showSet(VarsToElim_In_InputToLayer3, "VarsToElim_In_InputToLayer3", LMIQEOutputDataFp);
                    
                    struct timeval checks_start_ms, checks_finish_ms;
                    gettimeofday (&checks_start_ms, NULL);    
                    
                    string Result_Checks = simpleSMTChecks(InputToLayer3, VarsToElim_In_InputToLayer3, WidthTable);
                    
                    gettimeofday (&checks_finish_ms, NULL);
                    SimpleSMTChecksTimeBeforeLayer3 = checks_finish_ms.tv_sec * 1000 + checks_finish_ms.tv_usec / 1000;
                    SimpleSMTChecksTimeBeforeLayer3 -= checks_start_ms.tv_sec * 1000 + checks_start_ms.tv_usec / 1000;
                    
                    fprintf(LMIQEOutputDataFp, "\nResult of the checks is %s\n", Result_Checks.c_str());
                    
                    if(Result_Checks == "valid" || Result_Checks == "unsat")//exists VarsToElim. (InputToLayer3) is True
                        // or exists VarsToElim. (InputToLayer3) is False
                    {
                      fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                      fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
               
               
                      fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                      fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, SimpleSMTChecksTimeBeforeLayer3);
               
               
                      if(project_depth==0)
                        {
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                         
                         
                         fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t0\t0\t0\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, SimpleSMTChecksTimeBeforeLayer3);
                        
                         
                         FMTypeConjunctionCounter++;
                        }
                    else
                        {
                         fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                         fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                     
                         AdditionalConjunctionCounter++;
                        }
               
                    if(Result_Checks == "valid")//exists VarsToElim. (InputToLayer3) is True
                        {
                           return createDDFromDAGWithoutImplicationCheck(PartialResultDAG, WidthTable);
                        }
                    else//if(Result_Checks == "unsat")
                        {
                          return createDD(0);//Return false
                        }                    
                    }//if(Result_Checks == "valid" || Result_Checks == "unsat")
                } //if(SimpleSMTChecksBeforeLayer3)    

    DAGNode* InputToLayer4 = InputToLayer3;
    LMIVarsInputToLayer4Local = findNumberOfVarsToElimRemaining(InputToLayer4, VarsToElim);  
    
    //cout<<"\nLayer 4 called\n";
    //fprintf(LMIQEOutputDataFp, "\nCalling the expensive layers in ProjectReturningLMDD\n");   
    fprintf(LMIQEOutputDataFp, "\nCalling expensive layers for QE from %s\n", getDAGStr(InputToLayer4).c_str());
       
         
    struct timeval layer4_start_ms, layer4_finish_ms;
    gettimeofday (&layer4_start_ms, NULL);    
                                  
                 
    vector<DAGNode*> InputToLayer4_Vector;
    splitConjunctionIntoVectorGeneral(InputToLayer4, InputToLayer4_Vector);
    
    if(VarsToElim.size() > 1)
        {
        cout<<"\nError in function ProjectReturningLMDD in DDManager.cpp. call_applyLMILayer4_ReturningLMDD should be called with only a single variable\n";
        exit(1);
        }
    
    set<string>::iterator vit = VarsToElim.begin();
    string VariableSelected = *vit;
    
    //cout<<"\nVariable Selected for Elimination by Layer4 = "<<VariableSelected<<endl;
    
    unsigned long long int FMTime=0;
    unsigned long long int NormalizationTime=0;
    unsigned long long int MonTime=0;
                 
    DDNode* RemainingResult = call_applyLMILayer4_ReturningLMDD(InputToLayer4_Vector, VariableSelected, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth, FMTime, NormalizationTime, MonTime);  
                     
    gettimeofday (&layer4_finish_ms, NULL);
    LMILayer4TimeLocal = layer4_finish_ms.tv_sec * 1000 + layer4_finish_ms.tv_usec / 1000;
    LMILayer4TimeLocal -= layer4_start_ms.tv_sec * 1000 + layer4_start_ms.tv_usec / 1000;
                 
    //cout<<"\nLMILayer4TimeLocal = "<<LMILayer4TimeLocal<<endl;
                      
    if(checkTimeOut())
          {
          fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
          fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

          
           fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
           fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, SimpleSMTChecksTimeBeforeLayer3);

          
             if(project_depth == 0)
             {
                 if(false)
                    {
                    fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                    fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
                    
                    
                    fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
                    fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, SimpleSMTChecksTimeBeforeLayer3);
                    
                    
                    FMTypeConjunctionCounter++;
                    }
             }
             else
             {
               fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
               fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\tunknown\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
              AdditionalConjunctionCounter++;
             }
             // Time Out !!!
          cout<<"\nWarning!!!The function ProjectReturningLMDD in Eliminator.cpp has timed out. Its result is not exact\n";
          timed_out = true; // timed_out flag set
          return createDD(1);//Return true   
         }

   fprintf(LMIQEImportantDataFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
   fprintf(LMIQEImportantDataFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);

   
   fprintf(TimeAnalysisFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), PresentConjunctionNumberLocal, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
   fprintf(TimeAnalysisFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, SimpleSMTChecksTimeBeforeLayer3);

   
   
   if(project_depth==0)
            {
             fprintf(LMIQEImportantDataFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(LMIQEImportantDataFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
             
             
             fprintf(TimeAnalysisFourierMotzkinFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(TimeAnalysisFourierMotzkinFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal, FMTime, NormalizationTime, MonTime, SimpleSMTChecksTimeBeforeLayer3);
              
             
             FMTypeConjunctionCounter++;
            }
   else
            {
             fprintf(LMIQEImportantDataAdditionalFp, "\n\n%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%llu\t%llu", InFileName.c_str(), FMTypeConjunctionCounter, AdditionalConjunctionCounter, LMINoLMIsLocal, LMINoLMEsLocal, LMINoLMDsLocal, LMINoVarsLocal, LMINoVarsToElimLocal, LMITotalBitsLocal, LMIBitsToElimLocal);
             fprintf(LMIQEImportantDataAdditionalFp, "\t%d\t%d\t%d\t%llu\t%llu\t%llu\t%llu\n", LMIVarsInputToLayer2Local, LMIVarsInputToLayer3Local, LMIVarsInputToLayer4Local, LMILayer1TimeLocal, LMILayer2TimeLocal, LMILayer3TimeLocal, LMILayer4TimeLocal);
            AdditionalConjunctionCounter++;
            }

   
   DDNode* PartialResultDD = createDDFromDAGWithoutImplicationCheck(PartialResultDAG, WidthTable);
   
   return apply("and", RemainingResult, PartialResultDD);

}//end of function




// checks if \exists VarToElim. (/\ E /\ D /\ I) is already encountered
// return the result if yes; return NULL otherwise
DDNode* DDManager::getEntryFromTableForAPResolveModified_WithLMISupport(string VarToElim, set<DAGNode*> &E, set<DAGNode*> &D,  set<DAGNode*> &I, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = I.begin(); it != I.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForAPResolveModified->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// enter \exists VarToElim. (/\ E /\ D /\ I) ---> result into HashTable
void DDManager::insertEntryIntoTableForAPResolveModified_WithLMISupport(string VarToElim, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, DDNode* value, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];


  for(set<DAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<DAGNode*>::iterator it = D.begin(); it != D.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }
  
   for(set<DAGNode*>::iterator it = I.begin(); it != I.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForAPResolveModified->hashtable_search(key);


  if(result.success())
    {
      // Do nothing
      //cout<<"\nInside function DDManager::insertEntryIntoTableForAPResolveModified_WithLMISupport. Entry already exists for this node. Then why earlier search was not able to find this?\n";
      //exit(1);
    }
  else
  {
    result = PtrTableForAPResolveModified->hashtable_insert(key, value);
   
    if(!result.success())
        {
        cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForAPResolveModified_WithLMISupport\n";
        exit(1);
        }
    }//else
}





//  Version of  call_applyLMILayer4 which (1) returns a DDNode* and (2) eliminates a single variable
DDNode* DDManager::call_applyLMILayer4_ReturningLMDD(vector<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth, unsigned long long int &FMTime, unsigned long long int &NormalizationTime, unsigned long long int &MonTime)
{    
 DAGNode* F = getConjunctionFromVector(Constraints);
 
 set<DAGNode*> Constraints_Set;
 splitConjunctionIntoSetGeneral(F, Constraints_Set);
 
 
 set<DAGNode*> E;
 set<DAGNode*> D;
 set<DAGNode*> I;
 
 splitConstraintsIntoLMELMDLMISets(Constraints_Set, E,  D,  I);
 
        
 DDNode* Existing  = getEntryFromTableForAPResolveModified_WithLMISupport(VarToElim, E, D, I, PtrTableForAPResolveModified);

  if(Existing != NULL)
    {
      //cout<<"\nAPResolve's hash table hit\n";
      return Existing;
    }
 
 if(project_depth == 0)
                    {
                    set<string> VarsToElim_In_F;
                    
                    set<string> Support_F;
                    getDAGNames(F, Support_F);
                    
                    set<string> VarsToElim;
                    VarsToElim.insert(VarToElim);
                    
                    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_F.begin(), Support_F.end(),inserter(VarsToElim_In_F, VarsToElim_In_F.begin()));
                    attachInputToLayer3ToConstraintsFile(VarsToElim_In_F, F);
                   }


  DDNode* resultDD = applyLMILayer4_ReturningLMDD(F, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth, FMTime, NormalizationTime, MonTime);

  insertEntryIntoTableForAPResolveModified_WithLMISupport(VarToElim, E, D, I, resultDD, PtrTableForAPResolveModified);

  return resultDD;
}




// Version of applyLMILayer4 which (1) handles only one variable and (2) returns DDNode*
DDNode* DDManager::applyLMILayer4_ReturningLMDD(DAGNode* Phi, string VarToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth, unsigned long long int &FMTime, unsigned long long int &NormalizationTime, unsigned long long int &MonTime)
{
 //cout<<"\nCalling applyLayer4 on "<<getDAGStr(Phi)<<endl;
  
 // Convert >= constraints to <=.
 // Also recreate all the constraints with a proper variable ordering
 Phi = convertBVUGEConstraintsToBVULECorrected(Phi, WidthTable);
 //cout<<"\nConverted Phi = "<<getDAGStr(Phi)<<endl;
  
  set<DAGNode*> Constraints;
  
  splitConjunctionIntoSetGeneral(Phi, Constraints);
  //showAPSet("Constraints", Constraints);
  
  if(constraintsAlreadyInNormalForm(Constraints, VarToElim, WidthTable))
        {
        //cout<<"\nThe constraints are already in normal form\n";

        DAGNode* resultOfFM;
        
        struct timeval fm_start_ms, fm_finish_ms;
        gettimeofday (&fm_start_ms, NULL);    
        
        if(simpleFourierMotzkinCanBeApplied(Constraints, VarToElim, WidthTable))
            {
            resultOfFM = quickSimplify_WithLMISupport(applyFourierMotzkin(Constraints, VarToElim, WidthTable));
            }
        else
            {
            list<DAGNode*> APListForDD; 
            
            resultOfFM = quickSimplify_WithLMISupport(applyFourierMotzkinForEvenCoefficients(Constraints, VarToElim, WidthTable, APListForDD));
            
             if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                {
                   initializeAPMap(APListForDD, WidthTable);            
                }
            }  
        
        gettimeofday (&fm_finish_ms, NULL);
        FMTime = fm_finish_ms.tv_sec * 1000 + fm_finish_ms.tv_usec / 1000;
        FMTime -= fm_start_ms.tv_sec * 1000 + fm_start_ms.tv_usec / 1000;
        
        //cout<<"\nresultOfFM = "<<getDAGStr(resultOfFM)<<endl;
        return createDDFromDAGWithoutImplicationCheck(resultOfFM, WidthTable);
        }
    else
        {  
        //cout<<"\nThe constraints are not in normal form\n";
        // We need to eliminate VarsToElimNeedOfSplitting from Phi
      
      
        struct timeval norm_start_ms, norm_finish_ms;
        gettimeofday (&norm_start_ms, NULL);   
       
        DAGNode* NormalForm;
        
        set<string> VariablesSelectedForNormalization;
        VariablesSelectedForNormalization.insert(VarToElim);
        
        list<DAGNode*> APListForNormalizedDD;        
    
        NormalForm = quickSimplify_WithLMISupport(convertToNormalFormForMultipleVariablesCorrected(Constraints, VariablesSelectedForNormalization, WidthTable, APListForNormalizedDD));
        //cout<<"\nNormalForm = "<<getDAGStr(NormalForm)<<endl;   
        
        showSet(VariablesSelectedForNormalization, "VariablesSelectedForNormalization", LMIQEOutputDataFp);
        showAPSet("Constraints", Constraints, LMIQEOutputDataFp); 
        fprintf(LMIQEOutputDataFp, "\nNormal form = %s\n", getDAGStr(NormalForm).c_str());  
                
        if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
        {
            //cout<<"\nAPListForNormalizedDD\n";
            //showAPList(APListForNormalizedDD);
            //cout<<"\nThis part is not implemented\n";
            initializeAPMap(APListForNormalizedDD, WidthTable);            
        }
        
         DDNode* NormalFormDD = createDDFromDAGWithoutImplicationCheck(NormalForm, WidthTable);
         
         gettimeofday (&norm_finish_ms, NULL);
         NormalizationTime = norm_finish_ms.tv_sec * 1000 + norm_finish_ms.tv_usec / 1000;
         NormalizationTime -= norm_start_ms.tv_sec * 1000 + norm_start_ms.tv_usec / 1000;
                
        //cout<<"\nNormalFormDD  = "<<getDAGStr(createDAGFromDD(NormalFormDD))<<endl;
        //cout<<"\nSize of NormalFormDD = "<<getSizeOfDD(NormalFormDD)<<endl;
        
        //cout<<"\nCalling QElim_WithLMISupport to eliminate quantifiers from NormalForm\n";
         
        struct timeval mon_start_ms, mon_finish_ms;
        gettimeofday (&mon_start_ms, NULL);     
         
        
        set<DAGNode*> LMESet;
        set<DAGNode*> LMDSet;
        set<DAGNode*> LMISet;
   
        DDNode* QE_NormalFormDD = QElim_WithLMISupport(VarToElim, NormalFormDD, LMESet, LMDSet, LMISet, WidthTable, PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth+1);
    
        
        gettimeofday (&mon_finish_ms, NULL);
        MonTime = mon_finish_ms.tv_sec * 1000 + mon_finish_ms.tv_usec / 1000;
        MonTime -= mon_start_ms.tv_sec * 1000 + mon_start_ms.tv_usec / 1000;
        
        return QE_NormalFormDD;
        }
}//function




// simplifyLMDD with LMI Support
DDNode*  DDManager::DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(DDNode *dd, DAGNode *equation, string VarToElim, map<string, int> &WidthTable)
{ 
  RecursiveCallsToDAGResolve++;  
    
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms; 
    
  //cout<<"\nDagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport called\n";
    
  //Checking in HT for \exists VarToElim.(dd /\ equation) 

  bool UsePerVariableHT = true;
  
  if(!UsePerVariableHT)//old implementation
    {    
      map< pair<DDNode*, DAGNode*>, DDNode*>::iterator it = TableForDagResolve.find(make_pair(dd, equation));

      if(it != TableForDagResolve.end())
        {
          //cout<<"\nHit in dag-resolve hash table"<<endl; 
          return it->second;
        }
     //cout<<"\nMiss in dag-resolve hash table"<<endl;  
  }
  else //new implementation
  {
      gettimeofday (&start_ms, NULL);    
      
      DDNode* Existing = getEntryFromPerVariableHTForDagResolve_WithLMISupport(dd, equation, VarToElim);
      
      gettimeofday (&finish_ms, NULL);
      duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
      duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;    
      TimeSpentInHTLookUpInsideDAGResolve = TimeSpentInHTLookUpInsideDAGResolve + duration_ms;
                              
      if(Existing != NULL)        
         {
          //yes, get the hashed value
          //cout<<"\nHit in dag-resolve hash table"<<endl; 
          return Existing;
         }
      //cout<<"\nMiss in dag-resolve hash table"<<endl; 
  }  
  
  RecursiveCallsToDAGResolveMissed++;

  if(dd->getPredicate() == NULL) // Terminal
    {
      return dd;
    }

  RecursiveCallsToDAGResolveMissedNonTerminals++;
  
  DDNode *NewLeft = DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(dd->getLeft(), equation, VarToElim, WidthTable);
  DDNode *NewRight = DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(dd->getRight(), equation, VarToElim, WidthTable);

  DAGNode *MyPredicate = dd->getPredicate();

  DAGNode *NewPredicate;

  // Check if MyPredicate contains the variable to be eliminated
  bool UseEfficientVersionOffreeOfVarModified = true;
  
  bool MyPredicateFreeOfVarToElim;
  
  if(UseEfficientVersionOffreeOfVarModified)//Note that this procedure returns false even if MyPredicate has 0*VarToElim + ...
  {
     MyPredicateFreeOfVarToElim = freeOfVarModifiedEfficient(MyPredicate, VarToElim); 
  }
  else
  {
   MyPredicateFreeOfVarToElim = freeOfVarModified(MyPredicate, VarToElim);
  }

  if(MyPredicateFreeOfVarToElim)
    {
      RecursiveCallsToDAGResolveMissedNonTerminalsButFreeOfVarToElim++;
      
      NewPredicate = MyPredicate;
    }

  else if(!coeffIsOdd(equation, VarToElim)) // check if the coefficient of variable to be eliminated is odd in the equation selected
    {
      // Ideally, this should be done before selecting the equation
      NewPredicate = MyPredicate;
    }
 
  else
    {
      // Apply Layer1 to eliminate VarToElim from the predicate
      RecursiveCallsToDAGResolveLeadingToLayer1Call++;
      
      bool PerformSimplifyLMCWithHashing = true;
      
      if(PerformSimplifyLMCWithHashing)//Hashes the calls to Layer1 and tries to re-use them
        {
        NewPredicate = simplifyLMCInsideSimplifyLMDD(MyPredicate, equation, VarToElim, WidthTable);
        }
      else
        {
           set<DAGNode*> Constraints;
           Constraints.insert(equation);
           Constraints.insert(MyPredicate);
          
          //showAPSet("Constraints", Constraints);

          // Checking if Constraints is False before calling the eliminator

          set<DAGNode*> TemporaryDisEqSet;
          TemporaryDisEqSet.clear();
          set<DAGNode*> TemporaryInEqSet;
          TemporaryInEqSet.clear();

          gettimeofday (&start_ms, NULL);    

          bool theory_unsat = ThUnsat(Constraints, TemporaryDisEqSet, TemporaryInEqSet, WidthTable);

          gettimeofday (&finish_ms, NULL);
          duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
          duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;    
          TimeSpentInSMTCallsInsideDAGResolve = TimeSpentInSMTCallsInsideDAGResolve + duration_ms;


          if(theory_unsat)
            {
              NewPredicate = createLogicalConstant("false");
            }
          else
            {
            set<string> VarsToElim;
            VarsToElim.insert(VarToElim);

            gettimeofday (&start_ms, NULL);    

            NewPredicate = applyLMILayer1(Constraints, VarsToElim, WidthTable);

            gettimeofday (&finish_ms, NULL);
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;    
            TimeSpentInLayer1CallsInsideDAGResolve = TimeSpentInLayer1CallsInsideDAGResolve + duration_ms;

            if(NewPredicate->Get_node_name()=="=" || NewPredicate->Get_node_name()=="equivalent") // LME
                    {
                     NewPredicate = recreateEquationInProperForm(NewPredicate, WidthTable); // recreate in cannonical form
                    }
            if(NewPredicate->Get_node_name()=="=" || NewPredicate->Get_node_name()=="equivalent" ||
               NewPredicate->Get_node_name()=="bvule" || NewPredicate->Get_node_name()=="bvuge" ||
               NewPredicate->Get_node_name()=="bvult" || NewPredicate->Get_node_name()=="bvugt")
                    {
                     dynamicOrdering(MyPredicate, NewPredicate); // increment the indices of the predicates in APMap above MyPredicate by 1 and insert NewPredicate just on top of MyPredicate i.e. at MyPredicate's index + 1
                    }
            else if(isFalse(NewPredicate) || isTrue(NewPredicate))
                    {
                    //Fine
                    }
            else
                    {
                    cout<<"\nError in function DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport! The newly created predicate is not a constant or predicate\n";
                    exit(1);
                    }		
               }//else of  if(theory_unsat)
        }//else of if(PerformSimplifyLMCWithHashing)
    }//else of else if(!coeffIsOdd(equation, VarToElim))
  
  //cout<<endl<<"NewPredicate = "<<getDAGStr(NewPredicate)<<endl;

  // Note that in DD nodes, left is else and right is then

  gettimeofday (&start_ms, NULL);   
  
  DDNode* then_dd = NewRight;
  DDNode* els_dd = NewLeft;
  
  DDNode* codn_dd = createDDFromDAGWithoutImplicationCheck(NewPredicate, WidthTable);
  
  gettimeofday (&finish_ms, NULL);
  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;    
  TimeSpentInDDConversionInsideDAGResolve = TimeSpentInDDConversionInsideDAGResolve + duration_ms;
  
  gettimeofday (&start_ms, NULL);   
  
  DDNode* resultDD = createITE(codn_dd, then_dd, els_dd );
  
  gettimeofday (&finish_ms, NULL);
  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;    
  TimeSpentInDDCreationInsideDAGResolve = TimeSpentInDDCreationInsideDAGResolve + duration_ms;
  
  if(!UsePerVariableHT)//old implementation
    {    
    // Insert \exists VarToElim.(dd /\ equation) = resultDD  
    TableForDagResolve.insert(make_pair(make_pair(dd, equation), resultDD));
    }
  else //new implementation
    {
     // Insert \exists VarToElim.(dd /\ equation) = resultDD
     insertEntryIntoPerVariableHTForDagResolve_WithLMISupport(dd, equation, VarToElim, resultDD); 
    }

  return resultDD;
}



// Version of DDFreeOfVar with D.P. support
bool DDManager::DDFreeOfVar_WithDP(DDNode *dd, string VarToElim)
{
  HashTable <string, bool> HTForDDFreeOfVar;
  HashTable <string, bool> *PtrHTForDDFreeOfVar = &HTForDDFreeOfVar;

  bool DDIsFree = DDFreeOfVar_WithDP_Internal(dd,  VarToElim, PtrHTForDDFreeOfVar);
  return DDIsFree;
}

// Function called by DDFreeOfVar_WithDP_Internal
bool DDManager::DDFreeOfVar_WithDP_Internal(DDNode *dd, string VarToElim, HashTable <string, bool> *PtrHTForDDFreeOfVar)
{
  char temp_char[10];
  sprintf(temp_char , "%x", dd);
  string key = temp_char;
  key += VarToElim;
  
  // key created

  HTStatusValue<bool> ResultFromHT = PtrHTForDDFreeOfVar->hashtable_search(key);

  if(ResultFromHT.success())
    {
      //cout<<"\nHash table hit\n";
      return ResultFromHT.GetValue();
    }
  
  //cout<<"\nHash table miss\n";  
  bool ret_value;
  
  if(dd->getPredicate()!=NULL)
  {
      DAGNode *predicate = dd->getPredicate();
      
      if(!(freeOfVarModified(predicate, VarToElim))) 
      {
          ret_value = false;
      }
      else if(!DDFreeOfVar_WithDP_Internal(dd->getLeft(), VarToElim, PtrHTForDDFreeOfVar)) 
      {
          ret_value = false;
      }
      else if(!DDFreeOfVar_WithDP_Internal(dd->getRight(), VarToElim, PtrHTForDDFreeOfVar)) 
      {
          ret_value = false;
      }
      else
      {
      ret_value = true;      
      }
  }// if(dd->getPredicate()!=NULL)
  else
  {
       ret_value = true;
  }// if(dd->getPredicate()==NULL)
  
  ResultFromHT = PtrHTForDDFreeOfVar->hashtable_insert(key, ret_value);
  if(!ResultFromHT.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::DDFreeOfVar_WithDP_Internal\n";
      exit(1);
    }
  return ret_value;     
}



// Newly added on 1 April 2012 onwards ends here


// Newly added on 7 May 2012 starts here

// Returns an LMDD g such that g \vee PartialResult_In_LMDDWithMonniauxTypeSimplifications \equiv \exists VarsToElim. f
// f is an LMDD involving LMEs, LMDs, and LMIs
DDNode* DDManager::QuaLMoDE_WithLMISupport_WithMonniauxTypeSimplifications(list<string> VarsToElim, DDNode* f, map<string, int> &WidthTable)
{
  time_t t1, t2;
  
  set<string> VarSetToElim;
  copyListToSet(VarsToElim, VarSetToElim);

  DDNode *result;
  result = f;
  
  current_root_LMDD_node_in_lmdd_with_monniaux = result;
  
  FinalQEDoneByWhom = "lmdd";
  
  bool debug_monniaux_on_paths = false;
  // To check/debug how Monniaux performs if we try projection
  // on paths
  if(debug_monniaux_on_paths)
    {
    MonniauxOnLMDDPaths(VarSetToElim,  WidthTable, 0);  
    }

   while(!VarsToElim.empty() && result != NULL)
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function QuaLMoDE_WithLMISupport_WithMonniauxTypeSimplifications in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return result;
	}
      
      //showList(VarsToElim, "VarsToElim");
      
      time(&t1);

      string VarToElim = chooseVar(result, VarsToElim);

      cout<<"\nVariable chosen to eliminate = "<<VarToElim<<endl;
      fprintf(LMIQEOutputDataFp, "\nVariable chosen to eliminate = %s\n", VarToElim.c_str());
      
      OrderInWhichVariablesAreEliminated.push_back(VarToElim);
      
      time(&t2);

      cout<<"\n\nTime spent in finding var. to elim.  = "<< (t2 - t1)<<" SECONDS "<<endl<<endl;
      //fprintf(LMIQEOutputDataFp, "\nTime spent in finding var. to elim.  =   %d seconds\n", t2-t1);

      //cout<<"\nNumber of nodes in the input DD = "<<getSizeOfDD(result)<<endl;
      fprintf(LMIQEOutputDataFp, "\nNumber of nodes in the input DD = %d\n", getSizeOfDD(result));
      

      time(&t1);
      
      int project_depth = 0;
      
      if(!use_original_f_for_checking_in_lmdd_with_monniaux)//We should use the original f
        {
         current_root_LMDD_node_in_lmdd_with_monniaux = result;
        }
      
      
      if(DDFreeOfVar_WithDP(result, VarToElim))//result free of vartoelim
        {
          //no need to change result
          number_of_variables_for_which_no_elimination_needed++;
        }
      else if(use_path_based_strategy_in_lmdd_monniaux)
        {
         if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            cout<<"\nresult before variable elimination = "<<getDAGStr(createDAGFromDD(result))<<endl; 
            }
          
        result = callpathBasedQEStrategy_With_LMDD_Monniaux_Simplifications(VarSetToElim, VarToElim, result, WidthTable, project_depth);
        }      
      else
        {
        result = callQElim_WithLMISupport_WithMonniauxTypeSimplifications(VarSetToElim, VarToElim, result, WidthTable, project_depth);
        }
      
      time(&t2);

      cout<<"\n\nTime spent in callQElim_WithLMISupport_WithMonniauxTypeSimplifications to eliminate "<<VarToElim<<"  = "<< (t2 - t1)<<" SECONDS "<<endl<<endl;
      //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;
      fprintf(LMIQEOutputDataFp, "\nTime spent in callQElim_WithLMISupport_WithMonniauxTypeSimplifications to eliminate %s =   %d seconds\n", VarToElim.c_str(), t2-t1);
      
      if(result == NULL)//The entire result is in PartialResult; Monniaux finds the result
      {  
          FinalQEDoneByWhom = "monniaux";
          result = createDD(0);
          break;
      }
      
      
      //showDD(result);
 
      int NoOfDDNodes = 0;
      SetOfDDNodes.clear();
      getNoOfNodesInDD(result, NoOfDDNodes);

      cout<<"\nNumber of nodes in the output DD = "<<NoOfDDNodes<<endl;
      fprintf(LMIQEOutputDataFp, "\nNumber of nodes in the output DD = %d\n", NoOfDDNodes);

      if(NoOfDDNodes > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = NoOfDDNodes;
	}


      VarsToElim.remove(VarToElim); 

      //int userchar = getchar(); 
      //showAPMap();
    }

   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function QuaLMoDE_WithLMISupport_WithMonniauxTypeSimplifications in DDManager.cpp has timed out. Its result is not exact\n";
       timed_out = true; // timed_out flag set
       return result;
     }


  return result;
}


DDNode* DDManager::callQElim_WithLMISupport_WithMonniauxTypeSimplifications(set<string> &VarSetToElim, string VarToElim, DDNode* f, map<string, int> &WidthTable, int project_depth)
{ 
  TableForDagResolve.clear();

  set<DAGNode*> E, D, I, Neg_x_Context, Local_Neg_x_Context;
  E.clear(); D.clear(); I.clear();Neg_x_Context.clear();Local_Neg_x_Context.clear();


  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;

  DDNode* result;
  
  //cout<<"\nCalling call_LMDD_Monniaux_SingleVar\n";
  result = call_LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f, Neg_x_Context, Local_Neg_x_Context, E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
  //cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;
  
  if(result == NULL)//The entire result is in PartialResult; Monniaux finds the result
  {
      return NULL;
  }
  else
  {
     return result;
  }
}



// Returns an LMDD g such that g with PartialResult_In_LMDDWithMonniauxTypeSimplifications is \exists VarToElim. f
DDNode* DDManager::call_LMDD_Monniaux_SingleVar(set<string> &VarSetToElim, string VarToElim, DDNode* f, set<DAGNode*> &Neg_x_Context, set<DAGNode*> &Local_Neg_x_Context, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
  //cout<<"\nDDManager::call_LMDD_Monniaux_SingleVar called \n";
   time_t start, end, duration;
    
  call_LMDD_Monniaux_SingleVar_calls++;  
  fprintf(LMIQEOutputDataFp, "\nEntry into DDManager::call_LMDD_Monniaux_SingleVar\n");

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function call_LMDD_Monniaux_SingleVar in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}
  
  //showContext(f, E, D, I);   
  
  // Check if the result is 0
  if(f->getPredicate()==NULL) // Terminal node
    {
      //cout<<"\nTerminal node reached\n";

      int value = f->getVal();

      if(value == 0) //Terminal 0 reached
        {
	  //cout<<"\nresult=0 returned\n";
          fprintf(LMIQEOutputDataFp, "\nZero node\n");
          
         call_LMDD_Monniaux_SingleVar_calls_on_zero++;
          
          return createDD(0);
        } //Terminal 0 reached
      
    } // Terminal node  
  
  fprintf(LMIQEOutputDataFp, "\nNon-zero node\n");
  // check if result already exists
  DDNode* Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, PtrTableForQElimModified);

  if(Existing != NULL)
        {
        //cout<<"\nEntry existing in hash table returned\n";
        fprintf(LMIQEOutputDataFp, "\nEntry in HT returned\n");
        
        call_LMDD_Monniaux_SingleVar_calls_hits++;
              
        return Existing;
        }
  
  fprintf(LMIQEOutputDataFp, "\nNo entry in HT\n");
  
  map<string, string> Model_Of_SatCondition;
  bool ResultIsFalse;
  set<DAGNode*> CompleteContext_With_Path;
  
  if(!pure_lmdd_mode)
  {
  // Check if the result is needed under this context
  
  //cout<<"\nCheck if the result is needed under this context\n";
  
  set<DAGNode*> PartialContext;
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(PartialContext, PartialContext.begin()));
  set_union(PartialContext.begin(), PartialContext.end(), I.begin(), I.end(),inserter(PartialContext, PartialContext.begin()));
  set_union(PartialContext.begin(), PartialContext.end(), Local_Neg_x_Context.begin(), Local_Neg_x_Context.end(),inserter(PartialContext, PartialContext.begin()));
  
  
  DAGNode* PathCondition = createDAG("and", createDAGFromDD(f), getConjunctionFromSet(PartialContext), LOGICAL_SYMBOL, 1);
  //cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
   
  DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
  //cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
  
  DAGNode*  SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
  //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
  
  //fprintf(LMIQEOutputDataFp, "\nE and D and I and Local_Neg_x and f and not Pi checked\n");
  
  time(&start);
  
  ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Model_Of_SatCondition, WidthTable);
  
  time(&end);
  duration = end - start;
  call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_total_time = call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_total_time+duration;
  
  call_LMDD_Monniaux_SingleVar_calls_first_smt_calls++;
  call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_after_chunk_projects++;
  
  if(ResultIsFalse) // result is not needed under this context
  {
      //cout<<"\nexists x. (f wedge E wedge D wedge I) is not needed under Local_Neg_x_Context\n\nreturning 0\n";
      fprintf(LMIQEOutputDataFp, "\nE and D and I and Local_Neg_x and f and not Pi unsat; zero returned\n");
          
      call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat++;
      call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat_after_chunk_projects++;
          
      return createDD(0);
  }
  
  fprintf(LMIQEOutputDataFp, "\nE and D and I and Local_Neg_x and f and not Pi sat\n");
  
  if(check_effectiveness_of_first_smt_solver_call)
  {
      call_LMDD_Monniaux_SingleVar_calls_misses_on_sat++;
      
      SatisfiabilityCondition = createDAG("and", PathCondition, PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
      //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
      
      map<string, string> Temp_Model_Of_SatCondition;
      
      //fprintf(LMIQEOutputDataFp, "\nE and D and I and Local_Neg_x and f and Pi checked\n");
      
      ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
       if(ResultIsFalse)
       {
           fprintf(LMIQEOutputDataFp, "\nE and D and I and Local_Neg_x and f and Pi unsat\n");
           
           call_LMDD_Monniaux_SingleVar_calls_misses_on_sat_unsat_with_pi++;
       }
       else
       {
          fprintf(LMIQEOutputDataFp, "\nE and D and I and Local_Neg_x and f and Pi sat\n");
          
          call_LMDD_Monniaux_SingleVar_calls_misses_on_sat_sat_with_pi++;
       }
  }
  
  //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context\n";
  
  // result is needed under this context. Let's compute \exists x.(f \wedge E\wedge I)
  
  // Let's obtain the E \wedge D \wedge I \wedge C_{\neg x}
  
  set<DAGNode*> CompleteContext;
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(CompleteContext, CompleteContext.begin()));
  set_union(CompleteContext.begin(), CompleteContext.end(), I.begin(), I.end(),inserter(CompleteContext, CompleteContext.begin()));
  set_union(CompleteContext.begin(), CompleteContext.end(), Neg_x_Context.begin(), Neg_x_Context.end(),inserter(CompleteContext, CompleteContext.begin()));
  //showAPSet("CompleteContext", CompleteContext);  
  
  //cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;  
  if(check_f_condition_in_chunks_in_lmdd_with_monniaux && insufficientTruncationInLMDDWithMonniaux())
  {
      cout<<"\ncheck_f_condition_in_chunks_in_lmdd_with_monniaux && truncation insufficient\n";
      
      fprintf(LMIQEOutputDataFp, "\nChecking f and not Pi and Projecting in chunks of %d\n", f_condition_chunk_size_in_lmdd_with_monniaux);
      
      // resetting the counts  
      call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_after_chunk_projects=0;
      call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat_after_chunk_projects=0;       
        
      map<string, string> Temp_Model_Of_SatCondition;
      
      for(int chunk_it=1; chunk_it <= f_condition_chunk_size_in_lmdd_with_monniaux-1; chunk_it++)
      {
        cout<<"\nLoop iteration "<<chunk_it<<endl;
          
        NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        SatisfiabilityCondition = createDAG("and", createDAGFromDD(current_root_LMDD_node_in_lmdd_with_monniaux), NegPartialResult, LOGICAL_SYMBOL, 1); 
     
        time(&start);
        
        struct timeval smt_start_ms, smt_finish_ms;
        unsigned long long int SMTTime;
        gettimeofday (&smt_start_ms, NULL); 
      
        Temp_Model_Of_SatCondition.clear();
      
        ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
        
        gettimeofday (&smt_finish_ms, NULL);
        SMTTime = smt_finish_ms.tv_sec * 1000 + smt_finish_ms.tv_usec / 1000;
        SMTTime -= smt_start_ms.tv_sec * 1000 + smt_start_ms.tv_usec / 1000;     
        //cout<<"\nTime for SMT call = "<< SMTTime<<" milli seconds\n";
      
        time(&end);
        duration = end-start;        
        
        
        call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time = call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time + duration;
      
        call_LMDD_Monniaux_SingleVar_calls_second_smt_calls++;
     
        if(!ResultIsFalse) // sat
            {
            cout<<"\nf AND NOT Pi is sat\n";
            
            struct timeval start_ms, finish_ms;
            unsigned long long int duration_ms;
            
            gettimeofday (&start_ms, NULL);
            
            set<DAGNode*> Path;  
            //cout<<"\nCalling updateContextFromGivenAssignment\n";
            updateContextFromGivenAssignment(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, Path);
    
            gettimeofday (&finish_ms, NULL);
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
            
            //cout<<"\nTime to obtain path = "<<duration_ms<<" milli seconds\n";
            
            CompleteContext_With_Path.clear();
    
            copySet(CompleteContext_With_Path, Path);
            
            call_LMDD_Monniaux_SingleVar_calls_misses_with_project++;
      
            //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 
         
            gettimeofday (&start_ms, NULL);
            
            //cout<<"\nProject invoked\n";
            DAGNode* Pi_i;
            //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
            Pi_i = callProject(CompleteContext_With_Path, VarSetToElim, WidthTable, project_depth);
            //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";          
            
            gettimeofday (&finish_ms, NULL);
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
            
            //cout<<"\nTime for Project = "<<duration_ms<<" milli seconds\n";
            
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            //
            }
        else
            {
            cout<<"\nf AND NOT Pi is unsat\n";
            
            fprintf(LMIQEOutputDataFp, "\nf and not Pi unsat; returning NULL\n");
            
            return NULL;//The entire result is in PartialResult; Monniaux finds the result
            }//else of  if(!ResultIsFalse)
      }//for
     
   cout<<"\nLoop iteration "<<f_condition_chunk_size_in_lmdd_with_monniaux<<endl;   
      
    NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);   
      
    SatisfiabilityCondition = createDAG("and", createDAGFromDD(current_root_LMDD_node_in_lmdd_with_monniaux), NegPartialResult, LOGICAL_SYMBOL, 1); 
     
    time(&start);
      
    Temp_Model_Of_SatCondition.clear();
      
     ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
     time(&end);
     duration = end-start;
     call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time = call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time + duration;
      
     call_LMDD_Monniaux_SingleVar_calls_second_smt_calls++;
     
     if(!ResultIsFalse) // sat
        {
            cout<<"\nf AND NOT Pi is sat\n";
         
            set<DAGNode*> Path;  
            //cout<<"\nCalling updateContextFromGivenAssignment\n";
            updateContextFromGivenAssignment(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, Path);
    
            CompleteContext_With_Path.clear();
    
            copySet(CompleteContext_With_Path, Path);
        }
     else
        {
            cout<<"\nf AND NOT Pi is unsat\n";
            
            fprintf(LMIQEOutputDataFp, "\nf and not Pi unsat; returning NULL\n");
         
            return NULL;//The entire result is in PartialResult; Monniaux finds the result
        }  
      
  }//if(check_f_condition_in_chunks_in_lmdd_with_monniaux && insu.....
  else if(check_f_condition_in_lmdd_with_monniaux)
  {
     SatisfiabilityCondition = createDAG("and", createDAGFromDD(current_root_LMDD_node_in_lmdd_with_monniaux), NegPartialResult, LOGICAL_SYMBOL, 1); 
     
      time(&start);
      
     map<string, string> Temp_Model_Of_SatCondition;
      
     ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
     time(&end);
     duration = end-start;
     call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time = call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time + duration;
      
     call_LMDD_Monniaux_SingleVar_calls_second_smt_calls++;
     
     if(!ResultIsFalse) // sat
        {
     
            set<DAGNode*> Path;  
            //cout<<"\nCalling updateContextFromGivenAssignment\n";
            updateContextFromGivenAssignment(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, Path);
    
            CompleteContext_With_Path.clear();
    
            copySet(CompleteContext_With_Path, Path);
     }
     else
     {
            return NULL;//The entire result is in PartialResult; Monniaux finds the result
     }
  }
  else if(check_h_condition_in_lmdd_with_monniaux)
  {
      PathCondition = createDAG("and", createDAGFromDD(f), getConjunctionFromSet(CompleteContext), LOGICAL_SYMBOL, 1);
      //cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
   
     SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
     //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
     time(&start);
      
     map<string, string> Temp_Model_Of_SatCondition;
      
     ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
     time(&end);
     duration = end-start;
     call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time = call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time + duration;
      
     call_LMDD_Monniaux_SingleVar_calls_second_smt_calls++;
     
     if(!ResultIsFalse) // sat
        {
     
            set<DAGNode*> Path;  
            //cout<<"\nCalling updateContextFromGivenAssignment\n";
            updateContextFromGivenAssignment(f, Temp_Model_Of_SatCondition, WidthTable, Path);
    
            CompleteContext_With_Path.clear();
    
            set_union(Path.begin(), Path.end(), CompleteContext.begin(), CompleteContext.end(),inserter(CompleteContext_With_Path, CompleteContext_With_Path.begin()));
        }
     
  }  
  else if(check_path_condition_in_lmdd_with_monniaux)
  {
       set<DAGNode*> Path;    
        //cout<<"\nCalling updateContextFromGivenAssignment\n";
        updateContextFromGivenAssignment(f, Model_Of_SatCondition, WidthTable, Path);
        //showAPSet("Path", Path);
        
        CompleteContext_With_Path.clear();
        
        set_union(Path.begin(), Path.end(), CompleteContext.begin(), CompleteContext.end(),inserter(CompleteContext_With_Path, CompleteContext_With_Path.begin()));
  
        PathCondition = getConjunctionFromSet(CompleteContext_With_Path);
      
      SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
      
      //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
      time(&start);
      
      map<string, string> Temp_Model_Of_SatCondition;
      
      ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
      time(&end);
      duration = end-start;
      call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time = call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time + duration;
      
      call_LMDD_Monniaux_SingleVar_calls_second_smt_calls++;
  }
  else if(check_simplified_path_condition_in_lmdd_with_monniaux)
  {
      set<DAGNode*> Path;    
        //cout<<"\nCalling updateContextFromGivenAssignment\n";
      updateContextFromGivenAssignment(f, Model_Of_SatCondition, WidthTable, Path);
        //showAPSet("Path", Path);
      
       CompleteContext_With_Path.clear();
        
      set_union(Path.begin(), Path.end(), CompleteContext.begin(), CompleteContext.end(),inserter(CompleteContext_With_Path, CompleteContext_With_Path.begin()));
  
      PathCondition = getConjunctionFromSet(CompleteContext_With_Path);
        
      
      SatisfiabilityCondition = PathCondition;
      
       //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
      
       time(&start);
      
      map<string, string> Temp_Model_Of_SatCondition;
      
      call_LMDD_Monniaux_SingleVar_calls_to_simplified_path_condition++;
       
      ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
      if(!ResultIsFalse)
      {
          call_LMDD_Monniaux_SingleVar_calls_to_simplified_path_condition_sat++;
      }
      
      time(&end);
      duration = end-start;
      call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time = call_LMDD_Monniaux_SingleVar_calls_second_smt_calls_total_time + duration;
     
      
      call_LMDD_Monniaux_SingleVar_calls_second_smt_calls++;
  }
  else
    {
    ResultIsFalse = true;//Just move down; happens in   check_f_condition_in_chunks_in_lmdd_with_monniaux
    // when sufficient condition is false
    }
  }//if(!pure_lmdd_mode)
  else
  {
      ResultIsFalse = true; //so that it enters into unsat case in the pure LMDD mode
  }
   
   
  if(!ResultIsFalse) // sat
  {
      call_LMDD_Monniaux_SingleVar_calls_misses_with_project++;
      
     //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 
         
     //cout<<"\nProject invoked\n";
     DAGNode* Pi_i;
     //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
     Pi_i = callProject(CompleteContext_With_Path, VarSetToElim, WidthTable, project_depth);
     //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";
          
     PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
     //cout<<"\nPartialResult_In_LMDDWithMonniauxTypeSimplifications = "<<getDAGStr(PartialResult_In_LMDDWithMonniauxTypeSimplifications)<<"\n";
     
     set<DAGNode*> New_Local_Neg_x_Context;
     New_Local_Neg_x_Context.clear();
     
     //cout<<"\nTraversing down\n";
     
     fprintf(LMIQEOutputDataFp, "\nTraversing down\n");
     
     DDNode* result = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f, Neg_x_Context, New_Local_Neg_x_Context, E, D, I, WidthTable, Model_Of_SatCondition, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
     
     if(result == NULL)//The entire result is in PartialResult; Monniaux finds the result
    {
      return NULL;
    }
    
     
     insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, result, PtrTableForQElimModified);

     return result;        
  }
  else //unsat
  {
      set<DAGNode*> New_Local_Neg_x_Context;
      
      if(!pure_lmdd_mode)
      {
      call_LMDD_Monniaux_SingleVar_calls_misses_without_project++;
      //cout<<"\n! exists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 
      
      New_Local_Neg_x_Context.clear();
      }
      else
      {
        New_Local_Neg_x_Context = Local_Neg_x_Context;  
        //Model_Of_SatCondition is empty map here; already declared above
      }
     
      //cout<<"\nTraversing down\n";
      
      fprintf(LMIQEOutputDataFp, "\nTraversing down\n");
      
      DDNode* result = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f, Neg_x_Context, New_Local_Neg_x_Context, E, D, I, WidthTable, Model_Of_SatCondition, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
     
      if(result == NULL)//The entire result is in PartialResult; Monniaux finds the result
    {
      return NULL;
    }
      
      
      insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, result, PtrTableForQElimModified);

      return result;
  }
  
}
      


// calls Project
// Dynamic programming is not used, because,
// Constraints are the set of all constraints encountered on a path of an LMDD, and
// 1) There cannot be two identical paths in an LMDD and
// 2) Our algorithm will not call Project on the same path again
DAGNode* DDManager::callProject(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int project_depth)
{
    
 vector<DAGNode*> Constraints_Vector;   
    
 copySetToVector(Constraints, Constraints_Vector);
 
 DAGNode* Result;
 
 Result = Project(Constraints_Vector, VarsToElim, WidthTable, project_depth);
 
 return Result;    
}



DDNode* DDManager::LMDD_Monniaux_SingleVar(set<string> &VarSetToElim, string VarToElim, DDNode* f, set<DAGNode*> Neg_x_Context, set<DAGNode*> Local_Neg_x_Context, set<DAGNode*> E, set<DAGNode*> D, set<DAGNode*> I, map<string, int> &WidthTable, map<string, string> &Model_Of_SatCondition, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
  LMDD_Monniaux_SingleVar_calls++;  
  //cout<<"\nDDManager::LMDD_Monniaux_SingleVar called \n";

  if(checkTimeOut()) // check if we have already timed out
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function LMDD_Monniaux_SingleVar in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	}  
  
   if(pure_lmdd_mode || local_constraint_as_single_edge_mode)
    {
        DDNode* Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, f, E,  D, I, PtrTableForQElimModified);

        if(Existing != NULL)
                        {
                          //cout<<"\nEntry exists for the simplified dd in hash table\n";
                          return Existing;
                        }
   }
  

  if(f->getPredicate()==NULL) // Terminal node
    {
      //cout<<"\nTerminal node reached\n";

      DDNode* result;

      int value = f->getVal();

      if(value == 0) // 0-node
        {
          if(!pure_lmdd_mode)
          {
            cout<<"\nSatisfiable assignment leading to 0-node in function DDManager::LMDD_Monniaux_SingleVar\n";          
          
           exit(1);
          }
          else
          {
              result = createDD(0);
              insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, result, PtrTableForQElimModified);
              return result;
          }
          
        }// 0-node
      else // 1-node
        {        
        
          if(E.empty() && D.empty() && I.empty())
            {
	      //cout<<"\nContext empty. hence result=1\n";
              result = createDD(1);
            }
          else
            {
	      //cout<<"\nProject_ReturningLMDD invoked\n";
              result = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            }
         
          if(pure_lmdd_mode || local_constraint_as_single_edge_mode)
                {
                insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, result, PtrTableForQElimModified);
                }
          
          return result;
                  
        }// 1-node
     
    }//Terminal node
  else //Non-terminal node
    {
      //cout<<"\nNon-terminal node reached\n";
      // Check if f is already free of VarToElim
       if(DDFreeOfVar_WithDP(f, VarToElim))
                        {
                          //cout<<"\n"<<getDAGStr(createDAGFromDD(f))<<" is free of "<<VarToElim<<"\n";
                          
                          DDNode *temp;
                          
                          //cout<<"\nProject_ReturningLMDD invoked\n";
                           temp = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                          
                           temp = apply("and", f, temp);  
                           
                           insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);
                           
                           return temp;
                        }// if(DDFreeOfVar_WithDP(f, VarToElim))
      
      //cout<<"\n"<<getDAGStr(createDAGFromDD(f))<<" is not free of "<<VarToElim<<"\n";
      // Check if f can be simplified using LMEs
       if(DAGResolveApplicable(E, D, WidthTable))
                {
                  //cout<<"\nDAGResolve is applicable\n";

                  DAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable);
                  
                  if(e1 != NULL)
                    {
                      //cout<<"\nEquation selected is "<<getDAGStr(e1)<<endl;

                      //cout<<"\nOriginal DD before DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

                      //cout<<"\nSize of original dd = "<<getSizeOfDD(f)<<endl;
                                   

                      f = DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(f, e1, VarToElim, WidthTable);

                      //cout<<"\nDD after DAGResolve = "<<getDAGStr(createDAGFromDD(f))<<endl;

                      //cout<<"\nSize of DAGResolved dd = "<<getSizeOfDD(f)<<endl;

                      //f is simplified here. Check if there exists an entry with this simplfied version of f

                      DDNode* Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, f, E,  D, I, PtrTableForQElimModified);

                      if(Existing != NULL)
                        {
                          //cout<<"\nEntry exists for the simplified dd in hash table\n";
                          return Existing;
                        }

                      if(DDFreeOfVar_WithDP(f, VarToElim))
                        {
                          //cout<<"\nEntry does not exist for the simplified dd in hash table\n";
                          
                          DDNode *temp;
                          
                          //cout<<"\nProject_ReturningLMDD invoked\n";
                           
                          temp = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                          
                          //cout<<"\ncallProjectReturningLMDD on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

                          temp = apply("and", f, temp);                          
           

                          insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);

                          return temp;
                        }// if(DDFreeOfVar_WithDP(f, VarToElim))
                    }//if(e1 != NULL)
                }// if(DAGResolveApplicable(E, D, WidthTable))
       

      // Traverse down
     DAGNode* C = f->getPredicate();
     //cout<<"\nPredicate encountered = "<<getDAGStr(C)<<endl;     

      if(freeOfVarModified(C, VarToElim))
        {
	//cout<<"\nPredicate free of "<<VarToElim<<endl;
                  
        set<DAGNode*> Neg_x_Context_new_then, Neg_x_Context_new_else;
        
        copySet(Neg_x_Context_new_then, Neg_x_Context);
        Neg_x_Context_new_then.insert(C);
            
        copySet(Neg_x_Context_new_else, Neg_x_Context);
        Neg_x_Context_new_else.insert(negateConstraint_With_LMISupport(C));  
        
        set<DAGNode*> Local_Neg_x_Context_new_then, Local_Neg_x_Context_new_else;
            
        if(!local_constraint_as_single_edge_mode)
            {
            copySet(Local_Neg_x_Context_new_then, Local_Neg_x_Context);
            }
        Local_Neg_x_Context_new_then.insert(C);
            
        if(!local_constraint_as_single_edge_mode)
            {
            copySet(Local_Neg_x_Context_new_else, Local_Neg_x_Context);
            }
        Local_Neg_x_Context_new_else.insert(negateConstraint_With_LMISupport(C));  

        DDNode *thenb;
        DDNode *elseb;
        DDNode* C_dd;
        DDNode *temp;
        
          
        if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
          {
              //C is True under given model
              call_LMDD_Monniaux_SingleVar_calls++;  
              thenb = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getRight(), Neg_x_Context_new_then, Local_Neg_x_Context_new_then, E,  D,  I,  WidthTable, Model_Of_SatCondition, PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
               if(thenb == NULL)
              {
                  return NULL;//The entire result is in PartialResult
              }
              
              
              //call_LMDD_Monniaux_SingleVar_calls is incremented inside call_LMDD_Monniaux_SingleVar
              elseb = call_LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getLeft(), Neg_x_Context_new_else, Local_Neg_x_Context_new_else, E,  D,  I,  WidthTable,  PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
              if(elseb == NULL)
              {
                  return NULL;//The entire result is in PartialResult
              }
              
              C_dd = createDD(C, WidthTable);                            
          }
          else
          {
               //C is False under given model
              call_LMDD_Monniaux_SingleVar_calls++;  
              elseb = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getLeft(), Neg_x_Context_new_else, Local_Neg_x_Context_new_else, E,  D,  I,  WidthTable,  Model_Of_SatCondition, PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
               if(elseb == NULL)
              {
                  return NULL;//The entire result is in PartialResult
              }
              
              
              //call_LMDD_Monniaux_SingleVar_calls is incremented inside call_LMDD_Monniaux_SingleVar
              thenb = call_LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getRight(), Neg_x_Context_new_then, Local_Neg_x_Context_new_then, E,  D,  I,  WidthTable,  PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
              
               if(thenb == NULL)
              {
                  return NULL;//The entire result is in PartialResult
              }
              
              
              C_dd = createDD(C, WidthTable);    
          }
        
        
          if(!SMTCheckBeforeCombining_in_monniaux_type_simplification_in_dd)       
          {
            temp = createITE(C_dd, thenb, elseb);
          }
          else
          {
              set<DAGNode*> PartialContext;
              set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(PartialContext, PartialContext.begin()));
              set_union(PartialContext.begin(), PartialContext.end(), I.begin(), I.end(),inserter(PartialContext, PartialContext.begin()));
              set_union(PartialContext.begin(), PartialContext.end(), Local_Neg_x_Context.begin(), Local_Neg_x_Context.end(),inserter(PartialContext, PartialContext.begin()));
  
              DAGNode* PathCondition = createDAG("and", createDAGFromDD(f), getConjunctionFromSet(PartialContext), LOGICAL_SYMBOL, 1);
              //cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
              
              DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
              //cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
   
              DAGNode* SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
              //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
  
              map<string, string> Temp_Model;
      
              bool ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model, WidthTable);
              
              if(ResultIsFalse)
              {
                  //cout<<"\nSatisfiabilityCondition is unsat\n";
                  temp = createDD(0);
              }
              else
              {
                  //cout<<"\nSatisfiabilityCondition is sat\n";
                  temp = createITE(C_dd, thenb, elseb);
              }

            }// else of  if(!SMTCheckBeforeCombining) 
        
        if(pure_lmdd_mode || local_constraint_as_single_edge_mode)// Store the result
            {
            insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);
            }
     
          return temp;
        }
      else
        {
	  //cout<<"\nPredicate not free of "<<VarToElim<<endl;

          DDNode *temp, *thenb, *elseb;

         if(C->Get_node_name()=="=" || C->Get_node_name()=="equivalent")//LME
            {
            set<DAGNode*> E_new, D_new;  
              
            copySet(E_new, E);
            E_new.insert(C);
            
            copySet(D_new, D);
            D_new.insert(negateConstraint(C));   
            
            if(local_constraint_as_single_edge_mode)
                {
                Local_Neg_x_Context.clear();
                }
                
            
            if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
                {
                //C is True under given model
                call_LMDD_Monniaux_SingleVar_calls++;
                thenb = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getRight(), Neg_x_Context, Local_Neg_x_Context, E_new,  D,  I,  WidthTable,  Model_Of_SatCondition, PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
                 if(thenb == NULL)
              {
                  return NULL;//The entire result is in PartialResult
              }
                
                
                //call_LMDD_Monniaux_SingleVar_calls is incremented inside call_LMDD_Monniaux_SingleVar
                elseb = call_LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getLeft(), Neg_x_Context, Local_Neg_x_Context, E,  D_new,  I,  WidthTable,  PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
                
                 if(elseb == NULL)
                {
                  return NULL;//The entire result is in PartialResult
                }
              
                }
          else
                {
                 //C is False under given model
                call_LMDD_Monniaux_SingleVar_calls++;
                elseb = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getLeft(), Neg_x_Context, Local_Neg_x_Context, E,  D_new,  I,  WidthTable,  Model_Of_SatCondition, PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
                 if(elseb == NULL)
              {
                  return NULL;//The entire result is in PartialResult
              }
                
                
                //call_LMDD_Monniaux_SingleVar_calls is incremented inside call_LMDD_Monniaux_SingleVar
                thenb = call_LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getRight(), Neg_x_Context, Local_Neg_x_Context, E_new,  D,  I,  WidthTable,  PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
                
                 if(thenb == NULL)
              {
                  return NULL;//The entire result is in PartialResult
              }
                
               }            
            }//LME case ends here
          else//LMI
            {
            set<DAGNode*> I_new_then, I_new_else;
                          
            copySet(I_new_then, I);
            I_new_then.insert(C);
            
            copySet(I_new_else, I);
            I_new_else.insert(negateConstraint_With_LMISupport(C)); 
            
            if(local_constraint_as_single_edge_mode)
                {
                Local_Neg_x_Context.clear();
                }
            
            if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
                {
                //C is True under given model
                call_LMDD_Monniaux_SingleVar_calls++;
                thenb = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getRight(), Neg_x_Context, Local_Neg_x_Context, E,  D,  I_new_then,  WidthTable,  Model_Of_SatCondition, PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
                  if(thenb == NULL)
                {
                  return NULL;//The entire result is in PartialResult
                }
                
                //call_LMDD_Monniaux_SingleVar_calls is incremented inside call_LMDD_Monniaux_SingleVar
                elseb = call_LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getLeft(), Neg_x_Context, Local_Neg_x_Context, E,  D,  I_new_else,  WidthTable,  PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
                 if(elseb == NULL)
                {
                  return NULL;//The entire result is in PartialResult
                }
                
                }
           else
                {
                 //C is False under given model
                call_LMDD_Monniaux_SingleVar_calls++;
                elseb = LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getLeft(), Neg_x_Context, Local_Neg_x_Context, E,  D,  I_new_else,  WidthTable,  Model_Of_SatCondition, PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
              
                 if(elseb == NULL)
                {
                  return NULL;//The entire result is in PartialResult
                }
                
                //call_LMDD_Monniaux_SingleVar_calls is incremented inside call_LMDD_Monniaux_SingleVar
                thenb = call_LMDD_Monniaux_SingleVar(VarSetToElim, VarToElim, f->getRight(), Neg_x_Context, Local_Neg_x_Context, E,  D,  I_new_then,  WidthTable,  PtrTableForQElimModified,  PtrTableForAPResolveModified, project_depth);
                
                 if(thenb == NULL)
                    {
                  return NULL;//The entire result is in PartialResult
                    }
               }   

            }//LMI case ends here          
           
          if(!SMTCheckBeforeCombining_in_monniaux_type_simplification_in_dd)       
          {
             temp = apply("or", thenb, elseb);
          }
          else
          {
              set<DAGNode*> PartialContext;
              set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(PartialContext, PartialContext.begin()));
              set_union(PartialContext.begin(), PartialContext.end(), I.begin(), I.end(),inserter(PartialContext, PartialContext.begin()));
              set_union(PartialContext.begin(), PartialContext.end(), Local_Neg_x_Context.begin(), Local_Neg_x_Context.end(),inserter(PartialContext, PartialContext.begin()));
  
              DAGNode* PathCondition = createDAG("and", createDAGFromDD(f), getConjunctionFromSet(PartialContext), LOGICAL_SYMBOL, 1);
              //cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
              
              DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
              //cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
   
              DAGNode* SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
              //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
  
              map<string, string> Temp_Model;
      
              bool ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model, WidthTable);
              
              if(ResultIsFalse)
              {
                  //cout<<"\nSatisfiabilityCondition is unsat\n";
                  temp = createDD(0);
              }
              else
              {
                  //cout<<"\nSatisfiabilityCondition is sat\n";
                  temp = apply("or", thenb, elseb);
              }
           }// else of  if(!SMTCheckBeforeCombining)    
          
          if(pure_lmdd_mode || local_constraint_as_single_edge_mode)// Store the result 
            {
            insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, f, E, D, I, temp, PtrTableForQElimModified);
            }


           
          return temp;
        }//else of   if(freeOfVarModified(C, VarToElim))
    }//Non-terminal node
    
}//function
     


// Update the given context by traversing the LMDD f through the path guided by
// assignment in Model_In_LMDDWithMonniauxTypeSimplifications
void DDManager::updateContextFromGivenAssignment(DDNode* f, map<string, string> &Model_Of_SatCondition, map<string, int> &WidthTable, set<DAGNode*> &Context)
{
  if(f->getPredicate()==NULL) // Terminal node
    {
      int value = f->getVal();

      if(value == 0) // 0-node  
      {
          cout<<"\n0-node encountered in function DDManager::updateContextFromGivenAssignment while following a satisfiable path\n";
          exit(1);
      }
      else // 1-node
      {
          //do nothing
          return;
      }
    }// Terminal node
  else // Non-terminal node
  {
      DAGNode* C = f->getPredicate();
      
      if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
                {
                //C is True under given model
                Context.insert(C);
          
                updateContextFromGivenAssignment(f->getRight(), Model_Of_SatCondition, WidthTable, Context);
                 }
      else
                {
                // C is False under given model
                 Context.insert(negateConstraint_With_LMISupport(C));
                 
                 updateContextFromGivenAssignment(f->getLeft(), Model_Of_SatCondition, WidthTable, Context);
                }      
  }// Non-terminal node
}// function


// Used in LMDD+Monniaux style QE
// Checks if the truncation achieved by Monniaux in LMDD+Monniaux
// calls is insufficient (returns true) or sufficient (returns false)
bool DDManager::insufficientTruncationInLMDDWithMonniaux()
{
    cout<<"\ncall_LMDD_Monniaux_SingleVar_calls_misses_on_unsat_after_chunk_projects = "<<call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat_after_chunk_projects<<endl;
    cout<<"\ncall_LMDD_Monniaux_SingleVar_calls_first_smt_calls_after_chunk_projects = "<<call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_after_chunk_projects<<endl;
    
    if(call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_after_chunk_projects  < min_call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_for_satisfactory_truncation)
    {
        return false;
    }// allow the present activity to continue
    
    float present_truncation = (float)call_LMDD_Monniaux_SingleVar_calls_misses_on_unsat_after_chunk_projects/(float)call_LMDD_Monniaux_SingleVar_calls_first_smt_calls_after_chunk_projects;
    if(present_truncation >= satisfactory_truncation_in_lmdd_with_monniaux)//truncation insufficient
    {
        return false;
    }
    else
    {
        return true;
    }
}


// Function to try Monniaux on paths of LMDD current_root_LMDD_node_in_lmdd_with_monniaux
void DDManager::MonniauxOnLMDDPaths(set<string> &VarSetToElim,  map<string, int> &WidthTable, int project_depth)
{
    map<string, string> Temp_Model_Of_SatCondition;
    DAGNode* NegPartialResult;
    DAGNode* SatisfiabilityCondition;
    time_t start, end, duration;
    set<DAGNode*> CompleteContext_With_Path;
    bool ResultIsFalse;
      
      for(int chunk_it=1; chunk_it <= 100; chunk_it++)//Let's do 100 project operations 
      {
        cout<<"\nLoop iteration "<<chunk_it<<endl;
          
        NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        SatisfiabilityCondition = createDAG("and", createDAGFromDD(current_root_LMDD_node_in_lmdd_with_monniaux), NegPartialResult, LOGICAL_SYMBOL, 1); 
     
        time(&start);
      
        Temp_Model_Of_SatCondition.clear();
      
        ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
        time(&end);
        duration = end-start;
        
     
        if(!ResultIsFalse) // sat
            {
            cout<<"\nf AND NOT Pi is sat\n";
            
            set<DAGNode*> Path;  
            //cout<<"\nCalling updateContextFromGivenAssignment\n";
            updateContextFromGivenAssignment(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, Path);
    
            CompleteContext_With_Path.clear();
    
            copySet(CompleteContext_With_Path, Path);
            
            //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 
         
            //cout<<"\nProject invoked\n";
            DAGNode* Pi_i;
            //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
            Pi_i = callProject(CompleteContext_With_Path, VarSetToElim, WidthTable, project_depth);
            //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";          
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            //
            }
        else
            {
            cout<<"\nf AND NOT Pi is unsat\n";
            exit(1);
            }//else of  if(!ResultIsFalse)
      }//for
    
    exit(1);
}



// call QE using path based strategy with both Monniaux and LMDD type simplifications turned on
DDNode* DDManager::callpathBasedQEStrategy_With_LMDD_Monniaux_Simplifications(set<string> &VarSetToElim, string VarToElim, DDNode* f, map<string, int> &WidthTable, int project_depth)
{
  TableForDagResolve.clear();

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;

  DDNode* result;
  
  //cout<<"\nCalling pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
  result = pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications(VarSetToElim, VarToElim, f, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
  
  
  if(result == NULL)//The entire result is in PartialResult; Monniaux finds the result
  {
      return NULL;
  }
  else
  {
     return result;
  }
}


//QE using path based strategy with both Monniaux and LMDD type simplifications turned on
DDNode* DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications(set<string> &VarSetToElim, string VarToElim, DDNode* f, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
    map<string, string> Temp_Model_Of_SatCondition;
    DAGNode* NegPartialResult;
    DAGNode* SatisfiabilityCondition;
    time_t start, end, duration;
    set<DAGNode*> CompleteContext_With_Path;
    bool ResultIsFalse;
    struct timeval start_ms, finish_ms;
    unsigned long long int duration_ms;
    
    
    set<DAGNode*> E_Temp;
    set<DAGNode*> D_Temp;
    set<DAGNode*> I_Temp;    
    
    ext_select_strategy_calls++;
    
    if(selectStrategy(f, E_Temp, D_Temp, I_Temp, WidthTable, VarToElim,  VarSetToElim) == "lmdd")
           {               
              if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nQE_LMDD selected for QE\n";
                    }                  
               
             start_up_qe_lmdd_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
             //cout<<"\ncalling Qualmode_WithLMISupport\n";
             
             gettimeofday (&start_ms, NULL);
             
             DDNode* result = QElim_WithLMISupport(VarToElim, f, E_Temp, D_Temp, I_Temp, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
             
             gettimeofday (&finish_ms, NULL);
              
              duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
              duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
             
              start_up_time_spent_in_qe_lmdd = start_up_time_spent_in_qe_lmdd + duration_ms;
              
              time_spent_in_qe_lmdd = time_spent_in_qe_lmdd + duration_ms;   
              
              if(select_strategy_basis=="free")
               {
                ext_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar = ext_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar + duration_ms;
                
                ext_select_strategy_ddfreeofvar++;
               }
              else if(select_strategy_basis=="dagresolve")
              {
                ext_time_spent_in_qe_lmdd_for_select_strategy_dagresolve = ext_time_spent_in_qe_lmdd_for_select_strategy_dagresolve + duration_ms;   
                
                ext_select_strategy_dagresolve++;
              }
              else if(select_strategy_basis=="lowlevel")
              {
                ext_time_spent_in_qe_lmdd_for_select_strategy_lowlevel = ext_time_spent_in_qe_lmdd_for_select_strategy_lowlevel + duration_ms;   
                
                ext_select_strategy_lowlevel++;
              }
              else  if(select_strategy_basis=="size")
              {
                ext_time_spent_in_qe_lmdd_for_select_strategy_size = ext_time_spent_in_qe_lmdd_for_select_strategy_size + duration_ms;   
                
                ext_select_strategy_size++;
              }
              else  if(select_strategy_basis=="default")
              {
                ext_time_spent_in_qe_lmdd_for_select_strategy_default = ext_time_spent_in_qe_lmdd_for_select_strategy_default + duration_ms;   
                
                ext_select_strategy_for_lmdd_default++;
              }
              
              return result;
           }
    
    ext_select_strategy_for_monniax++;
    
    
    if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
    {
      cout<<"\nPi before starting elimination = "<<getDAGStr(PartialResult_In_LMDDWithMonniauxTypeSimplifications)<<endl;
     }
    
        
    if(do_monniaux_at_start_up_in_use_path_based_strategy_in_lmdd_monniaux)
    {
              
      for(int chunk_it=1; chunk_it <=monniaux_calls_at_start_up_in_use_path_based_strategy_in_lmdd_monniaux ; chunk_it++)//Let's do monniaux_calls_at_start_up_in_use_path_based_strategy_in_lmdd_monniaux project operations 
      {
        cout<<"\nLoop iteration "<<chunk_it<<endl;
          
        NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        if(use_dag_instead_of_lmdd_in_smt_calls_in_path_based_strategy_in_lmdd_monniaux)
            {
            SatisfiabilityCondition = createDAG("and", original_dag_in_path_based_strategy_in_lmdd_monniaux, NegPartialResult, LOGICAL_SYMBOL, 1); 
            }        
        else
            {
            SatisfiabilityCondition = createDAG("and", createDAGFromDD(current_root_LMDD_node_in_lmdd_with_monniaux), NegPartialResult, LOGICAL_SYMBOL, 1); 
            }
     
        time(&start);
        gettimeofday (&start_ms, NULL);
      
        Temp_Model_Of_SatCondition.clear();
      
        ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
      
        gettimeofday (&finish_ms, NULL);
        time(&end);
        duration = end-start;
        
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        
        time_in_start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux = time_in_start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux + duration_ms;
        
        start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux++;
        
     
        if(!ResultIsFalse) // sat
            {
            cout<<"\nORIGINAL f AND NOT Pi is sat\n";
            
            gettimeofday (&start_ms, NULL);
            
            if(PerformProjectsInPathBasedStrategyAtLMDDLevel)
            {
             set<DAGNode*> Path;
             
             DDNode* EasyNode;
             
              //cout<<"\nCalling updateContextFromGivenAssignmentUptoEasyNode\n";
             
             if(use_dag_instead_of_lmdd_in_smt_calls_in_path_based_strategy_in_lmdd_monniaux)
                {
                updateContextFromGivenAssignmentUptoEasyNode(f, Temp_Model_Of_SatCondition, WidthTable, Path, EasyNode, VarToElim);
                 }
            else
                {
                updateContextFromGivenAssignmentUptoEasyNode(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, Path, EasyNode, VarToElim);
                }
             
             if(EasyNode->getPredicate()==NULL) // EasyNode is a terminal node   
                {
                  if(EasyNode->getVal() == 0) // 0-node
                    {
                    cout<<"\nSatisfiable assignment leading to 0-node in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";          
          
                    exit(1);
                    }// 0-node
                  else // 1-node
                    {
                    //EasyNode is True here
                    //Find \exists X. (A)
                    CompleteContext_With_Path.clear();
    
                    copySet(CompleteContext_With_Path, Path);

                    //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 

                    //cout<<"\nProject invoked\n";

                    set<string> VarsToElim_In_CompleteContext_With_Path_Conjunction_Support;

                    if(remove_already_eliminated_variables_in_path_based_strategy)
                        {
                          DAGNode*   CompleteContext_With_Path_Conjunction = getConjunctionFromSet(CompleteContext_With_Path);
                          set<string> CompleteContext_With_Path_Conjunction_Support;
                          getDAGNames(CompleteContext_With_Path_Conjunction, CompleteContext_With_Path_Conjunction_Support);

                          set_intersection(CompleteContext_With_Path_Conjunction_Support.begin(), CompleteContext_With_Path_Conjunction_Support.end(), VarSetToElim.begin(), VarSetToElim.end(),inserter(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.begin()));

                          //showSet(VarSetToElim, "VarSetToElim");
                          //showSet(CompleteContext_With_Path_Conjunction_Support, "CompleteContext_With_Path_Conjunction_Support");
                          //showSet(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, "VarsToElim_In_CompleteContext_With_Path_Conjunction_Support");

                          //cout<<"VarSetToElim.size() = "<<VarSetToElim.size()<<endl;
                          //cout<<"CompleteContext_With_Path_Conjunction_Support.size() = "<<CompleteContext_With_Path_Conjunction_Support.size()<<endl;
                          //cout<<"VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size() = "<<VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size()<<endl;

                          total_vars_to_elim_in_projects = total_vars_to_elim_in_projects + VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size();
                        }   
                    else
                        {
                        VarsToElim_In_CompleteContext_With_Path_Conjunction_Support = VarSetToElim;
                        }

                    DAGNode* Pi_i;
                    //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
                    Pi_i = callProject(CompleteContext_With_Path, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, WidthTable, project_depth);
                    //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";          
                    PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            
                    }// EasyNode is a 1-node   
                }// EasyNode is a terminal node   
             else // EasyNode is a non-terminal node 
                {
                  CompleteContext_With_Path.clear();
    
                  copySet(CompleteContext_With_Path, Path);
                 
                  DAGNode* Pi_i;
                   //showAPSet("Path", Path);
                  Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(EasyNode, CompleteContext_With_Path, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                   //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";          
                  PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
                } // EasyNode is a non-terminal node            
             
            }//if(PerformProjectsInPathBasedStrategyAtLMDDLevel)            
            else
            {                
            set<DAGNode*> Path;  
            //cout<<"\nCalling updateContextFromGivenAssignment\n";
            
            if(use_dag_instead_of_lmdd_in_smt_calls_in_path_based_strategy_in_lmdd_monniaux)
                {
                updateContextFromGivenAssignment(f, Temp_Model_Of_SatCondition, WidthTable, Path);
                 }
            else
                {
                updateContextFromGivenAssignment(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, Path);
                }
    
            CompleteContext_With_Path.clear();
    
            copySet(CompleteContext_With_Path, Path);
            
            //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 
         
            //cout<<"\nProject invoked\n";
            
            set<string> VarsToElim_In_CompleteContext_With_Path_Conjunction_Support;
            
            if(remove_already_eliminated_variables_in_path_based_strategy)
                {
                  DAGNode*   CompleteContext_With_Path_Conjunction = getConjunctionFromSet(CompleteContext_With_Path);
                  set<string> CompleteContext_With_Path_Conjunction_Support;
                  getDAGNames(CompleteContext_With_Path_Conjunction, CompleteContext_With_Path_Conjunction_Support);

                  set_intersection(CompleteContext_With_Path_Conjunction_Support.begin(), CompleteContext_With_Path_Conjunction_Support.end(), VarSetToElim.begin(), VarSetToElim.end(),inserter(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.begin()));

                  //showSet(VarSetToElim, "VarSetToElim");
                  //showSet(CompleteContext_With_Path_Conjunction_Support, "CompleteContext_With_Path_Conjunction_Support");
                  //showSet(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, "VarsToElim_In_CompleteContext_With_Path_Conjunction_Support");

                  //cout<<"VarSetToElim.size() = "<<VarSetToElim.size()<<endl;
                  //cout<<"CompleteContext_With_Path_Conjunction_Support.size() = "<<CompleteContext_With_Path_Conjunction_Support.size()<<endl;
                  //cout<<"VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size() = "<<VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size()<<endl;

                  total_vars_to_elim_in_projects = total_vars_to_elim_in_projects + VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size();
                }   
            else
                {
                VarsToElim_In_CompleteContext_With_Path_Conjunction_Support = VarSetToElim;
                }
            
            DAGNode* Pi_i;
            //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
            Pi_i = callProject(CompleteContext_With_Path, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, WidthTable, project_depth);
            //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";          
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            }//else of if(PerformProjectsInPathBasedStrategyAtLMDDLevel)
            
            
            gettimeofday (&finish_ms, NULL);
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        
            total_generalization_and_projection_time = total_generalization_and_projection_time + duration_ms;
        
        
            project_calls_in_path_based_strategy_in_lmdd_monniaux++;
            
            startup_project_calls_in_path_based_strategy_in_lmdd_monniaux++;
            }
        else
            {
            cout<<"\nORIGINAL f AND NOT Pi is unsat\n";
            return NULL;
            }//else of  if(!ResultIsFalse)
      }//for
    }//if(do_monniaux_at_start_up_in_use_path_based_strategy_in_lmdd_monniaux)
    
    
    // 1) Obtain a path in f, 2) Do Project if it is sat , and 3) return NULL if it is unsat
    
    NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
    
    if(use_dag_instead_of_lmdd_in_smt_calls_in_path_based_strategy_in_lmdd_monniaux)
            {
            SatisfiabilityCondition = createDAG("and", original_dag_in_path_based_strategy_in_lmdd_monniaux, NegPartialResult, LOGICAL_SYMBOL, 1); 
            }   
    else
            {
            SatisfiabilityCondition = createDAG("and", createDAGFromDD(f), NegPartialResult, LOGICAL_SYMBOL, 1); 
            }
     
    time(&start);
    gettimeofday (&start_ms, NULL);
      
    Temp_Model_Of_SatCondition.clear();
      
    ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);

    gettimeofday (&finish_ms, NULL);
    time(&end);
    duration = end-start;
    
    duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
    duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
    
    time_in_start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux = time_in_start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux + duration_ms;
        
    start_up_smt_calls_in_path_based_strategy_in_lmdd_monniaux++;


    if(!ResultIsFalse) // sat
        {
        //cout<<"\nPi = "<<getDAGStr(PartialResult_In_LMDDWithMonniauxTypeSimplifications)<<endl;
        
        cout<<"\nf AND NOT Pi is sat\n";      
        
        
        if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            map<string, string> Temp_Check_Model;
      
            DAGNode* TempCheckDAG = createDAG("and", createDAGFromDD(f), NegPartialResult, LOGICAL_SYMBOL, 1); 
            
            bool TempResultIsFalse = getModelGeneral_WithSupportForLMIs(TempCheckDAG, Temp_Check_Model, WidthTable);
            
            if(TempResultIsFalse)
                    {
                    cout<<"\nf' AND NOT Pi is unsat\n";
                    }
            else
                    {
                    cout<<"\nf' AND NOT Pi is sat\n";
                     }
            
            //cout<<"\nZIGMA' \n";
            //showModel(Temp_Check_Model);
            
            //cout<<"\nPath in the original LMDD\n";
            set<DAGNode*> PathInOriginalDD;  
            //cout<<"\nCalling updateContextFromGivenAssignment\n";
            updateContextFromGivenAssignment(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, PathInOriginalDD);
            //showAPSet("PathInOriginalDD", PathInOriginalDD);           
            
            
            bool TempExact = checkForExactnessInOneDirectionUsingSTP_With_LMI_Support(original_dag_in_path_based_strategy_in_lmdd_monniaux, createDAGFromDD(f), WidthTable);
                               
            }
        
        gettimeofday (&start_ms, NULL);
        
        if(PerformProjectsInPathBasedStrategyAtLMDDLevel)
        {
             set<DAGNode*> Path;
             
             DDNode* EasyNode;
             
             if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nCalling updateContextFromGivenAssignmentUptoEasyNode\n";
                }
             
             if(use_dag_instead_of_lmdd_in_smt_calls_in_path_based_strategy_in_lmdd_monniaux)
                {
                updateContextFromGivenAssignmentUptoEasyNode(f, Temp_Model_Of_SatCondition, WidthTable, Path, EasyNode, VarToElim);
                 }
            else
                {
                updateContextFromGivenAssignmentUptoEasyNode(current_root_LMDD_node_in_lmdd_with_monniaux, Temp_Model_Of_SatCondition, WidthTable, Path, EasyNode, VarToElim);
                }
             
             if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                showAPSet("Path", Path);
                cout<<"\nEasyNode\n";
                cout<<getDAGStr(createDAGFromDD(EasyNode))<<endl;
                }
             
             if(EasyNode->getPredicate()==NULL) // EasyNode is a terminal node   
                {
                  if(EasyNode->getVal() == 0) // 0-node
                    {
                    cout<<"\nSatisfiable assignment leading to 0-node in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";          
                    exit(1);
                    }// 0-node
                  else // 1-node
                    {
                     if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {
                        cout<<"\nEasyNode is 1-node\n";
                        }                      
                    //EasyNode is True here
                    //Find \exists X. (A)
                    CompleteContext_With_Path.clear();
    
                    copySet(CompleteContext_With_Path, Path);

                    //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 

                    //cout<<"\nProject invoked\n";

                    set<string> VarsToElim_In_CompleteContext_With_Path_Conjunction_Support;

                    if(remove_already_eliminated_variables_in_path_based_strategy)
                        {
                          DAGNode*   CompleteContext_With_Path_Conjunction = getConjunctionFromSet(CompleteContext_With_Path);
                          set<string> CompleteContext_With_Path_Conjunction_Support;
                          getDAGNames(CompleteContext_With_Path_Conjunction, CompleteContext_With_Path_Conjunction_Support);

                          set_intersection(CompleteContext_With_Path_Conjunction_Support.begin(), CompleteContext_With_Path_Conjunction_Support.end(), VarSetToElim.begin(), VarSetToElim.end(),inserter(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.begin()));

                          //showSet(VarSetToElim, "VarSetToElim");
                          //showSet(CompleteContext_With_Path_Conjunction_Support, "CompleteContext_With_Path_Conjunction_Support");
                          //showSet(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, "VarsToElim_In_CompleteContext_With_Path_Conjunction_Support");

                          //cout<<"VarSetToElim.size() = "<<VarSetToElim.size()<<endl;
                          //cout<<"CompleteContext_With_Path_Conjunction_Support.size() = "<<CompleteContext_With_Path_Conjunction_Support.size()<<endl;
                          //cout<<"VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size() = "<<VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size()<<endl;

                          total_vars_to_elim_in_projects = total_vars_to_elim_in_projects + VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size();
                        }   
                    else
                        {
                        VarsToElim_In_CompleteContext_With_Path_Conjunction_Support = VarSetToElim;
                        }

                    DAGNode* Pi_i;      
                     
                    
                    //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
                    Pi_i = callProject(CompleteContext_With_Path, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, WidthTable, project_depth);
                    //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";      
                    
                    if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {
                        cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                        }                     
                    
                    PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            
                    }// EasyNode is a 1-node   
                }// EasyNode is a terminal node   
             else // EasyNode is a non-terminal node 
                {
                  if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {
                        cout<<"\nEasyNode is non-terminal\n";
                        }                  
                 
                  CompleteContext_With_Path.clear();
    
                  copySet(CompleteContext_With_Path, Path);
                 
                  DAGNode* Pi_i;
                   //showAPSet("Path", Path);
                  Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(EasyNode, CompleteContext_With_Path, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                   //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";     
                  
                   if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {
                        cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                        }                   
                  
                  PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
                } // EasyNode is a non-terminal node   
        } //if(PerformProjectsInPathBasedStrategyAtLMDDLevel)              
        else
        {
        set<DAGNode*> Path;  
        //cout<<"\nCalling updateContextFromGivenAssignment\n";
        updateContextFromGivenAssignment(f, Temp_Model_Of_SatCondition, WidthTable, Path);

        CompleteContext_With_Path.clear();

        copySet(CompleteContext_With_Path, Path);
        
        
        
        set<string> VarsToElim_In_CompleteContext_With_Path_Conjunction_Support;

        if(remove_already_eliminated_variables_in_path_based_strategy)
        {
          DAGNode*   CompleteContext_With_Path_Conjunction = getConjunctionFromSet(CompleteContext_With_Path);
          set<string> CompleteContext_With_Path_Conjunction_Support;
          getDAGNames(CompleteContext_With_Path_Conjunction, CompleteContext_With_Path_Conjunction_Support);

          set_intersection(CompleteContext_With_Path_Conjunction_Support.begin(), CompleteContext_With_Path_Conjunction_Support.end(), VarSetToElim.begin(), VarSetToElim.end(),inserter(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.begin()));

          //showSet(VarSetToElim, "VarSetToElim");
          //showSet(CompleteContext_With_Path_Conjunction_Support, "CompleteContext_With_Path_Conjunction_Support");
          //showSet(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, "VarsToElim_In_CompleteContext_With_Path_Conjunction_Support");
        
           //cout<<"VarSetToElim.size() = "<<VarSetToElim.size()<<endl;
           //cout<<"CompleteContext_With_Path_Conjunction_Support.size() = "<<CompleteContext_With_Path_Conjunction_Support.size()<<endl;
           //cout<<"VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size() = "<<VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size()<<endl;
      
           total_vars_to_elim_in_projects = total_vars_to_elim_in_projects + VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size();
        }   
        else
        {
            VarsToElim_In_CompleteContext_With_Path_Conjunction_Support = VarSetToElim;
        }

        //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 

        //cout<<"\nProject invoked\n";
        DAGNode* Pi_i;
        //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
        Pi_i = callProject(CompleteContext_With_Path, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, WidthTable, project_depth);
        //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";          
        PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
        }//else of  if(PerformProjectsInPathBasedStrategyAtLMDDLevel)
        
        
        gettimeofday (&finish_ms, NULL);
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        
        total_generalization_and_projection_time = total_generalization_and_projection_time + duration_ms;
        
        project_calls_in_path_based_strategy_in_lmdd_monniaux++;
        
        startup_project_calls_in_path_based_strategy_in_lmdd_monniaux++;
        
        }
    else
        {
        cout<<"\nf AND NOT Pi is unsat\n";
        return NULL;
        }//else of  if(!ResultIsFalse)
        

  DDNode* h;
  DDNode* result_of_h;
  vector< vector< DDNode*> > CurrentPath;
  // Five entries inside each entry : 
  // 1) DDNode* along the path 
  // 2) Child of this node on the path
  // 3) Child of this node off the path
  // 4) Quantified Child of this node on the path
  // 5) Quantified Child of this node off the path
  int LocationCounter;
  set<DAGNode*> E;
  set<DAGNode*> D;
  set<DAGNode*> I;
  set<DAGNode*> Neg_x;  
  
  set<DDNode*> NonHashableNodesInCurrentPath;
  // Stores the set of LMDD nodes in the CurrentPath, results of 
  // which should not be hashed
  
  if(apply_lmebased_simplification_on_sat_path)
        {
        obtainPathFromGivenAssignmentUpToPoint(f, Temp_Model_Of_SatCondition, WidthTable,  CurrentPath, VarToElim, false);
        } 
    else
       {
       obtainPathFromGivenAssignment(f, Temp_Model_Of_SatCondition, WidthTable,  CurrentPath);
       }
  
  
  if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
    {
    showDDPath(CurrentPath);
    }
  
  while(CurrentPath.size() >= 1)
    {
      
      if(CurrentPath.size() > max_path_depth_in_path_based_strategy_in_lmdd_monniaux)
        {
          max_path_depth_in_path_based_strategy_in_lmdd_monniaux = CurrentPath.size();
        }
      
     nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
     
      
      LocationCounter = CurrentPath.size() - 1;
      h = CurrentPath[LocationCounter][0];
      
      E.clear(); D.clear(); I.clear(); Neg_x.clear();
      //showDDPath(CurrentPath);
      obtainContextFromPath(CurrentPath, h, E, D, I, Neg_x, VarToElim);
      
       if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            cout<<"\nTaking the new node from the CurrentPath\n";
            showProblemInstance(h, E, D, I, Neg_x);
            cout<<"\nLocationCounter = "<<LocationCounter<<endl;
            }
      
      
      DDNode* Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, h, E,  D, I, PtrTableForQElimModified);

      if(Existing != NULL)
            {
              hit_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
              
              if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nEntry exists for the simplified dd in hash table\n";
                }
              result_of_h = Existing;
             // cout<<"\nresult_of_h = "<<getDAGStr(createDAGFromDD(result_of_h))<<endl;

              //cout<<"\nInserting the result at appropriate place in CurrentPath\n";
              // Inserting the result at appropriate place in CurrentPath
              if(LocationCounter == 0) // final result obtained
                {
                //cout<<"\nResult returned\n";
                return result_of_h;  
                }
              else// Inserting the result at appropriate place in CurrentPath
                {
                  if(CurrentPath[LocationCounter-1][3] == NULL)
                  {
                      //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<3<<endl;
                      CurrentPath[LocationCounter-1][3] = result_of_h;
                  }
                  else if(CurrentPath[LocationCounter-1][4] == NULL)
                  {
                      //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<4<<endl;
                       CurrentPath[LocationCounter-1][4] = result_of_h;
                  }
                  else
                  {
                      cout<<"\nBoth the children are already quantified in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
                      exit(1);
                  }
                }//else of if(LocationCounter == 0) 

              //Deleting the last entry from CurrentPath
              //cout<<"\nLast entry deleted from CurrentPath\n";
              CurrentPath.erase(CurrentPath.begin()+LocationCounter);
         }// entry exists in the hash table      
      // Entry does not exist in the hash table      
    else if(h->getPredicate()==NULL) // h is a terminal node
    {
        if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nTerminal node reached\n";
                }
      //cout<<"\nTerminal node reached\n";

      int value = h->getVal();

      if(value == 0) // 0-node
        {
           cout<<"\nSatisfiable assignment leading to 0-node in function \n";          
          
           exit(1);
       }// 0-node
      else // 1-node
        {        
          one_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
          if(E.empty() && D.empty() && I.empty())
            {
	      //cout<<"\nContext empty. hence result=1\n";
               if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nContext empty. hence result=1\n";
                }
              result_of_h = createDD(1);
            }
          else
            {
	      //cout<<"\nProject_ReturningLMDD invoked\n";
               if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nProject_ReturningLMDD invoked\n";
                }
              result_of_h = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            }          
          
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            cout<<"\nresult_of_h = "<<getDAGStr(createDAGFromDD(result_of_h))<<endl;
            }
          
          // Inserting the result at appropriate place in CurrentPath
          if(LocationCounter == 0) // final result obtained
            {
            //cout<<"\nResult returned\n";
            return result_of_h;  
            }
          else// Inserting the result at appropriate place in CurrentPath
            {
              if(CurrentPath[LocationCounter-1][3] == NULL)
              {
                  //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<3<<endl;
                  CurrentPath[LocationCounter-1][3] = result_of_h;
              }
              else if(CurrentPath[LocationCounter-1][4] == NULL)
              {
                  //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<4<<endl;
                   CurrentPath[LocationCounter-1][4] = result_of_h;
              }
              else
              {
                  cout<<"\nBoth the children are already quantified in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
                  exit(1);
              }
            }//else of if(LocationCounter == 0) 

          //Deleting the last entry from CurrentPath
          //cout<<"\nLast entry deleted from CurrentPath\n";
          CurrentPath.erase(CurrentPath.begin()+LocationCounter);   
          
          //Insert into hash table
          insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, h, E, D, I, result_of_h, PtrTableForQElimModified);
        }// 1-node
    }//else if h is a terminal node
    else if(DDFreeOfVar_WithDP(h, VarToElim)) //h is already free of VarToElim
        {
            free_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
        
             if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nDDFreeOfVar\n";
                cout<<"\nProject_ReturningLMDD invoked\n";
                }
        
              //cout<<"\n"<<getDAGStr(createDAGFromDD(f))<<" is free of "<<VarToElim<<"\n";
              //cout<<"\nProject_ReturningLMDD invoked\n";
               result_of_h = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);

               result_of_h = apply("and", h, result_of_h);  
               
               if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nresult_of_h = "<<getDAGStr(createDAGFromDD(result_of_h))<<endl;
                }
               
           // Inserting the result at appropriate place in CurrentPath
              if(LocationCounter == 0) // final result obtained
                {
                //cout<<"\nResult returned\n";
                return result_of_h;  
                }
              else// Inserting the result at appropriate place in CurrentPath
                {
                  if(CurrentPath[LocationCounter-1][3] == NULL)
                  {
                      //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<3<<endl;
                      CurrentPath[LocationCounter-1][3] = result_of_h;
                  }
                  else if(CurrentPath[LocationCounter-1][4] == NULL)
                  {
                      //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<4<<endl;
                       CurrentPath[LocationCounter-1][4] = result_of_h;
                  }
                  else
                  {
                      cout<<"\nBoth the children are already quantified in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
                      exit(1);
                  }
                }//else of if(LocationCounter == 0) 

              //Deleting the last entry from CurrentPath
              //cout<<"\nLast entry deleted from CurrentPath\n";
              CurrentPath.erase(CurrentPath.begin()+LocationCounter);   

              //Insert into hash table
              insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, h, E, D, I, result_of_h, PtrTableForQElimModified);

            }// if(DDFreeOfVar_WithDP(f, VarToElim))
      
    else if(apply_lmebased_simplification_on_sat_path && DAGResolveApplicable(E, D, WidthTable))
        {
        
         dagresolve_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
            
             if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nQE_LMDD invoked as DAGResolve is applicable\n";
                }
        
            qe_lmdd_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
             //cout<<"\ncalling Qualmode_WithLMISupport\n";
             
             gettimeofday (&start_ms, NULL);
             
             result_of_h = QElim_WithLMISupport(VarToElim, h, E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
             
             gettimeofday (&finish_ms, NULL);
              
              duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
              duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
             
              time_spent_in_qe_lmdd = time_spent_in_qe_lmdd + duration_ms;   
              
              // Inserting the result at appropriate place in CurrentPath
              if(LocationCounter == 0) // final result obtained
                {
                //cout<<"\nResult returned\n";
                return result_of_h;  
                }
              else// Inserting the result at appropriate place in CurrentPath
                {
                  if(CurrentPath[LocationCounter-1][3] == NULL)
                  {
                      //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<3<<endl;
                      CurrentPath[LocationCounter-1][3] = result_of_h;
                  }
                  else if(CurrentPath[LocationCounter-1][4] == NULL)
                  {
                      //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<4<<endl;
                       CurrentPath[LocationCounter-1][4] = result_of_h;
                  }
                  else
                  {
                      cout<<"\nBoth the children are already quantified in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
                      exit(1);
                  }
                }//else of if(LocationCounter == 0) 

              //Deleting the last entry from CurrentPath
              //cout<<"\nLast entry deleted from CurrentPath\n";
              CurrentPath.erase(CurrentPath.begin()+LocationCounter);   

        }
    else //if h is a non-terminal node
    {
          nonterminal_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
          
          set<DAGNode*> E_Original;
          set<DAGNode*> D_Original;
          set<DAGNode*> I_Original;
          set<DAGNode*> Neg_x_Original;
          
          copySet(E_Original, E);
          copySet(D_Original, D);
          copySet(I_Original, I);
          copySet(Neg_x_Original, Neg_x);
          
          DAGNode* C = h->getPredicate();
          DAGNode* C_for_ChildOffPath;
          //cout<<"\nPredicate = "<<getDAGStr(C)<<endl;
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nPredicate = "<<getDAGStr(C)<<endl;
                }
          
          DDNode* QEChildOnPath = CurrentPath[LocationCounter][3];
          
          //cout<<"\nQEChildOnPath = "<<getDAGStr(createDAGFromDD(QEChildOnPath))<<endl;
          //cout<<"\nQEChildOnPath is obtained\n";
          
          if(QEChildOnPath == NULL)
          {
              cout<<"\nQEChildOnPath == NULL  in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
              exit(1);
          }
          
           //cout<<"\nObtaining QEChildOffPath\n";
          
          DDNode* QEChildOffPath = CurrentPath[LocationCounter][4];
          
          DDNode* ChildOffPath = CurrentPath[LocationCounter][2];
          
          if(QEChildOffPath != NULL)
          {
              computed_offpathchild_in_path_based_strategy_in_lmdd_monniaux++;
              
               if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
               {
                cout<<"\nQEChildOffPath already computed\n";
               }
          }          
          else //QEChildOffPath == NULL. We need to compute it
          {
               if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
               {
                cout<<"\nQEChildOffPath not already computed\n";
               }
              //cout<<"\nNot already computed\n";
              //not already there
              
              // Let us compute it. 
              // Let us get the components first
              
              ChildOffPath = CurrentPath[LocationCounter][2];
              //cout<<"\nChildOffPath = "<<getDAGStr(createDAGFromDD(ChildOffPath))<<endl;
              
              //cout<<"\nGetting direction\n";              

               // We need to update the context
              string direction;
              if(ChildOffPath == h->getLeft())
                  {
                  direction = "left";
                  }
              else if(ChildOffPath == h->getRight())
                  {
                  direction = "right";
                  }
              else
                  {
                 cout<<"\nUnknown child in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
                 exit(1);
                 }
              
              //cout<<"\ndirection = "<<direction<<endl;
              
              if(direction=="left")//we need to negate C
                {
                 C_for_ChildOffPath = negateConstraint_With_LMISupport(C); 
                }
              else
                {
                  C_for_ChildOffPath = C;
                }
              
              //cout<<"\nChanged Predicate = "<<getDAGStr(C)<<endl;
              
              if(freeOfVarModified(C_for_ChildOffPath, VarToElim))
              {
                 Neg_x.insert(C_for_ChildOffPath) ;
              }
              else if(C_for_ChildOffPath->Get_node_name()=="=" || C_for_ChildOffPath->Get_node_name()=="equivalent" )
              {
                  E.insert(C_for_ChildOffPath);
              }
              else if(C_for_ChildOffPath->Get_node_name()=="is_not_equal" || C_for_ChildOffPath->Get_node_name()=="not_equal_to" )
              {
                  D.insert(C_for_ChildOffPath);
              }
              else
              {
                  I.insert(C_for_ChildOffPath);
              }
           
            //cout<<"\nContext updated\n";
            //showProblemInstance(ChildOffPath, E, D, I, Neg_x);
              
            //The QE instance is \exists VarToElim.(ChildOffPath \wedge E \wedge D \wedge I) under Neg_x
            Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, ChildOffPath, E,  D, I, PtrTableForQElimModified);

            if(Existing != NULL)
            {
              hashed_offpathchild_in_path_based_strategy_in_lmdd_monniaux++;  
                
                
              if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
               {
                cout<<"\nQEChildOffPath exists in hash table\n";
               }               
                
              //hit_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
              //cout<<"\nEntry exists for the simplified dd in hash table\n";
              QEChildOffPath = Existing;
              //cout<<"\nQEChildOffPath = "<<getDAGStr(createDAGFromDD(QEChildOffPath))<<endl;
            }// entry exists in the hash table      
             // Entry does not exist in the hash table      
           else if(ChildOffPath->getPredicate()==NULL) // ChildOffPath is a terminal node
            {
            //cout<<"\nTerminal node reached\n";
             if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
               {
                cout<<"\nChildOffPath is terminal\n";
               }   
               

            int value = ChildOffPath->getVal();

            if(value == 0) // 0-node
             {
                
             zero_offpathchild_in_path_based_strategy_in_lmdd_monniaux++;   
                
             if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
               {
                cout<<"\nChildOffPath is 0-node\n";
               }   
                
             zero_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
             //cout<<"\nZero node\n";
             QEChildOffPath = createDD(0);
             //cout<<"\nQEChildOffPath = "<<getDAGStr(createDAGFromDD(QEChildOffPath))<<endl;
             }// 0-node
           else // 1-node
             {    
                
               one_offpathchild_in_path_based_strategy_in_lmdd_monniaux++;    
                
               if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
               {
                cout<<"\nChildOffPath is 1-node\n";
               } 
                
                
             //one_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;  
             if(E.empty() && D.empty() && I.empty())
                {
                 if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {
                    cout<<"\nContext empty. Result is 1\n";
                    }                
                 
                //cout<<"\nContext empty. hence result=1\n";
                QEChildOffPath = createDD(1);
                //cout<<"\nQEChildOffPath = "<<getDAGStr(createDAGFromDD(QEChildOffPath))<<endl;
                }
          else
              {
                  
               if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nProject_ReturningLMDD invoked\n";
                    }      
                  
	      //cout<<"\nProject_ReturningLMDD invoked\n";
              QEChildOffPath = callProjectReturningLMDD(E, D, I, VarToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
              //cout<<"\nQEChildOffPath = "<<getDAGStr(createDAGFromDD(QEChildOffPath))<<endl;
              }      
            }// 1-node
            
            //Insert into hash table
            insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, ChildOffPath, E, D, I, QEChildOffPath, PtrTableForQElimModified);
            
          }//else if ChildOffPath is a terminal node
            //ChildOffPath is a non-terminal node
            // Let us decide the strategy for QE
           else if(selectStrategy(ChildOffPath, E, D, I, WidthTable, VarToElim,  VarSetToElim) == "lmdd")
           {
             int_select_strategy_calls++;  
               
              if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nQE_LMDD selected for QE\n";
                    }     
               
               
             qe_lmdd_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
             //cout<<"\ncalling Qualmode_WithLMISupport\n";
             
             gettimeofday (&start_ms, NULL);
             
             QEChildOffPath = QElim_WithLMISupport(VarToElim, ChildOffPath, E, D, I, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
             
             gettimeofday (&finish_ms, NULL);
              
              duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
              duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
             
              time_spent_in_qe_lmdd = time_spent_in_qe_lmdd + duration_ms;
              
              if(select_strategy_basis=="free")
               {
                int_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar = int_time_spent_in_qe_lmdd_for_select_strategy_ddfreeofvar + duration_ms;
                
                int_select_strategy_ddfreeofvar++;
               }
              else if(select_strategy_basis=="dagresolve")
              {
                int_time_spent_in_qe_lmdd_for_select_strategy_dagresolve = int_time_spent_in_qe_lmdd_for_select_strategy_dagresolve + duration_ms;   
                
                int_select_strategy_dagresolve++;
              }
              else if(select_strategy_basis=="lowlevel")
              {
                int_time_spent_in_qe_lmdd_for_select_strategy_lowlevel = int_time_spent_in_qe_lmdd_for_select_strategy_lowlevel + duration_ms;   
                
                int_select_strategy_lowlevel++;
              }
              else  if(select_strategy_basis=="size")
              {
                int_time_spent_in_qe_lmdd_for_select_strategy_size = int_time_spent_in_qe_lmdd_for_select_strategy_size + duration_ms;   
                
                int_select_strategy_size++;
              }
              else  if(select_strategy_basis=="default")
              {
                int_time_spent_in_qe_lmdd_for_select_strategy_default = int_time_spent_in_qe_lmdd_for_select_strategy_default + duration_ms;   
                
                int_select_strategy_for_lmdd_default++;
              }
              
             
             //cout<<"\nQEChildOffPath = "<<getDAGStr(createDAGFromDD(QEChildOffPath))<<endl;
           }
           else//strategy == "monniaux"
           {
              int_select_strategy_calls++;   
                
              int_select_strategy_for_monniax++;  
                
              if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nMonniaux selected for QE\n";
                    }  
                
                
              smt_calls_on_path_in_path_based_strategy_in_lmdd_monniaux++;
              //cout<<"\ncalling SMT solver\n";  
              set<DAGNode*> CompleteContext;
              set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(CompleteContext, CompleteContext.begin()));
              set_union(CompleteContext.begin(), CompleteContext.end(), I.begin(), I.end(),inserter(CompleteContext, CompleteContext.begin()));
              set_union(CompleteContext.begin(), CompleteContext.end(), Neg_x.begin(), Neg_x.end(),inserter(CompleteContext, CompleteContext.begin()));
  
              DAGNode* PathCondition = createDAG("and", createDAGFromDD(ChildOffPath), getConjunctionFromSet(CompleteContext), LOGICAL_SYMBOL, 1);
              //cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
              
              DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
              //cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
   
              DAGNode* SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
              //cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;  
              
              gettimeofday (&start_ms, NULL);
              
              Temp_Model_Of_SatCondition.clear();
      
              ResultIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, Temp_Model_Of_SatCondition, WidthTable);
              
              gettimeofday (&finish_ms, NULL);
              
              duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
              duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
             
              time_in_smt_calls_on_path_in_path_based_strategy_in_lmdd_monniaux = time_in_smt_calls_on_path_in_path_based_strategy_in_lmdd_monniaux + duration_ms;
              
              if(ResultIsFalse)
              {
                  if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nUnsat from SMT-solver\n";
                    }
                  
                  
                  unsat_nodes_on_path_in_path_based_strategy_in_lmdd_monniaux++;
                  //cout<<"\nUnsat. Zero node\n";
                  QEChildOffPath = createDD(0);
                  //cout<<"\nQEChildOffPath = "<<getDAGStr(createDAGFromDD(QEChildOffPath))<<endl;
                  
                  updateNonHashableNodesInCurrentPath(NonHashableNodesInCurrentPath, CurrentPath);
                  //Enter all nodes in CurrentPath into NonHashableNodesInCurrentPath
                  
                   if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {
                    showDDSet("NonHashableNodesInCurrentPath", NonHashableNodesInCurrentPath);
                     }
              }
              else
              {
                  //cout<<"\nSat. Need to go down\n";
                  
                   if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nSAT from SMT-solver. Need to go down\n";
                    }
                  
                  
                  QEChildOffPath = NULL;//We need to go down
              }
           }//strategy == "monniaux"
          } //else of if(QEChildOffPath != NULL)          
          
      if(QEChildOffPath == NULL)    
      {
          
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nObtaining path and going down\n";
                    }
          
          //obtain path starting at ChildOffPath
          vector< vector<DDNode*> > PartialPath;          
          
           if(apply_lmebased_simplification_on_sat_path)
            {
            obtainPathFromGivenAssignmentUpToPoint(ChildOffPath, Temp_Model_Of_SatCondition, WidthTable,  PartialPath, VarToElim, false);
            } 
          else
            {
            obtainPathFromGivenAssignment(ChildOffPath, Temp_Model_Of_SatCondition, WidthTable,  PartialPath);
            }
          
          
          
          extendPath(CurrentPath, PartialPath);
          
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            showDDPath(CurrentPath);
            }
          
          
          gettimeofday (&start_ms, NULL);
          
          if(PerformProjectsInPathBasedStrategyAtLMDDLevel)
           {
              
             if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                cout<<"\nCalling updateContextFromGivenAssignmentUptoEasyNode\n";
                }  
                        
             set<DAGNode*> Path;
             
             DDNode* EasyNode;
             
              //cout<<"\nCalling updateContextFromGivenAssignment\n";
             
             updateContextFromGivenAssignmentUptoEasyNode(ChildOffPath, Temp_Model_Of_SatCondition, WidthTable, Path, EasyNode, VarToElim);
              
              if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                {
                showAPSet("Path", Path);
                cout<<"\nEasyNode\n";
                cout<<getDAGStr(createDAGFromDD(EasyNode))<<endl;
                }
             
             if(EasyNode->getPredicate()==NULL) // EasyNode is a terminal node   
                {
                  if(EasyNode->getVal() == 0) // 0-node
                    {
                    cout<<"\nSatisfiable assignment leading to 0-node in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";          
          
                    exit(1);
                    }// 0-node
                  else // 1-node
                    {
                    //EasyNode is True here
                    //Find \exists X. (A)
                   if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {                        
                        cout<<"\nEasyNode is 1-node\n";
                        }                                           
                      
                    CompleteContext_With_Path.clear();
                    set<DAGNode*> CompleteContext;
                    set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(CompleteContext, CompleteContext.begin()));
                    set_union(CompleteContext.begin(), CompleteContext.end(), I.begin(), I.end(),inserter(CompleteContext, CompleteContext.begin()));
                    set_union(CompleteContext.begin(), CompleteContext.end(), Neg_x.begin(), Neg_x.end(),inserter(CompleteContext, CompleteContext.begin()));
                    set_union(CompleteContext.begin(), CompleteContext.end(), Path.begin(), Path.end(),inserter(CompleteContext_With_Path, CompleteContext_With_Path.begin()));
                   

                    //cout<<"\nexists x. (f wedge E wedge D wedge I) is needed under Local_Neg_x_Context since Local_Neg_x_Context subseteq Neg_x_Context\n"; 

                    //cout<<"\nProject invoked\n";

                    set<string> VarsToElim_In_CompleteContext_With_Path_Conjunction_Support;

                    if(remove_already_eliminated_variables_in_path_based_strategy)
                        {
                          DAGNode*   CompleteContext_With_Path_Conjunction = getConjunctionFromSet(CompleteContext_With_Path);
                          set<string> CompleteContext_With_Path_Conjunction_Support;
                          getDAGNames(CompleteContext_With_Path_Conjunction, CompleteContext_With_Path_Conjunction_Support);

                          set_intersection(CompleteContext_With_Path_Conjunction_Support.begin(), CompleteContext_With_Path_Conjunction_Support.end(), VarSetToElim.begin(), VarSetToElim.end(),inserter(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.begin()));

                          //showSet(VarSetToElim, "VarSetToElim");
                          //showSet(CompleteContext_With_Path_Conjunction_Support, "CompleteContext_With_Path_Conjunction_Support");
                          //showSet(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, "VarsToElim_In_CompleteContext_With_Path_Conjunction_Support");

                          //cout<<"VarSetToElim.size() = "<<VarSetToElim.size()<<endl;
                          //cout<<"CompleteContext_With_Path_Conjunction_Support.size() = "<<CompleteContext_With_Path_Conjunction_Support.size()<<endl;
                          //cout<<"VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size() = "<<VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size()<<endl;

                          total_vars_to_elim_in_projects = total_vars_to_elim_in_projects + VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size();
                        }   
                    else
                        {
                        VarsToElim_In_CompleteContext_With_Path_Conjunction_Support = VarSetToElim;
                        }

                    DAGNode* Pi_i;
                    //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
                    Pi_i = callProject(CompleteContext_With_Path, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, WidthTable, project_depth);
                    //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                    
                    if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {                        
                        cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";  
                        }                     
                    
                    PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            
                    }// EasyNode is a 1-node   
                }// EasyNode is a terminal node   
             else // EasyNode is a non-terminal node 
                {
                 
                 if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {                        
                        cout<<"\nEasyNode is non-terminal\n";  
                        }
                                    
                  CompleteContext_With_Path.clear();
                  set<DAGNode*> CompleteContext;
                  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(CompleteContext, CompleteContext.begin()));
                  set_union(CompleteContext.begin(), CompleteContext.end(), I.begin(), I.end(),inserter(CompleteContext, CompleteContext.begin()));
                  set_union(CompleteContext.begin(), CompleteContext.end(), Neg_x.begin(), Neg_x.end(),inserter(CompleteContext, CompleteContext.begin()));
                  set_union(CompleteContext.begin(), CompleteContext.end(), Path.begin(), Path.end(),inserter(CompleteContext_With_Path, CompleteContext_With_Path.begin()));
             
                 
                  DAGNode* Pi_i;
                   //showAPSet("Path", Path);
                  Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(EasyNode, CompleteContext_With_Path, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                   //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";   
                  
                  if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                        {                        
                        cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";  
                        }
                  
                  PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
                } // EasyNode is a non-terminal node   
           }            
         else
          {
             set<DAGNode*> Path;  
              //cout<<"\nCalling updateContextFromGivenAssignment\n";
             updateContextFromGivenAssignment(ChildOffPath, Temp_Model_Of_SatCondition, WidthTable, Path);

             CompleteContext_With_Path.clear();

             set<DAGNode*> CompleteContext;
             set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(CompleteContext, CompleteContext.begin()));
             set_union(CompleteContext.begin(), CompleteContext.end(), I.begin(), I.end(),inserter(CompleteContext, CompleteContext.begin()));
             set_union(CompleteContext.begin(), CompleteContext.end(), Neg_x.begin(), Neg_x.end(),inserter(CompleteContext, CompleteContext.begin()));
             set_union(CompleteContext.begin(), CompleteContext.end(), Path.begin(), Path.end(),inserter(CompleteContext_With_Path, CompleteContext_With_Path.begin()));



            set<string> VarsToElim_In_CompleteContext_With_Path_Conjunction_Support;

            if(remove_already_eliminated_variables_in_path_based_strategy)
            {
              DAGNode*   CompleteContext_With_Path_Conjunction = getConjunctionFromSet(CompleteContext_With_Path);
              set<string> CompleteContext_With_Path_Conjunction_Support;
              getDAGNames(CompleteContext_With_Path_Conjunction, CompleteContext_With_Path_Conjunction_Support);

              set_intersection(CompleteContext_With_Path_Conjunction_Support.begin(), CompleteContext_With_Path_Conjunction_Support.end(), VarSetToElim.begin(), VarSetToElim.end(),inserter(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.begin()));

              //showSet(VarSetToElim, "VarSetToElim");
              //showSet(CompleteContext_With_Path_Conjunction_Support, "CompleteContext_With_Path_Conjunction_Support");
              //showSet(VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, "VarsToElim_In_CompleteContext_With_Path_Conjunction_Support");

             //cout<<"VarSetToElim.size() = "<<VarSetToElim.size()<<endl;
             //cout<<"CompleteContext_With_Path_Conjunction_Support.size() = "<<CompleteContext_With_Path_Conjunction_Support.size()<<endl;
             //cout<<"VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size() = "<<VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size()<<endl;

            total_vars_to_elim_in_projects = total_vars_to_elim_in_projects + VarsToElim_In_CompleteContext_With_Path_Conjunction_Support.size();
            }   
            else
            {
                VarsToElim_In_CompleteContext_With_Path_Conjunction_Support = VarSetToElim;
            }

            //cout<<"\nProject invoked\n";
            DAGNode* Pi_i;
            //showAPSet("CompleteContext_With_Path", CompleteContext_With_Path);
            Pi_i = callProject(CompleteContext_With_Path, VarsToElim_In_CompleteContext_With_Path_Conjunction_Support, WidthTable, project_depth);
            //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";          
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
         }//else of if(PerformProjectsInPathBasedStrategyAtLMDDLevel)
        
        gettimeofday (&finish_ms, NULL);
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        
        total_generalization_and_projection_time = total_generalization_and_projection_time + duration_ms;
        
        
        
        project_calls_in_path_based_strategy_in_lmdd_monniaux++;
        
        project_calls_on_path_in_path_based_strategy_in_lmdd_monniaux++;
          
      }
      else //QEChildOffPath != NULL
      {
       if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {                   
                    cout<<"\nCombining the results from the two children\n";
                    cout<<"\nC used for combining = "<<getDAGStr(C)<<endl;
                    }             
              
      // Insert  QEChildOffPath  at appropriate location
      CurrentPath[LocationCounter][4] = QEChildOffPath;   
      //cout<<"\nQEChildOffPath inserted at"<<LocationCounter<<","<<4<<endl;
      
      //Finding result_h
      if(freeOfVarModified(C, VarToElim))
      {
          DDNode* C_dd = createDD(C, WidthTable);
          
          // Before combining we need to understand left and right
          
          DDNode *QELeftChild;
          DDNode *QERightChild;
          
          if(ChildOffPath == h->getLeft())
            {
            QELeftChild =  QEChildOffPath;
            QERightChild = QEChildOnPath;
            }
          else if(ChildOffPath == h->getRight())
            {
            QERightChild =  QEChildOffPath;
            QELeftChild = QEChildOnPath;      
             }
          else
            { 
              cout<<"\nUnknown child in DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
              exit(1);
            }
          
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            cout<<"\nresult_of_h is ITE of QERightChild, QELeftChild\n";
            }
          
          result_of_h = createITE(C_dd, QERightChild, QELeftChild);
      }
      else
      {
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
                cout<<"\nresult_of_h is disjunction of QEChildOnPath, QEChildOffPath\n";
            }
          
          result_of_h = apply("or", QEChildOnPath, QEChildOffPath);
      }
      
      //cout<<"\nresult_of_h computed"<<endl;
      //cout<<"\nresult_of_h = "<<getDAGStr(createDAGFromDD(result_of_h))<<endl;
      
       // Inserting the result at appropriate place in CurrentPath
      if(LocationCounter == 0) // final result obtained
        {
        //cout<<"\nResult returned"<<endl;  
        return result_of_h;  
        }
      else// Inserting the result at appropriate place in CurrentPath
        {
          if(CurrentPath[LocationCounter-1][3] == NULL)
          {
              //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<3<<endl;
              CurrentPath[LocationCounter-1][3] = result_of_h;
          }
          else if(CurrentPath[LocationCounter-1][4] == NULL)
          {
              //cout<<"\nResult inserted at"<<LocationCounter-1<<","<<3<<endl;
               CurrentPath[LocationCounter-1][4] = result_of_h;
          }
          else
          {
              cout<<"\nBoth the children are already quantified in function DDManager::pathBasedQEStrategy_With_LMDD_Monniaux_Simplifications\n";
              exit(1);
          }
        }//else of if(LocationCounter == 0) 

      //cout<<"\nDeleting the last entry from CurrentPath\n";
      //Deleting the last entry from CurrentPath
      CurrentPath.erase(CurrentPath.begin()+LocationCounter); 
      
      
      //Check if h is a hashable node
      if(NonHashableNodesInCurrentPath.empty())
        {
         //hash h
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
              cout<<"\nHash "<<getDAGStr(h->getPredicate())<<endl;
            }
         insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, h, E_Original, D_Original, I_Original, result_of_h, PtrTableForQElimModified);  
        }
      else if(NonHashableNodesInCurrentPath.find(h) == NonHashableNodesInCurrentPath.end()) //h is hashable
        {
        //hash h 
           if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            cout<<"\nHash "<<getDAGStr(h->getPredicate())<<endl;
            }
         insertEntryIntoTableForQElimModified_WithLMISupport(VarToElim, h, E_Original, D_Original, I_Original, result_of_h, PtrTableForQElimModified); 
        }
      else
        {//remove h from NonHashableNodesInCurrentPath
           if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
            {
            cout<<"\nDon't hash "<<getDAGStr(h->getPredicate())<<endl;
           }
           
          removeEntryFromNonHashableNodesInCurrentPath(NonHashableNodesInCurrentPath, h);
          
          if(debug_check_in_path_based_strategy_in_lmdd_monniaux)
                    {
                    showDDSet("NonHashableNodesInCurrentPath", NonHashableNodesInCurrentPath);
                     }
          
        }//h is not hashable   
      
      } //QEChildOffPath != NULL
    }//if h is a non-terminal node      
 }//while(CurrentPath.size() >= 1)
  
       
}//function ends here


// select the QE strategy to be chosen
string DDManager::selectStrategy(DDNode* f, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, map<string, int> &WidthTable, string VarToElim, set<string> &VarsToElim)
{
    bool always_monniaux=false;
    bool always_lmdd=false;
    bool check_if_lmes_exist=false;
    bool multiple_strategy=true;
    
    if(always_monniaux)
        {
        return "monniaux";
        }
    else if(always_lmdd)
        {
         return "lmdd";
        } 
    else if(check_if_lmes_exist)
        {
        if(DDFreeOfVar_WithDP(f, VarToElim))
            {
            return "lmdd"; 
            }
        else if(DAGResolveApplicable(E, D, WidthTable))
            {
            return "lmdd"; 
            }
         else
            {
            
            set<string> VarsPresent;
            
            SetOfDDNodes.clear(); // clear this first
            getDDNames(f, VarsPresent);
            
            set<string> VarsToElimPresent;
            set_intersection(VarsPresent.begin(), VarsPresent.end(), VarsToElim.begin(), VarsToElim.end(),inserter(VarsToElimPresent, VarsToElimPresent.begin()));

            
            
            int NoVars = VarsPresent.size();
            int NoVarsToElim = VarsToElimPresent.size();
            
            float ratio = (float)NoVarsToElim/(float)NoVars;
            int NoVarsRemaining = NoVars - NoVarsToElim;
            
            cout<<"\nNoVarsToElim = "<<NoVarsToElim<<endl;
            cout<<"\nNoVars = "<<NoVars<<endl;
            cout<<"\n(float)NoVarsToElim/(float)NoVars = "<<ratio<<endl;
            
            if(ratio >= NoVarsToElim_To_NoVars_Ratio_For_Monniaux && NoVarsRemaining < NoVarsRemaining_For_Monniaux)
                {
                cout<<"\nmonniaux\n";
                return "monniaux";
                }
            else
                {
                cout<<"\nlmdd\n";
                return "lmdd";
                }
             }
        }//else if(check_if_lmes_exist)
    else if(multiple_strategy)
    {
        select_strategy_calls++;
        
        if(DDFreeOfVar_WithDP(f, VarToElim))// f free of variable to be eliminated
            {      
            select_strategy_ddfreeofvar++;
            select_strategy_basis = "free";
            //cout<<select_strategy_basis<<endl;
            return "lmdd"; 
            }
        else if(DAGResolveApplicable(E, D, WidthTable))// There are LMEs in context with the variable to be eliminated
            {
            select_strategy_dagresolve++;
            select_strategy_basis = "dagresolve";
            //cout<<select_strategy_basis<<endl;
            return "lmdd"; 
            }
        else if(lowestLevelOfLMEWithVariable(f, VarToElim, 0) <=  Highest_Level_For_QE_LMDD) // There are LMEs in f at a level less than
            // or equal to Highest_Level_For_QE_LMDD
            {
            select_strategy_lowlevel++;
            select_strategy_basis = "lowlevel";
            //cout<<select_strategy_basis<<endl;
            return "lmdd"; 
            }
        else if(findNumberOfNodesWithVarInSupport(f, VarToElim) <= Lowest_Support_Nodes_For_QE_LMDD)
            {
            select_strategy_lownodeswith_lmi_in_support++;
            return "lmdd"; 
            }
        else if(getSizeOfDD(f) <= Lowest_Size_For_QE_LMDD)//Size of f is <= Lowest_Size_For_QE_LMDD
            {
            select_strategy_size++;
            select_strategy_basis = "size";
            //cout<<select_strategy_basis<<endl;
            return "lmdd";
            }
        else 
            {
            set<string> VarsPresent;
            
            SetOfDDNodes.clear(); // clear this first
            getDDNames(f, VarsPresent);
            
            set<string> VarsToElimPresent;
            set_intersection(VarsPresent.begin(), VarsPresent.end(), VarsToElim.begin(), VarsToElim.end(),inserter(VarsToElimPresent, VarsToElimPresent.begin()));

                      
            int NoVars = VarsPresent.size();
            int NoVarsToElim = VarsToElimPresent.size();
            
            float ratio = (float)NoVarsToElim/(float)NoVars;
            int NoVarsRemaining = NoVars - NoVarsToElim;
            
            //cout<<"\nNoVarsToElim = "<<NoVarsToElim<<endl;
            //cout<<"\nNoVars = "<<NoVars<<endl;
            //cout<<"\n(float)NoVarsToElim/(float)NoVars = "<<ratio<<endl;
            
            if(ratio < NoVarsToElim_To_NoVars_Ratio_For_Monniaux || NoVarsRemaining >= NoVarsRemaining_For_Monniaux)
                {
                select_strategy_for_lmdd_default++;
                //cout<<"\nlmdd\n";
                select_strategy_basis = "default";
                //cout<<select_strategy_basis<<endl;
                return "lmdd";                
                }
            else 
                {
                select_strategy_for_monniax++;
                //cout<<"\nmonniaux\n";
                return "monniaux";
                }
        }
    }//else if(multiple_strategy)
}//function


//Extend CurrentPath with PartialPath
void DDManager::extendPath(vector< vector<DDNode*> > &CurrentPath, vector< vector<DDNode*> > &PartialPath)
{
    //Let us first interchange the columns 1 & 2 of last entry of CurrentPath and
    // the columns 3 & 4 of last entry of CurrentPath
    
    vector<DDNode*> LastEntry;
    LastEntry = CurrentPath[CurrentPath.size()-1];
    
    vector<DDNode*> ModifiedLastEntry;
    ModifiedLastEntry.push_back(LastEntry[0]);
    ModifiedLastEntry.push_back(LastEntry[2]);
    ModifiedLastEntry.push_back(LastEntry[1]);
    ModifiedLastEntry.push_back(LastEntry[4]);
    ModifiedLastEntry.push_back(LastEntry[3]);
    
    int LocationOfDeletion = CurrentPath.size()-1;
    CurrentPath.erase(CurrentPath.begin()+LocationOfDeletion);  
    CurrentPath.push_back(ModifiedLastEntry);
    
    
    
    for(int i=0; i<PartialPath.size(); i++)
    {
        vector<DDNode*> Entry = PartialPath[i];
        CurrentPath.push_back(Entry);
    }
}


//Show the present DD based path
void DDManager::showDDPath(vector< vector< DDNode*> > &CurrentPath)
{
    cout<<"CurrentPath\n";
    for(int i=0; i<CurrentPath.size(); i++)
    {
        vector<DDNode*> Entry  = CurrentPath[i];
        DDNode* first = Entry[0];
        DDNode* second = Entry[1];
        DDNode* third = Entry[2];
        DDNode* fourth = Entry[3];
        DDNode* fifth = Entry[4];
        
        if(first==NULL)
        {
           cout<<"\nnode = NULL\t";
        }        
        else if(first->getPredicate()==NULL) // Terminal node
         {
            int value = first->getVal();

            if(value == 0) // 0-node  
                {   
                cout<<"\nnode = 0\t";
                }
            else
                {
                cout<<"\nnode = 1\t";
                }        
         }// Terminal node
        else
        {
         cout<<"\nnode = "<<getDAGStr(first->getPredicate())<<"\t";
        }
        
       if(second==NULL)
        {
           cout<<"\nchild on path = NULL\t";
        }        
        else if(second->getPredicate()==NULL) // Terminal node
         {
            int value = second->getVal();

            if(value == 0) // 0-node  
                {   
                cout<<"\nchild on path = 0\t";
                }
            else
                {
                cout<<"\nchild on path = 1\t";
                }        
         }// Terminal node
        else
        {
         cout<<"\nchild on path = "<<getDAGStr(second->getPredicate())<<"\t";
        } 
      
        
      if(third==NULL)
        {
           cout<<"\nchild off path = NULL\t";
        }        
      else if(third->getPredicate()==NULL) // Terminal node
       {
            int value = third->getVal();

            if(value == 0) // 0-node  
                {   
                cout<<"\nchild off path = 0\t";
                }
            else
                {
                cout<<"\nchild off path = 1\t";
                }        
        }// Terminal node
        else
        {
         cout<<"\nchild off path = "<<getDAGStr(third->getPredicate())<<"\t";
        }  
        
        
        if(fourth==NULL)
        {
           cout<<"\nquantified child on path = NULL\t";
        }        
      else if(fourth->getPredicate()==NULL) // Terminal node
       {
            int value = fourth->getVal();

            if(value == 0) // 0-node  
                {   
                cout<<"\nquantified child on path = 0\t";
                }
            else
                {
                cout<<"\nquantified child on path = 1\t";
                }        
        }// Terminal node
        else
        {
         cout<<"\nquantified child on path = "<<getDAGStr(fourth->getPredicate())<<"\t";
        }  
        
        
        if(fifth==NULL)
        {
           cout<<"\nquantified child off path = NULL\t";
        }        
      else if(fifth->getPredicate()==NULL) // Terminal node
       {
            int value = fifth->getVal();

            if(value == 0) // 0-node  
                {   
                cout<<"\nquantified child off path = 0\t";
                }
            else
                {
                cout<<"\nquantified child off path = 1\t";
                }        
        }// Terminal node
        else
        {
         cout<<"\nquantified child off path = "<<getDAGStr(fifth->getPredicate())<<"\t";
        }  
        
       
        cout<<endl;
        
    }//end of loop       
}//end of function


// Obtain the Path by traversing the LMDD f through the path guided by
// assignment in Model_Of_SatCondition
void DDManager::obtainPathFromGivenAssignment(DDNode* f, map<string, string> &Model_Of_SatCondition, map<string, int> &WidthTable,  vector< vector< DDNode*> > &CurrentPath)
{  
    
  if(f->getPredicate()==NULL) // Terminal node
    {
      int value = f->getVal();

      if(value == 0) // 0-node  
      {
          cout<<"\n0-node encountered in function DDManager::obtainPathFromGivenAssignment while following a satisfiable path\n";
          exit(1);
      }
      else // 1-node
      {
          vector<DDNode*> NewEntry;
          NewEntry.push_back(f);
          NewEntry.push_back(NULL);
          NewEntry.push_back(NULL);
          NewEntry.push_back(NULL);
          NewEntry.push_back(NULL);
          
          CurrentPath.push_back(NewEntry);
          
          return;
      }
    }// Terminal node
  else // Non-terminal node
  {    
      DAGNode* C = f->getPredicate();
      
      if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
                {
                //C is True under given model
                vector<DDNode*> NewEntry;
                NewEntry.push_back(f);
                NewEntry.push_back(f->getRight());
                NewEntry.push_back(f->getLeft());
                NewEntry.push_back(NULL);
                NewEntry.push_back(NULL);
          
                CurrentPath.push_back(NewEntry);
          
          
          
                obtainPathFromGivenAssignment(f->getRight(), Model_Of_SatCondition, WidthTable, CurrentPath);
                 }
      else
                {
                // C is False under given model
                vector<DDNode*> NewEntry;
                NewEntry.push_back(f);
                NewEntry.push_back(f->getLeft());
                NewEntry.push_back(f->getRight());
                NewEntry.push_back(NULL);
                NewEntry.push_back(NULL);
                
                CurrentPath.push_back(NewEntry);
                
                obtainPathFromGivenAssignment(f->getLeft(), Model_Of_SatCondition, WidthTable, CurrentPath);
                }      
  }// Non-terminal node
}// function


//Obtain context from path
void DDManager::obtainContextFromPath(vector< vector<DDNode*> > &CurrentPath, DDNode* h, set<DAGNode*>  &E, set<DAGNode*>  &D, set<DAGNode*>  &I, set<DAGNode*>  &Neg_x, string VarToElim)
{   
    for(int i=0; i<CurrentPath.size(); i++)
    {
        vector<DDNode*> Entry = CurrentPath[i];
        
        DDNode* Node = Entry[0];
        DDNode* ChildOnPath = Entry[1];
        DDNode* ChildOffPath = Entry[2];
        
        if(Node == h)
            {
            break;
            }
        
        DAGNode* C = Node->getPredicate();
        
        if(ChildOnPath == Node->getLeft())
        {
            C = negateConstraint_With_LMISupport(C); 
        }
        
         if(freeOfVarModified(C, VarToElim))
           {
              Neg_x.insert(C) ;
          }
          else if(C->Get_node_name()=="=" || C->Get_node_name()=="equivalent" )
          {
              E.insert(C);
          }
          else if(C->Get_node_name()=="is_not_equal" || C->Get_node_name()=="not_equal_to" )
          {
              D.insert(C);
          }
          else
          {
              I.insert(C);
          }
    }//for
    
}//function


//Show the problem instance
void DDManager::showProblemInstance(DDNode* h, set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, set<DAGNode*> &Neg_x)
{
     if(h==NULL)
        {
           cout<<"\nh = NULL\n";
        }        
      else 
       {
           cout<<"\nh = "<<getDAGStr(createDAGFromDD(h))<<endl;
        } 
     
    showAPSet("E", E); 
    showAPSet("D", D); 
    showAPSet("I", I); 
    showAPSet("Neg_x", Neg_x); 
}




// Obtain the Path by traversing the LMDD f through the path guided by
// assignment in Model_Of_SatCondition only upto the point where LME
// based simplification is possible.
void DDManager::obtainPathFromGivenAssignmentUpToPoint(DDNode* f, map<string, string> &Model_Of_SatCondition, map<string, int> &WidthTable,  vector< vector< DDNode*> > &CurrentPath, string VarToElim, bool parent_is_lme)
{  
    
  if(f->getPredicate()==NULL) // Terminal node
    {
      int value = f->getVal();

      if(value == 0) // 0-node  
      {
          cout<<"\n0-node encountered in function DDManager::obtainPathFromGivenAssignmentUpToPoint while following a satisfiable path\n";
          exit(1);
      }
      else // 1-node
      {
          vector<DDNode*> NewEntry;
          NewEntry.push_back(f);
          NewEntry.push_back(NULL);
          NewEntry.push_back(NULL);
          NewEntry.push_back(NULL);
          NewEntry.push_back(NULL);
          
          CurrentPath.push_back(NewEntry);
          
          return;
      }
    }// Terminal node
  else // Non-terminal node
  {    
      DAGNode* C = f->getPredicate();
      
      if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
                {
                //C is True under given model
                vector<DDNode*> NewEntry;
                NewEntry.push_back(f);
                NewEntry.push_back(f->getRight());
                NewEntry.push_back(f->getLeft());
                NewEntry.push_back(NULL);
                NewEntry.push_back(NULL);
          
                CurrentPath.push_back(NewEntry);
          
                if(parent_is_lme)
                    {
                    return;
                    }
                 
                else
                    {
                    // The node on path is f->getRight(). But f->getRight() is either free of VarToElim or
                    // it has an LME in the context
                    //set parent_is_lme
                    if(DDFreeOfVar_WithDP(f->getRight(), VarToElim) || DAGResolveApplicableUsingConstraint(C, VarToElim, WidthTable))
                        {
                        parent_is_lme = true;
                        }
                    else
                        {
                        parent_is_lme = false;
                        }                    
                    }
                obtainPathFromGivenAssignmentUpToPoint(f->getRight(), Model_Of_SatCondition, WidthTable, CurrentPath, VarToElim, parent_is_lme);
                 }
      else
                {
                // C is False under given model
                vector<DDNode*> NewEntry;
                NewEntry.push_back(f);
                NewEntry.push_back(f->getLeft());
                NewEntry.push_back(f->getRight());
                NewEntry.push_back(NULL);
                NewEntry.push_back(NULL);
                
                CurrentPath.push_back(NewEntry);
                
                if(parent_is_lme)
                    {
                    return;
                    }
                 
                else
                    {
                    // The node on path is f->getLeft(). But f->getLeft() is either free of VarToElim or
                    // it has an LME in the context
                    //set parent_is_lme
                    if(DDFreeOfVar_WithDP(f->getLeft(), VarToElim) || DAGResolveApplicableUsingConstraint(negateConstraint_With_LMISupport(C), VarToElim, WidthTable))
                        {
                        parent_is_lme = true;
                        }
                    else
                        {
                        parent_is_lme = false;
                        }                    
                    }
                
                obtainPathFromGivenAssignmentUpToPoint(f->getLeft(), Model_Of_SatCondition, WidthTable, CurrentPath, VarToElim, parent_is_lme);
                }      
  }// Non-terminal node
}// function


// check if DAGResolve is applicable using this constraint
bool DDManager::DAGResolveApplicableUsingConstraint(DAGNode* C, string VarToElim, map<string, int> &WidthTable)
{
    if(!freeOfVarModified(C, VarToElim))
    {
        if(C->Get_node_name() == "=" || C->Get_node_name() == "equivalent")
        //C is an LME containing VarToElim
        {
        return true;
        }
        else if(C->Get_node_name() == "not_equal_to" || C->Get_node_name() == "is_not_equal")
            {//C is an LMD containing VarToElim
            if( getNoOfBitsInConstraint(C, WidthTable) == 1)
                {//#of bits = 1. Hence effectively LME
                return true;
                }
            else
                {
                return false;
                }
            }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
                
}


// Returns the number of nodes in the DD with the variable in support
int DDManager::findNumberOfNodesWithVarInSupport(DDNode *v, string VarToElim)
{
  map<string, int> VarsToElimCount;
  VarsToElimCount.insert(make_pair(VarToElim, 0));
  
  HashTable <string, bool> DropVarsHT;
  HashTable <string, bool> *PtrDropVarsHT = &DropVarsHT;
  
  findDropVarsStepEfficient(v, VarsToElimCount, PtrDropVarsHT);
  
  map<string, int>::iterator mit = VarsToElimCount.begin();
  return mit->second;
}


// Returns the level of the lowest LME with var. to elim
int DDManager::lowestLevelOfLMEWithVariable(DDNode* f, string VarToElim, int level)
{
    if(level > Highest_Level_For_QE_LMDD)
    {
        return Highest_Level_For_QE_LMDD+1;
    }
    else
    {
        DAGNode *C = f->getPredicate();
        
        if(C == NULL)
        {
            return Highest_Level_For_QE_LMDD+1;
        }
        else
        {
            if( (C->Get_node_name()=="=" || C->Get_node_name() == "equivalent") && !freeOfVarModified(C, VarToElim))
            {
                return level;
            }
            else
            {
                int level_h = lowestLevelOfLMEWithVariable(f->getRight(), VarToElim, level+1);
                int level_l = lowestLevelOfLMEWithVariable(f->getLeft(), VarToElim, level+1);
                
                if(level_l < level_h) 
                        return level_l;
                else
                        return level_h;
            }
        }
    }
}


// Recreate the NonHashableNodesInCurrentPath by removing the DDNode* h
void DDManager::removeEntryFromNonHashableNodesInCurrentPath(set<DDNode*> &NonHashableNodesInCurrentPath, DDNode* NodeToRemove)
{
    set<DDNode*> NewSet;
    
    for(set<DDNode*>::iterator it=NonHashableNodesInCurrentPath.begin(); it != NonHashableNodesInCurrentPath.end(); it++)
    {
        if(*it != NodeToRemove)
        {
            NewSet.insert(*it);
        }//if
    }//for
    
   NonHashableNodesInCurrentPath = NewSet; 
}


//Enter all nodes in CurrentPath into NonHashableNodesInCurrentPath
void DDManager::updateNonHashableNodesInCurrentPath(set<DDNode*> &NonHashableNodesInCurrentPath, vector< vector< DDNode*> > &CurrentPath)
{
    for(int i=0; i<CurrentPath.size(); i++)
    {       
        NonHashableNodesInCurrentPath.insert(CurrentPath[i][0]);
    }
}


//Show the set of DDNodes
void DDManager::showDDSet(string WhoamI, set<DDNode*> &NonHashableNodesInCurrentPath)
{
    cout<<endl<<WhoamI<<endl;
    for(set<DDNode*>::iterator it=NonHashableNodesInCurrentPath.begin(); it != NonHashableNodesInCurrentPath.end(); it++)
    {
       DDNode* h = *it;
       if(h==NULL)
        {
           cout<<"\nNULL\n";
        }        
      else 
       {
           cout<<"\n"<<getDAGStr(h->getPredicate())<<endl;
        } 
    }//for
}//function



// Update the given context by traversing the LMDD f through the path guided by
// assignment in Model_In_LMDDWithMonniauxTypeSimplifications, and stop
// at EasyNode, where EasyNode is 1) 1-node or 2) node free of VarToElim ,or
// 3) node such that the occurances of VarToElim in it can be eliminated by an LME in Context
void DDManager::updateContextFromGivenAssignmentUptoEasyNode(DDNode* f, map<string, string> &Model_Of_SatCondition, map<string, int> &WidthTable, set<DAGNode*> &Context, DDNode* &EasyNode, string VarToElim)
{
  if(f->getPredicate()==NULL) // Terminal node
    {
      int value = f->getVal();

      if(value == 0) // 0-node  
      {
          cout<<"\n0-node encountered in function DDManager::updateContextFromGivenAssignmentUptoEasyNode while following a satisfiable path\n";
          exit(1);
      }
      else // 1-node
      {
          EasyNode = f;
          return;
      }
    }// Terminal node
  else // Non-terminal node
  {
      DAGNode* C = f->getPredicate();
      
      //cout<<"\nC = "<<getDAGStr(C)<<endl;
      
      if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
                {
                //C is True under given model
                //cout<<"\nC is True under given model\n";
                //cout<<"\nModel is\n";
                //showModel(Model_Of_SatCondition);
                Context.insert(C);
                
                // The node on path is f->getRight(). But if f->getRight() is either free of VarToElim or
                // it has an LME in the context, stop 
                
                if(DDFreeOfVar_WithDP(f->getRight(), VarToElim) || DAGResolveApplicableUsingConstraint(C, VarToElim, WidthTable))
                        {
                        EasyNode = f->getRight();
                        return;
                        }              
          
                updateContextFromGivenAssignmentUptoEasyNode(f->getRight(), Model_Of_SatCondition, WidthTable, Context, EasyNode, VarToElim);
                }
      else
                {
                // C is False under given model
                //cout<<"\nC false True under given model\n";
                //cout<<"\nModel is\n";
                //showModel(Model_Of_SatCondition);
                Context.insert(negateConstraint_With_LMISupport(C));
                 
                 // The node on path is f->getRight(). But if f->getRight() is either free of VarToElim or
                // it has an LME in the context, stop 
                
                if(DDFreeOfVar_WithDP(f->getLeft(), VarToElim) || DAGResolveApplicableUsingConstraint(negateConstraint_With_LMISupport(C), VarToElim, WidthTable))
                        {
                        EasyNode = f->getLeft();
                        return;
                        }     
                 
                 updateContextFromGivenAssignmentUptoEasyNode(f->getLeft(), Model_Of_SatCondition, WidthTable, Context, EasyNode, VarToElim);
                }      
  }// Non-terminal node
}// function



// Compute \exists VarsToElim. (f /\ Context) using LMDD approach
// We should not create new hash tables. The existing hash tables
// PtrTableForQElimModified and PtrTableForAPResolveModified
// are passed as arguments
DAGNode* DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext(DDNode* f, set<DAGNode*> &Context, set<string> &VarSetToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
  time_t t1, t2;

  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> LMESet;
  set<DAGNode*> LMDSet;
  set<DAGNode*> LMISet;
  
  list<string> VarsToElim;
  list<string> VarsStillToElim;
  copySetToList(VarSetToElim, VarsToElim);
  
  DDNode *result;
  result = f;
  
  int SizeOfMyDD = getSizeOfDD(f);
  
  //cout<<"\nSize of dd = "<<SizeOfMyDD<<endl;
  
  if(SizeOfMyDD > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = SizeOfMyDD;
	}  
  
  SizesOfEliminatedLMDDs.push_back(SizeOfMyDD);
  
  fprintf(LMIQEOutputDataFp, "\nSize of dd inside DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext = %d\n", SizeOfMyDD);
  fprintf(LMIQEOutputDataFp, "\nSize of context inside DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext = %d\n", Context.size());
  fprintf(LMIQEOutputDataFp, "\n#vars to elim in DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext = %d\n", VarSetToElim.size());
  TotalSizeOfTriangles = TotalSizeOfTriangles + SizeOfMyDD;
  TotalSizeOfTails = TotalSizeOfTails + Context.size();
  TotalVarsToElimFromTailedTriangles = TotalVarsToElimFromTailedTriangles + VarSetToElim.size();
  
  int eliminated_by_firstloop=0;
  int eliminated_by_secondloop=0;
  int eliminated_by_thirdloop=0;
  
  OrderInWhichVariablesAreEliminated.push_back("{");

  VarsStillToElim.clear();
  while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}
      
      string VarToElim;
      list<string>::iterator lit = VarsToElim.begin();
      VarToElim = *lit;    
      
      
      
      if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
        }
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Context, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      if(DAGResolveApplicable(LMESet, LMDSet, WidthTable))
        {
          
        OrderInWhichVariablesAreEliminated.push_back(VarToElim);  
          
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {  
            cout<<"\nDAGResolveApplicable(LMESet, LMDSet, WidthTable)\n";
            }
        
        eliminated_by_firstloop++; 
          
        TableForDagResolve.clear();
        
        result = QElim_WithLMISupport(VarToElim, result, LMESet, LMDSet, LMISet, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
    
        SizeOfMyDD = getSizeOfDD(result);
        
        //cout<<"\nSize of dd = "<<SizeOfMyDD<<endl;
        
        if(SizeOfMyDD > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = SizeOfMyDD;
	}
                
        SizesOfEliminatedLMDDs.push_back(SizeOfMyDD);
                
        VarsToElim.remove(VarToElim); 
        Context = Free_Constraints;
        }   
      else
        {
         if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            { 
            cout<<"\n~DAGResolveApplicable(LMESet, LMDSet, WidthTable)\n";         
            }
         
         VarsStillToElim.push_back(VarToElim);
         VarsToElim.remove(VarToElim); 
        }
    }//end(while)
  
  
  VarsToElim = VarsStillToElim;
  VarsStillToElim.clear();
  while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}
      
      string VarToElim;
      list<string>::iterator lit = VarsToElim.begin();
      VarToElim = *lit;    
      
            
      if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
        }
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Context, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      if(DDFreeOfVar_WithDP(result, VarToElim))
        {
        OrderInWhichVariablesAreEliminated.push_back(VarToElim);  
          
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {  
            cout<<"\nDDFreeOfVar_WithDP(result, VarToElim)\n";
            }
        
        eliminated_by_secondloop++; 
          
        TableForDagResolve.clear();
        
        result = QElim_WithLMISupport(VarToElim, result, LMESet, LMDSet, LMISet, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
    
        
        SizeOfMyDD = getSizeOfDD(result);
        
        //cout<<"\nSize of dd = "<<SizeOfMyDD<<endl;
        
        if(SizeOfMyDD > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = SizeOfMyDD;
	}
        
        
        SizesOfEliminatedLMDDs.push_back(SizeOfMyDD);        
        
        VarsToElim.remove(VarToElim); 
        Context = Free_Constraints;
        }   
      else
        {
          if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\n~DDFreeOfVar_WithDP(result, VarToElim)\n";
            }
          
         eliminated_by_thirdloop++;
         
         VarsStillToElim.push_back(VarToElim);
         VarsToElim.remove(VarToElim); 
        }
    }//end(while)
  
  
  VarsToElim = VarsStillToElim;
  while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}
      
      string VarToElim;
      VarToElim = chooseVar(result, VarsToElim);
      
      OrderInWhichVariablesAreEliminated.push_back(VarToElim);
      
      if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
            }
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Context, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      if(DDFreeOfVar_WithDP(result, VarToElim) && Bound_Constraints.empty())//elimination is not required
        {
          if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
             cout<<"\nDDFreeOfVar_WithDP(result, VarToElim) && Bound_Constraints.empty()\n";
            }
          
         VarsToElim.remove(VarToElim); 
        }
      else//elimination is required
        {
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {  
            cout<<"\n~(DDFreeOfVar_WithDP(result, VarToElim) && Bound_Constraints.empty())\n";
            }
        
        TableForDagResolve.clear();
        
        result = QElim_WithLMISupport(VarToElim, result, LMESet, LMDSet, LMISet, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
    
        
        SizeOfMyDD = getSizeOfDD(result);
        
        //cout<<"\nSize of dd = "<<SizeOfMyDD<<endl;
        
        if(SizeOfMyDD > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = SizeOfMyDD;
	}       
        
        
        SizesOfEliminatedLMDDs.push_back(SizeOfMyDD);        
        
        VarsToElim.remove(VarToElim); 
        Context = Free_Constraints;
        }      
    }//end(while)
  
  OrderInWhichVariablesAreEliminated.push_back("}");
  
   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function QuaLMoDE_WithLMISupport in DDManager.cpp has timed out. It's result is not exact\n";
       timed_out = true; // timed_out flag set
       return createLogicalConstant("true");
     }
  
  DAGNode* ConjunctionOfContext = getConjunctionFromSet(Context);
  DAGNode* LMDDPart = createDAGFromDD(result);
  
  DAGNode* FinalResult = quickSimplify_WithLMISupport(createDAG("and", LMDDPart, ConjunctionOfContext, LOGICAL_SYMBOL, 1));
  
  fprintf(LMIQEOutputDataFp, "\n#Vars benefited by LMEs in Context = %d\n", eliminated_by_firstloop);
  fprintf(LMIQEOutputDataFp, "\n#Vars such that LMDD is free of the variable = %d\n", eliminated_by_secondloop);
  fprintf(LMIQEOutputDataFp, "\n#Remaining variables = %d\n", eliminated_by_thirdloop);
  TotalVarsEliminatedByLMEsFromTailedTriangles = TotalVarsEliminatedByLMEsFromTailedTriangles + eliminated_by_firstloop;
  TotalVarsEliminatedFreeTrianglesFromTailedTriangles = TotalVarsEliminatedFreeTrianglesFromTailedTriangles + eliminated_by_secondloop;
  TotalRemainingVarsEliminatedFromTailedTriangles = TotalRemainingVarsEliminatedFromTailedTriangles + eliminated_by_thirdloop;
  
  return FinalResult;
}
  


// Returns an LMDD g such that g \vee PartialResult_In_LMDDWithMonniauxTypeSimplifications \equiv \exists VarsToElim. f
// f is an LMDD involving LMEs, LMDs, and LMIs
// QE is done with LMDD based all-variable style QE combined with Monniaux type simplifications
DDNode* DDManager::call_QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(set<string> &VarSetToElim, DDNode* f, map<string, int> &WidthTable)
{ 
    
  set<DAGNode*> C_X, Neg_C_X;
  C_X.clear(); Neg_C_X.clear();
  
  int project_depth = 0;

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForAllVarsQE;
  HashTable <string, DDNode*> *PtrTableForAllVarsQE = &TableForAllVarsQE;
  
  HashTable <string, DDNode*> PerVariableHT;
  HashTable <string, DDNode*> *PtrPerVariableHT = &PerVariableHT;
  
  if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
    {
    cout<<"\nCalling QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy\n";
    }
  
  DDNode* result = QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(VarSetToElim, f, C_X, Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, PtrPerVariableHT);
 
   if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
    {
    cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;
    }

   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function DDManager::call_QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy has timed out. Its result is not exact\n";
       timed_out = true; // timed_out flag set
       return result;
     }


  return result;
}



// The function called by call_QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy
DDNode* DDManager::QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(set<string> VarSetToElim, DDNode* f, set<DAGNode*> C_X, set<DAGNode*> Neg_C_X, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForAPResolveModified, HashTable <string, DDNode*> *PtrTableForAllVarsQE, int project_depth, HashTable <string, DDNode*> *PtrPerVariableHT)
{
  // check if we have already timed out   
 if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	} 
 
 CallsToQEWithMonEnabledAllVarStrategy++;
 
 if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
    {
    cout<<"\nCalling DDManager::QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy with parameters\n";
    showSet(VarSetToElim, "VarSetToElim");
    cout<<"\nf = "<<getDAGStr(createDAGFromDD(f))<<endl;
    showAPSet("C_X", C_X);
    showAPSet("Neg_C_X", Neg_C_X);
    }
  
  //Check if there exists an entry for \exists VarSetToElim. (f /\ C_X)
  DDNode* Existing  = getEntryFromTableForAllVarsQE(VarSetToElim, f, C_X, PtrTableForAllVarsQE);

  if(Existing != NULL)
    {
      CallsToQEWithMonEnabledAllVarStrategyGlobalHashed++;
      
      //cout<<"\nEntry existing in hash table returned\n";
      
       if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
        {
         cout<<"\nEntry existing in hash table returned\n";
        }
       
      return Existing;
    }    
  
  DDNode* result;
  
  //Check if f is free of VarSetToElim  
  
  // First check if f is a terminal. If yes, f is free of VarSetToElim
  if(f->getPredicate()==NULL) // Terminal node
    {
      
      CallsToQEWithMonEnabledAllVarStrategyOnTerminals++;
      
      //cout<<"\nTerminal node reached\n";
      
       if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
        {
        cout<<"\nTerminal node reached\n";
        }

      int value = f->getVal();

      if(value == 0)//0-node
        {
           if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                {
                cout<<"\nresult=0\n";
                }
          result = f;
        }//0-node
      else//1-node
        {

          if(C_X.empty())
            {
               if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nContext empty. hence result=1\n";
                    }
              result = createDD(1);
            }
          else if(ThUnsat(C_X, WidthTable))
            {
               if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nContext inconsistent. hence result=0\n";
                    }
	      result = createDD(0);
            }
          else
            {
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nContext consistent. hence result = \exists VarSetToElim. C_X. callProjectReturningLMDDForMultipleVariables invoked\n";
                    }
              result = callProjectReturningLMDDForMultipleVariables(C_X, VarSetToElim, WidthTable, PtrTableForAllVarsQE, PtrTableForAPResolveModified, project_depth);
               
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;
                    }
            }
        }//1-node
  }//Terminal node
  else //Non-terminal node
  {
      if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nf is a non-terminal\n";
                    }
      // Check if f is already free of VarSetToElim
       if(DDFreeOfVarSet_WithDP(f, VarSetToElim))//f is already free of VarSetToElim
        { 
           
          CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsFreeOfAllVars++; 
           
          //cout<<"\nnon-terminal f is free of all variables to be eliminated\n";   
           
          if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                     cout<<"\nf is free of all variables to be eliminated\n";  
                    }

          // result = f /\ \exists VarSetToElim. C_X
          // Let's make temp =  \exists VarSetToElim. C_X
          
          if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                     cout<<"\nLet's make temp =  exists VarSetToElim. C_X\n";  
                    }

          DDNode *temp;

          if(C_X.empty())
            {
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nContext empty. hence temp=1\n";
                     }
              temp = createDD(1);                            
            }                          
          else if(ThUnsat(C_X, WidthTable))
            {
             if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nContext inconsistent. hence temp=0\n";
                    }
              temp = createDD(0);
            }
          else 
            {
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nContext consistent. hence temp = \exists VarSetToElim. C_X. callProjectReturningLMDDForMultipleVariables invoked\n";
                    }
              temp = callProjectReturningLMDDForMultipleVariables(C_X, VarSetToElim, WidthTable, PtrTableForAllVarsQE, PtrTableForAPResolveModified, project_depth);
              
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;
                    }
            }

          //cout<<"\ncallProjectReturningLMDD on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

          // Now result = f /\ temp
          if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nresult = f AND temp"<<endl;
                    }
          
          result = apply("and", f, temp);
        }// if(DDFreeOfVar_WithDP(f, VarToElim))
       else//f is not free of VarSetToElim
       {
            // Let's try to simplify the problem instance \exists VarSetToElim. (f /\ C_X) using LMEs in C_X
          
          CallsToGetSimplifiedInstance++;
          
          //cout<<"\nLet's try to simplify the problem instance using LMEs in C_X\n";  
          
          if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                     cout<<"\nLet's try to simplify the problem instance using LMEs in C_X\n";  
                    }
          
           DDNode* Simplified_f;
           set<DAGNode*> Simplified_C_X;
           set<string> Simplified_VarSetToElim;
           
           struct timeval start_ms, finish_ms;
           unsigned long long int duration_ms;
           
           gettimeofday (&start_ms, NULL);
           
           getSimplifiedInstance(f, C_X, VarSetToElim, Simplified_f, Simplified_C_X, Simplified_VarSetToElim, WidthTable, PtrTableForAllVarsQE, PtrTableForAPResolveModified, project_depth, PtrPerVariableHT);
           
           gettimeofday (&finish_ms, NULL);
           
           duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
           duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
           
           cout<<"\nTime spent in getSimplifiedInstance = "<<duration_ms<<" milliseconds\n";
           
           TimeSpentInGetSimplifiedInstance = TimeSpentInGetSimplifiedInstance + duration_ms;
           
           //Now we have the simplified problem instance \exists Simplified_VarSetToElim. (Simplified_f /\ Simplified_C_X)
           if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                {
                cout<<"\nSimplified Instance\n";
                showSet(Simplified_VarSetToElim, "Simplified_VarSetToElim");
                cout<<"\nSimplified_f = "<<getDAGStr(createDAGFromDD(Simplified_f))<<endl;
                showAPSet("Simplified_C_X", Simplified_C_X);
                }
           
           
           // Check if Simplified_f is free of Simplified_VarSetToElim
           if(DDFreeOfVarSet_WithDP(Simplified_f, Simplified_VarSetToElim))//Simplified_f is free of Simplified_VarSetToElim
            {
              CallsToQEWithMonEnabledAllVarStrategyOnNonTerminalsWhereCallToGetSimplifiedInstanceSolved++; 
               
              //cout<<"\nSimplified_f is free of all variables to be eliminated\n";   
               
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {
                         cout<<"\nSimplified_f is free of all variables to be eliminated\n";  
                        }

              // result = Simplified_f /\ \exists Simplified_VarSetToElim. Simplified_C_X
              // Let's make temp =  \exists Simplified_VarSetToElim. Simplified_C_X

              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {
                         cout<<"\nLet's make temp =  exists Simplified_VarSetToElim. Simplified_C_X\n";  
                        }

              DDNode *temp;

              if(Simplified_C_X.empty())
                {
                  if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {
                        cout<<"\nContext empty. hence temp=1\n";
                         }
                  temp = createDD(1);                            
                }                          
              else if(ThUnsat(Simplified_C_X, WidthTable))
                {
                 if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {
                        cout<<"\nContext inconsistent. hence temp=0\n";
                        }
                  temp = createDD(0);
                }
              else 
                {
                  if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {
                        cout<<"\nContext consistent. hence temp = \exists Simplified_VarSetToElim. Simplified_C_X. callProjectReturningLMDDForMultipleVariables invoked\n";
                        }
                  temp = callProjectReturningLMDDForMultipleVariables(Simplified_C_X, Simplified_VarSetToElim, WidthTable, PtrTableForAllVarsQE, PtrTableForAPResolveModified, project_depth);

                  if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {
                        cout<<"\ntemp = "<<getDAGStr(createDAGFromDD(temp))<<endl;
                        }
                }

              //cout<<"\ncallProjectReturningLMDD on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;

              // Now result = Simplified_f /\ temp
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {
                        cout<<"\nresult = Simplified_f AND temp"<<endl;
                        }

              result = apply("and", Simplified_f, temp);
            }// if(DDFreeOfVar_WithDP(Simplified_f, Simplified_VarSetToElim))
           else//Simplified_f is not free of Simplified_VarSetToElim
           {    
               // Traverse down
               //cout<<"\nSimplified_f is not free of all variables to be eliminated. Let's traverse down\n";  
               
              DAGNode* C = Simplified_f->getPredicate();
              //cout<<"\nPredicate encountered = "<<getDAGStr(C)<<endl;
              if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                    {
                    cout<<"\nPredicate encountered = "<<getDAGStr(C)<<endl;
                    cout<<"\nTraversing down"<<endl;
                    }

              if(freeOfVarsEfficient(C, VarSetToElim))//predicate free of all vars to elim
                    {
                    // result := ITE(p, QE(H(f'), X', C_{X'}, C_{\neg X}\cup {p}), QE(L(f'), X', C_{X'}, C_{\neg X}\cup {\neg p}))

                    //cout<<"\nPredicate free of all variables to be eliminated"<<endl;
                     if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {                    
                        cout<<"\nPredicate free of all variables to be eliminated"<<endl;
                        }
                    set<DAGNode*> Neg_C_X_then, Neg_C_X_else;

                    copySet(Neg_C_X_then, Neg_C_X);
                    Neg_C_X_then.insert(C);

                    copySet(Neg_C_X_else, Neg_C_X);
                    Neg_C_X_else.insert(negateConstraint_With_LMISupport(C));   

                    DDNode *thenb = QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(Simplified_VarSetToElim, Simplified_f->getRight(), Simplified_C_X,  Neg_C_X_then, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, PtrPerVariableHT);
                    DDNode *elseb = QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(Simplified_VarSetToElim, Simplified_f->getLeft(), Simplified_C_X,  Neg_C_X_else, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, PtrPerVariableHT);

                    DDNode* C_dd = createDD(C, WidthTable);

                    result = createITE(C_dd, thenb, elseb);
                    }
              else//predicate is not free of all vars to elim
                    {
                    //result := OR(QE(H(f'), X', C_{X'}\cup {p}, C_{\neg X}), QE(L(f'), X', C_{X'}\cup {\neg p}, C_{\neg X}))        
                    //cout<<"\nPredicate not free of all variables to be eliminated"<<endl;
                    if(debug_check_in_use_monniaux_enabled_all_variable_strategy)
                        {                    
                        cout<<"\nPredicate not free of all variables to be eliminated"<<endl;
                        }

                    set<DAGNode*> Simplified_C_X_then, Simplified_C_X_else;

                    copySet(Simplified_C_X_then, Simplified_C_X);
                    Simplified_C_X_then.insert(C);

                    copySet(Simplified_C_X_else, Simplified_C_X);
                    Simplified_C_X_else.insert(negateConstraint_With_LMISupport(C));   

                    DDNode *thenb = QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(Simplified_VarSetToElim, Simplified_f->getRight(), Simplified_C_X_then,  Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, PtrPerVariableHT);
                    DDNode *elseb = QuaLMoDE_WithLMISupport_WithMonniauxEnabledAllVariableStrategy(Simplified_VarSetToElim, Simplified_f->getLeft(), Simplified_C_X_else,  Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, PtrPerVariableHT);

                    result = apply("or", thenb, elseb);
                    }//predicate is not free of all vars to elim
           }//Simplified_f is not free of Simplified_VarSetToElim
       }//f is not free of VarSetToElim
  }////Non-terminal node
  
  // Store \exists X.(f \wedge C_{X}) ---> result into Hash Table
  insertEntryIntoTableForAllVarsQE(VarSetToElim, f, C_X, result, PtrTableForAllVarsQE);
  
  // return the result
  return result;      
}//function ends here





// Check if the instance \exists VarSetToElim. (f /\ C_X) is already encountered
// If yes, return the result; NULL otherwise
DDNode* DDManager::getEntryFromTableForAllVarsQE(set<string> &VarSetToElim, DDNode* f, set<DAGNode*> &C_X, HashTable <string, DDNode*> *PtrTableForAllVarsQE)
{
  // Creating the hash key

  string key = "";
  
  bool variables_not_needed_in_hash_key = true;
    
  if(!variables_not_needed_in_hash_key)
    { 
    for(set<string>::iterator it = VarSetToElim.begin(); it != VarSetToElim.end(); it++)
        {
        key += *it;
        }
    }

  string temp_key;
  char temp_char[10];
  
  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = C_X.begin(); it != C_X.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForAllVarsQE->hashtable_search(key);
  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}



// Return true if /\C_X is unsat; false otherwise
// Uses STP/yices
// Calls bool DDManager::ThUnsat(set<DAGNode*> &E, set<DAGNode*> &D, set<DAGNode*> &I, map<string, int> &WidthTable)
// with empty dummy sets for D, I
bool DDManager::ThUnsat(set<DAGNode*> &C_X, map<string, int> &WidthTable)
{
  set<DAGNode*> E;
  set<DAGNode*> D;
  set<DAGNode*> I;
  
  E = C_X;
  D.clear();
  I.clear();
  
  return ThUnsat(E, D, I, WidthTable);
}


// Enter \exists VarSetToElim. (f /\ C_X) ---> result into Hash Table
void DDManager::insertEntryIntoTableForAllVarsQE(set<string> &VarSetToElim, DDNode* f, set<DAGNode*> &C_X, DDNode* value, HashTable <string, DDNode*> *PtrTableForAllVarsQE)
{
  // Creating the hash key

  string key = "";
  
  bool variables_not_needed_in_hash_key = true;
    
  if(!variables_not_needed_in_hash_key)
    { 
    for(set<string>::iterator it = VarSetToElim.begin(); it != VarSetToElim.end(); it++)
        {
        key += *it;
        }
    }

  string temp_key;
  char temp_char[10];
  
  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<DAGNode*>::iterator it = C_X.begin(); it != C_X.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForAllVarsQE->hashtable_search(key);
  
  if(result.success())
    {
      // Do nothing
    }
  else
    {
     result = PtrTableForAllVarsQE->hashtable_insert(key, value);
     if(!result.success())
        {
          cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForAllVarsQE\n";
          exit(1);
        }
    }//else
}


// DDFreeOfVar_WithDP extended to a set of variables
bool DDManager::DDFreeOfVarSet_WithDP(DDNode *dd, set<string> &VarSetToElim)
{
  HashTable <string, bool> HTForDDFreeOfVar;
  HashTable <string, bool> *PtrHTForDDFreeOfVar = &HTForDDFreeOfVar;

  bool DDIsFree = DDFreeOfVarSet_WithDP_Internal(dd,  VarSetToElim, PtrHTForDDFreeOfVar);
  return DDIsFree;
}

// Function called by DDFreeOfVarSet_WithDP
bool DDManager::DDFreeOfVarSet_WithDP_Internal(DDNode *dd, set<string> &VarSetToElim, HashTable <string, bool> *PtrHTForDDFreeOfVar)
{
  char temp_char[10];
  sprintf(temp_char , "%x", dd);
  string key = temp_char;
  
  // key created

  HTStatusValue<bool> ResultFromHT = PtrHTForDDFreeOfVar->hashtable_search(key);

  if(ResultFromHT.success())
    {
      //cout<<"\nHash table hit\n";
      return ResultFromHT.GetValue();
    }
  
  //cout<<"\nHash table miss\n";  
  bool ret_value;
  
  if(dd->getPredicate()!=NULL)
  {
      DAGNode *predicate = dd->getPredicate();
      
      if(!(freeOfVarsEfficient(predicate, VarSetToElim))) 
      {
          ret_value = false;
      }
      else if(!DDFreeOfVarSet_WithDP_Internal(dd->getLeft(), VarSetToElim, PtrHTForDDFreeOfVar))
      {
          ret_value = false;
      }
      else if(!DDFreeOfVarSet_WithDP_Internal(dd->getRight(), VarSetToElim, PtrHTForDDFreeOfVar))
      {
          ret_value = false;
      }
      else
      {
          ret_value = true;      
      }
  }// if(dd->getPredicate()!=NULL)
  else
  {
       ret_value = true;
  }// if(dd->getPredicate()==NULL)
  
  ResultFromHT = PtrHTForDDFreeOfVar->hashtable_insert(key, ret_value);
  if(!ResultFromHT.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::DDFreeOfVarSet_WithDP_Internal\n";
      exit(1);
    }
  return ret_value;     
}


// Version of copySet which works on sets of strings
void DDManager::copySetNew(set<string> &dest, set<string> &source)
{
  set<string>::iterator it;
  for(it=source.begin(); it!=source.end(); it++)
    dest.insert(*it);
}


// Given \exists X. (f \wedge C_{X}), this function returns
// \exists X'. (f' \wedge C_{X'}) such that \exists X. (f \wedge C_{X}) \equiv \exists X'. (f' \wedge C_{X'})
// where X' \subseteq X. The variables in X\X' are variables such that they can be 
// eliminated easily from the LMDD f.
void DDManager::getSimplifiedInstance(DDNode* f, set<DAGNode*> &C_X, set<string> &VarSetToElim, DDNode* &Simplified_f, set<DAGNode*> &Simplified_C_X, set<string> &Simplified_VarSetToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForAllVarsQE, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth, HashTable <string, DDNode*> *PtrPerVariableHT)
{
  struct timeval start_ms, finish_ms;
  struct timeval start_ms2, finish_ms2;
  unsigned long long int duration_ms;  
  unsigned long long int duration_ms2;  
           
  //bool allow_costly_projects_in_simplification = false;
  // The problem instance here is \exists VarSetToElim. (f /\ C_X)
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> LMESet;
  set<DAGNode*> LMDSet;
  set<DAGNode*> LMISet;
  
  Simplified_f = f;
  copySetNew(Simplified_C_X, C_X);
  Simplified_VarSetToElim.clear();
  
  // The problem instance here is \exists VarSetToElim. (Simplified_f /\ Simplified_C_X)
  
  for(set<string>::iterator it=VarSetToElim.begin(); it!=VarSetToElim.end(); it++)
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::getSimplifiedInstance has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return;
	}
      
      string VarToElim = *it;          
      cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
      
      gettimeofday (&start_ms, NULL);
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Simplified_C_X, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      //Simplified_C_X is now expressed as C_x /\ C_{\neg x}
      //Here C_x is Bound_Constraints, C_{\neg x} is Free_Constraints
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      gettimeofday (&finish_ms, NULL);
      duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
      duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
      cout<<"\nTime spent in scope reduction = "<<duration_ms<<" milliseconds\n";
      
      //C_x is now expressed as E_x /\ D_x /\ I_x
      //Here E_x, D_x, I_x are LMESet,  LMDSet,  LMISet respectively
      
      // Problem instance is now \exists (VarSetToElim \ x). (C_{\neg x} /\ \exists x. (Simplified_f /\ E_x /\ D_x /\ I_x))
      
      /* Let us comment the case where Simplified_f is free of VarToElim
       
      if(allow_costly_projects_in_simplification && DDFreeOfVar_WithDP(Simplified_f, VarToElim))
        {
        // In  \exists x. (Simplified_f /\ E_x /\ D_x /\ I_x), Simplified_f is free of x
        // Hence  \exists x. (Simplified_f /\ E_x /\ D_x /\ I_x) \equiv  Simplified_f /\  \exists x. (E_x /\ D_x /\ I_x)
        // Hence let's set 
        // Simplified_f  <------ Simplified_f /\  \exists x. (E_x /\ D_x /\ I_x)
        // Now problem instance becomes \exists (VarSetToElim \ x). (C_{\neg x} /\ Simplified_f)
        // Hence let's set Simplified_C_X <--------- C_{\neg x} and 
        // let's not put x in Simplified_VarSetToElim (x is already eliminated)
          
        //cout<<"\nSimplified_f free of "<<VarToElim<<endl;  
          
        DDNode *temp; //temp \equiv  \exists x. (E_x /\ D_x /\ I_x)
        
        if(Bound_Constraints.empty())
            {
	      //cout<<"\nContext empty. hence temp=1\n";
              temp = createDD(1);
            }                          
        else if(ThUnsat(Bound_Constraints, WidthTable))
            {
              //cout<<"\nContext inconsistent. \exists x. (E_x /\ D_x /\ I_x) is false\n";
              temp = createDD(0);
            }
        else //Let's compute \exists x. (E_x /\ D_x /\ I_x)
            {
              //cout<<"\nContext consistent. Project_ReturningLMDDForMultipleVariables invoked\n";
              // Note that the problem instance here is \exists x. (E_x /\ D_x /\ I_x)
              // We have a function callProjectReturningLMDD to compute this
              // But let us call callProjectReturningLMDDForMultipleVariables as this is the function,
              // which is called at the other places when the argument UseMonniauxEnabledAllVariableStrategy is true
              set<string> TempVarSetToElim;
              TempVarSetToElim.insert(VarToElim);
              temp = callProjectReturningLMDDForMultipleVariables(Bound_Constraints, TempVarSetToElim, WidthTable, PtrTableForAllVarsQE, PtrTableForAPResolveModified, project_depth);
             }
        //cout<<"\ncallProjectReturningLMDDForMultipleVariables on context gives  "<<getDAGStr(createDAGFromDD(temp))<<endl;
        
        // We have temp \equiv  \exists x. (E_x /\ D_x /\ I_x) here
        // Hence let's set 
        // Simplified_f  <------ Simplified_f /\  \exists x. (E_x /\ D_x /\ I_x)
        Simplified_f = apply("and", Simplified_f, temp);
        // Let's set Simplified_C_X <--------- C_{\neg x}
        Simplified_C_X = Free_Constraints;
        }
      else */
      
      gettimeofday (&start_ms, NULL);
      
      if(DAGResolveApplicable(LMESet, LMDSet, WidthTable))
        {
        // There exists an LME or an LMD at bit-level
        // i.e. In  \exists x. (Simplified_f /\ E_x /\ D_x /\ I_x), Simplified_f can be simplified using E_x, D_x
        // Hence  \exists x. (Simplified_f /\ E_x /\ D_x /\ I_x) \equiv  \exists x. (Simplified_f' /\ E_x /\ D_x /\ I_x)
          
        // If Simplified_f' is free of x, this is \equiv to Simplified_f' /\  \exists x. (E_x /\ D_x /\ I_x)
        // In this case, let's set 
        // Simplified_f  <------ Simplified_f' /\  \exists x. (E_x /\ D_x /\ I_x)
        // Now problem instance becomes \exists (VarSetToElim \ x). (C_{\neg x} /\ Simplified_f)
        // Hence let's set Simplified_C_X <--------- C_{\neg x} and 
        // let's not put x in Simplified_VarSetToElim (x is already eliminated)
          
        // In the other case, i.e. if  Simplified_f' is not free of x, the problem is still
        // \exists (VarSetToElim \ x). (C_{\neg x} /\ \exists x. (Simplified_f' /\ E_x /\ D_x /\ I_x))
        // x cannot be eliminated cheaply. Hence convert the problem back to
        // \exists (VarSetToElim). (Simplified_f' /\ C_{\neg x} /\ E_x /\ D_x /\ I_x))
        // i.e. set Simplified_f  <------ Simplified_f', don't change Simplified_C_X, and
        // let's put x in Simplified_VarSetToElim (x is not eliminated) 
          
        //cout<<"\nDAGResolveApplicable(LMESet, LMDSet, WidthTable)\n";
          
        gettimeofday (&start_ms2, NULL);  
          
        DAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(LMESet, LMDSet, VarToElim, WidthTable);
        
        gettimeofday (&finish_ms2, NULL);
        duration_ms2 = finish_ms2.tv_sec * 1000 + finish_ms2.tv_usec / 1000;
        duration_ms2 -= start_ms2.tv_sec * 1000 + start_ms2.tv_usec / 1000;
        cout<<"\nTime spent in selectEquationWithLeastCoefficientForVarToElimAdvanced = "<<duration_ms2<<" milliseconds\n";
                  
        if(e1 != NULL)
                    {
                      //cout<<"\nEquation selected is "<<getDAGStr(e1)<<endl;

                      //cout<<"\nOriginal DD before DAGResolve = "<<getDAGStr(createDAGFromDD(Simplified_f))<<endl;

                      //cout<<"\nSize of original dd = "<<getSizeOfDD(Simplified_f)<<endl;
                      
                      
                      //Check if DAGResolve(Simplified_f, e1, VarToElim) is already done
                      DDNode* Existing = getEntryFromPerVariableHT(Simplified_f, e1, VarToElim, PtrPerVariableHT);
                              
                      if(Existing != NULL)        
                        {
                        //yes, get the hashed value
                        //cout<<"\nHit in per-variable hash table"<<endl; 
                        
                        Simplified_f = Existing;  
                        }
                      else
                        {
                        //cout<<"\nMiss in per-variable hash table"<<endl;   
                          
                        TableForDagResolve.clear();  
                        
                        gettimeofday (&start_ms2, NULL);  
                          
                        DDNode* ResultOfDAGResolve = DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(Simplified_f, e1, VarToElim, WidthTable);
                        
                        gettimeofday (&finish_ms2, NULL);
                        duration_ms2 = finish_ms2.tv_sec * 1000 + finish_ms2.tv_usec / 1000;
                        duration_ms2 -= start_ms2.tv_sec * 1000 + start_ms2.tv_usec / 1000;
                        cout<<"\nTime spent in DAGResolve = "<<duration_ms2<<" milliseconds\n";
                        
                        // Insert DAGResolve(Simplified_f, e1, VarToElim) --->  ResultOfDAGresolve
                        insertEntryIntoPerVariableHT(Simplified_f, e1, VarToElim, ResultOfDAGResolve, PtrPerVariableHT);
                        
                        Simplified_f = ResultOfDAGResolve;
                        }                                                   

                      gettimeofday (&start_ms2, NULL);  
                      
                      if(DDFreeOfVar_WithDP(Simplified_f, VarToElim))
                        {
                          //cout<<"\nSimplified_f made free of "<<VarToElim<<" by DAGResolve"<<endl; 
                          
                          DDNode *temp; //temp \equiv  \exists x. (E_x /\ D_x /\ I_x)
        
                          if(Bound_Constraints.empty())
                            {
                              //cout<<"\nContext empty. hence temp=1\n";
                              temp = createDD(1);
                            }                          
                        else if(ThUnsat(Bound_Constraints, WidthTable))
                            {
                              //cout<<"\nContext inconsistent. \exists x. (E_x /\ D_x /\ I_x) is false\n";
                              temp = createDD(0);
                            }
                        else //Let's compute \exists x. (E_x /\ D_x /\ I_x)
                            {
                              //cout<<"\nContext consistent. Project_ReturningLMDDForMultipleVariables invoked\n";
                              // Note that the problem instance here is \exists x. (E_x /\ D_x /\ I_x)
                              // We have a function callProjectReturningLMDD to compute this
                              // But let us call callProjectReturningLMDDForMultipleVariables as this is the function,
                              // which is called at the other places when the argument UseMonniauxEnabledAllVariableStrategy is true
                              set<string> TempVarSetToElim;
                              TempVarSetToElim.insert(VarToElim);
                              temp = callProjectReturningLMDDForMultipleVariables(Bound_Constraints, TempVarSetToElim, WidthTable, PtrTableForAllVarsQE, PtrTableForAPResolveModified, project_depth);
                             }
                          
                            // We have temp \equiv  \exists x. (E_x /\ D_x /\ I_x) here
                            // Hence let's set 
                            // Simplified_f  <------ Simplified_f /\  \exists x. (E_x /\ D_x /\ I_x)
                            Simplified_f = apply("and", Simplified_f, temp);
                            // Let's set Simplified_C_X <--------- C_{\neg x}
                            Simplified_C_X = Free_Constraints;
                            }// if(DDFreeOfVar_WithDP(Simplified_f, VarToElim))                      
                      else//Simplified_f is not free of VarToElim
                            {
                            // Let's insert x in Simplified_VarSetToElim
                            // We could simplify Simplified_f although we could not make it free of VarToElim
                            //cout<<"\nSimplified_f could not be made free of "<<VarToElim<<" by DAGResolve"<<endl; 
                          
                            Simplified_VarSetToElim.insert(VarToElim);
                            }
                      
                    gettimeofday (&finish_ms2, NULL);
                    duration_ms2 = finish_ms2.tv_sec * 1000 + finish_ms2.tv_usec / 1000;
                    duration_ms2 -= start_ms2.tv_sec * 1000 + start_ms2.tv_usec / 1000;
                    cout<<"\nTime spent in final elimination = "<<duration_ms2<<" milliseconds\n";  
                      
                    }//if(e1 != NULL)                    
            else//e1 = NULL
                    {
                    // Let's insert x in Simplified_VarSetToElim
                    // We could not simplify Simplified_f through DAGResolve
                    //cout<<"\nCould not find proper LME containing "<<VarToElim<<" by DAGResolve"<<endl;             
            
                    Simplified_VarSetToElim.insert(VarToElim);
                    }
                }// if(DAGResolveApplicable(E, D, WidthTable))               
      else //DAGResolve is not applicable
            {
            // Let's insert x in Simplified_VarSetToElim
            // We could not simplify Simplified_f through DAGResolve
            //cout<<"\nDAGResolve not applicable in elimination of "<<VarToElim<<endl;   
          
            Simplified_VarSetToElim.insert(VarToElim);
             }
      
        gettimeofday (&finish_ms, NULL);
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        cout<<"\nTime spent in resolving = "<<duration_ms<<" milliseconds\n";      
        }//loop ends here    
}//function ends here
           

// Find \exists VarSetToElim. (/\ Constraints), convert the result to LMDD, store the result in hash table, and return the result
DDNode* DDManager::callProjectReturningLMDDForMultipleVariables(set<DAGNode*> &Constraints, set<string> &VarSetToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForAllVarsQE, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
bool temporary_implemenation_using_project = true;

if(temporary_implemenation_using_project)
    {
    DDNode* Existing  = getEntryFromTableForAPResolveModified_WithLMISupport_ForMultipleVariables(VarSetToElim, Constraints, PtrTableForAPResolveModified);

    if(Existing != NULL)
        {
        //cout<<"\nAPResolve's hash table hit\n";
        return Existing;
        }
    
    DAGNode* ResultDag = callProject(Constraints, VarSetToElim, WidthTable, project_depth);
    //cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";      
    DDNode* ResultDD = createDDFromDAGWithoutImplicationCheck(ResultDag, WidthTable);
    
    insertEntryIntoTableForAPResolveModified_WithLMISupport_ForMultipleVariables(VarSetToElim, Constraints, ResultDD, PtrTableForAPResolveModified);
    
    return ResultDD;
    }
else
    {
    cout<<"\nIn DDManager::callProjectReturningLMDDForMultipleVariables, implementation is not done\n";
    exit(1);
    }
}

// checks if \exists VarSetToElim. (/\ Constraints) is already encountered
// return the result if yes; return NULL otherwise
DDNode* DDManager::getEntryFromTableForAPResolveModified_WithLMISupport_ForMultipleVariables(set<string> &VarSetToElim, set<DAGNode*> &Constraints, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key

  string key = "";
  string temp_key;
  char temp_char[10];
  
  
  for(set<string>::iterator it = VarSetToElim.begin(); it != VarSetToElim.end(); it++)
    {
      string VarToElim = *it;
      key += VarToElim;
    }
  

  for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created

  HTStatusValue<DDNode*> result = PtrTableForAPResolveModified->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// Enter  \exists VarSetToElim. (/\ Constraints) ---> result into HashTable
void DDManager::insertEntryIntoTableForAPResolveModified_WithLMISupport_ForMultipleVariables(set<string> &VarSetToElim, set<DAGNode*> &Constraints, DDNode* value, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key
  
  string key = "";
  string temp_key;
  char temp_char[10];
  
  
  for(set<string>::iterator it = VarSetToElim.begin(); it != VarSetToElim.end(); it++)
    {
      string VarToElim = *it;
      key += VarToElim;
    }
  

  for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    } 

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForAPResolveModified->hashtable_search(key);


  if(result.success())
    {
      // Do nothing
    }
  else
  {
    result = PtrTableForAPResolveModified->hashtable_insert(key, value);
   
    if(!result.success())
        {
        cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForAPResolveModified_WithLMISupport_ForMultipleVariables\n";
        exit(1);
        }
    }//else
}


//Function to get entry from the per variable hash table
DDNode* DDManager::getEntryFromPerVariableHT(DDNode* dd, DAGNode* LME, string VarToElim, HashTable <string, DDNode*> *PtrPerVariableHT)
{
    // Creating the hash key

  string key = "";
  string temp_key;
  char temp_char[10];
  
  key += VarToElim;
  
  sprintf(temp_char , "%x", dd);
  temp_key = temp_char;
  key += temp_key;
  
  sprintf(temp_char , "%x", LME);
  temp_key = temp_char;
  key += temp_key;

  // Hash key created

  HTStatusValue<DDNode*> result = PtrPerVariableHT->hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}


//Function to insert entry into the per variable hash table
DDNode* DDManager::insertEntryIntoPerVariableHT(DDNode* dd, DAGNode* LME, string VarToElim, DDNode* value, HashTable <string, DDNode*> *PtrPerVariableHT)
{
    // Creating the hash key

  string key = "";
  string temp_key;
  char temp_char[10];
  
  key += VarToElim;
  
  sprintf(temp_char , "%x", dd);
  temp_key = temp_char;
  key += temp_key;
  
  sprintf(temp_char , "%x", LME);
  temp_key = temp_char;
  key += temp_key;

  // Hash key created

  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrPerVariableHT->hashtable_search(key);


  if(result.success())
    {
      // Do nothing
    }
  else
  {
    result = PtrPerVariableHT->hashtable_insert(key, value);
   
    if(!result.success())
        {
        cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoPerVariableHT\n";
        exit(1);
        }
    }//else
}


// Function which performs \exists VarSetToElim. (f) by first
// conversion of \exists VarSetToElim.(f) into 
// \exists VarSetToElim. (f_1 /\ C_1) \/ .... \/ \exists VarSetToElim. (f_n /\ C_n)
// and solving each of them using appropriate QE strategy
DDNode* DDManager::conversionToDisjunctionOfTailedTrianglesFollowedByQE(set<string> &VarSetToElim, DDNode* f, map<string, int> &WidthTable)
{
    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nInside DDManager::conversionToDisjunctionOfTailedTrianglesFollowedByQE\n";
        }
    
    if(PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles)
        {
        time_t mon_start_time, mon_end_time;
        
        time(&mon_start_time);
        TimeOutStart_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = mon_start_time;                
        int project_depth = 0;  
        PartialResult_In_LMDDWithMonniauxTypeSimplifications = callMonniaux_Quantifier_Eliminator_WithSupportForLMIs(original_dag_in_path_based_strategy_in_lmdd_monniaux, VarSetToElim, WidthTable, project_depth);
        time(&mon_end_time);
        
        //cout<<"\nPartialResult = "<<getDAGStr(PartialResult_In_LMDDWithMonniauxTypeSimplifications)<<endl;
        
        TimeSpentInMonniaux_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles = mon_end_time - mon_start_time;
        
        if(!TimedOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles)//No timeout. Hence result is in \pi
                {
                return createDD(0);
                }
        }
    
    vector< pair< set<DAGNode*>, DDNode* > > SimplifiedInstances;  
    
    set<DAGNode*> Context;
    
    
    if(LMEBasedSimplificationAsPreprocessor) //LME based simplification is already done
    // Conversion into tailed triangles must keep this in mind
    {
      bool ConversionToSimplifiedInstancesUsingSMTSolver = true;
      
      if(ConversionToSimplifiedInstancesUsingSMTSolver)
          {
           //Let us give f to an SMT solver
          
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nLet us give f to an SMT solver\n";
                }
          
           map<string, string> Model_Of_SatCondition;

           bool ResultIsFalse = getModelGeneral_WithSupportForLMIs(createDAGFromDD(f), Model_Of_SatCondition, WidthTable);

           if(ResultIsFalse)   
            {
             if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nResult is unsat\n";
                }   
            return createDD(0);   
            }
           else
            {
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nResult is sat\nModel_Of_SatCondition\n";
                showModel(Model_Of_SatCondition);
                }      
            convertToSimplifiedInstancesUsingSMTSolver(f, Context, SimplifiedInstances, Model_Of_SatCondition, WidthTable);          
            }
          }
      else
          {
           cout<<"\nUnsupported option in DDManager::conversionToDisjunctionOfTailedTrianglesFollowedByQE\n";
           exit(1);
          }
    }//if(LMEBasedSimplificationAsPreprocessor)
    else if(EagerConversionToDisjunctions)
    {
       if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nCalling convertToSimplifiedInstancesEager\n";
                }    
        
      convertToSimplifiedInstancesEager(f, SimplifiedInstances);
    }//else if(EagerConversionToDisjunctions)
    else
    {
     convertToSimplifiedInstances(f, Context, SimplifiedInstances);
    }
    
    NumberOfTailedTriangles = SimplifiedInstances.size();
    
    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        showSimplifiedInstances(SimplifiedInstances);
        }
    
    DDNode* result;
    
    if(DemandBasedConversionToDisjunctions)
        {
        result = QEFromDisjunctionOfTailedTrianglesWithDemandBasedDisjunctionCreation(SimplifiedInstances, VarSetToElim, WidthTable);
        }
    else if(CollectConsecutiveSatTailedTriangles_In_ConversionToDisjunctionOfTailedTriangles)
        {
         result = QEFromDisjunctionOfTailedTriangles_CollectConsecutiveSatTailedTriangles(SimplifiedInstances, VarSetToElim, WidthTable);
        }
    else
        {
        result = QEFromDisjunctionOfTailedTriangles(SimplifiedInstances, VarSetToElim, WidthTable);
        }
    
    return result;
}


// Version of obtainPathCounts which uses an SMT solver
string DDManager::obtainPathCountsUsingSMTSolver(DDNode* f, map<string, string> &Model_Of_SatCondition, map<string, int> &WidthTable)
{
    DAGNode* C = f->getPredicate();
    if(C==NULL)//terminal
    {
        if(f->getVal() == 1)//1-terminal
        {
          return "none";  
        }
        else
        {
            cout<<"\n0-node encountered following a satisfiable path in DDManager::obtainPathCountsUsingSMTSolver\n";
            exit(1);
        }
    }
    else//non-terminal
    {
        if(checkEntailment_WithSupportForSelsConcats_WithLMISupport(C, WidthTable, Model_Of_SatCondition))
        {
            return "right";
        }
        else
        {
            return "left";
        }
    }
}



// Function to convert \exists VarSetToElim.(f) into 
// \exists VarSetToElim. (f_1 /\ C_1) \/ .... \/ \exists VarSetToElim. (f_n /\ C_n)
// by following a satisfiable path
void DDManager::convertToSimplifiedInstancesUsingSMTSolver(DDNode* f, set<DAGNode*> Context, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, map<string, string> &Model_Of_SatCondition, map<string, int> &WidthTable)
{
    string SideSelected;
    
    SideSelected = obtainPathCountsUsingSMTSolver(f, Model_Of_SatCondition, WidthTable);
    
    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nSideSelected = "<<SideSelected<<endl;
                }   
    
    if(SideSelected == "none")//1-terminal reached
        {
          SimplifiedInstances.push_back(make_pair(Context, f));          
        }    
    else if(SideSelected == "right")//Let us follow the right branch and consider the sub-problem at left branch as a simplifiedInstance
        {
        set<DAGNode*> RightContext;
        set<DAGNode*> LeftContext;
        
        DAGNode* C = f->getPredicate();
        
        copySetNew(RightContext, Context);
        RightContext.insert(C);
            
        copySetNew(LeftContext, Context);
        LeftContext.insert(negateConstraint_With_LMISupport(C));    
        
        //\exists X. (f->getLeft() /\ LeftContext) is a simplifiedInstance
         if(!isFalseDD(f->getLeft()))
            {
            SimplifiedInstances.push_back(make_pair(LeftContext, f->getLeft()));
            }
        
        convertToSimplifiedInstancesUsingSMTSolver(f->getRight(), RightContext, SimplifiedInstances, Model_Of_SatCondition, WidthTable);
        }
    else //Let us follow the left branch and consider the sub-problem at right branch as a simplifiedInstance
        {
        set<DAGNode*> RightContext;
        set<DAGNode*> LeftContext;
        
        DAGNode* C = f->getPredicate();
        
        copySetNew(RightContext, Context);
        RightContext.insert(C);
            
        copySetNew(LeftContext, Context);
        LeftContext.insert(negateConstraint_With_LMISupport(C));    
        
        //\exists X. (f->getRight() /\ RightContext) is a simplifiedInstance
        
         if(!isFalseDD(f->getRight()))
            {
            SimplifiedInstances.push_back(make_pair(RightContext, f->getRight()));
            }
        
        convertToSimplifiedInstancesUsingSMTSolver(f->getLeft(), LeftContext, SimplifiedInstances, Model_Of_SatCondition, WidthTable);
        }
}



// Function to convert \exists VarSetToElim.(f) into 
// \exists VarSetToElim. (f_1 /\ C_1) \/ .... \/ \exists VarSetToElim. (f_n /\ C_n)
void DDManager::convertToSimplifiedInstances(DDNode* f, set<DAGNode*> Context, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances)
{
    int LeftPathCount, RightPathCount;
    
    obtainPathCounts(f, LeftPathCount, RightPathCount);
    
    if(LeftPathCount == 0 && RightPathCount == 0)//No path containing proper LMEs with variables to be eliminated
    // in the left or right branch of f
        {
        //\exists X. (f /\ Context) is a simplifiedInstance
        
        if(!isFalseDD(f))
            {
            SimplifiedInstances.push_back(make_pair(Context, f));
            }
        }
    else if(RightPathCount >= LeftPathCount)//Right branch of f has more paths containing proper LMEs with variables to be eliminated
        // compared to the left branch. Hence, let us follow the right branch and consider the sub-problem at left branch as a simplifiedInstance
        {
        set<DAGNode*> RightContext;
        set<DAGNode*> LeftContext;
        
        DAGNode* C = f->getPredicate();
        
        copySetNew(RightContext, Context);
        RightContext.insert(C);
            
        copySetNew(LeftContext, Context);
        LeftContext.insert(negateConstraint_With_LMISupport(C));    
        
        //\exists X. (f->getLeft() /\ LeftContext) is a simplifiedInstance
         if(!isFalseDD(f->getLeft()))
            {
            SimplifiedInstances.push_back(make_pair(LeftContext, f->getLeft()));
            }
        
        convertToSimplifiedInstances(f->getRight(), RightContext, SimplifiedInstances);
        }
    else //Left branch of f has more paths containing proper LMEs with variables to be eliminated
        // compared to the right branch. Hence, let us follow the left branch and consider the sub-problem at right branch as a simplifiedInstance
        {
        set<DAGNode*> RightContext;
        set<DAGNode*> LeftContext;
        
        DAGNode* C = f->getPredicate();
        
        copySetNew(RightContext, Context);
        RightContext.insert(C);
            
        copySetNew(LeftContext, Context);
        LeftContext.insert(negateConstraint_With_LMISupport(C));    
        
        //\exists X. (f->getRight() /\ RightContext) is a simplifiedInstance
        
         if(!isFalseDD(f->getRight()))
            {
            SimplifiedInstances.push_back(make_pair(RightContext, f->getRight()));
            }
        
        convertToSimplifiedInstances(f->getLeft(), LeftContext, SimplifiedInstances);
        }
}



// Obtain the # of paths containing proper LMEs with variables to be eliminated through the right branch in RightPathCount and
// the # of paths containing proper LMEs with variables to be eliminated through the left branch in LeftPathCount
void DDManager::obtainPathCounts(DDNode* f, int &LeftPathCount, int &RightPathCount)
{
   bool temporary_implementation = false;
   
   if(temporary_implementation)
     {
        if(f->getPredicate() == NULL)//f is terminal
        {
           LeftPathCount = 0;
           RightPathCount = 0; 
        }
        else
        {
           LeftPathCount = 0;
           RightPathCount = 1;  
        }
   }
   else
   {
   map<DDNode*, pair<int, int> >::iterator pit = PathCountsOfDDNodes.find(f);
         
   if(pit == PathCountsOfDDNodes.end())//No entry for f in PathCountsOfDDNodes
            {
            cout<<"\nNo entry for "<<f<<" in PathCountsOfDDNodes in DDManager::obtainPathCounts\n";
            exit(1);
            }
   
   RightPathCount = (pit->second).first;
   LeftPathCount = (pit->second).second;
   
   if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nf = "<<getDAGStr(createDAGFromDD(f))<<endl;
        cout<<"\nLeftPathCount = "<<LeftPathCount<<endl;
        cout<<"\nRightPathCount = "<<RightPathCount<<endl;
        }
   }
}



// Show the simplified instances
void DDManager::showSimplifiedInstances(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances)
{
    cout<<"\nSimplifiedInstances\n******************\n";
    
    for(int i=0; i < SimplifiedInstances.size(); i++)
    {
        set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;
        DDNode* Myf = (SimplifiedInstances[i]).second;
        
        showAPSet("Context", MyContext);
        cout<<"\nf = "<<getDAGStr(createDAGFromDD(Myf))<<endl;
    }
}


// Take each \exists X. (C_i /\ f_i) from "SimplifiedInstances", compute it, and disjunct it to PartialResult_In_LMDDWithMonniauxTypeSimplifications
DDNode* DDManager::QEFromDisjunctionOfTailedTriangles(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, set<string> &VarSetToElim, map<string, int> &WidthTable)
{
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;   
    
 TableForDagResolve.clear();

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;
  
  int project_depth = 0;

  for(int i = SimplifiedInstances.size()-1; i >= 0; i--)//Let's do them in reverse order
  // Simpler instances are towards the end of vector
    {
        set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;
        
        DDNode* MyLMDD = (SimplifiedInstances[i]).second;
        
        SizeOfTrianglesInEagerConversionToDisjunctions.push_back(getSizeOfDD(MyLMDD));
        
        //Let's see if \exists VarSetToElim. (MyLMDD /\ MyContext) is needed or not
        
        // PathCondition = MyLMDD /\ MyContext
        DAGNode* PathCondition = createDAG("and", createDAGFromDD(MyLMDD), getConjunctionFromSet(MyContext), LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
            }
   
        DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
            }
  
        DAGNode*  SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
        
        map<string, string> ModelOfSatisfiabilityCondition;
        
        gettimeofday (&start_ms, NULL);
        
        bool SatisfiabilityConditionIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, ModelOfSatisfiabilityCondition, WidthTable);
        
        gettimeofday (&finish_ms, NULL);
        
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        TotalSMTTimeInConversionToDisjunctionOfTailedTriangles = TotalSMTTimeInConversionToDisjunctionOfTailedTriangles + duration_ms;
            
        if(SatisfiabilityConditionIsFalse)
            {
            cout<<"\nexists X. (C_i and f_i) => Pi\n";
            
            SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("u");
            }
        else
            {
            cout<<"\nNOT(exists X. (C_i and f_i) => Pi)\n";
            
            SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("s");
            
            NumberOfTailedTrianglesToQE++;
            
            DAGNode* Pi_i;
            
            
            set<DAGNode*> FreeContext;
            set<DAGNode*> BoundContext;
            
            getFreeAndBoundConstraintsForMultipleVariables(MyContext, VarSetToElim, FreeContext, BoundContext, WidthTable);
            
            DAGNode* ConjunctionOfFreeContext = getConjunctionFromSet(FreeContext);

            
            gettimeofday (&start_ms, NULL);
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                 showAPSet("FreeContext", FreeContext);
                 showAPSet("BoundContext", BoundContext);
                 cout<<"\nMyLMDD = "<<getDAGStr(createDAGFromDD(MyLMDD))<<endl;   
                }  
            
            if(LMEBasedSimplificationAsPreprocessor)//LME based simplification is already done
            {
                Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs(MyLMDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
             }
            else
            {           
                Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(MyLMDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            }
            
            gettimeofday (&finish_ms, NULL);
        
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
            TotalQETimeInConversionToDisjunctionOfTailedTriangles = TotalQETimeInConversionToDisjunctionOfTailedTriangles + duration_ms;
            
            Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfFreeContext, LOGICAL_SYMBOL, 1));
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                }                   
                  
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            }
    }//for ends here  
  
  DDNode* result = createDD(0);// result is actually in PartialResult_In_LMDDWithMonniauxTypeSimplifications
  // The DD part of the result is always false
  
  return result;
}


// Returns true if f is terminal false; true otherwise
bool DDManager::isFalseDD(DDNode* f)
{
    if(f->getPredicate() != NULL)
     {
     return false;
     }
    else
    {
     if(f->getVal() == 0)   
     {
         return true;
     }
     else
     {
         return false;
     }
    }
    
}


//Function to get entry from the global per variable hash table PerVariableHTForDagResolve_WithLMISupport
DDNode* DDManager::getEntryFromPerVariableHTForDagResolve_WithLMISupport(DDNode* dd, DAGNode* LME, string VarToElim)
{
    // Creating the hash key

  string key = "";
  string temp_key;
  char temp_char[10];
  
  key += VarToElim;
  
  sprintf(temp_char , "%x", dd);
  temp_key = temp_char;
  key += temp_key;
  
  sprintf(temp_char , "%x", LME);
  temp_key = temp_char;
  key += temp_key;

  // Hash key created

  HTStatusValue<DDNode*> result = PerVariableHTForDagResolve_WithLMISupport.hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}


//Function to insert entry into the per variable global hash table PerVariableHTForDagResolve_WithLMISupport
DDNode* DDManager::insertEntryIntoPerVariableHTForDagResolve_WithLMISupport(DDNode* dd, DAGNode* LME, string VarToElim, DDNode* value)
{
    // Creating the hash key

  string key = "";
  string temp_key;
  char temp_char[10];
  
  key += VarToElim;
  
  sprintf(temp_char , "%x", dd);
  temp_key = temp_char;
  key += temp_key;
  
  sprintf(temp_char , "%x", LME);
  temp_key = temp_char;
  key += temp_key;

  // Hash key created

  // Check if entry already exists

  HTStatusValue<DDNode*> result = PerVariableHTForDagResolve_WithLMISupport.hashtable_search(key);


  if(result.success())
    {
      // Do nothing
    }
  else
  {
    result = PerVariableHTForDagResolve_WithLMISupport.hashtable_insert(key, value);
   
    if(!result.success())
        {
        cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoPerVariableHTForDagResolve_WithLMISupport\n";
        exit(1);
        }
    }//else
}





// Function which performs \exists VarSetToElim. (f) by 
// conversion of \exists VarSetToElim.(f) into 
// \exists VarSetToElim. (f_1 /\ C_1) \/ .... \/ \exists VarSetToElim. (f_n /\ C_n)
// and solving each of them using appropriate QE strategy
// with the following two additional features
// 1) LME based simplification is applied as and when the LME is encountered and
// 2) No explicit list of tailed triangles is created.
DDNode* DDManager::call_conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(set<string> &VarSetToElim, DDNode* f, map<string, int> &WidthTable)
{
  set<DAGNode*> C_X, Neg_C_X;
  C_X.clear(); Neg_C_X.clear();
  
  
  TableForDagResolve.clear();

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;
  
  int project_depth = 0; 
  
  
  if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
    {
    cout<<"\nCalling conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE\n";    
    }
  
   DDNode* result;
  
   conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(VarSetToElim, f, C_X, Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
   
   result = createDD(0);//returned result is not important
 
   if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
    {
    cout<<"\nSize of result = "<<getSizeOfDD(result)<<endl;
    }

   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function DDManager::call_conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE has timed out. Its result is not exact\n";
       timed_out = true; // timed_out flag set
       return result;
     }


  return result;    
}




// The function called by call_conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE
void DDManager::conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(set<string> VarSetToElim, DDNode* f, set<DAGNode*> C_X, set<DAGNode*> Neg_C_X, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForAPResolveModified, HashTable <string, DDNode*> *PtrTableForQElimModified, int project_depth)
{
struct timeval start_ms, finish_ms;
unsigned long long int duration_ms;
  // check if we have already timed out   
 if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return;
	}  
 
 
 if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
    {
    cout<<"\nCalling DDManager::conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE with parameters\n";
    showSet(VarSetToElim, "VarSetToElim");
    cout<<"\nf = "<<getDAGStr(createDAGFromDD(f))<<endl;
    showAPSet("C_X", C_X);
    showAPSet("Neg_C_X", Neg_C_X);
    }


NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplification++;


 if(isFalseDD(f)) /* \exists X.(f \wedge C_{X}) \equiv false. Hence no need to change the PartialResult */
    {
    int C_X_Size = C_X.size();
    int Neg_C_X_Size = Neg_C_X.size();
    
    TotalCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification = TotalCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification + C_X_Size;
    TotalNegCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification = TotalNegCXSizeInFalsecasesInConProbDisjTailTriaWithMESimplification + Neg_C_X_Size;
    
    fprintf(LMIQEOutputDataFp, "\nFalse encountered with C_X_Size = %d and Neg_C_X_Size = %d\n", C_X_Size, Neg_C_X_Size);     
     
    NoOfCallsToRecursiveAlgoInConProbDisjTailTriaWithMESimplificationFalse++; 
     
    return; 
    }
  
 /* Simplify \exists X.(f \wedge C_{X}) into \exists X".(f" \wedge C_{X"}) using LMEs and already computed results */
 
 set<string> Simplified_VarSetToElim;
 DDNode* Simplified_f;
 set<DAGNode*> Simplified_C_X;
 
 NoOfCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries++;
  
 getSimplifiedInstanceUsingLMEsAndHashTableEntries(f, C_X, VarSetToElim, Simplified_f, Simplified_C_X, Simplified_VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
       
 
 /* Note that the problem is now \exists Simplified_VarSetToElim.(Simplified_f \wedge Simplified_C_X) */
  
 /* Find the direction in which we need to traverse the LMDD f. Left/right/none 
depending on whether we decide to traverse down left/right first or solve the problem 
at this level */
 
 string direction = selectEdgeDependingOnLMEAvailability(Simplified_f);
 
 if(direction == "none")/* Let us not traverse further down. Compute the problem here itself. */
 {
       int Simplified_C_X_Size = Simplified_C_X.size();
       int Neg_C_X_Size = Neg_C_X.size();
       int Simplified_f_Size = getSizeOfDD(Simplified_f);
       
       TotalDDSizeInNonecasesInConProbDisjTailTriaWithMESimplification = TotalDDSizeInNonecasesInConProbDisjTailTriaWithMESimplification + Simplified_f_Size;
       TotalCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification = TotalCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification + Simplified_C_X_Size;
       TotalNegCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification = TotalNegCXSizeInNonecasesInConProbDisjTailTriaWithMESimplification + Neg_C_X_Size;
    
      fprintf(LMIQEOutputDataFp, "\nDD with size = %d encountered with Simplified_C_X_Size = %d and Neg_C_X_Size = %d\n", Simplified_f_Size, Simplified_C_X_Size, Neg_C_X_Size);  
     
     
       NonecasesInConProbDisjTailTriaWithMESimplification++;
       
     
       if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
       {
           cout<<"\ndirection = none\n";
       }
       
       
       if(SMTCallNeededInConversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_f))
       {       
      //Let's see if  \exists Simplified_VarSetToElim.(Simplified_f \wedge Simplified_C_X) is needed or not        
     
       // check if Simplified_f \wedge Simplified_C_X \wedge Neg_C_X \wedge \neg PartialResult is sat
     
       // PathCondition = Simplified_f \wedge Simplified_C_X \wedge Neg_C_X
       DAGNode* PathCondition = createDAG("and", createDAGFromDD(Simplified_f), getConjunctionFromSet(Simplified_C_X), LOGICAL_SYMBOL, 1);
      
       PathCondition = createDAG("and", PathCondition, getConjunctionFromSet(Neg_C_X), LOGICAL_SYMBOL, 1);
        
       if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
            }
   
        DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
            }
  
        DAGNode*  SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
        
        map<string, string> ModelOfSatisfiabilityCondition;
        
        gettimeofday (&start_ms, NULL);
        
        
        SATCallsInConProbDisjTailTriaWithMESimplification++;
        
        
        bool SatisfiabilityConditionIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, ModelOfSatisfiabilityCondition, WidthTable);
        
        gettimeofday (&finish_ms, NULL);
        
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        
        TimeInSATCallsInConProbDisjTailTriaWithMESimplification = TimeInSATCallsInConProbDisjTailTriaWithMESimplification + duration_ms;
              
            
        if(SatisfiabilityConditionIsFalse)
            {
            SATCallsInConProbDisjTailTriaWithMESimplificationWhichAreUnsat++;
            
            
            cout<<"\nexists X. (Simplified_f AND Simplified_C_X AND Neg_C_X) => Pi\n";
            
            bool additional_satcheck_to_exit = false;
            
            if(additional_satcheck_to_exit)
                {
                DAGNode*  SatisfiabilityConditionToExit = createDAG("and", original_dag_in_path_based_strategy_in_lmdd_monniaux, NegPartialResult, LOGICAL_SYMBOL, 1);
        
                map<string, string> ModelOfSatisfiabilityConditionToExit;
                
                if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nSatisfiabilityConditionToExit = "<<getDAGStr(SatisfiabilityConditionToExit)<<endl;
                    }
        
                gettimeofday (&start_ms, NULL);
        
                bool SatisfiabilityConditionToExitIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityConditionToExit, ModelOfSatisfiabilityConditionToExit, WidthTable);
        
                gettimeofday (&finish_ms, NULL);
        
                duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
                duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
                
                if(SatisfiabilityConditionToExitIsFalse)
                    {
                    cout<<"\nexists X. (original formula) => Pi\n";
                    exit(1);
                    }//if(SatisfiabilityConditionToExitIsFalse)
                else
                    {
                    cout<<"\nNOT(exists X. (original formula) => Pi)\n";
                    }//not of if(SatisfiabilityConditionToExitIsFalse)              
                }//if(additional_satcheck_to_exit)           
            }//if(SatisfiabilityConditionIsFalse)
        else
            {
            SATCallsInConProbDisjTailTriaWithMESimplificationWhichAreSat++;
            
            cout<<"\nNOT(exists X. (Simplified_f AND Simplified_C_X AND Neg_C_X) => Pi)\n";
            
            DAGNode* Pi_i;            
            
            set<DAGNode*> FreeContext;
            set<DAGNode*> BoundContext;
            
            getFreeAndBoundConstraintsForMultipleVariables(Simplified_C_X, Simplified_VarSetToElim, FreeContext, BoundContext, WidthTable);
            
            DAGNode* ConjunctionOfFreeContext = getConjunctionFromSet(FreeContext);
            
            gettimeofday (&start_ms, NULL);
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                 showAPSet("FreeContext", FreeContext);
                 showAPSet("BoundContext", BoundContext);
                 cout<<"\nSimplified_f = "<<getDAGStr(createDAGFromDD(Simplified_f))<<endl;   
                }  
            
            QE_LMDDCallsInConProbDisjTailTriaWithMESimplification++;
            
            Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs(Simplified_f, BoundContext, Simplified_VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            
            gettimeofday (&finish_ms, NULL);
        
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;   
            
            TimeInQE_LMDDCallsInConProbDisjTailTriaWithMESimplification = TimeInQE_LMDDCallsInConProbDisjTailTriaWithMESimplification + duration_ms;
            
            Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfFreeContext, LOGICAL_SYMBOL, 1));
            
            DAGNode* ConjunctionOfNeg_C_X = getConjunctionFromSet(Neg_C_X);
            
            Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfNeg_C_X, LOGICAL_SYMBOL, 1));
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                }                   
                  
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            }//if Simplified_f \wedge Simplified_C_X \wedge Neg_C_X \wedge \neg PartialResult is sat
       }//if(SMTCallNeeded...)
       else //Let's any way compute the problem using QE_LMDD
       {
         cout<<"\nDon't know if exists X. (Simplified_f AND Simplified_C_X AND Neg_C_X) => Pi\n";
            
        DAGNode* Pi_i;            

        set<DAGNode*> FreeContext;
        set<DAGNode*> BoundContext;

        getFreeAndBoundConstraintsForMultipleVariables(Simplified_C_X, Simplified_VarSetToElim, FreeContext, BoundContext, WidthTable);

        DAGNode* ConjunctionOfFreeContext = getConjunctionFromSet(FreeContext);

        gettimeofday (&start_ms, NULL);

        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
             showAPSet("FreeContext", FreeContext);
             showAPSet("BoundContext", BoundContext);
             cout<<"\nSimplified_f = "<<getDAGStr(createDAGFromDD(Simplified_f))<<endl;   
            }  

        QE_LMDDCallsInConProbDisjTailTriaWithMESimplification++;

        Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs(Simplified_f, BoundContext, Simplified_VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);

        gettimeofday (&finish_ms, NULL);

        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;   

        TimeInQE_LMDDCallsInConProbDisjTailTriaWithMESimplification = TimeInQE_LMDDCallsInConProbDisjTailTriaWithMESimplification + duration_ms;

        Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfFreeContext, LOGICAL_SYMBOL, 1));

        DAGNode* ConjunctionOfNeg_C_X = getConjunctionFromSet(Neg_C_X);

        Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfNeg_C_X, LOGICAL_SYMBOL, 1));

        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
            }                   

        PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
       }//Let's any way compute the problem using QE_LMDD
   }//if(direction == "none")
 else // i.e. direction is "left" or "right"
 {       
   // Traverse down
  DAGNode* C = Simplified_f->getPredicate();
  //cout<<"\nPredicate encountered = "<<getDAGStr(C)<<endl;
  if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\ndirection = "<<direction<<endl;
        cout<<"\nPredicate encountered = "<<getDAGStr(C)<<endl;
        cout<<"\nTraversing down"<<endl;
        }  
  if(direction == "left") /*Let's go to the left child first. Left child is easy to solve compared to
			    right child as it has more LMEs on variables to be eliminated */
        {     
        LeftcasesInConProbDisjTailTriaWithMESimplification++;
      
      
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nGoing to the left child first"<<endl;  
            }
            
        if(freeOfVarsEfficient(C, Simplified_VarSetToElim))//predicate free of all vars to elim
            {
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {                    
                cout<<"\nPredicate free of all variables to be eliminated"<<endl;
                }
            
            set<DAGNode*> Neg_C_X_then, Neg_C_X_else;
            
            copySet(Neg_C_X_then, Neg_C_X);
            Neg_C_X_then.insert(C);

            copySet(Neg_C_X_else, Neg_C_X);
            Neg_C_X_else.insert(negateConstraint_With_LMISupport(C));  
            
            conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getLeft(), Simplified_C_X, Neg_C_X_else, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
            conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getRight(), Simplified_C_X, Neg_C_X_then, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
            }
        else//predicate is not free of all vars to elim
            {
            
           if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {                    
                cout<<"\nPredicate not free of all variables to be eliminated"<<endl;
                }

            set<DAGNode*> Simplified_C_X_then, Simplified_C_X_else;

            copySet(Simplified_C_X_then, Simplified_C_X);
            Simplified_C_X_then.insert(C);

            copySet(Simplified_C_X_else, Simplified_C_X);
            Simplified_C_X_else.insert(negateConstraint_With_LMISupport(C)); 
            
            conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getLeft(), Simplified_C_X_else, Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
            conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getRight(), Simplified_C_X_then, Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
            }//predicate is not free of all vars to elim
    }//direction == left
  else /*Let's go to the right child first. Right child is easy to solve compared to
			    left child as it has more LMEs on variables to be eliminated */
        {   
        
        RightcasesInConProbDisjTailTriaWithMESimplification++;
      
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nGoing to the right child first"<<endl;  
            }
            
        if(freeOfVarsEfficient(C, Simplified_VarSetToElim))//predicate free of all vars to elim
            {
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {                    
                cout<<"\nPredicate free of all variables to be eliminated"<<endl;
                }
            
            set<DAGNode*> Neg_C_X_then, Neg_C_X_else;
            
            copySet(Neg_C_X_then, Neg_C_X);
            Neg_C_X_then.insert(C);

            copySet(Neg_C_X_else, Neg_C_X);
            Neg_C_X_else.insert(negateConstraint_With_LMISupport(C));  
            
             conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getRight(), Simplified_C_X, Neg_C_X_then, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
             conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getLeft(), Simplified_C_X, Neg_C_X_else, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
             }
        else//predicate is not free of all vars to elim
            {
            
           if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {                    
                cout<<"\nPredicate not free of all variables to be eliminated"<<endl;
                }

            set<DAGNode*> Simplified_C_X_then, Simplified_C_X_else;

            copySet(Simplified_C_X_then, Simplified_C_X);
            Simplified_C_X_then.insert(C);

            copySet(Simplified_C_X_else, Simplified_C_X);
            Simplified_C_X_else.insert(negateConstraint_With_LMISupport(C)); 
            
            conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getRight(), Simplified_C_X_then, Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
            conversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(Simplified_VarSetToElim, Simplified_f->getLeft(), Simplified_C_X_else, Neg_C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForQElimModified, project_depth);
            }//predicate is not free of all vars to elim
    }//direction == right
 }//direction is "left" or "right"
}//function ends here


// Function to select between left and right edge depending on LME availability
string DDManager::selectEdgeDependingOnLMEAvailability(DDNode* f)
{
   int LeftPathCount, RightPathCount;
   
   obtainPathCounts(f, LeftPathCount, RightPathCount) ;
   
   if(LeftPathCount == 0 && RightPathCount == 0)//No path containing proper LMEs with variables to be eliminated
    // in the left or right branch of f
   {
       return "none";
   }
   else if(LeftPathCount >= RightPathCount)//Number of paths containing proper LMEs with variables to be eliminated
    // in the left more than those in the right branch
   {
       return "left";
   }
   else//Number of paths containing proper LMEs with variables to be eliminated
    // in the right more than those in the left branch
   {
       return "right";
   }
}




// Version of QE_LMDD_ForMultipleVariables_UnderGivenContext such that there are no LMEs with vars to elim in the context
DAGNode* DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs(DDNode* f, set<DAGNode*> &Context, set<string> &VarSetToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> LMESet;
  set<DAGNode*> LMDSet;
  set<DAGNode*> LMISet;
  
  list<string> VarsToElim;
  list<string> VarsStillToElim;
  copySetToList(VarSetToElim, VarsToElim);
  
  DDNode *result;
  result = f;
  
  int SizeOfMyDD = getSizeOfDD(f);
  
  if(SizeOfMyDD > Max_Inter_LMDD_Size)
	{
	  Max_Inter_LMDD_Size = SizeOfMyDD;
	}
  
  
  fprintf(LMIQEOutputDataFp, "\nSize of dd inside DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs = %d\n", SizeOfMyDD);
  fprintf(LMIQEOutputDataFp, "\nSize of context inside DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs = %d\n", Context.size());
  fprintf(LMIQEOutputDataFp, "\n#vars to elim in DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs = %d\n", VarSetToElim.size());
  
  TotalSizeOfTriangles = TotalSizeOfTriangles + SizeOfMyDD;
  TotalSizeOfTails = TotalSizeOfTails + Context.size();
  TotalVarsToElimFromTailedTriangles = TotalVarsToElimFromTailedTriangles + VarSetToElim.size();
  
  int eliminated_by_firstloop=0;
  int eliminated_by_secondloop=0;
  int eliminated_by_thirdloop=0;
  
  OrderInWhichVariablesAreEliminated.push_back("{");

  VarsStillToElim.clear();
  
  while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}
      
      string VarToElim;
      list<string>::iterator lit = VarsToElim.begin();
      VarToElim = *lit;         
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Context, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      if(DDFreeOfVar_WithDP(result, VarToElim))
        {
          OrderInWhichVariablesAreEliminated.push_back(VarToElim);
          
          if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            { 
            cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
            
            cout<<"\nDDFreeOfVar_WithDP(result, VarToElim)\n";
            }
        
        eliminated_by_secondloop++; 
          
        TableForDagResolve.clear();
        
        result = QElim_WithLMISupport(VarToElim, result, LMESet, LMDSet, LMISet, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
    
        
        SizeOfMyDD = getSizeOfDD(result);
  
        if(SizeOfMyDD > Max_Inter_LMDD_Size)
            {
            Max_Inter_LMDD_Size = SizeOfMyDD;
            }
        
        
        VarsToElim.remove(VarToElim); 
        
        Context = Free_Constraints;
        }   
      else
        {
          if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\n~DDFreeOfVar_WithDP(result, VarToElim)\n";
            }
          
         eliminated_by_thirdloop++;
         
         VarsStillToElim.push_back(VarToElim);
         
         VarsToElim.remove(VarToElim); 
        }
    }//end(while)
  
  
  VarsToElim = VarsStillToElim;
  
  while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}
      
      string VarToElim;
      VarToElim = chooseVar(result, VarsToElim);
      
      OrderInWhichVariablesAreEliminated.push_back(VarToElim);
      
      if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
            }
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Context, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      if(DDFreeOfVar_WithDP(result, VarToElim) && Bound_Constraints.empty())//elimination is not required
        {
          if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
             cout<<"\nDDFreeOfVar_WithDP(result, VarToElim) && Bound_Constraints.empty()\n";
            }
          
         VarsToElim.remove(VarToElim); 
        }
      else//elimination is required
        {
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {  
            cout<<"\n~(DDFreeOfVar_WithDP(result, VarToElim) && Bound_Constraints.empty())\n";
            }
        
        TableForDagResolve.clear();
        
        result = QElim_WithLMISupport(VarToElim, result, LMESet, LMDSet, LMISet, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
    
        SizeOfMyDD = getSizeOfDD(result);
  
        if(SizeOfMyDD > Max_Inter_LMDD_Size)
            {
            Max_Inter_LMDD_Size = SizeOfMyDD;
            }        
        
        VarsToElim.remove(VarToElim); 
        Context = Free_Constraints;
        }      
    }//end(while)
  
  OrderInWhichVariablesAreEliminated.push_back("}");
  
   if(checkTimeOut())
     {
       // Time Out !!!
       cout<<"\nWarning!!!The function DDManager::QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs in DDManager.cpp has timed out. It's result is not exact\n";
       timed_out = true; // timed_out flag set
       return createLogicalConstant("true");
     }
  
  DAGNode* ConjunctionOfContext = getConjunctionFromSet(Context);
  DAGNode* LMDDPart = createDAGFromDD(result);
  
  DAGNode* FinalResult = quickSimplify_WithLMISupport(createDAG("and", LMDDPart, ConjunctionOfContext, LOGICAL_SYMBOL, 1));
  
  fprintf(LMIQEOutputDataFp, "\n#Vars benefited by LMEs in Context = %d\n", eliminated_by_firstloop);
  fprintf(LMIQEOutputDataFp, "\n#Vars such that LMDD is free of the variable = %d\n", eliminated_by_secondloop);
  fprintf(LMIQEOutputDataFp, "\n#Remaining variables = %d\n", eliminated_by_thirdloop);
  
  TotalVarsEliminatedByLMEsFromTailedTriangles = TotalVarsEliminatedByLMEsFromTailedTriangles + eliminated_by_firstloop;
  TotalVarsEliminatedFreeTrianglesFromTailedTriangles = TotalVarsEliminatedFreeTrianglesFromTailedTriangles + eliminated_by_secondloop;
  TotalRemainingVarsEliminatedFromTailedTriangles = TotalRemainingVarsEliminatedFromTailedTriangles + eliminated_by_thirdloop;
  
  return FinalResult;
}
  

// Version of getSimplifiedInstance such that the simplification is done using both LMEs and hash table entries
void DDManager::getSimplifiedInstanceUsingLMEsAndHashTableEntries(DDNode* f, set<DAGNode*> &Context, set<string> &VarSetToElim, DDNode* &Simplified_f, set<DAGNode*> &Simplified_C_X, set<string> &Simplified_VarSetToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified, int project_depth)
{
    
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;  
    
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> LMESet;
  set<DAGNode*> LMDSet;
  set<DAGNode*> LMISet;
  
  list<string> VarsToElim;
  list<string> VarsStillToElim;
  copySetToList(VarSetToElim, VarsToElim);
  
  DDNode* result = f;
  
  int number_of_variables_tobe_eliminated;
  int eliminated_by_firstloop=0;
  int eliminated_by_secondloop=0;  
  
  number_of_variables_tobe_eliminated = VarSetToElim.size();

  VarsStillToElim.clear();
  
  gettimeofday (&start_ms, NULL);  
  
  while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::getSimplifiedInstanceUsingLMEsAndHashTableEntries in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return;
	}
      
      string VarToElim;
      list<string>::iterator lit = VarsToElim.begin();
      VarToElim = *lit;          
            
      if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
        }
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Context, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      if(DAGResolveApplicable(LMESet, LMDSet, WidthTable))
        {
          
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {  
            cout<<"\nDAGResolveApplicable(LMESet, LMDSet, WidthTable)\n";
            }
        
        eliminated_by_firstloop++; 
          
        TableForDagResolve.clear();
        
        result = QElim_WithLMISupport(VarToElim, result, LMESet, LMDSet, LMISet, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
    
        VarsToElim.remove(VarToElim); 
        
        Context = Free_Constraints;
        }   
      else
        {
         if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            { 
            cout<<"\n~DAGResolveApplicable(LMESet, LMDSet, WidthTable)\n";         
            }
         
         VarsStillToElim.push_back(VarToElim);
         
         VarsToElim.remove(VarToElim); 
        }
    }//end(while)
  
  gettimeofday (&finish_ms, NULL);
  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;  
  TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForLMEs = TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForLMEs + duration_ms;
  
  
  VarsToElim = VarsStillToElim;
  
  VarsStillToElim.clear();
  
  
  gettimeofday (&start_ms, NULL);  
  
  while(!VarsToElim.empty())
    {
      if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::getSimplifiedInstanceUsingLMEsAndHashTableEntries in DDManager.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return;
	}
      
      string VarToElim;
      list<string>::iterator lit = VarsToElim.begin();
      VarToElim = *lit;  
      
      if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nVariable to eliminate = "<<VarToElim<<endl;
        }
             
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Context, VarToElim, Free_Constraints, Bound_Constraints, WidthTable);
      
      LMESet.clear();
      LMDSet.clear();
      LMISet.clear();
      splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
      
      DDNode* Existing  = getEntryFromTableForQElimModified_WithLMISupport(VarToElim, result, LMESet, LMDSet, LMISet, PtrTableForQElimModified);

     if(Existing != NULL)
        {
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {  
            cout<<"\nexists VarToElim. (result AND C_{x}) exists in HT\n";
            }
        
        eliminated_by_secondloop++; 
        
        result = Existing;
        
        VarsToElim.remove(VarToElim); 
        
        Context = Free_Constraints;
        }
      else
        {
          if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nexists VarToElim. (result AND C_{x}) does not exist in HT\n";
            }
         
         VarsStillToElim.push_back(VarToElim);
         
         VarsToElim.remove(VarToElim); 
        }
    }//end(while)
  
  gettimeofday (&finish_ms, NULL);
  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;  
  TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForHT = TimeIngetSimplifiedInstanceUsingLMEsAndHashTableEntriesForHT + duration_ms;

  
  Simplified_f = result;  
  Simplified_C_X = Context;  
  
  copyListToSet(VarsStillToElim, Simplified_VarSetToElim);
  
  
  NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries = NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntries + number_of_variables_tobe_eliminated;
  
  NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByLMEs = NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByLMEs + eliminated_by_firstloop;
  
  NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByHT = NoOfVarsInCallsTogetSimplifiedInstanceUsingLMEsAndHashTableEntriesBenefitedByHT + eliminated_by_secondloop;
  
}
  

// returns true if we decide to check if \exists X. f/\Context is contained in PartialResult already through an SMT solver call; false otherwise
bool DDManager::SMTCallNeededInConversionToDisjunctionOfTailedTrianglesWithAggressiveLMEBasedSimplificationAndQE(DDNode* f)
{
  bool always_use_smtcall=false;
  bool never_use_smtcall=false;  
  
  if(always_use_smtcall)//Always use an SMT call
  {
      return true;
  }
  else if(never_use_smtcall)//Never use an SMT call
  {
      return false;
  }
  else //Use a strategy to decide
  {
       int SizeOfMyDD = getSizeOfDD(f);
       
       cout<<"\nSize of dd = "<<SizeOfMyDD<<endl;
       
       if(SizeOfMyDD <= MaximumSizeOfLMDDForQELMDDInDisjunctionStrategy)//Size of f is <= MaximumSizeOfLMDDForQELMDDInDisjunctionStrategy
           //it can be done by QE_LMDD; no SMT call
        {
        return false;   
        }    
       else
        {
         return true;
        }
  }//Use a strategy to decide  
}//function ends here


//Performs LME based simplification as a preprocessing step before doing the QE
DDNode* DDManager::callPerformLMEBasedSimplificationAsPreprocessor(set<string> &VarsToElim, DDNode* dd, map<string, int> &WidthTable)
{
HashTable <string, DDNode*> TableForPerformLMEBasedSimplificationAsPreprocessor;
HashTable <string, DDNode*> *PtrTableForPerformLMEBasedSimplificationAsPreprocessor = &TableForPerformLMEBasedSimplificationAsPreprocessor;

return performLMEBasedSimplificationAsPreprocessor(VarsToElim, dd, WidthTable, PtrTableForPerformLMEBasedSimplificationAsPreprocessor);
}



//Function called by callPerformLMEBasedSimplificationAsPreprocessor
DDNode* DDManager::performLMEBasedSimplificationAsPreprocessor(set<string> VarSetToElim, DDNode* f, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForPerformLMEBasedSimplificationAsPreprocessor)
{
   CallsToLMEBasedSimplificationAsPreprocessor++; 
   
   DDNode* Existing  = getEntryFromTableForLMEBasedSimplificationAsPreprocessor(f, PtrTableForPerformLMEBasedSimplificationAsPreprocessor);

   if(Existing != NULL)
    {
      HitsInLMEBasedSimplificationAsPreprocessor++;       

     //cout<<"\nsize of DD returned = "<<getSizeOfDD(Existing)<<endl;

      return Existing;
    }
   
    MissesInLMEBasedSimplificationAsPreprocessor++;
   
    DDNode* result;

    if(f->getPredicate()==NULL) // terminal node
        {
                
        result = f;
        
        }
    else// non-terminal node
        {
        //cout<<"\nNon-terminal node\n";
        //cout<<"\nf = "<<getDAGStr(createDAGFromDD(f))<<endl;
        
        DAGNode* C = f->getPredicate();
        
        
        string OddVariableToEliminate;
        
        if(predicateIsLMEWithOddCoefForSomeVarToElim(C, VarSetToElim, OddVariableToEliminate))
            {
            
            DDNode *thenb_after_lme_simplification;
            
            if(DDFreeOfVar_WithDP(f->getRight(), OddVariableToEliminate))//f->getRight() is already free of OddVariableToEliminate.
                // No need to apply simplifyLMDD
            {
               CallsToLMEBasedSimplificationAsPreprocessorWhereLMDDIsFreeOfVar++; 
                
                thenb_after_lme_simplification =  f->getRight();
            }
            else
            {
                VariablesBenefittedByLMEBasedSimplificationAsPreprocessor.insert(OddVariableToEliminate);
                        
                struct timeval start_ms, finish_ms;
                unsigned long long int duration_ms;    
                gettimeofday (&start_ms, NULL);     
            
                OriginalCallsToDAGResolve++;                  
                
                if(PerformSimplifyLMDDInsidePreprocessingKeepingStructuralIsomorphism)
                // Tries to create an output DD, which is structurally isomorphic to f->getRight()
                    {
                    //cout<<"\nf->getRight() = "<<getDAGStr(createDAGFromDD(f->getRight()))<<endl;
                    
                    thenb_after_lme_simplification =  performSimplifyLMDDKeepingStructuralIsomorphism(f->getRight(), C, OddVariableToEliminate, WidthTable);
                    
                    //cout<<"\nthenb_after_lme_simplification = "<<getDAGStr(createDAGFromDD(thenb_after_lme_simplification))<<endl;
                    }
                else
                    {            
                    //cout<<"\nf->getRight() = "<<getDAGStr(createDAGFromDD(f->getRight()))<<endl;
                    
                    thenb_after_lme_simplification =  DagResolveModifiedAdvancedWithDynamicVariableOrdering_WithLMISupport(f->getRight(), C, OddVariableToEliminate, WidthTable);
                    
                    //cout<<"\nthenb_after_lme_simplification = "<<getDAGStr(createDAGFromDD(thenb_after_lme_simplification))<<endl;
                    }
            
                gettimeofday (&finish_ms, NULL);
                duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
                duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;  
        
                TimeInSimplifyLMDDInLMEBasedSimplificationAsPreprocessor = TimeInSimplifyLMDDInLMEBasedSimplificationAsPreprocessor + duration_ms;
            }
            //cout<<"\nthenb_after_lme_simplification = "<<getDAGStr(createDAGFromDD(thenb_after_lme_simplification))<<endl;
            
            //VarSetToElim on the then branch is changed now
            
            set<string> VarSetToElimThen;
            
            set<string> VariablesEliminated;
            
            VariablesEliminated.insert(OddVariableToEliminate);
            
            set_difference(VarSetToElim.begin(), VarSetToElim.end(), VariablesEliminated.begin(), VariablesEliminated.end(),inserter(VarSetToElimThen, VarSetToElimThen.begin()));
            
            DDNode *thenb = performLMEBasedSimplificationAsPreprocessor(VarSetToElimThen, thenb_after_lme_simplification, WidthTable, PtrTableForPerformLMEBasedSimplificationAsPreprocessor);
            
            DDNode *elseb = performLMEBasedSimplificationAsPreprocessor(VarSetToElim, f->getLeft(), WidthTable, PtrTableForPerformLMEBasedSimplificationAsPreprocessor);
            
            DDNode* C_dd = createDD(C, WidthTable);

            //cout<<"\nNumber of nodes in arguments of createITE = "<<getSizeOfDD(thenb)<<"\t"<<getSizeOfDD(elseb)<<"\t"<<getSizeOfDD(C_dd)<<endl;

            result = createITE(C_dd, thenb, elseb);            
            }
        else
            {            
            
            DDNode *thenb = performLMEBasedSimplificationAsPreprocessor(VarSetToElim, f->getRight(), WidthTable, PtrTableForPerformLMEBasedSimplificationAsPreprocessor);
            
            DDNode *elseb = performLMEBasedSimplificationAsPreprocessor(VarSetToElim, f->getLeft(), WidthTable, PtrTableForPerformLMEBasedSimplificationAsPreprocessor);
            
            DDNode* C_dd = createDD(C, WidthTable);

            //cout<<"\nNumber of nodes in arguments of createITE = "<<getSizeOfDD(thenb)<<"\t"<<getSizeOfDD(elseb)<<"\t"<<getSizeOfDD(C_dd)<<endl;

            result = createITE(C_dd, thenb, elseb);   
            
            }
        
        //cout<<"\nresult = "<<getDAGStr(createDAGFromDD(result))<<endl;
        
        }// non-terminal node
      
    insertEntryIntoTableForLMEBasedSimplificationAsPreprocessor(f, result, PtrTableForPerformLMEBasedSimplificationAsPreprocessor);

    //cout<<"\nsize of DD returned = "<<getSizeOfDD(result)<<endl;

    return result;
    
}//function



// Check if performLMEBasedSimplificationAsPreprocessor is already encountered on f
// If yes, return the result; NULL otherwise
DDNode* DDManager::getEntryFromTableForLMEBasedSimplificationAsPreprocessor(DDNode* f, HashTable <string, DDNode*> *PtrTableForPerformLMEBasedSimplificationAsPreprocessor)
{
  // Creating the hash key
  string key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  key = temp_char;

  // Hash key created
  HTStatusValue<DDNode*> result = PtrTableForPerformLMEBasedSimplificationAsPreprocessor->hashtable_search(key);
  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// Enter f ---> result into performLMEBasedSimplificationAsPreprocessor's hash table
void DDManager::insertEntryIntoTableForLMEBasedSimplificationAsPreprocessor(DDNode* f, DDNode* value, HashTable <string, DDNode*> *PtrTableForPerformLMEBasedSimplificationAsPreprocessor)
{
  // Creating the hash key

  string key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  key = temp_char;
  
  // Hash key created
  // Check if entry already exists

  HTStatusValue<DDNode*> result = PtrTableForPerformLMEBasedSimplificationAsPreprocessor->hashtable_search(key);
  
  if(result.success())
    {
      //cout<<"\nalready encountered node. do nothing\n";
      return;
    }

  result = PtrTableForPerformLMEBasedSimplificationAsPreprocessor->hashtable_insert(key, value);
  if(!result.success())
    {

      cout<<"\nHash insert failure inside function DDManager::insertEntryIntoTableForLMEBasedSimplificationAsPreprocessor\n";
      exit(1);
    }
}


// return \exists VarToElim. (LMC \wedge LME) in simplifyLMDD
DAGNode* DDManager::simplifyLMCInsideSimplifyLMDD(DAGNode* MyPredicate, DAGNode* equation, string VarToElim, map<string, int> &WidthTable)
{
    struct timeval start_ms, finish_ms;
    unsigned long long int duration_ms; 
    
    set<DAGNode*> Constraints;
    Constraints.insert(equation);
    Constraints.insert(MyPredicate);
    
    DAGNode* Existing;

    Existing = getEntryFromTableForSimplifyLMCInsideSimplifyLMDD(VarToElim, Constraints);

    if(Existing != NULL)
        {
        RecursiveCallsToDAGResolveLeadingToLayer1CallHit++;
        
        return Existing;
        }
    
    

    DAGNode* NewPredicate;
    // Checking if Constraints is False before calling the eliminator

    set<DAGNode*> TemporaryDisEqSet;
    TemporaryDisEqSet.clear();
    set<DAGNode*> TemporaryInEqSet;
    TemporaryInEqSet.clear();

    gettimeofday (&start_ms, NULL);    

    bool theory_unsat = ThUnsat(Constraints, TemporaryDisEqSet, TemporaryInEqSet, WidthTable);

    gettimeofday (&finish_ms, NULL);
    duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
    duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;    
    TimeSpentInSMTCallsInsideDAGResolve = TimeSpentInSMTCallsInsideDAGResolve + duration_ms;


    if(theory_unsat)
    {
    NewPredicate = createLogicalConstant("false");
    }
    else
    {
    set<string> VarsToElim;
    VarsToElim.insert(VarToElim);

    gettimeofday (&start_ms, NULL);    

    NewPredicate = applyLMILayer1(Constraints, VarsToElim, WidthTable);

    gettimeofday (&finish_ms, NULL);
    duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
    duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;    
    TimeSpentInLayer1CallsInsideDAGResolve = TimeSpentInLayer1CallsInsideDAGResolve + duration_ms;

    if(NewPredicate->Get_node_name()=="=" || NewPredicate->Get_node_name()=="equivalent") // LME
        {
         NewPredicate = recreateEquationInProperForm(NewPredicate, WidthTable); // recreate in cannonical form
        }
    if(NewPredicate->Get_node_name()=="=" || NewPredicate->Get_node_name()=="equivalent" ||
    NewPredicate->Get_node_name()=="bvule" || NewPredicate->Get_node_name()=="bvuge" ||
    NewPredicate->Get_node_name()=="bvult" || NewPredicate->Get_node_name()=="bvugt")
        {
         dynamicOrdering(MyPredicate, NewPredicate); // increment the indices of the predicates in APMap above MyPredicate by 1 and insert NewPredicate just on top of MyPredicate i.e. at MyPredicate's index + 1
        }
    else if(isFalse(NewPredicate) || isTrue(NewPredicate))
        {
        //Fine
        }
    else
        {
        cout<<"\nError in function DDManager::simplifyLMCInsideSimplifyLMDD! The newly created predicate is not a constant or predicate\n";
        exit(1);
        }		
    }//else of  if(theory_unsat)

    insertEntryIntoTableForSimplifyLMCInsideSimplifyLMDD(VarToElim, Constraints, NewPredicate);
    
    return NewPredicate;
}



// checks if \exists VarToElim. (LMC \wedge LMI) is already encountered
// inside simplifyLMDD
// return the result if yes; return NULL otherwise
DAGNode* DDManager::getEntryFromTableForSimplifyLMCInsideSimplifyLMDD(string VarToElim, set<DAGNode*> &Constraints)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  // Hash key created

  HTStatusValue<DAGNode*> result = TableForSimplifyLMCInsideSimplifyLMDD.hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }
  else
    {
      return NULL;
    }
}

// enter \exists VarToElim.(LMC \wedge LMI) ---> result into HashTable
void DDManager::insertEntryIntoTableForSimplifyLMCInsideSimplifyLMDD(string VarToElim, set<DAGNode*> &Constraints, DAGNode* value)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];


  for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    } 

  // Hash key created
  // Check if entry already exists

  HTStatusValue<DAGNode*> result = TableForSimplifyLMCInsideSimplifyLMDD.hashtable_search(key);


  if(result.success())
    {
      // Do nothing
      return;
    }
  else
  {
    result = TableForSimplifyLMCInsideSimplifyLMDD.hashtable_insert(key, value);
   
    if(!result.success())
        {
        cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForSimplifyLMCInsideSimplifyLMDD\n";
        exit(1);
        }
    }//else
}



// Function to convert \exists X.f into 
// \bigvee \limits_{i=1}^n (\exists X. (f_i \wedge C_i)) such that
// 1) \bigvee \limits_{i=1}^n (\exists X. (f_i \wedge C_i)) \equiv \exists X. f
// 2) (SMTCallsCommittedInEagerConversionToDisjunctions >= MaximumNoOfSMTCallsInEagerConversionToDisjunctions) \vee 
//     (size of all f_i's <= MaximumSizeOfTriangleInEagerConversionToDisjunctions)
void DDManager::convertToSimplifiedInstancesEager(DDNode* f, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances)
{
   set<DDNode*> UnSimplifiableTriangles; 
    
    while(SMTCallsCommittedInEagerConversionToDisjunctions < MaximumNoOfSMTCallsInEagerConversionToDisjunctions)
    {
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nInside while loop\n";
                cout<<"\nSMTCallsCommittedInEagerConversionToDisjunctions = "<<SMTCallsCommittedInEagerConversionToDisjunctions<<endl;
                }           
        
        if(SimplifiedInstances.size()==0)//SimplifiedInstances is empty
        {
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nSimplifiedInstances is empty\n";
                }            
            
            set<DAGNode*> TempContext;
            
           convertToSimplifiedInstances(f, TempContext, SimplifiedInstances); 
           //\bigvee \limits_{i=1}^n (\exists X. (f_i \wedge C_i))	:= convertToSimplifiedInstances(\exists X. f, \phi);
           
           //cout<<"\nInitial Simplified Instances\n";
           //showSizesOfSimplifiedInstances(SimplifiedInstances);
           
           SMTCallsCommittedInEagerConversionToDisjunctions = SimplifiedInstances.size();
           
           
           if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                showSimplifiedInstances(SimplifiedInstances);
                cout<<"\nSMTCallsCommittedInEagerConversionToDisjunctions = "<<SMTCallsCommittedInEagerConversionToDisjunctions<<endl;
                }          
           
        }
        else
        {
            
           if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nSimplifiedInstances is non-empty\n";
                }              
            
            bool AllLMDDSizesWithInThreshold = true;
            
                       
             for(int i = 0; i < SimplifiedInstances.size(); i++)
                {
                set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;//get C_i
                
                DDNode* MyLMDD = (SimplifiedInstances[i]).second;//get f_i
                
                if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nTaking f_i and C_i\n";
                    showAPSet("C_i", MyContext);
                    cout<<"\nf_i = "<<getDAGStr(createDAGFromDD(MyLMDD))<<endl;
                    }  
                
                int SizeOfMyLMDD = getSizeOfDD(MyLMDD);
                
                if(SizeOfMyLMDD > MaximumSizeOfTriangleInEagerConversionToDisjunctions && UnSimplifiableTriangles.find(MyLMDD) == UnSimplifiableTriangles.end())
                    {
                    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                        {
                        cout<<"\nsize of f_i > threshold t_2 and f_i is not an unsimplifiable triangle\n";
                        }                      
                    
                    //cout<<"\nTaking LMDD with size "<<SizeOfMyLMDD<<endl;
                    
                    /* Let us try to split MyLMDD into disjunction of tailed triangles */
                    
                    set<DAGNode*> TempContext_New;
                    
                    vector< pair< set<DAGNode*>, DDNode* > > SimplifiedInstances_New;
            
                    convertToSimplifiedInstances(MyLMDD, TempContext_New, SimplifiedInstances_New); 
                    
                    if(SimplifiedInstances_New.size() > 1)//convertToSimplifiedInstances could simplify the triangle MyLMDD
                        {
                    
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            cout<<"\nconvertToSimplifiedInstances could simplify the triangle MyLMDD\n";
                            } 
                        
                        AllLMDDSizesWithInThreshold = false;                   
                                        
                        /*We have  f_i  \equiv \bigvee \limits_{j=1}^m (f_j \wedge C_j)
                        Hence (f_i \wedge C_i)  \equiv \bigvee \limits_{j=1}^m (f_j \wedge C_j \wedge C_i)
                        Hence \exists X. (f_i \wedge C_i)  \equiv \bigvee \limits_{j=1}^m \exists X.(f_j \wedge C_j \wedge C_i) */
				
                        /* Remove \exists X. (f_i \wedge C_i) from \bigvee \limits_{i=1}^n (\exists X. (f_i \wedge C_i)) and insert 
                            \bigvee \limits_{j=1}^m (\exists X. (f_j \wedge C_j \wedge C_i)) at the end  */
                        
                        //cout<<"\nNew Simplified Instances\n";
                        //showSizesOfSimplifiedInstances(SimplifiedInstances_New);
                    
                        recreateSimplifiedInstances(SimplifiedInstances, i, SimplifiedInstances_New, MyContext);
                        
                        //cout<<"\nChanged Simplified Instances\n";
                        //showSizesOfSimplifiedInstances(SimplifiedInstances);
                    
                        SMTCallsCommittedInEagerConversionToDisjunctions = SimplifiedInstances.size();            
                                      
                    
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            showSimplifiedInstances(SimplifiedInstances);
                            cout<<"\nSMTCallsCommittedInEagerConversionToDisjunctions = "<<SMTCallsCommittedInEagerConversionToDisjunctions<<endl;
                            }                     
                    
                        break;
                        }//if
                    else //convertToSimplifiedInstances could not simplify the triangle MyLMDD.  Let us declare it as an unsimplifiable triangle
                        {
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            cout<<"\nconvertToSimplifiedInstances could not simplify the triangle MyLMDD.  Let us declare it as an unsimplifiable triangle\n";
                            }                        
                        
                        UnSimplifiableTriangles.insert(MyLMDD);
                        }//else
                    }//if
                }//for
            if(AllLMDDSizesWithInThreshold)
		{
		break;
		}//break from while loop and hence return
        }//else
    }//while
}//function
    
    
// Erase the entry at location i from SimplifiedInstances
// Take each entry from SimplifiedInstances_New, concatenate ExistingContext with C_i of each,
// and push back the tailed triangle at the end of SimplifiedInstances
void DDManager::recreateSimplifiedInstances(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, int i, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances_New, set<DAGNode*> &ExistingContext)
{
    // Erase the entry at location i from SimplifiedInstances
    SimplifiedInstances.erase(SimplifiedInstances.begin()+i);
 
    // Take each entry from SimplifiedInstances_New, concatenate ExistingContext with C_i of each,
    // and push back the tailed triangle at the end of SimplifiedInstances
    for(int i = 0; i < SimplifiedInstances_New.size(); i++)
                {
                set<DAGNode*> MyContext = (SimplifiedInstances_New[i]).first;//get C_i
                
                DDNode* MyLMDD = (SimplifiedInstances_New[i]).second;//get f_i
                
                set<DAGNode*> FinalContext;
                
                set_union(MyContext.begin(), MyContext.end(), ExistingContext.begin(), ExistingContext.end(), inserter(FinalContext, FinalContext.begin())); 
		
                SimplifiedInstances.push_back(make_pair(FinalContext, MyLMDD));
                }    
}




// Given an LMDD "dd", variable to eliminate "VarToElim", and equation "equation", this function replaces 
// each occurance of "VarToElim" in the dd using layer1 and returns an LMDD which is structurally isomorphic to "dd"
DDNode*  DDManager::performSimplifyLMDDKeepingStructuralIsomorphism(DDNode *dd, DAGNode *equation, string VarToElim, map<string, int> &WidthTable)
{ 
  HashTable <string, DDNode*> MapForPerformSimplifyLMDDKeepingStructuralIsomorphism;
  HashTable <string, DDNode*> *PtrMapForPerformSimplifyLMDDKeepingStructuralIsomorphism = &MapForPerformSimplifyLMDDKeepingStructuralIsomorphism;

  DDNode* result = performSimplifyLMDDKeepingStructuralIsomorphismInternal(dd, equation, VarToElim, WidthTable, PtrMapForPerformSimplifyLMDDKeepingStructuralIsomorphism);
  return result;
}


// Function internally called by DDManager::performSimplifyLMDDKeepingStructuralIsomorphism
DDNode*  DDManager::performSimplifyLMDDKeepingStructuralIsomorphismInternal(DDNode *dd, DAGNode *equation, string VarToElim, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrMapForPerformSimplifyLMDDKeepingStructuralIsomorphism)
{     
  DDNode* Existing = getEntryFromPerVariableHT(dd, equation, VarToElim, PtrMapForPerformSimplifyLMDDKeepingStructuralIsomorphism);
  
  if(Existing != NULL)        
    {
    //yes, get the hashed value
    //cout<<"\nHit in dag-resolve hash table"<<endl; 
    return Existing;
    }
  //cout<<"\nMiss in dag-resolve hash table"<<endl; 
  
  if(dd->getPredicate() == NULL) // Terminal
    {
      return dd;
    }

  
  DDNode *NewLeft = performSimplifyLMDDKeepingStructuralIsomorphismInternal(dd->getLeft(), equation, VarToElim, WidthTable, PtrMapForPerformSimplifyLMDDKeepingStructuralIsomorphism);
  DDNode *NewRight = performSimplifyLMDDKeepingStructuralIsomorphismInternal(dd->getRight(), equation, VarToElim, WidthTable, PtrMapForPerformSimplifyLMDDKeepingStructuralIsomorphism);

  DAGNode *MyPredicate = dd->getPredicate();

  DAGNode *NewPredicate;

  // Check if MyPredicate contains the variable to be eliminated
   
  bool MyPredicateFreeOfVarToElim= freeOfVarModifiedEfficient(MyPredicate, VarToElim); 
  //Note that this procedure returns false even if MyPredicate has 0*VarToElim + ...  

  if(MyPredicateFreeOfVarToElim)
    {
      NewPredicate = MyPredicate;
    }
  else if(!coeffIsOdd(equation, VarToElim)) // check if the coefficient of variable to be eliminated is odd in the equation selected
    {
      // Ideally, this should be done before selecting the equation
      NewPredicate = MyPredicate;
    } 
  else
    {
      // Apply Layer1 to eliminate VarToElim from the predicate and obtain the NewPredicate (Also insert NewPredicate in the APMap)
      // This function also hashes the calls to Layer1 and tries to re-use them
      
      NewPredicate = simplifyLMCInsideSimplifyLMDD(MyPredicate, equation, VarToElim, WidthTable);      
    }//else of else if(!coeffIsOdd(equation, VarToElim))
  
  //cout<<endl<<"NewPredicate = "<<getDAGStr(NewPredicate)<<endl;
  
  DDNode* resultDD;
  
  if(NewPredicate == NULL)
    {
      cout<<"\nNewPredicate is NULL in function DDManager::performSimplifyLMDDKeepingStructuralIsomorphismInternal\n";
      exit(1);
    }
  else if(isTrue(NewPredicate))
    {
      resultDD = createDD(1);
    }
  else if(isFalse(NewPredicate))
    {
      resultDD = createDD(0);
    }
  else
    {
    // Note that in DD nodes, left is else and right is then
    DDNode* then_dd = NewRight;
    DDNode* els_dd = NewLeft;
  
    resultDD = createDD(NewPredicate, els_dd, then_dd);
    //The order of children for createDD is like this
    }
  
   
  insertEntryIntoPerVariableHT(dd, equation, VarToElim, resultDD, PtrMapForPerformSimplifyLMDDKeepingStructuralIsomorphism);   

  return resultDD;
}


// Show sizes of the simplified instances
void DDManager::showSizesOfSimplifiedInstances(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances)
{
    cout<<"\nSizes of SimplifiedInstances\n******************\n";
    
    for(int i=0; i < SimplifiedInstances.size(); i++)
    {
        set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;
        DDNode* Myf = (SimplifiedInstances[i]).second;
        cout<<getSizeOfDD(Myf)<<"\t";
    }
    cout<<endl;
}


// Reverse the elements in SimplifiedInstances
void DDManager::obtainReversedSimplifiedInstances(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances)
{
   vector< pair< set<DAGNode*>, DDNode* > > New_SimplifiedInstances;
   
   for(int i=SimplifiedInstances.size()-1; i >= 0; i--)
    {
        set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;
        DDNode* Myf = (SimplifiedInstances[i]).second;
        
        New_SimplifiedInstances.push_back(make_pair(MyContext, Myf));
    }
   
    SimplifiedInstances =   New_SimplifiedInstances; 
}

// Version of DDManager::QEFromDisjunctionOfTailedTriangles, which
// creates new disjuncts from an existing disjunct "dd" 
// provided the number of SMT calls < threshold t_1 
// and size of "dd" > threshold t_2 and QE from "dd" is required
DDNode* DDManager::QEFromDisjunctionOfTailedTrianglesWithDemandBasedDisjunctionCreation(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, set<string> &VarSetToElim, map<string, int> &WidthTable)
{
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;   
    
 TableForDagResolve.clear();

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;
  
  int project_depth = 0;
  
  set<DDNode*> UnSimplifiableTriangles;
  
  obtainReversedSimplifiedInstances(SimplifiedInstances); //so that smaller triangles appear first

  int SMTCallsCommitted = SimplifiedInstances.size();


  while(SimplifiedInstances.size() != 0)//while !SimplifiedInstances.empty()
    {
        //f_i \wedge C_i := SimplifiedInstances[0];
      
        set<DAGNode*> MyContext = (SimplifiedInstances[0]).first;
        
        DDNode* MyLMDD = (SimplifiedInstances[0]).second;
        
        int SizeOfMyLMDD = getSizeOfDD(MyLMDD);
        
        SizeOfTrianglesInEagerConversionToDisjunctions.push_back(SizeOfMyLMDD);
        
        //Let's see if \exists VarSetToElim. (MyLMDD /\ MyContext) is needed or not
        
        // PathCondition = MyLMDD /\ MyContext
        DAGNode* PathCondition = createDAG("and", createDAGFromDD(MyLMDD), getConjunctionFromSet(MyContext), LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
            }
   
        DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
            }
  
        DAGNode*  SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
        
        map<string, string> ModelOfSatisfiabilityCondition;
        
        gettimeofday (&start_ms, NULL);
        
        bool SatisfiabilityConditionIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, ModelOfSatisfiabilityCondition, WidthTable);
        
        gettimeofday (&finish_ms, NULL);
        
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        TotalSMTTimeInConversionToDisjunctionOfTailedTriangles = TotalSMTTimeInConversionToDisjunctionOfTailedTriangles + duration_ms;
            
        if(SatisfiabilityConditionIsFalse)
            {
            cout<<"\nexists X. (C_i and f_i) => Pi\n";
            
            SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("un");
            
            NumberOfUnsatTailedTrianglesInDemandBasedConversionToDisjunctions++;
            
            //Remove \exists X. (f_i \wedge C_i) from SimplifiedInstances;
            SimplifiedInstances.erase(SimplifiedInstances.begin()+0);
            }
        else
            {
            cout<<"\nNOT(exists X. (C_i and f_i) => Pi)\n";
            
            if(SizeOfMyLMDD > MaximumSizeOfTriangleInDemandBasedConversionToDisjunctions && SMTCallsCommitted < MaximumNoOfSMTCallsInDemandBasedConversionToDisjunctions && UnSimplifiableTriangles.find(MyLMDD) == UnSimplifiableTriangles.end())
                    {
                
                    SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("sp");
                    
                    NumberOfSplitTailedTrianglesInDemandBasedConversionToDisjunctions++;
                
                    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                        {
                        cout<<"\nsize of f_i > threshold t_2 and f_i is not an unsimplifiable triangle\n";
                        }                      
                    
                    //cout<<"\nTaking LMDD with size "<<SizeOfMyLMDD<<endl;
                    
                    /* Let us try to split MyLMDD into disjunction of tailed triangles */
                    
                    set<DAGNode*> TempContext_New;
                    
                    vector< pair< set<DAGNode*>, DDNode* > > SimplifiedInstances_New;
            
                    convertToSimplifiedInstances(MyLMDD, TempContext_New, SimplifiedInstances_New); 
                    
                    if(SimplifiedInstances_New.size() > 1)//convertToSimplifiedInstances could simplify the triangle MyLMDD
                        {
                    
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            cout<<"\nconvertToSimplifiedInstances could simplify the triangle MyLMDD\n";
                            } 
                                        
                        /*We have  f_i  \equiv \bigvee \limits_{j=1}^m (f_j \wedge C_j)
                        Hence (f_i \wedge C_i)  \equiv \bigvee \limits_{j=1}^m (f_j \wedge C_j \wedge C_i)
                        Hence \exists X. (f_i \wedge C_i)  \equiv \bigvee \limits_{j=1}^m \exists X.(f_j \wedge C_j \wedge C_i) */
			
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            cout<<"\nNew Simplified Instances\n";
                            showSizesOfSimplifiedInstances(SimplifiedInstances_New);
                            }                        
                        
                         /* Remove \exists X. (f_i \wedge C_i) from SimplifiedInstances and insert 
                            \bigvee \limits_{j=1}^m (\exists X. (f_j \wedge C_j \wedge C_i)) at its position
			     i.e. at the beginning in the reverse order  */
                        
                        recreateSimplifiedInstancesForDemandBasedMode(SimplifiedInstances, SimplifiedInstances_New, MyContext);
                        
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            cout<<"\nChanged Simplified Instances\n";
                            showSizesOfSimplifiedInstances(SimplifiedInstances);
                            }
                    
                        //SMTCallsCommitted = SMTCallsCommitted + m;
                        
                        SMTCallsCommitted =   SMTCallsCommitted + SimplifiedInstances_New.size();                                      
                    
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            showSimplifiedInstances(SimplifiedInstances);
                            cout<<"\nSMTCallsCommitted = "<<SMTCallsCommitted<<endl;
                            }  
                        }//if convertToSimplifiedInstances could simplify the triangle MyLMDD
                    else //convertToSimplifiedInstances could not simplify the triangle MyLMDD.  Let us declare it as an unsimplifiable triangle
                        {
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            cout<<"\nconvertToSimplifiedInstances could not simplify the triangle MyLMDD.  Let us declare it as an unsimplifiable triangle\n";
                            }                        
                        
                        UnSimplifiableTriangles.insert(MyLMDD);
                        }//if convertToSimplifiedInstances could not simplify the triangle MyLMDD.  Let us declare it as an unsimplifiable triangle
                    }//if (the number of SMT calls < threshold t_1 and disjunct size > threshold t_2 and disjunct is simplifiable)       
            else
                    {                
                      //Pi_i := QEFromTailedTriangle( \exists X. (f_i \wedge C_i) );
                
                      //Pi := Pi \vee Pi_i;

                    SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("qe");

                    NumberOfTailedTrianglesToQE++;

                    DAGNode* Pi_i;


                    set<DAGNode*> FreeContext;
                    set<DAGNode*> BoundContext;

                    getFreeAndBoundConstraintsForMultipleVariables(MyContext, VarSetToElim, FreeContext, BoundContext, WidthTable);

                    DAGNode* ConjunctionOfFreeContext = getConjunctionFromSet(FreeContext);


                    gettimeofday (&start_ms, NULL);

                    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                        {
                         showAPSet("FreeContext", FreeContext);
                         showAPSet("BoundContext", BoundContext);
                         cout<<"\nMyLMDD = "<<getDAGStr(createDAGFromDD(MyLMDD))<<endl;   
                        }  

                    if(LMEBasedSimplificationAsPreprocessor)//LME based simplification is already done
                        {
                        Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext_WhichIsFreeOfLMEs(MyLMDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                        }
                    else
                        {           
                        Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(MyLMDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                        }

                    gettimeofday (&finish_ms, NULL);

                    duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
                    duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
                    TotalQETimeInConversionToDisjunctionOfTailedTriangles = TotalQETimeInConversionToDisjunctionOfTailedTriangles + duration_ms;

                    Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfFreeContext, LOGICAL_SYMBOL, 1));

                    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                        {
                        cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                        }                   

                    PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
                    
                    //Remove \exists X. (f_i \wedge C_i) from SimplifiedInstances;
                    SimplifiedInstances.erase(SimplifiedInstances.begin()+0);
                    
                    }//else of if (the number of SMT calls < threshold t_1 and disjunct size > threshold t_2 and disjunct is simplifiable)
            }//else of if(SatisfiabilityConditionIsFalse)
    }//while ends here  
  
  DDNode* result = createDD(0);// result is actually in PartialResult_In_LMDDWithMonniauxTypeSimplifications
  // The DD part of the result is always false
  
  return result;
}


// Recreate SimplifiedInstances in the following way:
// Reverse SimplifiedInstances_New
// Take each entry from SimplifiedInstances_New, concatenate ExistingContext with C_i of each,
// and push back the tailed triangle at the end of recreated SimplifiedInstances
// Push back the existing entries in SimplifiedInstances except the first one into recreated SimplifiedInstances
void DDManager:: recreateSimplifiedInstancesForDemandBasedMode(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances_New, set<DAGNode*> &ExistingContext)
{
    vector< pair< set<DAGNode*>, DDNode* > > RecreatedSimplifiedInstances;
    
    obtainReversedSimplifiedInstances(SimplifiedInstances_New); //so that smaller triangles appear first
 
    // Take each entry from SimplifiedInstances_New, concatenate ExistingContext with C_i of each,
    // and push back the tailed triangle at the end of RecreatedSimplifiedInstances
    for(int i = 0; i < SimplifiedInstances_New.size(); i++)
                {
                set<DAGNode*> MyContext = (SimplifiedInstances_New[i]).first;//get C_i
                
                DDNode* MyLMDD = (SimplifiedInstances_New[i]).second;//get f_i
                
                set<DAGNode*> FinalContext;
                
                set_union(MyContext.begin(), MyContext.end(), ExistingContext.begin(), ExistingContext.end(), inserter(FinalContext, FinalContext.begin())); 
		
                RecreatedSimplifiedInstances.push_back(make_pair(FinalContext, MyLMDD));
                }    
    
    // Push back the existing entries in SimplifiedInstances except the first one into RecreatedSimplifiedInstances
    for(int i = 1; i < SimplifiedInstances.size(); i++)
                {
                set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;//get C_i
                
                DDNode* MyLMDD = (SimplifiedInstances[i]).second;//get f_i
                
                RecreatedSimplifiedInstances.push_back(make_pair(MyContext, MyLMDD));
                }    
    
    SimplifiedInstances = RecreatedSimplifiedInstances;
}



// Version of DDManager::conversionToDisjunctionOfTailedTrianglesFollowedByQE, which 
// finds the tailed triangles using SMT solver calls
DDNode* DDManager::conversionToDisjunctionOfTailedTrianglesFollowedByQEUsingSMTSolverCalls(set<string> &VarSetToElim, DDNode* f, map<string, int> &WidthTable)
{
    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nInside DDManager::conversionToDisjunctionOfTailedTrianglesFollowedByQEUsingSMTSolverCalls\n";
        }
    
   TableForDagResolve.clear();

   HashTable <string, DDNode*> TableForAPResolveModified;
   HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

   HashTable <string, DDNode*> TableForQElimModified;
   HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;
    
   vector< pair< set<DAGNode*>, DDNode* > > SimplifiedInstances; //All the simplified instances 
    
   map< int, vector<int> > SimplifiedInstancesHierarchy; //The hierarchy of the simplified instances
   
   set<int> BrokenTailedTriangles;
    
   bool ResultIsFound = convertToSimplifiedInstancesBasedOnHierarchy(VarSetToElim, f, WidthTable, SimplifiedInstances, SimplifiedInstancesHierarchy, PtrTableForAPResolveModified, PtrTableForQElimModified, BrokenTailedTriangles);   
    
    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nAfter convertToSimplifiedInstancesBasedOnHierarchy call\n";
        
        showSimplifiedInstances(SimplifiedInstances);
        
        showHierarchy(SimplifiedInstancesHierarchy);
        }
    
    DDNode* result = createDD(0);
    
    if(!ResultIsFound)
        {
        fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\n\nBenchmark:%s", InFileName.c_str());
        showSimplifiedInstancesInFile(SimplifiedInstances);
        showHierarchyInFile(SimplifiedInstancesHierarchy);
        fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\nHierarchy is solved in the following way");
        fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\n(index, sat/unsat)\n");
        
        QEFromDisjunctionOfTailedTrianglesBasedOnHierarchy(VarSetToElim, f, WidthTable, SimplifiedInstances, SimplifiedInstancesHierarchy, PtrTableForAPResolveModified, PtrTableForQElimModified);  
        }
    else
        {
        fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\nBenchmark:%s", InFileName.c_str());
        showSimplifiedInstancesInFile(SimplifiedInstances);
        showHierarchyInFile(SimplifiedInstancesHierarchy);
        fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\nHierarchy was not solved");
        }
       
    return result;
}


// Returns SimplifiedInstances and SimplifiedInstancesHierarchy such that
// QE from the SimplifiedInstances based on SimplifiedInstancesHierarchy \equiv \exists X. f
bool DDManager::convertToSimplifiedInstancesBasedOnHierarchy(set<string> &VarSetToElim, DDNode* f, map<string, int> &WidthTable, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, map< int, vector<int> > &SimplifiedInstancesHierarchy, HashTable <string, DDNode*> *PtrTableForAPResolveModified, HashTable <string, DDNode*> *PtrTableForQElimModified, set<int> &BrokenTailedTriangles)
{
   struct timeval start_ms, finish_ms;
   unsigned long long int duration_ms;     
  
   int project_depth = 0;      
   int NoOfSMTCalls = 0;
    
   while(NoOfSMTCalls < MaxNoOfSMTCallsInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls)
        {           
        DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
            }
  
        DAGNode*  SatisfiabilityCondition = createDAG("and", original_dag_in_path_based_strategy_in_lmdd_monniaux, NegPartialResult, LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nSatisfiabilityCondition = "<<getDAGStr(SatisfiabilityCondition)<<endl;
            }
        
        
        map<string, string> ModelOfSatisfiabilityCondition;
        
        gettimeofday (&start_ms, NULL);
        
        bool SatisfiabilityConditionIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, ModelOfSatisfiabilityCondition, WidthTable);
        
        gettimeofday (&finish_ms, NULL);
        
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        TotalInitialSMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = TotalInitialSMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls + duration_ms;
                    
        if(SatisfiabilityConditionIsFalse)
            {
            cout<<"\nPi contains the result of QE\n";
            
            return true;
            }
        else
            {
            cout<<"\nPi is not the result of QE\n";
            
            // We have the model of f_dag \wedge \not Pi in Model
			
            vector< pair < set<DAGNode*>, DDNode* > > LocalSimplifiedInstances;		

            int ParentTailedTriangle = -1;	
            
            set<DAGNode*> TempContext;
            
            convertToSimplifiedInstancesUsingSMTSolverBasedOnHierarchy(f, TempContext, LocalSimplifiedInstances, ModelOfSatisfiabilityCondition, WidthTable, false, ParentTailedTriangle, SimplifiedInstancesHierarchy, SimplifiedInstances, BrokenTailedTriangles);

            
            if(checkTimeOut())
                {
                // Time Out !!!
                cout<<"\nWarning!!!The function DDManager::convertToSimplifiedInstancesBasedOnHierarchy has timed out. Its result is not exact\n";
                timed_out = true; // timed_out flag set
                    
                return true;    
                }
            
            
            
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nLocalSimplifiedInstances\n";
                showSimplifiedInstances(LocalSimplifiedInstances);
                }
            
            //Pi := Pi \vee \exists X. LocalSimplifiedInstances[LocalSimplifiedInstances's size - 1];
            
            if(LocalSimplifiedInstances.empty())
                {
                cout<<"\nLocalSimplifiedInstances is empty in function DDManager::convertToSimplifiedInstancesBasedOnHierarchy\n";
                exit(1);
                }
            
           set<DAGNode*> MyContext = (LocalSimplifiedInstances[LocalSimplifiedInstances.size()-1]).first;
        
           DDNode* MyLMDD = (LocalSimplifiedInstances[LocalSimplifiedInstances.size()-1]).second;
            
           SizeOfTrianglesInEagerConversionToDisjunctions.push_back(getSizeOfDD(MyLMDD));
           
           SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("s");
            
            NumberOfTailedTrianglesToQE++;
            
            NumberOfTailedTrianglesToQEInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls++;
            
            DAGNode* Pi_i;            
            
            set<DAGNode*> FreeContext;
            set<DAGNode*> BoundContext;
            
            getFreeAndBoundConstraintsForMultipleVariables(MyContext, VarSetToElim, FreeContext, BoundContext, WidthTable);
            
            DAGNode* ConjunctionOfFreeContext = getConjunctionFromSet(FreeContext);

            
            gettimeofday (&start_ms, NULL);
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                 showAPSet("FreeContext", FreeContext);
                 showAPSet("BoundContext", BoundContext);
                 cout<<"\nMyLMDD = "<<getDAGStr(createDAGFromDD(MyLMDD))<<endl;   
                }  
            
           
            Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(MyLMDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
                        
            gettimeofday (&finish_ms, NULL);
        
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
            TotalInitialQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = TotalInitialQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls + duration_ms;
            
            Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfFreeContext, LOGICAL_SYMBOL, 1));
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                }                   
                  
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
   
            
            //Remove LocalSimplifiedInstances[LocalSimplifiedInstances's size - 1] from LocalSimplifiedInstances;
            
            LocalSimplifiedInstances.erase(LocalSimplifiedInstances.begin()+LocalSimplifiedInstances.size()-1);
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nLast entry removed from LocalSimplifiedInstances\n";
                }
            
            if(LocalSimplifiedInstances.size() > 0)
			{
			updateHierarchy(ParentTailedTriangle, SimplifiedInstances.size(), LocalSimplifiedInstances.size(), SimplifiedInstancesHierarchy);
                        
                        BrokenTailedTriangles.insert(ParentTailedTriangle);//ParentTailedTriangle is broken
	
			//Insert LocalSimplifiedInstances into SimplifiedInstances
                        
                        for(int i=0; i<LocalSimplifiedInstances.size(); i++)
                            {
                            set<DAGNode*> ContextFromLocalSimplifiedInstances = (LocalSimplifiedInstances[i]).first;
                            DDNode* LMDDFromLocalSimplifiedInstances = (LocalSimplifiedInstances[i]).second;
                            
                            SimplifiedInstances.push_back(make_pair(ContextFromLocalSimplifiedInstances, LMDDFromLocalSimplifiedInstances));
                            }//for ends here
                        
                        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                            {
                            cout<<"\nSimplifiedInstances and Hierarchy are updated to\n";
                            showSimplifiedInstances(SimplifiedInstances);
                            showHierarchy(SimplifiedInstancesHierarchy);
                            }
                                                
			}//if ends here            
            }//else        
        
        NoOfSMTCalls++;
        
        NoOfInitialSMTCallsInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCallsCommitted++;
        }//while ends here
   
   return false;
}//function ends here




// Variant of the function convertToSimplifiedInstancesUsingSMTSolver which forms tailed triangles based
// on existing set of tailed triangles
void DDManager::convertToSimplifiedInstancesUsingSMTSolverBasedOnHierarchy(DDNode* f, set<DAGNode*> Context, vector< pair< set<DAGNode*>, DDNode* > > &LocalSimplifiedInstances, map<string, string> &Model_Of_SatCondition, map<string, int> &WidthTable, bool ParentTailedTriangleEncountered, int &ParentTailedTriangle, map< int, vector<int> > &SimplifiedInstancesHierarchy, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, set<int> &BrokenTailedTriangles)
{
    if(!ParentTailedTriangleEncountered && !SimplifiedInstancesHierarchy.empty())
       {
	obtainParentTriangle(f, Context, SimplifiedInstances, ParentTailedTriangleEncountered, ParentTailedTriangle, BrokenTailedTriangles);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            if(ParentTailedTriangleEncountered)
                {
                cout<<"\nParentTailedTriangle = "<<ParentTailedTriangle<<endl;
                }
            }//if  
	}//if
    
    string SideSelected;
    
    SideSelected = obtainPathCountsUsingSMTSolver(f, Model_Of_SatCondition, WidthTable);
    
    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
       {
       cout<<"\nSideSelected = "<<SideSelected<<endl;
       }   
    
    if(checkTimeOut())
        {
        // Time Out !!!
        cout<<"\nWarning!!!The function DDManager::convertToSimplifiedInstancesUsingSMTSolverBasedOnHierarchy  has timed out. Its result is not exact\n";
        timed_out = true; // timed_out flag set
                    
        return;    
        }
    
    if(SideSelected == "none" || getSizeOfDD(f) <= MaxTriangleSizeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls)//1-terminal reached
        // or we have encountered a node with size <= t_1
        {
         if(isFalseDD(f))
            {
            cout<<"\nSatisfiable path leading to 0-node in DDManager::convertToSimplifiedInstancesUsingSMTSolverBasedOnHierarchy\n";
            exit(1);
            }
        
         if(ParentTailedTriangleEncountered || SimplifiedInstancesHierarchy.empty())
            {
            LocalSimplifiedInstances.push_back(make_pair(Context, f));          
            }
        }    
    else if(SideSelected == "right")//Let us follow the right branch and consider the sub-problem at left branch as a simplifiedInstance
        {
        set<DAGNode*> RightContext;
        set<DAGNode*> LeftContext;
        
        DAGNode* C = f->getPredicate();
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nC = "<<getDAGStr(C)<<endl;
            }
        
        copySetNew(RightContext, Context);
        RightContext.insert(C);
            
        copySetNew(LeftContext, Context);
        LeftContext.insert(negateConstraint_With_LMISupport(C));    
        
        //\exists X. (f->getLeft() /\ LeftContext) is a simplifiedInstance
        
       if(ParentTailedTriangleEncountered || SimplifiedInstancesHierarchy.empty())
            {
            if(!isFalseDD(f->getLeft()))
                {
                LocalSimplifiedInstances.push_back(make_pair(LeftContext, f->getLeft()));
                }
            }
        
        convertToSimplifiedInstancesUsingSMTSolverBasedOnHierarchy(f->getRight(), RightContext, LocalSimplifiedInstances, Model_Of_SatCondition, WidthTable, ParentTailedTriangleEncountered, ParentTailedTriangle, SimplifiedInstancesHierarchy, SimplifiedInstances, BrokenTailedTriangles);
        }
    else //Let us follow the left branch and consider the sub-problem at right branch as a simplifiedInstance
        {
        set<DAGNode*> RightContext;
        set<DAGNode*> LeftContext;
        
        DAGNode* C = f->getPredicate();
        
         if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nC = "<<getDAGStr(C)<<endl;
            }
        
        copySetNew(RightContext, Context);
        RightContext.insert(C);
            
        copySetNew(LeftContext, Context);
        LeftContext.insert(negateConstraint_With_LMISupport(C));    
        
        //\exists X. (f->getRight() /\ RightContext) is a simplifiedInstance
        
        if(ParentTailedTriangleEncountered || SimplifiedInstancesHierarchy.empty())
                {
                if(!isFalseDD(f->getRight()))
                    {
                    LocalSimplifiedInstances.push_back(make_pair(RightContext, f->getRight()));
                    }
                }
        
        convertToSimplifiedInstancesUsingSMTSolverBasedOnHierarchy(f->getLeft(), LeftContext, LocalSimplifiedInstances, Model_Of_SatCondition, WidthTable, ParentTailedTriangleEncountered, ParentTailedTriangle, SimplifiedInstancesHierarchy, SimplifiedInstances, BrokenTailedTriangles);
        }
}//function ends here



// Check if DDNode* f with context "Context" is already existing tailed triangle
void DDManager::obtainParentTriangle(DDNode* f, set<DAGNode*> &Context, vector< pair < set<DAGNode*>, DDNode* > > &SimplifiedInstances, bool &ParentTailedTriangleEncountered, int &ParentTailedTriangle, set<int> &BrokenTailedTriangles)
{
for(int i=0; i<SimplifiedInstances.size(); i++)
	{	
	//Take each entry (f_i, C_i) from SimplifiedInstances
    
        set<DAGNode*> Context_i = SimplifiedInstances[i].first;
        
        DDNode* f_i = SimplifiedInstances[i].second;

        
        if(BrokenTailedTriangles.find(i) != BrokenTailedTriangles.end())//f_i is a broken tailed triangle
                {
                continue;
                }
        else if(f_i != f)
		{
		continue;
		}
        else if(!setsEqual(Context_i, Context))
		{
		continue;
		}
        else//f_i = f and Context_i = Context
		{
		ParentTailedTriangleEncountered = true;
			
		ParentTailedTriangle = i;

		return;
		}
	}//for
}//function


// Update the hierarchy SimplifiedInstancesHierarchy 
void DDManager::updateHierarchy(int ParentTailedTriangle, int SimplifiedInstancesSize, int LocalSimplifiedInstancesSize,  map< int, vector<int> > &SimplifiedInstancesHierarchy)
{
    //Insert ParentTailedTriangle ---> <SimplifiedInstancesSize + 0, ..., SimplifiedInstancesSize + LocalSimplifiedInstancesSize - 1> into Hierarchy

    vector<int> Children;    
    for(int i=0; i<LocalSimplifiedInstancesSize; i++)
        {
        Children.push_back(SimplifiedInstancesSize + i);
        }
    SimplifiedInstancesHierarchy.insert(make_pair(ParentTailedTriangle, Children));
}


// Show the hierarchy SimplifiedInstancesHierarchy 
void DDManager::showHierarchy(map< int, vector<int> > &SimplifiedInstancesHierarchy)
{
   cout<<"\nSimplifiedInstancesHierarchy\n";
   for(map< int, vector<int> >::iterator it = SimplifiedInstancesHierarchy.begin(); it != SimplifiedInstancesHierarchy.end(); it++)
    {
    cout<<"\nParent ID = "<<it->first<<endl;
    
    vector<int> Children;
    Children = it->second;
    cout<<"\nChildren\n";
    
    for(int i=0; i<Children.size(); i++)
        {
        cout<<Children[i]<<"\t";
        }    
    }//for
}//function


// Performs QE from SimplifiedInstances based on Hierarchy and returns the result in Pi
void DDManager::QEFromDisjunctionOfTailedTrianglesBasedOnHierarchy(set<string> &VarSetToElim, DDNode* f, map<string, int> &WidthTable, vector< pair < set<DAGNode*>, DDNode* > > &SimplifiedInstances, map< int, vector<int> > &SimplifiedInstancesHierarchy, HashTable <string, DDNode*> *PtrTableForAPResolveModified, HashTable <string, DDNode*> *PtrTableForQElimModified)
{
   struct timeval start_ms, finish_ms;
   unsigned long long int duration_ms;     
  
   int project_depth = 0;      
    
    vector<int> TTriangleVector;

    //TTriangleVector = Hierarchy[-1];
    map< int, vector<int> >::iterator start_it = SimplifiedInstancesHierarchy.find(-1);
    
    if(start_it == SimplifiedInstancesHierarchy.end())
        {
        cout<<"\nNo entry for -1 in SimplifiedInstancesHierarchy in function DDManager::QEFromDisjunctionOfTailedTrianglesBasedOnHierarchy\n";
        exit(1);
        }
    
   TTriangleVector = start_it->second;
   
   if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nTTriangleVector\n";
        showVector(TTriangleVector);
        }
   
   //TTriangleVector.reverse();
   reverseVector(TTriangleVector);
   
   if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
        {
        cout<<"\nTTriangleVector\n";
        showVector(TTriangleVector);
        }
   
   while(TTriangleVector.size() > 0)
   {
       int TTriangle_id = TTriangleVector[0];
       
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nTTriangleVector[0] = TTriangle_id = "<<TTriangle_id<<endl;
            }
       
       map< int, vector<int> >::iterator my_it = SimplifiedInstancesHierarchy.find(TTriangle_id);
       
       if(my_it == SimplifiedInstancesHierarchy.end())//This TTriangle is not divided
        {
          //f_i \wedge C_i := SimplifiedInstances[TTriangle_id]; 
           
         set<DAGNode*> MyContext = SimplifiedInstances[TTriangle_id].first;
        
         DDNode* MyLMDD = SimplifiedInstances[TTriangle_id].second;
         
         SizeOfTrianglesInEagerConversionToDisjunctions.push_back(getSizeOfDD(MyLMDD));
         
         //if(f_i \wedge C_i \wedge \not Pi is sat)
	 //		{
	 //		Pi = Pi \vee QEFromTailedTriangle( \exists X. (f_i\wedge C_i) );
	 //		}
        
        //Let's see if \exists VarSetToElim. (MyLMDD /\ MyContext) is needed or not
        
        // PathCondition = MyLMDD /\ MyContext
        DAGNode* PathCondition = createDAG("and", createDAGFromDD(MyLMDD), getConjunctionFromSet(MyContext), LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
            }
   
        DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
            }
  
        DAGNode*  SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
        
        map<string, string> ModelOfSatisfiabilityCondition;
        
        gettimeofday (&start_ms, NULL);
        
        bool SatisfiabilityConditionIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, ModelOfSatisfiabilityCondition, WidthTable);
        
        gettimeofday (&finish_ms, NULL);
        
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        TotalHierarchySMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = TotalHierarchySMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls + duration_ms;
            
        if(SatisfiabilityConditionIsFalse)
            {
            cout<<"\nexists X. (C_i and f_i) => Pi\n";
            
            SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("u");
            
            NumberOfTailedTrianglesUnsatInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls++;
            
            fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "(%d, u)\n", TTriangle_id);
            }
        else
            {
            cout<<"\nNOT(exists X. (C_i and f_i) => Pi)\n";
            
            SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("s");
            
            NumberOfTailedTrianglesToQE++;
            
            NumberOfTailedTrianglesToQEInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls++;
            
            fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "(%d, s)\n", TTriangle_id);
            
            DAGNode* Pi_i;
            
            
            set<DAGNode*> FreeContext;
            set<DAGNode*> BoundContext;
            
            getFreeAndBoundConstraintsForMultipleVariables(MyContext, VarSetToElim, FreeContext, BoundContext, WidthTable);
            
            DAGNode* ConjunctionOfFreeContext = getConjunctionFromSet(FreeContext);

            
            gettimeofday (&start_ms, NULL);
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                 showAPSet("FreeContext", FreeContext);
                 showAPSet("BoundContext", BoundContext);
                 cout<<"\nMyLMDD = "<<getDAGStr(createDAGFromDD(MyLMDD))<<endl;   
                }  
            
            Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(MyLMDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            
            
            gettimeofday (&finish_ms, NULL);
        
            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
            TotalHierarchyQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = TotalHierarchyQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls + duration_ms;
            
            Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfFreeContext, LOGICAL_SYMBOL, 1));
            
            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                }                   
                  
            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
            }//if the tailed triangle is required
        
        //Remove TTriangleVector[0]
        TTriangleVector.erase(TTriangleVector.begin()+0);        
        }//This TTriangle is not divided
       else //This TTriangle is divided
       {
           //vector<int> NewTTriangleVector = Hierarchy[TTriangle_id];
           
           vector<int> NewTTriangleVector;
           
           NewTTriangleVector = my_it->second;
           
           if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nNewTTriangleVector\n";
                showVector(NewTTriangleVector);
                }
           
           //TTriangle_id is broken into sub-problems in NewTTriangleVector
           
           if(!useDividedTailedTriangles())//Let's do at this level itself
            {
                  if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nLet's do at this level itself\n";  
                    }
               
                 //f_i \wedge C_i := SimplifiedInstances[TTriangle_id]; 
           
                 set<DAGNode*> MyContext = SimplifiedInstances[TTriangle_id].first;

                 DDNode* MyLMDD = SimplifiedInstances[TTriangle_id].second;

                 SizeOfTrianglesInEagerConversionToDisjunctions.push_back(getSizeOfDD(MyLMDD));

                 //if(f_i \wedge C_i \wedge \not Pi is sat)
                 //		{
                 //		Pi = Pi \vee QEFromTailedTriangle( \exists X. (f_i\wedge C_i) );
                 //		}

                //Let's see if \exists VarSetToElim. (MyLMDD /\ MyContext) is needed or not

                // PathCondition = MyLMDD /\ MyContext
                DAGNode* PathCondition = createDAG("and", createDAGFromDD(MyLMDD), getConjunctionFromSet(MyContext), LOGICAL_SYMBOL, 1);

                if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
                    }

                DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);

                if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
                    }

                DAGNode*  SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);

                map<string, string> ModelOfSatisfiabilityCondition;

                gettimeofday (&start_ms, NULL);

                bool SatisfiabilityConditionIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, ModelOfSatisfiabilityCondition, WidthTable);

                gettimeofday (&finish_ms, NULL);

                duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
                duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
                TotalHierarchySMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = TotalHierarchySMTTimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls + duration_ms;

                if(SatisfiabilityConditionIsFalse)
                    {
                    cout<<"\nexists X. (C_i and f_i) => Pi\n";

                    SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("u");
                    
                    NumberOfTailedTrianglesUnsatInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls++;
                    
                    fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "(%d, u)\n", TTriangle_id);
                    }
                else
                    {
                    cout<<"\nNOT(exists X. (C_i and f_i) => Pi)\n";

                    SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("s");

                    NumberOfTailedTrianglesToQE++;
                    
                    NumberOfTailedTrianglesToQEInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls++;
                    
                    fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "(%d, s)\n", TTriangle_id);

                    DAGNode* Pi_i;


                    set<DAGNode*> FreeContext;
                    set<DAGNode*> BoundContext;

                    getFreeAndBoundConstraintsForMultipleVariables(MyContext, VarSetToElim, FreeContext, BoundContext, WidthTable);

                    DAGNode* ConjunctionOfFreeContext = getConjunctionFromSet(FreeContext);


                    gettimeofday (&start_ms, NULL);

                    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                        {
                         showAPSet("FreeContext", FreeContext);
                         showAPSet("BoundContext", BoundContext);
                         cout<<"\nMyLMDD = "<<getDAGStr(createDAGFromDD(MyLMDD))<<endl;   
                        }  

                    Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(MyLMDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);


                    gettimeofday (&finish_ms, NULL);

                    duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
                    duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
                    TotalHierarchyQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls = TotalHierarchyQETimeInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls + duration_ms;

                    Pi_i = quickSimplify_WithLMISupport(createDAG("and", Pi_i, ConjunctionOfFreeContext, LOGICAL_SYMBOL, 1));

                    if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                        {
                        cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                        }                   

                    PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
                    }//if the tailed triangle is required

                //Remove TTriangleVector[0]
                TTriangleVector.erase(TTriangleVector.begin()+0);    
            }//we have decided to proceed with the original tailed triangle
           else
            {
                //NewTTriangleVector.reverse();
                reverseVector(NewTTriangleVector);
                
                //Remove TTriangleVector[0];
               TTriangleVector.erase(TTriangleVector.begin()+0);    
               
               //Insert content of NewTTriangleVector at position 0;
               recreateTailedTriangleVector(TTriangleVector, NewTTriangleVector);
               
               if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nTTriangleVector\n";
                    showVector(TTriangleVector);
                    }
            }//we have decided to proceed with the new set of tailed triangles
       }//This TTriangle is divided       
   }//while ends   
}//function ends 



//Reverse the given vector
void DDManager::reverseVector(vector<int> &MyVector)
{
    vector<int> ReversedVector;
    
    for(int i=MyVector.size()-1; i>=0; i--)
        {
        ReversedVector.push_back(MyVector[i]);
        }
    
    MyVector = ReversedVector;
}


//Recreate TTriangleVector by inserting the content of NewTTriangleVector at position 0;
void DDManager::recreateTailedTriangleVector(vector<int> &TTriangleVector, vector<int> &NewTTriangleVector)
{
    vector<int> RecreatedVector;
    
    for(int i=0; i<NewTTriangleVector.size(); i++)
        {
        RecreatedVector.push_back(NewTTriangleVector[i]);
        }
    
    for(int i=0; i<TTriangleVector.size(); i++)
        {
        RecreatedVector.push_back(TTriangleVector[i]);
        }
    
    TTriangleVector = RecreatedVector;
}


// Decide if we want to proceed with the original tailed triangle or 
// divided versions of the tailed triangle
bool DDManager::useDividedTailedTriangles()
{
    if(StrategyForChoosingInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls == "AlwaysChooseOriginal")
        {
        return false;
        }
    else if(StrategyForChoosingInConversionToDisjunctionOfTailedTrianglesUsingSMTSolverCalls == "AlwaysChooseSplit")
        {
        return true;
        }
    else
        {
        cout<<"\nUnknown strategy in DDManager::useDividedTailedTriangles\n";
        exit(1);
        }
}



// Show the hierarchy SimplifiedInstancesHierarchy in the file ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp
void DDManager::showHierarchyInFile(map< int, vector<int> > &SimplifiedInstancesHierarchy)
{   
   fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\nHierarchy"); 
   for(map< int, vector<int> >::iterator it = SimplifiedInstancesHierarchy.begin(); it != SimplifiedInstancesHierarchy.end(); it++)
    {
    fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\n%d:", it->first);
    
    vector<int> Children;
    Children = it->second;
    
    for(int i=0; i<Children.size(); i++)
        {
        fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "%d,", Children[i]);
        }//for    
    }//for
}//function


// Show the simplified instances in the file ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp
void DDManager::showSimplifiedInstancesInFile(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances)
{
    fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\nTailed triangles\nindex:(tail size, triangle size)");    
    for(int i=0; i < SimplifiedInstances.size(); i++)
    {
        set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;
        DDNode* Myf = (SimplifiedInstances[i]).second;
        
        fprintf(ConversionToDisjunctionOfProblemsWithSMTSolverHierarchyFp, "\n%d:(%d, %d)", i, MyContext.size(), getSizeOfDD(Myf));
    }
}



// Version of DDManager::QEFromDisjunctionOfTailedTriangles, which collects consecutive sat 
// C_i \wedge f_i's and performs QE on \vee (f_i\wedge C_i)
DDNode* DDManager::QEFromDisjunctionOfTailedTriangles_CollectConsecutiveSatTailedTriangles(vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, set<string> &VarSetToElim, map<string, int> &WidthTable)
{
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;   
    
 TableForDagResolve.clear();

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;
  
  int project_depth = 0;
  
  vector<int> CollectedSatTailedTriangles;

  for(int i = SimplifiedInstances.size()-1; i >= 0; i--)//Let's do them in reverse order
  // Simpler instances are towards the end of vector
    {
       if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nConsidering the tailed triangle "<<i<<endl;
            }
      
        set<DAGNode*> MyContext = (SimplifiedInstances[i]).first;
        
        DDNode* MyLMDD = (SimplifiedInstances[i]).second;
        
        SizeOfTrianglesInEagerConversionToDisjunctions.push_back(getSizeOfDD(MyLMDD));
        
        //Let's see if \exists VarSetToElim. (MyLMDD /\ MyContext) is needed or not
        
        // PathCondition = MyLMDD /\ MyContext
        DAGNode* PathCondition = createDAG("and", createDAGFromDD(MyLMDD), getConjunctionFromSet(MyContext), LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nPathCondition = "<<getDAGStr(PathCondition)<<endl;
            }
   
        DAGNode* NegPartialResult = createDAG("not", PartialResult_In_LMDDWithMonniauxTypeSimplifications, LOGICAL_SYMBOL, 1);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nNegPartialResult = "<<getDAGStr(NegPartialResult)<<endl;
            }
  
        DAGNode*  SatisfiabilityCondition = createDAG("and", PathCondition, NegPartialResult, LOGICAL_SYMBOL, 1);
        
        map<string, string> ModelOfSatisfiabilityCondition;
        
        gettimeofday (&start_ms, NULL);
        
        bool SatisfiabilityConditionIsFalse = getModelGeneral_WithSupportForLMIs(SatisfiabilityCondition, ModelOfSatisfiabilityCondition, WidthTable);
        
        gettimeofday (&finish_ms, NULL);
        
        duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
        duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
        TotalSMTTimeInConversionToDisjunctionOfTailedTriangles = TotalSMTTimeInConversionToDisjunctionOfTailedTriangles + duration_ms;
            
        if(SatisfiabilityConditionIsFalse)
            {
            cout<<"\nexists X. (C_i and f_i) => Pi\n";
            
            SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("u");
            
            // Encountered unsat tailed triangle
            // Let's do all the collected sat tailed triangles
            
            if(CollectedSatTailedTriangles.size() > 0)
                {
                //Let's convert the collected tailed triangles to a single DD
                DDNode* SingleDD = convertVectorOfTailedTrianglesToSingleDD(CollectedSatTailedTriangles, SimplifiedInstances, WidthTable);
                
                NumberOfTimesCollectedTailedTrianglesToQE++;
                
                DAGNode* Pi_i;
                
                gettimeofday (&start_ms, NULL);
                
                if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nSingleDD = "<<getDAGStr(createDAGFromDD(SingleDD))<<endl;   
                    }  
                
                set<DAGNode*> BoundContext;
                
                Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(SingleDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);
            
                gettimeofday (&finish_ms, NULL);
        
                duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
                duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
                TotalQETimeInConversionToDisjunctionOfTailedTriangles = TotalQETimeInConversionToDisjunctionOfTailedTriangles + duration_ms;
                   
                if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                    {
                    cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                    }                   
                  
                PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);
                
                CollectedSatTailedTriangles.clear();
                }//if(CollectedSatTailedTriangles.size() > 0)
            }//if(SatisfiabilityConditionIsFalse)
        else
            {
            cout<<"\nNOT(exists X. (C_i and f_i) => Pi)\n";
            
            SatUnsatOfTrianglesInEagerConversionToDisjunctions.push_back("s");
            
            NumberOfTailedTrianglesToQE++;
            
            CollectedSatTailedTriangles.push_back(i);
            }//if(!SatisfiabilityConditionIsFalse)
    }//for ends here
  
  if(CollectedSatTailedTriangles.size() > 0)
            {
            //Let's convert the collected tailed triangles to a single DD
            DDNode* SingleDD = convertVectorOfTailedTrianglesToSingleDD(CollectedSatTailedTriangles, SimplifiedInstances, WidthTable);

            NumberOfTimesCollectedTailedTrianglesToQE++;

            DAGNode* Pi_i;

            gettimeofday (&start_ms, NULL);

            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nSingleDD = "<<getDAGStr(createDAGFromDD(SingleDD))<<endl;   
                }  

            set<DAGNode*> BoundContext;

            Pi_i = QE_LMDD_ForMultipleVariables_UnderGivenContext(SingleDD, BoundContext, VarSetToElim, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified, project_depth);

            gettimeofday (&finish_ms, NULL);

            duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
            duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
            TotalQETimeInConversionToDisjunctionOfTailedTriangles = TotalQETimeInConversionToDisjunctionOfTailedTriangles + duration_ms;

            if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
                {
                cout<<"\nPi_i = "<<getDAGStr(Pi_i)<<"\n";    
                }                   

            PartialResult_In_LMDDWithMonniauxTypeSimplifications = createDAG("or", PartialResult_In_LMDDWithMonniauxTypeSimplifications, Pi_i, LOGICAL_SYMBOL, 1);

            CollectedSatTailedTriangles.clear();
            }//if(CollectedSatTailedTriangles.size() > 0)
  
  DDNode* result = createDD(0);// result is actually in PartialResult_In_LMDDWithMonniauxTypeSimplifications
  // The DD part of the result is always false
  
  return result;
}


//Function to convert a vector of sat. tailed triangles to a single DD
DDNode* DDManager::convertVectorOfTailedTrianglesToSingleDD(vector<int> &CollectedSatTailedTriangles, vector< pair< set<DAGNode*>, DDNode* > > &SimplifiedInstances, map<string, int> &WidthTable)
{
    DDNode* FinalDD = createDD(0);
    
    for(int i=0; i < CollectedSatTailedTriangles.size(); i++)
    {
        int IndexToSimplifiedInstances = CollectedSatTailedTriangles[i];
        
         if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nIndexToSimplifiedInstances =  "<<IndexToSimplifiedInstances<<endl;
            }
        
        set<DAGNode*> MyContext = (SimplifiedInstances[IndexToSimplifiedInstances]).first;
        
        DDNode* MyLMDD = (SimplifiedInstances[IndexToSimplifiedInstances]).second;
        
        DAGNode* MyConjunction = getConjunctionFromSet(MyContext);
        
        DDNode* DDFromConjunction = createDDFromDAGWithoutImplicationCheck(MyConjunction, WidthTable);
        
        DDNode* MyTTriangleDD = apply("and", DDFromConjunction, MyLMDD);
        
        if(debug_check_in_ConvertProblemIntoDisjunctionOfTailedTriangles_strategy)
            {
            cout<<"\nC_i = "<<getDAGStr(MyConjunction)<<endl;
            cout<<"\nf_i = "<<getDAGStr(createDAGFromDD(MyLMDD))<<endl;
            cout<<"\nC_iwedge f_i = "<<getDAGStr(createDAGFromDD(MyTTriangleDD))<<endl;
            }
        
        FinalDD = apply("or", FinalDD, MyTTriangleDD);
    }
    
    return FinalDD;
    
}

// Newly added on 7 May 2012 ends here


// Checks if \exists VarToElim. (PhiDag /\ XiDag) \equiv \exists VarToElim. (PhiDag) using BDDs
bool DDManager::quantifiedFormulaeAreEquivalentUsingBlastingAndCudd(DAGNode* PhiDag, DAGNode* XiDag, string VarToElim, map<string, int> &WidthTable, unsigned long long int &BlastTimeLocal, unsigned long long int &DDTimeLocal, unsigned long long int &QETimeLocal)
{
  bool apriori_fix_order_of_variables_in_bdd = false;  
  bool reinitializing_cudd = true;  
  bool set_time_limit = true;
  unsigned int cudd_node_limit = 100000000;
  
  bool debug = false;  
  
  bool QuantifiedFormulaeAreEquivalent;
  ofstream *outfile = new ofstream();
  
  if(debug)
    {     
     //outfile->open("blasted_DAGFile.txt");
  
     //*outfile<<"\nPhiDag\n";
     //dm.printDAG(PhiDag, outfile);
     
     //*outfile<<"\nXiDag\n";
     //dm.printDAG(XiDag, outfile);
     
     //*outfile<<"\nVarToElim = "<<VarToElim<<endl;
     
     cout<<"\nPhiDag\n";
     cout<<endl<<getDAGStr(PhiDag)<<endl;
     
     cout<<"\nXiDag\n";
     cout<<endl<<getDAGStr(XiDag)<<endl;
     
     cout<<"\nVarToElim = "<<VarToElim<<endl;
     cout<<"\nSize of PhiDAG = "<<dm.getSizeOfDAG(PhiDag)<<endl;
     cout<<"\nSize of XiDAG = "<<dm.getSizeOfDAG(XiDag)<<endl;
    }
  
  fprintf(TimeLayer2ChecksFp, "\n%d\t%d", dm.getSizeOfDAG(PhiDag), dm.getSizeOfDAG(XiDag));
  
  //cout<<"\nBlasting the formulae...\n";
  
  
  if(PrintBlastTimes)
        {
        fprintf(LMIQEOutputDataFp, "\nDetails of blasting\n");
        fprintf(LMIQEOutputDataFp, "\nPhiDag = ");
        fprintf(LMIQEOutputDataFp, "%s", getDAGStr(PhiDag).c_str());
        fprintf(LMIQEOutputDataFp, "\nXiDag = ");
        fprintf(LMIQEOutputDataFp, "%s", getDAGStr(XiDag).c_str());
        }
  
   struct timeval blast_start_ms, blast_finish_ms;                  
   gettimeofday (&blast_start_ms, NULL); 
  
  // Let us blast PhiDag and XiDag first
   DAGNode* BlastedPhiDag = getBitBlastedDAG(PhiDag);
   
   DAGNode* BlastedXiDag = getBitBlastedDAG(XiDag);
   
   gettimeofday (&blast_finish_ms, NULL);
   BlastTimeLocal = blast_finish_ms.tv_sec * 1000 + blast_finish_ms.tv_usec / 1000;
   BlastTimeLocal -= blast_start_ms.tv_sec * 1000 + blast_start_ms.tv_usec / 1000; 
   
   TotalLayer2CheckTimeOnBlasting = TotalLayer2CheckTimeOnBlasting + BlastTimeLocal;
      
   if(PrintBlastTimes)
        {
        fprintf(LMIQEOutputDataFp, "\nTotal blasting time = %llu\n", BlastTimeLocal);
        }
   
    fprintf(TimeLayer2ChecksFp, "\t%d\t%d", dm.getSizeOfDAG(BlastedPhiDag), dm.getSizeOfDAG(BlastedXiDag));
   
   if(debug)
    {
     //*outfile<<"\nBlastedPhiDag\n";
     //dm.printDAG(BlastedPhiDag, outfile);
     
     //*outfile<<"\nBlastedXiDag\n";
     //dm.printDAG(BlastedXiDag, outfile);
     
     cout<<"\nSize of BlastedPhiDAG = "<<dm.getSizeOfDAG(BlastedPhiDag)<<endl;
     cout<<"\nSize of BlastedXiDAG = "<<dm.getSizeOfDAG(BlastedXiDag)<<endl;  
       
     outfile->close();
    }
      
   //cout<<"\nCreating BDDs for the formulae...\n";
   
   struct timeval dd_start_ms, dd_finish_ms;                  
   gettimeofday (&dd_start_ms, NULL); 
   
   // re-initializing the bdd manager
   if(reinitializing_cudd)
    {
       // Code to reinitialize the CUDD manager and the maps; all previous results are lost. 
        manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
        NameIndexMap.clear();
        IndexNameMap.clear();
        bdd_index = 0;
    }  
   
     
   // Let's create BDDs for BlastedPhiDag and BlastedXiDag
   if(apriori_fix_order_of_variables_in_bdd)//Let's a-priori fix the order of variables in bdd
    {
        AprioriFixOrderOfVariablesInBDD(PhiDag, XiDag, WidthTable); 
    }   
   
    set<string> VarSetToElim;
    VarSetToElim.insert(VarToElim);
    
    map<DAGNode*,DdNode*> DPTable;
    set<int> BooleanVarSetToElim;

    DdNode *BDDForBlastedPhiDag = createBDDInDDManager(BlastedPhiDag, DPTable, VarSetToElim, BooleanVarSetToElim);
    
    
    if(debug)
        {
            //cout<<"\nBDDForBlastedPhiDag\n";
            //Cudd_PrintDebug(manager, BDDForBlastedPhiDag, 0, 4);

            cout<<endl<<"NameIndexMap"<<endl;
            for(map<string, int>::iterator it=NameIndexMap.begin(); it!=NameIndexMap.end(); it++)
            	{
            	cout<<it->first<<"\t"<<it->second<<endl;
            	}
            cout<<"\nbdd_index = "<<bdd_index<<endl;

            cout<<endl<<"BooleanVarSetToElim"<<endl;
            for(set<int>::iterator it=BooleanVarSetToElim.begin(); it!=BooleanVarSetToElim.end(); it++)
            	{
            	cout<<*it<<endl;
            	}
            cout<<"\nSize of BDD for BlastedPhiDAG = "<<Cudd_DagSize(BDDForBlastedPhiDag)<<endl;
     
        }
    
    
    DdNode *BDDForBlastedXiDag = createBDDInDDManager(BlastedXiDag, DPTable, VarSetToElim, BooleanVarSetToElim);
        
    if(debug)
        {
            //cout<<"\nBDDForBlastedXiDag\n";
            //Cudd_PrintDebug(manager, BDDForBlastedXiDag, 0, 4);

            cout<<endl<<"NameIndexMap"<<endl;
            for(map<string, int>::iterator it=NameIndexMap.begin(); it!=NameIndexMap.end(); it++)
            	{
            	cout<<it->first<<"\t"<<it->second<<endl;
            	}
            cout<<"\nbdd_index = "<<bdd_index<<endl;

            cout<<endl<<"BooleanVarSetToElim"<<endl;
            for(set<int>::iterator it=BooleanVarSetToElim.begin(); it!=BooleanVarSetToElim.end(); it++)
            	{
            	cout<<*it<<endl;
            	}
            cout<<"\nSize of BDD for BlastedXiDAG = "<<Cudd_DagSize(BDDForBlastedXiDag)<<endl;
        }
    
    DdNode *BDDForBlastedPhiAndXiDag  = Cudd_bddAnd(manager, BDDForBlastedPhiDag, BDDForBlastedXiDag);
    Cudd_Ref(BDDForBlastedPhiAndXiDag);
    
    if(debug)
        {
            //cout<<"\nBDDForBlastedPhiAndXiDag\n";
            //Cudd_PrintDebug(manager, BDDForBlastedPhiAndXiDag, 0, 4);
        }    
    
   gettimeofday (&dd_finish_ms, NULL);
   DDTimeLocal = dd_finish_ms.tv_sec * 1000 + dd_finish_ms.tv_usec / 1000;
   DDTimeLocal -= dd_start_ms.tv_sec * 1000 + dd_start_ms.tv_usec / 1000; 
   
   TotalLayer2CheckTimeOnDDManipulation = TotalLayer2CheckTimeOnDDManipulation + DDTimeLocal;
   TotalLayer2CheckTimeOnDDCreation = TotalLayer2CheckTimeOnDDCreation + DDTimeLocal;
   
   fprintf(TimeLayer2ChecksFp, "\t%d\t%d\t%d", Cudd_DagSize(BDDForBlastedPhiDag), Cudd_DagSize(BDDForBlastedXiDag), Cudd_DagSize(BDDForBlastedPhiAndXiDag));
    
    //cout<<"\nDoing QE...\n";
    
    struct timeval qe_start_ms, qe_finish_ms;                  
    gettimeofday (&qe_start_ms, NULL); 

    DdNode *CubeBDD = createCubeBDDInDDManager(BooleanVarSetToElim);

    if(debug)
        {
        //cout<<"\nCubeBDD\n";
        //Cudd_PrintDebug(manager, CubeBDD, 0, 4);
        }
    
    DdNode *QEBDDForBlastedPhiDag;
    
    if(!set_time_limit)
        {
        QEBDDForBlastedPhiDag = Cudd_bddExistAbstract(manager, BDDForBlastedPhiDag, CubeBDD);
         }
    else
        {                
        QEBDDForBlastedPhiDag = Cudd_bddExistAbstractLimit(manager, BDDForBlastedPhiDag, CubeBDD, cudd_node_limit);       
        
        
        if(QEBDDForBlastedPhiDag == NULL)//timed out
            {
            fprintf(TimeLayer2ChecksFp, "\tunknown\tunknown");
            fprintf(TimeLayer2ChecksFp, "\tTO\t%llu\t%llu\tTO\n", BlastTimeLocal, DDTimeLocal);
            fclose(TimeLayer2ChecksFp);
            
            cout<<"\nCudd_bddExistAbstractLimit node limit of " << cudd_node_limit << " reached\n";
            exit(1);
            }
        }
       

    if(debug)
	{
         //cout<<"\nQEBDDForBlastedPhiDag\n";
         //Cudd_PrintDebug(manager, QEBDDForBlastedPhiDag, 0, 4);
        cout<<"\nSize of BDD for QEBlastedPhiDAG = "<<Cudd_DagSize(QEBDDForBlastedPhiDag)<<endl;
        }
    
    DdNode *QEBDDForBlastedPhiAndXiDag;
    
    if(!set_time_limit)
        {
         QEBDDForBlastedPhiAndXiDag  = Cudd_bddExistAbstract(manager, BDDForBlastedPhiAndXiDag, CubeBDD);
        }
    else
        {
               
        QEBDDForBlastedPhiAndXiDag  = Cudd_bddExistAbstractLimit(manager, BDDForBlastedPhiAndXiDag, CubeBDD, cudd_node_limit);
              
        
        if(QEBDDForBlastedPhiAndXiDag == NULL)
            {
            fprintf(TimeLayer2ChecksFp, "\t%d\tunknown", Cudd_DagSize(QEBDDForBlastedPhiDag));
            fprintf(TimeLayer2ChecksFp, "\tTO\t%llu\t%llu\tTO\n", BlastTimeLocal, DDTimeLocal);
            fclose(TimeLayer2ChecksFp);
            cout<<"\nCudd_bddExistAbstractLimit timed out\n";
            exit(1);
            }
        }

    if(debug)
	{
         //cout<<"\nQEBDDForBlastedPhiAndXiDag\n";
         //Cudd_PrintDebug(manager, QEBDDForBlastedPhiAndXiDag, 0, 4);
        cout<<"\nSize of BDD for QEBlastedPhiAndXiDAG = "<<Cudd_DagSize(QEBDDForBlastedPhiAndXiDag)<<endl;
        }
    
   gettimeofday (&qe_finish_ms, NULL);
   QETimeLocal = qe_finish_ms.tv_sec * 1000 + qe_finish_ms.tv_usec / 1000;
   QETimeLocal -= qe_start_ms.tv_sec * 1000 + qe_start_ms.tv_usec / 1000; 
   
   TotalLayer2CheckTimeOnDDManipulation = TotalLayer2CheckTimeOnDDManipulation + QETimeLocal;
   TotalLayer2CheckTimeONQE = TotalLayer2CheckTimeONQE + QETimeLocal;
   
   fprintf(TimeLayer2ChecksFp, "\t%d\t%d", Cudd_DagSize(QEBDDForBlastedPhiDag), Cudd_DagSize(QEBDDForBlastedPhiAndXiDag));
   
   //cout<<"\nQE done\n";
   

    if(QEBDDForBlastedPhiDag == QEBDDForBlastedPhiAndXiDag)    
        {
         if(debug)
            {
             cout<<"\nexists x(phi) equivalent to exists x(phi and xi)\n";             
            }
        QuantifiedFormulaeAreEquivalent = true;
        }
    else
        {
        if(debug)
            {
             cout<<"\nexists x(phi) not equivalent to exists x(phi and xi)\n";            
            }
        QuantifiedFormulaeAreEquivalent = false;
        }
   
   if(reinitializing_cudd)
        {
        // Code to quit CUDD
        Cudd_Quit(manager);
        }
   
   return QuantifiedFormulaeAreEquivalent;

}


// This function fixes the order of variables before creating the bdds
void DDManager::AprioriFixOrderOfVariablesInBDD(DAGNode* PhiDAG, DAGNode* XiDAG, map<string, int> &WidthTable)
{   
    // Let's obtain the bv-level support
    set<string> Support;    
    getDAGNames(PhiDAG, Support);
    getDAGNames(XiDAG, Support);
    
    //check if the widths of the variables in the support are the same    
    set<string>::iterator it = Support.begin();
    int CommonWidth =  getWidthOfVariable(WidthTable,  *it);
    it++;
    
    for(; it != Support.end(); it++)
        {
        string variable = *it;
        int width = getWidthOfVariable(WidthTable,  variable);
        
        if(width != CommonWidth)       
            {
            cout<<"\nError in function DDManager::AprioriFixOrderOfVariablesInBDD! Only cases where all variables have the same width is implemented\n";
            exit(1);
            }
        }
    
    for(int bit_position = 0; bit_position < CommonWidth; bit_position++)
        {
        for(set<string>::iterator it = Support.begin(); it != Support.end(); it++)
            {
             string Variable = *it;
             string Bit_Name = Variable;
             Bit_Name += "_";

	     char index_char[100];
             sprintf(index_char, "%d", bit_position);
	     string index_str = index_char;
             Bit_Name += index_str;
             
             // Let's insert Bit_Name into NameIndexMap and IndexNameMap
             // if it is not already present
             map<string, int>::iterator nim_it = NameIndexMap.find(Bit_Name);
             if(nim_it == NameIndexMap.end()) // This variable is not already present
			{
			int variable_index = bdd_index;
			
			NameIndexMap.insert(make_pair(Bit_Name, variable_index));
			IndexNameMap.insert(make_pair(variable_index, Bit_Name));

			bdd_index++;
			}
            }//for
        }//for
}




// Given "DAG_to_blast" which is a bv-logic formula, 
// this function does bit-blasting + BDD creation
void DDManager::bitBlastAndCreateBDDInDDManager(DAGNode *DAG_to_blast, time_t &duration, unsigned long long int &BDDCreationTime_msec)
{
  cout<<"\nBlasting started\n";
    
  DAGNode* BitBlastedDAG = getBitBlastedDAG(DAG_to_blast);
  
  cout<<"\nBlasting finished\n";
  cout<<"\nCudd creation started\n";
  
  time_t start, end;  
  time(&start); 
  
  struct timeval start_ms, finish_ms;                  
  gettimeofday (&start_ms, NULL); 
  
  map<DAGNode*,DdNode*> DPTable;
  set<int> BooleanVarSetToElim;
  set<string> VarSetToElim;
  
  bool dynamic_reordering_enabled = dynamic_variable_ordering_in_blast_and_output;
  
  if(dynamic_reordering_enabled)
  {
        Cudd_AutodynEnable(manager, CUDD_REORDER_RANDOM);       
  }
  
  DdNode *BDD = createBDDInDDManager(BitBlastedDAG, DPTable, VarSetToElim, BooleanVarSetToElim);
  
  gettimeofday (&finish_ms, NULL);
  BDDCreationTime_msec = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  BDDCreationTime_msec -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000; 
  
  time(&end); 
  duration = end - start;
  
  if(dynamic_reordering_enabled)
  {
        Cudd_AutodynDisable(manager);
  }
  
  cout<<"\nCudd creation finished in "<<BDDCreationTime_msec<<"milliseconds, i.e. "<<duration<<"seconds\n";
}



// Added by Ajith on 20 July 2015: All path qelim with support for LMIs
DDNode* DDManager::call_All_Path_QElim_WithLMISupport(set<string> &VarSetToElim, DDNode* f, map<string, int> &WidthTable)
{ 
    
  set<DAGNode*> C_X;
  C_X.clear(); 
  
  int project_depth = 0;
  int path_count = 1;

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForAllVarsQE;
  HashTable <string, DDNode*> *PtrTableForAllVarsQE = &TableForAllVarsQE;
  
  DDNode* result = All_Path_QElim_WithLMISupport(VarSetToElim, f, C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, path_count);
 
  if(checkTimeOut())
  {
     // Time Out !!!
     cout<<"\nWarning!!!The function DDManager::call_All_Path_QElim_WithLMISupport has timed out. Its result is not exact\n";
     timed_out = true; // timed_out flag set
     return result;
  }


  return result;
}



// The function called by call_All_Path_QElim_WithLMISupport
DDNode* DDManager::All_Path_QElim_WithLMISupport(set<string> &VarSetToElim, DDNode* f, set<DAGNode*> C_X, map<string, int> &WidthTable, HashTable <string, DDNode*> *PtrTableForAPResolveModified, HashTable <string, DDNode*> *PtrTableForAllVarsQE, int project_depth, int &path_count)
{
  // check if we have already timed out   
 if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function DDManager::All_Path_QElim_WithLMISupport has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return f;
	} 
  //Check if there exists an entry for \exists VarSetToElim. (f /\ C_X)
  DDNode* Existing  = getEntryFromTableForAllVarsQE(VarSetToElim, f, C_X, PtrTableForAllVarsQE);

  if(Existing != NULL)
    {
      return Existing;
    }    
 
 
  DDNode* result;
  
  //Check if f is free of VarSetToElim  
  
  // First check if f is a terminal. If yes, f is free of VarSetToElim
  if(f->getPredicate()==NULL) // Terminal node
    {
      cout << endl << "path " << path_count << " encountered:";
      path_count++;
      
      int value = f->getVal();

      if(value == 0)//0-node
        {
           cout << " 0";
           result = f;
        }//0-node
      else//1-node
        {

          if(C_X.empty())
            {
              cout << " 1:empty";
              result = createDD(1);
            }
          else if(ThUnsat(C_X, WidthTable))
            {
              cout << " 1:unsat"; 
	      result = createDD(0);
            }
          else
            {
              cout << " 1:sat";
              result = callProjectReturningLMDDForMultipleVariables(C_X, VarSetToElim, WidthTable, PtrTableForAllVarsQE, PtrTableForAPResolveModified, project_depth);
               
            }
        }//1-node
  }//Terminal node
  else //Non-terminal node
  {
      // Traverse down
         
      DAGNode* C = f->getPredicate();
      

      if(freeOfVarsEfficient(C, VarSetToElim))//predicate free of all vars to elim
      {
                    
        DDNode *thenb = All_Path_QElim_WithLMISupport(VarSetToElim, f->getRight(), C_X,  WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, path_count);
        DDNode *elseb = All_Path_QElim_WithLMISupport(VarSetToElim, f->getLeft(), C_X, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, path_count);

        DDNode* C_dd = createDD(C, WidthTable);

        result = createITE(C_dd, thenb, elseb);
       }
       else//predicate is not free of all vars to elim
       {
         set<DAGNode*> C_X_then, C_X_else;

         copySet(C_X_then, C_X);
         C_X_then.insert(C);

         copySet(C_X_else, C_X);
         C_X_else.insert(negateConstraint_With_LMISupport(C));   

         DDNode *thenb = All_Path_QElim_WithLMISupport(VarSetToElim, f->getRight(), C_X_then, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, path_count);
         DDNode *elseb = All_Path_QElim_WithLMISupport(VarSetToElim, f->getLeft(), C_X_else, WidthTable, PtrTableForAPResolveModified, PtrTableForAllVarsQE, project_depth, path_count);

         result = apply("or", thenb, elseb);
       }//predicate is not free of all vars to elim
  }////Non-terminal node
  
  // Store \exists X.(f \wedge C_{X}) ---> result into Hash Table
  insertEntryIntoTableForAllVarsQE(VarSetToElim, f, C_X, result, PtrTableForAllVarsQE);
  
  // return the result
  return result;      
}//function ends here

