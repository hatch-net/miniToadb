#ifndef HAT_NODE_H
#define HAT_NODE_H

#include "dictionary.h"

typedef enum NodeType
{    
	T_AttrName,
    T_CreateStmt,
	T_ColumnDef,
	T_ColumnName,
	T_DropStmt,
	T_Expr,
	T_InsertStmt,
	T_JoinTblInfo,
	T_List,	    
	T_NestLoopNode,
	T_ProjectNode,
	T_QueryStmt,
	T_ResultNode,
	T_ScanNode,
	T_SelectNode,
	T_SelectStmt,
	T_TableRefInfo,
	T_TargetNode,
	T_TargetElement,
	T_ValuesData,
    T_END
}NodeType;

#pragma pack(push,1)

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

typedef struct InsertStmt
{
	NodeType type;
	char *tableName;
	Node *attrNameList;
	Node *valuesList;
}InsertStmt;

typedef struct AttrName
{
	NodeType type;
	char *attrName;
}AttrName;

typedef struct ValuesData
{
	NodeType	type;
	Data		dataVal;
	DataType    dataType;	
} ValuesData;

typedef struct SelectStmt
{
	NodeType type;
	Node *targetNode;
	Node *fromList;
	Node *whereList;
}SelectStmt;

typedef struct TargetNode
{
	NodeType type;
	int targetOption;
	Node *targetList;
}TargetNode;

typedef enum TargetPostion
{
	OTHER_TARGET,
	TOP_TARGET
}TargetPostion;

typedef struct TargetElement
{
	NodeType 			type;
	char 				*alias;
	int 				all;
	int 				isTop;
	Node 				*val;
	Node		 		*tblRefInfo;
	AttrInfo			*attr;
}TargetElement;


typedef enum Expr_Op
{
	EXPR_OP_POSITIVE,		/* + */
	EXPR_OP_NEGATIVE,		/* - */
	EXPR_OP_PLUS,			/* + */
	EXPR_OP_MINUS,			/* - */
	EXPR_OP_MULTIPLE,		/* * */
	EXPR_OP_DIVISIION,		/* / */
	EXPR_OP_MOD, 			/* % */

	/* logical operation start */
	EXPR_OP_LESS,			/* < */
	EXPR_OP_LESS_EQUAL,		/* <= */
	EXPR_OP_EQUAL,			/* = */
	EXPR_OP_GREATER_EQUAL,		/* >= */
	EXPR_OP_GREATER,		/* > */
	EXPR_OP_NOT_EQUAL,			/* <> */
	EXPR_OP_AND,		
	EXPR_OP_OR,		
	EXPR_OP_NOT,		
	EXPR_OP_NUM 			
}Expr_Op;

typedef struct Expr
{
	NodeType	type;
	Expr_Op 	exprOp;	
	Node	   *lexpr;			
	Node	   *rexpr;			
	Node 	   *srcData;
} Expr;

typedef struct ColumnName
{
	NodeType	type;
	char	   *parent_name;
	char	   *sub_name;	
} ColumnName;

typedef enum RelationType
{
	RT_SIMPLE,
	RT_JOIN,
	RT_SUBQUERY
}RelationType;

typedef struct TableRefInfo
{
	NodeType		type;
	RelationType 	rtype;
	char	   		*alias;
	char			*tblName;
	Node			*val;
	Node			*targetList;	
	Node			*expr;
	int 			isPlan;
} TableRefInfo;

typedef enum JoinType
{
	JOIN_INNER	= 0x01,					/* matching tuple pairs only */			
	JOIN_LEFT	= 0x02,					/* pairs + unmatched LHS tuples */
	JOIN_CROSS	= 0x04,					/* pairs + unmatched LHS + unmatched RHS */
	JOIN_RIGHT	= 0x08,					/* pairs + unmatched RHS tuples */
	JOIN_NATURAL= 0x10,				
	JOIN_INVALID
}JoinType;

typedef struct JoinTblInfo
{
	NodeType	type;
	JoinType 	joinType;
	Node 		*leftTblInfo;
	Node 		*rightTblInfo;
	Node 		*joinExpr;
	int 		isPlan;
}JoinTblInfo;

typedef struct QueryStmt
{
	NodeType	type;
	Node 		*parserTree;
	Node		*relationList;
	Node 		*joinList;
	Node		*topTargetList;
	Node 		*expr;
	Node 		*plan;
}QueryStmt;

typedef struct ExecStateInfo ExecStateInfo;
typedef struct PlanNode
{
	NodeType	  type;
	ExecStateInfo *execInfo;
}PlanNode;

typedef struct ProjectNode
{
	PlanNode	  planInfo;
	Node 		  *targetList;
	Node		  *subNode;
	int 		  isOnlyOnce;
}ProjectNode;

typedef struct SelectNode
{
	PlanNode	  	planInfo;
	Node			*expr;
	Node			*subNode;
	Node			*resultList;	
}SelectNode;

typedef struct NestLoopNode
{
	PlanNode	  	planInfo;
	JoinType 		joinType;
	Node 			*parserNode;
	Node 			*leftplan;
	Node 			*rightplan;
	Node			*joinExpr;
	Node			*lresult;
	Node			*rresult;
}NestLoopNode;

typedef struct ScanInfo ScanInfo;
typedef struct ScanNode
{
	PlanNode	planInfo;
	Node		*tblRefInfoNode;
	ScanInfo 	*scanInfo;
}ScanNode;

typedef struct TupleHeader TupleHeader;
typedef struct ResultNode
{
	PlanNode		planInfo;
	TupleHeader 	*tup;
	RelationInfo 	*rel;
	Node			*tblRefInfo;
}ResultNode;

#pragma pack(pop)

#define NewNode(type) (type *)CreateNode(sizeof(type),T_##type)
#define AppendNewList(n)    (Node*)AppendNode(NULL, n)
#define AppendList(l, n)    (Node*)AppendNode((List*)l, n)

Node* CreateNode(int size, NodeType type);
List* AppendNode(List* list, Node* n);
void travelParserTree(Node* root);

ValuesData* GetValueByName(char *attrName, List *attrList, List *valueList);

#endif 