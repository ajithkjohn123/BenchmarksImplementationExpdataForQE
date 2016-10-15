
#include "Factor_graph_Node.h"

Factor_graph_Node :: Factor_graph_Node(CDAGNode* data, bool node_type)
{
    this->node_type = node_type; //true for function node, false for variable node...............
    this->data = data; //address of DAGnode (data or function)
    this->ptr.clear();
}

void Factor_graph_Node :: append_ptr_list(CDAGNode* ptr)
{
    this->ptr.insert(ptr);
}

CDAGNode* Factor_graph_Node :: Get_FGnode_value()
{
    return this->data;
}

bool Factor_graph_Node :: Get_FGnode_type()
{
    return this->node_type;
}

unsigned int Factor_graph_Node :: Get_FG_pointee_size()
{
    return this->ptr.size();
}

Factor_graph_Node :: ~Factor_graph_Node()
{
    //empty destructor..........
}

set<CDAGNode*>::iterator Factor_graph_Node :: Get_FGnode_plist()
{
    set<CDAGNode*>::iterator itr = this->ptr.begin();
    return itr;
}

void Factor_graph_Node :: Delete_single_entry_from_plist(CDAGNode* var_address)
{
    this->ptr.erase(var_address);
}

void Factor_graph_Node :: Clear_plist(void)
{
    this->ptr.clear();
}



