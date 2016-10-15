/* 
 * File:   Factor_graph_Node.h
 * Author: sourabh
 *
 * Created on 7 July, 2009, 7:08 PM
 */

#ifndef _FACTOR_GRAPH_NODE_H
#define	_FACTOR_GRAPH_NODE_H

    using namespace std;

    #include "CDAGNode.h"
    #include <set>

    

    class Factor_graph_Node {
    public:
        Factor_graph_Node(CDAGNode*, bool);
        ~Factor_graph_Node();
        void append_ptr_list(CDAGNode*);
        CDAGNode* Get_FGnode_value();
        bool Get_FGnode_type();
        unsigned int Get_FG_pointee_size();
        set<CDAGNode*>::iterator Get_FGnode_plist();
        void Delete_single_entry_from_plist(CDAGNode*);
        void Clear_plist();
        

    private:
        bool node_type; //true for function node, false for variable node...............
        CDAGNode* data; //address of DAGnode (data or function)
        set<CDAGNode*> ptr;
};

#endif	/* _FACTOR_GRAPH_NODE_H */

