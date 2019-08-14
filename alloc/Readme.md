# What is this
This is a very simple allocator that can be used in cases where only stack-allocation is available.

As an example, a friend had to implement a protocol for reading data from chips, which could have variable length. But he had to implement it in the firmware of a simple device which did not provide a malloc implementation.
With this header he could have used it to allocate/free the space needed for the data.

# What does it do
There are three steps to using this allocator:
1. initialize the heap: `init_heap(void *free_space, int size)`
2. Allocate some memory: `alloc(int size)`
3. Free the memory when no longer needed `free(void *data)`

The regular restrictions apply
1. Sequential allocs do not return sequetial memory regions.
2. Some memory is needed for book keeping, so not the whole 'size' from the init_heap call can be used to store your data
3. double free'ing is a bad no-no

## Example
```
void main() {
    char space[AMOUNT_OF_ITEMS_MAX * sizeof(testitem) + sizeof(Region) * (AMOUNT_OF_ITEMS_MAX * 2 + 1)];
    init_heap(space, sizeof(space));

    main_with_heap();
}
    
void main_with_heap() {
    void *mydata = alloc(100);
    void *mydata2 = alloc(212);

    //read data into the buffers
    read(src, mydata);
    read(src, mydata2);

    do_something(mydata, mydata2);

    free(mydata);
    free(mydata2);
}
```