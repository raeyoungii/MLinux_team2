#include <stdio.h>
#include <stdlib.h>
#include "main.h"

//command
int mkdir(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    char tmp[MAX_NAME];
    int val;

    if(cmd == NULL){
        printf("mkdir: 잘못된 연산자\n");
        return -1;
    }

    //if cmd == aaa bbb : make two folders
    //if exist: return -1;

    tmpNode = dirTree->current;
    if(strcmp(cmd, "-p") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("mkdir: 잘못된 연산자\n");
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
    else{
        str = strtok(NULL, " ");
        if(str == NULL){
            if(strncmp(cmd, "/",1) == 0)
                dirTree->current = dirTree->root;

            if(strstr(cmd, "/") == NULL){
                MakeDir(dirTree, cmd, 'd');
                return 0;
            }
            str = strtok(cmd, "/");
            while(str != NULL){
                val = Movecurrent(dirTree, str);
                if(val != 0){
                    strncpy(tmp, str, MAX_NAME);
                    str = strtok(NULL, "/");
                    if(str == NULL){
                        MakeDir(dirTree, tmp, 'd');
                        dirTree->current = tmpNode;
                        return 0;
                    }
                    else{
                        printf("mkdir: 그런 파일이나 디렉터리가 없습니다\n");
                        return -1;
                    }
                }
                str = strtok(NULL, "/");
            }
        }
        else{
            //fork;
        }
    }
    return 0;
}
int rm(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    char yn[MAX_LENGTH];

    if(cmd == NULL){
        printf("rm: 잘못된 연산자\n");
        return -1;
    }

    if(strcmp(cmd, "-r") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("rm: 잘못된 연산자\n");
            return -1;
        }
        tmpNode = IsExistDir(dirTree, str, 'd');

        if(tmpNode == NULL){
            printf("rm: '%s'를 지울수없음: 그런 파일이나 디렉터리가 없습니다\n", str);
            return -1;
        }
        else{
            printf("이 디렉터리를 지우겠습니까?[y/n] ");
            while(1){
            scanf("%s", yn);

                if(strcmp(yn, "y") == 0){
                    RemoveDir(dirTree, str);
                    return 0;
                }
                else if(strcmp(yn, "n") == 0)
                    return -1;
                else{
                    printf("다시 한번 입력해주십시오[y/n] ");
                }
            }
        }
    }
    //need fix
    else if(strcmp(cmd, "-f") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("rm: 잘못된 연산자\n");
            return -1;
        }
        tmpNode = IsExistDir(dirTree, str, 'f');
        tmpNode2 = IsExistDir(dirTree, str, 'd');

        if(tmpNode2 != NULL){
            printf("rm:'%s'를 지울 수 없음: 디렉터리입니다\n", str);
            return -1;
        }
        if(tmpNode == NULL){
            printf("rm: '%s'를 지울수없음: 그런 파일이나 디렉터리가 없습니다\n", str);
            return -1;
        }
        else{
            RemoveDir(dirTree, str);
        }
    }
    else if(strcmp(cmd, "-rf") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("rm: 잘못된 연산자\n");
            return -1;
        }
        tmpNode = IsExistDir(dirTree, str, 'd');

        if(tmpNode == NULL){
            printf("rm: '%s'를 지울수없음: 그런 파일이나 디렉터리가 없습니다\n", str);
            return -1;
        }
        else{
            RemoveDir(dirTree, str);
        }
    }
    else{
        tmpNode = IsExistDir(dirTree, cmd, 'f');
        tmpNode2 = IsExistDir(dirTree, cmd, 'd');

        if(tmpNode2 != NULL){
            printf("rm:'%s'를 지울 수 없음: 디렉터리입니다\n", cmd);
            return -1;
        }
        if(tmpNode == NULL){
            printf("rm: '%s'를 지울수없음: 그런 파일이나 디렉터리가 없습니다\n", cmd);
            return -1;
        }
        else{
            printf("이 파일을 지우겠습니까?[y/n] ");
            while(1){
            scanf("%s", yn);
                if(strcmp(yn, "y") == 0){
                    RemoveDir(dirTree, cmd);
                    return 0;
                }
                else if(strcmp(yn, "n") == 0)
                    return -1;
                else{
                    printf("다시 한번 입력해주십시오[y/n] ");
                }
            }
        }
    }

    return 0;
}
int cd(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char tmp[MAX_DIR];
    int val;

    if(cmd == NULL){
        strcpy(tmp, usrList->current->dir);
        MovePath(dirTree, tmp);
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
            printf("-bash: cd: '%s': 그런 파일이나 디렉터리가 없습니다.\n", cmd);
    }
    return 0;
}

int pwd(DirectoryTree* dirTree, Stack* dirStack)
{
    PrintPath(dirTree, dirStack);
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
        else{
                printf("ls: 잘못된 연산자\n");
            return -1;
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
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    /**
        cat0: write, EOF to save
        cat1: read
        cat2: read w/ line number
    **/

    if(cmd == NULL){
        printf("cat: 잘못된 연산자\n");
        return -1;
    }

    if(strcmp(cmd, ">") == 0){
        if(HasPermission(dirTree->current, 'w') != 0){
            printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
            return -1;
        }
        str = strtok(NULL, " ");
        tmpNode = IsExistDir(dirTree, str, 'd');
        if(tmpNode != NULL){
            printf("cat: '%s': 디렉터리입니다\n", str);
            return -1;
        }
        else{
        Concatenate(dirTree, str, 0);
        }
        return 0;
    }
    else if(strcmp(cmd, "-n")== 0){
        str = strtok(NULL, " ");
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
    else if(strcmp(cmd, "-b")== 0){
        str = strtok(NULL, " ");
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
            printf("ls: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
            return -1;
        }
        else{
        Concatenate(dirTree, str, 3);
        }
    }
    else{
        if(strcmp(cmd, "/etc/passwd") == 0){
            Concatenate(dirTree, cmd, 4);
            return 0;
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
    return 1;
}

int chmod(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    char* str;
    int tmp;

    if(cmd == NULL){
        printf("chmod: 잘못된 연산자\n");
        return -1;
    }

    if(strcmp(cmd, "-R") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("chmod: 잘못된 연산자\n");
            return -1;
        }
        if(str[0]-'0'<8 && str[1]-'0'<8 && str[2]-'0'<8 && strlen(str)==3){
            tmp = atoi(str);
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("rm: 잘못된 연산자\n");
                return -1;
            }
            tmpNode = IsExistDir(dirTree, str, 'd');
            if(tmpNode != NULL){
                ChangeMode(dirTree, tmp, str);
                ChangeModeAll(tmpNode->LeftChild, tmp);
            }
            else{
                printf("chmod: '%s' 그런 파일이나 디렉터리가 없습니다\n", str);
                return -1;
            }
        }
        else{
            printf("chmod: 잘못된 모드: '%s'\n", str);
            return -1;
        }
    }
    else{
        if(cmd[0]-'0'<8 && cmd[1]-'0'<8 && cmd[2]-'0'<8 && strlen(cmd)==3){
            tmp = atoi(cmd);
            str = strtok(NULL, " ");
            if(str == NULL){
                printf("rm: 잘못된 연산자\n");
                return -1;
            }
            ChangeMode(dirTree, tmp, str);
        }
        else{
            printf("chmod: 잘못된 모드: '%s'\n", cmd);
            return -1;
        }
    }
    return 0;
}

int chown(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* tmpNode = NULL;
    UserNode* tmpUser = NULL;
    char* str;
    char tmp[MAX_NAME];

    if(cmd == NULL){
        printf("chown: 잘못된 연산자\n");
        return -1;
    }
    if(strcmp(cmd, "-R") == 0){
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("chmod: 잘못된 연산자\n");
            return -1;
        }
        tmpUser = IsExistUser(usrList, str);
        if(tmpUser != NULL){
            strncpy(tmp, str, MAX_NAME);
        }
        else{
            printf("chown: '%s' 유저가 존재하지 않습니다\n", str);
            return -1;
        }
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("chown: 잘못된 연산자\n");
            return -1;
        }
        tmpNode = IsExistDir(dirTree, str, 'd');
        if(tmpNode != NULL){
            ChangeOwner(dirTree, tmp, str);
            ChangeOwnerAll(tmpNode->LeftChild, tmp);
        }
        else{
            printf("chown: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
            return -1;
        }
    }
    else{
        strncpy(tmp, cmd, MAX_NAME);
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("chown: 잘못된 연산자\n");
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
            FindDir(dirTree, str, 0);
        }
        else{
            printf("find: 잘못된 연산자\n");
            return -1;
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
        pwd(dirTree, dStack);
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
        val = chown(dirTree, str);
        if(val == 0){
            SaveDir(dirTree, dStack);
        }
    }
    else if(strcmp(str, "find") == 0){
        str = strtok(NULL, " ");
        find_(dirTree, str);

    }else if(strcmp(str, "clear") == 0){
        system("cls");
    }
    else if(strcmp(cmd, "exit") == 0){
            printf("로그아웃");
            exit(0);
        }
    else{
        printf("잘못된 명령어 입니다\n");
    }
    return;
}

void getPath2(DirectoryTree* dirTree, Stack* dirStack)
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

    printf("[%s@os-Virtualbox ",usrList->current->name);

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

    if(strcmp(usrList->current->dir, tmp2) != 0){
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
        printf("~");
        while(IsEmpty(dirStack) == 0){
            printf("/");
            printf("%s",Pop(dirStack));
        }
    }
    printf("] %c ", usr);
}
