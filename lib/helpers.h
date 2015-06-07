#ifndef HELPERS
#define HELPERS

#include <unistd.h>
#include <sys/wait.h>


typedef struct execargs_t {
    char** args;
} execargs_t;


execargs_t* execargs_t_new(char** arguments);
void execargs_t_free(execargs_t* t);
int exec(struct execargs_t* args);
int runpiped(struct execargs_t** programs, size_t n);
int spawn(const char * file, char * const argv []);
ssize_t read_(int fd, void *buf, size_t count);
ssize_t read_until(int fd, void * buf, size_t count, char delimiter);
ssize_t write_(int fd, const void *buf, size_t count);

#endif