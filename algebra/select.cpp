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
int crtNewRelAsOld(char * newRelName,char *oldRelName );
void cnvrtTypeNumToStr(int num,char *str,int len);
void readRecInNewRel(int newRelNum,int oldRelNum,char attrName[],char value[],int compOp);
int offsetInAttrCache(int relNum,char * attrName);
int Select (int argc, char ** argv)
{
    /* print command line arguments */
    short	k;		/* iteration counter	    */
    printf ("%s:\n", argv[0]);
    for (k = 1 ; k < argc; ++k)
	printf ("\targv[%d] = %s\n", k, argv[k]);

     printf("Select c++ ......fine....\n");


    int newRelNum,oldRelNum;
    Rid startRid,foundRid;
    char recPtr[relCache[newRelNum].recLength];
    int oprtr;

     //1.0Create New Relation
       crtNewRelAsOld(argv[1],argv[2]);

    //2.OpenNew Relation
        newRelNum=OpenRel(argv[1]);
        oldRelNum=OpenRel(argv[2]);

    //3.Read Record In New Relation That Satiesfies The Condition
       if(newRelNum >1 && oldRelNum > 1)
       {
            oprtr=atoi(argv[4]);
            readRecInNewRel(newRelNum,oldRelNum,argv[3],argv[5],oprtr);
            return (OK);  /* all's fine */
       }
       else
       {
           printf("%s CAN NOT BE CREATED",argv[1]);
       }
     
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
        argc=relCache[oldRelNum].numAttrs*2+2;
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
void readRecInNewRel(int newRelNum,int oldRelNum,char attrName[],char value[],int compOp)
{
    char recPtr[relCache[oldRelNum].recLength];
    Rid startRid,foundRid;
    startRid.pid=0;
    startRid.slotnum=0;
    int recFound=0,count=0,attrOfst;
    float fvalue;
     int ivalue=atoi(value);
   // char attrValue[]
    //function return the offset of attribute in relCache[RelNUM].AttrHead[...offset ??..]
    attrOfst=offsetInAttrCache(oldRelNum,attrName);
    if(attrOfst>=0)
    {
        unsigned short attrType;
        unsigned attrSz;
        //char attrValPtr[relCache[oldRelNum].recLength];
        attrType=relCache[oldRelNum].attrHead[attrOfst].type;
        attrSz=relCache[oldRelNum].attrHead[attrOfst].length;
        
        switch(attrType)
        {            
            case DTINT:
               
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)&ivalue,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)&ivalue,compOp);
                }  
                
            break;  

            case DTSTRING:
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,value,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,value,compOp);
                }
                
            break;
            
            case DTFLOAT:

                fvalue=atof(value);
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)&fvalue,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)&fvalue,compOp);
                }
                
            break;
        }        
    }
}
int offsetInAttrCache(int relNum,char * attrName)
{
    for(int i=0;i<relCache[relNum].numAttrs;i++)
    {
        if(!strcmp(attrName,relCache[relNum].attrHead[i].attrName))
        {
            return i;
        }
    }

    return -1;
}

/*
//switch case code...
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
#if 0
switch(attrType)
        {
            case DTCHAR:
                /*startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)value,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                 recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)value,compOp);
                }   
                */
                printf("DTCHAR");
            break;
            case DTUNSIGNED_CHAR:
                /*
                unsigned char ucvalue;
                ucvalue=(unsigned char*)vlaue[0];
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char *)ucvalue,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
           
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char *)ucvalue,compOp);
                } */  
            break;
            case DTSHORT:
                /*short svalue;
                svalue=(short )atoi(value); 
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char *)svalue,compOp);
                while(recFound==1)
                {   
           
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char *)svalue,compOp);
                } 
                */  
            break;
            
            case DTUNSIGNED_SHORT:
                /*
                unsigned short usvalue;
                usvalue=(unsigned short)atoi(value); 
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char *)usvalue,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
           
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char *)usvalue,compOp);
                }*/ 
            break;  
            
            case DTINT:
                int ivalue=atoi(value);
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)ivalue,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                   recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)ivalue,compOp);
                }  
            break;  

            case DTUNSIGNED_INT:
                
                /*unsigned  uivalue;
                uivalue=(unsigned)atoi(value);
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)uivalue,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                   recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)uivalue,compOp);
                }  
                */

            break;
            
            case DTSTRING:
                
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,value,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,value,compOp);
                }

            break;
            
            case DTFLOAT:

                float fvalue;
                fvalue=atof(value);
                startRid.pid=0;
                startRid.slotnum=0;
                count=0;
                recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)fvalue,compOp);//GetNextRec(oldRelNum,&startRid,&foundRid,recPtr);
                while(recFound==1)
                {   
                    count++;//found is true so count Rec
                    InsertRec(newRelNum,(unsigned char*)recPtr);
                    startRid.slotnum=foundRid.slotnum+1;//Find Rec WIll Not Give Error If slotNum >recPerPg 
                    recFound=FindRec(oldRelNum,&startRid,&foundRid,recPtr,attrType,attrSz,attrOfst,(char*)fvalue,compOp);
                }

            break;
        }        
#endif