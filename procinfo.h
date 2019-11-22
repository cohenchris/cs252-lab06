#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  char * proc_name;
  size_t proc_id;
  char * proc_user;
  //TODO
  bool running;
  char * mem;
  char * virtual_mem;
  char * resident_mem;
  char * shared_mem;
  char * cpu_time;
  char * start_date;
} process_info;

process_info * get_proc_info();

int g_num_procs;
