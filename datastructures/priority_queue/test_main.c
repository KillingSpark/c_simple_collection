#include "prio.h"
#include <stdio.h>

#define ITEMS 1000

void print_queue(queue *q)
{
    int next_break = 1;
    for (int i = 0; i < q->len; i++)
    {
        if (i == next_break - 1)
        {
            printf("\n");
            next_break *= 2;
        }
        printf("%d, ", get(q, i)->index);
    }
    printf("\n");
    printf("\n");
}

void test_sort()
{
    int data[10] = {2,7,4,3,3,56,7,345,45,456};
    heap_sort(data, 10);
    for(int i = 1; i < 10; i++){
        if (data[i-1] > data[i]) {
            printf("NOT SORTED!");
            return;
        } 
    }
    printf("SORT: SUCCESS!\n");
}

void test_queue()
{
    queue testq;
    init_prio_queue_sized_malloc(&testq, ITEMS/2);

    int idx = 0;
    for (int i = 0; i < ITEMS/2; i++)
    {
        idx ^= (i * 200) % 43 << (i % 64);
        int x = idx % 1000;
        if (x < 0)
        {
            x = -x;
        }
        int err = push(&testq, (void *)x, x);
        if (err) {
            printf("OOM error!");
            return;
        }
    }

    queue_realloc_increase(&testq, 2);
    for (int i = 0; i < ITEMS/2; i++)
    {
        idx ^= (i * 200) % 43 << (i % 64);
        int x = idx % 1000;
        if (x < 0)
        {
            x = -x;
        }
        int err = push(&testq, (void *)x, x);
        if (err) {
            printf("OOM error!");
            return;
        }
    }

    printf("\n");

    int health = check_queue_health(&testq, 0);
    if (health > 0)
    {
        printf("Violations found: %d\n", health);
        print_queue(&testq);
        return;
    }

    void *ret_data;
    void *prev_ret_data = -1;

    for (int i = 0; i < ITEMS; i++)
    {
        int err = pop(&testq, &ret_data);
        if (err) {
            printf("Empty queue error!");
            return;
        }

        if ((int)prev_ret_data > (int)ret_data)
        {
            printf("Returned smaller value than previous, but this is a min queue! Now: %d, Last: %d, Iteration: %d\n", (int)ret_data, (int)prev_ret_data, i);
            return;
        }
        prev_ret_data = ret_data;
    }

    if (testq.len != 0)
    {
        printf("elements in queue: %d, should be: 0\n", testq.len);
        return;
    }

    printf("QUEUE: SUCCESS!\n");
}


void main() {
    test_queue();
    test_sort();
}