
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <stdio.h>
int FindRelNum(char *relName);
void getSlots2(int relNum,struct recidArray* RidArray,Rid attrcat0,int count);
void DeleteRec(int relNum,Rid* rid);
int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp );
int recursive_delete(char* dir_path,int error);

int Destroy(int argc,char ** argv)
{
    if(MR_CURR_DB[0]==0){
        printf("No database currently opened.\n");
        return -1;
    }

    int relNum=OpenRel(argv[1]);
    if(relNum>=0){
        FlushPage(relNum,gPgTable[relNum].pid);
        Rid tmpRid;
        //printf("%d\n",relNum);
        //printf("%d\t%d\n",relCache[relNum].Rid.pid,relCache[relNum].Rid.slotnum);
        tmpRid.pid=relCache[relNum].Rid.pid;
        tmpRid.slotnum=relCache[relNum].Rid.slotnum;
        DeleteRec(0,&tmpRid);
        struct recidArray RidArray[relCache[relNum].numAttrs];
        printf("rrrr%d\t%d\n",relCache[relNum].attr0Rid.pid,relCache[relNum].attr0Rid.slotnum);
        getSlots2(1,&RidArray[0],relCache[relNum].attr0Rid,relCache[relNum].numAttrs);
        for(int i=0;i<relCache[relNum].numAttrs;i++){
            //printf("%d\t%d\n",(RidArray+i)->Rid.pid,(RidArray+i)->Rid.slotnum);
            tmpRid.pid=(RidArray+i)->Rid.pid;
            tmpRid.slotnum=(RidArray+i)->Rid.slotnum;
            //printf("%d\t%d\n",tmpRid.pid,tmpRid.slotnum);
            DeleteRec(1,&tmpRid);
        }
        
        //printf("numrec%d\t%d\n",relCache[0].numRecs,relCache[1].numRecs);
        char path[MAX_PATH_LENGTH];
        getPath(path,relCache[relNum].relName);
        if(!recursive_delete(path,1)){
            printf("Error deleting the relation.\n");
        }
        printf("path  %s",path);
        relCache[relNum].valid='i';
        relCache[relNum].attrHead.clear();
        /*
        insert code to delete btree indices for the relation*/
        return (OK);
    }
    else{
        printf("Relation %s does not exist.\n",argv[1]);
        return (NOTOK);
    }
     /* all's fine */
}


