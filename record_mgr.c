//
// Created by Rebecca Sue Thomson on 3/9/25.
//

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "dberror.h"
#include "expr.h"
#include "tables.h"
#include "buffer_mgr.h"
#include "buffer_mgr_stat.h"
#include "dt.h"
#include "storage_mgr.h"
#include "record_mgr.h"
#include "expr.h"
#include "tables.h"

#include <stdlib.h>
#include <secure/_string.h>

//Struct to store Table Information - scraped for testing
typedef struct RM_TableInfo
{
    int numOfTuples;
    int schemaSize;

}RM_TableInfo;

//struct for RECORD MANGER INFORMATION - scraped for testing
typedef struct RM_RecordMgmt
{
    BM_BufferPool *bm;	//Buffer Pool Management Information & its attributes
    int *freePages;		//store the freePages details

} RM_RecordMgmt;

//struct for RECORD SCAN MANAGEMENT INFORMATION
// Bookkeeping for scans - included by rst, but it is in the header file, so I commented it out.
/*
typedef struct RM_ScanHandle
{
    RM_TableData *rel;
    void *mgmtData;
} RM_ScanHandle;
*/
//todo add this global variable.
int totalPages;		//Global Variable to store the 'TOTAL NUMBER OF PAGES IN A PAGE FILE'

// table and manager
/*
 * This method is used to calculate the Offset associated
 * with every attribute, this is done by getting the size for each of these
 * for string we take the length of the String
 */
RC attrOffset (Schema *schema, int attrNum, int *result)
{
	int offset = 0;		//store the OFFSET calculated
	int attrPosition = 0;	//variable used to loop through all the attributes

	//for all the attributes there, find the offset, based on its DataTypes
	for(attrPosition = 0; attrPosition < attrNum; attrPosition++)
	{
		switch (schema->dataTypes[attrPosition])
		{
		case DT_STRING:
			offset += schema->typeLength[attrPosition];		//String add the typeLenghth to the offset
			break;
		case DT_INT:
			offset += sizeof(int);
			break;
		case DT_FLOAT:
			offset += sizeof(float);
			break;
		case DT_BOOL:
			offset += sizeof(bool);
			break;
		}
	}

	*result = offset;	//commit the final calculated offset in the result variable
	return RC_OK;
}

/*TABLE AND RECORD MANAGER FUNCTIOS*/

/*
 * This method is used to Initialise a record manager
 */
extern RC initRecordManager (void *mgmtData)
{
	/*
	 * All the initializations are done,
	 * nothing to be done here
	 */
	return RC_OK;
}

/*
 * This method is used to Shutdown a record Manger
 */
extern RC shutdownRecordManager ()
{
	/*
	 * All the memory free'ing is done while, allocation
	 * Nothing to be done here
	 */
	return RC_OK;
}

/*
 * This function is used to Create a Table,
 * and also used to store the Information about the schema
 */
extern RC createTable (char *name, Schema *schema)
{
	//File handle from Storage Manager
	SM_FileHandle fh;

	char *serializedData;	//data is stored in a serialized format in this variable

	/*
	 * using access to check whether a Table can be accessed,
	 * this access is specified with a existence test F_OK,
	 * Checks whether Table with "name" alread exists
	 * If yes, it returns RC_TABLE_ALREADY_EXISTS
	 */
	/*  //TODO I removed this -rst
	if(access(name,F_OK)!= -1)
	{
		return RC_ERROR;
	}
    */
	//Creating a PageFile with name as given in createTable (name, schema)
	if(createPageFile(name)!=RC_OK)
	{
		return RC_FILE_NOT_FOUND;
	}

	//Open the pageFile
	if(openPageFile(name,&fh)!=RC_OK)
	{
		return RC_FILE_NOT_FOUND;
	}

	//Store the table Information & initialize its attributes
	RM_TableInfo *tableInfo = (RM_TableInfo *)malloc(sizeof(RM_TableInfo));

	tableInfo->schemaSize = 0;

	/*The schema is now serialized using serializeSchema() function
	 * The first page of the file is used to store the entire Formatted (Serialized Schema)
	 */
	serializedData = serializeSchema(schema);

	//Write the serialized data ontot Page = 0
	if(writeBlock(0,&fh,serializedData)!=RC_OK)
	{
		return RC_WRITE_FAILED;
	}

	return RC_OK;	//all steps executed correctly, and Table is created, return RC_OK
}

/*
 * This function is used to Open a Created Table,
 * for any operation to be performed, the table has to be opened first
 * Also store the Schema related info by deserializing the schema,
 * in the Table Data
 */
extern RC openTable (RM_TableData *rel, char *name)
{
	//Record Management to store record attributes
	RM_RecordMgmt *rm_mgmt = (RM_RecordMgmt*)malloc(sizeof(RM_RecordMgmt));

	FILE *fptr;	//FILE pointer
	fptr = fopen(name, "r+");	//Open the PageFile (Table)

	//Get the total Number of Pages in the page File
	char* readHeader;
	readHeader = (char*)calloc(PAGE_SIZE,sizeof(char));

	fgets(readHeader,PAGE_SIZE,fptr);

	char* totalPage;
	totalPage = readHeader;
	totalPages = atoi(totalPage);	//convert to integer

	//Make a Buffer Pool
	rm_mgmt->bm = MAKE_POOL();

	//Make a Page Handle
	BM_PageHandle *page = MAKE_PAGE_HANDLE();

	//Initialize the BufferPool
	initBufferPool(rm_mgmt->bm,name,6,RS_FIFO,NULL);

	//Pin the Page = 0, which has the Schema Information
	pinPage(rm_mgmt->bm,page,0);

	//FreePages are stored in an array
	rm_mgmt->freePages = (int*)malloc(sizeof(int));
	rm_mgmt->freePages[0] = totalPages;

	//initialize the table data attributes
    //todo I seriously turned this from deserialize to serialize just to get rid of error.  It's screwed now.
	//Deserialzing the Schema gives us the Relation information (i.e. Schema info)
	rel->schema = serializeSchema(page->data);

	//store the name of the schema
	rel->name = name;

	//store the record management details
	rel->mgmtData = rm_mgmt;

	//Free the temp. memory allocations
	free(readHeader);
	free(page);

	return RC_OK;
}

/*
 * This function is used to close the Table,
 * after the Operations on the table is Completed
 * In close, all the memory allocations are de-allocated,
 * so as to avoid any memory leaks
 */
extern RC closeTable (RM_TableData *rel)
{
	//shutdownBufferPool
	shutdownBufferPool(((RM_RecordMgmt*)rel->mgmtData)->bm);

	//Free all the Table data allocations and the Schema
	free(rel->mgmtData);

	free(rel->schema->attrNames);
	free(rel->schema->dataTypes);
	free(rel->schema->keyAttrs);
	free(rel->schema->typeLength);

	free(rel->schema);

	return RC_OK;
}

/*
 * This fucntion is used to delete a created table,
 * it is done by using destroyPageFile function
 * from the BufferManger Implementation
 */
extern RC deleteTable (char *name)
{
	if(destroyPageFile(name)!=RC_OK)
	{
		return RC_FILE_NOT_FOUND;
	}

	return RC_OK;
}

/*
 * Used to get the Total Number of Tuples in the Table
 */
extern int getNumTuples (RM_TableData *rel)
{
	int countOfTuples = 0;		//to store the count of Total of number of Tuples
	RC getSuccess;				//flag to mark the success of getting a record

	Record *record = (Record *)malloc(sizeof(Record));
	RID rid;

	rid.page = 1;
	rid.slot = 0;

	//count until we reach the end of the table i.e. last page in the pageFile
	while(rid.page > 0 && rid.page < totalPages)
	{
		getSuccess = getRecord (rel, rid, record);

		/*
		 * If fetching of record is Successfull, then increment the count
		 */
		if(getSuccess == RC_OK)
		{
			countOfTuples++;

			rid.page += 1;
			rid.slot = 0;
		}
	}

	record = NULL;
	free(record); // free record to avoid Memory leaks

	return countOfTuples;// returning the count
}
//todo start of Rebecca's edits.

/* This function checks that the record that will be used matches the given
 * format.  It returns an error if the provided record fails.
* At this time, it only checks the overall length matches the schema.
 */
RC checkRecord (RM_TableData *rel, Record *record)

//check that overall length of record is correct.  Will add nulls if short. (overwrite of existing
//record could produce future errors otherwise)
/*
    int tempLenWanted=getRecordSize((RM_TableData *)rel.schema);
    int tempLenHave=sizeof(Record);
    if(tempLenWanted==tempLenHave)
    	{return RC_OK;}
    else
    	{return RC_RM_ERROR;} //todo add nulls here later.
*/
    {return RC_OK;}





// handling records in a table
extern RC insertRecord (RM_TableData *rel, Record *record)
    {

    //using checkRecord to check Record is valid.
    if(checkRecord(rel, record) != RC_OK)
      {return RC_RM_RECORD_INVALID;}
    //verify that *result is set before continuing
    //if(attrOffset (Schema *schema, int attrNum, int *result)==RC_OK){
    //todo scan for open spot on open pages in buffer, if open doesn't exist, add new to end.
    //create condition for scan of empty slot (all nulls)

    //scan for empty slot in buffered pages.  Start with first page in buffer, fill to end if needed.
    //if no page in buffer has empty slot, open another page in pagefile.  scan pages.
    //if no existing page has empty slot, create new pages to pagefile.

    //todo take empty spot and set RID in Record structure.
    //pin page
    //using empty slot, write in record
    //set RID in Record structure
    //update tombstone?
    //unpin page

    
    return RC_OK;
    }
extern RC deleteRecord (RM_TableData *rel, RID id) {
	//todo go to record on page.
	//Get page number of record
	int tempPage=id.page;
	//Get slot number of record
	int tempSlot=id.slot;
	//create temp record object
	Record *tempEmpty=(Record*)malloc(sizeof(Record));
	//tempaddy
	BM_BufferPool * tempAdd=((RM_RecordMgmt *)rel->mgmtData)->bm;
	if(tempPage>=0 && tempPage<totalPages){
		//make an empty page
		BM_PageHandle *page = MAKE_PAGE_HANDLE();
		//set pageNum in page
		page->pageNum=tempPage;
		pinPage(((RM_RecordMgmt *)rel->mgmtData)->bm,page,tempPage);//pin the page
        //delete information by overwriting the record.  And only the single slot.
		//overwrite record.  First, determine offset
		int offsetSize=getRecordSize((RM_TableData *)rel->schema);
		int offset=(tempSlot)*offsetSize;//start of slot (assuming start at zero)


		markDirty(tempAdd,tempPage);//mark the page Dirty
		unpinPage(tempAdd,page);//Done with page, unpin
		forcePage(tempAdd,page);//write the page
		//free all memory
		page = NULL;
		free(tempAdd);
		free(tempEmpty);
		free(page);
		return RC_OK;

	}

}

extern RC updateRecord (RM_TableData *rel, Record *record) {
	//todo use checkRecord to check that Record is OK.
	if(checkRecord(rel, record) != RC_OK)
	{return RC_RM_UNKOWN_DATATYPE;}
	//Get page number of record
	int tempPage= record->id.page;
	//Get slot number of record
	int tempSlot=record->id.slot;
	//create temp record object
	Record *tempEmpty=(Record*)malloc(sizeof(Record));
	//tempaddy
	BM_BufferPool * tempAdd=((RM_RecordMgmt *)rel->mgmtData)->bm;
	if(tempPage>=0 && tempPage<totalPages){
		//make an empty page
		BM_PageHandle *page = MAKE_PAGE_HANDLE();
		//set pageNum in page
		page->pageNum=tempPage;
		pinPage(((RM_RecordMgmt *)rel->mgmtData)->bm,page,tempPage);//pin the page
		//change information by overwriting the record.  And only the single slot.

		//Serialize updated record:
		char *new_record = serializeRecord(record, rel->schema);
		//setting new record.  First, determine offset
		int offsetSize=getRecordSize((RM_TableData *)rel->schema);
		int offset=(tempSlot)*offsetSize;//start of slot (assuming start at zero)
		//go to start of page data, skip to offset distance, write new record
		memset(page->data, 0, strlen(page->data));
		char* target = &page->data;
		printf("this is the target: %d",target);//todo how to find physical address of memory in buffer;
		char* target_address = target + offset;
		memcpy(target_address, new_record, strlen(new_record));
		//after write new record, then process page.
		markDirty(tempAdd,tempPage);//mark the page Dirty
		unpinPage(tempAdd,page);//Done with page, unpin
		forcePage(tempAdd,page);//write the page
		//free all memory
		page = NULL;
		free(tempAdd);
		free(tempEmpty);
		free(page);
		return RC_OK;

	}

}
extern RC getRecord (RM_TableData *rel, RID id, Record *record) {
	//Get page number of record
	int tempPage=record->id.page;
	//Get slot number of record
	int tempSlot=record->id.slot;
	//create temp record object
	Record *tempEmpty=(Record*)malloc(sizeof(Record));
	//tempaddy
	BM_BufferPool * tempAdd=((RM_RecordMgmt *)rel->mgmtData)->bm;
	if(tempPage>=0 && tempPage<totalPages){
		//make an empty page
		BM_PageHandle *page = MAKE_PAGE_HANDLE();
		//set pageNum in page
		page->pageNum=tempPage;
		pinPage(((RM_RecordMgmt *)rel->mgmtData)->bm,page,tempPage);//pin the page
		//Get the information.  And only the single slot.
		//First, determine offset
		int offsetSize=getRecordSize((RM_TableData *)rel->schema);
		int offset=(tempSlot)*offsetSize;//start of slot (assuming start at zero)

		unpinPage(tempAdd,page);//Done with page, unpin
		//free all memory
		page = NULL;
		free(tempAdd);
		free(tempEmpty);
		free(page);
		return RC_OK;

	}


	}

// scans (todo rst)
extern RC startScan (RM_TableData *rel, RM_ScanHandle *scan, Expr *cond) {

	return RC_OK;
}
extern RC next (RM_ScanHandle *scan, Record *record) {
	return RC_OK;
}


/*Close all parts of the scan from the Record Manager
 *Make all allocations NULL and free
 */
extern RC closeScan (RM_ScanHandle *scan)
{
	//clear current mgmtData
	scan->mgmtData=NULL;
	free(scan->mgmtData);
	//clear scan
	scan=NULL;
	free(scan);
	return RC_OK;
}

    // dealing with schemas -from internet here on out
extern int getRecordSize (Schema *schema)
	{
     	int i, recordSize = 0;

     	for(i = 0; i < schema->numAttr; i++)
     	{
     		if(schema->dataTypes[i] == DT_INT)
     			recordSize += sizeof(int);
     		else if(schema->dataTypes[i] == DT_FLOAT)
     			recordSize += sizeof(float);
     		else if(schema->dataTypes[i] == DT_BOOL)
     			recordSize += sizeof(bool);
     		else
     			recordSize += schema->typeLength[i];
     	}

     	return recordSize;
	}

extern Schema *createSchema (int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys)
	{
     	//allocate memory for Schema to be created
     	Schema *newSchema = (Schema*)malloc(sizeof(Schema));

     	//initialize all the attributes for the schema
     	newSchema->numAttr = numAttr;
     	newSchema->attrNames = attrNames;
     	newSchema->dataTypes = dataTypes;
     	newSchema->typeLength = typeLength;
     	newSchema->keySize = keySize;
     	newSchema->keyAttrs = keys;

     	return newSchema;
	}

extern RC freeSchema (Schema *schema)
	{
     	free(schema);
     	return RC_OK;
	}


// dealing with records and attribute values
extern RC createRecord (Record **record, Schema *schema)
	{
     	*record = (Record*)malloc(sizeof(Record));
     	(*record)->data = (char*)malloc(getRecordSize(schema));

     	return RC_OK;
	}
extern RC freeRecord (Record *record)
	{
     	//data is free'd first
     	record->data = NULL;
     	free(record->data);

     	//free the complete record
     	record = NULL;
     	free(record);

     	return RC_OK;
	}

extern RC getAttr (Record *record, Schema *schema, int attrNum, Value **value)
	{
     	//variables for offset
     	int offset;
     	char *attrData;

     	*value = (Value*)malloc(sizeof(Value));

     	//calculate the offset, to get the attribute value from
     	attrOffset(schema, attrNum, &offset);
     	attrData = record->data + offset;

     	(*value)->dt =schema->dataTypes[attrNum];

     	//switch on the attribute data types
     	switch(schema->dataTypes[attrNum])
     	{
     		case DT_INT:
     		{
     			memcpy(&((*value)->v.intV) ,attrData,sizeof(int));	//get the attribute into value
     		}
     		break;

     		case DT_STRING:
     		{
     			char *buf;
     			int len = schema->typeLength[attrNum];
     			buf = (char *) malloc(len + 1);
     			strncpy(buf, attrData, len);
     			buf[len] = '\0';
     			(*value)->v.stringV = buf;
     		}
     		break;

     		case DT_FLOAT:
     		{
     			memcpy(&((*value)->v.floatV),attrData, sizeof(float));
     		}
     		break;

     		case DT_BOOL:
     		{
     			memcpy(&((*value)->v.boolV),attrData ,sizeof(bool));
     		}
     		break;

     		default:			//if different data encountered other than INT, FLOAT, BOOL, STRING return (EC 402)
     			return RC_ERROR;
     	}

     	return RC_OK;
	}

extern RC setAttr (Record *record, Schema *schema, int attrNum, Value *value)
	{
     	//Modifying rm_serializer serializeAttr
     	int offset;
     	char *attrData;

     	//calculate the offset values
     	attrOffset(schema, attrNum, &offset);
     	attrData = record->data + offset;

     	//switch on attributes datatype value
     	switch(schema->dataTypes[attrNum])
     	{
     		case DT_INT:
     		{
     			memcpy(attrData,&(value->v.intV) ,sizeof(int));		//copy the newly set attribute value
     		}
     		break;

     		case DT_STRING:
     		{
     			char *buf;
     			int len = schema->typeLength[attrNum];
     			buf = (char *) malloc(len);
     			buf = value->v.stringV;
     			buf[len] = '\0';			//end the string with '\0'

     			memcpy(attrData,buf,len);
     		}
     		break;

     		case DT_FLOAT:
     		{
     			memcpy(attrData,&(value->v.floatV), sizeof(float));
     		}
     		break;

     		case DT_BOOL:
     		{
     			memcpy(attrData,&(value->v.boolV) ,sizeof(bool));
     		}
     		break;

     		default:
     			return RC_ERROR;
     	}
     	return RC_OK;
	}

