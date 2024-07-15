// AMK.h
#ifndef AMK_H
#define AMK_H

#include <Arduino.h>

/**
 * @brief Indirizzi dei nodi degli inverter.
 */
#define AMK_INVERTER_1_NODE_ADDRESS 1 ///< Indirizzo nodo inverter 1.
#define AMK_INVERTER_2_NODE_ADDRESS 2 ///< Indirizzo nodo inverter 2.
#define AMK_INVERTER_3_NODE_ADDRESS 5 ///< Indirizzo nodo inverter 3.
#define AMK_INVERTER_4_NODE_ADDRESS 6 ///< Indirizzo nodo inverter 4.

/**
 * @brief Indirizzi base per i valori attuali e i setpoint.
 */
#define AMK_ACTUAL_VALUES_1_BASE_ADDRESS 0x282 ///< Indirizzo base per valori attuali tipo 1 (inverter -> CAN controller).
#define AMK_ACTUAL_VALUES_2_BASE_ADDRESS 0x284 ///< Indirizzo base per valori attuali tipo 2 (inverter -> CAN controller).
#define AMK_SETPOINTS_1_BASE_ADDRESS 0x183     ///< Indirizzo base per setpoint (CAN controller -> inverter).

/**
 * @brief Indirizzi calcolati per i pacchetti di ciascun inverter.
 */
/* INVERTER 1 */
#define AMK_INVERTER_1_ACTUAL_VALUES_1 (AMK_ACTUAL_VALUES_1_BASE_ADDRESS + AMK_INVERTER_1_NODE_ADDRESS) ///< Valori attuali tipo 1 per inverter 1.
#define AMK_INVERTER_1_ACTUAL_VALUES_2 (AMK_ACTUAL_VALUES_2_BASE_ADDRESS + AMK_INVERTER_1_NODE_ADDRESS) ///< Valori attuali tipo 2 per inverter 1.
#define AMK_INVERTER_1_SETPOINTS_1 (AMK_SETPOINTS_1_BASE_ADDRESS + AMK_INVERTER_1_NODE_ADDRESS)         ///< Setpoint per inverter 1.

/* INVERTER 2 */
#define AMK_INVERTER_2_ACTUAL_VALUES_1 (AMK_ACTUAL_VALUES_1_BASE_ADDRESS + AMK_INVERTER_2_NODE_ADDRESS) ///< Valori attuali tipo 1 per inverter 2.
#define AMK_INVERTER_2_ACTUAL_VALUES_2 (AMK_ACTUAL_VALUES_2_BASE_ADDRESS + AMK_INVERTER_2_NODE_ADDRESS) ///< Valori attuali tipo 2 per inverter 2.
#define AMK_INVERTER_2_SETPOINTS_1 (AMK_SETPOINTS_1_BASE_ADDRESS + AMK_INVERTER_2_NODE_ADDRESS)         ///< Setpoint per inverter 2.

/* INVERTER 3 */
#define AMK_INVERTER_3_ACTUAL_VALUES_1 (AMK_ACTUAL_VALUES_1_BASE_ADDRESS + AMK_INVERTER_3_NODE_ADDRESS) ///< Valori attuali tipo 1 per inverter 3.
#define AMK_INVERTER_3_ACTUAL_VALUES_2 (AMK_ACTUAL_VALUES_2_BASE_ADDRESS + AMK_INVERTER_3_NODE_ADDRESS) ///< Valori attuali tipo 2 per inverter 3.
#define AMK_INVERTER_3_SETPOINTS_1 (AMK_SETPOINTS_1_BASE_ADDRESS + AMK_INVERTER_3_NODE_ADDRESS)         ///< Setpoint per inverter 3.

/* INVERTER 4 */
#define AMK_INVERTER_4_ACTUAL_VALUES_1 (AMK_ACTUAL_VALUES_1_BASE_ADDRESS + AMK_INVERTER_4_NODE_ADDRESS) ///< Valori attuali tipo 1 per inverter 4.
#define AMK_INVERTER_4_ACTUAL_VALUES_2 (AMK_ACTUAL_VALUES_2_BASE_ADDRESS + AMK_INVERTER_4_NODE_ADDRESS) ///< Valori attuali tipo 2 per inverter 4.
#define AMK_INVERTER_4_SETPOINTS_1 (AMK_SETPOINTS_1_BASE_ADDRESS + AMK_INVERTER_4_NODE_ADDRESS)         ///< Setpoint per inverter 4.

/**
 * @brief Comandi di controllo per l'AMK.
 */
#define SET_NULL 0x0                ///< Comando nullo.
#define AMK_INVERTER_ON (1 << 8)    ///< Attiva il controller.
#define AMK_DC_ON (1 << 9)          ///< Attiva l'alta tensione.
#define AMK_DRIVER_ENABLE (1 << 10) ///< Abilita il driver.
#define AMK_ERROR_SET_ON (1 << 11)  ///< Rimuove l'errore.

/**
 * @brief parametri
 */
#define ID110 107 ///< Converter peak current

/**
 * @brief Struct per i setpoint di AMK.
 */
typedef struct
{
    uint16_t amk_control;              ///< Controllo.
    int16_t amk_target_velocity;       ///< Setpoint di velocità.
    int16_t amk_torque_limit_positive; ///< Limite di coppia positivo.
    int16_t amk_torque_limit_negative; ///< Limite di coppia negativo.
} amk_setpoint_1;

/**
 * @brief Struct per i valori attuali di AMK (tipo 1).
 */
typedef struct
{
    uint16_t amk_status;         ///< Stato.
    int16_t amk_actual_velocity; ///< Velocità attuale.
    int16_t torque_current;      ///< Corrente di coppia.
    int16_t magnetizing_current; ///< Corrente di magnetizzazione.
} amk_actual_values_1;

/**
 * @brief Struct per i valori attuali di AMK (tipo 2).
 */
typedef struct
{
    int16_t amk_temp_motor;    ///< Temperatura motore.
    int16_t amk_temp_inverter; ///< Temperatura inverter.
    uint16_t amk_error_info;   ///< Informazioni errori.
    int16_t amk_temp_igbt;     ///< Temperatura IGBT.
} amk_actual_values_2;

/**
 * @brief Union per contenere i valori attuali di AMK.
 */
typedef union
{
    amk_actual_values_1 actual_values_1; ///< Valori attuali di tipo 1.
    amk_actual_values_2 actual_values_2; ///< Valori attuali di tipo 2.
} amk_actual_values;

// Dichiarazioni delle funzioni

/**
 * @brief Inizializza il device e le sue funzionalità
 */
void init_device();

/**
 * @brief Costruisce un messaggio CAN dai setpoint di AMK.
 *
 * @param setpoint Puntatore alla struttura dei setpoint di AMK.
 * @param buffer Buffer dove il messaggio sarà costruito.
 */
void build_message(amk_setpoint_1 *setpoint, uint8_t *buffer);

/**
 * @brief Invia un messaggio CAN.
 *
 * @param message Puntatore al messaggio da inviare.
 * @param address Indirizzo CAN del destinatario.
 * @return true Se il messaggio è stato inviato correttamente.
 * @return false Se c'è stato un errore nell'invio del messaggio.
 */
bool send_message(uint8_t *message, const int address);

/**
 * @brief Gestisce la ricezione dei messaggi CAN.
 *
 * @param packetSize Dimensione del pacchetto ricevuto.
 */
void receive_message(int packetSize);

/**
 * @brief Stampa l'argomento e la descrizione dell'errore fornito
 * 
 * @param error_code Codice dell'errore ottenuto
 */
void print_error(uint16_t error_code);

/**
 * @brief Processa i valori attuali ricevuti da AMK.
 *
 * @param actual Puntatore alla union contenente i valori attuali.
 * @param is_actual_values_1 Flag che indica se i valori sono di tipo 1.
 */
void process_actual_values(const amk_actual_values *actual, bool is_actual_values_1);

#endif
