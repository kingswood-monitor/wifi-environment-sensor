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
  if (!init_scd30())
  {
    Serial.println("ERROR: SCD30 not detected. Freezing...");
    while (1)
      ;
  }
  if (!init_device())
    Serial.println("ERROR: Failed to initialise device");

  if (!init_mqtt())
    Serial.println("ERROR: Failed to initialise WiFi");
}

uint16_t packet_id = 0;
uint8_t packet_buffer[255];

void loop()
{
  blue_led.turnOn();
  // // loop_mqtt();

  // uint8_t bytes_written = read_sensor(packet_id++, packet_buffer, 255);
  // publish_measurement(packet_buffer, bytes_written);

  // red_led.blink(1, 100);
  // delay(refresh_millis);
}