/* 	
*	Property Translator - LEX
*	Author: Nikhil Saxena
*/
%{
	#include "propTransYacc.tab.h"
	void pperror(char *);
%}

upper_case_letter 	  [A-Z]
digit 			  [0-9]
lower_case_letter 	  [a-z]

letter		   	  ({upper_case_letter}|{lower_case_letter})
letter_or_digit	   	  ({letter}|{digit})

%%
(next)				{
					pplval.str = "next";
					return X;
				}
(eventually)			{
					pplval.str = "eventually";
					return F;
				}
(always)			{
					pplval.str = "always";
					return G;
				}
(never)				{
					pplval.str = "never";
					return N;
				}
0b[0-1]+			{
					pplval.str = pptext;
					return BVCONSTT;
				}

{letter}(_|{letter_or_digit})* {
					pplval.str = pptext;
					return IDENTIFIER;
				}
[0-9]+				{
					pplval.iVal = atoi(pptext);
					return INTEGER;
				}
"="				{
					pplval.str = "=";
					return OP;
				}
"<"				{
					pplval.str = "<";
					return OP;
				}
"<="				{
					pplval.str = "<=";
					return OP;
				}

"=>"				{
					pplval.str = "=>";
					return IMPLIES; 
				}
"&&"				{
					pplval.str = "&&";
					return AND; 
				}
"||"				{
					pplval.str = "||";
					return OR; 
				}
"!"				{
					pplval.str = "!";
					return NOT; 
				}
"-"				{
					pplval.str = "-";
					return ARITHOP; 
				}
"+"				{
					pplval.str = "+";
					return ARITHOP; 
				}
"*"				{
					pplval.str = "*";
					return ARITHOP; 
				}
"&"				{
					pplval.str = "&";
					return BV_BIN_OP;
				}
"|"				{
					pplval.str = "|";
					return BV_BIN_OP;
				}
"~"				{
					pplval.str = "~";
					return BV_NOT;
				}
":"				{
					pplval.str = "select";
					return SELECT;
				}
";"				{
					pplval.str = "sequence";
					return SEQUENCE;
				}
"@"				{ 	
					pplval.str = "concat";
					return CONCAT; 
				}
				
"{"				{
					pplval.str = "{";
					return L_BRACE;
				}
"}"				{
					pplval.str = "}";
					return R_BRACE;
				}
"("				{
					pplval.str = "(";
					return L_PAREN;
				}
")"				{
					pplval.str = ")";
					return R_PAREN;
				}
"\n"				{
					pplval.str = "\n";
					return NEW_LINE;
				}	
				
"["				{
					pplval.str = "[";
					return L_BRACKET;
				}
"]"				{
					pplval.str = "]";
					return R_BRACKET;
				}
[ \t]		   		;   	/* skip whitespace */
.           	    		pperror("Unknown character");
%%
int ppwrap(void) {
    return 1;
}
