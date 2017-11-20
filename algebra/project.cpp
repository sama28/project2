
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include "../include/fncn.h"
#include <stdio.h>
#include<string.h>
#include<stdlib.h>

int argCheck2(int relNum,int argc,char** argv,int order[]){
    int valid=1;
    if(argc-3>relCache[relNum].numAttrs){
        printf("Number of attributes exceeds the number of attributes in the relation.\n");
        return valid & 0;
    }

    int flag;
    for(int i=3;i<argc;i++){
        flag=0;
        for(int j=0;j<relCache[relNum].numAttrs && flag==0;j++){
            if(!strcmp(argv[i],relCache[relNum].attrHead[j].attrName) ){
                flag=1;
                order[i-3]=relCache[relNum].attrHead[j].offset;
            }
            //printf("%s\t%s\t%d\t%d\n",argv[i],relCache[relNum].attrHead[j].attrName,j,flag);
        }
        
        if(flag==0){
            valid=valid & 0;
        }
    }
    if(valid==0){
        printf("Attributes entered does not match the attributes of the relation.\n");
    }

    return valid;
}

int Project (int argc, char **argv)
{
    if(MR_CURR_DB[0]==0){
        printf("No database currently opened.\n");
        return NO_DB_OPEN;
    }
    int relNum=2;//call openrel here.
    {
        char relPath[MAX_PATH_LENGTH];
        relCache[relNum].valid='v';
        getPath(relPath,"q");strcat(relPath,"/");strcat(relPath,"q");

        relCache[relNum].relFile=fopen(relPath,"rb+");
        gPgTable[relNum].pid=1;
    }

    int order[argc-3],recLength=0;
    if(!argCheck2(relNum,argc,argv,order)){
        return 0;
    }

    for(int i=3;i<argc;i++){
        printf("%s\t%d\n",argv[i],order[i-3]);
        recLength+=relCache[relNum].attrHead[order[i-3]].length;
    }
    printf("%d\n",recLength);
    char record[recLength+1];
    char *string[2*(argc-3)+3];
    int count=0;

    printf("%d\n",2*argc-3);
    string[0]="create";
    string[1]=(char*)malloc(strlen(argv[1]));
    strcpy(string[1],argv[1]);
    for(int i=3;i<argc;i++){
        string[2*i-4]=(char*)malloc(strlen(argv[1]));
        strcpy(string[2*i-4],argv[i]);
        switch (relCache[relNum].attrHead[order[i-2]].type){
            case DTINT:{
                string[2*i-3]="i";
            }break;

            case DTFLOAT:{
                string[2*i-3]="f";
            }break;

            case DTSTRING:{
                string[2*i-3]="s";
            }break;
        }
    }
printf("aaaa\n");
    for(int i=0;i<2*argc-3;i++){
        printf("%s\n",string[i]);
    }
    return (OK);
}


