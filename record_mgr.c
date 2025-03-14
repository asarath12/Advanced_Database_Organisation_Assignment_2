//
// Created by Rebecca Sue Thomson on 3/9/25.
//


#include "dberror.h"
#include "expr.h"
#include "tables.h"

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
// Bookkeeping for scans - included by rst
typedef struct RM_ScanHandle
{
    RM_TableData *rel;
    void *mgmtData;
} RM_ScanHandle;

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
	if(access(name,F_OK)!= -1)
	{
		return RC_TABLE_ALREADY_EXISTS;
	}

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

	//Deserialzing the Schema gives us the Relation information (i.e. Schema info)
	rel->schema = deserializeSchema(page->data);

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

/* This function checks that the record that will be used matches the given
 * format.  It returns an error if the provided record fails.
 */
RC checkRecord (RM_TableData *rel, Record *record);
//todo with imported schema, check that record matches datatypes, formats, etc.
//itterate through schema, error checking that record matches format

//check that overall length of record is correct.  Add nulls if short. (overwrite of existing
//record could produce future errors otherwise)
    int tempLenWanted=getRecordLength(&rel);
    int tempLenHave=sizeof(&record);
    if (tempLenWanted < tempLenHave){return RC_RM_ERROR;}
    else if (tempLenWanted == tempLenHave){return RC_OK;}
    else {return RC_RM_ERROR;} //todo add nulls here later.





// handling records in a table
extern RC insertRecord (RM_TableData *rel, Record *record);
    {

    //using checkRecord to check Record is valid.
    if(checkRecord(rel, record) != RC_OK)
      {return RC_RM_RECORD_INVALID;}
    //verify that *result is set before continuing
    if(attrOffset (Schema *schema, int attrNum, int *result)==RC_OK){
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
    }
    return RC_OK;
    }
extern RC deleteRecord (RM_TableData *rel, RID id);
//todo go to record on page.
//Get page number of record
    tempPage=id.page;
//Get slot number of record
    tempSlot=id.slot;
//create temp record object
    Record ;

//open page into buffer. (Check if already open?)pin page
    readBlock(tempPage, SM_FileHandle *fHandle, SM_PageHandle memPage)
//write null over record.
    tempRecordlength=getRecordLength(rel);

//add empty slot into tombstone?

//un-pin page.  Leave in buffer for buffer manager to handle.


extern RC updateRecord (RM_TableData *rel, Record *record);
//todo use checkRecord to check that Record is OK.
    if(checkRecord(rel, record) != RC_OK)
        {return RC_RM_UNKOWN_DATATYPE;}
//Get page number of record
    tempPage=record->id.page;
//Get slot number of record
    tempSlot=record->id.slot;
//open page into buffer. (Check if already open?)pin page

//write new record over old.

//un-pin page.  Leave in buffer for buffer manager to handle.

extern RC getRecord (RM_TableData *rel, RID id, Record *record);
    {//Get page number of record
     tempPage=record->id.page;
      //Get slot number of record
     tempSlot=record->id.slot;
     if (tempPage>0 && tempPage<totalPages){
       BM_PageHandle *page=MAKE_PAGE_HANDLE();
       //Check that record is available.

       //Serialize updated record:
       char *new_record = serializeRecord(record, rel->schema);
        //pin page
       pinPage(((RM_TableData *)rel->mgmtData)->bm, page, tempPage);
       //setting new record.  First, determine offset
       int offsetSize=getRecordLength(rel);
       int offset=(tempSlot)*offsetSize;//start of slot (assuming start at zero)
       memset(page->data, 0, strlength(page->data));
       char* target = //todo how to find physical address of memory in buffer;
       char* target_address = target + offset
       memcpy(target_address, new_record, strlength(new_record));

       }
     //open page into buffer. (Check if already open?)pin page

//set pointer in pageframe in buffer to start of slot
// (get length of record, determine offset, and locate slot)
//read record

//set *record

//un-pin page.  Leave in buffer for buffer manager to handle.

// scans (todo rst)
extern RC startScan (RM_TableData *rel, RM_ScanHandle *scan, Expr *cond);
extern RC next (RM_ScanHandle *scan, Record *record);

/*Close all parts of the scan from the Record Manager
 *Make all allocations NULL and free
 */
extern RC closeScan (RM_ScanHandle *scan);
    //clear current mgmtData link to current Record
    ((RM_ScanMgmt *)scan->mgmtData)->currentRecord=NULL;
    free(((RM_ScanMgmt *)scan->mgmtData)->currentRecord);
    //clear mgmtData
    scan->mgmtData=NULL;
    free(scan->mgmtData);
    //clear scan
    scan=NULL;
    free(scan);
    return RC_OK;


    // dealing with schemas -from internet here on out
extern int getRecordSize (Schema *schema);
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

extern Schema *createSchema (int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys);
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

extern RC freeSchema (Schema *schema);
	{
     	free(schema);
     	return RC_OK;
	}


// dealing with records and attribute values
extern RC createRecord (Record **record, Schema *schema);
	{
     	*record = (Record*)malloc(sizeof(Record));
     	(*record)->data = (char*)malloc(getRecordSize(schema));

     	return RC_OK;
	}
extern RC freeRecord (Record *record);
	{
     	//data is free'd first
     	record->data = NULL;
     	free(record->data);

     	//free the complete record
     	record = NULL;
     	free(record);

     	return RC_OK;
	}

extern RC getAttr (Record *record, Schema *schema, int attrNum, Value **value);
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
     			return RC_RM_NO_DESERIALIZER_FOR_THIS_DATATYPE;
     	}

     	return RC_OK;
	}

extern RC setAttr (Record *record, Schema *schema, int attrNum, Value *value);
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
     			return RC_RM_NO_DESERIALIZER_FOR_THIS_DATATYPE;
     	}
     	return RC_OK;
	}
        
