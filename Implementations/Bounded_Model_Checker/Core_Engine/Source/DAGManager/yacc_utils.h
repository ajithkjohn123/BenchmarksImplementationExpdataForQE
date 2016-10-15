#ifndef DAG_READER_H
#define DAG_READER_H
#include<vector>

#include "DagManager.h"

using namespace std;





class VectorOfExpressions
{
 public:
  vector<DAGNode *> v_expressions;
  VectorOfExpressions(vector<DAGNode* > v_expressions_to_push)
    {
      for(int i=0;i<v_expressions_to_push.size();i++)
        {
          v_expressions.push_back(v_expressions_to_push[i]);
        }
    }
  VectorOfExpressions() //A do nothing constructor..
    {
    }

};


vector<DAGNode *> assign_expressions(class VectorOfExpressions *v_exprs_inst);


#endif
