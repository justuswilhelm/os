// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
.global gdt_flush

gdt_flush:
  mov 4(%esp), %eax
  lgdt (%eax)

  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss
  // 0x08 is the offset to our code segment: Far jump!
  ljmp $0x08, $.flush

.flush:
  ret
