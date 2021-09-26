#include "PIDcontroller.h"
#include "WiFiClientSecure.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <WebSocketsClient.h>
#include <WiFi.h>
#include <string.h>

#define addressID 1
#define addressScaler 2
#define baudRate 9600
#define serverIP "192.168.0.250"
#define port 80
#define reconnectInterval 5000
#define ONE_WIRE_BUS 32

//Define variables

float espID = 0;
float espRemoteID = 0;

const char *ssid = "2.4G_echostar_5c82";
const char *password = "00035022";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorTemperature(&oneWire);

WiFiClient client;
StaticJsonDocument<200> document;
WebSocketsClient webSocket;

//END Define variables

//Define functions

void okMessage(String logTrigger, String info) {
    Serial.println("[OK] + [" + logTrigger + "] " + info);
}

void infoMessage(String logTrigger, String info) {
    Serial.println("[MESSAGE] + [" + logTrigger + "] " + info);
}

void errorMessage(String logTrigger, String info) {
    Serial.println("[ERROR] + [" + logTrigger + "] " + info);
}

bool validID(int ID) {
    espID = EEPROM.read(addressID);
    espID = espID * EEPROM.read(addressScaler);

    if (espID == ID) {
        okMessage("ID VALIDATION", "ID OKAY");
        return true;
    }

    errorMessage("ID VALIDATION", "ID WRONG");

    return false;
}

float readTemnperature() {
    sensorTemperature.requestTemperatures();
    float temperature = sensorTemperature.getTempCByIndex(0);
    return temperature;
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
}

//END Define functions

void setup() {
    Serial.begin(baudRate);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        infoMessage("SETUP", "Waiting...");
        delay(500);
    }

    //websocket
    webSocket.begin(serverIP, port, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(reconnectInterval);

    // put your setup code here, to run once:
}

void loop() {
    webSocket.loop();
    // put your main code here, to run repeatedly:
}