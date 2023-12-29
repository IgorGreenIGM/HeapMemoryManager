#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#include "../include/constants.h"


extern void exit(int);

#define MIN_BLOCK_SIZE 10
#define VMA_MIN_SIZE 100 * Mo

bool is_heap_init = false;
bool vma_end_reached = false;

void* vma_end;
void* vma_start;
void* last_valid_addr;

struct block_controller
{
    state _state;
    size_t size;
};

void malloc_init()
{
    vma_start = mmap(NULL, VMA_MIN_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (vma_start == MAP_FAILED || vma_start == NULL)
    {
        fprintf(stderr, "cannot initialize the heap vma!\n");
        exit(-1);
    }
    else
    {
        last_valid_addr = vma_start;
        vma_end = (void*)((uint8_t*)vma_start + VMA_MIN_SIZE);
        is_heap_init = true;
    }
}


void free(void* ptr)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "warning, freeing a null pointer\n");
        return;
    }

    if (ptr - vma_start >= 0 && ptr - vma_end < 0)
    {
        struct block_controller *bc = (struct block_controller*)((uint8_t*)ptr - sizeof(struct block_controller));
        bc->_state = _free;
    }
    else
    {
        fprintf(stderr, "ptr access violation\n");
        //exit(-1);
    }
}

void* malloc(size_t numbytes)
{
    static const size_t BLOCK_CONTROLLER_SIZE = sizeof(struct block_controller);
    
    size_t space_required = numbytes + BLOCK_CONTROLLER_SIZE; 
    size_t space_available = vma_end - last_valid_addr; 

    if (!is_heap_init)
        malloc_init();

    if (!vma_end_reached)
    {
        if (space_available < space_required)
        {
            vma_end_reached = true;
            if (space_available >= BLOCK_CONTROLLER_SIZE + MIN_BLOCK_SIZE)
            {
                struct block_controller* bc = (struct block_controller*)last_valid_addr;
                bc->size = vma_end - (void*)((uint8_t*)last_valid_addr + BLOCK_CONTROLLER_SIZE);
                bc->_state = _free;

                last_valid_addr = vma_end;
            }

            return NULL;
        }
        else
        {
            struct block_controller* bc = (struct block_controller*)last_valid_addr;
            bc->_state = _taken;
            bc->size = numbytes;
            last_valid_addr = (void*)((uint8_t*)last_valid_addr + space_required);

            return (void*)((uint8_t*)bc + BLOCK_CONTROLLER_SIZE); 
        }
    }
    else
    {
        void* ptr = vma_start;
        while(ptr != last_valid_addr)
        {
            struct block_controller* bc = (struct block_controller*)ptr;
            
            if (bc->_state == _taken)
                ptr = (void*)((uint8_t*)ptr + BLOCK_CONTROLLER_SIZE + bc->size);
            
            else
            {
                if (bc->size < numbytes)
                    ptr = (void*)((uint8_t*)ptr + BLOCK_CONTROLLER_SIZE + bc->size);

                else
                {
                    bc->_state = _taken;
                    if ((bc->size - numbytes) >= (BLOCK_CONTROLLER_SIZE + MIN_BLOCK_SIZE))
                    {
                        struct block_controller* tmp_bc = (struct block_controller*)(ptr + BLOCK_CONTROLLER_SIZE + numbytes);
                        tmp_bc->_state = _free;
                        tmp_bc->size = bc->size - space_required;
                        bc->size = numbytes;
                    }

                    return (void*)((uint8_t*)ptr + BLOCK_CONTROLLER_SIZE); 
                }     
            }
        }

        void* next_end = sbrk(1 * Mo + numbytes);
        if (next_end == NULL)
            return NULL;

        else
        {
            struct block_controller* nbc =  (struct block_controller*)last_valid_addr;
            nbc->_state = _taken;
            nbc->size = numbytes;

            void* to_return = (void*)((uint8_t*)last_valid_addr + BLOCK_CONTROLLER_SIZE);
            last_valid_addr = (void*)((uint8_t*)last_valid_addr + BLOCK_CONTROLLER_SIZE + numbytes); 
            vma_end = next_end;

            return to_return;
        }
    }
}