#pragma once

#include "../configuration.h"



namespace LocalNetworkInterface
{
void initialize();

// FIXME: Om en eller begge callback-funksjonene ikke er registrert vil de peke
//        på og til slutt eksekvere tilfeldige minneadresser, noe som fører til at
//        ESP'en kræsjer. Finn på et bedre system for dette.
void register_recv_callback(void(* callback_function)());
void register_send_callback(void(* callback_function)());

void send_binary_package(const uint8_t *peer_addr, const uint8_t *data, size_t len);

extern uint8_t transmission_buffer[TRANSMISSION_BUFFER_SIZE];
extern uint8_t transmission_size;
}