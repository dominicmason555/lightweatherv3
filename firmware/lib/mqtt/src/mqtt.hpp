#pragma once

#include <functional>
#include <Arduino.h>
#include "AsyncMqttClient.h"
#include "ArduinoJson.h"

#include "evq.hpp"
#include "credentials.hpp"


const int RECEIVE_CAPACITY = JSON_OBJECT_SIZE(2);


class Mqtt
{
private:
    AsyncMqttClient client;
    void send_ir(uint8_t  button_num);
    void send_measurement(measurement_ev measurement);
    void handle_outputs_message(
        char* topic,
        char* payload,
        AsyncMqttClientMessageProperties properties,
        size_t len,
        size_t index,
        size_t total,
        Queues* queues
    );
    void onMqttConnect(bool sessionPresent);
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);

public:
    void setup(Queues* queues);
    void loop(Queues& queues);
    bool is_connected();
    void connect();
};
