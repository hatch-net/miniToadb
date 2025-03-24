#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlparser.h"
#include "sqlscanner.h"
#include "sqlgram.h"
#include "node.h"
#include "executor.h"
#include "dictionary.h"
#include "plan.h"

#define SQL_MAX_LEN 256

yyscan_t scanner_init(char *sqlStr, ParserTree *pt);

void InitToad();
void ExitToad();
int ReadInput(char *buf, int maxSize);

int main(int argc, char *argv[])
{
    char sqlStr[SQL_MAX_LEN] = {0};
    yyscan_t scannerinfo ;
    ParserTree pt;
    Node *plan = NULL;
    int yyresult = 0;

    InitToad();
    
    printf("MiniToad Database %s \n", MINITOADB_MAIN_VERION);
    
    while(1)
    {
        memset(&pt, 0x00, sizeof(pt));

        printf("miniToad <# ");

        ReadInput(sqlStr, SQL_MAX_LEN);
        if(strcmp(sqlStr, "quit\n") == 0)
        {
            break;
        }
        else if(strcmp(sqlStr, "\n") == 0)
        {
            continue;
        }

        scannerinfo = scanner_init(sqlStr, &pt);
        
        yyresult = yyparse(scannerinfo);        
        yylex_destroy(scannerinfo);
        
        if(yyresult)
            continue;   /* parse error */

        travelParserTree(pt.root);

        plan = PlanGenerator(pt.root);
    
        Executor(plan);
    }

    ExitToad();

    printf("MiniToadb is exit(0)\n");

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

int ReadInput(char *buf, int maxSize)
{
    int offset = 0;

    memset(buf, 0x00, maxSize);
    do
    {
        if(fgets(buf + offset, maxSize - offset, stdin) == NULL)
        {
            if(feof(stdin))
            {
                clearerr(stdin);
            }
        }

        offset = strlen(buf);
    }while(offset == 0);

    return offset;
}

void InitToad()
{
    int ret = 0;
    
    ret = InitializeDictionary();
    if(ret < 0)
        exit(ret);
}

void ExitToad()
{
    DestroyDictionary();
}