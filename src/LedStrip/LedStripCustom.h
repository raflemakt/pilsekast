#pragma once

void LedStripCustom_setup();
void LedStripCustom_loop();
void LedStripCustomUpdate(float light,float time);
void TurnOnStrip(float luminance, float longevity);
void TurnOffStrip(float luminance, float longevity);
void TurnOffMasterStrip();
extern int flag;
