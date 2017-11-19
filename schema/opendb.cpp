#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include "../include/mrdtypes.h"
#include <stdio.h> 

#include<errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
void testMain();
void shwRelCatRec(unsigned char recptr[]);
void shwAttrCatRec(unsigned char recptr[]);
void relCacheTest(void);
void testReadFile(void);
void findRelNumTest(void);
void openRelTest(void);
void readRelation(void);
void parseRecord(int relNum,char* record);
void chacheTest(int relNum);
int AddPage(int relNum);
int OpenDB(int argc,char ** argv)
{
  if(MR_CURR_DB[0]==0){
    int flag=1,p;
    struct dirent *dp;
    char path[MAX_PATH_LENGTH],*c;
    strcpy(path,HOME_MINIREL);
    c=strcat(path,"/data/");
    strcpy(path,c);
    printf("%s",path);
    DIR *dir= opendir(path);
    if (dir){
      while((dp =readdir(dir))!=NULL && flag){
        if(!(p=strcmp(dp->d_name,argv[1])))
          flag=0;
      }
      if(flag==0){
        strcpy(MR_CURR_DB,argv[1]);
        printf("\n---------------------------------------------------\n");
        printf("Database %s is opened successfully.---------------------\n",MR_CURR_DB);
        printf("\n---------------------------------------------------\n");
        
        OpenCats();
        unsigned char a[59]="absdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghj";
        
        //InsertRec(0,a);InsertRec(0,a);FlushPage(0,0);
        //for(int i=0;i<512;i++)
        //printf("%02x",gPgTable[0].contents[i]);
        //testMain();
        //readRelation();
        //chacheTest(0);
        //openRelTest();
      }
      else{
        printf("\n---------------------------------------------------\n");
        printf("Database not found.----------------------------------\n");
        printf("\n---------------------------------------------------\n");
      }
    }
    else if (errno){
        printf("\n---------------------------------------------------\n");
        printf("Error opening database.\n%s---------------------------\n",strerror(errno));
        printf("\n-------------------------------------------------------\n");
    }
    return 0;
  }
  else{
    printf("db already open.");
  }
}
void testMain()
{
//------------------------------

      //InsertRec(0,&a);
      //testing purpuse not part of original caode
      //relCache[1].dirty=1;
      int lwrlm=0,status;
      int uprlm=PAGESIZE;
      int rln = 0;
      Rid foundRid, startRid;
      //gPgTable[0].pid=3;
      //gPgTable[1].pid=10;
      startRid.pid = 0;
      startRid.slotnum =0;
      char record[MR_RELCAT_REC_SIZE];
      char record1[MR_ATTRCAT_REC_SIZE];//attrcat rec size

      //--------------------------------------------
      //data to test findRec-----------------------
      int a_type=relCache[0].attrHead[0].type;
      int a_size=relCache[0].attrHead[0].length;
      int a_offset=relCache[0].attrHead[0].offset;
      int a_compOp=LT;
      char a_vptr[100]="b";
      //unsigned val=3;
      //strcpy(a_vptr,(char*)&val);

      
      //ReadPage(,1);
      //testReadFile();
      //relCacheTest();
  /*
      GetNextRec(0, &startRid, &foundRid, record);
      shwRelCatRec((unsigned char *)record);

       startRid.slotnum =1;
      GetNextRec(rln, &startRid, &foundRid, record);
     
      //printf("relFile=%d atrFile%d",relCache[0].relFile,relCache[1].relFile);
      //printf("\n\nRecord Of RelCat Is");
     // printf("\n\n%x", record); 
      shwRelCatRec((unsigned char *)record);
    
      rln=1;
      startRid.slotnum=0;
      GetNextRec(rln, &startRid, &foundRid, record1);
      /*
      printf("\n\n-----------------\n\nBefore FindRec...value of att=%s",relCache[1].attrHead[2].attrName);
      printf("\n\n-----------------\n\nBefore FindRec...value of att=%u",relCache[1].attrHead[2].offset);
      printf("\n\n-----------------\n\nBefore FindRec...value of att=%u",relCache[1].attrHead[2].length);
      printf("\n\n-----------------\n\nBefore FindRec...value of att=%d",relCache[1].attrHead[2].type);
      FindRec(rln,&startRid,&foundRid,record1,a_type,a_size,a_offset,a_vptr,a_compOp);
  
      shwAttrCatRec((unsigned char*)record1);
      */

      /*
      for(int i=0;i<100;i++)
      {
      printf("\n\n------------%d",i);
      status=GetNextRec(1, &startRid, &foundRid, record1);
        if(status==1)
        {
            shwAttrCatRec((unsigned char *)record1);
        }
      startRid.slotnum=foundRid.slotnum+1;
      startRid.pid=foundRid.pid;
      }
      startRid.pid=0;
      startRid.slotnum=0;
      for(int i=0;i<100;i++)
      {
      printf("\n\n------------%d",i);
      status=GetNextRec(0, &startRid, &foundRid, record);
      if(status==1)
      {
         shwRelCatRec((unsigned char *)record);
      }
     
       startRid.slotnum=foundRid.slotnum+1;
      startRid.pid=foundRid.pid;
      }
            
           /* printf("\n\nInside opendb\testMain\n\nTesting ReadPage");
            //printf("\n\ncontent of the whole file\n");
            for(int i=lwrlm;i<uprlm;i++)//read the content from lower limit to upper limit
            {
              printf("%c",gPgTable[rln].contents[i]);
              //printf("%c",tempbuff[i]);
            }
            //shwAttrRec(gPgTable[1].contents);
            //------------------------------------
            */
}
void shwRelCatRec(unsigned char recptr[])
{
 unsigned char d[RELNAME];
  //char e[4],f[4];
  unsigned int e,f,g,h,i,j,k,l;
  //short unsigned g;
  int val=0;
  //reffernces...
  //bread_string(unsigned char* buffer,int size,int* init,unsigned char* tmp);
  //unsigned int bread_int(unsigned char* buffer,int size,int* init)
  //relcat(relName,recLength,rcPerPg,numPgs,numRecs,numAttrs,pid,rid)

  val=0;
  bread_string((unsigned char *)recptr,RELNAME,&val,d);
  e=bread_int((unsigned char *)recptr,4,&val);
  f=bread_int((unsigned char *)recptr,4,&val);
  g=bread_int((unsigned char *)recptr,4,&val);//numPgs
  h=bread_int((unsigned char *)recptr,4,&val);
  i=bread_int((unsigned char *)recptr,2,&val);
  j=bread_int((unsigned char *)recptr,4,&val);
  k=bread_int((unsigned char *)recptr,4,&val);
  //l=bread_int((unsigned char *)recptr,4,&val);

 /*
  fread(d,32,1,fda);
  fread(&e,4,1,fda);
  fread(&f,4,1,fda);
  fread(&g,2,1,fda);
*/
/*
memcpy(d,recptr,32);
memcpy(e,recptr+32,4);
memcpy(f,recptr+32+4,4);
memcpy(e,recptr+32+4+4,2);

printf("\n\n%s\n%u\n%u\n%d",(char*)d,(unsigned)e,(unsigned)f,(short)g);
*/
printf("\n\n%s %u %u %u %u %u %u %u\n\n",d,e,f,g,h,i,j,k);

}
void shwAttrCatRec(unsigned char recptr[])
{
  unsigned char d[RELNAME];
  //char e[4],f[4];
  unsigned int e,f;
  short unsigned g;
  int val=0;
  //reffernces...
  //bread_string(unsigned char* buffer,int size,int* init,unsigned char* tmp);
  //unsigned int bread_int(unsigned char* buffer,int size,int* init)
  //relcat(relName,recLength,rcPerPg,numPgs,numRecs,numAttrs,pid,rid)

  val=0;
  bread_string((unsigned char *)recptr,RELNAME,&val,d);
  e=bread_int((unsigned char *)recptr,4,&val);
  f=bread_int((unsigned char *)recptr,4,&val);
  g=bread_int((unsigned char *)recptr,2,&val);
  printf("\n\n%s %u %u %d",d,e,f,g);

}


void relCacheTest(void )
{
  printf("\n\ninside RelCache Test.....");

  printf("RelCacheIndex %d",relCacheIndex);
  if(relCache[0].relFile!=NULL)
  {
     printf("\n%s\n%u\n%u\n%u\n%u\n%u\n%u\n%u\n%c%c\n",relCache[0].relName,relCache[0].recLength,relCache[0].recPerPg,relCache[0].numPgs,relCache[0].numRecs,relCache[0].numAttrs,relCache[0].Rid.pid,relCache[0].Rid.slotnum,relCache[0].dirty,relCache[0].valid);
  }
  
  if(relCache[1].relFile!=NULL)
  {
     printf("\n%s\n%u\n%u\n%u\n%u\n%u\n%u\n%u\n%c%c\n",relCache[1].relName,relCache[1].recLength,relCache[1].recPerPg,relCache[1].numPgs,relCache[1].numRecs,relCache[1].numAttrs,relCache[1].Rid.pid,relCache[1].Rid.slotnum,relCache[1].dirty,relCache[1].valid);
  }

}
void testReadFile(void)
{
  printf("\n\nIN TESTREADFILE");
  FILE*fd ,*fda; 
  fd =relCache[0].relFile; //fopen(path,"rb+");
  fda =relCache[1].relFile; //fopen(path1,"rb+");

  //long offset=sizeof(pg.slotmap);
  char c[RELNAME]="reName";
  char d[RELNAME],b[32];
  int atcatofst;
  Rid recid;

  unsigned int e,f,x=5,y=6,nr,np;
  unsigned short g,z=7;
  unsigned char e1;
  int i;
  if(fd>0)
  {
  //	offset=s[4];//size of bitmapslot;		
  /*	fseek(fd,offset,SEEK_SET);
    fwrite(c,32,1,fd);
    fwrite(&x,4,1,fd);
    fwrite(&y,4,1,fd);
    fwrite(&z,2,1,fd);										
  */		
  //printf("\nfile opened successfully fd= %d,offset=%d",fd,offset);//debug code			
  //	fseek(fd,offset,SEEK_SET);			
    printf("\ndirect 4 byte result");				
    fseek(fd,0,SEEK_SET);
    for(i=0;i<30;i++)
    {
      fread(&e1,1,1,fd);
      printf("\n%x",e1);
    }
    printf("\n------------");
    //fread(b,32,1,fd);
    
    printf("\nbitmap of relcat");
    fseek(fd,0,SEEK_SET);
    for(i=0;i<MR_RELCAT_BITMS_NUM;i++)
    {
      fread(&e1,1,1,fd);
      printf("\n%x",e1);
    }

    fread(d,32,1,fd);
    fread(&e,4,1,fd);
    fread(&f,4,1,fd);
    fread(&np,4,1,fd);
    fread(&nr,4,1,fd);
    fread(&g,2,1,fd);
    fread(&(&recid)->pid,4,1,fd);
    fread(&(&recid)->slotnum,4,1,fd);

    
    //printf("\nfile opened successfully fd= %d,offset=%d",fd,offset);//debug code, can be removed
    
    printf("/ndataread is:-%s %u %u %u %u %u %u %u\n",d,e,f,g,np,nr,recid.pid,recid.slotnum);


    fread(d,32,1,fd);
    fread(&e,4,1,fd);
    fread(&f,4,1,fd);
  
    fread(&np,4,1,fd);
    fread(&nr,4,1,fd);
    fread(&g,2,1,fd);
    fread(&(&recid)->pid,4,1,fd);
    fread(&(&recid)->slotnum,4,1,fd);
    printf("/ndataread is:-%s %u %u %u %u %u %u %u\n",d,e,f,g,np,nr,recid.pid,recid.slotnum);
    //------------------------------------------------------------------------------------
    printf("\n Bitmap of attrcat is :- ");
    //reading attribute catalog
    //---------------------------
    /*fseek(fda,0,SEEK_SET);
    for(i=0;i<48;i++)
    {
      fread(&e,4,1,fda);
      printf("\n%x",e);
    }
    //-----------------------
    */
    fseek(fda,0,SEEK_SET);
      for(i=0;i<MR_ATTRCAT_BITMS_NUM;i++)
      {
        fread(&e1,1,1,fda);
        printf("\n%x",e1);
      }
      atcatofst=MR_ATTRCAT_BITMS_NUM;//(PAGESIZE-PGTAIL_SPACE)/(8*MR_ATTRCAT_REC_SIZE+1);
      fseek(fda,atcatofst,SEEK_SET);
    for(i=0;i<12;i++)
    {
                  
      fread(d,32,1,fda);
      fread(&e,4,1,fda);
      fread(&f,4,1,fda);
      fread(&g,2,1,fda);
      printf("/nattrcat dataread is:-%s %u %u %u\n",d,e,f,g);
    }
  
  }
  else
  {
    printf("file can not be oppened fd %d errno %d",fd,errno);//debug code	
  }

}
void openRelTest(void)
{
  //relCache[1].valid='i';
  //gPgTable[2].dirty='d';
  printf("\n\n\n\nOPENRELTEST");

  int n1=OpenRel("rel5");
  printf("numpage of relNum %d=%u",n1,relCache[n1].numPgs);
  AddPage(n1);
  printf("after adding new page numpage of relNum %d=%u",n1,relCache[n1].numPgs);
  //ReadPage(n1,0);
  //gPgTable[n1].dirty='d';
  //relCache[n1].dirty='d';
  //ReadPage(n1,1);
  //int n2=OpenRel("rel2");
  //relCache[n1].dirty='d';

  //ReadPage(n1,0);
  //gPgTable[n1].dirty='d';
 
  //int n3=OpenRel("rel3");
 // ReadPage(n2,1);

   //int n3=OpenRel("trel");
  // ReadPage(n2,0);
 /// relCache[n3].dirty='d';
}
void readRelation(void)
{
      int status,i,relNum;
      Rid foundRid, startRid;
      gPgTable[0].pid=3;
      gPgTable[1].pid=10;
      startRid.pid = 0;
      startRid.slotnum =0;
      char record[MR_RELCAT_REC_SIZE];
      char record1[MR_ATTRCAT_REC_SIZE];//attrcat rec size

 if(relCache[0].relFile!=NULL &&relCache[1].relFile!=NULL)
{
      for(int i=0;i<relCache[1].numRecs+5;i++)
      {
        printf("\n\n------------%d",i);
        status=GetNextRec(1, &startRid, &foundRid, record1);
        if(status==1)
        {
            //shwAttrCatRec((unsigned char *)record1);
            parseRecord(1,record1);
        }
        startRid.slotnum=foundRid.slotnum+1;
        startRid.pid=foundRid.pid;
      }
      startRid.pid=0;
      startRid.slotnum=0;
      for(int i=0;i<relCache[1].numRecs+5;i++)
      {
      printf("\n\n------------%d",i);
      status=GetNextRec(0, &startRid, &foundRid, record);
      if(status==1)
      {
          //shwRelCatRec((unsigned char *)record);
          parseRecord(0,record);
      }
     
       startRid.slotnum=foundRid.slotnum+1;
       startRid.pid=foundRid.pid;
      }

      relNum=OpenRel("trel3");
      startRid.pid=0;
      startRid.slotnum=0;
      if(relCache[relNum].relFile!=NULL && relNum>1)
      {
        for(int i=0;i<relCache[relNum].numRecs+5;i++)
      {
         printf("\n\n------------%d",i);
         status=GetNextRec(0, &startRid, &foundRid, record);
         if(status==1)
          {
            parseRecord(0,record);
          }
     
       startRid.slotnum=foundRid.slotnum+1;
       startRid.pid=foundRid.pid;
      }
  }
  
}
else
{
  printf("\n\nDATABASE IS NOT OPENED....");
} 
}
void chacheTest(int relNum)
{
  printf("\n\n<<<<RELCAHCE TEST>>>>");
if(relCache[relNum].relFile!=NULL)
  {   
     printf("\n%s\n%u\n%u\n%u\n%u\n%u\n%u\n%u\n%c%c\n",relCache[0].relName,relCache[0].recLength,relCache[0].recPerPg,relCache[0].numPgs,relCache[0].numRecs,relCache[0].numAttrs,relCache[0].Rid.pid,relCache[0].Rid.slotnum,relCache[0].dirty,relCache[0].valid);
     printf("gPgTable[%d] valid %c,dirty %c pid %u",relNum,gPgTable[relNum].valid,gPgTable[relNum].dirty,gPgTable[relNum].pid);
     for(int i=0;i<relCache[relNum].numAttrs;i++)
     {
       printf("\n\n");
       printf("attrName=%s  ,",relCache[relNum].attrHead[i].attrName);
     printf("offset=%u   ,",relCache[relNum].attrHead[i].offset);
     printf("length=%u   ,",relCache[relNum].attrHead[i].length);
     printf("tpe=%d  ,",relCache[relNum].attrHead[i].type);
     }
  }
}
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

    
    relCache[relNum].numPgs=relCache[relNum].numPgs+1;
    relCache[relNum].dirty='d';
    flushRelCacheEntry(relNum);

    //---------temporaray---------
    /*
    //ALTERNATIVE TO ABOVE FULSH RELCACHE ENTRY IMMEDIATE FLUSH  
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
    */ 
}