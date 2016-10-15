#include "NewDDManagerCleaned.h"

DDManager :: DDManager()
{
  count=-1;
}


// returns true if an entry exists in the APMap with index = u_index. Else returns false
bool DDManager::checkIfIndexExists(int u_index)
{

  map<CDAGNode*, int>::iterator APMap_it = APMap.begin();
  for(; APMap_it != APMap.end(); APMap_it++)
    {
      if(APMap_it->second == u_index) return true;
    }
  return false;
}


// returns the index of this node
int DDManager :: getIndex(DDNode *dd)
{
CDAGNode *pred = dd->getPredicate();
if(pred == NULL) // terminal node
	{
	return MAX_AP+1;
	}
else
	{
	if(APMap.find(pred)==APMap.end()) // no entry in APMap for the CDAGNode* pred : ERROR
		{
		cout<<"\nNo entry for APMap for the predicate node "<<pred<<"in function DDManager::getIndex()\n";
		exit(1);
		}
	return APMap[pred];
	}
}

// set the index of the given predicate node pred as new_index
void DDManager::setIndex(CDAGNode *pred, int new_index)
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

  bool old_implementation = false;

  CDAGNode *RecreatedEquation;
	
  if(old_implementation)
	{
	RecreatedEquation = getDAGFromEqs(A, D, MANAGER, VarNames);
	}
  else
	{
	RecreatedEquation = recreateDAGFromEqs(A, D, MANAGER, VarNames);
	}
 
  return RecreatedEquation;

}



CDAGNode* DDManager::recreateDAGFromEqs(Matrix EqCoefMat, Matrix EqConstMat, CHelper &MANAGER, list<string> VarNames)
	{
	list<string>::iterator it;

        CDAGNode* result=NULL;
	
	int rows = EqCoefMat.size();
	int cols = EqCoefMat[0].size();
	for(int i=0; i<rows; i++)
		{ 
	        CDAGNode *lhs=NULL, *eqn, *rhs;
		it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
                        if(EqCoefMat[i][j]==0) continue; // no need to add terms with zero coefficients

			string id_delta = *it;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
                        string delta = id_delta.substr(index+1);
                        int delta_int = atoi(delta.c_str());

			CDAGNode *variable = MANAGER.createDAG(id, delta_int);
			
			char coeff[20];
			sprintf(coeff, "%llu", EqCoefMat[i][j]);
			string coeff_str(coeff);

			CDAGNode *coefficient = MANAGER.createDAG(coeff_str);
			CDAGNode *term = MANAGER.createDAG("*", coefficient, variable);
			if(lhs==NULL) lhs=term; else lhs=MANAGER.createDAG("+", lhs, term);
			}

	       if(lhs==NULL)
			{
			cout<<"\nlhs becomes NULL in function DDManager::recreateDAGFromEqs\n";
			exit(1);
			}

		char constant[10];
		sprintf(constant, "%llu", EqConstMat[i][0]);
		string constant_str(constant);
		rhs = MANAGER.createDAG(constant_str);
		eqn = MANAGER.createDAG("equivalent", lhs, rhs);
		if(result==NULL) result=eqn; else result = MANAGER.createDAG("and", result, eqn);
		}
	return result;
	
	}
  



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

  key = pred_addr + opd_addr1 + opd_addr2; // check if new_predicate(1, 0) is there
  // Hash key created

  HTStatusValue<DDNode*> result = DD_HT.hashtable_search(key);

  if(result.success()) // Exists in HT
    {
      return result.GetValue();
    }

  new_predicate = recreateEquationInProperForm(new_predicate, WidthTable, MANAGER); // convert to cannonical form
 
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

	//cout<<"\nLME does not exist already. we need to insert one\n";  

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
	    
	
	APMap.insert((pair<CDAGNode*,int>(new_predicate,count)));
     }

  //cout<<"\nLME exists already\n"; 
	  
  DDNode* addr_DDnode = new DDNode(new_predicate, left, right); // create the dd node

  result = DD_HT.hashtable_insert(key, addr_DDnode);
   
   if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function DDManager :: createDD(DAGNode *new_predicate)\n";
	  exit(1);
	}

  return addr_DDnode;
}

// Function to create DDNode (new_predicate, right, left). Assumption : new_predicate is already there in the APMap. Hence it is already in cannonical form

DDNode* DDManager :: createDD(CDAGNode *new_predicate, DDNode* left, DDNode *right)
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
  
	  cout<<"\ncreateDD(DAGNode *new_predicate, DDNode* left, DDNode *right ) called to create DDs with new equations. If you want to create DDs with new equations, use createDD(DAGNode *new_predicate, map<string, int> WidthTable)\n";
	  exit(1);

        }

       DDNode* addr_DDnode = new DDNode(new_predicate, left, right);

       result = DD_HT.hashtable_insert(key, addr_DDnode);
   
       if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function DDManager :: createDD(DAGNode *new_predicate, DDNode* left, DDNode *right )\n";
	  exit(1);
	}
      return addr_DDnode;
    }
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
	if(dag->Get_DAGnode_outlist_size() != 2)
		{
		cout<<"\nNumber of children for AND is not two in DDManager::createDDFromDAG\n";
		exit(1);
		}

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
	if(dag->Get_DAGnode_outlist_size() != 2)
		{
		cout<<"\nNumber of children for OR is not two in DDManager::createDDFromDAG\n";
		exit(1);
		}
	
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

// This function initializes the APMap
void DDManager::initializeAPMap(list<CDAGNode*> &APList, map<string, int> WidthTable, CHelper &MANAGER)
{
  bool reverse_natural_order=false; // reverse natural order normally results in larger dds 
  if(reverse_natural_order)
    APList.reverse();
  list<CDAGNode*>::iterator it=APList.begin();
  for(;it!=APList.end(); it++)
    {
      CDAGNode* predicate = *it;
      cout<<"\nOriginal Equation = "<<predicate->getDAGStr()<<endl;
      predicate = recreateEquationInProperForm(predicate, WidthTable, MANAGER);
      cout<<"\nRecreated Equation = "<<predicate->getDAGStr()<<endl;
      
      if(APMap.find(predicate) == APMap.end()) // predicate is not there already in the APMap. Hence insert the AP
	    {
	      count++;
	      if(count>MAX_AP) 

		{cout<<"\nIn function  DDManager ::initializeAPMap , count has exceeded MAX_AP. Increase MAX_AP. Presently it is "<<MAX_AP<<"\n";
		exit(1);
		}

	      if(checkIfIndexExists(count)) // an entry exists in APMap with the same index : ERROR
		{
		  cout<<"\nIn DDManager::initializeAPMap, an entry exists in APMap with the given index "<<count<<endl;
		  exit(1);
		}
	      APMap.insert((pair<CDAGNode*,int>(predicate,count)));
	    }
      else // already there
	    {
	      cout<<"\nEquation is already there\n";
	    }
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

      u = createDD(u_value);
      result = TableForApplyModified.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyStepModified\n";
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
      u_low = applyStepModified(op, v_low1, v_low2);
      u_high = applyStepModified(op, v_high1, v_high2);

      u = createDD(u_AP, u_low, u_high);


      result = TableForApplyModified.hashtable_insert(key, u);
      if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyStepModified\n";
	    exit(1);
	  }

      return u;
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



  u_low = applyITEModified(condn_next_low, then_next_low, else_next_low);
  u_high = applyITEModified(condn_next_high, then_next_high, else_next_high);

  u = createDD(u_AP, u_low, u_high);

  result = TableForITEModified.hashtable_insert(key, u);
  if(!result.success())
	  { cout<<"\nHash Insert Failure Inside Function DDManager::applyITEModified\n";
	    exit(1);
	  }

  return u;
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
      if(v->getPredicate()!=NULL) getNoOfNodesInDD(v->getLeft(), NoOfNodesInDD);
      if(v->getPredicate()!=NULL) getNoOfNodesInDD(v->getRight(), NoOfNodesInDD);

      NoOfNodesInDD++; // New DDNode encountered
      SetOfDDNodes.insert(v);
    }
}


DDNode* DDManager::createDDNot(DDNode* v1)
{
  //DDNode* result = apply("xor", v1, createDD(1));

  DDNode* result = applyModified("xor", v1, createDD(1));

  return result;
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

  //cout<<"\ncreateDAGFromDDStep was not earlier called with argument "<<dd<<"\n";
  CDAGNode *result;
  if(dd->getPredicate()==NULL)
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

int DDManager::getNoOfBitsInConstraint(CDAGNode* root, map<string, int> WidthTable)
{
  // Todo
  set<string> names;
  root->getDAGNames(names);

  int maximum=0;

  for(set<string>::iterator it=names.begin(); it!=names.end(); it++)
    {
      string my_name=*it;
      int my_width = WidthTable[my_name];
      if(my_width>maximum) maximum = my_width;

      //cout<<endl<<my_name<<"\t"<<my_width<<endl;
    }


  //cout<<"\nmaximum = "<<maximum<<endl;

  return maximum;

}

void DDManager::writeBVDeclarationsInYicesFormat(FILE *smt_file, map<string, int> WidthTable)
{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
    {

      fprintf(smt_file,"(define %s",(it->first).c_str());
      fprintf(smt_file," :: (bitvector %d))\n",it->second);
    }

}


bool DDManager::ThUnsatSingleFormula(CDAGNode* f, map<string, int> WidthTable, CHelper &MANAGER)
{
  FILE *smt_file = fopen("thunsat_single_formula.ys", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);




  string dag_str = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(f, WidthTable);

 
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



void DDManager::Traverse_WithDPAdvanced(DDNode *v, list<DDNode*> vlist[])
{
  //procedure Traverse(v:vertex);

  set<DDNode*>::iterator it = SetOfDDNodes.find(v);

  if(it != SetOfDDNodes.end()) // dag with root v is already visited. Don't Go down
    {
      //cout<<"\nAlready visited\n";
      return;
    }
 
  if(v->getPredicate()!= NULL) // {v nonterminal}
    {
      Traverse_WithDPAdvanced(v->getLeft(), vlist);
      Traverse_WithDPAdvanced(v->getRight(), vlist);
    }

  
  vlist[getIndex(v)].push_back(v);

  SetOfDDNodes.insert(v);
  
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


// Search MyList for element
int DDManager::searchList(list<string> MyList, string element)
{
  for(list<string>::iterator it=MyList.begin(); it != MyList.end(); it++)
    if(*it==element) return 1;
  return 0;
}

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
      //cout<<"\nNo variables to eliminate. Return\n";
      return root;
    }

  set<string> VarsToElimOriginal;
  set<string> VarsToElimRenamed;
  root->getExistsDAGNames(VarsToElimOriginal);
  list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
  iout++;
  CDAGNode *DagPart = *iout;

  // Quick simplify the DagPart added on 31-03-'11

  bool calling_quicksimplify_before_dd_creation = true;

  if(calling_quicksimplify_before_dd_creation)
    {
      map<string, string> QuickSimplificationTable;
      
      DagPart = MANAGER.callQuickSimplify(DagPart, QuickSimplificationTable);
    }


  
  // Quick simplify the DagPart ends here


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




  cout<<"\nGetting the linearized dag\n";

  
  bool make_autonames_independent = true;

  CDAGNode* LinearDag;
  
  if(make_autonames_independent)
    {
      LinearDag = MANAGER.getLinearizedCloneWithGlobalCount(DagPart, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
    }
  else
    {
      LinearDag = MANAGER.getLinearizedClone(DagPart, LinearizationTable, AutoNameCount, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
    }

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

  set<string> VarSetToElim;

  for(list<string>::iterator vars_it = VarsToElim.begin();vars_it !=  VarsToElim.end(); vars_it++)
    {
      VarSetToElim.insert(*vars_it);
    }

  cout<<"\nVarSetToElim\n";
  for(set<string>::iterator vars_it = VarSetToElim.begin();vars_it !=  VarSetToElim.end(); vars_it++)
    {
      cout<<*vars_it<<endl;
    }


  MANAGER.checkForPropSkeltonModified(LinearDag);

  cout<<"\n\nDag's structure confirmed to be propositional\n\n";


  bool order_prefixing=true;
  bool order_based_on_vars_to_elim = false;

  if(order_prefixing)
    {
      list<CDAGNode*> APList;

      cout<<"\nGetting the APs in the DAG\n";

      MANAGER.getAPsForDAGInPropSkelton(LinearDag, APList);

      cout<<"\nAPList\n-------\n";
      MANAGER.showAPList(APList);

      
      if(order_based_on_vars_to_elim)
	{
	  MANAGER.modifyAPListBasedOnVarsToElim(APList, VarSetToElim);
	  cout<<"\nAPList\n-------\n";
	  MANAGER.showAPList(APList);
	}
  
      cout<<"\nInitializing the APMap\n";
      initializeAPMap(APList, WidthTable, MANAGER);
      cout<<"\nAPMap initialized\n";
    }


  bool input_needed = true;

  if(input_needed) 
    {
//       cout<<"\nLinearDag = "<<LinearDag->getDAGStr()<<endl;
//       cout<<"\nVarSetToElim\n";
//       for(set<string>::iterator vars_it = VarSetToElim.begin();vars_it !=  VarSetToElim.end(); vars_it++)
// 	{
// 	  cout<<*vars_it<<endl;
// 	}

      
      
      MANAGER.printQEliminatorResultsInFile(LinearDag, WidthTable, VarsToElim);
    }




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


  if(false) {printAPMap(); }


  //cout<<"\ndd = "<<createDAGFromDD(dd, MANAGER)->getDAGStr()<<endl;


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


  // Newly added on 30 March 2011 to simplify the dag

  bool calling_quicksimplify = true;

  if(calling_quicksimplify)
    {
      map<string, string> QuickSimplificationTable;
      
      dag_eliminated = MANAGER.callQuickSimplify(dag_eliminated, QuickSimplificationTable);
    }

  // Newly added on 30 March 2011 to simplify the dag ends here
  




  // Getting both original dag and dag_eliminated in files to see if dag_eliminated = exists(V, dag)

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

  
  //printAPMap();
  

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

void DDManager::showList(list<string> names, string WhoAmI)
{
  cout<<"\n"<<WhoAmI<<"\n********\n";
  for(list<string>::iterator it=names.begin(); it!=names.end();it++)
    cout<<*it<<endl;
}



DDNode* DDManager::MultiVarQElimWithStrategy(list<string> VarsToElim, DDNode* f, CHelper &MANAGER, map<string, int> WidthTable)
{
  time_t drop_time, t2;

  DDNode *result;
  result = f;

  //cout<<"\nresult before the loop = "<<result->getDDString()<<endl;

  while(!VarsToElim.empty())
    {
      showList(VarsToElim, "VarsToElim");

      cout<<"\nNo more variables to drop\n";

      string VarToElim = chooseVar(result, VarsToElim);

      cout<<"\nVariable chosen to eliminate = "<<VarToElim<<endl;

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

      //cout<<"\noutputDD = "<<createDAGFromDD(result, MANAGER)->getDAGStr()<<endl;

      VarsToElim.remove(VarToElim);
    }
  return result;
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

      if(v->getPredicate()!=NULL)// non-terminal
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


DDNode* DDManager::callQElim(string VarToElim, DDNode* f, CHelper &MANAGER, map<string, int> WidthTable)
{
  TableForDagResolve.clear();

  set<CDAGNode*> E, D;
  E.clear(); D.clear();

  HashTable <string, DDNode*> TableForAPResolveModified;
  HashTable <string, DDNode*> *PtrTableForAPResolveModified = &TableForAPResolveModified;

  HashTable <string, DDNode*> TableForQElimModified;
  HashTable <string, DDNode*> *PtrTableForQElimModified = &TableForQElimModified;

  return QElimWithDPModified(VarToElim, f, E, D, MANAGER, WidthTable, PtrTableForQElimModified, PtrTableForAPResolveModified);
}



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
  if(f->getPredicate()==NULL) // Terminal node
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

              f = DagResolveModifiedAdvancedWithDynamicVariableOrdering(f, e1, VarToElim, MANAGER, WidthTable);

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



bool DDManager::DDFreeOfVar(DDNode *f, string VarToElim)
{
  if(f->getPredicate()!=NULL) //non-terminal
    {
      CDAGNode *predicate = f->getPredicate();
      if(!(predicate->freeOfVarModified(VarToElim))) return false;
      if(!DDFreeOfVar(f->getLeft(), VarToElim)) return false;
      if(!DDFreeOfVar(f->getRight(), VarToElim)) return false;
      return true;
    }
  return true;
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



DDNode*  DDManager::DagResolveModifiedAdvancedWithDynamicVariableOrdering(DDNode *dd, CDAGNode *equation, string VarToElim, CHelper &MANAGER, map<string, int> WidthTable)
{

  //cout<<"\nDAGResolve called\n";

  map< pair<DDNode*, CDAGNode*>, DDNode*>::iterator it = TableForDagResolve.find(make_pair(dd, equation));

  if(it != TableForDagResolve.end())
    {
      return it->second;
    }

  if(dd->getPredicate() == NULL) // Terminal
    {
      return dd;
    }

  DDNode *NewLeft = DagResolveModifiedAdvancedWithDynamicVariableOrdering(dd->getLeft(), equation, VarToElim, MANAGER, WidthTable);
  DDNode *NewRight = DagResolveModifiedAdvancedWithDynamicVariableOrdering(dd->getRight(), equation, VarToElim, MANAGER, WidthTable);

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

      NoVars=VarNames.size();

      // Now we have the list of variables,  NoVars, NoVarsToElim

      set<CDAGNode*> Constraints;
      Constraints.insert(equation);
      Constraints.insert(MyPredicate);

     
      // Get the equations
      bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

      ULINT Mod=findPower(NoOfBits);

      // Now everything is ready. Just call Matrix qEliminaror

      // Checking if Constraints is False before calling the eliminator

      set<CDAGNode*> TemporaryDisEqSet;
      TemporaryDisEqSet.clear();
      bool theory_unsat = ThUnsat(Constraints, TemporaryDisEqSet, WidthTable, MANAGER);
      if(theory_unsat)
	{
	  NewPredicate = MANAGER.createDAG("0");
	}
      else
	{
	  NewPredicate = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, MANAGER, VarNames, WidthTable);
	  if(NewPredicate == NULL) 
	    {
	      NewPredicate = MANAGER.createDAG("1");
	    }
	  else // we have a new LME
	    {
	      if(!(NewPredicate->Get_DAGnode_value()=="=") && !(NewPredicate->Get_DAGnode_value()=="equivalent")) // not an LME
		{
		  cout<<"DDManager::DagResolveModifiedAdvancedWithDynamicVariableOrdering creats a new node which is not an LME \n";
		  exit(1);
		}

              //cout<<"\nNewPredicate = "<<NewPredicate->getDAGStr()<<endl;

	      NewPredicate = recreateEquationInProperForm(NewPredicate, WidthTable, MANAGER); // recreate in cannonical form

              //cout<<"\nNewPredicate = "<<NewPredicate->getDAGStr()<<", MyPredicate = "<<MyPredicate->getDAGStr()<<endl;

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

  codn_dd = createDDFromDAG(NewPredicate, MANAGER, WidthTable);

  DDNode* resultDD = createITEModified(codn_dd, then_dd, els_dd );

  TableForDagResolve.insert(make_pair(make_pair(dd, equation), resultDD));

  return resultDD;
}


void DDManager::dynamicOrdering(CDAGNode *MyPredicate, CDAGNode *NewPredicate)
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

void DDManager::incrementIndexOfLMEsInAPMAPAboveGivenIndex(int MyIndex)
{
  map<CDAGNode*, int>::iterator APMap_it = APMap.begin();
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

// RemainingVarNames = VarNames without first entry
void DDManager::createRemainingVarNames(list<string> VarNames, list<string> &RemainingVarNames)
{
  list<string>::iterator it = VarNames.begin();
  if(it == VarNames.end()) return;

  it++;
  for(;it != VarNames.end(); it++)
    RemainingVarNames.push_back(*it);
}

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
      v1_index = getIndex(v1);
      v2_index = getIndex(v2);
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

bool DDManager::coeffIsOdd(CDAGNode *equation, string VarToElim)
{
  ULINT coefficient=1;
  equation->getCoefficient(VarToElim, coefficient); // Now we have the coefficient of VarToElim in equation
  if(coefficient % 2 != 0) return true; // coefficient is odd
  return false;
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
