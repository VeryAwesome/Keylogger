//d_queue for DUMB QUEUE!

#ifndef _D_QUEUE_H_
#define _D_QUEUE_H_

#include <Windows.h>

void init_queue();

int push(WPARAM data);
WPARAM pop();

void destroy_queue();

#endif