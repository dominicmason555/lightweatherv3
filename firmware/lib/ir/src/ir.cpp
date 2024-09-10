#include "ir.hpp"


uint8_t Ir::get_button()
{
    uint8_t button_num = 0;
    if (receiver.decode(&results))
    {
        if (results.value > 0)
        {
            Serial.print("IR code: ");
            Serial.println(results.value);
            switch (results.value)
            {
                case 27643: // Sony play/pause
                    button_num = 1;
                    break;
                case 3835: // Sony stop
                    button_num = 2;
                    break;
                case 1787: // Sony previous
                    button_num = 3;
                    break;
                case 26363: // Sony hold previous
                    button_num = 19;
                    break;
                case 18171: // Sony next
                    button_num = 4;
                    break;
                case 5883: // Sony hold next
                    button_num = 20;
                    break;
                case 6907: // Sony repeat
                    button_num = 5;
                    break;
                case 251: // Sony 1
                    button_num = 6;
                    break;
                case 16635: // Sony 2
                    button_num = 7;
                    break;
                case 8443: // Sony 3
                    button_num = 8;
                    break;
                case 9467: // Sony plus
                    button_num = 9;
                    break;
                case 24827: // Sony 4
                    button_num = 10;
                    break;
                case 4347: // Sony 5
                    button_num = 11;
                    break;
                case 20731: // Sony 6
                    button_num = 12;
                    break;
                case 25851: // Sony minus
                    button_num = 13;
                    break;
                case 12539: // Sony 7
                    button_num = 14;
                    break;
                case 28923: // Sony 8
                    button_num = 15;
                    break;
                case 2299: // Sony 9
                    button_num = 16;
                    break;
                case 18683: // Sony 10
                    button_num = 17;
                    break;
                case 29435: // Sony +10
                    button_num = 18;
                    break;
            }
        }
    }
    return button_num;
}


void handle_button(Queues& queues, uint8_t button_num)
{
    switch (button_num)
    {
    case 17:
        queues.outputs.push({output_t::USB, out_command_t::TOGGLE});
        break;
    case 18:
        queues.outputs.push({output_t::TERMINAL, out_command_t::TOGGLE});
        break;
    }
}


void Ir::setup()
{
    receiver.enableIRIn();
}


void Ir::loop(Queues& queues)
{
    uint8_t button_num = get_button();
    if (button_num != 0)
    {
        if (((millis() - last_button_time) > IR_DEBOUNCE) || 
            (button_num != last_button))
        {
            queues.ir_buttons.push(button_num);
            handle_button(queues, button_num);
            last_button = button_num;
            last_button_time = millis();
        }
    }
}
