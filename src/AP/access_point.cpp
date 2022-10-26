#include <Arduino.h>
#include "AP/access_point.h"
#include "../configuration.h"
#include "network/LocalNetworkInterface.h"
#include "UserInterface/StringFormatters.h"


namespace AccessPoint
{
uint32_t program_counter = 0;


void test_print(){
    // TODO: lag ordentlig oppstartsmelding og flytt til printing
    Serial.println("\nPILSEKAST ACCESS POINT BOOT");
    Serial.print("  node name: ");
    Serial.println(NODE_NAME);
    Serial.print("  intrument type: ");
    Serial.println(INSTRUMENT_TYPE);
    Serial.println();
}

void on_local_data_receive() {
}

void on_local_data_send() {
}


void setup(){
    pinMode(0, INPUT_PULLUP);

    test_print();
    LocalNetworkInterface::initialize();
    LocalNetworkInterface::register_recv_callback(on_local_data_receive);
    LocalNetworkInterface::register_send_callback(on_local_data_send);
    Serial.println("Init complete from access_point.cpp --> starting loop.\n");
    
}

uint8_t test_data_num = 75;
void main(){
    if(!digitalRead(0)) {
        LocalNetworkInterface::send_binary_package(0, &test_data_num, sizeof(test_data_num));
        delay(150); // hehehe
    }
    program_counter++;
}
}