cmake_minimum_required(VERSION 3.10)
project(orderbook_engine VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#entry point
set(SOURCES 
    main.cpp 
    Orderbook.cpp
)
# Create the executable
add_executable(orderbook_app ${SOURCES})
# Tell CMake to look in the current directory for headers (.h files)
target_include_directories(orderbook_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#test for catch2
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests test_orderbook.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)