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

void ls(DirectoryTree* dirTree, char* cmd)
{
    if(strcmp(cmd, "-al") == 0){
        ListDir(dirTree, 1, 1);
    }
    else if(strcmp(cmd, "-l") == 0){
        ListDir(dirTree, 0, 1);
    }
    else if(strcmp(cmd, "-a") == 0){
        ListDir(dirTree, 1, 0);
    }
    else{
        ListDir(dirTree, 0, 0);
    }
}

int main()
{
    DirectoryTree* Linux = InitializeTree();
    Stack* dStack = InitializeStack();


    /**
            /(root) - user - Documents( - aaa), Downloads

    **/
    mkdir(Linux, "user");
    cd(Linux, "user");


    mkdir(Linux, "Documents");
    mkdir(Linux, "Downloads");

    cd(Linux, "/user/Documents");


    mkdir(Linux, "aaa");
    cd(Linux, "aaa");


    cd(Linux, "..");
    cd(Linux, "..");
    mkdir(Linux, ".abc");



    cd(Linux, "/user/Documents");

    printf("cd aaa\n");
    cd(Linux, "aaa");

    printf("pwd\n");
    PrintPath(Linux, dStack);

    printf("cd ..\n");
    cd(Linux, "..");

    printf("cd ..\n");
    cd(Linux, "..");

    printf("ls -l\n");
    ls(Linux, "");

    rm(Linux, "Documents");
    ls(Linux, "");
    rm(Linux, "Documents");
    ls(Linux, "");

    return 0;
}
