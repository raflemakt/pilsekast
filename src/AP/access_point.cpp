#include <Arduino.h>
#include "AP/access_point.h"
#include "../configuration.h"
#include "network/LocalNetworkInterface.h"


namespace AccessPoint
{
uint32_t program_counter = 0;

void print_last_package(){
    // TODO: move to printing/formatters.h
    using namespace LocalNetworkInterface;
    for (auto i = 0; i < transmission_size; i++){
        auto pos = sizeof(transmission_buffer) - transmission_size + i;
        Serial.print(transmission_buffer[pos]);
        Serial.print(" ");
    }
    Serial.println();
    Serial.print("  transmission_size: ");
    Serial.println(transmission_size);
}

void print_transmission_buffer(){
    // TODO: move to printing/formatters.h
    using namespace LocalNetworkInterface;
    for (auto i = 0; i < sizeof(transmission_buffer); i++){
        Serial.print((uint8_t)transmission_buffer[i]);
        Serial.print(" ");
    }
    Serial.println();
}

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
    Serial.print("Received data: ");
    print_transmission_buffer();
    Serial.println("\n");
}

void on_local_data_send() {
    Serial.print("Sent data: ");
    print_transmission_buffer();
    Serial.println("\n");
}


void setup(){
    pinMode(0, INPUT_PULLUP);

    test_print();
    LocalNetworkInterface::initialize();
    LocalNetworkInterface::register_recv_callback(on_local_data_receive);
    LocalNetworkInterface::register_send_callback(on_local_data_send);
    Serial.println("Init complete from access_point.cpp");
    
}


void main(){
    if(!digitalRead(0)) {
        uint8_t test_data = program_counter / 4;
        LocalNetworkInterface::send_binary_package(0, &test_data, 1);
        delay(150); // hehehe
    }
    program_counter++;
}
}