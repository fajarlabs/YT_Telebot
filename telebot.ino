/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  Example based on the Universal Arduino Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/FlashLED/FlashLED.ino
*/

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Set debug
const bool DEBUG = true;

// Replace with your network credentials
const char* ssid = "aburumaisha";
const char* password = "thxtoalloh";

// Initialize Telegram BOT
#define BOTtoken "1506076448:AAGX91T49oMHYEnEXsWVY6D07Jem5hYenuk"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "998429121"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int PINOUT0 = 0;
const int PINOUT1 = 2;
bool outState0 = LOW;
bool outState1 = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {

  if (DEBUG) {
    Serial.println("handleNewMessages");
    Serial.println(String(numNewMessages));
  }

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;

    if(DEBUG) {
      Serial.println(text);
    }

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/on1 to turn GPIO 0 ON \n";
      welcome += "/off1 to turn GPIO 0 OFF \n";
      welcome += "/on2 to turn GPIO 2 ON \n";
      welcome += "/off2 to turn GPIO 2 OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if(text.equals("/on1")) {
      bot.sendMessage(chat_id, "PIN 0 state set to ON", "");
      digitalWrite(PINOUT0, HIGH);
      outState0 = true;
    }

    if(text.equals("/off1")){
      bot.sendMessage(chat_id, "PIN 0 state set to OFF", "");
      digitalWrite(PINOUT0, LOW);
      outState0 = false;
    }

    if (text.equals("/on2")) {
      bot.sendMessage(chat_id, "PIN 2  set to ON", "");
      digitalWrite(PINOUT1, HIGH);
      outState1 = true;
    }
    
    if (text.equals("/off2")) {
      bot.sendMessage(chat_id, "PIN 2 set to OFF", "");
      digitalWrite(PINOUT1, LOW);
      outState1 = false;
    }
    
    if (text == "/state") {
      String message = "";
      message += "PIN 0 is "+String(outState0)+" \n";
      message += "PIN 2 is "+String(outState1)+" \n";
      bot.sendMessage(chat_id, message, "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  if(DEBUG) {
    Serial.println("DEVICE READY");
  }

  #ifdef ESP8266
    client.setInsecure();
  #endif

  pinMode(PINOUT0, OUTPUT);
  pinMode(PINOUT1, OUTPUT);
  digitalWrite(PINOUT0, outState0);
  digitalWrite(PINOUT1, outState1);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    if(DEBUG) {
      Serial.println("Connecting to WiFi..");
    }
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      if(DEBUG){
        Serial.println("got response");
      }
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
      
