#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <string.h>
void DeleteRec(int relNum,Rid* rid){
    //printf("%d\n%d ",rid->pid,rid->slotnum);
    int count=0,isOnlyRec=0,offset,newslot,tmp,slotByte,slotIndex,slot,slotArray[8],maxRec,NUM_SLOTS=(((PAGESIZE-PGTAIL_SPACE)/(8*relCache[relNum].recLength+1))+1);
    maxRec=(PAGESIZE-PGTAIL_SPACE-NUM_SLOTS)/relCache[relNum].recLength;
    unsigned char rec[relCache[relNum].recLength+1];
    for(int i=0;i<relCache[relNum].recLength;i++){
        rec[i]=0;
    }
    if(rid->pid>relCache[relNum].numPgs-1){
        printf("Pid exceeds total pages in the file.\n");
        return;
    }
    if(rid->slotnum>maxRec-1){
        printf("Rid exceeds total records in the page.\n");
        return;
    }
    char path[255];
    getPath(path,relCache[relNum].relName);
    if(!strcmp("relcat",relCache[relNum].relName) || !strcmp("attrcat",relCache[relNum].relName)){
        relCache[relNum].relFile=fopen(path,"rb+");
    }

    if(relCache[relNum].relFile!=NULL){
        unsigned int slotArray[8];
        unsigned char write_this_slot;
        if(gPgTable[relNum].pid!=rid->pid){
            fseek(relCache[relNum].relFile,rid->pid*PAGESIZE,SEEK_SET);
            fread(&gPgTable[relNum].contents,PAGESIZE,1,relCache[relNum].relFile);
            gPgTable[relNum].pid=rid->pid;
        }
        for(int i=0;i<NUM_SLOTS;i++){
            tmp=i;
            slot=bread_int((unsigned char*)&gPgTable[relNum].contents,1,&tmp);
            getBinary((unsigned int *)&slotArray,slot);
            for(int j=0;j<8;j++){
                if(slotArray[j]==1)
                    count++;
            }
        }printf("count%d\n",count);
        if(count==1){
            isOnlyRec=1;
        }
        //for(int j=0;j<PAGESIZE;j++)
        //printf("%02x",gPgTable[relNum].contents[j]);
        slotByte=rid->slotnum/8;
        tmp=slotByte;
        slot=bread_int((unsigned char*)&gPgTable[relNum].contents,1,&tmp);
        getBinary((unsigned int *)&slotArray,slot);
        //for(int i=0;i<8;i++)
        //printf("%d",slotArray[i]);
        slotIndex=rid->slotnum%8;
        printf("\n%d\n",slotIndex);
        slotArray[slotIndex]=0;
        newslot=getDecimal((unsigned int *)&slotArray);
        write_this_slot=newslot;
        //printf("%d",write_this_slot);
        offset=slotByte;
        gPgTable[relNum].contents[slotByte]=write_this_slot;
        offset=NUM_SLOTS+(slotByte*8+slotIndex)*relCache[relNum].recLength;
        //printf("offset%d\n",offset+relCache[relNum].recLength);
        for(int i=0;i<relCache[relNum].recLength;i++)
            gPgTable[relNum].contents[i+offset]=rec[i];
        //for(int j=0;j<PAGESIZE;j++)
        //printf("%02x",gPgTable[relNum].contents[j]);
        fseek(relCache[relNum].relFile,PAGESIZE*rid->pid,SEEK_SET);
        fwrite(&gPgTable[relNum].contents,PAGESIZE,1,relCache[relNum].relFile);
        fflush(relCache[relNum].relFile);
        relCache[relNum].numRecs--;
        relCache[relNum].dirty='d';
        /*
        if(isOnlyRec && relCache[relNum].numPgs==1){
            relCache[relNum].numPgs--;
        }*/
        printf("numpgs%dn",relCache[relNum].numPgs);
       /* if(!strcmp("relcat",relCache[relNum].relName) || !strcmp("attrcat",relCache[relNum].relName)){
            struct stat st;
            mode_t mode;
            stat(path, &st);
            mode = st.st_mode & 07777;
        
            // modify mode
            mode &= ~(S_IWUSR);    // Clear this bit 
            chmod(path, mode);
        }*/
    }
    else if(errno){
        printf("%s\n",strerror(errno));
    }
}