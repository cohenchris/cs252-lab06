#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sysinfo.h"
#include "procinfo.h"

void main() {
  printf("#################################################################\n");
  printf("#                        SYSTEM INFO                            #\n");
  printf("#################################################################\n");
  system_info curr_sys_info = get_system_info();

  printf("---------- RELEASE ----------\n");
  printf("%s\n", curr_sys_info.release_name);
  printf("Release %s\n", curr_sys_info.release_version);
  printf("Kernel %s\n", curr_sys_info.kernel_version);

  printf("\n---------- HARDWARE ----------\n");
  printf("Memory: %s kB\n", curr_sys_info.ram_size);
  printf("Processor: %s\b", curr_sys_info.cpu_info);

  printf("\n---------- DISK ----------\n");
  printf("Available Disk Space: %s kB\n", curr_sys_info.disk_space);

  printf("\n\n");

  printf("#################################################################\n");
  printf("#                        PROCESS INFO                           #\n");
  printf("#################################################################\n");
  process_info * curr_proc_info = get_proc_info();

  int num_processes = sizeof(curr_proc_info) / sizeof(curr_proc_info[0]);
  for (int i = 0; i < num_processes; i++) {
    printf("%lu - %s\n", curr_proc_info[i].proc_id, curr_proc_info[i].proc_name);
  }

  /* ------------------------------ FREEING ------------------------------ */

  free(curr_sys_info.release_name);
  curr_sys_info.release_name = NULL;
  free(curr_sys_info.release_version);
  curr_sys_info.release_version = NULL;
  free(curr_sys_info.kernel_version);
  curr_sys_info.kernel_version = NULL;
  free(curr_sys_info.ram_size);
  curr_sys_info.ram_size = NULL;
  free(curr_sys_info.cpu_info);
  curr_sys_info.cpu_info = NULL;
  free(curr_sys_info.disk_space);
  curr_sys_info.disk_space = NULL;

  for (int i = 0; i < num_processes; i++) {
    /*
    free(curr_proc_info[i].proc_id);
    curr_proc_info[i].proc_id = NULL;
    */
    free(curr_proc_info[i].proc_name);
    curr_proc_info[i].proc_name = NULL;
  }
} /* main() */