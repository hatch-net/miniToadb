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

#endif 