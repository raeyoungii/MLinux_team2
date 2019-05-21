#include <stdio.h>
#include <stdlib.h>
#include "main.h"

//utility
void Mode2Permission(DirectoryNode* dirNode)
{
    char buf[3];
    int tmp;
    int j;

    sprintf(buf, "%d", dirNode->mode);

    for(int i=0;i<3;i++){
        tmp = buf[i] - '0';
        j = 2;

        while(tmp != 0){
        dirNode->permission[3*i+j] = tmp%2;
        tmp/=2;
        j--;
        }
    }
}
void PrintPermission(DirectoryNode* dirNode)
{
    char rwx[3] = "rwx";

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(dirNode->permission[3*i+j] == 1)
                printf("%c", rwx[j]);
            else
                printf("-");
        }
    }
    printf(",");
}

void DestroyNode(DirectoryNode* dirNode)
{
    free(dirNode);
}

void DestroyDir(DirectoryNode* dirNode)
{
    if(dirNode->RightSibling != NULL){
        DestroyDir(dirNode->RightSibling);
    }
    if(dirNode->LeftChild != NULL){
        DestroyDir(dirNode->LeftChild);
    }

    dirNode->LeftChild = NULL;
    dirNode->RightSibling = NULL;

    DestroyNode(dirNode);
}

DirectoryNode* IsExist(DirectoryTree* dirTree, char* dirName)
{
    //variables
    DirectoryNode* returnNode = NULL;

    returnNode = dirTree->current->LeftChild;

    while(returnNode != NULL){
        if(strcmp(returnNode->name, dirName) == 0)
            break;
        returnNode = returnNode->RightSibling;
    }
    return returnNode;
}

//cd
int Movecurrent(DirectoryTree* dirTree, char* dirPath)
{
    if(strcmp(dirPath,".") == 0){
    }
    else if(strcmp(dirPath,"..") == 0){
        if(dirTree->current != dirTree->root){
            dirTree->current = dirTree->current->Parent;
            //strncpy(dirTree->currentname, dirTree->current->name, MAX_NAME);
        }
    }
    else{
        //if input path exist
        if( IsExist(dirTree, dirPath) != NULL){
            dirTree->current = IsExist(dirTree, dirPath);
            //strncpy(dirTree->currentname, dirTree->current->name, MAX_NAME);
        }
        else
            return -1;
    }
    return 0;
}

int MovePath(DirectoryTree* dirTree, char* dirPath)
{
    //variables
    DirectoryNode* tmpNode = NULL;
    char tmpPath[MAX_DIR];
    char* str = NULL;
    int val = 0;
    //set tmp
    strncpy(tmpPath, dirPath, MAX_DIR);
    tmpNode = dirTree->current;
    //if input is root
    if(strcmp(dirPath, "/") == 0){
        dirTree->current = dirTree->root;
        //strncpy(dirTree->currentname, dirTree->current->name, MAX_NAME);
    }
    else{
        //if input is absolute path
        if(strncmp(dirPath, "/",1) == 0){
            dirTree->current = dirTree->root;
        }
        //if input is relative path
        str = strtok(tmpPath, "/");
        while(str != NULL){
            val = Movecurrent(dirTree, str);
            //if input path doesn't exist
            if(val == -1){
                if(IsExist(dirTree, str) == NULL)
                    printf("denied, directory doesn't exist.\n");
                    dirTree->current = tmpNode;
                return -1;
            }
            str = strtok( NULL, "/");
        }
    }
    return 0;
}

//mkdir
DirectoryTree* InitializeTree()
{
    //variables
    DirectoryTree* dirTree = (DirectoryTree*)malloc(sizeof(DirectoryTree));
    DirectoryNode* NewNode = (DirectoryNode*)malloc(sizeof(DirectoryNode));
    //get time
    time(&ltime);
    today = localtime(&ltime);
    //set NewNode
    strncpy(NewNode->name, "/", MAX_NAME);
    //rwxr-xr-x
    NewNode->type ='d';
    NewNode->mode = 755;
    Mode2Permission(NewNode);
    strncpy(NewNode->UID, "root", MAX_NAME);
    strncpy(NewNode->GID, "root", MAX_NAME);
    NewNode->SIZE = 4096;
    NewNode->month = today->tm_mon+1;
    NewNode->day = today->tm_mday;
    NewNode->hour = today->tm_hour;
    NewNode->minute = today->tm_min;
    NewNode->Parent = NULL;
    NewNode->LeftChild = NULL;
    NewNode->RightSibling = NULL;

    //set dirTree
    dirTree->root = NewNode;
    dirTree->current = dirTree->root;
    //strncpy(dirTree->currentname, "/" , MAX_NAME);

    return dirTree;
}

int MakeDir(DirectoryTree* dirTree, char* dirName)
{
    //variables
    DirectoryNode* NewNode = (DirectoryNode*)malloc(sizeof(DirectoryNode));
    DirectoryNode* tmpNode = NULL;

    //get time
    time(&ltime);
    today = localtime(&ltime);

    //initialize NewNode
    NewNode->LeftChild = NULL;
    NewNode->RightSibling = NULL;

    //set NewNode
    strncpy(NewNode->name, dirName, MAX_NAME);
    //rwxr-xr-x
    NewNode->type ='d';
    NewNode->mode = 755;
    Mode2Permission(NewNode);
    strncpy(NewNode->UID, "root", MAX_NAME);
    strncpy(NewNode->GID, "root", MAX_NAME);
    NewNode->SIZE = 4096;
    NewNode->month = today->tm_mon + 1;
    NewNode->day = today->tm_mday;
    NewNode->hour = today->tm_hour;
    NewNode->minute = today->tm_min;
	NewNode->Parent = dirTree->current;

	if(dirTree->current->LeftChild == NULL){
        dirTree->current->LeftChild = NewNode;
	}
	else{
        tmpNode = dirTree->current->LeftChild;

        while(tmpNode->RightSibling!= NULL){
            tmpNode = tmpNode->RightSibling;
        }
        tmpNode->RightSibling = NewNode;
	}

    return 0;
}
//rm
int RemoveDir(DirectoryTree* dirTree, char* dirName)
{
    DirectoryNode* DelNode = NULL;
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* prevNode = NULL;

    tmpNode = dirTree->current->LeftChild;

    if(tmpNode == NULL){
        printf("No directory\n");
        return -1;
    }

    if(strcmp(tmpNode->name, dirName) == 0){
        dirTree->current->LeftChild = tmpNode->RightSibling;
        DelNode = tmpNode;

        DestroyDir(DelNode->LeftChild);
        DestroyNode(DelNode);
    }
    else{
        while(tmpNode != NULL){
            if(strcmp(tmpNode->name, dirName) == 0){
                DelNode = tmpNode;
                break;
            }
            prevNode = tmpNode;
            tmpNode = tmpNode->RightSibling;
        }
        if(DelNode != NULL){
            prevNode->RightSibling = DelNode->RightSibling;

            DestroyDir(DelNode->LeftChild);
            DestroyNode(DelNode);
        }
        else{
            printf("No directory\n");
            return -1;
        }
    }
    return 0;
}


//pwd
void PrintPath(DirectoryTree* dirTree, Stack* dirStack)
{
    //variables
    DirectoryNode* tmpNode = NULL;

    tmpNode = dirTree->current;

    //if current directory is root
    if(tmpNode == dirTree->root){
        printf("/");
    }
    else{
        //until current directory is root, repeat Push
        while(tmpNode->Parent != NULL){
            Push(dirStack, tmpNode->name);
            tmpNode = tmpNode->Parent;
        }
    //until stack is empty, repeat Pop
        while(IsEmpty(dirStack) == 0){
            printf("/");
            printf("%s",Pop(dirStack));
        }
    }
    printf("\n");
}
//ls
int ListDir(DirectoryTree* dirTree, int a, int l)
{
    //variables
    DirectoryNode* tmpNode = NULL;

    tmpNode = dirTree->current->LeftChild;

    if(tmpNode == NULL){
        printf("no files.\n");
        return -1;
    }

    if(l == 0){
        if(a == 1){
            printf(".\t");
            if(dirTree->current != dirTree->root)
                printf("..\t");
        }

        while(tmpNode != NULL){
            if(a == 0){
                if(strncmp(tmpNode->name,".",1) == 0){
                    tmpNode = tmpNode->RightSibling;
                    continue;
                }
            }
            printf("%s\t", tmpNode->name);
            tmpNode = tmpNode->RightSibling;
        }
        printf("\n");
    }
    else{
        if(a == 1){
            printf("%c", dirTree->current->type);
            PrintPermission(dirTree->current);
            printf("  ");
            printf("%-5s%-5s", dirTree->current->UID, dirTree->current->GID);
            printf("%5d ", dirTree->current->SIZE);
            GetMonth(dirTree->current->month);
            printf(" %d %02d:%02d ", dirTree->current->day, dirTree->current->hour, dirTree->current->minute);
            printf(".\n");

            if(dirTree->current != dirTree->root){
                printf("%c", dirTree->current->Parent->type);
                PrintPermission(dirTree->current->Parent);
                printf("  ");
                printf("%-5s%-5s", dirTree->current->Parent->UID, dirTree->current->Parent->GID);
                printf("%5d ", dirTree->current->SIZE);
                GetMonth(dirTree->current->month);
                printf(" %d %02d:%02d ", dirTree->current->Parent->day, dirTree->current->Parent->hour, dirTree->current->Parent->minute);
                printf("..\n");
            }
        }

        while(tmpNode != NULL){
            if(a == 0){
                if(strncmp(tmpNode->name,".",1) == 0){
                    tmpNode = tmpNode->RightSibling;
                    continue;
                }
            }
            printf("%c", tmpNode->type);
            PrintPermission(tmpNode);
            printf("  ");
            printf("%-5s%-5s", tmpNode->UID, tmpNode->GID);
            printf("%5d ", tmpNode->SIZE);
            GetMonth(tmpNode->month);
            printf(" %d %02d:%02d ", tmpNode->day, tmpNode->hour, tmpNode->minute);

            printf("%-15s\n", tmpNode->name);
            tmpNode = tmpNode->RightSibling;
        }
    }
    return 0;
}

