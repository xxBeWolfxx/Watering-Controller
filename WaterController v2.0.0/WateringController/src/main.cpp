#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <string.h>
#include <EEPROM.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#define addressID 1
#define baudRate 9600
#define maskID 5012
#define serverIP "100.100.100.100"
#define port 80
#define reconnectInterval 5000

//Define variables

float espID = 0;
float espRemoteID = 0;

const char *ssid = "LOGIN";
const char *password = "PASSWORD";

WiFiClient client;
StaticJsonDocument<200> document;
WebSocketsClient webSocket;

//END Define variables

//Define functions

void okMessage(String logTrigger, String info)
{
  Serial.println("[OK] + [" + logTrigger + "] " + info);
}

void infoMessage(String logTrigger, String info)
{
  Serial.println("[MESSAGE] + [" + logTrigger + "] " + info);
}

void errorMessage(String logTrigger, String info)
{
  Serial.println("[ERROR] + [" + logTrigger + "] " + info);
}

bool validID(int ID)
{
  espID = EEPROM.read(addressID);
  espID = espID * maskID;

  if (espID == ID)
  {
    Serial.println();
    return true;
  }

  return false;
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
}

//END Define functions

void setup()
{
  Serial.begin(baudRate);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    infoMessage("SETUP", "Waiting...");
    delay(500);
  }

  //websocket
  webSocket.begin(serverIP, port, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(reconnectInterval);

  // put your setup code here, to run once:
}

void loop()
{
  webSocket.loop();
  // put your main code here, to run repeatedly:
}