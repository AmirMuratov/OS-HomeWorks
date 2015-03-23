#include <helpers.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

size_t const buff_size = 8192;

int main() {
	char buffer[buff_size];
	char write_buffer[100];
	ssize_t bytes_num = 1;
	int shift = 0;
	while (1) {
		bytes_num = read_until(STDIN_FILENO, buffer + shift, buff_size, ' ');	
		if (bytes_num <= 0) {
			break;
		}
		int last = 0;
		for (int i = shift; i < shift + bytes_num; i++) {
			if (buffer[i] == ' ') {
				sprintf(write_buffer, "%d\n", i - last);
				last = i + 1;
				if (write_(STDOUT_FILENO, write_buffer, strlen(write_buffer)) < 0) {
					bytes_num = -1;
				}			
			}
		}
		for (int i = last; i < bytes_num + shift; i++) {
			buffer[i - last] = buffer[i];
		}
		shift = bytes_num - last + shift;
		if (bytes_num <= 0) {
			break;
		}
	}
	sprintf(write_buffer, "%d\n", shift);
	if (write_(STDOUT_FILENO, write_buffer, strlen(write_buffer)) < 0) {
		bytes_num = -1;			
	}
	if (bytes_num == 0) {
		return 0;
	} else {
		return 1;
	}
}
	