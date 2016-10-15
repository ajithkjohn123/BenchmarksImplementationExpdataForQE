#include "DD_hash_table.h"

//function to calculate the hash key (the function takes the address of the DD node in the string format
//and returns a hash key) {i.e. if the address of the DD node is 0x123456 then the function input is
//123456 a string}.
//the function has been taken from "http://www.partow.net/programming/hashfunctions/"

unsigned long DD_hash_table :: SDBMHash(const char* addr_str)
{
   string str(addr_str);
   unsigned long hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}

DD_hash_table :: DD_hash_table()
{
   DD_ht.clear();
}

void DD_hash_table :: Insert_DDnode_in_ht(const char* hash_fn_ip , DDNode* node_ptr)
{
    unsigned long hash_key;
    map < unsigned long , list <DDNode*> > :: iterator DD_ht_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    DD_ht_iterator = DD_ht.find(hash_key);
    
    if(DD_ht_iterator != DD_ht.end()) //i.e. if the hash key is not present in the hash table
    {
        DD_ht[hash_key].push_back(node_ptr);
    }
    else
    {
        list <DDNode*> temp_list;
        temp_list.clear();

        DD_ht.insert (pair<unsigned long,list<DDNode*> >(hash_key,temp_list));
        DD_ht[hash_key].push_back(node_ptr);
    }
}

bool DD_hash_table :: Check_DDnode_in_ht(const char* hash_fn_ip)
{
    bool DDnode_in_ht = false;
    unsigned long hash_key;
    map < unsigned long , list <DDNode*> > :: iterator DD_ht_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    cout<<"\nSearching for hash key "<<hash_key<<" in the hash table\n";
    

    DD_ht_iterator = DD_ht.find(hash_key);

    if(DD_ht_iterator != DD_ht.end()) //i.e. if the hash key is present in the hash table
    {
        
        DDnode_in_ht = true;
    }
    return DDnode_in_ht;
}

void DD_hash_table :: Delete_address_of_existing_DDnode_from_ht(  const char* hash_fn_ip          ,
                                                                    DDNode* node_ptr_to_be_deleted)
{
    unsigned long hash_key;
    hash_key = SDBMHash(hash_fn_ip);
    
    if(Check_DDnode_in_ht(hash_fn_ip)==true)
    {
//cout << "Deleting node from HT :: " << node_ptr_to_be_deleted->Get_DDnode_value() << endl << endl ;

        DD_ht[hash_key].remove(node_ptr_to_be_deleted);
        if(DD_ht[hash_key].empty())
        {
            DD_ht.erase(hash_key);

//cout << "HASH TABLE entry after deletion :: " <<DD_ht.size() << endl << endl ;
        }
    }
}

DDNode* DD_hash_table :: Get_address_of_existing_DDnode_from_ht(const char* hash_fn_ip, string DD_value)
{
    unsigned long hash_key;
    string DD_node_value;

    char value_here_string[10];
    list <DDNode*> :: iterator list_iterator;

    hash_key = SDBMHash(hash_fn_ip);

    list_iterator = DD_ht[hash_key].begin();
    do
    {
	if((*list_iterator)->getPredicate() == NULL)
		{
		int value_here = (*list_iterator)->getVal();
		sprintf(value_here_string, "%d", value_here);
		DD_node_value = value_here_string;
		}
	else
		{
		CDAGNode* adress_here = (*list_iterator)->getPredicate();
		
		// Temporary 

		cout<<endl<<adress_here->getDAGStr()<<" is existing \n";

		// Temporary  ends here

		sprintf(value_here_string, "%x", adress_here);
		DD_node_value = value_here_string;
		}

        //cout << DD_node_value << endl ;  //to be decommented only during debugging......

        //assuming that redandancy in the name is least likely,,,,,,
        //athouth this criteria will have to be modified to incorporate inlist and outlist
        //comparison also in case of eratic behaviour from the hash table during testing ........
        if(DD_node_value==DD_value)
        {
            break;
        }
        else
        {
            list_iterator++;
        }
    } while(list_iterator!=DD_ht[hash_key].end());

    if(list_iterator == DD_ht[hash_key].end())
      {
	cout<<"\nNon-existing node with DDvalue "<<DD_value<<" was searched in Hash Table in function Get_address_of_existing_DDnode_from_ht\n";
	cout<<"\nhash_input_fn is "<<hash_fn_ip<<endl;
	cout<<"\nHash Table\n";
	showDD_hash_table();
	exit(1);
      }
    
    return *list_iterator;
}

unsigned long DD_hash_table :: hash_table_size()
{
    return (unsigned long) DD_ht.size();
}

void DD_hash_table :: showDD_hash_table(void)
{
    map < unsigned long , list <DDNode*> > :: iterator DD_ht_iterator = DD_ht.begin();
    
    list<DDNode*> :: iterator icoll ;
    unsigned long coll_size = 0 ;

    for(DD_ht_iterator = DD_ht.begin(); DD_ht_iterator != DD_ht.end(); DD_ht_iterator++)
    {
        unsigned long key = DD_ht_iterator->first;
        icoll = DD_ht_iterator->second.begin();
	coll_size = (unsigned long) DD_ht_iterator->second.size();

        cout<<"\nKey = "<<key<<"\t";
        for(unsigned int j=0 ; j<coll_size; j++, icoll++)
        {
           cout<<*icoll<<",";
        }
    }
}



// New function added by Ajith on 15/1/10


DDNode* DD_hash_table :: Get_address_of_DDnode_from_ht(string hash_fn_ip)
{
    char hash_fn_ip_char[50] = " " ;

    // convert hash_fn_ip to char string form
    strcpy (hash_fn_ip_char, hash_fn_ip.c_str());


    // Finding if an entry with the same hash key exists

    unsigned long hash_key = SDBMHash(hash_fn_ip_char);
    
    map < unsigned long , list <DDNode*> > :: iterator DD_ht_iterator;
    
    //cout<<"\nSearching for hash key "<<hash_key<<" in the hash table\n";
    
    DD_ht_iterator = DD_ht.find(hash_key);

    if(DD_ht_iterator == DD_ht.end()) //i.e. if the hash key is absent in the hash table
    {
      //cout<<"\nDD Hash Table Miss\n";
      return NULL;  
    }
   
    
    // If hash key is present in the hash table, go to the list

    list <DDNode*> :: iterator list_iterator = (DD_ht_iterator->second).begin();

    string existing_hash_fn_ip;

    
    do
    {
      if((*list_iterator)->getPredicate() == NULL) // Terminal
		{
		int value_here = (*list_iterator)->getVal();
		char value_here_string[10];
		sprintf(value_here_string, "%d", value_here);
		existing_hash_fn_ip = value_here_string;
		}
	else
		{
		CDAGNode* address_here = (*list_iterator)->getPredicate();
		DDNode* left_here = (*list_iterator)->getLeft();
		DDNode* right_here = (*list_iterator)->getRight();
		
		char address_here_string[10], left_here_string[10], right_here_string[10];
		sprintf(address_here_string, "%x", address_here);
		sprintf(left_here_string, "%x", left_here);
		sprintf(right_here_string, "%x", right_here);

		string temp1(address_here_string);
		string temp2(left_here_string);
		string temp3(right_here_string);

		existing_hash_fn_ip = temp1 + temp2 + temp3;
		}

        //cout << DD_node_value << endl ;  //to be decommented only during debugging......

        if(existing_hash_fn_ip == hash_fn_ip)
	  {
	    //cout<<"\nDD Hash Table Hit\n";
            break;
	  }
        else
	  {
            list_iterator++;
	  }
    } while(list_iterator!=(DD_ht_iterator->second).end());

    if(list_iterator == (DD_ht_iterator->second).end())
      {
	cout<<"\nCollision Found\n";
	//cout<<"\nDD Hash Table Miss\n";
	return NULL;
      }
    
    //cout<<"\nReturning the hitted entry\n";
    return *list_iterator;
}
