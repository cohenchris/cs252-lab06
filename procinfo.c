#include "procinfo.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>

process_info * g_proc_info = NULL;
process_info * get_proc_info() {
  char * base_dir = "/proc";
  DIR * dir = opendir(base_dir);
  if (dir == NULL) {
    free(base_dir);
    base_dir = NULL;
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  struct dirent * dir_entry = { 0 };

  while ((dir_entry = readdir(dir)) != NULL) {
    //TODO: enter dir_entry->d_name and read 'comm'
    char *proc_dir_path = strdup(base_dir);
    proc_dir_path = realloc(proc_dir_path, sizeof(char) * (strlen(base_dir) +
                                           strlen(dir_entry->d_name) + 1));
    strcat(proc_dir_path, "/");
    strcat(proc_dir_path, dir_entry->d_name);

    DIR * proc_dir = opendir(proc_dir_path);
    if (proc_dir == NULL) {
      continue;
    }

    while ((dir_entry = readdir(proc_dir)) != NULL) {
      printf("d_name = %s\n", dir_entry->d_name);
    }
    printf("\n\n\n");
  }

  if (closedir(dir) == -1){
    perror("closedir");
    exit(EXIT_FAILURE);
  }
  dir = NULL;
  return g_proc_info;
} /* get_proc_info() */

/*
void update_curr_proc_info(...) {

}*/ /* update_curr_proc_info */
