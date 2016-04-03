CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
CFLAGS=-O2 -nostartfiles -std=gnu99
CXXFLAGS=-O2 -nostartfiles -std=c++11 -fno-exceptions -fno-unwind-tables -I/usr/include/newlib/c++/4.9/ -I/usr/include/newlib/c++/4.9/arm-none-eabi/
DEPS = 
OBJ = boot.o kernel.o cboot.o clib.o uart.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.S $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kernel.elf: $(OBJ)
	$(CC) -T linker.ld -o $@ $^ $(CFLAGS) -lstdc++
	
%.img: %.elf $(DEPS)
	arm-none-eabi-objcopy $< -O binary $@

.PHONY: clean

clean:
	rm -f *.o kernel.elf kernel.img
