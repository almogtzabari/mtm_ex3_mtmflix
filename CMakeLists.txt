cmake_minimum_required(VERSION 3.10)
project(mtm_ex3_mtmflix C)

set(CMAKE_C_STANDARD 99)

add_executable(mtm_ex3_mtmflix main.c map.h list.h mtmflix.h set.h
        mtmflix.c serie.c serie.h user.c user.h)