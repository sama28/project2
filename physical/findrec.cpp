#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include "../include/fncn.h"
#include "string.h"
int charcomp(char a,char b,int compOp);
int unsigncharcomp(unsigned char a,unsigned char b,int compOp);
int intcomp(int a,int b,int compOp);
int unsignintcomp(unsigned int a,unsigned int b,int compOp);
int shortcomp(short a,short b,int compOp);
int unsignshortcomp(unsigned short a,unsigned short b,int compOp);
int strncomp(char* a,char* b,int len,int compOp);int floatcomp(float a,float b,int compOp);

int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp )
{
 printf("FindRec :relNum=%d value =%s compOp=%d\n ",relNum,valuePtr,compOp);
int status,flag=1;      
//GetNextRec(int relNum,recid * startRid,recid* foundRid,void * recPtr)

status=GetNextRec(relNum,startRid,foundRid,recPtr);
//find record start from startRid
    while(flag==1)
    {
        if(status==1)//if record is found sucessfully
        {
            //find attr with Given offset
            //returnAttrNode(struct attrList* attrListHead,int offset,struct attrList* source)
        
            // returnAttrNode(relCache[relNum].attrHead,offset,&source);

            if(relCache[relNum].attrHead[offset].type==attrType && relCache[relNum].attrHead[offset].length==attrSize)
            {
                if(isRecRight(relNum,recPtr,offset,valuePtr,compOp))
                {
                    printf("In FindRec: record Found....");
                     return 1;
                }
                else
                {
                    printf("\n\nIn FindRec: Comparision Returns False");
                }
            }
             else{

                printf("\n\nIn FindRec: attrType Or attrlength mismatch with disired attr..");
            }        
        }
         else{

            printf("\n\nIn FindRec: GetNext Fail In Finding Rec");
            flag=0;
            return 0;
        }
        if(foundRid->slotnum < relCache[relNum].numRecs-1)//-1 is Important foundRid shows the Rid Of
        {                                                 // Found Rec and That is Not Desired

            startRid->slotnum=foundRid->slotnum+1;
        }
        else
        {
            startRid->pid=foundRid->pid+1;
            startRid->slotnum=0;
        }
        status=GetNextRec(relNum,startRid,foundRid,recPtr);
    }
    return 0;
}
int isRecRight(int relNum,char *recPtr,int offset,char *valuePtr,int compOp )
{   //checkes if Record Conforms To The Desired Test Parameter As Provided In Arguments 
  int bOfst;
  bOfst=attrOfstInRec(recPtr,relNum,offset);
    switch(relCache[relNum].attrHead[offset].type)
    {
        case DTCHAR:
            if(charcomp(*((char*)(recPtr+bOfst)),*(char*)valuePtr,compOp))
            {
                return 1;
            }
        break;

        case DTUNSIGNED_CHAR:
            if(unsigncharcomp(*((unsigned char*)(recPtr+bOfst)),*(unsigned char*)valuePtr,compOp))
            {
                return 1;
            }
        break;

        case DTSHORT:
            if(shortcomp(*((short *)(recPtr+bOfst)),*(short*)valuePtr,compOp))
            {
                return 1;
            }
        break;

        case DTUNSIGNED_SHORT:
            if( unsignshortcomp(*((unsigned short*)(recPtr+bOfst)),*(unsigned short*)valuePtr,compOp))
            {
                return 1;
            }        
        break;

        case DTINT:
            if(intcomp(*((int *)(recPtr+bOfst)),*(int *)valuePtr,compOp))
            {
                return 1;
            }
        break;

        case DTUNSIGNED_INT:
            if(unsignintcomp(*((unsigned int *)(recPtr+bOfst)),*(unsigned int *)valuePtr,compOp))
            {
                return 1;
            }
        break;

        case DTSTRING:
           if(strncomp((recPtr+bOfst),valuePtr,relCache[relNum].attrHead[offset].length,compOp))
            {
                return 1;
            }
        break;
        
        
        case DTFLOAT:
            if(floatcomp(*((float *)(recPtr+bOfst)),*(float *)valuePtr,compOp))
            {
                return 1;
            }
        break;
    }
    return 0;
}

unsigned attrOfstInRec(char *recPtr,int relNum,int offset)
{       //return the index to the 1st byte of nth attribute into recPtr(n==offset)
    int len=0;
    for(int i=0;i<offset;i++)
    {
        //returnAttrNode(relCache[relNum].attrHead,i,&source);
        len=len+relCache[relNum].attrHead[i].length;
    }
    return len;
}

int charcomp(char a,char b,int compOp)
{

    switch(compOp)
    {
        case EQ:
            if(a == b){return 1;}
        break;
        
        case GT:
            if(a > b){return 1;}
        break;

        case GTEQ:
            if(a >= b){return 1;}
        break;

        case LT:
            if(a < b){return 1;}
        break;

        case LTEQ:
            if(a<+b){return 1;}
        break;

        case NE:
            if(a!=b){return 1;}
        break;
    }
    return 0;
}
int unsigncharcomp(unsigned char a,unsigned char b,int compOp)
{

    switch(compOp)
    {
        case EQ:
            if(a == b){return 1;}
        break;
        
        case GT:
            if(a > b){return 1;}
        break;

        case GTEQ:
            if(a >= b){return 1;}
        break;

        case LT:
            if(a < b){return 1;}
        break;

        case LTEQ:
            if(a<+b){return 1;}
        break;

        case NE:
            if(a!=b){return 1;}
        break;
    }
    return 0;
}
int intcomp(int a,int b,int compOp)
{

    switch(compOp)
    {
        case EQ:
            if(a == b){return 1;}
        break;
        
        case GT:
            if(a > b){return 1;}
        break;

        case GTEQ:
            if(a >= b){return 1;}
        break;

        case LT:
            if(a < b){return 1;}
        break;

        case LTEQ:
            if(a<+b){return 1;}
        break;

        case NE:
            if(a!=b){return 1;}
        break;
    }
    return 0;
}
int unsignintcomp(unsigned int a,unsigned int b,int compOp)
{

    switch(compOp)
    {
        case EQ:
            if(a == b){return 1;}
        break;
        
        case GT:
            if(a > b){return 1;}
        break;

        case GTEQ:
            if(a >= b){return 1;}
        break;

        case LT:
            if(a < b){return 1;}
        break;

        case LTEQ:
            if(a<+b){return 1;}
        break;

        case NE:
            if(a!=b){return 1;}
        break;
    }
    return 0;
}
int shortcomp(short a,short b,int compOp)
{

    switch(compOp)
    {
        case EQ:
            if(a == b){return 1;}
        break;
        
        case GT:
            if(a > b){return 1;}
        break;

        case GTEQ:
            if(a >= b){return 1;}
        break;

        case LT:
            if(a < b){return 1;}
        break;

        case LTEQ:
            if(a<+b){return 1;}
        break;

        case NE:
            if(a!=b){return 1;}
        break;
    }
    return 0;
}
int unsignshortcomp(unsigned short a,unsigned short b,int compOp)
{

    switch(compOp)
    {
        case EQ:
            if(a == b){return 1;}
        break;
        
        case GT:
            if(a > b){return 1;}
        break;

        case GTEQ:
            if(a >= b){return 1;}
        break;

        case LT:
            if(a < b){return 1;}
        break;

        case LTEQ:
            if(a<+b){return 1;}
        break;

        case NE:
            if(a!=b){return 1;}
        break;
    }
    return 0;
}
int strncomp(char* a,char* b,int len,int compOp)
{

    switch(compOp)
    {
        case EQ:
            printf("strnEQcalled with a=%s b=%s",a,b);
            if(!strncmp(a,b,len)){return 1;}
        break;
        
        case GT:
            if(strncmp(a,b,len)>0){return 1;}
        break;

        case GTEQ:
            if(strncmp(a,b,len)>=0){return 1;}
        break;

        case LT:
            if(strncmp(a,b,len)<0){return 1;}
        break;

        case LTEQ:
            if(strncmp(a,b,len)<=0){return 1;}
        break;

        case NE:
            if(strncmp(a,b,len)!=0){return 1;}
        break;
    }
    return 0;
}
int floatcomp(float a,float b,int compOp)
{

    switch(compOp)
    {
        case EQ:
            if(a == b){return 1;}
        break;
        
        case GT:
            if(a > b){return 1;}
        break;

        case GTEQ:
            if(a >= b){return 1;}
        break;

        case LT:
            if(a < b){return 1;}
        break;

        case LTEQ:
            if(a<+b){return 1;}
        break;

        case NE:
            if(a!=b){return 1;}
        break;
    }
    return 0;
}