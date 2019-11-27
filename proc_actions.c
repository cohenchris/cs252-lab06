#include "proc_actions.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

#define MAX_FILENAME (50)
#define MAX_INT_LEN (10)

int stop_proc(pid_t pid) {
  if (kill(pid, SIGSTOP) != 0) {
    perror("stop process");
    return -1;
  }
  return 0;
} /* stop_proc() */

int continue_proc(pid_t pid) {
  if (kill(pid, SIGCONT) != 0) {
    perror("continue process");
    return -1;
  }
  return 0;
} /* continue_proc() */

int kill_proc(pid_t pid) {
  if (kill(pid, SIGKILL) != 0) {
    perror("kill process");
    return -1;
  }
  return 0;
} /* kill_proc() */

char * list_mm(pid_t pid) {
  char * mmap_file_name = strdup("/proc/");
  mmap_file_name = realloc(mmap_file_name, sizeof(char) * MAX_FILENAME);
  char * pid_str = malloc(sizeof(char) * MAX_INT_LEN);
  sprintf(pid_str, "%d", pid);
  strcat(mmap_file_name, pid_str);
  strcat(mmap_file_name, "/maps");

  FILE * mmap_fp = fopen(mmap_file_name, "r");
  if (mmap_fp == NULL) {
    return "invalid pid";
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;
  //char * mem_map = malloc(sizeof(char) * 50000);
  char * mem_map = NULL;
  while ((read_len = getline(&line, &len, mmap_fp)) != -1) {
    if (mem_map == NULL) {
      mem_map = realloc(mem_map, sizeof(char) * (read_len + 2));
    }
    else {
      mem_map = realloc(mem_map, sizeof(char) * (strlen(mem_map) + read_len + 3));
    }
    strcat(mem_map, line);
  }

  free(mmap_file_name);
  mmap_file_name = NULL;
  free(pid_str);
  pid_str = NULL;
  free(line);
  line = NULL;

  fclose(mmap_fp);
  mmap_fp = NULL;

  return mem_map;
} /* list_mm() */

char * list_open_files(pid_t pid) {

} /* list_open_files() */
