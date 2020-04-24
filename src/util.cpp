#include <Arduino.h>
#include "preferences.h"
#include "config.h"
#include "util.h"

KWPreferences preferences;

#define RED 16
#define BLUE 2

uint16_t refresh_millis = CFG_REFRESH_MILLIS;
uint8_t location_id = CFG_LOCATION_ID;

char chip_id[8];

void get_set_config()
{
#ifdef WRITE_LOCATION_ID_TO_EEPROM
    preferences.putInt("LOCATION_ID", CFG_LOCATION_ID);
    preferences.putInt("REFRESH_MILLIS", CFG_REFRESH_MILLIS);

    location_id = CFG_LOCATION_ID;
    refresh_millis = CFG_REFRESH_MILLIS;

    Serial.println("INFO: Wrote settings to EEPROM");
    Serial.println("INFO: Remember to comment WRITE_LOCATION_ID_TO_EEPROM in util.h");
#else
    location_id = preferences.getInt("LOCATION_ID", CFG_LOCATION_ID);
    refresh_millis = preferences.getInt("REFRESH_MILLIS", CFG_REFRESH_MILLIS);

    Serial.println("INFO: Loaded settings from EEPROM");
#endif
    Serial.print("      - LOCATION_ID: ");
    Serial.println(location_id);
    Serial.print("      - REFRESH_MILLIS: ");
    Serial.println(refresh_millis);
}

bool init_device(Kingswood::Pin::DigitalOut *red, Kingswood::Pin::DigitalOut *blue)
{
    Serial.begin(115200);
    delay(2000);

    blue_led.begin();
    blue_led.activeLow();
    blue_led.turnOn();
    red_led.begin();
    red_led.activeLow();
    red_led.turnOff();

    identify(location_id);

    *red = Kingswood::Pin::DigitalOut(RED);
    *blue = Kingswood::Pin::DigitalOut(BLUE);
    get_set_config();
    display_logo(FIRMWARE_NAME, FIRMWARE_VERSION, DEVICE_TYPE);
    generate_chip_id();

    return true;
}

void display_logo(const char *title, const char *version, const char *type)
{
    char strap_line[200];
    sprintf(strap_line, "                  |___/  %s v%s on %s", title, version, type);

    Serial.println("  _  __ _                                                _ ");
    Serial.println(" | |/ /(_) _ __    __ _  ___ __      __ ___    ___    __| |");
    Serial.println(" | ' / | || '_ \\  / _` |/ __|\\ \\ /\\ / // _ \\  / _ \\  / _` |");
    Serial.println(" | . \\ | || | | || (_| |\\__ \\ \\ V  V /| (_) || (_) || (_| |");
    Serial.println(" |_|\\_\\|_||_| |_| \\__, ||___/  \\_/\\_/  \\___/  \\___/  \\__,_|");
    Serial.println(strap_line);
    Serial.println();
}

void generate_chip_id()
{
    sprintf(chip_id, "%08X", ESP.getChipId());
}

void identify(int number)
{
    red_led.turnOff();
    delay(1000);
    red_led.blink(number, 200);
    red_led.turnOff();
    delay(1000);
}