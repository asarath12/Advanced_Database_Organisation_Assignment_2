
# Assignment 2
Buffer Manager


## Team Members

Team Members:
1. Anish Roy	    aroy21@hawk.iit.edu
2. Anushka Sarath   asarath@hawk.iit.edu
3. Rebecca Thomson	rthomson@hawk.iit.edu (rst)


Team Contribution:
Anish Roy:
- bool *getDirtyFlags (BM_BufferPool *const bm) 
- int *getFixCounts (BM_BufferPool *const bm) 
- int getNumReadIO (BM_BufferPool *const bm) 
- int getNumWriteIO (BM_BufferPool *const bm) 
- implementing thread safe logic
- merging code and running tests

    

Anushka Sarath:
- RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page) 
- RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page) 
- RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum) 
- PageNumber *getFrameContents (BM_BufferPool *const bm) 

    

Rebecca Thomson:
- RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, 	const int  numPages, ReplacementStrategy strategy, void *stratData) 
- RC shutdownBufferPool(BM_BufferPool *const bm) 
- RC forceFlushPool(BM_BufferPool *const bm) 
- RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page) 






## Folder structure

The assign2 folder contains the following files:
- dberror.h: This file contains the error constants defined with RC error codes. 2 macros THROW and CHECK are defined for throwing and checking error.
- dberror.c: This file gives the method definition of the printError and errorMessage methods declared in dberror.h.
- storage_mgr.h: This file defines a structure for SM_FileHandle with its fields and also defines SM_PageHandle. This file declares methods for file manipulation, read and writing files.
- storage_mgr.c: This file provides method definition to all the methods declared in storage_mgr.h for file manipulation, reading and writing in files.
- buffer_mgr.h: This file defines a structure for ReplacementStrategy, BM_BufferPool and BM_PageHandle. This file declares methods for Buffer Manager Interface Pool Handling, Buffer Manager Interface Access Pages and Statistics Interface.
- buffer_mgr.c: This file provides method definition to all the methods declared in buffer_mgr.h for Buffer Manager Interface Pool Handling, Buffer Manager Interface Access Pages and Statistics Interface.
- test_helper.h: This file defines set of macros used for testing purpose.
- dt.h: This file also defines set of macros used for testing purpose.
- test_assign2_1.c: This file is used for defining testcases such as testCreatingAndReadingDummyPages, testReadPage, testFIFO, testLRU
- test_assign2_2.c: This file is used for defining testcases such as testCreatingAndReadingDummyPages, testReadPage, testClock
- buffer_mgr_stat .c: This file declares many functions for displaying buffer or page content to console.
- buffer_mgr_stat .h: This file provides implementation for the functions declared in buffer_mgr_stat.h
- Makefile: This Makefile is used for compiling and running two test programs (test1 and test2). It includes options for debugging (-g) and enabling all compiler warnings (-Wall). Additionally, it links the pthread library (-pthread) to enable multithreading support. 
            The Makefile also defines rules for compiling individual source files and cleaning up generated files.
### ReplacementStrategies Implemented:
- FIFO
- LRU
- LFU
- CLOCK

### ThreadSafe:
Also, we have made the buffer pool functions thread safe by adding pthread_mutex_lock and pthread_mutex_unlock in appropriate places implementing it threadsafe.

### Output PDF:
We have attached a PDF in assign2 directory called, which contains the screenshots of successful execution of test_assign2_1 and test_assign2_2 with threadSafe implementation.

## Functions

__RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData):__

This method initializes the pageFrame and the buffer pool.

__RC shutdownBufferPool(BM_BufferPool *const bm)__

This method is used to free the memory or resources allocated for pageframe.

__RC forceFlushPool(BM_BufferPool *const bm)__

This method is responsible for writing the pages for which the dirtyFlag is set to the disk.

__RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page)__

This method is used to set the dirtyFlag, which will be used further for reflecting the changes in disk.

__RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page)__

This method is used to store the page contents from the page file to the disk.

__RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum)__

This method implements threadSafe and pins the page with page number pageNum where the role of the buffer manager is to set the ensureCapacity and then read the block from the memory with the
pageNum field of the page handle passed to the method. Additionally if the bufferpool is full, then a new page is being referenced with the data field pointing to
the page frame where the page is stored in (the area in memory storing the content of the page).

__RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page)__

This method is used for unpinning the page with threadSafe. The pageNum field implemented in this method checks which page to unpin and which page to not unpin.

__PageNumber *getFrameContents (BM_BufferPool *const bm)__

This method is used to get an array of PageNumbers which contains the data in a specific page frame. If the page frame is empty, NO_PAGE error is thrown.

__bool *getDirtyFlags (BM_BufferPool *const bm)__

This method contains a boolean operator if the page is stored in a specific page frame with the dirtyFlag variable of the page is true, which is later returned. The method will return false if the dirtyFlag is also false.

__int *getFixCounts (BM_BufferPool *const bm)__

This method implements an array of integers, which stores the fix count of the page in the respective page frame. If the fix count is 0, then the value stored in the array is also 0.

__int getNumReadIO (BM_BufferPool *const bm)__

This method returns a rearIndex of 1 whenever a page is being read from disk after initializing a buffer pool.

__int getNumWriteIO (BM_BufferPool *const bm)__

This method returns a writeCount, which is the no of pages written to the page file after initializing a buffer pool.

## Commands to execute the code:

The testing has been done in the linux/unix enironment (Ubuntu 20.04 OS).
The below mentioned commands should be executed to rcompile and execute the test cases.
Navigate to the folder where the files are present. Run the below shonw commands:

(1) 
make clean

(2) 
make
(3)
chmod -R 777 <fullFilepath_of_the_directory_where_files_are_present>
Example: filepath of directory=/home/user1/Downloads/vscode/assignment_2/4/assign2
Commands issued:
chmod -R 777 /home/user1/Downloads/vscode/assignment_2/4/assign2

(4)run the test case 1 and output in the file 1.txt:

./test1 0>1.txt 1>1.txt 2>1.txt

(5)run the test case 2 and output in the file 2.txt:
./test2 0>2.txt 1>2.txt 2>2.txt

