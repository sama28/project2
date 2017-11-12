#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <string.h>
#include <stdio.h>
int FindRelNum(char *relName)
{
  int i;
    printf("\n\nFindRelNum \n ");
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
                printf("\n\nname does not match...");
            }

        }
        return -1;
}
