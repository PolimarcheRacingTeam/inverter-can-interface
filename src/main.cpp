// main.cpp
#include <Arduino.h>
#include <CANSAME5x.h>
#include "AMK.h"


CANSAME5x CAN;
amk_actual_values actual_values;
amk_setpoint_1 setpoints;
uint8_t tx_packet[8];

int i, app, rpm;
int16_t torque;
void set_values(uint16_t control, int16_t torqueLimit, int16_t velocity);

void setup()
{
    init_device();
    pinMode(A0,INPUT);
    i=0;
    CAN.onReceive(receive_message);
}

void loop()
{
    app = analogRead(A0);
    Serial.println((int16_t)rpm);
    //min 115 max 970
    if(app>180){
        rpm=map(app,115,970,0,6000);
        torque = 100;
    }
    else
    {
        torque = 0;
        rpm=0;
    }
    
    switch (i)
    {
        case 0:
        //caso base accensione
        //reset degli errori setpoint = 0
        set_values(0x0800,0,0);
        break;

        case 1:
        //caso system ready
        //DC ON ACK
        set_values(0x0200,0,0);
        break;

        case 2:
        //caso DCON=1
        set_values(0x0200,0,0);
        break;

        case 3:
        //caso DCON e QuitDCON=1
        //driver enable & DC ON
        set_values(0x0600,0,0);
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
        set_values(0x0700,0,0);
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
        break;

        case 4:
        //caso Inverter On
        //controller & driver enable & Dc on (setpoint = 0)
        set_values(0x0700,0,0);  
        break;

        case 5:
        //Caso quit Inverter On
        //controller & driver enable & Dc on (setpoint con rotazione 10rpm coppia 2Nm)
        set_values(0x0700,torque,(int16_t)rpm);  
        break;
    }
    
    build_message(&setpoints, tx_packet);
    if (send_message(tx_packet, AMK_INVERTER_1_SETPOINTS_1) &&
        send_message(tx_packet, AMK_INVERTER_2_SETPOINTS_1))
    {
        //Serial.printf("Done Sending!\n");
        Serial.println(i);
        delay(40);
    }
    else
    {
        //Serial.printf("Error occurred while  sending!\n");
    }
}

void set_values(uint16_t control, int16_t torqueLimit, int16_t velocity){
    setpoints.amk_control = control;
    setpoints.amk_target_velocity = velocity;
    setpoints.amk_torque_limit_positive = torqueLimit;
    setpoints.amk_torque_limit_negative = 0;
}
