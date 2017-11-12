#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
#include<vector>
using namespace std;

void getSlots2(int relNum,struct recidArray* RidArray,Rid attrcat0,int count){
    //printf("aws%d\n",(RidArray+1)->Rid.pid);
    int pagenum=attrcat0.pid,slotnum=attrcat0.slotnum;
    if(slotnum>=relCache[relNum].recPerPg)
        return;
    for(int i=0;i<count;i++){
        if(slotnum>=relCache[relNum].recPerPg){
            pagenum++;
            slotnum=0;
        }
        (RidArray+i)->Rid.pid=pagenum;
        (RidArray+i)->Rid.slotnum=slotnum;
        slotnum++;
    }
}


void cachePopulate1(FILE* relcatFile, FILE* attrcatFile){
    int relcat_index=0,attrcat_index=0,fsize=fileSize(relcatFile);
    unsigned int offset,length;
    unsigned short type;
    char attrName[ATTRLEN];
    fread(gPgTable[0].contents,PAGESIZE,1,relcatFile);
    fread(gPgTable[1].contents,PAGESIZE,1,attrcatFile);
    gPgTable[0].pid=0;
    gPgTable[1].pid=0;
    //for (int i=0;i<PAGESIZE/4;i++)
      //  printf("%x",relcat_page[i]);
    relcat_index+=MR_RELCAT_BITMS_NUM;
    unsigned char tmp[32];
    bread_string((unsigned char*)gPgTable[0].contents,32,&relcat_index,tmp);
    strcpy(relCache[relCacheIndex].relName,(char*)tmp);
    relCache[relCacheIndex].recLength=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].recPerPg=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].numPgs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].numRecs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].numAttrs=bread_int((unsigned char*)gPgTable[0].contents,2,&relcat_index);
    struct recid attrcatRid;
    attrcatRid.pid=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    attrcatRid.slotnum=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].attr0Rid.pid=attrcatRid.pid;
    relCache[relCacheIndex].attr0Rid.slotnum=attrcatRid.slotnum;
    relCache[relCacheIndex].Rid.pid=0;
    relCache[relCacheIndex].Rid.slotnum=0;
    relCache[relCacheIndex].relFile=relcatFile;
    relCache[relCacheIndex].dirty='c';
    relCache[relCacheIndex].valid='v';
    //printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\n",relCache[0].relName,relCache[0].recLength,relCache[0].recPerPg,relCache[0].numPgs,relCache[0].numRecs,relCache[0].numAttrs,relCache[0].Rid.pid,relCache[0].Rid.slotnum,relCache[0].dirty);
    relCacheIndex++;

    bread_string((unsigned char*)gPgTable[0].contents,32,&relcat_index,tmp);
    strcpy(relCache[relCacheIndex].relName,(char*)tmp);
    relCache[relCacheIndex].recLength=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].recPerPg=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].numPgs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].numRecs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[relCacheIndex].numAttrs=bread_int((unsigned char*)gPgTable[0].contents,2,&relcat_index);
    relCache[relCacheIndex].attr0Rid.pid=attrcatRid.pid;
    relCache[relCacheIndex].attr0Rid.slotnum=attrcatRid.slotnum;
    relCache[relCacheIndex].Rid.pid=0;
    relCache[relCacheIndex].Rid.slotnum=1;
    relCache[relCacheIndex].relFile=attrcatFile;
    relCache[relCacheIndex].dirty='c';
    relCache[relCacheIndex].valid='v';
    //printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\n",relCache[1].relName,relCache[1].recLength,relCache[1].recPerPg,relCache[1].numPgs,relCache[1].numRecs,relCache[1].numAttrs,relCache[1].Rid.pid,relCache[1].Rid.slotnum,relCache[1].dirty);
    
/*
    struct recidArray RidArray[relCache[0].numAttrs];
    getSlots2(1,&RidArray[0],attrcatRid,relCache[0].numAttrs);
    Rid startRid,foundRid;
    startRid.pid=attrcatRid.pid;
    startRid.slotnum=attrcatRid.slotnum;
    //for(int i=0;i<8;i++)
    //printf("%d\t%d\n",(RidArray+i)->Rid.pid,(RidArray+i)->Rid.slotnum);
    unsigned char* rec;
    //for(int i=0;i<relCache[0].numAttrs;i++){
        attrcat_index=0;
        GetNextRec(1,&startRid,&foundRid,(char*)rec);
        bread_string(rec,32,&attrcat_index,tmp);
        strcpy(attrName,(char*)tmp);
        offset=bread_int(rec,4,&attrcat_index);
        length=bread_int(rec,4,&attrcat_index);
        type=bread_int(rec,2,&attrcat_index);
        printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
    //}*/
    
    fseek(attrcatFile,attrcatRid.pid*PAGESIZE,SEEK_SET);
    fread(gPgTable[1].contents,sizeof(gPgTable[1].contents),1,attrcatFile);
    attrcat_index+=MR_ATTRCAT_BITMS_NUM+attrcatRid.slotnum*relCache[1].recLength;
    for(int j=0;j<2;j++){
        struct attrList* attrListHead=NULL;
        for(int i=0;i<relCache[j].numAttrs;i++){
            bread_string((unsigned char*)gPgTable[1].contents,32,&attrcat_index,tmp);
            strcpy(attrName,(char*)tmp);
            offset=bread_int((unsigned char*)gPgTable[1].contents,4,&attrcat_index);
            length=bread_int((unsigned char*)gPgTable[1].contents,4,&attrcat_index);
            type=bread_int((unsigned char*)gPgTable[1].contents,2,&attrcat_index);
            //printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            //attrListHead=addAttrListNode(attrListHead,attrName,offset,length,type);
            struct attrList obj;
            strcpy(obj.attrName,attrName);
            obj.offset=offset;
            obj.length=length;
            obj.type=type;
            relCache[j].attrHead.push_back(obj);
        }
    }
    /* //Print attribute cache
    for(int j=0;j<2;j++){
        for(int i=0;i<relCache[j].numAttrs;i++){
            printf("qwert %s %u %u \n",relCache[j].attrHead[i].attrName,relCache[j].attrHead[i].offset,relCache[j].attrHead[i].length,relCache[j].attrHead[i].type);
        }printf("\n");}*/
    
    attrcatRid.pid=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    attrcatRid.slotnum=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    //printf("%d",relcat_index);
    int howmuchRec;
    relCacheIndex++;
    
    if(relCache[0].numRecs<20){
        howmuchRec=relCache[0].numRecs;
    }
    else{
        howmuchRec=20;
    }
    //for(int k=0;k<512;k++)
    //printf("%02x",gPgTable[0].contents[k]);
    Rid startRid1,foundRid1;gPgTable[0].pid=1;
    startRid1.pid=0;startRid1.slotnum=2;
    for (int i=2;i<howmuchRec;i++){
        //printf("%d\t%d\n",startRid1.pid,startRid1.slotnum);
        relCacheIndex++;
        char rec2[relCache[0].recLength+1];
        if(GetNextRec(0,&startRid1,&foundRid1,rec2)){
        int offset=0;
        strncpy(relCache[i].relName,rec2,32);offset+=32;
        relCache[i].recLength=*(unsigned*)(rec2+offset);offset+=4;
        printf("reclen%d\n",relCache[i].recLength);
        relCache[i].recPerPg=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].numPgs=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].numRecs=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].numAttrs=*(unsigned short*)(rec2+offset);offset+=2;
        attrcatRid.pid=*(unsigned*)(rec2+offset);offset+=4;
        attrcatRid.slotnum=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].attr0Rid.pid=attrcatRid.pid;
        relCache[i].attr0Rid.slotnum=attrcatRid.slotnum;
            
        }
           
        startRid1.slotnum=foundRid1.slotnum+1;
        startRid1.pid=foundRid1.pid;
        if(startRid1.slotnum>=relCache[0].recPerPg){
            startRid1.pid++;
            startRid1.slotnum=0;
        }
        /*
        bread_string(gPgTable[0].contents,32,&relcat_index,tmp);
        strcpy(relCache[i].relName,(char*)tmp);
        relCache[i].recLength=bread_int(gPgTable[0].contents,4,&relcat_index);
        relCache[i].recPerPg=bread_int(gPgTable[0].contents,4,&relcat_index);
        relCache[i].numPgs=bread_int(gPgTable[0].contents,4,&relcat_index);
        relCache[i].numRecs=bread_int(gPgTable[0].contents,4,&relcat_index);
        relCache[i].numAttrs=bread_int(gPgTable[0].contents,2,&relcat_index);
        attrcatRid.pid=bread_int(gPgTable[0].contents,4,&relcat_index);
        attrcatRid.slotnum=bread_int(gPgTable[0].contents,4,&relcat_index);
        relCache[i].attr0Rid.pid=attrcatRid.pid;
        relCache[i].attr0Rid.slotnum=attrcatRid.slotnum;
        */
        attrcat_index=0;
        struct recidArray RidArray[relCache[i].numAttrs];
        getSlots2(1,&RidArray[0],attrcatRid,relCache[i].numAttrs);
        
        unsigned char rec[relCache[1].recLength];
        for(int j=0;j<relCache[i].numAttrs;j++){
            //printf("rid of alpha%d in attrcat%d\t%d\n",j+1,(RidArray+j)->Rid.pid,(RidArray+j)->Rid.slotnum);
            int attrcat_index=0;
            Rid startRid,foundRid;
            startRid.pid=(RidArray+j)->Rid.pid;
            startRid.slotnum=(RidArray+j)->Rid.slotnum;
            GetNextRec(1,&startRid,&foundRid,(char*)rec);
            bread_string(rec,32,&attrcat_index,tmp);
            strcpy(attrName,(char*)tmp);
            offset=bread_int(rec,4,&attrcat_index);
            length=bread_int(rec,4,&attrcat_index);
            type=bread_int(rec,2,&attrcat_index);
            //printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            struct attrList obj;
            strcpy(obj.attrName,attrName);
            obj.offset=offset;
            obj.length=length;
            obj.type=type;
            relCache[i].attrHead.push_back(obj);
        }
        //relCache[i].attrHead=attrListHead;
        relCache[i].Rid.pid=0;
        relCache[i].Rid.slotnum=i;
        relCache[i].relFile=NULL;
        relCache[i].dirty='c';
        relCache[i].valid='i';//Read a page;
        //printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\n",relCache[i].relName,relCache[i].recLength,relCache[i].recPerPg,relCache[i].numPgs,relCache[i].numRecs,relCache[i].numAttrs,relCache[i].Rid.pid,relCache[i].Rid.slotnum,relCache[i].dirty);
    
    }
    /*    
    for(int j=0;j<relCache[0].numRecs;j++){
        for(int i=0;i<relCache[j].numAttrs;i++){
            printf("%s %s %u %u \n",relCache[j].relName,relCache[j].attrHead[i].attrName,relCache[j].attrHead[i].offset,relCache[j].attrHead[i].length,relCache[j].attrHead[i].type);
        }printf("\n");}
      */
    for(int i=0;i<relCache[0].numRecs;i++) 
    printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\n",relCache[i].relName,relCache[i].recLength,relCache[i].recPerPg,relCache[i].numPgs,relCache[i].numRecs,relCache[i].numAttrs,relCache[i].Rid.pid,relCache[i].Rid.slotnum,relCache[i].dirty);
    
}

void OpenCats(void )
{
    printf("reached opencat");
    int flag=1,size; 
    unsigned int slot_buffer[BITMS_NUM*sizeof(unsigned int)],*buffer;
    char path1[MAX_PATH_LENGTH],*c,*d,path2[MAX_PATH_LENGTH];
    

    strcpy(path1,HOME_MINIREL);
    strcpy(path2,HOME_MINIREL);
    c=strcat(path1,"/data/");
    d=strcat(path2,"/data/");
    c=strcat(c,MR_CURR_DB);
    d=strcat(d,MR_CURR_DB);
    c=strcat(c,"/catalog/relcat");
    d=strcat(d,"/catalog/attrcat");
    FILE *relcatFile=fopen(c,"rb");
    FILE *attrcatFile=fopen(d,"rb");
    if(relcatFile!=NULL && attrcatFile!=NULL){
        /*buffer=(unsigned int*)malloc(PAGESIZE/sizeof(unsigned int));
        fread(buffer,sizeof(unsigned int),PAGESIZE/sizeof(unsigned int),relcatFile);
        size=fileSize(relcatFile);
        for(int i=0;i<size/(sizeof(unsigned int));i++)
            printf("%x\n",buffer[i]);*/
        
        //fread(&slot_buffer,BITMS_NUM*sizeof(unsigned int),1,relcatFile);
        
        cachePopulate1(relcatFile,attrcatFile);
    }
    else if(errno){
        printf("%s",strerror(errno));
    }
    
    /*if (dir){
        while((dp =readdir(dir))!=NULL && flag){
          if(!(p=strcmp(dp->d_name,argv[1])))
            flag=0;
        }
        
      }
      else if (errno){
          printf("\n---------------------------------------------------\n");
          printf("Error opening database.\n%s---------------------------\n",strerror(errno));
          printf("\n---------------------------------------------------\n");
      }*/
}