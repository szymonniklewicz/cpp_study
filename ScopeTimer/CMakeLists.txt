cmake_minimum_required(VERSION 2.6)
project(ScopeTimer)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(googletest/)
set(CMAKE_PROJECT_VERSION 1)

set(HEADER_FILE ScopeTimer.hpp)
add_library(ScopeTimer ${HEADER_FILE} ScopeTimer.cpp)
set_property(TARGET ScopeTimer PROPERTY CXX_STANDARD 17)
target_include_directories(ScopeTimer PRIVATE include)

#add_executable(main main.cpp)
add_executable(ScopeTimerTest ScopeTimerTest.cpp)
target_link_libraries(ScopeTimerTest gtest_main ScopeTimer)
target_include_directories(ScopeTimerTest PRIVATE include)

# target_link_libraries(main gtest_main ScopeTimer)
# target_include_directories(main PRIVATE include)


# install(TARGETS scopetimer_test RUNTIME DESTINATION bin)
