// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
.global idt_flush

idt_flush:
  // Get the pointer to the IDT, passed as a parameter.
  mov 4(%esp), %eax
  // Load the IDT pointer.
  lidt (%eax)
  ret

.macro isr_noerrcode value
  .global isr\value
  isr\value:
    cli
    pushl $0
    pushl $\value
    jmp isr_common_stub
.endm

.macro isr_errcode value
  .global isr\value
  isr\value:
    cli
    pushl $\value
    jmp isr_common_stub
.endm

isr_noerrcode 0
isr_noerrcode 1
isr_noerrcode 2
isr_noerrcode 3
isr_noerrcode 4
isr_noerrcode 5
isr_noerrcode 6
isr_noerrcode 7
isr_errcode 8
isr_noerrcode 9
isr_errcode 10
isr_errcode 11
isr_errcode 12
isr_errcode 13
isr_errcode 14
isr_noerrcode 15
isr_noerrcode 16
isr_noerrcode 17
isr_noerrcode 18
isr_noerrcode 19
isr_noerrcode 20
isr_noerrcode 21
isr_noerrcode 22
isr_noerrcode 23
isr_noerrcode 24
isr_noerrcode 25
isr_noerrcode 26
isr_noerrcode 27
isr_noerrcode 28
isr_noerrcode 29
isr_noerrcode 30
isr_noerrcode 31

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
