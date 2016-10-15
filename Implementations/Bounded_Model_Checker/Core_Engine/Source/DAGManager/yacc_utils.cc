
#include "yacc_utils.h"

using namespace std;





vector<DAGNode *> assign_expressions(
                                          class VectorOfExpressions *v_exprs_in
                                          )
{
  vector<DAGNode *>v_exprs;
  for(int i=0;i<v_exprs_in->v_expressions.size();i++)
    {
      v_exprs.push_back(v_exprs_in->v_expressions[i]);
    }

  return v_exprs;
}









