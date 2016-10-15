
#include "DDManager.h"

DDManager :: DDManager()
{
  count=MAX_AP+1;
}


/* Following function was commented and new function added as a problem with the way hash collission was handled was found

DDNode* DDManager :: createDD(int new_val)  //to create terminal values '1' and '0'
{
char hash_fn_ip[10];
DDNode* addr_DDnode;

if(new_val==0) strcpy (hash_fn_ip, "0"); else if(new_val==1) strcpy (hash_fn_ip, "1"); else {cout<<"\nERROR!createDD(int new_val) called with argument "<<new_val<<endl; exit(1);}

string new_val_string(hash_fn_ip);

if(HT.Check_DDnode_in_ht(hash_fn_ip))
{
addr_DDnode = HT.Get_address_of_existing_DDnode_from_ht(hash_fn_ip, new_val_string);
}
else
{
DDNode* terminal_node = new DDNode(new_val);
addr_DDnode = terminal_node;
HT.Insert_DDnode_in_ht(hash_fn_ip , terminal_node);
}

return addr_DDnode;
}  */

/* Following function was commented on 29/1/2010 as the HT implementation was found inefficient. New function is written down


DDNode* DDManager :: createDD(int new_val)  //to create terminal values '1' and '0'
{
  char hash_fn_ip[10];
  DDNode* addr_DDnode;

  if(new_val==0) strcpy (hash_fn_ip, "0"); else if(new_val==1) strcpy (hash_fn_ip, "1"); else {cout<<"\nERROR!createDD(int new_val) called with argument "<<new_val<<endl; exit(1);}

  string hash_fn_ip_string(hash_fn_ip);

  addr_DDnode = HT.Get_address_of_DDnode_from_ht(hash_fn_ip_string);

  if(addr_DDnode == NULL) // New node
    {
      addr_DDnode = new DDNode(new_val);
      HT.Insert_DDnode_in_ht(hash_fn_ip , addr_DDnode);
    }

  return addr_DDnode;
  } */



DDNode* DDManager :: createDD(int new_val)  //to create terminal values '1' and '0'
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
      return addr_DDnode;
    }
       
}



// createDD(CDAGNode *new_predicate) changed on 15/2/2010

/*DDNode* DDManager :: createDD(CDAGNode *new_predicate)
{
  return createDD(new_predicate, createDD(0), createDD(1));
}*/

DDNode* DDManager :: createDD(CDAGNode *new_predicate, map<string, int> WidthTable, CHelper &MANAGER)
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

  key = pred_addr + opd_addr1 + opd_addr2;

  // Hash key created

  HTStatusValue<DDNode*> result = DD_HT.hashtable_search(key);

  if(result.success()) // Exists in HT
    {
      return result.GetValue();
    }
  else // new node
    {
      
      int node_index;

      if(APMap.find(new_predicate) == APMap.end()) // predicate is not there already
        {

	  // convert predicate to proper form and see

	  cout<<"\nOriginal Equation = "<<new_predicate->getDAGStr()<<endl;

	  new_predicate = recreateEquationInProperForm(new_predicate, WidthTable, MANAGER);

	  cout<<"\nRecreated Equation = "<<new_predicate->getDAGStr()<<endl;

	  if(APMap.find(new_predicate) == APMap.end()) // predicate is not there already
	    {


	      count--;

	      if(count<0) 

		{cout<<"\nIn function  DDManager :: createDD, count has become -ve. Increase MAX_AP\n";
		exit(1);
		}
	    

	      APMap.insert((pair<CDAGNode*,int>(new_predicate,count)));

	      //ReversedAPMap.insert((pair<int, CDAGNode*>(count, new_predicate)));

	      node_index = count;

	      //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) == APMap.end()) "<<count<<endl;
	    }
	  
	  else // already there
	    {
	      //count = APMap[new_predicate];

	      node_index = APMap[new_predicate];

	      //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) != APMap.end()) "<<count<<endl;
	    }



        }
      else // already there
        {
          //count = APMap[new_predicate];

          node_index = APMap[new_predicate];

          //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) != APMap.end()) "<<count<<endl;
        }

       DDNode* addr_DDnode = new DDNode(new_predicate, left, right, node_index);

       result = DD_HT.hashtable_insert(key, addr_DDnode);
   
       if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function DDManager :: createDD(CDAGNode *new_predicate)\n";
	  exit(1);
	}
      return addr_DDnode;
    }
}


/* Following function was commented and new function added as a problem with the way hash collission was handled was found
   DDNode* DDManager :: createDD(CDAGNode *new_predicate, DDNode* left, DDNode *right )
   {
   char hash_fn_ip[50] = " " ;
   char temp_char1[10], temp_char2[10], temp_char3[10];
   string temp;
   DDNode* addr_DDnode;

   sprintf(temp_char1 , "%x", left);
   sprintf(temp_char2 , "%x", right);
   sprintf(temp_char3 , "%x", new_predicate);

   string opd_addr1(temp_char1);
   string opd_addr2(temp_char2);
   string pred_addr(temp_char3);

   temp = pred_addr + opd_addr1 + opd_addr2;

   strcpy (hash_fn_ip, temp.c_str());

   cout<<"\nSearching for hash input fn "<<temp<<" i.e. "<<hash_fn_ip<<" in the hash table\n";

   if(HT.Check_DDnode_in_ht(hash_fn_ip))
   {
   // Temporary
   cout<<endl<<new_predicate->getDAGStr()<<" is the new predicate\n";
   // Temporary  ends here

   addr_DDnode = HT.Get_address_of_existing_DDnode_from_ht(hash_fn_ip, pred_addr);

   // Two lines added on 4/1/10
   //left->Append_DDnode_inlist(addr_DDnode);
   //right->Append_DDnode_inlist(addr_DDnode);
   // lines added on 4/1/10 ends here
   }
   else
   {
   int node_index;

   if(APMap.find(new_predicate) == APMap.end()) // Variable is not there already
   {
   count--;

   APMap.insert((pair<CDAGNode*,int>(new_predicate,count)));

   //ReversedAPMap.insert((pair<int, CDAGNode*>(count, new_predicate)));

   node_index = count;

   //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) == APMap.end()) "<<count<<endl;


   }
   else // already there
   {
   //count = APMap[new_predicate];

   node_index = APMap[new_predicate];

   //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) != APMap.end()) "<<count<<endl;
   }

   DDNode* non_terminal_node = new DDNode(new_predicate, left, right, node_index);

   // Two lines added on 4/1/10
   //left->Append_DDnode_inlist(non_terminal_node);
   //right->Append_DDnode_inlist(non_terminal_node);
   // lines added on 4/1/10 ends here

   addr_DDnode = non_terminal_node;
   HT.Insert_DDnode_in_ht(hash_fn_ip , non_terminal_node);
   }
   return addr_DDnode;
   }  */

/* Following function was commented on 29/1/2010 as the HT implementation was found inefficient. New function is written down

DDNode* DDManager :: createDD(CDAGNode *new_predicate, DDNode* left, DDNode *right )
{
  char hash_fn_ip[50] = " " ;
  char temp_char1[10], temp_char2[10], temp_char3[10];
  string hash_fn_ip_string;
  DDNode* addr_DDnode;

  sprintf(temp_char1 , "%x", left);
  sprintf(temp_char2 , "%x", right);
  sprintf(temp_char3 , "%x", new_predicate);

  string opd_addr1(temp_char1);
  string opd_addr2(temp_char2);
  string pred_addr(temp_char3);

  hash_fn_ip_string = pred_addr + opd_addr1 + opd_addr2;

  addr_DDnode = HT.Get_address_of_DDnode_from_ht(hash_fn_ip_string);

  if(addr_DDnode == NULL) // New node
    {
      int node_index;

      if(APMap.find(new_predicate) == APMap.end()) // Variable is not there already
        {


          count--;

          APMap.insert((pair<CDAGNode*,int>(new_predicate,count)));

          //ReversedAPMap.insert((pair<int, CDAGNode*>(count, new_predicate)));

          node_index = count;

          //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) == APMap.end()) "<<count<<endl;

        


        }
      else // already there
        {
          //count = APMap[new_predicate];

          node_index = APMap[new_predicate];

          //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) != APMap.end()) "<<count<<endl;
        }

      addr_DDnode = new DDNode(new_predicate, left, right, node_index);

      HT.Insert_DDnode_in_ht(hash_fn_ip , addr_DDnode);
    }
  return addr_DDnode;
  } */


DDNode* DDManager :: createDD(CDAGNode *new_predicate, DDNode* left, DDNode *right )
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
      
      int node_index;

      if(APMap.find(new_predicate) == APMap.end()) // Variable is not there already
        {
	  /*  Commented on 15 Feb 2010

          count--;

	  if(count<0) 

	    {cout<<"\nIn function  DDManager :: createDD, count has become -ve. Increase MAX_AP\n";
	     exit(1);
	    }
	    

          APMap.insert((pair<CDAGNode*,int>(new_predicate,count)));

          //ReversedAPMap.insert((pair<int, CDAGNode*>(count, new_predicate)));

          node_index = count;

          //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) == APMap.end()) "<<count<<endl;

	  */

	  
	  cout<<"\ncreateDD(CDAGNode *new_predicate, DDNode* left, DDNode *right ) called to create DDs with new equations. If you want to create DDs with new equations, use createDD(CDAGNode *new_predicate, map<string, int> WidthTable, CHelper &MANAGER)\n";
	  exit(1);



        }
      else // already there
        {
          //count = APMap[new_predicate];

          node_index = APMap[new_predicate];

          //cout<<"\nCurrent value of count is (inside (APMap.find(new_predicate) != APMap.end()) "<<count<<endl;
        }

       DDNode* addr_DDnode = new DDNode(new_predicate, left, right, node_index);

       result = DD_HT.hashtable_insert(key, addr_DDnode);
   
       if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function DDManager :: createDD(CDAGNode *new_predicate, DDNode* left, DDNode *right )\n";
	  exit(1);
	}
      return addr_DDnode;
    }
}




void DDManager :: printGraphInDotFormat(DDNode* me, string FileName)
{
  set<DDNode*> Nodes;

  Nodes.clear();

  fstream file_op(FileName.c_str(),ios::out);

  file_op<<"digraph G {"<<endl;

  create_node(file_op, me, Nodes);

  file_op<<"}"<<endl;

  file_op.close();
}

void DDManager :: create_node(fstream &file_op, DDNode* me, set<DDNode*> &Nodes)
{
  if(Nodes.find(me) != Nodes.end())
    {
      return;
    }
  else
    {
      if(me->getPredicate()!=NULL)
        {
          //cout<<"\nme = "<<me<<endl;
          file_op<<"n"<<me<<"[predicate=\""<< ((me->getPredicate())->getDAGStr()).c_str() <<"\"];"<<endl;
        }
      else
        {
          file_op<<"n"<<me<<"[value=\""<< me->getVal() <<"\"];"<<endl; return;
        }

      Nodes.insert(me);

      file_op<<"n"<<me<<"->"<<"n"<<me->getLeft()<<";"<<endl;
      file_op<<"n"<<me<<"->"<<"n"<<me->getRight()<<";"<<endl;

      create_node(file_op, me->getLeft(), Nodes);
      create_node(file_op, me->getRight(), Nodes);
    }
}

/*
DDNode* DDManager::apply(string op, DDNode* v1, DDNode* v2)
{
  //cout<<"\napply called\n";

  TableForApply.clear();
  //cout<<"\napply called with op ="<<op<<" and arguments "<<v1->getDDString()<<" and "<<v2->getDDString()<<endl;
  //cout<<"\ncalling applyStep\n";
  //cout<<"\ncalling applyStep\n";
  //cout<<"\ncalling applyStep\n";
  //cout<<"\ncalling applyStep\n";
  //cout<<"\ncalling applyStep\n";
  DDNode* result = applyStep(op, v1, v2);
  //cout<<"\n\nApply Completed\n";
  //cout<<"\nNow result="<<result->getDDString()<<endl;

  //printGraphInDotFormat(result, "before_reduce.dot");

  //cout<<"\n\nApply Completed\n";
  //cout<<"\nNow result="<<result->getDDString()<<endl;
  //cout<<"\n\nApply Completed\n";
  //cout<<"\nNow result="<<result->getDDString()<<endl;

  //map<DDNode*, DDNode*> RedundantNodes;
  //RedundantNodes.clear();
  //cout<<"\nCalling reduce\n"<<endl;
  //cout<<"\nCalling reduce\n"<<endl;
  //cout<<"\nCalling reduce\n"<<endl;
  //cout<<"\nCalling reduce\n"<<endl;
  //cout<<"\nCalling reduce\n"<<endl;
  //cout<<"\nCalling reduce\n"<<endl;

  //DDNode* reducedResult = Reduce(result, RedundantNodes);

  //DDNode* reducedResult = ReduceModified(result);

  DDNode* reducedResult = ReduceModifiedAdvanced(result);

  //DDNode* reducedResult = result;


  //cout<<"\nreduced result="<<reducedResult->getDDString()<<endl;

  //deleteRedundantNodes(RedundantNodes);

  //cout<<"\nDeletion of redundant nodes is done\n";

  //printGraphInDotFormat(reducedResult, "after_reduce.dot");

  //cout<<"\napply finished\n";

  return reducedResult;
}
DDNode* DDManager::applyStep(string op, DDNode* v1, DDNode* v2)
{


  //cout<<"\napplyStep called\n";

  DDNode* u;

  map< pair<DDNode*, DDNode*>, DDNode*>::iterator TableForApply_it = TableForApply.find(make_pair(v1,v2));

  if(TableForApply_it != TableForApply.end())
    {
      //cout<<"\nTableForApply HIT\n";
      u=TableForApply_it->second;
      return u;// {have already evaluated}
    }

  //cout<<"\nTableForApply MISS\n";

  int u_value, u_index;
  CDAGNode *u_AP;

  u_value = evaluateOp(op, v1, v2); // should be evaluateOp
  if(u_value != -1)
    {
      //{create terminal vertex}

      //cout<<"\napplyStep finished\n";

      u = createDD(u_value);
      TableForApply.insert(make_pair(make_pair(v1,v2), u));
      return u;


      //return createDD(u_value);
    }

  // NEWLY ADDED ON 2nd Jan 10
  // else if(partiallyEvaluatable(op, v1, v2))
  //  {
  //   u = partialEvaluation(op, v1, v2);
  
  //    TableForApply.insert(make_pair(make_pair(v1,v2), u));
  
        //cout<<"\napplyStep finished\n";
  
  //     return u;
  //   }
  //NEWLY ADDED ON 2nd Jan 10 ends here

  else //{create nonterminal and evaluate further down}
    {

      int v1_index, v2_index;
      v1_index = v1->getIndex();
      v2_index = v2->getIndex();
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

      TableForApply.insert(make_pair(make_pair(v1,v2), u));
      //{add vertex to table}

      //cout<<"\napplyStep finished\n";

      return u;
    }
    }*/


bool DDManager::partiallyEvaluatable(string op, DDNode *v1, DDNode *v2)
{
  if(v1->getPredicate()!= NULL && v2->getPredicate()!= NULL) return false;

  // Either of v1 or v2 is 1 or 0

  int value1;
  int value2;

  if(v1->getPredicate() == NULL) value1 = v1->getVal(); else value1 = -1;
  if(v2->getPredicate() == NULL) value2 = v2->getVal(); else value2 = -1;

  if((op=="or" || op=="xor") && (value1 == 0 || value2 == 0)) return true;
  if(op=="and" && (value1 == 1 || value2 == 1)) return true;

  return false;
}

DDNode* DDManager::partialEvaluation(string op, DDNode *v1, DDNode *v2)
{
  // Precondn : Partially evaluatable

  //cout<<"\npartialEvaluation triggered with";

  int value1;
  int value2;

  if(v1->getPredicate() == NULL) value1 = v1->getVal(); else value1 = -1;
  if(v2->getPredicate() == NULL) value2 = v2->getVal(); else value2 = -1;

  //cout<<"value1 = "<<value1<<" , value2 = "<<value2<<" , op = "<<op;

  if(op == "or" || op == "xor")
    {
      if(value1 == 0)
        { return v2;}
      if(value2 == 0)
        { return v1;}
      else
        {
          //cout<<"\nFunction partiallyEvaluate called with op ="<<op<<" and none of the arguments zero\n";
          exit(1);
        }
    }

  if(op == "and")
    {
      if(value1 == 1)
        { return v2;}
      if(value2 == 1)
        { return v1;}
      else
        {
          //cout<<"\nFunction partiallyEvaluate called with op ="<<op<<" and none of the arguments one\n";
          exit(1);
        }
    }
}


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


int DDManager::evaluateOpOld(string op, DDNode *v1, DDNode *v2)
{
  if(v1->getPredicate()!= NULL || v2->getPredicate()!= NULL) return -1;

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


int DDManager::findMin(int i, int j)
{
  if(i<=j) return i;
  else return j;
}

CDAGNode* DDManager::findAPFromIndex(int u_index)
{

  map<CDAGNode*, int>::iterator APMap_it = APMap.begin();
  for(; APMap_it != APMap.end(); APMap_it++)
    {
      if(APMap_it->second == u_index) return APMap_it->first;
    }

  //return ReversedAPMap[u_index];
}

void DDManager::nullifyMarks(DDNode *v)
{
  v->setMark(false);
  if(v->getIndex()<=MAX_AP) nullifyMarks(v->getLeft());
  if(v->getIndex()<=MAX_AP) nullifyMarks(v->getRight());
}

void DDManager::nullifyMarksModifiedWithDP(DDNode *v)
{
  //cout<<"\nnullifyMarksModifiedWithDP("<<v<<") called\n";
  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      //cout<<"\nAlready visited\n";
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {
      //cout<<"\nNot already visited\n";
      if(v->getIndex()<=MAX_AP) nullifyMarksModifiedWithDP(v->getLeft());
      if(v->getIndex()<=MAX_AP) nullifyMarksModifiedWithDP(v->getRight());
      v->setMark(false);
      SetOfDDNodes.insert(v);
    }
}

void DDManager::getNoOfNodesInDD(DDNode *v, int &NoOfNodesInDD)
{

  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      //cout<<"\nAlready visited\n";
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {
      //cout<<"\nNot already visited\n";
      if(v->getIndex()<=MAX_AP) getNoOfNodesInDD(v->getLeft(), NoOfNodesInDD);
      if(v->getIndex()<=MAX_AP) getNoOfNodesInDD(v->getRight(), NoOfNodesInDD);

      NoOfNodesInDD++; // New DDNode encountered
      SetOfDDNodes.insert(v);
    }
}

void DDManager::Traverse(DDNode *v, map<int, list<DDNode*> > &vlist)
{
  //procedure Traverse(v:vertex);
  v->setMark(not v->getMark());

  // Add v into list for v.index
  map<int, list<DDNode*> >::iterator vlist_it;
  vlist_it = vlist.find(v->getIndex());
  if(vlist_it != vlist.end()) vlist[v->getIndex()].push_back(v);
  else {list<DDNode*> new_list; new_list.push_back(v); vlist.insert(make_pair(v->getIndex(), new_list)); }

  //cout<<"\nNode "<<v->getDDString()<<" encountered ...\n";
  //showVlist(vlist);


  if(v->getIndex()<= MAX_AP) // {v nonterminal}
    {
      if(v->getMark() != (v->getLeft())->getMark())
        {Traverse(v->getLeft(), vlist);}
      if(v->getMark() != (v->getRight())->getMark())
        {Traverse(v->getRight(), vlist);}
    }
}

void DDManager::showVlist(map<int, list<DDNode*> > vlist)
{
  for(map<int, list<DDNode*> >::iterator vlist_it = vlist.begin(); vlist_it != vlist.end(); vlist_it++)
    {
      cout<<"\nIndex="<<vlist_it->first<<"\tList = ";
      list<DDNode*> List1 = vlist_it->second;
      for(list<DDNode*>::iterator it=List1.begin(); it!=List1.end(); it++)
        cout<<(*it)<<",";
    }
}

DDNode* DDManager::Reduce(DDNode *v, map<DDNode*, DDNode*> &RedundantNodes)
{
  //cout<<"\nReduce called\n";
  DDNode *u;
  map<int, DDNode*> subgraph;
  subgraph.clear();
  map<int, DDNode*>::iterator subgraph_it;
  map<int, list<DDNode*> > vlist;

  vlist.clear();

  SetOfDDNodes.clear();
  nullifyMarksModifiedWithDP(v);

  //nullifyMarks(v); // Inefficient : Temporary
  Traverse(v, vlist);

  //showVlist(vlist);

  map<pair<int, int>, DDNode*> Q;
  map<pair<int, int>, DDNode*>::iterator Q_it;
  pair<int, int> old_key, key;

  int nextid = 0;

  // count is the index of the last AP created

  //cout<<"\ncount = "<<count<<endl;

  for(int i=MAX_AP+1; i>=count; i--)
    {

      //cout<<"\ni="<<i<<endl;

      Q.clear();
      if(vlist.find(i) == vlist.end()) continue;


      list<DDNode*> CurrentList = vlist[i];

      for(list<DDNode*>::iterator it=CurrentList.begin(); it!=CurrentList.end(); it++)
        {
          u=*it;
          //cout<<"\nu="<<u<<endl;
          if(u->getIndex() == MAX_AP+1) // Terminal nodes
            {
              cout<<endl<<u->getVal()<<"\t"<<u->getVal()<<"\t"<<u<<" inserted into Q";
              Q.insert(make_pair(make_pair(u->getVal(), u->getVal()), u));
            }
          else if(u->getLeft()->getID() == u->getRight()->getID())
            {
              u->setID(u->getLeft()->getID()); // u is a redundant node
              RedundantNodes.insert(make_pair(u, u->getLeft()));
              // NEWly added
              //replaceDDNode(u, u->getLeft());
            }
          else
            {
              cout<<endl<<u->getLeft()->getID()<<"\t"<<u->getRight()->getID()<<"\t"<<u<<" inserted into Q";
              Q.insert(make_pair(make_pair(u->getLeft()->getID(), u->getRight()->getID()), u));
            }
        }

      //cout<<"\nQ formed\n";

      //showQ(Q);


      //sortQ(Q); // ADDED ON 13/1/2010

      old_key=make_pair(-1, -1);
      while(!Q.empty())
        {

          //cout<<"\nTrying to get element with minium k\n";

          Q_it=getElementWithMinKey(Q);

          //showQ(Q);

          //cout<<"\nElement with minium k obtained\n";

          key=Q_it->first;
          //cout<<"\nKey1="<<key.first<<", key2="<<key.second<<endl;
          u=Q_it->second;
          //cout<<"\nu="<<u<<endl;
          //showQ(Q);

          Q.erase(key);


          if (key.first==old_key.first && key.second==old_key.second)
            {

              //cout<<"\nMatching with existing vertex\n";
              u->setID(nextid); //{matches existing vertex}
              // existing vertex is subgraph[nextid]
              RedundantNodes.insert(make_pair(u, subgraph[nextid]));

              // NEWly added
              //replaceDDNode(u, subgraph[nextid]);
            }
          else //{unique vertex}
            {
              //cout<<"\nunique vertex\n";

              nextid = nextid + 1;
              u->setID(nextid);
              subgraph.insert(make_pair(nextid,u));

              if(u->getIndex() != MAX_AP+1)
                {
                  //cout<<"\nnon-terminal vertex\n";

                  subgraph_it=subgraph.find(u->getLeft()->getID());
                  if(subgraph_it != subgraph.end())
                    {
                      // CHANGED on 4/1/10

                      // Delete u from u's present left's in_list
                      //DDNode* PresentLeft = u->getLeft();

                      //if(PresentLeft != NULL)
                      //  PresentLeft->Delete_single_entry_from_DD_inlist(u);

                      // set new left for u
                      u->setLeft(subgraph_it->second);

                      // set the in_list for the new left
                      //(subgraph_it->second)->Append_DDnode_inlist(u);

                      // CHANGED on 4/1/10 ends here
                    }

                  subgraph_it=subgraph.find(u->getRight()->getID());
                  if(subgraph_it != subgraph.end())
                    {
                      // CHANGED on 4/1/10

                      // Delete u from u's present right's in_list
                      //DDNode* PresentRight = u->getRight();

                      //if(PresentRight != NULL)
                      //  PresentRight->Delete_single_entry_from_DD_inlist(u);

                      // set new right for u
                      u->setRight(subgraph_it->second);

                      // set the in_list for the new right
                      // (subgraph_it->second)->Append_DDnode_inlist(u);

                      // CHANGED on 4/1/10 ends here
                    }

                }



              old_key.first = key.first;
              old_key.second = key.second;
            }
        }
    }

  //cout<<"\nReduce finished\n";

  return(subgraph[v->getID()]);
}

// This function returns the iterator of Q which contains the minimum element
map<pair<int, int>, DDNode*>::iterator DDManager::getElementWithMinKey(map<pair<int, int>, DDNode*> Q)
{

  int key1_min, key2_min, key1, key2;
  map<pair<int, int>, DDNode*>::iterator Q_it=Q.begin();
  map<pair<int, int>, DDNode*>::iterator Selected_it=Q_it;
  key1_min=(Q_it->first).first;
  key2_min=(Q_it->first).second;
  if(Q_it!=Q.end()) Q_it++;

  for(; Q_it!=Q.end(); Q_it++)
    {
      key1 = (Q_it->first).first;
      key2 = (Q_it->first).second;
      if(key1<key1_min) {Selected_it=Q_it; key1_min=key1; key2_min=key2;}
      else if(key1==key1_min && key2<key2_min) {Selected_it=Q_it; key1_min=key1; key2_min=key2;}
    }
  return Selected_it;
}

void DDManager::showQ(map<pair<int, int>, DDNode*> Q)
{
  map<pair<int, int>, DDNode*>::iterator Q_it=Q.begin();

  for(; Q_it!=Q.end(); Q_it++)
    {
      int key1 = (Q_it->first).first;
      int key2 = (Q_it->first).second;
      DDNode* u = (Q_it->second);
      //cout<<"\nKey1="<<key1<<", Key2="<<key2<<", u="<<u<<endl;
    }
}

DDNode* DDManager::createDDNot(DDNode* v1)
{
  //DDNode* result = apply("xor", v1, createDD(1));

  DDNode* result = applyModified("xor", v1, createDD(1));

  return result;
}

// Function to create a DD from a DAG
DDNode* DDManager::createDDFromDAG(CDAGNode *dag, CHelper &MANAGER, map<string, int> WidthTable)
{
  string dag_str = dag->Get_DAGnode_value();

  if(dag_str == "equivalent" || dag_str == "=")
    {

      //cout<<"\nCreating DD for equivalent\n";
      //cout<<"\nCreating DD for "<<dag->getDAGStr()<<endl;
      return createDD(dag, WidthTable, MANAGER);
    }

  else if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
    {
      //cout<<"\nCreating DD for is_not_equal\n";
      //cout<<"\nCreating DD for "<<dag->getDAGStr()<<endl;
      list<CDAGNode*> :: iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;
      //cout<<"\nleft= "<<left->getDAGStr()<<endl;
      //cout<<"\nright= "<<right->getDAGStr()<<endl;
      CDAGNode* inverseDAG = MANAGER.createDAG("equivalent", left, right);
      //cout<<"\nCreating DD for "<<inverseDAG->getDAGStr()<<endl;
      DDNode* inverseDD = createDD(inverseDAG, WidthTable, MANAGER);
      //cout<<"\ninverseDD = "<<inverseDD->getDDString()<<endl;
      return createDDNot(inverseDD);
    }

  else if(dag_str == "and")
    {
      //cout<<"\nCreating DD for and\n";
      list<CDAGNode*> :: iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;


      DDNode* then_dd = createDDFromDAG(left, MANAGER, WidthTable);
      DDNode* els_dd = createDDFromDAG(right, MANAGER, WidthTable);

      //return apply("and", then_dd, els_dd);

      return applyModified("and", then_dd, els_dd);


      //return apply("and", createDDFromDAG(left, MANAGER), createDDFromDAG(right, MANAGER));
    }
  else if(dag_str == "or")
    {
      //cout<<"\nCreating DD for or\n";
      list<CDAGNode*> :: iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;


      DDNode* then_dd = createDDFromDAG(left, MANAGER, WidthTable);
      DDNode* els_dd = createDDFromDAG(right, MANAGER, WidthTable);

      //return apply("or", then_dd, els_dd);

      return applyModified("or", then_dd, els_dd);

      //return apply("or", createDDFromDAG(left, MANAGER), createDDFromDAG(right, MANAGER));
    }
  else if(dag_str == "ite")
    {
      //cout<<"\nCreating DD for ite\n";
      list<CDAGNode*> :: iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *codn, *left, *right;
      codn = *iout;

      //if(codn->Get_DAGnode_value()!="equivalent" && codn->Get_DAGnode_value()!="is_not_equal") {cout<<"\nite with condition part having name "<<codn->Get_DAGnode_value()<<" inside fn: createDDFromDAG. Exiting...\n"; exit(1);}
      iout++;
      left =*iout;
      iout++;
      right =*iout;

      //cout<<"\nITE DD creation started\n";

      // Changed on 2/1/2010 for improving efficiency

      //return apply("or", apply("and", createDDFromDAG(codn, MANAGER) , createDDFromDAG(left, MANAGER)) , apply("and", createDDNot(createDDFromDAG(codn, MANAGER)) , createDDFromDAG(right, MANAGER)));

      DDNode* then_dd = createDDFromDAG(left, MANAGER, WidthTable);
      DDNode* els_dd = createDDFromDAG(right, MANAGER, WidthTable);
      DDNode* codn_dd = createDDFromDAG(codn, MANAGER, WidthTable);

      //return createITE(codn_dd, then_dd, els_dd);
      return createITEModified(codn_dd, then_dd, els_dd);


      //cout<<"\nITE DD creation finished\n";

      // Changed on 2/1/2010 for improving efficiency ends here
    }
  else if(dag_str == "1" || dag_str == "true")
    {
      //cout<<"\nCreating DD for true\n";
      return createDD(1);
    }
  else if(dag_str == "0" || dag_str == "false")
    {
      //cout<<"\nCreating DD for false\n";
      return createDD(0);
    }
  else
    {
      cout<<"\ncreateDDFromDAG called with dag name "<<dag_str<<". Exiting...\n"; exit(1);
    }
}

// Function to create a DAG from a DD

CDAGNode* DDManager::createDAGFromDD(DDNode *dd, CHelper &MANAGER)
{
  //MapForDAGFromDD.clear();


  HashTable <string, CDAGNode*> MapForDAGFromDD;
  HashTable <string, CDAGNode*> *PtrMapForDAGFromDD = &MapForDAGFromDD;

  CDAGNode* result = createDAGFromDDStep(dd, MANAGER, PtrMapForDAGFromDD);
  return result;
}

CDAGNode* DDManager::createDAGFromDDStep(DDNode *dd, CHelper &MANAGER, HashTable <string, CDAGNode*> *PtrMapForDAGFromDD)
{
  //cout<<"\ncreateDAGFromDDStep called with argument "<<dd<<"\n";

  char temp_char[10];
  sprintf(temp_char , "%x", dd);
  string key = temp_char;
  
  // key created

  HTStatusValue<CDAGNode*> ResultFromHT = PtrMapForDAGFromDD->hashtable_search(key);

  if(ResultFromHT.success())
    {
      return ResultFromHT.GetValue();
    }

  /*
  map<DDNode*, CDAGNode*>::iterator it = MapForDAGFromDD.find(dd);
  if(it != MapForDAGFromDD.end())
    {
      //cout<<"\ncreateDAGFromDDStep was earlier called with argument "<<dd<<"\n";
      return it->second;
    }
  */

  //cout<<"\ncreateDAGFromDDStep was not earlier called with argument "<<dd<<"\n";
  CDAGNode *result;
  if(dd->getIndex()==MAX_AP+1)
    {
      int val=dd->getVal();
      string constant;
      if(val==1) constant="1"; else constant="0";
      result = MANAGER.createDAG(constant);
    }
  else
    {
      result = MANAGER.createDAG("ite", dd->getPredicate(), createDAGFromDDStep(dd->getRight(), MANAGER, PtrMapForDAGFromDD), createDAGFromDDStep(dd->getLeft(), MANAGER, PtrMapForDAGFromDD)); // Note that DD for equality eq is eq(0, 1).i.e eq with left child 0 and right child 1
    }

  //MapForDAGFromDD.insert(make_pair(dd, result));


  ResultFromHT = PtrMapForDAGFromDD->hashtable_insert(key, result);
  if(!ResultFromHT.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::createDAGFromDDStep\n";
      exit(1);
    }

  return result;
}

CDAGNode* DDManager::createDAGFromDDWithNoDP(DDNode *dd, CHelper &MANAGER)
{
  if(dd->getIndex()==MAX_AP+1)
    {
      int val=dd->getVal();
      string constant;
      if(val==1) constant="1"; else constant="0";
      return MANAGER.createDAG(constant);
    }
  else
    {
      return MANAGER.createDAG("ite", dd->getPredicate(), createDAGFromDDWithNoDP(dd->getRight(), MANAGER), createDAGFromDDWithNoDP(dd->getLeft(), MANAGER)); // Note that DD for equality eq is eq(0, 1).i.e eq with left child 0 and right child 1
    }
}


/*
DDNode* DDManager::DAGResolve(DDNode *dd, CDAGNode *equation, string VarToQuantify, CHelper &MANAGER)
{
  map< pair<DDNode*, CDAGNode*>, DDNode*>::iterator it = TableForDagResolve.find(make_pair(dd, equation));

  if(it != TableForDagResolve.end())
    {
      //cout<<"\nTable hit in DAGResolve\n";
      return it->second;
    }
  if(it == TableForDagResolve.end())
    {
      //cout<<"\nTable miss in DAGResolve\n";
      CDAGNode* dag = createDAGFromDD(dd, MANAGER);
      dag = MANAGER.createDAG("and", equation, dag);
      set<string> VarsToQuantify;
      VarsToQuantify.insert(VarToQuantify);
      //cout<<"\nInlining called with dag = "<<dag->getDAGStr()<<", and VarToQuantify = "<<VarToQuantify<<endl;
      dag = MANAGER.Inlining(dag, VarsToQuantify);
      //cout<<"\ndag="<<dag->getDAGStr()<<endl;
      DDNode* result = createDDFromDAG(dag, MANAGER);
      //cout<<"\nDD of dag ="<<result->getDDString()<<endl;

      TableForDagResolve.insert(make_pair(make_pair(dd, equation), result));
      return result;
    }
}
*/


bool DDManager::DDFreeOfVar(DDNode *f, string VarToElim)
{
  if(f->getIndex()<=MAX_AP)
    {
      CDAGNode *predicate = f->getPredicate();
      if(!(predicate->freeOfVarModified(VarToElim))) return false;
      if(!DDFreeOfVar(f->getLeft(), VarToElim)) return false;
      if(!DDFreeOfVar(f->getRight(), VarToElim)) return false;
      return true;
    }
  return true;
}

void DDManager::copySet(set<CDAGNode*> &dest, set<CDAGNode*> source)
{
  set<CDAGNode*>::iterator it;
  for(it=source.begin(); it!=source.end(); it++)
    dest.insert(*it);
}

CDAGNode* DDManager::negateConstraint(CDAGNode* C, CHelper &MANAGER)
{
  string new_name;
  if(C->Get_DAGnode_value() == "equivalent")
    new_name = "is_not_equal";
  else if(C->Get_DAGnode_value() == "is_not_equal")
    new_name = "equivalent";

  list<CDAGNode*>::iterator iout = C->Get_DAGnode_outlist();
  CDAGNode *left = *iout;
  iout++;
  CDAGNode *right=*iout;
  return MANAGER.createDAG(new_name, left, right);
}


/* Replaced with MultiVarQElimWithStrategy
DDNode* DDManager::MultiVarQElim(list<string> VarsToElim, DDNode* f, CHelper &MANAGER, map<string, int> WidthTable)
{
  DDNode *result;

  result = f;

  //cout<<"\nresult before the loop = "<<result->getDDString()<<endl;

  for(list<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      set<CDAGNode*> E, D;
      E.clear(); D.clear();
      string VarToElim = *it;
      TableForDagResolve.clear();

      //cout<<"\nresult before eliminating variable "<<VarToElim<<" = "<<result->getDDString()<<endl;
      result = QElim(VarToElim, result, E, D, MANAGER, WidthTable);
      //cout<<"\nresult after eliminating variable "<<VarToElim<<" = "<<result->getDDString()<<endl;
    }
  return result;
  } */


/* Replaced with QElimWithDPModified
DDNode* DDManager::QElim(string VarToElim, DDNode* f, set<CDAGNode*> E, set<CDAGNode*> D, CHelper &MANAGER, map<string, int> WidthTable)
{

  //cout<<"\nQElim called with DD Node "<<f<<endl;

  if(f->getIndex()==MAX_AP+1) // Terminal node
    {
      int value = f->getVal();

      //cout<<"\nTerminal node reached with value = "<<value<<endl;
      if(value == 0)
        { //cout<<"\nZero returned\n";
          return f; }
      //cout<<"\nTerminal node reached with value = "<<value<<endl;

      if(E.empty() && D.empty())
        {
          //cout<<"\nOne returned\n";
          return createDD(1);
        }

      cout<<"\nThUnsat called\n";
      bool ThUnsatResult = ThUnsat(E, D, WidthTable, MANAGER);
      cout<<"\nThUnsatResult = "<<ThUnsatResult<<endl;
      if(ThUnsatResult)
        { cout<<"\nUnsat. Return 0\n";
          return createDD(0);
        }
      DDNode *temp;
      cout<<"\nAPResolve called\n";
      temp = APResolve(E, D, VarToElim, WidthTable, MANAGER);
      cout<<"\nAPResolve finished\n";
      return temp;
    }
  else
    {

      //cout<<"\nChecking if E or D is empty\n";


      if (!E.empty() || !D.empty())
        {
          cout<<"\n!E.empty() or !D.empty()"<<endl;
          CDAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable, MANAGER);
          if(e1 != NULL)
            {
              cout<<"\nEquation Selected = "<<e1->getDAGStr()<<endl;
              cout<<"\nCalling DAGResolve\n";
              //cout<<"\nf="<<f->getDDString()<<endl;
              f = DAGResolve(f, e1, VarToElim, MANAGER);
              //cout<<"\nf after DAGResolve ="<<f->getDDString()<<endl;
              cout<<"\nDAGResolve finished\n";
              if(DDFreeOfVar(f, VarToElim))
                {
                  cout<<"\nf is free of "<<VarToElim<<endl;
                  cout<<"\nAPResolve called with Theory Check\n";
                  DDNode *temp;
                  if(ThUnsat(E, D, WidthTable, MANAGER)) temp = createDD(0);
                  else temp = APResolve(E, D, VarToElim, WidthTable, MANAGER);

                  cout<<"\nAPResolve finished\n";
                  //cout<<"\nAPResolve ="<<temp->getDDString()<<endl;
                  temp = apply("and", f, temp);
                  //cout<<"\nDAGResolve & APResolve ="<<temp->getDDString()<<endl;
                  //exit(1); //temporary
                  return temp;
                }
            }
        }



      //cout<<"\nGetting predicate C\n";
      CDAGNode* C = f->getPredicate();
      cout<<"\npredicate obtained is "<<C->getDAGStr()<<endl;
      if(C->freeOfVarModified(VarToElim))
        {
          cout<<"\nf is free of "<<VarToElim<<". Going down"<<endl;

          DDNode *thenb = QElim(VarToElim, f->getRight(), E, D, MANAGER, WidthTable);
          DDNode *elseb = QElim(VarToElim, f->getLeft(), E, D, MANAGER, WidthTable);

          // Changed on 2/1/2010 for improving efficiency

          //return apply("or", apply("and", createDD(C) , thenb) , apply("and", createDDNot(createDD(C)) ,elseb));

          DDNode* C_dd = createDD(C);
          return createITE(C_dd, thenb, elseb);

          // Changed on 2/1/2010 for improving efficiency ends here
        }
      else
        {
          cout<<"\nf is not free of "<<VarToElim<<". Going down"<<endl;

          set<CDAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C, MANAGER));
          DDNode *thenb = QElim(VarToElim, f->getRight(), E_new, D, MANAGER, WidthTable);
          DDNode *elseb = QElim(VarToElim, f->getLeft(), E, D_new, MANAGER, WidthTable);
          return apply("or", thenb, elseb);
        }
    }
}
*/

// Search MyList for element
int DDManager::searchList(list<string> MyList, string element)
{
  for(list<string>::iterator it=MyList.begin(); it != MyList.end(); it++)
    if(*it==element) return 1;
  return 0;
}


void DDManager::attachSetToList(set<string> MySet, list<string> &MyList)
{

  for(set<string>::iterator it=MySet.begin(); it!=MySet.end(); it++)
    {
      if(!MyList.empty())
        {if(searchList(MyList,*it) == 0)
            MyList.push_back(*it);}
      else
        MyList.push_back(*it);
    }
}


/* Replaced by APResolveModified

DDNode* DDManager::APResolve(set<CDAGNode*> E, set<CDAGNode*> D, string VarToElim, map<string, int> WidthTable, CHelper &MANAGER)
{

    map< set<CDAGNode*>, DDNode*>::iterator aptbit = getEntryFromTableForAPResolve(E, D);
   if(aptbit != TableForAPResolve.end())
    {
      //cout<<"\nAPResolve was earlier encountered with the same set of E and D\n";
      return aptbit->second;
    }

   else
     {
  //cout<<"\nAPResolve was not earlier encountered with the same set of E and D\n";

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
  for(set<CDAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    (*it)->getDAGNames(names);

  for(set<CDAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    (*it)->getDAGNames(names);

  attachSetToList(names, VarNames);

  //showList(VarNames, "VarNames");

  NoVars=VarNames.size();
  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<CDAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));


  cout<<"\nShowing the constraints before calling the Eliminator\n";

  for(set<CDAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
      {
        cout<<(*it)->getDAGStr()<<endl;
      }


  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  CDAGNode *resultDAG;


  //clock_t q_elim_start_time = clock();

  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, MANAGER, VarNames, WidthTable);

  //cout<<"\n\nQUANTIFIER ELIMINATION TIME  = "<< ((double)clock() - q_elim_start_time)/CLOCKS_PER_SEC <<endl<<endl;



  if(resultDAG == NULL) resultDAG = MANAGER.createDAG("1");

  cout<<"\nResult of APResolve in DAG = "<<resultDAG->getDAGStr()<<endl;

  //qEliminator(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, fptemp);

  DDNode* resultDD;

  //cout<<"\nCreating DD from APs\n";

  // Following line commented and new added on 15/1/2010

  //resultDD = createDDFromDAGFromAPs(resultDAG, MANAGER);

  resultDD = createDDFromDAG(resultDAG, MANAGER);

  //cout<<"\nDD created from APs\n";

  insertEntryIntoTableForAPResolve(E, D, resultDD);

  //cout<<"\nEntry inserted into TableForAPResolve\n";

  //cout<<"\nAPResolve() is finished\n";

  return resultDD;
  }

  } */

bool DDManager::getEquationsAndDisequations(set<CDAGNode*> Constraints, list<string> VarNames, int &NoOfBits, int &NoOfEqs, int &NoOfDisEqs, Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, map<string, int> WidthTable)
{

  NoOfEqs=0;
  NoOfDisEqs=0;
  NoOfBits=0;

  int NoOfBitsInConstraint;

  for(set<CDAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
      CDAGNode* root = (*it);

      //cout<<"\nroot = "<<root->getDAGStr()<<endl;

      NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);


      //cout<<"\nNo: of Bits in constraint = "<<NoOfBitsInConstraint<<endl;

      if(NoOfBitsInConstraint > NoOfBits)
        NoOfBits=NoOfBitsInConstraint;
    }


  //cout<<"\nNo: of Bits obtained\n";

  // Commented on 30 Dec 09. Even if no: of bits = 1, our tool can be used
  //if(NoOfBits==1) // As no: of bits==1, all constraints are at the bit-level; no need to use our tool
  //   {
  //    cout<<"\nNo: of bits=1. returning from getEquationsAndDisequations\n";
  //    return true;}

  // Now NoOfBits gives the maximum number of bits. Hence 2^NoOfBits gives the Mod

  for(set<CDAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
      CDAGNode* root = (*it);

      NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);


      //cout<<"\nNo: of Bits in constraint obtained\n";

      string type = root->Get_DAGnode_value();

      CDAGNode *lhs, *rhs;
      list<CDAGNode*>::iterator out_list_it = root->Get_DAGnode_outlist();
      lhs = (*out_list_it);
      out_list_it++;
      rhs = (*out_list_it);

      Matrix A, C; // Row matrices
      Matrix B, D; // Row matrices

      getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
      getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D


      //cout<<"\nCoefs and constants obtained\n";


      // We have AX+B op CX+D
      // Convert A to A-C and D to D-B to get (A-C)X op (D-B)

      ULINT LocalMod=findPower(NoOfBitsInConstraint);
      ULINT GlobalMod=findPower(NoOfBits);
      ULINT Multiple=findPower(NoOfBits-NoOfBitsInConstraint);

      matSub(A, C, LocalMod);
      matSub(D, B, LocalMod);

      // ADDED ON 31ST DEC 12 AM

      if(LocalMod == 2 && type=="is_not_equal")
        {
          // currently constant is in D[0][0]. we are converting the diseqn to eqn
          if(D[0][0]==1) D[0][0]=0; else D[0][0]=1;
          type = "equivalent";
        }

      // ADDED ON 31ST DEC 12 AM ENDS HERE

      scalarMatMultiply(A, Multiple, GlobalMod);
      scalarMatMultiply(D, Multiple, GlobalMod);

      // Final matrices are A and D

      if(!IsNullMatrix(A))
        {
          if(type=="equivalent")
            {
              addRow(EqCoefMat,A);
              addRow(EqConstMat,D);
              NoOfEqs++;
            }
          else if(type=="is_not_equal")
            {
              addRow(DisEqCoefMat,A);
              addRow(DisEqConstMat,D);
              NoOfDisEqs++;
            }

          else {cout<<"\nUnknown root name "<<type<<" inside function getEquationsAndDisequations. Exiting...\n"; exit(1);}

        }
      else
        {
          if(IsNullMatrix(B) && type=="is_not_equal") //we have 0<>0. false
            return false;
        }


    }

  //cout<<"\nFn: finished\n";
  return true;

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


CDAGNode* DDManager::selectEquationWithLeastCoefficientForVarToElim(set<CDAGNode*> E, string VarToElim, map<string, int> WidthTable, CHelper &MANAGER)
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
  for(set<CDAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    (*it)->getDAGNames(names);

  attachSetToList(names, VarNames);

  createRemainingVarNames(VarNames, RemainingVarNames); // RemainingVarNames = VarNames without first entry

  //showList(VarNames, "VarNames");
  //showList(RemainingVarNames, "RemainingVarNames");

  NoVars=VarNames.size();
  // Now we have the list of variables,  NoVars, NoVarsToElim

  // Get the equations
  bool result = getEquationsAndDisequations(E, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  int EqnToSubstIndex;

  getEquationWithOddCoefForGivenVariable(EqCoefMat, 0, EqnToSubstIndex);

  if(EqnToSubstIndex == -1)
    getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, 0, EqnToSubstIndex, Mod);

  Matrix SelectedEqn;
  copyRow(EqCoefMat, EqnToSubstIndex, SelectedEqn); // selected equation copied into SelectedEqn

  //cout<<"\nSelectedEqn\n";
  //showMatrix(SelectedEqn);

  ULINT SelectedCoef = SelectedEqn[0][0];

  //cout<<"\nSelectedCoef = "<<SelectedCoef<<endl;

  // Currently the equation is like 2^k*cx + M = const
  // Convert this to
  // 1) 2^k*cx = AI(M) + const

  deleteColumn(SelectedEqn, 0); // SelectedEqn is M
  aInverseOfMatrix(SelectedEqn, Mod); // SelectedEqn is AI(M) now

  //cout<<"\nAfter deleting 0th column and taking AI, SelectedEqn\n";
  //showMatrix(SelectedEqn);

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

  //cout<<"\nFinally\n";

  //cout<<"\nSelectedCoef = "<<SelectedCoef<<endl;
  //cout<<"\nConstant_Part = "<<Constant_Part<<endl;
  //cout<<"\nSelectedEqn\n";
  //showMatrix(SelectedEqn);

  CDAGNode* EquationDAG = getEquationDAGFromEqn(SelectedCoef, SelectedEqn, Constant_Part, MANAGER, RemainingVarNames, VarToElim);

  return EquationDAG;

}

int DDManager::getNoOfBitsInConstraint(CDAGNode* root, map<string, int> WidthTable)
{
  // Todo
  set<string> names;
  root->getDAGNames(names);

  set<string>::iterator names_it=names.begin();
  string my_name=*names_it;

  return WidthTable[my_name];
}


// get dag as AX+B : THIS NEEDS TO BE MODIFIED ANY WAY. EXTREMELY OVERSIMPLIFIED LOGIC
void DDManager::getCoefsAndConsts(CDAGNode *root, list<string> VarNames, Matrix &A, Matrix &B)
{
  set<string> names;
  root->getDAGNames(names);
  string Var;
  int element;
  Row MyRow;

  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++)
    {
      Var = *it;
      if(names.find(Var)==names.end())
        element=0;
      else
        element=1;
      MyRow.push_back(element);
    }
  A.push_back(MyRow);
  // A has the coefficients now : Note that logic here is extremely oversimplified. Coefficient is assumed to be 1 if the variable is present in the expressoion. But it seems this will work for leaf level RTL entities

  int constant=0;
  root->getConstant(constant);
  //cout<<"\nconstant="<<constant<<endl;

  Row MyConstRow;
  MyConstRow.push_back(constant);
  B.push_back(MyConstRow);

}

void DDManager::showList(list<string> names, string WhoAmI)
{
  cout<<"\n"<<WhoAmI<<"\n********\n";
  for(list<string>::iterator it=names.begin(); it!=names.end();it++)
    cout<<*it<<endl;
}

/*
// Function to create a DD from a DAG
DDNode* DDManager::createDDFromDAGFromAPs(CDAGNode *dag, CHelper &MANAGER)
{
  string dag_str = dag->Get_DAGnode_value();

  if(dag_str == "equivalent")
    {
      //cout<<"\nCreating DD for "<<dag->getDAGStr()<<endl;
      return createDD(dag);}

  else if(dag_str == "is_not_equal")
    {
      //cout<<"\nCreating DD for "<<dag->getDAGStr()<<endl;
      list<CDAGNode*> :: iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;
      //cout<<"\nleft= "<<left->getDAGStr()<<endl;
      //cout<<"\nright= "<<right->getDAGStr()<<endl;
      CDAGNode* inverseDAG = MANAGER.createDAG("equivalent", left, right);
      //cout<<"\nCreating DD for "<<inverseDAG->getDAGStr()<<endl;
      DDNode* inverseDD = createDD(inverseDAG);
      //cout<<"\ninverseDD = "<<inverseDD->getDDString()<<endl;
      return createDDNot(inverseDD);
    }
  else if(dag_str == "and")
    {
      list<CDAGNode*> :: iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;
      return apply("and", createDDFromDAGFromAPs(left, MANAGER), createDDFromDAGFromAPs(right, MANAGER));
    }
  else if(dag_str == "or")
    {
      list<CDAGNode*> :: iterator iout = dag->Get_DAGnode_outlist();
      CDAGNode *left, *right;
      left = *iout;
      iout++;
      right =*iout;
      return apply("or", createDDFromDAGFromAPs(left, MANAGER), createDDFromDAGFromAPs(right, MANAGER));
    }
  else if(dag_str == "1")
    {
      return createDD(1);
    }
  else if(dag_str == "0")
    {
      return createDD(0);
    }
  else
    {
      cout<<"\ncreateDDFromDAGFromAPs called with dag name "<<dag_str<<". Exiting...\n"; exit(1);
    }
    } */


void DDManager::writeBVDeclarationsInYicesFormat(FILE *smt_file, map<string, int> WidthTable)

{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
    {

      fprintf(smt_file,"(define %s",(it->first).c_str());
      fprintf(smt_file," :: (bitvector %d))\n",it->second);
    }

}

bool DDManager::ThUnsat(set<CDAGNode*> E, set<CDAGNode*> D, map<string, int> WidthTable, CHelper &MANAGER)
{

  //cout<<"\nThUnsat called\n";

  CDAGNode *root = NULL;
  set<CDAGNode*>::iterator it=E.begin();
  for(; it!=E.end(); it++)
    if(root==NULL) root=*it; else root=MANAGER.createDAG("and", root, *it);

  it=D.begin();
  for(; it!=D.end(); it++)
    if(root==NULL) root=*it; else root=MANAGER.createDAG("and", root, *it);

  FILE *smt_file = fopen("thunsat.ys", "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create thunsat.ys\n"; exit(1);}

  //cout<<"\nSMT file opened\n";

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  //cout<<"\nDeclarations written\n";




  /* FOLLOWING LINES WAS COMMENTED AND NEW ONE ADDED ON 18/1/2010 */

  //string dag_str = root->getDAGOfProperSkeltonFormatInYicesFormat("bool");

  string dag_str = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(root, WidthTable);

  /* ADDED ON 18/1/2010 ENDS HERE */




  //cout<<"\nDag written\n";

  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  system("yices -tc thunsat.ys > result_thunsat.txt");

  //cout<<"\nYices called\n";

  FILE *result_smt_file = fopen("result_thunsat.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  //cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::ThUnsat\n"; exit(1);
    }
}


bool DDManager::ThUnsatSingleFormula(CDAGNode* f, map<string, int> WidthTable, CHelper &MANAGER)
{
  FILE *smt_file = fopen("thunsat_single_formula.ys", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);



  /* FOLLOWING LINE WAS COMMENTED AND NEW ONE ADDED ON 18/1/2010 */

  //string dag_str = f->getDAGOfProperSkeltonFormatInYicesFormat("bool");

  string dag_str = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(f, WidthTable);

  /* ADDED ON 18/1/2010 ENDS HERE */




  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  system("yices -tc thunsat_single_formula.ys > result_thunsat_single_formula.txt");
  FILE *result_smt_file = fopen("result_thunsat_single_formula.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_single_formula.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  //cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::ThUnsat\n"; exit(1);
    }
}



// get dag as AX+B : The dag is lhs or rhs in lhs=rhs or lhs<>rhs. It is assumed to be in the form c1*x1+ ... +cn*xn.
void DDManager::getCoefsAndConstsModified(CDAGNode *root, list<string> VarNames, Matrix &A, Matrix &B)
{

  //cout<<"\ngetCoefsAndConstsModified called with root = "<<root->getDAGStr()<<endl;

  set<string> names;
  root->getDAGNames(names);
  string Var;
  ULINT Coefficient;
  Row MyRow;

  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++)
    {
      Var = *it;

      //cout<<"\nVar="<<Var<<endl;

      if(names.find(Var) != names.end())
        {
          //cout<<"\nnames.find(Var) != names.end()\n";
          Coefficient = 1;
          root->getCoefficient(Var, Coefficient);// get coefficient of variable var and store in Coefficient

        }
      else
        {
          //cout<<"\nnames.find(Var) == names.end()\n";
          Coefficient = 0;
        }

      //cout<<"\nCoefficient of "<<Var<<" is "<<Coefficient<<endl;
      MyRow.push_back(Coefficient);
    }
  A.push_back(MyRow);
  // A has the coefficients now

  ULINT constant=0;
  root->getConstantModified(constant);
  //cout<<"\nconstant="<<constant<<endl;

  Row MyConstRow;
  MyConstRow.push_back(constant);
  B.push_back(MyConstRow);

}

void DDManager::showMap(map<DDNode*, DDNode*> RedundantNodes)
{
  cout<<"\nRedundantNodes\n";
  int size = RedundantNodes.size();
  map<DDNode*, DDNode*>::iterator it = RedundantNodes.begin();
  for(int i=0; i<size; i++, it++)
    cout<<"\n"<<it->first<<"\t"<<it->second<<endl;
}

/*
  void DDManager::deleteRedundantNodes(map<DDNode*, DDNode*> OriginalRedundantNodes)
  {
  //cout<<"\nOriginal RedundantNodes MAP\n";

  //showMap(OriginalRedundantNodes);

  map<DDNode*, DDNode*> RedundantNodes;

  recreateDeleteRedundantNodes(OriginalRedundantNodes, RedundantNodes);

  //cout<<"\nRedundantNodes MAP\n";

  //showMap(RedundantNodes);


  //cout<<"\nDD HT presently is \n";
  //HT.showDD_hash_table();

  int size = RedundantNodes.size();
  map<DDNode*, DDNode*>::iterator it = RedundantNodes.begin();
  for(int i=0; i<size; i++, it++)
  {
  cout<<"\nNodes "<<it->first<<", and "<<it->second<<" are equivalent. They are\n";
  (it->first)->showNode();
  (it->second)->showNode();
  cout<<"\nPreparing "<<it->first<<" as replacable by "<<it->second<<endl;

  makeNodeFree(it->first, it->second); // On 4/1/2010
  cout<<"\nDeleting "<<it->first<<" equivalent to "<<it->second<<endl;
  deleteDDnode(it->first);
  cout<<"\nDeletion done\n";
  }

  //cout<<"\nDD HT after deletion of redundant nodes is \n";
  //HT.showDD_hash_table();
  }


  void DDManager :: replaceDDNode(DDNode *candidate, DDNode *substitute)
  {
  makeNodeFree(candidate, substitute);
  deleteDDnode(candidate);
  }


  void DDManager :: deleteDDnode(DDNode* DD_to_be_deleted)
  {
  if(DD_to_be_deleted!=NULL)
  {
  char hash_fn_ip[50] = " " ;
  char temp_char1[10], temp_char2[10], temp_char3[10];
  string temp = " ";

  if(DD_to_be_deleted->getIndex() == MAX_AP+1) // Terminal node
  {
  int value = DD_to_be_deleted->getVal();
  if(value==0) strcpy (hash_fn_ip, "0"); else if(value==1) strcpy (hash_fn_ip, "1");
  HT.Delete_address_of_existing_DDnode_from_ht(hash_fn_ip , DD_to_be_deleted);
  }
  else   // non-terminal node
  {
  CDAGNode *predicate = DD_to_be_deleted->getPredicate();
  DDNode *left = DD_to_be_deleted->getLeft();
  DDNode *right = DD_to_be_deleted->getRight();

  sprintf(temp_char1 , "%x", left);
  sprintf(temp_char2 , "%x", right);
  sprintf(temp_char3 , "%x", predicate);

  string opd_addr1(temp_char1);
  string opd_addr2(temp_char2);
  string pred_addr(temp_char3);

  temp = pred_addr + opd_addr1 + opd_addr2;
  strcpy (hash_fn_ip, temp.c_str());

  HT.Delete_address_of_existing_DDnode_from_ht(hash_fn_ip , DD_to_be_deleted);
  }

  cout << "node deleted :: " << DD_to_be_deleted << endl << endl ;

  // Delete DD_to_be_deleted from DD_to_be_deleted's present left's in_list
  //DDNode* PresentLeft = DD_to_be_deleted->getLeft();

  //if(PresentLeft != NULL)
  //  PresentLeft->Delete_single_entry_from_DD_inlist(DD_to_be_deleted);

  // set left of  DD_to_be_deleted to NULL
  DD_to_be_deleted->setLeft(NULL);

  //DDNode* PresentRight = DD_to_be_deleted->getRight();

  //if(PresentRight != NULL)
  //PresentRight->Delete_single_entry_from_DD_inlist(DD_to_be_deleted);

  // set right of  DD_to_be_deleted to NULL
  DD_to_be_deleted->setRight(NULL);

  //DD_to_be_deleted->setLeft(NULL);
  //DD_to_be_deleted->setRight(NULL);
  delete DD_to_be_deleted;
  }
  else
  {
  cout << endl << "ERROR :: Trying to delete NULL pointer in function deleteDDnode" << endl;
  exit(1);
  }
  }*/


/*
unsigned long DDManager :: getHTSize()
{
  return HT.hash_table_size();
}
*/


void DDManager::findDropVarsStep(DDNode *v, map<string, int> &VarsToElimCount)
{
  //cout<<"\nfindDropVarsStep called with "<<v<<"\n";
  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      //cout<<"\nAlready visited\n";
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {
      //cout<<"\nNot already visited\n";

      if(v->getIndex()<=MAX_AP)// non-terminal
        {
          findDropVarsStep(v->getLeft(), VarsToElimCount);
          findDropVarsStep(v->getRight(), VarsToElimCount);
          updateVarsToElimCount(v->getPredicate(), VarsToElimCount);
        }
      SetOfDDNodes.insert(v);
    }
}

void DDManager::updateVarsToElimCount(CDAGNode *predicate, map<string, int> &VarsToElimCount)
{
  map<string, int>::iterator it = VarsToElimCount.begin();

  for(;it!=VarsToElimCount.end(); it++)
    {
      if(!predicate->freeOfVarModified(it->first))
        it->second = it->second+1;
    }
}

void DDManager::findDropVars(DDNode *v, list<string> VarsToElim, set<string> &DropVars)
{
  map<string, int> VarsToElimCount;

  for(list<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      string var = *it;
      VarsToElimCount.insert(make_pair(var, 0));
    }

  SetOfDDNodes.clear();
  findDropVarsStep(v, VarsToElimCount);

  map<string, int>::iterator mit = VarsToElimCount.begin();

  for(;mit!=VarsToElimCount.end(); mit++)
    {
      if(mit->second <= 1)
        {
          DropVars.insert(mit->first);
        }
    }
}

string DDManager::chooseVar(DDNode *v, list<string> VarsToElim)
{
  map<string, int> VarsToElimCount;

  for(list<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      string var = *it;
      VarsToElimCount.insert(make_pair(var, 0));
    }

  SetOfDDNodes.clear();
  findDropVarsStep(v, VarsToElimCount);

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
  return mit_selected->first;
}


DDNode* DDManager::DropVariables(set<string> DropVars, DDNode* f, HashTable <string, DDNode*> *PtrMapForDropVars, CHelper &MANAGER, map<string, int> WidthTable)
{
  //cout<<"\nDropVariables called with node "<<f<<"\n";


  char temp_char[10];
  sprintf(temp_char , "%x", f);
  string key = temp_char;
  
  // key created

  HTStatusValue<DDNode*> ResultFromHT = PtrMapForDropVars->hashtable_search(key);

  if(ResultFromHT.success())
    {
      return ResultFromHT.GetValue();
    }
 
  DDNode* result;

  //cout<<"\nNode "<<f<<" is first time encountered\n";

  if(f->getIndex()==MAX_AP+1) // Terminal node
    {
      //cout<<"\nTerminal node with value "<<f->getVal()<<" reached\n";
      result = f;
    }
  else
    {
      //cout<<"\nGetting predicate C\n";
      CDAGNode* C = f->getPredicate();
      //cout<<"\npredicate obtained is "<<C->getDAGStr()<<endl;
      if(freeOfVars(C, DropVars))
	{
	  //cout<<"\nC is free of all variables in DropVars"<<endl;

	  DDNode *thenb = DropVariables(DropVars, f->getRight(), PtrMapForDropVars, MANAGER, WidthTable);
	  DDNode *elseb = DropVariables(DropVars, f->getLeft(), PtrMapForDropVars, MANAGER, WidthTable);

	  // changed on 2/1/2010 for improving efficiency

	  DDNode *C_dd = createDD(C, WidthTable, MANAGER);
	  //return createITE(C_dd, thenb, elseb);
	  result = createITEModified(C_dd, thenb, elseb);

	  // changed on 2/1/2010 for improving efficiency ends here
	}
      else
	{
	  //cout<<"\nC is not free of all variables in DropVars"<<endl;

	  DDNode *thenb = DropVariables(DropVars, f->getRight(), PtrMapForDropVars, MANAGER, WidthTable);
	  DDNode *elseb = DropVariables(DropVars, f->getLeft(), PtrMapForDropVars, MANAGER, WidthTable);

	  //result = apply("or", thenb, elseb);
	  result = applyModified("or", thenb, elseb);
	}
    }

  
  ResultFromHT = PtrMapForDropVars->hashtable_insert(key, result);
  if(!ResultFromHT.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::DropVariables\n";
      exit(1);
    }
  
  return result;
}


bool DDManager::freeOfVars(CDAGNode *predicate, set<string> DropVars)
{
  set<string>::iterator it = DropVars.begin();

  for(;it!=DropVars.end(); it++)
    {
      if(!predicate->freeOfVarModified(*it)) // not free
        return false;
    }

  return true;
}

DDNode* DDManager::callQElim(string VarToElim, DDNode* f, CHelper &MANAGER, map<string, int> WidthTable)
{
  // NB : QElimWithDP calls APResolve() internally. Also both the functions use inefficient DP tables. QElimWithDPModified calls APResolveModified() in place of APResolve(). Also the DP tables are hash tables passed to the functions as pointers



  TableForDagResolve.clear();

  // COMMENTED ON 26/1/2010 AS APResolveModified() is used in place of APResolve() now
  //TableForAPResolve.clear();

  set<CDAGNode*> E, D;
  E.clear(); D.clear();

  // CHANGED ON 25/1/2010


  //TableForQElim.clear();
  //return QElimWithDP(VarToElim, f, E, D, MANAGER, WidthTable);

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;

  return QElimWithDPModified(VarToElim, f, E, D, MANAGER, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);

  // CHANGED ON 25/1/2010 ENDS HERE
}

DDNode* DDManager::MultiVarQElimWithStrategy(list<string> VarsToElim, DDNode* f, CHelper &MANAGER, map<string, int> WidthTable)
{
  time_t drop_time, find_time, t2;

  DDNode *result;
  result = f;

  //cout<<"\nresult before the loop = "<<result->getDDString()<<endl;

  while(!VarsToElim.empty())
    {
      showList(VarsToElim, "VarsToElim");
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

	  result = DropVariables(DropVars, result, PtrMapForDropVars, MANAGER, WidthTable);  

	  time(&t2);
          //cout<<"\nResult after Eliminating variables in DropVars = "<<result->getDDString()<<endl;

          cout<<"\n\nTime spent in DropVariables  = "<< (t2 - drop_time)<<" SECONDS "<<endl<<endl;

          //CDAGNode *temp = createDAGFromDD(result, MANAGER);
          //cout<<"\nResult after Eliminating variables in DropVars in DAG form ="<<temp->getDAGStr()<<endl;

          for(set<string>::iterator it=DropVars.begin(); it != DropVars.end(); it++)
            VarsToElim.remove(*it);

          cout<<"\nVariables dropped\n";
          continue;
        }

      cout<<"\nNo more variables to drop\n";

      string VarToElim = chooseVar(result, VarsToElim);

      cout<<"\nVariable chosen to eliminate = "<<VarToElim<<endl;

      // Temporary on 14/1/10

      //CDAGNode *temp = createDAGFromDD(result, MANAGER);
      //cout<<"\nResult after Eliminating variables in DropVars in DAG form ="<<temp->getDAGStr()<<endl;

      // Temporary on 14/1/10 ends here


      time(&drop_time);
      result = callQElim(VarToElim, result, MANAGER, WidthTable);
      time(&t2);

      // Temporary to see time spent in eliminating a variable

      cout<<"\n\nTime spent in callQElim to eliminate "<<VarToElim<<"  = "<< (t2 - drop_time)<<" SECONDS "<<endl<<endl;

      cout<<"\nVariable eliminated = "<<VarToElim<<endl;

      int NoOfDDNodes = 0;
      SetOfDDNodes.clear();
      getNoOfNodesInDD(result, NoOfDDNodes);

      cout<<"\nNumber of nodes in the output DD = "<<NoOfDDNodes<<endl;

      //CDAGNode* dag_result = createDAGFromDD(result, MANAGER);

      //cout<<"\ndag_result = "<<dag_result->getDAGStr()<<endl;

      //exit(1);

      // Temporary to see time spent in eliminating a variable ends here


      //cout<<"\nresult = "<<result->getDDString()<<endl;
      VarsToElim.remove(VarToElim);
    }
  return result;
}


void DDManager::showSet(set<string> MySet, string WhoAmI)
{
  cout<<"\n"<<WhoAmI<<"\n";
  for(set<string>::iterator it=MySet.begin(); it != MySet.end(); it++)
    cout<<(*it)<<endl;
}



/*  Replaced by ...Modified ones
  map< set<CDAGNode*>, DDNode*>::iterator DDManager::getEntryFromTableForAPResolve(set<CDAGNode*> E, set<CDAGNode*> D)
  {
  map< set<CDAGNode*>, DDNode*>::iterator it = TableForAPResolve.begin();
  for(; it != TableForAPResolve.end(); it++)
  {
  set<CDAGNode*> existaps = it->first;
  set<CDAGNode*> newaps;
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(newaps, newaps.begin()));
  set<CDAGNode*> diff1, diff2;
  set_difference(newaps.begin(), newaps.end(), existaps.begin(), existaps.end(),inserter(diff1, diff1.begin()));
  set_difference(existaps.begin(), existaps.end(), newaps.begin(), newaps.end(),inserter(diff2, diff2.begin()));
  if(diff1.empty() && diff2.empty()) // existing = new. match found
  return it;
  }
  return it;
  }

  void DDManager::insertEntryIntoTableForAPResolve(set<CDAGNode*> E, set<CDAGNode*> D, DDNode* resultDD)
  {
  set<CDAGNode*> newaps;
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(newaps, newaps.begin()));
  TableForAPResolve.insert(make_pair(newaps, resultDD));
  }

*/


CDAGNode* DDManager::selectEquationWithLeastCoefficientForVarToElimAdvanced(set<CDAGNode*> E, set<CDAGNode*> D, string VarToElim, map<string, int> WidthTable, CHelper &MANAGER)
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
  for(set<CDAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    (*it)->getDAGNames(names);

  for(set<CDAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    (*it)->getDAGNames(names);

  attachSetToList(names, VarNames);

  createRemainingVarNames(VarNames, RemainingVarNames); // RemainingVarNames = VarNames without first entry
  //showList(VarNames, "VarNames");
  //showList(RemainingVarNames, "RemainingVarNames");

  NoVars=VarNames.size();
  // Now we have the list of variables,  NoVars, NoVarsToElim


  //cout<<"\nVariables found\n";


  set<CDAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);


  //cout<<"\ngetEquationsAndDisequations called\n";


  if(EqCoefMat.empty()) return NULL;

  ULINT Mod=findPower(NoOfBits);

  int EqnToSubstIndex;

  getEquationWithOddCoefForGivenVariable(EqCoefMat, 0, EqnToSubstIndex);

  if(EqnToSubstIndex == -1)
    getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, 0, EqnToSubstIndex, Mod);

  Matrix SelectedEqn;
  copyRow(EqCoefMat, EqnToSubstIndex, SelectedEqn); // selected equation copied into SelectedEqn

  //cout<<"\nEqn selected\n";
  //showMatrix(SelectedEqn);

  ULINT SelectedCoef = SelectedEqn[0][0];

  //cout<<"\nSelectedCoef = "<<SelectedCoef<<endl;

  // Currently the equation is like 2^k*cx + M = const
  // Convert this to
  // 1) 2^k*cx = AI(M) + const

  deleteColumn(SelectedEqn, 0); // SelectedEqn is M
  aInverseOfMatrix(SelectedEqn, Mod); // SelectedEqn is AI(M) now

  //cout<<"\nAfter deleting 0th column and taking AI, SelectedEqn\n";
  //showMatrix(SelectedEqn);

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

  //cout<<"\nFinally\n";

  //cout<<"\nSelectedCoef = "<<SelectedCoef<<endl;
  //cout<<"\nConstant_Part = "<<Constant_Part<<endl;
  //cout<<"\nSelectedEqn\n";
  //showMatrix(SelectedEqn);

  CDAGNode* EquationDAG = getEquationDAGFromEqn(SelectedCoef, SelectedEqn, Constant_Part, MANAGER, RemainingVarNames, VarToElim);


  //cout<<"\nReturning EqnDAG\n";

  return EquationDAG;

}


// Functions for direct ITE creation ADDED ON 2/1/2010

/*

DDNode* DDManager::createITE(DDNode* condn, DDNode* then, DDNode* els)
{

  //cout<<"\ncreateITE called\n";

  TableForITE.clear();

  DDNode* result = applyITE(condn, then, els);

  //cout<<"\napplyITE Completed\n";
  //cout<<"\nNow result="<<result->getDDString()<<endl;
  //printGraphInDotFormat(result, "before_reduce.dot");

  //map<DDNode*, DDNode*> RedundantNodes;
  //RedundantNodes.clear();
  //cout<<"\nCalling reduce\n"<<endl;

  //DDNode* reducedResult = Reduce(result, RedundantNodes);

  //DDNode* reducedResult = ReduceModified(result);

  DDNode* reducedResult = ReduceModifiedAdvanced(result);

  //DDNode* reducedResult = result;


  //cout<<"\nreduced result="<<reducedResult->getDDString()<<endl;

  //deleteRedundantNodes(RedundantNodes);

  //cout<<"\nDeletion of redundant nodes is done\n";

  //printGraphInDotFormat(reducedResult, "after_reduce.dot");


  //cout<<"\ncreateITE finished\n";

  return reducedResult;
}

DDNode* DDManager::applyITE(DDNode* condn, DDNode* then, DDNode* els)
{
  DDNode* u;

  //cout<<"\napplyITE called\n";

  //cout<<"\nChecking map\n";

  map< pair< pair<DDNode*, DDNode*>, DDNode*> , DDNode*>::iterator TableForITE_it = TableForITE.find(make_pair(make_pair(condn,then), els));

  if(TableForITE_it != TableForITE.end())
    {
      //cout<<"\nTableForITE HIT\n";
      u=TableForITE_it->second;

      //cout<<"\napplyITE finished\n";

      return u;// {have already evaluated}
    }

  //cout<<"\nTableForITE MISS\n";

  u = evaluateITEAdvanced(condn, then, els);

  if(u!=NULL)
    {
      //cout<<"\napplyITE finished\n";
      return u;
    }

  int u_value, u_index;
  CDAGNode *u_AP;


  //u_value = evaluateITE(condn, then, els);
  //if(u_value != -1)
  //  {
  //{create terminal vertex}

  //   return createDD(u_value);
  //  }

  // NEWLY ADDED ON 2nd Jan 10
  //  else if(partiallyEvaluatableIte(condn, then, els))
  //   {
  //   u = partialEvaluationIte(condn, then, els);

     //TableForApply.insert(make_pair(make_pair(v1,v2), u));

  //  return u;
     //   }
  // NEWLY ADDED ON 2nd Jan 10 ends here

  // else //{create nonterminal and evaluate further down}
  //  {

  int condn_index, then_index, else_index;
  condn_index = condn->getIndex();
  then_index = then->getIndex();
  else_index = els->getIndex();

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



  u_low = applyITE(condn_next_low, then_next_low, else_next_low);
  u_high = applyITE(condn_next_high, then_next_high, else_next_high);

  u = createDD(u_AP, u_low, u_high);

  TableForITE.insert(make_pair(make_pair(make_pair(condn,then), els), u));
  //{add vertex to table}

  //cout<<"\napplyITE finished\n";
  return u;
  //   }
}


*/



int DDManager::evaluateITE(DDNode *condn, DDNode *then, DDNode *els)
{
  if(condn->getPredicate()!= NULL || then->getPredicate()!= NULL || els->getPredicate()!= NULL) return -1;

  int c;
  int t;
  int e;

  c = condn->getVal();
  t = then->getVal();
  e = els->getVal();

  if(c==1 && t==1) return 1;
  if(c==0 && e==1) return 1;
  return 0;

}

DDNode* DDManager::evaluateITEAdvanced(DDNode *condn, DDNode *then, DDNode *els)
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


// NEWLY ADDED ON 3/1/2010

/*

Replaced by QElimWithDPModified

DDNode* DDManager::QElimWithDP(string VarToElim, DDNode* f, set<CDAGNode*> E, set<CDAGNode*> D, CHelper &MANAGER, map<string, int> WidthTable)
{
  map< pair<DDNode*, set<CDAGNode*> >, DDNode*>::iterator tqit = getEntryFromTableForQElim(f, E, D);

  if(tqit != TableForQElim.end())
    {
      //cout<<"\nQElim's Table hit\n";
      //cout<<"\nResult returned = "<<createDAGFromDD(tqit->second, MANAGER)->getDAGStr()<<endl;
      return tqit->second;
    }

  //cout<<"\nQElim's Table miss\n";
  if(f->getIndex()==MAX_AP+1) // Terminal node
    {
      DDNode* result;

      int value = f->getVal();

      //cout<<"\nTerminal node reached with value = "<<value<<endl;
      if(value == 0)
        {
          //cout<<"\nZero returned\n";
          result = f;
        }
      else
        {
          //cout<<"\nTerminal node reached with value = "<<value<<endl;

          if(E.empty() && D.empty())
            {
              //cout<<"\nE, D are empty\n";
              result = createDD(1);
            }
          else if(ThUnsat(E, D, WidthTable, MANAGER))
            {
              result = createDD(0);
            }
          else
            {
              result = APResolve(E, D, VarToElim, WidthTable, MANAGER);
            }
        }

      insertEntryIntoTableForQElim(f, E, D, result);


      //cout<<"\nResult returned = "<<createDAGFromDD(result, MANAGER)->getDAGStr()<<endl;

      return result;
    }
  else
    {

      //cout<<"\nChecking if E or D is empty\n";

      if (!E.empty() || !D.empty())
        {
          //cout<<"\n!E.empty() or !D.empty()"<<endl;
          CDAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable, MANAGER);
          if(e1 != NULL)
            {
              //cout<<"\nEquation Selected = "<<e1->getDAGStr()<<endl;
              //cout<<"\nDAGResolve() called"<<endl;
              //cout<<"\nf="<<f->getDDString()<<endl;

              // Following line commented and new line added on 18/1/2010

              //f = DAGResolve(f, e1, VarToElim, MANAGER);


              //f = DagResolveModified(f, e1, VarToElim, MANAGER);


              f = DagResolveModifiedAdvanced(f, e1, VarToElim, MANAGER, WidthTable);

              //cout<<"\nf after DAGResolve ="<<f->getDDString()<<endl;
              //cout<<"\nDAGResolve() finished"<<endl;
              if(DDFreeOfVar(f, VarToElim))
                {
                  //cout<<"\nf is free of "<<VarToElim<<endl;
                  //cout<<"\nAPResolve() called"<<endl;
                  DDNode *temp;
                  if(ThUnsat(E, D, WidthTable, MANAGER)) temp = createDD(0);
                  else temp = APResolve(E, D, VarToElim, WidthTable, MANAGER);

                  //cout<<"\nAPResolve() finished"<<endl;

                  //cout<<"\nAPResolve returns "<<temp->getDDString()<<endl;
                  temp = apply("and", f, temp);
                  //cout<<"\nDAGResolve & APResolve ="<<temp->getDDString()<<endl;
                  //exit(1); //temporary

                  //cout<<"\nDAGResolve & APResolve returned"<<endl;

                  insertEntryIntoTableForQElim(f, E, D, temp);


                  //cout<<"\nResult returned = "<<createDAGFromDD(temp, MANAGER)->getDAGStr()<<endl;
                  return temp;
                }
            }
        }

      //cout<<"\nGetting predicate C\n";
      CDAGNode* C = f->getPredicate();
      //cout<<"\npredicate obtained is "<<C->getDAGStr()<<endl;
      if(C->freeOfVarModified(VarToElim))
        {
          DDNode *temp;

          //cout<<"\nf is free of "<<VarToElim<<endl;

          DDNode *thenb = QElimWithDP(VarToElim, f->getRight(), E, D, MANAGER, WidthTable);
          DDNode *elseb = QElimWithDP(VarToElim, f->getLeft(), E, D, MANAGER, WidthTable);

          //return apply("or", apply("and", createDD(C) , thenb) , apply("and", createDDNot(createDD(C)) ,elseb));  COMMENTED ON 2/1/2010
          DDNode* C_dd = createDD(C);
          temp = createITE(C_dd, thenb, elseb);
          insertEntryIntoTableForQElim(f, E, D, temp);

          //cout<<"\nResult returned = "<<createDAGFromDD(temp, MANAGER)->getDAGStr()<<endl;
          return temp;
        }
      else
        {
          DDNode *temp;

          //cout<<"\nf is not free of "<<VarToElim<<endl;

          set<CDAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C, MANAGER));
          DDNode *thenb = QElimWithDP(VarToElim, f->getRight(), E_new, D, MANAGER, WidthTable);
          DDNode *elseb = QElimWithDP(VarToElim, f->getLeft(), E, D_new, MANAGER, WidthTable);
          temp = apply("or", thenb, elseb);
          insertEntryIntoTableForQElim(f, E, D, temp);

          //cout<<"\nResult returned = "<<createDAGFromDD(temp, MANAGER)->getDAGStr()<<endl;
          return temp;
        }
    }
}

*/


 /*
map< pair<DDNode*, set<CDAGNode*> >, DDNode*>::iterator DDManager::getEntryFromTableForQElim(DDNode*f, set<CDAGNode*> E, set<CDAGNode*> D)
{
  map< pair<DDNode*, set<CDAGNode*> >, DDNode*>::iterator it = TableForQElim.begin();
  for(; it != TableForQElim.end(); it++)
    {
      set<CDAGNode*> existaps = (it->first).second;
      DDNode* existf = (it->first).first;

      set<CDAGNode*> newaps;
      set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(newaps, newaps.begin()));
      set<CDAGNode*> diff1, diff2;
      set_difference(newaps.begin(), newaps.end(), existaps.begin(), existaps.end(),inserter(diff1, diff1.begin()));
      set_difference(existaps.begin(), existaps.end(), newaps.begin(), newaps.end(),inserter(diff2, diff2.begin()));
      if(diff1.empty() && diff2.empty() && existf == f) // existing = new. match found
        return it;
    }
  return it;
}

void DDManager::insertEntryIntoTableForQElim(DDNode*f, set<CDAGNode*> E, set<CDAGNode*> D, DDNode* resultDD)
{
  set<CDAGNode*> newaps;
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(newaps, newaps.begin()));
  TableForQElim.insert(make_pair(make_pair(f, newaps), resultDD));
} */


// NEWLY ADDED ON 3/1/2010  ENDS HERE

CDAGNode* DDManager::Quantifier_Eliminator_With_Redundant_Selection_Removal(CDAGNode *root, CHelper &MANAGER, map<string, int> &WidthTable)
{

  time_t qelim_start, qelim_end, qelim_duration;
  time(&qelim_start);

  unsigned long long int qelim_duration_ms;
  struct timeval qelim_start_ms, qelim_end_ms;
  gettimeofday(&qelim_start_ms, NULL);





  map<CDAGNode*, CDAGNode*> LinearizationTable;
  map<CDAGNode*, CDAGNode*> SubtractionTable;
  int AutoNameCount = 0;


  // Separating the root into exists part and dag part

  if(root->Get_DAGnode_value() != "exists")
    {
      cout<<"\nNo variables to eliminate. Return\n";
      return root;
    }

  set<string> VarsToElimOriginal;
  set<string> VarsToElimRenamed;
  root->getExistsDAGNames(VarsToElimOriginal);
  list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
  iout++;
  CDAGNode *DagPart = *iout;



  // Linearizing the dag part


  cout<<"\nVarsToElimOriginal\n";
  set<string>::iterator vit =  VarsToElimOriginal.begin();
  for(;vit !=  VarsToElimOriginal.end(); vit++)
    {
      cout<<*vit<<endl;
    }

  //cout<<"\nDag part = "<<DagPart->getDAGStr()<<"\n";

  DagPart = MANAGER.getPreprocessedClone(DagPart);

  //cout<<"\nDag part after replacement of predicates = "<<DagPart->getDAGStr()<<"\n";


  cout<<"\nWidth table\n";
  map<string, int>::iterator wit =  WidthTable.begin();
  for(;wit !=  WidthTable.end(); wit++)
    {
      cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
    }


  /* COMMENTING Removing unneeded selections AS IT IS ALREADY DONE IN CSymbSimulator.cpp


  cout<<"\nRemoving unneeded selections\n";

  set<CDAGNode*> UnneededSelects;

  int width = MANAGER.findUnneededSelections(DagPart, UnneededSelects, false, WidthTable);


  cout<<"\nUnneeded Selects are\n";

  for(set<CDAGNode*>::iterator it = UnneededSelects.begin(); it != UnneededSelects.end(); it++)
    {
      cout<<"\n"<<*it<<"\t"<<(*it)->getDAGStr()<<endl;
    }


  DagPart = MANAGER.getSelectionsRemovedClone(DagPart, UnneededSelects);

  cout<<"\nDAG after removing unneeded selections = "<<DagPart->getDAGStr()<<endl;

  //if(true) {exit(1);}

  */
  

  cout<<"\nGetting the linearized dag\n";

  CDAGNode* LinearDag = MANAGER.getLinearizedClone(DagPart, LinearizationTable, AutoNameCount, WidthTable, VarsToElimOriginal, VarsToElimRenamed);

  //cout<<"\nLinearDag = "<<LinearDag->getDAGStr()<<endl;

  cout<<"\nLinearization table\n";
  map<CDAGNode*, CDAGNode*>::iterator lit =  LinearizationTable.begin();
  for(;lit !=  LinearizationTable.end(); lit++)
    {
      cout<<"\n"<<(lit->first)->getDAGStr()<<"\t"<<(lit->second)->getDAGStr()<<endl;
    }


  cout<<"\nWidth table\n";
  wit =  WidthTable.begin();
  for(;wit !=  WidthTable.end(); wit++)
    {
      cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
    }



  SubtractionTable.clear();
  LinearDag = MANAGER.getSubtractionReplacedClone(LinearDag, SubtractionTable, WidthTable);
  //cout<<"\nAfter replacing subtractions by additions, LinearDag = "<<LinearDag->getDAGStr()<<endl;


  //if(true) {exit(1);}


  cout<<"\nVarsToElimRenamed\n";
  vit =  VarsToElimRenamed.begin();
  for(;vit !=  VarsToElimRenamed.end(); vit++)
    {
      cout<<*vit<<endl;
    }


  // Creating VarsToElim

  list<string> VarsToElim;

  /*

  vit =  VarsToElimOriginal.begin();
  for(;vit !=  VarsToElimOriginal.end(); vit++)
  {
  VarsToElim.push_back(*vit);
  }


  vit =  VarsToElimRenamed.begin();
  for(;vit !=  VarsToElimRenamed.end(); vit++)
  {
  VarsToElim.push_back(*vit);
  } */


  set<string> names;
  LinearDag->getDAGNames(names);
  for(set<string>::iterator it=names.begin(); it != names.end(); it++)
    {

      string name = *it;
      //cout<<"\nname = "<<name<<endl;
      if( VarsToElimOriginal.find(name) != VarsToElimOriginal.end() || VarsToElimRenamed.find(name) != VarsToElimRenamed.end())
        VarsToElim.push_back(name);
    }



  cout<<"\nVarsToElim\n";

  for(list<string>::iterator vars_it = VarsToElim.begin();vars_it !=  VarsToElim.end(); vars_it++)
    {
      cout<<*vars_it<<endl;
    }


  MANAGER.checkForPropSkelton(LinearDag);

  cout<<"\n\nDag's structure confirmed to be propositional\n\n";


  bool input_needed = true;

  if(input_needed) 
    {
      MANAGER.printQEliminatorResultsInFile(LinearDag, WidthTable, VarsToElim);
    }


  //if(true) {exit(1);}


  // dag-->dd, MultiVarQElimWithStrategy(dd), dd-->dag


  time_t initial_dd_creation_time, t2;
  time(&initial_dd_creation_time);


  bool call_create_dd_from_dag_with_hashing = true;
  
  DDNode* dd;

  if(call_create_dd_from_dag_with_hashing)
    {
      dd = createDDFromDAGWithHashing(LinearDag, MANAGER, WidthTable);
    }
  else
    {
      dd = createDDFromDAG(LinearDag, MANAGER, WidthTable);
    }

  time(&t2);

  cout<<"\nInitial DD created in "<< (t2 - initial_dd_creation_time) <<" seconds"<<endl;


  //if(true) {printAPMap();  exit(1);}


  //cout<<"\ndd = "<<dd->getDDString()<<endl;


  // Cross-checking if the DD is created properly

  if(false)
    {

      CDAGNode* dag_back = createDAGFromDD(dd, MANAGER);
      //cout<<"\ndag_back="<<dag_back->getDAGStr()<<endl;


      CDAGNode *cross_check = MANAGER.createDAG("is_not_equal", LinearDag, dag_back);


      // Temporarily commenting




      if(ThUnsatSingleFormula(cross_check, WidthTable, MANAGER))
        {
          cout<<"\ndag = dag_back\n"; // correct

        }
      else
        {
          cout<<"\ndag <> dag_back\n";
          exit(1);
        }
     }



  // Find the no: of nodes in the input DD

  int NoOfDDNodes = 0;
  SetOfDDNodes.clear();
  getNoOfNodesInDD(dd, NoOfDDNodes);

  cout<<"\nNumber of nodes in the input DD = "<<NoOfDDNodes<<endl;

  //int userchar = getchar(); //added newly


  //if(true) {if(NoOfDDNodes>=10000) exit(1);}


  time_t var_elim_start_time;
  time(&var_elim_start_time);

  DDNode* dd_eliminated = MultiVarQElimWithStrategy(VarsToElim, dd, MANAGER, WidthTable);

  time(&t2);
  cout<<"\n\nVARIABLE ELIMINATION TIME  = "<< (t2 - var_elim_start_time)<<" SECONDS "<<endl<<endl;

  NoOfDDNodes = 0;
  SetOfDDNodes.clear();
  getNoOfNodesInDD(dd_eliminated, NoOfDDNodes);
  cout<<"\nNumber of nodes in the output DD = "<<NoOfDDNodes<<endl;


  // Newly added on 04 Nov 2010 

  if(elimination_of_inconsistent_paths)
    {
      dd_eliminated = eliminateInconsistentPaths(dd_eliminated, WidthTable, MANAGER);
    }

  // Newly added on 04 Nov 2010  ends here




  //if(true) {exit(1);}


  //cout<<"\ndd_eliminated = "<<dd_eliminated->getDDString()<<endl;

  CDAGNode* dag_eliminated = createDAGFromDD(dd_eliminated, MANAGER);

  //cout<<"\nOriginal dag="<<LinearDag->getDAGStr()<<endl;

  //cout<<"\ndag_eliminated="<<dag_eliminated->getDAGStr()<<endl;

  //cout<<"\nHT Size = "<<getHTSize()<<endl;



  // Getting both original dag and dag_eliminated in files to see if dag_eliminated = exists(V, dag)

  // THIS CODE IS TEMPOARAY
  /*
    FILE *check_file = fopen("check.ys", "w");
    writeBVDeclarationsInYicesFormat(check_file, WidthTable);
    string dag_back_str = dag_back->getDAGOfProperSkeltonFormatInYicesFormat("bool");
    string dag_eliminated_str = dag_eliminated->getDAGOfProperSkeltonFormatInYicesFormat("bool");

    fprintf(check_file, "\nOriginal\n%s\n", dag_back_str.c_str());
    fprintf(check_file, "\nQE\n%s\n", dag_eliminated_str.c_str());
    fclose(check_file);

    exit(1); */

  // TEMPOARAY CODE ENDS HERE

  // CALL To CHECK EXACTNESS


  if(false)
   {


      bool exact = MANAGER.checkForExactness( LinearDag, dag_eliminated, WidthTable, VarsToElim);

      if(exact)
        {
          cout<<"\nExact\n";
          //exit(1);
        }
      else
        {
          cout<<"\nInexact\n";
          exit(1);
        }
    }
      
  //  CALL To CHECK EXACTNESS ENDS HERE

  bool results_needed = false;

  if(results_needed)
    {
      MANAGER.printQEliminatorResultsInFile(LinearDag, WidthTable, VarsToElim, (t2 - var_elim_start_time));
    }

  cout<<"\nNon linearizing the dag\n";

  CDAGNode* dag_eliminated_orig;

  if(!LinearizationTable.empty())
    {
      cout<<"\nLinearization table\n";
      lit =  LinearizationTable.begin();
      for(;lit !=  LinearizationTable.end(); lit++)
        {
          cout<<"\n"<<(lit->first)->getDAGStr()<<"\t"<<(lit->second)->getDAGStr()<<endl;
        }

      dag_eliminated_orig = MANAGER.getOriginalClone(dag_eliminated, LinearizationTable);
    }
  else
    {
      dag_eliminated_orig = dag_eliminated;
    }

  
  printAPMap();
  

  //cout<<"\ndag_eliminated_orig="<<dag_eliminated_orig->getDAGStr()<<endl;

  
  time(&qelim_end);
  qelim_duration = qelim_end - qelim_start;
  qe_time = qe_time + qelim_duration;



  gettimeofday(&qelim_end_ms, NULL);
  qelim_duration_ms = qelim_end_ms.tv_sec * 1000 + qelim_end_ms.tv_usec / 1000;
  qelim_duration_ms-= qelim_start_ms.tv_sec * 1000 + qelim_start_ms.tv_usec / 1000;
  qe_time_ms = qe_time_ms + qelim_duration_ms;	



  return dag_eliminated_orig;

}






// NEWLY ADDED ON 4/1/2010

/*

void DDManager::makeNodeFree(DDNode* candidate, DDNode* substitute)
{
set<DDNode*>::iterator candidate_in_list_itr = candidate->Get_DDnode_inlist();
unsigned int candidate_in_list_size = candidate->Get_DDnode_inlist_size();

for(int i=0; i<candidate_in_list_size; i++, candidate_in_list_itr++)
{
DDNode *parent = *candidate_in_list_itr;
//cout<<endl<<parent<<" points to "<<candidate<<endl;

if(parent->getLeft() != candidate && parent->getRight() != candidate)
{
cout<<"\nErraneous in_list.\n";
cout<<"\n"<<parent<<" is present in in_list of "<<candidate<<", but "<<candidate<<" is neither left or right of "<<parent<<endl;
cout<<"\nThey are\n";
cout<<"\nparent : \n";
parent->showNode();
cout<<"\ncandidate : \n";
candidate->showNode();

exit(1);
}
if(parent->getLeft() == candidate)
{
parent->setLeft(substitute);
substitute->Append_DDnode_inlist(parent);
candidate->Delete_single_entry_from_DD_inlist(parent);
}
if(parent->getRight() == candidate)
{
parent->setRight(substitute);
substitute->Append_DDnode_inlist(parent);
candidate->Delete_single_entry_from_DD_inlist(parent);
}
}
}


void DDManager::recreateDeleteRedundantNodes(map<DDNode*, DDNode*> RedundantNodesOriginal, map<DDNode*, DDNode*> &RedundantNodesNew)
{
RedundantNodesNew.clear();

map<DDNode*, DDNode*>::iterator it = RedundantNodesOriginal.begin();

for(; it != RedundantNodesOriginal.end(); it++)
{
DDNode *candidate = it->first;
DDNode *substitute = it->second;

map<DDNode*, DDNode*>::iterator it2 = RedundantNodesOriginal.find(substitute);

while(it2 != RedundantNodesOriginal.end())
{
substitute = it2->second;
it2 = RedundantNodesOriginal.find(substitute);
}

// substitute obtained

RedundantNodesNew.insert(make_pair(candidate, substitute));
}
}
*/





// NEWLY ADDED ON 4/1/2010  ENDS HERE


void DDManager::sortQ(map<pair<int, int>, DDNode*> Q)
{
  //static int count = 0;

  //if(count>100) return;

  map<int, map<int, DDNode*> > SortedMap;
  map<int, map<int, DDNode*> >::iterator SortedMap_it;

  cout<<"\nOriginal Map is\n";

  for(map<pair<int, int>, DDNode*>::iterator it = Q.begin(); it != Q.end(); it++)
    {
      // Get the elements
      pair<int, int> key = it->first;
      int key1 = key.first;
      int key2 = key.second;

      DDNode* element = it->second;

      cout<<key1<<"\t"<<key2<<"\t"<<element<<"\n";

      // Put into sorted map

      SortedMap_it = SortedMap.find(key1);
      if(SortedMap_it == SortedMap.end()) // key1 is not present already
        {
          // Create an entry for key1
          map<int, DDNode*> InternalMap;
          InternalMap.insert(make_pair(key2, element)); // InternalMap is ready

          // Insert (key1, InternalMap) into SortedMap
          SortedMap.insert(make_pair(key1, InternalMap));
        }

      else // key1 is present already
        {
          // InternalMap is SortedMap_it->second

          (SortedMap_it->second).insert(make_pair(key2, element)); // Insert into InternalMap
        }
    }

  // SortedMap is ready
  // Just go through it

  cout<<"\nSorted Map is\n";

  for(SortedMap_it = SortedMap.begin(); SortedMap_it != SortedMap.end(); SortedMap_it++)
    {
      int key1 = SortedMap_it->first;

      for(map<int, DDNode*>::iterator InternalMap_it = (SortedMap_it->second).begin(); InternalMap_it != (SortedMap_it->second).end(); InternalMap_it++)
        {
          int key2 = InternalMap_it->first;
          DDNode* element = InternalMap_it->second;
          cout<<key1<<"\t"<<key2<<"\t"<<element<<"\n";
        }
    }

  //count++;
}


DDNode* DDManager::ReduceModified(DDNode *v)
{
  //cout<<"\nReduce called\n";

  // count is the index of the last AP created

  //cout<<"\ncount = "<<count<<endl;


  DDNode *u;
  map<int, DDNode*> subgraph;
  subgraph.clear();
  map<int, DDNode*>::iterator subgraph_it;
  map<int, list<DDNode*> > vlist;

  vlist.clear();

  SetOfDDNodes.clear();

  //nullifyMarksModifiedWithDP(v);
  //Traverse(v, vlist);

  Traverse_WithDP(v, vlist);

  //showVlist(vlist);

  //map<pair<int, int>, DDNode*> Q;
  //map<pair<int, int>, DDNode*>::iterator Q_it;

  map< pair<int, int>, list<DDNode*> > Q;
  map< pair<int, int>, list<DDNode*> >::iterator Q_it;
  list<DDNode*> DDNodesWithSameKey;


  pair<int, int> old_key, key;

  int nextid = 0;



  for(int i=MAX_AP+1; i>=count; i--)
    {

      //cout<<"\ni="<<i<<endl;

      Q.clear();
      if(vlist.find(i) == vlist.end()) continue;


      list<DDNode*> CurrentList = vlist[i];

      for(list<DDNode*>::iterator it=CurrentList.begin(); it!=CurrentList.end(); it++)
        {
          u=*it;
          //cout<<"\nu="<<u<<endl;
          if(u->getIndex() == MAX_AP+1) // Terminal nodes
            {
              //Q.insert(make_pair(make_pair(u->getVal(), u->getVal()), u));

              insertIntoQ(Q, u->getVal(), u->getVal(), u);
            }
          else if(u->getLeft()->getID() == u->getRight()->getID())
            {
              u->setID(u->getLeft()->getID()); // u is a redundant node
              //RedundantNodes.insert(make_pair(u, u->getLeft()));
            }
          else
            {
              //map<pair<int, int>, DDNode*>::iterator Q_it2 = Q.find( make_pair(u->getLeft()->getID(), u->getRight()->getID()));

              //if(Q_it2 != Q.end())
              //  {
              //        cout<<"\nError! Another node with the same key and same index already exists\n";
              //        exit(1);
              //  }


              //Q.insert(make_pair(make_pair(u->getLeft()->getID(), u->getRight()->getID()), u));

              insertIntoQ(Q, u->getLeft()->getID(), u->getRight()->getID(), u);
            }
        }

      //cout<<"\nQ formed\n";

      //showQ(Q);

      // Q is ready here. Q is sorted by construction


      old_key=make_pair(-1, -1);

      for(Q_it = Q.begin(); Q_it != Q.end(); Q_it++) // Take elements one by one
        {

          DDNodesWithSameKey=Q_it->second;

          for(list<DDNode*>::iterator samekey_it = DDNodesWithSameKey.begin(); samekey_it != DDNodesWithSameKey.end(); samekey_it++)
            {

              u = (*samekey_it);
              //cout<<"\nu="<<u<<endl;


              //cout<<"\nElement with minium k obtained\n";

              key=Q_it->first;

              //cout<<"\nKey1="<<key.first<<", key2="<<key.second<<endl;



              if (key.first==old_key.first && key.second==old_key.second)
                {

                  //cout<<"\nMatching with existing vertex\n";
                  u->setID(nextid); //{matches existing vertex}
                  // existing vertex is subgraph[nextid]
                  //RedundantNodes.insert(make_pair(u, subgraph[nextid]));
                }

              else //{unique vertex}
                {
                  //cout<<"\nunique vertex\n";

                  nextid = nextid + 1;
                  u->setID(nextid);
                  subgraph.insert(make_pair(nextid,u));

                  if(u->getIndex() != MAX_AP+1)
                    {
                      //cout<<"\nnon-terminal vertex\n";

                      subgraph_it=subgraph.find((u->getLeft())->getID());
                      if(subgraph_it != subgraph.end())
                        {
                          // CHANGED on 4/1/10

                          // Delete u from u's present left's in_list
                          //DDNode* PresentLeft = u->getLeft();

                          //if(PresentLeft != NULL)
                          //  PresentLeft->Delete_single_entry_from_DD_inlist(u);

                          // set new left for u



                          /* Temporary code to see if subgraph is formed correctly */

                          if( (subgraph_it->second)->getID()<0)
                            {
                              cout<<"\nLeft is not processed already\n";
                              exit(1);
                            }

                          /* Temporary code to see if subgraph is formed correctly ends here*/




                          u->setLeft(subgraph_it->second);

                          // set the in_list for the new left
                          //(subgraph_it->second)->Append_DDnode_inlist(u);

                          // CHANGED on 4/1/10 ends here
                        }
                      else
                        {
                          cout<<"\nsubgraph does not have entry for id "<<u->getLeft()->getID()<<" inside function Reduce\n";
                          exit(1);
                        }

                      subgraph_it=subgraph.find((u->getRight())->getID());
                      if(subgraph_it != subgraph.end())
                        {
                          // CHANGED on 4/1/10

                          // Delete u from u's present right's in_list
                          //DDNode* PresentRight = u->getRight();

                          //if(PresentRight != NULL)
                          //  PresentRight->Delete_single_entry_from_DD_inlist(u);

                          // set new right for u


                          /* Temporary code to see if subgraph is formed correctly */

                          if( (subgraph_it->second)->getID()<0)
                            {
                              cout<<"\nLeft is not processed already\n";
                              exit(1);
                            }

                          /* Temporary code to see if subgraph is formed correctly ends here*/




                          u->setRight(subgraph_it->second);

                          // set the in_list for the new right
                          // (subgraph_it->second)->Append_DDnode_inlist(u);

                          // CHANGED on 4/1/10 ends here
                        }
                      else
                        {
                          cout<<"\nsubgraph does not have entry for id "<<u->getRight()->getID()<<" inside function Reduce\n";
                          exit(1);
                        }

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

  //cout<<"\nReduce finished\n";
  //return(subgraph[v->getID()]);

  subgraph_it=subgraph.find(v->getID());
  if(subgraph_it != subgraph.end())
    {
      return(subgraph_it->second);
    }
  else
    {
      cout<<"\nsubgraph does not have entry for id "<<v->getID()<<" inside function Reduce\n";
      exit(1);
    }

}



void DDManager::Traverse_WithDP(DDNode *v, map<int, list<DDNode*> > &vlist)
{
  //procedure Traverse(v:vertex);

  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      //cout<<"\nAlready visited\n";
    }
  if(it == SetOfDDNodes.end()) // dag with root v is not already visited. Go down
    {

      if(v->getIndex()<= MAX_AP) // {v nonterminal}
        {
          Traverse_WithDP(v->getLeft(), vlist);
          Traverse_WithDP(v->getRight(), vlist);
        }

      // Add v into list for v.index
      map<int, list<DDNode*> >::iterator vlist_it;
      vlist_it = vlist.find(v->getIndex());
      if(vlist_it != vlist.end()) vlist[v->getIndex()].push_back(v);
      else {list<DDNode*> new_list; new_list.push_back(v); vlist.insert(make_pair(v->getIndex(), new_list)); }

      SetOfDDNodes.insert(v);
    }
}


void DDManager::insertIntoQ(map< pair<int, int>, list<DDNode*> > &Q, int key1, int key2, DDNode* u)
{

  map< pair<int, int>, list<DDNode*> >::iterator Q_it;

  Q_it = Q.find(make_pair(key1, key2));

  if(Q_it != Q.end())
    {
      //cout<<"\nAnother node with the same key and same index already exists\n";

      (Q_it->second).push_back(u);
    }
  else
    {
      list<DDNode*> new_list;
      new_list.push_back(u);
      Q.insert(make_pair(make_pair(key1, key2), new_list));
    }
}

/*
DDNode*  DDManager::DagResolveModified(DDNode *dd, CDAGNode *equation, string VarToElim, CHelper &MANAGER)
{
  map< pair<DDNode*, CDAGNode*>, DDNode*>::iterator it = TableForDagResolve.find(make_pair(dd, equation));

  if(it != TableForDagResolve.end())
    {
      //cout<<"\nTable hit in DAGResolve\n";
      return it->second;
    }

  if(dd->getIndex() == MAX_AP+1) // Terminal
    {
      return dd;
    }

  DDNode *NewLeft = DagResolveModified(dd->getLeft(), equation, VarToElim, MANAGER);
  DDNode *NewRight = DagResolveModified(dd->getRight(), equation, VarToElim, MANAGER);

  CDAGNode *MyPredicate = dd->getPredicate();

  CDAGNode *NewPredicate;

  // Check if MyPredicate contains the variable to be eliminated

  if(MyPredicate->freeOfVarModified(VarToElim))
    {
      NewPredicate = MyPredicate;
    }

  else
    {

      // Temporary : Using Inlining to get the new equation
      CDAGNode *dag = MANAGER.createDAG("and", equation, MyPredicate);
      set<string> VarsToQuantify;
      VarsToQuantify.insert(VarToElim);
      cout<<"\nInlining called with dag = "<<dag->getDAGStr()<<", and VarToElim = "<<VarToElim<<endl;
      NewPredicate = MANAGER.Inlining(dag, VarsToQuantify);
      cout<<"\nNewPredicate="<<NewPredicate->getDAGStr()<<endl;


    }

  // Note that in DD nodes, left is else and right is then

  DDNode* then_dd = NewRight;
  DDNode* els_dd = NewLeft;

  DDNode* codn_dd = createDDFromDAG(NewPredicate, MANAGER);

  //DDNode* resultDD = createITE(codn_dd, then_dd, els_dd );
  DDNode* resultDD = createITEModified(codn_dd, then_dd, els_dd );

  //cout<<"\nresultDD = "<<resultDD->getDDString()<<endl;

  TableForDagResolve.insert(make_pair(make_pair(dd, equation), resultDD));

  return resultDD;
}

*/



DDNode*  DDManager::DagResolveModifiedAdvanced(DDNode *dd, CDAGNode *equation, string VarToElim, CHelper &MANAGER, map<string, int> WidthTable)
{
  map< pair<DDNode*, CDAGNode*>, DDNode*>::iterator it = TableForDagResolve.find(make_pair(dd, equation));

  if(it != TableForDagResolve.end())
    {
      //cout<<"\nTable hit in DAGResolve\n";
      return it->second;
    }

  if(dd->getIndex() == MAX_AP+1) // Terminal
    {
      return dd;
    }

  DDNode *NewLeft = DagResolveModifiedAdvanced(dd->getLeft(), equation, VarToElim, MANAGER, WidthTable);
  DDNode *NewRight = DagResolveModifiedAdvanced(dd->getRight(), equation, VarToElim, MANAGER, WidthTable);

  CDAGNode *MyPredicate = dd->getPredicate();

  CDAGNode *NewPredicate;

  // Check if MyPredicate contains the variable to be eliminated

  if(MyPredicate->freeOfVarModified(VarToElim))
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
      equation->getDAGNames(names);
      MyPredicate->getDAGNames(names);

      attachSetToList(names, VarNames);

      //showList(VarNames, "VarNames");

      NoVars=VarNames.size();
      // Now we have the list of variables,  NoVars, NoVarsToElim

      set<CDAGNode*> Constraints;
      Constraints.insert(equation);
      Constraints.insert(MyPredicate);


      //cout<<"\nShowing the constraints before calling the Eliminator\n";

      //for(set<CDAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
      //        {
      //          cout<<(*it)->getDAGStr()<<endl;
      //        }


      
      // Get the equations
      bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

      ULINT Mod=findPower(NoOfBits);

      // Now everything is ready. Just call Matrix qEliminaror

      // Checking if Constraints is False before calling the eliminator

      set<CDAGNode*> TemporaryDisEqSet;
      TemporaryDisEqSet.clear();
      bool theory_unsat = ThUnsat(Constraints, TemporaryDisEqSet,WidthTable, MANAGER);
      if(theory_unsat)
	{
	  //cout<<"\nSet of constraints is unsat\n";
	NewPredicate = MANAGER.createDAG("0");
	}
      else
	{
	  //cout<<"\nSet of constraints is sat\n";
	  //clock_t q_elim_start_time = clock();
	  NewPredicate = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, MANAGER, VarNames, WidthTable);

	  //cout<<"\n\nQUANTIFIER ELIMINATION TIME  = "<< ((double)clock() - q_elim_start_time)/CLOCKS_PER_SEC <<endl<<endl;
	  if(NewPredicate == NULL) NewPredicate = MANAGER.createDAG("1");
	}

      //cout<<"\nResult of QEliminator in DAG = "<<NewPredicate->getDAGStr()<<endl;

    }

  // Note that in DD nodes, left is else and right is then

  DDNode* then_dd = NewRight;
  DDNode* els_dd = NewLeft;

  DDNode* codn_dd = createDDFromDAG(NewPredicate, MANAGER, WidthTable);

  //DDNode* resultDD = createITE(codn_dd, then_dd, els_dd );
  DDNode* resultDD = createITEModified(codn_dd, then_dd, els_dd );

  //cout<<"\nresultDD = "<<resultDD->getDDString()<<endl;

  TableForDagResolve.insert(make_pair(make_pair(dd, equation), resultDD));

  return resultDD;
}


bool DDManager::coeffIsOdd(CDAGNode *equation, string VarToElim)
{
  ULINT coefficient=1;
  equation->getCoefficient(VarToElim, coefficient); // Now we have the coefficient of VarToElim in equation
  if(coefficient % 2 != 0) return true; // coefficient is odd
  return false;
}




// NEWLY ADDED ON 25/1/2010

DDNode* DDManager::QElimWithDPModified(string VarToElim, DDNode* f, set<CDAGNode*> E, set<CDAGNode*> D, CHelper &MANAGER, map<string, int> WidthTable, HashTable <string, DDNode*> *PtrTableForQElimModified, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  DDNode* Existing  = getEntryFromTableForQElimModified(VarToElim, f, E, D, PtrTableForQElimModified);

  if(Existing != NULL)
    {
      //cout<<"\nQElim's Table hit\n";
      //cout<<"\nResult returned = "<<createDAGFromDD(Existing, MANAGER)->getDAGStr()<<endl;
      return Existing;
    }

  //cout<<"\nQElim's Table miss\n";
  if(f->getIndex()==MAX_AP+1) // Terminal node
    {
      DDNode* result;

      int value = f->getVal();

      //cout<<"\nTerminal node reached with value = "<<value<<endl;
      if(value == 0)
        {
          //cout<<"\nZero returned\n";
          result = f;
        }
      else
        {
          //cout<<"\nTerminal node reached with value = "<<value<<endl;

          if(E.empty() && D.empty())
            {
              //cout<<"\nE, D are empty\n";
              result = createDD(1);
            }
          else if(ThUnsat(E, D, WidthTable, MANAGER))
            {
              result = createDD(0);
            }
          else
            {
              //result = APResolve(E, D, VarToElim, WidthTable, MANAGER);
	      result = APResolveModified(E, D, VarToElim, WidthTable, MANAGER, PtrTableForAPResolveModified);
            }
        }

      //Insert into QElim's DP Table
      insertEntryIntoTableForQElimModified(VarToElim, f, E, D, result, PtrTableForQElimModified);


      //cout<<"\nResult returned = "<<createDAGFromDD(result, MANAGER)->getDAGStr()<<endl;

      return result;
    }
  else
    {

      //cout<<"\nChecking if E or D is empty\n";

      //if (!E.empty() || !D.empty()) 
      if(DAGResolveApplicable(E, D, WidthTable))
        {
          //cout<<"\n!E.empty() or !D.empty()"<<endl;
          CDAGNode* e1 = selectEquationWithLeastCoefficientForVarToElimAdvanced(E, D, VarToElim, WidthTable, MANAGER);
          if(e1 != NULL)
            {
              //cout<<"\nEquation Selected = "<<e1->getDAGStr()<<endl;
              //cout<<"\nDAGResolve() called"<<endl;
              //cout<<"\nf="<<f->getDDString()<<endl;

              // Following line commented and new line added on 18/1/2010

              f = DagResolveModifiedAdvanced(f, e1, VarToElim, MANAGER, WidthTable);

              //f is simplified here. Check if there exists an entry with this simplfied version of f
              DDNode* Existing  = getEntryFromTableForQElimModified(VarToElim, f, E, D, PtrTableForQElimModified);

              if(Existing != NULL)
                {
                  //cout<<"\nQElim's Table hit\n";
                  //cout<<"\nResult returned = "<<createDAGFromDD(Existing, MANAGER)->getDAGStr()<<endl;
                  return Existing;
                }


              //cout<<"\nf after DAGResolve ="<<f->getDDString()<<endl;
              //cout<<"\nDAGResolve() finished"<<endl;
              if(DDFreeOfVar(f, VarToElim))
                {

                  //cout<<"\nf is free of "<<VarToElim<<endl;
                  //cout<<"\nAPResolve() called"<<endl;
                  DDNode *temp;
                  if(ThUnsat(E, D, WidthTable, MANAGER)) temp = createDD(0);
                  else 
		    {
		      //temp = APResolve(E, D, VarToElim, WidthTable, MANAGER);
		      temp = APResolveModified(E, D, VarToElim, WidthTable, MANAGER, PtrTableForAPResolveModified);
		    }

                  //cout<<"\nAPResolve() finished"<<endl;

                  //cout<<"\nAPResolve returns "<<temp->getDDString()<<endl;

                  //temp = apply("and", f, temp);
		  temp = applyModified("and", f, temp);

                  //cout<<"\nDAGResolve & APResolve ="<<temp->getDDString()<<endl;

                  //cout<<"\nDAGResolve & APResolve returned"<<endl;

                  //cout<<"\nf="<<f<<endl;
                  insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);


                  //cout<<"\nResult returned = "<<createDAGFromDD(temp, MANAGER)->getDAGStr()<<endl;
                  return temp;
                }
            }
        }

      //cout<<"\nGetting predicate C\n";
      CDAGNode* C = f->getPredicate();
      //cout<<"\npredicate obtained is "<<C->getDAGStr()<<endl;
      if(C->freeOfVarModified(VarToElim))
        {
          DDNode *temp;

          //cout<<"\npredicate is free of "<<VarToElim<<endl;

          DDNode *thenb = QElimWithDPModified(VarToElim, f->getRight(), E, D, MANAGER, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
          DDNode *elseb = QElimWithDPModified(VarToElim, f->getLeft(), E, D, MANAGER, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);

          DDNode* C_dd = createDD(C, WidthTable, MANAGER);

          //temp = createITE(C_dd, thenb, elseb);
	  temp = createITEModified(C_dd, thenb, elseb);

          insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

          //cout<<"\nResult returned = "<<createDAGFromDD(temp, MANAGER)->getDAGStr()<<endl;
          return temp;
        }
      else
        {
          DDNode *temp;

          //cout<<"\npredicate is not free of "<<VarToElim<<endl;

          set<CDAGNode*> E_new, D_new;
          copySet(E_new, E);
          E_new.insert(C);
          copySet(D_new, D);
          D_new.insert(negateConstraint(C, MANAGER));
          DDNode *thenb = QElimWithDPModified(VarToElim, f->getRight(), E_new, D, MANAGER, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
          DDNode *elseb = QElimWithDPModified(VarToElim, f->getLeft(), E, D_new, MANAGER, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);

          //temp = apply("or", thenb, elseb);
	  temp = applyModified("or", thenb, elseb);


          insertEntryIntoTableForQElimModified(VarToElim, f, E, D, temp, PtrTableForQElimModified);

          //cout<<"\nResult returned = "<<createDAGFromDD(temp, MANAGER)->getDAGStr()<<endl;
          return temp;
        }
    }
}



DDNode* DDManager::getEntryFromTableForQElimModified(string VarToElim, DDNode* f, set<CDAGNode*> E, set<CDAGNode*> D, HashTable <string, DDNode*> *PtrTableForQElimModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<CDAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<CDAGNode*>::iterator it = D.begin(); it != D.end(); it++)
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

void DDManager::insertEntryIntoTableForQElimModified(string VarToElim, DDNode*f, set<CDAGNode*> E, set<CDAGNode*> D, DDNode* value, HashTable <string, DDNode*> *PtrTableForQElimModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  sprintf(temp_char , "%x", f);
  temp_key = temp_char;
  key += temp_key;

  for(set<CDAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<CDAGNode*>::iterator it = D.begin(); it != D.end(); it++)
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

  //result = TableForQElimModified.hashtable_insert(key, value);
  result = PtrTableForQElimModified->hashtable_insert(key, value);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function DDManager::insertEntryIntoTableForQElimModified\n";
      exit(1);
    }
}


// NEWLY ADDED ON 25/1/2010  ENDS HERE

// NEWLY ADDED ON 26/1/2010

DDNode* DDManager::APResolveModified(set<CDAGNode*> E, set<CDAGNode*> D, string VarToElim, map<string, int> WidthTable, CHelper &MANAGER, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{

  DDNode* Existing  = getEntryFromTableForAPResolveModified(VarToElim, E, D, PtrTableForAPResolveModified);

  if(Existing != NULL)
    {
      //cout<<"\nAPResolve's Table hit\n";
      //cout<<"\nResult returned = "<<createDAGFromDD(Existing, MANAGER)->getDAGStr()<<endl;
      return Existing;
    }

  //cout<<"\nAPResolveModified was not earlier encountered with the same set of E and D\n";

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
  for(set<CDAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    (*it)->getDAGNames(names);

  for(set<CDAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    (*it)->getDAGNames(names);

  attachSetToList(names, VarNames);

  //showList(VarNames, "VarNames");

  NoVars=VarNames.size();
  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<CDAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));


  //cout<<"\nShowing the constraints before calling the Eliminator\n";

  //for(set<CDAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
  //    {
  //      cout<<(*it)->getDAGStr()<<endl;
  //    }


  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  CDAGNode *resultDAG;


  //clock_t q_elim_start_time = clock();

  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, MANAGER, VarNames, WidthTable);

  //cout<<"\n\nQUANTIFIER ELIMINATION TIME  = "<< ((double)clock() - q_elim_start_time)/CLOCKS_PER_SEC <<endl<<endl;



  if(resultDAG == NULL) resultDAG = MANAGER.createDAG("1");

  //cout<<"\nResult of APResolveModified in DAG = "<<resultDAG->getDAGStr()<<endl;

  DDNode* resultDD;

  //cout<<"\nCreating DD from APs\n";

  resultDD = createDDFromDAG(resultDAG, MANAGER, WidthTable);

  //cout<<"\nDD created from APs\n";

  insertEntryIntoTableForAPResolveModified(VarToElim, E, D, resultDD, PtrTableForAPResolveModified);

  //cout<<"\nEntry inserted into TableForAPResolveModified\n";

  //cout<<"\nAPResolveModified() is finished\n";

  return resultDD;

}


DDNode* DDManager::getEntryFromTableForAPResolveModified(string VarToElim, set<CDAGNode*> E, set<CDAGNode*> D, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];

  for(set<CDAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<CDAGNode*>::iterator it = D.begin(); it != D.end(); it++)
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

void DDManager::insertEntryIntoTableForAPResolveModified(string VarToElim, set<CDAGNode*> E, set<CDAGNode*> D, DDNode* value, HashTable <string, DDNode*> *PtrTableForAPResolveModified)
{
  // Creating the hash key

  string key = VarToElim;
  string temp_key;
  char temp_char[10];


  for(set<CDAGNode*>::iterator it = E.begin(); it != E.end(); it++)
    {
      sprintf(temp_char , "%x", *it);
      temp_key = temp_char;
      key += temp_key;
    }

  for(set<CDAGNode*>::iterator it = D.begin(); it != D.end(); it++)
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



// NEWLY ADDED ON 26/1/2010  ENDS HERE

DDNode* DDManager::ReduceModifiedAdvanced(DDNode *v)
{
  
  DDNode *u;
  DDNode* subgraph[MAX_NODES];
  list<DDNode*> vlist[MAX_AP+2];

  SetOfDDNodes.clear();
  Traverse_WithDPAdvanced(v, vlist);

  map< pair<int, int>, list<DDNode*> > Q;
  map< pair<int, int>, list<DDNode*> >::iterator Q_it;
  list<DDNode*> DDNodesWithSameKey;
  pair<int, int> old_key, key;
  int nextid = 0;


  for(int i=MAX_AP+1; i>=count; i--)
    {

      //cout<<"\ni="<<i<<endl;

      Q.clear();
      
      list<DDNode*> CurrentList = vlist[i];

      if(CurrentList.empty()) continue;

      for(list<DDNode*>::iterator it=CurrentList.begin(); it!=CurrentList.end(); it++)
        {
          u=*it;
          if(u->getIndex() == MAX_AP+1) // Terminal nodes
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

      //cout<<"\nQ formed\n";
      // Q is ready here. Q is sorted by construction


      old_key=make_pair(-1, -1);

      for(Q_it = Q.begin(); Q_it != Q.end(); Q_it++) // Take elements one by one
        {

          DDNodesWithSameKey=Q_it->second;

          for(list<DDNode*>::iterator samekey_it = DDNodesWithSameKey.begin(); samekey_it != DDNodesWithSameKey.end(); samekey_it++)
            {

              u = (*samekey_it);
	      //cout<<"\nElement with minium k obtained\n";

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

                  if(u->getIndex() != MAX_AP+1)
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



void DDManager::Traverse_WithDPAdvanced(DDNode *v, list<DDNode*> vlist[])
{
  //procedure Traverse(v:vertex);

  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      //cout<<"\nAlready visited\n";
      return;
    }
 
  if(v->getIndex()<= MAX_AP) // {v nonterminal}
    {
      Traverse_WithDPAdvanced(v->getLeft(), vlist);
      Traverse_WithDPAdvanced(v->getRight(), vlist);
    }

  
  vlist[v->getIndex()].push_back(v);

  SetOfDDNodes.insert(v);
  
}


void DDManager::showVlistAdvanced(list<DDNode*> vlist[])
{
  for(int i=MAX_AP+1; i>=0; i--)
    {
      list<DDNode*> List1 = vlist[i];
      if(List1.empty()) cout<<"\nvlist["<<i<<"] is empty\n";
      for(list<DDNode*>::iterator it=List1.begin(); it!=List1.end(); it++)
        cout<<(*it)<<",";
    }
}


bool DDManager::DAGResolveApplicable(set<CDAGNode*> E, set<CDAGNode*> D, map<string, int> WidthTable)
{
  if(!E.empty())
    return true;

  for(set<CDAGNode*>::iterator it=D.begin(); it != D.end(); it++)
    {
      CDAGNode* root = (*it);

      //cout<<"\nroot = "<<root->getDAGStr()<<endl;

      int NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);


      //cout<<"\nNo: of Bits in constraint = "<<NoOfBitsInConstraint<<endl;

      if(NoOfBitsInConstraint == 1)
        return true;
    }

  return false;
}




DDNode* DDManager::applyModified(string op, DDNode* v1, DDNode* v2)
{
  
  DDNode* result = applyStepModified(op, v1, v2);
  DDNode* reducedResult = ReduceModifiedAdvanced(result);
  return reducedResult;
}
DDNode* DDManager::applyStepModified(string op, DDNode* v1, DDNode* v2)
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

  HTStatusValue<DDNode*> result = TableForApplyModified.hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }

  //cout<<"\nTableForApply MISS\n";

  int u_value, u_index;
  CDAGNode *u_AP;

  u_value = evaluateOp(op, v1, v2); // should be evaluateOp
  if(u_value != -1)
    {
      //{create terminal vertex}

      //cout<<"\napplyStepModified finished\n";

      u = createDD(u_value);
      result = TableForApplyModified.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyStepModified\n";
	    exit(1);
	  }

      
      return u;


      //return createDD(u_value);
    }

  // NEWLY ADDED ON 2nd Jan 10
  // else if(partiallyEvaluatable(op, v1, v2))
  //  {
  //   u = partialEvaluation(op, v1, v2);
  
  //    TableForApply.insert(make_pair(make_pair(v1,v2), u));
  
        //cout<<"\napplyStepModified finished\n";
  
  //     return u;
  //   }
  //NEWLY ADDED ON 2nd Jan 10 ends here

  else //{create nonterminal and evaluate further down}
    {

      int v1_index, v2_index;
      v1_index = v1->getIndex();
      v2_index = v2->getIndex();
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
      u_low = applyStepModified(op, v_low1, v_low2);
      u_high = applyStepModified(op, v_high1, v_high2);

      u = createDD(u_AP, u_low, u_high);


      result = TableForApplyModified.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyStepModified\n";
	    exit(1);
	  }

      //{add vertex to table}

      //cout<<"\napplyStepModified finished\n";

      return u;
    }
}



// Functions for direct ITE creation ADDED ON 2/1/2010

DDNode* DDManager::createITEModified(DDNode* condn, DDNode* then, DDNode* els)
{

  DDNode* result = applyITEModified(condn, then, els);
  DDNode* reducedResult = ReduceModifiedAdvanced(result);
  return reducedResult;
}

DDNode* DDManager::applyITEModified(DDNode* condn, DDNode* then, DDNode* els)
{
  DDNode* u;

  // Creating the hash key

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

  HTStatusValue<DDNode*> result = TableForITEModified.hashtable_search(key);

  if(result.success())
    {
      return result.GetValue();
    }

  
  //cout<<"\nTableForITE MISS\n";

  u = evaluateITEAdvanced(condn, then, els);

  if(u!=NULL)
    {
      //cout<<"\napplyITEModified finished\n";
      return u;
    }

  int u_value, u_index;
  CDAGNode *u_AP;


  //u_value = evaluateITE(condn, then, els);
  //if(u_value != -1)
  //  {
  //{create terminal vertex}

  //   return createDD(u_value);
  //  }

  // NEWLY ADDED ON 2nd Jan 10
  /* else if(partiallyEvaluatableIte(condn, then, els))
     {
     u = partialEvaluationIte(condn, then, els);

     //TableForApply.insert(make_pair(make_pair(v1,v2), u));

     return u;
     }*/
  // NEWLY ADDED ON 2nd Jan 10 ends here

  // else //{create nonterminal and evaluate further down}
  //  {

  int condn_index, then_index, else_index;
  condn_index = condn->getIndex();
  then_index = then->getIndex();
  else_index = els->getIndex();

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



  u_low = applyITEModified(condn_next_low, then_next_low, else_next_low);
  u_high = applyITEModified(condn_next_high, then_next_high, else_next_high);

  u = createDD(u_AP, u_low, u_high);

  result = TableForITEModified.hashtable_insert(key, u);
  if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyITEModified\n";
	    exit(1);
	  }

  //TableForITE.insert(make_pair(make_pair(make_pair(condn,then), els), u));
  //{add vertex to table}

  //cout<<"\napplyITEModified finished\n";
  return u;
  //   }
}



CDAGNode* DDManager::recreateEquationInProperForm(CDAGNode* equation, map<string, int> WidthTable, CHelper &MANAGER)
{
  // Get the list of variables
  list<string> VarNames;
  set<string> names;
  
  equation->getDAGNames(names);
  attachSetToList(names, VarNames);
  VarNames.sort();

  CDAGNode *lhs, *rhs;
  list<CDAGNode*>::iterator out_list_it = equation->Get_DAGnode_outlist();
  lhs = (*out_list_it);
  out_list_it++;
  rhs = (*out_list_it);

  if(lhs==rhs) return equation;
  if(names.empty()) return equation;

 
  int NoOfBitsInConstraint=getNoOfBitsInConstraint(equation, WidthTable);
  //cout<<"\nNo: of Bits in constraint obtained\n";

 //  CDAGNode *lhs, *rhs;
//   list<CDAGNode*>::iterator out_list_it = equation->Get_DAGnode_outlist();
//   lhs = (*out_list_it);
//   out_list_it++;
//   rhs = (*out_list_it);

  Matrix A, C; // Row matrices
  Matrix B, D; // Row matrices

  getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
  getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D


  //cout<<"\nCoefs and constants obtained\n";


  // We have AX+B = CX+D
  // Convert A to A-C and D to D-B to get (A-C)X = (D-B)

  ULINT LocalMod=findPower(NoOfBitsInConstraint);
 
  matSub(A, C, LocalMod);
  matSub(D, B, LocalMod);

  // Now equation is AX = D

  // Converting the first element in coef mat to normal form

  ULINT FirstElement = A[0][0];

  ULINT OddPart = getOddPart(FirstElement);

    if(OddPart != 0)
    {


      ULINT MIOfOddPart = mInverse(OddPart, LocalMod);

      scalarMatMultiply(A, MIOfOddPart, LocalMod);

      scalarMatMultiply(D, MIOfOddPart, LocalMod);
    }


  // Now equation is AX = D with first element in normal form
  

  CDAGNode* RecreatedEquation = getDAGFromEqs(A, D, MANAGER, VarNames);

  return RecreatedEquation;

}


void DDManager::printAPMap()
{
  cout<<"\nAP MAP\n";

  cout<<"\nSize of APMAP = "<<APMap.size()<<"\n";

  map<CDAGNode*, int>::iterator APMap_it;

  for(APMap_it = APMap.begin(); APMap_it != APMap.end();  APMap_it++)
    {
      cout<<"\n"<<(APMap_it->first)->getDAGStr()<<"\t"<<APMap_it->second;
    }
}



/* Function for elimination of inconsistent paths in the DD pointed to by dd : This function is a simplified variant of eliminateInconsistentPaths in SMTLib_to_DAGConverter's DDManager.cpp */

DDNode* DDManager::eliminateInconsistentPaths(DDNode* dd, map<string, int> &WidthTable, CHelper &MANAGER)
{
  DDNode* TrueNode = createDD(1);

  DDNode* result;


  set<CDAGNode*> E, D;
  E.clear(); D.clear();


  result = eliminateInconsistentPathsInternal(dd, TrueNode, E, D, WidthTable, MANAGER); // This basically performs applyWithImplicationCheck("and", true, dd) but without the context-insensitive (evaluation based) simplification

  DDNode* reducedResult = ReduceModifiedAdvanced(result);
  return reducedResult;
}


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


// check if E/\D/\u_AP is unsat
bool DDManager::implicationCheck(set<CDAGNode*> E, set<CDAGNode*> D, CDAGNode *u_AP, map<string, int> &WidthTable, CHelper &MANAGER)
{
  CDAGNode *root = NULL;
  set<CDAGNode*>::iterator it=E.begin();
  for(; it!=E.end(); it++)
    if(root==NULL) root=*it; else root=MANAGER.createDAG("and", root, *it);

  it=D.begin();
  for(; it!=D.end(); it++)
    if(root==NULL) root=*it; else root=MANAGER.createDAG("and", root, *it);

   if(root==NULL)
   {
     root = u_AP;
   }
 else
   {
     root = MANAGER.createDAG("and", root, u_AP);
   }



  FILE *smt_file = fopen("thunsat.ys", "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create thunsat.ys\n"; exit(1);}

  //cout<<"\nSMT file opened\n";

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  //cout<<"\nDeclarations written\n";




  /* FOLLOWING LINES WAS COMMENTED AND NEW ONE ADDED ON 18/1/2010 */

  //string dag_str = root->getDAGOfProperSkeltonFormatInYicesFormat("bool");

  string dag_str = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(root, WidthTable);

  /* ADDED ON 18/1/2010 ENDS HERE */




  //cout<<"\nDag written\n";

  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  system("yices -tc thunsat.ys > result_thunsat.txt");

  //cout<<"\nYices called\n";

  FILE *result_smt_file = fopen("result_thunsat.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot open file result_thunsat.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  //cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::implicationCheck\n"; exit(1);
    }
}


DDNode* DDManager::eliminateInconsistentPathsInternal(DDNode* v1, DDNode* v2, set<CDAGNode*> E, set<CDAGNode*> D, map<string, int> &WidthTable, CHelper &MANAGER) // Note that v2 is always true
{

  DDNode* u; // result DD

  string key_level_1;
  CDAGNode *Negated_u_AP;

  // Hash Table MISS

  int u_value, u_index;
  CDAGNode *u_AP;

  u_value = evaluateOpOnTerminals("and", v1, v2); // if both operands are constants, evaluate and return
  if(u_value != -1)
    {
      //{create terminal vertex}
      u = createDD(u_value);
    }

  else // create nonterminal and evaluate further down 
    {

      int v1_index, v2_index;
      v1_index = v1->getIndex();
      v2_index = v2->getIndex();
      DDNode *v1_low, *v1_high, *v2_low, *v2_high, *v_low1, *v_high1, *v_low2, *v_high2, *u_low, *u_high;
      v1_low = v1->getLeft();
      v1_high = v1->getRight();
      v2_low = v2->getLeft();
      v2_high = v2->getRight();

      u_index = findMin(v1_index, v2_index);
      u_AP = findAPFromIndex(u_index);
      Negated_u_AP = negateConstraint(u_AP, MANAGER); 

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

      if(implicationCheck(E, D, Negated_u_AP, WidthTable, MANAGER)) // E/\D=>u_AP . Hence only the right matters
	{
	  u = eliminateInconsistentPathsInternal(v_high1, v_high2, E, D, WidthTable, MANAGER);
	}
      else if(implicationCheck(E, D, u_AP, WidthTable, MANAGER)) // E/\D=>not(u_AP) . Hence only the left matters
	{
	  u = eliminateInconsistentPathsInternal(v_low1, v_low2, E, D, WidthTable, MANAGER);
	}
       else // try to evaluate
	{
	set<CDAGNode*> E_new, D_new;
        copySet(E_new, E);
        E_new.insert(u_AP);
        copySet(D_new, D);
	D_new.insert(Negated_u_AP);

      	u_high = eliminateInconsistentPathsInternal(v_high1, v_high2, E_new, D, WidthTable, MANAGER);

	u_low = eliminateInconsistentPathsInternal(v_low1, v_low2, E, D_new, WidthTable, MANAGER);
	
	u = createDD(u_AP, u_low, u_high);
	}
    }// create non-terminal and evaluate further down


  return u;
}


/* Added on 11-01-2011 */

CDAGNode* DDManager::Qualmode_Blasting(CDAGNode *root, CHelper &MANAGER, map<string, int> &WidthTable, int frame)
{

  time_t qelim_start, qelim_end, qelim_duration;
  time(&qelim_start);

  map<CDAGNode*, CDAGNode*> LinearizationTable;
  map<CDAGNode*, CDAGNode*> SubtractionTable;
  int AutoNameCount = 0;


  // Separating the root into exists part and dag part

  if(root->Get_DAGnode_value() != "exists")
    {
      cout<<"\nNo variables to eliminate. Return\n";
      return root;
    }

  set<string> VarsToElimOriginal;
  set<string> VarsToElimRenamed;
  root->getExistsDAGNames(VarsToElimOriginal);
  list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
  iout++;
  CDAGNode *DagPart = *iout;



  // Linearizing the dag part


  cout<<"\nVarsToElimOriginal\n";
  set<string>::iterator vit =  VarsToElimOriginal.begin();
  for(;vit !=  VarsToElimOriginal.end(); vit++)
    {
      cout<<*vit<<endl;
    }

  //cout<<"\nDag part = "<<DagPart->getDAGStr()<<"\n";

  DagPart = MANAGER.getPreprocessedClone(DagPart);

  //cout<<"\nDag part after replacement of predicates = "<<DagPart->getDAGStr()<<"\n";


  cout<<"\nWidth table\n";
  map<string, int>::iterator wit =  WidthTable.begin();
  for(;wit !=  WidthTable.end(); wit++)
    {
      cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
    }


  CDAGNode* LinearDag;

  bool linearization_on=false;

  if(linearization_on)
    {

      cout<<"\nGetting the linearized dag\n";

      LinearDag = MANAGER.getLinearizedClone(DagPart, LinearizationTable, AutoNameCount, WidthTable, VarsToElimOriginal, VarsToElimRenamed);

      //cout<<"\nLinearDag = "<<LinearDag->getDAGStr()<<endl;

      cout<<"\nLinearization table\n";
      map<CDAGNode*, CDAGNode*>::iterator lit =  LinearizationTable.begin();
      for(;lit !=  LinearizationTable.end(); lit++)
	{
	  cout<<"\n"<<(lit->first)->getDAGStr()<<"\t"<<(lit->second)->getDAGStr()<<endl;
	}


      cout<<"\nWidth table\n";
      wit =  WidthTable.begin();
      for(;wit !=  WidthTable.end(); wit++)
	{
	  cout<<"\n"<<wit->first<<"\t"<<wit->second<<endl;
	}
    }
  else
    {
      LinearDag = DagPart;
      
      for(set<string>::iterator it=VarsToElimOriginal.begin();it!=VarsToElimOriginal.end();it++)
	{
	  VarsToElimRenamed.insert(*it);
	}
    }



  SubtractionTable.clear();
  LinearDag = MANAGER.getSubtractionReplacedClone(LinearDag, SubtractionTable, WidthTable);
  //cout<<"\nAfter replacing subtractions by additions, LinearDag = "<<LinearDag->getDAGStr()<<endl;


  //if(true) {exit(1);}


  cout<<"\nVarsToElimRenamed\n";
  vit =  VarsToElimRenamed.begin();
  for(;vit !=  VarsToElimRenamed.end(); vit++)
    {
      cout<<*vit<<endl;
    }


  // Creating VarsToElim

  list<string> VarsToElim;

  /*

  vit =  VarsToElimOriginal.begin();
  for(;vit !=  VarsToElimOriginal.end(); vit++)
  {
  VarsToElim.push_back(*vit);
  }


  vit =  VarsToElimRenamed.begin();
  for(;vit !=  VarsToElimRenamed.end(); vit++)
  {
  VarsToElim.push_back(*vit);
  } */


  set<string> names;
  LinearDag->getDAGNames(names);
  for(set<string>::iterator it=names.begin(); it != names.end(); it++)
    {

      string name = *it;
      //cout<<"\nname = "<<name<<endl;
      if( VarsToElimOriginal.find(name) != VarsToElimOriginal.end() || VarsToElimRenamed.find(name) != VarsToElimRenamed.end())
        VarsToElim.push_back(name);
    }



  cout<<"\nVarsToElim\n";

  for(list<string>::iterator vars_it = VarsToElim.begin();vars_it !=  VarsToElim.end(); vars_it++)
    {
      cout<<*vars_it<<endl;
    }


  MANAGER.checkForPropSkelton(LinearDag);

  cout<<"\n\nDag's structure confirmed to be propositional\n\n";


  bool input_needed = true;

  if(input_needed) 
    {
      MANAGER.printQEliminatorResultsInGivenFile(LinearDag, WidthTable, VarsToElim, "all_input.smt");
    }

 
  char frame_char[100];
  sprintf(frame_char,"%d", frame);
  string frame_str(frame_char);

  string command;
  command = "all -dd -black_blasting -frame=";
  command += frame_str;
  command += " -bmc_file=yices_input.ys";
  command += " -bmc_cache_file=blast_table.txt";
  command += " all_input.smt";

  cout<<endl<<"Executing "<<command<<endl;
  
  system(command.c_str());

  CDAGNode* dag_eliminated_orig = MANAGER.createDAG("abstraction", 0);


  time(&qelim_end);
  qelim_duration = qelim_end - qelim_start;
  qe_time = qe_time + qelim_duration;

  return dag_eliminated_orig;

}

/* Added on 11-01-2011 ends here*/



/* Added on 03-03-2011 starts here*/

// Function to create a DD from a DAG with hashing : Presently we are experiecing
// problem in putting the definition of GlobalTableForCreateDDFromDAG
// Hence no hashing now
DDNode* DDManager::createDDFromDAGWithHashing(CDAGNode *dag, CHelper &MANAGER, map<string, int> WidthTable)
{
  // First check if the dd for this dag was made earlier
//   map<CDAGNode*, DDNode*>::iterator it = GlobalTableForCreateDDFromDAG.find(dag);

//   if(it != GlobalTableForCreateDDFromDAG.end()) // Exists in HT
//     {
//       return it->second;
//     }
//   else
//     {
      // call createDDFromDAG
      DDNode *MyDD = createDDFromDAG(dag, MANAGER, WidthTable);
      //      GlobalTableForCreateDDFromDAG.insert(make_pair(dag, MyDD));
      return MyDD;
      //    }
}


/* Added on 03-03-2011 ends here*/
