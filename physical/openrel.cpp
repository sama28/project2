#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>

int OpenRel(char * relName)
{
     printf("\n\nOpenRel..... \n ");
     int staus,indexLoc;
     int rltnExist;
     char relCatEntry[MR_RELCAT_REC_SIZE];//Entry Of ReCord In relcat

     status=FindRelNum(relName);//check if relation is in cache

    if(status>0)
    {  //relation is already in the cache 
        return status;
    }
    else
    {   
        //relation is NOT in the Cache;
        //Bring The Relation In The Cache
        //FindRec In RelCat
        rltnExist=findRecInRelcat(relName,relCatEntry);

        if(rltnExist)
        {
            if(makeRelCacheEntry(relCatEntry,indexLoc))
            {
            
            }
        }
        
    }
    
}
int findRecInRelcat(char * relName,char *recPtr)
{
    //for Refferences...
    //int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp )
    
    int status;
    Rid startRid,foundRid;

    startRid.pid=0;
    startRid.slotnum=0;
    
    status=FindRec(0,&startRid,&foundRid,recPtr,(short)DTSTRING,(unsigned)RELNAME,(unsigned)0,relName,EQ);

    if(status==1)
    {
        printf("\n\nIn findRecInRelcat");
        return 1;
    }
    else{

        printf("\n\nInfindRecRelcat...");

        printf("\n\nRELATION DOES NOT EXISTS...");
    }
    return 0;
}
int makeRelCacheEntry(char *relCatEntry,int indexLoc)
{   //it will polpulate IndexLoc Cache index with relCatEntry

    /*//for Refference
    typedef struct relList{
	char valid;
	char relName[RELNAME];
	unsigned int recLength;
	unsigned int recPerPg;
	unsigned int numPgs;
	unsigned int numRecs;
	unsigned short numAttrs;
	struct recid Rid;
	FILE *relFile;
	char dirty;
	vector <struct attrList> attrHead;
    } relCacheEntry;
    */
    //relcat(relName,recLength,rcPerPg,numPgs,numRecs,numAttrs,pid,rid)
    int offset;
    offset=0;
    strncpy(relCache[indexLoc].relName,relCatEntry,RELNAME);
    offset=RELNAME;
    relCache[indexLoc].recLength=*(unsigned *)(relCatEntry+offset);
    offset=offset+4;
    relCache[indexLoc].recPerPg =*(unsigned *)(relCatEntry+offset);
    offset=offset+4;
    relCache[indexLoc].numPgs=*(unsigned *)(relCatEntry+offset);
    offset=offset+4;
    relCache[indexLoc].numRecs=*(unsigned *)(relCatEntry+offset);
    offset=offset+4;
    relCache[indexLoc].numAttrs=*(unsigned short *)(relCatEntry+offset);
    offset=offset+2;
    
    //do the Entry Of Pid And Rid
    //relCache[indexLoc].recLength=*(unsigned *)(relCatEntry+offset);
    //offset=offset+4;
}