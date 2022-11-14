#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include "network/LocalNetworkInterface.h"
#include "network/PacketHandler.h"
#include "utils/StringFormatters.h"


namespace esp_now_status {
    esp_err_t init;
    esp_err_t peer_add;
    esp_err_t register_send_callback;
    esp_err_t register_recv_callback;
    esp_now_send_status_t last_send;
}

esp_now_peer_info_t peer_buffer = {};
esp_now_peer_num_t peer_num;


namespace LocalNetworkInterface
{
MacAddress my_mac_address = {0};
UserCallbackFunction user_configured_recv_cb = nullptr;
UserCallbackFunction user_configured_send_cb = nullptr;

uint8_t transmission_buffer[TRANSMISSION_BUFFER_SIZE] = {0};
uint8_t transmission_size;

TransmissionStats transmission_stats = {0};


void add_address_to_peers(const uint8_t *mac_addr) {
    if (!esp_now_is_peer_exist(mac_addr)) {
        memset(&peer_buffer, 0, sizeof(peer_buffer));
        peer_buffer.channel = LOCAL_NETWORK_CHANNEL;
        peer_buffer.encrypt = false;
        memcpy(&peer_buffer.peer_addr, mac_addr, ESP_NOW_ETH_ALEN);

        esp_now_status::peer_add = esp_now_add_peer(&peer_buffer);

        Serial.print("  attempting to add peer: ");
        Serial.println(Format::mac_addr_from_array(mac_addr));;
        Serial.print("    result: ");
        Serial.println(esp_err_to_name(esp_now_status::peer_add));

        esp_now_get_peer_num(&peer_num);

        Serial.print("  number of peers: ");
        Serial.println(peer_num.total_num);
    } else {
        Serial.println("  address exists in peer list");
    }
}

void esp_now_on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    esp_now_status::last_send = status;
    Serial.print("  esp_now_send_status: ");
    Serial.print(esp_now_status::last_send);

    if (status != 0) {
        Serial.println(" (fail)\n");
        transmission_stats.total_pkg_send_fails ++;
        return;
    } else {
        Serial.println(" (success)");

        transmission_stats.total_pkg_sent ++;
        transmission_stats.total_bytes_sent += transmission_size;

        Serial.print("    total_pkg_sent: ");
        Serial.println(transmission_stats.total_pkg_sent);
        Serial.print("    total_bytes_sent: ");
        Serial.println(transmission_stats.total_bytes_sent);
    }
    user_configured_send_cb();
}

void esp_now_on_data_receive(const uint8_t *mac_addr, const uint8_t *incoming_data, int bytes_received) {
    Serial.print("\n## Received data from  ");
    Serial.println(Format::mac_addr_from_array(mac_addr));
    Serial.print("  bytes received: ");
    Serial.println(bytes_received);

    transmission_stats.total_pkg_received ++;
    transmission_stats.total_bytes_received += bytes_received;
    
    Serial.print("    total_pkg_received: ");
    Serial.println(transmission_stats.total_pkg_received);
    Serial.print("    total_bytes_received: ");
    Serial.println(transmission_stats.total_bytes_received);
    
    add_address_to_peers(mac_addr);
    PacketHandler::move_data_to_buffer(incoming_data, bytes_received);
    PacketHandler::move_data_to_register();
    user_configured_recv_cb();
}

void initialize(){
    WiFi.macAddress(my_mac_address);  // Vi setter ikke MAC her, men leser den over i 'my_mac_address'
    
    Serial.println("WiFi mode WIFI_STA");
    Serial.print("  my MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.print("  AP MAC: ");
    Serial.println(Format::mac_addr_from_array(AP_MAC_ADDRESS));

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_now_status::init = esp_now_init();

    Serial.println("Esp Now initializing");
    Serial.print("  init result: ");
    Serial.println(esp_err_to_name(esp_now_status::init));

    #ifndef IS_ACCESS_POINT
    add_address_to_peers(AP_MAC_ADDRESS);
    #endif

    esp_now_status::register_recv_callback = esp_now_register_recv_cb(esp_now_on_data_receive);
    esp_now_status::register_send_callback = esp_now_register_send_cb(esp_now_on_data_sent);
    
    Serial.println("  registering internal esp_now callbacks ");
    Serial.print("    send cb status: ");
    Serial.println(esp_err_to_name(esp_now_status::register_send_callback));
    Serial.print("    recv cb status: ");
    Serial.println(esp_err_to_name(esp_now_status::register_recv_callback));
}

void register_recv_callback(UserCallbackFunction callback_function) {
    Serial.println("  user recv cb registered");
    user_configured_recv_cb = callback_function;
}

void register_send_callback(UserCallbackFunction callback_function) {
    Serial.println("  user send cb registered");
    user_configured_send_cb = callback_function;
}

void send_buffer(const MacAddress destination_address) {
    Serial.print("  destination: ");
    Serial.println(Format::mac_addr_from_array(destination_address));

    esp_err_t result = esp_now_send(destination_address, transmission_buffer, transmission_size);

    Serial.print("  esp_now_send result: ");
    Serial.println(esp_err_to_name(result));
}

}
