#include "sleep_monitor.h"
#include <Arduino.h>

// 超时时间（毫秒）
static uint32_t sleep_timeout = SLEEP_TIMEOUT_MS;

// 睡眠监测器初始化
void sleep_monitor_init() {
    Serial.println("睡眠监测器初始化...");
    // 初始化代码（如果需要）
}

// 检查是否超时
bool sleep_monitor_check_timeout(uint32_t last_activity_time) {
    return (millis() - last_activity_time) > sleep_timeout;
}

// 重置超时计时器
void sleep_monitor_reset_timeout() {
    // 重置逻辑（如果需要）
}

// 设置超时时间
void sleep_monitor_set_timeout(uint32_t timeout_ms) {
    sleep_timeout = timeout_ms;
    Serial.print("睡眠超时时间设置为: ");
    Serial.print(timeout_ms / 1000);
    Serial.println("秒");
}

// 获取当前超时时间
uint32_t sleep_monitor_get_timeout() {
    return sleep_timeout;
}
