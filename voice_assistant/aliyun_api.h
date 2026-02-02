#ifndef ALIYUN_API_H
#define ALIYUN_API_H

#include <Arduino.h>
#include <stdint.h>
#include <cstddef>
#include "config.h"

// API初始化
bool aliyun_api_init();

// 语音转文本
String aliyun_api_speech_to_text(uint8_t* audio_data, size_t audio_size);

// 文本对话
String aliyun_api_chat(String text);

// 文本转语音
bool aliyun_api_text_to_speech(const char* text, uint8_t** audio_data, size_t* audio_size);

// 生成签名
String aliyun_api_generate_signature(String method, String url, String params, String body);

#endif // ALIYUN_API_H
