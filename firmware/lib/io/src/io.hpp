#pragma once

#include <Arduino.h>

#include "evq.hpp"


const uint8_t USB_PIN = 13;
const uint8_t TERMINAL_PIN = 4;


class Io
{
    bool usb_state;
    bool terminal_state;

    void handle_usb(out_command_t command);
    void handle_terminal(out_command_t command);

public:
    void setup();
    void loop(Queues& queues);

    Io(): usb_state(false), terminal_state(false) {};
};
