#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_system_info() {
  /* Execute bash script to parse system info and read in resulting file. */

  system("./get_sysinfo.sh > sysinfo.txt");
  FILE * fp = fopen("./sysinfo.txt", "r");
  if (fp == NULL) {
    perror("Couldn't read sysinfo.txt");
    exit(EXIT_FAILURE);
  }

  /* find file size */

  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char * buffer = malloc(sizeof(char) * fsize);

  fread(buffer, sizeof(char), fsize, fp);
  /*
  if (fread(buffer, sizeof(char), fsize, fp) <= fsize) {
    perror("fread");
    free(buffer);
    buffer = NULL;
    exit(EXIT_FAILURE);
  }
  */

  /* Parse each line to get system info needed */
  // RELEASE

  printf("%s\n", buffer);

  fclose(fp);
  fp = NULL;
} /* get_system_info() */

int main() {
  get_system_info();
} /* main() */
