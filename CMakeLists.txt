cmake_minimum_required(VERSION 3.10)
project(mtm_ex3_mtmflix C)

set(CMAKE_C_STANDARD 99)

add_executable(mtm_ex3_mtmflix main.c map.h list.h mtmflix.h set.h
        mtmflix.c series.c series.h user.c user.h mtm_ex3.h)

target_link_libraries(mtm_ex3_mtmflix mtm_ex3_mtmflix/libmtm.a)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG")