#include "procinfo.h"

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>

process_info * g_proc_info = NULL;
int g_num_procs = 0;

/*
 * Helps sort processes by parent id. Helpful for tree view in process manager.
 */

int compare_procs(const void * p1, const void * p2) {
  if (((process_info *) p1)->parent_id < ((process_info *) p2)->parent_id) {
    return -1;
  }
  else if (((process_info *) p1)->parent_id > ((process_info *) p2)->parent_id) {
    return 1;
  }
  else {
    return 0;
  }
} /* compare_procs() */

void read_status_file(process_info new_proc, char * proc_dir_path) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read_len = 0;

  /* Extract UID and get username from that */
  char * status_path = strdup(proc_dir_path);
  char * status = "/status";
  status_path = realloc(status_path, sizeof(char) *
                                    (strlen(status_path) +
                                     strlen(status) + 1));
  strcat(status_path, status);

  FILE * status_file = fopen(status_path, "r");
  if (status_file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char * first_tab = NULL;
  char * second_tab = NULL;
  size_t shared_size = 0;
  int shared_malloc_size = 0;
  while ((read_len = getline(&line, &len, status_file)) != -1) {
    first_tab = strchr(line, '\t');
    second_tab = strchr(first_tab + 1, '\t');
    if (strstr(line, "Uid:") != NULL) {
      /* Extract UID */
      char * uid = strndup(first_tab + 1, second_tab - first_tab - 1);
      struct passwd * pws_struct = { 0 };
      uid_t uid_num = 0;
      sscanf(uid, "%u", &uid_num);

      pws_struct = getpwuid(uid_num);
      new_proc.proc_user = strdup(pws_struct->pw_name);

      free(uid);
      uid = NULL;
    }
    else if (strstr(line, "State:")  != NULL) {
      /* Extract Running State */
      char * state = strndup(first_tab + 1, strlen(first_tab + 1) - 1);
      new_proc.state = strdup(state);

      free(state);
      state = NULL;
    }
    else if (strstr(line, "VmSize:")  != NULL) {
      /* Extract Virtual Memory Size */
      char * vmsize = strndup(first_tab + 1, strlen(first_tab + 1) - 1);
      new_proc.virtual_mem = strdup(vmsize);

      free(vmsize);
      vmsize = NULL;
    }
    else if (strstr(line, "VmRSS")  != NULL) {
      /* Extract Resident Memory */
      char * vmrss = strndup(first_tab + 1, strlen(first_tab + 1) - 1);
      new_proc.resident_mem = strdup(vmrss);

      free(vmrss);
      vmrss = NULL;
    }
    else if (strstr(line, "RssFile")  != NULL) {
      /* Extract one part of shared memory */
      char * rssfile = strndup(first_tab + 1, strlen(first_tab + 1) - 1);
      size_t rssfile_size = 0;
      sscanf(rssfile, "%lu", &rssfile_size);
      shared_size += rssfile_size;
      shared_malloc_size += strlen(rssfile);

      free(rssfile);
      rssfile = NULL;
    }
    else if (strstr(line, "RssShmem")  != NULL) {
      /* Extract second part of shared memory */
      char * rssshmem = strndup(first_tab + 1, strlen(first_tab + 1) - 1);
      size_t rssshmem_size = 0;
      sscanf(rssshmem, "%lu", &rssshmem_size);
      shared_size += rssshmem_size;
      shared_malloc_size += strlen(rssshmem);

      free(rssshmem);
      rssshmem = NULL;
    }
    else if (strstr(line, "PPid") != NULL) {
      /* Extract parent process ID to make tree view */
      char * ppid = strndup(first_tab + 1, strlen(first_tab + 1) - 1);
      size_t ppid_num = atol(ppid);
      new_proc.parent_id = ppid_num;

      free(ppid);
      ppid = NULL;
    }
    else if (strstr(line, "") != NULL) {
      /* Extract total normal memory */
      char * mem = strndup(first_tab + 1, strlen(first_tab + 1) - 1);

      free(mem);
      mem = NULL;
    }
  }

  /* Add up total shared memory. '+ 5' is to include the label 'kB' */

  new_proc.shared_mem = malloc(sizeof(char) * (shared_malloc_size + 5));
  sprintf(new_proc.shared_mem, "%lu", shared_size);
  strcat(new_proc.shared_mem, " kb");
  
  /* Add new process information to g_num_procs */

  g_proc_info[g_num_procs] = new_proc;
  g_num_procs++;

  free(status_path);
  status_path = NULL;
  fclose(status_file);
  status_file = NULL;
  free(line);
  line = NULL;
} /* read_status_file() */

/* TODO: header
 */

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

          read_status_file(new_proc, proc_dir_path);
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

  qsort(g_proc_info, g_num_procs, sizeof(process_info), compare_procs);

  return g_proc_info;
} /* get_proc_info() */

void get_time_info(process_info new_proc, char * proc_dir_path) {
  /* CPU Time from /proc/[pid]/stat file */

  char * stat = "/stat";
  char * stat_file_path = strdup(proc_dir_path);
  stat_file_path =  realloc(stat_file_path, sizeof(char) *
                                            (strlen(stat_file_path) +
                                             strlen(stat) + 1));
  strcat(stat_file_path, stat);

  FILE * stat_file = fopen(stat_file_path, "r");
  if (stat_file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  //TODO: cpu time

  fclose(stat_file);
  stat_file = NULL;

  /* date/time started from /proc/[pid]/uptime file */

  char * uptime = "/uptime";
  char * uptime_file_path = strdup(proc_dir_path);
  uptime_file_path = realloc(uptime_file_path, sizeof(char) *
                                               (strlen(uptime_file_path) +
                                                strlen(uptime) + 1));
  strcat(uptime_file_path, uptime);

  FILE * uptime_file = fopen(uptime_file_path, "r");
  if (uptime_file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  //TODO: date/time

  fclose(uptime_file);
  uptime_file = NULL;
} /* get_time_info() */
