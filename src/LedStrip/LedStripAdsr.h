#pragma once

void LedStripAdsrUpdate(float intensity, float time_attack, float time_decay, float level, float time_release, float duration, byte R, byte G, byte B);
void TurnOnStripAttack(float intensity, float time_attack, byte R, byte G, byte B);