#include "link.h"
#include <stdlib.h>

void InsertNodeHead(LinkNode *head, LinkNode *node)
{
    if(NULL == head)
        return;
    
    node->next = head->next;
    node->prev = head;

    if(head->next != NULL)
        head->next->prev = node;
    head->next = node;
    return;
}

void DelNode(LinkNode *head, LinkNode *node)
{
    if((NULL == head) || (head == node))
        return;

    node->prev->next = node->next;    
    if(node->next != NULL)
        node->next->prev = node->prev;
    return;
}