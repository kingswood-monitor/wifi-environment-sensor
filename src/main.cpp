/*
    Kingswood Monitoring System
    WiFi Environment Sensor

    Hardware: NodeMCU 8266
    OS:       Arduino
*/

#include <Arduino.h>

#include "config.h"
#include "util.h"
#include "mqtt.h"
#include "sensor.h"

void setup()
{
  Serial.begin(115200);
  delay(2000);

  if (!sensor_init())
  {
    Serial.println("ERROR: SCD30 not detected. Freezing...");
    while (1)
      ;
  }
  if (!util_init_device())
    Serial.println("ERROR: Failed to initialise device");

  initialise_mqtt();
  // if (!init_mqtt())
  //   Serial.println("ERROR: Failed to initialise WiFi");
}

uint16_t packet_id = 0;
uint8_t packet_buffer[255];

void loop()
{
  Serial.println("tick tock");
  // loop_mqtt();

  // uint8_t bytes_written = sensor_read(packet_id++, packet_buffer, 255);
  // mqtt_publish_measurement(packet_buffer, bytes_written);

  delay(refresh_millis);
}