#define _GNU_SOURCE
#include "helpers.h"
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

execargs_t* execargs_t_new(char** arguments) {
	execargs_t* ret = malloc(sizeof(execargs_t));
    int len = 0;
    while (arguments[len] != NULL) len++;
    ret->args = (char**) malloc((len + 1) * sizeof(char*));
    for (int i = 0; i < len; i++) {
        ret->args[i] = strdup(arguments[i]);
    }
    ret->args[len] = NULL;
    return ret;
}

void execargs_t_free(execargs_t* t) {
	free(t->args);
	free(t);
}

int exec(execargs_t* args) {
	return spawn(args->args[0], args->args);
}

int size;
int* childa;

void sig_handler(int sig) {
    int i;
    for (i = 0; i < size; i++) {
        kill(childa[i], SIGKILL);
    }
    size = 0;
}

int runpiped(execargs_t** programs, size_t n) {
	int infd = STDIN_FILENO;
	int childs[n];
	childa = (int *)childs;
	for (int i = 0; i < n; i++) {
		int curpipe[2];
		if (pipe(curpipe) == -1) {
			return -1;
		}
		int pid = fork();
		if (pid == -1) {
			return -1;
		}
		if (pid == 0) {
			dup2(infd, STDIN_FILENO);
			if (i != n - 1) {
				dup2(curpipe[1], STDOUT_FILENO);
			}
			_exit(exec(programs[i]));
		}
		close(curpipe[1]);
		if (i != 0) {
			close(infd);
		}
		infd = curpipe[0];
		childs[i] = pid;
	}
	
	size = n;
	
	struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = &sig_handler;

    if (sigaction(SIGINT, &act, NULL) < 0) {
        return -1;
	}

    int status;
    for (int i = 0; i < n; i++) {
        waitpid(childs[i], &status, 0);
    }
    size = 0;
    return 0;
}



int spawn(const char * file, char * const argv []) {
	int pid = fork();
	if (pid == 0) {
		exit(execvp(file, argv));
    } else {
    	if (pid < 0) {
        	return -1;
    	}
        int status;
        wait(&status);
        return status;
    }
}


ssize_t read_until(int fd, void * buffer, size_t count, char delimiter) {
	size_t bytes_read = 0;
	ssize_t res = read(fd, buffer, count);
	while (res > 0) {
		int ok = 0;
		for (int i = bytes_read; i < bytes_read + res; i++) {
			if (*((char*)buffer + i)  == delimiter) {
				ok = 1;
				break;
			}
		}
		buffer += res;
		bytes_read += res;
		if (ok == 1) {
			break;
		}
		res = read(fd, buffer, count - bytes_read);
	} 
	if (res < 0) {
		return res;
	} else {
		return bytes_read;
	}
}

ssize_t read_(int fd, void* buffer, size_t count) 
{
	size_t bytes_read = 0;
	ssize_t res = read(fd, buffer, count);
	while (res > 0) {
		buffer += res;
		bytes_read += res;
		res = read(fd, buffer, count - bytes_read);
	} 
	if (res < 0) {
		return res;
	} else {
		return bytes_read;
	}
}

ssize_t write_(int fd, const void* buffer, size_t count) 
{
	size_t bytes_written = 0;
	ssize_t res = write(fd, buffer, count);
	while (res > 0) {
		buffer += res;
		bytes_written += res;
		res = write(fd, buffer, count - bytes_written);
	} 
	if (res < 0) {
		return res;
	} else {
		return bytes_written;
	}
}
