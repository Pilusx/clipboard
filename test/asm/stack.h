//
// Created by bartek on 7/29/18.
//

#ifndef SO_7_STACK_H
#define SO_7_STACK_H

#include <stdint.h>

#define STACK_SIZE 100

typedef struct Stack {
    uint32_t value[STACK_SIZE];
    uint32_t* current;
}* stack_t;

uint32_t* lower_bound(stack_t s);

uint32_t* upper_bound(stack_t s);

void push(int value, stack_t stack);

int is_empty(const stack_t stack);

uint32_t top(stack_t stack);

void pop(stack_t stack);

void clear(stack_t stack);

void init(stack_t stack);

#endif //SO_7_STACK_H
