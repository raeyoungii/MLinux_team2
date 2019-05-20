#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

//utility
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
    char tmpPath[MAX_PATH];
    char* str = NULL;
    int val = 0;
    //set tmp
    strncpy(tmpPath, dirPath, MAX_PATH);
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
}

//mkdir
DirectoryTree* InitializeTree()
{
    //variables
    DirectoryTree* dirTree = (DirectoryTree*)malloc(sizeof(DirectoryTree));
    DirectoryNode* NewNode = (DirectoryNode*)malloc(sizeof(DirectoryNode));

    time(&ltime);
    today = localtime(&ltime);
    //set NewNode
    strncpy(NewNode->name, "/", MAX_NAME);
    //rwxr-xr-x
    NewNode->type ='d';
    NewNode->mode = 755;
    Mode2Permission(NewNode);
    strncpy(NewNode->username, "root", MAX_NAME);
    strncpy(NewNode->groupname, "root", MAX_NAME);

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

    //initialize NewNode
    NewNode->LeftChild = NULL;
    NewNode->RightSibling = NULL;

    //set NewNode
    strncpy(NewNode->name, dirName, MAX_NAME);
    //rwxr-xr-x
    NewNode->type ='d';
    NewNode->mode = 755;
    Mode2Permission(NewNode);
    strcpy(NewNode->username,"root");
	strcpy(NewNode->groupname,"root");
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
}

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
    }
    else{
        if(a == 1){
            printf("%c", dirTree->current->type);
            PrintPermission(dirTree->current);
            printf("\t");
            printf(".\t\n");

            if(dirTree->current != dirTree->root){
                printf("%c", dirTree->current->Parent->type);
                PrintPermission(dirTree->current->Parent);
                printf("\t");
                printf("..\t\n");
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
            printf("\t");
            /*
            printf("%c",type);
            printf("%7s %7s     %d¿ù %d³â",tmpNode->username,tmpNode->groupname,tmpNode->month,tmpNode->year);
            */
            printf("%-15s\n", tmpNode->name);
            tmpNode = tmpNode->RightSibling;
        }
    }
}

//rmdir
/*BOOL DeleteDir(DirectoryTree* dirTree, char* dirName, int option)
{
    //variables
    DirectoryNode* DeleteNode = NULL;
    DirectoryNode*
}
*/
