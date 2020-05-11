#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h>

#include <pb_encode.h>
#include "packet.pb.h"

#include "config.h"
#include "sensor.h"

float last_bmp388_temperature = SENTINEL_TEMPERATURE;
SCD30 scd30;
DFRobot_BMP388_I2C bmp388;

bool has_scd30 = false;
bool has_bmp388 = false;

bool encode_measurement(pb_ostream_t *ostream, const pb_field_iter_t *field, void *const *arg);
bool encode_field(Measurement *m, pb_ostream_t *ostream, const pb_field_iter_t *field);

bool sensor_init()
{
    Wire.begin();
    if (scd30.begin() == true)
    {
        has_scd30 = true;
        scd30.setTemperatureOffset(SCD30_DEFAULT_TEMPERATURE_OFFSET);
        scd30.setAmbientPressure(SCD30_DEFAULT_AMBIENT_PRESSURE);
        scd30.setAutoSelfCalibration(true);
        return true;
    }
    else

#ifdef HAS_BMP388
    bmp388.begin();
    has_bmp388 = (!((bmp388.readTemperature() == 0) & (bmp388.readPressure() == 0)));
#endif

}

uint8_t sensor_read(int packet_id, uint8_t *buffer, uint8_t buffer_size)
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
    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, 100);
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

        measurement.which_type = Measurement_temperature_tag;
        measurement.type.temperature = scd30.getTemperature();
        Serial.print("T:");
        Serial.print(scd30.getTemperature());
        Serial.print(" ");
        if (!encode_field(&measurement, ostream, field))
            return false;

        measurement.which_type = Measurement_humidity_tag;
        measurement.type.humidity = scd30.getHumidity();
        Serial.print("H:");
        Serial.print(scd30.getHumidity());
        Serial.print(" ");
        if (!encode_field(&measurement, ostream, field))
            return false;

        measurement.which_type = Measurement_co2_tag;
        measurement.type.co2 = scd30.getCO2();
        Serial.print("CO2:");
        Serial.println(scd30.getCO2());
        if (!encode_field(&measurement, ostream, field))
            return false;
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
    return true;
}

bool encode_field(Measurement *m, pb_ostream_t *ostream, const pb_field_iter_t *field)
{
    return pb_encode_tag_for_field(ostream, field) &&
           pb_encode_submessage(ostream, Measurement_fields, m);
}
