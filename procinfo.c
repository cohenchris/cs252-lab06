#include "procinfo.h"

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>

//TODO: keep track of user who owns the process!!

process_info * g_proc_info = NULL;
int g_num_procs = 0;

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

  size_t curr_proc_id = 0;
  char * comm = "/comm";
  char * proc_dir_path = NULL;

  while ((dir_entry = readdir(dir)) != NULL) {
    //TODO: enter dir_entry->d_name and read 'comm'
    proc_dir_path = strdup(base_dir);
    proc_dir_path = realloc(proc_dir_path, sizeof(char) * (strlen(base_dir) +
                                           strlen(dir_entry->d_name) + 2));
    strcat(proc_dir_path, "/");
    strcat(proc_dir_path, dir_entry->d_name);

    sscanf(dir_entry->d_name, "%zu", &curr_proc_id);

    DIR * proc_dir = opendir(proc_dir_path);
    if (proc_dir == NULL) {
      free(proc_dir_path);
      proc_dir_path = NULL;
      continue;
    }

    /* open and read 'comm' file, which contains the process name. */
    char * comm_path = strdup(proc_dir_path);
    comm_path = realloc(comm_path, sizeof(char) * (strlen(proc_dir_path) +
                                   strlen(comm) + 1));
    strcat(comm_path, comm);

    FILE * comm_file = fopen(comm_path, "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read_len = 0;
    if (comm_file != NULL) {
      if ((read_len = getline(&line, &len, comm_file)) != -1 ) {
        line[strlen(line) - 1] = '\0';
        if (strlen(line) > 0) {
          g_proc_info = realloc(g_proc_info, sizeof(process_info) *
                                             (g_num_procs + 1));

          process_info new_proc = { 0 };
          new_proc.proc_name = strdup(line);
          new_proc.proc_id = curr_proc_id;
          //new_proc.proc_user = strdup("cohen50");

          g_proc_info[g_num_procs] = new_proc;
          g_num_procs++;
        }
      }
      free(line);
      line = NULL;
      fclose(comm_file);
      comm_file = NULL;
    }
    free(comm_path);
    comm_path = NULL;
    free(proc_dir_path);
    proc_dir_path = NULL;
    closedir(proc_dir);
    proc_dir = NULL;
  }

  if (closedir(dir) == -1){
    perror("closedir");
    exit(EXIT_FAILURE);
  }
  dir = NULL;

  return g_proc_info;
} /* get_proc_info() */
