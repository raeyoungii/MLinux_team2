#include <string.h>
#include <time.h>
#ifndef MAIN_H_
#define MAIN_H_

#define MAX_DIR 50
#define MAX_NAME 20


//Directory
typedef struct tagDirectoryNode{
    char name[MAX_NAME];
    char type;
    int mode;
    int permission[9];
    int SIZE;
    char UID[MAX_NAME];
	char GID[MAX_NAME];
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


//stack using linked list
typedef struct tagStackNode{
	char name[MAX_NAME];
	struct tagStackNode* LinkNode;
}StackNode;

typedef struct tagStack{
	StackNode* TopNode;
}Stack;


//time
time_t ltime;
struct tm *today;


//utility
void Mode2Permission(DirectoryNode* dirNode);
void PrintPermission(DirectoryNode* dirNode);
void DestroyNode(DirectoryNode* dirNode);
void DestroyDir(DirectoryNode* dirNode);
DirectoryNode* IsExist(DirectoryTree* dirTree, char* dirName);


//cd
int Movecurrent(DirectoryTree* dirTree, char* dirPath);
int MovePath(DirectoryTree* dirTree, char* dirPath);
//mkdir
DirectoryTree* InitializeTree();
int MakeDir(DirectoryTree* dirTree, char* dirName);
//rm
int RemoveDir(DirectoryTree* dirTree, char* dirName);
//pwd
void PrintPath(DirectoryTree* dirTree, Stack* dirStack);
//ls
int ListDir(DirectoryTree* dirTree, int a, int l);


//stack
int IsEmpty(Stack* dirStack);
Stack* InitializeStack();
int Push(Stack* dirStack, char* dirName);
char* Pop(Stack* dirStack);


//time
void GetMonth(int i);


DirectoryTree* Linux;
Stack* dStack;

#endif // MAIN_H_
