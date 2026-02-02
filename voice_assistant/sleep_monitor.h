#ifndef SLEEP_MONITOR_H
#define SLEEP_MONITOR_H

#include <stdint.h>
#include "config.h"

// 睡眠监测器初始化
void sleep_monitor_init();

// 检查是否超时
bool sleep_monitor_check_timeout(uint32_t last_activity_time);

// 重置超时计时器
void sleep_monitor_reset_timeout();

// 设置超时时间
void sleep_monitor_set_timeout(uint32_t timeout_ms);

// 获取当前超时时间
uint32_t sleep_monitor_get_timeout();

#endif // SLEEP_MONITOR_H
