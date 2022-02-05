cmake_minimum_required(VERSION 3.5)

project(async LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost
find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_library(async SHARED
  async.cpp
  async.h
  logger.cpp
  logger.h
  assignments.h
  state.cpp
  state.h
  handler.cpp
  handler.h
)

target_compile_definitions(async PRIVATE ASYNC_LIBRARY)

# LINKING
find_package (Threads)
target_link_libraries (
    async
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_FILESYSTEM_LIBRARY}
)

if (MSVC)
    target_compile_options(async PRIVATE
        /W4
    )
else ()
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
