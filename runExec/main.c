#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/poll.h>

#define __unused __attribute__((unused))
#define MAX_ARGS 10

/* 
 * Usage: ret = myPipe("ls -l", &output);
 *
 * Return:  Return 0 if success, (-errno) if fail.
 */
int runExec(const char *cmd, char *output, ssize_t len)
{
    int err = 0, pid = -1;
    ssize_t n = 0;
    int fds[2]; // 0=read, 1=write
    struct pollfd pfd = {0};

    char *buf = NULL, *tok = NULL;
    int bufsize = strlen(cmd) + 1;
    char exec[64] = {0};
    char **argv = NULL;
    int nargs = 0;

    if(pipe(fds) < 0)
    {
        err = errno;
        goto out;
    }

    pid = fork();
    if(pid < 0)
    {
        err = errno;
        goto out;
    }

    if(pid == 0) /* Child */
    {

        close(fds[0]); /* remove reference to pipe read */
        dup2(fds[1], STDOUT_FILENO); /* make stdout point to pipe write */
        close(fds[1]); /* No need for pipe write fd anymore */

        /* At this point stdout -> pipe write */

        argv = (char **)malloc(sizeof(char *) * MAX_ARGS);
        if(argv == NULL)
        {
            err = errno;
            goto out;
        }

        buf = (char *)malloc(sizeof(char) * bufsize);
        if(buf == NULL)
        {
            err = errno;
            goto out;
        }
        memset(buf, 0, bufsize);
        strncpy(buf, cmd, bufsize);

        argv[nargs++] = strtok(buf, " ");
        while((tok = strtok(NULL, " ")) && (nargs < (MAX_ARGS-1)))
        {
            argv[nargs++] = tok;
        }
        argv[nargs] = NULL;

        strncpy(exec, argv[0], (sizeof(exec) - 1));

        execvp(exec, argv);
out:

        if(buf)
        {
            free(buf);
        }
        if(argv)
        {
            free(argv);
        }
        buf = NULL;
        argv = NULL;

        if(err != 0)
        {
            fprintf(stderr, "[runExec][error(%d)]: %s\n", err, strerror(err));
            return (-errno);
        }
    }
    else
    {
        if(pid == -1) /* Handle fork() error */
        {
            fprintf(stderr, "[runExec][error(%d)]: %s\n", err, strerror(err));
            return (-err);
        }
        close(fds[1]); /* Don't need write */

        pfd.fd = fds[0];
        pfd.events = POLLIN;

        if(poll(&pfd, 1, 5000) > 0) /* Timeout for 5s when no response */
        {
            if((n = read(pfd.fd, output, len)) < 0)
            {
                err = errno;
                fprintf(stderr, "[runExec][error(%d)]: %s\n", err, strerror(err));
                return (-err);
            }

            output[(n > len) ? len : n] = '\0';
        }

        wait(NULL);
    }

    return 0;
}

int main(int __unused argc, char __unused *argv[])
{
    const size_t len = 2048;
    char *output = (char *)malloc(sizeof(char) * len);
    //char *argv[] = {"ls", "-l", NULL}
    runExec("ls .", output, len-1);
    printf("%s\n", output);

    free(output);
    return 0;
}
