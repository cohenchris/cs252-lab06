#include "graphinfo.h"

#include <stdlib.h>
#include <stdio.h>

graph_info g_graph_info = {0};

void get_cpu() {
  // get average cpu usage, of all cpus
  g_graph_info.cpu_usage = 0;
  // TODO: get average of all cpu usage
}


void get_mem() {
  // get memory usage
  g_graph_info.mem_usage = 0;
  //TODO: read in mem from /proc/meminfo
}


void get_swap() {
  // get swap usage
  g_graph_info.swap_usage = 0;
  //TODO: read in swap from /proc/meminfo
}


void get_network() {
  // get network usage
  g_graph_info.network_usage = 0;
  //TODO: find network usage
}

int main() {

  get_cpu();
  get_mem();
  get_swap();
  get_network();

  printf("cpu usage: %Lf\n", g_graph_info.cpu_usage);
  printf("mem usage: %Lf\n", g_graph_info.mem_usage);
  printf("swap usage: %Lf\n", g_graph_info.swap_usage);
  printf("network usage: %Lf\n", g_graph_info.network_usage);

}
