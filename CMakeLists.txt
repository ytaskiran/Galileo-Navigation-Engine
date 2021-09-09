cmake_minimum_required(VERSION 3.16)
project(GalileoSolver VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)


add_library(galileo_page src/galileo_page.cpp)   
add_library(navigation_engine src/navigation_engine.cpp)   
   

target_link_libraries(navigation_engine 
                      PRIVATE 
                      galileo_page)

enable_testing()

add_executable(galileo_test test/galileo_unit_tests.cpp)
target_link_libraries(galileo_test galileo_page)
target_link_libraries(galileo_test gtest_main)

include(GoogleTest)
gtest_discover_tests(galileo_test)