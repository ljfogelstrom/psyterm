#ifndef PTYMAIN_H
#define PTYMAIN_H

int init_pty(void);
void reset_pty(int *fd_master, int *fd_xwin);

#endif /* PTYMAIN_H */