#include "stdlib.h"
#include "helping_functions.h"
using namespace std;


/**
   Synopsis: [A helper that allows translation of strings representing integer values to numeric integers]
   Description: [This function uses C's atoi for conversion. Returns 0 for empty string]
   SideEffects:[]
   SeeAlso:[]
   Arguments:[Valid string representing integer like "1234"]
   PostCondition: [Ensures correct conversion of valid input]
*/
int stringToInteger(string s)
{
  //returns 0 if the string is empty
  //Put a check to see whether input string is valid, i.e. does not contain alphabet chars
  for(int i=0;i<s.size();i++)
    {
      if(isalpha(s[i]))
        {
	  cerr<<"Invalid string in stringToInteger:"<<s<<endl;
          exit(-1);
        }
    }
  return atoi(s.c_str());
}




/**
   Synopsis: [A helper that allows translation of binary strings to integers]
   Description: [The function reads every character in binary string (LSB on right) and depending on its position 'i' and value 'v', computes v*(2^i)]
   SideEffects:[]
   SeeAlso:[]
   Arguments:[Valid binary string]
   PostCondition: [Ensures correct conversion of valid input, exits on invalid string]
*/


long int binaryStringToInteger(string str)
{
  int length=str.size();
  long int num=0;
  long int base=1;
  for(int i=length-1;i>=0;i--)
    {
	if((str[i]!='1') && (str[i]!='0'))
	{
	cerr<<"Unable to convert binary string to integer: "<<str<<endl;
	exit(-1);
	}
      if(str[i]=='1')
        {
          num=num+base;
        }
      base=base*2;
    }
  return num;


}











/**
   Synopsis: [A helper that allows translation of special integer strings possible in verilog to integers]
   Description: [The function reads every character in decimal string and depending on its position 'i' and value 'v', computes v*(10^i)]
   SideEffects:[]
   SeeAlso:[]
   Arguments:[Valid integer string]
   PostCondition: [Ensures correct conversion of valid input, exits on invalid string]
*/
int convertToInteger(string str)
{
  int ret_val = 0;

  if(str.empty())
    {
      cout << "ERROR!!! Attempting to convert an empty string to integer in helping_functions.cc" << endl;
      ret_val = -1;
    }
  else
    for(int i=0;i<=str.length()-1;i++)
      {
        if(str[i]=='-')
          continue;
	if(isalpha(str[i]))
	  {
	   cerr<<"Unable to convert a string containing non-digit character to integer: "<<str<<endl;
	   exit(-1);
	  }
        ret_val = ret_val*10 + convertToDigit(str[i]);
      }
  return ret_val;
}

/**
   Synopsis: [A helper that allows translation of decimal character to integers]
   Description: [The function reads a decimal character ('0' to '9') and returns its decimal value]
   SideEffects:[]
   SeeAlso:[]
   Arguments:[Valid decimal character]
   PostCondition: [Ensures correct conversion of valid input, exits on invalid input]
*/

int convertToDigit(char ch)
{
  int ret_val;
  switch(ch)
    {
    case '0' : {
      ret_val = 0;
      break;
    }
    case '1' : {
      ret_val = 1;
      break;
    }
    case '2' : {
      ret_val = 2;
      break;
    }
    case '3' : {
      ret_val = 3;
      break;
    }
    case '4' : {
      ret_val = 4;
      break;
    }
    case '5' : {
      ret_val = 5;
      break;
    }
    case '6' : {
      ret_val = 6;
      break;
    }
    case '7' : {
      ret_val = 7;
      break;
    }
    case '8' : {
      ret_val = 8;
      break;
    }
    case '9' : {
      ret_val = 9;
      break;
    }
    default : {
      cerr << "ERROR!!! Non-digit input given to convertToDigit:  " << ch<<endl;
      ret_val = 0; // Kept as a safety. To be removed. This case should not arise at all.
    }
    }
  return ret_val;
}

/**
   Synopsis: [A helper that returns binary string of an integer]
   Description: [Implements general algorithm for converting a integer to binary]
   SideEffects:[]
   SeeAlso:[]
   Arguments:[Integer >=0]
   PostCondition: [Ensures correct conversion of valid input, exits on invalid input]
*/

/*string integerToBinaryString(unsigned int i)
{
  if(i<0)
	cerr<<"ERROR!! Cannot convert "<<i<<" to binary "<<endl;
  if(i==0)
    return "0";
  string bin_str="";
  int j=i;
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
*/
string hexStringToBinaryString(string str)
{
  string bin="";
  while(str!="")
    {
      string hex = str.substr(str.length()-1,1);
      if(hex == "0")
        bin = "0000" + bin;
      else if(hex == "1")
        bin = "0001" + bin;
      else if(hex == "2")
        bin = "0010" + bin;
      else if(hex == "3")
        bin = "0011" + bin;
      else if(hex == "4")
        bin = "0100" + bin;
      else if(hex == "5")
        bin = "0101" + bin;
      else if(hex == "6")
        bin = "0110" + bin;
      else if(hex == "7")
        bin = "0111" + bin;
      else if(hex == "8")
        bin = "1000" + bin;
      else if(hex == "9")
        bin = "1001" + bin;
      else if((hex == "a") || (hex == "A"))
        bin = "1010" + bin;
      else if((hex == "b") || (hex == "B"))
        bin = "1011" + bin;
      else if((hex == "c") || (hex == "C"))
        bin = "1100" + bin;
      else if((hex == "d") || (hex == "D"))
        bin = "1101" + bin;
      else if((hex == "e") || (hex == "E"))
        bin = "1110" + bin;
      else if((hex == "f") || (hex == "F"))
        bin = "1111" + bin;
      else
        cout << "ERROR!!! Input not in hex format...in hexStringToBinaryString" << endl;

      str = str.substr(0,str.length()-1);
    }
  return bin;
}


string integerToString(int i)   //converts 123 to "123"
{
  if(i==0)
    return "0";

  string ret_val = "";
  int j=i;
  while(j!=0)
    {
      switch(j%10)
        {
        case 0 : {
          ret_val = "0" + ret_val;
          break;
        }
        case 1 : {
          ret_val = "1" + ret_val;
          break;
        }
        case 2 : {
          ret_val = "2" + ret_val;
          break;
        }
        case 3 : {
          ret_val = "3" + ret_val;
          break;
        }
        case 4 : {
          ret_val = "4" + ret_val;
          break;
        }
        case 5 : {
          ret_val = "5" + ret_val;
          break;
        }
        case 6 : {
          ret_val = "6" + ret_val;
          break;
        }
        case 7 : {
          ret_val = "7" + ret_val;
          break;
        }
        case 8 : {
          ret_val = "8" + ret_val;
          break;
        }
        case 9 : {
          ret_val = "9" + ret_val;
          break;
        }
        }
      j = j/10;
    }
  return ret_val;
}

/**
   Added by Ajith
   Synopsis: [A helper that returns binary string of an integer with zeros to pad to correct width]
   SideEffects:[]
*/

string integerToBinaryStringWithZeroPadding(unsigned long long int i, int width)
{
    if(i==0)
	{
	string ret_str="";
    	for(int i=0; i<width; i++)
      		ret_str += "0";
 	return ret_str;
	}

    string bin_str="";
    
    int j=i;
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

    int NoOfZeros = width - str.length();
    
    string ret_str="";
    for(int i=0; i<NoOfZeros; i++)
      ret_str += "0";
 
    ret_str += str;
    return ret_str;
}



/**
   Synopsis: [A helper that allows translation of binary strings to unsinged long long integers]
   Description: [The function reads every character in binary string (LSB on right) and depending on its position 'i' and value 'v', computes v*(2^i)]
   SideEffects:[]
   SeeAlso:[]
   Arguments:[Valid binary string]
   PostCondition: [Ensures correct conversion of valid input, exits on invalid string]
*/


unsigned long long int binaryStringToULINT(string str)
{
  int length=str.size();
  unsigned long long int num=0;
  unsigned long long int base=1;
  for(int i=length-1;i>=0;i--)
    {
	if((str[i]!='1') && (str[i]!='0'))
	{
	  cout<<"Unable to convert binary string to integer: "<<str<<" in binaryStringToULINT in /DAGManager/helping_functions.cc"<<endl;
	  exit(1);
	}
      if(str[i]=='1')
        {
          num=num+base;
        }
      base=base*2;
    }
  return num;


}
