#include <driver/i2s.h>
#include "arduinoFFT.h"

// Configuration
#define SAMPLE_BUFFER_SIZE 512
#define SAMPLE_RATE 44100
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_12
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_14
#define I2S_MIC_SERIAL_DATA GPIO_NUM_13

static int chirps = 0;
static bool is_microphone_initialized = false;

// I2S configuration
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 10,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

// FFT settings
double vReal[SAMPLE_BUFFER_SIZE];
double vImag[SAMPLE_BUFFER_SIZE];
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLE_BUFFER_SIZE, SAMPLE_RATE);

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

bool setupMicrophone()
{
  // I2S setup
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
  Serial.println("Microphone: Sensor initialized.");
  is_microphone_initialized = true;
  return is_microphone_initialized;
}

int32_t raw_samples[SAMPLE_BUFFER_SIZE];

void runMicrophoneLoop()
{
  int max_amplitude = 0;

  // Read from the I2S device
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);

  // Convert to double for FFT
  for (int i = 0; i < samples_read; i++)
  {
    vReal[i] = raw_samples[i] / 2147483648.0; // Normalize 32-bit signed int to double in range [-1.0, 1.0]
    vImag[i] = 0.0;                           // Imaginary part is zero for real input
    if (abs(raw_samples[i]) > max_amplitude)
    {
      max_amplitude = abs(raw_samples[i]);
    }
  }

  // // Perform FFT
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  double peak_frequency = FFT.majorPeak();

  if (max_amplitude > 1000000 && peak_frequency >= 3000 && peak_frequency <= 7000)
  {
    chirps++;
  }
}

int getMicrophoneValue()
{
  int result = chirps;
  chirps = 0;
  return result;
}
