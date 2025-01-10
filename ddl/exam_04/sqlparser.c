#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlparser.h"
#include "sqlscanner.h"
#include "sqlgram.h"

#define SQL_MAX_LEN 256

yyscan_t scanner_init(char *sqlStr);

int main(int argc, char *argv[])
{
    char sqlStr[SQL_MAX_LEN] = {0};
    yyscan_t scannerinfo ;

    while(1)
    {
        fgets(sqlStr, SQL_MAX_LEN, stdin);
        if(strcmp(sqlStr, "quit\n") == 0)
        {
            break;
        }

        scannerinfo = scanner_init(sqlStr);
        if(NULL == scannerinfo)
        {
            printf("init scanner null\n");
            break;
        }
        yyparse(scannerinfo);
        yylex_destroy(scannerinfo);
    }

    return 0;
}

yyscan_t scanner_init(char *sqlStr)
{
    yyscan_t yyscaninfo;
	
    yylex_init(&yyscaninfo);
    yy_scan_string(sqlStr,yyscaninfo);

    return yyscaninfo;
}