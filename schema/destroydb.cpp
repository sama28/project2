
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/fncn.h"
#include <stdio.h>
#include <dirent.h>
#include<sys/stat.h>
#include<string.h>

int recursive_delete(char* dir_path,int error){
	DIR *dir;
	struct dirent *root;
	//printf("%s\n",dir_path);
    	char path[255];
    	strcpy(path,dir_path);
    	if(path[0]==0){
    		//printf("Invalid path for deleting directory.\n");
    		error=error & 0;
    	}
	dir = opendir(dir_path);
	if (dir == NULL) {
		//printf("Unable to access the directory %s\n",dir_path);
		error=error & 0;
	}
	strcat(path,"/");
	
	while ((root = readdir(dir)) != NULL) {
		if(strcmp(root->d_name,".") && strcmp(root->d_name,"..")){
			if(root->d_type==DT_DIR){
				strcpy(path,dir_path);
				strcat(path,"/");
				strcat(path,root->d_name);
				//printf("%s\n",path);
				chmod(path,S_IWUSR|S_IRUSR|S_IXUSR);
				recursive_delete(path,error);
			}	
			else if(root->d_type==DT_REG){
				strcpy(path,dir_path);
				strcat(path,"/");
				strcat(path,root->d_name);
				chmod(path,S_IWUSR|S_IRUSR|S_IXUSR);
				int ret=remove(path);
				if(ret!=0){
		//			printf("Error occurred while deleting %s\n",path);
					error=error & 0;
				}
				//printf("%d\t%s\n",ret,path);
			}
		}
	}
	int ret2=remove(dir_path);
	if(ret2!=0){
		//printf("Error occurred while deleting %s\n",path);
		error=error & 0;
	}
	return error;	
}


int DestroyDB(int argc,char ** argv)
{
    //Check if current db is the one to be deleted.
    char *dbname;
    dbname=basename(argv[1]);
    //printf("%s",dbname);
    if(!strcmp(MR_CURR_DB,dbname)){
        MR_CURR_DB[0]=0;
    }
    if(recursive_delete(argv[1],1))
		 return (OK);  /* all's fine */
	else 
		return (NOTOK);
}


