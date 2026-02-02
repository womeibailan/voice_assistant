#include "config.h"
#include "audio_driver.h"
#include "wifi_manager.h"
#include "wakeword_detector.h"
#include "sleep_monitor.h"
#include "aliyun_api.h"
#include "voice_assistant.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("初始化语音助手...");
  
  // 初始化音频驱动
  if (!audio_driver_init()) {
    Serial.println("音频驱动初始化失败！");
    while (1);
  }
  
  // 初始化WiFi连接
  if (!wifi_manager_init()) {
    Serial.println("WiFi初始化失败！");
    while (1);
  }
  
  // 初始化唤醒词检测
  if (!wakeword_detector_init()) {
    Serial.println("唤醒词检测初始化失败！");
    while (1);
  }
  
  // 初始化睡眠监测
  sleep_monitor_init();
  
  // 初始化阿里云API
  if (!aliyun_api_init()) {
    Serial.println("阿里云API初始化失败！");
    while (1);
  }
  
  // 初始化语音助手
  voice_assistant_init();
  
  Serial.println("语音助手初始化完成！");
  Serial.println("等待唤醒词...");
}

void loop() {
  // 运行语音助手主循环
  voice_assistant_loop();
}
