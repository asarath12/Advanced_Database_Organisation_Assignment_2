# Record Manager - Assignment 3

## Team Members

| CWID         | Name              | Contribution (description) | Percent Contribution |
|-------------|------------------|---------------------------|----------------------|
| **A20555189** | **Anish Roy**       | Implemented functions `initRecordManager`, `shutdownRecordManager`, `createTable`, `openTable`,`closeTable`,`deleteTable`,`getNumTuples`, merging code and testing. | 33.3% |
| **A20576979** | **Anushka Sarath**  | Implemented functions `getRecordSize`, `createSchema`, `freeSchema`, `createRecord`, `freeRecord`, `getAttr`, `setAttr` and completing Readme.md file. | 33.3% |
| **20548618**  | **Rebecca Thomson**  | Implemented functions `insertRecord`, `deleteRecord`, `updateRecord`,  `getRecord`,  `startScan`,  `next`,  `closeScan`. | 33.3% |

---

## 📂 Folder Structure

The `assign3` folder contains the following files:

📌 **Core Implementation:**
- **`buffer_mgr.h`** - Defines structures (`ReplacementStrategy`, `BM_BufferPool`, `BM_PageHandle`) and declares Buffer Manager functions.
- **`buffer_mgr.c`** - Implements functions declared in `buffer_mgr.h`.
- **`storage_mgr.h`** - Declares file handling methods (`SM_FileHandle`, `SM_PageHandle`).
- **`storage_mgr.c`** - Implements methods declared in `storage_mgr.h`.
- **`record_mgr.h`** - Declares the record handling methods such as `initRecordManager`, `shutdownRecordManager`,`insertRecord` etc.
- **`record_mgr.c`** - Implements methods declared in `record_mgr.h`.
- **`dberror.h`** - Defines error codes, macros for error handling.
- **`dberror.c`** - Implements error handling functions.

📌 **Testing & Utilities:**
- **`test_assign3_1.c`** - Test cases for creating a record, deleting a record, updating record, checking record.
- **`test_helper.h`** & **`dt.h`** - Define macros for testing.
- **`buffer_mgr_stat.h`** & **`buffer_mgr_stat.c`** - Display buffer/page content.
- **`Makefile`** - Compilation & execution automation..

📌 **Additional:**
- **Output PDF** (`assign3/RecordManagerOutput.pdf`) - Screenshots of successful test executions.



## 🚀 Functions & Descriptions


- `extern RC initRecordManager (void *mgmtData)`  
  _This function initializes the Record Manager._

- `extern RC shutdownRecordManager ()`  
  _It shutsdown the record manager._


- `extern RC createTable(char *name, Schema *schema) `  
  _This function createTable is responsible  for creating a database table by  initializing a buffer pool,  writing metadata to a storage page,  and setting up schema-related attributes._

- `extern RC openTable (RM_TableData *rel, char *name)`  
  _  This function openTable is responsible for  opening an existing database table.  It loads the schema metadata from  storage, initializes a Schema structure,  and sets up memory management  for attributes._

- `extern RC closeTable(RM_TableData *rel)`  
  _This function is responsible for closing a table in the Record Manager. It involves shutting down the buffer pool and performing any necessary cleanup operations.._
  
  
- `extern RC deleteTable (char *name)`  
  _The function deleteTable is responsible for deleting a table from storage by removing  its associated page file._

- `extern int getNumTuples (RM_TableData *rel)`  
  _The function getNumTuples retrieves the number of tuples (records) in a given relation (table).It is a getter function used to access metadata related to the table._


- `extern RC insertRecord (RM_TableData *rel, Record *record)`  
  _The function insertRecord inserts a new record into a table by finding a free slot in a buffered page, updating metadata, and writing data into the slot.._


- ` extern RC deleteRecord(RM_TableData *rel, RID id) `  
  _The function marks a record as deleted in a buffered page, updates metadata, and writes the changes back to disk.._




- ` extern RC updateRecord (RM_TableData *rel, Record *record) `  
  _The function updates an existing record in a buffered page, marks it as modified, and writes the changes back to disk.._


- `extern RC getRecord(RM_TableData *rel, RID id, Record *record)`  
  _The function retrieves a record from a table using its Record ID (RID).._


- ` extern RC startScan(RM_TableData *rel, RM_ScanHandle *scan, Expr *cond) `  
  _This function initiates a scan on a table, applying a condition (Expr *cond) to filter records._


- ` extern RC next (RM_ScanHandle *scan, Record *record) `  
  _It scans the records sequentially, applies conditions (if any), and returns the next valid tuple._


- ` extern RC closeScan (RM_ScanHandle *scan) `  
  _This closes an ongoing scan operation in a record manager system. It resets relevant metadata and releases any allocated resources.._


- `extern int getRecordSize (Schema *schema)`  
  _It calculates and returns the total size of a record based on its attributes and their data types._


- ` extern Schema *createSchema (int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys)`  
  _This function dynamically creates a Schema structure,initializes its fields,  and returns a pointer to it._


- ` extern RC freeSchema (Schema *schema) `  
  _This function frees the allocated memory for a Schema structure._

- `extern RC createRecord (Record **record, Schema *schema)`  
  _This function is responsible for creating and initializing a new Record structure based on a given Schema._


- `extern RC freeRecord (Record *record)`  
  _This function is responsible for deallocating memory associated with a Record object._


- ` extern RC getAttr (Record *record, Schema *schema, int attrNum, Value **value) `  
  _This function retrieves the value of a specific attribute from a given record based on the schema._


- `extern RC setAttr (Record *record, Schema *schema, int attrNum, Value *value);`  
  _This function sets the value of a specific attribute in a given record according to the schema._




---

## 🏗️ Compilation & Execution
**Platform:** Windows 10

Navigate to the git/assign3 directory
```windows command prompt
cd "<path to git/assign3 directory>"

Example
cd "C:\Users\LENOVO\Downloads\ADO\assignment_3\3_newCodes\3\git\assign3"

```

Compile using make and remove old files:
```windows command prompt
make
```

Run the compiled file and output in a text output file:
```windows command prompt
make run >  <outputFile.txt>

Example:
make run > output_3.txt
```

---

## 🎥 Video Demonstration
A video demonstration of the project is available at the following link:

https://www.loom.com/share/01c76ef8a42b4aff832b5b9ae301dfc2?sid=ac89594f-b5e0-4178-9839-d0cbc31e682d

Click the link above to watch the demonstration .

---

## 📌 Summary
This project implements a **Record Manager** in a database. 

 

 **Output verification available in the PDF report under git/assign3!**

---

 **Developed as part of Assignment 3 - Record Manager**
