#include <Arduino.h>
#include "AP/access_point.h"
#include "../configuration.h"
#include "network/LocalNetworkInterface.h"
#include "UserInterface/StringFormatters.h"
#include "UserInterface/Messages.h"


namespace AccessPoint
{
uint32_t program_counter = 0;


void on_local_data_receive() {
    Serial.println("  on_local_data_receive called");
}

void on_local_data_send() {
    Serial.println("  on_local_data_send called");
}


void setup(){
    pinMode(0, INPUT_PULLUP);

    Messages::on_boot();
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