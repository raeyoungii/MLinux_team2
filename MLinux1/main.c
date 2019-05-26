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
    if(usrList->current == usrList->head)
            usr = '#';
        else
            usr = '$';

        printf("This is OS-Test by RaeYoung Kim\n");
        printf("//--version 10.4.3--//\n");
        printf("[%s@os-Virtualbox %s] %c ",usrList->current->name, Linux->current->name, usr);

    while(1){
        gets(cmd);
        if(cmd[0] == '\0')
            continue;
        if(strcmp(cmd, "exit") == 0){
            printf("\nThank you for using OS-Test\n");
            break;
        }
        Instruction(Linux, cmd);

        if(usrList->current == usrList->head)
            usr = '#';
        else
            usr = '$';
        printf("[%s@os-Virtualbox %s] %c ",usrList->current->name, Linux->current->name, usr);
    }

    return 0;
}
