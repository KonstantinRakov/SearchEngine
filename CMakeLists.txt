cmake_minimum_required(VERSION 3.25)
project(SearchEngine VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

enable_testing()

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json ${CMAKE_EXE_LINKER_FLAGS})

add_subdirectory(Test_SearchEngine)

if (NOT googletest)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()