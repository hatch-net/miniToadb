#include "expr.h"
#include "node.h"
#include "dictionary.h"
#include "executor.h"

#include <stdio.h>


typedef ValuesData* (*Operator2)(ValuesData* leftvalue, ValuesData* rightvalue);

#define DATATYPEPROC_VERSION    (0x3A01)
typedef struct DataTypeProcs
{
    int         version;
    DataType    dType;
    Operator2   opf2[EXPR_OP_NUM];
}DataTypeProcs;


ValuesData* GetColumnData(Node *col, Node *resList);

int GetBoolValue(ValuesData *value);

ValuesData* ComputeExpr(Expr_Op opType, ValuesData *lres, ValuesData *rres);



int ExprBoolEvaluation(Node *exprNode)
{
    ValuesData *resValue = ExprEvaluation(exprNode);

    return GetBoolValue(resValue);
}

ValuesData* ExprEvaluation(Node *exprNode)
{
    int lret = 0, rret = 0, ret = 0;
    Expr *expr = (Expr*)exprNode;
    ValuesData *ldata = NULL, *rdata = NULL;
    ValuesData *resData = NULL;

    if(expr == NULL)
        return NULL;

    if(expr->lexpr == NULL)
    {
        ldata = NULL;
    }
    else if(expr->lexpr->type == T_Expr)
    {
        ((Expr*)(expr->lexpr))->srcData = expr->srcData;
        ldata = ExprEvaluation(expr->lexpr);
    }
    else 
    {
        ldata = GetColumnData(expr->lexpr, expr->srcData);
    }

    if(expr->rexpr == NULL)
    {
        rdata = NULL;
    }
    else if(expr->rexpr->type == T_Expr)
    {
        ((Expr*)(expr->rexpr))->srcData = expr->srcData;
        rdata = ExprEvaluation(expr->rexpr);
    }
    else 
    {
        rdata = GetColumnData(expr->rexpr, expr->srcData);
    }

    resData = ComputeExpr(expr->exprOp, ldata, rdata);
    
    return resData;
}

ValuesData* GetColumnData(Node *col, Node *resList)
{
    ValuesData *resValue = NULL;

    switch(col->type)
    {
        case T_ValuesData:
            resValue = (ValuesData*)col;
        break;
        case T_TargetElement:
            resValue = FetchValuesData((TargetElement*)col, (ResultNode*)resList);
        break;
        default:
        break;
    }
   
    return resValue;
}

int GetBoolValue(ValuesData *value)
{
    if(value->dataType != DT_BOOL)
    {
        printf("result value is not bool type.(%d)\n", value->dataType);
    }

    return value->dataVal.iData;
}

ValuesData* GetIntegerValueData(DataType dtype, int value)
{
    ValuesData *res = NULL;
    res = NewNode(ValuesData);
    res->dataType = dtype;
    res->dataVal.iData = value;
    return res;
}

ValuesData* int32GreaterOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;
    ValuesData *res = NULL;
    
    if(first > seconde)
        result = 1;

    res = GetIntegerValueData(DT_BOOL, result);
    return res;
}

ValuesData* int32GreaterEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;
    ValuesData *res = NULL;
    
    if(first >= seconde)
        result = 1;

    res = GetIntegerValueData(DT_BOOL, result);
    return res;
}

ValuesData* int32EqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;
    ValuesData *res = NULL;
    
    if(first == seconde)
        result = 1;

    res = GetIntegerValueData(DT_BOOL, result);
    return res;
}

ValuesData* int32LessEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;
    ValuesData *res = NULL;
    
    if(first <= seconde)
        result = 1;

    res = GetIntegerValueData(DT_BOOL, result);
    return res;
}


ValuesData* int32LessOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;
    ValuesData *res = NULL;
    
    if(first < seconde)
        result = 1;

    res = GetIntegerValueData(DT_BOOL, result);
    return res;
}

ValuesData* int32NotEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;
    ValuesData *res = NULL;
    
    if(first != seconde)
        result = 1;

    res = GetIntegerValueData(DT_BOOL, result);
    return res;
}

ValuesData* int32NegtiveOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    ValuesData *res = NULL;
    
    res = GetIntegerValueData(DT_INTEGER, -first);
    return res;
}

ValuesData* int32PlusOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int l = leftvalue->dataVal.iData;
    int r = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    if(rightvalue->dataType != DT_INTEGER)
    {
        printf("Operation is not support between different types(%d-%d).\n", leftvalue->dataType, rightvalue->dataType);
        return NULL;
    }

    res = GetIntegerValueData(leftvalue->dataType, l+r);
    return res;
}

ValuesData* int32MinusOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    res = GetIntegerValueData(DT_INTEGER, first - seconde);
    return res;
}

ValuesData* int32MultiOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    res = GetIntegerValueData(DT_INTEGER, first * seconde);
    return res;
}

ValuesData* int32DivisionOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    res = GetIntegerValueData(DT_INTEGER, first / seconde);
    return res;
}

ValuesData* int32ModOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    res = GetIntegerValueData(DT_INTEGER, first % seconde);
    return res;
}

DataTypeProcs integerDataTypeProcEntry =
{
    DATATYPEPROC_VERSION,
    DT_INTEGER,
    {
        NULL,
        int32NegtiveOperator,
        int32PlusOperator,
        int32MinusOperator,
        int32MultiOperator,
        int32DivisionOperator,
        int32ModOperator,
        int32LessOperator,
        int32LessEqualOperator,
        int32EqualOperator,
        int32GreaterEqualOperator,
        int32GreaterOperator,
        int32NotEqualOperator,
    }   
};

DataTypeProcs* dTypesProc[DT_END] =
{
    &integerDataTypeProcEntry,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

ValuesData* ComputeExpr(Expr_Op opType, ValuesData *lres, ValuesData *rres)
{
    ValuesData *result = NULL;

    if(lres == NULL && rres == NULL)
        return NULL;
    
    if(dTypesProc[lres->dataType] != NULL && dTypesProc[lres->dataType]->opf2[opType] != NULL)
    {
        result = dTypesProc[lres->dataType]->opf2[opType](lres, rres);
    }

    return result;
}