#include "executor.h"
#include "node.h"
#include "dictionary.h"
#include "hatfile.h"
#include "pages.h"
#include "seqscan.h"
#include "portal.h"
#include "expr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ExecCreateStmt(Node *rootNode);
int ExecDropStmt(Node *rootNode);
int ExecInsertStmt(Node *rootNode);
int ExecSelectStmt(Node *rootNode);

int ExecJunk(Node *expr, TupleHeader *tup);
Node* ExecNodeProc(Node *planNode);

void ReleaseResultNode(Node *result);


void Executor(Node *node)
{
    int retCode = 0;

    if(node == NULL)
    {
        // printf("error: execute plan is illegal.\n");
        return;
    }
    
    switch(node->type)
    {
        case T_CreateStmt:
            retCode = ExecCreateStmt(node);
            printf("excutor CreateStmt %s\n", retCode < 0 ? "failure":"success" );
        break;
        
        case T_DropStmt:
            retCode = ExecDropStmt(node);
            printf("excutor DropStmt %s\n", retCode < 0 ? "failure":"success" );
        break;

        case T_InsertStmt:
            retCode = ExecInsertStmt(node);
            printf("excutor InsertStmt %s\n", retCode < 0 ? "failure":"success" );
        break;

        case T_QueryStmt:
            retCode = ExecSelectStmt(node);
            printf("excutor return %d rows\n", retCode );
        default:
        break;
    }
    
    return ;
}

int ExecCreateStmt(Node *rootNode)
{
    int fd = 0;
    int ret = 0;
    CreateStmt *stmt = (CreateStmt*)rootNode;
    RelationInfo *relInfo = NULL;

    fd = CreateFile(stmt->tableName, 0600);
    if(fd < 0)
        return -1;
    CloseFile(fd);

    relInfo = InitRelationInfo(stmt);
    if(relInfo == NULL)
    {
        DeleteFile(stmt->tableName);
        return -1;
    }

    ret = InsertDictionary(relInfo, 0);
    return ret;
}

int ExecDropStmt(Node *rootNode)
{
    DropStmt *stmt = (DropStmt*)rootNode;
    RelationInfo *relInfo = NULL;
    int ret = 0;

    relInfo = DeleteRelInfoByName(stmt->tableName);
    if(relInfo == NULL)
    {
        return -1;
    }
    CloseRelation(relInfo);

    ret = DeleteFile(stmt->tableName);

    return ret;
}


int ExecInsertStmt(Node *rootNode)
{
    InsertStmt *stmt = (InsertStmt*)rootNode;
    RelationInfo *relInfo = NULL;

    List* valuesList = (List*)stmt->valuesList;
    List* targetsList = (List*)stmt->attrNameList;

    ItemData item = {0};
    PageDataHeader *page = NULL;
    TupleHeader *tupHeader = NULL;

    char buff[PAGE_MAX_SIZE] = {0};
    int ret = 0;

    relInfo = OpenRelation(stmt->tableName);
    if(relInfo == NULL)
    {
        return -1;
    }

    /* treveal values list */
    for(ListNode *lnode = valuesList->head; lnode != NULL; lnode = lnode->next)
    {
        List *values = (List *)lnode->value;

        tupHeader = FormTupleData(relInfo, targetsList, values, buff, PAGE_MAX_SIZE);
        if(tupHeader == NULL)
        {
            ret = -2;
            break;
        }

        /* form item */
        item.len = tupHeader->size;

        /* get spacepage */
        if(page == NULL)
        {
            page = GetFreeSpacePage(relInfo, tupHeader->size, PAGE_NEW);
        }
        else if(!HasFreeSpace(page, tupHeader->size))
        {
            if(relInfo->relstorage.isDirt)
            {
                WritePage(relInfo, page);
                relInfo->relstorage.isDirt = 0;
            }
            page = GetFreeSpacePage(relInfo, tupHeader->size, PAGE_NEW);
        }

        /* insert */
        PutTupleToPage(page, &item, tupHeader);      
        relInfo->relstorage.isDirt = 1;      
    }

    /* close relation */
    CloseRelation(relInfo);

    return ret;
}

int ExecSelectStmt(Node *rootNode)
{
    int ret = 0, rowNum = 0;
    int showHead = 1;
    Node *resultList = NULL;
    QueryStmt *queryPlan = (QueryStmt*)rootNode;

    do {        
        resultList = ExecNodeProc(queryPlan->plan);
        if(resultList == NULL)
            break;
        
        PortalShowResult(resultList, queryPlan->topTargetList, showHead);
        if(showHead)
            showHead = 0;

        rowNum += 1; 
    }while(resultList != NULL);

    return rowNum;
}

Node* ExecNodeProc(Node *planNode)
{
    Node* rowData = NULL;

    PlanNode *node = (PlanNode*)planNode;

    if((NULL != node) && (NULL != node->execInfo->execNodeProc))
    {        
        rowData = node->execInfo->execNodeProc(planNode);
    }

    return rowData;
}

Node* ExecNodeReProc(Node *planNode)
{
    Node* rowData = NULL;

    PlanNode *node = (PlanNode*)planNode;

    if((NULL != node) && (NULL != node->execInfo->execNodeReProc))
    {        
        rowData = node->execInfo->execNodeReProc(planNode);
    }

    return rowData;
}

Node* ExecSeqScan(Node *planNode)
{
    ScanNode* seqScan = (ScanNode*)planNode;
    TableRefInfo *tblRefInfo = (TableRefInfo *)seqScan->tblRefInfoNode;
    TupleHeader *tup = NULL;
    ResultNode *result = NULL;
    int isJunk = 0;

    if(seqScan->scanInfo == NULL)
    {
        seqScan->scanInfo = (ScanInfo*)malloc(sizeof(ScanInfo));

        seqScan->scanInfo->item_offset = PAGE_ITEM_START;
        seqScan->scanInfo->pageNum = PAGE_NUM_START;

        seqScan->scanInfo->rel = OpenRelation(tblRefInfo->tblName);
    }

    do {
        tup = SeqScanNext(seqScan->scanInfo);
        if(tup == NULL) /* end of table */
            break;

        isJunk = ExecJunk(tblRefInfo->expr, tup);
        if(isJunk > 0)
        {
            result = NewNode(ResultNode);
            result->tup = tup;
            result->rel = seqScan->scanInfo->rel;
            result->tblRefInfo = seqScan->tblRefInfoNode;
            
            tup = NULL;
            break;
        }

        if(tup != NULL)
            free(tup);
    }while(isJunk == 0);

    if(result == NULL)
    {
        /* close relation */
        CloseRelation(seqScan->scanInfo->rel);
        seqScan->scanInfo->rel = NULL;
    }

    return (Node*)result;
}

Node* ExecReSeqScan(Node *planNode)
{
    ScanNode* seqScan = (ScanNode*)planNode;
    if(seqScan->scanInfo != NULL)
    {
        CloseRelation(seqScan->scanInfo->rel);
        free(seqScan->scanInfo);
        seqScan->scanInfo = NULL;
    }

    return NULL;
}

int ExecJunkResult(Node *expr, Node *resultList)
{
    Expr *exprNode = (Expr*)expr;

    if(exprNode == NULL )
    {
        return 1;
    }

    exprNode->srcData = resultList;

    return ExprBoolEvaluation(expr);
}

int ExecJunk(Node *expr, TupleHeader *tup)
{
    int ret = 0;
    ResultNode *resNode = NewNode(ResultNode);

    resNode->rel = NULL;
    resNode->tup = tup;

    ret = ExecJunkResult(expr, (Node*)resNode);
    free(resNode);

    return ret;
}

void ReleaseResultNode(Node *result)
{

}

Node* ExecSelect(Node *planNode)
{
    SelectNode *selectPlanNode = (SelectNode *)planNode;
    Node *result = NULL;
    int isJunk = 0;

    do {
        if(result != NULL)
            ReleaseResultNode(result);

        result = ExecNodeProc(selectPlanNode->subNode);
        if(result == NULL)
            break;

        isJunk = ExecJunkResult(selectPlanNode->expr, result);
    }while(isJunk == 0);

    return result;
}

ResultNode* GetResultNodeFromList(TargetElement *te, List *resultList)
{
    ListNode *lnode = NULL;
    ResultNode *resNode = NULL;

    if(resultList->type == T_ResultNode)
    {
        return (ResultNode*)resultList;
    }

    for(lnode = resultList->head; lnode != NULL; lnode = lnode->next)
    {
        resNode = (ResultNode*)lnode->value;
        if(te->tblRefInfo == resNode->tblRefInfo)
        {
            return resNode;
        }
    }

    return NULL;
}

int FetchColumnData(char *buf,  TargetElement *te, ResultNode *resNode)
{
    ColumnName *columnName = (ColumnName*)te->val;
    ColumnDataHeader *column = NULL;
    int offset = sizeof(TupleHeader);
    int i, index = -1;

    if(resNode->rel == NULL)
    {
        column = (ColumnDataHeader *)buf;
        column->size = NULL_COLUMN_SIZE;
        return sizeof(ColumnDataHeader);
    }

    for(i = 0; i < resNode->rel->attrNum; i++)
    {
        if(strcmp(columnName->sub_name,resNode->rel->attrs[i].attrName) == 0)
        {
            index = i;
            break;
        }
    }

    if(index < 0)
    {
        printf("column (%s) data is not found.\n", te->attr->attrName);
        return -1;
    }

    for(i = 0; i < index; i++)
    {
        column = (ColumnDataHeader*)((char*)resNode->tup + offset);
        offset += column->size;

        if(offset > resNode->tup->size)
        {
            printf("column (%s) data is destroyed.\n", te->attr->attrName);
            return -1;
        }
    }

    column = (ColumnDataHeader*)((char*)resNode->tup + offset);
    memcpy(buf, (char*)column, column->size);

    return column->size;
}

ValuesData* FetchValuesData(Node *te, Node *resNodeList)
{
    TargetElement *col = (TargetElement*)te;
    ValuesData *resValue = NULL;
    ResultNode *resNode = NULL;
    char tempBuf[PAGE_MAX_SIZE] = {0};
    int colSize = 0;

    resNode = GetResultNodeFromList(col, (List*)resNodeList);
    if(resNode == NULL)
    {
        printf("result is not found about attr %s.\n", ((ColumnName*)col->val)->sub_name);
        return NULL;
    }

    colSize = FetchColumnData(tempBuf, col, resNode);
    if(colSize <= 0)
        return NULL;
    
    resValue = FormCol2ValueData(col->attr, (ColumnDataHeader*)tempBuf);
    return resValue;
}

Node* FormTargetResult(Node *targetList, Node *resultList)
{
    List *tList = (List*)targetList;
    ListNode *tNode = NULL;
    TargetElement *te = NULL;
    ResultNode *resNode = NULL;
    ValuesData *resValue = NULL;
    char tempBuf[PAGE_MAX_SIZE] = {0};
    int offset = sizeof(TupleHeader);

    if(tList == NULL)
        return NULL;
    
    for(tNode = tList->head; tNode != NULL; tNode = tNode->next)
    {
        te = (TargetElement*)tNode->value;
        
        switch(te->val->type)
        {
            case T_ColumnName:
                /* get result by table name */
                resNode = GetResultNodeFromList(te, (List*)resultList);
                if(resNode == NULL)
                {
                    printf("result is not found about attr %s.\n", ((ColumnName*)te->val)->sub_name);
                    offset = 0;
                    break;
                }

                /* get column by column name */
                offset += FetchColumnData(tempBuf + offset, te, resNode);
            break;

            case T_Expr:
                ((Expr*)te->val)->srcData = resultList;
                resValue = ExprEvaluation(te->val);

                offset += FormColumnData(te->attr, resValue, tempBuf + offset, PAGE_MAX_SIZE - offset);
            break;

            case T_ValuesData:
                offset += FormColumnData(te->attr, (ValuesData*)te->val, tempBuf + offset, PAGE_MAX_SIZE - offset);
            break;

            default:
            break;
        }
    }

    if(offset > 0)
    {
        resNode = NewNode(ResultNode);
        resNode->rel = NULL;

        resNode->tup = (TupleHeader*)malloc(offset);
        memcpy(resNode->tup, tempBuf, offset);        
        resNode->tup->size = offset;
    }

    return (Node*)resNode;
}

Node* ExecProject(Node *planNode)
{
    ProjectNode *prjPlanNode = (ProjectNode *)planNode;
    Node *result = NULL;

    if(prjPlanNode->isOnlyOnce)
    {
        return NULL;
    }

    if(prjPlanNode->subNode != NULL)
    {
        result = ExecNodeProc(prjPlanNode->subNode);
        if(result == NULL)
            return NULL;        
    }
    else
    {
        /* only select */
        prjPlanNode->isOnlyOnce = 1;
    }
    
    result = FormTargetResult(prjPlanNode->targetList, result);

    /* target option process distinct/all */

    return result;
}

int ExecJoinExpr(NestLoopNode *joinNode, Node *resultList)
{
    int ret = 0;

    switch(joinNode->joinType)
    {
        case JOIN_CROSS:
            ret = 1;
        break;
        default:
            ret = ExecJunkResult(joinNode->joinExpr, resultList);
        break;
    }

    return ret;
}


Node *GeneralNullResult(Node *tblRefInfo)
{
    TupleHeader *nullTup = (TupleHeader *)malloc(sizeof(TupleHeader));
    ResultNode *result = NewNode(ResultNode);

    result->tup = nullTup;
    result->rel = NULL;
    result->tblRefInfo = tblRefInfo;

    nullTup->size = NULL_TUPLE_SIZE;

    return (Node*)result;
}


Node* ExecGeneralNullResult(Node *node)
{
    Node *result = NULL;

    if(node == NULL)
        return NULL;

    switch(node->type)
    {
        case T_ScanNode:
            result = GeneralNullResult(((ScanNode*)node)->tblRefInfoNode);
        break;
        case T_NestLoopNode:
        {
            Node *lresult = ExecGeneralNullResult(((NestLoopNode*)node)->leftplan);
            Node *rresult = ExecGeneralNullResult(((NestLoopNode*)node)->rightplan);
            result = MergeListNode(lresult, rresult);
        }
        break;
        default:
        break;
    }

    return result;
}

/* inner/cross join */
Node* ExecNestLoop(Node *planNode)
{
    NestLoopNode *nlNode = (NestLoopNode*)planNode;
    Node *lresult = NULL, *rresult = NULL;
    List *resultList = NULL;
    int isJunk = 0;
   
    /* outer */
    for( ; ; )
    {
        if(nlNode->needOuter)
        {
            lresult = ExecNodeProc(nlNode->leftplan);
            if(lresult == NULL)
            {
                break;
            }
            
            nlNode->lresult = lresult;
            nlNode->needOuter = 0;
            nlNode->outerOnce = 0;

            ExecReSeqScan(nlNode->rightplan);
        }
        else 
            lresult = nlNode->lresult;

        /* inner */
        for( ; ; )
        {
            if (nlNode->rresult != NULL)
            {
                ; /* release rresult */
                nlNode->rresult = NULL;
            }            

            rresult = ExecNodeProc(nlNode->rightplan);
            if(rresult == NULL)
            { 
                if(nlNode->joinType & JOIN_LEFT)
                {
                    if(!nlNode->outerOnce)
                    {
                        resultList = (List*)MergeListNode(lresult, ExecGeneralNullResult(nlNode->rightplan));

                        nlNode->needOuter = 1;
                        return (Node*)resultList;
                    }
                }

                break;            
            }

            nlNode->rresult = rresult;
            
            resultList = (List*)MergeListNode(lresult, rresult);
            isJunk = ExecJoinExpr(nlNode, (Node*)resultList);
            if(isJunk)
            {
                nlNode->outerOnce = 1;
                return (Node*)resultList;
            }

            if(resultList != NULL)
            {
                ;/* release list */
                resultList = NULL;
            }
        }

        nlNode->needOuter = 1;

        if (nlNode->lresult != NULL)
        {
            ; /* release lresult */
            nlNode->lresult = NULL;
        }
    }

    return NULL;
}


ExecStateInfo seqScanStateInfo = {
    ExecSeqScan,
    ExecReSeqScan,
    NULL
};

ExecStateInfo projectStateInfo = {
    ExecProject,
    NULL,
    NULL
};

ExecStateInfo selectStateInfo = {
    ExecSelect,
    NULL,
    NULL
};

ExecStateInfo nestloopStateInfo = {
    ExecNestLoop,
    NULL,
    NULL
};