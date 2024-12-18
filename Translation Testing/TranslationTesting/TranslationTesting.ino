#include <ArduinoJson.h>
#include <WiFi.h>
#include "WiFiClientSecure.h"

// Your WiFi credentials
char* ssid = "Brown-Guest";         

// LibreTranslate API endpoint
const char* apiUrl = "www.libretranslate.com";
String api_key = "49c4aa23-b141-4967-acc4-3352a7c61a30";
int status = WL_IDLE_STATUS;

// WiFiClient instance for making HTTP requests
WiFiClientSecure client;

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  while (!Serial);

  delay(1000);

  // attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED) {
    // Connect to WPA/WPA2 network:
    Serial.println("Attempting to connect to WPA SSID: " + String(ssid));
    status = WiFi.begin(ssid);

    // wait 5 seconds for connection:
    delay(5000);
  }
  Serial.println("Connected to WiFi!");

  Serial.print("\nTest (textIsTranslatedProperly): ");
  Serial.println(textIsTranslatedProperly() ? "Passed" : "Failed");

  Serial.print("\nTest (testLargeInput): ");
  Serial.println(testLargeInput() ? "Passed" : "Failed");

  Serial.print("\nTest (testUnsupportedLanguage): ");
  Serial.println(testUnsupportedLanguage() ? "Passed" : "Failed");

  Serial.print("\nTest (testEmptyTranslation): ");
  Serial.println(testEmptyTranslation() ? "Passed" : "Failed");

  Serial.print("\nTest (testTranslatingToSameLanguage): ");
  Serial.println(testTranslatingToSameLanguage() ? "Passed" : "Failed");

}

bool textIsTranslatedProperly(){

  String original_text = "hello";
  String translated_text = translateText(original_text, "en", "es"); // Translate from English to Spanish

  if(translated_text != "hola") return false;

  original_text = translated_text;
  translated_text = translateText(original_text, "es", "fr"); // Translate from Spanish to French

  if(translated_text != "bonjour") return false;

  original_text = translated_text;
  translated_text = translateText(original_text, "fr", "en"); // Translate from French to English

  if(translated_text != "hello") return false;

  return true;
}

bool testLargeInput(){

  String long_text = "This is a very long sentence that should be tested to see how the translation API handles it.";
  String result = translateText(long_text, "en", "es");
  String expected_result = "Esta es una frase muy larga que debe ser probada para ver cómo la API de traducción lo maneja.";

  return result == expected_result;

}

bool testUnsupportedLanguage(){

  String original_text = "hello";
  String translated_text = translateText(original_text, "en", "zz"); // ZZ is an unsupported language

  //Serial.println(translated_text);
  return translated_text == "zz is not supported";

}

bool testEmptyTranslation(){

  String original_text = "";
  String translated_text = translateText(original_text, "en", "es"); 

  return translated_text == "Invalid request: missing q parameter";
}

bool testTranslatingToSameLanguage(){

  String original_text = "hello";
  String translated_text = translateText(original_text, "en", "en"); 

  return translated_text == "hello";

}

void loop() {
  // Nothing to do here
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

  if(source_language == target_language) return text;

  if (!client.connect(apiUrl, 443)) { // HTTP over port 80
    return "Connection Failed!";
  }

  String encoded_text = encodeText(text);

  String post_data = 
  "q=" + encoded_text +
  "&source=" + source_language +
  "&target=" + target_language +
  "&api_key=" + api_key;

  client.println("POST /translate HTTP/1.1");
  client.println("Host: www.libretranslate.com");  // Host header with the domain
  client.println("accept: application/json");  // Accept header
  client.println("Content-Type: application/x-www-form-urlencoded");  // Content type header
  client.println("Content-Length: " + String(post_data.length()));  // Content length
  client.println("Connection: close");  // Close connection after request
  client.println();  // Blank line to indicate the end of headers

  // Send the form data
  client.println(post_data);

  while (!client.available()){
    delay(100);
  }

  // Wait for the response
  String response = "";
  String JSON = "";
  char meow = ' ';
  bool flag = false;
  while (client.available()) {

    meow = (char)client.read();
    response += meow;

    // Serial.print(meow);

    // Detect the end of the generic server response and the beginning of the JSON data
    if (meow == '\n' && response.endsWith("\r\n\r\n")) {
      flag = true;  // Begin reading JSON body
      continue; // avoid adding a newline to the JSON String
    }

    if(flag){
      JSON += meow;
    }

  }

  // Close the connection
  client.stop();

  JsonDocument doc;
  deserializeJson(doc, JSON);

  String translated_text = doc["translatedText"];

  if(translated_text == "null"){
    return doc["error"];
  }else{
    return doc["translatedText"];
  }
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
