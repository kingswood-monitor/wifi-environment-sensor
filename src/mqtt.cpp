#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "packet.pb.h"
#include <pb_decode.h>

#include "util.h"
#include "mqtt.h"
#include "secrets.h"

#define MQTT_SERVER_IP IPAddress(192, 168, 1, 30)
#define MQTT_MAX_TOPIC_LENGTH 50

WiFiClient espClient;
PubSubClient mqttClient(espClient);

char status_topic[MQTT_MAX_TOPIC_LENGTH];
char command_topic[MQTT_MAX_TOPIC_LENGTH];

bool init_wifi();
void mqtt_callback(char *topic, byte *payload, unsigned int length);
bool read_data(pb_istream_t *stream, const pb_field_iter_t *field, void **arg);
void publish_status(int location_id, const char *topic, const char *data);
void publish_measurement_float(int location_id, int sensor_type, int sensor_id, const char *topic, float val);

bool initialise_mqtt()
{
    Serial.print("INFO: Connecting to ");
    Serial.print(SSID_NAME);
    Serial.print("...");

    // Connnect to WiFi
    WiFi.begin(SSID_NAME, SSID_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // Initialise MQTT client
    mqttClient.setServer(MQTT_SERVER_IP, 1883);
    mqttClient.setCallback(mqtt_callback);

    return true;
}

/********************************************************
 * old code below
 */

bool init_mqtt()
{
    init_wifi();
    // mqttClient.setServer(MQTT_SERVER_IP, 1883);
    // mqttClient.setCallback(mqtt_callback);
    // return mqttClient.connected();
}

bool init_wifi()
{
    Serial.print("INFO: Connecting to ");
    Serial.print(SSID_NAME);
    Serial.print("...");

    WiFi.begin(SSID_NAME, SSID_PASS);

    while (WiFi.status() != WL_CONNECTED)
        ;

    // Serial.print(" IP: ");
    // Serial.println(WiFi.localIP());

    // blue_led.turnOn();
}

void reconnect_mqtt()
{
    // Loop until we're reconnected
    while (!mqttClient.connected())
    {
        // blue_led.toggle();
        Serial.print("INFO: Connecting MQTT...");
        // Attempt to connect
        if (mqttClient.connect("arduinoClient"))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            // mqttClient.publish(status_topic, "ONLINE");
            // ... and resubscribe
            // mqttClient.subscribe(command_topic);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(2000);
        }
    }
}

void loop_mqtt()
{
    if (!mqttClient.connected())
        reconnect_mqtt();
    mqttClient.loop();
}

void mqtt_publish_measurement(uint8_t *buffer, uint8_t bytes_written)
{
    // red_led.begin();
    // red_led.activeLow();

    Packet packet = Packet_init_zero;

    // Decode packet
    packet.measurements.funcs.decode = read_data;
    packet.measurements.arg = &packet.meta;
    pb_istream_t stream = pb_istream_from_buffer(buffer, bytes_written);
    if (!(pb_decode(&stream, Packet_fields, &packet)))
        Serial.print("ERROR: Failed to decode measurement packet");

    publish_status(packet.meta.location_id, "firmware", packet.meta.firmware_version);

    // red_led.blink(1, 200);
}

bool read_data(pb_istream_t *stream, const pb_field_iter_t *field, void **arg)
{
    Meta *meta = *(Meta **)arg;

    Measurement measurement;
    if (!pb_decode(stream, Measurement_fields, &measurement))
    {
        Serial.println("read_data.pb_decode = Decode error, false");
        return false;
    }

    switch (measurement.which_type)
    {
    case Measurement_co2_tag:
        publish_measurement_float(meta->location_id, meta->sensor_type, measurement.sensor, "co2", measurement.type.co2);
        break;
    case Measurement_humidity_tag:
        publish_measurement_float(meta->location_id, meta->sensor_type, measurement.sensor, "humidity", measurement.type.humidity);
        break;
    case Measurement_temperature_tag:
        publish_measurement_float(meta->location_id, meta->sensor_type, measurement.sensor, "temperature", measurement.type.temperature);
        break;
    }
}

void publish_measurement_float(int location_id, int sensor_type, int sensor_id, const char *topic, float val)
{
    const char measurement_topic[] = "sensor/measurement";

    char val_buf[10];
    char topic_buf[MQTT_MAX_TOPIC_LENGTH];

    sprintf(topic_buf, "%s/%d/%d/%d/%s", measurement_topic, location_id, sensor_type, sensor_id, topic);
    sprintf(val_buf, "%.3f", val);

    // Serial.print(topic_buf);
    // Serial.print(" ");
    // Serial.println(val_buf);

    mqttClient.publish(topic_buf, val_buf);
}

void publish_status(int location_id, const char *topic, const char *data)
{
    const char status_topic[] = "sensor/status";

    char topic_buf[MQTT_MAX_TOPIC_LENGTH];
    sprintf(topic_buf, "%s/%d/%s", status_topic, location_id, topic);

    // Serial.print(topic_buf);
    // Serial.print(" ");
    // Serial.println(data);

    mqttClient.publish(topic_buf, data);
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}