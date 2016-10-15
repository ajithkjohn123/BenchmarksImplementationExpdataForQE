
#include "CHelper.h"

 CHelper :: CHelper()
 {
        //empty constructor.......
 }

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
        char hash_fn_ip[20] = " " ;
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

    unsigned long CHelper ::  HT_size()
    {
        return HT.hash_table_size();
    }

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
            CDAGNode* temp = ac_exQ_it->first;
            if(temp->Get_DAGnode_value()=="select")
            {
                list<CDAGNode*>::iterator iout_select = temp->Get_DAGnode_outlist();
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

        //if((return_value != converted_ite_function)||(converted_ite_function!=root))
        //{
        //    garbage_collector();
        //}
    }
    else
    {
        return_value = root ;
    }
    return return_value;
}

CDAGNode* CHelper :: Global_Inlining(CDAGNode* root)
{
    CDAGNode* return_value;
    map < CDAGNode* , set<string> > exists_source_destination;
    list <CDAGNode*> inlining_order;
    inlining_order.clear();  //initialization......at start.....
    exists_source_destination.clear();  //initialization......at start.....

    find_and_store_ex_quant_from_global_DAG(root , exists_source_destination, inlining_order);

    //cout <<"\nSize of exists_source_destination = "<<  exists_source_destination.size()<<endl;
    if(inlining_order.empty())
    {
        return_value = root ;
    }
    else
    {

      //map < CDAGNode* , set<string> > ::iterator it;
      //for(it = exists_source_destination.begin(); it != exists_source_destination.end(); it++)
      //	{
      //	  cout<<"\nfirst = "<<(it->first)->getDAGStr()<<", second = \n";
      //	  set<string> VarSet = it->second;
	  
      //	  for(set<string>::iterator VarSet_it = VarSet.begin(); VarSet_it != VarSet.end(); VarSet_it++)
      //	    cout<<(*VarSet_it)<<",";

      //	  cout<<endl;
      //	}
      

      
      
      /*cout << "vector size before inlining :: " <<  inlining_order.size() << endl << endl ;

list <CDAGNode*> :: iterator a = inlining_order.begin();
set <string> :: iterator b;

printGraphInDotFormat(*a, "1.dot");
cout << (*a) << endl;
for(b=exists_source_destination[*a].begin();b!=exists_source_destination[*a].end();b++)
    cout << (*b) << " , ";
cout << endl;
a++;

printGraphInDotFormat(*a, "2.dot");
cout << (*a) << endl;
for(b=exists_source_destination[*a].begin();b!=exists_source_destination[*a].end();b++)
    cout << (*b) << " , ";
cout << endl;
a++;

printGraphInDotFormat(*a, "3.dot");
cout << (*a) << endl;
for(b=exists_source_destination[*a].begin();b!=exists_source_destination[*a].end();b++)
    cout << (*b) << " , ";
cout << endl;
a++;

printGraphInDotFormat(*a, "4.dot");
cout << (*a) << endl;
for(b=exists_source_destination[*a].begin();b!=exists_source_destination[*a].end();b++)
    cout << (*b) << " , ";
    cout << endl;*/


      //cout<<"\nActual Inlining starts here\n";
      

        return_value = Actual_Global_Inlining(root, exists_source_destination, inlining_order );
        //garbage_collector();


//cout << "vector size after inlining :: " <<  inlining_order.size() << endl << endl ;
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

    //cout<<"\nCalling Actual_Global_Inlining with root->name as "<<root->Get_DAGnode_value()<<endl;
    
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
                CDAGNode *inlined_node = Inlining(op2, exists_source_destination[root]);

                if(exists_source_destination[root].empty()) //if all the variables are quantified........
                {
                    node_after_inlining = inlined_node ;
                }
                else
                {
                    CDAGNode *new_lhs = Get_quantified_var_list_after_single_inlining(temp_op1, exists_source_destination[root]);
                    if(new_lhs!=NULL)
                    {
                        if(new_lhs==inlined_node)
                        {
                            node_after_inlining = inlined_node ;
                        }
                        else
                        {
                            node_after_inlining = createDAG(root->Get_DAGnode_value(),new_lhs, inlined_node);
                        }
                    }
                    else
                    {
                        node_after_inlining = inlined_node ;
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
            inlining_order.push_back(root);
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

void CHelper :: garbage_collector(void)
{
    HT.gc_for_ht();
}


CDAGNode* CHelper :: equivalentReduction(CDAGNode* root)
{
  cout<<"\n\nBEFORE INLINING = "<<root->getDAGStr()<<endl<<endl;
  
  CDAGNode* Inlined = Global_Inlining(root);

  cout<<"\n\nAFTER INLINING = "<<Inlined->getDAGStr()<<endl<<endl;

  return Inlined;
}

