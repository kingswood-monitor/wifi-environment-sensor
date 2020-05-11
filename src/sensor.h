#include "packet.pb.h"

bool sensor_init();
uint8_t read_and_encode_sensors(int packet_id, uint8_t *buffer, uint8_t buffer_size);