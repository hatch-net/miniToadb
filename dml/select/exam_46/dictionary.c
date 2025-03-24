#include "dictionary.h"
#include "node.h"
#include "pages.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

static RelInfoCache dictionaryCache; 

int InsertDictionary(RelationInfo *relInfo)
{
    RelInfoCacheNode *relCacheNode = NULL;

    if(NULL == relInfo)
        return -1;

    relCacheNode = (RelInfoCacheNode *)malloc(sizeof(RelInfoCacheNode));
    if(NULL == relCacheNode)
    {
        return -1;
    }

    relCacheNode->relInfo = relInfo;

    InsertNodeHead(&(dictionaryCache.head), (LinkNode*)relCacheNode);
    dictionaryCache.relNum += 1;

    relInfo->relId = dictionaryCache.relNum;

    return 0;
}

RelInfoCacheNode* FindRelInfoCacheNodeByName(char *relName)
{
    RelInfoCacheNode *cacheNode = NULL;

    if(relName == NULL)
        return NULL;
    
    cacheNode = (RelInfoCacheNode *)dictionaryCache.head.next;
    while(cacheNode != NULL)
    {
        if(strcasecmp(cacheNode->relInfo->relName, relName) == 0)   
            return cacheNode;
        
        cacheNode = (RelInfoCacheNode *)cacheNode->link.next;
    } 
    
    return NULL;
}

RelationInfo* FindRelInfoByName(char *relName)
{
    RelInfoCacheNode *cacheNode = NULL;

    if(relName == NULL)
        return NULL;
    
    cacheNode = FindRelInfoCacheNodeByName(relName);
    if(cacheNode != NULL)
        return cacheNode->relInfo;

    return NULL;
}

RelationInfo* DeleteRelInfoByName(char *relName)
{
    RelInfoCacheNode* cacheNode = NULL;
    RelationInfo* relInfo = NULL;

    if(relName == NULL)
        return NULL;
    
    cacheNode = FindRelInfoCacheNodeByName(relName);
    if(cacheNode != NULL)
    {
        relInfo = cacheNode->relInfo;
        DelNode(&(dictionaryCache.head), (LinkNode*)cacheNode);

        free(cacheNode);
    }

    return relInfo;
}

RelationInfo *InitRelationInfo(CreateStmt *stmt)
{
    RelationInfo *relInfo = NULL;
    List *colList = NULL;
    ListNode* nextNode = NULL;
    ColumnDef* coldef = NULL;
    int colNum = 0;

    if(stmt == NULL)
        return NULL;

    colList = (List*)stmt->colList;
    if(colList == NULL || colList->length <= 0)
    {
        return NULL;
    }
    colNum = colList->length;

    relInfo = (RelationInfo *)malloc(RelationInfoSize(colNum));
    if(relInfo == NULL)
        return NULL;

    strncpy(relInfo->relName, stmt->tableName, RELATION_NAME_MAX_LEN);
    relInfo->attrNum = colNum;

    nextNode = colList->head;
    for(int i = 0; (i < colNum) && (nextNode != NULL); i++, nextNode = nextNode->next)
    {
        if((nextNode == NULL) || (nextNode->value == NULL))
            break;
        
        coldef = (ColumnDef*)nextNode->value;

        relInfo->attrs[i].attrIndex = i + 1;
        strncpy(relInfo->attrs[i].attrName, coldef->colName, ATTR_NAME_MAX_LEN);
        
        relInfo->attrs[i].attrType = GetDataType(coldef->colType);
        if(relInfo->attrs[i].attrType == DT_END)
        {
            printf("data type (%s) is invalid.", coldef->colType);
            return NULL;
        }

        relInfo->attrs[i].maxWidth = strlen(coldef->colName);
    }

    return relInfo;
}

void DestroyDictionary()
{
    RelInfoCacheNode *cacheNode = NULL;
   
    cacheNode = (RelInfoCacheNode *)dictionaryCache.head.next;
    while(cacheNode != NULL)
    {       
        dictionaryCache.head.next = cacheNode->link.next;
        
        if(cacheNode->relInfo != NULL)
        {
            DestroyRelation(cacheNode->relInfo);
        }
        free(cacheNode);

        cacheNode = (RelInfoCacheNode *)dictionaryCache.head.next;
    } 
}


RelationInfo* OpenRelation(char *relName)
{    
    /* relation info */
    RelationInfo* relInfo = FindRelInfoByName(relName);
    if(relInfo == NULL)
    {
        printf("table %s is not found.\n", relName);
        return NULL;
    }

    if(relInfo->relstorage.fileDesc > 0)
    {
        return relInfo;
    }
    
    /* open relation file */
    if((relInfo->relstorage.fileDesc = OpenFile(relInfo->relName, 0600)) < 0)
    {
        printf("tablefile %s is not found.\n", relName);
        return NULL;
    }

    relInfo->relstorage.totalPageNum = GetFileBlockNum(relInfo->relstorage.fileDesc, PAGE_MAX_SIZE);

    return relInfo;
}

void DestroyRelation(RelationInfo* relInfo)
{
    if(relInfo->relstorage.page != NULL)
    {
        if(relInfo->relstorage.isDirt)
        {
            WritePage(relInfo, (PageDataHeader*)relInfo->relstorage.page);
        }

        free(relInfo->relstorage.page);
        relInfo->relstorage.page = NULL;
    }

    if(relInfo->relstorage.fileDesc > 0)
    {
        CloseFile(relInfo->relstorage.fileDesc);
        relInfo->relstorage.fileDesc = -1;
    }

    free(relInfo);
    return ;
}

void CloseRelation(RelationInfo* relInfo)
{
    if(relInfo == NULL)
        return ;
        
    if(relInfo->relstorage.page != NULL)
    {
        if(relInfo->relstorage.isDirt)
        {
            WritePage(relInfo, (PageDataHeader*)relInfo->relstorage.page);
        }
    }

    return ;
}

char *dataNameArray[] = {"integer","float","char","bool","varchar","text","unknown"};

DataType GetDataType(char *typeName)
{
    DataType type = DT_INTEGER;

    for(; type < DT_END; type++)
    {
        if(strcasecmp(typeName, dataNameArray[type]) == 0)
            return type;
    }
    
    return DT_END;
}

char* GetDataTypeName(DataType type)
{
    if(type <= DT_END)
        return dataNameArray[type];
    
    return NULL;
}