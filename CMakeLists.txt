cmake_minimum_required(VERSION 3.2)
project(kernel C CXX ASM)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99" )
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2" )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/newlib/c++/4.9/" )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/newlib/c++/4.9/arm-none-eabi/" )

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles" )
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/linker.ld")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map,${CMAKE_SOURCE_DIR}/kernel.map")

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )
set( CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    boot.S
    cboot.c
    clib.c
    kernel.cpp
    reg.h
    uart.c
    uart.h
        interrupts.cpp timer.h timer.cpp interrupts.h)

add_executable(kernel.elf ${SOURCE_FILES})

set_source_files_properties(uart.c PROPERTIES COMPILE_FLAGS -O0)

add_custom_command(
        TARGET kernel.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ./kernel.elf -O binary ${CMAKE_SOURCE_DIR}/kernel.img
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Convert the ELF output file to a binary image" )
