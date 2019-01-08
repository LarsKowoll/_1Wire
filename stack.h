#ifndef STACK_H_
#define STACK_H_

#include <stdint.h>
#include "general.h"

void stack_init();
void stack_push(int value);
int stack_pop();
int stack_get();

#endif /* STACK_H_ */