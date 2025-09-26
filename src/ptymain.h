#ifndef PTYMAIN_H
#define PTYMAIN_H

int init_pty(void);
void reset_pty(fd_set *, int, int);

#endif /* PTYMAIN_H */