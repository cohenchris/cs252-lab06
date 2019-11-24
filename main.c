#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sysinfo.h"
#include "procinfo.h"
#include "devices.h"

extern int g_num_procs;
extern int g_num_devices;

int main() {
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
  //while (curr_proc_info[num_procs].proc_name != NULL) {
  printf("%-10s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",
         "proc_id", "proc_user", "proc_name", "state", "mem", "virtual_mem",
         "resident_mem", "shared_mem", "cpu_time", "start_date");
  printf("%s\n",
      "---------------------------------------------------------------------------------------------------------------------------------------------");
  for (int i = 0; i < g_num_procs; i++) {
    printf("%-10zu%-15s%-15s%-18s%-15s%-15s%-15s%-15s%-15s%-15s\n",
           curr_proc_info[i].proc_id,
           curr_proc_info[i].proc_user,
           curr_proc_info[i].proc_name,
           curr_proc_info[i].state,
           curr_proc_info[i].mem,
           curr_proc_info[i].virtual_mem,
           curr_proc_info[i].resident_mem,
           curr_proc_info[i].shared_mem,
           curr_proc_info[i].cpu_time,
           curr_proc_info[i].start_date
           );
  }

  printf("#################################################################\n");
  printf("#                         DEVICE INFO                           #\n");
  printf("#################################################################\n");
  device_info * curr_device_info = get_device_info();

  printf("%-35s%-20s%-20s%-20s%-20s%s\n",
         "device name", "1k blocks", "used", "available",
         "use %", "mount point");
  printf("%s\n",
      "--------------------------------------------------------------------------------------------------------------------------");

  for (int i = 0; i < g_num_devices; i++) {
    printf("%-35s%-20s%-20s%-20s%-20s%-20s\n",
           curr_device_info[i].device,
           curr_device_info[i].num_blocks,
           curr_device_info[i].used,
           curr_device_info[i].available,
           curr_device_info[i].use_percent,
           curr_device_info[i].mount_point);
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

  for (int i = 0; i < g_num_procs; i++) {
    free(curr_proc_info[i].proc_name);
    curr_proc_info[i].proc_name = NULL;
    free(curr_proc_info[i].proc_user);
    curr_proc_info[i].proc_user = NULL;
    free(curr_proc_info[i].state);
    curr_proc_info[i].state = NULL;
    free(curr_proc_info[i].mem);
    curr_proc_info[i].mem = NULL;
    free(curr_proc_info[i].virtual_mem);
    curr_proc_info[i].virtual_mem = NULL;
    free(curr_proc_info[i].resident_mem);
    curr_proc_info[i].resident_mem = NULL;
    free(curr_proc_info[i].shared_mem);
    curr_proc_info[i].shared_mem = NULL;
    free(curr_proc_info[i].cpu_time);
    curr_proc_info[i].cpu_time = NULL;
    free(curr_proc_info[i].start_date);
    curr_proc_info[i].start_date = NULL;
  }
  free(curr_proc_info);
  curr_proc_info = NULL;

  for (int i = 0; i < g_num_devices; i++) {
    free(curr_device_info[i].device);
    curr_device_info[i].device = NULL;
    free(curr_device_info[i].mount_point);
    curr_device_info[i].mount_point = NULL;
    free(curr_device_info[i].num_blocks);
    curr_device_info[i].num_blocks = NULL;
    free(curr_device_info[i].used);
    curr_device_info[i].used = NULL;
    free(curr_device_info[i].available);
    curr_device_info[i].available = NULL;;
    free(curr_device_info[i].use_percent);
    curr_device_info[i].use_percent = NULL;
  }
  free(curr_device_info);
  curr_device_info = NULL;

  return EXIT_SUCCESS;
} /* main() */
