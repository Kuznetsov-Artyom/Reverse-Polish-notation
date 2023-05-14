CMAKE_MINIMUM_REQUIRED(VERSION 3.25.0)

project(reverse_polish_notation)

include_directories(./sources)
include_directories(./headers)

set(SRC
    sources/Token.cpp 
    sources/Record.cpp
    sources/Arithmetic.cpp
)
set(HDR
    headers/Token.hpp
    headers/ExceptionRecord.hpp
    headers/Record.hpp
    headers/Arithmetic.hpp
)

add_library(main_lib ${SRC} ${HDR})

add_executable(reverse_polish_notation main.cpp)
target_link_libraries(reverse_polish_notation main_lib)
