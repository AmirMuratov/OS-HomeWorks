#include <helpers.h>
#include <stdio.h>

size_t const buff_size = 8192;

int main(int argc, char *argv[]) {
	char buffer[buff_size];
	ssize_t bytes_num = 1;
	int shift = 0;
	char* args[argc];
	for (int i = 0; i < argc - 1; i++) {
		args[i] = argv[i + 1];
	}
	while (1) {
		bytes_num = read_until(STDIN_FILENO, buffer + shift, buff_size - shift, '\n');	
		int last = 0;	
		for (int i = shift; i < shift + bytes_num; i++) {
			if (buffer[i] == '\n') {
				buffer[i] = 0;
				args[argc - 1] = buffer + last;
				int status = spawn(args[0], args);
				buffer[i] = '\n';
				if (status == 0) {
					if (write_(STDOUT_FILENO, buffer + last, i - last + 1) < 0) {
						bytes_num = -1;
						break;
					}
				}
				last = i + 1;
			}
		}
		if (bytes_num <= 0) {
			break;
		}
		for (int i = last; i < bytes_num + shift; i++) {
			buffer[i - last] = buffer[i];
		}
		shift = bytes_num - last + shift;
	}
	args[argc - 1] = buffer;
	buffer[shift] = 0;
	int status = spawn(args[0], args);
	if (status == 0) {
		if (write_(STDOUT_FILENO, buffer, shift) < 0) {
			bytes_num = -1;			
		}
	}
	if (bytes_num == 0) {
		return 0;
	} else {
		return 1;
	}
}