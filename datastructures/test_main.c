#include "prio.h"
#include <stdio.h>

#define ITEMS 10000000

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

void main()
{
    queue testq;
    init_prio_queue_sized(&testq, ITEMS);

    int idx = 0;
    for (int i = 0; i < ITEMS; i++)
    {
        idx ^= (i * 200) % 43 << (i % 64);
        int x = idx % 1000;
        if (x < 0)
        {
            x = -x;
        }
        push(&testq, (void *)x, x);
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
        pop(&testq, &ret_data);

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
    }
}