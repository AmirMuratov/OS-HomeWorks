#ifndef HELPERS
#define HELPERS

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int spawn(const char * file, char * const argv []);
ssize_t read_(int fd, void *buf, size_t count);
ssize_t read_until(int fd, void * buf, size_t count, char delimiter);
ssize_t write_(int fd, const void *buf, size_t count);

#endif