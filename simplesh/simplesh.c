#define _GNU_SOURCE
#include <helpers.h>
#include <bufio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

size_t const BUFF_SIZE = 4096;
int const MAX_NUMBER_OF_ARGS = 256;

int normalize_str(char* str, int *len) {
	int cur = 0;
	int n = 1;
	int p = 0;
	while (str[p] == ' ') p++;
	(*len)--;
	while (str[*len] == ' ') (*len)--;
	(*len)++;
	str[0] = str[p];
	for (int i = p + 1; i < *len; i++) {
		if (str[i] == ' ') {
			if (str[cur] != 0 && str[cur] != '|') {
				cur++;
				str[cur] = 0;
			}
		} 
		else if (str[i] == '|') {
			if (str[cur] == 0) {
				str[cur] = '|';
			} else {
				cur++;
				str[cur] = '|';
			}
			n++;
		} 
		else {
			cur++;
			str[cur] = str[i];
		}
	}
	str[cur + 1] = '|';
	*len = cur + 2;
	return n;
}

void parse(char* str, int len, char **result) {
	int n = 1;
	for (int i = 0; i < len; i++) {
		if (str[i] == 0) {
			n++;
		}
	}
	result[0] = str;
	result[n] = NULL;
	int c = 1;
	for (int i = 0; i < len; i++) {
		if (str[i] == 0) {
			result[c] = str + i + 1;
			c++;
		}
	}
}
	
void sig_handler(int sig) {

}

int main() {
	struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = &sig_handler;
	
	if (sigaction(SIGINT, &act, NULL) < 0) {
        return 1;
    }
		

	struct buf_t *in = buf_new(BUFF_SIZE);
	char buf[BUFF_SIZE];
	while (1) {
		if (write(STDOUT_FILENO, "$", 1) < 0) {
			return 1;
		}
		int bytes_read = buf_getline(STDIN_FILENO, in, buf);
		//printf("%s\n%d\n", buf, bytes_read);
		if (bytes_read == 0) {
			return 0;
		}
		if (bytes_read < 0) {
			return 1;
		}
		int n = normalize_str(buf, &bytes_read);
		char* args[n][MAX_NUMBER_OF_ARGS];
		int last = 0;
		int c = 0;
		for (int i = 0; i < bytes_read; i++) {
			if (buf[i] == '|') {
				buf[i] = 0;
				parse(buf + last, i - last, args[c]);
				c++;
				last = i + 1;			
			}
		}
		/*printf("%d\n", n);
		for (int i = 0; i < n; i++) {
			int j = 0;
			while (args[i][j] != NULL) {
				printf("%s\n", args[i][j]);
				j++;
			}
			printf("------------\n");
		}
		printf("------------\n");
		*/
		execargs_t* programs[n];
		for (int i = 0; i < n; i++) {
			programs[i] = execargs_t_new(args[i]);
		}
		runpiped(programs, n);
		for (int i = 0; i < n; i++) {
			execargs_t_free(programs[i]);
		}
	}
	buf_free(in);
}
	