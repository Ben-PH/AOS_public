#
# Copyright 2019, Data61
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# ABN 41 687 119 230.
#
# This software may be distributed and modified according to the terms of
# the GNU General Public License version 2. Note that NO WARRANTY is provided.
# See "LICENSE_GPLv2.txt" for details.
#
# @TAG(DATA61_GPL)
#
cmake_minimum_required(VERSION 3.7.2)

project(tty_test C)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u __vsyscall_ptr")

add_executable(tty_test EXCLUDE_FROM_ALL C/ttyout.c C/tty_test.c)
target_link_libraries(tty_test muslc sel4 sosapi ${CMAKE_CURRENT_SOURCE_DIR}/target/aarch64-unknown-linux-musl/release/librus_tty_test.a)

# warn about everything
add_compile_options(-Wall -Werror -W -Wextra)

add_app(tty_test)
