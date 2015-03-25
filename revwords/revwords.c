#include <helpers.h>
#include <stdio.h>
#include <unistd.h>

size_t const buff_size = 4097;// (4096) + ' '

int main() {
	char buffer[buff_size];
	char write_buffer[buff_size];
	ssize_t bytes_num = 1;
	int shift = 0;
	while (1) {
		bytes_num = read_until(STDIN_FILENO, buffer + shift, buff_size - shift, ' ');	
		if (bytes_num <= 0) {
			break;
		}
		int last = 0;
		for (int i = shift; i < shift + bytes_num; i++) {
			if (buffer[i] == ' ') {
				int cur = 0;
				while (cur != (i - last)) {
					write_buffer[cur] = buffer[i - cur - 1];
					cur++;
				}
				write_buffer[cur] = ' ';
				cur++;
				last = i + 1;
				if (write_(STDOUT_FILENO, write_buffer, cur) < 0) {
					bytes_num = -1;			
				}
			}
		}
		for (int i = last; i < bytes_num + shift; i++) {
			buffer[i - last] = buffer[i];
		}
		shift = bytes_num - last + shift;
	}
	for (int i = 0; i < shift; i++) {
		write_buffer[i] = buffer[shift - i - 1];
	}
	if (write_(STDOUT_FILENO, write_buffer, shift) < 0) {
		bytes_num = -1;			
	}
	if (bytes_num == 0) {
		return 0;
	} else {
		return 1;
	}
}
	
