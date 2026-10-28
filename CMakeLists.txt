cmake_minimum_required(VERSION 3.20)
project(naiwa_n C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Kernel modules must use Kbuild's compiler flags and generated headers.
execute_process(COMMAND uname -r OUTPUT_VARIABLE KERNEL_RELEASE
                OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
set(KDIR "/lib/modules/${KERNEL_RELEASE}/build" CACHE PATH "Kernel build directory")
find_program(KBUILD_MAKE NAMES gmake make REQUIRED)

if(NOT EXISTS "${KDIR}/include/linux/kconfig.h")
    message(FATAL_ERROR "Kernel headers not found in ${KDIR}; set KDIR to a prepared kernel build tree")
endif()

# Give CLion a C target so its language engine sees kernel headers and definitions.
# This target is only for indexing; the module is still built exclusively by Kbuild.
execute_process(COMMAND uname -m OUTPUT_VARIABLE KERNEL_MACHINE
                OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
if(KERNEL_MACHINE MATCHES "^(x86_64|i[3-6]86)$")
    set(KERNEL_MACHINE x86)
elseif(KERNEL_MACHINE STREQUAL "aarch64")
    set(KERNEL_MACHINE arm64)
endif()
set(KERNEL_ARCH "${KERNEL_MACHINE}" CACHE STRING "Kernel architecture directory for IDE indexing")
add_library(kernel_index OBJECT EXCLUDE_FROM_ALL main.c)
set_target_properties(kernel_index PROPERTIES C_STANDARD 11 C_EXTENSIONS ON)
target_include_directories(kernel_index SYSTEM PRIVATE
    "${KDIR}/arch/${KERNEL_ARCH}/include"
    "${KDIR}/arch/${KERNEL_ARCH}/include/generated"
    "${KDIR}/include"
    "${KDIR}/arch/${KERNEL_ARCH}/include/uapi"
    "${KDIR}/arch/${KERNEL_ARCH}/include/generated/uapi"
    "${KDIR}/include/uapi"
    "${KDIR}/include/generated/uapi")
target_compile_definitions(kernel_index PRIVATE
    __KERNEL__ MODULE KBUILD_BASENAME="main" KBUILD_MODNAME="naiwa_n"
    __KBUILD_MODNAME=naiwa_n)
target_compile_options(kernel_index PRIVATE
    -nostdinc -fno-pie -fno-strict-aliasing -fno-builtin -fms-extensions
    "SHELL:-include '${KDIR}/include/linux/compiler-version.h'"
    "SHELL:-include '${KDIR}/include/linux/kconfig.h'"
    "SHELL:-include '${KDIR}/include/linux/compiler_types.h'")

add_custom_target(naiwa_n ALL
    COMMAND "${KBUILD_MAKE}" -C "${CMAKE_CURRENT_SOURCE_DIR}" "KDIR=${KDIR}" modules
    SOURCES main.c Makefile
    USES_TERMINAL
    VERBATIM)

add_custom_target(module_clean
    COMMAND "${KBUILD_MAKE}" -C "${CMAKE_CURRENT_SOURCE_DIR}" "KDIR=${KDIR}" clean
    USES_TERMINAL
    VERBATIM)

add_custom_target(compile_commands
    COMMAND "${KBUILD_MAKE}" -C "${CMAKE_CURRENT_SOURCE_DIR}" "KDIR=${KDIR}" compile_commands
    USES_TERMINAL
    VERBATIM)
