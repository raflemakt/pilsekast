#pragma once
#include "Arduino.h"

// Denne innstillingen velger om det er AP eller node som blir programmert.
//   Dersom IS_ACCESS_POINT er definert  ---> programmerer aksesspunkt (AP/basstromme)
//   Dersom IS_ACCESS_POINT ikke er definert ---> programmerer node (instrument)
#define IS_ACCESS_POINT

// Ekstrautstyr, kommenter ut dersom de ikke er tilkobla
//#define HAS_SOUND_SENSOR
//#define HAS_ICM_SENSOR
#define HAS_TTGO_SCREEN

// Antall lys i LED-stripe. 0 --> ingen LED-stripe
#define LED_STRIP_LED_AMOUNT 0

// Aktiver/deaktiver debug-printing til seriemonitor
#define DEBUG_PRINT_TO_SERIAL

// Baudrate for seriemonitor
const int SERIAL_BAUD_RATE = 115200;

// Konfigurasjon av aksesspunktsadresse.
//   Endre denne dersom en ny ESP32 med ny MAC-adresse brukes for aksesspunktet.
const uint8_t AP_MAC_ADDRESS[] = {0x84, 0xCC, 0xA8, 0x61, 0x56, 0x88}; // Dette er TTGOESP32 med skeiv resetknapp

// Transmisjonsbufferstørrelse i byte, må være stor nok til å romme største pakketype
const uint8_t TRANSMISSION_BUFFER_SIZE = 128;
const uint8_t LOCAL_NETWORK_CHANNEL = 0;

// Konfigurasjon av nodenavn og instrumenttype
const String NODE_NAME = "Bjarne Bass";
const String INSTRUMENT_TYPE = "basstromme";
