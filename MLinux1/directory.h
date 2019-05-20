#include <string.h>
#include <time.h>
#include "main.h"
#ifndef DIRECTORY_H
#define DIRECTORY_H

//Directory
typedef struct tagDirectoryNode{
    char name[MAX_NAME];
    char type;
    int mode;
    int permission[9];
    char username[MAX_NAME];
	char groupname[MAX_NAME];
	int volume;
	int month;
    int day;
    int hour;
    int minute;
	struct tagDirectoryNode* Parent;
	struct tagDirectoryNode* LeftChild;
	struct tagDirectoryNode* RightSibling;
}DirectoryNode;

typedef struct tagDirectoryTree{
	//char currentname[MAX_NAME]; //is it needed?
	//DirectoryNode* tmp;
	DirectoryNode* root;
	DirectoryNode* current;
}DirectoryTree;

//time
time_t ltime;
struct tm *today;

//utility
DirectoryNode* IsExist(DirectoryTree* dirTree, char* dirName);
void Mode2Permission(DirectoryNode* dirNode);
void PrintPermission(DirectoryNode* dirNode);

//cd
int Movecurrent(DirectoryTree* dirTree, char* dirPath);
int MovePath(DirectoryTree* dirTree, char* dirPath);
//mkdir
DirectoryTree* InitializeTree();
int MakeDir(DirectoryTree* dirTree, char* dirName);
//pwd
void PrintPath(DirectoryTree* dirTree, Stack* dirStack);
//ls
int ListDir(DirectoryTree* dirTree, int a, int l);
//rmdir
//BOOL DeleteDir(DirectoryTree* dirTree, char* dirName, int option);

#endif // DIRECTORY_H
