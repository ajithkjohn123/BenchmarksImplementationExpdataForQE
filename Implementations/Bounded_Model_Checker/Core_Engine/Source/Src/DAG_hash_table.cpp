


#include "DAG_hash_table.h"

//function to calculate the hash key (the function takes the address of the DAG node in the string format
//and returns a hash key) {i.e. if the address of the DAG node is 0x123456 then the function input is
//123456 a string}.
//the function has been taken from "http://www.partow.net/programming/hashfunctions/"

unsigned long DAG_hash_table :: SDBMHash(const char* addr_str)
{
   string str(addr_str);
   unsigned long hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}

DAG_hash_table :: DAG_hash_table()
{
   DAG_ht.clear();
}

void DAG_hash_table :: Insert_DAGnode_in_ht(const char* hash_fn_ip , CDAGNode* node_ptr)
{
    unsigned long hash_key;
    map < unsigned long , list <CDAGNode*> > :: iterator DAG_ht_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    DAG_ht_iterator = DAG_ht.find(hash_key);
    
    if(DAG_ht_iterator != DAG_ht.end()) //i.e. if the hash key is not present in the hash table
    {
        DAG_ht[hash_key].push_back(node_ptr);
    }
    else
    {
        list <CDAGNode*> temp_list;
        temp_list.clear();

        DAG_ht.insert (pair<unsigned long,list<CDAGNode*> >(hash_key,temp_list));
        DAG_ht[hash_key].push_back(node_ptr);
    }
}

bool DAG_hash_table :: Check_DAGnode_in_ht(const char* hash_fn_ip)
{
    bool DAGnode_in_ht = false;
    unsigned long hash_key;
    map < unsigned long , list <CDAGNode*> > :: iterator DAG_ht_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    DAG_ht_iterator = DAG_ht.find(hash_key);

    if(DAG_ht_iterator != DAG_ht.end()) //i.e. if the hash key is present in the hash table
    {
        DAGnode_in_ht = true;
    }
    return DAGnode_in_ht;
}

void DAG_hash_table :: Delete_address_of_existing_DAGnode_from_ht(  const char* hash_fn_ip          ,
                                                                    CDAGNode* node_ptr_to_be_deleted)
{
    unsigned long hash_key;
    hash_key = SDBMHash(hash_fn_ip);
    
    if(Check_DAGnode_in_ht(hash_fn_ip)==true)
    {
//cout << "Deleting node from HT :: " << node_ptr_to_be_deleted->Get_DAGnode_value() << endl << endl ;

        DAG_ht[hash_key].remove(node_ptr_to_be_deleted);
        if(DAG_ht[hash_key].empty())
        {
            DAG_ht.erase(hash_key);

//cout << "HASH TABLE entry after deletion :: " <<DAG_ht.size() << endl << endl ;
        }
    }
}

CDAGNode* DAG_hash_table :: Get_address_of_existing_DAGnode_from_ht(const char* hash_fn_ip, string dag_value)
{
    unsigned long hash_key;
    string dag_node_value;
    list <CDAGNode*> :: iterator list_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    list_iterator = DAG_ht[hash_key].begin();
    do
    {
        dag_node_value = (*list_iterator)->Get_DAGnode_value();

        //cout << dag_node_value << endl ;  //to be decommented only during debugging......

        //assuming that redandancy in the name is least likely,,,,,,
        //athouth this criteria will have to be modified to incorporate inlist and outlist
        //comparison also in case of eratic behaviour from the hash table during testing ........
        if(dag_node_value==dag_value)
        {
            break;
        }
        else
        {
            list_iterator++;
        }
    } while(list_iterator!=DAG_ht[hash_key].end());


    if(list_iterator == DAG_ht[hash_key].end())
      {
	cout<<"\nCollision in DAG Hash table found\n";
	exit(1);
      }
    
    return *list_iterator;
}

unsigned long DAG_hash_table :: hash_table_size()
{
    return (unsigned long) DAG_ht.size();
}

void DAG_hash_table :: parse_DAG_ht_for_floating_nodes(void)
{

    //no need to modify the outlist of the parent DAGnode...................
    //and only modyfying the inlist of the child DAGnode ...................

    list<CDAGNode*> :: iterator icoll ;
    unsigned int coll_size = 0 ;
    map < unsigned long , list <CDAGNode*> > :: iterator DAG_ht_iterator = DAG_ht.begin();
    unsigned long hash_table_size = DAG_ht.size();

    list<CDAGNode*> :: iterator iout ;
    unsigned int outlist_size = 0 ;

    for(unsigned long i=0;i<hash_table_size;i++)
    {
        icoll = DAG_ht[DAG_ht_iterator->first].begin();
        coll_size = (unsigned int)DAG_ht[DAG_ht_iterator->first].size();
        for(unsigned int j=0 ; j<coll_size; j++)
        {
            if(((*icoll)->Get_DAGnode_inlist_size()==0) && ((*icoll)->isroot()==false))
            {
                iout = (*icoll)->Get_DAGnode_outlist();
                outlist_size = (*icoll)->Get_DAGnode_outlist_size();
                for(unsigned int k=0 ; k<outlist_size; k++)
                {
                    (*iout)->Delete_single_entry_from_DAG_inlist((*icoll));
                    iout++;
                }
            }
            icoll++;
        }
        DAG_ht_iterator++;
    }
}

void DAG_hash_table :: del_DAGnode(CDAGNode* DAG_to_be_deleted)
 {
     if(DAG_to_be_deleted!=NULL)
     {         
         list<CDAGNode*> :: iterator ioutl ;
         unsigned int outlist_size = DAG_to_be_deleted->Get_DAGnode_outlist_size();

         char hash_fn_ip[50] = " " ;
         char temp_char1[10], temp_char2[10], temp_char3[10];
         string temp = " ";

         switch(outlist_size)
         {
             case 0:
             {
                 if(DAG_to_be_deleted->Get_DAG_node_type()==true)   //i.e. literal node.......
                 {
                    temp = DAG_to_be_deleted->Get_DAGnode_value();
                    strcpy (hash_fn_ip, temp.c_str());
                    Delete_address_of_existing_DAGnode_from_ht(  hash_fn_ip , DAG_to_be_deleted);
                 }
                 else   //variable node...........
                 {
                    temp = DAG_to_be_deleted->Get_DAGnode_value() + DAG_to_be_deleted->Get_DAGnode_delta_time_as_string();
                    strcpy (hash_fn_ip, temp.c_str());
                    Delete_address_of_existing_DAGnode_from_ht(  hash_fn_ip , DAG_to_be_deleted);
                 }
                 break;
             }
             case 1:
             {
                 ioutl = DAG_to_be_deleted->Get_DAGnode_outlist();
                 sprintf(temp_char1 , "%x", *ioutl);
                 string opd_addr1(temp_char1);
                 temp = DAG_to_be_deleted->Get_DAGnode_value() + opd_addr1;
                 strcpy (hash_fn_ip, temp.c_str());
                 Delete_address_of_existing_DAGnode_from_ht(  hash_fn_ip , DAG_to_be_deleted);
                 break;
             }
             case 2:
             {
                 ioutl = DAG_to_be_deleted->Get_DAGnode_outlist();
                 sprintf(temp_char1 , "%x", *ioutl);
                 ioutl++;
                 sprintf(temp_char2 , "%x", *ioutl);

                 string opd_addr1(temp_char1);
                 string opd_addr2(temp_char2);

                 temp = DAG_to_be_deleted->Get_DAGnode_value() + opd_addr1 + opd_addr2 ;
                 strcpy (hash_fn_ip, temp.c_str());
                 Delete_address_of_existing_DAGnode_from_ht(  hash_fn_ip , DAG_to_be_deleted);

                 break;
             }
             case 3:
             {
                 ioutl = DAG_to_be_deleted->Get_DAGnode_outlist();
                 sprintf(temp_char1 , "%x", *ioutl);
                 ioutl++;
                 sprintf(temp_char2 , "%x", *ioutl);
                 ioutl++;
                 sprintf(temp_char3 , "%x", *ioutl);

                 string opd_addr1(temp_char1);
                 string opd_addr2(temp_char2);
                 string opd_addr3(temp_char3);

                 temp = DAG_to_be_deleted->Get_DAGnode_value() + opd_addr1 + opd_addr2 + opd_addr3;
                 strcpy (hash_fn_ip, temp.c_str());
                 Delete_address_of_existing_DAGnode_from_ht(  hash_fn_ip , DAG_to_be_deleted);
                 break;
             }
         }
//zcout << "node name deleted :: " << DAG_to_be_deleted->Get_DAGnode_value() << endl << endl ;

         delete DAG_to_be_deleted;         
     }
     else
     {
         cout << endl << "WARNING :: Trying to delete NULL pointer" << endl;
     }
 }

void DAG_hash_table :: gc_for_ht(void)
{
    map < unsigned long , list <CDAGNode*> > :: iterator DAG_ht_iterator = DAG_ht.begin();
    
    list<CDAGNode*> :: iterator icoll ;
    unsigned long coll_size = 0 ;

    parse_DAG_ht_for_floating_nodes();
    
    for(DAG_ht_iterator = DAG_ht.begin(); DAG_ht_iterator != DAG_ht.end();DAG_ht_iterator++)
    {
        icoll = DAG_ht_iterator->second.begin();
        coll_size = (unsigned long) DAG_ht_iterator->second.size();
        for(unsigned int j=0 ; j<coll_size; j++, icoll++)
        {
            if(((*icoll)->Get_DAGnode_inlist_size()==0) && ((*icoll)->isroot()==false))
            {
                del_DAGnode((*icoll));
            }
        }
    }
}


