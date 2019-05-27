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

    printf("Welcome to Team2_OS 12.0.1\n\n");
    printf("Last login:");
    GetMonth(usrList->current->month);
    printf(" %d %02d:%02d\n", usrList->current->day, usrList->current->hour, usrList->current->minute);
    printf("[%s@os-Virtualbox %s] %c ",usrList->current->name, Linux->current->name, usr);
    time(&ltime);
    today = localtime(&ltime);
    usrList->current->month = today->tm_mon+1;
    usrList->current->day = today->tm_mday;
    usrList->current->hour = today->tm_hour;
    usrList->current->minute = today->tm_min;

    while(1){
        gets(cmd);
        if(cmd[0] == '\0'){
            continue;
        }
        if(strcmp(cmd, "exit") == 0){
            printf("Logout");
            SaveUserList(usrList);
            break;
        }

        Instruction(Linux, cmd);
        printf("[%s@os-Virtualbox %s] %c ",usrList->current->name, Linux->current->name, usr);
    }

    return 0;
}
