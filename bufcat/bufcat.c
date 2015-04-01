#include <bufio.h>

int main() {
    struct buf_t *buf= buf_new(8192);
    while (1) {
        ssize_t bytes_read = buf_fill(STDIN_FILENO, buf, buf_capacity(buf));
        if (bytes_read == 0) {
            return 0;
        }
        if (bytes_read < 0 || buf_flush(STDOUT_FILENO, buf, bytes_read) == -1) {
            return 1;
        }
    }
}