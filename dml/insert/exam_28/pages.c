#include "pages.h"
#include "dictionary.h"
#include "hatfile.h"
#include "node.h"

#include <stdlib.h>
#include <string.h>

void ExtensionPage(RelationInfo *relInfo);

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
    char page[PAGE_MAX_SIZE];

    InitPage((PageHeader*)page, relInfo->relstorage.totalPageNum+1);
    WritePage(relInfo, (PageDataHeader*)page);

    relInfo->relstorage.totalPageNum += 1;
}
