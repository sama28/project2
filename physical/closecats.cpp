#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
void sanitize(unsigned char* a,int num);
void CloseCats(void)
{
    unsigned char record[relCache[0].recLength+1];
    int offset=0;
    for(int i=0;i<20;i++){
        if(relCache[i].dirty=='d'){
            offset=0;
            //printf("%d\t%d\n",relCache[i].Rid.pid,relCache[i].Rid.slotnum);
            sanitize(record,relCache[0].recLength);
            for(int k=0;k<strlen(relCache[i].relName);k++){
                record[k]=relCache[i].relName[k];
            }
            offset=offset+RELNAME;

            bwrite_int(record,relCache[i].recLength,sizeof(int),&offset);
            bwrite_int(record,relCache[i].recPerPg,sizeof(int),&offset);
            bwrite_int(record,relCache[i].numPgs,sizeof(int),&offset);
            bwrite_int(record,relCache[i].numRecs,sizeof(int),&offset);
            bwrite_int(record,relCache[i].numAttrs,sizeof(short),&offset);
            bwrite_int(record,relCache[i].attr0Rid.pid,sizeof(int),&offset);
            bwrite_int(record,relCache[i].attr0Rid.slotnum,sizeof(int),&offset);

            for(int k=0;k<relCache[0].recLength;k++)
            printf("%02x",record[k]);
            printf("\n");
            
            int t=0;
            unsigned char q[32];
            bread_string(record,32,&t,q);
            int a1=bread_int(record,4,&t);int a2=bread_int(record,4,&t);int a3=bread_int(record,4,&t);
            int a4=bread_int(record,4,&t);int a5=bread_int(record,2,&t);int a6=bread_int(record,4,&t);
            int a7=bread_int(record,4,&t);
            printf("\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",q,a1,a2,a3,a4,a5,a6,a7);
            
            WriteRec(0,record,&relCache[i].Rid);
        }
    }
    fclose(relCache[0].relFile);
    fclose(relCache[1].relFile);
}
