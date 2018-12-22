cmake_minimum_required(VERSION 3.2)
project(kernel C CXX ASM)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99" )
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2" )
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra" )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra" )

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles" )
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/linker.ld")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map,${CMAKE_SOURCE_DIR}/kernel.map")

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )
set( CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )


set(SOURCE_FILES
    boot.S
    contextSwitch.S
    cboot.c
        clib.cpp
    kernel.cpp
        uart.cpp
    uart.h
        interrupts.cpp timer.h timer.cpp interrupts.h system.h eventloop.h concurrency.h md5.cpp md5.h mmu.cpp mmu.h smp.cpp smp.h fpu.cpp fpu.h process.cpp process.h ProcessTable.cpp ProcessTable.h utils.h
    Register.h Gpio.cpp Gpio.h system.cpp core.cpp core.h coprocessor.h)

add_executable(kernel.elf ${SOURCE_FILES})

add_custom_command(
        TARGET kernel.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ./kernel.elf -O binary ${CMAKE_SOURCE_DIR}/kernel.img
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Convert the ELF output file to a binary image" )
