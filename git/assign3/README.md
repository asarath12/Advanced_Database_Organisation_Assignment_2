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

---

---


---

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

 [Watch the Project Demo]

Click the link above to watch the demonstration .

---

## 📌 Summary
This project implements a **Record Manager** in a database. 

 

 **Output verification available in the PDF report under git/assign3!**

---

 **Developed as part of Assignment 3 - Record Manager**
