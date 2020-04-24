#pragma once
#include "DigitalOut.h"
#include "packet.pb.h"

/*************************************************
 * EEPROM configuration settings                */

// Logical id to identify on network
#define CFG_LOCATION_ID 5 // 4:living room, 5:bedroom
// Default refresh millis
#define CFG_REFRESH_MILLIS 2000

// Uncomment to write the LOCATION_ID to EEPROM memory
// #define WRITE_LOCATION_ID_TO_EEPROM

extern uint8_t location_id;
extern uint16_t refresh_millis;

/**************************************************/

// Firmware data
#define FIRMWARE_NAME "WiFi Environment Sensor"
#define FIRMWARE_VERSION "0.1.0"
#define DEVICE_TYPE "ESP8266"

// Sensor type
#define SENSOR_TYPE SensorType_Environment

// SCD30 defaults
#define SCD30_DEFAULT_AMBIENT_PRESSURE 1013
#define SCD30_DEFAULT_TEMPERATURE_OFFSET 0

// ESP8266 chip ID
extern char chip_id[8];

bool init_device();
bool init_mqtt();
bool init_sensors();

void get_set_config();
void identify(int number);
void generate_chip_id();
void display_logo(const char *firmware_name, const char *firmware_version, const char *device_type);