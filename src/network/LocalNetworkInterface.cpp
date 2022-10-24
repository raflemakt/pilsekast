#include <esp_now.h>
#include <WiFi.h>

#include "LocalNetworkInterface.h"


namespace esp_now_status {
    esp_err_t init;
    esp_err_t peer_add;
    esp_err_t register_send_callback;
    esp_err_t register_recv_callback;
    esp_now_send_status_t last_send;
}

esp_now_peer_info_t peer = {};
esp_now_peer_num_t peer_num;

void(* user_configured_recv_cb)();
void(* user_configured_send_cb)();

uint8_t LocalNetworkInterface::transmission_buffer[TRANSMISSION_BUFFER_SIZE] = {0};
uint8_t LocalNetworkInterface::transmission_size;

void move_data_to_buffer(const uint8_t *data) {
    Serial.print("  attempting to move data to buffer: ");
    
    for (auto i = 0; i < sizeof(data); i++){
        Serial.print(data[i]);
        if (i%8==0) Serial.print(" ");
    }
    Serial.println();
    
    if (sizeof(data) > TRANSMISSION_BUFFER_SIZE){
        Serial.println("  discard data: too large for buffer");
        return;
    }

    memset(&LocalNetworkInterface::transmission_buffer, 0, TRANSMISSION_BUFFER_SIZE);  // <-- strengt tatt unødvendig
    memcpy(&LocalNetworkInterface::transmission_buffer, &data, sizeof(data));
}

void esp_now_on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println("  esp_now cb: data sent");
  esp_now_status::last_send = status;
  user_configured_send_cb();
}


void esp_now_on_data_receive(const uint8_t *mac_addr, const uint8_t *incoming_data, int bytes_received) {
    Serial.println("  esp_now cb: data recv");
    Serial.print("  bytes received: ");
    Serial.println(bytes_received);

    LocalNetworkInterface::transmission_size = bytes_received;
    
    if (!esp_now_is_peer_exist(mac_addr)) {
        memset(&peer, 0, sizeof(peer));
        peer.channel = LOCAL_NETWORK_CHANNEL;
        peer.encrypt = false;
        memcpy(&peer.peer_addr, mac_addr, ESP_NOW_ETH_ALEN);

        Serial.print("  attempting to add peer: ");
        // TODO: Mac-address formatter in printing/formatters.h
        for (int i = 0; i<ESP_NOW_ETH_ALEN; i++){
            Serial.print(mac_addr[i], HEX);
            if (i != 5) Serial.print(":");
        }
        esp_now_status::peer_add = esp_now_add_peer(&peer);

        Serial.println();
        switch (esp_now_status::peer_add) {
            case (ESP_OK):
                Serial.println("  peer successfully added");
                break;
            case (ESP_ERR_ESPNOW_NOT_INIT):
                Serial.println("  failed, esp now not init");
                break;
            case (ESP_ERR_ESPNOW_FULL):
                Serial.println("  failed, peer list full");
                break;
            case (ESP_ERR_ESPNOW_EXIST):
                Serial.println("  failed, peer already exists");
                break;
        }
        esp_now_get_peer_num(&peer_num);

        Serial.print("  number of peers: ");
        Serial.println(peer_num.total_num);
        Serial.println();
    }
    
    move_data_to_buffer(incoming_data);
    user_configured_recv_cb();
}


namespace LocalNetworkInterface
{
void initialize(){
    Serial.println("WiFi mode WIFI_STA");
    Serial.print("  my MAC: ");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_now_status::init = esp_now_init();

    Serial.println("Esp Now initializing");
    Serial.print("  init result: ");
    Serial.println(esp_now_status::init, BIN);

    if (!IS_ACCESS_POINT) {
        peer.channel = 1;
        peer.encrypt = false;
        memcpy(&peer.peer_addr, AP_MAC_ADDRESS, ESP_NOW_ETH_ALEN);
        esp_now_status::peer_add = esp_now_add_peer(&peer);

        Serial.println("  manually added access point as peer.");
        Serial.print("  AP: ");

        // TODO: Mac-address formatter in printing/formatters.h
        for (int i = 0; i<ESP_NOW_ETH_ALEN; i++){
            Serial.print(AP_MAC_ADDRESS[i], HEX);
        if (i != 5) Serial.print(":");
        }
        Serial.println();
    }
    esp_now_get_peer_num(&peer_num);

    Serial.print("  number of peers: ");
    Serial.println(peer_num.total_num);

    esp_now_status::register_recv_callback = esp_now_register_recv_cb(esp_now_on_data_receive);
    esp_now_status::register_send_callback = esp_now_register_send_cb(esp_now_on_data_sent);
    
    Serial.println("  esp_now callback functions registered");
    Serial.print("    send cb status: ");
    Serial.print(esp_now_status::register_send_callback);
    Serial.print("    recv cb status: ");
    Serial.print(esp_now_status::register_recv_callback);
    Serial.println();
}

void register_recv_callback(void( *callback_function)()) {
    Serial.println("  user recv cb registered");
    user_configured_recv_cb = callback_function;
}

void register_send_callback(void( *callback_function)()) {
    Serial.println("  user send cb registered");
    user_configured_send_cb = callback_function;
}

void send_binary_package(const uint8_t *peer_addr, const uint8_t *data, size_t len) {
    esp_err_t result = esp_now_send(peer_addr, data, len);
    Serial.print("User called 'send_binary_package'. Result: ");
    Serial.println(result);
    move_data_to_buffer(data); // <--- hmm, vil vi dette?
}
}
