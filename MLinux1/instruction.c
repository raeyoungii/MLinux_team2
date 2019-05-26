#include <stdio.h>
#include <stdlib.h>
#include "main.h"
//command
int mkdir(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    char tmp[MAX_NAME];
    int val;

    if(cmd == NULL){
        printf("wrong command\n");
        return -1;
    }

    //if cmd == aaa bbb : make two folders
    //if exist: return -1;

    tmpNode = dirTree->current;
    if(strcmp(cmd, "-p") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("wrong command\n");
            return -1;
        }
        if(strncmp(str, "/", 1) == 0){
            dirTree->current = dirTree->root;
        }
        str = strtok(str, "/");
        printf("%s\n", str);
        while(str != NULL){
            val = Movecurrent(dirTree, str);
            if(val != 0){
                MakeDir(dirTree, str, 0);
                Movecurrent(dirTree, str);
            }
            str = strtok(NULL, "/");
            printf("%s", str);
        }
    }
    else{
        str = strtok(NULL, " ");
        if(str == NULL){
            if(strncmp(cmd, "/",1) == 0)
                dirTree->current = dirTree->root;

            if(strstr(cmd, "/") == NULL){
                MakeDir(dirTree, cmd, 0);
                return 0;
            }
            str = strtok(cmd, "/");
            while(str != NULL){
                val = Movecurrent(dirTree, str);
                if(val != 0){
                    strncpy(tmp, str, MAX_NAME);
                    str = strtok(NULL, "/");
                    if(str == NULL){
                        MakeDir(dirTree, tmp, 0);
                        dirTree->current = tmpNode;
                        return 0;
                    }
                    else{
                        printf("no Path.\n");
                        return -1;
                    }
                }
                str = strtok(NULL, "/");
            }
        }
        else{
            //fork;
        }
    }
    return 0;
}
int rm(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    char yn[MAX_LENGTH];

    if(cmd == NULL){
        printf("wrong command\n");
        return -1;
    }

    if(strcmp(cmd, "-r") == 0){
        str = strtok(NULL, " ");

        tmpNode = IsExist(dirTree, str, 'd');

        if(tmpNode == NULL){
            printf("path doesn't exist.\n");
            return -1;
        }
        else{
            printf("do you want to remove this file? [y/n] ");
            while(1){
            scanf("%s", yn);

                if(strcmp(yn, "y") == 0){
                    RemoveDir(dirTree, str);
                    return 0;
                }
                else if(strcmp(yn, "n") == 0)
                    return -1;
                else{
                    printf("please type [y/n] ");
                }
            }
        }
    }
    //need fix
    else if(strcmp(cmd, "-f") == 0){
        str = strtok(NULL, " ");

        tmpNode = IsExist(dirTree, str, 'f');
        tmpNode2 = IsExist(dirTree, str, 'd');

        if(tmpNode2 != NULL){
            printf("%s is a directory\n", str);
            return -1;
        }
        if(tmpNode == NULL){
            printf(" file doesn't exist.\n");
            return -1;
        }
        else{
            RemoveDir(dirTree, str);
        }
    }
    else if(strcmp(cmd, "-rf") == 0){
        str = strtok(NULL, " ");

        tmpNode = IsExist(dirTree, str, 'd');

        if(tmpNode == NULL){
            printf("path doesn't exist.\n");
            return -1;
        }
        else{
            RemoveDir(dirTree, str);
        }
    }
    else{
        tmpNode = IsExist(dirTree, cmd, 'f');
        tmpNode2 = IsExist(dirTree, cmd, 'd');

        if(tmpNode2 != NULL){
            printf("%s is a directory\n", cmd);
            return -1;
        }
        if(tmpNode == NULL){
            printf(" file doesn't exist.\n");
            return -1;
        }
        else{
            printf("do you want to remove this file? [y/n] ");
            while(1){
            scanf("%s", yn);

                if(strcmp(yn, "y") == 0){
                    RemoveDir(dirTree, cmd);
                    return 0;
                }
                else if(strcmp(yn, "n") == 0)
                    return -1;
                else{
                    printf("please type [y/n] ");
                }
            }
        }
    }

    return 0;
}
int cd(DirectoryTree* dirTree, char* cmd)
{
    char tmp[MAX_DIR];
    if(cmd == NULL){
        strcpy(tmp, usrList->current->dir);
        MovePath(dirTree, tmp);
    }
    else{
        MovePath(dirTree, cmd);
    }
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
        if(strcmp(cmd, "-al") == 0 || strcmp(cmd, "-la") == 0){
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
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    /**
        cat0: write, EOF to save
        cat1: read
        cat2: read w/ line number
    **/

    if(cmd == NULL){
        printf("wrong command\n");
        return -1;
    }

    if(strcmp(cmd, ">") == 0){
        str = strtok(NULL, " ");
        tmpNode = IsExist(dirTree, str, 'd');
        if(tmpNode != NULL){
            printf("%s is a directory.\n", str);
            return -1;
        }
        else{
        Concatenate(dirTree, str, 0);
        }
        return 0;
    }
    else if(strcmp(cmd, "-n")== 0){
        str = strtok(NULL, " ");
        tmpNode = IsExist(dirTree, str, 'd');
        tmpNode2 = IsExist(dirTree, str, 'f');

        if(tmpNode == NULL && tmpNode2 == NULL){
            printf("no directory of file.\n");
            return -1;
        }
        else if(tmpNode != NULL && tmpNode2 == NULL){
            printf("%s is a directory.\n", str);
            return -1;
        }
        else{
        Concatenate(dirTree, str, 2);
        }
    }
    else if(strcmp(cmd, "-b")== 0){
        str = strtok(NULL, " ");
        tmpNode = IsExist(dirTree, str, 'd');
        tmpNode2 = IsExist(dirTree, str, 'f');
        if(tmpNode == NULL && tmpNode2 == NULL){
            printf("no directory of file.\n");
            return -1;
        }
        else if(tmpNode != NULL && tmpNode2 == NULL){
            printf("%s is a directory.\n", str);
            return -1;
        }
        else{
        Concatenate(dirTree, str, 3);
        }
    }
    else{
        tmpNode = IsExist(dirTree, cmd, 'd');
        tmpNode2 = IsExist(dirTree, cmd, 'f');
        if(tmpNode == NULL && tmpNode2 == NULL){
            printf("no directory of file.\n");
            return -1;
        }
        else if(tmpNode != NULL && tmpNode2 == NULL){
            printf("%s is a directory.\n", cmd);
            return -1;
        }
        else{
        Concatenate(dirTree, cmd, 1);
        }
    }
    return 1;
}

int chmod(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    int tmp;

    if(strcmp(cmd, "-R") == 0){
        str = strtok(NULL, " ");
        if(str[0]-'0'<8 && str[1]-'0'<8 && str[2]-'0'<8 && strlen(str)==3){
            tmp = atoi(str);
            str = strtok(NULL, " ");
            tmpNode = IsExist(dirTree, str, 'd');
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

void Instruction(DirectoryTree* dirTree, char* cmd)
{
    char* str;
    int val;
    if(strcmp(cmd, "") == 0 || cmd[0] == ' '){
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
        if(val == 0){
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
    return;
}
