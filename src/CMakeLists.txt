cmake_minimum_required(VERSION 3.25)

add_executable(${PROJECT_NAME}
    main.cpp
    ConverterJSON.hpp
    InvertedIndex.hpp
    SearchServer.hpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
