#ifndef __SYS_H__
#define __SYS_H__

void init_syscall();

int sys_example();
int sys_shutdown(int n);
int sys_write(const char* s, int len);

#endif
