#include "helpers.h"
#include <stdio.h>
#include <unistd.h>

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