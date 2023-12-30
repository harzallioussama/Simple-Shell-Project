#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>

char *builtin_cmd[] = {"cd", "ls", "quit", "help" , "rename"};

bool rname(char **args)
{
    if (rename(args[1], args[2]) != 0)
    {
        fprintf(stderr, "Op failed!");
    }
    return 1;
}

bool quit(char **args)
{
    return 0;
}

bool cd(char **args)
{
    // int PATH_MAX = ;
    char cw[256];
    if (args[1] == NULL)
    {
        fprintf(stderr, "shl: expected argument\n");
        return 1;
    }
    if (chdir(args[1]) != 0)
    {
        perror("chdir() to /error failed");
        return 1;
    }
    getcwd(cw, sizeof(cw));
    printf("%s\n", cw);
    return 1;
}

bool ls(char **args)
{
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s ", entry->d_name);
    }
    printf("\n");
    return 1;
}

bool help(char **args)
{
    int m = sizeof(builtin_cmd) / sizeof(char *);
    printf("-FunShell - Just for Fun!\n");
    printf("-Type the program name and arguments to execute a program : ./prog args\n");
    printf("-Some Built-in commands:\n");
    for (int i = 0; i < m; i++)
    {
        printf("%d)\t%s\n", i + 1, builtin_cmd[i]);
    }
}

bool (*builtin_fct[])(char **) = {cd, ls, quit, help , rname};

int traverse_word(const char *str)
{
    int i = 0;
    int count = 0;
    while (str[i] != '\0')
    {
        while (str[i] != '\0' && str[i] != ' ')
            i++;
        count++;
        if (str[i] == ' ')
            i++;
    }
    return count;
}

char *read_line()
{
    printf("shl > ");
    size_t buffer_size = 0;
    char *buf = (char *)malloc(sizeof(char) * buffer_size);
    if (getline(&buf, &buffer_size, stdin) == -1)
    {
        fprintf(stderr, "couldn't read the input");
    }

    return buf;
}

char **parse(char *buf)
{
    int count_words = traverse_word(buf);
    count_words++;
    char **args = (char **)malloc(sizeof(char *) * count_words);
    int i = 0;
    int pos = 0;
    // we could use strtok to split the string or just replace spaces with null char
    while (buf[i] != '\0')
    {
        args[pos++] = &buf[i];
        while (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '\n')
        {
            i++;
        }
        buf[i] = '\0';
        i++;
    }
    args[pos] = NULL;
    return args;
}
bool start(char **args)
{
    __pid_t pid = fork();
    int status;
    if (pid == -1)
    {
        fprintf(stderr, "can't fork!\n");
    }
    else if (pid == 0)
    {
        execv(args[0], args);
    }
    else
    {
        if (waitpid(pid, &status, 0) > 0)
        {
            if (WIFEXITED(status) && !WEXITSTATUS(status))
            {
            }
            else
            {
                fprintf(stderr, "program didn't terminate normally\n");
            }
        }
    }
    return 1;
}

bool execute(char **args)
{

    int cmd_size = sizeof(builtin_cmd) / sizeof(char *);
    for (int i = 0; i < cmd_size; i++)
    {
        if (strcmp(args[0], builtin_cmd[i]) == 0)
        {
            return (*builtin_fct[i])(args);
        }
    }
    return start(args);
}

void loop()
{

    char **args;
    char *buffer;
    bool flag = 1;
    while (flag)
    {
        buffer = read_line();
        // parse the input :
        args = parse(buffer);
        // execute commands :
        flag = execute(args);
        free(args);
        free(buffer);
    }
}
int main()
{
    loop();
}