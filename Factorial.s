@@ Factorial.s :
@@ Test code for STM32 and linking assembly to C
 

    .code   16
    .text                   @@ - Code section (text -> ROM)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global factorial          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   factorial, %function   @@ - symbol type (not req)
@@ Declaration : int factorial(int x)
@@ Uses r0 for param 0
@@   r0: x
factorial:
    	PUSH {r4,lr}
	MOV r4,r0
	CMP r4,#0x01
	BNE NZ
	MOVS r0,#0x01
loop:	POP {r4,pc}
NZ:	SUBS r0,r4,#1
	BL factorial
	MUL r0,r4,r0
	B loop
stop: 	B stop
    .size   factorial, .-factorial    @@ - symbol size (not req)
