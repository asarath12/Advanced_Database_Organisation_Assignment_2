#include "storage_mgr.h"
#include <sys/stat.h>
#include "dberror.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

FILE *filePointer;

extern void initStorageManager()
{
    printf("================================================\n");
    printf("Initializing Storage Manager !\n");
}

extern RC createPageFile(char *fileName)
{
    int charSize=sizeof(char);
    int one=1;
    int zero=0;
    // Creates a new file
    filePointer = fopen(fileName, "w+");

    // Setting memory for an empty page
    SM_PageHandle ptr = (SM_PageHandle)calloc(PAGE_SIZE ,  charSize * one );

    // Checking if the file pointer is Null
    if (filePointer == NULL)
    {
        RC_message = "Failure in the creation of file";
        return RC_ERROR;
    }

    // Returns error code if the fwrite does not write properly
    if (fwrite(ptr, charSize * one, PAGE_SIZE, filePointer) >= PAGE_SIZE)
    {
        // Seeking to the end of file
        fseek(filePointer, zero, SEEK_END);

        // Closing the file
        fclose(filePointer);

        // Free or releasing the memory
        free(ptr);
        RC_message = "Creation of the file is successful.";
        return RC_OK;
    }
    else {
        RC_message = "Failure occurred in the writing of the file";
        return RC_WRITE_FAILED;
    }
}
extern RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
    int zero=0;
    int one=1;
    int tot_num_pages;
    filePointer = fopen(fileName, "r+");
    
    // Check and return file not found 
    //if the file is not present
    if (filePointer == NULL)
    {
        return RC_FILE_NOT_FOUND ;
    }
    else
    {
        fseek(filePointer, zero, SEEK_END);

        // Stores the size of file in a variable
        int sizeOfFile = ftell(filePointer);

        // Checks if the ftell returns -1, if so return error
        if (sizeOfFile == -1)
        {
            return RC_ERROR;
        }
            
        if (sizeOfFile % PAGE_SIZE == zero)
        {
            tot_num_pages = (sizeOfFile / PAGE_SIZE);
        }
        else
        {
            tot_num_pages = (sizeOfFile / PAGE_SIZE + one);
        }
        //tot_num_pages = tot_num_pages * one;

        //printf("tot_num_pages=");
        //
        // Setting the fileName
        fHandle->fileName = fileName;
        
        // Setting the value
        //of mgmtInfo
        fHandle->mgmtInfo = filePointer;
        
        // Setting the total 
        // number of pages (totalNumPages)
        fHandle->totalNumPages = sizeOfFile * one;
        
        // Setting the current 
        //page position (curPagePos)
        fHandle->curPagePos = zero;
        
        // Rewinding the file pointer 
        //to the beginning of the file
        fclose(filePointer);
        return RC_OK;
    }
}


extern RC closePageFile(SM_FileHandle *fHandle)
{
    // Check and return error if the fclose returns EOF
    if (fclose(fHandle->mgmtInfo) == EOF)
    {
        RC_message = "Exception occurred in closing of the file";
        return RC_ERROR;
    }
    else
    {
        RC_message = "The file was closed Successfully.";
        return RC_OK;
    }
}

extern RC destroyPageFile(char *fileName)
{
    // Storing the result returned by remove function
    int result = remove(fileName);
    int negOne = -1;

    // Return error 
    //if result is negOne
    if (result == negOne)
    {
        RC_message = "Error occurred while destroying the file";
        return RC_ERROR;
    }
    RC_message = "The file was Destroyed Successfully";
    return RC_OK;
}

extern RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int charSize=sizeof(char);
    int zero=0;
    int one=1;
    // If the pageNum is less than 0 
    //or greater than the total number of pages 
    //then, return error/exception
    if (pageNum < zero || pageNum > fHandle->totalNumPages)
    {
        RC_message = "Trying to read a page which is not existing.";
        return RC_READ_NON_EXISTING_PAGE;
    }

    // Calculating the position in the file for the requested pageNum
    long offset = pageNum * PAGE_SIZE;

    filePointer = fopen(fHandle->fileName, "r");

    // Checking if the fseek returns a number other 
    //than 0, if so 
    // there occurred an 
    //error in seeking the position
    fseek(filePointer, offset, SEEK_SET);
    fread(memPage, charSize * one, PAGE_SIZE, filePointer);

    // Setting the current page position to pageNum
    fHandle->curPagePos = pageNum * one;
    fclose(filePointer);
    return RC_OK;
}

extern int getBlockPos(SM_FileHandle *fHandle)
{
    // Returning the currentPage 
    //position inside the fileHandle pointer
    return fHandle->curPagePos;
}

extern RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int zero=0;
    // Returns the first 
    //block from the disk
    return readBlock(zero, fHandle, memPage);
}

extern RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int one=1;
    // Returns the previous 
    //block from the disk
    return readBlock(fHandle->curPagePos - one, fHandle, memPage);
}

extern RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    // Returns the current block
    // from the disk
    return readBlock(fHandle->curPagePos, fHandle, memPage);
}

extern RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int one=1;
    // Returns the next block
    // from the disk
    return readBlock(fHandle->curPagePos + one, fHandle, memPage);
}

extern RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int one =1;
    // Returns the last 
    //block from the disk
    return readBlock(fHandle->totalNumPages - one, fHandle, memPage);
}

extern RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int charSize=sizeof(char);
    int zero=0;
    int one=1;
    // Setting the filePointer
    FILE *fptr;
    fptr = fopen(fHandle->fileName, "rb+");

    // Calculating the position in the
    //the file for the requested pageNum
    long offset = pageNum * PAGE_SIZE;

    // Checking if the fseek returns 
    // a number  other than 0, 
    //if so there occurred an error
    // in seeking the position
    if (fseek(fptr, offset, SEEK_SET) != zero)
    {
        RC_message = "Error occurred in seeking the correct position";
        return RC_WRITE_FAILED;
    }

    // Writing the contents from memPage to the requested pageNum
    fwrite(memPage, charSize * one, PAGE_SIZE, fptr);
    fHandle->curPagePos = pageNum;
    fclose(fptr);
    return RC_OK;
}

extern RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    // writes the contents from the memPage 
    //to the disk in the current position of fHandle
    return writeBlock(getBlockPos(fHandle), fHandle, memPage);
}

extern RC appendEmptyBlock(SM_FileHandle *fHandle)
{
    int zero=0;
    int one=1;
    // Checking if the file handle is null 
    //and return appropriate error
    if(fHandle == NULL) 
    {
        RC_message = "File handle not initialized properly";
        return RC_FILE_HANDLE_NOT_INIT;
    }

    // Setting the filePointer
    filePointer = (FILE *)fHandle->mgmtInfo;

    // Returns error when the
    // mgmtInfo inside fHandle is null
    if (filePointer == NULL)
    {
        RC_message = "File not found";
        return RC_FILE_NOT_FOUND;
    }

    // Returning error when fseek not returns 0
    if (fseek(filePointer, zero, SEEK_END) != zero)
    {
        RC_message = "Seeking position failed";
        return RC_WRITE_FAILED;
    }

    // Allocating block of size PAGE_SIZE and initializing it to a empty pointer
    SM_PageHandle ptr = (SM_PageHandle)calloc(PAGE_SIZE, one);

    // Initializing a empty block
    fwrite(ptr, one, PAGE_SIZE, filePointer);

    // Seeking to the end of file
    fseek(filePointer, zero, SEEK_END);

    // Incrementing the total number of pages
    fHandle->totalNumPages += one;

    return RC_OK;
}

// This function checks if a 
//file has enough capacity to 
//store a specified number of pages
extern RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
    int zero=0;
    //int one=1;
    if (numberOfPages <= fHandle->totalNumPages) // checks if there is enough capacity
    {
        RC_message = "There is currently enough capacity";
        return RC_OK;
    }
    if (numberOfPages > fHandle->totalNumPages)
    {
        // calculating extra required number 
        //of pages we need to append to the file
        int extra_no_of_pages = numberOfPages - fHandle->totalNumPages;
        for (int p = zero; p < extra_no_of_pages; p++)
        {
            appendEmptyBlock(fHandle);
        }
    }
    return RC_OK;
}

