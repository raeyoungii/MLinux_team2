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
    MakeDir(dirTree, cmd, 0);
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

int cat(DirectoryTree* dirTree, char* cmd)
{
    char* str;
    /**
        cat0: write, EOF to save
        cat1: read
        cat2: read w/ line number
    **/
    if(strcmp(cmd, ">") == 0){
        str = strtok(NULL, " ");
        Concatenate(dirTree, str, 0);
        return 1;
    }
    else if(strcmp(cmd, "-n")== 0){
        str = strtok(NULL, " ");
        Concatenate(dirTree, str, 2);
    }
    else if(strcmp(cmd, "-b")== 0){
        str = strtok(NULL, " ");
        Concatenate(dirTree, str, 3);
    }
    else{
        Concatenate(dirTree, cmd, 1);
    }
    return 0;
}

int chmod(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    int tmp;

    if(strcmp(cmd, "-r") == 0){
        str = strtok(NULL, " ");
        if(str[0]-'0'<8 && str[1]-'0'<8 && str[2]-'0'<8 && strlen(str)==3){
            tmp = atoi(str);
            str = strtok(NULL, " ");
            tmpNode = IsExist(dirTree, str);
            if(tmpNode != NULL){
                ChangeModeAll(tmpNode, tmp);
            }
            else{
                printf("No file exists.\n");
                return -1;
            }
        }
        else{
            printf("wrong command\n");
            return -1;
        }
    }
    else{
        if(cmd[0]-'0'<8 && cmd[1]-'0'<8 && cmd[2]-'0'<8 && strlen(cmd)==3){
            tmp = atoi(cmd);
            str = strtok(NULL, " ");
            ChangeMode(dirTree, tmp, str);
        }
        else{
            printf("wrong command\n");
            return -1;
        }
    }
    return 0;
}

void Instruction(DirectoryTree* dirTree, Stack* dirStack, char* cmd)
{
    char* str;
    int val;
    if(strcmp(cmd, "") == 0){
        return;
    }
    str = strtok(cmd, " ");

    if(strcmp(str, "mkdir") == 0){
        str = strtok(NULL, " ");
        val = mkdir(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "rm") == 0){
        str = strtok(NULL, " ");
        val = rm(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
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
    else if(strcmp(str, "cat") == 0){
        str = strtok(NULL, " ");
        val = cat(dirTree, str);
        if(val == 1){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "chmod") == 0){
        str = strtok(NULL, " ");
        val = chmod(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else{
        printf("wrong command\n");
    }
}
