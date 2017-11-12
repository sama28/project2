//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include "../include/fncn.h"
#include  <string.h>
//int nextRec(int relNum, Rid* startRid, char *recPtr);
//unsigned firstRecSlotInd(int relNum);
//int isPgInBuff(int relNum, unsigned pgid);
unsigned int i;

//-------------------------------------------
int nextRec(int relNum, Rid* startRid, char *recPtr);
unsigned firstRecSlotInd(int relNum);
void ReadPage(int relNum,unsigned pid);

int GetNextRec(int relNum,Rid * startRid,Rid* foundRid,char * recPtr)
{
 printf("GetNextRec \n ");

 int bitmapByteNum;
 int bitmapbitnum;
 unsigned char cmprtr1, cmprtr2;
 unsigned fstrecind;

 foundRid->pid=startRid->pid;
 foundRid->slotnum=startRid->slotnum;     
 //------------------------------------------------
  if (relCache[relNum].relFile!=NULL)
  { //relation is open and entries are there in relCache
        while(foundRid->pid < relCache[relNum].numPgs)
        {
                ReadPage(relNum, foundRid->pid);
                 //1. Read the Required Page In gPgTable
                //2.If Page Is Read Successfully And Available In gPgTable
                if (gPgTable[relNum].pid == foundRid->pid)
                {
                        while(foundRid->slotnum < relCache[relNum].numRecs)
                        {
                         //pid is valid and not the outside of the page range                   
                          bitmapByteNum = foundRid->slotnum / 8;
                          bitmapbitnum = foundRid->slotnum % 8;
                          cmprtr1 = 0x80;
                          cmprtr1 = cmprtr1 >> bitmapbitnum;
                          cmprtr2 = gPgTable[relNum].contents[bitmapByteNum];
                          //printf("\n%x===cmprtr1(%x) & cmprtr2(%x)",(cmprtr1 & cmprtr2),cmprtr1,cmprtr2);
                          if ((cmprtr1 & cmprtr2) != 0x00)
                          { //slot is valid so record exists

                                  //finding the corresponding record slot
                                  fstrecind=firstRecSlotInd(relNum);
                                  fstrecind=fstrecind+relCache[relNum].recLength*foundRid->slotnum;
                                  //offset to the desired record;
                                  for(i=0;i<relCache[relNum].recLength;i++)
                                  {
                                      recPtr[i]=gPgTable[relNum].contents[fstrecind+i];
                                  }
                                  //memcpy(recPtr, &gPgTable[relNum].contents[fstrecind],relCache[relNum].recLength);
                                  printf("\n\nInGetnextRec : Rec Copied SuccessFully...");
                                  return 1;
                          }
                             //printf("\n\nIN GetNextRec There Is No Rec At Given Slot No");
                            foundRid->slotnum++;
                        }        
                }
                else{
                      printf("\n\nIn GetnextRec Failed To Bring The Required Page");
                      return -3;
                  }
                  foundRid->pid++;
                  foundRid->slotnum=0;
        }
        return -2;
        printf("\n\nIN GetNextRec RECORD NOT FOUND...");
   }      
  else{
      printf("\n\nIN GetNextRec RELATION IS NOT OPENED....");
      return -1;
  }
  
  return 0;
 //---------------------------------------------

}

//---------------------------------------------
//----------------------------------------------
int nextRec(int relNum, Rid* startRid, char *recPtr)
{
        int bitmapByteNum;
        int bitmapbitnum;
        unsigned char cmprtr1, cmprtr2;
        unsigned fstrecind;
        //cmprtr=cmprtr>>bitmapbitnum;

        if (relCache[relNum].relFile!=NULL)
        { //relation is open and entries are there in relCache

                if (startRid->pid < relCache[relNum].numPgs && startRid->slotnum < relCache[relNum].numRecs)
                { //pid is valid and not the outside of the page range

                        
                        ReadPage(relNum, startRid->pid);
                        //1. Read the Required Page In gPgTable
                        
                        //2.If Page Is Read Successfully And Available In gPgTable
                        if (gPgTable[relNum].pid == startRid->pid)
                        {
                                bitmapByteNum = startRid->slotnum / 8;
                                bitmapbitnum = startRid->slotnum % 8;
                                cmprtr1 = 0x80;
                                cmprtr1 = cmprtr1 >> bitmapbitnum;
                                cmprtr2 = gPgTable[relNum].contents[bitmapByteNum];

                                if ((cmprtr1 & cmprtr2) != 0)
                                { //slot is valid so record exists

                                        //finding the corresponding record slot
                                        fstrecind=firstRecSlotInd(relNum);
                                        fstrecind=fstrecind+relCache[relNum].recLength*startRid->slotnum;
                                        //offset to the desired record;
                                        for(i=0;i<relCache[relNum].recLength;i++)
                                        {
                                            recPtr[i]=gPgTable[relNum].contents[fstrecind+i];
                                        }
                                        //memcpy(recPtr, &gPgTable[relNum].contents[fstrecind],relCache[relNum].recLength);
                                        printf("\n\nInnextRec : Rec Copied SuccessFully...");
                                        return 1;
                                }
                                else
                                {
                                    printf("\n\nIn nextRec There Is No Rec At Given Slot No");
                                    return -4;
                                }
                        }
                        else{
                            printf("\n\nIn nextRec global Page Table Doesn't Contain The Required Page gPgTable[relNum].pid == startRid->pid");
                            return -3;

                        }
                }
                else
                {
                        //record is not in the File
                        printf("\n\nIn nextRec:Record Is Not In The File");
                        return -2;
                 }
        }
        else{

            printf("\n\nERROR IN GETNEXTREC relCache[relNum].relFile = NULL ");
            return -1;
        }
        return 0;
}

//return the index of 1st byte of recordSlot [Bitmap|recordSlot] byte
unsigned firstRecSlotInd(int relNum)
{
        return (unsigned)(((PAGESIZE - PGTAIL_SPACE) / (8 * relCache[relNum].recLength + 1))+1);
}
//----------------------------------------------------
/*
int isPgInBuff(int relNum, unsigned pgid)
{
        if (relNum < relCacheIndex && relCache[relNum].valid == 'v')
        {
                if (gPgTable[relNum].pid == pgid)
                {
                        printf("page in gt");

                        return 1;
                }
        }
        printf("page not in gt");

        return 0;
}
*/
/*
void ReadPage(int relNum, unsigned pid)
{

        //-------------------------------------
        //temporary---
        char tempbuff[3 * PAGESIZE];

        //-------------------------------------------
        int i;
        gPgTable[relNum].pid = 1;
        relCache[1].dirty = 0;

        //gPgTable[relNum].pid=0;
        //relCache[1].dirty=1;
        strcpy(gPgTable[relNum].contents, "this is the old content of page 1 of relation relnum 1");
        relCache[1].valid = 1;

        char path[MAX_PATH_LENGTH] = "/home/samadhan/Desktop/customminirel3/data/sam1/catalog/attrcat";
        chmod(path, S_IRUSR | S_IWUSR | S_ISVTX | O_DIRECTORY);
        fd = fopen(path, "rb+");

        relCache[1].relFile = fd //fopen("/home/samadhan/Desktop/git/minirel/run/tempread.txt","rb+");
            if (relCache[1].relFile <= 0)
        {
                printf("unble to upen stub file for wb+");
        }

        /*
relCache[1].relName[RELNAME];
relCache[1].recLength;
relCache[1].recPerPg;
relCache[1].numPgs;
relCache[1].numRecs;
relCache[1].numAttrs;
relCache[1].Rid;
relCache[1].relFile;
relCache[1].dirty=0;
relCache[1].attrHead;
*//*

        //----------------------------------------

        //----------------------------code part-----------------------------

        //dependent code that comes here
        if (relNum < relCacheIndex && relCache[relNum].valid == 1) //relNum is available in relCache)
        {

                // findPgInBuff(relNum,pid)//find pid in PGBUFF[relNum])
                if (!isPgInBuff(relNum, pid)) //pid is not availble in PGBUFF)
                {
                        //check whether dirty bit is set or not
                        if (relCache[relNum].dirty == 1)
                        {
                                //insure that file is open in wb+ mode
                                //write the dirty page to the disk 1st
                                FlushPage(relNum, gPgTable[relNum].pid);
                        }
                        //read pid in gPgTable[relNum];
                        //replace page if necessary

                        //-------------------------------------------
                        //--------------------------------------------
                        //fseek(relCache[relNum].relFile,0,SEEK_SET);
                        fseek(relCache[relNum].relFile, PAGESIZE * pid, SEEK_SET);
                        //if(fread(tempbuff,1,PAGESIZE*3,relCache[relNum].relFile) > 0)
                        if (fread(gPgTable[relNum].contents, PAGESIZE, 1, relCache[relNum].relFile) > 0)
                        {
                                gPgTable[relNum].pid = pid;
                                //printf("%s",gPgTable[relNum].contents);

                                int lwrlm = 0;
                                int uprlm = PAGESIZE;
                                printf("\n\ncontent of the whole file\n");
                                for (i = lwrlm; i < uprlm; i++) //read the content from lower limit to upper limit
                                {
                                        printf("%c", gPgTable[relNum].contents[i]);
                                        //printf("%c",tempbuff[i]);
                                }
                        }
                        else
                        {

                                printf("\nunable to read........");
                        }
                }
        }
        else //(relNum is not in cache)
        {
                printf("\n\nOPEN THE RELATION FIRST.....");
        }
        fclose(relCache[relNum].relFile);
        // printf("ReadPage \n ");
}
*/

//--------------------------------
//--buffer management related-----
//--prototypical instances and implementation so that buffermanagement could be decoupeled
//--from design perspective

/*
FlushPage(int relNum, unsigned pgid)
{
        int len;
        //assuming file is opened in ab+ mode directly write
        //printf("insigth flush...");
        fseek(relCache[relNum].relFile, PAGESIZE * pgid, SEEK_SET);
        //fseek(relCache[relNum].relFile,0,SEEK_SET);
        len = fwrite(gPgTable[relNum].contents, 1, PAGESIZE, relCache[relNum].relFile);
        printf("\n\nlen:-%d\n\n", len);
        if (len > 0) //actually condition shloud be ==PAGESIZE
        {
                fflush(relCache[relNum].relFile);
                relCache[relNum].dirty = 0;
                printf("\nflushing old page:-\n\n%s", gPgTable[relNum].contents);
        }
        else
        {

                printf("\n\nflushpage():->there is problem in flushing the page");
        }
}
*/

/*
int main()
{
        int rln = 1;
        Rid foundRid, startRid;
        startRid.pid = 0;
        startRid.slotnum = 6;
        char record[MR_ATTRCAT_REC_SIZE]; //attrcat rec size

        nextrec(rln, &startRid, &foundRid, record);
        printf("%s", record);
}
*/
