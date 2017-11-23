
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include "../include/fncn.h"
#include <stdio.h>
#include<string.h>
#include<stdlib.h>


int argCheck3(int relNum[],int argc,char** argv){
    int valid=1,flag=0,attrOffset[2];

    for(int i=0;i<2;i++){
        flag=0;
        for(int j=0;j<relCache[relNum[i]].numAttrs && flag==0;j++){
            if(strcmp(relCache[relNum[i]].attrHead[j].attrName,argv[3+2*i])==0){
                flag=1;
                attrOffset[i]=relCache[relNum[i]].attrHead[j].offset;
            }
            //printf("%s\t%s\t%d\n",relCache[relNum[i]].attrHead[j].attrName,argv[3+2*i],flag);
        }
        if(flag==0){
            valid&=0;
            //printf("ZZ");
        }
        if(!valid){
            printf("Given join attribute %s not present in relation %s.\n",argv[i*2+3],relCache[relNum[i]].relName);
            return valid;
        }
    }

    if(relCache[relNum[0]].attrHead[attrOffset[0]].type!=relCache[relNum[1]].attrHead[attrOffset[1]].type){
        printf("Join attributes do not have the same types.\n");
        valid&=0;
        return valid;
    }
    printf("off\t%d\t%d\n",attrOffset[0],attrOffset[1]);
    int count=(relCache[relNum[0]].numAttrs+relCache[relNum[1]].numAttrs-1)*2+2;
    char *string[count];
    string[0]=(char*)malloc(7);
    strcpy(string[0],"create");
    string[1]=(char*)malloc(strlen(argv[1]));
    strcpy(string[1],argv[1]);
    int c=2,x;
    for(int i=0;i<2;i++){
        for(int j=0;j<relCache[relNum[i]].numAttrs;j++){
            if(!(i==1 && attrOffset[1]==j)){
                string[c]=(char*)malloc(strlen(relCache[relNum[i]].attrHead[j].attrName));
                sprintf(string[c],"Rel%d.",i+1);
                printf("\nhere%s\n",string[c]);
                x=strlen(relCache[relNum[i]].attrHead[j].attrName);
                if(strlen(relCache[relNum[i]].attrHead[j].attrName)==ATTRLEN){
                    x=ATTRLEN-6;
                }
                strncpy(string[c]+5,relCache[relNum[i]].attrHead[j].attrName,x);
                c++;

                switch(relCache[relNum[i]].attrHead[j].type){
                    case DTINT:{
                        string[c]=(char*)malloc(2);
                        strcpy(string[c],"i");
                    }break;

                    case DTFLOAT:{
                        string[c]=(char*)malloc(2);
                        strcpy(string[c],"f");
                    }break;

                    case DTSTRING:{
                        string[c]=(char*)malloc(4);
                        sprintf(string[c],"s%d",relCache[relNum[i]].attrHead[j].length-1);
                    }break;
                }
            c++;
            }
        }    
    }
    
    for(int i=0;i<count;i++){
        printf("%s\t%d\n",string[i],strlen(string[i]));
    }

    if(Create(count,string)!=1){
        valid&=0;
        return 0;
    }
    for(int i=0;i<count;i++)
        free(string[i]);
    return valid;
}

int Join (int argc, char **argv)
{
    int relNum[2];
    relNum[0]=2;relNum[1]=3;
    argCheck3(relNum,argc,argv);
    /*for(int i=0;i<argc;i++){
        printf("%s\n",argv[i]);
    }*/
     return (OK);  /* all's fine */
}


