#define USE_MALLOC 1

#ifdef USE_MALLOC
#include <malloc.h>
#endif

struct node
{
    void *data;
    int index;
};
typedef struct node node;

struct queue
{
    node *head;
    int len;
    int cap;
    int resizeable;
};
typedef struct queue queue;

const int ERR_OOM = 1;
const int ERR_EMPTY = 2;

void init_prio_queue_sized_workspace(queue *q, int size, void *workspace)
{
    q->len = 0;
    q->cap = size;
    q->resizeable = 0;
    q->head = (node *)workspace;
}

#ifdef USE_MALLOC
void init_prio_queue_sized_malloc(queue *q, int initial_size)
{
    q->len = 0;
    q->cap = initial_size;
    q->resizeable = 1;
    if (initial_size > 0)
    {
        q->head = (node *)malloc(sizeof(node) * initial_size);
    }
}

void init_prio_queue(queue *q)
{
    init_prio_queue_sized_malloc(q, 0);
}

void queue_realloc_increase(queue *q, int factor)
{
    q->cap *= factor;
    void *new_alloc = realloc(q->head, sizeof(node) * q->cap);
    if (new_alloc)
    {
        q->head = (node *)new_alloc;
    }
}

void queue_realloc_decrease(queue *q, int factor)
{
    q->cap /= factor;
    void *new_alloc = realloc(q->head, sizeof(node) * q->cap);
    if (new_alloc)
    {
        q->head = (node *)new_alloc;
    }
}
#endif

int left_child(int pos)
{
    return pos * 2 + 1;
}

int right_child(int pos)
{
    return left_child(pos) + 1;
}

int parent(int pos)
{
    return (pos - 1) / 2;
}

node *get(queue *q, int pos)
{
    return &q->head[pos];
}

void sift_up(queue *q, int pos)
{
    int current_pos = pos;
    node *c = get(q, pos);
    int parent_pos = parent(current_pos);
    node *p = get(q, parent_pos);

    while (p->index > c->index && c != q->head)
    {
        node tmp = *p;
        *p = *c;
        *c = tmp;

        current_pos = parent_pos;
        c = p;
        parent_pos = parent(current_pos);
        p = get(q, parent_pos);
    }
}

int push(queue *q, void *data, int index)
{
    if (q->len == q->cap)
    {
        return ERR_OOM;
    }

    int pos = q->len;
    node *new_node = &(q->head[pos]);
    q->len += 1;
    new_node->data = data;
    new_node->index = index;

    sift_up(q, pos);

    return 0;
}

void sift_down(queue *q, int pos)
{
    int current_pos = pos;
    int left_child_pos = left_child(current_pos);
    int right_child_pos = right_child(current_pos);
    node *c = get(q, current_pos);
    node *l = get(q, left_child_pos);
    node *r = get(q, right_child_pos);

    while ((left_child_pos < q->len && c->index > l->index) || (right_child_pos < q->len && c->index > r->index))
    {
        int chosen_child_pos;
        node *chosen_child;
        if (l->index < r->index)
        {
            chosen_child_pos = left_child_pos;
            chosen_child = l;
        }
        else
        {
            chosen_child_pos = right_child_pos;
            chosen_child = r;
        }

        node tmp = *chosen_child;
        *chosen_child = *c;
        *c = tmp;

        current_pos = chosen_child_pos;
        c = chosen_child;

        left_child_pos = left_child(current_pos);
        right_child_pos = right_child(current_pos);
        l = get(q, left_child_pos);
        r = get(q, right_child_pos);
    }
}

int pop(queue *q, void **target)
{
    if (q->len == 0)
    {
        return ERR_EMPTY;
    }

    *target = q->head->data;

    q->head[0] = q->head[q->len - 1];
    q->len--;

    sift_down(q, 0);

    return 0;
}


void destroy_queue(queue *q)
{
    free(q->head);
}

void heap_sort(int *to_sort, int len)
{
    queue q;
    init_prio_queue_sized_malloc(&q, len);
    for (int i = 0; i < len; i++)
    {
        push(&q, (void *)&to_sort[i], to_sort[i]);
    }
    int *ret;
    void *ret_h = (void *)ret;
    for (int i = 0; i < len; i++)
    {
        pop(&q, &ret_h);
        to_sort[i] = *ret;
    }
    destroy_queue(&q);
}

int check_queue_health(queue *q, int pos)
{
    if (pos >= q->len)
    {
        return 0;
    }

    node *c = get(q, pos);

    int violations = 0;
    if (left_child(pos) < q->len)
    {
        node *l = get(q, left_child(pos));
        if (c->index > l->index)
        {
            violations++;
        }
        violations += check_queue_health(q, left_child(pos));
    }
    if (right_child(pos) < q->len)
    {
        node *r = get(q, right_child(pos));
        if (c->index > r->index)
        {
            violations++;
        }
        violations += check_queue_health(q, right_child(pos));
    }

    return violations;
}
