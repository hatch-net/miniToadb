#ifndef HAT_DICTIONARY_H
#define HAT_DICTIONARY_H

#include "link.h"

#define HAT_DICTIONARY_FILE     "relationmap"
#define DICTIONARY_VERSION       (0x2B3C)



typedef enum DataType
{
    DT_INTEGER,
    DT_FLOAT,
    DT_CHAR,
    DT_BOOL,
    DT_VARCHAR,    
    DT_TEXT,
    DT_END
}DataType;

typedef union Data
{
	void *pData;
	int  iData;
	char cData;
	double dData;
	float fData;
}Data;

#define IDENTIFY_NAME_LEN       64
#define RELATION_NAME_MAX_LEN   IDENTIFY_NAME_LEN
#define ATTR_NAME_MAX_LEN       IDENTIFY_NAME_LEN

typedef struct RelStorage
{
    int             fileDesc;
    int             totalPageNum;
    int             isDirt;
    unsigned int    dictionaryFileOffset;
    char            *page;
}RelStorage;

#pragma pack(push,1)

typedef struct AttrInfo
{
    int attrIndex;      // start 1
    char attrName[ATTR_NAME_MAX_LEN];
    DataType attrType;
    int maxWidth;
}AttrInfo;

typedef struct RelationInfoHeader
{
    int relId;        // start 1
    char relName[RELATION_NAME_MAX_LEN];
    int attrNum;
}RelationInfoHeader;

typedef struct RelationInfo
{
    int relId;        // start 1
    char relName[RELATION_NAME_MAX_LEN];
    int attrNum;
    RelStorage relstorage;
    AttrInfo attrs[1];
}RelationInfo;
#pragma pack(push,1)

#define RelationInfoSize(attrNum) (sizeof(RelationInfo) + ((attrNum) - 1) * sizeof(AttrInfo))

typedef struct RelInfoStorageInfo
{
    int             fd; 
    unsigned int    fileSize;
}RelInfoStorageInfo;

#define DICTIONARY_FILE_HEADER_SIZE (sizeof(int)*2)
typedef struct RelInfoCache
{
    int                 relInfoVersion;
    int                 relNum;
    int                 relNextId;
    LinkNode            head;
    RelInfoStorageInfo  cacheStorageInfo;
}RelInfoCache;

typedef struct RelInfoCacheNode
{
    LinkNode link;
    RelationInfo *relInfo;
}RelInfoCacheNode;

char* GetDataTypeName(DataType type);
DataType GetDataType(char *typeName);

int InsertDictionary(RelationInfo *relInfo, int isLoad);
RelationInfo * FindRelInfoByName(char *relName);
RelationInfo* DeleteRelInfoByName(char *relName);

int InitializeDictionary();
void DestroyDictionary();

void FlushRelInfoCache(RelationInfo *relInfo);

typedef struct CreateStmt CreateStmt;
RelationInfo *InitRelationInfo(CreateStmt *stmt);
void DestroyRelation(RelationInfo* relInfo);

RelationInfo* OpenRelation(char *relName);
void CloseRelation(RelationInfo* relInfo);

#endif 
