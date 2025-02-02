include_directories(${PROJECT_SOURCE_DIR}/sense-voice/csrc)
add_executable(sense-voice-quantize quantize.cc)

# Platform specific configurations
if (WIN32)
    set_target_properties(sense-voice-quantize PROPERTIES
        WIN32_EXECUTABLE FALSE)
    target_link_libraries(sense-voice-quantize PRIVATE sense-voice-core ggml common -lm -lgomp -lmingwthrd)
else()
    set_target_properties(sense-voice-quantize PROPERTIES
        WIN32_EXECUTABLE FALSE
        MACOSX_BUNDLE FALSE)
    target_link_libraries(sense-voice-quantize PRIVATE sense-voice-core ggml common)
endif()