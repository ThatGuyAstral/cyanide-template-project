
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = Objects/boot.o \
          Objects/gdt.o \
          Objects/driver.o \
          Objects/port.o \
          Objects/interruptstubs.o \
          Objects/interrupts.o \
          Objects/keyboard.o \
          Objects/mouse.o \
          Objects/kernel.o

Objects/%.o: %.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

Objects/%.o: Drivers/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

Objects/%.o: Hardware/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

Objects/%.o: Hardware/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

Objects/%.o: %.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

kernel.bin: link.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

Cyanide.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0'                     >> iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "Cyanide" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=Cyanide.iso iso
	rm -rf iso

.PHONY: clean
clean:
	rm -rf Objects kernel.bin Cyanide.iso