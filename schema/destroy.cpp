
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <stdio.h>

int Destroy(int argc,char ** argv)
{
    char path[MAX_PATH_LENGTH];
    getPath(path,argv[1]);
    return (OK);  /* all's fine */
}


