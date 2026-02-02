#include "aliyun_api.h"
#include "wifi_manager.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Base64编码辅助函数声明
namespace base64 {
    String encode(uint8_t* data, size_t size);
}

// API初始化
bool aliyun_api_init() {
    Serial.println("阿里云API初始化...");
    // 初始化代码（如果需要）
    return true;
}

// 语音转文本
String aliyun_api_speech_to_text(uint8_t* audio_data, size_t audio_size) {
    if (!wifi_is_connected()) {
        return "";
    }
    
    Serial.println("正在调用阿里云语音转文本API...");
    
    HTTPClient http;
    http.begin(ALIYUN_ASR_API_URL);
    
    // 设置请求头
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(ALIYUN_API_KEY));
    
    // 构建请求体
    DynamicJsonDocument doc(2048);
    doc["model"] = "paraformer-realtime-v1";
    doc["audio"] = base64::encode(audio_data, audio_size);
    doc["format"] = "pcm";
    doc["sample_rate"] = SAMPLE_RATE;
    
    String requestBody;
    serializeJson(doc, requestBody);
    
    // 发送请求
    int httpCode = http.POST(requestBody);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        // 解析JSON响应
        DynamicJsonDocument responseDoc(1024);
        DeserializationError error = deserializeJson(responseDoc, response);
        
        if (!error) {
            if (responseDoc.containsKey("result")) {
                String text = responseDoc["result"].as<String>();
                Serial.print("语音识别结果: ");
                Serial.println(text);
                return text;
            }
        }
    }
    
    Serial.print("语音转文本API调用失败，HTTP代码: ");
    Serial.println(httpCode);
    http.end();
    return "";
}

// 文本对话
String aliyun_api_chat(String text) {
    if (!wifi_is_connected()) {
        return "";
    }
    
    Serial.println("正在调用阿里云对话API...");
    
    HTTPClient http;
    http.begin(ALIYUN_CHAT_API_URL);
    
    // 设置请求头
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(ALIYUN_API_KEY));
    
    // 构建请求体
    DynamicJsonDocument doc(1024);
    doc["model"] = "qwen-plus";
    doc["messages"] = JsonArray();
    doc["messages"].add(JsonObject());
    doc["messages"][0]["role"] = "user";
    doc["messages"][0]["content"] = text;
    
    String requestBody;
    serializeJson(doc, requestBody);
    
    // 发送请求
    int httpCode = http.POST(requestBody);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        // 解析JSON响应
        DynamicJsonDocument responseDoc(1024);
        DeserializationError error = deserializeJson(responseDoc, response);
        
        if (!error) {
            if (responseDoc.containsKey("choices")) {
                String reply = responseDoc["choices"][0]["message"]["content"].as<String>();
                Serial.print("对话回复: ");
                Serial.println(reply);
                return reply;
            }
        }
    }
    
    Serial.print("对话API调用失败，HTTP代码: ");
    Serial.println(httpCode);
    http.end();
    return "";
}

// 文本转语音
bool aliyun_api_text_to_speech(const char* text, uint8_t** audio_data, size_t* audio_size) {
    if (!wifi_is_connected()) {
        return false;
    }
    
    Serial.println("正在调用阿里云文本转语音API...");
    
    HTTPClient http;
    http.begin(ALIYUN_TTS_API_URL);
    
    // 设置请求头
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(ALIYUN_API_KEY));
    
    // 构建请求体
    DynamicJsonDocument doc(1024);
    doc["model"] = "sambert-zhiyan"; 
    doc["text"] = text;
    doc["format"] = "pcm";
    doc["sample_rate"] = SAMPLE_RATE;
    
    String requestBody;
    serializeJson(doc, requestBody);
    
    // 发送请求
    int httpCode = http.POST(requestBody);
    
    if (httpCode == HTTP_CODE_OK) {
        // 获取响应数据
        uint8_t* responseBuffer = (uint8_t*)malloc(http.getSize());
        if (responseBuffer) {
            http.getStream().readBytes(responseBuffer, http.getSize());
            *audio_data = responseBuffer;
            *audio_size = http.getSize();
            http.end();
            return true;
        }
    }
    
    Serial.print("文本转语音API调用失败，HTTP代码: ");
    Serial.println(httpCode);
    http.end();
    return false;
}

// 生成API签名
String aliyun_api_generate_signature(String method, String url, String params, String body) {
    // 生成签名的代码（如果需要）
    return "";
}

// Base64编码辅助函数
namespace base64 {
    const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    String encode(uint8_t* data, size_t size) {
        String result;
        int i = 0;
        int j = 0;
        uint8_t char_array_3[3];
        uint8_t char_array_4[4];
        
        while (size--) {
            char_array_3[i++] = *(data++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;
                
                for (i = 0; i < 4; i++) {
                    result += chars[char_array_4[i]];
                }
                i = 0;
            }
        }
        
        if (i > 0) {
            for (j = i; j < 3; j++) {
                char_array_3[j] = 0;
            }
            
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (j = 0; j < i + 1; j++) {
                result += chars[char_array_4[j]];
            }
            
            while (i++ < 3) {
                result += '=';
            }
        }
        
        return result;
    }
}
