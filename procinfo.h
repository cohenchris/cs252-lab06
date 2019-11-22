#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char * proc_name;
  size_t proc_id;
} process_info;

process_info * get_proc_info();
