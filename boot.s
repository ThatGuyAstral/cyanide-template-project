.set MAGIC, 0x1BADB002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.section .text
.extern kmain
.global boot
boot:
  mov $kstack, %esp
  call kmain
_stop:
  cli
  hlt
  jmp _stop
.section .bss
.space 2*1024*1024
kstack:
