include_directories(${PROJECT_SOURCE_DIR}/sense-voice/csrc)
# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})
add_executable(sense-voice-stream stream.cc)

# Platform specific configurations
if (WIN32)
    set_target_properties(sense-voice-stream PROPERTIES
        WIN32_EXECUTABLE FALSE)
    target_link_libraries(sense-voice-stream PRIVATE 
        sense-voice-core 
        ggml 
        common 
        streamSDL 
        ${SDL2_LIBRARIES}
        -lm 
        -lgomp 
        -lmingwthrd)
else()
    set_target_properties(sense-voice-stream PROPERTIES
        WIN32_EXECUTABLE FALSE
        MACOSX_BUNDLE FALSE)
    target_link_libraries(sense-voice-stream PRIVATE 
        sense-voice-core 
        ggml 
        common 
        streamSDL 
        ${SDL2_LIBRARIES})
endif()