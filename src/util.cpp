#include <Arduino.h>

#include "preferences.h"
#include "packet.pb.h"

#include "config.h"
#include "util.h"

KWPreferences preferences;

Location location;
uint16_t refresh_millis;

Kingswood::Pin::DigitalOut util_red_led(RED_LED_PIN);

char chip_id[8];

void identify(int number);
void get_set_config();
void generate_chip_id();
void rand_str(char *dest, size_t length);
void display_logo(const char *firmware_name, const char *firmware_version);

bool init_device()
{
    Serial.begin(115200);
    delay(2000);

    get_set_config();

    identify(location);
    display_logo(FIRMWARE_NAME, FIRMWARE_VERSION);

    generate_chip_id();

    return true;
}

void get_set_config()
{
#ifdef WRITE_LOCATION_ID_TO_EEPROM
    preferences.putInt("LOCATION", (int)CFG_LOCATION);
    preferences.putInt("REFRESH_MILLIS", CFG_REFRESH_MILLIS);

    location = CFG_LOCATION;
    refresh_millis = CFG_REFRESH_MILLIS;

    Serial.println("INFO: Wrote settings to EEPROM");
    Serial.println("INFO: Remember to comment WRITE_LOCATION_ID_TO_EEPROM in util.h");
#else

    location = (Location)preferences.getInt("LOCATION", CFG_LOCATION);
    refresh_millis = preferences.getInt("REFRESH_MILLIS", CFG_REFRESH_MILLIS);

    Serial.println("INFO: Loaded configuration from EEPROM");
#endif
    Serial.print("INFO: LOCATION_ID: ");
    Serial.println(location);
    Serial.print("INFO: REFRESH_MILLIS: ");
    Serial.println(refresh_millis);
}

void identify(int number)
{
    util_red_led.begin();
    util_red_led.activeLow();
    util_red_led.turnOff();
    delay(1000);
    util_red_led.blink(number, 200);
    util_red_led.turnOff();
    delay(1000);
}

void generate_chip_id()
{
    sprintf(chip_id, "%08X\0", ESP.getChipId());
}

void display_logo(const char *title, const char *version)
{
    char strap_line[200];
    sprintf(strap_line, "                  |___/  %s v%s", title, version);

    Serial.println("  _  __ _                                                _ ");
    Serial.println(" | |/ /(_) _ __    __ _  ___ __      __ ___    ___    __| |");
    Serial.println(" | ' / | || '_ \\  / _` |/ __|\\ \\ /\\ / // _ \\  / _ \\  / _` |");
    Serial.println(" | . \\ | || | | || (_| |\\__ \\ \\ V  V /| (_) || (_) || (_| |");
    Serial.println(" |_|\\_\\|_||_| |_| \\__, ||___/  \\_/\\_/  \\___/  \\___/  \\__,_|");
    Serial.println(strap_line);
    Serial.println();
}