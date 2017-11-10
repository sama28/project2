#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include <stdio.h>

#include<errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
void testMain();
int OpenDB (argc, argv)
int	argc;
char	**argv;

{
  int flag=1,p;
  struct dirent *dp;
  char path[MAX_PATH_LENGTH],*c;

	strcpy(path,HOME_MINIREL);
	c=strcat(path,"/data/");
	strcpy(path,c);
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
      unsigned char a[58]="absdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghj";
      testMain();

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
      printf("\n---------------------------------------------------\n");
  }
  return 0;
  printf("");
  return 0;
}

shwRelCatRec(char recptr[]);
relCacheTest();

void testMain()
{
//---------------------------
      //InsertRec(0,&a);
      //testing purpuse not part of original caode
      //relCache[1].dirty=1;
      int lwrlm=0;
      int uprlm=PAGESIZE;
      int rln = 0;
      Rid foundRid, startRid;
      gPgTable[rln].pid=3;
      startRid.pid = 0;
      startRid.slotnum =0;
      char record[MR_RELCAT_REC_SIZE]; //attrcat rec size
      
     //ReadPage(rln,0);
      GetNextRec(rln, &startRid, &foundRid, record);
      shwRelCatRec(record);

      gPgTable[rln].contents[0]=0x40;
      GetNextRec(rln, &startRid, &foundRid, record);
      //relCacheTest();
      //printf("relFile=%d atrFile%d",relCache[0].relFile,relCache[1].relFile);
      printf("\n\nRecord Of RelCat Is");
     // printf("\n\n%x", record); 
      shwRelCatRec(record);

      rln=1;
      GetNextRec(rln, &startRid, &foundRid, record);
      shwAttrCatRec(record);

      startRid.slotnum=4;
      GetNextRec(rln, &startRid, &foundRid, record);
      shwAttrCatRec(record);

      startRid.slotnum=4;
      GetNextRec(rln, &startRid, &foundRid, record);
      shwAttrCatRec(record);
            
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
shwRelCatRec(char recptr[])
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
  bread_string(recptr,RELNAME,&val,d);
  e=bread_int(recptr,4,&val);
  f=bread_int(recptr,4,&val);
  g=bread_int(recptr,4,&val);//numPgs
  h=bread_int(recptr,4,&val);
  i=bread_int(recptr,2,&val);
  j=bread_int(recptr,4,&val);
  k=bread_int(recptr,4,&val);
  //l=bread_int(recptr,4,&val);

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
shwAttrCatRec(char recptr[])
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
  bread_string(recptr,RELNAME,&val,d);
  e=bread_int(recptr,4,&val);
  f=bread_int(recptr,4,&val);
  g=bread_int(recptr,2,&val);
  printf("\n\n%s %u %u %d",d,e,f,g);

}


relCacheTest()
{


printf("\n\ninside RelCache Test.....");
printf("\n%s\n%u\n%u\n%u\n%u\n%u\n%u\n%u\n%c\n",relCache[0].relName,relCache[0].recLength,relCache[0].recPerPg,relCache[0].numPgs,relCache[0].numRecs,relCache[0].numAttrs,relCache[0].Rid.pid,relCache[0].Rid.slotnum,relCache[0].dirty);
printf("\n%s\n%u\n%u\n%u\n%u\n%u\n%u\n%u\n%c\n",relCache[1].relName,relCache[1].recLength,relCache[1].recPerPg,relCache[1].numPgs,relCache[1].numRecs,relCache[1].numAttrs,relCache[1].Rid.pid,relCache[1].Rid.slotnum,relCache[1].dirty);


}