#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

#include <stdint.h>
#include <cstddef>
#include "config.h"

// 音频驱动初始化
bool audio_driver_init();

// 开始录制音频
void audio_start_recording();

// 停止录制音频
void audio_stop_recording();

// 读取音频数据
int audio_read_data(uint8_t* buffer, size_t size);

// 播放音频数据
void audio_play_data(uint8_t* buffer, size_t size);

// 停止播放
void audio_stop_playback();

// 设置音量
void audio_set_volume(uint8_t volume);

// 获取音频缓冲区状态
bool audio_buffer_available();

#endif // AUDIO_DRIVER_H
