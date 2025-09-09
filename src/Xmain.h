#ifndef XMAIN_H
#define XMAIN_H

void draw(int, int, int);
void repo(int, int);
void carriage_return(void);
void reset_screen(void);
int compose_input(char*, int);
int write_to_pipe(char*);
int read_from_pipe(char*);

#endif /* XMAIN_H */
