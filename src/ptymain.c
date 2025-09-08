#include <sys/types.h>
#include <unistd.h>
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/select.h>

#include "ptymain.h"

const char *SHELL_PATH = "/bin/sh";   /* placeholder */

/* return file descriptor for master */
int
init_pty(void)
{
    int fd_master;
    pid_t pid = forkpty(&fd_master, NULL, NULL, NULL);

    if (pid == -1)
    {
        perror("forkpty");
        exit(1);
    }

    /* parent: disable echo on master fd */
    if (pid > 0)
    {
        struct termios t;
        if (tcgetattr(fd_master, &t) == -1)
        {
            perror("tcgetattr");
            exit(1);
        } 

        t.c_lflag &= ~ECHO;
        tcsetattr(fd_master, TCSANOW, &t);
    }

    /* child: exec shell */
    if (pid == 0)
    {
        execlp(SHELL_PATH, SHELL_PATH, NULL);
        perror("execlp");   /* execlp doesn't return */
        exit(1);
    }

    return fd_master;
}

/* reset file descriptor set for select() loop */
void
reset_pty(int *fd_master, int *fd_xwin)
{
    fd_set fds;
    FD_ZERO(&fds);              /* clear set */
    FD_SET(*fd_master, &fds);   /* add master fd to set */
    FD_SET(*fd_xwin, &fds);     /* add xwin fd to set */
}