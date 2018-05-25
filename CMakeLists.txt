cmake_minimum_required(VERSION 3.10)
project(mtm_ex3_mtmflix C)

set(CMAKE_C_STANDARD 99)
add_executable(mtm_ex3_mtmflix main.c mtmflix.c user.h set.h list.h
        user.c series.h series.c mtm_ex3.h mtmflix.h map.h utilities.c
        utilities.h ranked_series.c ranked_series.h)
target_link_libraries(mtm_ex3_mtmflix ${CMAKE_SOURCE_DIR}/libmtm.a
        ${CMAKE_SOURCE_DIR}/libmtm_ex3.a)
#set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic-errors -DNDEBUG")