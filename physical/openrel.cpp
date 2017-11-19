#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
int bringInRelCache(char *,Rid *);
int findRecInRelcat(char * ,char *,Rid *);
int makeRelCacheEntry(char *,int ,Rid *);
 void makeRelCatRec(unsigned char *,int );
 int makeAttrCacheEntry(int ,Rid ,unsigned short ,unsigned );
 int flushRelCacheEntry(int );
 int entryWhere();
 int whomToReplace(void);
 void shwRelCatRec(unsigned char recptr[]);
int OpenRel(char * relName)
{

     printf("\n\nOpenRel..... \n ");
     int status,indexLoc,entryAt=0;
     int rltnExist;
     Rid foundRid;
     char relCatEntry[MR_RELCAT_REC_SIZE];//Entry Of ReCord In relcat

     status=FindRelNum(relName);//check if relation is in cache

    if( status >= 0 )
    {  //relation is already in the cache 
        return status;
    }
    else
    {   
        //relation is NOT in the Cache;
        //Bring The Relation In The Cache
        //FindRec In RelCat
        if(relCache[0].relFile != NULL && relCache[1].relFile != NULL)
        {
           rltnExist=findRecInRelcat(relName,relCatEntry,&foundRid);
            if(rltnExist)
            {
               entryAt=bringInRelCache(relCatEntry,&foundRid);
                if(entryAt>=MR_FIRST_USR_RLTN_IND)
                {
                    //compulSorily bring One Page Of New relaTion
                    //gPgTable[entryAt].pid=1;//this Entry There For Forced Read
                    //gPgTable[entryAt].dirty='c';
                    //ReadPage(entryAt,0);
                    return entryAt;
                }
                else
                {
                    //
                    return -1;
                }
            }
            else
            {
                return -2;
                printf("\n\nIN OPENREL :- RELATION DOESN'T EXISTS....");
            }
        }
        else
        {
            printf("\IN OPENREL :- OPEN THE DATABASE ....");
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
    loc=relCacheRplcmntInd;//entryWhere();
    flushed=flushRelCacheEntry(loc);
    if(flushed==1)
    {
        if(loc<NUM_RELCACHE_ENTRY-1)
        {

            if(loc>=MR_FIRST_USR_RLTN_IND)
            {
                relCacheRplcmntInd=loc+1;
                
            }
           else{
                printf("\n\nERROR SYSTEM CATALOG(RELCAT/ATTRCAT) ENTRY IN CACHE IS REPLACED BY OPENREL...");
               //relCacheRplcmntInd=MR_FIRST_USR_RLTN_IND;
           }
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
    //int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp )rltnExist=findRecInRelcat(relName,relCatEntry,&foundRid);
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

       
        printf("\n\nEntry In RelCahce:-\n%s %u %u %u %u %d %u %u",relCache[indexLoc].relName,relCache[indexLoc].recLength,relCache[indexLoc].recPerPg,relCache[indexLoc].numPgs,relCache[indexLoc].numRecs,relCache[indexLoc].numAttrs,relCache[indexLoc].attr0Rid.pid,relCache[indexLoc].attr0Rid.slotnum);
        //POPULATE attrHead
        //relCache[indexLoc].attrHead[]
        attrCatEntryMade=makeAttrCacheEntry(indexLoc,relCache[indexLoc].attr0Rid,relCache[indexLoc].numAttrs,relCache[indexLoc].recLength);
        if(attrCatEntryMade!=1)
        {
            relCache[indexLoc].valid='i';
            return 0;
            
        } 
        //-----------------------
        //open New Relation File
       
        getPath(path,relCache[indexLoc].relName);
        strcat(path,"/");
        strcat(path,relCache[indexLoc].relName);
        printf("\n\nmakeRelCahceEntry relName=%s RelNum=%d path %s",relCache[indexLoc].relName,indexLoc,path);
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
int makeAttrCacheEntry(int Loc,Rid start_Rid,unsigned short numAttrs,unsigned recLength)
{   
    int status=0,offset;
    Rid foundRid,startRid;
    char recPtr[recLength];
    struct attrList attrHead;
    unsigned short i,numOfAttrs;
    unsigned tpid;
    unsigned tslotnum;

    //functions Arguments Are Behaving UnPredictabllY In this Function So Copying Them
    tpid=start_Rid.pid;
    tslotnum=start_Rid.slotnum;
    numOfAttrs=numAttrs;

    printf("\n\nmakeAttrCacheEntry(int Loc,Rid startRid,unsigned numAttrs,unsigned recLength) numAttrs %u",numAttrs);
    relCache[Loc].attrHead.clear();
   
    i=0;
   while(i < numOfAttrs)
    {
        //printf(" \n\ni ->%d,numattrs %d",i,numOfAttrs);
        startRid.pid=tpid;
        startRid.slotnum=tslotnum;

        //WARNING RIGHT THING TO DO IS COMPARE OFFSET ORDERRING OF ATTRIBUTE....
        status=nextRec(1,&startRid,recPtr);
        if(status==1)
        {
            offset=0;
            strncpy(attrHead.attrName,recPtr,ATTRLEN);
            offset=ATTRLEN;
            attrHead.offset=*((unsigned *)(recPtr+offset));
            offset=offset+4;
            attrHead.length=*((unsigned *)(recPtr+offset));
            offset=offset+4;
            attrHead.type=*((short *)(recPtr+offset));    
            printf("\n\nENTRY MADE IN ATTRCACHE at loc %d %s %u %u %d",Loc,attrHead.attrName,attrHead.offset,attrHead.length,attrHead.type);
      
            relCache[Loc].attrHead.push_back(attrHead);
      
            tslotnum++;
            if(tslotnum >= relCache[1].recPerPg)
            {
                tpid++;
                tslotnum=0;
            }
        }
        else
        {
            break;
            //i=numAttrs+1;//just to break loop
        }
     i++;   
    }
    if(i==numOfAttrs)
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
    unsigned offset;

    //check Befor Flshing Cache That Corresponding Page In gPgTable Is Not Dirty Or Flushed That Page 1st
    if(gPgTable[Loc].dirty=='d' && relCache[Loc].valid=='v')
    {
        printf("\n\nflushRelCacheEntry(int Loc)....Calling FLUSH=%d",Loc);
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

    printf("flushRelCacheEntry(int Loc) gPg Flushed %d---relcahe[loc].dirty=%c",gPgFlushed,relCache[Loc].dirty);    
    if(gPgFlushed==1)//after success fully dealling with gPgTable
    {
        //Check if Cache Entry Is Dirty Or Not Corresponding To The Location That Is To Be Replaced
        if(relCache[Loc].dirty=='d')
        {
            //write this entry into relation and attribute Catalog
            //insert A Record At Particular Place
            makeRelCatRec(cacheEntry,Loc);//put the Cache Content in cacheEntry Rec
            printf("\n\n Printing RelCat Record that IS PREPAREd cahceEntry....");
            shwRelCatRec(cacheEntry);
            WriteRec(0,cacheEntry,&(relCache[Loc].Rid));//bug remove code with write checking
            
                //------------alTerNative Of ablove WriteRec -------------------
                //----------Quick Update--To Use Just CMNT ABOVE WriteRec and UNCMNT BELOVE PART----------
            /*
               ReadPage(0,relCache[Loc].Rid.pid);
                if(gPgTable[0].pid==relCache[Loc].Rid.pid)
                {
                 printf("Writing RelCache To Buffer");
                offset=MR_RELCAT_BITMS_NUM+relCache[0].recLength*relCache[Loc].Rid.slotnum;
                //shwRelCatRec(rec);
                //shwRelCatRec(&gPgTable[0].contents[offset]);
                for(int i=0;i<relCache[0].recLength;i++)
                {
                    gPgTable[0].contents[offset+i]=cacheEntry[i];
                }
                //shwRelCatRec(&gPgTable[0].contents[offset]);
                    gPgTable[0].dirty='d';
                    relCache[Loc].dirty='c';
                }
                 FlushPage(0,gPgTable[0].pid);
                //-----------------------------------
            */
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
    if(relCacheIndex < NUM_RELCACHE_ENTRY)//relCacheIndex<MAX_RELCACHE_ENTRY
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
    while(count<(NUM_RELCACHE_ENTRY-2)/3)
    {
        if(i<NUM_RELCACHE_ENTRY)
        {
            if(gPgTable[i].dirty=='c'||relCache[i].valid=='i')
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
