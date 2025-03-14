#include "record_mgr.h"
#include "storage_mgr.h"
#include "buffer_mgr.h"
#include "dberror.h"
#include "tables.h"
#include <stdlib.h>
#include <string.h>

SM_FileHandle fh_rec_mgr;
int tombStonedRIDsList[10000];
int currentScannedRecord = 0;

// Initialize the record manager
RC initRecordManager(void *mgmtData) {
    for (int i = 0; i < 10000; i++) {
        tombStonedRIDsList[i] = -99;
    }
    return RC_OK;
}

// Shutdown the record manager
RC shutdownRecordManager() {
    return RC_OK;
}

// Create a table
RC createTable(char *name, Schema *schema) {
    SM_PageHandle ph = (SM_PageHandle) malloc(PAGE_SIZE);
    
    if (createPageFile(name) != RC_OK) return RC_WRITE_FAILED;
    if (openPageFile(name, &fh_rec_mgr) != RC_OK) return RC_FILE_NOT_FOUND;
    
    memset(ph, '\0', PAGE_SIZE);
    strcpy(ph, serializeSchema(schema));
    writeBlock(0, &fh_rec_mgr, ph);

    memset(ph, '\0', PAGE_SIZE);
    writeBlock(1, &fh_rec_mgr, ph);

    free(ph);
    return RC_OK;
}

// Open a table
RC openTable(RM_TableData *rel, char *name) {
    rel->schema = (Schema *)malloc(sizeof(Schema));
    rel->name = (char *)malloc(100 * sizeof(char));
    SM_PageHandle ph = (SM_PageHandle) malloc(PAGE_SIZE);

    if (fh_rec_mgr.fileName == NULL) {
        openPageFile(name, &fh_rec_mgr);
    }

    strcpy(rel->name, name);
    readBlock(0, &fh_rec_mgr, ph);
    deSerializeSchema(ph, rel->schema);
    rel->mgmtData = NULL;
    
    free(ph);
    return (rel->schema != NULL && rel != NULL) ? RC_OK : RC_OPEN_TABLE_FAILED;
}

// Close a table
RC closeTable(RM_TableData *rel) {
    free(rel->schema);
    free(rel->name);
    closePageFile(&fh_rec_mgr);
    return RC_OK;
}

// Delete a table
RC deleteTable(char *name) {
    destroyPageFile(name);
    return RC_OK;
}

// Get number of tuples
int getNumTuples(RM_TableData *rel) {
    int freePageCount = 0;
    SM_PageHandle ph = (SM_PageHandle) malloc(PAGE_SIZE);

    readBlock(1, &fh_rec_mgr, ph);
    for (int i = 0; ph[i] != NULL; i++) {
        if (ph[i] == ';') freePageCount++;
    }

    free(ph);
    return fh_rec_mgr.totalNumPages - freePageCount;
}

// Insert a record
RC insertRecord(RM_TableData *rel, Record *record) {
    if (fh_rec_mgr.fileName == NULL) {
        openPageFile(rel->name, &fh_rec_mgr);
    }

    char *data = (char *)malloc(PAGE_SIZE);
    memset(data, '\0', PAGE_SIZE);
    strcpy(data, record->data);

    writeBlock(fh_rec_mgr.totalNumPages, &fh_rec_mgr, data);
    record->id.page = fh_rec_mgr.totalNumPages - 3;
    record->id.slot = -99; // Single record per page

    free(data);
    return RC_OK;
}

// Delete a record
RC deleteRecord(RM_TableData *rel, RID id) {
    for (int i = 0; tombStonedRIDsList[i] != -99; i++);
    tombStonedRIDsList[i] = id.page;
    return RC_OK;
}

// Update a record
RC updateRecord(RM_TableData *rel, Record *record) {
    openPageFile(rel->name, &fh_rec_mgr);
    writeBlock(record->id.page + 2, &fh_rec_mgr, record->data);
    closePageFile(&fh_rec_mgr);
    return RC_OK;
}

// Get a record
RC getRecord(RM_TableData *rel, RID id, Record *record) {
    SM_PageHandle ph = (SM_PageHandle) malloc(PAGE_SIZE);
    openPageFile(rel->name, &fh_rec_mgr);
    
    readBlock(id.page + 2, &fh_rec_mgr, ph);
    strcpy(record->data, ph);

    free(ph);
    return RC_OK;
}

// Start a scan
RC startScan(RM_TableData *rel, RM_ScanHandle *scan, Expr *cond) {
    scan->rel = rel;
    scan->mgmtData = cond;
    return RC_OK;
}

// Fetch next record in scan
RC next(RM_ScanHandle *scan, Record *record) {
    if (fh_rec_mgr.fileName == NULL) {
        openPageFile(scan->rel->name, &fh_rec_mgr);
    }

    Expr *expr = (Expr *) scan->mgmtData;
    while (currentScannedRecord < fh_rec_mgr.totalNumPages - 2) {
        SM_PageHandle ph = (SM_PageHandle) malloc(PAGE_SIZE);
        memset(ph, '\0', PAGE_SIZE);

        readBlock(currentScannedRecord + 2, &fh_rec_mgr, ph);
        char cellValue[PAGE_SIZE];
        memset(cellValue, '\0', PAGE_SIZE);

        int semiColonPosition[3];
        storeSemiColonPostion(ph, semiColonPosition);

        if (expr->expr.op->type == OP_COMP_EQUAL) {
            getColumnData(expr->expr.op->args[1]->expr.attrRef, ph, semiColonPosition, cellValue);
            if (expr->expr.op->args[0]->expr.cons->dt == DT_INT && atoi(cellValue) == expr->expr.op->args[0]->expr.cons->v.intV) {
                strcpy(record->data, ph);
                currentScannedRecord++;
                free(ph);
                return RC_OK;
            }
        }
        currentScannedRecord++;
        free(ph);
    }

    currentScannedRecord = 0;
    return RC_RM_NO_MORE_TUPLES;
}

// Close scan
RC closeScan(RM_ScanHandle *scan) {
    return RC_OK;
}

// Get record size
int getRecordSize(Schema *schema) {
    int size = 0;
    for (int i = 0; i < schema->numAttr; i++) {
        switch (schema->dataTypes[i]) {
            case DT_INT: size += sizeof(int); break;
            case DT_FLOAT: size += sizeof(float); break;
            case DT_BOOL: size += sizeof(bool); break;
            case DT_STRING: size += schema->typeLength[i]; break;
        }
    }
    return size;
}

// Create a schema
Schema *createSchema(int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys) {
    Schema *schema = (Schema *)malloc(sizeof(Schema));
    schema->numAttr = numAttr;
    schema->attrNames = attrNames;
    schema->dataTypes = dataTypes;
    schema->typeLength = typeLength;
    schema->keyAttrs = keys;
    schema->keySize = keySize;
    return schema;
}

// Free schema
RC freeSchema(Schema *schema) {
    free(schema);
    return RC_OK;
}

// Create a record
RC createRecord(Record **record, Schema *schema) {
    *record = (Record *)malloc(sizeof(Record));
    (*record)->data = (char *)malloc(PAGE_SIZE);
    memset((*record)->data, '\0', PAGE_SIZE);
    return RC_OK;
}

// Free a record
RC freeRecord(Record *record) {
    free(record->data);
    free(record);
    return RC_OK;
}

// Get an attribute
RC getAttr(Record *record, Schema *schema, int attrNum, Value **value) {
    *value = (Value *)malloc(sizeof(Value));
    int offset = 0;
    for (int i = 0; i < attrNum; i++) {
        offset += (schema->dataTypes[i] == DT_INT) ? sizeof(int) : (schema->dataTypes[i] == DT_STRING) ? schema->typeLength[i] : sizeof(float);
    }
    memcpy(&(*value)->v, record->data + offset, sizeof(Value));
    return RC_OK;
}

// Set an attribute
RC setAttr(Record *record, Schema *schema, int attrNum, Value *value) {
    int offset = 0;
    for (int i = 0; i < attrNum; i++) {
        offset += (schema->dataTypes[i] == DT_INT) ? sizeof(int) : (schema->dataTypes[i] == DT_STRING) ? schema->typeLength[i] : sizeof(float);
    }
    memcpy(record->data + offset, &value->v, sizeof(Value));
    return RC_OK;
}