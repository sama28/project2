#if 0
//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include <string.h>
#include <stdio.h>

//---------test stub---------------
int relCache1Index=2;//temporary definition remove it ones ameya changes reflect
int relNum =1;
//int pid=1;
int pid=0;
relCacheEntry relCache1[10];
main()
{
//-------------------------------------
//temporary---
char tempbuff[3*PAGESIZE];

//-------------------------------------------
int i,ret;
gPgTable[relNum].pid=1;
relCache1[1].dirty=0;

//gPgTable[relNum].pid=0;
//relCache1[1].dirty=1;
strcpy(gPgTable[relNum].contents,"this is the old content of page 1 of relation relnum 1");
relCache1[1].valid=1;
relCache1[1].relFile=fopen("/home/samadhan/Desktop/git/minirel/run/tempread.txt","rb+");
if(relCache1[1].relFile<=0)
{
    printf("unble to upen stub file for wb+");
}

/*`
relCache1[1].relName[RELNAME];
relCache1[1].recLength;
relCache1[1].recPerPg;
relCache1[1].numPgs;
relCache1[1].numRecs;
relCache1[1].numAttrs;
relCache1[1].Rid;
relCache1[1].relFile;
relCache1[1].dirty=0;
relCache1[1].attrHead;
*/
//----------------------------------------




//----------------------------code part-----------------------------

    //dependent code that comes here
     if(relNum < relCache1Index && relCache1[relNum].valid==1)//relNum is available in relCache1)       
        {
            
           // findPgInBuff(relNum,pid)//find pid in PGBUFF[relNum])
            if (!isPgInBuff(relNum,pid))//pid is not availble in PGBUFF)
            {
                //check whether dirty bit is set or not
                if(relCache1[relNum].dirty == 1)
                {
                    //insure that file is open in wb+ mode
                    //write the dirty page to the disk 1st
                    FlushPage(relNum,gPgTable[relNum].pid);
                }
                //read pid in gPgTable[relNum];
                //replace page if necessary
    
                //-------------------------------------------
                //--------------------------------------------
                //fseek(relCache1[relNum].relFile,0,SEEK_SET);
                fseek(relCache1[relNum].relFile,PAGESIZE*pid,SEEK_SET);
                //if(fread(tempbuff,1,PAGESIZE*3,relCache1[relNum].relFile) > 0)
                fread(gPgTable[relNum].contents,PAGESIZE,1,relCache1[relNum].relFile) ;
                if(!ferror(relCache1[relNum].relFile)> 0)
                {
                    gPgTable[relNum].pid=pid;
                    //printf("\n\ncontent of file As String:-%s",gPgTable[relNum].contents);  
                    int lwrlm=0;
                    int uprlm=PAGESIZE;
                    
                    printf("\n\ncontent of the whole file\n");
                    for( i=lwrlm;i<uprlm;i++)//read the content from lower limit to upper limit
                    {
                      printf("%c",gPgTable[relNum].contents[i]);
                      //printf("%c",tempbuff[i]);
                    }
                }
                else{

                    printf("\nunable to read page %d of relNum %d with erro %d........",pid,relNum,ferror(relCache1[relNum].relFile));
                }
           
            }        
        }
        else//(relNum is not in cache)
        {
            printf("\n\nOPEN THE RELATION FIRST.....");
        }
fclose(relCache1[relNum].relFile);
// printf("ReadPage \n ");
}


//--------------------------------
//--buffer management related-----
//--prototypical instances and implementation so that buffermanagement could be decoupeled 
//--from design perspective
int isPgInBuff(int relNum,unsigned pgid )
{
    if(relNum < relCache1Index && relCache1[relNum].valid==1)
    {
        if(gPgTable[relNum].pid==pgid)
        {
            printf("page in gt");
            
            return 1;
        }
        
    }
    printf("page not in gt:isPgInBuff retrurn 0");
    
    return 0;
}

FlushPage(int relNum,unsigned pgid)
{
    int len;
    //assuming file is opened in ab+ mode directly write 
    //printf("insigth flush...");
    fseek(relCache1[relNum].relFile,PAGESIZE*pgid,SEEK_SET);
    //fseek(relCache1[relNum].relFile,0,SEEK_SET);
    len=fwrite(gPgTable[relNum].contents,1,PAGESIZE,relCache1[relNum].relFile);
    printf("\n\nlen:-%d\n\n",len);
    if(len>0)//actually condition shloud be ==PAGESIZE
    {
        fflush(relCache1[relNum].relFile);
        relCache1[relNum].dirty=0;
        printf("\nflushing old page:-\n\n%s",gPgTable[relNum].contents);
    }
    else{

        printf("\n\nflushpage():->there is problem in flushing the page");
    }
}
#endif