#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

// Include return codes and methods for logging errors
#include "dberror.h"

// Include bool DT
#include "dt.h"

// Replacement Strategies
typedef enum ReplacementStrategy {
	RS_FIFO = 0,
	RS_LRU = 1,
	RS_CLOCK = 2,
	RS_LFU = 3,
	RS_LRU_K = 4
} ReplacementStrategy;

// Data Types and Structures
typedef int PageNumber;
#define NO_PAGE -1
/*This is the structure for each Frame in the Page Table.
 *It has pointers to the next,previous frame in the Page Table.
*/

typedef struct BM_PageHandle {
	PageNumber pageNum;//the page number of the buffered file in this frame spot, this will allow quick lookups into buffer pool.
	char *data; //location pointer to the memory of page in buffer pool
	int dirtyFlag; //Flag to mark if page is dirty (0= clean)
	int fixCount; //count to track number of users who have pinned this page
	int frameNum; //the number of this Frame in the Buffer.  The total is given elsewhere.
	int refBit;			// Used for CLOCK algorithm to identify recently accessed pages
	struct BM_PageHandle *next, *prev;
} BM_PageHandle;
/*This is the structure for the Buffer Pool.  It has pointers to the Page Table
 *
 */

typedef struct BM_BufferPool {
	int occupiedCount;		// Tracks the number of frames currently occupied in the buffer
	char *pageFile; //The file that the pages come from
	int numPages; //Total number of Pages in the Buffer Pool
	ReplacementStrategy strategy;
	int numRead; //Total number of times any page has been read.  Will initiate at zero.
	int numWrite; //Total number of times any page has been written.  Will initiate at zero.
	void *mgmtData; // use this one to store the bookkeeping info your buffer
	PageNumber *frameContent;	// Array storing the page numbers currently loaded in frames
	// manager needs for a buffer pool
	BM_PageHandle *head,*tail, *start; //Pointers to the first, last, and current frame in the Page Table
} BM_BufferPool;


// convenience macros
#define MAKE_POOL()					\
		((BM_BufferPool *) malloc (sizeof(BM_BufferPool)))

#define MAKE_PAGE_HANDLE()				\
		((BM_PageHandle *) malloc (sizeof(BM_PageHandle)))

// Buffer Manager Interface Pool Handling
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, 
		const int numPages, ReplacementStrategy strategy,
		void *stratData);
RC shutdownBufferPool(BM_BufferPool *const bm);
RC forceFlushPool(BM_BufferPool *const bm);

// Buffer Manager Interface Access Pages
RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page);
RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page);
RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page);
RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page, 
		const PageNumber pageNum);

// Statistics Interface
PageNumber *getFrameContents (BM_BufferPool *const bm);
bool *getDirtyFlags (BM_BufferPool *const bm);
int *getFixCounts (BM_BufferPool *const bm);
int getNumReadIO (BM_BufferPool *const bm);
int getNumWriteIO (BM_BufferPool *const bm);

#endif
