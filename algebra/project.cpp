
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include "../include/fncn.h"
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
int Create (int argc, char **argv);
int calcOffset(int relNum,int num);
void bwrite_float(unsigned char* tmp,float num,int size,int* init);
void bwrite_int(unsigned char* tmp,int num,int size,int* init);

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

void foo(int argc,char** argv){
    printf("argc %d\n",argc);
    for(int i=0;i<argc;i++){
        printf("%s\n",argv[i]);
    }
}

int Project (int argc, char **argv)
{
    if(MR_CURR_DB[0]==0){
        printf("No database currently opened.\n");
        return NO_DB_OPEN;
    }
    int relNum=OpenRel(argv[2]);//call openrel here.
    /*{
        char relPath[MAX_PATH_LENGTH];
        relCache[relNum].valid='v';
        getPath(relPath,"q");strcat(relPath,"/");strcat(relPath,"q");

        relCache[relNum].relFile=fopen(relPath,"rb+");
        gPgTable[relNum].pid=1;
    }ReadPage(relNum,0);*/
    if(relNum<2 && relNum>=0){
        printf("You do not have permission to access this relation.\n");
        return 0;
    }
    if(relNum<0){
        printf("Relation %s not found.\n",argv[1]);
        return 0;
    }

    int order[argc-3],recLength=0;
    if(!argCheck2(relNum,argc,argv,order)){
        return 0;
    }

    char *string[2*(argc-3)+2];
    int count=0;

    printf("%d\n",2*argc-4);
    string[0]="create";
    string[1]=(char*)malloc(strlen(argv[1]));
    strcpy(string[1],argv[1]);
   
    for(int i=3;i<argc;i++){
        string[2*i-4]=(char*)malloc(strlen(argv[i]));
        strcpy(string[2*i-4],argv[i]);
        switch (relCache[relNum].attrHead[order[i-3]].type){
            case DTINT:{
                string[2*i-3]="i";
            }break;

            case DTFLOAT:{
                string[2*i-3]="f";
            }break;

            case DTSTRING:{
                string[2*i-3]=(char*)malloc(3);
                sprintf(string[2*i-3],"s%d",relCache[relNum].attrHead[order[i-3]].length-1);
            }break;
        }
    }
    string[2*(argc-3)+2]='\0';
    /*for(int i=0;i<2*argc-3;i++){
        printf("%s\n",string[i],strlen(string[i]));
    }*/
    Create(2*(argc-3)+2,string);

    int relNum2=OpenRel(argv[1]);
    /*{
        char relPath[MAX_PATH_LENGTH];
        relCache[relNum2].valid='v';
        getPath(relPath,"w");strcat(relPath,"/");strcat(relPath,"w");

        relCache[relNum2].relFile=fopen(relPath,"rb+");
        gPgTable[relNum2].pid=1;
    }*/
    if(relNum2<2 && relNum2>=0){
        printf("You do not have permission to access this relation.\n");
        return 0;
    }
    if(relNum2<0){
        printf("Relation %s not found.\n",argv[1]);
        return 0;
    }

    for(int i=3;i<argc;i++){
        printf("%s\t%d\t%d\n",relCache[relNum].attrHead[order[i-3]].attrName,calcOffset(relNum2,i-3),calcOffset(relNum,order[i-3]));
    }  
    
    int offset,offset2;
    Rid startRid,foundRid;
    startRid.pid=0;
    startRid.slotnum=0;
    unsigned char record[relCache[relNum].recLength+1],record2[relCache[relNum2].recLength+1];
    int flag=0;
    for(int j=0;j<relCache[relNum].numRecs;j++){
        if(GetNextRec(relNum,&startRid,&foundRid,(char*)record)){
            for(int k=0;k<relCache[relNum].recLength;k++)printf("%02x",record[k]);printf("\n");
            for(int i=0;i<relCache[relNum2].numAttrs;i++){
                offset2=calcOffset(relNum2,i);
                offset=calcOffset(relNum,order[i]);
                switch(relCache[relNum2].attrHead[i].type){
                    case DTINT:{
                        int a=*(int*)(record+offset);
                        bwrite_int(record2,a,sizeof(int),&offset2);
                    }break;

                    case DTFLOAT:{
                        float f=*(float*)(record+offset);
                        bwrite_float(record2,f,sizeof(float),&offset2);
                    }break;

                    case DTSTRING:{
                        char ch[relCache[relNum2].attrHead[i].length+1];
                        strncpy(ch,(char*)(record+offset),relCache[relNum2].attrHead[i].length);
                        sprintf((char*)(record2+offset2),"%s",ch);
                    }break;
                }
            }
            printf("\n");
            for(int l=0;l<relCache[relNum2].recLength;l++)
            printf("%02x",record2[l]);printf("\n");
            int a;float f;char ch[35];
            strncpy(ch,(char*)(record2),35);
            f=*(float*)(record2+35);
            printf("%s\t%f\t\n",ch,f);
            InsertRec(relNum2,record2);
            startRid.pid=foundRid.pid;
            startRid.slotnum=foundRid.slotnum+1;
        }
    }
    return (OK);
}


