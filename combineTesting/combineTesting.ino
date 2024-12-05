#include <SPI.h>
#include "SdFat.h"
#include "I2SRecord.h"
#include "wave.h"

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// TFT Screen
#define TFT_DC 14
#define TFT_CS 10
#define TFT_MOSI 11
#define TFT_CLK 12
#define TFT_RST 9
#define TFT_MISO 13
#define TFT_SD_CS 8

// Define SD card SPI pins
#define SD_CS_PIN   10
#define SD_SCK_PIN  12
#define SD_MOSI_PIN 11
#define SD_MISO_PIN 13

// Define INMP441 I²S pins
#define I2S_SCK_PIN   5  // BCLK
#define I2S_WS_PIN    6  // LRCLK
#define I2S_SD_PIN    4  // DOUT
#define I2S_AMPLIFY   20 // Amplification factor
#define BUFFER_SIZE   1024

// Define button pin
#define BUTTON_PIN    7  // GPIO pin for button
#define DEBOUNCE_MS   50 // Debounce delay in milliseconds

// Recording configuration
#define SAMPLE_RATE   16000             // Sampling rate in Hz
#define RECORD_TIME   5                 // Recording duration in seconds
#define WAVE_HEADER_SIZE 44             // WAV file header size in bytes
#define FILENAME_SIZE 20                // Maximum filename size

SdFs sd;                                // SD card object
FsFile file;                            // Recording file object
I2SRecord i2sRecorder;                  // I²S Recorder object

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);


// Derived constants
const int waveDataSize = SAMPLE_RATE * RECORD_TIME * 2; // Data size (bytes)
int32_t communicationData[BUFFER_SIZE];                // I²S buffer
char partWavData[BUFFER_SIZE];                         // Processed WAV data

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure button pin with pull-up resistor

  tft.begin();
  showScreenInfo();
  //delay(20000);

  if (!initSDCard() || !initI2S()) {
    while (true); // Halt on initialization failure
  }

  Serial.println("Ready to record. Press the button to start.");
}

void loop() {
  if (buttonPressed()) {
    Serial.println("Button pressed. Starting recording...");
    recordAudio();
    Serial.println("Recording finished. Press the button to start a new one.");
  }
}

unsigned long showScreenInfo() {
  tft.setRotation(1); // vertical
  tft.fillScreen(0X1A27);
  unsigned long start = micros();

  // Date text color and size
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  tft.setCursor(5, 13); //location
  tft.print("Nov17 Mon 14:30"); 

  tft.setCursor(270, 13); // location
  tft.print("2024");

  // split line
  tft.drawFastHLine(0, 40, 320, ILI9341_YELLOW);

  return micros() - start;
}

bool initSDCard() {
  if (!sd.begin(SdSpiConfig(TFT_SD_CS, DEDICATED_SPI, 18000000))) {
    Serial.println("SD card initialization failed!");
    return false;
  }
  return true;
}

bool initI2S() {
  if (!i2sRecorder.InitInput(I2S_BITS_PER_SAMPLE_32BIT, I2S_SCK_PIN, I2S_WS_PIN, I2S_SD_PIN, SAMPLE_RATE)) {
    Serial.println("I²S initialization failed!");
    return false;
  }
  return true;
}

bool buttonPressed() {
  if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
    delay(DEBOUNCE_MS); // Debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) { // Confirm press
      return true;
    }
  }
  return false;
}

void recordAudio() {
  char filename[FILENAME_SIZE];
  snprintf(filename, FILENAME_SIZE, "/record_%lu.wav", millis());

  if (!createFile(filename)) {
    return;
  }

  writeWavHeader();
  captureAudio();
  file.close();

  Serial.print("Recording saved as: ");
  Serial.println(filename);
}

bool createFile(const char* filename) {
  sd.remove(filename); // Ensure no previous file with the same name
  file = sd.open(filename, O_WRITE | O_CREAT);
  if (!file) {
    Serial.println("File creation failed!");
    return false;
  }
  return true;
}

void writeWavHeader() {
  // Create a WAV file header

  // - 1: Number of channels (1 = mono)
  // - SAMPLE_RATE: Sampling rate 
  // - 16: Bits per sample (16-bit audio)
  WAV_HEADER header = CreateWaveHeader(1, SAMPLE_RATE, 16);

  // Calculate the riffSize
  // represents the total file size minus the first 8 bytes of "RIFF" and "WAVE"
  // Formula: waveDataSize (size of audio data) + WAVE_HEADER_SIZE (header size) - 8 bytes
  header.riffSize = waveDataSize + WAVE_HEADER_SIZE - 8;

  // Set the dataSize
  // represents the total number of bytes in the audio data section of the WAV file
  header.dataSize = waveDataSize;

  // Write the header to the first 44 bytes of the file using file.write()
  file.write(&header, WAVE_HEADER_SIZE);
}


void captureAudio() {
  for (int j = 0; j < waveDataSize / BUFFER_SIZE; ++j) {
    auto sz = i2sRecorder.Read((char*)communicationData, BUFFER_SIZE * 4);
    char* p = (char*)(communicationData);

    for (int i = 0; i < sz / 4; i++) {
      communicationData[i] *= I2S_AMPLIFY; // Amplify sound
      if (i % 2 == 0) { // Process right channel
        partWavData[i] = p[4 * i + 2];
        partWavData[i + 1] = p[4 * i + 3];
      }
    }
    file.write((const byte*)partWavData, BUFFER_SIZE);
  }
}
