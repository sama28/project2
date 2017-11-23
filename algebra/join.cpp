#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
#include <string.h>
int argCheck3(int relNum[], int argc, char **argv);
void intJoin1(int rsltRelNum, int relNum1, int relNum2, int attrOfst1, int attrOfst2, int compOp);
void joinThem(int rsltRelNum, int relNum1, int relNum2, int attrOfst1, int attrOfst2, int compOp);
void intJoin2(int rsltRelNum, int relNum2, int relNum1, int attrOfst1, int attrOfst2, int compOp);
void strJoin1(int rsltRelNum, int relNum2, int relNum1, int attrOfst1, int attrOfst2, int compOp);
void strJoin2(int rsltRelNum, int relNum2, int relNum1, int attrOfst1, int attrOfst2, int compOp);
void floatJoin1(int rsltRelNum, int relNum2, int relNum1, int attrOfst1, int attrOfst2, int compOp);
void floatJoin2(int rsltRelNum, int relNum2, int relNum1, int attrOfst1, int attrOfst2, int compOp); 
int Join(int argc, char **argv)
{

    /* print command line arguments */
    short k; /* iteration counter	    */
    printf("%s:\n", argv[0]);
    for (k = 1; k < argc; ++k)
        printf("\targv[%d] = %s\n", k, argv[k]);

    printf("Join \n");
    //return (OK); /* all's fine */
    //argCheck3(srcRel,argc,argv,&kl);
    int rsltRel, srcRel[2], ofst,status;
    int attrOfst1, attrOfst2;
    Rid startRid, foundRid,fRidtmp;
    char recPtrtmp[PAGESIZE];
    if (MR_CURR_DB[0] != 0)
    {

        rsltRel = FindRelNum(argv[1]);
        if(rsltRel<0)
        {
            status=findRecInRelcat(argv[1],recPtrtmp,&fRidtmp);
        }
        else
        {
            status=1;
        }
        if (status !=1)
        {
            srcRel[0] = OpenRel(argv[2]);
            srcRel[1] = OpenRel(argv[4]);
            if (srcRel[0] > 1 && srcRel[1] > 1)
            {
                //1.create rsltRel
                argCheck3(srcRel, argc, argv);
                //2open rslt Relation All Relation
                rsltRel = OpenRel(argv[1]);
                srcRel[0] = OpenRel(argv[2]);
                srcRel[1] = OpenRel(argv[4]);
                if (rsltRel > 1 && srcRel[0] > 1 && srcRel[1] > 1)
                {
                    attrOfst1 = offsetInAttrCache(srcRel[0], argv[3]);
                    attrOfst2 = offsetInAttrCache(srcRel[1], argv[5]);
                    if (attrOfst1 >= 0 && attrOfst2 >= 0)
                    {
                        joinThem(rsltRel, srcRel[0], srcRel[1], attrOfst1, attrOfst2, EQ);
                    }
                }
            }
            else
            {
                printf("\n\nIN JOIN :- %s OR %s RELATION DOES NOT EXISTs......", argv[2], argv[4]);
            }
        }
        else
        {
            printf("\n\nIN JOIN :- %s RELATION ALREADY EXIST......", argv[1]);
        }
    }
    else
    {
        printf("\n\nIN JOIN :- OPEN DATABASE FIRST...");
    }
}
void joinThem(int rsltRelNum, int relNum1, int relNum2, int attrOfst1, int attrOfst2, int compOp)
{
    printf("\n\nIN JOINTHEM....");
    //unsigned short attrType1;
    //unsigned attrSz1;
    //char attrValPtr[relCache[relNum2].recLength];
    // attrType1 = relCache[relNum1].attrHead[attrOfst1].type;
    //ttrSz1 = relCache[relNum1].attrHead[attrOfst1].length;

    //intJoin2(intrsltRelNum, relNum1, relNum2, attrOfst1, attrOfst2, compOp);

    if (relCache[relNum1].numRecs <= relCache[relNum2].numRecs)
    {
        switch (relCache[relNum1].attrHead[attrOfst1].type)
        {
        case DTINT:

            intJoin1(rsltRelNum, relNum1, relNum2, attrOfst1, attrOfst2, compOp);
            break;

        case DTSTRING:
            strJoin1(rsltRelNum, relNum1, relNum2, attrOfst1, attrOfst2, compOp);
            break;

        case DTFLOAT:
            floatJoin1(rsltRelNum, relNum1, relNum2, attrOfst1, attrOfst2, compOp);
            break;
        }
    }
    else
    {
        switch (relCache[relNum1].attrHead[attrOfst1].type)
        {
        case DTINT:

            intJoin2(rsltRelNum, relNum1, relNum2, attrOfst1, attrOfst2, compOp);
            break;

        case DTSTRING:
            strJoin2(rsltRelNum, relNum1, relNum2, attrOfst1, attrOfst2, compOp);
            break;

        case DTFLOAT:
            floatJoin2(rsltRelNum, relNum1, relNum2, attrOfst1, attrOfst2, compOp);
            break;
        }
    }
}
void intJoin1(int rsltRelNum, int relNum1, int relNum2, int attrOfst1, int attrOfst2, int compOp)
{
    //intRel1(1st srcRelation in Argument) As inner One And relNum2 As outerOne
    //insrt Record in result Relation As|=>    shecma(|relNum1 attrs|...|common attrs|...|relNum2 attrs|);
    printf("\n\nIN INTJOIN1 :- starts....");
    int ofstInRec1, ofstInRec2;
    Rid startRid1, foundRid1;
    Rid startRid2, foundRid2;
    int recFound1, recFound2;
    int attrType1, attrType2, attrSz1, attrSz2;
    int ivalue, count;
    int part1;
    char rsltRecPtr[relCache[rsltRelNum].recLength];
    char recPtr1[relCache[relNum1].recLength];
    char recPtr2[relCache[relNum2].recLength];
    int count1, count2;

    ofstInRec1 = calcOffset(relNum1, attrOfst1);
    ofstInRec2 = calcOffset(relNum2, attrOfst2);

    printf("\n\nIN INTJOIN1 :- offstInRec1 %d", ofstInRec1);
    printf("\n\nIN INTJOIN1 :- offstInRec2 %d", ofstInRec2);

    attrType1 = relCache[relNum1].attrHead[attrOfst1].type;
    attrSz1 = relCache[relNum1].attrHead[attrOfst1].length;

    attrType2 = relCache[relNum2].attrHead[attrOfst2].type;
    attrSz2 = relCache[relNum2].attrHead[attrOfst2].length;

    //NESTED LOOP JOIN IMPLEMENTATION..................................
    startRid1.pid = 0;
    startRid1.slotnum = 0;
    count1 = 0;
    recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    while (recFound1 == 1)
    {
        count1++;
        printf("\n\nIN INTLOOP :- outerLoop Count =%d", count1);
        printf("\n\nIN INTLOOP :- outerLoop foundRid.pid=%u ,fountRid.slotnum=%u", foundRid1.pid, foundRid1.slotnum);
        ivalue = *(int *)(recPtr1 + ofstInRec1); //atoi(value);
        startRid2.pid = 0;
        startRid2.slotnum = 0;
        count2 = 0;
        recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&ivalue, compOp);
        while (recFound2 == 1)
        {
            count2++; //found is true so count Rec
            printf("\n\nIN INTLOOP :- innerLoop Count =%d", count2);
            printf("\n\nIN INTLOOP :- innerLoop foundRid.pid=u ,fountRid.slotnum=u", foundRid2.pid, foundRid2.slotnum);
            memcpy(rsltRecPtr, recPtr1, relCache[relNum1].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum1, rsltRecPtr);
            memcpy((rsltRecPtr + relCache[relNum1].recLength), recPtr2, relCache[relNum2].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            part1 = ofstInRec2 + relCache[relNum2].attrHead[attrOfst2].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d", part1, (relCache[relNum1].recLength + part1), part1, relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum1].recLength + ofstInRec2), (recPtr2 + part1), relCache[relNum2].recLength - part1);
            printf("\n\nIN INTLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            InsertRec(rsltRelNum, (unsigned char *)rsltRecPtr);
            startRid2.pid = foundRid2.pid;
            startRid2.slotnum = foundRid2.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
            printf("\n\nIn INTLOOP ->findRec startRid2.slotnum ->%u startRid2.pid:- %u ", startRid2.slotnum, foundRid2.pid);
            recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&ivalue, compOp);
        }
        startRid1.pid = foundRid1.pid;
        startRid1.slotnum = foundRid1.slotnum + 1;
        recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    }
}
void intJoin2(int rsltRelNum, int relNum2, int relNum1, int attrOfst2, int attrOfst1, int compOp)
{
    //just Like intJoin1 but treats intRel2(1st srcRelation in Argument) As inner One And relNum1 As outerOne
    //insrt Record in result Relation As|=>    shecma(|relNum1 attrs|...|common attrs|...|relNum2 attrs|);
    printf("\n\nIN INTJOIN2 :- starts....");
    int ofstInRec1, ofstInRec2;
    Rid startRid1, foundRid1;
    Rid startRid2, foundRid2;
    int recFound1, recFound2;
    int attrType1, attrType2, attrSz1, attrSz2;
    int ivalue, count;
    int part1;
    char rsltRecPtr[relCache[rsltRelNum].recLength];
    char recPtr1[relCache[relNum1].recLength];
    char recPtr2[relCache[relNum2].recLength];
    int count1, count2;

    ofstInRec1 = calcOffset(relNum1, attrOfst1);
    ofstInRec2 = calcOffset(relNum2, attrOfst2);

    printf("\n\nIN INTJOIN2 :- offstInRec1 %d", ofstInRec1);
    printf("\n\nIN INTJOIN2 :- offstInRec2 %d", ofstInRec2);

    attrType1 = relCache[relNum1].attrHead[attrOfst1].type;
    attrSz1 = relCache[relNum1].attrHead[attrOfst1].length;

    attrType2 = relCache[relNum2].attrHead[attrOfst2].type;
    attrSz2 = relCache[relNum2].attrHead[attrOfst2].length;

    //NESTED LOOP JOIN IMPLEMENTATION..................................
    startRid1.pid = 0;
    startRid1.slotnum = 0;
    count1 = 0;
    recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    while (recFound1 == 1)
    {
        count1++;
        printf("\n\nIN INTLOOP :- outerLoop Count =%d", count1);
        printf("\n\nIN INTLOOP :- outerLoop foundRid.pid=%u ,fountRid.slotnum=%u", foundRid1.pid, foundRid1.slotnum);
        ivalue = *(int *)(recPtr1 + ofstInRec1); //atoi(value);
        startRid2.pid = 0;
        startRid2.slotnum = 0;
        count2 = 0;
        recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&ivalue, compOp);
        while (recFound2 == 1)
        {
            count2++; //found is true so count Rec
            printf("\n\nIN INTLOOP :- innerLoop Count =%d", count2);
            printf("\n\nIN INTLOOP :- innerLoop foundRid.pid=u ,fountRid.slotnum=u", foundRid2.pid, foundRid2.slotnum);
            /*memcpy(rsltRecPtr, recPtr1,relCache[relNum1].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum1, rsltRecPtr);
            memcpy((rsltRecPtr+relCache[relNum1].recLength), recPtr2,relCache[relNum2].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum,rsltRecPtr);
            part1 = ofstInRec2 + relCache[relNum2].attrHead[attrOfst2].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d",part1,(relCache[relNum1].recLength + part1), part1,relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum1].recLength + ofstInRec2), (recPtr2 + part1), relCache[relNum2].recLength - part1);
            printf("\n\nIN INTLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            */ //..............................................
            //Just Invert The Copy Logic From Int Join One..
            memcpy(rsltRecPtr, recPtr2, relCache[relNum2].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum2, rsltRecPtr);
            memcpy((rsltRecPtr + relCache[relNum2].recLength), recPtr1, relCache[relNum1].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            part1 = ofstInRec1 + relCache[relNum1].attrHead[attrOfst1].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d", part1, (relCache[relNum1].recLength + part1), part1, relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum2].recLength + ofstInRec1), (recPtr1 + part1), relCache[relNum1].recLength - part1);
            printf("\n\nIN INTLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            //................................................
            InsertRec(rsltRelNum, (unsigned char *)rsltRecPtr);
            //.................................................
            startRid2.pid = foundRid2.pid;
            startRid2.slotnum = foundRid2.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
            printf("\n\nIn  ->findRec startRid2.slotnum ->%u startRid2.pid:- %u ", startRid2.slotnum, foundRid2.pid);
            recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&ivalue, compOp);
        }
        startRid1.pid = foundRid1.pid;
        startRid1.slotnum = foundRid1.slotnum + 1;
        recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    }
}
//-----------------------------------------------------------------------------------------------
void strJoin1(int rsltRelNum, int relNum1, int relNum2, int attrOfst1, int attrOfst2, int compOp)
{
    printf("\n\nSTRJOIN1 :- ENTERING........");
    //intRel1(1st srcRelation in Argument) As inner One And relNum2 As outerOne
    //insrt Record in result Relation As|=>    shecma(|relNum1 attrs|...|common attrs|...|relNum2 attrs|);
    printf("\n\nIN STRJOIN1 :- starts....");
    int ofstInRec1, ofstInRec2;
    Rid startRid1, foundRid1;
    Rid startRid2, foundRid2;
    int recFound1, recFound2;
    int attrType1, attrType2, attrSz1, attrSz2;
    int count, part1;

    char rsltRecPtr[relCache[rsltRelNum].recLength];
    char recPtr1[relCache[relNum1].recLength];
    char recPtr2[relCache[relNum2].recLength];
    int count1, count2;

    ofstInRec1 = calcOffset(relNum1, attrOfst1);
    ofstInRec2 = calcOffset(relNum2, attrOfst2);

    printf("\n\nIN STRJOIN1 :- offstInRec1 %d", ofstInRec1);
    printf("\n\nIN STRJOIN1 :- offstInRec2 %d", ofstInRec2);

    attrType1 = relCache[relNum1].attrHead[attrOfst1].type;
    attrSz1 = relCache[relNum1].attrHead[attrOfst1].length;
    char cvalue[attrSz1]; //This Line Is Position Dependent......

    attrType2 = relCache[relNum2].attrHead[attrOfst2].type;
    attrSz2 = relCache[relNum2].attrHead[attrOfst2].length;

    //NESTED LOOP JOIN IMPLEMENTATION..................................
    startRid1.pid = 0;
    startRid1.slotnum = 0;
    count1 = 0;
    recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    while (recFound1 == 1)
    {
        count1++;
        printf("\n\nIN STRLOOP :- outerLoop Count =%d", count1);
        printf("\n\nIN STRLOOP :- outerLoop foundRid.pid=%u ,fountRid.slotnum=%u", foundRid1.pid, foundRid1.slotnum);
        //ivalue = *(int *)(recPtr1 + ofstInRec1); //atoi(value);
        memcpy(cvalue, (recPtr1 + ofstInRec1), attrSz1);
        startRid2.pid = 0;
        startRid2.slotnum = 0;
        count2 = 0;
        recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz1, attrOfst2, cvalue, compOp);
        while (recFound2 == 1)
        {
            count2++; //found is true so count Rec
            printf("\n\nIN STRLOOP :- innerLoop Count =%d", count2);
            printf("\n\nIN STRLOOP :- innerLoop foundRid.pid=u ,fountRid.slotnum=u", foundRid2.pid, foundRid2.slotnum);
            memcpy(rsltRecPtr, recPtr1, relCache[relNum1].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum1, rsltRecPtr);
            memcpy((rsltRecPtr + relCache[relNum1].recLength), recPtr2, relCache[relNum2].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            part1 = ofstInRec2 + attrSz2; //relCache[relNum2].attrHead[attrOfst2].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d", part1, (relCache[relNum1].recLength + part1), part1, relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum1].recLength + ofstInRec2), (recPtr2 + part1), relCache[relNum2].recLength - part1);
            printf("\n\nIN STRLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            InsertRec(rsltRelNum, (unsigned char *)rsltRecPtr);
            startRid2.pid = foundRid2.pid;
            startRid2.slotnum = foundRid2.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
            printf("\n\nIn STRLOOP ->findRec startRid2.slotnum ->%u startRid2.pid:- %u ", startRid2.slotnum, foundRid2.pid);
            recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz1, attrOfst2, cvalue, compOp);
        }
        startRid1.pid = foundRid1.pid;
        startRid1.slotnum = foundRid1.slotnum + 1;
        recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    }
    printf("\n\nSTRJOIN :- EXIT........");
}
//-----------------------------------------------------------------------------------------------
void strJoin2(int rsltRelNum, int relNum2, int relNum1, int attrOfst2, int attrOfst1, int compOp)
{
    printf("\n\nSTRJOIN2 :- ENTERING........");
    //just Like intJoin1 but treats intRel2(1st srcRelation in Argument) As inner One And relNum1 As outerOne
    //insrt Record in result Relation As|=>    shecma(|relNum1 attrs|...|common attrs|...|relNum2 attrs|);
    printf("\n\nIN STRJOIN2 :- starts....");
    int ofstInRec1, ofstInRec2;
    Rid startRid1, foundRid1;
    Rid startRid2, foundRid2;
    int recFound1, recFound2;
    int attrType1, attrType2, attrSz1, attrSz2;
    int count;
    int part1;
    char rsltRecPtr[relCache[rsltRelNum].recLength];
    char recPtr1[relCache[relNum1].recLength];
    char recPtr2[relCache[relNum2].recLength];
    int count1, count2;

    ofstInRec1 = calcOffset(relNum1, attrOfst1);
    ofstInRec2 = calcOffset(relNum2, attrOfst2);

    printf("\n\nIN STRJOIN2 :- offstInRec1 %d", ofstInRec1);
    printf("\n\nIN STRJOIN2 :- offstInRec2 %d", ofstInRec2);

    attrType1 = relCache[relNum1].attrHead[attrOfst1].type;
    attrSz1 = relCache[relNum1].attrHead[attrOfst1].length;

    attrType2 = relCache[relNum2].attrHead[attrOfst2].type;
    attrSz2 = relCache[relNum2].attrHead[attrOfst2].length;

    char cvalue[attrSz1];
    //NESTED LOOP JOIN IMPLEMENTATION..................................
    startRid1.pid = 0;
    startRid1.slotnum = 0;
    count1 = 0;
    recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    while (recFound1 == 1)
    {
        count1++;
        printf("\n\nIN STRLOOP :- outerLoop Count =%d", count1);
        printf("\n\nIN STRLOOP :- outerLoop foundRid.pid=%u ,fountRid.slotnum=%u", foundRid1.pid, foundRid1.slotnum);
        //ivalue = *(int *)(recPtr1 + ofstInRec1); //atoi(value);
        memcpy(cvalue, recPtr1 + ofstInRec1, attrSz1);
        startRid2.pid = 0;
        startRid2.slotnum = 0;
        count2 = 0;
        recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType2, attrSz2, attrOfst2, cvalue, compOp);
        while (recFound2 == 1)
        {
            count2++; //found is true so count Rec
            printf("\n\nIN STRLOOP :- innerLoop Count =%d", count2);
            printf("\n\nIN STRLOOP :- innerLoop foundRid.pid=u ,fountRid.slotnum=u", foundRid2.pid, foundRid2.slotnum);
            /*memcpy(rsltRecPtr, recPtr1,relCache[relNum1].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum1, rsltRecPtr);
            memcpy((rsltRecPtr+relCache[relNum1].recLength), recPtr2,relCache[relNum2].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum,rsltRecPtr);
            part1 = ofstInRec2 + relCache[relNum2].attrHead[attrOfst2].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d",part1,(relCache[relNum1].recLength + part1), part1,relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum1].recLength + ofstInRec2), (recPtr2 + part1), relCache[relNum2].recLength - part1);
            printf("\n\nIN STRLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            */ //..............................................
            //Just Invert The Copy Logic From Int Join One..
            memcpy(rsltRecPtr, recPtr2, relCache[relNum2].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum2, rsltRecPtr);
            memcpy((rsltRecPtr + relCache[relNum2].recLength), recPtr1, relCache[relNum1].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            part1 = ofstInRec1 + relCache[relNum1].attrHead[attrOfst1].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d", part1, (relCache[relNum1].recLength + part1), part1, relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum2].recLength + ofstInRec1), (recPtr1 + part1), relCache[relNum1].recLength - part1);
            printf("\n\nIN STRLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            //................................................
            InsertRec(rsltRelNum, (unsigned char *)rsltRecPtr);
            //.................................................
            startRid2.pid = foundRid2.pid;
            startRid2.slotnum = foundRid2.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
            printf("\n\nIn STRLOOP ->findRec startRid2.slotnum ->%u startRid2.pid:- %u ", startRid2.slotnum, foundRid2.pid);
            recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType2, attrSz2, attrOfst2, cvalue, compOp);
        }
        startRid1.pid = foundRid1.pid;
        startRid1.slotnum = foundRid1.slotnum + 1;
        recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    }
    printf("\n\nSTR JOIN1 :- EXIT...");
}
void floatJoin1(int rsltRelNum, int relNum1, int relNum2, int attrOfst1, int attrOfst2, int compOp)
{
    //intRel1(1st srcRelation in Argument) As inner One And relNum2 As outerOne
    //insrt Record in result Relation As|=>    shecma(|relNum1 attrs|...|common attrs|...|relNum2 attrs|);
    printf("\n\nIN FLOATJOIN1 :- starts....");
    int ofstInRec1, ofstInRec2;
    Rid startRid1, foundRid1;
    Rid startRid2, foundRid2;
    int recFound1, recFound2;
    int attrType1, attrType2, attrSz1, attrSz2;
    int count;
    float fvalue;
    int part1;
    char rsltRecPtr[relCache[rsltRelNum].recLength];
    char recPtr1[relCache[relNum1].recLength];
    char recPtr2[relCache[relNum2].recLength];
    int count1, count2;

    ofstInRec1 = calcOffset(relNum1, attrOfst1);
    ofstInRec2 = calcOffset(relNum2, attrOfst2);

    printf("\n\nIN FLOATJOIN1 :- offstInRec1 %d", ofstInRec1);
    printf("\n\nIN FLOATJOIN1 :- offstInRec2 %d", ofstInRec2);

    attrType1 = relCache[relNum1].attrHead[attrOfst1].type;
    attrSz1 = relCache[relNum1].attrHead[attrOfst1].length;

    attrType2 = relCache[relNum2].attrHead[attrOfst2].type;
    attrSz2 = relCache[relNum2].attrHead[attrOfst2].length;

    //NESTED LOOP JOIN IMPLEMENTATION..................................
    startRid1.pid = 0;
    startRid1.slotnum = 0;
    count1 = 0;
    recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    while (recFound1 == 1)
    {
        count1++;
        printf("\n\nIN FLOATLOOP :- outerLoop Count =%d", count1);
        printf("\n\nIN FLOATLOOP :- outerLoop foundRid.pid=%u ,fountRid.slotnum=%u", foundRid1.pid, foundRid1.slotnum);
        //ivalue = *(int *)(recPtr1 + ofstInRec1); //atoi(value);
        fvalue = *(float *)(recPtr1 + ofstInRec1);
        startRid2.pid = 0;
        startRid2.slotnum = 0;
        count2 = 0;
        recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&fvalue, compOp);
        while (recFound2 == 1)
        {
            count2++; //found is true so count Rec
            printf("\n\nIN FLOATLOOP :- innerLoop Count =%d", count2);
            printf("\n\nIN FLOATLOOP :- innerLoop foundRid.pid=u ,fountRid.slotnum=u", foundRid2.pid, foundRid2.slotnum);
            memcpy(rsltRecPtr, recPtr1, relCache[relNum1].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum1, rsltRecPtr);
            memcpy((rsltRecPtr + relCache[relNum1].recLength), recPtr2, relCache[relNum2].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            part1 = ofstInRec2 + relCache[relNum2].attrHead[attrOfst2].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d", part1, (relCache[relNum1].recLength + part1), part1, relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum1].recLength + ofstInRec2), (recPtr2 + part1), relCache[relNum2].recLength - part1);
            printf("\n\nIN FLOATLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            InsertRec(rsltRelNum, (unsigned char *)rsltRecPtr);
            startRid2.pid = foundRid2.pid;
            startRid2.slotnum = foundRid2.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
            printf("\n\nIn FLOATLOOP ->findRec startRid2.slotnum ->%u startRid2.pid:- %u ", startRid2.slotnum, foundRid2.pid);
            recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&fvalue, compOp);
        }
        startRid1.pid = foundRid1.pid;
        startRid1.slotnum = foundRid1.slotnum + 1;
        recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    }
}
void floatJoin2(int rsltRelNum, int relNum2, int relNum1, int attrOfst2, int attrOfst1, int compOp)
{
    //just Like intJoin1 but treats intRel2(1st srcRelation in Argument) As inner One And relNum1 As outerOne
    //insrt Record in result Relation As|=>    shecma(|relNum1 attrs|...|common attrs|...|relNum2 attrs|);
    printf("\n\nIN FLOATJOIN2 :- starts....");
    int ofstInRec1, ofstInRec2;
    Rid startRid1, foundRid1;
    Rid startRid2, foundRid2;
    int recFound1, recFound2;
    int attrType1, attrType2, attrSz1, attrSz2;
    int count;
    float fvalue;
    int part1;
    char rsltRecPtr[relCache[rsltRelNum].recLength];
    char recPtr1[relCache[relNum1].recLength];
    char recPtr2[relCache[relNum2].recLength];
    int count1, count2;

    ofstInRec1 = calcOffset(relNum1, attrOfst1);
    ofstInRec2 = calcOffset(relNum2, attrOfst2);

    printf("\n\nIN FLOATJOIN2 :- offstInRec1 %d", ofstInRec1);
    printf("\n\nIN FLOATJOIN2 :- offstInRec2 %d", ofstInRec2);

    attrType1 = relCache[relNum1].attrHead[attrOfst1].type;
    attrSz1 = relCache[relNum1].attrHead[attrOfst1].length;

    attrType2 = relCache[relNum2].attrHead[attrOfst2].type;
    attrSz2 = relCache[relNum2].attrHead[attrOfst2].length;

    //NESTED LOOP JOIN IMPLEMENTATION..................................
    startRid1.pid = 0;
    startRid1.slotnum = 0;
    count1 = 0;
    recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    while (recFound1 == 1)
    {
        count1++;
        printf("\n\nIN FLOATLOOP :- outerLoop Count =%d", count1);
        printf("\n\nIN FLOATLOOP :- outerLoop foundRid.pid=%u ,fountRid.slotnum=%u", foundRid1.pid, foundRid1.slotnum);
        //ivalue = *(int *)(recPtr1 + ofstInRec1); //atoi(value);
        fvalue = *(float *)(recPtr1 + ofstInRec1);
        startRid2.pid = 0;
        startRid2.slotnum = 0;
        count2 = 0;
        recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&fvalue, compOp);
        while (recFound2 == 1)
        {
            count2++; //found is true so count Rec
            printf("\n\nIN FLOATLOOP :- innerLoop Count =%d", count2);
            printf("\n\nIN FLOATLOOP :- innerLoop foundRid.pid=u ,fountRid.slotnum=u", foundRid2.pid, foundRid2.slotnum);
            /*memcpy(rsltRecPtr, recPtr1,relCache[relNum1].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum1, rsltRecPtr);
            memcpy((rsltRecPtr+relCache[relNum1].recLength), recPtr2,relCache[relNum2].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum,rsltRecPtr);
            part1 = ofstInRec2 + relCache[relNum2].attrHead[attrOfst2].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d",part1,(relCache[relNum1].recLength + part1), part1,relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum1].recLength + ofstInRec2), (recPtr2 + part1), relCache[relNum2].recLength - part1);
            printf("\n\nIN FLOATLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            */ //..............................................
            //Just Invert The Copy Logic From float Join One..
            memcpy(rsltRecPtr, recPtr2, relCache[relNum2].recLength);
            printf("\nafter 1st Copy->parse\n");
            parseRecord(relNum2, rsltRecPtr);
            memcpy((rsltRecPtr + relCache[relNum2].recLength), recPtr1, relCache[relNum1].recLength);
            printf("\nafter strCAT parse..\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            part1 = ofstInRec1 + relCache[relNum1].attrHead[attrOfst1].length;
            printf("\n\nPART1 :-%d ,(overLapingStarts from in rsltRec)%d  ,(overLapped rec2 from)%d,overLapped PART SIZE %d", part1, (relCache[relNum1].recLength + part1), part1, relCache[relNum2].recLength - part1);
            memcpy((rsltRecPtr + relCache[relNum2].recLength + ofstInRec1), (recPtr1 + part1), relCache[relNum1].recLength - part1);
            printf("\n\nIN FLOATLOOP:-Calling Parse record....\n\n");
            parseRecord(rsltRelNum, rsltRecPtr);
            //................................................
            InsertRec(rsltRelNum, (unsigned char *)rsltRecPtr);
            //.................................................
            startRid2.pid = foundRid2.pid;
            startRid2.slotnum = foundRid2.slotnum + 1; //Find Rec WIll Not Give Error If slotNum >recPerPg
            printf("\n\nIn FLOATLOOP ->findRec startRid2.slotnum ->%u startRid2.pid:- %u ", startRid2.slotnum, foundRid2.pid);
            recFound2 = FindRec(relNum2, &startRid2, &foundRid2, recPtr2, attrType1, attrSz2, attrOfst2, (char *)&fvalue, compOp);
        }
        startRid1.pid = foundRid1.pid;
        startRid1.slotnum = foundRid1.slotnum + 1;
        recFound1 = GetNextRec(relNum1, &startRid1, &foundRid1, recPtr1);
    }
}

/*
makeRecord(char *rsltRecPtr, char *recPtr1, char *recPtr2, int offset, int sz1)
{
    strcat(rsltRecPtr, recPtr1);
    strcat(rsltRecPtr + recptr2);
    memcpy((rsltRecPtr + offset + sz1 + attrsz2), (recptr2 + offset + attrsz2), sz2 - offset - attrsz2);
}*/
int argCheck3(int relNum[], int argc, char **argv)
{
    int valid = 1, flag = 0, attrOffset[2];

    for (int i = 0; i < 2; i++)
    {
        flag = 0;
        for (int j = 0; j < relCache[relNum[i]].numAttrs && flag == 0; j++)
        {
            if (strcmp(relCache[relNum[i]].attrHead[j].attrName, argv[3 + 2 * i]) == 0)
            {
                flag = 1;
                attrOffset[i] = relCache[relNum[i]].attrHead[j].offset;
            }
            //printf("%s\t%s\t%d\n",relCache[relNum[i]].attrHead[j].attrName,argv[3+2*i],flag);
        }
        if (flag == 0)
        {
            valid &= 0;
            //printf("ZZ");
        }
        if (!valid)
        {
            printf("Given join attribute %s not present in relation %s.\n", argv[i * 2 + 3], relCache[relNum[i]].relName);
            return valid;
        }
    }

    if (relCache[relNum[0]].attrHead[attrOffset[0]].type != relCache[relNum[1]].attrHead[attrOffset[1]].type)
    {
        printf("Join attributes do not have the same types.\n");
        valid &= 0;
        return valid;
    }
    printf("off\t%d\t%d\n", attrOffset[0], attrOffset[1]);
    int count = (relCache[relNum[0]].numAttrs + relCache[relNum[1]].numAttrs - 1) * 2 + 2;
    char *string[count];
    string[0] = "create";
    string[1] = (char *)malloc(strlen(argv[1]));
    strcpy(string[1], argv[1]);
    int c = 2;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < relCache[relNum[i]].numAttrs; j++)
        {
            if (!(i == 1 && attrOffset[1] == j))
            {
                string[c] = (char *)malloc(strlen(relCache[relNum[i]].attrHead[j].attrName));
                strcpy(string[c], relCache[relNum[i]].attrHead[j].attrName);
                c++;

                switch (relCache[relNum[i]].attrHead[j].type)
                {
                case DTINT:
                {
                    string[c] = "i";
                }
                break;

                case DTFLOAT:
                {
                    string[c] = "f";
                }
                break;

                case DTSTRING:
                {
                    string[c] = (char *)malloc(3);
                    sprintf(string[c], "s%d", relCache[relNum[i]].attrHead[j].length - 1);
                }
                break;
                }
                c++;
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        printf("%s\t%d\n", string[i], strlen(string[i]));
    }

    if (Create(count, string) != 1)
    {
        valid &= 0;
        return 0;
    }
    return valid;
}
