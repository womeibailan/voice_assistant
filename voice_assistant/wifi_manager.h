#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <stdint.h>
#include "config.h"

// WiFi管理器初始化
bool wifi_manager_init();

// 检查WiFi连接状态
bool wifi_is_connected();

// 重新连接WiFi
void wifi_reconnect();

// 获取当前IP地址
String wifi_get_ip_address();

#endif // WIFI_MANAGER_H
