%{

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <search.h>

#include "y.tab.h"

#include "idinfo.h"


# define YYLMAX BUFSIZ

extern char *yysptr, yysbuf[];
int yytchar;

#ifdef BASE_REPRESENTATION
#ifdef ECHO
#undef ECHO
#endif
#define ECHO echo(yytext)
static void echo(s)
char* s;
{
  char  loc[YYLMAX];
  int   l;
  l=strlen(s);
  loc[l]='\0';
  while(l--) loc[l]=toupper(*(s+l));  
  fprintf(yyout, "%s\n", loc);
}
#endif

int yycolumno=0;
/*
#undef input()
int input(){
  if (yysptr>yysbuf){
    yytchar=U(*--yysptr);
  }else{
    yytchar=getc(yyin);
  }
  if (yytchar=='\t')
    yycolumno+=8;
  else
    ++yycolumno;
  if (yytchar=='\n'){
    yylineno++;
  }
  if (yytchar==EOF)
    return 0;
  return yytchar;
}
#undef unput(c)
void unput(c)
char c;
{
  yytchar=c;
  *yysptr++=yytchar;
  if(yytchar=='\n')
    yylineno--;
  if (c=='\t')
    yycolumno-=8;
  else
    --yycolumno;
}
*/

#define MVL_LG_MC 15
#define MVL_NB_MC 89

int MVL_LINNUM=1;

typedef struct {
        char nom[MVL_LG_MC];
        int kval;
        } el_mc;

static el_mc tab_mc []=
  {
    {"abs"		,t_ABS},
    {"access"		,t_ACCESS},
    {"after"		,t_AFTER},
    {"alias"		,t_ALIAS},
    {"all"		,t_ALL},
    {"and"		,t_AND},
    {"architecture"	,t_ARCHITECTURE},
    {"array"		,t_ARRAY},
    {"assert"		,t_ASSERT},
    {"attribute"	,t_ATTRIBUTE}, /* 10 */

    {"begin"		,t_BEGIN},
    {"block"		,t_BLOCK},
    {"body"		,t_BODY},
    {"buffer"		,t_BUFFER},
    {"bus"		,t_BUS},

    {"case"		,t_CASE},
    {"component"	,t_COMPONENT},
    {"configuration"	,t_CONFIGURATION},
    {"constant"		,t_CONSTANT},

    {"disconnect"	,t_DISCONNECT},
    {"downto"		,t_DOWNTO},

    {"else"		,t_ELSE},
    {"elsif"		,t_ELSIF},
    {"end"		,t_END},
    {"entity"		,t_ENTITY},
    {"exit"		,t_EXIT},

    {"file"		,t_FILE},
    {"for"		,t_FOR},
    {"function"		,t_FUNCTION},

    {"generate"		,t_GENERATE},
    {"generic"		,t_GENERIC},
    {"guarded"		,t_GUARDED},

    {"if"		,t_IF},
    {"in"		,t_IN},
    {"inout"		,t_INOUT},
    {"is"		,t_IS},   /*36 */

    {"label"		,t_LABEL},
    {"library"		,t_LIBRARY},
    {"linkage"		,t_LINKAGE},
    {"loop"		,t_LOOP},

    {"map"		,t_MAP},
    {"mod"		,t_MOD},

    {"nand"		,t_NAND},
    {"new"		,t_NEW},
    {"next"		,t_NEXT},
    {"nor"		,t_NOR},
    {"not"		,t_NOT},
    {"null"		,t_NULL},

    {"of"		,t_OF},
    {"on"		,t_ON},
    {"open"		,t_OPEN},
    {"or"		,t_OR},
    {"others"		,t_OTHERS},
    {"out"		,t_OUT},

    {"package"		,t_PACKAGE},
    {"port"		,t_PORT},
    {"procedure"	,t_PROCEDURE},
    {"process"		,t_PROCESS},  /*58*/

    {"range"		,t_RANGE},
    {"record"		,t_RECORD},
    {"register"		,t_REGISTER},
    {"rem"		,t_REM},
    {"report"		,t_REPORT},
    {"return"		,t_RETURN},

    {"select"		,t_SELECT},
    {"severity"		,t_SEVERITY},
    {"signal"		,t_SIGNAL},
    {"subtype"		,t_SUBTYPE},  /*68 */

    {"then"		,t_THEN},
    {"to"		,t_TO},
    {"transport"	,t_TRANSPORT},
    {"type"		,t_TYPE},

    {"units"		,t_UNITS},
    {"until"		,t_UNTIL},
    {"use"		,t_USE},

    {"variable"		,t_VARIABLE},

    {"wait"		,t_WAIT},
    {"when"		,t_WHEN},
    {"while"		,t_WHILE},
    {"with"		,t_WITH},

    {"xor"		,t_XOR}, /*81*/

    {"sll"		,t_SLL},
    {"srl"		,t_SRL},
    {"sla"		,t_SLA},
    {"sra"		,t_SRA},
    {"rol"		,t_ROL},
    {"ror"		,t_ROR},  /*87*/

    {"inertial" 	,t_INERTIAL},  /*88*/
    {"xnor" 	,t_XNOR}  /*89*/

	
  };

int
node_compare(const void *node1, const void *node2)
{
    return strcmp(((el_mc *)node1)->nom,
        ((el_mc *)node2)->nom);
}


static int find_mc(char *s)
{
  char  loc[YYLMAX];
  int   l;
  el_mc *pt;
  int i = 0;

  l=strlen(s);
  strcpy(loc,s);
  while(l--) loc[l]=tolower(loc[l]);	/* conversion en minuscules */
  pt = NULL;
  while(i < MVL_NB_MC)
  {
	if(strcmp(tab_mc[i].nom, loc) == 0)
	{
		pt= (el_mc *) &tab_mc[i];
		break;
	}
	i++;
  }

  if (pt==NULL) return(-1);

  else return(pt->kval);
}
%}

upper_case_letter 	  [A-Z]
digit 			  [0-9]
special_character   	  [\#\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\_\|]
space_character 	  [ \t]
format_effector		  [\t\v\r\l\f]
end_of_line		  \n
lower_case_letter 	  [a-z]
other_special_character   [\!\$\@\?\[\\\]\^\`\{\}\~]

graphic_character	  ({basic_graphic_character}|{lower_case_letter}|{other_special_character})
basic_graphic_character	  ({upper_case_letter}|{digit}|{special_character}|{space_character})
letter		   	  ({upper_case_letter}|{lower_case_letter})
letter_or_digit	   	  ({letter}|{digit})
decimal_literal	   	  {integer}(\.{integer})?({exponent})?
integer		   	  {digit}(_?{digit})*
exponent	   	  ([eE][-+]?{integer})
base		   	  {integer}
based_integer	   	  {extended_digit}(_?{extended_digit})*
extended_digit	   	  ({digit}|[a-fA-F])
base_specifier	  	  (B|b|O|o|X|x)
%%
{space_character}	{
			/* nothing */
#ifndef BASE_REPRESENTATION			
			 /*return(t_Space);*/
#endif
			}
\&									{
			 return(t_Ampersand);
									}
\'									{
			 return(t_Apostrophe);
									}
\(									{
			 return(t_LeftParen);
									}
\)									{
			 return(t_RightParen);
									}
"**"									{
			 return(t_DoubleStar);
									}
\*									{
			 return(t_Star);
									}
\+									{
			 return(t_Plus);
									}
\,									{
			 return(t_Comma);
									}
\-									{
			 return(t_Minus);
									}
":="									{
			 return(t_VarAsgn);
									}
\:									{
			 return(t_Colon);
									}
\;									{
			 return(t_Semicolon);
									}
"<="	{
	    
	    return(t_LESym);
	}
">="	{
            
	    return(t_GESym);
	}
\<	{
            
	    return(t_LTSym);
	}
\>	{
            
	    return(t_GTSym);
	}
=									{
			 return(t_EQSym);
									}
\/=									{
			 return(t_NESym);
									}
"=>"	{
            
	    return(t_Arrow);
	}
"<>"	{
            
	    return(t_Box);
	}
\|									{
			 return(t_Bar);
									}
!									{
			 return(t_Bar);
									}
\.									{
			 return(t_Dot);
									}
\/									{
			 return(t_Slash);
									}
{letter}(_?{letter_or_digit})* 	{
	int itoken;
	itoken=find_mc(yytext);
	if (itoken== -1) 
	{
		//printf("****ns1(%s)****\n",yytext);
		
		yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
		yylval.ds_Id->pos=yycolumno;
		yylval.ds_Id->len=strlen(yytext);
		/*yylval.ds_Id->line=yylineno;*/
		yylval.ds_Id->name= strdup(yytext);
		yylval.ds_Id->type= t_Identifier;
	
		yylval.ds_Id->upper = NULL;
		yylval.ds_Id->lower = NULL;
		yylval.ds_Id->unit = NULL;	  
	
		return ( t_Identifier );
	}
	else
	{
		//printf("****ns2(%s)****\n",yytext);
		 return ( itoken );
	}
}

({decimal_literal})|({base}#{based_integer}(\.{based_integer})?#({exponent})?)|({base}:{based_integer}(\.{based_integer})?:({exponent})?)		{
	yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	yylval.ds_Id->len=strlen(yytext);
	yylval.ds_Id->name= strdup(yytext);
	yylval.ds_Id->type= t_AbstractLit;

	yylval.ds_Id->upper = NULL;
	yylval.ds_Id->lower = NULL;
	yylval.ds_Id->unit = NULL;	  

	//printf("****ns3(%s)****\n",yytext);
	 return ( t_AbstractLit );
	}

'({graphic_character}|\"|\%)' 	{
	yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	yylval.ds_Id->len=strlen(yytext);
	yylval.ds_Id->name= strdup(yytext);
	yylval.ds_Id->type= t_CharacterLit;

	yylval.ds_Id->upper = NULL;
	yylval.ds_Id->lower = NULL;
	yylval.ds_Id->unit = NULL;	  

	//printf("****ns4(%s)****\n\n",yytext);
	 return ( t_CharacterLit );
	}

(\"({graphic_character}|(\"\")|\%)*\")|(\%({graphic_character}|(\%\%)|\")*\%) {
	yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	yylval.ds_Id->len=strlen(yytext);
	yylval.ds_Id->name= strdup(yytext);
	yylval.ds_Id->type= t_StringLit;

	yylval.ds_Id->upper = NULL;
	yylval.ds_Id->lower = NULL;
	yylval.ds_Id->unit = NULL;	  

	//printf("****ns5(%s)****\n",yytext);
	 return ( t_StringLit );
	}

{base_specifier}(\"{extended_digit}(_?{extended_digit})*\"|\%{extended_digit}(_?{extended_digit})*\%) {
	yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	yylval.ds_Id->len=strlen(yytext);
	yylval.ds_Id->name= strdup(yytext);
	yylval.ds_Id->type= t_BitStringLit;

	yylval.ds_Id->upper = NULL;
	yylval.ds_Id->lower = NULL;
	yylval.ds_Id->unit = NULL;	  

	//printf("****ns6(%s)****\n",yytext);
	 return ( t_BitStringLit );
	}

\n									{
			/* end of line */
			MVL_LINNUM++;
			/* tobuf( "\n%4d\t", MVL_LINNUM);*/
#ifndef BASE_REPRESENTATION
			
#endif
			yycolumno=0;
			/*return(t_NEWLINE);*/
									}
\-\-.*$ 								{
			/* comment */
#ifndef BASE_REPRESENTATION
			
#endif
			/*return(t_COMMENT);*/
									}
.									{
			 /*return (t_UNKNOWN);*/
									}
%%
/************************************************************
* J.Gaisler reported:
* 	MVL_NB_MC should be set to 81 - done
* The original of this stuff was an illegal and wrong pointer
*************************************************************/
