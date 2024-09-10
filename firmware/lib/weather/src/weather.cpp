#include "weather.hpp"


void Weather::take_reading(size_t number)
{
    Serial.print("Weather reading ");
    Serial.println(number);
    sensors_event_t a_humidity, a_temperature;
    if (!aht.getEvent(&a_humidity, &a_temperature))
    {
        Serial.println("Failed to read from AHT");
    }
    if (!bmp.performReading())
    {
        Serial.println("Failed to read from BMP");
    }
    a_humidities[number - 1] = a_humidity.relative_humidity;
    a_temperatures[number - 1] = a_temperature.temperature;
    b_temperatures[number - 1] = bmp.temperature;
    b_pressures[number - 1] = bmp.pressure / 100.0;
}


void Weather::send_it(Queues& queues)
{
    std::sort(a_temperatures.begin(), a_temperatures.end());
    std::sort(a_humidities.begin(), a_humidities.end());
    std::sort(b_temperatures.begin(), b_temperatures.end());
    std::sort(b_pressures.begin(), b_pressures.end());
    measurement_ev ev = {
        b_temperatures[2],
        b_pressures[2],
        a_humidities[2],
        a_temperatures[2]
    };
    Serial.print("BMP Temp: ");
    Serial.print(ev.btemp);
    Serial.print(", AHT Temp: ");
    Serial.print(ev.atemp);
    Serial.print(", Pressure: ");
    Serial.print(ev.press);
    Serial.print(", Humidity: ");
    Serial.println(ev.humid);
    queues.measurements.push(ev);
}


bool Weather::setup()
{
    if (!bmp.begin_I2C())
    {
        Serial.println("Failed to initialise BMP");
        return false;
    }
    if (!aht.begin())
    {
        Serial.println("Failed to initialise AHT");
        return false;
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_50_HZ);
    last_update = millis();
    return true;
}


void Weather::loop(Queues& queues)
{
    uint64_t now = millis();
    if ((now - last_update) > STATE_LENGTH)
    {
        last_update = now;
        switch (state)
        {
        case weather_st::reading_1:
            take_reading(1);
            state = weather_st::reading_2;
            break;
        case weather_st::reading_2:
            take_reading(2);
            state = weather_st::reading_3;
            break;
        case weather_st::reading_3:
            take_reading(3);
            state = weather_st::reading_4;
            break;
        case weather_st::reading_4:
            take_reading(4);
            state = weather_st::reading_5;
            break;
        case weather_st::reading_5:
            take_reading(5);
            send_it(queues);
            state = weather_st::reading_1;
            break;
        }
    }
}