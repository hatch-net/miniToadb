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

string      '(\\.|''|[^'\n])*'|\"(\\.|\"\"|[^\"\n])*\"

intnumber   [-]?[0-9]+ 

/*  1.12 ; 12.  ; .12 ;  1E-23 */
floatnumber  -?[0-9]+"."[0-9]*|-?"."[0-9]+|-?[0-9]+"."[0-9]*E[-+]?[0-9]+|-?[0-9]+E[+-]?[0-9]+|-?[0-9]*"."[0-9]+E[+-]?[0-9]+


%% 
DROP            {
                    return DROP;
                }
CREATE          {
                    return CREATE; 
                }
TABLE	        {
		            return TABLE;  
	            }
INSERT          {
                    return INSERT;
                }
INTO            {
                    return INTO;
                }
VALUES          {
                    return VALUES;
                }
SELECT          {
                    return SELECT;
                } 
FROM            {
                    return FROM;
                }   
WHERE           {
                    return WHERE;
                }                                                   
ALL             {
                    return ALL;
                }
DISTINCT        {
                    return DISTINCT;
                }
AS              {
                    return AS;
                }     
JOIN            {
                    return JOIN;
                }                  
INNER           {
                    return INNER;
                }    
CROSS           {
                    return CROSS;
                }    
LEFT            {
                    return LEFT;
                }    
RIGHT           {
                    return RIGHT;
                }    
OUTER           {
                    return OUTER;
                }             
NATURAL         {
                    return NATURAL;
                }    
ON              {
                    return ON;
                }
USING           {
                    return USING;
                }


{identify}      {
                    yylval->sval = strdup(yytext);
                    return IDENT; 
                }
{string}        {
                    int len = 0;

                    /* skip ' or " charactor */
                    yylval->sval = strdup(yytext+1);
                    len = strlen(yylval->sval);
                    yylval->sval[len-1] = '\0';

                    return STRING;
                }
{intnumber}     {
                    yylval->ival = atoi(yytext);
                    return INTNUMBER;
                }
{floatnumber}   {
                    yylval->fval = atof(yytext);
                    return FLOATNUMBER;
                }                
{flagself}      {
                    return yytext[0];  
                }
{whitespace}    {
                    ; /* ignore */
                }

%%

