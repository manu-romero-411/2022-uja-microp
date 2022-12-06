/************************
 * Created: 21-May-2019 *
 * Author:  J.F.C.D.    *
 ************************/
 
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/*******************************************/
/*** PARÁMETROS ELEGIBLES POR EL USUARIO ***/
/*******************************************/
// Tiempo en segundos de espera antes de la activación del sistema de vigilancia (tiempo de escape del usuario).
#define ACTIVATION_DELAY 30
// Tiempo en segundos antes de activación de la alerta para que el usuario desactive el sistema si acaba de llegar.
#define ALERT_DELAY 15
// Tiempo en segundos antes de envío del SMS de aviso después de la activación de la alerta (opcional).
#define SMS_DELAY 1
// Tiempo de duración en segundos de la alerta una vez activada (30 min. x 60 seg. = 1800 seg.).
#define ALERT_DURATION 1800 

/******************************/
/*** PARÁMETROS DEL SISTEMA ***/
/******************************/
#define BUZZER 5
#define ALERT_SIGNAL 7
#define SERIAL_BAUDRATE 9600
#define GSM_BAUDRATE 9600
#define RX_pin 9
#define TX_pin 11

/****************************************/
/*** CONSTANTES SIMPLIFICACIÓN CÓDIGO ***/
/****************************************/
#define ON 1
#define OFF 0

#endif
