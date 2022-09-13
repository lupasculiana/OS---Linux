#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

char* getPerm(char *path)
{
    struct stat statbuf;
    //char *perm;
    //perm = malloc(sizeof(char) * 9 + 1);
    char string[10] = {};
    char *perm = string;
    if(stat(path, &statbuf) == 0)
    {
        mode_t permission = statbuf.st_mode;
        if(permission & S_IRUSR)
        {
            string[0] = '1';
        }
        else
        {
            string[0] = '0';
        }
        if(permission & S_IWUSR)
        {
            string[1] = '1';
        }
        else
        {
            string[1] = '0';
        }
        if(permission & S_IXUSR)
        {
            string[2] = '1';
        }
        else
        {
            string[2] = '0';
        }
        if(permission & S_IRGRP)
        {
            string[3] = '1';
        }
        else
        {
            string[3] = '0';
        }
        if(permission & S_IWGRP)
        {
            string[4] = '1';
        }
        else
        {
            string[4] = '0';
        }
        if(permission & S_IXGRP)
        {
            string[5] = '1';
        }
        else
        {
            string[5] = '0';
        }
        if(permission & S_IROTH)
        {
            string[6] = '1';
        }
        else
        {
            string[6] = '0';
        }
        if(permission & S_IWOTH)
        {
            string[7] = '1';
        }
        else
        {
            string[7] = '0';
        }
        if(permission & S_IXOTH)
        {
            string[8] = '1';
        }
        else
        {
            string[8] = '0';
        }
        string[9]='\0';
    }
    return perm;
}

void listDir(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return ;
    }
    else
    {
        printf("SUCCESS\n");
    }

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            printf("%s/%s\n", path,entry->d_name);

        }
    }
    closedir(dir);
}

void listDirPerm(const char *path, const char *permissions)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    char *permission;
    permission = (char*) malloc (9);
    char *perm;
    perm = (char*) malloc (9);
    int cont = 0;

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return ;
    }
    else
    {
        printf("SUCCESS\n");
    }

    for(int i=0; i<9; i++)
    {
        if(permissions[i]=='r' || permissions[i]=='w'|| permissions[i]=='x')
        {
            permission[cont]='1';
            cont++;
        }
        else if(permissions[i]=='-')
        {
            permission[cont]='0';
            cont++;
        }
    }


    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            strcpy(perm, getPerm(filePath));
            if(strcmp(perm, permission) == 0)
            {
                printf("%s\n", filePath);
            }
        }
    }
    closedir(dir);
    free(permission);
    free(perm);
}

void listDirString(const char *path, const char *starts)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return ;
    }
    else
    {
        printf("SUCCESS\n");
    }
    while((entry = readdir(dir)) != NULL)
    {

        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            if(strstr(entry->d_name,starts) != NULL)
            {
                printf("%s\n",filePath);
            }
        }
    }
    closedir(dir);

}

void listRec(const char *path, int first)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }

    if(first == 0)
    {
        printf("SUCCESS\n");
        first = 1;
    }
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0)
            {
                printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode))
                {
                    first = 1;
                    listRec(fullPath,first);
                }
            }
        }
    }
    closedir(dir);

}

void listRecPerm(const char *path, const char *permissions, int first)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    char *permission;
    permission = (char*) malloc (9);
    char *perm;
    perm = (char*) malloc (9);
    int cont = 0;
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }

    if(first == 0)
    {
        printf("SUCCESS\n");
        first = 1;
    }
    for(int i=0; i<9; i++)
    {
        if(permissions[i]=='r' || permissions[i]=='w'|| permissions[i]=='x')
        {
            permission[cont]='1';
            cont++;
        }
        else if(permissions[i]=='-')
        {
            permission[cont]='0';
            cont++;
        }
    }

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0)
            {
                strcpy(perm, getPerm(fullPath));
                if(strcmp(perm, permission) == 0)
                {
                    printf("%s\n", fullPath);
                }
                if(S_ISDIR(statbuf.st_mode))
                {
                    first = 1;
                    listRecPerm(fullPath,permissions,first);
                }
            }
        }
    }
    closedir(dir);
    free(perm);
    free(permission);

}


void listRecString(const char *path, const char *starts, int first)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }

    if(first == 0)
    {
        printf("SUCCESS\n");
        first = 1;
    }
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0)
            {
                if(strstr(entry->d_name,starts) != NULL)
                {
                    printf("%s\n",fullPath);
                }
                if(S_ISDIR(statbuf.st_mode))
                {
                    first = 1;
                    listRec(fullPath,first);
                }
            }
        }
    }
    closedir(dir);

}

int main(int argc, char **argv)
{
    char *delim;
    char *delim2;
    char *delim3;
    char *path;
    char *permissions;
    char *starts;

    int first = 0;
    if(argc < 2)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
    }
    else if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0)
        {
            printf("79559\n"); //2.2
        }
        if(strcmp(argv[1], "list") == 0)
        {
            if(argc == 3)
            {
                delim = "=";
                path = strtok(argv[2],delim);
                path = strtok(NULL,delim);
                listDir(path);
            }
            else if(argc >= 3)
            {
                if(argc == 4)
                {
                    if(strncmp(argv[2],"path=",5) == 0)
                    {
                        delim = "=";
                        path = strtok(argv[2],delim);
                        path = strtok(NULL,delim);
                        if(strcmp(argv[3],"recursive") == 0)
                        {
                            listRec(path,0);
                        }
                        else if(strncmp(argv[3],"permissions=",12) == 0)
                        {
                            delim2 = "=";
                            permissions = strtok(argv[3],delim2);
                            permissions = strtok(NULL,delim2);
                            listDirPerm(path,permissions);
                        }
                        else if(strncmp(argv[3],"name_starts_with=",17) == 0)
                        {
                            delim3 = "=";
                            starts = strtok(argv[3],delim3);
                            starts = strtok(NULL,delim3);
                            listDirString(path,starts);
                        }
                    }
                    else if(strncmp(argv[3],"path=",5) == 0)
                    {
                        delim = "=";
                        path = strtok(argv[3],delim);
                        path = strtok(NULL,delim);
                        if(strcmp(argv[2],"recursive") == 0)
                        {
                            listRec(path,first);
                        }
                        else if(strncmp(argv[2],"permissions=",12) == 0)
                        {
                            delim2 = "=";
                            permissions = strtok(argv[2],delim2);
                            permissions = strtok(NULL,delim2);
                            listDirPerm(path,permissions);
                        }
                        else if(strncmp(argv[2],"name_starts_with=",17) == 0)
                        {
                            delim3 = "=";
                            starts = strtok(argv[2],delim3);
                            starts = strtok(NULL,delim3);
                            listDirString(path,starts);
                        }
                    }
                }
                else if(argc >=4)
                {
                    if(argc == 5)
                    {
                        if(strncmp(argv[2],"path=",5) == 0)
                        {
                            delim = "=";
                            path = strtok(argv[2],delim);
                            path = strtok(NULL,delim);
                            if(strcmp(argv[3],"recursive") == 0)
                            {
                                if(strncmp(argv[4],"permissions=",12) == 0)
                                {
                                    delim2 = "=";
                                    permissions = strtok(argv[4],delim2);
                                    permissions = strtok(NULL,delim2);
                                    listRecPerm(path,permissions,first);
                                }
                                else if(strncmp(argv[4],"name_starts_with=",17) == 0)
                                {
                                    delim3 = "=";
                                    starts = strtok(argv[4],delim3);
                                    starts = strtok(NULL,delim3);
                                    listDirString(path,starts);
                                }
                            }
                            else if(strcmp(argv[4],"recursive") == 0)
                            {
                                if(strncmp(argv[3],"permissions=",12) == 0)
                                {
                                    delim2 = "=";
                                    permissions = strtok(argv[3],delim2);
                                    permissions = strtok(NULL,delim2);
                                    listRecPerm(path,permissions,first);
                                }
                                else if(strncmp(argv[3],"name_starts_with=",17) == 0)
                                {
                                    delim3 = "=";
                                    starts = strtok(argv[3],delim3);
                                    starts = strtok(NULL,delim3);
                                    listDirString(path,starts);
                                }
                            }
                        }
                        else if(strncmp(argv[3],"path=",5) == 0)
                        {
                            delim = "=";
                            path = strtok(argv[3],delim);
                            path = strtok(NULL,delim);
                            if(strcmp(argv[2],"recursive") == 0)
                            {
                                if(strncmp(argv[4],"permissions=",12) == 0)
                                {
                                    delim2 = "=";
                                    permissions = strtok(argv[4],delim2);
                                    permissions = strtok(NULL,delim2);
                                    listRecPerm(path,permissions,first);
                                }
                                else if(strncmp(argv[4],"name_starts_with=",17) == 0)
                                {
                                    delim3 = "=";
                                    starts = strtok(argv[4],delim3);
                                    starts = strtok(NULL,delim3);
                                    listDirString(path,starts);
                                }
                            }
                            if(strcmp(argv[4],"recursive") == 0)
                            {
                                if(strncmp(argv[2],"permissions=",12) == 0)
                                {
                                    delim2 = "=";
                                    permissions = strtok(argv[2],delim2);
                                    permissions = strtok(NULL,delim2);
                                    listRecPerm(path,permissions,first);
                                }
                                else if(strncmp(argv[2],"name_starts_with=",17) == 0)
                                {
                                    delim3 = "=";
                                    starts = strtok(argv[2],delim3);
                                    starts = strtok(NULL,delim3);
                                    listDirString(path,starts);
                                }
                            }
                        }
                        else if(strncmp(argv[4],"path=",5) == 0)
                        {
                            delim = "=";
                            path = strtok(argv[4],delim);
                            path = strtok(NULL,delim);
                            if(strcmp(argv[2],"recursive") == 0)
                            {
                                if(strncmp(argv[3],"permissions=",12) == 0)
                                {
                                    delim2 = "=";
                                    permissions = strtok(argv[3],delim2);
                                    permissions = strtok(NULL,delim2);
                                    listRecPerm(path,permissions,first);
                                }
                                else if(strncmp(argv[3],"name_starts_with=",17) == 0)
                                {
                                    delim3 = "=";
                                    starts = strtok(argv[3],delim3);
                                    starts = strtok(NULL,delim3);
                                    listDirString(path,starts);
                                }
                            }
                            else if(strcmp(argv[3],"recursive") == 0)
                            {
                                if(strncmp(argv[2],"permissions=",12) == 0)
                                {
                                    delim2 = "=";
                                    permissions = strtok(argv[2],delim2);
                                    permissions = strtok(NULL,delim2);
                                    listRecPerm(path,permissions,first);
                                }
                                else if(strncmp(argv[2],"name_starts_with=",17) == 0)
                                {
                                    delim3 = "=";
                                    starts = strtok(argv[2],delim3);
                                    starts = strtok(NULL,delim3);
                                    listDirString(path,starts);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
