#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main()
{
    Linux = LoadDir();
    usrList = LoadUserList();
    dStack = InitializeStack();

    char cmd[50];
    char usr;
    char tmp[MAX_DIR];

    strcpy(tmp, usrList->current->dir);
    MovePath(Linux, tmp);

    while(1){
        if(usrList->current == usrList->head)
            usr = '#';
        else
            usr = '$';
        printf("[%s@os-Virtualbox] %s %c ",usrList->current->name, Linux->current->name, usr);
        gets(cmd);
        if(strcmp(cmd, "exit") == 0){
            break;
        }
        Instruction(Linux, cmd);
    }

    return 0;
}
