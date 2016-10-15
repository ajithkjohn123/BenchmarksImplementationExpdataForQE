%{
#include "y.tab.h"
#include <stdlib.h>
#include<string>
using namespace std;

%}

%%


"("               { printf("Reading string ( \n"); return t_OBrack;}
")"               { printf("Reading string ) \n");return t_CBrack;}
","               { printf("Reading string , \n"); return t_Comma;}


"="               {printf("Reading string = \n");return t_Eq;}




[-\.a-zA-Z_0-9/\[\]\$]+     { yylval.string_value=strdup(yytext); yylval.string_value[yyleng]='\0'; printf("Reading string %s \n",yylval);  return t_String;}


%%
int yywrap(void) {
return 1;
}

