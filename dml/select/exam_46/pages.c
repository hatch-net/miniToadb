#include "pages.h"
#include "dictionary.h"
#include "hatfile.h"
#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ExtensionPage(RelationInfo *relInfo);
int FormColumnData(AttrInfo *attr, ValuesData *value, char *colData, int maxSize);

PageHeader *NewPage()
{
    PageHeader *pageData = malloc(PAGE_MAX_SIZE);
    if (NULL == pageData)
        exit(-1);

    memset(pageData, 0x00, PAGE_MAX_SIZE);
    return pageData;
}

void InitPage(PageHeader *page, int pageNum)
{
    PageDataHeader *pageDataHeader = (PageDataHeader *)page;

    memset(page, 0x00, PAGE_MAX_SIZE);
    page->pageVersion = PAGE_VERSION;
    page->pageNum = pageNum;

    pageDataHeader->dataStartOffset = PAGE_MAX_SIZE;
    pageDataHeader->itemEndOffset = PAGE_ITEM_START;
}

PageDataHeader *ReadPage(RelationInfo *rel, int blockNum)
{
    PageDataHeader *pageData = NULL;
    unsigned long offset = 0;

    if (rel == NULL)
        return NULL;

    if (blockNum > rel->relstorage.totalPageNum)
        return NULL;

    pageData = (PageDataHeader *)rel->relstorage.page;
    if (pageData != NULL)
    {
        if (blockNum == pageData->header.pageNum)
            return pageData;
    }
    else
    {
        pageData = (PageDataHeader *)NewPage();
        rel->relstorage.page = (char *)pageData;
    }

    offset = (unsigned long)(blockNum - 1) * PAGE_MAX_SIZE;
    if (ReadFile(rel->relstorage.fileDesc, offset, PAGE_MAX_SIZE, rel->relstorage.page) < 0)
    {
        return NULL;
    }

    return pageData;
}

void WritePage(RelationInfo *rel, PageDataHeader *page)
{
    unsigned long offset = 0;
    int ret = 0;

    if (rel == NULL || page == NULL)
        return;

    offset = (unsigned long)(page->header.pageNum - 1) * PAGE_MAX_SIZE;
    ret = WriteFile(rel->relstorage.fileDesc, offset, PAGE_MAX_SIZE, (char *)page);
    if (ret < 0)
    {
        return;
    }

    if (page->header.pageNum > rel->relstorage.totalPageNum)
    {
        rel->relstorage.totalPageNum = page->header.pageNum;
    }

    SyncFile(rel->relstorage.fileDesc);
    return;
}

PageDataHeader* GetFreeSpacePage(RelationInfo *rel, int size, PageOp flag)
{
    PageDataHeader *page = NULL;
    int pageNum = PAGE_NUM_START;

    if (size + PAGE_DATA_HEADER_SIZE >= PAGE_MAX_SIZE)
    {
        return NULL;
    }

    do {
        for (;; pageNum++)
        {
            page = ReadPage(rel, pageNum);
            if (NULL == page)
                break;

            if (HasFreeSpace(page, size))
            {
                break;
            }
        }

        if (page == NULL && flag == PAGE_NEW)
        {
            ExtensionPage(rel);
        }
    }while(page == NULL);

    return page;
}

void ExtensionPage(RelationInfo *relInfo)
{
    PageHeader *page = NewPage();

    InitPage(page, relInfo->relstorage.totalPageNum+1);
    WritePage(relInfo, (PageDataHeader*)page);
}

TupleHeader *FormTupleData(RelationInfo *relInfo, List *targetList, List *values, char *buff, int maxSize)
{
    int rowDataSize = sizeof(TupleHeader);
    int colDataSize = 0;
    int isAll = 0;
    AttrInfo *attr = NULL;
    TupleHeader *tupHeader = NULL;
    ColumnDataHeader *columnData = NULL;
    ValuesData *value = NULL;
    ListNode *valueNode = NULL;

    if (targetList == NULL)
    {
        valueNode = values->head;
        isAll = 1;
    }
    else 
    {
        if(targetList->length != values->length)
        {
            printf("attrbutes is not equal values\n");
            return NULL;
        }
    }

    if (isAll && (relInfo->attrNum != values->length))
    {
        printf("attrbutes is not equal values\n");
        return NULL;
    }

    /* form tuple, is null */
    for (int i = 0; i < relInfo->attrNum; i++)
    {
        attr = relInfo->attrs + i;

        if (isAll)
        {
            value = (ValuesData *)valueNode->value;
            valueNode = valueNode->next;
        }
        else
        {
            value = GetValueByName(attr->attrName, targetList, values);
        }

        if (value == NULL)
        {
            // null value
            columnData = (ColumnDataHeader *)(buff + rowDataSize);
            columnData->size = sizeof(ColumnDataHeader);
            rowDataSize += columnData->size;
            continue;
        }

        colDataSize = FormColumnData(attr, value, buff + rowDataSize, maxSize - rowDataSize);
        if (colDataSize < 0)
            return NULL;

        rowDataSize += colDataSize;

        if(attr->maxWidth < colDataSize)
            attr->maxWidth = colDataSize;
    }

    tupHeader = (TupleHeader *)buff;
    tupHeader->size = rowDataSize;

    return tupHeader;
}

int FormColumnData(AttrInfo *attr, ValuesData *value, char *colData, int maxSize)
{
    int colDataSize = sizeof(ColumnDataHeader);
    ColumnDataHeader *colDataHeader = (ColumnDataHeader *)colData;

    switch (attr->attrType)
    {
    case DT_INTEGER:
    {
        int *val = (int *)(colData + colDataSize);
        if (value->dataType != DT_INTEGER)
            return -1;

        *val = value->dataVal.iData;
        colDataSize += sizeof(int);
    }
    break;

    case DT_FLOAT:
    {
        float *val = (float *)(colData + colDataSize);
        if (value->dataType != DT_FLOAT)
            return -1;

        *val = value->dataVal.fData;
        colDataSize += sizeof(int);
    }
    break;

    case DT_CHAR:
    {
        char *val = (char *)(colData + colDataSize);
        if (value->dataType != DT_VARCHAR)
            return -1;

        *val = ((char *)(value->dataVal.pData))[0];
        colDataSize += sizeof(char);
    }
    break;

    case DT_BOOL:
    {
        int *val = (int *)(colData + colDataSize);
        if (value->dataType != DT_INTEGER)
            return -1;

        *val = value->dataVal.iData;
        colDataSize += sizeof(int);
    }
    break;

    case DT_VARCHAR:
    {
        char *val = (char *)(colData + colDataSize);
        int len = strlen(value->dataVal.pData);

        if ((value->dataType != DT_VARCHAR) || (len > maxSize))
            return -1;

        strncpy(val, value->dataVal.pData, len + 1);
        colDataSize += len + 1;
    }
    break;

    case DT_TEXT:
    {
        char *val = (char *)(colData + colDataSize);
        int len = strlen(value->dataVal.pData);

        if ((value->dataType != DT_VARCHAR) || (len > maxSize))
            return -1;

        strncpy(val, value->dataVal.pData, len + 1);
        colDataSize += len + 1;
    }
    break;

    default:
        break;
    }

    colDataHeader->size = colDataSize;

    return colDataSize;
}

void PutTupleToPage(PageDataHeader *page, ItemData *item, TupleHeader *tup)
{
    page->dataStartOffset -= item->len;
    memcpy((char *)page + page->dataStartOffset, (char *)tup + sizeof(TupleHeader), item->len);

    item->offset = page->dataStartOffset;
    memcpy((char *)page + page->itemEndOffset, item, sizeof(ItemData));
    page->itemEndOffset += sizeof(ItemData);
    return;
}
