/* Include the sbrk function */

#include <unistd.h>

#define true 1;
#define false 0;
typedef int bool;

bool has_initialized = false;
void *managed_memory_start;
void *last_valid_address;


void malloc_init()
{	
	last_valid_address = sbrk(0);
	managed_memory_start = last_valid_address;
	has_initialized = 1;
}

struct mem_control_block
{
	bool is_available;
	int size;
};


void free(void *firstbyte)
{
	struct mem_control_block *mcb;

	mcb = firstbyte - sizeof(struct mem_control_block);
	mcb->is_available = 1;
}


void* malloc(long numbytes)
{
	void *current_location;

	struct mem_control_block *current_location_mcb;

	void *memory_location;

	if (!has_initialized)
	{
		malloc_init();
	}

	numbytes = numbytes + sizeof(struct mem_control_block);

	memory_location = 0;

	current_location = managed_memory_start;

	while (current_location != last_valid_address)
	{
		current_location_mcb = (struct mem_control_block *)current_location;

		if (current_location_mcb->is_available)
		{
			if (current_location_mcb->size >= numbytes)
			{
				current_location_mcb->is_available = 0;

				memory_location = current_location;

				break;
			}
		}

		current_location = current_location + current_location_mcb->size;
	}

	if (!memory_location)
	{
		sbrk(numbytes);

		memory_location = last_valid_address;
		last_valid_address = last_valid_address + numbytes;

		current_location_mcb = memory_location;
		current_location_mcb->is_available = 0;
		current_location_mcb->size = numbytes;
	}

	memory_location = memory_location + sizeof(struct mem_control_block);

	return memory_location;
}