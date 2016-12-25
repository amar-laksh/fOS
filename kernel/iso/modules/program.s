.section .text
.global _start

_start:
	mov $0xDEADBEEF, %eax
	hlt
