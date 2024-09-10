#include "io.hpp"


void Io::handle_terminal(out_command_t command)
{
    switch (command)
    {
    case out_command_t::ON:
        digitalWrite(TERMINAL_PIN, HIGH);
        Serial.println("Switching terminal on");
        terminal_state = true;
        break;
    case out_command_t::OFF:
        digitalWrite(TERMINAL_PIN, LOW);
        Serial.println("Switching terminal off");
        terminal_state = false;
        break;
    case out_command_t::TOGGLE:
        terminal_state = !terminal_state;
        digitalWrite(TERMINAL_PIN, terminal_state);
        Serial.println("Toggling terminal");
        break;
    }
}


void Io::handle_usb(out_command_t command)
{
    switch (command)
    {
    case out_command_t::ON:
        digitalWrite(USB_PIN, HIGH);
        Serial.println("Switching USB on");
        usb_state = true;
        break;
    case out_command_t::OFF:
        digitalWrite(USB_PIN, LOW);
        Serial.println("Switching USB off");
        usb_state = false;
        break;
    case out_command_t::TOGGLE:
        usb_state = !usb_state;
        digitalWrite(USB_PIN, usb_state);
        Serial.println("Toggling USB");
        break;
    }
}


void Io::setup()
{
    pinMode(13, OUTPUT);
    pinMode(4, OUTPUT);
}


void Io::loop(Queues& queues)
{
    while (queues.outputs.available())
    {
        output_ev ev = queues.outputs.pop();
        switch (ev.output)
        {
        case output_t::TERMINAL:
            handle_terminal(ev.command);
            break;
        case output_t::USB:
            handle_usb(ev.command);
            break;
        }
    }
}
