# CMAKE VERSION
cmake_minimum_required(VERSION 3.5)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "bulk" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})

# SOURCES
set (
    SOURCES
        handler.cpp
        state.cpp
        logger.cpp
        async.cpp
        context.cpp
)

# HEADERS
set (
    HEADERS
        assignments.h
        handler.h
        state.h
        logger.h
        async.h
        context.h
)

# EXECUTABLE
add_executable(bulk main.cpp ${SOURCES} ${HEADERS})
add_executable(gtest_handler gtest_handler.cpp ${SOURCES} ${HEADERS})

# PROPERTIES
set_target_properties (
    bulk
    gtest_handler
        PROPERTIES
            CMAKE_CXX_STANDARD 17
            CMAKE_CXX_STANDARD_REQUIRED ON
)

# INCLUDEPATH

# Linking
find_package (Threads)
target_link_libraries (bulk ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (gtest_handler ${CMAKE_THREAD_LIBS_INIT})

# Compiler options
if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
    target_compile_options(gtest_handler PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(gtest_handler PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

### Package
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

## UNIT-tests
# GoogleTest package
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Tests linking
gtest_discover_tests(gtest_handler)
target_link_libraries(
    gtest_handler
    gtest_main
)

# Testing
enable_testing()
add_test(gtest_handler gtest_handler)
