#include "vector.h"

#include <stdio.h>

void main() {
    vector v;
    init_vector_malloc(&v, 2);

    int x = 1;
    int y = 1337;
    int z = 42;

    vector_push_realloc(&v, &x);
    vector_push_realloc(&v, &y);
    vector_push_realloc(&v, &z);


    int x_ = *((int *)vector_get(&v, 0));
    int y_ = *((int *)vector_get(&v, 1));
    int z_ = *((int *)vector_get(&v, 2));

    if (x != x_) {
        printf("Wrong get x. Should be: %d, got: %d\n", x, x_);
        return;
    }

    if (x != x_) {
        printf("Wrong get y. Should be: %d, got: %d\n", y, y_);
        return;
    }

    if (x != x_) {
        printf("Wrong get z. Should be: %d, got: %d\n", z, z_);
        return;
    }

    printf("SUCCESS\n");
}