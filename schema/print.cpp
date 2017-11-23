
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
#include<string>

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

void parseRecord2(int relNum, int rowlen2,char *record)
{
    int a, offset = 0,width=0;
    float s;
    char d[35];
    printf("| ");
    for (int i = 0; i < relCache[relNum].numAttrs; i++)
    {
        switch (relCache[relNum].attrHead[i].type)
        {
        case DTINT:
        {
            width=MAXINTLEN;
            printf("%*d ", max(width,strlen(relCache[relNum].attrHead[i].attrName)),*(int *)(record + offset));
            offset += sizeof(int);
        }
        break;

        case DTFLOAT:
        {
            width=MAXFLOATLEN;
            printf("%*.8g ", max(width,strlen(relCache[relNum].attrHead[i].attrName)),*(float *)(record + offset));
            offset += sizeof(float);
        }
        break;

        case DTSTRING:
        {
            char str[relCache[relNum].attrHead[i].length + 1];
            strncpy(str, record + offset, relCache[relNum].attrHead[i].length);
            width=relCache[relNum].attrHead[i].length;
            printf("%*s ", max(width,strlen(relCache[relNum].attrHead[i].attrName)),str);
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
    for(int i=0;i<rowlen2+2;i++)
        printf("-");
        printf("\n");

    /*off=0;
        a=*(int*)(record+off);off+=4;
        s=*(float*)(record+off);off+=4;
        strncpy(d,record+off,35);
        printf("%d\t%f\t%s\n",a,s,d);*/
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
        int width,rowlen=0;
        for (int i = 0; i < relCache[relNum].numAttrs; i++){
            if(relCache[relNum].attrHead[i].type==DTSTRING){
                width=relCache[relNum].attrHead[i].length;
            }
            else if(relCache[relNum].attrHead[i].type==DTFLOAT){
                width=MAXFLOATLEN;
            }
            else{
                width=MAXINTLEN;
            }
            rowlen+=max(width,strlen(relCache[relNum].attrHead[i].attrName))+2;
        }
        printf("\n");
        for(int i=0;i<rowlen+2;i++)
            printf("=");
        printf("\n| ");
        for (int i = 0; i < relCache[relNum].numAttrs; i++)
        {
            if(relCache[relNum].attrHead[i].type==DTSTRING){
                width=relCache[relNum].attrHead[i].length;
            }
            else if(relCache[relNum].attrHead[i].type==DTFLOAT){
                width=MAXFLOATLEN;
            }
            else{
                width=MAXINTLEN;
            }
            
            printf("%*s |", max(width,strlen(relCache[relNum].attrHead[i].attrName)),relCache[relNum].attrHead[i].attrName);
        }
        printf("\n");
        for(int i=0;i<rowlen+2;i++)
            printf("=");
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
                parseRecord2(relNum, rowlen,record);
            }
        }
    }
    else if(relNum==0)
    {
         printRelCat();
    }
    else if(relNum==1)
    {
       printAttrCat();
       printf("\n\nprinting AttrCat");
    }
    printf("opened");
    return (OK); /* all's fine */
}
