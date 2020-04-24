#include "packet.pb.h"

#define MAX_PROTOBUF_BYTES 120

bool init_scd30();
uint8_t read_sensor(int packet_id, uint8_t *buffer, uint8_t buffer_size);
