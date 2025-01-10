#include "node.h"
#include <stdlib.h>

Node* CreateNode(int size, NodeType type)
{
    Node* node = malloc(size);
    if(NULL == node)
    {
        exit(-1);
    }

    node->type = type;
}

ListNode* CreateListNode()
{
    ListNode* cell = malloc(sizeof(ListNode));
    if(NULL == cell)
    {
        exit(-1);
    }

    cell->next = NULL;
    cell->value = NULL;

    return cell;
}

List* AppendNode(List* list, Node* n)
{
    /* new list */
    if(NULL == list)
    {
        list = (List*)CreateNode(sizeof(List), T_List);
    }

    /* new listnode */
    ListNode* cell = CreateListNode();
    if(NULL == cell)
    {
        exit(-1);
    }
    cell->next = NULL;
    cell->value = n;

    /* append to list */
    if(list->length == 0)
    {
        list->head = cell;
        list->tail = cell;
    }
    else
    {
        list->tail->next = cell;
        list->tail = cell;
    }
    list->length += 1;

    return list;
}
