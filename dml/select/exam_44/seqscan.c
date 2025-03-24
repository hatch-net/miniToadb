#include "seqscan.h"
#include "pages.h"
#include "dictionary.h"

#include <stdlib.h>
#include <string.h>

TupleHeader* SeqScanNext(ScanInfo* scaninfo)
{
    PageDataHeader *currentPage = NULL;
    TupleHeader *nextTup = NULL;
    ItemData *currentItem = NULL;
    int itemIndex = 0;

    if((scaninfo == NULL) || (scaninfo->rel == NULL))
    {
        return NULL;
    }

    do {
        if(scaninfo->pageNum > scaninfo->rel->relstorage.totalPageNum)
        {
            /* end of relation file. */
            return NULL;
        }

        currentPage = ReadPage(scaninfo->rel, scaninfo->pageNum);
        if(scaninfo->item_offset >= currentPage->itemEndOffset)
        {
            /* end of current block, scan next block. */
            scaninfo->pageNum ++;
            scaninfo->item_offset = PAGE_ITEM_START;
            continue;
        }

        itemIndex = PAGE_ITEM_INDEX(scaninfo->item_offset);
        currentItem = currentPage->item + itemIndex;

        nextTup = (TupleHeader *)malloc(currentItem->len + sizeof(TupleHeader));
        nextTup->size = currentItem->len;
        memcpy((char *)nextTup + sizeof(TupleHeader), (char *)currentPage + currentItem->offset, nextTup->size);
        
        scaninfo->item_offset += sizeof(ItemData);
    }while(nextTup == NULL);

    return nextTup;
}

