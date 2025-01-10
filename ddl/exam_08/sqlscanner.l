/* 
 * flex 
*/
%{
#include <stdio.h>
#include <string.h>

#include "sqlparser.h"
#include "sqlgram.h"

%}

%option case-insensitive
%option noyywrap

%option reentrant
%option bison-bridge
%option header-file="sqlscanner.h"

space       [ \t\n\r\f]
nonewline   [^\n\r]
comment     ("--"{nonewline}*)
whitespace	({space}+|{comment})

identify    [a-zA-Z][a-zA-Z0-9_]*

flagself	[,().;\+\-\*\/\%\<\>\=]

%% 
CREATE          {
                    return CREATE; 
                }
TABLE	        {
		            return TABLE;  
	            }
{identify}      {
                    yylval->sval = strdup(yytext);
                    return IDENT; 
                }
{flagself}      {
                    return yytext[0];  
                }
{whitespace}    {
                    ; /* ignore */
                }

%%

