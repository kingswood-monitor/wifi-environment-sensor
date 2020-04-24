
#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

#include "socket.h"

const char *ssid = "Kingswood";
const char *password = "wbtc0rar";

char path[] = "/";
char host[] = "192.168.1.30";

WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void connect_websocket()
{

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(5000);

    // Connect to the websocket server
    if (client.connect(host, 1880))
    {
        Serial.println("Connected");
    }
    else
    {
        Serial.println("Connection failed.");
        while (1)
        {
            // Hang on failure
        }
    }

    // Handshake with the server
    webSocketClient.path = path;
    webSocketClient.host = host;
    if (webSocketClient.handshake(client))
    {
        Serial.println("Handshake successful");
    }
    else
    {
        Serial.println("Handshake failed.");
        while (1)
        {
            // Hang on failure
        }
    }
}