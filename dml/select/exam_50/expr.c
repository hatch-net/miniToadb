#include "expr.h"
#include "node.h"
#include "dictionary.h"
#include "executor.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

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

ValuesData* GetSimpleValueData(DataType dtype, void *value)
{
    ValuesData *res = NULL;
    res = NewNode(ValuesData);
    res->dataType = dtype;
    switch(dtype)
    {
        case DT_INTEGER:
        case DT_BOOL:
            res->dataVal.iData = *((int*)value);
        break;
        case DT_FLOAT:
            res->dataVal.fData = *((float*)value);
        break;
        default:
        break;
    }
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

    res = GetSimpleValueData(DT_BOOL, &result);
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

    res = GetSimpleValueData(DT_BOOL, &result);
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

    res = GetSimpleValueData(DT_BOOL, &result);
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

    res = GetSimpleValueData(DT_BOOL, &result);
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

    res = GetSimpleValueData(DT_BOOL, &result);
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

    res = GetSimpleValueData(DT_BOOL, &result);
    return res;
}

ValuesData* int32NegtiveOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    ValuesData *res = NULL;
    first = -first;

    res = GetSimpleValueData(DT_INTEGER, &first);
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

    l = l + r;
    res = GetSimpleValueData(leftvalue->dataType, &l);
    return res;
}

ValuesData* int32MinusOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    first = first - seconde;
    res = GetSimpleValueData(DT_INTEGER, &first);
    return res;
}

ValuesData* int32MultiOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    first = first * seconde;
    res = GetSimpleValueData(DT_INTEGER, &first);
    return res;
}

ValuesData* int32DivisionOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    if(seconde == 0)
    {
        printf("division by zero, failure to excutor.\n");
        return NULL;
    }

    first = first / seconde;
    res = GetSimpleValueData(DT_INTEGER, &first);
    return res;
}

ValuesData* int32ModOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    ValuesData *res = NULL;

    first = first % seconde;
    res = GetSimpleValueData(DT_INTEGER, &first);
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


ValuesData* floatNegtiveOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    ValuesData *res = NULL;
    
    first = -first;
    res = GetSimpleValueData(DT_FLOAT, &first);
    return res;
}

ValuesData* floatPlusOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float l = leftvalue->dataVal.fData;
    float r = rightvalue->dataVal.fData;
    ValuesData *res = NULL;

    if(rightvalue->dataType != DT_FLOAT)
    {
        printf("Operation is not support between different types(%d-%d).\n", leftvalue->dataType, rightvalue->dataType);
        return NULL;
    }

    l = l + r;
    res = GetSimpleValueData(leftvalue->dataType, &l);
    return res;
}

ValuesData* floatMinusOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    ValuesData *res = NULL;

    first = first - seconde;
    res = GetSimpleValueData(leftvalue->dataType, &first);
    return res;
}

ValuesData* floatMultiOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    ValuesData *res = NULL;

    first = first * seconde;
    res = GetSimpleValueData(leftvalue->dataType, &first);
    return res;
}

ValuesData* floatDivisionOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    ValuesData *res = NULL;

    if(fabs(seconde) < FLOAT_EPSILON)
    {
        printf("division by zero, failure to excutor.\n");
        return NULL;
    }

    first = first / seconde;
    res = GetSimpleValueData(leftvalue->dataType, &first);
    return res;
}

ValuesData* floatGreaterOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    int result = 0;

    if((first - seconde) > FLOAT_EPSILON)
        result =  1;
    return GetSimpleValueData(DT_BOOL, &result);
}


ValuesData* floatGreaterEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    int result = 0;

    first -= seconde;
    if((first > FLOAT_EPSILON) || fabs(first) < FLOAT_EPSILON)
        result =  1;
    return GetSimpleValueData(DT_BOOL, &result);
}


ValuesData* floatEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    int result = 0;

    first -= seconde;
    if(fabs(first) < FLOAT_EPSILON)
        result =  1;
    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* floatLessEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    int result = 0;

    first -= seconde;
    if((first < -FLOAT_EPSILON) || fabs(first) < FLOAT_EPSILON)
        result =  1;    
    return GetSimpleValueData(DT_BOOL, &result);
}


ValuesData* floatLessOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    float first = leftvalue->dataVal.fData;
    float seconde = rightvalue->dataVal.fData;
    int result = 0;

    first -= seconde;
    if(first < -FLOAT_EPSILON)
        result =  1;    
    return GetSimpleValueData(DT_BOOL, &result);
}

DataTypeProcs FloatDataTypeProcEntry =
{
    DATATYPEPROC_VERSION,
    DT_FLOAT,
    {
        NULL,
        floatNegtiveOperator,
        floatPlusOperator,
        floatMinusOperator,
        floatMultiOperator,
        floatDivisionOperator,
        NULL,
        floatLessOperator,
        floatLessEqualOperator,
        floatEqualOperator,
        floatGreaterEqualOperator,
        floatGreaterOperator,
        NULL,
    }   
};

ValuesData* charGreaterOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char first = leftvalue->dataVal.cData;
    char seconde = rightvalue->dataVal.cData;

    if(first > seconde)
        result = 1;

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* charGreaterEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char first = leftvalue->dataVal.cData;
    char seconde = rightvalue->dataVal.cData;

    if(first >= seconde)
        result = 1;

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* charEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char first = leftvalue->dataVal.cData;
    char seconde = rightvalue->dataVal.cData;

    if(first == seconde)
        result = 1;

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* charLessEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char first = leftvalue->dataVal.cData;
    char seconde = rightvalue->dataVal.cData;

    if(first <= seconde)
        result = 1;

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* charLessOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char first = leftvalue->dataVal.cData;
    char seconde = rightvalue->dataVal.cData;

    if(first < seconde)
        result = 1;

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* charNotEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char first = leftvalue->dataVal.cData;
    char seconde = rightvalue->dataVal.cData;

    if(first != seconde)
        result = 1;

    return GetSimpleValueData(DT_BOOL, &result);
}

DataTypeProcs CharDataTypeProcEntry =
{
    DATATYPEPROC_VERSION,
    DT_CHAR,
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        charLessOperator,
        charLessEqualOperator,
        charEqualOperator,
        charGreaterEqualOperator,
        charGreaterOperator,
        charNotEqualOperator,
    }   
};


ValuesData* boolEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;

    if(first == seconde)
        result = 1;
    return GetSimpleValueData(DT_BOOL, &result);
}


ValuesData* boolNotEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int first = leftvalue->dataVal.iData;
    int seconde = rightvalue->dataVal.iData;
    int result = 0;

    if(first != seconde)
        result = 1;
    return GetSimpleValueData(DT_BOOL, &result);
}

DataTypeProcs BoolDataTypeProcEntry =
{
    DATATYPEPROC_VERSION,
    DT_BOOL,
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        boolEqualOperator,
        NULL,
        NULL,
        boolNotEqualOperator,
    }   
};

ValuesData* stringGreaterOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char *str1 = leftvalue->dataVal.pData;
    char *str2 = rightvalue->dataVal.pData;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    do {
        if(len1 > len2)
        {
            result = 1;
            break;
        }
        else if(len1 < len2)
        {
            // false
            break;
        }

        if(strncmp(str1,str2, len1) > 0)
        {
            result = 1;
            break;
        }
    }while(0);

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* stringGreaterEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char *str1 = leftvalue->dataVal.pData;
    char *str2 = rightvalue->dataVal.pData;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    do {
        if(len1 > len2)
        {
            result = 1;
            break;
        }
        else if(len1 < len2)
        {
            // false
            break;
        }

        if(strncmp(str1,str2, len1) >= 0)
        {
            result = 1;
            break;
        }
    }while(0);

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* stringEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char *str1 = leftvalue->dataVal.pData;
    char *str2 = rightvalue->dataVal.pData;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    do {
        if(len1 > len2)
        {
            // false
            break;
        }
        else if(len1 < len2)
        {
            // false
            break;
        }

        if(strncmp(str1,str2, len1) == 0)
        {
            result = 1;
            break;
        }
    }while(0);

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* stringLessEqualOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char *str1 = leftvalue->dataVal.pData;
    char *str2 = rightvalue->dataVal.pData;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    do {
        if(len1 > len2)
        {
            // false
            break;
        }
        else if(len1 < len2)
        {
            result = 1;
            break;
        }

        if(strncmp(str1,str2, len1) <= 0)
        {
            result = 1;
            break;
        }
    }while(0);

    return GetSimpleValueData(DT_BOOL, &result);
}

ValuesData* stringLessOperator(ValuesData* leftvalue, ValuesData* rightvalue)
{
    int result = 0;
    char *str1 = leftvalue->dataVal.pData;
    char *str2 = rightvalue->dataVal.pData;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    do {
        if(len1 > len2)
        {
            break;
        }
        else if(len1 < len2)
        {
            result = 1;
            break;
        }

        if(strncmp(str1,str2, len1) < 0)
        {
            result = 1;
            break;
        }
    }while(0);

    return GetSimpleValueData(DT_BOOL, &result);
}


DataTypeProcs VarcharDataTypeProcEntry =
{
    DATATYPEPROC_VERSION,
    DT_VARCHAR,
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        stringLessOperator,
        stringLessEqualOperator,
        stringEqualOperator,
        stringGreaterEqualOperator,
        stringGreaterOperator,
        NULL,
    }   
};

DataTypeProcs TextDataTypeProcEntry =
{
    DATATYPEPROC_VERSION,
    DT_TEXT,
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        stringLessOperator,
        stringLessEqualOperator,
        stringEqualOperator,
        stringGreaterEqualOperator,
        stringGreaterOperator,
        NULL,
    }   
};

DataTypeProcs* dTypesProc[DT_END] =
{
    &integerDataTypeProcEntry,
    &FloatDataTypeProcEntry,
    &CharDataTypeProcEntry,
    &BoolDataTypeProcEntry,
    &VarcharDataTypeProcEntry,
    &TextDataTypeProcEntry
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