#ifndef HAT_NODE_H
#define HAT_NODE_H

typedef enum NodeType
{
    T_List,
    T_CreateStmt,
	T_DropStmt,
    T_ColumnDef,
    T_END
}NodeType;

typedef struct Node
{
    NodeType type;
}Node;

typedef struct ListNode
{
    Node* value;
    struct ListNode *next;
}ListNode;

/* tree list node */
typedef struct List
{
	NodeType type;
	int length;         /* number of ListNode struct */
	ListNode *head;
	ListNode *tail;
}List;

typedef struct CreateStmt
{
	NodeType type;
	char *tableName;
	Node *colList;
}CreateStmt;

typedef struct ColumnDef
{
	NodeType type;
	char *colName;
	char *colType;	
}ColumnDef;

typedef struct DropStmt
{
	NodeType type;
	char *tableName;
}DropStmt;

#define NewNode(type) (type *)CreateNode(sizeof(type),T_##type)
#define AppendNewList(n)    (Node*)AppendNode(NULL, n)
#define AppendList(l, n)    (Node*)AppendNode((List*)l, n)

Node* CreateNode(int size, NodeType type);
List* AppendNode(List* list, Node* n);
void travelParserTree(Node* root);

#endif 