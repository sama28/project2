int AddPage(int relNum)
{
  printf("\n\nInADD PAGE ...");
  
  int status=0;
    FlushPage(relNum,gPgTable[relNum].pid);
    unsigned char rec[MR_RELCAT_REC_SIZE];
    for(int i=0;i<PAGESIZE;i++)
    {
        gPgTable[relNum].contents[i]='\0';
    }
    gPgTable[relNum].dirty='d';
    gPgTable[relNum].pid=relCache[relNum].numPgs;
    gPgTable[relNum].valid='v';

    /*
    relCache[relNum].numPgs=relCache[relNum].numPgs+1;
    relCache[relNum].dirty='d';
    flushRelCacheEntry(relNum);
    */
    //---------temporaray---------
    
    relCache[relNum].numPgs=relCache[relNum].numPgs+1;
    relCache[relNum].dirty='d';
    unsigned offset;
    ReadPage(0,relCache[relNum].Rid.pid);
    makeRelCatRec(rec,relNum);
    if(gPgTable[0].pid==relCache[relNum].Rid.pid)
    {
      printf("Writing RelCache To Buffer");
      offset=MR_RELCAT_BITMS_NUM+relCache[0].recLength*relCache[relNum].Rid.slotnum;
      //shwRelCatRec(rec);
      //shwRelCatRec(&gPgTable[0].contents[offset]);
      for(int i=0;i<relCache[0].recLength;i++)
      {
        gPgTable[0].contents[offset+i]=rec[i];
      }
      //shwRelCatRec(&gPgTable[0].contents[offset]);
      gPgTable[0].dirty='d';
      relCache[relNum].dirty='c';
    }
    FlushPage(0,gPgTable[0].pid);
    
}