
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include "../include/fncn.h"
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
int sanitize(unsigned char*,int);
void bwrite_float(unsigned char* tmp,float num,int size,int* init);
void bwrite_int(unsigned char* tmp,int num,int size,int* init);
void parseRecord(int relNum,char* record);

int argCheck(int argc,char** argv,int relNum,int attrFound[]){
    int attrCount=(argc-2)/2,valid=1;
    if(attrCount!=relCache[relNum].numAttrs){
        printf("Number of attributes entered does not equal number of attributes in %s.\n",argv[1]);
        return 0;
    }
    char seen[argc][ATTRLEN];
    for(int i=0;i<argc;i=i+1)
        strcpy(seen[i],argv[i]);
    
    for(int i=2;i<argc;i=i+2){
        for(int j=2;j<argc;j=j+2){
            if(i!=j && !strcmp(seen[i],argv[j])){
                valid=valid & 0;
            }
            else{
                valid=valid & 1;
            }
        }
        //printf("%s\n",seen[i]);
    }
    if(!valid){
        printf("Invalid attributes for relation %s.\n",argv[1]);
        return valid;
    }

    for(int i=0;i<relCache[relNum].numAttrs;i++){
        attrFound[i]=-1;
    }

    int k=0;
    for(int i=2;i<argc && valid;i+=2){
        int found=0;
        //printf("%s\n",argv[i]);
        for(int j=0;j<relCache[relNum].numAttrs && found==0;j++){
            if(!strcmp(argv[i],relCache[relNum].attrHead[j].attrName)){
                    attrFound[k]=relCache[relNum].attrHead[j].offset;
                    found=1;
            }
        }
        k++;
    }  
    //printf("valid %d\n",valid);
    for(int j=0;j<relCache[relNum].numAttrs && valid;j++){
        //printf("%d\t",attrFound[j]);
        if(attrFound[j]==-1)
            valid = valid & 0;
    }  
    
    if(!valid){
        printf("Attributes entered does not match the attributes of the relation.\n");
        return valid;
    }
    for(int i=0;i<3;i++){
        printf("%d\n",relCache[relNum].attrHead[attrFound[i]].type);
    }
    for(int i=2;i<argc;i+=2){
        switch(relCache[relNum].attrHead[attrFound[i/2-1]].type){
            
            case DTINT:{
                //printf("%s\t%s\n",argv[i],argv[i+1]);
                if(!(argv[i+1][0]>47 && argv[i+1][0]<58))
                    valid=valid & 0;
            }break;

            case DTFLOAT:{
                //printf("%s\t%s\n",argv[i],argv[i+1]);
                if(argv[i+1][0]>47 && argv[i+1][0]<58 )
                    valid=valid & 1;
                else
                    valid=valid & 0;
            }break;
                
            case DTSTRING:{
                if(strlen(argv[i+1])>=relCache[relNum].attrHead[attrFound[i/2-1]].length)
                    valid=valid & 0;
            }break; 
        }
    }

    if(!valid){
        printf("Invalid values given to attributes.\n");
        return valid;
    }
    return valid;
}

int calcOffset(int relNum,int num){
    int offset=0;
    for(int i=0;i<num;i++){
        offset+=relCache[relNum].attrHead[i].length;
    }
    return offset;
}

void trim(char* s){
    size_t size;
    char *end;
    int index=0;
    size = strlen(s);

    if (!size)
        return ;

    for(int i=strlen(s)-1;i>-1 && (s[i]==0x20 || s[i]=='\t');i--){
        s[i]=0x00;
    }

    for(int i=0;i<strlen(s) && (s[i]==0x20 || s[i]=='\t');i++)
        index++;
	//printf("\nindex%d\n",index);
	for(int i=0;i<strlen(s);i++){
		s[i]=s[i+index];
	}
}


int recordExist(int relNum,char* record){
    //Remove lines below.
    /*{
        char relPath[MAX_PATH_LENGTH];
        relCache[relNum].valid='v';
        getPath(relPath,"q");strcat(relPath,"/");strcat(relPath,"q");

        relCache[relNum].relFile=fopen(relPath,"rb+");
        gPgTable[relNum].pid=1;
    }
    ReadPage(relNum,0);*/

    Rid startRid,foundRid;
    startRid.pid=0;
    startRid.slotnum=0;
    char record2[relCache[relNum].recLength+1];
    int flag=0;
    for(int i=0;i<relCache[relNum].numRecs && flag==0;i++){
        if(GetNextRec(relNum,&startRid,&foundRid,record2)){
            printf("\n2 records below\n");
            for(int k=0;k<relCache[relNum].recLength;k++)
            printf("%02x",record[k]);
            printf("\n");
            for(int k=0;k<relCache[relNum].recLength;k++)
            printf("%02x",record2[k]);
            printf("\n");
            int n=memcmp(record,record2,relCache[relNum].recLength);
            if(n==0){
                flag=1;
                printf("\naa%d\n",n);
            }


            startRid.pid=foundRid.pid;
            startRid.slotnum=foundRid.slotnum+1;
            //printf("rid  %d\t%d\n",foundRid.pid,foundRid.slotnum);
            //parseRecord(relNum,record);
        }
        printf("\n%d)\n",i);
    }
    for(int k=0;k<relCache[relNum].recLength;k++)
    printf("%02x",record2[k]);printf("\n");
    return flag;
}

int Insert (int argc, char **argv)
{   
    if(MR_CURR_DB[0]==0){
        printf("No database is currently opened.\n");
        return 0;
    }
    int relNum=OpenRel(argv[1]);//use openrel here.
    int order[relCache[relNum].numAttrs];
    if(relNum<2 && relNum>=0){
        printf("You do not have permission to access this relation.\n");
        return 0;
    }
    if(relNum<0){
        printf("Relation %s not found.\n",argv[1]);
        return 0;
    }

    if(argCheck(argc,argv,relNum,order)==0){
        return 0;
    }
    
    unsigned char record[relCache[relNum].recLength+1];char* in;
    int offset=0;
    sanitize(record,relCache[relNum].recLength+1);
    for(int i=0;i<relCache[relNum].recLength;i++)
        printf("%02x",record[i]);printf("\n");

    for(int i=2;i<argc;i+=2){
        //printf("%d",calcOffset(relNum,order[i/2-1]));
        offset=calcOffset(relNum,order[i/2-1]);
        switch(relCache[relNum].attrHead[order[i/2-1]].type){
            case DTINT:{
                int a=atoi(argv[i+1]);
                printf("%d\n",a);
                bwrite_int(record,a,sizeof(int),&offset);
            }break;
            
            case DTFLOAT:{
                float f=strtof(argv[i+1],&in);
                printf("%f\n",f);
                bwrite_float(record,f,sizeof(float),&offset);
            }break;

            case DTSTRING:{
                printf("strlen :::%d\n",strlen(argv[i+1]));
                trim(argv[i+1]);
                printf("strlen :::%d\n",strlen(argv[i+1]));
                for(int j=0;j<strlen(argv[i+1])/*relCache[relNum].attrHead[order[i/2-1]].length*/;j++){
                    record[offset+j]=argv[i+1][j];
                }
            }break;
           
        }
         for(int i=0;i<relCache[relNum].recLength;i++)
        printf("%02x",record[i]);printf("\n");
    }

    parseRecord(relNum,(char*)record); 
    if(!recordExist(relNum,(char*)record)){
        printf("Record does not exist\n");
        InsertRec(relNum,record);
    }
    else{
        printf("Record is already present in the relation.\n");
    }
    return (OK);
}


