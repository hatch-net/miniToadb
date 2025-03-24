#ifndef HAT_PAGES_H
#define HAT_PAGES_H

#define PAGE_MAX_SIZE (4096)      // 4KB 
#define PAGE_VERSION (0x2B3C)

#define PAGE_NUM_START  (1)

typedef struct PageHeader
{
    unsigned int pageVersion;
    unsigned int pageNum;
}PageHeader;

typedef struct ItemData
{
    int offset;
    int len;           
}ItemData;

typedef struct PageDataHeader
{
    PageHeader header;
    int itemEndOffset;     
    int dataStartOffset;  
    ItemData item[1];
}PageDataHeader;

typedef struct TupleHeader
{
    int size;
}TupleHeader;

typedef struct ColumnDataHeader
{
    int size;
}ColumnDataHeader;

#define PAGE_DATA_HEADER_SIZE (sizeof(PageDataHeader)) 
#define PAGE_ITEM_START       (PAGE_DATA_HEADER_SIZE - sizeof(ItemData))
#define HasFreeSpace(page, size) ((((PageDataHeader *)page)->dataStartOffset - ((PageDataHeader *)page)->itemEndOffset) >= ((size) + sizeof(ItemData)))

typedef enum PageOp
{
    PAGE_NEW,
    PAGE_EXIST
}PageOp;

typedef struct RelationInfo RelationInfo;
PageDataHeader* ReadPage(RelationInfo *rel, int blockNum);
void WritePage(RelationInfo *rel, PageDataHeader* page);

PageDataHeader*  GetFreeSpacePage(RelationInfo *rel, int size, PageOp flag);

typedef struct List List;
TupleHeader *FormTupleData(RelationInfo *relInfo, List *targetList, List *values, char *buff, int maxSize);

void PutTupleToPage(PageDataHeader *page, ItemData *item, TupleHeader *tup);

#endif 