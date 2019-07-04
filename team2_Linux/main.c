#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main()
{
    char cmd[50];

    Linux = LoadDir();
    usrList = LoadUserList();
    dStack = InitializeStack();

    Login(usrList, Linux);
    PrintStart();
    SaveUserList(usrList);

    while(1){
        PrintHead(Linux, dStack);
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = '\0';
        Instruction(Linux, cmd);
    }

    return 0;
}
