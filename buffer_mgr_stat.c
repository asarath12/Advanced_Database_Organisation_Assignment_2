#include "buffer_mgr_stat.h"
#include "buffer_mgr.h"

#include <stdio.h>
#include <stdlib.h>

// local functions
static void printStrat (BM_BufferPool *const bm);

// external functions
void 
printPoolContent (BM_BufferPool *const bm)
{
	PageNumber *frameContent;
	bool *dirty;
	int *fixCount;
	int i;

	frameContent = getFrameContents(bm);
	dirty = getDirtyFlags(bm);
	fixCount = getFixCounts(bm);

	printf("{");
	printStrat(bm);
	printf(" %i}: ", bm->numPages);

	for (i = 0; i < bm->numPages; i++)
		printf("%s[%i%s%i]", ((i == 0) ? "" : ",") , frameContent[i], (dirty[i] ? "x": " "), fixCount[i]);
	printf("\n");
}

char *
sprintPoolContent (BM_BufferPool *const bm)
{
	PageNumber *frameContent;
	bool *dirty;
	int *fixCount;
	int i;
	char *message;
	int pos = 0;

	message = (char *) malloc(256 + (22 * bm->numPages));
	frameContent = getFrameContents(bm);
	dirty = getDirtyFlags(bm);
	fixCount = getFixCounts(bm);

	for (i = 0; i < bm->numPages; i++)
		pos += sprintf(message + pos, "%s[%i%s%i]", ((i == 0) ? "" : ",") , frameContent[i], (dirty[i] ? "x": " "), fixCount[i]);

	return message;
}


void
printPageContent (BM_PageHandle *const page)
{
	int i;

	printf("[Page %i]\n", page->pageNum);

	for (i = 1; i <= PAGE_SIZE; i++)
		printf("%02X%s%s", page->data[i], (i % 8) ? "" : " ", (i % 64) ? "" : "\n");
}

char *
sprintPageContent (BM_PageHandle *const page)
{
	int i;
	char *message;
	int pos = 0;

	message = (char *) malloc(30 + (2 * PAGE_SIZE) + (PAGE_SIZE % 64) + (PAGE_SIZE % 8));
	pos += sprintf(message + pos, "[Page %i]\n", page->pageNum);

	for (i = 1; i <= PAGE_SIZE; i++)
		pos += sprintf(message + pos, "%02X%s%s", page->data[i], (i % 8) ? "" : " ", (i % 64) ? "" : "\n");

	return message;
}

void
printStrat (BM_BufferPool *const bm)
{
	switch (bm->strategy)
	{
	case RS_FIFO:
		printf("FIFO");
		break;
	case RS_LRU:
		printf("LRU");
		break;
	case RS_CLOCK:
		printf("CLOCK");
		break;
	case RS_LFU:
		printf("LFU");
		break;
	case RS_LRU_K:
		printf("LRU-K");
		break;
	default:
		printf("%i", bm->strategy);
		break;
	}
}

/* 
 * Structure representing a Page Frame within the Buffer Pool.
 * Each frame acts as a container for storing a page from disk.
 */
typedef struct PageFrame
{
	int frameNum;		// Unique identifier assigned to the frame
	int pageNum;		// Page number of the data currently stored in this frame
	int dirtyFlag;		// Indicates if the page has been modified (1 = Yes, 0 = No)
	int fixCount;		// Counter to track the number of clients using this page
	int refBit;			// Used for CLOCK algorithm to identify recently accessed pages
	char *data;			// Pointer to the actual data content of the page
	struct pageFrame *next, *prev;	// Pointers for doubly linked list navigation, linking frames together
} PageFrame;

/* 
 * Structure used to manage metadata and operations of the Buffer Pool.
 * Keeps track of all active pages and buffer-related statistics.
 */
typedef struct BM_BufferPool_Mgmt
{
	int occupiedCount;		// Tracks the number of frames currently occupied in the buffer
	void *stratData;		// Placeholder for algorithm-specific data used in page replacement strategies
	PageFrame *head, *tail, *start;	// Pointers to manage linked list of frames in the buffer pool
	PageNumber *frameContent;	// Array storing the page numbers currently loaded in frames
	int *fixCount;				// Array maintaining the fix count values for each page in the buffer
	bool *dirtyBit;				// Boolean array marking whether a page has been modified or remains unchanged
	int numRead;				// Total count of pages loaded into the buffer from disk
	int numWrite;				// Total count of pages written back to disk from the buffer
} BM_BufferPool_Mgmt;

/*
 * Structure representing a file handler for disk-based storage management.
 * This keeps track of important details regarding a database file.
 */
typedef struct SM_FileHandle {
    char *fileName;       // Stores the name of the page file being handled
    int totalNumPages;    // Keeps count of the total number of pages in the file
    int curPagePos;       // Records the current position of the file cursor
    void *mgmtInfo;       // Pointer to additional file management metadata
} SM_FileHandle;


// Function to unpin a page by reducing its fix count
RC unpinPage(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle)
{
    // Retrieve metadata of the buffer pool for managing frames
    BM_BufferPool_Mgmt *bufferManager;
    bufferManager = bufferPool->mgmtData; // Extract management data from the buffer pool

    // Get the first frame in the buffer pool for traversal
    PageFrame *currentFrame = bufferManager->head;

    // Iterate over the linked list of page frames to locate the target page
    do
    {
        // Check if the current frame holds the requested page
        if (pageHandle->pageNum == currentFrame->pageNum)
        {
            // Reduce the fix count, indicating one less user is accessing this page
            currentFrame->fixCount--;

            return RC_OK; // Return success after successfully unpinning the page
        }
        currentFrame = currentFrame->next; // Move to the next frame in the circular linked list
    } while (currentFrame != bufferManager->head); // Stop once we've looped back to the starting frame

    return RC_OK; // Return success, even if the page was not found (no error handling required)
}



// Function to write a page back to disk if it has been modified (dirty)
RC forcePage(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle)
{
    // Retrieve the buffer pool's internal management data
    BM_BufferPool_Mgmt *bufferManager;
    bufferManager = bufferPool->mgmtData; // Extract metadata containing buffer details

    // Pointer to traverse the list of frames in the buffer
    PageFrame *framePtr = bufferManager->head;

    // File handler used for reading/writing pages to disk
    SM_FileHandle fileHandler;

    // Attempt to open the corresponding page file from storage
    if (openPageFile((char *)(bufferPool->pageFile), &fileHandler) != RC_OK)
    {
        return RC_FILE_NOT_FOUND; // Return error if the file does not exist
    }

    // Loop through the buffer pool to find the page that needs to be flushed
    do
    {
        // Check if the current frame contains the requested page and if it has been modified
        if (framePtr->pageNum == pageHandle->pageNum && framePtr->dirtyFlag == 1)
        {
            // Write the updated page data back to the disk file
            if (writeBlock(framePtr->pageNum, &fileHandler, framePtr->data) != RC_OK)
            {
                closePageFile(&fileHandler); // Close file before returning error
                return RC_WRITE_FAILED; // Return failure if write operation fails
            }

            // Increment the counter that tracks the number of write operations
            bufferManager->numWrite++;  

            // Mark the page as clean since the updated data has been written to disk
            framePtr->dirtyFlag = 0;    

            break; // Exit the loop once the required page has been processed
        }
        
        framePtr = framePtr->next; // Move to the next frame in the circular linked list
    } while (framePtr != bufferManager->head); // Stop looping once we return to the starting frame

    // Close the file after successfully writing the modified page back to disk
    closePageFile(&fileHandler); 

    return RC_OK; // Return success status
}


// Function to pin a page in the buffer pool, ensuring it is available for use
RC pinPage(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNumber)
{
    // Check which page replacement strategy is assigned to the buffer pool
    switch (bufferPool->strategy)
    {
    case RS_FIFO: // If the strategy is First-In-First-Out (FIFO)
        return pinPageFIFO(bufferPool, pageHandle, pageNumber); // Use FIFO page replacement logic
    
    case RS_LRU: // If the strategy is Least Recently Used (LRU)
        return pinPageLRU(bufferPool, pageHandle, pageNumber); // Call the LRU-based page pinning function
    
    case RS_CLOCK: // If the strategy is CLOCK (a circular buffer-based approach)
        return pinPageCLOCK(bufferPool, pageHandle, pageNumber); // Invoke the CLOCK algorithm for pinning pages
    
    default:
        return RC_OK; // Return success in case no valid replacement strategy is found
    }
}

// Function implementing FIFO (First-In-First-Out) page replacement strategy
RC pinPageFIFO(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNumber)
{
    // Declare a file handler to manage disk operations
    SM_FileHandle fileHandle;

    // Access the buffer pool's internal management data
    BM_BufferPool_Mgmt *bufferControl = bufferPool->mgmtData;

    // Pointer to the first frame in the buffer pool
    PageFrame *currentFrame = bufferControl->head;

    // Open the corresponding page file to enable page retrieval
    openPageFile((char*) bufferPool->pageFile, &fileHandle);

    // Check if the requested page is already present in the buffer pool
    do
    {
        // If the page is found, update page information and increment fix count
        if (currentFrame->pageNum == pageNumber)
        {
            pageHandle->pageNum = pageNumber;
            pageHandle->data = currentFrame->data;

            currentFrame->pageNum = pageNumber;
            currentFrame->fixCount++;
            return RC_OK; // Page successfully pinned
        }
        currentFrame = currentFrame->next; // Move to the next frame in the buffer
    } while (currentFrame != bufferControl->head); // Stop when the loop returns to the head

    // If there is available space in the buffer pool, pin the page in an empty frame
    if (bufferControl->occupiedCount < bufferPool->numPages)
    {
        currentFrame = bufferControl->head; // Start from the head frame
        currentFrame->pageNum = pageNumber; // Assign the requested page number

        // Advance the head pointer to the next available slot
        if (currentFrame->next != bufferControl->head)
        {
            bufferControl->head = currentFrame->next;
        }
        currentFrame->fixCount++; // Increment fix count since page is now in use
        bufferControl->occupiedCount++; // Track the number of occupied frames
    }
    else // If the buffer pool is full, apply FIFO replacement strategy
    {
        currentFrame = bufferControl->tail; // Start replacement from the tail

        do
        {
            // If the page is still in use (fix count > 0), move to the next frame
            if (currentFrame->fixCount != 0)
            {
                currentFrame = currentFrame->next;
            }
            else
            {
                // Before replacing, check if the page is dirty and write it back to disk
                if (currentFrame->dirtyFlag == 1)
                {
                    ensureCapacity(currentFrame->pageNum, &fileHandle);
                    if (writeBlock(currentFrame->pageNum, &fileHandle, currentFrame->data) != RC_OK)
                    {
                        closePageFile(&fileHandle);
                        return RC_WRITE_FAILED; // Error in writing the page back to disk
                    }
                    bufferControl->numWrite++; // Increment write count after successful write
                }

                // Update frame attributes with the new page data
                currentFrame->pageNum = pageNumber;
                currentFrame->fixCount++;

                // Adjust buffer pool pointers to maintain FIFO order
                bufferControl->tail = currentFrame->next;
                bufferControl->head = currentFrame;

                break; // Exit loop after replacing the page
            }
        } while (currentFrame != bufferControl->head); // Stop when back at the head frame
    }

    // Ensure the page file has enough pages, creating additional pages if required
    ensureCapacity((pageNumber + 1), &fileHandle);

    // Read the requested page from disk into the allocated frame
    if (readBlock(pageNumber, &fileHandle, currentFrame->data) != RC_OK)
    {
        closePageFile(&fileHandle);
        return RC_READ_NON_EXISTING_PAGE; // Return error if page does not exist
    }

    // Increment the count of read operations performed
    bufferControl->numRead++;

    // Assign the requested page details to the page handle
    pageHandle->pageNum = pageNumber;
    pageHandle->data = currentFrame->data;

    // Close the page file after successful loading
    closePageFile(&fileHandle);

    return RC_OK; // Successfully pinned the page
}

// Function implementing LRU (Least Recently Used) page replacement strategy
RC pinPageLRU(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNumber)
{
    // Retrieve buffer pool management metadata
    BM_BufferPool_Mgmt *bufferControl = bufferPool->mgmtData;

    // Pointer to the first frame in the buffer pool
    PageFrame *currentFrame = bufferControl->head;

    // Declare a file handler for disk operations
    SM_FileHandle fileHandler;

    // Open the database file to access pages
    openPageFile((char*)bufferPool->pageFile, &fileHandler);

    // Check if the requested page is already present in the buffer pool
    do
    {
        if (currentFrame->pageNum == pageNumber)
        {
            // Update the page handle with relevant details
            pageHandle->pageNum = pageNumber;
            pageHandle->data = currentFrame->data;

            // Refresh page attributes
            currentFrame->pageNum = pageNumber;
            currentFrame->fixCount++;

            // Adjust the LRU ordering by updating head and tail pointers
            bufferControl->tail = bufferControl->head->next;
            bufferControl->head = currentFrame;

            return RC_OK; // Page successfully pinned
        }
        currentFrame = currentFrame->next; // Move to the next frame in the buffer
    } while (currentFrame != bufferControl->head); // Stop when looping back to the starting frame

    // If there are empty slots in the buffer pool, store the page in an available frame
    if (bufferControl->occupiedCount < bufferPool->numPages)
    {
        currentFrame = bufferControl->head; // Start from the head frame
        currentFrame->pageNum = pageNumber; // Assign the requested page number

        // Shift the head pointer to point to the next available frame
        if (currentFrame->next != bufferControl->head)
        {
            bufferControl->head = currentFrame->next;
        }
        currentFrame->fixCount++; // Increment fix count since page is now being used
        bufferControl->occupiedCount++; // Track the total occupied frames
    }
    else
    {
        // If the buffer is full, use the LRU algorithm to replace the least recently used page
        currentFrame = bufferControl->tail; // Start replacement from the tail frame

        do
        {
            // If the page is still in use (fix count > 0), move to the next candidate for replacement
            if (currentFrame->fixCount != 0)
            {
                currentFrame = currentFrame->next;
            }
            else
            {
                // Before replacing, check if the page is modified and needs to be written back to disk
                if (currentFrame->dirtyFlag == 1)
                {
                    ensureCapacity(currentFrame->pageNum, &fileHandler);
                    if (writeBlock(currentFrame->pageNum, &fileHandler, currentFrame->data) != RC_OK)
                    {
                        closePageFile(&fileHandler);
                        return RC_WRITE_FAILED; // Error if the write operation fails
                    }
                    bufferControl->numWrite++; // Track the number of writes performed
                }

                // Identify and replace the least recently used page
                if (bufferControl->tail != bufferControl->head)
                {
                    currentFrame->pageNum = pageNumber;
                    currentFrame->fixCount++;
                    bufferControl->tail = currentFrame;
                    bufferControl->tail = currentFrame->next;
                    break;
                }
                else
                {
                    currentFrame = currentFrame->next;
                    currentFrame->pageNum = pageNumber;
                    currentFrame->fixCount++;
                    bufferControl->tail = currentFrame;
                    bufferControl->head = currentFrame;
                    bufferControl->tail = currentFrame->prev;
                    break;
                }
            }
        } while (currentFrame != bufferControl->tail); // Continue searching for a replacable frame
    }

    // Ensure the page file has the required capacity, expanding if needed
    ensureCapacity((pageNumber + 1), &fileHandler);

    // Read the requested page into the allocated frame from disk
    if (readBlock(pageNumber, &fileHandler, currentFrame->data) != RC_OK)
    {
        return RC_READ_NON_EXISTING_PAGE; // Return error if the page is missing
    }

    // Increment the number of pages read from disk
    bufferControl->numRead++;

    // Assign the loaded page details to the page handle
    pageHandle->pageNum = pageNumber;
    pageHandle->data = currentFrame->data;

    // Close the page file after successful retrieval
    closePageFile(&fileHandler);

    return RC_OK; // Successfully pinned the page
}

// Function implementing CLOCK page replacement strategy
RC pinPageCLOCK(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNumber)
{
    // Declare a file handler for disk-related operations
    SM_FileHandle fileHandler;

    // Retrieve buffer pool management metadata
    BM_BufferPool_Mgmt *bufferManager = bufferPool->mgmtData;

    // Pointer to traverse the frames within the buffer pool
    PageFrame *currentFrame = bufferManager->head;
    PageFrame *replacementFrame;

    // Open the required page file from disk
    openPageFile((char*)bufferPool->pageFile, &fileHandler);

    // Check if the requested page is already loaded in the buffer
    do
    {
        // If the page is found, update the frame details and reference bit
        if(currentFrame->pageNum == pageNumber)
        {
            pageHandle->pageNum = pageNumber;
            pageHandle->data = currentFrame->data;

            // Mark the reference bit to indicate recent usage
            currentFrame->refBit = 1; 
            currentFrame->pageNum = pageNumber;
            currentFrame->fixCount++;

            return RC_OK; // Page successfully pinned
        }
        currentFrame = currentFrame->next; // Move to the next frame
    } while(currentFrame != bufferManager->head); // Stop when looping back to the starting frame

    // If there is available space in the buffer, store the page in an empty frame
    if(bufferManager->occupiedCount < bufferPool->numPages)
    {
        currentFrame = bufferManager->head; // Start from the head frame

        currentFrame->pageNum = pageNumber; // Assign the requested page number
        currentFrame->refBit = 1; // Mark the reference bit to indicate usage

        // Shift the head pointer to point to the next available frame
        if(currentFrame->next != bufferManager->head)
        {
            bufferManager->head = currentFrame->next;
        }

        currentFrame->fixCount++; // Increment fix count since the page is now in use
        bufferManager->occupiedCount++; // Track the total occupied frames
    }
    else
    {
        // If the buffer is full, use the CLOCK algorithm to replace a page
        currentFrame = bufferManager->head; // Start from the head pointer

        do
        {
            // Identify a page with a reference bit of 0 for replacement
            if(currentFrame->fixCount != 0)
            {
                currentFrame = currentFrame->next;
            }
            else
            {
                while(currentFrame->refBit != 0)
                {
                    // Reset reference bit to 0 for pages that have been accessed
                    currentFrame->refBit = 0;
                    currentFrame = currentFrame->next;
                }

                // If a page with a reference bit of 0 is found, proceed with replacement
                if(currentFrame->refBit == 0)
                {
                    // Before replacing, check if the page is modified and needs to be written back
                    if(currentFrame->dirtyFlag == 1)
                    {
                        ensureCapacity(currentFrame->pageNum, &fileHandler);
                        if(writeBlock(currentFrame->pageNum, &fileHandler, currentFrame->data) != RC_OK)
                        {
                            closePageFile(&fileHandler);
                            return RC_WRITE_FAILED; // Return error if the write fails
                        }
                        bufferManager->numWrite++; // Track the number of write operations performed
                    }

                    // Update the frame attributes with new page details
                    currentFrame->refBit = 1;
                    currentFrame->pageNum = pageNumber;
                    currentFrame->fixCount++;

                    // Adjust the buffer manager’s head pointer
                    bufferManager->head = currentFrame->next;

                    break; // Exit loop after successful replacement
                }
            }
        } while(currentFrame != bufferManager->head); // Continue searching for a replacable frame
    }

    // Ensure that the file has enough space to accommodate the new page
    ensureCapacity((pageNumber + 1), &fileHandler);

    // Read the requested page into the allocated frame from disk
    if(readBlock(pageNumber, &fileHandler, currentFrame->data) != RC_OK)
    {
        closePageFile(&fileHandler);
        return RC_READ_NON_EXISTING_PAGE; // Return error if the page does not exist
    }

    // Increment the counter tracking the number of pages read from disk
    bufferManager->numRead++;

    // Assign the loaded page details to the page handle
    pageHandle->pageNum = pageNumber;
    pageHandle->data = currentFrame->data;

    // Close the file after successful retrieval of the page
    closePageFile(&fileHandler);

    return RC_OK; // Successfully pinned the page
}


// Function to retrieve an array containing the page numbers currently stored in buffer pool frames
PageNumber *getFrameContents(BM_BufferPool *const bufferPool)
{
    // Access the internal management structure of the buffer pool
    BM_BufferPool_Mgmt *bufferManager;
    bufferManager = bufferPool->mgmtData; // Extract metadata responsible for tracking buffer pool information

    // Allocate memory dynamically to store the page numbers present in buffer frames
    bufferManager->frameContent = (PageNumber *)malloc(sizeof(PageNumber) * bufferPool->numPages);

    // Pointer to traverse the linked list of frames in the buffer pool
    PageFrame *framePtr = bufferManager->start;

    // Retrieve the allocated memory space to store frame content information
    PageNumber *frameContentArray = bufferManager->frameContent;

    int i; // Loop variable for iteration
    int totalPages = bufferPool->numPages; // Store the total number of frames in the buffer pool

    // If memory allocation was successful, populate the array with frame page numbers
    if (frameContentArray != NULL)
    {
        for (i = 0; i < totalPages; i++) // Iterate over all frames in the buffer pool
        {
            frameContentArray[i] = framePtr->pageNum; // Store the page number from the current frame
            framePtr = framePtr->next; // Move to the next frame in the circular linked list
        }
    }

    return frameContentArray; // Return the array containing the page numbers stored in buffer frames
}