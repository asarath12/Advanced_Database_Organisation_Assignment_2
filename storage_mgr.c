#include "storage_mgr.h"
#include <sys/stat.h>
#include "dberror.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>



//global variable filePointer declaration
FILE *filePointer;


/*
 * Function: initStorageManager
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input: 
 *    no input parameter
 * Output: 
 *    void
 */
extern void initStorageManager()
{
    printf("======================================\n");
    printf("extern void initStorageManager()\n");
    printf("Initializing Storage Manager !\n");
    filePointer = NULL;
}


/*
 * Function: createPageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input: 
 *    fileName : pointer to the filename of the file to be created.
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC createPageFile(char *fileName)
{
    printf("log entry: extern RC createPageFile(char *fileName)\n");
    // Opening file stream in read & write mode. 
    //'w+' mode creates an empty file for both reading and writing.
    filePointer = fopen(fileName, "w+");
    // Checking if the file pointer is Null
    if (filePointer == NULL)
    {
        RC_message = "Failure encountered in the file creation";
        return  RC_ERROR;
    }
    int zero=0;
    int one=1;
    int charSize = sizeof(char);
    // Setting memory for an empty page
    SM_PageHandle smPageHandlePtr = (SM_PageHandle)calloc(PAGE_SIZE, (charSize * one));

    // Returns error code if the fwrite does not write properly
    if (fwrite(smPageHandlePtr, charSize * one, PAGE_SIZE, filePointer) >= PAGE_SIZE)
    {
        // Seeking to the end of file using SEEK_END
        fseek(filePointer, zero, SEEK_END);

        // Closing the file 
        //pointed to by filePointer
        fclose(filePointer);

        // Free or releasing the memory 
        //held by smPageHandlePtr
        free(smPageHandlePtr);

        RC_message = "Creation of file is successful";
        return RC_OK;
    }
    else 
    {
        RC_message = "Failure in writing the file";
        return RC_WRITE_FAILED;
    }
}




/*
 * Function: openPageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input: 
 *    fileName : pointer to the filename of the file to be created.
 *    fHandle  : pointer to SM_FileHandle object
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
    printf("log entry: extern RC openPageFile(char *fileName, SM_FileHandle *fHandle)\n");
    filePointer = fopen(fileName, "r+");
    int one=1;
    int zero=0;
    // Check and return file not found if the file is not present
    if (filePointer == NULL)
    {
        printf("file pointer is null \n");
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        if (fseek(filePointer, zero, SEEK_END) != 0) 
        {
            fclose(filePointer);
            return RC_ERROR;
        }

        // Stores the size of file in a variable
        int sizeOfFile = ftell(filePointer);
        printf("FileSize (sizeOfFile) = %d\n", sizeOfFile);
        
        // Checks if the ftell returns -1, if so return error
        if (sizeOfFile == -1) 
        {
            fclose(filePointer);
            return RC_ERROR;
        }  

        int numberOfPages = (sizeOfFile * one) / PAGE_SIZE;
        printf("Total Number of Pages (numberOfPages)= %d\n", numberOfPages);
        printf("The file name is (fileName) = %s\n", fileName);

        // Setting the fHandle->fileName
        fHandle->fileName = strdup(fileName);  // Deep copy to avoid pointer issues

        // Setting the fHandle->mgmtInfo
        fHandle->mgmtInfo = filePointer;

        // Setting the total number of pages(fHandle->totalNumPages)
        fHandle->totalNumPages = numberOfPages * one;

        // Setting the current page position(fHandle->curPagePos)
        fHandle->curPagePos = zero;

        // Rewinding the file pointer to beginning of the file
        rewind(filePointer);
        // return OK code
        return RC_OK;
    }
}



/*
 * Function: closePageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input: 
 *    fHandle  : pointer to SM_FileHandle object
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC closePageFile(SM_FileHandle *fHandle)
{
    printf("log entry: extern RC closePageFile(SM_FileHandle *fHandle)\n");
    if (fHandle == NULL || fHandle->mgmtInfo == NULL) 
    {
        printf("fHandle == NULL || fHandle->mgmtInfo == NULL \n");
        RC_message = "Invalid file handle. File handle not initialized";
        return RC_ERROR;
    }
    // Check and return error if the fclose returns EOF
    if (fclose((FILE *)fHandle->mgmtInfo) == EOF)
    {
        printf("fclose((FILE *)fHandle->mgmtInfo) == EOF \n");
        RC_message = "Error encountered in closing of the file";
        return RC_ERROR;
    }
    else
    {
        printf("The file was closed successfully \n");
        RC_message = "The file closed successfully";
        return RC_OK;
    }
}


/*
 * Function: destroyPageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input: 
 *    fileName : pointer to the filename of the file to be created.
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */

extern RC destroyPageFile(char *fileName)
{
    printf("log entry: extern RC destroyPageFile(char *fileName)\n");
    // Storing the result returned by remove function
    int removeFileResult = remove(fileName);
    int negOne = -1;
    // Return error if res is negOne
    if (removeFileResult == negOne)
    {
        printf("removeFileResult == Failure. The file could not be deleted\n");
        RC_message = "Error occurred while destroying file";
        return RC_ERROR;
    }
    RC_message = "File destroyed successfully";
    return RC_OK;
}



/*
 * Function: readBlock
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * blockNumber: The page number to read (starting from 0).
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 * 
 * returns: RC_OK if successful, error code otherwise.
 */
RC readBlock(int blockNumber, SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    // Check if the block number is invalid (negative or beyond total number of pages)
    if (blockNumber < 0 || blockNumber >= fileHandle->totalNumPages) {
        return RC_READ_NON_EXISTING_PAGE; // Invalid page access
    } else {
        // Open the file in read-only mode
        int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
        if (fileDescriptor < 0) // If file couldn't be opened
            return RC_FILE_NOT_FOUND;

        // Move the file pointer to the starting byte of the desired page
        int offset = lseek(fileDescriptor, (PAGE_SIZE * blockNumber), SEEK_SET);
        if (offset >= 0) {
            read(fileDescriptor, memoryPage, PAGE_SIZE); // Read PAGE_SIZE bytes into memoryPage
            fileHandle->curPagePos = blockNumber;        // Update the current page position
            close(fileDescriptor);                       // Close the file after reading
            return RC_OK;                                // Successful read
        } else {
            close(fileDescriptor);                       // Ensure the file is closed even on failure
            return RC_READ_NON_EXISTING_PAGE;            // Page offset could not be set
        }
    }
}

/*
 * Function: getBlockPos
 * ---------------------
 * Retrieves the current position of the file pointer (current page number).
 * 
 * fileHandle: Pointer to the file handle structure.
 * 
 * returns: The current page position in the file.
 */
int getBlockPos(SM_FileHandle *fileHandle) {
    return fileHandle->curPagePos;
}

/*
 * Function: readFirstBlock
 * ------------------------
 * Reads the first block (page) of the file into memory.
 * 
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 * 
 * returns: RC_OK if successful, error code otherwise.
 */
RC readFirstBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY); // Open file for reading
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND; // File does not exist or cannot be opened

    int offset = lseek(fileDescriptor, 0, SEEK_SET); // Move to the start of the file (offset 0)
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the first page
        fileHandle->curPagePos = 0;                  // Set current page position to 0
        close(fileDescriptor);                       // Close the file
        return RC_OK;
    } else {
        close(fileDescriptor);                       // Close file even if seeking fails
        return RC_READ_NON_EXISTING_PAGE;            // Error seeking to the first page
    }
}

/*
 * Function: readPreviousBlock
 * ---------------------------
 * Reads the previous block relative to the current page position.
 * 
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 * 
 * returns: RC_OK if successful, error code otherwise.
 */
RC readPreviousBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Check if current page is the first page (no previous block exists)
    if (fileHandle->curPagePos == 0) {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;
    }

    // Move the file pointer to the previous block
    int offset = lseek(fileDescriptor, (fileHandle->curPagePos - 1) * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the previous block
        fileHandle->curPagePos--;                    // Update current page position
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;            // Error seeking to the previous page
    }
}

/*
 * Function: readCurrentBlock
 * --------------------------
 * Reads the current block (page) based on the current page position.
 * 
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 * 
 * returns: RC_OK if successful, error code otherwise.
 */
RC readCurrentBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Move the file pointer to the current page's position
    int offset = lseek(fileDescriptor, fileHandle->curPagePos * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the current page
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;            // Error reading current page
    }
}

/*
 * Function: readNextBlock
 * -----------------------
 * Reads the next block relative to the current page position.
 * 
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 * 
 * returns: RC_OK if successful, error code otherwise.
 */
RC readNextBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Check if we are already at the last page
    if (fileHandle->curPagePos >= fileHandle->totalNumPages - 1) {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;
    }

    // Move to the next page
    int offset = lseek(fileDescriptor, (fileHandle->curPagePos + 1) * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the next page
        fileHandle->curPagePos++;                    // Update the current page position
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;            // Error seeking to the next page
    }
}

/*
 * Function: readLastBlock
 * -----------------------
 * Reads the last block (page) of the file into memory.
 * 
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 * 
 * returns: RC_OK if successful, error code otherwise.
 */
RC readLastBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Move the file pointer to the start of the last block
    int offset = lseek(fileDescriptor, (fileHandle->totalNumPages - 1) * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE);         // Read the last block
        fileHandle->curPagePos = fileHandle->totalNumPages - 1; // Update position to last page
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;                    // Error reading the last page
    }
}



/*
 * Function: writeBlock
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * Input: 
 *    pageNum: The page number to write.
 *    fHandle: Pointer to the file handle structure.
 *    memPage: Memory location where the data will be stored.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */


extern RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    printf("pageNum=%d \n", pageNum);
    int one=1;
    // Set the value for the filePointer
    filePointer = (FILE *)fHandle->mgmtInfo;
    // If the pageNum is less than 0 
    // or greater than total number of pages return error
    if ( pageNum > ((fHandle->totalNumPages) * one) || pageNum < 0)
    {
        printf("Trying to read data from a page which is non-existent. \n");
        RC_message = "Trying to read data from a page which is non-existent.";
        return RC_WRITE_FAILED;
    }
    // Returns error when filePointer is null
    if (filePointer == NULL)
    {
        printf("filePointer value is NULL \n");
        RC_message = "filePointer value is NULL";
        return RC_FILE_NOT_FOUND;
    }
    // Getting the size of char data type
    int charSize = sizeof(char);
    // Calculating the position in the file for the requested pageNum
    int offsetValue = pageNum * PAGE_SIZE * charSize * one ;
    // Checking if the fseek returns a number other than 0, 
    // if so, there is error in seeking the correct position
    if (fseek(filePointer, offsetValue, SEEK_SET) != 0)
    {
        printf("Error encountered in seeking the correct offset position in the file ! \n");
        RC_message = "Error encountered in seeking the correct offset position in the file !";
        return RC_WRITE_FAILED;
    }
    // Writing the contents from memPage to the requested pageNum
    //fwrite(memPage, sizeof(char), PAGE_SIZE, filePointer);
    fwrite(memPage, (charSize * one), PAGE_SIZE, filePointer);
    printf("Writing of the data is successful. \n");
    RC_message = "Writing of the data is successful";
    return RC_OK;
}



/*
 * Function: writeCurrentBlock
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * Input: 
 *    fHandle: Pointer to the file handle structure.
 *    memPage: Memory location where the data will be stored.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */


extern RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    // get block position
    int one=1;
    int blockPosition = getBlockPos(fHandle); 
    blockPosition = blockPosition * one;
    // write the data contents from the memPage variable 
    // into the disk at the location of 
    //the current position of fHandle variable
    return writeBlock(blockPosition, fHandle, memPage);
}



/*
 * Function: appendEmptyBlock
 * -------------------
 * Adds an empty page onto the end of an existing file.  This page shall have
 * zero bytes.  Increases the totalNumPages by one
 * 
 * Input: 
 *    fHandle: Pointer to the file handle structure.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */

extern RC appendEmptyBlock(SM_FileHandle *fHandle)
{
    printf("log entry: appendEmptyBlock method \n");
    int one = 1;
    int zero = 0;
    // Check to see the file handle is null or not.
    // If file handle is null, return proper error message
    if(fHandle == NULL) 
    {
        printf("File handle has not been correctly initialized, and it is null \n");
        RC_message = "File handle has not been correctly initialized, and it is null";

        return RC_FILE_HANDLE_NOT_INIT ;
    }
    // Setting the filePointer
    filePointer = (FILE *)fHandle->mgmtInfo;

    // If mgmtInfo inside the fHandle is null,
    // then return error
    if (filePointer == NULL)
    {
        printf("The file not found error \n");
        RC_message = "The file not found during append.";
        return RC_FILE_NOT_FOUND;
    }
    // If fseek  does not return 0
    // return proper error message

    if (fseek(filePointer, zero, SEEK_END) != 0)
    {
        printf("fseek of the file has failed. \n");
        RC_message = "fseek of the file during append has failed";
        return RC_APPEND_FAILED;
    }
    // Allocate a block of size equal to PAGE_SIZE
    // and initialize it to an empty pointer
    SM_PageHandle sm_PageHandle_ptr = (SM_PageHandle)calloc(PAGE_SIZE, one);

    // Initialize  an empty block using fwrite
    fwrite(sm_PageHandle_ptr, one, PAGE_SIZE, filePointer);

    // fSeek to the end of the file
    fseek(filePointer, zero, SEEK_END);

    // Increment the total number of pages
    // in fHandle by 1
    fHandle->totalNumPages = fHandle->totalNumPages + one;
    return RC_OK;
}



/*
 * Function: ensureCapacity
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * Input: 
 *    numberOfPages: number of pages.
 *    fHandle: Pointer to the file handle structure.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */

extern RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
    printf("log entry: ensureCapacity method \n");
    int one=1;
    // Check to see if the capacity 
    // to accomodate for the  required number of
    //  pages is present  or not
    if (numberOfPages <= (fHandle->totalNumPages) * one)
    {
        printf("Capacity left is not enough. \n");
        RC_message = "Capacity left is not enough";
        return RC_ERROR;
    }
    int totalNoOfPages = (fHandle->totalNumPages) * one;
    int requiredNumberOfPages = (numberOfPages - totalNoOfPages) * one;

    // Append empty blocks for 
    // the required extra number of the pages
    int k = 0;
    while (k < (requiredNumberOfPages * one))
    {
        appendEmptyBlock(fHandle);
        k = k + 1;
    }
    printf("log exit: ensureCapacity method \n");
    return RC_OK;
}