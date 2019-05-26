#include <stdio.h>
#include <stdlib.h>
#include "main.h"

UserList* InitializeUser()
{
    UserList* returnList = (UserList*)malloc(sizeof(UserList));
    UserNode* NewNode = (UserNode*)malloc(sizeof(UserNode));

    //get time
    time(&ltime);
    today = localtime(&ltime);

    strncpy(NewNode->name, "root", MAX_NAME);
    strncpy(NewNode->dir, "/", MAX_NAME);
    NewNode->UID = 0;
    NewNode->GID = 0;
    NewNode->month = today->tm_mon+1;
    NewNode->day = today->tm_mday;
    NewNode->hour = today->tm_hour;
    NewNode->minute = today->tm_min;
    NewNode->LinkNode = NULL;

    returnList->head = NewNode;
    returnList->tail = NewNode;
    returnList->current = NewNode;
    returnList->topUID = 0;
    returnList->topGID = 0;

    return returnList;
}

int AddUser(UserList* userList, char* userName)
{
    UserNode* NewNode = (UserNode*)malloc(sizeof(UserNode));
    char* userdir = "/home";

    strcat(userdir, userName);

    if(NewNode == NULL){
        printf("error occurred, NewUser.\n");
        return -1;
    }
    //get time
    time(&ltime);
    today = localtime(&ltime);

    strncpy(NewNode->name, userName, MAX_NAME);
    strncpy(NewNode->dir, userdir, MAX_NAME);
    NewNode->UID = userList->topUID+1;
    NewNode->GID = userList->topGID+1;
    NewNode->month = today->tm_mon+1;
    NewNode->day = today->tm_mday;
    NewNode->hour = today->tm_hour;
    NewNode->minute = today->tm_min;
    NewNode->LinkNode = NULL;
    userList->tail->LinkNode = NewNode;
    userList->tail = NewNode;
    userList->topUID++;
    userList->topGID++;

    return 0;
}

void WriteUser(UserList* userList, UserNode* userNode)
{
    fprintf(User, "%s %s ", userNode->name, userNode->dir);
    fprintf(User, "%d %d %d %d %d %d", userNode->UID, userNode->GID, userNode->month, userNode->day, userNode->hour, userNode->minute);

    if(userNode == userList->head)
        fprintf(User, " /");
    if(userNode == userList->current)
        fprintf(User, " c");
    fprintf(User, "\n");
    if(userNode->LinkNode != NULL){
        WriteUser(userList, userNode->LinkNode);
    }

}

void SaveUserList(UserList* userList)
{
    User = fopen("User.txt", "w");

    WriteUser(userList, userList->head);

    fclose(Dir);
}

int ReadUser(UserList* userList, char* tmp)
{
    UserNode* NewNode = (UserNode*)malloc(sizeof(UserNode));
    char* str;

    NewNode->LinkNode = NULL;

    str = strtok(tmp, " ");
    strncpy(NewNode->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    strncpy(NewNode->dir, str, MAX_DIR);
    str = strtok(NULL, " ");
    NewNode->UID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->GID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->month = atoi(str);
    str = strtok(NULL, " ");
    NewNode->day = atoi(str);
    str = strtok(NULL, " ");
    NewNode->hour = atoi(str);
    str = strtok(NULL, " ");
    NewNode->minute = atoi(str);


    str = strtok(NULL, " ");
    if(str == NULL){
        userList->tail->LinkNode = NewNode;
        userList->tail = NewNode;
    }
    else if(str[0] == '/'){
        userList->head = NewNode;
        userList->tail = NewNode;

        str = strtok(NULL, " ");
        if(str != NULL)
            userList->current = NewNode;
    }
    else if(str[0] == 'c'){
        userList->current = NewNode;
        userList->tail->LinkNode = NewNode;
        userList->tail = NewNode;
    }
    return 0;
}

UserList* LoadUserList()
{
    UserList* userList = (UserList*)malloc(sizeof(UserList));
    char tmp[MAX_LENGTH];

    User = fopen("User.txt", "r");

    while(fgets(tmp, MAX_LENGTH, User) != NULL){
        ReadUser(userList, tmp);
    }

    fclose(User);

    return userList;
}


char* GetUID(DirectoryNode* dirNode)
{
    UserNode* tmpNode = NULL;

    tmpNode = usrList->head;
    while(tmpNode != NULL){
        if(tmpNode->UID == dirNode->UID)
            break;
        tmpNode = tmpNode->LinkNode;
    }
    return tmpNode->name;
}

char* GetGID(DirectoryNode* dirNode)
{
    UserNode* tmpNode = NULL;

    tmpNode = usrList->head;
    while(tmpNode != NULL){
        if(tmpNode->GID == dirNode->GID)
            break;
        tmpNode = tmpNode->LinkNode;
    }
    return tmpNode->name;
}

int HasPermission(DirectoryNode* dirNode, char o)
{
    if(usrList->current->UID == 0)
        return 0;

    if(usrList->current->UID == dirNode->UID){
        if(o == 'r'){
            if(dirNode->permission[0] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'w'){
            if(dirNode->permission[1] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'x'){
            if(dirNode->permission[2] == 0)
                return -1;
            else
                return 0;
        }
    }
    else if(usrList->current->GID == dirNode->GID){
        if(o == 'r'){
            if(dirNode->permission[3] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'w'){
            if(dirNode->permission[4] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'x'){
            if(dirNode->permission[5] == 0)
                return -1;
            else
                return 0;
        }
    }
    else{
        if(o == 'r'){
            if(dirNode->permission[6] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'w'){
            if(dirNode->permission[7] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'x'){
            if(dirNode->permission[8] == 0)
                return -1;
            else
                return 0;
        }
    }
    return -1;
}











