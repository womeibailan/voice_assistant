#include "audio_driver.h"
#include "driver/i2s.h"
#include <Arduino.h>

// I2S配置
static const i2s_config_t i2s_out_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 128,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
};

static const i2s_config_t i2s_in_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 128,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};

static const i2s_pin_config_t i2s_out_pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
};

static const i2s_pin_config_t i2s_in_pin_config = {
    .bck_io_num = I2S_MIC_BCLK,
    .ws_io_num = I2S_MIC_LRC,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_DIN
};

// 音频驱动初始化
bool audio_driver_init() {
    // 初始化I2S输出（扬声器）
    if (i2s_driver_install(I2S_NUM_0, &i2s_out_config, 0, NULL) != ESP_OK) {
        return false;
    }
    if (i2s_set_pin(I2S_NUM_0, &i2s_out_pin_config) != ESP_OK) {
        return false;
    }
    if (i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO) != ESP_OK) {
        return false;
    }
    
    // 初始化I2S输入（麦克风）
    if (i2s_driver_install(I2S_NUM_1, &i2s_in_config, 0, NULL) != ESP_OK) {
        return false;
    }
    if (i2s_set_pin(I2S_NUM_1, &i2s_in_pin_config) != ESP_OK) {
        return false;
    }
    if (i2s_set_clk(I2S_NUM_1, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO) != ESP_OK) {
        return false;
    }
    
    return true;
}

// 开始录制音频
void audio_start_recording() {
    i2s_start(I2S_NUM_1);
}

// 停止录制音频
void audio_stop_recording() {
    i2s_stop(I2S_NUM_1);
}

// 读取音频数据
int audio_read_data(uint8_t* buffer, size_t size) {
    size_t bytes_read = 0;
    i2s_read(I2S_NUM_1, buffer, size, &bytes_read, portMAX_DELAY);
    return bytes_read;
}

// 播放音频数据
void audio_play_data(uint8_t* buffer, size_t size) {
    size_t bytes_written = 0;
    i2s_write(I2S_NUM_0, buffer, size, &bytes_written, portMAX_DELAY);
}

// 停止播放
void audio_stop_playback() {
    i2s_stop(I2S_NUM_0);
}

// 设置音量
void audio_set_volume(uint8_t volume) {
    // MAX98357A是固定增益，这里可以通过软件实现音量控制
    // 实际应用中可以通过调整音频数据的 amplitude 来实现
}

// 获取音频缓冲区状态
bool audio_buffer_available() {
    // 简化实现：返回true表示缓冲区可用
    // 实际应用中应该使用正确的I2S API函数
    return true;
}
