# Record Manager - Assignment 3

## Team Members

| CWID         | Name              | Contribution (description)                                                                                                                                        | Percent Contribution |
|-------------|------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------|
| **A20555189** | **Anish Roy**       | Implemented functions `initIndexManager`, `shutdownIndexManager`, `createBtree`, `openBtree`,`closeBtree`, merging code and testing. | 33.3% |
| **A20576979** | **Anushka Sarath**  | Implemented functions `deleteBtree`, `getNumNodes`, `getNumEntries`, `getKeyType`, `findKey`, `insertKey`, loom video presentation.       | 33.3% |
| **20548618**  | **Rebecca Thomson**  | Implemented functions `deleteKey`, `openTreeScan`, `nextEntry`,  `closeTreeScan`,  `printTree`, and completing Readme.md file.                                    | 33.3% |

---

## 📂 Folder Structure

The `assign4` folder contains the following files:

📌 **Core Implementation:**
- **`btree_mgr.h`** - Defines structures (`BTreeHandle`, `BT_ScanHandle`, `treeNode`) and declares B+ Tree Manager functions.
- **`btree_mgr.c`** - Implements functions declared in `btree_mgr.h`.
- **`buffer_mgr.h`** - Defines structures (`ReplacementStrategy`, `BM_BufferPool`, `BM_PageHandle`) and declares Buffer Manager functions.
- **`buffer_mgr.c`** - Implements functions declared in `buffer_mgr.h`.
- **`storage_mgr.h`** - Declares file handling methods (`SM_FileHandle`, `SM_PageHandle`).
- **`storage_mgr.c`** - Implements methods declared in `storage_mgr.h`.
- **`record_mgr.h`** - Declares the record handling methods such as `initRecordManager`, `shutdownRecordManager`,`insertRecord` etc.
- **`record_mgr.c`** - Implements methods declared in `record_mgr.h`.
- **`dberror.h`** - Defines error codes, macros for error handling.
- **`dberror.c`** - Implements error handling functions.

📌 **Testing & Utilities:**
- **`test_assign4_1.c`** - Test cases for creating a record, deleting a record, updating record, checking record.
- **`test_helper.h`** & **`dt.h`** - Define macros for testing.
- **`buffer_mgr_stat.h`** & **`buffer_mgr_stat.c`** - Display buffer/page content.
- **`Makefile`** - Compilation & execution automation..

📌 **Additional:**
- **Output PDF** (`assign4/RecordManagerOutput.pdf`) - Screenshots of successful test executions.



## 🚀 Functions & Descriptions


- `extern RC initIndexManager (void *mgmtData)`  
  _This function initializes the Index Manager._

- `extern RC shutdownIndexManager ()`  
  _It shuts down the Index Manager, releasing all resources._


- `extern RC createBtree (char *idxId, DataType keyType, int n) `  
  _This function createBtree is responsible for creating a B+ Tree by initializing a index manager._

- `extern RC openBtree (BTreeHandle **tree, char *idxId)`  
  _This function openBtree is responsible for opening an existing B+ Tree._

- `extern RC closeBtree (BTreeHandle *tree)`  
  _This function is responsible for closing a B+ Tree in the Index Manager. It involves shutting down the B+ Tree and performing any necessary cleanup operations._
  
  
- `extern RC deleteBtree (char *idxId)`  
  _The function deleteBtree is responsible for deleting a B+ Tree from the Index Manager by removing root, interior, and leaf nodes from the tree and removing the existing page file._

- `extern RC getNumNodes (BTreeHandle *tree, int *result)`  
  _The function getNumNodes retrieves the number of nodes (root,interior, and leaf) in a given B+ Tree (Index Manager).It is a getter function used to access metadata related to the B+ Tree._

- `extern RC getNumEntries (BTreeHandle *tree, int *result)`  
  _The function getNumEntries retrieves the number of entries (<key, record pointer> pairs found in the leaf nodes) in a given B+ Tree (Index Manager).It is a getter function used to access metadata related to the B+ Tree._

- `extern RC getKeyType (BTreeHandle *tree, DataType *result)`  
  _The function getKeyType determines the key datatype._

- `extern RC findKey (BTreeHandle *tree, Value *key, RID *result)`  
  _The function determines if the given key exists within the B+ Tree.  If found, it returns the RID for the entry within the search key, otherwise it returns RC_IM_KEY_NOT_FOUND._


- ` extern RC insertKey (BTreeHandle *tree, Value *key, RID rid)`  
  _This function inserts a new key and record pointer pair into the B+ Tree.  This function will trigger any new B+ Tree configuration as needed.  If the key already exists, the function returns RC_IM_KEY_ALREADY_EXISTS._


- `extern RC deleteKey (BTreeHandle *tree, Value *key) `  
  _This function deletes the given key and record pointer from the tree.  This function will trigger any new B+ Tree configuration as needed.  If the key does not exist, it returns RC_IM_KEY_NOT_FOUND._

- `extern RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle)`  
  _This function begins a scan of all entries of the B+ Tree._


- `extern RC nextEntry (BT_ScanHandle *handle, RID *result)`  
  _This function scans the next entry of the B+ Tree.  When no more entries are returned, RC_NO_MORE_ENTRIES is returned._


- `extern RC closeTreeScan (BT_ScanHandle *handle) `  
  _This function closes the B+ Tree Scan._


- `extern char *printTree (BTreeHandle *tree);`  
  _This function prints every node of the B+ Tree in a Breadth First Search of the entire tree.  Each line is an individual node.  Each interior node follows the format: (Node Number)[Child Node Number, Key Value,...]. Each leaf nod follows the format of (Node Number)[RID,Key Value]._




---

## 🏗️ Compilation & Execution
**Platform:** Windows 10

Navigate to the git/assign3 directory
```windows command prompt
cd "<path to git/assign4 directory>"

Example
cd "C:\Users\LENOVO\Downloads\ADO\assignment_3\3_newCodes\3\git\assign4"

```

Compile using make and remove old files:
```windows command prompt
make
```

Run the compiled file and output in a text output file:
```windows command prompt
make run >  <outputFile.txt>

Example:
make run > output_4.txt
```

---

## 🎥 Video Demonstration
A video demonstration of the project is available at the following link:

https://www.loom.com/share/01c76ef8a42b4aff832b5b9ae301dfc2?sid=ac89594f-b5e0-4178-9839-d0cbc31e682d

Click the link above to watch the demonstration .

---

## 📌 Summary
This project implements a **Index Manager** in a database. It creates a B+ Tree following the logic covered in CS525, Advanced Database Organization.  Functions for creating B+ Trees, inserting new <Key, value> pairs, finding existing keys, and deleting the keys are provided.

 

 **Output verification available in the PDF report under git/assign4!**

---

 **Developed as part of Assignment 3 - Record Manager**
