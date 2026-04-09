cmake_minimum_required(VERSION 3.15)
project(typeape VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB SOURCES src/*.cpp)
add_executable(typeape ${SOURCES})
target_include_directories(typeape PRIVATE include)
target_link_libraries(typeape pthread)
target_compile_options(typeape PRIVATE -O2 -Wall)

target_compile_definitions(typeape PRIVATE TYPEAPE_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/typeape/data")

install(TARGETS typeape DESTINATION bin)
install(FILES data/words.txt DESTINATION share/typeape/data)
