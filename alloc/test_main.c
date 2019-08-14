#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"

#define AMOUNT_OF_ITEMS_MAX 10000
#define SEED 1635219128
#define LOOPS 100000

typedef struct
{
    int a;
    int b;
} testitem;

void print_list(int verbose)
{
    printf("#####\n");
    printf("Heap start: %p\n", heap);
    if (!verbose)
    {
        printf("Header addr: %p, free: %d\n", free_list, free_list->free_after_this);
    }
    int nodes = 0;
    int free_total = 0;
    for (Region *reg = free_list; reg != 0; reg = reg->next)
    {
        if (verbose)
        {
            if (next_addr_after_region(reg) == reg->next)
            {
                printf("MERGEABLE!\t");
            }
            else
            {
                printf("NOTMERGEABLE! Gap: %ld\t", next_addr_after_region(reg) - (void *)reg->next);
            }
            printf("Node addr: %p, free: %d\n", reg, reg->free_after_this);
        }
        nodes++;
        free_total += reg->free_after_this;
    }
    printf("nodes: %d\n", nodes);
    printf("free total: %d\n", free_total);
    printf("#####\n");
}

int main()
{
    char space[AMOUNT_OF_ITEMS_MAX * sizeof(testitem) + sizeof(Region) * (AMOUNT_OF_ITEMS_MAX * 2 + 1)];
    init_heap(space, sizeof(space));
    testitem *items[AMOUNT_OF_ITEMS_MAX];
    testitem correct_items[AMOUNT_OF_ITEMS_MAX];

    int rand = SEED;
    int items_allocated = 0;

    for (int i = 0; i < AMOUNT_OF_ITEMS_MAX; i++)
    {
        items[i] = 0;
    }

    for (int i = 0; i < LOOPS; i++)
    {
        //print_list(1);
        rand += rand % (AMOUNT_OF_ITEMS_MAX + 1);
        rand += rand % (AMOUNT_OF_ITEMS_MAX + 2);

        int r = rand % AMOUNT_OF_ITEMS_MAX;
        r = r < 0 ? -r : r;
        printf("r: %d, ", r);

        if (items[r])
        {
            printf("Free\n");
            if (items[r]->a != correct_items[r].a)
            {
                printf("Failed\n");
                return 1;
            }

            if (items[r]->b != correct_items[r].b)
            {
                printf("Failed\n");
                return 1;
            }

            free(items[r]);
            items_allocated--;
            items[r] = 0;
        }
        else
        {
            printf("Alloc\n");
            testitem *tmp = alloc(sizeof(testitem));
            if (tmp)
            {
                items_allocated++;
                items[r] = tmp;
                items[r]->a = rand;
                items[r]->b = (rand * 2) % 3;

                correct_items[r].a = rand;
                correct_items[r].b = (rand * 2) % 3;
            }
            else
            {
                printf("Alloc failed\n");
                printf("Items alloced: %d\n", items_allocated);
                print_list(1);
                return 0;
            }
        }
    }

    for (int i = 0; i < AMOUNT_OF_ITEMS_MAX; i++)
    {
        if (items[i])
        {
            free(items[i]);
        }
    }

    int orig_size_off = free_list->free_after_this - (sizeof(space) - sizeof(Region));
    printf("original size after freeing all: %s\n", (orig_size_off == 0) ? "true" : "false");
    if (orig_size_off != 0)
    {
        printf("Should be: %ld", sizeof(space) - sizeof(Region));
        print_list(1);
    }
}