//v2.0.0

#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 124 // OLED display width, in pixels
#define SCREEN_HEIGHT 28 // OLED display height, in pixels

#define EEPROMthreshold1 0
#define EEPROMthreshold2 1
#define EEPROMtemperature 2

#define sensors1 A0
#define sensors2 A1
#define sensors3 A2
#define TemperatureSensor A3

#define PUMP1 13 //YOU MUST CHECK if this is correct
#define PUMP2 11

//**************************Define special function**********************************
void SensoreRead(); //reading values from sensors
void WaterPump();   //turn on or off diods and pumps

//**************************Variables********************************
String words[7][2] =
    {
        {"Watering", "Controller v2.0.0"},
        {"Plant:", "Humidity:"},
        {"Temperature:", "Period:"},
        {"1.Plant", "2.Plant"},
        {"2.Changing", "Value:"},
        {"     SAVED!     ", "       Return       "},
        {"     >     ", "ESC"}

};                           //declare all scenes
Adafruit_SSD1306 display(9); //

int threshold1 = 0; //set threshold for each plant
int threshold2 = 0;
int Thresholdtemperature = 0;
int temphold1; //temporary value of thresholds, which disappear after turnig off
int temphold2;

int sensorTemperature = 0;
int sensorHUM1 = 0; //value of humidity sensor
int sensorHUM2 = 0;
int sensorBright = 0;

int tabValue[10][4];
int screenOLED = 0;                                    //which scene is showing now
int workingPeriod[] = {2, 4, 5, 10, 30, 60, 120};      //how long the pump have to works
int workingIntensity[] = {5, 10, 20, 40, 50, 80, 100}; //how many energy need to

//*******************************counters***********************************

int counter = 0; //counters which will count every period of every executing
int counterPomp1 = 0;
int counterPomp2 = 0;

void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    threshold1 = EEPROM.read(EEPROMthreshold1); //read last value of threshold
    threshold2 = EEPROM.read(EEPROMthreshold2);
    Thresholdtemperature = EEPROM.read(EEPROMtemperature);

    temperature = Thresholdtemperature;
    temphold1 = threshold1;
    temphold2 = threshold2;

    DisplayOLED(screenOLED++); //show first scene on LCD

    sensorHUM1 = map(analogRead(sensors1), 1024, 0, 0, 100);
    sensorHUM2 = map(analogRead(sensors2), 1024, 0, 0, 100);
    sensorTemperature = map(analogRead(TemperatureSensor), 1024, 0, 0, 100);
    sensorBright = map(analogRead(sensors3), 1024, 0, 0, 100);
}

void loop()
{

    SensoreRead();
}

void SensoreRead() //add array to make avarage value of sensores
{
    tabValue[counter][0] = map(analogRead(sensors1), 1024, 0, 0, 100);
    tabValue[counter][1] = map(analogRead(sensors2), 1024, 0, 0, 100);
    tabValue[counter][2] = map(analogRead(sensors3), 1024, 0, 0, 100);
    tabValue[counter][3] = map(analogRead(TemperatureSensor), 1024, 0, 0, 100);
    if (counter == 9)
    {
        sensorBright = 0;
        sensorHUM1 = 0;
        sensorHUM2 = 0;
        sensorTemperature = 0;

        for (int i = 0; i < 10; i++)
        {
            sensorHUM1 = sensorHUM1 + tabValue[i][0];
            sensorHUM2 = sensorHUM2 + tabValue[i][1];
            sensorBright = sensorBright + tabValue[i][2];
            sensorTemperature = sensorTemperature + tabValue[i][3];
        }
        counter = 0;
        sensorHUM1 = sensorHUM1 / 10;
        sensorHUM2 = sensorHUM2 / 10;
        sensorTemperature = sensorTemperature / 10;

        return;
    }

    counter++;

    return;
}

void DisplayOLED(int screenOLED)
{
    if (screenOLED == 0)
    {
        display.setCursor(0, 0);
        display.println(words[screenOLED][screenOLED]);
        display.setCursor(0, 8);
        display.println(words[screenOLED][screenOLED + 1]);
        display.display();
    }
    if (screenOLED == 1)
    {
        display.setCursor(0, 0);
        display.print(words[screenOLED][screenOLED]);
        display.setCursor(30, 0);
        display.print(threshold1);
        display.setCursor(0, 8);
        display.println(words[screenOLED][screenOLED + 1]);
        display.setCursor(30, 0);
        display.display();
    }

    delay(2000);
}

void WaterPump(int temperature, int Sensore1, int Sensore2)
{
}