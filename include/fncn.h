
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
void InsertRec(int relNum,unsigned char *rec);
void WriteRec(int relNum,unsigned char* rec,Rid* rid);
void GetSlots(struct recidArray* ridArray,int count,int relNum);
void CloseCats(void);
void bwrite_int(unsigned char* tmp,int num,int size,int* init);
int isRecRight(int relNum,char *recPtr,int offset,char *valuePtr,int compOp );
unsigned attrOfstInRec(char *recPtr,int relNum,int offset);
int FindRec(int relNum,Rid*startRid,Rid *foundRid,char *recPtr,unsigned short attrType,unsigned attrSize,unsigned offset,char *valuePtr,int compOp );
int FindRelNum(char *relName);
int nextRec(int relNum, Rid* startRid, char *recPtr);
int OpenRel(char * relName);
#endif