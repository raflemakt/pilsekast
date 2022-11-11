# Pilsekast
System for fjernstyring av lys og aktuatorer, samt innsamling av sensordata. Aksesspunktet er tilkoblet en sensor eller en elektrisk trommepad og vil kringkaste trommeslagene til alle nodene som er koblet til. Nodene kan sende telemetrimeldinger tilbake til aksesspunktet.
 * Stjernetopologi: Ett aksesspunkt, opp til 20 noder.
 * Skalerbart sambandssystem: sømløs og rask tilkopling av noder til aksesspunkt vha. ESP NOW.
 * Mulighet for kryptering av all transmisjon vha. ESP NOW.
 * Rask og effektiv transmisjon over TELEPILS/ØLKAST, vår egendefinerte binære applikasjonslagsprotokoll.

Systemet er utviklet for ESP32, men vil fungere på andre mikrokontrollere så lenge de støttes av byggesystemet (platform.io).

Prosjektarbeid i IELET2001: Datakommunikasjon.

## Maskinvare
Aksesspunkt: ESP32 med tilleggsutstyr:
 * Elektrisk trommepad eller kontaktmikrofon med minijackkobling
 * Fem variable motstander for justering av parametere
 * TTGO-skjerm (valgfritt)
 * RGB-LED, indikatorlampe (valgfritt)

 Node: ESP32 med tilleggsutstyr:
  * LED-stripe
  * ICM-bevegelsessensor (valgfritt)
  * MAX9814 Lydsensor (valgfritt)
  * TTGO-skjerm (valgfritt)

## Byggeinstruks
Konfigurer `src/configuration.h` med riktig informasjon. Et eksempel på konfigurasjon kan være:
```cpp
#define IS_ACCESS_POINT
#define HAS_SOUND_SENSOR
#define HAS_ICM_SENSOR
#define HAS_TTGO_SCREEN
#define LED_STRIP_LED_AMOUNT 20

const bool DEBUG_PRINT_TO_SERIAL = true;
const int SERIAL_BAUD_RATE = 115200;
const uint8_t AP_MAC_ADDRESS[] = {0x84, 0xCC, 0xA8, 0x61, 0x56, 0x88};
const uint8_t TRANSMISSION_BUFFER_SIZE = 128;
const uint8_t LOCAL_NETWORK_CHANNEL = 0;
const String NODE_NAME = "Bjarne Bass";
const String INSTRUMENT_TYPE = "basstromme";
```
Det kan kun eksistere ett aksesspunkt i et pilsekast-nettverk. MAC-addressen til denne ESP32'en skal hardkodes inn på alle nodene.
```cpp
const uint8_t AP_MAC_ADDRESS[] = {0x84, 0xCC, 0xA8, 0x61, 0x56, 0x88};
```
Hvilken prosedyre som kompileres og lastes opp, enten "node.cpp" eller "AP.cpp", velges med
```cpp
#define IS_ACCESS_POINT
```
Denne linjen fjernes eller kommenteres ut for å laste opp prosedyren til "node.cpp".
```cpp
//#define IS_ACCESS_POINT
```

Vi bruker platform.io som byggesystem. Platform.io kan installeres globalt på systemet, eller som *vscode-extension*. For å laste opp, trykk på *upload* i vscode eller bruk terminal:
```
pio run -t upload
```
Konfigurasjon av mikrokontrollere og tredjepartsbiblioteker gjøres i `platform.ini`
