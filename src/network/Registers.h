#pragma once
#include "network/protocols/TelepilsPkg.h"
#include "network/protocols/OelkastPkg.h"

extern TelepilsAnnounce telepils_announce;
extern TelepilsNodeStatus telepils_node_status;
extern TelepilsTemperature telepils_temperature;
extern TelepilsNoise telepils_noise;

extern OelkastLightSimple oelkast_light_simple;
extern OelkastLightSimpleHue oelkast_light_simple_hue;
extern OelkastLightEnveloped oelkast_light_enveloped;
extern OelkastLightAnimationMode oelkast_light_animation_mode;