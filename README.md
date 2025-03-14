# Record Manager - Assignment 3

## Team Members
| CWID       | Name             | Contribution (description)                                      | Percent Contribution |
|------------|----------------|----------------------------------------------------------------|----------------------|
| A20555189  | Anish Roy      | Implemented table management functions including initRecordManager, shutdownRecordManager, createTable, openTable, closeTable, deleteTable, and getNumTuples. Merging code and testing. | 33.3% |
| A20576979  | Anushka Sarath  | Implemented schema and attribute functions including getRecordSize, createSchema, freeSchema, createRecord, freeRecord, getAttr, and setAttr. Completing Readme.md file | 33.3% |
| 20548618   | Rebecca Thomson | Implemented record handling and scanning functions including insertRecord, deleteRecord, updateRecord, getRecord, startScan, next, and closeScan. | 33.3% |

---

## 📂 Folder Structure
The `assign3` folder contains the following files:

### 📌 **Core Implementation:**
- `record_mgr.h` - Declares Record Manager functions.
- `record_mgr.c` - Implements functions declared in `record_mgr.h`.
- `tables.h` - Defines structures related to record management (`Schema`, `Record`, `RM_TableData`, etc.).
- `storage_mgr.h` - Declares file handling functions (`SM_FileHandle`, `SM_PageHandle`).
- `storage_mgr.c` - Implements methods for managing storage pages.
- `dberror.h` - Defines error codes and error-handling macros.
- `dberror.c` - Implements error handling functions.

### 📌 **Testing & Utilities:**
- `test_assign3_1.c` - Contains test cases for basic table and record operations.
- `test_assign3_2.c` - Contains test cases for scanning and schema operations.
- `test_expr.c` - Tests expressions used in record scanning.
- `test_helper.h` & `dt.h` - Define macros for testing.
- `rm_serializer.c` - Serializes and deserializes records.
- `Makefile` - Compilation & execution automation with thread support.

---

## 🚀 **Functions & Descriptions**

### 🛠 **Table Management**
- `RC initRecordManager(void *mgmtData)` - Initializes the Record Manager.
- `RC shutdownRecordManager()` - Shuts down the Record Manager.
- `RC createTable(char *name, Schema *schema)` - Creates a new table.
- `RC openTable(RM_TableData *rel, char *name)` - Opens an existing table.
- `RC closeTable(RM_TableData *rel)` - Closes the table.
- `RC deleteTable(char *name)` - Deletes the table from storage.
- `int getNumTuples(RM_TableData *rel)` - Returns the number of tuples in the table.

### 📄 **Handling Records**
- `RC insertRecord(RM_TableData *rel, Record *record)` - Inserts a new record into a table.
- `RC deleteRecord(RM_TableData *rel, RID id)` - Deletes a record with a given RID.
- `RC updateRecord(RM_TableData *rel, Record *record)` - Updates an existing record.
- `RC getRecord(RM_TableData *rel, RID id, Record *record)` - Retrieves a record by its RID.

### 🔍 **Scanning Records**
- `RC startScan(RM_TableData *rel, RM_ScanHandle *scan, Expr *cond)` - Initializes a table scan.
- `RC next(RM_ScanHandle *scan, Record *record)` - Retrieves the next record that matches a scan condition.
- `RC closeScan(RM_ScanHandle *scan)` - Closes the scan and frees resources.

### 🏗️ **Schema Operations**
- `int getRecordSize(Schema *schema)` - Returns the size of a record based on the schema.
- `Schema *createSchema(int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys)` - Creates a schema definition.
- `RC freeSchema(Schema *schema)` - Frees allocated memory for a schema.

### 🎯 **Attribute Handling**
- `RC createRecord(Record **record, Schema *schema)` - Allocates memory and initializes a new record.
- `RC freeRecord(Record *record)` - Frees memory allocated for a record.
- `RC getAttr(Record *record, Schema *schema, int attrNum, Value **value)` - Retrieves an attribute value from a record.
- `RC setAttr(Record *record, Schema *schema, int attrNum, Value *value)` - Sets an attribute value in a record.

---

## 🏗️ **Compilation & Execution**
**Platform:** Ubuntu 20.04 (Linux/Unix)

1️⃣ **Clean previous builds:**
```sh
make clean
```

2️⃣ **Compile the code:**
```sh
make
```

3️⃣ **Set permissions for execution:**
```sh
chmod -R 777 <fullFilePath_of_directory>
```
_Example:_
```sh
chmod -R 777 /home/user1/record_manager/assign3
```

4️⃣ **Run Test Case 1 and store output in 1.txt:**
```sh
./test_assign3_1 > 1.txt 2>&1
```

5️⃣ **Run Test Case 2 and store output in 2.txt:**
```sh
./test_assign3_2 > 2.txt 2>&1
```

---

## 🎥 **Video Demonstration**
A video demonstration of the project is available at the following link:

🔗 **[Watch the Project Demo](#)** *(Insert video link here)*

---

## 📌 **Summary**
This project implements a **Record Manager** that supports table creation, record insertion, deletion, updates, scanning, and schema management. The test cases validate the implementation for correctness.

✅ Efficient record management
✅ Scanning & query filtering support
✅ Automated testing & debugging
✅ Thread-safe operations

📜 Output verification available in test execution logs under `assign3/output.txt`!

🎯 **Developed as part of Assignment 3 - Record Manager** 🚀
