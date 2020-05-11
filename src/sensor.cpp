#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <DFRobot_BMP388_I2C.h>

#include <pb_encode.h>
#include "packet.pb.h"

#include "config.h"
#include "util.h"
#include "sensor.h"

// Maximum protobuf length - keep this as small as possible to conserve memory
#define MAX_PROTOBUF_BYTES 120

// variables for Report By Exception processing
#define DELTA_TEMPERATURE 0.1
#define SENTINEL_TEMPERATURE -99.0
float last_scd30_temperature = SENTINEL_TEMPERATURE;
float last_bmp388_temperature = SENTINEL_TEMPERATURE;
float last_dewpoint = SENTINEL_TEMPERATURE;

#define DELTA_HUMIDITY 1.0
#define SENTINEL_HUMIDITY -1
float last_humidity = SENTINEL_HUMIDITY;

#define DELTA_SCD30_CO2 1.0
#define SENTINEL_CO2 0
float last_co2 = SENTINEL_CO2;

#define DELTA_PRESSURE 0.1
#define SENTINEL_PRESSURE 0
float last_pressure = SENTINEL_PRESSURE;

// sensors
SCD30 scd30;
DFRobot_BMP388_I2C bmp388;

bool has_scd30 = false;
bool has_bmp388 = false;

bool encode_measurement(pb_ostream_t *ostream, const pb_field_iter_t *field, void *const *arg);
bool encode_field(Measurement *m, pb_ostream_t *ostream, const pb_field_iter_t *field);

bool sensor_init()
{
    Wire.begin();

#ifdef HAS_SCD30
    if (scd30.begin() == true)
    {
        has_scd30 = true;
        scd30.setTemperatureOffset(SCD30_DEFAULT_TEMPERATURE_OFFSET);
        scd30.setAmbientPressure(SCD30_DEFAULT_AMBIENT_PRESSURE);
        scd30.setAutoSelfCalibration(true);
    }
#endif

#ifdef HAS_BMP388
    bmp388.begin();
    has_bmp388 = (!((bmp388.readTemperature() == 0) & (bmp388.readPressure() == 0)));
#endif

    return true;
}

uint8_t read_and_encode_sensors(int packet_id, uint8_t *buffer, uint8_t buffer_size)
{
    Meta META = {
        location : location,
        sensor_type : SENSOR_TYPE,
        firmware_version : FIRMWARE_VERSION,
    };
    strcpy(META.chip_id, chip_id);

    Packet packet = Packet_init_zero;

    // Build packet
    packet.packet_id = packet_id;
    packet.has_meta = true;
    packet.meta = META;
    packet.measurements.funcs.encode = encode_measurement;

    // Encode the proto buffer
    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, MAX_PROTOBUF_BYTES);
    if (!pb_encode(&ostream, Packet_fields, &packet))
    {
        Serial.println("Encoding Error");
        Serial.println(ostream.errmsg);
        return ostream.bytes_written;
    }

    return ostream.bytes_written;
}

bool encode_measurement(pb_ostream_t *ostream, const pb_field_iter_t *field, void *const *arg)
{
    Measurement measurement = Measurement_init_default;

    if (has_scd30)
    {
        measurement.sensor = Sensor_SDC30;

        // temperature
        float current_temperature = scd30.getTemperature();
        if (abs(current_temperature - last_scd30_temperature) >= DELTA_TEMPERATURE)
        {
            last_scd30_temperature = current_temperature;

            measurement.type.temperature = current_temperature;
            measurement.which_type = Measurement_temperature_tag;

            Serial.print("T:");
            Serial.print(current_temperature);
            Serial.print(" ");

            if (!encode_field(&measurement, ostream, field))
                return false;
        }

        // humidity
        float current_humidity = scd30.getHumidity();
        if (abs(current_humidity - last_humidity) >= DELTA_HUMIDITY)
        {
            last_humidity = current_humidity;

            measurement.type.humidity = current_humidity;
            measurement.which_type = Measurement_humidity_tag;

            Serial.print("H:");
            Serial.print(current_humidity);
            Serial.print(" ");

            if (!encode_field(&measurement, ostream, field))
                return false;
        }

        // CO2
        float current_co2 = scd30.getCO2();
        if (abs(current_co2 - last_co2) >= DELTA_HUMIDITY)
        {
            last_co2 = current_co2;

            measurement.type.co2 = current_co2;
            measurement.which_type = Measurement_co2_tag;

            Serial.print("CO2:");
            Serial.print(current_co2);
            Serial.print(" ");

            if (!encode_field(&measurement, ostream, field))
                return false;
        }

        // dew point
        float temp_c = scd30.getTemperature();
        float relative_humidity = scd30.getHumidity();
        float current_dewpoint = dew_point(temp_c, relative_humidity);

        if (abs(current_dewpoint - last_dewpoint) >= DELTA_TEMPERATURE)
        {
            last_dewpoint = current_dewpoint;

            measurement.which_type = Measurement_dewpoint_tag;
            measurement.sensor = Sensor_VIRTUAL;
            measurement.type.dewpoint = current_dewpoint;
            if (!encode_field(&measurement, ostream, field))
                return false;
        }
    }

    if (has_bmp388)
    {
        measurement.sensor = Sensor_BMP388;

        // pressure
        float current_pressure = bmp388.readPressure() / 100;

        if (abs(current_pressure - last_pressure) >= DELTA_PRESSURE)
        {
            last_pressure = current_pressure;

            measurement.type.pressure = current_pressure;
            measurement.which_type = Measurement_pressure_tag;

            Serial.print("P:");
            Serial.print(current_pressure);
            Serial.print(" ");

            if (!encode_field(&measurement, ostream, field))
                return false;
        }

        // temperature
        float current_bmp388_temperature = bmp388.readTemperature();
        if (abs(current_bmp388_temperature - last_bmp388_temperature) >= DELTA_TEMPERATURE)
        {
            last_bmp388_temperature = current_bmp388_temperature;

            measurement.type.temperature = current_bmp388_temperature;
            measurement.which_type = Measurement_temperature_tag;

            Serial.print("T:");
            Serial.print(current_bmp388_temperature);

            if (!encode_field(&measurement, ostream, field))
                return false;
        }
    }
    Serial.println();
    return true;
}

bool encode_field(Measurement *m, pb_ostream_t *ostream, const pb_field_iter_t *field)
{
    return pb_encode_tag_for_field(ostream, field) &&
           pb_encode_submessage(ostream, Measurement_fields, m);
}
