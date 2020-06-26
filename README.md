# Watering-Controller
Project created as a University's exam. It is based on Arduino Nano which controls two water pumps by relays.
## Watering Controller v1.0.0

**Description**:

It is a basic project which has got keypad (microswitches) and LCD to present a menu of aplication. Arduino checks values from humidity sensors and compares them to thresholds, which are saved in EEPROM. If the sensors's values are lower than thresholds's values, pumps start working. Arduino sets logical 0 or 1 on pumps's pins.

**PCB**:
PCB was created in KiCad (soon on GitHub). 



## Watering Controller v2.0.0

**Description**:

> Still in progress

*New version of my project*. I replaced a few components and added OLED(ssd1306) display and encoder. New  design has a RTC (DS1307) which improves a pump's working. 
