#include "dictionary.h"
#include "node.h"
#include "pages.h"
#include "hatfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

static RelInfoCache dictionaryCache; 

int InsertDictionary(RelationInfo *relInfo, int isLoad)
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

    if(!isLoad)
    {
        relInfo->relId = dictionaryCache.relNextId++;
        FlushRelInfoCache(relInfo);
    }
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


void FlushRelInfoCache(RelationInfo *relInfo)
{
    unsigned int offset = dictionaryCache.cacheStorageInfo.fileSize;
    RelationInfoHeader *relHeader = NULL;
    int headData[2] = {DICTIONARY_VERSION, 0};

    if(relInfo->relstorage.dictionaryFileOffset != 0)
    {
        offset = relInfo->relstorage.dictionaryFileOffset;
    }

    relHeader = (RelationInfoHeader*)relInfo;
    offset += WriteFile(dictionaryCache.cacheStorageInfo.fd,
                        offset,
                        sizeof(RelationInfoHeader),
                        (char*)relHeader);

    offset += WriteFile(dictionaryCache.cacheStorageInfo.fd,
                        offset,
                        sizeof(AttrInfo) * relInfo->attrNum,
                        (char*)relInfo->attrs);

    if(relInfo->relstorage.dictionaryFileOffset == 0)
    {
        relInfo->relstorage.dictionaryFileOffset = dictionaryCache.cacheStorageInfo.fileSize;
        dictionaryCache.cacheStorageInfo.fileSize = offset;

        headData[1] = dictionaryCache.relNum;
        WriteFile(dictionaryCache.cacheStorageInfo.fd, 0, DICTIONARY_FILE_HEADER_SIZE, (char*)headData);
    }                       
}

int OpenRelationInfoFile()
{
    int fd = 0;
    int headData[2] = {0};

    fd = OpenFile(HAT_DICTIONARY_FILE, 0600);
    if(fd < 0)
    {
        fd = CreateFile(HAT_DICTIONARY_FILE, 0600);
        if(fd < 0)
        {
            printf("Dictionary file open failure.\n");
            return -1;
        }

        headData[0] = DICTIONARY_VERSION;
        headData[1] = 0;
        WriteFile(fd, 0, DICTIONARY_FILE_HEADER_SIZE, (char*)headData);
    }

    dictionaryCache.cacheStorageInfo.fd = fd;
    dictionaryCache.cacheStorageInfo.fileSize = DICTIONARY_FILE_HEADER_SIZE;
    return fd;
}

int LoadRelInfoCache()
{
    int dicHeader[2] = {0};
    int offset = DICTIONARY_FILE_HEADER_SIZE;
    int relIndex = 0;
    int relOffset = 0;
    RelationInfoHeader relHeader = {0};
    RelationInfo *relInfo = NULL;

    ReadFile(dictionaryCache.cacheStorageInfo.fd, 0, DICTIONARY_FILE_HEADER_SIZE, (char*)dicHeader);

    if(dicHeader[0] != DICTIONARY_VERSION)
    {
        printf("dictionary file version is invalid(%0x).",dicHeader[0]);
        return -1;
    }

    if(dicHeader[1] == 0)
    {
        return 0;
    }

    dictionaryCache.relInfoVersion = dicHeader[0];
    for(relIndex = 0; relIndex < dicHeader[1]; relIndex++)
    {
        relOffset = offset;
        offset += ReadFile(dictionaryCache.cacheStorageInfo.fd,
                            offset, 
                            sizeof(RelationInfoHeader),
                            (char*)&relHeader);

        if(relHeader.attrNum <= 0)
        {
            printf("dictionary file is distroyed.\n");
            return -1;                         
        }

        relInfo = (RelationInfo *)malloc(RelationInfoSize(relHeader.attrNum));
        if(relInfo == NULL)
            return -1;

        relInfo->relId = relHeader.relId;
        strncpy(relInfo->relName, relHeader.relName, RELATION_NAME_MAX_LEN);
        relInfo->attrNum = relHeader.attrNum;

        offset += ReadFile(dictionaryCache.cacheStorageInfo.fd,
                            offset, 
                            sizeof(AttrInfo) * relInfo->attrNum,
                            (char*)relInfo->attrs);

        relInfo->relstorage.dictionaryFileOffset = relOffset;     

        if(InsertDictionary(relInfo, 1) < 0)
            return -1;
        
        dictionaryCache.relNextId = relInfo->relId + 1;
    }

    return relInfo->attrNum;
}

int InitializeDictionary()
{
    int ret = 0;

    ret = OpenRelationInfoFile();
    if(ret < 0)
        return ret;
    
    ret = LoadRelInfoCache();
    return ret;
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
            FlushRelInfoCache(cacheNode->relInfo);
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