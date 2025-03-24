#ifndef HAT_DICTIONARY_H
#define HAT_DICTIONARY_H

#include "link.h"

#define HAT_DICTIONARY_FILE     "relationmap"

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

#define IDENTIFY_NAME_LEN       64
#define RELATION_NAME_MAX_LEN   IDENTIFY_NAME_LEN
#define ATTR_NAME_MAX_LEN       IDENTIFY_NAME_LEN

typedef struct AttrInfo
{
    int attrIndex;      // start 1
    char attrName[ATTR_NAME_MAX_LEN];
    DataType attrType;
}AttrInfo;

typedef struct RelationInfo
{
    int relId;        // start 1
    char relName[RELATION_NAME_MAX_LEN];
    int attrNum;
    AttrInfo attrs[1];
}RelationInfo;

#define RelationInfoSize(attrNum) (sizeof(RelationInfo) + ((attrNum) - 1) * sizeof(AttrInfo))

typedef struct RelInfoCache
{
    int relNum;
    LinkNode head;
}RelInfoCache;

typedef struct RelInfoCacheNode
{
    LinkNode link;
    RelationInfo *relInfo;
}RelInfoCacheNode;

char* GetDataTypeName(DataType type);
DataType GetDataType(char *typeName);

int InsertDictionary(RelationInfo *relInfo);
RelationInfo * FindRelInfoByName(char *relName);
RelationInfo* DeleteRelInfoByName(char *relName);

void DestroyDictionary();

typedef struct CreateStmt CreateStmt;
RelationInfo *InitRelationInfo(CreateStmt *stmt);

#endif 
