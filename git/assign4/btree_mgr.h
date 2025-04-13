#ifndef BTREE_MGR_H
#define BTREE_MGR_H

#include "dberror.h"
#include "tables.h"
#include "dt.h"

//user-defined struct
typedef struct NodeInformation{
  int *mgmtInfo;
  int completed;
  int capacity;
}NodeInformation;



//user-defined struct
typedef struct BPlusTree{
  int numberOfNodes;
  struct BPlusTree *inhrt;
  int checkForCountOfChildren;
  NodeInformation *childRightNodeInfo;
  struct BPlusTree **arrayOfNode;
  NodeInformation *nodeFieldsInfo;
  NodeInformation *nodeBrnch;
  struct BPlusTree *nodeRight_bplusTree;
  NodeInformation *childLeft_nodeInfo;
  struct BPlusTree *nodeLeftBplusTree;
}BPlusTree;



typedef struct BTreeHandle {
  DataType keyType;
  char *idxId;
  void *mgmtData;
  //user-defined variables
  int num_Of_Children;
  int treeRootIndexValue;
  int next_Page_Info;
  BPlusTree *BPlusTree_root;
  int numOfFields;
  int depthOfTheTree;
  int capacityVal;
} BTreeHandle;



//user-defined struct
typedef struct TreeInfo{
  BPlusTree *currentBPlusTree;
  int index_value;
}TreeInfo;



typedef struct BT_ScanHandle {
  BTreeHandle *tree_BTreeHandle;
  void *mnmgmentData;
} BT_ScanHandle;



// init and shutdown index manager
extern RC initIndexManager (void *mgmtData);
extern RC shutdownIndexManager ();

// create, destroy, open, and close an btree index
extern RC createBtree (char *idxId, DataType keyType, int n);
extern RC openBtree (BTreeHandle **tree, char *idxId);
extern RC closeBtree (BTreeHandle *tree);
extern RC deleteBtree (char *idxId);

// access information about a b-tree
extern RC getNumNodes (BTreeHandle *tree, int *result);
extern RC getNumEntries (BTreeHandle *tree, int *result);
extern RC getKeyType (BTreeHandle *tree, DataType *result);


// index access
extern RC findKey (BTreeHandle *tree, Value *key, RID *result);
extern RC insertKey (BTreeHandle *tree, Value *key, RID rid);
extern RC deleteKey (BTreeHandle *tree, Value *key);
extern RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle);
extern RC nextEntry (BT_ScanHandle *handle, RID *result);
extern RC closeTreeScan (BT_ScanHandle *handle);

// debug and test functions
extern char *printTree (BTreeHandle *tree);

#endif // BTREE_MGR_H
