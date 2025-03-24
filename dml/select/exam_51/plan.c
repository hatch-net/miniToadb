#include "plan.h"
#include "executor.h"
#include "node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int LogicalPlan(QueryStmt *queryStmt);

void BaseRelationCollect(QueryStmt *queryStmt, Node *parserTree);

int TargetListCollect(QueryStmt *queryStmt);

void TableNodeProcess(QueryStmt *queryStmt, Node *node);

void TableRefNodeProcess(QueryStmt *queryStmt, TableRefInfo *tblRefInfo);

void JoinTblInfoNodeProcess(QueryStmt *queryStmt, JoinTblInfo *joinTblInfo);

void ExprTblProcess(QueryStmt *queryStmt, Node *node);

int AddAllAtrributes(TargetElement *te, QueryStmt *queryStmt);

int CheckAttributes(TargetElement *te, QueryStmt *queryStmt);

int ExprTargetProcess(Expr *exprNode, QueryStmt *queryStmt);

int TargetElementCheck(TargetElement *te, QueryStmt *queryStmt);

int ExprProcess(QueryStmt *queryStmt);

int PhysicalPlan(QueryStmt *queryStmt);

Node *JoinPlanProcess(QueryStmt *queryStmt);

Node* TablePlanNode(Node *node);

Node* CrossJoinPlan(Node *subplan, Node *node);

void ShowLogicalPlan(QueryStmt *queryStmt);


Node *PlanGenerator(Node *parse)
{
    int ret = 0;
    QueryStmt *queryStmt = NewNode(QueryStmt);
    queryStmt->parserTree = parse;

    if (parse == NULL)
    {
        return NULL;
    }

    switch (parse->type)
    {
    case T_SelectStmt:
        ret = LogicalPlan(queryStmt);
        if(ret < 0)
            return NULL;

        ret = PhysicalPlan(queryStmt);
        if(ret < 0)
            return NULL;
        break;
    default:
        return parse;
    }

    ShowLogicalPlan(queryStmt);
    return (Node *)queryStmt;
}

int LogicalPlan(QueryStmt *queryStmt)
{
    int retCode = 0;
    SelectStmt *selectStmt = (SelectStmt*)queryStmt->parserTree;
    
    if(selectStmt->targetNode == NULL)
        return -1;
    
    queryStmt->relationList = NULL;
    queryStmt->joinList = NULL;
    queryStmt->expr = NULL;

    BaseRelationCollect(queryStmt, queryStmt->parserTree);

    retCode = TargetListCollect(queryStmt);
    if(retCode < 0)
    {
        return retCode;
    }

    retCode = ExprProcess(queryStmt);

    return retCode;
}

Node* SubExprProcess(Node *subExprNode, QueryStmt *queryStmt)
{
    int ret = 0;
    TargetElement *te = NULL;
    Node *node = subExprNode;

    if(subExprNode->type == T_Expr)
    {
        ret = ExprTargetProcess((Expr*)subExprNode, queryStmt);
    }   
    else if(subExprNode->type == T_ColumnName)
    {
        te = NewNode(TargetElement);
        te->val = subExprNode;
        te->all = 0;
        te->tblRefInfo = NULL;
        te->attr = NULL;
        te->alias = NULL;
        te->isTop = OTHER_TARGET;        

        ret = TargetElementCheck(te, queryStmt);
        node = (Node*)te;
    }

    if(ret < 0)
    {
        if(te != NULL)
            free(te);
        return NULL;
    }

    return node;
}


int ExprProcess(QueryStmt *queryStmt)
{
    SelectStmt *selectStmt = (SelectStmt*)queryStmt->parserTree;
    Expr *whereClause = (Expr*)selectStmt->whereList;
    int ret = 0;

    if(whereClause == NULL)
        return 0;

    if(whereClause->exprOp < EXPR_OP_LESS)
    {
        printf("where clause is not bool expression.\n");
        return -1;
    }

    ret = ExprTargetProcess(whereClause, queryStmt);
    if(ret >= 0)
    {
        queryStmt->expr = (Node*)whereClause;
    }
    return ret;
}

int ExprTargetProcess(Expr *exprNode, QueryStmt *queryStmt)
{
    int ret = 0;

    do {
        if(exprNode->lexpr != NULL)
        {
            exprNode->lexpr = SubExprProcess(exprNode->lexpr, queryStmt);
        }
        if(ret < 0)
            break;

        if(exprNode->rexpr != NULL)
        {
            exprNode->rexpr = SubExprProcess(exprNode->rexpr, queryStmt);
        }
    }while(0);

    return ret;
}


AttrInfo* EstimateAttr(TargetElement *te)
{
    AttrInfo *attr = NULL;

    switch(te->val->type)
    {
        case T_ValuesData:

            attr = malloc(sizeof(AttrInfo));
            
            strcpy(attr->attrName, DEFAULT_COLUMN_HEAD);
            attr->maxWidth = strlen(attr->attrName) + 1;
            attr->attrType = ((ValuesData*)te->val)->dataType;
        break;

        case T_Expr:
        {
            Expr *ep = (Expr*)te->val;
            Node *lnode = NULL;

            /* expr tree, search leaf node */
            do {
                if(ep->lexpr != NULL)
                    lnode = ep->lexpr;
                else
                    lnode = ep->rexpr;

                if(lnode->type == T_TargetElement)
                {
                    attr = EstimateAttr((TargetElement*)lnode);
                    break;
                }
                else if(lnode->type == T_ValuesData)
                {
                    attr = malloc(sizeof(AttrInfo));

                    strcpy(attr->attrName, DEFAULT_COLUMN_HEAD);
                    attr->maxWidth = strlen(attr->attrName) + 1;
                    attr->attrType = ((ValuesData*)te->val)->dataType;
                    break;
                }
                else if(lnode->type == T_Expr)
                {
                    ep = (Expr*)lnode;
                }
            }while(lnode != NULL) ;
        }
        break;

        case T_ColumnName:
            attr = malloc(sizeof(AttrInfo));         
            strcpy(attr->attrName, DEFAULT_COLUMN_HEAD);
            attr->maxWidth = strlen(attr->attrName) + 1;

            attr->attrType = te->attr->attrType;
            attr->maxWidth = te->attr->maxWidth > attr->maxWidth ? te->attr->maxWidth : attr->maxWidth;
        default:
        break;
    }

    return attr;
}

int TargetElementCheck(TargetElement *te, QueryStmt *queryStmt)
{
    int ret = 0;

    switch(te->val->type)
    {
        case T_ColumnName:
            ret = CheckAttributes(te, queryStmt);
        break;

        case T_Expr:
            ret = ExprTargetProcess((Expr*)te->val, queryStmt);

        /* break; continue */
        case T_ValuesData:
            if(ret >= 0 && te->isTop == TOP_TARGET)
                queryStmt->topTargetList = AppendList(queryStmt->topTargetList, (Node*)te);

            te->attr = EstimateAttr(te);    
            if(te->attr != NULL)
                te->alias = te->attr->attrName;
        break;

        default:
        break;
    }

    return ret;
}

int TargetListCollect(QueryStmt *queryStmt)
{
    SelectStmt *selectStmt = (SelectStmt*)queryStmt->parserTree;
    TargetNode *targetClause = (TargetNode*)selectStmt->targetNode;
    List *targetList = (List*)targetClause->targetList;
    ListNode *lnode = NULL;
    TargetElement *te = NULL;
    int ret = 0;

    for(lnode = targetList->head; lnode != NULL; lnode = lnode->next)
    {
        te = (TargetElement*)lnode->value;

        if(te->all)
        {
            ret = AddAllAtrributes(te, queryStmt);
        }
        else
        {
            ret = TargetElementCheck(te, queryStmt);
        }

        if(ret < 0)
            break;
    }

    return ret;
}

TableRefInfo* SearchBaseRelationInfo(char *tblName, List *baseRelationList)
{
    ListNode *relNode = NULL;
    TableRefInfo *tblRefInfo = NULL;

    for(relNode = baseRelationList->head; relNode != NULL; relNode = relNode->next)
    {
        tblRefInfo = (TableRefInfo *)relNode->value;
        if(strcmp(tblRefInfo->alias, tblName) == 0)
        {
            return tblRefInfo;
        }
    }

    return NULL;
}

int AddOneRelationAttributes(TableRefInfo *tblRefInfo, QueryStmt *queryStmt)
{
    RelationInfo *relation = OpenRelation(tblRefInfo->tblName);
    TargetElement *te = NULL;
    ColumnName *columnName = NULL;
    int index = 0;

    if(relation == NULL)
    {
        return -1;
    }

    for(index = 0; index < relation->attrNum; index++)
    {
        columnName = NewNode(ColumnName);
        columnName->parent_name = relation->relName;
        columnName->sub_name = relation->attrs[index].attrName;

        te = NewNode(TargetElement);
        te->alias = columnName->sub_name;
        te->all = 0;
        te->val = (Node*)columnName;
        te->tblRefInfo = (Node*)tblRefInfo;
        te->attr = &(relation->attrs[index]);
        te->isTop = TOP_TARGET;

        queryStmt->topTargetList = AppendList(queryStmt->topTargetList, (Node*)te);
    }

    CloseRelation(relation);
}

int AddAllAtrributes(TargetElement *te, QueryStmt *queryStmt)
{
    ColumnName *refNode = (ColumnName*)te->val;
    TableRefInfo *tblRefInfo = NULL;
    ListNode *relNode = NULL;
    int ret = 0;

    if(refNode != NULL)
    {
        tblRefInfo = SearchBaseRelationInfo(refNode->parent_name, (List*)queryStmt->relationList);
        if(tblRefInfo == NULL)
        {
            printf("attribute %s reference relation %s is not found. \n", refNode->sub_name, refNode->parent_name);
            return -1;
        }

        ret = AddOneRelationAttributes(tblRefInfo, queryStmt);
        return ret;
    }

    for(relNode = ((List*)queryStmt->relationList)->head; relNode != NULL; relNode = relNode->next)
    {
        tblRefInfo = (TableRefInfo *)relNode->value;
        ret = AddOneRelationAttributes(tblRefInfo, queryStmt);
        if(ret < 0)
            break;
    }

    return ret;
}

int CheckOneAttribute(TargetElement *te, char *attrName, TableRefInfo *tblRefInfo)
{
    RelationInfo *relation = OpenRelation(tblRefInfo->tblName);
    int index = 0;
    int ret = -1;

    if(relation == NULL)
    {
        return -1;
    }

    for(index = 0; index < relation->attrNum; index++)
    {
        if(strcmp(attrName, relation->attrs[index].attrName) == 0)
        {
            ret = 1;
            te->tblRefInfo = (Node*)tblRefInfo;
            te->attr = &(relation->attrs[index]);

            break;
        }
    }

    CloseRelation(relation);

    return ret;
}

int CheckAttributes(TargetElement *te, QueryStmt *queryStmt)
{
    ColumnName *refNode = (ColumnName*)te->val;
    TableRefInfo *tblRefInfo = NULL;
    ListNode *relNode = NULL;
    int ret = 0;

    if(refNode->parent_name != NULL)
    {
        tblRefInfo = SearchBaseRelationInfo(refNode->parent_name, (List*)queryStmt->relationList);
        if(tblRefInfo == NULL)
        {
            printf("attribute %s reference relation %s is not found. \n", refNode->sub_name, refNode->parent_name);
            return -1;
        }

        ret = CheckOneAttribute(te, refNode->sub_name, tblRefInfo);        
    }
    else 
    {
        for(relNode = ((List*)queryStmt->relationList)->head; relNode != NULL; relNode = relNode->next)
        {
            tblRefInfo = (TableRefInfo *)relNode->value;
            ret += CheckOneAttribute(te, refNode->sub_name, tblRefInfo);
            if(ret > 1)
                break;
        }
    }
    
    if(ret == 1)
    {
        if(te->alias == NULL)
            te->alias = refNode->sub_name;
            
        refNode->parent_name = tblRefInfo->tblName;

        if(te->isTop == TOP_TARGET)
            queryStmt->topTargetList = AppendList(queryStmt->topTargetList, (Node*)te);
    }
    else if(ret > 1)
    {
        printf("attribute %s is ambiguous.\n", refNode->sub_name);
        ret = -1;
    }

    return ret;
}

void BaseRelationCollect(QueryStmt *queryStmt, Node *parserTree)
{
    List *relList = NULL;
    List *fromClause = NULL;
    List *whereCause = NULL;
    SelectStmt *selectStmt = (SelectStmt *)parserTree;
    ListNode *listNode = NULL;

    /* from */
    if (selectStmt->fromList == NULL)
    {
        return;
    }

    fromClause = (List*)selectStmt->fromList;
    for (listNode = fromClause->head; listNode != NULL; listNode = listNode->next)
    {
        Node *node = (Node *)listNode->value;
        TableNodeProcess(queryStmt, node);
    }

    /* where */
    if(selectStmt->whereList == NULL)
        return ;

    ExprTblProcess(queryStmt, selectStmt->whereList);
    return;
}

void TableNodeProcess(QueryStmt *queryStmt, Node *node)
{
    switch (node->type)
    {
    case T_TableRefInfo:
        TableRefNodeProcess(queryStmt, (TableRefInfo *)node);
        break;
    case T_JoinTblInfo:
        JoinTblInfoNodeProcess(queryStmt, (JoinTblInfo *)node);
        break;
    default:
        break;
    }
}

void TableRefNodeProcess(QueryStmt *queryStmt, TableRefInfo *tblRefInfo)
{
    if (tblRefInfo->rtype == RT_SUBQUERY)
    {
        BaseRelationCollect(queryStmt, tblRefInfo->val);
        return;
    }

    tblRefInfo->tblName = ((ColumnName*)tblRefInfo->val)->sub_name;
    if(tblRefInfo->alias == NULL)
        tblRefInfo->alias = tblRefInfo->tblName;
    
    tblRefInfo->isPlan = 0;
    queryStmt->relationList = AppendList(queryStmt->relationList, (Node *)tblRefInfo);
    return;
}

void JoinTblInfoNodeProcess(QueryStmt *queryStmt, JoinTblInfo *joinTblInfo)
{
    if (joinTblInfo->leftTblInfo != NULL)
    {
        TableNodeProcess(queryStmt, joinTblInfo->leftTblInfo);
    }

    if(joinTblInfo->rightTblInfo != NULL)
    {
        TableNodeProcess(queryStmt, joinTblInfo->rightTblInfo);
    }

    joinTblInfo->isPlan = 0;
    queryStmt->joinList = AppendList(queryStmt->joinList, (Node*)joinTblInfo);

    //joinTblInfo->joinExpr
    return;
}

void ExprTblProcess(QueryStmt *queryStmt, Node *node)
{

}


void ShowPlanNode(Node *plan)
{
    if(plan == NULL)
        return ;
   
    switch(plan->type)
    {
        case T_ProjectNode:
            printf("projectnode type:%d \n", plan->type);
            ShowPlanNode(((ProjectNode*)plan)->subNode);
        break;
        case T_SelectNode:
            printf("selectnode type:%d \n", plan->type);
            ShowPlanNode(((SelectNode*)plan)->subNode);
        break;
        case T_ScanNode:
            printf("scannode type:%d tablename:%s\n", plan->type, ((TableRefInfo*)((ScanNode*)plan)->tblRefInfoNode)->tblName);
        break;
        case T_NestLoopNode:
            printf("nestloop node type:%d {\n leftnode: \n", ((NestLoopNode*)plan)->joinType);
            ShowPlanNode(((NestLoopNode*)plan)->leftplan);
            printf("rightnode: \n");
            ShowPlanNode(((NestLoopNode*)plan)->rightplan);
            printf("} \n");
        break;
        default:
        break;
    }
}

void ShowLogicalPlan(QueryStmt *queryStmt)
{
    if(queryStmt->plan == NULL)
    {
        printf("plan is NULL\n");
        return ;
    }

    ShowPlanNode(queryStmt->plan);
}

int PhysicalPlan(QueryStmt *queryStmt)
{
    ProjectNode *prjNode = (ProjectNode*)NewNode(ProjectNode);
    SelectNode  *selectNode = NULL;
    ListNode *listNode = NULL;
    TableRefInfo *tblRefInfo = NULL;
    Node  *subNode = NULL;

    if(queryStmt->expr != NULL)
    {
        selectNode = (SelectNode*)NewNode(SelectNode);
        selectNode->expr = queryStmt->expr;
        selectNode->planInfo.execInfo = &selectStateInfo;
        selectNode->subNode = NULL;
        prjNode->subNode = (Node*)selectNode;
    }

    prjNode->isOnlyOnce = 0;
    prjNode->targetList = queryStmt->topTargetList;
    prjNode->planInfo.execInfo = &projectStateInfo;
    queryStmt->plan = (Node*)prjNode;

    if(queryStmt->joinList != NULL)
        subNode = JoinPlanProcess(queryStmt);

    if(queryStmt->relationList != NULL)
    { 
        for(listNode = ((List*)queryStmt->relationList)->head; listNode != NULL; listNode = listNode->next)
        {
            tblRefInfo = (TableRefInfo*)listNode->value;
            if(tblRefInfo->isPlan)
                continue;
            subNode = CrossJoinPlan(subNode, listNode->value);
        }
    }

PlanEnd:
    if(selectNode != NULL)
    {
        selectNode->subNode = subNode;
    }
    else 
    {
        prjNode->subNode = subNode;
    }

    return 0;
}

Node *JoinPlanProcess(QueryStmt *queryStmt)
{
    ListNode *listNode = NULL;
    Node *node = NULL;
    Node *subplan = NULL;
    List *list = NULL;

    for(listNode = ((List*)queryStmt->joinList)->head; listNode != NULL; listNode = listNode->next)
    {
        JoinTblInfo* joinTbl = (JoinTblInfo*)listNode->value;
        if(joinTbl->isPlan)
            continue;

        node = TablePlanNode((Node*)joinTbl);

        if(node != NULL)
            list = (List*)AppendList(list, node);
    }

    if(list->length > 1)
    {
        subplan = node;
        for(listNode = list->head; listNode != NULL; listNode = listNode->next)
        {
            if(listNode->value == node)
                continue;
            subplan = CrossJoinPlan(subplan, listNode->value);
        }
    }
    else 
    {
        if(list != NULL)
            subplan = list->head->value;
    }

    return subplan;
}

Node* JoinPlanNode(JoinTblInfo *joinTbl)
{
    NestLoopNode *nlNode = (NestLoopNode*)NewNode(NestLoopNode);

    nlNode->joinType = joinTbl->joinType;
    nlNode->parserNode = (Node*)joinTbl;
    nlNode->planInfo.execInfo = &nestloopStateInfo;
    nlNode->lresult = NULL;
    nlNode->rresult = NULL;

    joinTbl->isPlan = 1;

    if(joinTbl->leftTblInfo != NULL)
    {
        nlNode->leftplan = TablePlanNode(joinTbl->leftTblInfo);
    }

    if(joinTbl->rightTblInfo != NULL)
    {
        nlNode->rightplan = TablePlanNode(joinTbl->rightTblInfo);
    }

    return (Node*)nlNode;
}

Node* ScanPlanNode(TableRefInfo *tblRefInfoNode)
{
    ScanNode *scanNode = (ScanNode*)NewNode(ScanNode);
    scanNode->tblRefInfoNode = (Node*)tblRefInfoNode;
    scanNode->planInfo.execInfo = &seqScanStateInfo;

    tblRefInfoNode->isPlan = 1;
    
    return (Node*)scanNode;
}

Node* TablePlanNode(Node *node)
{
    Node *subplan = NULL;

    switch (node->type)
    {
    case T_TableRefInfo:
        subplan = ScanPlanNode((TableRefInfo*)node);
        break;
    case T_JoinTblInfo:
        subplan = JoinPlanNode((JoinTblInfo *)node);
        break;
    case T_NestLoopNode:
        subplan = node;
        break;
    default:
        break;
    }

    return subplan;
}

Node* CrossJoinPlan(Node *subplan, Node *node)
{
    NestLoopNode *nlNode = NULL;

    if(subplan == NULL)
    {
        subplan = TablePlanNode(node);
    }
    else 
    {
        nlNode = NewNode(NestLoopNode);
        nlNode->joinType = JOIN_CROSS;
        nlNode->parserNode = NULL;

        nlNode->leftplan = subplan;
        nlNode->rightplan = TablePlanNode(node);
        nlNode->planInfo.execInfo = &nestloopStateInfo;
        nlNode->lresult = NULL;
        nlNode->rresult = NULL;

        subplan = (Node*)nlNode;
    }

    return subplan;
}

