#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char * proc_name;
  size_t proc_id;
  //char * proc_user;
} process_info;

process_info * get_proc_info();

int g_num_procs;
