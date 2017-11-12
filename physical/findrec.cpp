#if 0 
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include "../include/fncn.h"


int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp )
{
 //printf("FindRec \n ");
int status;
attrList source;
source.next=NULL;        
//GetNextRec(int relNum,recid * startRid,recid* foundRid,void * recPtr)
status=GetNextRec(relNum,startRid,foundRid,recPtr);
//find record start from startRid
    if(status==1)//if record is found sucessfully
    {
        attrList source;
        source.next=NULL;
        //find attr with Given offset
        //returnAttrNode(struct attrList* attrListHead,int offset,struct attrList* source)
        
        //returnAttrNode(relCache[relNum].attrHead,offset,&source);
        if(source.type==attrType && source.length==attrSize)
        {
            if(isRecRight(relNum,recPtr,source,))
        }
    }
}
int isRecRight(int relNum,char *recPtr,attrList *source,int offset,char *valuePtr,int compOp )
{   //checkes if Record Conforms To The Desired Test Parameter As Provided In Arguments 
  int bOfst;
  bOfst=attrOfstInRec(recPtr,relNum,offset);
    switch(source->type)
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
            if(unsignshortcomp(*(unsigned short*)(recPtr+bOfst)),*(unsigned short*)valuePtr,compOp)
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
            if(unsignintcomp(*((unsigned int *)(recPtr+bOfst)) == *(unsigned int *)valuePtr,compOp))
            {
                return 1;
            }
        break;

        case DTSTRING:
           if(strncomp((recPtr+bOfst),valuePtr,source.length,compOp))
            {
                return 1;
            }
        break;
        
        case DTFLOT:
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
    attrList source;
    source.next=NULL;
    int len=0;
    for(int i=0;i<offset;i++)
    {
        //returnAttrNode(relCache[relNum].attrHead,i,&source);
        len=len+source.length;
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
#endif