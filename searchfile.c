#include "head.h"

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include<dirent.h>

bool scanfile(char* path,const char* filename){
    bool res = false;
    DIR* dir = opendir(path);
    struct dirent* entry;
    while(entry = readdir(dir)){
        if (0 == strcmp(entry->d_name, filename)){
            res = true;
            break;
        }
    }
    return res;
}

const char* searchfile(const char* filename){  
    //printf(">>%s<<\n", filename);
    const char* kPath = getenv("PATH");
    int size = (strlen(kPath)+1);
    char* path = (char*)malloc((size)*sizeof(char*));
    //add . to $PATH
    //char* path = malloc((2+size)*sizeof(char*));
    //path[0]='.';
    //path[1]=':';
    for(int i=0;i<size;++i){
        //path[i+2] = kPath[i];
        path[i] = kPath[i];
    }
    //printf("%s\n", path);
    //strtok具有状态！线程不安全，并且会改变原字符串！
    //printf("==%s\n",path);
    //printf("=%s\n", filename);
    path = strtok(path, ":");
    bool res = false;
    while(path!=NULL){
        if(scanfile(path, filename)){
            printf("%s\n", path);
            res = true;
            break;
        }
        path = strtok(NULL, ":");
    }
    //
    if(res){
        int path_size = strlen(path);
        int cmd_size = strlen(filename);
        printf("=%d %d\n", path_size, cmd_size);
        int tot_size = path_size+cmd_size+2;
        char* filepath = (char* )malloc((tot_size)*sizeof(char));
        for(int i=0;i<tot_size;++i){
            if(i == path_size){
                filepath[i] = '/';
            }else if(i == tot_size - 1){
                filepath[i] = '\0';
            }else if(i < path_size){
                filepath[i] = path[i];
            }else if(i > path_size){
                filepath[i] = filename[i-path_size-1];
            }
        }
        return filepath;
    }else{
        return NULL;
    }

}
//void test(){
//    searchfile("ls");
//}
//void test2(){
//    searchfile("c");
//}
//void test3(){
//    if(!searchfile("u78")){
//        printf("u78 not found");
//    }
//}
//int main(void){
//    //test();
//    //test2();
//    test3();
//    return 0;
//}
//int main(void){
//    const char* s = searchfile("ls");
//    printf("%s\n", s);
//    return 0;
//}
