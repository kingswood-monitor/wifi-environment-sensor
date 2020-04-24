#include "DigitalOut.h"

bool initialise_mqtt();

// old below

bool init_mqtt();
void loop_mqtt();
void reconnect_mqtt();
void mqtt_publish_measurement(uint8_t *packet, uint8_t bytes_written);
