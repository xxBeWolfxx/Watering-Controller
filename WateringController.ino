#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define kboard A0
#define EEPROMthreshold1 0
#define EEPROMthreshold2 1
#define senosrs1 A1
#define senosrs2 A2

LiquidCrystal_I2C lcd(0x27, 16, 2);

//**************************Define special function**********************************
int KeyboardCheck();
void LCD_swipe(int x);
void LCD_displayLCD(int x);
void SensoreRead();
void WaterPump();

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

};
int threshold1 = 0; //set threshold for each plant
int threshold2 = 0;
int temphold1;
int temphold2;
int counter = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;
int displayLCD = 0; //

void setup()
{
    threshold1 = EEPROM.read(EEPROMthreshold1); //read last value of threshold
    threshold2 = EEPROM.read(EEPROMthreshold2);
    temphold1 = threshold1;
    temphold2 = threshold2;

    lcd.begin();
    lcd.backlight();
    lcd.clear();
    LCD_display(displayLCD);
    displayLCD++;
    delay(1000);
    lcd.clear();
    Serial.begin(9600);
}

void loop()
{
    SensoreRead();
    LCD_display(displayLCD);
    LCD_swipe(KeyboardCheck());
}

int KeyboardCheck()
{
    int reading = analogRead(A0);
    delay(100);
    // int temp = analogRead(A0);
    Serial.println(reading);
    //Serial.println(temp);
    if (reading < 1030 && reading > 1010)
        return 0;
    if (reading < 1010) //check which button has been pushed
    {
        if (reading < 390 && reading > 330)
            return 1;
        if (reading < 70 && reading > 2)
            return -1;
        if (reading < 565 && reading > 425)
            return 2;
        if (reading < 660 && reading > 580)
            return -2;
        if (reading < 700 && reading > 670)
            return 3;
        if (reading < 750 && reading > 700)
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
        }
        if (displayLCD == 4)
        {
            threshold2 = temphold2;
            EEPROM.write(EEPROMthreshold2, threshold2);
            LCD_display(5);
        }
        break;
    }
    case 4:
    {
        LCD_display(6);
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
        lcd.setCursor(9, 0);
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
        sensorValue1 = map(analogRead(senosrs1), 1024, 0, 0, 100);
        sensorValue2 = map(analogRead(senosrs2), 1024, 0, 0, 100);
        counter = 0;
    }
    counter++;
}