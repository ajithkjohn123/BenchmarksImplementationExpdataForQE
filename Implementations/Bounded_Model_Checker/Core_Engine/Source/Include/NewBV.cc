/********************************************************************
  This code was developed entirely by Ashutosh Kulkarni and Supratik
  Chakraborty at IIT Bombay, India.  No part of this code may be used
  for commercial or other purposes without explicit permission from
  kashutosh@cse.iitb.ac.in and/or supratik@cse.iitb.ac.in

  August 10, 2009
*********************************************************************/


#include <iostream>
#include "Oprs.h"

using namespace std;
BVManager* BVManager::bvmanager_instance = NULL;

BVManager::BVManager() { /* constructor */
  block_size = ((unsigned int) sizeof(bv_block_t) * BYTE_SIZE);
  avail_block_size = block_size - 1;
  cumulative_array_size = 0;
  /*allocate an array of so many (block_size) bv_block_t elements and 
    store the array in mask.*/
  mask = new bv_block_t[block_size];
  for (int i = 0; i < block_size; i++) {
    mask[i] = ((bv_block_t) 1) << i;
 }
}

BVManager* BVManager::getInstance()
{
    if(bvmanager_instance == NULL)
    {
        bvmanager_instance = new BVManager();
    }
    return bvmanager_instance;  
}

inline unsigned int max(unsigned int i1,unsigned int i2)
{
  return (i1<i2)?i2:i1;
}

inline unsigned int BVManager::offset_from_width(unsigned int w) 
{
  return ((w % avail_block_size == 0) ? (avail_block_size - 1) : ((w % avail_block_size) - 1));
}

inline unsigned int BVManager::block_from_width(unsigned int w) 
{
  return ((w/avail_block_size) - ((w % avail_block_size == 0) ? 1 : 0));
}

inline unsigned int BVManager::offset_from_pos(unsigned int p) 
{
  return (p % avail_block_size);
}

inline unsigned int BVManager::block_from_pos(unsigned int p) 
{
  return (p/avail_block_size);
}

inline unsigned int BVManager::get_msb_offset(bvatom bv_inst)
{
  return (offset_from_width(bv_inst.bv_width));
}

inline unsigned int BVManager::get_msb_block(bvatom bv_inst)
{
  return (bv_inst.bv_array_size - 1);
}

inline bool BVManager::isMsb1(bvatom bv_inst)
{
  return ((bv_inst.bv_array[get_msb_block(bv_inst)] & mask[get_msb_offset(bv_inst)]) != (bv_block_t) 0);
}

void BVManager::printBVatom(bvatom bv_inst, int pretty)
{
  if (pretty == 1) {
    printPrettyBVatom(bv_inst);
  }
  else {
    printHexBV(bv_inst.bv_array, bv_inst.bv_array_size * block_size);
    cout << "[" << dec << bv_inst.bv_width << "]";
  }
}

void BVManager::printPrettyBVatom(bvatom bv_inst)
{
  unsigned int trim_array_size = bv_inst.bv_width/block_size;

  if (bv_inst.bv_width % block_size != 0) {
    trim_array_size++;
  }

  bv_block_t *trim_bv = new bv_block_t[trim_array_size];
  for (int i = 0; i < trim_array_size; i++) {
    trim_bv[i] = (bv_block_t) 0;
  }
  
  for (int i = 0; i < bv_inst.bv_width; i++) {
    unsigned int trim_block = i/block_size;
    unsigned int trim_offset = i % block_size;

    unsigned int bv_block = i/avail_block_size;
    unsigned int bv_offset = i % avail_block_size;

    if ((bv_inst.bv_array[bv_block] & mask[bv_offset]) != (bv_block_t)0) {
      trim_bv[trim_block] = trim_bv[trim_block] | mask[trim_offset];
    }
  }

  printHexBV(trim_bv, bv_inst.bv_width);
  cout << "[" << dec << bv_inst.bv_width << "]";

  delete[] trim_bv;
}

void BVManager::printHexBV(bv_block_t *bvarray, unsigned int width)
{
  if (width == 0) {
    cerr << "ERROR! Can't print bitvector of length 0." << endl;
    exit(-1);
  }

  static unsigned int hex_width;
  static unsigned int hex_blocks_in_block;
  static bv_block_t * hex_blocks_mask;
  static int called_once = 0;

  if (called_once == 0) {
    hex_width = 4;
    hex_blocks_in_block = block_size/hex_width;
    hex_blocks_mask = new bv_block_t[hex_blocks_in_block];
    called_once = 1;

    for (int i = 0; i < hex_blocks_in_block; i++) {
      hex_blocks_mask[i] = (bv_block_t) 0;
      for (int j = 0; j < hex_width; j++) {
	hex_blocks_mask[i] = hex_blocks_mask[i] | mask[i*hex_width + j];
      }
    }
  }

  if (block_size % hex_width != 0) {
    cerr << "ERROR! Block size " << block_size;
    cerr << " not a multiple of hex_width " << hex_width << endl;
    exit(-1);
  }

  if ((width % hex_width) != 0) {
    width += (hex_width - (width % hex_width));
    /* This makes width a multiple of hex_width */
  }
    

  int hex_blocks_mask_index;
  if (width % block_size == 0) {
    hex_blocks_mask_index = hex_blocks_in_block - 1;
  }
  else {
    hex_blocks_mask_index = (width % block_size)/hex_width - 1;
  }

  cout << " 0x";
  for (int i = width-1; i >= 0; i = i - hex_width) {
    unsigned int block = i / block_size;

    bv_block_t curr_val = bvarray[block] & hex_blocks_mask[hex_blocks_mask_index];
    if (hex_blocks_mask_index != 0) {
      curr_val = curr_val >> (hex_blocks_mask_index * hex_width);
      hex_blocks_mask_index--;
    }
    else {
      hex_blocks_mask_index = (block_size/hex_width - 1);
    }

    cout << hex << curr_val;
  }
}

unsigned int BVManager:: get_bv_array_size(unsigned int w)
{
 unsigned int result = w/ (avail_block_size);
 if (result * avail_block_size < w) {
   result = result + 1;
 }
 return result;
}

bvatom BVManager::getZeroedBVatom(unsigned int width) 
{
  bvatom result;
  result.bv_array_size = get_bv_array_size(width);
  //allocate an array of so many (result.bv_array_size) bv_block_t elements and
  //  store in result.bv_array;
  result.bv_array= new bv_block_t[result.bv_array_size];
  //  initialize all elements of array to (bv_block_t) 0.
  for(int i = 0; i < result.bv_array_size; i++)
    result.bv_array[i]=(bv_block_t)0;

  result.bv_width = width;
  cumulative_array_size += result.bv_array_size;
  return result;
}

void BVManager:: replaceBySelBVatom(bvatom bv_src, bvatom &bv_dest, unsigned int src_start, unsigned int dest_start, unsigned int length) 
{

 if (bv_dest.bv_width < dest_start + length) {
   cerr << "Insufficient width in destination BV ..."<<endl;
   exit(-1);
 }
 if (bv_src.bv_width < src_start + length) {
   cerr << "Insufficient width in source BV ..."<<endl;
   exit(-1);
 }

 /* Three cases to consider */

 if (dest_start == src_start) {
   unsigned int src_start_pos = offset_from_pos(src_start);
   unsigned int src_start_block = block_from_pos(src_start);
   unsigned int src_end_pos = offset_from_pos(src_start + length - 1);
   unsigned int src_end_block = block_from_pos(src_start + length-1);

   bv_block_t start_mask = (bv_block_t)0;
   bv_block_t end_mask = (bv_block_t)0;

   for (int i = 0; i < src_start_pos; i++) {
     start_mask = start_mask | mask[i];
   }

   for (int i = 0; i <= src_end_pos; i++) {
     end_mask = end_mask | mask[i];
   }

   if (src_start_block == src_end_block) {
     end_mask = ~start_mask & end_mask;
     start_mask = ~end_mask;
   }
 
   bv_block_t curr_val = bv_dest.bv_array[src_start_block];
   curr_val = curr_val & start_mask;
   curr_val = curr_val | (bv_src.bv_array[src_start_block] & ~start_mask);
   bv_dest.bv_array[src_start_block] = curr_val;

   for (int i = src_start_block + 1; i < src_end_block; i++) {
     bv_dest.bv_array[i] = bv_src.bv_array[i];
   }

   curr_val = bv_dest.bv_array[src_end_block];
   curr_val = curr_val & ~end_mask;
   curr_val = curr_val | (bv_src.bv_array[src_end_block] & end_mask);
   bv_dest.bv_array[src_end_block] = curr_val;
   
 }
 else if (src_start > dest_start) {
   unsigned int shift = src_start - dest_start;
   
   /* Preserve width while right shifting */
   bvatom temp_bv = rightshiftBVatom(bv_src, shift, 1);
   replaceBySelBVatom(temp_bv, bv_dest, dest_start, dest_start, length);
   freeBVatom(temp_bv);
 }
 else { /* dest_start > src_start */
   unsigned int shift = dest_start - src_start;

   /* Extend width while left shifting */
   bvatom temp_bv = leftshiftBVatom(bv_src, shift, 0);
   replaceBySelBVatom(temp_bv, bv_dest, dest_start, dest_start, length);
   freeBVatom(temp_bv);
 }
}

bvatom BVManager::getBVatom(unsigned int width, string &bv_str)
{
  if (width == 0) {
    cerr << "ERROR! getBVatom invoked with zero width" << endl;
    exit(-1);
  }

  bvatom result = getZeroedBVatom(width);
  unsigned int last_block_offset = get_msb_offset(result); /* width % avail_block_size; */
  int curr_block = (int) get_msb_block(result); /* result.bv_array_size - 1; */
  
  int bv_str_index = 0;
  
  /*   if (last_block_offset > 0) { */
  if (last_block_offset < avail_block_size - 1) {
    
    /* fill in result.bv_array[result.bv_array_size-1] with the first
       (leftmost) last_block_offset characters of bv_str */
    
    bv_block_t curr_val = (bv_block_t) 0;
    
    for (int i = last_block_offset; i >= 0; i--) {
      curr_val = curr_val << 1;
      if ((bv_str[bv_str_index] != '0') && (bv_str[bv_str_index] != '1')) {
	cerr << "ERROR!! Invalid string occured\n";
	exit(-1);
      }
      else {
	if (bv_str[bv_str_index] == '1') {
	  curr_val |= (bv_block_t) 1;
	}
	bv_str_index++;
      }
    }
    result.bv_array[curr_block] = curr_val;
    curr_block--;
  }
  
  for (; curr_block >= 0; curr_block--) {
    bv_block_t curr_val = (bv_block_t) 0;
    
    for (int i = avail_block_size - 1; i >= 0; i--) {
      curr_val = curr_val << 1;
      if ((bv_str[bv_str_index] != '0') && (bv_str[bv_str_index] != '1')) {
	cerr << "ERROR!! Invalid char of string " << bv_str << " at index " << bv_str_index;
	cerr << " : " << bv_str[bv_str_index] << "\n";
	exit(-1);
      }
      else {
	if (bv_str[bv_str_index] == '1') {
	  curr_val |= (bv_block_t) 1;
	}
	bv_str_index++;
      }
    }
    
    result.bv_array[curr_block] = curr_val;
  }
  
  if (bv_str_index != bv_str.size()) {
    cerr << "ERROR!! bv string not of same width as specified.\n";
    exit(-1);
  }
  return result;
}

void BVManager::freeBVatom(bvatom bv_inst) 
{
 cumulative_array_size -= bv_inst.bv_array_size;
 delete[] bv_inst.bv_array;
}

unsigned int BVManager::convertBVToUnsignedInt(bvatom bv)
{
  unsigned int result = (unsigned int) 0;
  if(sizeof(unsigned int) < bv.bv_width)
    {
      cerr<<"ERROR!! ConvertBVToUnsignedInt error\n";
      exit(-1);
    }
  else
    {
      for(int i = 0; i < bv.bv_array_size; i++)
	{
	  result=(result<<(avail_block_size))+ (unsigned int) bv.bv_array[i];
	}
      return result;
    }
}

bvatom BVManager::left_shift(bvatom bv_inst, bvatom bv_shift)
{
  unsigned int shift=convertBVToUnsignedInt(bv_shift);
  /* Last parameter = 1 indicates width of bitvector to be preserved */
  bvatom result = leftshiftBVatom(bv_inst, shift, (unsigned int) 1);
  return result;
}

bvatom BVManager::ext_left_shift(bvatom bv_inst, bvatom bv_shift)
{
  unsigned int shift=convertBVToUnsignedInt(bv_shift);
  /* Last parameter = 0 indicates width of bitvector to be extended, if needed */
  bvatom result = leftshiftBVatom(bv_inst, shift, (unsigned int) 0);
  return result;
}

bvatom BVManager::right_shift(bvatom bv_inst, bvatom bv_shift)
{
  unsigned int shift=convertBVToUnsignedInt(bv_shift);
  /* Last parameter = 1 indicates width of bitvector to be preserved */
  bvatom result = rightshiftBVatom(bv_inst, shift, (unsigned int) 1);
  return result;
}

bvatom BVManager::leftshiftBVatom(bvatom bv_inst, unsigned int shift, int preserve_width)
{
  unsigned int width;
  if (preserve_width == 0) {
    width = ((unsigned int) bv_inst.bv_width) + shift;
  }
  else {
    width = (unsigned int) bv_inst.bv_width;
  }

  unsigned int untruncated_width = bv_inst.bv_width + shift;
  unsigned int untruncated_arr_size = get_bv_array_size(untruncated_width);

  bvatom result = getZeroedBVatom(width);
  
  unsigned int start_block = block_from_pos(shift);
  unsigned int start_pos = offset_from_pos(shift);

  bv_block_t start_mask = (bv_block_t) 0;

  for (int i = 0; i < avail_block_size - start_pos; i++) {
    start_mask = start_mask | mask[i];
  }

  bv_block_t residue = (bv_block_t) 0;
  for (int i = start_block; i < result.bv_array_size; i++) {
    bv_block_t curr_val;

    curr_val = (bv_inst.bv_array[i - start_block] & start_mask);
    curr_val = curr_val << start_pos;
    curr_val = curr_val | residue;
    residue = (bv_inst.bv_array[i - start_block] & ~start_mask);
    residue = residue >> (avail_block_size - start_pos);

    result.bv_array[i] = curr_val;
  }    

  return result;
}

bvatom BVManager::rightshiftBVatom(bvatom bv_inst, unsigned int shift, int preserve_width)
{
  unsigned int width;
  bvatom result;

  if (preserve_width == 0) {
    width = bv_inst.bv_width + shift;
    result = getZeroedBVatom(width);

    /* Simply pad "shift" 0's to the left of bv_inst */
    for (int i = 0; i < bv_inst.bv_array_size; i++) {
      result.bv_array[i] = bv_inst.bv_array[i];
    }
  }
  else {
    width = bv_inst.bv_width;
    result = getZeroedBVatom(width);
  
    unsigned int start_block = block_from_pos(shift);
    unsigned int start_pos = offset_from_pos(shift);
    
    bv_block_t start_mask = (bv_block_t) 0;
  
  for (int i = start_pos; i < avail_block_size; i++) {
    start_mask = start_mask | mask[i];
  }

    bv_block_t residue;
    for (int i = start_block; i < bv_inst.bv_array_size; i++) {
      bv_block_t curr_val;
      
      if (i != bv_inst.bv_array_size - 1) {
	curr_val = bv_inst.bv_array[i] & start_mask;
	curr_val = curr_val >> start_pos;
	residue = (bv_inst.bv_array[i+1]) & ~start_mask;
	residue = residue << (avail_block_size - start_pos);
	curr_val = curr_val | residue;
      }
      else {
	curr_val = bv_inst.bv_array[i] & start_mask;
	curr_val = curr_val >> start_pos;
      }

      result.bv_array[i - start_block] = curr_val;
    }    
  }

  return result;
}

bvatom BVManager::BVExtend(bvatom bv_inst, unsigned int width, int sign_extend)
{

  if(width <= bv_inst.bv_width)
    {
      /* We don't truncate bv_inst, instead return the whole of bv_inst */

      bvatom result=getZeroedBVatom(bv_inst.bv_width);
      for (int i = 0; i < bv_inst.bv_array_size; i++) {
	result.bv_array[i] = bv_inst.bv_array[i];
      }
      return result;
    }
  else
    {
      unsigned int msb_offset = get_msb_offset(bv_inst);	
      unsigned int msb;

      if ((sign_extend == 1) && isMsb1(bv_inst))
	{
	  msb = 1;
	}
      else
	{
	  msb = 0;
	}

      bvatom result = getZeroedBVatom(width);

      for (int i = 0; i < bv_inst.bv_array_size; i++) {
	result.bv_array[i] = bv_inst.bv_array[i];
      }

      if(msb == 1)
	{
	  bv_block_t start_mask = (bv_block_t) 0;

	  for (int i = msb_offset + 1; i < avail_block_size; i++) {
	    start_mask = start_mask | mask[i];
	  }

	  bv_block_t mid_mask = ~mask[block_size - 1];

	  unsigned int end_offset = offset_from_width(width);
	  bv_block_t end_mask = (bv_block_t) 0;
	  for (int i = 0; i <= end_offset; i++) {
	    end_mask = end_mask | mask[i];
	  }

	  if (result.bv_array_size == bv_inst.bv_array_size) {
	    bv_block_t curr_val = result.bv_array[result.bv_array_size - 1];
	    result.bv_array[result.bv_array_size - 1] = (curr_val | start_mask) & end_mask;
	  }
	  else {
	    result.bv_array[bv_inst.bv_array_size - 1] =
	      result.bv_array[bv_inst.bv_array_size - 1] | start_mask;

	    for (int i = bv_inst.bv_array_size; i < result.bv_array_size - 1; i++) {
	      result.bv_array[i] = mid_mask;
	    }

	    result.bv_array[result.bv_array_size - 1] = end_mask;
	  }
	}
      return result;
    }
}


bvatom BVManager::arith_neg(bvatom bv_inst)
{
  if(checkBVForZero(bv_inst))
    {
      return getZeroedBVatom(bv_inst.bv_width);
    }
  else
    {
      bvatom result=getZeroedBVatom(bv_inst.bv_width);
      int flag = 0;
      for(int i = 0; i < bv_inst.bv_array_size; i++)
	{
	  if(flag==1)
	    {
	      bv_block_t current_val=~(bv_inst.bv_array[i]) & ~(mask[block_size-1]);
	      result.bv_array[i]=current_val;
	    }
	  else
	    {
	      if(bv_inst.bv_array[i]==(bv_block_t)0)
		{
		  result.bv_array[i]=(bv_block_t)0;
		}
	      else
		{
		  bv_block_t current_val=(bv_inst.bv_array[i]);
		  int counter = 0;
		  bv_block_t current_mask=mask[0];
		  while(counter<avail_block_size)
		    {
		      if(current_val%2 == 1)
			{
			  break;
			}
		      else
			{
			  counter++;
			  current_val=current_val>>1;
			  current_mask=current_mask | mask[counter];
			}
		    }
		  
		  if(counter==avail_block_size) {
		    cerr<<"ERROR!! shift overflow in arith_neg\n";
		    exit(-1);
		  }
		  
		  current_val=bv_inst.bv_array[i] & current_mask;
		  bv_block_t higher_current_val=bv_inst.bv_array[i] & ~current_mask;
		  higher_current_val=
		    (~higher_current_val & ~current_mask) & ~mask[block_size-1];
		  result.bv_array[i]=current_val | higher_current_val;
		 
		  flag=1;
		}
	    }
	}//end for

      /* Now ensure that proper width is restored to the result bitvector */

      unsigned int width = bv_inst.bv_width;
      unsigned int msb_block = block_from_width(width);
      unsigned int msb_offset = offset_from_width(width);

      bv_block_t current_mask=(bv_block_t)0;
      for(int i = 0; i <= msb_offset; i++)
	{
	  current_mask=current_mask | mask[i];
	}
      result.bv_array[msb_block] = result.bv_array[msb_block] & current_mask;
      return result;
    }
}

bvatom BVManager::arith_signed_add(bvatom bv_inst1, bvatom bv_inst2)
{
  unsigned int msb1 = isMsb1(bv_inst1) ? 1:0;
  unsigned int msb2 = isMsb1(bv_inst2) ? 1:0;

  bvatom result = arith_add(bv_inst1, bv_inst2);
  unsigned int maxw = max(bv_inst1.bv_width, bv_inst2.bv_width);

  unsigned int msbr = isMsb1(result) ? 1:0;

  if ((msb1 == 1) && (msb2 == 1)) {
    /* Both operands were negative. Result must have an additional bit,
       and result.bv_width = max of operands' widths + 1. msbr must be
       1 in this case.
       However, if both msbr and the previous bit of the result are 1,
       one of the 1's can be shaved off.
    */
    
    unsigned int m2sbr_offset = offset_from_width(result.bv_width - 1);
    unsigned int m2sbr_block = block_from_width(result.bv_width - 1);

    unsigned int m2sbr = 
      ((result.bv_array[m2sbr_block] & mask[m2sbr_offset]) == (bv_block_t)0) ? 0:1;

    if (m2sbr == 1) {
      bvatom new_result = getZeroedBVatom(result.bv_width - 1);
      replaceBySelBVatom(result, new_result, (unsigned int) 0, (unsigned int) 0, result.bv_width - 1);
      freeBVatom(result);
      result = new_result;
    }
  }
  else if ((msb1 == 0) && (msb2 == 0)) {
    /* Both operands were positive. Result cannot have an additional bit,
       and result.bv_width = max of operands' widths. 
       Now, if msbr is 1, a 0 has to be padded to the left, so that the
       result is not interpretted as negative.
    */

    if (msbr == 1) {
      bvatom new_result = BVExtend(result, result.bv_width + 1, 0);
      freeBVatom(result);
      result = new_result;
    }
  }
  else {
    /* One operand is positive and the other negative.  The width of
       the result need not be larger than the max width of the operands.
    */

    if (result.bv_width > maxw) {
      bvatom new_result = getZeroedBVatom(maxw);
      replaceBySelBVatom(result, new_result, (unsigned int) 0, (unsigned int) 0, maxw);
      freeBVatom(result);
      result = new_result;
    }
  }

  return result;
}

bvatom BVManager::arith_add(bvatom bv_inst1_in, bvatom bv_inst2_in)
{
  /* Assume both operands are unsigned. */

  unsigned int w1 = bv_inst1_in.bv_width;
  unsigned int w2 = bv_inst2_in.bv_width;
  bvatom bv_inst1,bv_inst2;

  if(w1 > w2)
    {
      bv_inst2=BVExtend(bv_inst2_in, w1, 0);
      bv_inst1=bv_inst1_in;
    }
  else if(w1 < w2)
    {
      bv_inst1=BVExtend(bv_inst1_in, w2, 0);
      bv_inst2=bv_inst2_in;
      
    }
  else
    {
      bv_inst1=bv_inst1_in;
      bv_inst2=bv_inst2_in;
    }


  unsigned int width = bv_inst1.bv_width+ 1;
  bvatom result = getZeroedBVatom(width);

  unsigned int max_arr_sz = bv_inst1.bv_array_size;

  unsigned int carry = (unsigned int) 0; 
  bv_block_t curr_result=(bv_block_t)0;

  for (int i = 0; i < max_arr_sz; i++) { 
    curr_result = (bv_inst1.bv_array[i]) + (bv_inst2.bv_array[i]) + (bv_block_t) carry;
    unsigned int carry_out = 
      ((curr_result & mask[block_size -1]) == (bv_block_t)0) ? (bv_block_t)0: (bv_block_t)1;
    curr_result = curr_result & ~mask[block_size-1];
    result.bv_array[i] = curr_result;
    carry = carry_out;
  }//end for

  if (carry != (unsigned int) 0) {
    if (max_arr_sz < result.bv_array_size) {
      result.bv_array[result.bv_array_size - 1] = (bv_block_t) 1;
    }
  }

  /* Check if it is ok to reduce the bit vector width by 1 */

  if (!isMsb1(result))
    {
      //ok to decrease width
      bvatom new_result=getZeroedBVatom(width - 1);
      replaceBySelBVatom(result, new_result, (unsigned int) 0, (unsigned int) 0, width-1);
      freeBVatom(result);
      result = new_result;
    }
    
  if(w1 > w2)
    {
      freeBVatom(bv_inst2);
    }
  else if(w1 < w2)
    {
      freeBVatom(bv_inst1);
    }
  
 return(result);
}

bvatom BVManager::arith_sub(bvatom bv_inst1, bvatom bv_inst2)
{
  /* Both arguments assumed to be signed bitvectors */

  bvatom neg_bv_inst2 = arith_neg(bv_inst2);

  if (checkBVEquality(neg_bv_inst2, bv_inst2) && (!checkBVForZero(bv_inst2))) {
    /* There are only two values whose 2's compl. negation has the same
       representation as the original no.
       One of these is 10000000...0000 and the other is 0.
       In the former case, when we negate the value, we need to
       pad a 0 at the left to designate the positive value.
       Otherwise, neg(1000) would again be 1000, i.e.
       neg(-8) would be -8 again.  With an additional 0 padded
       to the left of the result, we get neg(1000) = 01000,
       or neg(-8) = 8.
    */
    neg_bv_inst2 = BVExtend(bv_inst2, bv_inst2.bv_width + 1, 0);
  }

  bvatom result=arith_signed_add(bv_inst1, neg_bv_inst2);
  return result;
}

bvatom BVManager::arith_mult(bvatom bv_inst1_in, bvatom bv_inst2_in)
{
  /* Assume both operands are unsigned */

  unsigned int w1 = bv_inst1_in.bv_width;
  unsigned int w2 = bv_inst2_in.bv_width;

  unsigned int width = w1+w2;
  bvatom result = getZeroedBVatom(width);
  bvatom shifted_op = getZeroedBVatom(width);

  /* Let bv_inst1 be the shorter bit vector, and bv_inst2 be the longer one */
  
  bvatom bv_inst1, bv_inst2;

  if (w1 <= w2) {
    bv_inst1 = bv_inst1_in;
    bv_inst2 = bv_inst2_in;
  }
  else {
    bv_inst1 = bv_inst2_in;
    bv_inst2 = bv_inst1_in;
  }

  /* Copy bv_inst2 into shifted_op */
  replaceBySelBVatom(bv_inst2, shifted_op, (unsigned int) 0, (unsigned int) 0, bv_inst2.bv_width);

  unsigned int incremental_shift;

  for (int i = 0; i < bv_inst1.bv_array_size; i++) {
    bv_block_t curr_val = bv_inst1.bv_array[i];

    if (curr_val == (bv_block_t) 0) {
      /* Width preserving left shift by avail_block_size */
      bvatom temp_shifted_op = leftshiftBVatom(shifted_op, avail_block_size, 1);
      freeBVatom(shifted_op);
      shifted_op = temp_shifted_op;
    }
    else {
      incremental_shift = 0;
      int max_offset = 0;

      if (i == bv_inst1.bv_array_size - 1) {
	max_offset = get_msb_offset(bv_inst1) + 1;
      }
      else {
	max_offset = avail_block_size;
      }

      for (int j = 0; j < max_offset; j++) {
	if ((bv_inst1.bv_array[i] & mask[j]) != (bv_block_t)0) {
	  if (incremental_shift > 0) {
	    bvatom temp_shifted_op = leftshiftBVatom(shifted_op, incremental_shift, 1);
	    freeBVatom(shifted_op);
	    shifted_op = temp_shifted_op;
	  }
	  
	  bvatom temp_result = arith_add(result, shifted_op);
	  freeBVatom(result);
	  result = temp_result;
	  
	  incremental_shift = 0;
	}

	incremental_shift++;
      }
    }
  }

  /* Now check if the width really needs to be larger than the
     longest operand's, i.e., bv_inst2's bit width */

  bvatom truncated_result = getZeroedBVatom(bv_inst2.bv_width);
  replaceBySelBVatom(result, truncated_result, (unsigned int) 0, (unsigned int) 0, bv_inst2.bv_width);
  bvatom ext_trunc_result = BVExtend(truncated_result, result.bv_width, 0);

  if (checkBVEquality(result, ext_trunc_result)) {
    freeBVatom(result);
    freeBVatom(ext_trunc_result);
    return truncated_result;
  }
  else {
    freeBVatom(truncated_result);
    freeBVatom(ext_trunc_result);
    return result;
  }
}

bvatom BVManager::arith_div_mod(bvatom dividend, bvatom divisor, int div_mod_flag)
{
  unsigned int w1 = dividend.bv_width; 
  unsigned int w2 = divisor.bv_width; 

  /* Assume both dividend and divisor are unsigned, and extend them 
     to the same bit width */

  bvatom ext_dividend, ext_divisor;

  if (w1 < w2) {
    if (isMsb1(divisor)) {
      ext_dividend = BVExtend(dividend, w2+1, 0);
      ext_divisor = BVExtend(divisor, w2+1, 0);
    }
    else {
      ext_dividend = BVExtend(dividend, w2, 0);
      ext_divisor = divisor;
    }
  }
  else if (w2 < w1) {
    if (isMsb1(dividend)) {
      ext_divisor = BVExtend(divisor, w1+1, 0);
      ext_dividend = BVExtend(dividend, w1+1, 0);
    }
    else {
      ext_divisor = BVExtend(divisor, w1, 0);
      ext_dividend = dividend;
    }      
  }
  else { /* w1 == w2 */
    if (isMsb1(dividend) || isMsb1(divisor)) {
      ext_dividend = BVExtend(dividend, w1+1, 0);
      ext_divisor = BVExtend(divisor, w1+1, 0);
    }
    else {
      ext_dividend = dividend;
      ext_divisor = divisor;
    }
  }

  /* Now ext_dividend.bv_width == ext_divisor.bv_width and each
     has a 0 as its msb.  This 0-padding is needed to allow the
     unsigned integer interpretation even when we use arith_sub
     below.
  */
 
  bvatom quotient = getZeroedBVatom((unsigned int) 1);
  bvatom residue;

  if (checkBVForZero(ext_divisor)) {
    cerr << "ERROR! Division invoked with zero divisor.\n";
    exit(-1);
  }
  else if (checkBVGreaterThan(ext_divisor, ext_dividend)) {
    /* Divisor greater than dividend */

    quotient.bv_array[0] = (bv_block_t) 0;
    residue = getZeroedBVatom(ext_dividend.bv_width);
    replaceBySelBVatom(ext_dividend, residue, (unsigned int) 0, (unsigned int) 0, ext_dividend.bv_width);
    
    /* Ready to return after garbage collection */
  }
  else if (checkBVEquality(ext_divisor, ext_dividend)) {
    /* Divisor equal to dividend */

    quotient.bv_array[0] = (bv_block_t) 1;
    residue = getZeroedBVatom(ext_dividend.bv_width);

    /* Ready to return after garbage collection */
  }
  else {
    /* Divisor less than dividend */

    unsigned int true_divisor_width = get_len_ignoring_left_zero_padding(ext_divisor);
    
    bvatom const_one = getZeroedBVatom((unsigned int) 1);
    const_one.bv_array[0] = (bv_block_t) 1;
    bvatom const_two = getZeroedBVatom((unsigned int) 2);
    const_two.bv_array[0] = (bv_block_t) 2;
    
    quotient.bv_array[0] = (bv_block_t) 0;
      
    /* Copy ext_dividend in residue */
    residue = getZeroedBVatom(ext_dividend.bv_width);
    replaceBySelBVatom(ext_dividend, residue, (unsigned int) 0, (unsigned int) 0, ext_dividend.bv_width);

    do {
      /* Loop invariant: 
	 residue > ext_divisor  AND
	 residue.bv_width == ext_divisor.bv_width */

      bvatom new_residue = arith_sub(residue, ext_divisor);
      
      if (checkBVGreaterThan(ext_divisor, new_residue)) {
	bvatom new_quotient = arith_add(quotient, const_one);
	freeBVatom(quotient);
	freeBVatom(residue);
	quotient = new_quotient;
	residue = new_residue;
	break;
	/* Ready to return after garbage collecting */
      }
      else if (checkBVEquality(ext_divisor, new_residue)) {
	bvatom new_quotient = arith_add(quotient, const_two);
	freeBVatom(quotient);
	freeBVatom(residue);
	quotient = new_quotient;
	residue = getZeroedBVatom((unsigned int) 1);
	residue.bv_array[0] = (bv_block_t) 0;
	break;
	/* Ready to return after garbage collecting */
      }
      else {
	/* new_residue > ext_divisor, i.e. residue > 2*ext_divisor */
	
	freeBVatom(new_residue);
	
	unsigned int true_residue_width = get_len_ignoring_left_zero_padding(residue);
	unsigned int width_diff = true_residue_width - true_divisor_width;
	
	/* Width preserving left shift of divisor. Recall that the total
	   bit widths (as in bvatom class) of residue and ext_divisor 
	   are already matched up.
	*/
	bvatom shifted_divisor = leftshiftBVatom(ext_divisor, width_diff, 1);
	
	if (checkBVGreaterThan(shifted_divisor, residue)) {
	  /* Left shift by one less.
	     Since we already know that new_residue = residue - divisor > divisor,
	     therefore residue > 2*divisor.
	     Therefore, width_diff must be > 1.  And hence, width_diff - 1 >= 1.
	  */
	  width_diff --;
	  freeBVatom(shifted_divisor);
	  shifted_divisor = leftshiftBVatom(ext_divisor, width_diff, 1);
	}
	
	new_residue = arith_sub(residue, shifted_divisor);
	bvatom increment_quotient = leftshiftBVatom(const_one, width_diff, 0);
	bvatom new_quotient = arith_add(quotient, increment_quotient);

	freeBVatom(increment_quotient);
	freeBVatom(quotient);
	freeBVatom(residue);
	freeBVatom(shifted_divisor);

	residue = new_residue;
	quotient = new_quotient;
	
	if (checkBVForZero(residue) || checkBVGreaterThan(ext_divisor, residue)) {
	  break;
	  
	  /* Ready to return after garbage collection */
	}
	else if (checkBVEquality(residue, ext_divisor)) {
	  bvatom new_quotient = arith_add(quotient, const_one);
	  freeBVatom(quotient);
	  freeBVatom(residue);
	  quotient = new_quotient;
	  residue = getZeroedBVatom((unsigned int) 1);
	  residue.bv_array[0] = (bv_block_t) 0;
	  break;
	}
      }
    } while (1);
    
    freeBVatom(const_one);
    freeBVatom(const_two);
  }

  /* Garbage collection and return */
  if (ext_dividend.bv_width != dividend.bv_width) {
    freeBVatom(ext_dividend);
  }
  if (ext_divisor.bv_width != divisor.bv_width) {
    freeBVatom(ext_divisor);
  }

  if (div_mod_flag == 1) {
    freeBVatom(residue);
  
    /* Trim down the quotient of residue keeping it an unsigned bit vector */
    unsigned int quot_len = get_len_ignoring_left_zero_padding(quotient);
    if (quot_len == 0) {
      freeBVatom(quotient);
      quotient = getZeroedBVatom(1);
      quotient.bv_array[0] = (bv_block_t) 0;
    }
    else {
      if (quot_len != quotient.bv_width) {
	bvatom new_quotient = getZeroedBVatom(quot_len);
	replaceBySelBVatom(quotient, new_quotient, (unsigned int) 0, (unsigned int) 0, quot_len);
	freeBVatom(quotient);
	quotient = new_quotient;
      }
    }
    return quotient;
  }
  else {
    freeBVatom(quotient);

    /* Trim down the width of residue keeping it an unsigned bit vector */
    unsigned int res_width = get_len_ignoring_left_zero_padding(residue);
    if (res_width == 0) {
      freeBVatom(residue);
      residue = getZeroedBVatom(1);
      residue.bv_array[0] = (bv_block_t)0;
    }
    else {
      if (res_width != residue.bv_width) {
	bvatom new_res = getZeroedBVatom(res_width);
	replaceBySelBVatom(residue, new_res, (unsigned int) 0, (unsigned int) 0, res_width);
	freeBVatom(residue);
	residue = new_res;
      }
    }
    return residue;
  }
}

unsigned int BVManager::get_len_ignoring_left_zero_padding(bvatom bv_inst)
{
  if (checkBVForZero(bv_inst)) {
    return 0;
  }

  int leftmost1_block = -1;
  int i;

  for (i = bv_inst.bv_array_size - 1; i >= 0; i--) {
    if (bv_inst.bv_array[i] != (bv_block_t) 0) {
      leftmost1_block = i;
      break;
    }
  }

  if (leftmost1_block == -1) {
    cerr << "ERROR! Could not find leftmost 1 when checking length.\n";
    exit(-1);
  }
  else {
    int leftmost1_offset = -1;
  
    for (int j = avail_block_size - 1; j >= 0; j--) {
      if ((bv_inst.bv_array[i] & mask[j]) != (bv_block_t)0) {
	leftmost1_offset = j;
	break;
      }
    }
    
    if (leftmost1_offset == -1) {
      cerr << "ERROR! Could not find leftmost 1 when checking length.\n";
      exit(-1);
    }
      
    return (unsigned int) (leftmost1_block*avail_block_size + leftmost1_offset + 1);
  }
}

bvatom BVManager::arith_div(bvatom dividend, bvatom divisor)
{
  return arith_div_mod(dividend, divisor, 1);
}

bvatom BVManager::arith_mod(bvatom dividend, bvatom divisor)
{
  return arith_div_mod(dividend, divisor, 0);
}

bvatom BVManager::bitwise_and(bvatom bv_inst1,bvatom bv_inst2)
{

  if(bv_inst1.bv_width!=bv_inst2.bv_width)
    {
      cerr<<"ERROR!! bitwise_and\n";
      exit(-1);
    }
  else
    {
      bvatom result = getZeroedBVatom(bv_inst1.bv_width);
      bv_block_t msbmask = ~mask[block_size-1];
      for(int i = 0; i < bv_inst1.bv_array_size; i++)
	{
	  result.bv_array[i] = (bv_inst1.bv_array[i] &  bv_inst2.bv_array[i]) & msbmask; 
	}
      return result;
    }
}


bvatom BVManager::bitwise_or(bvatom bv_inst1,bvatom bv_inst2)
{

  if(bv_inst1.bv_width!=bv_inst2.bv_width)
    {
      cerr<<"ERROR!! bitwise_or\n";
      exit(-1);
    }
  else
    {
      bvatom result = getZeroedBVatom(bv_inst1.bv_width);
      bv_block_t msbmask = ~mask[block_size-1];
      for(int i = 0; i<bv_inst1.bv_array_size; i++)
	{
	  result.bv_array[i] = (bv_inst1.bv_array[i] |  bv_inst2.bv_array[i]) & msbmask; 
	}
      return result;
    }
}


bvatom BVManager::bitwise_xor(bvatom bv_inst1,bvatom bv_inst2)
{

  if(bv_inst1.bv_width!=bv_inst2.bv_width)
    {
      cerr<<"ERROR!! bitwise_xor\n";
      exit(-1);
    }
  else
    {
      bvatom result = getZeroedBVatom(bv_inst1.bv_width);
      bv_block_t msbmask = ~mask[block_size-1];
      for(int i = 0; i<bv_inst1.bv_array_size; i++)
	{
	  result.bv_array[i] = (bv_inst1.bv_array[i] ^  bv_inst2.bv_array[i]) & msbmask; 
	}
      return result;
    }
}


bvatom BVManager::bitwise_xnor(bvatom bv_inst1,bvatom bv_inst2)
{

  if(bv_inst1.bv_width!=bv_inst2.bv_width)
    {
      cerr<<"ERROR!! bitwise_xnor\n";
      exit(-1);
    }
  else
    {
      bvatom result = getZeroedBVatom(bv_inst1.bv_width);
      bv_block_t spcl_bit_mask = ~mask[block_size-1];
      for(int i = 0; i < bv_inst1.bv_array_size; i++)
	{
	  result.bv_array[i] = ~(bv_inst1.bv_array[i] ^  bv_inst2.bv_array[i]) & spcl_bit_mask; 
	}
      unsigned int last_offset = get_msb_offset(result); /* result.bv_width % avail_block_size; */
      if(last_offset < avail_block_size -1)
	{
	  bv_block_t last_mask = (bv_block_t)0;
	  for(int i = last_offset+1; i < block_size; i++)
	    last_mask = last_mask | mask[i];
	  last_mask = ~last_mask;
	  result.bv_array[result.bv_array_size -1 ] = result.bv_array[result.bv_array_size -1 ] & last_mask;
	}
      return result;
    }
}


bvatom BVManager::bitwise_negate(bvatom bv_inst1)
{

  bvatom result = getZeroedBVatom(bv_inst1.bv_width);
  bv_block_t spcl_bit_mask = ~mask[block_size-1];
  for(int i = 0; i < bv_inst1.bv_array_size; i++)
    {
      result.bv_array[i] = ~(bv_inst1.bv_array[i] ) & spcl_bit_mask; 
    }
  unsigned int last_offset = get_msb_offset(result);
  if(last_offset < avail_block_size - 1)
    {
      bv_block_t last_mask = (bv_block_t)0;
      for(int i = last_offset+1; i<block_size; i++)
	last_mask = last_mask | mask[i];
      last_mask = ~last_mask;
      result.bv_array[result.bv_array_size -1 ] = result.bv_array[result.bv_array_size -1 ] & last_mask;
    }
  
  return result;

}

bvatom BVManager::reduction_and(bvatom bv_inst1)
{
  bvatom result = getZeroedBVatom((unsigned int) 1);
  bv_block_t spcl_bit_mask = ~mask[block_size-1];
  int flag = 0;
  for(int i = 0; i < bv_inst1.bv_array_size - 1; i++)
    {
      bv_block_t current = ~(bv_inst1.bv_array[i] ) & spcl_bit_mask;
      if(current != (bv_block_t)0)
	{
	  flag = 1;
	  break;
	}
    }
  if(flag==1)
    return result;
  else
    {
      unsigned int last_offset = get_msb_offset(bv_inst1);
      if(last_offset < avail_block_size - 1)
	{
	  bv_block_t last_mask = (bv_block_t)0;
	  for(int i = last_offset + 1; i < block_size; i++)
	    last_mask = last_mask | mask[i];
	  last_mask = ~last_mask;
	  if((~(bv_inst1.bv_array[result.bv_array_size-1]) & last_mask) !=(bv_block_t)0)
	    return result; /* i.e. 0 */
	  else
	    {
	      result.bv_array[0]=(bv_block_t)1;
	      return result;
	    }
	}
      else
	{ /* last_offset == 0 */

	  if ((~(bv_inst1.bv_array[bv_inst1.bv_array_size - 1]) & spcl_bit_mask) == (bv_block_t)0) {
	    result.bv_array[0]=(bv_block_t)1;
	  }
	  else {
	    /* result = 0 */
	  }
	  return result;
	}
    }
}


bvatom BVManager::reduction_or(bvatom bv_inst1)
{
  bvatom result = getZeroedBVatom((unsigned int) 1);
  bv_block_t spcl_bit_mask = ~mask[block_size-1];
  int flag = 0;
  for(int i = 0; i < bv_inst1.bv_array_size; i++)
    {
      bv_block_t current = (bv_inst1.bv_array[i]) & spcl_bit_mask;
      if(current != (bv_block_t)0)
	{
	  flag = 1;
	  break;
	}
    }
  if(flag == 0) //note carefully
    return result;
  else
    {
      result.bv_array[0] = (bv_block_t)1;
      return result;
    }
  
}

//todo
bvatom BVManager::reduction_nor(bvatom bv_inst1)
{
  bvatom result = reduction_or(bv_inst1);
  if (result.bv_array[0] == (bv_block_t)0) {
    result.bv_array[0] = (bv_block_t)1;
  }
  else {
    result.bv_array[0] = (bv_block_t)0;
  }

  return result;
}

bvatom BVManager::reduction_nand(bvatom bv_inst1)
{
  bvatom result = reduction_and(bv_inst1);
  if (result.bv_array[0] == (bv_block_t)0) {
    result.bv_array[0] = (bv_block_t)1;
  }
  else {
    result.bv_array[0] = (bv_block_t)0;
  }

  return result;
}

bvatom BVManager::reduction_xor(bvatom bv_inst1)
{
  bvatom result = getZeroedBVatom((unsigned int) 1);
  int count_of_ones = 0;
  for(int i = 0; i < bv_inst1.bv_array_size; i++)
    {
      bv_block_t current = bv_inst1.bv_array[i];
      for(int j = 0; j < avail_block_size; j++)
	{
	  if(current%2 == 1)
	    {
	      count_of_ones++;
	    }
	  current=current>>1;
	}
    }
  if(count_of_ones%2 == 1)
    {
      result.bv_array[0]= (bv_block_t)1;
    }

  return result;
}

bvatom BVManager::reduction_xnor(bvatom bv_inst1)
{
  bvatom result = reduction_xor(bv_inst1);
  if (result.bv_array[0] == (bv_block_t)0) {
    result.bv_array[0] = (bv_block_t)1;
  }
  else {
    result.bv_array[0] = (bv_block_t)0;
  }

  return result;
}  


bvatom BVManager::concat(bvatom bv_inst1,bvatom bv_inst2)
{
  unsigned int w1=bv_inst1.bv_width;
  unsigned int w2=bv_inst2.bv_width;

  bvatom result = BVExtend(bv_inst2, w1+w2, 0);
  replaceBySelBVatom(bv_inst1, result, (unsigned int) 0, w2, w1);

  return result;
}


bvatom BVManager::select_with_int(bvatom bv_inst1, unsigned int hi, unsigned int lo)
{
  if (hi < lo) {
    cerr << "ERROR! hi (" << hi << ") of select_with_bv < lo (" << lo << ")" << endl;
    exit(-1);
  }
  bvatom result=getZeroedBVatom((unsigned int) (hi-lo+1));
  replaceBySelBVatom(bv_inst1, result, lo, (unsigned int) 0, (unsigned int) (hi-lo+1));
  return result;

}

bvatom BVManager::select_with_bv(bvatom bv_inst1, bvatom bv_hi, bvatom bv_lo)
{
  unsigned int hi=convertBVToUnsignedInt(bv_hi);
  unsigned int lo=convertBVToUnsignedInt(bv_lo);
  if (hi < lo) {
    cerr << "ERROR! hi (" << hi << ") of select_with_bv < lo (" << lo << ")" << endl;
    exit(-1);
  }
  bvatom result = select_with_int(bv_inst1, hi, lo);
  return result;
}


bool BVManager::checkBVGreaterThan(bvatom bv_inst1, bvatom bv_inst2)
{
  if(bv_inst1.bv_width != bv_inst2.bv_width)
    {
      cerr<<"ERROR!! greater than error\n";
      exit(-1);
    }
  else
    {
      for(int i = bv_inst1.bv_array_size-1; i >= 0; i--)
	{
	  if (bv_inst1.bv_array[i] > bv_inst2.bv_array[i]) {
	    return true;
	  }
	  if (bv_inst1.bv_array[i] < bv_inst2.bv_array[i]) {
	    return false;
	  }
	}
      return false;
    }

}

bool BVManager::checkBVEquality(bvatom bv_inst1, bvatom bv_inst2)
  {
  if(bv_inst1.bv_width!=bv_inst2.bv_width)
    {
      cerr<<"ERROR!! greater than error\n";
      exit(-1);
    }
  else
    {
      for(int i = bv_inst1.bv_array_size-1; i >= 0; i--)
	{
	  if(bv_inst1.bv_array[i] != bv_inst2.bv_array[i])
	    return false;
	}
      return true;
    }

  }

bool BVManager::checkBVForZero(bvatom bv_inst1)
{
      for(int i = bv_inst1.bv_array_size - 1; i >= 0; i--)
	{
	  if(bv_inst1.bv_array[i] != (bv_block_t)0)
	    return false;
	}
      return true;

}


string BVManager::convertBVToHexString(bvatom bv_inst)
{
  return convertBVToHexString_Internal(bv_inst.bv_array, bv_inst.bv_array_size * block_size);
}


string BVManager::convertBVToHexString_Internal(bv_block_t *bvarray, unsigned int width)
{
  if (width == 0) {
    cerr << "ERROR! Can't convert bitvector of length 0." << endl;
    exit(-1);
  }

  static unsigned int hex_width;
  static unsigned int hex_blocks_in_block;
  static bv_block_t * hex_blocks_mask;
  static int called_once = 0;

  if (called_once == 0) {
    hex_width = 4;
    hex_blocks_in_block = block_size/hex_width;
    hex_blocks_mask = new bv_block_t[hex_blocks_in_block];
    called_once = 1;

    for (int i = 0; i < hex_blocks_in_block; i++) {
      hex_blocks_mask[i] = (bv_block_t) 0;
      for (int j = 0; j < hex_width; j++) {
	hex_blocks_mask[i] = hex_blocks_mask[i] | mask[i*hex_width + j];
      }
    }
  }

  if (block_size % hex_width != 0) {
    cerr << "ERROR! Block size " << block_size;
    cerr << " not a multiple of hex_width " << hex_width << endl;
    exit(-1);
  }

  if ((width % hex_width) != 0) {
    width += (hex_width - (width % hex_width));
    /* This makes width a multiple of hex_width */
  }
    

  int hex_blocks_mask_index;
  if (width % block_size == 0) {
    hex_blocks_mask_index = hex_blocks_in_block - 1;
  }
  else {
    hex_blocks_mask_index = (width % block_size)/hex_width - 1;
  }

  string str_result="";
  char word[10];
  string word_str;

  for (int i = width-1; i >= 0; i = i - hex_width) {
    unsigned int block = i / block_size;

    bv_block_t curr_val = bvarray[block] & hex_blocks_mask[hex_blocks_mask_index];
    if (hex_blocks_mask_index != 0) {
      curr_val = curr_val >> (hex_blocks_mask_index * hex_width);
      hex_blocks_mask_index--;
    }
    else {
      hex_blocks_mask_index = (block_size/hex_width - 1);
    }

    sprintf(word, "%x", curr_val);
    word_str = word;
    str_result += word_str;
  }

  return str_result;
}
