
#ifndef FNCTN
#define FNCTN

void getPath(char* path,char* filename);
unsigned int bread_int(unsigned char* buffer,int size,int* init);
void bread_string(unsigned char* buffer,int size,int* init,unsigned char* tmp);
void getBinary(unsigned int* z,unsigned int x);
int getDecimal(unsigned int* z);
int FlushPage(int relNum,unsigned pgid);
int isPgInBuff(int relNum,unsigned pgid );
int isPgInBuff(int relNum,unsigned pgid );
void showPageContent(int relNum);
void ReadPage(int relNum,unsigned pid);
int GetNextRec(int relNum,Rid * startRid,Rid* foundRid,char * recPtr);
int fileSize(FILE *fp);
int CreateCats(char *d);
void OpenCats(void);

#endif