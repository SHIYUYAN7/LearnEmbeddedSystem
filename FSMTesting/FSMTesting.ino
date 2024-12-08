#include <SPI.h>
#include "SdFat.h"
#include "I2SRecord.h"
#include "wave.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// TFT Screen
#define TFT_DC    14
#define TFT_CS    10
#define TFT_MOSI  11
#define TFT_CLK   12
#define TFT_RST   9
#define TFT_MISO  13
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

// Define button pins
#define RECORD_BUTTON_PIN   7  // GPIO pin for record button
#define TRANS_BUTTON_PIN    3  // GPIO pin for translate button
#define DEBOUNCE_MS         50 // Debounce delay in milliseconds

// Recording configuration
#define SAMPLE_RATE   16000
#define RECORD_TIME   5
#define WAVE_HEADER_SIZE 44
#define FILENAME_SIZE 20

// FSM States
enum State {
  STATE_INIT,
  STATE_STANDBY,
  STATE_RECORDING,
  STATE_VOICE_RECOGNITION,
  STATE_EXECUTE_COMMAND,
  STATE_UNRECOGNIZED_COMMAND,
  STATE_RESET,
  STATE_TRANSLATING
};

typedef struct {
  String stateStatus;
  String eventText;
  String dateTime;
  String temperature;
} ShowText;

State currentState = STATE_INIT;

ShowText currentText = {
  "Hello",
  "Today's event: Demo at 2 PM",
  "Sun Nov 17 2024 12:30pm",
  "5.6 C"
};

// Global objects
SdFs sd;
FsFile file;
I2SRecord i2sRecorder;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// Debounce variables
unsigned long lastDebounceTime = 0;

// Function Prototypes
void checkButtonPress();
void stateInit();
void stateStandby();
void stateRecording();
void stateVoiceRecognition();
void stateExecuteCommand();
void stateUnrecognizedCommand();
void stateReset();
void stateTranslating();

// Display a message on screen following your specified format
unsigned long showScreenMessage() {
  tft.setRotation(1); // vertical
  tft.fillScreen(0x1A27);
  unsigned long start = micros();
  // Date text color and size
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(5, 13);
  tft.print(currentText.dateTime.c_str());
  tft.setCursor(5, 40);
  tft.print(currentText.temperature.c_str());

  // split line
  tft.drawFastHLine(0, 60, 320, ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(5, 70);
  tft.print(currentText.stateStatus.c_str());

  tft.setCursor(5, 110);
  tft.print(currentText.eventText.c_str());
  return micros() - start;
}

void setup() {
  Serial.begin(115200);

  pinMode(RECORD_BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRANS_BUTTON_PIN, INPUT_PULLUP);

  tft.begin();
  // If needed for ESP32 S3, adjust SPI or other initializations
  // sd.begin(SD_CS_PIN, SPI);
  
  currentState = STATE_INIT;
}

void loop() {
  checkButtonPress();

  // Run the current state logic
  switch (currentState) {
    case STATE_INIT:
      stateInit();
      break;
    case STATE_STANDBY:
      stateStandby();
      break;
    case STATE_RECORDING:
      stateRecording();
      break;
    case STATE_VOICE_RECOGNITION:
      stateVoiceRecognition();
      break;
    case STATE_EXECUTE_COMMAND:
      stateExecuteCommand();
      break;
    case STATE_UNRECOGNIZED_COMMAND:
      stateUnrecognizedCommand();
      break;
    case STATE_RESET:
      stateReset();
      break;
    case STATE_TRANSLATING:
      stateTranslating();
      break;
  }
}

void checkButtonPress() {
  if (digitalRead(RECORD_BUTTON_PIN) == LOW) { // Button pressed
    delay(DEBOUNCE_MS); // Debounce delay
    if (digitalRead(RECORD_BUTTON_PIN) == LOW) { // Confirm press
       currentState = STATE_RECORDING;
    }
  }
  else if (digitalRead(TRANS_BUTTON_PIN) == LOW) { // Button pressed
    delay(DEBOUNCE_MS); // Debounce delay
    if (digitalRead(TRANS_BUTTON_PIN) == LOW) { // Confirm press
       currentState = STATE_TRANSLATING;
    }
  }

  // if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
  //   if (digitalRead(RECORD_BUTTON_PIN) == LOW) {
  //     Serial.println("record click");
  //     lastDebounceTime = millis();
  //     currentState = STATE_RECORDING;
  //   } else if (digitalRead(TRANS_BUTTON_PIN) == LOW) {
  //     Serial.println("trans click");
  //     lastDebounceTime = millis();
  //     currentState = STATE_TRANSLATING;
  //   }
  // }
}

// State Functions
void stateInit() {
  currentText.stateStatus = "Welcome!";
  showScreenMessage();
  
  delay(2000);
  currentState = STATE_STANDBY;
}

void stateStandby() {
  currentText.stateStatus = "Hello!";
  showScreenMessage();
  delay(1000);
  // Remain in standby until button press triggers state change
}

void stateRecording() {
  currentText.stateStatus = "Recording!";
  showScreenMessage();
  delay(RECORD_TIME * 1000); // Simulate recording duration
  currentState = STATE_VOICE_RECOGNITION;
}

void stateVoiceRecognition() {
  currentText.stateStatus = "Analyzing!";
  showScreenMessage();
  delay(3000); // Simulate analysis time
  // After analyzing audio, decide whether command recognized or not
  // For now, let's assume command recognized:
  currentState = STATE_EXECUTE_COMMAND;
}

void stateExecuteCommand() {
  currentText.stateStatus = "Executing Command...";
  showScreenMessage();
  delay(2000);
  currentState = STATE_STANDBY;
}

void stateUnrecognizedCommand() {
  currentText.stateStatus = "Command Not Recognized";
  showScreenMessage();
  delay(2000);
  currentState = STATE_STANDBY;
}

void stateReset() {
  currentText.stateStatus = "System Reset";
  showScreenMessage();
  delay(2000);
  currentState = STATE_INIT;
}

void stateTranslating() {
  currentText.stateStatus = "Translating...";
  showScreenMessage();
  delay(2000);
  currentState = STATE_STANDBY;
}
