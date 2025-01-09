#include "audio_buffer.h"
#include <cstring>
#include <algorithm>

namespace sense_voice {

AudioBuffer::AudioBuffer(int step_ms, int length_ms, int keep_ms, int sample_rate)
    : sample_rate_(sample_rate) {
    // 将毫秒转换为采样点数
    step_size_ = (step_ms * sample_rate_) / 1000;
    buffer_size_ = (length_ms * sample_rate_) / 1000;
    keep_size_ = (keep_ms * sample_rate_) / 1000;
    
    // 初始化缓冲区
    current_buffer_.resize(buffer_size_, 0.0f);
    new_buffer_.resize(buffer_size_, 0.0f);
}

void AudioBuffer::push(const float* data, int size) {
    if (size <= 0) return;
    
    // 保存当前缓冲区作为历史数据
    previous_buffer_ = current_buffer_;
    
    // 将新数据复制到new_buffer_
    new_buffer_.resize(size);
    std::memcpy(new_buffer_.data(), data, size * sizeof(float));
    
    // 计算需要保留的历史数据大小
    int keep_samples = std::min(keep_size_, (int)previous_buffer_.size());
    
    // 调整当前缓冲区大小以容纳新旧数据
    current_buffer_.resize(keep_samples + size);
    
    // 复制保留的历史数据
    if (keep_samples > 0) {
        std::memcpy(current_buffer_.data(), 
                   previous_buffer_.data() + previous_buffer_.size() - keep_samples,
                   keep_samples * sizeof(float));
    }
    
    // 复制新数据
    std::memcpy(current_buffer_.data() + keep_samples,
                data,
                size * sizeof(float));
}

const std::vector<float>& AudioBuffer::get() const {
    return current_buffer_;
}

void AudioBuffer::clear() {
    current_buffer_.clear();
    previous_buffer_.clear();
    new_buffer_.clear();
    
    current_buffer_.resize(buffer_size_, 0.0f);
    new_buffer_.resize(buffer_size_, 0.0f);
}

} // namespace sense_voice 