.section .text
.global _start
_start:
	mov $2, %edx
	mov $new_msg,%ecx
	mov $0, %ebx
	mov $11, %eax
	int $69

.section .data
new_msg: .ascii "Hello World\n"
