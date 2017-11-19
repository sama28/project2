//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include  <errno.h>
#include <string.h>
#include <stdio.h>
/*
//---------test stub---------------
int relCacheIndex=2;//temporary definition remove it ones ameya changes reflect
int relNum =1;
//int pid=1;
int pid=0;
relCacheEntry relCache[10];
*/
int FlushPage(int relNum,unsigned pgid);
int isPgInBuff(int relNum,unsigned pgid );
//int isPgInBuff(int relNum,unsigned pgid );
void showPageContent(int relNum);
void ReadPage(int relNum,unsigned pid)
{

int i;
unsigned len;
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
*/
//----------------------------------------
//----------------------------code part-----------------------------
   // printf("relCacheIndex =%d relCache%d.valid=%c",relCacheIndex,relNum,relCache[relNum].valid);
    // correct one//if(relNum < relCacheIndex && relCache[relNum].valid=='v')//relNum is available in relCache) 
    if(relCache[relNum].valid=='v' && relCache[relNum].relFile!=NULL)//temporaray        
        {
            
           // findPgInBuff(relNum,pid)//find pid in PGBUFF[relNum])
            if (gPgTable[relNum].pid!=pid)//pid is availble in PGBUFF)
            {
                //check whether dirty bit is set or not and page is valid or not
                if(gPgTable[relNum].dirty == 'd' && gPgTable[relNum].valid=='v')
                {
                    //insure that file is open in wb+ mode
                    //write the dirty page to the disk 1st
                    FlushPage(relNum,gPgTable[relNum].pid);
                }
                //read pid in gPgTable[relNum];
                //replace page if necessary
                //-----------------------------------------------
                if(pid<relCache[relNum].numPgs)
                {
                    fseek(relCache[relNum].relFile,PAGESIZE*pid,SEEK_SET);
                    len=fread(gPgTable[relNum].contents,PAGESIZE,1,relCache[relNum].relFile);
                    //printf("number of byte read %u",len);
                    if(!ferror(relCache[relNum].relFile))
                    {
                        gPgTable[relNum].pid=pid;
                        gPgTable[relNum].valid='v';
                        gPgTable[relNum].dirty='c';
                        //printf("%s",gPgTable[relNum].contents);            
                    }
                    else{

                        printf("\nIN READPAGE :-unable to read %u of %s ........",pid,relCache[relNum].relName);
                    }
                }
            }
            else
            {
                printf("\n\nIn ReadPage :- relNum=%d Is In Buffer...",relNum);
            }       
        }
        else//(relNum is not in cache)
        {
            printf("\n\nInRead PAge OPEN THE RELATION FIRST.....");
        }
}


//--------------------------------
//--buffer management related-----
//--prototypical instances and implementation so that buffermanagement could be decoupeled 
//--from design perspective
int isPgInBuff(int relNum,unsigned pgid )
{
    if(relNum < relCacheIndex && relCache[relNum].valid=='v')
    {
        if(gPgTable[relNum].pid==pgid)
        {
          //   printf("page in gt");           
            return 1;
        }      
    }
    //printf("page not in gt");
    
    return 0;
}
void showPageContent(int relNum)
{
  //  printf("\n\n------------------------------------------------");
    //printf("\n\nRELATION:-%s  PAGE %u",relCache[relNum].relName,gPgTable[relNum].pid);
    //printf("\n\n------------------------------------------------");


}
//currently working on show page content
/*
  else if(pid == relCache[relNum].numPgs)
                {
                    printf("\nINReadPage :-...NEW PAGE CREATED....for %d",pid);
                    for(i=0;i<PAGESIZE;i++)
                    {
                        gPgTable[pid].contents[i]='\0';
                        gPgTable[relNum].pid=pid;
                        gPgTable[relNum].valid='v';
                        gPgTable[relNum].dirty='c';
                    }
                }
                else{
                    printf("\n\nPID is Not valid");
                }
                */