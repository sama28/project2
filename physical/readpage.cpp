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
//-------------------------------------
//temporary---
//char tempbuff[3*PAGESIZE];
//printf("\n\ninside read page");
//-------------------------------------------
int i;
unsigned len;
//gPgTable[relNum].pid=1;
//relCache[1].dirty=0;

//gPgTable[relNum].pid=0;
//relCache[1].dirty=1;
//strcpy(gPgTable[relNum].contents,"this is the old content of page 1 of relation relnum 1");
//relCache[1].valid=1;
//relCache[1].relFile=fopen("/home/samadhan/Desktop/git/minirel/run/tempread.txt","rb+");
//if(relCache[1].relFile<=0)
//{
//    printf("unble to upen stub file for wb+");
//}

/*`
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
    //temporary testing code must remove


    relCache[relNum].valid='v';//must remove BUG
    //-------------------------

   // printf("relCacheIndex =%d relCache%d.valid=%c",relCacheIndex,relNum,relCache[relNum].valid);
    // correct one//if(relNum < relCacheIndex && relCache[relNum].valid=='v')//relNum is available in relCache) 
    if(relCache[relNum].valid=='v')//temporaray        
        {
            
           // findPgInBuff(relNum,pid)//find pid in PGBUFF[relNum])
            if (!isPgInBuff(relNum,pid))//pid is not availble in PGBUFF)
            {


                //check whether dirty bit is set or not
                if(relCache[relNum].dirty == 'd')
                {
                    //insure that file is open in wb+ mode
                    //write the dirty page to the disk 1st
                    FlushPage(relNum,gPgTable[relNum].pid);
                }
                //read pid in gPgTable[relNum];
                //replace page if necessary
    
                //-------------------------------------------
                //--------------------------------------------
                //fseek(relCache[relNum].relFile,0,SEEK_SET);
                fseek(relCache[relNum].relFile,PAGESIZE*pid,SEEK_SET);
                //if(fread(tempbuff,1,PAGESIZE*3,relCache[relNum].relFile) > 0)
                len=fread(gPgTable[relNum].contents,PAGESIZE,1,relCache[relNum].relFile);
     //           printf("number of byte read %u",len);
                if(!ferror(relCache[relNum].relFile))
                {
                    gPgTable[relNum].pid=pid;
                    //printf("%s",gPgTable[relNum].contents);
                   
                  int lwrlm=0;
                  int uprlm=PAGESIZE;
       //           printf("\n\nreadPage:content of the whole file.... %s\n",strerror(errno));
                  for(i=lwrlm;i<uprlm;i++)//read the content from lower limit to upper limit
                  {
         //           printf("%x",gPgTable[relNum].contents[i]);
                    //printf("%c",tempbuff[i]);

                    //showPageContent(relNum);
                  }
           //       printf("\n\n1st Byte Of rel %d is %c",relNum,gPgTable[relNum].contents[1]);
                
                
                }
                else{

             //       printf("\nunable to read........");
                }
           
            }
            else
            {
               // printf("\n\nIn raedPage Page Is In Buffer...");
            }       
        }
        else//(relNum is not in cache)
        {
       //     printf("\n\nOPEN THE RELATION FIRST.....");
        }
       //fclose(relCache[relNum].relFile);
// printf("ReadPage \n ");

//printf("\n\nreadPage Exit...");
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
  //          printf("page in gt");
            
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
