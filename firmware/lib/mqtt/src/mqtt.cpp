#include "mqtt.hpp"


void Mqtt::handle_outputs_message(
    char* topic,
    char* payload,
    AsyncMqttClientMessageProperties properties,
    size_t len,
    size_t index,
    size_t total,
    Queues* queues
)
{
    Serial.print("MQTT message: ");
    Serial.println(payload);
    StaticJsonDocument<RECEIVE_CAPACITY> json_in;
    auto err = deserializeJson(json_in, payload);
    if (err)
    {
        Serial.println(err.f_str());
        return;
    }
    const char* output = json_in["output"];
    const char* command = json_in["command"];
    if ((output == nullptr) || command == nullptr)
    {
        Serial.println("Failed to validate JSON");
        return;
    }
    bool valid = true;
    auto output_type = output_t::TERMINAL;
    auto output_command = out_command_t::TOGGLE;
    if (std::strcmp(output, "terminal") == 0)
    {}
    else if (std::strcmp(output, "usb") == 0)
    {
        output_type = output_t::USB;
    }
    else
    {
        valid = false;
    }
    if (std::strcmp(command, "toggle") == 0)
    {}
    else if (std::strcmp(command, "on") == 0)
    {
        output_command = out_command_t::ON;
    }
    else if (std::strcmp(command, "off") == 0)
    {
        output_command = out_command_t::OFF;
    }
    else
    {
        valid = false;
    }
    if (valid)
    {
        queues->outputs.push({output_type, output_command});
    }
}


void Mqtt::send_ir(uint8_t  button_num)
{
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> message;
    message["button"] = button_num;
    char output[128];
    size_t written = serializeJson(message, output, 128);
    uint16_t published = client.publish(ir_topic, 0, false, output, written);
    Serial.print("Published: ");
    Serial.println(published);
}


void Mqtt::send_measurement(measurement_ev measurement)
{
    StaticJsonDocument<JSON_OBJECT_SIZE(4)> message;
    message["btemp"] = measurement.btemp;
    message["atemp"] = measurement.atemp;
    message["humid"] = measurement.humid;
    message["press"] = measurement.press;
    char output[256];
    size_t written = serializeJson(message, output, 256);
    uint16_t published = client.publish(weather_topic, 0, false, output, written);
    Serial.print("Published: ");
    Serial.println(published);
}


void Mqtt::onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    uint16_t packetIdSub = client.subscribe(io_topic, 0);
    Serial.print("Subscribing at QoS 0, packetId: ");
    Serial.println(packetIdSub);
}


void Mqtt::onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    if (WiFi.isConnected())
    {
        client.connect();
    }
}


void Mqtt::setup(Queues* queues)
{
    client.setServer(Broker_IP, Broker_Port);
    client.setCleanSession(true);
    AsyncMqttClientInternals::OnMessageUserCallback message_callback =
    [=](
        char* topic,
        char* payload,
        AsyncMqttClientMessageProperties properties,
        size_t len,
        size_t index,
        size_t total
    )
    {
        this->handle_outputs_message(
            topic,
            payload,
            properties,
            len,
            index,
            total,
            queues
        );
    };
    std::function<void(bool)> on_connect =
    [=](bool sesh)
    {
        this->onMqttConnect(sesh);
    };
    std::function<void(AsyncMqttClientDisconnectReason)> on_disconnect =
    [=](AsyncMqttClientDisconnectReason reason)
    {
        this->onMqttDisconnect(reason);
    };
    client.onMessage(message_callback);
    client.onConnect(on_connect);
    client.onDisconnect(on_disconnect);
    client.connect();
}


void Mqtt::connect()
{
    client.connect();
}


bool Mqtt::is_connected()
{
    return client.connected();
}


void Mqtt::loop(Queues& queues)
{
    while (queues.ir_buttons.available())
    {
        uint8_t button_num = queues.ir_buttons.pop();
        Serial.print("MQTT got IR button ");
        Serial.println(button_num);
        if (client.connected())
        {
            send_ir(button_num);
        }
        else
        {
            Serial.println("Can't send ir button, MQTT not connected");
        }
    }
    while (queues.measurements.available())
    {
        measurement_ev measurement = queues.measurements.pop();
        if (client.connected())
        {
            send_measurement(measurement);
        }
        else
        {
            Serial.println("Can't send measurement, MQTT not connected");
        }
    }
}
