#ifndef CONFIG_H
#define CONFIG_H

// WiFi配置
#define WIFI_SSID "HJD"
#define WIFI_PASSWORD "bjhongjidian123"

// 阿里云API配置
#ifndef ALIYUN_API_KEY
  #define ALIYUN_API_KEY "your api key"
#endif

// 阿里云服务API URL
#define ALIYUN_ASR_API_URL "https://dashscope.aliyuncs.com/api/v1/services/audio/asr/transcription"
#define ALIYUN_CHAT_API_URL "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions"
#define ALIYUN_TTS_API_URL "https://dashscope.aliyuncs.com/api/v1/services/audio/tts/generation"

// 硬件引脚配置
// MAX98357A I2S音频放大器
#define I2S_BCLK 12
#define I2S_LRC 13
#define I2S_DOUT 11

// INMP441 数字麦克风
#define I2S_MIC_BCLK 12
#define I2S_MIC_LRC 13
#define I2S_MIC_DIN 10

// 唤醒词配置
#define WAKEWORD_MODEL_PATH "/wakeword_model.tflite"
#define WAKEWORD_THRESHOLD 0.8

// 睡眠监测配置
#define SLEEP_TIMEOUT_MS 300000  // 5分钟无操作进入睡眠
#define AUDIO_BUFFER_SIZE 1024
#define SAMPLE_RATE 16000
#define BITS_PER_SAMPLE 16
#define CHANNELS 1

#endif // CONFIG_H
