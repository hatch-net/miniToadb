#include "node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

Node* CreateNode(int size, NodeType type)
{
    Node* node = malloc(size);
    if(NULL == node)
    {
        exit(-1);
    }
    memset(node, 0x00, size);
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

ValuesData* GetValueByName(char *attrName, List *attrList, List *valueList)
{
    ValuesData *value = NULL;
    ListNode *valueNode = valueList->head;

    for(ListNode *node = attrList->head; 
            (node != NULL) && (valueNode != NULL); 
            node = node->next, valueNode = valueNode->next)
    {
        AttrName *attr = (AttrName*)node->value;

        if(strcasecmp(attrName, attr->attrName) == 0)
        {
            value = (ValuesData*)valueNode->value;
            break;
        }
    }

    return value;
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

static void ShowNodDropStmt(DropStmt* node, int level)
{
    ShowBlank(level+1);
    printf("tablename:%s \n", node->tableName);
}

static void ShowNodColumnDef(ColumnDef* node, int level)
{
    ShowBlank(level+1);
    printf("colName:%s \n", node->colName);

    ShowBlank(level+1);
    printf("colType:%s \n", node->colType);
    return;
}

static void ShowNodInsertStmt(InsertStmt* node, int level)
{
    ShowBlank(level+1);
    printf("tableName:%s \n", node->tableName);

    ShowNode(node->attrNameList, level+1);
    ShowNode(node->valuesList, level+1);
    return;
}

static void ShowNodAttrName(AttrName* node, int level)
{
    ShowBlank(level+1);
    printf("attrName:%s \n", node->attrName);

    return;
}

static void ShowNodValuesData(ValuesData* node, int level)
{
    //ShowBlank(level+1);
    //printf("colName:%s \n", node->colName);

    ShowBlank(level+1);
    printf("colType:%s \n", GetDataTypeName(node->dataType));
    return;
}

static void ShowNodSelectStmt(SelectStmt* node, int level)
{
    ShowBlank(level+1);

    ShowNode(node->targetNode, level+1);
    ShowNode(node->fromList, level+1);
    ShowNode(node->whereList, level+1);
    return;
}

static void ShowNodTargetNode(TargetNode* node, int level)
{
    ShowBlank(level+1);

    printf("targetOption:%0x \n", node->targetOption);
    ShowNode(node->targetList, level+1);
    return;
}

static void ShowNodTargetElement(TargetElement* node, int level)
{
    ShowBlank(level+1);

    printf("alias:%s \n", node->alias);
    printf("all:%d \n", node->all);
    ShowNode(node->val, level+1);
    return;
}

static void ShowNodExpr(Expr* node, int level)
{
    ShowBlank(level+1);

    printf("exprOp:%d \n", node->exprOp);
    ShowNode(node->lexpr, level+1);
    ShowNode(node->rexpr, level+1);
    return;
}

static void ShowNodColumnName(ColumnName* node, int level)
{
    ShowBlank(level+1);

    printf("parent_name:%s \n", node->parent_name);
    printf("sub_name:%s \n", node->sub_name);
    return;
}

static void ShowNodTableRefInfo(TableRefInfo* node, int level)
{
    ShowBlank(level+1);

    printf("alias:%s \n", node->alias);
    ShowNode(node->val, level+1);
    return;
}

static void ShowNodJoinTblInfo(JoinTblInfo* node, int level)
{
    ShowBlank(level+1);

    printf("joinType:%d \n", node->joinType);
    ShowNode(node->leftTblInfo, level+1);
    ShowNode(node->rightTblInfo, level+1);
    ShowNode(node->joinExpr, level+1);
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
        case T_AttrName:
            ShowNodAttrName((AttrName*)n, level);
        break;
        case T_CreateStmt:
            ShowNodCreateStmt((CreateStmt*)n, level);
        break;
        case T_ColumnDef:
            ShowNodColumnDef((ColumnDef*)n, level);
        break; 
        case T_ColumnName:
            ShowNodColumnName((ColumnName*)n, level);
        break;   
        case T_DropStmt:
            ShowNodDropStmt((DropStmt*)n, level);
        break; 
        case T_Expr:
            ShowNodExpr((Expr*)n, level);
        break; 
        case T_InsertStmt:
            ShowNodInsertStmt((InsertStmt*)n, level);
        break;
        case T_JoinTblInfo:
            ShowNodJoinTblInfo((JoinTblInfo*)n, level);
        break;
        case T_List:
            TravelListNode((List*)n, level);
        break;
        case T_SelectStmt:
            ShowNodSelectStmt((SelectStmt*)n, level);
        break;
        case T_TableRefInfo:
            ShowNodTableRefInfo((TableRefInfo*)n, level);
        break;
        case T_TargetNode:
            ShowNodTargetNode((TargetNode*)n, level);
        break;
        case T_TargetElement:
            ShowNodTargetElement((TargetElement*)n, level);
        break;
        case T_ValuesData:
            ShowNodValuesData((ValuesData*)n, level);
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