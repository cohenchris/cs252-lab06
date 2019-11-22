#include <stdio.h>
#include <stdlib.h>

typedef struct {

  long double cpu_usage;
  long double mem_usage;
  long double swap_usage;
  long double network_usage;

} graph_info;

void get_cpu();
void get_mem();
void get_swap();
void get_network();
