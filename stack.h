#include "main.h"
#ifndef STACK_H
#define STACK_H

//stack using linked list
typedef struct tagStackNode{
	char name[MAX_NAME];
	struct tagStackNode* LinkNode;
}StackNode;

typedef struct tagStack{
	//int ptr;
	StackNode* TopNode;
}Stack;

int IsEmpty(Stack* dirStack);
Stack* InitializeStack();
int Push(Stack* dirStack, char* dirName);
char* Pop(Stack* dirStack);

#endif // STACK_H
