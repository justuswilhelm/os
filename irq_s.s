// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
// irq_number (IRQ Number) is a number between 0-15
// ir_number (Interrupt Number) is a number between 32-47
.macro irq irq_number  ir_number
  .global irq\irq_number
  irq\irq_number:
    cli
    pushl $0
    pushl $\ir_number
    jmp irq_common_stub
.endm

irq 0, 32
irq 1, 33
irq 2, 34
irq 3, 35
irq 4, 36
irq 5, 37
irq 6, 38
irq 7, 39
irq 8, 40
irq 9, 41
irq 10, 42
irq 11, 43
irq 12, 44
irq 13, 45
irq 14, 46
irq 15, 47

.extern irq_handler

// This is our common IRQ stub. It saves the processor state, sets
// up for kernel mode segments, calls the C-level fault handler,
// and finally restores the stack frame.
irq_common_stub:
  // Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
  pusha

  // Lower 16-bits of eax = ds.
  mov %ds, %ax
  // save the data segment descriptor
  push %eax

  // load the kernel data segment descriptor
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  call irq_handler

  // reload the original data segment descriptor
  pop %ebx
  mov %bx, %ds
  mov %bx, %es
  mov %bx, %fs
  mov %bx, %gs
  // Pops edi,esi,ebp...
  popa
  // Cleans up the pushed error code and pushed ISR number
  add $8, %esp
  sti
  // pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
  iret
