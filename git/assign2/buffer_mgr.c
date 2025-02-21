#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "buffer_mgr.h"
#include "storage_mgr.h"
#include "pthread.h"

static pthread_mutex_t mutex_initializer = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_unpinpage_initializer = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_pinpage_initializer = PTHREAD_MUTEX_INITIALIZER;

typedef struct Page
{
	SM_PageHandle data;
	PageNumber pageNumber;
	int fixCount;
	int hitLRU;
	int refLRU;
	int dirtyFlag;
} PageFrame;


int writeCount = 0; // number of pages written to disk
int clockPointer = 0; //pointer for CLOCK replacement strategy
int lfuPointer = 0; //pointer for LFU replacement strategy
int bufferSize = 0; // number of pages in buffer pool

int rearIndex = 0; //used for FIFO replacement strategy
int hitCount = 0; // keeps tracks of pages access frequency


//FIFO implements the First-In-First-Out (FIFO) 
//page replacement policy for a 
//buffer pool in a database management system.
void FIFO(BM_BufferPool *const bm_Buffer_Pool, PageFrame *page_Frame)
{
    //rearIndex represents the index of the page that 
    //should be replaced (following FIFO order). 
    //bufferSize is the number of frames in the buffer pool. 
    //The modulo operation (% bufferSize) ensures 
    //that indexValue stays within valid buffer indices. 

    int indexValue = rearIndex % bufferSize;
    
    //bm_Buffer_Pool->mgmtData is  store the list of page frames.
	PageFrame *pgFrame = (PageFrame *)bm_Buffer_Pool->mgmtData;

    //file_handle is declared to interact with the storage manager
	SM_FileHandle file_handle;
    
    //loop counter
    int i = 0;

    //This loop ensures we check up to 
    //bufferSize pages to find a replaceable page. 
	while (i < bufferSize)
	{
        //If fixCount == 0, it means the page is not pinned and can be replaced. 
        // If fixCount != 0, the page is still in use, 
        //so we must search for another frame.
		if (pgFrame[indexValue].fixCount == 0)
		{
            // Checking if the 
            //dirty flag is set 
            //or not
            //If a page has been modified (dirtyFlag == 1), 
            //it must be written back to disk before being replaced. 

			if (pgFrame[indexValue].dirtyFlag == 1)
			{
                //The file containing the buffer pool is opened. 
               //writeCount is incremented (presumably to track write operations). 
               //The page is written back to disk using writeBlock.
				openPageFile(bm_Buffer_Pool->pageFile, &file_handle);
                writeCount++;
				writeBlock(pgFrame[indexValue].pageNumber, &file_handle, pgFrame[indexValue].data);
			}
             
            int dirtyFlag = page_Frame->dirtyFlag;

            // Assigning dirty flag
            //The dirtyFlag from page_Frame (new page) is copied into the buffer pool. 
			pgFrame[indexValue].dirtyFlag = dirtyFlag;

            // Assigning data
            // The actual page data is assigned to the buffer pool. 
			pgFrame[indexValue].data = page_Frame->data;

            int fixCount_var = page_Frame->fixCount;

            // Assigning pagenumber
			pgFrame[indexValue].pageNumber = page_Frame->pageNumber;

            // Assigning fixCount
			pgFrame[indexValue].fixCount = fixCount_var;

			break;
		}
        //If the page at indexValue is pinned (fixCount != 0), move to the next frame. 
       //The modulo operation ensures circular traversal of the buffer pool. 
		else
		{
			indexValue += 1;
			if (indexValue % bufferSize == 0)
            {
				indexValue = 0;
            }
		}
        i++;
	}
}




//The function CLOCK implements the Clock Page Replacement Algorithm 
//for a buffer pool. This algorithm is a variation of FIFO with 
//a second-chance mechanism, ensuring that frequently accessed pages 
//stay in memory longer.
//- This function replaces a page in the buffer pool using the Clock replacement policy.
//- It finds a victim page using a circular list (clockPointer).
//- If a page has hitLRU == 0 (i.e., not recently used), it gets replaced.
//- If a page has hitLRU == 1, it gets a second chance (set to 0).
//- If a dirty page is replaced, it is written back to disk.
void CLOCK(BM_BufferPool *const buffer_pool, PageFrame *pageFrm)
{
    
    //Declares a file handle to perform disk I/O operations if needed
    SM_FileHandle file_handle;

    // Initializing pageframe
    PageFrame *pageframe = (PageFrame *)buffer_pool->mgmtData;
    //- A loop runs indefinitely until a page is replaced.
   //  - Ensures that a suitable victim page is found.
    while (true)
    {
        // calculating the offset
        //Ensures circular behavior by resetting c
        //lockPointer when it reaches bufferSize.
        // clockPointer % bufferSize wraps around if
        // clockPointer exceeds the buffer size.
        int valueOffset = clockPointer % bufferSize;
        if(valueOffset == 0)
        {
            clockPointer = 0;
        }
    
        //If hitLRU == 0, the page was not recently used and is a candidate for replacement.
       // If hitLRU == 1, the page gets a second chance (handled later).
        if(pageframe[clockPointer].hitLRU == 0)
        {
            if (pageframe[clockPointer].dirtyFlag == 1)
            {
                openPageFile(buffer_pool->pageFile, &file_handle);
                SM_PageHandle mem_p = pageframe[clockPointer].data;
                writeCount++;
                
                writeBlock(pageframe[clockPointer].pageNumber, &file_handle, mem_p);
            }
            
            // Setting the hit lru
            pageframe[clockPointer].hitLRU = pageFrm->hitLRU;

            // Setting the page number
            pageframe[clockPointer].pageNumber = pageFrm->pageNumber;
            int fixCount_var = pageFrm->fixCount;
            int dirtyFlag_var = pageFrm->dirtyFlag;
            
            // Setting the dirty flag
            pageframe[clockPointer].dirtyFlag = dirtyFlag_var;
           
            // Setting the data
            pageframe[clockPointer].data = pageFrm->data;
            
            // Setting the fix count
            pageframe[clockPointer].fixCount = fixCount_var;
            
            // Incrementing the clock pointer
            clockPointer++;
            break;
            


        } 
        //- If hitLRU == 1, the page was recently used.
      // It gets a second chance (set hitLRU = 0).
     // The algorithm skips this page and moves forward to find a victim.

        else 
        {
            pageframe[clockPointer++].hitLRU = 0;
        }
    }
}




void LFU(BM_BufferPool *const buffer_pool, PageFrame *page_frame)
{
    //loop counters
    int i = 0, j;

    // Defining file handle
    //Declares a file handle for disk operations, 
    //used if a dirty page needs to be written back.
    SM_FileHandle fileHandle;

    //Retrieves the buffer pool's page frames by casting buffer_pool->mgmtData 
    //into a PageFrame .
  // pframe now points to the array of page frames in memory.
    // Initializing page frame
    PageFrame *pframe = (PageFrame *)buffer_pool->mgmtData;
    
    // Initializing least frequent index and least frequent reference
    //leastFreqIndex: Stores the index of the least frequently used page 
    //(starts at lfuPointer).
    // leastFreqRef: Stores the smallest reference count (refLRU).
    int leastFreqIndex = lfuPointer;
    int leastFreqRef = 0;

    //Finds an unpinned page (fixCount == 0).
    //Starts the search from lfuPointer, scanning the buffer.
    // If an unpinned page is found:
     //  leastFreqIndex is updated.
     //  leastFreqRef is set to this page’s refLRU.
     //  Breaks the loop after the first unpinned page is found.
   //  Uses % bufferSize to handle circular indexing.
    while (i < bufferSize)
    {
        if (pframe[leastFreqIndex].fixCount == 0)
        {
            leastFreqIndex = leastFreqIndex + i;
            leastFreqIndex = leastFreqIndex % bufferSize;
            leastFreqRef = pframe[leastFreqIndex].refLRU;
            break;
        }
        i++;
    }
    i = (leastFreqIndex + 1) % bufferSize;

    j = 0;
    while (j < bufferSize)
    {
        if (!(pframe[i].refLRU > leastFreqRef))
        {
            leastFreqIndex = i;
            leastFreqRef = pframe[i].refLRU;
        }
        i = (i + 1);
        i = i % bufferSize;
        j++;
    }

    // Checking if the Page is Dirty
    if (pframe[leastFreqIndex].dirtyFlag == 1)
    {
        // Opening the page file
        //- If the victim page (least frequently used) is dirty (dirtyFlag == 1):
  //- Opens the file using openPageFile().
  //- Writes the page back to disk using writeBlock().
  // Increments writeCount, which tracks the number of disk writes.
        openPageFile(buffer_pool->pageFile, &fileHandle);
        SM_PageHandle memoryPage = pframe[leastFreqIndex].data;
        // Writing the contents
        writeBlock(pframe[leastFreqIndex].pageNumber, &fileHandle, memoryPage);
        writeCount++;
    }

    // Setting the lfuPointer
    //- Moves the LFU pointer forward so that the next replacement starts from a different index.
   // It should use % bufferSize to ensure circular indexing.
    lfuPointer = leastFreqIndex + 1;

    int dirtyFlag_var = page_frame->dirtyFlag;
    // Setting the dirty flag
    pframe[leastFreqIndex].dirtyFlag = dirtyFlag_var;

    int fixCount_var = page_frame->fixCount;

    // Setting the fixCount
    pframe[leastFreqIndex].fixCount = fixCount_var;

    // Setting the data
    pframe[leastFreqIndex].data = page_frame->data;

    // Setting the pageNumber
    pframe[leastFreqIndex].pageNumber = page_frame->pageNumber;

}




//This function implements Least Recently Used (LRU)
// page replacement in a buffer pool. 
//The LRU algorithm replaces the page that was 
//used the longest time ago.
void LRU(BM_BufferPool *const bm, PageFrame *page)
{
    PageFrame *pframe = (PageFrame *)bm->mgmtData;
    SM_FileHandle fileHandle;

    int lHR;
    int lHC; 

    //initialise index
    int index = 0;

    //The loop scans through the buffer pool (pframe array) 
    //looking for a free page (fixCount == 0).
    //When it finds the first unpinned page, it:
    //Stores its index (lHR).
   //Stores its hitLRU counter (lHC).
   //Breaks out of the loop.
    while (index < bufferSize)
    {
        if (pframe[index].fixCount == 0)
        {
            lHC = pframe[index].hitLRU;
            lHR = index;
            break;
        }
        index++;
    }

    // Assigning index to lhr plus one
    //Starts scanning from lHR + 1 (next page) 
    //to find the page with the lowest hitLRU value.
    //The smaller the hitLRU value, the older 
    //the page, so it updates lHR whenever it 
    //finds a page with a smaller hitLRU.
    index = lHR + 1;

    for (int i = index; i < bufferSize; i++)
    {
        if (pframe[i].hitLRU < lHC)
        {
            lHC = pframe[i].hitLRU;
            lHR = i;
        }
    }


    // If the selected LRU page is dirty (dirtyFlag == 1):
   //Open the page file (bm>pageFile).
   //Write the page back to disk (writeBlock()).
   //Increase the writeCount (tracks how many times pages are written to disk).
   //This ensures data consistency before replacing the page.

    if (pframe[lHR].dirtyFlag == 1)
    {
        openPageFile(bm->pageFile, &fileHandle);
        writeCount++;
        SM_PageHandle mem_p = pframe[lHR].data;
        writeBlock(pframe[lHR].pageNumber, &fileHandle, mem_p);
    }
    
    int pageNumber_var = page->pageNumber;

    // Setting the hitLru
    pframe[lHR].hitLRU = page->hitLRU;

    int dirtyFlag = page->dirtyFlag;

    int fixCount_var = page->fixCount;

    // Setting the dirty flag
    pframe[lHR].dirtyFlag = dirtyFlag;

    // Set the fixcount
    pframe[lHR].fixCount = fixCount_var;

    // Set the data
    pframe[lHR].data = page->data;

    // Set the pagenumber
    pframe[lHR].pageNumber = pageNumber_var;
}



//This function initializes a buffer pool 
//by allocating memory for page frames and
// setting up necessary tracking variables 
//for different replacement 
//strategies.

RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, 	const int numPages, ReplacementStrategy strategy, void *stratData) {
    //Locks the mutex_initializer mutex to ensure that 
    //only one thread initializes the buffer pool at a time.
  //Prevents race conditions if multiple threads try 
  //to initialize the buffer pool simultaneously.

            pthread_mutex_lock(&mutex_initializer);
            int index = 0;
            bufferSize = numPages;
            int one=1;
            
            //Allocates memory for numPages page frames in the buffer pool.
       // Uses malloc() to dynamically allocate an array of PageFrame structures.


            PageFrame *page_Frame = malloc(sizeof(PageFrame) * numPages * one);


            //index → Used for looping.
   //   bufferSize = numPages → Stores the total number 
   //of pages in the global variable bufferSize.
   //Iterates through all allocated page frames and initializes:
   //refLRU = 0 → Least Frequently Used (LFU) reference counter.
   //hitLRU = 0 → Least Recently Used (LRU) hit counter.
   //data = NULL → Initially, no page data is loaded.

            for(index = 0; index < numPages; index++) 
            {
                page_Frame[index].refLRU = 0;
                page_Frame[index].hitLRU = 0;
                page_Frame[index].data = NULL;
            }


            // Loops through the page frames again to initialize:
   //pageNumber = 1 → Indicates that no valid page is stored.
   //dirtyFlag = 0 → Page has not been modified.
   //fixCount = 0 → No clients are using this page.
            index = 0;
            for(index = 0; index < numPages; index++) 
            {
                page_Frame[index].pageNumber = -1;
                page_Frame[index].dirtyFlag = 0;
                page_Frame[index].fixCount = 0;
            }

            bm->pageFile = (char *)pageFileName;
            bm->mgmtData = page_Frame;
            bm->numPages = numPages;
            bm->strategy = strategy;

            //reset tracking variable
            //Resets tracking variables:
      //writeCount = 0 → Counts the number of page writes to disk.
      //clockPointer = 0 → Used for the Clock replacement algorithm.
   //lfuPointer = 0 → Used for the Least Frequently Used (LFU) replacement algorithm.
            writeCount = 0;
            clockPointer = 0;
            lfuPointer = 0;
            pthread_mutex_unlock(&mutex_initializer);
            return RC_OK;
} 


//This function shuts down a buffer pool, 
//ensuring that all pages are properly flushed to disk 
//and no pages are still in use (pinned). If any 
//page is pinned, it prevents the shutdown.

RC shutdownBufferPool(BM_BufferPool *const bm) 
{
    //loop counter
    int index = 0; 
    bool results = false;
    //Retrieves the array of page frames stored
    // in bm>mgmtData.
   //Casts it to a PageFrame, assuming 
   //mgmtData stores a dynamically allocated PageFrame array.
    PageFrame *pageFrame = (PageFrame*) bm->mgmtData;

    //Flush All Pages to Disk
    forceFlushPool(bm);

    //Iterates through all page frames in the buffer pool.
 //   Retrieves pageFrame[i].fixCount:
    //If fixCount != 0, it means the page
    // is still pinned (being used by another process).
     //Sets res = true and exits 
     //the loop immediately.
   //If all pages have fixCount == 0, 
   //the buffer pool can be safely shut down.
    while(index < bufferSize) 
    {
        int fixedCount = pageFrame[index].fixCount;
        if(fixedCount != 0) 
        {
            results = true;
            //when true, break
            break;
        }
        //increment index
        index++;
    }
    // if result is true
    // return buffer has pinned pages
    //it returns RC_BUFFER_POOL_HAS_PINNED_PAGES.
 //  This prevents shutdown until 
 //all pages are unpinned.
    if(results == true)
    {
        return RC_BUFFER_POOL_HAS_PINNED_PAGES;
    }

    else 
    {
        //Nullifies the buffer pool's management 
        //data, ensuring that the now 
        //deallocated memory is not accessed.

        bm->mgmtData = NULL;
        // freeing the Buffer Pool
        free(pageFrame);
        return RC_OK;
    }
}






RC forceFlushPool(BM_BufferPool *const bm) 
{
    // Retrieves the array of page frames from bm>mgmtData.
    PageFrame *pageFrame = (PageFrame*) bm->mgmtData;

    ////Loop counter for iterating through all pages in the buffer pool.
    int index = 0; 

    //File handle for opening 
    //and writing to the page file.
    SM_FileHandle file_handle;

    //Iterates through all pages in the buffer pool using i as an index.
    while(index < bufferSize) 
    {
        //Check for Dirty and Unpinned Pages
        //conditions:
  //pageFrame[i].fixCount == 0 . The page is not pinned (not being used).
  //pageFrame[i].dirtyFlag == 1 . The page has been modified but not yet written to disk.
  //If both conditions are met, the page is written to disk.
        if(pageFrame[index].fixCount == 0 && pageFrame[index].dirtyFlag == 1) 
        {
            //Retrieves the logical page number of the dirty page.
            int pageNumber = pageFrame[index].pageNumber;
            //Opens the page file (associated with the buffer pool) for writing.
            // and retireve the data
            openPageFile(bm->pageFile, &file_handle);
            SM_PageHandle pageHandle = pageFrame[index].data;
            //Writes the page's data to 
            //its correct position in the file.
            writeBlock(pageNumber, &file_handle, pageHandle);

            //Reset Dirty Flag & Update Write Count
            pageFrame[index].dirtyFlag = 0;
            writeCount++;
        }
        index++;
    }
    return RC_OK;
}


//This function marks a page 
//as dirty in the buffer pool, 
//meaning the page has been modified and 
//needs to be written back to disk 
//before it can be replaced.

RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page) 
{
    //Retrieves the page frame array from the buffer pool.
 //Casts bm>mgmtData to PageFrame, assuming 
 //it holds a dynamically allocated array of 
 //PageFrame structures.
 //dirtyPointer is now pointing to the array of pages in the buffer.

    PageFrame *dirty_Pointer = (PageFrame *)bm->mgmtData;

    // Loops through all pages 
    //in the buffer pool.
    for(int index=0; index < bufferSize; index++) 
    {
        // Checks if the page number in the buffer 
        //matches the page number in the given page handle.
     //This ensures that we find the correct 
     //page in the buffer pool.


        if(page->pageNum == dirty_Pointer[index].pageNumber) 
        {
            //Marks the page as dirty by setting its dirtyFlag to 1.
    //This means the page has been 
    //modified and needs to be written back 
    //to disk before being replaced.
            dirty_Pointer[index].dirtyFlag = 1;
            return RC_OK;
        }
    }
    return RC_ERROR;
}


//This function decrements 
//the fix count of a page in the buffer pool,
// meaning the page is no longer "pinned" 
//(actively in use). If no processes are using 
//the page, it can now be replaced by another
// page when needed.


RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page) 
{
    // Locks a mutex to ensure thread 
    //safety while modifying the buffer pool.
  //This prevents race conditions when multiple
  // threads try to unpin pages simultaneously.



    pthread_mutex_lock(&mutex_unpinpage_initializer);

    //Retrieves the page frame array 
    //from the buffer pool.
 //Casts bm>mgmtData to PageFrame
 //unpinpagePointer is  pointing to the array of pages in the buffer.

    PageFrame *unpinpagePointer = (PageFrame *)bm->mgmtData;

    //loop through buffer
        for(int i=0; i<bufferSize; i++) 
        {
            // Checks if the page number 
            //in the buffer matches the page
            // number in the given page handle.
    //This ensures that we find the 
    //correct page in the buffer pool.

    //Decrements the fix count of the page.
 //The fixCount tracks how many processes are currently using the page.
 //If fixCount == 0, it means no one is 
 //using the page anymore, and it can be replaced if necessary.
        if(page->pageNum == unpinpagePointer[i].pageNumber) 
        {
            unpinpagePointer[i].fixCount -= 1;

            ////Unlocks the mutex after modifying the buffer pool.
   //This allows other threads to access the buffer pool safely.
            pthread_mutex_unlock(&mutex_unpinpage_initializer);
            return RC_OK;
        }
    }
    //Unlocks the mutex after modifying the buffer pool.
   //This allows other threads to access the buffer pool safely.
    pthread_mutex_unlock(&mutex_unpinpage_initializer);
    return RC_ERROR;
}




RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page) 
{
    //Retrieves the page frame array 
    //from the buffer pool.
 //Casts bm>mgmtData to PageFrame, 
 //assuming it holds a dynamically allocated 
 //array of PageFrame structures.
    //force_Page_Ptr now points to 
    //the array of pages in the buffer.

    PageFrame *force_Page_Ptr = (PageFrame *)bm->mgmtData;
    //Loops through all pages in 
    //the buffer pool to find the target page.
    for(int index = 0; index < bufferSize; index++) 
    {
        //This finds the matching page in the buffer.
        if(!(force_Page_Ptr[index].pageNumber != page->pageNum)) 
        {
            //Declares a file handle (smFHandle) for the page file.
             //Opens the file associated with the buffer pool (bm>pageFile).
            SM_FileHandle smFHandle;
            openPageFile(bm->pageFile, &smFHandle);

            //Marks the page as clean because its contents are now written to disk.
    //The dirty flag indicates whether the page has been modified since it was loaded.
            force_Page_Ptr[index].dirtyFlag = 0;

            //Writes the page from memory to disk.
            //Ensures that the latest version of the page is stored 
            writeBlock(force_Page_Ptr[index].pageNumber, &smFHandle, force_Page_Ptr[index].data);
            writeCount = writeCount + 1;
        }
    }
    return RC_OK;
}


//This function pins a page into the 
//buffer pool. If the page is already in 
//the buffer, it increments its fixCount. 
//Otherwise, it loads the page from disk 
//using the specified replacement 
//strategy (FIFO, LRU, LFU, CLOCK).  
RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum) 
{
    //Locking Mutex to Ensure 
    //Thread Safety
    pthread_mutex_lock(&mutex_pinpage_initializer);
    // Retrieves the array of 
    //page frames (PageFrame structures)
    // from bm>mgmtData.
    PageFrame *page_Frame = (PageFrame *)bm->mgmtData;
    bool isBufferFull = true;
    SM_FileHandle fHandle;
    //if the first page frame is empty
    // (pageNumber == 1), the buffer is uninitialized.
 //Opens the file and allocates memory for the page.
 //Calls ensureCapacity(pageNum, &fHandle) 
 //to make sure the file can store pageNum pages.
 //Reads the page from disk and 
 //updates page metadata (fixCount, hitLRU, pageNumber).
 //Stores the page data into the BM_PageHandle.
 //Unlocks mutex and returns RC_OK.

    if(page_Frame[0].pageNumber == -1) 
    {
        rearIndex = 0;
        openPageFile(bm->pageFile, &fHandle);
        page_Frame[0].data = (SM_PageHandle) malloc(PAGE_SIZE);
        page_Frame[0].refLRU = 0;
        ensureCapacity(pageNum, &fHandle);
        page_Frame[0].fixCount += 1;
        readBlock(pageNum, &fHandle, page_Frame[0].data);
        page_Frame[0].pageNumber = pageNum;
        hitCount = 0;
        page_Frame[0].hitLRU = hitCount; 
        page->pageNum = pageNum;
        page->data = page_Frame[0].data;
        pthread_mutex_unlock(&mutex_pinpage_initializer);
        return RC_OK;
    }
    else 
    {
        //Loop through the buffer pool to 
        //check if the page is already loaded.
 //If pageNumber == pageNum, the page is 
 //already in the buffer.
 //Increments hitCount and marks the
 //buffer as not full (isBufferFull = false).
 //Increases the fix count (fixCount += 1) 
 // indicates that another process is using the page.
        for(int i = 0; i < bufferSize; i++) 
        {
            if(page_Frame[i].pageNumber != -1) 
            {
                if(page_Frame[i].pageNumber == pageNum) 
                {
                    hitCount+=1;
                    isBufferFull = false; 
                    page_Frame[i].fixCount += 1;
                    page->pageNum = pageNum;
                    page->data = page_Frame[i].data;
                    //Updates metadata based on the replacement strategy:
   //CLOCK: 
   //LFU (Least Frequently Used): 
    //LRU (Least Recently Used):
   // Increments clockPointer for CLOCK replacement.

                    switch(bm->strategy) 
                    {
                        case RS_CLOCK:
                            page_Frame[i].hitLRU = 1;
                            break;
                        case RS_LFU:
                            page_Frame[i].refLRU += 1;
                            break;
                        case RS_LRU:
                            page_Frame[i].hitLRU = hitCount;
                            break;
                        default:
                            break;
                    }
                    clockPointer+=1;
                    break;
                }
            } 
            else 
            {
                // If Page is Not in Buffer, Load it From Disk
                //Finds an empty frame (pageNumber == 1).
      //Allocates memory and loads the page from disk (readBlock).
     //Updates page metadata (pageNumber, fixCount, hitLRU).

                page_Frame[i].data = (SM_PageHandle) malloc(PAGE_SIZE);
                openPageFile(bm->pageFile, &fHandle);
                readBlock(pageNum, &fHandle, page_Frame[i].data);
                page_Frame[i].pageNumber = pageNum;
                rearIndex += 1;
                page_Frame[i].refLRU = 0;
                hitCount += 1;
                page_Frame[i].fixCount = 1; 

                page->pageNum = pageNum;
                page->data = page_Frame[i].data;
                switch (bm->strategy) 
                {
                    case RS_CLOCK:
                        page_Frame[i].hitLRU = 1;
                        break;
                    
                    case RS_LRU:
                        page_Frame[i].hitLRU = hitCount;
                        break;

                    default:
                        break;
                }
                isBufferFull = false;  
                break;      
            }
        }

        //Apply Replacement Strategy if Buffer is Full
        if(isBufferFull) 
        {
            PageFrame *pageNew = (PageFrame *) malloc(sizeof(PageFrame));
            rearIndex += 1;
            openPageFile(bm->pageFile, &fHandle);
            pageNew->data = (SM_PageHandle) malloc(PAGE_SIZE);
            hitCount += 1;
            readBlock(pageNum, &fHandle, pageNew->data);
            pageNew->dirtyFlag = 0;
            pageNew->refLRU = 0;
            pageNew->fixCount = 1;
            pageNew->pageNumber = pageNum;

            
            switch (bm->strategy)
            {
            case RS_CLOCK:
                pageNew->hitLRU = 1;
                break;
            
            case RS_LRU:
                pageNew->hitLRU = hitCount;
                break;

            default:
                break;
            }

            page->data = pageNew ->data;
            page->pageNum = pageNum;

            ReplacementStrategy replacementStrategy = bm->strategy;


            //choose a replacement strategy
            //alls the appropriate page replacement 
            //function (CLOCK, FIFO, LFU, or LRU).
        //These functions decide which page to 
        //evict based on the strategy.
            switch (replacementStrategy)
            {
            case RS_CLOCK:
                CLOCK(bm, pageNew);
                break;

            case RS_FIFO:
                FIFO(bm, pageNew);
                break;

            case RS_LFU:
                LFU(bm, pageNew);
                break;

            case RS_LRU:
                LRU(bm, pageNew);
                break;

            default:
                break;
            }
        }

        //Unlocks the mutex.
     //Returns RC_OK to indicate successful page pinning.
        pthread_mutex_unlock(&mutex_pinpage_initializer);
        return RC_OK;
    }
    ////Unlocks the mutex.
    pthread_mutex_unlock(&mutex_pinpage_initializer);
}




//This function retrieves the current 
//contents of the buffer pool and returns 
//an array containing the page numbers 
//stored in each frame.
PageNumber *getFrameContents (BM_BufferPool *const bm) 
{
    //Pointer to the buffer pool 
    //whose contents we want to retrieve.
    PageFrame *page_Frame = (PageFrame *)bm->mgmtData;
    //Retrieves the page frame array 
    //from the buffer pool.
    //Casts bm>mgmtData to PageFrame,
     //assuming that mgmtData holds an array of PageFrame structures.
    //pageFrame now points to the list of pages currently in the buffer pool.
 

    //Dynamically allocates memory 
    //to store the page numbers of all frames in the buffer pool.
    PageNumber *frame_Contentss = malloc(sizeof(PageNumber) * bufferSize);
    // Iterates through each page frame in the buffer.
    for(int index = 0; index < bufferSize; index++) 
    {
        //Checks if the frame is empty (pageNumber == 1).
   //1 usually indicates that the 
   //frame does not contain a valid page.
   //Stores NO_PAGE (typically 1) in 
   //frameContents[i] to indicate an empty frame.

        if(page_Frame[index].pageNumber == -1) 
        {
            frame_Contentss[index] = NO_PAGE;
        } 
        // If the frame is not empty,
         //store the actual page number in frameContents[i].
        else 
        {
            frame_Contentss[index] = page_Frame[index].pageNumber;
        }
    }
    return frame_Contentss;
}



//This function retrieves an array of dirty flags for all 
//pages in a buffer pool. The dirty flag indicates whether 
//a page has been modified (`true`) or not (`false`). 
//The function dynamically allocates an array 
//of `bool` values and returns it.

bool *getDirtyFlags (BM_BufferPool *const bm) 
{
    PageFrame *pageFrame = (PageFrame *)bm->mgmtData;

    // Dynamically allocates memory for an 
    //array of bool values of size `bufferSize` 
    bool *dirtyFlags =  malloc(sizeof(bool) * bufferSize);

    //Loops through all the bufferSize p
    //ages in the buffer pool.
    for(int i = 0;  i < bufferSize; i++) 
    {
        //Checks whether the dirtyFlag of the current page is set (1).
  //If yes, set dirtyFlags[i] = true (meaning the page has been modified).
  // If no, set dirtyFlags[i] = false (meaning the page is clean and 
  //does not need to be written back to disk).

        if(pageFrame[i].dirtyFlag == 1) 
        {
            dirtyFlags[i] = true;
        } 
        
        else 
        {
            dirtyFlags[i] = false;
        }
    }
    return dirtyFlags;
}




//This function retrieves the fix count of
// each page in a buffer pool and returns an array
// of these values. The fix count represents how many 
//clients (transactions) have pinned a particular
// page. A page with fixCount > 0 is currently in use
// and cannot be replaced.
int *getFixCounts (BM_BufferPool *const bm) 
{
    //Dynamically allocates an array to store fix counts for all pages.
    int *fixCounts = malloc(sizeof(int) * bufferSize);
    //- bm->mgmtData stores the array of page frames.
    //This line casts bm->mgmtData into a 
    //PageFrame  so it can be accessed as an array.
    PageFrame *pageFrame = (PageFrame *)bm->mgmtData;

    //
     // Loops through each page frame in the buffer pool.
    for(int i = 0; i < bufferSize; i++) 
    {
        //If fixCount is valid (!= -1), store the actual fixCount.
      //If fixCount is -1, store 0 instead.
        if(pageFrame[i].fixCount != -1) 
        {
            fixCounts[i] = pageFrame[i].fixCount;
        } 
        else 
        {
            fixCounts[i] = 0;
        }
    }
    return fixCounts;
}



int getNumReadIO (BM_BufferPool *const bm) 
{
    ////return value of rearIndex+1
    return rearIndex + 1;
}

int getNumWriteIO (BM_BufferPool *const bm) 
{
    //return value of writeCount
    return writeCount;
}