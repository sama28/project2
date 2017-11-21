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
    //-----------samadhan addition---------
    gPgTable[0].pid=0;gPgTable[0].valid='v';gPgTable[0].dirty='c';
    gPgTable[1].pid=0;gPgTable[1].valid='v';gPgTable[1].dirty='c';
    for (int i=0;i<PAGESIZE;i++)
        printf("%02x",gPgTable[0].contents[i]);
    relcat_index+=MR_RELCAT_BITMS_NUM;
    unsigned char tmp[32];
    bread_string((unsigned char*)gPgTable[0].contents,32,&relcat_index,tmp);
    strcpy(relCache[0].relName,(char*)tmp);
    relCache[0].recLength=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[0].recPerPg=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[0].numPgs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[0].numRecs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[0].numAttrs=bread_int((unsigned char*)gPgTable[0].contents,2,&relcat_index);
    relCache[0].attr0Rid.pid=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[0].attr0Rid.slotnum=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[0].Rid.pid=0;
    relCache[0].Rid.slotnum=0;
    relCache[0].relFile=relcatFile;
    relCache[0].dirty='c';
    relCache[0].valid='v';
    /*int i=0;
    struct recid startRid1,foundRid1;
    char rec2[58];
    for(int i=0;i<2;i++){
        startRid1.pid=0;
        startRid1.slotnum=i;
        if(GetNextRec(0,&startRid1,&foundRid1,rec2)){
        int offset=0;
        strncpy(relCache[i].relName,rec2,32);offset+=32;
        relCache[i].recLength=*(unsigned*)(rec2+offset);offset+=4;
        printf("reclen%d\n",relCache[i].recLength);
        relCache[i].recPerPg=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].numPgs=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].numRecs=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].numAttrs=*(unsigned short*)(rec2+offset);offset+=2;
        relCache[i].attr0Rid.pid=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].attr0Rid.slotnum=*(unsigned*)(rec2+offset);offset+=4;
        relCache[i].Rid.pid=0;
        relCache[i].Rid.slotnum=1;
        //relCache[1].relFile=attrcatFile;
        relCache[i].dirty='c';
        relCache[i].valid='v';
        }
    }*/
    

    printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\n",relCache[0].relName,relCache[0].recLength,relCache[0].recPerPg,relCache[0].numPgs,relCache[0].numRecs,relCache[0].numAttrs,relCache[0].Rid.pid,relCache[0].Rid.slotnum,relCache[0].dirty);
    

    bread_string((unsigned char*)gPgTable[0].contents,32,&relcat_index,tmp);
    strcpy(relCache[1].relName,(char*)tmp);
    relCache[1].recLength=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[1].recPerPg=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[1].numPgs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[1].numRecs=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[1].numAttrs=bread_int((unsigned char*)gPgTable[0].contents,2,&relcat_index);
    relCache[1].attr0Rid.pid=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[1].attr0Rid.slotnum=bread_int((unsigned char*)gPgTable[0].contents,4,&relcat_index);
    relCache[1].Rid.pid=0;
    relCache[1].Rid.slotnum=1;
    relCache[1].relFile=attrcatFile;
    relCache[1].dirty='c';
    relCache[1].valid='v';
    //printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\n",relCache[1].relName,relCache[1].recLength,relCache[1].recPerPg,relCache[1].numPgs,relCache[1].numRecs,relCache[1].numAttrs,relCache[1].Rid.pid,relCache[1].Rid.slotnum,relCache[1].dirty);

    
    char rec[relCache[1].recLength];
    
    for(int j=0;j<2;j++){
        struct recidArray RidArray[relCache[j].numAttrs];
        getSlots2(1,&RidArray[0],relCache[j].attr0Rid,relCache[j].numAttrs);
        for(int i=0;i<relCache[j].numAttrs;i++)
            printf("\nslotsattrcache\t%d\t%d\n",(RidArray+i)->Rid.pid,(RidArray+i)->Rid.slotnum);
        
        for(int i=0;i<relCache[j].numAttrs;i++){
            int attrcat_index=0;
            Rid startRid,foundRid;
            startRid.pid=(RidArray+i)->Rid.pid;
            startRid.slotnum=(RidArray+i)->Rid.slotnum;
            GetNextRec(1,&startRid,&foundRid,rec);
            
            //printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            struct attrList obj;
            strncpy(obj.attrName,rec,32);attrcat_index+=32;
            obj.offset=*(unsigned*)(rec+attrcat_index);attrcat_index+=4;
            obj.length=*(unsigned*)(rec+attrcat_index);attrcat_index+=4;
            obj.type=*(unsigned short*)(rec+attrcat_index);attrcat_index+=2;
            relCache[j].attrHead.push_back(obj);
            //printf("attrcacheloc\t%d\t%d\n",foundRid.pid,foundRid.slotnum);
        }
        printf("pid%d\t%d\n",relCache[j].attr0Rid.pid,relCache[j].attr0Rid.slotnum);
    }
    /* //Print attribute cache
    for(int j=0;j<2;j++){
        for(int i=0;i<relCache[j].numAttrs;i++){
            printf("qwert %s %u %u \n",relCache[j].attrHead[i].attrName,relCache[j].attrHead[i].offset,relCache[j].attrHead[i].length,relCache[j].attrHead[i].type);
        }printf("\n");}*/
    
    
    //printf("%d",relcat_index);
    
    
    relCacheIndex=2;
}

void cachePopulate2(FILE* relcatFile,FILE* attrcatFile){
    int howmuchRec;
    if(relCache[0].numRecs<MAXOPEN){
        howmuchRec=relCache[0].numRecs;
    }
    else{
        howmuchRec=MAXOPEN;
    }

    printf("howmuch%d\n",relCache[0].numRecs);
    //for(int k=0;k<512;k++)
    //printf("%02x",gPgTable[0].contents[k]);
    Rid startRid1,foundRid1,attrcatRid;
    int offset;
    startRid1.pid=0;startRid1.slotnum=2;
    for (int i=2;i<8;i++){
        //printf("%d\t%d\n",startRid1.pid,startRid1.slotnum);
        
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
            relCache[i].attr0Rid.pid=*(unsigned*)(rec2+offset);offset+=4;;
            relCache[i].attr0Rid.slotnum=*(unsigned*)(rec2+offset);offset+=4;;
            relCache[i].Rid.pid=foundRid1.pid;
            relCache[i].Rid.slotnum=foundRid1.slotnum;

        }
        relCache[i].relFile=NULL;
        relCache[i].dirty='c';
        relCache[i].valid='i';//Read a page;
          //------------samadhan addition-----------
        gPgTable[i].pid=1;
        gPgTable[i].valid='i';
        gPgTable[i].dirty='c';


        startRid1.slotnum=foundRid1.slotnum+1;
        startRid1.pid=foundRid1.pid;
        if(startRid1.slotnum>=relCache[0].recPerPg){
            startRid1.pid++;
            startRid1.slotnum=0;
        }
        int attrcat_index=0;
        struct recidArray RidArray[relCache[i].numAttrs];
        getSlots2(1,&RidArray[0],relCache[i].attr0Rid,relCache[i].numAttrs);
        for(int j=0;j<relCache[i].numAttrs;j++)
            printf("\nslotsattrcache\t%d\t%d\n",(RidArray+j)->Rid.pid,(RidArray+j)->Rid.slotnum);
         char rec[relCache[1].recLength];
        for(int j=0;j<relCache[i].numAttrs;j++){
            //printf("rid of alpha%d in attrcat%d\t%d\n",j+1,(RidArray+j)->Rid.pid,(RidArray+j)->Rid.slotnum);
            int attrcat_index=0;
            Rid startRid,foundRid;
            startRid.pid=(RidArray+j)->Rid.pid;
            startRid.slotnum=(RidArray+j)->Rid.slotnum;
            GetNextRec(1,&startRid,&foundRid,(char*)rec);
            
            //printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            struct attrList obj;
            strncpy(obj.attrName,rec,32);attrcat_index+=32;
            obj.offset=*(unsigned*)(rec+attrcat_index);attrcat_index+=4;
            obj.length=*(unsigned*)(rec+attrcat_index);attrcat_index+=4;
            obj.type=*(unsigned short*)(rec+attrcat_index);attrcat_index+=2;
            relCache[i].attrHead.push_back(obj);
            printf("attrcacheloc\t%d\t%d\n",foundRid.pid,foundRid.slotnum);
        }
        //relCache[i].attrHead=attrListHead;
        
        
        //printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\n",relCache[i].relName,relCache[i].recLength,relCache[i].recPerPg,relCache[i].numPgs,relCache[i].numRecs,relCache[i].numAttrs,relCache[i].Rid.pid,relCache[i].Rid.slotnum,relCache[i].dirty);
        relCacheIndex++;
    }
      
    for(int j=0;j<howmuchRec;j++){
        for(int i=0;i<relCache[j].numAttrs;i++){
            printf("%s %s %u %u \n",relCache[j].relName,relCache[j].attrHead[i].attrName,relCache[j].attrHead[i].offset,relCache[j].attrHead[i].length,relCache[j].attrHead[i].type);
        }printf("\n");}
      
    for(int i=0;i<howmuchRec;i++) 
    printf("Relname\t%s\nreclength\t%x\nrecprpg\t%x\nnumpgs\t%x\nnumrecs\t%x\nnumattr\t%x\npid\t%x\nslotnum\t%x\ndirty\t%c\nattr0 %d %d\n",relCache[i].relName,relCache[i].recLength,relCache[i].recPerPg,relCache[i].numPgs,relCache[i].numRecs,relCache[i].numAttrs,relCache[i].Rid.pid,relCache[i].Rid.slotnum,relCache[i].dirty,relCache[i].attr0Rid.pid,relCache[i].attr0Rid.slotnum);

}

void OpenCats(void )
{
    printf("reached opencat");
    int flag=1,size; 
    char path1[MAX_PATH_LENGTH],*c,*d,path2[MAX_PATH_LENGTH];
    

    strcpy(path1,HOME_MINIREL);
    strcpy(path2,HOME_MINIREL);
    c=strcat(path1,"/data/");
    d=strcat(path2,"/data/");
    c=strcat(c,MR_CURR_DB);
    d=strcat(d,MR_CURR_DB);
    c=strcat(c,"/catalog/relcat");
    d=strcat(d,"/catalog/attrcat");
    chmod(c,S_IWUSR|S_IRUSR);
    chmod(d,S_IWUSR|S_IRUSR);
    FILE *relcatFile=fopen(c,"rb+");
    FILE *attrcatFile=fopen(d,"rb+");
    if(relcatFile!=NULL && attrcatFile!=NULL){
        /*buffer=(unsigned int*)malloc(PAGESIZE/sizeof(unsigned int));
        fread(buffer,sizeof(unsigned int),PAGESIZE/sizeof(unsigned int),relcatFile);
        size=fileSize(relcatFile);
        for(int i=0;i<size/(sizeof(unsigned int));i++)
            printf("%x\n",buffer[i]);*/
        
        //fread(&slot_buffer,BITMS_NUM*sizeof(unsigned int),1,relcatFile);
        
        cachePopulate1(relcatFile,attrcatFile);
        cachePopulate2(relcatFile,attrcatFile);
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