
/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#include "BitBlaster.h"
#include "util.h"
#include "cudd.h"
#include <sys/time.h>

extern DdManager *manager;

extern unsigned long long int zminimal_duration_ms;

extern bool blast_and_output;
extern set<string> independent_variables;

// util.h, cudd.h for including the CUDD library

// Internal function called by the function getBitBlastedDAG in BitBlaster.cc
// Given a bit-vector formula rooted at "node", returns the bits of the propositional
// logic formual obtained by blasting "node", as a vector<DAGNode *>
vector<DAGNode *> getBitBlastedDAGRecursively(DAGNode * node)
{
  struct timeval step_start_ms, step_finish_ms;    
  unsigned long long int step_time;  
  struct timeval compute_start_ms, compute_finish_ms;    
  unsigned long long int compute_time;  
  
    
    
  vector<DAGNode *> Bits;
  if(node->Get_visited_flag() == true)
	{  
        // code for memoization can be added here
	}
  node->Set_visited_flag();
  if(node->Get_outlist_size()==0) // leaf level
    {
      string MyName = node->Get_node_name();
      Value* MyValue = (Value*)(node->getValue());
      Type MyType = MyValue->get_node_type();
      int MyWidth = MyType.width;
      int MyDataType = MyType.data_type;


      if(MyDataType == LOGICAL_CONSTANT)
	    {

	      //cout<<"\nI am "<<MyName<<" with DataType = "<<MyDataType<<" and with width = "<<MyWidth<<"\n";
	      Bits.push_back(node);
	       
	   }

      else if(MyDataType == BITVECTOR_SYMBOL)
	    {
              if(PrintBlastTimes)
                {
                    gettimeofday (&step_start_ms, NULL);        
                }
	      //cout<<"\nI am "<<MyName<<" with DataType = "<<MyDataType<<" and with width = "<<MyWidth<<"\n";

	      for(int index=0; index<MyWidth; index++)
		{

		  Value *v = new Value();
		  Type *t = new Type();
		  t->data_type = LOGICAL_SYMBOL;
		  t->width = 1;
		  v->set_node_type(*t);

		  string Bit_Name = MyName;
		  Bit_Name += "_";

		  char index_char[100];
		  sprintf(index_char, "%d", index);
		  string index_str = index_char;
		  Bit_Name += index_str;

		  DAGNode *bit_node = dm.createNode (Bit_Name,(void*)v);
		  Bits.push_back(bit_node);
		}
              
              if(PrintBlastTimes)
                {
                    gettimeofday (&step_finish_ms, NULL);
                    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time blast symbol = %llu\t", step_time);
                }
	       
	}
      else if(MyDataType == BITVECTOR_CONSTANT)
	{
	  //cout<<"\nI am "<<MyName<<" with DataType = "<<MyDataType<<" and with width = "<<MyWidth<<"\n";
          
          if(PrintBlastTimes)
                {
                    gettimeofday (&step_start_ms, NULL);        
                }

	  unsigned long long int number=strtoull(MyName.c_str(), NULL, 0);
	  //cout<<"\nnumber = "<<number<<endl;
	  MyName = integerToBinaryStringWithZeroPadding(number, MyWidth);
	  //cout<<"\nMyName = "<<MyName<<endl;
	  for(int index=MyWidth-1; index>=0; index--)
	    {

	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);

	      string Bit_Name;
	      if(MyName[index]=='0')
		Bit_Name="false";
	      else
		Bit_Name="true";

	      DAGNode *bit_node = dm.createNode (Bit_Name,(void*)v);

	      Bits.push_back(bit_node);
	    }
          
          if(PrintBlastTimes)
                {
                    gettimeofday (&step_finish_ms, NULL);
                    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time blast constant = %llu\t", step_time);
                }
	   

	}
    }

  else
  {
  string MyName = node->Get_node_name();

  if(MyName == "=")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
      if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWarning!Width mismatch inside function getBitBlastedDAGRecursively for =\n";
	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);	 
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      
      vector<DAGNode *> ChildrenOfAnd;

      for(int index=0; index<BitsChild1.size(); index++)
	{

	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = LOGICAL_SYMBOL;
	  t->width = 1;
	  v->set_node_type(*t);

	  vector<DAGNode *> ChildrenOfEqual;
	  ChildrenOfEqual.push_back(BitsChild1[index]);
	  ChildrenOfEqual.push_back(BitsChild2[index]);

	  DAGNode * equal_node = dm.createNode("=",ChildrenOfEqual, (void*)v);
	  ChildrenOfAnd.push_back(equal_node);
	}

      if(ChildrenOfAnd.size()==0)
	{
	  cout<<"\nError! = node encountered with no children in bit-blasting\n"; exit(1);
	}
    
      else if(ChildrenOfAnd.size()==1)
	{
	  Bits.push_back(ChildrenOfAnd[0]);
          BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(ChildrenOfAnd[0], node)); // node is blasted to ChildrenOfAnd[0]
	}
      else
	{
    
	  DAGNode *and_node = createDAG("and",ChildrenOfAnd[0], ChildrenOfAnd[1], LOGICAL_SYMBOL, 1);
	  for(int index=2; index<BitsChild1.size(); index++)
		  and_node = createDAG("and",and_node, ChildrenOfAnd[index], LOGICAL_SYMBOL, 1);
	  Bits.push_back(and_node);
          BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(and_node, node)); // node is blasted to and_node
	}

       
    }

   else if(MyName == "if_then_else")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild3 = getBitBlastedDAGRecursively(*it);
      
      if(BitsChild1.size() != 1 || BitsChild2.size() != 1 || BitsChild3.size() != 1)
	{
	  cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for if_then_else\n";
	  exit(1);
	}
      
      vector<DAGNode *> ChildrenOfIte;
      ChildrenOfIte.push_back(BitsChild1[0]);
      ChildrenOfIte.push_back(BitsChild2[0]);
      ChildrenOfIte.push_back(BitsChild3[0]);

      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_SYMBOL;
      t->width = 1;
      v->set_node_type(*t);


      DAGNode * ite_node = dm.createNode("if_then_else",ChildrenOfIte, (void*)v);
      Bits.push_back(ite_node);
       
    }

    else if(MyName == "and")
    {
      if(node->Get_outlist_size()==2)
	{
	  list<DAGNode*>::iterator it = node->Get_outlist_start();
	  vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
	  it++;
	  vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
	  if(BitsChild1.size() != 1 || BitsChild2.size() != 1)
	    {
	      cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for and\n";
	      exit(1);
	    }
      
	  vector<DAGNode *> ChildrenOfAnd;
	  ChildrenOfAnd.push_back(BitsChild1[0]);
	  ChildrenOfAnd.push_back(BitsChild2[0]);

	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = LOGICAL_SYMBOL;
	  t->width = 1;
	  v->set_node_type(*t);


	  DAGNode * and_node = dm.createNode("and",ChildrenOfAnd, (void*)v);
	  Bits.push_back(and_node);
	   
	}
      else if(node->Get_outlist_size() > 2)
        {
          vector<DAGNode *> ChildrenOfAnd;
          vector<DAGNode *> BitsChild;
          
          
          list<DAGNode*>::iterator it = node->Get_outlist_start();
          for(unsigned int child_it=0; child_it<node->Get_outlist_size(); child_it++, it++)
            {
            BitsChild = getBitBlastedDAGRecursively(*it);
            
            if(BitsChild.size() != 1)
                {
                cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for and\n";
                exit(1);
                }
            
            ChildrenOfAnd.push_back(BitsChild[0]);            
            BitsChild.clear();
            }
          
          
          DAGNode* and_node = createDAG("and", ChildrenOfAnd[0], ChildrenOfAnd[1], LOGICAL_SYMBOL, 1);
          
          for(unsigned int child_it = 2; child_it < node->Get_outlist_size(); child_it++)
            {
             and_node = createDAG("and", and_node, ChildrenOfAnd[child_it], LOGICAL_SYMBOL, 1); 
            }
          
	  Bits.push_back(and_node);
        }
      else
	{
	  cout<<"\nnumber of children of and is "<<node->Get_outlist_size()<<". Less than 2 of "<<getDAGStr(node)<<"\n";
	  exit(1);
	}
    }

   else if(MyName == "or")
    {
      if(node->Get_outlist_size()==2)
	{
	  list<DAGNode*>::iterator it = node->Get_outlist_start();
	  vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
	  it++;
	  vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
	  if(BitsChild1.size() != 1 || BitsChild2.size() != 1)
	    {
	      cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for or\n";
	      exit(1);
	    }
      
	  vector<DAGNode *> ChildrenOfOr;
	  ChildrenOfOr.push_back(BitsChild1[0]);
	  ChildrenOfOr.push_back(BitsChild2[0]);

	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = LOGICAL_SYMBOL;
	  t->width = 1;
	  v->set_node_type(*t);


	  DAGNode * or_node = dm.createNode("or",ChildrenOfOr, (void*)v);
	  Bits.push_back(or_node);
	  
	}
      else if(node->Get_outlist_size() > 2)
        {
          vector<DAGNode *> ChildrenOfOr;
          vector<DAGNode *> BitsChild;
          
          
          list<DAGNode*>::iterator it = node->Get_outlist_start();
          for(unsigned int child_it=0; child_it<node->Get_outlist_size(); child_it++, it++)
            {
            BitsChild = getBitBlastedDAGRecursively(*it);
            
            if(BitsChild.size() != 1)
                {
                cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for and\n";
                exit(1);
                }
            
            ChildrenOfOr.push_back(BitsChild[0]);            
            BitsChild.clear();
            }
          
          DAGNode* or_node = createDAG("or", ChildrenOfOr[0], ChildrenOfOr[1], LOGICAL_SYMBOL, 1);
          
          for(unsigned int child_it = 2; child_it < node->Get_outlist_size(); child_it++)
            {
             or_node = createDAG("or", or_node, ChildrenOfOr[child_it], LOGICAL_SYMBOL, 1); 
            }
          
	  Bits.push_back(or_node);
        } 
      else
	{
	 cout<<"\nnumber of children of or is less than 2\n";
	 exit(1);
	} 
    }

  else if(MyName == "not")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      
      vector<DAGNode *> ChildrenOfNot;
      ChildrenOfNot.push_back(BitsChild1[0]);
 
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_SYMBOL;
      t->width = 1;
      v->set_node_type(*t);


      DAGNode * not_node = dm.createNode("not",ChildrenOfNot, (void*)v);
      Bits.push_back(not_node);
      
    }

  else if(MyName == "bvadd")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
       if(PrintBlastTimes)
                {
                    gettimeofday (&step_start_ms, NULL);        
                }
      
      if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for bvadd\n";

	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      
      vector<DAGNode *> Carry;      

      vector<DAGNode *> ChildrenOfAnd;
      ChildrenOfAnd.push_back(BitsChild1[0]);
      ChildrenOfAnd.push_back(BitsChild2[0]);
      
	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
	
	DAGNode * and_node = dm.createNode("and",ChildrenOfAnd, (void*)v);
	Carry.push_back(and_node);
	


      for(int index=1; index<BitsChild1.size()-1; index++)
	{
         vector<DAGNode *> ChildrenOfOr;

	 vector<DAGNode *> ChildrenOfAnd;
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(BitsChild2[index]);
      	Value *v1 = new Value();
	Type *t1 = new Type();
	t1->data_type = LOGICAL_SYMBOL;
	t1->width = 1;
	v1->set_node_type(*t1);
	DAGNode * and_node1 = dm.createNode("and",ChildrenOfAnd, (void*)v);
	ChildrenOfOr.push_back(and_node1);

	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(Carry[index-1]);
      	Value *v2 = new Value();
	Type *t2 = new Type();
	t2->data_type = LOGICAL_SYMBOL;
	t2->width = 1;
	v2->set_node_type(*t2);
	DAGNode * and_node2 = dm.createNode("and",ChildrenOfAnd, (void*)v);
	ChildrenOfOr.push_back(and_node2);
	
	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(BitsChild2[index]);
      	 ChildrenOfAnd.push_back(Carry[index-1]);
      	Value *v3 = new Value();
	Type *t3 = new Type();
	t3->data_type = LOGICAL_SYMBOL;
	t3->width = 1;
	v3->set_node_type(*t3);
	DAGNode * and_node3 = dm.createNode("and",ChildrenOfAnd, (void*)v);
	ChildrenOfOr.push_back(and_node3);



	  DAGNode *or_node1 = createDAG("or", ChildrenOfOr[0], ChildrenOfOr[1], LOGICAL_SYMBOL, 1);
	  DAGNode *or_node = createDAG("or", or_node1, ChildrenOfOr[2], LOGICAL_SYMBOL, 1);

	  Carry.push_back(or_node);
	}

       for(int index=0; index<BitsChild1.size(); index++)
	{
	 vector<DAGNode *> ChildrenOfExor;
      	 ChildrenOfExor.push_back(BitsChild1[index]);
      	 ChildrenOfExor.push_back(BitsChild2[index]);
	if(index>0) ChildrenOfExor.push_back(Carry[index-1]);

	
	DAGNode * exor_node;
	DAGNode *exor_node1 = createDAG("exor", ChildrenOfExor[0], ChildrenOfExor[1], LOGICAL_SYMBOL, 1);
	if(index>0)
		exor_node = createDAG("exor", exor_node1, ChildrenOfExor[2], LOGICAL_SYMBOL, 1);
	else
		exor_node = exor_node1;


	Bits.push_back(exor_node);
	}
        
         if(PrintBlastTimes)
                {
                  gettimeofday (&step_finish_ms, NULL);
                  step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                  step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                  fprintf(LMIQEOutputDataFp, "time blast bvadd given blasted children = %llu\t", step_time);
                }

     }


  else if(MyName == "bvmul")
    {
          bool ApplyEfficientProcessingForMinusVariable = true;
          
          if(ApplyEfficientProcessingForMinusVariable && ItIsMinusVariable(node))//check if the bvmul appears as constant * variable, where constant is the 
              // maximum constant with the given width. 
              // In this case, constant * variable is effectively -variable i.e. ~variable + 1
          {
              vector<DAGNode *> Variable;
              
              list<DAGNode*>::iterator it = node->Get_outlist_start();
              DAGNode* Child1 = *it;
              it++;
              DAGNode* Child2 = *it;
              
              if(getNodeType(Child1).data_type == BITVECTOR_CONSTANT)
                {
                  Variable = getBitBlastedDAGRecursively(Child2);
                }
              else  if(getNodeType(Child2).data_type == BITVECTOR_CONSTANT)
                {
                  Variable = getBitBlastedDAGRecursively(Child1); 
                }
              else
                {
                  cout<<"\nbvmul applied on operands both of which are non-constant\n";
                  exit(1);
                }     
              
               if(PrintBlastTimes)
                {
                    gettimeofday (&step_start_ms, NULL);        
                }
            
            
            //First operand is ~Variable
            vector<DAGNode *> NegatedVariable;
            
            for(vector<DAGNode*>::iterator var_it = Variable.begin(); var_it != Variable.end(); var_it++) 
                {
                   NegatedVariable.push_back(createDAG("not", *var_it, LOGICAL_SYMBOL, 1));
                }
            
            //Second operand is 1
            
            vector<DAGNode *> BlastedOne;
            
            BlastedOne.push_back(createLogicalConstant("true"));
            
            for(int i=1; i<Variable.size() ;i++) 
                {
                BlastedOne.push_back(createLogicalConstant("false"));
                }
            
            vector<DAGNode *> Product;
            
             if(PrintBlastTimes)
                {
                    gettimeofday (&compute_start_ms, NULL);        
                }
            
            Product = getSumOfArrays(NegatedVariable, BlastedOne);
            
            if(PrintBlastTimes)
                {
                    gettimeofday (&compute_finish_ms, NULL);
                    compute_time = compute_finish_ms.tv_sec * 1000 + compute_finish_ms.tv_usec / 1000;
                    compute_time -= compute_start_ms.tv_sec * 1000 + compute_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time consumed in getSumOfArrays = %llu\t", compute_time);
                } 
            
            int i=0;
            for(vector<DAGNode*>::iterator Product_it = Product.begin(); Product_it != Product.end() && i<Variable.size(); Product_it++, i++)
                {
                  Bits.push_back(*Product_it);
                }
            
            if(PrintBlastTimes)
                {
                    gettimeofday (&step_finish_ms, NULL);
                    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time blast bvmul given blasted children = %llu\t", step_time);
                } 
              
          }
          else
          {

              list<DAGNode*>::iterator it = node->Get_outlist_start();
              vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
              it++;
              vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);

              if(BitsChild1.size() != BitsChild2.size())
                {
                  //cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for bvmul\n";

                  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
                  //cout<<"\nZero padding the shorter one to match with the longer one\n";
                  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
                  zeroPadding(BitsChild1, needed_width);
                  zeroPadding(BitsChild2, needed_width);
                  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
                }


              int whichIsConstantArray = getConstantArray(BitsChild1, BitsChild2);

              if(whichIsConstantArray == 0)
                {
                  cout<<"\nbvmul applied on operands both of which are non-constant\n";
                  exit(1);
                }

              vector<DAGNode *> Constant;
              vector<DAGNode *> Variable;

              if(whichIsConstantArray == 1)
                {
                //cout<<"\nArray1 is constant\n";
                Constant = BitsChild1;
                Variable =  BitsChild2;
                //showVector(Constant, "Constant");
                //showVector(Variable, "Variable");
                }
              else if(whichIsConstantArray == 2)
                {
                //cout<<"\nArray2 is constant\n";  
                Constant = BitsChild2;
                Variable =  BitsChild1;
                //showVector(Constant, "Constant");
                //showVector(Variable, "Variable");
                }
              else
                {
                 cout<<"\nUnknown value for whichIsConstantArray in function getBitBlastedDAGRecursively\n";
                 exit(1);
                } 

              vector< vector <DAGNode*> > OperandsOfAddition;
              getOperandsOfAddition(Variable, Constant, OperandsOfAddition);
             //cout<<"\nOperands of addition\n";
             // for(vector< vector <DAGNode*> >::iterator Operands_it = OperandsOfAddition.begin(); Operands_it != OperandsOfAddition.end(); Operands_it++)
             //	{
            //	 showVector(*Operands_it, ""); 
            //	}


              vector <DAGNode*> Product;
              if(OperandsOfAddition.size()==1) 

                {
                  Product = OperandsOfAddition[0];
                }

              else if(OperandsOfAddition.size()==2) 
                {
                  Product = getSumOfArrays(OperandsOfAddition[0], OperandsOfAddition[1]);
                }

              else
                {
                 Product = getSumOfArrays(OperandsOfAddition[0], OperandsOfAddition[1]);
                 //showVector(Product, "Product");
                 vector< vector <DAGNode*> >::iterator Operands_it = OperandsOfAddition.begin(); 
                 Operands_it++;
                 Operands_it++;
                 for(;Operands_it != OperandsOfAddition.end(); Operands_it++)
                   {
                     Product = getSumOfArrays(Product, *Operands_it);
                     //showVector(Product, "Product");
                   }
                }

              //cout<<"\nResult\n";

              //showVector(Product, "Product");

              int i=0;
              for(vector<DAGNode*>::iterator Product_it = Product.begin(); Product_it != Product.end() && i<BitsChild1.size(); Product_it++, i++)
                {
                  Bits.push_back(*Product_it);
                }
          }

      //cout<<"\nFinal result\n";
      
      //showVector(Bits, "Bits");
 
    }
 
  else if(MyName == "is_not_equal")
    {
      DAGNode *NegatedNode = negateConstraint(node);
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(NegatedNode);
      
      vector<DAGNode *> ChildrenOfNot;
      ChildrenOfNot.push_back(BitsChild1[0]);
 
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_SYMBOL;
      t->width = 1;
      v->set_node_type(*t);


      DAGNode * not_node = dm.createNode("not",ChildrenOfNot, (void*)v);
      Bits.push_back(not_node);
      
      BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(not_node, node)); // node is blasted to not_node
      
    }
  else if(MyName == "select")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      DAGNode *low_sel = *it;
      it++;
      DAGNode *high_sel = *it;

      string low_sel_string = low_sel->Get_node_name();
      string high_sel_string = high_sel->Get_node_name();

      int low_sel_number = atoi(low_sel_string.c_str());
      int high_sel_number = atoi(high_sel_string.c_str());

      int i=0;
      for(vector<DAGNode*>::iterator BitsChild1_it = BitsChild1.begin(); BitsChild1_it != BitsChild1.end() && i<BitsChild1.size(); BitsChild1_it++, i++)
	{
	  if(i>=low_sel_number && i<=high_sel_number) // within selection
	    {
	      Bits.push_back(*BitsChild1_it);
	    }
	}
    }
	
  else if(MyName == "concat")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);

      for(vector<DAGNode*>::iterator BitsChild1_it = BitsChild1.begin(); BitsChild1_it != BitsChild1.end(); BitsChild1_it++)
	{
	      Bits.push_back(*BitsChild1_it);
	}
      for(vector<DAGNode*>::iterator BitsChild2_it = BitsChild2.begin(); BitsChild2_it != BitsChild2.end(); BitsChild2_it++)
	{
	      Bits.push_back(*BitsChild2_it);
	}

    }
  else if(MyName == "bvult")
    {
      // Obtain the bits of child1 and child2
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
      // Negate the bits of child 2
      if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for bvult\n";

	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      vector<DAGNode *> NegatedBitsChild2;  
      for(int i=0; i<BitsChild2.size(); i++)
	{
      	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
        
       vector<DAGNode *> ChildrenOfNot;
       ChildrenOfNot.push_back(BitsChild2[i]);
	
	DAGNode * not_node = dm.createNode("not",ChildrenOfNot, (void*)v);
	NegatedBitsChild2.push_back(not_node);
      }
     // Negation of bits of child2 obtained in NegatedBitsChild2
     
      // Get  (BitsChild1+NegatedBitsChild2+Cin=1).carry_out 
        
      vector<DAGNode *>  Carry;
      Carry.push_back(createLogicalConstant("true")); // Cin = 1
      
      for(int index=0; index<BitsChild1.size(); index++)
	{
         vector<DAGNode *> ChildrenOfOr;

	 vector<DAGNode *> ChildrenOfAnd;
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	Value *v1 = new Value();
	Type *t1 = new Type();
	t1->data_type = LOGICAL_SYMBOL;
	t1->width = 1;
	v1->set_node_type(*t1);
	DAGNode * and_node1 = dm.createNode("and",ChildrenOfAnd, (void*)v1); // A/\B
	ChildrenOfOr.push_back(and_node1);

	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v2 = new Value();
	Type *t2 = new Type();
	t2->data_type = LOGICAL_SYMBOL;
	t2->width = 1;
	v2->set_node_type(*t2);
	DAGNode * and_node2 = dm.createNode("and",ChildrenOfAnd, (void*)v2);// A/\Cin
	ChildrenOfOr.push_back(and_node2);
	
	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v3 = new Value();
	Type *t3 = new Type();
	t3->data_type = LOGICAL_SYMBOL;
	t3->width = 1;
	v3->set_node_type(*t3);
	DAGNode * and_node3 = dm.createNode("and",ChildrenOfAnd, (void*)v3);// B/\Cin
	ChildrenOfOr.push_back(and_node3);



	  DAGNode *or_node1 = createDAG("or", ChildrenOfOr[0], ChildrenOfOr[1], LOGICAL_SYMBOL, 1);
	  DAGNode *or_node = createDAG("or", or_node1, ChildrenOfOr[2], LOGICAL_SYMBOL, 1);

	  Carry.push_back(or_node);// Cout = A/\B \/ A/\Cin \/ B/\Cin
	}

      // Bits = negation of carry_out from the last bit
        vector<DAGNode *> ChildrenOfNot;
       ChildrenOfNot.push_back(Carry[Carry.size()-1]);
      
      
	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
	
	DAGNode * not_node = dm.createNode("not",ChildrenOfNot, (void*)v);
        Bits.push_back(not_node);
        
        BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(not_node, node)); // node is blasted to not_node
    }

  else if(MyName == "bvuge")
    {
      // Obtain the bits of child1 and child2
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
      // Negate the bits of child 2
      if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for bvult\n";

	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      vector<DAGNode *> NegatedBitsChild2;  
      for(int i=0; i<BitsChild2.size(); i++)
	{
      	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
        
       vector<DAGNode *> ChildrenOfNot;
       ChildrenOfNot.push_back(BitsChild2[i]);
	
	DAGNode * not_node = dm.createNode("not",ChildrenOfNot, (void*)v);
	NegatedBitsChild2.push_back(not_node);
      }
     // Negation of bits of child2 obtained in NegatedBitsChild2
     

      // Get  (BitsChild1+NegatedBitsChild2+Cin=1).carry_out 
        
      vector<DAGNode *>  Carry;
      Carry.push_back(createLogicalConstant("true")); // Cin = 1
      
      for(int index=0; index<BitsChild1.size(); index++)
	{
         vector<DAGNode *> ChildrenOfOr;

	 vector<DAGNode *> ChildrenOfAnd;
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	Value *v1 = new Value();
	Type *t1 = new Type();
	t1->data_type = LOGICAL_SYMBOL;
	t1->width = 1;
	v1->set_node_type(*t1);
	DAGNode * and_node1 = dm.createNode("and",ChildrenOfAnd, (void*)v1); // A/\B
	ChildrenOfOr.push_back(and_node1);

	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v2 = new Value();
	Type *t2 = new Type();
	t2->data_type = LOGICAL_SYMBOL;
	t2->width = 1;
	v2->set_node_type(*t2);
	DAGNode * and_node2 = dm.createNode("and",ChildrenOfAnd, (void*)v2);// A/\Cin
	ChildrenOfOr.push_back(and_node2);
	
	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v3 = new Value();
	Type *t3 = new Type();
	t3->data_type = LOGICAL_SYMBOL;
	t3->width = 1;
	v3->set_node_type(*t3);
	DAGNode * and_node3 = dm.createNode("and",ChildrenOfAnd, (void*)v3);// B/\Cin
	ChildrenOfOr.push_back(and_node3);



	  DAGNode *or_node1 = createDAG("or", ChildrenOfOr[0], ChildrenOfOr[1], LOGICAL_SYMBOL, 1);
	  DAGNode *or_node = createDAG("or", or_node1, ChildrenOfOr[2], LOGICAL_SYMBOL, 1);

	  Carry.push_back(or_node);// Cout = A/\B \/ A/\Cin \/ B/\Cin
	}
      // Bits = carry_out from the last bit
       Bits.push_back(Carry[Carry.size()-1]);
        
       BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(Carry[Carry.size()-1], node)); // node is blasted to Carry[Carry.size()-1]
    }
  
  else if(MyName == "bvule")
    {
      // Obtain the bits of child1 and child2
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
      
       if(PrintBlastTimes)
                {
                    gettimeofday (&step_start_ms, NULL);        
                }
      
      // Negate the bits of child 2
      if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for bvult\n";

	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      vector<DAGNode *> NegatedBitsChild2;  
      for(int i=0; i<BitsChild2.size(); i++)
	{
      	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
        
       vector<DAGNode *> ChildrenOfNot;
       ChildrenOfNot.push_back(BitsChild2[i]);
	
	DAGNode * not_node = dm.createNode("not",ChildrenOfNot, (void*)v);
	NegatedBitsChild2.push_back(not_node);
      }
     // Negation of bits of child2 obtained in NegatedBitsChild2
     
     // Get  (BitsChild1+NegatedBitsChild2+Cin=1).carry_out 
        
      vector<DAGNode *>  Carry;
      Carry.push_back(createLogicalConstant("true")); // Cin = 1
      
      for(int index=0; index<BitsChild1.size(); index++)
	{
         vector<DAGNode *> ChildrenOfOr;

	 vector<DAGNode *> ChildrenOfAnd;
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	Value *v1 = new Value();
	Type *t1 = new Type();
	t1->data_type = LOGICAL_SYMBOL;
	t1->width = 1;
	v1->set_node_type(*t1);
	DAGNode * and_node1 = dm.createNode("and",ChildrenOfAnd, (void*)v1); // A/\B
	ChildrenOfOr.push_back(and_node1);

	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v2 = new Value();
	Type *t2 = new Type();
	t2->data_type = LOGICAL_SYMBOL;
	t2->width = 1;
	v2->set_node_type(*t2);
	DAGNode * and_node2 = dm.createNode("and",ChildrenOfAnd, (void*)v2);// A/\Cin
	ChildrenOfOr.push_back(and_node2);
	
	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v3 = new Value();
	Type *t3 = new Type();
	t3->data_type = LOGICAL_SYMBOL;
	t3->width = 1;
	v3->set_node_type(*t3);
	DAGNode * and_node3 = dm.createNode("and",ChildrenOfAnd, (void*)v3);// B/\Cin
	ChildrenOfOr.push_back(and_node3);



	  DAGNode *or_node1 = createDAG("or", ChildrenOfOr[0], ChildrenOfOr[1], LOGICAL_SYMBOL, 1);
	  DAGNode *or_node = createDAG("or", or_node1, ChildrenOfOr[2], LOGICAL_SYMBOL, 1);

	  Carry.push_back(or_node);// Cout = A/\B \/ A/\Cin \/ B/\Cin
	}
        // Bits = (negation of carry_out from the last bit)  OR (BitsChild1 = BitsChild2)
        vector<DAGNode *> ChildrenOfNot;
       ChildrenOfNot.push_back(Carry[Carry.size()-1]);
      
      
	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
	
	DAGNode * less_node = dm.createNode("not",ChildrenOfNot, (void*)v);  
        
        DAGNode * eq_node;
        
         if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWarning!Width mismatch inside function getBitBlastedDAGRecursively for bvule\n";
	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);	 
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      
      vector<DAGNode *> ChildrenOfAnd;

      for(int index=0; index<BitsChild1.size(); index++)
	{

	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = LOGICAL_SYMBOL;
	  t->width = 1;
	  v->set_node_type(*t);

	  vector<DAGNode *> ChildrenOfEqual;
	  ChildrenOfEqual.push_back(BitsChild1[index]);
	  ChildrenOfEqual.push_back(BitsChild2[index]);

	  DAGNode * equal_node = dm.createNode("=",ChildrenOfEqual, (void*)v);
	  ChildrenOfAnd.push_back(equal_node);
	}

      if(ChildrenOfAnd.size()==0)
	{
	  cout<<"\nError! bvule node encountered with no children in bit-blasting\n"; exit(1);
	}
    
      else if(ChildrenOfAnd.size()==1)
	{
	  eq_node = ChildrenOfAnd[0];
	}
      else
	{
    
	  DAGNode *and_node = createDAG("and",ChildrenOfAnd[0], ChildrenOfAnd[1], LOGICAL_SYMBOL, 1);
	  for(int index=2; index<BitsChild1.size(); index++)
		  and_node = createDAG("and",and_node, ChildrenOfAnd[index], LOGICAL_SYMBOL, 1);
	  eq_node = and_node;
	}
        
       vector<DAGNode*> ChildrenOfOr;
       ChildrenOfOr.push_back(less_node);
       ChildrenOfOr.push_back(eq_node);
      
      
	v = new Value();
	t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
	
	DAGNode * lte_node = dm.createNode("or",ChildrenOfOr, (void*)v);  
        
         Bits.push_back(lte_node);
        
         BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(lte_node, node)); // node is blasted to lte_node
         
                  
          if(PrintBlastTimes)
                {
                    gettimeofday (&step_finish_ms, NULL);
                    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time blast bvule given blasted children = %llu\t", step_time);
                } 
         
         
    }
  
  else if(MyName == "bvugt")
    {
      // Obtain the bits of child1 and child2
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1 = getBitBlastedDAGRecursively(*it);
      it++;
      vector<DAGNode *> BitsChild2 = getBitBlastedDAGRecursively(*it);
      
      // Negate the bits of child 2
      if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWidth mismatch inside function getBitBlastedDAGRecursively for bvult\n";

	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      vector<DAGNode *> NegatedBitsChild2;  
      for(int i=0; i<BitsChild2.size(); i++)
	{
      	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
        
       vector<DAGNode *> ChildrenOfNot;
       ChildrenOfNot.push_back(BitsChild2[i]);
	
	DAGNode * not_node = dm.createNode("not",ChildrenOfNot, (void*)v);
	NegatedBitsChild2.push_back(not_node);
      }
     // Negation of bits of child2 obtained in NegatedBitsChild2
     
       // Get  (BitsChild1+NegatedBitsChild2+Cin=1).carry_out 
        
      vector<DAGNode *>  Carry;
      Carry.push_back(createLogicalConstant("true")); // Cin = 1
      
      for(int index=0; index<BitsChild1.size(); index++)
	{
         vector<DAGNode *> ChildrenOfOr;

	 vector<DAGNode *> ChildrenOfAnd;
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	Value *v1 = new Value();
	Type *t1 = new Type();
	t1->data_type = LOGICAL_SYMBOL;
	t1->width = 1;
	v1->set_node_type(*t1);
	DAGNode * and_node1 = dm.createNode("and",ChildrenOfAnd, (void*)v1); // A/\B
	ChildrenOfOr.push_back(and_node1);

	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(BitsChild1[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v2 = new Value();
	Type *t2 = new Type();
	t2->data_type = LOGICAL_SYMBOL;
	t2->width = 1;
	v2->set_node_type(*t2);
	DAGNode * and_node2 = dm.createNode("and",ChildrenOfAnd, (void*)v2);// A/\Cin
	ChildrenOfOr.push_back(and_node2);
	
	ChildrenOfAnd.clear();
      	 ChildrenOfAnd.push_back(NegatedBitsChild2[index]);
      	 ChildrenOfAnd.push_back(Carry[index]);
      	Value *v3 = new Value();
	Type *t3 = new Type();
	t3->data_type = LOGICAL_SYMBOL;
	t3->width = 1;
	v3->set_node_type(*t3);
	DAGNode * and_node3 = dm.createNode("and",ChildrenOfAnd, (void*)v3);// B/\Cin
	ChildrenOfOr.push_back(and_node3);



	  DAGNode *or_node1 = createDAG("or", ChildrenOfOr[0], ChildrenOfOr[1], LOGICAL_SYMBOL, 1);
	  DAGNode *or_node = createDAG("or", or_node1, ChildrenOfOr[2], LOGICAL_SYMBOL, 1);

	  Carry.push_back(or_node);// Cout = A/\B \/ A/\Cin \/ B/\Cin
	}

        // Bits = (negation of carry_out from the last bit)  OR (BitsChild1 = BitsChild2)
        vector<DAGNode *> ChildrenOfNot;
       ChildrenOfNot.push_back(Carry[Carry.size()-1]);
      
      
	Value *v = new Value();
	Type *t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
	
	DAGNode * less_node = dm.createNode("not",ChildrenOfNot, (void*)v);  
        
        DAGNode * eq_node;
        
         if(BitsChild1.size() != BitsChild2.size())
	{
	  //cout<<"\nWarning!Width mismatch inside function getBitBlastedDAGRecursively for bvule\n";
	  //cout<<"\nWidths of children are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	  //cout<<"\nZero padding the shorter one to match with the longer one\n";
	  
	  int needed_width = (BitsChild1.size()>BitsChild2.size())?BitsChild1.size():BitsChild2.size();
	  zeroPadding(BitsChild1, needed_width);
	  zeroPadding(BitsChild2, needed_width);	 
	  //cout<<"\nWidths of children now are "<<BitsChild1.size()<<" , "<<BitsChild2.size()<<endl;
	}
      
      vector<DAGNode *> ChildrenOfAnd;

      for(int index=0; index<BitsChild1.size(); index++)
	{

	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = LOGICAL_SYMBOL;
	  t->width = 1;
	  v->set_node_type(*t);

	  vector<DAGNode *> ChildrenOfEqual;
	  ChildrenOfEqual.push_back(BitsChild1[index]);
	  ChildrenOfEqual.push_back(BitsChild2[index]);

	  DAGNode * equal_node = dm.createNode("=",ChildrenOfEqual, (void*)v);
	  ChildrenOfAnd.push_back(equal_node);
	}

      if(ChildrenOfAnd.size()==0)
	{
	  cout<<"\nError! bvule node encountered with no children in bit-blasting\n"; exit(1);
	}
    
      else if(ChildrenOfAnd.size()==1)
	{
	  eq_node = ChildrenOfAnd[0];
	}
      else
	{
    
	  DAGNode *and_node = createDAG("and",ChildrenOfAnd[0], ChildrenOfAnd[1], LOGICAL_SYMBOL, 1);
	  for(int index=2; index<BitsChild1.size(); index++)
		  and_node = createDAG("and",and_node, ChildrenOfAnd[index], LOGICAL_SYMBOL, 1);
	  eq_node = and_node;
	}
        
       vector<DAGNode*> ChildrenOfOr;
       ChildrenOfOr.push_back(less_node);
       ChildrenOfOr.push_back(eq_node);
      
      
	v = new Value();
	t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
	
	DAGNode * lte_node = dm.createNode("or",ChildrenOfOr, (void*)v);  
        
        
        ChildrenOfNot.clear();
       ChildrenOfNot.push_back(lte_node);
      
      
	v = new Value();
	t = new Type();
	t->data_type = LOGICAL_SYMBOL;
	t->width = 1;
	v->set_node_type(*t);
	
	DAGNode * gt_node = dm.createNode("not",ChildrenOfNot, (void*)v);  
        
         Bits.push_back(gt_node);
         
         BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(gt_node, node)); // node is blasted to gt_node
        
    }
  
  
  else
	{
	cout<<"\nOperator "<<MyName<<" is not handled in function getBitBlastedDAGRecursively in BitBlaster.cc\n";
	exit(1);
	}
	

   }

return Bits; 

}

// Given a bit-vector formula, return the proposotional
// formula obtained by blasting it
DAGNode* getBitBlastedDAG(DAGNode *node)
{
  bool stp_style_implementation = true;
  
  if(stp_style_implementation)
  {
      //add code for dynamic programming here
      map<DAGNode*, vector<DAGNode*> > BBFormMem;
      
      vector<DAGNode *> Bits;     
      Bits = BBForm(node, BBFormMem);
      if(Bits.size()>1) 
        {
          cout<<"\nWarning!Bit-blasting required on non-Boolean dag\n";
        } 
      return Bits[0];      
  }
  else
  {
      DAGNode* BlastedDAG;
      vector<DAGNode *> Bits;
      Bits.clear();

      bool reset_visited = dm.ResetVisitedFlagOfDAGNodes();
      if(reset_visited == false)
        {
          cout<<"\nCould not reset visited flag of all dag nodes\n";
          exit(1);
        }


     Bits = getBitBlastedDAGRecursively(node);

      if(Bits.size()>1) 
        {
          cout<<"\nWarning!Bit-blasting required on non-Boolean dag\n";
        } 
      return Bits[0];
  }//else ends here
}//function

// Get label table which gives unique labels to the nodes in the 
// dag for the propositional logic formula rooted at "node".
void getLabelTable(DAGNode *node, map<DAGNode*, string> &LabelTable)
{

  unsigned long long int label_count = 1;
 
  bool reset_visited = dm.ResetVisitedFlagOfDAGNodes();

  if(reset_visited == false)
    {
      cout<<"\nCould not reset visited flag of all DAG nodes\n";
      exit(1);
    }

  getLabelTableRecursively(node, label_count, LabelTable);
}

// Internal function called by the function getLabelTable
void getLabelTableRecursively(DAGNode *node, unsigned long long int &label_count, map<DAGNode*, string> &LabelTable)
{
  if(node->Get_visited_flag() == true)
    return;
  node->Set_visited_flag();
 
     char count[100];
     sprintf(count,"%llu",label_count);
     string Label = count;
     LabelTable.insert (pair<DAGNode*, string>(node, Label));
     label_count++;

     list<DAGNode*>::iterator it = node->Get_outlist_start(); 
     for(unsigned int i=0; i<node->Get_outlist_size(); i++, it++)
		{
		getLabelTableRecursively(*it, label_count, LabelTable);
		}
}

// Get CNF for the propositional logic formula rooted at "node" 
// in terms of the labels in the "LabelTable"
void getCNF(DAGNode *node, map<DAGNode*, string> &LabelTable, set<string> &CNF)
{

  CNF.insert("1 0");

  bool reset_visited = dm.ResetVisitedFlagOfDAGNodes();
  if(reset_visited == false)
    {
      cout<<"\nCould not reset visited flag of all DAG nodes\n";
      exit(1);
    }

  getCNFRecursively(node, LabelTable, CNF);
}

// Internal function called by function getCNFRecursively
void getCNFRecursively(DAGNode *node, map<DAGNode*, string> &LabelTable, set<string> &CNF)
{

    if(node->Get_visited_flag() == true)
    	return;
    node->Set_visited_flag();

    
    string node_label = LabelTable[node];

              if(node->Get_node_name()=="true" )

		{

                    if(node->Get_outlist_size() != 0) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}

 		    string current_1 = ""; // node_label 
		    current_1 += node_label;
		    current_1 += " ";
		    current_1 += "0";

                    CNF.insert(current_1);
		}

	     else if(node->Get_node_name()=="false" )

		{

                    if(node->Get_outlist_size() != 0) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}

 		    string current_1 = ""; // - node_label 
	            current_1 +="-";
		    current_1 += node_label;
		    current_1 += " ";
		    current_1 += "0";

                    CNF.insert(current_1);
		}

              else if(node->Get_node_name()=="not" )

		{

                    if(node->Get_outlist_size() != 1) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}


                    string child_1_label ;
                    
		    list<DAGNode*>::iterator iout = node->Get_outlist_start();
                    
                    child_1_label = LabelTable[*iout];
			
		    string current_1 = ""; // -1 -2
		    current_1 +="-";
		    current_1 += node_label;
		    current_1 += " ";
		    current_1 +="-";
		    current_1 += child_1_label;
		    current_1 += " 0";
		    
		    string current_2 = ""; // 1 2
 		    current_2 += node_label;
		    current_2 += " ";
		    current_2 += child_1_label;
		    current_2 += " 0";
		   
   
		    CNF.insert(current_1);
		    CNF.insert(current_2);
	
		}
                       
        else if(node->Get_node_name()=="and" )

		{

                    if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}

                    string child_1_label ;
                    string child_2_label ;

		    list<DAGNode*>::iterator iout = node->Get_outlist_start();
                    
                    child_1_label = LabelTable[*iout];
		    iout++;
                    child_2_label = LabelTable[*iout];

			
		    string current_1 = ""; // -1 2
		    current_1 +="-";
		    current_1 += node_label;
		    current_1 += " ";
		    current_1 += child_1_label;
		    current_1 += " 0";
		    
		    string current_2 = ""; // -1 3
                    current_2 +="-";
		    current_2 += node_label;
		    current_2 += " ";
		    current_2 += child_2_label;
		    current_2 += " 0";
		   
		    string current_3 = ""; // 1 -2 -3
                    current_3 += node_label;
		    current_3 += " ";
		    current_3 +="-";
		    current_3 += child_1_label;
		    current_3 += " ";
		    current_3 +="-";
		    current_3 += child_2_label;
		    current_3 += " 0";
		   
		    CNF.insert(current_1);
		    CNF.insert(current_2);
		    CNF.insert(current_3);
		
		}

              else if(node->Get_node_name()=="or" )  
                { 

                    if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}

		    string child_1_label ;
                    string child_2_label ;

		    list<DAGNode*>::iterator iout = node->Get_outlist_start();
                    
                    child_1_label = LabelTable[*iout];
		    iout++;
                    child_2_label = LabelTable[*iout];
			
		    string current_1 = ""; // 1 -2
		    current_1 += node_label;
		    current_1 += " ";
		    current_1 +="-";
		    current_1 += child_1_label;
		    current_1 += " 0";
		    
		    string current_2 = ""; // 1 -3
                    current_2 += node_label;
		    current_2 += " ";
		    current_2 +="-";
		    current_2 += child_2_label;
		    current_2 += " 0";
		   
		    string current_3 = ""; // -1 2 3
		    current_3 +="-";
                    current_3 += node_label;
		    current_3 += " ";
		    current_3 += child_1_label;
		    current_3 += " ";
		    current_3 += child_2_label;
		    current_3 += " 0";
		   
		    CNF.insert(current_1);
		    CNF.insert(current_2);
		    CNF.insert(current_3);
		
		    }
             else if(node->Get_node_name()=="=" )  
                { 

                    if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}

		    string child_1_label ;
                    string child_2_label ;

		    list<DAGNode*>::iterator iout = node->Get_outlist_start();
                    
                    child_1_label = LabelTable[*iout];
		    iout++;
                    child_2_label = LabelTable[*iout];
			
		    string current_1 = ""; // -1 -2 3
                    current_1 +="-";
		    current_1 += node_label;
		    current_1 += " ";
		    current_1 +="-";
		    current_1 += child_1_label;
                    current_1 += " ";
		    current_1 += child_2_label;
		    current_1 += " 0";
		    
		    string current_2 = ""; // -1 -3 2
                    current_2 +="-";
                    current_2 += node_label;
		    current_2 += " ";
		    current_2 +="-";
		    current_2 += child_2_label;
                    current_2 += " ";
 		    current_2 += child_1_label;
		    current_2 += " 0";
		   
		    string current_3 = ""; // 1 2 3
                    current_3 += node_label;
		    current_3 += " ";
		    current_3 += child_1_label;
		    current_3 += " ";
		    current_3 += child_2_label;
		    current_3 += " 0";

                    string current_4 = ""; // 1 -2 -3
                    current_4 += node_label;
		    current_4 += " ";
                    current_4 +="-";
		    current_4 += child_1_label;
		    current_4 += " ";
                    current_4 +="-";
		    current_4 += child_2_label;
		    current_4 += " 0";

		   
		    CNF.insert(current_1);
		    CNF.insert(current_2);
		    CNF.insert(current_3);
                    CNF.insert(current_4);
		
		    }

               else if(node->Get_node_name()=="exor" )  
                { 

                    if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}

		    string child_1_label ;
                    string child_2_label ;

		    list<DAGNode*>::iterator iout = node->Get_outlist_start();
                    
                    child_1_label = LabelTable[*iout];
		    iout++;
                    child_2_label = LabelTable[*iout];
			
		    string current_1 = ""; // 1 -2 3
 		    current_1 += node_label;
		    current_1 += " ";
		    current_1 +="-";
		    current_1 += child_1_label;
                    current_1 += " ";
		    current_1 += child_2_label;
		    current_1 += " 0";
		    
		    string current_2 = ""; // 1 -3 2
                    current_2 += node_label;
		    current_2 += " ";
		    current_2 +="-";
		    current_2 += child_2_label;
                    current_2 += " ";
 		    current_2 += child_1_label;
		    current_2 += " 0";
		   
		    string current_3 = ""; // -1 2 3
                    current_3 +="-";
                    current_3 += node_label;
		    current_3 += " ";
		    current_3 += child_1_label;
		    current_3 += " ";
		    current_3 += child_2_label;
		    current_3 += " 0";

                    string current_4 = ""; // -1 -2 -3
                    current_4 +="-";
                    current_4 += node_label;
		    current_4 += " ";
                    current_4 +="-";
		    current_4 += child_1_label;
		    current_4 += " ";
                    current_4 +="-";
		    current_4 += child_2_label;
		    current_4 += " 0";

		   
		    CNF.insert(current_1);
		    CNF.insert(current_2);
		    CNF.insert(current_3);
                    CNF.insert(current_4);
		
		    }
	    else if(node->Get_node_name()=="if_then_else" )  
                { 

                    if(node->Get_outlist_size() != 3) 
			{
			cout<<"\nUnexpected outlist size in CNF creation\n";
			exit(1);
			}

		    string child_1_label ;
                    string child_2_label ;
		    string child_3_label ;

		    list<DAGNode*>::iterator iout = node->Get_outlist_start();
                    
                    child_1_label = LabelTable[*iout];
		    //cout<<"\nchild_1_ data = "<<(*iout)->Get_DAGnode_value()<<"\tchild_1_label = "<<child_1_label<<"\n";
		    iout++;
                    child_2_label = LabelTable[*iout];
		    //cout<<"\nchild_2_ data = "<<(*iout)->Get_DAGnode_value()<<"\tchild_2_label = "<<child_2_label<<"\n";
		    iout++;
                    child_3_label = LabelTable[*iout];
		    //cout<<"\nchild_3_ data = "<<(*iout)->Get_DAGnode_value()<<"\tchild_3_label = "<<child_3_label<<"\n";
			
		    string current_1 = ""; // -1 -2 3
		    current_1 +="-";
		    current_1 += node_label;
		    current_1 += " ";
		    current_1 +="-";
		    current_1 += child_1_label;
		    current_1 += " ";
		    current_1 += child_2_label;
		    current_1 += " 0";
		    
		    string current_2 = ""; // -1 2 4 
		    current_2 +="-";
                    current_2 += node_label;
		    current_2 += " ";
		    current_2 += child_1_label;
		    current_2 += " ";
		    current_2 += child_3_label;
		    current_2 += " 0";
		   
		    string current_3 = ""; // 1 2 -4
		    current_3 += node_label;
		    current_3 += " ";
		    current_3 += child_1_label;
		    current_3 += " ";
		    current_3 +="-";
		    current_3 += child_3_label;
		    current_3 += " 0";

		    string current_4 = ""; // 1 -2 -3
		    current_4 += node_label;
		    current_4 += " ";
		    current_4 +="-";
		    current_4 += child_1_label;
		    current_4 += " ";
		    current_4 +="-";
		    current_4 += child_2_label;
		    current_4 += " 0";

		    string current_5 = ""; // 1 -3 -4
		    current_5 += node_label;
		    current_5 += " ";
		    current_5 +="-";
		    current_5 += child_2_label;
		    current_5 += " ";
		    current_5 +="-";
		    current_5 += child_3_label;
		    current_5 += " 0";

		    CNF.insert(current_1);
		    CNF.insert(current_2);
		    CNF.insert(current_3);
		    CNF.insert(current_4);
		    CNF.insert(current_5);
		
		    
                }

	    else if(node->Get_outlist_size()>0)
	      {
		cout<<"\nUnknown node "<<node<<" with name "<<node->Get_node_name()<<" in function getCNF\n";
		exit(1);
	      }
		
	      //cout<<"\nAfter processing node "<<node<<" with name "<<node->Get_node_name()<<", CNF is\n";
	
	      //set<string>::iterator CNF_it;

	      //for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
	     //	{
	     //	  cout<<"\n"<<*CNF_it;
             //    }


	list<DAGNode*>::iterator iout = node->Get_outlist_start();
        unsigned int outlist_size = node->Get_outlist_size();
     
        for(unsigned int i=0; i<outlist_size; i++, iout++)
		getCNFRecursively((*iout), LabelTable, CNF);

    }    
 
// Call QBF solver to solve \exists ExQuantifiedVars1. \forall 
// UniversallyQuantifiedVars. \exists ExQuantifiedVars2.CNF where CNF 
// represents the equisatisfiable CNF for some propositional logic formula
// return true if unsat; false if sat 
bool checkQBFUnsat(set<string> &CNF, int NumVars, set<string> &ExQuantifiedVars1, set<string> &UniversallyQuantifiedVars, set<string> &ExQuantifiedVars2)
{
  FILE *fcnf, *fres;
  string result;

  if( (fcnf = fopen("blasted.cnf", "w")) == NULL)
    cout<<"\nError! Cannot open file blasted.cnf for writing\n\n";
  
  int NumClauses = CNF.size();
  
  fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

  fprintf(fcnf, "e ");

  set<string>::iterator Vars_it;

  for(Vars_it = ExQuantifiedVars1.begin(); Vars_it != ExQuantifiedVars1.end(); Vars_it++)
    {
      fprintf(fcnf, "%s ", (*Vars_it).c_str());
    }

  fprintf(fcnf, "0\n");


  fprintf(fcnf, "a ");

  for(Vars_it = UniversallyQuantifiedVars.begin(); Vars_it != UniversallyQuantifiedVars.end(); Vars_it++)
    {
      fprintf(fcnf, "%s ", (*Vars_it).c_str());
    }

  fprintf(fcnf, "0\n");

  fprintf(fcnf, "e ");

  for(Vars_it = ExQuantifiedVars2.begin(); Vars_it != ExQuantifiedVars2.end(); Vars_it++)
    {
      fprintf(fcnf, "%s ", (*Vars_it).c_str());
    }

  fprintf(fcnf, "0\n");

  set<string>::iterator CNF_it;
  int temp=0;

  for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
  if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
  fclose(fcnf);

  time_t solver_start_time;
  time_t solver_end_time;
  
   struct timeval start_ms, finish_ms;
   unsigned long long int duration_ms;
    
  time(&solver_start_time);
  
  gettimeofday (&start_ms, NULL);
  
  system("QuBE7 blasted.cnf > blasted.txt");
  
  gettimeofday (&finish_ms, NULL);

  time(&solver_end_time);
  cout<<"\n\nSolving time = "<< (solver_end_time - solver_start_time) <<" SECONDS "<<endl<<endl;
  
  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
  cout<<"\n\nSolving time = "<< duration_ms <<" MILLI SECONDS "<<endl<<endl;

  if( (fres = fopen("blasted.txt", "r")) == NULL)
    cout<<"\nError! Cannot open file blasted.txt for reading\n\n";

  char word[100], c;
  int i;

  for(i=0;i<7;i++) {c=fgetc(fres); word[i]=c;}
	
  word[i]='\0'; printf("\n%s",word);

  if(strcmp(word, "s cnf 0")==0) { cout<<"\nUnsat\n"; fclose(fres); cout<<"\nf' => exists(X, f)\n"; return true;}
  else if(strcmp(word, "s cnf 1")==0) { cout<<"\nSat\n"; fclose(fres); cout<<"\nnot(f' => exists(X, f))\n"; return false;}
  else { cout<<"\nUnknown result from QBF Solver\n"; fclose(fres); cout<<"\nf' => exists(X, f) is unknown\n"; return false;}
}

// Check if "BitBlastedDAG" is the correct bit-blasted version of "DAG_to_blast"
// return true if exact; false otherwise
bool checkForExactnessOfBlasting(DAGNode* DAG_to_blast, map<string, int> &WidthTable, DAGNode *BitBlastedDAG)
{

  FILE *check_file = fopen("blast_check.ys", "w");

  if(check_file == NULL) 
   {cout<<"\nCannot create blast_check.ys\n"; exit(1);}


  writeBVDeclarationsInYicesFormat(check_file, WidthTable);


  string original_str;
  
  if(LMI_handling_enabled)
  {
     original_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(DAG_to_blast, WidthTable); 
  }
  else
  {
    original_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(DAG_to_blast, WidthTable);
  }
  
  
  string BitBlastedDAGInString = getBooleanDagAsString(BitBlastedDAG, "bool");
  
  fprintf(check_file,"\n(assert (not  (= %s %s) ) )", original_str.c_str(), BitBlastedDAGInString.c_str());

  fprintf(check_file,"\n(check)\n");
  
  fclose(check_file);

  system("yices -tc blast_check.ys > result_blast_check.txt");

  cout<<"\nYices called\n";

  FILE *result_check_file = fopen("result_blast_check.txt", "r");
  
  if(result_check_file == NULL) 
   {cout<<"\nCannot create result_blast_check.txt\n"; exit(1);}



  char result[100];
  fscanf(result_check_file, "%s", result);
  fclose(result_check_file);
  string result_str(result);

  cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat")
    {
       return true;
     }
      
  else if(result_str=="sat")
    { 
      return false;
    }
  else
    {
      cout<<"\nUnknown result from Yices"<<result_str<<endl;
      exit(1);
    }


}

// This function checks if \phi' \and \forall VarSetToElim.(\neg \phi) where 
// \phi' \equiv \exists VarSetToElim.(\phi). 
// "DAG_to_blast" here represents \phi' \and (\neg \phi), 
// returns true if \phi' \and \forall VarSetToElim.(\neg \phi); false otherwise
bool checkForExactnessInOtherDirection(DAGNode *DAG_to_blast, set<string> VarSetToElim, set<string> VarSetRemaining, map<string, int> WidthTable)
{
  ofstream *outfile = new ofstream();
  outfile->open("blasted_DAGFile.txt");
  DAGNode* node = DAG_to_blast;
  DAGNode* BitBlastedDAG = getBitBlastedDAG(node);
  cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
  *outfile<<"\nBlasted Formula "<<endl;
  dm.printDAG(BitBlastedDAG, outfile);
  outfile->close();

  bool debug_blasting = false;

  if(debug_blasting)
	{
  
  	if(!checkForExactnessOfBlasting(DAG_to_blast, WidthTable, BitBlastedDAG))
		{
		cout<<"\nBit blasting not exact\n"; exit(1);
		}
  	else
		{
		cout<<"\nBit blasting exact\n"; 
		}
	}

  // Creating equisatisfiable CNF
  // Creating a table which has a unique label for each dag node

   map<DAGNode*, string> LabelTable;
   getLabelTable(BitBlastedDAG, LabelTable);
   cout<<"\nLabel Table\n";
   map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
     }
   cout<<"\n";	
 
   set<string> ExQuantifiedVars1;
   set<string> UniversallyQuantifiedVars;
   set<string> ExQuantifiedVars2;

   LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
	{
	DAGNode *bit_node = (*LabelTable_it).first;
	string variable = bit_node->Get_node_name();
	
	Value* bit_node_value = (Value*)(node->getValue());
        Type bit_node_type = bit_node_value->get_node_type();
        int bit_node_width = bit_node_type.width;
        int bit_node_datatype = bit_node_type.data_type;

	if(bit_node_datatype == LOGICAL_CONSTANT || bit_node->Get_outlist_size() != 0) 
		{
		ExQuantifiedVars2.insert((*LabelTable_it).second);
		continue;
		}
	
	// we have a variable
        int index = variable.find_last_of("_");
	variable = variable.substr(0, index);

	if(VarSetToElim.find(variable) != VarSetToElim.end()) // this bit is from a variable to be eliminated. i.e. this bit is to be universally quantified
		{
                cout<<"\n"<<variable<<" is from a variable to be eliminated. i.e. this bit is to be universally quantified\n";
		UniversallyQuantifiedVars.insert((*LabelTable_it).second);
		}

	else if(VarSetRemaining.find(variable) != VarSetRemaining.end()) // this bit is from a variable remaining i.e. this bit is to be ex. quantified
		{
                cout<<"\n"<<variable<<" is from a variable to remain. i.e. this bit is to be ex. quantified\n";
		ExQuantifiedVars1.insert((*LabelTable_it).second);
		}
	else
	  {
	    cout<<"\nWarning!Cannot associate a quantifier with "<<variable<<" with datatype "<<bit_node_datatype<<" and width "<<bit_node_width<<"\n";
	    
	    if(variable == "true" || variable == "false")
	      ExQuantifiedVars2.insert((*LabelTable_it).second);
	    else
	      {
		exit(1);
	      }
	  }
	
	}

   set<string> CNF;
   cout<<"\nCreating equisatisfiable CNF\n";
   getCNF(BitBlastedDAG, LabelTable, CNF);
   cout<<"\nCNF created. Calling QBF Solver\n";
   return checkQBFUnsat(CNF, LabelTable.size(), ExQuantifiedVars1, UniversallyQuantifiedVars, ExQuantifiedVars2);

  
}

// returns 1 if "BitsChild1" is a vector containing true/false;
// returns 2 if "BitsChild2" is a vector containing true/false;
// returns 0 otherwise.
int getConstantArray(vector<DAGNode*> BitsChild1, vector<DAGNode*> BitsChild2)
{
  bool constant1=true, constant2=true;
  vector<DAGNode*>::iterator bit;

  for(bit=BitsChild1.begin(); bit!=BitsChild1.end(); bit++) 
    {
      if((*bit)->Get_node_name() != "true" && (*bit)->Get_node_name() != "false")
	
	{
	  constant1=false;
	  break;
	}
    }

  for(bit=BitsChild2.begin(); bit!=BitsChild2.end(); bit++) 
    {
      if((*bit)->Get_node_name() != "true" && (*bit)->Get_node_name() != "false")
	
	{
	  constant2=false;
	  break;
	}
    }

  if(constant1) return 1;
  if(constant2) return 2;
  return 0;
}
	
// Show the given vector of DAGNode*
void showVector(vector<DAGNode*> MyVector, string WhoAmI)
{
  cout<<"\n"<<WhoAmI<<"\n************\n";
  vector<DAGNode*>::iterator bit;
  for(bit=MyVector.begin(); bit!=MyVector.end(); bit++) 
    {
      cout<<(*bit)->Get_node_name()<<"\t";
    }
}

// Get the operands of the addition in "OperandsOfAddition"
void getOperandsOfAddition(vector <DAGNode*> Variable, vector <DAGNode*> Constant, vector< vector <DAGNode*> > &OperandsOfAddition)
{
 bool avoid_adding_false = true;//if constant_name == "false", then we will be adding all false, which can be avoided     

  int position=0;
  for(vector<DAGNode*>::iterator Constant_it = Constant.begin(); Constant_it != Constant.end(); Constant_it++, position++)
    {
      vector<DAGNode*> Bit;
      string constant_name = (*Constant_it)->Get_node_name();
      
      for(int i=0;i<position;i++) Bit.push_back(createLogicalConstant("false"));
      
      if(constant_name == "false")
	{
	  int variable_size=Variable.size();
	  for(int i=0;i<variable_size && Bit.size()<variable_size ;i++) Bit.push_back(createLogicalConstant("false"));
	}
      else if(constant_name == "true")
	{
	  for(vector<DAGNode*>::iterator var_it = Variable.begin(); var_it != Variable.end() &&  Bit.size()<Variable.size(); var_it++) Bit.push_back(*var_it);
	}
      else
	{
	  cout<<"Unknown logical constant inside function getOperandsOfAddition.\n"; exit(1);
	}
      
      if(avoid_adding_false)
        {
          if(constant_name == "true")// no need to add all false
            {
             OperandsOfAddition.push_back(Bit); 
            }
        }      
      else
        {
        OperandsOfAddition.push_back(Bit);
        }
    }//for
  
  if(avoid_adding_false)
    {
      if(OperandsOfAddition.size() == 0)// Let's check if every thing is false
        {
         vector<DAGNode*> Bit;
         // Let's create all false vector
         for(int i=0; i<Variable.size() ;i++) 
            {
             Bit.push_back(createLogicalConstant("false"));
            }
         //insert Bit to OperandsOfAddition
         OperandsOfAddition.push_back(Bit);
        }//if
    }// if(avoid_adding_false)
  
}


// Return the sum of the vectors "BitsChild1" and "BitsChild2"
vector <DAGNode*> getSumOfArrays(vector <DAGNode*> &BitsChild1, vector <DAGNode*> &BitsChild2)
{

  vector <DAGNode*> Bits;

  if(BitsChild1.size() != BitsChild2.size())
    {
      cout<<"\nWidth mismatch inside function getSumOfArrays\n";
      exit(1);
    }
      
  vector<DAGNode *> Carry;      

  vector<DAGNode *> ChildrenOfAnd;
  ChildrenOfAnd.push_back(BitsChild1[0]);
  ChildrenOfAnd.push_back(BitsChild2[0]);
      
  Value *v = new Value();
  Type *t = new Type();
  t->data_type = LOGICAL_SYMBOL;
  t->width = 1;
  v->set_node_type(*t);
	
  DAGNode * and_node = dm.createNode("and",ChildrenOfAnd, (void*)v);
  Carry.push_back(and_node);
	


  for(int index=1; index<BitsChild1.size()-1; index++)
    {
      vector<DAGNode *> ChildrenOfOr;

      vector<DAGNode *> ChildrenOfAnd;
      ChildrenOfAnd.push_back(BitsChild1[index]);
      ChildrenOfAnd.push_back(BitsChild2[index]);
      Value *v1 = new Value();
      Type *t1 = new Type();
      t1->data_type = LOGICAL_SYMBOL;
      t1->width = 1;
      v1->set_node_type(*t1);
      DAGNode * and_node1 = dm.createNode("and",ChildrenOfAnd, (void*)v);
      ChildrenOfOr.push_back(and_node1);

      ChildrenOfAnd.clear();
      ChildrenOfAnd.push_back(BitsChild1[index]);
      ChildrenOfAnd.push_back(Carry[index-1]);
      Value *v2 = new Value();
      Type *t2 = new Type();
      t2->data_type = LOGICAL_SYMBOL;
      t2->width = 1;
      v2->set_node_type(*t2);
      DAGNode * and_node2 = dm.createNode("and",ChildrenOfAnd, (void*)v);
      ChildrenOfOr.push_back(and_node2);
	
      ChildrenOfAnd.clear();
      ChildrenOfAnd.push_back(BitsChild2[index]);
      ChildrenOfAnd.push_back(Carry[index-1]);
      Value *v3 = new Value();
      Type *t3 = new Type();
      t3->data_type = LOGICAL_SYMBOL;
      t3->width = 1;
      v3->set_node_type(*t3);
      DAGNode * and_node3 = dm.createNode("and",ChildrenOfAnd, (void*)v);
      ChildrenOfOr.push_back(and_node3);



     DAGNode *or_node1 = createDAG("or", ChildrenOfOr[0], ChildrenOfOr[1], LOGICAL_SYMBOL, 1);
     DAGNode *or_node = createDAG("or", or_node1, ChildrenOfOr[2], LOGICAL_SYMBOL, 1);

      Carry.push_back(or_node);
    }

  for(int index=0; index<BitsChild1.size(); index++)
    {
      vector<DAGNode *> ChildrenOfExor;
      ChildrenOfExor.push_back(BitsChild1[index]);
      ChildrenOfExor.push_back(BitsChild2[index]);
      if(index>0) ChildrenOfExor.push_back(Carry[index-1]);

 
      DAGNode * exor_node;
      DAGNode *exor_node1 = createDAG("exor", ChildrenOfExor[0], ChildrenOfExor[1], LOGICAL_SYMBOL, 1);
      if(index>0)
 		exor_node = createDAG("exor", exor_node1, ChildrenOfExor[2], LOGICAL_SYMBOL, 1);
      else
		exor_node = exor_node1;
      Bits.push_back(exor_node);
    }

  return Bits; 
}


// Function to add add sufficient no: of zeros as msbs to vector Bits to make it of size width
void zeroPadding(vector <DAGNode*> &Bits, int width)
{
  int pad_size = width - Bits.size();
  for(int i=0; i<pad_size; i++)
    {
      Bits.push_back(createLogicalConstant("false"));
    }
}



/* Functions to take a dag A, bit-blast it to get B. Now given exists(X, A) in bv-level, we have exists({x1,...,xt}, B(x1,...,xn)) in bit-level. Now convert B to equisatisfiable CNF with internal labels l1,...,lm. So exists({x1,...,xt}, B(x1,...,xn)) <=> exists({x1,...,xt,l1,...,lm}, B'(x1,...,xn,l1,...,lm)) where B' is the eq.sat transformation of B. Now print exists({x1,...,xt,l1,...,lm}, B'(x1,...,xn,l1,...,lm)) in QDimacs format */


// Function to write exists({x1,...,xt,l1,...,lm}, B'(x1,...,xn,l1,...,lm)) in QDimacs format 
// in file blasted_qualmode.cnf 

void writeInQDimacs(set<string> &CNF, int NumVars, set<string> &ExQuantifiedVars1, set<string> &ExQuantifiedVars2)
{
  FILE *fcnf, *fres;
  string result;

  if( (fcnf = fopen("blasted_qualmode.cnf", "w")) == NULL)
    cout<<"\nError! Cannot open file blasted_qualmode.cnf for writing\n\n";
  
  int NumClauses = CNF.size();
  
  fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

  fprintf(fcnf, "e ");

  set<string>::iterator Vars_it;

  for(Vars_it = ExQuantifiedVars1.begin(); Vars_it != ExQuantifiedVars1.end(); Vars_it++)
    {
      fprintf(fcnf, "%s ", (*Vars_it).c_str());
    }

  for(Vars_it = ExQuantifiedVars2.begin(); Vars_it != ExQuantifiedVars2.end(); Vars_it++)
    {
      fprintf(fcnf, "%s ", (*Vars_it).c_str());
    }

  fprintf(fcnf, "0\n");


  set<string>::iterator CNF_it;
  int temp=0;

  for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
  if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
  fclose(fcnf);

  cout<<"\nblasted problem written to file blasted_qualmode.cnf\n\n";
  
}


// Function to create B' from A as described previously 
// in file blasted_qualmode.cnf

void bitBlastAndQuantify(DAGNode *DAG_to_blast, set<string> VarSetToElim, map<string, int> WidthTable)
{
  ofstream *outfile = new ofstream();
  outfile->open("blasted_DAGFile.txt");
  DAGNode* node = DAG_to_blast;
  DAGNode* BitBlastedDAG = getBitBlastedDAG(node);
  cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
  *outfile<<"\nBlasted Formula "<<endl;
  dm.printDAG(BitBlastedDAG, outfile);
  outfile->close();

  bool debug_blasting = false;

  if(debug_blasting)
	{
  
  	if(!checkForExactnessOfBlasting(DAG_to_blast, WidthTable, BitBlastedDAG))
		{
		cout<<"\nBit blasting not exact\n"; exit(1);
		}
  	else
		{
		cout<<"\nBit blasting exact\n"; 
		}
	}

  // Creating equisatisfiable CNF
  // Creating a table which has a unique label for each dag node

   map<DAGNode*, string> LabelTable;
   getLabelTable(BitBlastedDAG, LabelTable);
   cout<<"\nLabel Table\n";
   map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
     }
   cout<<"\n";	
 
   set<string> ExQuantifiedVars1;
   //set<string> UniversallyQuantifiedVars;
   set<string> ExQuantifiedVars2;

   LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
	{
	DAGNode *bit_node = (*LabelTable_it).first;
	string variable = bit_node->Get_node_name();
	
	Value* bit_node_value = (Value*)(node->getValue());
        Type bit_node_type = bit_node_value->get_node_type();
        int bit_node_width = bit_node_type.width;
        int bit_node_datatype = bit_node_type.data_type;

	if(bit_node_datatype == LOGICAL_CONSTANT || bit_node->Get_outlist_size() != 0) 
		{
		cout<<"\n"<<(*LabelTable_it).second<<" is an internal label. i.e. this bit is to be ex. quantified\n";
		ExQuantifiedVars2.insert((*LabelTable_it).second);
		continue;
		}
	
	// we have a variable
        int index = variable.find_last_of("_");
	variable = variable.substr(0, index);

	/*if(VarSetToElim.find(variable) != VarSetToElim.end()) // this bit is from a variable to be eliminated. i.e. this bit is to be universally quantified
		{
                cout<<"\n"<<variable<<" is from a variable to be eliminated. i.e. this bit is to be universally quantified\n";
		UniversallyQuantifiedVars.insert((*LabelTable_it).second);
		}*/

	if(VarSetToElim.find(variable) != VarSetToElim.end()) // this bit is from a variable to be eliminated i.e. this bit is to be ex. quantified
		{
                cout<<"\n"<<variable<<" is from a variable to quantify out. i.e. this bit is to be ex. quantified\n";
		ExQuantifiedVars1.insert((*LabelTable_it).second);
		}
	else
	  	{
		if(variable == "true" || variable == "false")
			{
			cout<<"\n"<<(*LabelTable_it).second<<" is from a true/false. i.e. this bit is to be ex. quantified\n";
			ExQuantifiedVars2.insert((*LabelTable_it).second);
			}
	    	else
	      		{
			cout<<variable<<" is from a variable to remain. So no quantifier with it. "<<" \n";
	      		}
	  	}
	
	}

   set<string> CNF;
   cout<<"\nCreating equisatisfiable CNF\n";
   getCNF(BitBlastedDAG, LabelTable, CNF);
   cout<<"\nCNF created. Writing in QDimacs format\n";
   
   writeInQDimacs(CNF, LabelTable.size(), ExQuantifiedVars1, ExQuantifiedVars2);
 
}


// The following functions are required to apply CUDD based ex. quantifier elimination

// Given "node" which is a propositional logic formula, and "VarSetToElim"  which is a set of variables,
// this function 1) creates a BDD for "node", 2) gets the indices of the variables to be eliminated, 
// and 3) returns the BDD.
DdNode* createBDD(DAGNode *node, DdManager *manager, map<DAGNode*, DdNode*> &DPTable, set<string> &VarSetToElim, set<int> &BooleanVarSetToElim) // node is a propositional logic formula
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
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
			exit(1);
			}
	        result = Cudd_ReadOne(manager);
	        Cudd_Ref(result);
		}

	 else if(node->Get_node_name()=="false" )

		{
		 if(node->Get_outlist_size() != 0) 
			{
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
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
				cout<<endl<<variable_name<<" is a variable to eliminate \n";
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
				cout<<endl<<variable_name<<" is a variable to eliminate \n";
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
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_Not(temp);
                 Cudd_Ref(result);
  		}                                           
        else if(node->Get_node_name()=="and" )
		{
		if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddAnd(manager,temp1,temp2);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,temp1);
		 //Cudd_RecursiveDeref(manager,temp2);
		}

              else if(node->Get_node_name()=="or" )  
                { 
		if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddOr(manager,temp1,temp2);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,temp1);
		 //Cudd_RecursiveDeref(manager,temp2);
		 }
             else if(node->Get_node_name()=="=" )  
                { 

                    if(node->Get_outlist_size() != 2) 
			{
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
			exit(1);
			}

		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
                 
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
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *temp1 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *temp2 = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddXor(manager,temp1,temp2);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,temp1);
		 //Cudd_RecursiveDeref(manager,temp2);
		}
	    else if(node->Get_node_name()=="if_then_else" )  
                { 

                    if(node->Get_outlist_size() != 3) 
			{
			cout<<"\nUnexpected outlist size in function createBDD in BitBlaster.cc\n";
			exit(1);
			}
		 list<DAGNode*>::iterator iout = node->Get_outlist_start();
	         DdNode *condn_dd = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
		 iout++;
		 DdNode *then_dd = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
	         iout++;
		 DdNode *else_dd = createBDD(*iout,manager,DPTable,VarSetToElim, BooleanVarSetToElim);
                 result = Cudd_bddIte(manager, condn_dd, then_dd, else_dd);
                 Cudd_Ref(result);
		 //Cudd_RecursiveDeref(manager,condn_dd);
		 //Cudd_RecursiveDeref(manager,then_dd);
		 //Cudd_RecursiveDeref(manager,else_dd);
                }

	    else if(node->Get_outlist_size()>0)
	      {
		cout<<"\nUnknown node "<<node<<" with name "<<node->Get_node_name()<<" in function function createBDD in BitBlaster.cc\n";
		exit(1);
	      }
	}//if(node->Get_outlist_size() != 0) 

DPTable.insert(make_pair(node, result));

return result;

} // Miss in DPTable


}


// Returns "BooleanVarSetToElim" as a cube
DdNode* createCubeBDD(set<int> &BooleanVarSetToElim, DdManager *manager)
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
DdNode* createBDDExternal(DAGNode *node, DdManager *manager, set<string> &VarSetToElim)
{
map<DAGNode*,DdNode*> DPTable;
set<int> BooleanVarSetToElim;

DdNode *BDD = createBDD(node, manager, DPTable, VarSetToElim, BooleanVarSetToElim);

//cout<<"\nBDD\n";
//Cudd_PrintDebug(manager, BDD, 0, 4);

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

DdNode *CubeBDD = createCubeBDD(BooleanVarSetToElim, manager);

//cout<<"\nCubeBDD\n";
//Cudd_PrintDebug(manager, CubeBDD, 0, 4);

DdNode *QuantifiedBDD = Cudd_bddExistAbstract(manager, BDD, CubeBDD);

//cout<<"\nQuantifiedBDD\n";
//Cudd_PrintDebug(manager, QuantifiedBDD, 0, 4);


return BDD;
}


// Calls the function createBDDExternal		
void quantifyAtBitLevel(DAGNode *node, set<string> &VarSetToElim) // node is a propositional logic formula
{
bool reinitializing_cudd = false;

if(reinitializing_cudd)
{
// Code to reinitialize the CUDD manager and the maps; all previous results are lost. This was the way we previously used to invoke CUDD
manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
NameIndexMap.clear();
IndexNameMap.clear();
bdd_index = 0;
}

DdNode *BDD = createBDDExternal(node, manager, VarSetToElim);

if(reinitializing_cudd)
{
// Code to quit CUDD
Cudd_Quit(manager);
}

}

// Given "DAG_to_blast" which is a bv-logic formula, and "VarSetToElim"  which is a set of variables,
// this function performs \exists VarSetToElim. DAG_to_blast using bit-blasting + BDD
void bitBlastAndQuantifyUsingCDD(DAGNode *DAG_to_blast, set<string> VarSetToElim, map<string, int> WidthTable)
{

unsigned long long int blasting_time, qelim_time,duration_ms;
struct timeval start_ms, finish_ms;
gettimeofday (&start_ms, NULL);



  //ofstream *outfile = new ofstream();
  //outfile->open("blasted_DAGFile.txt");
  DAGNode* node = DAG_to_blast;
  DAGNode* BitBlastedDAG = getBitBlastedDAG(node);
  //cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
  //*outfile<<"\nBlasted Formula "<<endl;
  //dm.printDAG(BitBlastedDAG, outfile);
  //outfile->close();

  bool debug_blasting = false;

  if(debug_blasting)
	{
  
  	if(!checkForExactnessOfBlasting(DAG_to_blast, WidthTable, BitBlastedDAG))
		{
		cout<<"\nBit blasting not exact\n"; exit(1);
		}
  	else
		{
		cout<<"\nBit blasting exact\n"; 
		}
	}

gettimeofday (&finish_ms, NULL);
duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

blasting_time = duration_ms;


  // Creating equisatisfiable CNF
  // Creating a table which has a unique label for each dag node

 //DAGNode *formula = createTemporaryDAG();

 DAGNode *formula = BitBlastedDAG;
 //cout<<"\nformula = "<<getDAGStr(formula)<<endl;

gettimeofday (&start_ms, NULL);

quantifyAtBitLevel(formula, VarSetToElim);

gettimeofday (&finish_ms, NULL);
duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

qelim_time = duration_ms;

cout<<"\nBlasting time = "<<blasting_time<<" m.secs , Quantification time "<<qelim_time<<" m.secs\n";

}

// Function to parse a given string and collect some words
void parse_print_string(char *line_char, set<string> &UnNeeded)
{
  
  int i=1;
  
  int index=0;

  char word[500];

  int word_index = 0;
  
  while(line_char[index]!='\0')
	  {
	    
	    char read = line_char[index];

	    if(read == '\t' || read == ' ' || read == '\n')
	      {
		if(word_index==0) // empty word
		  {
		    index++;
		  }
		else
		  {
		    word[word_index] = '\0';

		    UnNeeded.insert(word);
                      
		    i++;

		    word_index = 0;

		    index++;
		  }
	      }
	    else
	      {
		word[word_index] = line_char[index];

		word_index++;

		index++;
	      }
	      
	    
	  }

}

// Given the unsat. core in file "corefilename" in a format returned by SAT solver,
// get them in a set of strings "CoreClauses"
void getCoreClauses(set<string> &CNF, string corefilename, set<string> &CoreClauses)
{

set<string> UnNeeded;

FILE *fp = fopen(corefilename.c_str(), "r");
if (fp == NULL)
    {
      cout<<"\nError while opening file "<<corefilename<<" for reading\n";
      exit(1);
    }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;
while ((read = getline(&line, &len, fp)) != -1) {
    string line_str(line);
    //cout<<line_str<<endl;
    if(line_str.find("Unneeded")!=string::npos)
      {
    	cout<<line_str<<endl;
	break;
      }
    }

cout<<"\nStarting new loop\n";
while ((read = getline(&line, &len, fp)) != -1) {
     string line_str(line);
     cout<<line_str<<endl;
     parse_print_string(line, UnNeeded);
    }

showSet(UnNeeded, "UnNeeded");

int index = 0;
for(set<string>::iterator cit = CNF.begin(); cit != CNF.end(); cit++)
	{
        string clause = *cit;

	char char_index[100];
	sprintf(char_index, "%d", index);
	string string_index(char_index);

        cout<<"\nclause = "<<clause<<",string_index = "<<string_index<<endl;

	if(UnNeeded.find(string_index)==UnNeeded.end()) // This clause is not unneeded
		{
	        cout<<"\nIt is in CoreClauses\n";
		CoreClauses.insert(clause);
		}
	index++;
	}

showSet(CoreClauses, "CoreClauses");

fclose(fp);

}


// Given "node" which is an unsat bv-logic formula, this function returns the unsat core
// in "UnsatCoreSignals".
// This is achieved by bit-blasting + using a SAT solver which returns unsat core
// This is the first version of the function to achieve this
void blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignals(DAGNode *node, map<string, int> &MyWidthTable, set<string> &UnsatCoreSignals)
{
string filename = "unsat_core_bit_level.cnf";
string corefilename = "unsat_core_bit_level_result.txt";

cout<<"\nnode = "<<getDAGStr(node)<<endl;

DAGNode* BitBlastedDAG = getBitBlastedDAG(node);

cout<<"\nBitBlastedDAG = "<<getDAGStr(BitBlastedDAG)<<endl;

bool check_exactness_blasting = false;

if(check_exactness_blasting)
	{
	bool exact = checkForExactnessOfBlasting(node, MyWidthTable, BitBlastedDAG);
	if(exact)
		{
		cout<<"\nBlasting is exact\n";
		//exit(1);
		}
	else
		{
		cout<<"\nBlasting is not exact\n";
		exit(1);
		}
	}

map<DAGNode*, string> LabelTable;
map<string, DAGNode*> ReverseLabelTable;

getBirectionalLabelTable(BitBlastedDAG, LabelTable, ReverseLabelTable);
cout<<"\nLabel Table\n";
map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
     }
cout<<"\n";

cout<<"\nReverse Label Table\n";
map<string, DAGNode*>::iterator ReverseLabelTable_it = ReverseLabelTable.begin();
for(;ReverseLabelTable_it != ReverseLabelTable.end();ReverseLabelTable_it++)
     {
       cout<<endl<<(*ReverseLabelTable_it).first<<"\t"<<((*ReverseLabelTable_it).second)->Get_node_name();
     }
cout<<"\n";

set<string> CNF;
cout<<"\nCreating equisatisfiable CNF\n";
getCNF(BitBlastedDAG, LabelTable, CNF);
cout<<"\nCNF created. Writing into DIMACS\n";


FILE *fcnf;
string result;

if( (fcnf = fopen(filename.c_str(), "w")) == NULL)
	{
	cout<<"\nError! Cannot open file "<<filename<<" for writing\n";
	exit(1);
	}
  
int NumVars = LabelTable.size();
int NumClauses = CNF.size();
  
fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

set<string>::iterator CNF_it;
int temp=0;

for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
fclose(fcnf);

cout<<"\nblasted problem written to file "<<filename<<"\n\n";

//cout<<"\nPress any key to continue\n";

//char c = getchar();

string command = "zminimal ";
command += filename;
command += " > ";
command += corefilename;

struct timeval start_ms, finish_ms;
gettimeofday (&start_ms, NULL);

system(command.c_str());

gettimeofday (&finish_ms, NULL);
		
zminimal_duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
zminimal_duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;


// use zminimal in /home/ajith/Documents/SAT-Solvers/ZChaff/zchaff/


set<string> CoreClauses;
getCoreClauses(CNF, corefilename, CoreClauses);

set<string> SupportOfCoreClauses;
getSupportOfCoreClauses(CoreClauses, SupportOfCoreClauses);


set<string> BitLevelSupportOfCoreClauses;
getBitLevelSupportOfCoreClauses(SupportOfCoreClauses, ReverseLabelTable, BitLevelSupportOfCoreClauses);

getBitVectorLevelSupportOfCoreClauses(BitLevelSupportOfCoreClauses, UnsatCoreSignals);
}


// Get LabelTable and ReverseLabelTable which give unique labels to the nodes in the 
// dag and vice-versa for the propositional logic formula rooted at "node".
// Similar to the function getLabelTable
void getBirectionalLabelTable(DAGNode *node, map<DAGNode*, string> &LabelTable, map<string, DAGNode*> &ReverseLabelTable)
{

  unsigned long long int label_count = 1;
 
  bool reset_visited = dm.ResetVisitedFlagOfDAGNodes();

  if(reset_visited == false)
    {
      cout<<"\nCould not reset visited flag of all DAG nodes\n";
      exit(1);
    }

  getBirectionalLabelTableRecursively(node, label_count, LabelTable, ReverseLabelTable);
}

// Internal function called by getBirectionalLabelTable
void getBirectionalLabelTableRecursively(DAGNode *node, unsigned long long int &label_count, map<DAGNode*, string> &LabelTable, map<string, DAGNode*> &ReverseLabelTable)
{
  if(node->Get_visited_flag() == true)
    return;
  node->Set_visited_flag();
 
     char count[100];
     sprintf(count,"%llu",label_count);
     string Label = count;

     LabelTable.insert (pair<DAGNode*, string>(node, Label));
     ReverseLabelTable.insert (pair<string, DAGNode*>(Label, node));

     label_count++;

     list<DAGNode*>::iterator it = node->Get_outlist_start(); 
     for(unsigned int i=0; i<node->Get_outlist_size(); i++, it++)
		{
		getBirectionalLabelTableRecursively(*it, label_count, LabelTable, ReverseLabelTable);
		}
}

// Get the set of variables (support) in the given clause "Clause" 
void splitClause(string Clause, set<string> &Variables)
{
string Literal, Variable; 

int index;

index = Clause.find(" ");

while(index != string::npos)
	{
	Literal = Clause.substr(0, index);

	Variable = getVariableFromLiteral(Literal);
	Variables.insert(Variable);

	Clause = Clause.substr(index+1);
	index = Clause.find(" ");
	}
}

// Given -i, return i; given i, return -i
string negateLiteral(string Var)
{
string Neg_Var; 
int index;

index = Var.find("-");
if(index != string::npos)
	{
	Neg_Var = Var.substr(index+1);
	}
else
	{
	Neg_Var = "-";
	Neg_Var += Var;
	}

return Neg_Var;
}

// Get the variable from the literal 
// literal can be variable or -variable
string getVariableFromLiteral(string Literal)
{
if(Literal.find("-")!=string::npos) //negated literal
	{
	return negateLiteral(Literal);
	}
else //original literal
	{
	return Literal;
	}
}


// Get the support of the clauses in "CoreClauses" in set "SupportOfCoreClauses"
void getSupportOfCoreClauses(set<string> &CoreClauses, set<string> &SupportOfCoreClauses)
{
for(set<string>::iterator it=CoreClauses.begin(); it!=CoreClauses.end(); it++)
	{
	string clause = *it;
	splitClause(clause, SupportOfCoreClauses);
	}
//showSet(SupportOfCoreClauses, "SupportOfCoreClauses");
}



// Get the bit-level support of core clauses
void getBitLevelSupportOfCoreClauses(set<string> &SupportOfCoreClauses, map<string, DAGNode*> &ReverseLabelTable,  set<string> &BitLevelSupportOfCoreClauses)
{

  bool restrict_to_nodes_with_no_descendents_in_important_nodes = true;

  if(restrict_to_nodes_with_no_descendents_in_important_nodes)
    {
      set<DAGNode*> ImportantNodes;
      
      for(set<string>::iterator it = SupportOfCoreClauses.begin(); it != SupportOfCoreClauses.end(); it++)
	{
	  string signal = *it;
	  map<string, DAGNode*>::iterator rit = ReverseLabelTable.find(signal);
	  if(rit == ReverseLabelTable.end())
	    {
	      cout<<"\nNo entry for "<<signal<<" in ReverseLabelTable in getSupportOfCoreClauses in Eliminator.cpp\n";
	      exit(1);
	    }

	  DAGNode* node = rit->second;

	  cout<<endl<<"\nsignal = "<<signal<<"\tnode = "<<getDAGStr(node)<<endl;
	  
	  ImportantNodes.insert(node);
	}

      set<DAGNode*> RestrictedImportantNodes;

      for(set<DAGNode*>::iterator it = ImportantNodes.begin(); it != ImportantNodes.end(); it++)
	{
      
	  DAGNode* node = *it;

	  set<DAGNode*> Descendents;

	  getDescendents(node, Descendents);

	  set<DAGNode*> ImportantDescendents;

	  set_intersection(Descendents.begin(), Descendents.end(), ImportantNodes.begin(), ImportantNodes.end(), inserter(ImportantDescendents, ImportantDescendents.begin()));

	  if(ImportantDescendents.empty())// no descendents of this node are important
	    {
	      cout<<endl<<"\tnode = "<<getDAGStr(node)<<" is really important\n";

	      RestrictedImportantNodes.insert(node);
	    }
	}

      for(set<DAGNode*>::iterator it = RestrictedImportantNodes.begin(); it != RestrictedImportantNodes.end(); it++)
	{

	  DAGNode* node = *it;
	  
	  set<string> MySupport;
	  getDAGNames(node, MySupport);
	  set_union(BitLevelSupportOfCoreClauses.begin(), BitLevelSupportOfCoreClauses.end(), MySupport.begin(), MySupport.end(),inserter(BitLevelSupportOfCoreClauses, BitLevelSupportOfCoreClauses.begin()));
	}
    }// if ends here
  else
    {

      for(set<string>::iterator it = SupportOfCoreClauses.begin(); it != SupportOfCoreClauses.end(); it++)
	{
	  string signal = *it;
	  map<string, DAGNode*>::iterator rit = ReverseLabelTable.find(signal);
	  if(rit == ReverseLabelTable.end())
	    {
	      cout<<"\nNo entry for "<<signal<<" in ReverseLabelTable in getSupportOfCoreClauses in Eliminator.cpp\n";
	      exit(1);
	    }

	  DAGNode* node = rit->second;

	  Value* MyValue = (Value*)(node->getValue());
	  Type MyType = MyValue->get_node_type();
	  int MyDataType = MyType.data_type;

	  cout<<endl<<"\nsignal = "<<signal<<"\tnode = "<<getDAGStr(node)<<endl;


 


	  if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)  //if the dag value is variable
	    {
	      BitLevelSupportOfCoreClauses.insert(node->Get_node_name());
	    }
	  else if(node->Get_outlist_size() > 0 && node->Get_node_name()== "=") // atomic predicates which are equalities
	    {
	      set<string> MySupport;
	      getDAGNames(node, MySupport);
	      set_union(BitLevelSupportOfCoreClauses.begin(), BitLevelSupportOfCoreClauses.end(), MySupport.begin(), MySupport.end(),inserter(BitLevelSupportOfCoreClauses, BitLevelSupportOfCoreClauses.begin()));
	    }
   
	}
    } 

  showSet(BitLevelSupportOfCoreClauses, "BitLevelSupportOfCoreClauses");
}

// Get the bit-vector level support of core clauses
void getBitVectorLevelSupportOfCoreClauses(set<string> &BitLevelSupportOfCoreClauses, set<string> &UnsatCoreSignals)
{
for(set<string>::iterator it = BitLevelSupportOfCoreClauses.begin(); it != BitLevelSupportOfCoreClauses.end(); it++)
	{
	string signal = *it;
	int index = signal.find_last_of("_");
	if(index == string::npos)
		{
		cout<<"\nBit name encountered without _ in getBitVectorLevelSupportOfCoreClauses in Eliminator.cpp\n";
		exit(1);
		}
	signal = signal.substr(0, index);
	UnsatCoreSignals.insert(signal);
	}
showSet(UnsatCoreSignals, "UnsatCoreSignals");
}


// Same as blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignals
// except that the assignment part is written more efficiently
void blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvanced(DAGNode *node, map<string, int> &MyWidthTable, set<string> &OriginalBits, set<string> &NegatedBits, set<string> &UnsatCoreSignals)
{
string filename = "unsat_core_bit_level.cnf";
string corefilename = "unsat_core_bit_level_result.txt";

cout<<"\nnode = "<<getDAGStr(node)<<endl;

DAGNode* BitBlastedDAG = getBitBlastedDAG(node);

cout<<"\nBitBlastedDAG = "<<getDAGStr(BitBlastedDAG)<<endl;

bool check_exactness_blasting = false;

if(check_exactness_blasting)
	{
	bool exact = checkForExactnessOfBlasting(node, MyWidthTable, BitBlastedDAG);
	if(exact)
		{
		cout<<"\nBlasting is exact\n";
		//exit(1);
		}
	else
		{
		cout<<"\nBlasting is not exact\n";
		exit(1);
		}
	}

map<DAGNode*, string> LabelTable;
map<string, DAGNode*> ReverseLabelTable;

getBirectionalLabelTable(BitBlastedDAG, LabelTable, ReverseLabelTable);
cout<<"\nLabel Table\n";
map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
     }
cout<<"\n";

cout<<"\nReverse Label Table\n";
map<string, DAGNode*>::iterator ReverseLabelTable_it = ReverseLabelTable.begin();
for(;ReverseLabelTable_it != ReverseLabelTable.end();ReverseLabelTable_it++)
     {
       cout<<endl<<(*ReverseLabelTable_it).first<<"\t"<<((*ReverseLabelTable_it).second)->Get_node_name();
     }
cout<<"\n";

map<string, string> Bit_Index_Table;
getBit_Index_Table(LabelTable, Bit_Index_Table);
cout<<"\nBit_Index_Table\n";
map<string, string>::iterator Bit_Index_Table_it = Bit_Index_Table.begin();
for(;Bit_Index_Table_it != Bit_Index_Table.end(); Bit_Index_Table_it++)
     {
       cout<<endl<<(*Bit_Index_Table_it).first<<"\t"<<(*Bit_Index_Table_it).second;
     }
cout<<"\n";

map<string, string> Reverse_Bit_Index_Table;
getReverse_Bit_Index_Table(Bit_Index_Table, Reverse_Bit_Index_Table);
cout<<"\nReverse_Bit_Index_Table\n";
map<string, string>::iterator Reverse_Bit_Index_Table_it = Reverse_Bit_Index_Table.begin();
for(;Reverse_Bit_Index_Table_it != Reverse_Bit_Index_Table.end(); Reverse_Bit_Index_Table_it++)
     {
       cout<<endl<<(*Reverse_Bit_Index_Table_it).first<<"\t"<<(*Reverse_Bit_Index_Table_it).second;
     }
cout<<"\n";

set<string> CNF;
cout<<"\nCreating equisatisfiable CNF\n";
getCNF(BitBlastedDAG, LabelTable, CNF);
cout<<"\nFormula part added to CNF. Adding the assignment part\n";

attachAssignmentPartToCNF(CNF, OriginalBits, NegatedBits, Bit_Index_Table);

cout<<"\nCNF created. Writing into DIMACS\n";


FILE *fcnf;
string result;

if( (fcnf = fopen(filename.c_str(), "w")) == NULL)
	{
	cout<<"\nError! Cannot open file "<<filename<<" for writing\n";
	exit(1);
	}
  
int NumVars = LabelTable.size();
int NumClauses = CNF.size();
  
fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

set<string>::iterator CNF_it;
int temp=0;

for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
fclose(fcnf);

cout<<"\nblasted problem written to file "<<filename<<"\n\n";

//cout<<"\nPress any key to continue\n";

//char c = getchar();

string command = "zminimal ";
command += filename;
command += " > ";
command += corefilename;

struct timeval start_ms, finish_ms;
gettimeofday (&start_ms, NULL);


system(command.c_str());

gettimeofday (&finish_ms, NULL);
		
zminimal_duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
zminimal_duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;


// use zminimal in /home/ajith/Documents/SAT-Solvers/ZChaff/zchaff/

set<string> CoreClauses;
getCoreClauses(CNF, corefilename, CoreClauses);
showSet(CoreClauses, "CoreClauses");

set<string> BitLevelSupportOfCoreClauses;
getBitLevelSupportOfCoreClausesAdvanced(CoreClauses, Reverse_Bit_Index_Table, BitLevelSupportOfCoreClauses);

getBitVectorLevelSupportOfCoreClauses(BitLevelSupportOfCoreClauses, UnsatCoreSignals);
}


// Get bit-index table, which maps name of variable and bdd index
void getBit_Index_Table(map<DAGNode*, string> &LabelTable,  map<string, string> &Bit_Index_Table)
{

for(map<DAGNode*, string>::iterator it = LabelTable.begin(); it != LabelTable.end(); it++)
{
DAGNode* node = it->first;
string index = it->second;

Value* MyValue = (Value*)(node->getValue());
Type MyType = MyValue->get_node_type();
int MyDataType = MyType.data_type;

if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)  //if the dag value is variable
        {
	Bit_Index_Table.insert(make_pair(node->Get_node_name(), index));
	}
}// for

}// function


// Function to change the CNF by attaching the assignment part to it
void attachAssignmentPartToCNF(set<string> &CNF, set<string> &OriginalBits, set<string> &NegatedBits, map<string, string> &Bit_Index_Table)
{
for(set<string>::iterator it = OriginalBits.begin(); it != OriginalBits.end(); it++)
	{
	string bit = *it;
	map<string, string>::iterator Bit_Index_Table_it = Bit_Index_Table.find(bit);
	if(Bit_Index_Table_it == Bit_Index_Table.end())
		{
		cout<<"\nWarning! "<<bit<<" has no entry in Bit_Index_Table in attachAssignmentPartToCNF in BitBlaster.cc\n";
		continue;
		}
	string index = Bit_Index_Table_it->second;
	string clause = index;
	clause += " 0";
	CNF.insert(clause);
	}

for(set<string>::iterator it = NegatedBits.begin(); it != NegatedBits.end(); it++)
	{
	string bit = *it;
	map<string, string>::iterator Bit_Index_Table_it = Bit_Index_Table.find(bit);
	if(Bit_Index_Table_it == Bit_Index_Table.end())
		{
		cout<<"\nWarning! "<<bit<<" has no entry in Bit_Index_Table in attachAssignmentPartToCNF in BitBlaster.cc\n";
		continue;
		}
	string index = Bit_Index_Table_it->second;
	string clause = "-";
	clause += index;
	clause += " 0";
	CNF.insert(clause);
	}
	
}

// Get reverse bit-index table, which maps bdd index and variable name
void getReverse_Bit_Index_Table(map<string, string> &Bit_Index_Table, map<string, string> &Reverse_Bit_Index_Table)
{
for(map<string, string>::iterator it = Bit_Index_Table.begin(); it != Bit_Index_Table.end(); it++)
	{
	string bit = it->first;
	string index = it->second;
	Reverse_Bit_Index_Table.insert(make_pair(index, bit));
	}
}

// The latest version of getBitLevelSupportOfCoreClauses
void getBitLevelSupportOfCoreClausesAdvanced(set<string> &CoreClauses, map<string, string> &Reverse_Bit_Index_Table,  set<string> &BitLevelSupportOfCoreClauses)
{
for(set<string>::iterator it = CoreClauses.begin(); it != CoreClauses.end(); it++)
	{
	string clause = *it;
	set<string> variables;
	splitClause(clause, variables);
	if(variables.size()==1) // only one variable is there
		{
		set<string>::iterator vit = variables.begin();
		string index = *vit;

		cout<<"\nindex = "<<index<<endl;
		
		map<string, string>::iterator Reverse_Bit_Index_Table_it = Reverse_Bit_Index_Table.find(index);
		if(Reverse_Bit_Index_Table_it != Reverse_Bit_Index_Table.end())
			{	
			string bit = Reverse_Bit_Index_Table_it->second;
			cout<<"\nbit = "<<bit<<endl;
			BitLevelSupportOfCoreClauses.insert(bit);
			}//if
		}//if
	}//for
}//function


// Latest version of the function getCoreClauses
void getCoreClausesAdvanced(string corefilename, set<string> &CoreClauses)
{

 FILE *fp = fopen(corefilename.c_str(), "r");
 if (fp == NULL)
    {
      cout<<"\nError while opening file "<<corefilename<<" for reading\n";
      exit(1);
    }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  bool flag = false;
while ((read = getline(&line, &len, fp)) != -1) {
    string line_str(line);
    //cout<<line_str<<endl;
    if(line_str.find("Original")!=string::npos)
      {
    	flag = true;
      }
    else if(flag)
      {
       //cout<<"line_str = "<<line_str<<endl;
       string trimmed_line = trimLeadingSpaces(line);
       //cout<<"trimmed_line = "<<trimmed_line<<endl;
       CoreClauses.insert(trimmed_line);
       flag = false;
      }
    }

//showSet(CoreClauses, "CoreClauses");

fclose(fp);

}

// Trim leading spaces from the string char *line_char
string trimLeadingSpaces(char *line_char)
{
  int index=0;

  char word[1000];

  int word_index = 0;

  bool started = false;
  
  while(line_char[index]!='\0')
	  {
	    
	    char read = line_char[index];

	    if(read == ' ' && !started)
	      {
		index++;
	      }
	    else
	      {
		started = true;		

		word[word_index] = line_char[index];

		word_index++;

		index++;
	      }
	      
	    
	  }

 word[word_index] = '\0';

 string trimmed_line(word);

 return trimmed_line;

}

// Same as blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignals
// except that 
// 1) The assignment part is written more efficiently
// 2) run_till_fix_modified.pl is used instead of zminimal
// This is the final function to achieve this functionality
void blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficient(DAGNode *node, map<string, int> &MyWidthTable, set<string> &OriginalBits, set<string> &NegatedBits, set<string> &UnsatCoreSignals)
{
string filename = "unsat_core_bit_level.cnf";
string corefilename = "core.cnf";

cout<<"\nnode = "<<getDAGStr(node)<<endl;

DAGNode* BitBlastedDAG = getBitBlastedDAG(node);

//cout<<"\nBitBlastedDAG = "<<getDAGStr(BitBlastedDAG)<<endl;

bool check_exactness_blasting = false;

if(check_exactness_blasting)
	{
	bool exact = checkForExactnessOfBlasting(node, MyWidthTable, BitBlastedDAG);
	if(exact)
		{
		cout<<"\nBlasting is exact\n";
		//exit(1);
		}
	else
		{
		cout<<"\nBlasting is not exact\n";
		exit(1);
		}
	}

map<DAGNode*, string> LabelTable;
map<string, DAGNode*> ReverseLabelTable;

getBirectionalLabelTable(BitBlastedDAG, LabelTable, ReverseLabelTable);
cout<<"\nLabel Table\n";
map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
     }
cout<<"\n";

cout<<"\nReverse Label Table\n";
map<string, DAGNode*>::iterator ReverseLabelTable_it = ReverseLabelTable.begin();
for(;ReverseLabelTable_it != ReverseLabelTable.end();ReverseLabelTable_it++)
     {
       cout<<endl<<(*ReverseLabelTable_it).first<<"\t"<<((*ReverseLabelTable_it).second)->Get_node_name();
     }
cout<<"\n";

map<string, string> Bit_Index_Table;
getBit_Index_Table(LabelTable, Bit_Index_Table);
cout<<"\nBit_Index_Table\n";
map<string, string>::iterator Bit_Index_Table_it = Bit_Index_Table.begin();
for(;Bit_Index_Table_it != Bit_Index_Table.end(); Bit_Index_Table_it++)
     {
       cout<<endl<<(*Bit_Index_Table_it).first<<"\t"<<(*Bit_Index_Table_it).second;
     }
cout<<"\n";

map<string, string> Reverse_Bit_Index_Table;
getReverse_Bit_Index_Table(Bit_Index_Table, Reverse_Bit_Index_Table);
cout<<"\nReverse_Bit_Index_Table\n";
map<string, string>::iterator Reverse_Bit_Index_Table_it = Reverse_Bit_Index_Table.begin();
for(;Reverse_Bit_Index_Table_it != Reverse_Bit_Index_Table.end(); Reverse_Bit_Index_Table_it++)
     {
       cout<<endl<<(*Reverse_Bit_Index_Table_it).first<<"\t"<<(*Reverse_Bit_Index_Table_it).second;
     }
cout<<"\n";

set<string> CNF;
cout<<"\nCreating equisatisfiable CNF\n";
getCNF(BitBlastedDAG, LabelTable, CNF);
cout<<"\nFormula part added to CNF. Adding the assignment part\n";

attachAssignmentPartToCNF(CNF, OriginalBits, NegatedBits, Bit_Index_Table);

cout<<"\nCNF created. Writing into DIMACS\n";

//showSet(CNF, "CNF");

removeTrivialClauses(CNF);

cout<<"\nAfter removing trivial clauses\n";

//showSet(CNF, "CNF");

FILE *fcnf;
string result;

if( (fcnf = fopen(filename.c_str(), "w")) == NULL)
	{
	cout<<"\nError! Cannot open file "<<filename<<" for writing\n";
	exit(1);
	}
  
int NumVars = LabelTable.size();
int NumClauses = CNF.size();
  
fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

set<string>::iterator CNF_it;
int temp=0;

for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
fclose(fcnf);

cout<<"\nblasted problem written to file "<<filename<<"\n\n";

//cout<<"\nPress any key to continue\n";

//char c = getchar();

string command = "run_till_fix_modified.pl ";
command += filename;

struct timeval start_ms, finish_ms;
gettimeofday (&start_ms, NULL);


system(command.c_str());

gettimeofday (&finish_ms, NULL);
		
zminimal_duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
zminimal_duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;


// use run_till_fix_modified.pl in /home/ajith/Documents/SAT-Solvers/ZChaff/zchaff/

set<string> CoreClauses;
getCoreClausesAdvanced(corefilename, CoreClauses);
showSet(CoreClauses, "CoreClauses");

set<string> BitLevelSupportOfCoreClauses;
getBitLevelSupportOfCoreClausesAdvanced(CoreClauses, Reverse_Bit_Index_Table, BitLevelSupportOfCoreClauses);

getBitVectorLevelSupportOfCoreClauses(BitLevelSupportOfCoreClauses, UnsatCoreSignals);
}

// Remove trivial clauses from CNF
void removeTrivialClauses(set<string> &CNF)
{
  set<string> NewCNF;

  set<string>::iterator CNF_it;

for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      if(!trivialClause(*CNF_it))
	{
	  NewCNF.insert(*CNF_it);
	}
       else
	{
	cout<<"\nRemoving the trivial clause "<<(*CNF_it)<<endl;
	}
     }

 CNF = NewCNF; 
  
}


// Return true if "Clause" is trivial; false otherwise.
// A clause is trivial if a variable is present in both original and negated versions.
bool trivialClause(string Clause)
{
set<string> NegatedVariables;
set<string> OriginalVariables;

string Literal, Variable; 

int index;

index = Clause.find(" ");

while(index != string::npos)
	{
	Literal = Clause.substr(0, index);
	Variable = getVariableFromLiteral(Literal);

	if(Literal.find("-") != string::npos)// Literal is a NegatedVariable
	  {
	    NegatedVariables.insert(Variable);
	  }
	else // Literal is an OriginalVariable
	  {
	    OriginalVariables.insert(Variable);
	  }

	Clause = Clause.substr(index+1);
	index = Clause.find(" ");
	}


 set<string> CommonVariables;
 
 set_intersection(NegatedVariables.begin(), NegatedVariables.end(), OriginalVariables.begin(), OriginalVariables.end(), inserter(CommonVariables, CommonVariables.begin()));
 
 if(CommonVariables.empty())
   {
     return false;
   }
 return true;
}


// Functions added on 12-04-'11 to check sat. using QBF solvers

// return true if "DAG_to_blast" is unsat; return false otherwise.
// Satisfiability is checked by blasting followed by QBF solving.
bool checkSatUsingQBFSolver(DAGNode *DAG_to_blast, map<string, int> WidthTable)
{
  ofstream *outfile = new ofstream();
  outfile->open("blasted_DAGFile.txt");
  DAGNode* node = DAG_to_blast;
  DAGNode* BitBlastedDAG = getBitBlastedDAG(node);
  cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
  *outfile<<"\nBlasted Formula "<<endl;
  dm.printDAG(BitBlastedDAG, outfile);
  outfile->close();

  // Creating equisatisfiable CNF
  // Creating a table which has a unique label for each dag node

   map<DAGNode*, string> LabelTable;
   getLabelTable(BitBlastedDAG, LabelTable);
   cout<<"\nLabel Table\n";
   map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
     }
   cout<<"\n";	
 
   set<string> ExQuantifiedVars;
 
   LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
	{
	ExQuantifiedVars.insert((*LabelTable_it).second);
	}

   set<string> CNF;
   cout<<"\nCreating equisatisfiable CNF\n";
   getCNF(BitBlastedDAG, LabelTable, CNF);
   cout<<"\nCNF created. Calling QBF Solver\n";
   return checkQBFUnsat(CNF, LabelTable.size(), ExQuantifiedVars);
  
}

// Call QBF solver to solve \exists ExQuantifiedVars. CNF where CNF 
// represents the equisatisfiable CNF for some propositional logic formula
// return true if unsat; false if sat
bool checkQBFUnsat(set<string> &CNF, int NumVars, set<string> &ExQuantifiedVars)
{
  FILE *fcnf, *fres;
  string result;

  if( (fcnf = fopen("blasted.cnf", "w")) == NULL)
    cout<<"\nError! Cannot open file blasted.cnf for writing\n\n";
  
  int NumClauses = CNF.size();
  
  fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

  fprintf(fcnf, "e ");

  set<string>::iterator Vars_it;

  for(Vars_it = ExQuantifiedVars.begin(); Vars_it != ExQuantifiedVars.end(); Vars_it++)
    {
      fprintf(fcnf, "%s ", (*Vars_it).c_str());
    }

  fprintf(fcnf, "0\n");


  set<string>::iterator CNF_it;
  int temp=0;

  for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
  if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
  fclose(fcnf);

  system("QuBE7 blasted.cnf > blasted.txt");

  if( (fres = fopen("blasted.txt", "r")) == NULL)
    cout<<"\nError! Cannot open file blasted.txt for reading\n\n";

  char word[100], c;
  int i;

  for(i=0;i<7;i++) {c=fgetc(fres); word[i]=c;}
	
  word[i]='\0'; printf("\n%s",word);

  if(strcmp(word, "s cnf 0")==0) { cout<<"\nUnsat\n"; fclose(fres); return true;}
  else if(strcmp(word, "s cnf 1")==0) { cout<<"\nSat\n"; fclose(fres); return false;}
  else { cout<<"\nUnknown result from QBF Solver\n"; fclose(fres); cout<<"\nf' => exists(X, f) is unknown\n"; exit(1);}
}

// Functions added on 12-04-'11 to check sat. using QBF solvers end here

// Functions added on 19-12-'11 start here

// This function checks if 
// \exists RemainingVars \forall VarSetToElim.(\neg \phi)  is unsat
// "DAG_to_blast" here represents \phi 
// returns true if \exists RemainingVars \forall VarSetToElim.(\neg \phi)  is unsat ; false otherwise
bool checkQBFSolverEffectiveness(DAGNode *DAG_to_blast, set<string> &VarSetToElim, set<string> &VarSetRemaining, map<string, int> &WidthTable)
{
  ofstream *outfile = new ofstream();
  outfile->open("blasted_DAGFile.txt");
  DAGNode* node = createDAG("not",DAG_to_blast, LOGICAL_SYMBOL, 1);
  DAGNode* BitBlastedDAG = getBitBlastedDAG(node);
  cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
  *outfile<<"\nBlasted Formula "<<endl;
  dm.printDAG(BitBlastedDAG, outfile);
  outfile->close();

  bool debug_blasting = false;

  if(debug_blasting)
	{
  
  	if(!checkForExactnessOfBlasting(node, WidthTable, BitBlastedDAG))
		{
		cout<<"\nBit blasting not exact\n"; exit(1);
		}
  	else
		{
		cout<<"\nBit blasting exact\n"; 
		}
	}

  // Creating equisatisfiable CNF
  // Creating a table which has a unique label for each dag node

   map<DAGNode*, string> LabelTable;
   getLabelTable(BitBlastedDAG, LabelTable);
   cout<<"\nLabel Table\n";
   map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
     }
   cout<<"\n";	
 
   set<string> ExQuantifiedVars1;
   set<string> UniversallyQuantifiedVars;
   set<string> ExQuantifiedVars2;

   LabelTable_it = LabelTable.begin();
   for(;LabelTable_it != LabelTable.end();LabelTable_it++)
	{
	DAGNode *bit_node = (*LabelTable_it).first;
	string variable = bit_node->Get_node_name();
	
	Value* bit_node_value = (Value*)(node->getValue());
        Type bit_node_type = bit_node_value->get_node_type();
        int bit_node_width = bit_node_type.width;
        int bit_node_datatype = bit_node_type.data_type;

	if(bit_node_datatype == LOGICAL_CONSTANT || bit_node->Get_outlist_size() != 0) 
		{
		ExQuantifiedVars2.insert((*LabelTable_it).second);
		continue;
		}
	
	// we have a variable
        int index = variable.find_last_of("_");
	variable = variable.substr(0, index);

	if(VarSetToElim.find(variable) != VarSetToElim.end()) // this bit is from a variable to be eliminated. i.e. this bit is to be universally quantified
		{
                cout<<"\n"<<variable<<" is from a variable to be eliminated. i.e. this bit is to be universally quantified\n";
		UniversallyQuantifiedVars.insert((*LabelTable_it).second);
		}

	else if(VarSetRemaining.find(variable) != VarSetRemaining.end()) // this bit is from a variable remaining i.e. this bit is to be ex. quantified
		{
                cout<<"\n"<<variable<<" is from a variable to remain. i.e. this bit is to be ex. quantified\n";
		ExQuantifiedVars1.insert((*LabelTable_it).second);
		}
	else
	  {
	    cout<<"\nWarning!Cannot associate a quantifier with "<<variable<<" with datatype "<<bit_node_datatype<<" and width "<<bit_node_width<<"\n";
	    
	    if(variable == "true" || variable == "false")
	      ExQuantifiedVars2.insert((*LabelTable_it).second);
	    else
	      {
		exit(1);
	      }
	  }
	
	}

   set<string> CNF;
   cout<<"\nCreating equisatisfiable CNF\n";
   getCNF(BitBlastedDAG, LabelTable, CNF);
   cout<<"\nCNF created. Calling QBF Solver\n";
   return checkQBFUnsat(CNF, LabelTable.size(), ExQuantifiedVars1, UniversallyQuantifiedVars, ExQuantifiedVars2);  
}





//  Version of  blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficient
// for a given dag. There is no assignment part here. Also, unlike blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficient,
// this function finds the UnsatCoreSignals in a different way
void blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAG(DAGNode *node, map<string, int> &MyWidthTable, set<DAGNode*> &CoreConstraints)
{
string filename = "unsat_core_bit_level.cnf";
string corefilename = "core.cnf";

//Removing the previous files
system("rm unsat_core_bit_level.cnf");
system("rm unsat_core_bit_level.cnf_itr_0");
system("rm unsat_core_bit_level.cnf_itr_1"); 
system("rm core.cnf");

cout<<"\nnode = "<<getDAGStr(node)<<endl;

DAGNode* BitBlastedDAG = getBitBlastedDAG(node);

if(true)       {
                    ofstream *outfile = new ofstream();
                    //cout<<"\nOriginal DAG = "<<getDAGStr(root[0])<<endl;
                    outfile->open("blasted_DAGFile.txt");
                    cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
                     *outfile<<"\nBlasted Formula "<<endl;
                    dm.printDAG(BitBlastedDAG, outfile);
                    outfile->close();
                    //showMap(WidthTable);
                }

//cout<<"\nBitBlastedDAG = "<<getDAGStr(BitBlastedDAG)<<endl;

bool check_exactness_blasting = false;

if(check_exactness_blasting)
	{
	bool exact = checkForExactnessOfBlasting(node, MyWidthTable, BitBlastedDAG);
	if(exact)
		{
		cout<<"\nBlasting is exact\n";
		//exit(1);
		}
	else
		{
		cout<<"\nBlasting is not exact\n";
		exit(1);
		}
	}

map<DAGNode*, string> LabelTable;
map<string, DAGNode*> ReverseLabelTable;

getBirectionalLabelTable(BitBlastedDAG, LabelTable, ReverseLabelTable);
cout<<"\nLabel Table\n";
map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       //cout<<"\n"<<(*LabelTable_it).first<<"\t"<<(*LabelTable_it).second; 
     }
cout<<"\n";

cout<<"\nReverse Label Table\n";
map<string, DAGNode*>::iterator ReverseLabelTable_it = ReverseLabelTable.begin();
for(;ReverseLabelTable_it != ReverseLabelTable.end();ReverseLabelTable_it++)
     {
       //cout<<endl<<(*ReverseLabelTable_it).first<<"\t"<<(*ReverseLabelTable_it).second<<"\t"<<((*ReverseLabelTable_it).second)->Get_node_name();
     }
cout<<"\n";

map<string, string> Bit_Index_Table;
getBit_Index_Table(LabelTable, Bit_Index_Table);
cout<<"\nBit_Index_Table\n";
map<string, string>::iterator Bit_Index_Table_it = Bit_Index_Table.begin();
for(;Bit_Index_Table_it != Bit_Index_Table.end(); Bit_Index_Table_it++)
     {
       //cout<<endl<<(*Bit_Index_Table_it).first<<"\t"<<(*Bit_Index_Table_it).second;
     }
cout<<"\n";

map<string, string> Reverse_Bit_Index_Table;
getReverse_Bit_Index_Table(Bit_Index_Table, Reverse_Bit_Index_Table);
cout<<"\nReverse_Bit_Index_Table\n";
map<string, string>::iterator Reverse_Bit_Index_Table_it = Reverse_Bit_Index_Table.begin();
for(;Reverse_Bit_Index_Table_it != Reverse_Bit_Index_Table.end(); Reverse_Bit_Index_Table_it++)
     {
       //cout<<endl<<(*Reverse_Bit_Index_Table_it).first<<"\t"<<(*Reverse_Bit_Index_Table_it).second;
     }
cout<<"\n";

set<string> CNF;
cout<<"\nCreating equisatisfiable CNF\n";
getCNF(BitBlastedDAG, LabelTable, CNF);

if(true)
    {
    removeTrivialClauses(CNF);
    }

cout<<"\nEquisatisfiable CNF created. Writing into DIMACS\n";

//showSet(CNF, "CNF");

FILE *fcnf;
string result;

if( (fcnf = fopen(filename.c_str(), "w")) == NULL)
	{
	cout<<"\nError! Cannot open file "<<filename<<" for writing\n";
	exit(1);
	}
  
int NumVars = LabelTable.size();
int NumClauses = CNF.size();
  
fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

set<string>::iterator CNF_it;
int temp=0;

for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
fclose(fcnf);

cout<<"\nblasted problem written to file "<<filename<<"\n\n";

//cout<<"\nPress any key to continue\n";

//char c = getchar();

string command = "run_till_fix_modified.pl ";
command += filename;

struct timeval start_ms, finish_ms;
gettimeofday (&start_ms, NULL);


system(command.c_str());

gettimeofday (&finish_ms, NULL);
		
unsigned long long int duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;


// use run_till_fix_modified.pl in /home/ajith/Documents/SAT-Solvers/ZChaff/zchaff/


set<string> CoreClauses;
getCoreClausesAdvanced(corefilename, CoreClauses);
//showSet(CoreClauses, "CoreClauses");

set<string> SupportOfCoreClauses;
getSupportOfCoreClauses(CoreClauses, SupportOfCoreClauses);
//showSet(SupportOfCoreClauses, "SupportOfCoreClauses");

set<DAGNode*> BlastedDAGLevelSupportOfCoreClauses;
getBlastedDAGLevelSupportOfCoreClauses(SupportOfCoreClauses, ReverseLabelTable, BlastedDAGLevelSupportOfCoreClauses);
//showAPSet("BlastedDAGLevelSupportOfCoreClauses", BlastedDAGLevelSupportOfCoreClauses);

CoreConstraints.clear();
getOriginalDAGLevelSupportOfCoreClauses(BlastedDAGLevelSupportOfCoreClauses,  CoreConstraints);
showAPSet("CoreConstraints", CoreConstraints);

}

// Get the blasted dag level support of core clauses
void getBlastedDAGLevelSupportOfCoreClauses(set<string> &SupportOfCoreClauses, map<string, DAGNode*> &ReverseLabelTable,  set<DAGNode*> &BlastedDAGLevelSupportOfCoreClauses)
{
  for(set<string>::iterator it = SupportOfCoreClauses.begin(); it != SupportOfCoreClauses.end(); it++)
    {
      string signal = *it;
      map<string, DAGNode*>::iterator rit = ReverseLabelTable.find(signal);
      if(rit == ReverseLabelTable.end())
        {
          cout<<"\nNo entry for "<<signal<<" in ReverseLabelTable in getBlastedDAGLevelSupportOfCoreClauses in Eliminator.cpp\n";
          exit(1);
        }

      DAGNode* node = rit->second;
      //cout<<endl<<"\nsignal = "<<signal<<"\tnode = "<<getDAGStr(node)<<endl;
      BlastedDAGLevelSupportOfCoreClauses.insert(node);
    }
}



// Get the original dag level support of core clauses
void getOriginalDAGLevelSupportOfCoreClauses(set<DAGNode*> &BlastedDAGLevelSupportOfCoreClauses,  set<DAGNode*> &OriginalDAGLevelSupportOfCoreClauses)
{
  for(set<DAGNode*>::iterator it = BlastedDAGLevelSupportOfCoreClauses.begin(); it != BlastedDAGLevelSupportOfCoreClauses.end(); it++)
    {
      DAGNode* dag = *it;
      map<DAGNode*, DAGNode*>::iterator rit = BlastedDAGToOriginalDAGMapForPredicates.find(dag);
      if(rit != BlastedDAGToOriginalDAGMapForPredicates.end())
        {
          DAGNode* node = rit->second;
          OriginalDAGLevelSupportOfCoreClauses.insert(node);
        }
  }
}




//  Version of  blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAG
// The SAT solver used is picosat, which is more efficient that zchaff
void blastAndWriteInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficientForGivenDAGUsingPicoSAT(DAGNode *node, map<string, int> &MyWidthTable, set<DAGNode*> &CoreConstraints)
{
string filename = "unsat_core_bit_level.cnf";
string corefilename = "core_vars_pico.txt";

//Removing the previous files
system("rm unsat_core_bit_level.cnf");
system("rm core_vars_pico.txt");

cout<<"\nnode = "<<getDAGStr(node)<<endl;

DAGNode* BitBlastedDAG = getBitBlastedDAG(node);

if(true)       {
                    ofstream *outfile = new ofstream();
                    //cout<<"\nOriginal DAG = "<<getDAGStr(root[0])<<endl;
                    outfile->open("blasted_DAGFile.txt");
                    cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
                     *outfile<<"\nBlasted Formula "<<endl;
                    dm.printDAG(BitBlastedDAG, outfile);
                    outfile->close();
                    //showMap(WidthTable);
                }

//cout<<"\nBitBlastedDAG = "<<getDAGStr(BitBlastedDAG)<<endl;

bool check_exactness_blasting = false;

if(check_exactness_blasting)
	{
	bool exact = checkForExactnessOfBlasting(node, MyWidthTable, BitBlastedDAG);
	if(exact)
		{
		cout<<"\nBlasting is exact\n";
		//exit(1);
		}
	else
		{
		cout<<"\nBlasting is not exact\n";
		exit(1);
		}
	}

map<DAGNode*, string> LabelTable;
map<string, DAGNode*> ReverseLabelTable;

getBirectionalLabelTable(BitBlastedDAG, LabelTable, ReverseLabelTable);
cout<<"\nLabel Table\n";
map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
for(;LabelTable_it != LabelTable.end();LabelTable_it++)
     {
       if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
       //cout<<"\n"<<(*LabelTable_it).first<<"\t"<<(*LabelTable_it).second; 
     }
cout<<"\n";

cout<<"\nReverse Label Table\n";
map<string, DAGNode*>::iterator ReverseLabelTable_it = ReverseLabelTable.begin();
for(;ReverseLabelTable_it != ReverseLabelTable.end();ReverseLabelTable_it++)
     {
       //cout<<endl<<(*ReverseLabelTable_it).first<<"\t"<<(*ReverseLabelTable_it).second<<"\t"<<((*ReverseLabelTable_it).second)->Get_node_name();
     }
cout<<"\n";

map<string, string> Bit_Index_Table;
getBit_Index_Table(LabelTable, Bit_Index_Table);
cout<<"\nBit_Index_Table\n";
map<string, string>::iterator Bit_Index_Table_it = Bit_Index_Table.begin();
for(;Bit_Index_Table_it != Bit_Index_Table.end(); Bit_Index_Table_it++)
     {
       //cout<<endl<<(*Bit_Index_Table_it).first<<"\t"<<(*Bit_Index_Table_it).second;
     }
cout<<"\n";

map<string, string> Reverse_Bit_Index_Table;
getReverse_Bit_Index_Table(Bit_Index_Table, Reverse_Bit_Index_Table);
cout<<"\nReverse_Bit_Index_Table\n";
map<string, string>::iterator Reverse_Bit_Index_Table_it = Reverse_Bit_Index_Table.begin();
for(;Reverse_Bit_Index_Table_it != Reverse_Bit_Index_Table.end(); Reverse_Bit_Index_Table_it++)
     {
       //cout<<endl<<(*Reverse_Bit_Index_Table_it).first<<"\t"<<(*Reverse_Bit_Index_Table_it).second;
     }
cout<<"\n";

set<string> CNF;
cout<<"\nCreating equisatisfiable CNF\n";
getCNF(BitBlastedDAG, LabelTable, CNF);

if(true)
    {
    removeTrivialClauses(CNF);
    }

cout<<"\nEquisatisfiable CNF created. Writing into DIMACS\n";

//showSet(CNF, "CNF");

FILE *fcnf;
string result;

if( (fcnf = fopen(filename.c_str(), "w")) == NULL)
	{
	cout<<"\nError! Cannot open file "<<filename<<" for writing\n";
	exit(1);
	}
  
int NumVars = LabelTable.size();
int NumClauses = CNF.size();
  
fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);

set<string>::iterator CNF_it;
int temp=0;

for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
    {
      fprintf(fcnf, "%s\n", (*CNF_it).c_str());
      temp++;
    }
  
if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}
  
fclose(fcnf);

cout<<"\nblasted problem written to file "<<filename<<"\n\n";

//cout<<"\nPress any key to continue\n";

//char c = getchar();

string command = "picosat -V ";
command += corefilename;
command += " ";
command += filename;

struct timeval start_ms, finish_ms;
gettimeofday (&start_ms, NULL);


system(command.c_str());

gettimeofday (&finish_ms, NULL);
		
unsigned long long int duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;



set<string> SupportOfCoreClauses;
getSupportOfCoreClausesFromPicoSAT(corefilename, SupportOfCoreClauses);
//showSet(SupportOfCoreClauses, "SupportOfCoreClauses");

set<DAGNode*> BlastedDAGLevelSupportOfCoreClauses;
getBlastedDAGLevelSupportOfCoreClauses(SupportOfCoreClauses, ReverseLabelTable, BlastedDAGLevelSupportOfCoreClauses);
//showAPSet("BlastedDAGLevelSupportOfCoreClauses", BlastedDAGLevelSupportOfCoreClauses);

CoreConstraints.clear();
getOriginalDAGLevelSupportOfCoreClauses(BlastedDAGLevelSupportOfCoreClauses,  CoreConstraints);
//showAPSet("CoreConstraints", CoreConstraints);

}


// corefilename is teh list of variables used in the unsat core generated by picosat
// Just read it, and put it in SupportOfCoreClauses
void getSupportOfCoreClausesFromPicoSAT(string corefilename, set<string> &SupportOfCoreClauses)
{
 FILE *fp = fopen(corefilename.c_str(), "r");
 if (fp == NULL)
    {
      cout<<"\nError while opening file "<<corefilename<<" for reading\n";
      exit(1);
    }

  char word[100];
  int read;
  
  while ((read = fscanf(fp, "%s", word)) != -1) {
        string word_str(word);
        SupportOfCoreClauses.insert(word_str);       
      }

//showSet(SupportOfCoreClauses, "CoreClauses");

fclose(fp);
}



// Bit-blast the given dag and print the dag in DIMACS format in the given file
bool blastAndOutputInGivenFile(DAGNode *DAG_to_blast, map<string, int> &WidthTable, string blasted_output_file)
{
  //cout<<"\nBit-blasting the formula"<<endl;  
  cout<<"\nSize of original dag = "<<dm.getSizeOfDAG(DAG_to_blast)<<endl;
  
  struct timeval step_start_ms, step_finish_ms;    
  unsigned long long int step_time;  
  gettimeofday (&step_start_ms, NULL);     
    
  DAGNode* BitBlastedDAG = getBitBlastedDAG(DAG_to_blast);
  
  gettimeofday (&step_finish_ms, NULL);
  step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
  step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
  printf("\nblast time = %llu milli seconds\n", step_time);
  
  
  cout<<"\nSize of blasted dag = "<<dm.getSizeOfDAG(BitBlastedDAG)<<endl;
  
  if(false)
    {
    set<string> Support;
    getDAGNames(BitBlastedDAG, Support);
    cout<<"\nSize of support of blasted dag = "<<Support.size()<<endl;
    }
  
  bool debug = false;  
  if(debug)
    {
     ofstream *outfile = new ofstream();
     outfile->open("blasted_DAGFile.txt");
          
     *outfile<<"\nblasted formula\n";
     dm.printDAG(BitBlastedDAG, outfile);
     
     outfile->close();
    }
  
  bool check_exactness = false;
  if(check_exactness)
    {
      if(checkForExactnessOfBlasting(DAG_to_blast, WidthTable, BitBlastedDAG))
      {
          cout<<"\nBlasting is exact\n";
      }
      else
      {
           cout<<"\nBlasting is inexact\n";
      }
    }
  
  bool create_equisat_cnf = true;
  
  if(create_equisat_cnf)
      {
      // Creating equisatisfiable CNF
      // Creating a table which has a unique label for each dag node

       map<DAGNode*, string> LabelTable;
       getLabelTable(BitBlastedDAG, LabelTable);

       cout<<"\nLabel Table\n";

       map<DAGNode*, string>::iterator LabelTable_it = LabelTable.begin();
       for(;LabelTable_it != LabelTable.end();LabelTable_it++)
         {
           if((*LabelTable_it).second == "") {cout<<"\nUnassigned label in label table\n"; exit(1);}
           if((*LabelTable_it).second == " ") {cout<<"\nUnassigned label in label table\n"; exit(1);}
           cout<<"\n"<<(*LabelTable_it).first<<"\t"<<((*LabelTable_it).first)->Get_node_name()<<"\t"<<(*LabelTable_it).second; 
         }
       cout<<"\n";	

       set<string> CNF;
       cout<<"\nCreating equisatisfiable CNF\n";
       getCNF(BitBlastedDAG, LabelTable, CNF);
       cout<<"\nCNF created. Writing in given file\n";

       FILE* fcnf;

       if( (fcnf = fopen(blasted_output_file.c_str(), "w")) == NULL)
        {
        cout<<"\nError! Cannot open file "<<blasted_output_file<<" for writing\n";
        }
       
     
      if(blast_and_output && independent_variables.size() != 0)  // This is needed for Kuldeep for printing independent labels
      {
          set<string> LabelsOfIndependentVariables;
          findLabelsOfIndependentVariables(LabelTable, independent_variables, LabelsOfIndependentVariables);
          
          // print the labels
          int label_count = 0;
          int labels_per_line = 10;          
          
          for(set<string>::iterator lit = LabelsOfIndependentVariables.begin(); lit != LabelsOfIndependentVariables.end(); lit++)
          {
               string LabelOfVariable = *lit;
               
               if(label_count == 0)
               {
                   fprintf(fcnf, "c ind %s", LabelOfVariable.c_str());
               }
               else if(label_count % labels_per_line == 0)
               {
                   fprintf(fcnf, " 0\nc ind %s", LabelOfVariable.c_str());
               }
               else
               {
                    fprintf(fcnf, " %s", LabelOfVariable.c_str());
               }
               
               label_count++;
          }
          
         fprintf(fcnf, " 0\n"); 
      }
       

      int NumVars = LabelTable.size();
      int NumClauses = CNF.size();

      fprintf(fcnf, "p cnf %d %d\n", NumVars, NumClauses);


      set<string>::iterator CNF_it;
      int temp=0;

      for(CNF_it = CNF.begin(); CNF_it != CNF.end(); CNF_it++)
        {
          fprintf(fcnf, "%s\n", (*CNF_it).c_str());
          temp++;
        }

      if(temp!=NumClauses) {cout<<"\nNumClauses!=AddedClauses\n"; exit(1);}

      fclose(fcnf);

      cout<<"\nCNF written in "<<blasted_output_file<<endl;
    }
}


// returns true if the bvmul appears as constant * variable, where constant is the 
// maximum constant with the given width; returns false otherwise
bool ItIsMinusVariable(DAGNode* node)
{
      if(node->Get_node_name() != "bvmul")
      {
          cout<<"\nFunction ItIsMinusVariable in BitBlaster.cc called on a node other than bvmul\n";
          exit(1);
      }
    
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      DAGNode* Child1 = *it;
      it++;
      DAGNode* Child2 = *it;
      
      DAGNode* Variable;
      DAGNode* Constant;

      if(getNodeType(Child1).data_type == BITVECTOR_CONSTANT)
        {
          Constant = Child1;
          Variable = Child2;
        }
      else  if(getNodeType(Child2).data_type == BITVECTOR_CONSTANT)
        {
          Constant = Child2;
          Variable = Child1; 
        }
      else
        {
          cout<<"\nIn function ItIsMinusVariable in BitBlaster.cc. bvmul applied on operands both of which are non-constants\n";
          exit(1);
        } 
      
      unsigned long long int ConstantNumber = strtoull( (Constant->Get_node_name()).c_str(), NULL, 0);
      int WidthOfVariable = (getNodeType(Variable)).width;
      unsigned long long int MaximumPossible = findPower(WidthOfVariable) - 1;
      
      if(ConstantNumber == MaximumPossible)
      {
          return true;
      }
      
      return false;
}


// Internal function called by the function getBitBlastedDAG in BitBlaster.cc
// Given a bit-vector formula rooted at "node", returns the bits of the propositional
// logic formual obtained by blasting "node", as a vector<DAGNode *>
// This implements bit-blasting similar to the way bit-blasting is implemented in STP
const vector<DAGNode *> BBForm(DAGNode * node, map<DAGNode*, vector<DAGNode*> > &BBFormMem)
{ 
  struct timeval step_start_ms, step_finish_ms;    
  unsigned long long int step_time;  
  struct timeval compute_start_ms, compute_finish_ms;    
  unsigned long long int compute_time;     
  
  //fprintf(LMIQEOutputDataFp, "node = %s\n", getDAGStr(node).c_str());
  
  map<DAGNode*, vector<DAGNode*> >::iterator memit = BBFormMem.find(node);  
  if(memit != BBFormMem.end())
  {
    //fprintf(LMIQEOutputDataFp, "Hit\n");
    return memit->second;
  }
  else
  {
  //fprintf(LMIQEOutputDataFp, "Miss\n");    
  vector<DAGNode *> Bits;
  
  if(node->Get_outlist_size()==0) // leaf level
    {
      string MyName = node->Get_node_name();
      Value* MyValue = (Value*)(node->getValue());
      Type MyType = MyValue->get_node_type();
      int MyWidth = MyType.width;
      int MyDataType = MyType.data_type;


      if(MyDataType == LOGICAL_CONSTANT)
	    {
	      //cout<<"\nI am "<<MyName<<" with DataType = "<<MyDataType<<" and with width = "<<MyWidth<<"\n";
	      Bits.push_back(node);	       
            }//logical constant
      else if(MyDataType == BITVECTOR_SYMBOL)
	    {
              if(PrintBlastTimes)
                {
                    gettimeofday (&step_start_ms, NULL);        
                }
	      //cout<<"\nI am "<<MyName<<" with DataType = "<<MyDataType<<" and with width = "<<MyWidth<<"\n";

	      for(int index=0; index<MyWidth; index++)
		{		 
                  // get name of the bit
		  string Bit_Name = MyName;
		  Bit_Name += "_";
		  char index_char[100];
		  sprintf(index_char, "%d", index);
		  string index_str = index_char;
		  Bit_Name += index_str;

                  // create the bit and push back
		  DAGNode *bit_node = createLogicalVariable(Bit_Name);
		  Bits.push_back(bit_node);
		}
              
              if(PrintBlastTimes)
                {
                    gettimeofday (&step_finish_ms, NULL);
                    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time blast symbol = %llu\t", step_time);
                }
	       
	   }//bit-vector symbol
      else if(MyDataType == BITVECTOR_CONSTANT)
	{
	  //cout<<"\nI am "<<MyName<<" with DataType = "<<MyDataType<<" and with width = "<<MyWidth<<"\n";
          
          if(PrintBlastTimes)
                {
                    gettimeofday (&step_start_ms, NULL);        
                }

	  unsigned long long int number=strtoull(MyName.c_str(), NULL, 0);
	  //cout<<"\nnumber = "<<number<<endl;
	  MyName = integerToBinaryStringWithZeroPadding(number, MyWidth);
	  //cout<<"\nMyName = "<<MyName<<endl;
	  for(int index=MyWidth-1; index>=0; index--)
                {
                    // get name of the bit
                    string Bit_Name;
                    if(MyName[index]=='0')
                        Bit_Name="false";
                    else
                        Bit_Name="true";

                    DAGNode *bit_node = createLogicalConstant(Bit_Name);
                    Bits.push_back(bit_node);
                }
          
          if(PrintBlastTimes)
                {
                    gettimeofday (&step_finish_ms, NULL);
                    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time blast constant = %llu\t", step_time);
                }
      }//bit-vector constant
    }//leaf level  
  else //non-leaf level
  {
  string MyName = node->Get_node_name();

  if(MyName == "=")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      const vector<DAGNode *>& BitsChild1Orig = BBForm(*it, BBFormMem);
      it++;
      const vector<DAGNode *>& BitsChild2Orig = BBForm(*it, BBFormMem);    
      
      int child1_width = BitsChild1Orig.size();
      int child2_width = BitsChild2Orig.size();
      
      const vector<DAGNode *>& BitsChild1 = child1_width < child2_width ? zeroPaddingWithRet(BitsChild1Orig, child2_width) : BitsChild1Orig;
      const vector<DAGNode *>& BitsChild2 = child2_width < child1_width ? zeroPaddingWithRet(BitsChild2Orig, child1_width) : BitsChild2Orig; 
             
      if(BitsChild1.size() == 1)
        {
            DAGNode* equal_node = createDAG("=", BitsChild1[0], BitsChild2[0], LOGICAL_SYMBOL, 1);
            
            Bits.push_back(equal_node);
            BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(equal_node, node)); // node is blasted to equal_node
        }
      else
        { 
            DAGNode* equal_node = createDAG("=", BitsChild1[0], BitsChild2[0], LOGICAL_SYMBOL, 1);
            DAGNode* and_node = equal_node;
                    
            for(int index=1; index<BitsChild1.size(); index++)            
                {            
                equal_node = createDAG("=", BitsChild1[index], BitsChild2[index], LOGICAL_SYMBOL, 1);
                and_node = createDAG("and", and_node, equal_node, LOGICAL_SYMBOL, 1);
                }
            
            Bits.push_back(and_node);
            BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(and_node, node)); // node is blasted to and_node
        }       
    }//= ends here
  else if(MyName == "if_then_else")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      const vector<DAGNode *>& BitsChild1 = BBForm(*it, BBFormMem);
      it++;
      const vector<DAGNode *>& BitsChild2 = BBForm(*it, BBFormMem);
      it++;
      const vector<DAGNode *>& BitsChild3 = BBForm(*it, BBFormMem);
      
      if(BitsChild1.size() != 1 || BitsChild2.size() != 1 || BitsChild3.size() != 1)
	{
	  cout<<"\nWidth mismatch inside function BBForm for if_then_else\n";
	  exit(1);
	}
      
      DAGNode* ite_node = createDAG("if_then_else", BitsChild1[0], BitsChild2[0], BitsChild3[0], LOGICAL_SYMBOL, 1);
      Bits.push_back(ite_node);       
    }//if_then_else ends here
  else if(MyName == "ite")// then and else of ite are terms, not formulae
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      const vector<DAGNode *>& BitsChild1 = BBForm(*it, BBFormMem);
      it++;
      const vector<DAGNode *>& BitsChild2Orig = BBForm(*it, BBFormMem);
      it++;
      const vector<DAGNode *>& BitsChild3Orig = BBForm(*it, BBFormMem);
      
      if(BitsChild1.size() != 1)
	{
	  cout<<"\nWidth mismatch inside function BBForm for ite; type of first argument of ite is not boolean\n";
	  exit(1);
	}

      int child2_width = BitsChild2Orig.size();
      int child3_width = BitsChild3Orig.size();
      
      const vector<DAGNode *>& BitsChild2 = child2_width < child3_width ? zeroPaddingWithRet(BitsChild2Orig, child3_width) : BitsChild2Orig;
      const vector<DAGNode *>& BitsChild3 = child3_width < child2_width ? zeroPaddingWithRet(BitsChild3Orig, child2_width) : BitsChild3Orig; 

      for(int index=0; index < BitsChild2.size(); index++)
	{
	      DAGNode* ite_node = createDAG("if_then_else", BitsChild1[0], BitsChild2[index], BitsChild3[index], LOGICAL_SYMBOL, 1);
	      Bits.push_back(ite_node);
	}    
    }//ite ends here
  else if(MyName == "and")
    {
    if(node->Get_outlist_size()==2)
	{
         list<DAGNode*>::iterator it = node->Get_outlist_start();
         const vector<DAGNode *>& BitsChild1 = BBForm(*it, BBFormMem);
         it++;
         const vector<DAGNode *>& BitsChild2 = BBForm(*it, BBFormMem);
      
         if(BitsChild1.size() != 1 || BitsChild2.size() != 1)
            {
            cout<<"\nWidth mismatch inside function BBForm for and\n";
            exit(1);
            }
            
         DAGNode* and_node = createDAG("and", BitsChild1[0], BitsChild2[0], LOGICAL_SYMBOL, 1);
         Bits.push_back(and_node);       
        }   
      else if(node->Get_outlist_size() > 2)
        {          
         list<DAGNode*>::iterator it = node->Get_outlist_start();
         const vector<DAGNode *>& BitsChild = BBForm(*it, BBFormMem);
         it++;
         
         if(BitsChild.size() != 1)
            {
            cout<<"\nWidth mismatch inside function BBForm for and\n";
            exit(1);
            }
                     
         DAGNode* and_node = BitsChild[0];
        
         for(unsigned int child_it=1; child_it<node->Get_outlist_size(); child_it++, it++)
            {
            const vector<DAGNode *>& BitsChild_i= BBForm(*it, BBFormMem);
            
            if(BitsChild_i.size() != 1)
                {
                cout<<"\nWidth mismatch inside function BBForm for and\n";
                exit(1);
                }
            
            and_node = createDAG("and", and_node, BitsChild_i[0], LOGICAL_SYMBOL, 1);
            }         
          
	  Bits.push_back(and_node);
      }
      else
	{
	  cout<<"\nnumber of children of and is "<<node->Get_outlist_size()<<". Less than 2 of "<<getDAGStr(node)<<"\n";
	  exit(1);
	}
  }//and ends here
  else if(MyName == "or")
    {
    if(node->Get_outlist_size()==2)
	{
         list<DAGNode*>::iterator it = node->Get_outlist_start();
         const vector<DAGNode *>& BitsChild1 = BBForm(*it, BBFormMem);
         it++;
         const vector<DAGNode *>& BitsChild2 = BBForm(*it, BBFormMem);
      
         if(BitsChild1.size() != 1 || BitsChild2.size() != 1)
            {
            cout<<"\nWidth mismatch inside function BBForm for or\n";
            exit(1);
            }
            
         DAGNode* or_node = createDAG("or", BitsChild1[0], BitsChild2[0], LOGICAL_SYMBOL, 1);
         Bits.push_back(or_node);       
        }   
      else if(node->Get_outlist_size() > 2)
        {          
         list<DAGNode*>::iterator it = node->Get_outlist_start();
         const vector<DAGNode *>& BitsChild = BBForm(*it, BBFormMem);
         it++;
         
         if(BitsChild.size() != 1)
            {
            cout<<"\nWidth mismatch inside function BBForm for or\n";
            exit(1);
            }
                     
         DAGNode* or_node = BitsChild[0];
        
         for(unsigned int child_it=1; child_it<node->Get_outlist_size(); child_it++, it++)
            {
            const vector<DAGNode *>&  BitsChild_i = BBForm(*it, BBFormMem);
            
            if(BitsChild_i.size() != 1)
                {
                cout<<"\nWidth mismatch inside function BBForm for or\n";
                exit(1);
                }
            
            or_node = createDAG("or", or_node, BitsChild_i[0], LOGICAL_SYMBOL, 1);
            }         
          
	  Bits.push_back(or_node);
      }
      else
	{
	  cout<<"\nnumber of children of or is "<<node->Get_outlist_size()<<". Less than 2 of "<<getDAGStr(node)<<"\n";
	  exit(1);
	}
  }//or ends here
  else if(MyName == "not")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      const vector<DAGNode *>& BitsChild = BBForm(*it, BBFormMem);
               
      if(BitsChild.size() != 1)
            {
            cout<<"\nWidth mismatch inside function BBForm for not\n";
            exit(1);
            }
      
      DAGNode* not_node = createDAG("not", BitsChild[0], LOGICAL_SYMBOL, 1);
      Bits.push_back(not_node);      
    }//not ends here
   else if(MyName == "bvadd")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> BitsChild1Orig  = BBForm(*it, BBFormMem);
      it++;
      const vector<DAGNode *>& BitsChild2Orig = BBForm(*it, BBFormMem);
      
      int child1_width = BitsChild1Orig.size();
      int child2_width = BitsChild2Orig.size();
      
      vector<DAGNode *> tmp_res = child1_width < child2_width ? zeroPaddingWithRet(BitsChild1Orig, child2_width) : BitsChild1Orig;
      const vector<DAGNode *>& tmp = child2_width < child1_width ? zeroPaddingWithRet(BitsChild2Orig, child1_width) : BitsChild2Orig; 
             
      
       if(PrintBlastTimes)
         {
         gettimeofday (&step_start_ms, NULL);        
         }
      
      BBPlus(tmp_res, tmp, createLogicalConstant("false"));
      Bits = tmp_res;
      
      if(PrintBlastTimes)
       {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
        fprintf(LMIQEOutputDataFp, "time blast bvadd given blasted children = %llu\t", step_time);
       }
   }//bvadd ends here
   else  if(MyName == "bvmul")
   {
       bool ApplyEfficientProcessingForMinusVariable = true;
          
          if(ApplyEfficientProcessingForMinusVariable && ItIsMinusVariable(node))//check if the bvmul appears as constant * variable, where constant is the 
              // maximum constant with the given width. 
              // In this case, constant * variable is effectively -variable i.e. ~variable + 1
          {    
              //cout<<"\nInside special processing\n";
              
              list<DAGNode*>::iterator it = node->Get_outlist_start();
              DAGNode* Child1 = *it;
              it++;
              DAGNode* Child2 = *it;
              
              if(getNodeType(Child1).data_type != BITVECTOR_CONSTANT && getNodeType(Child2).data_type != BITVECTOR_CONSTANT)
                  {
                  cout<<"\nbvmul applied on operands both of which are non-constant\n";
                  exit(1);
                  }  
              
              const vector<DAGNode *>& Variable = ( getNodeType(Child1).data_type == BITVECTOR_CONSTANT ) ? BBForm(Child2, BBFormMem) : BBForm(Child1, BBFormMem); 
                           
               if(PrintBlastTimes)
                {
                 gettimeofday (&step_start_ms, NULL);        
                }          
           
            const vector<DAGNode *>& NegatedVariable = BBNeg(Variable);
            Bits = BBInc(NegatedVariable);
            
            if(PrintBlastTimes)
                {
                    gettimeofday (&step_finish_ms, NULL);
                    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                    fprintf(LMIQEOutputDataFp, "time blast bvmul given blasted children = %llu\t", step_time);
                } 
              
          }//if(max.constant * x)
          else
          {     
              //cout<<"\nInside normal processing\n";
           
              list<DAGNode*>::iterator it = node->Get_outlist_start();
              DAGNode* child1 = *it;
              const vector<DAGNode *> &BitsChild1Orig = BBForm(*it, BBFormMem);
              it++;
              DAGNode* child2 = *it;
              const vector<DAGNode *> &BitsChild2Orig = BBForm(*it, BBFormMem);
              
              int child1_width = BitsChild1Orig.size();
              int child2_width = BitsChild2Orig.size();
      
             const vector<DAGNode *>& mpcd1 = child1_width < child2_width ? zeroPaddingWithRet(BitsChild1Orig, child2_width) : BitsChild1Orig;
             const vector<DAGNode *>& mpcd2 = child2_width < child1_width ? zeroPaddingWithRet(BitsChild2Orig, child1_width) : BitsChild2Orig; 

               if(PrintBlastTimes)
                 {
                 gettimeofday (&step_start_ms, NULL);        
                 }              

              Bits = BBMult(mpcd1, mpcd2, child1, child2);

              if(PrintBlastTimes)
               {
                gettimeofday (&step_finish_ms, NULL);
                step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                fprintf(LMIQEOutputDataFp, "time blast bvmul given blasted children = %llu\t", step_time);
               }
          }//else of if(max.constant * x)
   }//bvmul ends here
   else if(MyName == "is_not_equal")
    {
      DAGNode *NegatedNode = negateConstraint(node);
      const vector<DAGNode *>& BitsNegatedNode = BBForm(NegatedNode, BBFormMem);
      
      DAGNode* not_node = createDAG("not", BitsNegatedNode[0], LOGICAL_SYMBOL, 1);
      Bits.push_back(not_node); 
      
      BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(not_node, node)); // node is blasted to not_node      
    }//is_not_equal ends here
   else if(MyName == "select")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      const vector<DAGNode *>& BitsChild1 = BBForm(*it, BBFormMem);
      it++;
      DAGNode *low_sel = *it;
      it++;
      DAGNode *high_sel = *it;

      string low_sel_string = low_sel->Get_node_name();
      string high_sel_string = high_sel->Get_node_name();

      int low_sel_number = atoi(low_sel_string.c_str());
      int high_sel_number = atoi(high_sel_string.c_str());

      for(int index=0; index<BitsChild1.size(); index++)
	{
	  if(index>=low_sel_number && index<=high_sel_number) // within selection
	    {
	      Bits.push_back(BitsChild1[index]);
	    }
	}
    }//select ends here
  else if(MyName == "concat")
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      const vector<DAGNode *>& BitsChild1 = BBForm(*it, BBFormMem);
      it++;
      const vector<DAGNode *>& BitsChild2 = BBForm(*it, BBFormMem);

      for(int index=0; index < BitsChild1.size(); index++)
	{
	      Bits.push_back(BitsChild1[index]);
	}
      for(int index=0; index < BitsChild2.size(); index++)
	{
	      Bits.push_back(BitsChild2[index]);
	}
     }//concat ends here
      else if(MyName == "bvule")
      {
          list<DAGNode*>::iterator it = node->Get_outlist_start();
          const vector<DAGNode *>& BitsChild1Orig = BBForm(*it, BBFormMem);
          it++;
          const vector<DAGNode *>& BitsChild2Orig = BBForm(*it, BBFormMem);
          
          int child1_width = BitsChild1Orig.size();
          int child2_width = BitsChild2Orig.size();
      
          const vector<DAGNode *>& left = child1_width < child2_width ? zeroPaddingWithRet(BitsChild1Orig, child2_width) : BitsChild1Orig;
          const vector<DAGNode *>& right = child2_width < child1_width ? zeroPaddingWithRet(BitsChild2Orig, child1_width) : BitsChild2Orig; 
          
          if(PrintBlastTimes)
                {
                 gettimeofday (&step_start_ms, NULL);        
                }  

          DAGNode* result = BBbvule(left, right, false);   
          
          if(PrintBlastTimes)
               {
                gettimeofday (&step_finish_ms, NULL);
                step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                fprintf(LMIQEOutputDataFp, "time blast bvule given blasted children = %llu\t", step_time);
               }
          
          BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(result, node));
          Bits.push_back(result);
      }//bvule ends here
      else if(MyName == "bvuge")
      {
          list<DAGNode*>::iterator it = node->Get_outlist_start();
          const vector<DAGNode *>& BitsChild1Orig = BBForm(*it, BBFormMem);
          it++;
          const vector<DAGNode *>& BitsChild2Orig = BBForm(*it, BBFormMem);
          
          int child1_width = BitsChild1Orig.size();
          int child2_width = BitsChild2Orig.size();
      
          const vector<DAGNode *>& left = child1_width < child2_width ? zeroPaddingWithRet(BitsChild1Orig, child2_width) : BitsChild1Orig;
          const vector<DAGNode *>& right = child2_width < child1_width ? zeroPaddingWithRet(BitsChild2Orig, child1_width) : BitsChild2Orig; 
          
           if(PrintBlastTimes)
                {
                 gettimeofday (&step_start_ms, NULL);        
                }  

          DAGNode* result = BBbvule(right, left, false);      
          
          if(PrintBlastTimes)
               {
                gettimeofday (&step_finish_ms, NULL);
                step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                fprintf(LMIQEOutputDataFp, "time blast bvuge given blasted children = %llu\t", step_time);
               }
          
          BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(result, node));
          Bits.push_back(result);
      }//bvuge ends here
      else if(MyName == "bvult")
      {
          list<DAGNode*>::iterator it = node->Get_outlist_start();
          const vector<DAGNode *>& BitsChild1Orig = BBForm(*it, BBFormMem);
          it++;
          const vector<DAGNode *>& BitsChild2Orig = BBForm(*it, BBFormMem);
          
          int child1_width = BitsChild1Orig.size();
          int child2_width = BitsChild2Orig.size();
      
          const vector<DAGNode *>& left = child1_width < child2_width ? zeroPaddingWithRet(BitsChild1Orig, child2_width) : BitsChild1Orig;
          const vector<DAGNode *>& right = child2_width < child1_width ? zeroPaddingWithRet(BitsChild2Orig, child1_width) : BitsChild2Orig; 
          
           if(PrintBlastTimes)
                {
                 gettimeofday (&step_start_ms, NULL);        
                }  

          DAGNode* result = BBbvule(left, right, true);  
          
          if(PrintBlastTimes)
               {
                gettimeofday (&step_finish_ms, NULL);
                step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                fprintf(LMIQEOutputDataFp, "time blast bvult given blasted children = %llu\t", step_time);
               }
          
          BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(result, node));
          Bits.push_back(result);
      }//bvult ends here
      else if(MyName == "bvugt")
      {
          list<DAGNode*>::iterator it = node->Get_outlist_start();
          const vector<DAGNode *>& BitsChild1Orig = BBForm(*it, BBFormMem);
          it++;
          const vector<DAGNode *>& BitsChild2Orig = BBForm(*it, BBFormMem);
          
          int child1_width = BitsChild1Orig.size();
          int child2_width = BitsChild2Orig.size();
      
          const vector<DAGNode *>& left = child1_width < child2_width ? zeroPaddingWithRet(BitsChild1Orig, child2_width) : BitsChild1Orig;
          const vector<DAGNode *>& right = child2_width < child1_width ? zeroPaddingWithRet(BitsChild2Orig, child1_width) : BitsChild2Orig; 
          
           if(PrintBlastTimes)
                {
                 gettimeofday (&step_start_ms, NULL);        
                }  

          DAGNode* result = BBbvule(right, left, true); 
          
          if(PrintBlastTimes)
               {
                gettimeofday (&step_finish_ms, NULL);
                step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
                step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
                fprintf(LMIQEOutputDataFp, "time blast bvugt given blasted children = %llu\t", step_time);
               }
          
          BlastedDAGToOriginalDAGMapForPredicates.insert(make_pair(result, node));
          Bits.push_back(result);
      }//bvugt ends here   
  }//non-leaf level  

  //added on 16 feb 2016 for debugging
  //showBlastedBits(node, Bits);
  //added on 16 feb 2016 for debugging ends here

  BBFormMem.insert(make_pair(node, Bits));
  return Bits;  
  }//hash miss
}//function ends here
      


//bit-blasts bvule node; left and right are blasted children
DAGNode* BBbvule(const vector<DAGNode*>& left_, const vector<DAGNode*>& right_, bool is_bvlt)
    {
      const vector<DAGNode*>& left = !is_bvlt ? left_ : right_;
      const vector<DAGNode*>& right = !is_bvlt ? right_ : left_;
      
      DAGNode* prevbit = createLogicalConstant("true");
      DAGNode* thisbit;
      
      for (int index=0; index < left.size(); index++)
        {
        thisbit = createDAG("if_then_else", createDAG("=", left[index], right[index], LOGICAL_SYMBOL, 1), prevbit, right[index], LOGICAL_SYMBOL, 1);
        prevbit = thisbit;
        }         
      if (is_bvlt)
        {
        return createDAG("not", thisbit, LOGICAL_SYMBOL, 1);
        }
      else
        {
        return thisbit;  
        }
    }



//function to do blasting of + operator
void BBPlus(vector<DAGNode*> &sum, const vector<DAGNode*>& y, DAGNode* cin)
{
    const int bitWidth = sum.size();
    
    for (int i = 0; i < bitWidth; i++)
        {
        DAGNode* nextcin = Majority(sum[i], y[i], cin);
        sum[i] = createDAG("exor", createDAG("exor", sum[i], y[i], LOGICAL_SYMBOL, 1), cin, LOGICAL_SYMBOL, 1);
        cin = nextcin;
        }
}


// return the majority function of a, b, and c
DAGNode* Majority(DAGNode* a, DAGNode* b, DAGNode* c)
{
    if (isTrue(a))
        {
        return createDAG("or", b, c, LOGICAL_SYMBOL, 1);
        }
    else if (isFalse(a))
        {
        return createDAG("and", b, c, LOGICAL_SYMBOL, 1); 
        }
    else if(isTrue(b))
        {
        return createDAG("or", a, c, LOGICAL_SYMBOL, 1);
        }
    else if (isFalse(b))
        {
        return createDAG("and", a, c, LOGICAL_SYMBOL, 1); 
        }
    else if(isTrue(c))
        {
        return createDAG("or", a, b, LOGICAL_SYMBOL, 1);
        }
    else if (isFalse(c))
        {
        return createDAG("and", a, b, LOGICAL_SYMBOL, 1); 
        }
    else
        {
        DAGNode* or_1 = createDAG("or", createDAG("and", a, b, LOGICAL_SYMBOL, 1), createDAG("and", b, c, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);
        return createDAG("or", or_1,  createDAG("and", a, c, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);
        }
}



//function to do blasting of * operator
vector<DAGNode*> BBMult(const vector<DAGNode*> &x, const vector<DAGNode*> &y,  DAGNode* Child1, DAGNode *Child2)
{    
     if(getNodeType(Child1).data_type != BITVECTOR_CONSTANT && getNodeType(Child2).data_type != BITVECTOR_CONSTANT)
    {
         cout<<"\nbvmul applied on operands both of which are non-constant\n";
         exit(1);
    }
    
    const vector<DAGNode*>& Constant = (getNodeType(Child1).data_type == BITVECTOR_CONSTANT) ? x : y; 
    const vector<DAGNode*>& Variable =  (getNodeType(Child1).data_type == BITVECTOR_CONSTANT) ? y : x;    
    
    const int bitWidth = Constant.size();
    vector<DAGNode*> VariableCopy(Variable);
    vector<DAGNode*> prod = isFalse(Constant[0])? FalseVector(bitWidth) : Variable;// start prod with first partial product.

    for (int i = 1; i < bitWidth; i++)       // start loop at next bit.
        {
        DAGNode* bit = Constant[i];
        // shift first
        BBLShift(VariableCopy, 1);
        
        if (isFalse(bit))
          {
          // If this bit is zero, the partial product will
          // be zero.  No reason to add that in.
          continue;
          }

        vector<DAGNode*> pprod = VariableCopy;
        BBPlus(prod, pprod, createLogicalConstant("false"));
        }
    
  return prod;
}


// Left shift  within fixed field inserting zeros at LSB.
// Writes result into first argument.
void BBLShift(vector<DAGNode*> &x, unsigned int shift)
    {
      // left shift x (destructively) within width.
      // loop backwards so that copy to self works correctly. (DON'T use STL insert!)
      for (int i = ((int) x.size()) - 1; i >= 0; i--)
        {
        if (i - (int) shift >= 0)
          x[i] = x[i - (int) shift];
        else
          x[i] = createLogicalConstant("false"); // new LSB is zero.
        }
    }

//return false vector of given size
vector<DAGNode*> FalseVector(int bitWidth)
{
vector<DAGNode*> AllFalseVector;    
for(int index=bitWidth-1; index>=0; index--)
    {
    AllFalseVector.push_back(createLogicalConstant("false"));
    }    
return AllFalseVector;
}
   
  
//function to do blasting of x+1
vector<DAGNode*> BBInc(const vector<DAGNode*>& x)
{
    const int bitWidth = x.size();
    DAGNode* cin = createLogicalConstant("true");
    vector<DAGNode*> result;
    
    for (int i = 0; i < bitWidth; i++)
        {
        DAGNode* nextcin = createDAG("and", x[i], cin, LOGICAL_SYMBOL, 1);
        DAGNode* sumbit = createDAG("exor", x[i], cin, LOGICAL_SYMBOL, 1);        
        //cout<<"\nnextcin = "<<getDAGStr(nextcin)<<", sumbit = "<<getDAGStr(sumbit)<<endl;        
        result.push_back(sumbit);
        cin = nextcin;
        }
    
    return result;
}

//function to do negate all bits
vector<DAGNode*> BBNeg(const vector<DAGNode*>& x)
{
    const int bitWidth = x.size();
    vector<DAGNode*> result;
    
    for (int i = 0; i < bitWidth; i++)
        {
        result.push_back(createDAG("not", x[i], LOGICAL_SYMBOL, 1));        
        }
    
    return result;
}

// version of function zeroPadding with return
vector<DAGNode*> zeroPaddingWithRet(const vector <DAGNode*> &Bits, int width)
{
  vector<DAGNode*>  ZeroPaddedBits = Bits;
  int pad_size = width - ZeroPaddedBits.size();
  for(int i=0; i<pad_size; i++)
    {
      ZeroPaddedBits.push_back(createLogicalConstant("false"));
    }
  return ZeroPaddedBits;
}



// Find the labels of the independent variables; This is needed for Kuldeep
void findLabelsOfIndependentVariables(map<DAGNode*, string> &LabelTable, set<string> &independent_variables, set<string> &LabelsOfIndependentVariables)
{
    for(map<DAGNode*, string>::iterator lit = LabelTable.begin(); lit != LabelTable.end(); lit++)
    {
        DAGNode* node = lit->first;
        string name = node->Get_node_name();
        string label = lit->second;
        
        if(name == "true" || name == "false")
        {
            LabelsOfIndependentVariables.insert(label);
        }
        
        int index = name.find_last_of("_");
        if(index != string::npos)
        {
           string var_name = name.substr(0, index); 
           if(independent_variables.find(var_name) != independent_variables.end()) // var_name is an independent variable
           {
               LabelsOfIndependentVariables.insert(label);
           }// if var_name is independent
           
        }//if(index != string::npos)
	
    }//for
}//function



// In getBitBlastedDAG, the argument should be a formula. 
// This is a general version of getBitBlastedDAG that takes 
// a bit-vector term or formula
const vector<DAGNode *> getBitBlastedDAG_General(DAGNode *node)
{
  bool stp_style_implementation = true;
  
  if(stp_style_implementation)
  {
      //add code for dynamic programming here
      map<DAGNode*, vector<DAGNode*> > BBFormMem;
      
      vector<DAGNode *> Bits;     
      Bits = BBForm(node, BBFormMem);
      return Bits;      
  }
  else
  {
      cout << "\nOnly implementation possible is stp_style_implementation! The code for operator ite is not added in the function getBitBlastedDAGRecursively\n";
      exit(1);  
  }//else ends here
}//function



// Function to show blasted bits of a node
void showBlastedBits(DAGNode* node, vector<DAGNode*> &Bits)
{
	cout <<"\nnode = " << getDAGStr(node) << endl;
	int bits_size = Bits.size();
	cout<<"\n";
	for(int i = 0; i < bits_size; i++)
  		cout <<"\nBits[" << i << "] = " << getDAGStr(Bits[i]) << endl;
 	cout<<"\n";
}
