#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>

//int	argc;
//char	**argv;
//extern "C" int Select(int ,char **);
int Select (int argc, char ** argv)
{
    /* print command line arguments */
    short	k;		/* iteration counter	    */
    printf ("%s:\n", argv[0]);
    for (k = 1 ; k < argc; ++k)
	printf ("\targv[%d] = %s\n", k, argv[k]);

     printf("Select c++ ......fine....\n");



     //1.0Create New Relation
       // crtNewRelAsOld();


     return (OK);  /* all's fine */
}
int crtNewRelAsOld(char * newRelName,char *oldRelName )
{   //create New Relation As Old One 

   // printf();
    int oldRelNum=-1,newRelNum=-1,i;
    oldRelNum=OpenRel(oldRelName);
    int argc;
    char *argv[relCache[2*oldRelNum].numAttrs+3];//two for 0.create,1 newRelName, Last NULL,
    char type[relCache[oldRelNum].numAttrs+1][8];
    char typestr[8];
    argv[0]="create";
    argv[1]=newRelName;
    argv[2*relCache[oldRelNum].numAttrs+2]=NULL;

    if(oldRelNum>=MR_FIRST_USR_RLTN_IND)
    {
        i=0;
        while(i<relCache[oldRelNum].numAttrs)
        {
            argv[2*i+2]=relCache[oldRelNum].attrHead[i].attrName;
            //argv[2*i+3]=relCache[oldRelNum].attrHead[i].type;
            cnvrtTypeNumToStr(relCache[oldRelNum].attrHead[i].type,typestr,relCache[oldRelNum].attrHead[i].length);
            strcpy(type[i],typestr);
            argv[2*i+3]=type[i];
            i++;
        }
        Create(argc,argv);
    }
    

}
void cnvrtTypeNumToStr(int num,char *str,int len)
{  
    char slen[4];
    switch(num)
    {
        case DTCHAR:
            str[0]='c';
            str[1]='\0';
            str[2]='\0';
        break;

        case DTUNSIGNED_CHAR:
            str[0]='k';
            str[1]='\0';
            str[2]='\0';
        break;

        case DTSHORT:
            str[0]='l';
            str[1]='\0';
            str[2]='\0';

        break;

        case DTUNSIGNED_SHORT:
            str[0]='m';
            str[1]='\0';
            str[2]='\0';
     
        break;

        case DTINT:
            str[0]='i';
            str[1]='\0';
            str[2]='\0';

        break;

        case DTUNSIGNED_INT:
            str[0]='u';
            str[1]='\0';
            str[2]='\0';

        break;

        case DTSTRING:
           sprintf(slen,"%d",len);
            str[0]='s';
            str[1]='\0';
            strcat(str,slen);
        break;
        case DTFLOAT:

            str[0]='f';
            str[1]='\0';
            str[2]='\0';

        break;

    }
    
}
