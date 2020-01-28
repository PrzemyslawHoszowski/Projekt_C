cmake_minimum_required(VERSION 3.15)
project(Kreator_map)

set(C_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
add_executable(Kreator_map
        funkcje/funkcje.c
        funkcje/funkcje.h
        kreator_map.c
        funkcje/kolejka.c
        funkcje/kolejka.h
        funkcje/wypisanie_do_pliku.c
        funkcje/wypisanie_do_pliku.h)
target_link_libraries(Kreator_map -ljpeg)
