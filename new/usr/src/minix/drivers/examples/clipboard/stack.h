#ifndef SO_7_STACK_H
#define SO_7_STACK_H

#include <stdint.h>

#define STACK_SIZE 100

typedef struct Stack {
    uint32_t value[STACK_SIZE];
    uint32_t* current;
}* bstack_t;

uint32_t* lower_bound(bstack_t s);

uint32_t* upper_bound(bstack_t s);

void push(int value, bstack_t stack);

int is_empty(const bstack_t stack);

uint32_t top(bstack_t stack);

void pop(bstack_t stack);

void clear(bstack_t stack);

void init(bstack_t stack);

#endif //SO_7_STACK_H
