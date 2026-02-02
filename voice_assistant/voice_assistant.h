#ifndef VOICE_ASSISTANT_H
#define VOICE_ASSISTANT_H

#include <stdint.h>
#include "config.h"

// 语音助手状态枚举
typedef enum {
    VA_STATE_SLEEP,         // 睡眠状态
    VA_STATE_WAITING_WAKEUP, // 等待唤醒状态
    VA_STATE_ACTIVE,         // 活跃状态
    VA_STATE_PROCESSING,     // 处理中状态
    VA_STATE_SPEAKING        // 说话状态
} va_state_t;

// 语音助手初始化
void voice_assistant_init();

// 语音助手主循环
void voice_assistant_loop();

// 获取当前状态
va_state_t voice_assistant_get_state();

// 设置状态
void voice_assistant_set_state(va_state_t state);

// 唤醒语音助手
void voice_assistant_wakeup();

// 使语音助手睡眠
void voice_assistant_sleep();

// 处理语音命令
void voice_assistant_process_command();

// 播放响应
void voice_assistant_play_response(const char* response_text);

#endif // VOICE_ASSISTANT_H
