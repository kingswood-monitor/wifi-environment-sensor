/**
 * KWConfig.h
 * 
 * A classs for getting setting configuration variables in EEPROM.
 * 
 * Recreates the ESP32 "Preferences" library api for ESP8266
 */
#include <EEPROM.h>
#include <Embedis.h>

class KWPreferences
{
public:
    KWPreferences();
    uint16_t getInt(const char *param, int default_value);
    void putInt(const char *param, int value);
    float getFloat(const char *param, float default_value);
    void putFloat(const char *param, float value);
};

KWPreferences::KWPreferences()
{
    EEPROM.begin(SPI_FLASH_SEC_SIZE);
    Embedis::dictionary(
        "EEPROM",
        SPI_FLASH_SEC_SIZE,
        [](size_t pos) -> char { return EEPROM.read(pos); },
        [](size_t pos, char value) { EEPROM.write(pos, value); },
        []() { EEPROM.commit(); });
}

uint16_t KWPreferences::getInt(const char *param, int default_value)
{
    String the_param = param;
    String the_value;
    if (Embedis::get(the_param, the_value))
        return the_value.toInt();
    else
        return default_value;
}

void KWPreferences::putInt(const char *param, int value)
{
    Embedis::set(String(param), String(value));
}

float KWPreferences::getFloat(const char *param, float default_value)
{
    String the_param = param;
    String the_value;
    if (Embedis::get(the_param, the_value))
        return the_value.toFloat();
    else
        return default_value;
}

void KWPreferences::putFloat(const char *param, float value)
{
    Embedis::set(String(param), String(value));
}
