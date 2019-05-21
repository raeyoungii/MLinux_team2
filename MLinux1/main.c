#include <stdio.h>
#include <stdlib.h>
#include "main.h"
//command
void mkdir(DirectoryTree* dirTree, char* cmd)
{
    MakeDir(dirTree, cmd);
}
void rm(DirectoryTree* dirTree, char* cmd)
{
    RemoveDir(dirTree, cmd);
}
void cd(DirectoryTree* dirTree, char* cmd)
{
    MovePath(dirTree, cmd);
}

void pwd(DirectoryTree* dirTree, Stack* dirStack)
{
    PrintPath(dirTree, dirStack);
}

int ls(DirectoryTree* dirTree, char* cmd)
{
    if(cmd == NULL){
       ListDir(dirTree, 0, 0);
    }
    else if(strcmp(cmd, "-al") == 0){
        ListDir(dirTree, 1, 1);
    }
    else if(strcmp(cmd, "-l") == 0){
        ListDir(dirTree, 0, 1);
    }
    else if(strcmp(cmd, "-a") == 0){
        ListDir(dirTree, 1, 0);
    }
    else{cmd = strtok(NULL, " ");
        return -1;
    }
    return 0;
}

void Instruction(DirectoryTree* dirTree, char* cmd)
{
    char* str;

    str = strtok(cmd, " ");

    if(strcmp(str, "mkdir") == 0){
        str = strtok(NULL, " ");
        mkdir(dirTree, str);
    }
    else if(strcmp(str, "rm") == 0){
        str = strtok(NULL, " ");
        rm(dirTree, str);
    }
    else if(strcmp(str, "cd") == 0){
        str = strtok(NULL, " ");
        cd(dirTree, str);
    }
    else if(strcmp(str, "pwd") == 0){
        pwd(dirTree, dStack);
    }
    else if(strcmp(str, "ls") == 0){
        str = strtok(NULL, " ");
        ls(dirTree, str);
    }
    else{
        printf("wrong command\n");
    }
}

int main()
{
    char cmd[50];
    Linux = InitializeTree();
    dStack = InitializeStack();

    while(1){
        printf("[root@localhost %s] $ ",Linux->current->name);
        gets(cmd);
        Instruction(Linux, cmd);
    }

    return 0;
}
