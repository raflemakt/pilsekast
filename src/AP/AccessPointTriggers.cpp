#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"
#include "node/HardwareInstance.h"

namespace AccessPointTriggers
{
    void decide_action_on_pkg_receive()
    {
        Serial.println("Executing action from packet");
        ProtocolDescriptor header = (ProtocolDescriptor)LocalNetworkInterface::transmission_buffer[0];

        switch (header)
        {
        case OELKAST_LIGHT_SIMPLE:
        {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE packet");
            Serial.println("  no action taken");
        }
        break;

        case OELKAST_LIGHT_SIMPLE_HUE:
        {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE_HUE packet");
            Serial.println("  no action taken");
        }
        break;

        case OELKAST_LIGHT_ENVELOPED:
        {
            Serial.println("  detected OELKAST_LIGHT_ENVELOPED packet");
            Serial.println("  no action taken");
        }
        break;

        case OELKAST_LIGHT_ANIMATION_MODE:
        {
            Serial.println("  detected OELKAST_LIGHT_ANIMATION_MODE packet");
            Serial.println("  no action taken");
        }
        break;

        case TELEPILS_ANNOUNCE:
        {
            Serial.println("  detected TELEPILS_ANNOUNCE packet");
            Serial.print("    node_name: ");
            Serial.println(telepils_announce.node_name);
            Serial.print("    instrument_type: ");
            Serial.println(telepils_announce.instrument_type);
            Serial.println("    peripherals on node:");
            if (telepils_announce.has_icm_sensor)     Serial.println("    ->ICM motion sensor");
            if (telepils_announce.has_sound_sensor)   Serial.println("    ->sound sensor");
            if (telepils_announce.has_ttgo_screen)    Serial.println("    ->TTGO screen");
            if (telepils_announce.led_strip_led_amount) Serial.print("    ->LED strip with ");
            Serial.print(telepils_announce.led_strip_led_amount); Serial.println(" LEDs");
            if (telepils_announce.is_access_point)  Serial.println("  warning: Node is configured as access point");
        }
        break;

        case TELEPILS_NODE_STATUS:
        {
            Serial.println("  detected TELEPILS_NODE_STATUS packet");
        }
        break;

        case TELEPILS_TEMPERATURE:
        {
            Serial.println("  detected TELEPILS_TEMPERATURE packet");
        }
        break;

        case TELEPILS_ACCELERATION:
        {
            Serial.println("  detected TELEPILS_ACCELERATION packet");
            Serial.println("    --> Alerting all nodes");
            oelkast_light_simple_hue.intensity = 255;
            oelkast_light_simple_hue.hue = 255;  // <--- rød?
            led_a.set_color(oelkast_light_simple_hue.hue);

            LocalNetworkInterface::send<OelkastLightSimpleHue>(&oelkast_light_simple_hue, BROADCAST);
        }
        break;

        case TELEPILS_NOISE:
        {
            Serial.println("  detected TELEPILS_NOISE packet");
            Serial.print("    noise level: ");
            Serial.println(telepils_noise.noise_level);
            Serial.print("    noise record: ");
            Serial.println(telepils_noise.noise_record);
        }
        break;

        default:
        {
            Serial.println("  unknown pkg_type --> no action taken");
        }
        }
    }
}