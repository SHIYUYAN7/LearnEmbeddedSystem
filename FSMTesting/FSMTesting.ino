#include <SPI.h>
#include "SdFat.h"
#include "I2SRecord.h"
#include "wave.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "timeAPIKey.h" 
#include "time.h"

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

const char* certificate = 
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

WiFiClientSecure client;
// HTTPClient http;
const char* ssid = "Brown-Guest";

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

#define ONE_MIN 60000

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

int lastTimeApiRequest;

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

const int ENGLISH = 0, SPANISH = 1, FRENCH = 2, CHINESE = 3;
String languages[4] = {"en", "es", "fr", "zh"};
int current_language = ENGLISH; // Always start off with English

// Debounce variables
unsigned long lastDebounceTime = 0;

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

  delay(1000);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  lastTimeApiRequest = millis();


  // Print time and data on start up:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  currentText.dateTime = printLocalTime();
  // Fetch current weather and events on calendar
  currentText.temperature = fetchWeather();
  delay(1000);
  currentText.eventText = fetchCalendar();

  String source_language = languages[current_language];
  String target_language = "zh"; 
  
  Serial.println(current_language);
  currentText.stateStatus = translateText(currentText.stateStatus, source_language, target_language);
  currentText.eventText = translateText(currentText.eventText, source_language, target_language);

  Serial.println(currentText.stateStatus);
  Serial.println(currentText.eventText);

  tft.begin();

  currentState = STATE_INIT;

}

void loop() {

  if (millis() - lastTimeApiRequest >= ONE_MIN){
    currentText.dateTime = printLocalTime();
    lastTimeApiRequest = millis();
  }

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
  currentText.stateStatus = "大家好!";
  showScreenMessage();
  
  delay(2000);
  currentState = STATE_STANDBY;
}

void stateStandby() {
  // currentText.stateStatus = "Hello!";
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

  char buffer[128];
  strftime(buffer, sizeof(buffer), "%A, %B %d %Y", &timeinfo); // Formats day, month, date, year

  // Construct the full string including hour, minute, and AM/PM
  String finalString = String(buffer) + " " 
                    + String(hour) + ":" 
                    + ((timeinfo.tm_min < 10) ? "0" : "") 
                    + String(timeinfo.tm_min) + " " 
                    + String(period);

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
  client.setCACert(certificate);
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
  Serial.print("Encoded text: " + encoded_text);

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

  Serial.println("posted");
  // Wait for the API response
  while(!client.available()){
    delay(100);
  }

  Serial.println("is available");

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
  Serial.println("JSON");
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

/*
*  Cycles the current_language state variable
*/
String nextLanguage(){

  // Chinese is the last language in the order, so it must be set back to the first in the order (english)
  if(current_language == CHINESE){

    return languages[ENGLISH];
  }

  return languages[current_language + 1];
}
