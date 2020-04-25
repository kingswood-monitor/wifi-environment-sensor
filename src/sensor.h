#include "packet.pb.h"

#define MAX_PROTOBUF_BYTES 120

bool sensor_init();
uint8_t sensor_read(int packet_id, uint8_t *buffer, uint8_t buffer_size);