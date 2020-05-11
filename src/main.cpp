/*
    Kingswood Monitoring System
    WiFi Environment Sensor

    Hardware: NodeMCU 8266
    OS:       Arduino
*/

#include <Arduino.h>

#include "config.h"
#include "util.h"
#include "socket.h"
#include "sensor.h"

void setup()
{
  Serial.begin(115200);
  delay(2000);

  if (!init_device())
    Serial.println("ERROR: Failed to initialise device");

  if (!sensor_init())
  {
    Serial.println("ERROR: SCD30 not detected. Freezing...");
    while (1)
      ;
  }
  if (!init_socket())
  {
    Serial.println("ERROR: Failed to connect socket. Freezing...");
    while (1)
      ;
  }
}

uint16_t packet_id = 0;
uint8_t packet_buffer[255];

void loop()
{
  loop_socket();

  uint8_t bytes_written = read_and_encode_sensors(packet_id++, packet_buffer, 255);
  socket_send_measurement(packet_buffer, bytes_written);

  delay(refresh_secs * 1000);
}