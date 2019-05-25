#include <string.h>
#include <time.h>
#ifndef MAIN_H_
#define MAIN_H_

#define MAX_BUFFER 512
#define MAX_LENGTH 200
#define MAX_DIR 50
#define MAX_NAME 20

//User
typedef struct tagUserNode{
    char name[MAX_NAME];
    char dir[MAX_DIR];
    int UID;
    int GID;
    int month;
    int day;
    int hour;
    int minute;
    struct tagUserNode* LinkNode;
}UserNode;

typedef struct tagUser{
    int topUID;
    int topGID;
    UserNode* head;
    UserNode* tail;
    UserNode* current;
}UserList;


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
	DirectoryNode* root;
	DirectoryNode* current;
}DirectoryTree;


//stack using linked list
typedef struct tagStackNode{
	char name[MAX_NAME];
	struct tagStackNode* LinkNode;
}StackNode;
//change to tagPathNode, PathNode;

typedef struct tagStack{
	StackNode* TopNode;
	int cnt;
}Stack;
//change to tagPathStack, PathStack;

//time
time_t ltime;
struct tm *today;


//utility
int Mode2Permission(DirectoryNode* dirNode);
void PrintPermission(DirectoryNode* dirNode);
void DestroyNode(DirectoryNode* dirNode);
void DestroyDir(DirectoryNode* dirNode);
DirectoryNode* IsExist(DirectoryTree* dirTree, char* dirName);

//save & load
void WriteNode(DirectoryTree* dirTree, DirectoryNode* dirNode, Stack* dirStack);
void SaveDir(DirectoryTree* dirTree, Stack* dirStack);
int ReadNode(DirectoryTree* dirTree, char* tmp);
DirectoryTree* LoadDir();

//mkdir
DirectoryTree* InitializeTree();
int MakeDir(DirectoryTree* dirTree, char* dirName, int type);
//rm
int RemoveDir(DirectoryTree* dirTree, char* dirName);
//cd
int Movecurrent(DirectoryTree* dirTree, char* dirPath);
int MovePath(DirectoryTree* dirTree, char* dirPath);
//pwd
int PrintPath(DirectoryTree* dirTree, Stack* dirStack);
//ls
int ListDir(DirectoryTree* dirTree, int a, int l);
//cat
int Concatenate(DirectoryTree* dirTree, char* fName, int o);
//chmod
int ChangeMode(DirectoryTree* dirTree, int mode, char* dirName);
void ChangeModeAll(DirectoryNode* dirNode, int mode);

//stack
int IsEmpty(Stack* dirStack);
Stack* InitializeStack();
int Push(Stack* dirStack, char* dirName);
char* Pop(Stack* dirStack);


//time
void GetMonth(int i);

//Instruction
int mkdir(DirectoryTree* dirTree, char* cmd);
int rm(DirectoryTree* dirTree, char* cmd);
int cd(DirectoryTree* dirTree, char* cmd);
int pwd(DirectoryTree* dirTree, Stack* dirStack);
int ls(DirectoryTree* dirTree, char* cmd);
int cat(DirectoryTree* dirTree, char* cmd);
int chmod(DirectoryTree* dirTree, char* cmd);
void Instruction(DirectoryTree* dirTree, Stack* dirStack, char* cmd);

//user
UserList* InitializeUser();
int AddUser(UserList* userList, char* userName);
void WriteUser(UserList* userList, UserNode* userNode);
void SaveUserList(UserList* userList);
int ReadUser(UserList* userList, char* tmp);
UserList* LoadUserList();


DirectoryTree* Linux;
Stack* dStack;
UserList* usrList;
FILE* Dir;
FILE* User;
#endif // MAIN_H_
