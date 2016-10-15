/*
#include "y.tab.c"
#include "lex.yy.c"
*/

#include "propTransYacc.tab.h"
#include "CHelper.h"

using namespace std;

extern int pplex(void);	
extern int ppparse(void);
void pperror(char* str);
void getInitWidthTable(map<string, string> &initWidthTable);

CHelper MANAGER;
CDAGNode* root;

string output_type = "smtlib";
string output_file = "";
string support_file = "";


void usage()
{
  cout<<"\nUsage\n*********\n";
  cout<<"\npropTrans < BoundedPSL-property-file options\n";
  cout<<"\nCurrently options are\n";
  cout<<"\n-output_file=filename : filename is the name of the file where the unrolled property needs to be written";
  cout<<"\n-support_file=filename : filename is the name of the file where the support of the property needs to be written ";
  cout<<"\nyices : Write the output file in Yices format(default)";
  cout<<"\nsmtlib : Write the output file in SMTLIB format";
  cout<<"\nproper_smtlib : Write the output file as a proper SMTLIB file (with declarations)";
}



void readArguments(int argc, char **argv)
{

  int i=1;
  while(i<=argc-1)
    {
      char* argument=argv[i];

      //cout<<endl<<argument<<endl;

      if(strcmp(argument,"smtlib")==0)
	{
	  output_type = "smtlib";
	}
      else if(strcmp(argument,"yices")==0)
	{
	  output_type = "yices";
	}
      else if(strcmp(argument,"proper_smtlib")==0)
	{
	  output_type = "proper_smtlib";
	}
      else 
	{
	  string str_arg = argument;
	  int index = str_arg.find_last_of("=");
	  if(index!=string::npos)
	    {
	      if(str_arg.substr(0, index)=="-output_file")
		{
		  output_file = str_arg.substr(index+1);	
		}
	      else if(str_arg.substr(0, index)=="-support_file")
		{
		  support_file = str_arg.substr(index+1);	
		}

	      else
		{
		  cout<<"\nUnknown command line argument "<<argument<<endl;
		  usage();
		  exit(1);
		}
	    }
	  else
	    {
	      cout<<"\nUnknown command line argument "<<argument<<endl;
	      usage();
	      exit(1);
	    }
	}//else
      i++;
    }

  //cout<<"\noutput_type = "<<output_type<<endl;
  //cout<<"\noutput_file = "<<output_file<<endl;
  //cout<<"\nsupport_file = "<<support_file<<endl;
}


int main(int argc, char **argv) {

        readArguments(argc, argv);

	//cout<<"In main; ppparse() is going to be called"<<endl;
	ppparse();
	
	//with 'X','G','F' quantifiers---------
	//MANAGER.printGraphInDotFormat(root, "formula0.dot");
	
	root = MANAGER.callUnrolledClone(root);
	
	//without 'X','G','F' quantifiers---------
	//MANAGER.printGraphInDotFormat(root, "formula1.dot");
	
	//identifier-width input file
	//use file.cpp 2nd function prepare initial-id-width-table-hashmap
	map<string, string> initWidthTable;
	map<string, int> widthTable;
	getInitWidthTable(initWidthTable);
	
	//prepare a set<string>names from getDagNames(names)
	set<string>names;
	root->getDAGNames(names);
	
	//create id sperate from its dt
	//split each identifier to look its corresponding width from initial-id-width-hashmap
	//looping 
	set<string>::iterator sit = names.begin();

		for(; sit != names.end(); sit++){
			string id_delta = *sit;
			int index = id_delta.find_last_of("_");
			string id = id_delta.substr(0, index);
			int width;
			//make an iterator over this set
			map<string, string> :: iterator  map_iter =  initWidthTable.find(id);			
			//if not found, generate warning and exit
				if(map_iter ==  initWidthTable.end()){
					cout << "\n No width specified for identifier :"<< id <<" in input file" << endl;
					exit(1);
				}
			//else again prepare a fresh id-width-hashmap
				else
				{
					width = atoi((map_iter->second).c_str() );
					widthTable.insert(make_pair(id_delta,width));
				}
		}
		
		///*
		 // for checking what is there is width table
			//cout<<"\nchecking what is there is width table::::"<<endl;
			map<string, int> :: iterator  mit =  widthTable.begin();			
			for(; mit != widthTable.end(); mit++)
			{
				// do nothing
			}
						
		//*/
		
		
	int tempCount = 0;

	if(output_file == "")
	  {
	    cout<<"\nOutput file is not specified\n";
	    exit(1);
	  }

	FILE *yices_fp = fopen(output_file.c_str(), "w");
	//check for errors too
		if(yices_fp == NULL){
		  cout<<"\nCannot create "<<output_file<<"\n"; 
		  exit(1);
		}

	//cloneFromPSLToSEL
	//cout<<"Cloning from PSL->SEL.."<<endl;
	root = MANAGER.callCloneFromPSLToSEL(root);
	//cout<<"Cloning done.."<<endl;
	map<CDAGNode*, string> labelTable;
	
	// string s = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTableWithDP(root, widthTable, tempCount, yices_fp, labelTable);
// 	fprintf(yices_fp,"\n(assert (not %s))\n",s.c_str()); //negation added
// 	fprintf(yices_fp,"\n(check)\n");


	string s;
	
	if(output_type=="yices")
	  {
	    s = MANAGER.callgetDAGOfProperSkeltonFormatInYicesFormatWithWidthTable(root, widthTable);
	    fprintf(yices_fp,"(not %s)",s.c_str()); //negation added
	    fclose(yices_fp);
	  }
	else if(output_type=="smtlib")
	  {
	    s = MANAGER.callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(root, widthTable);
	    fprintf(yices_fp,"(not %s)",s.c_str()); //negation added
	    fclose(yices_fp);
	  }
	else if(output_type=="proper_smtlib")
	  {
	    fprintf(yices_fp,"(benchmark test\n");
	    MANAGER.writeBVDeclarationsFromDAGInSMTLibFormat(yices_fp, root, widthTable);
	    s = MANAGER.callgetDAGOfProperSkeltonFormatInNewSMTLibFormatWithWidthTable(root, widthTable);
	    fprintf(yices_fp,"\n:formula (not %s)",s.c_str()); //negation added
	    fprintf(yices_fp,"\n)\n");
	    fclose(yices_fp);
	  }
	else
	  {
	    cout<<"\nUnknown output_type in main.cpp\n";
	    exit(1);
	  }


	if(support_file == "")
	  {
	    cout<<"\nSupport file is not specified\n";
	  }
	else
	  {
	    FILE *support_fp = fopen(support_file.c_str(), "w");
	    //check for errors too
	    if(support_fp == NULL){
	      cout<<"\nCannot create "<<support_file<<"\n"; 
	      exit(1);
	    }
	    set<string> Support;
	    root->getDAGNames(Support);

	    for(set<string>::iterator it = Support.begin(); it != Support.end(); it++)
	      {
		string signal = *it;
		fprintf(support_fp, "%s\n", signal.c_str());
	      }
	    fclose(support_fp);
	  }

	
	return 0;
}
void getInitWidthTable(map<string, string> &initWidthTable)
{
  ifstream *infile;
  infile = new ifstream();
  infile->open("widthtable.txt");
  while(!infile->eof())
    {
      string s;
      *infile>>s;
      if(s=="")
	break;
      //cout<<"Line read is "<<s<<endl;
      
      int equal_pos = s.find("-");
      string signal_name = s.substr(0, equal_pos);
      //cout<<"signal_name "<<signal_name<<endl;
      string value  = s.substr(equal_pos+1);
      //cout<<"value "<<value<<endl;

      initWidthTable.insert(make_pair(signal_name, value));
    }

  infile->close();

}
