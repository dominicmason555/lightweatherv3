#pragma once

#include <cmath>
#include <iostream>


template<typename T, size_t length>
class evq
{
private:
    T q[(length + 1)];
    size_t head;
    size_t tail;

public:
    evq(): head(0), tail(0) {};
    
    void push(T ev)
    {
        q[tail] = ev;
        tail = (tail + 1) % (length + 1);
        if (head == tail) // Sacrifice the oldest
        {
            head = (head + 1) % (length + 1);
        }
    }

    bool available()
    {
        return (head % (length + 1)) != tail;
    }

    T pop()
    {
        size_t oldhead = head;
        head = (head + 1) % (length + 1);
        return q[oldhead];
    }
};


enum class output_t
{
    USB = 0,
    TERMINAL = 1
};


enum class out_command_t
{
    ON = 0,
    OFF = 1,
    TOGGLE = 2
};


struct output_ev
{
    output_t output;
    out_command_t command;
};


struct measurement_ev
{
    double btemp;
    double press;
    double humid;
    double atemp;
};


struct Queues
{
    evq<output_ev, 4> outputs;
    evq<measurement_ev, 1> measurements;
    evq<uint8_t, 1> ir_buttons;
};


