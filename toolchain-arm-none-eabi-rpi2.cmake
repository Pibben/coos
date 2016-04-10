
include(CMakeForceCompiler)

set( CMAKE_SYSTEM_NAME          Generic )
set( CMAKE_SYSTEM_PROCESSOR     BCM2836 )

set( TC_PATH "" )

set( CROSS_COMPILE arm-none-eabi- )

CMAKE_FORCE_C_COMPILER( ${TC_PATH}${CROSS_COMPILE}gcc GNU )
CMAKE_FORCE_CXX_COMPILER( ${TC_PATH}${CROSS_COMPILE}g++ GNU )

set( CMAKE_OBJCOPY      ${TC_PATH}${CROSS_COMPILE}objcopy
    CACHE FILEPATH "The toolchain objcopy command " FORCE )

# Set the CMAKE C flags (which should also be used by the assembler!
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon-vfpv4" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=hard" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=cortex-a7" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon-vfpv4" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv7-a" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=cortex-a7" )

set( CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mfpu=neon-vfpv4" )
set( CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mfloat-abi=hard" )
set( CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -march=armv7-a" )
set( CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mtune=cortex-a7" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "" )
set( CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}" CACHE STRING "" )

add_definitions( -DRPI=2 )
