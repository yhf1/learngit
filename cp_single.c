#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
//#define PATH_MAX 260


void cp_single(char *temp_src_path,char * temp_dest_path)
{
	struct stat buf;
	int len;
	char ch[10],filename[PATH_MAX+1],dest_dir[PATH_MAX+1];
	int fdrd,fdwt,i,j,k;
	char src_path[PATH_MAX+1],dest_path[PATH_MAX+1];
	strcpy (src_path,temp_src_path);
	strcpy (dest_path,temp_dest_path);
	for(k=0;k<strlen(src_path);k++){
		if(src_path[k]=='/')
			break ;
	}
	char temp_path[PATH_MAX+1]="./";
	//查看源文件有没有绝对路径，如果没有在源文件的头部添加'./'
	if(k==strlen(src_path)){
		strcat(temp_path,src_path);
		strcpy(src_path,temp_path);
	}
	
	//extract the file name from src_path  (包含/,列如：/filename)
	for(i=strlen(src_path)-1;i>0;i--){
		if(src_path[i]=='/')
			break ;
	}
	j=k=0;
	for(j=i;j<strlen(src_path);j++){
		filename[k++]=src_path[j];
	}
	filename[k]='\0';
	
	//check the if dest path has exited  
	if(stat(dest_path,&buf)==0){
		//dest_path exsited 如果目的路径最底层是目录的话，则将源文件连接在它的末端
		if(S_ISDIR(buf.st_mode)){
			if(dest_path[strlen(dest_path)-1]=='/')
				dest_path[strlen(dest_path)-1]='\0';
			strcat(dest_path,filename);
		}
	}
	else{
		//the dest_path didn't exsited
		for(i=strlen(dest_path)-1;i>=0;i--){
			if(dest_path[i]=='/')
				break ;
		}
		if(i>=0){
			//查看次级目录有没有存在，如果不存在则退出
			strncpy(dest_dir,dest_path,i+1);
			if(stat(dest_dir,&buf)==-1){
				printf("mp_cp:accesing:\"%s\" :it isn't a directory.\n",dest_path);
				exit (1);
			}
		}
	}
	//fistly the content which was read from srouce file will be write the dest file
	if((fdrd=open(src_path,O_RDONLY))==-1){
		perror("open");
		exit(1);
	}
	if(lseek(fdrd,0,SEEK_END)==-1){
		perror("lseek");
		exit(1);
	}
	if((len=lseek(fdrd,0,SEEK_CUR))==-1){
		perror("lseek");
		exit(1);
	}
	if(lseek(fdrd,0,SEEK_SET)==-1){
		perror("lseek");
		exit(1);
	}
	//open the dest file
	if((fdwt=open(dest_path,O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1){
		perror("open");
		exit(1);
	}
	close(fdwt);
	if((fdwt=open(dest_path,O_WRONLY|O_APPEND))==-1){
		perror("open");
		exit(1);
	}
	while(len-->0){
		//write all characters to dest file
		if(read(fdrd,ch,1)!=1){
			perror("read");
			exit(1);
		}
		//printf("%c\n",ch[0]);
		if(write(fdwt,ch,1)!=1){
			perror("write");
			exit(1);
		}
	}
	
	//get src file's attributes
	if(fstat(fdrd,&buf)==-1){
		perror("fstat");
		exit(1);
	}
	
	//get the dest file's file acces fight
	if(fchmod(fdwt,buf.st_mode)==-1){
		perror("fchmod");
		exit (1);
	}
	
	//set file's user id and group id
	if(fchown(fdwt,buf.st_uid,buf.st_gid)==-1){
		perror("fchown");
		exit(1);
	}
	close(fdwt);
	close(fdrd);
}	


















