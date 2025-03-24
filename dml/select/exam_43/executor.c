#include "executor.h"
#include "node.h"
#include "dictionary.h"
#include "hatfile.h"
#include "pages.h"

#include <stdio.h>
#include <stdlib.h>

int ExecCreateStmt(Node *rootNode);
int ExecDropStmt(Node *rootNode);
int ExecInsertStmt(Node *rootNode);

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
        
        case T_DropStmt:
            retCode = ExecDropStmt(node);
            printf("excutor DropStmt %s\n", retCode < 0 ? "failure":"success" );
        break;

        case T_InsertStmt:
            retCode = ExecInsertStmt(node);
            printf("excutor InsertStmt %s\n", retCode < 0 ? "failure":"success" );
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

int ExecDropStmt(Node *rootNode)
{
    DropStmt *stmt = (DropStmt*)rootNode;
    RelationInfo *relInfo = NULL;
    int ret = 0;

    relInfo = DeleteRelInfoByName(stmt->tableName);
    if(relInfo == NULL)
    {
        return -1;
    }
    CloseRelation(relInfo);

    ret = DeleteFile(stmt->tableName);

    return ret;
}


int ExecInsertStmt(Node *rootNode)
{
    InsertStmt *stmt = (InsertStmt*)rootNode;
    RelationInfo *relInfo = NULL;

    List* valuesList = (List*)stmt->valuesList;
    List* targetsList = (List*)stmt->attrNameList;

    ItemData item = {0};
    PageDataHeader *page = NULL;
    TupleHeader *tupHeader = NULL;

    char buff[PAGE_MAX_SIZE] = {0};
    int ret = 0;

    relInfo = OpenRelation(stmt->tableName);
    if(relInfo == NULL)
    {
        return -1;
    }

    /* treveal values list */
    for(ListNode *lnode = valuesList->head; lnode != NULL; lnode = lnode->next)
    {
        List *values = (List *)lnode->value;

        tupHeader = FormTupleData(relInfo, targetsList, values, buff, PAGE_MAX_SIZE);
        if(tupHeader == NULL)
            break;
            
        /* form item */
        item.len = tupHeader->size;

        /* get spacepage */
        if(page == NULL)
        {
            page = GetFreeSpacePage(relInfo, tupHeader->size, PAGE_NEW);
        }
        else if(!HasFreeSpace(page, tupHeader->size))
        {
            if(relInfo->relstorage.isDirt)
            {
                WritePage(relInfo, page);
                relInfo->relstorage.isDirt = 0;
            }
            page = GetFreeSpacePage(relInfo, tupHeader->size, PAGE_NEW);
        }

        /* insert */
        PutTupleToPage(page, &item, tupHeader);      
        relInfo->relstorage.isDirt = 1;      
    }

    /* close relation */
    CloseRelation(relInfo);

    return ret;
}
