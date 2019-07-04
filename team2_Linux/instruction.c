#include <stdio.h>
#include <stdlib.h>
#include "main.h"

//command
int mkdir(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;
    int tmpMode;
    if(cmd == NULL){
        printf("mkdir: 잘못된 연산자\n");
        printf("Try 'mkdir --help' for more information.\n");
        return -1;
    }

    tmpNode = dirTree->current;
    if(cmd[0] == '-'){
        if(strcmp(cmd, "-p") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("mkdir: 잘못된 연산자\n");
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
            if(strncmp(str, "/", 1) == 0){
                dirTree->current = dirTree->root;
            }
            str = strtok(str, "/");
            while(str != NULL){
                val = Movecurrent(dirTree, str);
                if(val != 0){
                    MakeDir(dirTree, str, 'd');
                    Movecurrent(dirTree, str);
                }
                str = strtok(NULL, "/");
            }
            dirTree->current = tmpNode;
        }
        else if(strcmp(cmd, "-m") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("mkdir: 잘못된 연산자\n");
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
            if(str[0]-'0'<8 && str[1]-'0'<8 && str[2]-'0'<8 && strlen(str)==3){
                tmpMode = atoi(str);
                str = strtok(NULL, " ");
                if(str == NULL){
                    printf("mkdir: 잘못된 연산자\n");
                    printf("Try 'mkdir --help' for more information.\n");
                    return -1;
                }
                val = MakeDir(dirTree, str, 'd');
                if(val == 0){
                    tmpNode = IsExistDir(dirTree, str, 'd');
                    tmpNode->mode = tmpMode;
                    Mode2Permission(tmpNode);
                }
            }
            else{
                printf("mkdir: 잘못된 모드: '%s'\n", str);
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
        }
        else if(strcmp(cmd, "--help") == 0){
            printf("사용법: mkdir [옵션]... 디렉터리...\n");
            printf("  Create the DIRECTORY(ies), if they do not already exists.\n\n");
            printf("  Options:\n");
            printf("    -m, --mode=MODE\t set file mode (as in chmod)\n");
            printf("    -p, --parents  \t no error if existing, make parent directories as needed\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("mkdir: 잘못된 연산자\n");
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
            else{
                printf("mkdir: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
        }
    }
    else{
        str = strtok(NULL, " ");
        if(str == NULL){
            strncpy(tmp, cmd, MAX_DIR);
            if(strstr(cmd, "/") == NULL){
                MakeDir(dirTree, cmd, 'd');
                return 0;
            }
            else{
                strncpy(tmp2, getDir(cmd), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if(val != 0){
                    printf("mkdir: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while(str != NULL){
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                MakeDir(dirTree, tmp3 , 'd');
                dirTree->current = tmpNode;
            }
        }
        else{
            DirectoryNode* NewNode = (DirectoryNode*)malloc(sizeof(DirectoryNode));
            DirectoryNode* tmpNode2 = NULL;
            int fd[2];
            int state = 0;
            pid_t pid;

            state = pipe(fd);
            if(state == -1){
                printf("pipe() error\n");
                return -1;
            }

            state = sem_init(&semp, 1, 1);
            write(fd[0], NewNode, sizeof(DirectoryNode));
            pid = fork();
            if(pid == 0){
                sleep(0.1);
                sem_wait(&semp);
                read(fd[1], NewNode, sizeof(DirectoryNode));
                //get time
                time(&ltime);
                today = localtime(&ltime);

                //initialize NewNode
                NewNode->LeftChild = NULL;
                NewNode->RightSibling = NULL;

                //set NewNode
                strncpy(NewNode->name, str, MAX_NAME);
                if(str[0] == '.'){
                    NewNode->type = 'd';
                    //rwx------
                    NewNode->mode = 700;
                    NewNode->SIZE = 4096;
                }
                else{
                    NewNode->type = 'd';
                    //rwxr-xr-x
                    NewNode->mode = 755;
                    NewNode->SIZE = 4096;
                }
                Mode2Permission(NewNode);
                NewNode->UID = usrList->current->UID;
                NewNode->GID = usrList->current->GID;
                NewNode->month = today->tm_mon + 1;
                NewNode->day = today->tm_mday;
                NewNode->hour = today->tm_hour;
                NewNode->minute = today->tm_min;
                NewNode->Parent = NULL;

                write(fd[1], NewNode, sizeof(DirectoryNode));
                sem_post(&semp);
                exit(0);
            }
            else{
                MakeDir(dirTree, cmd, 'd');
                sleep(0.1);
                sem_wait(&semp);
                read(fd[0], NewNode, sizeof(DirectoryNode));
                if(HasPermission(dirTree->current, 'w') != 0){
                    printf("mkdir: '%s' 디렉터리를 만들 수 없습니다: 허가 거부\n", NewNode->name);
                    free(NewNode);
                    return -1;
                }
                if(strcmp(NewNode->name, ".") == 0 || strcmp(NewNode->name, "..") == 0){
                    printf("mkdir: '%s' 디렉터리를 만들 수 없습니다\n", NewNode->name);
                    free(NewNode);
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, NewNode->name, 'd');
                if(tmpNode != NULL && tmpNode->type == 'd'){
                    printf("mkdir: '%s' 디렉터리를 만들 수 없습니다: 파일이 존재합니다\n", NewNode->name);
                    free(NewNode);
                    return -1;
                }

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

                sem_post(&semp);
            }
            sem_destroy(&semp);
        }
    }

    return 0;
}

int rm(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* currentNode = NULL;
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;

    if(cmd == NULL){
        printf("rm: 잘못된 연산자\n");
        printf("Try 'rm --help' for more information.\n");
        return -1;
    }
    currentNode = dirTree->current;
    if(cmd[0] == '-'){
        if(strcmp(cmd, "-r") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("rm: 잘못된 연산자\n");
                printf("Try 'rm --help' for more information.\n");

                return -1;
            }
            strncpy(tmp, str, MAX_DIR);
            if(strstr(str, "/") == NULL){
                tmpNode = IsExistDir(dirTree, str, 'd');
                if(tmpNode == NULL){
                    printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                }
                else{
                    if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                        printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", str);
                        return -1;
                    }
                    RemoveDir(dirTree, str);
                }
            }
            else{
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if(val != 0){
                    printf("rm: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while(str != NULL){
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                if(tmpNode == NULL){
                    printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else{
                    if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                        printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", tmp3);
                        dirTree->current = currentNode;
                        return -1;
                    }
                    RemoveDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        }
        else if(strcmp(cmd, "-f") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                return -1;
            }
            strncpy(tmp, str, MAX_DIR);
            if(strstr(str, "/") == NULL){
                tmpNode = IsExistDir(dirTree, str, 'f');
                tmpNode2 = IsExistDir(dirTree, str, 'd');

                if(tmpNode2 != NULL){
                    return -1;
                }
                if(tmpNode == NULL){
                    return -1;
                }
                else{
                    if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                        return -1;
                    }
                    RemoveDir(dirTree, str);
                }
            }
            else{
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if(val != 0){
                    return -1;
                }
                str = strtok(tmp, "/");
                while(str != NULL){
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'f');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'd');

                if(tmpNode2 != NULL){
                    dirTree->current = currentNode;
                    return -1;
                }
                if(tmpNode == NULL){
                    dirTree->current = currentNode;
                    return -1;
                }
                else{
                    if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                        dirTree->current = currentNode;
                        return -1;
                    }
                    RemoveDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        }
        else if(strcmp(cmd, "-rf") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                return -1;
            }
            strncpy(tmp, str, MAX_DIR);
            if(strstr(str, "/") == NULL){
                tmpNode = IsExistDir(dirTree, str, 'd');
                if(tmpNode == NULL){
                    return -1;
                }
                else{
                    if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                        return -1;
                    }
                    RemoveDir(dirTree, str);
                }
            }
            else{
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if(val != 0){
                    return -1;
                }
                str = strtok(tmp, "/");
                while(str != NULL){
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                if(tmpNode == NULL){
                    dirTree->current = currentNode;
                    return -1;
                }
                else{
                    if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                        dirTree->current = currentNode;
                        return -1;
                    }
                    RemoveDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        }
        else if(strcmp(cmd, "--help") == 0){
            printf("사용법: rm [<옵션>]... [<파일>]...\n");
            printf("  Remove (unlink) the FILE(s).\n\n");
            printf("  Options:\n");
            printf("    -f, --force    \t ignore nonexistent files and arguments, never prompt\n");
            printf("    -r, --recursive\t remove directories and their contents recursively\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("rm: 잘못된 연산자\n");
                printf("Try 'rm --help' for more information.\n");
                return -1;
            }
            else{
                printf("rm: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'rm --help' for more information.\n");
                return -1;
            }
        }
    }
    else{
        strncpy(tmp, cmd, MAX_DIR);
        if(strstr(cmd, "/") == NULL){
            tmpNode = IsExistDir(dirTree, cmd, 'f');
            tmpNode2 = IsExistDir(dirTree, cmd, 'd');

            if(tmpNode2 != NULL){
                printf("rm:'%s'를 지울 수 없음: 디렉터리입니다\n", cmd);
                return -1;
            }
            if(tmpNode == NULL){
                printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", cmd);
                return -1;
            }
            else{
                if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                    printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", cmd);
                    return -1;
                }
                RemoveDir(dirTree, cmd);
            }
        }
        else{
            strncpy(tmp2, getDir(cmd), MAX_DIR);
            val = MovePath(dirTree, tmp2);
            if(val != 0){
                printf("rm: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                return -1;
            }
            str = strtok(tmp, "/");
            while(str != NULL){
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'f');
            tmpNode2 = IsExistDir(dirTree, tmp3, 'd');

            if(tmpNode2 != NULL){
                printf("rm:'%s'를 지울 수 없음: 디렉터리입니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            }
            if(tmpNode == NULL){
                printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            }
            else{
                if(HasPermission(dirTree->current, 'w') != 0 || HasPermission(tmpNode, 'w') != 0){
                    printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                RemoveDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    }

    return 0;
}

int cd(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str = NULL;
    char tmp[MAX_DIR];
    int val;

    if(cmd == NULL){
        strcpy(tmp, usrList->current->dir);
        MovePath(dirTree, tmp);
    }
    else if(cmd[0] == '-'){
        if(strcmp(cmd, "--help") == 0){
            printf("사용법: cd 디렉터리...\n");
            printf("  Change the shell working directory.\n\n");
            printf("  Options:\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("cd: 잘못된 연산자\n");
                printf("Try 'cd --help' for more information.\n");
                return -1;
            }
            else{
            printf("cd: 부적절한 옵션 -- '%s'\n", str);
            printf("Try 'cd --help' for more information.\n");

            return -1;
            }
        }
    }
    else{
        tmpNode = IsExistDir(dirTree, cmd, 'd');
        if(tmpNode != NULL){
            if(HasPermission(tmpNode, 'r') != 0){
                printf("-bash: cd: '%s': 허가거부\n", cmd);
                return -1;
            }
        }
        tmpNode = IsExistDir(dirTree, cmd,  'f');
        if(tmpNode != NULL){
            printf("-bash: cd: '%s': 디렉터리가 아닙니다\n", cmd);
            return -1;
        }
        val = MovePath(dirTree, cmd);
        if(val != 0)
            printf("-bash: cd: '%s': 그런 파일이나 디렉터리가 없습니다\n", cmd);
    }
    return 0;
}

int pwd(DirectoryTree* dirTree, Stack* dirStack, char* cmd)
{
    char* str = NULL;
    if(cmd == NULL){
        PrintPath(dirTree, dirStack);
    }
    else if(cmd[0] == '-'){
        if(strcmp(cmd, "--help") == 0){
            printf("사용법: pwd\n");
            printf("  Print the name of the current working directory.\n\n");
            printf("  Options:\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("pwd: 잘못된 연산자\n");
                printf("Try 'cd --help' for more information.\n");
                return -1;
            }
            else{
            printf("pwd: 부적절한 옵션 -- '%s'\n", str);
            printf("Try 'pwd --help' for more information.\n");
            return -1;
            }
        }
    }
    
    return 0;
}

int ls(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    int val;

    if(cmd == NULL){
        ListDir(dirTree, 0, 0);
        return 0;
    }

    if(cmd[0] == '-'){
        if(strcmp(cmd, "-al") == 0 || strcmp(cmd, "-la") == 0){
            str = strtok(NULL, " ");
            if(str != NULL){
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if(val != 0)
                    return -1;
            }
            ListDir(dirTree, 1, 1);
        }
        else if(strcmp(cmd, "-l") == 0){
            str = strtok(NULL, " ");
            if(str != NULL){
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if(val != 0)
                    return -1;
            }
            ListDir(dirTree, 0, 1);
        }
        else if(strcmp(cmd, "-a") == 0){
            str = strtok(NULL, " ");
            if(str != NULL){
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if(val != 0)
                    return -1;
            }
            ListDir(dirTree, 1, 0);
        }
        else if(strcmp(cmd, "--help") == 0){
            printf("사용법: ls [<옵션>]... [<파일>]...\n");
            printf("  List information about the FILEs (the current directory by default).\n\n");
            printf("  Options:\n");
            printf("    -a, --all\t do not ignore entries starting with .\n");
            printf("    -l       \t use a long listing format\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("ls: 잘못된 연산자\n");
                printf("Try 'ls --help' for more information.\n");
                return -1;
            }
            else{
            printf("ls: 부적절한 옵션 -- '%s'\n", str);
            printf("Try 'ls --help' for more information.\n");
            return -1;
            }
        }
    }
    else{
        tmpNode = dirTree->current;
        val = MovePath(dirTree, cmd);
        if(val != 0)
            return -1;
        ListDir(dirTree, 0, 0);
        dirTree->current = tmpNode;
    }

    if(str != NULL)
        dirTree->current = tmpNode;

    return 0;
}

int cat(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* currentNode = NULL;
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;

    /**
        cat0: write, EOF to save
        cat1: read
        cat2: read w/ line number
    **/

    if(cmd == NULL){
        printf("cat: 잘못된 연산자\n");
        return -1;
    }
    currentNode = dirTree->current;

    if(strcmp(cmd, ">") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("cat: 잘못된 연산자\n");
            printf("Try 'cat --help' for more information.\n");
            return -1;
        }
        strncpy(tmp, str, MAX_DIR);
        if(strstr(str, "/") == NULL){
            if(HasPermission(dirTree->current, 'w') != 0){
                printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                return -1;
            }
            tmpNode = IsExistDir(dirTree, str, 'd');
            if(tmpNode != NULL){
                printf("cat: '%s': 디렉터리입니다\n", str);
                return -1;
            }
            else{
            Concatenate(dirTree, str, 0);
            }
        }
        else{
            strncpy(tmp2, getDir(str), MAX_DIR);
            val = MovePath(dirTree, tmp2);
            if(val != 0){
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                return -1;
            }
            str = strtok(tmp, "/");
            while(str != NULL){
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            if(HasPermission(dirTree->current, 'w') != 0){
                printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                dirTree->current = currentNode;
                return -1;
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'd');
            if(tmpNode != NULL){
                printf("cat: '%s': 디렉터리입니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            }
            else{
            Concatenate(dirTree, tmp3, 0);
            }
            dirTree->current = currentNode;
        }
        return 0;
    }
    else if(cmd[0] == '-'){
        if(strcmp(cmd, "-n")== 0){
            str = strtok(NULL, " ");
            strncpy(tmp, str, MAX_DIR);
            if(strstr(str, "/") == NULL){
                if(HasPermission(dirTree->current, 'w') != 0){
                    printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                tmpNode2 = IsExistDir(dirTree, str, 'f');
    
                if(tmpNode == NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                }
                else if(tmpNode != NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 디렉터리입니다\n", str);
                    return -1;
                }
                else if(tmpNode2 != NULL && HasPermission(tmpNode2, 'r') != 0){
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    return -1;
                }
                else{
                Concatenate(dirTree, str, 2);
                }
            }
            else{
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if(val != 0){
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while(str != NULL){
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
    
                if(tmpNode == NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if(tmpNode != NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 디렉터리입니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if(tmpNode2 != NULL && HasPermission(tmpNode2, 'r') != 0){
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    dirTree->current = currentNode;
                    return -1;
                }
                else{
                Concatenate(dirTree, tmp3, 2);
                }
                dirTree->current = currentNode;
            }
        }
        else if(strcmp(cmd, "-b")== 0){
            str = strtok(NULL, " ");
            strncpy(tmp, str, MAX_DIR);
            if(strstr(str, "/") == NULL){
                if(HasPermission(dirTree->current, 'w') != 0){
                    printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                tmpNode2 = IsExistDir(dirTree, str, 'f');
                if(tmpNode == NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                }
                else if(tmpNode != NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 디렉터리입니다\n", str);
                    return -1;
                }
                else if(tmpNode2 != NULL && HasPermission(tmpNode2, 'r') != 0){
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    return -1;
                }
                else{
                Concatenate(dirTree, str, 3);
                }
            }
            else{
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if(val != 0){
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while(str != NULL){
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
                if(tmpNode == NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if(tmpNode != NULL && tmpNode2 == NULL){
                    printf("cat: '%s': 디렉터리입니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if(tmpNode2 != NULL && HasPermission(tmpNode2, 'r') != 0){
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    dirTree->current = currentNode;
                    return -1;
                }
                else{
                Concatenate(dirTree, tmp3, 3);
                }
                dirTree->current = currentNode;
            }
        }
        else if(strcmp(cmd, "--help") == 0){
            printf("사용법: cat [<옵션>]... [<파일>]...\n");
            printf("  FILE(들)을 합쳐서 표준 출력으로 보낸다.\n\n");
            printf("  Options:\n");
            printf("    -n, --number         \t number all output line\n");
            printf("    -b, --number-nonblank\t number nonempty output line\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("cat: 잘못된 연산자\n");
                printf("Try 'cat --help' for more information.\n");
                return -1;
            }
            else{
                printf("cat: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'cat --help' for more information.\n");
                return -1;
            }
        }
    }
    else{
        if(strcmp(cmd, "/etc/passwd") == 0){
            Concatenate(dirTree, cmd, 4);
            return 0;
        }

        strncpy(tmp, cmd, MAX_DIR);
        if(strstr(cmd, "/") == NULL){
            if(HasPermission(dirTree->current, 'w') != 0){
                printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                return -1;
            }
            tmpNode = IsExistDir(dirTree, cmd, 'd');
            tmpNode2 = IsExistDir(dirTree, cmd, 'f');
            if(tmpNode == NULL && tmpNode2 == NULL){
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", cmd);
                return -1;
            }
            else if(tmpNode != NULL && tmpNode2 == NULL){
                printf("cat: '%s': 디렉터리입니다\n", cmd);
                return -1;
            }
            else if(tmpNode2 != NULL && HasPermission(tmpNode2, 'r') != 0){
                printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                return -1;
            }
            else{
            Concatenate(dirTree, cmd, 1);
            }

        }
        else{
            strncpy(tmp2, getDir(cmd), MAX_DIR);
            val = MovePath(dirTree, tmp2);
            if(val != 0){
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                return -1;
            }
            str = strtok(tmp, "/");
            while(str != NULL){
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'd');
            tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
            if(tmpNode == NULL && tmpNode2 == NULL){
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            }
            else if(tmpNode != NULL && tmpNode2 == NULL){
                printf("cat: '%s': 디렉터리입니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            }
            else if(tmpNode2 != NULL && HasPermission(tmpNode2, 'r') != 0){
                printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                dirTree->current = currentNode;
                return -1;
            }
            else{
            Concatenate(dirTree, tmp3, 1);
            }

            dirTree->current = currentNode;
        }
    }
    return 1;
}


int chmod(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    int tmp;

    if(cmd == NULL){
        printf("chmod: 잘못된 연산자\n");
        printf("Try 'chmod --help' for more information.\n");
        return -1;
    }
    if(cmd[0] == '-'){
        if(strcmp(cmd, "-R") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("chmod: 잘못된 연산자\n");
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
            if(str[0]-'0'<8 && str[1]-'0'<8 && str[2]-'0'<8 && strlen(str)==3){
                tmp = atoi(str);
                str = strtok(NULL, " ");
                if(str == NULL){
                    printf("chmod: 잘못된 연산자\n");
                    printf("Try 'chmod --help' for more information.\n");
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                if(tmpNode != NULL){
                    if(tmpNode->LeftChild == NULL)
                        ChangeMode(dirTree, tmp, str);
                    else{
                        ChangeMode(dirTree, tmp, str);
                        ChangeModeAll(tmpNode->LeftChild, tmp);
                    }
                }
                else{
                    printf("chmod: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                }
            }
            else{
                printf("chmod: 잘못된 모드: '%s'\n", str);
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
        }
        else if(strcmp(cmd, "--help") == 0){
            printf("사용법: chmod [옵션]... 8진수-MODE... 디렉터리...\n");
            printf("  Change the mode of each FILE to MODE.\n\n");
            printf("  Options:\n");
            printf("    -R, --recursive\t change files and directories recursively\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("chmod: 잘못된 연산자\n");
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
            else{
                printf("chmod: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
        }
    }
    else{
        if(cmd[0]-'0'<8 && cmd[1]-'0'<8 && cmd[2]-'0'<8 && strlen(cmd)==3){
            tmp = atoi(cmd);
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("chmod: 잘못된 연산자\n");
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
            ChangeMode(dirTree, tmp, str);
        }
        else{
            printf("chmod: 잘못된 모드: '%s'\n", cmd);
            printf("Try 'chmod --help' for more information.\n");
            return -1;
        }
    }
    return 0;
}

int chown_(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    UserNode* tmpUser = NULL;
    char* str;
    char tmp[MAX_NAME];

    if(cmd == NULL){
        printf("chown: 잘못된 연산자\n");
        printf("Try 'chown --help' for more information.\n");
        return -1;
    }
    if(cmd[0] == '-'){
        if(strcmp(cmd, "-R") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("chown: 잘못된 연산자\n");
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
            tmpUser = IsExistUser(usrList, str);
            if(tmpUser != NULL){
                strncpy(tmp, str, MAX_NAME);
            }
            else{
                printf("chown: 잘못된 사용자: '%s'\n", str);
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("chown: 잘못된 연산자\n");
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
            tmpNode = IsExistDir(dirTree, str, 'd');
            if(tmpNode != NULL){
                if(tmpNode->LeftChild == NULL)
                    ChangeOwner(dirTree, tmp, str);
                else{
                    ChangeOwner(dirTree, tmp, str);
                    ChangeOwnerAll(tmpNode->LeftChild, tmp);
                }
            }
            else{
                printf("chown: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                return -1;
            }
        }
        else if(strcmp(cmd, "--help") == 0){
            printf("사용법: chown [옵션]... [소유자]... 파일...\n");
            printf("  Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\n");
            printf("  Options:\n");
            printf("    -R, --recursive\t change files and directories recursively\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("chown: 잘못된 연산자\n");
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
            else{
                printf("chown: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
        }
    }
    else{
        strncpy(tmp, cmd, MAX_NAME);
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("chown: 잘못된 연산자\n");
            printf("Try 'chown --help' for more information.\n");
            return -1;
        }
        else{
            ChangeOwner(dirTree, tmp, str);
        }
    }
    return 0;
}

int find_(DirectoryTree* dirTree, char* cmd)
{
    char* str;
    if(cmd == NULL){
        FindDir(dirTree, dirTree->current->name, 1);
        return 0;
    }
    else if(cmd[0] == '-'){
        if(strcmp(cmd, "-name") == 0){
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("find: 잘못된 연산자\n");
                printf("Try 'find --help' for more information.\n");
                return -1;
            }
            FindDir(dirTree, str, 0);
        }
        else if(strcmp(cmd, "--help") == 0){
            printf("사용법: find [<옵션>]... [<파일>]...\n");
            printf("\n");
            printf("  Options:\n");
            printf("    -name\t finds file by name\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        }
        else{
            str = strtok(cmd, "-");
            if(str == NULL){
                printf("find: 잘못된 연산자\n");
                printf("Try 'find --help' for more information.\n");
                return -1;
            }
            else{
                printf("find: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'find --help' for more information.\n");
                return -1;
            }
        }
    }
    else{
        FindDir(dirTree, cmd, 1);
    }

    return 0;
}

void Instruction(DirectoryTree* dirTree, char* cmd)
{
    char* str;
    int val;
    if(strcmp(cmd, "") == 0 || cmd[0] == ' '){
        return;
    }
    str = strtok(cmd, " ");

    if(strcmp(str, "mkdir") == 0){
        str = strtok(NULL, " ");
        val = mkdir(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "rm") == 0){
        str = strtok(NULL, " ");
        val = rm(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "cd") == 0){
        str = strtok(NULL, " ");
        cd(dirTree, str);
    }
    else if(strcmp(str, "pwd") == 0){
        str = strtok(NULL, " ");
        pwd(dirTree, dStack, str);
    }
    else if(strcmp(str, "ls") == 0){
        str = strtok(NULL, " ");
        ls(dirTree, str);
    }
    else if(strcmp(str, "cat") == 0){
        str = strtok(NULL, " ");
        val = cat(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "chmod") == 0){
        str = strtok(NULL, " ");
        val = chmod(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "chown") == 0){
        str = strtok(NULL, " ");
        val = chown_(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "find") == 0){
        str = strtok(NULL, " ");
        find_(dirTree, str);
    }
    else if(strcmp(cmd, "exit") == 0){
            printf("로그아웃\n");
            exit(0);
    }
    else{
        printf("'%s': 명령을 찾을 수 없습니다\n", cmd);
    }
    return;
}

void PrintStart()
{
    printf("Welcome to Ubuntu 18.04.2 LTS (GNU/Linux 4.18.0-17-generic x86_64)\n\n");
    printf(" * Documentation: https://help.ubuntu.com\n");
    printf(" * Management:    https://landscape.canonial.com\n");
    printf(" * Support:       https://ubuntu.com/advantage\n\n\n");
    printf(" * Canonial Livepatch is available for installation.\n");
    printf("   - Reduce system reboots and improve kernel security. Activate at:\n");
    printf("     https://ubuntu.com/livepatch\n\n");
    printf("Your Hardware Enablement Stack(HWE) is supported until April 2023.\n");
    printf("Last login: ");
    GetWeekday(usrList->current->wday);
    GetMonth(usrList->current->month);
    printf("%d %02d:%02d:%02d %d\n", usrList->current->day, usrList->current->hour, usrList->current->minute, usrList->current->sec, usrList->current->year);
}

void PrintHead(DirectoryTree* dirTree, Stack* dirStack)
{
    //variables
    DirectoryNode* tmpNode = NULL;
    char tmp[MAX_DIR] = "";
    char tmp2[MAX_DIR] = "";
    char usr;

    if(usrList->current == usrList->head)
        usr = '#';
    else
        usr = '$';

    BOLD;GREEN;
    printf("%s@os-Virtualbox", usrList->current->name);
    DEFAULT;
    printf(":");
    tmpNode = dirTree->current;

    if(tmpNode == dirTree->root){
        strcpy(tmp, "/");
    }
    else{
        while(tmpNode->Parent != NULL){
            Push(dirStack, tmpNode->name);
            tmpNode = tmpNode->Parent;
        }
        while(IsEmpty(dirStack) == 0){
                strcat(tmp, "/");
                strcat(tmp ,Pop(dirStack));
        }
    }

    strncpy(tmp2, tmp, strlen(usrList->current->dir));

    if(usrList->current == usrList->head){
        BOLD;BLUE;
        printf("%s", tmp);
    }
    else if(strcmp(usrList->current->dir, tmp2) != 0){
	BOLD;BLUE;
        printf("%s", tmp);
    }
    else{
        tmpNode = dirTree->current;
        while(tmpNode->Parent != NULL){
            Push(dirStack, tmpNode->name);
            tmpNode = tmpNode->Parent;
        }
        Pop(dirStack);
        Pop(dirStack);
	BOLD;BLUE;
        printf("~");
        while(IsEmpty(dirStack) == 0){
            printf("/");
            printf("%s",Pop(dirStack));
        }
    }
    DEFAULT;
    printf("%c ", usr);
}
