#pragma once
#include "Arduino.h"

// Denne innstillingen velger om det er AP eller node som blir programmert.
//   Dersom IS_ACCESS_POINT er true  ---> programmerer aksesspunkt (AP/basstromme)
//   Dersom IS_ACCESS_POINT er false ---> programmerer node (instrument)
const bool IS_ACCESS_POINT = true;

// Aktiver/deaktiver debug-printing til seriemonitor
const bool DEBUG_PRINT_TO_SERIAL = true;

// Baudrate for seriemonitor
const int SERIAL_BAUD_RATE = 115200;

// Konfigurasjon av aksesspunktsadresse.
//   Endre denne dersom en ny ESP32 med ny MAC-adresse brukes for aksesspunktet.
const uint8_t AP_MAC_ADDRESS[] = {0x84, 0xCC, 0xA8, 0x61, 0x56, 0x88}; // Dette er TTGOESP32 med skeiv resetknapp

// Transmisjonsbufferstørrelse i byte, må være stor nok til å romme største pakketype
const uint8_t TRANSMISSION_BUFFER_SIZE = 64;
const uint8_t LOCAL_NETWORK_CHANNEL = 0;

// Konfigurasjon av nodenavn og instrumenttype
const String NODE_NAME = "Tore Trombone";
const String INSTRUMENT_TYPE = "trekkbasun";

