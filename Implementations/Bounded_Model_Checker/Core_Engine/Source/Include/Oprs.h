/********************************************************************
  This code was developed entirely by Ashutosh Kulkarni and Supratik
  Chakraborty at IIT Bombay, India.  No part of this code may be used
  for commercial or other purposes without explicit permission from
  kashutosh@cse.iitb.ac.in and/or supratik@cse.iitb.ac.in

  August 10, 2009
*********************************************************************/

#ifndef oprh_h
#define oprh_h
#include<string>
#include<fstream>
#include<stdlib.h>
#include<iostream>
#include "macros.h"
using namespace std;

#define NUM_INTS 8
#define BYTE_SIZE 8

typedef unsigned long long int bv_block_t;// In the original code, it was unsigned long int; but sizeof(unsigned long long int) >= sizeof(unsigned long int) and hence avoids problems when 32-bit variables are encountered

typedef struct bv_internal_type {
 bv_block_t *bv_array;
 unsigned int bv_array_size;
 unsigned int bv_width;
} bvatom;

class BVManager
{
 private:
  unsigned int block_size, avail_block_size;
  unsigned int cumulative_array_size;
  bv_block_t *mask;

  unsigned int get_bv_array_size(unsigned int w);
  void         replaceBySelBVatom(bvatom bv_src, bvatom &bv_dest, 
				  unsigned int src_start, unsigned int dest_start, 
				  unsigned int length);
  void         freeBVatom(bvatom bv_inst) ;
  unsigned int convertBVToUnsignedInt(bvatom bv);
  unsigned int get_len_ignoring_left_zero_padding(bvatom bv_inst);
  void         printHexBV(bv_block_t * bv_inst, unsigned int width);
  bvatom       leftshiftBVatom(bvatom bv_inst, unsigned int shift, int preserve_width);
  bvatom       rightshiftBVatom(bvatom bv_inst, unsigned int shift, int preserve_width);
  bvatom       arith_div_mod(bvatom dividend, bvatom divisor, int div_mod_flag);
  inline unsigned int offset_from_width(unsigned int w);
  inline unsigned int block_from_width(unsigned int w);
  inline unsigned int offset_from_pos(unsigned int p);
  inline unsigned int block_from_pos(unsigned int p);
  inline unsigned int get_msb_offset(bvatom bv_inst);
  inline unsigned int get_msb_block(bvatom bv_inst);

 public:
  static BVManager *bvmanager_instance;

  static BVManager *getInstance();
  BVManager();
  bvatom       getBVatom(unsigned int width, string &bv_str);
  bvatom       getZeroedBVatom(unsigned int width);
  bvatom       bitwise_and(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       bitwise_or(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       bitwise_xor(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       bitwise_xnor(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       concat(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       select_with_int(bvatom bv_inst1, unsigned int hi, unsigned int lo);
  bvatom       select_with_bv(bvatom bv_inst1, bvatom bv_hi, bvatom bv_lo);
  bvatom       bitwise_negate(bvatom bv_inst);
  bvatom       reduction_and(bvatom bv_inst);
  bvatom       reduction_or(bvatom bv_inst);
  bvatom       reduction_xor(bvatom bv_inst);
  bvatom       reduction_xnor(bvatom bv_inst);
  bvatom       reduction_nor(bvatom bv_inst);
  bvatom       reduction_nand(bvatom bv_inst);
  bvatom       arith_sub(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       arith_add(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       arith_signed_add(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       arith_mult(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       arith_div(bvatom dividend, bvatom divisor);
  bvatom       arith_mod(bvatom dividend, bvatom divisor);
  bvatom       arith_neg(bvatom bv_inst);
  bvatom       left_shift(bvatom bv_inst, bvatom bv_shift);
  bvatom       ext_left_shift(bvatom bv_inst, bvatom bv_shift);
  bvatom       right_shift(bvatom bv_inst, bvatom bv_shift);
  bool         checkBVGreaterThan(bvatom bv_inst1, bvatom bv_inst2);
  bool         checkBVEquality(bvatom bv_inst1, bvatom bv_inst2);
  bvatom       BVExtend(bvatom bv_inst, unsigned int width, int sign_extend);
  bool         checkBVForZero(bvatom bv_inst);
  inline bool  isMsb1(bvatom bv_inst);
  void         printBVatom(bvatom bv_inst, int pretty);
  void         printPrettyBVatom(bvatom bv_inst);
  
  string convertBVToHexString(bvatom bv); // Newly added by Ajith on 7 Feb 2011
  string convertBVToHexString_Internal(bv_block_t *bvarray, unsigned int width); // Newly added by Ajith on 7 Feb 2011

};

/******* Commenting for testing

void printValue(ofstream *outfile,int type);
void printBVAtom(bvatom bv_result,ofstream * outfile);

class Type
{
 public:
  int data_type;
  int width;
  void printType(ofstream *outfile)
  {
        *outfile <<dec<<" ["<<data_type<<"] ["<<width<<"] ";
	//cout <<" "<<data_type<<" "<<width<<" ";
  }
};

typedef union
{
  bvatom bv_result;
  int int_result;
  bool bool_result;
  void printValue(ofstream *outfile,int type)
  {
    if(type==BITVECTOR_CONSTANT)
      {
	//printBVAtom(bv_result,outfile);
      }
    else if(type==INTEGER_CONSTANT)
      {
	*outfile<<int_result<<endl;
      }
    else if(type==LOGICAL_CONSTANT)
      {
	if(bool_result)
	  *outfile<<"true"<<endl;
	else
	  *outfile<<"false"<<endl;
      }
  }
  
}result_value;

 

class Result
{
 public:
  struct Type type_of_value;
  result_value value;
  Result()
    {
      type_of_value.data_type=UNKNOWN_DATA_TYPE;
      type_of_value.width=-1;
    }
  bool greater_than(Result r);
  bool equals(Result r);
  void printResult(ofstream *outfile)
  {
    type_of_value.printType(outfile);
    value.printValue(outfile,type_of_value.data_type);

  }
};



void makeMasks();
void makeRegMasks();
bvatom getBVOfInt(int num);
void printBVAtom(bvatom toPrint);
void mask(bvatom &num, int width);
void rmask(bvatom &num, int width);
void createZero();
void createOne();
void createRegZero();
void createRegOne();
unsigned long long int  getULLOfString(string s);
unsigned long long int  getRegULLOfString(string s);
bvatom createRegBVAtom(string bitvector);
bvatom createBVAtom(string bitvector);
bvatom exor(bvatom num1, bvatom num2);
bvatom bvlognot(bvatom num,int width);
bvatom bvneg(bvatom num,int width); //BITWISE_NEGATION
bvatom bvredand(bvatom num,int width);
bvatom bvredxor(bvatom num, int width);
bvatom bvredor(bvatom num,int width);
bvatom bvrednor(bvatom num,int width);
bvatom bvxor(bvatom num1,bvatom num2,int width);
bvatom bvand(bvatom num1,bvatom num2,int width);
bvatom bvor(bvatom num1,bvatom num2,int width);
bvatom bvxnor(bvatom num1,bvatom num2,int width);
bvatom bvequal(bvatom num1,bvatom num2,int width);
bvatom bvinequal(bvatom num1,bvatom num2,int width);
bvatom bvgt(bvatom num1,bvatom num2, int width);
bvatom bvlt(bvatom num1,bvatom num2, int width);
bvatom bvge(bvatom num1,bvatom num2, int width);
bvatom bvle(bvatom num1,bvatom num2, int width);
bvatom bvlogand(bvatom num1,bvatom num2, int width);
bvatom bvlogor(bvatom num1,bvatom num2, int width);
bvatom bvselect(bvatom num,unsigned long long int lo,unsigned long long int hi,unsigned long long int width);
bvatom bvite(bvatom cond, bvatom then_rslt,bvatom else_rslt,int width);
bvatom bvlftshft(bvatom num,int shift,int width);
bvatom bvconcat(bvatom num1, int w1, bvatom num2,int w2);
bvatom bvsub(bvatom num1,bvatom num2, int width);
bvatom bvadd(bvatom num1,bvatom num2, int width);

*******/


void printValue(ofstream *outfile,int type);
void printBVAtom(bvatom bv_result,ofstream * outfile);

class Type
{
 public:
  int data_type;
  int width;
  void printType(ofstream *outfile)
  {
        *outfile <<dec<<" ["<<data_type<<"] ["<<width<<"] ";
	//cout <<" "<<data_type<<" "<<width<<" ";
  }
};

typedef union
{
  bvatom bv_result;
  int int_result;
  bool bool_result;
  double real_result;

  void printValue(ofstream *outfile,int type)
  {
    if(type==BITVECTOR_CONSTANT)
      {
	//printBVAtom(bv_result,outfile);
      }
    else if(type==INTEGER_CONSTANT)
      {
	*outfile<<int_result<<endl;
      }
    else if(type==LOGICAL_CONSTANT)
      {
	if(bool_result)
	  *outfile<<"true"<<endl;
	else
	  *outfile<<"false"<<endl;
      }
    else if(type==REAL_CONSTANT)
      {
	*outfile<<real_result<<endl;
      }
  }
  
}result_value;

 

class Result
{
 public:
  struct Type type_of_value;
  result_value value;
  Result()
    {
      type_of_value.data_type=UNKNOWN_DATA_TYPE;
      type_of_value.width=-1;
    }
  bool greater_than(Result r);
  bool equals(Result r);
  void printResult(ofstream *outfile)
  {
    type_of_value.printType(outfile);
    value.printValue(outfile,type_of_value.data_type);

  }
};



void makeMasks();
void makeRegMasks();
bvatom getBVOfInt(int num);
void printBVAtom(bvatom toPrint);
void mask(bvatom &num, int width);
void rmask(bvatom &num, int width);
void createZero();
void createOne();
void createRegZero();
void createRegOne();
unsigned long long int  getULLOfString(string s);
unsigned long long int  getRegULLOfString(string s);
bvatom createRegBVAtom(string bitvector);
bvatom createBVAtom(string bitvector);
bvatom exor(bvatom num1, bvatom num2);
bvatom bvlognot(bvatom num,int width);
bvatom bvneg(bvatom num,int width); //BITWISE_NEGATION
bvatom bvredand(bvatom num,int width);
bvatom bvredxor(bvatom num, int width);
bvatom bvredor(bvatom num,int width);
bvatom bvrednor(bvatom num,int width);
bvatom bvxor(bvatom num1,bvatom num2,int width);
bvatom bvand(bvatom num1,bvatom num2,int width);
bvatom bvor(bvatom num1,bvatom num2,int width);
bvatom bvxnor(bvatom num1,bvatom num2,int width);
bvatom bvequal(bvatom num1,bvatom num2,int width);
bvatom bvinequal(bvatom num1,bvatom num2,int width);
bvatom bvgt(bvatom num1,bvatom num2, int width);
bvatom bvlt(bvatom num1,bvatom num2, int width);
bvatom bvge(bvatom num1,bvatom num2, int width);
bvatom bvle(bvatom num1,bvatom num2, int width);
bvatom bvlogand(bvatom num1,bvatom num2, int width);
bvatom bvlogor(bvatom num1,bvatom num2, int width);
bvatom bvselect(bvatom num,unsigned long long int lo,unsigned long long int hi,unsigned long long int width);
bvatom bvite(bvatom cond, bvatom then_rslt,bvatom else_rslt,int width);
bvatom bvlftshft(bvatom num,int shift,int width);
bvatom bvconcat(bvatom num1, int w1, bvatom num2,int w2);
bvatom bvsub(bvatom num1,bvatom num2, int width);
bvatom bvadd(bvatom num1,bvatom num2, int width);
#endif


