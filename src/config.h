#include "packet.pb.h"

/******************************************************************
 * EEPROM configuration settings                                 */

// Logical id to identify on network
#define CFG_LOCATION Location_Bedroom // 4:living room, 5:bedroom
// Default refresh millis
#define CFG_REFRESH_MILLIS 2000

// Uncomment to write the LOCATION_ID to EEPROM memory
// #define WRITE_LOCATION_ID_TO_EEPROM

extern Location location;
extern uint16_t refresh_millis;

/*****************************************************************/

// Firmware data
#define FIRMWARE_NAME "WiFi Environment Sensor"
#define FIRMWARE_VERSION "0.1.0"
#define DEVICE_TYPE "ESP8266"

// Sensor type
#define SENSOR_TYPE SensorType_Environment

// SCD30 defaults
#define SCD30_DEFAULT_AMBIENT_PRESSURE 1013
#define SCD30_DEFAULT_TEMPERATURE_OFFSET 0

// LED pins
#define RED_LED_PIN 16
#define BLUE_LED_PIN 2