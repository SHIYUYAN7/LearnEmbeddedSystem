#include <SPI.h>
#include "SdFat.h"
#include "I2SRecord.h"
#include "wave.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <map>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "timeAPIKey.h" 
#include "time.h"
#include <esp_task_wdt.h> // watchdog


const char* weatherCertificate = 
"-----BEGIN CERTIFICATE-----\n"
"MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n"
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n"
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n"
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n"
"MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n"
"BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n"
"aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n"
"dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n"
"AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n"
"3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n"
"tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n"
"Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n"
"VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n"
"79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n"
"c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n"
"Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n"
"c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n"
"UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n"
"Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n"
"BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n"
"A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n"
"Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n"
"VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n"
"ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n"
"8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n"
"iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n"
"Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n"
"XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n"
"qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n"
"VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n"
"L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n"
"jjxDah2nGN59PRbxYvnKkKj9\n"
"-----END CERTIFICATE-----\n";

const char* calendarCertificate = 
"-----BEGIN CERTIFICATE-----\n" \
"MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n" \
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
"MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
"MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
"Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n" \
"A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n" \
"27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n" \
"Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n" \
"TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n" \
"qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n" \
"szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n" \
"Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n" \
"MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n" \
"wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n" \
"aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n" \
"VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n" \
"AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
"FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n" \
"C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n" \
"QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n" \
"h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n" \
"7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n" \
"ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n" \
"MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n" \
"Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n" \
"6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n" \
"0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n" \
"2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n" \
"bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
"-----END CERTIFICATE-----\n";

const char* libre_translate_cert = "-----BEGIN CERTIFICATE-----\n"
"MIICCTCCAY6gAwIBAgINAgPlwGjvYxqccpBQUjAKBggqhkjOPQQDAzBHMQswCQYD\n"
"VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEUMBIG\n"
"A1UEAxMLR1RTIFJvb3QgUjQwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAwMDAw\n"
"WjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2Vz\n"
"IExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjQwdjAQBgcqhkjOPQIBBgUrgQQAIgNi\n"
"AATzdHOnaItgrkO4NcWBMHtLSZ37wWHO5t5GvWvVYRg1rkDdc/eJkTBa6zzuhXyi\n"
"QHY7qca4R9gq55KRanPpsXI5nymfopjTX15YhmUPoYRlBtHci8nHc8iMai/lxKvR\n"
"HYqjQjBAMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW\n"
"BBSATNbrdP9JNqPV2Py1PsVq8JQdjDAKBggqhkjOPQQDAwNpADBmAjEA6ED/g94D\n"
"9J+uHXqnLrmvT/aDHQ4thQEd0dlq7A/Cr8deVl5c1RxYIigL9zC2L7F8AjEA8GE8\n"
"p/SgguMh1YQdc4acLa/KNJvxn7kjNuK8YAOdgLOaVsjh4rsUecrNIdSUtUlD\n"
"-----END CERTIFICATE-----\n";

/*
* 
* Define for Pins and Constants
*  
*/

// TFT Screen
#define TFT_DC    14
#define TFT_CS    10
#define TFT_MOSI  11
#define TFT_CLK   12
#define TFT_RST   9
#define TFT_MISO  13
#define TFT_SD_CS 8

// // SD card SPI pins
// #define SD_CS_PIN   10
// #define SD_SCK_PIN  12
// #define SD_MOSI_PIN 11
// #define SD_MISO_PIN 13

// INMP441 I²S pins
#define I2S_SCK_PIN   5  // BCLK
#define I2S_WS_PIN    6  // LRCLK
#define I2S_SD_PIN    4  // DOUT
#define I2S_AMPLIFY   20 // Amplification factor
#define BUFFER_SIZE   1024

// button pins
#define RECORD_BUTTON_PIN   7  // GPIO pin for record button
#define TRANS_BUTTON_PIN    3  // GPIO pin for translate button
#define DEBOUNCE_MS         50 // Debounce delay in milliseconds

// LED pins
#define LED_PIN             15 // GPIO pin for LED light

// recording configuration
#define SAMPLE_RATE   16000
#define RECORD_TIME   1000
#define ANALYZE_TIME  8000  
#define WAVE_HEADER_SIZE 44
#define FILENAME_SIZE 20

// time space
#define ONE_MIN 60000
#define TEN_MINS 600000

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

// showing screen text structure
typedef struct {
  String stateStatus;
  String eventText;
  String dateTime;
  String temperature;
} ShowText;


// Mapping of states to translations in different languages
// when during testing the whole process, using this existed variable, other than that, let's using the generateStateTranslations() 
// at setup function to fullfill this gloabl variable.
std::map<State, std::map<String, String>> stateTranslations = {
    {STATE_INIT, {{"en", "Init"}, {"es", "Inicio"}, {"fr", "Initialiser"}}},
    {STATE_STANDBY, {{"en", "Hello"}, {"es", "Hola"}, {"fr", "Bonjour"}}},
    {STATE_RECORDING, {{"en", "Recording"}, {"es", "Grabando"}, {"fr", "Enregistrement"}}},
    {STATE_VOICE_RECOGNITION, {{"en", "Voice Recognition"}, {"es", "Reconocimiento de Voz"}, {"fr", "Reconnaissance Vocale"}}},
    {STATE_EXECUTE_COMMAND, {{"en", "Execute Command"}, {"es", "Ejecutar Comando"}, {"fr", "Exécuter la Commande"}}},
    {STATE_UNRECOGNIZED_COMMAND, {{"en", "Unrecognized Command"}, {"es", "Comando No Reconocido"}, {"fr", "Commande Non Reconnaissance"}}},
    {STATE_RESET, {{"en", "Reset"}, {"es", "Reiniciar"}, {"fr", "Réinitialiser"}}},
    {STATE_TRANSLATING, {{"en", "Translating"}, {"es", "Traduciendo"}, {"fr", "Traduction"}}}
};

// initialization objects
WiFiClientSecure client;
const char* ssid = "Brown-Guest";

volatile unsigned long lastPressTimeRecord = 0; // Last press time for record button
volatile unsigned long lastPressTimeTrans = 0;  // Last press time for translate button

int lastTimeApiRequest; // track for updating datetime
int lastCalendarAndTempRequest; // track for updating 

// Current state variable (must be volatile because it is modified in ISR)
volatile State currentState = STATE_INIT;

// tft screen object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// dummy text for screen showing 
ShowText currentText = {
  "Hello",
  "Today's event: Demo at 2 PM",
  "Sun Nov 17 2024 12:30pm",
  "5.6 C"
};

// recorder related
SdFs sd;
FsFile file;
I2SRecord i2sRecorder;

// translating relatted
const int ENGLISH = 0, SPANISH = 1, FRENCH = 2;
String languages[4] = {"en", "es", "fr"};
int current_language = ENGLISH; // Always start off with English
// std::map<State, std::map<String, String>> stateTranslations;

// NTP protocol for current local time: 
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 0;

// Google Calendar API details 
String calendarID = "5f3728ad2cfeb9b7846eb7ff605f472e9549a757e074621f4048389327947fb0@group.calendar.google.com";
String calendarApiKey = SECRET_CALENDAR_API_KEY;
const char* server = "www.googleapis.com";
String endpoint = "/calendar/v3/calendars/" + calendarID + "/events?key=" + calendarApiKey;

// Weather API details
String weatherApiKey = SECRET_WEATHER_API_KEY;
String lat = "41.825226"; // Coordinates for Providence:
String lon = "-71.418884";
const char* weatherServer = "api.openweathermap.org";
String weatherEndpoint = "/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + weatherApiKey;


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
std::map<State, std::map<String, String>> generateStateTranslations();
void IRAM_ATTR onRecordButtonPress();
void IRAM_ATTR onTransButtonPress();


// Initial screen loading waiting all the request
unsigned long showScreenLoading() {
  tft.setRotation(1); // vertical
  tft.fillScreen(0x1A27); // background color

  unsigned long start = micros();

  // Date text color and size
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(4);
  tft.setCursor(5, 13);
  tft.print("Loading..."); // datetime
  
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(5, 50);
  tft.print("Be patient!");

  return micros() - start;
}


// Display a message on screen following your specified format
unsigned long showScreenMessage() {
  tft.setRotation(1); // vertical
  tft.fillScreen(0x1A27); // background color

  unsigned long start = micros();

  // Date text color and size
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(5, 13);
  tft.print(currentText.dateTime.c_str()); // datetime

  tft.setCursor(5, 40);
  tft.print("Temp: " + currentText.temperature); // temperature

  tft.setCursor(285, 40);
  tft.print(toUpperCase(languages[current_language])); // lanaguage

  // split line
  tft.drawFastHLine(0, 60, 320, ILI9341_YELLOW);

  tft.setTextSize(2);
  tft.setCursor(5, 70);
  tft.print(currentText.stateStatus.c_str()); // state status

  tft.setCursor(5, 110);
  tft.print(currentText.eventText.c_str()); // event
  return micros() - start;
}

void setup() {
  Serial.begin(115200);

  pinMode(RECORD_BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRANS_BUTTON_PIN, INPUT_PULLUP);

  // Setup button interrupts
  attachInterrupt(digitalPinToInterrupt(RECORD_BUTTON_PIN), onRecordButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(TRANS_BUTTON_PIN), onTransButtonPress, FALLING);

  ledcAttach(LED_PIN, 5000, 8); // LED channel 0, 5kHz, bit

  tft.begin();
  showScreenLoading(); // loading all needed information

  // // Generate translations dynamically (when comment out, it using the global variable stateTranslations above)
  // Serial.println("Start generate translations");
  // stateTranslations = generateStateTranslations();
  // Serial.println("Generated translations");

  delay(1000);

  currentState = STATE_INIT;

  // Back-initialize the existing WDT first (if it already exists)
  esp_task_wdt_deinit();
  // initial watchdog for waiting 20S, it will reset after that.
  // watchdog struct
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 20000,              // 20 seconds
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1, // monitor two cores
    .trigger_panic = false               // reset after 30 seconds
  };

  // initial watchdog
  esp_err_t err = esp_task_wdt_init(&wdt_config);
  if (err == ESP_OK) {
    Serial.println("Watchdog initialized successfully!");
  } else {
    Serial.print("Watchdog init failed with error: ");
    Serial.println(err);
  }
  // put current process in monitor
  esp_task_wdt_add(NULL);

}

void loop() {

  // pet the dog before the start of each loop to avoid watchdog timeout
  esp_task_wdt_reset();

  // fetch the datetime per mins
  if (millis() - lastTimeApiRequest >= ONE_MIN){
    currentText.dateTime = printLocalTime();
    lastTimeApiRequest = millis();
  }

  // fetch the calendar and temp per 10 mins
  if (millis() - lastCalendarAndTempRequest >= TEN_MINS){
    currentText.temperature = fetchWeather();
    currentText.eventText = fetchCalendar();
    lastCalendarAndTempRequest = millis();
  }

  // update the status message according to the current language
  currentText.stateStatus = getStateTranslation(currentState, languages[current_language]);
  Serial.println("Current State " + currentText.stateStatus);

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

  // pet the dog again before the end of each loop
  esp_task_wdt_reset();
}


// State Functions
void stateInit() {

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    esp_task_wdt_reset(); // pet the dog and avoid resetting during wifi connection
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  // Print time and data on start up:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  const int MAX_ATTEMPTS = 10; // max trying attempts
  int attempts = 0;
  while (attempts < MAX_ATTEMPTS) {

    esp_task_wdt_reset(); // pet the dog and avoid resetting during fetch data

    // Call each function and update the respective variables
    currentText.dateTime = printLocalTime();
    currentText.temperature = fetchWeather();
    currentText.eventText = fetchCalendar();
    esp_task_wdt_reset(); // same

    // Check if all values are valid (not null)
    if (currentText.dateTime != "null" && 
        currentText.temperature != "null" && 
        currentText.eventText != "null") {
        Serial.println("All data fetched successfully!");
        break; // Exit the function as all values are valid
    }

    // Log an error for debugging
    Serial.println("One or more values are null. Retrying...");
    delay(1000); // Optional delay between retries

    attempts++;
  }

  lastTimeApiRequest = millis();
  lastCalendarAndTempRequest = millis();
  
  showScreenMessage();
  
  delay(1500);
  currentState = STATE_STANDBY;

  esp_task_wdt_reset(); // pet dog
}

void stateStandby() {
  showScreenMessage();
  
  unsigned long startTime = millis(); // record the start time of mode
  while (millis() - startTime < 60000) { // 60s

    esp_task_wdt_reset(); // pet the dog regularly while waiting interrupt

    // check interrupt
    if (currentState != STATE_STANDBY) { //interrpt happened
        Serial.println("Interrupt detected! Exiting standby mode.");
        return;
    }

    delay(1); // small delay
  }

  esp_task_wdt_reset(); // pet dog

}

void stateRecording() {

  showScreenMessage();
  delay(RECORD_TIME); // Simulate recording duration
  currentState = STATE_VOICE_RECOGNITION;
  
  esp_task_wdt_reset(); // pet dog
}

void stateVoiceRecognition() {

  showScreenMessage();
  delay(3000); // Simulate analysis time

  // After analyzing audio, decide whether command recognized or not
  // For now, let's assume command recognized:
  currentState = STATE_EXECUTE_COMMAND;

  esp_task_wdt_reset(); // pet dog
}

void stateExecuteCommand() {

  showScreenMessage();

  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(LED_PIN, dutyCycle);
    esp_task_wdt_reset(); // pet dog
    delay(15);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(LED_PIN, dutyCycle);   
    esp_task_wdt_reset(); // pet dog
    delay(15);
  }
  delay(1000);
  currentState = STATE_STANDBY;

  esp_task_wdt_reset(); //pet dog
}

void stateUnrecognizedCommand() {

  showScreenMessage();
  delay(2000);
  currentState = STATE_STANDBY;

  esp_task_wdt_reset(); // pet dog
}

void stateReset() {

  showScreenMessage();
  delay(2000);
  currentState = STATE_INIT;
  
  esp_task_wdt_reset(); // pet dog
}

void stateTranslating() {

  // Get source and target languages
  String source_language, target_language;
  getNextLanguagePair(source_language, target_language);
  
  esp_task_wdt_reset(); // Peg the dog to avoid blocking translation calls for too long
  currentText.stateStatus = translateText(currentText.stateStatus, source_language, target_language);
  esp_task_wdt_reset(); // same reason
  currentText.eventText = translateText(currentText.eventText, source_language, target_language);
  esp_task_wdt_reset(); // same reason

  showScreenMessage();
  delay(1000);
  if(currentText.stateStatus == "Connection Failed!" || currentText.eventText == "Connection Failed!"){
    currentState = STATE_RESET;
  }
  else{
    currentState = STATE_STANDBY;
  }

  esp_task_wdt_reset();
  
}

// output the datetime from api
String printLocalTime(){

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "null";
  }

  // Determine if it's AM or PM
  String period = "AM";
  int hour = timeinfo.tm_hour; // Get the hour in 24-hour format
  if (hour >= 12) {
    period = "PM";
    if (hour > 12) hour -= 12; // Convert to 12-hour format
  } else if (hour == 0) {
    hour = 12; 
  }

  // Print date and time
  Serial.print(&timeinfo, "%A, %B %d %Y "); // Day, Month Date Year
  Serial.print(hour); // 12-hour format hour
  Serial.print(":");
  if (timeinfo.tm_min < 10) Serial.print("0"); // Add leading zero for minutes
  Serial.print(timeinfo.tm_min);
  Serial.print(" ");
  Serial.println(period); // Print AM/PM

  // Format the date
  char buffer[128];
  strftime(buffer, sizeof(buffer), "%a, %b %d %y", &timeinfo); // Formats as Mon, Dec 09 24

  // Construct the full string including hour, minute, and AM/PM
  String finalString = String(buffer) + " " 
                      + String(hour) + ":" 
                      + ((timeinfo.tm_min < 10) ? "0" : "") 
                      + String(timeinfo.tm_min) + " " 
                      + period;

  return finalString;
}

String fetchWeather() {
  Serial.println("in fetchweather");
  // Begin request:
  client.setCACert(weatherCertificate);

  if (client.connect(weatherServer, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET ");
    client.print(weatherEndpoint);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(weatherServer);
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("client connection failed!");
  }

  // Read each character in the JSON response one by one 
  String payload = "";

  while (!client.available()) {
    delay(100);
  }

  while (client.available()) {
    char c = client.read(); // Read character-by-character
    payload += c;
  }

  // Extract JSON body from the response to deserialize
  int bodyIndex = payload.indexOf("\r\n\r\n");
  if (bodyIndex != -1) {
    payload = payload.substring(bodyIndex + 4); // Skip the headers
    // Serial.println("Extracted JSON payload:");
    // Serial.println(payload);
  } else {
    Serial.println("Failed to find JSON body.");
    return "null";
  }

  // Deserialize
  JsonDocument doc; 
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("Failed to deserialize JSON: ");
    Serial.println(error.f_str());
    return "null";
  }

  JsonObject obj = doc.as<JsonObject>();

  // Extract weather details
  const char* description = obj["weather"][0]["description"].as<const char*>();
  const char* icon = obj["weather"][0]["icon"].as<const char*>();
  const float temp = obj["main"]["temp"].as<float>();

  Serial.print(temp);
  Serial.println(" C ");

  client.stop();
  return String(temp) + " C";


}


String fetchCalendar() {
  Serial.println("in fetch calendar!");
  // Connect to client and make http request: 
  client.setCACert(calendarCertificate);
  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET ");
    client.print(endpoint);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("client connection failed!");
  }


  // Read each character in the JSON response one by one 
  String payload = "";
  // delay(5000);

  while (!client.available()) {
    delay(100);
  }

  while (client.available()) {
    char c = client.read(); // Read character-by-character
    payload += c;
  }

  // Extract JSON body from the response to deserialize
  int bodyIndex = payload.indexOf("\r\n\r\n");
  if (bodyIndex != -1) {
    payload = payload.substring(bodyIndex + 4); // Skip the headers

    // Remove any characters before the JSON object
    int jsonStart = payload.indexOf("{");
    if (jsonStart != -1) {
      payload = payload.substring(jsonStart); // Extract JSON starting from the '{'
    } else {
      Serial.println("No valid JSON object found.");
      return "null";
    }

    // Serial.println("Extracted JSON payload:");
    // Serial.println(payload);
  } else {
    Serial.println("Failed to find JSON body.");
    return "null";
  }

  // Deserialize
  JsonDocument doc; // Ensure size is adequate for your JSON structure
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("Failed to deserialize JSON: ");
    Serial.println(error.f_str());
    return "null";
  }

  JsonArray events = doc["items"].as<JsonArray>();

  // Display the time and summary of each event:
  int line = 0;
  String eventBuffer = ""; // Initialize an empty buffer to store events

  for (JsonObject event : events) {
    const char* summary = event["summary"];
    const char* start = event["start"]["dateTime"] | event["start"]["date"];

    // Format each event and append it to the buffer
    eventBuffer += String(start).substring(0, 10); // Add date
    eventBuffer += ": ";
    eventBuffer += summary; // Add event name
    eventBuffer += "\n"; // Add newline for next event

    line++;
    if (line >= 3) break; // Display 3 events
  }

  // Print all events at once
  Serial.println(eventBuffer);

  client.stop();
  return eventBuffer;

}

/*
*  Sends a translation request to LibreTranslate API
*  - String text: The text to be translated 
*  - String source_language: The language of the current text that needs translating
*  - String target_language: The language to translate the current text into
*
*  returns: The translated text as a String
*/
String translateText(String text, String source_language, String target_language) {

  // LibreTranslate API endpoint
  const char* api_url = "www.libretranslate.com";
  String api_key = "49c4aa23-b141-4967-acc4-3352a7c61a30";

  client.setCACert(libre_translate_cert);

  if (!client.connect(api_url, 443)) { // HTTP over port 80
    return "Connection Failed!";
  }


  String encoded_text = encodeText(text);
  //Serial.print("Encoded text: " + encoded_text);

  String postData = 
  "q=" + encoded_text +
  "&source=" + source_language +
  "&target=" + target_language +
  "&api_key=" + api_key;

  client.println("POST /translate HTTP/1.1");
  client.println("Host: www.libretranslate.com");  // Host header with the domain
  client.println("accept: application/json");  // Accept header
  client.println("Content-Type: application/x-www-form-urlencoded");  // Content type header
  client.println("Content-Length: " + String(postData.length()));  // Content length
  client.println("Connection: close");  // Close connection after request
  client.println();  // Blank line to indicate the end of headers

  // Send the form data
  client.println(postData);

  // Serial.println("Making GET request");
  // client.println("GET /languages HTTP/1.1");
  // client.println("Host: www.libretranslate.com");
  // client.println("Connection: close");
  // client.println(); // Blank line to indicate end of headers

  // Wait for the API response
  while(!client.available()){
    delay(100);
  }

  // Read the response, and extract the JSON
  String response = "";
  String JSON = "";
  char meow = ' ';
  bool flag = false; // true when JSON portion of the response has been reached

  while (client.available()) {

    meow = (char)client.read();
    response += meow;

    // Serial.print(meow);

    // If the end of the main response/beginning of the JSON portion has been reached
    if (meow == '\n' && response.endsWith("\r\n\r\n")) {
      flag = true; 
      continue; // avoid adding a newline to the JSON String
    }

    if(flag){
      JSON += meow;
    }

  }

  // Close the connection
  client.stop();

  // Extract the translation
  JsonDocument doc;
  deserializeJson(doc, JSON);
  return doc["translatedText"];
}

/*
*  Takes the given text and translates any non-alphanumeric chatacters into the proper URL encoding
*  - String text: The text to be encoded
*
*  returns: The encoded text as a String
*/
String encodeText(String text) {

  String encoded = "";

  for (int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);

    if (c == ' ') {
      encoded += "%20";  // Encoding space as %20

    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
               c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;  // Characters that don't need encoding
      
    } else {
      // Encode other characters
      encoded += "%" + String(c, HEX);
    }
  }
  return encoded;
}

// Function to switch to the next language and return source/target languages
void getNextLanguagePair(String &source_language, String &target_language) {
    source_language = languages[current_language];
    current_language = (current_language + 1) % 3; // Cycle through ENGLISH, SPANISH, FRENCH
    target_language = languages[current_language];
}


// Function to get the translation of a state
String getStateTranslation(State state, const String& language) {
    // Check if the state exists in the translations
    if (stateTranslations.find(state) != stateTranslations.end()) {
        // Check if the language exists for this state
        if (stateTranslations[state].find(language) != stateTranslations[state].end()) {
            return stateTranslations[state][language];
        }
    }
    return "Translation Not Found";
}

// Function to generate translations for each state dynamically
std::map<State, std::map<String, String>> generateStateTranslations() {
    std::map<State, String> baseStates = {
        {STATE_INIT, "Init"},
        {STATE_STANDBY, "Hello"},
        {STATE_RECORDING, "Recording"},
        {STATE_VOICE_RECOGNITION, "Voice Recognition"},
        {STATE_EXECUTE_COMMAND, "Execute Command"},
        {STATE_UNRECOGNIZED_COMMAND, "Unrecognized Command"},
        {STATE_RESET, "Reset"},
        {STATE_TRANSLATING, "Translating"}
    };

    std::map<State, std::map<String, String>> translations;

    for (const auto& [state, baseText] : baseStates) {
        std::map<String, String> languageMap;

        // Translate baseText to each target language
        for (const auto& targetLanguage : languages) {
            if (targetLanguage == "en") {
                // English is the base language
                languageMap[targetLanguage] = baseText;
            } else {
                // Translate to other languages
                String translatedText = translateText(baseText, "en", targetLanguage);
                languageMap[targetLanguage] = translatedText;
            }
        }

        translations[state] = languageMap;
    }

    return translations;
}

// Function to convert a String to uppercase
String toUpperCase(String input) {
    input.toUpperCase(); // Convert the entire String to uppercase
    return input;
}

// ISR to change the state
void IRAM_ATTR onRecordButtonPress() {
    currentState = STATE_RECORDING;
}

void IRAM_ATTR onTransButtonPress() {
    currentState = STATE_TRANSLATING;
}

