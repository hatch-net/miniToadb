#include "dictionary.h"
#include "node.h"

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

RelationInfo * FindRelInfoByName(char *relName)
{
    RelInfoCacheNode *cacheNode = NULL;

    if(relName == NULL)
        return NULL;
    
    cacheNode = (RelInfoCacheNode *)dictionaryCache.head.next;
    while(cacheNode != NULL)
    {
        if(strcasecmp(cacheNode->relInfo->relName, relName) == 0)   
            return cacheNode->relInfo;
        
        cacheNode = (RelInfoCacheNode *)cacheNode->link.next;
    } 
    
    return NULL;
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
    for(int i = 0; i < colNum; i++)
    {
        if((nextNode == NULL) || (nextNode->value == NULL))
            break;
        
        coldef = (ColumnDef*)nextNode->value;

        relInfo->attrs[i].attrIndex = i + 1;
        strncpy(relInfo->attrs[i].attrName, coldef->colName, ATTR_NAME_MAX_LEN);
        relInfo->attrs[i].attrType = GetDataType(coldef->colType);
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
            free(cacheNode->relInfo);
        free(cacheNode);

        cacheNode = (RelInfoCacheNode *)dictionaryCache.head.next;
    } 
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