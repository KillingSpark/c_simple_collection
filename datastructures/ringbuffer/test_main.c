#include "ringbuffer.h"

#include "stdio.h"

void main() {
    ringbuffer rb;
    init_ringbuffer_malloc(&rb, 100);

    int data[10] = {1,2,3,3,2,1,3,4,3,3};
    ringbuf_push(&rb, (void*)&data[0]);
    ringbuf_push(&rb, (void*)&data[1]);
    ringbuf_push(&rb, (void*)&data[2]);
    ringbuf_push(&rb, (void*)&data[3]);

    int *ret;
    void **ret_h = (void **)&ret;
    
    ringbuf_pop(&rb, ret_h);
    if (*ret != data[0]){
        printf("Wrong pop 0, wanted: %d got: %d\n", data[0], *ret);
        return;
    }
    ringbuf_pop(&rb, ret_h);
    if (*ret != data[1]){
        printf("Wrong pop 1, wanted: %d got: %d\n", data[1], *ret);
        return;
    }
    ringbuf_pop(&rb, ret_h);
    if (*ret != data[2]){
        printf("Wrong pop 2, wanted: %d got: %d\n", data[2], *ret);
        return;
    }
    ringbuf_pop(&rb, ret_h);
    if (*ret != data[3]){
        printf("Wrong pop 3, wanted: %d got: %d\n", data[3], *ret);
        return;
    }

    printf("SUCCESS\n");

}