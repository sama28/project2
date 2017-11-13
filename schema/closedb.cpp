
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <stdio.h>

int CloseDB (int argc,char ** argv)
{
    if(MR_CURR_DB[0]==0){
        printf("No database currently opened.\n");
        return NO_DB_OPEN;
    }

    
    MR_CURR_DB[0]=0;
    /* FLush all dirty global pages.
    for(int i=0;i<20;i++){
        if(gPgTable[i].dirty=='d'){
            FlushPage(i,gPgTable[i].pid);
        }
    }*/
    CloseCats();
     return (OK);  /* all's fine */
}


