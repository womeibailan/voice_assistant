#include "../voice_assistant/config.h"
#include "../voice_assistant/audio_driver.h"
#include "../voice_assistant/wifi_manager.h"
#include "../voice_assistant/wakeword_detector.h"
#include "../voice_assistant/sleep_monitor.h"
#include "../voice_assistant/aliyun_api.h"
#include "../voice_assistant/voice_assistant.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== 语音助手测试 ===");
  
  // 测试1: 初始化音频驱动
  Serial.println("\n测试1: 初始化音频驱动");
  if (audio_driver_init()) {
    Serial.println("✓ 音频驱动初始化成功");
  } else {
    Serial.println("✗ 音频驱动初始化失败");
  }
  
  // 测试2: 初始化WiFi
  Serial.println("\n测试2: 初始化WiFi");
  if (wifi_manager_init()) {
    Serial.println("✓ WiFi初始化成功");
    Serial.print("IP地址: ");
    Serial.println(wifi_get_ip_address());
  } else {
    Serial.println("✗ WiFi初始化失败");
  }
  
  // 测试3: 初始化唤醒词检测器
  Serial.println("\n测试3: 初始化唤醒词检测器");
  if (wakeword_detector_init()) {
    Serial.println("✓ 唤醒词检测器初始化成功");
  } else {
    Serial.println("✗ 唤醒词检测器初始化失败");
  }
  
  // 测试4: 初始化阿里云API
  Serial.println("\n测试4: 初始化阿里云API");
  if (aliyun_api_init()) {
    Serial.println("✓ 阿里云API初始化成功");
  } else {
    Serial.println("✗ 阿里云API初始化失败");
  }
  
  // 测试5: 初始化语音助手
  Serial.println("\n测试5: 初始化语音助手");
  voice_assistant_init();
  Serial.println("✓ 语音助手初始化成功");
  
  Serial.println("\n=== 测试完成 ===");
  Serial.println("现在进入主循环，开始测试语音助手功能...");
}

void loop() {
  // 运行语音助手主循环
  voice_assistant_loop();
  
  // 每5秒打印一次状态
  static uint32_t last_status_time = 0;
  if (millis() - last_status_time > 5000) {
    last_status_time = millis();
    
    Serial.print("\n当前状态: ");
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
    
    Serial.print("WiFi连接状态: ");
    Serial.println(wifi_is_connected() ? "已连接" : "未连接");
  }
}
