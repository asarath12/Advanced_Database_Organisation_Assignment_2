#ifndef RECORD_MGR_H
#define RECORD_MGR_H

#include "dberror.h"
#include "expr.h"
#include "tables.h"
#include "buffer_mgr.h"
#include "storage_mgr.h"

// Bookkeeping for scans
typedef struct RM_ScanHandle
{
	RM_TableData *rel;
	void *mgmtData;
} RM_ScanHandle;

/*
This structure manages records in the DBMS.
Fields:
	BM_BufferPool bm_buffer_pool; 
o	Buffer pool for managing pages in memory.
	int tCount; 
o	Tracks the number of tables.
	int scans_count; 
o	Counts active scans.
	Expr *conditions; 
o	Stores conditions for filtering records.
	RID record_ID; 
o	Stores a Record Identifier (RID) for locating records.
	RM_ScanHandle rm_scanHandle; 
o	Scan handle to manage table scanning.
	int free_Page; 
o	Stores the first free page in the table.
	BM_PageHandle bm_pageHandle; 
o	Manages page data in the buffer pool.
*/
typedef struct RecordManager
{
	BM_BufferPool bm_buffer_pool;	
	int tCount;
	int scans_count;
	Expr *conditions;
	RID record_ID;
	RM_ScanHandle rm_scanHandle;
	int free_Page;
	BM_PageHandle bm_pageHandle;	
} RecMgr;


// table and manager
extern RC initRecordManager (void *mgmtData);
extern RC shutdownRecordManager ();
extern RC createTable (char *name, Schema *schema);
extern RC openTable (RM_TableData *rel, char *name);
extern RC closeTable (RM_TableData *rel);
extern RC deleteTable (char *name);
extern int getNumTuples (RM_TableData *rel);


// handling records in a table
extern RC insertRecord (RM_TableData *rel, Record *record);
extern RC deleteRecord (RM_TableData *rel, RID id);
extern RC updateRecord (RM_TableData *rel, Record *record);
extern RC getRecord (RM_TableData *rel, RID id, Record *record);





// scans
extern RC startScan (RM_TableData *rel, RM_ScanHandle *scan, Expr *cond);
extern RC next (RM_ScanHandle *scan, Record *record);
extern RC closeScan (RM_ScanHandle *scan);




// dealing with schemas
extern int getRecordSize (Schema *schema);
extern Schema *createSchema (int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys);
extern RC freeSchema (Schema *schema);




// dealing with records and attribute values
extern RC createRecord (Record **record, Schema *schema);
extern RC freeRecord (Record *record);
extern RC getAttr (Record *record, Schema *schema, int attrNum, Value **value);
extern RC setAttr (Record *record, Schema *schema, int attrNum, Value *value);

#endif // RECORD_MGR_H
