// microphone.cpp

#include <Arduino.h>
#include <driver/i2s.h>

#include "microphone.h"

// // INMP441 microphone
// constexpr int I2S_SD = 16;
// constexpr int I2S_SCK = 17;
// constexpr int I2S_WS = 18;
// constexpr i2s_port_t I2S_PORT = I2S_NUM_0;

// constexpr int SAMPLE_COUNT = 4096;
// constexpr double FULL_SCALE = 33554432.0;  // 2^25    

int32_t samples[SAMPLE_COUNT];

void setup_microphone()
{
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 4,
        .dma_buf_len = 256,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pins = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD
    };

    esp_err_t result = i2s_driver_install(
        I2S_PORT,
        &config,
        0,
        nullptr
    );

    if (result != ESP_OK) {
        Serial.printf("i2s_driver_install failed: %d\r\n", result);
        return;
    }

    result = i2s_set_pin(I2S_PORT, &pins);

    if (result != ESP_OK) {
        Serial.printf("i2s_set_pin failed: %d\r\n", result);
        return;
    }

    Serial.println("I2S microphone configured");
}

double measure_sound_level() {
    size_t bytes_read = 0;
    esp_err_t result = i2s_read(
        I2S_PORT,
        samples,
        sizeof(samples),
        &bytes_read,
        portMAX_DELAY
    );

    if (result != ESP_OK) {
        Serial.printf("i2s_read failed: %d\r\n", result);
        return 0.0;
    }

    int samples_read = bytes_read / sizeof(int32_t);
    // just retain the meaningful bits
    for (int i = 0; i < samples_read; i++) {
        samples[i] >>= 6;
    }    

    // calculate mean of the samples
    double total = 0.0;
    for (int i = 0; i < samples_read; i++) {
        int32_t sample = samples[i];
        total += sample;
    }
    double mean = total / samples_read;

    // Now calculate RMS relative to the mean
    double square_total = 0.0;
    for (int i = 0; i < samples_read; i++) {
        double sample = (double)samples[i] - mean;
        square_total += sample * sample;
    }
    double rms = sqrt(square_total / samples_read);
    // (crudely) convert the rms to dB 
    double db = 20.0 * log10(rms);
    // and db full scale value
    double dbfs = 20.0 * log10(rms / FULL_SCALE);
    // convert to Sound Pressure Level (SPL) using 94 dB SPL  →  -26 dBFS  
    double db_spl = dbfs + 120.0;

    Serial.printf(
        "RMS: %.0f dBFS: %.1f estimated SPL: %.1f dB ",
        rms,
        dbfs, 
        db_spl
    );
    return db_spl;
}
