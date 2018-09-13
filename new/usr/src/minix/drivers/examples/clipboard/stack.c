#include <stdlib.h>
#include <assert.h>
#include "stack.h"

uint32_t* lower_bound(bstack_t s) {
    return &s->value[0];
}

uint32_t* upper_bound(bstack_t s) {
    return &s->value[STACK_SIZE];
}

void push(int value, bstack_t stack) {
    if(stack->current != upper_bound(stack)) {
        *(stack->current) = value;
        stack->current++;
    }
}

int is_empty(const bstack_t stack) {
    return stack->current == lower_bound(stack);
}

uint32_t top(bstack_t stack) {
    assert(!is_empty(stack));
    uint32_t* ptr = stack->current;
    return *(--ptr);
}

void pop(bstack_t stack) {
    --stack->current;
}

void init(bstack_t stack) {
    for(uint32_t i = 0; i < STACK_SIZE; i++) {
        stack->value[i] = i;
    }
    stack->current = upper_bound(stack);
}

void clear(bstack_t stack) {
    stack->current = lower_bound(stack);
}
