#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlparser.h"
#include "sqlscanner.h"
#include "sqlgram.h"
#include "node.h"
#include "executor.h"
#include "dictionary.h"

#define SQL_MAX_LEN 256

yyscan_t scanner_init(char *sqlStr, ParserTree *pt);

void ExitToad();

int main(int argc, char *argv[])
{
    char sqlStr[SQL_MAX_LEN] = {0};
    yyscan_t scannerinfo ;
    ParserTree pt;

    while(1)
    {
        fgets(sqlStr, SQL_MAX_LEN, stdin);
        if(strcmp(sqlStr, "quit\n") == 0)
        {
            break;
        }

        scannerinfo = scanner_init(sqlStr, &pt);
        
        yyparse(scannerinfo);
        yylex_destroy(scannerinfo);

        // travelParserTree(pt.root);
        Executor(pt.root);
    }

    ExitToad();
    return 0;
}

yyscan_t scanner_init(char *sqlStr, ParserTree *pt)
{
    yyscan_t yyscaninfo;
	
    yylex_init(&yyscaninfo);
    yy_scan_string(sqlStr,yyscaninfo);

    yyset_extra(pt,yyscaninfo);
    return yyscaninfo;
}

void ExitToad()
{
    DestroyDictionary();
}