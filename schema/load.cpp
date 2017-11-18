
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
int OpenRel(char * relName);
void CloseRel(int relNum);
void InsertRec(int,unsigned char*);

int Load (int argc,char ** argv)
{
    char dataPath[MAX_PATH_LENGTH],relPath[MAX_PATH_LENGTH];
    int relNum=2;
    relCache[relNum].valid='v';
    getPath(relPath,argv[1]);strcat(relPath,"/");strcpy(dataPath,relPath);
    strcat(dataPath,argv[2]);strcat(relPath,argv[1]);
    //printf("%s\n%s",dataPath,relPath);
    FILE* dataFile=fopen(dataPath,"rb");
    relCache[relNum].relFile=fopen(relPath,"rb+");
    
    if(dataFile==NULL || relCache[relNum].relFile==NULL){
        printf("Unable to open data file.\n");
        return 0;
    }
    //ReadPage(relNum,0);
    int recLength=relCache[relNum].recLength;
	int recPrPg=PAGESIZE/recLength,numRecs=fileSize(dataFile)/recLength;
    int a,off=0,recRead=0,NPAGESIZE=recPrPg*recLength,numPgs=fileSize(dataFile)/NPAGESIZE;
    float s;char d[35];
    char data[NPAGESIZE+1];
    unsigned char record[recLength+1];
	if(relCache[relNum].numRecs==0){
        for(int j=0;j<numPgs;j++){
            fseek(dataFile,NPAGESIZE*j,SEEK_SET);
            fread(&data,NPAGESIZE,1,dataFile);
            for(int i=0;i<NPAGESIZE/recLength;i++){
                off=0;
                memcpy((char*)record,data+i*recLength,recLength);
                /*a=*(int*)(record+off);off+=4;
                s=*(float*)(record+off);off+=4;
                strncpy(d,(char*)record+off,35);
                printf("%d\t%f\t%s\n",a,s,d);
                recRead+=recLength;*/
                InsertRec(relNum,record);
            }
        }
    }
    else{
        printf("Relation is not empty.\n");
    }
    printf("numrec%d",relCache[relNum].numRecs);
    //read
    return (OK);  /* all's fine*/
}


