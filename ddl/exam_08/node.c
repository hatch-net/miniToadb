#include "node.h"
#include <stdlib.h>
#include <stdio.h>


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

static void ShowBlank(int level)
{
    for(int i = level; i > 0; i--)
    {
        printf("  ");
    }
}

static void ShowNode(Node* n, int level);
static void TravelListNode(List* node, int level)
{
    ListNode* nextNode = NULL;
  
    if(NULL == node)
    {
        printf("list node is null\n");
        return;
    }

    ShowBlank(level+1);
    printf("length:%d \n", node->length);

    /* list cell node show */
    for(nextNode = node->head; nextNode != NULL; nextNode = nextNode->next)
    {
        Node* node = (Node*)nextNode->value;
        ShowNode(node, level+1);
    }

    return;
}

static void ShowNodCreateStmt(CreateStmt* node, int level)
{
    ShowBlank(level+1);
    printf("tablename:%s \n", node->tableName);

    ShowNode(node->colList, level+1);
    return;
}

static void ShowNodColumnDef(ColumnDef* node, int level)
{
    ShowBlank(level+1);
    printf("colName:%s \n", node->colName);

    ShowBlank(level+1);
    printf("colType:%s \n", node->colType);
    return;
}

static void ShowNode(Node* n, int level)
{
    if(NULL == n)
    {
        printf("node is null\n");
        return;
    }

    /* list node show */
    ShowBlank(level);
    printf("{ \n");

    switch(n->type)
    {
        case T_List:
            TravelListNode((List*)n, level);
        break;
        
        case T_CreateStmt:
            ShowNodCreateStmt((CreateStmt*)n, level);
        break;
        
        case T_ColumnDef:
            ShowNodColumnDef((ColumnDef*)n, level);
        break;      
        
        default:
            ShowBlank(level);
        break;
    }

    ShowBlank(level);
    printf("}   \n");
}

void travelParserTree(Node* root)
{
    printf("parser tree :\n");
    ShowNode(root, 1);
    return ;
}