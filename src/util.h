#pragma once
#include "DigitalOut.h"
#include "packet.pb.h"

#include "config.h"

// ESP8266 chip ID
extern char chip_id[8];

bool util_init_device();
bool init_sensors();

void get_set_config();
void identify(int number);
void generate_chip_id();
void rand_str(char *dest, size_t length);
void display_logo(const char *firmware_name, const char *firmware_version, const char *device_type);