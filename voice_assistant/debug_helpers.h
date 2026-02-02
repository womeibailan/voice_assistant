#ifndef DEBUG_HELPERS_H
#define DEBUG_HELPERS_H

#include <Arduino.h>
#include <stdint.h>
#include <cstddef>
#include "config.h"

// 调试级别
typedef enum {
    DEBUG_LEVEL_ERROR,   // 错误
    DEBUG_LEVEL_WARN,    // 警告
    DEBUG_LEVEL_INFO,    // 信息
    DEBUG_LEVEL_DEBUG,   // 调试
    DEBUG_LEVEL_VERBOSE  // 详细
} debug_level_t;

// 当前调试级别
static debug_level_t current_debug_level = DEBUG_LEVEL_INFO;

// 设置调试级别
void debug_set_level(debug_level_t level);

// 调试日志函数
void debug_log(debug_level_t level, const char* format, ...);

// 音频数据调试
void debug_audio_data(uint8_t* data, size_t size);

// WiFi状态调试
void debug_wifi_status();

// 语音助手状态调试
void debug_va_status();

// API响应调试
void debug_api_response(const char* response);

#endif // DEBUG_HELPERS_H
