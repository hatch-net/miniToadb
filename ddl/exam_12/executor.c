#include "executor.h"
#include "node.h"
#include "dictionary.h"
#include "hatfile.h"
#include <stdio.h>

int ExecCreateStmt(Node *rootNode);

void Executor(Node *node)
{
    int retCode = 0;

    if(node == NULL)
        return;
    
    switch(node->type)
    {
        case T_CreateStmt:
            retCode = ExecCreateStmt(node);
            printf("excutor CreateStmt %s\n", retCode < 0 ? "failure":"success" );
        break;
        default:
        break;
    }
    
    return ;
}

int ExecCreateStmt(Node *rootNode)
{
    int fd = 0;
    int ret = 0;
    CreateStmt *stmt = (CreateStmt*)rootNode;
    RelationInfo *relInfo = NULL;

    fd = CreateFile(stmt->tableName, 0600);
    if(fd < 0)
        return -1;
    CloseFile(fd);

    relInfo = InitRelationInfo(stmt);
    if(relInfo == NULL)
    {
        return -1;
    }

    ret = InsertDictionary(relInfo);
    return ret;
}

