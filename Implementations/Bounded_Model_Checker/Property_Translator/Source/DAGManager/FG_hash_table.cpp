
#include <set>



#include "FG_hash_table.h"

//function to calculate the hash key (the function takes the address of the DAG node in the string format
//and returns a hash key) {i.e. if the address of the DAG node is 0x123456 then the function input is
//123456 a string}.
//the function has been taken from "http://www.partow.net/programming/hashfunctions/"

unsigned long FG_hash_table :: SDBMHash(const char* addr_str)
{
   string str(addr_str);
   unsigned long hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}

FG_hash_table :: FG_hash_table()    //constructor............
{
   FG_ht.clear();
}

void FG_hash_table :: Insert_FGnode_in_ht(const char* hash_fn_ip , Factor_graph_Node* node_ptr)
{
    unsigned long hash_key;
    map < unsigned long , list <Factor_graph_Node*> > :: iterator FG_ht_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    FG_ht_iterator = FG_ht.find(hash_key);

    if(FG_ht_iterator != FG_ht.end()) //i.e. if the hash key is not present in the hash table
    {
        FG_ht[hash_key].push_back(node_ptr);
    }
    else
    {
        list <Factor_graph_Node*> temp_list;
        temp_list.clear();

        FG_ht.insert (pair<unsigned long,list<Factor_graph_Node*> >(hash_key,temp_list));
        FG_ht[hash_key].push_back(node_ptr);
    }
}

bool FG_hash_table :: Check_FGnode_in_ht(const char* hash_fn_ip)
{
    bool FGnode_in_ht = false;
    unsigned long hash_key;
    map < unsigned long , list <Factor_graph_Node*> > :: iterator FG_ht_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    FG_ht_iterator = FG_ht.find(hash_key);

    if(FG_ht_iterator != FG_ht.end()) //i.e. if the hash key is present in the hash table
    {
        FGnode_in_ht = true;
    }
    return FGnode_in_ht;
}

void FG_hash_table :: Delete_address_of_existing_FGnode_from_ht(  const char* hash_fn_ip          ,
                                                                  Factor_graph_Node* node_ptr_to_be_deleted)
{
    unsigned long hash_key;
    hash_key = SDBMHash(hash_fn_ip);

    if(Check_FGnode_in_ht(hash_fn_ip)==true)
    {
       FG_ht[hash_key].remove(node_ptr_to_be_deleted);
        if(FG_ht[hash_key].empty())
        {
            FG_ht.erase(hash_key);
        }
    }
}

Factor_graph_Node* FG_hash_table :: Get_address_of_existing_FGnode_from_ht(const char* hash_fn_ip, CDAGNode* dag_value)
{
    unsigned long hash_key;
    CDAGNode* dag_node_value;
    list <Factor_graph_Node*> :: iterator list_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    list_iterator = FG_ht[hash_key].begin();
    do
    {
        dag_node_value = (*list_iterator)->Get_FGnode_value();
        if(dag_node_value==dag_value)
        {
            break;
        }
        else
        {
            list_iterator++;
        }
    } while(list_iterator!=FG_ht[hash_key].end());

    return *list_iterator;
}

unsigned long FG_hash_table :: hash_table_size()
{
    return (unsigned long) FG_ht.size();
}

Factor_graph_Node* FG_hash_table :: Get_FG_var_node_with_min_func(set<string>& vars_to_ex_quantify)
{
    Factor_graph_Node* return_value;
    unsigned int min = UINT_MAX;  //max no possible in a 32 bit machine
    map < unsigned long , list <Factor_graph_Node*> >::iterator FG_ht_iter = FG_ht.begin();
    string var_to_remove;

    for(unsigned long i=0;i<hash_table_size();i++, FG_ht_iter++)
    {
        list <Factor_graph_Node*>::iterator l_iter ;
        for(l_iter = FG_ht_iter->second.begin();l_iter != FG_ht_iter->second.end();l_iter++)
        {
            if((*l_iter)->Get_FGnode_type()==false) //i.e. in case of variable nodes...............
            {
                string temp_string = (*l_iter)->Get_FGnode_value()->Get_DAGnode_value() + "_" + 
                                     (*l_iter)->Get_FGnode_value()->Get_DAGnode_delta_time_as_string();
                if(vars_to_ex_quantify.find(temp_string) != vars_to_ex_quantify.end())
                {
                    if((*l_iter)->Get_FG_pointee_size()<min)
                    {
                        min = (*l_iter)->Get_FG_pointee_size();
                        return_value = (*l_iter);
                        var_to_remove = temp_string;
                    }
                }
            }
        }
    }
    vars_to_ex_quantify.erase(var_to_remove);
    return return_value;
}

void FG_hash_table :: Release_FG_memory(void)
{
    map < unsigned long , list <Factor_graph_Node*> >::iterator FG_ht_iter;
    for(FG_ht_iter = FG_ht.begin();FG_ht_iter != FG_ht.end();FG_ht_iter++)
    {
        list <Factor_graph_Node*>::iterator l_iter ;
        for(l_iter = FG_ht_iter->second.begin();l_iter != FG_ht_iter->second.end();l_iter++)
        {
            delete (*l_iter);
        }
    }
    FG_ht.clear();
}

void FG_hash_table :: Get_scope_reduced_funcs(set<CDAGNode*>& good_functions)
{
    map < unsigned long , list <Factor_graph_Node*> >::iterator FG_ht_iter = FG_ht.begin();

    for(unsigned long i=0;i<hash_table_size();i++, FG_ht_iter++)
    {
        list <Factor_graph_Node*>::iterator l_iter ;
        for(l_iter = FG_ht_iter->second.begin();l_iter != FG_ht_iter->second.end();l_iter++)
        {
            if(((*l_iter)->Get_FGnode_type()==true)&&((*l_iter)->Get_FG_pointee_size()>0))
            //i.e. in case of function nodes...............
            {
                good_functions.insert((*l_iter)->Get_FGnode_value());
                (*l_iter)->Clear_plist();
            }
        }
    }
}












