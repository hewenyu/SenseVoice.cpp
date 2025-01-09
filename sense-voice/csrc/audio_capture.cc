#include "audio_capture.h"
#include <cstring>
#include <stdexcept>

namespace sense_voice {

AudioCapture::AudioCapture(int sample_rate, int channels, int buffer_size_ms)
    : sample_rate_(sample_rate)
    , channels_(channels)
    , buffer_size_((sample_rate * channels * buffer_size_ms) / 1000) {
    
    // 初始化临时缓冲区
    temp_buffer_.resize(buffer_size_);
}

AudioCapture::~AudioCapture() {
    stop();
    if (pa_handle_) {
        pa_simple_free(pa_handle_);
    }
}

bool AudioCapture::init(SourceType source, const std::string& app_name) {
    // PulseAudio配置
    pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16LE;  // 16位有符号整数
    ss.channels = channels_;
    ss.rate = sample_rate_;
    
    pa_buffer_attr buffer_attr;
    buffer_attr.maxlength = (uint32_t)-1;
    buffer_attr.tlength = (uint32_t)-1;
    buffer_attr.prebuf = (uint32_t)-1;
    buffer_attr.minreq = (uint32_t)-1;
    buffer_attr.fragsize = (uint32_t)buffer_size_ * sizeof(int16_t);
    
    int error;
    switch (source) {
        case SourceType::DEFAULT_OUTPUT:
            pa_handle_ = pa_simple_new(
                nullptr,               // 使用默认服务器
                "SenseVoice",         // 应用名称
                PA_STREAM_RECORD,     // 录制流
                nullptr,              // 使用默认设备
                "audio capture",      // 流描述
                &ss,                  // 采样配置
                nullptr,              // 使用默认通道映射
                &buffer_attr,         // 缓冲区属性
                &error                // 错误代码
            );
            break;
            
        case SourceType::DEFAULT_INPUT:
            pa_handle_ = pa_simple_new(
                nullptr,
                "SenseVoice",
                PA_STREAM_RECORD,
                nullptr,
                "microphone capture",
                &ss,
                nullptr,
                &buffer_attr,
                &error
            );
            break;
            
        case SourceType::SPECIFIC_APP:
            if (app_name.empty()) {
                return false;
            }
            // TODO: 实现特定应用的音频捕获
            break;
    }
    
    return pa_handle_ != nullptr;
}

bool AudioCapture::start() {
    if (!pa_handle_) {
        return false;
    }
    is_running_ = true;
    return true;
}

void AudioCapture::stop() {
    is_running_ = false;
}

int AudioCapture::read(float* buffer, int size) {
    if (!is_running_ || !pa_handle_) {
        return 0;
    }
    
    // 确保临时缓冲区足够大
    if (temp_buffer_.size() < size_t(size)) {
        temp_buffer_.resize(size);
    }
    
    int error;
    if (pa_simple_read(pa_handle_, temp_buffer_.data(), size * sizeof(int16_t), &error) < 0) {
        return -1;
    }
    
    // 转换为float格式
    convertToFloat(temp_buffer_.data(), buffer, size);
    
    return size;
}

std::string AudioCapture::getError() const {
    int error;  // 用于存储错误代码
    if (pa_handle_) {
        pa_simple_drain(pa_handle_, &error);  // 获取最后的错误代码
        return pa_strerror(error);
    }
    return "Invalid PulseAudio handle";
}

void AudioCapture::convertToFloat(const int16_t* input, float* output, int size) {
    const float scale = 1.0f / 32768.0f;  // 将16位整数转换为[-1,1]范围的浮点数
    for (int i = 0; i < size; ++i) {
        output[i] = input[i] * scale;
    }
}

} // namespace sense_voice 