
%{
/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/



#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <search.h>

#include "y.tab.h"

#include "idinfo.h"

#define YYDEBUG 1
# define YYLMAX BUFSIZ

extern char *yysptr, yysbuf[];
int yytchar;

#ifdef ECHO
#undef ECHO
#endif
#define ECHO echo(yytext)

static void echo(char *s)
{
  char  loc[YYLMAX];
  int   l;
  l=strlen(s);
  loc[l]='\0';
  while(l--) loc[l]=toupper(*(s+l));  
  fprintf(yyout, "%s", loc);
}

int yycolumno=0;

#define MVL_LG_MC 15
#define MVL_NB_MC 44

int MVL_LINNUM=1;

typedef struct {
        char nom[MVL_LG_MC];
        int kval;
        } el_mc;

static el_mc tab_mc []=
  {
	{"not",LOG_NOT},
	{"and",Log_And},
	{"or",Log_Or},
	{"ite",Ite},
    {"define", Define},
    {"bool", Bool},
    {"real", Real},
    {"Int", Int},
    {"assert", Assert},
    {"check", Check},
    {"definetype", Define_Type},
	{"utype", UType},
	{"BitVec", BitVector},
	{"bvadd",BV_ADD},
	{"bvmul",BV_MUL},
	{"bvsub",BV_SUB},
	{"bvudiv",BV_UDIV},
	{"bvult",BV_ULT},
	{"bvugt",BV_UGT},
	{"bvule",BV_ULE},
	{"bvuge",BV_UGE},
	{"bvxor",BV_XOR},
	{"iff",IFF},
        {"benchmark", Benchmark},
        {"extrafuns", Extrafuns},
	{"extrapreds", Extrapreds},
	{"formula", Formula},
	{"assumption", Assumption},
	{"if_then_else",Ifthenelse},
	{"extract",Extract},
	{"concat",Concat},
	{"bvor",BV_OR},
	{"let",Let},
	{"flet",Flet},
	{"bvand",BV_AND},
	{"bvnot",BV_NOT},
	{"bvneg",BV_NEG},
	{"bvshl",BV_SHL},
	{"bvsle",BV_SLE},
	{"bvsge",BV_SGE},
	{"bvlshr",BV_LSHR},
	{"zero_extend",ZERO_EXTEND},
	{"sign_extend",SIGN_EXTEND},
        {"exists",Exists}
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
  //while(l--) loc[l]=tolower(loc[l]);	/* conversion en minuscules */
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
space_character 	  [ \t]
end_of_line		  \n
lower_case_letter 	  [a-z]

letter		   	  ({upper_case_letter}|{lower_case_letter})
letter_or_digit	   	  ({letter}|{digit})
decimal_literal	   	  {integer}(\.{integer})?({exponent})?
integer		   	  (-)?{digit}(_?{digit})*
exponent	   	  ([eE][-+]?{integer})
base		   	  {integer}
bitvector_literal_dec         (bv)({digit})*


%%

\=						 		        {
			ECHO; return(t_EQSym);
									}
\&									{
			ECHO; return(t_Ampersand);
									}
\(								        {
			ECHO; return(t_LeftParen);
									}
\)									{
			ECHO; return(t_RightParen);
									}
\*									{
			ECHO; return(t_Star);
									}
\+									{
			ECHO; return(t_Plus);
									}
\-									{
			ECHO; return(t_Minus);
									}
\~                                                                      {
                        ECHO; return(t_UnaryMinus);
                                                                            }
"::"									{
			ECHO; return(t_doubleColon);
									}
\:									{
			ECHO; return(t_Colon);
									}
\;									{
			ECHO; return(t_Semicolon);
									}
"<="	{
	    ECHO;
	    return(t_LESym);
	}
">="	{
            ECHO;
	    return(t_GESym);
	}
\<	{
            ECHO;
	    return(t_LTSym);
	}
\>	{
            ECHO;
	    return(t_GTSym);
	}
\/=									{
			ECHO; return(t_NESym);
									}
"=>"	{
            ECHO;
	    return(t_Arrow);
	}
"<>"	{
            ECHO;
	    return(t_Box);
	}
\|									{
			ECHO; return(t_Bar);
									}
\.									{
			ECHO; return(t_Dot);
									}
\/									{
			ECHO; return(t_Slash);
									}

\[						 		        {
			ECHO; return(t_LeftBrack);
									}

\]						 		        {
			ECHO; return(t_RightBrack);
									}


"false"  {
  yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
  yylval.ds_Id->len=strlen(yytext);
  yylval.ds_Id->name= strdup(yytext);
  yylval.ds_Id->type= t_BoolLit;
	
  ECHO; return ( t_BoolLit );
}

"true"  {
  yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
  yylval.ds_Id->len=strlen(yytext);
  yylval.ds_Id->name= strdup(yytext);
  yylval.ds_Id->type= t_BoolLit;
	
  ECHO; return ( t_BoolLit );
}


{bitvector_literal_dec} {
  yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
  yylval.ds_Id->len=strlen(yytext);
  yylval.ds_Id->name= strdup(yytext);
  yylval.ds_Id->type= t_BvLit_Dec;
	
  ECHO; return ( t_BvLit_Dec );
}

{letter}(_|-|{letter_or_digit})* 	{
	int itoken;
	itoken=find_mc(yytext);

	if (itoken== -1) 
	{
	  ECHO;
	  yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	  yylval.ds_Id->len=strlen(yytext);
	  yylval.ds_Id->name= strdup(yytext);
	  yylval.ds_Id->type= t_Identifier;
 	  return ( t_Identifier );
	}
	else
	{
	  ECHO; return ( itoken );
	}
}

{integer} {
        yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	yylval.ds_Id->len=strlen(yytext);
	yylval.ds_Id->name= strdup(yytext);
	yylval.ds_Id->type= t_AbstractLit;
	
	ECHO; return ( t_AbstractLit );
	}

($){letter}(_|-|{letter_or_digit})* 	{
	int itoken;
	itoken=find_mc(yytext);

	if (itoken== -1) 
	{
	  ECHO;
	  yylval.ds_Id = (struct IDinfo*) malloc(sizeof(struct IDinfo));	
	  yylval.ds_Id->len=strlen(yytext);
	  yylval.ds_Id->name= strdup(yytext);
	  yylval.ds_Id->type= t_Var;
 	  return ( t_Var );
	}
	else
	{
	  ECHO; return ( itoken );
	} 
}


%%

int yywrap(void) {
return 1;
}

