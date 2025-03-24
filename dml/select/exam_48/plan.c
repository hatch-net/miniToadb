#include "plan.h"
#include "executor.h"

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

    //ShowLogicalPlan(queryStmt);
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

    BaseRelationCollect(queryStmt, queryStmt->parserTree);
    retCode = TargetListCollect(queryStmt);

    return retCode;
}

int TargetListCollect(QueryStmt *queryStmt)
{
    SelectStmt *selectStmt = (SelectStmt*)queryStmt->parserTree;
    TargetNode *targetClause = (TargetNode*)selectStmt->targetNode;
    List *targetList = (List*)targetClause->targetList;
    ListNode *lnode = NULL;
    TargetElement *te = NULL;
    int ret = 0;

    if(queryStmt->relationList == NULL)
        return 0;

    for(lnode = targetList->head; lnode != NULL; lnode = lnode->next)
    {
        te = (TargetElement*)lnode->value;

        if(te->all)
        {
            ret = AddAllAtrributes(te, queryStmt);
        }
        else
        {
            ret = CheckAttributes(te, queryStmt);
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
    SelectNode  *selectNode = (SelectNode*)NewNode(SelectNode);
    ListNode *listNode = NULL;
    TableRefInfo *tblRefInfo = NULL;

    prjNode->targetList = queryStmt->topTargetList;
    prjNode->subNode = (Node*)selectNode;
    prjNode->planInfo.execInfo = &projectStateInfo;

    selectNode->expr = queryStmt->expr;
    selectNode->planInfo.execInfo = &selectStateInfo;
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