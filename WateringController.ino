#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define kboard A0
#define EEPROMthreshold1 0
#define EEPROMthreshold2 1
#define senosrs1 A1
#define senosrs2 A2

LiquidCrystal_I2C lcd(0x27, 16, 2);

//**************************Define special function**********************************
int KeyboardCheck();     //checking if button was pressed
void LCD_swipe(int x);   //changing scenes on LCD
void LCD_display(int x); //printing LCD
void SensoreRead();      //reading values from sensors
void WaterPump();        //turn on or off diods and pumps

//**************************Variables********************************
String words[7][2] =
    {
        {"  Watering  ", "Controller v1.0"},
        {"1.Plant:", "Humidity:"},
        {"2.Plant:", "Humidity:"},
        {"1.Changing", "Value:"},
        {"2.Changing", "Value:"},
        {"      SAVED     ", "       !!       "},
        {"     Return     ", "       !!       "}

};                  //declare all scenes
int threshold1 = 0; //set threshold for each plant
int threshold2 = 0;
int temphold1; //temporary value of thresholds, which disappear after turnig off
int temphold2;
int counter = 0; //counters which will count every period of every executing
int counterPomp1 = 0;
int counterPomp2 = 0;
int sensorValue1 = 0; //value of humidity sensor
int sensorValue2 = 0;
int displayLCD = 0; //which scene is showing now

void setup()
{
    for (int i = 4; i < 10; i++)
        pinMode(i, 1);

    threshold1 = EEPROM.read(EEPROMthreshold1); //read last value of threshold
    threshold2 = EEPROM.read(EEPROMthreshold2);

    temphold1 = threshold1;
    temphold2 = threshold2;

    lcd.begin();
    lcd.backlight();
    lcd.clear();
    LCD_display(displayLCD); //show first scene on LCD
    displayLCD++;
    delay(1800);
    lcd.clear();
    sensorValue1 = map(analogRead(senosrs1), 900, 0, 0, 100);
    sensorValue2 = map(analogRead(senosrs2), 900, 0, 0, 100);
    //Serial.begin(9600);
}

void loop()
{
    SensoreRead();
    LCD_display(displayLCD);
    LCD_swipe(KeyboardCheck());
    WaterPump();
}

//******************FUNCTIONS*********************
int KeyboardCheck()
{
    int reading = analogRead(A0);
    delay(100);
    // int temp = analogRead(A0);
    //Serial.println(reading);
    //Serial.println(temp);
    if (reading < 1025 && reading > 950)
        return 0;
    if (reading < 950) //check which button has been pushed
    {
        if (reading < 450 && reading > 380) //Right button
            return 1;
        if (reading < 40) //left button
            return -1;
        if (reading < 650 && reading > 600) //Up button
            return 2;
        if (reading < 770 && reading > 700) //Down button
            return -2;
        if (reading < 870 && reading > 770) //Save button
            return 3;
        if (reading < 910 && reading > 870) //Return button
            return 4;
    }
}

void LCD_swipe(int x)
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
void SensoreRead()
{
    if (counter == 50)
    {
        sensorValue1 = map(analogRead(senosrs1), 900, 0, 0, 100);
        sensorValue2 = map(analogRead(senosrs2), 900, 0, 0, 100);
        counter = 0;
    }
    counter++;
}

void WaterPump()
{
    if (sensorValue1 > threshold1)
    {
        digitalWrite(4, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 1);
    }
    if (sensorValue2 > threshold2)
    {
        digitalWrite(5, 0);
        digitalWrite(8, 1);
        digitalWrite(9, 0);
    }
    if (sensorValue1 <= threshold1)
    {
        digitalWrite(6, 1);
        digitalWrite(7, 0);
        if (counterPomp1 < 50)
        {
            digitalWrite(4, 1);
            counterPomp1 = 0;
        }
        counterPomp1++;
    }
    if (sensorValue2 <= threshold2)
    {
        digitalWrite(8, 0);
        digitalWrite(9, 1);
        if (counterPomp2 < 50)
        {
            digitalWrite(5, 1);
            counterPomp2 = 0;
        }
        counterPomp2++;
    }
}