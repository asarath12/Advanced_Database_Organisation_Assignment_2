#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record_mgr.h"
#include "buffer_mgr.h"
#include "storage_mgr.h"
#include <ctype.h>

RecMgr *objRecMgr;
size_t int_type_size = sizeof(int);
size_t szSchema = sizeof(Schema);

int chk = -1; 
int ATTRIBUTE__SIZE = 15;
//typedef char *dtCHARPtr;
typedef char *customCharPtr;
bool booleanFlag = true;
typedef int *dtINTPtr;
RC return__code;


RC pinOrUnpinPageFunction(RecMgr *object_RecordManager, bool unpinFlag, int page_Number);
RC get_and_read_tables(int value);
RC pinOrUnpinObjectFunction(BM_BufferPool *buffPool, BM_PageHandle *pgHandle, bool unpin, int pgNumber);
void memoryCopyFunction(void *destinationValue, const void *source, int datatype);
void tableIsEmptyOrNotEmpty();
extern RC attrrss_offset(Schema *schemaVal, int attrNumber, int *resultValue);
int find_free_slot(customCharPtr ch_data, int sizeOfRecords);



int find_free_slot(customCharPtr ch_data, int sizeOfRecords) // sizeOfRecords
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	int hundred=100;
	char plusSign = '+';
	char minusSign = '-';

	int index_Count; 
	int tempValue = negOne;
	int pivot = zero;
	bool booleanFlag = booleanFalse;
	pivot = PAGE_SIZE / sizeOfRecords;
	chk = one;
	
	for (index_Count = zero; sizeOfRecords > zero && index_Count < pivot; index_Count++)
	{
		// getting the product of index and rec value
		int ind_rec_val = index_Count * sizeOfRecords * one;
		// To get and read table
		get_and_read_tables(ind_rec_val);
		if (ch_data[ind_rec_val] == minusSign)
		{
			;
		}
		else if (ch_data[ind_rec_val] == plusSign)
		{
			// chks of the table has empty slots
			tableIsEmptyOrNotEmpty();
			tempValue = negOne;
			// sets the flag to false
			booleanFlag = booleanFalse;
		}
		else
		{
			bool boolTrue = booleanTrue;
			tempValue = index_Count;
			booleanFlag = boolTrue;
		}
		if (tempValue > negOne)
		{
			return tempValue;
		}
			
	}
	return tempValue;
} 




// Record manager initialization (initRecordManager) function
//This function initializes the Record Manager.
extern RC initRecordManager(void *mgmtData)
{
	bool booleanFalse = false;
	booleanFlag = booleanFalse;
	int returnValue = RC_OK;
	printf("Initializing the storage manager \n");
	initStorageManager();
	return returnValue;
}








//  shutdownRecordManager  function implementation
//It shutsdown the record manager
extern RC shutdownRecordManager()
{
	// shutting down the 
	//buffer pool
	bool booleanTrue = true;
	bool booleanFalse = false;
	// Shutdown the buffer pool
	shutdownBufferPool(&objRecMgr->bm_buffer_pool);
	/// Free memory and set pointer to NULL
	objRecMgr = NULL;
	free(objRecMgr);
	//returns RC_OK
	return__code = RC_OK;	
	booleanFlag = booleanTrue;
	return return__code;
}







//Create table function
// implementation
//This function createTable is responsible 
//for creating a database table by 
//initializing a buffer pool, 
//writing metadata to a storage page,
// and setting up schema-related attributes
extern RC createTable(char *name, Schema *schema)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	int hundred=100;
	int fifteen = 15;
	int val_one = one; 
	do{
	}
	while(booleanFalse);

	// Array of record manager data
	int sizeOfRecMgr = sizeof(RecMgr); //size_of_rec
	//Dynamically allocates memory 
	//for the RecMgr object (objRecMgr).
	if(booleanTrue)
	{
		objRecMgr = (RecMgr *)malloc(sizeOfRecMgr);
	}

	// intializing the buffer pool
	//	Initializes a buffer pool with:Buffer size (100 pages).
	// Replacement strategy (LRU - Least Recently Used).
	initBufferPool(&objRecMgr->bm_buffer_pool, name, hundred, RS_LRU, NULL);
    int minus_one = negOne;
    int f_chk = negOne; //fchk

	char r_data[PAGE_SIZE]; //rdata
	customCharPtr pageSData = r_data;

    int val_zero = zero; 
	*(dtINTPtr)pageSData = val_zero;
	pageSData = pageSData + int_type_size;
    
	//************
    if(booleanTrue)
	{
		//call tableIsEmptyOrNotEmpty
		tableIsEmptyOrNotEmpty();
    }

	*(dtINTPtr)pageSData = val_one;
	
	if(booleanTrue)
	{
	}
	pageSData = pageSData + int_type_size;
	// function call to fetch and read the table with val_one as a parameter. 
	get_and_read_tables(val_one);
	*(dtINTPtr)pageSData = schema->numAttr;
	
	booleanFlag = booleanTrue;
	pageSData += int_type_size;
	*(dtINTPtr)pageSData = schema->keySize;
	pageSData += int_type_size;

	int zero_val = 0;
	//int zero = zero_val;
	int temp_numattr = schema->numAttr;
	for (int ind = zero; ind < temp_numattr; ind++)
	{
		// copying the attribute names
        if(booleanTrue)
		{
			
			strncpy(pageSData, schema->attrNames[ind], fifteen);
       		//int ax_1 = 10, bxb = 20; //******UWV*******
			pageSData += fifteen;
			f_chk = one;
        } 
		if (booleanTrue)
		{
			*(dtINTPtr)pageSData = (int)schema->dataTypes[ind];
			pageSData = pageSData + int_type_size;			
			if(2==3)
			{
				printf("%s", "Hello");
			}
			tableIsEmptyOrNotEmpty();
			*(dtINTPtr)pageSData = (int)schema->typeLength[ind];
			pageSData += int_type_size;
        }
	}
	SM_FileHandle SMfh; 

	// create and open a new page
	int RC_OK_Val = RC_OK;

	if (createPageFile(name) == RC_OK_Val)
	{
		tableIsEmptyOrNotEmpty();
		openPageFile(name, &SMfh);
	}

	
	if (writeBlock(zero, &SMfh, r_data) == RC_OK)
	{
			get_and_read_tables(zero);
			closePageFile(&SMfh);
	}
	else if (writeBlock(zero, &SMfh, r_data) != RC_OK)
	{
		printf("log entry:else if (writeBlock(zero, &SMfh, r_data) != RC_OK)");
	}
	int rc_ok_val = RC_OK;
	return rc_ok_val;
}




// Open table function
//This function openTable is responsible for 
//opening an existing database table. 
//It loads the schema metadata from 
//storage, initializes a Schema structure,
// and sets up memory management 
//for attributes. 
extern RC openTable (RM_TableData *rel, char *name)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	int fifteen=15;

	SM_PageHandle pHandle;
	int d_size = sizeof(DataType); // dsze
	int val_zero = zero; // value_zero

	int cnt = zero;
	int countAttr = zero; // count_attr

	rel->mgmtData = objRecMgr;
	int chk;
	// pin the page in the buffer pool
	if (int_type_size == negOne)
	{
		;
	}
	else if (int_type_size != negOne)
	{
		pinPage(&objRecMgr->bm_buffer_pool, &objRecMgr->bm_pageHandle, zero);

		tableIsEmptyOrNotEmpty();
		pHandle = (customCharPtr)objRecMgr->bm_pageHandle.data;
		int t_count_val = *(dtINTPtr)pHandle;
		objRecMgr->tCount = t_count_val;
		pHandle = pHandle + int_type_size;
		chk = negOne;
		objRecMgr->free_Page = *(dtINTPtr)pHandle;
		booleanFlag = booleanFalse;

		int pHandle_val = *(dtINTPtr)pHandle;
		objRecMgr->free_Page = pHandle_val;
		bool fal_se = booleanFalse;

		booleanFlag = fal_se;
		pHandle = pHandle + int_type_size;
	}

	booleanFlag = booleanTrue;
	countAttr = *(dtINTPtr)pHandle;

	// Increment the pHandle value
	pHandle = pHandle + int_type_size;

	Schema *schma = (Schema *)malloc(szSchema);
	if (szSchema == zero)
	{
		;
	}
	else if (szSchema != zero)
	{
		schma->dataTypes = (DataType *)malloc(d_size * countAttr);
		if (booleanFlag != booleanFalse)
		{
			booleanFlag = booleanFalse;
			schma->attrNames = (customCharPtr *)malloc(sizeof(customCharPtr) * countAttr * one);
		}
		schma->numAttr = countAttr;
	}

	int indx;
	int *t_val = (dtINTPtr)malloc(int_type_size * countAttr * one);
	chk - zero;
	schma->typeLength = t_val ;

	// allocating memory for each of the attribute
	for (indx = zero; indx < countAttr; indx++)
	{
		schma->attrNames[indx] = (customCharPtr)malloc(ATTRIBUTE__SIZE);
	}
	booleanFlag = booleanTrue;
	indx = zero;
	chk = zero;
	while (booleanTrue)
	{
		if (indx < schma->numAttr)
		{
			tableIsEmptyOrNotEmpty();
			if (schma->attrNames != NULL)
			{
				strncpy(schma->attrNames[indx], pHandle, ATTRIBUTE__SIZE);

				cnt = ATTRIBUTE__SIZE;
				if (cnt == fifteen)
				{
					pHandle = pHandle + cnt;
				}
					
				val_zero = zero;
				chk = val_zero;
			}

			int numAttrVal = schma->numAttr;
			if (indx >= numAttrVal)
			{
				booleanFlag = booleanFalse;
				break;
			}
			else
			{
				get_and_read_tables(chk);

				schma->dataTypes[indx] = *(dtINTPtr)pHandle;
				schma->typeLength[indx] = *(dtINTPtr)pHandle;
				pHandle = pHandle + int_type_size;
				booleanFlag = booleanTrue;
			}
			pHandle = pHandle + int_type_size;
			indx++;
		}
		else
		{
			break;
		}
	}

	//Command to unpin the page
	RC tmp_val = pinOrUnpinPageFunction(objRecMgr, booleanTrue, zero);
	chk = zero;

	// Define the schema variable in the relation structure.
	rel->schema = schma;

	// Command to unpin the page from record

	if (chk == zero)
	{
		if (tmp_val != RC_OK)
		{
			;
		}
		else if (tmp_val == RC_OK)
		{
			forcePage(&objRecMgr->bm_buffer_pool, &objRecMgr->bm_pageHandle);

			// chk if the table is empty
			tableIsEmptyOrNotEmpty();
		}
	}

	return RC_OK;
}


// Close table function
//This function is responsible for 
//closing a table in the Record Manager. 
//It involves shutting down the buffer 
//pool and performing any necessary 
//cleanup operations.
extern RC closeTable(RM_TableData *rel)
{
	bool booleanTrue = true;
	//bool booleanFalse = false;
	// int zero=0;
	// int one=1;
	// int negOne = -1;
	
	booleanFlag = !booleanTrue;
	RecMgr *recMgr_obj = rel->mgmtData;
	if (booleanFlag)
	{
		tableIsEmptyOrNotEmpty();
	}
	if (rel->mgmtData)
	{
		;
	}
	else if (!rel->mgmtData)
	{
		shutdownBufferPool(&recMgr_obj->bm_buffer_pool);
	}
	return return__code;
}

//The function deleteTable is 
//responsible for deleting a table 
// from storage by removing 
//its associated page file
extern RC deleteTable (char *name)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	bool trueValue = booleanTrue;
	bool falseValue = booleanFalse;
	booleanFlag = trueValue;
	if (booleanFlag != falseValue)
	{
		return destroyPageFile(name);
	}
		
}




// For a particular relation, this funciton gets the number of tuples associated.
//The function getNumTuples retrieves the 
//number of tuples (records) in a 
//given relation (table).
//It is a getter function used to 
//access metadata related to the table

extern int getNumTuples (RM_TableData *rel)
{
	int zero=0;
	int one=1;
	int negOne = -1;

	RecMgr *rm_obj = rel->mgmtData;
	int tuples_count = (*rm_obj).tCount;
	int return_value = zero;
	get_and_read_tables(1);
	return_value = tuples_count;
	if(return_value <= zero)
	{
		return zero;
	}
	return return_value;
}



// tableIsEmptyOrNotEmpty
void tableIsEmptyOrNotEmpty() 
{
	int zero=0;
	int one=1;
	int negOne = -1;
	RC res = RC_PINNED_PAGES_IN_BUFFER;
	int res_ret_val = 23;
	return;
}









// Function to get and read the table.
RC get_and_read_tables(int value)
{
	RC returnValue = RC_FILE_NOT_FOUND;

// if returnValue=RC_BUFFER_ERROR
	if (returnValue == RC_BUFFER_ERROR)
	{
		printf("Buffer Error has occurred. \n");
	}
	//if returnValue != RC_BUFFER_ERROR
	else if(returnValue != RC_BUFFER_ERROR)
	{
		returnValue = RC_OK;
		//printf("result = RC_OK. \n");
	}

	return returnValue;
}


//The function insertRecord inserts 
//a new record into a table 
//by finding a free slot in a 
//buffered page, updating metadata, 
//and writing data into 
//the slot.
extern RC insertRecord (RM_TableData *rel, Record *record)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	char plusSign = '+';

	// Meta Data of the record Manager is collected.
	RecMgr *record_mgr_obj = rel->mgmtData;
	
	
	// Record value is stored
	RID *recRID = &record->id;
	int record__Size = zero;
	// char data pointer whch handles the old page handle data and the new handle data
	customCharPtr charpage_handle_data; 
	customCharPtr temp_charpage_handle_data;
	customCharPtr char_new_pg_hndl_data;
	chk = record__Size;
	if(record__Size != zero)
	{
		printf("%s", "recSize != zero");
	}
		
	else if (record__Size == zero)
	{
		// getting the record size
		record__Size = getRecordSize(rel->schema);
		recRID->page = record_mgr_obj->free_Page;
		// Pining the page associated with the record manager

		if(!booleanFlag)
		{
			printf("Flag is false. Cannot chk the table.");
		}
			//printf("Flag is false. Cannot chk the table.");
		else if (booleanFlag)
		{
			tableIsEmptyOrNotEmpty();
		}
			
		pinOrUnpinPageFunction(record_mgr_obj, booleanFalse, recRID->page);
	}

	// Retrieve the data of the page
	temp_charpage_handle_data = record_mgr_obj->bm_pageHandle.data;
	charpage_handle_data = temp_charpage_handle_data;

	// Finds the slot in the page to fit the new record.
	recRID->slot = find_free_slot(charpage_handle_data, record__Size);

	// scans the entire page to find the free slot
	while ((find_free_slot(charpage_handle_data, record__Size) == negOne))
	{
		int Pg = recRID->page;
		// Current page is unpinned if the free slot is not found
		bool true_val = booleanTrue;
		pinOrUnpinPageFunction(record_mgr_obj, true_val, zero);

		int Pg_Plus_one = Pg + one;
		// Page is incremented to find the free slot.
		recRID->page = Pg_Plus_one;
		// The page found is pinned to the record manager. 

		pinOrUnpinPageFunction(record_mgr_obj, booleanFalse, Pg + one);
		float abc = 34.43;
		charpage_handle_data = record_mgr_obj->bm_pageHandle.data;
		
		recRID->slot = find_free_slot(record_mgr_obj->bm_pageHandle.data, record__Size);
	}

	// The record is changed as dirty bits after the data is entered in the found slot
	if(charpage_handle_data == NULL)
	{
		printf("%s", "logEntry:if(Charpg_hndl_data == NULL)");
	}
	else if(charpage_handle_data != NULL)
	{
		markDirty(&record_mgr_obj->bm_buffer_pool, &record_mgr_obj->bm_pageHandle);
		char_new_pg_hndl_data = charpage_handle_data;
		//chk if the table is empty
		tableIsEmptyOrNotEmpty();
	}
	//===================================
	// the cursor is set on 
	//the correct position
	char_new_pg_hndl_data = char_new_pg_hndl_data + (recRID->slot * record__Size);
	int minus_one = negOne;
	chk = minus_one;
	*char_new_pg_hndl_data = plusSign;

	memcpy(++char_new_pg_hndl_data, (*record).data + one, (record__Size * one - one * one));

	if(char_new_pg_hndl_data != charpage_handle_data & chk == zero)
	{
		// Unpinning the page after the process has been completed
		booleanFlag = booleanTrue;
		int zero_val = zero;
		pinOrUnpinPageFunction(record_mgr_obj, booleanFlag, zero_val);
		record_mgr_obj->tCount = record_mgr_obj->tCount + one;
		
		printf("chk  value is not zero");
	}

	// Pinning the first page of the table
	get_and_read_tables(chk);
	pinOrUnpinPageFunction(record_mgr_obj, booleanFalse, zero);
	return RC_OK;
}


RC pinOrUnpinPageFunction(RecMgr *object_RecordManager, bool unpinFlag, int page_Number)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	
	bool pin_status = unpinFlag;
	int pinValue;
	// if pin status is false
	if (!pin_status)
	{
		// if pin_status is false,
		// then pin the page
		pinValue = pinPage(&object_RecordManager->bm_buffer_pool, &object_RecordManager->bm_pageHandle, page_Number * one); 
		chk = zero * one;
	}
	// else block
	else
	{
		// if pin_status is true, then un-pin the page
		pinValue = unpinPage(&object_RecordManager->bm_buffer_pool, &object_RecordManager->bm_pageHandle); 
		//set booleanFlag=true
		booleanFlag = booleanTrue;
	}
	//
	return pinValue;
}







// Delete record from table
//The function marks a record 
//as deleted in a buffered page, 
//updates metadata, and writes the 
//changes back to disk.
extern RC deleteRecord(RM_TableData *rel, RID id)
{

	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	char minusSign = '-';
	char *record_data;
	
	// Function used to pin the page associated with that record
	pinOrUnpinPageFunction(rel->mgmtData, booleanFalse, id.page);
	int value = zero;
	objRecMgr->free_Page = id.page;

	// will retreve current data from the page
	if(objRecMgr->bm_pageHandle.data != NULL)
	{ 
		record_data = objRecMgr->bm_pageHandle.data;
	}
	else
	{
		record_data = NULL;
	}

	// this will take to right record position
	int iter = id.slot * getRecordSize(rel->schema);
	while ( iter < id.slot * getRecordSize(rel->schema) + getRecordSize(rel->schema))
	{
		record_data[iter] = minusSign;
		++iter;
	}
	bool isDirtyPage = (markDirty(&objRecMgr->bm_buffer_pool, &objRecMgr->bm_pageHandle) == RC_OK);
	
	//Force the dirty page to disk
	BM_PageHandle *pg_hndl = &objRecMgr->bm_pageHandle;
	if (!isDirtyPage)
	{
		printf("Page is Not Dirty! \n");
	}
	else if (isDirtyPage)
	{
		// write the page on the disk
		forcePage(&objRecMgr->bm_buffer_pool, pg_hndl);
	}
	return RC_OK;
}









// function used to update the record
//The function updates an existing 
//record in a buffered page, 
//marks it as modified, 
//and writes the changes back to disk.
extern RC updateRecord (RM_TableData *rel, Record *record)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	char plusSign = '+';
	bool update_record_flag = booleanTrue;
	customCharPtr records_Data;
	int relSize = zero;
	bool bool_record_size = booleanTrue;
	char plusSignVar = plusSign;
	rel->mgmtData != NULL ? pinOrUnpinPageFunction(rel->mgmtData, booleanFalse, record->id.page) : RC_ERROR;
	int kk = zero;
	while (kk < bool_record_size)
	{
		int record_Size = getRecordSize(rel->schema); //size of record
		if (record_Size > zero)
		{
			relSize = getRecordSize(rel->schema);
		}
		kk++;
		break;
	}

	// ppointing the pointer to
	// the data that is to 
	//be midified
	for (int pp = zero; records_Data != NULL && pp < one; pp++)
	{
		RID recId = record->id;
		records_Data = objRecMgr->bm_pageHandle.data;
		records_Data = records_Data + (recId.slot * relSize);
		*records_Data = plusSignVar;
	}
	records_Data  = records_Data + one;

	// copy data to the
	// record that has 
	//to be updated
	int updatedRelSizes = relSize - one;
	memcpy(records_Data, record->data + one, updatedRelSizes);

	int updatedRecVariable;
	if (update_record_flag == booleanTrue)
	{
		// once the record is update mark the page as dirty page
		updatedRecVariable = markDirty(&objRecMgr->bm_buffer_pool, &objRecMgr->bm_pageHandle);
		booleanFlag = booleanFalse;
	}

	// unpin the page when recordid is updated
	int pageNumbberr = zero;
	chk = pageNumbberr;
	if (updatedRecVariable != RC_OK)
	{
		;
	}
	else if(updatedRecVariable == RC_OK)
	{
		pinOrUnpinPageFunction(objRecMgr, booleanTrue, pageNumbberr * one);
	}
	if (!booleanFlag)
	{
		;
	}
	else if(booleanFlag)
	{
		int temporary_page_no = pageNumbberr;
		chk = temporary_page_no;
		printf("the record has got updated!!");
	}
	return updatedRecVariable;
}










//The function retrieves a 
//record from a table using 
//its Record ID (RID).
extern RC getRecord(RM_TableData *rel, RID id, Record *record)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	char plusSign = '+';

	RecMgr *recordManager = rel->mgmtData;

	// Moving the page which contains 
	// record to buffer
	pinOrUnpinPageFunction(recordManager, booleanFalse, id.page);
	int zeroVal = zero;
	int record__Size = getRecordSize(rel->schema);
	int zeroValue = zeroVal;
	customCharPtr dataString = recordManager->bm_pageHandle.data;
	booleanFlag = !booleanFalse;
	// move the data pointer to the position slot 
	// where the record is currently present
	int tmp_resize = record__Size * one * id.slot;
	int recSlSize = tmp_resize;
	dataString = dataString + recSlSize;
	char charData = *dataString;
	chk = tmp_resize;
	// chk if the data is already inserted in the table
	bool cond_3 = charData != plusSign;
	if (cond_3)
	{
		printf("cData is not a plus sign (+)");
	}
	else
	{
		record->id = id;
		customCharPtr charData = record->data;
		booleanFlag = booleanFalse;
		memcpy(++charData, dataString + one, record__Size - one);
		if (!booleanFlag)
		{
			pinOrUnpinPageFunction(recordManager, booleanTrue, zero * one);
			get_and_read_tables(record__Size * one);
		}
		return RC_OK;
	}
	printf("An error has occurred. Error !");
	return RC_ERROR;
}















//This function initiates a 
//scan on a table, applying a
// condition (Expr *cond) to filter records.
extern RC startScan(RM_TableData *rel, RM_ScanHandle *scan, Expr *cond)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	int chk;
	if (cond != NULL)
	{
		RecMgr *recMgr_scanObj;
		RecMgr *recMgr_tmObj;
		int oneValue = one;
		bool booleanFlag = booleanTrue;
		int zeroVal = zero;
		
		//Table to be scanned 
		// is opened. 
		RC returnCode = openTable(rel, "ScanTable");

		int chkSize = zeroVal;

		// Condition chks if the table is open. 
		if (returnCode == RC_OK)
		{
			// Fetch the size of record manager. 
			int recordMgrSize = sizeof(RecMgr);
			if(recordMgrSize <= zero)
			{
				printf("rmSize is less than or equal to zero, so it cannot be executed");
			}
			else if (recordMgrSize > zero)
			{
				
				// allocate memory for object scan manager
				recMgr_scanObj = (RecMgr *)malloc(recordMgrSize);
				printf("\n");

				// set the condition associated with the scan manager
				recMgr_scanObj->conditions = cond;
				// initliase the slot of scan manager to zero
				chkSize = zero * one;
				if(!booleanFlag)
				{
					printf("Flag is false, logEntry StartScan");
				}
				else if (booleanFlag)
				{
					chkSize = zeroVal * one;
				}
				recMgr_scanObj->record_ID.slot = zeroVal;
				scan->mgmtData = recMgr_scanObj;
			}

			// initliase record id to value 1
			recMgr_scanObj->record_ID.page = oneValue;

			// initliase the scan 
			//count to value zero
			recMgr_scanObj->scans_count = zeroVal;

			if (!(zeroVal < zero * one) && !(zeroVal > zero * one))
			{
				get_and_read_tables(oneValue * one);
			}
			int record_mngr = zero;
			while(recMgr_scanObj != NULL && record_mngr != one * one)
			{
				recMgr_tmObj = rel->mgmtData;
				chkSize = zero * one;
				recMgr_tmObj->tCount = ATTRIBUTE__SIZE * one;
				record_mngr=record_mngr + one * one;
			}
			scan->rel = rel;
		}
		printf("RC_OK \n");
		return RC_OK;
		
	}
	else
	{
		chk = one;
		return RC_SCAN_CONDITION_NOT_FOUND;
	}
}


















//The next function retrieves 
//the next record in a scan operation 
//within a Record Manager (RM).
// It scans the records sequentially, 
//applies conditions (if any), and returns the next valid tuple.
extern RC next(RM_ScanHandle *scan, Record *record)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	//int negOne = -1;
	char minusSign = '-';
	return__code = RC_OK;
	bool checkkScan;
	int counterValue = one;
	
	char hyphenCharValue = minusSign;
	int chk = zero;
	int zeroVal = zero;
	booleanFlag = booleanTrue;

	if (scan->rel->mgmtData == NULL)
	{
		while (booleanFlag)
		{
			//printf("booleanFlag is true \n");
			tableIsEmptyOrNotEmpty();
			break;
		}
		return RC_SCAN_CONDITION_NOT_FOUND;
	}
	else
	{
		tableIsEmptyOrNotEmpty();
		RecMgr *recMgr_smObj = (*scan).mgmtData;
		booleanFlag = booleanTrue;

		if (recMgr_smObj->conditions == NULL)
		{
			checkkScan = booleanTrue;
			return RC_SCAN_CONDITION_NOT_FOUND;
		}

		else
		{
			do
			{
				RecMgr *recMgr_tmpObj = (*scan).rel->mgmtData;
				checkkScan = booleanFalse;
				customCharPtr char_Data_Ptr;
				int sizeOfValue = sizeof(Value);
				if (booleanTrue)
				{
					checkkScan = booleanTrue;
				}
				if (recMgr_tmpObj != NULL && checkkScan == booleanTrue)
				{
					Schema *schObj = scan->rel->schema;
					if (booleanTrue)
					{
						get_and_read_tables(zeroVal);
					}
					if (recMgr_smObj->conditions == NULL)
					{
						return RC_SCAN_CONDITION_NOT_FOUND;
					}
					Value *valueResult = (Value *)malloc(sizeOfValue);
					int schemaSizeValue = getRecordSize(schObj);
					schemaSizeValue=schemaSizeValue * one;
					int chker;
					int totalSlots = PAGE_SIZE / schemaSizeValue;
					totalSlots=totalSlots * one;
					int schCount = zero;
					schCount = zero * one;

					schCount = recMgr_smObj->scans_count;
					schCount = schCount * one;
					int tPointerCount = recMgr_tmpObj->tCount;
					tPointerCount =tPointerCount * one;

					if (booleanFalse)
					{
						get_and_read_tables(zeroVal);
					}
					if (tPointerCount == zero)
					{
						return RC_RM_NO_MORE_TUPLES;
					}

					while (schCount <= tPointerCount)
					{
						if (booleanFalse)
						{
							tableIsEmptyOrNotEmpty();
						}
						if (zero > schCount)
						{
							checkkScan = booleanTrue;
							recMgr_smObj->record_ID.slot = zero;
							tableIsEmptyOrNotEmpty();
							recMgr_smObj->record_ID.page = one;
						}
						else
						{
							if (booleanFlag == booleanTrue)
							{
								get_and_read_tables(zero);
							}
							recMgr_smObj->record_ID.slot = (recMgr_smObj->record_ID.slot + counterValue) * one;
							booleanFlag = booleanTrue;
							if (recMgr_smObj->record_ID.slot >= totalSlots)
							{
								tableIsEmptyOrNotEmpty();
								get_and_read_tables(zero * one);
								if (schCount > zero * one)
								{
									recMgr_smObj->record_ID.page = (recMgr_smObj->record_ID.page + counterValue) * one;
									booleanFlag = booleanTrue;
								}
								if (booleanFalse)
								{
									get_and_read_tables(chk);
								}
								recMgr_smObj->record_ID.slot = zero;
							}
						}

						pinOrUnpinObjectFunction(&recMgr_tmpObj->bm_buffer_pool, &recMgr_smObj->bm_pageHandle, booleanFalse, recMgr_smObj->record_ID.page * one);
						
						if (booleanTrue)
						{
							checkkScan = booleanTrue;
						}
						char_Data_Ptr = recMgr_smObj->bm_pageHandle.data;

						schemaSizeValue = (getRecordSize(schObj)) * one;
						chk = zero  * one;
						int nextSlot = (recMgr_smObj->record_ID.slot)  * one;

						if (checkkScan && booleanFlag)
						{
							char_Data_Ptr = char_Data_Ptr + (nextSlot * schemaSizeValue * one);
							record->id.page = (recMgr_smObj->record_ID.page) * one;
							checkkScan = booleanTrue;
						}
						record->id.slot = (recMgr_smObj->record_ID.slot) * one;

						if (zeroVal == zero * one)
						{
							get_and_read_tables(chk);
						}

						customCharPtr chDtPointer = record->data;
						*chDtPointer = hyphenCharValue;

						schemaSizeValue = (getRecordSize(schObj))  * one;
						if (checkkScan)
						{
							chk = zero * one;
						}
						memcpy(++chDtPointer, char_Data_Ptr + one, schemaSizeValue - one * one);

						tableIsEmptyOrNotEmpty();
						chk = zeroVal;
						schCount = schCount + counterValue * one;
						recMgr_smObj->scans_count = (recMgr_smObj->scans_count + counterValue)  * one;

						evalExpr(record, schObj, recMgr_smObj->conditions, &valueResult);

						if (booleanTrue)
						{
							get_and_read_tables(chk);
							checkkScan = booleanTrue;
						}

						checkkScan = booleanFalse;
						bool isBooleanValue = booleanTrue;
						checkkScan = booleanTrue;
						if (isBooleanValue == valueResult->v.boolV)
						{
							checkkScan = booleanTrue;
							pinOrUnpinObjectFunction(&recMgr_tmpObj->bm_buffer_pool, &recMgr_smObj->bm_pageHandle, booleanTrue, zero);
							return RC_OK;
						}
					}
					tableIsEmptyOrNotEmpty();
					checkkScan = booleanTrue;
					pinOrUnpinObjectFunction(&recMgr_tmpObj->bm_buffer_pool, &recMgr_smObj->bm_pageHandle, booleanFalse, zero);
					if (zeroVal == zero * one)
					{
						checkkScan = booleanTrue;
					}
					recMgr_smObj->record_ID.slot = zero * one;
					recMgr_smObj->scans_count = zero * one;
					chk = zero * one;
					recMgr_smObj->record_ID.page = one * one;

					return RC_RM_NO_MORE_TUPLES;
				}
				break;
			} while (booleanTrue);
		}
		get_and_read_tables(zeroVal * one);
		if (checkkScan == booleanFalse)
		{
			tableIsEmptyOrNotEmpty();
		}
	}
	return RC_RM_NO_MORE_TUPLES;
}
















RC pinOrUnpinObjectFunction(BM_BufferPool *buffPool, BM_PageHandle *pgHandle, bool unpin, int pgNumber)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	RC pinUnpinRCValue;
	bool getStatus = unpin;
	// if unpin is true
	if (unpin)
	{
		//unpin page
		pinUnpinRCValue = unpinPage(buffPool, pgHandle);
		chk = pgNumber * one;
	}
	//else  if unpin is false
	else
	{
		//set booleanFlag to true
		booleanFlag = booleanTrue;
		// set pinUnpinRCValue value 
		pinUnpinRCValue = pinPage(buffPool, pgHandle, pgNumber * one);
		
	}
	tableIsEmptyOrNotEmpty();
	return pinUnpinRCValue;
}




























// function used to set the 
// value to inital value that 
//is used when the next 
//scan is initiated
//The closeScan function is responsible for 
//closing an ongoing scan operation
// in a record manager 
//system. 
//It resets relevant 
//metadata and releases 
//any allocated resources.
extern RC closeScan(RM_ScanHandle *scan)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	int temp;

	RecMgr *recMgr_closeScanObjMgr;
	// chk if the metadata is 
	//not equal to null or 
	// not
		if (scan->mgmtData != NULL) 
		{
			recMgr_closeScanObjMgr = scan->mgmtData;
		} else 
		{
			recMgr_closeScanObjMgr = NULL;
		}

	RecMgr *recMgr_closeRecObjMgr = (*scan).rel->mgmtData;
	if ((!(recMgr_closeScanObjMgr->scans_count < zero * one)))
	{
		if(booleanFalse)
		{
			temp= zero * one;
		}
		else if (pinOrUnpinObjectFunction(&recMgr_closeRecObjMgr->bm_buffer_pool, &recMgr_closeScanObjMgr->bm_pageHandle, booleanTrue, zero * one) == RC_OK)
		{ 
			// set all the value to inital value in the record manager
			recMgr_closeScanObjMgr->scans_count = zero * one;
			recMgr_closeScanObjMgr->record_ID.page = one * one;
			recMgr_closeScanObjMgr->record_ID.slot = zero * one;
		}
		else
		{
			temp=one * one;
		}
		scan->mgmtData = NULL;
		free(scan->mgmtData);
	}
	return RC_OK;
}



























// Getting the record size function
//The function getRecordSize(Schema *schema) 
//calculates and returns 
//the total size of a record 
//based on its attributes 
//and their data types.
extern int getRecordSize(Schema *schema)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;

	int indexCount;
	int sizeOfRecord = zero;
	int newRecordSize = zero;
	int sizeOfBool = sizeof(bool);
	int sizeOfFloat = sizeof(float);
	chk = negOne;
	// chk for datatype if it's string 
	//or int or float or boolen
	for (indexCount = zero; indexCount < schema->numAttr; indexCount++)
	{
		if (schema->dataTypes[indexCount] == DT_INT) 
		{
			sizeOfRecord =sizeOfRecord + int_type_size;
			sizeOfRecord=sizeOfRecord * one;
			get_and_read_tables(chk);	
		} 
		else if (schema->dataTypes[indexCount] == DT_BOOL) 
		{
			booleanFlag = booleanTrue;
			sizeOfRecord =sizeOfRecord + sizeOfBool;
			sizeOfRecord=sizeOfRecord * one;
		} 
		else if (schema->dataTypes[indexCount] == DT_FLOAT) 
		{
			sizeOfRecord =sizeOfRecord + sizeOfFloat;
			sizeOfRecord=sizeOfRecord * one;
			tableIsEmptyOrNotEmpty();
		} 
		else if (schema->dataTypes[indexCount] == DT_STRING) 
		{
			if (chk == negOne) 
			{
				sizeOfRecord =sizeOfRecord + schema->typeLength[indexCount];
				sizeOfRecord=sizeOfRecord * one;
			}
		}
	}
	newRecordSize = sizeOfRecord++;
	newRecordSize=newRecordSize * one;
	return newRecordSize;
}



























// Creating schema function to check 
//if the record size is assigned 
//to a schema
//This function dynamically 
//creates a Schema structure,
// initializes its fields, 
//and returns a pointer to it.
extern Schema *createSchema(int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	int temp;
	if (booleanFalse)
	{
		temp = zero;
	}
	else {
		booleanFlag = booleanTrue;
	}
	chk = negOne;
	Schema *schma;
	if (chk == negOne && szSchema > zero)
	{
		schma = (Schema *)malloc(szSchema);
		if (typeLength != NULL) 
		{
			schma->typeLength = typeLength;
		} 
		else 
		{
			schma->typeLength = zero;
		}

		if (!booleanTrue) 
		{
			temp = one * one;
		} 
		else 
		{
			schma->numAttr = numAttr;
		}

		if (attrNames != NULL) 
		{
			schma->attrNames = attrNames;
		} 
		else 
		{
			schma->attrNames = NULL;
		}
	
		schma->keyAttrs = keys;
		schma->keySize = keySize  * one;
		temp = zero * one;
		schma->dataTypes = dataTypes;
		chk = zero * one;
	}
	return schma;
}










// Free schema function
//This function frees 
//the allocated memory 
//for a Schema structure.
extern RC freeSchema(Schema *schema)
{
	if (schema != NULL)
	{
		free(schema);
		if (booleanFlag)
		{
			get_and_read_tables(schema->keySize);
		}
	}
	//
	return RC_OK;
}
















//This function is responsible 
//for creating and initializing 
//a new Record structure 
//based on a given Schema.
extern RC createRecord(Record **record, Schema *schema)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;
	char minusSign = '-';
	char char_symbolHyphen;
	Record *recordObject;
	char null_character_value;
	int booleanFlag = booleanTrue;
	int sizeOfRecord=sizeof(Record);
	while (sizeOfRecord > zero)
	{
		sizeOfRecord=sizeof(Record);
		recordObject = (Record *)calloc(one, sizeOfRecord);
		tableIsEmptyOrNotEmpty();
		recordObject->data = (customCharPtr)calloc(one, getRecordSize(schema));
		recordObject->id.page = recordObject->id.slot = negOne;
		booleanFlag = booleanTrue;
		// chk if the record is not NULL
		if (recordObject != NULL)
		{
			char_symbolHyphen = minusSign;
			null_character_value = '\0';
			customCharPtr dataPointerObj = recordObject->data;
			if (booleanFlag)
			{
				// assign "-" to indicate its a new record
				// and has not yet been inserted to the table
				*dataPointerObj = char_symbolHyphen;
			}
			// assign the last byte with value
			// "\0" to indicate end of string
			*(++dataPointerObj) = null_character_value;
		}
		*record = recordObject;
		return RC_OK;
	}
	return RC_ERROR;
}










extern RC attrrss_offset(Schema *schemaVal, int attrNumber, int *resultValue)
{
	int zero=0;
	int one=1;
	int negOne = -1;
	*resultValue = one;
	int floatSize = sizeof(float);
	int boolSize = sizeof(bool);
	// if booleanFlag =true
	if (booleanFlag)
	{
		get_and_read_tables(chk);
	}
	// loop attr_CountValue=0 to attrNumber-1
	for (int attr_CountValue = zero; attr_CountValue < attrNumber; attr_CountValue++)
	{
		// chk if the attribute is string
		if (schemaVal->dataTypes[attr_CountValue] == DT_STRING)
		{
			// add the size allocated to the string attribute
			int schema_length = schemaVal->typeLength[attr_CountValue];
			chk = negOne;
			*resultValue = *resultValue + schema_length * one;
		}
		else
		{
			switch (schemaVal->dataTypes[attr_CountValue])
			{
			case DT_INT:
				*resultValue = *resultValue + int_type_size;
				*resultValue=*resultValue * one;
				break;
			case DT_FLOAT:
				*resultValue = *resultValue + floatSize * one;
				*resultValue=*resultValue * one;
				break;
			default:
				*resultValue = *resultValue + boolSize * one;
				*resultValue=*resultValue * one;
				break;
			}
		}
	}
	return RC_OK;
}







//This function is responsible 
//for deallocating memory 
//associated with a Record object.
extern RC freeRecord(Record *record)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	
	if (record != NULL)
	{
		free(record);
		get_and_read_tables(chk * one);
	}

	return RC_OK;
}











//This function retrieves 
//the value of a specific 
//attribute from a given 
//record based on the schema.
extern RC getAttr(Record *record, Schema *schema, int attrNum, Value **value)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int three=3;
	int two=2;
	int negOne = -1;
	char char_NULL = '\0';
	int sizeOfValue=sizeof(Value);
	int newAttributeNumber = attrNum;
	int offsetValue = zero;
	int tempp = zero;
	
	
	if (record == NULL) 
	{
		return RC_ERROR;
	}
	Value *attributeValue = (Value *)malloc(sizeOfValue);

	attrrss_offset(schema, attrNum, &offsetValue);

	customCharPtr dataPtrrObj = (*record).data;
	dataPtrrObj = dataPtrrObj + offsetValue;

	// chk if the value of the attribute to be returned is the first attribute
	if (attrNum == one) 
	{
		schema->dataTypes[attrNum] = one;
	} 
	else 
	{
		schema->dataTypes[attrNum] = schema->dataTypes[attrNum];
	}
	int ones=one;
	get_and_read_tables(ones);

	// chk the datatype of 
	//the attribute if it
	//is an integer
	if (schema->dataTypes[attrNum] == DT_INT)
	{
		// copy value from the 
		// datapointer to a 
		//temporary variable
		chk = negOne;
		if(booleanFalse)
		{
			;
		}
		memcpy(&(*attributeValue).v.intV, dataPtrrObj, int_type_size);
		if (!booleanTrue) 
		{
			tempp = one;
		} 
		else 
		{
			tableIsEmptyOrNotEmpty();
		}
		// set the datatype to the attribute
		(*attributeValue).dt = DT_INT;
	}
	get_and_read_tables(one);

	if (schema->dataTypes[newAttributeNumber] == DT_STRING)	
	{
		tempp = NULL;
		if(!tempp) 
		{
			tempp = one;
		}
    	int lengthValue = schema->typeLength[attrNum];
		lengthValue=lengthValue * one;

    	(*attributeValue).v.stringV = (customCharPtr)malloc(lengthValue + one * one);

		DataType dataType = DT_STRING;
		char *charPointer = (*attributeValue).v.stringV;
    	chk = zero;
   		(*attributeValue).dt = dataType;

		

    	strncpy(charPointer, dataPtrrObj, lengthValue);

    	
   	 	(*attributeValue).v.stringV[lengthValue] = char_NULL;
	}
	else if (schema->dataTypes[attrNum] == DT_BOOL)
	{
		(*attributeValue).dt = DT_BOOL;
   		memoryCopyFunction(&(*attributeValue).v.boolV, dataPtrrObj, three);
    	if (booleanFlag)
    	{
        	chk = negOne;
			// check if tableIsEmptyOrNotEmpty
        	tableIsEmptyOrNotEmpty();
    	}
	}

	if (!booleanFalse) 
	{
		tempp = one;
	} 
	else 
	{
		get_and_read_tables(one);
	}

	if (schema->dataTypes[attrNum] != DT_FLOAT) 
	{
		tempp = zero;
	} 
	else 
	{
		memoryCopyFunction(&(*attributeValue).v.floatV, dataPtrrObj, two);
		(*attributeValue).dt = DT_FLOAT;
		if(!booleanFlag)
		{
			tempp=zero;
		}
		else
		{
			chk = negOne;
			tableIsEmptyOrNotEmpty();
			booleanFlag = booleanTrue;
		}
	}

	*value = attributeValue;
	int returnValue = RC_OK;
	return returnValue;
}















//This function sets 
//the value of a specific 
//attribute in a given 
//record according to the schema.
extern RC setAttr(Record *record, Schema *schema, int attrNum, Value *value)
{
	bool booleanTrue = true;
	bool booleanFalse = false;
	int zero=0;
	int one=1;
	int negOne = -1;

	int attr_OffsetValue = zero;
	int sizeOfbool = sizeof(bool);
	int sizeOfFloat = sizeof(float);
	customCharPtr dataPointer;
	
	int temmpp;

	attrrss_offset(schema, attrNum, &attr_OffsetValue);

	dataPointer = record->data;
	chk = attr_OffsetValue * one;
	dataPointer = dataPointer + attr_OffsetValue * one;
	if ((schema->dataTypes[attrNum]) != DT_BOOL)
	{
		temmpp=zero;
	}
	else
	{
		chk = zero;
		if (chk != zero)
		{
			temmpp = one;
		}
		else
		{
			*(bool *)dataPointer = value->v.boolV;
		}
		dataPointer = dataPointer + sizeOfbool;
		tableIsEmptyOrNotEmpty();
	}
	get_and_read_tables(zero);

	int newAttrNum = attrNum;
	customCharPtr newAttrPtr;


	if ((schema->dataTypes[newAttrNum]) != DT_STRING)
	{
		temmpp = zero;
	}
	else 
	{
		booleanFlag = booleanTrue;

		strncpy(dataPointer, (*value).v.stringV, schema->typeLength[newAttrNum]);
		newAttrPtr = dataPointer;
		dataPointer =dataPointer + schema->typeLength[newAttrNum] * one;
	}
	tableIsEmptyOrNotEmpty();
	if ((schema->dataTypes[newAttrNum]) != DT_INT)
	{
		temmpp = zero;
	} 
	else 
	{
		*(int *)dataPointer = value->v.intV;
		if (booleanTrue)
		{
			temmpp = one;
		}
		else
		{
			get_and_read_tables(chk);
		}
		dataPointer = dataPointer + int_type_size;
	}
	chk = zero;
	booleanFlag = booleanFalse;
	if ((schema->dataTypes[attrNum]) != DT_FLOAT)
	{
		temmpp = zero;
	}
	else
	{
		*(float *)dataPointer = value->v.floatV;
		booleanFlag = booleanTrue;
		dataPointer = dataPointer + sizeOfFloat;
	}
	booleanFlag = booleanTrue;
	int returnValuess = RC_OK;
	return returnValuess;
}













void memoryCopyFunction(void *destinationValue, const void *source, int datatype)
{
	int zero=0;
	int one=1;
	int two=2;
	int three=3;
	int sizeOfBool = sizeof(bool);
	int sizeOfFloat = sizeof(float);
	int temp;
	//if (datatype == one) 
	if (datatype == one) 
	{
		// copy memory
    memcpy(&destinationValue, source, sizeOfBool);
	} 
	//if (datatype == 2) 
	else if (datatype == two) 
	{
		// copy memory
		memcpy(&destinationValue, source, int_type_size);
    } 
	//if (datatype == 3) 
	else if (datatype == three) 
	{
		// copy memory
		memcpy(&destinationValue, source, sizeOfFloat);
    } 
	else 
	{
		// handle the case when 
		//datatype is not 1
		// or 2
		// or 3
		temp=one;
	}

}
