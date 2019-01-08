
#include "general.h"
#include "stack.h"

int pointer;
int array[20];

void stack_init() {
	pointer = 0;
	
	int get, pop;
	
	stack_push(3);
	pop = stack_pop();
	stack_push(5);
	stack_push(2);
	get = stack_get();
	pop = stack_pop();
	pop = stack_pop();
	pop = stack_pop();
	pop = stack_pop();
}

void stack_push(int value) {
	array[pointer] = value;
	pointer++;
}

int stack_pop() {
	pointer--;
	if (pointer < 0) {
		pointer = 0;
	}
	return array[pointer];
}

int stack_get() {
	return array[pointer - 1];
}