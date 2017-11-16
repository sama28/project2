
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
int OpenRel(char * relName);
void CloseRel(int relNum);

int Load (int argc,char ** argv)
{
    char rel[RELNAME],data[50],recPtr[100];
    strcpy(rel,argv[1]);
    strcpy(data,argv[2]);
    int relNum=OpenRel(rel);
    printf("%s\t%s\t%d\n",rel,data,relNum);
    //printf("realtion opened successfully%s\t%d\t%d\n",relCache[7].relName,relCache[7].attr0Rid.pid,relCache[7].attr0Rid.slotnum);
    char *dataPath;
    getPath(dataPath,data);
    FILE* fp=fopen(dataPath,"rb");
    /*if(fp==NULL){
        printf("Unable to open data file.\n");
        return 0;
    }*/


    unsigned char datapage[PAGESIZE+1];
    //read
    return (OK);  /* all's fine*/
}


