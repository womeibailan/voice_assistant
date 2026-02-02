#ifndef WAKEWORD_DETECTOR_H
#define WAKEWORD_DETECTOR_H

#include <stdint.h>
#include <cstddef>
#include "config.h"

// 唤醒词检测器初始化
bool wakeword_detector_init();

// 检查是否检测到唤醒词
bool wakeword_detector_check();

// 设置唤醒词阈值
void wakeword_detector_set_threshold(float threshold);

// 获取当前阈值
float wakeword_detector_get_threshold();

#endif // WAKEWORD_DETECTOR_H
