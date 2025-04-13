#include <string.h>
#include <math.h>
#include "stdarg.h"
#include "btree_mgr.h"
#include "storage_mgr.h"
#include "buffer_mgr.h"
#include <stdlib.h>
#include "unistd.h"

int counter_Value = 0;

bool rightNode = true;
bool leftNode = true;


NodeInformation *initializeTree(int capacity)
{
    int zero=0;
    int one=1;
    int sizeOfInt=sizeof(int);

    NodeInformation *BTreePlusData = (NodeInformation *)malloc(sizeof(NodeInformation));
    
    int capacity_tobe_assgned = capacity;
    BTreePlusData->completed = zero;
    BTreePlusData->capacity = capacity_tobe_assgned * one;
    BTreePlusData->mgmtInfo = (int *)malloc(sizeOfInt * capacity * one);
    return BTreePlusData;
}




BPlusTree *nodeCreate(int capacity, int checkForChildren, int noOfNodes)
{
    int zero=0;
    int one=1;
    int sizeOf_Int=sizeof(int);
    int sizeOfBPlusTree=sizeof(BPlusTree);
    int sizeOf_NodeInformation=sizeof(NodeInformation);
    

    
    BPlusTree *BTree_plus_data = (BPlusTree *)malloc(sizeOfBPlusTree);
    BTree_plus_data->numberOfNodes = noOfNodes;
    
    BTree_plus_data->nodeRight_bplusTree = NULL;
    BTree_plus_data->nodeLeftBplusTree = NULL;
    
    NodeInformation *nodes_list = (NodeInformation *)malloc(sizeOf_NodeInformation);
    nodes_list->capacity = capacity * one;
    BTree_plus_data->checkForCountOfChildren = checkForChildren;
    nodes_list->mgmtInfo = (int *)malloc(sizeOf_Int * capacity * one);
    BTree_plus_data->inhrt = NULL;
    nodes_list->completed = zero * one;
    BTree_plus_data->nodeFieldsInfo = nodes_list;
    if (!(checkForChildren))
    {
        NodeInformation *node_sub_tree = (NodeInformation *)malloc(sizeOf_NodeInformation * one);
        node_sub_tree->mgmtInfo = (int *)malloc(sizeOf_Int * (capacity + one * one));
        node_sub_tree->completed = zero * one;
        BTree_plus_data->arrayOfNode = (BPlusTree *)malloc(sizeOfBPlusTree * (capacity + one * one));
        node_sub_tree->capacity = capacity + one * one;
        BTree_plus_data->nodeBrnch = node_sub_tree;
        return BTree_plus_data;
    }
    BTree_plus_data->childLeft_nodeInfo = initializeTree(capacity * one);
    BTree_plus_data->childRightNodeInfo = initializeTree(capacity * one);
    return BTree_plus_data;
}






int search_in_the_tree(NodeInformation *nodeList, int count, int *ptr)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int two=2;
    int sizeOfInt=sizeof(int);

    int completed_value = nodeList->completed - one;
    int ind_values = zero;
    int indexxValue = zero;
    if (completed_value < zero)
    {
        (*ptr) = zero;
        return negOne;
    }
    while (true)
    {
        indexxValue = (ind_values + completed_value) / two;
        counter_Value = zero;
        if (nodeList->mgmtInfo[indexxValue] == count)
        {
            while (nodeList->mgmtInfo[indexxValue - one] == count && indexxValue)
            {
                indexxValue = indexxValue - one * one;
            }
            (*ptr) = indexxValue;
            return indexxValue;
        }
        if (completed_value <= ind_values)
        {
            if (nodeList->mgmtInfo[ind_values] < (count  * one))
            {
                ind_values = ind_values + one;
            }
                
            (*ptr) = ind_values;
            return negOne;
        }
        if (nodeList->mgmtInfo[indexxValue] > count)
        {
            completed_value = indexxValue - one;
            counter_Value=counter_Value - one;
        }
        else
        {
            ind_values = indexxValue + one  * one;
            counter_Value= counter_Value + one * one;
        }
    }
}




BPlusTree *searchNodeInBtree(BTreeHandle *bTreeHandle, int value)
{  
    int zero=0;
    int one=1;
    int negOne = -1;
    int sizeOfInt=sizeof(int);

    int search_counts;
    int counterValue = zero;
    
    BPlusTree *bVal__tree = bTreeHandle->BPlusTree_root;
    bool flagValue = bVal__tree != NULL;
    
    while (flagValue && !bVal__tree->checkForCountOfChildren)
    {
        bool searchingCheck = search_in_the_tree(bVal__tree->nodeFieldsInfo, value, &search_counts) >= zero;
        if (searchingCheck != NULL && !(searchingCheck == false))
        {
            search_counts = search_counts + one * one;
        }
        counterValue = search_counts * one;
        counterValue=counterValue * one;
        bVal__tree = bVal__tree->arrayOfNode[search_counts * one];
    }
    return bVal__tree;
}




int insertInTree(NodeInformation *nodeList, int value, int position)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    bool checkBoolean = position <= nodeList->completed && nodeList->completed < nodeList->capacity;
    bool check2Boolean = position == nodeList->completed;
    int completedNodeValue = nodeList->completed;
    if (checkBoolean)
    {
        if (!check2Boolean)
        {
            while (completedNodeValue > position)
            {
                nodeList->mgmtInfo[completedNodeValue * one] = nodeList->mgmtInfo[completedNodeValue - one * one];
                int value = completedNodeValue - one  * one;
                completedNodeValue = value * one;
            }
        }
        nodeList->mgmtInfo[position] = value;
        completedNodeValue = zero * one;
        nodeList->completed = nodeList->completed + one * one;
        counter_Value = zero * one;
        return position;
    }
    return negOne;
}




int insertCountAtIndex(NodeInformation *NodeInformation, int value)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int indValue = zero * one;
    int countValues = negOne * one;
    bool nodeInsertBoolean = true;
    if (NodeInformation->capacity > NodeInformation->completed)
    {
        if (nodeInsertBoolean)
        {
            indValue = search_in_the_tree(NodeInformation, value, &countValues);

        }
            
        if (nodeInsertBoolean)
        {
            countValues = insertInTree(NodeInformation, value, countValues);

        }
            
    }
    return countValues;
}




void getThechildRightOfTree(int nodeValue, BPlusTree *bPlusTree, BPlusTree *childRight)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    nodeValue = nodeValue + one * one;
    int rightValuues = (childRight->numberOfNodes) * one;
    insertInTree(bPlusTree->nodeBrnch, rightValuues, nodeValue);
    int completd_valuue = (bPlusTree->nodeFieldsInfo->completed) * one;
    while (nodeValue < completd_valuue)
    {
        int indexToFetch = completd_valuue - one * one;
        bPlusTree->arrayOfNode[completd_valuue * one] = bPlusTree->arrayOfNode[indexToFetch];
        int valuuue = completd_valuue * one;
        valuuue = valuuue - one * one;
        completd_valuue = valuuue * one;
    }
    bPlusTree->arrayOfNode[nodeValue * one] = childRight;
}




void deleteValueAtIndex(NodeInformation *nodeList, int value, int value2)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int updateValue = (nodeList->completed - value2) * one;
    int indexValue = value * one;
    nodeList->completed = updateValue;
    int delCompletedVal = nodeList->completed;
    while (delCompletedVal > indexValue)
    {
        nodeList->mgmtInfo[indexValue] = nodeList->mgmtInfo[indexValue + value2];
        indexValue=indexValue + one * one;
        if (delCompletedVal * one > indexValue * one)
        {
            break;
        }
    }
}




int updateNodeValues(BPlusTree *ancesstur, int nodeCount, BPlusTree *node, int value)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int val1_int = nodeCount * one;
    int val2_int = (node->nodeBrnch->completed - value) * one;
    ancesstur->nodeFieldsInfo->completed = val1_int * one;
    ancesstur->nodeBrnch->completed = val2_int * one;
    return ancesstur->nodeBrnch->completed;
}



RC modifyAncestor(BTreeHandle *tHandle, BPlusTree *rightBPlusTree, BPlusTree *left, int uniqueIdx)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    BPlusTree *bplusTR;
    int valuues;
    BPlusTree *treeNodeBplusTree = left->inhrt;
    int numberValue;
    bool inheritBooleanValue = trueValue;
    if (!(treeNodeBplusTree != NULL))
    {
        treeNodeBplusTree = nodeCreate(tHandle->capacityVal, zero * one, tHandle->next_Page_Info);
        int counter_Values = zero * one;
        insertInTree(treeNodeBplusTree->nodeBrnch, left->numberOfNodes, zero * one);
        treeNodeBplusTree->arrayOfNode[zero * one] = left;
        while (counter_Value < three * one)
        {
            int valueInt = counter_Value * one;
            valueInt = valueInt + one * one;
            if (counter_Value == zero * one)
            {
                tHandle->treeRootIndexValue = treeNodeBplusTree->numberOfNodes;
                tHandle->next_Page_Info = tHandle->next_Page_Info + one * one;
            }
            else if (counter_Value == one * one)
            {
                tHandle->depthOfTheTree= tHandle->depthOfTheTree + one * one;
                tHandle->num_Of_Children= tHandle->num_Of_Children + one;
                
            }
            else
            {
                tHandle->BPlusTree_root = treeNodeBplusTree;
            }
            counter_Value = valueInt;
        }
    }
    if (inheritBooleanValue)
    {
        rightBPlusTree->inhrt = treeNodeBplusTree;
        left->inhrt = treeNodeBplusTree;
    }
    int index_Value = uniqueIdx * one;
    int positionInTree = insertCountAtIndex(treeNodeBplusTree->nodeFieldsInfo, index_Value * one);
    if (positionInTree <= negOne * one)
    {
        int capacityValuuu = tHandle->capacityVal * one;
        BPlusTree *bTreeNode = nodeCreate(capacityValuuu + one * one, zero * one, negOne);
        int complt3 = treeNodeBplusTree->nodeFieldsInfo->completed;
        bTreeNode->nodeFieldsInfo->completed = complt3;
        int complete2_ = treeNodeBplusTree->nodeBrnch->completed;
        int completed;
        bTreeNode->nodeBrnch->completed = complete2_;
        int intSizes = sizeOfInt * one;
        completed = treeNodeBplusTree->nodeBrnch->completed;
        memcpy(bTreeNode->nodeBrnch->mgmtInfo, treeNodeBplusTree->nodeBrnch->mgmtInfo, intSizes * completed);
        int completedSizess = intSizes * completed * one;
        memcpy(bTreeNode->nodeFieldsInfo->mgmtInfo, treeNodeBplusTree->nodeFieldsInfo->mgmtInfo, completedSizess * one);
        int bTreeSizeSize = sizeof(BPlusTree *);
        memcpy(bTreeNode->arrayOfNode, treeNodeBplusTree->arrayOfNode, bTreeSizeSize * completed);
        positionInTree = insertCountAtIndex(bTreeNode->nodeFieldsInfo, uniqueIdx);
        positionInTree=positionInTree * one;
        insertInTree(bTreeNode->nodeBrnch, rightBPlusTree->numberOfNodes, positionInTree + one * one);
        int kk = zero * one;
        int pIndex = (treeNodeBplusTree->nodeBrnch->completed) * one;
        bool flag = trueValue;
        int leftCompleted;
        int rightCompleted;
        while ((positionInTree + one * one < pIndex || kk <= one * one) && flag)
        {
            if (positionInTree + one * one >= pIndex)
            {
                ++kk;
                if (kk != one)
                {
                    tHandle->num_Of_Children= tHandle->num_Of_Children + one * one;
                    tHandle->next_Page_Info= tHandle->next_Page_Info + one * one;
                    bool flagBoolean1 = trueValue;
                    int valueInt3 = treeNodeBplusTree->nodeFieldsInfo->completed / (two * one);
                    treeNodeBplusTree->nodeBrnch->completed = valueInt3 + one * one;
                    while (flagBoolean1)
                    {
                        treeNodeBplusTree->nodeFieldsInfo->completed =treeNodeBplusTree->nodeFieldsInfo->completed/ two * one;
                        numberValue = treeNodeBplusTree->nodeBrnch->completed * one;
                        flagBoolean1 = falseValue;
                    }
                }
                else
                {
                    bTreeNode->arrayOfNode[++positionInTree] = rightBPlusTree;
                    leftCompleted = bTreeNode->nodeFieldsInfo->completed / two * one;
                    rightCompleted = bTreeNode->nodeFieldsInfo->completed - leftCompleted * one;
                    bplusTR = nodeCreate(tHandle->capacityVal, zero * one, tHandle->next_Page_Info);
                }
            }
            else
            {
                bTreeNode->arrayOfNode[pIndex] = bTreeNode->arrayOfNode[pIndex - one * one];
                pIndex = pIndex - one * one;
            }
        }
        int sizeOf_BPlusTree = sizeof(BPlusTree *);
        
        valuues = updateNodeValues(bplusTR, rightCompleted, bTreeNode, numberValue);
        for (int pointerIndex= zero * one; pointerIndex < three * one; pointerIndex = pointerIndex + one * one)
        {
            int value11 = pointerIndex;
            if (value11 == two * one)
            {
                break;

            }
                //break;
            if (value11 == zero * one)
            {
                memmove(treeNodeBplusTree->arrayOfNode, bTreeNode->arrayOfNode, sizeOf_BPlusTree * numberValue);
                int value3 = sizeOfInt * leftCompleted;
                memmove(treeNodeBplusTree->nodeFieldsInfo->mgmtInfo, bTreeNode->nodeFieldsInfo->mgmtInfo, value3);
            }
            else
            {
                memmove(bplusTR->arrayOfNode, bTreeNode->arrayOfNode + numberValue, sizeOf_BPlusTree * valuues);
                int value4 = sizeOfInt * rightCompleted;
                memmove(bplusTR->nodeFieldsInfo->mgmtInfo, bTreeNode->nodeFieldsInfo->mgmtInfo + leftCompleted, value4);
            }
            if (value11 == zero * one)
            {
                memmove(treeNodeBplusTree->nodeBrnch->mgmtInfo, bTreeNode->nodeBrnch->mgmtInfo, sizeOfInt * numberValue);
            }
            else
            {
                memmove(bplusTR->nodeBrnch->mgmtInfo, bTreeNode->nodeBrnch->mgmtInfo + numberValue, sizeOfInt * valuues);
            }
        }
        int value_3 = bplusTR->nodeFieldsInfo->mgmtInfo[zero * one];
        uniqueIdx = value_3;
        deleteValueAtIndex(bplusTR->nodeFieldsInfo, zero * one, one * one);
        return modifyAncestor(tHandle, bplusTR, treeNodeBplusTree, uniqueIdx);
    }
    else
    {
        getThechildRightOfTree(positionInTree, treeNodeBplusTree, rightBPlusTree);
    }
}




RC initIndexManager(void *mgmtData)
{
    printf("Initializing the index manager !! \n");
    initStorageManager();
    return RC_OK;
}



RC shutdownIndexManager()
{
    printf("Shutting-down the index manager!! \n");
    return RC_OK;
}




BPlusTree *setPosition(BTreeHandle *btreeHandle, BPlusTree *childdd)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int sizeOf_BPlusTreeStar = sizeof(BPlusTree *);
    childdd = nodeCreate(btreeHandle->capacityVal, one * one, btreeHandle->next_Page_Info);
    int child_vvalue = childdd->numberOfNodes * one;
    btreeHandle->treeRootIndexValue = child_vvalue * one;
    int infoNextVal = btreeHandle->next_Page_Info + one * one;
    btreeHandle->next_Page_Info = infoNextVal;
    btreeHandle->BPlusTree_root = childdd;
    int depthValuee = btreeHandle->depthOfTheTree + one * one;
    btreeHandle->depthOfTheTree = depthValuee;
    int numOfChildrenss = btreeHandle->num_Of_Children + one * one;
    btreeHandle->num_Of_Children = numOfChildrenss;
    return childdd;
}




int indexModification(DataType *dataTypePtr, char *charValuePtr)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int sizeOfInt=sizeof(int);
    
    int index_values = zero;
    int intSize = sizeOfInt;
    memcpy(charValuePtr, dataTypePtr, sizeOfInt * one);
    charValuePtr = charValuePtr + intSize * one;
    return index_values;
}




RC createBtree(char *idxId, DataType keyType, int n)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    int sizeOfInt=sizeof(int);
    int sizeOf_char=sizeof(char);
    int sizeOf_SM_FileHandle=sizeof(SM_FileHandle);
    
    bool open_PageFile_Value = falseValue;
    SM_FileHandle *sm_FileHandlePtr = (SM_FileHandle *)malloc(sizeOf_SM_FileHandle * one);
    rightNode = trueValue;
    char *initialChar = (char *)calloc(PAGE_SIZE * one, sizeOf_char);
    open_PageFile_Value = trueValue;
    createPageFile(idxId);
    if (open_PageFile_Value)
    {
        openPageFile(idxId, sm_FileHandlePtr);

    }
        
    bool freeValue = falseValue;
    memmove(initialChar, &n, sizeOfInt * one);
    int indexModfnValue = indexModification(&keyType, initialChar + sizeOfInt * one);
    int nextPage = one * one;
    int height = zero * one;
    int records = zero * one;
    int totalCnt = zero * one;
    for (int counter = zero * one; counter < (four * one); counter++)
    {
        leftNode = trueValue;
        int values = counter;
        if (counter == zero && counter != one)
        {
            memcpy(initialChar + sizeOfInt * one, &indexModfnValue,  sizeOfInt );
            values = values + one * one;
            counter = values * one;
        }
        else if (counter == one)
        {
            values = values + one * one;
            counter = values * one;
            memcpy(initialChar + sizeOfInt * one + sizeOfInt * one, &totalCnt,  sizeOfInt * one );
        }
        else if (counter == two)
        {
            values = values + one * one;
            counter = values * one;
            memcpy(initialChar + sizeOfInt * one + sizeOfInt * one + sizeOfInt * one, &records, sizeOfInt  * one);
        }
        else if (counter == three && leftNode)
        {
            values = values + one * one;
            counter = values * one;
            memcpy(initialChar + sizeOfInt * one + sizeOfInt * one + sizeOfInt * one + sizeOfInt * one, &height, sizeOfInt * one );
            leftNode = falseValue;
        }
        else if (counter != one * one && counter != two * one && counter != three && counter != zero)
        {
            memcpy(initialChar + sizeOfInt * one + sizeOfInt  * one + sizeOfInt * one + sizeOfInt * one + sizeOfInt * one, &nextPage, sizeOfInt * one );
        }
    }
    writeBlock(zero * one, sm_FileHandlePtr, initialChar);
    freeValue = trueValue;
    closePageFile(sm_FileHandlePtr);
    if (freeValue)
    {
        free(initialChar);
        free(sm_FileHandlePtr);
    }
    return RC_OK;
}





void setFunction(char *ptr, BTreeHandle *tree)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    
    
    memmove(&tree->keyType, ptr + sizeOfInt * one, sizeOfInt  * one);
    bool setFunctionVal;
    memmove(&tree->next_Page_Info, ptr + sizeOfInt  * one + sizeOfInt * one + sizeOfInt * one + sizeOfInt * one + sizeOfInt  * one + sizeOfInt * one, sizeOfInt * one);
    memmove(&tree->num_Of_Children, ptr + sizeOfInt  * one + sizeOfInt  * one + sizeOfInt * one, sizeOfInt * one);
    setFunctionVal = trueValue;
    memmove(&tree->treeRootIndexValue, ptr + sizeOfInt * one + sizeOfInt * one, sizeOfInt * one);
    if (setFunctionVal == trueValue)
    {
        memmove(&tree->depthOfTheTree, ptr + sizeOfInt * one  + sizeOfInt  * one + sizeOfInt * one  + sizeOfInt * one  + sizeOfInt * one , sizeOfInt * one );
        setFunctionVal = falseValue;
        rightNode = trueValue;
    }
    memmove(&tree->capacityVal, ptr, sizeOfInt * one );
    int sizeToBeAssigned;
    sizeToBeAssigned = sizeOfInt  * one + sizeOfInt * one  + sizeOfInt * one  + sizeOfInt * one ;
    memmove(&tree->numOfFields, ptr + sizeToBeAssigned, sizeOfInt * one );
    
}





int modifyTreeKey(BPlusTree *bTreeNode, Value *index, RID rid)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    
    int tvalue_1 = rid.slot * one;
    int tvalue_2 = rid.page * one;
    int valint = index->v.intV  * one;
    NodeInformation *information = bTreeNode->nodeFieldsInfo;
    int responseValue = insertCountAtIndex(information, valint)  * one;
    bool insrtbool = falseValue;
    insertInTree(bTreeNode->childLeft_nodeInfo, tvalue_1, responseValue);
    insrtbool = trueValue;
    if (insrtbool)
    {
        insertInTree(bTreeNode->childRightNodeInfo, tvalue_2, responseValue);

    }
        
    return responseValue;
}





RC openBtree(BTreeHandle **tree, char *idxId)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    int ten=10;
    int sizeOfInt=sizeof(int);
    int sizeOf_char=sizeof(char);
    int sizeOf_SM_FileHandle=sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool=sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle=sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle=sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);

    bool initialBufferValue = false;
    int depthOf_B_PlusTree = zero * one;
    int tempValue = zero * one;
    BM_BufferPool *buffer_Pool_Pointer = (BM_BufferPool *)malloc( sizeOf_BM_BufferPool * one );
    initialBufferValue = true;
    if (initialBufferValue)
    {
        initBufferPool(buffer_Pool_Pointer, idxId, ten, RS_LRU, NULL);
    }
        
    int sizeValue = sizeOf_BM_PageHandle;
    BM_PageHandle *bm_pageHandlePtr = (BM_PageHandle *)malloc(sizeValue);
    initialBufferValue = false;
    BTreeHandle *bTreeHandlePointer = (BTreeHandle *)malloc( sizeOf_BTreeHandle * one);
    pinPage(buffer_Pool_Pointer, bm_pageHandlePtr, tempValue);
    leftNode = false;
    bool setFunction_val = false;
    bTreeHandlePointer->idxId = idxId;
    setFunction_val = true;
    bTreeHandlePointer->mgmtData = buffer_Pool_Pointer;
    if (setFunction_val)
    {
        setFunction(bm_pageHandlePtr->data, bTreeHandlePointer);

    }
        
    rightNode = true;
    bTreeHandlePointer->BPlusTree_root = NULL;
    
    depthOf_B_PlusTree = bTreeHandlePointer->depthOfTheTree;
    rightNode = true;
    int sizeVals = zero * one;
    sizeVals = sizeOf_BPlusTree * one * depthOf_B_PlusTree;
    if (setFunction_val)
    {
        *tree = bTreeHandlePointer;
    }
        
    leftNode = true;
    return RC_OK;
}





BPlusTree *getChildInfo(BTreeHandle *bTreeHandle, BPlusTree *btNode, int cap, int rightValue, int leftValue)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int sizeOf_BPlusTreess = sizeof(BPlusTree *);
    
    int tempint = zero * one;
    bTreeHandle->num_Of_Children = bTreeHandle->num_Of_Children + one * one;
    counter_Value = zero * one;
    bTreeHandle->next_Page_Info = bTreeHandle->next_Page_Info + one * one;
    BPlusTree *childBplusTree = nodeCreate(bTreeHandle->capacityVal, one * one, bTreeHandle->next_Page_Info);
    counter_Value=counter_Value + one * one;
    tempint = zero * one;
    childBplusTree->nodeFieldsInfo->completed = rightValue;
    counter_Value=counter_Value + one;
    childBplusTree->childRightNodeInfo->completed = rightValue;
    tempint = zero * one;
    childBplusTree->childLeft_nodeInfo->completed = rightValue;
    memmove(childBplusTree->childRightNodeInfo->mgmtInfo, btNode->childRightNodeInfo->mgmtInfo + leftValue, cap);
    memmove(childBplusTree->childLeft_nodeInfo->mgmtInfo, btNode->childLeft_nodeInfo->mgmtInfo + leftValue, cap);
    memmove(childBplusTree->nodeFieldsInfo->mgmtInfo, btNode->nodeFieldsInfo->mgmtInfo + leftValue, cap);
    if (tempint != zero * one)
    {
        tempint = zero * one;
    }
    return childBplusTree;
}




RC closeBtree(BTreeHandle *bTreeHandle)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);

    int temp = zero * one;
    shutdownBufferPool(bTreeHandle->mgmtData);
    if (bTreeHandle->BPlusTree_root != NULL && trueValue)
    {
        BPlusTree *childBPlusTreePtr = bTreeHandle->BPlusTree_root;
        while (!childBPlusTreePtr->checkForCountOfChildren && trueValue)
        {
            temp = zero;
            childBPlusTreePtr = childBPlusTreePtr->arrayOfNode[zero * one];
        }
        BPlusTree *ancestorBPlusTreePtr = childBPlusTreePtr->inhrt;
        if (trueValue)
        {
            temp = zero * one;
        }
        while (childBPlusTreePtr != NULL && !falseValue)
        {
            childBPlusTreePtr = childBPlusTreePtr->nodeRight_bplusTree;
            counter_Value=counter_Value + one * one;
        }
        childBPlusTreePtr = ancestorBPlusTreePtr;
        ancestorBPlusTreePtr = childBPlusTreePtr->inhrt;
    }
    if (trueValue)
    {
        temp = zero * one;
    }
    return RC_OK;
}




RC deleteBtree(char *indexId)
{

    int zero=0;
    int one=1;
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    bool booleanFlag = true;
    int tempData = zero * one;
    if (booleanFlag)
    {
        unlink(indexId);
    }
        
    booleanFlag = false;
    return RC_OK;
}




RC getNumNodes(BTreeHandle *bTreeHandle, int *res)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int tempValue = zero * one;
    if (trueValue)
    {
        while (trueValue)
        {
            *res = (bTreeHandle->num_Of_Children) * one;
            break;
        }
    }
    return RC_OK;
}





RC getNumEntries(BTreeHandle *BTreeHandle, int *res)
{
    int zero=0;
    int one=1;
    
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int tempValue = zero * one;
    if (tempValue == zero * one)
    {
        *res = (BTreeHandle->numOfFields) * one;
    }
    return RC_OK;
}




RC getKeyType(BTreeHandle *BTreeHandle, DataType *res)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int tempValue = zero;
    *res = tempValue == (zero * one) ? BTreeHandle->keyType : (zero * one);
    return RC_OK;
}




void obtainchildLeft(BPlusTree *bPlusTreeChildren, int intvaluu, BPlusTree *bplusTNode)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int sizeOf_BPlusTreess = sizeof(BPlusTree *);
    
    int valueToAssign = intvaluu * one;
    bPlusTreeChildren->nodeFieldsInfo->completed = valueToAssign * one;
    int sizeOfVal = sizeOfInt * intvaluu * one;
    bPlusTreeChildren->childRightNodeInfo->completed = intvaluu * one;
    bool flag = falseValue;
    bPlusTreeChildren->childLeft_nodeInfo->completed = valueToAssign;
    if (!flag)
    {
        memmove(bPlusTreeChildren->childLeft_nodeInfo->mgmtInfo, bplusTNode->childLeft_nodeInfo->mgmtInfo, sizeOfVal);

    }
        
    memmove(bPlusTreeChildren->childRightNodeInfo->mgmtInfo, bplusTNode->childRightNodeInfo->mgmtInfo, sizeOfVal);
    leftNode = trueValue;
    flag = trueValue;
    rightNode = trueValue;
    if (flag)
    {
        memmove(bPlusTreeChildren->nodeFieldsInfo->mgmtInfo, bplusTNode->nodeFieldsInfo->mgmtInfo, sizeOfVal);

    }
        
}




RC findKey(BTreeHandle *tree, Value *key, RID *result)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    bool tmpValue = trueValue;
    int intTemp = zero * one;
    BPlusTree *childTree = searchNodeInBtree(tree, key->v.intV);
    int countOfVal;
    int keySearchValue = search_in_the_tree(childTree->nodeFieldsInfo, key->v.intV, &countOfVal) * one;
    rightNode = falseValue;
    while (tmpValue && leftNode && !rightNode)
    {
        if (keySearchValue < zero  * one)
        {
            counter_Value=counter_Value - one;
            intTemp = intTemp - 1;
            return RC_IM_KEY_NOT_FOUND;
        }
        else if (childTree == NULL)
        {
            counter_Value = counter_Value + one;
            intTemp = intTemp + one;
            return RC_ERROR;
        }
        else
        {
            int temp2value = intTemp * one;
            tmpValue = trueValue;
            result->slot = childTree->childLeft_nodeInfo->mgmtInfo[keySearchValue * one];
            rightNode = trueValue;
            int searchValueIndex = keySearchValue * one;
            result->page = childTree->childRightNodeInfo->mgmtInfo[searchValueIndex];
            tmpValue = falseValue;
        }
        break;
    }
    return RC_OK;
}




char *printTree(BTreeHandle *tree)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int eleven=11;
    int fourteen=14;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int levelOfTree = zero * one;
    int value_1 = tree->num_Of_Children * tree->depthOfTheTree * eleven * one;
    int value_2 = tree->depthOfTheTree + fourteen * one;
    int value_3 = tree->num_Of_Children;
    int sizeOfB = value_1  * one + value_2 * one + value_3 * one;
    BPlusTree *bPlusTreeHandleNodePtr = tree->BPlusTree_root;
    while (bPlusTreeHandleNodePtr != NULL)
    {
        bool does_Children_Exists = bPlusTreeHandleNodePtr->checkForCountOfChildren;
        if (does_Children_Exists)
        {
            bPlusTreeHandleNodePtr = bPlusTreeHandleNodePtr->childRightNodeInfo;
            does_Children_Exists = falseValue;
        }
        else
        {
            if (!(bPlusTreeHandleNodePtr->childRightNodeInfo != NULL))
            {
                BPlusTree *tempBPlusTree = tree->BPlusTree_root;
                leftNode = trueValue;
                for (int counter = zero * one; counter <= levelOfTree; counter++)
                {
                    int valuess = counter * one;
                    tempBPlusTree = tempBPlusTree->childLeft_nodeInfo;
                }
                rightNode = falseValue;
                bPlusTreeHandleNodePtr = tempBPlusTree;
                int levelValue = levelOfTree * one;
                levelValue =levelValue + one * one;
                levelOfTree = levelValue * one;
            }
            else
            {
                bPlusTreeHandleNodePtr = bPlusTreeHandleNodePtr->childRightNodeInfo;
                does_Children_Exists = falseValue;
            }
        }
    }
    sizeOfB = zero * one;
    value_1 = zero * one;
    value_2 = zero * one;
    value_3 = zero * one;
    return RC_OK;
}



RC insertKey(BTreeHandle *tree, Value *key, RID rid)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    BPlusTree *childTree_BPlusTree = searchNodeInBtree(tree, key->v.intV);
    int valuesToBeInserted;
    for (int counterValuess = zero * one; counterValuess < two * one; counterValuess++)
    {
        if (counterValuess == zero * one)
        {
            leftNode = trueValue;
            if (NULL == childTree_BPlusTree || childTree_BPlusTree == "")
            {
                childTree_BPlusTree = setPosition(tree, childTree_BPlusTree);
            }
            rightNode = trueValue;
            int val2 = counterValuess;
            val2=val2 + one * one;
            val2 = counterValuess * one;
        }
        if (counterValuess == one * one)
        {
            counterValuess = counterValuess + one;
            int search_value = search_in_the_tree(childTree_BPlusTree->nodeFieldsInfo, key->v.intV, &valuesToBeInserted);
            if (search_value >=  zero * one)
            {
                return RC_IM_KEY_ALREADY_EXISTS;
            }
            int val_int = counterValuess * one;
            val_int = val_int + one * one;
            val_int = counterValuess * one;
        }
    }
    int indIndexVale = zero * one;
    indIndexVale = insertCountAtIndex(childTree_BPlusTree->nodeFieldsInfo, key->v.intV);
    if (indIndexVale >= zero * one)
    {
        int child_Right_Insert;
        child_Right_Insert = insertInTree(childTree_BPlusTree->childRightNodeInfo, rid.page, indIndexVale);
        child_Right_Insert = child_Right_Insert * one;
        int child_Left_Insertion;
        child_Left_Insertion = insertInTree(childTree_BPlusTree->childLeft_nodeInfo, rid.slot, indIndexVale);
        child_Left_Insertion = child_Left_Insertion * one;
    }
    else
    {
        int checkingChld = one * one;
        BPlusTree *nextBTree = nodeCreate(tree->capacityVal + one * one, checkingChld, negOne);
        int counter_valuee = zero * one;
        while (counter_valuee < three * one)
        {
            leftNode = falseValue;
            if (counter_valuee == zero * one)
            {
                int completeVal = childTree_BPlusTree->nodeFieldsInfo->completed;
                int sizeValuees = sizeOfInt * completeVal;
                memmove(nextBTree->nodeFieldsInfo->mgmtInfo, childTree_BPlusTree->nodeFieldsInfo->mgmtInfo, sizeValuees);
                rightNode = trueValue;
                nextBTree->nodeFieldsInfo->completed = completeVal;
            }
            else if (counter_valuee > one * one)
            {
                int completedValue2 = childTree_BPlusTree->nodeFieldsInfo->completed;
                completedValue2 + completedValue2 * one;
                int sizes = sizeOfInt * completedValue2 * one;
                memmove(nextBTree->childLeft_nodeInfo->mgmtInfo, childTree_BPlusTree->childLeft_nodeInfo->mgmtInfo, sizes * one);
                nextBTree->childLeft_nodeInfo->completed = completedValue2;
            }
            else
            {
                int completedValue3 = childTree_BPlusTree->childRightNodeInfo->completed * one;
                int size3 = sizeOfInt * completedValue3 * one;
                memmove(nextBTree->childRightNodeInfo->mgmtInfo, childTree_BPlusTree->childRightNodeInfo->mgmtInfo, size3 * one);
                nextBTree->childRightNodeInfo->completed = completedValue3;
            }
            counter_valuee = counter_valuee + one;
        }
        leftNode = trueValue;
        indIndexVale = modifyTreeKey(nextBTree, key, rid);
        rightNode = trueValue;
        int childVariable = nextBTree->nodeFieldsInfo->completed - ceil((float)nextBTree->nodeFieldsInfo->completed / (two * one));
        childVariable = childVariable * one;
        bool rightChildObtainVAlue = falseValue;
        rightChildObtainVAlue = !rightChildObtainVAlue;
        int sizeVal = ceil((float)nextBTree->nodeFieldsInfo->completed / (two * one));
        BPlusTree *bChildInfo = getChildInfo(tree, nextBTree, sizeOfInt * childVariable * one, childVariable * one, sizeVal);
        if(rightChildObtainVAlue)
        {
            obtainchildLeft(childTree_BPlusTree, ceil((float)nextBTree->nodeFieldsInfo->completed / (two * one)), nextBTree);

        }
        
        int counterVal2 = two * one;
        while (counterVal2 > zero * one)
        {
            if (counterVal2 < two * one)
            {
                childTree_BPlusTree->nodeRight_bplusTree = bChildInfo;
                bool tempBoolean = falseValue;
                bChildInfo->nodeLeftBplusTree = childTree_BPlusTree;
                tempBoolean = !tempBoolean;
                if(tempBoolean)
                {
                    modifyAncestor(tree, bChildInfo, childTree_BPlusTree, bChildInfo->nodeFieldsInfo->mgmtInfo[zero * one]);

                }
                
            }
            else
            {
                bChildInfo->nodeRight_bplusTree = childTree_BPlusTree->nodeRight_bplusTree;
                bool tempBoolean2 = falseValue;
                if (bChildInfo->nodeRight_bplusTree != NULL)
                {
                    tempBoolean2 = !tempBoolean2;

                }
                
                if(tempBoolean2)
                {
                    bChildInfo->nodeRight_bplusTree->nodeLeftBplusTree = bChildInfo;

                }
                
            }
            int valTemp3;
            valTemp3 = zero * one;
            valTemp3 = counterVal2 * one;
            valTemp3 = valTemp3  - one * one;
            counterVal2 = valTemp3 * one;
        }
    }
    tree->numOfFields = tree->numOfFields +  one*one;
    return RC_OK;
}




void changeValues(RID *outputrid, TreeInfo *childtreeInfo)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int sizeOf_BPlusTreess = sizeof(BPlusTree *);
    bool changeValueValuebool = false;
    changeValueValuebool = trueValue;
    if (changeValueValuebool)
    {
        int index_Valuuu = childtreeInfo->index_value * one;
        outputrid->page = childtreeInfo->currentBPlusTree->childRightNodeInfo->mgmtInfo[index_Valuuu * one];
        int slottss_Value = childtreeInfo->currentBPlusTree->childLeft_nodeInfo->mgmtInfo[childtreeInfo->index_value] * one;
        outputrid->slot = slottss_Value * one;
    }
    int indexVal = childtreeInfo->index_value * one;
    int valIndex = indexVal + one  * one;
    childtreeInfo->index_value = valIndex * one;
}




RC deleteKey(BTreeHandle *tree, Value *key)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    bool deleteKeyValue = falseValue;
    BPlusTree *child_BPlusTree = searchNodeInBtree(tree, key->v.intV);
    leftNode = trueValue;
    deleteKeyValue = !deleteKeyValue;
    if (child_BPlusTree != NULL && deleteKeyValue)
    {
        int searchTreeAnces = zero * one;
        int value2;
        leftNode = trueValue;
        searchTreeAnces = search_in_the_tree(child_BPlusTree->nodeFieldsInfo, key->v.intV, &value2);
        rightNode = trueValue;
        int searchedIndexValue = search_in_the_tree(child_BPlusTree->nodeFieldsInfo, key->v.intV, &value2);
        if (searchedIndexValue < zero  * one)
        {
            searchedIndexValue = searchedIndexValue * one;
            return RC_IM_KEY_NOT_FOUND;
        }
        deleteValueAtIndex(child_BPlusTree->childLeft_nodeInfo, searchTreeAnces, one  * one);
        int searchingValue = search_in_the_tree(child_BPlusTree->nodeFieldsInfo, key->v.intV, &value2);
        searchingValue = searchingValue * one;
        deleteValueAtIndex(child_BPlusTree->nodeFieldsInfo, searchingValue, one  * one);
        int noOfFields;
        deleteValueAtIndex(child_BPlusTree->childRightNodeInfo, searchTreeAnces, one  * one);
        noOfFields = tree->numOfFields - one * one;
        tree->numOfFields = noOfFields;
    }
    else
    {
        return RC_ERROR;
    }
    return RC_OK;
}




RC openTreeScan(BTreeHandle *tree, BT_ScanHandle **handle)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    TreeInfo *treeInfo = (TreeInfo *)malloc(sizeof(TreeInfo));
    int sizeVal = zero * one;
    sizeVal = sizeOf_BT_ScanHandle;
    leftNode = falseValue;
    BT_ScanHandle *bt_scanHandleValue = (BT_ScanHandle *)malloc(sizeVal);
    bt_scanHandleValue->tree_BTreeHandle = tree;
    if (NULL != bt_scanHandleValue->tree_BTreeHandle)
    {
        treeInfo->currentBPlusTree = tree->BPlusTree_root;
        rightNode = falseValue;
        int counterV = zero  * one;
        bool flagValueBool = trueValue;
        while (flagValueBool)
        {
            if (!treeInfo->currentBPlusTree->checkForCountOfChildren || counterV < one  * one)
            {
                rightNode = trueValue;
                int checkValuu = !treeInfo->currentBPlusTree->checkForCountOfChildren;
                if (checkValuu)
                {
                    treeInfo->currentBPlusTree = treeInfo->currentBPlusTree->arrayOfNode[zero  * one];
                    checkValuu = negOne  * one;
                    leftNode = trueValue;
                }
                else
                {
                    int valueCountss = counterV * one;
                    int index_valuu = (treeInfo->index_value) * one;
                    leftNode = falseValue;
                    treeInfo->index_value = zero  * one;
                    bt_scanHandleValue->mnmgmentData = treeInfo;
                    counter_Value = zero  * one;
                    valueCountss =valueCountss + one * one;
                    *handle = bt_scanHandleValue;
                    flagValueBool = falseValue;
                    counterV = valueCountss;
                    break;
                }
            }
        }
    }
    else
    {
        return RC_ERROR;
    }
    return RC_OK;
}




RC nextEntry(BT_ScanHandle *handle, RID *result)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    int entry_Values = zero * one;
    if (NULL != handle)
    {
        TreeInfo *mgrInfo_treeInfo = handle->mnmgmentData;
        int nextValuuu = zero * one;
        int countValuee = zero * one;
        while (nextValuuu != two * one)
        {
            countValuee = nextValuuu * one;
            bool val1 = mgrInfo_treeInfo->index_value;
            bool booleaninfo = mgrInfo_treeInfo->currentBPlusTree->childRightNodeInfo->completed <= val1;
            countValuee = countValuee + one * one;
            bool boolean_check = mgrInfo_treeInfo->index_value == mgrInfo_treeInfo->currentBPlusTree->nodeFieldsInfo->completed;
            bool checkInfoBooleanVal = mgrInfo_treeInfo->currentBPlusTree->childRightNodeInfo->completed <= mgrInfo_treeInfo->index_value;
            ;
            if (false != checkInfoBooleanVal)
            {
                if (NULL == mgrInfo_treeInfo->currentBPlusTree->nodeRight_bplusTree && boolean_check)
                {
                    return RC_IM_NO_MORE_ENTRIES;
                }
                else
                {
                    leftNode = false;
                    entry_Values=entry_Values + one * one;
                    mgrInfo_treeInfo->currentBPlusTree = mgrInfo_treeInfo->currentBPlusTree->nodeRight_bplusTree;
                    (*mgrInfo_treeInfo).index_value = zero * one;
                }
            }
            leftNode = true;
            nextValuuu = countValuee;
        }
        counter_Value = zero  * one;
        if (counter_Value == zero * one)
        {
            int index_valuu = mgrInfo_treeInfo->index_value * one;
            result->page = mgrInfo_treeInfo->currentBPlusTree->childRightNodeInfo->mgmtInfo[index_valuu * one];
            result->slot = mgrInfo_treeInfo->currentBPlusTree->childLeft_nodeInfo->mgmtInfo[index_valuu * one];
            int indexValuess = mgrInfo_treeInfo->index_value + one * one;
            mgrInfo_treeInfo->index_value = indexValuess;
        }
    }
    else
    {
        entry_Values = negOne * one;
        return RC_ERROR;
    }
    return RC_OK;
}





RC closeTreeScan(BT_ScanHandle *handle)
{
    int zero=0;
    int one=1;
    int negOne = -1;
    int four=4;
    int two=2;
    int three=3;
    bool trueValue=true;
    bool falseValue=false;
    
    int sizeOfInt = sizeof(int);
    int sizeOf_char = sizeof(char);
    int sizeOf_SM_FileHandle = sizeof(SM_FileHandle);
    int sizeOf_BM_BufferPool = sizeof(BM_BufferPool);
    int sizeOf_BM_PageHandle = sizeof(BM_PageHandle);
    int sizeOf_BTreeHandle = sizeof(BTreeHandle);
    int sizeOf_BPlusTree = sizeof(BPlusTree);
    int sizeOf_BT_ScanHandle = sizeof(BT_ScanHandle);
    bool booleanTemporaryValuu = falseValue;
    BT_ScanHandle *handleBTreeScanBT_ScanHandle = (BT_ScanHandle *)malloc(sizeOf_BT_ScanHandle);
    if (handleBTreeScanBT_ScanHandle->tree_BTreeHandle == NULL)
    {
        counter_Value=counter_Value - one * one;
        bool booleanTempData = falseValue;
        bool booleanTempData2 = trueValue;
        return RC_ERROR;
    }
    else
    {
        bool booleanTemp = falseValue;
        bool booleanTempData2 = trueValue;
        counter_Value=(counter_Value + one) * one;
    }
    booleanTemporaryValuu = trueValue;
    if (handle != NULL && booleanTemporaryValuu)
    {
        handle->tree_BTreeHandle = NULL;
        handle->mnmgmentData = NULL;
    }
    free(handle);
    booleanTemporaryValuu = falseValue;
    return RC_OK;
}
