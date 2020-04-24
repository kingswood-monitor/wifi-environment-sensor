

bool init_mqtt();
void loop_mqtt();
void reconnect_mqtt();
void publish_measurement(uint8_t *packet, uint8_t bytes_written);
bool read_data(pb_istream_t *stream, const pb_field_iter_t *field, void **arg);
void publish_status(int location_id, const char *topic, const char *data);
void publish_measurement_float(int location_id, int sensor_type, int sensor_id, const char *topic, float val);