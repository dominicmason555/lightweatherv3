#include <Arduino.h>

#include <Wire.h>
#include <array>

#include "ESP8266WiFi.h"
#include "AsyncMqttClient.h"
#include "ArduinoJSON.h"
#include "IRremoteESP8266.h"

#include "evq.hpp"
#include "ir.hpp"
#include "io.hpp"
#include "mqtt.hpp"
#include "weather.hpp"
#include "credentials.hpp"


Queues queues;
Ir ir;
Mqtt mqtt;
Io io;
Weather weather;

const uint32_t COUNT_MAX = 120; // One minute until restart
uint32_t restart_countdown = COUNT_MAX;


void check_wifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        uint8_t connection_attempts = 0;
        while (WiFi.status() != WL_CONNECTED && connection_attempts < 50)
        {
            delay(500);
            Serial.print(".");
            connection_attempts++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nConnected to WiFi");
        }
        else
        {
            Serial.println("Failed to connect to WiFi, restarting...");
            ESP.restart();
        }
    }
}


void check_mqtt()
{
    if (mqtt.is_connected())
    {
        restart_countdown = COUNT_MAX;
    }
    else
    {
        restart_countdown--;
        mqtt.connect();
        delay(500);
    }
    if (restart_countdown == 0)
    {
        Serial.println("Failed to reconnect to MQTT, restarting...");
        ESP.restart();
    }
}


void connect_wifi()
{
    Serial.print("Connecting to Wi-Fi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, WiFi_Pass);
    check_wifi();
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
}


void setup()
{
    Wire.begin(2, 14);
    pinMode(13, OUTPUT);
    pinMode(4, OUTPUT);
    Serial.begin(115200);

    connect_wifi();

    ir.setup();
    io.setup();
    weather.setup();
    mqtt.setup(&queues);
}

void loop()
{
    check_wifi();
    check_mqtt();
    ir.loop(queues);
    weather.loop(queues);
    mqtt.loop(queues);
    io.loop(queues);
}
