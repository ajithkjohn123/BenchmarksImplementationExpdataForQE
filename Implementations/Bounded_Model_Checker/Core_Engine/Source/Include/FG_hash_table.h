/* 
 * File:   FG_hash_table.h
 * Author: sourabh
 *
 * Created on 7 July, 2009, 10:39 PM
 */

#ifndef _FG_HASH_TABLE_H
#define	_FG_HASH_TABLE_H

    #include <map>
    #include <list>
    #include <string>
    #include <iostream>
    #include <limits.h>
    #include "Factor_graph_Node.h"
    #include "CDAGNode.h"

    using namespace std;

    class FG_hash_table
    {
        public:
            FG_hash_table();
            void Insert_FGnode_in_ht(const char* , Factor_graph_Node*);
            bool Check_FGnode_in_ht(const char*);
            Factor_graph_Node* Get_address_of_existing_FGnode_from_ht(const char*, CDAGNode*);
            Factor_graph_Node* Get_FG_var_node_with_min_func(set<string>& );
            unsigned long hash_table_size();
            void Get_scope_reduced_funcs(set<CDAGNode*>& );
            void Release_FG_memory(void);

        private:
            map < unsigned long , list <Factor_graph_Node*> > FG_ht;
            unsigned long SDBMHash(const char*);
            void Delete_address_of_existing_FGnode_from_ht(  const char* , Factor_graph_Node*);
    };



#endif	/* _FG_HASH_TABLE_H */

