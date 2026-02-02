#include "voice_assistant.h"
#include "audio_driver.h"
#include "wifi_manager.h"
#include "wakeword_detector.h"
#include "sleep_monitor.h"
#include "aliyun_api.h"
#include <Arduino.h>

// 当前状态
static va_state_t current_state = VA_STATE_WAITING_WAKEUP;
// 上次活动时间
static uint32_t last_activity_time = 0;

// 语音助手初始化
void voice_assistant_init() {
    current_state = VA_STATE_WAITING_WAKEUP;
    last_activity_time = millis();
    
    // 初始化各模块
    audio_start_recording();
    sleep_monitor_init();
    
    Serial.println("语音助手初始化完成！");
}

// 语音助手主循环
void voice_assistant_loop() {
    // 更新活动时间
    if (current_state != VA_STATE_SLEEP) {
        last_activity_time = millis();
    }
    
    // 检查睡眠状态
    if (sleep_monitor_check_timeout(last_activity_time)) {
        voice_assistant_sleep();
    }
    
    // 根据当前状态执行不同操作
    switch (current_state) {
        case VA_STATE_SLEEP:
            // 睡眠状态，等待唤醒
            if (wakeword_detector_check()) {
                voice_assistant_wakeup();
            }
            break;
            
        case VA_STATE_WAITING_WAKEUP:
            // 等待唤醒状态，检测唤醒词
            if (wakeword_detector_check()) {
                voice_assistant_wakeup();
            }
            break;
            
        case VA_STATE_ACTIVE:
            // 活跃状态，等待用户输入
            voice_assistant_process_command();
            break;
            
        case VA_STATE_PROCESSING:
            // 处理中状态，等待处理完成
            break;
            
        case VA_STATE_SPEAKING:
            // 说话状态，等待说话完成
            break;
    }
}

// 获取当前状态
va_state_t voice_assistant_get_state() {
    return current_state;
}

// 设置状态
void voice_assistant_set_state(va_state_t state) {
    current_state = state;
    last_activity_time = millis();
    
    Serial.print("状态切换: ");
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

// 唤醒语音助手
void voice_assistant_wakeup() {
    voice_assistant_set_state(VA_STATE_ACTIVE);
    
    // 播放唤醒提示音
    const char* wakeup_prompt = "你好，我在听，请说！";
    voice_assistant_play_response(wakeup_prompt);
    
    Serial.println("语音助手已唤醒！");
}

// 使语音助手睡眠
void voice_assistant_sleep() {
    voice_assistant_set_state(VA_STATE_SLEEP);
    Serial.println("语音助手已进入睡眠状态！");
}

// 处理语音命令
void voice_assistant_process_command() {
    if (!wifi_is_connected()) {
        Serial.println("网络未连接，无法处理命令！");
        voice_assistant_play_response("网络未连接，请检查网络设置。");
        voice_assistant_set_state(VA_STATE_WAITING_WAKEUP);
        return;
    }
    
    voice_assistant_set_state(VA_STATE_PROCESSING);
    
    // 录制音频
    Serial.println("正在录制音频...");
    
    // 读取音频数据
    uint8_t audio_buffer[AUDIO_BUFFER_SIZE];
    int bytes_read = audio_read_data(audio_buffer, sizeof(audio_buffer));
    
    if (bytes_read > 0) {
        // 调用阿里云API进行语音转文本
        String text = aliyun_api_speech_to_text(audio_buffer, bytes_read);
        
        if (text.length() > 0) {
            Serial.print("识别结果: ");
            Serial.println(text);
            
            // 调用阿里云API进行对话
            String response = aliyun_api_chat(text);
            
            if (response.length() > 0) {
                Serial.print("对话结果: ");
                Serial.println(response);
                
                // 播放响应
                voice_assistant_play_response(response.c_str());
            } else {
                Serial.println("对话失败！");
                voice_assistant_play_response("抱歉，我无法理解您的意思。");
            }
        } else {
            Serial.println("语音识别失败！");
            voice_assistant_play_response("抱歉，我没有听清，请再说一遍。");
        }
    } else {
        Serial.println("音频录制失败！");
        voice_assistant_play_response("抱歉，音频录制失败，请检查麦克风。");
    }
    
    // 处理完成，回到等待唤醒状态
    voice_assistant_set_state(VA_STATE_WAITING_WAKEUP);
}

// 播放响应
void voice_assistant_play_response(const char* response_text) {
    voice_assistant_set_state(VA_STATE_SPEAKING);
    
    // 调用阿里云API进行语音合成
    uint8_t* audio_data = NULL;
    size_t audio_size = 0;
    
    if (aliyun_api_text_to_speech(response_text, &audio_data, &audio_size)) {
        if (audio_data && audio_size > 0) {
            // 播放合成的语音
            audio_play_data(audio_data, audio_size);
            free(audio_data);
        }
    } else {
        Serial.println("语音合成失败！");
    }
    
    voice_assistant_set_state(VA_STATE_WAITING_WAKEUP);
}
