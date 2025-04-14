
# Assignment 4
Advanced Database Organizations


## Team Members

- A20555189-----Anish Roy------------33.33%
- A20576979-----Anushka Sarath-------33.33%
- A20548618-----Rebecca Thomson------33.33%


## Team Contribution
**Anish Roy** : 
• extern RC deleteBtree (char *idxId);
• extern RC getNumNodes (BTreeHandle *tree, int *result);
• extern RC getNumEntries (BTreeHandle *tree, int *result);
• extern RC getKeyType (BTreeHandle *tree, DataType *result);
• extern RC findKey (BTreeHandle *tree, Value *key, RID *result);
• loom video presentation
• merging code and testing

**Anushka Sarath** :
• extern RC initIndexManager (void *mgmtData);
• extern RC shutdownIndexManager ();
• extern RC createBtree (char *idxId, DataType keyType, int n);
• extern RC openBtree (BTreeHandle **tree, char *idxId);
• extern RC closeBtree (BTreeHandle *tree);
• extern RC insertKey (BTreeHandle *tree, Value *key, RID rid);



**Rebecca Thomson** : 
• extern RC deleteKey (BTreeHandle *tree, Value *key);
• extern RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle);
• extern RC nextEntry (BT_ScanHandle *handle, RID *result);
• extern RC closeTreeScan (BT_ScanHandle *handle);
• extern char *printTree (BTreeHandle *tree);
• README file


## Folder structure

The assign4 folder contains the following files:
expr.h
expr.c
dt.h
dberror.c
buffer_mgr_stat.c
record_mgr.h
storage_mgr.c
test_assign4_1.c
buffer_mgr_stat.h
tables.h
test_helper.h
record_mgr.c
buffer_mgr.c
buffer_mgr.h
makefile
storage_mgr.h
test_expr.c
dberror.h
rm_serializer.c
btree_mgr.h
makeClean.txt
makeRun.txt
test_assign4_1.o
btree_mgr.o
rm_serializer.o
record_mgr.o
dberror.o
storage_mgr.o
buffer_mgr.o
expr.o
output1
4.txt
btree_mgr.c
README.md

## Output Screenshot:
We have attached a picture in assign4 directory called "Output_Screenshot", which contains the screenshots of successful execution of test_assign4_1.c.

## Executing the code:
- Open the assign4 folder in the terminal
- Run `make clean` to delete the output files (if any)
- Run `make` to compile and build
- Run `chmod 777 *.*` to change file permissions
- Run `make run 0>"outputFileName.txt" 1>"outputFileName.txt" 2>"outputFileName.txt"` to run and output the logs in output file "outputFileName.txt"

  Example: `make run 0>4.txt 1>4.txt 2>4.txt` to run and output the logs in output file "4.txt"


- Run `grep -c "OK: expected" "outputFileName.txt"` to see the count of successful test case execution
 
 example: Run `grep -c "OK: expected" "4.txt"` to see the count of successful test case execution


- Run `grep -c "exception" "outputFileName.txt"` to see the count of exception in test case execution
   Example: Run `grep -c "exception" 4.txt` to see the count of exception in test case execution



## Functions of btree_mgr.h implemented

__extern RC initIndexManager (void *mgmtData):__
This method initializes the index manager.

__extern RC shutdownIndexManager():__
This method is used to shuwdown the index manager. 

__extern RC createBtree (char *idxId, DataType keyType, int n):__
This method is used to create the tree with the given idxId, and keyType. It intializes the treemanager and stores additional information of the B+ tree.

__extern RC openBtree (BTreeHandle **tree, char *idxId):__
This method is used to open the existing B+tree, stored specified by the idxId value.

__extern RC closeBtree (BTreeHandle *tree):__
This method closes the B+ tree and the buffer pool is also shutdown freeing up the memory allocated.

__extern RC deleteBtree (char *idxId):__
This method deletes the tree and unlink the tree by indexId.

__extern RC getNumNodes (BTreeHandle *tree, int *result):__
This method gives the number of nodes in the tree

__RC getNumEntries(BTreeHandle *BTreeHandle, int *res):__
This method is used to get the number of entities present in the tree.

__extern RC getKeyType (BTreeHandle *tree, DataType *result)__
This method is used to get the data type of the key stored in the B+ tree. 

__extern RC findKey (BTreeHandle *tree, Value *key, RID *result)__
This method is used to search in the tree using the key which is specified as the argument in the function.

__extern RC insertKey (BTreeHandle *tree, Value *key, RID rid)__
This method is used to insert the key in the tree. For this, we find the tree using the key specified in the argument and if present, adds to the existing tree or else creates a new tree.

__extern RC deleteKey (BTreeHandle *tree, Value *key)__:
This method retrieves the child using the key specified and searches in tree, and then deletes the key of the tree.

__extern RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle):__
This method initializes the scan in order to scan the elements in the B+ Tree in the sorted key order.

__extern RC nextEntry (BT_ScanHandle *handle, RID *result):__
This method is used to traverse through the entities of the tree and throws RC_IM_NO_MORE_ENTRIES when there are no more entities to visit.

__extern RC closeTreeScan (BT_ScanHandle *handle)__
This method is used to end the tree scan and freeing up the memory allocated.

__extern char *printTree (BTreeHandle *tree):__
This method is used to print the B+ tree.
