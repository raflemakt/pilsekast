#pragma once

#include "configuration.h"

#include "network/PacketHandler.h"
#include "utils/StringFormatters.h"
#include "utils/Logging.h"

#define BROADCAST 0

namespace LocalNetworkInterface
{
typedef uint8_t MacAddress[6];
typedef void(*UserCallbackFunction)();

struct TransmissionStats {
    uint16_t total_pkg_received;
    uint16_t total_pkg_sent;
    uint16_t total_bytes_received;
    uint16_t total_bytes_sent;
    uint16_t total_pkg_send_fails;
};


void initialize();
void register_recv_callback(UserCallbackFunction callback_function);
void register_send_callback(UserCallbackFunction callback_function);
void send_buffer(const MacAddress destination_address);

extern uint8_t transmission_buffer[TRANSMISSION_BUFFER_SIZE];
extern uint8_t transmission_size;
extern MacAddress my_mac_address;
extern TransmissionStats transmission_stats;

template<typename T>
void send(T* package, const MacAddress destination_address) {
    LOGLN("\n## User called the 'send<pkg_type>' function");
    transmission_size = sizeof(T);

    if (transmission_size > TRANSMISSION_BUFFER_SIZE) LOGLN("  data too large for buffer, discarding");

    PacketHandler::update_protocol_descriptors_in_registers();
    memcpy(&transmission_buffer, package, transmission_size);

    LOGLN("  overwriting transmission buffer");
    LOG("    bytes: ");
    LOGLN(transmission_size);

    LOG("    transmission buffer (dec): ");
    LOGLN(Format::array_as_decimal(transmission_buffer, transmission_size));

    LOG("    transmission buffer (hex): ");
    LOGLN(Format::array_as_hex(transmission_buffer, transmission_size));
    send_buffer(destination_address);
}
}