

/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/



#include "Eliminator.h"
#include "ILP_Eliminator.h"
#include "BitBlaster.h"
#include "NewBV.cc"

// Hashtable for getDAGNames

HashTable <string, set<string> > GetDagNames_HT;

extern time_t Simulator_Time_In_ImplicationCheck;
BVManager* bvm = new BVManager();

// return Additive Inverse of the first input modulo the second input
ULINT aInverse(ULINT number, ULINT mod)
{
if(number<0 || number>=mod) {cout<<"\nNumber "<<number<<" more than mod or less than zero inside function aInverse. Exiting...\n"; exit(1);}
 if(number==0) return 0; 
return mod-number;
}

// return the power of 2 in number. Eg: if number=4, return 2
ULINT powerOfTwo(ULINT number)
{
  ULINT power=0;
  while(number%2==0) {number=number/2; power++;}
  return power;    
}

// return 2 ^ number
ULINT findPower(ULINT number)
{
  ULINT count=0;
  ULINT value=1;
  while(count<number) {count=count+1; value=value*2;}
  return value;
}

// return Multiplicative Inverse of the first input modulo the second input
ULINT mInverse(ULINT number, ULINT mod)
{
if(number<0 || number>=mod) {cout<<"\nNumber "<<number<<" more than mod or less than zero inside function mInverse. Exiting...\n"; exit(1);} 
if(number%2==0) {cout<<"\nNumber "<<number<<" is even. Hence no multiplicative inverse. Exiting...\n"; exit(1);}
ULINT inverse=number;
while (number != 1)
	{
	number = (number*number)%mod;
	inverse = (inverse*number)%mod;
	}
return inverse;
}

// return a+b modulo mod
ULINT add(ULINT a, ULINT b, ULINT mod)
{
if(a<0 || a>=mod) {cout<<"\nNumber "<<a<<" more than mod or less than zero inside function add\n"; exit(1);} 
if(b<0 || b>=mod) {cout<<"\nNumber "<<b<<" more than mod or less than zero inside function add\n"; exit(1);} 
return (a+b)%mod;
}

// return a-b modulo mod
ULINT sub(ULINT a, ULINT b, ULINT mod)
{
if(a<0 || a>=mod) {cout<<"\nNumber "<<a<<" more than mod or less than zero inside function sub\n"; exit(1);} 
if(b<0 || b>=mod) {cout<<"\nNumber "<<b<<" more than mod or less than zero inside function sub\n"; exit(1);} 
return (a+aInverse(b,mod))%mod;
}

// return a*b modulo mod
ULINT mul(ULINT a, ULINT b, ULINT mod)
{
if(a<0 || a>=mod) {cout<<"\nNumber "<<a<<" more than mod or less than zero inside function mul\n"; exit(1);} 
if(b<0 || b>=mod) {cout<<"\nNumber "<<b<<" more than mod or less than zero inside function mul\n"; exit(1);} 
return (a*b)%mod;
}

// return a/b modulo mod. Note than a/b mod is defined only if b has a MI.i.e. if b is odd
ULINT div(ULINT a, ULINT b, ULINT mod)
{
if(a<0 || a>=mod) {cout<<"\nNumber "<<a<<" more than mod or less than zero inside function div\n"; exit(1);} 
if(b<0 || b>=mod) {cout<<"\nNumber "<<b<<" more than mod or less than zero inside function div\n"; exit(1);} 
if(b%2==0) {cout<<"\nNumber "<<b<<" which is divisor is even. Hence no multiplicative inverse and no division possible. Exiting...\n"; exit(1);}
return (a*mInverse(b,mod))%mod;
}


// Show the number of zero, non-zero elements in the matrix
void countZeroNonZeroElements(Matrix &MyMatrix)
{
int zero_count=0;
int nonzero_count=0;
    
int rows = MyMatrix.size();

int cols = 0;
if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

for(int i=0; i<rows; i++)
		{
		for(int j=0; j<cols; j++)
			{
                        if(MyMatrix[i][j] == 0)
                            {
                            zero_count++;
                            }
                        else
                            {
                            nonzero_count++;
                            }
			}//for
		}//for
cout<<"\nzero_count = "<<zero_count<<"\tnonzero_count = "<<nonzero_count<<endl;
}



// Show the matrix
void showMatrix(Matrix &MyMatrix)
{
int rows = MyMatrix.size();

int cols = 0;
if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

for(int i=0; i<rows; i++)
		{ cout<<"\n";
		for(int j=0; j<cols; j++)
			{
			cout<<MyMatrix[i][j]<<"\t";
			}
		}
cout<<"\n";
}

// Show Equalities 
void showEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix)
{
int rows = MyCoefMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyCoefMatrix[0].size();
	}

for(int i=0; i<rows; i++)
		{ cout<<"\n\n\n";
		for(int j=0; j<cols; j++)
			{
			cout<<MyCoefMatrix[i][j]<<"x"<<j;
			if(j<cols-1) cout<<" + ";
			}
		cout<<" = "; 
		cout<<MyConstMatrix[i][0];
		}
cout<<"\n";
}


// Show the disequations 
void showDisEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix) 
{
int rows = MyCoefMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyCoefMatrix[0].size();
	}

for(int i=0; i<rows; i++)
		{ cout<<"\n\n\n";
		for(int j=0; j<cols; j++)
			{
			cout<<MyCoefMatrix[i][j]<<"x"<<j;
			if(j<cols-1) cout<<" + ";
			}
		cout<<" <> "; 
		cout<<MyConstMatrix[i][0];
		}
cout<<"\n";
}

// Print Equalities
void printEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix, FILE *fp)
{
int rows = MyCoefMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyCoefMatrix[0].size();
	}

for(int i=0; i<rows; i++)
		{ 
		for(int j=0; j<cols; j++)
			{
			fprintf(fp,"%llux%llu", MyCoefMatrix[i][j], j);
			if(j<cols-1) fprintf(fp," + ");
			}
		fprintf(fp, " = "); 
		fprintf(fp, "%llu", MyConstMatrix[i][0]);
	        fprintf(fp, ", ");
		}
}


// Print the disequations 
void printDisEquations(Matrix &MyCoefMatrix, Matrix &MyConstMatrix, FILE *fp) 
{
int rows = MyCoefMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyCoefMatrix[0].size();
	}

for(int i=0; i<rows; i++)
		{ 
		for(int j=0; j<cols; j++)
			{
			fprintf(fp,"%llux%llu", MyCoefMatrix[i][j], j);
			if(j<cols-1) fprintf(fp," + ");
			}
		fprintf(fp, " <> "); 
		fprintf(fp, "%llu", MyConstMatrix[i][0]);
	        fprintf(fp, ", ");
		}
}


// Get two matrices from an input file : coefficient and constant matrices
// Both have same no: of rows and no: of columns of coefficient matrix is cols
// constant matrix has 1 column : But constant is (last entry - second last entry)
void getEquations(Matrix &MyMatrixCoef, Matrix &MyMatrixConst, int rows, int cols, ULINT Mod)
{
ULINT element;
ULINT const1, const2;
for(int i=0; i<rows; i++)
		{ 
		  //cout<<"\nRow"<<i<<endl;  
		Row MyRow;
		for(int j=0; j<cols; j++)
			{
			 cin>>element;
			 MyRow.push_back(element);
			}
		MyMatrixCoef.push_back(MyRow);

		cin>>const1;
		cin>>const2;
		
		//cout<<"\nconst1 = "<<const1<<"\tconst2 = "<<const2<<endl;
		element=sub(const2, const1, Mod);
		MyRow.clear();
		MyRow.push_back(element);
		MyMatrixConst.push_back(MyRow);
		}
}



// Get two matrices from an input file : coefficient and constant matrices
// Both have same no: of rows and no: of columns of coefficient matrix is cols
// constant matrix has 1 column : But constant is (last entry - second last entry)
void getDisequations(Matrix &MyMatrixCoef, Matrix &MyMatrixConst, int rows, int cols, ULINT Mod)
{
ULINT element;
ULINT const1, const2;
for(int i=0; i<rows; i++)
		{ 
		  //cout<<"\nRow"<<i<<endl;  
		Row MyRow;
		for(int j=0; j<cols; j++)
			{
			cin>>element;
			MyRow.push_back(element);
			}
		MyMatrixCoef.push_back(MyRow);

		cin>>const1;
		cin>>const2;

		//cout<<"\nconst1 = "<<const1<<"\tconst2 = "<<const2<<endl;
		element=sub(const2, const1, Mod);
		MyRow.clear();
		MyRow.push_back(element);
		MyMatrixConst.push_back(MyRow);
		}
}



// Get a matrix from key board
void getMatrix(Matrix &MyMatrix, int rows, int cols)
{
ULINT element;
for(int i=0; i<rows; i++)
		{ Row MyRow;
		for(int j=0; j<cols; j++)
			{
			cin>>element;
			MyRow.push_back(element);
			}
		MyMatrix.push_back(MyRow);
		}
}

// Get the index of the equation with any odd coef for variable indexed by VarToElimIndex
void getEquationWithOddCoefForGivenVariable(Matrix &EqCoefMat, int VarToElimIndex, int &EqnToSubstIndex)
{
int rows = EqCoefMat.size();

EqnToSubstIndex=-1;

for(int i=0; i<rows; i++)
			{
			if(EqCoefMat[i][VarToElimIndex]%2!=0) // if it is odd
				{
				EqnToSubstIndex=i;
				return;
				}
			}
}


// Get the index of the equation with least even coef for variable indexed by VarToElimIndex
void getEquationWithLeastEvenCoefForGivenVariable(Matrix &EqCoefMat, int VarToElimIndex, int &EqnToSubstIndex, ULINT Mod)
{
EqnToSubstIndex=-1;

int rows = EqCoefMat.size();

// We need to find the least even element in the VarToElimIndex column of the matrix EqCoefMat
 Matrix MyColumn;
  
 if(EqCoefMat.empty())
	return;
 else
     	copyColumn(EqCoefMat, VarToElimIndex, MyColumn); // selected column copied into MyColumn

 if(IsNullMatrix(MyColumn)) 
   {
     //cout<<"\nNo equation with a term containing x"<<VarToElimIndex<<"\n"; 
     return;
   }
 else
   {
     ULINT SelectedPower, MyPower;
     if(MyColumn[0][0]==0) {SelectedPower=Mod; EqnToSubstIndex=0;} else {SelectedPower=powerOfTwo(MyColumn[0][0]); EqnToSubstIndex=0; }// In order to take care of coefficient = 0, we set the power to mod which is something like +infinity
     for(int i=1; i<rows; i++)
       {
	 if(MyColumn[i][0]==0) MyPower=Mod; else MyPower=powerOfTwo(MyColumn[i][0]); 
	 if(MyPower<SelectedPower) 
				{
				  SelectedPower=MyPower;
				  EqnToSubstIndex=i;
				}
       }
   }
}

// Nullify column indexed by  SelColumn 
void nullifyColumn(Matrix &A, int SelColumn)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

for(int i=0; i<rows; i++)
		{
		  A[i][SelColumn] = 0;
		}
}


// Divide column indexed by  SelColumn by power(2,SelectedPower) if divisible. If not divisible set to zero
void divideColumnByPowerOf2IfDivisible(Matrix &EqCoefMat, int SelColumn, ULINT SelectedPower)
{
int rows = EqCoefMat.size();
int cols = 0;

if(rows != 0)
	{
	cols = EqCoefMat[0].size();
	}

for(int i=0; i<rows; i++)
		{
		  if( EqCoefMat[i][SelColumn] % findPower(SelectedPower) != 0) // Not divisible
			EqCoefMat[i][SelColumn] = 0;
		  else
		  	EqCoefMat[i][SelColumn] = EqCoefMat[i][SelColumn]/findPower(SelectedPower);
		}
}

// In the column indexed by SelColumn if the entry is divisible by power(2,SelectedPower) then make it zero (we have eliminated the variable indexed by SelColumn from it), else keep it
void modifyDisEqCoefMat(Matrix &DisEqCoefMat, int SelColumn, ULINT SelectedPower)
{
int rows = DisEqCoefMat.size();

for(int i=0; i<rows; i++)
		{
		  if(DisEqCoefMat[i][SelColumn]%findPower(SelectedPower)==0)
			DisEqCoefMat[i][SelColumn] = 0;
		}
}

//  Function to push back the the selectedIndexth row into NewMatrix. 
void copyRow(Matrix &MyMatrix, int selectedIndex, Matrix &NewMatrix)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

ULINT element;
Row MyRow;
if(selectedIndex<0 || selectedIndex>=rows) {cout<<"\nInvalid row "<<selectedIndex<<" in function copyRow\n"; exit(1);}
for(int j=0; j<cols; j++)
			{
			element=MyMatrix[selectedIndex][j];
			MyRow.push_back(element);
			}
NewMatrix.push_back(MyRow); 
}

//  Function to copy the the selectedIndexth column into MyCoulmnMatrix. Initially MyColumnMatrix is empty
void copyColumn(Matrix &MyMatrix, int selectedIndex, Matrix &MyColumnMatrix)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

ULINT element;
//if(selectedIndex<0 || selectedIndex>=cols) {cout<<"\nInvalid column "<<selectedIndex<<" in function copyColumn\n"; exit(1);}
for(int i=0; i<rows; i++)
			{
			Row MyRow;
			element=MyMatrix[i][selectedIndex];
			MyRow.push_back(element);
			MyColumnMatrix.push_back(MyRow); 
			}
}

//  Function to copy the MyMatrix into MyMatrixCopy
void copyMatrix(Matrix &MyMatrix, Matrix &MyMatrixCopy)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

ULINT element;

for(int i=0; i<rows; i++)
		{ Row MyRow;
		for(int j=0; j<cols; j++)
			{
			element = MyMatrix[i][j];
			MyRow.push_back(element);
			}
		MyMatrixCopy.push_back(MyRow);
		}
}

//Convert MyMatrix to AI(MyMatrix)
void aInverseOfMatrix(Matrix &MyMatrix, ULINT Mod)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      MyMatrix[i][j]=aInverse(MyMatrix[i][j], Mod);
}

// Convert MyMatrix to MyMatrix*Scalar
void scalarMatMultiply(Matrix &MyMatrix, ULINT Scalar, ULINT Mod)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      MyMatrix[i][j]=mul(MyMatrix[i][j], Scalar, Mod);
}

// Multiply matrixes A and B to get C
void matMultiply(Matrix &A, Matrix &B, Matrix &C, ULINT Mod)
{
int RowsA = A.size();
int ColsA = 0;

if(RowsA != 0)
	{
	ColsA = A[0].size();
	}

int RowsB = B.size();
int ColsB = 0;

if(RowsB != 0)
	{
	ColsB = B[0].size();
	}

  int i, j, k;
  ULINT element;
  for(i=0;i<RowsA;i++)
    {
      Row MyRow;
     
      for(j=0;j<ColsB;j++)
	{
	element=0;
	for(k=0;k<ColsA;k++)
	  {
	    element=add(element, mul(A[i][k], B[k][j], Mod), Mod);
	  }
	MyRow.push_back(element);
	}
      C.push_back(MyRow);
    }
}



// A = A + B where A, B are matrixes
void matAdd(Matrix &A, Matrix &B, ULINT Mod)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      A[i][j]=add(A[i][j], B[i][j], Mod);
}


// A = A - B where A, B are matrixes
void matSub(Matrix &A, Matrix &B, ULINT Mod)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      A[i][j]=sub(A[i][j], B[i][j], Mod);
}

// Delete a row from matrix indexed by Index
void deleteRow(Matrix &A, int Index)
{
A.erase(A.begin()+Index);
}

// Delete a column from matrix indexed by Index
void deleteColumn(Matrix &A, int Index)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

int i;
// delete elements A[i][Index] where i=0..(rows-1)
for(i=0;i<rows;i++)
	A[i].erase(A[i].begin()+Index);
}

// Add RowMatrix as a new row into MyMatrix
void addRow(Matrix &MyMatrix, Matrix &RowMatrix)
{
  Row MyRow = RowMatrix[0];
  MyMatrix.push_back(MyRow);
}

// Add all the rows in YourMatrix as a new rows into MyMatrix
void addRows(Matrix &MyMatrix, Matrix &YourMatrix)
{
  int rows = YourMatrix.size();
  for(int i=0;i<rows;i++)
    {
      Row YourRow = YourMatrix[i];
      MyMatrix.push_back(YourRow);
    }
}

// Here MyMatrix is a column matrix. This function adds element as the last element of MyMatrix
void addElementToColumnMatrix(Matrix &MyMatrix, ULINT element)
{
  Row MyRow;
  MyRow.push_back(element);
  MyMatrix.push_back(MyRow);
}

// Check if MyMatrix is a null matrix
int IsNullMatrix(Matrix &MyMatrix)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      {
	if(MyMatrix[i][j]!=0) return 0;
      }
  return 1;
}

// Get the matrix which is the element-wise maximum of matrices in arg1 and arg2
void getMaximumMatrix(Matrix &A, Matrix &B, Matrix &C)
{
int RowsA = A.size();
int ColsA = 0;

if(RowsA != 0)
	{
	ColsA = A[0].size();
	}

int RowsB = B.size();
int ColsB = 0;

if(RowsB != 0)
	{
	ColsB = B[0].size();
	}

if(RowsA != RowsB || ColsA != ColsB) {cout<<"\nMatrix sizes not matching in getMaximumMatrix. Exiting...\n"; exit(1);}

  int i, j;
  ULINT element;

  for(i=0;i<RowsA;i++)
    {
      Row MyRow;
     
      for(j=0;j<ColsA;j++)
	{
	element= (A[i][j]>=B[i][j]?A[i][j]:B[i][j]);
	MyRow.push_back(element);
	}
      C.push_back(MyRow);
    }
}

// Given matrix A and mod, generate matrix B s.t. element in B = mod - corresponding element in A
void convertMatrixToDifferenceOfPowerOf2(Matrix &A, ULINT mod, Matrix &B)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

  int i, j;
  ULINT element;
  for(i=0;i<rows;i++)
	{Row MyRow;
       	for(j=0;j<cols;j++)
      		{
		if(mod<A[i][j]) {cout<<"\nmod is less than element "<<A[i][j]<<" in convertMatrixToDifferenceOfPowerOf2. Exiting...\n"; exit(1);}
		element=mod-A[i][j];
		MyRow.push_back(element);
      		}
	B.push_back(MyRow);
	}
}

// return gcd of a and b
ULINT GCD(ULINT a, ULINT b)
{ 
  if(a==1 || b==1) return 1;
  while(a!=0 && b!=0)
    {
      if(a>b) a=a-b;
      else b=b-a;
    }
  if(a==0) return b;
  else return a;
}
      


// Find GCD of a set of numbers
ULINT findGCD(set<ULINT> &numbers)
{
  ULINT a, b, gcd;
  set<ULINT>::iterator it=numbers.begin();
  a=(*it);
  it++;
  b=(*it);
  it++;
  //cout<<"\nFinding gcd("<<a<<","<<b<<")\n";
  gcd=GCD(a, b);
  //cout<<"\ngcd = "<<gcd<<"\n";

  for(;it!=numbers.end();it++) 
    {
      a=(*it);
      //cout<<"\nFinding gcd("<<gcd<<","<<a<<")\n";
      gcd=GCD(gcd, a);
      //cout<<"\ngcd = "<<gcd<<"\n";
    }
  return gcd;  
}


// Check if selected equation is solvable
int equationSolvable(Matrix &MyMatrix, ULINT Const, ULINT Mod)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}


  if(Const==0) return 1; // gcd divides 0 always - equation solvable

  set<ULINT> Coefficients;
  // Copy coefs from MyMatrix into set<ULINT> Coefficients
  for(int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
      {
	Coefficients.insert(MyMatrix[i][j]);
      }
  Coefficients.insert(Mod);
  
  ULINT gcd = findGCD(Coefficients);

  //cout<<"\ngcd = "<<gcd<<"\n";
  
  if(Const%gcd == 0) // gcd divides constant part - equation solvable
    return 1;
  else
    return 0;
}


// return true if the column indexed by index is all zeros in A
bool columnIsNull(Matrix &A,int index)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

ULINT element;

 if(index>=cols) {cout<<"\nFunction columnIsNull is called with index > #columns of matrix. Exiting...\n"; exit(1);}
 
 for(int i=0; i<rows; i++)
			{
			element = A[i][index];
			if(element != 0) return false;
			}
 return true;
}


// Search MyVector and return index of element
int searchVector(vector<int> &MyVector, int element)
{
int size = MyVector.size();

for(int i=0;i<size;i++)
	if(MyVector[i]==element) return i;
return -1;
}

// show MyVector
void showVector(vector<int> &MyVector)
{
int size = MyVector.size();
 cout<<"\n";
for(int i=0;i<size;i++)
  cout<<MyVector[i]<<"\t";
 cout<<"\n";
}


//  Function to copy the the non-zero elements in the selectedIndexth column into MyMap. Initially MyMap is empty. 
// first field of MyMap is the row index and the second field is the element MyMatrix[first field][selectedIndexth]
// if MyMatrix[first field][selectedIndexth] != 0
void copyColumnToMap(Matrix &MyMatrix, int selectedIndex, map<int, ULINT> &MyMap)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

ULINT element;
if(selectedIndex<0 || selectedIndex>=cols) {cout<<"\nInvalid column "<<selectedIndex<<" in function copyColumnToMap\n"; exit(1);}
for(int i=0; i<rows; i++)
			{
			element=MyMatrix[i][selectedIndex];
			if(element != 0) 
			  {MyMap.insert(make_pair(i, element));}
			}
}

// Function to convert the mapped elements of MyMap to powers of 2. 
void convertCoefToPowerOf2(map<int, ULINT> &MyMap)
{
for(map<int, ULINT>::iterator it=MyMap.begin(); it != MyMap.end(); it++)
  	it->second=powerOfTwo(it->second);
}

// Function to convert the mapped elements of MyMap to number-element
void subtractMappedElementsFromNumber(map<int, ULINT> &MyMap, ULINT number)
{
for(map<int, ULINT>::iterator it=MyMap.begin(); it != MyMap.end(); it++)
  	it->second=number - it->second;
}

// Function to convert the mapped elements of MyMap to element - number
void subtractNumberFromMappedElements(map<int, ULINT> &MyMap, ULINT number)
{
for(map<int, ULINT>::iterator it=MyMap.begin(); it != MyMap.end(); it++)
  	it->second=it->second - number;
}


// Function to find the maximum of the mapped elements of MyMap
ULINT getMaximumOfMappedElement(map<int, ULINT> &MyMap)
{
  ULINT maximum=0;
for(map<int, ULINT>::iterator it=MyMap.begin(); it != MyMap.end(); it++)
  {if(it->second>maximum)  {maximum=it->second;}}
 return maximum; 
}

// Function to find the minimum of the mapped elements of MyMap
ULINT getMinimumOfMappedElement(map<int, ULINT> &MyMap)
{
  map<int, ULINT>::iterator it=MyMap.begin();
  ULINT minimum=it->second;
  for(; it != MyMap.end(); it++)
    {if(it->second<minimum)  {minimum=it->second;} }
 return minimum; 
}


// Function to show MyMap
int showMap(map<int, ULINT> &MyMap)
{
for(map<int, ULINT>::iterator it=MyMap.begin(); it != MyMap.end(); it++)
  cout<<it->first<<"\t"<<it->second<<"\n";
}



// Eliminate entries in MyMap with mapped element = zero
void eliminateEntriesWithZeroMappedElements(map<int, ULINT> &MyMap)
{
  // Find first entries with second entry = 0
  set<int> Disequations;

  for(map<int, ULINT>::iterator it=MyMap.begin(); it != MyMap.end(); it++)
    { 
      if(it->second==0) Disequations.insert(it->first);
    }

  // Eliminate the entries with first element contained in Disequations

  for(set<int>::iterator it= Disequations.begin(); it !=  Disequations.end(); it++)
    { 
      MyMap.erase(*it);
    }
  
}


// Return the value of bit at location loc in the value count
ULINT getTypeOfConstraint(ULINT count, ULINT loc)
{
  ULINT power=findPower(loc);
  ULINT type=count/power; // Let count=3 loc=1 power=2 3/2 gives 1. This is in fact >> by loc times 
  
  if(type%2 == 0) type=0; else type=1;
  return type;
}

// Eliminate the disequations free of the variable to be eliminated from the disequations in
// DisEqCoefMat, DisEqConstMat, and copy them in DisEqFreeOfVarToElimCoefMat, DisEqFreeOfVarToElimConstMat
void eliminateDisequationsFreeOfVarToElimAndCopyInNewMatrix(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int VarToElimIndex, Matrix &DisEqFreeOfVarToElimCoefMat, Matrix &DisEqFreeOfVarToElimConstMat)
{
  list<int> FreeDiseqs;
 for(int i=0; i<DisEqCoefMat.size(); i++)
		{
		  if(DisEqCoefMat[i][VarToElimIndex] == 0) // Disequation i free of variable VarToElimIndex
		    FreeDiseqs.push_back(i);
		}

 FreeDiseqs.reverse();

 // reverse the list so that the indices remain intact even after each deletion

 for(list<int>::iterator it=FreeDiseqs.begin(); it != FreeDiseqs.end(); it++)
   {
     copyRow(DisEqCoefMat, *it, DisEqFreeOfVarToElimCoefMat); // copyRow does not depend on whether DisEqFreeOfVarToElimCoefMat is row matrix or if it is empty
     deleteRow(DisEqCoefMat, *it);
     
     copyRow(DisEqConstMat, *it, DisEqFreeOfVarToElimConstMat); // copyRow does not depend on whether DisEqFreeOfVarToElimConstMat is row matrix or if it is empty
     deleteRow(DisEqConstMat, *it);
   }
}


// This function finds the disequations in which there are no variables left to eliminate, removes them from set of disequations and copies them in a new matrices
void eliminateDisequationsFreeOfAllVarsToElimAndCopyInNewMatrix(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, Matrix &DisEqFreeOfAllVarsToElimCoefMat, Matrix &DisEqFreeOfAllVarsToElimConstMat)
{
  // Take each disequation d and check if Coef[d][v]=0 for v = 0 to NoOfVarsToElim
  
 list<int> FreeDiseqs;
 for(int i=0; i<DisEqCoefMat.size(); i++)
		{
		  bool IsDisEqFree=true;
		  for(int v=0; v<NoVarsToElim; v++)
		    {
		    if(DisEqCoefMat[i][v] != 0) // Disequation i is not free of variable v
		      {
			IsDisEqFree=false;
			break;
		      }
		    }
		  if(IsDisEqFree)
		    { //cout<<"\nDisequation "<<i<<" is free of all variables to be eliminated\n";
		    FreeDiseqs.push_back(i);}
		}

 FreeDiseqs.reverse(); // To delete the higher rows first
 for(list<int>::iterator it=FreeDiseqs.begin(); it != FreeDiseqs.end(); it++)
   {
     //showMatrix(DisEqCoefMat);
     copyRow(DisEqCoefMat, *it, DisEqFreeOfAllVarsToElimCoefMat); // copyRow does not depend on whether DisEqFreeOfVarToElimCoefMat is row matrix or if it is empty
     deleteRow(DisEqCoefMat, *it);
     
     //showMatrix(DisEqConstMat);
     copyRow(DisEqConstMat, *it, DisEqFreeOfAllVarsToElimConstMat); // copyRow does not depend on whether DisEqFreeOfVarToElimConstMat is row matrix or if it is empty
     deleteRow(DisEqConstMat, *it);
   }
}

   
// The function which implements the Layer 1 in CAV'11 paper
void qEliminatorForEquationsAndDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod)
{

// Here first NoVarsToElim of the variables is to be eliminated    


int VarToElimIndex, EqnToSubstIndex;

// NoOfVarsEliminated is the no: of variables from x1,...,xM-1 already elimiated

//cout<<"\nTrying to eliminate variables with odd coefficients first\n";

 for(VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++) 
   {

     if(EqCoefMat.empty()) 
       {
	 //cout<<"\nNo equations left\n"; 
       break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has odd coefficient \n";
     
     getEquationWithOddCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex); 

     if(EqnToSubstIndex==-1) {
                              //cout<<"\nNo equation with odd coefficient for the variable\n";
			      }

     else
	{
	  //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";

	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";

	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnEq, PivotColumnDiseq;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow

	//cout<<"\nChecking if the equation is solvable\n";
	// Check if the Equation selected is solvable. If no, return saying unsat
	bool checking_unsat_of_equation = false;
	if(checking_unsat_of_equation)
		{
		if(!equationSolvable(PivotRow, PivotCoef, Mod))
	  		{cout<<"\nThe equation selected is unsolvable. Hence set of equations is unsat. Exiting...\n"; exit(1);}
		}


	copyColumn(EqCoefMat, VarToElimIndex, PivotColumnEq); // selected column copied into PivotColumnEqn
	
	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	// delete pivot element from pivot column of equations as this would lead to equation with all coefs set to zero 
	deleteRow(PivotColumnEq, EqnToSubstIndex);


	// PivotRow deleted from EqCoefMat
	deleteRow(EqCoefMat, EqnToSubstIndex);
	// PivotRow deleted from EqConstMat
	deleteRow(EqConstMat, EqnToSubstIndex);

	// PivotColumn nullified in EqCoefMat
	nullifyColumn(EqCoefMat, VarToElimIndex);


	//cout<<"\nAfter deletions\n";
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);

	Matrix NewEqCoefMat;
	// Multiply PivotColumnEq and PivotRow to get NewEqCoefMat
	matMultiply(PivotColumnEq, PivotRow, NewEqCoefMat, Mod);
	//cout<<"\nNewEqCoefMat\n";
	//showMatrix(NewEqCoefMat);

	// EqCoefMat =  EqCoefMat + NewEqCoefMat
	matAdd(EqCoefMat, NewEqCoefMat, Mod);
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);


	// Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef*MI(Pivot element) = "<<PivotCoef<<"\n";

	// Convert PivotColumnEq to PivotColumnEq*PivotCoef
	scalarMatMultiply(PivotColumnEq, PivotCoef, Mod);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	// EqConstMat = EqConstMat-PivotColumnEq
	matSub(EqConstMat, PivotColumnEq, Mod);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);

	//cout<<"\nAfter elimination, we have\n";
	//showEquations(EqCoefMat, EqConstMat);


	// For disequations

	if(!DisEqCoefMat.empty())
	  {
	    copyColumn(DisEqCoefMat, VarToElimIndex, PivotColumnDiseq); // selected column copied into PivotColumnDiseqn
	
	    // PivotColumn nullified in DisEqCoefMat
	    nullifyColumn(DisEqCoefMat, VarToElimIndex);
	
	    Matrix NewDisEqCoefMat;
	    // Multiply PivotColumnDiseq and PivotRow to get NewDisEqCoefMat
	    matMultiply(PivotColumnDiseq, PivotRow, NewDisEqCoefMat, Mod);
	    //cout<<"\nNewDisEqCoefMat\n";
	    //showMatrix(NewDisEqCoefMat);
	    
	    // DisEqCoefMat =  DisEqCoefMat + NewDisEqCoefMat
	    matAdd(DisEqCoefMat, NewDisEqCoefMat, Mod);
	    //cout<<"\nDisEqCoefMat\n";
	    //showMatrix(DisEqCoefMat);
	

	    // Convert PivotColumnDiseq to PivotColumnDiseq*PivotCoef
	    //cout<<"\nPivotColumnDiseq*PivotCoef\n";
	    scalarMatMultiply(PivotColumnDiseq, PivotCoef, Mod);
	    //cout<<"\nPivotColumnDiseq\n";
	    //showMatrix(PivotColumnDiseq);
	    
	    // DisEqConstMat = DisEqConstMat - PivotColumnDiseq
	    matSub(DisEqConstMat, PivotColumnDiseq, Mod);
	    //cout<<"\nDisEqConstMat\n";
	    //showMatrix(DisEqConstMat);
	    
	    //cout<<"\nAfter elimination, we have\n";
	    //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
	  }
	
	}
   }

 //cout<<"\nTrying to eliminate variables with even coefficients\n";

 for(int VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++)
   {

     if(EqCoefMat.empty()) 
       {
	 //cout<<"\nNo equations left\n"; 
	 break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has even coefficient \n";

     getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex, Mod); 
	
     //cout<<"\nEqnToSubstIndex= "<<EqnToSubstIndex<<endl;

    if(EqnToSubstIndex==-1) continue;

     if(EqCoefMat[EqnToSubstIndex][VarToElimIndex] == 0) 
	{cout<<"\nNo equation with even coefficient for the variable.\n";}

     else
	{
	  //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";
		
	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";
	//cout<<"\nDividing Pivot Element by it's power of 2\n";
	
	ULINT PowerOfPivotElement=powerOfTwo(PivotElement);
	//cout<<"\nPower Of Pivot Element = "<<PowerOfPivotElement<<"\n";

	PivotElement=PivotElement/findPower(PowerOfPivotElement);
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";
	
	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnEq, PivotColumnDiseq;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	

	// Check if the Equation selected is solvable. If no, return saying unsat
	bool checking_unsat_of_equation = false;
	if(checking_unsat_of_equation)
		{
		if(!equationSolvable(PivotRow, PivotCoef, Mod))
		  	{cout<<"\nThe equation selected is unsolvable. Hence set of equations is unsat. Exiting...\n"; exit(1);}
		}

	
	// As the coeff. selected is even, we need to add a new equality into the EqCoefMat

	Matrix NewEquality; // Note that this is a row matrix
	
	copyRow(EqCoefMat, EqnToSubstIndex, NewEquality); // selected equation copied into NewEquality
     
	// Pivot element nullified in  NewEquality 
	nullifyColumn(NewEquality, VarToElimIndex);

	// Convert NewEquality to AI(NewEquality)
	aInverseOfMatrix(NewEquality, Mod); 

	// Convert NewEquality to AI(NewEquality)*pow(2, powerOfTwo(Mod)-PowerOfPivotElement)
	scalarMatMultiply(NewEquality, findPower(powerOfTwo(Mod)-PowerOfPivotElement), Mod);

	// The constant part of the new equality in NewEqualityConst
	ULINT NewEqualityConst=PivotCoef;

	// Convert NewEqualityConst to NewEqualityConst*pow(2, powerOfTwo(Mod)-PowerOfPivotElement)
	NewEqualityConst = mul(NewEqualityConst, findPower(powerOfTwo(Mod)-PowerOfPivotElement), Mod);

	// Convert NewEqualityConst to AI(NewEqualityConst*pow(2, powerOfTwo(Mod)-PowerOfPivotElement))
	NewEqualityConst = aInverse(NewEqualityConst, Mod);
	// Now NewEquality = NewEqualityConst is a new equality to be added to EqCoefMat at the end of eliminating the variable

	
	// Rest is exactly like the way for odd coefficient
	
	copyColumn(EqCoefMat, VarToElimIndex, PivotColumnEq); // selected column copied into PivotColumnEqn
	
	// Dividing the PivotColumnEq by power(2, PowerOfPivotElement). Note that PivotColumnEq is always divisible
	divideColumnByPowerOf2IfDivisible(PivotColumnEq, 0, PowerOfPivotElement);
	
	
	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	// delete pivot element from pivot column of equations as this would lead to equation with all coefs set to zero 
	deleteRow(PivotColumnEq, EqnToSubstIndex);


	// PivotRow deleted from EqCoefMat
	deleteRow(EqCoefMat, EqnToSubstIndex);
	// PivotRow deleted from EqConstMat
	deleteRow(EqConstMat, EqnToSubstIndex);

	// PivotColumn nullified in EqCoefMat
	nullifyColumn(EqCoefMat, VarToElimIndex);
	
	

	//cout<<"\nAfter deletions\n";
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);

	Matrix NewEqCoefMat;
	// Multiply PivotColumnEq and PivotRow to get NewEqCoefMat
	matMultiply(PivotColumnEq, PivotRow, NewEqCoefMat, Mod);
	//cout<<"\nNewEqCoefMat\n";
	//showMatrix(NewEqCoefMat);

	// EqCoefMat =  EqCoefMat + NewEqCoefMat
	matAdd(EqCoefMat, NewEqCoefMat, Mod);
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);


	// Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef*MI(Pivot element) = "<<PivotCoef<<"\n";

	// Convert PivotColumnEq to PivotColumnEq*PivotCoef
	scalarMatMultiply(PivotColumnEq, PivotCoef, Mod);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	// EqConstMat = EqConstMat-PivotColumnEq
	matSub(EqConstMat, PivotColumnEq, Mod);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);

	//cout<<"\nAfter elimination, we have\n";
	//showEquations(EqCoefMat, EqConstMat);


	//cout<<"\nThe new equality is to be added\n";	
	//cout<<"\nChecking if new equality is to be added as the coefficient is even\n";
	 // Now NewEquality = NewEqualityConst is a new equality to be added to EqCoefMat and EqConstMat at the end of eliminating the variable unless all coefs in NewEquality are zero

     if(!IsNullMatrix(NewEquality))
       {
         cout<<"\nEquality is added from QElimSingleLME\n";
	 addRow(EqCoefMat, NewEquality); // NewEquality is a matrix
	 addElementToColumnMatrix(EqConstMat, NewEqualityConst);
	}
     else
       {
	 if(NewEqualityConst==0)
	   {cout<<"\nNew equality not added as it is 0 = 0\n";}
	 else
	   {cout<<"\nNull matrix <> 0 inside qEliminator function. Exiting...\n"; exit(1);}
       }

     //cout<<"\nAfter adding new equality, we have\n";
     //showEquations(EqCoefMat, EqConstMat);

	// For disequations

       
     if(!DisEqCoefMat.empty())
       {
	
	 copyColumn(DisEqCoefMat, VarToElimIndex, PivotColumnDiseq); // selected column copied into PivotColumnDiseqn
     
         // Dividing the PivotColumnDiseq by power(2, PowerOfPivotElement). Note that PivotColumnDiseq is not always divisible. If not divisible, element is set to zero
	divideColumnByPowerOf2IfDivisible(PivotColumnDiseq, 0, PowerOfPivotElement);

	// For DisEqCoefMat, in the column corresponding to the variable to be eliminated, if entry is not divisible by 2^PowerOfPivotElement, then keep it. Else make it zero.
	modifyDisEqCoefMat(DisEqCoefMat, VarToElimIndex, PowerOfPivotElement);	

	Matrix NewDisEqCoefMat;
	// Multiply PivotColumnDiseq and PivotRow to get NewDisEqCoefMat
	matMultiply(PivotColumnDiseq, PivotRow, NewDisEqCoefMat, Mod);
	//cout<<"\nNewDisEqCoefMat\n";
	//showMatrix(NewDisEqCoefMat);
	
	// DisEqCoefMat =  DisEqCoefMat + NewDisEqCoefMat
	matAdd(DisEqCoefMat, NewDisEqCoefMat, Mod);
	//cout<<"\nDisEqCoefMat\n";
	//showMatrix(DisEqCoefMat);
	

	// Convert PivotColumnDiseq to PivotColumnDiseq*PivotCoef
	scalarMatMultiply(PivotColumnDiseq, PivotCoef, Mod);
	//cout<<"\nPivotColumnDiseq\n";
	//showMatrix(PivotColumnDiseq);
	
	// DisEqConstMat = DisEqConstMat - PivotColumnDiseq
	matSub(DisEqConstMat, PivotColumnDiseq, Mod);
	//cout<<"\nDisEqConstMat\n";
	//showMatrix(DisEqConstMat);
	
	//cout<<"\nAfter elimination, we have\n";
	//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
       }

	}
   }
  
}


// Function to eliminate a single variable from a conjunction of LMEs and LMDs.
// Returns the result as a DAG
DAGNode* qEliminatorReturningDAG(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable)
{

  time_t start, end, duration;
  unsigned int number_of_lmes, number_of_lmds, number_of_bits;
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;
  unsigned int number_of_vars;

  
  bool show_zero_nonzero_elements = false;
  if(show_zero_nonzero_elements)
    {
    cout<<"\nEqCoefMatPassed\n"  ;
    countZeroNonZeroElements(EqCoefMatPassed);
    cout<<"\nDisEqCoefMatPassed\n"  ;
    countZeroNonZeroElements(DisEqCoefMatPassed);
    }
    
    
  
  getNumberOfBitsInVarToElim(number_of_bits, VarNames, WidthTable);

  // Delete the duplicate contraints

  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  //cout<<"\nShowing original set of constraints\n";
  //showEquations(EqCoefMatPassed, EqConstMatPassed); // Show the equations
  //showDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed); // Show the disequations
  //cout<<"\nWe need to eliminate the first "<<NoVarsToElim<<" variables \n";


  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  
  //cout<<"\nShowing duplicate eliminated set of constraints\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

  update_APQElim_Calls();
  getNumberOfLMEsNumberOfLMDs_Layer_1(number_of_lmes, number_of_lmds, EqCoefMat, DisEqCoefMat);
  getLME_LMD_Counts(EqCoefMat.size(),DisEqCoefMat.size());

  // To find the #vars in the ap_qelim instance
  getNumberOfVars(number_of_vars, EqCoefMat, DisEqCoefMat);
  // To find the #vars in the ap_qelim instance ends here
  // To find some additional details about ap_qelim instance
  getAP_QElimDetails(number_of_lmes, number_of_lmds, number_of_vars);
  // To find some additional details about ap_qelim instance ends here

  DAGNode *resultDAG=NULL;

  Matrix OriginalEqCoefMat, OriginalEqConstMat; // Copy of equalities

  // Keep a copy of equations in OriginalEqCoefMat, OriginalEqConstMat
  copyMatrix(EqCoefMat, OriginalEqCoefMat);
  copyMatrix(EqConstMat, OriginalEqConstMat);
  
  //cout<<"\n************************************************************\n";

//   if(!DisEqCoefMat.empty())
// 	{
//   	cout<<"\nEqualities and Disequalities at the beginning of the function\n";
//   	cout<<"\nShowing the Equalities\n";
//   	showEquations(EqCoefMat, EqConstMat); // Show the equations
//   	cout<<"\nShowing the Disequalities\n";
//   	showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
// 	showList(VarNames, "VarNames");
// 	}
  

  //cout<<"\nEliminating variables from Equations and Disequations\n";

  time(&start); 

  gettimeofday (&start_ms, NULL);
  
  if(!EqCoefMat.empty())
    qEliminatorForEquationsAndDisequations(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod); // Eliminate first NoVarsToElim variables from the disequations CX<>D and equations AX=B and get back the matrices

  gettimeofday (&finish_ms, NULL);

  time(&end); 

  duration = end - start;

  time_in_layer1 = time_in_layer1 + duration;

  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

  print_Layer_i_Calls(number_of_lmes, number_of_lmds, number_of_bits, duration, 1, duration_ms);


  //cout<<"\nShowing the Equalities\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nShowing the Disequalities\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations


  
  update_APQElim_Calls_Solved_By_QElimLMA(DisEqCoefMat, NoVarsToElim); 
  getNumberOfLMEsNumberOfLMDs_Layer_2(number_of_lmes, number_of_lmds, OriginalEqCoefMat, DisEqCoefMat, NoVarsToElim);

  //cout<<"\nDropping the unconstraining disequations\n";
  // Note that for dropping unconstraining disequations, we should look at the maximum no: of bits constrained by any equality. The only thing we want from the set of equations in fact is the most constraining equality (Look at the proof for further clarification)


  time(&start); 

  gettimeofday (&start_ms, NULL);
  

  if(!DisEqCoefMat.empty())
    	{
	dropUnconstrainingDisequationsModified(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);
	}

  gettimeofday (&finish_ms, NULL);

  time(&end); 

  duration = end - start;

  time_in_layer2 = time_in_layer2 + duration;

  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;


  print_Layer_i_Calls(number_of_lmes, number_of_lmds, number_of_bits, duration, 2, duration_ms);



  //cout<<"\nShowing the Equalities\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nShowing the Disequalities\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

  
  update_APQElim_Calls_Solved_By_DropLMDs_1(DisEqCoefMat, NoVarsToElim);


  // At this point, all variables to eliminate from equations are eliminated
  // Variables to eliminate if exists are in disequations

  // Find the first variable still to be eliminated : This is to start with. Later on implement this for all variables
  
  bool EliminationDone=true; // Is elimination already done?

  int VarToElimIndex; // Index of the first variable still to be eliminated 

  if(!DisEqCoefMat.empty())
    {
      for(VarToElimIndex=0; VarToElimIndex<NoVarsToElim; VarToElimIndex++)
	{
	  if(!columnIsNull(DisEqCoefMat, VarToElimIndex))  // Column for variable to eliminate is non null => variable is not eliminated
		    {
		     //cout<<"\nVariable x"<<VarToElimIndex<<" is present in disequations\n";
		     EliminationDone=false;
		     break;
		    }
	}
    }


  // Print the equations and disequations which are free of all the variables to be eliminated


  // This function finds the disequations in which there are no variables left to eliminate, removes them from set of disequations and copies them in a matrices DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat
  
  Matrix DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat;
  
  if(!DisEqCoefMat.empty()) eliminateDisequationsFreeOfAllVarsToElimAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, NoVarsToElim, DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat);

  
  //cout<<"\n###############################\nPrinting the Equalities free of all variables to eliminate\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nPrinting the Disequalities free of all variables to eliminate\n";
  //showDisEquations(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat); // Show the disequations
  //cout<<"\n###############################\n";

  
  eliminateTrivialEquationsAndDisequations(EqCoefMat, EqConstMat, DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat);
  // eliminate trivial disequations and equations like 0X=0 and 0X<>non-zeroconstant

  //showDisEquations(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat); // Show the disequations : Temporary


   DAGNode *DagFromEqs, *DagFromDiseqs;
   DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, VarNames, Mod);
   DagFromDiseqs = getDAGFromDisEqs(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat, VarNames, Mod);
  	
  if(EliminationDone) 
    {
      //cout<<"\nElimination done. Exiting from this call...\n";
      

      if(DagFromEqs != NULL)
	{
	if(resultDAG == NULL) resultDAG=DagFromEqs; 
	else 
	  { 
	    resultDAG = createDAG("and",resultDAG, DagFromEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromDiseqs != NULL)
	{
      	if(resultDAG == NULL) resultDAG=DagFromDiseqs; 
	else 
	  {
	    resultDAG = createDAG("and",resultDAG, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}
     
      return resultDAG;
     }


  cout<<"\nLayer DropLMDs invoked\n";

  //exit(1); // Let's check the following portion

  cout<<"\nWe are left with the following constraints\n";

  cout<<"\n###############################\n";
  showEquations(OriginalEqCoefMat, OriginalEqConstMat); // Show the equations
  showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  cout<<"\n###############################\n";

  eliminateImpliedDisequations(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, Mod, WidthTable, VarNames);
  // disequations implied by the equations and other disequations are eliminated. Further scope for dropping disequations may be there

  cout<<"\nWe are left with the following constraints after eliminateImpliedDisequations\n";

  cout<<"\n###############################\n";
  showEquations(OriginalEqCoefMat, OriginalEqConstMat); // Show the equations
  showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  cout<<"\n###############################\n";

  cout<<"\nChecking if more disequations can be dropped\n";

  if(!DisEqCoefMat.empty())
	{
	dropUnconstrainingDisequationsModified(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);
	}

  update_APQElim_Calls_Solved_By_DropLMDs(DisEqCoefMat, NoVarsToElim);
  update_APQElim_Calls_Given_To_APQElim_Exp(DisEqCoefMat, NoVarsToElim);


  cout<<"\nWe are left with the following constraints after dropping disequations\n";

  cout<<"\n###############################\n";
  showEquations(OriginalEqCoefMat, OriginalEqConstMat); // Show the equations
  showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  cout<<"\n###############################\n";


  EliminationDone=true; // Is elimination already done?
  VarToElimIndex; // Index of the first variable still to be eliminated 

  if(!DisEqCoefMat.empty())
    {
      for(VarToElimIndex=0; VarToElimIndex<NoVarsToElim; VarToElimIndex++)
	{
	  if(!columnIsNull(DisEqCoefMat, VarToElimIndex))  // Column for variable to eliminate is non null => variable is not eliminated
		    {
		     cout<<"\nVariable x"<<VarToElimIndex<<" is present in disequations\n";
		     EliminationDone=false;
		     break;
		    }
	}
    }
 
  if(EliminationDone) 
    {
      cout<<"\nElimination done. Exiting from this call...\n";
      
      if(DagFromEqs != NULL)
	{
	if(resultDAG == NULL) 
	  resultDAG=DagFromEqs; 
	else
	  {
	  resultDAG = createDAG("and",resultDAG, DagFromEqs, LOGICAL_SYMBOL, 1);
	  }
	}

      if(DagFromDiseqs != NULL)
	{
      	if(resultDAG == NULL) 
	  resultDAG=DagFromDiseqs; 
	else 
	  {
	  resultDAG = createDAG("and",resultDAG, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}
     
      return resultDAG;
     }

  

  
  if(!SplitFlag && !OriginalEqCoefMat.empty()) // Disequations are not split already and there are equations
    {
      cout<<"\nSplitting\n";

      //exit(1); // Added to catch splitting
      
      SplitFlag=true;

      DAGNode *resultSplit = splitDisequationsReturningDAG(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, VarToElimIndex, Mod, NoVarsToElim, SplitFlag, VarNames, WidthTable);
	
      

      if(DagFromEqs != NULL)
	{
	  if(resultDAG == NULL) 
	    resultDAG=DagFromEqs; 
	  else 
	    {
	    resultDAG = createDAG("and",resultDAG, DagFromEqs, LOGICAL_SYMBOL, 1);
	    }
	}

       if(DagFromDiseqs != NULL)
      	 {
	   if(resultDAG == NULL) 
	     resultDAG=DagFromDiseqs; 
	   else 
	     {
	     resultDAG = createDAG("and",resultDAG, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	     }
	 }

      if(resultSplit != NULL)
	{
	  if(resultDAG == NULL) 
	    resultDAG=resultSplit; 
	  else 
	    {
	    resultDAG = createDAG("and",resultDAG, resultSplit, LOGICAL_SYMBOL, 1);
	    }
	}
      
      return resultDAG;

    }

    // Else bit-blast

  else 
	{
	cout<<"\nBit-level reasoning needed.\n"; 

	
	DAGNode* resultEagerSplit = eagerSplitReturningDAG(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, VarToElimIndex, Mod, NoVarsToElim, SplitFlag, VarNames, WidthTable);

	resultDAG=resultEagerSplit;

	/* Note that while doing eager splitting we are dividing the problem at this point into a number of simpler problems each of which can be solved by recursively calling qElimReturningDAG. But each simpler problem inherits all the constraints - LMEs and LMDs we have at this point. Hence there is no need to conjoin resultEagerSplit with DagFromEqs and DagFromDisEqs */

	//cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

	return resultDAG;
	}
  
}

// The function which implements the LME based splitting scheme described in the CAV'11 paper
// Returns the result as a DAG
DAGNode* splitDisequationsReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable)
{
DAGNode *resultDAG=NULL;

// Get the equation with the least even coefficient. This is sufficient since all other equations can be eliminated using this
int EqnIndex;
getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnIndex, Mod);
Matrix EqnCoef;
copyRow(EqCoefMat, EqnIndex, EqnCoef);
Matrix EqnConst;
copyRow(EqConstMat, EqnIndex, EqnConst);
// EqnCoef = EqnConst contains the selected equation

// There can be disequations free of the variable to be splitted. They need not be splitted. We will add them to each set of constraints after splitting separately
Matrix DisEqFreeOfVarToElimCoefMat, DisEqFreeOfVarToElimConstMat;
eliminateDisequationsFreeOfVarToElimAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, VarToElimIndex,DisEqFreeOfVarToElimCoefMat, DisEqFreeOfVarToElimConstMat);

//cout<<"\nAfter eliminating free disequations\n";

//cout<<"\nEquation with least coefficient is\n";
//cout<<"\nEqnCoef\n";
//showMatrix(EqnCoef);
//cout<<"\nEqnConst\n";
//showMatrix(EqnConst);

//cout<<"\nShowing the Disequalities Free of the variable\n";
//cout<<"\nDisEqFreeOfVarToElimCoefMat\n";
//showMatrix(DisEqFreeOfVarToElimCoefMat);
//cout<<"\nDisEqFreeOfVarToElimConstMat\n";
//showMatrix(DisEqFreeOfVarToElimConstMat);


//cout<<"\nShowing the Disequalities\n";
//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations



// Find the power of 2 of the variable to be eliminated in the equation selected
ULINT PivotElement=EqnCoef[0][VarToElimIndex]; 
//cout<<"\nPivot Element = "<<PivotElement<<"\n";
ULINT PowerOfPivotElement=powerOfTwo(PivotElement);
//cout<<"\nPower Of Pivot Element = "<<PowerOfPivotElement<<"\n";


// Split each of disequations in DisEqCoefMat <> DisEqConstMat using the equation EqnCoef = EqnConst into two constraints
// Use a counter to generate all the possible combinations of splitted parts

 ULINT max=findPower(DisEqCoefMat.size())-1; // We have 2^(#disequations) combinations possible

 cout<<"\nMax no: iterations = "<<max<<"\n";

 

 for(ULINT count=0;count<=max;count++)
   {
     cout<<"\nStarting loop iteration number "<<count<<" \n";

     Matrix NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat;

     addRow(NewEqCoefMat, EqnCoef); // The already present equation is added to the new constraint
     addRow(NewEqConstMat, EqnConst);

     addRows(NewDisEqCoefMat, DisEqFreeOfVarToElimCoefMat); // The disequations free of variable to be eliminated added to the new constraint
     addRows(NewDisEqConstMat, DisEqFreeOfVarToElimConstMat);
     
     for(int i=0; i<DisEqCoefMat.size(); i++)
		{
		  ULINT WhichConstraint = getTypeOfConstraint(count, i);
		  //cout<<"\nWhichConstraint = "<<WhichConstraint<<"\n";
	          ULINT PivotElementInDiseq=DisEqCoefMat[i][VarToElimIndex]; 
		  ULINT PowerOfPivotElementInDiseq=powerOfTwo(PivotElementInDiseq);
		  ULINT PowerToMultiply = PowerOfPivotElement-PowerOfPivotElementInDiseq;
		  if(WhichConstraint == 0) // Create constraint 1 i.e.reduced disequality
		    {
		      // Row matrices representing costraint1 i.e. reduced disequality
		      Matrix Constraint1_Diseq_Coef;
		      Matrix Constraint1_Diseq_Const;

		      // constraint1's disequality is nothing but the original disequality multiplied by 2^PowerOfPivotElement
		      copyRow(DisEqCoefMat, i, Constraint1_Diseq_Coef);
		      copyRow(DisEqConstMat, i, Constraint1_Diseq_Const);
		      scalarMatMultiply(Constraint1_Diseq_Coef, findPower(PowerToMultiply), Mod);
		      scalarMatMultiply(Constraint1_Diseq_Const, findPower(PowerToMultiply), Mod);

		      addRow(NewDisEqCoefMat, Constraint1_Diseq_Coef); // reduced disequality is added to the new constraint
		      addRow(NewDisEqConstMat, Constraint1_Diseq_Const);
		    }
		  else
		    {
		      	// Row matrices representing constraint2 i.e. original disequality and reduced equality
		      Matrix Constraint2_Eq_Coef;
		      Matrix Constraint2_Eq_Const;
		      Matrix Constraint2_Diseq_Coef;
		      Matrix Constraint2_Diseq_Const;
		      
		      // constraint2's disequality is nothing but the original disequality
		      copyRow(DisEqCoefMat, i, Constraint2_Diseq_Coef);
		      copyRow(DisEqConstMat, i, Constraint2_Diseq_Const);

		      // constraint2's equality is nothing but the original disequality multiplied by 2^PowerOfPivotElement
		      copyRow(DisEqCoefMat, i, Constraint2_Eq_Coef);
		      copyRow(DisEqConstMat, i, Constraint2_Eq_Const);		
		      scalarMatMultiply(Constraint2_Eq_Coef, findPower(PowerToMultiply), Mod);
		      scalarMatMultiply(Constraint2_Eq_Const, findPower(PowerToMultiply), Mod);

		      addRow(NewDisEqCoefMat, Constraint2_Diseq_Coef); // original disequality is added to the new constraint
		      addRow(NewDisEqConstMat, Constraint2_Diseq_Const);
		      addRow(NewEqCoefMat, Constraint2_Eq_Coef); // reduced equality is added to the new constraint
		      addRow(NewEqConstMat, Constraint2_Eq_Const);
		    }
		}
    
     
     //cout<<"\nShowing the Equalities\n";
     //showEquations(NewEqCoefMat, NewEqConstMat); // Show the equations
     // cout<<"\nShowing the Disequalities\n";
     //showDisEquations(NewDisEqCoefMat, NewDisEqConstMat); // Show the disequations

     //cout<<"\nCalling qEliminator recursively\n";

     DAGNode* DAGFromqElim = qEliminatorReturningDAG(NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat, NoVarsToElim, Mod, SplitFlag, VarNames, WidthTable);

     if(DAGFromqElim != NULL)
       {
	 if(resultDAG==NULL) 
	   resultDAG = DAGFromqElim; 
	 else 
	   {
           resultDAG = createDAG("or",resultDAG, DAGFromqElim, LOGICAL_SYMBOL, 1);
	   }
       }
     
   }
   return resultDAG;
}

// Creates a dag from a conjunction of equalities specified as matrices, and returns it
DAGNode* getDAGFromEqs(Matrix &EqCoefMat, Matrix &EqConstMat, list<string> &VarNames, ULINT Mod)
	{

	 struct timeval start_ms, finish_ms;
	 unsigned long long int duration_ms;

	int width;
	width = powerOfTwo(Mod);
	
	list<string>::iterator it;

        DAGNode* result=NULL;
	
	int rows = EqCoefMat.size();
	int cols = 0;

	if(rows != 0)
		{
		cols = EqCoefMat[0].size();
		}

	for(int i=0; i<rows; i++)
		{ 
		if(log_for_per_analysis){
   			gettimeofday (&start_ms, NULL); 
   			}

	        DAGNode *lhs=NULL, *eqn, *rhs;
		it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
			string varname = *it;
			DAGNode *variable = createBVVariable(varname, width);
			
			char coeff[20];
			sprintf(coeff, "%llu", EqCoefMat[i][j]);
			string coeff_str(coeff);

			DAGNode *coefficient = createBVConstant(coeff_str,width);
			DAGNode *term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
			if(lhs==NULL) lhs=term; else lhs=createDAG("bvadd", lhs, term, BITVECTOR_SYMBOL, width);
			}
		char constant[10];
		sprintf(constant, "%llu", EqConstMat[i][0]);
		string constant_str(constant);
		rhs = createBVConstant(constant_str,width);
		eqn = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);
		if(result==NULL) result=eqn; else result = createDAG("and", result, eqn, LOGICAL_SYMBOL, 1);

		if(log_for_per_analysis){ 
   			gettimeofday (&finish_ms, NULL);
   			duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
   			duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
   			//cout<<"\nTime for row "<<i<<" in millisecond = "<<duration_ms<<endl;
   			}


		}
	return result;
	
	}

// Creates a dag from an equality specified as matrices, and returns it
DAGNode* getEquationDAGFromEqn(ULINT SelectedCoef, Matrix &SelectedEqn, ULINT SelectedConst, list<string> &VarNames, string VarToElim, ULINT Mod)
	{
	int width;
	width = powerOfTwo(Mod);

	list<string>::iterator it;

        DAGNode* result=NULL;
	
	int cols = 0;
	if(!SelectedEqn.empty())
		{	
		cols = SelectedEqn[0].size();
		}

	DAGNode *rhs=NULL, *eqn, *lhs;
	it=VarNames.begin();
	for(int j=0; j<cols; j++, it++)
			{
			string varname = *it;
			DAGNode *variable = createBVVariable(varname, width);
			char coeff[20];
			sprintf(coeff, "%llu", SelectedEqn[0][j]);
			string coeff_str(coeff);
			DAGNode *coefficient = createBVConstant(coeff_str,width);
			DAGNode *term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
			if(rhs==NULL) rhs=term; else rhs=createDAG("bvadd", rhs, term, BITVECTOR_SYMBOL, width);
			}
	char constant[20];
	sprintf(constant, "%llu", SelectedConst);
	string constant_str(constant);
	DAGNode *term = createBVConstant(constant_str,width);
	if(rhs==NULL) rhs=term; else rhs=createDAG("bvadd", rhs, term, BITVECTOR_SYMBOL, width);
	

	DAGNode *variable = createBVVariable(VarToElim, width);

        if(SelectedCoef != 1)
		{
		char coeff[20];
		sprintf(coeff, "%llu", SelectedCoef);
		string coeff_str(coeff);
		DAGNode *coefficient = createBVConstant(coeff_str,width);
		term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
		}
	else
		{
		term = variable;
		}
	
	eqn = createDAG("=", term, rhs, LOGICAL_SYMBOL, 1);
	return eqn;
	}


// Creates a dag from a conjunction of disequalities specified as matrices, and returns it	
DAGNode* getDAGFromDisEqs(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, list<string> &VarNames, ULINT Mod)
	{
	int width;
	width = powerOfTwo(Mod);

	list<string>::iterator it;

        DAGNode* result=NULL;
	
	int rows = DisEqCoefMat.size();
	int cols = 0;

	if(rows != 0)
		{
		cols = DisEqCoefMat[0].size();
		}


	for(int i=0; i<rows; i++)
		{ 
	        DAGNode *lhs=NULL, *eqn, *rhs;
	        it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
			string varname = *it;
			DAGNode *variable = createBVVariable(varname, width);
			char coeff[20];

			sprintf(coeff, "%llu", DisEqCoefMat[i][j]);
			string coeff_str(coeff);
			DAGNode *coefficient = createBVConstant(coeff_str,width);
			DAGNode *term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
			if(lhs==NULL) lhs=term; else lhs=createDAG("bvadd", lhs, term, BITVECTOR_SYMBOL, width);
			}
		char constant[10];
		sprintf(constant, "%llu", DisEqConstMat[i][0]);
		string constant_str(constant);
		rhs = createBVConstant(constant_str,width);
		eqn = createDAG("is_not_equal", lhs, rhs, LOGICAL_SYMBOL, 1);
		if(result==NULL) result=eqn; else result = createDAG("and", result, eqn, LOGICAL_SYMBOL, 1);
		}
	return result;
	
	}		

  
// Given number in form 2^k*c return c Eg: if number=4, return 1
ULINT getOddPart(ULINT number)
{
  if(number == 0) {cout<<"\nWarning!Function getOddPart called with number = 0\n"; return 0;}


  ULINT power=0;
  while(number%2==0) {number=number/2; power++;}
  return number;    
}



// This function eliminates duplicate entries from a set of equations/disequations and recreates a new set of constraints
void deleteDuplicateConstraints(Matrix &CoefMat, Matrix &ConstMat, Matrix &NewCoefMat, Matrix &NewConstMat)
{
int rows = CoefMat.size();

for(int i=0; i<rows; i++)
		{ 
		  Row LHS = CoefMat[i];
		  ULINT RHS = ConstMat[i][0];

		  if(NewCoefMat.empty())
		    addConstraint(NewCoefMat, NewConstMat, LHS, RHS);
		  
		  else if(!constraintPresent(NewCoefMat, NewConstMat, LHS, RHS))
		    addConstraint(NewCoefMat, NewConstMat, LHS, RHS);
		  		  
		}
}

// This function adds a new constraint into the set of constraints
void addConstraint(Matrix &NewCoefMat, Matrix &NewConstMat, Row &LHS, ULINT RHS)
{
  NewCoefMat.push_back(LHS);
  Row NewConst;
  NewConst.push_back(RHS);
  NewConstMat.push_back(NewConst);
}

// Checks if the constraint LHS op RHS is already present in the set of constraints
bool constraintPresent(Matrix &NewCoefMat, Matrix &NewConstMat, Row &LHS, ULINT RHS)
{

int rows = NewCoefMat.size();
for(int i=0; i<rows; i++)
		{ 
		  Row MyLHS = NewCoefMat[i];
		  ULINT MyRHS = NewConstMat[i][0];
		  
		  if(rowsEqual(LHS, MyLHS) && RHS==MyRHS)
		    return true;
		}
 return false;
}

// Check if two rows are the same
bool rowsEqual(Row &row1, Row &row2)
{
  int cols1 = row1.size();
  int cols2 = row2.size();

  if(cols1 != cols2) return false;
  
  for(int i=0; i<cols1; i++)
    {
      if(row1[i] != row2[i]) return false;
    }
  return true;
}
  
// Eliminates trivial LMEs and LMDs from the given conjunction of LMEs and LMDs
void eliminateTrivialEquationsAndDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat)
{
   //cout<<"\neliminateTrivialEquationsAndDisequations called\n";

  //void deleteRow(Matrix &A, int Index)

  list<int> RowsToDelete;

  int rows = EqCoefMat.size();
  int SelectedRow;

  for(int i=0; i<rows;i++)
    {
      SelectedRow = i;
      if(rowIsNull(EqCoefMat, SelectedRow) && EqConstMat[SelectedRow][0]==0)
	RowsToDelete.push_back(SelectedRow);
    }

  //cout<<"\nRows in set of equations to delete\n";
  
  //for(set<int>::iterator it=RowsToDelete.begin(); it != RowsToDelete.end(); it++)
  //  {
  //    cout<<*it<<"\t";
  //  }
  //cout<<endl;

  //cout<<"\nDeleting them\n";

  RowsToDelete.reverse(); // reverse so that indices remain intact after deletion

  for(list<int>::iterator it=RowsToDelete.begin(); it!=RowsToDelete.end(); it++)
    {
      deleteRow(EqCoefMat, *it);
      deleteRow(EqConstMat, *it);
    }



  RowsToDelete.clear();

  rows = DisEqCoefMat.size();
  
  for(int i=0; i<rows;i++)
    {
      SelectedRow = i;
      if(rowIsNull(DisEqCoefMat, SelectedRow) && DisEqConstMat[SelectedRow][0]!=0)
	RowsToDelete.push_back(SelectedRow);
    }

  RowsToDelete.reverse();

  ///cout<<"\nRows in set of disequations to delete\n";
  
  //for(list<int>::iterator it=RowsToDelete.begin(); it != RowsToDelete.end(); it++)
  //  {
  //    cout<<*it<<"\t";
  //  }
  // cout<<endl;

  //cout<<"\nDeleting them\n";

  for(list<int>::iterator it=RowsToDelete.begin(); it!=RowsToDelete.end(); it++)
    {
     deleteRow(DisEqCoefMat, *it);
     deleteRow(DisEqConstMat, *it);
    }
  
}


// return true if the row indexed by index is all zeros in A; false otherwise
bool rowIsNull(Matrix &A,int index)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

ULINT element;

 if(index>=rows) {cout<<"\nFunction rowIsNull is called with index > #rows of matrix. Exiting...\n"; exit(1);}
 
 for(int i=0; i<cols; i++)
			{
			element = A[index][i];
			if(element != 0) return false;
			}
 return true;
}


// Function which implements Layer 3 in CAV'11 paper. This takes a conjunction of LMEs and LMDs
// and drops LMDs which are implied by the conjunction of LMEs and the remaining LMDs
void eliminateImpliedDisequations(Matrix EqCoefMat, Matrix EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, ULINT Mod, map<string, int> WidthTable, list<string> VarNames)
{  
  
  while(true)
    {

      //cout<<"\nShowing the Equalities\n";
      //showEquations(EqCoefMat, EqConstMat); // Show the equations
      //cout<<"\nShowing the Disequalities\n";
      //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations



      int DisEqImplied = -1;
      
      int rows=DisEqCoefMat.size();
      for(int i=0; i<rows; i++)
	{
	  if(disEquationImpliedByOthers(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, Mod, WidthTable, i, VarNames)) // check if ith disequation implied by others
	    {
	      DisEqImplied = i;
	      break;
	    }
	}
      
      if(DisEqImplied == -1) // No disequation implied by others
	{
	  break;
	}
      else // ith disequation implied by others. Hence delete it
	{
	  cout<<"\nDeleting the implied disequation\n";

	  deleteRow(DisEqCoefMat, DisEqImplied);
	  deleteRow(DisEqConstMat, DisEqImplied);
	}
    }
}

// Check if the LMD at the row SelectedIndex is implied by LMEs and the other LMDs. Return true if this happens; false otherwise
bool disEquationImpliedByOthers(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, ULINT Mod, map<string, int> WidthTable, int SelectedIndex, list<string> VarNames)
{
  Matrix SelectedDisEqCoefMat,SelectedDisEqConstMat; 

  // Copy the ith disequation into selected disequation
  copyRow(DisEqCoefMat, SelectedIndex, SelectedDisEqCoefMat);
  copyRow(DisEqConstMat, SelectedIndex, SelectedDisEqConstMat);

  // delete the selected disequation from the set of disequations
  deleteRow(DisEqCoefMat, SelectedIndex);
  deleteRow(DisEqConstMat, SelectedIndex);

  char implicationHolds = 'x';

  if(implicationHolds=='t')
    return true;

  if(implicationHolds=='f')
    return false;


  // Forming the dags
  DAGNode*  DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, VarNames, Mod);
  DAGNode*  DagFromDiseqs = getDAGFromDisEqs(DisEqCoefMat, DisEqConstMat, VarNames, Mod);
  
  DAGNode*  resultDAG;
  
  if(DagFromEqs != NULL)
    {
      if(DagFromDiseqs != NULL)
      	{
	  resultDAG = createDAG("and", DagFromEqs, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	}
      else
	{
	  resultDAG = DagFromEqs;
	}
    }
  
  else
    {
      if(DagFromDiseqs != NULL)
      	{
	  resultDAG = DagFromDiseqs;
	}
      else
	{
	  cout<<"\nNo constraint to imply the selected constraint\n";
	  return false;
	}
    } 

  DAGNode*  DagFromSelectedDisEq = getDAGFromDisEqs(SelectedDisEqCoefMat, SelectedDisEqConstMat, VarNames, Mod);
  
  //cout<<"\nChecking if "<<resultDAG->getDAGStr()<<" implies "<<DagFromSelectedDisEq->getDAGStr()<<endl;

  resultDAG = createDAG("and", resultDAG, createDAG("not",DagFromSelectedDisEq, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);

  cout<<"\nDAG formed\n";


  if(internal_solver=="yices")
	{
	FILE *yices_file = fopen("thunsat.ys", "w");

  	writeBVDeclarationsInYicesFormatWithLookup(yices_file, WidthTable, VarNames);
  	string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(resultDAG, WidthTable);
  	fprintf(yices_file, "\n(assert %s )\n", dag_str.c_str());
  	fprintf(yices_file, "\n(check)\n");
  	fclose(yices_file);
  	system("yices -tc thunsat.ys > result_thunsat.txt");

  	cout<<"\nYices called\n"; 

  	FILE *result_yices_file = fopen("result_thunsat.txt", "r");

  	if(result_yices_file == NULL) {cout<<"\nCannot create file result_thunsat.txt\n"; exit(1);}

  	char result[100];
  	fscanf(result_yices_file, "%s", result);
  	fclose(result_yices_file);
  	string result_str(result); 

  	cout<<"\nresult_str = "<<result_str<<endl;

  	if(result_str=="unsat") return true;
  	else if(result_str=="sat") return false;
  	else
    		{
      		cout<<"\nUnknown returned from SMT Solver in function  Eliminator::disEquationImpliedByOthers\n"; exit(1);
    		}
	}

   else if(internal_solver == "stp")
	{
	FILE *smt_file = fopen("thunsat.smt", "w");

  	if(smt_file == NULL)
    		{cout<<"\nCannot create thunsat.smt\n"; exit(1);}

  	fprintf(smt_file,"(benchmark test\n");

  	writeBVDeclarationsInSMTLibFormatWithLookup(smt_file, WidthTable, VarNames);

  	string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(resultDAG, WidthTable);
  
  	fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  	fprintf(smt_file, "\n)\n");

  	fclose(smt_file);

  	system("stp --SMTLIB1 thunsat.smt > result_thunsat.txt"); //call to STP

	cout<<"\nSTP called\n"; 

  	FILE *result_smt_file = fopen("result_thunsat.txt", "r");

  	if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat.txt\n"; exit(1);}

  	char result[100];
  	fscanf(result_smt_file, "%s", result);
  	fclose(result_smt_file);
  	string result_str(result);

  	if(result_str=="unsat") return true;
  	else if(result_str=="sat") return false;
  	else
    		{
      		cout<<"\nUnknown returned from SMT Solver in function  Eliminator::disEquationImpliedByOthers\n"; exit(1);
    		}
	}
    else
	{
	cout<<"\nUnknown internal_solver in function  Eliminator::disEquationImpliedByOthers\n"; exit(1);
	}

}
  
  
// Function to write the declarations of the variables in WidthTable which are appearing in the list "VarNames",
// in Yices format in the file yices_file
void writeBVDeclarationsInYicesFormatWithLookup(FILE *yices_file, map<string, int> WidthTable, list<string> VarNames)

{
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++)
    {
      map<string, int>::iterator wt_it = WidthTable.find(*it);
      if(wt_it == WidthTable.end())
	{
	 cout<<"\nInside function writeBVDeclarationsInYicesFormatWithLookup in Eliminator. No width table entry found for "<<*it<<endl;
	 exit(1);
	}
      
      fprintf(yices_file,"(define %s",(*it).c_str());
      fprintf(yices_file," :: (bitvector %d))\n", wt_it->second);
    }

}


// Function to get a rows X cols null matrix in NullMatrix
void getNullMatrix(Matrix &NullMatrix, int rows, int cols)
{
for(int i=0; i<rows; i++)
		{ 
		Row MyRow;
		for(int j=0; j<cols; j++)
			{
			MyRow.push_back(0);
			}
		NullMatrix.push_back(MyRow);
		}
}  

// Function to get a rows X cols unit matrix in UnitMatrix
void getUnitMatrix(Matrix &UnitMatrix, int rows, int cols)
{
  int element;
for(int i=0; i<rows; i++)
		{ 
		Row MyRow;
		for(int j=0; j<cols; j++)
			{
			  if(i==j)
			    element=1;
			  else
			    element=0;
			MyRow.push_back(element);
			}
		UnitMatrix.push_back(MyRow);
		}
}  

// Makes destination = Source1 concatenat Source2 
// destination is rows X cols matrix where Source1 is a rows X cols1 
// and Sourc2 is a rows X cols2 matrix and cols = cols1 + cols2
void concatenateMatrices(Matrix Source1, Matrix Source2, Matrix &Destination)
{
  int rows = Source1.size();
  int cols1 = 0;

  if(!Source1.empty())
	{
	cols1 = Source1[0].size();
	}

  int cols2 = 0;
  if(!Source2.empty())
	{
	cols2 = Source2[0].size();
	}

  int element;

  for(int i=0; i<rows; i++)
		{ 
		Row MyRow;
		for(int j=0; j<cols1; j++)
		  {
		    element=Source1[i][j];
		    MyRow.push_back(element);
		  }
		for(int j=0; j<cols2; j++)
		  {
		    element=Source2[i][j];
		    MyRow.push_back(element);
		  }
		
		Destination.push_back(MyRow);
		}
}  

// Checks how many non-zero entries are there in the matrix MyMatrix
// Returns the non-zero element if there is only a single non-zero element
// returns -1 otherwise
int getSingleNonZeroEntry(Matrix MyMatrix)
{
  int rows = MyMatrix.size();
  int cols = 0;

  if(rows != 0)
	{
	cols = MyMatrix[0].size(); 
	}
  
  int NoOfNonZeroElements=0;
  int element;
   
  for(int i=0; i<rows; i++)
		{ 
		for(int j=0; j<cols; j++)
		  {
		    if(MyMatrix[i][j] != 0)
		      {
			NoOfNonZeroElements++;
			element=MyMatrix[i][j];
		      }
		  }
		}

  if(NoOfNonZeroElements==1)
    return element;
  else
    return -1;
}

// Function to create a dagnode with label, children, data_type, and width as specified
DAGNode* createDAG(string name, vector<DAGNode *> children, int data_type, int width)
{
  Value *v = new Value();
  Type *t = new Type();
  t->data_type = data_type;
  t->width = width;
  v->set_node_type(*t);
	    
  return dm.createNode(name, children, (void*)v);
}

// Function to create a dagnode with label, children, data_type, and width as specified
DAGNode* createDAG(string name, DAGNode *child1, int data_type, int width)
{
vector<DAGNode *> children;
children.push_back(child1);
return createDAG(name, children, data_type, width);
}

// Function to create a dagnode with label, children, data_type, and width as specified
DAGNode* createDAG(string name, DAGNode *child1, DAGNode *child2, int data_type, int width)
{
vector<DAGNode *> children;
children.push_back(child1);
children.push_back(child2);
return createDAG(name, children, data_type, width);
}

// Function to create a dagnode with label, children, data_type, and width as specified
DAGNode* createDAG(string name, DAGNode *child1, DAGNode *child2, DAGNode *child3, int data_type, int width)
{
vector<DAGNode *> children;
children.push_back(child1);
children.push_back(child2);
children.push_back(child3);
return createDAG(name, children, data_type, width);
}

// Function to create a dagnode for a bit-vector variable with label and width as specified
DAGNode* createBVVariable(string name, int width)
{
  Value *v = new Value();
  Type *t = new Type();
  t->data_type = BITVECTOR_SYMBOL;
  t->width = width;
  v->set_node_type(*t);
			 
  return dm.createNode(name,(void*)v);
}

// Function to create a dagnode for a bit-vector constant with label and width as specified
DAGNode* createBVConstant(string constant, int width)
{
  Value *v = new Value();
  Type *t = new Type();
  t->data_type = BITVECTOR_CONSTANT;
  t->width = width;
  v->set_node_type(*t);
			 
  return dm.createNode(constant,(void*)v);
}

// Function to create a dagnode for a logical variable with label as specified
DAGNode* createLogicalVariable(string name)
{
  Value *v = new Value();
  Type *t = new Type();
  t->data_type = LOGICAL_SYMBOL;
  t->width = 1;
  v->set_node_type(*t);
			 
  return dm.createNode(name,(void*)v);
}

// Function to create a dagnode for a logical constant with label as specified
DAGNode* createLogicalConstant(string constant)
{
  Value *v = new Value();
  Type *t = new Type();
  t->data_type = LOGICAL_CONSTANT;
  t->width = 1;
  v->set_node_type(*t);
			 
  return dm.createNode(constant,(void*)v);
}

// Function to return the content of the dag as a string
string getDAGStr(DAGNode *node)
{
    string dag_str = node->Get_node_name();
    
    if(node->Get_outlist_size()==0) return dag_str;

    dag_str += "(";
    list<DAGNode*> :: iterator iout = node->Get_outlist_start();
    for(unsigned int i=0; i<node->Get_outlist_size(); i++, iout++)
    {
        dag_str += getDAGStr(*iout) + ",";
    }
    dag_str += ")";
    return dag_str;
}

// Function to check if the dag node "parent" has propositional structure
// i.e. the predicates should appear only at the leaves
// exit from the program otherwise
void checkForPropSkelton(DAGNode* parent)
    {
                
        if(parent->Get_outlist_size() != 0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();
            unsigned int outlist_size = parent->Get_outlist_size();
	    string MyName = parent->Get_node_name();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 = *iout;
		    string child_1_name = child_1->Get_node_name();

		    if(MyName == "not")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of not node in the DAG\n";
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }
		    
		    // Do this for the child also

		    checkForPropSkelton(child_1);
		    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;

		    string child_1_name,child_2_name; 
                    
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;

		    child_1_name = child_1->Get_node_name();
		    child_2_name = child_2->Get_node_name();

		    if(MyName == "and" || MyName == "or")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(!Logical(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }

		    else if(MyName == "=" || MyName == "bvadd" || MyName == "bvmul" || MyName == "is_not_equal" || MyName == "bvsub" ||
                               MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt")
		      {
			if(LogicalOperator(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    cout<<"\nNode is "<<getDAGStr(parent)<<endl;
			    exit(1);
			  }

			if(LogicalOperator(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    cout<<"\nNode is "<<getDAGStr(parent)<<endl;
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }

		    checkForPropSkelton(child_1);
		    checkForPropSkelton(child_2);
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

		    string child_1_name,child_2_name, child_3_name; 
                    
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;
		    iout++;
                    child_3 = *iout;

		    child_1_name = child_1->Get_node_name();
		    child_2_name = child_2->Get_node_name();
		    child_3_name = child_3->Get_node_name();

		    if(MyName == "if_then_else" || MyName == "and" || MyName == "or")
		      {
			if(!Logical(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(!Logical(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
			
			if(!Logical(child_3_name))
			 {
			    cout<<"\n"<<child_3_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }

		    else if(MyName == "select" || MyName == "ite")
		      {
			if(LogicalOperator(child_1_name))
			  {
			    cout<<"\n"<<child_1_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }

			if(LogicalOperator(child_2_name))
			 {
			    cout<<"\n"<<child_2_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
			
			if(LogicalOperator(child_3_name))
			 {
			    cout<<"\n"<<child_3_name<<" occures as child of "<<MyName<<" node in the DAG\n";
			    exit(1);
			  }
		      }
		    else
		      {
			cout<<"\nUnexpected operator "<<MyName<<endl;
			exit(1);
		      }

		    checkForPropSkelton(child_1);
		    checkForPropSkelton(child_2);
		    checkForPropSkelton(child_3);
                    
                    break;
                }
	    case 0:
	      {
		// Do nothing for constants
		break;
	      }
            default:
		{
		if(MyName == "and" || MyName == "or")
		      {
                        cout<<"\n"<<MyName<<" occures with "<<outlist_size<<" children \n";
                	DAGNode *child;
			for(int i=0; i<outlist_size; i++)
				{
				child = *iout;
				if(!Logical(child->Get_node_name())) 
					{
					cout<<"\n"<<child->Get_node_name()<<" occures as child of "<<MyName<<" node in the DAG\n";		
			    		exit(1);
					}
				checkForPropSkelton(child);
				iout++;
				}
			}
				
			
		}
            }
        }
        else
        {
	  // Do nothing for variables
        }        
        
    }    

// return true if MyName is a logical operator or constant; false otherwise
bool Logical(string MyName)
{
  if(MyName=="if_then_else" || MyName=="and" || MyName=="not" || MyName=="or" || MyName=="=" || MyName=="is_not_equal" || MyName=="1"|| MyName=="0"|| MyName=="true"|| MyName=="false" || MyName=="bvule" || MyName=="bvuge" || MyName=="bvult" || MyName=="bvugt")
    return true;
  else
    return false;
}

// return true if MyName is a logical operator; false otherwise
bool LogicalOperator(string MyName)
{
  if(MyName=="if_then_else" || MyName=="and" || MyName=="not" || MyName=="or" || MyName=="=" || MyName=="is_not_equal"  || MyName=="bvule" || MyName=="bvuge" || MyName=="bvult" || MyName=="bvugt")
    return true;
  else
    return false;
}


// Function to find the support of the dag in the parameter node
// Example : if node is x+y+z, function should make names = {x, y, z}
void getDAGNames(DAGNode *node, set<string> &names)
{
  bool external_dp=false;

  if(external_dp)
    {
      string key;
      char temp_char[10];
      sprintf(temp_char , "%x", node);
      key = temp_char;

      HTStatusValue< set<string> > result = GetDagNames_HT.hashtable_search(key);

      if(result.success())
	{
	  //cout<<"\nCache Hit of getDAGNames for node "<<node<<"\n";
	  set<string> ExistingSet;
	  ExistingSet = result.GetValue();
	  set_union(names.begin(), names.end(), ExistingSet.begin(), ExistingSet.end(),inserter(names, names.begin()));
	}
      else
	{
	  set<string> NewSet;

	  bool dp_enabled=true;

	  if(dp_enabled)
	    {
	      set<DAGNode*> Encountered;
	      getDAGNamesInternal(node, NewSet, Encountered);
	    }
	  else
	    {
	      getDAGNamesInternalNoDP(node, NewSet);
	    }

	  result = GetDagNames_HT.hashtable_insert(key, NewSet);

	  if(!result.success())
	    {

	      cout<<"\nHash Insert Failure Inside Function  getDAGNames\n";
	      exit(1);
	    }
	  set_union(names.begin(), names.end(), NewSet.begin(), NewSet.end(),inserter(names, names.begin()));
	}// else of if found in hash table

    }// if(extenal_dp)

  else
    {
      bool dp_enabled=false;

      if(dp_enabled)
	{
	  set<DAGNode*> Encountered;
	  getDAGNamesInternal(node, names, Encountered);
	}
      else
	{
	  getDAGNamesInternalNoDP(node, names);
	}
    }// else

}// end of function

// Function called from getDAGNames to get the support : note that this function
// works with dynamic programming
void getDAGNamesInternal(DAGNode *node, set<string> &names, set<DAGNode*> &Encountered)
{
  if(Encountered.find(node) != Encountered.end()) // node already encountered
    {
      return;
    }

  string dag_str = node->Get_node_name();

  if(dag_str == "exists")
    {
      set<string> ExistsVarSet;
      set<string> VarSet;

      list<DAGNode*>::iterator iout = node->Get_outlist_start();

      getDAGNamesInternal(*iout, ExistsVarSet, Encountered);
      iout++;
      getDAGNamesInternal(*iout, VarSet, Encountered);

      set_difference(VarSet.begin(), VarSet.end(), ExistsVarSet.begin(), ExistsVarSet.end(),
		     inserter(names, names.begin()));
    }// if
  else
    {
      Value* MyValue = (Value*)(node->getValue());
      Type MyType = MyValue->get_node_type();
      //int MyWidth = MyType.width;
      int MyDataType = MyType.data_type;

      if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)  //if the dag value is variable
        {
	  names.insert(dag_str);
        }

      list<DAGNode*>::iterator iout = node->Get_outlist_start();

      for(unsigned int i=0; i<node->Get_outlist_size() ; i++, iout++)
        {
	  getDAGNamesInternal(*iout, names, Encountered);
        }
    }// else

  Encountered.insert(node);

}

// Function called from getDAGNames to get the support
void getDAGNamesInternalNoDP(DAGNode *node, set<string> &names)
{
  string dag_str = node->Get_node_name();

  if(dag_str == "exists")
    {
      set<string> ExistsVarSet;
      set<string> VarSet;

      list<DAGNode*>::iterator iout = node->Get_outlist_start();

      getDAGNamesInternalNoDP(*iout, ExistsVarSet);
      iout++;
      getDAGNamesInternalNoDP(*iout, VarSet);

      set_difference(VarSet.begin(), VarSet.end(), ExistsVarSet.begin(), ExistsVarSet.end(),
		     inserter(names, names.begin()));
    }// if
  else
    {
      Value* MyValue = (Value*)(node->getValue());
      Type MyType = MyValue->get_node_type();
      //int MyWidth = MyType.width;
      int MyDataType = MyType.data_type;

      if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)  //if the dag value is variable
        {
	  names.insert(dag_str);
        }

      list<DAGNode*>::iterator iout = node->Get_outlist_start();

      for(unsigned int i=0; i<node->Get_outlist_size() ; i++, iout++)
        {
	  getDAGNamesInternalNoDP(*iout, names);
        }
    }// else

}

// return 1 if element appears in MyList; false otherwise
int searchList(list<string> MyList, string element)
{
  for(list<string>::iterator it=MyList.begin(); it != MyList.end(); it++)
    if(*it==element) return 1;
  return 0;
}

// append the content of MySet to MyList
void attachSetToList(set<string> MySet, list<string> &MyList)
{

  for(set<string>::iterator it=MySet.begin(); it!=MySet.end(); it++)
    {
      if(!MyList.empty())
        {if(searchList(MyList,*it) == 0)
            MyList.push_back(*it);}
      else
        MyList.push_back(*it);
    }
}

// return the #bits in the constraint root
int getNoOfBitsInConstraint(DAGNode* root, map<string, int> &WidthTable)
{
  set<string> names;
  getDAGNames(root, names);

  // Newly added on 12.5.2010 

  int maximum=0;

  for(set<string>::iterator it=names.begin(); it!=names.end(); it++)
    {
      string my_name=*it;
      int my_width = WidthTable[my_name];
      if(my_width>maximum) maximum = my_width;

      //cout<<endl<<my_name<<"\t"<<my_width<<endl;
    }


  //cout<<"\nmaximum = "<<maximum<<endl;

  return maximum;

  // Newly added on 12.5.2010


//   set<string>::iterator names_it=names.begin();
//   string my_name=*names_it;
 
//   //cout<<"\nmy_name = "<<my_name<<endl;

//   return WidthTable[my_name];
}

// convert a number specified as string into ULINT, and return it
ULINT convertLiteralToNumber(string literal)
{
  ULINT number;

  if(literal[1] =='1' && literal[0] =='\'' && literal[2] =='\'') 
    number=1; 
  else if(literal[1] =='0' && literal[0] =='\'' && literal[2] =='\'')  
    number=0; 
  else if(literal[0]=='x')
    {
      literal.erase(literal.begin(), literal.begin()+2);
      literal.erase(literal.end()-1, literal.end());
      number=convertToDecimal(literal);
    }
  else // Normal decimal number
    {
      number=strtoull(literal.c_str(), NULL, 0);
    }
  return number;
}

// convert a hexadecimal number to decimal (ULINT) and return it
ULINT convertToDecimal(string x)
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
	power2=findPower16(power);
	number+=power2*coef;
	i++;
	}

return number;
}


// Given c1.x1 + .... + cn.xn + c0, get c0, and put it in constant
void getConstantModified(DAGNode* node, ULINT &constant)
{
 // Go down the bvadd operator until constant is encountered.

  string dag_str = node->Get_node_name();

  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;
  
  if(MyDataType == BITVECTOR_CONSTANT && node->Get_outlist_size()==0) // bv-constant
    
    {
      constant = convertLiteralToNumber(dag_str); 
      return;
    }
  else if(MyDataType == INTEGER_CONSTANT && node->Get_outlist_size()==0) // integer-constant
    
    {
      cout<<"\nWarning!Integer constant "<<dag_str<<" encountered in getConstantModified in Eliminator.cpp\n";
      constant = convertLiteralToNumber(dag_str); 
      return;
    }
  else if(dag_str == "bvadd")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	   
          getConstantModified(*iout, constant);
	  iout++;
	  getConstantModified(*iout, constant);
	}
}



// Given c1.x1 + .... + cn.xn + c0, and xi, get ci, and put it in Coefficient
void getCoefficient(DAGNode *node, string Var, ULINT &Coefficient)
{
  string dag_str = node->Get_node_name();

	if(dag_str == "bvmul")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	   DAGNode *coef = *iout;
	   iout++;
	   DAGNode *var = *iout;
	   string variable = var->Get_node_name();

           if(variable == Var)
		{
		getConstantModified(coef, Coefficient);
		}
	}
  
      else     
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  
          for(unsigned int i=0; i<node->Get_outlist_size(); i++, iout++)
		getCoefficient(*iout, Var, Coefficient);
	}
}

// Given an LME, recreate it as 2^k.x = t, where x is the first one in the lexicographic
// ordering, and t is free of x 
DAGNode* recreateEquationInProperForm(DAGNode* equation, map<string, int> &WidthTable)
{
  // Get the list of variables
  list<string> VarNames;
  set<string> names;
  
  getDAGNames(equation, names);
  attachSetToList(names, VarNames);
  VarNames.sort();

  DAGNode *lhs, *rhs;
  list<DAGNode*>::iterator out_list_it = equation->Get_outlist_start();
  lhs = (*out_list_it);
  out_list_it++;
  rhs = (*out_list_it);
  
  if(lhs==rhs) return equation;
  if(names.empty()) return equation;

 
  int NoOfBitsInConstraint=getNoOfBitsInConstraint(equation, WidthTable);

  //cout<<"\nNo: of bits in constraint = "<<NoOfBitsInConstraint<<endl;

//   DAGNode *lhs, *rhs;
//   list<DAGNode*>::iterator out_list_it = equation->Get_outlist_start();
//   lhs = (*out_list_it);
//   out_list_it++;
//   rhs = (*out_list_it);

  Matrix A, C; // Row matrices
  Matrix B, D; // Row matrices

  getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
  getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D


   // We have AX+B = CX+D
  // Convert A to A-C and D to D-B to get (A-C)X = (D-B)

  ULINT LocalMod=findPower(NoOfBitsInConstraint);
 
  matSub(A, C, LocalMod);
  matSub(D, B, LocalMod);

  // Now equation is AX = D

  // Converting the first element in coef mat to normal form

  ULINT FirstElement = A[0][0];

  ULINT OddPart = getOddPart(FirstElement);


  if(OddPart != 0)
    {

      ULINT MIOfOddPart = mInverse(OddPart, LocalMod);

      scalarMatMultiply(A, MIOfOddPart, LocalMod);

      scalarMatMultiply(D, MIOfOddPart, LocalMod);
    }


  // Now equation is AX = D with first element in normal form
  
  DAGNode* RecreatedEquation;
  
  bool old_imp = false;
  if(old_imp)
	RecreatedEquation = getDAGFromEqs(A, D, VarNames, LocalMod);
  else
	RecreatedEquation = recreateDAGFromEqs(A, D, VarNames, LocalMod);
  

  return RecreatedEquation;

}

// Function to return the content of a dag (in propositional skeleton format) in Yices format
// Note that this function exits if the argument dag has concats in it.
// If the argument dag has concats in it, please use the function 
// callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats
string callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(DAGNode *root, map<string, int> WidthTable)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  propogateWidths(root, WidthTableForNodes, 1);

  return getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(root, "bool", WidthTableForNodes, 1);
}

// Function to create a table WidthTableForNodes, which maps a dag node to its width
void createWidthTableForOperators(DAGNode *node, map<DAGNode*, int> &WidthTableForNodes, map<string, int> WidthTable)
{
  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

 
 if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0) // variable
    {
    
      int Width =  WidthTable[name];
      WidthTableForNodes.insert(make_pair(node, Width));
    }

  else
    {
     			
    list<DAGNode*>::iterator iout = node->Get_outlist_start();
    unsigned int outlist_size = node->Get_outlist_size();
    
    

    switch(outlist_size)
      {
      case 1:
	{
	  DAGNode *child1 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);

	  if(name == "not")
	    {
	      int Width =  1;
	      WidthTableForNodes.insert(make_pair(node, Width));
	    }
	  else
	    {
	     cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperators\n";
	     exit(1);
	    }
 
	 
	  break;
	}
      case 2:
	{
	  DAGNode *child1 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child2 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	 
	  if(name == "=" || name == "bvadd" || name == "bvmul" || name == "is_not_equal" || name == "concat" || name == "bvsub")
	    {

	      int Width1 =  WidthTableForNodes[child1];
	      int Width2 =  WidthTableForNodes[child2];

	      int Width;

	      if(name == "=" || name == "bvadd" || name == "bvmul" || name == "is_not_equal" || name == "bvsub")
		Width = findMaximum(Width1, Width2);
	      else // concat
		Width = Width1 + Width2;

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }

	  else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }
	  else if(name == "bvor" || name == "bvand")
	    {
	      int Width = WidthTableForNodes[child1];

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }

	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperators\n";
	      exit(1);
	    }

	  break;
	}
      case 3:
	{
	  DAGNode *child1 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child2 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child3 = *iout;
	  createWidthTableForOperators(*iout, WidthTableForNodes, WidthTable);

	  if(name == "ite" || name == "if_then_else") // Width of an ite is in fact maximum of widths of child2 and child3
	    {
	      
	       int Width1 =  WidthTableForNodes[child2];
	       int Width2 =  WidthTableForNodes[child3];

	      int Width = findMaximum(Width1, Width2);

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }
	  else if(name == "select")
	    {

	      int Width = getWidthOfSelectionNode(node);

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }
	else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }

	   else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperators\n";
	      exit(1);
	    }

	 
	  break;
	}
      case 0: // constants
	{
	  if(name[0] == 'x') // hex
	    {
	      WidthTableForNodes.insert(make_pair(node, (name.length()-3)*4));
	    }
	  else // 
	    {
             int Width;

	     if(MyWidth != IMMATERIAL_WIDTH) 
	       {
		 //Width = MyWidth;
		 Width = findMinimumWidthRequired(name);
		 //cout<<"\nWidth immaterial encountered\n";
	       }
	     else 
	       {
		 Width = findMinimumWidthRequired(name);
		 //cout<<"\nMin. Width found\n";
	       }

	     //cout<<endl<<"name = "<<name<<" Width = "<<Width<<endl;

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }
	break;
	}

      default :
	{
          for(int i=0; i<outlist_size; i++)
		{
		DAGNode* child = *iout;
		createWidthTableForOperators(child, WidthTableForNodes, WidthTable);
		iout++;
		}
	if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(node, Width));
	    }

	else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperators\n";
	      exit(1);
	    }

	 
	  break;
	}
      }

    }
}





// Find maximum of i and j
int findMaximum(int i, int j)
{
  if(i>=j) return i;
  else return j;
}



// return the minimum width required to represent a Number
// Number is a string here
int findMinimumWidthRequired(string Number)
{
  if(Number == "'1'" || Number == "'0'" || Number == "true" || Number == "false")
    return 1;
  ULINT DecNumber = strtoull(Number.c_str(), NULL, 0);
  int Width = findMinimumWidthRequiredForNumber(DecNumber);
  return Width;
}

// return the minimum width required to represent a Number
// Number is ULINT here
int findMinimumWidthRequiredForNumber(ULINT number)
{
  int width = 0;

  if(number<0) {cout<<"\nint findMinimumWidthRequired(ULINT mumber) called with negative argument\n"; exit(1);}

  do
    {
      number = number/2;
      width++;
    }
  while(number>0);
  
  return width;
}


// Function to adjust the widths in WidthTableForNodes. This function parses the dagnode "node", and
// changes the entries in WidthTableForNodes according to the contents of "node"
void propogateWidths(DAGNode *node, map<DAGNode*, int> &WidthTableForNodes, int WidthFromParent)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

  if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0) // variable
    {
    
      // Do nothing : Variable's width should not be changed
    }

  else
    {

    list<DAGNode*>::iterator iout = node->Get_outlist_start();
    unsigned int outlist_size = node->Get_outlist_size();

    switch(outlist_size)
      {
      case 1:
	{
	  int width = WidthTableForNodes[node];
	  propogateWidths(*iout, WidthTableForNodes, width);

	  break;
	}
      case 2:
	{
	  if(name == "bvadd" || name == "bvmul")
	    {
	      WidthTableForNodes[node] = WidthFromParent;
	    }

	  int width = WidthTableForNodes[node];
	  propogateWidths(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths(*iout, WidthTableForNodes, width);
	 
	  break;
	}
      case 3:
	{
	  int width = WidthTableForNodes[node];
	  propogateWidths(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths(*iout, WidthTableForNodes, width);
	 
	  break;
	}
      case 0: // constants
	{
	  // For all constants, width field is set
	  // But note that width field makes sense only for bit-vector constants
          break;
	}
       default:
	{
	  int width = WidthTableForNodes[node];
	   
          for(int i=0; i<outlist_size; i++)
		{
                propogateWidths(*iout, WidthTableForNodes, width);
		iout++;
		}
	}
      }

    }
}


// Function to traverse the dag node "node" and return the content of a dag (in propositional skeleton format) in Yices format
// This is called by callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable
string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)
        {
               dag_str = name;
               return dag_str;
        }
        else
          {

            if(name == "select")
              {
                dag_str = "(bv-extract ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();

                iout++;

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "const", WidthTableForNodes, 1) + " ";

                iout = node->Get_outlist_start();

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "const", WidthTableForNodes, 1) + " ";

                iout = node->Get_outlist_start();

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {

		cout<<"\nconcat is not supported by function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		exit(1);

              }

	    else if(name == "bvmul")
	      {
		dag_str = "(bv-mul ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "bvadd")
	      {
		dag_str = "(bv-add ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	     else if(name == "bvsub")
	      {
		dag_str = "(bv-sub ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "if_then_else"))
	      {
                if(name == "if_then_else") name = "ite";

		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {

                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);
		
              }

	    else if(name == "'0'")
              {
                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

              }

	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      return dag_str;
		      }
		else if(context == "bv")
		      {
	
			if(name[0] == 'x') // hex
			  {
			  dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);
			  }
			else // Decimal
			  {
			  dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);
			  }
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
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


// Function to return the 4-bit binary representation, given a hexadecimal
// number in parameter "c"
string getBinaryOfChar(char c)
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
	case 'A' : return "1010";
	case 'B' : return "1011";
	case 'C' : return "1100";
	case 'D' : return "1101";	
	case 'E' : return "1110";
	case 'F' : return "1111";

	}
}


// Function to convert a hex string into Yices binary format, and return it
string convertHexToBitvectorWithProperWidth(string hex, int Width)
{
hex.erase(hex.begin(), hex.begin()+2);
hex.erase(hex.end()-1, hex.end());
string bv="0b";

string str = hexToBinaryString(hex);
 
int NoOfZeros = Width - str.length();

for(int i=0; i<NoOfZeros; i++)
   bv += "0";
 
bv += str;

return bv;
}

// Function to convert a hex string into a binary string
string hexToBinaryString(string hex)
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


// convert a literal which is '1' or '0' into Yices format, and return it
string convertLiteralToBitvectorWithProperWidth(string literal, int Width)
{

string bv="0b";

string str;

 if(literal == "'1'")
   str="1";
 else if(literal == "'0'")
   str="0";
 else
   {cout<<"\nstring convertLiteralToBitvectorWithProperWidth(string literal, int Width) called with literal = "<<literal<<endl; exit(1);}
 
int NoOfZeros = Width - 1;

for(int i=0; i<NoOfZeros; i++)
   bv += "0";
 
bv += str;

return bv;
}

// Function to convert a decimal string into Yices binary format, and return it
string convertDecimalToBitvectorWithProperWidth(string dec, int Width)
{
string bv="0b";
ULINT number = strtoull(dec.c_str(), NULL, 0);
string str = integerToBinaryString(number);
int NoOfZeros = Width - str.length();


for(int i=0; i<NoOfZeros; i++)
   bv += "0";
 
bv += str;

return bv;
}

// Function to convert a decimal number into a binary string
string integerToBinaryString(ULINT i)
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


// Function to write the declarations of the variables in WidthTable in Yices format in the file "smt_file"
void writeBVDeclarationsInYicesFormat(FILE *smt_file, map<string, int> WidthTable)

{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
    {

      fprintf(smt_file,"(define %s",(it->first).c_str());
      fprintf(smt_file," :: (bitvector %d))\n",it->second);
    }

}


// returns true if f is unsat; false otherwise
// In future, use this function whenever there is a need to check
// satisfiability of an arbitrary formula
bool ThUnsatSingleFormula(DAGNode* f, map<string, int> WidthTable)
{
  if(internal_solver == "stp")
	{
	//return ThUnsatSingleFormulaInSMTLibFormat(f, WidthTable);
        //cout<<"\nf = "<<getDAGStr(f)<<endl;
        return ThUnsatSingleFormulaInSMTLibFormatAsDAG(f, WidthTable);
	}


  FILE *smt_file = fopen("thunsat_single_formula.ys", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(f, WidthTable);


  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  system("yices -tc thunsat_single_formula.ys > result_thunsat_single_formula.txt");
  FILE *result_smt_file = fopen("result_thunsat_single_formula.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_single_formula.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::ThUnsat\n"; exit(1);
    }
}


// returns true if the dag node "node" is free of the variable VarToElim; false otherwise
bool freeOfVarModified(DAGNode* node, string VarToElim)
{
set<string> names;
getDAGNames(node, names);
if(names.find(VarToElim) == names.end())  return true;
ULINT coefficient=1;
getCoefficient(node, VarToElim, coefficient);
if(coefficient == 0) return true;
return false;
}

// shows the list of string "names"
void showList(list<string> names, string WhoAmI)
{
  cout<<"\n"<<WhoAmI<<"\n";
  for(list<string>::iterator it=names.begin(); it!=names.end();it++)
    cout<<*it<<endl;
}

// returns true if the dag node "predicate" is free of a set of variables DropVars; false otherwise
bool freeOfVars(DAGNode *predicate, set<string> &DropVars)
{
  set<string>::iterator it = DropVars.begin();

  for(;it!=DropVars.end(); it++)
    {
      if(!freeOfVarModified(predicate, *it)) // not free
        return false;
    }

  return true;
}

// returns true if exists(X, original) => eliminated i.e. original \and \neg eliminated is unsat; false otherwise
bool checkForExactnessInOneDirection(DAGNode* original, DAGNode* eliminated, map<string, int> WidthTable)
{
  FILE *check_file = fopen("check.ys", "w");

  if(check_file == NULL) 
   {cout<<"\nCannot create check.ys\n"; exit(1);}


  writeBVDeclarationsInYicesFormat(check_file, WidthTable);


  string original_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(original, WidthTable);
  string eliminated_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(eliminated, WidthTable);

  fprintf(check_file,"\n(assert (and  %s (not %s)))", original_str.c_str(), eliminated_str.c_str());
  fprintf(check_file,"\n(check)\n");
  
  fclose(check_file);

  system("yices -tc check.ys > result_check.txt");

  cout<<"\nYices called\n";

  FILE *result_check_file = fopen("result_check.txt", "r");
  
  if(result_check_file == NULL) 
   {cout<<"\nCannot create result_check.txt\n"; exit(1);}



  char result[100];
  fscanf(result_check_file, "%s", result);
  fclose(result_check_file);
  string result_str(result);

  cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat")
    {
      cout<<"\nexists(X,f)=>f' holds\n";
      return true;
     }
      
  else if(result_str=="sat")
    { cout<<"\nexists(X,f)=>f' does not hold\n";
      return false;
    }
  else
    {
      cout<<"\nUnknown result from Yices"<<result_str<<endl;
      exit(1);
    }


}


// Here "node" represents a propositional logic formula
// This function returns the content of "node" as a string in Yices format
string getBooleanDagAsString(DAGNode *node, string context)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

	cout<<"\nname = "<<name<<", context = "<<context<<endl;

        if(MyDataType != LOGICAL_CONSTANT && node->Get_outlist_size() == 0)
        {
               int index = name.find_last_of("_");
	       string var_name = name.substr(0, index);
	       string location_str = name.substr(index+1);
	       dag_str = "(bv-extract ";
	       dag_str += location_str;
	       dag_str += " ";
	       dag_str += location_str;
	       dag_str += " ";
               dag_str += var_name;
	       dag_str += ")";               
               return dag_str;
        } 
        else
          {

	if(name == "=")
	      {
 	       dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

               for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
		   dag_str += getBooleanDagAsString(*iout, "bv");
	            dag_str += " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	else if(name == "and" || name == "or" || name == "not")
	      {
                if(context == "bv") name = "bv-" + name;

		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
		   dag_str += getBooleanDagAsString(*iout, context);
		   dag_str += " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	 else if(name == "if_then_else")
	      {
                dag_str = "(ite ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
		   dag_str += getBooleanDagAsString(*iout, context);
		   dag_str += " ";
                  }

                dag_str += ")";

                return dag_str;

              }

          else if(name == "exor")
	      {

		if(context != "bv") {cout<<"\nexor occuring in dag is not operating on bit-vectors\n";
		  exit(1);
		}
		          

		unsigned int outlist_size = node->Get_outlist_size();
		
		if(outlist_size != 2)
			{
			cout<<"\nNumber of children of exor not 2 in function getBooleanDagAsString\n";
			exit(1);
			}

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       
	       string op1 = getBooleanDagAsString(*iout, context);
	       iout++;
	       string op2 = getBooleanDagAsString(*iout, context);
	        
	       dag_str = "(bv-or (bv-and (bv-not ";
	       dag_str += op1;
	       dag_str += ") ";
	       dag_str += op2;
               dag_str += ") ";
	       
               dag_str += "(bv-and ";
	       dag_str += op1;
	       dag_str += " (bv-not ";
               dag_str += op2;
               dag_str += ") ) ";
               dag_str += ")";               
               return dag_str;

              }



	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		if(name == "true") { if (context=="bv") return "0b1"; else return "true";}
		else if(name == "false") { if(context=="bv") return "0b0"; else return "false";}
		else { cout<<"\nUnanticipated constant "<<name<<" inside function getDAGAsString\n"; exit(1);}
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated constant "<<name<<" inside function getDAGAsString\n"; exit(1);
	      }
		    
	  }
}

// Updates the set VarSetToElim depending on the content of the node "parent"
void updateVarSetToElim(set<string> &VarSetToElim, DAGNode *parent, string new_name)
{
  list<DAGNode*>::iterator it = parent->Get_outlist_start();
  DAGNode *child = *it;
  string Name = child->Get_node_name();
  if (VarSetToElim.find(Name)!=VarSetToElim.end())
    VarSetToElim.insert(new_name);
}

// returns true if "parent" is a non-linear multiplication; false otherwise
bool nonLinearMult(DAGNode *parent)
{
  if(parent->Get_node_name()!="bvmul") return false;

  list<DAGNode*>::iterator it = parent->Get_outlist_start();
  DAGNode *child1 = *it;
  it++;
  DAGNode *child2 = *it;
  //cout<<"\nChecking Non-linear multiplication of "<<getDAGStr(parent)<<"\n";

  bool child1_operator, child2_operator;
  int Child1DataType, Child2DataType;
  Value* ChildValue;
  Type ChildType;

  if(child1->Get_outlist_size()!=0) 
    child1_operator=true;
  else
    {
      child1_operator=false;
      ChildValue = (Value*)(child1->getValue());
      ChildType = ChildValue->get_node_type();
      Child1DataType = ChildType.data_type;
    }

  if(child2->Get_outlist_size()!=0) 
    child2_operator=true;
  else
    {
      child2_operator=false;
      ChildValue = (Value*)(child2->getValue());
      ChildType = ChildValue->get_node_type();
      Child2DataType = ChildType.data_type;
    }

  if(child1_operator && child2_operator)
    {
      cout<<"\nNon-linear multiplication encountered\n";
      return true;
    }


  if(Child1DataType !=BITVECTOR_CONSTANT && Child2DataType !=BITVECTOR_CONSTANT)
    {
      cout<<"\nNon-linear multiplication encountered\n";
      return true;
    }
  return false;
}

// linearize the dag in "parent". i.e. replace the nodes containing non-linear operations such as
// concat, select etc. by dag nodes which are fresh variables, and then return the linearized dag
// This function does not support LMIs
DAGNode* getLinearizedClone(DAGNode* parent, map<DAGNode*, DAGNode*> &LinearizationTable, int &count, map<string, int> &WidthTable, set<string> &VarSetToElim)
{
  DAGNode* clone;
  string MyName = parent->Get_node_name();

  if(MyName == "concat" || MyName == "select" || MyName == "bvor" || MyName == "bvand" || MyName == "bvnot" || MyName == "bvxor" || nonLinearMult(parent)) // node to be linearized encountered
    {
      map<DAGNode*, DAGNode*>::iterator it = LinearizationTable.find(parent);
	    
      if(it != LinearizationTable.end()) // parent is already added to the Table. get the address of the new node and return it
	{
	  clone = it->second;
	}
      else // parent is newly encountered. Create a new node and return it's address
	{
	  string new_name = "autoname_";
	  count++;
	  char char_count[10] = " ";
	  sprintf(char_count , "%d", count);
	  string string_count(char_count);
	  new_name += string_count;


	  int width = getWidthOfNode(parent, WidthTable);

	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = BITVECTOR_SYMBOL;
	  t->width = width;
	  v->set_node_type(*t);
	  clone = dm.createNode (new_name,(void*)v);

	  LinearizationTable.insert(make_pair(parent, clone)); // inserted into LinearizationTable
	  WidthTable.insert(make_pair(new_name, width));
	}
    }
                   
  else if(MyName == "bvadd" || MyName == "bvmul" || MyName == "=" || MyName == "not" || MyName == "or" || MyName == "and" || MyName == "if_then_else")  //i.e. if an operator node allowed in DD creation
    {
      list<DAGNode*>::iterator it = parent->Get_outlist_start();
      vector<DAGNode *> Children;
      	
      for(int i=0; i<parent->Get_outlist_size(); i++)
	{
	  DAGNode *child = getLinearizedClone(*it, LinearizationTable, count, WidthTable, VarSetToElim);
	  Children.push_back(child);
	  it++;
	}
	
      clone = dm.createNode(MyName,Children, parent->getValue());
    }
	
  else if(parent->Get_outlist_size()==0) // leaf level node
    {
      clone = dm.createNode(MyName, parent->getValue());
    }
  else
    {
      cout<<"\nIllegal operator "<<MyName<<" encountered in function getLinearizedClone in Eliminator.cpp\n";
      exit(1);
    }

  return clone;
}        



// Version of getLinearizedClone with support for LMIs
DAGNode* getLinearizedCloneWithLMISupport(DAGNode* parent, map<DAGNode*, DAGNode*> &LinearizationTable, int &count, map<string, int> &WidthTable, set<string> &VarSetToElim)
{
  DAGNode* clone;
  string MyName = parent->Get_node_name();

  if(MyName == "concat" || MyName == "select" || MyName == "bvor" || MyName == "bvand" || MyName == "bvnot" || MyName == "bvxor" || nonLinearMult(parent)) // node to be linearized encountered
    {
      map<DAGNode*, DAGNode*>::iterator it = LinearizationTable.find(parent);
	    
      if(it != LinearizationTable.end()) // parent is already added to the Table. get the address of the new node and return it
	{
	  clone = it->second;
	}
      else // parent is newly encountered. Create a new node and return it's address
	{
	  string new_name = "autoname_";
	  count++;
	  char char_count[10] = " ";
	  sprintf(char_count , "%d", count);
	  string string_count(char_count);
	  new_name += string_count;


	  int width = getWidthOfNode(parent, WidthTable);

	  Value *v = new Value();
	  Type *t = new Type();
	  t->data_type = BITVECTOR_SYMBOL;
	  t->width = width;
	  v->set_node_type(*t);
	  clone = dm.createNode (new_name,(void*)v);

	  LinearizationTable.insert(make_pair(parent, clone)); // inserted into LinearizationTable
	  WidthTable.insert(make_pair(new_name, width));
	}
    }
                   
  else if(MyName == "bvadd" || MyName == "bvmul" || MyName == "=" || MyName == "not" || MyName == "or" || MyName == "and" || MyName == "if_then_else" ||
          MyName == "bvuge" || MyName == "bvule" || MyName == "bvugt" || MyName == "bvult" || MyName == "iff")  //i.e. if an operator node allowed in DD creation
    {
      list<DAGNode*>::iterator it = parent->Get_outlist_start();
      vector<DAGNode *> Children;
      	
      for(int i=0; i<parent->Get_outlist_size(); i++)
	{
	  DAGNode *child = getLinearizedCloneWithLMISupport(*it, LinearizationTable, count, WidthTable, VarSetToElim);
	  Children.push_back(child);
	  it++;
	}

      if(MyName == "iff")
	{
		DAGNode* child1 = Children[0];
		DAGNode* child2 = Children[1];

		DAGNode* iff_child1 = createDAG("and",child1, child2, LOGICAL_SYMBOL, 1);
		DAGNode* iff_child2 = createDAG("and",  createDAG("not",child1, LOGICAL_SYMBOL, 1) ,  createDAG("not",child2, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);

		clone = createDAG("or", iff_child1, iff_child2, LOGICAL_SYMBOL, 1);
	}
	else
	{
		clone = dm.createNode(MyName,Children, parent->getValue());
	}
    }
	
  else if(parent->Get_outlist_size()==0) // leaf level node
    {
      clone = dm.createNode(MyName, parent->getValue());
    }
  else
    {
      cout<<"\nIllegal operator "<<MyName<<" encountered in function getLinearizedCloneWithLMISupport in Eliminator.cpp\n";
      exit(1);
    }

  return clone;
}        




// Find the width of the node "node" and return it
int getWidthOfNode(DAGNode *node, map<string, int> &WidthTable)
{
  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

 
  if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0) // variable
    {
    
      return WidthTable[name];
    }

  else
    {
     			
      list<DAGNode*>::iterator iout = node->Get_outlist_start();
      unsigned int outlist_size = node->Get_outlist_size();
    
    

      switch(outlist_size)
	{
	case 1:
	  {
	    if(name == "bvnot")
	      {
		return getWidthOfNode(*iout, WidthTable);
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getWidthOfNode\n";
		exit(1);
	      }
 
	 
	    break;
	  }
	case 2:
	  {
	    int Width1 = getWidthOfNode(*iout, WidthTable);
	    iout++;
	    int Width2 = getWidthOfNode(*iout, WidthTable);
	 
	    if(name == "bvadd" || name == "bvmul" || name == "concat" || name == "bvsub" || name == "bvand" || name == "bvor" || name == "bvxor")
	      {

		if(name == "bvadd" || name == "bvmul" || name == "bvsub" || name == "bvand" || name == "bvor" || name == "bvxor")
		  {
		  return findMaximum(Width1, Width2);
		  }
		else // concat
		  {
		  return (Width1 + Width2);
		  }
   
	      }

	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getWidthOfNode\n";
		exit(1);
	      }

	    break;
	  }
	case 3:
	  {
	    if(name == "select")
	      {
		return getWidthOfSelectionNode(node);
	      }   
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getWidthOfNode\n";
		exit(1);
	      }
	    break;
	  }
	case 0: // constants
	  {
	    if(MyDataType == BITVECTOR_CONSTANT) 
	      {
		return MyWidth;
	      }
	    if(MyDataType == LOGICAL_CONSTANT) 
	      {
		return 1;
	      }
	    else 
	      { 
		cout<<"\nUnanticipated integer constant inside function getWidthOfNode\n"; exit(1);
	      }
	  }// case 0

	}// switch


    }// else
}// function



// Functions newly added on 28th May 2010



// The final version of the function to drop unconstraining LMDs. i.e. function implementing Layer 2 of the CAV'11 paper
void dropUnconstrainingDisequationsModified(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod) 
{

  // Take variables to be eliminated one by one. For each variable, eliminate the disequations which can be eliminated

for(int VarToElimIndex=0; VarToElimIndex<NoVarsToElim; 	VarToElimIndex++)
	{
	 if(DisEqCoefMat.empty())
	   return;
	    
	//cout<<"\nTrying to drop disequalities looking at variable x"<<VarToElimIndex<<"\n";

	map<int, ULINT> CoefsOfVarToElimEqualities, CoefsOfVarToElimDisEqualities;
	// The map stores coef of variable to be eliminated for each disequality. If coef of variable to be eliminated for the disequality is zero, then it is not stored in the map

	CoefsOfVarToElimEqualities.clear();

	if(!EqCoefMat.empty()) 
	  {
	    copyColumnToMap(EqCoefMat, VarToElimIndex, CoefsOfVarToElimEqualities);
	  }

	copyColumnToMap(DisEqCoefMat, VarToElimIndex, CoefsOfVarToElimDisEqualities);
		
	if(CoefsOfVarToElimDisEqualities.empty())  // variable not present in disequalities. No need to eliminate
	  {
	    //cout<<"\nVariable x"<<VarToElimIndex<<" is already eliminated from disequations\n";
	    continue;
	  }

	//cout<<"\nCoefsOfVarToElimEqualities\n";
	//showMap(CoefsOfVarToElimEqualities);
	//cout<<"\nCoefsOfVarToElimDisEqualities\n";
	//showMap(CoefsOfVarToElimDisEqualities);
	
	// CoefsOfVarToElimEqualities, CoefsOfVarToElimDisEqualities contain coefficients of the variable to eliminate VarToElimIndex in equations and disequations

	// Convert them to powers of 2

	convertCoefToPowerOf2(CoefsOfVarToElimEqualities);
	convertCoefToPowerOf2(CoefsOfVarToElimDisEqualities);

	//cout<<"\nAfter converting into powers of 2\n";
	//cout<<"\nCoefsOfVarToElimEqualities\n";
	//showMap(CoefsOfVarToElimEqualities);
	//cout<<"\nCoefsOfVarToElimDisEqualities\n";
	//showMap(CoefsOfVarToElimDisEqualities);

	ULINT KEqn = findMinimumKValue(CoefsOfVarToElimEqualities, Mod);



/* Newly added on 4 July 2010 to take care of KEqn s.t. KEqn <= Ki where Ki is the Ki of a disequation */

	ULINT KDisEqMax = findMaximumKValue(CoefsOfVarToElimDisEqualities);
        if(KEqn<=KDisEqMax) // This is a bogus LME. 
		{
		KEqn = powerOfTwo(Mod);
		}	

/* Newly added code on 4 July ends here */



	//cout<<"\nKEqn = "<<KEqn<<endl;

	// Drop the disequalities

	set<int> CoreLMDs; // CoreLMDs is the set of LMDs which cannot be dropped
	// Let there is an LMD free of the variable to be eliminated. This cannot be dropped. So it must be in CoreLMDs
	initializeCoreLMDs(DisEqCoefMat, VarToElimIndex, CoreLMDs);
	
	//showSet(CoreLMDs, "CoreLMDs");	

	eliminateDisequalitiesModified(KEqn, CoefsOfVarToElimDisEqualities, CoreLMDs);

	//showSet(CoreLMDs, "CoreLMDs");	
	
	//cout<<"\nShowing the Original Set of Disequalities\n";
	//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

	//cout<<"\nDeleting disequalities not in CoreLMDs\n";



	list<int> DisequationsToEliminate;

	// Get the list of LMDs to eliminate
	for(int i=0; i<DisEqCoefMat.size(); i++) {
	  if(CoreLMDs.find(i)==CoreLMDs.end()) // i is not in CoreLMDs. Hence drop
	    DisequationsToEliminate.push_back(i);
	}
            
	//cout<<"\nDisequationsToEliminate\n";
	//for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
	//  {cout<<*it<<endl;}
	    
	// Eliminate them in reverse order so that after each elimination, the row indexes are intact.

	DisequationsToEliminate.reverse();

	//cout<<"\nDisequationsToEliminate\n";
	//for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
	 // {cout<<*it<<endl;}
  
	for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
	  {
	    //cout<<"\nDisequation "<<*it<<" can be eliminated\n";
	    deleteRow(DisEqCoefMat, *it);
	    deleteRow(DisEqConstMat, *it);
	  }
      
	
        //cout<<"\nShowing the Modified Set of Disequalities\n";
	//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
	}

  
}


//  Function to copy the zero elements in the selectedIndexth column into CoreLMDs. Initially CoreLMDs is empty. 
void initializeCoreLMDs(Matrix &MyMatrix, int selectedIndex, set<int> &CoreLMDs)
{
int rows = MyMatrix.size();
int cols = 0;

if(rows != 0)
	{
	cols = MyMatrix[0].size();
	}

ULINT element;
if(selectedIndex<0 || selectedIndex>=cols) {cout<<"\nInvalid column "<<selectedIndex<<" in function initializeCoreLMDs\n"; exit(1);}
for(int i=0; i<rows; i++)
			{
			element=MyMatrix[i][selectedIndex];
			if(element == 0) 
			  {CoreLMDs.insert(i);}
			}
}

// Given map<int, ULINT> CoefsOfVarToElimEqualities, it returns minium of the second field
// of CoefsOfVarToElimEqualities if CoefsOfVarToElimEqualities is non-empty, and powerOfTwo(Mod)
// if CoefsOfVarToElimEqualities is empty
ULINT findMinimumKValue(map<int, ULINT> &CoefsOfVarToElimEqualities, ULINT Mod)
{
if(CoefsOfVarToElimEqualities.empty())
	return powerOfTwo(Mod);

ULINT MinimumK=powerOfTwo(Mod);
for(map<int, ULINT>::iterator it=CoefsOfVarToElimEqualities.begin(); it != CoefsOfVarToElimEqualities.end(); it++)
	{
	if(it->second<MinimumK)
		MinimumK = it->second;
	}
return MinimumK;
} 

//Eliminates unconstraining LMDs. Called by dropUnconstrainingDisequationsModified
void eliminateDisequalitiesModified(ULINT KEqn, map<int, ULINT> &KValuesForDisEquations, set<int> &CoreLMDs)
{
 set<int> core; // core initially empty
 set<int> D; // set of disequations
 for(map<int, ULINT>::iterator it=KValuesForDisEquations.begin(); it != KValuesForDisEquations.end(); it++)
	{
	D.insert(it->first);
	} // set of disequations obtained


 //cout<<"\nSet of disequations initially in D we have is \n";
 //showSet(D, "D");

 //cout<<"\nInitially core is\n";
 //showSet(core, "core");
  
 while(!setsEqual(core, D))
	{
	//cout<<"\ncore != D\n";
	if(isExtModified(core, D, KEqn, KValuesForDisEquations)) // Any solution to core can be extended
		{
		//cout<<"\ncore is extendable\n";
                set_union(core.begin(), core.end(), CoreLMDs.begin(), CoreLMDs.end(),inserter(CoreLMDs, CoreLMDs.begin()));
		//showSet(CoreLMDs, "CoreLMDs");
		return;
		}
	else
		{
		//cout<<"\ncore is not extendable\n";
		set<int> d = getLeastConstrLMDs(D, core, KValuesForDisEquations); // get set of least constraining LMDs from D\core 	
		set_union(core.begin(), core.end(), d.begin(), d.end(),inserter(core, core.begin()));
		//cout<<"\nNow core is\n";
		//showSet(core, "core");
		}
	}

set_union(core.begin(), core.end(), CoreLMDs.begin(), CoreLMDs.end(),inserter(CoreLMDs, CoreLMDs.begin()));
return;
}

// returns true if set A = set B; false otherwise
bool setsEqual(set<int> &A, set<int> &B)
{
set<int> Diff1;
set<int> Diff2;

set_difference(A.begin(), A.end(), B.begin(), B.end(),inserter(Diff1, Diff1.begin()));
set_difference(B.begin(), B.end(), A.begin(), A.end(),inserter(Diff2, Diff2.begin()));

if(Diff1.empty() && Diff2.empty()) return true;
return false;
}


// Function getLeastConstrLMDs in CAV'11 paper.
// Given a set of LMDs D, and set of LMDs core,
// returns the least constrained set of LMDs from D
set<int> getLeastConstrLMDs(set<int> &D, set<int> &core, map<int, ULINT> &KValuesForDisEquations)
{
  //cout<<"\nFinding the least constrained LMDs\n";
// LeastConstLMDs = LMDs with maximum k in D\core
// Find maximum k of LMDs in D\core
ULINT kMax=0;

for(set<int>::iterator D_it=D.begin(); D_it != D.end(); D_it++)
	{
	int DiseqIndex = *D_it;
	if(core.find(DiseqIndex)!=core.end()) // disequation is in core
		continue;
	ULINT k=KValuesForDisEquations[DiseqIndex];
	
	if(k>kMax) kMax=k;	
	}

 //cout<<"\nkMax = "<<kMax<<endl;

set<int> LeastConstrLMDs;
for(set<int>::iterator D_it=D.begin(); D_it != D.end(); D_it++)
	{
	int DiseqIndex = *D_it;
	if(core.find(DiseqIndex)!=core.end()) // disequation is in core
		continue;
	ULINT k=KValuesForDisEquations[DiseqIndex];
	
	if(k==kMax) LeastConstrLMDs.insert(DiseqIndex);	
	}

//showSet(LeastConstrLMDs, "LeastConstrLMDs");
return LeastConstrLMDs;
}

// Function to show a given set of integers
void showSet(set<int> &MySet, string WhoAmI)
{
cout<<endl<<WhoAmI<<endl<<"***************"<<endl;
for(set<int>::iterator it=MySet.begin(); it != MySet.end(); it++)
  cout<<*it<<"\n";
}

// Function to show a given set of strings
void showSet(set<string> &MySet, string WhoAmI)
{
cout<<endl<<WhoAmI<<endl<<"***************"<<endl;
for(set<string>::iterator it=MySet.begin(); it != MySet.end(); it++)
  cout<<*it<<"\n";
}


// Function to show a given set of strings in a given file
void showSet(set<string> &MySet, string WhoAmI, FILE *fp)
{
fprintf(fp, "\n%s\n***************\n", WhoAmI.c_str());

for(set<string>::iterator it=MySet.begin(); it != MySet.end(); it++)
    {
    fprintf(fp, "%s\n", (*it).c_str());
    }
}


// The final version of isExt in CAV'11 paper
bool isExtModified(set<int> &core, set<int> &D, ULINT KEqn, map<int, ULINT> &KValuesForDisEquations)
{
//cout<<"\nChecking if core is extendable\n";
// find kcore
ULINT kcore;

if(core.empty()) kcore=KEqn;
else
	{
	// kcore is the minimum of k values of disequations in core
	set<int>::iterator core_it=core.begin();
	kcore = KValuesForDisEquations[*core_it];
	core_it++;
	for(; core_it!=core.end(); core_it++)
		{
		if(KValuesForDisEquations[*core_it] < kcore) kcore = KValuesForDisEquations[*core_it];
	 	}
	}

 //cout<<endl<<"kcore = "<<kcore<<endl;
// we got kcore
// create a map<ki, ri> for all disequations in D but not in core
map<ULINT, int> KMap;
for(set<int>::iterator D_it=D.begin(); D_it != D.end(); D_it++)
	{
	int DiseqIndex = *D_it;
	if(core.find(DiseqIndex)!=core.end()) // disequation is in core
		continue;
	ULINT k=KValuesForDisEquations[DiseqIndex];
	// check if entry for k exists in KMap
	map<ULINT, int>::iterator KMap_it=KMap.find(k);
	if(KMap_it!=KMap.end()) // entry already exists
		KMap_it->second=(KMap_it->second)+1;
	else // add a new entry (k, 1)
		KMap.insert(make_pair(k, 1));
	}
// KMap created. Check if extendable
// showKMap
//cout<<"\nshowKMap\n";
//for(map<ULINT, int>::iterator KMap_it=KMap.begin(); KMap_it!=KMap.end(); KMap_it++)
//	{
//	cout<<endl<<KMap_it->first<<"\t"<<KMap_it->second<<endl;
//	}


ULINT CombinationsAllowed=0, CombinationsPrevented=0;

map<ULINT, int>::iterator it1=KMap.begin();
for(;it1!=KMap.end(); it1++)
	{
	ULINT ki;
	int ri;

	ki=it1->first;
	ri=it1->second;
	
	CombinationsPrevented += findPower(ki)*ri;
	}

CombinationsAllowed = findPower(kcore);

//cout<<"\nCombinations Allowed = "<<CombinationsAllowed<<"\tCombinations Prevented = "<<CombinationsPrevented<<endl;

if(CombinationsAllowed > CombinationsPrevented)
	{
	//cout<<"\nExtendable\n";
	return true;
	}

//cout<<"\nNot extendable\n";
return false;
}



// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// and a number shift, create a copy of parent with indexes of the variable nodes changed
// as new index = existing index + shift
// This is used in iterative squaring
DAGNode* cloneAndShift(DAGNode* parent, int shift)
    {
        DAGNode* clone;
	// Get the size of outlist of parent
	unsigned int outlist_size = parent->Get_outlist_size();
	
                   
        if(outlist_size>0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = cloneAndShift(*iout, shift);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = cloneAndShift(*iout, shift);
		    iout++;
                    child_2 = cloneAndShift(*iout, shift);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

                    child_1 = cloneAndShift(*iout, shift);
		    iout++;
                    child_2 = cloneAndShift(*iout, shift);
		    iout++;
                    child_3 = cloneAndShift(*iout, shift);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    Children.push_back(child_3);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function cloneAndShift in Eliminator.cpp\n";
		 exit(1);
		}

	    }
	}
	else // leaf node
	  {
	    string id_delta = parent->Get_node_name();
	    Value* MyValue = (Value*)(parent->getValue());
	    Type MyType = MyValue->get_node_type();
	    int MyWidth = MyType.width;
	    int MyDataType = MyType.data_type;

	    if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
	      {
		int index = id_delta.find_last_of("_");
		if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function cloneAndShift in Eliminator.cpp\n";
		    exit(1);
		  }
		string id = id_delta.substr(0, index);
		string delta_str = id_delta.substr(index+1);	
		int delta = atoi(delta_str.c_str());
		int new_delta = delta+shift;
		
		string new_name = id;
		new_name += "_";
		char temp[100];
		sprintf(temp, "%d", new_delta);
		delta_str = temp;
		new_name += delta_str;

		Value *v = new Value();
		Type *t = new Type();
 		t->data_type = MyDataType;
		t->width = MyWidth;
		v->set_node_type(*t);
		clone = dm.createNode (new_name,(void*)v);
	      }
	    else
	      {
		clone = dm.createNode(parent->Get_node_name(), parent->getValue());
	      }
            
	  }
        return clone;
    }    



// This function extracts the APs from a DAG node "parent"
// APs from the condition part of the ites are inserted before the APs from the then and the else parts
void getAPsForDAGInPropSkelton(DAGNode* parent, list<DAGNode*> &APList)
    {
       	unsigned int outlist_size = parent->Get_outlist_size();
	                   
        if(outlist_size>0)  //i.e. if an operator node
        {
	  list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
		  DAGNode* child_1 = *iout;
		  getAPsForDAGInPropSkelton(child_1, APList);
		  break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;

                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;

		    string MyName = parent->Get_node_name();

		    if(MyName == "equivalent" || MyName == "is_not_equal" || MyName == "=" || MyName == "not_equal_to" || MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt")
		      {
			//cout<<"\n"<<parent->getDAGStr()<<" inserted into APList\n";

			if(!searchAPList(APList, parent)) // insert parent into APList if it is not present already
			  {
			    APList.push_back(parent);
			  }
		      }

		    else 
		      {
			getAPsForDAGInPropSkelton(child_1, APList);
			getAPsForDAGInPropSkelton(child_2, APList);
		      }

                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

		   
                    child_1 = *iout;
		    iout++;
                    child_2 = *iout;
		    iout++;
                    child_3 = *iout;

		    getAPsForDAGInPropSkelton(child_1, APList);
		    getAPsForDAGInPropSkelton(child_2, APList); 
		    getAPsForDAGInPropSkelton(child_3, APList);
		    
		    break;
                }
	    default:
		{
		 string MyName = parent->Get_node_name();
		 if(MyName == "and" || MyName == "or") // and/or with more than two children encountered
			{
			int i=0;
			for(;i<outlist_size;i++)
				{
				getAPsForDAGInPropSkelton(*iout, APList);
				iout++;
				}
			} 

		else
		  {
		  cout<<"\nUnknown operator "<<parent->Get_node_name()<<" reached in function getAPsForDAGInPropSkelton in file Eliminator.cpp\n";
		  exit(1);
		  }
		}
	    }
	}
        else // variables or constants
        {
	  //cout<<"\nVaraible/constant node with data "<<parent->Get_node_name()<<" reached in function getAPsForDAGInPropSkelton in file Eliminator.cpp without going through an equality node\n";
	  //exit(1);
        }        
        
    }    

// Shows a list of dag nodes
void showAPList(list<DAGNode*> &APList)
{
  list<DAGNode*>::iterator it=APList.begin();
  for(;it!=APList.end(); it++)
    {
      cout<<endl<<*it<<"\t"<<getDAGStr(*it)<<endl;
    }
}

// Check if a dag node is already present in the APList. 
// Return true if yes; false otherwise
bool searchAPList(list<DAGNode*> &APList, DAGNode *target)
{
  list<DAGNode*>::iterator it=APList.begin();
  for(;it!=APList.end(); it++)
    {
      if(*it==target) return true;
    }
  return false;
}

// checks if the given node is true
bool isTrue(DAGNode *node)
{
 if (node->Get_node_name()=="true") return true;
 return false;
}


// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// create a copy of parent with indexes of the variable nodes changed
// as new index = 2 if existing index = 1, and new index = existing index otherwise.
// This is used in iterative squaring
DAGNode* cloneWithRenamedDeltas(DAGNode* parent)
    {
        DAGNode* clone;
	// Get the size of outlist of parent
	unsigned int outlist_size = parent->Get_outlist_size();
	
                   
        if(outlist_size>0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = cloneWithRenamedDeltas(*iout);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = cloneWithRenamedDeltas(*iout);
		    iout++;
                    child_2 = cloneWithRenamedDeltas(*iout);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

                    child_1 = cloneWithRenamedDeltas(*iout);
		    iout++;
                    child_2 = cloneWithRenamedDeltas(*iout);
		    iout++;
                    child_3 = cloneWithRenamedDeltas(*iout);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    Children.push_back(child_3);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function cloneWithRenamedDeltas in Eliminator.cpp\n";
		 exit(1);
		}

	    }
	}
	else // leaf node
	  {
	    string id_delta = parent->Get_node_name();
	    Value* MyValue = (Value*)(parent->getValue());
	    Type MyType = MyValue->get_node_type();
	    int MyWidth = MyType.width;
	    int MyDataType = MyType.data_type;

	    if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
	      {
		int index = id_delta.find_last_of("_");
		if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function cloneWithRenamedDeltas in Eliminator.cpp\n";
		    exit(1);
		  }
		string id = id_delta.substr(0, index);
		string delta_str = id_delta.substr(index+1);	
		int delta = atoi(delta_str.c_str());
		int new_delta;

		if(delta==2) new_delta=1; else new_delta = delta;
		
		string new_name = id;
		new_name += "_";
		char temp[100];
		sprintf(temp, "%d", new_delta);
		delta_str = temp;
		new_name += delta_str;

		Value *v = new Value();
		Type *t = new Type();
 		t->data_type = MyDataType;
		t->width = MyWidth;
		v->set_node_type(*t);
		clone = dm.createNode (new_name,(void*)v);
	      }
	    else
	      {
		clone = dm.createNode(parent->Get_node_name(), parent->getValue());
	      }
            
	  }
        return clone;
    }  

// Called by recreateEquationInProperForm to recreate an LME as 2^k.x = t, 
// where x is the first one in the lexicographic
// ordering, and t is free of x 
DAGNode* recreateDAGFromEqs(Matrix EqCoefMat, Matrix EqConstMat, list<string> VarNames, ULINT Mod)
	{
	int width;
	width = powerOfTwo(Mod);
	
	list<string>::iterator it;

        DAGNode* result=NULL;
	
	int rows = EqCoefMat.size();
	int cols = 0;

  	if(rows != 0)
		{
		cols = EqCoefMat[0].size();
		}

	for(int i=0; i<rows; i++)
		{ 
	        DAGNode *lhs=NULL, *eqn, *rhs;
		it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
                        if(EqCoefMat[i][j]==0) continue; // no need to add terms with zero coefficients

			string varname = *it;
			DAGNode *variable = createBVVariable(varname, width);
			
			char coeff[20];
			sprintf(coeff, "%llu", EqCoefMat[i][j]);
			string coeff_str(coeff);

			DAGNode *coefficient = createBVConstant(coeff_str,width);
			DAGNode *term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
			if(lhs==NULL) lhs=term; else lhs=createDAG("bvadd", lhs, term, BITVECTOR_SYMBOL, width);
			}

	       if(lhs==NULL)
			{
			cout<<"\nlhs becomes NULL in function recreateDAGFromEqs in Eliminator.cpp\n";
			exit(1);
			}

		char constant[10];
		sprintf(constant, "%llu", EqConstMat[i][0]);
		string constant_str(constant);
		rhs = createBVConstant(constant_str,width);
		eqn = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);
		if(result==NULL) result=eqn; else result = createDAG("and", result, eqn, LOGICAL_SYMBOL, 1);
		}
	return result;
	
	}
  
// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// create a copy of parent with indexes of the variable nodes changed
// based on the DeltaMap
// NB : Note that a version of this function cloneWithRenamedDeltasWithDeltaMap,
// making use of dynamic programming is avaiable
DAGNode* createShiftedLMEBasedOnDeltaMap(DAGNode* parent, map<int, int> &DeltaMap)
    {
        DAGNode* clone;
	// Get the size of outlist of parent
	unsigned int outlist_size = parent->Get_outlist_size();
	
                   
        if(outlist_size>0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = createShiftedLMEBasedOnDeltaMap(*iout, DeltaMap);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = createShiftedLMEBasedOnDeltaMap(*iout, DeltaMap);
		    iout++;
                    child_2 = createShiftedLMEBasedOnDeltaMap(*iout, DeltaMap);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

                    child_1 = createShiftedLMEBasedOnDeltaMap(*iout, DeltaMap);
		    iout++;
                    child_2 = createShiftedLMEBasedOnDeltaMap(*iout, DeltaMap);
		    iout++;
                    child_3 = createShiftedLMEBasedOnDeltaMap(*iout, DeltaMap);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    Children.push_back(child_3);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function createShiftedLMEBasedOnDeltaMap in Eliminator.cpp\n";
		 exit(1);
		}

	    }
	}
	else // leaf node
	  {
	    string id_delta = parent->Get_node_name();
	    Value* MyValue = (Value*)(parent->getValue());
	    Type MyType = MyValue->get_node_type();
	    int MyWidth = MyType.width;
	    int MyDataType = MyType.data_type;

	    if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
	      {
		int index = id_delta.find_last_of("_");
		if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function createShiftedLMEBasedOnDeltaMap in Eliminator.cpp\n";
		    exit(1);
		  }
		string id = id_delta.substr(0, index);
		string delta_str = id_delta.substr(index+1);	
		int delta = atoi(delta_str.c_str());
		int new_delta;

		if(DeltaMap.find(delta) != DeltaMap.end())
		  new_delta = DeltaMap[delta];
		else
		  new_delta = delta;
		
		string new_name = id;
		new_name += "_";
		char temp[100];
		sprintf(temp, "%d", new_delta);
		delta_str = temp;
		new_name += delta_str;

		Value *v = new Value();
		Type *t = new Type();
 		t->data_type = MyDataType;
		t->width = MyWidth;
		v->set_node_type(*t);
		clone = dm.createNode (new_name,(void*)v);
	      }
	    else
	      {
		clone = dm.createNode(parent->Get_node_name(), parent->getValue());
	      }
            
	  }
        return clone;
    }  


// Function to write declarations in SMTLib format
void writeBVDeclarationsInSMTLibFormat(FILE *smt_file, map<string, int> WidthTable)

{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {
	  fprintf(smt_file,":extrafuns ((%s",(it->first).c_str());
	  fprintf(smt_file," BitVec[%d]))\n",it->second);
	}
 
}

// Function to return the content of a dag (in propositional skeleton format) in SMTLIB format
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(DAGNode *root, map<string, int> WidthTable)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  propogateWidths(root, WidthTableForNodes, 1);
   
  return getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, "bool", WidthTableForNodes, 1);
}

// Function to traverse the dag node "node" and return the content of a dag (in propositional skeleton format) in SMTLIB format
// This is called by callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)
        {
               dag_str = name;
               return dag_str;
        }
        else
          {

            if(name == "select")
              {
                dag_str = "(extract[";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();

                iout++;

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "const", WidthTableForNodes, 1) + ":";

                iout = node->Get_outlist_start();

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "const", WidthTableForNodes, 1) + "] ";

                iout = node->Get_outlist_start();

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {


		dag_str = "(concat ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
		    if(isBVConstant(*iout)) // child is a bv-constant. our concatenation printing may be incorrect
		      {
			cout<<"\nconcat is not supported by function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable currently if any operator is a bv-constant. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		      exit(1);
		      }
			
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "bvmul")
	      {
		dag_str = "(bvmul ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "bvadd")
	      {
		dag_str = "(bvadd ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	     else if(name == "bvsub")
	      {
		dag_str = "(bvsub ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "bvor" || name == "bvand")
	      {
		dag_str =  "(" +  name + " ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "if_then_else"))
	      {
		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {

                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		
              }

	    else if(name == "'0'")
              {
                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);

              }

	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      return dag_str;
		      }
		else if(context == "bv")
		      {
	
			if(name[0] == 'x') // hex
			  {
			  dag_str = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			else // Decimal
			  {
			  dag_str = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
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



// Function to convert a decimal string into SMTLIB binary format, and return it
string convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(string dec, int Width)
{

  //if(Width>32) {cout<<"\nWidth more than 32 is not permitted by SMTLib format. Exiting\n"; exit(1);}

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

// Function to convert a hex string into SMTLIB binary format, and return it
string convertHexToBitvectorWithProperWidthInNewSMTLibFormat(string hex, int Width)
{

  //if(Width>32) {cout<<"\nWidth more than 32 is not permitted by SMTLib format. Exiting\n";}

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

// Function to convert a literal into SMTLIB binary format, and return it
string convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(string literal, int Width)
{
  
 string str;

 if(literal == "'1'")
   str="bv1[1]";
 else if(literal == "'0'")
   str="bv0[1]";
 else
   {cout<<"\nconvertLiteralToBitvectorWithProperWidthInNewSMTLibFormat in Eliminator.cpp called with literal = "<<literal<<endl; exit(1);}
 

 // cout<<"\nInput Number = "<<literal<<endl;
 //cout<<"\nBV in SMTLib format  = "<<str<<endl;

return str;
}



/* Functions to decide satisfiability of conjunction of LMEs and LMDs */

// Function to do QELIM from a conjunction of LMEs and LMDs using Layer 1 for deciding sat of the conjunction of LMEs and LMDs. 
// Should be called with  NoVarsToElim equal to number of variables
// Returns 'u' if unsat, 's' if sat, 'x' if unknown
char solveForEquationsAndDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod)
{
int VarToElimIndex, EqnToSubstIndex;

//cout<<"\nTrying to eliminate variables with odd coefficients first\n";

 for(VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++) 
   {
     //cout<<"\nEliminating the variable x"<<VarToElimIndex<<"\n";
  

//      char syntactic_check_result = syntacticCheckForSatisfiability(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat);

//      if(syntactic_check_result != 'x') // result obtained
//        {
// 	 return syntactic_check_result;
//        }      

       
     if(EqCoefMat.empty()) // No equations left. But there are disequations. Hence do not know about sat and no more processing possible in this function
       {
	 //cout<<"\nNo equations left in solveForEquationsAndDisequations\n"; 
	 break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has odd coefficient \n";
     
     getEquationWithOddCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex); 

     if(EqnToSubstIndex==-1) 
	{
	//fprintf(ImpCheckFile,"\nNo equation with odd coefficient for the variable\n");
	}

     else
	{
	  //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";

	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";

	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnEq, PivotColumnDiseq;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow

	//cout<<"\nChecking if the equation is solvable\n";
	// Check if the Equation selected is solvable. If no, return saying unsat
	if(!equationSolvable(PivotRow, PivotCoef, Mod))
	  {
	  fprintf(ImpCheckFile,"\nThe equation selected is unsolvable - found by gcd check. Hence set of equations is unsat.\n"); 
	  return 'u';
	  }


	copyColumn(EqCoefMat, VarToElimIndex, PivotColumnEq); // selected column copied into PivotColumnEqn
	
	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	// delete pivot element from pivot column of equations as this would lead to equation with all coefs set to zero 
	deleteRow(PivotColumnEq, EqnToSubstIndex);


	// PivotRow deleted from EqCoefMat
	deleteRow(EqCoefMat, EqnToSubstIndex);
	// PivotRow deleted from EqConstMat
	deleteRow(EqConstMat, EqnToSubstIndex);

	// PivotColumn nullified in EqCoefMat
	nullifyColumn(EqCoefMat, VarToElimIndex);


	//cout<<"\nAfter deletions\n";
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);

	Matrix NewEqCoefMat;
	// Multiply PivotColumnEq and PivotRow to get NewEqCoefMat
	matMultiply(PivotColumnEq, PivotRow, NewEqCoefMat, Mod);
	//cout<<"\nNewEqCoefMat\n";
	//showMatrix(NewEqCoefMat);

	// EqCoefMat =  EqCoefMat + NewEqCoefMat
	matAdd(EqCoefMat, NewEqCoefMat, Mod);
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);


	// Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef*MI(Pivot element) = "<<PivotCoef<<"\n";

	// Convert PivotColumnEq to PivotColumnEq*PivotCoef
	scalarMatMultiply(PivotColumnEq, PivotCoef, Mod);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	// EqConstMat = EqConstMat-PivotColumnEq
	matSub(EqConstMat, PivotColumnEq, Mod);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);

	//cout<<"\nAfter elimination, we have\n";
	//showEquations(EqCoefMat, EqConstMat);


	// For disequations

	if(!DisEqCoefMat.empty())
	  {
	    copyColumn(DisEqCoefMat, VarToElimIndex, PivotColumnDiseq); // selected column copied into PivotColumnDiseqn
	
	    // PivotColumn nullified in DisEqCoefMat
	    nullifyColumn(DisEqCoefMat, VarToElimIndex);
	
	    Matrix NewDisEqCoefMat;
	    // Multiply PivotColumnDiseq and PivotRow to get NewDisEqCoefMat
	    matMultiply(PivotColumnDiseq, PivotRow, NewDisEqCoefMat, Mod);
	    //cout<<"\nNewDisEqCoefMat\n";
	    //showMatrix(NewDisEqCoefMat);
	    
	    // DisEqCoefMat =  DisEqCoefMat + NewDisEqCoefMat
	    matAdd(DisEqCoefMat, NewDisEqCoefMat, Mod);
	    //cout<<"\nDisEqCoefMat\n";
	    //showMatrix(DisEqCoefMat);
	

	    // Convert PivotColumnDiseq to PivotColumnDiseq*PivotCoef
	    //cout<<"\nPivotColumnDiseq*PivotCoef\n";
	    scalarMatMultiply(PivotColumnDiseq, PivotCoef, Mod);
	    //cout<<"\nPivotColumnDiseq\n";
	    //showMatrix(PivotColumnDiseq);
	    
	    // DisEqConstMat = DisEqConstMat - PivotColumnDiseq
	    matSub(DisEqConstMat, PivotColumnDiseq, Mod);
	    //cout<<"\nDisEqConstMat\n";
	    //showMatrix(DisEqConstMat);
	    
	    //cout<<"\nAfter elimination, we have\n";
	    //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
	  }
	
	}
   }


 
 //cout<<"\nTrying to eliminate variables with even coefficients\n";

 for(int VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++)
   {

//      char syntactic_check_result = syntacticCheckForSatisfiability(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat);

//      if(syntactic_check_result != 'x') // result obtained
//        {
// 	 return syntactic_check_result;
//        }      



     if(EqCoefMat.empty()) 
       {
	 //cout<<"\nNo equations left in solveForEquationsAndDisequations\n"; 
	 break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has even coefficient \n";

     getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex, Mod); 
	
     //cout<<"\nEqnToSubstIndex= "<<EqnToSubstIndex<<endl;

    if(EqnToSubstIndex==-1) continue;

     if(EqCoefMat[EqnToSubstIndex][VarToElimIndex] == 0) 
	{
	//fprintf(ImpCheckFile,"\nNo equation with even coefficient for the variable.\n");
	}

     else
	{
	  //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";
		
	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";
	//cout<<"\nDividing Pivot Element by it's power of 2\n";
	
	ULINT PowerOfPivotElement=powerOfTwo(PivotElement);
	//cout<<"\nPower Of Pivot Element = "<<PowerOfPivotElement<<"\n";

	PivotElement=PivotElement/findPower(PowerOfPivotElement);
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";
	
	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnEq, PivotColumnDiseq;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	

	// Check if the Equation selected is solvable. If no, return saying unsat
	if(!equationSolvable(PivotRow, PivotCoef, Mod))
	  {
	    fprintf(ImpCheckFile,"\nThe equation selected is unsolvable - found by gcd check. Hence set of equations is unsat.\n"); 
	    return 'u';
	  }

	
	// As the coeff. selected is even, we need to add a new equality into the EqCoefMat

	Matrix NewEquality; // Note that this is a row matrix
	
	copyRow(EqCoefMat, EqnToSubstIndex, NewEquality); // selected equation copied into NewEquality
     
	// Pivot element nullified in  NewEquality 
	nullifyColumn(NewEquality, VarToElimIndex);

	// Convert NewEquality to AI(NewEquality)
	aInverseOfMatrix(NewEquality, Mod); 

	// Convert NewEquality to AI(NewEquality)*pow(2, powerOfTwo(Mod)-PowerOfPivotElement)
	scalarMatMultiply(NewEquality, findPower(powerOfTwo(Mod)-PowerOfPivotElement), Mod);

	// The constant part of the new equality in NewEqualityConst
	ULINT NewEqualityConst=PivotCoef;

	// Convert NewEqualityConst to NewEqualityConst*pow(2, powerOfTwo(Mod)-PowerOfPivotElement)
	NewEqualityConst = mul(NewEqualityConst, findPower(powerOfTwo(Mod)-PowerOfPivotElement), Mod);

	// Convert NewEqualityConst to AI(NewEqualityConst*pow(2, powerOfTwo(Mod)-PowerOfPivotElement))
	NewEqualityConst = aInverse(NewEqualityConst, Mod);
	// Now NewEquality = NewEqualityConst is a new equality to be added to EqCoefMat at the end of eliminating the variable

	
	// Rest is exactly like the way for odd coefficient
	
	copyColumn(EqCoefMat, VarToElimIndex, PivotColumnEq); // selected column copied into PivotColumnEqn
	
	// Dividing the PivotColumnEq by power(2, PowerOfPivotElement). Note that PivotColumnEq is always divisible
	divideColumnByPowerOf2IfDivisible(PivotColumnEq, 0, PowerOfPivotElement);
	
	
	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	// delete pivot element from pivot column of equations as this would lead to equation with all coefs set to zero 
	deleteRow(PivotColumnEq, EqnToSubstIndex);


	// PivotRow deleted from EqCoefMat
	deleteRow(EqCoefMat, EqnToSubstIndex);
	// PivotRow deleted from EqConstMat
	deleteRow(EqConstMat, EqnToSubstIndex);

	// PivotColumn nullified in EqCoefMat
	nullifyColumn(EqCoefMat, VarToElimIndex);
	
	

	//cout<<"\nAfter deletions\n";
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);

	Matrix NewEqCoefMat;
	// Multiply PivotColumnEq and PivotRow to get NewEqCoefMat
	matMultiply(PivotColumnEq, PivotRow, NewEqCoefMat, Mod);
	//cout<<"\nNewEqCoefMat\n";
	//showMatrix(NewEqCoefMat);

	// EqCoefMat =  EqCoefMat + NewEqCoefMat
	matAdd(EqCoefMat, NewEqCoefMat, Mod);
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);


	// Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef*MI(Pivot element) = "<<PivotCoef<<"\n";

	// Convert PivotColumnEq to PivotColumnEq*PivotCoef
	scalarMatMultiply(PivotColumnEq, PivotCoef, Mod);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	// EqConstMat = EqConstMat-PivotColumnEq
	matSub(EqConstMat, PivotColumnEq, Mod);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);

	//cout<<"\nAfter elimination, we have\n";
	//showEquations(EqCoefMat, EqConstMat);


	//cout<<"\nThe new equality is to be added\n";	
	//cout<<"\nChecking if new equality is to be added as the coefficient is even\n";
	 // Now NewEquality = NewEqualityConst is a new equality to be added to EqCoefMat and EqConstMat at the end of eliminating the variable unless all coefs in NewEquality are zero

     if(!IsNullMatrix(NewEquality))
       {
	 addRow(EqCoefMat, NewEquality); // NewEquality is a matrix
	 addElementToColumnMatrix(EqConstMat, NewEqualityConst);
	}
     else
       {
	 if(NewEqualityConst==0)
	   {
	   //fprintf(ImpCheckFile,"\nNew equality not added as it is 0 = 0\n");
	   }
	 else
	   {
	   cout<<"\nNull matrix <> 0 inside solveForEquationsAndDisequations function in Eliminator.cpp. Exiting...\n"; 
	   exit(1);
           }
       }

     //cout<<"\nAfter adding new equality, we have\n";
     //showEquations(EqCoefMat, EqConstMat);

	// For disequations

       
     if(!DisEqCoefMat.empty())
       {
	
	 copyColumn(DisEqCoefMat, VarToElimIndex, PivotColumnDiseq); // selected column copied into PivotColumnDiseqn
     
         // Dividing the PivotColumnDiseq by power(2, PowerOfPivotElement). Note that PivotColumnDiseq is not always divisible. If not divisible, element is set to zero
	divideColumnByPowerOf2IfDivisible(PivotColumnDiseq, 0, PowerOfPivotElement);

	// For DisEqCoefMat, in the column corresponding to the variable to be eliminated, if entry is not divisible by 2^PowerOfPivotElement, then keep it. Else make it zero.
	modifyDisEqCoefMat(DisEqCoefMat, VarToElimIndex, PowerOfPivotElement);	

	Matrix NewDisEqCoefMat;
	// Multiply PivotColumnDiseq and PivotRow to get NewDisEqCoefMat
	matMultiply(PivotColumnDiseq, PivotRow, NewDisEqCoefMat, Mod);
	//cout<<"\nNewDisEqCoefMat\n";
	//showMatrix(NewDisEqCoefMat);
	
	// DisEqCoefMat =  DisEqCoefMat + NewDisEqCoefMat
	matAdd(DisEqCoefMat, NewDisEqCoefMat, Mod);
	//cout<<"\nDisEqCoefMat\n";
	//showMatrix(DisEqCoefMat);
	

	// Convert PivotColumnDiseq to PivotColumnDiseq*PivotCoef
	scalarMatMultiply(PivotColumnDiseq, PivotCoef, Mod);
	//cout<<"\nPivotColumnDiseq\n";
	//showMatrix(PivotColumnDiseq);
	
	// DisEqConstMat = DisEqConstMat - PivotColumnDiseq
	matSub(DisEqConstMat, PivotColumnDiseq, Mod);
	//cout<<"\nDisEqConstMat\n";
	//showMatrix(DisEqConstMat);
	
	//cout<<"\nAfter elimination, we have\n";
	//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
       }

	}
   }

 // check if the set of LMEs and LMDs is sat/unsat syntactically

 char syntactic_check_result = syntacticCheckForSatisfiability(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat);

 //fprintf(ImpCheckFile, "\nsyntacticCheckForSatisfiability returns %c\n", syntactic_check_result); 

 return syntactic_check_result;
  
}


// check if the set of LMEs and LMDs is sat/unsat syntactically (trivial check)
// Returns 'u' if unsat, 's' if sat, 'x' if unknown
char syntacticCheckForSatisfiability(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat)
{
  if(EqCoefMat.empty() && DisEqCoefMat.empty()) // No more equations/disequations. Hence sat
    {
      return 's';
    }
  
  if(setOfEquationsSyntacticallyUnsat(EqCoefMat, EqConstMat))
    {
      return 'u';
    }

  if(setOfDisequationsSyntacticallyUnsat(DisEqCoefMat, DisEqConstMat))
    {
      return 'u';
    }
  return 'x';
}
  
  
// check if the set of LMEs sat/unsat syntactically (trivial check)
// Returns true if unsat, false if sat/unknown
bool setOfEquationsSyntacticallyUnsat(Matrix &MyCoefMatrix, Matrix &MyConstMatrix)
{
int rows = MyCoefMatrix.size();
 for(int i=0; i<rows; i++) // if there exists any row s.t. coef-row is zero, and const row is non-zero, then the set is unsat
		{ 
		if(rowIsNull(MyCoefMatrix, i) && MyConstMatrix[i][0]!=0)
		  {
		  return true;
		  }
		}
 return false; 
}


// check if the set of LMDs sat/unsat syntactically  (trivial check)
// Returns true if unsat, false if sat/unknown
bool setOfDisequationsSyntacticallyUnsat(Matrix &MyCoefMatrix, Matrix &MyConstMatrix)
{
int rows = MyCoefMatrix.size();
 for(int i=0; i<rows; i++) // if there exists any row s.t. coef-row is zero, and const row is zero, then the set is unsat
		{ 
		if(rowIsNull(MyCoefMatrix, i) && MyConstMatrix[i][0]==0)
		  {
		  return true;
		  }
		}
 return false; 
}

// Function to do QELIM from a conjunction of LMEs and LMDs for deciding sat of the conjunction of LMEs and LMDs. 
// Should be called with  NoVarsToElim equal to number of variables
// This uses only Layers 1&2 as the deeper layers are expensive
// Returns 'u' if unsat, 's' if sat, 'x' if unknown
char qEliminatorForSolving(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod)
{
  // Delete the duplicate contraints

  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  //cout<<"\nShowing original set of constraints\n";
  //showEquations(EqCoefMatPassed, EqConstMatPassed); // Show the equations
  //showDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed); // Show the disequations
  //cout<<"\nWe need to eliminate the first "<<NoVarsToElim<<" variables \n";

  //fprintf(ImpCheckFile, "\nShowing original set of constraints\n");
  //printEquations(EqCoefMatPassed, EqConstMatPassed, ImpCheckFile); // Show the equations
  //printDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed, ImpCheckFile); // Show the disequations
  //fprintf(ImpCheckFile, "\nWe need to eliminate the first %d variables\n", NoVarsToElim);


  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  
  //fprintf(ImpCheckFile, "\nShowing duplicate eliminated set of constraints\n");
  //printEquations(EqCoefMat, EqConstMat, ImpCheckFile); // Show the equations
  //printDisEquations(DisEqCoefMat, DisEqConstMat, ImpCheckFile); // Show the disequations




  Matrix OriginalEqCoefMat, OriginalEqConstMat; // Copy of equalities

  // Keep a copy of equations in OriginalEqCoefMat, OriginalEqConstMat
  copyMatrix(EqCoefMat, OriginalEqCoefMat);
  copyMatrix(EqConstMat, OriginalEqConstMat);
  
  //fprintf(ImpCheckFile,"\n************************************************************\n");

  //fprintf(ImpCheckFile, "\nEqualities and Disequalities at the beginning of the function\n");

  //fprintf(ImpCheckFile, "\nShowing the Equalities\n");
  //printEquations(EqCoefMat, EqConstMat, ImpCheckFile); // Show the equations
  //fprintf(ImpCheckFile, "\nShowing the Disequalities\n");
  //printDisEquations(DisEqCoefMat, DisEqConstMat, ImpCheckFile); // Show the disequations
  

  //fprintf(ImpCheckFile, "\nEliminating variables from Equations and Disequations\n");
  
  if(!EqCoefMat.empty())
    {
    char result_from_solveForEquationsAndDisequations = solveForEquationsAndDisequations(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);
    if(result_from_solveForEquationsAndDisequations != 'x')
	{
	return result_from_solveForEquationsAndDisequations;
	}
     }
  
  //fprintf(ImpCheckFile, "\nEqualities and Disequalities after LMA based elimination\n");  
  
  //fprintf(ImpCheckFile, "\nShowing the Equalities\n");
  //printEquations(EqCoefMat, EqConstMat, ImpCheckFile); // Show the equations
  //fprintf(ImpCheckFile, "\nShowing the Disequalities\n");
  //printDisEquations(DisEqCoefMat, DisEqConstMat, ImpCheckFile); // Show the disequations


  //fprintf(ImpCheckFile, "\nDropping the unconstraining disequations\n");
  // Note that for dropping unconstraining disequations, we should look at the maximum no: of bits constrained by any equality. The only thing we want from the set of equations in fact is the most constraining equality (Look at the proof for further clarification)


  if(!DisEqCoefMat.empty())
	{
	dropUnconstrainingDisequationsModified(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);
	}

   //fprintf(ImpCheckFile, "\nEqualities and Disequalities after dropping LMDs\n");  
   //fprintf(ImpCheckFile, "\nShowing the Equalities\n");
   //printEquations(EqCoefMat, EqConstMat, ImpCheckFile); // Show the equations
   //fprintf(ImpCheckFile, "\nShowing the Disequalities\n");
   //printDisEquations(DisEqCoefMat, DisEqConstMat, ImpCheckFile); // Show the disequations

   eliminateTrivialEquationsAndDisequations(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat); // Eliminating trivial equations and disequations

   //fprintf(ImpCheckFile, "\nEqualities and Disequalities after eliminating trivial LMEs and LMDs\n");  
   //fprintf(ImpCheckFile, "\nShowing the Equalities\n");
   //printEquations(EqCoefMat, EqConstMat, ImpCheckFile); // Show the equations
   //fprintf(ImpCheckFile, "\nShowing the Disequalities\n");
   //printDisEquations(DisEqCoefMat, DisEqConstMat, ImpCheckFile); // Show the disequations

  if(EqCoefMat.empty() && DisEqCoefMat.empty()) // No more equations/disequations. Hence sat
    {
      return 's';
    }
  // dropping disequations cannot reveal any more unsat cases
  return 'x';
}

// Given map<int, ULINT> CoefsOfVarToElimEqualities, it returns maximum of the second field
// of CoefsOfVarToElimEqualities if CoefsOfVarToElimEqualities is non-empty, and zero
// if CoefsOfVarToElimEqualities is empty
ULINT findMaximumKValue(map<int, ULINT> &CoefsOfVarToElimDisEqualities)
{
ULINT MaximumK=0;
for(map<int, ULINT>::iterator it=CoefsOfVarToElimDisEqualities.begin(); it != CoefsOfVarToElimDisEqualities.end(); it++)
	{
	if(it->second>MaximumK)
		MaximumK = it->second;
	}
return MaximumK;
} 


// Functions added from 19 July 2010 to implement eager splitting

// Implements synthesizeLMEs in CAV'11 paper
// This synthesizes new LMEs from the given problem instance, which simplifies the given instance
// Returns true if eager splitting is not required; false otherwise
bool bitLevelReasoning(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int VarToElimIndex, ULINT Mod)
{
  //cout<<"\nIn function bitLevelReasoning\n";
map<int, ULINT> KValueOfVarToElimEqualities, KValueOfVarToElimDisEqualities;
// The map stores k-values of variable to be eliminated for each equality & disequality. If coef of variable to be eliminated for the disequality is zero, then it is not stored in the map
if(EqCoefMat.empty()) 
	{
	cout<<"\nNo bit-level reasoning possible. There are no LMEs. Hence Eager Splitting needed\n";
	return true;
	}

copyColumnToMap(EqCoefMat, VarToElimIndex, KValueOfVarToElimEqualities);
copyColumnToMap(DisEqCoefMat, VarToElimIndex, KValueOfVarToElimDisEqualities);

// Currently KValueOfVarToElimEqualities, KValueOfVarToElimDisEqualities store the coefficients; not the k values
		
if(KValueOfVarToElimDisEqualities.empty())  // variable not present in disequalities. No need to eliminate
	  {
	    cout<<"\nVariable x"<<VarToElimIndex<<" is already eliminated from disequations\n";
	    return false;
	  }
// Convert them to powers of 2

convertCoefToPowerOf2(KValueOfVarToElimEqualities);
convertCoefToPowerOf2(KValueOfVarToElimDisEqualities);

//cout<<"\nKValueOfVarToElimEqualities\n";
//showMap(KValueOfVarToElimEqualities);
//cout<<"\nKValueOfVarToElimDisEqualities\n";
//showMap(KValueOfVarToElimDisEqualities);

ULINT KEqn = findMinimumKValue(KValueOfVarToElimEqualities, Mod);

cout<<"\nKEqn = "<<KEqn<<endl;

set<int> BitLevelLMDs;

for(int i=0;i<DisEqCoefMat.size();i++)
	{
	  //cout<<"\nLMD coefficent row matrix selected is \n";
	  //showRow(DisEqCoefMat[i]);
	  //cout<<"\nconstant = "<<DisEqConstMat[i][0]<<endl;
	

	if(KValueOfVarToElimDisEqualities.find(i)==KValueOfVarToElimDisEqualities.end()) //This LMD is free of the var. to elim		
		{
		  //cout<<"\nLMD is free of variable to be eliminated\n";
		continue;
		}

	ULINT kDiseqn = KValueOfVarToElimDisEqualities[i];
	if(kDiseqn>=KEqn) // diseqn can be eliminated
		{
		  //cout<<"\nLME exists with lesser k value\n";
		continue;
		}
	
	if(!constrainingEquationExists(EqCoefMat, EqConstMat, DisEqCoefMat[i], DisEqConstMat[i][0], Mod)) // constraining equation e exists such that e is 2.lhs = 2.rhs where lhs <> rhs
		{
		  //cout<<"\nNo constraining LME exists\n";
		continue;
		}
	BitLevelLMDs.insert(i);	
	}

cout<<"\nBitLevelLMDs : i.e. LMDs such that they constrain only a single bit\n";
for(set<int>::iterator it=BitLevelLMDs.begin();it!=BitLevelLMDs.end();it++)
	{
	cout<<"\nLMD coefficent row is \n";
	showRow(DisEqCoefMat[*it]);
	cout<<"\nconstant = "<<DisEqConstMat[*it][0]<<endl;
	}
cout<<"\nThey constrain the bit "<<powerOfTwo(Mod)-KEqn<<endl;

if(BitLevelLMDs.size()<=1) // we need at least two LMDs to do bit-level reasoning
	{
	cout<<"\n<=1 bit-level LMD. No need to do bit-level reasoning\n";
	return true;
	}

Matrix BitLevelLMDsNormalizedCoef;
Matrix BitLevelLMDsNormalizedConst;

for(set<int>::iterator it=BitLevelLMDs.begin();it!=BitLevelLMDs.end();it++)
	{
	int lmd_index=*it;
	Matrix LMDCoef;
	ULINT LMDConst;

	copyRow(DisEqCoefMat, lmd_index, LMDCoef);
	LMDConst = DisEqConstMat[lmd_index][0];

	ULINT OddPart = getOddPart(LMDCoef[0][VarToElimIndex]);
	nullifyColumn(LMDCoef, VarToElimIndex);
	aInverseOfMatrix(LMDCoef, Mod);
	scalarMatMultiply(LMDCoef, OddPart, Mod);
	LMDConst = mul(LMDConst, OddPart, Mod);
	Row LMDConstRow;
	LMDConstRow.push_back(LMDConst);
	
	BitLevelLMDsNormalizedCoef.push_back(LMDCoef[0]);
	BitLevelLMDsNormalizedConst.push_back(LMDConstRow);
	}

//cout<<"\nBitLevelLMDsNormalized\n";
//for(int i=0;i<BitLevelLMDsNormalizedCoef.size();i++)
//	{
//	cout<<"\nrow matrix is \n";
//	showRow(BitLevelLMDsNormalizedCoef[i]);
//	cout<<"\nconstant = "<<BitLevelLMDsNormalizedConst[i][0]<<endl;
//	}

 Matrix NewLMEsCoefMat;
 Matrix NewLMEsConstMat;

 Matrix PivotEqnCoefMat;
 ULINT PivotEqnConst;

 copyRow(BitLevelLMDsNormalizedCoef, 0, PivotEqnCoefMat);
 PivotEqnConst = BitLevelLMDsNormalizedConst[0][0];
 
 for(int i=1;i<BitLevelLMDsNormalizedCoef.size();i++)
   {
     Matrix NewEqnCoefMat;
     ULINT NewEqnConst;

     copyRow(BitLevelLMDsNormalizedCoef, i, NewEqnCoefMat);
     NewEqnConst = BitLevelLMDsNormalizedConst[i][0];

     
     aInverseOfMatrix(NewEqnCoefMat, Mod);
     NewEqnConst = aInverse(NewEqnConst, Mod);

     matAdd(NewEqnCoefMat, PivotEqnCoefMat, Mod);
     NewEqnConst = add(NewEqnConst, PivotEqnConst, Mod);
     
     NewLMEsCoefMat.push_back(NewEqnCoefMat[0]);

     Row NewLMEConstRow;
     NewLMEConstRow.push_back(NewEqnConst);
     NewLMEsConstMat.push_back(NewLMEConstRow);
   }

 // add these new LMEs into the set of LMEs and LMDs in the problem

 for(int i=0;i<NewLMEsCoefMat.size();i++)
   {
     Matrix TempEqnCoefMat;
     Matrix TempEqnConstMat;

     copyRow(NewLMEsCoefMat, i, TempEqnCoefMat);
     copyRow(NewLMEsConstMat, i, TempEqnConstMat);

     EqCoefMat.push_back(TempEqnCoefMat[0]);
     EqConstMat.push_back(TempEqnConstMat[0]);
   }

 // Removing all LMDs where bit-level reasoning is done except one

 list<int> DisequationsToEliminate;
 set<int>::iterator it=BitLevelLMDs.begin();
 it++;
 for(;it!=BitLevelLMDs.end();it++)
	{
	  DisequationsToEliminate.push_back(*it);
	}

  // Eliminate them in reverse order so that after each elimination, the row indexes are intact.

 DisequationsToEliminate.reverse();
  
 for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
    {
      //cout<<"\nDisequation "<<*it<<" can be eliminated\n";
      deleteRow(DisEqCoefMat, *it);
      deleteRow(DisEqConstMat, *it);
    }
      
 return false;
 
}

// show a row matrix
void showRow(Row &MyRow)
{
int size = MyRow.size();
 cout<<"\n";
for(int i=0;i<size;i++)
  cout<<MyRow[i]<<"\t";
 cout<<"\n";
}

// returns true if there exists an LME e in E s.t. e is like 2.lhs = 2.rhs, and LMD d is like lhs <> rhs; false otherwise
bool constrainingEquationExists(Matrix &EqCoefMat, Matrix &EqConstMat, Row &DisEqCoef, ULINT DisEqConst, ULINT Mod)
{
for(int i=0;i<EqCoefMat.size();i++)
	{
	if(equationPairOfDisequation(EqCoefMat[i], EqConstMat[i][0], DisEqCoef, DisEqConst, Mod))
		{
		return true;
		}
	}
return false;
}

// called by constrainingEquationExists to check if LME e is like 2.lhs = 2.rhs, and LMD d is like lhs <> rhs.
// returns true if exists; false otherwise
bool equationPairOfDisequation(Row &EqCoef, ULINT EqConst, Row &DisEqCoef, ULINT DisEqConst, ULINT Mod)
{
for(int i=0;i<EqCoef.size();i++)
	{
	ULINT Coef_Eqn = EqCoef[i];
	ULINT Coef_DisEqn = DisEqCoef[i];
	if(Coef_Eqn != mul(Coef_DisEqn, 2, Mod))
		{
		return false;
		}
	}
if(EqConst != mul(DisEqConst, 2, Mod))
		{
		return false;
		}
return true;
}

// Function to implement the eager splitting scheme in CAV'11 paper
DAGNode* eagerSplitReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable)
{
DAGNode* resultDAG=NULL;

bool EagerSplitting = bitLevelReasoning(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, VarToElimIndex, Mod);

cout<<"\nLMEs and LMDs after bit-level reasoning\n";
showEquations(EqCoefMat, EqConstMat); 
showDisEquations(DisEqCoefMat, DisEqConstMat);  

 if(EagerSplitting)
   {
     cout<<"\nEager Splitting needed \n";
     resultDAG = eagerSplitInternalReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, VarToElimIndex, Mod, NoVarsToElim, SplitFlag, VarNames, WidthTable);
   }

else
  {
   cout<<"\nEager Splitting is not needed\n";
   resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, SplitFlag, VarNames, WidthTable);
    }

return resultDAG;

}


// return x ^ number
ULINT findPowerOfAnyNumber(ULINT x, ULINT number)
{
  ULINT count=0;
  ULINT value=1;
  while(count<number) {count=count+1; value=value*x;}
  return value;
}


// Function which actually does the eager splitting
DAGNode* eagerSplitInternalReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable)
{
DAGNode *resultDAG=NULL;

if(EqCoefMat.empty())//case where there are no LMEs
{
ULINT Base = powerOfTwo(Mod);
cout<<"\nBase = "<<Base<<endl;
ULINT max=findPowerOfAnyNumber(Base, DisEqCoefMat.size())-1; // We have Base^(#disequations) combinations possible
cout<<"\nMax no: iterations = "<<max<<"\n";

Matrix DisEqFreeOfSplittingCoefMat, DisEqFreeOfSplittingConstMat;
eliminateDisequationsFreeOfSplittingAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, VarToElimIndex, DisEqFreeOfSplittingCoefMat, DisEqFreeOfSplittingConstMat, powerOfTwo(Mod));


for(ULINT count=0;count<=max;count++)
   {
     cout<<"\nStarting loop iteration number "<<count<<" \n";

     Matrix NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat;

     addRows(NewEqCoefMat, EqCoefMat); // The already present equations are added to the new constraints
     addRows(NewEqConstMat, EqConstMat);

     addRows(NewDisEqCoefMat, DisEqFreeOfSplittingCoefMat); // The disequations free of splitting added to the new constraints
     addRows(NewDisEqConstMat, DisEqFreeOfSplittingConstMat);
     
     for(int i=0; i<DisEqCoefMat.size(); i++)
		{
                  cout<<"\nDisequation number = "<<i<<endl;
		  ULINT WhichConstraint = getTypeOfConstraintForEagerSplitting(count, i, Base);
		  cout<<"\nWhichConstraint = "<<WhichConstraint<<"\n";
		  ULINT PowerToMultiplyToGetNewLMD = WhichConstraint;
		  ULINT PowerToMultiplyToGetNewLME = WhichConstraint+1;

		  cout<<"\nPowerToMultiplyToGetLMD = "<<PowerToMultiplyToGetNewLMD<<endl;
		  cout<<"\nPowerToMultiplyToGetLME = "<<PowerToMultiplyToGetNewLME<<endl;


		  // Row matrices representing costraint1 i.e. reduced disequality
		  Matrix Constraint1_Diseq_Coef;
		  Matrix Constraint1_Diseq_Const;

		  // disequality is nothing but the original disequality multiplied by 2^PowerToMultiplyToGetNewLMD
		  copyRow(DisEqCoefMat, i, Constraint1_Diseq_Coef);
		  copyRow(DisEqConstMat, i, Constraint1_Diseq_Const);
		  scalarMatMultiply(Constraint1_Diseq_Coef, findPower(PowerToMultiplyToGetNewLMD), Mod);
		  scalarMatMultiply(Constraint1_Diseq_Const, findPower(PowerToMultiplyToGetNewLMD), Mod);


		  if( !(IsNullMatrix(Constraint1_Diseq_Coef) && IsNullMatrix(Constraint1_Diseq_Const)) )
		    {
		      addRow(NewDisEqCoefMat, Constraint1_Diseq_Coef); // reduced disequality is added to the new constraint
		      addRow(NewDisEqConstMat, Constraint1_Diseq_Const);
		    }

		  // Row matrices representing costraint2 i.e. reduced equality
		  // equality is nothing but the original disequality multiplied by 2^PowerToMultiplyToGetNewLME. But this needs to be done only if PowerToMultiplyToGetNewLME<Base

		if(PowerToMultiplyToGetNewLME<Base){

		  Matrix Constraint1_Eq_Coef;
		  Matrix Constraint1_Eq_Const;

		  copyRow(DisEqCoefMat, i, Constraint1_Eq_Coef);
		  copyRow(DisEqConstMat, i, Constraint1_Eq_Const);
		  scalarMatMultiply(Constraint1_Eq_Coef, findPower(PowerToMultiplyToGetNewLME), Mod);
		  scalarMatMultiply(Constraint1_Eq_Const, findPower(PowerToMultiplyToGetNewLME), Mod);

		  if( !(IsNullMatrix(Constraint1_Eq_Coef) && IsNullMatrix(Constraint1_Eq_Const)) )
		    {
		      addRow(NewEqCoefMat, Constraint1_Eq_Coef); // reduced equality is added to the new constraint
		      addRow(NewEqConstMat, Constraint1_Eq_Const);
		    }
		}
		  
	}
    
     
     cout<<"\nConstraints obtained in loop iteration number "<<count<<" \n";
     cout<<"\nShowing the Equalities\n";
     showEquations(NewEqCoefMat, NewEqConstMat); // Show the equations
     cout<<"\nShowing the Disequalities\n";
     showDisEquations(NewDisEqCoefMat, NewDisEqConstMat); // Show the disequations


     cout<<"\nApplying Bit-level reasoning\n";

     bool EagerSplitting = bitLevelReasoning(NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat, VarToElimIndex, Mod);

     cout<<"\nLMEs and LMDs after bit-level reasoning\n";
     showEquations(NewEqCoefMat, NewEqConstMat); 
     showDisEquations(NewDisEqCoefMat, NewDisEqConstMat);  


    /* if(EagerSplitting)
       {
	 cout<<"\nUnable to eliminate even after eager splitting.\n";
	 exit(1);
       }*/

     DAGNode* DAGFromqElim = qEliminatorReturningDAG(NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat, NoVarsToElim, Mod, SplitFlag, VarNames, WidthTable);

     if(DAGFromqElim != NULL)
       {

         cout<<"\nDAGFromqElim = "<<getDAGStr(DAGFromqElim)<<endl;
	 if(resultDAG==NULL) 
	   resultDAG = DAGFromqElim; 
	 else 
	   {
           resultDAG = createDAG("or",resultDAG, DAGFromqElim, LOGICAL_SYMBOL, 1);
	   }
       }
     
   }
   cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl; 
   return resultDAG;
   
}// case where there are no LMEs ends here
else // case where there are LMEs
  {

ULINT KEqn;
ULINT PivotElement;

// Get KEqn
if(EqCoefMat.empty())
	{
	KEqn = powerOfTwo(Mod);
	cout<<"\nKEqn = "<<KEqn<<endl;
	}
else
	{
	int EqnIndex;
	getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnIndex, Mod);
	Matrix EqnCoef;
	copyRow(EqCoefMat, EqnIndex, EqnCoef);
	PivotElement=EqnCoef[0][VarToElimIndex]; 
	KEqn=powerOfTwo(PivotElement);
	cout<<"\nKEqn = "<<KEqn<<endl;
	}
// KEqn obtained

// There can be disequations free of the variable to be splitted. Also, there may be disequations such that splitting is not required to eliminate the variable from then. They need not be splitted. We will add them to each set of constraints after splitting separately. These LMDs can be cut from DisEqCoefMat, DisEqConstMat and can be stored in DisEqFreeOfSplittingCoefMat, DisEqFreeOfSplittingConstMat
Matrix DisEqFreeOfSplittingCoefMat, DisEqFreeOfSplittingConstMat;
eliminateDisequationsFreeOfSplittingAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, VarToElimIndex, DisEqFreeOfSplittingCoefMat, DisEqFreeOfSplittingConstMat, KEqn);


//cout<<"\nShowing the Disequalities Free of Splitting\n";
//cout<<"\nDisEqFreeOfSplittingCoefMat\n";
//showMatrix(DisEqFreeOfSplittingCoefMat);
//cout<<"\nDisEqFreeOfSplittingConstMat\n";
//showMatrix(DisEqFreeOfSplittingConstMat);

//cout<<"\nShowing the Disequalities\n";
//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations


// Split each of disequation in DisEqCoefMat <> DisEqConstMat  constraints
// Use a counter to generate all the possible combinations of splitted parts


// Get KDisEqnMin
int DisEqnIndex;
getEquationWithLeastEvenCoefForGivenVariable(DisEqCoefMat, VarToElimIndex, DisEqnIndex, Mod); // The function getEquationWithLeastEvenCoefForGivenVariable does not depend on whether constraints are equations or not. Hence it can be applied here also
Matrix DisEqnCoef;
copyRow(DisEqCoefMat, DisEqnIndex, DisEqnCoef);
PivotElement=DisEqnCoef[0][VarToElimIndex]; 
ULINT KDisEqnMin=powerOfTwo(PivotElement);
cout<<"\nKDisEqnMin = "<<KDisEqnMin<<endl;
// KDisEqnMin obtained

ULINT Base = KEqn - KDisEqnMin;
ULINT max=findPowerOfAnyNumber(Base, DisEqCoefMat.size())-1; // We have Base^(#disequations) combinations possible
cout<<"\nMax no: iterations = "<<max<<"\n";

 

 for(ULINT count=0;count<=max;count++)
   {
     cout<<"\nStarting loop iteration number "<<count<<" \n";

     Matrix NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat;

     addRows(NewEqCoefMat, EqCoefMat); // The already present equations are added to the new constraints
     addRows(NewEqConstMat, EqConstMat);

     addRows(NewDisEqCoefMat, DisEqFreeOfSplittingCoefMat); // The disequations free of splitting added to the new constraints
     addRows(NewDisEqConstMat, DisEqFreeOfSplittingConstMat);
     
     for(int i=0; i<DisEqCoefMat.size(); i++)
		{
                  //cout<<"\nDisequation number = "<<i<<endl;
		  ULINT WhichConstraint = getTypeOfConstraintForEagerSplitting(count, i, Base);
		  //cout<<"\nWhichConstraint = "<<WhichConstraint<<"\n";
		  ULINT PowerToMultiplyToGetNewLMD = WhichConstraint;
		  ULINT PowerToMultiplyToGetNewLME = WhichConstraint+1;

		  //cout<<"\nPowerToMultiplyToGetLMD = "<<PowerToMultiplyToGetNewLMD<<endl;
		  //cout<<"\nPowerToMultiplyToGetLME = "<<PowerToMultiplyToGetNewLME<<endl;


		  // Row matrices representing costraint1 i.e. reduced disequality
		  Matrix Constraint1_Diseq_Coef;
		  Matrix Constraint1_Diseq_Const;

		  // disequality is nothing but the original disequality multiplied by 2^PowerToMultiplyToGetNewLMD
		  copyRow(DisEqCoefMat, i, Constraint1_Diseq_Coef);
		  copyRow(DisEqConstMat, i, Constraint1_Diseq_Const);
		  scalarMatMultiply(Constraint1_Diseq_Coef, findPower(PowerToMultiplyToGetNewLMD), Mod);
		  scalarMatMultiply(Constraint1_Diseq_Const, findPower(PowerToMultiplyToGetNewLMD), Mod);


		  if( !(IsNullMatrix(Constraint1_Diseq_Coef) && IsNullMatrix(Constraint1_Diseq_Const)) )
		    {
		      addRow(NewDisEqCoefMat, Constraint1_Diseq_Coef); // reduced disequality is added to the new constraint
		      addRow(NewDisEqConstMat, Constraint1_Diseq_Const);
		    }

		  // Row matrices representing costraint2 i.e. reduced equality
		  // equality is nothing but the original disequality multiplied by 2^PowerToMultiplyToGetNewLME. But this needs to be done only if PowerToMultiplyToGetNewLME<Base

		if(PowerToMultiplyToGetNewLME<Base){

		  Matrix Constraint1_Eq_Coef;
		  Matrix Constraint1_Eq_Const;

		  copyRow(DisEqCoefMat, i, Constraint1_Eq_Coef);
		  copyRow(DisEqConstMat, i, Constraint1_Eq_Const);
		  scalarMatMultiply(Constraint1_Eq_Coef, findPower(PowerToMultiplyToGetNewLME), Mod);
		  scalarMatMultiply(Constraint1_Eq_Const, findPower(PowerToMultiplyToGetNewLME), Mod);

		  if( !(IsNullMatrix(Constraint1_Eq_Coef) && IsNullMatrix(Constraint1_Eq_Const)) )
		    {
		      addRow(NewEqCoefMat, Constraint1_Eq_Coef); // reduced equality is added to the new constraint
		      addRow(NewEqConstMat, Constraint1_Eq_Const);
		    }
		}
		  
	}
    
     
     cout<<"\nConstraints obtained in loop iteration number "<<count<<" \n";
     cout<<"\nShowing the Equalities\n";
     showEquations(NewEqCoefMat, NewEqConstMat); // Show the equations
     cout<<"\nShowing the Disequalities\n";
     showDisEquations(NewDisEqCoefMat, NewDisEqConstMat); // Show the disequations


     cout<<"\nApplying Bit-level reasoning\n";

     bool EagerSplitting = bitLevelReasoning(NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat, VarToElimIndex, Mod);

     cout<<"\nLMEs and LMDs after bit-level reasoning\n";
     showEquations(NewEqCoefMat, NewEqConstMat); 
     showDisEquations(NewDisEqCoefMat, NewDisEqConstMat);  


    /* if(EagerSplitting)
       {
	 cout<<"\nUnable to eliminate even after eager splitting.\n";
	 exit(1);
       }*/

     DAGNode* DAGFromqElim = qEliminatorReturningDAG(NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat, NoVarsToElim, Mod, SplitFlag, VarNames, WidthTable);

     if(DAGFromqElim != NULL)
       {
	 if(resultDAG==NULL) 
	   resultDAG = DAGFromqElim; 
	 else 
	   {
           resultDAG = createDAG("or",resultDAG, DAGFromqElim, LOGICAL_SYMBOL, 1);
	   }
       }
     
   }
   return resultDAG;
}// Case where there are LMEs

}// End of function


// This function finds the LMDs free of the variable to be eliminated before splitting,
// and copies them in DisEqFreeOfSplittingCoefMat, DisEqFreeOfSplittingConstMat
void eliminateDisequationsFreeOfSplittingAndCopyInNewMatrix(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int VarToElimIndex, Matrix &DisEqFreeOfSplittingCoefMat, Matrix &DisEqFreeOfSplittingConstMat, ULINT KEqn)
{
  list<int> FreeDiseqs;
 for(int i=0; i<DisEqCoefMat.size(); i++)
		{
		  if(DisEqCoefMat[i][VarToElimIndex] == 0) // Disequation i free of variable VarToElimIndex
			{
			FreeDiseqs.push_back(i);
			}
		else if(powerOfTwo(DisEqCoefMat[i][VarToElimIndex]) >= KEqn)
			{
			FreeDiseqs.push_back(i);
			}
		}

 FreeDiseqs.reverse();

 // reverse the list so that the indices remain intact even after each deletion

 for(list<int>::iterator it=FreeDiseqs.begin(); it != FreeDiseqs.end(); it++)
   {
     copyRow(DisEqCoefMat, *it, DisEqFreeOfSplittingCoefMat); // copyRow does not depend on whether DisEqFreeOfSplittingCoefMat is row matrix or if it is empty
     deleteRow(DisEqCoefMat, *it);
     
     copyRow(DisEqConstMat, *it, DisEqFreeOfSplittingConstMat); // copyRow does not depend on whether DisEqFreeOfSplittingConstMat is row matrix or if it is empty
     deleteRow(DisEqConstMat, *it);
   }
}

// Return the value of bit at location loc in the value count when expressed with base as Base
ULINT getTypeOfConstraintForEagerSplitting(ULINT count, ULINT loc, ULINT Base)
{
  ULINT power=findPowerOfAnyNumber(Base, loc);
  ULINT type=count/power; // Let count=3 loc=1 power=2 3/2 gives 1. This is in fact >> by loc times 
  return type%Base;
}



// check if there exists LMDs which contain any of the variables to be eliminated
// returns true if exists; false otherwise
bool findEliminationDone(Matrix DisEqCoefMat, int NoVarsToElim)
{
  bool EliminationDone=true; // Is elimination already done?
  int VarToElimIndex; // Index of the first variable still to be eliminated 
  if(!DisEqCoefMat.empty())
    {
      for(VarToElimIndex=0; VarToElimIndex<NoVarsToElim; VarToElimIndex++)
	{
	  if(!columnIsNull(DisEqCoefMat, VarToElimIndex))  // Column for variable to eliminate is non null => variable is not eliminated
		    {
		      //cout<<"\nVariable x"<<VarToElimIndex<<" is present in disequations\n";
		     EliminationDone=false;
		     break;
		    }
	}
    }
  return EliminationDone;
}

// updates the global variable APQElim_Calls
void update_APQElim_Calls()
{
  if(!apqelim_status_on) return;
  APQElim_Calls++;
}

// updates the global variable Multivar_APQElim_Calls
void update_Multivar_APQElim_Calls()
{
 if(!apqelim_status_on) return;
  Multivar_APQElim_Calls++;
} 

// updates the global variable APQElim_Calls_Solved_By_QElimLMA
void update_APQElim_Calls_Solved_By_QElimLMA(Matrix DisEqCoefMat, int NoVarsToElim)
{
  if(!apqelim_status_on) return;
  if(findEliminationDone(DisEqCoefMat, NoVarsToElim)) 
    {
      APQElim_Calls_Solved_By_QElimLMA++;
    }
}

// updates the global variable APQElim_Calls_Solved_By_DropLMDs_1
void update_APQElim_Calls_Solved_By_DropLMDs_1(Matrix DisEqCoefMat, int NoVarsToElim)
{
  if(!apqelim_status_on) return;
  if(findEliminationDone(DisEqCoefMat, NoVarsToElim)) 
    {
      APQElim_Calls_Solved_By_DropLMDs_1++;
    }
}

// updates the global variable APQElim_Calls_Solved_By_DropLMDs
void update_APQElim_Calls_Solved_By_DropLMDs(Matrix DisEqCoefMat, int NoVarsToElim)
{
  if(!apqelim_status_on) return;
  if(findEliminationDone(DisEqCoefMat, NoVarsToElim)) 
    {
      APQElim_Calls_Solved_By_DropLMDs++;
    }
}

// updates the global variable APQElim_Calls_Given_To_APQElim_Exp
void update_APQElim_Calls_Given_To_APQElim_Exp(Matrix DisEqCoefMat, int NoVarsToElim)
{
  if(!apqelim_status_on) return;
  if(!findEliminationDone(DisEqCoefMat, NoVarsToElim)) 
    {
      APQElim_Calls_Given_To_APQElim_Exp++;
    }
}
    
// Splits the dag node which is a conjunction of APs into a set of APs in Conjuncts   
void splitConjunctionIntoSet(DAGNode* Constraint, set<DAGNode*> &Conjuncts)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "=" || MyName == "is_not_equal") // terminal reached
    {
      Conjuncts.insert(Constraint);
    }

  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoSet in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoSet(*iout, Conjuncts);
      
      iout++;

      splitConjunctionIntoSet(*iout, Conjuncts);
    }
  else 
    {
     cout<<"\nunkwnon node "<<MyName<<" encountered in function splitConjunctionIntoSet in Eliminator.cpp\n";
     exit(1); 
    }
}

// show a set of DAGNodes
void showAPSet(string WhoamI, set<DAGNode*> &APSet)
{
  cout<<"\n"<<WhoamI<<"\n"<<"**************"<<"\n";
  set<DAGNode*>::iterator it=APSet.begin();
  for(;it!=APSet.end(); it++)
    {
      cout<<endl<<*it<<"\t"<<getDAGStr(*it)<<endl;
    }
}


// show a set of DAGNodes in a given file
void showAPSet(string WhoamI, set<DAGNode*> &APSet, FILE *fp)
{
  fprintf(fp, "\n%s\n****************\n", WhoamI.c_str());
  set<DAGNode*>::iterator it=APSet.begin();
  for(;it!=APSet.end(); it++)
    {
      fprintf(fp, "\n%s\n", (getDAGStr(*it)).c_str());
    }
}

// Given a set of DAGNodes, which are LMCs, convert them into matrix representation
// A pair of matrices for LMEs, and another pair for LMDs is generated
bool getEquationsAndDisequations(set<DAGNode*> &Constraints, list<string> &VarNames, int &NoOfBits, int &NoOfEqs, int &NoOfDisEqs, Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, map<string, int> &WidthTable)
{

  NoOfEqs=0;
  NoOfDisEqs=0;
  NoOfBits=0;

  int NoOfBitsInConstraint;

  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
      DAGNode* root = (*it);

      NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);

      if(NoOfBitsInConstraint > NoOfBits)
        NoOfBits=NoOfBitsInConstraint;
    }


  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
      DAGNode* root = (*it);

      NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);

      string type = root->Get_node_name();

      DAGNode *lhs, *rhs;
      list<DAGNode*>::iterator out_list_it = root->Get_outlist_start();
      lhs = (*out_list_it);
      out_list_it++;
      rhs = (*out_list_it);

      Matrix A, C; // Row matrices
      Matrix B, D; // Row matrices

      getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
      getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D

      /*cout<<"\nlhs = "<<getDAGStr(lhs)<<endl;
      cout<<"\nA\n";
      showMatrix(A);
      cout<<"\nB\n";
      showMatrix(B);
      cout<<"\nrhs = "<<getDAGStr(rhs)<<endl;
      cout<<"\nC\n";
      showMatrix(C);
      cout<<"\nD\n";
      showMatrix(D);*/



      // We have AX+B op CX+D
      // Convert A to A-C and D to D-B to get (A-C)X op (D-B)

      ULINT LocalMod=findPower(NoOfBitsInConstraint);
      ULINT GlobalMod=findPower(NoOfBits);
      ULINT Multiple=findPower(NoOfBits-NoOfBitsInConstraint);

      matSub(A, C, LocalMod);
      matSub(D, B, LocalMod);


      if(LocalMod == 2 && type=="is_not_equal")
        {
          // currently constant is in D[0][0]. we are converting the diseqn to eqn
          if(D[0][0]==1) D[0][0]=0; else D[0][0]=1;
          type = "=";
        }

 
      scalarMatMultiply(A, Multiple, GlobalMod);
      scalarMatMultiply(D, Multiple, GlobalMod);

      // Final matrices are A and D

      if(!IsNullMatrix(A))
        {
          if(type=="=")
            {
              addRow(EqCoefMat,A);
              addRow(EqConstMat,D);
              NoOfEqs++;
            }
          else if(type=="is_not_equal")
            {
              addRow(DisEqCoefMat,A);
              addRow(DisEqConstMat,D);
              NoOfDisEqs++;
            }

          else {cout<<"\nUnknown root name "<<type<<" inside function getEquationsAndDisequations. Exiting...\n"; exit(1);}

        }
      else
        {
          if(IsNullMatrix(B) && type=="is_not_equal") //we have 0<>0. false
            return false;
        }


    }

  return true;

}


/* Functions for quantifying out multiple variables and returning the result */


// Given a set of LMCs and a variable to be eliminated, this function first converts the 
// LMCs into matrix representation, and then calls the matrix based QEliminator, and 
// returns the result as a DAGNode*
DAGNode* callMatrixQEliminator(set<DAGNode*> &Constraints, string VariableToElim, map<string, int> &WidthTable)
{
  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();
  names.insert(VariableToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    getDAGNames(*it, names);


  attachSetToList(names, VarNames);

  NoVars=VarNames.size();


  // Get the equations and disequations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;

  if(compare_apqelim_omegatest) // we need to compare between AP_QElim and Omegatest times, but continue with the dag computed by AP_QElim. This branch calls only AP_QElim
    {
      if(strcmp(qEliminator_type,"-lma")==0)
	{
	  time_t start, end, duration;

	  struct timeval start_ms, finish_ms;
	  unsigned long long int duration_ms;

	  time(&start);

	  gettimeofday (&start_ms, NULL);

	  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
          
	  gettimeofday (&finish_ms, NULL);

	  time(&end);
	  
	  duration = end - start;

	  Total_AP_QElim_Time = Total_AP_QElim_Time + duration;


	  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;
	  	  
	  Total_AP_QElim_Time_In_ms =  Total_AP_QElim_Time_In_ms + duration_ms;
	  
	}
      else
	{
	  cout<<"\nUnknown qEliminator_type "<<qEliminator_type<<" inside function callMatrixQEliminator\n";
	  exit(1);
	}
    }// if(compare_apqelim_omegatest)

  else if(compare_apqelim_blasting)
   {
      if(strcmp(qEliminator_type,"-lma")==0)
	{

	  time_t start, end, duration;

	  struct timeval start_ms, finish_ms;
	  unsigned long long int duration_ms;

	  time(&start);

	  gettimeofday (&start_ms, NULL);

	  resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
          gettimeofday (&finish_ms, NULL);

	  time(&end);
	  
	  duration = end - start;

	  Total_AP_QElim_Time = Total_AP_QElim_Time + duration;


	  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

	  //cout<<"\nAP_QElim duration = "<<duration_ms<<endl;
	  	  
	  Total_AP_QElim_Time_In_ms =  Total_AP_QElim_Time_In_ms + duration_ms;


          if(!bitblasting_with_scopereduction) // Bit-blasting to be done at the same level as AP_QElim_Easy
	  {

	  	time(&start);

	  	gettimeofday (&start_ms, NULL);

	  	DAGNode *tempDAG = qEliminatorReturningDAGOnlyForOddCoefficientsFollowedByBitBlasting(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);

	  	gettimeofday (&finish_ms, NULL);

	  	time(&end);
	  
	  	duration = end - start;

	  	Total_Blasting_Time = Total_Blasting_Time + duration;

	  	duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
	  	duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

	  	//cout<<"\nBit-blasting duration = "<<duration_ms<<endl;
	  	  
	  	Total_Blasting_Time_In_ms = Total_Blasting_Time_In_ms  + duration_ms;
	  }

	}
      else
	{
	  cout<<"\nUnknown qEliminator_type "<<qEliminator_type<<" inside function callMatrixQEliminator\n";
	  exit(1);
	}
	
    }

  else if(strcmp(qEliminator_type,"-lma")==0)
    {
    resultDAG = qEliminatorReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);
    }
  else
    {
      cout<<"\nUnknown qEliminator_type "<<qEliminator_type<<" inside function callMatrixQEliminator in Eliminator.cpp\n";
      exit(1);
    }

  return resultDAG;
}


// Given a set of LMCs in "Constraints" and a variable to be eliminated, generate two sets of constraints - 
// A set of constraints free of the variable to be eliminated and another set of constraints which contain
// the variable to be eliminated
void getFreeAndBoundConstraints(set<DAGNode*> &Constraints, string Variable, set<DAGNode*> &Free_Constraints, set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable)
{
  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
      set<string> Names;

      DAGNode* root = (*it);

      getDAGNames(root, Names);

      if(Names.find(Variable)!=Names.end()) // Variable occures in the constraint. Hence it is bound
	{
	  Bound_Constraints.insert(root);
	}
      else
	{
	 Free_Constraints.insert(root);
	} 
    }
}

// Given a set of LMCs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode*
DAGNode* APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> &WidthTable)
{

    if(VarsToElim.size()>0)
    {
      update_Multivar_APQElim_Calls();
    }
  
  //cout<<"\nAPResolveModifiedWithMultipleVariablesUsingLMA called\n";

  set<DAGNode*> Constraints;
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  DAGNode* QElim_Bound_Constraint;

  //showSet(VarsToElim,"VarsToElim");

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  //showAPSet("Constraints", Constraints);



  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;

      //cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints);

      QElim_Bound_Constraints.clear();
      QElim_Bound_Constraint = NULL; // stands for true

      if(Bound_Constraints.size()>0)
	{
	  QElim_Bound_Constraint = callMatrixQEliminator(Bound_Constraints, VariableToElim, WidthTable);
	}

      if(QElim_Bound_Constraint != NULL) // stands for true
	{
	  //cout<<"\nQElim_Bound_Constraint = "<<getDAGStr(QElim_Bound_Constraint)<<"\n";
	  splitConjunctionIntoSet(QElim_Bound_Constraint, QElim_Bound_Constraints);
	}

      //showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      //showAPSet("Constraints", Constraints);
    }

  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
  
    if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    resultDAG = quickSimplify(resultDAG);

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    return resultDAG;
}




/* Functions for Experiment-III : Remember that experiment-III is to do quantifier elimination on conjunction of APs. */

// Split a cojunction of LMCs in "Constraint" into a set of LMEs E and a set of LMDs D
void splitConjunctionIntoSetsWithSupportForNot(DAGNode* Constraint, set<DAGNode*> &E, set<DAGNode*> &D)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "=") // terminal reached
    {
      E.insert(Constraint);
    }

  else if(MyName == "is_not_equal") // terminal reached
    {
      D.insert(Constraint);
    }


  else if(MyName == "not") // terminal (not (= lhs rhs)) reached
    {
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
      DAGNode *child = *iout;
      if(child->Get_node_name()!= "=") 
	{
	  cout<<"\nErraneous not node in function splitConjunctionIntoSetWithSupportForNot in Eliminator.cpp\n";
	  exit(1);
	}
      iout = child->Get_outlist_start();
      DAGNode *lhs = *iout;
      iout++;
      DAGNode *rhs = *iout;
      
      DAGNode *diseqn = createDAG("is_not_equal", lhs, rhs, LOGICAL_SYMBOL, 1);
      
      D.insert(diseqn);
    }


  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoSet in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoSetsWithSupportForNot(*iout, E, D);
      
      iout++;

      splitConjunctionIntoSetsWithSupportForNot(*iout, E, D);
    }
  else
    {
     cout<<"\nunkwnon node encountered in function splitConjunctionIntoSetsWithSupportForNot in Eliminator.cpp\n";
     exit(1); 
    }
}


// Function to simplify the dag node "node" based on some simple syntactic rules
DAGNode*  quickSimplify(DAGNode *node)
{
    
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  DAGNode *ret = quickSimplifyInternal(node, PtrDPTable);
  return ret;
}
  
  

// Function called by quickSimplify to simplify the dag node "node" based on some simple syntactic rules  
// Returns "node" with the syntactic simplification rules applied
DAGNode*  quickSimplifyInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable)
{
  DAGNode* ret;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 

  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }


  unsigned int outlist_size = node->Get_outlist_size();
  
  if(outlist_size == 0) { // Terminals
	  ret = node;
  }

  else
  {
    string dag_str = node->Get_node_name();
    if(!(dag_str == "bvule" || dag_str == "bvuge" || dag_str == "bvult" || dag_str == "bvugt" || dag_str == "and" || dag_str == "or" || dag_str=="if_then_else" || dag_str == "not"|| dag_str == "bvadd"|| dag_str == "bvsub"|| dag_str == "bvmul"|| dag_str == "=" || dag_str == "is_not_equal"))
      {
	//cout<<dag_str<<"node encountered. The nodes down cannot be simplified\n";
	ret = node;
      } 
    else
      {
	vector<DAGNode*> SimplifiedChildren;
	list<DAGNode*>::iterator iout = node->Get_outlist_start();
    

	for(unsigned int i=0;i<outlist_size; i++,iout++)
	  {
	    SimplifiedChildren.push_back(quickSimplifyInternal(*iout, DPTable));
	  }

   
	if(dag_str == "and" && outlist_size==2){// binary and
	  if((SimplifiedChildren[0])->Get_node_name()=="true"|| (SimplifiedChildren[1])->Get_node_name()=="false")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="true" || (SimplifiedChildren[0])->Get_node_name()=="false")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("and", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1) ;
	}
	else if(dag_str == "or" && outlist_size==2){// binary or
	  if((SimplifiedChildren[0])->Get_node_name()=="false"|| (SimplifiedChildren[1])->Get_node_name()=="true")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="false" || (SimplifiedChildren[0])->Get_node_name()=="true")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("or", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);
	}
	else if(dag_str == "and" && outlist_size>2){// and with more than two children
	  
	    ret = simplifyAndWithMultipleChildren(SimplifiedChildren);
	}
	else if(dag_str == "or" && outlist_size>2){// or with more than two children
	  
	    ret = simplifyOrWithMultipleChildren(SimplifiedChildren);
	}
	else if(dag_str == "not"){
	  if((SimplifiedChildren[0])->Get_node_name()=="false")
	    ret = createLogicalConstant("true");
	  else if((SimplifiedChildren[0])->Get_node_name()=="true")
	    ret = createLogicalConstant("false");
	  else
	    ret = createDAG("not", SimplifiedChildren[0], LOGICAL_SYMBOL, 1);
	}

	else if(dag_str == "if_then_else"){
	  if((SimplifiedChildren[0])->Get_node_name()=="false")
	    ret = SimplifiedChildren[2];
	  else if((SimplifiedChildren[0])->Get_node_name()=="true")
	    ret = SimplifiedChildren[1];
	  else if(SimplifiedChildren[1] == SimplifiedChildren[2])
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="false" && (SimplifiedChildren[2])->Get_node_name()=="true")
	    ret = createDAG("not", SimplifiedChildren[0], LOGICAL_SYMBOL, 1);
	  else if((SimplifiedChildren[1])->Get_node_name()=="true" && (SimplifiedChildren[2])->Get_node_name()=="false")
	    ret = SimplifiedChildren[0];
	  else 
	    ret = createDAG("if_then_else", SimplifiedChildren[0], SimplifiedChildren[1], SimplifiedChildren[2], LOGICAL_SYMBOL, 1);
	}


	else if(dag_str == "bvadd"){
	  if((SimplifiedChildren[0])->Get_node_name()=="0")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("bvadd", SimplifiedChildren[0], SimplifiedChildren[1], BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	}
	else if(dag_str == "bvsub"){
	  if((SimplifiedChildren[1])->Get_node_name()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("bvsub", SimplifiedChildren[0], SimplifiedChildren[1], BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	}
	else if(dag_str == "bvmul"){
	  if((SimplifiedChildren[0])->Get_node_name()=="1" || (SimplifiedChildren[1])->Get_node_name()=="0")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="1"|| (SimplifiedChildren[0])->Get_node_name()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("bvmul", SimplifiedChildren[0], SimplifiedChildren[1], BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	}
	else if(dag_str == "="){
	  if(SimplifiedChildren[0] == SimplifiedChildren[1])
	    ret = createLogicalConstant("true");
	  else if((SimplifiedChildren[0]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[0]))) && (SimplifiedChildren[1]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[1])))) // both operands are constants
	    ret = createLogicalConstant("false");
	  else
	    ret = createDAG("=", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);	 
     
	}
	else if(dag_str == "is_not_equal"){
	  if(SimplifiedChildren[0] == SimplifiedChildren[1])
	    ret = createLogicalConstant("false");
	  else if((SimplifiedChildren[0]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[0]))) && (SimplifiedChildren[1]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[1])))) // both operands are constants
	    ret = createLogicalConstant("true");
	  else
	    ret = createDAG("is_not_equal", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);;	 
     
	}
        else if(dag_str == "bvule" || dag_str == "bvuge" || dag_str == "bvult" || dag_str == "bvugt")
        {
           ret = createDAG(dag_str, SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1); 
        }
	else
	  {
	    cout<<dag_str<<"node encountered\n";
	    exit(1);
	  }
      }

  }
  result = DPTable->hashtable_insert(key, ret);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function  quickSimplifyInternal\n";
      exit(1);
    }
  return ret;
}

// Return true if "node" is a constant; false otherwise  
bool isConstant(DAGNode *node)
{
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyDataType = MyType.data_type;

  if(MyDataType == BITVECTOR_CONSTANT || MyDataType == LOGICAL_CONSTANT || MyDataType == INTEGER_CONSTANT || MyDataType == REAL_CONSTANT) return true;
  return false;
}



// Given a previously linearized dag and a linearization table based on which linearization was done,
// return a copy of the dag with the linear nodes replaced by the original nodes 
DAGNode* getOriginalClone(DAGNode* node, map<DAGNode*, DAGNode*> &LinearizationTable)
{
  HashTable <string, DAGNode*> MapForNonLinearization;
  HashTable <string, DAGNode*> *PtrMapForNonLinearization = &MapForNonLinearization;

  DAGNode* result = getOriginalClone_Internal(node, LinearizationTable, PtrMapForNonLinearization);
  return result;
}

// Function called by getOriginalClone
DAGNode* getOriginalClone_Internal(DAGNode* node, map<DAGNode*, DAGNode*> &LinearizationTable, HashTable <string, DAGNode*> *PtrMapForNonLinearization)
    {
      char temp_char[10];
      sprintf(temp_char , "%x", node);
      string key = temp_char;

      // key created

      HTStatusValue<DAGNode*> ResultFromHT = PtrMapForNonLinearization->hashtable_search(key);

      if(ResultFromHT.success())
        {
          return ResultFromHT.GetValue();
        }
    
      DAGNode* clone;
                   
        if(node->Get_outlist_size()!=0) //operator   
        {
            list<DAGNode*>::iterator iout = node->Get_outlist_start();
            unsigned int outlist_size = node->Get_outlist_size();

	    vector<DAGNode*> children;
            for(int i=0;i<outlist_size;i++)
		{
		DAGNode* child = getOriginalClone_Internal(*iout, LinearizationTable, PtrMapForNonLinearization);
                children.push_back(child);
		iout++;
		}
            clone = dm.createNode(node->Get_node_name(),children, node->getValue());
        }
        else if(isConstant(node))
        {
         clone = dm.createNode(node->Get_node_name(), node->getValue());
        }
        else
	{
        map<DAGNode*, DAGNode*>::iterator ltit = findInLinearizationTable(LinearizationTable, node);
        
        if(ltit != LinearizationTable.end()) // Occures in LT
	    {
              //cout<<"\nMatch\n";
	      clone = ltit->first; 	     
	    }	  
	  else
	    {	      
	     clone = dm.createNode(node->Get_node_name(), node->getValue()); 
	    }
        } //if-...-else ends here
      
  ResultFromHT = PtrMapForNonLinearization->hashtable_insert(key, clone);
  if(!ResultFromHT.success())
        {
        cout<<"\nHash Insert Failure Inside Function getOriginalClone_Internal in Eliminator.cpp\n";
        exit(1);
        }   
      
    return clone;
    } 

// Search the LinearizationTable for a particular node and return the iterator
map<DAGNode*, DAGNode*>::iterator findInLinearizationTable(map<DAGNode*, DAGNode*> &LinearizationTable, DAGNode* clone)
{
  map<DAGNode*, DAGNode*>::iterator ltit = LinearizationTable.begin();

  for(; ltit != LinearizationTable.end(); ltit++)
    {
      //string existing_id_delta = (ltit->second)->Get_DAGnode_data_as_id_delta();
      //if(existing_id_delta == id_delta)
      if(ltit->second==clone)
	{
	  return ltit;
	}
    }
  return ltit;
}


// Function to write declarations in SMTLIB format for the set of variables
// in the set "Vars"
void writeBVDeclarationsInSMTLibFormatForSelectedVars(FILE *smt_file, map<string, int> WidthTable, set<string> &Vars)
{
  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {
	  if(Vars.find(it->first)!=Vars.end())
	    {
	      fprintf(smt_file,":extrafuns ((%s",(it->first).c_str());
	      fprintf(smt_file," BitVec[%d]))\n",it->second);
	    }
	}
 
}

// Print the given conjunction "root" in SMTLIB format in file "outfile_name" where
// "outfile_name" = "infile_name" without extension + "_simplified.smt"
void printConjunctInSMTLibFormat(DAGNode *root, map<string, int> &WidthTable, string infile_name, string &outfile_name)
{
  
  int index = infile_name.find(".");
  if(index != string::npos) 
		{
 		outfile_name = infile_name.substr(0, index);
                outfile_name += "_simplified.smt";
		}		
  else
		{
		cout<<"\nInvalid file name in printConjunctInSMTLibFormat in Eliminator.cpp\n";
		exit(1);
		}
 
  FILE *smt_file = fopen(outfile_name.c_str(), "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<outfile_name<<"\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  set<string> Vars;
  getDAGNames(root, Vars);

  writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, WidthTable, Vars);

  string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);
}


// Here "node" is a selection node. This function returns
// the width of selection node which is,
// given, sel(x, 1, 3), return 3
int getWidthOfSelectionNode(DAGNode *node)
{
  list<DAGNode*>::iterator iout = node->Get_outlist_start();
  iout++;
  // getting low
  DAGNode *low_dag = *iout;
  iout++;
  DAGNode *high_dag = *iout;
  string low_str = low_dag->Get_node_name();
  string high_str = high_dag->Get_node_name();
  
  int low = atoi(low_str.c_str());
  int high = atoi(high_str.c_str());

  return high-low+1;
}

// return true if "node" is a bit-vector constant; false otherwise
bool isBVConstant(DAGNode *node)
{
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyDataType = MyType.data_type;

  if(MyDataType == BITVECTOR_CONSTANT) return true;
  return false;
}


// ImpureVars are variables occuring in non-linear operations
// This function traverse the dag "node" and gets the impure variables
// in the set ImpureVars
void getImpureVariables(DAGNode *node, set<string> &ImpureVars)
{
    string MyName = node->Get_node_name();

    if(MyName == "concat" || MyName == "select" || MyName == "bvor" || MyName == "bvand" || MyName == "bvnot" || MyName == "bvxor" || nonLinearMult(node)) // non-linear node encountered
      {
	getDAGNames(node, ImpureVars);
      }
    else
      {
	list<DAGNode*> :: iterator iout = node->Get_outlist_start();
	for(unsigned int i=0; i<node->Get_outlist_size(); i++, iout++)
	  {
	    getImpureVariables(*iout, ImpureVars);
	  }
      }
}


// Function to update the set VarSetToElim. This function removes the impure
// variables i.e. variables which cannot be eliminated by the CAV'11 paper based
// work from the set VarSetToElim
void updateVarSetToElim(set<string> &VarSetToElim, DAGNode *node)
{
  set<string> ImpureVars;
  getImpureVariables(node, ImpureVars);
  
  set<string> ImpureVarsToQElim;
  set<string> PureVarsToQElim;
  
  set_difference(VarSetToElim.begin(), VarSetToElim.end(), ImpureVars.begin(), ImpureVars.end(),inserter(PureVarsToQElim, PureVarsToQElim.begin()));
  set_intersection(VarSetToElim.begin(), VarSetToElim.end(), ImpureVars.begin(), ImpureVars.end(),inserter(ImpureVarsToQElim, ImpureVarsToQElim.begin()));

  if(!ImpureVarsToQElim.empty())
    {
      cout<<"\nWARNING!!Following variables cannot be eliminated as they occure in non-linear operations\n";
      showSet(ImpureVarsToQElim, "ImpureVarsToQElim");
      VarSetToElim.clear();
      set_union(VarSetToElim.begin(), VarSetToElim.end(), PureVarsToQElim.begin(), PureVarsToQElim.end(),inserter(VarSetToElim, VarSetToElim.begin()));
      showSet(VarSetToElim, "VarSetToElim");
    }
}
  
// Print the given string "result" in SMTLIB format in file "outfile_name" where
// "outfile_name" = "infile_name" without extension + "_simplified.smt"
void printTrivialConjunctInSMTLibFormat(string result, string infile_name, string &outfile_name)
{
  
  int index = infile_name.find(".");
  if(index != string::npos) 
		{
 		outfile_name = infile_name.substr(0, index);
                outfile_name += "_simplified.smt";
		}		
  else
		{
		cout<<"\nInvalid file name in printConjunctInSMTLibFormat in Eliminator.cpp\n";
		exit(1);
		}
 
  FILE *smt_file = fopen(outfile_name.c_str(), "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<outfile_name<<"\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  fprintf(smt_file, "\n:formula (%s)\n", result.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);
}

 
// Function to decide sat of conjunction of LMEs and LMDs by QE. 
// Calls getEquationsAndDisequations for dag ---> matrix conversion and
// qEliminatorForSolving for sat. checking from conjunction of LMEs and LMDs represented as matrices
// Returns 'u' if unsat, 's' if sat, 'x' if unknown  
char satCheckByExQuantifierElimination(DAGNode *root, set<string> &VarsToElim, map<string, int> &WidthTable)
{ 
 set<DAGNode*> E;
 set<DAGNode*> D;
 splitConjunctionIntoSetsWithSupportForNot(root, E, D); // splitting the root which is a conjunction into a set of APs

 //clock_t exsolver_start, exsolver_end;
 //exsolver_start = clock();

 time_t exsolver_start, exsolver_end;
 time(&exsolver_start);

 
  char ret_value;

  // get the matrices
  
  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  attachSetToList(VarsToElim, VarNames);

  NoVarsToElim=VarNames.size();

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);


  attachSetToList(names, VarNames);

  NoVars=VarNames.size();

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));

  // Get the equations and disequations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  if(!result) // false 
    {
      ret_value = 'u';
    }
  else
   {

   ULINT Mod=findPower(NoOfBits);

   //exsolver_start = clock();
   time(&exsolver_start);

   cout<<"\nexsolver started\n";

   ret_value = qEliminatorForSolving(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);

   //exsolver_end = clock();
   //double diffticks = exsolver_end - exsolver_start;
   //cout<<"\nexsolver_duration = "<<diffticks<<" ticks \n";
   //cout<<"\nexsolver_duration = "<<(diffticks*10)/CLOCKS_PER_SEC<<" secs \n";

   time(&exsolver_end);
   cout<<"\nexsolver_duration = "<<exsolver_end-exsolver_start<<" secs \n";

   }

  //exsolver_end = clock();
  //double diffticks = exsolver_end - exsolver_start;
  //cout<<"\nexsolver_duration = "<<diffticks<<" ticks \n";
  //cout<<"\nexsolver_duration = "<<(diffticks*10)/CLOCKS_PER_SEC<<" secs \n";

 cout<<"\nret_value = "<<ret_value<<endl;

 return ret_value;
  
}

// Print conjunction of LMCs in E, D, and u_AP in an SMTLIB file; name of the SMTLIB file
// is decided in the function printConjunctInSMTLibFormatWithCount
void printImplicationCheckInSMTFile(set<DAGNode*> &E, set<DAGNode*> &D, DAGNode *u_AP, int Show_ImpCheck_Calls, map<string, int> &WidthTable)
{

  set<DAGNode*> Constraints;
  Constraints.clear();
  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));
  Constraints.insert(u_AP);


  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }

  printConjunctInSMTLibFormatWithCount(resultDAG, WidthTable, Show_ImpCheck_Calls);
}

// Print the given conjunction "root" in SMTLIB format. The file name 
// is decided as "disjunct_count".smt and is put in a separate directory
void printConjunctInSMTLibFormatWithCount(DAGNode *root, map<string, int> &WidthTable, int disjunct_count)
{
  string impchecks_directory="IMPCHECKS";
  char count[100];
  sprintf(count,"%d",disjunct_count);
  string file_name;
  file_name += impchecks_directory;
  file_name += "/";
  file_name +=count;
  file_name += ".smt";
  FILE *smt_file = fopen(file_name.c_str(), "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<file_name<<"\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  set<string> Vars;
  getDAGNames(root, Vars);

  writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, WidthTable, Vars);

  /*

  fprintf(smt_file,"\n:exists(");
  for(set<string>::iterator it=Vars.begin(); it!=Vars.end(); it++)
    {
      fprintf(smt_file," %s",(*it).c_str());
    }
  fprintf(smt_file,")\n");

  */

  string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);
}

// Print the given conjunction "root" in SMTLIB format with the variables to eliminate
// The file name is decided as "infilename_withno_ext"+"_square"+count+".smt"
void printInputToQEliminatorInSMTLibFormatWithCount(DAGNode *root, map<string, int> &WidthTable, set<string> &VarSetToElim, string infilename)
{
  static int file_count=1;
  int index = infilename.find_last_of(".");
  string infilename_withno_ext = infilename.substr(0, index);

  char count[100];
  sprintf(count,"%d",file_count);
  string file_name;
  file_name += infilename_withno_ext;
  file_name += "_square_";
  file_name += count;
  file_name += ".smt";

  cout<<"\nPrinting in "<<file_name<<endl;

  FILE *smt_file = fopen(file_name.c_str(), "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<file_name<<"\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  set<string> Vars;
  getDAGNames(root, Vars);

  writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, WidthTable, Vars);

 
  fprintf(smt_file,"\n:exists(");
  for(set<string>::iterator it=VarSetToElim.begin(); it!=VarSetToElim.end(); it++)
    {
      fprintf(smt_file," %s",(*it).c_str());
    }
  fprintf(smt_file,")\n");

  
  string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);
  file_count++;

  cout<<"\nPrinted in "<<file_name<<endl;
  }


// return a single variable from the support of "node";
// rerturn "" if the support is empty
string getSingleVariable(DAGNode *node)
{
    string dag_str = node->Get_node_name();

    if(dag_str == "exists")
    {
        list<DAGNode*>::iterator iout = node->Get_outlist_start();
	iout++;
        return getSingleVariable(*iout);
    }
    else
    {
          Value* MyValue = (Value*)(node->getValue());
  	  Type MyType = MyValue->get_node_type();
  	  int MyWidth = MyType.width;
  	  int MyDataType = MyType.data_type;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)  //if the dag value is variable
        {
	  return dag_str;
        }

        list<DAGNode*>::iterator iout = node->Get_outlist_start();

        for(unsigned int i=0; i<node->Get_outlist_size() ; i++, iout++)
        {
            string dag_str_from_child = getSingleVariable(*iout);
	    if(dag_str_from_child != "")
	      {
		return dag_str_from_child;
	      }
        }
	
	return "";
    }

}



// checks if E/\D/\Negated_u_AP is unsat/sat by simulating the dag
// returns
//   's' if sat
//   'u' if unsat
//   'x' if it cannot find
char implicationCheckBySimulation(set<DAGNode*> &E, set<DAGNode*> &D, DAGNode *Negated_u_AP, map<string, int> &WidthTable)
{

//     cout<<"\nimplicationCheckBySimulation called with\n";
//     showAPSet("E", E);
//     showAPSet("D", D);
//     cout<<"\nNegated_u_AP = "<<getDAGStr(Negated_u_AP)<<endl;

  time_t solver_start, solver_end, solver_duration;
  time(&solver_start);

  if(E.empty() && D.empty()) // empty context
    {
      cout<<"\nWarning!!Context empty in function implicationCheckBySimulation in Eliminator.cpp. Empty context is interpreted as true context. Hence, result depends on consequent\n"; 
     }


  bool freezing_variables=true;
  bool freezing_strategy_for_simple_lmdes=true;

  char ret_value='x';

  // find width
  string variable="";
  variable=getSingleVariable(Negated_u_AP);
  if(variable=="")
    {
      cout<<"\nLME/LMD "<<getDAGStr(Negated_u_AP)<<" encountered with no variable names in function implicationCheckBySimulation in Eliminator.cpp\n";
      exit(1);
    }

  int width = WidthTable[variable];

  ULINT Range = findPower(width);

  // get the Variables

  set<string> Variables;

  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    {
      getDAGNames(*it, Variables);
    }

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    {
      getDAGNames(*it, Variables);
    }

  getDAGNames(Negated_u_AP, Variables);

  int variable_count = Variables.size();

  // Create the Frozen_Map : map which can be frozen across all simulation runs

  map<string, ULINT> Frozen_Map;

  Frozen_Map.clear();

  if(freezing_variables)
    {

      for(set<DAGNode*>::iterator it=E.begin(); it!=E.end() && Frozen_Map.size()!= variable_count; it++)
	{
	  DAGNode *lme = *it;
	  list<DAGNode*>::iterator iout;
	  iout = lme->Get_outlist_start();
	  DAGNode *term = *iout;
	  iout++;
	  DAGNode *constant = *iout;
      
	  if(term->Get_node_name()=="bvmul")
	    {
	      iout = term->Get_outlist_start();
	      DAGNode *coef = *iout;
	      iout++;
	      DAGNode *var = *iout;
	  
	      if(coef->Get_node_name()=="1") // LME of the form 1*x_0 = constant
		{
		  string frozen_var = var->Get_node_name();
		  string frozen_val_str = constant->Get_node_name();
		  ULINT frozen_val_dec = strtoull(frozen_val_str.c_str(), NULL, 0);
		  Frozen_Map.insert(make_pair(frozen_var, frozen_val_dec));
		}
	    }
	}// for each equation loop over
	      
      for(set<DAGNode*>::iterator it=D.begin(); it!=D.end() && Frozen_Map.size()!= variable_count; it++) //  && Frozen_Map.size()!= variable_count checks if all variables by frozen already
	{
	  DAGNode *lmd = *it;
	  list<DAGNode*>::iterator iout;
	  iout = lmd->Get_outlist_start();
	  DAGNode *term = *iout;
	  iout++;
	  DAGNode *constant = *iout;
      
	  if(term->Get_node_name()=="bvmul")
	    {
	      iout = term->Get_outlist_start();
	      DAGNode *coef = *iout;
	      iout++;
	      DAGNode *var = *iout;
	  
	      if(coef->Get_node_name()=="1") // LMD of the form 1*x_0 <> constant
		{
		  string frozen_var = var->Get_node_name();
		  string frozen_val_str = constant->Get_node_name();
		  ULINT frozen_val_dec = strtoull(frozen_val_str.c_str(), NULL, 0);
		  frozen_val_dec = add(frozen_val_dec, 1, Range);
		  Frozen_Map.insert(make_pair(frozen_var, frozen_val_dec)); 
		}
	    }
	}// for each disequation loop over
    
      // for Negated_u_AP 

      if(Frozen_Map.size()!= variable_count) //  Frozen_Map.size()!= variable_count checks if all variables by frozen already
	{
	  list<DAGNode*>::iterator iout;
	  iout = Negated_u_AP->Get_outlist_start();
	  DAGNode *term = *iout;
	  iout++;
	  DAGNode *constant = *iout;
      
	  if(term->Get_node_name()=="bvmul")
	    {
	      iout = term->Get_outlist_start();
	      DAGNode *coef = *iout;
	      iout++;
	      DAGNode *var = *iout;
	  
	      if(coef->Get_node_name()=="1") // LMDE of the form 1*x_0 op constant
		{
		  string frozen_var = var->Get_node_name();
		  string frozen_val_str = constant->Get_node_name();
		  ULINT frozen_val_dec = strtoull(frozen_val_str.c_str(), NULL, 0);

		  if(Negated_u_AP->Get_node_name()=="is_not_equal") // op is <>
		    {
		      frozen_val_dec = add(frozen_val_dec, 1, Range);
		    }
		  Frozen_Map.insert(make_pair(frozen_var, frozen_val_dec)); 
		}
	    }
	}
  
      // Frozen_Map is ready

      //showVariable_Value_Map("Frozen_Map", Frozen_Map);

    }// if(freezing_variables)


  if(freezing_strategy_for_simple_lmdes) // Further freeze the Frozen_Map 
    {

      for(set<DAGNode*>::iterator it=E.begin(); it!=E.end() && Frozen_Map.size()!= variable_count; it++)
	{
	  DAGNode *lme = *it;
	  list<DAGNode*>::iterator iout;
	  iout = lme->Get_outlist_start();
	  iout++;
	  DAGNode *constant = *iout;

	  set<string> variables_in_lme;
	  getDAGNames(lme, variables_in_lme);

	  if(variables_in_lme.size()!=2) 
	    {
	      continue;
	    }
	  set<string>::iterator variables_in_lme_it = variables_in_lme.begin();
	  string first_variable = *variables_in_lme_it;
	  variables_in_lme_it++;
	  string second_variable = *variables_in_lme_it;

	  map<string, ULINT>::iterator  Frozen_Map_it; 
	  Frozen_Map_it = Frozen_Map.find(first_variable);
	  if(Frozen_Map_it != Frozen_Map.end()) // yes; first variable is frozen. use it to decide the second variable
	    {
	      ULINT first_variable_value = Frozen_Map_it->second;
	      string frozen_constant_str = constant->Get_node_name();
	      ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2; // 0 or 1
	      ULINT second_variable_value;

	      if(frozen_constant_dec==0)
		{
		  second_variable_value = first_variable_value;
		}
	      else//frozen_constant_dec==1
		{
		  second_variable_value  = add(first_variable_value%2, 1, 2);
		}
	      
	      Frozen_Map.insert(make_pair(second_variable, second_variable_value));
	    }
	  else // first variable is not frozen. try the second variable
	    {
	      map<string, ULINT>::iterator  Frozen_Map_it; 
	      Frozen_Map_it = Frozen_Map.find(second_variable);
	      if(Frozen_Map_it != Frozen_Map.end()) // yes; second variable is frozen. use it to decide the first variable
		{
		  ULINT second_variable_value = Frozen_Map_it->second;
		  string frozen_constant_str = constant->Get_node_name();
		  ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2;
		  ULINT first_variable_value;

		  if(frozen_constant_dec==0)
		    {
		      first_variable_value = second_variable_value;
		    }
		  else//frozen_constant_dec==1
		    {
		      first_variable_value  = add(second_variable_value%2, 1, 2);
		    }
	      	  
		  Frozen_Map.insert(make_pair(first_variable, first_variable_value));
		}
	      else // both variables are not frozen. set first variable to say 0 and decide second variable based on this
		{
		  ULINT first_variable_value = 0;
		  string frozen_constant_str = constant->Get_node_name();
		  ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2;
		  ULINT second_variable_value;

		  if(frozen_constant_dec==0)
		    {
		      second_variable_value = first_variable_value;
		    }
		  else//frozen_constant_dec==1
		    {
		      second_variable_value  = add(first_variable_value%2, 1, 2);
		    }
		  
		  
		  Frozen_Map.insert(make_pair(first_variable, first_variable_value)); 
		  Frozen_Map.insert(make_pair(second_variable, second_variable_value)); 
		}
	    }// else of if(Frozen_Map_it != Frozen_Map.end())
	}// for each equation in E ends here

      for(set<DAGNode*>::iterator it=D.begin(); it!=D.end() && Frozen_Map.size()!= variable_count; it++)
	{
	  DAGNode *lmd = *it;
	  list<DAGNode*>::iterator iout;
	  iout = lmd->Get_outlist_start();
	  iout++;
	  DAGNode *constant = *iout;

	  set<string> variables_in_lmd;
	  getDAGNames(lmd, variables_in_lmd);

	  if(variables_in_lmd.size()!=2) 
	    {
	      continue;
	    }
	  set<string>::iterator variables_in_lmd_it = variables_in_lmd.begin();
	  string first_variable = *variables_in_lmd_it;
	  variables_in_lmd_it++;
	  string second_variable = *variables_in_lmd_it;

	  map<string, ULINT>::iterator  Frozen_Map_it; 
	  Frozen_Map_it = Frozen_Map.find(first_variable);
	  if(Frozen_Map_it != Frozen_Map.end()) // yes; first variable is frozen. use it to decide the second variable
	    {
	      ULINT first_variable_value = Frozen_Map_it->second;
	      string frozen_constant_str = constant->Get_node_name();
	      ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2; // 0 or 1
	      ULINT second_variable_value;

	      if(frozen_constant_dec==0)
		{
		  second_variable_value  = add(first_variable_value%2, 1, 2);
		}
	      else//frozen_constant_dec==1
		{
		  second_variable_value = first_variable_value;
		}
	      
	      Frozen_Map.insert(make_pair(second_variable, second_variable_value));

	    }
	  else // first variable is not frozen. try the second variable
	    {
	      map<string, ULINT>::iterator  Frozen_Map_it; 
	      Frozen_Map_it = Frozen_Map.find(second_variable);
	      if(Frozen_Map_it != Frozen_Map.end()) // yes; second variable is frozen. use it to decide the first variable
		{
		  ULINT second_variable_value = Frozen_Map_it->second;
		  string frozen_constant_str = constant->Get_node_name();
		  ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2;
		  ULINT first_variable_value;

		  if(frozen_constant_dec==0)
		    {
		      first_variable_value  = add(second_variable_value%2, 1, 2);
		      
		    }
		  else//frozen_constant_dec==1
		    {
		      first_variable_value = second_variable_value;
		    }
	      	  
		  Frozen_Map.insert(make_pair(first_variable, first_variable_value));

		}
	      else // both variables are not frozen. set first variable to say 0 and decide second variable based on this
		{
		  ULINT first_variable_value = 0;
		  string frozen_constant_str = constant->Get_node_name();
		  ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2;
		  ULINT second_variable_value;

		  if(frozen_constant_dec==0)
		    {
		      second_variable_value  = add(first_variable_value%2, 1, 2);
		      
		    }
		  else//frozen_constant_dec==1
		    {
		      second_variable_value = first_variable_value;
		    }
		  
		  
		  Frozen_Map.insert(make_pair(first_variable, first_variable_value)); 
		  Frozen_Map.insert(make_pair(second_variable, second_variable_value)); 

		}
	    }// else of if(Frozen_Map_it != Frozen_Map.end())
	}// for each disequation in D ends here


      // for Negated_u_AP 

      if(Frozen_Map.size()!= variable_count) //  Frozen_Map.size()!= variable_count checks if all variables by frozen already
	{
	  list<DAGNode*>::iterator iout;
	  iout = Negated_u_AP->Get_outlist_start();
	  iout++;
	  DAGNode *constant = *iout;

	  set<string> variables_in_lmde;
	  getDAGNames(Negated_u_AP, variables_in_lmde);

	  if(variables_in_lmde.size()==2) 
	    {
	      set<string>::iterator variables_in_lmde_it = variables_in_lmde.begin();
	      string first_variable = *variables_in_lmde_it;
	      variables_in_lmde_it++;
	      string second_variable = *variables_in_lmde_it;

	      map<string, ULINT>::iterator  Frozen_Map_it; 
	      Frozen_Map_it = Frozen_Map.find(first_variable);
	      if(Frozen_Map_it != Frozen_Map.end()) // yes; first variable is frozen. use it to decide the second variable
		{
		  ULINT first_variable_value = Frozen_Map_it->second;
		  string frozen_constant_str = constant->Get_node_name();
		  ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2; // 0 or 1
		  ULINT second_variable_value;

		  if(frozen_constant_dec==0)
		    {
		      if(Negated_u_AP->Get_node_name()=="=")
			{
			  second_variable_value = first_variable_value;
			}
		      else
			{
			  second_variable_value  = add(first_variable_value%2, 1, 2);
			}
		    }
		  else//frozen_constant_dec==1
		    {
		      if(Negated_u_AP->Get_node_name()=="=")
			{
			  second_variable_value  = add(first_variable_value%2, 1, 2);
			}
		      else
			{
			  second_variable_value = first_variable_value;
			}
		    }
	      
		  Frozen_Map.insert(make_pair(second_variable, second_variable_value));

		}
	      else // first variable is not frozen. try the second variable
		{
		  map<string, ULINT>::iterator  Frozen_Map_it; 
		  Frozen_Map_it = Frozen_Map.find(second_variable);
		  if(Frozen_Map_it != Frozen_Map.end()) // yes; second variable is frozen. use it to decide the first variable
		    {
		      ULINT second_variable_value = Frozen_Map_it->second;
		      string frozen_constant_str = constant->Get_node_name();
		      ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2;
		      ULINT first_variable_value;

		      if(frozen_constant_dec==0)
			{
			  if(Negated_u_AP->Get_node_name()=="=")
			    {
			      first_variable_value = second_variable_value;
			    }
			  else
			    {
			      first_variable_value  = add(second_variable_value%2, 1, 2);
			    }
		      
			}
		      else//frozen_constant_dec==1
			{
			  if(Negated_u_AP->Get_node_name()=="=")
			    {
			      first_variable_value  = add(second_variable_value%2, 1, 2);
			    }
			  else
			    {
			      first_variable_value = second_variable_value;
			    }
			}
	      	  
		      Frozen_Map.insert(make_pair(first_variable, first_variable_value));

		    }
		  else // both variables are not frozen. set first variable to say 0 and decide second variable based on this
		    {
		      ULINT first_variable_value = 0;
		      string frozen_constant_str = constant->Get_node_name();
		      ULINT frozen_constant_dec = (strtoull(frozen_constant_str.c_str(), NULL, 0))%2;
		      ULINT second_variable_value;

		      if(frozen_constant_dec==0)
			{
			  if(Negated_u_AP->Get_node_name()=="=")
			    {
			      second_variable_value = first_variable_value;
			    }
			  else
			    {
			      second_variable_value  = add(first_variable_value%2, 1, 2);
			    }
			}
		      else//frozen_constant_dec==1
			{
			  if(Negated_u_AP->Get_node_name()=="=")
			    { 
			      second_variable_value  = add(first_variable_value%2, 1, 2);
			    }
			  else
			    {
			      second_variable_value = first_variable_value;
			    }
			}
		  
		  
		      Frozen_Map.insert(make_pair(first_variable, first_variable_value)); 
		      Frozen_Map.insert(make_pair(second_variable, second_variable_value)); 
		    }
		}// else of if(Frozen_Map_it != Frozen_Map.end())

	    }// if(variables_in_lmde.size()==2) 
 
	}// if(Frozen_Map.size()!= variable_count)
  
      // Frozen_Map is ready

      //cout<<"\nAfter using freezing strategy\n";

      //showVariable_Value_Map("Frozen_Map", Frozen_Map);

    }// if(freezing_strategy_for_simple_lmdes)
  
  // create the Variable_Value_Map

  map<string, ULINT> Variable_Value_Map;

  int max_times = 2;// take care : max_times should be <= Range

  if(Frozen_Map.size()==Variables.size()) // all variables frozen
    {
      max_times = 1;
    }


  for(int iterations=0; iterations<max_times; iterations++)
    {
      Variable_Value_Map.clear();

      ULINT value = iterations; // simplest scheme to generate simulation vectors

      for(set<string>::iterator it =  Variables.begin(); it !=  Variables.end(); it++)
	{
	  string var = *it;

	  map<string, ULINT>::iterator  Frozen_Map_it = Frozen_Map.find(var); // check if the variable is frozen

	  if(Frozen_Map_it != Frozen_Map.end()) // yes; it is frozen. hence use the frozen value
	    {
	      
	      Variable_Value_Map.insert(make_pair(Frozen_Map_it->first, Frozen_Map_it->second));
	    }
	  else // no; it is not frozen. hence use the random value
	    {
	      Variable_Value_Map.insert(make_pair(var, value));
	    }
	}

      // Variable_Value_Map created

      //showVariable_Value_Map("Variable_Value_Map", Variable_Value_Map);

      bool result = conjunctionOfLMDEEvaluator(E, D, Negated_u_AP, width, Variable_Value_Map);

      if(result) // sat
	{
 	  //cout<<"\nresult = true\n";
 	  //cout<<"\nreturning sat\n"; 
	  ret_value = 's';
	  break;
	}
      //cout<<"\nresult = false\n"; 
    }
  
  //cout<<"\nreturning unknown\n"; 

  time(&solver_end);
  solver_duration = solver_end-solver_start;
  Simulator_Time_In_ImplicationCheck = Simulator_Time_In_ImplicationCheck + solver_duration;


  return ret_value;
}


// returns the value of E/\D/\Negated_u_AP if it is evaluated using the variable ---> value map "Variable_Value_Map"
bool conjunctionOfLMDEEvaluator(set<DAGNode*> &E, set<DAGNode*> &D, DAGNode *Negated_u_AP, int width, map<string, ULINT> &Variable_Value_Map)
{
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    {
      bool boolean_result;
      bvatom bv_result = lmdeEvaluator(*it, width, Variable_Value_Map, boolean_result);
      if(!boolean_result) // there exists one constraint which is  false. Return false
	{
// 	  cout<<endl<<getDAGStr(*it)<<" is false\n";
	  return false;
	}
    }

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    {
      bool boolean_result;
      bvatom bv_result = lmdeEvaluator(*it, width, Variable_Value_Map, boolean_result);
      if(!boolean_result) // there exists one constraint which is  false. Return false
	{
// 	  cout<<endl<<getDAGStr(*it)<<" is false\n";
	  return false;
	}
    }

  bool boolean_result;
  bvatom bv_result = lmdeEvaluator(Negated_u_AP, width, Variable_Value_Map, boolean_result);
  if(!boolean_result) // there exists one constraint which is  false. Return false
    {
//       cout<<endl<<getDAGStr(Negated_u_AP)<<" is false\n";
      return false;
    }

//   cout<<endl<<" conjunction is true\n";
  return true; // all constraints are true. Return true
}


// Returns the value of the LMDE rooted at "node" 
// if it is evaluated using the variable ---> value map "Variable_Value_Map".
// Note that value here is a decimal number
// Return value of type "bvatom" is the bit-vector result. The parameter "boolean_result" is set to 
// true/false if the return value is Boolean
bvatom lmdeEvaluator(DAGNode *node, int width, map<string, ULINT> &Variable_Value_Map, bool &boolean_result)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyDataType = MyType.data_type;


  bvatom bv_result;
  string dag_str;

  if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0) // variable
    {
      // Look at the map to get the value
      dag_str = name;
      map<string, ULINT>::iterator Variable_Value_Map_it = Variable_Value_Map.find(dag_str);
      if(Variable_Value_Map_it == Variable_Value_Map.end())
	{
	  cout<<"\nNo entry in Variable_Value_Map for variable "<<dag_str<<" in function lmdeEvaluator in Eliminator.cpp\n";
	  exit(1);
	}
      ULINT dec_value = Variable_Value_Map_it->second; // decimal value of the variable from map 
      string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
      bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()
	  

      if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	{
	  bv_result = bvm->BVExtend(result, width, 0);
	}
      else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	{
	  bv_result = bvm->select_with_int(result, width-1, 0);
	}
      else
	{
	  bv_result = result;
	}
    } // variable ends here

  else  // either operators or constants
    {
      if(name == "is_not_equal")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	  iout++;
	  bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

	  if (inter_bool_result) { // reverse the result as it is is_not_equal
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 0;
	    boolean_result = false;		  
	  }
	  else {
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 1;
	    boolean_result = true;		  

	  }
	} //name == "is_not_equal"

      else if(name == "=")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	  iout++;
	  bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

	  if (inter_bool_result) { // no need to reverse
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 1;
	    boolean_result = true;		  
	  }
	  else {
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 0;
	    boolean_result = false;		  

	  }
	} //name == "="

      else if(name == "bvmul")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  
	  if((*iout)->Get_node_name()=="1")// multiplication by 1
	    {
	      //cout<<"\nmultiplication by 1\n";
	      iout++;
	      bv_result = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained - no need to select. op2 is already of proper width
	    }
	  else
	    {

	      bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	      iout++;
	      bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	      bvatom result = bvm->arith_mult(op1, op2); // result obtained
	      bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
	    }
	}//name == "bvmul"

      else if(name == "bvadd")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  bvatom op1 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	  iout++;
	  bvatom op2 = lmdeEvaluator(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	  bvatom result = bvm->arith_add(op1, op2); // result obtained
	  bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
	}//name == "bvadd"


      else if(node->Get_outlist_size() == 0) // Constants

	{
	  dag_str = name;
	  ULINT dec_value = strtoull(dag_str.c_str(), NULL, 0); // decimal value of the constant obtained from the dag node
	  string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
	  bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

	  if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	    {
	      bv_result = bvm->BVExtend(result, width, 0);
	    }
	  else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	    {
	      bv_result = bvm->select_with_int(result, width-1, 0);
	    }
	  else
	    {
	      bv_result = result;
	    }
	}//constants
      else
	{
	  cout<<"\nUnanticipated constant "<<name<<" inside function lmdeEvaluator in Eliminator.cpp\n";
	  exit(1);
	}//unanticipated
		  
    }// either operators or constants ends here

  return bv_result;
		    
}// function ends

// show the Variable_Value_Map
void showVariable_Value_Map(string WhoamI, map<string, ULINT> &Variable_Value_Map)
{
  cout<<endl<<WhoamI<<endl<<"***********"<<endl;
  for(map<string, ULINT>::iterator it=Variable_Value_Map.begin();it!=Variable_Value_Map.end();it++)  
    {
      cout<<"\n"<<it->first<<"\t"<<it->second<<endl;
    }
}

// return the negation of the LMC
// eg: negateConstraint(x+y=z) is x+y<>z, and vice-versa
DAGNode* negateConstraint(DAGNode* C)
{
  string new_name;
  if( (C->Get_node_name() == "equivalent") || (C->Get_node_name() == "=") )
    new_name = "is_not_equal";
  else if(C->Get_node_name() == "is_not_equal")
    new_name = "=";

  list<DAGNode*>::iterator iout = C->Get_outlist_start();
  DAGNode *left = *iout;
  iout++;
  DAGNode *right=*iout;
  return createDAG(new_name, left, right, LOGICAL_SYMBOL, 1);
}



// FUNCTIONS REQUIRED IN DAGEliminator.cpp

// Returns result of evaluating the the LMC "lmde" using the variable--->value map Model
bool checkEntailment(DAGNode *lmde, map<string, int> &WidthTable, map<string, string> &Model)
{
  // find width
  string variable="";
  variable=getSingleVariable(lmde);
  
  
  if(variable=="")
    {
      cout<<"\nERROR!LMDE "<<getDAGStr(lmde)<<" encountered with no variable names in function checkEntailment in Eliminator.cpp\n";
      exit(1);
    }



  map<string, int>::iterator wit=WidthTable.find(variable);
  
  if(wit == WidthTable.end())
    {
      cout<<"\nVariable "<<variable<<" encountered with no entry in WidthTable in function checkEntailment in Eliminator.cpp\n";
      exit(1);
    }

  int width = wit->second;
  
  bool boolean_result;
  bvatom bv_result = lmdeEvaluatorWithBinaryModel(lmde, width, Model, boolean_result);
  return boolean_result;
}

// Same as the function lmdeEvaluator except that, 
// In the value ---> variable map "Variable_Value_Map", value is a binary string
bvatom lmdeEvaluatorWithBinaryModel(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyDataType = MyType.data_type;


  bvatom bv_result;
  string dag_str;

  if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0) // variable
    {
      // Look at the map to get the value
      dag_str = name;
      map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(dag_str);
      if(Variable_Value_Map_it == Variable_Value_Map.end())
	{
	  cout<<"\nNo entry in Variable_Value_Map for variable "<<dag_str<<" in function lmdeEvaluatorWithBinaryModel in Eliminator.cpp\n";
	  exit(1);
	}
 
      string bin_value = Variable_Value_Map_it->second; // binary value from map
      bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()
	  

      if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	{
	  bv_result = bvm->BVExtend(result, width, 0);
	}
      else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	{
	  bv_result = bvm->select_with_int(result, width-1, 0);
	}
      else
	{
	  bv_result = result;
	}
    } // variable ends here

  else  // either operators or constants
    {
      if(name == "is_not_equal")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  bvatom op1 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	  iout++;
	  bvatom op2 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

	  if (inter_bool_result) { // reverse the result as it is is_not_equal
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 0;
	    boolean_result = false;		  
	  }
	  else {
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 1;
	    boolean_result = true;		  

	  }
	} //name == "is_not_equal"

      else if(name == "=")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  bvatom op1 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	  iout++;
	  bvatom op2 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

	  if (inter_bool_result) { // no need to reverse
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 1;
	    boolean_result = true;		  
	  }
	  else {
	    bv_result = bvm->getZeroedBVatom(1);
	    bv_result.bv_array[0] = (bv_block_t) 0;
	    boolean_result = false;		  

	  }
	} //name == "="

      else if(name == "bvmul")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  
	  if((*iout)->Get_node_name()=="1")// multiplication by 1
	    {
	      //cout<<"\nmultiplication by 1\n";
	      iout++;
	      bv_result = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained - no need to select. op2 is already of proper width
	    }
	  else
	    {

	      bvatom op1 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	      iout++;
	      bvatom op2 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	      bvatom result = bvm->arith_mult(op1, op2); // result obtained
	      bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
	    }
	}//name == "bvmul"

      else if(name == "bvadd")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  bvatom op1 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
	  iout++;
	  bvatom op2 = lmdeEvaluatorWithBinaryModel(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

	  bvatom result = bvm->arith_add(op1, op2); // result obtained
	  bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
	}//name == "bvadd"


      else if(node->Get_outlist_size() == 0) // Constants

	{
	  dag_str = name;
	  ULINT dec_value = strtoull(dag_str.c_str(), NULL, 0); // decimal value of the constant obtained from the dag node
	  string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
	  bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

	  if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	    {
	      bv_result = bvm->BVExtend(result, width, 0);
	    }
	  else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	    {
	      bv_result = bvm->select_with_int(result, width-1, 0);
	    }
	  else
	    {
	      bv_result = result;
	    }
	}//constants
      else
	{
	  cout<<"\nUnanticipated constant "<<name<<" inside function lmdeEvaluatorWithBinaryModel in Eliminator.cpp\n";
	  exit(1);
	}//unanticipated
		  
    }// either operators or constants ends here

  return bv_result;
		    
}// function ends


// FUNCTIONS FOR DAGEliminator.cpp ends here

// FUNCTIONS TO PROVE EXACTNESS using CVC3

// returns true if eliminated => exists(X, original) i.e. eliminated \and (\forall X.(\neg original)) is unsat; false otherwise
bool checkForExactnessInOtherDirectionUsingCVC3(DAGNode* original, DAGNode* eliminated, map<string, int> WidthTable, set<string> &VarSetToElim)
{
  FILE *check_file = fopen("check.smt", "w");

  if(check_file == NULL) 
   {cout<<"\nCannot create check.smt\n"; exit(1);}

  fprintf(check_file,"(benchmark test.smt\n:logic QF_UFBV[32]\n");


  writeBVDeclarationsInSMTLibFormat(check_file, WidthTable);


  string original_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(original, WidthTable, VarSetToElim);
  string eliminated_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(eliminated, WidthTable);

  string exists_str = getExistsString(VarSetToElim,WidthTable);

  fprintf(check_file,"\n:formula (and  \n\n%s\n\n (forall %s (not %s)))", eliminated_str.c_str(), exists_str.c_str(), original_str.c_str());
  fprintf(check_file,"\n)\n");
  fclose(check_file);

	
  system("cvc3-2.2-i686-linux-opt -lang smt check.smt > result_check.txt");
 
  cout<<"\nCVC3 called\n";

  FILE *result_check_file = fopen("result_check.txt", "r");
  
  if(result_check_file == NULL) 
   {cout<<"\nCannot create result_check.txt\n"; exit(1);}


  char result[100];
  fscanf(result_check_file, "%s", result);
  fclose(result_check_file);
  string result_str(result);

  cout<<"\nresult_str = "<<result_str<<endl;

  if(result_str=="unsat")
    {
      cout<<"\nf'=>exists(X,f) holds\n";
      return true;
     }
      
  else if(result_str=="sat")
    { cout<<"\nf'=>exists(X,f) does not hold\n";
      return false;
    }
  else
    {
      cout<<"\nUnknown result from Yices"<<result_str<<endl;
      exit(1);
    }


}

// Given a set of strings VarsToElim, return it as \exists variable_1....\exists variable_n
// in the SMTLIB old format (Note that CVC3 we have uses the old SMTLIB format)
string  getExistsString(set<string> &VarsToElim, map<string, int> &WidthTable)
{
  string ret="";
  for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
      string id_delta = *it;
      map<string, int>::iterator wit = WidthTable.find(id_delta);
      if(wit==WidthTable.end())
	{
	cout<<"\nVariable "<<id_delta<<" without width in function getExistsString in Eliminator.cpp\n";
	exit(1);
	}

      int width = wit->second;
      char temp[100];
      sprintf(temp, "%d", width);
      string width_str(temp);

      ret+= " (";
      ret+= "?";
      ret+= id_delta;
      ret+= " ";
      ret+="BitVec[";
      ret+= width_str;
      ret+="]) ";
    }
      
  return ret;
}

// Function to return the content of a dag (in propositional skeleton format) in old SMTLIB format
// Note that the CVC3 we have uses the old SMTLIB format
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(DAGNode *root, map<string, int> WidthTable, set<string> &VarSetToElim)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  propogateWidths(root, WidthTableForNodes, 1);
   
  return getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(root, "bool", WidthTableForNodes, 1, VarSetToElim);
}

// Function to traverse the dag node "node" and return the content of a dag (in propositional skeleton format) in old SMTLIB format
// This is called by callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style
// Note that the CVC3 we have uses the old SMTLIB format
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, set<string> &VarSetToElim)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)
        {
               if(VarSetToElim.find(name) != VarSetToElim.end())
		 {
		   dag_str="?";
		   dag_str+=name;		   
		 }
	       else
		{
		dag_str=name;
		}
               return dag_str;
        }
        else
          {

            if(name == "select")
              {
                dag_str = "(extract[";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();

                iout++;

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "const", WidthTableForNodes, 1, VarSetToElim) + ":";

                iout = node->Get_outlist_start();

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "const", WidthTableForNodes, 1, VarSetToElim) + "] ";

                iout = node->Get_outlist_start();

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {


		dag_str = "(concat ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
		    if(isBVConstant(*iout)) // child is a bv-constant. our concatenation printing may be incorrect
		      {
			cout<<"\nconcat is not supported by function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style currently if any operator is a bv-constant. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		      exit(1);
		      }
			
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "bvmul")
	      {
		dag_str = "(bvmul ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "bvadd")
	      {
		dag_str = "(bvadd ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	     else if(name == "bvsub")
	      {
		dag_str = "(bvsub ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "bvor" || name == "bvand")
	      {
		dag_str =  "(" +  name + " ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "if_then_else"))
	      {
		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {

                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		
              }

	    else if(name == "'0'")
              {
                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);

              }

	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      return dag_str;
		      }
		else if(context == "bv")
		      {
	
			if(name[0] == 'x') // hex
			  {
			  dag_str = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			else // Decimal
			  {
			  dag_str = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
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
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style\n";
		exit(1);
	      }
		    
	  }
}




// FUNCTIONS TO PROVE EXACTNESS using CVC3 ends here

// Given a dag node "parent" where the variable nodes have indexes(delta times) associated with them,
// create a copy of parent with indexes of the variable nodes changed
// based on the DeltaMap; This function uses dynamic programming
DAGNode* cloneWithRenamedDeltasWithDeltaMap(DAGNode* parent, map<int, int> &DeltaMap)
{
map<DAGNode*, DAGNode*> DPTable;
return cloneWithRenamedDeltasWithDeltaMapInternal(parent, DeltaMap, DPTable);
}


// Function to create clone with delta maps. Called from cloneWithRenamedDeltasWithDeltaMap
DAGNode* cloneWithRenamedDeltasWithDeltaMapInternal(DAGNode* parent, map<int, int> &DeltaMap, map<DAGNode*, DAGNode*> &DPTable)
    {

       map<DAGNode*, DAGNode*>::iterator dit=DPTable.find(parent);

       if(dit!=DPTable.end()) // Hit in DPTable
	{
	return dit->second;
	}        

        DAGNode* clone;
	// Get the size of outlist of parent
	unsigned int outlist_size = parent->Get_outlist_size();
	
                   
        if(outlist_size>0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = cloneWithRenamedDeltasWithDeltaMapInternal(*iout, DeltaMap, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = cloneWithRenamedDeltasWithDeltaMapInternal(*iout, DeltaMap, DPTable);
		    iout++;
                    child_2 = cloneWithRenamedDeltasWithDeltaMapInternal(*iout, DeltaMap, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

                    child_1 = cloneWithRenamedDeltasWithDeltaMapInternal(*iout, DeltaMap, DPTable);
		    iout++;
                    child_2 = cloneWithRenamedDeltasWithDeltaMapInternal(*iout, DeltaMap, DPTable);
		    iout++;
                    child_3 = cloneWithRenamedDeltasWithDeltaMapInternal(*iout, DeltaMap, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    Children.push_back(child_3);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function cloneWithRenamedDeltasWithDeltaMapInternal in Eliminator.cpp\n";
		 exit(1);
		}

	    }
	}
	else // leaf node
	  {
	    string id_delta = parent->Get_node_name();
	    Value* MyValue = (Value*)(parent->getValue());
	    Type MyType = MyValue->get_node_type();
	    int MyWidth = MyType.width;
	    int MyDataType = MyType.data_type;

	    if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
	      {
		int index = id_delta.find_last_of("_");
		if(index==string::npos)
		  {
		    cout<<"\nVariable name without time stamp in function cloneWithRenamedDeltas in Eliminator.cpp\n";
		    exit(1);
		  }
		string id = id_delta.substr(0, index);
		string delta_str = id_delta.substr(index+1);	
		int delta = atoi(delta_str.c_str());
		int new_delta;

                map<int, int>::iterator dmit = DeltaMap.find(delta); 
		if(dmit != DeltaMap.end())
			{
			new_delta = dmit->second;
			}
		else
			{
			new_delta = delta;
			}
		
		string new_name = id;
		new_name += "_";
		char temp[100];
		sprintf(temp, "%d", new_delta);
		delta_str = temp;
		new_name += delta_str;

		Value *v = new Value();
		Type *t = new Type();
 		t->data_type = MyDataType;
		t->width = MyWidth;
		v->set_node_type(*t);
		clone = dm.createNode (new_name,(void*)v);
	      }
	    else
	      {
		clone = dm.createNode(parent->Get_node_name(), parent->getValue());
	      }
            
	  }
  
        DPTable.insert(make_pair(parent, clone));
        return clone;
    } 

// Function called by getSize. Gets size of dag rooted at "v" in variable "Size"
void getSizeInternal(DAGNode *v, int &Size, set<DAGNode*> &DPTable)
{

  set<DAGNode*>::iterator dit = DPTable.find(v);

  if(dit != DPTable.end()) 
    {
      return;
    }
  if(dit == DPTable.end()) 
    {
      list<DAGNode*>::iterator iout = v->Get_outlist_start();
      unsigned int outlist_size = v->Get_outlist_size();
      
      for(int i=0;i<outlist_size;i++)
	{
	getSizeInternal(*iout, Size, DPTable);
        iout++;
	}
      Size++; // New DDNode encountered
      DPTable.insert(v);
    }
}

// Get size of a dag; uses dynamic programming
int getSize(DAGNode *dag)
{
  int Size = 0;
  set<DAGNode*> DPTable;
  DPTable.clear();
  getSizeInternal(dag, Size, DPTable);
  return Size;
}

 
// Simplify and node with multiple children and return the simplified node
// Called by quickSimplifyInternal
DAGNode* simplifyAndWithMultipleChildren(vector<DAGNode*> &Children)
{
  // check if any of the Children is false. If yes return false
  
  for(int i=0; i<Children.size(); i++)
    {
      if((Children[i])->Get_node_name()=="false")
	{
	  return createLogicalConstant("false");
	}
    }

  // check if any of the Children is true. If yes remove such children

  while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<Children.size();i++)
	{
	  if((Children[i])->Get_node_name()=="true") 
	    {
	      Children.erase(Children.begin()+i);
	      fixpoint_reached=false;
	      break;
	    }
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here
  
  return createDAG("and", Children, LOGICAL_SYMBOL, 1);
}


// Simplify or node with multiple children  and return the simplified node
// Called by quickSimplifyInternal
DAGNode* simplifyOrWithMultipleChildren(vector<DAGNode*> &Children)
{
  // check if any of the Children is true. If yes return true
  
  for(int i=0; i<Children.size(); i++)
    {
      if((Children[i])->Get_node_name()=="true")
	{
	  return createLogicalConstant("true");
	}
    }

  // check if any of the Children is false. If yes remove such children

  while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<Children.size();i++)
	{
	  if((Children[i])->Get_node_name()=="false") 
	    {
	      Children.erase(Children.begin()+i);
	      fixpoint_reached=false;
	      break;
	    }
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here
  
  return createDAG("or", Children, LOGICAL_SYMBOL, 1);
}

// change the global variables Max_LMEs, Min_LMEs, Max_LMDs, and Min_LMDs
// using NoOfEqs and NoOfDiseqs
void getLME_LMD_Counts(int NoOfEqs,int NoOfDiseqs)
{
  if(!apqelim_status_on) return;

  if(NoOfEqs > Max_LMEs)
    {
      Max_LMEs = NoOfEqs;
    }
  if(NoOfEqs < Min_LMEs)
    {
      Min_LMEs = NoOfEqs;
    }
  if(NoOfDiseqs > Max_LMDs)
    {
      Max_LMDs = NoOfDiseqs;
    }
  if(NoOfDiseqs < Min_LMDs)
    {
      Min_LMDs = NoOfDiseqs;
    }
}

// Find bit-level LMDs among the LMDs in LMDSet, and put them in BitLevelLMDSet 
void getBitLevelLMDs(set<DAGNode*> &LMDSet, map<string, int> &WidthTable, set<DAGNode*> &BitLevelLMDSet)
{
  for(set<DAGNode*>::iterator it = LMDSet.begin(); it != LMDSet.end(); it++)
    {
      if(getNoOfBitsInConstraint(*it, WidthTable) == 1)
	{
	  BitLevelLMDSet.insert(*it);
	}
    }
}

// Get the #LMEs, #LMDs, #bit-level LMDs in the dag "node", and
// set the global variables LMECount_In_DAG, LMDCount_In_DAG, and
// BitLevel_LMDCount_In_DAG
void getLMDECounts(DAGNode* node, map<string, int> &WidthTable)
{
set<DAGNode*> LMESet;
set<DAGNode*> LMDSet;
char status='t';
set<string> AlreadyEncountered;

//cout<<"\nnode = "<<getDAGStr(node)<<endl;

getLMEsAndLMDsInternal(node, LMESet, LMDSet, status, AlreadyEncountered);  

//showAPSet("LMESet", LMESet);
//showAPSet("LMDSet", LMDSet);

set<DAGNode*> BitLevelLMDSet;
getBitLevelLMDs(LMDSet, WidthTable, BitLevelLMDSet);
//showAPSet("BitLevelLMDSet", BitLevelLMDSet);

LMECount_In_DAG = LMESet.size();
LMDCount_In_DAG = LMDSet.size();
BitLevel_LMDCount_In_DAG = BitLevelLMDSet.size();

//cout<<"\nLMECount_In_DAG = "<<LMECount_In_DAG<<"\nLMDCount_In_DAG = "<<LMDCount_In_DAG<<endl;
//cout<<"BitLevel_LMDCount_In_DAG = "<<BitLevel_LMDCount_In_DAG<<endl;

}

// Function called by getLMDECounts to get the details
// Given dag rooted at "node", this function finds the set of LMEs and the set of LMDs in "node"
void getLMEsAndLMDsInternal(DAGNode* node, set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, char status, set<string> &AlreadyEncountered)
{
  char node_char[100];
  sprintf(node_char , "%x", node);
  string node_str(node_char);
      
  string key = node_str;
  key += status;

  // key includes address of node concatenated with status

  if(AlreadyEncountered.find(key)!=AlreadyEncountered.end()) // node is already encountered with the same status
    {
      return;
    }
           
  string MyName = node->Get_node_name();

  if(MyName == "=" || MyName == "equivalent") // LME node encountered
    {
      if(status=='x')
	{
	  // encountered in both original and negated form
	  LMESet.insert(node);
	  LMDSet.insert(node); // we need only the count of LME, LMD nodes. Hence fine even if we do not negate
	}
      else if(status=='t')
	{
	  // encountered in original form
	  LMESet.insert(node);
	}
      else if(status=='f')
	{
	  // encountered in negated form
	  LMDSet.insert(node);
	}
      else
	{
	  cout<<"\nUndefined status "<<status<<" in function getLMEsAndLMDsInternal in Eliminator.cpp\n";
	  exit(1);
	}
    }
  else if(MyName == "is_not_equal" || MyName == "not_equal_to") // LME node encountered
    {
      if(status=='x')
	{
	  // encountered in both original and negated form
	  LMESet.insert(node);
	  LMDSet.insert(node); // we need only the count of LME, LMD nodes. Hence fine even if we do not negate
	}
      else if(status=='t')
	{
	  // encountered in original form
	  LMDSet.insert(node);
	}
      else if(status=='f')
	{
	  // encountered in negated form
	  LMESet.insert(node);
	}
      else
	{
	  cout<<"\nUndefined status "<<status<<" in function getLMEsAndLMDsInternal in Eliminator.cpp\n";
	  exit(1);
	}

    }      
  else if(MyName == "if_then_else")  
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      getLMEsAndLMDsInternal(*it, LMESet, LMDSet, 'x', AlreadyEncountered);
      it++;
      getLMEsAndLMDsInternal(*it, LMESet, LMDSet, status, AlreadyEncountered);
      it++;
      getLMEsAndLMDsInternal(*it, LMESet, LMDSet, status, AlreadyEncountered);
    }
  else if(MyName == "not")  
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      if(status=='x')
	{
	  getLMEsAndLMDsInternal(*it, LMESet, LMDSet, status, AlreadyEncountered);
	}
      else if(status=='t')
	{
	  getLMEsAndLMDsInternal(*it, LMESet, LMDSet, 'f', AlreadyEncountered);
	}
      else if(status=='f')
	{
	  getLMEsAndLMDsInternal(*it, LMESet, LMDSet, 't', AlreadyEncountered);
	}
      else
	{
	  cout<<"\nUndefined status "<<status<<" in function getLMEsAndLMDsInternal in Eliminator.cpp\n";
	  exit(1);
	}

    }
  else
    {	  
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      for(int i=0; i<node->Get_outlist_size(); i++)
	{
	  getLMEsAndLMDsInternal(*it, LMESet, LMDSet, status, AlreadyEncountered);
	  it++;
	}
    }


  AlreadyEncountered.insert(key);
	
}        


// Get the number of bits in the variable to be eliminated in number_of_bits
// Note that the variable to be eliminated is VarNames[0]
void getNumberOfBitsInVarToElim(unsigned int &number_of_bits, list<string> &VarNames, map<string, int> &WidthTable)
{
  // variable to be eliminated is VarNames[0]
  list<string>::iterator it=VarNames.begin();
  string var_to_elim = *it;
  
  map<string, int>::iterator wit = WidthTable.find(var_to_elim);
  if(wit == WidthTable.end())
    {
      cout<<"\nVariable "<<var_to_elim<<" has no entry in WidthTable in function getNumberOfBitsInVarToElim in Eliminator.cpp\n";
      exit(1);
    }
  number_of_bits = wit->second;
}
  
// Print the details needed per call in files layer1_calls_fp and layer2_calls_fp, and set global variables
// according to the details of the call
void print_Layer_i_Calls(unsigned int number_of_lmes, unsigned int number_of_lmds, unsigned int number_of_bits, int time_in_layer, int layer, unsigned long long int duration_layer_ms)
{
  if(!apqelim_status_on) return; // This is in order to catch the q.e. instances arising from DAGQElim

  if(layer==1)
    {
      // printing the data
      
      //fprintf(layer1_calls_fp, "\n%u   \t%u   \t%u                  \t%d\n", number_of_lmes, number_of_lmds, number_of_bits, time_in_layer);
      fprintf(layer1_calls_fp, "\n%u   \t%u   \t%u                  \t%llu\n", number_of_lmes, number_of_lmds, number_of_bits, duration_layer_ms);
      //cout<<"\n#LMEs = "<<number_of_lmes<<"\t#LMDs = "<<number_of_lmds<<"\t#Bits = "<<number_of_bits<<"\tTime = "<<time_in_layer<<endl;

      // updating the data for layer-I

      L1_Calls++;

      L1_Total_Time += time_in_layer;
      
      if(time_in_layer > L1_Max_Time)
	{
	  L1_Max_Time = time_in_layer;
	}

      if(time_in_layer < L1_Min_Time)
	{
	  L1_Min_Time = time_in_layer;
	}


      L1_Total_Time_In_ms += duration_layer_ms;


      if(duration_layer_ms > L1_Max_Time_In_ms)
	{
	  L1_Max_Time_In_ms = duration_layer_ms;
	}

      if(duration_layer_ms < L1_Min_Time_In_ms)
	{
	  L1_Min_Time_In_ms = duration_layer_ms;
	}
      
      L1_Total_Bitwidth_of_Var_Eliminated += number_of_bits;
      
      if(number_of_bits > L1_Max_Bitwidth_of_Var_Eliminated)
	{
	  L1_Max_Bitwidth_of_Var_Eliminated = number_of_bits;
	}

      if(number_of_bits < L1_Min_Bitwidth_of_Var_Eliminated)
	{
	  L1_Min_Bitwidth_of_Var_Eliminated = number_of_bits;
	}

      if(number_of_lmds > 0) 
	{
	  L1_Calls_with_Gt_Zero_Diseqs++;


	  L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs += time_in_layer;

	  if(time_in_layer > L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs = time_in_layer;
	    }

	  if(time_in_layer < L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs = time_in_layer;
	    }


      L1_Total_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms += duration_layer_ms;


      if(duration_layer_ms > L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms)
	{
	  L1_Max_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms = duration_layer_ms;
	}

      if(duration_layer_ms < L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms)
	{
	 L1_Min_Time_for_Calls_with_Gt_Zero_Diseqs_In_ms  = duration_layer_ms;
	}


      
	  L1_Total_Diseqs_for_Calls_with_Gt_Zero_Diseqs += number_of_lmds;
      
	  if(number_of_lmds > L1_Max_Diseqs_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Max_Diseqs_for_Calls_with_Gt_Zero_Diseqs = number_of_lmds;
	    }

	  if(number_of_lmds < L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Min_Diseqs_for_Calls_with_Gt_Zero_Diseqs = number_of_lmds;
	    }


	  float eqs_div_diseqs = (float)number_of_lmes/(float)number_of_lmds;

	  L1_Total_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs += eqs_div_diseqs;
      
	  if(eqs_div_diseqs > L1_Max_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Max_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs = eqs_div_diseqs;
	    }

	  if(eqs_div_diseqs < L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Min_Eq_Div_Diseq_for_Calls_with_Gt_Zero_Diseqs = eqs_div_diseqs;
	    }


	  // New details needed for Layer -I and Layer -II

	  L1_Total_Eqs_for_Calls_with_Gt_Zero_Diseqs += number_of_lmes;
      
	  if(number_of_lmes > L1_Max_Eqs_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Max_Eqs_for_Calls_with_Gt_Zero_Diseqs = number_of_lmes;
	    }

	  if(number_of_lmes < L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Min_Eqs_for_Calls_with_Gt_Zero_Diseqs = number_of_lmes;
	    }

	  L1_Total_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs += number_of_bits;
      
	  if(number_of_bits > L1_Max_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs)
	    {
	     L1_Max_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs  = number_of_bits;
	    }

	  if(number_of_bits < L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs)
	    {
	      L1_Min_Bitwidth_of_Var_Eliminated_for_Calls_with_Gt_Zero_Diseqs = number_of_bits;
	    }

	  // New details needed for Layer -I and Layer -II ends here



	  
	}// if(number_of_lmds > 0) ends here
      else
	{
	  L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs += time_in_layer;

	  if(time_in_layer > L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs)
	    {
	      L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs = time_in_layer;
	    }

	  if(time_in_layer < L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs)
	    {
	      L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs = time_in_layer;
	    }

      L1_Total_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms += duration_layer_ms;


      if(duration_layer_ms > L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms)
	{
	  L1_Max_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms = duration_layer_ms;
	}

      if(duration_layer_ms < L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms)
	{
	 L1_Min_Time_for_Calls_with_Eq_Zero_Diseqs_In_ms  = duration_layer_ms;
	}

	  
	}// number_of_lmds <= 0 ends here
      

    }//if(layer==1)

  else if(layer==2)
    {
      if(number_of_lmds!=0) // only if number_of_lmds!=0 layer-2's internals are called
	{
	  //fprintf(layer2_calls_fp, "\n%u   \t%u   \t%u                  \t%d\n", number_of_lmes, number_of_lmds, number_of_bits, time_in_layer);
	  fprintf(layer2_calls_fp, "\n%u   \t%u   \t%u                  \t%llu\n", number_of_lmes, number_of_lmds, number_of_bits, duration_layer_ms);
	  //cout<<"\n#LMEs = "<<number_of_lmes<<"\t#LMDs = "<<number_of_lmds<<"\t#Bits = "<<number_of_bits<<"\tTime = "<<time_in_layer<<endl;

	  // updating the data for layer-II

	  L2_Calls++;

	  L2_Total_Time += time_in_layer;

	  if(time_in_layer > L2_Max_Time)
	    {
	      L2_Max_Time = time_in_layer;
	    }

	  if(time_in_layer < L2_Min_Time)
	    {
	      L2_Min_Time = time_in_layer;
	    }

      L2_Total_Time_In_ms += duration_layer_ms;


      if(duration_layer_ms > L2_Max_Time_In_ms)
	{
	  L2_Max_Time_In_ms = duration_layer_ms;
	}

      if(duration_layer_ms < L2_Min_Time_In_ms)
	{
	  L2_Min_Time_In_ms = duration_layer_ms;
	}

        
      
	  L2_Total_Bitwidth_of_Var_Eliminated += number_of_bits;
      
	  if(number_of_bits > L2_Max_Bitwidth_of_Var_Eliminated)
	    {
	      L2_Max_Bitwidth_of_Var_Eliminated = number_of_bits;
	    }

	  if(number_of_bits < L2_Min_Bitwidth_of_Var_Eliminated)
	    {
	      L2_Min_Bitwidth_of_Var_Eliminated = number_of_bits;
	    }

	  L2_Total_Diseqs += number_of_lmds;
      
	  if(number_of_lmds > L2_Max_Diseqs)
	    {
	      L2_Max_Diseqs = number_of_lmds;
	    }

	  if(number_of_lmds < L2_Min_Diseqs)
	    {
	      L2_Min_Diseqs = number_of_lmds;
	    }

	  if(number_of_lmes > 0)
	    {
	      L2_Calls_with_Eqs++;
	    }
	  else
	    {
	      L2_Calls_with_Zero_Eqs++;
	    }

	}// if(number_of_lmds!=0)
    }// if(layer==2)
  else
    {
      cout<<"\nUndefined layer "<<layer<<" in function print_Layer_i_Calls in Eliminator.cpp\n";
      exit(1);
    }
}

// Get the #LMEs and #LMDs in the input to Layer 1
void getNumberOfLMEsNumberOfLMDs_Layer_1(unsigned int &number_of_lmes, unsigned int &number_of_lmds, Matrix &EqCoefMat, Matrix &DisEqCoefMat)
{
  number_of_lmes = EqCoefMat.size();
  number_of_lmds = DisEqCoefMat.size();

} 

// Get the #LMEs and #LMDs in the input to Layer 2
void getNumberOfLMEsNumberOfLMDs_Layer_2(unsigned int &number_of_lmes, unsigned int &number_of_lmds, Matrix &EqCoefMat, Matrix &DisEqCoefMat, int NoVarsToElim)
{
  if(NoVarsToElim>1) // if this happens, then this function is not defined for this
    {
      cout<<"\nWarning! Function getNumberOfLMEsNumberOfLMDsNumberOfVars_Layer_2 in Eliminator.cpp called with NoVarsToElim>1\n";
      number_of_lmes=0;
      number_of_lmds=0;
      return;
    }

  if(DisEqCoefMat.empty()) // no disequations. No role of layer 2
    {
      number_of_lmes=0;
      number_of_lmds=0;
      return;
    }


  map<int, ULINT> CoefsOfVarToElimEqualities, CoefsOfVarToElimDisEqualities;
  // The map stores coef of variable to be eliminated for each disequality. If coef of variable to be eliminated for the disequality is zero, then it is not stored in the map

  CoefsOfVarToElimEqualities.clear();

  if(!EqCoefMat.empty()) 
    {
      copyColumnToMap(EqCoefMat, 0, CoefsOfVarToElimEqualities);
    }

  copyColumnToMap(DisEqCoefMat, 0, CoefsOfVarToElimDisEqualities);
		
  number_of_lmds = CoefsOfVarToElimDisEqualities.size();

  if(number_of_lmds == 0)// no disequations. No role of layer 2
    {
      number_of_lmes=0;
      number_of_lmds=0;
      return;
    }

  // Check if there is at least one LME in the original problem.

  if(!CoefsOfVarToElimEqualities.empty()) // yes, there is
    {
      number_of_lmes=1;
    }
  else
    {
      number_of_lmes=0;
    }
  return;

}

// Print the given formula rooted at "root" in SMTLIB format (with the additional :exists specifier) in file "file_name"
// In the future use this function to express a given arbitrary formula in SMTLIB format
// when the result is needed in a tree format
void printInputToQEliminatorInSMTLibFormatInGivenFile(DAGNode *root, map<string, int> &WidthTable, set<string> &VarSetToElim, string file_name)
{
  cout<<"\nPrinting in "<<file_name<<endl;

  FILE *smt_file = fopen(file_name.c_str(), "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<file_name<<"\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  set<string> Vars;
  getDAGNames(root, Vars);

  writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, WidthTable, Vars);

 
  fprintf(smt_file,"\n:exists(");
  for(set<string>::iterator it=VarSetToElim.begin(); it!=VarSetToElim.end(); it++)
    {
      fprintf(smt_file," %s",(*it).c_str());
    }
  fprintf(smt_file,")\n");

  
  string dag_str;
  
  if(LMI_handling_enabled)
  {
   dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(root, WidthTable);
   }
  else
  {
  dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);
  }

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);

  cout<<"\nPrinted in "<<file_name<<endl;
  }

// Return true if the global variable "time_out mode" is true and time out has happened;
// Return false otherwise
bool checkTimeOut()
	{
	  if(!time_out_enabled) // time_out mode is disabled
		{
		return false;
		}

	  if(timed_out) // already timed out. No need to check
	    {
	      return true;
	    }

	  if(time_out_start == 0)
		{
		cout<<"\nYou have set the variable time_out_enabled in main.cc. But time_out_start is not set. Either disable time_out_enabled in main.cc (this disables the timeout mechanism) or set time_out_start before calling DDManager::MultiVarQElimWithStrategy / DAGEliminator::Monniaux_Quantifier_Eliminator"<<endl;
		exit(1);
		}

	  time_t present_time, duration;
	  
	  time(&present_time);
	  duration = present_time - time_out_start;
	  if(duration >= time_out)
		{
		return true;
		}
	return false;
	}

  

//  Functions added on 26 Nov 2010 to enable quantifier elimination at bit-level for comparison

// Function to do the same work as done by Ganesh & Dill
// Given a conjunction of LMCs as matrices, this substitutes the occurances of the odd variables
// to be eliminated using Ganesh & Dill's work
// Effect : The matrices are changed
void qEliminatorForEquationsAndDisequationsOnlyForOddCoefficients(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod)
{

// Here first NoVarsToElim of the variables is to be eliminated

int VarToElimIndex, EqnToSubstIndex;

// NoOfVarsEliminated is the no: of variables from x1,...,xM-1 already elimiated

//cout<<"\nTrying to eliminate variables with odd coefficients first\n";

 for(VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++) 
   {

     if(EqCoefMat.empty()) 
       {
	 //cout<<"\nNo equations left\n"; 
       break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has odd coefficient \n";
     
     getEquationWithOddCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex); 

     if(EqnToSubstIndex==-1) {cout<<"\nNo equation with odd coefficient for the variable\n";}

     else
	{
	  //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";

	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";

	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnEq, PivotColumnDiseq;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow

	//cout<<"\nChecking if the equation is solvable\n";
	// Check if the Equation selected is solvable. If no, return saying unsat
	if(!equationSolvable(PivotRow, PivotCoef, Mod))
	  {cout<<"\nThe equation selected is unsolvable. Hence set of equations is unsat. Exiting...\n"; exit(1);}


	copyColumn(EqCoefMat, VarToElimIndex, PivotColumnEq); // selected column copied into PivotColumnEqn
	
	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	// delete pivot element from pivot column of equations as this would lead to equation with all coefs set to zero 
	deleteRow(PivotColumnEq, EqnToSubstIndex);


	// PivotRow deleted from EqCoefMat
	deleteRow(EqCoefMat, EqnToSubstIndex);
	// PivotRow deleted from EqConstMat
	deleteRow(EqConstMat, EqnToSubstIndex);

	// PivotColumn nullified in EqCoefMat
	nullifyColumn(EqCoefMat, VarToElimIndex);


	//cout<<"\nAfter deletions\n";
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);

	Matrix NewEqCoefMat;
	// Multiply PivotColumnEq and PivotRow to get NewEqCoefMat
	matMultiply(PivotColumnEq, PivotRow, NewEqCoefMat, Mod);
	//cout<<"\nNewEqCoefMat\n";
	//showMatrix(NewEqCoefMat);

	// EqCoefMat =  EqCoefMat + NewEqCoefMat
	matAdd(EqCoefMat, NewEqCoefMat, Mod);
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);


	// Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef*MI(Pivot element) = "<<PivotCoef<<"\n";

	// Convert PivotColumnEq to PivotColumnEq*PivotCoef
	scalarMatMultiply(PivotColumnEq, PivotCoef, Mod);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	// EqConstMat = EqConstMat-PivotColumnEq
	matSub(EqConstMat, PivotColumnEq, Mod);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);

	//cout<<"\nAfter elimination, we have\n";
	//showEquations(EqCoefMat, EqConstMat);


	// For disequations

	if(!DisEqCoefMat.empty())
	  {
	    copyColumn(DisEqCoefMat, VarToElimIndex, PivotColumnDiseq); // selected column copied into PivotColumnDiseqn
	
	    // PivotColumn nullified in DisEqCoefMat
	    nullifyColumn(DisEqCoefMat, VarToElimIndex);
	
	    Matrix NewDisEqCoefMat;
	    // Multiply PivotColumnDiseq and PivotRow to get NewDisEqCoefMat
	    matMultiply(PivotColumnDiseq, PivotRow, NewDisEqCoefMat, Mod);
	    //cout<<"\nNewDisEqCoefMat\n";
	    //showMatrix(NewDisEqCoefMat);
	    
	    // DisEqCoefMat =  DisEqCoefMat + NewDisEqCoefMat
	    matAdd(DisEqCoefMat, NewDisEqCoefMat, Mod);
	    //cout<<"\nDisEqCoefMat\n";
	    //showMatrix(DisEqCoefMat);
	

	    // Convert PivotColumnDiseq to PivotColumnDiseq*PivotCoef
	    //cout<<"\nPivotColumnDiseq*PivotCoef\n";
	    scalarMatMultiply(PivotColumnDiseq, PivotCoef, Mod);
	    //cout<<"\nPivotColumnDiseq\n";
	    //showMatrix(PivotColumnDiseq);
	    
	    // DisEqConstMat = DisEqConstMat - PivotColumnDiseq
	    matSub(DisEqConstMat, PivotColumnDiseq, Mod);
	    //cout<<"\nDisEqConstMat\n";
	    //showMatrix(DisEqConstMat);
	    
	    //cout<<"\nAfter elimination, we have\n";
	    //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
	  }
	
	}
   }// for each variable
}// Function ends here




// Function implementing the procedure BV_Solve in CAV'11 paper
// Given a conjunction of LMCs as matrices, this substitutes the occurances of the odd variables
// to be eliminated using Ganesh & Dill's work, and employs bit-blasting + QE at bit-level
// Result is NULL always (Although the result is generated, it is not returned)
DAGNode* qEliminatorReturningDAGOnlyForOddCoefficientsFollowedByBitBlasting(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable)
{
  
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  //cout<<"\nShowing original set of constraints\n";
  //showEquations(EqCoefMatPassed, EqConstMatPassed); // Show the equations
  //showDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed); // Show the disequations
  //cout<<"\nWe need to eliminate the first "<<NoVarsToElim<<" variables \n";


  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  
  //cout<<"\nShowing duplicate eliminated set of constraints\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

  //cout<<"\nNo: of vars to elim = "<<NoVarsToElim<<endl;

  DAGNode *resultDAG=NULL;

  if(!EqCoefMat.empty())
    qEliminatorForEquationsAndDisequationsOnlyForOddCoefficients(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod); // Eliminate first NoVarsToElim variables from the disequations CX<>D and equations AX=B and get back the matrices

  // This function finds the disequations in which there are no variables left to eliminate, removes them from set of disequations and copies them in a matrices DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat
  
  Matrix DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat;
  Matrix EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat;
  
  if(!DisEqCoefMat.empty()) eliminateDisequationsFreeOfAllVarsToElimAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, NoVarsToElim, DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat);

  if(!EqCoefMat.empty()) eliminateEquationsFreeOfAllVarsToElimAndCopyInNewMatrix(EqCoefMat, EqConstMat, NoVarsToElim, EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat);

  
  //cout<<"\n###############################\nPrinting the Equalities containing any variable to eliminate\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nPrinting the Disequalities containing any variable to eliminate\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  //cout<<"\n###############################\n";

  
  
   DAGNode *DagFromEqs, *DagFromDiseqs;
   DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, VarNames, Mod);
   DagFromDiseqs = getDAGFromDisEqs(DisEqCoefMat, DisEqConstMat, VarNames, Mod);
  	
    if(DagFromEqs != NULL)
	{
	if(resultDAG == NULL) resultDAG=DagFromEqs; 
	else 
	  { 
	    resultDAG = createDAG("and",resultDAG, DagFromEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromDiseqs != NULL)
	{
      	if(resultDAG == NULL) resultDAG=DagFromDiseqs; 
	else 
	  {
	    resultDAG = createDAG("and",resultDAG, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}

 DAGNode* BlastedResultDAG = NULL;

 //if(DisEqCoefMat.size()>0) // specific to benchmark: Why this is needed?
//	{
         if(resultDAG!=NULL)
		{
        	resultDAG = quickSimplify(resultDAG);
		cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;
        	bool do_blasting = true;
		if(do_blasting)
			{
			set<string> VarSetToElim;
			getVarSetToElimFromVarNames(VarNames, NoVarsToElim, VarSetToElim);
			
			bitBlastAndQuantifyUsingCDD(resultDAG, VarSetToElim, WidthTable);
			}
		bool print_input_in_smtlib_format = true;
		if(print_input_in_smtlib_format)
			{
			set<string> VarSetToElim;
			getVarSetToElimFromVarNames(VarNames, NoVarsToElim, VarSetToElim);
			showSet(VarSetToElim, "VarSetToElim");
			static int file_counter = 1;
  			char file_counter_char[100];
			sprintf(file_counter_char, "%d", file_counter);
			string file_counter_str(file_counter_char);
			string file_name = "input_to_blasting_";
			file_name += file_counter_str;
			file_name += ".smt";
			printInputToQEliminatorInSMTLibFormatInGivenFile(resultDAG, WidthTable, VarSetToElim, file_name);
			file_counter++;
			}
		}
//	}
     
 return BlastedResultDAG;
 
}

// Get set of variables to be eliminated "VarSetToElim" from list of variables "VarNames"
// A variable in "VarNames" is to be eliminated if its index <= NoVarsToElim
// Called by qEliminatorReturningDAGOnlyForOddCoefficientsFollowedByBitBlasting
void getVarSetToElimFromVarNames(list<string> &VarNames, int NoVarsToElim, set<string> &VarSetToElim)
{
int var_number = 1;
for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++)
	{
	if(var_number <= NoVarsToElim)
		{
		VarSetToElim.insert(*it);
		}
	var_number++;
	}
}


// This function finds the equations in which there are no variables left to eliminate, removes them from set of equations and copies them in a new matrices
void eliminateEquationsFreeOfAllVarsToElimAndCopyInNewMatrix(Matrix &EqCoefMat, Matrix &EqConstMat, int NoVarsToElim, Matrix &EqFreeOfAllVarsToElimCoefMat, Matrix &EqFreeOfAllVarsToElimConstMat)
{
  // Take each equation d and check if Coef[d][v]=0 for v = 0 to NoOfVarsToElim
  
 list<int> FreeEqs;
 for(int i=0; i<EqCoefMat.size(); i++)
		{
		  bool IsEqFree=true;
		  for(int v=0; v<NoVarsToElim; v++)
		    {
		    if(EqCoefMat[i][v] != 0) // Equation i is not free of variable v
		      {
			IsEqFree=false;
			break;
		      }
		    }
		  if(IsEqFree)
		    { //cout<<"\nEquation "<<i<<" is free of all variables to be eliminated\n";
		    FreeEqs.push_back(i);}
		}

 FreeEqs.reverse(); // To delete the higher rows first
 for(list<int>::iterator it=FreeEqs.begin(); it != FreeEqs.end(); it++)
   {
     //showMatrix(EqCoefMat);
     copyRow(EqCoefMat, *it, EqFreeOfAllVarsToElimCoefMat); // copyRow does not depend on whether EqFreeOfVarToElimCoefMat is row matrix or if it is empty
     deleteRow(EqCoefMat, *it);
     
     //showMatrix(EqConstMat);
     copyRow(EqConstMat, *it, EqFreeOfAllVarsToElimConstMat); // copyRow does not depend on whether EqFreeOfVarToElimConstMat is row matrix or if it is empty
     deleteRow(EqConstMat, *it);
   }
}


// Given a set of constraints and a set of variables to be eliminated,
// this function calls OmegaTest with scope reduction as well as QE_LMC (APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG), and
// returns the result from APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG
DAGNode* QEliminatorForMultipleVariablesUsingLMAandILPwithScopeReductionReturningDAG(set<DAGNode*> Constraints, set<string> VarsToElim, map<string, int> &WidthTable)
{

  //cout<<"\nQEliminatorForMultipleVariablesUsingLMAandILPwithScopeReductionReturningDAG called\n";

  while(VarsToElim.size()!=0)
  {

    //showSet(VarsToElim,"VarsToElim");
    //showAPSet("Constraints", Constraints);

  map<DAGNode*, set<string> > Constraint_VarsToElim_Map;
  map<string, set<DAGNode*> > VarToElim_Constraints_Map;
  map<set<DAGNode*>, set<string> > Constraints_VarsToElim_Map;

  // Create Constraint_VarsToElim_Map
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;
     set<string> Support;
     getDAGNames(Constraint, Support); // Support of Constraint obtained
     set<string> VarsToElimInSupport;
     set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));
     
     Constraint_VarsToElim_Map.insert(make_pair(Constraint, VarsToElimInSupport));
    }

  // Showing Constraint_VarsToElim_Map

//   cout<<"\nConstraint_VarsToElim_Map\n";

//   for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
//     {
//      DAGNode *Constraint = it->first;
//      set<string> Support = it->second;
//      cout<<"\nConstraint = "<<getDAGStr(Constraint)<<"\t";
//      showSet(Support, "Support");
//    }
   
  // Create VarToElim_Constraints_Map

  bool Constraints_Free_Of_VarsToElim = true;

  for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
    {
     DAGNode *Constraint = it->first;
     set<string> Support = it->second;

     if(!Support.empty())
	{
	Constraints_Free_Of_VarsToElim = false;
	}

     for(set<string>::iterator it2=Support.begin(); it2!=Support.end(); it2++)
    	{
	string Variable = *it2;
	map<string, set<DAGNode*> >::iterator vcm_it = VarToElim_Constraints_Map.find(Variable);
	if(vcm_it != VarToElim_Constraints_Map.end())
		{
		(vcm_it->second).insert(Constraint);
		}
	else
		{
		set<DAGNode*> Constraints;
		Constraints.insert(Constraint);
		VarToElim_Constraints_Map.insert(make_pair(Variable, Constraints));
		}
   	}  
     }

  if(Constraints_Free_Of_VarsToElim) // All constraints free of VarsToElim
	{
	break;
	}

  // Showing VarToElim_Constraints_Map

//   cout<<"\nVarToElim_Constraints_Map\n";

//   for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
//     {
//      string VarToElim = it->first;
//      set<DAGNode*> Constraints = it->second;
//      cout<<"\nVarToElim = "<<VarToElim<<"\t";
//      showAPSet("Constraints", Constraints);
//    }

  // Create Constraints_VarsToElim_Map

  for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
    {
     string VarToElim = it->first;
     set<DAGNode*> Constraints = it->second;
     map<set<DAGNode*>, set<string> >::iterator cvm_it = Constraints_VarsToElim_Map.find(Constraints);
     if(cvm_it != Constraints_VarsToElim_Map.end())
	{
	(cvm_it->second).insert(VarToElim);
	}
    else	
	{
	set<string> Support;
	Support.insert(VarToElim);
	Constraints_VarsToElim_Map.insert(make_pair(Constraints, Support));
	}
    }

  // Showing Constraints_VarsToElim_Map

//   cout<<"\nConstraints_VarsToElim_Map\n";

//   for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
//     {
//      set<DAGNode*> Constraints = it->first;
//      set<string>  VarsToElimInConstraints = it->second;
//      showAPSet("Constraints", Constraints);
//      showSet(VarsToElimInConstraints, "VarsToElimInConstraints");
//    }

  // selecting the entry in Constraints_VarsToElim_Map with the minimum sized LHS i.e. Constraints

  map<set<DAGNode*>, set<string> >::iterator selected_it=Constraints_VarsToElim_Map.begin(); 
  int size_of_constraints = (selected_it->first).size();

  for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
    {
    if( (it->first).size() < size_of_constraints)
	{
	selected_it = it;
	size_of_constraints = (it->first).size();
	}
    }
  
  // selected_it is the entry in Constraints_VarsToElim_Map with the minimum sized LHS i.e. Constraints

  set<string> VariablesSelectedForElimination = selected_it->second;
  set<DAGNode*> ConstraintsSelected = selected_it->first;
  set<DAGNode*> RemainingConstraints;
  set<DAGNode*> QEliminated_Constraints;

  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;
     if(ConstraintsSelected.find(Constraint)==ConstraintsSelected.end()) // Constraint is not ConstraintsSelected
	{
	RemainingConstraints.insert(Constraint);
	}
   }

//   showAPSet("ConstraintsSelected", ConstraintsSelected);
//   showAPSet("RemainingConstraints", RemainingConstraints);
  
  set<DAGNode*> Temp_Set; // only required to satisfy the fact that APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG requires a set as second argument. APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG unions first and second sets to get a set say 'S' and does qunatifier elimination on this. Hence, it is fine to keep first argument as Constraints_Set and second argument as Temp_Set (which is empty)

   DAGNode* QEliminated_Constraint = NULL;

   if(ConstraintsSelected.size()>0)
	{
	QEliminated_Constraint = quickSimplify(APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(ConstraintsSelected, Temp_Set, VariablesSelectedForElimination, WidthTable)); // Calls elimination by LMA
         
        DAGNode *TempDAG = callOmegaTestReturningDAG(ConstraintsSelected, Temp_Set, VariablesSelectedForElimination, WidthTable); // Calls elimination by OmegaTest
	}

   if(QEliminated_Constraint != NULL && !isTrue(QEliminated_Constraint) ) // stands for true
	{
	  //cout<<"\nQEliminated_Constraint = "<<getDAGStr(QEliminated_Constraint)<<"\n";
	  splitConjunctionIntoSet(QEliminated_Constraint, QEliminated_Constraints);
	  //showAPSet("QEliminated_Constraints", QEliminated_Constraints);
	}

    Constraints.clear();
    set_union(RemainingConstraints.begin(), RemainingConstraints.end(), QEliminated_Constraints.begin(), QEliminated_Constraints.end(),inserter(Constraints, Constraints.begin())); 
     // Constraints recreated

    set<string> RemainingVariables;
    set_difference(VarsToElim.begin(), VarsToElim.end(), VariablesSelectedForElimination.begin(), VariablesSelectedForElimination.end(),inserter(RemainingVariables, RemainingVariables.begin()));
    VarsToElim = RemainingVariables;
   // VarsToElim recreated
  }// while(VarsToElim.size()!=0)

  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
  
    if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    return resultDAG;
}


// Given a set of LMEs and LMDs, and a set of variables to be eliminated, 
// perform QE using OmegaTest. return value is always TRUE dag node
DAGNode* callOmegaTestReturningDAG(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> &WidthTable)
{

  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();

  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
     names.insert(*it); 
    }

  attachSetToList(names, VarNames);
  NoVarsToElim=VarsToElim.size();

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  attachSetToList(names, VarNames);

  NoVars=VarNames.size();

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));



  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;

  time_t start, end, duration;
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;

  time(&start);
  gettimeofday (&start_ms, NULL);


  resultDAG = qEliminator_ILP_ReturningDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, VarNames, WidthTable);

  gettimeofday (&finish_ms, NULL);
  time(&end);
	
  duration = end - start;
  Total_Omega_Test_Time = Total_Omega_Test_Time + duration;
  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;	  	  
  Total_Omega_Test_Time_In_ms =  Total_Omega_Test_Time_In_ms + duration_ms;



  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }


 return resultDAG;

}

// Get the #variables in a set of LMEs and set of LMDs
void getNumberOfVars(unsigned int &number_of_vars, Matrix &EqCoefMat, Matrix &DisEqCoefMat)
{
  if(!EqCoefMat.empty())
    {
      number_of_vars = EqCoefMat[0].size();
    }
  else if(!DisEqCoefMat.empty())
    {
      number_of_vars = DisEqCoefMat[0].size();
    }
  else
    {
      number_of_vars = 0;
    }

} 

// Set global variables according to the details per call of QE_LMC
void getAP_QElimDetails(unsigned int number_of_lmes, unsigned int number_of_lmds, unsigned int number_of_vars)
{
  if(!apqelim_status_on) return; // This is in order to catch the q.e. instances arising from DAGQElim

  
  L1_Total_Eqs += number_of_lmes;
  
  if(number_of_lmes > L1_Max_Eqs)
	    {
	      L1_Max_Eqs = number_of_lmes;
	    }

  if(number_of_lmes < L1_Min_Eqs)
	    {
	      L1_Min_Eqs = number_of_lmes;
	    }

   L1_Total_Diseqs += number_of_lmds;
  
  if(number_of_lmds > L1_Max_Diseqs)
	    {
	      L1_Max_Diseqs = number_of_lmds;
	    }

  if(number_of_lmds < L1_Min_Diseqs)
	    {
	      L1_Min_Diseqs = number_of_lmds;
	    }

  L1_Total_Vars += number_of_vars;
  
  if(number_of_vars > L1_Max_Vars)
	    {
	      L1_Max_Vars = number_of_vars;
	    }

  if(number_of_vars < L1_Min_Vars)
	    {
	      L1_Min_Vars = number_of_vars;
	    }
}

// Seggregate a set of LMCs into (i) a set of LMEs and (ii) a set of LMDs, and
// recreate the bit-level LMDs as LMEs in the form 2^k.x = t where x is the lowest
// in the lexicographic ordering, and t is free of x
void seggregateIntoLMEsAndLMDs(set<DAGNode*> &Constraints, set<DAGNode*> &LMEs, set<DAGNode*> &LMDs, map<string, int> &WidthTable)
{
for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     DAGNode* Constraint = *it; 
     if(Constraint->Get_node_name()=="=") // LME
       {
         DAGNode *RecreatedEqn = recreateEquationInProperForm(Constraint, WidthTable);
	 LMEs.insert(RecreatedEqn);
       }
     else if(Constraint->Get_node_name()=="is_not_equal") // LMD
       {
	 if(getNoOfBitsInConstraint(Constraint, WidthTable) == 1)// Bit-level LMD = LME
	   {
            DAGNode *RecreatedEqn = recreateEquationFromBitLevelDisequationInProperForm(Constraint, WidthTable);
	    LMEs.insert(RecreatedEqn);
	   }
	 else // Non Bit-level LMD
	   {
	    LMDs.insert(Constraint);
	   }
       }
     else
       {
	 cout<<"\nUnknown constraint "<<getDAGStr(Constraint)<<" inside seggregateIntoLMEsAndLMDs in Eliminator.cpp\n";
	 exit(1);
       }
    }
}

// recreate a bit-level LMD as LME in the form 2^k.x = t where x is the lowest
// in the lexicographic ordering, and t is free of x. Return the LME as dag node*
DAGNode* recreateEquationFromBitLevelDisequationInProperForm(DAGNode* disequation, map<string, int> WidthTable)
{
  // Get the list of variables
  list<string> VarNames;
  set<string> names;
  
  getDAGNames(disequation, names);
  attachSetToList(names, VarNames);
  VarNames.sort();

  DAGNode *lhs, *rhs;
  list<DAGNode*>::iterator out_list_it = disequation->Get_outlist_start();
  lhs = (*out_list_it);
  out_list_it++;
  rhs = (*out_list_it);
  
  if(lhs==rhs) 
	{
	cout<<"\nDisequation "<<getDAGStr(disequation)<<" which is false inside recreateEquationFromBitLevelDisequationInProperForm in Eliminator.cpp\n";
	exit(1);
	}
  if(names.empty()) return disequation;

 
  Matrix A, C; // Row matrices
  Matrix B, D; // Row matrices

  getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
  getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D


   // We have AX+B <> CX+D i.e. not(AX+B = CX+D)
  // Convert A to A-C and D to D-B to get not((A-C)X = (D-B))

  ULINT LocalMod=2;
 
  matSub(A, C, LocalMod);
  matSub(D, B, LocalMod);

  // Now we have not(AX = D)

  // currently constant is in D[0][0]. we are converting the diseqn to eqn

  if(D[0][0]==1) D[0][0]=0; else D[0][0]=1;
 
  // Now we have AX = not D

  // Converting the first element in coef mat to normal form

  ULINT FirstElement = A[0][0];

  ULINT OddPart = getOddPart(FirstElement);


  if(OddPart != 0)
    {

      ULINT MIOfOddPart = mInverse(OddPart, LocalMod);

      scalarMatMultiply(A, MIOfOddPart, LocalMod);

      scalarMatMultiply(D, MIOfOddPart, LocalMod);
    }


  // Now equation is AX = D with first element in normal form
  
  DAGNode* RecreatedEquation;
  
  bool old_imp = false;
  if(old_imp)
	RecreatedEquation = getDAGFromEqs(A, D, VarNames, LocalMod);
  else
	RecreatedEquation = recreateDAGFromEqs(A, D, VarNames, LocalMod);
  

  return RecreatedEquation;

}


// Functions for Bit-blasting with scope reduction


// Given a set of constraints and a set of variables to be eliminated,
// this function calls bit-blasting + QE at bit-level with scope reduction 
// as well as QE_LMC (APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG), and
// returns the result from APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG
DAGNode* QEliminatorForMultipleVariablesUsingLMAandBitBlastingwithScopeReductionReturningDAG(set<DAGNode*> Constraints, set<string> VarsToElim, map<string, int> &WidthTable)
{

  //cout<<"\nQEliminatorForMultipleVariablesUsingLMAandBitBlastingwithScopeReductionReturningDAG called\n";

  while(VarsToElim.size()!=0)
  {

    //showSet(VarsToElim,"VarsToElim");
    //showAPSet("Constraints", Constraints);

  map<DAGNode*, set<string> > Constraint_VarsToElim_Map;
  map<string, set<DAGNode*> > VarToElim_Constraints_Map;
  map<set<DAGNode*>, set<string> > Constraints_VarsToElim_Map;

  // Create Constraint_VarsToElim_Map
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;
     set<string> Support;
     getDAGNames(Constraint, Support); // Support of Constraint obtained
     set<string> VarsToElimInSupport;
     set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));
     
     Constraint_VarsToElim_Map.insert(make_pair(Constraint, VarsToElimInSupport));
    }

  // Showing Constraint_VarsToElim_Map

//   cout<<"\nConstraint_VarsToElim_Map\n";

//   for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
//     {
//      DAGNode *Constraint = it->first;
//      set<string> Support = it->second;
//      cout<<"\nConstraint = "<<getDAGStr(Constraint)<<"\t";
//      showSet(Support, "Support");
//    }
   
  // Create VarToElim_Constraints_Map

  bool Constraints_Free_Of_VarsToElim = true;

  for(map<DAGNode*, set<string> >::iterator it=Constraint_VarsToElim_Map.begin(); it!=Constraint_VarsToElim_Map.end(); it++)
    {
     DAGNode *Constraint = it->first;
     set<string> Support = it->second;

     if(!Support.empty())
	{
	Constraints_Free_Of_VarsToElim = false;
	}

     for(set<string>::iterator it2=Support.begin(); it2!=Support.end(); it2++)
    	{
	string Variable = *it2;
	map<string, set<DAGNode*> >::iterator vcm_it = VarToElim_Constraints_Map.find(Variable);
	if(vcm_it != VarToElim_Constraints_Map.end())
		{
		(vcm_it->second).insert(Constraint);
		}
	else
		{
		set<DAGNode*> Constraints;
		Constraints.insert(Constraint);
		VarToElim_Constraints_Map.insert(make_pair(Variable, Constraints));
		}
   	}  
     }

  if(Constraints_Free_Of_VarsToElim) // All constraints free of VarsToElim
	{
	break;
	}

  // Showing VarToElim_Constraints_Map

//   cout<<"\nVarToElim_Constraints_Map\n";

//   for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
//     {
//      string VarToElim = it->first;
//      set<DAGNode*> Constraints = it->second;
//      cout<<"\nVarToElim = "<<VarToElim<<"\t";
//      showAPSet("Constraints", Constraints);
//    }

  // Create Constraints_VarsToElim_Map

  for(map<string, set<DAGNode*> >::iterator it=VarToElim_Constraints_Map.begin(); it!=VarToElim_Constraints_Map.end(); it++)
    {
     string VarToElim = it->first;
     set<DAGNode*> Constraints = it->second;
     map<set<DAGNode*>, set<string> >::iterator cvm_it = Constraints_VarsToElim_Map.find(Constraints);
     if(cvm_it != Constraints_VarsToElim_Map.end())
	{
	(cvm_it->second).insert(VarToElim);
	}
    else	
	{
	set<string> Support;
	Support.insert(VarToElim);
	Constraints_VarsToElim_Map.insert(make_pair(Constraints, Support));
	}
    }

  // Showing Constraints_VarsToElim_Map

//   cout<<"\nConstraints_VarsToElim_Map\n";

//   for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
//     {
//      set<DAGNode*> Constraints = it->first;
//      set<string>  VarsToElimInConstraints = it->second;
//      showAPSet("Constraints", Constraints);
//      showSet(VarsToElimInConstraints, "VarsToElimInConstraints");
//    }

  // selecting the entry in Constraints_VarsToElim_Map with the minimum sized LHS i.e. Constraints

  map<set<DAGNode*>, set<string> >::iterator selected_it=Constraints_VarsToElim_Map.begin(); 
  int size_of_constraints = (selected_it->first).size();

  for(map<set<DAGNode*>, set<string> >::iterator it=Constraints_VarsToElim_Map.begin(); it!=Constraints_VarsToElim_Map.end(); it++)
    {
    if( (it->first).size() < size_of_constraints)
	{
	selected_it = it;
	size_of_constraints = (it->first).size();
	}
    }
  
  // selected_it is the entry in Constraints_VarsToElim_Map with the minimum sized LHS i.e. Constraints

  set<string> VariablesSelectedForElimination = selected_it->second;
  set<DAGNode*> ConstraintsSelected = selected_it->first;
  set<DAGNode*> RemainingConstraints;
  set<DAGNode*> QEliminated_Constraints;

  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;
     if(ConstraintsSelected.find(Constraint)==ConstraintsSelected.end()) // Constraint is not ConstraintsSelected
	{
	RemainingConstraints.insert(Constraint);
	}
   }

//   showAPSet("ConstraintsSelected", ConstraintsSelected);
//   showAPSet("RemainingConstraints", RemainingConstraints);
  
  set<DAGNode*> Temp_Set; // only required to satisfy the fact that APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG requires a set as second argument. APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG unions first and second sets to get a set say 'S' and does qunatifier elimination on this. Hence, it is fine to keep first argument as Constraints_Set and second argument as Temp_Set (which is empty)

   DAGNode* QEliminated_Constraint = NULL;

   if(ConstraintsSelected.size()>0)
	{
	QEliminated_Constraint = quickSimplify(APResolveModifiedWithMultipleVariablesUsingLMAReturningDAG(ConstraintsSelected, Temp_Set, VariablesSelectedForElimination, WidthTable)); // Calls elimination by LMA
         
        DAGNode *TempDAG = callBitBlastingReturningDAG(ConstraintsSelected, Temp_Set, VariablesSelectedForElimination, WidthTable); // Calls elimination by Bit-blasting
	}

   if(QEliminated_Constraint != NULL && !isTrue(QEliminated_Constraint) ) // stands for true
	{
	  //cout<<"\nQEliminated_Constraint = "<<getDAGStr(QEliminated_Constraint)<<"\n";
	  splitConjunctionIntoSet(QEliminated_Constraint, QEliminated_Constraints);
	  //showAPSet("QEliminated_Constraints", QEliminated_Constraints);
	}

    Constraints.clear();
    set_union(RemainingConstraints.begin(), RemainingConstraints.end(), QEliminated_Constraints.begin(), QEliminated_Constraints.end(),inserter(Constraints, Constraints.begin())); 
     // Constraints recreated

    set<string> RemainingVariables;
    set_difference(VarsToElim.begin(), VarsToElim.end(), VariablesSelectedForElimination.begin(), VariablesSelectedForElimination.end(),inserter(RemainingVariables, RemainingVariables.begin()));
    VarsToElim = RemainingVariables;
   // VarsToElim recreated
  }// while(VarsToElim.size()!=0)

  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
  
    if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    return resultDAG;
}

// Given a set of LMEs and LMDs, and a set of variables to be eliminated, 
// perform QE using GD layer + bit-blasting + QE at bit-level. return value is always TRUE dag node
// This effectively implements BV_Solve in CAV'11 paper (But note that the result is always TRUE,
// hence this function was used only in the initial performance measurement experiments. The 
// function which was actually used in the CAV'11 experiments to implement BV_Solve is 
// DDManager::APResolveModifiedWithMultipleVariablesUsingLMAWithBlasting)
DAGNode* callBitBlastingReturningDAG(set<DAGNode*> E, set<DAGNode*> D, set<string> VarsToElim, map<string, int> &WidthTable)
{
 
  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();

  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
     names.insert(*it); 
    }

  attachSetToList(names, VarNames);
  NoVarsToElim=VarsToElim.size();

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=E.begin(); it!=E.end(); it++)
    getDAGNames(*it, names);

  for(set<DAGNode*>::iterator it=D.begin(); it!=D.end(); it++)
    getDAGNames(*it, names);

  attachSetToList(names, VarNames);

  NoVars=VarNames.size();

  // Now we have the list of variables,  NoVars, NoVarsToElim

  set<DAGNode*> Constraints;

  set_union(E.begin(), E.end(), D.begin(), D.end(),inserter(Constraints, Constraints.begin()));



  // Get the equations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;

  time_t start, end, duration;
  struct timeval start_ms, finish_ms;
  unsigned long long int duration_ms;

  time(&start);
  gettimeofday (&start_ms, NULL);

  resultDAG = qEliminatorReturningDAGOnlyForOddCoefficientsFollowedByBitBlasting(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);

  gettimeofday (&finish_ms, NULL);

  time(&end);
	  
  duration = end - start;

  Total_Blasting_Time = Total_Blasting_Time + duration;

  duration_ms = finish_ms.tv_sec * 1000 + finish_ms.tv_usec / 1000;
  duration_ms -= start_ms.tv_sec * 1000 + start_ms.tv_usec / 1000;

  //cout<<"\nBit-blasting duration = "<<duration_ms<<endl;
	  	  
  Total_Blasting_Time_In_ms = Total_Blasting_Time_In_ms  + duration_ms;
  


  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }


 return resultDAG;

}


// Added on 8 Jan 2011 for LMDD:BDD

// Given a conjunction of LMCs as matrices, this substitutes the occurances of the odd variables
// to be eliminated using Ganesh & Dill's work.
// The result has two parts Phi1 which is free of variables to be eliminated, and
// Phi2 which contains the variables to be eliminated
// Phi1 is returned, and Phi2 is a parameter
DAGNode* qEliminatorReturningDAGOnlyForOddCoefficients(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable, DAGNode* &Phi2)
{
  
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  //cout<<"\nShowing original set of constraints\n";
  //showEquations(EqCoefMatPassed, EqConstMatPassed); // Show the equations
  //showDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed); // Show the disequations
  //cout<<"\nWe need to eliminate the first "<<NoVarsToElim<<" variables \n";


  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  
  //cout<<"\nShowing duplicate eliminated set of constraints\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

  //cout<<"\nNo: of vars to elim = "<<NoVarsToElim<<endl;

  DAGNode *Phi1=NULL;
  Phi2=NULL;

  if(!EqCoefMat.empty())
    qEliminatorForEquationsAndDisequationsOnlyForOddCoefficients(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod); // Eliminate first NoVarsToElim variables from the disequations CX<>D and equations AX=B and get back the matrices

  // This function finds the disequations in which there are no variables left to eliminate, removes them from set of disequations and copies them in a matrices DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat
  
  Matrix DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat;
  Matrix EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat;
  
  if(!DisEqCoefMat.empty()) eliminateDisequationsFreeOfAllVarsToElimAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, NoVarsToElim, DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat);

  if(!EqCoefMat.empty()) eliminateEquationsFreeOfAllVarsToElimAndCopyInNewMatrix(EqCoefMat, EqConstMat, NoVarsToElim, EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat);

  
  DAGNode *DagFromFreeEqs, *DagFromFreeDiseqs;
  DagFromFreeEqs = getDAGFromEqs(EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat, VarNames, Mod);
  DagFromFreeDiseqs = getDAGFromDisEqs(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat, VarNames, Mod);
  	
    if(DagFromFreeEqs != NULL)
	{
	if(Phi1 == NULL) Phi1=DagFromFreeEqs; 
	else 
	  { 
	    Phi1 = createDAG("and",Phi1, DagFromFreeEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromFreeDiseqs != NULL)
	{
      	if(Phi1 == NULL) Phi1=DagFromFreeDiseqs; 
	else 
	  {
	    Phi1 = createDAG("and",Phi1, DagFromFreeDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}

    if(Phi1 == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      Phi1 = dm.createNode("true",(void*)v);
	    }
  
  //cout<<"\n###############################\nPrinting the Equalities containing any variable to eliminate\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nPrinting the Disequalities containing any variable to eliminate\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  //cout<<"\n###############################\n";
 
  
   DAGNode *DagFromEqs, *DagFromDiseqs;
   DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, VarNames, Mod);
   DagFromDiseqs = getDAGFromDisEqs(DisEqCoefMat, DisEqConstMat, VarNames, Mod);
  	
    if(DagFromEqs != NULL)
	{
	if(Phi2 == NULL) Phi2=DagFromEqs; 
	else 
	  { 
	    Phi2 = createDAG("and",Phi2, DagFromEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromDiseqs != NULL)
	{
      	if(Phi2 == NULL) Phi2=DagFromDiseqs; 
	else 
	  {
	    Phi2 = createDAG("and",Phi2, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}

    if(Phi2 == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      Phi2 = dm.createNode("true",(void*)v);
	    }

    return Phi1;
 
}

// Added on 8 Jan 2011 for LMDD:BDD ends here


/* Functions for All path QElim with GD layer + Blasting start here */

// Same as qEliminatorReturningDAGOnlyForOddCoefficients except that the result is 
// returned as Phi1 and Phi2
DAGNode* qEliminatorReturningDAGOnlyForOddCoefficientsReturningSingleDAG(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> VarNames, map<string, int> WidthTable)
{
  
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  //cout<<"\nShowing original set of constraints\n";
  //showEquations(EqCoefMatPassed, EqConstMatPassed); // Show the equations
  //showDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed); // Show the disequations
  //cout<<"\nWe need to eliminate the first "<<NoVarsToElim<<" variables \n";


  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  
  //cout<<"\nShowing duplicate eliminated set of constraints\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

  //cout<<"\nNo: of vars to elim = "<<NoVarsToElim<<endl;

  DAGNode *Phi1=NULL;
  DAGNode *Phi2=NULL;

  if(!EqCoefMat.empty())
    qEliminatorForEquationsAndDisequationsOnlyForOddCoefficients(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod); // Eliminate first NoVarsToElim variables from the disequations CX<>D and equations AX=B and get back the matrices

  // This function finds the disequations in which there are no variables left to eliminate, removes them from set of disequations and copies them in a matrices DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat
  
  Matrix DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat;
  Matrix EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat;
  
  if(!DisEqCoefMat.empty()) eliminateDisequationsFreeOfAllVarsToElimAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, NoVarsToElim, DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat);

  if(!EqCoefMat.empty()) eliminateEquationsFreeOfAllVarsToElimAndCopyInNewMatrix(EqCoefMat, EqConstMat, NoVarsToElim, EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat);

  
  DAGNode *DagFromFreeEqs, *DagFromFreeDiseqs;
  DagFromFreeEqs = getDAGFromEqs(EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat, VarNames, Mod);
  DagFromFreeDiseqs = getDAGFromDisEqs(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat, VarNames, Mod);
  	
    if(DagFromFreeEqs != NULL)
	{
	if(Phi1 == NULL) Phi1=DagFromFreeEqs; 
	else 
	  { 
	    Phi1 = createDAG("and",Phi1, DagFromFreeEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromFreeDiseqs != NULL)
	{
      	if(Phi1 == NULL) Phi1=DagFromFreeDiseqs; 
	else 
	  {
	    Phi1 = createDAG("and",Phi1, DagFromFreeDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}

    if(Phi1 == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      Phi1 = dm.createNode("true",(void*)v);
	    }
  
  //cout<<"\n###############################\nPrinting the Equalities containing any variable to eliminate\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nPrinting the Disequalities containing any variable to eliminate\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  //cout<<"\n###############################\n";
 
  
   DAGNode *DagFromEqs, *DagFromDiseqs;
   DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, VarNames, Mod);
   DagFromDiseqs = getDAGFromDisEqs(DisEqCoefMat, DisEqConstMat, VarNames, Mod);
  	
    if(DagFromEqs != NULL)
	{
	if(Phi2 == NULL) Phi2=DagFromEqs; 
	else 
	  { 
	    Phi2 = createDAG("and",Phi2, DagFromEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromDiseqs != NULL)
	{
      	if(Phi2 == NULL) Phi2=DagFromDiseqs; 
	else 
	  {
	    Phi2 = createDAG("and",Phi2, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}

    if(Phi2 == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      Phi2 = dm.createNode("true",(void*)v);
	    }

    

   Phi1 = quickSimplify(Phi1);
   Phi2 = quickSimplify(Phi2);

   //cout<<"\nPhi1 = "<<getDAGStr(Phi1)<<endl;
   //cout<<"\nPhi2 = "<<getDAGStr(Phi2)<<endl;

   DAGNode *Phi; // final result


   if(isTrue(Phi1)) // Phi1 is true. Phi = Phi2
	{
	Phi = Phi2;
	}
   else if(isTrue(Phi2)) // Phi2 is true. Phi = Phi1
	{
	Phi = Phi1;
	}
   else // Phi = Phi1 /\ Phi2
	{
	Phi = createDAG("and",Phi1, Phi2, LOGICAL_SYMBOL, 1);
	} 

   //cout<<"\nPhi = "<<getDAGStr(Phi)<<endl;

   return Phi;
 
}

// Given a set of constraints, converts them into matrices and 
// qEliminatorReturningDAGOnlyForOddCoefficientsReturningSingleDAG on the matrices
// returns the result from qEliminatorReturningDAGOnlyForOddCoefficientsReturningSingleDAG
DAGNode* callMatrixQEliminatorForGDLayer(set<DAGNode*> &Constraints, string VariableToElim, map<string, int> &WidthTable)
{
  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();
  names.insert(VariableToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    getDAGNames(*it, names);


  attachSetToList(names, VarNames);

  NoVars=VarNames.size();


  // Get the equations and disequations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;

  resultDAG = qEliminatorReturningDAGOnlyForOddCoefficientsReturningSingleDAG(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable);

  return resultDAG;
}


// Given a set of constraints and a set of variables to be eliminated, this function applies
// scope reduction and callMatrixQEliminatorForGDLayer to each variable
DAGNode* APResolveUsingGDLayerWithMultipleVariablesUsingLMAReturningDAG(set<DAGNode*> Constraints, set<string> VarsToElim, map<string, int> &WidthTable)
{

    if(VarsToElim.size()>0)
    {
      update_Multivar_APQElim_Calls();
    }
  
  //cout<<"\nAPResolveModifiedWithMultipleVariablesUsingLMAReturningDAG called\n";

  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  DAGNode* QElim_Bound_Constraint;

  //showSet(VarsToElim,"VarsToElim");

  //showAPSet("Constraints", Constraints);



  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;

      //cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints);

      QElim_Bound_Constraints.clear();
      QElim_Bound_Constraint = NULL; 
  
        
       if(QElim_Bound_Constraint == NULL)
	      {
      	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      QElim_Bound_Constraint = dm.createNode("true",(void*)v);
	      }

      if(Bound_Constraints.size()>0)
	{
	  QElim_Bound_Constraint = callMatrixQEliminatorForGDLayer(Bound_Constraints, VariableToElim, WidthTable);
	}

      if(!isTrue(QElim_Bound_Constraint)) 
	{
	  //cout<<"\nQElim_Bound_Constraint = "<<getDAGStr(QElim_Bound_Constraint)<<"\n";
	  splitConjunctionIntoSet(QElim_Bound_Constraint, QElim_Bound_Constraints);
	}

      //showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      //showAPSet("Constraints", Constraints);
    }

  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
  
    if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

    return resultDAG;
}


// Print the formula rooted at "root" in SMTLIB format in the file file name with 
// special handling of labels of root nodes. Labels of root nodes are printed as
// abstraction_"root_number_str"
void printOutputOfQEliminatorInSMTLibFormatInGivenFile(DAGNode *root, map<string, int> &WidthTable, string file_name, int root_number)
{
  cout<<"\nPrinting in "<<file_name<<endl;

  FILE *smt_file = fopen(file_name.c_str(), "a+");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<file_name<<"\n"; exit(1);}

  fprintf(smt_file,"\n\n");


  set<string> Vars;

  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {
	Vars.insert(it->first);
	}

  writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, WidthTable, Vars);

  string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);
  
  char root_number_char[100];

  sprintf(root_number_char,"%d", root_number);

  string root_number_str(root_number_char);

  string pred_decl = ":extrapreds ((abstraction_";
  pred_decl += root_number_str;
  pred_decl += "))";

  string assum_str = ":assumption (iff abstraction_";
  assum_str += root_number_str;
  assum_str += " ";
  assum_str += dag_str;
  assum_str += ")";

  fprintf(smt_file, "\n%s\n", pred_decl.c_str());
  fprintf(smt_file, "\n%s\n", assum_str.c_str());

  //fprintf(smt_file, "\n)\n");
  fclose(smt_file);

  cout<<"\nPrinted in "<<file_name<<endl;


  }

// Same as printOutputOfQEliminatorInSMTLibFormatInGivenFile except that 
// only a selected set of variables are declared
void printOutputOfQEliminatorInSMTLibFormatInGivenFileForNonZeroFrame(DAGNode *root, map<string, int> &WidthTable, string file_name, int root_number)
{
  cout<<"\nPrinting in "<<file_name<<endl;

  FILE *smt_file = fopen(file_name.c_str(), "a+");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<file_name<<"\n"; exit(1);}

  fprintf(smt_file,"\n\n");

  set<string> Vars;

  map<string, int>::iterator it;
  for(it=WidthTable.begin(); it!=WidthTable.end(); it++)
        {
	string variable = it->first;
	int index_of_uscore = variable.find_last_of("_");
	string variable_name_no_uscore = variable.substr(0, index_of_uscore);
	string  delta_str = variable.substr(index_of_uscore+1);
	int delta = atoi(delta_str.c_str());

	if(delta != root_number)
		{
		Vars.insert(it->first);
		}
	}


  writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, WidthTable, Vars);

  string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);
  
  char root_number_char[100];

  sprintf(root_number_char,"%d", root_number);

  string root_number_str(root_number_char);

  string pred_decl = ":extrapreds ((abstraction_";
  pred_decl += root_number_str;
  pred_decl += "))";

  string assum_str = ":assumption (iff abstraction_";
  assum_str += root_number_str;
  assum_str += " ";
  assum_str += dag_str;
  assum_str += ")";

  fprintf(smt_file, "\n%s\n", pred_decl.c_str());
  fprintf(smt_file, "\n%s\n", assum_str.c_str());

  //fprintf(smt_file, "\n)\n");
  fclose(smt_file);

  cout<<"\nPrinted in "<<file_name<<endl;


  }



/* Functions for All path QElim with GD layer + Blasting end here */

// Given a set of LMCs in "Constraints" and a set of variables to be eliminated,
// create conjunctions of constraints of the same bit-width and give to the function 
// createConjunctionAndPrintInSMTLibFormat
void createConjunctionsAccordingToModAndPrintInSMTLibFormat(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
set<int> BitWidthsInTheProblem;
for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
	{
	string var_to_elim = *it;
      	map<string, int>::iterator wit = WidthTable.find(var_to_elim);
      	if(wit == WidthTable.end())
	{
	  cout<<"\nVariable "<<var_to_elim<<" has no entry in WidthTable in function createConjunctionsAccordingToModAndPrintInSMTLibFormat in Eliminator.cpp\n";
	  exit(1);
	}
	BitWidthsInTheProblem.insert(wit->second);
	}

for(set<int>::iterator it=BitWidthsInTheProblem.begin(); it != BitWidthsInTheProblem.end(); it++)
	{
        int bitwidth = *it;
        cout<<"\nBit-Width = "<<*it<<endl;
	
	vector<DAGNode*> MyConstraints;
	for(int i=0;i<Constraints.size(); i++)
    		{
		DAGNode *Constraint = Constraints[i];
		int NOfBitsInConstraint=getNoOfBitsInConstraint(Constraint, WidthTable);
		if(NOfBitsInConstraint == bitwidth)
			{
			MyConstraints.push_back(Constraint);
			}     		
    		}

	set<string> MyVarsToElim;
	for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
		{
		string var_to_elim = *it;
      		map<string, int>::iterator wit = WidthTable.find(var_to_elim);
      		if(wit == WidthTable.end())
			{
	  		cout<<"\nVariable "<<var_to_elim<<" has no entry in WidthTable in function createConjunctionsAccordingToModAndPrintInSMTLibFormat in Eliminator.cpp\n";
	  		exit(1);
			}
		if(wit->second == bitwidth)
			{
			MyVarsToElim.insert(var_to_elim);
			}   
		}
	createConjunctionAndPrintInSMTLibFormat(MyConstraints, MyVarsToElim, WidthTable);
	}	

}

// create conjunction of the given set of constraints and print in SMTLIB format
void createConjunctionAndPrintInSMTLibFormat(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
bool print_input_in_smtlib_format = true;
if(print_input_in_smtlib_format)
		{
		DAGNode *resultDAG = NULL;
		
		// initialDAG is the conjunction of the constraints in Constraints
		for(int i=0;i<Constraints.size(); i++)
    		{
		DAGNode *Constraint = Constraints[i];
      		if(resultDAG==NULL)
			{
	  		resultDAG = Constraint;
			}
      		else
			{
	  		resultDAG = createDAG("and",resultDAG, Constraint, LOGICAL_SYMBOL, 1);
			}
    		}

		if(resultDAG == NULL)
			{
			return;
			}

                set<string> Support;
		getDAGNames(resultDAG, Support);
		set<string> CommonVars;

		 set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(CommonVars, CommonVars.begin()));

		if(CommonVars.empty())
			{
			return;
			}		

		//showSet(CommonVars, "CommonVars");
		static int file_counter = 1;
  		char file_counter_char[100];
		sprintf(file_counter_char, "%d", file_counter);
		string file_counter_str(file_counter_char);


		int index = InFileName.find_last_of(".");
		if(index==string::npos)
			{
			cout<<"\nInFileName with no extension in createConjunctionAndPrintInSMTLibFormat in Eliminator.cpp\n";
			exit(1);
			}

		string InFileNameNoExtension = InFileName.substr(0, index);

		string Elimination_Type_Str;

                if(strcmp(Elimination_Type, "-black_blasting")==0)
			{
			Elimination_Type_Str = "black_blasting";
			}
		else
			{
			if(strcmp(Dag_Eliminator_Type, "-dd")==0)
				{

					if(ConvertProblemIntoDisjunctionOfTailedTriangles)
					{
						Elimination_Type_Str = "QECOMBINED";
					}
					else
					{
						Elimination_Type_Str = "QELMDD";
					}
				}
			else
				{
				Elimination_Type_Str = "QESMT";
				}
			}
				



		string file_name = InFileNameNoExtension;
		file_name += "_";
		file_name += Elimination_Type_Str;
		file_name += "_benchmark_conjunction_";
		file_name += file_counter_str;
		file_name += ".smt";
		printInputToQEliminatorInSMTLibFormatInGivenFile(resultDAG, WidthTable, CommonVars, file_name);
		file_counter++;
		}
}


// Function to get QE_LMC details when QE_LMC is directly called by the user
// This function is called by the function DDManager::QELIM_Conjunction_level
void getAP_QElimDetailsAtDAGLevel_For_QELIM_Conjunctions(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, unsigned int &number_of_lmes, unsigned int &number_of_lmds, unsigned int &number_of_vars, unsigned int &number_of_vars_to_elim, unsigned int &number_of_bits)
{
  //showAPSet("Constraints", Constraints);
  //showSet(VarsToElim,"VarsToElim");

  set<DAGNode*> LMEs;
  set<DAGNode*> LMDs;
  
  seggregateIntoLMEsAndLMDs(Constraints, LMEs, LMDs, WidthTable);

  //showAPSet("LMEs", LMEs);
  //showAPSet("LMDs", LMDs);

  number_of_lmes = LMEs.size();
  number_of_lmds = LMDs.size();

  set<string> Support;
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;
     set<string> Support_Constraint;
     getDAGNames(Constraint, Support_Constraint);
     set<string> NewSet;
     set_union(Support_Constraint.begin(), Support_Constraint.end(), Support.begin(), Support.end(),inserter(NewSet, NewSet.begin())); 
     Support = NewSet;
   }
  number_of_vars = Support.size();

  set<string> VarsToElimInSupport;
  set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(VarsToElimInSupport, VarsToElimInSupport.begin()));
  number_of_vars_to_elim = VarsToElimInSupport.size();


  number_of_bits = 0;
  for(set<string>::iterator it=VarsToElimInSupport.begin(); it!=VarsToElimInSupport.end(); it++)
    {
      string var_to_elim = *it;
      map<string, int>::iterator wit = WidthTable.find(var_to_elim);
      if(wit == WidthTable.end())
	{
	  cout<<"\nVariable "<<var_to_elim<<" has no entry in WidthTable in function getAP_QElimDetailsAtDAGLevel in DAGEliminator.cpp\n";
	  exit(1);
	}
      number_of_bits += wit->second;
    }

  // number_of_lmes, number_of_lmds, number_of_vars, number_of_vars_to_elim, number_of_bits found

  cout<<"\nnumber_of_lmes = "<<number_of_lmes<<", number_of_lmds = "<<number_of_lmds<<", number_of_vars = "<<number_of_vars<<", number_of_vars_to_elim = "<<number_of_vars_to_elim<<", number_of_bits = "<<number_of_bits<<endl;
  
}

// Functions for dag evaluation and unsat. core finding start here

// ADDED ON 23-2-2011

// evaluateFunction can be used to evaluate dags which are in the following form
// dag ::= ite(dag1, dag, dag) | bit-vector | dag1
// dag1 ::= dag1 /\ dag1 | dag1 \/ dag1 | !dag1 | lmde
// Thus the result is always a bit-vector (returned as a string)
// This calls evaluateFunction_Internal which gets the result as bvatom.
// The result is converted to string by this function
string evaluateFunction(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable)
{
  // make node point to the RHS of the function

  bool evaluate_only_rhs = false;

  if(evaluate_only_rhs)
    {
      if(node->Get_node_name() == "="  || node->Get_node_name() == "equivalent")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  iout++;
	  node = *iout;
	}
      else
	{
	  cout<<"\nArgument dag is not a function in evaluateFunction in Eliminator.cpp\n";
	  exit(1);
	}
    }


  map<DAGNode*, bvatom> DPTable;
  DPTable.clear();
  bvatom ret_value = evaluateFunction_Internal(node, Variable_Value_Map, MyWidthTable, DPTable);
  // printing the result
  cout<<"\nret_value = ";
  bvm->printBVatom(ret_value, 0);
  // convert ret_bv_value from bvatom to string
  string hex_str = bvm->convertBVToHexString(ret_value);
  cout<<"\nhex_str = "<<hex_str<<endl;
  ULINT dec_value = hexStringToULINT(hex_str); // decimal value of the hex string
  cout<<"\ndec_value = "<<dec_value<<endl;
  string ret_value_str = integerToBinaryString(dec_value); // binary value with minimum required width
  cout<<"\nret_value_str = "<<ret_value_str<<endl;
  return ret_value_str;
}

// Called by evaluateFunction as explained above
bvatom evaluateFunction_Internal(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, map<DAGNode*, bvatom> &DPTable)
{

  map<DAGNode*, bvatom> ::iterator dpit = DPTable.find(node);
 
  if(dpit!=DPTable.end()) // node already encountered
    {
      //cout<<"\nDPTable Hit\n";
      return dpit->second;
    }

  else { // encountered first time

    bvatom ret_value;
    string dag_str;
    int outlist_size;

    dag_str = node->Get_node_name();
    outlist_size = node->Get_outlist_size();
    Value* MyValue = (Value*)(node->getValue());
    Type MyType = MyValue->get_node_type();
    int MyWidth = MyType.width;
    int MyDataType = MyType.data_type;
    
     cout<<"\n"<<dag_str<<" node encountered in evaluateFunction_Internal\n";

    if(dag_str == "=" || dag_str == "is_not_equal" || dag_str == "not_equal_to" || dag_str == "equivalent") // LME/LMD
      {
	ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
      }
    else if(outlist_size != 0)// Internal node
      {
      
	if(dag_str == "and" && outlist_size  == 2) // binary and
	  {
	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bvatom child2 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);

	    ret_value = bvm->bitwise_and(child1, child2);

	  }//binary and
      
	else if(dag_str == "or" && outlist_size  == 2) // binary or
	  {
	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bvatom child2 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 

	    ret_value = bvm->bitwise_or(child1, child2);

	  }// binary or

	else if(dag_str == "ite" || dag_str == "if_then_else")
	  {
	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    iout++;

	    bvatom child2 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 

	    iout++;

	    bvatom child3 = evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable); 


	    bvatom true_value = bvm->getZeroedBVatom(1);
	    true_value.bv_array[0] = (bv_block_t) 1;


	    if(bvm->checkBVEquality(child1, true_value))// child1 is true
	      { 
		ret_value = child2;
	      }
	
	    else 
	      { 
		ret_value = child3;
	      }
	  
	  }// ite


	else if(dag_str == "not")
	  {
	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  evaluateFunction_Internal(*iout, Variable_Value_Map, MyWidthTable, DPTable);
	  
	    ret_value = bvm->bitwise_negate(child1);
	    
	  }// not
	else if(dag_str == "bvmul" || dag_str == "bvadd")
	  {
	    ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
	  }// * and +
	else if(dag_str == "select")
	  {
	    ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
	  }// * and +

	else	  
	  {
	    cout<<"\nUnknown operator "<<dag_str<<" in function evaluateFunction_Internal in Eliminator.cpp\n";
	    exit(1);
	  }
       

      }// internal node

    else // constants and variables
      {
	if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT) //i.e. if variable
	  {
	    ret_value = call_lmdeEvaluatorReturningBV(node, Variable_Value_Map, MyWidthTable);
	    
	  } //variable node ends here

	else // constants
	  {
	
	  ULINT dec_value = strtoull(dag_str.c_str(), NULL, 0); // decimal value of the constant obtained from the dag node
	  string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
	  bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

	  if(bin_value.size() < MyWidth) // bin_value needs to be zero-extended to match MyWidth
	    {
	      ret_value = bvm->BVExtend(result, MyWidth, 0);
	    }
	  else if(bin_value.size() > MyWidth) // bin_value needs to be truncated to match width. Hence select from 0 to MyWidth-1
	    {
	      ret_value = bvm->select_with_int(result, MyWidth-1, 0);
	    }
	  else
	    {
	      ret_value = result;
	    }

	  }//constants
      }//constants and variables

    DPTable.insert(make_pair(node, ret_value));

    cout<<"\nret_value from "<<dag_str<<" for evaluateFunction_Internal = ";
    bvm->printBVatom(ret_value, 0);

    return ret_value;
  }// DPTable miss

}// Function ends 


// Function which calls lmdeEvaluatorModified to evaluate an LMDE
bvatom call_lmdeEvaluatorReturningBV(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable)
{
  cout<<"\ncall_lmdeEvaluatorReturningBV called\n";
  
  if(node == NULL)
    {
     cout<<"Function call_lmdeEvaluatorReturningBV in Eliminator.cpp called with NULL node\n";
     exit(1);
    } 

  int width;

  bool scheme_with_selections = true;

  // find width
  if(scheme_with_selections)
    {
      width = getWidthOfLMDEWithSelects(node, MyWidthTable);

      cout<<"\nwidth = "<<width<<endl;
    }
  else
    {
      string variable = "";
      variable = getSingleVariable(node);
      if(variable=="")
	{
	  cout<<"\nLME/LMD "<<getDAGStr(node)<<" encountered with no variable names in function call_lmdeEvaluatorReturningBV in Eliminator.cpp\n";
	  exit(1);
	}
  
      map<string, int>::iterator MyWidthTable_it = MyWidthTable.find(variable);
      if(MyWidthTable_it == MyWidthTable.end())
	{
	  cout<<"\nNo entry in MyWidthTable for variable "<<variable<<" in function call_lmdeEvaluatorReturningBV in Eliminator.cpp\n";
	  exit(1);
	}
      width = MyWidthTable_it->second;
    }

  bool boolean_result;

  bvatom bv_result = lmdeEvaluatorModified(node, width, Variable_Value_Map, boolean_result);

  cout<<"\ncall_lmdeEvaluatorReturningBV finished\n";

  return bv_result; 
}


// lmdeEvaluatorModified is lmdeEvaluatorWithBinaryModel except that, 
// it handles simple selections of the form variable[integer:integer]
bvatom lmdeEvaluatorModified(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result)
    {
      bvatom bv_result;
      string dag_str;
      int outlist_size;

      dag_str = node->Get_node_name();
      outlist_size = node->Get_outlist_size();
      Value* MyValue = (Value*)(node->getValue());
      Type MyType = MyValue->get_node_type();
      int MyWidth = MyType.width;
      int MyDataType = MyType.data_type;

      cout<<"\n"<<dag_str<<" node encountered in lmdeEvaluatorModified\n";


      if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && outlist_size==0)  //i.e. if a variable node
	{
	  map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(dag_str);
	  if(Variable_Value_Map_it == Variable_Value_Map.end())
	    {
	      cout<<"\nNo entry in Variable_Value_Map for variable "<<dag_str<<" in function lmdeEvaluatorModified in Eliminator.cpp\n";
	      exit(1);
	    }
	  string bin_value = Variable_Value_Map_it->second; // binary value of the variable from map 

	  bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()

	  //cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

	  if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	    {
	      //cout<<"\nbin_value.size() < width\n";
	      bv_result = bvm->BVExtend(result, width, 0);
	    }
	  else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	    {
	      //cout<<"\nbin_value.size() < width\n";
	      bv_result = bvm->select_with_int(result, width-1, 0);
	    }
	  else
	    {
	      //cout<<"\nbin_value.size() = width\n";
	      bv_result = result;
	    }


	  //cout<<"\nbv_result = "<<endl;
	  //bvm->printBVatom(bv_result, 0);

	} //operand node ends here

      else   //i.e. if an operator node or a constant node
        {
            list<DAGNode*>::iterator iout = node->Get_outlist_start();
	    
	    if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
		{
		  bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained
		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained
		  if (inter_bool_result) { // reverse the result as it is is_not_equal
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		    }
		}// "is_not_equal"/"not_equal_to" ends here

	      else if(dag_str == "equivalent" || dag_str == "=")
		{
		  bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

		  if (inter_bool_result) { // no need to reverse
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  

		  }
		}// "equivalent"/"=" ends here

	      else if(dag_str == "bvmul")
		{
		  if((*iout)->Get_node_name()=="1")// multiplication by 1
		    {
		      //cout<<"\nmultiplication by 1\n";
		      iout++;
		      bv_result = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained - no need to select. op2 is already of proper width
		    }
		  else
		    {

		      bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		      iout++;
		      bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

		      bvatom result = bvm->arith_mult(op1, op2); // result obtained
		      bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
		    } 
		}// "bvmul" ends here

	    else if(dag_str == "bvadd")
		{
		  bvatom op1 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified(*iout, width, Variable_Value_Map, boolean_result); // op2 obtained

		  bvatom result = bvm->arith_add(op1, op2); // result obtained
		  bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
 
		}// "bvadd" ends here
	    else if(dag_str == "select")
	      {
		DAGNode *operand = *iout;
		iout++;
		DAGNode *low_sel = *iout;
		iout++;
		DAGNode *high_sel = *iout;


		Value* OperandValue = (Value*)(operand->getValue());
		Type OperandType = OperandValue->get_node_type();
		int OperandWidth = OperandType.width;
		int OperandDataType = OperandType.data_type;

		
		string operand_string = operand->Get_node_name();
		string low_sel_string = low_sel->Get_node_name();
		string high_sel_string = high_sel->Get_node_name();

		int low_sel_number = atoi(low_sel_string.c_str());
		int high_sel_number = atoi(high_sel_string.c_str());
      
		map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(operand_string);
		if(Variable_Value_Map_it == Variable_Value_Map.end())
		  {
		    cout<<"\nNo entry in Variable_Value_Map for variable "<<operand_string<<" in function lmdeEvaluatorModified in Eliminator.cpp\n";
		    exit(1);
		  }
		string bin_value = Variable_Value_Map_it->second; // binary value of the variable from map 

		cout<<"\noperand_string = "<<operand_string<<"\tlow_sel_number = "<<low_sel_number<<"\thigh_sel_number = "<<high_sel_number<<endl;
		cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

		bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()
		cout<<"\nresult = "<<endl;
		bvm->printBVatom(result, 0);


		//cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

		if(bin_value.size() < OperandWidth) // bin_value needs to be zero-extended to match width
		  {
		    //cout<<"\nbin_value.size() < width\n";
		    bv_result = bvm->BVExtend(result, OperandWidth, 0);
		  }
		else if(bin_value.size() > OperandWidth) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		  {
		    //cout<<"\nbin_value.size() < width\n";
		    bv_result = bvm->select_with_int(result, OperandWidth-1, 0);
		  }
		else
		  {
		    //cout<<"\nbin_value.size() = width\n";
		    bv_result = result;
		  }

		cout<<"\nbv_result = "<<endl;
		bvm->printBVatom(bv_result, 0);


		// Now we have obtained the bv_result from the operand part; let us take the selection part also

		bv_result = bvm->select_with_int(bv_result, high_sel_number, low_sel_number);

	      } //"select" ends here

	    else if(outlist_size==0) // constants
	      {

		ULINT dec_value = strtoull(dag_str.c_str(), NULL, 0); // decimal value of the constant obtained from the dag node
		string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
		bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

		if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
		  {
		    bv_result = bvm->BVExtend(result, width, 0);
		  }
		else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		  {
		    bv_result = bvm->select_with_int(result, width-1, 0);
		  }
		else
		  {
		    bv_result = result;
		  }

	      }// if a constant node ends here

	}//if an operator node or a constant node ends here


      cout<<"\nbv_result from "<<dag_str<<" for lmdeEvaluatorModified = ";
      bvm->printBVatom(bv_result, 0);
  
  return bv_result;
		    
}// function ends here


// Return decimal digit corresponding to a given hex digit
ULINT getCoefOfChar(char c)
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

// Return power^16
ULINT findPower16(int power)
{
ULINT number=1;
int i=power;
while(i>0)
	{
	number=number*16;
	i--;
	}
return number;
}

// Return decimal number corresponding to a hex string
ULINT hexStringToULINT(string x)
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

	if(c!= '0')
	  {
	    coef=getCoefOfChar(c);
	    power=length-i-1;
	    power2=findPower16(power);
	    number+=power2*coef;
	  }
	i++;
	}

return number;
}



// Same as evaluateFunction except that the function also returns a subset of the support of dag
// which is sufficient to evaluate dag to the specific value (in "ProblematicSignalsAtPreviousStateForSignal")
string findUnsatCoreEvaluationBasedStrategy(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal)
{
  // make node point to the RHS of the function

  bool evaluate_only_rhs = false;

  if(evaluate_only_rhs)
    {
      if(node->Get_node_name() == "="  || node->Get_node_name() == "equivalent")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  iout++;
	  node = *iout;
	}
      else
	{
	  cout<<"\nArgument dag is "<<node->Get_node_name()<<" i.e. not a function in findUnsatCoreEvaluationBasedStrategy in Eliminator.cpp\n";
	  exit(1);
	}
    }
  


  map<DAGNode*, pair<bvatom, set<string> > > DPTable;
  DPTable.clear();
  bvatom ret_value = findUnsatCoreEvaluationBasedStrategy_Internal(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal, DPTable);

  // printing the result for debugging
  cout<<"\nret_value = ";
  bvm->printBVatom(ret_value, 0);

  showSet(ProblematicSignalsAtPreviousStateForSignal, "ProblematicSignalsAtPreviousStateForSignal");

  string hex_str = bvm->convertBVToHexString(ret_value);
  cout<<"\nhex_str = "<<hex_str<<endl;
  unsigned long long int dec_value = hexStringToULINT(hex_str); // decimal value of the hex string
  cout<<"\ndec_value = "<<dec_value<<endl;
  string ret_value_str = integerToBinaryString(dec_value); // binary value with minimum required width
  cout<<"\nret_value_str = "<<ret_value_str<<endl;
  return ret_value_str;

}

// Function called by findUnsatCoreEvaluationBasedStrategy to achieve the functionality described
// in the synopsys of findUnsatCoreEvaluationBasedStrategy
bvatom findUnsatCoreEvaluationBasedStrategy_Internal(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal, map<DAGNode*, pair<bvatom, set<string> > > &DPTable)
{

  map<DAGNode*, pair<bvatom, set<string> > > ::iterator dpit = DPTable.find(node);
 
  if(dpit!=DPTable.end()) // node already encountered
    {
      //cout<<"\nDPTable Hit\n";
      ProblematicSignalsAtPreviousStateForSignal = (dpit->second).second;
      return (dpit->second).first;
    }

  else { // encountered first time

    bvatom ret_value;
    string dag_str;
    int outlist_size;

    dag_str = node->Get_node_name();
    outlist_size = node->Get_outlist_size();
    Value* MyValue = (Value*)(node->getValue());
    Type MyType = MyValue->get_node_type();
    int MyWidth = MyType.width;
    int MyDataType = MyType.data_type;


    ProblematicSignalsAtPreviousStateForSignal.clear();


    cout<<"\n"<<dag_str<<" node encountered in findUnsatCoreEvaluationBasedStrategy_Internal in Eliminator.cpp\n";

    if(dag_str == "=" || dag_str == "is_not_equal" || dag_str == "not_equal_to" || dag_str == "equivalent") // LME/LMD
      {
	ret_value = call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal);
      }
    else if(outlist_size != 0)// Internal node
      {
	bvatom true_value = bvm->getZeroedBVatom(1);
	true_value.bv_array[0] = (bv_block_t) 1;

	bvatom false_value = bvm->getZeroedBVatom(1);
	false_value.bv_array[0] = (bv_block_t) 0;

      
	if(dag_str == "and" && outlist_size  == 2) // binary and
	  {

	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild2;

	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);
	  
	    iout++;

	    bvatom child2 = findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild2, DPTable);

	    bool child1_is_false = bvm->checkBVEquality(child1, false_value);
	    bool child2_is_false = bvm->checkBVEquality(child2, false_value);

	    if(child1_is_false && child2_is_false)// child1 is false and child2 is false
	      {
		findSmallerModel(ProblematicSignalsAtPreviousStateForSignalChild1, ProblematicSignalsAtPreviousStateForSignalChild2, ProblematicSignalsAtPreviousStateForSignal); 
		ret_value = false_value;
	      }
	    else if(child1_is_false)// only child1 is false
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild1;
		ret_value = false_value;
	      }
	    else if(child2_is_false)// only child2 is false
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild2;
		ret_value = false_value;
	      }
	    else // child1 is true and child2 is true
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild2.begin(), ProblematicSignalsAtPreviousStateForSignalChild2.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		ret_value = true_value;
	      }
	  }//binary and
      
	else if(dag_str == "or" && outlist_size  == 2) // binary or
	  {
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild2;

	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);
	  
	    iout++;

	    bvatom child2 = findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild2, DPTable);

	    bool child1_is_true = bvm->checkBVEquality(child1, true_value);
	    bool child2_is_true = bvm->checkBVEquality(child2, true_value);

	    if(child1_is_true && child2_is_true)// child1 is true and child2 is true
	      {
		findSmallerModel(ProblematicSignalsAtPreviousStateForSignalChild1, ProblematicSignalsAtPreviousStateForSignalChild2, ProblematicSignalsAtPreviousStateForSignal); 
		ret_value = true_value;
	      }
	    else if(child1_is_true)// only child1 is true
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild1;
		ret_value = true_value;
	      }
	    else if(child2_is_true)// only child2 is true
	      {
		ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild2;
		ret_value = true_value;
	      }
	    else // child1 is false and child2 is false
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild2.begin(), ProblematicSignalsAtPreviousStateForSignalChild2.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		ret_value = false_value;
	      }

	  }// binary or

	else if(dag_str == "ite" || dag_str == "if_then_else")
	  {

	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild2;
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild3;


	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);
	  
	    iout++;

	    bvatom child2 = findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild2, DPTable); 

	    iout++;

	    bvatom child3 = findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild3, DPTable); 

	    bool child1_is_true = bvm->checkBVEquality(child1, true_value);

	    if(child1_is_true)// child1 is true
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild2.begin(), ProblematicSignalsAtPreviousStateForSignalChild2.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		
		ret_value = child2;
	      }
	    else // child1 is false
	      {
		set_union(ProblematicSignalsAtPreviousStateForSignalChild1.begin(), ProblematicSignalsAtPreviousStateForSignalChild1.end(), ProblematicSignalsAtPreviousStateForSignalChild3.begin(), ProblematicSignalsAtPreviousStateForSignalChild3.end(), inserter(ProblematicSignalsAtPreviousStateForSignal, ProblematicSignalsAtPreviousStateForSignal.begin())); 
		
		ret_value = child3;
	      }
	  
	  }// ite


	else if(dag_str == "not")
	  {
	    set<string> ProblematicSignalsAtPreviousStateForSignalChild1;

	    list<DAGNode*>::iterator iout = node->Get_outlist_start();

	    bvatom child1 =  findUnsatCoreEvaluationBasedStrategy_Internal(*iout, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignalChild1, DPTable);

	    ProblematicSignalsAtPreviousStateForSignal = ProblematicSignalsAtPreviousStateForSignalChild1;
	  
	    ret_value = bvm->bitwise_negate(child1);
	    
	  }// not
	else if(dag_str == "bvmul" || dag_str == "bvadd")
	  {
	    ret_value = call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal);
	  }// * and +
	else
	  {
	    cout<<"\nUnknown operator "<<dag_str<<" in function findUnsatCoreEvaluationBasedStrategy_Internal in Eliminator.cpp\n";
	    exit(1);
	  }
       

      }// internal node

    else // constants and variables
      {
	if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT)  //i.e. if variable
	  {
	    ret_value = call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(node, Variable_Value_Map, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal);
	    
	  } //variable node ends here

	else // constants
	  {
	    ULINT dec_value = strtoull(dag_str.c_str(), NULL, 0); // decimal value of the constant obtained from the dag node
	    string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
	    bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

	    if(bin_value.size() < MyWidth) // bin_value needs to be zero-extended to match MyWidth
	      {
		ret_value = bvm->BVExtend(result, MyWidth, 0);
	      }
	    else if(bin_value.size() > MyWidth) // bin_value needs to be truncated to match width. Hence select from 0 to MyWidth-1
	      {
		ret_value = bvm->select_with_int(result, MyWidth-1, 0);
	      }
	    else
	      {
		ret_value = result;
	      }
	  }//constants
      }//constants and variables

    DPTable.insert(make_pair(node, make_pair(ret_value, ProblematicSignalsAtPreviousStateForSignal)));

    cout<<"\nret_value from "<<dag_str<<" for findUnsatCoreEvaluationBasedStrategy_Internal = ";
    bvm->printBVatom(ret_value, 0);
    cout<<"\nProblematicSignalsAtPreviousStateForSignal from "<<dag_str<<" for findUnsatCoreEvaluationBasedStrategy_Internal = \n";
    showSet(ProblematicSignalsAtPreviousStateForSignal, "ProblematicSignalsAtPreviousStateForSignal");

    return ret_value;
  }// DPTable miss

}// Function ends 



// Same as call_lmdeEvaluatorReturningBV except that it also finds the support of
// the LMDE rooted at "node" in "ProblematicSignalsAtPreviousStateForSignal"
bvatom call_lmdeEvaluatorReturningBVWithMinimumSupportFinding(DAGNode *node, map<string, string> &Variable_Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal)
{
  //cout<<"\ncall_lmdeEvaluatorReturningBVWithMinimumSupportFinding called\n";
  
  if(node == NULL)
    {
     cout<<"Function call_lmdeEvaluatorReturningBVWithMinimumSupportFinding in Eliminator.cpp called with NULL node\n";
     exit(1);
    } 
    // find width
  string variable = "";
  variable = getSingleVariable(node);
  if(variable=="")
    {
      cout<<"\nLME/LMD "<<getDAGStr(node)<<" encountered with no variable names in function call_lmdeEvaluatorReturningBVWithMinimumSupportFinding in Eliminator.cpp\n";
      exit(1);
    }
  
  map<string, int>::iterator MyWidthTable_it = MyWidthTable.find(variable);
  if(MyWidthTable_it == MyWidthTable.end())
	    {
	      cout<<"\nNo entry in MyWidthTable for variable "<<variable<<" in function call_lmdeEvaluatorReturningBVWithMinimumSupportFinding in Eliminator.cpp\n";
	      exit(1);
	    }
  int width = MyWidthTable_it->second;

  bool boolean_result;

  bvatom bv_result = lmdeEvaluatorModified(node, width, Variable_Value_Map, boolean_result);

  // ProblematicSignalsAtPreviousStateForSignal is presently just the support of node

  getDAGNames(node, ProblematicSignalsAtPreviousStateForSignal);  

  //cout<<"\ncall_lmdeEvaluatorReturningBVWithMinimumSupportFinding finished\n";

  return bv_result; 
}

// ReducedModel is the smaller one of ReducedModelChild1 and ReducedModelChild2
void findSmallerModel(set<string> &ReducedModelChild1, set<string> &ReducedModelChild2, set<string> &ReducedModel)
{
  if(ReducedModelChild1.size() < ReducedModelChild2.size()) // ReducedModelChild1 is smaller
	{
	ReducedModel = ReducedModelChild1;
	}
  else
	{
	ReducedModel = ReducedModelChild2;
	}
}


// create the dag for ("variable" = "value") and return it
DAGNode* getEquationDAGFromAssignment(string variable, int width, string value)
	{
        DAGNode* result=NULL;
	DAGNode *lhs=NULL, *rhs=NULL;

	lhs = createBVVariable(variable, width);
	rhs = createBVConstant(value, width);
	result = createDAG("=", lhs, rhs, LOGICAL_SYMBOL, 1);
	return result;
	}

// Given a dag rooted at "node" and a variable --> value map "Value_Map", find unsat. core for
// the formula (node \and \and(variable_i = value_i)) where i=0 to sizeof(Value_Map)-1 
// Unsat core is computed in ProblematicSignalsAtPreviousStateForSignal. The return string is "Hi" always.
string findUnsatCoreSMTBasedStrategy(DAGNode *node, map<string, string> &Value_Map, map<string, int> &MyWidthTable, set<string> &ProblematicSignalsAtPreviousStateForSignal)
{

  int implemention_type = 2;


  // Value_Map has values in binary. Convert into Variable_Value_Map where the values are in ULINT; but stored as string
  
  map<string, string> Variable_Value_Map;

  for(map<string, string>::iterator vit = Value_Map.begin(); vit != Value_Map.end(); vit++)
    {
      string variable = vit->first;
      string value = vit->second;
      ULINT value_ulint = binaryStringToULINT(value);
      char value_ulint_char[100];
      sprintf(value_ulint_char, "%llu", value_ulint);
      string value_ulint_string(value_ulint_char);
      
      Variable_Value_Map.insert(make_pair(variable, value_ulint_string));
      }


  cout<<"\nVariable_Value_Map\n";
  for(map<string, string>::iterator vit = Variable_Value_Map.begin(); vit != Variable_Value_Map.end(); vit++)
    {	
      cout<<endl<<vit->first<<"\t"<<vit->second<<endl;
    }


   

  if(implemention_type == 0)// old method of writing CNF : with new dag nodes in assignment part with zminimal
	{

  	// make node point to node /\ Equations created from Variable_Value_Map

  	set<string> SupportNode;

  	getDAGNames(node, SupportNode);


  	DAGNode *EquationPart = NULL;

  	for(map<string, string>::iterator it = Variable_Value_Map.begin(); it != Variable_Value_Map.end(); it++)
		{
		string variable = it->first;
		string value = it->second;

        	if(SupportNode.find(variable) == SupportNode.end()) // variable is not in Support
			{
			continue;
			}

		map<string, int>::iterator wit = MyWidthTable.find(variable);
		if(wit == MyWidthTable.end())
			{
			cout<<"\nNo entry for "<<variable<<" in WidthTable in findUnsatCoreSMTBasedStrategy in Eliminator.cpp\n";	
			exit(1);
			}
		int width = wit->second;
	
		cout<<"\nvariable = "<<variable<<"\tvalue = "<<value<<"\twidth = "<<width<<endl;
		
		DAGNode *Equation = getEquationDAGFromAssignment(variable, width, value);

		if(EquationPart == NULL)
			{
			EquationPart = Equation;
			}
		else
			{
			EquationPart = createDAG("and", EquationPart, Equation, LOGICAL_SYMBOL, 1);
			}
		}

  	cout<<"\nConjoining EquationPart with node,\n";

  	if(EquationPart != NULL)
		{
		node = createDAG("and", node, EquationPart, LOGICAL_SYMBOL, 1);
		}

  	cout<<"\nnode = "<<getDAGStr(node)<<endl;

  	// Let's just ensure that node is unsat

  	bool ensure_unsat = true;

  	if(ensure_unsat)
		{
		FILE *smt_file = fopen("unsat_core_bv_level.smt", "w");
		if(smt_file == NULL)
    			{cout<<"\nCannot create unsat_core_bv_level.smt\n"; exit(1);}
		fprintf(smt_file,"(benchmark test\n");
		set<string> Vars;
  		getDAGNames(node, Vars);
		writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, MyWidthTable, Vars);
		string dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(node, MyWidthTable);
		fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  		fprintf(smt_file, "\n)\n");
  		fclose(smt_file);
		}
	
  	blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignals(node, MyWidthTable, ProblematicSignalsAtPreviousStateForSignal); 

  	return "Hi"; 
	}

else if(implemention_type == 1)// new method of writing CNF : no new dag nodes in assignment part with zminimal
	{
	set<string> SupportNode;
	getDAGNames(node, SupportNode);


	set<string> OriginalBits;
	set<string> NegatedBits;
	
	for(map<string, string>::iterator it = Variable_Value_Map.begin(); it != Variable_Value_Map.end(); it++)
		{
		string variable = it->first;
		string value = it->second;

		if(SupportNode.find(variable) == SupportNode.end()) // variable is not in Support
			{
			continue;
			}

		ULINT value_ulint = strtoull(value.c_str(), NULL, 0);

		map<string, int>::iterator wit = MyWidthTable.find(variable);
		if(wit == MyWidthTable.end())
			{
			cout<<"\nNo entry for "<<variable<<" in WidthTable in findUnsatCoreSMTBasedStrategy in Eliminator.cpp\n";	
			exit(1);
			}
		int width = wit->second;

		string value_bin = integerToBinaryStringWithZeroPadding(value_ulint, width);

                int bitposition = 0;
		for(int index=width-1; index>=0; index--)
			    {
			    // create variable_index;
			    string bit;
			    bit = variable;
			    
			    char bitposition_char[100];
			    sprintf(bitposition_char, "%d", bitposition);
			    string bitposition_string(bitposition_char);

			    bit += "_";
			    bit += bitposition_string;

			    if(value_bin[index]=='0') // value of bin is false
				{
				NegatedBits.insert(bit);
				}
			    else
				{
				OriginalBits.insert(bit);
				}
			   bitposition++;
			   }// for each index
		}//for each entry in Variable_Value_Map

            showSet(OriginalBits, "OriginalBits");
	    showSet(NegatedBits, "NegatedBits");


	    cout<<"\nnode = "<<getDAGStr(node)<<endl;

  	    blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvanced(node, MyWidthTable,OriginalBits,NegatedBits, ProblematicSignalsAtPreviousStateForSignal);

 	    return "Hi";
	    }//else if

else if(implemention_type == 2)// new method of writing CNF : no new dag nodes in assignment part with run_till_fix_modified.pl
	{
	set<string> SupportNode;
	getDAGNames(node, SupportNode);


	set<string> OriginalBits;
	set<string> NegatedBits;
	
	for(map<string, string>::iterator it = Variable_Value_Map.begin(); it != Variable_Value_Map.end(); it++)
		{
		string variable = it->first;
		string value = it->second;

		if(SupportNode.find(variable) == SupportNode.end()) // variable is not in Support
			{
			continue;
			}

		ULINT value_ulint = strtoull(value.c_str(), NULL, 0);

		map<string, int>::iterator wit = MyWidthTable.find(variable);
		if(wit == MyWidthTable.end())
			{
			cout<<"\nNo entry for "<<variable<<" in WidthTable in findUnsatCoreSMTBasedStrategy in Eliminator.cpp\n";	
			exit(1);
			}
		int width = wit->second;

		string value_bin = integerToBinaryStringWithZeroPadding(value_ulint, width);

		cout<<endl<<"value_bin = "<<value_bin<<endl;

                int bitposition = 0;
		for(int index=width-1; index>=0; index--)
			    {
			    // create variable_index;
			    string bit;
			    bit = variable;
			    
			    char bitposition_char[100];
			    sprintf(bitposition_char, "%d", bitposition);
			    string bitposition_string(bitposition_char);

			    bit += "_";
			    bit += bitposition_string;

			    if(value_bin[index]=='0') // value of bin is false
				{
				NegatedBits.insert(bit);
				}
			    else
				{
				OriginalBits.insert(bit);
				}
			   bitposition++;
			   }// for each index
		}//for each entry in Variable_Value_Map

            showSet(OriginalBits, "OriginalBits");
	    showSet(NegatedBits, "NegatedBits");


	    cout<<"\nnode = "<<getDAGStr(node)<<endl;

  	    blastAndWriteInGivenFileInDIMACSFormatAndGetUnsatCoreSignalsAdvancedEfficient(node, MyWidthTable,OriginalBits,NegatedBits, ProblematicSignalsAtPreviousStateForSignal);

 	    return "Hi";
	    }//else if

else
	{
	cout<<"\nUnknown implementation_type in findUnsatCoreSMTBasedStrategy in Eliminator.cpp\n";
	exit(1);
	}

}// function ends


// Internal function called by getDescendents to get the set of descendents of a node
void getDescendentsInternal(DAGNode *node, set<DAGNode*> &InitialDescendents)
{
  InitialDescendents.insert(node);

  list<DAGNode*>::iterator iout = node->Get_outlist_start();

  for(unsigned int i=0; i<node->Get_outlist_size() ; i++, iout++)
        {
            getDescendentsInternal(*iout, InitialDescendents);
        }

}


// Get the set of descendents of the dag node "node"
void getDescendents(DAGNode *node, set<DAGNode*> &Descendents)
{
  set<DAGNode*> InitialDescendents;
  
  getDescendentsInternal(node, InitialDescendents);

   // Descendents = InitialDescendents \ {node, true, false}

  DAGNode* TrueNode = createLogicalConstant("true");

  DAGNode* FalseNode = createLogicalConstant("false");
  
  set<DAGNode*> MySet;

  MySet.insert(node);

  MySet.insert(TrueNode);

  MySet.insert(FalseNode);

  set_difference(InitialDescendents.begin(), InitialDescendents.end(), MySet.begin(), MySet.end(), inserter(Descendents, Descendents.begin()));

}

// Given a dag rooted at "parent", create a dag where the subtractions are replaced by additions
// with additive inverses
DAGNode* getSubtractionReplacedClone(DAGNode* parent, map<DAGNode*, DAGNode*> &SubtractionTable, map<string, int> &WidthTable)
{
  DAGNode* clone;

  if(parent->Get_node_name() == "bvsub") // subtraction node encountered
    {
      map<DAGNode*, DAGNode*>::iterator it = SubtractionTable.find(parent);
	    
      if(it != SubtractionTable.end()) // parent is already added to the Table. get the address of the new node and return it
	{
	  clone = it->second;
	}
      else // parent is newly encountered. Create a new node and return it's address
	{
	  clone = recreateSubtractionDAG(parent, WidthTable);
	  SubtractionTable.insert(make_pair(parent, clone)); // inserted into SubtractionTable
	}
    }// subtraction node
                   
  else
    {
      unsigned int outlist_size = parent->Get_outlist_size();

      if(outlist_size>0)  //i.e. if an operator node
	{
	  list<DAGNode*>::iterator iout = parent->Get_outlist_start();

	  switch(outlist_size)
            {
	    case 1:
	      {
		DAGNode* child_1 ;
		child_1 = getSubtractionReplacedClone(*iout, SubtractionTable, WidthTable);

		vector<DAGNode *> Children;
		Children.push_back(child_1);
		clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
		break;
	      }
	    case 2:
	      {
		DAGNode* child_1 ;
		DAGNode* child_2 ;
                    
		child_1 = getSubtractionReplacedClone(*iout, SubtractionTable, WidthTable);
		iout++;
		child_2 = getSubtractionReplacedClone(*iout, SubtractionTable, WidthTable);

		vector<DAGNode *> Children;
		Children.push_back(child_1);
		Children.push_back(child_2);
		clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
		break;
	      }
	    case 3:
	      {
		DAGNode* child_1 ;
		DAGNode* child_2 ;
		DAGNode* child_3 ;

		child_1 = getSubtractionReplacedClone(*iout, SubtractionTable, WidthTable);
		iout++;
		child_2 = getSubtractionReplacedClone(*iout, SubtractionTable, WidthTable);
		iout++;
		child_3 = getSubtractionReplacedClone(*iout, SubtractionTable, WidthTable);

		vector<DAGNode *> Children;
		Children.push_back(child_1);
		Children.push_back(child_2);
		Children.push_back(child_3);
		clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
		break;
	      }
	    default:
	      {
		cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function getSubtractionReplacedClone in Eliminator.cpp\n";
		exit(1);
	      }

	    }//switch(outlist_size)
	}//if(outlist_size>0)
      else // leaf node
	{
	  clone = dm.createNode(parent->Get_node_name(), parent->getValue());
	}
    }//else

  return clone;
}

// Copy the set of strings to a list of strings
void copySetToList(set<string> MySet, list<string> &MyList)
{
  
for(set<string>::iterator it=MySet.begin(); it!=MySet.end(); it++)
  {
    MyList.push_back(*it);
  }
}



// Given a dag node rooted at "parent" where the label of the root is "bvsub", return 
// a node where the "bvsub" is replaced with "bvadd" with additive inverse
DAGNode* recreateSubtractionDAG(DAGNode* parent, map<string, int> &WidthTable)
{
  set<string> Names;
  list<string> Vars;
  list<ULINT> Coefs;
  
  int Width;
  ULINT Mod;
  list<string>::iterator it;

  cout<<"\nOriginal DAG = "<<getDAGStr(parent)<<endl;
  
  getDAGNames(parent, Names);
  
  copySetToList(Names, Vars);

  it = Vars.begin();

  map<string, int>::iterator wit = WidthTable.find(*it);

  if(wit == WidthTable.end())
    {
      cout<<"\n"<<*it<<" has no entry in WidthTable in recreateSubtractionDAG in Eliminator.cpp\n";
      exit(1);
    }


  Width = wit->second;

  Mod = findPower(Width);

  cout<<"\nWidth = "<<Width<<"\tMod = "<<Mod<<endl;
  
  for(it=Vars.begin(); it!=Vars.end(); it++)
    {
    ULINT Coef=0; 
    int Sign=0;

    getCoefficientWithMinusSupported(parent, *it, Coef, Sign, Mod);

    cout<<"\nVar = "<<*it<<"\tCoef = "<<Coef<<endl;
      
    Coefs.push_back(Coef);
    }

  ULINT constant=0;
  int Sign=0;

  getConstantModifiedWithMinusSupported(parent, constant, Sign, Mod);

    
  DAGNode* RecreatedNode = getDAGFromVarsCoefsAndConstant(Vars, Coefs, constant, Width);

  cout<<"\nRecreated DAG = "<<getDAGStr(RecreatedNode)<<endl;
  
  return RecreatedNode;
}

// Given lists {c1,...,cn}, {x1,...,xn} and c0, create dag for c1.x1 + ... + cn.xn + c0, and return it
DAGNode* getDAGFromVarsCoefsAndConstant(list<string> Vars, list<ULINT> Coefs, ULINT SelectedConst, int width)
{
  	list<string>::iterator Vars_it;
	list<ULINT>::iterator Coefs_it;

        DAGNode *lhs=NULL;

	Vars_it=Vars.begin();
	Coefs_it=Coefs.begin();
	
	for(; Vars_it != Vars.end(); Vars_it++, Coefs_it++)
	  {
	    string varname = *Vars_it;

	    DAGNode *variable = createBVVariable(varname, width);

	    ULINT Coef = *Coefs_it;

	    char coeff[20];
	    sprintf(coeff, "%llu", Coef);
	    string coeff_str(coeff);

	    DAGNode *coefficient = createBVConstant(coeff_str,width);
	    DAGNode *term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
	    if(lhs==NULL) lhs=term; else lhs=createDAG("bvadd", lhs, term, BITVECTOR_SYMBOL, width);
	  }

	if(SelectedConst != 0)
	  {
	    char constant[20];
	    sprintf(constant, "%llu", SelectedConst);
	    string constant_str(constant);

	    DAGNode *term = createBVConstant(constant_str,width);
	    if(lhs==NULL) lhs=term; else lhs=createDAG("bvadd", lhs, term, BITVECTOR_SYMBOL, width);
	  }

	return lhs;
}



// Function getConstantModified with support for bvsub nodes.
// bvsub is treated as bvadd with the additive inverse
void getConstantModifiedWithMinusSupported(DAGNode* node, ULINT &constant, int Sign, ULINT Mod)
{
 // Go down the +, - operator until constant is encountered.

  string dag_str = node->Get_node_name();

  int outlist_size = node->Get_outlist_size();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

  
  if(outlist_size == 0 && MyDataType == BITVECTOR_CONSTANT) // bv-constant
    {
      ULINT LocalConstant;
      LocalConstant = convertLiteralToNumber(dag_str); 
      if(Sign == 1) 
	{
	  LocalConstant = aInverse(LocalConstant, Mod);
	}
      constant = add(constant, LocalConstant, Mod);
    }
  else if(outlist_size == 0 && MyDataType == INTEGER_CONSTANT) // integer-constant
    {
      cout<<"\nWarning!Integer constant "<<dag_str<<" encountered in function getConstantModifiedWithMinusSupported in Eliminator.cpp\n";
      ULINT LocalConstant;
      LocalConstant = convertLiteralToNumber(dag_str); 
      if(Sign == 1) 
	{
	  LocalConstant = aInverse(LocalConstant, Mod);
	}
      constant = add(constant, LocalConstant, Mod);
    }
  else if(dag_str == "bvadd")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	   
          getConstantModifiedWithMinusSupported(*iout, constant, Sign, Mod);
	  iout++;
	  getConstantModifiedWithMinusSupported(*iout, constant, Sign, Mod);
	}

  else if(dag_str == "bvsub")
     {
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	   
          getConstantModifiedWithMinusSupported(*iout, constant, Sign, Mod);
	  iout++;

	  int Opposite = (Sign==0)?1:0;
	  getConstantModifiedWithMinusSupported(*iout, constant, Opposite, Mod);
	}
}


// Function getCoefficient with support for bvsub nodes.
// bvsub is treated as bvadd with the additive inverse.
void getCoefficientWithMinusSupported(DAGNode* node, string Var, ULINT &Coefficient, int Sign, ULINT Mod)
{
  string dag_str = node->Get_node_name();
  int outlist_size = node->Get_outlist_size();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;


  if(dag_str == "bvmul")
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();

	  DAGNode *coef = *iout;
	  iout++;
	  DAGNode *var = *iout;
	  string variable = var->Get_node_name();

	   //cout<<"\nvariable = "<<variable<<endl;

           if(variable == Var)
		{
		ULINT LocalCoef;
		getConstantModified(coef, LocalCoef);
		if(Sign == 1) 
		  {
		    LocalCoef = aInverse(LocalCoef, Mod);
		  }
		Coefficient = add(Coefficient, LocalCoef, Mod);
		}
	}

  else if(dag_str == "bvsub")     
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  
	  getCoefficientWithMinusSupported(*iout, Var, Coefficient, Sign, Mod);
	  
	  iout++;
	  
	  int Opposite = (Sign==0)?1:0;

	  getCoefficientWithMinusSupported(*iout, Var, Coefficient, Opposite, Mod);
	}
  
      else if(dag_str == "bvadd")         
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();

	  getCoefficientWithMinusSupported(*iout, Var, Coefficient, Sign, Mod);

	  iout++;

	  getCoefficientWithMinusSupported(*iout, Var, Coefficient, Sign, Mod);

	}
      else if(outlist_size == 0) // Leaf level
	   {
	     if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT) // operand
	       {
		 string variable = dag_str; 
		 if(variable == Var)
		   {
		     ULINT LocalCoef = 1;
		     if(Sign == 1) 
		       {
			 LocalCoef = aInverse(LocalCoef, Mod);
		       }
		     Coefficient = add(Coefficient, LocalCoef, Mod);
		   }
	       }
	   }
	 else
	   {
	     cout<<"\nUnknown node "<<dag_str<<" occured inside getCoefficientWithMinusSupported in Eliminator.cpp\n";
	     exit(1);
	   }
	 
}


// Return the width of an LMDE node with selection operator
int getWidthOfLMDEWithSelects(DAGNode *node, map<string, int> MyWidthTable)
{
  map<DAGNode*, DAGNode*> LinearizationTable;
  int count=0;
  set<string> VarSetToElim;

  DAGNode* LinearizedClone = getLinearizedClone(node, LinearizationTable, count, MyWidthTable, VarSetToElim);

  string variable = "";
  variable = getSingleVariable(LinearizedClone);
  if(variable=="")
    {
      cout<<"\nLME/LMD "<<getDAGStr(LinearizedClone)<<" encountered with no variable names in function getWidthOfLMDEWithSelects in Eliminator.cpp\n";
      exit(1);
    }
  
  map<string, int>::iterator MyWidthTable_it = MyWidthTable.find(variable);
  if(MyWidthTable_it == MyWidthTable.end())
    {
      cout<<"\nNo entry in MyWidthTable for variable "<<variable<<" in function getWidthOfLMDEWithSelects in Eliminator.cpp\n";
      exit(1);
    }
  int width = MyWidthTable_it->second;

  return width;
}

// Returns true if f is unsat; false otherwise using STP
// Here the dag is printed in SMTLIB format as dag itself - with commmon sub-expression sharing
// Called by ThUnsatSingleFormula
bool ThUnsatSingleFormulaInSMTLibFormatAsDAG(DAGNode* f, map<string, int> &WidthTable)
{
  FILE *smt_file = fopen("thunsat_single_formula.smt", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.smt\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  string dag_str = printDAGInSMTLIBFormatAsDAG(f, WidthTable, smt_file);

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);

  if(LMI_handling_enabled)
    system("stp -w --SMTLIB1 thunsat_single_formula.smt > result_thunsat_single_formula.txt"); //call to STP with word-level simplifcations off
  else 
    system("stp --SMTLIB1 thunsat_single_formula.smt > result_thunsat_single_formula.txt"); //call to STP

  FILE *result_smt_file = fopen("result_thunsat_single_formula.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_single_formula.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function  DDManager::ThUnsatSingleFormulaInSMTLibFormatAsDAG\n"; exit(1);
    }
}

// Functions for dag evaluation and unsat. core finding end here

// Functions for replacing Yices by STP

// Function to print the dag "root" in SMTLIB format as dag itself - with commmon sub-expression sharing
// Called by ThUnsatSingleFormulaInSMTLibFormatAsDAG
// In the future use this function to express a given arbitrary formula in SMTLIB format
// when the result is needed in a dag format
string printDAGInSMTLIBFormatAsDAG(DAGNode *root, map<string, int> &WidthTable, FILE *fp)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();

  //cout<<"\nObtaining the WidthTable\n";

  createWidthTableForOperatorsWithDP(root, WidthTableForNodes, WidthTable);

  //cout<<"\nWidthTable obtained\n";

  set<DAGNode*> DPTable1;

  //cout<<"\nPropgating widths\n";

  DPTable1.clear();

  propogateWidthsWithDP(root, WidthTableForNodes, 1, DPTable1);

  //cout<<"\nWidths propogated\n";

  DPTable1.clear();


  //cout<<"\nSetting the types\n";

  map<DAGNode*, string> TypeTableOfNodes;

  setTypesWithDP(root, "bool", TypeTableOfNodes);

  //cout<<"\nTypes set\n";

  //cout<<"\nGetting SMTLIB string\n";

  string smtlib_str;

  //cout<<"\nDAG to be printed in SMTLib format "<<root->getDAGStr()<<endl;

  int TempCount = 0;
  map<DAGNode*, string> LabelTable;
  LabelTable.clear();


  smtlib_str =  printInSMTLibFormatModified(root, "bool", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes);


  //cout<<"\nSMTLIB string obtained\n";

   return smtlib_str;
}


// Modified version of the function convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat,
// where the width is not one.
string convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(string literal, int width)
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
   {cout<<"\nFunction string convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(string literal, int Width) in Eliminator.cpp called with literal = "<<literal<<endl; exit(1);}
 

 //cout<<"\nInput Number = "<<literal<<endl;
 //cout<<"\nBV in SMTLib format  = "<<str<<endl;

return str;
}


// Function to print the dag "root" in SMTLIB format as dag itself - with commmon sub-expression sharing
// Called by printDAGInSMTLIBFormatAsDAG
string printInSMTLibFormatModified(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<DAGNode*, string> &LabelTable, FILE *fp, map<string, int> &WidthTable, int &TempCount, map<DAGNode*, string> &TypeTableOfNodes)
{
  string label;
  string name = node->Get_node_name();
  unsigned int outlist_size = node->Get_outlist_size();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int node_type = MyType.data_type;
  list<DAGNode*>::iterator iout = node->Get_outlist_start();

  //cout<<"\nname=\t"<<name<<endl;    //debug code

  if( (node_type == BITVECTOR_CONSTANT || node_type == LOGICAL_CONSTANT || node_type == INTEGER_CONSTANT) && (outlist_size == 0)) // constants 
    {
      if(name == "'1'")
	{
	  label = convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);
	  
	}

      else if(name == "'0'")
	{
	  label = convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);

	}

      else if(context == "const")
	{
	  label = name;
	}
      else if(context == "bv")
	{
	     
	  if(name[0] == 'x') // hex
	    {
		 
	      label = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
	    }
	  else // Decimal
	    {

	      label = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
	    }
			  
	}
      else if(context == "bool")
	{
	  if(name == "1") label = "true";
	  else if(name == "0") label = "false";
	  else if(name == "true") label = "true";
	  else if(name == "false") label = "false";

	  else // interpreting a Boolean constant other than true/false as a bit-vector
	    {
	      if(name[0] == 'x') // hex
		{
		  label = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		}
	      else // Decimal
		{
		  label = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		}
	    }

	}
      else
	{
	  cout<<"\nUnkown name "<<name<<" with context "<<context<<" encountered in function printInSMTLibFormatModified in Eliminator.cpp\n";
	  exit(1);
	}
      return label;
    }

  else // variable or operator
    {
      map<DAGNode*, string>::iterator it=LabelTable.find(node);
      //cout<<"\nnode = "<<node<<", label = "<<label<<endl;
      if(it!=LabelTable.end()) // entry exists for the node
	{
	  //cout<<"\nentry exists for "<<node<<endl;
	  label = it->second;
	  return label;
	}
      else
	{
	  if(!(node_type == BITVECTOR_CONSTANT || node_type == LOGICAL_CONSTANT || node_type == INTEGER_CONSTANT) && outlist_size == 0)  // variable
	    {
	      label = name; // label obtained
	      map<string, int>::iterator wit = WidthTable.find(label);
	      if(wit == WidthTable.end())
		{
		  cout<<"\nThe variable "<<label<<" has no entry in the WidthTable in function printInSMTLibFormatModified in Eliminator.cpp\n";
		  exit(1);
		}
	      int width = wit->second; // width obtained

	      fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written		

	      //cout<<"\n"<<label<<" printed "<<endl;

	      LabelTable.insert(make_pair(node, label));

	      //cout<<"\nentry ("<<node<<","<<label<<")"<<" added "<<endl;

// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";

	      return label;
	    }
	  else // operator
	    {
	      //cout<<"\nname=\t"<<name<<endl;    //debug code

	      if(name == "select")
		{
		  vector<string> ChildrenLabel;

		  iout++;

		  iout++;

		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
	  
		  iout = node->Get_outlist_start();

		  iout++;

		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));

		  iout = node->Get_outlist_start();

		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  int child_1_width, child_2_width, width;

		  child_1_width = atoi(ChildrenLabel[0].c_str());
		  child_2_width = atoi(ChildrenLabel[1].c_str());
		  width = child_1_width - child_2_width + 1;


		 //  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
// 		  if(wit == WidthTableForNodes.end())
// 		    {
// 		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
// 		      exit(1);
// 		    }
// 		  int width = wit->second; // width obtained

		  fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		  fprintf(fp,":assumption (= %s (extract [%s:%s] %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      // NB: Assumption here is that not_equal_to applies only between bit-vectors

	      else if(name == "is_not_equal" || name == "not_equal_to")
		{
		  vector<string> ChildrenLabel;
		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		  iout++;
		  ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

	          if(TypeTableOfNodes[node]=="bv") // should be written as bv
		    	{
			fprintf(fp,":assumption (iff  %s (not (= %s %s)))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
		  else // should be written as bool
		    	{
			fprintf(fp,":assumption (iff  %s (not (iff %s %s)))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
	  
		  
		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "equivalent" || name == "=")
		{
		  if(name ==  "equivalent") name = "=";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		  if(TypeTableOfNodes[node]=="bv") // should be written as bv
		    	{
			fprintf(fp,":assumption (iff %s (= %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
		  else	
			{
			fprintf(fp,":assumption (iff %s (iff %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
	  
		  

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
              else if(name == "bvuge" || name == "bvule" || name == "bvugt" || name == "bvult")
		{
		 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		 fprintf(fp,":assumption (iff %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
		
		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "bvmul" || name == "bvadd" || name == "bvsub")
		{
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  map<DAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		  if(wit == WidthTableForNodes.end())
		    {
		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes in function printInSMTLibFormatModified in Eliminator.cpp\n";
		      exit(1);
		    }
		  int width = wit->second; // width obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		  fprintf(fp,":assumption (= %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "and" || name == "or" || name == "not")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(name == "not")
		    {
		      fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		      fprintf(fp,":assumption (iff %s (not %s))\n",label.c_str(), ChildrenLabel[0].c_str()); // written
		      
		    }
                  else // and, or node
		    {
                          fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

                          if(ChildrenLabel.size()<2)
                          {
                              cout<<"\nand node with "<<ChildrenLabel.size()<<" children encountered in function printInSMTLibFormatModified in Eliminator.cpp\n";
                              exit(1);
                          }                      
                          else if(ChildrenLabel.size()==2) // Only two children
                          {
                         
                             fprintf(fp,":assumption (iff %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
                             
                          }
                          else //more than two children
                          {
                               fprintf(fp,":assumption (iff %s (%s",label.c_str(), name.c_str());
                               for(int LabelIndex=0; LabelIndex<ChildrenLabel.size(); LabelIndex++)
                               {
                                fprintf(fp," %s", ChildrenLabel[LabelIndex].c_str());   
                               }
                              fprintf(fp, "))");
                          }
                         
		    }// and, or node ends here

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}

	      else if(name == "ite" || name == "if_then_else")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(TypeTableOfNodes[node]=="bv") // should be written as bv; get the width
		    {
		      map<DAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		      if(wit == WidthTableForNodes.end())
			{
			  cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes  in function printInSMTLibFormatModified in Eliminator.cpp\n";
			  exit(1);
			}
		      int width = wit->second; // width obtained

		      fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		      fprintf(fp,":assumption (= %s (ite %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		      
		    }
		  else
		    {
		      fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		      fprintf(fp,":assumption (iff %s (if_then_else %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else
		{
		  cout<<"\nUnanticipated operator "<<name<<" inside function printInSMTLibFormatModified in Eliminator.cpp\n";
		  exit(1);
		}
		    
	    }
	}
    }
}

// Modified version of the function createWidthTableForOperators with dynamic programming
void createWidthTableForOperatorsWithDP(DAGNode *tree, map<DAGNode*, int> &WidthTableForNodes, map<string, int> &WidthTable)
{
  if(WidthTableForNodes.find(tree) != WidthTableForNodes.end()) // already width found
    return;

  string name = tree->Get_node_name();
  unsigned int outlist_size = tree->Get_outlist_size();
  Value* MyValue = (Value*)(tree->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int ntype = MyType.data_type;
  list<DAGNode*>::iterator iout = tree->Get_outlist_start();


   if(!(ntype == BITVECTOR_CONSTANT || ntype == LOGICAL_CONSTANT || ntype == INTEGER_CONSTANT) && outlist_size == 0) // variable
    {
    
      int Width =  WidthTable[name];
      WidthTableForNodes.insert(make_pair(tree, Width));
    }

  else
    {

    switch(outlist_size)
      {
      case 1:
	{
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);

	  if(name == "not")
	    {
	      int Width =  1;
	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	     cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	     exit(1);
	    }
 
	 
	  break;
	}
      case 2:
	{
          DAGNode *child1 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child2 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	 
	  if(name == "equivalent" || name == "=" || name == "bvadd" || name == "bvmul" || name == "is_not_equal" || name == "not_equal_to" || name == "concat" || name == "bvsub" || name == "bvule" || name == "bvuge" || name == "bvult" || name == "bvugt")
	    {

	      int Width1 =  WidthTableForNodes[child1];
	      int Width2 =  WidthTableForNodes[child2];

	      int Width;

	      if(name == "equivalent" || name == "=" || name == "bvadd" || name == "bvmul" || name == "is_not_equal" || name == "not_equal_to"|| name == "bvsub" || name == "bvule" || name == "bvuge" || name == "bvult" || name == "bvugt")
		Width = findMaximum(Width1, Width2);
	      else // concat
		Width = Width1 + Width2;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	  else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
          else if(name == "bvor" || name == "bvand")
	    {
	      int Width = WidthTableForNodes[child1];

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	      exit(1);
	    }

	  break;
	}
      case 3:
	{
	  DAGNode *child1 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child2 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child3 = *iout;
	  createWidthTableForOperatorsWithDP(*iout, WidthTableForNodes, WidthTable);

	  if(name == "ite" || name == "if_then_else") // Width of an ite is in fact maximum of widths of child2 and child3
	    {
	      
	      //int Width = 1;

	       int Width1 =  WidthTableForNodes[child2];
	       int Width2 =  WidthTableForNodes[child3];

	      int Width = findMaximum(Width1, Width2);

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else if(name == "select")
	    {
	      int Width1 =  WidthTableForNodes[child2];
	      int Width2 =  WidthTableForNodes[child3];

	      int Width = Width2 - Width1 +1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	   else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	      exit(1);
	    }

	 
	  break;
	}
      case 0: // constants
	{
	  if(name[0] == 'x') // hex
	    {
	      WidthTableForNodes.insert(make_pair(tree, (name.length()-3)*4));
	    }
	  else // 
	    {
	      int Width = findMinimumWidthRequired(name);

	      WidthTableForNodes.insert(make_pair(tree, Width));

	    }
	break;
	}// case 0 ends here

       default :
	{
          for(int i=0; i<outlist_size; i++)
		{
		DAGNode* child = *iout;
		createWidthTableForOperatorsWithDP(child, WidthTableForNodes, WidthTable);
		iout++;
		}
	if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	      exit(1);
	    }

	 
	  break;
	}//default ends here
      }//switch

    }//else
}//function


 
// Modified version of the function propogateWidths function with dynamic programming  
void propogateWidthsWithDP(DAGNode *tree, map<DAGNode*, int> &WidthTableForNodes, int WidthFromParent, set<DAGNode*> &DPTable)
{
  if(DPTable.find(tree) != DPTable.end()) return;

  string name = tree->Get_node_name();
  unsigned int outlist_size = tree->Get_outlist_size();
  Value* MyValue = (Value*)(tree->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int ntype = MyType.data_type;
  list<DAGNode*>::iterator iout = tree->Get_outlist_start();


  

   if(!(ntype == BITVECTOR_CONSTANT || ntype == LOGICAL_CONSTANT || ntype == INTEGER_CONSTANT) && outlist_size == 0) // variable
    {
    
      // Do nothing : Variable's width should not be changed
    }

  else
    {

    switch(outlist_size)
      {
      case 1:
	{
	  int width = WidthTableForNodes[tree];
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);

	  break;
	}
      case 2:
	{
	  if(name == "bvadd" || name == "bvmul")
	    {
	      WidthTableForNodes[tree] = WidthFromParent;
	    }

	  int width = WidthTableForNodes[tree];
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	  iout++;
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	 
	  break;
	}
      case 3:
	{
	  int width = WidthTableForNodes[tree];
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	  iout++;
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	  iout++;
	  propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
	 
	  break;
	}
      case 0: // constants
	{
	  // For all constants, width field is set
	  // But note that width field makes sense only for bit-vector constants
	  //WidthTableForNodes[tree] = WidthFromParent;	  
	}
      
      default:
	{
	  int width = WidthTableForNodes[tree];
	   
          for(int i=0; i<outlist_size; i++)
		{
                propogateWidthsWithDP(*iout, WidthTableForNodes, width, DPTable);
		iout++;
		}
	}//default
      }//switch

    }//else

   DPTable.insert(tree);
}//function



// Function to set properly, the types of the nodes in the dag rooted at "tree" 
string setTypesWithDP(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfNodes)
{
  HashTable<string, string> HTable;
  HashTable<string, string> *PtrHTable=&HTable;

  return setTypesWithDPInternal(tree, context, TypeTableOfNodes, PtrHTable);

}


// Function called by the function setTypesWithDP to set the types of the nodes 
string setTypesWithDPInternal(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfNodes, HashTable<string, string> *PtrHTable)
{
  // check if the node tree is encountered with the same context before

  // create the key

  string key;
  char temp_char[10];
  sprintf(temp_char, "%x", tree);
  string tree_addr(temp_char);
  key = tree_addr + context; 

  HTStatusValue<string> result = PtrHTable->hashtable_search(key);

  if(result.success()) // hash table hit
    {
      return result.GetValue();
    }
  
  else // hash table miss
    {

      string name = tree->Get_node_name();
      unsigned int outlist_size = tree->Get_outlist_size();
      Value* MyValue = (Value*)(tree->getValue());
      Type MyType = MyValue->get_node_type();
      int MyWidth = MyType.width;
      int node_type = MyType.data_type;
      list<DAGNode*>::iterator iout = tree->Get_outlist_start();
      string TreeType;
  

      if(!(node_type == BITVECTOR_CONSTANT || node_type == LOGICAL_CONSTANT || node_type == INTEGER_CONSTANT) && outlist_size == 0)//variable
	{
	  TreeType = "bv";
	}
      else
	{

	  //cout<<"\nNAME=\t"<<name<<endl;    //debug code

	  if(name == "select")
	    {
	      iout++;

	      iout++;
	  
	      setTypesWithDPInternal(*iout, "const", TypeTableOfNodes, PtrHTable);

	      iout = tree->Get_outlist_start();

	      iout++;

	      setTypesWithDPInternal(*iout, "const", TypeTableOfNodes, PtrHTable);

	      iout = tree->Get_outlist_start();

	      setTypesWithDPInternal(*iout, "bv", TypeTableOfNodes, PtrHTable);

	      TreeType = "bv";

	    }
      
	  else if(name == "is_not_equal" || name == "not_equal_to")
	    {
	     string ChildType;

	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  ChildType = setTypesWithDPInternal(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TypeTableOfNodes.insert(make_pair(tree, ChildType));

	      TreeType = "bool";
	    }

	  // NB: Assumption here is that equivalent applies only between bit-vectors

          else if(name == "equivalent" || name == "=" || name == "bvuge" || name == "bvule" || name == "bvugt" || name == "bvult")
	    {
	    string ChildType;

	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  ChildType = setTypesWithDPInternal(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

              TypeTableOfNodes.insert(make_pair(tree, ChildType));

	      TreeType = "bool";
	    }

      
	  else if(name == "bvmul")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bv";

	    }

	  else if(name == "bvadd")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bv";

	    }


	  // Newly added on 20.1.2010

	  else if(name == "bvsub")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bv";

	    }

      
	  else if(name == "and" || (name == "or") || (name == "not"))
	    {
	  
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal(*iout, "bool", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bool";
	    }

	  else if(name == "ite" || name == "if_then_else")
	    {
	      setTypesWithDPInternal(*iout, "bool", TypeTableOfNodes, PtrHTable);

	      iout++;

	      string ChildType;

	      for(unsigned int i=1; i<outlist_size; i++,iout++)
		{
		  ChildType = setTypesWithDPInternal(*iout, "bool", TypeTableOfNodes, PtrHTable);
		}

	      TypeTableOfNodes.insert(make_pair(tree, ChildType));

	      TreeType = ChildType;

	    }

	  else if(name == "'1'")
	    {
	      TreeType = "bv";
	    }

	  else if(name == "'0'")
	    {
	      TreeType = "bv";
	    }

	  else if(outlist_size == 0) // Other constants

	    {
		
	      if(context == "const")
		{
		  TreeType = "const";
		}
	      else if(context == "bv")
		{
		  TreeType = "bv";	
		}
	      else if(context == "bool")
		{
		  if(name == "1") TreeType = "bool";
		  else if(name == "0") TreeType = "bool";
		  else if(name == "true") TreeType = "bool";
		  else if(name == "false") TreeType = "bool";

		  else // interpreting a Boolean constant other than true/false as a bit-vector
		    {
		      if(name[0] == 'x') // hex
			{
			  TreeType = "bv";
			}
		      else // Decimal
			{
			  TreeType = "bv";
			}
		    }

		}
	      else
		{
		  cout<<"\nUnanticipated constant "<<name<<" inside function setTypesWithDPInternal in Eliminator.cpp\n";
		  exit(1);
		}
		  
	    }
	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function setTypesWithDPInternal in Eliminator.cpp\n";
	      exit(1);
	    }
		    
	}// else 

      result = PtrHTable->hashtable_insert(key, TreeType);
   
      if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function setTypesWithDPInternal in Eliminator.cpp\n";
	  exit(1);
	}

      return TreeType;

    }// hash table miss

}


// Function to write the declarations in SMTLIB format for the selected set of 
// variables in the list "VarNames"
void writeBVDeclarationsInSMTLibFormatWithLookup(FILE *smt_file, map<string, int> &WidthTable, list<string> &VarNames)
{
  for(list<string>::iterator it=VarNames.begin(); it!=VarNames.end(); it++)
    {
      map<string, int>::iterator wt_it = WidthTable.find(*it);
      if(wt_it == WidthTable.end())
	{
	 cout<<"\nInside function writeBVDeclarationsInSMTLibFormatWithLookup in Eliminator. No width table entry found for "<<*it<<endl;
	 exit(1);
	}
      fprintf(smt_file,":extrafuns ((%s",(*it).c_str());
      fprintf(smt_file," BitVec[%d]))\n",wt_it->second);
    }

}

// Returns true if exists(X, original) => eliminated i.e. original \and \neg eliminated is unsat; false otherwise
// Underlying SMT-Solver used is STP, not Yices.
bool checkForExactnessInOneDirectionUsingSTP(DAGNode* original, DAGNode* eliminated, map<string, int> &WidthTable)
{

  DAGNode *Final;

  Final = createDAG("and", original, createDAG("not", eliminated, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);

  bool result = ThUnsatSingleFormulaInSMTLibFormatAsDAG(Final, WidthTable);

  if(result)//unsat
    {
      cout<<"\nexists(X,f)=>f' holds\n";
      return true;
     }
      
  else //sat
    { cout<<"\nexists(X,f)=>f' does not hold\n";
      return false;
    }
 //Other cases are alreday taken care
}



// Get the #equations, #disequations, etc. in the dag "node", and
// set global variables accordingly
void getEquationsDisequationsCounts(DAGNode* node, map<string, int> &WidthTable)
{
set<DAGNode*> EquationSet;
set<DAGNode*> DisequationSet;
char status='t';
set<string> AlreadyEncountered;

//cout<<"\nnode = "<<getDAGStr(node)<<endl;

getEquationsAndDisequationsInternal(node, EquationSet, DisequationSet, status, AlreadyEncountered);  

showAPSet("EquationSet", EquationSet);
showAPSet("DisequationSet", DisequationSet);

cout<<"\nRemoving trivial disequations\n";

set<DAGNode*> EquationSetTemp;
set<DAGNode*> DisequationSetTemp;

for(set<DAGNode*>::iterator it = EquationSet.begin(); it != EquationSet.end(); it++)
   {
     string var = getSingleVariable(*it);
     if(var != "")
       {
	 EquationSetTemp.insert(*it);
       }     
   }
for(set<DAGNode*>::iterator it = DisequationSet.begin(); it != DisequationSet.end(); it++)
   {
     string var = getSingleVariable(*it);
     if(var != "")
       {
	 DisequationSetTemp.insert(*it);
       }     
   }

EquationSet = EquationSetTemp;
DisequationSet = DisequationSetTemp;

showAPSet("EquationSet", EquationSet);
showAPSet("DisequationSet", DisequationSet);


set<DAGNode*> BitLevelDisequationSet;
getBitLevelDisequations(DisequationSet, WidthTable, BitLevelDisequationSet);
showAPSet("BitLevelDisequationSet", BitLevelDisequationSet);

set<DAGNode*> NonBitLevelDisequationSet;
set_difference(DisequationSet.begin(), DisequationSet.end(), BitLevelDisequationSet.begin(), BitLevelDisequationSet.end(), inserter(NonBitLevelDisequationSet, NonBitLevelDisequationSet.begin()));


EquationCount_In_DAG = EquationSet.size();
DisequationCount_In_DAG = DisequationSet.size();
BitLevel_DisequationCount_In_DAG = BitLevelDisequationSet.size();
NonBitLevel_DisequationCount_In_DAG = NonBitLevelDisequationSet.size();

set<DAGNode*> EquationSetWithSelsConcats;
set<DAGNode*> BitLevelDisequationSetWithSelsConcats;
set<DAGNode*> NonBitLevelDisequationSetWithSelsConcats;

for(set<DAGNode*>::iterator it = EquationSet.begin(); it != EquationSet.end(); it++)
   {
     bool has_selects_concats=false;
     checkForSelectsConcats(*it, has_selects_concats);
     if(has_selects_concats)
       {
	 EquationSetWithSelsConcats.insert(*it);
       }     
   }
    
for(set<DAGNode*>::iterator it = BitLevelDisequationSet.begin(); it != BitLevelDisequationSet.end(); it++)
   {
    bool has_selects_concats=false;
    checkForSelectsConcats(*it, has_selects_concats);

     if(has_selects_concats)
       {
	 BitLevelDisequationSetWithSelsConcats.insert(*it);
       }     
   }

for(set<DAGNode*>::iterator it = NonBitLevelDisequationSet.begin(); it != NonBitLevelDisequationSet.end(); it++)
   {
     bool has_selects_concats=false;
     checkForSelectsConcats(*it, has_selects_concats);

     if(has_selects_concats)
       {
	 NonBitLevelDisequationSetWithSelsConcats.insert(*it);
       }     
   }

EquationWithSelsConcatsCount_In_DAG = EquationSetWithSelsConcats.size();
BitLevel_DisequationWithSelsConcatsCount_In_DAG = BitLevelDisequationSetWithSelsConcats.size();
NonBitLevel_DisequationWithSelsConcatsCount_In_DAG = NonBitLevelDisequationSetWithSelsConcats.size();

    

cout<<"\nEquationCount_In_DAG = "<<EquationCount_In_DAG<<endl;
cout<<"DisequationCount_In_DAG = "<<DisequationCount_In_DAG<<endl;
cout<<"BitLevel_DisequationCount_In_DAG = "<<BitLevel_DisequationCount_In_DAG<<endl;
cout<<"NonBitLevel_DisequationCount_In_DAG = "<<NonBitLevel_DisequationCount_In_DAG<<endl;
cout<<"EquationWithSelsConcatsCount_In_DAG = "<<EquationWithSelsConcatsCount_In_DAG<<endl;
cout<<"BitLevel_DisequationWithSelsConcatsCount_In_DAG = "<<BitLevel_DisequationWithSelsConcatsCount_In_DAG<<endl;
cout<<"NonBitLevel_DisequationWithSelsConcatsCount_In_DAG = "<<NonBitLevel_DisequationWithSelsConcatsCount_In_DAG<<endl;

}

// Function to obtain the set of equations and the set of disequations in the dag node rooted at "node"
void getEquationsAndDisequationsInternal(DAGNode* node, set<DAGNode*> &EquationSet, set<DAGNode*> &DisequationSet, char status, set<string> &AlreadyEncountered)
{
  char node_char[100];
  sprintf(node_char , "%x", node);
  string node_str(node_char);
      
  string key = node_str;
  key += status;

  // key includes address of node concatenated with status

  if(AlreadyEncountered.find(key)!=AlreadyEncountered.end()) // node is already encountered with the same status
    {
      return;
    }
           
  string MyName = node->Get_node_name();

  if(MyName == "=" || MyName == "equivalent") // Equation node encountered
    {
      if(status=='x')
	{
	  // encountered in both original and negated form
	  EquationSet.insert(node);
	  DisequationSet.insert(node); // we need only the count of Equation, Disequation nodes. Hence fine even if we do not negate
	}
      else if(status=='t')
	{
	  // encountered in original form
	  EquationSet.insert(node);
	}
      else if(status=='f')
	{
	  // encountered in negated form
	  DisequationSet.insert(node);
	}
      else
	{
	  cout<<"\nUndefined status "<<status<<" in function getEquationsAndDisequationsInternal in Eliminator.cpp\n";
	  exit(1);
	}
    }
  else if(MyName == "is_not_equal" || MyName == "not_equal_to") // LME node encountered
    {
      if(status=='x')
	{
	  // encountered in both original and negated form
	  EquationSet.insert(node);
	  DisequationSet.insert(node); // we need only the count of Equation, Disequation nodes. Hence fine even if we do not negate
	}
      else if(status=='t')
	{
	  // encountered in original form
	  DisequationSet.insert(node);
	}
      else if(status=='f')
	{
	  // encountered in negated form
	  EquationSet.insert(node);
	}
      else
	{
	  cout<<"\nUndefined status "<<status<<" in function getEquationsAndDisequationsInternal in Eliminator.cpp\n";
	  exit(1);
	}

    }      
  else if(MyName == "if_then_else")  
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      getEquationsAndDisequationsInternal(*it, EquationSet, DisequationSet, 'x', AlreadyEncountered);
      it++;
      getEquationsAndDisequationsInternal(*it, EquationSet, DisequationSet, status, AlreadyEncountered);
      it++;
      getEquationsAndDisequationsInternal(*it, EquationSet, DisequationSet, status, AlreadyEncountered);
    }
  else if(MyName == "not")  
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      if(status=='x')
	{
	  getEquationsAndDisequationsInternal(*it, EquationSet, DisequationSet, status, AlreadyEncountered);
	}
      else if(status=='t')
	{
	  getEquationsAndDisequationsInternal(*it, EquationSet, DisequationSet, 'f', AlreadyEncountered);
	}
      else if(status=='f')
	{
	  getEquationsAndDisequationsInternal(*it, EquationSet, DisequationSet, 't', AlreadyEncountered);
	}
      else
	{
	  cout<<"\nUndefined status "<<status<<" in function getEquationsAndDisequationsInternal in Eliminator.cpp\n";
	  exit(1);
	}

    }
  else
    {	  
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      for(int i=0; i<node->Get_outlist_size(); i++)
	{
	  getEquationsAndDisequationsInternal(*it, EquationSet, DisequationSet, status, AlreadyEncountered);
	  it++;
	}
    }


  AlreadyEncountered.insert(key);
	
}        

 
// Function to put the bit-level disequations in "DisequationSet" into "BitLevelDisequationSet"
void getBitLevelDisequations(set<DAGNode*> &DisequationSet, map<string, int> &WidthTable, set<DAGNode*> &BitLevelDisequationSet)
{
  for(set<DAGNode*>::iterator it = DisequationSet.begin(); it != DisequationSet.end(); it++)
    {
      if(getWidthOfLMDEWithSelects(*it, WidthTable) == 1)
	{
	  BitLevelDisequationSet.insert(*it);
	}
    }
}


// This function sets the variable has_selects_concats to true if the dag
// rooted at "node" has selects / concats inside it
void checkForSelectsConcats(DAGNode* node, bool &has_selects_concats)
{
  string MyName = node->Get_node_name();

  if(MyName == "concat" || MyName == "select") // node to be linearized encountered
    {
      has_selects_concats = true;
    }
                   
  else 
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
	        	
      for(int i=0; i<node->Get_outlist_size(); i++)
	{
	  checkForSelectsConcats(*it, has_selects_concats);
	  it++;
	}
    }
	
}        

// Same as updateVarSetToElim(set<string> &VarSetToElim, DAGNode *node) except that
// the result is obtained in parameters ImpureVarsToQElim and PureVarsToQElim
void updateVarSetToElim(set<string> &VarSetToElim, DAGNode *node, set<string> &ImpureVarsToQElim, set<string> &PureVarsToQElim)
{
  set<string> ImpureVars;
  getImpureVariables(node, ImpureVars);
  showSet(ImpureVars, "ImpureVars");
  
  set_difference(VarSetToElim.begin(), VarSetToElim.end(), ImpureVars.begin(), ImpureVars.end(),inserter(PureVarsToQElim, PureVarsToQElim.begin()));
  set_intersection(VarSetToElim.begin(), VarSetToElim.end(), ImpureVars.begin(), ImpureVars.end(),inserter(ImpureVarsToQElim, ImpureVarsToQElim.begin()));

  showSet(ImpureVarsToQElim, "ImpureVarsToQElim");
  showSet(PureVarsToQElim, "PureVarsToQElim");

  ImpureVarsToQElimCount = ImpureVarsToQElim.size();
  PureVarsToQElimCount = PureVarsToQElim.size();

}


// Functions added from 22 July 2011 onwards to implement Monniaux style QE on dags with LMCs and sels, concats

// Same as callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with support for concatenation 
// Use this function in the argument dag has concats in it
string callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(DAGNode *root, map<string, int> &WidthTable)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperators(root, WidthTableForNodes, WidthTable);

  propogateWidths_WithSupportForSelsConcats(root, WidthTableForNodes, 1);

  return getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(root, "bool", WidthTableForNodes, 1, WidthTable);
}


// Modified version of getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with handling of concat (&) added : Presently & is handled in the same way as in SSimulator/CDAGNode.cpp::getDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable
string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<string, int> &WidthTable)
{

  string name = node->Get_node_name();

  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)
        {
               dag_str = name;
               return dag_str;
        }
        else
          {

            if(name == "select")
              {
                dag_str = "(bv-extract ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();

                iout++;

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "const", WidthTableForNodes, 1, WidthTable) + " ";

                iout = node->Get_outlist_start();

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "const", WidthTableForNodes, 1, WidthTable) + " ";

                iout = node->Get_outlist_start();

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "concat")
	      {
		
		// The only type of concat the function currently properly supports are of the type variable @ ... @  variable. There can be width problems in the other cases
		bool variable_concatenation = isVariableConcatenation(node);// This is true only if either each child is a variable or concat itself. i.e. variable_concatenation is true only if this node is of the form variable 1 @ variable 2 @ ...@ variable n

		list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();


		if(!variable_concatenation) 
		  {
		    
		    DAGNode* child1;
		    DAGNode *child2;

		    child1 = *iout;
		    iout++;
		    child2 = *iout;
		    
		    
		    if(isConstant(child1)) // it is like constant @ some node : constants create problems as the width is not known. Hence infer width from the other node.
		      {
			dag_str = "(bv-concat ";
			int child2_size = getWidthOfNode(child2, WidthTable);
			dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(child1, "bv", WidthTableForNodes, WidthTableForNodes[node] - child2_size, WidthTable) + " ";
			dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(child2, "bv", WidthTableForNodes, child2_size, WidthTable) + " ";
			dag_str += ")";
			return dag_str; 
		      }
		    else if(isConstant(child2)) // it is like some node  @ constant : constants create problems as the width is not known. Hence infer width from the other node.
		      {
			dag_str = "(bv-concat ";
			int child1_size = getWidthOfNode(child1, WidthTable);
			dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(child1, "bv", WidthTableForNodes, child1_size, WidthTable) + " ";
			dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(child2, "bv", WidthTableForNodes, WidthTableForNodes[node] - child1_size, WidthTable) + " ";
			dag_str += ")";
			return dag_str; 
			
		      }
		    else  // it is like some node  @ some node : There can be problem of width here if "some node" has constants
		      {
			dag_str = "(bv-concat ";
			int child1_size = getWidthOfNode(child1, WidthTable);
			dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(child1, "bv", WidthTableForNodes, child1_size, WidthTable) + " ";
			int child2_size = getWidthOfNode(child2, WidthTable);
			dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(child2, "bv", WidthTableForNodes, child2_size, WidthTable) + " ";
			dag_str += ")";
			return dag_str; 
		      }

		  }//if(!variable_concatenation)

		else // @ of the type variable @ ... @  variable. No need to worry about width
		  {

		    dag_str = "(bv-concat ";

		    for(unsigned int i=0; i<outlist_size; i++, iout++)
		      {
			dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
		      }

		    dag_str += ")";

		    return dag_str; 
		  }//else of if(!variable_concatenation)
              }//else if (name == "concat")


	    else if(name == "bvmul")
	      {
		dag_str = "(bv-mul ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "bvadd")
	      {
		dag_str = "(bv-add ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	     else if(name == "bvsub")
	      {
		dag_str = "(bv-sub ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "if_then_else"))
	      {
                if(name == "if_then_else") name = "ite";

		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {

                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);
		
              }

	    else if(name == "'0'")
              {
                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

              }

	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      return dag_str;
		      }
		else if(context == "bv")
		      {
	
			if(name[0] == 'x') // hex
			  {
			  dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);
			  }
			else // Decimal
			  {
			  dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);
			  }
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
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
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForSelsConcats\n";
		exit(1);
	      }
		    
	  }
}

// Returns true if the "node" is a variable; false otherwise
bool isVariable(DAGNode *node)
{
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyDataType = MyType.data_type;

  if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && MyDataType != REAL_CONSTANT && node->Get_outlist_size() == 0)
    {
      return true;
    }
  return false;
}

// Returns true if the node is a concatenation of variables such as v1 @ ... @ vn
bool isVariableConcatenation(DAGNode *node)
{
if(!isVariable(node) && node->Get_node_name() != "concat") // not a variable and not a concat
  {
    return false;
  }
 if(node->Get_node_name() == "concat") // concat
  {
    list<DAGNode*>::iterator iout = node->Get_outlist_start();
    unsigned int outlist_size = node->Get_outlist_size();
    
    for(unsigned int i=0; i<outlist_size; i++, iout++)
      {
	if(!isVariableConcatenation(*iout))
	  {
	    return false;
	  }
      }
    return true;
  }
 else // variable
   {
     return true;
   }
} 
    
// Same as propogateWidths with support for concats
void propogateWidths_WithSupportForSelsConcats(DAGNode *node, map<DAGNode*, int> &WidthTableForNodes, int WidthFromParent)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

  if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0) // variable
    {
    
      // Do nothing : Variable's width should not be changed
    }

  else
    {

    list<DAGNode*>::iterator iout = node->Get_outlist_start();
    unsigned int outlist_size = node->Get_outlist_size();

    switch(outlist_size)
      {
      case 1:
	{
	  int width = WidthTableForNodes[node];
	  propogateWidths_WithSupportForSelsConcats(*iout, WidthTableForNodes, width);

	  break;
	}
      case 2:
	{
	  if(name == "bvadd" || name == "bvmul"|| name == "concat")
	    {
	      WidthTableForNodes[node] = WidthFromParent;
	    }

	  int width = WidthTableForNodes[node];
	  propogateWidths_WithSupportForSelsConcats(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths_WithSupportForSelsConcats(*iout, WidthTableForNodes, width);
	 
	  break;
	}
      case 3:
	{
	  int width = WidthTableForNodes[node];
	  propogateWidths_WithSupportForSelsConcats(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths_WithSupportForSelsConcats(*iout, WidthTableForNodes, width);
	  iout++;
	  propogateWidths_WithSupportForSelsConcats(*iout, WidthTableForNodes, width);
	 
	  break;
	}
      case 0: // constants
	{
	  // For all constants, width field is set
	  // But note that width field makes sense only for bit-vector constants
          break;
	}
       default:
	{
	  int width = WidthTableForNodes[node];
	   
          for(int i=0; i<outlist_size; i++)
		{
                propogateWidths_WithSupportForSelsConcats(*iout, WidthTableForNodes, width);
		iout++;
		}
	}
      }

    }
}




// lmdeEvaluatorModified_With_Concat_Support is lmdeEvaluatorWithBinaryModel except that,
// it handles selections of the form expression[integer:integer], and concatenations
bvatom lmdeEvaluatorModified_With_Concat_Support(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result, map<string, int> &WidthTable)
    {
      bvatom bv_result;
      string dag_str;
      int outlist_size;

      dag_str = node->Get_node_name();
      outlist_size = node->Get_outlist_size();
      Value* MyValue = (Value*)(node->getValue());
      Type MyType = MyValue->get_node_type();
      int MyWidth = MyType.width;
      int MyDataType = MyType.data_type;

      //cout<<"\n"<<dag_str<<" node encountered in lmdeEvaluatorModified_With_Concat_Support\n";


      if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && outlist_size==0)  //i.e. if a variable node
	{
	  map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(dag_str);
	  if(Variable_Value_Map_it == Variable_Value_Map.end())
	    {
	      cout<<"\nNo entry in Variable_Value_Map for variable "<<dag_str<<" in function lmdeEvaluatorModified_With_Concat_Support in Eliminator.cpp\n";
	      exit(1);
	    }
	  string bin_value = Variable_Value_Map_it->second; // binary value of the variable from map 

	  bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()

	  //cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

	  if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
	    {
	      //cout<<"\nbin_value.size() < width\n";
	      bv_result = bvm->BVExtend(result, width, 0);
	    }
	  else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
	    {
	      //cout<<"\nbin_value.size() < width\n";
	      bv_result = bvm->select_with_int(result, width-1, 0);
	    }
	  else
	    {
	      //cout<<"\nbin_value.size() = width\n";
	      bv_result = result;
	    }


	  //cout<<"\nbv_result = "<<endl;
	  //bvm->printBVatom(bv_result, 0);

	} //operand node ends here

      else   //i.e. if an operator node or a constant node
        {
            list<DAGNode*>::iterator iout = node->Get_outlist_start();
	    
	    if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained
		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained
		  if (inter_bool_result) { // reverse the result as it is is_not_equal
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		    }
		}// "is_not_equal"/"not_equal_to" ends here

	      else if(dag_str == "equivalent" || dag_str == "=")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

		  if (inter_bool_result) { // no need to reverse
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  

		  }
		}// "equivalent"/"=" ends here

	      else if(dag_str == "bvmul")
		{
		  if((*iout)->Get_node_name()=="1")// multiplication by 1
		    {
		      //cout<<"\nmultiplication by 1\n";
		      iout++;
		      bv_result = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained - no need to select. op2 is already of proper width
		    }
		  else
		    {

		      bvatom op1 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		      iout++;
		      bvatom op2 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		      bvatom result = bvm->arith_mult(op1, op2); // result obtained
		      bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
		    } 
		}// "bvmul" ends here

	    else if(dag_str == "bvadd")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bvatom result = bvm->arith_add(op1, op2); // result obtained
		  bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
 
		}// "bvadd" ends here
	    else if(dag_str == "select")
	      {
		DAGNode *operand = *iout;
		iout++;
		DAGNode *low_sel = *iout;
		iout++;
		DAGNode *high_sel = *iout;

                // Find the width needed for the operand part
		int width_operand = getWidthOfNode(operand, WidthTable);
		bv_result = lmdeEvaluatorModified_With_Concat_Support(operand, width_operand, Variable_Value_Map, boolean_result, WidthTable); // operand's part obtained
		//cout<<"\nbv_result = "<<endl;
		//bvm->printBVatom(bv_result, 0);

		string low_sel_string = low_sel->Get_node_name();
		string high_sel_string = high_sel->Get_node_name();

		int low_sel_number = atoi(low_sel_string.c_str());
		int high_sel_number = atoi(high_sel_string.c_str());
 
		// Now we have obtained the bv_result from the operand part; let us take the selection part also

		bv_result = bvm->select_with_int(bv_result, high_sel_number, low_sel_number);

	      } //"select" ends here

	    else if(dag_str == "concat")
	      {
		if(outlist_size > 2)
		  {
		    cout<<"\nPresently concatenations of number of children more than two are not supported by the function lmdeEvaluatorModified_With_Concat_Support in Eliminator.cpp\n";
		    exit(1);
		  }

		int width_node = getWidthOfNode(node, WidthTable);

		// The only type of concat the function currently properly supports are of the type variable @ ... @  variable. There can be width problems in the other cases
		bool variable_concatenation = isVariableConcatenation(node);// This is true only if either each child is a variable or concat itself. i.e. variable_concatenation is true only if this node is of the form variable 1 @ variable 2 @ ...@ variable n

		if(!variable_concatenation) 
		  {
		    
		    DAGNode* child1;
		    DAGNode *child2;

		    child1 = *iout;
		    iout++;
		    child2 = *iout;
		    
		    
		    if(isConstant(child1)) // it is like constant @ some node : constants create problems as the width is not known. Hence infer width from the other node.
		      {
			int child2_size = getWidthOfNode(child2, WidthTable);
			bvatom bin_value_1;
			bvatom bin_value_2;
			bin_value_1 = lmdeEvaluatorModified_With_Concat_Support(child1, width_node - child2_size, Variable_Value_Map, boolean_result,WidthTable);
			bin_value_2 = lmdeEvaluatorModified_With_Concat_Support(child2, child2_size, Variable_Value_Map, boolean_result,WidthTable);
			bv_result = bvm->concat(bin_value_1,bin_value_2);
		      }
		    else if(isConstant(child2)) // it is like some node  @ constant : constants create problems as the width is not known. Hence infer width from the other node.
		      {
			int child1_size = getWidthOfNode(child1, WidthTable);
			bvatom bin_value_1;
			bvatom bin_value_2;
			bin_value_1 = lmdeEvaluatorModified_With_Concat_Support(child1, child1_size, Variable_Value_Map, boolean_result,WidthTable);
			bin_value_2 = lmdeEvaluatorModified_With_Concat_Support(child2, width_node - child1_size, Variable_Value_Map, boolean_result,WidthTable);
			bv_result = bvm->concat(bin_value_1,bin_value_2);			
		      }
		    else  // it is like some node  @ some node : There can be problem of width here if "some node" has constants
		      {
			int child1_size = getWidthOfNode(child1, WidthTable);
			int child2_size = getWidthOfNode(child2, WidthTable);
			bvatom bin_value_1;
			bvatom bin_value_2;
			bin_value_1 = lmdeEvaluatorModified_With_Concat_Support(child1, child1_size, Variable_Value_Map, boolean_result,WidthTable);
			bin_value_2 = lmdeEvaluatorModified_With_Concat_Support(child2, child2_size, Variable_Value_Map, boolean_result,WidthTable);
			bv_result = bvm->concat(bin_value_1,bin_value_2);
		      }

		  }//if(!variable_concatenation)

		else // @ of the type variable @ ... @  variable. No need to worry about width
		  {
		  		    
		  DAGNode* child1;
		  DAGNode *child2;

		  child1 = *iout;
		  iout++;
		  child2 = *iout;

		  int child1_size = getWidthOfNode(child1, WidthTable);
		  int child2_size = getWidthOfNode(child2, WidthTable);
		  bvatom bin_value_1;
		  bvatom bin_value_2;
		  bin_value_1 = lmdeEvaluatorModified_With_Concat_Support(child1, child1_size, Variable_Value_Map, boolean_result,WidthTable);
		  bin_value_2 = lmdeEvaluatorModified_With_Concat_Support(child2, child2_size, Variable_Value_Map, boolean_result,WidthTable);
		  bv_result = bvm->concat(bin_value_1,bin_value_2);		    
		  }//else of if(!variable_concatenation)
	      }//"concat" ends here


	    else if(outlist_size==0) // constants
	      {

		ULINT dec_value = strtoull(dag_str.c_str(), NULL, 0); // decimal value of the constant obtained from the dag node
		string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
		bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

		if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
		  {
		    bv_result = bvm->BVExtend(result, width, 0);
		  }
		else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		  {
		    bv_result = bvm->select_with_int(result, width-1, 0);
		  }
		else
		  {
		    bv_result = result;
		  }

	      }// if a constant node ends here
	    else
	      {
		cout<<"\nOperator "<<dag_str<<" is not handled in function lmdeEvaluatorModified_With_Concat_Support in Eliminator.cpp\n";
		exit(1);
	      }// else case
	    

	}//if an operator node or a constant node ends here


      //cout<<"\nbv_result from "<<dag_str<<" for lmdeEvaluatorModified_With_Concat_Support = ";
      //bvm->printBVatom(bv_result, 0);
  
  return bv_result;
		    
}// function ends here


// Same as checkEntailment with support for selects and concats
bool checkEntailment_WithSupportForSelsConcats(DAGNode *constraint, map<string, int> &WidthTable, map<string, string> &Model)
{
  // find width

  // constraint is equality or disequality
  // hence, width(constraint) = width(lhs);

  list<DAGNode*>::iterator iout = constraint->Get_outlist_start();

  DAGNode* lhs = *iout;

  int width = getWidthOfNode(lhs, WidthTable);
  
  bool boolean_result;

  //cout<<"\n\nChecking entailment of "<<getDAGStr(constraint)<<endl<<endl;

  bvatom bv_result = lmdeEvaluatorModified_With_Concat_Support(constraint, width, Model, boolean_result, WidthTable);

  return boolean_result;
}


// Given a set of LMCs in "Constraints" and a set of variables to be eliminated, generate two sets of constraints - 
// A set of constraints free of the variables to be eliminated and another set of constraints which contain
// the variables to be eliminated
void getFreeAndBoundConstraintsForMultipleVariables(vector<DAGNode*> &Constraints, set<string> &Variables, set<DAGNode*> &Free_Constraints, set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable)
{
  for(int i=0; i<Constraints.size() ; i++)
    {
      set<string> Support;

      DAGNode* root = Constraints[i];

      getDAGNames(root, Support);

      set<string> CommonVariables;

      set_intersection(Support.begin(), Support.end(), Variables.begin(), Variables.end(),inserter(CommonVariables, CommonVariables.begin()));


      if(!CommonVariables.empty()) // There are variables to QE in the constraint. Hence it is bound
	{
	  Bound_Constraints.insert(root);
	}
      else
	{
	 Free_Constraints.insert(root);
	} 
    }
}

// Same as getLinearizedClone except 
// 1) it handles only selection operator
// 2) if the selection operator x[2:3] is found, and  x is a variable to be eliminated,
//    then the fresh variable replacing x[2:3] is also added to the set of variables to be eliminated
DAGNode* getSimpliedLinearizedCloneWithChangeInVarsToElim(DAGNode* parent, map<DAGNode*, DAGNode*> &LinearizationTable, int &count, map<string, int> &WidthTable, set<string> &VarSetToElim, set<string> &AutoVarsToElim)
    {
        DAGNode* clone;
	string MyName = parent->Get_node_name();

	if(MyName == "select") // node to be linearized encountered
	  {
	    map<DAGNode*, DAGNode*>::iterator it = LinearizationTable.find(parent);
	    
	    if(it != LinearizationTable.end()) // parent is already added to the Table. get the address of the new node and return it
	      {
		clone = it->second;
	      }
	    else // parent is newly encountered. Create a new node and return it's address
	      {
		string new_name = "autoname_";
		count++;
		char char_count[10] = " ";
		sprintf(char_count , "%d", count);
		string string_count(char_count);
		new_name += string_count;


		int width = getWidthOfNode(parent, WidthTable);

		Value *v = new Value();
		Type *t = new Type();
 		t->data_type = BITVECTOR_SYMBOL;
		t->width = width;
		v->set_node_type(*t);
		clone = dm.createNode (new_name,(void*)v);

		LinearizationTable.insert(make_pair(parent, clone)); // inserted into LinearizationTable
		WidthTable.insert(make_pair(new_name, width));

		list<DAGNode*>::iterator it = parent->Get_outlist_start();
		string my_name = (*it)->Get_node_name();

		// cout<<"\nmy_name = "<<my_name<<endl;
		// showSet(VarSetToElim, "VarSetToElim");

		if(VarSetToElim.find(my_name) != VarSetToElim.end())
		  {
		    AutoVarsToElim.insert(new_name); 
		  }
		
	      }//else
	  }//if(MyName == "select") 
                   
        else if(MyName == "bvadd" || MyName == "bvmul" || MyName == "=" || MyName == "not" || MyName == "or" || MyName == "and" || MyName == "if_then_else")  //i.e. if an operator node allowed in DD creation
        {
	   list<DAGNode*>::iterator it = parent->Get_outlist_start();
	   vector<DAGNode *> Children;
      	
	   for(int i=0; i<parent->Get_outlist_size(); i++)
		{
		  DAGNode *child = getSimpliedLinearizedCloneWithChangeInVarsToElim(*it, LinearizationTable, count, WidthTable, VarSetToElim, AutoVarsToElim);
		Children.push_back(child);
		it++;
		}
	
	   clone = dm.createNode(MyName,Children, parent->getValue());
      	}
	
	else if(parent->Get_outlist_size()==0) // leaf level node
    		{
		clone = dm.createNode(MyName, parent->getValue());
      		}
	else
		{
		cout<<"\nIllegal operator "<<MyName<<" encountered in function getSimpliedLinearizedCloneWithChangeInVarsToElim in Eliminator.cpp\n";
		exit(1);
		}

        return clone;
    }        



// Function to create a conjunction of constraints in "Constraints", and dump in
// a given file (Note that although from the function name, it appears that the 
// conjunction is printed in SMTLIB format, really it is just dumped as text file)
void createConjunctionAndPrintInSMTLibFormatInFileWithGivenPreName(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, string file_pre_name, int file_counter)
{
  bool print_input_in_smtlib_format = false;
  if(print_input_in_smtlib_format)
    {
      return;
    }
  else // Dumps in a text file
    {
      DAGNode *resultDAG = NULL;
		
      // resultDAG is the conjunction of the constraints in Constraints
      for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
	{
	  DAGNode *Constraint = *it;
	  if(resultDAG==NULL)
	    {
	      resultDAG = Constraint;
	    }
	  else
	    {
	      resultDAG = createDAG("and",resultDAG, Constraint, LOGICAL_SYMBOL, 1);
	    }
	}

      if(resultDAG == NULL)
	{
	  return;
	}

      set<string> Support;
      getDAGNames(resultDAG, Support);
      set<string> CommonVars;

      set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(CommonVars, CommonVars.begin()));

      if(CommonVars.empty())
	{
	  return;
	}		

      //showSet(CommonVars, "CommonVars");
      
      char file_counter_char[100];
      sprintf(file_counter_char, "%d", file_counter);
      string file_counter_str(file_counter_char);


      string file_name = file_pre_name;
      file_name += "_";
      file_name += file_counter_str;
      file_name += ".txt";
      
      
      
      FILE *fp = fopen(file_name.c_str(), "w");
      if(fp == NULL)
	{
	  cout<<"\nCannot open file "<<file_name<<" in function  in Eliminator.cpp\n";
	  exit(1);
	}

      fprintf(fp, "\nVars\n");
      for(set<string>::iterator it = Support.begin(); it != Support.end(); it++)
	{
	  int width;
	  map<string, int>::iterator wit = WidthTable.find(*it);

	  if(wit == WidthTable.end())
		{
		cout<<"\nNo entry for variable "<<*it<<" in width table in function createConjunctionAndPrintInSMTLibFormatInFileWithGivenPreName in Eliminator.cpp\n";
		exit(1);
		}
	  width = wit->second;

	  fprintf(fp, "\n%s : %d\n", (*it).c_str(), width);
	}

      
      fprintf(fp, "\nVarsToElim\n");
      for(set<string>::iterator it = CommonVars.begin(); it != CommonVars.end(); it++)
	{
	  fprintf(fp, "\n%s\n", (*it).c_str());
	}

      string content = getDAGStr(resultDAG);
      fprintf(fp, "\n\nContent = %s\n", content.c_str());     
      
      fclose(fp);

      
    }// dumps in a text file
}

// Function to dump the contents of a given dag rooted at "resultDAG" in
// a given file (Note that although from the function name, it appears that the 
// conjunction is printed in SMTLIB format, really it is just dumped as text file)
void PrintInSMTLibFormatInFileWithGivenPreName(DAGNode* resultDAG, set<string> &VarsToElim, map<string, int> &WidthTable, string file_pre_name, int file_counter)
{
  bool print_input_in_smtlib_format = false;
  if(print_input_in_smtlib_format)
    {
      return;
    }
  else // Dumps in a text file
    {

      set<string> Support;
      getDAGNames(resultDAG, Support);
      set<string> CommonVars;

      set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(CommonVars, CommonVars.begin()));

      if(CommonVars.empty())
	{
	  return;
	}		

      //showSet(CommonVars, "CommonVars");
      
      char file_counter_char[100];
      sprintf(file_counter_char, "%d", file_counter);
      string file_counter_str(file_counter_char);


      string file_name = file_pre_name;
      file_name += "_";
      file_name += file_counter_str;
      file_name += ".txt";
      
      
      
      FILE *fp = fopen(file_name.c_str(), "w");
      if(fp == NULL)
	{
	  cout<<"\nCannot open file "<<file_name<<" in function  in Eliminator.cpp\n";
	  exit(1);
	}

      fprintf(fp, "\nVars\n");
      for(set<string>::iterator it = Support.begin(); it != Support.end(); it++)
	{
	  int width;
	  map<string, int>::iterator wit = WidthTable.find(*it);

	  if(wit == WidthTable.end())
		{
		cout<<"\nNo entry for variable "<<*it<<" in width table in function createConjunctionAndPrintInSMTLibFormatInFileWithGivenPreName in Eliminator.cpp\n";
		exit(1);
		}
	  width = wit->second;

	  fprintf(fp, "\n%s : %d\n", (*it).c_str(), width);
	}

      
      fprintf(fp, "\nVarsToElim\n");
      for(set<string>::iterator it = CommonVars.begin(); it != CommonVars.end(); it++)
	{
	  fprintf(fp, "\n%s\n", (*it).c_str());
	}

      string content = getDAGStr(resultDAG);
      fprintf(fp, "\n\nContent = %s\n", content.c_str());     
      
      fclose(fp);

      
    }// dumps in a text file
}

// Find the width of the variable from the WidthTable, and return it
// If there is no entry for the variable in WidthTable, exit from the program with a message
int getWidthOfVariable(map<string, int> &WidthTable, string variable)
{
  map<string, int>::iterator wit = WidthTable.find(variable);
  if(wit == WidthTable.end())
		{
		cout<<"\nNo entry for variable "<<variable<<" in width table in function getWidthOfVariable in Eliminator.cpp\n";
		exit(1);
		}
  int width = wit->second;
  return width;
}


// Create a selection node with given name, low index, high index, and width, and return it
DAGNode* createSelectionNode(string variable, int width, int low, int high)
{
  DAGNode* Child1 = createBVVariable(variable, width);

  char low_position_char[100], high_position_char[100];
  sprintf(low_position_char, "%d", low);
  sprintf(high_position_char, "%d", high);
	  
  string low_position_str(low_position_char);
  string high_position_str(high_position_char);

  Value *v1 = new Value();
  Type t1;
  t1.data_type = INTEGER_CONSTANT;
  t1.width = IMMATERIAL_WIDTH;
  v1->set_node_type(t1);
  DAGNode* Child2 = dm.createNode(low_position_str, (void*)v1);
	  
  Value *v2 = new Value();
  Type t2;
  t2.data_type = INTEGER_CONSTANT;
  t2.width = IMMATERIAL_WIDTH;
  v2->set_node_type(t2);
  DAGNode* Child3 = dm.createNode(high_position_str, (void*)v2);


  DAGNode *node = createDAG("select", Child1, Child2, Child3, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);

  return node;

}


// Functions to implement Monniaux style QE on dags with LMCs and sels, concats end here

// Functions which are modified to make things more efficient

// Function to get a dag rooted at "root" which is of the form
// c1*x1+ ... +cn*xn+c0 as AX+B.
// This function converts this to [c1 ... cn] and [c0], 
// pushes [c1 ... cn] into A and [c0] into B
// If ci is 1, it may be omitted. i.e. 1.x1+1.x2 can appear as x1+x2 as well
void getCoefsAndConstsModified(DAGNode *root, list<string> &VarNames, Matrix &A, Matrix &B)
{
  Row MyRow;

  set<string> Support;
  getDAGNames(root, Support);

  map<string, ULINT> VariableCoefficientMap;
  
  // initialize the VariableCoefficientMap with Coefficients 0 or 1

  list<string>::iterator varnames_it = VarNames.begin();
  list<string>::iterator varnames_end = VarNames.end();

  for(;varnames_it != varnames_end; varnames_it++)
	{
	string Variable = *varnames_it;
	ULINT Coefficient;

        if(Support.find(Variable) != Support.end())
	        {
	          Coefficient = 1;
	        }
	      else
	        {
	          Coefficient = 0;
	        }
	VariableCoefficientMap.insert(make_pair(Variable, Coefficient));
	}

   // Get the actual coefficients

   updateVariableCoefficientMap(root, VariableCoefficientMap);

   // Now we have the coefficients for all variables in VariableCoefficientMap
   // Fill in this in the matrices

   varnames_it = VarNames.begin();
   for(;varnames_it != varnames_end; varnames_it++)
	{
	string Variable = *varnames_it;
	map<string, ULINT>::iterator vcit = VariableCoefficientMap.find(Variable);
	if(vcit == VariableCoefficientMap.end())
		{
		cout<<"\nNo entry for the variable "<<Variable<<" in VariableCoefficientMap in getCoefsAndConstsModified in Eliminator.cpp\n";
		exit(1);
		}

	ULINT Coefficient = vcit->second;
	MyRow.push_back(Coefficient);
	}
   
  A.push_back(MyRow);
  // A has the coefficients now

  ULINT Constant=0;
  getConstantModified(root, Constant);

  Row MyConstRow;
  MyConstRow.push_back(Constant);
  // B has the constant now

  B.push_back(MyConstRow);
}

// Given a dag rooted at "root" which is of the form c1*x1+ ... +cn*xn+c0,
// this function generates the map {x1-->c1,...,xn--->cn}
// If ci is 1, it may be omitted. i.e. 1.x1+1.x2 can appear as x1+x2 as well
// precondition: Initialize "VariableCoefficientMap" to {x1-->1,...,xn--->1}
// The function obtainVariableCoefficientMap performs this initialization and
// then calls updateVariableCoefficientMap
void updateVariableCoefficientMap(DAGNode *node, map<string, ULINT> &VariableCoefficientMap)
{
  string dag_str = node->Get_node_name();

  if(dag_str == "bvmul")
	{
	list<DAGNode*>::iterator iout = node->Get_outlist_start();
	DAGNode *coef = *iout;
	iout++;
	DAGNode *var = *iout;
	string variable = var->Get_node_name();

	map<string, ULINT>::iterator vcit = VariableCoefficientMap.find(variable);
	if(vcit == VariableCoefficientMap.end())
		{
		cout<<"\nWarning!No entry for the variable "<<variable<<" in VariableCoefficientMap in updateVariableCoefficientMap in Eliminator.cpp\n";
		}
	else
		{
		ULINT Coefficient = vcit->second;
		if(Coefficient == 1)
			{
			getConstantModified(coef, Coefficient);
			vcit->second = Coefficient;
			}
		}
	}
  
  else     
	{
	  list<DAGNode*>::iterator iout = node->Get_outlist_start();
	  
          for(unsigned int i=0; i<node->Get_outlist_size(); i++, iout++)
		updateVariableCoefficientMap(*iout, VariableCoefficientMap);
	}

}



// Functions which are modified to make things more efficient end here


// Functions added from 2 December 2011 onwards to implement Monniaux style QE on dags with LMCs and LMIs

// Same as callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with support for LMIs
// Use this function if the argument dag has LMIs in it
string callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(DAGNode *root, map<string, int> &WidthTable)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperatorsWithDP_WithSupportForLMIs(root, WidthTableForNodes, WidthTable);

  propogateWidths(root, WidthTableForNodes, 1);

  return getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(root, "bool", WidthTableForNodes, 1, WidthTable);
}


// Version of createWidthTableForOperatorsWithDP with support for LMIs
void createWidthTableForOperatorsWithDP_WithSupportForLMIs(DAGNode *tree, map<DAGNode*, int> &WidthTableForNodes, map<string, int> &WidthTable)
{
  if(WidthTableForNodes.find(tree) != WidthTableForNodes.end()) // already width found
    return;

  string name = tree->Get_node_name();
  unsigned int outlist_size = tree->Get_outlist_size();
  Value* MyValue = (Value*)(tree->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int ntype = MyType.data_type;
  list<DAGNode*>::iterator iout = tree->Get_outlist_start();


   if(!(ntype == BITVECTOR_CONSTANT || ntype == LOGICAL_CONSTANT || ntype == INTEGER_CONSTANT) && outlist_size == 0) // variable
    {
    
      int Width =  WidthTable[name];
      WidthTableForNodes.insert(make_pair(tree, Width));
    }

  else
    {

    switch(outlist_size)
      {
      case 1:
	{
	  createWidthTableForOperatorsWithDP_WithSupportForLMIs(*iout, WidthTableForNodes, WidthTable);

	  if(name == "not")
	    {
	      int Width =  1;
	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	     cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP_WithSupportForLMIs in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	     exit(1);
	    }
 
	 
	  break;
	}
      case 2:
	{
          DAGNode *child1 = *iout;
	  createWidthTableForOperatorsWithDP_WithSupportForLMIs(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child2 = *iout;
	  createWidthTableForOperatorsWithDP_WithSupportForLMIs(*iout, WidthTableForNodes, WidthTable);
	 
	  if(name == "equivalent" || name == "=" || name == "bvadd" || name == "bvmul" || name == "is_not_equal"
           || name == "not_equal_to" || name == "concat" || name == "bvsub"|| name == "bvugt" || 
                  name == "bvult" || name == "bvuge" || name == "bvule")
	    {

	      int Width1 =  WidthTableForNodes[child1];
	      int Width2 =  WidthTableForNodes[child2];

	      int Width;

	      if(name == "equivalent" || name == "=" || name == "bvadd" || name == "bvmul" || name == "is_not_equal"
              || name == "not_equal_to"|| name == "bvsub"|| name == "bvugt" || 
                  name == "bvult" || name == "bvuge" || name == "bvule")
                {
                  Width = findMaximum(Width1, Width2);
              }
	      else // concat
		Width = Width1 + Width2;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	  else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
          else if(name == "bvor" || name == "bvand")
	    {
	      int Width = WidthTableForNodes[child1];

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	      exit(1);
	    }

	  break;
	}
      case 3:
	{
	  DAGNode *child1 = *iout;
	  createWidthTableForOperatorsWithDP_WithSupportForLMIs(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child2 = *iout;
	  createWidthTableForOperatorsWithDP_WithSupportForLMIs(*iout, WidthTableForNodes, WidthTable);
	  iout++;
	  DAGNode *child3 = *iout;
	  createWidthTableForOperatorsWithDP_WithSupportForLMIs(*iout, WidthTableForNodes, WidthTable);

	  if(name == "ite" || name == "if_then_else") // Width of an ite is in fact maximum of widths of child2 and child3
	    {
	      
	      //int Width = 1;

	       int Width1 =  WidthTableForNodes[child2];
	       int Width2 =  WidthTableForNodes[child3];

	      int Width = findMaximum(Width1, Width2);

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else if(name == "select")
	    {
	      int Width1 =  WidthTableForNodes[child2];
	      int Width2 =  WidthTableForNodes[child3];

	      int Width = Width2 - Width1 +1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }
	  else if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	   else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	      exit(1);
	    }

	 
	  break;
	}
      case 0: // constants
	{
	  if(name[0] == 'x') // hex
	    {
	      WidthTableForNodes.insert(make_pair(tree, (name.length()-3)*4));
	    }
	  else // 
	    {
	      int Width = findMinimumWidthRequired(name);

	      WidthTableForNodes.insert(make_pair(tree, Width));

	    }
	break;
	}// case 0 ends here

       default :
	{
          for(int i=0; i<outlist_size; i++)
		{
		DAGNode* child = *iout;
		createWidthTableForOperatorsWithDP_WithSupportForLMIs(child, WidthTableForNodes, WidthTable);
		iout++;
		}
	if(name == "and" || name == "or")
	    {
	      int Width = 1;

	      WidthTableForNodes.insert(make_pair(tree, Width));
	    }

	else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function createWidthTableForOperatorsWithDP_WithSupportForLMIs in Eliminator.cpp with outlist_size = "<<outlist_size<<"\n";
	      exit(1);
	    }

	 
	  break;
	}//default ends here
      }//switch

    }//else
}//function



// Modified version of getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable with handling of LMIs
string getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<string, int> &WidthTable)
{

  string name = node->Get_node_name();

  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)
        {
               dag_str = name;
               return dag_str;
        }
        else
          {

            if(name == "select")
              {
                dag_str = "(bv-extract ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();

                iout++;

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "const", WidthTableForNodes, 1, WidthTable) + " ";

                iout = node->Get_outlist_start();

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "const", WidthTableForNodes, 1, WidthTable) + " ";

                iout = node->Get_outlist_start();

                dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
            
             else if(name == "bvult")
	      {
		dag_str = "(bv-lt ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

            else if(name == "bvugt")
	      {
		dag_str = "(bv-gt ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
            
            else if(name == "bvule")
	      {
		dag_str = "(bv-le ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
            
            else if(name == "bvuge")
	      {
		dag_str = "(bv-ge ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }



	    else if(name == "concat")
	      {
		cout<<"\nconcat is not supported by function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs currently. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		exit(1);		
              }//else if (name == "concat")


	    else if(name == "bvmul")
	      {
		dag_str = "(bv-mul ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "bvadd")
	      {
		dag_str = "(bv-add ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	     else if(name == "bvsub")
	      {
		dag_str = "(bv-sub ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "if_then_else"))
	      {
                if(name == "if_then_else") name = "ite";

		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], WidthTable) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {

                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);
		
              }

	    else if(name == "'0'")
              {
                return convertLiteralToBitvectorWithProperWidth(name, Width_Required);

              }

	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      return dag_str;
		      }
		else if(context == "bv")
		      {
	
			if(name[0] == 'x') // hex
			  {
			  dag_str = convertHexToBitvectorWithProperWidth(name, Width_Required);
			  }
			else // Decimal
			  {
			  dag_str = convertDecimalToBitvectorWithProperWidth(name, Width_Required);
			  }
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
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
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs\n";
		exit(1);
	      }
		    
	  }
}



// Version of printDAGInSMTLIBFormatAsDAG with support for LMIs
string printDAGInSMTLIBFormatAsDAG_WithSupportForLMIs(DAGNode *root, map<string, int> &WidthTable, FILE *fp)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();

  //cout<<"\nObtaining the WidthTable\n";

  createWidthTableForOperatorsWithDP_WithSupportForLMIs(root, WidthTableForNodes, WidthTable);

  //cout<<"\nWidthTable obtained\n";

  set<DAGNode*> DPTable1;

  //cout<<"\nPropgating widths\n";

  DPTable1.clear();

  propogateWidthsWithDP(root, WidthTableForNodes, 1, DPTable1);

  //cout<<"\nWidths propogated\n";

  DPTable1.clear();


  //cout<<"\nSetting the types\n";

  map<DAGNode*, string> TypeTableOfNodes;

  setTypesWithDP_WithSupportForLMIs(root, "bool", TypeTableOfNodes);

  //cout<<"\nTypes set\n";

  //cout<<"\nGetting SMTLIB string\n";

  string smtlib_str;

  //cout<<"\nDAG to be printed in SMTLib format "<<root->getDAGStr()<<endl;

  int TempCount = 0;
  map<DAGNode*, string> LabelTable;
  LabelTable.clear();


  smtlib_str =  printInSMTLibFormatModified_WithSupportForLMIs(root, "bool", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes);


  //cout<<"\nSMTLIB string obtained\n";

   return smtlib_str;
}


    
// Version of setTypesWithDP with support for LMIs
string setTypesWithDP_WithSupportForLMIs(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfNodes)
{
  HashTable<string, string> HTable;
  HashTable<string, string> *PtrHTable=&HTable;

  return setTypesWithDPInternal_WithSupportForLMIs(tree, context, TypeTableOfNodes, PtrHTable);

}


// Version of setTypesWithDPInternal with support for LMIs
string setTypesWithDPInternal_WithSupportForLMIs(DAGNode *tree, string context, map<DAGNode*, string> &TypeTableOfNodes, HashTable<string, string> *PtrHTable)
{
  // check if the node tree is encountered with the same context before

  // create the key

  string key;
  char temp_char[10];
  sprintf(temp_char, "%x", tree);
  string tree_addr(temp_char);
  key = tree_addr + context; 

  HTStatusValue<string> result = PtrHTable->hashtable_search(key);

  if(result.success()) // hash table hit
    {
      return result.GetValue();
    }
  
  else // hash table miss
    {

      string name = tree->Get_node_name();
      unsigned int outlist_size = tree->Get_outlist_size();
      Value* MyValue = (Value*)(tree->getValue());
      Type MyType = MyValue->get_node_type();
      int MyWidth = MyType.width;
      int node_type = MyType.data_type;
      list<DAGNode*>::iterator iout = tree->Get_outlist_start();
      string TreeType;
  

      if(!(node_type == BITVECTOR_CONSTANT || node_type == LOGICAL_CONSTANT || node_type == INTEGER_CONSTANT) && outlist_size == 0)//variable
	{
	  TreeType = "bv";
	}
      else
	{

	  //cout<<"\nNAME=\t"<<name<<endl;    //debug code

	  if(name == "select")
	    {
	      iout++;

	      iout++;
	  
	      setTypesWithDPInternal_WithSupportForLMIs(*iout, "const", TypeTableOfNodes, PtrHTable);

	      iout = tree->Get_outlist_start();

	      iout++;

	      setTypesWithDPInternal_WithSupportForLMIs(*iout, "const", TypeTableOfNodes, PtrHTable);

	      iout = tree->Get_outlist_start();

	      setTypesWithDPInternal_WithSupportForLMIs(*iout, "bv", TypeTableOfNodes, PtrHTable);

	      TreeType = "bv";

	    }
      
	  else if(name == "is_not_equal" || name == "not_equal_to")
	    {
	     string ChildType;

	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  ChildType = setTypesWithDPInternal_WithSupportForLMIs(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TypeTableOfNodes.insert(make_pair(tree, ChildType));

	      TreeType = "bool";
	    }

	  // NB: Assumption here is that equivalent applies only between bit-vectors

	  else if(name == "equivalent" || name == "=")
	    {
	    string ChildType;

	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  ChildType = setTypesWithDPInternal_WithSupportForLMIs(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

              TypeTableOfNodes.insert(make_pair(tree, ChildType));

	      TreeType = "bool";
	    }

          else if(name == "bvugt" || name == "bvult" || name == "bvuge" || name == "bvule")
	    {
	    string ChildType;

	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  ChildType = setTypesWithDPInternal_WithSupportForLMIs(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

              TypeTableOfNodes.insert(make_pair(tree, ChildType));

	      TreeType = "bool";
	    }

      
	  else if(name == "bvmul")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal_WithSupportForLMIs(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bv";

	    }

	  else if(name == "bvadd")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal_WithSupportForLMIs(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bv";

	    }


	  // Newly added on 20.1.2010

	  else if(name == "bvsub")
	    {
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal_WithSupportForLMIs(*iout, "bv", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bv";

	    }

      
	  else if(name == "and" || (name == "or") || (name == "not"))
	    {
	  
	      for(unsigned int i=0; i<outlist_size; i++, iout++)
		{
		  setTypesWithDPInternal_WithSupportForLMIs(*iout, "bool", TypeTableOfNodes, PtrHTable);
		}

	      TreeType = "bool";
	    }

	  else if(name == "ite" || name == "if_then_else")
	    {
	      setTypesWithDPInternal_WithSupportForLMIs(*iout, "bool", TypeTableOfNodes, PtrHTable);

	      iout++;

	      string ChildType;

	      for(unsigned int i=1; i<outlist_size; i++,iout++)
		{
		  ChildType = setTypesWithDPInternal_WithSupportForLMIs(*iout, "bool", TypeTableOfNodes, PtrHTable);
		}

	      TypeTableOfNodes.insert(make_pair(tree, ChildType));

	      TreeType = ChildType;

	    }

	  else if(name == "'1'")
	    {
	      TreeType = "bv";
	    }

	  else if(name == "'0'")
	    {
	      TreeType = "bv";
	    }

	  else if(outlist_size == 0) // Other constants

	    {
		
	      if(context == "const")
		{
		  TreeType = "const";
		}
	      else if(context == "bv")
		{
		  TreeType = "bv";	
		}
	      else if(context == "bool")
		{
		  if(name == "1") TreeType = "bool";
		  else if(name == "0") TreeType = "bool";
		  else if(name == "true") TreeType = "bool";
		  else if(name == "false") TreeType = "bool";

		  else // interpreting a Boolean constant other than true/false as a bit-vector
		    {
		      if(name[0] == 'x') // hex
			{
			  TreeType = "bv";
			}
		      else // Decimal
			{
			  TreeType = "bv";
			}
		    }

		}
	      else
		{
		  cout<<"\nUnanticipated constant "<<name<<" inside function setTypesWithDPInternal_WithSupportForLMIs in Eliminator.cpp\n";
		  exit(1);
		}
		  
	    }
	  else
	    {
	      cout<<"\nUnanticipated operator "<<name<<" inside function setTypesWithDPInternal_WithSupportForLMIs in Eliminator.cpp\n";
	      exit(1);
	    }
		    
	}// else 

      result = PtrHTable->hashtable_insert(key, TreeType);
   
      if(!result.success())
	{

	  cout<<"\nHash Insert Failure Inside Function setTypesWithDPInternal_WithSupportForLMIs in Eliminator.cpp\n";
	  exit(1);
	}

      return TreeType;

    }// hash table miss

}



// Version of printInSMTLibFormatModified with support for LMIs
string printInSMTLibFormatModified_WithSupportForLMIs(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, map<DAGNode*, string> &LabelTable, FILE *fp, map<string, int> &WidthTable, int &TempCount, map<DAGNode*, string> &TypeTableOfNodes)
{
  string label;
  string name = node->Get_node_name();
  unsigned int outlist_size = node->Get_outlist_size();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int node_type = MyType.data_type;
  list<DAGNode*>::iterator iout = node->Get_outlist_start();

  //cout<<"\nname=\t"<<name<<endl;    //debug code

  if( (node_type == BITVECTOR_CONSTANT || node_type == LOGICAL_CONSTANT || node_type == INTEGER_CONSTANT) && (outlist_size == 0)) // constants 
    {
      if(name == "'1'")
	{
	  label = convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);
	  
	}

      else if(name == "'0'")
	{
	  label = convertLiteralToBitvectorWithProperWidthInNewSMTLibFormatModified(name, Width_Required);

	}

      else if(context == "const")
	{
	  label = name;
	}
      else if(context == "bv")
	{
	     
	  if(name[0] == 'x') // hex
	    {
		 
	      label = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
	    }
	  else // Decimal
	    {

	      label = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
	    }
			  
	}
      else if(context == "bool")
	{
	  if(name == "1") label = "true";
	  else if(name == "0") label = "false";
	  else if(name == "true") label = "true";
	  else if(name == "false") label = "false";

	  else // interpreting a Boolean constant other than true/false as a bit-vector
	    {
	      if(name[0] == 'x') // hex
		{
		  label = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		}
	      else // Decimal
		{
		  label = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		}
	    }

	}
      else
	{
	  cout<<"\nUnkown name "<<name<<" with context "<<context<<" encountered in function printInSMTLibFormatModified_WithSupportForLMIs in Eliminator.cpp\n";
	  exit(1);
	}
      return label;
    }

  else // variable or operator
    {
      map<DAGNode*, string>::iterator it=LabelTable.find(node);
      //cout<<"\nnode = "<<node<<", label = "<<label<<endl;
      if(it!=LabelTable.end()) // entry exists for the node
	{
	  //cout<<"\nentry exists for "<<node<<endl;
	  label = it->second;
	  return label;
	}
      else
	{
	  if(!(node_type == BITVECTOR_CONSTANT || node_type == LOGICAL_CONSTANT || node_type == INTEGER_CONSTANT) && outlist_size == 0)  // variable
	    {
	      label = name; // label obtained
	      map<string, int>::iterator wit = WidthTable.find(label);
	      if(wit == WidthTable.end())
		{
		  cout<<"\nThe variable "<<label<<" has no entry in the WidthTable in function printInSMTLibFormatModified_WithSupportForLMIs in Eliminator.cpp\n";
		  exit(1);
		}
	      int width = wit->second; // width obtained

	      fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written		

	      //cout<<"\n"<<label<<" printed "<<endl;

	      LabelTable.insert(make_pair(node, label));

	      //cout<<"\nentry ("<<node<<","<<label<<")"<<" added "<<endl;

// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";
// 	      cout<<"\nReturning\n";

	      return label;
	    }
	  else // operator
	    {
	      //cout<<"\nname=\t"<<name<<endl;    //debug code

	      if(name == "select")
		{
		  vector<string> ChildrenLabel;

		  iout++;

		  iout++;

		  ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
	  
		  iout = node->Get_outlist_start();

		  iout++;

		  ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "const", WidthTableForNodes, 1, LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));

		  iout = node->Get_outlist_start();

		  ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  int child_1_width, child_2_width, width;

		  child_1_width = atoi(ChildrenLabel[0].c_str());
		  child_2_width = atoi(ChildrenLabel[1].c_str());
		  width = child_1_width - child_2_width + 1;


		 //  map<CDAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
// 		  if(wit == WidthTableForNodes.end())
// 		    {
// 		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes\n";
// 		      exit(1);
// 		    }
// 		  int width = wit->second; // width obtained

		  fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		  fprintf(fp,":assumption (= %s (extract [%s:%s] %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      // NB: Assumption here is that not_equal_to applies only between bit-vectors

	      else if(name == "is_not_equal" || name == "not_equal_to")
		{
		  vector<string> ChildrenLabel;
		  ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		  iout++;
		  ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes)); // Children label obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

	          if(TypeTableOfNodes[node]=="bv") // should be written as bv
		    	{
			fprintf(fp,":assumption (iff  %s (not (= %s %s)))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
		  else // should be written as bool
		    	{
			fprintf(fp,":assumption (iff  %s (not (iff %s %s)))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
	  
		  
		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "equivalent" || name == "=")
		{
		  if(name ==  "equivalent") name = "=";
	    
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		  if(TypeTableOfNodes[node]=="bv") // should be written as bv
		    	{
			fprintf(fp,":assumption (iff %s (= %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
		  else	
			{
			fprintf(fp,":assumption (iff %s (iff %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
	  
		  

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
              else if(name == "bvult" || name == "bvugt" || name == "bvule" || name == "bvuge")
		{
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		  if(TypeTableOfNodes[node]=="bv") // This means that the operands are bit-vectors
                       {
			fprintf(fp,":assumption (iff %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
			}
		  else	// To catch LMIs on bits
			{
			cout<<"\nUnknown type in function printInSMTLibFormatModified_WithSupportForLMIs in Eliminator.cpp. Exiting...";
                        exit(1);
			}
	  

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "bvmul" || name == "bvadd" || name == "bvsub")
		{
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  map<DAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		  if(wit == WidthTableForNodes.end())
		    {
		      cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes in function printInSMTLibFormatModified_WithSupportForLMIs in Eliminator.cpp\n";
		      exit(1);
		    }
		  int width = wit->second; // width obtained

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained

		  fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		  fprintf(fp,":assumption (= %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else if(name == "and" || name == "or" || name == "not")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(name == "not")
		    {
		      fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		      fprintf(fp,":assumption (iff %s (not %s))\n",label.c_str(), ChildrenLabel[0].c_str()); // written
		      
		    }
                  else // and, or node
		    {
                          fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

                          if(ChildrenLabel.size()<2)
                          {
                              cout<<"\nand node with "<<ChildrenLabel.size()<<" children encountered in function printInSMTLibFormatModified_WithSupportForLMIs in Eliminator.cpp\n";
                              exit(1);
                          }                      
                          else if(ChildrenLabel.size()==2) // Only two children
                          {
                         
                             fprintf(fp,":assumption (iff %s (%s %s %s))\n",label.c_str(), name.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written
                             
                          }
                          else //more than two children
                          {
                               fprintf(fp,":assumption (iff %s (%s",label.c_str(), name.c_str());
                               for(int LabelIndex=0; LabelIndex<ChildrenLabel.size(); LabelIndex++)
                               {
                                fprintf(fp," %s", ChildrenLabel[LabelIndex].c_str());   
                               }
                              fprintf(fp, "))");
                          }
                         
		    }// and, or node ends here

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}// and, or ,not node ends here

	      else if(name == "ite" || name == "if_then_else")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printInSMTLibFormatModified_WithSupportForLMIs(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], LabelTable, fp, WidthTable, TempCount, TypeTableOfNodes));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "tempname";
		  label += "_";
		  label += temp_string; // label obtained
	  
		  if(TypeTableOfNodes[node]=="bv") // should be written as bv; get the width
		    {
		      map<DAGNode*, int>::iterator wit = WidthTableForNodes.find(node);
		      if(wit == WidthTableForNodes.end())
			{
			  cout<<"\nThe node "<<node<<" has no entry in the WidthTableForNodes  in function printInSMTLibFormatModified_WithSupportForLMIs in Eliminator.cpp\n";
			  exit(1);
			}
		      int width = wit->second; // width obtained

		      fprintf(fp,":extrafuns ((%s BitVec[%d]))\n",label.c_str(), width); // written

		      fprintf(fp,":assumption (= %s (ite %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		      
		    }
		  else
		    {
		      fprintf(fp,":extrapreds ((%s))\n",label.c_str()); // written

		      fprintf(fp,":assumption (iff %s (if_then_else %s %s %s))\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str(), ChildrenLabel[2].c_str()); // written
		    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else
		{
		  cout<<"\nUnanticipated operator "<<name<<" inside function printInSMTLibFormatModified_WithSupportForLMIs in Eliminator.cpp\n";
		  exit(1);
		}
		    
	    }
	}
    }
}



//  Version of checkEntailment_WithSupportForSelsConcats with support for LMIs also
bool checkEntailment_WithSupportForSelsConcats_WithLMISupport(DAGNode *constraint, map<string, int> &WidthTable, map<string, string> &Model)
{
    if(pure_lmdd_mode)
    {
        return true;
    }
    
  // find width

  // constraint is equality or disequality or LMI
  // hence, width(constraint) = width(lhs);

  list<DAGNode*>::iterator iout = constraint->Get_outlist_start();

  DAGNode* lhs = *iout;
  
  //cout<<"\n\nlhs = "<<getDAGStr(lhs)<<endl<<endl;

  int width = getWidthOfNode(lhs, WidthTable);
  
  bool boolean_result;

  //cout<<"\n\nChecking entailment of "<<getDAGStr(constraint)<<endl<<endl;
  //cout<<"\nwidth = "<<width<<endl;

  bvatom bv_result = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(constraint, width, Model, boolean_result, WidthTable);

  return boolean_result;
}




//  Version of lmdeEvaluatorModified_With_Concat_Support with support for LMIs also
bvatom lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(DAGNode *node, int width, map<string, string> &Variable_Value_Map, bool &boolean_result, map<string, int> &WidthTable)
    {
      bvatom bv_result;
      string dag_str;
      int outlist_size;

      dag_str = node->Get_node_name();
      outlist_size = node->Get_outlist_size();
      Value* MyValue = (Value*)(node->getValue());
      Type MyType = MyValue->get_node_type();
      int MyWidth = MyType.width;
      int MyDataType = MyType.data_type;

      //cout<<"\n"<<dag_str<<" node encountered in lmdeEvaluatorModified_With_Concat_Support\n";


      if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && outlist_size==0)  //i.e. if a variable node
	{
	  map<string, string>::iterator Variable_Value_Map_it = Variable_Value_Map.find(dag_str);
	  if(Variable_Value_Map_it == Variable_Value_Map.end())
	    {
               if(checkTimeOut())
                    {
                    // Time Out !!!
                    cout<<"\nWarning!!!The function lmdeEvaluatorModified_With_Concat_Support_WithLMISupport in Eliminator.cpp has timed out. Its result is not exact\n";
                    timed_out = true; // timed_out flag set
                    
                    //set something into bv_result
                    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
                    }
               else
                    {             
                    cout<<"\nNo entry in Variable_Value_Map for variable "<<dag_str<<" in function lmdeEvaluatorModified_With_Concat_Support_WithLMISupport in Eliminator.cpp\n";
                    exit(1);
                    }
	    }
          else
          {
              string bin_value = Variable_Value_Map_it->second; // binary value of the variable from map 

              bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()

              //cout<<"\nbin_value = "<<bin_value<<"\tbin_value.size() = "<<bin_value.size()<<endl;

              if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
                {
                  //cout<<"\nbin_value.size() < width\n";
                  bv_result = bvm->BVExtend(result, width, 0);
                }
              else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
                {
                  //cout<<"\nbin_value.size() < width\n";
                  bv_result = bvm->select_with_int(result, width-1, 0);
                }
              else
                {
                  //cout<<"\nbin_value.size() = width\n";
                  bv_result = result;
                }
          }


	  //cout<<"\nbv_result = "<<endl;
	  //bvm->printBVatom(bv_result, 0);

	} //operand node ends here

      else   //i.e. if an operator node or a constant node
        {
            list<DAGNode*>::iterator iout = node->Get_outlist_start();
	    
	    if(dag_str == "is_not_equal" || dag_str == "not_equal_to")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained
		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained
		  if (inter_bool_result) { // reverse the result as it is is_not_equal
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		    }
		}// "is_not_equal"/"not_equal_to" ends here

	      else if(dag_str == "equivalent" || dag_str == "=")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bool inter_bool_result = bvm->checkBVEquality(op1, op2); // result of equality obtained

		  if (inter_bool_result) { // no need to reverse
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  

		  }
		}// "equivalent"/"=" ends here

            else if(dag_str == "bvugt")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bool inter_bool_result = bvm->checkBVGreaterThan(op1, op2); // result of op1 > op2 obtained

		  if (inter_bool_result) { // no need to reverse
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  

		  }
		}// "bvugt" ends here
            
            else if(dag_str == "bvuge")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bool inter_bool_result_gt = bvm->checkBVGreaterThan(op1, op2); // result of op1 > op2 obtained
                  bool inter_bool_result_eq = bvm->checkBVEquality(op1, op2); // result of op1 = op2 obtained
                  // op1 >= op2 \equiv (op1 > op2) \/ (op1 = op2) 

		  if (inter_bool_result_gt || inter_bool_result_eq) { // no need to reverse
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;		  

		  }
		}// "bvuge" ends here
            
            else if(dag_str == "bvule")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bool inter_bool_result = bvm->checkBVGreaterThan(op1, op2); // result of op1 > op2 obtained

		  if (inter_bool_result) { // need to reverse, since op1 <= op2 \equiv ~(op1 > op2)
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 0;
		    boolean_result = false;;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  

		  }
		}// "bvugt" ends here
            
            else if(dag_str == "bvult")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bool inter_bool_result_gt = bvm->checkBVGreaterThan(op1, op2); // result of op1 > op2 obtained
                  bool inter_bool_result_eq = bvm->checkBVEquality(op1, op2); // result of op1 = op2 obtained
                  // op1 >= op2 \equiv (op1 > op2) \/ (op1 = op2)                 

		  if (inter_bool_result_gt || inter_bool_result_eq) {// need to reverse, since op1 < op2 \equiv ~(op1 >= op2)
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t)0;
		    boolean_result = false;		  
		  }
		  else {
		    bv_result = bvm->getZeroedBVatom(1);
		    bv_result.bv_array[0] = (bv_block_t) 1;
		    boolean_result = true;		  

		  }
		}// "bvult" ends here
            
	      else if(dag_str == "bvmul")
		{
		  if((*iout)->Get_node_name()=="1")// multiplication by 1
		    {
		      //cout<<"\nmultiplication by 1\n";
		      iout++;
		      bv_result = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained - no need to select. op2 is already of proper width
		    }
		  else
		    {

		      bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		      iout++;
		      bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		      bvatom result = bvm->arith_mult(op1, op2); // result obtained
		      bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
		    } 
		}// "bvmul" ends here

	    else if(dag_str == "bvadd")
		{
		  bvatom op1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op1 obtained
		  iout++;
		  bvatom op2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(*iout, width, Variable_Value_Map, boolean_result, WidthTable); // op2 obtained

		  bvatom result = bvm->arith_add(op1, op2); // result obtained
		  bv_result = bvm->select_with_int(result, width-1, 0); // size of result is always >= width. There is no problem with selection
 
		}// "bvadd" ends here
	    else if(dag_str == "select")
	      {
		DAGNode *operand = *iout;
		iout++;
		DAGNode *low_sel = *iout;
		iout++;
		DAGNode *high_sel = *iout;

                // Find the width needed for the operand part
		int width_operand = getWidthOfNode(operand, WidthTable);
		bv_result = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(operand, width_operand, Variable_Value_Map, boolean_result, WidthTable); // operand's part obtained
		//cout<<"\nbv_result = "<<endl;
		//bvm->printBVatom(bv_result, 0);

		string low_sel_string = low_sel->Get_node_name();
		string high_sel_string = high_sel->Get_node_name();

		int low_sel_number = atoi(low_sel_string.c_str());
		int high_sel_number = atoi(high_sel_string.c_str());
 
		// Now we have obtained the bv_result from the operand part; let us take the selection part also

		bv_result = bvm->select_with_int(bv_result, high_sel_number, low_sel_number);

	      } //"select" ends here

	    else if(dag_str == "concat")
	      {
		if(outlist_size > 2)
		  {
		    cout<<"\nPresently concatenations of number of children more than two are not supported by the function lmdeEvaluatorModified_With_Concat_Support_WithLMISupport in Eliminator.cpp\n";
		    exit(1);
		  }

		int width_node = getWidthOfNode(node, WidthTable);

		// The only type of concat the function currently properly supports are of the type variable @ ... @  variable. There can be width problems in the other cases
		bool variable_concatenation = isVariableConcatenation(node);// This is true only if either each child is a variable or concat itself. i.e. variable_concatenation is true only if this node is of the form variable 1 @ variable 2 @ ...@ variable n

		if(!variable_concatenation) 
		  {
		    
		    DAGNode* child1;
		    DAGNode *child2;

		    child1 = *iout;
		    iout++;
		    child2 = *iout;
		    
		    
		    if(isConstant(child1)) // it is like constant @ some node : constants create problems as the width is not known. Hence infer width from the other node.
		      {
			int child2_size = getWidthOfNode(child2, WidthTable);
			bvatom bin_value_1;
			bvatom bin_value_2;
			bin_value_1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child1, width_node - child2_size, Variable_Value_Map, boolean_result,WidthTable);
			bin_value_2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child2, child2_size, Variable_Value_Map, boolean_result,WidthTable);
			bv_result = bvm->concat(bin_value_1,bin_value_2);
		      }
		    else if(isConstant(child2)) // it is like some node  @ constant : constants create problems as the width is not known. Hence infer width from the other node.
		      {
			int child1_size = getWidthOfNode(child1, WidthTable);
			bvatom bin_value_1;
			bvatom bin_value_2;
			bin_value_1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child1, child1_size, Variable_Value_Map, boolean_result,WidthTable);
			bin_value_2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child2, width_node - child1_size, Variable_Value_Map, boolean_result,WidthTable);
			bv_result = bvm->concat(bin_value_1,bin_value_2);			
		      }
		    else  // it is like some node  @ some node : There can be problem of width here if "some node" has constants
		      {
			int child1_size = getWidthOfNode(child1, WidthTable);
			int child2_size = getWidthOfNode(child2, WidthTable);
			bvatom bin_value_1;
			bvatom bin_value_2;
			bin_value_1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child1, child1_size, Variable_Value_Map, boolean_result,WidthTable);
			bin_value_2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child2, child2_size, Variable_Value_Map, boolean_result,WidthTable);
			bv_result = bvm->concat(bin_value_1,bin_value_2);
		      }

		  }//if(!variable_concatenation)

		else // @ of the type variable @ ... @  variable. No need to worry about width
		  {
		  		    
		  DAGNode* child1;
		  DAGNode *child2;

		  child1 = *iout;
		  iout++;
		  child2 = *iout;

		  int child1_size = getWidthOfNode(child1, WidthTable);
		  int child2_size = getWidthOfNode(child2, WidthTable);
		  bvatom bin_value_1;
		  bvatom bin_value_2;
		  bin_value_1 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child1, child1_size, Variable_Value_Map, boolean_result,WidthTable);
		  bin_value_2 = lmdeEvaluatorModified_With_Concat_Support_WithLMISupport(child2, child2_size, Variable_Value_Map, boolean_result,WidthTable);
		  bv_result = bvm->concat(bin_value_1,bin_value_2);		    
		  }//else of if(!variable_concatenation)
	      }//"concat" ends here


	    else if(outlist_size==0) // constants
	      {

		ULINT dec_value = strtoull(dag_str.c_str(), NULL, 0); // decimal value of the constant obtained from the dag node
		string bin_value = integerToBinaryString(dec_value); // binary value with minimum required width
		bvatom result = bvm->getBVatom(bin_value.size(), bin_value); // result is a bvatom with content in bin_value and size as bin_value.size()	  

		if(bin_value.size() < width) // bin_value needs to be zero-extended to match width
		  {
		    bv_result = bvm->BVExtend(result, width, 0);
		  }
		else if(bin_value.size() > width) // bin_value needs to be truncated to match width. Hence select from 0 to width-1
		  {
		    bv_result = bvm->select_with_int(result, width-1, 0);
		  }
		else
		  {
		    bv_result = result;
		  }

	      }// if a constant node ends here
	    else
	      {
		cout<<"\nOperator "<<dag_str<<" is not handled in function lmdeEvaluatorModified_With_Concat_Support_WithLMISupport in Eliminator.cpp\n";
		exit(1);
	      }// else case
	    

	}//if an operator node or a constant node ends here


      //cout<<"\nbv_result from "<<dag_str<<" for lmdeEvaluatorModified_With_Concat_Support_WithLMISupport = ";
      //bvm->printBVatom(bv_result, 0);
  
  return bv_result;
		    
}// function ends here



// negateConstraint with support for LMIs
DAGNode* negateConstraint_With_LMISupport(DAGNode* C)
{
  string new_name;
  if( (C->Get_node_name() == "equivalent") || (C->Get_node_name() == "=") )
    new_name = "is_not_equal";
  else if(C->Get_node_name() == "is_not_equal")
    new_name = "=";
  else if(C->Get_node_name() == "bvuge")
    new_name = "bvult";
  else if(C->Get_node_name() == "bvule")
    new_name = "bvugt";
  else if(C->Get_node_name() == "bvugt")
    new_name = "bvule";
  else if(C->Get_node_name() == "bvult")
    new_name = "bvuge";

  list<DAGNode*>::iterator iout = C->Get_outlist_start();
  DAGNode *left = *iout;
  iout++;
  DAGNode *right=*iout;
  return createDAG(new_name, left, right, LOGICAL_SYMBOL, 1);
}



// checkForExactnessInOneDirectionUsingSTP with support for LMIs
bool checkForExactnessInOneDirectionUsingSTP_With_LMI_Support(DAGNode* original, DAGNode* eliminated, map<string, int> &WidthTable)
{

  DAGNode *Final;

  Final = createDAG("and", original, createDAG("not", eliminated, LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1);

  bool result = ThUnsatSingleFormulaInSMTLibFormatAsDAG_With_LMI_Support(Final, WidthTable);

  if(result)//unsat
    {
      cout<<"\nexists(X,f)=>f' holds\n";
      return true;
     }
      
  else //sat
    { cout<<"\nexists(X,f)=>f' does not hold\n";
      return false;
    }
 //Other cases are alreday taken care
}


//  Version of ThUnsatSingleFormulaInSMTLibFormatAsDAG with support for LMIs
bool ThUnsatSingleFormulaInSMTLibFormatAsDAG_With_LMI_Support(DAGNode* f, map<string, int> &WidthTable)
{
  FILE *smt_file = fopen("thunsat_single_formula.smt", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.smt\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  string dag_str = printDAGInSMTLIBFormatAsDAG_WithSupportForLMIs(f, WidthTable, smt_file);

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);

  system("stp --SMTLIB1 thunsat_single_formula.smt > result_thunsat_single_formula.txt"); //call to STP

  FILE *result_smt_file = fopen("result_thunsat_single_formula.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_single_formula.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function ThUnsatSingleFormulaInSMTLibFormatAsDAG_With_LMI_Support in Elminator.cpp \n"; exit(1);
    }
}



// callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable with support for LMIs
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(DAGNode *root, map<string, int> WidthTable)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperatorsWithDP_WithSupportForLMIs(root, WidthTableForNodes, WidthTable);

  propogateWidths(root, WidthTableForNodes, 1);
   
  return getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(root, "bool", WidthTableForNodes, 1);
}


// getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable with support for LMIs
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)
        {
               dag_str = name;
               return dag_str;
        }
        else
          {

            if(name == "select")
              {
                dag_str = "(extract[";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();

                iout++;

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "const", WidthTableForNodes, 1) + ":";

                iout = node->Get_outlist_start();

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "const", WidthTableForNodes, 1) + "] ";

                iout = node->Get_outlist_start();

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
            else if(name == "bvult")
	      {
		dag_str = "(bvult ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

            else if(name == "bvugt")
	      {
		dag_str = "(bvugt ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
            
            else if(name == "bvule")
	      {
		dag_str = "(bvule ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
            
            else if(name == "bvuge")
	      {
		dag_str = "(bvuge ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }



	    else if(name == "concat")
	      {


		dag_str = "(concat ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
		    if(isBVConstant(*iout)) // child is a bv-constant. our concatenation printing may be incorrect
		      {
			cout<<"\nconcat is not supported by function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs currently if any operator is a bv-constant. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		      exit(1);
		      }
			
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "bvmul")
	      {
		dag_str = "(bvmul ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "bvadd")
	      {
		dag_str = "(bvadd ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	     else if(name == "bvsub")
	      {
		dag_str = "(bvsub ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "bvor" || name == "bvand")
	      {
		dag_str =  "(" +  name + " ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "if_then_else"))
	      {
		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node]) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {

                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		
              }

	    else if(name == "'0'")
              {
                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);

              }

	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      return dag_str;
		      }
		else if(context == "bv")
		      {
	
			if(name[0] == 'x') // hex
			  {
			  dag_str = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			else // Decimal
			  {
			  dag_str = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
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
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs\n";
		exit(1);
	      }
		    
	  }
}

// Function to convert each variable x to x+((2^p)/2) mod 2^p,
// where 2^p is ModWidthInteger and (2^p)/2 is ModWidthIntegerHalf
DAGNode*  convertToUnsigned(DAGNode *node, map<string, int> &WidthTable)
{
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  DAGNode *ret =convertToUnsignedInternal(node, PtrDPTable, WidthTable);
  return ret;
}

// Function called by  convertToUnsigned
// Returns "node" with the conversion applied
DAGNode*  convertToUnsignedInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable, map<string, int> &WidthTable)
{
  DAGNode* clone;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 
  
  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }
//cout<<"\nCache Miss for node "<<node<<"\n";
// Create the converted dag in clone
string MyName = node->Get_node_name();
Value* MyValue = (Value*)(node->getValue());
Type MyType = MyValue->get_node_type();
int MyWidth = MyType.width;
int MyDataType = MyType.data_type;

 if(MyName == "bvmul") // operator node to be unsigned encountered
    {
     clone = convertMultiplyNodeToUnsigned(node, WidthTable);
    }
                   
 else if(node->Get_outlist_size() > 0)  // operator node not to be converted encountered; go down
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> Children;
      	
      for(int i=0; i<node->Get_outlist_size(); i++)
	{
	  DAGNode *child = convertToUnsignedInternal(*it, DPTable, WidthTable);
	  Children.push_back(child);
	  it++;
	}
	
      clone = dm.createNode(MyName,Children, node->getValue());
    }
	
 else if(node->Get_outlist_size()==0) // leaf level node
    {
      
      if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT) // variable
          // encountered
            {
            clone = convertVariableNodeToUnsigned(node, WidthTable);
            }
       else // constant 
            {
            clone = dm.createNode(MyName, node->getValue());
            }
    }
  else // unhandled operator
    {
      cout<<"\nIllegal operator "<<MyName<<" encountered in function convertToUnsignedInternal in Eliminator.cpp\n";
      exit(1);
    }
  
  result = DPTable->hashtable_insert(key, clone);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function  convertToUnsignedInternal\n";
      exit(1);
    }
  return clone;
}


// Function to convert variable x to  x+((2^p)/2) mod 2^p
DAGNode* convertVariableNodeToUnsigned(DAGNode *node, map<string, int> &WidthTable)
{
int width;
string MyName = node->Get_node_name();
map<string, int>::iterator wt_it = WidthTable.find(MyName);
if(wt_it == WidthTable.end())
	{
	 cout<<"\nInside function convertVariableNodeToUnsigned in Eliminator.cpp, no width table entry found for "<<MyName<<endl;
	 exit(1);
	}
width = (wt_it)->second;

char coeff[20];
sprintf(coeff, "%llu", ModWidthIntegerHalf);
string coeff_str(coeff);

                        
DAGNode *coefficient = createBVConstant(coeff_str,width);
DAGNode *clone =createDAG("bvadd", node, coefficient, BITVECTOR_SYMBOL, width);    
return clone;
}

// Function to convert the term c.x to  c.x+c.((2^p)/2) mod 2^p
DAGNode* convertMultiplyNodeToUnsigned(DAGNode *node, map<string, int> &WidthTable)
{
 list<DAGNode*>::iterator iout = node->Get_outlist_start();
 DAGNode *coef = *iout;
 iout++;
 DAGNode *var = *iout;
 
 ULINT Coefficient = 1;
 getConstantModified(coef, Coefficient);
 
int width;
string MyName = var->Get_node_name();
map<string, int>::iterator wt_it = WidthTable.find(MyName);
if(wt_it == WidthTable.end())
	{
	 cout<<"\nInside functionconvertMultiplyNodeToUnsigned in Eliminator.cpp, no width table entry found for "<<MyName<<endl;
	 exit(1);
	}
width = (wt_it)->second;

ULINT Product = mul(Coefficient, ModWidthIntegerHalf, ModWidthInteger);// Product = c.((2^p)/2) mod 2^p

char coeff[20];
sprintf(coeff, "%llu", Product);
string Product_str(coeff);

                        
DAGNode *constant = createBVConstant(Product_str,width);
DAGNode *clone =createDAG("bvadd", node, constant, BITVECTOR_SYMBOL, width);    
return clone;
}



// create conjunction of the given set of constraints and 
// call the function checkForExactnessInOtherDirection in BitBlaster.cpp
void createConjunctionAndCheckQBFSolverEffectivess(vector<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
DAGNode *resultDAG = NULL;

// initialDAG is the conjunction of the constraints in Constraints
for(int i=0;i<Constraints.size(); i++)
{
DAGNode *Constraint = Constraints[i];
if(resultDAG==NULL)
        {
        resultDAG = Constraint;
        }
else
        {
        resultDAG = createDAG("and",resultDAG, Constraint, LOGICAL_SYMBOL, 1);
        }
}

if(resultDAG == NULL)
        {
        return;
        }

cout<<"\nSize of dag = "<<dm.getSizeOfDAG(resultDAG)<<endl;

set<string> Support;
getDAGNames(resultDAG, Support);
set<string> CommonVars;
set<string> VarSetRemaining;

 set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(CommonVars, CommonVars.begin()));

if(CommonVars.empty())
        {
        return;
        }	
 
 for(map<string, int>::iterator wit=WidthTable.begin(); wit!=WidthTable.end(); wit++)
    {
      if(CommonVars.find(wit->first) == CommonVars.end())
	{
	VarSetRemaining.insert(wit->first);
	}
    }
 
//showSet(CommonVars, "CommonVars");
//showSet(VarSetRemaining, "VarSetRemaining");
 
bool qbf_result = checkQBFSolverEffectiveness(resultDAG, CommonVars, VarSetRemaining, WidthTable);		
}





// Version of getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style with support for LMIs
string getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(DAGNode *node, string context, map<DAGNode*, int> &WidthTableForNodes, int Width_Required, set<string> &VarSetToElim)
{

  string name = node->Get_node_name();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int MyDataType = MyType.data_type;

        string dag_str;

        if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && node->Get_outlist_size() == 0)
        {
               if(VarSetToElim.find(name) != VarSetToElim.end())
		 {
		   dag_str="?";
		   dag_str+=name;		   
		 }
	       else
		{
		dag_str=name;
		}
               return dag_str;
        }
        else
          {

            if(name == "select")
              {
                dag_str = "(extract[";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();

                iout++;

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "const", WidthTableForNodes, 1, VarSetToElim) + ":";

                iout = node->Get_outlist_start();

                iout++;

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "const", WidthTableForNodes, 1, VarSetToElim) + "] ";

                iout = node->Get_outlist_start();

                dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim);

                dag_str += ")";

                return dag_str;
              }

	    // NB: Assumption here is that not_equal_to applies only between bit-vectors

            else if(name == "is_not_equal" || name == "not_equal_to")
              {
                dag_str = "(not (= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
		unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += "))";

                return dag_str;
              }

	    // NB: Assumption here is that equivalent applies only between bit-vectors

            else if(name == "equivalent" || name == "=")
	      {
		dag_str = "(= ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }
            
            
            else if(name == "bvult")
	      {
		dag_str = "(bvult ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                 }

                dag_str += ")";

                return dag_str;
              }

            else if(name == "bvugt")
	      {
		dag_str = "(bvugt ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                 }

                dag_str += ")";

                return dag_str;
              }
            
            else if(name == "bvule")
	      {
		dag_str = "(bvule ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                 }

                dag_str += ")";

                return dag_str;
              }
            
            else if(name == "bvuge")
	      {
		dag_str = "(bvuge ";

                list<DAGNode*>::iterator iout = node->Get_outlist_start();
	        unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                 }

                dag_str += ")";

                return dag_str;
              }


	    else if(name == "concat")
	      {


		dag_str = "(concat ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
		    if(isBVConstant(*iout)) // child is a bv-constant. our concatenation printing may be incorrect
		      {
			cout<<"\nconcat is not supported by function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport currently if any operator is a bv-constant. The reason is that given 1@x, currently the function pads leading 0's to 1 to make it's size equal to size of x\n";
		      exit(1);
		      }
			
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "bvmul")
	      {
		dag_str = "(bvmul ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	     else if(name == "bvadd")
	      {
		dag_str = "(bvadd ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	     else if(name == "bvsub")
	      {
		dag_str = "(bvsub ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }

	    else if(name == "bvor" || name == "bvand")
	      {
		dag_str =  "(" +  name + " ";

               list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)

                  {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bv", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;
              }


	    // NB: Assumption here is that we do not have bit wise boolean operations on bit-vectors. i.e. and, or, not are on boolean. Also it is assumed that ite is (ite bool bool bool) which is true if the SE comes from dag of proper propositional skelton form

	    else if(name == "and" || (name == "or") || (name == "not") || (name == "if_then_else"))
	      {
		dag_str = "(" +  name + " ";

	       list<DAGNode*>::iterator iout = node->Get_outlist_start();
	       unsigned int outlist_size = node->Get_outlist_size();

                for(unsigned int i=0; i<outlist_size; i++, iout++)
                 {
                    dag_str += getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(*iout, "bool", WidthTableForNodes, WidthTableForNodes[node], VarSetToElim) + " ";
                  }

                dag_str += ")";

                return dag_str;

              }

	    else if(name == "'1'")
              {

                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
		
              }

	    else if(name == "'0'")
              {
                return convertLiteralToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);

              }

	    else if(node->Get_outlist_size() == 0) // Other constants

	      {
		
		if(context == "const")
		      {
		      dag_str = name;
		      return dag_str;
		      }
		else if(context == "bv")
		      {
	
			if(name[0] == 'x') // hex
			  {
			  dag_str = convertHexToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			else // Decimal
			  {
			  dag_str = convertDecimalToBitvectorWithProperWidthInNewSMTLibFormat(name, Width_Required);
			  }
			return dag_str;
		      }
		else if(context == "bool")
		      {
			if(name == "1") dag_str = "true";
			else if(name == "0") dag_str = "false";
			else if(name == "true") dag_str = "true";
			else if(name == "false") dag_str = "false";

			else // interpreting a Boolean constant other than true/false as a bit-vector
			  {
			    if(name[0] == 'x') // hex
			      {
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
			cout<<"\nUnanticipated constant "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport\n";
			exit(1);
		      }
		  
	      }
	    else
	      {
		cout<<"\nUnanticipated operator "<<name<<" inside function getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport\n";
		exit(1);
	      }
		    
	  }
}




// Version of callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style with support for LMIs
string callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(DAGNode *root, map<string, int> &WidthTable, set<string> &VarSetToElim)
{
  map<DAGNode*, int> WidthTableForNodes;

  WidthTableForNodes.clear();
  
  createWidthTableForOperatorsWithDP_WithSupportForLMIs(root, WidthTableForNodes, WidthTable);

  propogateWidths(root, WidthTableForNodes, 1);
   
  return getDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTableInCVC3Style_WithLMISupport(root, "bool", WidthTableForNodes, 1, VarSetToElim);
}



// Given a dag node "parent" and a map "ValueMap" from variable names to valueindecimal[width]
// for some vaiables, create a copy of parent with variable nodes replaced by value[width] nodes
// for the variables present in the "ValueMap".
// This function uses dynamic programming
DAGNode* cloneUsingValueMap(DAGNode* parent, map<string, string> &ValueMap)
{
map<DAGNode*, DAGNode*> DPTable;
return cloneUsingValueMapInternal(parent, ValueMap, DPTable);
}


// Function to create clone with value map. Called from cloneUsingValueMap
DAGNode* cloneUsingValueMapInternal(DAGNode* parent, map<string, string> &ValueMap, map<DAGNode*, DAGNode*> &DPTable)
    {

       map<DAGNode*, DAGNode*>::iterator dit=DPTable.find(parent);

       if(dit!=DPTable.end()) // Hit in DPTable
	{
	return dit->second;
	}        

        DAGNode* clone;
	// Get the size of outlist of parent
	unsigned int outlist_size = parent->Get_outlist_size();
	
                   
        if(outlist_size>0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = cloneUsingValueMapInternal(*iout, ValueMap, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = cloneUsingValueMapInternal(*iout, ValueMap, DPTable);
		    iout++;
                    child_2 = cloneUsingValueMapInternal(*iout, ValueMap, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

                    child_1 = cloneUsingValueMapInternal(*iout, ValueMap, DPTable);
		    iout++;
                    child_2 = cloneUsingValueMapInternal(*iout, ValueMap, DPTable);
		    iout++;
                    child_3 = cloneUsingValueMapInternal(*iout, ValueMap, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    Children.push_back(child_3);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function cloneUsingValueMapInternal in Eliminator.cpp\n";
		 exit(1);
		}

	    }
	}
	else // leaf node
	  {
	    string id_delta = parent->Get_node_name();
	    Value* MyValue = (Value*)(parent->getValue());
	    Type MyType = MyValue->get_node_type();
	    int MyWidth = MyType.width;
	    int MyDataType = MyType.data_type;

	    if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
	      {
		// create the clone
                map<string, string>::iterator vit = ValueMap.find(id_delta);
                if(vit == ValueMap.end()) // no need to replace by constant
                {
                 clone = dm.createNode(parent->Get_node_name(), parent->getValue()); 
                }
                else // replace by constant node
                {
                string value_width = vit->second;
                int width_start_pos = value_width.find("[");
                string value  = value_width.substr(0, width_start_pos);
                string width = value_width.substr(width_start_pos+1);
                int width_end_pos = width.find("]");
                width  = width.substr(0,width_end_pos);
                int width_int = atoi(width.c_str());
                
                
		clone =  createBVConstant(value, width_int);
                }
	      }
	    else // constant
	      {
		clone = dm.createNode(parent->Get_node_name(), parent->getValue());
	      }
            
	  }// leaf node
  
        DPTable.insert(make_pair(parent, clone));
        return clone;
    } 





// Given a dag node "parent", a "shift", and "VarsToElim",
// create a copy of parent with occurances of variables v in "VarsToElim" changed as
// v_shift; This function uses dynamic programming
DAGNode* cloneAndShift(DAGNode* parent, int shift, set<string> &VarsToElim)
{
map<DAGNode*, DAGNode*> DPTable;
return cloneAndShiftInternal(parent, shift, VarsToElim, DPTable);
}


// Function called from cloneAndShift
DAGNode* cloneAndShiftInternal(DAGNode* parent,  int shift,  set<string> &VarsToElim, map<DAGNode*, DAGNode*> &DPTable)
    {

       map<DAGNode*, DAGNode*>::iterator dit=DPTable.find(parent);

       if(dit!=DPTable.end()) // Hit in DPTable
	{
	return dit->second;
	}        

        DAGNode* clone;
	// Get the size of outlist of parent
	unsigned int outlist_size = parent->Get_outlist_size();
	
                   
        if(outlist_size>0)  //i.e. if an operator node
        {
            list<DAGNode*>::iterator iout = parent->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = cloneAndShiftInternal(*iout, shift, VarsToElim, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = cloneAndShiftInternal(*iout, shift, VarsToElim, DPTable);
		    iout++;
                    child_2 = cloneAndShiftInternal(*iout, shift, VarsToElim, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

                    child_1 = cloneAndShiftInternal(*iout, shift, VarsToElim, DPTable);
		    iout++;
                    child_2 = cloneAndShiftInternal(*iout, shift, VarsToElim, DPTable);
		    iout++;
                    child_3 = cloneAndShiftInternal(*iout, shift, VarsToElim, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    Children.push_back(child_3);
		    clone = dm.createNode(parent->Get_node_name(),Children, parent->getValue());
                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function cloneAndShiftInternal in Eliminator.cpp\n";
		 exit(1);
		}

	    }
	}
	else // leaf node
	  {
	    string id = parent->Get_node_name();
	    Value* MyValue = (Value*)(parent->getValue());
	    Type MyType = MyValue->get_node_type();
	    int MyWidth = MyType.width;
	    int MyDataType = MyType.data_type;

	    if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT && parent->Get_outlist_size() == 0) // variable
	      {
                
                if(VarsToElim.find(id) != VarsToElim.end()) // id is a variable to be eliminated
                {
                string new_name = id;
		new_name += "_";
		char temp[100];
		sprintf(temp, "%d", shift);
		string delta_str = temp;
		new_name += delta_str; // new name

		Value *v = new Value();
		Type *t = new Type();
 		t->data_type = MyDataType;
		t->width = MyWidth; // width = old variable's width
		v->set_node_type(*t);
		clone = dm.createNode (new_name,(void*)v);
                }
                else // id is a variable to remain
                {
                 clone = dm.createNode(parent->Get_node_name(), parent->getValue());   
                }
	      }
	    else // constants
	      {
		clone = dm.createNode(parent->Get_node_name(), parent->getValue());
	      }
            
	  }// leaf node ends
  
        DPTable.insert(make_pair(parent, clone));
        return clone;
    } 



// Insert new entries into the width table after cloneAndShif function
void updateWidthTableAftercloneAndShift(map<string, int> &WidthTable, int shift, set<string> &VarsToElim)
{
    for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
        string variable = *it;
        int width;
        map<string, int>::iterator wit = WidthTable.find(variable);
        if(wit == WidthTable.end())
        {
            cout<<"\nNo entry for "<<variable<<" in WidthTable. Error in function updateWidthTableAftercloneAndShift in Eliminator.cpp\n";
            exit(1);
        }
        width = wit->second;
        
        string new_name = variable;
	new_name += "_";
	char temp[100];
	sprintf(temp, "%d", shift);
	string delta_str = temp;
	new_name += delta_str;
        
        WidthTable.insert(make_pair(new_name, width));
    }
}

// Call cloneAndShift followed by updateWidthTableAftercloneAndShift
DAGNode* cloneAndShiftWithWidthTableUpdate(DAGNode* Conjunction, int shift, set<string> &VarsToElim, map<string, int> &WidthTable)
{
    DAGNode* ClonedConjunction = cloneAndShift(Conjunction, shift, VarsToElim);
    updateWidthTableAftercloneAndShift(WidthTable, shift, VarsToElim);
    return ClonedConjunction;
}




// Function to convert each constant to constant - 2^(p-1)
// where 2^p is ModWidthInteger and 2^(p-1) is ModWidthIntegerHalf
DAGNode*  convertConstants(DAGNode *node)
{
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  
  bool bvmul_encountered = false;
  DAGNode *ret =convertConstantsInternal(node, PtrDPTable, bvmul_encountered);
  return ret;
}

// Function called by  convertConstants
// Returns "node" with the conversion applied
DAGNode*  convertConstantsInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable, bool bvmul_encountered)
{
  DAGNode* clone;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 
  
  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }
//cout<<"\nCache Miss for node "<<node<<"\n";
// Create the converted dag in clone
string MyName = node->Get_node_name();
Value* MyValue = (Value*)(node->getValue());
Type MyType = MyValue->get_node_type();
int MyWidth = MyType.width;
int MyDataType = MyType.data_type;

 if(MyName == "bvmul") // bvmul encountered
    {
     list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> Children;
      	
      for(int i=0; i<node->Get_outlist_size(); i++)
	{
	  DAGNode *child = convertConstantsInternal(*it, DPTable, true);
	  Children.push_back(child);
	  it++;
	}
	
      clone = dm.createNode(MyName,Children, node->getValue());
    }
                   
 else if(node->Get_outlist_size() > 0)  // other operator nodes
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> Children;
      	
      for(int i=0; i<node->Get_outlist_size(); i++)
	{
	  DAGNode *child = convertConstantsInternal(*it, DPTable, bvmul_encountered);
	  Children.push_back(child);
	  it++;
	}
	
      clone = dm.createNode(MyName,Children, node->getValue());
    }
	
 else if(node->Get_outlist_size()==0) // leaf level node
    {
      
      if(MyDataType != BITVECTOR_CONSTANT && MyDataType != LOGICAL_CONSTANT && MyDataType != INTEGER_CONSTANT) // variable
          // encountered
            {
            clone = dm.createNode(MyName, node->getValue());
            }
       else // constant 
            {
            if(bvmul_encountered) // constant inside bvmul. No need to substract 2^(p-1)
                {
                clone = dm.createNode(MyName, node->getValue());  
                }
            else //other constants. substract 2^(p-1)
                {
                string NewName = subtractHalfMod(MyName);
                clone = dm.createNode(NewName, node->getValue());  
                }    
            
            }// constant ends here
    }// leaf level node
  else // unhandled operator
    {
      cout<<"\nIllegal operator "<<MyName<<" encountered in function convertConstantsInternal in Eliminator.cpp\n";
      exit(1);
    }
  
  result = DPTable->hashtable_insert(key, clone);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside function  convertConstantsInternal in Eliminator.cpp\n";
      exit(1);
    }
  return clone;
}


// Convert "bvconstant" to "bvconstant - ModWIdthIntegerHalf"
string subtractHalfMod(string bvconstant)
{
unsigned long long int number=strtoull(bvconstant.c_str(), NULL, 0);
number = number - ModWidthIntegerHalf;
char number_char[10];
sprintf(number_char, "%llu", number);
string number_str(number_char);
return number_str;
}


// Function to convert each side of LMI p \bowtie q to p+2^(p-1) \bowtie q+2^(p-1)
// where  2^(p-1) is ModWidthIntegerHalf
DAGNode*  addHalfModulusToBothSides(DAGNode *node)
{
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  
  DAGNode *ret = addHalfModulusToBothSidesInternal(node, PtrDPTable);  
  return ret;
}

// Function called by  addHalfModulusToBothSides
// Returns "node" with the conversion applied
DAGNode*  addHalfModulusToBothSidesInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable)
{
  DAGNode* clone;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 
  
  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }
//cout<<"\nCache Miss for node "<<node<<"\n";
// Create the converted dag in clone
string MyName = node->Get_node_name();
Value* MyValue = (Value*)(node->getValue());
Type MyType = MyValue->get_node_type();
int MyWidth = MyType.width;
int MyDataType = MyType.data_type;

 if(MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt") // LMI encountered
    {
     list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> Children;
      
      DAGNode *LHS = addHalfModulusToLHS(*it);
      Children.push_back(LHS);
      it++;
      DAGNode *RHS = addHalfModulusToRHS(*it);
      Children.push_back(RHS);
	
      clone = dm.createNode(MyName,Children, node->getValue());
    }
                   
 else if(node->Get_outlist_size() > 0)  // other operator nodes
    {
      list<DAGNode*>::iterator it = node->Get_outlist_start();
      vector<DAGNode *> Children;
      	
      for(int i=0; i<node->Get_outlist_size(); i++)
	{
	  DAGNode *child = addHalfModulusToBothSidesInternal(*it, DPTable);
	  Children.push_back(child);
	  it++;
	}
	
      clone = dm.createNode(MyName,Children, node->getValue());
    }
	
 else if(node->Get_outlist_size()==0) // leaf level node
    {
     clone = dm.createNode(MyName, node->getValue());
    }// leaf level node

  else // unhandled operator
    {
      cout<<"\nIllegal operator "<<MyName<<" encountered in function addHalfModulusToBothSidesInternal in Eliminator.cpp\n";
      exit(1);
    }
  
  result = DPTable->hashtable_insert(key, clone);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside function  addHalfModulusToBothSidesInternal in Eliminator.cpp\n";
      exit(1);
    }
  return clone;
}


// LHS is a1.x1 +a2.x2 + .... +an.xn 
// Convert this to a1.x1 +a2.x2 + .... +an.xn+ModWidthIntegerHalf
DAGNode* addHalfModulusToLHS(DAGNode *LHS)
{
    char number_char[10];
    sprintf(number_char, "%llu", ModWidthIntegerHalf);
    string number_str(number_char);
    
    DAGNode *Constant = createBVConstant(number_str, WidthInteger);
    if(LHS == NULL)
    {
        cout<<"\nLHS becoming NULL in function addHalfModulusToLHS in Eliminator.cpp\n";
        exit(1);
    }
    DAGNode *ret = createDAG("bvadd", LHS, Constant, BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
    return ret;
}


// RHS is a constant c 
// Convert this to c+ModWidthIntegerHalf
DAGNode* addHalfModulusToRHS(DAGNode *RHS)
{
string MyName = RHS->Get_node_name();
string NewName = addHalfMod(MyName);
DAGNode *ret = dm.createNode(NewName, RHS->getValue());
return ret;
}

// Convert "bvconstant" to "(bvconstant + ModWIdthIntegerHalf) Mod ModWIdthInteger"
string addHalfMod(string bvconstant)
{
ULINT number=strtoull(bvconstant.c_str(), NULL, 0);
number = add(number, ModWidthIntegerHalf, ModWidthInteger);
char number_char[10];
sprintf(number_char, "%llu", number);
string number_str(number_char);
return number_str;
}


// Version of   splitConjunctionIntoSet where APs are LMIs
void splitConjunctionIntoLMIVector(DAGNode* Constraint, vector<DAGNode*> &Conjuncts)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt") // terminal reached
    {
      Conjuncts.push_back(Constraint);
    }

  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoLMIVector in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoLMIVector(*iout, Conjuncts);
      
      iout++;

      splitConjunctionIntoLMIVector(*iout, Conjuncts);
    }
  else 
    {
     cout<<"\nunkwnon node "<<MyName<<" encountered in function splitConjunctionIntoLMIVector in Eliminator.cpp\n";
     exit(1); 
    }
}


// Version of splitConjunctionIntoLMIVector except that set is used in place of vector
void splitConjunctionIntoLMISet(DAGNode* Constraint, set<DAGNode*> &Conjuncts)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt") // terminal reached
    {
      Conjuncts.insert(Constraint);
    }

  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoLMISet in DAGEliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoLMISet(*iout, Conjuncts);
      
      iout++;

      splitConjunctionIntoLMISet(*iout, Conjuncts);
    }
  else 
    {
     cout<<"\nunkwnon node "<<MyName<<" encountered in function splitConjunctionIntoLMISet in DAGEliminator.cpp\n";
     exit(1); 
    }
}



// Version of setsEqual for strings
bool setsEqual(set<string> &A, set<string> &B)
{
set<string> Diff1;
set<string> Diff2;

set_difference(A.begin(), A.end(), B.begin(), B.end(),inserter(Diff1, Diff1.begin()));
set_difference(B.begin(), B.end(), A.begin(), A.end(),inserter(Diff2, Diff2.begin()));

if(Diff1.empty() && Diff2.empty()) return true;
return false;
}



// Given a dag "root" of the form c1.x1+...+cn.xn+c0, obtain the map xi--->ci 
// in VariableCoefficientMap, and c0 in ConstantPart
void obtainVariableCoefficientMap(DAGNode* root, map<string, ULINT> &VariableCoefficientMap, ULINT &ConstantPart)
{
set<string> Support;
getDAGNames(root, Support);
  
  // initialize the VariableCoefficientMap with Coefficients 1
  for(set<string>::iterator it=Support.begin(); it != Support.end(); it++)
	{
	string Variable = *it;
	ULINT Coefficient = 1;
	VariableCoefficientMap.insert(make_pair(Variable, Coefficient));
	}

   // Get the actual coefficients
   updateVariableCoefficientMap(root, VariableCoefficientMap);
   
   ConstantPart=0;
   getConstantModified(root, ConstantPart);
}



// Split the given set of constraints into sets of LMEs, LMDs, and LMIs
void splitConstraintsIntoLMELMDLMISets(set<DAGNode*> &Constraints, set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet)
{
 for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
  {
      DAGNode* Constraint = *it;
      
      if(Constraint->Get_node_name()=="=")
      {
          LMESet.insert(Constraint);
      }
      else if(Constraint->Get_node_name()=="is_not_equal")
      {
          LMDSet.insert(Constraint);
      }
      else if(Constraint->Get_node_name()=="bvule" || Constraint->Get_node_name()=="bvuge"  || Constraint->Get_node_name()=="bvult" || Constraint->Get_node_name()=="bvugt")
      {
          LMISet.insert(Constraint);
      }
      else
      {
          cout<<"\nUnknown constraint type "<<Constraint->Get_node_name()<<" in function splitConstraintsIntoLMELMDLMISets  in Eliminator.cpp\n";
          exit(1);
      }
  }//for
}// function


// Version of splitConjunctionIntoLMISet except that we need to take all APs, not just LMIs
void splitConjunctionIntoSetGeneral(DAGNode* Constraint, set<DAGNode*> &Conjuncts)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt" || MyName == "=" || MyName == "is_not_equal" ) // terminal reached
    {
      Conjuncts.insert(Constraint);
    }

  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoSetGeneral in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoSetGeneral(*iout, Conjuncts);
      
      iout++;

      splitConjunctionIntoSetGeneral(*iout, Conjuncts);
    }
  else if(MyName != "true")
    {
     cout<<"\nunkwnon node "<<MyName<<" encountered in function splitConjunctionIntoSetGeneral in Eliminator.cpp\n";
     exit(1); 
    }
}



// Given a set of LMCs, LMIs and a variable to be eliminated, this function first converts the 
// LMCs, LMIs into matrix representation, and then calls the matrix based LME based simplification
DAGNode* callMatrixQEliminatorForLMCsAndLMIs(set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet, string VariableToElim, map<string, int> &WidthTable)
{
    
   set<DAGNode*> Constraints;
   set_union(LMESet.begin(), LMESet.end(), LMDSet.begin(), LMDSet.end(), inserter(Constraints, Constraints.begin())); 
   
   // Apply LME based simplification for LMEs and LMDs
   
    DAGNode* MostConstrainingLME;
    
    DAGNode* LMELMDPart = callMatrixQEliminatorOnlyWithLayer1CAV11(Constraints, VariableToElim, WidthTable, MostConstrainingLME);
    
    if(LMELMDPart==NULL)
        {
        LMELMDPart = createLogicalConstant("true");
        }
    else
        {
        LMELMDPart = quickSimplify(LMELMDPart);
        }
    
    //cout<<"\nResult of QE from LMEs and LMDs = "<<getDAGStr(LMELMDPart)<<endl;
    //cout<<"\nMost constraining LME = "<<getDAGStr(MostConstrainingLME)<<endl;
        
    DAGNode* LMIPart = callMatrixQEliminatorForLMIs(LMESet, LMISet, VariableToElim, WidthTable);
    //cout<<"\nResult of QE from LMIs = "<<getDAGStr(LMIPart)<<endl;
    
    DAGNode* resultDAG;
    
    if(isTrue(LMELMDPart))
    {
        resultDAG = LMIPart;
    }
    else if(isTrue(LMIPart))
    {
        resultDAG = LMELMDPart;        
    }
    else
    {
        resultDAG = createDAG("and", LMELMDPart, LMIPart, LOGICAL_SYMBOL, 1);
    }  
    
    
    set<string> Support_resultDAG;
    getDAGNames(resultDAG, Support_resultDAG);
    if(Support_resultDAG.find(VariableToElim) != Support_resultDAG.end())// There are still constraints with var to elim
        {
        resultDAG = createDAG("and", resultDAG, MostConstrainingLME, LOGICAL_SYMBOL, 1);
        }    
    
    return resultDAG;
}


// Given a set of LMIs and LMEs and a variable to be eliminated, this function first converts them
//  into matrix representation, and then calls the matrix based QEliminator to simplify the LMIs
DAGNode* callMatrixQEliminatorForLMIs(set<DAGNode*> &LMESet, set<DAGNode*> &LMISet, string VariableToElim, map<string, int> &WidthTable)
{
   set<DAGNode*> Constraints;
   set_union(LMESet.begin(), LMESet.end(), LMISet.begin(), LMISet.end(), inserter(Constraints, Constraints.begin()));     
    
  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfInEqs;
  Matrix EqCoefMat, EqConstMat, InEqLHSCoefMat, InEqLHSConstMat, InEqRHSCoefMat, InEqRHSConstMat;
  vector<string> LMITypes;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();
  names.insert(VariableToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    getDAGNames(*it, names);


  attachSetToList(names, VarNames);

  NoVars=VarNames.size();


  // Get the equations and inequations
  bool result = getEquationsAndInequations(LMESet, LMISet, VarNames, NoOfBits, NoOfEqs, NoOfInEqs, EqCoefMat, EqConstMat, InEqLHSCoefMat, InEqLHSConstMat, InEqRHSCoefMat, InEqRHSConstMat, WidthTable, LMITypes);

  //cout<<"\nEqCoefMat\n";
  //showMatrix(EqCoefMat);
  //cout<<"\nEqConstMat\n";
  //showMatrix(EqConstMat);
  //cout<<"\nInEqLHSCoefMat\n";
  //showMatrix(InEqLHSCoefMat);
  //cout<<"\nInEqLHSConstMat\n";
  //showMatrix(InEqLHSConstMat);
  //cout<<"\nInEqRHSCoefMat\n";
  //showMatrix(InEqRHSCoefMat);
  //cout<<"\nInEqRHSConstMat\n";
  //showMatrix(InEqRHSConstMat);
  
  ULINT Mod=findPower(NoOfBits);

  
  DAGNode *resultDAG;
   

  resultDAG = qEliminatorForInequations(EqCoefMat, EqConstMat, InEqLHSCoefMat, InEqLHSConstMat, InEqRHSCoefMat, InEqRHSConstMat, NoVarsToElim, Mod, LMITypes, VarNames);
 
  
  //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;
  
  resultDAG = quickSimplify_WithLMISupport(resultDAG);
  
  //cout<<"\nresultDAG after quickSimplify = "<<getDAGStr(resultDAG)<<endl;
  
  return resultDAG;
}


// Given a set of DAGNodes, which are LMIs and LMEs, convert them into matrix representation
// A pair of matrices for LMEs, and four matrices for LMIs, and one vector for types of LMIs are generated
bool getEquationsAndInequations(set<DAGNode*> &LMESet, set<DAGNode*> &LMISet, list<string> &VarNames, int &NoOfBits, int &NoOfEqs, int &NoOfInEqs, Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &InEqLHSCoefMat, Matrix &InEqLHSConstMat, Matrix &InEqRHSCoefMat, Matrix &InEqRHSConstMat, map<string, int> &WidthTable, vector<string> &LMITypes)
{

  set<DAGNode*> Constraints;
  set_union(LMESet.begin(), LMESet.end(), LMISet.begin(), LMISet.end(), inserter(Constraints, Constraints.begin()));       
    
  NoOfEqs=0;
  NoOfInEqs=0;
  NoOfBits=0;

  int NoOfBitsInConstraint;

  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
      DAGNode* root = (*it);

      NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);

      if(NoOfBitsInConstraint > NoOfBits)
        NoOfBits=NoOfBitsInConstraint;
    }


   for(set<DAGNode*>::iterator it=LMESet.begin(); it != LMESet.end(); it++)
    {
        DAGNode* root = (*it);
        
        NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);
        
        DAGNode *lhs, *rhs;
        list<DAGNode*>::iterator out_list_it = root->Get_outlist_start();
        lhs = (*out_list_it);
        out_list_it++;
        rhs = (*out_list_it);

        Matrix A, C; // Row matrices
        Matrix B, D; // Row matrices

        getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
        getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D
        
        ULINT LocalMod=findPower(NoOfBitsInConstraint);
        ULINT GlobalMod=findPower(NoOfBits);
        ULINT Multiple=findPower(NoOfBits-NoOfBitsInConstraint);

        matSub(A, C, LocalMod);
        matSub(D, B, LocalMod);
        
        scalarMatMultiply(A, Multiple, GlobalMod);
        scalarMatMultiply(D, Multiple, GlobalMod);
        
         addRow(EqCoefMat,A);
         addRow(EqConstMat,D);
         NoOfEqs++;
   }
  
   for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* root = (*it);
        
        LMITypes.push_back(root->Get_node_name());
        
        NoOfBitsInConstraint=getNoOfBitsInConstraint(root, WidthTable);
        
        DAGNode *lhs, *rhs;
        list<DAGNode*>::iterator out_list_it = root->Get_outlist_start();
        lhs = (*out_list_it);
        out_list_it++;
        rhs = (*out_list_it);

        Matrix A, C; // Row matrices
        Matrix B, D; // Row matrices

        getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
        getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D
        
        ULINT LocalMod=findPower(NoOfBitsInConstraint);
        ULINT GlobalMod=findPower(NoOfBits);
        ULINT Multiple=findPower(NoOfBits-NoOfBitsInConstraint);

           
        scalarMatMultiply(A, Multiple, GlobalMod);
        scalarMatMultiply(B, Multiple, GlobalMod);
        scalarMatMultiply(C, Multiple, GlobalMod);
        scalarMatMultiply(D, Multiple, GlobalMod);
        
         addRow(InEqLHSCoefMat,A);
         addRow(InEqLHSConstMat,B);
         addRow(InEqRHSCoefMat,C);
         addRow(InEqRHSConstMat,D);
         
         NoOfInEqs++;
   }


  return true;

}


 

// The function simplifies LMIs using LMEs
DAGNode* qEliminatorForInequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &InEqLHSCoefMat, Matrix &InEqLHSConstMat, Matrix &InEqRHSCoefMat, Matrix &InEqRHSConstMat,int NoVarsToElim, ULINT Mod, vector<string> &LMITypes, list<string> &VarNames)
{
    
  bool show_zero_nonzero_elements = false;
  if(show_zero_nonzero_elements)
    {
    cout<<"\nEqCoefMat\n"  ;
    countZeroNonZeroElements(EqCoefMat);
    cout<<"\nInEqLHSCoefMat\n"  ;
    countZeroNonZeroElements(InEqLHSCoefMat);
    cout<<"\nInEqRHSCoefMat\n"  ;
    countZeroNonZeroElements(InEqRHSCoefMat);
    }
    
    
    
    
// Here first NoVarsToElim of the variables is to be eliminated

int VarToElimIndex, EqnToSubstIndex;

// NoOfVarsEliminated is the no: of variables from x1,...,xM-1 already elimiated

//cout<<"\nTrying to eliminate variables with odd coefficients first\n";

 for(VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++) 
   {

     if(EqCoefMat.empty()) 
       {
       //cout<<"\nNo equations left\n"; 
       break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has odd coefficient \n";
     
     getEquationWithOddCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex); 

     if(EqnToSubstIndex==-1) {
                              //cout<<"\nNo equation with odd coefficient for the variable in function qEliminatorForInequations in Eliminator.cpp\n";
                              break;
			      }

     else
	{
        //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";

	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";

	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnInEqLHS, PivotColumnInEqRHS;;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow

	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);
        
        // Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef = "<<PivotCoef<<endl;

	// For inequations

	if(!InEqLHSCoefMat.empty())
	  {
	    copyColumn(InEqLHSCoefMat, VarToElimIndex, PivotColumnInEqLHS); // selected column copied into PivotColumnInEqLHS
	
	    // PivotColumn nullified in InEqLHSCoefMat
	    nullifyColumn(InEqLHSCoefMat, VarToElimIndex);
	
	    Matrix NewInEqLHSCoefMat;
	    // Multiply PivotColumnInEqLHS and PivotRow to get NewInEqLHSCoefMat
	    matMultiply(PivotColumnInEqLHS, PivotRow, NewInEqLHSCoefMat, Mod);
	    //cout<<"\nNewInEqLHSCoefMat\n";
	    //showMatrix(NewInEqLHSCoefMat);
	    
	    // InEqLHSCoefMat =  InEqLHSCoefMat + NewInEqLHSCoefMat
	    matAdd(InEqLHSCoefMat, NewInEqLHSCoefMat, Mod);
	    //cout<<"\nInEqLHSCoefMat\n";
	    //showMatrix(InEqLHSCoefMat);
	

	    Matrix NewPivotColumnInEqLHS;            
            copyMatrix(PivotColumnInEqLHS, NewPivotColumnInEqLHS);
	    
            //cout<<"\nNewPivotColumnInEqLHS*PivotCoef\n";
	    scalarMatMultiply(NewPivotColumnInEqLHS, PivotCoef, Mod);
	    //cout<<"\nNewPivotColumnInEqLHS\n";
	    //showMatrix(NewPivotColumnInEqLHS);
	    
	    // InEqLHSConstMat = InEqLHSConstMat + NewPivotColumnInEqLHS
	    matAdd(InEqLHSConstMat, NewPivotColumnInEqLHS, Mod);
	    //cout<<"\nInEqLHSConstMat\n";
	    //showMatrix(InEqLHSConstMat);
	    
            
            
            copyColumn(InEqRHSCoefMat, VarToElimIndex, PivotColumnInEqRHS); // selected column copied into PivotColumnInEqRHS
	
	    // PivotColumn nullified in InEqRHSCoefMat
	    nullifyColumn(InEqRHSCoefMat, VarToElimIndex);
	
	    Matrix NewInEqRHSCoefMat;
	    // Multiply PivotColumnInEqRHS and PivotRow to get NewInEqRHSCoefMat
	    matMultiply(PivotColumnInEqRHS, PivotRow, NewInEqRHSCoefMat, Mod);
	    //cout<<"\nNewInEqRHSCoefMat\n";
	    //showMatrix(NewInEqRHSCoefMat);
	    
	    // InEqRHSCoefMat =  InEqRHSCoefMat + NewInEqRHSCoefMat
	    matAdd(InEqRHSCoefMat, NewInEqRHSCoefMat, Mod);//cout<<"\nNo equations left\n"; 
	    //cout<<"\nInEqRHSCoefMat\n";
	    //showMatrix(InEqRHSCoefMat);
	

	    Matrix NewPivotColumnInEqRHS;
            copyMatrix(PivotColumnInEqRHS, NewPivotColumnInEqRHS);
	    
            //cout<<"\nNewPivotColumnInEqRHS*PivotCoef\n";
	    scalarMatMultiply(NewPivotColumnInEqRHS, PivotCoef, Mod);
	    //cout<<"\nNewPivotColumnInEqRHS\n";
	    //showMatrix(NewPivotColumnInEqRHS);
	    
	    // InEqRHSConstMat = InEqRHSConstMat + PivotColumnInEqRHS
	    matAdd(InEqRHSConstMat, NewPivotColumnInEqRHS, Mod);
	    //cout<<"\nInEqRHSConstMat\n";
	    //showMatrix(InEqRHSConstMat);
	    
            
	    //cout<<"\nAfter elimination, we have\n";
            
            //cout<<"\nInEqLHSCoefMat\n";
            //showMatrix(InEqLHSCoefMat);
            //cout<<"\nInEqLHSConstMat\n";
            //showMatrix(InEqLHSConstMat);
            //cout<<"\nInEqRHSCoefMat\n";
            //showMatrix(InEqRHSCoefMat);
            //cout<<"\nInEqRHSConstMat\n";
            //showMatrix(InEqRHSConstMat);
            

            // Show the inequations
            
	  }//if(!InEqLHSCoefMat.empty())
	
	}//else
   }//for



 //cout<<"\nTrying to eliminate variables with even coefficients\n";

 for(int VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++)
   {

     if(EqCoefMat.empty()) 
       {
         break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has even coefficient \n";

     getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex, Mod); 
	
     //cout<<"\nEqnToSubstIndex= "<<EqnToSubstIndex<<endl;

     if(EqnToSubstIndex==-1) 
        {
         //cout<<"\nNo equation with even coefficient for the variable.\n";
         break;
        }

     if(EqCoefMat[EqnToSubstIndex][VarToElimIndex] == 0) 
	{
         //cout<<"\nNo equation with even coefficient for the variable.\n";
         break;
        }

     else
	{
        //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";

	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";
        
        ULINT PowerOfPivotElement=powerOfTwo(PivotElement);
	//cout<<"\nPower Of Pivot Element = "<<PowerOfPivotElement<<"\n";

	PivotElement=PivotElement/findPower(PowerOfPivotElement);
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";

	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnInEqLHS, PivotColumnInEqRHS;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow

	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);
        //cout<<"\nPivotRow after nullification\n";
	//showMatrix(PivotRow);

	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);
        
        // Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef = "<<PivotCoef<<endl;

	// For inequations

	if(!InEqLHSCoefMat.empty())
	  {
	    copyColumn(InEqLHSCoefMat, VarToElimIndex, PivotColumnInEqLHS); // selected column copied into PivotColumnInEqLHS
            //cout<<"\nPivotColumnInEqLHS\n";
            //showMatrix(PivotColumnInEqLHS);
            
            // Dividing the PivotColumnInEqLHS by power(2, PowerOfPivotElement). Note that  PivotColumnInEqLHS is not always divisible. If not divisible, element is set to zero
            divideColumnByPowerOf2IfDivisible(PivotColumnInEqLHS, 0, PowerOfPivotElement);
            //cout<<"\nPivotColumnInEqLHS after dividing by PowerOfPivotElement\n";
            //showMatrix(PivotColumnInEqLHS);
	            
            // If entry is not divisible by 2^PowerOfPivotElement, then keep it. Else make it zero.
            modifyDisEqCoefMat(InEqLHSCoefMat, VarToElimIndex, PowerOfPivotElement);	
            //cout<<"\nInEqLHSCoefMat after modification\n";
            //showMatrix(InEqLHSCoefMat);

	    
            Matrix NewInEqLHSCoefMat;
	    // Multiply PivotColumnInEqLHS and PivotRow to get NewInEqLHSCoefMat
	    matMultiply(PivotColumnInEqLHS, PivotRow, NewInEqLHSCoefMat, Mod);
	    //cout<<"\nNewInEqLHSCoefMat\n";
	    //showMatrix(NewInEqLHSCoefMat);
	    
	    // InEqLHSCoefMat =  InEqLHSCoefMat + NewInEqLHSCoefMat
	    matAdd(InEqLHSCoefMat, NewInEqLHSCoefMat, Mod);
	    //cout<<"\nInEqLHSCoefMat\n";
	    //showMatrix(InEqLHSCoefMat);
	

	    Matrix NewPivotColumnInEqLHS;            
            copyMatrix(PivotColumnInEqLHS, NewPivotColumnInEqLHS);
	    
            //cout<<"\nNewPivotColumnInEqLHS*PivotCoef\n";
	    scalarMatMultiply(NewPivotColumnInEqLHS, PivotCoef, Mod);
	    //cout<<"\nNewPivotColumnInEqLHS\n";
	    //showMatrix(NewPivotColumnInEqLHS);
	    
	    // InEqLHSConstMat = InEqLHSConstMat + NewPivotColumnInEqLHS
	    matAdd(InEqLHSConstMat, NewPivotColumnInEqLHS, Mod);
	    //cout<<"\nInEqLHSConstMat\n";
	    //showMatrix(InEqLHSConstMat);
	    
            
            
            
            copyColumn(InEqRHSCoefMat, VarToElimIndex, PivotColumnInEqRHS); // selected column copied into PivotColumnInEqRHS
            //cout<<"\nPivotColumnInEqRHS\n";
            //showMatrix(PivotColumnInEqRHS);
            
            // Dividing the PivotColumnInEqRHS by power(2, PowerOfPivotElement). Note that  PivotColumnInEqRHS is not always divisible. If not divisible, element is set to zero
            divideColumnByPowerOf2IfDivisible(PivotColumnInEqRHS, 0, PowerOfPivotElement);
            //cout<<"\nPivotColumnInEqRHS after dividing by PowerOfPivotElement\n";
            //showMatrix(PivotColumnInEqRHS);
 	            
            // If entry is not divisible by 2^PowerOfPivotElement, then keep it. Else make it zero.
            modifyDisEqCoefMat(InEqRHSCoefMat, VarToElimIndex, PowerOfPivotElement);	
            //cout<<"\nInEqRHSCoefMat after modification\n";
            //showMatrix(InEqRHSCoefMat);
            
	    
            Matrix NewInEqRHSCoefMat;
	    // Multiply PivotColumnInEqRHS and PivotRow to get NewInEqRHSCoefMat
	    matMultiply(PivotColumnInEqRHS, PivotRow, NewInEqRHSCoefMat, Mod);
	    //cout<<"\nNewInEqRHSCoefMat\n";
	    //showMatrix(NewInEqRHSCoefMat);
	    
	    // InEqRHSCoefMat =  InEqRHSCoefMat + NewInEqRHSCoefMat
	    matAdd(InEqRHSCoefMat, NewInEqRHSCoefMat, Mod);
	    //cout<<"\nInEqRHSCoefMat\n";
	    //showMatrix(InEqRHSCoefMat);
	

	    Matrix NewPivotColumnInEqRHS;            
            copyMatrix(PivotColumnInEqRHS, NewPivotColumnInEqRHS);
	    
            //cout<<"\nNewPivotColumnInEqRHS*PivotCoef\n";
	    scalarMatMultiply(NewPivotColumnInEqRHS, PivotCoef, Mod);
	    //cout<<"\nNewPivotColumnInEqRHS\n";
	    //showMatrix(NewPivotColumnInEqRHS);
	    
	    // InEqRHSConstMat = InEqRHSConstMat + NewPivotColumnInEqRHS
	    matAdd(InEqRHSConstMat, NewPivotColumnInEqRHS, Mod);
	    //cout<<"\nInEqRHSConstMat\n";
	    //showMatrix(InEqRHSConstMat);
            
            
                      
	    //cout<<"\nAfter elimination, we have\n";
            
            //cout<<"\nInEqLHSCoefMat\n";
            //showMatrix(InEqLHSCoefMat);
            //cout<<"\nInEqLHSConstMat\n";
            //showMatrix(InEqLHSConstMat);
            //cout<<"\nInEqRHSCoefMat\n";
            //showMatrix(InEqRHSCoefMat);
            //cout<<"\nInEqRHSConstMat\n";
            //showMatrix(InEqRHSConstMat);
            

            // Show the inequations
            
	  }//if(!InEqRHSCoefMat.empty())
        }//else
    }//for

         
       

// Convert matrices to dag and return

DAGNode* resultDAG;

if(InEqLHSCoefMat.empty())
    {
    resultDAG = createLogicalConstant("true");
    }
else
    {
    resultDAG = getDAGFromInequations(InEqLHSCoefMat, InEqLHSConstMat, InEqRHSCoefMat, InEqRHSConstMat, VarNames, Mod, LMITypes);
    }
return resultDAG;


}// function



// Creates a dag from a conjunction of inequations specified as matrices, and returns it	
DAGNode* getDAGFromInequations(Matrix &InEqLHSCoefMat, Matrix &InEqLHSConstMat, Matrix &InEqRHSCoefMat, Matrix &InEqRHSConstMat, list<string> &VarNames, ULINT Mod, vector<string> &LMITypes)
	{
	int width;
	width = powerOfTwo(Mod);

	list<string>::iterator it;

        DAGNode* result=NULL;
	
	int rows = InEqLHSCoefMat.size();
	int cols = 0;

	if(rows != 0)
		{
		cols = InEqLHSCoefMat[0].size();
		}
        
        if(rows != LMITypes.size())
            {
            cout<<"\nNumber of rows in function getDAGFromInequations in Eliminator.cpp is not the same as the number of inequations\n";
            exit(1);
            }


	for(int i=0; i<rows; i++)
		{ 
	        DAGNode *lhscoefterm=NULL, *lhsconstterm;
                DAGNode *rhscoefterm=NULL, *rhsconstterm;
                DAGNode *lhs=NULL, *rhs=NULL, *ineq;
                
	        it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
			string varname = *it;
			DAGNode *variable = createBVVariable(varname, width);
			char coeff[20];

			sprintf(coeff, "%llu", InEqLHSCoefMat[i][j]);
			string coeff_str(coeff);
			DAGNode *coefficient = createBVConstant(coeff_str,width);
			DAGNode *term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
			if(lhscoefterm==NULL) lhscoefterm=term; else lhscoefterm=createDAG("bvadd", lhscoefterm, term, BITVECTOR_SYMBOL, width);
			}
		char constant[10];
		sprintf(constant, "%llu", InEqLHSConstMat[i][0]);
		string constant_str(constant);
		lhsconstterm = createBVConstant(constant_str,width);
                
                lhs =createDAG("bvadd", lhscoefterm, lhsconstterm, BITVECTOR_SYMBOL, width);
                
                it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
			string varname = *it;
			DAGNode *variable = createBVVariable(varname, width);
			char coeff[20];

			sprintf(coeff, "%llu", InEqRHSCoefMat[i][j]);
			string coeff_str(coeff);
			DAGNode *coefficient = createBVConstant(coeff_str,width);
			DAGNode *term = createDAG("bvmul", coefficient, variable, BITVECTOR_SYMBOL, width);
			if(rhscoefterm==NULL) rhscoefterm=term; else rhscoefterm=createDAG("bvadd", rhscoefterm, term, BITVECTOR_SYMBOL, width);
			}
		sprintf(constant, "%llu", InEqRHSConstMat[i][0]);
		constant_str = constant;
		rhsconstterm = createBVConstant(constant_str,width);
                
                rhs =createDAG("bvadd", rhscoefterm, rhsconstterm, BITVECTOR_SYMBOL, width);
                
                string LMIType = LMITypes[i];
                
		ineq = createDAG(LMIType, lhs, rhs, LOGICAL_SYMBOL, 1);
                
		if(result==NULL) result=ineq; else result = createDAG("and", result, ineq, LOGICAL_SYMBOL, 1);
		}
	return result;
	
}	


// Version of setsEqual for DAGNode*
bool setsEqual(set<DAGNode*> &A, set<DAGNode*> &B)
{
set<DAGNode*> Diff1;
set<DAGNode*> Diff2;

set_difference(A.begin(), A.end(), B.begin(), B.end(),inserter(Diff1, Diff1.begin()));
set_difference(B.begin(), B.end(), A.begin(), A.end(),inserter(Diff2, Diff2.begin()));

if(Diff1.empty() && Diff2.empty()) return true;
return false;
}




// Modification of getCoefficient which can operate on any dag.
// Remember that getCoefficient operates on dags of the form c1.x1 + .... + cn.xn + c0
void getCoefficientCorrected(DAGNode *node, string Var, ULINT &Coefficient)
{
    
 set<string> Support;
 getDAGNames(node, Support);
 
 if(Support.find(Var)==Support.end())// node is free if Var
    {
    Coefficient = 0;    
    }
 else
    {
    // node has Var. It can happen that there's no explicit multiplication
    Coefficient = 1;
    getCoefficient(node, Var, Coefficient);
    }
}
    

// checks if the given node is false
bool isFalse(DAGNode *node)
{
 if (node->Get_node_name()=="false") return true;
 return false;
}


// Show a vector of DAGNode* nodes
void showAPVector(string WhoamI, vector<DAGNode*> &APVector)
{
  cout<<"\n"<<WhoamI<<"\n"<<"**************"<<"\n";
  for(int i=0;i<APVector.size(); i++)
    {
      cout<<endl<<getDAGStr(APVector[i])<<endl;
    }
}


// Show a vector of DAGNode* nodes in given file
void showAPVector(string WhoamI, vector<DAGNode*> &APVector, FILE* fp)
{
  fprintf(fp, "\n%s\n************\n", WhoamI.c_str());
  for(int i=0;i<APVector.size(); i++)
    {
      fprintf(fp, "\n%s\n", getDAGStr(APVector[i]).c_str());
    }
}


// Nullify all columns except the one indexed by  SelColumn 
void nullifyColumnsExceptOne(Matrix &A, int SelColumn)
{
int rows = A.size();
int cols = 0;

if(rows != 0)
	{
	cols = A[0].size();
	}

for(int i=0; i<rows; i++)
		{
                  for(int j=0; j<cols; j++)
                  {
                      if(j!=SelColumn)
                      {
                      A[i][j] = 0;
                      }//if
                  }//for		  
		}//for
}


// Copy the contents of the set to vector
void copySetToVector(set<DAGNode*> &MySet, vector<DAGNode*> &MyVector)
{
    for(set<DAGNode*>::iterator it=MySet.begin(); it!=MySet.end(); it++)
    {
        MyVector.push_back(*it);
    }
}




// Version of quickSimplify which handles LMI operators also
DAGNode*  quickSimplify_WithLMISupport(DAGNode *node)
{
  struct timeval step_start_ms, step_finish_ms;    
  unsigned long long int step_time;
  if(PrintLayer2Times)
    {
    gettimeofday (&step_start_ms, NULL);        
    }
    
  HashTable<string, DAGNode*> DPTable;
  
   if(PrintLayer2Times)
    {
    gettimeofday (&step_finish_ms, NULL);
    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;  
    fprintf(LMIQEOutputDataFp, "quick simplify preparation time = %llu\t", step_time);
    }    
  
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable; 
  
  DAGNode *ret = quickSimplify_WithLMISupportInternal(node, PtrDPTable);
  return ret;
}
  
  

// Function called by quickSimplify_WithLMISupport to simplify the dag node "node" based on some simple syntactic rules  
// Returns "node" with the syntactic simplification rules applied
DAGNode*  quickSimplify_WithLMISupportInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable)
{
  DAGNode* ret;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

  struct timeval step_start_ms, step_finish_ms;    
  unsigned long long int step_time;

  if(PrintLayer2Times)
    {
    gettimeofday (&step_start_ms, NULL);        
    }

  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }


  unsigned int outlist_size = node->Get_outlist_size();
  
  if(outlist_size == 0) { // Terminals
	  ret = node;
  }

  else
  {
    string dag_str = node->Get_node_name();
    if(!(dag_str == "bvule" || dag_str == "bvuge" || dag_str == "bvult" || dag_str == "bvugt" || dag_str == "and" || dag_str == "or" || dag_str=="if_then_else" || dag_str == "not"|| dag_str == "bvadd"|| dag_str == "bvsub"|| dag_str == "bvmul"|| dag_str == "=" || dag_str == "is_not_equal"))
      {
	//cout<<dag_str<<"node encountered. The nodes down cannot be simplified\n";
	ret = node;
      } 
    else
      {
	vector<DAGNode*> SimplifiedChildren;
	list<DAGNode*>::iterator iout = node->Get_outlist_start();
    

	for(unsigned int i=0;i<outlist_size; i++,iout++)
	  {
	    SimplifiedChildren.push_back(quickSimplify_WithLMISupportInternal(*iout, DPTable));
	  }

   
	if(dag_str == "and" && outlist_size==2){// binary and
	  if((SimplifiedChildren[0])->Get_node_name()=="true"|| (SimplifiedChildren[1])->Get_node_name()=="false")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="true" || (SimplifiedChildren[0])->Get_node_name()=="false")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("and", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1) ;
	}
	else if(dag_str == "or" && outlist_size==2){// binary or
	  if((SimplifiedChildren[0])->Get_node_name()=="false"|| (SimplifiedChildren[1])->Get_node_name()=="true")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="false" || (SimplifiedChildren[0])->Get_node_name()=="true")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("or", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);
	}
	else if(dag_str == "and" && outlist_size>2){// and with more than two children
	  
	    ret = simplifyAndWithMultipleChildren(SimplifiedChildren);
	}
	else if(dag_str == "or" && outlist_size>2){// or with more than two children
	  
	    ret = simplifyOrWithMultipleChildren(SimplifiedChildren);
	}
	else if(dag_str == "not"){
	  if((SimplifiedChildren[0])->Get_node_name()=="false")
	    ret = createLogicalConstant("true");
	  else if((SimplifiedChildren[0])->Get_node_name()=="true")
	    ret = createLogicalConstant("false");
	  else
	    ret = createDAG("not", SimplifiedChildren[0], LOGICAL_SYMBOL, 1);
	}

	else if(dag_str == "if_then_else"){
	  if((SimplifiedChildren[0])->Get_node_name()=="false")
	    ret = SimplifiedChildren[2];
	  else if((SimplifiedChildren[0])->Get_node_name()=="true")
	    ret = SimplifiedChildren[1];
	  else if(SimplifiedChildren[1] == SimplifiedChildren[2])
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="false" && (SimplifiedChildren[2])->Get_node_name()=="true")
	    ret = createDAG("not", SimplifiedChildren[0], LOGICAL_SYMBOL, 1);
	  else if((SimplifiedChildren[1])->Get_node_name()=="true" && (SimplifiedChildren[2])->Get_node_name()=="false")
	    ret = SimplifiedChildren[0];
	  else 
	    ret = createDAG("if_then_else", SimplifiedChildren[0], SimplifiedChildren[1], SimplifiedChildren[2], LOGICAL_SYMBOL, 1);
	}


	else if(dag_str == "bvadd"){
	  if((SimplifiedChildren[0])->Get_node_name()=="0")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("bvadd", SimplifiedChildren[0], SimplifiedChildren[1], BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	}
	else if(dag_str == "bvsub"){
	  if((SimplifiedChildren[1])->Get_node_name()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("bvsub", SimplifiedChildren[0], SimplifiedChildren[1], BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	}
	else if(dag_str == "bvmul"){
	  if((SimplifiedChildren[0])->Get_node_name()=="1" || (SimplifiedChildren[1])->Get_node_name()=="0")
	    ret = SimplifiedChildren[1];
	  else if((SimplifiedChildren[1])->Get_node_name()=="1"|| (SimplifiedChildren[0])->Get_node_name()=="0")
	    ret = SimplifiedChildren[0];
	  else
	    ret = createDAG("bvmul", SimplifiedChildren[0], SimplifiedChildren[1], BITVECTOR_SYMBOL, IMMATERIAL_WIDTH);
	}
	else if(dag_str == "="){
	  if(SimplifiedChildren[0] == SimplifiedChildren[1])
	    ret = createLogicalConstant("true");
	  else if((SimplifiedChildren[0]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[0]))) && (SimplifiedChildren[1]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[1])))) // both operands are constants
	    ret = createLogicalConstant("false");
	  else
	    ret = createDAG("=", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);	 
     
	}
	else if(dag_str == "is_not_equal"){
	  if(SimplifiedChildren[0] == SimplifiedChildren[1])
	    ret = createLogicalConstant("false");
	  else if((SimplifiedChildren[0]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[0]))) && (SimplifiedChildren[1]->Get_outlist_size()==0 && (isConstant(SimplifiedChildren[1])))) // both operands are constants
	    ret = createLogicalConstant("true");
	  else
	    ret = createDAG("is_not_equal", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);;	 
     
	}
        else if(dag_str == "bvule" || dag_str == "bvuge" || dag_str == "bvult" || dag_str == "bvugt")
        {
            char resultLMIExpression = booleanLMIExpression(dag_str, SimplifiedChildren[0], SimplifiedChildren[1]);
            
            if(resultLMIExpression=='t')
                ret = createLogicalConstant("true");
            else if(resultLMIExpression=='f')
                ret = createLogicalConstant("false");
            else
                ret = createDAG(dag_str, SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1); 
        }
	else
	  {
	    cout<<dag_str<<"node encountered in Function  quickSimplify_WithLMISupportInternal\n";
	    exit(1);
	  }
      }

  }
  result = DPTable->hashtable_insert(key, ret);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function  quickSimplify_WithLMISupportInternal\n";
      exit(1);
    }
  
  
  if(PrintLayer2Times)
    {
    gettimeofday (&step_finish_ms, NULL);
    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;  
    fprintf(LMIQEOutputDataFp, "quick simplify internal time = %llu\t", step_time);
    }
  
  return ret;
}


// Evaluates an LMI expression where the operands are constants
char booleanLMIExpression(string LMIType, DAGNode* LHS, DAGNode* RHS)
{
      if(LMIType == "bvule" || LMIType == "bvuge")
      {
          if(LHS == RHS)
          {
              return 't';
          }
      }
    
    if(!isConstant(LHS) || !isConstant(RHS))
    {
        return 'x';
    }
    ULINT LHS_Value;
    ULINT RHS_Value;
    
    string ValueString = LHS->Get_node_name();
    LHS_Value =strtoull(ValueString.c_str(), NULL, 0);
    
    ValueString = RHS->Get_node_name();
    RHS_Value =strtoull(ValueString.c_str(), NULL, 0);
    
    if(LMIType == "bvule")
    {
        if(LHS_Value <= RHS_Value)
            return 't';
        else
            return 'f';
    }
    else  if(LMIType == "bvuge")
    {
        if(LHS_Value >= RHS_Value)
            return 't';
        else
            return 'f';
    }
    else if(LMIType == "bvult")
    {
        if(LHS_Value < RHS_Value)
            return 't';
        else
            return 'f';
    }
    else if(LMIType == "bvugt")
    {
        if(LHS_Value > RHS_Value)
            return 't';
        else
            return 'f';
    }
    else
    {
        cout<<"\nUnknown LMIType "<<LMIType<<" in function booleanLMIExpression in Eliminator.cpp\n";
        exit(1);
    }
}


// Given a set of constraints, converts them into matrices and then
// qEliminatorReturningDAGOnlyWithLayer1CAV11 is called on the matrices
// qEliminatorReturningDAGOnlyWithLayer1CAV11 is the layer1 of CAV'11 work
DAGNode* callMatrixQEliminatorOnlyWithLayer1CAV11(set<DAGNode*> &Constraints, string VariableToElim, map<string, int> &WidthTable, DAGNode* &PivotEquation)
{
  // Write in matrix form

  int NoVars, NoVarsToElim, NoOfBits, NoOfEqs, NoOfDisEqs;
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  // Get the list of variables,  NoVars, NoVarsToElim
  list<string> VarNames;
  set<string> names;

  names.clear();
  names.insert(VariableToElim);
  attachSetToList(names, VarNames);
  NoVarsToElim=1;

  names.clear();
  // we should include variables from all equations and disequations in names
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    getDAGNames(*it, names);


  attachSetToList(names, VarNames);

  NoVars=VarNames.size();


  // Get the equations and disequations
  bool result = getEquationsAndDisequations(Constraints, VarNames, NoOfBits, NoOfEqs, NoOfDisEqs, EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, WidthTable);

  ULINT Mod=findPower(NoOfBits);

  DAGNode *resultDAG;
  DAGNode *FreePart;
  DAGNode *BoundPart;
  

  resultDAG = qEliminatorReturningDAGOnlyWithLayer1CAV11(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, false, VarNames, WidthTable, FreePart, BoundPart, PivotEquation);

  
  //cout<<"\nFreePart = "<<getDAGStr(FreePart)<<endl;
  //cout<<"\nBoundPart = "<<getDAGStr(BoundPart)<<endl;
  //cout<<"\nPivotEquation = "<<getDAGStr(PivotEquation)<<endl;
  
  
  return resultDAG;
  
}



// Apply Layer1 of CAV'11 on the matrices
DAGNode* qEliminatorReturningDAGOnlyWithLayer1CAV11(Matrix &EqCoefMatPassed, Matrix &EqConstMatPassed, Matrix &DisEqCoefMatPassed, Matrix &DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, list<string> &VarNames, map<string, int> &WidthTable, DAGNode* &Phi1, DAGNode* &Phi2, DAGNode* &PivotEquation)
{
    
  bool show_zero_nonzero_elements = false;
  if(show_zero_nonzero_elements)
    {
    cout<<"\nEqCoefMatPassed\n"  ;
    countZeroNonZeroElements(EqCoefMatPassed);
    cout<<"\nDisEqCoefMatPassed\n"  ;
    countZeroNonZeroElements(DisEqCoefMatPassed);
    }  
    
    
  
  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  Phi1=NULL;
  Phi2=NULL;
  
  Matrix MyPivotRow;
  ULINT MyPivotCoef;

  if(!EqCoefMat.empty())
    {    
    qEliminatorForEquationsAndDisequationsWhichReturnsPivotEquation(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod, MyPivotRow, MyPivotCoef); // Eliminate first NoVarsToElim variables from the disequations CX<>D and equations AX=B and get back the matrices
    }
  else
    {
    cout<<"\nEqCoefMat.empty() in qEliminatorReturningDAGOnlyWithLayer1CAV11 in Eliminator.cpp\n";
    exit(1);
    }
  
  if(MyPivotRow.empty())
  {
      cout<<"\nPivot Row becoming empty in qEliminatorReturningDAGOnlyWithLayer1CAV11 in Eliminator.cpp\n";
      exit(1);
  }  
  
  Matrix MyPivotRowConstantPart;
  getNullMatrix(MyPivotRowConstantPart, 1, 1);
  MyPivotRowConstantPart[0][0] = MyPivotCoef;
  PivotEquation = getDAGFromEqs(MyPivotRow, MyPivotRowConstantPart, VarNames, Mod); 
  
  PivotEquation = quickSimplify(PivotEquation);
  
  // This function finds the disequations in which there are no variables left to eliminate, removes them from set of disequations and copies them in a matrices DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat
  
  Matrix DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat;
  Matrix EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat;
  
  if(!DisEqCoefMat.empty()) eliminateDisequationsFreeOfAllVarsToElimAndCopyInNewMatrix(DisEqCoefMat, DisEqConstMat, NoVarsToElim, DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat);

  if(!EqCoefMat.empty()) eliminateEquationsFreeOfAllVarsToElimAndCopyInNewMatrix(EqCoefMat, EqConstMat, NoVarsToElim, EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat);

  
  DAGNode *DagFromFreeEqs, *DagFromFreeDiseqs;
  DagFromFreeEqs = getDAGFromEqs(EqFreeOfAllVarsToElimCoefMat, EqFreeOfAllVarsToElimConstMat, VarNames, Mod);
  DagFromFreeDiseqs = getDAGFromDisEqs(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat, VarNames, Mod);
  	
    if(DagFromFreeEqs != NULL)
	{
	if(Phi1 == NULL) Phi1=DagFromFreeEqs; 
	else 
	  { 
	    Phi1 = createDAG("and",Phi1, DagFromFreeEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromFreeDiseqs != NULL)
	{
      	if(Phi1 == NULL) Phi1=DagFromFreeDiseqs; 
	else 
	  {
	    Phi1 = createDAG("and",Phi1, DagFromFreeDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}

    if(Phi1 == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      Phi1 = dm.createNode("true",(void*)v);
	    }
  
  //cout<<"\n###############################\nPrinting the Equalities containing any variable to eliminate\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nPrinting the Disequalities containing any variable to eliminate\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  //cout<<"\n###############################\n";
 
  
   DAGNode *DagFromEqs, *DagFromDiseqs;
   DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, VarNames, Mod);
   DagFromDiseqs = getDAGFromDisEqs(DisEqCoefMat, DisEqConstMat, VarNames, Mod);
  	
    if(DagFromEqs != NULL)
	{
	if(Phi2 == NULL) Phi2=DagFromEqs; 
	else 
	  { 
	    Phi2 = createDAG("and",Phi2, DagFromEqs, LOGICAL_SYMBOL, 1);
	  }
	}
      if(DagFromDiseqs != NULL)
	{
      	if(Phi2 == NULL) Phi2=DagFromDiseqs; 
	else 
	  {
	    Phi2 = createDAG("and",Phi2, DagFromDiseqs, LOGICAL_SYMBOL, 1);
	  }
	}

    if(Phi2 == NULL) 
	    {
	      Value *v = new Value();
	      Type *t = new Type();
	      t->data_type = LOGICAL_CONSTANT;
	      t->width = 1;
	      v->set_node_type(*t);
	      Phi2 = dm.createNode("true",(void*)v);
	    }

    

   Phi1 = quickSimplify(Phi1);
   Phi2 = quickSimplify(Phi2);

   //cout<<"\nPhi1 = "<<getDAGStr(Phi1)<<endl;
   //cout<<"\nPhi2 = "<<getDAGStr(Phi2)<<endl;

   DAGNode *Phi; // final result


   if(isTrue(Phi1)) // Phi1 is true. Phi = Phi2
	{
	Phi = Phi2;
	}
   else if(isTrue(Phi2)) // Phi2 is true. Phi = Phi1
	{
	Phi = Phi1;
	}
   else // Phi = Phi1 /\ Phi2
	{
	Phi = createDAG("and",Phi1, Phi2, LOGICAL_SYMBOL, 1);
	} 

   //cout<<"\nPhi = "<<getDAGStr(Phi)<<endl;

   return Phi;
 
}



// Version of  qEliminatorForEquationsAndDisequations, which returns the pivot LME
void qEliminatorForEquationsAndDisequationsWhichReturnsPivotEquation(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod, Matrix &MyPivotRow, ULINT &MyPivotCoef)
{

// Here first NoVarsToElim of the variables is to be eliminated

int VarToElimIndex, EqnToSubstIndex;

// NoOfVarsEliminated is the no: of variables from x1,...,xM-1 already elimiated

//cout<<"\nTrying to eliminate variables with odd coefficients first\n";

 for(VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++) 
   {

     if(EqCoefMat.empty()) 
       {
	 //cout<<"\nNo equations left\n"; 
       break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has odd coefficient \n";
     
     getEquationWithOddCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex); 

     if(EqnToSubstIndex==-1) {
                              //cout<<"\nNo equation with odd coefficient for the variable\n";
			      }

     else
	{
	  //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";

	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";

	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnEq, PivotColumnDiseq;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow
        
        MyPivotRow = PivotRow;
        
        MyPivotCoef = PivotCoef;

	//cout<<"\nChecking if the equation is solvable\n";
	// Check if the Equation selected is solvable. If no, return saying unsat
	bool checking_unsat_of_equation = false;
	if(checking_unsat_of_equation)
		{
		if(!equationSolvable(PivotRow, PivotCoef, Mod))
	  		{cout<<"\nThe equation selected is unsolvable. Hence set of equations is unsat. Exiting...\n"; exit(1);}
		}


	copyColumn(EqCoefMat, VarToElimIndex, PivotColumnEq); // selected column copied into PivotColumnEqn
	
	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	// delete pivot element from pivot column of equations as this would lead to equation with all coefs set to zero 
	deleteRow(PivotColumnEq, EqnToSubstIndex);


	// PivotRow deleted from EqCoefMat
	deleteRow(EqCoefMat, EqnToSubstIndex);
	// PivotRow deleted from EqConstMat
	deleteRow(EqConstMat, EqnToSubstIndex);

	// PivotColumn nullified in EqCoefMat
	nullifyColumn(EqCoefMat, VarToElimIndex);


	//cout<<"\nAfter deletions\n";
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);

	Matrix NewEqCoefMat;
	// Multiply PivotColumnEq and PivotRow to get NewEqCoefMat
	matMultiply(PivotColumnEq, PivotRow, NewEqCoefMat, Mod);
	//cout<<"\nNewEqCoefMat\n";
	//showMatrix(NewEqCoefMat);

	// EqCoefMat =  EqCoefMat + NewEqCoefMat
	matAdd(EqCoefMat, NewEqCoefMat, Mod);
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);


	// Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef*MI(Pivot element) = "<<PivotCoef<<"\n";

	// Convert PivotColumnEq to PivotColumnEq*PivotCoef
	scalarMatMultiply(PivotColumnEq, PivotCoef, Mod);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	// EqConstMat = EqConstMat-PivotColumnEq
	matSub(EqConstMat, PivotColumnEq, Mod);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);

	//cout<<"\nAfter elimination, we have\n";
	//showEquations(EqCoefMat, EqConstMat);


	// For disequations

	if(!DisEqCoefMat.empty())
	  {
	    copyColumn(DisEqCoefMat, VarToElimIndex, PivotColumnDiseq); // selected column copied into PivotColumnDiseqn
	
	    // PivotColumn nullified in DisEqCoefMat
	    nullifyColumn(DisEqCoefMat, VarToElimIndex);
	
	    Matrix NewDisEqCoefMat;
	    // Multiply PivotColumnDiseq and PivotRow to get NewDisEqCoefMat
	    matMultiply(PivotColumnDiseq, PivotRow, NewDisEqCoefMat, Mod);
	    //cout<<"\nNewDisEqCoefMat\n";
	    //showMatrix(NewDisEqCoefMat);
	    
	    // DisEqCoefMat =  DisEqCoefMat + NewDisEqCoefMat
	    matAdd(DisEqCoefMat, NewDisEqCoefMat, Mod);
	    //cout<<"\nDisEqCoefMat\n";
	    //showMatrix(DisEqCoefMat);
	

	    // Convert PivotColumnDiseq to PivotColumnDiseq*PivotCoef
	    //cout<<"\nPivotColumnDiseq*PivotCoef\n";
	    scalarMatMultiply(PivotColumnDiseq, PivotCoef, Mod);
	    //cout<<"\nPivotColumnDiseq\n";
	    //showMatrix(PivotColumnDiseq);
	    
	    // DisEqConstMat = DisEqConstMat - PivotColumnDiseq
	    matSub(DisEqConstMat, PivotColumnDiseq, Mod);
	    //cout<<"\nDisEqConstMat\n";
	    //showMatrix(DisEqConstMat);
	    
	    //cout<<"\nAfter elimination, we have\n";
	    //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
	  }
	
	}
   }

 //cout<<"\nTrying to eliminate variables with even coefficients\n";

 for(int VarToElimIndex=0; VarToElimIndex < NoVarsToElim; VarToElimIndex++)
   {

     if(EqCoefMat.empty()) 
       {
	 //cout<<"\nNo equations left\n"; 
	 break;
       }

     //cout<<"\nChecking if there exists an equation where the variable x"<<VarToElimIndex<<" has even coefficient \n";

     getEquationWithLeastEvenCoefForGivenVariable(EqCoefMat, VarToElimIndex, EqnToSubstIndex, Mod); 
	
     //cout<<"\nEqnToSubstIndex= "<<EqnToSubstIndex<<endl;

    if(EqnToSubstIndex==-1) continue;

     if(EqCoefMat[EqnToSubstIndex][VarToElimIndex] == 0) 
	{cout<<"\nNo equation with even coefficient for the variable.\n";}

     else
	{
	  //cout<<"\nThere exists an equation where the variable has coefficient "<<EqCoefMat[EqnToSubstIndex][VarToElimIndex]<<"\n";
		
	ULINT PivotElement=EqCoefMat[EqnToSubstIndex][VarToElimIndex]; 
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";
	//cout<<"\nDividing Pivot Element by it's power of 2\n";
	
	ULINT PowerOfPivotElement=powerOfTwo(PivotElement);
	//cout<<"\nPower Of Pivot Element = "<<PowerOfPivotElement<<"\n";

	PivotElement=PivotElement/findPower(PowerOfPivotElement);
	//cout<<"\nPivot Element = "<<PivotElement<<"\n";
	
	ULINT PivotCoef=EqConstMat[EqnToSubstIndex][0];
	//cout<<"\nPivot Coefficient = "<<PivotCoef<<"\n";

	Matrix PivotRow, PivotColumnEq, PivotColumnDiseq;

	copyRow(EqCoefMat, EqnToSubstIndex, PivotRow); // selected equation copied into PivotRow
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
        
        
        MyPivotRow = PivotRow;
        
        MyPivotCoef = PivotCoef;
	

	// Check if the Equation selected is solvable. If no, return saying unsat
	bool checking_unsat_of_equation = false;
	if(checking_unsat_of_equation)
		{
		if(!equationSolvable(PivotRow, PivotCoef, Mod))
		  	{cout<<"\nThe equation selected is unsolvable. Hence set of equations is unsat. Exiting...\n"; exit(1);}
		}

	
	// As the coeff. selected is even, we need to add a new equality into the EqCoefMat

	Matrix NewEquality; // Note that this is a row matrix
	
	copyRow(EqCoefMat, EqnToSubstIndex, NewEquality); // selected equation copied into NewEquality
     
	// Pivot element nullified in  NewEquality 
	nullifyColumn(NewEquality, VarToElimIndex);

	// Convert NewEquality to AI(NewEquality)
	aInverseOfMatrix(NewEquality, Mod); 

	// Convert NewEquality to AI(NewEquality)*pow(2, powerOfTwo(Mod)-PowerOfPivotElement)
	scalarMatMultiply(NewEquality, findPower(powerOfTwo(Mod)-PowerOfPivotElement), Mod);

	// The constant part of the new equality in NewEqualityConst
	ULINT NewEqualityConst=PivotCoef;

	// Convert NewEqualityConst to NewEqualityConst*pow(2, powerOfTwo(Mod)-PowerOfPivotElement)
	NewEqualityConst = mul(NewEqualityConst, findPower(powerOfTwo(Mod)-PowerOfPivotElement), Mod);

	// Convert NewEqualityConst to AI(NewEqualityConst*pow(2, powerOfTwo(Mod)-PowerOfPivotElement))
	NewEqualityConst = aInverse(NewEqualityConst, Mod);
	// Now NewEquality = NewEqualityConst is a new equality to be added to EqCoefMat at the end of eliminating the variable

	
	// Rest is exactly like the way for odd coefficient
	
	copyColumn(EqCoefMat, VarToElimIndex, PivotColumnEq); // selected column copied into PivotColumnEqn
	
	// Dividing the PivotColumnEq by power(2, PowerOfPivotElement). Note that PivotColumnEq is always divisible
	divideColumnByPowerOf2IfDivisible(PivotColumnEq, 0, PowerOfPivotElement);
	
	
	// Pivot element set to zero in PivotRow 
	nullifyColumn(PivotRow, VarToElimIndex);

	// delete pivot element from pivot column of equations as this would lead to equation with all coefs set to zero 
	deleteRow(PivotColumnEq, EqnToSubstIndex);


	// PivotRow deleted from EqCoefMat
	deleteRow(EqCoefMat, EqnToSubstIndex);
	// PivotRow deleted from EqConstMat
	deleteRow(EqConstMat, EqnToSubstIndex);

	// PivotColumn nullified in EqCoefMat
	nullifyColumn(EqCoefMat, VarToElimIndex);
	
	

	//cout<<"\nAfter deletions\n";
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);
	//cout<<"\nPivotRow\n";
	//showMatrix(PivotRow);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	ULINT MIPivotElement;
	// Get MI of Pivot element in MIPivotElement
	MIPivotElement=mInverse(PivotElement, Mod);
	//cout<<"\nMI(Pivot element) = "<<MIPivotElement<<"\n";
	
	// Convert PivotRow to AI(PivotRow)
	aInverseOfMatrix(PivotRow, Mod);
	//cout<<"\nAI(PivotRow)\n";
	//showMatrix(PivotRow);

	// Convert PivotRow to AI(PivotRow)*MI(Pivot element)
	scalarMatMultiply(PivotRow, MIPivotElement, Mod);
	//cout<<"\nAI(PivotRow)*MI(Pivot element)\n";
	//showMatrix(PivotRow);

	Matrix NewEqCoefMat;
	// Multiply PivotColumnEq and PivotRow to get NewEqCoefMat
	matMultiply(PivotColumnEq, PivotRow, NewEqCoefMat, Mod);
	//cout<<"\nNewEqCoefMat\n";
	//showMatrix(NewEqCoefMat);

	// EqCoefMat =  EqCoefMat + NewEqCoefMat
	matAdd(EqCoefMat, NewEqCoefMat, Mod);
	//cout<<"\nEqCoefMat\n";
	//showMatrix(EqCoefMat);


	// Convert PivotCoef to PivotCoef*MI(Pivot element)
	PivotCoef = mul(PivotCoef, MIPivotElement, Mod);
	//cout<<"\nPivotCoef*MI(Pivot element) = "<<PivotCoef<<"\n";

	// Convert PivotColumnEq to PivotColumnEq*PivotCoef
	scalarMatMultiply(PivotColumnEq, PivotCoef, Mod);
	//cout<<"\nPivotColumnEq\n";
	//showMatrix(PivotColumnEq);


	// EqConstMat = EqConstMat-PivotColumnEq
	matSub(EqConstMat, PivotColumnEq, Mod);
	//cout<<"\nEqConstMat\n";
	//showMatrix(EqConstMat);

	//cout<<"\nAfter elimination, we have\n";
	//showEquations(EqCoefMat, EqConstMat);


	//cout<<"\nThe new equality is to be added\n";	
	//cout<<"\nChecking if new equality is to be added as the coefficient is even\n";
	 // Now NewEquality = NewEqualityConst is a new equality to be added to EqCoefMat and EqConstMat at the end of eliminating the variable unless all coefs in NewEquality are zero

     if(!IsNullMatrix(NewEquality))
       {
         //cout<<"\nEquality is added from QElimSingleLME\n";
	 addRow(EqCoefMat, NewEquality); // NewEquality is a matrix
	 addElementToColumnMatrix(EqConstMat, NewEqualityConst);
	}
     else
       {
	 if(NewEqualityConst==0)
	   {
             //cout<<"\nNew equality not added as it is 0 = 0\n";
            }
	 else
	   {cout<<"\nNull matrix <> 0 inside qEliminator function. Exiting...\n"; exit(1);}
       }

     //cout<<"\nAfter adding new equality, we have\n";
     //showEquations(EqCoefMat, EqConstMat);

	// For disequations

       
     if(!DisEqCoefMat.empty())
       {
	
	 copyColumn(DisEqCoefMat, VarToElimIndex, PivotColumnDiseq); // selected column copied into PivotColumnDiseqn
     
         // Dividing the PivotColumnDiseq by power(2, PowerOfPivotElement). Note that PivotColumnDiseq is not always divisible. If not divisible, element is set to zero
	divideColumnByPowerOf2IfDivisible(PivotColumnDiseq, 0, PowerOfPivotElement);

	// For DisEqCoefMat, in the column corresponding to the variable to be eliminated, if entry is not divisible by 2^PowerOfPivotElement, then keep it. Else make it zero.
	modifyDisEqCoefMat(DisEqCoefMat, VarToElimIndex, PowerOfPivotElement);	

	Matrix NewDisEqCoefMat;
	// Multiply PivotColumnDiseq and PivotRow to get NewDisEqCoefMat
	matMultiply(PivotColumnDiseq, PivotRow, NewDisEqCoefMat, Mod);
	//cout<<"\nNewDisEqCoefMat\n";
	//showMatrix(NewDisEqCoefMat);
	
	// DisEqCoefMat =  DisEqCoefMat + NewDisEqCoefMat
	matAdd(DisEqCoefMat, NewDisEqCoefMat, Mod);
	//cout<<"\nDisEqCoefMat\n";
	//showMatrix(DisEqCoefMat);
	

	// Convert PivotColumnDiseq to PivotColumnDiseq*PivotCoef
	scalarMatMultiply(PivotColumnDiseq, PivotCoef, Mod);
	//cout<<"\nPivotColumnDiseq\n";
	//showMatrix(PivotColumnDiseq);
	
	// DisEqConstMat = DisEqConstMat - PivotColumnDiseq
	matSub(DisEqConstMat, PivotColumnDiseq, Mod);
	//cout<<"\nDisEqConstMat\n";
	//showMatrix(DisEqConstMat);
	
	//cout<<"\nAfter elimination, we have\n";
	//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
       }

	}
   }
  
}

// returns true if the dag node "node" contains the variable VarToElim; false otherwise
bool isBound(DAGNode* node, string VarToElim)
{
struct timeval step_start_ms, step_finish_ms;    
unsigned long long int step_time;

if(PrintLayer2Times)
    {
    gettimeofday (&step_start_ms, NULL);        
    }

bool call_quicksimplify_to_avoid_trivial_occurances = false;
//I do not think this call is needed, as we are calling quickSimplify before
// Monniuax/QE_LMDD always

if(call_quicksimplify_to_avoid_trivial_occurances)
    {
    node = quickSimplify_WithLMISupport(node);//To avoid trivial occurances
    }

if(PrintLayer2Times)
    {
    gettimeofday (&step_finish_ms, NULL);
    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;  
    }

if(PrintLayer2Times)
    {
        fprintf(LMIQEOutputDataFp, "quick simplify time = %llu\t", step_time);
    }

if(PrintLayer2Times)
    {
    gettimeofday (&step_start_ms, NULL); 
    }

set<string> names;
getDAGNames(node, names);

if(PrintLayer2Times)
    {
    gettimeofday (&step_finish_ms, NULL);
    step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
    step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;   
    }

if(PrintLayer2Times)
    {
        fprintf(LMIQEOutputDataFp, "getDAGNames time = %llu\t", step_time);
    }

if(names.find(VarToElim) == names.end())  return false;
return true;
}


// Find minimum of i and j
ULINT findMinimum(ULINT i, ULINT j)
{
   //cout<<"\ni="<<i<<"\tj="<<j<<endl;
  if(i<=j) return i;
  else return j;
}



// Find maximum of i and j
ULINT findMaximum(ULINT i, ULINT j)
{
  if(i>=j) return i;
  else return j;
}




// Version of splitConjunctionIntoSetGeneral, except that the output is obtained in a vector
void splitConjunctionIntoVectorGeneral(DAGNode* Constraint, vector<DAGNode*> &Conjuncts)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt" || MyName == "=" || MyName == "is_not_equal" ) // terminal reached
    {
      Conjuncts.push_back(Constraint);
    }

  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoVectorGeneral in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoVectorGeneral(*iout, Conjuncts);
      
      iout++;

      splitConjunctionIntoVectorGeneral(*iout, Conjuncts);
    }
  else if(MyName != "true")
    {
     cout<<"\nunkwnon node "<<MyName<<" encountered in function splitConjunctionIntoVectorGeneral in Eliminator.cpp\n";
     exit(1); 
    }
}





// Given an equation, convert it to the form c1.x1+....+cn.xn = dn
DAGNode* recreateEquationInOriginalForm(DAGNode* equation, map<string, int> &WidthTable)
{
  // Get the list of variables
  list<string> VarNames;
  set<string> names;
  
  getDAGNames(equation, names);
  attachSetToList(names, VarNames);
  VarNames.sort();

  DAGNode *lhs, *rhs;
  list<DAGNode*>::iterator out_list_it = equation->Get_outlist_start();
  lhs = (*out_list_it);
  out_list_it++;
  rhs = (*out_list_it);
  
  if(lhs==rhs) return equation;
  if(names.empty()) return equation;

 
  int NoOfBitsInConstraint=getNoOfBitsInConstraint(equation, WidthTable);

  //cout<<"\nNo: of bits in constraint = "<<NoOfBitsInConstraint<<endl;

//   DAGNode *lhs, *rhs;
//   list<DAGNode*>::iterator out_list_it = equation->Get_outlist_start();
//   lhs = (*out_list_it);
//   out_list_it++;
//   rhs = (*out_list_it);

  Matrix A, C; // Row matrices
  Matrix B, D; // Row matrices

  getCoefsAndConstsModified(lhs, VarNames, A, B); // get lhs as AX+B
  getCoefsAndConstsModified(rhs, VarNames, C, D); // get rhs as CX+D


   // We have AX+B = CX+D
  // Convert A to A-C and D to D-B to get (A-C)X = (D-B)

  ULINT LocalMod=findPower(NoOfBitsInConstraint);
 
  matSub(A, C, LocalMod);
  matSub(D, B, LocalMod);

  // Now equation is AX = D
  
  bool conversion_to_normal_form = false;
  
  if(conversion_to_normal_form)
  {
      // Converting the first element in coef mat to normal form

      ULINT FirstElement = A[0][0];

      ULINT OddPart = getOddPart(FirstElement);


      if(OddPart != 0)
        {

          ULINT MIOfOddPart = mInverse(OddPart, LocalMod);

          scalarMatMultiply(A, MIOfOddPart, LocalMod);

          scalarMatMultiply(D, MIOfOddPart, LocalMod);
        }


      // Now equation is AX = D with first element in normal form
  }
  
  DAGNode* RecreatedEquation;
  
  bool old_imp = false;
  if(old_imp)
	RecreatedEquation = getDAGFromEqs(A, D, VarNames, LocalMod);
  else
	RecreatedEquation = recreateDAGFromEqs(A, D, VarNames, LocalMod);
  

  return RecreatedEquation;

}



// Version of convertStrictLMIsToWeakLMIs with support for LMEs and LMDs
void convertStrictLMIsToWeakLMIs_WithLMDESupport(vector<DAGNode*> &Constraints, map<string, int> &WidthTable, set<string> &VarsToElim)
{
    vector<DAGNode*> NewConstraints;
    
    for(int i=0; i<Constraints.size(); i++)
    {
        DAGNode* Constraint = Constraints[i];
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
        string LMIType = Constraint->Get_node_name();
        
        if(LMIType == "=" || LMIType == "is_not_equal")
        {
           DAGNode* NewConstraint;           
           NewConstraint = Constraint;           
           NewConstraints.push_back(NewConstraint);   
        }              
        else if(LMIType == "bvule" || LMIType == "bvuge")//already weak
        {
         DAGNode* NewConstraint;         
         recreateWeakConstraint(Constraint, WidthTable, NewConstraint);  // This helps in proper ordering of terms         
         NewConstraints.push_back(NewConstraint);   
        }
        else//strict LMI
        {
            DAGNode* NewConstraint;
            
            if(LMIType=="bvugt") //LHS > RHS
            {
                 DAGNode* RecreatedConstraint1;
                 DAGNode* RecreatedConstraint2;
                 
                 getRecreatedBVUGTConstraintsModified(LHS, RHS, WidthTable, RecreatedConstraint1, RecreatedConstraint2, VarsToElim);
                 
                 if(!isTrue(RecreatedConstraint1))
                 {
                   NewConstraints.push_back(RecreatedConstraint1);    
                 }
               NewConstraints.push_back(RecreatedConstraint2);    
            }//if(LMIType=="bvugt")
            else  if(LMIType=="bvult") //We have LHS < RHS. Recreate using RHS > LHS
            {
                 DAGNode* RecreatedConstraint1;
                 DAGNode* RecreatedConstraint2;
                 
                 getRecreatedBVUGTConstraintsModified(RHS, LHS, WidthTable, RecreatedConstraint1, RecreatedConstraint2, VarsToElim);
                 
                 if(!isTrue(RecreatedConstraint1))
                 {
                   NewConstraints.push_back(RecreatedConstraint1);    
                 }
               NewConstraints.push_back(RecreatedConstraint2);    
            }//else if(LMIType=="bvult")
            else
            {
                cout<<"\nUnknown predicate "<<LMIType<<" in function convertStrictLMIsToWeakLMIs_WithLMDESupport in Eliminator.cpp\n";
                exit(1);
            }//else of else if(LMIType=="bvult")
        } // else of strict LMI
    }//for
    
    
    Constraints = NewConstraints;
    
}//function




// Recreate the constraint with proper ordering of terms
void recreateWeakConstraint(DAGNode *Constraint, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint)
{
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
        
    string LMIType = Constraint->Get_node_name();
    
    
    set<string> Support_Constraint;
    getDAGNames(LHS, Support_Constraint);
    getDAGNames(RHS, Support_Constraint);
    
    if(Support_Constraint.empty())
    {
        cout<<"\nEmpty support for constraint "<<getDAGStr(Constraint)<<" in function recreateWeakConstraint in Eliminator.cpp\n";
        exit(1);
    }
    set<string>::iterator it = Support_Constraint.begin();
    string Variable = *it;
    map<string, int>::iterator wit = WidthTable.find(Variable);
    if(wit == WidthTable.end())
    {
        cout<<"\nNo entry for the variable "<<Variable<<" in WidthTable in function recreateWeakConstraint in Eliminator.cpp\n";
        exit(1);
    }
    int WidthOfVar = wit->second;
    ULINT ModWidthOfVar = findPower(WidthOfVar);
    
    
    list<string> VarNames;
    attachSetToList(Support_Constraint, VarNames);

    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;

    getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
   
     vector<string> LMITypes;
     LMITypes.push_back(LMIType);
    
    RecreatedConstraint = quickSimplify(getDAGFromInequations(S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, VarNames, ModWidthOfVar, LMITypes));
}





// Slightly modified version of getRecreatedBVUGTConstraints
// Recreate LHS > RHS 
void getRecreatedBVUGTConstraintsModified(DAGNode *LHS, DAGNode *RHS, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint1, DAGNode* &RecreatedConstraint2, set<string> &VarsToElim)
{
    bool avoid_change_in_vartoelim = true;    
    
    set<string> Support_Constraint;
    getDAGNames(LHS, Support_Constraint);
    getDAGNames(RHS, Support_Constraint);
    
    if(Support_Constraint.empty())
    {
        cout<<"\nEmpty support for constraint in function getRecreatedBVUGTConstraintsModified in Eliminator.cpp\n";
        exit(1);
    }
    set<string>::iterator it = Support_Constraint.begin();
    string Variable = *it;
    map<string, int>::iterator wit = WidthTable.find(Variable);
    if(wit == WidthTable.end())
    {
        cout<<"\nNo entry for the variable "<<Variable<<" in WidthTable in function getRecreatedBVUGTConstraintsModified in Eliminator.cpp\n";
        exit(1);
    }
    int WidthOfVar = wit->second;
    ULINT ModWidthOfVar = findPower(WidthOfVar);
    
    ULINT MaxValue = ModWidthOfVar- 1;
    char MaxValue_char[10];
    sprintf(MaxValue_char, "%llu", MaxValue);
    string MaxValue_str(MaxValue_char);
    DAGNode *MaxValue_dag = createBVConstant(MaxValue_str, WidthOfVar);
    
    DAGNode *NewConstraint1 = quickSimplify(createDAG("is_not_equal", RHS, MaxValue_dag, LOGICAL_SYMBOL, 1));
    
    
    ULINT MinValue = 0;
    char MinValue_char[10];
    sprintf(MinValue_char, "%llu", MinValue);
    string MinValue_str(MinValue_char);
    DAGNode *MinValue_dag = createBVConstant(MinValue_str, WidthOfVar);
    
    DAGNode *NewConstraint2 = quickSimplify(createDAG("is_not_equal", LHS, MinValue_dag, LOGICAL_SYMBOL, 1));
 
    if(isTrue(NewConstraint1))
    {
        RecreatedConstraint1 = NewConstraint1;
        
         list<string> VarNames;
        attachSetToList(Support_Constraint, VarNames);
    
        Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;
    
        getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
        getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
        getUnitMatrix(Unit_Const_Mat, 1, 1);
    
        matAdd(T_Const_Mat, Unit_Const_Mat, ModWidthOfVar);
    
        vector<string> LMITypes;
        LMITypes.push_back("bvuge");
    
        RecreatedConstraint2 = quickSimplify(getDAGFromInequations(S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, VarNames, ModWidthOfVar, LMITypes));
        
        return;
    }
    else if(isFalse(NewConstraint1))
    {
       cout<<"\nLMI "<<getDAGStr(LHS) <<" > "<<getDAGStr(RHS)<<" becoming FALSE in function getRecreatedBVUGTConstraintsModified in Eliminator.cpp\n";
       exit(1);
    }
    else if(isTrue(NewConstraint2))
    {
        RecreatedConstraint1 = NewConstraint2;
        
        list<string> VarNames;
        attachSetToList(Support_Constraint, VarNames);
    
        Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;
    
        getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
        getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
        getUnitMatrix(Unit_Const_Mat, 1, 1);
    
        matSub(S_Const_Mat, Unit_Const_Mat, ModWidthOfVar);
    
        vector<string> LMITypes;
        LMITypes.push_back("bvuge");
    
        RecreatedConstraint2 = quickSimplify(getDAGFromInequations(S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, VarNames, ModWidthOfVar, LMITypes));
        
        return;
    }
    else if(isFalse(NewConstraint2))
    {
       cout<<"\nLMI "<<getDAGStr(LHS) <<" > "<<getDAGStr(RHS)<<" becoming FALSE in function getRecreatedBVUGTConstraintsModified in Eliminator.cpp\n";
       exit(1);
    }
    else
    {
        
       if(avoid_change_in_vartoelim)
       {
           set<string> Support_LHS, Support_RHS;
           getDAGNames(LHS, Support_LHS);
           getDAGNames(RHS, Support_RHS);
           
           bool RHS_Free_Of_VarsToElim;
           bool LHS_Free_Of_VarsToElim;
           
           set<string> LHSVarsToElim, RHSVarsToElim;
           
           set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_LHS.begin(), Support_LHS.end(),inserter(LHSVarsToElim, LHSVarsToElim.begin()));
           set_intersection(VarsToElim.begin(), VarsToElim.end(), Support_RHS.begin(), Support_RHS.end(),inserter(RHSVarsToElim, RHSVarsToElim.begin()));
           
           if(LHSVarsToElim.empty())
               LHS_Free_Of_VarsToElim = true;
           else
               LHS_Free_Of_VarsToElim = false;
           
           if(RHSVarsToElim.empty())
               RHS_Free_Of_VarsToElim = true;
           else
               RHS_Free_Of_VarsToElim = false;
           
           
           if(RHS_Free_Of_VarsToElim)
           {
                ULINT MaxValueLessOne = ModWidthOfVar- 2;
                char MaxValueLessOne_char[10];
                sprintf(MaxValueLessOne_char, "%llu", MaxValueLessOne);
                string MaxValueLessOne_str(MaxValueLessOne_char);
                DAGNode *MaxValueLessOne_dag = createBVConstant(MaxValueLessOne_str, WidthOfVar);

                RecreatedConstraint1 = quickSimplify(createDAG("bvule", RHS, MaxValueLessOne_dag, LOGICAL_SYMBOL, 1));

                 list<string> VarNames;
                attachSetToList(Support_Constraint, VarNames);

                Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;

                getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
                getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
                getUnitMatrix(Unit_Const_Mat, 1, 1);

                matAdd(T_Const_Mat, Unit_Const_Mat, ModWidthOfVar);

                vector<string> LMITypes;
                LMITypes.push_back("bvuge");

                RecreatedConstraint2 = quickSimplify(getDAGFromInequations(S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, VarNames, ModWidthOfVar, LMITypes));

                 return;   
           }
           else if(LHS_Free_Of_VarsToElim)
           {
                ULINT MinValuePlusOne = 1;
                char MinValuePlusOne_char[10];
                sprintf(MinValuePlusOne_char, "%llu", MinValuePlusOne);
                string MinValuePlusOne_str(MinValuePlusOne_char);
                DAGNode *MinValuePlusOne_dag = createBVConstant(MinValuePlusOne_str, WidthOfVar);

                RecreatedConstraint1 = quickSimplify(createDAG("bvuge", LHS, MinValuePlusOne_dag, LOGICAL_SYMBOL, 1));

                 list<string> VarNames;
                attachSetToList(Support_Constraint, VarNames);

                Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, MinusOne_Const_Mat;

                getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
                getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
                
                getNullMatrix(MinusOne_Const_Mat, 1, 1);
                MinusOne_Const_Mat[0][0]=ModWidthOfVar-1;
                
                matAdd(S_Const_Mat, MinusOne_Const_Mat, ModWidthOfVar);

                vector<string> LMITypes;
                LMITypes.push_back("bvule");

                RecreatedConstraint2 = quickSimplify(getDAGFromInequations(T_Coef_Mat, T_Const_Mat, S_Coef_Mat, S_Const_Mat, VarNames, ModWidthOfVar, LMITypes));

                return;  
           }
           else
           {
               ULINT MaxValueLessOne = ModWidthOfVar- 2;
               char MaxValueLessOne_char[10];
               sprintf(MaxValueLessOne_char, "%llu", MaxValueLessOne);
               string MaxValueLessOne_str(MaxValueLessOne_char);
               DAGNode *MaxValueLessOne_dag = createBVConstant(MaxValueLessOne_str, WidthOfVar);

               RecreatedConstraint1 = quickSimplify(createDAG("bvule", RHS, MaxValueLessOne_dag, LOGICAL_SYMBOL, 1));

               list<string> VarNames;
               
               attachSetToList(Support_Constraint, VarNames);
               Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;

               getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
               getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
               getUnitMatrix(Unit_Const_Mat, 1, 1);

               matAdd(T_Const_Mat, Unit_Const_Mat, ModWidthOfVar);

               vector<string> LMITypes;
               LMITypes.push_back("bvuge");

               RecreatedConstraint2 = quickSimplify(getDAGFromInequations(S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, VarNames, ModWidthOfVar, LMITypes));

               return;              
           }

       }//if(avoid_change_in_vartoelim)
       else
       {        
        ULINT MaxValueLessOne = ModWidthOfVar- 2;
        char MaxValueLessOne_char[10];
        sprintf(MaxValueLessOne_char, "%llu", MaxValueLessOne);
        string MaxValueLessOne_str(MaxValueLessOne_char);
        DAGNode *MaxValueLessOne_dag = createBVConstant(MaxValueLessOne_str, WidthOfVar);
    
        RecreatedConstraint1 = quickSimplify(createDAG("bvule", RHS, MaxValueLessOne_dag, LOGICAL_SYMBOL, 1));
        
         list<string> VarNames;
        attachSetToList(Support_Constraint, VarNames);
    
        Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;
    
        getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
        getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
        getUnitMatrix(Unit_Const_Mat, 1, 1);
    
        matAdd(T_Const_Mat, Unit_Const_Mat, ModWidthOfVar);
    
        vector<string> LMITypes;
        LMITypes.push_back("bvuge");
    
        RecreatedConstraint2 = quickSimplify(getDAGFromInequations(S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, VarNames, ModWidthOfVar, LMITypes));
  
         return;
       }// if(!avoid_change_in_vartoelim)
    }
    
     
}



// Convert the LMDEs to the form 2^k.x \bowtie t and do not change the LMIs
void convertLMDEsToProperForm(vector<DAGNode*> &Constraints, map<string, int> &WidthTable)
{
    vector<DAGNode*> NewConstraints;
    
    for(int i=0; i<Constraints.size(); i++)
    {
        DAGNode* Constraint = Constraints[i];
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
        string LMIType = Constraint->Get_node_name();
        
        if(LMIType == "=")
        {
           DAGNode* NewConstraint;           
           NewConstraint = recreateEquationInOriginalForm(Constraint, WidthTable);
           NewConstraints.push_back(NewConstraint);   
        }              
        else if(LMIType == "is_not_equal")
        {
         DAGNode* NewConstraint;         
         NewConstraint = negateConstraint(recreateEquationInOriginalForm(negateConstraint(Constraint), WidthTable));
         NewConstraints.push_back(NewConstraint);   
        }
        else//LMI
        {
          NewConstraints.push_back(Constraint);    
        } 
    }//for   
    
    Constraints = NewConstraints;    
}//function




// Version of simplifiableLMIPairFound_ForWeakLMIs which works on any weak constraints
bool simplifiableLMIPairFound_ForWeakLMIs_Advanced(DAGNode* uge, DAGNode* ule)
{
    list<DAGNode*>::iterator iout;
    iout = uge->Get_outlist_start();
    DAGNode *uge_LHS = *iout;
    iout++;
    DAGNode *uge_RHS = *iout;	
    
    iout = ule->Get_outlist_start();
    DAGNode *ule_LHS = *iout;
    iout++;
    DAGNode *ule_RHS = *iout;	
    
    if(uge_LHS != ule_LHS)
    {
        return false;
    }
    
     if(uge_RHS != ule_RHS)
    {
        return false;
    }
    
   return true;
}


// Version of simplifiableLMIPairFound_ForWeakLMIs which works on LMIs of the same type
bool simplifiableLMIPairFound_ForWeakLMIs_Advanced_With_Same_Operator(DAGNode* first, DAGNode* second)
{
    list<DAGNode*>::iterator iout;
    iout = first->Get_outlist_start();
    DAGNode *first_LHS = *iout;
    iout++;
    DAGNode *first_RHS = *iout;	
    
    iout = second->Get_outlist_start();
    DAGNode *second_LHS = *iout;
    iout++;
    DAGNode *second_RHS = *iout;	
    
    if(first_LHS == second_RHS && first_RHS == second_LHS)
    {
        return true;
    }
    
    return false;
}




// Try to synthesize LMEs from LMIs 
void tryToSynthesizeLMEsFromLMIs(vector<DAGNode*> &Constraints,  set<string> &VarsToElim,  map<string, int> &WidthTable)
{    
 vector<DAGNode*> BVULE_Vector;// set of constraints in Constraints of the form <=
 vector<DAGNode*> BVUGE_Vector;// Set of constraints in Constraints of the form >=
 vector<DAGNode*> Remaining_Vector; // Set of remaining Constraints
 
 for(int i=0; i<Constraints.size(); i++)
        {
           DAGNode* MyConstraint = Constraints[i];
           
           if(MyConstraint->Get_node_name()=="bvule")
           {
               BVULE_Vector.push_back(MyConstraint);
           }
           else if(MyConstraint->Get_node_name()=="bvuge")
           {
               BVUGE_Vector.push_back(MyConstraint);
           }
           else
           {
                Remaining_Vector.push_back(MyConstraint);
           }             
           
        }//for ends here
 
 
 //showAPVector("BVULE_Vector", BVULE_Vector);
 //showAPVector("BVUGE_Vector", BVUGE_Vector);
 //showAPVector("Remaining_Vector", Remaining_Vector);
      

set<DAGNode*> AlreadyDone;
AlreadyDone.clear();

vector< pair<DAGNode*, DAGNode*> > SimplifiableLMIPairs;

while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<BVUGE_Vector.size();i++)
	{
          DAGNode* uge = BVUGE_Vector[i];
          //cout<<"\nTaking LMI "<<getDAGStr(uge)<<endl;
          if(AlreadyDone.find(uge)!=AlreadyDone.end()) // We have already checked if there exists a pair for uge
		{
		//cout<<"\nAlready Done\n";
		continue;
		}
          
          for(int j=0; j<BVULE_Vector.size(); j++)
          {
              DAGNode* ule = BVULE_Vector[j];
              
              if(simplifiableLMIPairFound_ForWeakLMIs_Advanced(uge, ule))
              {
                  //cout<<"\nMatches with "<<getDAGStr(ule)<<endl;
                  //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
                  SimplifiableLMIPairs.push_back(make_pair(ule, uge));
                  BVUGE_Vector.erase(BVUGE_Vector.begin()+i);
                  BVULE_Vector.erase(BVULE_Vector.begin()+j);
                  fixpoint_reached=false;
                  break;
              }
          }
	
        if(!fixpoint_reached)// BVUGE_Vector is changed. Restart the loop
        {
            //cout<<"\nStarting from the beginning of BVUGE_Vector\n";
            break;
        }
        
        //cout<<"\nNo matching ule constraint found\n";
	AlreadyDone.insert(uge);
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here



AlreadyDone.clear();
while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<BVUGE_Vector.size();i++)
	{
          DAGNode* first = BVUGE_Vector[i];
          //cout<<"\nTaking LMI "<<getDAGStr(first)<<endl;
          if(AlreadyDone.find(first)!=AlreadyDone.end()) // We have already checked if there exists a pair for first
		{
		//cout<<"\nAlready Done\n";
		continue;
		}
          
          for(int j=i+1; j<BVUGE_Vector.size(); j++)
          {
              DAGNode* second = BVUGE_Vector[j];
              
              if(simplifiableLMIPairFound_ForWeakLMIs_Advanced_With_Same_Operator(first, second))
              {
                  //cout<<"\nMatches with "<<getDAGStr(second)<<endl;
                  //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
                  SimplifiableLMIPairs.push_back(make_pair(first, second));
                  BVUGE_Vector.erase(BVUGE_Vector.begin()+j);
                  BVUGE_Vector.erase(BVUGE_Vector.begin()+i);// Important to delete from j^{th} position first as j>i
                  fixpoint_reached=false;
                  break;
              }
          }
	
        if(!fixpoint_reached)// BVUGE_Vector is changed. Restart the loop
        {
            //cout<<"\nStarting from the beginning\n";
            break;
        }
        
        //cout<<"\nNo matching constraint found\n";
	AlreadyDone.insert(first);
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here



AlreadyDone.clear();
while(true)
    {
      bool fixpoint_reached=true;

      for(int i=0;i<BVULE_Vector.size();i++)
	{
          DAGNode* first = BVULE_Vector[i];
          //cout<<"\nTaking LMI "<<getDAGStr(first)<<endl;
          if(AlreadyDone.find(first)!=AlreadyDone.end()) // We have already checked if there exists a pair for first
		{
		//cout<<"\nAlready Done\n";
		continue;
		}
          
          for(int j=i+1; j<BVULE_Vector.size(); j++)
          {
              DAGNode* second = BVULE_Vector[j];
              
              if(simplifiableLMIPairFound_ForWeakLMIs_Advanced_With_Same_Operator(first, second))
              {
                  //cout<<"\nMatches with "<<getDAGStr(second)<<endl;
                  //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
                  SimplifiableLMIPairs.push_back(make_pair(first, second));
                  BVULE_Vector.erase(BVULE_Vector.begin()+j);
                  BVULE_Vector.erase(BVULE_Vector.begin()+i);// Important to delete from j^{th} position first as j>i
                  fixpoint_reached=false;
                  break;
              }
          }
	
        if(!fixpoint_reached)// BVULE_Vector is changed. Restart the loop
        {
            //cout<<"\nStarting from the beginning\n";
            break;
        }
        
        //cout<<"\nNo matching constraint found\n";
	AlreadyDone.insert(first);
	}// for ends here
      
      if(fixpoint_reached)
	{
	  break;
	}
    }// while(true) ends here


vector<DAGNode*> LME_Vector;

synthesizeLMEsFromLMIs(SimplifiableLMIPairs, LME_Vector, WidthTable);

for(int i=0; i<BVULE_Vector.size(); i++)
    {
    DAGNode* Constraint = BVULE_Vector[i];
     list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *Constraint_LHS = *iout;
    iout++;
    DAGNode *Constraint_RHS = *iout;	
    
    string RHSName = Constraint_RHS->Get_node_name();
    
    if(RHSName == "0")
        {
        //cout<<"\n Trivial LMI: "<<getDAGStr(Constraint)<<endl;
        BVULE_Vector.erase(BVULE_Vector.begin()+i);
        DAGNode* LMEObtained = createDAG("=", Constraint_LHS, Constraint_RHS, LOGICAL_SYMBOL, 1);
        LME_Vector.push_back(LMEObtained);
        }
    }//for


for(int i=0; i<BVUGE_Vector.size(); i++)
    {
    DAGNode* Constraint = BVUGE_Vector[i];
     list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *Constraint_LHS = *iout;
    iout++;
    DAGNode *Constraint_RHS = *iout;	
    
    if(isConstant(Constraint_RHS))
        {    
        string RHSName = Constraint_RHS->Get_node_name();
        int WidthOfRHS = getWidthOfNode(Constraint_LHS, WidthTable);
        
        unsigned long long int RHSConstant =  strtoull(RHSName.c_str(), NULL, 0);
        unsigned long long int MaxPossibleRHS = findPower(WidthOfRHS)-1;
    
        if(RHSConstant == MaxPossibleRHS)
            {
            //cout<<"\n Trivial LMI: "<<getDAGStr(Constraint)<<endl;
            BVUGE_Vector.erase(BVUGE_Vector.begin()+i);
            DAGNode* LMEObtained = createDAG("=", Constraint_LHS, Constraint_RHS, LOGICAL_SYMBOL, 1);
            LME_Vector.push_back(LMEObtained);
            }//if(RHSConstant == MaxPossibleRHS)
        }//if(isConstant(Constraint_RHS))
    }//for

//cout<<"\nThe problem we need to solve is\n";

//showSet(VarsToElim, "VarsToElim");
//showAPVector("BVULE_Vector", BVULE_Vector);
//showAPVector("BVUGE_Vector", BVUGE_Vector);
//showAPVector("LME_Vector", LME_Vector);
//showAPVector("Remaining_Vector", Remaining_Vector);

Constraints.clear();

for(int i=0; i<BVULE_Vector.size(); i++)
    {
    Constraints.push_back(BVULE_Vector[i]);    
    }
for(int i=0; i<BVUGE_Vector.size(); i++)
    {
    Constraints.push_back(BVUGE_Vector[i]);    
    }
for(int i=0; i<LME_Vector.size(); i++)
    {
    Constraints.push_back(LME_Vector[i]);    
    }
for(int i=0; i<Remaining_Vector.size(); i++)
    {
    Constraints.push_back(Remaining_Vector[i]);    
    }
}


// synthesize LMEs from LMIs 
void synthesizeLMEsFromLMIs(vector< pair<DAGNode*, DAGNode*> > &SimplifiableLMIPairs, vector<DAGNode*> &LMEs, map<string, int> &WidthTable)
{
    
    for(vector< pair<DAGNode*, DAGNode*> >::iterator it = SimplifiableLMIPairs.begin(); it != SimplifiableLMIPairs.end(); it++)
    {
     DAGNode* ule = it->first;
     DAGNode* ugt = it->second;
     
     //cout<<"\nule = "<<getDAGStr(ule)<<"\tugt = "<<getDAGStr(ugt)<<endl;
     
     list<DAGNode*>::iterator iout = ule->Get_outlist_start();
     DAGNode *ule_LHS = *iout;
     iout++;
     DAGNode *ule_RHS = *iout;	
     
     DAGNode* InitialLME = createDAG("=", ule_LHS, ule_RHS, LOGICAL_SYMBOL, 1);
     
     //cout<<"\nInitialLME = "<<getDAGStr(InitialLME)<<endl;
     
     // Let us try to convert this to the form var1 = var2+constant (Let us do this later)
     
    //set<string> names;
    //getDAGNames(InitialLME, names);
    
    //set<string>::iterator names_it = names.begin();
    //string VarToElim = *names_it;
    
    //DAGNode* TempDAG = simplifyLMEToProperForm(InitialLME, WidthTable, VarToElim);
     
     LMEs.push_back(InitialLME);
    }// for ends hre  
    
}




// Returns the conjunction of the constraints in "Constraints"
DAGNode* getConjunctionFromSet(set<DAGNode*> &Constraints)
{
  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
	  
  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }
  return resultDAG;
}




// Version of getRelevantLMIDetails s.t. there's no printing and 
// change is made in the variables which are passed
void getRelevantLMIDetails_With_Recursive_Splitting(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable, int &LMINoVarsToElimLocal, int &LMINoLMIsLocal, int &LMINoLMEsLocal, int &LMINoLMDsLocal, int &LMINoVarsLocal, unsigned long long int &LMITotalBitsLocal, unsigned long long int &LMIBitsToElimLocal, int &PresentConjunctionNumberLocal)
{
    LMIMonniauxCounter++;
    PresentConjunctionNumberLocal = LMIMonniauxCounter;
    
    set<string> Support;
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode *Constraint = *it;
        
        getDAGNames(Constraint, Support);
        
        string ConstraintType = Constraint->Get_node_name();
        
        if(ConstraintType == "=" )
            LMINoLMEsLocal++;
        else if(ConstraintType == "is_not_equal" )
            LMINoLMDsLocal++;
        else if(ConstraintType == "bvule" ||  ConstraintType == "bvugt" || ConstraintType == "bvult" || ConstraintType == "bvuge")
            LMINoLMIsLocal++;
        else
        {
            cout<<"\nUnknown constraint type "<<ConstraintType<<" in function getRelevantLMIDetails_With_Recursive_Splitting in Eliminator.cpp\n";
            exit(1);
        }
    }// for
    
    set<string> CommonVars;
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    LMINoVarsToElimLocal = CommonVars.size();
    LMINoVarsLocal = Support.size();
    
    LMITotalBitsLocal = 0;
    
    for(set<string>::iterator it=Support.begin(); it != Support.end(); it++)
        {
        string signal = *it;
        map<string, int>::iterator wit =WidthTable.find(signal);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for "<<signal<<" in the WidthTable in function getRelevantLMIDetails_With_Recursive_Splitting in Eliminator.cpp\n";
            exit(1);
            }
        LMITotalBitsLocal += wit->second;        
        }

    LMIBitsToElimLocal = 0;
    
    for(set<string>::iterator it=CommonVars.begin(); it != CommonVars.end(); it++)
        {
        string signal = *it;
        map<string, int>::iterator wit =WidthTable.find(signal);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for "<<signal<<" in the WidthTable in function getRelevantLMIDetails_With_Recursive_Splitting in Eliminator.cpp\n";
            exit(1);
            }
        LMIBitsToElimLocal += wit->second;        
        }
    
    //cout<<"\nLMINoLMEsLocal = "<<LMINoLMEsLocal<<endl;
    //cout<<"\nLMINoLMDsLocal = "<<LMINoLMDsLocal<<endl;
    //cout<<"\nLMINoLMIsLocal = "<<LMINoLMIsLocal<<endl;
    //cout<<"\nLMINoVarsToElimLocal = "<<LMINoVarsToElimLocal<<endl;
    //cout<<"\nLMIBitsToElimLocal = "<<LMIBitsToElimLocal<<endl;
}// function ends here
  




// Given a set of LMEs, LMDs, and LMIs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode* using Layer1
// Layer1: CAV'11 work / LME based simplification
DAGNode* applyLMILayer1(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{ 
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  DAGNode* QElim_Bound_Constraint;

  //showSet(VarsToElim,"VarsToElim");
  //showAPSet("Constraints", Constraints);

  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;
      //cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints);

      QElim_Bound_Constraints.clear();
      QElim_Bound_Constraint = NULL; // stands for true

      if(Bound_Constraints.size()>0)
	{
          set<DAGNode*> LMESet;
          set<DAGNode*> LMDSet;
          set<DAGNode*> LMISet;
          
          splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
          
          if(LMISet.empty()) // no LMIs. Use CAV'11 work to eliminate this variable
              // Elimination guaranteed
            {
              //cout<<"\nLMIs do not appear in the conjunction\n";
              // CAV'11 work
              QElim_Bound_Constraint = callMatrixQEliminator(Bound_Constraints, VariableToElim, WidthTable);
            }
          else
            {
              //cout<<"\nLMIs appear in the conjunction\n";
              //showAPSet("LMESet", LMESet);
              //showAPSet("LMDSet", LMDSet);
              //showAPSet("LMISet", LMISet);
              
              if(LMESet.empty()) //Layer1 cannot simplify
                { 
		  //cout << "\nLayer1 cannot simplify\n";                 
                  QElim_Bound_Constraint = getConjunctionFromSet(Bound_Constraints);
                }//if(LMESet.empty())
              else// LMEBased simplification for LMDs and LMIs
                {                 
                  // Note that this is only LME based simplification
		  //cout << "\nApply LME based simplification\n";
                  QElim_Bound_Constraint = callMatrixQEliminatorForLMCsAndLMIs(LMESet, LMDSet, LMISet, VariableToElim, WidthTable);
                 }// LMEBased simplification for LMDs and LMIs
            }//else of if(LMISet.empty())	  
	}// if(Bound_Constraints.size()>0)

      if(QElim_Bound_Constraint != NULL) // stands for true
	{         
          if(!isTrue(QElim_Bound_Constraint))
                {
                //cout<<"\nQElim_Bound_Constraint = "<<getDAGStr(QElim_Bound_Constraint)<<"\n";
                splitConjunctionIntoSetGeneral(QElim_Bound_Constraint, QElim_Bound_Constraints);
                }
	}

      //showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      //showAPSet("Constraints", Constraints);
    }

  DAGNode *resultDAG = getConjunctionFromSet(Constraints);
    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

   resultDAG = quickSimplify_WithLMISupport(resultDAG);
    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    return resultDAG;
}


// Given a dag Phi and a set of variables to  be eliminated, find the # of variables 
// to be eliminated still remaining
int findNumberOfVarsToElimRemaining(DAGNode* Phi, set<string> &VarsToElim)
{
    set<string> Support;
    getDAGNames(Phi, Support);
    
    set<string> CommonVars;
    set_intersection(VarsToElim.begin(), VarsToElim.end(), Support.begin(), Support.end(),inserter(CommonVars, CommonVars.begin()));
    
    return CommonVars.size();
}



// Given a set of LMIs and a set of variables to be eliminated, this function calls
// applyLMILayer2 repeatedly as long as it drops variables
DAGNode* applyLMILayer2Recursive(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
    set<string> VarsToElimInLoop;
    set<string> NewVarsToElim;
    set<DAGNode*> ConstraintsInLoop;
    DAGNode* ResultDAG;    
    
    VarsToElimInLoop = VarsToElim;
    ConstraintsInLoop = Constraints;  
    
    int iteration_number = 1;
    
    while(true)
    {
        //cout<<"\nCalling Layer2 for the "<<iteration_number<<" time\n";
        ResultDAG = applyLMILayer2(ConstraintsInLoop, VarsToElimInLoop, WidthTable);
        if(isTrue(ResultDAG))
            {
            break;
            }
        set<string> Support;
        getDAGNames(ResultDAG, Support);
        NewVarsToElim.clear();
        set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),inserter(NewVarsToElim, NewVarsToElim.begin()));
        
        //showSet(VarsToElimInLoop, "VarsToElimInLoop");
        //showSet(NewVarsToElim, "NewVarsToElim");
        
        if(setsEqual(NewVarsToElim, VarsToElimInLoop))
        {
            //cout<<"\nOriginalVarsToElim ==  NewVarsToElim\n";
            break;
        }
        
        //cout<<"\nOriginalVarsToElim !=  NewVarsToElim\n";
        VarsToElimInLoop = NewVarsToElim;
        ConstraintsInLoop.clear();
	splitConjunctionIntoSetGeneral(ResultDAG, ConstraintsInLoop);     
        
        iteration_number++;
    }// while(true)
    
    return ResultDAG;
    
}// end of function




// Given a set of LMCs and a set of variables to be eliminated, this function eliminates the 
// variables one by one, and returns the result as a DAGNode* using Layer2
DAGNode* applyLMILayer2(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable)
{
 
  set<DAGNode*> Free_Constraints;
  set<DAGNode*> Bound_Constraints;
  set<DAGNode*> QElim_Bound_Constraints;
  DAGNode* QElim_Bound_Constraint;

  //showSet(VarsToElim,"VarsToElim");
  //showAPSet("Constraints", Constraints);


  for(set<string>::iterator it=VarsToElim.begin(); it!=VarsToElim.end(); it++)
    {
      string VariableToElim = *it;

      //cout<<"\nVariableToElim = "<<VariableToElim<<"\n";
      
      Free_Constraints.clear();
      Bound_Constraints.clear();
      getFreeAndBoundConstraints(Constraints, VariableToElim, Free_Constraints, Bound_Constraints, WidthTable);

      //showAPSet("Free_Constraints", Free_Constraints);
      //showAPSet("Bound_Constraints", Bound_Constraints);

      QElim_Bound_Constraints.clear();
      QElim_Bound_Constraint = NULL; // stands for true

      if(Bound_Constraints.size()>0)
	{
          set<DAGNode*> LMESet;
          set<DAGNode*> LMDSet;
          set<DAGNode*> LMISet;
          
          splitConstraintsIntoLMELMDLMISets(Bound_Constraints, LMESet,  LMDSet,  LMISet);
          
         //showAPSet("LMESet", LMESet);
         //showAPSet("LMDSet", LMDSet);
         //showAPSet("LMISet", LMISet);
         
         int WidthOfVarToElim = getWidthOfVariable(WidthTable, VariableToElim);         
         //cout<<"\nWidthOfVarToElim = "<<WidthOfVarToElim<<endl;
         
         ULINT ModOfVarToElim = findPower(WidthOfVarToElim);         
         //cout<<"\nModOfVarToElim = "<<ModOfVarToElim<<endl;
         
         QElim_Bound_Constraint = dropUnconstrainingDisequationsAndInequationsCorrected(LMESet, LMDSet, LMISet, VariableToElim, WidthOfVarToElim, ModOfVarToElim, WidthTable);
         //cout<<"\nModOfVarToElim = "<<ModOfVarToElim<<endl;
        }//if(Bound_Constraints.size()>0)

      if(QElim_Bound_Constraint != NULL) // stands for true
	{
          
          if(!isTrue(QElim_Bound_Constraint))
                {
                //cout<<"\nQElim_Bound_Constraint = "<<getDAGStr(QElim_Bound_Constraint)<<"\n";
                splitConjunctionIntoSetGeneral(QElim_Bound_Constraint, QElim_Bound_Constraints);
                }
	}

      //showAPSet("QElim_Bound_Constraints", QElim_Bound_Constraints);
      
      Constraints.clear();
      set_union(Free_Constraints.begin(), Free_Constraints.end(), QElim_Bound_Constraints.begin(), QElim_Bound_Constraints.end(),inserter(Constraints, Constraints.begin()));   

      //showAPSet("Constraints", Constraints);
    }//for

  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(set<DAGNode*>::iterator it=Constraints.begin(); it!=Constraints.end(); it++)
    {
      if(resultDAG==NULL)
	{
	  resultDAG=*it;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, *it, LOGICAL_SYMBOL, 1);
	}
    }
  
    if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    resultDAG = quickSimplify_WithLMISupport(resultDAG);

    //cout<<"\nresultDAG = "<<getDAGStr(resultDAG)<<endl;

    return resultDAG;
}




// Version of dropUnconstrainingDisequationsAndInequations,
// which is corrected to handle any conjunction of LMEs, LMDs, and LMIs
DAGNode* dropUnconstrainingDisequationsAndInequationsCorrected(set<DAGNode*> &LMESet, set<DAGNode*> &LMDSet, set<DAGNode*> &LMISet, string VarToElim,  int WidthOfVarToElim, ULINT ModOfVarToElim, map<string, int> &WidthTable)
{
set<DAGNode*> Constraints;
set<DAGNode*> Phi;
set<DAGNode*> Xi;

// Constraints <--- E \union D \union I
set_union(LMESet.begin(), LMESet.end(), LMDSet.begin(), LMDSet.end(), inserter(Constraints, Constraints.begin()));   
set_union(Constraints.begin(), Constraints.end(), LMISet.begin(), LMISet.end(), inserter(Constraints, Constraints.begin()));  
//showAPSet("Constraints", Constraints);

// \phi <---- E
set_union(Phi.begin(), Phi.end(), LMESet.begin(), LMESet.end(), inserter(Phi, Phi.begin()));   
//showAPSet("Phi", Phi);

// \xi <--- Costraints \ \phi
set_difference(Constraints.begin(), Constraints.end(), Phi.begin(), Phi.end(), inserter(Xi, Xi.begin()));   
//showAPSet("Xi", Xi);

while(!setsEqual(Phi, Constraints))
    {
    //cout<<"\nPhi neq Constraints\n";
    //showAPSet("Phi", Phi);
    //showAPSet("Xi", Xi);
    
    // Let us call the generic function which does the layer2 check either using BDDs or using our counting argument
    if(isExtForLMDsAndLMIsGeneral(Phi, Xi, VarToElim, WidthOfVarToElim, ModOfVarToElim, WidthTable)) // \exists x. (Xi /\ Phi) \equiv \exists x. (Phi)   
        {
          //cout<<"\nisExt returns true\n";
          if(Phi.empty())
                    return createLogicalConstant("true");
          else
                    return getConjunctionFromSet(Phi);
        }//if  \exists x. (Xi /\ Phi) \equiv \exists x. (Phi)
    else
        {
        //cout<<"\nisExt returns false\n";
        set<DAGNode*> Pi;
        findConstraintsWithMaximumKValueForLMDsAndLMIsCorrected(Xi, VarToElim, Pi, ModOfVarToElim);
        set_union(Phi.begin(), Phi.end(), Pi.begin(), Pi.end(), inserter(Phi, Phi.begin()));  // \phi <---- \phi \union \pi 
        Xi.clear();
        set_difference(Constraints.begin(), Constraints.end(), Phi.begin(), Phi.end(), inserter(Xi, Xi.begin()));  // \xi <--- Costraints \ \phi
        }//else
    
    }//while

//cout<<"\nPhi = Constraints\n";
return getConjunctionFromSet(Phi);
}//function


// Corrected version of findConstraintsWithMaximumKValueForLMDsAndLMIs
void findConstraintsWithMaximumKValueForLMDsAndLMIsCorrected(set<DAGNode*> &Xi, string VarToElim, set<DAGNode*> &Pi, ULINT ModOfVarToElim)
{
    map<DAGNode*, ULINT> KValueMap;
    
    ULINT MaxKValue = 0;
    
    for(set<DAGNode*>::iterator it=Xi.begin(); it != Xi.end(); it++)
    {
        DAGNode* Constraint = *it;           
        ULINT MyKValue = findKValue(Constraint, VarToElim,ModOfVarToElim);
        //cout<<"\nMyKValue = "<<MyKValue<<endl;
        
        if(MyKValue > MaxKValue)
            {
            MaxKValue = MyKValue;
            }
        
        KValueMap.insert(make_pair(Constraint, MyKValue));
    }//for
    
    //cout<<"\nMaxKValue = "<<MaxKValue<<endl;
    
    for(map<DAGNode*, ULINT>::iterator it=KValueMap.begin(); it != KValueMap.end(); it++)
    {
        if(it->second == MaxKValue)
        {
           Pi.insert(it->first); 
        }//if
    }//for
    
    //showAPSet("Pi", Pi);
}


// Find k-value of the constraint
ULINT findKValue(DAGNode* Constraint, string VarToElim, ULINT ModOfVarToElim)
{
    string ConstraintType = Constraint->Get_node_name();
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *Constraint_LHS = *iout;
    iout++;
    DAGNode *Constraint_RHS = *iout;	
    ULINT MyKValue;
    
    if(ConstraintType == "=" || ConstraintType == "is_not_equal")
    {
        ULINT Coefficient_LHS;
        getCoefficientCorrected(Constraint_LHS, VarToElim, Coefficient_LHS);
        ULINT Coefficient_RHS;
        getCoefficientCorrected(Constraint_RHS, VarToElim, Coefficient_RHS);
        ULINT Coefficient;
        Coefficient = sub(Coefficient_LHS, Coefficient_RHS, ModOfVarToElim);
        MyKValue = powerOfTwo(Coefficient);
    }//if
    else if(ConstraintType == "bvule" || ConstraintType == "bvuge" || ConstraintType == "bvugt" || ConstraintType == "bvult")
    {
        ULINT Coefficient_LHS;
        getCoefficientCorrected(Constraint_LHS, VarToElim, Coefficient_LHS);
        ULINT Coefficient_RHS;
        getCoefficientCorrected(Constraint_RHS, VarToElim, Coefficient_RHS);
        
        ULINT KValue_LHS, KValue_RHS;
        
        if(Coefficient_LHS == 0)
        {
            if(Coefficient_RHS == 0)
            {
                cout<<"\nCoefficient of "<<VarToElim<<" is zero in both LHS and RHS of "<<getDAGStr(Constraint)<<" in function findKValue in Eliminator.cpp "<<endl;
                exit(1);
            }
            else
            {
                MyKValue = powerOfTwo(Coefficient_RHS);
            }
        }
        else if(Coefficient_RHS == 0)
        {
            if(Coefficient_LHS == 0)
            {
               cout<<"\nCoefficient of "<<VarToElim<<" is zero in both LHS and RHS of "<<getDAGStr(Constraint)<<" in function findKValue in Eliminator.cpp "<<endl;
               exit(1);  
            }
            else
            {
                MyKValue = powerOfTwo(Coefficient_LHS);
            }
        }
        else
        {

            KValue_LHS = powerOfTwo(Coefficient_LHS);
            KValue_RHS = powerOfTwo(Coefficient_RHS);

            if(KValue_LHS < KValue_RHS)
            {
                MyKValue = KValue_LHS;
            }
            else
            {
                MyKValue = KValue_RHS;
            }
        }
    }//else if
    return MyKValue;
}


// Find minimum k-value from a set of constraints
ULINT findMinimumKValue(set<DAGNode*> &Phi, string VarToElim, ULINT WidthOfVarToElim, ULINT ModOfVarToElim)
{
        ULINT MinKValue = WidthOfVarToElim;   
        
        for(set<DAGNode*>::iterator it=Phi.begin(); it != Phi.end(); it++)
        {
        DAGNode* Constraint = *it;           
        ULINT MyKValue = findKValue(Constraint, VarToElim, ModOfVarToElim);
        
        if(MyKValue < MinKValue)
            {
            MinKValue = MyKValue;
            }
        }//for
        
        return MinKValue;
}

// Corrected version of isExtLMDsAndLMIs
// Checks if \exists VarToElim. (Phi /\ Xi) \equiv \exists VarToElim. (Phi) using Counting Lemma
bool isExtForLMDsAndLMIsCorrected(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, ULINT ModOfVarToElim)
{
    
    struct timeval check_start_ms, check_finish_ms;  
    struct timeval step_start_ms, step_finish_ms;  
    unsigned long long int step_time;
    
    if(PrintLayer2Times)
        {
        gettimeofday (&check_start_ms, NULL);                           
        gettimeofday (&step_start_ms, NULL);    
        }
        
    ULINT k1 = findMinimumKValue(Phi, VarToElim, WidthOfVarToElim, ModOfVarToElim);    
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
        }
    
     if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "\nMin K Value finding time = %llu\t", step_time);
        }
    
    //cout<<"\nk1 = "<<k1<<endl;
    //cout<<"\nWidthOfVarToElim = "<<WidthOfVarToElim<<endl;
    //cout<<"\nModWidthOfVarToElim = "<<ModOfVarToElim<<endl;
    if(PrintLayer2Times)
        {
        gettimeofday (&step_start_ms, NULL);   
        }
    
    set<DAGNode*> LMESet;
    set<DAGNode*> LMDSet;
    set<DAGNode*> LMISet;
    
    splitConstraintsIntoLMELMDLMISets(Xi, LMESet, LMDSet, LMISet);
    // Note that LMESet here is empty always
    
   if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
        }
    
     if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "Splitting time = %llu\t", step_time);
        }
   
    if(PrintLayer2Times)
        {
        gettimeofday (&step_start_ms, NULL);   
        }
    
    // 1) Ensure that VarToElim does not appear on both sides. If yes, return false. cannot drop
    // 2) In LMIs of the form LHS op RHS, where op \in \{ <=, >= \}, only LHS/RHS has the variable to elim, 
    // and both LHS and RHS are not constants, replace by tighter LMIs where RHS is constant
    // 3) In other LMIs, do not do anything    
    set<DAGNode*> TighterLMIs;
    
    //showAPSet("LMISet", LMISet);
    
    for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* Constraint = *it;
        string LMIType = Constraint->Get_node_name();
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *Constraint_LHS = *iout;
        iout++;
        DAGNode *Constraint_RHS = *iout;
        
        bool lhs_bound = isBound(Constraint_LHS, VarToElim);
        bool rhs_bound = isBound(Constraint_RHS, VarToElim);
        
        
        if(isConstant(Constraint_LHS) || isConstant(Constraint_RHS)) // One side is already a constant
            {
            TighterLMIs.insert(Constraint);
            }
        else if(lhs_bound && rhs_bound)
            {
            // Both sides of the LMI contains the variable to be eliminated
            //cout<<"\nBoth sides of "<<getDAGStr(Constraint)<<" contains the variable to be eliminated\n";
            return false;
            }
        else if(LMIType == "bvule" && lhs_bound)
             {
             DAGNode* TighterConstraint;
             TighterConstraint = findTighterSimplerConstraint("bvule", Constraint_LHS, Constraint_RHS, ModOfVarToElim, WidthOfVarToElim);
             TighterLMIs.insert(TighterConstraint);
            }
         else if(LMIType == "bvuge" && lhs_bound)
             {
             DAGNode* TighterConstraint;
             TighterConstraint = findTighterSimplerConstraint("bvuge", Constraint_LHS, Constraint_RHS, ModOfVarToElim, WidthOfVarToElim);
             TighterLMIs.insert(TighterConstraint);
            }
         else if(LMIType == "bvule" && rhs_bound)
             {
             DAGNode* TighterConstraint;
             TighterConstraint = findTighterSimplerConstraint("bvuge", Constraint_RHS, Constraint_LHS, ModOfVarToElim, WidthOfVarToElim);
             TighterLMIs.insert(TighterConstraint);
            }
        else if(LMIType == "bvuge" && rhs_bound)
             {
             DAGNode* TighterConstraint;
             TighterConstraint = findTighterSimplerConstraint("bvule", Constraint_RHS, Constraint_LHS, ModOfVarToElim, WidthOfVarToElim);
             TighterLMIs.insert(TighterConstraint);
            }
        else
            {
            cout<<"\nSuspicious LMI "<<getDAGStr(Constraint)<<" encountered in function isExtForLMDsAndLMIsCorrected in Eliminator.cpp\n";
            exit(1);
            }
       
    }//for  
    
    LMISet = TighterLMIs; 
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
        }
    
     if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "Tightening time = %llu\t", step_time);
        }    
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_start_ms, NULL);   
        }
    
    
    //showAPSet("LMISet after tightening", LMISet);    
    set<DAGNode*> OneSidedLMIs;
    
    //Ensure that in LMIs where one side is constant, the constant appears on the right side
    for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* Constraint = *it;
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *Constraint_LHS = *iout;
        iout++;
        DAGNode *Constraint_RHS = *iout;	
        string ConstraintType = Constraint->Get_node_name();
        
        DAGNode* OneSidedLMI;
       
       if(ConstraintType == "bvule" && isConstant(Constraint_LHS))
       {
           OneSidedLMI = createDAG("bvuge", Constraint_RHS, Constraint_LHS, LOGICAL_SYMBOL, 1);
       }        
        else if(ConstraintType == "bvuge" && isConstant(Constraint_LHS))
       {
           OneSidedLMI = createDAG("bvule", Constraint_RHS, Constraint_LHS, LOGICAL_SYMBOL, 1);
       }
        else
        {
            OneSidedLMI = Constraint;
        }
      OneSidedLMIs.insert(OneSidedLMI); 
    }//for
    
    LMISet = OneSidedLMIs;  
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
        }
    
     if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "Time to make them one-sided = %llu\t", step_time);
        }
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_start_ms, NULL);   
        }
    
    
    bool DropSimpleImpliedLMIs = true;
    
    if(DropSimpleImpliedLMIs)
    {
        //showAPSet("LMISet before dropping simple implied LMIs", LMISet);    
        
        set<DAGNode*> LMISetAfterDropping;
                
        dropSimpleImpliedLMIs(LMISet, LMISetAfterDropping, WidthOfVarToElim);
        
        LMISet = LMISetAfterDropping;
        
        //showAPSet("LMISet after dropping simple implied LMIs", LMISet);    
    }
    else
    {
      //do nothing  
    }   
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
        }
    
     if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "Dropping time = %llu\t", step_time);
        }  
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_start_ms, NULL);   
        }
    
    
   // Let us form LMI Pairs 
   vector<pair<DAGNode*, DAGNode*> > LMIPairs; 
   identifyLMIPairs_ForWeakLMIs(LMISet, LMIPairs);
   //showLMIPairs(LMIPairs); 
   
   if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;    
        }
    
     if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "Pairing time = %llu\t", step_time);
        }  
   
   if(PrintLayer2Times)
        {
        gettimeofday (&check_finish_ms, NULL);
        }
   
     unsigned long long int PreparationTime;
     unsigned long long int HeuristicTime;
     
      if(PrintLayer2Times)
        {
        PreparationTime = check_finish_ms.tv_sec * 1000 + check_finish_ms.tv_usec / 1000;
        PreparationTime -= check_start_ms.tv_sec * 1000 + check_start_ms.tv_usec / 1000;    
        }
     
      if(PrintLayer2Times)
        {
        gettimeofday (&check_start_ms, NULL);     
        }
   
   
   long long int CombinationsPermittedByLMIs;
   
   if(LMISet.empty())
    {
       CombinationsPermittedByLMIs = findPower(k1);
    }
   else
   {
       CombinationsPermittedByLMIs = findCombinationsPermittedByLMIs_OnWeakLMIs(LMIPairs,  VarToElim, k1, WidthOfVarToElim, ModOfVarToElim);
   }
   
   long long int CombinationsDisallowedByLMDs =  findCombinationsDisallowedByLMDsCorrected(LMDSet,  VarToElim, ModOfVarToElim);
   
   //cout<<"\nCombinationsPermittedByLMIs = "<<CombinationsPermittedByLMIs<<endl;
   //cout<<"\nCombinationsDisallowedByLMDs = "<<CombinationsDisallowedByLMDs<<endl;
   
    if(PrintLayer2Times)
        {
        gettimeofday (&check_finish_ms, NULL);
        HeuristicTime = check_finish_ms.tv_sec * 1000 + check_finish_ms.tv_usec / 1000;
        HeuristicTime -= check_start_ms.tv_sec * 1000 + check_start_ms.tv_usec / 1000; 
        }
   
   if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "\nPreparation time = %llu\tHeuristic time = %llu\n", PreparationTime, HeuristicTime);
        }
      
   
   if(CombinationsPermittedByLMIs - CombinationsDisallowedByLMDs >= 1)
   {
       return true;
   }

   return false;
}


// Corrected version of findCombinationsDisallowedByLMDs
 long long int findCombinationsDisallowedByLMDsCorrected(set<DAGNode*> &LMDSet,  string VarToElim, ULINT ModOfVarToElim)
 {
     long long int CombinationsDisallowed=0;
     
     for(set<DAGNode*>::iterator it=LMDSet.begin(); it != LMDSet.end(); it++)
        {
         DAGNode* LMD = *it;
         ULINT MyKValue = findKValue(LMD, VarToElim, ModOfVarToElim);
         ULINT MyContribution = findPower(MyKValue);
         CombinationsDisallowed += MyContribution;         
         }
      return CombinationsDisallowed;
 }
 
 
 
 // Check if InputDag is a conjunction of LMEs
 // If yes, return true node
 // Else return the original node
 DAGNode* singleLMEElimination(DAGNode* InputDag)
 {
    set<DAGNode*> Constraints;
    splitConjunctionIntoSetGeneral(InputDag, Constraints);
    
    set<DAGNode*> LMESet;
    set<DAGNode*> LMDSet;
    set<DAGNode*> LMISet;
    
    splitConstraintsIntoLMELMDLMISets(Constraints, LMESet, LMDSet, LMISet);
    
    if(LMDSet.empty() && LMISet.empty())
    {
        return createLogicalConstant("true");
    }
    return InputDag;
 }
 
 
 
 
 // Recreate LMIs with terms on both sides as tighter constraints with constants on RHS
DAGNode* findTighterSimplerConstraint(string LMIType, DAGNode* LHS, DAGNode* RHS,  ULINT ModOfVarToElim, int WidthOfVarToElim)
{
    // Obtain the coefficients of the variables and the constant of the RHS part
    // i.e. given c1.x1+...+cn.xn+d, obtain {x1-->c1,...,xn--->cn} and d
    
    struct timeval step_start_ms, step_finish_ms;    
    unsigned long long int step_time;
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_start_ms, NULL);        
        }
    
    map<string, ULINT> VariableCoefficientMap;
    ULINT ConstantPart;
    obtainVariableCoefficientMap(RHS, VariableCoefficientMap, ConstantPart);
    
    if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;  
        }

    if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "obtainVariableCoefficientMap time = %llu\t", step_time);
        }
    
     if(PrintLayer2Times)
        {
        gettimeofday (&step_start_ms, NULL);        
        }
    
    // Find minimum k-value of the coefficient part
    ULINT MinimumCoefK = WidthOfVarToElim;
    
    for(map<string, ULINT>::iterator it=VariableCoefficientMap.begin();  it!=VariableCoefficientMap.end(); it++)
        {
        ULINT Coefficient = it->second;
        ULINT MyKValue = powerOfTwo(Coefficient); 
        if(MyKValue < MinimumCoefK)
            {
            MinimumCoefK = MyKValue;
            }
        }//for
    
    ULINT ConstantK;
    ULINT MinimumK;//MinimumK = minimum(MinimumCoefK, ConstantK)
    
    if(ConstantPart==0)//ConstantK is undefined.
        {
        MinimumK = MinimumCoefK;
        }
    else
        {
        ConstantK = powerOfTwo(ConstantPart);
        if(ConstantK <= MinimumCoefK)
            MinimumK = ConstantK;
        else
            MinimumK = MinimumCoefK;
        }
    
    if(LMIType == "bvuge")
        {
        ConstantPart = ModOfVarToElim - findPower(MinimumK);        
        }
    else if(LMIType == "bvule")
        {
        ConstantPart = ConstantPart % (findPower(MinimumCoefK));
        }   
    else
        {
        cout<<"\nUnsupported LMIType "<<LMIType<<" in function findTighterSimplerConstraint in Eliminator.cpp\n";
        exit(1);
        }
    
    char ConstantPart_char[10];
    sprintf(ConstantPart_char, "%llu", ConstantPart);
    string ConstantPart_str(ConstantPart_char);
    // LHS \bowtie ConstantPart is a tighter LMI
    DAGNode *NewRHS = createBVConstant(ConstantPart_str, WidthOfVarToElim); 
    DAGNode *NewLMI = createDAG(LMIType, LHS, NewRHS, LOGICAL_SYMBOL, 1);
        
    if(PrintLayer2Times)
        {
        gettimeofday (&step_finish_ms, NULL);
        step_time = step_finish_ms.tv_sec * 1000 + step_finish_ms.tv_usec / 1000;
        step_time -= step_start_ms.tv_sec * 1000 + step_start_ms.tv_usec / 1000;  
        }

    if(PrintLayer2Times)
        {
        fprintf(LMIQEOutputDataFp, "Time to create NewLMI = %llu\t", step_time);
        }   
    
    return NewLMI;
}



// Version of findCombinationsPermittedByLMIs which works on weak LMIs
long long int findCombinationsPermittedByLMIs_OnWeakLMIs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs,  string VariableToElim, int k1, ULINT WidthOfVarToElim, ULINT ModWidthOfVarToElim)
{
   // Find the delta, k of each LMI pair
    vector<long long int> Deltas;
    vector<ULINT> KValues;
        
    for(int i=0; i<LMIPairs.size(); i++)
    {
        //cout<<"\nReached here at beginning of loop\n";
        DAGNode *ule = LMIPairs[i].first;
        DAGNode *ugt = LMIPairs[i].second;
        
        DAGNode *High=NULL;
        DAGNode *Low=NULL;
        DAGNode *Constraint_LHS=NULL;
        
        if(ule == NULL)
          {
            //cout<<"\nInside if(ule == NULL)\n";
            
            list<DAGNode*>::iterator iout;
            iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = NULL;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ugt; 
        }        
        else if(ugt==NULL)
            {
            list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
        
            High = RHS_ule;
            Low = NULL;
            Constraint_LHS = LHS_ule; 
            } 
        else if(ule != NULL && ugt != NULL)
        {
           list<DAGNode*>::iterator iout;
            iout = ule->Get_outlist_start();
            DAGNode *LHS_ule = *iout;
            iout++;
            DAGNode *RHS_ule = *iout;
             iout = ugt->Get_outlist_start();
            DAGNode *LHS_ugt = *iout;
            iout++;
            DAGNode *RHS_ugt = *iout;
        
            High = RHS_ule;
            Low = RHS_ugt;
            Constraint_LHS = LHS_ule;  
        }
        else
        {
            cout<<"\nule and ugt both NULL in function findCombinationsPermittedByLMIs_OnWeakLMIs in DAGEliminator.cpp\n";
            exit(1);
        }
        
        //cout<<"\noutside if-else structure\n";
        //cout<<"\nConstraint_LHS = "<<getDAGStr(Constraint_LHS)<<endl;        
        ULINT Coefficient;
        getCoefficientCorrected(Constraint_LHS, VariableToElim, Coefficient);        
        //cout<<"\nCoefficient = "<<Coefficient<<endl;        
        ULINT MyKValue = powerOfTwo(Coefficient);
        KValues.push_back(MyKValue);               
        //cout<<"\nMyKValue = "<<MyKValue<<endl;
       
       
        long long int l2, u2;
        long long int delta;
    
        if(Low == NULL)
        {
            l2 = 0;
        }
        else
        {
            string Low_str = Low->Get_node_name();
            l2 = strtoull(Low_str.c_str(), NULL, 0);
        }
    
        if(High == NULL)
        {
            u2 = ModWidthOfVarToElim - 1;
        }
        else
        {
            string High_str = High->Get_node_name();
            u2 = strtoull(High_str.c_str(), NULL, 0);
        }
    
        //cout<<"\nu2 = "<<u2<<"\tl2 = "<<l2<<endl;
        
        delta = u2-l2+1;
        
        //cout<<"\ndelta = "<<delta<<endl;
        
        if(delta < 0)
            {
            return 0; //This may happen due to tightening leading to the constraint to become false
            }
    
        Deltas.push_back(delta);       
    }// for
    
       
    list<ULINT> SortedKValues;
    vector<long long int> Ways;

    set<ULINT> KValuesSet;
    for(int i=0; i<KValues.size(); i++)
    {
       KValuesSet.insert(KValues[i]);
    }//for 


    SortedKValues.push_back(k1);
    for(set<ULINT>::iterator it=KValuesSet.begin(); it != KValuesSet.end(); it++)
    {
      SortedKValues.push_back(*it);           
    }//for

    SortedKValues.sort();
    SortedKValues.reverse();

    //cout<<"\nSortedKValues\n";
    //for(list<ULINT>::iterator it=SortedKValues.begin(); it != SortedKValues.end(); it++)
    //{
    //   cout<<*it<<endl;
    //}//for

    vector<ULINT> SortedKValuesVector;
    for(list<ULINT>::iterator it=SortedKValues.begin(); it != SortedKValues.end(); it++)
    {
       SortedKValuesVector.push_back(*it);
    }//fors       


    for(int k_index=0; k_index < SortedKValuesVector.size()-1 ; k_index++)
    {
      ULINT K_1 = SortedKValuesVector[k_index];
      ULINT K_2 = SortedKValuesVector[k_index+1];

      //cout<<"\nK_1 = "<<K_1<<endl;
      //cout<<"\nK_2 = "<<K_2<<endl;
      //cout<<"\nFinding the number of ways in which "<<K_1<<" to "<<K_2<<" part can be engineered\n";

      // Finding the number of ways in which K_1 to K_2 part can be engineered

      long long int W_i = 0;
      ULINT DeltaCount = 0;

      for(int i=0; i<KValues.size(); i++)
      {
        ULINT K_i = KValues[i];
        ULINT Delta_i = Deltas[i];

        //cout<<"\nK_i = "<<K_i<<"\tDelta_i = "<<Delta_i<<endl;

        if(K_1 > K_i)
        {
            DeltaCount++;

            ULINT Denominator = findPower(WidthOfVarToElim-K_1+K_i);
            ULINT Numerator = Delta_i;

            //cout<<"\nNumerator = "<<Numerator<<"\tDenominator = "<<Denominator<<endl;

            double quotient = floor(Numerator/Denominator);
            ULINT X_i = (ULINT)quotient;

            //cout<<"\nX_i = "<<X_i<<endl;
            //cout<<"\nfindPower(K_1-K_2) = "<<findPower(K_1-K_2)<<endl;
            //cout<<"\nk_index = "<<k_index<<endl;

            ULINT Y_i;
            
            if(k_index==0)
                {
                 Y_i = findMinimum(findPower(K_1-K_2), X_i);
                 //cout<<"\nY_i = "<<Y_i<<endl;
                }
            else
                {
                Y_i = findMinimum(findPower(K_1-K_2), findMaximum(X_i, (ULINT)1));
                }

            //cout<<"\nY_i = "<<Y_i<<endl;

            W_i += (long long int)Y_i;

           //cout<<"\nW_i = "<<W_i<<endl;
        }//if(K_1 > K_i)                
      }//for

      W_i = W_i - (DeltaCount-1)*findPower(K_1-K_2);

     //cout<<"\nW_i = "<<W_i<<endl;

      Ways.push_back(W_i);            
    }//for 

    //cout<<"\nFinding the number of ways in which unconstrained bits can be engineered\n";

    long long int W_n = findPower(SortedKValuesVector[SortedKValuesVector.size()-1]);
    
    //cout<<W_n<<endl;
    
    Ways.push_back(W_n);


    //cout<<"\nWays\n";
    //for(int i=0; i<Ways.size(); i++)
    //{
    // cout<<Ways[i]<<endl;
    // }//for

    long long int CombinationsPermitted=1;
    for(int i=0; i<Ways.size(); i++)
    {
       if(Ways[i] <= 0)
        {
        CombinationsPermitted = 0;
        break;
        }
       else
          CombinationsPermitted = CombinationsPermitted * Ways[i]; 
    }//for

  //cout<<"\nCombinationsPermitted = "<<CombinationsPermitted<<endl;
  //char c = getchar();
  return CombinationsPermitted;  
      
}// end of function



//Version of identifyLMIPairs for BVULE, BVUGE constraints
 void identifyLMIPairs_ForWeakLMIs(set<DAGNode*> &Constraints, vector<pair<DAGNode*, DAGNode*> > &LMIPairs)
{
vector<DAGNode*> BVULE_Vector;// set of constraints in Constraints of the form <=
vector<DAGNode*> BVUGT_Vector;// Set of constraints in Constraints of the form >=

for(set<DAGNode*>::iterator it = Constraints.begin(); it != Constraints.end(); it++)
    {
       DAGNode* MyConstraint = *it;
       if(MyConstraint->Get_node_name()=="bvule")
       {
           BVULE_Vector.push_back(MyConstraint);
       }
       else if(MyConstraint->Get_node_name()=="bvuge")
       {
           BVUGT_Vector.push_back(MyConstraint);
       }
       else
       {
         cout<<"\nUnknown predicate "<<MyConstraint->Get_node_name()<<" inside function identifyLMIPairs_ForWeakLMIs in Eliminator.cpp\n";
         exit(1);
       }             

    }//for ends here


//showAPVector("BVULE_Vector", BVULE_Vector);
//showAPVector("BVUGT_Vector", BVUGT_Vector);

while(BVUGT_Vector.size()>0)
{
      DAGNode* ugt = BVUGT_Vector[0];
      //cout<<"\nTaking LMI "<<getDAGStr(ugt)<<endl;
      
      bool match_found=false;
      
      for(int j=0; j<BVULE_Vector.size(); j++)
      {
          DAGNode* ule = BVULE_Vector[j];

          if(LMIPairFound(ugt, ule))
          {
              //cout<<"\nMatches with "<<getDAGStr(ule)<<endl;
              //cout<<"\nRemoving the pair from the vectors and putting them in another vector\n";
              LMIPairs.push_back(make_pair(ule, ugt));
              BVUGT_Vector.erase(BVUGT_Vector.begin());
              BVULE_Vector.erase(BVULE_Vector.begin()+j);
              match_found=true;
              break;
          }
      }

    if(!match_found)
        {
        //cout<<"\nNo matching ule constraint found. Removing the LMI\n";
        
        if(lmiRHSConstant(ugt))
            {
            DAGNode* ule = NULL;
            LMIPairs.push_back(make_pair(ule, ugt));
            }
        
        BVUGT_Vector.erase(BVUGT_Vector.begin());       
        }
    }// while ends here


for(int j=0; j<BVULE_Vector.size(); j++)
      {
       
            DAGNode* ule = BVULE_Vector[j];
            //cout<<"\nRemoving the LMI "<<getDAGStr(ule)<<"\n";
            if(lmiRHSConstant(ule))
                {
                DAGNode* ugt = NULL;
                LMIPairs.push_back(make_pair(ule, ugt));
                }//if(lmiRHSConstant(ule))
    }//for
 }// end of function
 
 
 
 // Show vector< pair<DAGNode*, DAGNode*> > LMIPairs
void showLMIPairs(vector< pair<DAGNode*, DAGNode*> > &LMIPairs)
{
    cout<<"\nLMIPairs\n**********\n";
    for(int i=0; i<LMIPairs.size(); i++)
    {
       if(LMIPairs[i].first==NULL) 
       {
          cout<<"NULL"<<"\t"<<getDAGStr(LMIPairs[i].second)<<endl; 
       }
       else if(LMIPairs[i].second==NULL)
       {
          cout<<getDAGStr(LMIPairs[i].first)<<"\tNULL"<<endl; 
       }
       else
       {
           cout<<getDAGStr(LMIPairs[i].first)<<"\t"<<getDAGStr(LMIPairs[i].second)<<endl;
       }     
        
    }//for
}// end of function


// Check if the given pair of LMIs can be put as a pair
bool LMIPairFound(DAGNode* ugt, DAGNode* ule)
{
    list<DAGNode*>::iterator iout;
    iout = ugt->Get_outlist_start();
    DAGNode *ugt_LHS = *iout;
    iout++;
    DAGNode *ugt_RHS = *iout;	
    
    iout = ule->Get_outlist_start();
    DAGNode *ule_LHS = *iout;
    iout++;
    DAGNode *ule_RHS = *iout;	
    
    if(ugt_LHS != ule_LHS)
    {
        return false;
    }
    
    if(!isConstant(ugt_RHS) || !isConstant(ule_RHS))
    {
        return false;
    }

    return true;
}

 // Check if the RHS of the given LMI is a constant
bool lmiRHSConstant(DAGNode* lmi)
{
    list<DAGNode*>::iterator iout;
    iout = lmi->Get_outlist_start();
    DAGNode *lmi_LHS = *iout;
    iout++;
    DAGNode *lmi_RHS = *iout;	
   
    if(isConstant(lmi_RHS))
    {
        return true;
    }

    return false;
}


// Given a set of constraints in "Constraints" and a set of variables in "Variables",
// get the set of constraints free of variables in "Variables" in "Free_Constraints", 
// and the remaining constraints in "Bound_Constraints"
void getFreeAndBoundConstraintsForMultipleVariables(set<DAGNode*> &Constraints, set<string> &Variables, set<DAGNode*> &Free_Constraints, set<DAGNode*> &Bound_Constraints, map<string, int> &WidthTable)
{
  for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
      set<string> Names;

      DAGNode* root = (*it);

      getDAGNames(root, Names);

      set<string> Common;

      set_intersection(Names.begin(), Names.end(), Variables.begin(), Variables.end(),inserter(Common, Common.begin()));

      if(!Common.empty()) // There is some variable in common. Hence it is bound
	{
	  Bound_Constraints.insert(root);
	}
      else
	{
	 Free_Constraints.insert(root);
	} 
    }
}




// Convert Simplified_Bound_Part into conjunction of QFree_Bound_Part and Quantified_Bound_Part
// Quantified_Bound_Part has VarsToElim
// QFree_Bound_Part is free of VarsToElim 
void convertToBoundAndFreeParts(DAGNode* Simplified_Bound_Part, set<string> &VarsToElim, DAGNode* &QFree_Bound_Part, DAGNode* &Quantified_Bound_Part, map<string, int> &WidthTable)
{
    set<DAGNode*> Constraints;
    splitConjunctionIntoSetGeneral(Simplified_Bound_Part, Constraints);
    vector<DAGNode*> Constraints_Vector;
    copySetToVector(Constraints, Constraints_Vector);
    
    set<DAGNode*> Free_Constraints;
    set<DAGNode*> Bound_Constraints;
    getFreeAndBoundConstraintsForMultipleVariables(Constraints_Vector, VarsToElim, Free_Constraints, Bound_Constraints, WidthTable);
    
    
    QFree_Bound_Part = getConjunctionFromSet(Free_Constraints);
    Quantified_Bound_Part = getConjunctionFromSet(Bound_Constraints);
}


// Version of convertBVUGEConstraintsToBVULE s.t. Phi 
// can contain equalities, disequalities also.
// Also recreate all the constraints with a proper variable ordering
DAGNode* convertBVUGEConstraintsToBVULECorrected(DAGNode* Phi, map<string, int> &WidthTable)
{
   set<DAGNode*> Constraints;
   splitConjunctionIntoSetGeneral(Phi, Constraints);
   set<DAGNode*> NewConstraints;
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* Constraint = *it;
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
        string ConstraintType = Constraint->Get_node_name();
               
        if(ConstraintType == "bvule")//already bvule
        {
         DAGNode* NewConstraint;         
         recreateWeakConstraint(Constraint, WidthTable, NewConstraint);
         // This helps in proper ordering of terms
         NewConstraints.insert(NewConstraint);   
        }
        else if(ConstraintType == "bvuge")//bvuge constraint
        {
         DAGNode* NewConstraint;
         recreateWeakConstraintFromComponents(RHS, LHS, "bvule", WidthTable, NewConstraint);
         //recreate LHS >= RHS as RHS <= LHS
         NewConstraints.insert(NewConstraint);   
        } // bvuge type
        else
        {
         NewConstraints.insert(Constraint);    
         }
    }//for
    
    
DAGNode* resultDAG = getConjunctionFromSet(NewConstraints);
return resultDAG;
    
}//function





// Version of recreateWeakConstraint except that recreation happens from components
void recreateWeakConstraintFromComponents(DAGNode *LHS, DAGNode *RHS, string LMIType, map<string, int> &WidthTable, DAGNode* &RecreatedConstraint)
{
    set<string> Support_Constraint;
    getDAGNames(LHS, Support_Constraint);
    getDAGNames(RHS, Support_Constraint);
    
    if(Support_Constraint.empty())
    {
        cout<<"\nEmpty support for constraint in function recreateWeakConstraintFromComponents in Eliminator.cpp\n";
        exit(1);
    }
    set<string>::iterator it = Support_Constraint.begin();
    string Variable = *it;
    map<string, int>::iterator wit = WidthTable.find(Variable);
    if(wit == WidthTable.end())
    {
        cout<<"\nNo entry for the variable "<<Variable<<" in WidthTable in function recreateWeakConstraintFromComponents in Eliminator.cpp\n";
        exit(1);
    }
    int WidthOfVar = wit->second;
    ULINT ModWidthOfVar = findPower(WidthOfVar);
    
    
    list<string> VarNames;
    attachSetToList(Support_Constraint, VarNames);

    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, Unit_Const_Mat;

    getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(RHS, VarNames, T_Coef_Mat, T_Const_Mat);
   
     vector<string> LMITypes;
     LMITypes.push_back(LMIType);
    
    RecreatedConstraint = quickSimplify(getDAGFromInequations(S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat, VarNames, ModWidthOfVar, LMITypes));
}




// Check if each constraint is either 1) free of variable or
// 2) Does not have variable on both sides and appears as k.v <= RHS or LHS <= k.v (note that k should 
// be the same for all constraints) where k is an odd number
bool constraintsAlreadyInNormalForm(set<DAGNode*> &Constraints, string variable, map<string, int> &WidthTable)
{
    bool KValue_obtained = false;
    ULINT Public_KValue;
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* Constraint = *it;
        //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
        
        set<string> Support_Constraint;
        getDAGNames(Constraint, Support_Constraint);
        
        if(Support_Constraint.find(variable) == Support_Constraint.end()) //Constraint free of variable
            {
            continue;
            }
        
        if(Constraint->Get_node_name() == "is_not_equal") //LMD encountered
            // LMD needs to be normalized into disjunction of LMIs
            {
            //cout<<"\n"<<getDAGStr(Constraint)<<" is an LMD in function constraintsAlreadyInNormalForm in DAGEliminator.cpp\n";
            return false;
            }
        
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
        set<string> Support_LHS;
        set<string> Support_RHS;
        
        getDAGNames(LHS, Support_LHS);
        getDAGNames(RHS, Support_RHS);
        
        if(Support_LHS.find(variable) != Support_LHS.end() && Support_RHS.find(variable) != Support_RHS.end()) //Both LHS and RHS have the variable
            {
            //cout<<"\nBoth LHS and RHS of "<<getDAGStr(Constraint)<<" has the variable "<<variable<<" in function constraintsAlreadyInNormalForm in DAGEliminator.cpp\n";
            //exit(1);
            return false;
            }
        
        if(Support_LHS.find(variable) != Support_LHS.end())// variable appears only in the LHS
            {
             //cout<<"\nVariable appears only in the LHS";
            
             list<string> VarNames;
             VarNames.push_back(variable);
             
             attachSetToList(Support_LHS, VarNames);

              Matrix S_Coef_Mat, S_Const_Mat;
              getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
              
              ULINT My_KValue = S_Coef_Mat[0][0];
              
              //cout<<"\nMy_KValue = "<<My_KValue<<endl;
              
              nullifyColumn(S_Coef_Mat, 0);
              
              if(!IsNullMatrix(S_Coef_Mat) || !IsNullMatrix(S_Const_Mat))
                {//something is appearing with k.variable
                  
                  //cout<<"\nsomething is appearing with k.variable\n";
                  return false;
                }
              if(KValue_obtained)
                {
                 //cout<<"\nKValue_obtained\n";                   
                 if(My_KValue != Public_KValue) // ks are different
                    {
                     //cout<<"\nMy_KValue != Public_KValue\n";
                     return false;
                    }// if(My_KValue != Public_KValue) 
                }// if(KValue_obtained)
              else
                {
                  //cout<<"\nK_value is  not obtained\n";
                  Public_KValue = My_KValue;
                  KValue_obtained = true;
                }//else of if(KValue_obtained)
              
            }//if(Support_LHS.find(variable) != Support_LHS.end())
        else//variable appears on RHS
             {
             list<string> VarNames;
             VarNames.push_back(variable);
             
             attachSetToList(Support_RHS, VarNames);

              Matrix S_Coef_Mat, S_Const_Mat;
              getCoefsAndConstsModified(RHS, VarNames, S_Coef_Mat, S_Const_Mat);
              
              ULINT My_KValue = S_Coef_Mat[0][0];
              
              //cout<<"\nMy_KValue = "<<My_KValue<<endl;
              
              nullifyColumn(S_Coef_Mat, 0);
              
              if(!IsNullMatrix(S_Coef_Mat) || !IsNullMatrix(S_Const_Mat))
                {//something is appearing with k.variable
                  //cout<<"\nsomething is appearing with k.variable\n";
                  return false;
                }
              if(KValue_obtained)
                {
                 //cout<<"\nKValue_obtained\n";  
                 if(My_KValue != Public_KValue) // ks are different
                    {
                     //cout<<"\nMy_KValue != Public_KValue\n";
                     return false;
                    }// if(My_KValue != Public_KValue) 
                }// if(KValue_obtained)
              else
                {
                  //cout<<"\nK_value is  not obtained\n";
                  Public_KValue = My_KValue;
                  KValue_obtained = true;
                }//else of if(KValue_obtained)
              
            }//else of if(Support_LHS.find(variable) != Support_LHS.end())       
        
    }//for
    
    
    //cout<<"\nAll occurances are like k.variable\n";
    
    return true;    
}





// Checks if the simple version of Fourier-Mozkin can be applied for QE
// from /\Constraints.
// Simple version of Fourier-Motzkin can be applied if
// all occurances of variable in the Constraints are odd
bool simpleFourierMotzkinCanBeApplied(set<DAGNode*> &Constraints, string variable,  map<string, int> &WidthTable)
{
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        DAGNode* Constraint = *it;
               
        set<string> Support_Constraint;
        getDAGNames(Constraint, Support_Constraint);
        
        if(Support_Constraint.find(variable) == Support_Constraint.end()) //Constraint free of variable
            {
            cout<<"\nThe constraint "<<getDAGStr(Constraint)<<" is free of "<<variable<<". Then why is it passed to the function simpleFourierMotzkinCanBeApplied in Eliminator.cpp\n";
            exit(1);
            }
        
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *LHS = *iout;
        iout++;
        DAGNode *RHS = *iout;	
        
        set<string> Support_LHS;
        set<string> Support_RHS;
        
        getDAGNames(LHS, Support_LHS);
        getDAGNames(RHS, Support_RHS);
        
        if(Support_LHS.find(variable) != Support_LHS.end() && Support_RHS.find(variable) != Support_RHS.end()) //Both LHS and RHS has the variable
            {
            cout<<"\nBoth LHS and RHS of "<<getDAGStr(Constraint)<<" has the variable "<<variable<<" in function simpleFourierMotzkinCanBeApplied in Eliminator.cpp\n";
            exit(1);
            }
        
        if(Support_LHS.find(variable) != Support_LHS.end())// variable appears only in the LHS
            {
             list<string> VarNames;
             VarNames.push_back(variable);
             
             attachSetToList(Support_LHS, VarNames);

             Matrix S_Coef_Mat, S_Const_Mat;
             getCoefsAndConstsModified(LHS, VarNames, S_Coef_Mat, S_Const_Mat);
              
             ULINT MyCoefficient = S_Coef_Mat[0][0];
             
             if(MyCoefficient%2 == 0) return false;//even coefficient
             }//if(Support_LHS.find(variable) != Support_LHS.end())
        else//variable appears on RHS
             {
             list<string> VarNames;
             VarNames.push_back(variable);
             
             attachSetToList(Support_RHS, VarNames);

             Matrix S_Coef_Mat, S_Const_Mat;
             getCoefsAndConstsModified(RHS, VarNames, S_Coef_Mat, S_Const_Mat);
              
             ULINT MyCoefficient = S_Coef_Mat[0][0];
             
             if(MyCoefficient%2 == 0) return false;//even coefficient
            }//else of if(Support_LHS.find(variable) != Support_LHS.end())       
        
        }//for   
       
    return true;    
    }




// Eliminate quantifiers from the conjunction using Fourier-Motzkin style QE where the variable has odd coefficient
DAGNode* applyFourierMotzkin(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable)
{
  set<DAGNode*> QE_Constraints;
  vector<DAGNode*> LowerBounds;
  vector<DAGNode*> UpperBounds;
  

   for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;

     list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

     DAGNode *LHS = *iout;
     iout++;
     DAGNode *RHS = *iout;	       

     set<string> Support_LHS;
     set<string> Support_RHS;

     getDAGNames(LHS, Support_LHS);
     getDAGNames(RHS, Support_RHS);

     if(Support_LHS.find(VarToElim)!= Support_LHS.end())
            {   
            UpperBounds.push_back(RHS);
            }
     else if(Support_RHS.find(VarToElim)!= Support_RHS.end())
            {
            LowerBounds.push_back(LHS); 
            }
    }//for ends here

  for(int i=0; i<LowerBounds.size(); i++)
    {
     for(int j=0; j<UpperBounds.size(); j++)
     {
     DAGNode* L = LowerBounds[i];
     DAGNode* U = UpperBounds[j];

     //cout<<"\nDoing FM on "<<getDAGStr(L)<<" <= "<<VarToElim<< " <= "<<getDAGStr(U)<<endl;

     DAGNode* FMResult = createDAG("bvule", L,  U, LOGICAL_SYMBOL, 1);
     //cout<<"\nFMResult = "<<getDAGStr(FMResult)<<endl;   
     QE_Constraints.insert(FMResult);
     }// for j
    }// for i


DAGNode* Result = quickSimplify_WithLMISupport(getConjunctionFromSet(QE_Constraints));    
//cout<<"\nFinal result of FM elimination = "<<getDAGStr(Result)<<endl;

return Result; 
}//function




// Corrected version of convertToNormalForm for multiple variables.
// We found some bugs before reporting to CAV'12.
// These bugs are corrected here.
DAGNode* convertToNormalFormForMultipleVariablesCorrected(set<DAGNode*> &Constraints, set<string> &VarsToElim, map<string, int> &WidthTable,  list<DAGNode*> &APListForNormalizedDD)
{
    
   bool LMD_Normalization_On = true; 
    
   if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp has timed out. Its result is not exact\n";
	timed_out = true; // timed_out flag set
	return createLogicalConstant("true");
	}  
    
    
    vector<DAGNode*> Constraints_Vector;
    vector< vector<DAGNode*> > SplittedConstraints;
    
    if(!LMD_Normalization_On) //LMDs are not permitted
    {
        // Let us ensure that we have only <= constraints
        for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
        {
            DAGNode* Constraint = *it;
            if(Constraint->Get_node_name() != "bvule")
            {
                cout<<"\nUnsupported constraint "<<getDAGStr(Constraint)<<" in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
                cout<<"Presently the normalization is implemented only for LMIs\n";
                exit(1);
            }
        }//for
   }// if(!LMD_Normalization_On)
    
    for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
        Constraints_Vector.push_back(*it);        
    }
    
    map<DAGNode*, string> ConstraintVarToElimMap;
    
    for(int constraint_index=0; constraint_index < Constraints_Vector.size(); constraint_index++)
    {
        DAGNode *Constraint = Constraints_Vector[constraint_index];
        //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
        set<string> Support;
        
        getDAGNames(Constraint, Support);
        set<string> MyVariables;
        set_intersection(Support.begin(), Support.end(), VarsToElim.begin(), VarsToElim.end(),
		     inserter(MyVariables, MyVariables.begin()));
        
         if(MyVariables.size() == 0)
            {
            //cout<<"\nNo variables to normalize\n";
            continue;
            }
        
        if(MyVariables.size() > 1)
            {
            cout<<"\nVariables to normalize is multiple in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
            exit(1);
            }
        string MyVariable;
        set<string>::iterator it = MyVariables.begin();
        MyVariable = *it;
        //cout<<"\nMyVariable = "<<MyVariable<<endl;        
        ConstraintVarToElimMap.insert(make_pair(Constraint, MyVariable));
    }
    
    map<string, bool> ConstraintEncountered;
    for(set<string>::iterator it=VarsToElim.begin(); it != VarsToElim.end(); it++)
    {
        string MyVariable = *it;
        ConstraintEncountered.insert(make_pair(MyVariable, false));
    }   
    
    map<string, ULINT> CoefsOfVariable;
    map<string, ULINT> AICoefsOfVariable;
    
    
    for(int constraint_index=0; constraint_index < Constraints_Vector.size(); constraint_index++)
    {
        DAGNode *Constraint = Constraints_Vector[constraint_index];
        //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
        
        string constraint_type = Constraint->Get_node_name();
        
        map<DAGNode*, string>::iterator it=ConstraintVarToElimMap.find(Constraint);
        if(it == ConstraintVarToElimMap.end())
        {
            //cout<<"\nConstraint free of vars to elim\n";
            vector<DAGNode*> SplittedVector;
            SplittedVector.push_back(Constraint);
            SplittedConstraints.push_back(SplittedVector);  
            continue;
        }
        
        string VarToElim = it->second;
        //cout<<"\nVarToElim = "<<VarToElim<<endl;
        
        
        int WidthOfVarToElim;
        ULINT ModOfVarToElim;
        
    
        map<string, int>::iterator wit = WidthTable.find(VarToElim);
        if(wit == WidthTable.end())
            {
            cout<<"\nNo entry for the variable "<<VarToElim<<" in WidthTable in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
            exit(1);
            }
        WidthOfVarToElim = wit->second;
        ModOfVarToElim = findPower(WidthOfVarToElim);
    
        //cout<<"\nWidthOfVarToElim = "<<WidthOfVarToElim<<endl;
        //cout<<"\nModOfVarToElim = "<<ModOfVarToElim<<endl;
        
        string side_of_variable = findSideOfVariableInConstraint(Constraint, VarToElim);
        //cout<<"\nside_of_variable = "<<side_of_variable<<endl;
        
        bool both_sides_implemented = true;
        if(!both_sides_implemented)
        {
               if(side_of_variable == "both")
                {
                 cout<<"\nThe variable "<<VarToElim<<" appears on both sides of "<<getDAGStr(Constraint)<<" in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp"<<endl;
                exit(1);
                }//if(side_of_variable == "both")
        }
        
        if(side_of_variable == "none")
        {
            //cout<<"\nThe variable "<<VarToElim<<" does not appear on any side of "<<getDAGStr(Constraint)<<" in function convertToNormalForm in DAGEliminator.cpp"<<endl;
            
            vector<DAGNode*> SplittedVector;
            SplittedVector.push_back(Constraint);
            SplittedConstraints.push_back(SplittedVector);  
            continue;
        }//if(side_of_variable == "none")
        
        string type_of_coefficient = "original";
        
        map<string, bool>::iterator cit = ConstraintEncountered.find(VarToElim);
        if(cit == ConstraintEncountered.end())
            {
            cout<<"\nNo entry for the variable "<<VarToElim<<" in ConstraintEncountered in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
            exit(1);
            }
        bool first_constraint_encountered = cit->second;
        
        if(!first_constraint_encountered)//first constraint
        {
            //cout<<"\n"<<VarToElim<<" is encountered the first time\n";
            ULINT CoefOfVarToElim;
            ULINT AIOfCoefOfVarToElim;
            getCoefOfVarToElimAndAIOfCoefOfVarToElim(Constraint, VarToElim, side_of_variable, ModOfVarToElim, CoefOfVarToElim, AIOfCoefOfVarToElim);
            //cout<<"\nCoefOfVarToElim = "<<CoefOfVarToElim<<", AIOfCoefOfVarToElim = "<<AIOfCoefOfVarToElim<<endl;
            CoefsOfVariable.insert(make_pair(VarToElim, CoefOfVarToElim));
            AICoefsOfVariable.insert(make_pair(VarToElim, AIOfCoefOfVarToElim));
            ConstraintEncountered[VarToElim] = true;
        }//if(constr_index==0)
        else
        {
            //cout<<"\n"<<VarToElim<<" is not encountered the first time\n";
            ULINT CoefOfVarToElim;
            ULINT AIOfCoefOfVarToElim;
            
            map<string, ULINT>::iterator coef_it = CoefsOfVariable.find(VarToElim);
            if(coef_it == CoefsOfVariable.end())
                {
                cout<<"\nNo entry for "<<VarToElim<<" in CoefsOfVariable in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
                exit(1);
                }
            CoefOfVarToElim = coef_it->second;
            
            map<string, ULINT>::iterator aicoef_it = AICoefsOfVariable.find(VarToElim);
            if(aicoef_it == AICoefsOfVariable.end())
                {
                cout<<"\nNo entry for "<<VarToElim<<" in AICoefsOfVariable in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
                exit(1);
                }
            AIOfCoefOfVarToElim = aicoef_it->second;
            
            
            type_of_coefficient = findTypeOfCoefficient(Constraint, VarToElim, side_of_variable, CoefOfVarToElim, AIOfCoefOfVarToElim);
            //cout<<"\ntype_of_coefficient = "<<type_of_coefficient<<endl;
        }// else of if(constr_index==0)
        
             
        vector<DAGNode*> SplittedVector;
        
        
        if(constraint_type == "bvule")
        {
            if(side_of_variable == "left" || side_of_variable == "right")
            {
               normalizeConstraintsWithVariableOnOneSide(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector, APListForNormalizedDD);
             }
            else if(side_of_variable == "both")
            {
                normalizeConstraintsWithVariableOnBothSides(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector, APListForNormalizedDD);
             }
            else
            {
              cout<<"\nUnexpected side_of_variable "<<side_of_variable<<" in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
              exit(1);  
            }
         }// if(constraint_type == "bvule")
        else if(constraint_type == "is_not_equal")
        {
            normalizeLMDs(Constraint, VarToElim, WidthTable, side_of_variable, type_of_coefficient, ModOfVarToElim, SplittedVector, APListForNormalizedDD);            
        }//else if(constraint_type == "is_not_equal")
        else
        {
           cout<<"\nUnhandled constraint_type "<<constraint_type<<" in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
           exit(1);  
        }
        
      //showAPVector("SplittedVector", SplittedVector);
      
      if(SplittedVector.size()>0)
        {
        SplittedConstraints.push_back(SplittedVector);        
        }
    }//for ends
 
 vector<DAGNode*> Final_Vector;
 
 for(int i=0; i<SplittedConstraints.size(); i++)
    {
      vector<DAGNode*> TempVector = SplittedConstraints[i];
      
      if(TempVector.size()==0)
        {
        cout<<"\nSplitted vector becoming empty in function convertToNormalFormForMultipleVariablesCorrected in Eliminator.cpp\n";
        exit(1);
        }
      
      DAGNode* TempDAG = getDisjunctionFromVector(TempVector);
      Final_Vector.push_back(TempDAG);
    }
 
 DAGNode* resultDAG = getConjunctionFromVector(Final_Vector);
 return resultDAG;
}  




// Normalize the constraints of the form ((+/-).kx+e_1 <= e_2) and (e_1 <= (+/-).kx+e_2)
void normalizeConstraintsWithVariableOnOneSide(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector, list<DAGNode*> &APListForNormalizedDD)
{    
    list<string> VarNames;
    VarNames.push_back(VarToElim);
    set<string> Support;
    getDAGNames(Constraint, Support);
    attachSetToList(Support, VarNames);
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *S = *iout;
    iout++;
    DAGNode *T = *iout;	
    
    getCoefsAndConstsModified(S, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(T, VarNames, T_Coef_Mat, T_Const_Mat);
        
    //showList(VarNames, "VarNames");
    //cout<<"\nS_Coef_Mat\n";
    //showMatrix(S_Coef_Mat);
    //cout<<"\nS_Const_Mat\n";
    //showMatrix(S_Const_Mat);
    //cout<<"\nT_Coef_Mat\n";
    //showMatrix(T_Coef_Mat);
    //cout<<"\nT_Const_Mat\n";
    //showMatrix(T_Const_Mat);
    
    
    Matrix e1_Coef_Mat, e1_Const_Mat, AIe1_Coef_Mat, AIe1_Const_Mat;
    Matrix e2_Coef_Mat, e2_Const_Mat, AIe2_Coef_Mat, AIe2_Const_Mat;
    Matrix K_Coef_Mat, K_Const_Mat;
    
    if(type_of_coefficient == "original")
    {
    
        if(side_of_variable=="left")
        {
            copyMatrix(S_Coef_Mat, K_Coef_Mat);
            nullifyColumnsExceptOne(K_Coef_Mat, 0);
            getNullMatrix(K_Const_Mat, 1, 1);
            copyMatrix(S_Coef_Mat, e1_Coef_Mat);
            copyMatrix(S_Const_Mat, e1_Const_Mat);
            nullifyColumn(e1_Coef_Mat, 0);

            copyMatrix(e1_Coef_Mat, AIe1_Coef_Mat);
            copyMatrix(e1_Const_Mat, AIe1_Const_Mat);
            aInverseOfMatrix(AIe1_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe1_Const_Mat, ModOfVarToElim);

            copyMatrix(T_Coef_Mat, e2_Coef_Mat);
            copyMatrix(T_Const_Mat, e2_Const_Mat);      

            copyMatrix(e2_Coef_Mat, AIe2_Coef_Mat);
            copyMatrix(e2_Const_Mat, AIe2_Const_Mat);
            aInverseOfMatrix(AIe2_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe2_Const_Mat, ModOfVarToElim);
            
            
            //cout<<"\ne1_Coef_Mat\n";
            //showMatrix(e1_Coef_Mat);
            //cout<<"\ne1_Const_Mat\n";
            //showMatrix(e1_Const_Mat);
            //cout<<"\nAIe1_Coef_Mat\n";
            //showMatrix(AIe1_Coef_Mat);
            //cout<<"\nAIe1_Const_Mat\n";
            //showMatrix(AIe1_Const_Mat);
            //cout<<"\ne2_Coef_Mat\n";
            //showMatrix(e2_Coef_Mat);
            //cout<<"\ne2_Const_Mat\n";
            //showMatrix(e2_Const_Mat);
            //cout<<"\nAIe2_Coef_Mat\n";
            //showMatrix(AIe2_Coef_Mat);
            //cout<<"\nAIe2_Const_Mat\n";
            //showMatrix(AIe2_Const_Mat);
            //cout<<"\nK_Coef_Mat\n";
            //showMatrix(K_Coef_Mat);
            //cout<<"\nK_Const_Mat\n";
            //showMatrix(K_Const_Mat);
            
            
            Matrix e2PlusAIe1_Coef_Mat, e2PlusAIe1_Const_Mat;
            // Finding e2+AI(e1)
            copyMatrix(AIe1_Coef_Mat, e2PlusAIe1_Coef_Mat);
            copyMatrix(AIe1_Const_Mat, e2PlusAIe1_Const_Mat);
            matAdd(e2PlusAIe1_Coef_Mat, e2_Coef_Mat, ModOfVarToElim);
            matAdd(e2PlusAIe1_Const_Mat, e2_Const_Mat, ModOfVarToElim);
            
            //cout<<"\ne2PlusAIe1_Coef_Mat\n";
            //showMatrix(e2PlusAIe1_Coef_Mat);
            //cout<<"\ne2PlusAIe1_Const_Mat\n";
            //showMatrix(e2PlusAIe1_Const_Mat);
            
            
            DAGNode *e1EqZero, *kxGteAIe1, *e2Gtee1, *kxLtee2PlusAIe1;
            
            vector<string> LMITypesBVULE;
            LMITypesBVULE.push_back("bvule");
    
            vector<string> LMITypesBVUGE;
            LMITypesBVUGE.push_back("bvuge");
    
    
            e1EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(e1_Coef_Mat, AIe1_Const_Mat, VarNames, ModOfVarToElim));
            //cout<<"\ne1EqZero = "<<getDAGStr(e1EqZero)<<endl;
    
            kxGteAIe1 = quickSimplify_WithLMISupport(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIe1_Coef_Mat, AIe1_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
            //cout<<"\nkxGteAIe1 = "<<getDAGStr(kxGteAIe1)<<endl;
    
            e2Gtee1 = quickSimplify_WithLMISupport(getDAGFromInequations(e2_Coef_Mat, e2_Const_Mat, e1_Coef_Mat, e1_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
            //cout<<"\ne2Gtee1 = "<<getDAGStr(e2Gtee1)<<endl;
    
            kxLtee2PlusAIe1 = quickSimplify_WithLMISupport(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, e2PlusAIe1_Coef_Mat, e2PlusAIe1_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
            //cout<<"\nkxLtee2PlusAIe1 = "<<getDAGStr(kxLtee2PlusAIe1)<<endl;
            
            
            if(isTrue(e1EqZero))//e_1 = 0. No need to normalize
            {
              //cout<<"\n"<<getDAGStr(Constraint)<<" is already normalized\n";
              SplittedVector.push_back(Constraint);  
            }
            else
            {            
                vector<DAGNode*> Constraint1_Vector;
                Constraint1_Vector.push_back(createDAG("not", e1EqZero, LOGICAL_SYMBOL, 1));
                Constraint1_Vector.push_back(kxGteAIe1);
                Constraint1_Vector.push_back(e2Gtee1);                   
                DAGNode* Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);


                Constraint1_Vector.clear();
                Constraint1_Vector.push_back(e1EqZero);
                Constraint1_Vector.push_back(kxLtee2PlusAIe1);
                Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);


                Constraint1_Vector.clear();
                Constraint1_Vector.push_back(kxGteAIe1);
                Constraint1_Vector.push_back(kxLtee2PlusAIe1);
                Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);


                Constraint1_Vector.clear();
                Constraint1_Vector.push_back(createDAG("not", e1EqZero, LOGICAL_SYMBOL, 1));
                Constraint1_Vector.push_back(e2Gtee1);
                Constraint1_Vector.push_back(kxLtee2PlusAIe1);
                Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);
                
                
                if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                {
                    pushBackIntoAPList(APListForNormalizedDD, e1EqZero);
                    pushBackIntoAPList(APListForNormalizedDD, kxGteAIe1);
                    pushBackIntoAPList(APListForNormalizedDD, e2Gtee1);
                    pushBackIntoAPList(APListForNormalizedDD, kxLtee2PlusAIe1);
                }// if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                
            }
            
        }
        else  if(side_of_variable=="right")
        {
            copyMatrix(T_Coef_Mat, K_Coef_Mat);
            nullifyColumnsExceptOne(K_Coef_Mat, 0);
            getNullMatrix(K_Const_Mat, 1, 1);        
            copyMatrix(T_Coef_Mat, e2_Coef_Mat);
            copyMatrix(T_Const_Mat, e2_Const_Mat);
            nullifyColumn(e2_Coef_Mat, 0);

            copyMatrix(e2_Coef_Mat, AIe2_Coef_Mat);
            copyMatrix(e2_Const_Mat, AIe2_Const_Mat);
            aInverseOfMatrix(AIe2_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe2_Const_Mat, ModOfVarToElim);

            copyMatrix(S_Coef_Mat, e1_Coef_Mat);
            copyMatrix(S_Const_Mat, e1_Const_Mat);     


            copyMatrix(e1_Coef_Mat, AIe1_Coef_Mat);
            copyMatrix(e1_Const_Mat, AIe1_Const_Mat);
            aInverseOfMatrix(AIe1_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe1_Const_Mat, ModOfVarToElim);
            
            
            //cout<<"\ne1_Coef_Mat\n";
            //showMatrix(e1_Coef_Mat);
            //cout<<"\ne1_Const_Mat\n";
            //showMatrix(e1_Const_Mat);
            //cout<<"\nAIe1_Coef_Mat\n";
            //showMatrix(AIe1_Coef_Mat);
            //cout<<"\nAIe1_Const_Mat\n";
            //showMatrix(AIe1_Const_Mat);
            //cout<<"\ne2_Coef_Mat\n";
            //showMatrix(e2_Coef_Mat);
            //cout<<"\ne2_Const_Mat\n";
            //showMatrix(e2_Const_Mat);
            //cout<<"\nAIe2_Coef_Mat\n";
            //showMatrix(AIe2_Coef_Mat);
            //cout<<"\nAIe2_Const_Mat\n";
            //showMatrix(AIe2_Const_Mat);
            //cout<<"\nK_Coef_Mat\n";
            //showMatrix(K_Coef_Mat);
            //cout<<"\nK_Const_Mat\n";
            //showMatrix(K_Const_Mat);
            
            
            Matrix e1PlusAIe2_Coef_Mat, e1PlusAIe2_Const_Mat;
            // Finding e1+AI(e2)
            copyMatrix(AIe2_Coef_Mat, e1PlusAIe2_Coef_Mat);
            copyMatrix(AIe2_Const_Mat, e1PlusAIe2_Const_Mat);
            matAdd(e1PlusAIe2_Coef_Mat, e1_Coef_Mat, ModOfVarToElim);
            matAdd(e1PlusAIe2_Const_Mat, e1_Const_Mat, ModOfVarToElim);
            
            //cout<<"\ne1PlusAIe2_Coef_Mat\n";
            //showMatrix(e1PlusAIe2_Coef_Mat);
            //cout<<"\ne1PlusAIe2_Const_Mat\n";
            //showMatrix(e1PlusAIe2_Const_Mat);
            
            Matrix MinusOne_Coef_Mat, MinusOne_Const_Mat;
              
            getNullMatrix(MinusOne_Coef_Mat, 1, VarNames.size());
            getNullMatrix(MinusOne_Const_Mat, 1, 1);
            MinusOne_Const_Mat[0][0]=ModOfVarToElim-1;
            
            //cout<<"\nMinusOne_Coef_Mat\n";
            //showMatrix(MinusOne_Coef_Mat);
            //cout<<"\nMinusOne_Const_Mat\n";
            //showMatrix(MinusOne_Const_Mat);
            
            
            Matrix e2MinusOne_Coef_Mat, e2MinusOne_Const_Mat;
            Matrix AIe2MinusOne_Coef_Mat, AIe2MinusOne_Const_Mat;
            // Finding e2-1 and AI(e2)-1
            copyMatrix(e2_Coef_Mat, e2MinusOne_Coef_Mat);
            copyMatrix(e2_Const_Mat, e2MinusOne_Const_Mat);
            matAdd(e2MinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
            matAdd(e2MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
            
            copyMatrix(AIe2_Coef_Mat, AIe2MinusOne_Coef_Mat);
            copyMatrix(AIe2_Const_Mat, AIe2MinusOne_Const_Mat);
            matAdd(AIe2MinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
            matAdd(AIe2MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
            
            //cout<<"\ne2MinusOne_Coef_Mat\n";
            //showMatrix(e2MinusOne_Coef_Mat);
            //cout<<"\ne2MinusOne_Const_Mat\n";
            //showMatrix(e2MinusOne_Const_Mat);
            
            //cout<<"\nAIe2MinusOne_Coef_Mat\n";
            //showMatrix(AIe2MinusOne_Coef_Mat);
            //cout<<"\nAIe2MinusOne_Const_Mat\n";
            //showMatrix(AIe2MinusOne_Const_Mat);
            
            
            DAGNode *e2EqZero, *kxLteAIe2MinusOne, *e1Ltee2MinusOne, *e1PlusAIe2Ltekx;
            
            vector<string> LMITypesBVULE;
            LMITypesBVULE.push_back("bvule");
    
              
            e2EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(e2_Coef_Mat, AIe2_Const_Mat, VarNames, ModOfVarToElim));
            //cout<<"\ne2EqZero = "<<getDAGStr(e2EqZero)<<endl;
    
            kxLteAIe2MinusOne = quickSimplify_WithLMISupport(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, AIe2MinusOne_Coef_Mat, AIe2MinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
            //cout<<"\nkxLteAIe2MinusOne = "<<getDAGStr(kxLteAIe2MinusOne)<<endl;
    
            e1Ltee2MinusOne = quickSimplify_WithLMISupport(getDAGFromInequations(e1_Coef_Mat, e1_Const_Mat, e2MinusOne_Coef_Mat, e2MinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
            //cout<<"\ne1Ltee2MinusOne = "<<getDAGStr(e1Ltee2MinusOne)<<endl;
    
            e1PlusAIe2Ltekx = quickSimplify_WithLMISupport(getDAGFromInequations(e1PlusAIe2_Coef_Mat, e1PlusAIe2_Const_Mat,  K_Coef_Mat, K_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
            //cout<<"\ne1PlusAIe2Ltekx = "<<getDAGStr(e1PlusAIe2Ltekx)<<endl;
            
            
            if(isTrue(e2EqZero))//e_2 = 0. No need to normalize
            {
              //cout<<"\n"<<getDAGStr(Constraint)<<" is already normalized\n";
              SplittedVector.push_back(Constraint);  
            }
            else
            { 
                vector<DAGNode*> Constraint1_Vector;
                Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));
                Constraint1_Vector.push_back(kxLteAIe2MinusOne);
                Constraint1_Vector.push_back(e1Ltee2MinusOne);                   
                DAGNode* Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);


                Constraint1_Vector.clear();
                Constraint1_Vector.push_back(e2EqZero);
                Constraint1_Vector.push_back(e1PlusAIe2Ltekx);
                Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);


                Constraint1_Vector.clear();
                Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));
                Constraint1_Vector.push_back(e1Ltee2MinusOne);
                Constraint1_Vector.push_back(e1PlusAIe2Ltekx);
                Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);


                Constraint1_Vector.clear();
                Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));
                Constraint1_Vector.push_back(kxLteAIe2MinusOne);
                Constraint1_Vector.push_back(e1PlusAIe2Ltekx);
                Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
                SplittedVector.push_back(Constraint1);
                
                if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                {
                    pushBackIntoAPList(APListForNormalizedDD, e2EqZero);
                    pushBackIntoAPList(APListForNormalizedDD, e1Ltee2MinusOne);
                    pushBackIntoAPList(APListForNormalizedDD, kxLteAIe2MinusOne);                    
                    pushBackIntoAPList(APListForNormalizedDD, e1PlusAIe2Ltekx);
                }// if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
            }
            
        }
        else
        {
             cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function normalizeConstraintsWithVariableOnOneSide in Eliminator.cpp\n";
             exit(1);
        }
    }
    else
    {
             
         if(side_of_variable=="left")
        {
            copyMatrix(S_Coef_Mat, K_Coef_Mat);
            nullifyColumnsExceptOne(K_Coef_Mat, 0);
            getNullMatrix(K_Const_Mat, 1, 1);
            aInverseOfMatrix(K_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(K_Const_Mat, ModOfVarToElim);
            
            copyMatrix(S_Coef_Mat, e1_Coef_Mat);
            copyMatrix(S_Const_Mat, e1_Const_Mat);
            nullifyColumn(e1_Coef_Mat, 0);

            copyMatrix(e1_Coef_Mat, AIe1_Coef_Mat);
            copyMatrix(e1_Const_Mat, AIe1_Const_Mat);
            aInverseOfMatrix(AIe1_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe1_Const_Mat, ModOfVarToElim);

            copyMatrix(T_Coef_Mat, e2_Coef_Mat);
            copyMatrix(T_Const_Mat, e2_Const_Mat);      

            copyMatrix(e2_Coef_Mat, AIe2_Coef_Mat);
            copyMatrix(e2_Const_Mat, AIe2_Const_Mat);
            aInverseOfMatrix(AIe2_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe2_Const_Mat, ModOfVarToElim);
            
            
            //cout<<"\ne1_Coef_Mat\n";
            //showMatrix(e1_Coef_Mat);
            //cout<<"\ne1_Const_Mat\n";
            //showMatrix(e1_Const_Mat);
            //cout<<"\nAIe1_Coef_Mat\n";
            //showMatrix(AIe1_Coef_Mat);
            //cout<<"\nAIe1_Const_Mat\n";
            //showMatrix(AIe1_Const_Mat);
            //cout<<"\ne2_Coef_Mat\n";
            //showMatrix(e2_Coef_Mat);
            //cout<<"\ne2_Const_Mat\n";
            //showMatrix(e2_Const_Mat);
            //cout<<"\nAIe2_Coef_Mat\n";
            //showMatrix(AIe2_Coef_Mat);
            //cout<<"\nAIe2_Const_Mat\n";
            //showMatrix(AIe2_Const_Mat);
            //cout<<"\nK_Coef_Mat\n";
            //showMatrix(K_Coef_Mat);
            //cout<<"\nK_Const_Mat\n";
            //showMatrix(K_Const_Mat);
            
            
            Matrix e2PlusAIe1_Coef_Mat, e2PlusAIe1_Const_Mat, AIe2PlusAIe1_Const_Mat;
            // Finding e2+AI(e1)
            copyMatrix(AIe1_Coef_Mat, e2PlusAIe1_Coef_Mat);
            copyMatrix(AIe1_Const_Mat, e2PlusAIe1_Const_Mat);
            matAdd(e2PlusAIe1_Coef_Mat, e2_Coef_Mat, ModOfVarToElim);
            matAdd(e2PlusAIe1_Const_Mat, e2_Const_Mat, ModOfVarToElim);
            
            copyMatrix(e2PlusAIe1_Const_Mat, AIe2PlusAIe1_Const_Mat);
            aInverseOfMatrix(AIe2PlusAIe1_Const_Mat, ModOfVarToElim);
            
            //cout<<"\ne2PlusAIe1_Coef_Mat\n";
            //showMatrix(e2PlusAIe1_Coef_Mat);
            //cout<<"\ne2PlusAIe1_Const_Mat\n";
            //showMatrix(e2PlusAIe1_Const_Mat);
            //cout<<"\nAIe2PlusAIe1_Const_Mat\n";
            //showMatrix(AIe2PlusAIe1_Const_Mat);
            
            
            Matrix e1PlusAIe2_Coef_Mat, e1PlusAIe2_Const_Mat;
            // Finding e1+AI(e2)
            copyMatrix(AIe2_Coef_Mat, e1PlusAIe2_Coef_Mat);
            copyMatrix(AIe2_Const_Mat, e1PlusAIe2_Const_Mat);
            matAdd(e1PlusAIe2_Coef_Mat, e1_Coef_Mat, ModOfVarToElim);
            matAdd(e1PlusAIe2_Const_Mat, e1_Const_Mat, ModOfVarToElim);
            
            //cout<<"\ne1PlusAIe2_Coef_Mat\n";
            //showMatrix(e1PlusAIe2_Coef_Mat);
            //cout<<"\ne1PlusAIe2_Const_Mat\n";
            //showMatrix(e1PlusAIe2_Const_Mat);
            
            
            DAGNode *e1EqZero, *kxEqZero, *e1Gtekx, *e2Gtee1, *e2PlusAIe1NeqZero, *kxGteAIe2Pluse1, *e2PlusAIe1EqZero;
            
               
            vector<string> LMITypesBVUGE;
            LMITypesBVUGE.push_back("bvuge");
    
    
            e1EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(e1_Coef_Mat, AIe1_Const_Mat, VarNames, ModOfVarToElim));
            //cout<<"\ne1EqZero = "<<getDAGStr(e1EqZero)<<endl;
    
            kxEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(K_Coef_Mat, K_Const_Mat, VarNames, ModOfVarToElim));
            //cout<<"\nkxEqZero = "<<getDAGStr(kxEqZero)<<endl; //K_Const_Mat is NULL always
    
            e1Gtekx = quickSimplify_WithLMISupport(getDAGFromInequations(e1_Coef_Mat, e1_Const_Mat, K_Coef_Mat, K_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
            //cout<<"\ne1Gtekx = "<<getDAGStr(e1Gtekx)<<endl;
    
            e2Gtee1 = quickSimplify_WithLMISupport(getDAGFromInequations(e2_Coef_Mat, e2_Const_Mat, e1_Coef_Mat, e1_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
            //cout<<"\ne2Gtee1 = "<<getDAGStr(e2Gtee1)<<endl;
    
            e2PlusAIe1EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(e2PlusAIe1_Coef_Mat, AIe2PlusAIe1_Const_Mat, VarNames, ModOfVarToElim));
           
            e2PlusAIe1NeqZero = createDAG("not",  e2PlusAIe1EqZero, LOGICAL_SYMBOL, 1);
            //cout<<"\ne2PlusAIe1NeqZero = "<<getDAGStr(e2PlusAIe1NeqZero)<<endl;
            
            kxGteAIe2Pluse1 = quickSimplify_WithLMISupport(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, e1PlusAIe2_Coef_Mat, e1PlusAIe2_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
            //cout<<"\nkxGteAIe2Pluse1 = "<<getDAGStr(kxGteAIe2Pluse1)<<endl;
            
            vector<DAGNode*> Constraint1_Vector;
            Constraint1_Vector.push_back(createDAG("not", e1EqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(createDAG("not", kxEqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(e1Gtekx);     
            Constraint1_Vector.push_back(e2Gtee1);     
            DAGNode* Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(e1EqZero);
            Constraint1_Vector.push_back(kxEqZero);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(e1EqZero);
            Constraint1_Vector.push_back(e2PlusAIe1NeqZero);
            Constraint1_Vector.push_back(kxGteAIe2Pluse1);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(createDAG("not", kxEqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(e1Gtekx);
            Constraint1_Vector.push_back(e2PlusAIe1NeqZero);
            Constraint1_Vector.push_back(kxGteAIe2Pluse1);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(createDAG("not", e1EqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(kxEqZero);
            Constraint1_Vector.push_back(e2Gtee1);     
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            Constraint1_Vector.clear(); 
            Constraint1_Vector.push_back(createDAG("not", e1EqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(e2Gtee1);   
            Constraint1_Vector.push_back(e2PlusAIe1NeqZero);   
            Constraint1_Vector.push_back(kxGteAIe2Pluse1);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            if(order_lmes_before_lmis_in_normalization)
            {
                pushBackIntoAPList(APListForNormalizedDD, e1EqZero);
                pushBackIntoAPList(APListForNormalizedDD, kxEqZero);
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1EqZero); 
                pushBackIntoAPList(APListForNormalizedDD, e2Gtee1);
                pushBackIntoAPList(APListForNormalizedDD, e1Gtekx);                              
                pushBackIntoAPList(APListForNormalizedDD, kxGteAIe2Pluse1);
            }// if(order_lmes_before_lmis_in_normalization)
            else if(order_lmcs_for_small_lmdds_in_normalization)
            {
                pushBackIntoAPList(APListForNormalizedDD, e1EqZero);
                pushBackIntoAPList(APListForNormalizedDD, kxEqZero);
                pushBackIntoAPList(APListForNormalizedDD, e1Gtekx);
                pushBackIntoAPList(APListForNormalizedDD, e2Gtee1);
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1EqZero);                
                pushBackIntoAPList(APListForNormalizedDD, kxGteAIe2Pluse1); 
            }// if(order_lmcs_for_small_lmdds_in_normalization)
        }
         
         else  if(side_of_variable=="right")
        {
            copyMatrix(T_Coef_Mat, K_Coef_Mat);
            nullifyColumnsExceptOne(K_Coef_Mat, 0);
            getNullMatrix(K_Const_Mat, 1, 1);
            aInverseOfMatrix(K_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(K_Const_Mat, ModOfVarToElim);
            
            copyMatrix(T_Coef_Mat, e2_Coef_Mat);
            copyMatrix(T_Const_Mat, e2_Const_Mat);
            nullifyColumn(e2_Coef_Mat, 0);

            copyMatrix(e2_Coef_Mat, AIe2_Coef_Mat);
            copyMatrix(e2_Const_Mat, AIe2_Const_Mat);
            aInverseOfMatrix(AIe2_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe2_Const_Mat, ModOfVarToElim);

            copyMatrix(S_Coef_Mat, e1_Coef_Mat);
            copyMatrix(S_Const_Mat, e1_Const_Mat);      

            copyMatrix(e1_Coef_Mat, AIe1_Coef_Mat);
            copyMatrix(e1_Const_Mat, AIe1_Const_Mat);
            aInverseOfMatrix(AIe1_Coef_Mat, ModOfVarToElim);
            aInverseOfMatrix(AIe1_Const_Mat, ModOfVarToElim);
            
            
            //cout<<"\ne1_Coef_Mat\n";
            //showMatrix(e1_Coef_Mat);
            //cout<<"\ne1_Const_Mat\n";
            //showMatrix(e1_Const_Mat);
            //cout<<"\nAIe1_Coef_Mat\n";
            //showMatrix(AIe1_Coef_Mat);
            //cout<<"\nAIe1_Const_Mat\n";
            //showMatrix(AIe1_Const_Mat);
            //cout<<"\ne2_Coef_Mat\n";
            //showMatrix(e2_Coef_Mat);
            //cout<<"\ne2_Const_Mat\n";
            //showMatrix(e2_Const_Mat);
            //cout<<"\nAIe2_Coef_Mat\n";
            //showMatrix(AIe2_Coef_Mat);
            //cout<<"\nAIe2_Const_Mat\n";
            //showMatrix(AIe2_Const_Mat);
            //cout<<"\nK_Coef_Mat\n";
            //showMatrix(K_Coef_Mat);
            //cout<<"\nK_Const_Mat\n";
            //showMatrix(K_Const_Mat);
            
            
            
            Matrix One_Coef_Mat, One_Const_Mat;
            
            getNullMatrix(One_Coef_Mat, 1, VarNames.size());
            getNullMatrix(One_Const_Mat, 1, 1);
            One_Const_Mat[0][0]=1;
            
            //cout<<"\nOne_Coef_Mat\n";
            //showMatrix(One_Coef_Mat);
            //cout<<"\nOne_Const_Mat\n";
            //showMatrix(One_Const_Mat);
            
            Matrix MinusOne_Coef_Mat, MinusOne_Const_Mat;
              
            getNullMatrix(MinusOne_Coef_Mat, 1, VarNames.size());
            getNullMatrix(MinusOne_Const_Mat, 1, 1);
            MinusOne_Const_Mat[0][0]=ModOfVarToElim-1;
            
            //cout<<"\nMinusOne_Coef_Mat\n";
            //showMatrix(MinusOne_Coef_Mat);
            //cout<<"\nMinusOne_Const_Mat\n";
            //showMatrix(MinusOne_Const_Mat);
            
            
            
            // Finding e2-1
            Matrix e2MinusOne_Coef_Mat, e2MinusOne_Const_Mat;
            // Finding e2-1
            copyMatrix(e2_Coef_Mat, e2MinusOne_Coef_Mat);
            copyMatrix(e2_Const_Mat, e2MinusOne_Const_Mat);
            matAdd(e2MinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
            matAdd(e2MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
             
            //cout<<"\ne2MinusOne_Coef_Mat\n";
            //showMatrix(e2MinusOne_Coef_Mat);
            //cout<<"\ne2MinusOne_Const_Mat\n";
            //showMatrix(e2MinusOne_Const_Mat);
                
            
            Matrix e2PlusAIe1_Coef_Mat, e2PlusAIe1_Const_Mat;
            // Finding e2+AI(e1)
            copyMatrix(AIe1_Coef_Mat, e2PlusAIe1_Coef_Mat);
            copyMatrix(AIe1_Const_Mat, e2PlusAIe1_Const_Mat);
            matAdd(e2PlusAIe1_Coef_Mat, e2_Coef_Mat, ModOfVarToElim);
            matAdd(e2PlusAIe1_Const_Mat, e2_Const_Mat, ModOfVarToElim);
            
            
            //cout<<"\ne2PlusAIe1_Coef_Mat\n";
            //showMatrix(e2PlusAIe1_Coef_Mat);
            //cout<<"\ne2PlusAIe1_Const_Mat\n";
            //showMatrix(e2PlusAIe1_Const_Mat);
            
            
            
            Matrix e1PlusAIe2_Coef_Mat, e1PlusAIe2_Const_Mat, AIe1PlusAIe2_Const_Mat;
            // Finding e1+AI(e2)
            copyMatrix(AIe2_Coef_Mat, e1PlusAIe2_Coef_Mat);
            copyMatrix(AIe2_Const_Mat, e1PlusAIe2_Const_Mat);
            matAdd(e1PlusAIe2_Coef_Mat, e1_Coef_Mat, ModOfVarToElim);
            matAdd(e1PlusAIe2_Const_Mat, e1_Const_Mat, ModOfVarToElim);
            
            copyMatrix(e1PlusAIe2_Const_Mat, AIe1PlusAIe2_Const_Mat);
            aInverseOfMatrix(AIe1PlusAIe2_Const_Mat, ModOfVarToElim);
            
            //cout<<"\ne1PlusAIe2_Coef_Mat\n";
            //showMatrix(e1PlusAIe2_Coef_Mat);
            //cout<<"\ne1PlusAIe2_Const_Mat\n";
            //showMatrix(e1PlusAIe2_Const_Mat);
            //cout<<"\nAIe1PlusAIe2_Const_Mat\n";
            //showMatrix(AIe1PlusAIe2_Const_Mat);
            
                      
            
            Matrix e2PlusOne_Coef_Mat, e2PlusOne_Const_Mat;
            // Finding e2+1
            copyMatrix(e2_Coef_Mat, e2PlusOne_Coef_Mat);
            copyMatrix(e2_Const_Mat, e2PlusOne_Const_Mat);
            matAdd(e2PlusOne_Coef_Mat, One_Coef_Mat, ModOfVarToElim);
            matAdd(e2PlusOne_Const_Mat, One_Const_Mat, ModOfVarToElim);
            
            //cout<<"\ne2PlusOne_Coef_Mat\n";
            //showMatrix(e2PlusOne_Coef_Mat);
            //cout<<"\ne2PlusOne_Const_Mat\n";
            //showMatrix(e2PlusOne_Const_Mat);
            
            
            Matrix AIe2MinusOne_Coef_Mat, AIe2MinusOne_Const_Mat, AIAIe2MinusOne_Const_Mat;
            // Finding AI(e2)-1
            copyMatrix(AIe2_Coef_Mat, AIe2MinusOne_Coef_Mat);
            copyMatrix(AIe2_Const_Mat, AIe2MinusOne_Const_Mat);
            matAdd(AIe2MinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
            matAdd(AIe2MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim);
            
            copyMatrix(AIe2MinusOne_Const_Mat, AIAIe2MinusOne_Const_Mat);
            aInverseOfMatrix(AIAIe2MinusOne_Const_Mat, ModOfVarToElim);
            
            //cout<<"\nAIe2MinusOne_Coef_Mat\n";
            //showMatrix(AIe2MinusOne_Coef_Mat);
            //cout<<"\nAIe2MinusOne_Const_Mat\n";
            //showMatrix(AIe2MinusOne_Const_Mat);
            //cout<<"\nAIAIe2MinusOne_Const_Mat\n";
            //showMatrix(AIAIe2MinusOne_Const_Mat);
            
            
            DAGNode *e2EqZero, *kxEqZero, *e1Ltee2MinusOne, *AIe2MinusOneNeqZero;
            DAGNode *e1PlusAIe2EqZero, *kxGtee2PlusOne, *e2PlusAIe1Gtekx;
            DAGNode *AIe2MinusOneEqZero;
            
               
            vector<string> LMITypesBVUGE;
            LMITypesBVUGE.push_back("bvuge");
            vector<string> LMITypesBVULE;
            LMITypesBVULE.push_back("bvule");
    
    
            e2EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(e2_Coef_Mat, AIe2_Const_Mat, VarNames, ModOfVarToElim));
            //cout<<"\ne2EqZero = "<<getDAGStr(e2EqZero)<<endl;
    
            kxEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(K_Coef_Mat, K_Const_Mat, VarNames, ModOfVarToElim));
            //cout<<"\nkxEqZero = "<<getDAGStr(kxEqZero)<<endl; //K_Const_Mat is NULL always
    
            e1Ltee2MinusOne = quickSimplify_WithLMISupport(getDAGFromInequations(e1_Coef_Mat, e1_Const_Mat, e2MinusOne_Coef_Mat, e2MinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
            //cout<<"\ne1Ltee2MinusOne = "<<getDAGStr(e1Ltee2MinusOne)<<endl;
            
            AIe2MinusOneEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(AIe2MinusOne_Coef_Mat, AIAIe2MinusOne_Const_Mat, VarNames, ModOfVarToElim));
            
            AIe2MinusOneNeqZero = createDAG("not", AIe2MinusOneEqZero, LOGICAL_SYMBOL, 1);
            //cout<<"\nAIe2MinusOneNeqZero = "<<getDAGStr(AIe2MinusOneNeqZero)<<endl;
    
            e1PlusAIe2EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(e1PlusAIe2_Coef_Mat, AIe1PlusAIe2_Const_Mat, VarNames, ModOfVarToElim));
            //cout<<"\ne1PlusAIe2EqZero = "<<getDAGStr(e1PlusAIe2EqZero)<<endl;
            
            kxGtee2PlusOne = quickSimplify_WithLMISupport(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, e2PlusOne_Coef_Mat, e2PlusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
            //cout<<"\nkxGtee2PlusOne = "<<getDAGStr(kxGtee2PlusOne)<<endl;
            
            e2PlusAIe1Gtekx = quickSimplify_WithLMISupport(getDAGFromInequations(e2PlusAIe1_Coef_Mat, e2PlusAIe1_Const_Mat, K_Coef_Mat, K_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
            //cout<<"\ne2PlusAIe1Gtekx = "<<getDAGStr(e2PlusAIe1Gtekx)<<endl;
            
            vector<DAGNode*> Constraint1_Vector;
            Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(kxEqZero);
            Constraint1_Vector.push_back(e1Ltee2MinusOne);     
            DAGNode* Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(e1Ltee2MinusOne);     
            Constraint1_Vector.push_back(AIe2MinusOneNeqZero);     
            Constraint1_Vector.push_back(kxGtee2PlusOne);     
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(e2EqZero);
            Constraint1_Vector.push_back(e1PlusAIe2EqZero);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(e2EqZero);
            Constraint1_Vector.push_back(createDAG("not", kxEqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(e2PlusAIe1Gtekx);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(e1Ltee2MinusOne);
            Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(e1PlusAIe2EqZero);     
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear(); 
            Constraint1_Vector.push_back(e1Ltee2MinusOne);
            Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));   
            Constraint1_Vector.push_back(createDAG("not", kxEqZero, LOGICAL_SYMBOL, 1));   
            Constraint1_Vector.push_back(e2PlusAIe1Gtekx);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear(); 
            Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));   
            Constraint1_Vector.push_back(kxEqZero);   
            Constraint1_Vector.push_back(e1PlusAIe2EqZero);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear(); 
            Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));   
            Constraint1_Vector.push_back(AIe2MinusOneNeqZero);   
            Constraint1_Vector.push_back(kxGtee2PlusOne);
            Constraint1_Vector.push_back(e1PlusAIe2EqZero);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);
            
            
            Constraint1_Vector.clear(); 
            Constraint1_Vector.push_back(createDAG("not", e2EqZero, LOGICAL_SYMBOL, 1));   
            Constraint1_Vector.push_back(AIe2MinusOneNeqZero);   
            Constraint1_Vector.push_back(kxGtee2PlusOne);
            Constraint1_Vector.push_back(createDAG("not", kxEqZero, LOGICAL_SYMBOL, 1));
            Constraint1_Vector.push_back(e2PlusAIe1Gtekx);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);           
            
           
            if(order_lmes_before_lmis_in_normalization)
            {
                pushBackIntoAPList(APListForNormalizedDD, e2EqZero);
                pushBackIntoAPList(APListForNormalizedDD, e1PlusAIe2EqZero);
                pushBackIntoAPList(APListForNormalizedDD, kxEqZero);
                pushBackIntoAPList(APListForNormalizedDD, AIe2MinusOneEqZero);
                pushBackIntoAPList(APListForNormalizedDD, e1Ltee2MinusOne);
                pushBackIntoAPList(APListForNormalizedDD, kxGtee2PlusOne);
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1Gtekx);
            }//if(order_lmes_before_lmis_in_normalization)
            else if(order_lmcs_for_small_lmdds_in_normalization)
            {
                pushBackIntoAPList(APListForNormalizedDD, e2EqZero);
                pushBackIntoAPList(APListForNormalizedDD, e1Ltee2MinusOne);
                pushBackIntoAPList(APListForNormalizedDD, kxEqZero);
                pushBackIntoAPList(APListForNormalizedDD, AIe2MinusOneEqZero);
                pushBackIntoAPList(APListForNormalizedDD, kxGtee2PlusOne);
                pushBackIntoAPList(APListForNormalizedDD, e1PlusAIe2EqZero);               
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1Gtekx); 
            }//if(order_lmcs_for_small_lmdds_in_normalization)
            
        }         
        else
        {
             cout<<"\nIllegal value "<<side_of_variable<<" for side_of_variable in function normalizeConstraintsWithVariableOnOneSide in Eliminator.cpp\n";
             exit(1);
        }
    }
    
     //showAPVector("SplittedVector", SplittedVector);   
   
}// end of function


// Normalize LMDs of the form kx+e_1 \neq e_2 and -kx+e_1 \neq e_2
void normalizeLMDs(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector, list<DAGNode*> &APListForNormalizedDD)
{    
    list<string> VarNames;
    VarNames.push_back(VarToElim);
    set<string> Support;
    getDAGNames(Constraint, Support);
    attachSetToList(Support, VarNames);
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *S = *iout;
    iout++;
    DAGNode *T = *iout;	
    
    getCoefsAndConstsModified(S, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(T, VarNames, T_Coef_Mat, T_Const_Mat);
        
     
    Matrix One_Coef_Mat, One_Const_Mat;
            
    getNullMatrix(One_Coef_Mat, 1, VarNames.size());
    getNullMatrix(One_Const_Mat, 1, 1);
    One_Const_Mat[0][0]=1;

    Matrix MinusOne_Coef_Mat, MinusOne_Const_Mat;

    getNullMatrix(MinusOne_Coef_Mat, 1, VarNames.size());
    getNullMatrix(MinusOne_Const_Mat, 1, 1);
    MinusOne_Const_Mat[0][0]=ModOfVarToElim-1;

        
    Matrix e1_Coef_Mat, e1_Const_Mat, AIe1_Coef_Mat, AIe1_Const_Mat;
    Matrix e2_Coef_Mat, e2_Const_Mat, AIe2_Coef_Mat, AIe2_Const_Mat;
    Matrix K_Coef_Mat, K_Const_Mat;
    
    if(type_of_coefficient == "original")
    {    
       copyMatrix(S_Coef_Mat, K_Coef_Mat);
       nullifyColumnsExceptOne(K_Coef_Mat, 0);
       getNullMatrix(K_Const_Mat, 1, 1);
       copyMatrix(S_Coef_Mat, e1_Coef_Mat);
       copyMatrix(S_Const_Mat, e1_Const_Mat);
       nullifyColumn(e1_Coef_Mat, 0);
       
        copyMatrix(T_Coef_Mat, e2_Coef_Mat);
        copyMatrix(T_Const_Mat, e2_Const_Mat);      
    }
    else
    {
       copyMatrix(S_Coef_Mat, K_Coef_Mat);
       nullifyColumnsExceptOne(K_Coef_Mat, 0);
       getNullMatrix(K_Const_Mat, 1, 1);
       copyMatrix(S_Coef_Mat, e2_Coef_Mat);
       copyMatrix(S_Const_Mat, e2_Const_Mat);
       nullifyColumn(e2_Coef_Mat, 0);
       
       copyMatrix(T_Coef_Mat, e1_Coef_Mat);
       copyMatrix(T_Const_Mat, e1_Const_Mat);  
       
       aInverseOfMatrix(K_Coef_Mat, ModOfVarToElim);
       aInverseOfMatrix(K_Const_Mat, ModOfVarToElim);
    }

   copyMatrix(e1_Coef_Mat, AIe1_Coef_Mat);
   copyMatrix(e1_Const_Mat, AIe1_Const_Mat);
   aInverseOfMatrix(AIe1_Coef_Mat, ModOfVarToElim);
   aInverseOfMatrix(AIe1_Const_Mat, ModOfVarToElim);



    Matrix e2PlusAIe1_Coef_Mat, e2PlusAIe1_Const_Mat;
    // Finding e2+AI(e1)
    copyMatrix(AIe1_Coef_Mat, e2PlusAIe1_Coef_Mat);
    copyMatrix(AIe1_Const_Mat, e2PlusAIe1_Const_Mat);
    matAdd(e2PlusAIe1_Coef_Mat, e2_Coef_Mat, ModOfVarToElim);
    matAdd(e2PlusAIe1_Const_Mat, e2_Const_Mat, ModOfVarToElim);

    Matrix e2PlusAIe1PlusOne_Coef_Mat, e2PlusAIe1PlusOne_Const_Mat;
    Matrix e2PlusAIe1MinusOne_Coef_Mat, e2PlusAIe1MinusOne_Const_Mat;
    // Finding e2+AI(e1)+1, e2+AI(e1)-1
    copyMatrix(e2PlusAIe1_Coef_Mat, e2PlusAIe1PlusOne_Coef_Mat);
    copyMatrix(e2PlusAIe1_Const_Mat, e2PlusAIe1PlusOne_Const_Mat);
    matAdd(e2PlusAIe1PlusOne_Coef_Mat, One_Coef_Mat, ModOfVarToElim);
    matAdd(e2PlusAIe1PlusOne_Const_Mat, One_Const_Mat, ModOfVarToElim);  

    copyMatrix(e2PlusAIe1_Coef_Mat, e2PlusAIe1MinusOne_Coef_Mat);
    copyMatrix(e2PlusAIe1_Const_Mat, e2PlusAIe1MinusOne_Const_Mat);
    matAdd(e2PlusAIe1MinusOne_Coef_Mat, MinusOne_Coef_Mat, ModOfVarToElim);
    matAdd(e2PlusAIe1MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim); 

    Matrix AIe2PlusAIe1_Const_Mat, AIe2PlusAIe1MinusOne_Const_Mat;

    copyMatrix(e2PlusAIe1_Const_Mat, AIe2PlusAIe1_Const_Mat);
    aInverseOfMatrix(AIe2PlusAIe1_Const_Mat, ModOfVarToElim);

    copyMatrix(AIe2PlusAIe1_Const_Mat, AIe2PlusAIe1MinusOne_Const_Mat);
    matAdd(AIe2PlusAIe1MinusOne_Const_Mat, MinusOne_Const_Mat, ModOfVarToElim); 



    vector<string> LMITypesBVULE;
    LMITypesBVULE.push_back("bvule");
    vector<string> LMITypesBVUGE;
    LMITypesBVUGE.push_back("bvuge");

    DAGNode* kxLtee2PlusAIe1MinusOne = quickSimplify_WithLMISupport(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, e2PlusAIe1MinusOne_Coef_Mat, e2PlusAIe1MinusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVULE));
    //cout<<"\nkxLtee2PlusAIe1MinusOne = "<<getDAGStr(kxLtee2PlusAIe1MinusOne)<<endl;
    DAGNode* kxGtee2PlusAIe1PlusOne = quickSimplify_WithLMISupport(getDAGFromInequations(K_Coef_Mat, K_Const_Mat, e2PlusAIe1PlusOne_Coef_Mat, e2PlusAIe1PlusOne_Const_Mat, VarNames, ModOfVarToElim, LMITypesBVUGE));
    //cout<<"\nkxGtee2PlusAIe1PlusOne = "<<getDAGStr(kxGtee2PlusAIe1PlusOne)<<endl;

    DAGNode* e2PlusAIe1EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(e2PlusAIe1_Coef_Mat, AIe2PlusAIe1_Const_Mat, VarNames, ModOfVarToElim));
    DAGNode* e2PlusAIe1NeqZero = quickSimplify_WithLMISupport(createDAG("not", e2PlusAIe1EqZero, LOGICAL_SYMBOL, 1));
    //cout<<"\ne2PlusAIe1NeqZero = "<<getDAGStr(e2PlusAIe1NeqZero)<<endl;                
    
    DAGNode* e2PlusAIe1EqMinusOne = quickSimplify_WithLMISupport(getDAGFromEqs(e2PlusAIe1_Coef_Mat, AIe2PlusAIe1MinusOne_Const_Mat, VarNames, ModOfVarToElim));
    DAGNode* e2PlusAIe1NeqMinusOne =  quickSimplify_WithLMISupport(createDAG("not",  e2PlusAIe1EqMinusOne, LOGICAL_SYMBOL, 1));
    //cout<<"\ne2PlusAIe1NeqMinusOne = "<<getDAGStr(e2PlusAIe1NeqMinusOne)<<endl;      

    DAGNode* RecreatedConstraint1 = quickSimplify_WithLMISupport(createDAG("and", e2PlusAIe1NeqZero, kxLtee2PlusAIe1MinusOne,LOGICAL_SYMBOL, 1));
    DAGNode* RecreatedConstraint2 = quickSimplify_WithLMISupport(createDAG("and", e2PlusAIe1NeqMinusOne, kxGtee2PlusAIe1PlusOne,LOGICAL_SYMBOL, 1));

    SplittedVector.push_back(RecreatedConstraint1);
    SplittedVector.push_back(RecreatedConstraint2);
    
     if(order_lmes_before_lmis_in_normalization)
            {
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1EqZero);
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1EqMinusOne);
                pushBackIntoAPList(APListForNormalizedDD, kxLtee2PlusAIe1MinusOne);
                pushBackIntoAPList(APListForNormalizedDD, kxGtee2PlusAIe1PlusOne);                
            }//if(order_lmes_before_lmis_in_normalization)
     else if(order_lmcs_for_small_lmdds_in_normalization)
            {
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1EqZero);
                pushBackIntoAPList(APListForNormalizedDD, kxLtee2PlusAIe1MinusOne);
                pushBackIntoAPList(APListForNormalizedDD, e2PlusAIe1EqMinusOne);                
                pushBackIntoAPList(APListForNormalizedDD, kxGtee2PlusAIe1PlusOne);                
            }//if(order_lmcs_for_small_lmdds_in_normalization)
    
    //showAPVector("SplittedVector", SplittedVector);   
    //char c = getchar();
    
}//end of function



// Normalize the constraints of the form ((+/-).kx+e_1 <= (+/-).kx+e_2)
void normalizeConstraintsWithVariableOnBothSides(DAGNode *Constraint, string VarToElim, map<string, int> &WidthTable, string side_of_variable, string type_of_coefficient,  ULINT &ModOfVarToElim, vector<DAGNode*> &SplittedVector, list<DAGNode*> &APListForNormalizedDD)
{    
    list<string> VarNames;
    VarNames.push_back(VarToElim);
    set<string> Support;
    getDAGNames(Constraint, Support);
    attachSetToList(Support, VarNames);
    
    //cout<<"\nConstraint = "<<getDAGStr(Constraint)<<endl;
    
    Matrix S_Coef_Mat, S_Const_Mat, T_Coef_Mat, T_Const_Mat;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *S = *iout;
    iout++;
    DAGNode *T = *iout;	
    
    getCoefsAndConstsModified(S, VarNames, S_Coef_Mat, S_Const_Mat);
    getCoefsAndConstsModified(T, VarNames, T_Coef_Mat, T_Const_Mat);
    
    //showList(VarNames, "VarNames");
    //cout<<"\nS_Coef_Mat\n";
    //showMatrix(S_Coef_Mat);
    //cout<<"\nS_Const_Mat\n";
    //showMatrix(S_Const_Mat);
    //cout<<"\nT_Coef_Mat\n";
    //showMatrix(T_Coef_Mat);
    //cout<<"\nT_Const_Mat\n";
    //showMatrix(T_Const_Mat);
    
    Matrix E_Coef_Mat, E_Const_Mat, F_Coef_Mat, F_Const_Mat;
    Matrix Var_Coef_Mat, Var_Const_Mat;
    ULINT E, F;
    
    copyMatrix(S_Coef_Mat, E_Coef_Mat);
    copyMatrix(S_Const_Mat, E_Const_Mat);
    nullifyColumn(E_Coef_Mat, 0);
    E = E_Const_Mat[0][0];
    
    copyMatrix(T_Coef_Mat, F_Coef_Mat);
    copyMatrix(T_Const_Mat, F_Const_Mat);
    nullifyColumn(F_Coef_Mat, 0);
    F = F_Const_Mat[0][0];
    
    copyMatrix(S_Coef_Mat, Var_Coef_Mat);
    nullifyColumnsExceptOne(Var_Coef_Mat, 0);
    getNullMatrix(Var_Const_Mat, 1, 1);
    
       
    //cout<<"\nE_Coef_Mat\n";
    //showMatrix(E_Coef_Mat);
    //cout<<"\nE_Const_Mat\n";
    //showMatrix(E_Const_Mat);
    
    //cout<<"\nF_Coef_Mat\n";
    //showMatrix(F_Coef_Mat);
    //cout<<"\nF_Const_Mat\n";
    //showMatrix(F_Const_Mat);
    
    //cout<<"\nVar_Coef_Mat\n";
    //showMatrix(Var_Coef_Mat);
    //cout<<"\nVar_Const_Mat\n";
    //showMatrix(Var_Const_Mat);
    
    
    if(IsNullMatrix(E_Coef_Mat)==1 && IsNullMatrix(F_Coef_Mat)==1)
    {
        //cout<<"\nDifference between the LHS and RHS of "<<getDAGStr(Constraint)<<" is a constant\n";
        
          if(type_of_coefficient=="original")
                {
                if(E == F)
                    {
                    //cout<<"\nNo need to reconstruct "<<getDAGStr(Constraint)<<" as both the LHS and RHS are the same in function normalizeConstraintsWithVariableOnBothSides in DAGEliminator.cpp\n";
                    //cout<<"The result is True\n";
                    }
                else if(E == 0)
                    {
                    ULINT AI_of_F = aInverse(F, ModOfVarToElim);
                    ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
                    Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
                    getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
                    getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
                    AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
                    
                    vector<string> LMITypes;
                    LMITypes.push_back("bvule");
                    DAGNode* ReconstructedConstraint = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                    SplittedVector.push_back(ReconstructedConstraint); 
                    
                    if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        {
                        pushBackIntoAPList(APListForNormalizedDD, ReconstructedConstraint);               
                        }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                    
                    }
                else if(F == 0)
                    {
                    ULINT AI_of_E = aInverse(E, ModOfVarToElim);
                    Matrix AI_of_E_Coef_Mat, AI_of_E_Const_Mat;
                    getNullMatrix(AI_of_E_Coef_Mat, 1, VarNames.size());
                    getNullMatrix(AI_of_E_Const_Mat, 1, 1);
                    AI_of_E_Const_Mat[0][0] = AI_of_E;
                    
                    vector<string> LMITypes;
                    LMITypes.push_back("bvuge");
                    DAGNode* ReconstructedConstraint = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_E_Coef_Mat, AI_of_E_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                    SplittedVector.push_back(ReconstructedConstraint);  
                    
                    if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        {
                        pushBackIntoAPList(APListForNormalizedDD, ReconstructedConstraint);               
                        }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                    
                    }
                else if(E < F)
                    {
                    ULINT AI_of_F = aInverse(F, ModOfVarToElim);
                    ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
                    Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
                    getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
                    getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
                    AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
                    
                    ULINT AI_of_E = aInverse(E, ModOfVarToElim);
                    Matrix AI_of_E_Coef_Mat, AI_of_E_Const_Mat;
                    getNullMatrix(AI_of_E_Coef_Mat, 1, VarNames.size());
                    getNullMatrix(AI_of_E_Const_Mat, 1, 1);
                    AI_of_E_Const_Mat[0][0] = AI_of_E;
                    
                    
                    vector<string> LMITypes;
                    LMITypes.push_back("bvule");
                    DAGNode* ReconstructedConstraint1 = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                    SplittedVector.push_back(ReconstructedConstraint1);   
                    
                    LMITypes.clear();
                    LMITypes.push_back("bvuge");
                    DAGNode* ReconstructedConstraint2 = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_E_Coef_Mat, AI_of_E_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                    SplittedVector.push_back(ReconstructedConstraint2);   
                    
                    
                    if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        {
                        pushBackIntoAPList(APListForNormalizedDD, ReconstructedConstraint1);   
                        pushBackIntoAPList(APListForNormalizedDD, ReconstructedConstraint2); 
                        }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                    }
                else
                    {
                    ULINT AI_of_F = aInverse(F, ModOfVarToElim);
                    ULINT AI_of_F_Minus_One = add(AI_of_F, ModOfVarToElim-1, ModOfVarToElim);
                    Matrix AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat;
                    getNullMatrix(AI_of_F_Minus_One_Coef_Mat, 1, VarNames.size());
                    getNullMatrix(AI_of_F_Minus_One_Const_Mat, 1, 1);
                    AI_of_F_Minus_One_Const_Mat[0][0] = AI_of_F_Minus_One;
                    
                    ULINT AI_of_E = aInverse(E, ModOfVarToElim);
                    Matrix AI_of_E_Coef_Mat, AI_of_E_Const_Mat;
                    getNullMatrix(AI_of_E_Coef_Mat, 1, VarNames.size());
                    getNullMatrix(AI_of_E_Const_Mat, 1, 1);
                    AI_of_E_Const_Mat[0][0] = AI_of_E;
                    
                    
                    vector<string> LMITypes;
                    LMITypes.push_back("bvule");
                    DAGNode* ReconstructedConstraint1 = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_F_Minus_One_Coef_Mat, AI_of_F_Minus_One_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                                        
                    LMITypes.clear();
                    LMITypes.push_back("bvuge");
                    DAGNode* ReconstructedConstraint2 = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, AI_of_E_Coef_Mat, AI_of_E_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                    
                    DAGNode* ReconstructedConstraint=createDAG("and", ReconstructedConstraint1, ReconstructedConstraint2, LOGICAL_SYMBOL, 1);
                    SplittedVector.push_back(ReconstructedConstraint);   
                    
                     if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        {
                        pushBackIntoAPList(APListForNormalizedDD, ReconstructedConstraint1);   
                        pushBackIntoAPList(APListForNormalizedDD, ReconstructedConstraint2); 
                        }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                    
                    }
          }// if(type_of_coefficient=="original")
          else if(type_of_coefficient=="negated")
          {
              aInverseOfMatrix(Var_Coef_Mat, ModOfVarToElim);
              aInverseOfMatrix(Var_Const_Mat, ModOfVarToElim);
           
               if(E == F)
                    {
                    //cout<<"\nNo need to reconstruct "<<getDAGStr(Constraint)<<" as both the LHS and RHS are the same in function normalizeConstraintsWithVariableOnBothSides in DAGEliminator.cpp\n";
                    //cout<<"The result is True\n";
                    }// if(E == F)
                else if(E == 0)
                    {
                    ULINT F_Plus_One = add(F, 1, ModOfVarToElim);
                    
                    if(F_Plus_One == 0)
                        {
                        DAGNode* kxEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim));
                        //Var_Const_Mat is NULL always
                        SplittedVector.push_back(kxEqZero); 
                        
                        if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                            {
                            pushBackIntoAPList(APListForNormalizedDD, kxEqZero);               
                            }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        }
                    else
                        {
                        Matrix F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat;
                        getNullMatrix(F_Plus_One_Coef_Mat, 1, VarNames.size());
                        getNullMatrix(F_Plus_One_Const_Mat, 1, 1);
                        F_Plus_One_Const_Mat[0][0] = F_Plus_One;
                        
                        DAGNode* kxEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim));
                        //Var_Const_Mat is NULL always
                    
                        vector<string> LMITypes;
                        LMITypes.push_back("bvuge");
                        DAGNode* kxGteFPlusOne = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                        
                        SplittedVector.push_back(kxEqZero); 
                        SplittedVector.push_back(kxGteFPlusOne);   
                        
                        
                        if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                            {
                            pushBackIntoAPList(APListForNormalizedDD, kxEqZero);  
                            pushBackIntoAPList(APListForNormalizedDD, kxGteFPlusOne);   
                            }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        }
                    }//else if(E == 0)
                else if(F == 0)
                    {
                                   
                     DAGNode* kxEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim));
                     //Var_Const_Mat is NULL always
                    
                     vector<string> LMITypes;
                     LMITypes.push_back("bvule");
                     DAGNode* kxLteE = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, E_Coef_Mat, E_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                        
                     DAGNode* ReconstructedConstraint = quickSimplify_WithLMISupport(createDAG("and", kxLteE, createDAG("not", kxEqZero,LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1));
                      
                     SplittedVector.push_back(ReconstructedConstraint); 
                     
                     if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                            {
                            pushBackIntoAPList(APListForNormalizedDD, kxEqZero);   
                            pushBackIntoAPList(APListForNormalizedDD, kxLteE); 
                            }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                    }//else if(F == 0)
                else if(E < F)
                    {
                    ULINT F_Plus_One = add(F, 1, ModOfVarToElim);
                    
                    if(F_Plus_One == 0)
                        {
                         DAGNode* kxEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim));
                        //Var_Const_Mat is NULL always
                    
                        vector<string> LMITypes;
                        LMITypes.push_back("bvule");
                        DAGNode* kxLteE = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, E_Coef_Mat, E_Const_Mat,VarNames, ModOfVarToElim, LMITypes));
                        
                        DAGNode* ReconstructedConstraint1 = kxEqZero;
                        DAGNode* ReconstructedConstraint2 = quickSimplify_WithLMISupport(createDAG("and", kxLteE, createDAG("not", kxEqZero,LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1));
                      
                        SplittedVector.push_back(ReconstructedConstraint1);  
                        SplittedVector.push_back(ReconstructedConstraint2); 
                        
                        if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                            {
                            pushBackIntoAPList(APListForNormalizedDD, kxEqZero);   
                            pushBackIntoAPList(APListForNormalizedDD, kxLteE); 
                            }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        }
                    else
                        {
                        Matrix F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat;
                        getNullMatrix(F_Plus_One_Coef_Mat, 1, VarNames.size());
                        getNullMatrix(F_Plus_One_Const_Mat, 1, 1);
                        F_Plus_One_Const_Mat[0][0] = F_Plus_One;
                        
                        DAGNode* kxEqZero = quickSimplify_WithLMISupport(getDAGFromEqs(Var_Coef_Mat, Var_Const_Mat, VarNames, ModOfVarToElim));
                        //Var_Const_Mat is NULL always
                    
                        vector<string> LMITypesBVUGE;
                        LMITypesBVUGE.push_back("bvuge");
                        DAGNode* kxGteFPlusOne = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat,VarNames, ModOfVarToElim, LMITypesBVUGE));
                        
                        vector<string> LMITypesBVULE;
                        LMITypesBVULE.push_back("bvule");
                        DAGNode* kxLteE = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, E_Coef_Mat, E_Const_Mat,VarNames, ModOfVarToElim, LMITypesBVULE));
                      
                        DAGNode* ReconstructedConstraint1 = kxEqZero;
                        DAGNode* ReconstructedConstraint2 = quickSimplify_WithLMISupport(createDAG("and", kxLteE, createDAG("not", kxEqZero,LOGICAL_SYMBOL, 1), LOGICAL_SYMBOL, 1));
                        DAGNode* ReconstructedConstraint3 = kxGteFPlusOne;
                        
                        SplittedVector.push_back(ReconstructedConstraint1);  
                        SplittedVector.push_back(ReconstructedConstraint2); 
                        SplittedVector.push_back(ReconstructedConstraint3); 
                        
                        if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                            {
                            pushBackIntoAPList(APListForNormalizedDD, kxEqZero);   
                            pushBackIntoAPList(APListForNormalizedDD, kxLteE); 
                            pushBackIntoAPList(APListForNormalizedDD, kxGteFPlusOne); 
                            }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        
                        }
                    }
                else
                    {
                     ULINT F_Plus_One = add(F, 1, ModOfVarToElim);
                    
                    if(F_Plus_One == 0)
                        {
                         cout<<"\nThe constraint "<<getDAGStr(Constraint)<<" is False in function normalizeConstraintsWithVariableOnBothSides  in Eliminator.cpp\n";
                         exit(1);
                        }
                    else
                        {
                        Matrix F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat;
                        getNullMatrix(F_Plus_One_Coef_Mat, 1, VarNames.size());
                        getNullMatrix(F_Plus_One_Const_Mat, 1, 1);
                        F_Plus_One_Const_Mat[0][0] = F_Plus_One;
                        
                        vector<string> LMITypesBVUGE;
                        LMITypesBVUGE.push_back("bvuge");
                        DAGNode* kxGteFPlusOne = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, F_Plus_One_Coef_Mat, F_Plus_One_Const_Mat,VarNames, ModOfVarToElim, LMITypesBVUGE));
                        
                        vector<string> LMITypesBVULE;
                        LMITypesBVULE.push_back("bvule");
                        DAGNode* kxLteE = quickSimplify_WithLMISupport(getDAGFromInequations(Var_Coef_Mat, Var_Const_Mat, E_Coef_Mat, E_Const_Mat,VarNames, ModOfVarToElim, LMITypesBVULE));
                      
                        DAGNode* ReconstructedConstraint = quickSimplify_WithLMISupport(createDAG("and", kxLteE, kxGteFPlusOne, LOGICAL_SYMBOL, 1));
                         
                        SplittedVector.push_back(ReconstructedConstraint); 
                        
                         if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                            {
                            pushBackIntoAPList(APListForNormalizedDD, kxLteE);  
                            pushBackIntoAPList(APListForNormalizedDD, kxGteFPlusOne);   
                            }//if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                        
                        }
                    }
              
          }// else if(type_of_coefficient=="negated")
          else
          {
           cout<<"\nUnknown value "<<type_of_coefficient<<" for type_of_coefficient in function normalizeConstraintsWithVariableOnBothSides in Eliminator.cpp\n";
           exit(1); 
          }
    }// if(IsNullMatrix(E_Coef_Mat)==1 && IsNullMatrix(F_Coef_Mat)==1)
    else
    {
      cout<<"\nDifference between the LHS and RHS of "<<getDAGStr(Constraint)<<" is not a constant  in function normalizeConstraintsWithVariableOnBothSides in Eliminator.cpp\n";
      exit(1);
    }
    //showAPVector("SplittedVector", SplittedVector);
}//end of function




// Find the side in which variable appears in Constraint
// It can be left, right, none, or both
string findSideOfVariableInConstraint(DAGNode* Constraint, string VarToElim)
{
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
    
    bool left=false, right=false;
    
    set<string> LHS_Support;
    set<string> RHS_Support;
    
    getDAGNames(LHS, LHS_Support);
    getDAGNames(RHS, RHS_Support);
    
    if(LHS_Support.find(VarToElim) != LHS_Support.end())// VarToElim appears on LHS
    {
        left = true;
    }
    
    if(RHS_Support.find(VarToElim) != RHS_Support.end())// VarToElim appears on RHS
    {
        right = true;
    } 
    
    
    if(left && !right)
    {
        return "left";
    }
    if(!left && right)
    {
        return "right";
    }
    if(!left && !right)
    {
        return "none";
    }
    if(left && right)
    {
        return "both";
    }
}


// Find the coefficient of the VarToElim in Constraint, and its additive inverse
void getCoefOfVarToElimAndAIOfCoefOfVarToElim(DAGNode *Constraint, string VarToElim, string side_of_variable, ULINT ModOfVarToElim, ULINT &CoefOfVarToElim, ULINT &AIOfCoefOfVarToElim)
{
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
    
    if(side_of_variable == "left")
    {
        getCoefficientCorrected(LHS, VarToElim, CoefOfVarToElim);
    }
    else if(side_of_variable == "right")
    {
        getCoefficientCorrected(RHS, VarToElim, CoefOfVarToElim);
    }
    else if(side_of_variable == "both")
    {
        ULINT CoefficientOnLHS, CoefficientOnRHS;
        getCoefficientCorrected(LHS, VarToElim, CoefficientOnLHS);
        getCoefficientCorrected(RHS, VarToElim, CoefficientOnRHS);
        
        if(CoefficientOnLHS != CoefficientOnRHS)
        {
            cout<<"\nError in function getCoefOfVarToElimAndAIOfCoefOfVarToElim in Eliminator.cpp. Variable "<<VarToElim<<" appears in "<<getDAGStr(Constraint)<<" with different coefficients on different sides\n";
            exit(1);
        }
        
       CoefOfVarToElim =  CoefficientOnLHS;
    }
    else
    {
        cout<<"\nUnexpected side_of_variable "<<side_of_variable<<" in function getCoefOfVarToElimAndAIOfCoefOfVarToElim in Eliminator.cpp\n";
        exit(1);
    }
 
    AIOfCoefOfVarToElim = aInverse(CoefOfVarToElim, ModOfVarToElim);
    
    //cout<<"\nCoefOfVarToElim = "<<CoefOfVarToElim<<", AIOfCoefOfVarToElim = "<<AIOfCoefOfVarToElim<<endl;
}


// Checks if the coefficient of the variable to be  eliminated is the same/additive inverse
// of the coefficients which previously appeared. Returns original/negated according to it
string findTypeOfCoefficient(DAGNode* Constraint, string VarToElim, string side_of_variable, ULINT CoefOfVarToElim, ULINT AIOfCoefOfVarToElim)
{
    ULINT MyCoefficient;
    
    list<DAGNode*>::iterator iout;
    iout = Constraint->Get_outlist_start();
    DAGNode *LHS = *iout;
    iout++;
    DAGNode *RHS = *iout;	
    
    if(side_of_variable == "left")
    {
        getCoefficientCorrected(LHS, VarToElim, MyCoefficient);
    }
    else if(side_of_variable == "right")
    {
        getCoefficientCorrected(RHS, VarToElim, MyCoefficient);
    }
    else if(side_of_variable == "both")
    {
        ULINT CoefficientOnLHS, CoefficientOnRHS;
        getCoefficientCorrected(LHS, VarToElim, CoefficientOnLHS);
        getCoefficientCorrected(RHS, VarToElim, CoefficientOnRHS);
        
        if(CoefficientOnLHS != CoefficientOnRHS)
        {
            cout<<"\nError in function findTypeOfCoefficient in Eliminator.cpp. Variable "<<VarToElim<<" appears in "<<getDAGStr(Constraint)<<" with different coefficients on different sides\n";
            exit(1);
        }
        
       MyCoefficient =  CoefficientOnLHS;
    }
    else
    {
        cout<<"\nUnexpected side_of_variable "<<side_of_variable<<" in function findTypeOfCoefficient in Eliminator.cpp\n";
        exit(1);
    } 
    
    //cout<<"\nMyCoefficient = "<<MyCoefficient<<endl;
    
    if(CoefOfVarToElim == MyCoefficient)
    {
        return "original";
    }
    else if(AIOfCoefOfVarToElim == MyCoefficient)
    {
        return "negated";
    }
    else
    {
        cout<<"\nDifferent coefficients for the variable to be eliminated in function findTypeOfCoefficient in Eliminator.cpp\n";
        exit(1);
    }
}



// Returns the disjunction of the constraints in "Constraints"
DAGNode* getDisjunctionFromVector(vector<DAGNode*> &Constraints)
{
  DAGNode *resultDAG=NULL;
  // result is the disjunction of the constraints in Constraints
  for(int i=0; i<Constraints.size(); i++)
    {
      DAGNode* Constraint = Constraints[i];
  
      if(resultDAG==NULL)
	{
	  resultDAG=Constraint;
	}
      else
	{
	  resultDAG = createDAG("or",resultDAG, Constraint, LOGICAL_SYMBOL, 1);
	}
    }
	  
  if(resultDAG == NULL) 
    {
     cout<<"\nConstraints = empty in function getDisjunctionFromVector in Eliminator.cpp\n";
     exit(1);
    }
  return resultDAG;
}



// Returns the conjunction of the constraints in "Constraints"
DAGNode* getConjunctionFromVector(vector<DAGNode*> &Constraints)
{
  DAGNode *resultDAG=NULL;
  // result is the conjunction of the constraints in Constraints
  for(int i=0; i<Constraints.size(); i++)
    {
      DAGNode* Constraint = Constraints[i];
      if(resultDAG==NULL)
	{
	  resultDAG=Constraint;
	}
      else
	{
	  resultDAG = createDAG("and",resultDAG, Constraint, LOGICAL_SYMBOL, 1);
	}
    }
	  
  if(resultDAG == NULL) 
    {
      Value *v = new Value();
      Type *t = new Type();
      t->data_type = LOGICAL_CONSTANT;
      t->width = 1;
      v->set_node_type(*t);
      resultDAG = dm.createNode("true",(void*)v);
    }
  return resultDAG;
}


// Get the children and name of the given node
void getChildrenAndNameOfNode(DAGNode* Node, vector<DAGNode*> &Children, string &Name)
{
    list<DAGNode*>::iterator iout = Node->Get_outlist_start();
    unsigned int outlist_size = Node->Get_outlist_size();
    
    for(unsigned int i=0;i<outlist_size; i++,iout++)
	  {
	    Children.push_back(*iout);
	  }
    
    Name = Node->Get_node_name();
}




// Function to simplify the dag node "node" 
// using the rule t_1 <= c_1 \wedge \neg (t_1 <= c_1-1) ---> t_1 = c_1
DAGNode*  simplifyLMIDag(DAGNode *node, map<string, int> &WidthTable)
{
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  DAGNode *ret = simplifyLMIDagInternal(node, PtrDPTable, WidthTable);
  return ret;
}
  
  

// Function called by simplifyLMIDag 
DAGNode*  simplifyLMIDagInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable, map<string, int> &WidthTable)
{
  DAGNode* ret;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 

  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }
  
  unsigned int outlist_size = node->Get_outlist_size();
  
  if(outlist_size == 0) { // Terminals
	  ret = node;
  }

  else
  {
    string dag_str = node->Get_node_name();
    
    Value* MyValue = (Value*)(node->getValue());
    Type MyType = MyValue->get_node_type();
    int MyWidth = MyType.width;
    int MyDataType = MyType.data_type;
    
    vector<DAGNode*> SimplifiedChildren;
    list<DAGNode*>::iterator iout = node->Get_outlist_start();    

    for(unsigned int i=0;i<outlist_size; i++,iout++)
	  {
	    SimplifiedChildren.push_back(simplifyLMIDagInternal(*iout, DPTable, WidthTable));
	  }   
    
    if(dag_str == "bvule" && SimplifiedChildren[1]->Get_node_name()=="0")
    {
        ret = createDAG("=", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);
    }
    else if(dag_str == "bvuge" && SimplifiedChildren[0]->Get_node_name()=="0")
    {
        ret = createDAG("=", SimplifiedChildren[1], SimplifiedChildren[0], LOGICAL_SYMBOL, 1);
    }
    else if(dag_str == "bvult" && SimplifiedChildren[1]->Get_node_name()=="1")
    {
        int WidthOfConstant = getWidthOfNode(SimplifiedChildren[0], WidthTable);        
        ret = createDAG("=", SimplifiedChildren[0], createBVConstant("0", WidthOfConstant), LOGICAL_SYMBOL, 1);
    }
    else if(dag_str == "bvugt" && SimplifiedChildren[0]->Get_node_name()=="1")
    {
         int WidthOfConstant = getWidthOfNode(SimplifiedChildren[1], WidthTable);        
        ret = createDAG("=", SimplifiedChildren[1], createBVConstant("0", WidthOfConstant), LOGICAL_SYMBOL, 1);
    }
    else  if(dag_str == "bvule" && isConstant(SimplifiedChildren[0]))
    {        
        string ConstantName = SimplifiedChildren[0]->Get_node_name();
         int WidthOfConstant = getWidthOfNode(SimplifiedChildren[1], WidthTable);  
        
        ULINT ConstantValue =  strtoull(ConstantName.c_str(), NULL, 0);
        ULINT MaxPossibleRHS = findPower(WidthOfConstant)-1;
    
        if(ConstantValue == MaxPossibleRHS)
            {
            ret = createDAG("=", SimplifiedChildren[1], SimplifiedChildren[0], LOGICAL_SYMBOL, 1);
            }//if(RHSConstant == MaxPossibleRHS)
        else
            {
             ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
            }
    }
    else if(dag_str == "bvuge" && isConstant(SimplifiedChildren[1]))
    {
        string ConstantName = SimplifiedChildren[1]->Get_node_name();
         int WidthOfConstant = getWidthOfNode(SimplifiedChildren[0], WidthTable);  
        
        ULINT ConstantValue =  strtoull(ConstantName.c_str(), NULL, 0);
        ULINT MaxPossibleRHS = findPower(WidthOfConstant)-1;
    
        if(ConstantValue == MaxPossibleRHS)
            {
            ret = createDAG("=", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);
            }//if(RHSConstant == MaxPossibleRHS)
        else
            {
             ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
            }
    }
    else  if(dag_str == "bvult" && isConstant(SimplifiedChildren[0]))
    {        
        string ConstantName = SimplifiedChildren[0]->Get_node_name();
         int WidthOfConstant = getWidthOfNode(SimplifiedChildren[1], WidthTable);  
        
        ULINT ConstantValue =  strtoull(ConstantName.c_str(), NULL, 0);
        ULINT MaxPossibleRHS = findPower(WidthOfConstant)-1;
        
        char constant[10];
	sprintf(constant, "%llu", MaxPossibleRHS);
	string MaxPossibleRHSName(constant);
    
        if(ConstantValue == MaxPossibleRHS-1)
            {
            ret = createDAG("=", SimplifiedChildren[1],createBVConstant(MaxPossibleRHSName, WidthOfConstant), LOGICAL_SYMBOL, 1);
            }//if(RHSConstant == MaxPossibleRHS)
        else
            {
             ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
            }
    }
    else if(dag_str == "bvugt" && isConstant(SimplifiedChildren[1]))
    {
        string ConstantName = SimplifiedChildren[1]->Get_node_name();
         int WidthOfConstant = getWidthOfNode(SimplifiedChildren[0], WidthTable);  
        
        ULINT ConstantValue =  strtoull(ConstantName.c_str(), NULL, 0);
        ULINT MaxPossibleRHS = findPower(WidthOfConstant)-1;
        
        char constant[10];
	sprintf(constant, "%llu", MaxPossibleRHS);
	string MaxPossibleRHSName(constant);
        
        if(ConstantValue == MaxPossibleRHS-1)
            {
            ret = createDAG("=", SimplifiedChildren[0], createBVConstant(MaxPossibleRHSName, WidthOfConstant), LOGICAL_SYMBOL, 1);
            }//if(RHSConstant == MaxPossibleRHS)
        else
            {
             ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
            }
    }    
    else if(dag_str != "and" || outlist_size != 2)
    {
	//cout<<dag_str<<"node encountered. The nodes down cannot be simplified\n";
	ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
     } 
    else
      {
        vector<DAGNode*> and_Children;
       and_Children.push_back(SimplifiedChildren[0]);
       and_Children.push_back(SimplifiedChildren[1]);
        
        if(and_Children[1]->Get_node_name() != "not" || and_Children[0]->Get_node_name() != "bvule" )
            {
            ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
            }
        else
            {
            //cout<<"\nand_Children[1]->Get_node_name() == not && and_Children[0]->Get_node_name() == bvule\n";
            
            DAGNode* bvule_node = and_Children[0];
            string bvule_Name;
            vector<DAGNode*> bvule_Children;
            getChildrenAndNameOfNode(bvule_node, bvule_Children, bvule_Name);
            
        
            DAGNode* not_node = and_Children[1];
            string not_Name;
            vector<DAGNode*> not_Children;
            getChildrenAndNameOfNode(not_node, not_Children, not_Name);
            
        
            if(not_Children[0]->Get_node_name() != "bvule")
            {
                ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
            }       
            else
            {
                //cout<<"\nnot_Children[0]->Get_node_name() == bvule\n";
                
                DAGNode* bvugt_node = not_Children[0];
                string bvugt_Name;
                vector<DAGNode*> bvugt_Children;
                getChildrenAndNameOfNode(bvugt_node, bvugt_Children, bvugt_Name);
                
                if(  !(bvule_Children[0]==bvugt_Children[0] && isConstant(bvule_Children[1]) && isConstant(bvugt_Children[1]))  )
                {
                    ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
                }
                else
                {
                    //cout<<"\n(bvule_Children[0]==bvugt_Children[0] && isConstant(bvule_Children[1]) && isConstant(bvugt_Children[1]))\n";
                    
                    ULINT bvule_constant = strtoull( ((bvule_Children[1])->Get_node_name()).c_str(), NULL, 0);
                    ULINT bvugt_constant = strtoull( ((bvugt_Children[1])->Get_node_name()).c_str(), NULL, 0);
                    
                    if(bvugt_constant == bvule_constant-1)
                    {
                        //cout<<"\nbvugt_constant == bvule_constant-1\n";
                        //simplified node
                        ret = createDAG("=", bvule_Children[0], bvule_Children[1], LOGICAL_SYMBOL, 1);
                    }
                    else
                    {
                        ret = createDAG(dag_str, SimplifiedChildren, MyDataType, MyWidth);
                    }// else of  if(bvugt_constant == bvule_constant-1)
                }//else of if(  !(bvule_Children[0]==bvugt_Children[0] && isConstant(bvule_Children[1]) && isConstant(bvugt_Children[1]))  )
            }//else of  if(not_Children[0]->Get_node_name() != "bvule")
        }// else of if(and_Children[1]->Get_node_name() != "not" || and_Children[0]->Get_node_name() != "bvule" )    
    }// else of if(dag_str != "and" || outlist_size != 2)  	
  }// non-terminals
  result = DPTable->hashtable_insert(key, ret);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function  simplifyLMIDagInternal\n";
      exit(1);
    }
  return ret;
}


// Push back LMC into APList if (1) LMC is not true/false and (2) LMC is already present
void pushBackIntoAPList(list<DAGNode*> &APList, DAGNode* LMC)
{
    if(!isConstant(LMC) && !searchAPList(APList, LMC))
    {
        APList.push_back(LMC);
    }
}



// Eliminate quantifiers from the conjunction using Fourier-Motzkin style QE where the variable has even coefficient
DAGNode* applyFourierMotzkinForEvenCoefficients(set<DAGNode*> &Constraints, string VarToElim, map<string, int> &WidthTable, list<DAGNode*> &APListForDD)
{
  set<DAGNode*> QE_Constraints;
  vector<DAGNode*> LowerBounds;
  vector<DAGNode*> UpperBounds;
  DAGNode* VarToElimPart = NULL;

   for(set<DAGNode*>::iterator it=Constraints.begin(); it != Constraints.end(); it++)
    {
     DAGNode *Constraint = *it;

     list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

     DAGNode *LHS = *iout;
     iout++;
     DAGNode *RHS = *iout;	       

     set<string> Support_LHS;
     set<string> Support_RHS;

     getDAGNames(LHS, Support_LHS);
     getDAGNames(RHS, Support_RHS);

     if(Support_LHS.find(VarToElim)!= Support_LHS.end())
            {   
            UpperBounds.push_back(RHS);
            if(VarToElimPart == NULL)
                {
                VarToElimPart = LHS;
                }
            }
     else if(Support_RHS.find(VarToElim)!= Support_RHS.end())
            {
            LowerBounds.push_back(LHS);  
             if(VarToElimPart == NULL)
                {
                VarToElimPart = RHS;
                }
            }
    }//for ends here

  for(int i=0; i<LowerBounds.size(); i++)
    {
     for(int j=0; j<UpperBounds.size(); j++)
     {
     DAGNode* L = LowerBounds[i];
     DAGNode* U = UpperBounds[j];

     //cout<<"\nDoing FM on "<<getDAGStr(L)<<" <= "<<getDAGStr(VarToElimPart)<<" <= "<<getDAGStr(U)<<endl;
     
     //char c = getchar();

     DAGNode* FMResult = applyFourierMotzkinForEvenCoefficientsForLMIPairs(L, VarToElimPart, U, VarToElim, WidthTable, APListForDD);
     //cout<<"\nFMResult = "<<getDAGStr(FMResult)<<endl;   
     
     //c = getchar();
     
     QE_Constraints.insert(FMResult);
     }// for j
    }// for i

  if(UpperBounds.size()==0)// All are lower bounds
  {
     for(int i=0; i<LowerBounds.size(); i++)
        {
        DAGNode *L = LowerBounds[i];

        //cout<<"\nDoing FM on "<<getDAGStr(L)<<" <= "<<getDAGStr(VarToElimPart)<<endl;
        
        //char c = getchar();

        DAGNode* FMResult = applyFourierMotzkinForEvenCoefficientsForSingleLMI(L, VarToElimPart, VarToElim, WidthTable, APListForDD);          
        //cout<<"\nFMResult = "<<getDAGStr(FMResult)<<endl;          
        
        //c = getchar();
        
        QE_Constraints.insert(FMResult);
        }  
  }
  else if (LowerBounds.size()==0)// All are upper bounds
    {
    DAGNode* FMResult = createLogicalConstant("true");
    //cout<<"\nFMResult = "<<getDAGStr(FMResult)<<endl;   
    
    //char c = getchar();
    
    QE_Constraints.insert(FMResult);
    }

DAGNode* Result = quickSimplify_WithLMISupport(getConjunctionFromSet(QE_Constraints));    
//cout<<"\nFinal result of FM elimination = "<<getDAGStr(Result)<<endl;

return Result;        

}//function


// Find result of \exists VarToElim. (First_LHS <= First_RHS)
DAGNode* applyFourierMotzkinForEvenCoefficientsForSingleLMI(DAGNode* First_LHS, DAGNode* First_RHS, string VarToElim, map<string, int> &WidthTable, list<DAGNode*> &APListForDD)
{
     set<string> Support_First_LHS;    
     getDAGNames(First_LHS, Support_First_LHS);
     set<string> Support_First_RHS;    
     getDAGNames(First_RHS, Support_First_RHS);
     
     if(Support_First_LHS.find(VarToElim)!= Support_First_LHS.end())//First_LHS is like 2^k.x and First_RHS is like t_1
            {
            return createLogicalConstant("true");
            }
     else if(Support_First_RHS.find(VarToElim)!= Support_First_RHS.end())//First_LHS is like t_1 and First_RHS is like 2^k.x 
            {
            list<string> VarNames;
            VarNames.push_back(VarToElim);
             
            attachSetToList(Support_First_RHS, VarNames);

            Matrix T_Coef_Mat, T_Const_Mat;
            getCoefsAndConstsModified(First_RHS, VarNames, T_Coef_Mat, T_Const_Mat);
              
            ULINT MyCoefficient = T_Coef_Mat[0][0];
            ULINT MyKValue = powerOfTwo(MyCoefficient);
            
            ULINT MyWidth; // Width of variable to be eliminated
            map<string, int>::iterator wit = WidthTable.find(VarToElim);
            if(wit == WidthTable.end())
            {
                cout<<"\nError in applyFourierMotzkinForEvenCoefficientsForSingleLMI in Eliminator.cpp\nNo entry for variable "<<VarToElim<<" in the WidthTable\n";
                exit(1);
            }
            
            MyWidth = wit->second;            
            
            //cout<<"\nMyKValue = "<<MyKValue<<endl;
            //cout<<"\nMyWidth = "<<MyWidth<<endl;            
            
            ULINT DiffCoef = findPower(MyWidth) - findPower(MyKValue);
            char DiffCoef_char[20];
            sprintf(DiffCoef_char, "%llu", DiffCoef);
            string DiffCoeff_str(DiffCoef_char);

	    DAGNode *Constant_DAG = createBVConstant(DiffCoeff_str, MyWidth);
            
            DAGNode* Result_DAG = createDAG("bvule", First_LHS, Constant_DAG, LOGICAL_SYMBOL, 1);   
            
           if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
                {
                    pushBackIntoAPList(APListForDD, Result_DAG);
                }// if(order_lmes_before_lmis_in_normalization || order_lmcs_for_small_lmdds_in_normalization)
            
            return Result_DAG;
            }//First_LHS is like t_1 and First_RHS is like 2^k.x 
}



// Find result of \exists VarToElim. (First_LHS <= First_RHS)\wedge (First_RHS <= Second_RHS)
DAGNode* applyFourierMotzkinForEvenCoefficientsForLMIPairs(DAGNode* First_LHS, DAGNode* First_RHS, DAGNode* Second_RHS, string VarToElim, map<string, int> &WidthTable, list<DAGNode*> &APListForDD)
{
     set<string> Support_First_RHS;    
     getDAGNames(First_RHS, Support_First_RHS);  
     set<string> Support_First_LHS;    
     getDAGNames(First_LHS, Support_First_LHS);    
     set<string> Support_Second_RHS;    
     getDAGNames(Second_RHS, Support_Second_RHS);    
     
     
     if(Support_First_RHS.find(VarToElim)!= Support_First_RHS.end())//First_RHS is like 2^k.x 
            {
            ULINT k, p, pMinusk, TwoPowerpMinusk, TwoPowerpMinusTwoPowerk, TwoPowerk, TwoPowerp;
            
            list<string> VarNames;
            VarNames.push_back(VarToElim);
             
            attachSetToList(Support_First_RHS, VarNames);
            attachSetToList(Support_First_LHS, VarNames);
            attachSetToList(Support_Second_RHS, VarNames);

            Matrix T_Coef_Mat, T_Const_Mat;
            getCoefsAndConstsModified(First_RHS, VarNames, T_Coef_Mat, T_Const_Mat);
              
            ULINT MyCoefficient = T_Coef_Mat[0][0];
            k = powerOfTwo(MyCoefficient);
            
            map<string, int>::iterator wit = WidthTable.find(VarToElim);
            if(wit == WidthTable.end())
                {
                cout<<"\nError in applyFourierMotzkinForEvenCoefficientsForLMIPairs in Eliminator.cpp\nNo entry for variable "<<VarToElim<<" in the WidthTable\n";
                exit(1);
                }
            
            p = wit->second;   
            
            pMinusk = p-k;
            
            //cout<<"\nk = "<<k<<endl;
            //cout<<"\np = "<<p<<endl;            
            
            TwoPowerp = findPower(p); 
            TwoPowerk = findPower(k);
            TwoPowerpMinusTwoPowerk = TwoPowerp - TwoPowerk;
            TwoPowerpMinusk = findPower(pMinusk);            
               
            
            char TwoPowerpMinusTwoPowerk_char[20];
            sprintf(TwoPowerpMinusTwoPowerk_char, "%llu", TwoPowerpMinusTwoPowerk);
            string TwoPowerpMinusTwoPowerk_str(TwoPowerpMinusTwoPowerk_char);

	    DAGNode* TwoPowerpMinusTwoPowerk_DAG = createBVConstant(TwoPowerpMinusTwoPowerk_str, p);
            
            DAGNode* t1Ltet3;
            DAGNode* t1LteTwoPowerpMinusTwoPowerk;
                    
            t1Ltet3 = createDAG("bvule", First_LHS,  Second_RHS, LOGICAL_SYMBOL, 1);   
            //cout<<"\nt1Ltet3 = "<<getDAGStr(t1Ltet3)<<endl;
            
            t1LteTwoPowerpMinusTwoPowerk = createDAG("bvule", First_LHS,  TwoPowerpMinusTwoPowerk_DAG, LOGICAL_SYMBOL, 1);     
            //cout<<"\nt1LteTwoPowerpMinusTwoPowerk = "<<getDAGStr(t1LteTwoPowerpMinusTwoPowerk)<<endl;
            
            Matrix t1_Coef_Mat, t1_Const_Mat, t3_Coef_Mat, t3_Const_Mat;
            Matrix TwoPowerpMinuskt1_Coef_Mat, TwoPowerpMinuskt1_Const_Mat, TwoPowerpMinuskt3_Coef_Mat, TwoPowerpMinuskt3_Const_Mat;
            Matrix t1PlusTwoPowerkMinusOne_Const_Mat;
            Matrix  t3PlusOne_Const_Mat;
            Matrix TwoPowerpMinuskt3PlusOne_Const_Mat;
            Matrix AITwoPowerpMinuskt1_Const_Mat;
            
            
            getCoefsAndConstsModified(First_LHS, VarNames, t1_Coef_Mat, t1_Const_Mat);
            getCoefsAndConstsModified(Second_RHS, VarNames, t3_Coef_Mat, t3_Const_Mat);
            
            
            copyMatrix(t1_Coef_Mat, TwoPowerpMinuskt1_Coef_Mat);
            scalarMatMultiply(TwoPowerpMinuskt1_Coef_Mat,  TwoPowerpMinusk,  TwoPowerp);
            
            copyMatrix(t1_Const_Mat, TwoPowerpMinuskt1_Const_Mat);
            scalarMatMultiply(TwoPowerpMinuskt1_Const_Mat,  TwoPowerpMinusk,  TwoPowerp);
            
            copyMatrix(t3_Coef_Mat, TwoPowerpMinuskt3_Coef_Mat);
            scalarMatMultiply(TwoPowerpMinuskt3_Coef_Mat,  TwoPowerpMinusk,  TwoPowerp);
            
            copyMatrix(t3_Const_Mat, TwoPowerpMinuskt3_Const_Mat);
            scalarMatMultiply(TwoPowerpMinuskt3_Const_Mat,  TwoPowerpMinusk,  TwoPowerp);
            
            Matrix One_Const_Mat, Zero_Const_Mat, TwoPowerkMinusOne_Const_Mat;
            
            getNullMatrix(One_Const_Mat, 1, 1);
            One_Const_Mat[0][0]=1;
            
            getNullMatrix(Zero_Const_Mat, 1, 1);
            
            getNullMatrix(TwoPowerkMinusOne_Const_Mat, 1, 1);
            TwoPowerkMinusOne_Const_Mat[0][0]=TwoPowerk-1;
            
            copyMatrix(t1_Const_Mat, t1PlusTwoPowerkMinusOne_Const_Mat);
            matAdd(t1PlusTwoPowerkMinusOne_Const_Mat, TwoPowerkMinusOne_Const_Mat, TwoPowerp);
            
            copyMatrix(t3_Const_Mat, t3PlusOne_Const_Mat);
            matAdd(t3PlusOne_Const_Mat, One_Const_Mat, TwoPowerp);
            
            copyMatrix(TwoPowerpMinuskt3_Const_Mat, TwoPowerpMinuskt3PlusOne_Const_Mat);
            matAdd(TwoPowerpMinuskt3PlusOne_Const_Mat, One_Const_Mat, TwoPowerp);
            
           
            copyMatrix(TwoPowerpMinuskt1_Const_Mat, AITwoPowerpMinuskt1_Const_Mat);
            aInverseOfMatrix(AITwoPowerpMinuskt1_Const_Mat, TwoPowerp);
            
            DAGNode* TwoPowerpMinuskt1EqZero;
            TwoPowerpMinuskt1EqZero = quickSimplify_WithLMISupport(getDAGFromEqs(TwoPowerpMinuskt1_Coef_Mat, AITwoPowerpMinuskt1_Const_Mat, VarNames, TwoPowerp));
            //cout<<"\nTwoPowerpMinuskt1EqZero = "<<getDAGStr(TwoPowerpMinuskt1EqZero)<<endl;
            
            vector<string> LMITypesBVUGE;
            LMITypesBVUGE.push_back("bvuge");
            vector<string> LMITypesBVULE;
            LMITypesBVULE.push_back("bvule");    
    
            DAGNode* t3Gtet1PlusTwoPowerkMinusOne = quickSimplify_WithLMISupport(getDAGFromInequations(t3_Coef_Mat, t3_Const_Mat, t1_Coef_Mat, t1PlusTwoPowerkMinusOne_Const_Mat, VarNames, TwoPowerp, LMITypesBVUGE));
            //cout<<"\nt3Gtet1PlusTwoPowerkMinusOne = "<<getDAGStr(t3Gtet1PlusTwoPowerkMinusOne)<<endl;
            
            DAGNode* t3PlusOneLtet1PlusTwoPowerkMinusOne = quickSimplify_WithLMISupport(getDAGFromInequations(t3_Coef_Mat, t3PlusOne_Const_Mat, t1_Coef_Mat, t1PlusTwoPowerkMinusOne_Const_Mat, VarNames, TwoPowerp, LMITypesBVULE));
            //cout<<"\nt3PlusOneLtet1PlusTwoPowerkMinusOne = "<<getDAGStr(t3PlusOneLtet1PlusTwoPowerkMinusOne)<<endl;
            
            DAGNode* TwoPowerpMinuskt3PlusOneLteTwoPowerpMinuskt1 = quickSimplify_WithLMISupport(getDAGFromInequations(TwoPowerpMinuskt3_Coef_Mat, TwoPowerpMinuskt3PlusOne_Const_Mat, TwoPowerpMinuskt1_Coef_Mat, TwoPowerpMinuskt1_Const_Mat, VarNames, TwoPowerp, LMITypesBVULE));
            //cout<<"\nTwoPowerpMinuskt3PlusOneLteTwoPowerpMinuskt1 = "<<getDAGStr(TwoPowerpMinuskt3PlusOneLteTwoPowerpMinuskt1)<<endl;
            
            
            
            Matrix MinusTwo_Const_Mat, MinusTwo_Coef_Mat;
              
            getNullMatrix(MinusTwo_Coef_Mat, 1, VarNames.size());
            getNullMatrix(MinusTwo_Const_Mat, 1, 1);
            MinusTwo_Const_Mat[0][0]=TwoPowerp-2;
            
            DAGNode* t3LteMinusTwo = quickSimplify_WithLMISupport(getDAGFromInequations(t3_Coef_Mat, t3_Const_Mat, MinusTwo_Coef_Mat, MinusTwo_Const_Mat, VarNames, TwoPowerp, LMITypesBVULE));
            //cout<<"\nt3LteMinusTwo1 = "<<getDAGStr(t3LteMinusTwo)<<endl;    
            
            
            //Create the Boolean combination
            
            vector<DAGNode*> SplittedVector;
            
            vector<DAGNode*> Constraint1_Vector;
            Constraint1_Vector.push_back(t1Ltet3);
            Constraint1_Vector.push_back(TwoPowerpMinuskt1EqZero);                            
            DAGNode* Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);


            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(t1Ltet3);
            Constraint1_Vector.push_back(t3Gtet1PlusTwoPowerkMinusOne);
            Constraint1_Vector.push_back(t1LteTwoPowerpMinusTwoPowerk);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);


            Constraint1_Vector.clear();
            Constraint1_Vector.push_back(t1Ltet3);
            Constraint1_Vector.push_back(t3LteMinusTwo);
            Constraint1_Vector.push_back(t3PlusOneLtet1PlusTwoPowerkMinusOne);            
            Constraint1_Vector.push_back(TwoPowerpMinuskt3PlusOneLteTwoPowerpMinuskt1);
            Constraint1 = quickSimplify_WithLMISupport(getConjunctionFromVector(Constraint1_Vector));
            SplittedVector.push_back(Constraint1);   
            
            
            DAGNode* FMResult = quickSimplify_WithLMISupport(getDisjunctionFromVector(SplittedVector));
            //cout<<"\nFMResult = "<<getDAGStr(FMResult)<<endl;
            
            if(order_lmes_before_lmis_in_normalization)
                {
                    pushBackIntoAPList(APListForDD, TwoPowerpMinuskt1EqZero);
                    pushBackIntoAPList(APListForDD, t1Ltet3);
                    pushBackIntoAPList(APListForDD, t3Gtet1PlusTwoPowerkMinusOne);
                    pushBackIntoAPList(APListForDD, t1LteTwoPowerpMinusTwoPowerk);
                    pushBackIntoAPList(APListForDD, t3LteMinusTwo);
                    pushBackIntoAPList(APListForDD, t3PlusOneLtet1PlusTwoPowerkMinusOne);
                    pushBackIntoAPList(APListForDD, TwoPowerpMinuskt3PlusOneLteTwoPowerpMinuskt1);                    
                }// if(order_lmes_before_lmis_in_normalization)
            else if(order_lmcs_for_small_lmdds_in_normalization)
                {
                    pushBackIntoAPList(APListForDD, t1Ltet3);
                    pushBackIntoAPList(APListForDD, t3Gtet1PlusTwoPowerkMinusOne);
                    pushBackIntoAPList(APListForDD, t1LteTwoPowerpMinusTwoPowerk);
                    pushBackIntoAPList(APListForDD, t3LteMinusTwo);
                    pushBackIntoAPList(APListForDD, t3PlusOneLtet1PlusTwoPowerkMinusOne);
                    pushBackIntoAPList(APListForDD, TwoPowerpMinuskt3PlusOneLteTwoPowerpMinuskt1);
                    pushBackIntoAPList(APListForDD, TwoPowerpMinuskt1EqZero);
                }// if(order_lmcs_for_small_lmdds_in_normalization)
            
            return FMResult;            
            }//First_RHS is like 2^k.x 
     else
         {
         cout<<"\n"<<First_RHS<<" free of variable to be eliminated in function applyFourierMotzkinForEvenCoefficientsForLMIPairs in Eliminator.cpp\n";
         exit(1);
         }//First_RHS is not like 2^k.x 
}//function



// Check if dag is a conjunction of LMCs. Return true if yes, false otherwise
bool checkIfConjunctionOfLMCs(DAGNode* Constraint)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt" || MyName == "=" || MyName == "is_not_equal" ) // terminal reached
    {
      return true;
    }
  else if(MyName == "true") // terminal reached
    {
      return true;
    }

  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function checkIfConjunctionOfLMCs in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      bool LeftConjunction = checkIfConjunctionOfLMCs(*iout);
      
      iout++;

      bool RightConjunction = checkIfConjunctionOfLMCs(*iout);
      
      if(LeftConjunction && RightConjunction)
          return true;
      else
          return false;      
    }
  else 
  {
      return false;
  }
}




// Copy the list of strings to a set of strings
void copyListToSet(list<string> &MyList, set<string> &MySet)
{
  
for(list<string>::iterator it=MyList.begin(); it!=MyList.end(); it++)
  {
    MySet.insert(*it);
  }
}



// Functions moved from DAGEliminator.cpp

// Version of getModelGeneral which handles equalities, disequalities with inequations
bool getModelGeneral_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
time_t start_s, finish_s, duration_s;
time(&start_s); 

bool return_value;

if(true)
                {
                    //ofstream *outfile = new ofstream();
                    //cout<<"\nOriginal DAG = "<<getDAGStr(root[0])<<endl;
                    //outfile->open("DAG_to_print.txt");
                    cout<<"\nSize of dag = "<<dm.getSizeOfDAG(node)<<endl;
                    //*outfile<<"\nDAG "<<endl;
                    //dm.printDAG(node, outfile);
                    //outfile->close();
                    //showMap(WidthTable);
                }    
if(internal_solver == "yices")
	{
	return_value = getModel_WithSupportForLMIs(node, Model, WidthTable);
	}
else if(internal_solver == "stp")
	{
	return_value = getModelUsingSTP_WithSupportForLMIs(node, Model, WidthTable);
        }
else	
	{
	cout<<"\nUnknown solver in getModelGeneral_WithSupportForLMIs\n";
	exit(1);
	}

  time(&finish_s); 
  duration_s = finish_s - start_s;	  	  
  Time_in_SMT_calls = Time_in_SMT_calls  + duration_s;
  
  return return_value;
}


// Version of getModel which handles equalities, disequalities with inequations
bool getModel_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  Model.clear();
  if(ThUnsatSingleFormulaWithCEXFile_WithSupportForLMIs(node, WidthTable)) // node is unsat; return true
    {
      return true;
    }

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function getModel_WithSupportForLMIs in Eliminator.cpp has timed out. Its result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}

  // node is sat; CEX is in result_thunsat_single_formula.txt
  getModelFromFile(Model);
  //showModel(Model);
	
  time(&end);
  duration = end - start;
  getModel_time = getModel_time + duration;

  return false;
}
  


//  Version of ThUnsatSingleFormulaWithCEXFile which handles equalities, disequalities with inequations
bool ThUnsatSingleFormulaWithCEXFile_WithSupportForLMIs(DAGNode* f, map<string, int> &WidthTable)
{
  FILE *smt_file = fopen("thunsat_single_formula.ys", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.ys\n"; exit(1);}

  writeBVDeclarationsInYicesFormat(smt_file, WidthTable);

  string dag_str = callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable_WithSupportForLMIs(f, WidthTable);


  fprintf(smt_file, "\n(assert %s )\n", dag_str.c_str());
  fprintf(smt_file, "\n(check)\n");
  fclose(smt_file);

  string command;
  command = "yices -tc -e ";

  char tout_char[100];
  sprintf(tout_char,"%d",time_out);
  string tout_str(tout_char);

  command += "-tm ";
  command += tout_str;
  command += " thunsat_single_formula.ys > result_thunsat_single_formula.txt";
  system(command.c_str());

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function ThUnsatSingleFormulaWithCEXFile_WithSupportForLMIs in Eliminator.cpp has timed out. Its result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}



  //system("yices -tc -e thunsat_single_formula.ys > result_thunsat_single_formula.txt");
  FILE *result_smt_file = fopen("result_thunsat_single_formula.txt", "r");

  if(result_smt_file == NULL) {cout<<"\nCannot create file result_thunsat_single_formula.txt\n"; exit(1);}

  char result[100];
  fscanf(result_smt_file, "%s", result);
  fclose(result_smt_file);
  string result_str(result);

  if(result_str=="unsat") return true;
  else if(result_str=="sat") return false;
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function ThUnsatSingleFormulaWithCEXFile_WithSupportForLMIs\n"; exit(1);
    }
}




// Version of ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG with support for LMIs
bool ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(DAGNode* f, map<string, int> &WidthTable)
{
  FILE *smt_file = fopen("thunsat_single_formula.smt", "w");

  if(smt_file == NULL) {cout<<"\nCannot create file thunsat_single_formula.smt\n"; exit(1);}

  fprintf(smt_file,"(benchmark test\n");

  string dag_str = printDAGInSMTLIBFormatAsDAG_WithSupportForLMIs(f, WidthTable, smt_file);

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);

  system("stp -p --SMTLIB1 thunsat_single_formula.smt > result_thunsat_single_formula.txt"); //call to STP

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs in Eliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}


  ifstream *infile;
  infile = new ifstream();
  infile->open("result_thunsat_single_formula.txt");
  
  string s="unknown";

  while(!infile->eof())
    {
 	
      *infile>>s;
      if(s=="")
	break;
    }

  infile->close();

  if(s=="unsat") {
		//cout<<"\ns==unsat\n";
		return true;
		}
		
  else if(s=="sat") {
		    //cout<<"\ns==sat\n";
		    return false;
		    }
  else
    {
      cout<<"\nUnknown returned from SMT Solver in function ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs in Eliminator.cpp\n"; exit(1);
    }
}


// Version of getModelUsingSTP with support for LMIs
bool getModelUsingSTP_WithSupportForLMIs(DAGNode *node, map<string, string> &Model, map<string, int> &WidthTable)
{
  time_t start;
  time_t end;
  time_t duration;

  time(&start); 

  Model.clear();
  if(ThUnsatSingleFormulaWithCEXFileInSMTLibFormatAsDAG_WithSupportForLMIs(node, WidthTable)) // node is unsat; return true
    {
      return true;
    }

  if(checkTimeOut())
	{
	// Time Out !!!
        cout<<"\nWarning!!!The function getModelUsingSTP_WithSupportForLMIs in Eliminator.cpp has timed out. It's result is unknown\n";
	timed_out = true; // timed_out flag set
	return false; // Let's temporarily treat it as sat in the called function
	}

  // node is sat; CEX is in result_thunsat_single_formula.txt
  getModelFromSTPCEXFile(Model);
  //showModel(Model);
	
  time(&end);
  duration = end - start;
  getModel_time = getModel_time + duration;

  return false;
}
  

// Get the model in the variable --> value map Model 
// by parsing the CEX returned from yices in the file result_thunsat_single_formula.txt
void getModelFromFile(map<string, string> &Model)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("result_thunsat_single_formula.txt");
  
  string s;
  string signal;
  string value;

  for(int count=0;!infile->eof(); count++)
    {
 	
      *infile>>s;
      if(s=="")
	break;
      //cout<<"Line read is "<<s<<endl;
      if(count==0) // sat/unsat line
	{
	  continue;
	}
      if(count%3==1) // (= line
	{
	  continue;
	}
      if(count%3==2) // signal line
	{
	  signal = s;
	  continue;
	}
      else // value line
	{
	  value  = s;

	  int start_pos = value.find("b");
	  value  = value.substr(start_pos+1);
	  int end_pos = value.find(")");
	  value  = value.substr(0,end_pos);

	  Model.insert(make_pair(signal, value));
	}
    }

  infile->close();

}

// Display the variable --> value map "Model"
void showModel(map<string, string> &Model)
{
  map<string, string> :: iterator  mit =  Model.begin();			
  for(; mit != Model.end(); mit++)
    cout<<mit->first<<":"<<mit->second<<endl;
}


// Get the model in the variable --> value map Model 
// by parsing the CEX returned from STP in the file result_thunsat_single_formula.txt
void getModelFromSTPCEXFile(map<string, string> &Model)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("result_thunsat_single_formula.txt");
  
  string word="", prev_word="", prev_prev_word="";
  while(!infile->eof())
    {
      prev_prev_word = prev_word;
      prev_word = word;
      *infile>>word;
      //cout<<"\nword = "<<word<<endl;
      if(word=="")
	break;
      if(prev_word=="=")
	{
	//cout<<"\nprev_prev_word = "<<prev_prev_word<<"\tprev_word = "<<prev_word<<"\tword = "<<word<<endl;
        int start_pos = word.find("b");
	if(start_pos != string::npos)
		{
		word  = word.substr(start_pos+1);
		}
        start_pos = word.find("x");
	if(start_pos != string::npos)
		{
		word  = word.substr(start_pos+1);
		word = hexToBinaryString(word);
		}
        //cout<<"\nafter transformation: prev_prev_word = "<<prev_prev_word<<"\tprev_word = "<<prev_word<<"\tword = "<<word<<endl;
        
	Model.insert(make_pair(prev_prev_word, word));
	}
    }

  infile->close();

  
}



// Faster version of freeOfVars
// A problem with this is that even if predicate has something like 0*x+..., it will
// say that predicate is not free of x
bool freeOfVarsEfficient(DAGNode *predicate, set<string> &DropVars)
{
  set<string> PredicateSupport;
  getDAGNames(predicate, PredicateSupport);  
  
  set<string> CommonVariables;
  
  set_intersection(PredicateSupport.begin(), PredicateSupport.end(), DropVars.begin(), DropVars.end(),inserter(CommonVariables, CommonVariables.begin()));
  
  if(CommonVariables.empty())//no common variables
    {
     return true;
    }
  else
    {
    return false;  
    }
}



//check if MyPredicate is an LME with odd coefficient for any of the variables to be eliminated
// variables to be eliminated is in the global variable CopyOfVarSetToElim
bool predicateIsLMEWithOddCoefForSomeVarToElim(DAGNode* MyPredicate)
{
    if(MyPredicate->Get_node_name() != "=" && MyPredicate->Get_node_name() != "equivalent")//MyPredicate is not an LME
    {
        return false;
    }
    else 
    {
     set<string> Support;
     getDAGNames(MyPredicate, Support);   
     set<string> CommonVariables;
     set_intersection(Support.begin(), Support.end(), CopyOfVarSetToElim.begin(), CopyOfVarSetToElim.end(),inserter(CommonVariables, CommonVariables.begin()));
    
     if(CommonVariables.empty())//There are no variables to eliminate in MyPredicate
        {
        return false; 
        }
     else//There are variables to eliminate in MyPredicate
        {
         // Let's see if any of these variables appear with odd coefficient
         list<DAGNode*>::iterator iout;
         iout = MyPredicate->Get_outlist_start();
         DAGNode *LHS = *iout;
         iout++;
         DAGNode *RHS = *iout;	
         
         map<string, ULINT> VariableCoefficientMap;
         ULINT ConstantPart;
         
         VariableCoefficientMap.clear();         
         obtainVariableCoefficientMap(LHS, VariableCoefficientMap, ConstantPart);
         
         for(map<string, ULINT>::iterator mit = VariableCoefficientMap.begin(); mit != VariableCoefficientMap.end(); mit++)
            {
             string variable = mit->first;
             ULINT coefficient = mit->second;
             
             if(CopyOfVarSetToElim.find(variable) != CopyOfVarSetToElim.end() && coefficient % 2 != 0)
              // variable \in CopyOfVarSetToElim && coefficient is odd
                {
                 return true;
                }
            }//loop
         
         VariableCoefficientMap.clear();         
         obtainVariableCoefficientMap(RHS, VariableCoefficientMap, ConstantPart);
         
         for(map<string, ULINT>::iterator mit = VariableCoefficientMap.begin(); mit != VariableCoefficientMap.end(); mit++)
            {
             string variable = mit->first;
             ULINT coefficient = mit->second;
             
             if(CopyOfVarSetToElim.find(variable) != CopyOfVarSetToElim.end() && coefficient % 2 != 0)
              // variable \in CopyOfVarSetToElim && coefficient is odd
                {
                 return true;
                }
            }//loop
         
         return false;//There are variables to eliminate in MyPredicate, but
         //all of them appear with even coefficients
         
        }////There are variables to eliminate in MyPredicate  
    }//MyPredicate is an LME
}//end of function



//Version of predicateIsLMEWithOddCoefForSomeVarToElim with the following differences
//1) Variables to be eliminated is a parameter; not a global variable CopyOfVarSetToElim
//2) Sets OddVariableToEliminate as the variable to be eliminated with odd coefficient in MyPredicate 
bool predicateIsLMEWithOddCoefForSomeVarToElim(DAGNode* MyPredicate, set<string> &VarSetToElim, string &OddVariableToEliminate)
{
    if(MyPredicate->Get_node_name() != "=" && MyPredicate->Get_node_name() != "equivalent")//MyPredicate is not an LME
    {
        return false;
    }
    else 
    {
     set<string> Support;
     getDAGNames(MyPredicate, Support);   
     set<string> CommonVariables;
     set_intersection(Support.begin(), Support.end(), VarSetToElim.begin(), VarSetToElim.end(),inserter(CommonVariables, CommonVariables.begin()));
    
     if(CommonVariables.empty())//There are no variables to eliminate in MyPredicate
        {
        return false; 
        }
     else//There are variables to eliminate in MyPredicate
        {
         // Let's see if any of these variables appear with odd coefficient
         list<DAGNode*>::iterator iout;
         iout = MyPredicate->Get_outlist_start();
         DAGNode *LHS = *iout;
         iout++;
         DAGNode *RHS = *iout;	
         
         map<string, ULINT> VariableCoefficientMap;
         ULINT ConstantPart;
         
         VariableCoefficientMap.clear();         
         obtainVariableCoefficientMap(LHS, VariableCoefficientMap, ConstantPart);
         
         for(map<string, ULINT>::iterator mit = VariableCoefficientMap.begin(); mit != VariableCoefficientMap.end(); mit++)
            {
             string variable = mit->first;
             ULINT coefficient = mit->second;
             
             if(VarSetToElim.find(variable) != VarSetToElim.end() && coefficient % 2 != 0)
              // variable \in VarSetToElim && coefficient is odd
                {
                 OddVariableToEliminate = variable;
                 
                 return true;
                }
            }//loop
         
         VariableCoefficientMap.clear();         
         obtainVariableCoefficientMap(RHS, VariableCoefficientMap, ConstantPart);
         
         for(map<string, ULINT>::iterator mit = VariableCoefficientMap.begin(); mit != VariableCoefficientMap.end(); mit++)
            {
             string variable = mit->first;
             ULINT coefficient = mit->second;
             
             if(VarSetToElim.find(variable) != VarSetToElim.end() && coefficient % 2 != 0)
              // variable \in VarSetToElim && coefficient is odd
                {
                 OddVariableToEliminate = variable;
                 
                 return true;
                }
            }//loop
         
         return false;//There are variables to eliminate in MyPredicate, but
         //all of them appear with even coefficients
         
        }////There are variables to eliminate in MyPredicate  
    }//MyPredicate is an LME
}//end of function



// Faster version of freeOfVarModified
// A problem with this is that even if node has something like 0*x+..., it will
// say that node is not free of x
bool freeOfVarModifiedEfficient(DAGNode* node, string VarToElim)
{
set<string> names;
getDAGNames(node, names);
if(names.find(VarToElim) == names.end())  return true;
return false;
}

// Functions moved from DAGEliminator.cpp end here


// Return true if the global variable "PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles" is true 
// and time out has happened;
// Return false otherwise
bool checkTimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles()
{
  if(!PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles) //disabled
        {
        return false;
        }

  if(TimedOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles) // already timed out. No need to check
    {
      return true;
    }

  if(TimeOutStart_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles == 0)
        {
        cout<<"\nYou have set the variable PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles  in main.cc. But TimeOutStart_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles is not set. Either disable PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles  in main.cc (this disables the timeout mechanism) or set TimeOutStart_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles before calling checkTimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles in Eliminator.cpp"<<endl;
        exit(1);
        }

  time_t present_time, duration;

  time(&present_time);
  duration = present_time - TimeOutStart_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles;
  if(duration >= TimeOut_In_PerformMonniauxAtStartupInConversionToDisjunctionOfTailedTriangles)
        {
        return true;
        }
return false;
}



// Function to drop simple implied LMIs from LMISet, before applying Layer2
// Result is obtianed in LMISetAfterDropping
void dropSimpleImpliedLMIs(set<DAGNode*> &LMISet, set<DAGNode*> &LMISetAfterDropping, int WidthOfVarToElim)
{
  map<DAGNode*, ULINT> UpperBounds;
  map<DAGNode*, ULINT> LowerBounds;
    
  for(set<DAGNode*>::iterator it=LMISet.begin(); it != LMISet.end(); it++)
    {
        DAGNode* Constraint = *it;
        list<DAGNode*>::iterator iout;
        iout = Constraint->Get_outlist_start();
        DAGNode *Constraint_LHS = *iout;
        iout++;
        DAGNode *Constraint_RHS = *iout;	
        string ConstraintType = Constraint->Get_node_name();
        
        if(!isConstant(Constraint_LHS) && !isConstant(Constraint_RHS))
        {
            //both sides are not constants; just insert it into the final set
            LMISetAfterDropping.insert(Constraint);
        }
        else if(ConstraintType == "bvule")
        {
           DAGNode* Term = Constraint_LHS;
           string Constant_str = Constraint_RHS->Get_node_name();
           ULINT NewUpperBound = strtoull(Constant_str.c_str(), NULL, 0);
           
           map<DAGNode*, ULINT>::iterator map_it = UpperBounds.find(Term);
           
           if(map_it == UpperBounds.end())//!existing
            {
            UpperBounds.insert(make_pair(Term, NewUpperBound));  
            }
           else//existing
           {
            ULINT ExistingUpperBound = map_it->second;
            ULINT FinalUpperBound = findMinimum(NewUpperBound, ExistingUpperBound);
            map_it->second = FinalUpperBound;//use the stronger one
           }
        }//else if(ConstraintType == "bvule")
        else if(ConstraintType == "bvuge")
        {
           DAGNode* Term = Constraint_LHS;
           string Constant_str = Constraint_RHS->Get_node_name();
           ULINT NewLowerBound = strtoull(Constant_str.c_str(), NULL, 0);

           map<DAGNode*, ULINT>::iterator map_it = LowerBounds.find(Term);

           if(map_it == LowerBounds.end())//!existing
            {
            LowerBounds.insert(make_pair(Term, NewLowerBound));  
            }
           else//existing
           {
            ULINT ExistingLowerBound = map_it->second;
            ULINT FinalLowerBound = findMaximum(NewLowerBound, ExistingLowerBound);
            map_it->second = FinalLowerBound;//use the stronger one
           } 
        }//else if(ConstraintType == "bvuge")
        else
        {
          cout<<"\nSuspicious LMI "<<getDAGStr(Constraint)<<" in function dropSimpleImpliedLMIs in Eliminator.cpp\n";
          exit(1);
        }      
    }//for   
  
  //Obtain the constraints from the maps and insert them into LMISetAfterDropping
  
  for(map<DAGNode*, ULINT>::iterator map_it = UpperBounds.begin(); map_it != UpperBounds.end(); map_it++)
    {
    DAGNode* Term = map_it->first;
    ULINT Constant = map_it->second;
  
    char ConstantPart_char[10];
    sprintf(ConstantPart_char, "%llu", Constant);
    string ConstantPart_str(ConstantPart_char);
    // Term <= Constant is the new LMI
    DAGNode *NewRHS = createBVConstant(ConstantPart_str, WidthOfVarToElim); 
    DAGNode *NewLMI = createDAG("bvule", Term, NewRHS, LOGICAL_SYMBOL, 1);
    
    LMISetAfterDropping.insert(NewLMI);
    }
  
 for(map<DAGNode*, ULINT>::iterator map_it = LowerBounds.begin(); map_it != LowerBounds.end(); map_it++)
    {
    DAGNode* Term = map_it->first;
    ULINT Constant = map_it->second;
  
    char ConstantPart_char[10];
    sprintf(ConstantPart_char, "%llu", Constant);
    string ConstantPart_str(ConstantPart_char);
    // Term >= Constant is the new LMI
    DAGNode *NewRHS = createBVConstant(ConstantPart_str, WidthOfVarToElim); 
    DAGNode *NewLMI = createDAG("bvuge", Term, NewRHS, LOGICAL_SYMBOL, 1);
    
    LMISetAfterDropping.insert(NewLMI);
    } 
    
}



// There is a file "ConstraintsToLayer3.txt".
// Attach \exists VarsToElim. (Conjunction) to this.
void attachInputToLayer3ToConstraintsFile(set<string> &VarsToElim, DAGNode* Conjunction)
{
  FILE* CFile = fopen("ConstraintsToLayer3.txt", "a+");
  if(CFile == NULL)
    {
      cout<<"\nCould not open ConstraintsToLayer3.txt\n";
      exit(1);
    }
  
 fprintf(CFile, "\n\nBenchmark = %s\tMethod = %s\n", InFileName.c_str(), Dag_Eliminator_Type);
 
 fprintf(CFile, "X = {"); 
 for(set<string>::iterator it = VarsToElim.begin(); it != VarsToElim.end(); it++)
 {
    string variable = *it;
    fprintf(CFile, "%s, ", variable.c_str()); 
 }
 fprintf(CFile, "}\n"); 
 
 vector<DAGNode*> Conjunction_Vector;
 splitConjunctionIntoVectorGeneral(Conjunction, Conjunction_Vector);
 showAPVector("A", Conjunction_Vector, CFile);
  fprintf(CFile, "\n"); 

 fclose(CFile);
}


// Checks if \exists VarToElim. (Phi /\ Xi) \equiv \exists VarToElim. (Phi) 
// either using Counting Lemma or using BDDs, depending on the argument
// Layer2CheckUsingBDDs
bool isExtForLMDsAndLMIsGeneral(set<DAGNode*> &Phi, set<DAGNode*> &Xi, string VarToElim, int WidthOfVarToElim, ULINT ModOfVarToElim, map<string, int> &WidthTable)
{
    bool isExtValue;
    
    if(PrintLayer2Times)
    {
        fprintf(LMIQEOutputDataFp, "\nLayer2 Check Details\n");
        showAPSet("Phi", Phi, LMIQEOutputDataFp);
        showAPSet("Xi", Xi, LMIQEOutputDataFp);
    }
    
      
    TimeLayer2ChecksFp = fopen("Layer2ChecksData.txt", "a+");
    if(TimeLayer2ChecksFp == NULL)
        {
        cout<<"\nCould not open Layer2ChecksData.txt\n";
        exit(1);
        }    
    
    unsigned long long int BlastTimeLocal, DDTimeLocal, QETimeLocal;
    
    struct timeval check_start_ms, check_finish_ms;                  
    gettimeofday (&check_start_ms, NULL);    
    
        
    if(Layer2CheckUsingBDDs)
        {         
        isExtValue = isExtForLMDsAndLMIsUsingBDDs(Phi, Xi, VarToElim, WidthTable, BlastTimeLocal, DDTimeLocal, QETimeLocal);       
        }
    else //using counting argument
        {
        isExtValue = isExtForLMDsAndLMIsCorrected(Phi, Xi, VarToElim, WidthOfVarToElim, ModOfVarToElim);         
        }
    
    gettimeofday (&check_finish_ms, NULL);
    unsigned long long int CheckTimeLocal = check_finish_ms.tv_sec * 1000 + check_finish_ms.tv_usec / 1000;
    CheckTimeLocal -= check_start_ms.tv_sec * 1000 + check_start_ms.tv_usec / 1000; 
    
    TotalLayer2CheckTime = TotalLayer2CheckTime +CheckTimeLocal; 
    
    TotalLayer2Checks++;
    
      
    if(Layer2CheckUsingBDDs)
        {
        fprintf(TimeLayer2ChecksFp, "\t%llu\t%llu\t%llu\t%llu\n", CheckTimeLocal, BlastTimeLocal, DDTimeLocal, QETimeLocal);
        }
    else
        {
        fprintf(TimeLayer2ChecksFp, "\n%llu\n", CheckTimeLocal);
        }  
    
    if(PrintLayer2Times)
    {
        fprintf(LMIQEOutputDataFp, "\nTotal time = %llu\n", CheckTimeLocal);
    }
    
     fclose(TimeLayer2ChecksFp);
    
     return isExtValue;
}


// Return the type of the given dagnode
Type getNodeType(DAGNode *node)
{
     Value* MyValue = (Value*)(node->getValue());
     Type MyType = MyValue->get_node_type();
     return MyType;
}



// Version of splitConjunctionIntoVectorGeneral, where it is not checked what the individual formulas are
void splitConjunctionIntoVectorGeneralFormulae(DAGNode* Constraint, vector<DAGNode*> &Conjuncts)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoVectorGeneralFormulae in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoVectorGeneralFormulae(*iout, Conjuncts);
      
      iout++;

      splitConjunctionIntoVectorGeneralFormulae(*iout, Conjuncts);
    }
  else 
    {
     Conjuncts.push_back(Constraint); 
    }
}


// Function to convert a relation to a function
void convertRelationToFunction(DAGNode* node, DAGNode* &lhs, DAGNode* &rhs)
{
  lhs = NULL;  
  bool apply_function_conversion = true;
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  rhs = convertRelationToFunctionInternal(node, PtrDPTable, lhs, apply_function_conversion);
}
  
  

// Function called by convertRelationToFunction
DAGNode*  convertRelationToFunctionInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable, DAGNode* &lhs, bool apply_function_conversion)
{
  DAGNode* ret;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 

  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }


  unsigned int outlist_size = node->Get_outlist_size();
  
  if(outlist_size == 0) { // Terminals
	  ret = node;
  }
  else if(outlist_size == 1) { // Unary operators: no change
	  ret = node;
  }
  else if(outlist_size == 2) { // Binary operators
	  
	  if(apply_function_conversion)
	  {
		list<DAGNode*>::iterator iout = node->Get_outlist_start();
		if(lhs == NULL)
		{
			lhs = *iout;
		}
		assert(lhs == *iout);

		iout++;
		ret = *iout;		
	  }
	  else
	  {
		ret = node;
	  }
  }
  else if(outlist_size == 3) { // Ternary operators
	  
	  string dag_str = node->Get_node_name();
	  assert(dag_str == "if_then_else");
	  if(apply_function_conversion)
	  {
		list<DAGNode*>::iterator iout = node->Get_outlist_start();
		DAGNode* condition = *iout;
		iout++;
		DAGNode* then = *iout;
		iout++;
		DAGNode* els = *iout;

		DAGNode* simplified_condition = convertRelationToFunctionInternal(condition, DPTable, lhs, false);
		assert(simplified_condition != NULL);

		DAGNode* simplified_then = convertRelationToFunctionInternal(then, DPTable, lhs, true);
		assert(simplified_then != NULL);

		DAGNode* simplified_els = convertRelationToFunctionInternal(els, DPTable, lhs, true);
		assert(simplified_els != NULL);

		ret = createDAG("ite", simplified_condition, simplified_then, simplified_els, LOGICAL_SYMBOL, 1);
	  }
	  else
	  {
		ret = node;
	  }
  }
  else {

	string dag_str = node->Get_node_name();
	cout<<dag_str<<" operator encountered in Function convertRelationToFunctionInternal\n";
	exit(1);
  }

  result = DPTable->hashtable_insert(key, ret);
  if(!result.success())
    {

      cout<<"\nHash Insert Failure Inside Function  convertRelationToFunctionInternal\n";
      exit(1);
    }

  return ret;
}



// This function takes a dag "node" as input. Now, if the dag is like and(...(and(R1, R2), R3),..., Rn), 
// it converts it into a set {R1,...,Rn}. Now convert each Ri into a functional form outi=Fi, now creates 
// a map (outi, Fi)
void convertRelationToFunctionMap(DAGNode* node, map<DAGNode*, DAGNode*> &FunctionMap)
{
  //cout<<"\nnode = "<<getDAGStr(node)<<endl;

  vector<DAGNode*> Conjuncts;
  splitConjunctionIntoVectorGeneralFormulae(node, Conjuncts);

   //cout<<"\nDecomposed Relations\n**********\n";
   //for(int i = 0; i < Conjuncts.size(); i++)
   //  {
   //    cout<<getDAGStr(Conjuncts[i])<<endl;
   //  }
  
  //cout<<"\nCreating the function map\n**********\n";
  for(int i = 0; i < Conjuncts.size(); i++)
    {
	DAGNode* Conjunct = Conjuncts[i];
        //cout<<"\nRelation = "<<getDAGStr(Conjunct)<<endl;

	DAGNode *lhs;
	DAGNode *rhs;
	
	convertRelationToFunction(Conjunct, lhs, rhs);
 	
	//cout<<"\nlhs = "<<getDAGStr(lhs)<<endl;
	//cout<<"\nrhs = "<<getDAGStr(rhs)<<endl;

	if(getDAGStr(lhs) == "property_1")
	{
		//cout << "\nproperty_1 encountered\n";
		rhs = simplifyProperty(rhs);
		//cout<<"\nchanged rhs = "<<getDAGStr(rhs)<<endl;
	}

	FunctionMap.insert(make_pair(lhs, rhs));
    }
}
  
  

// Blast a function map, and then print it in Verilog format
void printFunctionMapInVerilog(map<DAGNode*, DAGNode*> &FunctionMap, map<string, int> &WidthTable, string blasted_output_file)
{
	map<DAGNode*, DAGNode*> Blasted_FunctionMap;
	
	for(map<DAGNode*, DAGNode*>::iterator fit = FunctionMap.begin(); fit != FunctionMap.end(); fit++)
	{
		DAGNode *lhs = fit->first;
		DAGNode *rhs = fit->second;

		//cout<<"\nlhs = "<<getDAGStr(lhs)<<endl;
		//cout<<"\nrhs = "<<getDAGStr(rhs)<<endl;

		const vector<DAGNode *>& Bits_lhs_Orig = getBitBlastedDAG_General(lhs);
      		const vector<DAGNode *>& Bits_rhs_Orig = getBitBlastedDAG_General(rhs);
      
      		int lhs_width = Bits_lhs_Orig.size();
      		int rhs_width = Bits_rhs_Orig.size();

		//cout<<"\nlhs_width = "<< lhs_width << endl;
		//cout<<"\nrhs_width = "<< rhs_width << endl;
      
      		const vector<DAGNode *>& Bits_lhs_Padded = lhs_width < rhs_width ? zeroPaddingWithRet(Bits_lhs_Orig, rhs_width) : Bits_lhs_Orig;
      		const vector<DAGNode *>& Bits_rhs_Padded = rhs_width < lhs_width ? zeroPaddingWithRet(Bits_rhs_Orig, lhs_width) : Bits_rhs_Orig;

		assert(Bits_lhs_Padded.size() == Bits_rhs_Padded.size());

		vector<DAGNode *> Bits_lhs;
		vector<DAGNode *> Bits_rhs;
      		for(int index=0; index < Bits_lhs_Padded.size(); index++)
		{
	      		Bits_lhs.push_back(Bits_lhs_Padded[index]);
			Bits_rhs.push_back(Bits_rhs_Padded[index]);
		}   		

		for(int index = 0; index < Bits_lhs.size(); index++)
		{
			DAGNode *lhs_bit = Bits_lhs[index];
			DAGNode *rhs_bit = quickSimplify(Bits_rhs[index]);

			//cout<<"\nlhs_bit = "<<getDAGStr(lhs_bit)<<endl;
			//cout<<"\nrhs_bit = "<<getDAGStr(rhs_bit)<<endl;

			lhs_bit = simplifyTransitionFunction(lhs_bit);// To avoid the time stamp (no time stamp needed with variables while printing in Verilog)
			rhs_bit = quickSimplify(simplifyTransitionFunction(rhs_bit)); // To avoid time stamp of variables, avoid ites, and simplify exors

			//cout<<"\nsimplified_lhs_bit = "<<getDAGStr(lhs_bit)<<endl;
			//cout<<"\nsimplified_rhs_bit = "<<getDAGStr(rhs_bit)<<endl;

			if(isTrue(lhs_bit) || isFalse(lhs_bit))
			{		
				// This bit has resulted due to our padding process; not a constraint 
			} 
			else if(Blasted_FunctionMap.find(lhs_bit) != Blasted_FunctionMap.end())
			{
				// Entry already exists
			}
			else
			{		
				Blasted_FunctionMap.insert(make_pair(lhs_bit, rhs_bit));	     
			}		
		}  
	}

	//cout<<"\nBlasted function map\n**********\n";
	//for(map<DAGNode*, DAGNode*>::iterator fit = Blasted_FunctionMap.begin(); fit != Blasted_FunctionMap.end(); fit++)
	//{
	//	DAGNode *lhs = fit->first;
	//	DAGNode *rhs = fit->second;
	//
	//	cout<<"\nlhs = "<<getDAGStr(lhs)<<endl;
	//	cout<<"\nrhs = "<<getDAGStr(rhs)<<endl;
	//}

	// Let's print in Verilog

	// First find the inputs, outputs, registers, and wires
	set<string> vinputs;
	set<string> voutputs;
	set<string> vregisters;
	set<string> vwires;
	string vobservedoutput;

	set<string> voutputs_and_vregisters;
	set<string> vinputs_and_vregisters;	
	for(map<DAGNode*, DAGNode*>::iterator fit = Blasted_FunctionMap.begin(); fit != Blasted_FunctionMap.end(); fit++)
	{
		DAGNode *lhs = fit->first;
		DAGNode *rhs = fit->second;

		string lhs_variable = lhs->Get_node_name();
		voutputs_and_vregisters.insert(lhs_variable);

		set<string> rhs_variables;
		getDAGNames(rhs, rhs_variables);
		set_union(vinputs_and_vregisters.begin(), vinputs_and_vregisters.end(), rhs_variables.begin(), rhs_variables.end(),inserter(vinputs_and_vregisters, vinputs_and_vregisters.begin()));		
	}

	//showSet(voutputs_and_vregisters, "voutputs_and_vregisters");
	//showSet(vinputs_and_vregisters, "vinputs_and_vregisters");

	vobservedoutput = "property";

	set_intersection(vinputs_and_vregisters.begin(), vinputs_and_vregisters.end(), voutputs_and_vregisters.begin(), voutputs_and_vregisters.end(),inserter(vregisters, vregisters.begin()));
	
	set_difference(vinputs_and_vregisters.begin(), vinputs_and_vregisters.end(), vregisters.begin(), vregisters.end(),inserter(vinputs, vinputs.begin()));

	set_difference(voutputs_and_vregisters.begin(), voutputs_and_vregisters.end(), vregisters.begin(), vregisters.end(),inserter(voutputs, voutputs.begin()));

	vwires = voutputs; // all outputs are wires except the property output
	
	for(set<string>::iterator reg_it = vregisters.begin(); reg_it != vregisters.end(); reg_it++)
	{
		string reg_name = *reg_it;
		string wire_name = reg_name;
		wire_name += "_in";

		vwires.insert(wire_name);
	}

	//showSet(vinputs, "vinputs");
	//showSet(vregisters, "vregisters");
	//showSet(vwires, "vwires");

	string blasted_output_file_temp;
	blasted_output_file_temp = blasted_output_file;
	blasted_output_file_temp += "_temp";
	FILE *verilog_temp_fp = fopen(blasted_output_file_temp.c_str(), "w+");
	assert(verilog_temp_fp != NULL);

	int TempCount = 0;
	map<DAGNode*, string> LabelTable;
  	LabelTable.clear();

	map<string, string> Blasted_FunctionMap_Names_After_Writing;
	for(map<DAGNode*, DAGNode*>::iterator fit = Blasted_FunctionMap.begin(); fit != Blasted_FunctionMap.end(); fit++)
	{
		DAGNode *lhs = fit->first;
		DAGNode *rhs = fit->second;

		string lhs_variable = lhs->Get_node_name();
		if(vregisters.find(lhs_variable) != vregisters.end())//lhs_variable is a register
		{
			lhs_variable += "_in";
		}		

		string rhs_str;
		rhs_str = printBinaryDAGInVerilogInternal(rhs, LabelTable, verilog_temp_fp, TempCount, vwires);

		Blasted_FunctionMap_Names_After_Writing.insert(make_pair(lhs_variable, rhs_str));
	}


	for(map<string, string>::iterator fit = Blasted_FunctionMap_Names_After_Writing.begin(); fit != Blasted_FunctionMap_Names_After_Writing.end(); fit++)
	{
		string lhs_string = fit->first;
		string rhs_string = fit->second;

		fprintf(verilog_temp_fp,"  assign %s = %s;\n", lhs_string.c_str(), rhs_string.c_str()); // written
	}

	fprintf(verilog_temp_fp,"  assign property = property_0;\n");

	fprintf(verilog_temp_fp,"  always @ (posedge clock) begin\n"); // written

	for(set<string>::iterator reg_it = vregisters.begin(); reg_it != vregisters.end(); reg_it++)
	{
		string reg_name = *reg_it;
		fprintf(verilog_temp_fp,"    %s <= %s_in;\n", reg_name.c_str(), reg_name.c_str()); 
	}

	fprintf(verilog_temp_fp,"  end\n"); 
	fprintf(verilog_temp_fp,"endmodule\n\n"); 	

	fclose(verilog_temp_fp);

	string blasted_output_file_temp1;
	blasted_output_file_temp1 = blasted_output_file;
	blasted_output_file_temp1 += "_temp1";
	FILE *verilog_temp1_fp = fopen(blasted_output_file_temp1.c_str(), "w+");
	assert(verilog_temp1_fp != NULL);

	fprintf(verilog_temp1_fp,"module blasted_TR ( clock, \n");
	set<string> vinputs_and_vobservedoutput(vinputs.begin(), vinputs.end());
	vinputs_and_vobservedoutput.insert(vobservedoutput);
	printSetOnLines(verilog_temp1_fp, vinputs_and_vobservedoutput, 5, 4);
	fprintf(verilog_temp1_fp,");\n");
	
	fprintf(verilog_temp1_fp,"  input clock;\n");
	fprintf(verilog_temp1_fp,"  input ");
	printSetOnLines(verilog_temp1_fp, vinputs, 5, 4);
	fprintf(verilog_temp1_fp,";\n"); 

	fprintf(verilog_temp1_fp,"  output %s;\n", vobservedoutput.c_str());

	fprintf(verilog_temp1_fp,"  reg ");
	printSetOnLines(verilog_temp1_fp, vregisters, 5, 4);
	fprintf(verilog_temp1_fp,";\n"); 

	fprintf(verilog_temp1_fp,"  wire ");
	printSetOnLines(verilog_temp1_fp, vwires, 5, 4);
	fprintf(verilog_temp1_fp,";\n"); 
	
	fclose(verilog_temp1_fp);

	string command = "cat ";
	command += blasted_output_file_temp1;
	command += " ";
	command += blasted_output_file_temp;
	command += " > ";
	command += blasted_output_file;
	system(command.c_str());

	command = "rm ";
	command += blasted_output_file_temp1;
	system(command.c_str());

	command = "rm ";
	command += blasted_output_file_temp;
	system(command.c_str());


	//Print the inputs in inputs.txt and non_inputs in non_inputs.txt
	string inputs_file;
	inputs_file = blasted_output_file;
	inputs_file += "_inputs.txt";
	FILE *inputs_file_fp = fopen(inputs_file.c_str(), "w+");
	assert(inputs_file_fp != NULL);
	printSet(inputs_file_fp, vinputs);
	fclose(inputs_file_fp);

	string non_inputs_file;
	non_inputs_file = blasted_output_file;
	non_inputs_file += "_non_inputs.txt";
	FILE *non_inputs_file_fp = fopen(non_inputs_file.c_str(), "w+");
	assert(non_inputs_file_fp != NULL);
	printSet(non_inputs_file_fp, vregisters);
	fclose(non_inputs_file_fp);	
}


// For pretty-printing in Verilog files
void printSetOnLines(FILE* verilog_fp, set<string> &myset, int per_line, int spaces)
{
	int set_index = 0;
	int set_size = myset.size();
	for(set<string>::iterator set_it = myset.begin(); set_it != myset.end(); set_it++)
	{
		string signal = *set_it;

		if(set_index == 0)
		{
			// do nothing				
		}
		else if(set_index % per_line == 0)
		{
			fprintf(verilog_fp,"\n");

			for(int i = 0; i < spaces; i++)
			{
				fprintf(verilog_fp," ");	
			}
		}
		
		if(set_index == set_size - 1)
			fprintf(verilog_fp,"%s ", signal.c_str()); 
		else
			fprintf(verilog_fp,"%s, ", signal.c_str()); 			

		set_index++;
	}
}



// Function to simplify transition function and put it in a format so that printing in Verilog is easy
DAGNode*  simplifyTransitionFunction(DAGNode *node)
{
    
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  DAGNode *ret = simplifyTransitionFunctionInternal(node, PtrDPTable);
  return ret;
}
  
  

// Function called by simplifyTransitionFunction
DAGNode*  simplifyTransitionFunctionInternal(DAGNode *node, HashTable<string, DAGNode*> *DPTable)
{
  DAGNode* ret;
  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char;

 

  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }

  // hash table miss
  unsigned int outlist_size = node->Get_outlist_size();
  
  if(outlist_size == 0) { // Terminals
	    string id_delta_bit = node->Get_node_name();
	    Value* MyValue = (Value*)(node->getValue());
	    Type MyType = MyValue->get_node_type();
	    int MyWidth = MyType.width;
	    int MyDataType = MyType.data_type;

	    if(MyDataType == LOGICAL_SYMBOL) // bit-level variable
	      {
		int index = id_delta_bit.find_last_of("_");
		assert(index != string::npos);
		string id_delta = id_delta_bit.substr(0, index);
		string bit_str = id_delta_bit.substr(index+1);	

		index = id_delta.find_last_of("_");
		assert(index != string::npos);
		string id = id_delta.substr(0, index);
		string delta_str = id_delta.substr(index+1);	

		string new_name = id;
		new_name += "_";
		new_name += bit_str; // Avoid the time stamp: not needed

		ret = createLogicalVariable(new_name);
	      }
	    else
	      {
		ret = node;
	      }
  }
  else // Non-terminals
  {
    string dag_str = node->Get_node_name();
    
    vector<DAGNode*> SimplifiedChildren;
    list<DAGNode*>::iterator iout = node->Get_outlist_start();
    

    for(unsigned int i=0;i<outlist_size; i++,iout++)
	{
	    SimplifiedChildren.push_back(simplifyTransitionFunctionInternal(*iout, DPTable));
	}

    if(dag_str == "exor" && outlist_size == 2){// binary exor

	  if((SimplifiedChildren[0])->Get_node_name()=="true")

	    ret = createDAG("not", SimplifiedChildren[1], LOGICAL_SYMBOL, 1) ;

	  else if((SimplifiedChildren[1])->Get_node_name()=="true")

	    ret = createDAG("not", SimplifiedChildren[0], LOGICAL_SYMBOL, 1) ;

	  else if((SimplifiedChildren[0])->Get_node_name()=="false")

	    ret = SimplifiedChildren[1];

	  else if((SimplifiedChildren[1])->Get_node_name()=="false")

	    ret = SimplifiedChildren[0];

	  else

	    ret = createDAG("exor", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1) ;
	}

    else if(dag_str == "if_then_else"){

            DAGNode* conjunct_1 = createDAG("and", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1);

	    DAGNode* conjunct_2 = createDAG("and", createDAG("not", SimplifiedChildren[0], LOGICAL_SYMBOL, 1), SimplifiedChildren[2], LOGICAL_SYMBOL, 1);

	    ret = createDAG("or", conjunct_1, conjunct_2, LOGICAL_SYMBOL, 1);

	}

   else if(dag_str == "="){

	  if((SimplifiedChildren[0])->Get_node_name()=="false")

	    ret = createDAG("not", SimplifiedChildren[1], LOGICAL_SYMBOL, 1) ;

	  else if((SimplifiedChildren[1])->Get_node_name()=="false")

	    ret = createDAG("not", SimplifiedChildren[0], LOGICAL_SYMBOL, 1) ;

	  else if((SimplifiedChildren[0])->Get_node_name()=="true")

	    ret = SimplifiedChildren[1];

	  else if((SimplifiedChildren[1])->Get_node_name()=="true")

	    ret = SimplifiedChildren[0];

	  else

	    ret = createDAG("=", SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1) ;
	}

   else if(outlist_size == 1)
        {
           ret = createDAG(dag_str, SimplifiedChildren[0], LOGICAL_SYMBOL, 1); 
        }

   else if(outlist_size == 2)
        {
           ret = createDAG(dag_str, SimplifiedChildren[0], SimplifiedChildren[1], LOGICAL_SYMBOL, 1); 
        }

   else if(outlist_size == 3)
        {
           ret = createDAG(dag_str, SimplifiedChildren[0], SimplifiedChildren[1], SimplifiedChildren[2], LOGICAL_SYMBOL, 1); 
        }
   else
	{
	    cout<<"\nUnknown node " << dag_str<<" node encountered in simplifyTransitionFunctionInternal\n";
	    exit(1);
	}

  } // Non-terminals end here

  result = DPTable->hashtable_insert(key, ret);

  if(!result.success())
  {

      cout<<"\nHash Insert Failure Inside Function simplifyTransitionFunctionInternal\n";
      exit(1);
  }

  return ret;
}


// Function to print the dag "root" in Verilog format
string printBinaryDAGInVerilogInternal(DAGNode* node, map<DAGNode*, string> &LabelTable, FILE *fp, int &TempCount, set<string> &vwires)
{
  string label;
  string name = node->Get_node_name();
  unsigned int outlist_size = node->Get_outlist_size();
  Value* MyValue = (Value*)(node->getValue());
  Type MyType = MyValue->get_node_type();
  int MyWidth = MyType.width;
  int node_type = MyType.data_type;
  list<DAGNode*>::iterator iout = node->Get_outlist_start();

  //cout<<"\nname=\t"<<name<<endl;    //debug code

  if(node_type == LOGICAL_CONSTANT) // constants 
    {
      if(name == "true") label = "1'b1";
      else if(name == "false") label = "1'b0";
      else assert(false);
      return label;
    }
  else // variable or operator
    {
      map<DAGNode*, string>::iterator it=LabelTable.find(node);
      //cout<<"\nnode = "<<node<<", label = "<<label<<endl;
      if(it!=LabelTable.end()) // entry exists for the node
	{
	  //cout<<"\nentry exists for "<<node<<endl;
	  label = it->second;
	  return label;
	}
      else
	{
	  if(outlist_size == 0)  // variable
	    {
	      label = name; // label obtained
	      return label;
	    }
	  else // operator
	    {
	      //cout<<"\nname=\t"<<name<<endl;    //debug code

	     if(name == "and" || name == "or" || name == "not" || name == "exor" || name == "=")
		{
	 
		  vector<string> ChildrenLabel;
		  for(unsigned int i=0; i<outlist_size; i++, iout++)
		    {
		      ChildrenLabel.push_back(printBinaryDAGInVerilogInternal(*iout, LabelTable, fp, TempCount, vwires));
		    }

		  TempCount = TempCount+1;
		  char temp_char[10];
		  sprintf(temp_char, "%d", TempCount);
		  string temp_string = temp_char;
		  label = "n";
		  label += temp_string; // label obtained
		  vwires.insert(label);
	  
		  if(name == "not")
		    {
		      assert(ChildrenLabel.size() == 1);

		      fprintf(fp,"  assign %s = ~%s;\n",label.c_str(), ChildrenLabel[0].c_str()); // written
		      
		    }
                  else if(name == "and" || name == "or" || name == "exor") // and, or, exor node
		    {
		      assert(ChildrenLabel.size() == 2);
				
                      string operator_name;
		      if(name == "and") operator_name = "&";
		      else if(name == "or") operator_name = "|";
                      else operator_name = "^";

		      fprintf(fp,"  assign %s = %s %s %s;\n",label.c_str(), ChildrenLabel[0].c_str(), operator_name.c_str(), ChildrenLabel[1].c_str()); // written                      
		    }// and, or, exor node ends here
		  else // =
		    {
		      assert(ChildrenLabel.size() == 2);
				
                      fprintf(fp,"  assign %s = ~(%s ^ %s);\n",label.c_str(), ChildrenLabel[0].c_str(), ChildrenLabel[1].c_str()); // written 
                    }

		  LabelTable.insert(make_pair(node, label));

		  return label;
		}
	      else
		{
		  cout<<"\nUnanticipated operator "<<name<<" inside function printBinaryDAGInVerilogInternal in Eliminator.cpp\n";
		  exit(1);
		}
		    
	    }// operator
	}//no entry in hash table
    }// variable or operator
}//function ends here


// For printing in file
void printSet(FILE* verilog_fp, set<string> &myset)
{
	for(set<string>::iterator set_it = myset.begin(); set_it != myset.end(); set_it++)
	{
		string signal = *set_it;
		fprintf(verilog_fp,"%s\n", signal.c_str());
	}
}


// Function to simplify property assuming that changes are true
DAGNode*  simplifyProperty(DAGNode *node)
{
    
  HashTable<string, DAGNode*> DPTable;
  HashTable<string, DAGNode*> *PtrDPTable=&DPTable;
  DAGNode *ret = simplifyProperty_recur(node, PtrDPTable);
  return ret;
}
  

// Internal recursive function called by simplifyProperty
DAGNode* simplifyProperty_recur(DAGNode* node, HashTable<string, DAGNode*> *DPTable)
{

  string key;
  char temp_char[10];
  sprintf(temp_char , "%x", node);
  key = temp_char; 

  HTStatusValue<DAGNode*> result = DPTable->hashtable_search(key);

  if(result.success())
    {
      //cout<<"\nCache Hit for node "<<node<<"\n";
      return result.GetValue();
    }

  // hash table miss
  DAGNode* clone;
  // Get the size of outlist of node
  unsigned int outlist_size = node->Get_outlist_size();
	
                   
  if(outlist_size>0)  //i.e. if an operator node
  {
            list<DAGNode*>::iterator iout = node->Get_outlist_start();

            switch(outlist_size)
            {
                case 1:
                {
                    DAGNode* child_1 ;

		    child_1 = simplifyProperty_recur(*iout, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    clone = dm.createNode(node->Get_node_name(),Children, node->getValue());
                    break;
                }
                case 2:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    
                    child_1 = simplifyProperty_recur(*iout, DPTable);
		    iout++;
                    child_2 = simplifyProperty_recur(*iout, DPTable);

		    vector<DAGNode *> Children;
		    Children.push_back(child_1);
		    Children.push_back(child_2);
		    clone = dm.createNode(node->Get_node_name(),Children, node->getValue());
                    break;
                }
                case 3:
                {
                    DAGNode* child_1 ;
                    DAGNode* child_2 ;
                    DAGNode* child_3 ;

		    if(node->Get_node_name() == "ite")
		    {
				char Condn = evaluateChangeCondition(*iout);
				if(Condn == 't') //Condn is true
				  {
				    iout++;
				    child_2 = simplifyProperty_recur(*iout, DPTable);
				    clone = child_2;		
				  }
				else // We are not sure what the Condn is
				  {
				    child_1 = simplifyProperty_recur(*iout, DPTable);
				    iout++;
				    child_2 = simplifyProperty_recur(*iout, DPTable);
				    iout++;
				    child_3 = simplifyProperty_recur(*iout, DPTable);

				    vector<DAGNode *> Children;
				    Children.push_back(child_1);
				    Children.push_back(child_2);
				    Children.push_back(child_3);
				    clone = dm.createNode(node->Get_node_name(),Children, node->getValue());
				  }
		    }	
		    else
		    {
		            child_1 = simplifyProperty_recur(*iout, DPTable);
			    iout++;
		            child_2 = simplifyProperty_recur(*iout, DPTable);
			    iout++;
		            child_3 = simplifyProperty_recur(*iout, DPTable);

			    vector<DAGNode *> Children;
			    Children.push_back(child_1);
			    Children.push_back(child_2);
			    Children.push_back(child_3);
			    clone = dm.createNode(node->Get_node_name(),Children, node->getValue());
		    }

                    break;
                }
	      default:
		{
		  cout<<"\nNode with oulist size other than 1 or 2 or 3 encountered in function simplifyProperty_recur in Eliminator.cpp\n";
		 exit(1);
		}

	    }
   }
  else // leaf node
  {
	    
		clone = dm.createNode(node->Get_node_name(), node->getValue());
            
   }

  result = DPTable->hashtable_insert(key, clone);

  if(!result.success())
  {

      cout<<"\nHash Insert Failure Inside Function simplifyProperty_recur in Eliminator.cpp\n";
      exit(1);
  }

    return clone;
}    


// Function that asserts change in signals to true
char evaluateChangeCondition(DAGNode* node)
{
  string dag_str = node->Get_node_name();
  //cout<<"\ndag_str = "<<dag_str<<endl;

  if(dag_str == "not")
    {
      //cout<<"\nInside not\t";
      list<DAGNode*>::iterator iout = node->Get_outlist_start();
      char child1 =  evaluateChangeCondition(*iout);
      if(child1 == 'f') 
	{
		//cout<<"return t from not\n";
		return 't';
	}
      else 
	{
		//cout<<"return u from not\n";
		return 'u';
	}
    }
  else if(dag_str == "and")
    {
      //cout<<"\nInside and\t";
      list<DAGNode*>::iterator iout = node->Get_outlist_start();
      char child1 =  evaluateChangeCondition(*iout);
      iout++;
      char child2 =  evaluateChangeCondition(*iout);
      if(child1 == 't' && child2 == 't') 
	{
		//cout<<"return t from and\n";
		return 't';
	}
      else 
	{
		//cout<<"return u from and\n";
		return 'u';
	}
    }
  else if(dag_str == "or")
    {
      //cout<<"\nInside or\t";
      list<DAGNode*>::iterator iout = node->Get_outlist_start();
      char child1 =  evaluateChangeCondition(*iout);
      iout++;
      char child2 =  evaluateChangeCondition(*iout);
      if(child1 == 't' || child2 == 't') 
	{
		//cout<<"return t from or\n";
		return 't';
	}
      else 
        {
		//cout<<"return u from or\n";
		return 'u';
	}
    }
  else if(dag_str == "=")
    {
      //cout<<"\nInside equivalent\t"; 
     list<DAGNode*>::iterator iout = node->Get_outlist_start();
     
     DAGNode *child1 = *iout;
     iout++;
     DAGNode *child2 = *iout;

     if(getNodeType(child1).data_type == BITVECTOR_SYMBOL && getNodeType(child2).data_type == BITVECTOR_SYMBOL)
     {
      	string child1_string = child1->Get_node_name();
        string child2_string = child2->Get_node_name();
	
	int u_location = child1_string.find_last_of("_");
	string child1_name = child1_string.substr(0, u_location);
	string child1_delta_str = child1_string.substr(u_location+1);
	int child1_delta = atoi(child1_delta_str.c_str());

	u_location = child2_string.find_last_of("_");
	string child2_name = child2_string.substr(0, u_location);
	string child2_delta_str = child2_string.substr(u_location+1);
	int child2_delta = atoi(child2_delta_str.c_str());

	if(child1_name == child2_name && ((child1_delta == 0 && child2_delta == 1) || (child1_delta == 1 && child2_delta == 0))) // we have x_0 = x_1 which is false
	{
		//cout<<"return f from equivalent\n";
		return 'f';
	}
	else
	{
		//cout<<"return u from equivalent\n";
		return 'u';
	}
      }
     else
      {
	 //cout<<"return u from equivalent\n";
         return 'u';
      }
    }
  else
    {
      //cout<<"\nSome other node\treturn u from some other node\n";
      return 'u';
    }
}
     


// Version of splitConjunctionIntoVectorGeneral with support for not node
void splitConjunctionIntoVectorGeneralWithSupportForNot(DAGNode* Constraint, vector<DAGNode*> &Conjuncts)
{
  string MyName = Constraint->Get_node_name();

  if(MyName == "bvule" || MyName == "bvuge" || MyName == "bvult" || MyName == "bvugt" || MyName == "=" || MyName == "is_not_equal" ) // terminal reached
    {
      Conjuncts.push_back(Constraint);
    }
  else if(MyName == "not") // terminal (not (op lhs rhs)) reached; push not inside
    {
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();
      DAGNode *child = *iout;
      string ChildName = child->Get_node_name();
      string NegatedChildName;
	
      if(ChildName == "bvule") NegatedChildName = "bvugt";
      else if(ChildName == "bvuge") NegatedChildName = "bvult";
      else if(ChildName == "bvult") NegatedChildName = "bvuge";
      else if(ChildName == "bvugt") NegatedChildName = "bvule";	
      else if(ChildName == "=") NegatedChildName = "is_not_equal";
      else 
	{
	  cout<<"\nErraneous not node in function splitConjunctionIntoVectorGeneralWithSupportForNot in Eliminator.cpp\n";
	  assert(false);
	}	

      iout = child->Get_outlist_start();
      DAGNode *lhs = *iout;
      iout++;
      DAGNode *rhs = *iout;
      
      DAGNode *NewConstraint = createDAG(NegatedChildName, lhs, rhs, LOGICAL_SYMBOL, 1);
      Conjuncts.push_back(NewConstraint);
    }
  else if(MyName == "and") // root is "and" . hence recurse on the children
    {
      if(Constraint->Get_outlist_size()!=2)
	{
	  cout<<"\nand node with number of children not equal to two encountered in function splitConjunctionIntoVectorGeneralWithSupportForNot in Eliminator.cpp\n";
	  exit(1);
	}
          
      list<DAGNode*>::iterator iout = Constraint->Get_outlist_start();

      splitConjunctionIntoVectorGeneralWithSupportForNot(*iout, Conjuncts);
      
      iout++;

      splitConjunctionIntoVectorGeneralWithSupportForNot(*iout, Conjuncts);
    }
  else if(MyName != "true")
    {
     cout<<"\nunkwnon node "<<MyName<<" encountered in function splitConjunctionIntoVectorGeneralWithSupportForNot in Eliminator.cpp\n";
     exit(1); 
    }
}




// Print the given formula in SMTLIB format
void printQEliminatedResultInSMTLibFormatInGivenFile(DAGNode *root, map<string, int> &WidthTable, string file_name, string benchmark_name)
{
  //cout<<"\nPrinting result in "<<file_name<<endl;

  FILE *smt_file = fopen(file_name.c_str(), "w");

  if(smt_file == NULL)
    {cout<<"\nCannot create "<<file_name<<"\n"; exit(1);}

  fprintf(smt_file, "(benchmark %s\n", benchmark_name.c_str());

  set<string> Vars;
  getDAGNames(root, Vars);

  writeBVDeclarationsInSMTLibFormatForSelectedVars(smt_file, WidthTable, Vars);

 
  string dag_str;
  
  if(LMI_handling_enabled)
  {
   dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable_WithSupportForLMIs(root, WidthTable);
   }
  else
  {
  dag_str = callgetDAGOfProperSkeltonFormatInSMTLibFormatWithWidthTable(root, WidthTable);
  }

  fprintf(smt_file, "\n:formula %s\n", dag_str.c_str());
  fprintf(smt_file, "\n)\n");
  fclose(smt_file);
  
  }

