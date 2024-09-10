#pragma once

#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRtext.h>
#include <IRutils.h>

#include "evq.hpp"


const uint64_t IR_DEBOUNCE = 200;


class Ir
{
private:
    IRrecv receiver;
    decode_results results;
    uint64_t last_button_time;
    uint8_t last_button;

    uint8_t get_button();

public:
    void loop(Queues& queues);

    void setup();

    Ir(): receiver(5, 512, 15, true) {};
};
