#include "DigitalOut.h"

bool mqtt_init();
void loop_mqtt();
void reconnect_mqtt();
void mqtt_publish_measurement(uint8_t *packet, uint8_t bytes_written);
