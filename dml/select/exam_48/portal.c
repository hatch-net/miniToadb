#include "portal.h"
#include "dictionary.h"
#include "pages.h"
#include "node.h"

#include <stdlib.h>
#include <stdio.h>

void ShowTableHeadRaw(RelationInfo *rel);
void ShowTableHead(List *targetList);
void ShowDataByTypes(char *buf, int bufOffset, int dataType, int maxWidth, ColumnDataHeader *colData);

void PortalShowResult(Node *result, Node *targetList, int showHead)
{
    ListNode *lnode = NULL;
    TargetElement *te = NULL;
    ColumnDataHeader *colData = NULL;
    TupleHeader *tupleData = NULL;

    int attno = 0;
    int dataOffset = sizeof(TupleHeader);
    char buf[PORT_BUFFER_SIZE] = {0};
    int bufOffset = 1;

    if((result == NULL) || (targetList == NULL))
        return ;

    if(showHead > 0)
        ShowTableHead((List*)targetList);

    tupleData = ((ResultNode*)result)->tup;
    buf[0] = '|';
    for(lnode = ((List*)targetList)->head; lnode != NULL; lnode = lnode->next)
    {
        te = (TargetElement*)lnode->value;

        colData = (ColumnDataHeader *)((char *)tupleData + dataOffset);
        ShowDataByTypes(buf, bufOffset, te->attr->attrType, te->attr->maxWidth, colData);

        dataOffset += colData->size;
        bufOffset += te->attr->maxWidth + 1;
    }

    printf("%s\n", buf);
    return ;
}

void PortalShowRowData(RelationInfo *rel, TupleHeader *tupleData, int showHead)
{
    int attno = 0;
    int dataOffset = sizeof(TupleHeader);
    ColumnDataHeader *colData = NULL;
    char buf[PORT_BUFFER_SIZE] = {0};
    int bufOffset = 1;

    if(showHead > 0)
        ShowTableHeadRaw(rel);

    buf[0] = '|';
    for(attno = 0; attno < rel->attrNum; attno ++)
    {
        colData = (ColumnDataHeader *)((char *)tupleData + dataOffset);
        ShowDataByTypes(buf, bufOffset, rel->attrs[attno].attrType, rel->attrs[attno].maxWidth, colData);

        dataOffset += colData->size;
        bufOffset += rel->attrs[attno].maxWidth + 1;
    }

    printf("%s\n", buf);
    return ;
}

void ShowTableHead(List *targetList)
{
    ListNode *lnode = NULL;
    TargetElement *te = NULL;

    char buf[PORT_BUFFER_SIZE] = {0};
    int bufOffset = 0;
    int index = 0;
    int colNameSize = 0;
    
    for(lnode = targetList->head; lnode != NULL; lnode = lnode->next)
    {
        te = (TargetElement*)lnode->value;

        snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "|%-*s", te->attr->maxWidth, te->alias);
        bufOffset += te->attr->maxWidth + 1;
    }

    snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "|");
    printf("%s\n", buf);
    return ;
}

void ShowTableHeadRaw(RelationInfo *rel)
{
    char buf[PORT_BUFFER_SIZE] = {0};
    int bufOffset = 0;
    int index = 0;
    int colNameSize = 0;
       
    for(index = 0; index < rel->attrNum; index++)
    {
        snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "|%-*s", rel->attrs[index].maxWidth, rel->attrs[index].attrName);
        bufOffset += rel->attrs[index].maxWidth + 1;
    }

    snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "|");
    printf("%s\n", buf);
    return ;
}


void ShowDataByTypes(char *buf, int bufOffset, int dataType, int maxWidth, ColumnDataHeader *colData)
{
    int colDataOffset = sizeof(ColumnDataHeader);

    switch(dataType)
    {
    case DT_INTEGER:
        {
            int *data = (int*)((char*)colData + colDataOffset);
            snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "%-*d|", maxWidth, *data);
        }
        break;
    case DT_FLOAT:
        {
            float *data = (float*)((char*)colData + colDataOffset);
            snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "%-*f|", maxWidth, *data);
        }
        break;
    case DT_CHAR:
        {
            char *data = (char*)((char*)colData + colDataOffset);
            snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "%-*c|", maxWidth, *data);
        }    
        break;
    case DT_BOOL:
        {
            int *data = (int*)((char*)colData + colDataOffset);
            snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "%-*s|", maxWidth, *data > 0? "t":"f");
        }    
        break;
    case DT_VARCHAR:
        {
            char *data = (char*)((char*)colData + colDataOffset);
            snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "%-*s|", maxWidth, data);
        }    
        break;   
    case DT_TEXT:
        {
            char *data = (char*)((char*)colData + colDataOffset);
            snprintf(buf + bufOffset, PORT_BUFFER_SIZE - bufOffset, "%-*s|", maxWidth, data);
        }    
        break;
    default:
        break;
    }
    return ;
}

