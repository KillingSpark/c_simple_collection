
typedef struct ringbuffer {
    void **data;
    int cap;
    int len;
    int zero_idx;
} ringbuffer;

void init_ringbuffer_workspace(ringbuffer *rb, int size, void **workspace) {
    rb->cap = size;
    rb->data = workspace;
    rb->zero_idx = 0;
    rb->len = 0;
}

#define USE_MALLOC 1

#ifdef USE_MALLOC 
#include <malloc.h>
void init_ringbuffer_malloc(ringbuffer *rb, int size) {
    void **workspace = (void**)malloc(sizeof(void*) * size);
    init_ringbuffer_workspace(rb, size, workspace);
}
#endif

int ERR_OOM = 1;

int ringbuf_push(ringbuffer *rb, void *element) {
    if (rb->len == rb->cap) {
        return ERR_OOM;
    }

    int idx = (rb->zero_idx + rb->len) % rb->cap;
    rb->data[idx] = element;
    rb->len++;
}

int ringbuf_pop(ringbuffer *rb, void** target) {
    *target = rb->data[rb->zero_idx];

    rb->len--;
    rb->zero_idx++;
}