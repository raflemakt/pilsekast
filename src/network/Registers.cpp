#include "Registers.h"


// TODO: I stedet for å instansiere alle disse structene der de fleste vil
//       forbli ubrukt kan vi vurdere å gjøre dynamisk allokering av minne.
//       Vi bruker i skrivende stund 12% av RAM, så dette er kanskje en
//       helt unødvendig optimalisering.
TelepilsAnnounce telepils_announce = {0};
TelepilsNodeStatus telepils_node_status = {0};
TelepilsTemperature telepils_temperature = {0};

OelkastLightSimple oelkast_light_simple = {0};
OelkastLightSimpleHue oelkast_light_simple_hue = {0};
OelkastLightEnveloped oelkast_light_enveloped = {0};
OelkastLightAnimationMode oelkast_light_animation_mode = {0};