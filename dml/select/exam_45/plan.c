#include "plan.h"

#include <stdlib.h>
#include <stdio.h>

int LogicalPlan(QueryStmt *queryStmt);

void BaseRelationCollect(QueryStmt *queryStmt, Node *parserTree);

void TableNodeProcess(QueryStmt *queryStmt, Node *node);

void TableRefNodeProcess(QueryStmt *queryStmt, TableRefInfo *tblRefInfo);

void JoinTblInfoNodeProcess(QueryStmt *queryStmt, JoinTblInfo *joinTblInfo);

void ExprTblProcess(QueryStmt *queryStmt, Node *node);

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
    queryStmt->relationList = NULL;
    queryStmt->joinList = NULL;

    BaseRelationCollect(queryStmt, queryStmt->parserTree);

    return 0;
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
    SelectNode  *selectNode = (SelectNode*)NewNode(SelectNode);
    ListNode *listNode = NULL;
    TableRefInfo *tblRefInfo = NULL;

    prjNode->targetList = queryStmt->topTargetList;
    prjNode->subNode = (Node*)selectNode;

    selectNode->expr = queryStmt->expr;
    selectNode->subNode = NULL;

    queryStmt->plan = (Node*)prjNode;

    if(queryStmt->joinList != NULL)
        selectNode->subNode = JoinPlanProcess(queryStmt);

    if(queryStmt->relationList == NULL)
    {
        return  1;
    }    
    
    if((selectNode->subNode == NULL) && ((List*)queryStmt->relationList)->length == 1)
    {
        selectNode->subNode = TablePlanNode(((List*)queryStmt->relationList)->head->value);
        return 0;
    }

    for(listNode = ((List*)queryStmt->relationList)->head; listNode != NULL; listNode = listNode->next)
    {
        tblRefInfo = (TableRefInfo*)listNode->value;
        if(tblRefInfo->isPlan)
            continue;
        selectNode->subNode = CrossJoinPlan(selectNode->subNode, listNode->value);
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
    NestLoopNode *nlNode = (NestLoopNode*)NewNode(NestLoopNode);

    nlNode->joinType = JOIN_CROSS;
    nlNode->parserNode = NULL;

    nlNode->leftplan = subplan;
    nlNode->rightplan = TablePlanNode(node);

    return (Node*)nlNode;
}