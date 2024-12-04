#include <SPI.h>
#include "SdFat.h"
#include "I2SRecord.h"
#include "wave.h"

// Define SD card SPI pins
#define SD_CS_PIN   10
#define SD_SCK_PIN  12
#define SD_MOSI_PIN 11
#define SD_MISO_PIN 13

// Define INMP441 I2S pins
#define I2S_SCK_PIN  5  // SCK (BCLK)
#define I2S_WS_PIN   6  // WS (LRCLK)
#define I2S_SD_PIN   4  // SD (DOUT) also connecting a 1k resistor between sd and pin 4

SdFs sd;      // SD card
FsFile file;  // Recording file

I2SRecord mi;
const int record_time = 10;  // Seconds
const char filename[] = "/my.wav";

const int waveDataSize = record_time * 88200;
int32_t communicationData[1024];     // Receive buffer
char partWavData[1024];

void setup() {
  Serial.begin(115200);
  delay(500);

  // Initialize SD card
  if (!sd.begin(SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, 18000000))) { // CS = SD_CS_PIN
    Serial.println("Init SD card error");
    return;
  }

  // Delete and create file
  sd.remove(filename);
  file = sd.open(filename, O_WRITE | O_CREAT);
  if (!file) {
    Serial.println("Create file error");
    return;
  }

  auto header = CreateWaveHeader(1, 44100, 16);
  header.riffSize = waveDataSize + 44 - 8;
  header.dataSize = waveDataSize;
  file.write(&header, 44);

  // Initialize I²S for INMP441
  if (!mi.InitInput(I2S_BITS_PER_SAMPLE_32BIT, I2S_SCK_PIN, I2S_WS_PIN, I2S_SD_PIN)) {
    Serial.println("Init I2S error");
    return;
  }

  Serial.println("Start");

  for (int j = 0; j < waveDataSize / 1024; ++j) {
    auto sz = mi.Read((char*)communicationData, 4096);
    char* p = (char*)(communicationData);
    for (int i = 0; i < sz / 4; i++) {
      communicationData[i] *= 20;  // Amplify sound
      if (i % 2 == 0) {   // The first Int32 data is the right channel
        partWavData[i] = p[4 * i + 2];
        partWavData[i + 1] = p[4 * i + 3];
      }
    }
    file.write((const byte*)partWavData, 1024);
  }
  file.close();
  Serial.println("Finish");
}

void loop() {
  // Main loop
}
