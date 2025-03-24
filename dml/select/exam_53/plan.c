#include "plan.h"
#include "executor.h"
#include "node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int LogicalPlan(QueryStmt *queryStmt);

int BaseRelationCollect(QueryStmt *queryStmt, Node *parserTree);

int TargetListCollect(QueryStmt *queryStmt);

int TableNodeProcess(QueryStmt *queryStmt, Node *node);

int TableRefNodeProcess(QueryStmt *queryStmt, TableRefInfo *tblRefInfo);

int JoinTblInfoNodeProcess(QueryStmt *queryStmt, JoinTblInfo *joinTblInfo);

int ExprSubTblProcess(QueryStmt *queryStmt, Node *node);

int AddAllAtrributes(TargetElement *te, QueryStmt *queryStmt);

int CheckAttributes(TargetElement *te, QueryStmt *queryStmt);

int ExprTargetProcess(Expr *exprNode, QueryStmt *queryStmt);

int TargetElementCheck(TargetElement *te, QueryStmt *queryStmt);

int WhereExprProcess(QueryStmt *queryStmt);

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

    retCode = BaseRelationCollect(queryStmt, queryStmt->parserTree);
    if(retCode <0)
        return retCode;

    retCode = TargetListCollect(queryStmt);
    if(retCode < 0)
    {
        return retCode;
    }

    retCode = WhereExprProcess(queryStmt);
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


int WhereExprProcess(QueryStmt *queryStmt)
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
    int ret = 0;

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
    else
    {
        printf("attribute %s is not found.\n", refNode->sub_name);
        ret = -1;
    }

    return ret;
}

int BaseRelationCollect(QueryStmt *queryStmt, Node *parserTree)
{
    List *relList = NULL;
    List *fromClause = NULL;
    List *whereCause = NULL;
    SelectStmt *selectStmt = (SelectStmt *)parserTree;
    ListNode *listNode = NULL;
    int ret = 0;

    /* from */
    if (selectStmt->fromList == NULL)
    {
        return 0;
    }

    fromClause = (List*)selectStmt->fromList;
    for (listNode = fromClause->head; listNode != NULL; listNode = listNode->next)
    {
        Node *node = (Node *)listNode->value;
        ret = TableNodeProcess(queryStmt, node);
        if(ret < 0)
        {
            return ret;
        }
    }

    /* where */
    if(selectStmt->whereList == NULL)
        return 0;

    ret = ExprSubTblProcess(queryStmt, selectStmt->whereList);
    return ret;
}

int TableNodeProcess(QueryStmt *queryStmt, Node *node)
{
    int ret = -1;

    switch (node->type)
    {
    case T_TableRefInfo:
        ret = TableRefNodeProcess(queryStmt, (TableRefInfo *)node);
        break;
    case T_JoinTblInfo:
        ret = JoinTblInfoNodeProcess(queryStmt, (JoinTblInfo *)node);
        break;
    default:
        break;
    }

    return ret;
}

int TableRefNodeProcess(QueryStmt *queryStmt, TableRefInfo *tblRefInfo)
{
    int ret = -1;

    if (tblRefInfo->rtype == RT_SUBQUERY)
    {
        ret = BaseRelationCollect(queryStmt, tblRefInfo->val);
        return ret;
    }

    tblRefInfo->tblName = ((ColumnName*)tblRefInfo->val)->sub_name;
    if(tblRefInfo->alias == NULL)
        tblRefInfo->alias = tblRefInfo->tblName;
    
    tblRefInfo->isPlan = 0;
    queryStmt->relationList = AppendList(queryStmt->relationList, (Node *)tblRefInfo);

    return 0;
}

List* JoinExprRefTbl(Node *joinNode)
{
    List *refTblList = NULL;

    switch(joinNode->type)
    {
        case T_JoinTblInfo:
        {
            List *lrefTblList = NULL, *rrefTblList = NULL;
            JoinTblInfo *joinTbl = (JoinTblInfo*)joinNode;

            if(joinTbl->leftTblInfo != NULL)
            {
                lrefTblList = JoinExprRefTbl(joinTbl->leftTblInfo);
            }

            if(joinTbl->rightTblInfo != NULL)
            {
                rrefTblList = JoinExprRefTbl(joinTbl->rightTblInfo);
            }
            refTblList = (List*)MergeListNode((Node*)lrefTblList, (Node*)rrefTblList);
        }
        break;

        case T_TableRefInfo:
            refTblList = (List*)AppendList(refTblList, joinNode);
        break;
    }

    return refTblList;
}

Node* AddJoinExpr(Node *joinExpr, List *targetList)
{
    Expr *ep = NewNode(Expr);
    ep->exprOp = EXPR_OP_EQUAL;
    ep->lexpr = targetList->head->value;
    ep->rexpr = targetList->tail->value;

    ; /* release list */

    if(joinExpr == NULL)
        joinExpr = (Node*)ep;
    else
    {
        Expr *andExpr = NewNode(Expr);
        andExpr->exprOp = EXPR_OP_AND;
        andExpr->lexpr = joinExpr;
        andExpr->rexpr = (Node*)ep;

        joinExpr = (Node*)andExpr;
    }

    return joinExpr;
}

List* GeneralUsingTargetList(ColumnName *colName, List *tblList)
{
    List *targetList = NULL;
    ListNode *relNode = NULL;
    TableRefInfo *tblRefInfo = NULL;
    TargetElement *te = NULL;
    int ret = 0;

    for(relNode = tblList->head; relNode != NULL; relNode = relNode->next)
    {
        tblRefInfo = (TableRefInfo *)relNode->value;

        te = NewNode(TargetElement);
        te->val = (Node*)colName;

        ret = CheckOneAttribute(te, colName->sub_name, tblRefInfo);
        if(ret > 0)  
        {
            if(colName->parent_name == NULL)
               colName->parent_name = tblRefInfo->tblName;            
            te->alias = colName->sub_name;  

            targetList = (List*)AppendList(targetList, (Node*)te);
        }
    }

    return targetList;
}

int GeneralJoinExpr(Node *joinNode)
{
    JoinTblInfo *joinTbl = (JoinTblInfo*)joinNode;
    List *attrList = (List*)joinTbl->joinExpr;
    List *refTblList = NULL;
    List *targetList = NULL;
    ListNode *lnode = NULL;
    ColumnName *colName = NULL;

    refTblList = JoinExprRefTbl(joinNode);
    if(refTblList == NULL)
    {   
        return -1;
    }

    joinTbl->joinExpr = NULL;
    for(lnode = attrList->head; lnode != NULL; lnode = lnode->next)
    {
        colName = (ColumnName*)lnode->value;

        targetList = GeneralUsingTargetList(colName, refTblList);
        if(targetList == NULL || targetList->length != 2)
        {
            ; /* release targetList/refTblList */
            printf("column %s is not a couple of join tables.\n", colName->sub_name);
            return -1;
        }

        joinTbl->joinExpr = AddJoinExpr(joinTbl->joinExpr, targetList);
    }

    ; /* release targetList/refTblList/attrList */
    return 0;
}

int JoinConditionProcess(QueryStmt *queryStmt, Node *joinNode)
{
    int ret = -1;
    JoinTblInfo *joinTbl = (JoinTblInfo*)joinNode;
    
    if(joinTbl == NULL)
    {
        return 0;
    }

    if(joinTbl->joinExpr != NULL)
    {
        switch(joinTbl->joinExpr->type)
        {
            case T_Expr:
                ret = ExprTargetProcess((Expr*)joinTbl->joinExpr, queryStmt);
            break;
            case T_List:
                ret = GeneralJoinExpr(joinNode);
            break;
            default:
                printf("join condition is invalid type.[%d]\n", joinTbl->joinExpr->type);
            break;
        }
    }
    else
    {
        ; //printf("not support nature join.\n");
    }

    return 0;
}

int JoinTblInfoNodeProcess(QueryStmt *queryStmt, JoinTblInfo *joinTblInfo)
{
    int ret = -1;
    Node *tmp = NULL;

    if (joinTblInfo->leftTblInfo != NULL)
    {
        ret = TableNodeProcess(queryStmt, joinTblInfo->leftTblInfo);
        if(ret < 0)
            return ret;
    }

    if(joinTblInfo->rightTblInfo != NULL)
    {
       ret = TableNodeProcess(queryStmt, joinTblInfo->rightTblInfo);
       if(ret < 0)
            return ret;
    }

    joinTblInfo->isPlan = 0;
    queryStmt->joinList = AppendList(queryStmt->joinList, (Node*)joinTblInfo);

    if(joinTblInfo->joinExpr != NULL || joinTblInfo->joinType & JOIN_NATURAL)
    {
        ret = JoinConditionProcess(queryStmt, (Node*)joinTblInfo);
    }

    /* right join -> left join ,after base table */
    if(joinTblInfo->joinType & JOIN_RIGHT)
    {
        joinTblInfo->joinType &= ~JOIN_RIGHT;
        joinTblInfo->joinType |= JOIN_LEFT;

        tmp = joinTblInfo->leftTblInfo;
        joinTblInfo->leftTblInfo = joinTblInfo->rightTblInfo;
        joinTblInfo->rightTblInfo = tmp;
    }

    return ret;
}

int ExprSubTblProcess(QueryStmt *queryStmt, Node *node)
{
    return 0;
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

    if((list != NULL) && (list->length > 1))
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
    nlNode->joinExpr = joinTbl->joinExpr;
    nlNode->lresult = NULL;
    nlNode->rresult = NULL;
    nlNode->needOuter = 1;

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
        nlNode->needOuter = 1;
        nlNode->joinExpr = NULL;

        subplan = (Node*)nlNode;
    }

    return subplan;
}

