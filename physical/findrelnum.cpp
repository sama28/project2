#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <string.h>
#include <stdio.h>
int FindRelNum(char *relName)
{   //returns the Relation Number On success Else Returns Falls
  int i;
    printf("\n\nFindRelNum args:%s  \n ",relName);
        i=0;
        while(i < NUM_RELCACHE_ENTRY)
        {
            printf("\n\n -> %c <- relcache[%d].valid",relCache[i].valid,i);
            if(relCache[i].valid == 'v')
            {
                //printf("\n\neror traped%d",i);
                if(!strcmp(relName,relCache[i].relName))
                {   
                    return i;
                }
            }
            i++;
        }
        printf("\n\nFindRelNum :- Relation is Not in Cache");
        return -1;
}
