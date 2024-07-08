cmake_minimum_required (VERSION 3.26)

project ("Test_SearchEngine" VERSION 1.0)

set(Sources
    TestInvertedIndex.cpp
    TestSearchServer.cpp
)

add_executable(${PROJECT_NAME} ${Sources})
target_link_libraries(${PROJECT_NAME}
    gtest_main
)
add_test(NAME TestInvertedIndex COMMAND ${PROJECT_NAME})
add_test(NAME TestSearchServer COMMAND ${PROJECT_NAME})