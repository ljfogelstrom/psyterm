#ifndef PTYMAIN_H
#define PTYMAIN_H

int init_pty(void);
void reset_pty(fd_set *, int, int);

int fd_master;
int fd_xwin;
fd_set fds;
int nfds; 


#endif /* PTYMAIN_H */
