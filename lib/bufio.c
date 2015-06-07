#include "bufio.h"

struct buf_t {
    size_t capacity;
    size_t size;
    char *buffer;
};

struct buf_t *buf_new(size_t capacity) {
    struct buf_t *p = malloc(2 * sizeof(size_t) + sizeof(void*) + capacity);
    p->capacity = capacity;
    p->size = 0;
    p->buffer = (char*)p + 2 * sizeof(size_t) + sizeof(void*);
    return p;
}

void buf_free(struct buf_t * p) {
    free(p);
}

size_t buf_capacity(struct buf_t * p) {
    #ifdef DEBUG
        if (buf == NULL) {
            abort();
        }
    #endif
    return p->capacity;
}

size_t buf_size(struct buf_t * p) {
    #ifdef DEBUG
        if (buf == NULL) {
            abort();
        }
    #endif
    return p->size;
}

ssize_t buf_fill(fd_t fd, struct buf_t *buf, size_t required) {
    #ifdef DEBUG
        if (buf == NULL || buf->capacity < required) {
            abort();
        }
    #endif
    ssize_t read_size;
    while (1) {
        read_size = read(fd, buf->buffer + buf->size, required - buf->size);
        if (read_size == -1) {
            return -1;
        }
        buf->size += read_size;
        if (read_size == 0 || buf->size >= required) {
            return buf->size;
        }
    }
}

ssize_t buf_flush(fd_t fd, struct buf_t *buf, size_t required) {
    #ifdef DEBUG
        if (buf == NULL) {
            abort();
        }
    #endif
    ssize_t write_size;
    ssize_t written = 0;
    while (1) {
        write_size = write(fd, buf->buffer + written, buf->size - written);
        if (write_size == -1) {
            return -1;
        }
        written += write_size;
        if (written >= required || buf->size == written) {
            int i;
            for (i = 0; i < buf->size - written; i++) {
                buf[i] = buf[i + written];
            }
            buf->size -= written;
            return written;
        }
    }
}



ssize_t buf_getline(fd_t fd, struct buf_t* buf, char* dest) {
    #ifdef DEBUG
        if (buf == NULL) {
            abort();
        }
    #endif
    int len = 0;
    for (int i = 0; i < buf->size; i++) {
        if (buf->buffer[i] == '\n') {
            for (int j = i + 1; j < buf->size; j++) {
                buf->buffer[j - (i + 1)] = buf->buffer[j];
            }
            buf->size = buf->size - i - 1;
            return len;
        }
        dest[len] = buf->buffer[i];
        len++;
    }
    while (1) {
        size_t read_size = read(fd, buf->buffer, buf->capacity);
        if (read_size == -1) {
            buf->size = 0;
            return -1;
        }
        if (read_size == 0) {
            buf->size = 0;
            return -2;
        }
        buf->size = read_size;
        for (int i = 0; i < buf->size; i++) {
            if (buf->buffer[i] == '\n') {
                for (int j = i + 1; j < buf->size; j++) {
                    buf->buffer[j - (i + 1)] = buf->buffer[j];
                }
                buf->size = buf->size - i - 1;
                return len;
            }
            dest[len] = buf->buffer[i];
            len++;        
        }
    }
}