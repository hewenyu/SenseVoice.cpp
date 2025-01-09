#include "vad_detector.h"
#include <cmath>

namespace sense_voice {

VADDetector::VADDetector(float threshold, float freq_threshold, int sample_rate)
    : threshold_(threshold)
    , freq_threshold_(freq_threshold)
    , sample_rate_(sample_rate) {
}

bool VADDetector::detect(const float* audio, int size) {
    if (size <= 0) return false;
    
    // 计算音频能量
    float energy = calculateEnergy(audio, size);
    
    // 计算过零率
    float zero_crossings = calculateZeroCrossings(audio, size);
    
    // 如果能量超过阈值且过零率在合理范围内，认为有语音活动
    return (energy > threshold_) && (zero_crossings > freq_threshold_);
}

float VADDetector::calculateEnergy(const float* audio, int size) {
    float energy = 0.0f;
    
    // 计算平均能量
    for (int i = 0; i < size; ++i) {
        energy += audio[i] * audio[i];
    }
    
    return energy / size;
}

float VADDetector::calculateZeroCrossings(const float* audio, int size) {
    int crossings = 0;
    
    // 计算过零率
    for (int i = 1; i < size; ++i) {
        if ((audio[i] >= 0 && audio[i-1] < 0) || 
            (audio[i] < 0 && audio[i-1] >= 0)) {
            crossings++;
        }
    }
    
    // 转换为频率
    return (float)crossings * sample_rate_ / (size * 2.0f);
}

} // namespace sense_voice 