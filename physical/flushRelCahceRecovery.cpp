int flushRelCacheEntry(int Loc)
{   //ONLY FLUSHES RELCHACHE ENTRY TO THE RELCAT AND DOESNT DO ENYTHING FOR ATTRCAT
    //IF UPDATE COMMAND CHANGES THE ATTRIBUTE COULD CREATE PROBLEM
    printf("\n\nint flushRelCacheEntry(int Loc)=%d",Loc);
    int gPgFlushed=0;
    unsigned char cacheEntry[MR_RELCAT_REC_SIZE];
    unsigned offset;

    //check Befor Flshing Cache That Corresponding Page In gPgTable Is Not Dirty Or Flushed That Page 1st
    if(gPgTable[Loc].dirty=='d' && relCache[Loc].valid=='v')
    {
        printf("\n\nflushRelCacheEntry(int Loc)....Calling FLUSH=%d",Loc);
        gPgFlushed=FlushPage(Loc,gPgTable[Loc].pid);
    }
    else{
        gPgFlushed=1;
    }
    
    //---------------------------------------------
    //CODE FOR FLUSHING ATTRCAT ENTRY SHOLD BE hERE
    //....
    //....

    //---------------------------------------------

    printf("flushRelCacheEntry(int Loc) gPg Flushed %d---relcahe[loc].dirty=%c",gPgFlushed,relCache[Loc].dirty);    
    if(gPgFlushed==1)//after success fully dealling with gPgTable
    {
        //Check if Cache Entry Is Dirty Or Not Corresponding To The Location That Is To Be Replaced
        if(relCache[Loc].dirty=='d')
        {
            //write this entry into relation and attribute Catalog
            //insert A Record At Particular Place
            makeRelCatRec(cacheEntry,Loc);//put the Cache Content in cacheEntry Rec
            printf("\n\n Printing RelCat Record that IS PREPAREd cahceEntry....");
            shwRelCatRec(cacheEntry);
            //WriteRec(0,cacheEntry,&(relCache[Loc].Rid));//bug remove code with write checking
            
            //-----------------------------------
            //----------Quick Update-------------------
            
            ReadPage(0,relCache[Loc].Rid.pid);
            if(gPgTable[0].pid==relCache[Loc].Rid.pid)
            {
              printf("Writing RelCache To Buffer");
              offset=MR_RELCAT_BITMS_NUM+relCache[0].recLength*relCache[Loc].Rid.slotnum;
              //shwRelCatRec(rec);
              //shwRelCatRec(&gPgTable[0].contents[offset]);
              for(int i=0;i<relCache[0].recLength;i++)
              {
                gPgTable[0].contents[offset+i]=cacheEntry[i];
              }
              //shwRelCatRec(&gPgTable[0].contents[offset]);
                gPgTable[0].dirty='d';
                relCache[Loc].dirty='c';
            }
            FlushPage(0,gPgTable[0].pid);
            //-----------------------------------
            return 1;
        }
        else{
            return 1;
        }
    }
    return 0;
}