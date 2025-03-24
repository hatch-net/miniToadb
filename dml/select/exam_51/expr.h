#ifndef HAT_EXPR_H
#define HAT_EXPR_H

#define FLOAT_EPSILON (1e-6)

typedef struct Node Node;
int ExprBoolEvaluation(Node *exprNode);

typedef struct ValuesData ValuesData;
ValuesData* ExprEvaluation(Node *exprNode);

#endif 
