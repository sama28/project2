
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
void sanitize(unsigned char *a, int num);
void parseRecord(int relNum, char *record)
{
    int a, offset = 0;
    float s;
    char d[35];
    printf("|");
    for (int i = 0; i < relCache[relNum].numAttrs; i++)
    {
        switch (relCache[relNum].attrHead[i].type)
        {
        case DTINT:
        {
            printf("%d", *(int *)(record + offset));
            offset += sizeof(int);
        }
        break;

        case DTFLOAT:
        {
            printf("%f", *(float *)(record + offset));
            offset += sizeof(float);
        }
        break;

        case DTSTRING:
        {
            char str[relCache[relNum].attrHead[i].length + 1];
            strncpy(str, record + offset, relCache[relNum].attrHead[i].length);
            printf("%s", str);
            offset += relCache[relNum].attrHead[i].length;
        }
        break;

        default:
        {
            printf("Invalid attribute entry in attribute cache.\n");
        }
        break;
        }
        printf("|");
    }
    printf("\n");

    /*off=0;
        a=*(int*)(record+off);off+=4;
        s=*(float*)(record+off);off+=4;
        strncpy(d,record+off,35);
        printf("%d\t%f\t%s\n",a,s,d);*/
    }
    
int max(int a,int b){
    if(a<b)
        return b;
    return a;
}

int Print(int argc, char **argv)
{
    char relPath[MAX_PATH_LENGTH];
    int relNum = OpenRel(argv[1]);
    relCache[relNum].valid = 'v';

    if (relNum > 1)
    {
        //getPath(relPath,argv[1]);strcat(relPath,"/");strcat(relPath,argv[1]);

        //relCache[relNum].relFile=fopen(relPath,"rb+");
        //gPgTable[relNum].pid=1;
        //ReadPage(relNum,0);
        if (relCache[relNum].relFile == NULL)
        {
            printf("Unable to open data file.\n");
            return 0;
        }
        Rid startRid, foundRid;
        startRid.pid = 0;
        startRid.slotnum = 0;
        char record[relCache[relNum].recLength + 1];
        printf("|");
        for (int i = 0; i < relCache[relNum].numAttrs; i++)
        {
            printf("\t%s\t|", relCache[relNum].attrHead[i].attrName);
        }
        printf("\n");
        for (int i = 0; i < relCache[relNum].numRecs; i++)
        {
            if (GetNextRec(relNum, &startRid, &foundRid, record))
            {
                /*
            for(int k=0;k<relCache[relNum].recLength;k++)
            printf("%02x",record[k]);
            printf("\n");
            */
                startRid.pid = foundRid.pid;
                startRid.slotnum = foundRid.slotnum + 1;
                //printf("rid  %d\t%d\n",foundRid.pid,foundRid.slotnum);
                parseRecord(relNum, record);
            }
        }
    }
    else if (relNum == 0)
    {
        updateCat();
        printRelCat();
    }
    else if (relNum == 1)
    {
        updateCat();
        printAttrCat();
        printf("\n\nprinting AttrCat");
    }
    printf("opened");
    return (OK); /* all's fine */
}
void updateCat(void)
{
    unsigned char record[relCache[0].recLength + 1];
    int offset = 0;

    for (int i = 0; i < MAXOPEN; i++)
    {

        if (relCache[i].dirty == 'd' && relCache[i].valid == 'v')
        {
            offset = 0;
            //printf("%d\t%d\n",relCache[i].Rid.pid,relCache[i].Rid.slotnum);
            sanitize(record, relCache[0].recLength);
            for (int k = 0; k < strlen(relCache[i].relName); k++)
            {
                record[k] = relCache[i].relName[k];
            }
            offset = offset + RELNAME;

            bwrite_int(record, relCache[i].recLength, sizeof(int), &offset);
            bwrite_int(record, relCache[i].recPerPg, sizeof(int), &offset);
            bwrite_int(record, relCache[i].numPgs, sizeof(int), &offset);
            bwrite_int(record, relCache[i].numRecs, sizeof(int), &offset);
            bwrite_int(record, relCache[i].numAttrs, sizeof(short), &offset);
            bwrite_int(record, relCache[i].attr0Rid.pid, sizeof(int), &offset);
            bwrite_int(record, relCache[i].attr0Rid.slotnum, sizeof(int), &offset);

            for (int k = 0; k < relCache[0].recLength; k++)
                printf("%02x", record[k]);
            printf("\n");
            /*
            int t=0;
            unsigned char q[32];
            bread_string(record,32,&t,q);
            int a1=bread_int(record,4,&t);int a2=bread_int(record,4,&t);int a3=bread_int(record,4,&t);
            int a4=bread_int(record,4,&t);int a5=bread_int(record,2,&t);int a6=bread_int(record,4,&t);
            int a7=bread_int(record,4,&t);
            printf("\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",q,a1,a2,a3,a4,a5,a6,a7);
            */
            WriteRec(0, record, &relCache[i].Rid);
        }
    }
}
