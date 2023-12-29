#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>

#define MIN 5
#define LEVELS 8

enum flag
{
    Free,
    Taken
};

// memory bloc
typedef struct Block
{
    enum flag status;
    size_t size;
    struct Block *next;
} Block;

// memory manager struct
typedef struct BuddyAllocator
{
    Block *memory;
    Block *freeLists[LEVELS];
} BuddyAllocator;

// occuped bloc size
typedef struct TakenBlockList
{
    Block *memory;
    Block *TakenLists[256];
} TakenBlockList;


/**
 * @brief init allocator
 *
 * @param totalSize entry allocation size
 * @return void*
 */
void *initAllocator(size_t totalSize)
{
    size_t taken_size = sizeof(Block);
    Block *memory = mmap(NULL, totalSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (memory == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // init first bloc size
    memory->status = Free;
    memory->size = totalSize;
    memory->next = NULL;

    // init allocator struct
    BuddyAllocator *allocator;
    allocator->memory = memory;

    // init free blocks list
    for (int i = 0; i < LEVELS; i++)
        allocator->freeLists[i] = NULL;

    return allocator;
}

// allocator method
void *buddyMalloc(BuddyAllocator *allocator, size_t size)
{
    // Find the smallest block size that can hold the requested size
    int level = MIN;
    while ((1 << level) < size + sizeof(Block))
    {
        level++;
        if (level >= LEVELS)
        {
            return NULL; // The requested size is too large
        }
    }

    // Find a free block of the appropriate size
    for (int i = level; i < LEVELS; i++)
    {
        if (allocator->freeLists[i] != NULL)
        {
            Block *block = allocator->freeLists[i];
            allocator->freeLists[i] = block->next; // Remove from list of free blocks

            /* Split the block if necessary: ​​The objective is to split the current block into smaller blocks until reaching the desired level.
            Each iteration of the loop creates a companion block that is freed and added to the list of free blocks. */
            while (i > level)
            {
                i--;
                size_t blockSize = 1 << i;
                Block *buddy = (Block *)((char *)block + blockSize);
                buddy->status = Free; // We make the neighbor Free
                buddy->size = (1 << i);
                buddy->next = allocator->freeLists[i];
                allocator->freeLists[i] = buddy;
            }

            block->status = Taken;
            size_t blockSize = sizeof(Block);

            return (void *)((char *)block + blockSize);
        }
    }

    return NULL; // No free block of the required size is available
}
