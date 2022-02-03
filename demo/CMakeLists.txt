# CMAKE VERSION
cmake_minimum_required(VERSION 3.5)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "async" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../async")

# EXECUTABLE
add_executable(async_demo main.cpp)

# PROPERTIES
set_target_properties (
    async_demo
        PROPERTIES
            CMAKE_CXX_STANDARD 17
            CMAKE_CXX_STANDARD_REQUIRED ON
)

# INCLUDEPATH
target_include_directories(async_demo
  PRIVATE
    ${PROJECT_SOURCE_DIR}/../async/
)

# Linking
find_package (Threads)
target_link_libraries (async_demo async ${CMAKE_THREAD_LIBS_INIT})

# Compiler options
if (MSVC)
    target_compile_options(async_demo PRIVATE
        /W4
    )
else ()
    target_compile_options(async_demo PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

# # ## Package
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
#set(CPACK_GENERATOR DEB)
#set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
#set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
#set(CPACK_PACKAGE_CONTACT example@example.com)
#include(CPack)

### UNIT-tests
## GoogleTest package
#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#include(GoogleTest)

## Tests linking
#gtest_discover_tests(gtest_handler)
#target_link_libraries(
#    gtest_handler
#    gtest_main
#)

## Testing
#enable_testing()
#add_test(gtest_handler gtest_handler)
