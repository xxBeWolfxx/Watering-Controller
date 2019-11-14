#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define kboard A0
#define EEPROMthreshold1 0
#define EEPROMthreshold2 1

LiquidCrystal_I2C lcd(0x27, 16, 2);

//**************************Define special function**********************************
int KeyboardCheck();
void LCD_swipe(int x);
void LCD_display(int x);

//**************************Variables********************************
String words[5][2] = {
    {"  Watering  ", "Controller v1.0"},
    {"1.Plant:", "Humidity:"},
    {"2.Plant:", "Humidity:"},
    {"1.Changing", "Value:"},
    {"2.Changing", "Value:"}

};
int threshold1 = 0; //set threshold for each plant
int threshold2 = 0;
int display = 0; //

void setup()
{
    threshold1 = EEPROM.read(EEPROMthreshold1); //read last value of threshold
    threshold2 = EEPROM.read(EEPROMthreshold2);

    lcd.begin();
    lcd.backlight();
    lcd.clear();
    LCD_display(display);
    display++;
    delay(1000);
    lcd.clear();
}

void loop()
{
    LCD_display(display);
    LCD_swipe(KeyboardCheck());
    lcd.clear();
}

int KeyboardCheck()
{
    int reading = analogRead(kboard);
    int solution = 0;

    if (reading != 0)
    {
        //map solution
    }

    return solution;
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
        if (display == 1)
            display = 4;
        else
            display--;
    }

    case 1:
    case 2:
    {
        if (display == 4)
            display = 1;
        else
            display++;
        break;
    }
    case 3:
    {
        lcd.setCursor(6, 1);
        lcd.cursor_on();
        if (display == 3)
        {
            threshold1 += 5;
            lcd.print(threshold1);
        }
        if (display == 4)
        {
            threshold2 += 5;
            lcd.print(threshold2);
        }
        break;
    }
    case 4:

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
}