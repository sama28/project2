#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <string.h>
#include <stdio.h>
int FindRelNum(char *relName)
<<<<<<< HEAD
{   //returns the Relation Number On success Else Returns Falls
  int i;
    printf("\n\nFindRelNum args:%s  \n ",relName);
=======
{
  int i;
    printf("\n\nFindRelNum  \n ");
>>>>>>> f3f3b1399efc1d0ba65c3ee303626aa2e017f706
        i=0;
        while(i<=relCacheIndex)
        {
            if(!strcmp(relName,relCache[i].relName))
            {   
                return i;
            }
            else
            {
                i++;
<<<<<<< HEAD
                printf("\n\nname does not match...");
            }

        }
        printf("\n\nPrintf");
        return -1;
}
=======
                //printf("\n\nname does not match...");
            }

        }
        return -1;
}
>>>>>>> f3f3b1399efc1d0ba65c3ee303626aa2e017f706
