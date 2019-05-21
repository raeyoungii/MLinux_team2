#include <stdio.h>
#include <stdlib.h>
#include "main.h"
//command
int mkdir(DirectoryTree* dirTree, char* cmd)
{
    //if -m: permission
    //if -p:
    //if cmd == aaa bbb : make two folders
    //if exist: return -1;
    MakeDir(dirTree, cmd);
    return 0;
}
int rm(DirectoryTree* dirTree, char* cmd)
{
    RemoveDir(dirTree, cmd);
    return 0;
}
int cd(DirectoryTree* dirTree, char* cmd)
{
    MovePath(dirTree, cmd);
    return 0;
}

int pwd(DirectoryTree* dirTree, Stack* dirStack)
{
    PrintPath(dirTree, dirStack);
    return 0;
}

int ls(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    int val;

    if(cmd == NULL){
        ListDir(dirTree, 0, 0);
        return 0;
    }

    if(cmd[0] == '-'){
        if(strcmp(cmd, "-al") == 0){
            str = strtok(NULL, " ");
            if(str != NULL){
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if(val != 0)
                    return -1;
            }
            ListDir(dirTree, 1, 1);
        }
        else if(strcmp(cmd, "-l") == 0){
            str = strtok(NULL, " ");
            if(str != NULL){
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if(val != 0)
                    return -1;
            }
            ListDir(dirTree, 0, 1);
        }
        else if(strcmp(cmd, "-a") == 0){
            str = strtok(NULL, " ");
            if(str != NULL){
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if(val != 0)
                    return -1;
            }
            ListDir(dirTree, 1, 0);
        }
        else{
                printf("wrong command\n");
            return -1;
        }
    }
    else{
        tmpNode = dirTree->current;
        val = MovePath(dirTree, cmd);
        if(val != 0)
            return -1;
        ListDir(dirTree, 0, 0);
        dirTree->current = tmpNode;
    }

    if(str != NULL)
        dirTree->current = tmpNode;

    return 0;
}

void Instruction(DirectoryTree* dirTree, char* cmd)
{
    char* str;
    if(strcmp(cmd, "") == 0){
        return;
    }
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
    else if(strcmp(str, "ls") == 0){
        str = strtok(NULL, " ");
        ls(dirTree, str);
    }
    else if(strcmp(str, "pwd") == 0){
        pwd(dirTree, dStack);
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
