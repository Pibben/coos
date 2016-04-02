CC=arm-none-eabi-gcc
CFLAGS=-O2 -nostartfiles -std=gnu99
DEPS = 
OBJ = boot.o kernel.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.S $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kernel.elf: $(OBJ)
	$(CC) -T linker.ld -o $@ $^ $(CFLAGS)
	
%.img: %.elf $(DEPS)
	arm-none-eabi-objcopy $< -O binary $@

.PHONY: clean

clean:
	rm -f *.o kernel.elf kernel.img
