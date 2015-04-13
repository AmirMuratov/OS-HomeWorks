#include <bufio.h>
#include <helpers.h>
#include <stdio.h>

size_t const buff_size = 8192;

int main(int argc, char *argv[]) {
	char* args[argc];
	for (int i = 0; i < argc - 1; i++) {
		args[i] = argv[i + 1];
	}
	
	struct buf_t* in= buf_new(buff_size);
    struct buf_t* out= buf_new(buff_size);
    char cur[buff_size];
    int ok = 0;
    while (1) {
		ssize_t len = buf_getline(STDIN_FILENO, in, cur);	
		if (len == -1) {
			ok = -1;
			break;
		}
		if (len == 0) {
			break;
		}
		args[argc - 1] = cur;
		int status = spawn(args[0], args);
		if (status == 0) {
			cur[len] = '\n';
			ssize_t res = buf_write(STDOUT_FILENO, out, cur, len + 1);
			if (res < 0) {
				ok = -1;
				break;
			}
		}
	}
	ssize_t res = buf_flush(STDOUT_FILENO, out, 1);
	if (res < 0) {
		ok = -1;
	} 
	return ok;
}