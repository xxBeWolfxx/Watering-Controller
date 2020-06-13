//v2.0.0

#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define EEPROMthreshold1 0
#define EEPROMthreshold2 1
#define senosrs1 A0
#define senosrs2 A1
#define senosrs3 A2
#define PUMP1 13 //YOU MUST CHECK if this is correct
#define PUMP2 11

#define NUMFLAKES 10 // Number of snowflakes in the animation example

//**************************Define special function**********************************
//void LCD_swipe(int x);   //changing scenes on LCD
//void LCD_display(int x); //printing LCD
void SensoreRead(); //reading values from sensors
void WaterPump();   //turn on or off diods and pumps

//**************************Variables********************************
String words[7][2] =
    {
        {"  Watering  ", "Controller v2.0.0"},
        {"1.Plant:", "Humidity:"},
        {"2.Plant:", "Humidity:"},
        {"1.Changing", "Value:"},
        {"2.Changing", "Value:"},
        {"     SAVED!     ", "       !!       "},
        {"     Return     ", "       !!       "}

}; //declare all scenes
Adafruit_SSD1306 display(4);

int threshold1 = 0; //set threshold for each plant
int threshold2 = 0;
int temphold1; //temporary value of thresholds, which disappear after turnig off
int temphold2;
int sensorTemperature;
int sensorValue1 = 0; //value of humidity sensor
int sensorValue2 = 0;
int tabValue[20][3];
int displayOLED = 0;                                   //which scene is showing now
int workingPeriod[] = {2, 4, 5, 10, 30, 60, 120};      //how long the pump have to works
int workingIntensity[] = {5, 10, 20, 40, 50, 80, 100}; //how many energy need to

//*******************************counters***********************************

int counter = 0; //counters which will count every period of every executing
int counterPomp1 = 0;
int counterPomp2 = 0;

void setup()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    for (int j = 2; j < 5; j++)
        pinMode(j, INPUT_PULLUP);

    for (int i = 4; i < 10; i++)
        pinMode(i, 1);

    threshold1 = EEPROM.read(EEPROMthreshold1); //read last value of threshold
    threshold2 = EEPROM.read(EEPROMthreshold2);

    temphold1 = threshold1;
    temphold2 = threshold2;

    //  LCD_display(displayLCD); //show first scene on LCD

    sensorValue1 = map(analogRead(senosrs1), 1024, 0, 0, 100);
    sensorValue2 = map(analogRead(senosrs2), 1024, 0, 0, 100);
}

void loop()
{
    SensoreRead();
}
//******************FUNCTIONS*********************

/*void LCD_swipe(int x)
{
    if (x == 0)
        return;
    lcd.clear();
    switch (x)
    {
    case -1:
    {
        if (displayLCD == 1)
            displayLCD = 4;
        else
            displayLCD--;
        break;
    }
    case 1:
    {
        if (displayLCD == 4)
            displayLCD = 1;
        else
            displayLCD++;
        break;
    }
    case 2:
    {
        if (displayLCD == 3)
        {
            if (temphold1 <= 100)
                temphold1 += 5;
        }
        if (displayLCD == 4)
        {
            if (temphold2 <= 100)
                temphold2 += 5;
        }
        break;
    }
    case -2:
    {
        if (displayLCD == 3)
        {
            if (temphold1 >= 0)
                temphold1 -= 5;
        }
        if (displayLCD == 4)
        {
            if (temphold2 >= 0)
                temphold2 -= 5;
        }
        break;
    }
    case 3:
    {
        if (displayLCD == 3)
        {
            threshold1 = temphold1;
            EEPROM.write(EEPROMthreshold1, threshold1);
            LCD_display(5);
            delay(1000);
            lcd.clear();
        }
        if (displayLCD == 4)
        {
            threshold2 = temphold2;
            EEPROM.write(EEPROMthreshold2, threshold2);
            LCD_display(5);
            delay(1000);
            lcd.clear();
        }
        break;
    }
    case 4:
    {
        LCD_display(6);
        delay(1000);
        lcd.clear();
        if (displayLCD == 3)
        {
            temphold1 = threshold1;
        }
        if (displayLCD == 4)
        {
            temphold2 = threshold2;
        }
        break;
    }
    default:
        break;
    }
}

void LCD_display(int x)
{
    lcd.setCursor(0, 0);
    lcd.print(words[x][0]);
    lcd.setCursor(0, 1);
    lcd.print(words[x][1]);
    if (x == 1 || x == 2)
    {
        lcd.setCursor(8, 0);
        if (x == 1)
        {
            lcd.print(threshold1);
        }
        else if (x == 2)
        {
            lcd.print(threshold2);
        }
        lcd.setCursor(9, 1);
        if (x == 1)
        {
            lcd.print(sensorValue1);
        }
        else if (x == 2)
        {
            lcd.print(sensorValue2);
        }
    }
    else if (x == 3 || x == 4)
    {
        lcd.setCursor(6, 1);
        if (x == 3)
        {
            lcd.print(temphold1);
        }
        else if (x == 4)
        {
            lcd.print(temphold2);
        }
    }
    return;
}
*/
void SensoreRead() //add array to make avarage value of sensores
{
    tabValue[counter][0] = map(analogRead(senosrs1), 1024, 0, 0, 100);
    tabValue[counter][1] = map(analogRead(senosrs2), 1024, 0, 0, 100);
    tabValue[counter][2] = map(analogRead(senosrs2), 1024, 0, 0, 100);
    if (counter == 20)
    {
        for (int i = 0; i < 19; i++)
        {
            sensorValue1 = sensorValue1 + tabValue[i][0];
            sensorValue2 = sensorValue2 + tabValue[i][1];
            sensorTemperature = sensorTemperature + tabValue[i][2];
        }
        counter = 0;
        sensorValue1 = sensorValue1 / 20;
        sensorValue2 = sensorValue2 / 20;
        sensorTemperature = sensorTemperature / 20;

        return;
    }

    counter++;
    sensorValue1 = 0;
    sensorValue2 = 0;

    return;
}

void WaterPump(int temperature, int Sensore1, int Sensore2)
{
}