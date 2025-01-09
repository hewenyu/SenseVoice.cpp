#pragma once

#include <pulse/simple.h>
#include <pulse/error.h>
#include <vector>
#include <string>
#include <memory>

namespace sense_voice {

class AudioCapture {
public:
    // 音频源类型
    enum class SourceType {
        DEFAULT_OUTPUT,  // 系统默认输出
        DEFAULT_INPUT,   // 系统默认输入(麦克风)
        SPECIFIC_APP    // 特定应用程序
    };

    AudioCapture(int sample_rate = 16000, int channels = 1, int buffer_size_ms = 100);
    ~AudioCapture();

    // 初始化音频捕获
    bool init(SourceType source = SourceType::DEFAULT_OUTPUT, const std::string& app_name = "");

    // 开始捕获
    bool start();

    // 停止捕获
    void stop();

    // 读取音频数据
    int read(float* buffer, int size);

    // 获取错误信息
    std::string getError() const;

    // 获取配置信息
    int getSampleRate() const { return sample_rate_; }
    int getChannels() const { return channels_; }
    int getBufferSize() const { return buffer_size_; }

private:
    pa_simple* pa_handle_ = nullptr;
    int sample_rate_;
    int channels_;
    int buffer_size_;    // 缓冲区大小(采样点数)
    bool is_running_ = false;
    
    // 转换音频格式
    void convertToFloat(const int16_t* input, float* output, int size);
    
    // 临时缓冲区
    std::vector<int16_t> temp_buffer_;
};

} // namespace sense_voice 