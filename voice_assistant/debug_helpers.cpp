#include "debug_helpers.h"
#include "voice_assistant.h"
#include "wifi_manager.h"
#include <WiFi.h>
#include <stdarg.h>
#include <cstddef>

// 设置调试级别
void debug_set_level(debug_level_t level) {
    current_debug_level = level;
    Serial.print("调试级别设置为: ");
    switch (level) {
        case DEBUG_LEVEL_ERROR:
            Serial.println("ERROR");
            break;
        case DEBUG_LEVEL_WARN:
            Serial.println("WARN");
            break;
        case DEBUG_LEVEL_INFO:
            Serial.println("INFO");
            break;
        case DEBUG_LEVEL_DEBUG:
            Serial.println("DEBUG");
            break;
        case DEBUG_LEVEL_VERBOSE:
            Serial.println("VERBOSE");
            break;
    }
}

// 调试日志函数
void debug_log(debug_level_t level, const char* format, ...) {
    if (level <= current_debug_level) {
        // 打印时间戳
        Serial.print(millis());
        Serial.print(" [");
        
        // 打印级别
        switch (level) {
            case DEBUG_LEVEL_ERROR:
                Serial.print("ERROR");
                break;
            case DEBUG_LEVEL_WARN:
                Serial.print("WARN");
                break;
            case DEBUG_LEVEL_INFO:
                Serial.print("INFO");
                break;
            case DEBUG_LEVEL_DEBUG:
                Serial.print("DEBUG");
                break;
            case DEBUG_LEVEL_VERBOSE:
                Serial.print("VERBOSE");
                break;
        }
        
        Serial.print("] ");
        
        // 打印格式化消息
        va_list args;
        va_start(args, format);
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), format, args);
        Serial.println(buffer);
        va_end(args);
    }
}

// 音频数据调试
void debug_audio_data(uint8_t* data, size_t size) {
    if (current_debug_level >= DEBUG_LEVEL_VERBOSE) {
        Serial.print("音频数据 (前16字节): ");
        for (size_t i = 0; i < size && i < 16; i++) {
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        Serial.print("音频数据大小: ");
        Serial.println(size);
    }
}

// WiFi状态调试
void debug_wifi_status() {
    if (current_debug_level >= DEBUG_LEVEL_INFO) {
        Serial.print("WiFi状态: ");
        if (wifi_is_connected()) {
            Serial.println("已连接");
            Serial.print("IP地址: ");
            Serial.println(wifi_get_ip_address());
            Serial.print("RSSI: ");
            Serial.println(WiFi.RSSI());
        } else {
            Serial.println("未连接");
        }
    }
}

// 语音助手状态调试
void debug_va_status() {
    if (current_debug_level >= DEBUG_LEVEL_INFO) {
        Serial.print("语音助手状态: ");
        va_state_t state = voice_assistant_get_state();
        switch (state) {
            case VA_STATE_SLEEP:
                Serial.println("睡眠");
                break;
            case VA_STATE_WAITING_WAKEUP:
                Serial.println("等待唤醒");
                break;
            case VA_STATE_ACTIVE:
                Serial.println("活跃");
                break;
            case VA_STATE_PROCESSING:
                Serial.println("处理中");
                break;
            case VA_STATE_SPEAKING:
                Serial.println("说话中");
                break;
        }
    }
}

// API响应调试
void debug_api_response(const char* response) {
    if (current_debug_level >= DEBUG_LEVEL_DEBUG) {
        Serial.println("API响应:");
        Serial.println(response);
        Serial.println();
    }
}
