#include <stdlib.h>
#include <stdio.h>

int stop_proc(pid_t pid);
int continue_proc(pid_t pid);
int kill_proc(pid_t pid);
char * list_mm(pid_t pid);
char * list_open_files(pid_t pid);
