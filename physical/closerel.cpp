#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
void sanitize(unsigned char* a,int num);

void clearCacheEntry(int relNum){
    relCache[relNum].valid='i';
    relCache[relNum].attrHead.clear();
    if(relCache[relNum].relFile!=NULL)
        fclose(relCache[relNum].relFile);
    relCache[relNum].attrHead.clear();
    relCache[relNum].dirty='c';
}

void CloseRel(int relNum)
{
    printf("%d\n",relCache[relNum].numRecs);
    if(gPgTable[relNum].dirty=='d'){
        FlushPage(relNum,gPgTable[relNum].pid);
    }

    if(relCache[relNum].dirty=='d'){
        unsigned char record[relCache[0].recLength+1];
        int offset=0;
        sanitize(record,relCache[0].recLength);
            for(int k=0;k<strlen(relCache[relNum].relName);k++){
                record[k]=relCache[relNum].relName[k];
            }
        offset=offset+RELNAME;

        bwrite_int(record,relCache[relNum].recLength,sizeof(int),&offset);
        bwrite_int(record,relCache[relNum].recPerPg,sizeof(int),&offset);
        bwrite_int(record,relCache[relNum].numPgs,sizeof(int),&offset);
        bwrite_int(record,relCache[relNum].numRecs,sizeof(int),&offset);
        bwrite_int(record,relCache[relNum].numAttrs,sizeof(short),&offset);
        bwrite_int(record,relCache[relNum].attr0Rid.pid,sizeof(int),&offset);
        bwrite_int(record,relCache[relNum].attr0Rid.slotnum,sizeof(int),&offset);
        printf("stringlen %d",strlen((char*)record));
        /*for(int k=0;k<relCache[0].recLength;k++)
            printf("%02x",record[k]);
            printf("\n");
            
            int t=0;
            unsigned char q[32];
            bread_string(record,32,&t,q);
            int a1=bread_int(record,4,&t);int a2=bread_int(record,4,&t);int a3=bread_int(record,4,&t);
            int a4=bread_int(record,4,&t);int a5=bread_int(record,2,&t);int a6=bread_int(record,4,&t);
            int a7=bread_int(record,4,&t);
            printf("asasa\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",q,a1,a2,a3,a4,a5,a6,a7);
            

        for(int k=0;k<relCache[0].recLength;k++)
        printf("%02x",record[k]);
        printf("\n");
        */
        WriteRec(0,record,&relCache[relNum].Rid);
        //clearCacheEntry(relNum);
        
    }

    
}