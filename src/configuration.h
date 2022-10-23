#include "Arduino.h"

// Denne innstillingen velger om det er AP eller node som blir programmert.
//   Dersom IS_ACCESS_POINT er definert ---> programmerer aksesspunkt (AP/basstromme)
//   Dersom IS_ACCESS_POINT ikke er definert ---> programmerer node (instrument)
#define IS_ACCESS_POINT                 // Kommenter ut/inn for å endre

//   Konfigurasjon av aksesspunktsadresse.
//   Endre denne dersom en ny ESP32 med ny MAC-adresse brukes for
//   aksesspunktet.
const uint8_t AP_MAC_ADDRESS[] = {0x84, 0xCC, 0xA8, 0x61, 0x56, 0x88}; // Dette er TTGOESP32 med skeiv resetknapp


// Konfigurasjon av nodenavn og instrumenttype
const String NODE_NAME = "Tore Trombone";
const String INSTRUMENT_TYPE = "trekkbasun";
