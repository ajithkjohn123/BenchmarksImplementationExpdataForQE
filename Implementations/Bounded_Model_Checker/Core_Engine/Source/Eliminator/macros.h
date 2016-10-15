/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/

#ifndef _MACROS_H
#define	_MACROS_H

//difffent data types............

    #define UNKNOWN_DATA_TYPE   0
    
    #define LOGICAL_CONSTANT    1
    #define INTEGER_CONSTANT    2
    #define BITVECTOR_CONSTANT  3
    #define REAL_CONSTANT       4

    #define LOGICAL_SYMBOL      5
    #define INTEGER_SYMBOL      6
    #define BITVECTOR_SYMBOL    7
    #define REAL_SYMBOL         8

    #define LOGICAL_OPERATOR    9
    #define INTEGER_OPERATOR    10
    #define BITVECTOR_OPERATOR  11
    #define REAL_OPERATOR       12



    #define IMMATERIAL_WIDTH    -1

    //Currently no provision for signed or unsigned bit vectors.............
    //default implementation assumed in the bit vector library implementation......


//end of data type definitions...

#endif	/* _MACROS_H */

//this file is being used in the Oprs.h, SMT_LIB_operator_funcs class (.h and .cpp) and expression simplifier class (.h and .cpp)
//and DAGNode class (.h and .cpp) and will also be used by any new data structure used to represent the DAG.....

