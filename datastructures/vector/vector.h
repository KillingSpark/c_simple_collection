typedef struct vector
{
    void **data;
    int len;
    int cap;
} vector;

void init_vector_workspace(vector *v, int size, void **workspace)
{
    v->cap = size;
    v->data = workspace;
    v->len = 0;
}

#define USE_MALLOC 1

#ifdef USE_MALLOC
#include <malloc.h>
void init_vector_malloc(vector *v, int size)
{
    void **workspace = (void **)malloc(sizeof(void *) * size);
    init_vector_workspace(v, size, workspace);
}

void realloc_vector(vector *v, int factor)
{
    void *new_alloc = realloc(v->data, sizeof(void *) * v->cap * factor);
    if (new_alloc)
    {
        v->cap *= factor;
        v->data = (void **)new_alloc;
    }
}

void realloc_vector_smaller(vector *v, int factor)
{
    void *new_alloc = realloc(v->data, sizeof(void *) * v->cap / factor);
    if (new_alloc)
    {
        v->cap /= factor;
        v->data = (void **)new_alloc;
    }
}
#endif

int ERR_OOM = 1;
int ERR_EMPTY = 1;

int vector_push(vector *v, void *element)
{
    if (v->len == v->cap)
    {
        return ERR_OOM;
    }

    v->data[v->len] = element;
    v->len++;

    return 0;
}

int vector_push_realloc(vector *v, void *element) {
    if(vector_push(v, element) == ERR_OOM) {
        realloc_vector(v, 2);
        return vector_push(v,element);
    }
    return 0;
}

int vector_pop(vector *v, void** target) {
    if (v->len == 0){
        return ERR_EMPTY;
    }

    v->len--;
    *target = v->data[v->len];
    return 0;
}

void *vector_get(vector *v, int idx) {
    if(v->len <= idx) {
        return 0;
    }
    return v->data[idx];    
}