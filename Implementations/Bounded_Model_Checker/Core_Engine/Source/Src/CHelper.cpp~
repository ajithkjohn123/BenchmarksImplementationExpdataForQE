#include "CHelper.h"
//#include "NewBV.cc"
#include <assert.h>

extern bool qelim_bit_level;
extern string TRFileName;
extern BVManager* bvm;
//BVManager* bvm = new BVManager();

 CHelper :: CHelper()
 {
        //empty constructor.......
 }


// The createDAG functions are changed on 28/1/2010 as there was a problem in Sourabh's hashing scheme. Ashutosh's hashing scheme is used

/*
    CDAGNode* CHelper :: createDAG(string oprnd_lit)  //to create constant values like '1' and '0'
    {
        char hash_fn_ip[10] = " " ;
        CDAGNode* addr_DAGnode;

        strcpy (hash_fn_ip, oprnd_lit.c_str());


        if(HT.Check_DAGnode_in_ht(hash_fn_ip))
        {
            addr_DAGnode = HT.Get_address_of_existing_DAGnode_from_ht(hash_fn_ip, oprnd_lit);
        }
        else
        {
            CDAGNode* literal_node = new CDAGNode(oprnd_lit);
            addr_DAGnode = literal_node;
            HT.Insert_DAGnode_in_ht(hash_fn_ip , literal_node);
        }
        return addr_DAGnode;
    }

    CDAGNode* CHelper :: createDAG(string oprnd_var, int dt)
    //to create operand variables.....
    //IMPORTANT NOTE:::In  the current implementation the hash fn input is just the variable name
    // Hence currently a_1 and a_3 will always return addrss of a_1
    // eventually delta time also has to be incorporated in the variable name to
    //distinguish between two variables having different delta times........
    {
        char hash_fn_ip[200] = " " ; // Width increased by Ajith : Note that this is a variable
        char char_ptr[10];

        CDAGNode* addr_DAGnode;

        sprintf(char_ptr , "%d", dt);

        string str_dt(char_ptr);
        string temp = oprnd_var + str_dt;;

        //cout << temp << endl ;    //to be decommented only during debugging,....

        strcpy (hash_fn_ip, temp.c_str());
        
        if(HT.Check_DAGnode_in_ht(hash_fn_ip))
        {
            addr_DAGnode = HT.Get_address_of_existing_DAGnode_from_ht(hash_fn_ip, oprnd_var);
        }
        else
        {
            CDAGNode* var_node = new CDAGNode(oprnd_var, dt);
            addr_DAGnode = var_node;
            HT.Insert_DAGnode_in_ht(hash_fn_ip , var_node);
        }
        return addr_DAGnode;

    }

    CDAGNode* CHelper :: createDAG(string oprtor_name, CDAGNode* operand_ptr)
    //to create unary operators....
    {
        char hash_fn_ip[50] = " " ;
        char temp_char[10];
        string temp;
        CDAGNode* addr_DAGnode;

        sprintf(temp_char , "%x", operand_ptr);
        string opd_addr(temp_char);
        temp = oprtor_name + opd_addr;

        strcpy (hash_fn_ip, temp.c_str());

        if(HT.Check_DAGnode_in_ht(hash_fn_ip))
        {
            addr_DAGnode = HT.Get_address_of_existing_DAGnode_from_ht(hash_fn_ip, oprtor_name);
	    operand_ptr->Append_DAGnode_inlist(addr_DAGnode);
        }
        else
        {
            CDAGNode* operand_node = new CDAGNode(oprtor_name, operand_ptr);
            operand_ptr->Append_DAGnode_inlist(operand_node);
            addr_DAGnode = operand_node;
            HT.Insert_DAGnode_in_ht(hash_fn_ip , operand_node);
        }
        return addr_DAGnode;

    }

    CDAGNode* CHelper :: createDAG(string oprtor_name, CDAGNode* operand_ptr1 , CDAGNode* operand_ptr2 )
    //to create binary operators....
    {
        char hash_fn_ip[50] = " " ;
        char temp_char1[10], temp_char2[10];
        string temp;
        CDAGNode* addr_DAGnode;

        sprintf(temp_char1 , "%x", operand_ptr1);
        sprintf(temp_char2 , "%x", operand_ptr2);

        string opd_addr1(temp_char1);
        string opd_addr2(temp_char2);
        temp = oprtor_name + opd_addr1 + opd_addr2;

        strcpy (hash_fn_ip, temp.c_str());

        //cout << temp << endl ;

        if(HT.Check_DAGnode_in_ht(hash_fn_ip))
        {
            addr_DAGnode = HT.Get_address_of_existing_DAGnode_from_ht(hash_fn_ip, oprtor_name);
            operand_ptr1->Append_DAGnode_inlist(addr_DAGnode);
	    operand_ptr2->Append_DAGnode_inlist(addr_DAGnode);
        }
        else
        {
            CDAGNode* operand_node = new CDAGNode(oprtor_name, operand_ptr1, operand_ptr2);
            operand_ptr1->Append_DAGnode_inlist(operand_node);
            operand_ptr2->Append_DAGnode_inlist(operand_node);
            addr_DAGnode = operand_node;
            HT.Insert_DAGnode_in_ht(hash_fn_ip , operand_node);
        }
        return addr_DAGnode;
    }

    CDAGNode* CHelper :: createDAG(string oprtor_name, CDAGNode* operand_ptr1     ,
                                                 CDAGNode* operand_ptr2 , CDAGNode* operand_ptr3 )
    //to create ternary operators......
    {
        char hash_fn_ip[50] = " " ;
        char temp_char1[10], temp_char2[10], temp_char3[10];
        string temp;
        CDAGNode* addr_DAGnode;

        sprintf(temp_char1 , "%x", operand_ptr1);
        sprintf(temp_char2 , "%x", operand_ptr2);
        sprintf(temp_char3 , "%x", operand_ptr3);

        string opd_addr1(temp_char1);
        string opd_addr2(temp_char2);
        string opd_addr3(temp_char3);

        temp = oprtor_name + opd_addr1 + opd_addr2 + opd_addr3;
        strcpy (hash_fn_ip, temp.c_str());

        if(HT.Check_DAGnode_in_ht(hash_fn_ip))
        {
            addr_DAGnode = HT.Get_address_of_existing_DAGnode_from_ht(hash_fn_ip, oprtor_name);
	    operand_ptr1->Append_DAGnode_inlist(addr_DAGnode);
	    operand_ptr2->Append_DAGnode_inlist(addr_DAGnode);
	    operand_ptr3->Append_DAGnode_inlist(addr_DAGnode);	    
        }
        else
        {
            CDAGNode* operand_node = new CDAGNode(oprtor_name, operand_ptr1, operand_ptr2, operand_ptr3);
            operand_ptr1->Append_DAGnode_inlist(operand_node);
            operand_ptr2->Append_DAGnode_inlist(operand_node);
            operand_ptr3->Append_DAGnode_inlist(operand_node);

            addr_DAGnode = operand_node;
            HT.Insert_DAGnode_in_ht(hash_fn_ip , operand_node);
        }
        return addr_DAGnode;
    }

*/





    CDAGNode* CHelper :: createDAG(string oprnd_lit)  //to create constant values like '1' and '0'
    {
        
        string key = oprnd_lit;
        
	// Hash key created

	HTStatusValue<CDAGNode*> result = DAG_HT.hashtable_search(key);

	if(result.success()) // Exists in HT
	  {
	    return result.GetValue();
	  }
	else
	  {
            CDAGNode* addr_DAGnode = new CDAGNode(oprnd_lit);
	    
	    result = DAG_HT.hashtable_insert(key, addr_DAGnode);
   
	    if(!result.success())
	      {

		cout<<"\nHash Insert Failure Inside Function CHelper::createDAG(string oprnd_lit)\n";
		exit(1);
	      }
           return addr_DAGnode;
	  }
       
    }

    CDAGNode* CHelper :: createDAG(string oprnd_var, int dt)
     {
        string key;
        char char_ptr[10];

        sprintf(char_ptr , "%d", dt);
	string str_dt(char_ptr);
        
	key = oprnd_var + str_dt;;

        // Hash key created

	HTStatusValue<CDAGNode*> result = DAG_HT.hashtable_search(key);

	if(result.success()) // Exists in HT
	  {
	    return result.GetValue();
	  }
	else
	  {
	    
            CDAGNode* addr_DAGnode = new CDAGNode(oprnd_var, dt);

	    result = DAG_HT.hashtable_insert(key, addr_DAGnode);
   
	    if(!result.success())
	      {

		cout<<"\nHash Insert Failure Inside Function CHelper::createDAG(string oprnd_var, int dt)\n";
		exit(1);
	      }
           return addr_DAGnode;
        }
       
    }

    CDAGNode* CHelper :: createDAG(string oprtor_name, CDAGNode* operand_ptr)
    //to create unary operators....
    {
        string key;
        char temp_char[10];

	CDAGNode* addr_DAGnode;
        
        sprintf(temp_char , "%x", operand_ptr);
        string opd_addr(temp_char);
        key = oprtor_name + opd_addr;

        // Hash key created

	HTStatusValue<CDAGNode*> result = DAG_HT.hashtable_search(key);

	if(result.success()) // Exists in HT
	  {
	    addr_DAGnode = result.GetValue();
	    operand_ptr->Append_DAGnode_inlist(addr_DAGnode);
	  }

        else
        {
	  addr_DAGnode = new CDAGNode(oprtor_name, operand_ptr);
	  result = DAG_HT.hashtable_insert(key, addr_DAGnode);
   
	  if(!result.success())
	      {

		cout<<"\nHash Insert Failure Inside Function CHelper::createDAG(string oprtor_name, CDAGNode* operand_ptr)\n";
		exit(1);
	      }
	  
	  operand_ptr->Append_DAGnode_inlist(addr_DAGnode);
	}
	
        return addr_DAGnode;

    }

    CDAGNode* CHelper :: createDAG(string oprtor_name, CDAGNode* operand_ptr1 , CDAGNode* operand_ptr2 )
    //to create binary operators....
    {
        string key;
	char temp_char1[10], temp_char2[10];
        
        CDAGNode* addr_DAGnode;

        sprintf(temp_char1 , "%x", operand_ptr1);
        sprintf(temp_char2 , "%x", operand_ptr2);

        string opd_addr1(temp_char1);
        string opd_addr2(temp_char2);

        key = oprtor_name + opd_addr1 + opd_addr2;

	// Hash key created

	HTStatusValue<CDAGNode*> result = DAG_HT.hashtable_search(key);

	if(result.success()) // Exists in HT
	  {
	    addr_DAGnode = result.GetValue();
	    operand_ptr1->Append_DAGnode_inlist(addr_DAGnode);
	    operand_ptr2->Append_DAGnode_inlist(addr_DAGnode);
	  }
	
        else
        {
	  addr_DAGnode = new CDAGNode(oprtor_name, operand_ptr1, operand_ptr2);
	  result = DAG_HT.hashtable_insert(key, addr_DAGnode);
   
	  if(!result.success())
	      {

		cout<<"\nHash Insert Failure Inside Function CHelper::createDAG(string oprtor_name, CDAGNode* operand_ptr1 , CDAGNode* operand_ptr2)\n";
		exit(1);
	      }
	  
	  operand_ptr1->Append_DAGnode_inlist(addr_DAGnode);
          operand_ptr2->Append_DAGnode_inlist(addr_DAGnode);
	}
     
        return addr_DAGnode;
    }

    CDAGNode* CHelper :: createDAG(string oprtor_name, CDAGNode* operand_ptr1     ,
                                                 CDAGNode* operand_ptr2 , CDAGNode* operand_ptr3 )
    //to create ternary operators......
    {
        string key;
        char temp_char1[10], temp_char2[10], temp_char3[10];
        
        CDAGNode* addr_DAGnode;

        sprintf(temp_char1 , "%x", operand_ptr1);
        sprintf(temp_char2 , "%x", operand_ptr2);
        sprintf(temp_char3 , "%x", operand_ptr3);

        string opd_addr1(temp_char1);
        string opd_addr2(temp_char2);
        string opd_addr3(temp_char3);

        key = oprtor_name + opd_addr1 + opd_addr2 + opd_addr3;
        
	// Hash key created

	HTStatusValue<CDAGNode*> result = DAG_HT.hashtable_search(key);

	if(result.success()) // Exists in HT
	  {
	    addr_DAGnode = result.GetValue();
	    operand_ptr1->Append_DAGnode_inlist(addr_DAGnode);
	    operand_ptr2->Append_DAGnode_inlist(addr_DAGnode);
	    operand_ptr3->Append_DAGnode_inlist(addr_DAGnode);	
	  }
	
        else
        {
	  addr_DAGnode = new CDAGNode(oprtor_name, operand_ptr1, operand_ptr2, operand_ptr3);
	  result = DAG_HT.hashtable_insert(key, addr_DAGnode);
   
	  if(!result.success())
	      {

		cout<<"\nHash Insert Failure Inside Function CHelper::createDAG(string oprtor_name, CDAGNode* operand_ptr1, CDAGNode* operand_ptr2 , CDAGNode* operand_ptr3 )\n";
		exit(1);
	      }
	  
	  operand_ptr1->Append_DAGnode_inlist(addr_DAGnode);
          operand_ptr2->Append_DAGnode_inlist(addr_DAGnode);
	  operand_ptr3->Append_DAGnode_inlist(addr_DAGnode);
	}

	return addr_DAGnode;
    }






    CDAGNode* CHelper :: createDAG(string oprtor_name , CDAGNode* operand_ptr1 ,
                                                 int operand2 , int operand3                )
    //for special operators like select(a, 2 , 3)........
    {
        char char_ptr_op2[10] = " ";
        char char_ptr_op3[10] = " ";
        sprintf(char_ptr_op2 , "%d", operand2);
        sprintf(char_ptr_op3 , "%d", operand3);

        string operand2_str(char_ptr_op2);
        string operand3_str(char_ptr_op3);

        CDAGNode* operand_ptr2 = createDAG(operand2_str);
        CDAGNode* operand_ptr3 = createDAG(operand3_str);

        return createDAG(oprtor_name, operand_ptr1 , operand_ptr2 , operand_ptr3 );
    }

    CDAGNode* CHelper :: createDAG(string oprtor_name , CDAGNode* operand_ptr1     ,
                                                 string operand2 , string operand3              )
    //for special operators like select(a, 2 , 3)........
    {
        CDAGNode* operand_ptr2 = createDAG(operand2);
        CDAGNode* operand_ptr3 = createDAG(operand3);

        return createDAG(oprtor_name, operand_ptr1 , operand_ptr2 , operand_ptr3 );
    }

    CDAGNode* CHelper ::createDAG(string name, int lower, int upper, int delta_time)
    {
	CDAGNode* operand_ptr1 = createDAG(name, delta_time);
	
	return createDAG("select", operand_ptr1,  lower, upper);			
    }

    CDAGNode* CHelper :: cloneAndShift(CDAGNode* parent, int shift)
    {
        CDAGNode* clone;
                   
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = cloneAndShift(*iout, shift);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = cloneAndShift(*iout, shift);
		    iout++;
                    child_2 = cloneAndShift(*iout, shift);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = cloneAndShift(*iout, shift);
		    iout++;
                    child_2 = cloneAndShift(*iout, shift);
		    iout++;
                    child_3 = cloneAndShift(*iout, shift);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
            }
        }
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               (parent->Get_DAGnode_delta_time_as_int()+shift));
        }        
        return clone;
    }    


    
    CDAGNode* CHelper :: cloneAndReplace(CDAGNode* parent, map < CDAGNode* , CDAGNode* > var_replacement)
    {
        CDAGNode* clone = parent;

        map < CDAGNode* , CDAGNode* > :: iterator  var_replacement_iter =
        var_replacement.find(parent);
        if(var_replacement_iter!=var_replacement.end())
        {
            clone = var_replacement_iter->second;
        }
        else
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            CDAGNode* child_1, *child_2, *child_3 ;
            

            switch(outlist_size)
            {
                case 1:
                {
                    child_1 = cloneAndReplace(*iout, var_replacement);
                    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    child_1 = cloneAndReplace(*iout, var_replacement);
                    iout++;
                    child_2 = cloneAndReplace(*iout, var_replacement);
                    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    child_1 = cloneAndReplace(*iout, var_replacement);
                    iout++;
                    child_2 = cloneAndReplace(*iout, var_replacement);
                    iout++;
                    child_3 = cloneAndReplace(*iout, var_replacement);
                    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
                case 0:
                {
                    break;
                }
            }
        }
        return clone;
    }

/*
    unsigned long CHelper ::  HT_size()
    {
        return HT.hash_table_size();
    }
*/


  void CHelper :: printGraphInDotFormat(CDAGNode* me, string FileName)
    {
        set<CDAGNode*> Nodes;

	Nodes.clear();
     
	fstream file_op(FileName.c_str(),ios::out);

        file_op<<"digraph G {"<<endl;

	create_node(file_op, me, Nodes);

	file_op<<"}"<<endl;

	file_op.close();
    }

  void CHelper :: create_node(fstream &file_op, CDAGNode* me, set<CDAGNode*> &Nodes)
     {
      if(Nodes.find(me) != Nodes.end())
           {
	   return;
	   }
       else
	 {
	   if(!me->Get_DAG_node_type())
	     {
	       file_op<<"n"<<me<<"[label=\""<< me->Get_DAGnode_value() <<"_"<< me->Get_DAGnode_delta_time_as_string() <<"\"];"<<endl;
	     }
	   else
	     {
	       file_op<<"n"<<me<<"[label=\""<< me->Get_DAGnode_value() <<"\"];"<<endl;
	     } 
	   
	   Nodes.insert(me);

	   unsigned int size = me->Get_DAGnode_outlist_size();
	   list<CDAGNode*>::iterator iter = me->Get_DAGnode_outlist();


	   for(unsigned int i=0; i<size; i++, iter++)
	     {
	       file_op<<"n"<<me<<"->"<<"n"<<(*iter)<<";"<<endl;
	     }

	   iter = me->Get_DAGnode_outlist();

	   for(unsigned int i=0; i<size; i++, iter++)
	     {
	       create_node(file_op, *iter, Nodes);
	     }
	 }
      }

CDAGNode* CHelper :: Inlining(CDAGNode* root, set<string>& vars_to_ex_quantify)
{
    CDAGNode* return_value, *converted_ite_function;
    map < CDAGNode* , list <CDAGNode*> > expressions_for_externally_quantified_vars;
    expressions_for_externally_quantified_vars.clear(); //map initializations.......
    if(root->Get_DAGnode_value() != "equivalent")
    {
        converted_ite_function = convertIteToFunction(root);

        find_and_save_expressions_for_exQuant_vars(converted_ite_function, vars_to_ex_quantify,
                                                   expressions_for_externally_quantified_vars );

        //modifying the original set to indicate the no of variables which can be quantified for the given DAG
        map < CDAGNode* , list <CDAGNode*> > :: iterator ac_exQ_it;
        for(ac_exQ_it = expressions_for_externally_quantified_vars.begin()  ;
            ac_exQ_it != expressions_for_externally_quantified_vars.end()   ;
            ac_exQ_it++                                                     )
        {
            CDAGNode* temp = ac_exQ_it->first;  //this is the address of the "exists" node.....
            if(temp->Get_DAGnode_value()=="select")
            {
                list<CDAGNode*>::iterator iout_select = temp->Get_DAGnode_outlist();
                //assuming that the search string / variable name is the first argument of the select operator......
                string search_string =  (*iout_select)->Get_DAGnode_value() + "_" +
                                        (*iout_select)->Get_DAGnode_delta_time_as_string();
                if(vars_to_ex_quantify.find(search_string) != vars_to_ex_quantify.end())
                {
                    vars_to_ex_quantify.erase(search_string);
                }
            }
            else
            {
                string search_string = temp->Get_DAGnode_value() + " " + temp->Get_DAGnode_delta_time_as_string();
                if(vars_to_ex_quantify.find(search_string) != vars_to_ex_quantify.end())
                {
                    vars_to_ex_quantify.erase(search_string);
                }
            }
        }
        find_smallest_expr_for_exQuant_vars(expressions_for_externally_quantified_vars);
        return_value = Actual_inlining_substitution(converted_ite_function, expressions_for_externally_quantified_vars);
    }
    else
    {
        return_value = root ;
    }
    return return_value;
}


CDAGNode* CHelper :: Actual_Global_Inlining(CDAGNode* root                              ,
                                            map < CDAGNode* , set<string> >& exists_source_destination   ,
                                            list <CDAGNode*>& inlining_order                           )
{
    unsigned int node_outlist_size = root->Get_DAGnode_outlist_size();
    //iterator to the node outlist......
    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();;

    CDAGNode* node_after_inlining = root;
    
    switch(node_outlist_size)
    {
        case 0:
        {
            break;  //do nothing...................
        }
        case 1:
        {
            CDAGNode    *op1;
            CDAGNode *temp_op1;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = Actual_Global_Inlining((*iout), exists_source_destination, inlining_order );

            if(temp_op1!=op1)
            {
                node_after_inlining = createDAG(root->Get_DAGnode_value(),op1);
                temp_op1->Delete_single_entry_from_DAG_inlist(root);

                if(root->isroot()==true)
                {
                    root->mark_redendant_node_for_deletion();
                }
            }
            break;
        }
        case 2:
        {
            CDAGNode *op1, *op2;
            CDAGNode *temp_op1, *temp_op2;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = Actual_Global_Inlining((*iout), exists_source_destination, inlining_order );
            iout++;
            temp_op2 = *iout;
            op2 = Actual_Global_Inlining((*iout), exists_source_destination, inlining_order );

            if(exists_source_destination.find(root)!=exists_source_destination.end())  //i.e. if the node is an exists node.....
            {
                CDAGNode *inlined_node;
                set<string> temp_set;
                unsigned int count = 0;
                
                for(    set<string> :: iterator temp_set_iter = exists_source_destination[root].begin()   ;
                        temp_set_iter != exists_source_destination[root].end() ; temp_set_iter++, count++ )
                {
                    CDAGNode  *local_temp, *temp;

                    temp_set.clear();
                    temp_set.insert(*temp_set_iter);

                    switch(count)
                    {
                        case 0:
                        {
                            temp = Inlining(op2, temp_set);
                            if(temp_set.empty())
                            {
                                exists_source_destination[root].erase(temp_set_iter);
                            }
                            if(temp!=op2)
                            {
                                inlined_node = temp;
                                if(op2->isroot()==true)
                                {
                                    op2->mark_redendant_node_for_deletion();
                                }
                                unsigned int ol_size = op2->Get_DAGnode_outlist_size();
                                list<CDAGNode*>::iterator li = op2->Get_DAGnode_outlist();
                                for(unsigned int i=0;i<ol_size;i++, li++)
                                {
                                    (*li)->Delete_single_entry_from_DAG_inlist(op2);
                                }
                            }
                            else
                            {
                                inlined_node = op2;
                            }
                            break;
                        }
                        default:
                        {
                            local_temp = Inlining(temp, temp_set);

                            if(temp_set.empty())
                            {
                                exists_source_destination[root].erase(temp_set_iter);
                            }

                            if(local_temp!=temp)
                            {
                                inlined_node = local_temp;
                                if(temp->isroot()==true)
                                {
                                    temp->mark_redendant_node_for_deletion();
                                }
                                unsigned int ol_size = temp->Get_DAGnode_outlist_size();
                                list<CDAGNode*>::iterator li = temp->Get_DAGnode_outlist();
                                for(unsigned int i=0;i<ol_size;i++, li++)
                                {
                                    (*li)->Delete_single_entry_from_DAG_inlist(temp);
                                }
                            }
                            else
                            {
                                inlined_node = temp;
                            }
                            temp = local_temp;
                            break;
                            
                        }
                    }//end switch.........
                }

                if(exists_source_destination[root].empty()) //if all the variables are quantified........
                {
                    node_after_inlining = inlined_node ;
                    if(root->isroot()==true)
                    {
                        root->mark_redendant_node_for_deletion();
                    }
                    temp_op1->Delete_single_entry_from_DAG_inlist(root);
                    temp_op2->Delete_single_entry_from_DAG_inlist(root);
                }
                else
                {
                    CDAGNode *new_lhs = Get_quantified_var_list_after_single_inlining(temp_op1, exists_source_destination[root]);
                    if(new_lhs!=NULL)
                    {
                        if(new_lhs==inlined_node)
                        {
                            node_after_inlining = inlined_node ;
                            if(root->isroot()==true)
                            {
                                root->mark_redendant_node_for_deletion();
                            }
                            temp_op1->Delete_single_entry_from_DAG_inlist(root);
                            temp_op2->Delete_single_entry_from_DAG_inlist(root);
                        }
                        else
                        {
                            node_after_inlining = createDAG(root->Get_DAGnode_value(),new_lhs, inlined_node);
                        }
                    }
                    else
                    {
                        node_after_inlining = inlined_node ;
                        if(root->isroot()==true)
                        {
                            root->mark_redendant_node_for_deletion();
                        }
                        temp_op1->Delete_single_entry_from_DAG_inlist(root);
                        temp_op2->Delete_single_entry_from_DAG_inlist(root);
                    }
                }
                inlining_order.remove(root);
            }
            else
            {
                if((temp_op1!=op1)||(temp_op2!=op2))
                {
                    if(op1==op2)
                    {
                        node_after_inlining = op1;  //to eliminate duplicate nodes.......
                    }
                    else
                    {
                        node_after_inlining = createDAG(root->Get_DAGnode_value(),op1, op2);
                    }

                    if(root->isroot()==true)
                    {
                        root->mark_redendant_node_for_deletion();
                    }
                    temp_op1->Delete_single_entry_from_DAG_inlist(root);
                    temp_op2->Delete_single_entry_from_DAG_inlist(root);
                }
            }
            break;
        }
        case 3:
        {
            CDAGNode    *op1, *op2, *op3;
            CDAGNode *temp_op1, *temp_op2, *temp_op3;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = Actual_Global_Inlining((*iout), exists_source_destination, inlining_order );
            iout++;
            temp_op2 = *iout;
            op2 = Actual_Global_Inlining((*iout), exists_source_destination, inlining_order );
            iout++;
            temp_op3 = *iout;
            op3 = Actual_Global_Inlining((*iout), exists_source_destination, inlining_order );

            if((temp_op1!=op1)||(temp_op2!=op2)||(temp_op3!=op3))
            {
                node_after_inlining = createDAG(root->Get_DAGnode_value(),op1, op2, op3);
                temp_op1->Delete_single_entry_from_DAG_inlist(root);
                temp_op2->Delete_single_entry_from_DAG_inlist(root);
                temp_op3->Delete_single_entry_from_DAG_inlist(root);

                if(root->isroot()==true)
                {
                    root->mark_redendant_node_for_deletion();
                }

            }
            break;
        }
    }
    return node_after_inlining;
}

void CHelper :: find_and_store_ex_quant_from_global_DAG(CDAGNode* root                              ,
                                       map < CDAGNode* , set<string> >& exists_source_destination   ,
                                       list <CDAGNode*>& inlining_order                           )
{
    list<CDAGNode*>::iterator iout ;

    if(root->Get_DAGnode_value() == "exists")
    {
        CDAGNode *lhs,*rhs;

        iout = root->Get_DAGnode_outlist();

        lhs = (*iout);
        ++iout;
        rhs = (*iout);
        //implementation assumes that, the quantified variables are
        //on the left side of the exists operator
        if(lhs->Get_DAGnode_value()== "comma")
        {
            set <string> temp_string_set;
            temp_string_set.clear();    //initialization at start......
            assimilate_all_exQuant_vars_from_a_comma(lhs,temp_string_set);
            exists_source_destination.insert(pair< CDAGNode* , set<string> >(root , temp_string_set));
            inlining_order.push_back(root);
        }
        else
        {
            iout = root->Get_DAGnode_outlist();
            set <string> temp_string_set;
            temp_string_set.clear();    //initialization at start......
            temp_string_set.insert((lhs->Get_DAGnode_value() + "_" +
                                   lhs->Get_DAGnode_delta_time_as_string()));
            exists_source_destination.insert(pair< CDAGNode* , set<string> >(root , temp_string_set));
            inlining_order.push_back(root); //store the address of the exists node.......
        }

        find_and_store_ex_quant_from_global_DAG(rhs , exists_source_destination, inlining_order);
    }
    else
    {
        iout = root->Get_DAGnode_outlist();
        unsigned int osize = root->Get_DAGnode_outlist_size();
        for(unsigned int i=0;i<osize; i++, iout++)
        {
            find_and_store_ex_quant_from_global_DAG((*iout) , exists_source_destination, inlining_order);
        }
    }
}

void CHelper :: assimilate_all_exQuant_vars_from_a_comma(CDAGNode* root, set<string>& exQvars)
{
    //implementation assumes that if multiple items are present;
    //then lhs is the variable and rhs is again a ','......
    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();

    CDAGNode *lhs,*rhs;
    lhs = (*iout);
    ++iout;
    rhs = (*iout);
    if((lhs->Get_DAGnode_value() != "comma") && (rhs->Get_DAGnode_value() != "comma"))
    {
        exQvars.insert((lhs->Get_DAGnode_value() + "_" + lhs->Get_DAGnode_delta_time_as_string()));
        exQvars.insert((rhs->Get_DAGnode_value() + "_" + rhs->Get_DAGnode_delta_time_as_string()));
    }
    else
    {
        exQvars.insert((rhs->Get_DAGnode_value() + "_" +rhs->Get_DAGnode_delta_time_as_string()));
        assimilate_all_exQuant_vars_from_a_comma(lhs,exQvars);
    }
}

void CHelper :: find_and_save_expressions_for_exQuant_vars( CDAGNode* root                            ,
                                                            set<string>& vars_to_ex_quantify          ,
                                                            map < CDAGNode* , list <CDAGNode*> >&
                                                            expressions_for_externally_quantified_vars)
{
    if(root->Get_DAGnode_value()=="and")
    {
        list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();

        find_and_save_expressions_for_exQuant_vars((*iout), vars_to_ex_quantify,
                                               expressions_for_externally_quantified_vars );
        iout++;
        find_and_save_expressions_for_exQuant_vars((*iout), vars_to_ex_quantify,
                                               expressions_for_externally_quantified_vars );

    }
    else
    {
        list<CDAGNode*>::iterator iout ;

        if(root->Get_DAGnode_value() == "equivalent")
        {
            CDAGNode *lhs,*rhs;

            iout = root->Get_DAGnode_outlist();
            lhs = (*iout);
            ++iout;
            rhs = (*iout);

            if((lhs->Get_DAGnode_value()=="select")||(rhs->Get_DAGnode_value()=="select"))
            {
                CDAGNode *lhs_select,*rhs_select;
                string exQ_var_select;
                map < CDAGNode* , list <CDAGNode*> > :: iterator map_iterator_select;

                if(lhs->Get_DAGnode_value()=="select")
                {
                    list<CDAGNode*>::iterator iout_lhs_select = lhs->Get_DAGnode_outlist();
                    lhs_select = (*iout_lhs_select);
                    

                    exQ_var_select = lhs_select->Get_DAGnode_value() + "_" + lhs_select->Get_DAGnode_delta_time_as_string();
                    if (vars_to_ex_quantify.find(exQ_var_select)!=vars_to_ex_quantify.end())
                    {
                        map_iterator_select = expressions_for_externally_quantified_vars.find(lhs);

                        if(map_iterator_select != expressions_for_externally_quantified_vars.end())
                        //i.e. if the externally quantified variable is not present in the MAP.....
                        {
                            expressions_for_externally_quantified_vars[lhs].push_back(rhs);
                        }
                        else
                        {
                            list <CDAGNode*> temp_list;
                            temp_list.clear();

                            expressions_for_externally_quantified_vars.insert
                            (pair<CDAGNode* ,list<CDAGNode*> >(lhs,temp_list));
                            expressions_for_externally_quantified_vars[lhs].push_back(rhs);
                        }
                    }
                }
                else
                {
                    list<CDAGNode*>::iterator iout_rhs_select = rhs->Get_DAGnode_outlist();
                    rhs_select = (*iout_rhs_select);
                    
                    exQ_var_select = rhs_select->Get_DAGnode_value() + "_" + rhs_select->Get_DAGnode_delta_time_as_string();
                    if (vars_to_ex_quantify.find(exQ_var_select)!=vars_to_ex_quantify.end())
                    {
                        map_iterator_select = expressions_for_externally_quantified_vars.find(rhs);

                        if(map_iterator_select != expressions_for_externally_quantified_vars.end())
                        //i.e. if the externally quantified variable is not present in the MAP.....
                        {
                            expressions_for_externally_quantified_vars[rhs].push_back(lhs);
                        }
                        else
                        {
                            list <CDAGNode*> temp_list;
                            temp_list.clear();

                            expressions_for_externally_quantified_vars.insert
                            (pair<CDAGNode* ,list<CDAGNode*> >(rhs,temp_list));
                            expressions_for_externally_quantified_vars[rhs].push_back(lhs);
                        }
                    }
                }
            }
            else
            {
                string exQ_var;
                map < CDAGNode* , list <CDAGNode*> > :: iterator map_iterator;

                exQ_var = lhs->Get_DAGnode_value() + "_" +
                          lhs->Get_DAGnode_delta_time_as_string();
                if (vars_to_ex_quantify.find(exQ_var)!=vars_to_ex_quantify.end())
                {
                    map_iterator = expressions_for_externally_quantified_vars.find(lhs);

                    if(map_iterator != expressions_for_externally_quantified_vars.end())
                    //i.e. if the externally quantified variable is not present in the MAP.....
                    {
                        expressions_for_externally_quantified_vars[lhs].push_back(rhs);
                    }
                    else
                    {
                        list <CDAGNode*> temp_list;
                        temp_list.clear();

                        expressions_for_externally_quantified_vars.insert
                        (pair<CDAGNode* ,list<CDAGNode*> >(lhs,temp_list));
                        expressions_for_externally_quantified_vars[lhs].push_back(rhs);
                    }
                }

                exQ_var = rhs->Get_DAGnode_value() + "_" +
                          rhs->Get_DAGnode_delta_time_as_string();
                if (vars_to_ex_quantify.find(exQ_var)!=vars_to_ex_quantify.end())
                {
                    map_iterator = expressions_for_externally_quantified_vars.find(rhs);

                    if(map_iterator != expressions_for_externally_quantified_vars.end())
                    //i.e. if the externally quantified variable is not present in the MAP.....
                    {
                        expressions_for_externally_quantified_vars[rhs].push_back(lhs);
                    }
                    else
                    {
                        list <CDAGNode*> temp_list;
                        temp_list.clear();

                        expressions_for_externally_quantified_vars.insert
                        (pair<CDAGNode* ,list<CDAGNode*> >(rhs,temp_list));
                        expressions_for_externally_quantified_vars[rhs].push_back(lhs);
                    }
                }
            }
        }
    }
}

void CHelper :: find_smallest_expr_for_exQuant_vars( map < CDAGNode* , list <CDAGNode*> >&
                                                     expressions_for_externally_quantified_vars)
{
    map < CDAGNode* , list <CDAGNode*> > smallest_expr;
    map < CDAGNode* , list <CDAGNode*> > :: iterator temp_iter =
                                         expressions_for_externally_quantified_vars.begin();

    unsigned int no_of_exQ_vars = expressions_for_externally_quantified_vars.size();
    list <CDAGNode*> temp_list;
    list <CDAGNode*> :: iterator temp_list_iter ;

    smallest_expr.clear();
    for(unsigned int i=0;i<no_of_exQ_vars;i++)
    {
        CDAGNode *min_size;
        unsigned int no_of_exQ_var_exprs = 
                expressions_for_externally_quantified_vars[temp_iter->first].size();
        
        temp_list.clear();
        smallest_expr.insert(pair<CDAGNode* ,list<CDAGNode*> >(temp_iter->first,temp_list));
        
        temp_list_iter = expressions_for_externally_quantified_vars[temp_iter->first].begin();
        min_size = (*temp_list_iter);
        for(unsigned int j=0;j<no_of_exQ_var_exprs;j++)
        {
            int dummy_min=0, node_depth_min=0, dummy, node_depth=0; //initialization is very important

            min_size->size(dummy_min, node_depth_min);
            (*temp_list_iter)->size(dummy, node_depth);
            min_size = (node_depth < node_depth_min) ? (*temp_list_iter) : min_size ;
            temp_list_iter++;
        }
        smallest_expr[temp_iter->first].push_back(min_size);
        temp_iter++;
    }
    expressions_for_externally_quantified_vars = smallest_expr;
}

/* NEW FUNCTION FOR INLINING
void CHelper :: find_smallest_expr_for_exQuant_vars( map < CDAGNode* , list <CDAGNode*> >&
                                                     expressions_for_externally_quantified_vars)
{
    map < CDAGNode* , list <CDAGNode*> > smallest_expr;
    map < CDAGNode* , list <CDAGNode*> > :: iterator temp_iter =
                                         expressions_for_externally_quantified_vars.begin();

    unsigned int no_of_exQ_vars = expressions_for_externally_quantified_vars.size();
    list <CDAGNode*> temp_list;
    list <CDAGNode*> :: iterator temp_list_iter ;

    smallest_expr.clear();
    for(unsigned int i=0;i<no_of_exQ_vars;i++)
    {
        CDAGNode *min_size;
        unsigned int no_of_exQ_var_exprs =
                expressions_for_externally_quantified_vars[temp_iter->first].size();

        temp_list.clear();
        smallest_expr.insert(pair<CDAGNode* ,list<CDAGNode*> >(temp_iter->first,temp_list));

	if(no_of_exQ_var_exprs > 1)
	{
		temp_list_iter = expressions_for_externally_quantified_vars[temp_iter->first].begin();
		if((*temp_list_iter)==temp_iter->first)
		{
			temp_list_iter++;
			min_size = (*temp_list_iter);
		}
		else
		{
			min_size = (*temp_list_iter);
		}

		temp_list_iter = expressions_for_externally_quantified_vars[temp_iter->first].begin();
	        for(unsigned int j=0;j<no_of_exQ_var_exprs;j++)
	        {
	            int dummy_min=0, node_depth_min=0, dummy, node_depth=0; //initialization is very important

	            min_size->size(dummy_min, node_depth_min);
	            (*temp_list_iter)->size(dummy, node_depth);
	            min_size = ( (node_depth < node_depth_min) && ((*temp_list_iter)!=temp_iter->first) )? (*temp_list_iter) : min_size ;
	            temp_list_iter++;
	        }
	}
	else
	{
		temp_list_iter = expressions_for_externally_quantified_vars[temp_iter->first].begin();
		min_size = (*temp_list_iter);
	}

	if(temp_iter->first != min_size)	//this is to take care of the fact that if the first equality encountered is of the type x=x....
	{
	        smallest_expr[temp_iter->first].push_back(min_size);
	}
        temp_iter++;
    }

    vector <CDAGNode*> exQ_vars_having_no_valid_assignments;
    exQ_vars_having_no_valid_assignments.clear();
    for(temp_iter=smallest_expr.begin(); temp_iter!=smallest_expr.end(); temp_iter++)
    {
	if(smallest_expr[temp_iter->first].size()==0)
	{
	   exQ_vars_having_no_valid_assignments.push_back(temp_iter->first);
	}
    }   
    for(unsigned int i=0; i<exQ_vars_having_no_valid_assignments.size(); i++)
    {
	smallest_expr.erase(exQ_vars_having_no_valid_assignments[i]);
    }

    expressions_for_externally_quantified_vars = smallest_expr;
}

*/

CDAGNode* CHelper :: Actual_inlining_substitution(CDAGNode* root, map < CDAGNode* , list <CDAGNode*> >&
                                                            expressions_for_externally_quantified_vars)
{
    CDAGNode* node_after_inlining;
    unsigned int node_outlist_size = root->Get_DAGnode_outlist_size();
    //iterator to the node outlist......
    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();


    map < CDAGNode* , list <CDAGNode*> > :: iterator map_iterator = expressions_for_externally_quantified_vars.find(root);
    list <CDAGNode*> :: iterator temp_list_iter ;
    if(map_iterator != expressions_for_externally_quantified_vars.end())
    {
        temp_list_iter = expressions_for_externally_quantified_vars[map_iterator->first].begin();
        node_after_inlining = (*temp_list_iter);

        for(unsigned int i=0;i<node_outlist_size;i++, iout++)
        {
            (*iout)->Delete_single_entry_from_DAG_inlist(root);
        }
        if(root->isroot()==true)
        {
            root->mark_redendant_node_for_deletion();
        }

    }
    else
    {
        node_after_inlining = root;

        switch(node_outlist_size)
        {
            case 0:
            {
                break;
            }
            case 1:
            {
                CDAGNode    *op1;
                CDAGNode *temp_op1;
                temp_op1 = *iout;
                op1 = Actual_inlining_substitution((*iout),expressions_for_externally_quantified_vars);
                if(temp_op1!=op1)
                {
                    if(op1==NULL)
                    {
                        node_after_inlining = NULL ;
                    }
                    else
                    {
                        node_after_inlining = createDAG(root->Get_DAGnode_value(),op1);
                    }

                    temp_op1->Delete_single_entry_from_DAG_inlist(root);

                    if(root->isroot()==true)
                    {
                        root->mark_redendant_node_for_deletion();
                    }
                }
                else
                {
                    node_after_inlining = root;
                }
                break;
            }
            case 2:
            {
                CDAGNode    *op1, *op2;
                CDAGNode *temp_op1, *temp_op2;
                iout = root->Get_DAGnode_outlist();
                temp_op1 = *iout;
                op1 = Actual_inlining_substitution((*iout),expressions_for_externally_quantified_vars);
                iout++;
                temp_op2 = *iout;
                op2 = Actual_inlining_substitution((*iout),expressions_for_externally_quantified_vars);


                if((temp_op1!=op1)||(temp_op2!=op2))
                {
                    if((root->Get_DAGnode_value()=="equivalent")&&(op1==op2))
                    {
                        node_after_inlining = NULL;
                    }
                    else
                    {
                        if((op1==NULL)&&(op2==NULL))
                        {
                            node_after_inlining = NULL;
                        }
                        else
                        {
                            if((op1!=NULL)&&(op2!=NULL))
                            {
                                node_after_inlining = createDAG(root->Get_DAGnode_value(),op1, op2);
                            }
                            else
                            {
                                if(op1==NULL)
                                {
                                    node_after_inlining = op2;
                                }
                                else
                                {
                                    node_after_inlining = op1;
                                }
                            }
                        }
                    }
                    temp_op1->Delete_single_entry_from_DAG_inlist(root);
                    temp_op2->Delete_single_entry_from_DAG_inlist(root);

                    if(root->isroot()==true)
                    {
                        root->mark_redendant_node_for_deletion();
                    }
                }
                else
                {
                    node_after_inlining = root;
                }

                break;
            }
            case 3:
            {
                CDAGNode    *op1, *op2, *op3;
                CDAGNode *temp_op1, *temp_op2, *temp_op3;
                iout = root->Get_DAGnode_outlist();
                temp_op1 = *iout;
                op1 = Actual_inlining_substitution((*iout),expressions_for_externally_quantified_vars);
                iout++;
                temp_op2 = *iout;
                op2 = Actual_inlining_substitution((*iout),expressions_for_externally_quantified_vars);
                iout++;
                temp_op3 = *iout;
                op3 = Actual_inlining_substitution((*iout),expressions_for_externally_quantified_vars);

                if((temp_op1!=op1)||(temp_op2!=op2)||(temp_op3!=op3))
                {
                    if((op1!=NULL)&&(op2!=NULL)&&(op3!=NULL))
                    {
                        node_after_inlining = createDAG(root->Get_DAGnode_value(),op1, op2, op3);
                    }
                    else
                    {
                        node_after_inlining = NULL;
                    }

                    temp_op1->Delete_single_entry_from_DAG_inlist(root);
                    temp_op2->Delete_single_entry_from_DAG_inlist(root);
                    temp_op3->Delete_single_entry_from_DAG_inlist(root);

                    if(root->isroot()==true)
                    {
                        root->mark_redendant_node_for_deletion();
                    }
                }
                else
                {
                    node_after_inlining = root;
                }
                break;
            }
        }
    }
    return node_after_inlining ;
}

CDAGNode* CHelper :: convertIteToFunction(CDAGNode *root)
{
    unsigned int node_outlist_size = root->Get_DAGnode_outlist_size();
    //iterator to the node outlist......
    list<CDAGNode*>::iterator iout ;
    
    CDAGNode* ite_as_function = root;

    switch(node_outlist_size)
    {
        case 3:
        {
            if(root->Get_DAGnode_value() == "ite")
            {
                CDAGNode *cond,*stat1,*stat2;

                iout = root->Get_DAGnode_outlist();
                cond = (*iout);
                ++iout;
                stat1 = (*iout);
                ++iout;
                stat2 = (*iout);

                if( (stat1->Get_DAGnode_value() == "equivalent")   &&
                    (stat2->Get_DAGnode_value() == "equivalent")    )
                {
                    list<CDAGNode*>::iterator stat1_child  = stat1->Get_DAGnode_outlist();
                    list<CDAGNode*>::iterator stat2_child  = stat2->Get_DAGnode_outlist();
                    CDAGNode *lhs1,*lhs2,*rhs1,*rhs2;

                    lhs1 = (*stat1_child);
                    lhs2 = (*stat2_child);
                    stat1_child++;
                    stat2_child++;
                    rhs1 = (*stat1_child);
                    rhs2 = (*stat2_child);

                    if((lhs1->Get_DAGnode_value() == lhs2->Get_DAGnode_value()))
                    {
                        lhs1->Delete_single_entry_from_DAG_inlist(stat1);
                        lhs2->Delete_single_entry_from_DAG_inlist(stat2);
                        rhs1->Delete_single_entry_from_DAG_inlist(stat1);
                        rhs2->Delete_single_entry_from_DAG_inlist(stat2);

                        cond->Delete_single_entry_from_DAG_inlist(root);
                        stat1->Delete_single_entry_from_DAG_inlist(root);
                        stat2->Delete_single_entry_from_DAG_inlist(root);

                        ite_as_function = createDAG("equivalent",lhs1,createDAG("ite",cond,rhs1,rhs2));
                        if(root->isroot()==true)
                        {
                            root->mark_redendant_node_for_deletion();
                        }
                    }
                 }
            }
            else
            {
                CDAGNode    *op1, *op2, *op3;
                CDAGNode *temp_op1, *temp_op2, *temp_op3;
                iout = root->Get_DAGnode_outlist();
                temp_op1 = *iout;
                op1 = convertIteToFunction(*iout);
                iout++;
                temp_op2 = *iout;
                op2 = convertIteToFunction(*iout);
                iout++;
                temp_op3 = *iout;
                op3 = convertIteToFunction(*iout);
                if((temp_op1!=op1)||(temp_op2!=op2)||(temp_op3!=op3))
                {
                    ite_as_function = createDAG(root->Get_DAGnode_value(),op1, op2, op3);
                    temp_op1->Delete_single_entry_from_DAG_inlist(root);
                    temp_op2->Delete_single_entry_from_DAG_inlist(root);
                    temp_op3->Delete_single_entry_from_DAG_inlist(root);
                    if(root->isroot()==true)
                    {
                        root->mark_redendant_node_for_deletion();
                    }
                }
            }
            break;
        }
        case 0:
        {
            break;
        }
        case 1:
        {
            CDAGNode    *op1;
            CDAGNode *temp_op1;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = convertIteToFunction(*iout);
            if(temp_op1!=op1)
            {
                ite_as_function = createDAG(root->Get_DAGnode_value(),op1);
                temp_op1->Delete_single_entry_from_DAG_inlist(root);
                if(root->isroot()==true)
                {
                    root->mark_redendant_node_for_deletion();
                }
            }
            break;
        }
        case 2:
        {            
            CDAGNode    *op1, *op2;
            CDAGNode *temp_op1, *temp_op2;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = convertIteToFunction(*iout);
            iout++;
            temp_op2 = *iout;
            op2 = convertIteToFunction(*iout);
            if((temp_op1!=op1)||(temp_op2!=op2))
            {
                ite_as_function = createDAG(root->Get_DAGnode_value(),op1, op2);
                temp_op1->Delete_single_entry_from_DAG_inlist(root);
                temp_op2->Delete_single_entry_from_DAG_inlist(root);
                if(root->isroot()==true)
                {
                    root->mark_redendant_node_for_deletion();
                }
            }
            break;
        }
    }
    return ite_as_function;
}

CDAGNode* CHelper :: Get_quantified_var_list_after_single_inlining(CDAGNode* root, set<string>& vars_not_exQ)
{
    CDAGNode* return_value = root;

    if(root->Get_DAGnode_value()=="comma")
    {
        list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();

        CDAGNode *lhs,*rhs,*new_lhs,*new_rhs ;
        lhs = (*iout);
        ++iout;
        rhs = (*iout);
        new_lhs = Get_quantified_var_list_after_single_inlining(lhs, vars_not_exQ);
        new_rhs = Get_quantified_var_list_after_single_inlining(rhs, vars_not_exQ);

        if((new_lhs!=lhs)||(new_rhs!=rhs))
        {
            if((new_lhs==NULL)&&(new_rhs==NULL))
            {
                return_value = NULL;
            }
            else
            {
                if((new_lhs!=NULL)&&(new_rhs!=NULL))
                {
                    return_value = createDAG(root->Get_DAGnode_value(),new_lhs, new_rhs);
                }
                else
                {
                    if(new_lhs==NULL)
                    {
                        return_value = new_rhs;
                    }
                    else
                    {
                        return_value = new_lhs;
                    }
                }
            }

            if(root->isroot()==true)
            {
                root->mark_redendant_node_for_deletion();
            }
            lhs->Delete_single_entry_from_DAG_inlist(root);
            rhs->Delete_single_entry_from_DAG_inlist(root);
        }
    }
    else
    {
        string temp_string = root->Get_DAGnode_value() + "_" + root->Get_DAGnode_delta_time_as_string();
        if(vars_not_exQ.find(temp_string)==vars_not_exQ.end())  //i.e. root variable has been quantified out.........
        {
            return_value = NULL;
        }
    }
    return return_value;
}

/*
void CHelper :: garbage_collector(void)
{
    HT.gc_for_ht();
}
*/

void CHelper :: Get_functions_for_sum_product_algo_from_a_DAG(CDAGNode* root, list<CDAGNode*>& function_list,
                                                              list<CDAGNode*>& special_function_list        )
{
    //assumption is that the lists are initially empty.........

    if(root->Get_DAGnode_value()=="and")
    {
        list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();

        Get_functions_for_sum_product_algo_from_a_DAG((*iout), function_list, special_function_list );
        iout++;
        Get_functions_for_sum_product_algo_from_a_DAG((*iout), function_list, special_function_list );
    }
    else
    {
        if(root->Get_DAG_node_type()==true)
        {
            if(root->Get_DAGnode_outlist_size()!=0) //neglect constant noses i.e literals..........
            {
                function_list.push_back(root);
            }
        }
        else
        {
            special_function_list.push_back(root);  //holds the variable node addresses which act as function nodes.....
        }
    }
}

void CHelper :: form_factor_graph(CDAGNode* root)
{
    list<CDAGNode*> function_list;
    list<CDAGNode*> special_function_list;
    function_list.clear();
    special_function_list.clear();
    Get_functions_for_sum_product_algo_from_a_DAG(root, function_list, special_function_list);
    
    if(!function_list.empty())
    {
        list<CDAGNode*> :: iterator func_node_iter;
        for(func_node_iter=function_list.begin(); func_node_iter!=function_list.end(); func_node_iter++)
        {
            set<CDAGNode*> vars_of_a_func;
            vars_of_a_func.clear();

            Get_vars_of_func_and_form_var_nodes((*func_node_iter), (*func_node_iter), vars_of_a_func);
            Factor_graph_Node* temp = createFG_func_node((*func_node_iter));
            for(set<CDAGNode*>::iterator i = vars_of_a_func.begin();i!=vars_of_a_func.end(); i++)
            {
                temp->append_ptr_list(*i);
            }
        }
    }
    
    if(!special_function_list.empty())
    {
        list<CDAGNode*> :: iterator sp_func_node_iter;
        for(sp_func_node_iter=special_function_list.begin(); sp_func_node_iter!=special_function_list.end();
            sp_func_node_iter++)
        {
            Factor_graph_Node *temp_var = createFG_var_node(*sp_func_node_iter);
            Factor_graph_Node *temp_func = createFG_func_node(*sp_func_node_iter);
            temp_var->append_ptr_list(*sp_func_node_iter);
            temp_func->append_ptr_list(*sp_func_node_iter);
        }
    }
}

void CHelper :: Get_vars_of_func_and_form_var_nodes(CDAGNode* root, CDAGNode* function_node, set<CDAGNode*>& vars_of_a_func)
{
    if(root->Get_DAG_node_type()==false)    //i.e. a variable node in the DAG.......
    {
        Factor_graph_Node* temp = createFG_var_node(root);
        temp->append_ptr_list(function_node);
        vars_of_a_func.insert(root);
    }
    else
    {
        list<CDAGNode*> :: iterator iout = root->Get_DAGnode_outlist();
        unsigned int out_list_size = root->Get_DAGnode_outlist_size();
        for(unsigned int i=0;i<out_list_size; i++, iout++)
        {
            Get_vars_of_func_and_form_var_nodes((*iout), function_node, vars_of_a_func);
        }
    }
}

Factor_graph_Node* CHelper :: createFG_var_node(CDAGNode* var_address)
{
    char hash_fn_ip[10] = " " ;
    char temp_char[10];
    Factor_graph_Node* addr_FGnode;

    sprintf(temp_char , "%x", var_address);
    string temp(temp_char);
    strcpy (hash_fn_ip, temp.c_str());

    if(FG_HT.Check_FGnode_in_ht(hash_fn_ip))
    {
        addr_FGnode = FG_HT.Get_address_of_existing_FGnode_from_ht(hash_fn_ip, var_address);
    }
    else
    {
        addr_FGnode = new Factor_graph_Node(var_address, false);
        FG_HT.Insert_FGnode_in_ht(hash_fn_ip , addr_FGnode);
    }
    return addr_FGnode;
}

Factor_graph_Node* CHelper :: createFG_func_node(CDAGNode* func_address)
{
    char hash_fn_ip[10] = " " ;
    Factor_graph_Node* addr_FGnode;

    char temp_char[10];
    sprintf(temp_char , "%x", func_address);
    string temp(temp_char);
    strcpy (hash_fn_ip, temp.c_str());

    if(FG_HT.Check_FGnode_in_ht(hash_fn_ip))
    {
        addr_FGnode = FG_HT.Get_address_of_existing_FGnode_from_ht(hash_fn_ip, func_address);
    }
    else
    {
        addr_FGnode = new Factor_graph_Node(func_address, true);
        FG_HT.Insert_FGnode_in_ht(hash_fn_ip , addr_FGnode);
    }
    return addr_FGnode;
}

CDAGNode* CHelper :: Global_Scope_reduction(CDAGNode* root)
{
    CDAGNode* return_value;
    map < CDAGNode* , set<string> > exists_source_destination;  //store in the format:: address of the exists node and
                                                                //the variables it has to quantify......
    list <CDAGNode*> scope_reduction_order;
    scope_reduction_order.clear();  //initialization......at start.....
    exists_source_destination.clear();  //initialization......at start.....

    find_and_store_ex_quant_from_global_DAG(root , exists_source_destination, scope_reduction_order);
    if(scope_reduction_order.empty())
    {
        return_value = root ;
    }
    else
    {
        return_value = Actual_Global_Scope_reduction(root, exists_source_destination, scope_reduction_order );
        FG_HT.Release_FG_memory();
        //garbage_collector();
    }
    return return_value;
}

CDAGNode* CHelper :: Actual_Global_Scope_reduction( CDAGNode* root                                               ,
                                                    map < CDAGNode* , set<string> >& exists_source_destination   ,
                                                    list <CDAGNode*>& scope_reduction_order                      )
{
    unsigned int node_outlist_size = root->Get_DAGnode_outlist_size();
    //iterator to the node outlist......
    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();;

    CDAGNode* node_after_sr = root;

    switch(node_outlist_size)
    {
        case 0:
        {
            break;  //do nothing...................
        }
        case 1:
        {
            CDAGNode    *op1;
            CDAGNode *temp_op1;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = Actual_Global_Scope_reduction((*iout), exists_source_destination, scope_reduction_order );

            if(temp_op1!=op1)
            {
                node_after_sr = createDAG(root->Get_DAGnode_value(),op1);
                temp_op1->Delete_single_entry_from_DAG_inlist(root);

                if(root->isroot()==true)
                {
                    root->mark_redendant_node_for_deletion();
                }
            }
            break;
        }
        case 2:
        {
            CDAGNode *op1, *op2;
            CDAGNode *temp_op1, *temp_op2;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = Actual_Global_Scope_reduction((*iout), exists_source_destination, scope_reduction_order );
            iout++;
            temp_op2 = *iout;
            op2 = Actual_Global_Scope_reduction((*iout), exists_source_destination, scope_reduction_order );

            if(exists_source_destination.find(root)!=exists_source_destination.end())  //i.e. if the node is an exists node.....
            {
                node_after_sr = Scope_reduction(op2, exists_source_destination[root]);

                if(root->isroot()==true)
                {
                    root->mark_redendant_node_for_deletion();
                }
                temp_op1->Delete_single_entry_from_DAG_inlist(root);
                temp_op2->Delete_single_entry_from_DAG_inlist(root);

                if(node_after_sr!=op2)
                {
                    if(op2->isroot()==true)
                    {
                        op2->mark_redendant_node_for_deletion();
                    }
                    unsigned int ol_size = op2->Get_DAGnode_outlist_size();
                    list<CDAGNode*>::iterator li = op2->Get_DAGnode_outlist();
                    for(unsigned int i=0;i<ol_size;i++, li++)
                    {
                        (*li)->Delete_single_entry_from_DAG_inlist(op2);
                    }
                }

                scope_reduction_order.remove(root);
            }
            else
            {
                if((temp_op1!=op1)||(temp_op2!=op2))
                {
                    if(op1==op2)
                    {
                        node_after_sr = op1;  //to eliminate duplicate nodes.......
                    }
                    else
                    {
                        node_after_sr = createDAG(root->Get_DAGnode_value(),op1, op2);
                    }

                    if(root->isroot()==true)
                    {
                        root->mark_redendant_node_for_deletion();
                    }
                    temp_op1->Delete_single_entry_from_DAG_inlist(root);
                    temp_op2->Delete_single_entry_from_DAG_inlist(root);
                }
            }
            break;
        }
        case 3:
        {
            CDAGNode    *op1, *op2, *op3;
            CDAGNode *temp_op1, *temp_op2, *temp_op3;
            iout = root->Get_DAGnode_outlist();
            temp_op1 = *iout;
            op1 = Actual_Global_Scope_reduction((*iout), exists_source_destination, scope_reduction_order );
            iout++;
            temp_op2 = *iout;
            op2 = Actual_Global_Scope_reduction((*iout), exists_source_destination, scope_reduction_order );
            iout++;
            temp_op3 = *iout;
            op3 = Actual_Global_Scope_reduction((*iout), exists_source_destination, scope_reduction_order );

            if((temp_op1!=op1)||(temp_op2!=op2)||(temp_op3!=op3))
            {
                node_after_sr = createDAG(root->Get_DAGnode_value(),op1, op2, op3);
                temp_op1->Delete_single_entry_from_DAG_inlist(root);
                temp_op2->Delete_single_entry_from_DAG_inlist(root);
                temp_op3->Delete_single_entry_from_DAG_inlist(root);

                if(root->isroot()==true)
                {
                    root->mark_redendant_node_for_deletion();
                }
            }
            break;
        }
    }
    return node_after_sr;
}

CDAGNode* CHelper :: Scope_reduction(CDAGNode* root, set<string>& vars_to_ex_quantify)
{
    CDAGNode* return_value;
    set<string> vars_of_DAG;
    vars_of_DAG.clear();
    root->getDAGNames(vars_of_DAG);

    set<string> vars_to_exQ_in_DAG;
    vars_to_exQ_in_DAG.clear();
    
    for(set<string> :: iterator i=vars_to_ex_quantify.begin();i!=vars_to_ex_quantify.end();i++)
    {
        if(vars_of_DAG.find(*i)!=vars_of_DAG.end())
        {
            vars_to_exQ_in_DAG.insert(*i);
        }
    }

    if(vars_to_exQ_in_DAG.size() > 0)
    {
        form_factor_graph(root);    //form factor graph...............

        while(vars_to_exQ_in_DAG.size() > 0)
        {
            Factor_graph_Node *FG_node_to_merge = FG_HT.Get_FG_var_node_with_min_func(vars_to_exQ_in_DAG);

//cout << FG_node_to_merge->Get_FGnode_value()->Get_DAGnode_value() << FG_node_to_merge->Get_FG_pointee_size() << endl;


            CDAGNode* new_func_node_data = Merge_FG_nodes(FG_node_to_merge);
        }
        return_value = Actual_scope_reduction();
    }
    else
    {
        return_value = root ;
    }
    
    return return_value;
}

void CHelper :: Delete_a_var_from_FG(Factor_graph_Node* FG_node, Factor_graph_Node* new_func_node_addr)
{
    set<CDAGNode*>::iterator itr;
    set<Factor_graph_Node*> FG_addr_set, FG_func_var_set;
    set<CDAGNode*> pointee_for_new_func_node;

    pointee_for_new_func_node.clear();

    itr = FG_node->Get_FGnode_plist();
    FG_addr_set.clear();
    for(unsigned int i=0;i<FG_node->Get_FG_pointee_size();i++, itr++)   //get FG addr for variable pointee list.....
    {
        char hash_fn_ip[10] = " " ;
        char temp_char[10];
        sprintf(temp_char , "%x", *itr);
        string temp(temp_char);
        strcpy (hash_fn_ip, temp.c_str());
        Factor_graph_Node *temp_FG_addr = FG_HT.Get_address_of_existing_FGnode_from_ht(hash_fn_ip, *itr);

        FG_addr_set.insert(temp_FG_addr);   //factor graph addresses corresponding to the pointee entries.....
        temp_FG_addr->Delete_single_entry_from_plist(FG_node->Get_FGnode_value());//deleting old variable entry....        
    }

    FG_func_var_set.clear();
    for(set<Factor_graph_Node*>::iterator temp_FG_addr_set_iter = FG_addr_set.begin();
        temp_FG_addr_set_iter != FG_addr_set.end(); temp_FG_addr_set_iter++)
    {
        set<CDAGNode*>::iterator temp_itr = (*temp_FG_addr_set_iter)->Get_FGnode_plist();

        for(unsigned int k = 0; k<(*temp_FG_addr_set_iter)->Get_FG_pointee_size(); k++, temp_itr++)
        {
            char hash_fn_ip[10] = " " ;
            char temp_char[10];
            sprintf(temp_char , "%x", *temp_itr);
            string temp(temp_char);
            strcpy (hash_fn_ip, temp.c_str());
            Factor_graph_Node *temp_FG_addr = FG_HT.Get_address_of_existing_FGnode_from_ht(hash_fn_ip, *temp_itr);

            FG_func_var_set.insert(temp_FG_addr);
            temp_FG_addr->Delete_single_entry_from_plist((*temp_FG_addr_set_iter)->Get_FGnode_value());
        }
    }
        
    for(set<Factor_graph_Node*>::iterator temp_FG_addr_set_iter = FG_func_var_set.begin();
        temp_FG_addr_set_iter != FG_func_var_set.end(); temp_FG_addr_set_iter++)
    {
        (*temp_FG_addr_set_iter)->append_ptr_list(new_func_node_addr->Get_FGnode_value());
        pointee_for_new_func_node.insert((*temp_FG_addr_set_iter)->Get_FGnode_value());
    }

    for(set<CDAGNode*>::iterator temp_FG_addr_set_iter = pointee_for_new_func_node.begin();
        temp_FG_addr_set_iter != pointee_for_new_func_node.end(); temp_FG_addr_set_iter++)
    {
        new_func_node_addr->append_ptr_list(*temp_FG_addr_set_iter);
    }
    
    //clear plist of the concerned variable and the functions associated with the variable.............
    FG_node->Clear_plist();
    for(set<Factor_graph_Node*>::iterator temp_FG_addr_set_iter = FG_addr_set.begin();
        temp_FG_addr_set_iter != FG_addr_set.end(); temp_FG_addr_set_iter++)
    {
        (*temp_FG_addr_set_iter)->Clear_plist();
    }
}

CDAGNode* CHelper :: Merge_FG_nodes(Factor_graph_Node* FG_node)
{
    CDAGNode* return_type;
    
    switch(FG_node->Get_FG_pointee_size())
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            set<CDAGNode*>::iterator itr = FG_node->Get_FGnode_plist();
            return_type = createDAG("exists", FG_node->Get_FGnode_value(), *itr);

            Factor_graph_Node *FG_func_node = createFG_func_node(return_type);
            Delete_a_var_from_FG(FG_node, FG_func_node);
            break;
        }
        case 2:
        {
            

            CDAGNode* temp_final;
            set<CDAGNode*>::iterator itr1 = FG_node->Get_FGnode_plist();
            set<CDAGNode*>::iterator itr2 = FG_node->Get_FGnode_plist(); itr2++;
            temp_final = createDAG("and", *itr1, *itr2);
            return_type = createDAG("exists", FG_node->Get_FGnode_value(), temp_final);

            Factor_graph_Node *FG_func_node = createFG_func_node(return_type);
            Delete_a_var_from_FG(FG_node, FG_func_node);
            break;
        }
        default:
        {
            

            CDAGNode* temp_temp_final, *temp_final;
            set<CDAGNode*>::iterator itr1 = FG_node->Get_FGnode_plist();
            set<CDAGNode*>::iterator itr2 = FG_node->Get_FGnode_plist(); itr2++;
            set<CDAGNode*>::iterator itr3 = FG_node->Get_FGnode_plist(); itr3++; itr3++;
            temp_temp_final = createDAG("and", *itr1, *itr2);

            for(unsigned int cnt = 2; cnt < FG_node->Get_FG_pointee_size(); cnt++, itr3++)
            {
                temp_final = createDAG("and", *itr3, temp_temp_final);
                temp_temp_final = temp_final;
            }
            return_type = createDAG("exists", FG_node->Get_FGnode_value(), temp_final);
            
            Factor_graph_Node *FG_func_node = createFG_func_node(return_type);
            Delete_a_var_from_FG(FG_node, FG_func_node);
            break;
        }
    }
    return return_type;
}


CDAGNode* CHelper :: Actual_scope_reduction(void)
{
    CDAGNode* return_value;
    set<CDAGNode*> good_functions;
    good_functions.clear();

    FG_HT.Get_scope_reduced_funcs(good_functions);

    switch(good_functions.size())
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            set<CDAGNode*>::iterator itr = good_functions.begin();
            return_value = *itr;
            break;
        }
        case 2:
        {
            set<CDAGNode*>::iterator itr1 = good_functions.begin();
            set<CDAGNode*>::iterator itr2 = good_functions.begin();
            itr2++;
            return_value = createDAG("and", *itr1, *itr2);
            break;
        }
        default:
        {
            CDAGNode* temp_temp_final, *temp_final;
            set<CDAGNode*>::iterator itr1 = good_functions.begin();
            set<CDAGNode*>::iterator itr2 = good_functions.begin(); itr2++;
            set<CDAGNode*>::iterator itr3 = good_functions.begin(); itr3++; itr3++;
            temp_temp_final = createDAG("and", *itr1, *itr2);

            for(unsigned int cnt = 2; cnt < good_functions.size(); cnt++, itr3++)
            {
                temp_final = createDAG("and", *itr3, temp_temp_final);
                temp_temp_final = temp_final;
            }
            return_value = temp_final;
            break;
        }
    }
    return return_value;
}


CDAGNode* CHelper :: equivalentReduction(CDAGNode* root)
{
  //cout<<"\n\nBEFORE SCOPE REDUCTION = "<<root->getDAGStr()<<endl<<endl;
  
  CDAGNode* ScopeReduced = Global_Scope_reduction(root);

  //cout<<"\n\nAFTER SCOPE REDUCTION = "<<ScopeReduced->getDAGStr()<<endl<<endl;

  return ScopeReduced;
}


CDAGNode* CHelper :: findMinDeltaDelay(CDAGNode* root, int &MinDeltaDelay)
    {
      
      int dtime;
      if(root->Get_DAG_node_type() == false)  //i.e. operand
	{
	  dtime = root->Get_DAGnode_delta_time_as_int();
	  if(dtime<MinDeltaDelay) MinDeltaDelay = dtime;
	}

    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
    unsigned int outlist_size = root->Get_DAGnode_outlist_size();

    for(unsigned int i=0; i<outlist_size; i++, iout++)
      {
        findMinDeltaDelay(*iout, MinDeltaDelay);
	  }
    }


CDAGNode* CHelper :: findMaxDeltaDelay(CDAGNode* root, string signal, int &MaxDeltaDelay)
    {
      
      int dtime;
      string value;


      if(root->Get_DAG_node_type() == false)  //i.e. operand
	{
	  value = root->Get_DAGnode_value();
	  if(value == signal)
	    {
	      dtime = root->Get_DAGnode_delta_time_as_int();
	      if(dtime>MaxDeltaDelay) MaxDeltaDelay = dtime;
	    }
	}

    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist(); 
    unsigned int outlist_size = root->Get_DAGnode_outlist_size();

    for(unsigned int i=0; i<outlist_size; i++, iout++)
      {
        findMaxDeltaDelay(*iout, signal, MaxDeltaDelay);
	  }
    }


/*void CHelper :: getNewDeltas(CDAGNode* root, string output, set<int> DeltasIncluded, set<int> &NewDeltas)
{
  
  int dtime;
  string value;
  
      if(root->Get_DAG_node_type() == false)  //i.e. operand
	{
	  value = root->Get_DAGnode_value();
	  dtime = root->Get_DAGnode_delta_time_as_int();
	  
	  if(value==output && dtime>0 && DeltasIncluded.find(dtime) == DeltasIncluded.end())
	    NewDeltas.insert(dtime);
	}

    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();  
    unsigned int outlist_size = root->Get_DAGnode_outlist_size();

    for(unsigned int i=0; i<outlist_size; i++, iout++)
      {
        getNewDeltas(*iout, output, DeltasIncluded, NewDeltas);
      }
}*/

bool CHelper :: NewDeltaCheck(string value, int dtime, set<int> DeltasIncluded, set<string> output_inter_id_names)
{
  set<string>::iterator it = output_inter_id_names.begin();
  for(;it != output_inter_id_names.end(); it++)
    {
      string signal = *it;
      if(value==signal && dtime>0 && DeltasIncluded.find(dtime) == DeltasIncluded.end()) return true;
    }
  return false;
}

void CHelper :: getNewDeltasModified(CDAGNode* root, set<string> output_inter_id_names, set<int> DeltasIncluded, set<int> &NewDeltas)
{
  
  int dtime;
  string value;
  
      if(root->Get_DAG_node_type() == false)  //i.e. operand
	{
	  value = root->Get_DAGnode_value();
	  dtime = root->Get_DAGnode_delta_time_as_int();
	  
	  if(NewDeltaCheck(value, dtime, DeltasIncluded, output_inter_id_names))
	    NewDeltas.insert(dtime);
	}

    list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();  
    unsigned int outlist_size = root->Get_DAGnode_outlist_size();

    for(unsigned int i=0; i<outlist_size; i++, iout++)
      {
        getNewDeltasModified(*iout, output_inter_id_names, DeltasIncluded, NewDeltas);
      }
}
	    


CDAGNode* CHelper :: cloneWithDeltasAtLimit(CDAGNode* parent, int low, int high, set<string> names)
    {
        CDAGNode* clone;
                   
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = cloneWithDeltasAtLimit(*iout, low, high, names);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = cloneWithDeltasAtLimit(*iout, low, high, names);
		    iout++;
                    child_2 = cloneWithDeltasAtLimit(*iout, low, high, names);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = cloneWithDeltasAtLimit(*iout, low, high, names);
		    iout++;
                    child_2 = cloneWithDeltasAtLimit(*iout, low, high, names);
		    iout++;
                    child_3 = cloneWithDeltasAtLimit(*iout, low, high, names);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
            }
        }
        else //operand node
        {
	  int dtime = parent->Get_DAGnode_delta_time_as_int();;
	  string value = parent->Get_DAGnode_value();
	  
	  if(names.find(value) != names.end())
	    {

	      if(dtime>high) 
		{
		  //cout<<"\nReplacing "<<value<<" at delta "<<dtime<<" by "<<high<<endl;
		  clone = createDAG( parent->Get_DAGnode_value(), high);
		}
	      else if(dtime<low)  
		{
		  //cout<<"\nReplacing "<<value<<" at delta "<<dtime<<" by "<<low<<endl;
		  clone = createDAG( parent->Get_DAGnode_value(), low);
		}
	      else clone = createDAG( parent->Get_DAGnode_value(), parent->Get_DAGnode_delta_time_as_int());

	    }
	  else
	    {
	      
	      clone = createDAG( parent->Get_DAGnode_value(), parent->Get_DAGnode_delta_time_as_int());
	    }
        }        
        return clone;
    }    



CDAGNode* CHelper :: unrollTransitionRelation(CDAGNode* root, int times)
{
 CDAGNode *final=NULL;
  
for(int i=0; i<times; i++)
	{
	  // Fn: replaced by a DP enabled one
	//CDAGNode *copy = cloneAndShift(root, i);
	 CDAGNode *copy = callCloneAndShiftWithDP(root, i);

	//cout<<"\ncopy = "<<copy->getDAGStr()<<endl;
	if(final==NULL) final=copy; else final=createDAG("and", final, copy);
	//cout<<"\nfinal = "<<final->getDAGStr()<<endl;
	}
return final;
}

// Added on 29 Dec 2009 4pm 
CDAGNode* CHelper :: convertEquationToProperForm(CDAGNode* root)
{
  // root is equivalent(lhs, rhs)
  // rhs is like ite(codn1, ite(codn2,s1,s2), s3) and lhs is say X
  // output should be ite(codn1, ite(codn2,X=s1,X=s2), X=s3)

  string dag_str = root->Get_DAGnode_value();
  if(dag_str != "equivalent") {cout<<"\nconvertEquationToProperForm called with root->name "<<dag_str<<endl; exit(1);}
  else
    {
      list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
      CDAGNode *lhs = *iout;
      iout++;
      CDAGNode *rhs = *iout;
      
      CDAGNode *Equation = getCloneOfEquationInProperForm(lhs, rhs);
      return Equation;
    }
  
}


CDAGNode* CHelper :: getCloneOfEquationInProperForm(CDAGNode* lhs, CDAGNode* rhs)
{

  CDAGNode* clone;

  string dag_str = rhs->Get_DAGnode_value();

  if(dag_str == "ite")
    {
      list<CDAGNode*>::iterator iout = rhs->Get_DAGnode_outlist();
           
      CDAGNode* child_1 ;
      CDAGNode* child_2 ;
      CDAGNode* child_3 ;

      child_1 = *iout;
      iout++;
      child_2 = getCloneOfEquationInProperForm(lhs, *iout);
      iout++;
      child_3 = getCloneOfEquationInProperForm(lhs, *iout);

      clone = createDAG("ite", child_1, child_2, child_3);
      
    }
  else 
    {
      clone = createDAG("equivalent", lhs, rhs);
    }

  return clone;
}



  
    
CDAGNode* CHelper :: getLinearizedClone(CDAGNode* parent, map<CDAGNode*, CDAGNode*> &LinearizationTable, int &count, map<string, int> &WidthTable, set<string> &VarsToElimOriginal, set<string> &VarsToElimRenamed)
    {
        CDAGNode* clone;

	if(parent->Get_DAGnode_value() == "select") // select node encountered
	  {
	    map<CDAGNode*, CDAGNode*>::iterator it = LinearizationTable.find(parent);
	    
	    if(it != LinearizationTable.end()) // parent is already added to the Table. get the address of the new node and return it
	      {
		clone = it->second;
	      }
	    else // parent is newly encountered. Create a new node and return it's address
	      {
		string new_name = "autoname";
		count++;
		int new_delta_time = count;
		char char_ptr[10] = " ";
		sprintf(char_ptr , "%d", new_delta_time);
		string dt(char_ptr);
		string new_id_delta = new_name;
		new_id_delta +="_";
		new_id_delta += dt; // new_id_delta created
		clone = createDAG(new_name, new_delta_time);
		LinearizationTable.insert(make_pair(parent, clone)); // inserted into LinearizationTable

		
		list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
		CDAGNode* child_1 = *iout;
		iout++;
		CDAGNode* child_2 = *iout;
		iout++;
		CDAGNode* child_3 = *iout;

		// Updating VarsToElimRenamed

		string id_delta = child_1->Get_DAGnode_data_as_id_delta();
		if(VarsToElimOriginal.find(id_delta) != VarsToElimOriginal.end()) // variable is to be eliminated
		  {		    
		    VarsToElimRenamed.insert(new_id_delta);
		    //VarsToElimOriginal.erase(id_delta); // Newly added on 8th Jan 2010
		  }		    


		// For getting the size
		string lower = child_2->Get_DAGnode_value();
		string upper = child_3->Get_DAGnode_value();
		int l = atoi(lower.c_str());
		int u = atoi(upper.c_str());
		int size = u - l + 1;

		if(size<=0) {cout<<"\nSize of newly generated bit-vector non-positive inside function getLinearizedClone\n"; exit(1);}

		WidthTable.insert(make_pair(new_id_delta, size));
	      }
	  }
                   
        else if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = getLinearizedClone(*iout, LinearizationTable, count, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = getLinearizedClone(*iout, LinearizationTable, count, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    iout++;
                    child_2 = getLinearizedClone(*iout, LinearizationTable, count, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = getLinearizedClone(*iout, LinearizationTable, count, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    iout++;
                    child_2 = getLinearizedClone(*iout, LinearizationTable, count, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    iout++;
                    child_3 = getLinearizedClone(*iout, LinearizationTable, count, WidthTable, VarsToElimOriginal, VarsToElimRenamed);

		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
	    }
	}
        
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               parent->Get_DAGnode_delta_time_as_int());
        } 
    
    
        return clone;
    }        



CDAGNode* CHelper :: getPreprocessedClone(CDAGNode* parent)
    {
        CDAGNode* clone;

	if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = getPreprocessedClone(*iout);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = getPreprocessedClone(*iout);
		    iout++;
                    child_2 = getPreprocessedClone(*iout);

		    if(parent->Get_DAGnode_value() == "=")
		      clone = createDAG("equivalent", child_1, child_2);
		    else if(parent->Get_DAGnode_value() == "not_equal_to")
		      clone = createDAG("is_not_equal", child_1, child_2);
		    else
		      clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = getPreprocessedClone(*iout);
		    iout++;
                    child_2 = getPreprocessedClone(*iout);
		    iout++;
                    child_3 = getPreprocessedClone(*iout);

		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
	    }
	}
        
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               parent->Get_DAGnode_delta_time_as_int());
        } 
    
    
        return clone;
    }        



CDAGNode* CHelper :: cloneWithReplaceVariablesModified(CDAGNode* parent, map<string, int> MaxDeltaMap, int &count, map<CDAGNode*,CDAGNode*> &DPTable, map<string, string> &ReplacedVarsMap)
    {
        CDAGNode* clone;
                   
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = cloneWithReplaceVariablesModified(*iout, MaxDeltaMap, count, DPTable, ReplacedVarsMap);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = cloneWithReplaceVariablesModified(*iout, MaxDeltaMap, count, DPTable, ReplacedVarsMap);
		    iout++;
                    child_2 = cloneWithReplaceVariablesModified(*iout, MaxDeltaMap, count, DPTable, ReplacedVarsMap);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = cloneWithReplaceVariablesModified(*iout, MaxDeltaMap, count, DPTable, ReplacedVarsMap);
		    iout++;
                    child_2 = cloneWithReplaceVariablesModified(*iout, MaxDeltaMap, count, DPTable, ReplacedVarsMap);
		    iout++;
                    child_3 = cloneWithReplaceVariablesModified(*iout, MaxDeltaMap, count, DPTable, ReplacedVarsMap);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
            }
        }
        else //operand node
        {
	  map<CDAGNode*,CDAGNode*>::iterator dpit = DPTable.find(parent);

	  if(dpit != DPTable.end())
	    {
	      clone = dpit->second;
	    }
	  else
	    {
	      int dtime = parent->Get_DAGnode_delta_time_as_int();;
	      string value = parent->Get_DAGnode_value();

	      if(MaxDeltaMap.find(value) != MaxDeltaMap.end())
		{
		  int MaxDelta = MaxDeltaMap[value];
		  
		  if(dtime>0 && dtime<MaxDelta)
		    {
		      char temp[100];
		      sprintf(temp, "%d", count);
		      string count_str(temp);
		      clone = createDAG( "X_"+count_str, 0);
		      count++;

		      //cout<<"\nReplacing "<<value<<"_"<<dtime<<" by X_"<<count_str<<"_"<<0<<endl;

		      ReplacedVarsMap.insert(make_pair(clone->Get_DAGnode_value(), parent->Get_DAGnode_value()));
		    } 
		  else
		    {
		      clone = createDAG( parent->Get_DAGnode_value(), parent->Get_DAGnode_delta_time_as_int());
		    }
		}
	      
	      else
		{
	      
		  clone = createDAG( parent->Get_DAGnode_value(), parent->Get_DAGnode_delta_time_as_int());
		}
	      
	      DPTable.insert(make_pair(parent, clone));
	    }
	  
	}        
      return clone;
    }    



CExprSimpNode* CHelper :: convertTreeToDAG(CDAGNode *tree, map<CDAGNode*, CExprSimpNode*> &DPTable)
{

  map<CDAGNode*, CExprSimpNode*>::iterator it = DPTable.find(tree);

  if(it != DPTable.end())
    {
      return it->second;
    }
  
  CExprSimpNode *n_ptr = new CExprSimpNode();
  string name = tree->Get_DAGnode_value();
  int delta_time = tree->Get_DAGnode_delta_time_as_int();
  bool ntype = tree->Get_DAG_node_type();

   if(!ntype) // AP
    {
    
      char temp[10];
      sprintf(temp, "%d", delta_time);
      name += "_";
      name += temp;
       
      n_ptr->name = name;
    }

  else
    {
     			
      if(name == "select")
	{
	  name = "Sel";
	}

      else if(name == "&")// symbol for concatenation in VHDL is &
	{
	  name =  "Concat";
	}
      else if(name == "not")
	{
	  name =  "LOG_NOT";
	}
      else if(name == "and")
	{
	  name =  "Log_And";
	}
      else if(name == "or")
	{
	  name =  "Log_Or";
	} 
      else if(name == "ite")
	{
	  name =  "Ternary";
	} 
      else if(name == "equivalent")
	{
	  name =  "Log_Equal";
	}
      else if(name == "=")
	{
	  name =  "Log_Equal";
	}
      else if(name == "is_not_equal")
	{
	  name =  "Log_Inequal";
	} 
      else if(name == "not_equal_to")
	{
	  name =  "Log_Inequal";
			  } 
      else if(name == "-")
	{
	  name =  "Sub";
	} 
      else if(name == "+")
	{
	  name =  "Add";
	} 
      else if(name == ">")
	{
	  name =  "Grt_than";
	} 
      else if(name == "<")
	{
	  name =  "Less_than";
	} 
      else if(name == ">=")
	{
	  name =  "Grt_Equal";
	} 
      else if(name == "<=")
	{
	  name =  "Less_Equal";
	} 
      else if(name == "*")
	{
	  name =  "Mul";
	} 
			
		
      else if(tree->Get_DAGnode_outlist_size() == 0)
	{
	  //cout<<"\nNumber "<<name<<" inside the function convertTreeToDAG in the file CExprSimpStructure.h\n";
	  
	 if(name == "true")
	      name = "1'b1";
	if(name == "false")
	      name = "1'b0";
	}
      else
	 {
	 cout<<"\nUnknown function "<<name<<" inside the function convertTreeToDAG in the file CSymbSimulator.cpp...Exiting\n";
	 exit(1);
	 }

    list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
    unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  CExprSimpNode* child1 = convertTreeToDAG(*iout, DPTable);
			      
	  n_ptr->name = name;

	  (n_ptr->out_list).push_back(child1);
	  ((child1)->in_list).push_back(n_ptr);

	  break;
	}
      case 2:
	{
	  CExprSimpNode *child1 = convertTreeToDAG(*iout, DPTable);
	  iout++;
	  CExprSimpNode *child2 = convertTreeToDAG(*iout, DPTable);

	  n_ptr->name = name;

	  (n_ptr->out_list).push_back(child1);
	  (n_ptr->out_list).push_back(child2);

	  ((child1)->in_list).push_back(n_ptr);
	  ((child2)->in_list).push_back(n_ptr);

	  break;
	}
      case 3:
	{
	  CExprSimpNode *child1 = convertTreeToDAG(*iout, DPTable);
	  iout++;
	  CExprSimpNode *child2 = convertTreeToDAG(*iout, DPTable);
	  iout++;
	  CExprSimpNode *child3 = convertTreeToDAG(*iout, DPTable);

	  n_ptr->name = name;

	  (n_ptr->out_list).push_back(child1);
	  (n_ptr->out_list).push_back(child2);
	  (n_ptr->out_list).push_back(child3);

	  ((child1)->in_list).push_back(n_ptr);
	  ((child2)->in_list).push_back(n_ptr);
	  ((child3)->in_list).push_back(n_ptr);

	  break;
	}
      case 0: // constants
	{
	  n_ptr->name = name;
	}
      }
    }

   
   DPTable.insert(make_pair(tree, n_ptr));  
   return n_ptr;
}

	
 CExprSimpNode* CHelper::convertCDAGNodeToCExprSimpNode(CDAGNode *tree)
 {
   map<CDAGNode*, CExprSimpNode*> DPTable;
   DPTable.clear();
   CExprSimpNode* n_ptr = convertTreeToDAG(tree, DPTable);
   return n_ptr;
 }



CDAGNode* CHelper::convertCExprSimpNodeToCDAGNode(CExprSimpNode *dag)
{
  string name = dag->name;
  list<CExprSimpNode*> out_list = dag->out_list;

  if(out_list.begin() == out_list.end()) // Terminal node
    {
      if(name.find("_") == string::npos) // Constant
	{
	  //cout<<"\nConstant = "<<name<<endl;
	  if(name == "1'b1")
	    name = "true";
	  if(name == "1'b0")
	    name = "false";
	  //cout<<"\nConstant = "<<name<<endl; 
	  return createDAG(name); 
	}
      else // Variable
	{
	  int index = name.find_last_of("_");
	  string id = name.substr(0, index); 
	  int delta = atoi(name.substr(index+1).c_str()); 
	  return createDAG(id, delta);
	}
    }
  else
    {
      if(name == "Sel")
	{
	  name = "select";
	}

      else if(name == "Concat")
	{
	  name =  "&";// & is the symbol for concatenation in VHDL
	}
      else if(name == "LOG_NOT")
	{
	  name =  "not";
	}
      else if(name == "Log_And")
	{
	  name =  "and";
	}
      else if(name == "Log_Or")
	{
	  name =  "or";
	} 
      else if(name == "Ternary")
	{
	  name =  "ite";
	} 
      else if(name == "Log_Equal")
	{
	  name =  "equivalent";
	} 
      else if(name == "Log_Inequal")
	{
	  name =  "is_not_equal";
	} 
      else if(name == "Exists")
	{
	  name =  "exists";
	} 
      else if(name == "Add")
	{
	  name =  "+";
	} 
      else if(name == "Sub")
	{
	  name =  "-";
	} 
      else if(name == "Grt_than")
	{
	  name =  ">";
	} 
      else if(name == "Less_than")
	{
	  name =  "<";
	} 
      else if(name == "Grt_Equal")
	{
	  name =  ">=";
	} 
      else if(name == "Less_Equal")
	{
	  name =  "<=";
	} 
      else if(name == "Mul")
	{
	  name =  "*";
	} 
	
			
      else
	{
	  cout<<"\nUnknown function "<<name<<" inside the function convertCExprSimpNodeToCDAGNode in the file CSymbSimulator.cpp...Exiting\n";
	  exit(1);
	}


      if(name == "not") // Unary operators
	{
	  list<CExprSimpNode*>::iterator it = out_list.begin();
			    
	  CDAGNode* child1 = convertCExprSimpNodeToCDAGNode(*it);

	  return createDAG(name, child1);
	}
			    
      else if(name == "&" || name == "or" || name == "and" || name == "equivalent" || name == "is_not_equal" || name == "exists" || name == "+" || name == "-" || name == ">" || name == "<" || name == ">=" || name == "<=" || name == "*") //binary operators
	{
	  list<CExprSimpNode*>::iterator it = out_list.begin();
			    
	  CDAGNode* child1 = convertCExprSimpNodeToCDAGNode(*it);
	  it++;
	  CDAGNode* child2 = convertCExprSimpNodeToCDAGNode(*it);

	  return createDAG(name, child1, child2);

	}
      else if(name == "ite" || name == "select")// operators with three arguments 
	{
	  list<CExprSimpNode*>::iterator it = out_list.begin();
	  CDAGNode* child1 = convertCExprSimpNodeToCDAGNode(*it);
	  it++;
	  CDAGNode* child2 = convertCExprSimpNodeToCDAGNode(*it);
	  it++;
	  CDAGNode* child3 = convertCExprSimpNodeToCDAGNode(*it);
			    
	  return createDAG(name, child1, child2, child3);
	}

    }
}

void CHelper::writeBVDeclarationsFromWidthTableInYicesFormat(FILE *smt_file, map<string, int> WidthTable)

{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {

	  fprintf(smt_file,"(define %s",(it->first).c_str());
	  fprintf(smt_file," :: (bitvector %d))\n",it->second);
	}
 
}


bool CHelper::checkUnast(CDAGNode* formula, map<string, int> WidthTable)
{
  FILE *check_file = fopen("check.ys", "w");

  if(check_file == NULL) 
   {cout<<"\nCannot create check.ys\n"; exit(1);}

  writeBVDeclarationsFromWidthTableInYicesFormat(check_file, WidthTable);
  //string formula_str = formula->getDAGOfProperSkeltonFormatInYicesFormat("bool"); 
  string formula_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(formula, WidthTable);
 
  fprintf(check_file,"\n(assert %s)", formula_str.c_str());
  fprintf(check_file,"\n(check)\n");
  
  fclose(check_file);

  system("yices -tc check.ys > result_check.txt");

  //cout<<"\nYices called\n";

  FILE *result_check_file = fopen("result_check.txt", "r");

   if(result_check_file == NULL) 
   {cout<<"\nCannot create result_check.txt\n"; exit(1);}


  char result[100];
  fscanf(result_check_file, "%s", result);
  fclose(result_check_file);
  string result_str(result);

  //cout<<"\nresult_str = "<<result_str<<endl;

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

// return 2 ^ number
int CHelper::findPower(int number)
{
  int count=0;
  int value=1;
  while(count<number) {count=count+1; value=value*2;}
  return value;
}


bool CHelper::checkForExactness(CDAGNode* original, CDAGNode* eliminated, map<string, int> WidthTable, list<string> VarsToElim)
{
  FILE *check_file = fopen("check.ys", "w");

  if(check_file == NULL) 
   {cout<<"\nCannot create check.ys\n"; exit(1);}


  writeBVDeclarationsFromWidthTableInYicesFormat(check_file, WidthTable);


  /* FOLLOWING TWO LINES WERE COMMENTED AND NEW ONES ADDED ON 18/1/2010 */

  //string original_str = original->getDAGOfProperSkeltonFormatInYicesFormat("bool"); 
  //string eliminated_str = eliminated->getDAGOfProperSkeltonFormatInYicesFormat("bool"); 

  string original_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(original, WidthTable);
  string eliminated_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(eliminated, WidthTable);

  /* ADDED ON 18/1/2010 ENDS HERE */

  fprintf(check_file,"\n(assert (and  %s (not %s)))", original_str.c_str(), eliminated_str.c_str());
  fprintf(check_file,"\n(check)\n");
  
  fclose(check_file);

  system("yices -tc check.ys > result_check.txt");

  //cout<<"\nYices called\n";

  FILE *result_check_file = fopen("result_check.txt", "r");
  
  if(result_check_file == NULL) 
   {cout<<"\nCannot create result_check.txt\n"; exit(1);}



  char result[100];
  fscanf(result_check_file, "%s", result);
  fclose(result_check_file);
  string result_str(result);

  //cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat")
    {
      cout<<"\nexists(X,f)=>f' holds\n";
      //return true;
    }
      
  else if(result_str=="sat")
    { cout<<"\nexists(X,f)=>f' does not hold\n";
      return false;
    }
  else
    {
      cout<<"\nUnknown result from Yices"<<result_str<<endl;
      exit(1);
    }

  
  // Code to experiment in CVC3

  bool experiment = true;
  if(experiment)
      {
	string result_str = checkImplicationInCVC(original, eliminated, WidthTable, VarsToElim);

	if(result_str=="unsat")
	  {
	    cout<<"\nf'=>exists(X,f) holds\n";
	    return true;
	  }
      
	else if(result_str=="sat")
	  { cout<<"\nf'=>exists(X,f) does not hold\n";
	  return false;
	  }
	else
	  {
	    cout<<"\nCVC3 returns "<<result_str<<endl;
	    exit(1);
	   }

      }
  //cout<<"\nTrying the values one by one\n";




  // Checking f' => exists(X, f) Trying the values one by one

  CDAGNode *dag = eliminated;

  original = createDAG("not", original);

  map<string, int> WidthsOfVarsToELim;
  map<string, int> RangesOfVarsToELim;

  int NoOfCombs = 1;

  for(list<string>::iterator it =  VarsToElim.begin(); it !=  VarsToElim.end(); it++)
    {
      string Var = *it;
      int Width = WidthTable[Var];
      int Range = findPower(Width);

      NoOfCombs = NoOfCombs*Range;
      
      WidthsOfVarsToELim.insert(make_pair(Var, Width));
      RangesOfVarsToELim.insert(make_pair(Var, Range));
    }


  list< vector<int> > ValuesConsidered;
  ValuesConsidered.clear();
  srand((unsigned)time(0));

  int iterations = 1;

  cout<<"\nNoOfCombs = "<<NoOfCombs<<endl;

  while(1)
    {
      //cout<<"\nInside First While Loop\n";
      cout<<"\nIteration Count = "<<iterations<<endl;

      if(iterations > NoOfCombs)  // all combinations considered
	{
	  cout<<"\nexists(X,f)<=f' does not hold\n";
	  return false;
	}
  
      map<string, string> ValueMap;
      while(1)
	{
	  //cout<<"\nInside Second While Loop\n";
	  vector<int> NewValues;
	  for(map<string, int>::iterator it =  RangesOfVarsToELim.begin(); it != RangesOfVarsToELim.end(); it++)
	    {
	      string Var = it->first;
	      int Range = it->second;

	      int random_no = rand()% Range;
	      //cout<<"\nVariable = "<<Var<<endl;
	      //cout<<"\nRandom no: in range "<<Range<<" is "<<random_no<<endl;
	      NewValues.push_back(random_no);
	    }

	  //showVector(NewValues, "NewValues");
	  //showListOfVectors(ValuesConsidered, "ValuesConsidered");

	  if(!ValuesConsidered.empty() && foundInValuesConsidered(ValuesConsidered, NewValues)) continue;

	  //cout<<"\nHERE1\n";
	     
	  ValuesConsidered.push_back(NewValues);

	  ValueMap.clear();	  
	     
	  map<string, int>::iterator it =  RangesOfVarsToELim.begin();
	  for(int i=0; i<NewValues.size(); i++, it++)
	       {
		 string Var = it->first;
		 char Value[20];
		 sprintf(Value, "%d", NewValues[i]);
		 string Value_str(Value);

		 ValueMap.insert(make_pair(Var, Value_str));
	       } 
	  break;
	}
      
      //cout<<"\nHERE\n";
      
      cout<<"\nModel Tried is\n";

      showMap(ValueMap, "ValueMap");

      CDAGNode *inlined = replaceVarsByValues(original, ValueMap);

      cout<<"\ninlined = "<<inlined->getDAGStr()<<endl;

      cout<<"\ninlined in Yices format = "<<callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(inlined, WidthTable)<<endl;
 	       
      dag = createDAG("and", dag, inlined);

      //cout<<"\ndag = "<<dag->getDAGStr()<<endl;
   
      if(checkUnast(dag, WidthTable))
      	{
      	  cout<<"\nexists(X,f)<=f' holds\n";
      	  return true;
      	}

      iterations++;
    }

}


  /*void CHelper::addNewValuesIntoValuesConsidered(set< vector<int> > &ValuesConsidered, vector<int> NewValues)
{
  vector<int> temp;
  
  int size = NewValues.size();

  for(int i=0;i<size;i++)
    {
      temp.push_back(NewValues[i]);
    }

  ValuesConsidered.insert(temp);
  }*/

bool CHelper::foundInValuesConsidered(list< vector<int> > ValuesConsidered, vector<int> NewValues)
{
  list< vector<int> >::iterator it = ValuesConsidered.begin();
  
  for(; it != ValuesConsidered.end(); it++)
    {
      vector<int> ExistingValues = *it;

      if(matchingVectors(ExistingValues, NewValues))
	return true;
    }

  cout<<"\nNewValues is new\n";
  return false;
}

bool CHelper::matchingVectors(vector<int> V1, vector<int> V2)
{
  int size = V1.size();

  for(int i=0;i<size;i++)
    {
      if(V1[i] != V2[i]) return false;
    }
  return true;
}



string CHelper::convertDecimalToBitvector(int number)
{
string bv="0b";
string str = integerToBinaryString(number);
bv += str;

return bv;
}


string CHelper::integerToBinaryString(int i)
{
    if(i==0)
        return "0";
    string bin_str="";
    int j=i;
    while(j!=0)
    {
        if(j%2 == 0)
            bin_str = bin_str + "0";
        else
            bin_str = bin_str + "1";
        j = j / 2;
    }
    string str="";
    //Reversing the binary string to bring it to final format
    for(int k=bin_str.size()-1;k>=0;k--)
        str += bin_str[k];
    return str;
}


CDAGNode* CHelper::replaceVarsByValues(CDAGNode *original, map<string, string> ValueMap)
  {
    CDAGNode *modified;
    set<string> VarsToElim;

    modified = original;

    for(map<string, string>::iterator it=ValueMap.begin(); it != ValueMap.end(); it++)
      {
	string id_delta = it->first;
	int index = id_delta.find_last_of("_");
	string id = id_delta.substr(0, index);
	string delta_string = id_delta.substr(index+1);

	int delta = atoi(delta_string.c_str());

	VarsToElim.insert(id_delta);

	string value = it->second;
	
	
	CDAGNode* temp_var = createDAG(id, delta);
	CDAGNode* temp_val = createDAG(value);

	CDAGNode *temp_eq = createDAG("equivalent", temp_var, temp_val);
	
	modified = createDAG("and", modified, temp_eq);
      }

    cout<<"\nmodified = "<<modified->getDAGStr()<<endl;

    showSet(VarsToElim,  "VarsToElim");

    CDAGNode* inlined = Inlining(modified, VarsToElim);

    cout<<"\nTrying to print the inlined expression"<<endl;

    cout<<"\ninlined = "<<inlined->getDAGStr()<<endl;

    
    
    return inlined;
  }


void CHelper::showSet(set<string> me, string WhoAmI)
{
cout<<"\n"<<WhoAmI<<"\n*****\n\n";
set<string>::iterator me_it;

for(me_it=me.begin(); me_it!=me.end(); me_it++)
	{
	cout<<(*me_it)<<"\n";
	}
}


void CHelper::showMap(map<string, string> me, string WhoAmI)
{
cout<<"\n"<<WhoAmI<<"\n*****\n\n";
 map<string, string>::iterator me_it;

for(me_it=me.begin(); me_it!=me.end(); me_it++)
	{
	  cout<<me_it->first<<"\t"<<me_it->second<<"\n";
	}
}

void CHelper::showVector(vector<int> MyVector, string WhoAmI)
{

  cout<<"\n"<<WhoAmI<<"\n*****\n\n";
int size = MyVector.size();
 cout<<"\n";
for(int i=0;i<size;i++)
  cout<<MyVector[i]<<"\t";
 cout<<"\n";
}

void CHelper::showListOfVectors(list< vector<int> >  MySet, string WhoAmI)
{
  list< vector<int> >::iterator it = MySet.begin();
  

  for(;it!=MySet.end();it++)
    {
      vector<int> MyVector = *it;
      showVector(MyVector, "Vector");
    }
}


void CHelper :: createWidthTableForOperators(CDAGNode *tree, map<CDAGNode*, int> &WidthTableForNodes, map<string, int> WidthTable)
{
  string name = tree->Get_DAGnode_value();
  int delta_time = tree->Get_DAGnode_delta_time_as_int();
  bool ntype = tree->Get_DAG_node_type();

   if(!ntype) // variable
    {
    
      char temp[10];
      sprintf(temp, "%d", delta_time);
      name += "_";
      name += temp;
       
      int Width =  WidthTable[name];
      WidthTableForNodes.insert(make_pair(tree, Width));
    }

  else
    {
     			
    list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
    unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  CDAGNode *child1 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);

	  if(name == "not")
	    {
	      int Width =  1;
	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	     cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::createWidthTableForOperators\n";
	     exit(1);
	    }
 
	 
	  break;
	}
      case 2:
	{
	  CDAGNode *child1 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  CDAGNode *child2 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	 
	  if(name == "equivalent" || name == "+" || name == "*" || name == "is_not_equal" || name == "&" || name == "-" || name == "<=" || name == ">=" || name == ">" || name == "<")// & stands for concat
	    {

	      int Width1 =  WidthTableForNodes[child1];
	      int Width2 =  WidthTableForNodes[child2];

	      int Width;

	      if(name == "equivalent" || name == "+" || name == "*" || name == "is_not_equal" || name == "-"|| name == "<=" || name == ">=" || name == ">" || name == "<")
		Width = findMaximum(Width1, Width2);
	      else // concat i.e. &
		Width = Width1 + Width2;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	  else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::createWidthTableForOperators\n";
	      exit(1);
	    }

	  break;
	}
      case 3:
	{
	  CDAGNode *child1 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  CDAGNode *child2 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  CDAGNode *child3 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);

	  if(name == "ite") // Width of an ite is in fact maximum of widths of child2 and child3
	    {
	      
	      //int Width = 1;

	       int Width1 =  WidthTableForNodes[child2];
	       int Width2 =  WidthTableForNodes[child3];

	      int Width = findMaximum(Width1, Width2);

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else if(name == "select")
	    {
	      int Width1 =  WidthTableForNodes[child2];
	      int Width2 =  WidthTableForNodes[child3];

	      int Width = Width2 - Width1 +1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	   else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::createWidthTableForOperators\n";
	      exit(1);
	    }

	 
	  break;
	}
      case 0: // constants
	{
	  if(name[0] == 'x') // hex
	    {
	      WidthTableForNodes.insert(make_pair(tree, (name.length()-3)*4));
	    }
	  else // 
	    {
	      int Width = findMinimumWidthRequired(name);
	      
	      //cout<<"\nname = "<<name<<"\t"<<"Width = "<<Width<<endl;

	      WidthTableForNodes.insert(make_pair(tree, Width));

	      

	      //WidthTableForNodes.insert(make_pair(tree, 0));
	    }
	}
      }

    }
}



string CHelper :: callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(CDAGNode *root, map<string, int> WidthTable)
{
  map<CDAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  // propogateWidths is newly added on 19.1.2010

  propogateWidths(root, WidthTableForNodes, 1);
  
  
  return root->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bool", WidthTableForNodes, 1);
}


int CHelper::findMaximum(int i, int j)
{
  if(i>=j) return i;
  else return j;
}


// In fact in our SEs, if we have x with width 15, it is included the dag as x(0:14). This causes problems while linearizing the dag. Hence this function finds those selection nodes which are unneeded. Function returns width of the expression.

int CHelper :: findUnneededSelections(CDAGNode *tree, set<CDAGNode*> &UnneededSelects, bool InsideSelection, map<string, int> WidthTable)
{
  string name = tree->Get_DAGnode_value();
  int delta_time = tree->Get_DAGnode_delta_time_as_int();
  bool ntype = tree->Get_DAG_node_type();

   if(!ntype) // variable
     {
      char temp[10];
      sprintf(temp, "%d", delta_time);
      name += "_";
      name += temp;

      int width =  WidthTable[name];
      return width;
    }

  else
    {
     			
    list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
    unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  CDAGNode *child1 = *iout;
	  int WidthOfChild1 = findUnneededSelections(child1, UnneededSelects, false, WidthTable);
	  return WidthOfChild1; // Bitwise not and unary minus
	  break;
	}
      case 2:
	{
	  int WidthOfChild1, WidthOfChild2;
	  CDAGNode *child1 = *iout;
	  WidthOfChild1 = findUnneededSelections(child1, UnneededSelects, false, WidthTable);
	  iout++;
	  CDAGNode *child2 = *iout;
	  WidthOfChild2 = findUnneededSelections(child2, UnneededSelects, false, WidthTable);

	  if(name == "equivalent" || name == "is_not_equal")
	    return 1;

	  else
	    return findMaximum(WidthOfChild1, WidthOfChild2);
	 
	  break;
	}
      case 3:
	{
	  int WidthOfChild1, WidthOfChild2, WidthOfChild3;
	  CDAGNode *child1 = *iout;
	  iout++;
	  CDAGNode *child2 = *iout;
	  iout++;
	  CDAGNode *child3 = *iout;
	  
	  if(name == "select")
	    {
	      WidthOfChild1 = findUnneededSelections(child1, UnneededSelects, false, WidthTable);
	      WidthOfChild2 = findUnneededSelections(child2, UnneededSelects, true, WidthTable);
	      WidthOfChild3 = findUnneededSelections(child3, UnneededSelects, true, WidthTable);
	      
	      int SizeOfSelection = WidthOfChild3 - WidthOfChild2 +1;
	      if(SizeOfSelection == WidthOfChild1) // Size of selection matches with the actual size. hence selection unneeded
		{
		  //cout<<"\nUnneeded selection found\n";
		  UnneededSelects.insert(tree);
		}

	      return SizeOfSelection;
	    }

	  else //ite
	    {
	      WidthOfChild1 = findUnneededSelections(child1, UnneededSelects, true, WidthTable);
	      WidthOfChild2 = findUnneededSelections(child2, UnneededSelects, true, WidthTable);
	      WidthOfChild3 = findUnneededSelections(child3, UnneededSelects, true, WidthTable);
	      return findMaximum(WidthOfChild2, WidthOfChild3); 
	    }
		
	  break;
	}
      case 0: // constants The important thing here is that only constants appearing inside selects should return their proper size
	{
	  if(InsideSelection)
	    {
	      return atoi(name.c_str()); 
	    }
	  else
	    return -1;
	}
      }

    }
}



CDAGNode* CHelper :: getSelectionsRemovedClone(CDAGNode* parent, set<CDAGNode*> UnneededSelects)
    {
        CDAGNode* clone;

	if(parent->Get_DAGnode_value() == "select") // select node encountered
	  {
	    list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();

	    if(UnneededSelects.find(parent) != UnneededSelects.end()) // selection is unneeded : create clone avoiding the constants
	      {
		//cout<<"\nUnneeded selection node encountered inside fn: getSelectionsRemovedClone\n";
		clone = getSelectionsRemovedClone(*iout, UnneededSelects);
		//cout<<"\nold One = "<<parent->getDAGStr()<<endl;
		//cout<<"\nNew One = "<<clone->getDAGStr()<<endl;
	      }
	    else
	      {
		  CDAGNode* child_1 ;
		  CDAGNode* child_2 ;
		  CDAGNode* child_3 ;

		  child_1 = getSelectionsRemovedClone(*iout, UnneededSelects);
		  iout++;
		  child_2 = getSelectionsRemovedClone(*iout, UnneededSelects);
		  iout++;
		  child_3 = getSelectionsRemovedClone(*iout, UnneededSelects);

		  clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
	      }
	      
	  }
                   
        else if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = getSelectionsRemovedClone(*iout, UnneededSelects);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = getSelectionsRemovedClone(*iout, UnneededSelects);
		    iout++;
                    child_2 = getSelectionsRemovedClone(*iout, UnneededSelects);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = getSelectionsRemovedClone(*iout, UnneededSelects);
		    iout++;
                    child_2 = getSelectionsRemovedClone(*iout, UnneededSelects);
		    iout++;
                    child_3 = getSelectionsRemovedClone(*iout, UnneededSelects);

		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
	    }
	}
        
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               parent->Get_DAGnode_delta_time_as_int());
        } 
    
    
        return clone;
    }        




/* NEW FUNCTIONS ADDED ON 19/1/2010 */

int CHelper :: findMinimumWidthRequired(string Number)
{
  if(Number == "'1'" || Number == "'0'" || Number == "true" || Number == "false")
    return 1;

  
  //cout<<"\nNumber= "<<Number<<endl;
  ULINT DecNumber = strtoull(Number.c_str(), NULL, 0);
  //int DecNumber = atoi(Number.c_str());
  //cout<<"\nDecNumber= "<<DecNumber<<endl;
  int Width = findMinimumWidthRequiredForNumber(DecNumber);
  return Width;
}


int CHelper :: findMinimumWidthRequiredForNumber(ULINT number)
{
  int width = 0;

  //cout<<"\nnumber is"<<number<<endl;

  if(number<0) {cout<<"\nint CHelper :: findMinimumWidthRequired(ULINT mumber) called with negative argument\n"; exit(1);}

  do
    {
      number = number/2;
      width++;

      //cout<<"\nnumber = "<<number<<"\twidth="<<width<<endl;
    }
  while(number>0);
  
  return width;
}



void CHelper :: propogateWidths(CDAGNode *tree, map<CDAGNode*, int> &WidthTableForNodes, int WidthFromParent)
{
  string name = tree->Get_DAGnode_value();
  int delta_time = tree->Get_DAGnode_delta_time_as_int();
  bool ntype = tree->Get_DAG_node_type();
  

   if(!ntype) // variable
    {
    
      // Do nothing : Variable's width should not be changed
    }

  else
    {
     			
    list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
    unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  int width = WidthTableForNodes[tree];
	  propogateWidths(*iout, WidthTableForNodes, width);

	  break;
	}
      case 2:
	{
	  if(name == "+" || name == "*")
	    {
	      WidthTableForNodes[tree] = WidthFromParent;
	    }

	  int width = WidthTableForNodes[tree];
	  propogateWidths(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths(*iout, WidthTableForNodes, width);
	 
	  break;
	}
      case 3:
	{
	  int width = WidthTableForNodes[tree];
	  propogateWidths(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths(*iout, WidthTableForNodes, width);
	 
	  break;
	}
      case 0: // constants
	{
	  // For all constants, width field is set
	  // But note that width field makes sense only for bit-vector constants
	  //WidthTableForNodes[tree] = WidthFromParent;	  
	}
      }

    }
}



void CHelper :: checkForPropSkelton(CDAGNode* parent)
    {
                
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();
	    string MyName = parent->Get_DAGnode_value();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 = *iout;
		    string child_1_name = child_1->Get_DAGnode_value();

		    if(MyName == "not")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of not node in the DAG\n";
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }
		    
		    // Do this for the child also

		    checkForPropSkelton(child_1);
		    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;

		    string child_1_name,child_2_name; 
                    
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;

		    child_1_name = child_1->Get_DAGnode_value();
		    child_2_name = child_2->Get_DAGnode_value();

		    if(MyName == "and" || MyName == "or")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(!Logical(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }

		    else if(MyName == "equivalent" || MyName == "+" || MyName == "*" || MyName == "is_not_equal" || MyName == "-")
		      {
			if(LogicalOperator(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    cout<<"\nNode is "<<parent->getDAGStr()<<endl;
			    exit(1);
			  }

			if(LogicalOperator(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    cout<<"\nNode is "<<parent->getDAGStr()<<endl;
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }

		    checkForPropSkelton(child_1);
		    checkForPropSkelton(child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

		    string child_1_name,child_2_name, child_3_name; 
                    
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;
		    iout++;
                    child_3 = *iout;

		    child_1_name = child_1->Get_DAGnode_value();
		    child_2_name = child_2->Get_DAGnode_value();
		    child_3_name = child_3->Get_DAGnode_value();

		    if(MyName == "ite")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(!Logical(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
			
			if(!Logical(child_3_name))
			 {
			    cout<<"\n"<<child_3_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }

		    else if(MyName == "select")
		      {
			if(LogicalOperator(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(LogicalOperator(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
			
			if(LogicalOperator(child_3_name))
			 {
			    cout<<"\n"<<child_3_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }

		    checkForPropSkelton(child_1);
		    checkForPropSkelton(child_2);
		    checkForPropSkelton(child_3);
                    
                    break;
                }
	    case 0:
	      {
		// Do nothing for constants
		break;
	      }
            }
        }
        else
        {
	  // Do nothing for variables
        }        
        
    }    


bool CHelper :: Logical(string MyName)
{
  if(MyName=="ite" || MyName=="and" || MyName=="not" || MyName=="or" || MyName=="equivalent" || MyName=="is_not_equal" || MyName=="1"|| MyName=="0"|| MyName=="true"|| MyName=="false" || MyName=="<=" || MyName==">=" || MyName==">" || MyName=="<")
    return true;
  else
    return false;
}

bool CHelper :: LogicalOperator(string MyName)
{
  if(MyName=="ite" || MyName=="and" || MyName=="not" || MyName=="or" || MyName=="equivalent" || MyName=="is_not_equal"  || MyName=="<=" || MyName==">=" || MyName==">" || MyName=="<")
    return true;
  else
    return false;
}



void CHelper::copySetToList(set<string> MySet, list<string> &MyList)
{
  
for(set<string>::iterator it=MySet.begin(); it!=MySet.end(); it++)
  {
    MyList.push_back(*it);
  }
}



CDAGNode* CHelper :: recreateSubtractionDAG(CDAGNode* parent, map<string, int> WidthTable)
{
  set<string> Names;
  list<string> Vars;
  list<ULINT> Coefs;
  
  int Width;
  ULINT Mod;
  list<string>::iterator it;


  //cout<<"\nOriginal DAG = "<<parent->getDAGStr()<<endl;
  
  parent->getDAGNames(Names);
  
  copySetToList(Names, Vars);

  it = Vars.begin();

  Width = WidthTable[*it];

  Mod = findPowerWithLargerRange(Width);

  //cout<<"\nWidth = "<<Width<<"\tMod = "<<Mod<<endl;
  
  for(it=Vars.begin(); it!=Vars.end(); it++)
    {
    ULINT Coef=0; 
    int Sign=0;

    parent->getCoefficientWithMinusSupported(*it, Coef, Sign, Mod);

    //cout<<"\nVar = "<<*it<<"\tCoef = "<<Coef<<endl;
      
    Coefs.push_back(Coef);
    }

  ULINT constant=0;
  int Sign=0;

  parent->getConstantModifiedWithMinusSupported(constant, Sign, Mod);

    
  CDAGNode* RecreatedNode = getDAGFromVarsCoefsAndConstant(Vars, Coefs, constant);

  //cout<<"\nRecreated DAG = "<<RecreatedNode->getDAGStr()<<endl;
  
  return RecreatedNode;
}

CDAGNode* CHelper :: getDAGFromVarsCoefsAndConstant(list<string> Vars, list<ULINT> Coefs, ULINT SelectedConst)
{
  	list<string>::iterator Vars_it;
	list<ULINT>::iterator Coefs_it;

        CDAGNode *rhs=NULL;

	Vars_it=Vars.begin();
	Coefs_it=Coefs.begin();
	
	for(; Vars_it != Vars.end(); Vars_it++, Coefs_it++)
	  {
	    string id_delta = *Vars_it;

	    //cout<<"\nid_delta = "<<id_delta<<endl;

	    int index = id_delta.find_last_of("_");
	    string id = id_delta.substr(0, index);
	    string delta = id_delta.substr(index+1);
	    int delta_int = atoi(delta.c_str());

	    CDAGNode *variable = createDAG(id, delta_int);

	    
	    ULINT Coef = *Coefs_it;

	    char coeff[20];
	    sprintf(coeff, "%llu", Coef);
	    string coeff_str(coeff);
	    CDAGNode *coefficient = createDAG(coeff_str);
	    CDAGNode *term = createDAG("*", coefficient, variable);
	    if(rhs==NULL) rhs=term; else rhs=createDAG("+", rhs, term);
	  }

	if(SelectedConst != 0)
	  {
	    char constant[20];
	    sprintf(constant, "%llu", SelectedConst);
	    string constant_str(constant);
	    CDAGNode *term = createDAG(constant_str);
	    if(rhs==NULL) rhs=term; else rhs=createDAG("+", rhs, term);
	  }

	return rhs;
}


void CHelper :: callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile(CDAGNode *root, map<string, int> WidthTable, FILE *yices_fp)
{
  map<CDAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  // propogateWidths is newly added on 19.1.2010

  propogateWidths(root, WidthTableForNodes, 1);
  
  
  root->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bool", WidthTableForNodes, 1, yices_fp);
}



CDAGNode* CHelper :: cloneAndShiftWithDP(CDAGNode* parent, int shift, map<CDAGNode*, CDAGNode*> *PtrDPTable)
    {
        CDAGNode* clone;

	map<CDAGNode*, CDAGNode*>::iterator DPTable_it;

	DPTable_it = PtrDPTable->find(parent);

	if(DPTable_it != (*PtrDPTable).end()) // DPTable hit
	  {
	    clone = DPTable_it->second;
	    return clone;
	  }
                   
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = cloneAndShiftWithDP(*iout, shift, PtrDPTable);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = cloneAndShiftWithDP(*iout, shift, PtrDPTable);
		    iout++;
                    child_2 = cloneAndShiftWithDP(*iout, shift, PtrDPTable);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = cloneAndShiftWithDP(*iout, shift, PtrDPTable);
		    iout++;
                    child_2 = cloneAndShiftWithDP(*iout, shift, PtrDPTable);
		    iout++;
                    child_3 = cloneAndShiftWithDP(*iout, shift, PtrDPTable);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
            }
        }
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               (parent->Get_DAGnode_delta_time_as_int()+shift));
        }    

	PtrDPTable->insert(make_pair(parent, clone));
        return clone;
    }    



CDAGNode* CHelper :: callCloneAndShiftWithDP(CDAGNode* parent, int shift)
{
  map<CDAGNode*, CDAGNode*> DPTable;
  map<CDAGNode*, CDAGNode*> *PtrDPTable = &DPTable;

  return cloneAndShiftWithDP(parent, shift, PtrDPTable);
}


void CHelper :: UnrollTR_and_Print_in_Yices_Format(CDAGNode *root, map<string, int> WidthTable, FILE *yices_fp, int times, string property)
{
  map<CDAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  propogateWidths(root, WidthTableForNodes, 1);

  // Now root is ready to be printed
  // Print it "times" times with additional "and"s

  if(times == 0) 
    {
      return;
    }
  
  if(times == 1)
    {
    fprintf(yices_fp, "\n(assert ");
    
    if(property!="") fprintf(yices_fp, "\n(and\n");

    root->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bool", WidthTableForNodes, 1, yices_fp, 0);

    if(property!="") fprintf(yices_fp, "\n\n%s)", property.c_str());

    fprintf(yices_fp, "\n)");
    fprintf(yices_fp, "\n(check)\n");
    return;
    }

    fprintf(yices_fp, "\n(assert ");

    if(property!="") fprintf(yices_fp, "\n(and\n");

    // Unrolled SE part
    
    fprintf(yices_fp, "\n(and ");
       
    for(int i=0; i<times; i++)
      {
	root->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bool", WidthTableForNodes, 1, yices_fp, i);
	fprintf(yices_fp, "\n\n");
      }
    fprintf(yices_fp, "\n)");

    // Unrolled SE part finishes

    if(property!="") fprintf(yices_fp, "\n\n%s)", property.c_str());
    
    fprintf(yices_fp, "\n)");
    fprintf(yices_fp, "\n(check)\n");
}



void CHelper :: getAPsForDAGInPropSkelton(CDAGNode* parent, set<CDAGNode*> &APTable)
    {
                
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();
	    string MyName = parent->Get_DAGnode_value();

            switch(outlist_size)
            {
                case 1:
                {
		  CDAGNode* child_1 = *iout;
		  getAPsForDAGInPropSkelton(child_1, APTable);
		  break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;

                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;

		    if(MyName == "equivalent" || MyName == "is_not_equal" || MyName == "=" || MyName == "not_equal_to")
		      {
			//cout<<"\n"<<parent->getDAGStr()<<" inserted into APTable\n";
			APTable.insert(parent);
		      }

		    else 
		      {
			getAPsForDAGInPropSkelton(child_1, APTable);
			getAPsForDAGInPropSkelton(child_2, APTable);
		      }

                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

		   
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;
		    iout++;
                    child_3 = *iout;

		    getAPsForDAGInPropSkelton(child_1, APTable);
		    getAPsForDAGInPropSkelton(child_2, APTable);
		    getAPsForDAGInPropSkelton(child_3, APTable);
		    
		    break;
                }
	    case 0:
	      {
		// Do nothing for constants
		break;
	      }
            }
        }
        else
        {
	  // Do nothing for variables
        }        
        
    }    



// This function is findUnneededSelections modified with ability to remove selections like x"4"(0:3)

int CHelper :: findUnneededSelectionsModified(CDAGNode *tree, set<CDAGNode*> &UnneededSelects, bool InsideSelection, map<string, int> WidthTable, bool InsideFirstArgOfSelect)
{
  string name = tree->Get_DAGnode_value();
  int delta_time = tree->Get_DAGnode_delta_time_as_int();
  bool ntype = tree->Get_DAG_node_type();

   if(!ntype) // variable
     {
      char temp[10];
      sprintf(temp, "%d", delta_time);
      name += "_";
      name += temp;

      int width =  WidthTable[name];
      return width;
    }

  else
    {
     			
    list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
    unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  CDAGNode *child1 = *iout;
	  int WidthOfChild1 = findUnneededSelectionsModified(child1, UnneededSelects, false, WidthTable, false);
	  return WidthOfChild1; // Bitwise not and unary minus
	  break;
	}
      case 2:
	{
	  int WidthOfChild1, WidthOfChild2;
	  CDAGNode *child1 = *iout;
	  WidthOfChild1 = findUnneededSelectionsModified(child1, UnneededSelects, false, WidthTable, false);
	  iout++;
	  CDAGNode *child2 = *iout;
	  WidthOfChild2 = findUnneededSelectionsModified(child2, UnneededSelects, false, WidthTable, false);

	  if(name == "equivalent" || name == "is_not_equal" || name == "=" || name == "not_equal_to")
	    return 1;

	  else
	    return findMaximum(WidthOfChild1, WidthOfChild2);
	 
	  break;
	}
      case 3:
	{
	  int WidthOfChild1, WidthOfChild2, WidthOfChild3;
	  CDAGNode *child1 = *iout;
	  iout++;
	  CDAGNode *child2 = *iout;
	  iout++;
	  CDAGNode *child3 = *iout;
	  
	  if(name == "select")
	    {
	      WidthOfChild1 = findUnneededSelectionsModified(child1, UnneededSelects, true, WidthTable, true);
	      WidthOfChild2 = findUnneededSelectionsModified(child2, UnneededSelects, true, WidthTable, false);
	      WidthOfChild3 = findUnneededSelectionsModified(child3, UnneededSelects, true, WidthTable, false);
	      
	      int SizeOfSelection = WidthOfChild3 - WidthOfChild2 +1;
	      if(SizeOfSelection == WidthOfChild1) // Size of selection matches with the actual size. hence selection unneeded
		{
		  //cout<<"\nUnneeded selection found\n";
		  UnneededSelects.insert(tree);
		}

	      return SizeOfSelection;
	    }

	  else //ite
	    {
	      WidthOfChild1 = findUnneededSelectionsModified(child1, UnneededSelects, true, WidthTable, false);
	      WidthOfChild2 = findUnneededSelectionsModified(child2, UnneededSelects, true, WidthTable, false);
	      WidthOfChild3 = findUnneededSelectionsModified(child3, UnneededSelects, true, WidthTable, false);
	      return findMaximum(WidthOfChild2, WidthOfChild3); 
	    }
		
	  break;
	}
      case 0: // constants The important thing here is that only constants appearing inside selects should return their proper size
	{
	  if(InsideSelection)
	    {
	  

	      if(InsideFirstArgOfSelect)
		{
		  if(name[0] == 'x') // hex
		    {
		      return (name.length()-3)*4;
		    }
		  else // 
		    {
		      return name.length(); 
		    }
		}
	        else // 
		    {
		      return atoi(name.c_str()); 
		    }
	    }
	  else
	    return -1;
	}
      }

    }
}



void CHelper::writeBVDeclarationsFromWidthTableInSMTLibFormat(FILE *smt_file, map<string, int> WidthTable)

{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {
	  string id_delta = it->first;

	  int index = id_delta.find_last_of("_");
	  string id = id_delta.substr(0, index);
	  int delta = atoi(id_delta.substr(index+1).c_str());
	  delta = delta*-1;
	  
	  fprintf(smt_file,":extrafuns ((%s_%d",id.c_str(), delta);
	  fprintf(smt_file," BitVec[%d]))\n",it->second);
	}
 
}


string CHelper :: callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(CDAGNode *root, map<string, int> WidthTable, set<string> &VarsToElimSet)
{
  map<CDAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  // propogateWidths is newly added on 19.1.2010

  propogateWidths(root, WidthTableForNodes, 1);
  
  
  return root->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bool", WidthTableForNodes, 1, VarsToElimSet);
}


string CHelper :: callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(CDAGNode *root, map<string, int> WidthTable)
{
  map<CDAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  // propogateWidths is newly added on 19.1.2010

  propogateWidths(root, WidthTableForNodes, 1);
  
  
  return root->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bool", WidthTableForNodes, 1);
}


  CDAGNode* CHelper :: cloneWithDeltaToggled(CDAGNode* parent)
    {
        CDAGNode* clone;
                   
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = cloneWithDeltaToggled(*iout);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = cloneWithDeltaToggled(*iout);
		    iout++;
                    child_2 = cloneWithDeltaToggled(*iout);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = cloneWithDeltaToggled(*iout);
		    iout++;
                    child_2 = cloneWithDeltaToggled(*iout);
		    iout++;
                    child_3 = cloneWithDeltaToggled(*iout);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
            }
        }
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               (parent->Get_DAGnode_delta_time_as_int()*-1));
        }        
        return clone;
    }    


void CHelper :: createSetFromList(set<string> &VarsToElimSet, list<string> VarsToElim)
{
  for(list<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    VarsToElimSet.insert(*it);
}

void CHelper :: getDeltaToggledSet(set<string> VarsToElimSet, set<string> &VarsToElimSetToggled)
{
  for(set<string>::iterator it=VarsToElimSet.begin(); it != VarsToElimSet.end(); it++)
    {
      string id_delta = *it;

      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      int delta = atoi(id_delta.substr(index+1).c_str());
      delta = delta*-1;

      char temp[100];
      sprintf(temp, "%d", delta);
      string new_delta(temp);
      id+="_";
      id+=new_delta;
      
      VarsToElimSetToggled.insert(id);
    }
}

string  CHelper :: getExistsString(set<string> VarsToElimSetToggled, map<string, int> WidthTable)
{
  string ret="";
  for(set<string>::iterator it=VarsToElimSetToggled.begin(); it != VarsToElimSetToggled.end(); it++)
    {
      string id_delta = *it;
      int index = id_delta.find_last_of("_");
      string id = id_delta.substr(0, index);
      int delta = atoi(id_delta.substr(index+1).c_str());
      delta = delta*-1;

      char temp[100];
      sprintf(temp, "%d", delta);
      string new_delta(temp);
      id+="_";
      id+=new_delta;
      
      int width = WidthTable[id];
      sprintf(temp, "%d", width);
      string width_str(temp);

      ret+= " (?";
      ret+= id_delta;
      ret+= " ";
      ret+="BitVec[";
      ret+= width_str;
      ret+="]) ";
    }
      
  return ret;
}


string CHelper :: checkImplicationInCVC(CDAGNode *original, CDAGNode *eliminated, map<string, int> WidthTable, list<string> VarsToElim)
{
        
  set<string> VarsToElimSet;
  createSetFromList(VarsToElimSet, VarsToElim);

  	FILE *cvc_file = fopen("cvc_check.smt", "w");

	if(cvc_file == NULL) 
	{cout<<"\nCannot create exp.smt\n"; exit(1);}

	fprintf(cvc_file,"(benchmark test.smt\n:logic QF_UFBV[32]\n");
	
	writeBVDeclarationsFromWidthTableInSMTLibFormat(cvc_file, WidthTable);


	//cout<<"\nOriginal = "<<original->getDAGStr()<<endl;
	original = cloneWithDeltaToggled(original);
	//cout<<"\nOriginal = "<<original->getDAGStr()<<endl;
	
	set<string> VarsToElimSetToggled;
	getDeltaToggledSet(VarsToElimSet, VarsToElimSetToggled);
	string original_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(original, WidthTable, VarsToElimSetToggled);

	
	//cout<<"\neliminated = "<<eliminated->getDAGStr()<<endl;
	eliminated = cloneWithDeltaToggled(eliminated);
	//cout<<"\neliminated = "<<eliminated->getDAGStr()<<endl;

	set<string> temp;
	string eliminated_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(eliminated, WidthTable, temp);

	string exists_str = getExistsString(VarsToElimSetToggled,WidthTable);

	fprintf(cvc_file,"\n:formula (and  \n\n%s\n\n (forall %s (not %s)))", eliminated_str.c_str(), exists_str.c_str(), original_str.c_str());
	fprintf(cvc_file,"\n)\n");
  
	fclose(cvc_file);

	
	system("cvc3 -lang smt cvc_check.smt > result_cvc_check.txt");

	cout<<"\nCVC3 called\n";

	FILE *result_check_file = fopen("result_cvc_check.txt", "r");
  
	if(result_check_file == NULL) 
	  {cout<<"\nCannot create result_cvc_check.txt\n"; exit(1);}



	char result[100];
	fscanf(result_check_file, "%s", result);
	fclose(result_check_file);
	string result_str(result);

	//cout<<"\nresult_str = "<<result_str<<endl;
	
	return result_str;
   
}



void CHelper :: printQEliminatorResultsInFile(CDAGNode *original, map<string, int> WidthTable, list<string> VarsToElim, time_t qelimtime)
{
  static int count=0;
        
  set<string> VarsToElimSet;
  createSetFromList(VarsToElimSet, VarsToElim);

  string SmtFileName = "case";
  char count_chr[100];
  sprintf(count_chr,"%d",count);
  string count_str = count_chr;
  SmtFileName += "_";
  SmtFileName += count_str;
  SmtFileName += ".smt";

  bool delta_toggling=false;


  FILE *Myfile = fopen(SmtFileName.c_str(), "w");

  if(Myfile == NULL) 
    {cout<<"\nCannot create case.smt\n"; exit(1);}

  fprintf(Myfile,"(benchmark test\n");

  if(delta_toggling)
    {
	
      writeBVDeclarationsFromWidthTableInSMTLibFormat(Myfile, WidthTable);

      original = cloneWithDeltaToggled(original);
	
      set<string> VarsToElimSetToggled;
      getDeltaToggledSet(VarsToElimSet, VarsToElimSetToggled);
      string original_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(original, WidthTable);

      fprintf(Myfile,"\n:exists (");
      for(set<string>::iterator it=VarsToElimSet.begin(); it!=VarsToElimSet.end(); it++)
	{
	  string id_delta = *it;
	  int index = id_delta.find_last_of("_");
	  string id = id_delta.substr(0, index);
	  int delta = atoi(id_delta.substr(index+1).c_str());
	  delta = delta*-1;
	  fprintf(Myfile," %s_%d ", id.c_str(), delta);
	}
      fprintf(Myfile,")");


      fprintf(Myfile,"\n\n:formula %s\n\n:notes \"Elimination Time = %d seconds\")\n\n\n", original_str.c_str(), qelimtime);
    }
  else
    {
      	
      writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggle(Myfile, WidthTable);

      fprintf(Myfile,"\n:exists (");
      for(set<string>::iterator it=VarsToElimSet.begin(); it!=VarsToElimSet.end(); it++)
	{
	  string id_delta = *it;
	  fprintf(Myfile," %s ", id_delta.c_str());
	}
      fprintf(Myfile,")");

      string original_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(original, WidthTable);

      fprintf(Myfile,"\n\n:formula %s\n\n)\n\n\n", original_str.c_str());
    }
      
  fclose(Myfile);

  count++;
    
}



void CHelper :: conjunctiveDecomposition(set<CDAGNode*> &Relations, CDAGNode *root)
{
	        
  string dagName = root->Get_DAGnode_value();

  //cout<<"\nDAG NAME="<<dagName<<endl;

  if(dagName == "and")
	      
    {
      list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
      unsigned int outlist_size = root->Get_DAGnode_outlist_size();

      for(unsigned int i=0; i<outlist_size; i++, iout++)
	{
	  conjunctiveDecomposition(Relations,(*iout));
	}
    }
  else
    {
      Relations.insert(root);
    }

} 


CDAGNode* CHelper :: convertRelationToFunction(CDAGNode *root)
{
  string dagName = root->Get_DAGnode_value();

  if(dagName == "equivalent") return root;

  else if(dagName == "ite")
    {
      list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
      CDAGNode *cond = *iout;
      iout++;
      CDAGNode *then = *iout;
      iout++;
      CDAGNode *els =*iout;

      CDAGNode *lhs;
      CDAGNode *rhs;
      CDAGNode *newthen;
      CDAGNode *newels;
      
      string thenName = then->Get_DAGnode_value();
      if(thenName == "equivalent")
	{
	  list<CDAGNode*>::iterator iout1 = then->Get_DAGnode_outlist();
	  lhs = *iout1;
	  iout1++;
	  newthen = *iout1;
	}
      else
	{
	  cout<<"\nThen of ite is not in proper format in function CHelper::convertRelationToFunction\n";
	  exit(1);
	}
      
      string elsName = els->Get_DAGnode_value();
      if(elsName == "equivalent")
	{
	  list<CDAGNode*>::iterator iout1 = els->Get_DAGnode_outlist();
	  if(lhs != *iout1)
	    {
	     cout<<"\nlhs mismatch in function CHelper::convertRelationToFunction\n";
	     exit(1);
	    }
	  iout1++;
	  newels = *iout1;
	}
      else
	{
	  cout<<"\nElse of ite is not in proper format in function CHelper::convertRelationToFunction\n";
	  exit(1);
	}

      rhs = createDAG("ite", cond, newthen, newels);
      CDAGNode* ret =  createDAG("equivalent", lhs, rhs);
      
      return ret;
    }
  else
    {
      cout<<"\nUnallowed node in function CHelper::convertRelationToFunction\n";
      exit(1);
    }
}

   

// This function takes a dag "root" as input. Now, if the dag is like and(...(and(R1, R2), R3),..., Rn), it converts it into a set {R1,...,Rn}. Now convert each Ri into a functional form outi=Fi, now create a map (name, Fi) where name is the id_delta of dag node outi

void CHelper :: createFunctionMap(CDAGNode* root, map<string, CDAGNode*> &FunctionMap)
{
  set<CDAGNode*> Relations;
    
  //cout<<"\nroot = "<<root->getDAGStr()<<endl;

  conjunctiveDecomposition(Relations, root);

  // cout<<"\nDecomposed Relations\n**********\n";
//   for(set<CDAGNode*>::iterator it=Relations.begin(); it!=Relations.end(); it++)
//     {
//       cout<<(*it)->getDAGStr()<<endl;
//     }
  
  //cout<<"\nCreating the function map\n**********\n";
  for(set<CDAGNode*>::iterator it=Relations.begin(); it!=Relations.end(); it++)
    {
      // cout<<"\nRelation = "<<(*it)->getDAGStr()<<endl;

      CDAGNode *Function = convertRelationToFunction(*it);
      //cout<<"\nFunction = "<<Function->getDAGStr()<<endl;
      

      list<CDAGNode*>::iterator iout = Function->Get_DAGnode_outlist();
      CDAGNode *lhs = *iout;
      iout++;
      CDAGNode *rhs = *iout;

      string lhs_name;
      
      if(lhs->Get_DAGnode_value()!="select")
	{
	lhs_name = lhs->Get_DAGnode_value();
	lhs_name += "_";
	lhs_name += lhs->Get_DAGnode_delta_time_as_string();
	}
      else
	{
	  list<CDAGNode*>::iterator iout2 = lhs->Get_DAGnode_outlist();
	  CDAGNode* child1 = *iout2;
	  lhs_name = child1->Get_DAGnode_value();
	  lhs_name += "_";
	  lhs_name += child1->Get_DAGnode_delta_time_as_string();
	}
	 
 
      FunctionMap.insert(make_pair(lhs_name, Function));
    }
}
  
  
      
	  


// This function takes a dag "root" as input. Now, if the dag is like and(...(and(R1, R2), R3),..., Rn), it converts it into a set {R1,...,Rn}. Now convert each Ri into a functional form outi=Fi, now create a map (name, Fi) where name is the id_delta of dag node outi, and it finds the dependency map

void CHelper :: getDepMapFinal(CDAGNode* root, map<string, set<string> > &DepMapFinal)
{
  set<CDAGNode*> Relations;
  map<string, CDAGNode*> FunctionMap;
  map<string, set<string> > DepMap;

  
  //cout<<"\nroot = "<<root->getDAGStr()<<endl;

  conjunctiveDecomposition(Relations, root);

//   cout<<"\nDecomposed Relations\n**********\n";
//   for(set<CDAGNode*>::iterator it=Relations.begin(); it!=Relations.end(); it++)
//     {
//       cout<<(*it)->getDAGStr()<<endl;
//     }
  
  //cout<<"\nCreating the function map\n**********\n";
  for(set<CDAGNode*>::iterator it=Relations.begin(); it!=Relations.end(); it++)
    {
      //cout<<"\nRelation = "<<(*it)->getDAGStr()<<endl;

      CDAGNode *Function = convertRelationToFunction(*it);
      //cout<<"\nFunction = "<<Function->getDAGStr()<<endl;
      

      list<CDAGNode*>::iterator iout = Function->Get_DAGnode_outlist();
      CDAGNode *lhs = *iout;
      iout++;
      CDAGNode *rhs = *iout;

      string lhs_name = lhs->Get_DAGnode_value();
      lhs_name += "_";
      lhs_name += lhs->Get_DAGnode_delta_time_as_string();
      FunctionMap.insert(make_pair(lhs_name, rhs));
    }

  for(map<string, CDAGNode*>::iterator it=FunctionMap.begin(); it!=FunctionMap.end(); it++)
    {

      //cout<<"\nlhs_name = "<<it->first<<" , rhs = "<<(it->second)->getDAGStr()<<"\n";
      string lhs_name = it->first;
      set<string> friends;
      (it->second)->getDAGNamesWithCheck(friends);

      //cout<<"Friends\n";
      //for(set<string>::iterator it1=friends.begin(); it1!=friends.end(); it1++)
      //	cout<<*it1<<"\t";
      //cout<<"\n";

      DepMap.insert(make_pair(lhs_name, friends));
    }


  //cout<<"\nDepMap\n***********\n";
  // for(map<string, set<string> >::iterator it=DepMap.begin();it!=DepMap.end();it++)
//     {
//       string signal = it->first;
//       set<string> friends = it->second;
//       cout<<"\nSignal = "<<signal<<endl;
//       cout<<"Friends\n";
//       for(set<string>::iterator it1=friends.begin(); it1!=friends.end(); it1++)
// 	cout<<*it1<<"\t";
//       cout<<"\n";      
//     }

  for(map<string, set<string> >::iterator it=DepMap.begin();it!=DepMap.end();it++)
    {
      string signal = it->first;
      set<string> State = it->second;
      set<string> FixPoint;
      getFixPoint(DepMap, State, FixPoint);
      DepMapFinal.insert(make_pair(signal, FixPoint));
    }

//   cout<<"\nDepMapFinal\n***********\n";
//     for(map<string, set<string> >::iterator it=DepMapFinal.begin();it!=DepMapFinal.end();it++)
//     {
//       string signal = it->first;
//       set<string> friends = it->second;
//       cout<<"\nSignal = "<<signal<<endl;
//       cout<<"Friends\n";
//       for(set<string>::iterator it1=friends.begin(); it1!=friends.end(); it1++)
// 	cout<<*it1<<"\t";
//       cout<<"\n";      
//     }


}
  

void CHelper :: getFixPoint(map<string, set<string> > &DepMap, set<string> &State, set<string> &FixPoint)
{
  
  for(set<string>::iterator it=State.begin(); it!=State.end(); it++)
    {
      FixPoint.insert(*it);
    }
  
  while(1){
    set<string> NewState;
    getImage(DepMap, FixPoint, NewState);
    set<string> NewFixPoint;
    set_union(FixPoint.begin(), FixPoint.end(), NewState.begin(), NewState.end(),inserter(NewFixPoint, NewFixPoint.begin()));
    if(setsEqual(FixPoint, NewFixPoint)) break;
    set_union(FixPoint.begin(), FixPoint.end(), NewFixPoint.begin(), NewFixPoint.end(),inserter(FixPoint, FixPoint.begin()));    
  }
}

void CHelper :: getImage(map<string, set<string> > &DepMap, set<string> &FixPoint, set<string> &NewState)
{
  for(set<string>::iterator it=FixPoint.begin(); it!=FixPoint.end(); it++)
    {
      string signal = *it;
      map<string, set<string> > ::iterator it2 = DepMap.find(signal);
      if(it2!=DepMap.end())
	{
	  set<string> Image = it2->second;
	  set_union(Image.begin(), Image.end(), NewState.begin(), NewState.end(),inserter(NewState, NewState.begin()));
	}
    }
}

bool CHelper :: setsEqual(set<string> &FixPoint, set<string> &NewFixPoint)
{
 set<string> Set1, Set2;
 set_difference(FixPoint.begin(), FixPoint.end(), NewFixPoint.begin(), NewFixPoint.end(),inserter(Set1, Set1.begin()));
 set_difference(NewFixPoint.begin(), NewFixPoint.end(), FixPoint.begin(), FixPoint.end(),inserter(Set2, Set2.begin()));
 if(Set1.empty() && Set2.empty()) return true;
 return false;
}
 


CDAGNode* CHelper :: cloneWithClockBasedSimplification(CDAGNode* parent)
{
  CDAGNode* clone;
                   
  if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
    {
      list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
      unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

      switch(outlist_size)
	{
	case 1:
	  {
	    CDAGNode* child_1 ;

	    child_1 = cloneWithClockBasedSimplification(*iout);
	    clone = createDAG(parent->Get_DAGnode_value(), child_1);
	    break;
	  }
	case 2:
	  {
	    CDAGNode* child_1 ;
	    CDAGNode* child_2 ;
                    
	    child_1 = cloneWithClockBasedSimplification(*iout);
	    iout++;
	    child_2 = cloneWithClockBasedSimplification(*iout);
	    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
	    break;
	  }
	case 3:
	  {
	    CDAGNode* child_1 ;
	    CDAGNode* child_2 ;
	    CDAGNode* child_3 ;

	    if(parent->Get_DAGnode_value()=="ite")
	      {
	    
		bool CondnFalse = (*iout)->evaluateTrivialCondition();
		bool CondnTrue = (*iout)->evaluateClockCondition(); // This reduction assumes clock is changing continuously

		if(!CondnFalse) // condition part is trivially false : This node is same as the else part
		  {
		    iout++;
		    iout++;
		    child_3 = cloneWithClockBasedSimplification(*iout);
		    clone = child_3;		
		  }
		else if(CondnTrue) // condition part is trivially true : This node is same as the then part
		  {
		    iout++;
		    child_2 = cloneWithClockBasedSimplification(*iout);
		    clone = child_2;
		  }
		else
		  {
		    child_1 = cloneWithClockBasedSimplification(*iout);
		    iout++;
		    child_2 = cloneWithClockBasedSimplification(*iout);
		    iout++;
		    child_3 = cloneWithClockBasedSimplification(*iout);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
		  }
	      }
	    else
	      {
		child_1 = cloneWithClockBasedSimplification(*iout);
		iout++;
		child_2 = cloneWithClockBasedSimplification(*iout);
		iout++;
		child_3 = cloneWithClockBasedSimplification(*iout);
		clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
	      }
	    
	    break;
	  }
	case 0:
	  {
	    clone = createDAG( parent->Get_DAGnode_value());
	    break;
	  }
	}
    }
  else
    {
      clone = createDAG( parent->Get_DAGnode_value(),
			 parent->Get_DAGnode_delta_time_as_int());
    }        
  return clone;
}    




CDAGNode* CHelper :: cloneWithTrivialDisequationsEliminated(CDAGNode* parent)
{
  CDAGNode* clone;
                   
  if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
    {
      list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
      unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

      switch(outlist_size)
	{
	case 1:
	  {
	    CDAGNode* child_1 ;

	    child_1 = cloneWithTrivialDisequationsEliminated(*iout);
	    clone = createDAG(parent->Get_DAGnode_value(), child_1);
	    break;
	  }
	case 2:
	  {
	    CDAGNode* child_1 ;
	    CDAGNode* child_2 ;
                    
	    child_1 = cloneWithTrivialDisequationsEliminated(*iout);
	    iout++;
	    child_2 = cloneWithTrivialDisequationsEliminated(*iout);
	    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
	    break;
	  }
	case 3:
	  {
	    CDAGNode* child_1 ;
	    CDAGNode* child_2 ;
	    CDAGNode* child_3 ;

	    if(parent->Get_DAGnode_value()=="ite")
	      {
	    
		bool CondnFalse = (*iout)->evaluateTrivialCondition();
		
		if(!CondnFalse) // condition part is trivially false : This node is same as the else part
		  {
		    iout++;
		    iout++;
		    child_3 = cloneWithTrivialDisequationsEliminated(*iout);
		    clone = child_3;		
		  }
		else
		  {
		    child_1 = cloneWithTrivialDisequationsEliminated(*iout);
		    iout++;
		    child_2 = cloneWithTrivialDisequationsEliminated(*iout);
		    iout++;
		    child_3 = cloneWithTrivialDisequationsEliminated(*iout);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
		  }
	      }
	    else
	      {
		child_1 = cloneWithTrivialDisequationsEliminated(*iout);
		iout++;
		child_2 = cloneWithTrivialDisequationsEliminated(*iout);
		iout++;
		child_3 = cloneWithTrivialDisequationsEliminated(*iout);
		clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
	      }
	    
	    break;
	  }
	case 0:
	  {
	    clone = createDAG( parent->Get_DAGnode_value());
	    break;
	  }
	}
    }
  else
    {
      clone = createDAG( parent->Get_DAGnode_value(),
			 parent->Get_DAGnode_delta_time_as_int());
    }        
  return clone;
}    



CDAGNode* CHelper :: cloneWithTrivialEquationsEliminated(CDAGNode* parent)
{
  CDAGNode* clone;
                   
  if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
    {
      list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
      unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

      switch(outlist_size)
	{
	case 1:
	  {
	    CDAGNode* child_1 ;

	    child_1 = cloneWithTrivialEquationsEliminated(*iout);
	    clone = createDAG(parent->Get_DAGnode_value(), child_1);
	    break;
	  }
	case 2:
	  {
	    CDAGNode* child_1 ;
	    CDAGNode* child_2 ;
                    
	    child_1 = cloneWithTrivialEquationsEliminated(*iout);
	    iout++;
	    child_2 = cloneWithTrivialEquationsEliminated(*iout);

	    if(parent->Get_DAGnode_value()=="equivalent" && child_1==child_2)
	      clone = createDAG("true");
	    else
	      clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
	    break;
	  }
	case 3:
	  {
	    CDAGNode* child_1 ;
	    CDAGNode* child_2 ;
	    CDAGNode* child_3 ;

	    
	    child_1 = cloneWithTrivialEquationsEliminated(*iout);
	    iout++;
	    child_2 = cloneWithTrivialEquationsEliminated(*iout);
	    iout++;
	    child_3 = cloneWithTrivialEquationsEliminated(*iout);
	    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
	    
	    break;
	  }
	case 0:
	  {
	    clone = createDAG( parent->Get_DAGnode_value());
	    break;
	  }
	}
    }
  else
    {
      clone = createDAG( parent->Get_DAGnode_value(),
			 parent->Get_DAGnode_delta_time_as_int());
    }        
  return clone;
}    



CDAGNode* CHelper :: callQuickSimplify(CDAGNode *node, map<string, string> &Values)
{
  HashTable<string, CDAGNode*> HTable;
  HashTable<string, CDAGNode*> *PtrHTable=&HTable;
  CDAGNode* ret = quickSimplifyInternal(node, Values, PtrHTable);
  // free HashTable
  return ret;
}



CDAGNode* CHelper :: quickSimplify(CDAGNode *node, map<string, string> &Values, HashTable<string, CDAGNode*> *PtrHTable)
{
  CDAGNode* ret;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

  for(map<string, string>::iterator it=Values.begin();it!=Values.end(); it++)
    {
      key+=it->first;
      key+=it->second;
    }

  HTStatusValue<CDAGNode*> result = PtrHTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"in function CHelper :: quickSimplify\n";
      return result.GetValue();
    }

  else{
  HashTable<string, CDAGNode*> DPTable;
  HashTable<string, CDAGNode*> *PtrDPTable=&DPTable;
  ret = quickSimplifyInternal(node, Values, PtrDPTable);
  
  // free HashTable
  result = PtrHTable->hashtable_insert(key, ret);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function CHelper :: quickSimplify\n";
      exit(1);
    }
  return ret;}
}
  
  

  
CDAGNode* CHelper :: quickSimplifyInternal(CDAGNode *node, map<string, string> &Values, HashTable<string, CDAGNode*> *DPTable)
{
  CDAGNode* ret;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 

  HTStatusValue<CDAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }


  unsigned int outlist_size = node->Get_DAGnode_outlist_size();
  
  if(outlist_size == 0) {
    if(node->Get_DAG_node_type()==true)
      ret = node;
    else
      {
	string node_value = node->Get_DAGnode_value();
	node_value+="_";
	node_value+=node->Get_DAGnode_delta_time_as_string();
	
	map<string, string>::iterator it=Values.find(node_value);
	if(it!=Values.end())
	    {
	      ret = createDAG(it->second);
	    }
	else
	  ret = node;
      }
  }

  else
  {
    string dag_str = node->Get_DAGnode_value();
    if(!(dag_str == "and" || dag_str == "or" || dag_str=="ite" || dag_str == "not"|| dag_str == "+"|| dag_str == "-"|| dag_str == "*"|| dag_str == "equivalent" || dag_str == "is_not_equal"))
      {
	//cout<<dag_str<<"node encountered. The nodes down cannot be simplified\n";
	ret = node;
      } 
    else
      {
	vector<CDAGNode*> SimplifiedChildren;
	list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
    

	for(unsigned int i=0;i<outlist_size; i++,iout++)
	  {
	    SimplifiedChildren.push_back(quickSimplifyInternal(*iout, Values, DPTable));
	  }

   
	if(dag_str == "and"){
	  if((SimplifiedChildren[0])->Get_DAGnode_value()=="true"|| (SimplifiedChildren[1])->Get_DAGnode_value()=="false")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_DAGnode_value()=="true" || (SimplifiedChildren[0])->Get_DAGnode_value()=="false")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("and", SimplifiedChildren[0], SimplifiedChildren[1]) ;
	}
	else if(dag_str == "or"){
	  if((SimplifiedChildren[0])->Get_DAGnode_value()=="false"|| (SimplifiedChildren[1])->Get_DAGnode_value()=="true")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_DAGnode_value()=="false" || (SimplifiedChildren[0])->Get_DAGnode_value()=="true")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("or", SimplifiedChildren[0], SimplifiedChildren[1]);
	}
	else if(dag_str == "not"){
	  if((SimplifiedChildren[0])->Get_DAGnode_value()=="false")
	    ret = createDAG("true");
	  else if((SimplifiedChildren[0])->Get_DAGnode_value()=="true")
	    ret = createDAG("false");
	  else
	    ret = createDAG("not", SimplifiedChildren[0]);
	}

	else if(dag_str == "ite"){
	  if((SimplifiedChildren[0])->Get_DAGnode_value()=="false")
	    ret = SimplifiedChildren[2];
	  else if((SimplifiedChildren[0])->Get_DAGnode_value()=="true")
	    ret = SimplifiedChildren[1];
	  else if(SimplifiedChildren[1] == SimplifiedChildren[2])
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_DAGnode_value()=="false" && (SimplifiedChildren[2])->Get_DAGnode_value()=="true")
	    ret = createDAG("not", SimplifiedChildren[0]);
	  else if((SimplifiedChildren[1])->Get_DAGnode_value()=="true" && (SimplifiedChildren[2])->Get_DAGnode_value()=="false")
	    ret = SimplifiedChildren[0];
	  else 
	    ret = createDAG("ite", SimplifiedChildren[0], SimplifiedChildren[1], SimplifiedChildren[2]);
	}


	else if(dag_str == "+"){
	  if((SimplifiedChildren[0])->Get_DAGnode_value()=="0")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_DAGnode_value()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("+", SimplifiedChildren[0], SimplifiedChildren[1]);
	}
	else if(dag_str == "-"){
	  if((SimplifiedChildren[1])->Get_DAGnode_value()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("-", SimplifiedChildren[0], SimplifiedChildren[1]);
	}
	else if(dag_str == "*"){
	  if((SimplifiedChildren[0])->Get_DAGnode_value()=="1" || (SimplifiedChildren[1])->Get_DAGnode_value()=="0")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_DAGnode_value()=="1"|| (SimplifiedChildren[0])->Get_DAGnode_value()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("*", SimplifiedChildren[0], SimplifiedChildren[1]);
	}
	else if(dag_str == "equivalent"){
	  if(SimplifiedChildren[0] == SimplifiedChildren[1])
	    ret = createDAG("true");
	  else if((SimplifiedChildren[0]->Get_DAGnode_outlist_size()==0 && (SimplifiedChildren[0]->Get_DAG_node_type()==true)) && (SimplifiedChildren[1]->Get_DAGnode_outlist_size()==0 && (SimplifiedChildren[1]->Get_DAG_node_type()==true))) // both operands are constants
	    ret = createDAG("false");
	  else
	    ret = createDAG("equivalent", SimplifiedChildren[0], SimplifiedChildren[1]);	 
     
	}
	else if(dag_str == "is_not_equal"){
	  if(SimplifiedChildren[0] == SimplifiedChildren[1])
	    ret = createDAG("false");
	  else if((SimplifiedChildren[0]->Get_DAGnode_outlist_size()==0 && (SimplifiedChildren[0]->Get_DAG_node_type()==true)) && (SimplifiedChildren[1]->Get_DAGnode_outlist_size()==0 && (SimplifiedChildren[1]->Get_DAG_node_type()==true))) // both operands are constants
	    ret = createDAG("true");
	  else
	    ret = createDAG("is_not_equal", SimplifiedChildren[0], SimplifiedChildren[1]);;	 
     
	}
	else
	  {
	    cout<<dag_str<<"node encountered\n";
	    exit(1);
	  }
      }

  }
  result = DPTable->hashtable_insert(key, ret);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function CHelper :: quickSimplifyInternal\n";
      exit(1);
    }
  return ret;
}

  
// createWidthTableForOperators function with Memoizaton added

void CHelper :: createWidthTableForOperatorsWithDP(CDAGNode *tree, map<CDAGNode*, int> &WidthTableForNodes, map<string, int> &WidthTable)
{
  if(WidthTableForNodes.find(tree) != WidthTableForNodes.end()) // already width found
    return;

  string name = tree->Get_DAGnode_value();
  int delta_time = tree->Get_DAGnode_delta_time_as_int();
  bool ntype = tree->Get_DAG_node_type();

   if(!ntype) // variable
    {
    
      char temp[10];
      sprintf(temp, "%d", delta_time);
      name += "_";
      name += temp;
       
      int Width =  WidthTable[name];
      WidthTableForNodes.insert(make_pair(tree, Width));
    }

  else
    {
     			
    list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
    unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  CDAGNode *child1 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);

	  if(name == "not")
	    {
	      int Width =  1;
	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	     cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::createWidthTableForOperatorsWithDP\n";
	     exit(1);
	    }
 
	 
	  break;
	}
      case 2:
	{
	  CDAGNode *child1 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  CDAGNode *child2 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	 
	  if(name == "equivalent" || name == "+" || name == "*" || name == "is_not_equal" || name == "concat" || name == "-" || name == "<=" || name == ">=" || name == "<" || name == ">")
	    {

	      int Width1 =  WidthTableForNodes[child1];
	      int Width2 =  WidthTableForNodes[child2];

	      int Width;

	      if(name == "equivalent" || name == "+" || name == "*" || name == "is_not_equal" || name == "-"|| name == "<=" || name == ">=" || name == "<" || name == ">")
		Width = findMaximum(Width1, Width2);
	      else // concat
		Width = Width1 + Width2;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	  else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::createWidthTableForOperatorsWithDP\n";
	      exit(1);
	    }

	  break;
	}
      case 3:
	{
	  CDAGNode *child1 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  CDAGNode *child2 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  CDAGNode *child3 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);

	  if(name == "ite") // Width of an ite is in fact maximum of widths of child2 and child3
	    {
	      
	      //int Width = 1;

	       int Width1 =  WidthTableForNodes[child2];
	       int Width2 =  WidthTableForNodes[child3];

	      int Width = findMaximum(Width1, Width2);

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else if(name == "select")
	    {
	      int Width1 =  WidthTableForNodes[child2];
	      int Width2 =  WidthTableForNodes[child3];

	      int Width = Width2 - Width1 +1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	   else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::createWidthTableForOperatorsWithDP\n";
	      exit(1);
	    }

	 
	  break;
	}
      case 0: // constants
	{
	  if(name[0] == 'x') // hex
	    {
	      WidthTableForNodes.insert(make_pair(tree, (name.length()-3)*4));
	    }
	  else // 
	    {
	      int Width = findMinimumWidthRequired(name);
	      
	      //cout<<"\nname = "<<name<<"\t"<<"Width = "<<Width<<endl;

	      WidthTableForNodes.insert(make_pair(tree, Width));

	      

	      //WidthTableForNodes.insert(make_pair(tree, 0));
	    }
	}
      }

    }
}


 
// propogateWidths function with Memoizaton added
  
void CHelper :: propogateWidthsWithDP(CDAGNode *tree, map<CDAGNode*, int> &WidthTableForNodes, int WidthFromParent, set<CDAGNode*> &DPTable)
{
  if(DPTable.find(tree) != DPTable.end()) return;

  string name = tree->Get_DAGnode_value();
  int delta_time = tree->Get_DAGnode_delta_time_as_int();
  bool ntype = tree->Get_DAG_node_type();
  

   if(!ntype) // variable
    {
    
      // Do nothing : Variable's width should not be changed
    }

  else
    {
     			
    list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
    unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  int width = WidthTableForNodes[tree];
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);

	  break;
	}
      case 2:
	{
	  if(name == "+" || name == "*")
	    {
	      WidthTableForNodes[tree] = WidthFromParent;
	    }

	  int width = WidthTableForNodes[tree];
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	  iout++;
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	 
	  break;
	}
      case 3:
	{
	  int width = WidthTableForNodes[tree];
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	  iout++;
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	  iout++;
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	 
	  break;
	}
      case 0: // constants
	{
	  // For all constants, width field is set
	  // But note that width field makes sense only for bit-vector constants
	  //WidthTableForNodes[tree] = WidthFromParent;	  
	}
      }

    }

   DPTable.insert(tree);
}
  

// callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with memoization for the child functions

string CHelper :: callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(CDAGNode *root, map<string, int> WidthTable, int &TempCount, FILE *fp, map<CDAGNode*, string> &LabelTable, set<string> &SupportOfUnrolling)
{
 
 
  time_t start, end, function_start_time, function_end_time;
  time(&function_start_time);





  map<CDAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();

  //cout<<"\nObtaining the WidthTable\n";

  time(&start);
  
  createWidthTableForOperatorsWithDP(root, WidthTableForNodes, WidthTable);

  time(&end);
  //cout<<"\ncreateWidthTableForOperatorsWithDP Time "<<end-start<<" seconds \n";

  //cout<<"\nWidthTable obtained\n";

  // propogateWidths is newly added on 19.1.2010

  set<CDAGNode*> DPTable1;

  //cout<<"\nPropgating widths\n";

  DPTable1.clear();


  time(&start);

  propogateWidthsWithDP(root, WidthTableForNodes, 1, DPTable1);

  time(&end);
  //cout<<"\npropogateWidthsWithDP Time "<<end-start<<" seconds \n";


  //cout<<"\nWidths propogated\n";

  DPTable1.clear();


  //cout<<"\nSetting the types\n";

  map<CDAGNode*, string> TypeTableOfITENodes;


  time(&start);


  bool settypes_with_dp = true;

  if(settypes_with_dp)
    {
      setTypesWithDP(root, "bool", TypeTableOfITENodes);
    }
  else
    {
      setTypes(root, "bool", TypeTableOfITENodes);
    }


  time(&end);
  //cout<<"\nsetTypes Time "<<end-start<<" seconds \n";




  //cout<<"\nTypes set\n";

  //cout<<"\nGetting Yices string\n";

//   fprintf(fp, "\nIn our format\n");
  
//   fprintf(fp, "\n%s\n", (root->getDAGStr()).c_str());

//   fprintf(fp, "\nIn Yices format\n");


  time(&start);

  string yices_str;

  //yices_str =  printInYicesFormat(root, "bool", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount);

  if(solver_for_bmc == "yices")
    {
      yices_str =  printInYicesFormatModified(root, "bool", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling);
    }
  else if(solver_for_bmc == "smtlib")
    {
      //cout<<"\nDAG to be printed in SMTLib format "<<root->getDAGStr()<<endl;
      yices_str =  printInSMTLibFormatModified(root, "bool", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling);
    }
  else
    {
      cout<<"\nUnknown solver for bmc "<<solver_for_bmc<<endl;
      exit(1);
    }


  time(&end);
  //cout<<"\nPrinting Time "<<end-start<<" seconds \n";



  //cout<<"\nYices string obtained\n";

  time(&function_end_time);

  //cout<<"\nTotal Function Time "<<function_end_time-function_start_time<<" seconds \n";
  
  return yices_str;
}
  
 


// getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with memoization added

string CHelper :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(CDAGNode *node, string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, HashTable<string, string> *DPTable)
{
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

  HTStatusValue<string> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }
 
  cout<<"\nCache Miss for node "<<node<<"\n";
  string dag_str;

  string name = node->Get_DAGnode_value();
  bool node_type = node->Get_DAG_node_type();
  unsigned int outlist_size = node->Get_DAGnode_outlist_size();
  list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
  string delta_string = node->Get_DAGnode_delta_time_as_string();
  

  if(!node_type)
    {
      dag_str = name + "_" + delta_string;
    }
  else
    {

      //cout<<"\nNAME=\t"<<name<<endl;    //debug code

      if(name == "select")
	{
	  dag_str = "(bv-extract ";

	  iout++;

	  iout++;

	  dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "const", WidthTableForNodes, 1, DPTable) + " ";

	  iout = node->Get_DAGnode_outlist();

	  iout++;

	  dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "const", WidthTableForNodes, 1, DPTable) + " ";

	  iout = node->Get_DAGnode_outlist();

	  dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], DPTable);

	  dag_str += ")";

	}
      // NB: Assumption here is that not_equal_to applies only between bit-vectors

      else if(name == "is_not_equal" || name == "not_equal_to")
	{
	  dag_str = "(not (= ";

	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], DPTable) + " ";
	    }

	  dag_str += "))";
	}

      // NB: Assumption here is that equivalent applies only between bit-vectors

      else if(name == "equivalent" || name == "=")
	{
	  dag_str = "(= ";

	  for(unsigned int i=0; i<outlist_size; i++,iout++)
	    {
	      dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      else if(name == "concat")
	{

	  cout<<"\nconcat is not supported by function CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
	  exit(1);

	  /*
	    dag_str = "(bv-concat ";

	    list<CDAGNode*>::iterator iout = out_list.begin();

	    for(; iout != out_list.end(); iout++)
	    {
	    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes) + " ";
	    }

	    dag_str += ")";

	    return dag_str; */
	}

      else if(name == "*")
	{
	  dag_str = "(bv-mul ";

	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      else if(name == "+")
	{
	  dag_str = "(bv-add ";

	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], DPTable) + " ";
	    }

	  dag_str += ")";

	}


      // Newly added on 20.1.2010

      else if(name == "-")
	{
	  dag_str = "(bv-sub ";

	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      // Newly added on 20.1.2010 ends here
	    

      // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

      else if(name == "and" || (name == "or") || (name == "not") || (name == "ite"))
	{
	  dag_str = "(" +  name + " ";

	  for(unsigned int i=0; i<outlist_size; i++,iout++)
	    {
	      dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      else if(name == "'1'")
	{
	  // following line added on 19.1.2010

	  //int Width_Required = WidthTableForNodes[node];
		

	  dag_str = node->convertLiteralToBitvectorWithProperWidth(name, Width_Required);
		
	  //dag_str = "0b1";

	  //return dag_str;
	}

      else if(name == "'0'")
	{
	  // following line added on 19.1.2010

	  //int Width_Required = WidthTableForNodes[node];


	  dag_str = node->convertLiteralToBitvectorWithProperWidth(name, Width_Required);

	  //dag_str = "0b0";

	  //return dag_str;
	}

      else if(outlist_size == 0) // Other constants

	{
		
	  if(context == "const")
	    {
	      dag_str = name;
	      //cout<<"\ndag_str = "<<dag_str<<endl; 
	    }
	  else if(context == "bv")
	    {
	      //cout<<"\nname = "<<name<<endl; 

	      // following line added on 19.1.2010

	      //int Width_Required = WidthTableForNodes[node];
			
	      if(name[0] == 'x') // hex
		{
		  //dag_str = convertHexToBitvector(name);
		  dag_str = node->convertHexToBitvectorWithProperWidth(name, Width_Required);
		}
	      else // Decimal
		{

		  // Following if else if commented on 18 Jan 2010 

		  //if(name == "1") dag_str="0b1";
		  //else if(name == "0") dag_str="0b0";
		  // else 
		  //  {
		  dag_str = node->convertDecimalToBitvectorWithProperWidth(name, Width_Required);
		  //  }
		}
			  
	      //cout<<"\ndag_str = "<<dag_str<<endl; 		
	    }
	  else if(context == "bool")
	    {
	      if(name == "1") dag_str = "true";
	      else if(name == "0") dag_str = "false";
	      else if(name == "true") dag_str = "true";
	      else if(name == "false") dag_str = "false";
	      //else dag_str = name;

	      else // interpreting a Boolean constant other than true/false as a bit-vector
		{
		  if(name[0] == 'x') // hex
		    {
		      //dag_str = convertHexToBitvector(name);
		      dag_str = node->convertHexToBitvectorWithProperWidth(name, Width_Required);
		    }
		  else // Decimal
		    {
		      dag_str = node->convertDecimalToBitvectorWithProperWidth(name, Width_Required);
		    }
		}

	    }
	  else
	    {
	      cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP\n";
	      exit(1);
	    }
		  
	}
      else
	{
	  cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP\n";
	  exit(1);
	}
		    
    }

  result = DPTable->hashtable_insert(key, dag_str);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function CHelper :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP\n";
      exit(1);
    }
  return dag_str;
}





// Printing in Yices : New algorithm on 3-5-2010

string CHelper :: printInYicesFormat(CDAGNode *node, string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, map<CDAGNode*, string> &LabelTable, FILE *fp, map<string, int> &WidthTable, int &TempCount)
{
  string label;
  string name = node->Get_DAGnode_value();
  bool node_type = node->Get_DAG_node_type();
  unsigned int outlist_size = node->Get_DAGnode_outlist_size();
  list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
  string delta_string = node->Get_DAGnode_delta_time_as_string();
  
  cout<<"\nname=\t"<<name<<endl;    //debug code

  if(node_type && (outlist_size == 0)) // constants 
    {
      if(name == "'1'")
	{
	  label = node->convertLiteralToBitvectorWithProperWidth(name, Width_Required);
	  
	}

      else if(name == "'0'")
	{
	  label = node->convertLiteralToBitvectorWithProperWidth(name, Width_Required);

	}

      else if(context == "const")
	{
	  label = name;
	}
      else if(context == "bv")
	{
	     
	  if(name[0] == 'x') // hex
	    {
		 
	      label = node->convertHexToBitvectorWithProperWidth(name, Width_Required);
	    }
	  else // Decimal
	    {

	      label = node->convertDecimalToBitvectorWithProperWidth(name, Width_Required);
	    }
			  
	}
      else if(context == "bool")
	{
	  if(name == "1") label = "true";
	  else if(name == "0") label = "false";
	  else if(name == "true") label = "true";
	  else if(name == "false") label = "false";

	  else // interpreting a Boolean constant other than true/false as a bit-vector
	    {
	      if(name[0] == 'x') // hex
		{
		  label = node->convertHexToBitvectorWithProperWidth(name, Width_Required);
		}
	      else // Decimal
		{
		  label = node->convertDecimalToBitvectorWithProperWidth(name, Width_Required);
		}
	    }

	}
      else
	{
	  cout<<"\nUnkown name "<<name<<" with context "<<context<<" encountered in function CHelper :: printInYicesFormat\n";
	  exit(1);
	}
      return label;
    }

  else // variable or operator
    {
      map<CDAGNode*, string>::iterator it=LabelTable.find(node);
      cout<<"\nnode = "<<node<<", label = "<<label<<endl;
      if(it!=LabelTable.end()) // entry exists for the node
	{
	  cout<<"\nentry exists for "<<node<<endl;
	  label = it->second;
	  return label;
	}
      else
	{
	  if(!node_type && outlist_size == 0)  // variable
	    {
	      label = name + "_" + delta_string; // label obtained
	      map<string, int>::iterator wit = WidthTable.find(label);
	      if(wit == WidthTable.end())
		{
		  cout<<"\nThe variable "<<label<<" has no entry in the WidthTable\n";
		  exit(1);
		}
	      int width = wit->second; // width obtained

	      fprintf(fp,"(define %s :: (bitvector %d))\n",label.c_str(), width); // written

	      cout<<"\n"<<label<<" printed "<<endl;

	      LabelTable.insert(make_pair(node, label));

	      cout<<"\nentry ("<<node<<","<<label<<")"<<" added "<<endl;

// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";
	      

	      return label;
	    }
	  else // operator
	    {
	      cout<<"\nname=\t"<<name<<endl;    //debug code

	      if(name == "select")
		{
		  vector<string> ChildrenLabel;

		  iout++;

		  iout++;

		  ChildrenLabel.push_back(printInYicesFormat(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount));
	  
		  iout = node->Get_DAGnode_outlist();

		  iout++;

		  ChildrenLabel.push_back(printInYicesFormat(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount));
		  iout = node->Get_DAGnode_outlist();

		  ChildrenLabel.push_back(printInYicesFormat(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		  if(wit == WidthTableForNodes.end())
		    {
		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
		      exit(1);
		    }
		  int width = wit->second; // width obtained

		  fprintf(fp,"(define %s :: (bitvector %d) (bv-extract %s %s %s))\n",label.c_str(), width, ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      // NB: Assumption here is that not_equal_to applies only between bit-vectors

	      else if(name == "is_not_equal" || name == "not_equal_to")
		{
		  vector<string> ChildrenLabel;
		  ChildrenLabel.push_back(printInYicesFormat(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount));
		  iout++;
		  ChildrenLabel.push_back(printInYicesFormat(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  fprintf(fp,"(define %s :: bool (not(= %s %s)))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "equivalent" || name == "=")
		{
		  if(name ==  "equivalent") name = "=";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInYicesFormat(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  fprintf(fp,"(define %s :: bool (= %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "*" || name == "+" || name == "-")
		{
		  if(name ==  "+") name = "bv-add";
		  else if(name ==  "-") name = "bv-sub";
		  else if(name ==  "*") name = "bv-mul";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInYicesFormat(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount));
		    }

		  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		  if(wit == WidthTableForNodes.end())
		    {
		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
		      exit(1);
		    }
		  int width = wit->second; // width obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  fprintf(fp,"(define %s :: (bitvector %d) (%s %s %s))\n",label.c_str(), width, name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "and" || name == "or" || name == "not" || name == "ite")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInYicesFormat(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(name == "not")
		    {
		      fprintf(fp,"(define %s :: bool (not %s))\n",label.c_str(), ChildrenLabel[0].c_str()); // written
		    }
		  else if(name == "ite")
		    {
		      fprintf(fp,"(define %s :: bool (ite %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		    }
		  else
		    {
		      fprintf(fp,"(define %s :: bool (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
		    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else
		{
		  cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::printInYicesFormat\n";
		  exit(1);
		}
		    
	    }
	}
    }
}






// Printing in Yices : New algorithm on 3-5-2010

string CHelper :: printInYicesFormatModified(CDAGNode *node, string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, map<CDAGNode*, string> &LabelTable, FILE *fp, map<string, int> &WidthTable, int &TempCount, map<CDAGNode*, string> &TypeTableOfITENodes, set<string> &SupportOfUnrolling)
{
  string label;
  string name = node->Get_DAGnode_value();
  bool node_type = node->Get_DAG_node_type();
  unsigned int outlist_size = node->Get_DAGnode_outlist_size();
  list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
  string delta_string = node->Get_DAGnode_delta_time_as_string();
  
  cout<<"\nname=\t"<<name<<endl;    //debug code

  if(node_type && (outlist_size == 0)) // constants 
    {
      if(name == "'1'")
	{
	  label = node->convertLiteralToBitvectorWithProperWidth(name, Width_Required);
	  
	}

      else if(name == "'0'")
	{
	  label = node->convertLiteralToBitvectorWithProperWidth(name, Width_Required);

	}

      else if(context == "const")
	{
	  label = name;
	}
      else if(context == "bv")
	{
	     
	  if(name[0] == 'x') // hex
	    {
		 
	      label = node->convertHexToBitvectorWithProperWidth(name, Width_Required);
	    }
	  else // Decimal
	    {

	      label = node->convertDecimalToBitvectorWithProperWidth(name, Width_Required);
	    }
			  
	}
      else if(context == "bool")
	{
	  if(name == "1") label = "true";
	  else if(name == "0") label = "false";
	  else if(name == "true") label = "true";
	  else if(name == "false") label = "false";

	  else // interpreting a Boolean constant other than true/false as a bit-vector
	    {
	      if(name[0] == 'x') // hex
		{
		  label = node->convertHexToBitvectorWithProperWidth(name, Width_Required);
		}
	      else // Decimal
		{
		  label = node->convertDecimalToBitvectorWithProperWidth(name, Width_Required);
		}
	    }

	}
      else
	{
	  cout<<"\nUnkown name "<<name<<" with context "<<context<<" encountered in function CHelper :: printInYicesFormatModified\n";
	  exit(1);
	}
      return label;
    }

  else // variable or operator
    {
      map<CDAGNode*, string>::iterator it=LabelTable.find(node);
      cout<<"\nnode = "<<node<<", label = "<<label<<endl;
      if(it!=LabelTable.end()) // entry exists for the node
	{
	  cout<<"\nentry exists for "<<node<<endl;
	  label = it->second;
	  return label;
	}
      else
	{
	  if(!node_type && outlist_size == 0)  // variable
	    {
	      label = name + "_" + delta_string; // label obtained
	      map<string, int>::iterator wit = WidthTable.find(label);
	      if(wit == WidthTable.end())
		{
		  cout<<"\nThe variable "<<label<<" has no entry in the WidthTable\n";
		  exit(1);
		}
	      int width = wit->second; // width obtained

	      fprintf(fp,"(define %s :: (bitvector %d))\n",label.c_str(), width); // written

	      cout<<"\n"<<label<<" printed "<<endl;

	      LabelTable.insert(make_pair(node, label));

	      cout<<"\nentry ("<<node<<","<<label<<")"<<" added "<<endl;

// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";

	      SupportOfUnrolling.insert(label);
	      

	      return label;
	    }
	  else // operator
	    {
	      cout<<"\nname=\t"<<name<<endl;    //debug code

	      if(name == "select")
		{
		  vector<string> ChildrenLabel;

		  iout++;

		  iout++;

		  ChildrenLabel.push_back(printInYicesFormatModified(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
	  
		  iout = node->Get_DAGnode_outlist();

		  iout++;

		  ChildrenLabel.push_back(printInYicesFormatModified(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		  iout = node->Get_DAGnode_outlist();

		  ChildrenLabel.push_back(printInYicesFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  int child_1_width, child_2_width, width;

		  child_1_width = atoi(ChildrenLabel[0].c_str());
		  child_2_width = atoi(ChildrenLabel[1].c_str());
		  width = child_1_width - child_2_width + 1;


		 //  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
// 		  if(wit == WidthTableForNodes.end())
// 		    {
// 		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
// 		      exit(1);
// 		    }
// 		  int width = wit->second; // width obtained

		  fprintf(fp,"(define %s :: (bitvector %d) (bv-extract %s %s %s))\n",label.c_str(), width, ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      // NB: Assumption here is that not_equal_to applies only between bit-vectors

	      else if(name == "is_not_equal" || name == "not_equal_to")
		{
		  vector<string> ChildrenLabel;
		  ChildrenLabel.push_back(printInYicesFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		  iout++;
		  ChildrenLabel.push_back(printInYicesFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  fprintf(fp,"(define %s :: bool (not(= %s %s)))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "equivalent" || name == "=")
		{
		  if(name ==  "equivalent") name = "=";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInYicesFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  fprintf(fp,"(define %s :: bool (= %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "*" || name == "+" || name == "-")
		{
		  if(name ==  "+") name = "bv-add";
		  else if(name ==  "-") name = "bv-sub";
		  else if(name ==  "*") name = "bv-mul";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInYicesFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		  if(wit == WidthTableForNodes.end())
		    {
		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
		      exit(1);
		    }
		  int width = wit->second; // width obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  fprintf(fp,"(define %s :: (bitvector %d) (%s %s %s))\n",label.c_str(), width, name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "and" || name == "or" || name == "not")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInYicesFormatModified(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(name == "not")
		    {
		      fprintf(fp,"(define %s :: bool (not %s))\n",label.c_str(), ChildrenLabel[0].c_str()); // written
		    }
		  else
		    {
		      fprintf(fp,"(define %s :: bool (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
		    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}

	      else if(name == "ite")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInYicesFormatModified(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(TypeTableOfITENodes[node]=="bv") // should be written as bv; get the width
		    {
		      map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		      if(wit == WidthTableForNodes.end())
			{
			  cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
			  exit(1);
			}
		      int width = wit->second; // width obtained

		      fprintf(fp,"(define %s :: (bitvector %d) (ite %s %s %s))\n",label.c_str(), width, ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		      
		    }
		  else
		    {
		      fprintf(fp,"(define %s :: bool (ite %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else
		{
		  cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::printInYicesFormatModified\n";
		  exit(1);
		}
		    
	    }
	}
    }
}





// set the types of the nodes
  
string CHelper :: setTypes(CDAGNode *tree, string context, map<CDAGNode*, string> &TypeTableOfITENodes)
{
 
  string name = tree->Get_DAGnode_value();
  bool node_type = tree->Get_DAG_node_type();
  unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
  list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
  string delta_string = tree->Get_DAGnode_delta_time_as_string();
  

  if(!node_type)
    {
      return "bv";
    }
  else
    {

      //cout<<"\nNAME=\t"<<name<<endl;    //debug code

      if(name == "select")
	{
	  iout++;

	  iout++;
	  
	  setTypes(*iout, "const", TypeTableOfITENodes);

	  iout = tree->Get_DAGnode_outlist();

	  iout++;

	  setTypes(*iout, "const", TypeTableOfITENodes);

	  iout = tree->Get_DAGnode_outlist();

	  setTypes(*iout, "bv", TypeTableOfITENodes);

	  return "bv";

	}
      
      else if(name == "is_not_equal" || name == "not_equal_to")
	{
	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      setTypes(*iout, "bv", TypeTableOfITENodes);
	    }

	  return "bool";
	}

      // NB: Assumption here is that equivalent applies only between bit-vectors

      else if(name == "equivalent" || name == "=")
	{
	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      setTypes(*iout, "bv", TypeTableOfITENodes);
	    }

	  return "bool";
	}

      
      else if(name == "*")
	{
	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      setTypes(*iout, "bv", TypeTableOfITENodes);
	    }

	  return "bv";

	}

      else if(name == "+")
	{
	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      setTypes(*iout, "bv", TypeTableOfITENodes);
	    }

	  return "bv";

	}


      // Newly added on 20.1.2010

      else if(name == "-")
	{
	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      setTypes(*iout, "bv", TypeTableOfITENodes);
	    }

	  return "bv";

	}

      
      else if(name == "and" || (name == "or") || (name == "not"))
	{
	  
	  for(unsigned int i=0; i<outlist_size; i++, iout++)
	    {
	      setTypes(*iout, "bool", TypeTableOfITENodes);
	    }

	  return "bool";
	}

      else if(name == "ite")
	{
	  setTypes(*iout, "bool", TypeTableOfITENodes);

	  iout++;

	  string ChildType;

	  for(unsigned int i=1; i<outlist_size; i++,iout++)
	    {
	      ChildType = setTypes(*iout, "bool", TypeTableOfITENodes);
	    }

	  TypeTableOfITENodes.insert(make_pair(tree, ChildType));

	  return ChildType;

	}

      else if(name == "'1'")
	{
	  return "bv";
	}

      else if(name == "'0'")
	{
	  return "bv";
	}

      else if(outlist_size == 0) // Other constants

	{
		
	  if(context == "const")
	    {
	      return "const";
	    }
	  else if(context == "bv")
	    {
	      return "bv";	
	    }
	  else if(context == "bool")
	    {
	      if(name == "1") return "bool";
	      else if(name == "0") return "bool";
	      else if(name == "true") return "bool";
	      else if(name == "false") return "bool";
	      //else dag_str = name;

	      else // interpreting a Boolean constant other than true/false as a bit-vector
		{
		  if(name[0] == 'x') // hex
		    {
		      return "bv";
		    }
		  else // Decimal
		    {
		      return "bv";
		    }
		}

	    }
	  else
	    {
	      cout<<"\nUnanticipated constant "<<name<<" inside function setTypes\n";
	      exit(1);
	    }
		  
	}
      else
	{
	  cout<<"\nUnanticipated operator "<<name<<" inside function setTypes\n";
	  exit(1);
	}
		    
    }

}
  


/* Functions added on 14 June 2010 to print the transition relation in SMTLib format */


void CHelper::writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggle(FILE *smt_file, map<string, int> WidthTable)

{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {
	  string id_delta = it->first;
	  fprintf(smt_file,":extrafuns ((%s",id_delta.c_str());
	  fprintf(smt_file," BitVec[%d]))\n",it->second);
	}
 
}

void CHelper :: printTRInFile(CDAGNode *TR, map<string, int> WidthTable)
{

  //cout<<"\nTR="<<TR->getDAGStr()<<endl;
        
  string SmtFileName = TRFileName;

  FILE *Myfile = fopen(SmtFileName.c_str(), "w");

  if(Myfile == NULL) 
    {cout<<"\nCannot create "<<SmtFileName<<"\n"; exit(1);}

  fprintf(Myfile,"(benchmark tr\n");
	
  writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggle(Myfile, WidthTable);

  string TR_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(TR, WidthTable);

  fprintf(Myfile,"\n\n:formula %s\n\n)\n\n\n", TR_str.c_str());

  fclose(Myfile);
   
}


void CHelper :: printQEliminatorResultsInFile(CDAGNode *original, map<string, int> WidthTable, list<string> VarsToElim)
{
  static int count=0;
        
  set<string> VarsToElimSet;
  createSetFromList(VarsToElimSet, VarsToElim);

  string SmtFileName = "case";
  char count_chr[100];
  sprintf(count_chr,"%d",count);
  string count_str = count_chr;
  SmtFileName += "_";
  SmtFileName += count_str;
  SmtFileName += ".smt";

  bool delta_toggling=false;


  FILE *Myfile = fopen(SmtFileName.c_str(), "w");

  if(Myfile == NULL) 
    {cout<<"\nCannot create case.smt\n"; exit(1);}

  fprintf(Myfile,"(benchmark test\n");

  if(delta_toggling)
    {
	
      writeBVDeclarationsFromWidthTableInSMTLibFormat(Myfile, WidthTable);

      original = cloneWithDeltaToggled(original);
	
      set<string> VarsToElimSetToggled;
      getDeltaToggledSet(VarsToElimSet, VarsToElimSetToggled);
      string original_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(original, WidthTable);

      fprintf(Myfile,"\n:exists (");
      for(set<string>::iterator it=VarsToElimSet.begin(); it!=VarsToElimSet.end(); it++)
	{
	  string id_delta = *it;
	  int index = id_delta.find_last_of("_");
	  string id = id_delta.substr(0, index);
	  int delta = atoi(id_delta.substr(index+1).c_str());
	  delta = delta*-1;
	  fprintf(Myfile," %s_%d ", id.c_str(), delta);
	}
      fprintf(Myfile,")");


      fprintf(Myfile,"\n\n:formula %s)\n\n\n", original_str.c_str());
    }
  else
    {
      	
      writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggle(Myfile, WidthTable);

      fprintf(Myfile,"\n:exists (");
      for(set<string>::iterator it=VarsToElimSet.begin(); it!=VarsToElimSet.end(); it++)
	{
	  string id_delta = *it;
	  fprintf(Myfile," %s ", id_delta.c_str());
	}
      fprintf(Myfile,")");

      string original_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(original, WidthTable);

      fprintf(Myfile,"\n\n:formula %s\n\n)\n\n\n", original_str.c_str());
    }
      
  fclose(Myfile);

  count++;
    
}




string CHelper :: printInSMTLibFormatModified(CDAGNode *node, string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, map<CDAGNode*, string> &LabelTable, FILE *fp, map<string, int> &WidthTable, int &TempCount, map<CDAGNode*, string> &TypeTableOfITENodes, set<string> &SupportOfUnrolling)
{
  string label;
  string name = node->Get_DAGnode_value();
  bool node_type = node->Get_DAG_node_type();
  unsigned int outlist_size = node->Get_DAGnode_outlist_size();
  list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
  string delta_string = node->Get_DAGnode_delta_time_as_string();
  
  //cout<<"\nname=\t"<<name<<endl;    //debug code

  if(node_type && (outlist_size == 0)) // constants 
    {
      if(name == "'1'")
	{
	  label = node->convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);
	  
	}

      else if(name == "'0'")
	{
	  label = node->convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);

	}

      else if(context == "const")
	{
	  label = name;
	}
      else if(context == "bv")
	{
	     
	  if(name[0] == 'x') // hex
	    {
		 
	      label = node->convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
	    }
	  else // Decimal
	    {

	      label = node->convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
	    }
			  
	}
      else if(context == "bool")
	{
	  if(name == "1") label = "true";
	  else if(name == "0") label = "false";
	  else if(name == "true") label = "true";
	  else if(name == "false") label = "false";

	  else // interpreting a Boolean constant other than true/false as a bit-vector
	    {
	      if(name[0] == 'x') // hex
		{
		  label = node->convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		}
	      else // Decimal
		{
		  label = node->convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		}
	    }

	}
      else
	{
	  cout<<"\nUnkown name "<<name<<" with context "<<context<<" encountered in function CHelper :: printInSMTLibFormatModified\n";
	  exit(1);
	}
      return label;
    }

  else // variable or operator
    {
      map<CDAGNode*, string>::iterator it=LabelTable.find(node);
      //cout<<"\nnode = "<<node<<", label = "<<label<<endl;
      if(it!=LabelTable.end()) // entry exists for the node
	{
	  //cout<<"\nentry exists for "<<node<<endl;
	  label = it->second;
	  return label;
	}
      else
	{
	  if(!node_type && outlist_size == 0)  // variable
	    {
	      label = name + "_" + delta_string; // label obtained
	      map<string, int>::iterator wit = WidthTable.find(label);
	      if(wit == WidthTable.end())
		{
		  cout<<"\nThe variable "<<label<<" has no entry in the WidthTable\n";
		  exit(1);
		}
	      int width = wit->second; // width obtained

	      if(!(name=="abstraction" && qelim_bit_level))
		{
		  fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written
		}

	      //cout<<"\n"<<label<<" printed "<<endl;

	      LabelTable.insert(make_pair(node, label));

	      //cout<<"\nentry ("<<node<<","<<label<<")"<<" added "<<endl;

// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";

	      SupportOfUnrolling.insert(label);
	      

	      return label;
	    }
	  else // operator
	    {
	      //cout<<"\nname=\t"<<name<<endl;    //debug code

	      if(name == "select")
		{
		  vector<string> ChildrenLabel;

		  iout++;

		  iout++;

		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
	  
		  iout = node->Get_DAGnode_outlist();

		  iout++;

		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		  iout = node->Get_DAGnode_outlist();

		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  int child_1_width, child_2_width, width;

		  child_1_width = atoi(ChildrenLabel[0].c_str());
		  child_2_width = atoi(ChildrenLabel[1].c_str());
		  width = child_1_width - child_2_width + 1;


		 //  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
// 		  if(wit == WidthTableForNodes.end())
// 		    {
// 		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
// 		      exit(1);
// 		    }
// 		  int width = wit->second; // width obtained

		  fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		  fprintf(fp,":assumption (= %s (extract [%s:%s] %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      // NB: Assumption here is that not_equal_to applies only between bit-vectors

	      else if(name == "is_not_equal" || name == "not_equal_to")
		{
		  vector<string> ChildrenLabel;
		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		  iout++;
		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		  fprintf(fp,":assumption (iff  %s (not (= %s %s)))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
	  
		  
		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "equivalent" || name == "=")
		{
		  if(name ==  "equivalent") name = "=";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		  fprintf(fp,":assumption (iff %s (= %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
	  
		  

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
              else if(name == "<=" || name == ">="|| name == "<" || name == ">")
		{
		  if(name ==  "<=") name = "bvule";
		  else if(name ==  ">=") name = "bvuge";
		  else if(name ==  "<") name = "bvult";
		  else name = "bvugt";

	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		  fprintf(fp,":assumption (iff %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
	  
		  

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "*" || name == "+" || name == "-")
		{
		  if(name ==  "+") name = "bvadd";
		  else if(name ==  "-") name = "bvsub";
		  else if(name ==  "*") name = "bvmul";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		  if(wit == WidthTableForNodes.end())
		    {
		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
		      exit(1);
		    }
		  int width = wit->second; // width obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		  fprintf(fp,":assumption (= %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "and" || name == "or" || name == "not")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(name == "not")
		    {
		      fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		      fprintf(fp,":assumption (iff %s (not %s))\n",label.c_str(), ChildrenLabel[0].c_str()); // written
		      
		    }
		  else
		    {
		      fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		      fprintf(fp,":assumption (iff %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
		    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}

	      else if(name == "ite")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfITENodes, SupportOfUnrolling));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(TypeTableOfITENodes[node]=="bv") // should be written as bv; get the width
		    {
		      map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		      if(wit == WidthTableForNodes.end())
			{
			  cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
			  exit(1);
			}
		      int width = wit->second; // width obtained

		      fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		      fprintf(fp,":assumption (= %s (ite %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		      
		    }
		  else
		    {
		      fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		      fprintf(fp,":assumption (iff %s (if_then_else %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else
		{
		  cout<<"\nUnanticipated operator "<<name<<" inside function CHelper::printInSMTLibFormatModified\n";
		  exit(1);
		}
		    
	    }
	}
    }
}


void CHelper::printDeltaTRInFile(CDAGNode *DeltaTR, map<string, int> &WidthTable)
{

  //cout<<"\nDeltaTR="<<DeltaTR->getDAGStr()<<endl;
        
  string SmtFileName = "DeltaTR";
  SmtFileName += ".smt";

  FILE *Myfile = fopen(SmtFileName.c_str(), "w");

  if(Myfile == NULL) 
    {cout<<"\nCannot create "<<SmtFileName<<"\n"; exit(1);}

  fprintf(Myfile,"(benchmark tr\n");
	
  writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggle(Myfile, WidthTable);

  string DeltaTR_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(DeltaTR, WidthTable);

  fprintf(Myfile,"\n\n:formula %s\n\n)\n\n\n", DeltaTR_str.c_str());

  fclose(Myfile);
   
}



// ADDED ON 11-1-2011


void CHelper :: printQEliminatorResultsInGivenFile(CDAGNode *original, map<string, int> WidthTable, list<string> VarsToElim, string SMTFileName)
{
    
  set<string> VarsToElimSet;
  createSetFromList(VarsToElimSet, VarsToElim);

  bool delta_toggling=false;


  FILE *Myfile = fopen(SMTFileName.c_str(), "w");

  if(Myfile == NULL) 
    {cout<<"\nCannot create case.smt\n"; exit(1);}

  fprintf(Myfile,"(benchmark test\n");

  if(delta_toggling)
    {
	
      writeBVDeclarationsFromWidthTableInSMTLibFormat(Myfile, WidthTable);

      original = cloneWithDeltaToggled(original);
	
      set<string> VarsToElimSetToggled;
      getDeltaToggledSet(VarsToElimSet, VarsToElimSetToggled);
      string original_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(original, WidthTable);

      fprintf(Myfile,"\n:exists (");
      for(set<string>::iterator it=VarsToElimSet.begin(); it!=VarsToElimSet.end(); it++)
	{
	  string id_delta = *it;
	  int index = id_delta.find_last_of("_");
	  string id = id_delta.substr(0, index);
	  int delta = atoi(id_delta.substr(index+1).c_str());
	  delta = delta*-1;
	  fprintf(Myfile," %s_%d ", id.c_str(), delta);
	}
      fprintf(Myfile,")");


      fprintf(Myfile,"\n\n:formula %s)\n\n\n", original_str.c_str());
    }
  else
    {
      	
      writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggle(Myfile, WidthTable);

      fprintf(Myfile,"\n:exists (");
      for(set<string>::iterator it=VarsToElimSet.begin(); it!=VarsToElimSet.end(); it++)
	{
	  string id_delta = *it;
	  fprintf(Myfile," %s ", id_delta.c_str());
	}
      fprintf(Myfile,")");

      string original_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(original, WidthTable);

      fprintf(Myfile,"\n\n:formula %s\n\n)\n\n\n", original_str.c_str());
    }
      
  fclose(Myfile);
    
}

// ADDED ON 11-1-2011 ends here




// ADDED ON 1-2-2011 

bool CHelper :: call_lmdeEvaluator(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable)
{
  if(node == NULL)
    {
     cout<<"Function CHelper :: call_lmdeEvaluator called with NULL node\n";
     exit(1);
    } 
    // find width
  string variable = "";
  variable = node->getSingleVariable();
  if(variable=="")
    {
      cout<<"\nLME/LMD "<<node->getDAGStr()<<" encountered with no variable names in function CHelper :: call_lmdeEvaluator\n";
      exit(1);
    }
  
  map<string, int>::iterator MyWidthTable_it = MyWidthTable.find(variable);
  if(MyWidthTable_it == MyWidthTable.end())
	    {
	      cout<<"\nNo entry in MyWidthTable for variable "<<variable<<" in function CHelper :: call_lmdeEvaluator\n";
	      exit(1);
	    }
  int width = MyWidthTable_it->second;

  bool boolean_result;
  bvatom bv_result = lmdeEvaluator(node, width, Variable_Value_Map, boolean_result);
  if(!boolean_result) 
    {
      return false;
    }

  return true; 
}

// Note that lmdeEvaluator which takes only decimal constants

bvatom CHelper :: lmdeEvaluator(CDAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result)
    {
      bvatom bv_result;
      string dag_str;
      unsigned int outlist_size;

      dag_str = node->Get_DAGnode_value();
      outlist_size = node->Get_DAGnode_outlist_size();


      if(node->Get_DAG_node_type()==false && outlist_size==0)  //i.e. if an operand node
	{

	  dag_str += "_";
	  dag_str += node->Get_DAGnode_delta_time_as_string();

	  map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(dag_str);
	  if(Variable_Value_Map_it == Variable_Value_Map.end())
	    {
	      cout<<"\nNo entry in Variable_Value_Map for variable "<<dag_str<<" in function CHelper :: lmdeEvaluator\n";
	      exit(1);
	    }
	  string bin_value = Variable_Value_Map_it->second; // binary value of the variable from map 
	  bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()
	  if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	    {
	      bv_result = bvm->BVExtend(result, width, 0);
	    }
	  else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	    {
	      bv_result = bvm->select_with_int(result, width-1, 0);
	    }
	  else
	    {
	      bv_result = result;
	    }
	} //operand node ends here

      else   //i.e. if an operator node or a constant node
        {
            list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
	    
	    if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
		{
		  bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained
		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained
		  if (inter_bool_result) { // reverse the result as it is is_not_equal
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		    }
		}// "is_not_equal"/"not_equal_to" ends here

	      else if(dag_str == "equivalent" || dag_str == "=")
		{
		  bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

		  if (inter_bool_result) { // no need to reverse
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  

		  }
		}// "equivalent"/"=" ends here

	      else if(dag_str == "*")
		{
		  if((*iout)->Get_DAGnode_value()=="1")// multiplication by 1
		    {
		      //cout<<"\nmultiplication by 1\n";
		      iout++;
		      bv_result = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained - no need to select. op2 is already of proper width
		    }
		  else
		    {

		      bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		      iout++;
		      bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

		      bvatom result = bvm->arith_mult(op1, op2); // result obtained
		      bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
		    } 
		}// "*" ends here

	    else if(dag_str == "+")
		{
		  bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

		  bvatom result = bvm->arith_add(op1, op2); // result obtained
		  bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
 
		}// "+" ends here

	    else if(outlist_size==0) // constants
	      {
		int dec_value = atoi(dag_str.c_str()); // decimal value of the constant obtained from the dag node
		string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
		bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

		if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
		  {
		    bv_result = bvm->BVExtend(result, width, 0);
		  }
		else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		  {
		    bv_result = bvm->select_with_int(result, width-1, 0);
		  }
		else
		  {
		    bv_result = result;
		  }
	      }// "constants" ends here
	    else
	      {
		cout<<"\nUnanticipated operator "<<dag_str<<" inside function CHelper :: lmdeEvaluator. Note that presently the function CHelper :: lmdeEvaluator is restricted to pure LMDEs\n";
		exit(1);
	      }
	}// if an operator/constant node ends here


  return bv_result;
		    
}// function ends here


// evaluate can be used to evaluate dags which are in propositional skelton form
// In other words, the allowed dag is like

// dag ---> dag /\ dag | dag \/ dag | !dag | ite(dag, dag, dag) | lmde

// Thus the result is always Boolean

bool CHelper :: evaluate(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable)
{
  map<CDAGNode*, bool> DPTable;
  DPTable.clear();
  bool ret_value = evaluate_Internal(node, Variable_Value_Map, MyWidthTable, DPTable);
  return ret_value; 
}


bool CHelper :: evaluate_Internal(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, map<CDAGNode*, bool> &DPTable)
{

  map<CDAGNode*, bool> ::iterator dpit = DPTable.find(node);
 
  if(dpit!=DPTable.end()) // node already encountered
    {
      //cout<<"\nDPTable Hit\n";
      return dpit->second;
    }

  else { // encountered first time

    bool ret_value;
    string dag_str;
    unsigned int outlist_size;

    dag_str = node->Get_DAGnode_value();
    outlist_size = node->Get_DAGnode_outlist_size();

    if(dag_str == "=" || dag_str == "is_not_equal" || dag_str == "not_equal_to" || dag_str == "equivalent") // LME/LMD
      {
	ret_value = call_lmdeEvaluator(node, Variable_Value_Map, MyWidthTable);
      }
    else if(outlist_size != 0)// Internal node
      {
      
	if(dag_str == "and" && outlist_size  == 2) // binary and
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bool child1 =  evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bool child2 = evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 

	    if(!child1)
	    
	      { 
		ret_value = false;
	      }
	    
	    else if(!child2)
	    
	      { 
		ret_value = false;
	      }

	    else
	      { 
		ret_value = true;
	      }
	    
	  }//binary and
      
	else if(dag_str == "or" && outlist_size  == 2) // binary or
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bool child1 =  evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bool child2 = evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 

	    if(child1)
	    
	      { 
		ret_value = true;
	      }
	    
	    else if(child2)
	    
	      { 
		ret_value = true;
	      }

	    else
	      { 
		ret_value = false;
	      }
	    
	  }// binary or

	else if(dag_str == "ite")
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bool child1 =  evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bool child2 = evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 

	    iout++;

	    bool child3 = evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 


	    if(child1 && child2)

	      { 
		ret_value = true;
	      }
	
	    else if(!child1 && child3)

	      { 
		ret_value = true;
	      }
	  
	    else if(child1 && !child2)

	      { 
		ret_value = false;
	      }
	
	    else if(!child1 && !child3)
	    
	      { 
		ret_value = false;
	      }

	    else if(!child2 && !child3)

	      { 
		ret_value = false;
	      }
	  
	  
	    else
	      { 
		cout<<"\nIllegal combination in ite in function CHelper :: evaluate_Internal\n";
		exit(1);
	      }
	  }// ite


	else if(dag_str == "not")
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bool child1 =  evaluate_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  

	    if(child1)
	    
	      { 
		ret_value = false;
	      }
	    
	    else     
	      { 
		ret_value = true;
	      }

	  }// not

	else
	  {
	    cout<<"\nUnknown operator "<<dag_str<<" in "<<node->getDAGStr()<<" in function CHelper :: evaluate_Internal\n";
	    exit(1);
	  }
       

      }// internal node

    else // leaf
      {
	if(dag_str == "true")
	  {
	    ret_value = true;
	  }
	else if(dag_str == "false")
	  {
	    ret_value = false;
	  }
	else
	  {
	    cout<<"\nUnknown node "<<dag_str<<" in function CHelper :: evaluate_Internal\n";
	    exit(1);
	  }
      }// leaf

    DPTable.insert(make_pair(node, ret_value));

    return ret_value;
  }// DPTable miss

}// Function ends 



// ADDED ON 1-2-2011 ends here 



// ADDED ON 7-2-2011 starts here 



// evaluateFunction can be used to evaluate dags which are in the following functional form

// dag ---> ite(dag1, dag, dag) | bit-vector

// dag1 ---> dag1 /\ dag1 | dag1 \/ dag1 | !dag1 | lmde

// Thus the result is always a bit-vector (returned as a string)

string CHelper :: evaluateFunction(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable)
{

  // convert node to a node which is free of subtractions

  bool make_node_substraction_free = true;

  if(make_node_substraction_free)
    {
       map<CDAGNode*, CDAGNode*> SubtractionTable;
       //cout<<"\nnode before replacing substractions = "<<node->getDAGStr()<<endl;
      SubtractionTable.clear();
      node = getSubtractionReplacedClone(node, SubtractionTable, MyWidthTable);
      //cout<<"\nnode after replacing substractions = "<<node->getDAGStr()<<endl;;
    }


  // make node point to the RHS of the function

  if(node->Get_DAGnode_value() == "="  || node->Get_DAGnode_value() == "equivalent")
    {
      list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
      iout++;
      node = *iout;
    }
  else
    {
      cout<<"\nArgument dag is not a function in CHelper :: evaluateFunction\n";
      exit(1);
    }
  


  map<CDAGNode*, bvatom> DPTable;
  DPTable.clear();
  bvatom ret_value = evaluateFunction_Internal(node, Variable_Value_Map, MyWidthTable, DPTable);
  // printing the result
  cout<<"\nret_value = ";
  bvm->printBVatom(ret_value, 0);
  // convert ret_bv_value from bvatom to string
  string hex_str = bvm->convertBVToHexString(ret_value);
  cout<<"\nhex_str = "<<hex_str<<endl;
  unsigned long long int dec_value = hexStringToULINT(hex_str); // decimal value of the hex string
  cout<<"\ndec_value = "<<dec_value<<endl;
  string ret_value_str = ULINTToBinaryString(dec_value); // binary value with minimum required width
  cout<<"\nret_value_str = "<<ret_value_str<<endl;
  return ret_value_str;
}


bvatom CHelper :: evaluateFunction_Internal(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, map<CDAGNode*, bvatom> &DPTable)
{

  map<CDAGNode*, bvatom> ::iterator dpit = DPTable.find(node);
 
  if(dpit!=DPTable.end()) // node already encountered
    {
      cout<<"\nDPTable Hit\n";
      return dpit->second;
    }

  else { // encountered first time

    cout<<"\nDPTable Miss\n";
    bvatom ret_value;
    string dag_str;
    unsigned int outlist_size;

    dag_str = node->Get_DAGnode_value();
    outlist_size = node->Get_DAGnode_outlist_size();


    cout<<"\n"<<dag_str<<" node encountered in evaluateFunction_Internal\n";

    if(dag_str == "=" || dag_str == "is_not_equal" || dag_str == "not_equal_to" || dag_str == "equivalent") // LME/LMD
      {
	ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
      }
    else if(outlist_size != 0)// Internal node
      {
      
	if(dag_str == "and" && outlist_size  == 2) // binary and
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bvatom child2 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);

	    ret_value = bvm->bitwise_and(child1, child2);

	  }//binary and
      
	else if(dag_str == "or" && outlist_size  == 2) // binary or
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bvatom child2 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 

	    ret_value = bvm->bitwise_or(child1, child2);

	  }// binary or

	else if(dag_str == "ite")
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bvatom child2 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 

	    iout++;

	    bvatom child3 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 


	    bvatom true_value = bvm->getZeroedBVatom(1);
	    true_value.bv_array[0] = (bv_block_t) 1;


	    if(bvm->checkBVEquality(child1, true_value))// child1 is true
	      { 
		cout<<"\nchild1 is true\n";
		ret_value = child2;
	      }
	
	    else 
	      { 
		cout<<"\nchild1 is false\n";
		ret_value = child3;
	      }
	  
	  }// ite


	else if(dag_str == "not")
	  {
	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    ret_value = bvm->bitwise_negate(child1);
	    
	  }// not
	else if(dag_str == "*" || dag_str == "+")
	  {
	    ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
	  }// * and +
	else if(dag_str == "select")
	  {
	    ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
	  }// select

	else
	  {
	    cout<<"\nUnknown operator "<<dag_str<<" in function CHelper :: evaluateFunction_Internal\n";
	    exit(1);
	  }
       

      }// internal node

    else // constants and variables
      {
	if(node->Get_DAG_node_type()==false)  //i.e. if variable
	  {
	    ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
	    
	  } //variable node ends here

	else // constants
	  {
	    if(dag_str == "true" || dag_str == "'1'")
	      {
		ret_value = bvm->getZeroedBVatom(1);
		ret_value.bv_array[0] = (bv_block_t) 1;
	      }
	    else if(dag_str == "false" || dag_str == "'0'")
	      {
		ret_value = bvm->getZeroedBVatom(1);
		ret_value.bv_array[0] = (bv_block_t) 0;
	      }
	    else
	      {
		if(dag_str[0]=='x')// hex constant
		  {
		    dag_str.erase(dag_str.begin(), dag_str.begin()+2);
		    dag_str.erase(dag_str.end()-1, dag_str.end());
		    int width = dag_str.length()*4;

		    // trim leading zeros from dag_str: let's see what happens without this
		    unsigned long long int dec_value = hexStringToULINT(dag_str); // decimal value of the constant obtained from the dag node
		    string bin_value = ULINTToBinaryString(dec_value); // binary value with minimum required width
		    bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

		    if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
		      {
			ret_value = bvm->BVExtend(result, width, 0);
		      }
		    else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		      {
			ret_value = bvm->select_with_int(result, width-1, 0);
		      }
		    else
		      {
			ret_value = result;
		      }
		  }// hex constant
		else
		  {
		    cout<<"\nUnknown constant "<<dag_str<<" in function CHelper :: evaluateFunction_Internal\n";
		    exit(1);
		  }
		
	      }// else
	  }//constants
      }//constants and variables

    DPTable.insert(make_pair(node, ret_value));

    cout<<"\nret_value from "<<dag_str<<" for evaluateFunction_Internal = ";
    bvm->printBVatom(ret_value, 0);

    return ret_value;
  }// DPTable miss

}// Function ends 



bvatom CHelper :: call_lmdeEvaluatorReturningBV(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable)
{
  //cout<<"\ncall_lmdeEvaluatorReturningBV called\n";
  
  if(node == NULL)
    {
     cout<<"Function CHelper :: call_lmdeEvaluatorReturningBV called with NULL node\n";
     exit(1);
    } 

  int width;
  
  bool scheme_with_selections = true;

  // find width
  if(scheme_with_selections)
    {
      width = getWidthOfLMDEWithSelects(node, MyWidthTable);
      
      cout<<"\nwidth = "<<width<<endl;
    }

  else
    {
      string variable = "";
      variable = node->getSingleVariable();
      if(variable=="")
	{
	  cout<<"\nLME/LMD "<<node->getDAGStr()<<" encountered with no variable names in function CHelper :: call_lmdeEvaluatorReturningBV\n";
	  exit(1);
	}
  
      map<string, int>::iterator MyWidthTable_it = MyWidthTable.find(variable);
      if(MyWidthTable_it == MyWidthTable.end())
	{
	  cout<<"\nNo entry in MyWidthTable for variable "<<variable<<" in function CHelper :: call_lmdeEvaluatorReturningBV\n";
	  exit(1);
	}
      width = MyWidthTable_it->second;
    }

  bool boolean_result;

  bvatom bv_result = lmdeEvaluatorModified(node, width, Variable_Value_Map, boolean_result, MyWidthTable);

  //cout<<"\ncall_lmdeEvaluatorReturningBV finished\n";

  return bv_result; 
}


// lmdeEvaluatorModified is lmdeEvaluator which takes any type of constants - not only decimal constants

bvatom CHelper :: lmdeEvaluatorModified(CDAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result, map<string, int> &MyWidthTable)
    {
      bvatom bv_result;
      string dag_str;
      unsigned int outlist_size;

      dag_str = node->Get_DAGnode_value();
      outlist_size = node->Get_DAGnode_outlist_size();

      cout<<"\n"<<dag_str<<" node encountered in lmdeEvaluatorModified\n";


      if(node->Get_DAG_node_type()==false && outlist_size==0)  //i.e. if an operand node
	{

	  dag_str += "_";
	  dag_str += node->Get_DAGnode_delta_time_as_string();

	  cout<<"\ndag_str = "<<dag_str<<endl;

	  map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(dag_str);
	  if(Variable_Value_Map_it == Variable_Value_Map.end())
	    {
	      cout<<"\nNo entry in Variable_Value_Map for variable "<<dag_str<<" in function CHelper :: lmdeEvaluatorModified\n";
	      exit(1);
	    }
	  string bin_value = Variable_Value_Map_it->second; // binary value of the variable from map 

	  cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

	  bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()

	  cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

	  if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	    {
	      cout<<"\nbin_value.size() < width\n";
	      bv_result = bvm->BVExtend(result, width, 0);
	    }
	  else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	    {
	      cout<<"\nbin_value.size() < width\n";
	      bv_result = bvm->select_with_int(result, width-1, 0);
	    }
	  else
	    {
	      cout<<"\nbin_value.size() = width\n";
	      bv_result = result;
	    }


	  //cout<<"\nbv_result = "<<endl;
	  //bvm->printBVatom(bv_result, 0);

	} //operand node ends here

      else   //i.e. if an operator node or a constant node
        {
            list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
	    
	    if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
		{
		  bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op2 obtained
		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained
		  if (inter_bool_result) { // reverse the result as it is is_not_equal
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		    }
		}// "is_not_equal"/"not_equal_to" ends here

	      else if(dag_str == "equivalent" || dag_str == "=")
		{
		  bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op2 obtained

		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

		  if (inter_bool_result) { // no need to reverse
		    cout<<"\nCondition is true\n";
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		  }
		  else {
		    cout<<"\nCondition is false\n";
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  

		  }
		}// "equivalent"/"=" ends here

	      else if(dag_str == "*")
		{
		  if((*iout)->Get_DAGnode_value()=="1")// multiplication by 1
		    {
		      //cout<<"\nmultiplication by 1\n";
		      iout++;
		      bv_result = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op2 obtained - no need to select. op2 is already of proper width
		    }
		  else
		    {

		      bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op1 obtained
		      iout++;
		      bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op2 obtained

		      bvatom result = bvm->arith_mult(op1, op2); // result obtained
		      bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
		    } 
		}// "*" ends here

	    else if(dag_str == "+")
		{
		  bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result, MyWidthTable); // op2 obtained

		  bvatom result = bvm->arith_add(op1, op2); // result obtained
		  bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
 
		}// "+" ends here

	    else if(dag_str == "select")
	      {
		CDAGNode *operand = *iout;
		iout++;
		CDAGNode *low_sel = *iout;
		iout++;
		CDAGNode *high_sel = *iout;

		string operand_string = operand->Get_DAGnode_value();
		operand_string += "_";
		operand_string += operand->Get_DAGnode_delta_time_as_string();

		string low_sel_string = low_sel->Get_DAGnode_value();
		string high_sel_string = high_sel->Get_DAGnode_value();

		int low_sel_number = atoi(low_sel_string.c_str());
		int high_sel_number = atoi(high_sel_string.c_str());


		

		map<string, int>::iterator wit = MyWidthTable.find(operand_string);
		if(wit == MyWidthTable.end())
		  {
		    cout<<"\nNo entry for "<<operand_string<<" in MyWidthTable in function CHelper::lmdeEvaluatorModified\n";
		    exit(1);
		  }
		int OperandWidth = wit->second;


      
		map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(operand_string);
		if(Variable_Value_Map_it == Variable_Value_Map.end())
		  {
		    cout<<"\nNo entry in Variable_Value_Map for variable "<<operand_string<<" in function CHelper::lmdeEvaluatorModified\n";
		    exit(1);
		  }
		string bin_value = Variable_Value_Map_it->second; // binary value of the variable from map 

		cout<<"\noperand_string = "<<operand_string<<"\tlow_sel_number = "<<low_sel_number<<"\thigh_sel_number = "<<high_sel_number<<endl;
		cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

		bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()
		cout<<"\nresult = "<<endl;
		bvm->printBVatom(result, 0);


		//cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

		if(bin_value.size() < OperandWidth) // bin_value needs to be zero-extended to match width
		  {
		    //cout<<"\nbin_value.size() < width\n";
		    bv_result = bvm->BVExtend(result, OperandWidth, 0);
		  }
		else if(bin_value.size() > OperandWidth) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		  {
		    //cout<<"\nbin_value.size() < width\n";
		    bv_result = bvm->select_with_int(result, OperandWidth-1, 0);
		  }
		else
		  {
		    //cout<<"\nbin_value.size() = width\n";
		    bv_result = result;
		  }

		cout<<"\nbv_result = "<<endl;
		bvm->printBVatom(bv_result, 0);


		// Now we have obtained the bv_result from the operand part; let us take the selection part also

		bv_result = bvm->select_with_int(bv_result, high_sel_number, low_sel_number);

	      } //"select" ends here


	    else if(outlist_size==0) // constants
	      {
		unsigned long long int dec_value;

		if(dag_str == "true" || dag_str == "'1'")// literal constant 1
		  {
		    dag_str = "1";
		    dec_value = atoi(dag_str.c_str()); 
		  }
		else if(dag_str == "false" || dag_str == "'0'")// literal constant 0
		  {
		    dag_str = "0";
		    dec_value = atoi(dag_str.c_str()); 
		  }
		else 
		  {
		    if(dag_str[0]=='x')// hex constant
		      {
			dag_str.erase(dag_str.begin(), dag_str.begin()+2);
			dag_str.erase(dag_str.end()-1, dag_str.end());
			dec_value = hexStringToULINT(dag_str);
		      }
		    else// decimal constant
		      {
			dec_value  = strtoull(dag_str.c_str(), NULL, 0);
		      }
		  }

		cout<<"\ndec_value = "<<dec_value<<endl;

		string bin_value = ULINTToBinaryString(dec_value); // binary value with minimum required width
		bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	 

		cout<<"\nbin_value = "<<bin_value<<endl;

		if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
		  {
		    bv_result = bvm->BVExtend(result, width, 0);
		  }
		else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		  {
		    bv_result = bvm->select_with_int(result, width-1, 0);
		  }
		else
		  {
		    bv_result = result;
		  }
		  
	      }// "constants" ends here
	    else
	      {
		cout<<"\nUnanticipated operator "<<dag_str<<" inside function CHelper :: lmdeEvaluatorModified. Note that presently the function CHelper :: lmdeEvaluatorModified is restricted to pure LMDEs\n";
		exit(1);
	      }
	}// if an operator/constant node ends here


      cout<<"\nbv_result from "<<dag_str<<" for lmdeEvaluatorModified = ";
      bvm->printBVatom(bv_result, 0);
  
  return bv_result;
		    
}// function ends here



string CHelper::ULINTToBinaryString(unsigned long long int i)
{
    if(i==0)
        return "0";
    string bin_str="";
    unsigned long long int j=i;
    while(j!=0)
    {
        if(j%2 == 0)
            bin_str = bin_str + "0";
        else
            bin_str = bin_str + "1";
        j = j / 2;
    }
    string str="";
    //Reversing the binary string to bring it to final format
    for(int k=bin_str.size()-1;k>=0;k--)
        str += bin_str[k];
    return str;
}



unsigned long long int CHelper::getCoefOfChar(char c)
{
switch(c)
	{
	case '0' : return 0;
	case '1' : return 1;
	case '2' : return 2;
	case '3' : return 3;
	case '4' : return 4;
	case '5' : return 5;
	case '6' : return 6;
	case '7' : return 7;	
	case '8' : return 8;	
	case '9' : return 9;
	case 'a' : return 10;
	case 'b' : return 11;
	case 'c' : return 12;
	case 'd' : return 13;	
	case 'e' : return 14;
	case 'f' : return 15;
	}
}

unsigned long long int CHelper::findPower16(int power)
{
unsigned long long int number=1;
int i=power;
while(i>0)
	{
	number=number*16;
	i--;
	}
return number;
}

unsigned long long int CHelper::hexStringToULINT(string x)
{
char c;
unsigned long long int coef, power, power2, length;
unsigned long long int number=0, i=0;
char *y=strdup(x.c_str()); 
length=x.length();

while(y[i]!='\0')
	{
	c=y[i];
	x.erase(x.begin(), x.begin());

	if(c!= '0')
	  {
	    coef=getCoefOfChar(c);
	    power=length-i-1;
	    power2=findPower16(power);
	    number+=power2*coef;
	  }
	i++;
	}

return number;
}



// findProblematicSignalsAtPreviousStateForSignal can be used to evaluate 

// dags which are in the following functional form

// dag ---> ite(dag1, dag, dag) | bit-vector

// dag1 ---> dag1 /\ dag1 | dag1 \/ dag1 | !dag1 | lmde

// Thus the result is always a bit-vector (returned as a string)

// The function also returns a subset of the support of dag

// which is sufficient to evaluate dag to the specific value

string CHelper :: findProblematicSignalsAtPreviousStateForSignal(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal)
{
  // make node point to the RHS of the function

  if(node->Get_DAGnode_value() == "="  || node->Get_DAGnode_value() == "equivalent")
    {
      list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();
      iout++;
      node = *iout;
    }
  else
    {
      cout<<"\nArgument dag is "<<node->Get_DAGnode_value()<<" i.e. not a function in CHelper :: findProblematicSignalsAtPreviousStateForSignal\n";
      exit(1);
    }
  


  map<CDAGNode*, pair<bvatom, set<string> > > DPTable;
  DPTable.clear();
  bvatom ret_value = findProblematicSignalsAtPreviousStateForSignal_Internal(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal, DPTable);

  // printing the result for debugging
  cout<<"\nret_value = ";
  bvm->printBVatom(ret_value, 0);

  showSet(ProblematicSignalsAtPreviousStateForSignal, "ProblematicSignalsAtPreviousStateForSignal");

  string hex_str = bvm->convertBVToHexString(ret_value);
  cout<<"\nhex_str = "<<hex_str<<endl;
  unsigned long long int dec_value = hexStringToULINT(hex_str); // decimal value of the hex string
  cout<<"\ndec_value = "<<dec_value<<endl;
  string ret_value_str = ULINTToBinaryString(dec_value); // binary value with minimum required width
  cout<<"\nret_value_str = "<<ret_value_str<<endl;
  return ret_value_str;

}


bvatom CHelper :: findProblematicSignalsAtPreviousStateForSignal_Internal(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal, map<CDAGNode*, pair<bvatom, set<string> > > &DPTable)
{

  map<CDAGNode*, pair<bvatom, set<string> > > ::iterator dpit = DPTable.find(node);
 
  if(dpit!=DPTable.end()) // node already encountered
    {
      //cout<<"\nDPTable Hit\n";
      ProblematicSignalsAtPreviousStateForSignal = (dpit->second).second;
      return (dpit->second).first;
    }

  else { // encountered first time

    bvatom ret_value;
    string dag_str;
    unsigned int outlist_size;

    dag_str = node->Get_DAGnode_value();
    outlist_size = node->Get_DAGnode_outlist_size();

    ProblematicSignalsAtPreviousStateForSignal.clear();


    cout<<"\n"<<dag_str<<" node encountered in findProblematicSignalsAtPreviousStateForSignal_Internal\n";

    if(dag_str == "=" || dag_str == "is_not_equal" || dag_str == "not_equal_to" || dag_str == "equivalent") // LME/LMD
      {
	ret_value = call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal);
      }
    else if(outlist_size != 0)// Internal node
      {
	bvatom true_value = bvm->getZeroedBVatom(1);
	true_value.bv_array[0] = (bv_block_t) 1;

	bvatom false_value = bvm->getZeroedBVatom(1);
	false_value.bv_array[0] = (bv_block_t) 0;

      
	if(dag_str == "and" && outlist_size  == 2) // binary and
	  {

	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild2;

	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);
	  
	    iout++;

	    bvatom child2 = findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild2, DPTable);

	    bool child1_is_false = bvm->checkBVEquality(child1, false_value);
	    bool child2_is_false = bvm->checkBVEquality(child2, false_value);

	    if(child1_is_false && child2_is_false)// child1 is false and child2 is false
	      {
		findSmaller(ProblematicSignalsAtPreviousStateForSignalChild1, ProblematicSignalsAtPreviousStateForSignalChild2, ProblematicSignalsAtPreviousStateForSignal); 
		ret_value = false_value;
	      }
	    else if(child1_is_false)// only child1 is false
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild1;
		ret_value = false_value;
	      }
	    else if(child2_is_false)// only child2 is false
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild2;
		ret_value = false_value;
	      }
	    else // child1 is true and child2 is true
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild2.begin(), ProblematicSignalsAtPreviousStateForSignalChild2.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		ret_value = true_value;
	      }
	  }//binary and
      
	else if(dag_str == "or" && outlist_size  == 2) // binary or
	  {
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild2;

	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);
	  
	    iout++;

	    bvatom child2 = findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild2, DPTable);

	    bool child1_is_true = bvm->checkBVEquality(child1, true_value);
	    bool child2_is_true = bvm->checkBVEquality(child2, true_value);

	    if(child1_is_true && child2_is_true)// child1 is true and child2 is true
	      {
		findSmaller(ProblematicSignalsAtPreviousStateForSignalChild1, ProblematicSignalsAtPreviousStateForSignalChild2, ProblematicSignalsAtPreviousStateForSignal); 
		ret_value = true_value;
	      }
	    else if(child1_is_true)// only child1 is true
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild1;
		ret_value = true_value;
	      }
	    else if(child2_is_true)// only child2 is true
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild2;
		ret_value = true_value;
	      }
	    else // child1 is false and child2 is false
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild2.begin(), ProblematicSignalsAtPreviousStateForSignalChild2.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		ret_value = false_value;
	      }

	  }// binary or

	else if(dag_str == "ite")
	  {

	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild2;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild3;


	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);
	  
	    iout++;

	    bvatom child2 = findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild2, DPTable); 

	    iout++;

	    bvatom child3 = findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild3, DPTable); 

	    bool child1_is_true = bvm->checkBVEquality(child1, true_value);

	    if(child1_is_true)// child1 is true
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild2.begin(), ProblematicSignalsAtPreviousStateForSignalChild2.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		
		ret_value = child2;
	      }
	    else // child1 is false
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild3.begin(), ProblematicSignalsAtPreviousStateForSignalChild3.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		
		ret_value = child3;
	      }
	  
	  }// ite


	else if(dag_str == "not")
	  {
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;

	    list<CDAGNode*>::iterator iout = node->Get_DAGnode_outlist();

	    bvatom child1 =  findProblematicSignalsAtPreviousStateForSignal_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);

	    ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild1;
	  
	    ret_value = bvm->bitwise_negate(child1);
	    
	  }// not
	else if(dag_str == "*" || dag_str == "+")
	  {
	    ret_value = call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal);
	  }// * and +
	else
	  {
	    cout<<"\nUnknown operator "<<dag_str<<" in function CHelper :: findProblematicSignalsAtPreviousStateForSignal_Internal\n";
	    exit(1);
	  }
       

      }// internal node

    else // constants and variables
      {
	if(node->Get_DAG_node_type()==false)  //i.e. if variable
	  {
	    ret_value = call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal);
	    
	  } //variable node ends here

	else // constants
	  {
	    if(dag_str == "true" || dag_str == "'1'")
	      {
		ret_value = bvm->getZeroedBVatom(1);
		ret_value.bv_array[0] = (bv_block_t) 1;
	      }
	    else if(dag_str == "false" || dag_str == "'0'")
	      {
		ret_value = bvm->getZeroedBVatom(1);
		ret_value.bv_array[0] = (bv_block_t) 0;
	      }
	    else
	      {
		if(dag_str[0]=='x')// hex constant
		  {
		    dag_str.erase(dag_str.begin(), dag_str.begin()+2);
		    dag_str.erase(dag_str.end()-1, dag_str.end());
		    int width = dag_str.length()*4;

		    // trim leading zeros from dag_str: let's see what happens without this
		    unsigned long long int dec_value = hexStringToULINT(dag_str); // decimal value of the constant obtained from the dag node
		    string bin_value = ULINTToBinaryString(dec_value); // binary value with minimum required width
		    bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

		    if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
		      {
			ret_value = bvm->BVExtend(result, width, 0);
		      }
		    else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		      {
			ret_value = bvm->select_with_int(result, width-1, 0);
		      }
		    else
		      {
			ret_value = result;
		      }
		  }// hex constant
		else
		  {
		    cout<<"\nUnknown constant "<<dag_str<<" in function CHelper :: findProblematicSignalsAtPreviousStateForSignal_Internal\n";
		    exit(1);
		  }
		
	      }// else
	  }//constants
      }//constants and variables

    DPTable.insert(make_pair(node, make_pair(ret_value, ProblematicSignalsAtPreviousStateForSignal)));

    cout<<"\nret_value from "<<dag_str<<" for findProblematicSignalsAtPreviousStateForSignal_Internal = ";
    bvm->printBVatom(ret_value, 0);
    cout<<"\nProblematicSignalsAtPreviousStateForSignal from "<<dag_str<<" for findProblematicSignalsAtPreviousStateForSignal_Internal = \n";
    showSet(ProblematicSignalsAtPreviousStateForSignal, "ProblematicSignalsAtPreviousStateForSignal");

    return ret_value;
  }// DPTable miss

}// Function ends 




bvatom CHelper :: call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(CDAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal)
{
  //cout<<"\ncall_lmdeEvaluatorReturningBVWithMinimumSupportFinding called\n";
  
  if(node == NULL)
    {
     cout<<"Function CHelper :: call_lmdeEvaluatorReturningBVWithMinimumSupportFinding called with NULL node\n";
     exit(1);
    } 
    // find width
  string variable = "";
  variable = node->getSingleVariable();
  if(variable=="")
    {
      cout<<"\nLME/LMD "<<node->getDAGStr()<<" encountered with no variable names in function CHelper :: call_lmdeEvaluatorReturningBVWithMinimumSupportFinding\n";
      exit(1);
    }
  
  map<string, int>::iterator MyWidthTable_it = MyWidthTable.find(variable);
  if(MyWidthTable_it == MyWidthTable.end())
	    {
	      cout<<"\nNo entry in MyWidthTable for variable "<<variable<<" in function CHelper :: call_lmdeEvaluatorReturningBVWithMinimumSupportFinding\n";
	      exit(1);
	    }
  int width = MyWidthTable_it->second;

  bool boolean_result;

  bvatom bv_result = lmdeEvaluatorModified(node, width, Variable_Value_Map, boolean_result, MyWidthTable);

  // ProblematicSignalsAtPreviousStateForSignal is presently just the support of node

  node->getDAGNames(ProblematicSignalsAtPreviousStateForSignal);  

  //cout<<"\ncall_lmdeEvaluatorReturningBVWithMinimumSupportFinding finished\n";

  return bv_result; 
}


void CHelper :: findSmaller(set<string> &ReducedModelChild1, set<string> &ReducedModelChild2, set<string> &ReducedModel)
{
  if(ReducedModelChild1.size() < ReducedModelChild2.size()) // ReducedModelChild1 is smaller
	{
	ReducedModel = ReducedModelChild1;
	}
  else
	{
	ReducedModel = ReducedModelChild2;
	}
}


  

// ADDED ON 7-2-2011 ends here


// ADDED ON 24-2-2011 starts here


void CHelper :: printDAGInSMTLibFormatInGivenFile(CDAGNode *dag, map<string, int> &WidthTable, string SMTFileName)
{
    
  FILE *Myfile = fopen(SMTFileName.c_str(), "w");

  if(Myfile == NULL) 
    {cout<<"\nCannot create "<<SMTFileName<<"\n"; exit(1);}

  fprintf(Myfile,"(benchmark test\n");

  set<string> MySupport;

  dag->getDAGNames(MySupport);

  writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggleForGivenSetOfVariables(Myfile, WidthTable, MySupport);

  string original_str = callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(dag, WidthTable);

  fprintf(Myfile,"\n\n:formula %s\n\n)\n\n\n", original_str.c_str());

  fclose(Myfile);
    
}


void CHelper::writeBVDeclarationsFromWidthTableInSMTLibFormatWithoutDeltaToggleForGivenSetOfVariables(FILE *smt_file, map<string, int> WidthTable, set<string> &MySet)
{
   for(set<string>::iterator it2=MySet.begin(); it2!=MySet.end(); it2++)
        {
	  map<string, int>::iterator it = WidthTable.find(*it2);

	  if(it == WidthTable.end())
	    {
	      cout<<"\nNo entry for "<<*it2<<" in WidthTable\n";
	      exit(1);
	    }

	  string id_delta = it->first;
	  fprintf(smt_file,":extrafuns ((%s",id_delta.c_str());
	  fprintf(smt_file," BitVec[%d]))\n",it->second);
	}
 
}



int CHelper::getWidthOfLMDEWithSelects(CDAGNode *node, map<string, int> MyWidthTable)
{
  map<CDAGNode*, CDAGNode*> LinearizationTable;
  int count=0;
  set<string> VarSetToElim;
  set<string> VarSetToElimTemp;

  CDAGNode* LinearizedClone = getLinearizedClone(node, LinearizationTable, count, MyWidthTable, VarSetToElim, VarSetToElimTemp);

  string variable = "";
  variable = LinearizedClone->getSingleVariable();
  if(variable=="")
    {
      cout<<"\nLME/LMD "<<LinearizedClone->getDAGStr()<<" encountered with no variable names in function CHelper::getWidthOfLMDEWithSelects\n";
      exit(1);
    }
  
  map<string, int>::iterator MyWidthTable_it = MyWidthTable.find(variable);
  if(MyWidthTable_it == MyWidthTable.end())
    {
      cout<<"\nNo entry in MyWidthTable for variable "<<variable<<" in function CHelper::getWidthOfLMDEWithSelects\n";
      exit(1);
    }
  int width = MyWidthTable_it->second;

  return width;
}

// return 2 ^ number with larger range
ULINT CHelper::findPowerWithLargerRange(ULINT number)
{
  ULINT count=0;
  ULINT value=1;
  while(count<number) {count=count+1; value=value*2;}
  return value;
}


// ADDED ON 24-2-2011 ends here



// ADDED ON 28-3-2011 FOR INTEGRATION WITH THE SSIMULATOR

void CHelper :: checkForPropSkeltonModified(CDAGNode* parent)
    {
                
        if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();
	    string MyName = parent->Get_DAGnode_value();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 = *iout;
		    string child_1_name = child_1->Get_DAGnode_value();

		    if(MyName == "not")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of not node in the DAG\n";
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }
		    
		    // Do this for the child also

		    checkForPropSkeltonModified(child_1);
		    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;

		    string child_1_name,child_2_name; 
                    
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;

		    child_1_name = child_1->Get_DAGnode_value();
		    child_2_name = child_2->Get_DAGnode_value();

		    if(MyName == "and" || MyName == "or")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(!Logical(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }

		    else if(MyName == "equivalent" || MyName == "+" || MyName == "*" || MyName == "is_not_equal" || MyName == "-" || MyName == "<=" || MyName == ">=" || MyName == "<" || MyName == ">")
		      {
			if(LogicalOperator(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    cout<<"\nNode is "<<parent->getDAGStr()<<endl;
			    exit(1);
			  }

			if(LogicalOperator(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    cout<<"\nNode is "<<parent->getDAGStr()<<endl;
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }

		    checkForPropSkeltonModified(child_1);
		    checkForPropSkeltonModified(child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

		    string child_1_name,child_2_name, child_3_name; 
                    
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;
		    iout++;
                    child_3 = *iout;

		    child_1_name = child_1->Get_DAGnode_value();
		    child_2_name = child_2->Get_DAGnode_value();
		    child_3_name = child_3->Get_DAGnode_value();

		    if(MyName == "ite")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(!Logical(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
			
			if(!Logical(child_3_name))
			 {
			    cout<<"\n"<<child_3_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }

		    else if(MyName == "select")
		      {
			if(LogicalOperator(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(LogicalOperator(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
			
			if(LogicalOperator(child_3_name))
			 {
			    cout<<"\n"<<child_3_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }

		    checkForPropSkeltonModified(child_1);
		    checkForPropSkeltonModified(child_2);
		    checkForPropSkeltonModified(child_3);
                    
                    break;
                }
	    case 0:
	      {
		// Do nothing for constants
		break;
	      }
	    default :
	      {
		cout<<"\nIllegal control inside CHelper::checkForPropSkeltonModified\n";
		exit(1);
	       }
		
            }//switch
        }//if
        else
        {
	  // Do nothing for variables
        }        
        
    }    


// This function extracts the APs from a DAG with the restriction that the APs from the condition part of the ites should occure before the APs from the then and the else parts
void CHelper::getAPsForDAGInPropSkelton(CDAGNode* parent, list<CDAGNode*> &APList)
    {
       	unsigned int outlist_size = parent->Get_DAGnode_outlist_size();
	                   
        if(outlist_size>0)  //i.e. if an operator node
        {
	  list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();

            switch(outlist_size)
            {
                case 1:
                {
		  CDAGNode* child_1 = *iout;
		  getAPsForDAGInPropSkelton(child_1, APList);
		  break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;

                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;

		    string MyName = parent->Get_DAGnode_value();

		    if(MyName == "equivalent" || MyName == "is_not_equal" || MyName == "=" || MyName == "not_equal_to")
		      {
			//cout<<"\n"<<parent->getDAGStr()<<" inserted into APList\n";

			if(!searchAPList(APList, parent)) // insert parent into APList if it is not present already
			  {
			    APList.push_back(parent);
			  }
		      }

		    else 
		      {
			getAPsForDAGInPropSkelton(child_1, APList);
			getAPsForDAGInPropSkelton(child_2, APList);
		      }

                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

		   
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;
		    iout++;
                    child_3 = *iout;

		    getAPsForDAGInPropSkelton(child_1, APList);
		    getAPsForDAGInPropSkelton(child_2, APList); 
		    getAPsForDAGInPropSkelton(child_3, APList);
		    
		    break;
                }
	    default:
		{
		 string MyName = parent->Get_DAGnode_value();
		 if(MyName == "and" || MyName == "or") // and/or with more than two children encountered
			{
			int i=0;
			for(;i<outlist_size;i++)
				{
				getAPsForDAGInPropSkelton(*iout, APList);
				iout++;
				}
			} 

		else
		  {
		  cout<<"\nUnknown operator "<<parent->Get_DAGnode_value()<<" reached in function CHelper::getAPsForDAGInPropSkelton\n";
		  exit(1);
		  }
		}//default
	    }//switch
	}
        else // variables or constants
        {
	  //cout<<"\nVaraible/constant node with data "<<parent->Get_node_name()<<" reached in function getAPsForDAGInPropSkelton in file Eliminator.cpp without going through an equality node\n";
	  //exit(1);
        }        
        
    }    

// Search if a dag node is already present in the APList
bool CHelper::searchAPList(list<CDAGNode*> &APList, CDAGNode *target)
{
  list<CDAGNode*>::iterator it=APList.begin();
  for(;it!=APList.end(); it++)
    {
      if(*it==target) return true;
    }
  return false;
}

void CHelper::copyList(list<CDAGNode*> &DestList, list<CDAGNode*> &SourceList)
{
for(list<CDAGNode*>::iterator it=SourceList.begin(); it != SourceList.end(); it++)
	{
	CDAGNode *AP = *it;
	DestList.push_back(AP);
	}
}

void CHelper::showAPList(list<CDAGNode*> &APList)
{
  list<CDAGNode*>::iterator it=APList.begin();
  for(;it!=APList.end(); it++)
    {
      cout<<endl<<*it<<"\t"<<(*it)->getDAGStr()<<endl;
    }
}



void CHelper::modifyAPListBasedOnVarsToElim(list<CDAGNode*> &APList, set<string> &VarsToElim)
{
list<CDAGNode*> OriginalAPList;

copyList(OriginalAPList, APList); // copy APList to OriginalAPList

APList.clear();

set<CDAGNode*> AddedAPs;

for(list<CDAGNode*>::iterator it=OriginalAPList.begin(); it != OriginalAPList.end(); it++)
	{
	CDAGNode *AP = *it;
	set<string> Support;
	AP->getDAGNames(Support);
	set<string> CommonVars;
	set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(CommonVars, CommonVars.begin()));
	if(!CommonVars.empty()) // AP has some variable to be eliminated
		{
		APList.push_back(AP);
		AddedAPs.insert(AP);
		}
	}
	
for(list<CDAGNode*>::iterator it=OriginalAPList.begin(); it != OriginalAPList.end(); it++)
	{
	CDAGNode *AP = *it;
	if(AddedAPs.find(AP)==AddedAPs.end()) // AP is not inserted already
		{
		APList.push_back(AP);
		AddedAPs.insert(AP);
		}
	}
}


CDAGNode* CHelper :: getLinearizedCloneWithGlobalCount(CDAGNode* parent, map<CDAGNode*, CDAGNode*> &LinearizationTable, map<string, int> &WidthTable, set<string> &VarsToElimOriginal, set<string> &VarsToElimRenamed)
    {
        CDAGNode* clone;

	if(parent->Get_DAGnode_value() == "select") // select node encountered
	  {
	    map<CDAGNode*, CDAGNode*>::iterator it = LinearizationTable.find(parent);
	    
	    if(it != LinearizationTable.end()) // parent is already added to the Table. get the address of the new node and return it
	      {
		clone = it->second;
	      }
	    else // parent is newly encountered. Create a new node and return it's address
	      {
		string new_name = "autoname";
		GlobalAutonameCount++;
		int new_delta_time = GlobalAutonameCount;
		char char_ptr[10] = " ";
		sprintf(char_ptr , "%d", new_delta_time);
		string dt(char_ptr);
		string new_id_delta = new_name;
		new_id_delta +="_";
		new_id_delta += dt; // new_id_delta created
		clone = createDAG(new_name, new_delta_time);
		LinearizationTable.insert(make_pair(parent, clone)); // inserted into LinearizationTable

		
		list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
		CDAGNode* child_1 = *iout;
		iout++;
		CDAGNode* child_2 = *iout;
		iout++;
		CDAGNode* child_3 = *iout;

		// Updating VarsToElimRenamed

		string id_delta = child_1->Get_DAGnode_data_as_id_delta();
		if(VarsToElimOriginal.find(id_delta) != VarsToElimOriginal.end()) // variable is to be eliminated
		  {		    
		    VarsToElimRenamed.insert(new_id_delta);
		    //VarsToElimOriginal.erase(id_delta); // Newly added on 8th Jan 2010
		  }		    


		// For getting the size
		string lower = child_2->Get_DAGnode_value();
		string upper = child_3->Get_DAGnode_value();
		int l = atoi(lower.c_str());
		int u = atoi(upper.c_str());
		int size = u - l + 1;

		if(size<=0) {cout<<"\nSize of newly generated bit-vector non-positive inside function getLinearizedClone\n"; exit(1);}

		WidthTable.insert(make_pair(new_id_delta, size));
	      }
	  }
                   
        else if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = getLinearizedCloneWithGlobalCount(*iout, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = getLinearizedCloneWithGlobalCount(*iout, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    iout++;
                    child_2 = getLinearizedCloneWithGlobalCount(*iout, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = getLinearizedCloneWithGlobalCount(*iout, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    iout++;
                    child_2 = getLinearizedCloneWithGlobalCount(*iout, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);
		    iout++;
                    child_3 = getLinearizedCloneWithGlobalCount(*iout, LinearizationTable, WidthTable, VarsToElimOriginal, VarsToElimRenamed);

		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
	    }
	}
        
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               parent->Get_DAGnode_delta_time_as_int());
        } 
    
    
        return clone;
    }        


CDAGNode* CHelper :: getSubtractionReplacedClone(CDAGNode* parent, map<CDAGNode*, CDAGNode*> &SubtractionTable, map<string, int> &WidthTable)
{
  map<CDAGNode*, CDAGNode*> DPTable;
  CDAGNode *Clone;

  Clone = getSubtractionReplacedCloneInternal(parent, SubtractionTable, WidthTable, DPTable);
  return Clone;
  
}




CDAGNode* CHelper :: getSubtractionReplacedCloneInternal(CDAGNode* parent, map<CDAGNode*, CDAGNode*> &SubtractionTable, map<string, int> &WidthTable, map<CDAGNode*, CDAGNode*> &DPTable)
    {

      map<CDAGNode*, CDAGNode*>::iterator DPTable_it = DPTable.find(parent);

      if(DPTable_it != DPTable.end()) // hit in DPTable
	{
	  return DPTable_it->second;
	}
      else // miss in DPTable
	{
        CDAGNode* clone;

	if(parent->Get_DAGnode_value() == "-") // subtraction node encountered
	  {
	    map<CDAGNode*, CDAGNode*>::iterator it = SubtractionTable.find(parent);
	    
	    if(it != SubtractionTable.end()) // parent is already added to the Table. get the address of the new node and return it
	      {
		clone = it->second;
	      }
	    else // parent is newly encountered. Create a new node and return it's address
	      {
		clone = recreateSubtractionDAG(parent, WidthTable);
		SubtractionTable.insert(make_pair(parent, clone)); // inserted into SubtractionTable
	      }
	  }
                   
        else if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
            list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
            unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

            switch(outlist_size)
            {
                case 1:
                {
                    CDAGNode* child_1 ;

		    child_1 = getSubtractionReplacedCloneInternal(*iout, SubtractionTable, WidthTable, DPTable);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1);
                    break;
                }
                case 2:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    
                    child_1 = getSubtractionReplacedCloneInternal(*iout, SubtractionTable, WidthTable, DPTable);
		    iout++;
                    child_2 = getSubtractionReplacedCloneInternal(*iout, SubtractionTable, WidthTable, DPTable);
		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
                    break;
                }
                case 3:
                {
                    CDAGNode* child_1 ;
                    CDAGNode* child_2 ;
                    CDAGNode* child_3 ;

                    child_1 = getSubtractionReplacedCloneInternal(*iout, SubtractionTable, WidthTable, DPTable);
		    iout++;
                    child_2 = getSubtractionReplacedCloneInternal(*iout, SubtractionTable, WidthTable, DPTable);
		    iout++;
                    child_3 = getSubtractionReplacedCloneInternal(*iout, SubtractionTable, WidthTable, DPTable);

		    clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
                    break;
                }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
	    }
	}
        
        else
        {
            clone = createDAG( parent->Get_DAGnode_value(),
                               parent->Get_DAGnode_delta_time_as_int());
        } 
    
	
	DPTable.insert(make_pair(parent, clone));
    
        return clone;
	}// miss in DPTable
    }        




CDAGNode* CHelper :: getOriginalClone(CDAGNode* parent, map<CDAGNode*, CDAGNode*> &LinearizationTable)
{
  map<CDAGNode*, CDAGNode*> DPTable;
  CDAGNode *Clone;

  Clone = getOriginalCloneInternal(parent, LinearizationTable, DPTable);
  return Clone;
  
}



CDAGNode* CHelper :: getOriginalCloneInternal(CDAGNode* parent, map<CDAGNode*, CDAGNode*> &LinearizationTable, map<CDAGNode*, CDAGNode*> &DPTable)
{
  map<CDAGNode*, CDAGNode*>::iterator DPTable_it = DPTable.find(parent);

  if(DPTable_it != DPTable.end()) // hit in DPTable
    {
      return DPTable_it->second;
    }
  else // miss in DPTable
    {
      CDAGNode* clone;
                   
      if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
	  list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
	  unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

	  switch(outlist_size)
            {
	    case 1:
	      {
		CDAGNode* child_1 ;

		child_1 = getOriginalCloneInternal(*iout, LinearizationTable, DPTable);
		clone = createDAG(parent->Get_DAGnode_value(), child_1);
		break;
	      }
	    case 2:
	      {
		CDAGNode* child_1 ;
		CDAGNode* child_2 ;
                    
		child_1 = getOriginalCloneInternal(*iout, LinearizationTable, DPTable);
		iout++;
		child_2 = getOriginalCloneInternal(*iout, LinearizationTable, DPTable);
		clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
		break;
	      }
	    case 3:
	      {
		CDAGNode* child_1 ;
		CDAGNode* child_2 ;
		CDAGNode* child_3 ;

		child_1 = getOriginalCloneInternal(*iout, LinearizationTable, DPTable);
		iout++;
		child_2 = getOriginalCloneInternal(*iout, LinearizationTable, DPTable);
		iout++;
		child_3 = getOriginalCloneInternal(*iout, LinearizationTable, DPTable);
		clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
		break;
	      }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
            }
        }
      else //operand node
        {
	  string id_delta = parent->Get_DAGnode_data_as_id_delta();

	  map<CDAGNode*, CDAGNode*>::iterator ltit = findInLinearizationTable(LinearizationTable, id_delta);

	  if(ltit != LinearizationTable.end()) // Occures in LT
	    {
	      clone = ltit->first; // Note : first is the parent node
	     
	    }
	  
	  else
	    {
	      
	      clone = createDAG( parent->Get_DAGnode_value(), parent->Get_DAGnode_delta_time_as_int());
	    }
        }   


      DPTable.insert(make_pair(parent, clone));
     
      return clone;
    }// miss in DPTable
}        

map<CDAGNode*, CDAGNode*>::iterator CHelper :: findInLinearizationTable(map<CDAGNode*, CDAGNode*> &LinearizationTable, string id_delta)
{
  map<CDAGNode*, CDAGNode*>::iterator ltit = LinearizationTable.begin();

  for(; ltit != LinearizationTable.end(); ltit++)
    {
      string existing_id_delta = (ltit->second)->Get_DAGnode_data_as_id_delta();
      if(existing_id_delta == id_delta)
	{
	  return ltit;
	}
    }
  return ltit;
}
      

CDAGNode* CHelper :: cloneWithRename(CDAGNode* parent, map<string, string> &RenameMap)
{
  map<CDAGNode*, CDAGNode*> DPTable;
  CDAGNode *Clone;

  Clone = cloneWithRenameInternal(parent, RenameMap, DPTable);
  return Clone;
  
}


CDAGNode* CHelper :: cloneWithRenameInternal(CDAGNode* parent, map<string, string> &RenameMap, map<CDAGNode*, CDAGNode*> &DPTable)
{
  map<CDAGNode*, CDAGNode*>::iterator DPTable_it = DPTable.find(parent);

  if(DPTable_it != DPTable.end()) // hit in DPTable
    {
      return DPTable_it->second;
    }
  else // miss in DPTable
    {
      CDAGNode* clone;
                   
      if(parent->Get_DAG_node_type()==true)  //i.e. if an operator node
        {
	  list<CDAGNode*>::iterator iout = parent->Get_DAGnode_outlist();
	  unsigned int outlist_size = parent->Get_DAGnode_outlist_size();

	  switch(outlist_size)
            {
	    case 1:
	      {
		CDAGNode* child_1 ;

		child_1 = cloneWithRenameInternal(*iout, RenameMap, DPTable);
		clone = createDAG(parent->Get_DAGnode_value(), child_1);
		break;
	      }
	    case 2:
	      {
		CDAGNode* child_1 ;
		CDAGNode* child_2 ;
                    
		child_1 = cloneWithRenameInternal(*iout, RenameMap, DPTable);
		iout++;
		child_2 = cloneWithRenameInternal(*iout, RenameMap, DPTable);
		clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2);
		break;
	      }
	    case 3:
	      {
		CDAGNode* child_1 ;
		CDAGNode* child_2 ;
		CDAGNode* child_3 ;

		child_1 = cloneWithRenameInternal(*iout, RenameMap, DPTable);
		iout++;
		child_2 = cloneWithRenameInternal(*iout, RenameMap, DPTable);
		iout++;
		child_3 = cloneWithRenameInternal(*iout, RenameMap, DPTable);
		clone = createDAG(parent->Get_DAGnode_value(), child_1, child_2, child_3);
		break;
	      }
	    case 0:
	      {
		clone = createDAG( parent->Get_DAGnode_value());
		break;
	      }
            }
        }// operator node
      else //operand node
        {
	  string str_delta_time = parent->Get_DAGnode_delta_time_as_string();
	  string value = parent->Get_DAGnode_value();
	  
	  string id_delta = value + "_" + str_delta_time;

	  if(RenameMap.find(id_delta) != RenameMap.end())

	    {
	      string id_delta_new = RenameMap[id_delta];

	      int index = id_delta_new.find_last_of("_");

	      string name_new = id_delta_new.substr(0, index); 

	      int delta_new = atoi(id_delta_new.substr(index+1).c_str()); 

	      clone = createDAG( name_new, delta_new);
                             
                                                       
	    }
	  else
	    {
	      
	      clone = createDAG( parent->Get_DAGnode_value(), parent->Get_DAGnode_delta_time_as_int());
	    }
        }//operand node

      DPTable.insert(make_pair(parent, clone));
	
      return clone;
    }// miss in DPTable
}    



// set the types of the nodes with dynamic programming


string CHelper :: setTypesWithDP(CDAGNode *tree, string context, map<CDAGNode*, string> &TypeTableOfITENodes)
{
  HashTable<string, string> HTable;
  HashTable<string, string> *PtrHTable=&HTable;

  return setTypesWithDPInternal(tree, context, TypeTableOfITENodes, PtrHTable);

}


  
string CHelper :: setTypesWithDPInternal(CDAGNode *tree, string context, map<CDAGNode*, string> &TypeTableOfITENodes, HashTable<string, string> *PtrHTable)
{
  // check if the node tree is encountered with the same context before

  // create the key

  string key;
  char temp_char[10];
  sprintf(temp_char, "%x", tree);
  string tree_addr(temp_char);
  key = tree_addr + context; 

  HTStatusValue<string> result = PtrHTable->hashtable_search(key);

  if(result.success()) // hash table hit
    {
      return result.GetValue();
    }
  
  else // hash table miss
    {

      string name = tree->Get_DAGnode_value();
      bool node_type = tree->Get_DAG_node_type();
      unsigned int outlist_size = tree->Get_DAGnode_outlist_size();
      list<CDAGNode*>::iterator iout = tree->Get_DAGnode_outlist();
      string delta_string = tree->Get_DAGnode_delta_time_as_string();
      string TreeType;
  

      if(!node_type)
	{
	  TreeType = "bv";
	}
      else
	{

	  //cout<<"\nNAME=\t"<<name<<endl;    //debug code

	  if(name == "select")
	    {
	      iout++;

	      iout++;
	  
	      setTypesWithDPInternal(*iout, "const", TypeTableOfITENodes, PtrHTable);

	      iout = tree->Get_DAGnode_outlist();

	      iout++;

	      setTypesWithDPInternal(*iout, "const", TypeTableOfITENodes, PtrHTable);

	      iout = tree->Get_DAGnode_outlist();

	      setTypesWithDPInternal(*iout, "bv", TypeTableOfITENodes, PtrHTable);

	      TreeType = "bv";

	    }
      
	  else if(name == "is_not_equal" || name == "not_equal_to")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfITENodes, PtrHTable);
		}

	      TreeType = "bool";
	    }

	  // NB: Assumption here is that equivalent applies only between bit-vectors

	  else if(name == "equivalent" || name == "=")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfITENodes, PtrHTable);
		}

	      TreeType = "bool";
	    }

	 else if(name == "<=" || name == ">=" || name == "<" || name == ">")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfITENodes, PtrHTable);
		}

	      TreeType = "bool";
	    }

      
	  else if(name == "*")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfITENodes, PtrHTable);
		}

	      TreeType = "bv";

	    }

	  else if(name == "+")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfITENodes, PtrHTable);
		}

	      TreeType = "bv";

	    }


	  // Newly added on 20.1.2010

	  else if(name == "-")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfITENodes, PtrHTable);
		}

	      TreeType = "bv";

	    }

      
	  else if(name == "and" || (name == "or") || (name == "not"))
	    {
	  
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bool", TypeTableOfITENodes, PtrHTable);
		}

	      TreeType = "bool";
	    }

	  else if(name == "ite")
	    {
	      setTypesWithDPInternal(*iout, "bool", TypeTableOfITENodes, PtrHTable);

	      iout++;

	      string ChildType;

	      for(unsigned int i=1; i<outlist_size; i++,iout++)
		{
		  ChildType = setTypesWithDPInternal(*iout, "bool", TypeTableOfITENodes, PtrHTable);
		}

	      TypeTableOfITENodes.insert(make_pair(tree, ChildType));

	      TreeType = ChildType;

	    }

	  else if(name == "'1'")
	    {
	      TreeType = "bv";
	    }

	  else if(name == "'0'")
	    {
	      TreeType = "bv";
	    }

	  else if(outlist_size == 0) // Other constants

	    {
		
	      if(context == "const")
		{
		  TreeType = "const";
		}
	      else if(context == "bv")
		{
		  TreeType = "bv";	
		}
	      else if(context == "bool")
		{
		  if(name == "1") TreeType = "bool";
		  else if(name == "0") TreeType = "bool";
		  else if(name == "true") TreeType = "bool";
		  else if(name == "false") TreeType = "bool";

		  else // interpreting a Boolean constant other than true/false as a bit-vector
		    {
		      if(name[0] == 'x') // hex
			{
			  TreeType = "bv";
			}
		      else // Decimal
			{
			  TreeType = "bv";
			}
		    }

		}
	      else
		{
		  cout<<"\nUnanticipated constant "<<name<<" inside function setTypes\n";
		  exit(1);
		}
		  
	    }
	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function setTypes\n";
	      exit(1);
	    }
		    
	}// else 

      result = PtrHTable->hashtable_insert(key, TreeType);
   
      if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function CHelper::setTypes\n";
	  exit(1);
	}

      return TreeType;

    }// hash table miss

  

}



CDAGNode* CHelper :: Global_Inlining(CDAGNode* dag)
{
  bool hashing_enabled = true;

  if(hashing_enabled)
    {
      // check if already exists in hash table
      string key;
      
      char temp_char[10];
      sprintf(temp_char , "%x", dag);
      string dag_addr(temp_char);
      key = dag_addr;
      // key is formed

      // check if already exists
      HTStatusValue<CDAGNode*> result = Inlining_HT.hashtable_search(key);
      if(result.success()) // Exists in HT
	{
	  inlining_ht_hash_hits++;
	  return result.GetValue();
	}
      inlining_ht_hash_misses++;

      //cout<<"\nOriginal DAG before inlining = "<<dag->getDAGStr()<<endl;

      CDAGNode *InlinedDAG = Global_Inlining_Internal(dag);
  
      result = Inlining_HT.hashtable_insert(key, InlinedDAG);
   
      if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function CHelper :: Global_Inlining\n";
	  exit(1);
	}     
      return InlinedDAG;
    }// if(hashing_enabled)
  else
    {
      return Global_Inlining_Internal(dag);
    }// else of if(hashing_enabled)
}
  


CDAGNode* CHelper :: Global_Inlining_Internal(CDAGNode* root)
{
    CDAGNode* return_value;
    map < CDAGNode* , set<string> > exists_source_destination;  //store in the format:: address of the exists node and
                                                                //the variables it has to quantify......
    list <CDAGNode*> inlining_order;
    inlining_order.clear();  //initialization......at start.....
    exists_source_destination.clear();  //initialization......at start.....

    find_and_store_ex_quant_from_global_DAG(root , exists_source_destination, inlining_order);
    if(inlining_order.empty())
    {
        return_value = root ;
    }
    else
    {
      return_value = Actual_Global_Inlining(root, exists_source_destination, inlining_order );
    }

    return return_value;
}



// ADDED ON 28-3-2011 FOR INTEGRATION WITH THE SSIMULATOR ENDS HERE



// Added by Ajith John on 9 Oct 2013


// The following two functions help in converting dags like and(and(f1, f2), and(f2, f3))
// to and(f1, and(f2, f3))
CDAGNode* CHelper :: recreateAfterAndFlattening(CDAGNode* root)
{
	assert(root != NULL);

	string name = root->Get_DAGnode_value();
      	bool node_type = root->Get_DAG_node_type();

	
	if(name == "and" && node_type) // and operator encountered; flatten it
	{
		set<CDAGNode*> FlattenedChildren;
		set<CDAGNode*> AlreadyFlattened;
		
		andFlattening(root, FlattenedChildren, AlreadyFlattened);
		assert(FlattenedChildren.size() > 0);

		// we have root \equiv and of FlattenedChildren
		// Create and of FlattenedChildren and return it

		CDAGNode* RecreatedRoot = NULL; 

		for(set<CDAGNode*>::iterator fit = FlattenedChildren.begin(); fit != FlattenedChildren.end(); fit++)
		{
			CDAGNode* Child = *fit;
			assert(Child != NULL);

			if(RecreatedRoot == NULL)
				RecreatedRoot = Child;
			else
				RecreatedRoot = createDAG("and", RecreatedRoot, Child);
		}

		assert(RecreatedRoot != NULL);
		return RecreatedRoot;
    	}
	else
	{
		return root;
	}
}


void CHelper :: andFlattening(CDAGNode* root, set<CDAGNode*> &FlattenedChildren, set<CDAGNode*> &AlreadyFlattened)
{
	assert(root != NULL);

	string name = root->Get_DAGnode_value();
      	bool node_type = root->Get_DAG_node_type();

	
	if(name == "and" && node_type) // and operator encountered
	{
		if(AlreadyFlattened.find(root) == AlreadyFlattened.end()) // not already flattened; flatten it
		{
			// get the children
			list<CDAGNode*>::iterator iout = root->Get_DAGnode_outlist();
            		unsigned int outlist_size = root->Get_DAGnode_outlist_size();

			assert(outlist_size == 2);

			CDAGNode* Child_1 = *iout;
			assert(Child_1 != NULL);
			iout++;
			CDAGNode* Child_2 = *iout;
			assert(Child_2 != NULL);

			AlreadyFlattened.insert(root);
			// flatten the children
			andFlattening(Child_1, FlattenedChildren, AlreadyFlattened);
			andFlattening(Child_2, FlattenedChildren, AlreadyFlattened);
		}
	}
	else
	{
		FlattenedChildren.insert(root);
	}
}


// Added by Ajith John on 9 Oct 2013 ends here
