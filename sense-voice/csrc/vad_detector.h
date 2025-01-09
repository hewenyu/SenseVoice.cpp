#pragma once

#include <vector>

namespace sense_voice {

class VADDetector {
public:
    VADDetector(float threshold = 0.6f, float freq_threshold = 100.0f, int sample_rate = 16000);
    
    // 检测音频数据中是否有语音活动
    bool detect(const float* audio, int size);
    
    // 设置/获取阈值
    void setThreshold(float threshold) { threshold_ = threshold; }
    float getThreshold() const { return threshold_; }
    
    // 设置/获取频率阈值
    void setFreqThreshold(float threshold) { freq_threshold_ = threshold; }
    float getFreqThreshold() const { return freq_threshold_; }

private:
    // 计算音频能量
    float calculateEnergy(const float* audio, int size);
    
    // 计算过零率
    float calculateZeroCrossings(const float* audio, int size);
    
    float threshold_;      // 能量阈值
    float freq_threshold_; // 频率阈值
    int sample_rate_;     // 采样率
};

} // namespace sense_voice 