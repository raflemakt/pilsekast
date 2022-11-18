#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"
#include "AP/HardwareInstance.h"
#include "utils/Logging.h"

uint8_t current_reported_noise_record = 0;

namespace AccessPointTriggers
{
void decide_action_on_pkg_receive() {
    LOGLN("Executing action from packet");
    ProtocolDescriptor header = (ProtocolDescriptor)LocalNetworkInterface::transmission_buffer[0];

    switch (header) {
        case OELKAST_LIGHT_SIMPLE: {
            LOGLN("  detected OELKAST_LIGHT_SIMPLE packet");
            LOGLN("  no action taken");
        } break;

        case OELKAST_LIGHT_SIMPLE_HUE: {
            LOGLN("  detected OELKAST_LIGHT_SIMPLE_HUE packet");
            LOGLN("  no action taken");
        } break;

        case OELKAST_LIGHT_ENVELOPED: {
            LOGLN("  detected OELKAST_LIGHT_ENVELOPED packet");
            LOGLN("  no action taken");
        } break;

        case OELKAST_LIGHT_ANIMATION_MODE: {
            LOGLN("  detected OELKAST_LIGHT_ANIMATION_MODE packet");
            LOGLN("  no action taken");
        } break;

        case TELEPILS_ANNOUNCE: {
            LOGLN("  detected TELEPILS_ANNOUNCE packet");
            LOG("    node_name: ");
            LOGLN(telepils_announce.node_name);
            LOG("    instrument_type: ");
            LOGLN(telepils_announce.instrument_type);
            LOGLN("    peripherals on node:");
            if (telepils_announce.has_icm_sensor)     LOGLN("    ->ICM motion sensor");
            if (telepils_announce.has_sound_sensor)   LOGLN("    ->sound sensor");
            if (telepils_announce.has_ttgo_screen)    LOGLN("    ->TTGO screen");
            if (telepils_announce.led_strip_led_amount) {
                LOG("    ->LED strip with ");
                LOG(telepils_announce.led_strip_led_amount);
                LOGLN(" LEDs");
            }
            if (telepils_announce.is_access_point)  LOGLN("  warning: Node is configured as access point");
        } break;

        case TELEPILS_NODE_STATUS: {
            LOGLN("  detected TELEPILS_NODE_STATUS packet");
            LOGLN("  no action taken");
        } break;

        case TELEPILS_TEMPERATURE: {
            LOGLN("  detected TELEPILS_TEMPERATURE packet");
            LOGLN("  no action taken");
        } break;

        case TELEPILS_ACCELERATION: {
            LOGLN("  detected TELEPILS_ACCELERATION packet");
            LOGLN("    --> Alerting all nodes");
            oelkast_light_simple_hue.intensity = 255;
            oelkast_light_simple_hue.hue = 255;  // <--- rød
            led_a.set_color(oelkast_light_simple_hue.hue);

            LocalNetworkInterface::send<OelkastLightSimpleHue>(&oelkast_light_simple_hue, BROADCAST);
        } break;

        case TELEPILS_NOISE: {
            LOGLN("  detected TELEPILS_NOISE packet");
            LOG("    noise_level: ");
            LOGLN(telepils_noise.noise_level);
            LOG("    noise_record: ");
            LOGLN(telepils_noise.noise_record);

            if (telepils_noise.noise_record > current_reported_noise_record) {
                LOGLN("    --> New noise record! Alerting all nodes");
                current_reported_noise_record = telepils_noise.noise_record;
                oelkast_light_simple_hue.intensity = 255;
                oelkast_light_simple_hue.hue = 170;  // <--- blå?
                led_a.set_color(oelkast_light_simple_hue.hue);

                LocalNetworkInterface::send<OelkastLightSimpleHue>(&oelkast_light_simple_hue, BROADCAST);
            }
        } break;

        default: {
            LOGLN("  unknown pkg_type --> no action taken");
        }
    }
} // end of void decide_action_on_pkg_receive()
} // end of namespace AccessPointTriggers