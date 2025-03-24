#ifndef HAT_PORTAL_H
#define HAT_PORTAL_H


#define PORT_BUFFER_SIZE 1024

typedef struct RelationInfo RelationInfo;
typedef struct TupleHeader TupleHeader;

void PortalShowRowData(RelationInfo *rel, TupleHeader *tupleData, int showHead);

typedef struct Node Node;
void PortalShowResult(Node *result, Node *targetList, int showHead);

#endif 