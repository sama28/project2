#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>

//int	argc;
//char	**argv;
//extern "C" int Select(int ,char **);
int Select (int argc, char ** argv)
{
#if 0 
    /* print command line arguments */
    short	k;		/* iteration counter	    */
    printf ("%s:\n", argv[0]);
    for (k = 1 ; k < argc; ++k)
	printf ("\targv[%d] = %s\n", k, argv[k]);

     printf("Select c++ ......fine....\n");


    int newRelNum;
    Rid startRid,foundRid;
    
     //1.0Create New Relation
       crtNewRelAsOld(argv[1],argv[2]);

    //2.OpenNew Relation
        newRelNum=OpenRel(argv[1]);

    char recPtr[relCache[newRelNum].recLength];

    //3.Read Record In New Relation That Satiesfies The Condition
        readRecInNewRel()


     return (OK);  /* all's fine */
}
int crtNewRelAsOld(char * newRelName,char *oldRelName )
{   //create New Relation As Old One 

   printf("int crtNewRelAsOld(char * newRelName=%s,char *oldRelName=%s )",newRelName,oldRelName);
    int oldRelNum=-1,newRelNum=-1,i;
    oldRelNum=OpenRel(oldRelName);
    int argc;
    char *argv[relCache[2*oldRelNum].numAttrs+3];//two for 0.create,1 newRelName, Last NULL,
    char type[relCache[oldRelNum].numAttrs+1][8];
    char typestr[8];
    argv[0]="create";
    argv[1]=newRelName;
    argv[2*relCache[oldRelNum].numAttrs+2]=NULL;

    if(oldRelNum>=MR_FIRST_USR_RLTN_IND)
    {
        i=0;
        while(i<relCache[oldRelNum].numAttrs)
        {
            argv[2*i+2]=relCache[oldRelNum].attrHead[i].attrName;
            //argv[2*i+3]=relCache[oldRelNum].attrHead[i].type;
            cnvrtTypeNumToStr(relCache[oldRelNum].attrHead[i].type,typestr,relCache[oldRelNum].attrHead[i].length);
            strcpy(type[i],typestr);
            argv[2*i+3]=type[i];
            i++;
        }
        Create(argc,argv);
    }
    

}
void cnvrtTypeNumToStr(int num,char *str,int len)
{  
    char slen[4];
    switch(num)
    {
        case DTCHAR:
            str[0]='c';
            str[1]='\0';
            str[2]='\0';
        break;

        case DTUNSIGNED_CHAR:
            str[0]='k';
            str[1]='\0';
            str[2]='\0';
        break;

        case DTSHORT:
            str[0]='l';
            str[1]='\0';
            str[2]='\0';

        break;

        case DTUNSIGNED_SHORT:
            str[0]='m';
            str[1]='\0';
            str[2]='\0';
     
        break;

        case DTINT:
            str[0]='i';
            str[1]='\0';
            str[2]='\0';

        break;

        case DTUNSIGNED_INT:
            str[0]='u';
            str[1]='\0';
            str[2]='\0';

        break;

        case DTSTRING:
           sprintf(slen,"%d",len);
            str[0]='s';
            str[1]='\0';
            strcat(str,slen);
        break;
        case DTFLOAT:

            str[0]='f';
            str[1]='\0';
            str[2]='\0';

        break;

    }
    
}
readRecInNewRel(int newRelNum,int oldRelNum,char attrName[],char value[],int compOp)
{
    char recPtr[relCache[oldRelNum].recLength];
    Rid startRid,foundRid;
    startRid.pid=0;
    startRid.slotnum=0;
    int recFound=0,count=0,attrOfst;
   
   // char attrValue[]


    //function return the offset of attribute in relCache[RelNUM].AttrHead[...offset ??..]
    attrOfst=offsetInAttrCache(oldRelNum,attrName);
    if(attrOfst>0)
    {
        unsigned short attrType;
        unsigned attrSz;
        //char attrValPtr[relCache[oldRelNum].recLength];
        attrType=relCache[oldRelNum].attrHead[attrOfst].type;
        attrSz=relCache[oldRelNum].attrHead[attrOfst].length;
        switch(relCache[relNum].attrHead[offset].type)
        {
            case DTCHAR:
                //
                char attrValPtr;
                attrValPtr=value[0];
                //sprintf(attrValPtr,"%c",value);
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,attrValPtr,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
           
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recfound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,attrValPtr,compOp);
                }   
                
            break;

            case DTUNSIGNED_CHAR:
                //
                char attrValPtr;
                attrValPtr=value[0];
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,attrValPtr,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
           
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recfound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,attrValPtr,compOp);
                }   

            break;

            case DTSHORT:
                //
                short attrValPtr;


            break;

            case DTUNSIGNED_SHORT:
                //      
            break;  

            case DTINT:
                //
            break;  

            case DTUNSIGNED_INT:
                //
            break;

            case DTSTRING:
                //
            break;
        
        
            case DTFLOAT:
                //
            break;
        }

        
        startRid.pid=0;
        startRid.slotnum=0;
        count=0;
        recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,attrValPtr,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
        while(recFound==1)
        {   
           
            count++;//found is true so count Rec
            InsertRec(newRelNum,(unsigned char*)recPtr);
            startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
            recfound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,attrValPtr,compOp);
        }   

    }
#endif
}
/*
 switch(relCache[relNum].attrHead[offset].type)
        {
            case DTCHAR:
                //
            break;

            case DTUNSIGNED_CHAR:
                //
            break;

            case DTSHORT:
                //
            break;

            case DTUNSIGNED_SHORT:
                //      
            break;  

            case DTINT:
                //
            break;  

            case DTUNSIGNED_INT:
                //
            break;

            case DTSTRING:
                //
            break;
        
        
            case DTFLOAT:
                //
            break;
        }
        
        */