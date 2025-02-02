add_library(common STATIC
        common-ggml.h
        common-ggml.cc
)

# Find SDL2 package
if (WIN32)
    # On Windows, we need both SDL2 and SDL2main
    find_package(SDL2 REQUIRED)
    if(SDL2_FOUND)
        include_directories(${SDL2_INCLUDE_DIRS})
        add_library(streamSDL STATIC common-sdl.h common-sdl.cpp)
        target_link_libraries(streamSDL PRIVATE ${SDL2_LIBRARIES})
    endif()
else()
    find_package(SDL2 QUIET)
    if(SDL2_FOUND)
        include_directories(${SDL2_INCLUDE_DIRS})
        add_library(streamSDL STATIC common-sdl.h common-sdl.cpp)
        target_link_libraries(streamSDL PRIVATE ${SDL2_LIBRARIES})
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(ExampleLibs ggml)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.3)
    list(APPEND ExampleLibs stdc++fs)
endif()

target_link_libraries(common ${ExampleLibs})

# Add quantize example without SDL2 dependency
add_subdirectory(quantize)

# Add stream example only if SDL2 is found
if(SDL2_FOUND)
    add_subdirectory(stream)
endif()
