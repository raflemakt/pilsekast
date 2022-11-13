#pragma once

#include "../configuration.h"

#include "network/LocalNetworkInterface.h"
#include "network/PacketHandler.h"
#include "utils/StringFormatters.h"

#define BROADCAST 0

namespace LocalNetworkInterface
{
typedef uint8_t MacAddress[6];
typedef void(*UserCallbackFunction)();

void initialize();
void register_recv_callback(UserCallbackFunction callback_function);
void register_send_callback(UserCallbackFunction callback_function);
void send_binary_package(const uint8_t *peer_addr, const uint8_t *data, size_t len);
void send_buffer(const MacAddress destination_address);

extern uint8_t transmission_buffer[TRANSMISSION_BUFFER_SIZE];
extern uint8_t transmission_size;
extern MacAddress my_mac_address;

template<typename T>
void send(T* package, const MacAddress destination_address) {
    Serial.println("\n## User called the 'send<pkg_type>' function");
    transmission_size = sizeof(T);

    if (transmission_size > TRANSMISSION_BUFFER_SIZE) Serial.println("  data too large for buffer, discarding");

    PacketHandler::update_protocol_descriptors_in_registers();
    memcpy(&transmission_buffer, package, transmission_size);

    Serial.println("  overwriting transmission buffer");
    Serial.print("    bytes: ");
    Serial.println(transmission_size);

    Serial.print("    transmission buffer (dec): ");
    Serial.println(Format::array_as_decimal(transmission_buffer, transmission_size));

    Serial.print("    transmission buffer (hex): ");
    Serial.println(Format::array_as_hex(transmission_buffer, transmission_size));
    send_buffer(destination_address);
}
}