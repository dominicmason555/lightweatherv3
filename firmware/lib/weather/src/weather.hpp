#pragma once

#include <array>
#include <Arduino.h>

#include "Adafruit_Sensor.h"
#include "Adafruit_BMP3XX.h"
#include "Adafruit_AHTX0.h"

#include "evq.hpp"


const uint64_t STATE_LENGTH = 60000;


enum class weather_st
{
    reading_1,
    reading_2,
    reading_3,
    reading_4,
    reading_5
};


class Weather
{
private:
    Adafruit_BMP3XX bmp;
    Adafruit_AHTX0 aht;

    std::array<double, 5> a_temperatures;
    std::array<double, 5> a_humidities;
    std::array<double, 5> b_temperatures;
    std::array<double, 5> b_pressures;

    weather_st state;
    uint64_t last_update;

    void take_reading(size_t number);
    void send_it(Queues& queues);

public:
    bool setup();
    void loop(Queues& queues);

    Weather():
        a_temperatures{0},
        a_humidities{0},
        b_temperatures{0},
        b_pressures{0},
        state(weather_st::reading_1)
        {};
};
