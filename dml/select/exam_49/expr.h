#ifndef HAT_EXPR_H
#define HAT_EXPR_H

typedef struct Node Node;
int ExprBoolEvaluation(Node *exprNode);

typedef struct ValuesData ValuesData;
ValuesData* ExprEvaluation(Node *exprNode);

#endif 
