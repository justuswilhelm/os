// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
.global idt_flush

idt_flush:
  // Get the pointer to the IDT, passed as a parameter.
  mov 4(%esp), %eax
  // Load the IDT pointer.
  lidt (%eax)
  ret

.macro isr_noerrcode name value
  .global isr\name
  isr\name:
    cli
    pushl $0
    pushl \value
    jmp isr_common_stub
.endm

.macro isr_errcode name value
  .global isr\name
  isr\name:
    cli
    pushl \value
    jmp isr_common_stub
.endm

isr_noerrcode 0 $0
isr_noerrcode 1, $1
isr_noerrcode 2, $2
isr_noerrcode 3, $3
isr_noerrcode 4, $4
isr_noerrcode 5, $5
isr_noerrcode 6, $6
isr_noerrcode 7, $7
isr_errcode 8, $8
isr_noerrcode 9, $9
isr_errcode 10, $10
isr_errcode 11, $11
isr_errcode 12, $12
isr_errcode 13, $13
isr_errcode 14, $14
isr_noerrcode 15, $15
isr_noerrcode 16, $16
isr_noerrcode 17, $17
isr_noerrcode 18, $18
isr_noerrcode 19, $19
isr_noerrcode 20, $20
isr_noerrcode 21, $21
isr_noerrcode 22, $22
isr_noerrcode 23, $23
isr_noerrcode 24, $24
isr_noerrcode 25, $25
isr_noerrcode 26, $26
isr_noerrcode 27, $27
isr_noerrcode 28, $28
isr_noerrcode 29, $29
isr_noerrcode 30, $30
isr_noerrcode 31, $31

.extern isr_handler

// This is our common ISR stub. It saves the processor state, sets
// up for kernel mode segments, calls the C-level fault handler,
// and finally restores the stack frame.
isr_common_stub:
  // Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
  pusha
  // Lower 16-bits of eax = ds.
  mov %ds, %ax
  // save the data segment descriptor
  pushl %eax

  // load the kernel data segment descriptor
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  call isr_handler
  // reload the original data segment descriptor
  pop %eax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  // Pops edi,esi,ebp...
  popa
  // Cleans up the pushed error code and pushed ISR number
  add $8, %esp
  sti
  // pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
  iret
