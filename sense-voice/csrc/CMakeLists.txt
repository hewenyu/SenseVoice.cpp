set(SOURCE_FILES
        common.h
        common.cc
        sense-voice-frontend.h
        sense-voice-frontend.cc
        fftsg.cc
        log-mel-filter-bank.h
        sense-voice-encoder.h
        sense-voice-encoder.cc
        sense-voice-decoder.h
        sense-voice-decoder.cc
        sense-voice.h
        sense-voice.cc
        silero-vad.h
        silero-vad.cc
)

# 查找必要的依赖
find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSEAUDIO REQUIRED libpulse)
find_package(Threads REQUIRED)

add_library(sense-voice-core STATIC ${SOURCE_FILES})
target_link_libraries(sense-voice-core PRIVATE ggml)

# 添加音频处理库
add_library(sense_voice
    audio_buffer.cc
    vad_detector.cc
    audio_capture.cc
)

target_include_directories(sense_voice PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PULSEAUDIO_INCLUDE_DIRS}
)

target_link_libraries(sense_voice PUBLIC
    sense-voice-core
    ${PULSEAUDIO_LIBRARIES}
    pulse
    pulse-simple
    ${CMAKE_THREAD_LIBS_INIT}
)

# 添加测试用例
add_executable(test_audio_stream test_audio_stream.cc)
target_link_libraries(test_audio_stream PRIVATE sense_voice)

add_executable(test_audio_capture test_audio_capture.cc)
target_link_libraries(test_audio_capture PRIVATE sense_voice)

# 添加 ggml 头文件路径
include_directories(
    ${CMAKE_SOURCE_DIR}/ggml/include
    ${CMAKE_SOURCE_DIR}/ggml/src
    ${CMAKE_SOURCE_DIR}/ggml/examples
    ${CMAKE_BINARY_DIR}/ggml/src  # 生成的头文件可能在这里
)

if(NOT TARGET test_stream_recognition)
    add_executable(test_stream_recognition test_stream_recognition.cc)
    target_link_libraries(test_stream_recognition
        PRIVATE
        sense_voice
        ggml
        ${PULSEAUDIO_LIBRARIES}
        pulse
        pulse-simple
    )
    # 确保 test_stream_recognition 能找到所有需要的头文件
    target_include_directories(test_stream_recognition
        PRIVATE
        ${CMAKE_SOURCE_DIR}/ggml/include
        ${CMAKE_SOURCE_DIR}/ggml/src
        ${CMAKE_SOURCE_DIR}/ggml/examples
        ${CMAKE_BINARY_DIR}/ggml/src
    )
endif()