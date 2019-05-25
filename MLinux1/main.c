#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main()
{
    Linux = LoadDir();
    usrList = LoadUserList();
    dStack = InitializeStack();
    char cmd[50];

    while(1){
        printf("[%s@localhost %s] $ ",usrList->current->name, Linux->current->name);
        gets(cmd);
        if(strcmp(cmd, "exit") == 0){
            break;
        }
        Instruction(Linux, dStack, cmd);
    }

    return 0;
}
