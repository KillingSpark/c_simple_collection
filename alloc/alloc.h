typedef struct region_t
{
    int free_after_this;
    struct region_t *next;
} Region;

void *heap = 0;
Region *free_list = 0;

void *region_to_data(Region *reg)
{
    return ((void *)reg) + sizeof(Region);
}

void *region_for_data(void *data)
{
    return (Region *)(data - sizeof(Region));
}

void *next_addr_after_region(Region *reg)
{
    return region_to_data(reg) + reg->free_after_this;
}

int init_heap(void *free_space, int size)
{
    heap = free_space;
    for (int i = 0; i < size; i++)
    {
        *((char *)heap + i) = 0;
    }
    free_list = (Region *)heap;
    free_list->free_after_this = size - sizeof(Region);
    free_list->next = 0;

    return 0;
}

//returns pointer to the new region that was cut from end of the region
Region *make_region(Region *reg, int size)
{
    reg->free_after_this -= size;
    Region *new_reg = (Region *)(next_addr_after_region(reg));
    new_reg->free_after_this = size - sizeof(Region);
    
    new_reg->next = 0;
    
    return new_reg;
}

//currently uses "first fit" strategy
void *alloc(int size)
{
    int size_with_reg = size + sizeof(Region);
    //perfect region fit
    if (free_list->free_after_this == size)
    {
        Region *result = free_list;
        free_list = free_list->next;
        return region_to_data(result);
    }

    //can create new region in this region
    if (free_list->free_after_this >= size_with_reg)
    {
        Region *new_reg = make_region(free_list, size_with_reg);
        return region_to_data(new_reg);
    }

    for (Region *reg = free_list; reg->next != 0; reg = reg->next)
    {
        //perfect region fit
        if (reg->next->free_after_this == size)
        {
            Region *result = reg->next;
            reg->next = reg->next->next;
            return region_to_data(result);
        }

        //can create new region in this region
        if (reg->next->free_after_this >= size_with_reg)
        {
            Region *new_reg = make_region(reg->next, size_with_reg);
            return region_to_data(new_reg);
        }
    }
    return 0;
}

void free(void *data)
{
    Region *free_reg = (Region *)region_for_data(data);

    //if the list is currently empty
    if (free_list == 0)
    {
        free_list = free_reg;
        free_reg->next = 0;
        return;
    }

    //if the free'd region is on a location before free_list that means it is the new header in the list
    if (free_list > free_reg)
    {
        if (next_addr_after_region(free_reg) == free_list)
        {
            //merge
            free_reg->free_after_this += free_list->free_after_this + sizeof(Region);
            free_reg->next = free_list->next;
            free_list = free_reg;
        }
        else
        {
            free_reg->next = free_list;
            free_list = free_reg;
        }
        return;
    }

    for (Region *reg = free_list; reg < free_reg; reg = reg->next)
    {
        //free'd region is adjacent to "reg" -> just expand reg
        if (next_addr_after_region(reg) == free_reg)
        {
            //printf("Merged: %d + %d\n", reg->free_after_this, free_reg->free_after_this);
            reg->free_after_this += free_reg->free_after_this + sizeof(Region);

            //check if the now bigger region can be merged with the next one
            if ((Region *)next_addr_after_region(reg) == reg->next)
            {
                //printf("Merged with next\n");
                reg->free_after_this += reg->next->free_after_this + sizeof(Region);
                reg->next = reg->next->next;
            }
            return;
        }
        else
        {
            //check if we can merge with the region after free_region
            if ((Region *)next_addr_after_region(free_reg) == reg->next)
            {
                free_reg->free_after_this += reg->next->free_after_this + sizeof(Region);
                free_reg->next = reg->next->next;
                reg->next = free_reg;
                return;
            }
        }

        //need to insert this region into the list without merging
        if (reg->next > free_reg || reg->next == 0)
        {
            //printf("Inserted: %d\n", free_reg->free_after_this);
            free_reg->next = reg->next;
            reg->next = free_reg;
            return;
        }
    }
}

