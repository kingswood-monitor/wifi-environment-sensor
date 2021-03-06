#include "packet.pb.h"

/******************************************************************
 * EEPROM configuration settings                                 */

// Defaults
// Logical id to identify on network
// #define CFG_LOCATION Location_lounge // 4:living room, 5:bedroom
#define CFG_LOCATION Location_bedroom // 4:living room, 5:bedroom

// Default refresh millis
#define CFG_REFRESH_SECS 30

// Uncomment the line below write the LOCATION_ID to EEPROM memory
// #define WRITE_LOCATION_ID_TO_EEPROM

extern Location location;
extern uint16_t refresh_secs;

/*****************************************************************/

// Sockets server host
#define SERVER_HOST "192.168.1.30"
#define SERVER_PORT 1880

// Sensor type
#define SENSOR_TYPE SensorType_environment

// Sensors
#define HAS_SCD30
// #define HAS_BMP388

// Firmware data
#define FIRMWARE_NAME "WiFi Environment Sensor"
#define FIRMWARE_VERSION "1.0.0"
#define DEVICE_TYPE "ESP8266"

// SCD30 defaults
#define SCD30_DEFAULT_AMBIENT_PRESSURE 1013
#define SCD30_DEFAULT_TEMPERATURE_OFFSET 0

// LED pins
#define RED_LED_PIN 16
#define BLUE_LED_PIN 2

// ESP8266 chip ID
extern char chip_id[8];