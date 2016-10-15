//prevent multiple inclusions of the header file......
#ifndef _DD_HASH_TABLE_H
#define	_DD_HASH_TABLE_H

    #include <map>
    #include <list>
    #include <string>
    #include <iostream>
    #include "NewDDNode.h"

    using namespace std;

    class DD_hash_table
    {
        public:
            DD_hash_table();
            void Insert_DDnode_in_ht(const char* , DDNode*);
            bool Check_DDnode_in_ht(const char*);
            DDNode* Get_address_of_existing_DDnode_from_ht(const char*, string);
            unsigned long hash_table_size();
            void Delete_address_of_existing_DDnode_from_ht(  const char* , DDNode*);
	    void showDD_hash_table(void);
	    DDNode* Get_address_of_DDnode_from_ht(string hash_fn_ip);

        private:
            map < unsigned long , list <DDNode*> > DD_ht;
            unsigned long SDBMHash(const char*);
            
     };


#endif	/* _DD_HASH_TABLE_H */

