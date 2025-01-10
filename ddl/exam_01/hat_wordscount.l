/* 
 * flex 
*/
%{
#include <stdio.h>
#include <string.h>

int chars = 0;
int words = 0;
int lines = 0;
%}
%% 
[a-zA-Z]+  { 
				words++; 
				chars += strlen(yytext);
		    }
\n 		    {
				chars++; 
				lines++;
			}
.			{
				chars++;
			}
%%
int main(int argc, char *argv[])
{
	yylex();

	printf(" lines:%8d\n words:%8d\n chars:%8d\n", lines, words, chars);
	return 0;
}

int yywrap()
{
	return 0;
}