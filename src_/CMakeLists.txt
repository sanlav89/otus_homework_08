cmake_minimum_required(VERSION 3.5)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "async" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})
include(GNUInstallDirs)

# SOURCES
set (
    SOURCES
        handler.cpp
        state.cpp
        logger.cpp
        async.cpp
)

# HEADERS
set (
    HEADERS
        assignments.h
        handler.h
        state.h
        logger.h
        async.h
)

# LIBRARY
add_library(async SHARED ${SOURCES} ${HEADERS})

# LINKING
find_package (Threads)
target_link_libraries(async ${CMAKE_THREAD_LIBS_INIT})

# PROPERTIES
set_target_properties (
    async
        PROPERTIES
            CMAKE_CXX_STANDARD 17
            CMAKE_CXX_STANDARD_REQUIRED ON
            PUBLIC_HEADER async.h
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

configure_file(async.pc.in async.pc @ONLY)
target_include_directories(async PRIVATE .)

install (
    TARGETS async
        LIBRARY DESTINATION ./${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ./${CMAKE_INSTALL_INCLUDEDIR}
)

install (
    FILES ${CMAKE_BINARY_DIR}/async.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)
