Team Members:
1. Anish Roy	    aroy21@hawk.iit.edu
2. Anushka Sarath   asarath@hawk.iit.edu
3. Rebecca Thomson	rthomson@hawk.iit.edu A20548618


Team Contribution for Project 2:
Anish Roy:
    

Anushka Sarath:
    

Rebecca Thomson:
initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData)
shutdownBufferPool(BM_BufferPool *const bm)
forceFlushPool(BM_BufferPool *const bm)
markDirty(BM_BufferPool *const bm, BM_PageHandle *const page)
adding to structures BM_BufferPool and BM_PageHandle
adding function create_bm_page_handle(BM_BufferPool *bufferPool, int frameNum, int maxFrameNum)


Folder structure
The assign2_storage_manager folder contains the following files:
-buffer_mgr_stat.c: Provides several functions for outputting buffer or page content to stdout or into a string.
-buffer_mgr_stat.h: Provides several functions for outputting buffer or page content to stdout or into a string.
-buffer_mgr.c: This file provides method definition to all methods declared in buffer.mgr.h, as well as supporting methods to create, modify, and close a buffer pool as well as provide statistical tracking of the buffer pool use.
-buffer.mgr.h: This file defines the structures BM_PageHandle and BM_BufferPool.  It also declares methods associated with the buffer pool.  It declares two macros.
- dberror.h: This file contains the error constants defined with RC error codes. 2 macros THROW and CHECK are defined for throwing and checking error.
- dberror.c: This file gives the method definition of the printError and errorMessage methods declared in dberror.h.
-dt.h:
- storage_mgr.h: This file defines a structure for SM_FileHandle with its fields and also defines SM_PageHandle. This file declares methods for file manipulation, read and writing files.
- storage_mgr.c: This file provides method definition to all the methods declared in initStorageManager.h for file manipulation, reading and writing in files.
- test_helper.h: This files defines set of macros used for testing purpose.
- test_assign1_1.c: This file has 2 methods testCreateOpenClose and testSinglePageContent which is used for testing the functionality of the program.
-test_assign2_1.c: 
-test_assign2_2.c:
- Makefile: This file is used to create binary test_assign1 from test_assign1_1.c and it is executed when make run command is executed. 
            The file also declares the output fileName after compiling and executing the above c files, which are db_error.o, storage_mgr.o and test_assign1_1.o.


Functions:
//Buffer Manager Interface Pool Handling
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData);
This is a method that initializes the buffer pool and the page table.  The page table is a linked list.

RC shutdownBufferPool(BM_BufferPool *const bm);
This method shuts down the buffer pool and page table.  It returns an error if there are any pinned pages in the page table.

RC forceFlushPool(BM_BufferPool *const bm);
This method writes all dirty pages that are not pinned to disk that are in memory in the buffer pool.

Void create_bm_page_handle(BM_BufferPool *bufferPool, int frameNum, int maxFrameNum); This is a helper method created to create BM_PageHandle objects for use in the Page Table of the Buffer.  It initializes each object with a dirty/clean flag, pinned count, frame number, and space for the page number (from the file) and the page data saved to memory.

// Buffer Manager Interface Access Pages
RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page);
This method will mark the page as dirty in the page table that matches the given page.  If no page is found, it will return an error.

RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page);
This method unpins a page in the buffer pool.

RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page);
This method writes the page back to disk and marks the page as clean.

RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page, 
		const PageNumber pageNum);
This method pins the given page.  The pin number can be greater than 0. 


// Statistics Interface
PageNumber *getFrameContents (BM_BufferPool *const bm);
The method returns an array of page numbers of size numPages (the size of the buffer) listing which pages are currently in the buffer.  And empty page frame is shown as NO_PAGE.

bool *getDirtyFlags (BM_BufferPool *const bm);
The method returns an array of Booleans of size numPages (the size of the buffer) listing if the pages are dirty or not.  Empty pages are considered clean.

int *getFixCounts (BM_BufferPool *const bm);
The method returns an array of ints of size numPages (the size of the buffer) listing the number of pins, or fix counts, for each page in the buffer.  Empty frames are shown as 0.

int getNumReadIO (BM_BufferPool *const bm);
The total count of pages read since the buffer’s initialization to this point.

int getNumWriteIO (BM_BufferPool *const bm);
The total count of pages written since the buffer’s initialization to this point.


(These are functions from Part 1 of the Project)
extern void initStorageManager (void)
This method initializes the storage manager program.

extern RC createPageFile (char *fileName)
This method is used to create a new page file using the fileName specified as parameter.

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle)
This method opens the existing page file, if the file not found returns RC_FILE_NOT_FOUND error. After opening the file, 
information about the file is initialized in fHandle fields. 

extern RC closePageFile (SM_FileHandle *fHandle)
This method is used to close the file, return error if there is any error in closing the file.

extern RC destroyPageFile (char *fileName)
This method is used to destroy or delete the file using remove method and if this method returns -1 ,
return error in destroying the file.

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
This method is used to read the block for the specified page number. Return RC_READ_NON_EXISTING_PAGE error 
if the pageNum given is less than 0 or greater than the total number of pages. This method calclates the offset
to access the required pageNum, seek to the required position and tries to read the contents and stores it 
in memPage. If there is error in seeking the required position or in reading, returns approriate error.

extern int getBlockPos (SM_FileHandle *fHandle)
This method returns the current position of the page in file handle.

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the contents of the first page and stores it in memPage.

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the previous page relative to the current page of the file and stores it in memPage.

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This methods reads the contents of the current page and stores it in memPage.

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the next page relative to the current page of the file and stores it in memPage.

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the contents of the last page and stores it in memPage.

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
This method writes the contents from memPage to the requested pageNum. 
Return RC_WRITE_FAILED error if the pageNum given is less than 0 or greater than the total number of pages. 
This method calclates the offset to access the required pageNum, seek to the required position 
and tries to write the contents from the in memPage to the requested pageNum. 
If there is error in seeking the required position or in writing, returns approriate error.

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method is used to write the contents of the memPage and store it in the current page. Returns approriate errors if 
there is any problem in seeking the required position or in writing the contents.

extern RC appendEmptyBlock (SM_FileHandle *fHandle)
This method is defined to increase the number of pages. Here, the file is seeked to the end and 
an empty block is allocated and written, also incrementing the number of pages by 1.

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
This method makes sure if the number of pages in the file is less than the requested number of pages. 
If so, appends possible number of extra pages towards end of the file.



To run the file, navigate to the directory where the code files are present and then run the following commands:
make
make run

The output of running the file is shown in outputFile2.txt.


