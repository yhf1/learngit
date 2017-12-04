#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "cp_single.h"
//#define PATH_MAX 260


void cp_directory(char * original_src_path,char * original_dest_path)
{
	struct stat buf;
	DIR *dir;
	struct dirent *ptr;
	char path[PATH_MAX+1];
	char src_path[PATH_MAX+1],dest_path[PATH_MAX+1];
	int a=0,b=0;
	
	strcpy(src_path,original_src_path);
	strcpy(dest_path,original_dest_path);
	
	//如果原路径的最低级目录末端没有加'/'的话，把'/'加在最低级目录的末端
	if(src_path[strlen(src_path)-1]!='/'){
		strncat(src_path,"/",1);
	}
	
	//如果目的路径的最低级目录不存在，用mkdir创建最低级目录，如果次低级目录也不存在，则在创建的时候报错
	if(stat(dest_path,&buf)==-1){
		//creat a directory which name is dest_path
		stat(src_path,&buf);
		if(mkdir(dest_path,buf.st_mode)==-1){
			printf("my_cp:create the directory\"%s\"erroe.\n",dest_path);
			return ;
		}
	}
	else{
		//exist   如果目的路径的最底层目录存在，判断它是不是目录
		if(!S_ISDIR(buf.st_mode)){
			printf("my_cp:the directory\"%s\" can't cover the no-directory\"%s\".\n",src_path);
			return ;
		}
		else{
			//如果目的路径的最低级目录末端没有加'/',则添加上'/'
				if(dest_path[strlen(dest_path)-1]!='/'){
	                strncat(dest_path,"/",1);
	            }
	            //extract the lowest directory 提取原路径最低级目录（包含'/'）
	            int i,k=0;
	            char lowestdir[PATH_MAX+1]={0};
	            for(i=strlen(src_path)-1-1;i>0;i--){
	                if(src_path[i]=='/'){
	                    i=i+1;
	                    break;
	                }
	            }
	 
	            for(;i<strlen(src_path);i++){
	                lowestdir[k++]=src_path[i];
	            }
                 //将原路径的最低级目录连接在目的路径的最低级目录下面
	            strncat(dest_path,lowestdir,strlen(lowestdir));
	            struct stat temp_buf;
	            char temp_path[PATH_MAX+1]="rm -rf ";
                 //如果之前的目的路径的最低级目录下面存在名字和源最低级目录一样的目录，则删除，重新创建
	            if(stat(dest_path,&temp_buf)==0){
	               strcat(temp_path,dest_path);
	                system(temp_path);
				}		
				
				if(mkdir(dest_path,buf.st_mode)==-1){
					printf("my_cp:create the directory \"%s\" error.\n",dest_path);
					return ;
				}
				//char temp_path[PATH_MAX+1]="mkdir ";
				//strcat(temp_path,dest_path);
				
				//system(temp_path);
	        }
	    
		}
		
	if((dir=opendir(src_path))==NULL){
	    printf("my_cp:open the srouce path \"%s\" error.\n",src_path);
	    return ;
	}
	char temp_dest_path[PATH_MAX+1];
	strcpy(temp_dest_path,dest_path);
	while((ptr=readdir(dir))!=NULL){
	    if(!strcmp(ptr->d_name,"."))
	        continue;
	    if(!strcmp(ptr->d_name,".."))
	        continue;
	    strcpy(path,src_path);
	    strcat(path,ptr->d_name);
	    if(stat(path,&buf)==-1){
	        printf("my_cp:open the file \"%s\" error.\n",path);
	        return ;
	    }
	    strcpy(dest_path,temp_dest_path);

	    //get the right dest_path
	    if(S_ISDIR(buf.st_mode)){
			printf("105 cp_dir=%d\n",b++);
			printf("path=%s\n",path);
			printf("dest_path=%s\n",dest_path);
	        cp_directory(path,dest_path);  
				printf("path=%s\n",path);
				printf("dest_path=%s\n",dest_path);
	    }
	    else{
			printf("path=%s\n",path);
			printf("dest_path=%s\n",dest_path);
			printf("115 cp_single=%d\n",a++);
	        cp_single(path,dest_path);
	    }
	}
}































