.global	get_stack_pointer
.type	get_stack_pointer, function

get_stack_pointer:
	mov r0, sp
	bx lr

.global	get_program_counter
.type	get_program_counter, function

get_program_counter:
	mov r0, pc
	bx lr