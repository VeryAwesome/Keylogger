#include "d_queue.h"

#include <string.h>
#include <stdint.h>

#define MAX_COUNT 8192

uint8_t volatile running;//Fuck visual studio not containing stdbool.h
size_t front, back;
WPARAM data_pool[MAX_COUNT];

void init_queue()
{
	running = 1;
	front = back = 0;
	memset(data_pool, 0, sizeof(data_pool));
}

int push(WPARAM data)
{
	int retval = 0;

	if(!running)
		return -1;

	data_pool[front++] = data;

	if(front > MAX_COUNT)
		front = 0;
}

WPARAM pop()
{
	WPARAM retval = 0;

	if(!running)
		return retval;

	if(back != front)
	{
		retval = data_pool[back++];

		if(back > MAX_COUNT)
			back = 0;
	}
	return retval;
}

void destroy_queue()
{
	running = 0;
}