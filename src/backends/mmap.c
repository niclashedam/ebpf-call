#include "../loader.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int fd_prog[UBPF_ENGINES];
int fd_data[UBPF_ENGINES];
int fd_trigger[UBPF_ENGINES];
int fd_status[UBPF_ENGINES];
int fd_load[UBPF_ENGINES];

int remove_directory(const char *path) {
  DIR *d = opendir(path);
  size_t path_len = strlen(path);
  int r = -1;

  if (d) {
    struct dirent *p;

    r = 0;
    while (!r && (p = readdir(d))) {
      int r2 = -1;
      char *buf;
      size_t len;

      /* Skip the names "." and ".." as we don't want to recurse on them. */
      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
        continue;

      len = path_len + strlen(p->d_name) + 2;
      buf = malloc(len);

      if (buf) {
        struct stat statbuf;

        snprintf(buf, len, "%s/%s", path, p->d_name);
        if (!stat(buf, &statbuf)) {
          if (S_ISDIR(statbuf.st_mode))
            r2 = remove_directory(buf);
          else
            r2 = unlink(buf);
        }
        free(buf);
      }
      r = r2;
    }
    closedir(d);
  }

  if (!r)
    r = rmdir(path);

  return r;
}

int bpf_start() {
  // If not root, fail
  if (getuid() != 0)
    fprintf(stderr, "MMAP backend requires root!\n");
  return getuid();
}

int bpf_expose(struct bpf_slot *slot) {
  struct stat st = {0};

  char dir[64];
  char file_prog[64];
  char file_data[64];
  char file_trigger[64];
  char file_status[64];
  char file_load[64];

  sprintf(dir, "/tmp/bpf/%i", slot->i);
  sprintf(file_prog, "/tmp/bpf/%i/program", slot->i);
  sprintf(file_data, "/tmp/bpf/%i/data", slot->i);
  ;
  sprintf(file_trigger, "/tmp/bpf/%i/trigger", slot->i);
  sprintf(file_status, "/tmp/bpf/%i/status", slot->i);
  sprintf(file_load, "/tmp/bpf/%i/load", slot->i);

  if (stat(dir, &st) == -1) {
    mkdir("/tmp/bpf", 0777);
    mkdir(dir, 0777);
  }

  fd_prog[slot->i] = open(file_prog, O_RDWR | O_CREAT, (mode_t)0777);
  fd_data[slot->i] = open(file_data, O_RDWR | O_CREAT, (mode_t)0777);
  fd_trigger[slot->i] = open(file_trigger, O_RDWR | O_CREAT, (mode_t)0777);
  fd_status[slot->i] = open(file_status, O_RDWR | O_CREAT, (mode_t)0777);
  fd_load[slot->i] = open(file_load, O_RDWR | O_CREAT, (mode_t)0777);

  /* Seek to the end of the files */

  if (lseek(fd_prog[slot->i], UBPF_PROG_BUF, SEEK_SET) == -1) {
    close(fd_prog[slot->i]);
    fprintf(stderr, "Unable to set filesize of PROG for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (lseek(fd_data[slot->i], UBPF_DATA_BUF, SEEK_SET) == -1) {
    close(fd_data[slot->i]);
    fprintf(stderr, "Unable to set filesize of DATA for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (lseek(fd_trigger[slot->i], sizeof(char), SEEK_SET) == -1) {
    close(fd_trigger[slot->i]);
    fprintf(stderr, "Unable to set filesize of TRIGGER for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (lseek(fd_status[slot->i], sizeof(char), SEEK_SET) == -1) {
    close(fd_status[slot->i]);
    fprintf(stderr, "Unable to set filesize of STATUS for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (lseek(fd_load[slot->i], sizeof(char), SEEK_SET) == -1) {
    close(fd_load[slot->i]);
    fprintf(stderr, "Unable to set filesize of LOAD for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  /* Write last bytes to expose size of FS */

  if (write(fd_prog[slot->i], "", 1) == -1) {
    close(fd_prog[slot->i]);
    fprintf(stderr, "Error writing last byte of PROG file for slot %i\n",
            slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (write(fd_data[slot->i], "", 1) == -1) {
    close(fd_data[slot->i]);
    fprintf(stderr, "Error writing last byte of DATA file for slot %i\n",
            slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (write(fd_trigger[slot->i], "", 1) == -1) {
    close(fd_trigger[slot->i]);
    fprintf(stderr, "Error writing last byte of TRIGGER file for slot %i\n",
            slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (write(fd_status[slot->i], "", 1) == -1) {
    close(fd_status[slot->i]);
    fprintf(stderr, "Error writing last byte of STATUS file for slot %i\n",
            slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  if (write(fd_load[slot->i], "", 1) == -1) {
    close(fd_load[slot->i]);
    fprintf(stderr, "Error writing last byte of LOAD file for slot %i\n",
            slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  /* Expose file to BPF loader */
  slot->prog = mmap(0, UBPF_PROG_BUF, PROT_READ | PROT_WRITE, MAP_SHARED,
                    fd_prog[slot->i], 0);
  if (slot->prog == MAP_FAILED) {
    close(fd_prog[slot->i]);
    fprintf(stderr, "Error mmapping the PROG file for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  slot->data = mmap(0, UBPF_DATA_BUF, PROT_READ | PROT_WRITE, MAP_SHARED,
                    fd_data[slot->i], 0);
  if (slot->prog == MAP_FAILED) {
    close(fd_data[slot->i]);
    fprintf(stderr, "Error mmapping the DATA file for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  slot->trigger = mmap(0, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED,
                       fd_trigger[slot->i], 0);
  if (slot->trigger == MAP_FAILED) {
    close(fd_trigger[slot->i]);
    fprintf(stderr, "Error mmapping the TRIGGER file for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  slot->status = mmap(0, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED,
                      fd_status[slot->i], 0);
  if (slot->status == MAP_FAILED) {
    close(fd_status[slot->i]);
    fprintf(stderr, "Error mmapping the STATUS file for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  slot->load = mmap(0, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED,
                    fd_load[slot->i], 0);
  if (slot->prog == MAP_FAILED) {
    close(fd_load[slot->i]);
    fprintf(stderr, "Error mmapping the LOAD file for slot %i\n", slot->i);
    perror("");
    exit(EXIT_FAILURE);
  }

  return 0;
}

void bpf_unexpose(struct bpf_slot *slot) {
  //
}

void bpf_end() {
  //
}
