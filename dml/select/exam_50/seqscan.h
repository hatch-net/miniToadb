#ifndef HAT_SEQSCAN_H
#define HAT_SEQSCAN_H

typedef struct RelationInfo RelationInfo;
typedef struct TupleHeader TupleHeader;

typedef struct ScanInfo
{
    RelationInfo *rel;
    int pageNum;
    int item_offset; 
}ScanInfo;


TupleHeader* SeqScanNext(ScanInfo* scaninfo);

#endif 