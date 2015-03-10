#include <helpers.h>
#include <stdio.h>
#include <unistd.h>

size_t const buff_size = 2048;

int main() {
	char buffer[buff_size];
	
	ssize_t bytes_num = read_(STDIN_FILENO, buffer, buff_size);
	while (bytes_num > 0) {
		if (write_(STDOUT_FILENO, buffer, bytes_num) < 0) {
			break;			
		}
		bytes_num = read_(STDIN_FILENO, buffer, buff_size);	
	}
	if (bytes_num == 0) {
		return 0;
	} else {
		return 1;
	}
}
	