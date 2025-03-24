#ifndef HAT_EXECUTOR_H
#define HAT_EXECUTOR_H

#define FILE_PATH_MAX_LEN       1024

typedef struct Node Node;
typedef Node* (*execNodeProcFunc)(Node *planNode);

typedef struct ExecStateInfo
{
    execNodeProcFunc execNodeProc;
    execNodeProcFunc execNodeReProc;
    execNodeProcFunc execNodeEndProc;
}ExecStateInfo;


extern ExecStateInfo seqScanStateInfo;
extern ExecStateInfo projectStateInfo;
extern ExecStateInfo selectStateInfo;
extern ExecStateInfo nestloopStateInfo;

void Executor(Node *node);

Node* ExecSeqScan(Node *planNode);

typedef struct ValuesData ValuesData;
typedef struct TargetElement TargetElement;
typedef struct ResultNode ResultNode;
ValuesData* FetchValuesData(TargetElement *te, ResultNode *resNode);

#endif 
