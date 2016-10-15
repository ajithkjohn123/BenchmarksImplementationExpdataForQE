/* 
 * File:   DAG_hash_table.h
 * Author: sourabh
 *
 * Created on 20 May, 2009, 1:16 PM
 */

//prevent multiple inclusions of the header file......
#ifndef _DAG_HASH_TABLE_H
#define	_DAG_HASH_TABLE_H

    #include <map>
    #include <list>
    #include <string>
    #include <iostream>
    #include "CDAGNode.h"

    using namespace std;

    class DAG_hash_table
    {
        public:
            DAG_hash_table();
            void del_DAGnode(CDAGNode* );
            void Insert_DAGnode_in_ht(const char* , CDAGNode*);
            bool Check_DAGnode_in_ht(const char*);
            CDAGNode* Get_address_of_existing_DAGnode_from_ht(const char*, string);
            unsigned long hash_table_size();
            void gc_for_ht();
            
        private:
            map < unsigned long , list <CDAGNode*> > DAG_ht;
            unsigned long SDBMHash(const char*);
            void Delete_address_of_existing_DAGnode_from_ht(  const char* , CDAGNode*);
            void parse_DAG_ht_for_floating_nodes();

    };


#endif	/* _DAG_HASH_TABLE_H */

