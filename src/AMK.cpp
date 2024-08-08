// AMK.cpp
#include "AMK.h"
#include "error_map.h"
#include <CAN.h>
#include <CANSAME5x.h>

extern CANSAME5x CAN;
extern amk_actual_values actual_values;
extern int i;
void init_device()
{
    Serial.begin(115200);
    
    pinMode(PIN_CAN_STANDBY, OUTPUT);
    digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
    pinMode(PIN_CAN_BOOSTEN, OUTPUT);
    digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

    // avvia il can bus a 500kbps
    if (!CAN.begin(500E3))
    {
        Serial.printf("Starting CAN failed!\n");
        // se la comunicazione non avviene correttamente il led lampeggia
        while (1)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
}

void build_message(amk_setpoint_1 *setpoint, uint8_t *buffer)
{
    buffer[0] = setpoint->amk_control & 0xFF;
    buffer[1] = (setpoint->amk_control >> 8) & 0xFF;
    buffer[2] = setpoint->amk_target_velocity & 0xFF;
    buffer[3] = (setpoint->amk_target_velocity >> 8) & 0xFF;
    buffer[4] = setpoint->amk_torque_limit_positive & 0xFF;
    buffer[5] = (setpoint->amk_torque_limit_positive >> 8) & 0xFF;
    buffer[6] = setpoint->amk_torque_limit_negative & 0xFF;
    buffer[7] = (setpoint->amk_torque_limit_negative >> 8) & 0xFF;
}

bool send_message(uint8_t *message, const int address)
{
    //Serial.printf("Sending packet to ...0x%X\n", address);
    CAN.beginPacket(address);
    size_t bytesSent = CAN.write(message, 8);
    CAN.endPacket();
    return (bytesSent == 8);
}

void receive_message(int packetSize)
{
    long packet_ID = CAN.packetId();
    //Serial.printf("Received CAN packet from...0x%X\n", packet_ID);

    bool is_actual_values_1 = (packet_ID == AMK_INVERTER_1_ACTUAL_VALUES_1 || packet_ID == AMK_INVERTER_2_ACTUAL_VALUES_1);

    if (CAN.available() >= packetSize)
    {
        if (is_actual_values_1)
        {
            actual_values.actual_values_1.amk_status = CAN.read() | (CAN.read() << 8);
            actual_values.actual_values_1.amk_actual_velocity = CAN.read() | (CAN.read() << 8);
            actual_values.actual_values_1.torque_current = CAN.read() | (CAN.read() << 8);
            actual_values.actual_values_1.magnetizing_current = CAN.read() | (CAN.read() << 8);
        }
        else
        {
            actual_values.actual_values_2.amk_temp_motor = CAN.read() | (CAN.read() << 8);
            actual_values.actual_values_2.amk_temp_inverter = CAN.read() | (CAN.read() << 8);
            actual_values.actual_values_2.amk_error_info = CAN.read() | (CAN.read() << 8);
            actual_values.actual_values_2.amk_temp_igbt = CAN.read() | (CAN.read() << 8);
        }
    }
    else
    {
        Serial.println("Invalid packet size");
        return;
    }

    process_actual_values(&actual_values, is_actual_values_1);
}

void process_actual_values(const amk_actual_values *actual, bool is_actual_values_1)
{
    if (is_actual_values_1)
    {
        if((actual->actual_values_1.amk_status&0x7FFF) == 0x0100) {
            i = 1; // System ready senza errori
        } else if((actual->actual_values_1.amk_status&0x7FFF) == 0x1100) {
            i = 2; // DCON = 1
        } else if((actual->actual_values_1.amk_status&0x7FFF) == 0x1900) {
            i = 3; // QuitDCON & DCON = 1
        } else if((actual->actual_values_1.amk_status&0x7FFF) == 0x5900) {
            i = 4; // inverter On
        }else if((actual->actual_values_1.amk_status&0x7FFF) == 0x7900) {
            i = 5; // quit inverter On
        } else {
            i = 0; // Default case
        }
        Serial.printf("AMK_Status: %X\n", actual->actual_values_1.amk_status);
        Serial.printf("AMK_ActualVelocity: %d\n", actual->actual_values_1.amk_actual_velocity);
        //Serial.printf("AMK_TorqueCurrent: %f\n", (float)(ID110 * actual->actual_values_1.torque_current) / 16384);
        //Serial.printf("AMK_MagnetCurrent: %f\n", (float)(ID110 * actual->actual_values_1.magnetizing_current) / 16384);
    }
    else
    {
        Serial.printf("AMK_TempMotor: %f\n", (float)actual->actual_values_2.amk_temp_motor * 0.1);
        Serial.printf("AMK_TempInverter: %f\n", (float)actual->actual_values_2.amk_temp_inverter * 0.1);
        //Serial.printf("AMK_ErrorInfo: %d\n", actual->actual_values_2.amk_error_info);
        //Serial.printf("AMK_TempIGBT: %f\n", (float)actual->actual_values_2.amk_temp_igbt * 0.1);
        
        if(actual->actual_values_2.amk_error_info != 0){
            print_error(actual->actual_values_2.amk_error_info);
        }
    }
}

void print_error(uint16_t error_code)
{
    auto error = ERROR_MAP.find(error_code);
    if (error != ERROR_MAP.end()) {
        //std::printf("Error with code %d: %s - %s\n", error_code, error->second.first, error->second.second);
    } else {
        //std::printf("Error with code %d is unknown\n", error_code);
    }
}
