

#include "CDAGNode.h"

CDAGNode :: ~CDAGNode()  //destructor for the DAG node......
{
    //cout << endl << "DAGNode destroyed" << endl ;   //to be used only during debugging.......
}

CDAGNode :: CDAGNode()  //constructor.....with empty argument
{
    name = " ";
    delta_time = 0;
    node_type = false;  //as operand node.....
    labels.clear();
    out_list.clear();
    in_list.clear();
    visited = false;
    cost = 0;
    root = true;    //a node is always the root at creation........
}

CDAGNode :: CDAGNode(string node_name)  //constructor.....with 1 argument (dag node name)
{
    name = node_name;
    delta_time = 0;
    node_type = true;  //although not an operator node but the concept of delta time is absent here........
    labels.clear();
    out_list.clear();
    in_list.clear();
    visited = false;
    cost = 0;
    root = true;    //a node is always the root at creation........

    //cout << "DAG node name created :: " << name << endl << endl ;
}

CDAGNode :: CDAGNode(string n, int dt)  //constructor.....with 2 argument (node name and delta-time )
{
    name = n;
    delta_time = dt;
    node_type = false;  //as operand node.....
    labels.clear();
    out_list.clear();
    in_list.clear();
    visited = false;
    cost = 0;
    root = true;    //a node is always the root at creation........

    //cout << "DAG node name created :: " << name << endl << endl ;
}

CDAGNode :: CDAGNode(string op_n , CDAGNode* oprnd)  //constructor.....for unary operators
{
    name = op_n;
    node_type = true;   //as operator node node.....
    delta_time = 0;
    labels.clear();
    out_list.clear();
    out_list.push_back(oprnd);
    in_list.clear();
    visited = false;
    cost = 0;
    root = true;    //a node is always the root at creation........

    //cout << "DAG node name created :: " << name << endl << endl ;
}

CDAGNode :: CDAGNode(string op_n , CDAGNode* oprnd1,  CDAGNode* oprnd2)  //constructor.....for binary operators
{
    name = op_n;
    node_type = true;   //as operator node node.....
    delta_time = 0;
    labels.clear();
    out_list.clear();
    out_list.push_back(oprnd1);
    out_list.push_back(oprnd2);
    in_list.clear();
    visited = false;
    cost = 0;
    root = true;    //a node is always the root at creation........

    //cout << "DAG node name created :: " << name << endl << endl ;
}

CDAGNode :: CDAGNode(string op_n , CDAGNode* oprnd1,  CDAGNode* oprnd2,  CDAGNode* oprnd3)
//constructor.....for ternary operators
{
    name = op_n;
    node_type = true;   //as operator node node.....
    delta_time = 0;
    labels.clear();
    out_list.clear();
    out_list.push_back(oprnd1);
    out_list.push_back(oprnd2);
    out_list.push_back(oprnd3);
    in_list.clear();
    visited = false;
    cost = 0;
    root = true;    //a node is always the root at creation........

    //cout << "DAG node name created :: " << name << endl << endl ;
}

string CDAGNode :: Get_DAGnode_value(void)
{
    return this->name;
}

string CDAGNode :: Get_DAGnode_delta_time_as_string(void)
{
    char char_ptr[10] = " ";
    sprintf(char_ptr , "%d", delta_time);

    string dt(char_ptr);

    return dt;
}

int CDAGNode :: Get_DAGnode_delta_time_as_int(void)
{
    return this -> delta_time ;
}


void CDAGNode :: Append_DAGnode_inlist(CDAGNode* oprnd)
{
    this->in_list.push_back(oprnd);
    mark_redendant_node_for_deletion();
}

bool CDAGNode :: isroot(void)
{
    return this->root ;
}

void CDAGNode :: mark_redendant_node_for_deletion(void)
{
    this->root = false ;
}

void CDAGNode :: Delete_single_entry_from_DAG_inlist(CDAGNode* entry_to_be_deleted)
{
    this->in_list.remove(entry_to_be_deleted);
}

unsigned int CDAGNode :: Get_DAGnode_inlist_size()
{
    unsigned int size ;
    size = (unsigned int) this->in_list.size();
    return size;
}

list<CDAGNode*>::iterator CDAGNode :: Get_DAGnode_inlist()
{
    list<CDAGNode*>::iterator itr = this->in_list.begin();
    return itr;
}

//returns an iterator pointing to the begining of the inlist

void CDAGNode :: Set_DAGnode_delta_time(int new_dt)
{
   this->delta_time = new_dt;
}

string CDAGNode :: Print_DAGnode_attributes(void)
{
    char temp_char[10] = " ";

    string dag_str = ",{ name = ";
    dag_str += this->name;

    dag_str += "_( node type = ";
    if(this->node_type)
        dag_str += "operator";
    else
        dag_str += "operand";
    dag_str += " )";

    dag_str += "_( delta_time = ";
    if(this->node_type)
        dag_str += "irrelevant";
    else
        dag_str += Get_DAGnode_delta_time_as_string();
    dag_str += " )";

    dag_str += "_( labels = ";
    list <string> :: iterator lb_iter = labels.begin();
    for(; lb_iter != labels.end(); lb_iter++)
    {
        dag_str += ", ";
        dag_str += *lb_iter;
    }
    dag_str += " )";
    
    dag_str += "_( out_list = ";
    list <CDAGNode*> :: iterator ol_iter = out_list.begin();
    for(; ol_iter != out_list.end(); ol_iter++)
    {
        sprintf(temp_char , "%x", *ol_iter);
        string ol(temp_char);
        dag_str += ", ";
        dag_str += ol;
        temp_char[10];
    }
    dag_str += " )";

    dag_str += "_( inlist = ";
    list <CDAGNode*> :: iterator il_iter = in_list.begin();
    for(; il_iter != in_list.end(); il_iter++)
    {
        sprintf(temp_char , "%x", *il_iter);
        string il(temp_char);
        dag_str += ", ";
        dag_str += il;
        temp_char[10];
    }
    dag_str += " )";

    dag_str += "_( status = ";
    if(this->visited)
        dag_str += "visited";
    else
        dag_str += "not visited";
    dag_str += " )";

    dag_str += "_( ";
    if(this->root)
        dag_str += "This is a ROOT node";
    dag_str += " )";

    dag_str += "_( Own Address = ";
    sprintf(temp_char , "%x", this);
    string own_addr(temp_char);
    dag_str += own_addr;
    dag_str += " )";

    dag_str += " }";

    return dag_str;
}

bool CDAGNode :: Get_DAG_node_type(void)
{
    return this->node_type;
}

bool CDAGNode :: equals(CDAGNode* dag2)
{

    //cout<<"\nDAG1->NAME="<<name<<",DAG2->NAME="<<dag2->name<<endl;
    if (this->name != dag2->Get_DAGnode_value()
        || this->delta_time != dag2->Get_DAGnode_delta_time_as_int()
        || this->out_list.size() != dag2->Get_DAGnode_outlist_size())
    {
        return false;
    }

    list<CDAGNode*>::iterator iout1 = this->out_list.begin();
    list<CDAGNode*>::iterator iout2 = dag2->Get_DAGnode_outlist();

    bool ret_value = true;

    for(; iout1 != out_list.end(); iout1++, iout2++ )
    {
        ret_value = ret_value & (*iout1)->equals(*iout2);
    }

    return ret_value;
}

bool CDAGNode :: less(CDAGNode *dag2)
{
    if(this->name < dag2->Get_DAGnode_value())
    {
        return true;
    }
    else if(this->delta_time < dag2->Get_DAGnode_delta_time_as_int())
    {
        return true;
    }
    else if(this->out_list.size() < dag2->Get_DAGnode_outlist_size())
    {
        return true;
    }
    else
    {
        list<CDAGNode*>::iterator iout1 = this->out_list.begin();
        list<CDAGNode*>::iterator iout2 = dag2->Get_DAGnode_outlist();

        for(; iout1 != out_list.end(); iout1++, iout2++ )
        {
            if((*iout1)->less(*iout2))
            {
                return true;
            }
        }
    }
    return false;
}

pair<int, int> CDAGNode :: getLHSRange(void)
{
    if(this->name != "equivalent")
    {
        return make_pair(-1, -1);
    }

    list<CDAGNode*>::iterator iout1 = this->Get_DAGnode_outlist();

    if((*iout1)->name != "select")
    {
        return make_pair(-1, -1);
    }

    list<CDAGNode*>::iterator iout2 = (*iout1)->Get_DAGnode_outlist();

    iout2++;
    int lower = atoi((*iout2)->name.c_str());

    iout2++;
    int upper = atoi((*iout2)->name.c_str());

    return make_pair(lower, upper);
}

void CDAGNode :: getSize(int &excnt, int &count, set<CDAGNode*> &Nodes)
{

  if(Nodes.find(this) != Nodes.end())
    return;
  else
    {
      Nodes.insert(this);

      if(name == "exists")
	{
	  excnt++;
	}
      
      string id_delta = this->name + "_" + this->Get_DAGnode_delta_time_as_string();

      if((this->node_type== true) && (this->out_list.size()==0))
	{
	  return;
	}

      count++;    

      list<CDAGNode*>::iterator iout = out_list.begin();

      for(; iout != out_list.end(); iout++)
	{
	  (*iout)->getSize(excnt, count, Nodes);
	}
    }
}
 

void CDAGNode :: size(int &excnt, int &count)
{
   set<CDAGNode*> Nodes;

   Nodes.clear();

   getSize(excnt, count, Nodes);

}



unsigned int CDAGNode :: Get_DAGnode_outlist_size(void)
{
    unsigned int size ;
    size = (unsigned int) this->out_list.size();
    return size;
}

list<CDAGNode*>::iterator CDAGNode :: Get_DAGnode_outlist(void)
{
    list<CDAGNode*>::iterator itr = this->out_list.begin();
    return itr;
}

string CDAGNode :: getDAGStr()
// Function to return the content of an entire tree in a string
{
  
    string dag_str = name;

    if(node_type == false)  //i.e. only for "operand" nodes delta time makes some sense......
    {
        char temp[10];
        sprintf(temp, "%d", delta_time);
        dag_str += "_";
        dag_str += temp;
        return dag_str;
    }

    dag_str += "(";
    list<CDAGNode*> :: iterator iout = out_list.begin();
    for(; iout != out_list.end(); iout++)
    {
        dag_str += (*iout)->getDAGStr() + ",";
    }
    dag_str += ")";
    return dag_str;
}

void CDAGNode :: getDAGNames(set<string> &names)
{
    string dag_str = name;

    if(dag_str == "exists")
    {
        set<string> ExistsVarSet;
        set<string> VarSet;

        list<CDAGNode*>::iterator iout = out_list.begin();

        (*iout)->getDAGNames(ExistsVarSet);
        iout++;
        (*iout)->getDAGNames(VarSet);

        set_difference(VarSet.begin(), VarSet.end(), ExistsVarSet.begin(), ExistsVarSet.end(),
                                                               inserter(names, names.begin()));
    }
    else
    {
        if(node_type == false)  //if the dag value is vaiable.....
        {
            char temp[10];

            sprintf(temp, "%d", delta_time);

            //char *t = strdup(dag_str.c_str());

            dag_str += "_";

            dag_str += temp;

            names.insert(dag_str);
        }

        list<CDAGNode*>::iterator iout = out_list.begin();

        for(; iout != out_list.end(); iout++)
        {
            (*iout)->getDAGNames(names);
        }
    }

}



void CDAGNode :: getExistsDAGNames(set<string> &ExistsVarSet)
// get the existentially qunatified  variable names
//(only the name - not sel(name, index, index) from the DAG
{
    string dag_str = name;

    if(dag_str == "exists")
    {
        set<string> NewVarSet;

        list<CDAGNode*>::iterator iout = out_list.begin();

        (*iout)->getDAGNames(NewVarSet);

        // add NewVarSet to ExistsVarSet
        set_union(NewVarSet.begin(), NewVarSet.end(), ExistsVarSet.begin(), ExistsVarSet.end(),
                                                  inserter(ExistsVarSet, ExistsVarSet.begin()));
        iout++;

        (*iout)->getExistsDAGNames(ExistsVarSet);

    }
    else
    {
        list<CDAGNode*>::iterator iout = out_list.begin();

        for(; iout != out_list.end(); iout++)
        {
            (*iout)->getExistsDAGNames(ExistsVarSet);
        }
    }

}

void CDAGNode :: findExistsDAGs(list<CDAGNode*> &ExistsList)
{

    string dag_str = this->name;

    if(dag_str == "exists")
    {
        ExistsList.push_back(this);
    }
    
    list<CDAGNode*>::iterator iout = this->out_list.begin();

    for(; iout != this->out_list.end(); iout++)
    {
        (*iout)->findExistsDAGs(ExistsList);
    }
}




/* This is the modified calculateDeltaDelays function. The previous calculateDeltaDelays function is given above (commented). The difference is this. 

Let we have an input x(0:10). It may be appearing in the symbolic expression as x(0:4) and x(5:10). Hence what this modified function does is that, it takes each variable from the symbolic expression, checks if there is an input with input's name = variable's name and input's range = variable's range. If yes, the variable's delta value is read. If a match is not found for the variable, it checks if there is an input with input's name = variable's name. If yes, the variable's delta value is read. Else the variable's delta value is not read */

void CDAGNode :: calculateDeltaDelays(map<CSensitiveElement*, string> &delays)
{
    list<CDAGNode*>::iterator iout = out_list.begin();

    if(name == "select")
    {

      string signal_name = (*iout)->Get_DAGnode_value();
      int signal_delta_time = (*iout)->Get_DAGnode_delta_time_as_int();
      iout++;
      string signal_lower = (*iout)->Get_DAGnode_value();
      iout++;
      string signal_upper = (*iout)->Get_DAGnode_value();

      map<CSensitiveElement*, string>::iterator delays_it;

      bool match_found = false;

      for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
        {
          if( ((delays_it->first)->name == signal_name) && ((delays_it->first)->lower == signal_lower) && ((delays_it->first)->upper == signal_upper))
            {
              if(delays_it->second == "")

                {
                  char temp[20];
                  sprintf(temp,"%d",signal_delta_time);
                  delays_it->second = temp;
                  //cout<<"\n\nIN IF PART\n\n";
                }

              else if( signal_delta_time > atoi((delays_it->second).c_str()) )
                {
                  char temp[20];
                  sprintf(temp,"%d",signal_delta_time);
                  delays_it->second = temp;
                  //cout<<"\n\nIN ELSE PART\n\n";
                }
              match_found = true;
              break;
            }
        }

      if(!match_found)
        {
           for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
             {
               if((delays_it->first)->name == signal_name)
                 {
                   if(delays_it->second == "")

                     {
                       char temp[20];
                       sprintf(temp,"%d",signal_delta_time);
                       delays_it->second = temp;
                       //cout<<"\n\nIN IF PART\n\n";
                     }

                   else if( signal_delta_time > atoi((delays_it->second).c_str()) )
                     {
                       char temp[20];
                       sprintf(temp,"%d",signal_delta_time);
                       delays_it->second = temp;
                       //cout<<"\n\nIN ELSE PART\n\n";
                     }
                   match_found = true;
                   break;
                 }
             }
        }
    }
    for(; iout != out_list.end(); iout++)
    {
            (*iout)->calculateDeltaDelays(delays);
    }
}


// FUNCTIONS FOR INTEGRATING WITH QELIMINATOR ADDED FROM 30/12/09 ONWARDS

string CDAGNode :: Get_DAGnode_data_as_id_delta(void)
{
    char char_ptr[10] = " ";
    sprintf(char_ptr , "%d", delta_time);
    string dt(char_ptr);
    string dag_str = name;
    dag_str +="_";
    dag_str += dt;

    return dag_str;
}


// New functions for QEliminator

bool CDAGNode :: freeOfVar(string VarToElim)
{
set<string> names;
this->getDAGNames(names);
if(names.find(VarToElim) == names.end())  return true;
return false;
}


void CDAGNode::getConstant(int &constant)
{
  string dag_str = name;
  
  if(node_type && this->Get_DAGnode_outlist_size()==0) // constant
    
    {
      constant = convertLiteralToNumber(dag_str);
    }
  else
    {
     int sign;

      if(dag_str == "+" || dag_str == "-")
	{
	  if(dag_str == "+") sign=0; else sign=1;
	  
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	  iout++;
	  CDAGNode *rhs=*iout;
	  
	  if(rhs->Get_DAGnode_outlist_size()==0) // leaf level

	    {
	      if(rhs->Get_DAG_node_type() == true) // constant
		{
		  constant = convertLiteralToNumber(rhs->Get_DAGnode_value());
		  if(sign==1) constant=constant*-1;
		}
	      else
		return;
	    }
	  else
	    return;
	}
    }
}




ULINT CDAGNode::convertLiteralToNumber(string literal)
{
  ULINT number;
  //cout<<"\nliteral="<<literal<<endl;

  if(literal[1] =='1' && literal[0] =='\'' && literal[2] =='\'') 
    number=1; 
  else if(literal[1] =='0' && literal[0] =='\'' && literal[2] =='\'')  
    number=0; 
  else if(literal[0]=='x')
    {
      literal.erase(literal.begin(), literal.begin()+2);
      literal.erase(literal.end()-1, literal.end());
      //cout<<"\nliteral="<<literal<<endl;
      number=convertToDecimal(literal);
    }
  else // Normal decimal number
    {
      //number=atoi(literal.c_str());
      number=strtoull(literal.c_str(), NULL, 0);
    }
  
  
  //cout<<"\nnumber inside fn="<<number;
return number;
}

ULINT CDAGNode::getCoefOfChar(char c)
{
switch(c)
	{
	case '0' : return 0;
	case '1' : return 1;
	case '2' : return 2;
	case '3' : return 3;
	case '4' : return 4;
	case '5' : return 5;
	case '6' : return 6;
	case '7' : return 7;	
	case '8' : return 8;	
	case '9' : return 9;
	case 'a' : return 10;
	case 'b' : return 11;
	case 'c' : return 12;
	case 'd' : return 13;	
	case 'e' : return 14;
	case 'f' : return 15;
	}
}

ULINT CDAGNode::findPower16(int power)
{
int number=1;
int i=power;
while(i>0)
	{
	number=number*16;
	i--;
	}
return number;
}

ULINT CDAGNode::convertToDecimal(string x)
{
char c;
ULINT coef, power, power2, length;
ULINT number=0, i=0;
char *y=strdup(x.c_str()); 
length=x.length();

while(y[i]!='\0')
	{
	c=y[i];
	x.erase(x.begin(), x.begin());
	coef=getCoefOfChar(c);
	power=length-i-1;
	//cout<<"\ncoef="<<coef<<", power="<<power<<endl;
	power2=findPower16(power);
	//cout<<"\npower2="<<power2<<endl;
	number+=power2*coef;
	//cout<<"\nnumber="<<number<<endl;
	i++;
	}

return number;
}

void CDAGNode::getConstantModified(ULINT &constant)
{
 // Go down the + operator until constant is encountered.

  string dag_str = name;
  
  if(node_type && this->Get_DAGnode_outlist_size()==0) // constant
    
    {
      constant = convertLiteralToNumber(dag_str); 
      return;
    }
  else if(dag_str == "+")
	{
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	   
          (*iout)->getConstantModified(constant);
	  iout++;
	  (*iout)->getConstantModified(constant);
	}
}


void CDAGNode::getCoefficient(string Var, ULINT &Coefficient)
{
	 string dag_str = name;

         if(dag_str == "*")
	{
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	   CDAGNode *coef = *iout;
	   iout++;
	   CDAGNode *var = *iout;
	   string variable = var->Get_DAGnode_data_as_id_delta();

	   //cout<<"\nvariable = "<<variable<<endl;

           if(variable == Var)
		{
		coef->getConstantModified(Coefficient);
		//cout<<"\nCoefficient = "<<Coefficient<<endl;
		}
	}
  
      else     
	{
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	  
          for(; iout!=this->out_list.end(); iout++)
		(*iout)->getCoefficient(Var, Coefficient);
	}
}

bool CDAGNode :: freeOfVarModified(string VarToElim)
{
set<string> names;
this->getDAGNames(names);
if(names.find(VarToElim) == names.end())  return true;
ULINT coefficient=1;
this->getCoefficient(VarToElim, coefficient);
if(coefficient == 0) return true;
return false;
}


// This function returns the contents of dag in Yices format
// ASSUMPTIONS : 1) not_equal_to(is_not_equal), equivalent(=) applies only between bit-vectors
//               2) we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. 
//               3) ite is (ite bool bool bool) 
//
//		 1 & 2 are valid from practical examples and 3 valid given the formula comes with dag of proper propositional skelton form
//


string CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormat(string context)
{
        string dag_str;

        if(!node_type)
        {
               dag_str = Get_DAGnode_value() + "_" + Get_DAGnode_delta_time_as_string();
               return dag_str;
        }
        else
          {

            //cout<<"\nNAME=\t"<<name<<endl;    //debug code

            if(name == "select")
              {
                dag_str = "(bv-extract ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                iout++;

                iout++;

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("const") + " ";

                iout = out_list.begin();

                iout++;

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("const") + " ";

                iout = out_list.begin();

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("bv");

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("bv") + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("bv") + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {
		dag_str = "(bv-concat ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("bv") + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "*")
	      {
		dag_str = "(bv-mul ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("bv") + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "+")
	      {
		dag_str = "(bv-add ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("bv") + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
	    

	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "ite"))
	      {
		dag_str = "(" +  name + " ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormat("bool") + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {
                dag_str = "0b1";

		return dag_str;
              }

	    else if(name == "'0'")
              {
                dag_str = "0b0";

		return dag_str;
              }

	    else if(Get_DAGnode_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      //cout<<"\ndag_str = "<<dag_str<<endl; 
		      return dag_str;
		      }
		else if(context == "bv")
		      {
			//cout<<"\nname = "<<name<<endl; 
			
			if(name[0] == 'x') // hex
			  dag_str = convertHexToBitvector(name);
			else // Decimal
			  {
			    if(name == "1") dag_str="0b1";
			    else if(name == "0") dag_str="0b0";
			    else dag_str = convertDecimalToBitvector(name);
			  }
			  
			//cout<<"\ndag_str = "<<dag_str<<endl; 		
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else dag_str = name;
			return dag_str;
		      }
		else
		      {
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormat\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormat\n";
		exit(1);
	      }
		    
	  }
}


string CDAGNode::getBinaryOfChar(char c)
{
switch(c)
	{
	case '0' : return "0000";
	case '1' : return "0001";
	case '2' : return "0010";
	case '3' : return "0011";
	case '4' : return "0100";
	case '5' : return "0101";
	case '6' : return "0110";
	case '7' : return "0111";	
	case '8' : return "1000";	
	case '9' : return "1001";
	case 'a' : return "1010";
	case 'b' : return "1011";
	case 'c' : return "1100";
	case 'd' : return "1101";	
	case 'e' : return "1110";
	case 'f' : return "1111";
	}
}


string CDAGNode::convertHexToBitvector(string hex)
{
 

hex.erase(hex.begin(), hex.begin()+2);
hex.erase(hex.end()-1, hex.end());

char c;
int i=0;
char *y=strdup(hex.c_str()); 
string bv="0b";
string position;

 while(y[i]!='\0')
	{
	c=y[i];
	position = getBinaryOfChar(c);
	bv=bv+position;
	i++;
	}

return bv;
}


// FUNCTIONS FOR PRINITING IN YICES IN PROPER FORMAT : CURRENTLY NOT DONE PROPERLY; HENCE COMMENTED

/*
void CDAGNode :: Set_id(ULINT MyId)
{
  this->id = MyId;
}

ULINT CDAGNode :: Get_id()
{

  return this->id;
}

void CDAGNode :: nullifyVisited()
{
  this->visited = false;

  for(list<CDAGNode*>::iterator iout = out_list.begin(); iout != out_list.end(); iout++)
    (*iout)->nullifyVisited();
}

void CDAGNode :: attachID(ULINT &MyID)
{
  if(this->visited) return; // node already visited
  if(!this->node_type) return; // operand node has no id
  this->visited=true;
  this->Set_id(My_ID);
  My_ID++;
  
  for(list<CDAGNode*>::iterator iout = out_list.begin(); iout != out_list.end(); iout++)
    (*iout)->attachID(MyID);
}
  
void CDAGNode :: setIdentifiers()
{
  this->nullifyVisited(); // Nullify visited field of all nodes in the dag
  ULINT MyID=0;
  this->attachID(MyID); // attach unique ids to all operator nodes
}

*/


string CDAGNode::convertDecimalToBitvector(string dec)
{
string bv="0b";
ULINT number = strtoull(dec.c_str(), NULL, 0);
string str = integerToBinaryString(number);
bv += str;

return bv;
}

string CDAGNode::convertDecimalToBitvectorWithProperWidth(string dec, int Width)
{
string bv="0b";
ULINT number = strtoull(dec.c_str(), NULL, 0);
string str = integerToBinaryString(number);
int NoOfZeros = Width - str.length();

//cout<<"\nNumber = "<<dec<<endl;
//cout<<"\nOriginal BV = "<<str<<endl;

for(int i=0; i<NoOfZeros; i++)
   bv += "0";
 
bv += str;

//cout<<"\nBV in Yices format  = "<<bv<<endl;

return bv;
}


string CDAGNode::convertDecimalToBitvectorWithProperWidthInSMTLibFormat(string dec, int Width)
{

  if(Width>32) {cout<<"\nWidth more than 32 is not permitted by SMTLib format. Exiting\n";}

  // Width is the required width

  int msb = Width-1;
  int lsb = 0;

  char msb_char[100], lsb_char[100];
  
  sprintf(msb_char, "%d", msb);
  sprintf(lsb_char, "%d", lsb);

  string msb_string, lsb_string;

  msb_string = msb_char;
  lsb_string = lsb_char;
 
  string bv="(extract[";
  bv+=msb_string;
  bv+=":";
  bv+=lsb_string;
  bv+="] ";


 
bv += "bv";
 bv += dec;
 bv+=")"; 

 cout<<"\nInput Number = "<<dec<<endl;
cout<<"\nBV in SMTLib format  = "<<bv<<endl;

return bv;
}


string CDAGNode::convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(string dec, int Width)
{

  if(Width>32) {cout<<"\nWidth more than 32 is not permitted by SMTLib format. Exiting\n"; exit(1);}

  // Width is the required width

  char width_char[100];

  sprintf(width_char, "%d", Width);
  
  string bv = "bv";
  bv += dec;
  bv+="[";
  bv+=width_char;
  bv+="] ";


  //cout<<"\nInput Number = "<<dec<<endl;
  //cout<<"\nBV in SMTLib format  = "<<bv<<endl;

  return bv;
}




string CDAGNode::integerToBinaryString(ULINT i)
{
    if(i==0)
        return "0";
    string bin_str="";
    ULINT j=i;
    while(j!=0)
    {
        if(j%2 == 0)
            bin_str = bin_str + "0";
        else
            bin_str = bin_str + "1";
        j = j / 2;
    }
    string str="";
    //Reversing the binary string to bring it to final format
    for(int k=bin_str.size()-1;k>=0;k--)
        str += bin_str[k];
    return str;
}


// Slightly modified version of getDAGOfProperSkeltonFormatInYicesFormat with the additional feature that for decimal constants, the bit-vector printed is adjusted with prefixing zeros.

string CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required)
{
        string dag_str;

        if(!node_type)
        {
               dag_str = Get_DAGnode_value() + "_" + Get_DAGnode_delta_time_as_string();
               return dag_str;
        }
        else
          {

            //cout<<"\nNAME=\t"<<name<<endl;    //debug code

            if(name == "select")
              {
                dag_str = "(bv-extract ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                iout++;

                iout++;

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("const", WidthTableForNodes, 1) + " ";

                iout = out_list.begin();

                iout++;

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("const", WidthTableForNodes, 1) + " ";

                iout = out_list.begin();

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {

		cout<<"\nconcat is not supported by function CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		exit(1);

		/*
		dag_str = "(bv-concat ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bv", WidthTableForNodes) + " ";
                  }

                dag_str += ")";

                return dag_str; */
              }

	    else if(name == "*")
	      {
		dag_str = "(bv-mul ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "+")
	      {
		dag_str = "(bv-add ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // Newly added on 20.1.2010

	     else if(name == "-")
	      {
		dag_str = "(bv-sub ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    // Newly added on 20.1.2010 ends here
	    

	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "ite"))
	      {
		dag_str = "(" +  name + " ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];
		

                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);
		
		//dag_str = "0b1";

		//return dag_str;
              }

	    else if(name == "'0'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];


                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

		//dag_str = "0b0";

		//return dag_str;
              }

	    else if(Get_DAGnode_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      //cout<<"\ndag_str = "<<dag_str<<endl; 
		      return dag_str;
		      }
		else if(context == "bv")
		      {
			//cout<<"\nname = "<<name<<endl; 

			// following line added on 19.1.2010

			//int Width_Required = WidthTableForNodes[this];
			
			if(name[0] == 'x') // hex
			  {
			    //dag_str = convertHexToBitvector(name);
			  dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);
			  }
			else // Decimal
			  {

			    // Following if else if commented on 18 Jan 2010 

			    //if(name == "1") dag_str="0b1";
			    //else if(name == "0") dag_str="0b0";
			    // else 
			    //  {
				dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);
				//  }
			  }
			  
			//cout<<"\ndag_str = "<<dag_str<<endl; 		
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";
			//else dag_str = name;

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
				//dag_str = convertHexToBitvector(name);
				dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);
			      }
			    else // Decimal
			      {
				dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);
			      }
			  }

			
			return dag_str;
		      }
		else
		      {
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable\n";
		exit(1);
	      }
		    
	  }
}




/* This is the modified calculateDeltaDelays function ON 11/1/2009. The previous calculateDeltaDelays function is given above (commented). The difference is this. 

Let we have an input x(0:10). It may be appearing in the symbolic expression as x(0:4) and x(5:10). Hence what this modified function does is that, it takes each variable from the symbolic expression, checks if there is an input with input's name = variable's name and input's range = variable's range. If yes, the variable's delta value is read. If a match is not found for the variable, it checks if there is an input with input's name = variable's name. If yes, the variable's delta value is read. Else the variable's delta value is not read */

void CDAGNode :: calculateDeltaDelaysModified(map<CSensitiveElement*, string> &delays)
{
    list<CDAGNode*>::iterator iout = out_list.begin();

    if(name == "select")
    {
      string signal_name;
      int signal_delta_time;
      
      (*iout)->getVariableNameFromSelect(signal_name, signal_delta_time);
     
      iout++;
      string signal_lower = (*iout)->Get_DAGnode_value();
      iout++;
      string signal_upper = (*iout)->Get_DAGnode_value();

      map<CSensitiveElement*, string>::iterator delays_it;

      bool match_found = false;

      for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
        {
          if( ((delays_it->first)->name == signal_name) && ((delays_it->first)->lower == signal_lower) && ((delays_it->first)->upper == signal_upper))
            {
              if(delays_it->second == "")

                {
                  char temp[20];
                  sprintf(temp,"%d",signal_delta_time);
                  delays_it->second = temp;
                  //cout<<"\n\nIN IF PART\n\n";
                }

              else if( signal_delta_time > atoi((delays_it->second).c_str()) )
                {
                  char temp[20];
                  sprintf(temp,"%d",signal_delta_time);
                  delays_it->second = temp;
                  //cout<<"\n\nIN ELSE PART\n\n";
                }
              match_found = true;
              break;
            }
        }

      if(!match_found)
        {
           for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
             {
               if((delays_it->first)->name == signal_name)
                 {
                   if(delays_it->second == "")

                     {
                       char temp[20];
                       sprintf(temp,"%d",signal_delta_time);
                       delays_it->second = temp;
                       //cout<<"\n\nIN IF PART\n\n";
                     }

                   else if( signal_delta_time > atoi((delays_it->second).c_str()) )
                     {
                       char temp[20];
                       sprintf(temp,"%d",signal_delta_time);
                       delays_it->second = temp;
                       //cout<<"\n\nIN ELSE PART\n\n";
                     }
                   match_found = true;
                   break;
                 }
             }
        }
    }


    else if(!node_type) // Leaf level node encountered
    {
      string signal_name = this->Get_DAGnode_value();
      int signal_delta_time = this->Get_DAGnode_delta_time_as_int();
      
     
      map<CSensitiveElement*, string>::iterator delays_it;

      bool match_found = false;

      for(delays_it = delays.begin();delays_it != delays.end(); delays_it++)
             {
               if((delays_it->first)->name == signal_name)
                 {
                   if(delays_it->second == "")

                     {
                       char temp[20];
                       sprintf(temp,"%d",signal_delta_time);
                       delays_it->second = temp;
                       //cout<<"\n\nIN IF PART\n\n";
                     }

                   else if( signal_delta_time > atoi((delays_it->second).c_str()) )
                     {
                       char temp[20];
                       sprintf(temp,"%d",signal_delta_time);
                       delays_it->second = temp;
                       //cout<<"\n\nIN ELSE PART\n\n";
                     }
                   match_found = true;
                   break;
                 }
             }
    }


    for(; iout != out_list.end(); iout++)
    {
            (*iout)->calculateDeltaDelaysModified(delays);
    }
}


void CDAGNode::getVariableNameFromSelect(string &MyName, int &MyDelta)
{
  
  if(this->Get_DAGnode_value() != "select")
    {
      MyName = this->Get_DAGnode_value();
      MyDelta = this->Get_DAGnode_delta_time_as_int();
    }

  else // go down the first child until a node which is not select is encountered
    {
      
      list<CDAGNode*>::iterator iout = out_list.begin();
      (*iout)->getVariableNameFromSelect(MyName, MyDelta);
    }
}
    

/* NEW FUNCTIONS ADDED ON 19/1/2010 */

  
string CDAGNode::convertHexToBitvectorWithProperWidth(string hex, int Width)
{
hex.erase(hex.begin(), hex.begin()+2);
hex.erase(hex.end()-1, hex.end());
string bv="0b";

string str = hexToBinaryString(hex);
 
int NoOfZeros = Width - str.length();

for(int i=0; i<NoOfZeros; i++)
   bv += "0";
 
bv += str;

//cout<<"\nBV in Yices format  = "<<bv<<endl;

return bv;
}


string CDAGNode::convertHexToBitvectorWithProperWidthInSMTLibFormat(string hex, int Width)
{

  if(Width>32) {cout<<"\nWidth more than 32 is not permitted by SMTLib format. Exiting\n";}

  // Width is the required width

  int msb = Width-1;
  int lsb = 0;

  char msb_char[100], lsb_char[100];
  
  sprintf(msb_char, "%d", msb);
  sprintf(lsb_char, "%d", lsb);

  string msb_string, lsb_string;

  msb_string = msb_char;
  lsb_string = lsb_char;
 
  string bv="(extract[";
  bv+=msb_string;
  bv+=":";
  bv+=lsb_string;
  bv+="] ";

 
hex.erase(hex.begin(), hex.begin()+2);
hex.erase(hex.end()-1, hex.end());
// now hex is in proper hexa decimal form
ULINT number = convertToDecimal(hex);

 char temp[100];
 sprintf(temp, "%d", number);
 string str(temp);

 
bv += "bv";
 bv += str;
 bv+=")"; 


cout<<"\nInput Number in hex = "<<hex<<endl;
cout<<"\nBV in SMTLib format  = "<<bv<<endl;

return bv;
}


string CDAGNode::convertHexToBitvectorWithProperWidthInNewSMTLibFormat(string hex, int Width)
{

  if(Width>32) {cout<<"\nWidth more than 32 is not permitted by SMTLib format. Exiting\n";}

  // Width is the required width

  char width_char[100];
  
  sprintf(width_char, "%d", Width);
 
  hex.erase(hex.begin(), hex.begin()+2);
  hex.erase(hex.end()-1, hex.end());
  // now hex is in proper hexa decimal form
  ULINT number = convertToDecimal(hex);

  char temp[100];
  sprintf(temp, "%d", number);
  string str(temp);

 
  string bv = "bv";
  bv += str;
  bv+="[";
  bv+=width_char;
  bv+="]";


  //cout<<"\nInput Number in hex = "<<hex<<endl;
  //cout<<"\nBV in SMTLib format  = "<<bv<<endl;

  return bv;
}



string CDAGNode::hexToBinaryString(string hex)
{
char c;
int i=0;
char *y=strdup(hex.c_str()); 
string bv="";
string position;

while(y[i]!='\0')
	{
	c=y[i];
	position = getBinaryOfChar(c);
	bv=bv+position;
	i++;
	}

return bv;
}

string CDAGNode::convertLiteralToBitvectorWithProperWidth(string literal, int Width)
{

string bv="0b";

string str;

 if(literal == "'1'")
   str="1";
 else if(literal == "'0'")
   str="0";
 else
   {cout<<"\nstring CDAGNode::convertLiteralToBitvectorWithProperWidth(string literal, int Width) called with literal = "<<literal<<endl; exit(1);}
 
int NoOfZeros = Width - 1;

for(int i=0; i<NoOfZeros; i++)
   bv += "0";
 
bv += str;

//cout<<"\nBV in Yices format  = "<<bv<<endl;

return bv;
}


string CDAGNode::convertLiteralToBitvectorWithProperWidthInSMTLibFormat(string literal, int Width)
{
  

  // Width is the required width

  int msb = Width-1;
  int lsb = 0;

  char msb_char[100], lsb_char[100];
  
  sprintf(msb_char, "%d", msb);
  sprintf(lsb_char, "%d", lsb);

  string msb_string, lsb_string;

  msb_string = msb_char;
  lsb_string = lsb_char;
 
  string bv="(extract[";
  bv+=msb_string;
  bv+=":";
  bv+=lsb_string;
  bv+="] ";

  string str;

 if(literal == "'1'")
   str="bv1";
 else if(literal == "'0'")
   str="bv0";
 else
   {cout<<"\nstring CDAGNode::convertLiteralToBitvectorWithProperWidthInSMTLibFormat(string literal, int Width) called with literal = "<<literal<<endl; exit(1);}
 

 bv += str;
 bv+=")";

 cout<<"\nInput Number = "<<literal<<endl;
 cout<<"\nBV in SMTLib format  = "<<bv<<endl;

return bv;
}


string CDAGNode::convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(string literal, int Width)
{
  
 string str;

 if(literal == "'1'")
   str="bv1[1]";
 else if(literal == "'0'")
   str="bv0[1]";
 else
   {cout<<"\nstring CDAGNode::convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(string literal, int Width) called with literal = "<<literal<<endl; exit(1);}
 

 //cout<<"\nInput Number = "<<literal<<endl;
 //cout<<"\nBV in SMTLib format  = "<<str<<endl;

return str;
}



void CDAGNode::getConstantModifiedWithMinusSupported(ULINT &constant, int Sign, ULINT Mod)
{
 // Go down the +, - operator until constant is encountered.

  string dag_str = name;
  
  if(node_type && this->Get_DAGnode_outlist_size()==0) // constant
    
    {
      ULINT LocalConstant;
      LocalConstant = convertLiteralToNumber(dag_str); 
      if(Sign == 1) LocalConstant = additiveInverse(LocalConstant, Mod);
      constant = addWithModulo(constant, LocalConstant, Mod);
    }
  else if(dag_str == "+")
	{
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	   
          (*iout)->getConstantModifiedWithMinusSupported(constant, Sign, Mod);
	  iout++;
	  (*iout)->getConstantModifiedWithMinusSupported(constant, Sign, Mod);
	}

  else if(dag_str == "-")
     {
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	   
          (*iout)->getConstantModifiedWithMinusSupported(constant, Sign, Mod);
	  iout++;

	  int Opposite = (Sign==0)?1:0;
	  (*iout)->getConstantModifiedWithMinusSupported(constant, Opposite, Mod);
	}
}


void CDAGNode::getCoefficientWithMinusSupported(string Var, ULINT &Coefficient, int Sign, ULINT Mod)
{
	 string dag_str = name;

         if(dag_str == "*")
	{
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	   CDAGNode *coef = *iout;
	   iout++;
	   CDAGNode *var = *iout;
	   string variable = var->Get_DAGnode_data_as_id_delta();

	   //cout<<"\nvariable = "<<variable<<endl;

           if(variable == Var)
		{
		ULINT LocalCoef;
		coef->getConstantModified(LocalCoef);
		if(Sign == 1) LocalCoef = additiveInverse(LocalCoef, Mod);
		Coefficient = addWithModulo(Coefficient, LocalCoef, Mod);
		}
	}

	 else if(dag_str == "-")     
	{
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	  
	  (*iout)->getCoefficientWithMinusSupported(Var, Coefficient, Sign, Mod);
	  iout++;
	  
	  int Opposite = (Sign==0)?1:0;

	  (*iout)->getCoefficientWithMinusSupported(Var, Coefficient, Opposite, Mod);
	}
  
      else if(dag_str == "+")         
	{
	  list<CDAGNode*>::iterator iout = this->out_list.begin();
	  
          for(; iout!=this->out_list.end(); iout++)
		(*iout)->getCoefficientWithMinusSupported(Var, Coefficient, Sign, Mod);
	}
      else if(Get_DAGnode_outlist_size() == 0) // Leaf level
	   {
	     if(!node_type) // operand
	       {
		 string variable = this->Get_DAGnode_data_as_id_delta(); 
		 if(variable == Var)
		   {
		     ULINT LocalCoef = 1;
		     if(Sign == 1) LocalCoef = additiveInverse(LocalCoef, Mod);
		     Coefficient = addWithModulo(Coefficient, LocalCoef, Mod);
		   }
	       }
	   }
	 else
	   {
	     cout<<"\nUnknown node "<<dag_str<<" occured inside getCoefficientWithMinusSupported\n";
	     exit(1);
	   }
	 
}



// return Additive Inverse of the first input modulo the second input
ULINT CDAGNode::additiveInverse(ULINT number, ULINT mod)
{
if(number<0 || number>=mod) {cout<<"\nNumber "<<number<<" more than mod or less than zero inside function aInverse. Exiting...\n"; exit(1);}
 if(number==0) return 0; 
return mod-number;
}

// return a+b modulo mod
ULINT CDAGNode::addWithModulo(ULINT a, ULINT b, ULINT mod)
{
if(a<0 || a>=mod) {cout<<"\nNumber "<<a<<" more than mod or less than zero inside function add\n"; exit(1);} 
if(b<0 || b>=mod) {cout<<"\nNumber "<<b<<" more than mod or less than zero inside function add\n"; exit(1);} 
return (a+b)%mod;
}



// Modified version of getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable s.t., the output is directly written to file

void CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, FILE *yices_fp)
{
  string dag_str;

        if(!node_type)
        {
               dag_str = Get_DAGnode_value() + "_" + Get_DAGnode_delta_time_as_string();
               
	       //return dag_str;
	       fprintf(yices_fp, "%s", dag_str.c_str());
        }
        else
          {

            //cout<<"\nNAME=\t"<<name<<endl;    //debug code

            if(name == "select")
              {
                //dag_str = "(bv-extract ";
		fprintf(yices_fp, "(bv-extract ");

                list<CDAGNode*>::iterator iout = out_list.begin();

                iout++;

                iout++;

                //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("const", WidthTableForNodes, 1, yices_fp) + " ";
		(*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("const", WidthTableForNodes, 1, yices_fp);

		fprintf(yices_fp," ");

                iout = out_list.begin();

                iout++;
		
                //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("const", WidthTableForNodes, 1, yices_fp) + " ";
		(*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("const", WidthTableForNodes, 1, yices_fp);

		fprintf(yices_fp," ");

                iout = out_list.begin();

                //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);

		(*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);

                //dag_str += ")";

                //return dag_str;

		fprintf(yices_fp, ")");
		
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                //dag_str = "(not (= ";
		
		fprintf(yices_fp, "(not (= ");

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += "))";

		fprintf(yices_fp, "))");

                //return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		//dag_str = "(= ";

		fprintf(yices_fp, "(= ");

		list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";
		    
		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");
	
                //return dag_str;
              }

	    else if(name == "concat")
	      {

		cout<<"\nconcat is not supported by function CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		exit(1);

		/*
		dag_str = "(bv-concat ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes) + " ";
                  }

                dag_str += ")";

                return dag_str; */
              }

	    else if(name == "*")
	      {
		//dag_str = "(bv-mul ";
		fprintf(yices_fp, "(bv-mul ");

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;
              }

	     else if(name == "+")
	      {
		//dag_str = "(bv-add ";
		fprintf(yices_fp, "(bv-add ");

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;
              }


	    // Newly added on 20.1.2010

	     else if(name == "-")
	      {
		//dag_str = "(bv-sub ";
		fprintf(yices_fp, "(bv-sub ");

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;
              }

	    // Newly added on 20.1.2010 ends here
	    

	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "ite"))
	      {
		//dag_str = "(" +  name + " ";
		
		fprintf(yices_fp, "(%s ", name.c_str());

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bool", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";
		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile("bool", WidthTableForNodes, WidthTableForNodes[this], yices_fp);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;

              }

	    else if(name == "'1'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];
		

                //return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

		fprintf(yices_fp,"%s", convertLiteralToBitvectorWithProperWidth(name, Width_Required).c_str());
		
		//dag_str = "0b1";

		//return dag_str;
              }

	    else if(name == "'0'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];


                //return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

		fprintf(yices_fp,"%s",convertLiteralToBitvectorWithProperWidth(name, Width_Required).c_str());

		//dag_str = "0b0";

		//return dag_str;
              }

	    else if(Get_DAGnode_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
			//dag_str = name;
		      //cout<<"\ndag_str = "<<dag_str<<endl; 
		      //return dag_str;

			fprintf(yices_fp,"%s",name.c_str());
		      }
		else if(context == "bv")
		      {
			//cout<<"\nname = "<<name<<endl; 

			// following line added on 19.1.2010

			//int Width_Required = WidthTableForNodes[this];
			
			if(name[0] == 'x') // hex
			  {
			    //dag_str = convertHexToBitvector(name);
			    //dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);

			    fprintf(yices_fp,"%s", convertHexToBitvectorWithProperWidth(name, Width_Required).c_str());
			  }
			else // Decimal
			  {

			    // Following if else if commented on 18 Jan 2010 

			    //if(name == "1") dag_str="0b1";
			    //else if(name == "0") dag_str="0b0";
			    // else 
			    //  {

			    //dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);

			    fprintf(yices_fp,"%s", convertDecimalToBitvectorWithProperWidth(name, Width_Required).c_str());
				

				//  }
			  }
			  
			//cout<<"\ndag_str = "<<dag_str<<endl; 		
			//return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") fprintf(yices_fp,"true"); //dag_str = "true";
			else if(name == "0") fprintf(yices_fp,"false"); //dag_str = "false";
			else if(name == "true") fprintf(yices_fp,"true"); //dag_str = "true";
			else if(name == "false") fprintf(yices_fp,"false"); //dag_str = "false";
			//else dag_str = name;

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
				//dag_str = convertHexToBitvector(name);
				//dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);

				fprintf(yices_fp,"%s", convertHexToBitvectorWithProperWidth(name, Width_Required).c_str());
			      }
			    else // Decimal
			      {
				//dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);

				fprintf(yices_fp,"%s", convertDecimalToBitvectorWithProperWidth(name, Width_Required).c_str());
			      }
			  }

			
			//return dag_str;
		      }
		else
		      {
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile\n";
		exit(1);
	      }
		    
	  }
}





// Modified version of getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFile s.t., the SE is printed s.t. the variables are printed with their delta times as delta time = delta time + shift

void CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, FILE *yices_fp, int shift)
{
  string dag_str;

        if(!node_type)
        {

	  int new_delta_time = delta_time + shift;
	  char new_delta_time_char[200];
	  sprintf(new_delta_time_char, "%d", new_delta_time);
	  string new_delta_time_str(new_delta_time_char);
	  
	  
	  dag_str = Get_DAGnode_value() + "_" + new_delta_time_str;
               
	  //return dag_str;
	  fprintf(yices_fp, "%s", dag_str.c_str());
        }
        else
          {

            //cout<<"\nNAME=\t"<<name<<endl;    //debug code

            if(name == "select")
              {
                //dag_str = "(bv-extract ";
		fprintf(yices_fp, "(bv-extract ");

                list<CDAGNode*>::iterator iout = out_list.begin();

                iout++;

                iout++;

                //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("const", WidthTableForNodes, 1, yices_fp) + " ";
		(*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("const", WidthTableForNodes, 1, yices_fp, shift);

		fprintf(yices_fp," ");

                iout = out_list.begin();

                iout++;
		
                //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("const", WidthTableForNodes, 1, yices_fp) + " ";
		(*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("const", WidthTableForNodes, 1, yices_fp, shift);

		fprintf(yices_fp," ");

                iout = out_list.begin();

                //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp);

		(*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp, shift);

                //dag_str += ")";

                //return dag_str;

		fprintf(yices_fp, ")");
		
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                //dag_str = "(not (= ";
		
		fprintf(yices_fp, "(not (= ");

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp, shift);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += "))";

		fprintf(yices_fp, "))");

                //return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		//dag_str = "(= ";

		fprintf(yices_fp, "(= ");

		list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";
		    
		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp, shift);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");
	
                //return dag_str;
              }

	    else if(name == "concat")
	      {

		cout<<"\nconcat is not supported by function CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		exit(1);

		/*
		dag_str = "(bv-concat ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes) + " ";
                  }

                dag_str += ")";

                return dag_str; */
              }

	    else if(name == "*")
	      {
		//dag_str = "(bv-mul ";
		fprintf(yices_fp, "(bv-mul ");

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp, shift);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;
              }

	     else if(name == "+")
	      {
		//dag_str = "(bv-add ";
		fprintf(yices_fp, "(bv-add ");

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp, shift);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;
              }


	    // Newly added on 20.1.2010

	     else if(name == "-")
	      {
		//dag_str = "(bv-sub ";
		fprintf(yices_fp, "(bv-sub ");

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";

		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bv", WidthTableForNodes, WidthTableForNodes[this], yices_fp, shift);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;
              }

	    // Newly added on 20.1.2010 ends here
	    

	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "ite"))
	      {

		if(context == "bv")
		  {
		    if(name == "and")
		      {
			cout<<"\nand renamed as bv-and\n";
			dag_str = "bv-and";
		      }
		    else if(name == "or")
		      {
			cout<<"\nor renamed as bv-or\n";
			dag_str = "bv-or";
		      }
		    else if(name == "not")
		      {
			cout<<"\nnot renamed as bv-not\n";
			dag_str = "bv-not";
		      }
		    else
		      {
			dag_str = name;
		      }
		  }
		else
		  {
		    dag_str = name;
		  }
		    
		
		fprintf(yices_fp, "(%s ", dag_str.c_str());

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    //dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bool", WidthTableForNodes, WidthTableForNodes[this], yices_fp) + " ";
		    (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift("bool", WidthTableForNodes, WidthTableForNodes[this], yices_fp, shift);
		    fprintf(yices_fp, " ");
                  }

                //dag_str += ")";
		fprintf(yices_fp, ")");

                //return dag_str;

              }

	    else if(name == "'1'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];
		

                //return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

		fprintf(yices_fp,"%s", convertLiteralToBitvectorWithProperWidth(name, Width_Required).c_str());
		
		//dag_str = "0b1";

		//return dag_str;
              }

	    else if(name == "'0'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];


                //return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

		fprintf(yices_fp,"%s",convertLiteralToBitvectorWithProperWidth(name, Width_Required).c_str());

		//dag_str = "0b0";

		//return dag_str;
              }

	    else if(Get_DAGnode_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
			//dag_str = name;
		      //cout<<"\ndag_str = "<<dag_str<<endl; 
		      //return dag_str;

			fprintf(yices_fp,"%s",name.c_str());
		      }
		else if(context == "bv")
		      {
			//cout<<"\nname = "<<name<<endl; 

			// following line added on 19.1.2010

			//int Width_Required = WidthTableForNodes[this];
			
			if(name[0] == 'x') // hex
			  {
			    //dag_str = convertHexToBitvector(name);
			    //dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);

			    fprintf(yices_fp,"%s", convertHexToBitvectorWithProperWidth(name, Width_Required).c_str());
			  }
			else // Decimal
			  {

			    // Following if else if commented on 18 Jan 2010 

			    //if(name == "1") dag_str="0b1";
			    //else if(name == "0") dag_str="0b0";
			    // else 
			    //  {

			    //dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);

			    fprintf(yices_fp,"%s", convertDecimalToBitvectorWithProperWidth(name, Width_Required).c_str());
				

				//  }
			  }
			  
			//cout<<"\ndag_str = "<<dag_str<<endl; 		
			//return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") fprintf(yices_fp,"true"); //dag_str = "true";
			else if(name == "0") fprintf(yices_fp,"false"); //dag_str = "false";
			else if(name == "true") fprintf(yices_fp,"true"); //dag_str = "true";
			else if(name == "false") fprintf(yices_fp,"false"); //dag_str = "false";
			//else dag_str = name;

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
				//dag_str = convertHexToBitvector(name);
				//dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);

				fprintf(yices_fp,"%s", convertHexToBitvectorWithProperWidth(name, Width_Required).c_str());
			      }
			    else // Decimal
			      {
				//dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);

				fprintf(yices_fp,"%s", convertDecimalToBitvectorWithProperWidth(name, Width_Required).c_str());
			      }
			  }

			
			//return dag_str;
		      }
		else
		      {
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableToFileWithShift\n";
		exit(1);
	      }
		    
	  }
}




// The below function prints a dag in proper propositional skelton format in old SMTLib format which is understood by CVC3

string CDAGNode :: getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, set<string> &VarsToElimSet)
{
        string dag_str;

        if(!node_type)
        {
	       string id_delta = Get_DAGnode_value() + "_" + Get_DAGnode_delta_time_as_string();
	       
	       //cout<<"\nid_delta = "<<id_delta<<endl;

	       if(VarsToElimSet.find(id_delta) != VarsToElimSet.end())
		 {
		   //cout<<"\nmatched\n";
		   dag_str="?";
		   dag_str+=id_delta;
		   //cout<<"\ndag_str = "<<dag_str<<endl;
		 }
	       else
		 {
		   dag_str=id_delta;
		 }
               return dag_str;
        }
        else
          {

            //cout<<"\nNAME=\t"<<name<<endl;    //debug code

            if(name == "select")
              {
                dag_str = "(extract[";

                list<CDAGNode*>::iterator iout = out_list.begin();

                iout++;

                iout++;

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("const", WidthTableForNodes, 1, VarsToElimSet) + " ";

                iout = out_list.begin();

                iout++;

		dag_str += ":";

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("const", WidthTableForNodes, 1, VarsToElimSet) + " ";

		dag_str += "] ";

                iout = out_list.begin();

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {

		cout<<"\nconcat is not supported by function CDAGNode :: getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		exit(1);

		/*
		dag_str = "(bv-concat ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bv", WidthTableForNodes) + " ";
                  }

                dag_str += ")";

                return dag_str; */
              }

	    else if(name == "*")
	      {
		dag_str = "(bvmul ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "+")
	      {
		dag_str = "(bvadd ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // Newly added on 20.1.2010

	     else if(name == "-")
	      {
		dag_str = "(bvsub ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    // Newly added on 20.1.2010 ends here
	    

	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not"))
	      {
		dag_str = "(" +  name + " ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "ite")
	      {
		dag_str = "(" +  name + " ";

                list<CDAGNode*>::iterator iout = out_list.begin();

		string cond_string = (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet);
		iout++;
		
		string then_string = (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet);

		iout++;
		
		string else_string = (*iout)->getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this], VarsToElimSet);
		
		
                dag_str = "(and (implies ";

		dag_str+= cond_string;

		dag_str+= " ";
		
		dag_str+= then_string;

		dag_str+= ")";



		dag_str += "(implies ";

		dag_str+= "(not ";
		
		dag_str+= cond_string;

		dag_str+= ")";
		
		dag_str+= " ";
		
		dag_str+= else_string;

		dag_str+= "))";

                return dag_str;

              }

	    else if(name == "'1'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];
		

                return convertLiteralToBitvectorWithProperWidthInSMTLibFormat(name, Width_Required);
		
		//dag_str = "0b1";

		//return dag_str;
              }

	    else if(name == "'0'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];


                return convertLiteralToBitvectorWithProperWidthInSMTLibFormat(name, Width_Required);

		//dag_str = "0b0";

		//return dag_str;
              }

	    else if(Get_DAGnode_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      //cout<<"\ndag_str = "<<dag_str<<endl; 
		      return dag_str;
		      }
		else if(context == "bv")
		      {
			//cout<<"\nname = "<<name<<endl; 

			// following line added on 19.1.2010

			//int Width_Required = WidthTableForNodes[this];
			
			if(name[0] == 'x') // hex
			  {
			    //dag_str = convertHexToBitvector(name);
			  dag_str = convertHexToBitvectorWithProperWidthInSMTLibFormat(name, Width_Required);
			  }
			else // Decimal
			  {

			    // Following if else if commented on 18 Jan 2010 

			    //if(name == "1") dag_str="0b1";
			    //else if(name == "0") dag_str="0b0";
			    // else 
			    //  {
				dag_str = convertDecimalToBitvectorWithProperWidthInSMTLibFormat(name, Width_Required);
				//  }
			  }
			  
			//cout<<"\ndag_str = "<<dag_str<<endl; 		
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";
			//else dag_str = name;

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
				//dag_str = convertHexToBitvector(name);
				dag_str = convertHexToBitvectorWithProperWidthInSMTLibFormat(name, Width_Required);
			      }
			    else // Decimal
			      {
				dag_str = convertDecimalToBitvectorWithProperWidthInSMTLibFormat(name, Width_Required);
			      }
			  }

			
			return dag_str;
		      }
		else
		      {
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable\n";
		exit(1);
	      }
		    
	  }
}


// The below function prints a dag in proper propositional skelton format in latest SMTLib format which is understood by our SMTLib file to dag parser

string CDAGNode :: getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required)
{
        string dag_str;

        if(!node_type)
        {
	       dag_str = Get_DAGnode_value() + "_" + Get_DAGnode_delta_time_as_string();
	       return dag_str;
        }
        else
          {

            //cout<<"\nNAME=\t"<<name<<endl;    //debug code

            if(name == "select")
              {
                dag_str = "(extract[";

                list<CDAGNode*>::iterator iout = out_list.begin();

                iout++;

                iout++;

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("const", WidthTableForNodes, 1) + " ";

                iout = out_list.begin();

                iout++;

		dag_str += ":";

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("const", WidthTableForNodes, 1) + " ";

		dag_str += "] ";

                iout = out_list.begin();

                dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {

		cout<<"\nconcat is not supported by function CDAGNode :: getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		exit(1);

		/*
		dag_str = "(bv-concat ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bv", WidthTableForNodes) + " ";
                  }

                dag_str += ")";

                return dag_str; */
              }

	    else if(name == "*")
	      {
		dag_str = "(bvmul ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "+")
	      {
		dag_str = "(bvadd ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // Newly added on 20.1.2010

	     else if(name == "-")
	      {
		dag_str = "(bvsub ";

		 list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bv", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    // Newly added on 20.1.2010 ends here
	    

	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not"))
	      {
		dag_str = "(" +  name + " ";

                list<CDAGNode*>::iterator iout = out_list.begin();

                for(; iout != out_list.end(); iout++)
                  {
                    dag_str += (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this]) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "ite")
	      {
		dag_str = "(if_then_else ";

                list<CDAGNode*>::iterator iout = out_list.begin();

		string cond_string = (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this]);
		iout++;
		
		string then_string = (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this]);

		iout++;
		
		string else_string = (*iout)->getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable("bool", WidthTableForNodes, WidthTableForNodes[this]);
		
		
		dag_str+= cond_string;

		dag_str+= " ";
		
		dag_str+= then_string;

		dag_str+= " ";
		
		dag_str+= else_string;

		dag_str+= ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];
		

                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);
		
		//dag_str = "0b1";

		//return dag_str;
              }

	    else if(name == "'0'")
              {
		// following line added on 19.1.2010

		//int Width_Required = WidthTableForNodes[this];


                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);

		//dag_str = "0b0";

		//return dag_str;
              }

	    else if(Get_DAGnode_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      //cout<<"\ndag_str = "<<dag_str<<endl; 
		      return dag_str;
		      }
		else if(context == "bv")
		      {
			//cout<<"\nname = "<<name<<endl; 

			// following line added on 19.1.2010

			//int Width_Required = WidthTableForNodes[this];
			
			if(name[0] == 'x') // hex
			  {
			    //dag_str = convertHexToBitvector(name);
			  dag_str = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			else // Decimal
			  {

			    // Following if else if commented on 18 Jan 2010 

			    //if(name == "1") dag_str="0b1";
			    //else if(name == "0") dag_str="0b0";
			    // else 
			    //  {
				dag_str = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
				//  }
			  }
			  
			//cout<<"\ndag_str = "<<dag_str<<endl; 		
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";
			//else dag_str = name;

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
				//dag_str = convertHexToBitvector(name);
				dag_str = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			      }
			    else // Decimal
			      {
				dag_str = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			      }
			  }

			
			return dag_str;
		      }
		else
		      {
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable\n";
		exit(1);
	      }
		    
	  }
}





bool CDAGNode :: evaluateTrivialCondition()
{
  //cout<<"\nthis = "<<this->getDAGStr()<<endl;
  string dag_str = name;
  //cout<<"\ndag_str = "<<dag_str<<endl;
  if(dag_str == "and")
    {
      //cout<<"\nInside and\n";
      list<CDAGNode*>::iterator iout = out_list.begin();
      bool child1 =  (*iout)->evaluateTrivialCondition();
      iout++;
      bool child2 =  (*iout)->evaluateTrivialCondition();
      if(!child1 || !child2) return false;
      return true;
    }
  else if(dag_str == "or")
    {
      //cout<<"\nInside and\n";
      list<CDAGNode*>::iterator iout = out_list.begin();
      bool child1 =  (*iout)->evaluateTrivialCondition();
      iout++;
      bool child2 =  (*iout)->evaluateTrivialCondition();
      if(!child1 && !child2) return false;
      return true;
    }
  else if(dag_str == "is_not_equal")
    {
      //cout<<"\nInside is_not_equal\n"; 
     list<CDAGNode*>::iterator iout = out_list.begin();
     
     CDAGNode *child1 = *iout;
     //cout<<"\nchild1 = "<<child1->getDAGStr()<<endl;
     if(child1->Get_DAGnode_outlist_size()!=0 || (child1->Get_DAG_node_type()==true)) // not a variable
       return true;
     string child1_name = child1->Get_DAGnode_value();
     child1_name += child1->Get_DAGnode_delta_time_as_string();
     
     iout++;
     
     CDAGNode *child2 = *iout;
     //cout<<"\nchild2 = "<<child2->getDAGStr()<<endl;
     if(child2->Get_DAGnode_outlist_size()!=0 || (child2->Get_DAG_node_type()==true)) // not a variable
       return true;
     string child2_name = child2->Get_DAGnode_value();
     child2_name += child2->Get_DAGnode_delta_time_as_string();

     if(child1_name == child2_name) return false;
     return true;
    }
  else
    {
      //cout<<"\nSome other node\n";
      return true;
    }
}


bool CDAGNode :: evaluateClockCondition()
{
  //cout<<"\nthis = "<<this->getDAGStr()<<endl;
  string dag_str = name;
  //cout<<"\ndag_str = "<<dag_str<<endl;
  if(dag_str == "and")
    {
      //cout<<"\nInside and\n";
      list<CDAGNode*>::iterator iout = out_list.begin();
      bool child1 =  (*iout)->evaluateClockCondition();
      iout++;
      bool child2 =  (*iout)->evaluateClockCondition();
      if(child1 && child2) return true;
      return false;
    }
  else if(dag_str == "or")
    {
      //cout<<"\nInside and\n";
      list<CDAGNode*>::iterator iout = out_list.begin();
      bool child1 =  (*iout)->evaluateClockCondition();
      iout++;
      bool child2 =  (*iout)->evaluateClockCondition();
      if(child1 || child2) return true;
      return false;
    }
  else if(dag_str == "is_not_equal")
    {
      //cout<<"\nInside is_not_equal\n"; 
     list<CDAGNode*>::iterator iout = out_list.begin();
     
     CDAGNode *child1 = *iout;
     //cout<<"\nchild1 = "<<child1->getDAGStr()<<endl;
     if(child1->Get_DAGnode_outlist_size()!=0 || (child1->Get_DAG_node_type()==true)) // not a variable
       return false;
     string child1_name = child1->Get_DAGnode_value();
     child1_name += child1->Get_DAGnode_delta_time_as_string();
     
     iout++;
     
     CDAGNode *child2 = *iout;
     //cout<<"\nchild2 = "<<child2->getDAGStr()<<endl;
     if(child2->Get_DAGnode_outlist_size()!=0 || (child2->Get_DAG_node_type()==true)) // not a variable
       return false;

     string child2_name = child2->Get_DAGnode_value();
     child2_name += child2->Get_DAGnode_delta_time_as_string();

     //cout<<"\nchild1_name = "<<child1_name<<"\tchild2_name = "<<child2_name<<endl;
     if(child1_name == "clk1" && child2_name == "clk0") 
       {
	 //cout<<"\nReturn true\n";
	 return true;
       }
     else 
       {
	 //cout<<"\nReturn False\n";
	 return false;
       }

    }
   else if(dag_str == "equivalent")
    {
      //cout<<"\nInside equivalent\n"; 
     list<CDAGNode*>::iterator iout = out_list.begin();
     
     CDAGNode *child1 = *iout;
     //cout<<"\nchild1 = "<<child1->getDAGStr()<<endl;
     if(child1->Get_DAGnode_outlist_size()!=0 || (child1->Get_DAG_node_type()==true)) // not a variable
       return false;
     string child1_name = child1->Get_DAGnode_value();
     child1_name += child1->Get_DAGnode_delta_time_as_string();
     
     iout++;
     
     CDAGNode *child2 = *iout;
     //cout<<"\nchild2 = "<<child2->getDAGStr()<<endl;

     string child2_name = child2->Get_DAGnode_value();
     //cout<<"\nchild1_name = "<<child1_name<<"\tchild2_name = "<<child2_name<<endl;
     if(child1_name == "clk1" && child2_name == "'1'")
       {
	 //cout<<"\nReturn true\n";
	 return true;
       }
     else
       {
	 //cout<<"\nReturn false\n";
	 return false;
       }

     }
  else
    {
      //cout<<"\nSome other node\n";
      return false;
    }
}
     

void CDAGNode :: getDAGNamesWithCheck(set<string> &names)
{
    string dag_str = name;

    if(dag_str == "ite")
    {
        list<CDAGNode*>::iterator iout = out_list.begin();
	
	bool CondnFalse = (*iout)->evaluateTrivialCondition();
	bool CondnTrue = (*iout)->evaluateClockCondition();
	
	if(!CondnFalse) // condition part is trivially false : do not go to then part
	  {
	    cout<<"\nCondition Part is false\n";
	    (*iout)->getDAGNamesWithCheck(names);
	    iout++;
	    iout++;
	    (*iout)->getDAGNamesWithCheck(names);
	  }
	else if(CondnTrue) // condition part is trivially true : do not go to else part
	  {
	    cout<<"\nCondition Part is true\n";
	    (*iout)->getDAGNamesWithCheck(names);
	    iout++;
	    (*iout)->getDAGNamesWithCheck(names);
	  }
	else // can't say
	  {
	    cout<<"\nCondition Part is can't say\n";
	   (*iout)->getDAGNamesWithCheck(names);
	    iout++;
	    (*iout)->getDAGNamesWithCheck(names);
	    iout++;
	    (*iout)->getDAGNamesWithCheck(names);
	  }

    }
    else
    {
        if(node_type == false)  //if the dag value is variable.....
        {
            char temp[10];

            sprintf(temp, "%d", delta_time);

            dag_str += "_";

            dag_str += temp;

            names.insert(dag_str);
        }

        list<CDAGNode*>::iterator iout = out_list.begin();

        for(; iout != out_list.end(); iout++)
        {
            (*iout)->getDAGNamesWithCheck(names);
        }
    }

}



void CDAGNode :: getDAGNamesOfDAG_WithNoExists_WithDP(set<string> &names, set<CDAGNode*> *PtrDPTable)
{
  set<CDAGNode*>::iterator DPTable_it;

  DPTable_it = PtrDPTable->find(this);

  if(DPTable_it != (*PtrDPTable).end()) // DPTable hit
    {
      return;
    }


  string dag_str = name;

  if(node_type == false)  //if the dag value is variable.....
    {
      char temp[10];

      sprintf(temp, "%d", delta_time);

      //char *t = strdup(dag_str.c_str());

      dag_str += "_";

      dag_str += temp;

      names.insert(dag_str);
    }

  list<CDAGNode*>::iterator iout = out_list.begin();

  for(; iout != out_list.end(); iout++)
    {
      (*iout)->getDAGNamesOfDAG_WithNoExists_WithDP(names, PtrDPTable);
    }

  PtrDPTable->insert(this);
 
}




// getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with memoization added

string CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(string context, map<CDAGNode*, int> &WidthTableForNodes, int Width_Required, map<CDAGNode*, string> &DPTable)
{
  map<CDAGNode*, string>::iterator DPTable_it = DPTable.find(this);
  if(DPTable_it != DPTable.end())
    {
    cout<<"\nCache Hit\n";
    return DPTable_it->second;
    }
  
  
  

  string dag_str;

  if(!node_type)
    {
      dag_str = Get_DAGnode_value() + "_" + Get_DAGnode_delta_time_as_string();
    }
  else
    {

      //cout<<"\nNAME=\t"<<name<<endl;    //debug code

      if(name == "select")
	{
	  dag_str = "(bv-extract ";

	  list<CDAGNode*>::iterator iout = out_list.begin();

	  iout++;

	  iout++;

	  dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("const", WidthTableForNodes, 1, DPTable) + " ";

	  iout = out_list.begin();

	  iout++;

	  dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("const", WidthTableForNodes, 1, DPTable) + " ";

	  iout = out_list.begin();

	  dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes, WidthTableForNodes[this], DPTable);

	  dag_str += ")";

	}
      // NB: Assumption here is that not_equal_to applies only between bit-vectors

      else if(name == "is_not_equal" || name == "not_equal_to")
	{
	  dag_str = "(not (= ";

	  list<CDAGNode*>::iterator iout = out_list.begin();

	  for(; iout != out_list.end(); iout++)
	    {
	      dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes, WidthTableForNodes[this], DPTable) + " ";
	    }

	  dag_str += "))";
	}

      // NB: Assumption here is that equivalent applies only between bit-vectors

      else if(name == "equivalent" || name == "=")
	{
	  dag_str = "(= ";

	  list<CDAGNode*>::iterator iout = out_list.begin();

	  for(; iout != out_list.end(); iout++)
	    {
	      dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes, WidthTableForNodes[this], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      else if(name == "concat")
	{

	  cout<<"\nconcat is not supported by function CDAGNode :: getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
	  exit(1);

	  /*
	    dag_str = "(bv-concat ";

	    list<CDAGNode*>::iterator iout = out_list.begin();

	    for(; iout != out_list.end(); iout++)
	    {
	    dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes) + " ";
	    }

	    dag_str += ")";

	    return dag_str; */
	}

      else if(name == "*")
	{
	  dag_str = "(bv-mul ";

	  list<CDAGNode*>::iterator iout = out_list.begin();

	  for(; iout != out_list.end(); iout++)
	    {
	      dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes, WidthTableForNodes[this], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      else if(name == "+")
	{
	  dag_str = "(bv-add ";

	  list<CDAGNode*>::iterator iout = out_list.begin();

	  for(; iout != out_list.end(); iout++)
	    {
	      dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes, WidthTableForNodes[this], DPTable) + " ";
	    }

	  dag_str += ")";

	}


      // Newly added on 20.1.2010

      else if(name == "-")
	{
	  dag_str = "(bv-sub ";

	  list<CDAGNode*>::iterator iout = out_list.begin();

	  for(; iout != out_list.end(); iout++)
	    {
	      dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bv", WidthTableForNodes, WidthTableForNodes[this], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      // Newly added on 20.1.2010 ends here
	    

      // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

      else if(name == "and" || (name == "or") || (name == "not") || (name == "ite"))
	{
	  dag_str = "(" +  name + " ";

	  list<CDAGNode*>::iterator iout = out_list.begin();

	  for(; iout != out_list.end(); iout++)
	    {
	      dag_str += (*iout)->getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP("bool", WidthTableForNodes, WidthTableForNodes[this], DPTable) + " ";
	    }

	  dag_str += ")";

	}

      else if(name == "'1'")
	{
	  // following line added on 19.1.2010

	  //int Width_Required = WidthTableForNodes[this];
		

	  dag_str = convertLiteralToBitvectorWithProperWidth(name, Width_Required);
		
	  //dag_str = "0b1";

	  //return dag_str;
	}

      else if(name == "'0'")
	{
	  // following line added on 19.1.2010

	  //int Width_Required = WidthTableForNodes[this];


	  dag_str = convertLiteralToBitvectorWithProperWidth(name, Width_Required);

	  //dag_str = "0b0";

	  //return dag_str;
	}

      else if(Get_DAGnode_outlist_size() == 0) // Other constants

	{
		
	  if(context == "const")
	    {
	      dag_str = name;
	      //cout<<"\ndag_str = "<<dag_str<<endl; 
	    }
	  else if(context == "bv")
	    {
	      //cout<<"\nname = "<<name<<endl; 

	      // following line added on 19.1.2010

	      //int Width_Required = WidthTableForNodes[this];
			
	      if(name[0] == 'x') // hex
		{
		  //dag_str = convertHexToBitvector(name);
		  dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);
		}
	      else // Decimal
		{

		  // Following if else if commented on 18 Jan 2010 

		  //if(name == "1") dag_str="0b1";
		  //else if(name == "0") dag_str="0b0";
		  // else 
		  //  {
		  dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);
		  //  }
		}
			  
	      //cout<<"\ndag_str = "<<dag_str<<endl; 		
	    }
	  else if(context == "bool")
	    {
	      if(name == "1") dag_str = "true";
	      else if(name == "0") dag_str = "false";
	      else if(name == "true") dag_str = "true";
	      else if(name == "false") dag_str = "false";
	      //else dag_str = name;

	      else // interpreting a Boolean constant other than true/false as a bit-vector
		{
		  if(name[0] == 'x') // hex
		    {
		      //dag_str = convertHexToBitvector(name);
		      dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);
		    }
		  else // Decimal
		    {
		      dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);
		    }
		}

	    }
	  else
	    {
	      cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP\n";
	      exit(1);
	    }
		  
	}
      else
	{
	  cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP\n";
	  exit(1);
	}
		    
    }

  DPTable.insert(make_pair(this, dag_str));
  return dag_str;
}



// ADDED by Ajith On 11-11-2010 for allowing printing in SMTLib format	



string CDAGNode::convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(string literal, int width)
{

  char width_char[100];  
  sprintf(width_char, "%d", width);
  string width_string;
  width_string = width_char;
  
 string str;

 if(literal == "'1'")
   {
   str="bv1[";
   str+=width_string;
   str+="]";
   }
 
 else if(literal == "'0'")
   {
   str="bv0[";
   str+=width_string;
   str+="]";
   }

 else
   {cout<<"\nstring CDAGNode::convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(string literal, int Width) called with literal = "<<literal<<endl; exit(1);}
 

 //cout<<"\nInput Number = "<<literal<<endl;
 //cout<<"\nBV in SMTLib format  = "<<str<<endl;

return str;
}


// ADDED by Ajith On 11-11-2010 for allowing printing in SMTLib format	
