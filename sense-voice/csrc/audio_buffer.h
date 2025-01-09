#pragma once

#include <vector>
#include <cstdint>

namespace sense_voice {

class AudioBuffer {
public:
    AudioBuffer(int step_ms = 3000, int length_ms = 10000, int keep_ms = 200, int sample_rate = 16000);
    
    // 添加新的音频数据
    void push(const float* data, int size);
    
    // 获取当前缓冲区的数据
    const std::vector<float>& get() const;
    
    // 清空缓冲区
    void clear();
    
    // 获取配置参数
    int getStepSize() const { return step_size_; }
    int getBufferSize() const { return buffer_size_; }
    int getKeepSize() const { return keep_size_; }
    
    // 检查是否有足够的数据可以处理
    bool ready() const {
        return current_buffer_.size() >= buffer_size_;
    }

private:
    std::vector<float> current_buffer_;  // 当前处理的音频数据
    std::vector<float> previous_buffer_; // 上一段音频数据
    std::vector<float> new_buffer_;      // 新采集的音频数据
    
    int step_size_;    // 每次处理的采样点数
    int buffer_size_;  // 总缓冲区大小(采样点)
    int keep_size_;    // 保留前一段音频的采样点数
    int sample_rate_;  // 采样率
};

} // namespace sense_voice 