//
//
//


// Include return codes and methods for logging errors
#include "dberror.h"
// Include bool DT
#include "dt.h"
//include header file
#include "buffer_mgr.h"

#include <stdlib.h>

#include "storage_mgr.h"

//global variable filePointer declaration
FILE *filePointer;
BM_PageHandle (*frame_array)[];
/*Creating a single page of the Page Table for the buffer pool.
*This function will be called by initBufferPool

*/
void create_bm_page_handle(BM_BufferPool *bufferPool, int frameNum, int maxFrameNum) {
    //creating single frame and reserving memory to it
    BM_PageHandle *disFrame = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
    //initialize default values of properties
    disFrame->dirty = 0;
    disFrame->pinned=0;
    disFrame->pageNum=-1;
    disFrame->frameNumber=frameNum; //The individual frame's number.  For iteration.
    //the location where the actual memory of the data taken from the file is kept.

    disFrame->data = (SM_PageHandle) calloc(PAGE_SIZE,sizeof(char*));
    if (frameNum ==0) {
        bufferPool->first = disFrame;// setting head frame
        bufferPool->current = disFrame;//setting current
        bufferPool->last = disFrame;// setting last, will overwrite.
    }else {
        bufferPool->last-> next= disFrame; //overwrite last frame's next frame with this one
        disFrame->prev = bufferPool->last;//write on this Frame's prev Frame with currently shown last Frame
        bufferPool->last-> next= disFrame; //overwrite old values to the new last frame.
    }
    //Now, turning the double linked list into a circle.
    bufferPool->last->next = bufferPool->first; //point to the head
    bufferPool->first->prev = bufferPool->last; //point head back to tail
    //printf("Size of this frame: %d\n",sizeof(disFrame));
    //return disFrame;
}


// Buffer Manager Interface Pool Handling
/*This function creates the new buffer pool.  Within this instance, a total of NumPages are tracked
 *using the page replacement strategy.  This pool caches pages from pageFileName.
 *Initially, this produces an empty Buffer Pool with a circular double-linked list
 *Page Table.
 *This does NOT generate a new pageFileName.
 *stratData is used to pass info for page replacement in ReplacementStrategy.
 *bm-> stores the management Data
 *pageFileName -> name of originating file that will be loaded
 *numPages -> number of frames in the buffer pool
 *strategy -> the code for the selected Page Replacement Strategy
 *stratData -> Other parameters if needed for selected Page Replacement Strategy
 *first, last, current -> pointers to PageHandles on the Page Table.
 */
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
        const int numPages, ReplacementStrategy strategy,
        void *stratData) {
    printf("======================================\n");
    printf("Initializing Storage Manager Buffer!\n");
    printf("======================================\n");
    filePointer = NULL;
    // check that pageFileName exists
    //printf("log entry: initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData)\n");
    filePointer = fopen(pageFileName, "r+");
    int one=1;
    int zero=0;
    // Check and return file not found if the file is not present
    if (filePointer == NULL)
    {
        printf("file pointer is null \n");
        return RC_FILE_NOT_FOUND;
    }
    //Allocating memory for the Buffer Pool data structure
    BM_BufferPool *bmp = (BM_BufferPool *)malloc(sizeof(BM_BufferPool));

    //file handle for storage manager
    SM_FileHandle fHandle;
    //open the original file
    openPageFile((char*)pageFileName, &fHandle);
    printf("The number of page frames in buffer: %d\n", numPages);
    printf("The size of buff: %d\n", sizeof(bmp));
    //if (numPages != 0){printf("So far");}
    //initializing Frames in Page Table
    //fill frames by iteration.
    for (int i = 0; i < numPages; i++) {
        create_bm_page_handle(bmp, i, numPages);
    }
    printf("The page frames are initialized.\n");

    //declare pointer to Page table for later. (//todo make global variable)
    //BM_PageHandle (*frame_array)[numPages]=&page_in_frame;

    bmp->last = bmp->first;
    bm->strategy=strategy;
    bm->numPages=numPages;
    bm->pageFile=(char*) pageFileName;
    bm->total_num_reads=0;//Initial number of total number of reads during the life of the buffer.
    bm->total_num_writes=0;//Initial number of total number of writes during the life of the buffer.
    bm->mgmtData=bmp;//linking const with this instance
    printf("The size of buff: %d\n", sizeof(bmp));
    //close file
    fclose(filePointer);

    //close the page file to prevent memory leakage
    //closePageFile(&fHandle);
    return RC_OK;
}


/*This function destroys the Buffer Pool.
 *todo Determine if we are to unpin pages if this function is called, or just return an error.
 *If pinned pages exist, return an error /or/ Unpin pages if required.
 *All dirty pages are written to disk.
 *All resources are freed.
 *Return RC_OK when process finished.
 *
 */
RC shutdownBufferPool(BM_BufferPool *const bm) {
    printf("=============================================\n");
    printf("Initializing Storage Manager Buffer Shutdown!\n");
    printf("=============================================\n");

    //Check for pinned pages//
    // load data
    BM_BufferPool *bmp;
    bmp = bm->mgmtData;//linking const and instance
    //check link exists
    if (bmp == NULL) {return RC_BUFFER_SHUTDOWN_FAILED;}
    //start at first node of page Table
    BM_PageHandle *disFrame = bm->first;
    SM_FileHandle fHandle;

    //open page file first
    if (openPageFile ((char *)(bm->pageFile),&fHandle)!=RC_OK) {
        return RC_BUFFER_SHUTDOWN_FAILED;
    }
    // Iterate through linked list of Page Table, checking for pinned frames
    do {
        if (disFrame->pinned!=0) {
            //If pinned, stop
            printf("The page in frame %d is pinned\n",disFrame->frameNumber);
            printf("Shutdown canceled\n");
            return RC_PINNED_PAGES_EXIST;
        }
        disFrame=disFrame->next;//step to next frame
    }while (disFrame!=bm->last);
    //close for memory leaks
    closePageFile(&fHandle);

    //call forceFlushPool to write all dirty pages to disk
    if (forceFlushPool(bm)!=RC_OK) { //todo check attributes of call
        return RC_BUFFER_SHUTDOWN_FAILED;//error code for
    }else {
        //Free all resources
        bmp->first = NULL;
        bmp->last = NULL;
        bmp->current = NULL;
        free(bmp);
        free(disFrame);
        bm->mgmtData=NULL;
        bm->pageFile=NULL;
        bm->numPages=0;
        //bm->strategy=NULL;
        //Return RC_OK when finished.
        return RC_OK;
    }
    //}
    //if something else happens, let's return an error:
    //return RC_BUFFER_SHUTDOWN_FAILED;
}
/*This function causes all dirty pages with a pin count of 0 to be
 *written to disk.
 *If pin count !=0, it does not change the dirty marker, nor write the page, even if it
 *is dirty. (The page is in use)
 *Iterates through page table, writing dirty pages to disk.
 *Set dirty flag to 0.
 *Return RC_OK when process finished.
 *
 */
RC forceFlushPool(BM_BufferPool *const bm) {
    // load data
    BM_BufferPool *bmp;
    bmp = bm->mgmtData; //linking const and instance
    //check link exists
    if (bmp == NULL) {return RC_FLUSH_FAILED;}
    //start at first node of page Table
    BM_PageHandle *disFrame = bm->first;
    SM_FileHandle fHandle;

    //open page file first
    if (openPageFile ((char *)(bm->pageFile),&fHandle)!=RC_OK) {
        return RC_FLUSH_FAILED;
    }
    // Iterate through linked list of Page Table, if pinned = 0 and dirty !=0, write page to disk.
    do {
        if (disFrame->dirty==0 && disFrame->pinned==0) {
            //write to Block
            if (writeBlock(disFrame->pageNum,&fHandle, disFrame->data)!=RC_OK){return RC_FLUSH_FAILED;}
            //set dirty=0
            disFrame->dirty=0;
        }
        disFrame=disFrame->next;//step to next frame
    }while (disFrame!=bm->last);
    //close for memory leaks
    closePageFile(&fHandle);

    return RC_OK;
}

// Buffer Manager Interface Access Pages
/*This function will mark a single page as dirty.
 *Used when page is modified for any reason.
 */
RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page){
    BM_BufferPool *bmp;
    bmp = bm->mgmtData; //linking const and instance
    //check link exists
    if (bmp == NULL) {return RC_ERROR;}
    //start at first node of page Table
    BM_PageHandle *disFrame = bm->first;
    SM_FileHandle fHandle;

    //open page file first
    if (openPageFile ((char *)(bm->pageFile),&fHandle)!=RC_OK) {
        return RC_ERROR;
    }
    // Iterate through linked list of Page Table, if pinned = 0 and dirty !=0, write page to disk.
    do {
        if (disFrame==page) {

            //set dirty=
            disFrame->dirty=1;
            return RC_OK;
        }
        disFrame=disFrame->next;//step to next frame
    }while (disFrame!=bm->last);
    //close for memory leaks
    closePageFile(&fHandle);
    //because page not found,
    return RC_PAGE_NOT_IN_BUFFER;

}
/*This function is called when the client is finished using a page.
 *A client is finished using a page when all modifications are complete,
 *and the client can not access any editing of the page in memory.
 *NOTE: Because more than one client may have access to the page at a time,
 *the fix count could be greater than one.  Therefore, this function
 * only reduces the fix count by 1.
 */
RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page){
//Verify that the fix count is greater than 0.
    //Subtract -1 from fix count
    return RC_OK;
}

/*This function calls writeBlock to write the given page to disk.
 *Sets dirty to 0
 *
 */
RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page){
    //call writeBlock to write page to disk.
    // set dirty to 0

    //On success, return
    return RC_OK;
}

/*This function pins the page of page number pageNum.
 *
 *
 */
RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page,
const PageNumber pageNum){

    return RC_OK;
}


// Statistics Interface
PageNumber *getFrameContents (BM_BufferPool *const bm){

    return RC_OK;
}

bool *getDirtyFlags (BM_BufferPool *const bm){

    return RC_OK;
}

int *getFixCounts (BM_BufferPool *const bm){

    return RC_OK;
}

int getNumReadIO (BM_BufferPool *const bm){
    //bm->total_num_reads //the information looking for
    return RC_OK;
}

int getNumWriteIO (BM_BufferPool *const bm){

    return RC_OK;
}

