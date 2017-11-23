#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
//relCache=2;//just for testing remove it compulsorily

int FlushPage(int relNum,unsigned pgid)
{
    int len;
    //assuming file is opened in ab+ mode directly write 
    //printf("insigth flush...");

    if(relCache[relNum].relFile!=NULL && relCache[relNum].valid=='v')
    {
        if(gPgTable[relNum].pid==pgid )
        {
            if(gPgTable[relNum].dirty == 'd' && gPgTable[relNum].valid=='v')
            {
                if(pgid < relCache[relNum].numPgs)
                {
                    fseek(relCache[relNum].relFile,PAGESIZE*pgid,SEEK_SET);
                    //fseek(relCache[relNum].relFile,0,SEEK_SET);
                    len=fwrite(gPgTable[relNum].contents,1,PAGESIZE,relCache[relNum].relFile);
                //    printf("\n\nlen:-%d\n\n",len);
                    if(!ferror(relCache[relNum].relFile))//actually condition shloud be ==PAGESIZE
                    {
                        fflush(relCache[relNum].relFile);
                        gPgTable[relNum].dirty='c';
              //          printf("\nflushing old page of relation%d :-\n\n%x",relNum,gPgTable[relNum].contents);
                        return 1;
                     }
                    else
                    {
            //            printf("\n\nflushpage():->there is problem in flushing the page");
                    }  
                }
                else
                {
          //          printf("\n\nflushpage():->Pid=%u Is Out Of Expected Range [0-%u]",pgid,relCache[relNum].numPgs);
                }
            }
            else{
                     
                //printf("\n\nFlushPage :- Page(pid=%d) Is Clean",gPgTable[relNum].pid);
                return 1;
            } 
        }
        else
        {
             //printf("\n\nFlushPage :- corresponding Page In Page Table Is Not Valid...");
        }
    }
    else
    {
        //printf("\n\nFLUSH PAGE:-relCache Is Not Valid Or Relation is not opened....");
    }
    return 0;
}

/*
 if(pgid==relCache[relNum].numPgs)
                        {
                            relCache[relNum].numPgs=relCache[relNum].numPgs+1;
                            relCache[relNum].dirty='d';
                        }
*/