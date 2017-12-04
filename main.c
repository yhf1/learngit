#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cp_single.h"
#include "cp_directory.h"
#define PATH_MAX 260


char dest_path[PATH_MAX+1];
char temp_dest_path[PATH_MAX+1];
char src_path[256][PATH_MAX+1];
int i,k,num,src_num,index_r,param_r;
struct stat buf;

int main(int argc,char *argv[])
{
	/***************check the legality of option,only -r or -R****************/
	for(i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			if((!strcmp(argv[i],"-r"))||(!strcmp(argv[i],"-R"))){
				param_r=1;
				index_r=i;
			}
			else{
				printf("my_cp:invalid options: %s\n",argv[i]);
				exit(1);
			}
		}
	}
	
	if(param_r){
		num=argc-1-1;
		src_num=num-1;
	}
	else{
		num=argc-1;
		src_num=num-1;
	}
	if(num<2){
		printf("my_cp:[option] (source)(dest)\n");
		exit(1);
	}
	
	// extract the dest path
	if(index_r!=argc-1){
		strcpy(dest_path,argv[argc-1]);
	}
	else{
		strcpy(dest_path,argv[argc-2]);
	}
	
	//extract the src path
	k=0;
	for(i=1;i<argc-1;i++){
		if(i==index_r&&param_r)
			continue ;
		else{
			strcpy(src_path[k++],argv[i]);
		}
	}
	
	//当源文件个数大于1的时候
	if(src_num>1){
		if(stat(dest_path,&buf)==-1){
			printf("my_cp:\"%s\"is not a directory.\n",dest_path);
			exit (1);
		}
		//the dest path is vaild
		if(S_ISDIR(buf.st_mode)){
			strcpy(temp_dest_path,dest_path);
			//the dest path is directory
			//一一解析源文件，可能都是普通文件；也可能都是目录；如果既有普通文件，又有目录，则跳过目录
			for(i=0;i<src_num;i++){
				if(stat(src_path[i],&buf)==-1){
					printf("my_cp: can't get file status of \"%s\":no this file or directory.\n",src_path[i]);
					continue ;	
				}
				//the src_path exist
				if(!S_ISDIR(buf.st_mode)){
					cp_single(src_path[i],dest_path);
				}
				else if(param_r){
					cp_directory(src_path[i],dest_path);
				}
				else{
					printf("my_cp:skip the directory:\"%s\".\n",src_path[i]);
				}
				strcpy(dest_path,temp_dest_path);
			}
				
		}
		else{
			printf("my_cp:\"%s\"is not a dirtory.\n",dest_path);
			exit(1);
		}
	}
	
	//当源文件个数为1的时候
	else{
		//only one src path
		if(stat(src_path[0],&buf)==-1){
			printf("my_cp: can't get file status of \"%s\":no this file\n",src_path[0]);
			exit (1);
		}
		if(S_ISDIR(buf.st_mode)){
			if(param_r){
				//printf("main.c 1 cp_dir 108\n");
				cp_directory(src_path[0],dest_path); //源文件是目录，并且输入了"-r or -R"
				exit (0);
			}
			else{
				printf("my_cp:skip the directory :\"%s\":\n",src_path[0]); //源文件是目录，没有携带"-r or -R"
				exit (1);
			}
		}
		else{
			cp_single(src_path[0],dest_path);    //源文件就是个普通文件
		}
	}
}

















