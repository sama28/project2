#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
int bringInRelCache(char * relCatEntry,Rid *foundRid);
int findRecInRelcat(char * relName,char *recPtr,Rid *foundRid);
int makeRelCacheEntry(char *relCatEntry,int indexLoc,Rid *foundRid);
 void makeRelCatRec(unsigned char *cacheEntry,int indexLoc);
 int makeAttrCacheEntry(int Loc,Rid startRid,unsigned numAttrs,unsigned recLength);
 int flushRelCacheEntry(int Loc);
 int entryWhere();
 int whomToReplace(void);
int OpenRel(char * relName)
{

    printf("\n\nOpenRel..... \n ");
     int status,indexLoc,entryAt=0;
     int rltnExist;
     Rid foundRid;
     char relCatEntry[MR_RELCAT_REC_SIZE];//Entry Of ReCord In relcat

     status=FindRelNum(relName);//check if relation is in cache

    if(status>=0 && relCache[status].valid == 'v')
    {  //relation is already in the cache 
        return status;
    }
    else
    {   
        //relation is NOT in the Cache;
        //Bring The Relation In The Cache
        //FindRec In RelCat
        rltnExist=findRecInRelcat(relName,relCatEntry,&foundRid);

        if(rltnExist)
        {
           entryAt=bringInRelCache(relCatEntry,&foundRid);
            if(entryAt>=MR_FIRST_USR_RLTN_IND)
            {
                //compulSorily bring One Page Of New relaTion
                gPgTable[entryAt].pid=1;//this Entry There For Forced Read
                gPgTable[entryAt].dirty='c';
                ReadPage(entryAt,0);
                return entryAt;
            }
            else
            {
                printf("\n\nERROR SYSTEM CATALOG(RELCAT/ATTRCAT) ENTRY IN CACHE IS REPLACED BY OPENREL...");
                return -1;
            }
        }
        else
        {
            return -2;
            printf("\n\nIN OPENREL :- RELATION DOESN'T EXISTS....");
        }
    }
    return -3;
    
}
int bringInRelCache(char * relCatEntry,Rid *foundRid)
{   //brings the specified entry into the relCahe foundRid Is Rid Of relCatentry Into relcat relation need 
    //to be store in The RelCache
    //retruns the index of The location in relCache at which New Entry Is Placed

    printf("bringInRelCache(char * relCatEntry=%s,Rid *foundRid(pid ,slotnum)(%u %u))",relCatEntry,foundRid->pid,foundRid->slotnum);
    int loc,flushed,entrymade=0;
    loc=entryWhere();
    flushed=flushRelCacheEntry(loc);
    if(flushed==1)
    {
        if(loc<19)
        {
           relCacheRplcmntInd=loc+1;
        }
        else
        {
            relCacheRplcmntInd=MR_FIRST_USR_RLTN_IND;
        }
       entrymade=makeRelCacheEntry(relCatEntry,loc,foundRid);
      if(entrymade==1)
      {
          return loc;
      }
    }
    return -1;
}
int findRecInRelcat(char * relName,char *recPtr,Rid *foundRid)
{
    //for Refferences...
    //int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp )
    printf("findRecInRelcat(char * relName =%s,char *recPtr= %s,Rid *foundRid(pid,slotnum)=%u %u )",relName,recPtr,foundRid->pid,foundRid->slotnum);
    int status;
    Rid startRid;

    startRid.pid=0;
    startRid.slotnum=0;
    
    status=FindRec(0,&startRid,foundRid,recPtr,(short)DTSTRING,(unsigned)RELNAME,(unsigned)0,relName,EQ);

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
int makeRelCacheEntry(char *relCatEntry,int indexLoc,Rid *foundRid)
{   //it will polpulate IndexLoc Cache index with relCatEntry

    /*for Refference
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

    printf("\n\nint makeRelCacheEntry(char *relCatEntry=%s,int indexLoc =%d,Rid *foundRid(pid,SlotNum)(%u ,%u))",relCatEntry,indexLoc,foundRid->pid,foundRid->slotnum);
    int offset,attrCatEntryMade=0;
    offset=0;
    char path[MAX_PATH_LENGTH];
    int status;
    if(relCache[indexLoc].valid=='v' && relCache[indexLoc].relFile !=NULL )
    {
        status=fclose(relCache[indexLoc].relFile);
    }
    else{
        status=0;
    }
    if(status==0)
    {
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
        relCache[indexLoc].attr0Rid.pid=*(unsigned *)(relCatEntry+offset);
        offset=offset+4;
        relCache[indexLoc].attr0Rid.slotnum=*(unsigned *)(relCatEntry+offset);

        relCache[indexLoc].Rid.pid=foundRid->pid;
        relCache[indexLoc].Rid.slotnum=foundRid->slotnum;

        relCache[indexLoc].dirty='c';
        relCache[indexLoc].valid='v';

       
        printf("\n\nEntry In RelCahce:-\n%s %u %u %u %u %d %u %u",relCache[indexLoc].relName,relCache[indexLoc].recLength,relCache[indexLoc].recPerPg,relCache[indexLoc].numRecs,relCache[indexLoc].numAttrs,relCache[indexLoc].attr0Rid.pid,relCache[indexLoc].attr0Rid.slotnum);
        //POPULATE attrHead
        //relCache[indexLoc].attrHead[]
        attrCatEntryMade=makeAttrCacheEntry(indexLoc,relCache[indexLoc].attr0Rid,relCache[indexLoc].numAttrs,relCache[indexLoc].recLength);
        if(attrCatEntryMade!=1)
        {
            relCache[indexLoc].valid='i';
            return 0;
            
        } 
        //open New Relation File
        getPath(path,relCache[indexLoc].relName);
        relCache[indexLoc].relFile=fopen(path,"rb+");
        if(relCache[indexLoc].relFile == NULL)
        {
            printf("\n\nNEW RELATION FILE CAN NOT BE OPENED");
            return 0;
        }
        return 1;
    }
    else
    {
        relCache[indexLoc].valid='i';
        printf("\n\nEROR IN CLOSING OLDRELATION FILE.....");
        return 0;
    }

}
void makeRelCatRec(unsigned char *cacheEntry,int indexLoc)
{       //convert relCache Entry Of indexLoc Into relCatRec Formate That Can be stored;
    int offset;
    offset=0;
    strncpy((char *)cacheEntry,relCache[indexLoc].relName,RELNAME);
    offset=RELNAME;
    *(unsigned *)(cacheEntry+offset)=relCache[indexLoc].recLength;
    offset=offset+4;
    *(unsigned *)(cacheEntry+offset)=relCache[indexLoc].recPerPg ;
    offset=offset+4;
    *(unsigned *)(cacheEntry+offset)=relCache[indexLoc].numPgs;
    offset=offset+4;
   *(unsigned *)(cacheEntry+offset)=relCache[indexLoc].numRecs;
    offset=offset+4;
    *(unsigned short *)(cacheEntry+offset)=relCache[indexLoc].numAttrs;
    offset=offset+2;
    *(unsigned *)(cacheEntry+offset)=relCache[indexLoc].attr0Rid.pid;
    offset=offset+4;
    *(unsigned *)(cacheEntry+offset)=relCache[indexLoc].attr0Rid.slotnum;
}
int makeAttrCacheEntry(int Loc,Rid startRid,unsigned numAttrs,unsigned recLength)
{   
    //Rid startRid;
    int status=0,i,offset;
    Rid foundRid;
    char recPtr[recLength];
    struct attrList attrHead;
    //startRid.pid=*(unsigned *)relCatEntry+attr0pidIndInRec;
    //startRid.slotnum=*(unsigned *)(relCatEntry+attr0pidIndInRec+4);
    //int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp );
    
    //int nextRec(int relNum, Rid* startRid, char *recPtr)

    printf("\n\nmakeAttrCacheEntry(int Loc,Rid startRid,unsigned numAttrs,unsigned recLength)");
    relCache[Loc].attrHead.clear();
    //status=nextRec(1,&startRid,recPtr);
   for(i=0;i<numAttrs;i++)
    {
        //WARNING RIGHT THING TO DO IS COMPARE OFFSET ORDERRING OF ATTRIBUTE....
        status=nextRec(1,&startRid,recPtr);
        if(status==1)
        {
            offset=0;
            strncpy(attrHead.attrName,recPtr,ATTRLEN);
            offset=ATTRLEN;
            attrHead.offset=*(unsigned *)(recPtr+offset);
            offset=offset+4;
            attrHead.length=*(unsigned *)(recPtr+offset);
            offset=offset+4;
            attrHead.type=*(short *)(recPtr+offset);    
            printf("\n\nENTRY MADE IN ATTRCACHE %s %u %u %d",attrHead.attrName,attrHead.offset,attrHead.length,attrHead.type);
      
            relCache[Loc].attrHead.push_back(attrHead);
      
            startRid.slotnum++;
            if(startRid.slotnum >= relCache[1].numRecs)
            {
               startRid.pid++;
                startRid.slotnum=0;
            }
        }
        else
        {
            break;
            i=numAttrs+1;//just to break loop
        }
    }
    if(i==numAttrs)
    {
        return 1;
    }
    else
    {
        printf("\n\nMakeAttrCacheEntry Function Fails In Pupulating Cache.....");
        return 0;
    }
    
}
int flushRelCacheEntry(int Loc)
{   //ONLY FLUSHES RELCHACHE ENTRY TO THE RELCAT AND DOESNT DO ENYTHING FOR ATTRCAT
    //IF UPDATE COMMAND CHANGES THE ATTRIBUTE COULD CREATE PROBLEM
    printf("\n\nint flushRelCacheEntry(int Loc)=%d",Loc);
    int gPgFlushed=0;
    unsigned char cacheEntry[MR_RELCAT_REC_SIZE];

    //check Befor Flshing Cache That Corresponding Page In gPgTable Is Not Dirty Or Flushed That Page 1st
    if(gPgTable[Loc].dirty=='d' && relCache[Loc].valid=='v')
    {
        printf("\n\nint flushRelCacheEntry(int Loc)=%d",Loc);
        gPgFlushed=FlushPage(Loc,gPgTable[Loc].pid);
    }
    else{
        gPgFlushed=1;
    }
    
    //---------------------------------------------
    //CODE FOR FLUSHING ATTRCAT ENTRY SHOLD BE hERE
    //....
    //....

    //---------------------------------------------

    
    if(gPgFlushed==1)//after success fully dealling with gPgTable
    {
        //Check if Cache Entry Is Dirty Or Not Corresponding To The Location That Is To Be Replaced
        if(relCache[Loc].dirty=='d')
        {
            //write this entry into relation and attribute Catalog
            //insert A Record At Particular Place
            makeRelCatRec(cacheEntry,Loc);//put the Cache Content in cacheEntry Rec
            WriteRec(0,cacheEntry,&(relCache[Loc].Rid));//bug remove code with write checking
            return 1;
        }
        else{
            return 1;
        }
    }
    return 0;
}
int entryWhere()
{
    if(relCacheIndex < 20)//relCacheIndex<MAX_RELCACHE_ENTRY
    {
        printf("\n\nint entryWhere()=%d",relCacheIndex);
        return relCacheIndex;
    }
    else{
            //decide replacement policy
            //for fifo use below
            printf("\n\nint entryWhere()-> whomToReplace()");
            //return whomToReplace();
            return relCacheRplcmntInd;
    }
}
int whomToReplace(void)
{   //Returns The Cache Index Of The Entry Which Can Be Replced According To fifo
    int count,i;
    count=0;
    i=relCacheRplcmntInd;
    while(count<20)
    {
        if(i<20)
        {
            if(gPgTable[i].dirty=='c')
            {
                //relCacheRplcmntInd=i+1;
                return i; 
            }
            else{
                i++;
                count++;
            }
        }
        else
        {
            i=MR_FIRST_USR_RLTN_IND;
        }
    }
    printf("\n\nint whomToReplace(void)=%d",relCacheRplcmntInd);
    return relCacheRplcmntInd;
    /*
    if(relCacheRplcmntInd<20)//relCacheIndex<MAX_RELCACHE_ENTRY
    {

        //relCacheRplcmntInd++;
        return relCacheRplcmntInd-1;
    }
    else{
        relCacheRplcmntInd=MR_FIRST_USR_RLTN_IND+1;
        return MR_FIRST_USR_RLTN_IND;
    }
    */
}