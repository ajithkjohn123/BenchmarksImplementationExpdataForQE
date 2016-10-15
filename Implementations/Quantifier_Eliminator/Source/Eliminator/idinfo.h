/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/

#ifndef __IDINFO_H
#define __IDINFO_H

struct IDinfo
{
    int pos;
    int len;
    int line;
    char *name;
    int type;
    char *upper;
    char *lower;

};


#endif

