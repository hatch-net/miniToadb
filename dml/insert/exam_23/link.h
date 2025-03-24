#ifndef HAT_LINKNODE_H
#define HAT_LINKNODE_H


typedef struct LinkNode
{
    struct LinkNode *next;
    struct LinkNode *prev;
}LinkNode;

void InsertNodeHead(LinkNode *head, LinkNode *node);
void DelNode(LinkNode *head, LinkNode *node);

#endif 
