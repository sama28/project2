#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>

void deleteRecFromRel(int relNum, char attrName[], char value[], int compOp);
int Delete(int argc, char **argv)
{
    /* print command line arguments */
    short k; /* iteration counter	    */
    printf("%s:\n", argv[0]);
    for (k = 1; k < argc; ++k)
        printf("\targv[%d] = %s\n", k, argv[k]);
    printf("Delete ..........fine.........\n");

    //---------------------------------------------------
    //DELETE CODE .............
    //---------------------------------------------------
    int status1,status;
    Rid startRid;
    int relNum;
    if (MR_CURR_DB[0] != 0)
    {
        char recPtr[relCache[0].recLength];
        printf("Select c++ ......fine....\n");
        //making sure that temporary relation->argv[1] doesnot exist and relation argv[2] exist
        status1=FindRelNum(argv[1]);
        if(status1 < 0 )
        {   
            startRid.pid=0;
            startRid.slotnum=0;
            status = findRecInRelcat(argv[1], recPtr, &startRid);
        }
         else
        {
            status=1;//relation already exists
            //printf("\n\nIN SELECT :- RELATION %s ALREADY EXISTS....",argv[1]);
        }
        //status == 1 if RELATION EXISTS
        if(status==1)
        {
            relNum=OpenRel(argv[1]);
            if(relNum > 1)//CATALOG SHOULD NOT BE TOUCHED....
            {
                //---delet record---
                deleteRecFromRel(relNum,argv[2],argv[4],*(int *)argv[3]);
            }
            else
            {
                printf("\n\nDELETE :- NOT ALLOWED....");
            }
        }
        else
        {
            printf("\n\nDELETE :- RELATION DOES NOT EXISTS....");
        }
    }
    else
    {
        printf("\n\nDELETE:- OPEN THE DATABASE FIRST....");
    }
    return (OK); /* all's fine */
}
void deleteRecFromRel(int relNum, char attrName[], char value[], int compOp)
{
    printf("\n\nInside deleteRec");
    char recPtr[relCache[relNum].recLength];
    Rid startRid, foundRid;
    startRid.pid = 0;
    startRid.slotnum = 0;
    int recFound = 0, count = 0, attrOfst;
    float fvalue;
    int ivalue;
    // char attrValue[] 
    //function return the offset of attribute in relCache[RelNUM].AttrHead[...offset ??..]
    attrOfst = offsetInAttrCache(relNum, attrName);
    if (attrOfst >= 0)
    {
        unsigned short attrType;
        unsigned attrSz;
        //char attrValPtr[relCache[relNum].recLength];
        attrType = relCache[relNum].attrHead[attrOfst].type;
        attrSz = relCache[relNum].attrHead[attrOfst].length;

        switch (attrType)
        {
        case DTINT:

            ivalue = atoi(value);
            startRid.pid = 0;
            startRid.slotnum = 0;
            count = 0;
            recFound = FindRec(relNum, &startRid, &foundRid, recPtr, attrType, attrSz, attrOfst, (char *)&ivalue, compOp); //GetNextRec(relNum,&startRid,&foundRid,recPtr);
            while (recFound == 1)
            {
                count++; //found is true so count 
                DeleteRec(relNum,&foundRid);
                startRid.pid = foundRid.pid;
                startRid.slotnum = foundRid.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
                //printf("\n\ndeletRec ->findRec startRid.slotnum ->%u startRid.pid:- %u ", startRid.slotnum, foundRid.pid);
                recFound = FindRec(relNum, &startRid, &foundRid, recPtr, attrType, attrSz, attrOfst, (char *)&ivalue, compOp);
            }

            break;

        case DTSTRING:
            startRid.pid = 0;
            startRid.slotnum = 0;
            count = 0;
            recFound = FindRec(relNum, &startRid, &foundRid, recPtr, attrType, attrSz, attrOfst, value, compOp); //GetNextRec(relNum,&startRid,&foundRid,recPtr);
            while (recFound == 1)
            {
                count++; //found is true so count Rec
                DeleteRec(relNum,&foundRid);
                startRid.pid = foundRid.pid;
                startRid.slotnum = foundRid.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
                recFound = FindRec(relNum, &startRid, &foundRid, recPtr, attrType, attrSz, attrOfst, value, compOp);
            }

            break;

        case DTFLOAT:

            fvalue = atof(value);
            startRid.pid = 0;
            startRid.slotnum = 0;
            count = 0;
            recFound = FindRec(relNum, &startRid, &foundRid, recPtr, attrType, attrSz, attrOfst, (char *)&fvalue, compOp); //GetNextRec(relNum,&startRid,&foundRid,recPtr);
            while (recFound == 1)
            {
                count++; //found is true so count Rec
                DeleteRec(relNum,&foundRid);
                startRid.pid = foundRid.pid;
                startRid.slotnum = foundRid.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
                recFound = FindRec(relNum, &startRid, &foundRid, recPtr, attrType, attrSz, attrOfst, (char *)&fvalue, compOp);
            }

            break;
        }
    }
}