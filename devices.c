#include "devices.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_num_devices = 0;
device_info * g_device_info;

device_info * get_device_info() {
  FILE * mounts_file = fopen("/proc/mounts", "r");
  if (mounts_file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;
  char * field = NULL;

  while ((read_len = getline(&line, &len, mounts_file)) != -1) {
    device_info new_device = { 0 };
    // device name
    field = strtok(line, " ");
    new_device.device = strdup(field);
    // mount point
    field = strtok(line, " ");
    new_device.mount_point = strdup(field);

    // add new device to list
    g_device_info = realloc(g_device_info,
                            sizeof(device_info) * (g_num_devices + 1));
    g_device_info[g_num_devices] = new_device;
    g_num_devices++;
  }

  fclose(mounts_file);
  mounts_file = NULL;
  free(line);
  line = NULL;

  return g_device_info;
} /* get_device_info() */
