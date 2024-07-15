// main.cpp
#include <Arduino.h>
#include <CANSAME5x.h>
#include "AMK.h"

CANSAME5x CAN;
amk_actual_values actual_values;
amk_setpoint_1 setpoints;
uint8_t tx_packet[8];

void set_values();

void setup()
{
    init_device();

    CAN.onReceive(receive_message);
}

void loop()
{
    set_values();

    build_message(&setpoints, tx_packet);
    if (send_message(tx_packet, AMK_INVERTER_1_SETPOINTS_1) &&
        send_message(tx_packet, AMK_INVERTER_2_SETPOINTS_1))
    {
        Serial.printf("Done Sending!\n");
        delay(40);
    }
    else
    {
        Serial.printf("Error occurred while  sending!\n");
    }
}

void set_values(){
    setpoints.amk_control = AMK_DC_ON | AMK_DRIVER_ENABLE | AMK_INVERTER_ON | AMK_ERROR_SET_ON;
    setpoints.amk_target_velocity = 45;
    setpoints.amk_torque_limit_positive = 3;
    setpoints.amk_torque_limit_negative = -3;
}
