#include "wifi_manager.h"
#include <Arduino.h>
#include <WiFi.h>

// WiFi连接状态
static bool wifi_connected = false;

// WiFi管理器初始化
bool wifi_manager_init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("正在连接WiFi...");
    
    // 等待WiFi连接
    uint32_t start_time = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        
        // 超时检查
        if (millis() - start_time > 30000) {
            Serial.println("WiFi连接超时！");
            return false;
        }
    }
    
    wifi_connected = true;
    Serial.println("WiFi连接成功！");
    Serial.print("IP地址: ");
    Serial.println(WiFi.localIP());
    
    // 移除事件处理，使用简单的连接检查
    
    return true;
}

// 检查WiFi连接状态
bool wifi_is_connected() {
    return wifi_connected && (WiFi.status() == WL_CONNECTED);
}

// 重新连接WiFi
void wifi_reconnect() {
    if (!wifi_is_connected()) {
        Serial.println("正在重新连接WiFi...");
        WiFi.reconnect();
        
        // 等待重新连接
        uint32_t start_time = millis();
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            if (millis() - start_time > 30000) {
                Serial.println("WiFi重连失败！");
                wifi_connected = false;
                return;
            }
        }
        
        wifi_connected = true;
        Serial.println("WiFi重连成功！");
        Serial.print("IP地址: ");
        Serial.println(WiFi.localIP());
    }
}

// 获取当前IP地址
String wifi_get_ip_address() {
    if (wifi_is_connected()) {
        return WiFi.localIP().toString();
    }
    return "0.0.0.0";
}


