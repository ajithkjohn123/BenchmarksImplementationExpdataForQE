#include "NewEliminator.h"

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

// Show the matrix
void showMatrix(Matrix MyMatrix)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();
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
void showEquations(Matrix MyCoefMatrix, Matrix MyConstMatrix)
{
int rows = MyCoefMatrix.size();
int cols = MyCoefMatrix[0].size();
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
void showDisEquations(Matrix MyCoefMatrix, Matrix MyConstMatrix) 
{
int rows = MyCoefMatrix.size();
int cols = MyCoefMatrix[0].size();
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
void printEquations(Matrix MyCoefMatrix, Matrix MyConstMatrix, FILE *fp)
{
int rows = MyCoefMatrix.size();
int cols = MyCoefMatrix[0].size();
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
void printDisEquations(Matrix MyCoefMatrix, Matrix MyConstMatrix, FILE *fp) 
{
int rows = MyCoefMatrix.size();
int cols = MyCoefMatrix[0].size();
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


// Get two matrices from key board : coefficient and constant matrices
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

		cin>>const1>>const2;
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
void getEquationWithOddCoefForGivenVariable(Matrix EqCoefMat, int VarToElimIndex, int &EqnToSubstIndex)
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
void getEquationWithLeastEvenCoefForGivenVariable(Matrix EqCoefMat, int VarToElimIndex, int &EqnToSubstIndex, ULINT Mod)
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
int cols = A[0].size();

for(int i=0; i<rows; i++)
		{
		  A[i][SelColumn] = 0;
		}
}


// Divide column indexed by  SelColumn by power(2,SelectedPower) if divisible. If not divisible set to zero
void divideColumnByPowerOf2IfDivisible(Matrix &EqCoefMat, int SelColumn, ULINT SelectedPower)
{
int rows = EqCoefMat.size();
int cols = EqCoefMat[0].size();

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
void copyRow(Matrix MyMatrix, int selectedIndex, Matrix &NewMatrix)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();

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
void copyColumn(Matrix MyMatrix, int selectedIndex, Matrix &MyColumnMatrix)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();

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
void copyMatrix(Matrix MyMatrix, Matrix &MyMatrixCopy)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();
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
int cols = MyMatrix[0].size();

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      MyMatrix[i][j]=aInverse(MyMatrix[i][j], Mod);
}

// Convert MyMatrix to MyMatrix*Scalar
void scalarMatMultiply(Matrix &MyMatrix, ULINT Scalar, ULINT Mod)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      MyMatrix[i][j]=mul(MyMatrix[i][j], Scalar, Mod);
}

// Multiply matrixes A and B to get C
void matMultiply(Matrix A, Matrix B, Matrix &C, ULINT Mod)
{
int RowsA = A.size();
int ColsA = A[0].size();
int RowsB = B.size();
int ColsB = B[0].size();

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
void matAdd(Matrix &A, Matrix B, ULINT Mod)
{
int rows = A.size();
int cols = A[0].size();
  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      A[i][j]=add(A[i][j], B[i][j], Mod);
}


// A = A - B where A, B are matrixes
void matSub(Matrix &A, Matrix B, ULINT Mod)
{
int rows = A.size();
int cols = A[0].size();
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
int cols = A[0].size();

int i;
// delete elements A[i][Index] where i=0..(rows-1)
for(i=0;i<rows;i++)
	A[i].erase(A[i].begin()+Index);
}

// Add RowMatrix as a new row into MyMatrix
void addRow(Matrix &MyMatrix, Matrix RowMatrix)
{
  Row MyRow = RowMatrix[0];
  MyMatrix.push_back(MyRow);
}

// Add all the rows in YourMatrix as a new rows into MyMatrix
void addRows(Matrix &MyMatrix, Matrix YourMatrix)
{
  int rows = YourMatrix.size();
  for(int i=0;i<rows;i++)
    {
      Row YourRow = YourMatrix[i];
      MyMatrix.push_back(YourRow);
    }
}

// Here MyMatrix is a column matrix. This function adds element as the last elemnt of MyMatrix
void addElementToColumnMatrix(Matrix &MyMatrix, ULINT element)
{
  Row MyRow;
  MyRow.push_back(element);
  MyMatrix.push_back(MyRow);
}

// Check if MyMatrix is a null matrix
int IsNullMatrix(Matrix MyMatrix)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();

  int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      {
	if(MyMatrix[i][j]!=0) return 0;
      }
  return 1;
}

// Get the matrix which is the element-wise maximum of matrices in arg1 and arg2
void getMaximumMatrix(Matrix A, Matrix B, Matrix &C)
{
int RowsA = A.size();
int ColsA = A[0].size();
int RowsB = B.size();
int ColsB = B[0].size();

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

void convertMatrixToDifferenceOfPowerOf2(Matrix A, ULINT mod, Matrix &B)
{
int rows = A.size();
int cols = A[0].size();

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
      


// Find GCD(set<ULINT> numbers)
ULINT findGCD(set<ULINT> numbers)
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
int equationSolvable(Matrix MyMatrix, ULINT Const, ULINT Mod)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();


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
bool columnIsNull(Matrix A,int index)
{
int rows = A.size();
int cols = A[0].size();
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
int searchVector(vector<int> MyVector, int element)
{
int size = MyVector.size();

for(int i=0;i<size;i++)
	if(MyVector[i]==element) return i;
return -1;
}


void showVector(vector<int> MyVector)
{
int size = MyVector.size();
 cout<<"\n";
for(int i=0;i<size;i++)
  cout<<MyVector[i]<<"\t";
 cout<<"\n";
}


//  Function to copy the the non-zero elements in the selectedIndexth column into MyMap. Initially MyMap is empty. 
void copyColumnToMap(Matrix MyMatrix, int selectedIndex, map<int, ULINT> &MyMap)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();

ULINT element;
if(selectedIndex<0 || selectedIndex>=cols) {cout<<"\nInvalid column "<<selectedIndex<<" in function copyColumnToMap\n"; exit(1);}
for(int i=0; i<rows; i++)
			{
			element=MyMatrix[i][selectedIndex];
			if(element != 0) 
			  {MyMap.insert(make_pair(i, element));}
			}
}

// Function to convert the mapped elements of MyMap to powers of 2
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
ULINT getMaximumOfMappedElement(map<int, ULINT> MyMap)
{
  ULINT maximum=0;
for(map<int, ULINT>::iterator it=MyMap.begin(); it != MyMap.end(); it++)
  {if(it->second>maximum)  {maximum=it->second;}}
 return maximum; 
}

// Function to find the minimum of the mapped elements of MyMap
ULINT getMinimumOfMappedElement(map<int, ULINT> MyMap)
{
  map<int, ULINT>::iterator it=MyMap.begin();
  ULINT minimum=it->second;
  for(; it != MyMap.end(); it++)
    {if(it->second<minimum)  {minimum=it->second;} }
 return minimum; 
}


// Function to show MyMap
int showMap(map<int, ULINT> MyMap)
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


void dropUnconstrainingDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod) 
{

  // Take variables to be eliminated one by one. For each variable, eliminate the disequations which can be eliminated

for(int VarToElimIndex=0; VarToElimIndex<NoVarsToElim; 	VarToElimIndex++)
	{
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

	// Get the number of bits to play with for equations and disequations
	
	subtractMappedElementsFromNumber(CoefsOfVarToElimEqualities, powerOfTwo(Mod));
	subtractMappedElementsFromNumber(CoefsOfVarToElimDisEqualities, powerOfTwo(Mod));

	//cout<<"\nNo:of bits to play with\n";
	//cout<<"\nCoefsOfVarToElimEqualities\n";
	//showMap(CoefsOfVarToElimEqualities);
	//cout<<"\nCoefsOfVarToElimDisEqualities\n";
	//showMap(CoefsOfVarToElimDisEqualities);


	// The powers in equalities are less than that in disequalities
	// Now we need to subtract the no: of bits with which the equations are playing from the no: of bits with which disequations are playing. For this, find the maximum no: of bits with which the equations are playing.

	if(!CoefsOfVarToElimDisEqualities.empty())  // if variable is not present in equalities, then no need to subtract
	  {
	    ULINT MaxNoOfBitsToPlayWithForEquations = getMaximumOfMappedElement(CoefsOfVarToElimEqualities);
	    //cout<<"\nMaxNoOfBitsToPlayWithForEquations = "<<MaxNoOfBitsToPlayWithForEquations<<"\n";
	    subtractNumberFromMappedElements(CoefsOfVarToElimDisEqualities, MaxNoOfBitsToPlayWithForEquations);
	  }
	
	// Now CoefsOfVarToElimDisEqualities gives the no: of bits to play with for disequations

	//cout<<"\nNo:of bits to play with for disequations after subtracting bits played by equations\n";
	//cout<<"\nCoefsOfVarToElimDisEqualities\n";
	//showMap(CoefsOfVarToElimDisEqualities);
		
	// Eliminate the disequalities looking at the bits to play with

	eliminateDisequalities(CoefsOfVarToElimDisEqualities, DisEqCoefMat, DisEqConstMat);
	
	//cout<<"\nShowing the Disequalities\n";
	//showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

	}

  
}

bool isSetOfDisEquationsExtendable(map<int, ULINT> NoOfBitsToPlayWithForDisEquations)
{
  ULINT MaxNoOfBitsToPlayWith = getMaximumOfMappedElement(NoOfBitsToPlayWithForDisEquations);
  ULINT NoOfCombinationsAllowed =  findPower(MaxNoOfBitsToPlayWith);
  ULINT NoOfCombinationsAsked = 0;

  subtractMappedElementsFromNumber(NoOfBitsToPlayWithForDisEquations, MaxNoOfBitsToPlayWith);
  
  for(map<int, ULINT>::iterator it=NoOfBitsToPlayWithForDisEquations.begin(); it != NoOfBitsToPlayWithForDisEquations.end(); it++)
    NoOfCombinationsAsked += findPower(it->second);

  //cout<<"\nNoOfCombinationsAsked = "<<NoOfCombinationsAsked<<" NoOfCombinationsAllowed = "<<NoOfCombinationsAllowed<<"\n";

  if(NoOfCombinationsAsked >= NoOfCombinationsAllowed) return false;
  else return true;
}
  

void eliminateDisequalities(map<int, ULINT> &NoOfBitsToPlayWithForDisEquations, Matrix &DisEqCoefMat, Matrix &DisEqConstMat)
{
 list<int> DisequationsToEliminate;
  
  while(!NoOfBitsToPlayWithForDisEquations.empty())
    {
      if(isSetOfDisEquationsExtendable(NoOfBitsToPlayWithForDisEquations)) break;

      // Eliminate the most constrained disequations

      //cout<<"\nEliminating the most constrained disequations\n";

      ULINT MinNoOfBitsToPlayWith = getMinimumOfMappedElement(NoOfBitsToPlayWithForDisEquations);
      subtractNumberFromMappedElements(NoOfBitsToPlayWithForDisEquations, MinNoOfBitsToPlayWith);
      eliminateEntriesWithZeroMappedElements(NoOfBitsToPlayWithForDisEquations);
    }

  // NoOfBitsToPlayWithForDisEquations contains the disequations which can be eliminated
  for(map<int, ULINT>::iterator it=NoOfBitsToPlayWithForDisEquations.begin(); it != NoOfBitsToPlayWithForDisEquations.end(); it++) {DisequationsToEliminate.push_back(it->first);}

  // Eliminate them in reverse order so that after each elimination, the row indexes are intact.

   DisequationsToEliminate.reverse();
  
  for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
    {
      //cout<<"\nDisequation "<<*it<<" can be eliminated\n";
      deleteRow(DisEqCoefMat, *it);
      deleteRow(DisEqConstMat, *it);
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


void eliminateDisequationsFreeOfVarToElimAndCopyInNewMatrix(Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int VarToElimIndex, Matrix &DisEqFreeOfVarToElimCoefMat, Matrix &DisEqFreeOfVarToElimConstMat)
{
  list<int> FreeDiseqs;
 for(int i=0; i<DisEqCoefMat.size(); i++)
		{
		  if(DisEqCoefMat[i][VarToElimIndex] == 0) // Disequation i free of variable VarToElimIndex
		    FreeDiseqs.push_back(i);
		}
 
 FreeDiseqs.reverse();

 for(list<int>::iterator it=FreeDiseqs.begin(); it != FreeDiseqs.end(); it++)
   {
     copyRow(DisEqCoefMat, *it, DisEqFreeOfVarToElimCoefMat); // copyRow does not depend on whether DisEqFreeOfVarToElimCoefMat is row matrix or if it is empty
     deleteRow(DisEqCoefMat, *it);
     
     copyRow(DisEqConstMat, *it, DisEqFreeOfVarToElimConstMat); // copyRow does not depend on whether DisEqFreeOfVarToElimConstMat is row matrix or if it is empty
     deleteRow(DisEqConstMat, *it);
   }
}


void splitDisequations(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, FILE *fp)
{
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

cout<<"\nAfter eliminating free disequations\n";

cout<<"\nEquation with least coefficient is\n";
cout<<"\nEqnCoef\n";
showMatrix(EqnCoef);
cout<<"\nEqnConst\n";
showMatrix(EqnConst);

cout<<"\nShowing the Disequalities Free of the variable\n";
cout<<"\nDisEqFreeOfVarToElimCoefMat\n";
showMatrix(DisEqFreeOfVarToElimCoefMat);
cout<<"\nDisEqFreeOfVarToElimConstMat\n";
showMatrix(DisEqFreeOfVarToElimConstMat);


//cout<<"\nShowing the Equalities\n";
//showEquations(EqCoefMat, EqConstMat, RemainingVariablesInEqualities); // Show the equations
cout<<"\nShowing the Disequalities\n";
showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations



// Find the power of 2 of the variable to be eliminated in the equation selected
ULINT PivotElement=EqnCoef[0][VarToElimIndex]; 
cout<<"\nPivot Element = "<<PivotElement<<"\n";
ULINT PowerOfPivotElement=powerOfTwo(PivotElement);
cout<<"\nPower Of Pivot Element = "<<PowerOfPivotElement<<"\n";


// Split each of disequations in DisEqCoefMat <> DisEqConstMat using the equation EqnCoef = EqnConst into two constraints
// Use a counter to generate all the possible combinations of splitted parts

 ULINT max=findPower(DisEqCoefMat.size())-1; // We have 2^(#disequations) combinations possible

 cout<<"\nMax no: iterations = "<<max<<"\n";

 fprintf(fp,"or(");

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
		  cout<<"\nWhichConstraint = "<<WhichConstraint<<"\n";
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
    
     
     cout<<"\nShowing the Equalities\n";
     showEquations(NewEqCoefMat, NewEqConstMat); // Show the equations
     cout<<"\nShowing the Disequalities\n";
     showDisEquations(NewDisEqCoefMat, NewDisEqConstMat); // Show the disequations

     cout<<"\nCalling qEliminator recursively\n";

     qEliminator(NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat, NoVarsToElim, Mod, SplitFlag, fp);
     fprintf(fp,", ");	

   }
   fprintf(fp,")");	
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

   
// This is the function which eliminates the variables

void qEliminator(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int NoVarsToElim, ULINT Mod, bool SplitFlag, FILE *fp)
{
  Matrix OriginalEqCoefMat, OriginalEqConstMat; // Copy of equalities

  // Keep a copy of equations in OriginalEqCoefMat, OriginalEqConstMat
  copyMatrix(EqCoefMat, OriginalEqCoefMat);
  copyMatrix(EqConstMat, OriginalEqConstMat);
  
  cout<<"\nEqualities and Disequalities at the beginning of the function\n";

  cout<<"\nShowing the Equalities\n";
  showEquations(EqCoefMat, EqConstMat); // Show the equations
  cout<<"\nShowing the Disequalities\n";
  showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  

  //cout<<"\nEliminating variables from Equations and Disequations\n";
  
  if(!EqCoefMat.empty())
    qEliminatorForEquationsAndDisequations(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod); // Eliminate first NoVarsToElim variables from the disequations CX<>D and equations AX=B and get back the matrices

  //cout<<"\nShowing the Equalities\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nShowing the Disequalities\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations


  //cout<<"\nDropping the unconstraining disequations\n";
  // Note that for dropping unconstraining disequations, we should look at the maximum no: of bits constrained by any equality. The only thing we want from the set of equations in fact is the most constraining equality (Look at the proof for further clarification)

  if(!DisEqCoefMat.empty())
    dropUnconstrainingDisequations(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);

  //cout<<"\nShowing the Equalities\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nShowing the Disequalities\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations


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
		     cout<<"\nVariable x"<<VarToElimIndex<<" is present in disequations\n";
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

  //cout<<"\nWriting to the file\n";
  fprintf(fp,"and(");
  printEquations(EqCoefMat, EqConstMat, fp); 
  printDisEquations(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat, fp); 
  fprintf(fp,"\n");
	
  if(EliminationDone) 
    {
      cout<<"\nElimination done. Exiting from this call...\n";
      fprintf(fp,")");
      return;}

  // Check if all the disequations are already split

  if(SplitFlag) {cout<<"\nBit-blasting needed. Exiting...\n"; fprintf(fp," Bit-blasting)"); return;}
  
  if(!SplitFlag) // Disequations are not split already
    {
      cout<<"\nSplitting\n";
      
      SplitFlag=true;

      splitDisequations(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, VarToElimIndex, Mod, NoVarsToElim, SplitFlag, fp);
	
      fprintf(fp,")");
    }
  
}




// Main function to do quantifier elimination for equations and disequations

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
	if(!equationSolvable(PivotRow, PivotCoef, Mod))
	  {cout<<"\nThe equation selected is unsolvable. Hence set of equations is unsat. Exiting...\n"; exit(1);}

	
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


// This is the function which eliminates the variables


CDAGNode* qEliminatorReturningDAG(Matrix EqCoefMatPassed, Matrix EqConstMatPassed, Matrix DisEqCoefMatPassed, Matrix DisEqConstMatPassed, int NoVarsToElim, ULINT Mod, bool SplitFlag, CHelper &MANAGER, list<string> VarNames, map<string, int> WidthTable)
{
  // Delete the duplicate contraints

  Matrix EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat;

  //cout<<"\nShowing original set of constraints\n";
  //showEquations(EqCoefMatPassed, EqConstMatPassed); // Show the equations
  //showDisEquations(DisEqCoefMatPassed, DisEqConstMatPassed); // Show the disequations


  deleteDuplicateConstraints(EqCoefMatPassed, EqConstMatPassed, EqCoefMat, EqConstMat);
  deleteDuplicateConstraints(DisEqCoefMatPassed, DisEqConstMatPassed, DisEqCoefMat, DisEqConstMat);

  
  //cout<<"\nShowing duplicate eliminated set of constraints\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations




  CDAGNode *resultDAG=NULL;

  Matrix OriginalEqCoefMat, OriginalEqConstMat; // Copy of equalities

  // Keep a copy of equations in OriginalEqCoefMat, OriginalEqConstMat
  copyMatrix(EqCoefMat, OriginalEqCoefMat);
  copyMatrix(EqConstMat, OriginalEqConstMat);
  
  //cout<<"\n************************************************************\n";

  //cout<<"\nEqualities and Disequalities at the beginning of the function\n";

  //cout<<"\nShowing the Equalities\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nShowing the Disequalities\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  

  cout<<"\nEliminating variables from Equations and Disequations\n";
  
  if(!EqCoefMat.empty())
    qEliminatorForEquationsAndDisequations(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod); // Eliminate first NoVarsToElim variables from the disequations CX<>D and equations AX=B and get back the matrices

  //cout<<"\nWe are left with the following constraints after elimination of variables through LMA\n";

  //cout<<"\nShowing the Equalities\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nShowing the Disequalities\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations


  cout<<"\nDropping the unconstraining disequations\n";
  // Note that for dropping unconstraining disequations, we should look at the maximum no: of bits constrained by any equality. The only thing we want from the set of equations in fact is the most constraining equality (Look at the proof for further clarification)

  int DropLMDs=2;
  if(!DisEqCoefMat.empty())
    if(DropLMDs==1)
      dropUnconstrainingDisequations(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);
    else
      dropUnconstrainingDisequationsModified(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);

  //cout<<"\nAfter dropping the unconstraining disequations\n";
  //cout<<"\nShowing the Equalities\n";
  //showEquations(EqCoefMat, EqConstMat); // Show the equations
  //cout<<"\nShowing the Disequalities\n";
  //showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations


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


   CDAGNode *DagFromEqs, *DagFromDiseqs;
   DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, MANAGER, VarNames);
   DagFromDiseqs = getDAGFromDisEqs(DisEqFreeOfAllVarsToElimCoefMat, DisEqFreeOfAllVarsToElimConstMat, MANAGER, VarNames);
  	
  if(EliminationDone) 
    {
      //cout<<"\nElimination done. Exiting from this call...\n";
      

      if(DagFromEqs != NULL)
	if(resultDAG == NULL) resultDAG=DagFromEqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromEqs);
      if(DagFromDiseqs != NULL)
      	if(resultDAG == NULL) resultDAG=DagFromDiseqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromDiseqs);
     
      return resultDAG;
     }


  // NEWLY ADDED ON 8/2/2010 

  cout<<"\nWe are left with the following constraints\n";

  cout<<"\n###############################\n";
  showEquations(OriginalEqCoefMat, OriginalEqConstMat); // Show the equations
  showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  cout<<"\n###############################\n";

  eliminateImpliedDisequations(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, Mod, WidthTable, VarNames, MANAGER);
  // disequations implied by the equations and other disequations are eliminated. Further scope for dropping disequations may be there

  cout<<"\nWe are left with the following constraints after eliminateImpliedDisequations\n";

  cout<<"\n###############################\n";
  showEquations(OriginalEqCoefMat, OriginalEqConstMat); // Show the equations
  showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
  cout<<"\n###############################\n";

  cout<<"\nChecking if more disequations can be dropped\n";
  if(!DisEqCoefMat.empty())
    if(DropLMDs==1)
      dropUnconstrainingDisequations(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);
    else
      dropUnconstrainingDisequationsModified(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, NoVarsToElim, Mod);


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
	if(resultDAG == NULL) resultDAG=DagFromEqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromEqs);
      if(DagFromDiseqs != NULL)
      	if(resultDAG == NULL) resultDAG=DagFromDiseqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromDiseqs);
     
      //cout<<"\nResulting DAG = "<<resultDAG->getDAGStr()<<endl;

      //exit(1); // Temporary

      return resultDAG;
     }

  

  // NEWLY ADDED ON 8/2/2010 ENDS HERE

  
  if(!SplitFlag && !OriginalEqCoefMat.empty()) // Disequations are not split already and there are equations
    {
      cout<<"\nSplitting needed. Exiting\n";

      exit(1);
      
      SplitFlag=true;

      CDAGNode *resultSplit = splitDisequationsReturningDAG(OriginalEqCoefMat, OriginalEqConstMat, DisEqCoefMat, DisEqConstMat, VarToElimIndex, Mod, NoVarsToElim, SplitFlag, MANAGER, VarNames, WidthTable);
	
      

      if(DagFromEqs != NULL)
	  if(resultDAG == NULL) resultDAG=DagFromEqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromEqs);
       if(DagFromDiseqs != NULL)
      	 if(resultDAG == NULL) resultDAG=DagFromDiseqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromDiseqs);
      if(resultSplit != NULL)
	 if(resultDAG == NULL) resultDAG=resultSplit; else resultDAG = MANAGER.createDAG("and", resultDAG, resultSplit);
      
      return resultDAG;

    }

    // Else bit-blast

  else 
	{
	  cout<<"\nBit-blasting needed. Exiting...\n"; 

	  exit(1);
	
	//Assuming that result of bit-blasting is true 
        if(DagFromEqs != NULL)
	  if(resultDAG == NULL) resultDAG=DagFromEqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromEqs);
       if(DagFromDiseqs != NULL)
      	 if(resultDAG == NULL) resultDAG=DagFromDiseqs; else resultDAG = MANAGER.createDAG("and", resultDAG, DagFromDiseqs);
      return resultDAG;
       }
  
}

CDAGNode* splitDisequationsReturningDAG(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, int VarToElimIndex, ULINT Mod, int NoVarsToElim, bool SplitFlag, CHelper &MANAGER, list<string> VarNames, map<string, int> WidthTable)
{
CDAGNode *resultDAG=NULL;

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

     CDAGNode* DAGFromqElim = qEliminatorReturningDAG(NewEqCoefMat, NewEqConstMat, NewDisEqCoefMat, NewDisEqConstMat, NoVarsToElim, Mod, SplitFlag, MANAGER, VarNames, WidthTable);
     if(DAGFromqElim != NULL)
	     if(resultDAG==NULL) resultDAG = DAGFromqElim; else resultDAG = MANAGER.createDAG("or", resultDAG, DAGFromqElim);
     
   }
   return resultDAG;
}


CDAGNode* getDAGFromEqs(Matrix EqCoefMat, Matrix EqConstMat, CHelper &MANAGER, list<string> VarNames)
	{

	list<string>::iterator it;

        CDAGNode* result=NULL;
	
	int rows = EqCoefMat.size();
	int cols = EqCoefMat[0].size();
	for(int i=0; i<rows; i++)
		{ 
	        CDAGNode *lhs=NULL, *eqn, *rhs;
		it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
			string id_delta = *it;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
                        string delta = id_delta.substr(index+1);
                        int delta_int = atoi(delta.c_str());

			CDAGNode *variable = MANAGER.createDAG(id, delta_int);
			char coeff[20];

			sprintf(coeff, "%llu", EqCoefMat[i][j]);
			string coeff_str(coeff);
			CDAGNode *coefficient = MANAGER.createDAG(coeff_str);
			CDAGNode *term = MANAGER.createDAG("*", coefficient, variable);
			if(lhs==NULL) lhs=term; else lhs=MANAGER.createDAG("+", lhs, term);
			}
		char constant[10];
		sprintf(constant, "%llu", EqConstMat[i][0]);
		string constant_str(constant);
		rhs = MANAGER.createDAG(constant_str);
		eqn = MANAGER.createDAG("equivalent", lhs, rhs);
		if(result==NULL) result=eqn; else result = MANAGER.createDAG("and", result, eqn);
		}
	return result;
	
	}

CDAGNode* getEquationDAGFromEqn(ULINT SelectedCoef, Matrix SelectedEqn, ULINT SelectedConst, CHelper &MANAGER, list<string> VarNames, string VarToElim)
	{
	list<string>::iterator it;

        CDAGNode* result=NULL;
	
	int cols = SelectedEqn[0].size();

	CDAGNode *rhs=NULL, *eqn, *lhs;
	it=VarNames.begin();
	for(int j=0; j<cols; j++, it++)
			{
                        string id_delta = *it;

			//cout<<"\nid_delta = "<<id_delta<<endl;

			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
                        string delta = id_delta.substr(index+1);
                        int delta_int = atoi(delta.c_str());

			CDAGNode *variable = MANAGER.createDAG(id, delta_int);
			char coeff[20];
			sprintf(coeff, "%llu", SelectedEqn[0][j]);
			string coeff_str(coeff);
			CDAGNode *coefficient = MANAGER.createDAG(coeff_str);
			CDAGNode *term = MANAGER.createDAG("*", coefficient, variable);
			if(rhs==NULL) rhs=term; else rhs=MANAGER.createDAG("+", rhs, term);
			}
	char constant[20];
	sprintf(constant, "%llu", SelectedConst);
	string constant_str(constant);
	CDAGNode *term = MANAGER.createDAG(constant_str);
	if(rhs==NULL) rhs=term; else rhs=MANAGER.createDAG("+", rhs, term);
	

	string id_delta = VarToElim;
	int index = id_delta.find_last_of("_");
	string id = id_delta.substr(0, index);
        string delta = id_delta.substr(index+1);
        int delta_int = atoi(delta.c_str());
	CDAGNode *variable = MANAGER.createDAG(id, delta_int);

        if(SelectedCoef != 1)
		{
		char coeff[20];
		sprintf(coeff, "%llu", SelectedCoef);
		string coeff_str(coeff);
		CDAGNode *coefficient = MANAGER.createDAG(coeff_str);
		term = MANAGER.createDAG("*", coefficient, variable);
		}
	else
		{
		term = variable;
		}
	
	eqn = MANAGER.createDAG("equivalent", term, rhs);
	return eqn;
	}
	
CDAGNode* getDAGFromDisEqs(Matrix DisEqCoefMat, Matrix DisEqConstMat, CHelper &MANAGER, list<string> VarNames)
	{

	list<string>::iterator it;

        CDAGNode* result=NULL;
	
	int rows = DisEqCoefMat.size();
	int cols = DisEqCoefMat[0].size();
	for(int i=0; i<rows; i++)
		{ 
	        CDAGNode *lhs=NULL, *eqn, *rhs;
	        it=VarNames.begin();
		for(int j=0; j<cols; j++, it++)
			{
			string id_delta = *it;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
                        string delta = id_delta.substr(index+1);
                        int delta_int = atoi(delta.c_str());

			CDAGNode *variable = MANAGER.createDAG(id, delta_int);
			char coeff[20];

			sprintf(coeff, "%llu", DisEqCoefMat[i][j]);
			string coeff_str(coeff);
			CDAGNode *coefficient = MANAGER.createDAG(coeff_str);
			CDAGNode *term = MANAGER.createDAG("*", coefficient, variable);
			if(lhs==NULL) lhs=term; else lhs=MANAGER.createDAG("+", lhs, term);
			}
		char constant[10];
		sprintf(constant, "%llu", DisEqConstMat[i][0]);
		string constant_str(constant);
		rhs = MANAGER.createDAG(constant_str);
		eqn = MANAGER.createDAG("is_not_equal", lhs, rhs);
		if(result==NULL) result=eqn; else result = MANAGER.createDAG("and", result, eqn);
		}
	return result;
	
	}		

  
// Given number in form 2^k*c return c Eg: if number=4, return 1
ULINT getOddPart(ULINT number)
{
  if(number == 0) {cout<<"\nFunction getOddPart called with number = 0\n";  return 0;}


  ULINT power=0;
  while(number%2==0) {number=number/2; power++;}
  return number;    
}



// This function eliminates duplicate entries from a set of equations/disequations and recreates a new set of constraints
void deleteDuplicateConstraints(Matrix CoefMat, Matrix ConstMat, Matrix &NewCoefMat, Matrix &NewConstMat)
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
void addConstraint(Matrix &NewCoefMat, Matrix &NewConstMat, Row LHS, ULINT RHS)
{
  NewCoefMat.push_back(LHS);
  Row NewConst;
  NewConst.push_back(RHS);
  NewConstMat.push_back(NewConst);
}

// Checks if the constraint LHS op RHS is already present in the set of constraints
bool constraintPresent(Matrix NewCoefMat, Matrix NewConstMat, Row LHS, ULINT RHS)
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
bool rowsEqual(Row row1, Row row2)
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
  

void eliminateTrivialEquationsAndDisequations(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat)
{
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

  RowsToDelete.reverse(); // reverse so that indices remain intact after deletion

  //cout<<"\nRows in set of equations to delete\n";
  
  //for(set<int>::iterator it=RowsToDelete.begin(); it != RowsToDelete.end(); it++)
  //  {
  //    cout<<*it<<"\t";
  //  }
  //cout<<endl;

  //cout<<"\nDeleting them\n";

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

  //cout<<"\nRows in set of disequations to delete\n";
  
  //for(set<int>::iterator it=RowsToDelete.begin(); it != RowsToDelete.end(); it++)
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


// return true if the row indexed by index is all zeros in A
bool rowIsNull(Matrix A,int index)
{
int rows = A.size();
int cols = A[0].size();
ULINT element;

 if(index>=rows) {cout<<"\nFunction rowIsNull is called with index > #rows of matrix. Exiting...\n"; exit(1);}
 
 for(int i=0; i<cols; i++)
			{
			element = A[index][i];
			if(element != 0) return false;
			}
 return true;
}



void eliminateImpliedDisequations(Matrix EqCoefMat, Matrix EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, ULINT Mod, map<string, int> WidthTable, list<string> VarNames, CHelper &MANAGER)
{  
  
  while(true)
    {

      cout<<"\nShowing the Equalities\n";
      showEquations(EqCoefMat, EqConstMat); // Show the equations
      cout<<"\nShowing the Disequalities\n";
      showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations



      int DisEqImplied = -1;
      
      int rows=DisEqCoefMat.size();
      for(int i=0; i<rows; i++)
	{
	  if(disEquationImpliedByOthers(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, Mod, WidthTable, i, VarNames, MANAGER)) // check if ith disequation implied by others
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


bool disEquationImpliedByOthers(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, ULINT Mod, map<string, int> WidthTable, int SelectedIndex, list<string> VarNames, CHelper &MANAGER)
{
  Matrix SelectedDisEqCoefMat,SelectedDisEqConstMat; 

  // Copy the ith disequation into selected disequation
  copyRow(DisEqCoefMat, SelectedIndex, SelectedDisEqCoefMat);
  copyRow(DisEqConstMat, SelectedIndex, SelectedDisEqConstMat);

  // delete the selected disequation from the set of disequations
  deleteRow(DisEqCoefMat, SelectedIndex);
  deleteRow(DisEqConstMat, SelectedIndex);



  char implicationHolds = syntacticCheckForImplication(EqCoefMat, EqConstMat, DisEqCoefMat, DisEqConstMat, SelectedDisEqCoefMat, SelectedDisEqConstMat, Mod); // We can use a cheap check for implication if the no: of disequations is one

  if(implicationHolds=='t')
    return true;

  if(implicationHolds=='f')
    return false;


  // Forming the dags
  CDAGNode*  DagFromEqs = getDAGFromEqs(EqCoefMat, EqConstMat, MANAGER, VarNames);
  CDAGNode*  DagFromDiseqs = getDAGFromDisEqs(DisEqCoefMat, DisEqConstMat, MANAGER, VarNames);
  
  CDAGNode*  resultDAG;
  
  if(DagFromEqs != NULL)
    {
      if(DagFromDiseqs != NULL)
      	{
	  resultDAG = MANAGER.createDAG("and", DagFromEqs, DagFromDiseqs);
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

  CDAGNode*  DagFromSelectedDisEq = getDAGFromDisEqs(SelectedDisEqCoefMat, SelectedDisEqConstMat, MANAGER, VarNames);
  
  cout<<"\nChecking if "<<resultDAG->getDAGStr()<<" implies "<<DagFromSelectedDisEq->getDAGStr()<<endl;

  resultDAG = MANAGER.createDAG("and", resultDAG, MANAGER.createDAG("not",DagFromSelectedDisEq));

  cout<<"\nDAG formed\n";


  FILE *yices_file = fopen("thunsat.ys", "w");

  writeBVDeclarationsInYicesFormatWithLookup(yices_file, WidthTable, VarNames);
  string dag_str = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(resultDAG, WidthTable);
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

void concatenateMatrices(Matrix Source1, Matrix Source2, Matrix &Destination)
{
  int rows = Source1.size();
  int cols1 = Source1[0].size();
  int cols2 = Source2[0].size();
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

// Ensure that there is only a single non-zero entry and return it
int getSingleNonZeroEntry(Matrix MyMatrix)
{
  int rows = MyMatrix.size();
  int cols = MyMatrix[0].size(); 
  
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


char syntacticCheckForImplication(Matrix EqCoefMat, Matrix EqConstMat, Matrix DisEqCoefMat, Matrix DisEqConstMat, Matrix SelectedDisEqCoefMat, Matrix SelectedDisEqConstMat, ULINT Mod)
{
  int NoOfDisEqs = DisEqCoefMat.size();
  int NoOfEqs = EqCoefMat.size();
  int NoVarsToElim = EqCoefMat[0].size();

  int NoOfNewVars = NoOfDisEqs;

  // Extend equations with new variables with coefficient zero
  
  // get a null matrix of size #Equations * NoOfNewVars

  Matrix NullMatrix;
  getNullMatrix(NullMatrix, NoOfEqs, NoOfNewVars);

  //cout<<"\nNull Matrix is\n";
  //showMatrix(NullMatrix);
  
  Matrix TempMatrix;
  concatenateMatrices(EqCoefMat, NullMatrix, TempMatrix);
  EqCoefMat.clear();
  copyMatrix(TempMatrix, EqCoefMat);

  cout<<"\nEqCoefMat extended with zeros is\n";
  showMatrix(EqCoefMat);

  // get a unit matrix of size NoOfNewVars * NoOfNewVars 
  Matrix NewEquationCoefMat;
  Matrix UnitMatrix;
  getUnitMatrix(UnitMatrix, NoOfNewVars, NoOfNewVars);

  //cout<<"\nUnit Matrix is\n";
  //showMatrix(UnitMatrix);

  concatenateMatrices(DisEqCoefMat, UnitMatrix, NewEquationCoefMat);

  cout<<"\nNewEquationCoefMat obtained from disequations is\n";
  showMatrix(NewEquationCoefMat);


  // Equations = Equations + NewEquation
  addRows(EqCoefMat, NewEquationCoefMat);
  addRows(EqConstMat, DisEqConstMat);

  
  cout<<"\nShowing the Equalities adding the new set of equations obtained from disequations\n";
  showEquations(EqCoefMat, EqConstMat); // Show the equations



  // get a null matrix of size 1 * NoOfNewVars
  getNullMatrix(NullMatrix, 1, NoOfNewVars);
  TempMatrix.clear();
  concatenateMatrices(SelectedDisEqCoefMat, NullMatrix, TempMatrix);
  SelectedDisEqCoefMat.clear();
  copyMatrix(TempMatrix, SelectedDisEqCoefMat);

  cout<<"\nShowing the Selected Disequality\n";
  showDisEquations(SelectedDisEqCoefMat, SelectedDisEqConstMat); // Show the disequations

  //exit(1);

  qEliminatorForEquationsAndDisequations(EqCoefMat, EqConstMat, SelectedDisEqCoefMat, SelectedDisEqConstMat, NoVarsToElim, Mod);

  
  // Find the first variable still to be eliminated 
  
  bool EliminationDone=true; // Is elimination already done?

  int VarToElimIndex; // Index of the first variable still to be eliminated 

  if(!SelectedDisEqCoefMat.empty())
    {
      for(VarToElimIndex=0; VarToElimIndex<NoVarsToElim; VarToElimIndex++)
	{
	  if(!columnIsNull(SelectedDisEqCoefMat, VarToElimIndex))  // Column for variable to eliminate is non null => variable is not eliminated
	    {
	      cout<<"\nVariable x"<<VarToElimIndex<<" is present in disequations\n";
	      EliminationDone=false;
	      return 'x'; // Need an SMT solver. Disequation cannot be expressed in terms of epsilons
	    }
	}
    }
  
  cout<<"\nShowing the Equalities and Disequalities after eliminating all variables other than epsilons\n";
  cout<<"\nShowing the Equalities\n";
  showEquations(EqCoefMat, EqConstMat); // Show the equations
  cout<<"\nShowing the Disequalities\n";
  showDisEquations(SelectedDisEqCoefMat, SelectedDisEqConstMat); // Show the disequations


  if(IsNullMatrix(SelectedDisEqCoefMat) && SelectedDisEqConstMat[0][0] == 0) return 'x'; // 0 <> 0 Use SMT solver
  
  int CoefOfEpsilon = getSingleNonZeroEntry(SelectedDisEqCoefMat);

  cout<<"\nCoefOfEpsilon =  "<<CoefOfEpsilon<<endl;

  if(CoefOfEpsilon==-1) return 'x'; // More than one epsilons Use SMT solver

  if(CoefOfEpsilon%2 != 0 && SelectedDisEqConstMat[0][0] == 0)  // MI exists and can be converted to epsilon<>0 which is followed by our assumption
    {
      cout<<"\nReturning true\n";
      return 't';
    }

  cout<<"\nReturning false\n";
  return 'f';

}
  
  
// Functions newly added on 28th May 2010

void dropUnconstrainingDisequationsModified(Matrix &EqCoefMat, Matrix &EqConstMat, Matrix &DisEqCoefMat, Matrix &DisEqConstMat, int NoVarsToElim, ULINT Mod) 
{

  // Take variables to be eliminated one by one. For each variable, eliminate the disequations which can be eliminated

for(int VarToElimIndex=0; VarToElimIndex<NoVarsToElim; 	VarToElimIndex++)
	{
	 if(DisEqCoefMat.empty())
	   return;
	    
	cout<<"\nTrying to drop disequalities looking at variable x"<<VarToElimIndex<<"\n";

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
	    cout<<"\nVariable x"<<VarToElimIndex<<" is already eliminated from disequations\n";
	    continue;
	  }

	cout<<"\nCoefsOfVarToElimEqualities\n";
	showMap(CoefsOfVarToElimEqualities);
	cout<<"\nCoefsOfVarToElimDisEqualities\n";
	showMap(CoefsOfVarToElimDisEqualities);
	
	// CoefsOfVarToElimEqualities, CoefsOfVarToElimDisEqualities contain coefficients of the variable to eliminate VarToElimIndex in equations and disequations

	// Convert them to powers of 2

	convertCoefToPowerOf2(CoefsOfVarToElimEqualities);
	convertCoefToPowerOf2(CoefsOfVarToElimDisEqualities);

	cout<<"\nAfter converting into powers of 2\n";
	cout<<"\nCoefsOfVarToElimEqualities\n";
	showMap(CoefsOfVarToElimEqualities);
	cout<<"\nCoefsOfVarToElimDisEqualities\n";
	showMap(CoefsOfVarToElimDisEqualities);

	ULINT KEqn = findMinimumKValue(CoefsOfVarToElimEqualities, Mod);

	cout<<"\nKEqn = "<<KEqn<<endl;


	/* Newly added on 4 July 2010 to take care of KEqn s.t. KEqn <= Ki where Ki is the Ki of a disequation */

	ULINT KDisEqMax = findMaximumKValue(CoefsOfVarToElimDisEqualities);
        if(KEqn<=KDisEqMax) // This is a bogus LME. 
		{
		KEqn = powerOfTwo(Mod);
		}	

	/* Newly added code on 4 July ends here */
	

	// Drop the disequalities

	set<int> CoreLMDs; // CoreLMDs is the set of LMDs which cannot be dropped
	// Let there is an LMD free of the variable to be eliminated. This cannot be dropped. So it must be in CoreLMDs
	initializeCoreLMDs(DisEqCoefMat, VarToElimIndex, CoreLMDs);
	
	showSet(CoreLMDs, "CoreLMDs");	

	eliminateDisequalitiesModified(KEqn, CoefsOfVarToElimDisEqualities, CoreLMDs);

	showSet(CoreLMDs, "CoreLMDs");	
	
	cout<<"\nShowing the Original Set of Disequalities\n";
	showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations

	cout<<"\nDeleting disequalities not in CoreLMDs\n";



	list<int> DisequationsToEliminate;

	// Get the list of LMDs to eliminate
	for(int i=0; i<DisEqCoefMat.size(); i++) {
	  if(CoreLMDs.find(i)==CoreLMDs.end()) // i is not in CoreLMDs. Hence drop
	    DisequationsToEliminate.push_back(i);
	}
            
	cout<<"\nDisequationsToEliminate\n";
	for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
	  {cout<<*it<<endl;}
	    
	// Eliminate them in reverse order so that after each elimination, the row indexes are intact.

	DisequationsToEliminate.reverse();

	cout<<"\nDisequationsToEliminate\n";
	for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
	  {cout<<*it<<endl;}
  
	for(list<int>::iterator it=DisequationsToEliminate.begin(); it != DisequationsToEliminate.end(); it++)
	  {
	    cout<<"\nDisequation "<<*it<<" can be eliminated\n";
	    deleteRow(DisEqCoefMat, *it);
	    deleteRow(DisEqConstMat, *it);
	  }
      
	
        cout<<"\nShowing the Modified Set of Disequalities\n";
	showDisEquations(DisEqCoefMat, DisEqConstMat); // Show the disequations
	}

  
}


//  Function to copy the zero elements in the selectedIndexth column into CoreLMDs. Initially CoreLMDs is empty. 
void initializeCoreLMDs(Matrix MyMatrix, int selectedIndex, set<int> &CoreLMDs)
{
int rows = MyMatrix.size();
int cols = MyMatrix[0].size();

ULINT element;
if(selectedIndex<0 || selectedIndex>=cols) {cout<<"\nInvalid column "<<selectedIndex<<" in function initializeCoreLMDs\n"; exit(1);}
for(int i=0; i<rows; i++)
			{
			element=MyMatrix[i][selectedIndex];
			if(element == 0) 
			  {CoreLMDs.insert(i);}
			}
}

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

void eliminateDisequalitiesModified(ULINT KEqn, map<int, ULINT> &KValuesForDisEquations, set<int> &CoreLMDs)
{
 set<int> core; // core initially empty
 set<int> D; // set of disequations
 for(map<int, ULINT>::iterator it=KValuesForDisEquations.begin(); it != KValuesForDisEquations.end(); it++)
	{
	D.insert(it->first);
	} // set of disequations obtained


 cout<<"\nSet of disequations initially in D we have is \n";
 showSet(D, "D");

 cout<<"\nInitially core is\n";
 showSet(core, "core");
  
 while(!setsEqual(core, D))
	{
	cout<<"\ncore != D\n";
	
	if(isExtModified(core, D, KEqn, KValuesForDisEquations)) // Any solution to core can be extended
		{
		cout<<"\ncore is extendable\n";
                set_union(core.begin(), core.end(), CoreLMDs.begin(), CoreLMDs.end(),inserter(CoreLMDs, CoreLMDs.begin()));
		showSet(CoreLMDs, "CoreLMDs");
		return;
		}
	else
		{
		cout<<"\ncore is not extendable\n";
		set<int> d = getLeastConstrLMDs(D, core, KValuesForDisEquations); // get set of least constraining LMDs from D\core 	
		set_union(core.begin(), core.end(), d.begin(), d.end(),inserter(core, core.begin()));
		cout<<"\nNow core is\n";
		showSet(core, "core");
		}
	}

set_union(core.begin(), core.end(), CoreLMDs.begin(), CoreLMDs.end(),inserter(CoreLMDs, CoreLMDs.begin()));
return;
}

bool setsEqual(set<int> &A, set<int> &B)
{
set<int> Diff1;
set<int> Diff2;

set_difference(A.begin(), A.end(), B.begin(), B.end(),inserter(Diff1, Diff1.begin()));
set_difference(B.begin(), B.end(), A.begin(), A.end(),inserter(Diff2, Diff2.begin()));

if(Diff1.empty() && Diff2.empty()) return true;
return false;
}



bool isExt(set<int> core, set<int> D, ULINT KEqn, map<int, ULINT> &KValuesForDisEquations)
{
cout<<"\nChecking if core is extendable\n";
// find k1
ULINT k1;

if(core.empty()) k1=KEqn;
else
	{
	// k1 is the minimum of k values of disequations in core
	set<int>::iterator core_it=core.begin();
	k1 = KValuesForDisEquations[*core_it];
	core_it++;
	for(; core_it!=core.end(); core_it++)
		{
		if(KValuesForDisEquations[*core_it] < k1) k1 = KValuesForDisEquations[*core_it];
		}
	}

 cout<<endl<<"k1 = "<<k1<<endl;
// we got k1
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
cout<<"\nshowKMap\n";
for(map<ULINT, int>::iterator KMap_it=KMap.begin(); KMap_it!=KMap.end(); KMap_it++)
	{
	cout<<endl<<KMap_it->first<<"\t"<<KMap_it->second<<endl;
	}


map<ULINT, int>::iterator it1=KMap.begin();
map<ULINT, int>::iterator it2=KMap.begin();
if(it2!=KMap.end()) it2++;

ULINT k_next, k_now;
int r_now;

for(;it2!=KMap.end(); it1++, it2++)
	{

	k_now=it1->first;
	r_now=it1->second;
	k_next=it2->first;

	ULINT combinations = findPower(k_next-k_now)-r_now;
	if(combinations<1)
		return false;
	}

k_now=it1->first;
r_now=it1->second;
k_next=k1;

ULINT combinations = findPower(k_next-k_now)-r_now;
if(combinations<1)
	return false;
return true;
}

set<int> getLeastConstrLMDs(set<int> D, set<int> core, map<int, ULINT> KValuesForDisEquations)
{
  cout<<"\nFinding the least constrained LMDs\n";
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

 cout<<"\nkMax = "<<kMax<<endl;

set<int> LeastConstrLMDs;
for(set<int>::iterator D_it=D.begin(); D_it != D.end(); D_it++)
	{
	int DiseqIndex = *D_it;
	if(core.find(DiseqIndex)!=core.end()) // disequation is in core
		continue;
	ULINT k=KValuesForDisEquations[DiseqIndex];
	
	if(k==kMax) LeastConstrLMDs.insert(DiseqIndex);	
	}

showSet(LeastConstrLMDs, "LeastConstrLMDs");
return LeastConstrLMDs;
}

// Function to show Set
void showSet(set<int> MySet, string WhoAmI)
{
cout<<endl<<WhoAmI<<endl<<"***************"<<endl;
for(set<int>::iterator it=MySet.begin(); it != MySet.end(); it++)
  cout<<*it<<"\n";
}

// The new version of isExt after discussion on 11 June 2010 which is r1.2^k1+...+rn.2^kn < 2^kcore.

bool isExtModified(set<int> core, set<int> D, ULINT KEqn, map<int, ULINT> &KValuesForDisEquations)
{
cout<<"\nChecking if core is extendable\n";
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

 cout<<endl<<"kcore = "<<kcore<<endl;
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
cout<<"\nshowKMap\n";
for(map<ULINT, int>::iterator KMap_it=KMap.begin(); KMap_it!=KMap.end(); KMap_it++)
	{
	cout<<endl<<KMap_it->first<<"\t"<<KMap_it->second<<endl;
	}


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

cout<<"\nCombinations Allowed = "<<CombinationsAllowed<<"\tCombinations Prevented = "<<CombinationsPrevented<<endl;

if(CombinationsAllowed > CombinationsPrevented)
	{
	cout<<"\nExtendable\n";
	return true;
	}

cout<<"\nNot extendable\n";
return false;
}


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

