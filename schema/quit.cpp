
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <stdio.h>

int CloseDB (int argc,char ** argv);

int Quit(int argc,char ** argv)
{
    //printf("inside quit");
    if(MR_CURR_DB[0]!=0){
        CloseDB(argc,argv);
        //printf("1");
    }
    //printf("2");
}


