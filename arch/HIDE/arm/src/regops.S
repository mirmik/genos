.global	arm_get_stack_pointer
.type	arm_get_stack_pointer, function

get_stack_pointer:
	mov r0, sp
	bx lr

.global	arm_get_program_counter
.type	arm_get_program_counter, function

get_program_counter:
	mov r0, pc
	bx lr