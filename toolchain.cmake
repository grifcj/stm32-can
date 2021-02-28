set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

if (NOT EXISTS $ENV{TOOLCHAIN_DIR})
   message(FATAL_ERROR "TOOLCHAIN_DIR must be specified")
else()
   set(TOOLCHAIN_DIR $ENV{TOOLCHAIN_DIR})
endif()

find_program(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc)
if (NOT CMAKE_C_COMPILER)
   message(FATAL_ERROR "Can't find C compiler")
endif()

find_program(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-none-eabi-g++)
if (NOT CMAKE_CXX_COMPILER)
   message(FATAL_ERROR "Can't find C++ compiler")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(archFlags -mthumb -mcpu=${CMAKE_SYSTEM_PROCESSOR} -mfloat-abi=hard -mfpu=fpv4-sp-d16)

add_compile_options(
   ${archFlags}
   -Os
   $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
   $<$<COMPILE_LANGUAGE:CXX>:-fno-unwind-tables>
   $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
   --specs=nano.specs
   -ffunction-sections
   -fdata-sections)
add_link_options(
   ${archFlags}
   --specs=nano.specs
   -Wl,--gc-sections
   -Wl,--print-memory-usage)
