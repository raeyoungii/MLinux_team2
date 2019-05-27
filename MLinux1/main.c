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

    Login(usrList, Linux);

    if(usrList->current == usrList->head)
        usr = '#';
    else
        usr = '$';

    printf("Welcome to Team2_OS 12.0.1\n\n");
    printf("Last login: ");
    GetMonth(usrList->current->month);
    printf(" %d %02d:%02d\n", usrList->current->day, usrList->current->hour, usrList->current->minute);
    SaveUserList(usrList);

    printf("[%s@os-Virtualbox %s] %c ",usrList->current->name, Linux->current->name, usr);

    while(1){
        gets(cmd);
        if(cmd[0] == '\0')
            continue;

        Instruction(Linux, cmd);
        printf("[%s@os-Virtualbox %s] %c ",usrList->current->name, Linux->current->name, usr);
    }

    return 0;
}
