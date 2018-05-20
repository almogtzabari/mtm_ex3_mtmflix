cmake_minimum_required(VERSION 3.10)
project(hw3 C)

set(CMAKE_C_STANDARD 99)
add_executable(hw3 main.c mtmflix.c user.h user.c series.h series.c mtm_ex3.h mtmflix.h map.h)
target_link_libraries(hw3 ${CMAKE_SOURCE_DIR}/libmtm.a ${CMAKE_SOURCE_DIR}/libmtm_ex3.a)
#set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic-errors -DNDEBUG")