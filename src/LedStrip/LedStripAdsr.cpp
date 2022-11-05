#include "Arduino.h"
#include "NeoPixelBus.h"
#include <NeoPixelAnimator.h>
#include "LedStripCustom.h"

const uint16_t PixelCount = 22;      // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 4;         // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t AnimationChannels = 1; // we only need one as all the pixels are animated at once
byte expression;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripAdsr(PixelCount, PixelPin);
NeoPixelAnimator animationsAdsr(AnimationChannels); // NeoPixel animation management object

struct MyAnimationStateAdsr
{
    RgbColor StartingColor;
    RgbColor EndingColor;
    RgbColor ToneColor;
    RgbColor TwoColor;
};

MyAnimationStateAdsr animationStateAdsr[AnimationChannels];

void AnimationUpdateLinear(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationStateAdsr[param.index].StartingColor,
        animationStateAdsr[param.index].EndingColor,
        param.progress);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        stripAdsr.SetPixelColor(pixel, updatedColor);
    }
}

void AnimationUpdateUnlinear(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    float progress_y = -pow(param.progress, 3) + 1.89 * param.progress;
    RgbColor updatedColor = RgbColor::BilinearBlend(
        animationStateAdsr[param.index].StartingColor,
        animationStateAdsr[param.index].ToneColor,
        animationStateAdsr[param.index].TwoColor,
        animationStateAdsr[param.index].EndingColor,
        param.progress, progress_y);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        stripAdsr.SetPixelColor(pixel, updatedColor);
    }
}

void TurnOnStripAttack(float intensity, float time_attack)
{
    RgbColor target = HslColor(120 / 360.0f, 1.0f, intensity);
    uint16_t time = time_attack;

    animationStateAdsr[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationStateAdsr[0].EndingColor = target;

    animationsAdsr.StartAnimation(0, time, AnimationUpdateLinear);
    stripAdsr.Show();
    expression = 1;
}

void TurnOnStripdecay(float time_decay, float level, float intensity)
{
    float luminance_target1 = (intensity - level) * 0.7;
    float luminance_target2 = (intensity - level) * 0.3;
    RgbColor target_1 = HslColor(120 / 360.0f, 1.0f, luminance_target1);
    RgbColor target_2 = HslColor(120 / 360.0f, 1.0f, luminance_target2);
    RgbColor target_3 = HslColor(120 / 360.0f, 1.0f, level);
    uint16_t time = time_decay;

    animationStateAdsr[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationStateAdsr[0].ToneColor = target_1;
    animationStateAdsr[0].TwoColor = target_2;
    animationStateAdsr[0].EndingColor = target_3;

    animationsAdsr.StartAnimation(0, time, AnimationUpdateUnlinear);
    stripAdsr.Show();
}

void TurnOnStripSubstain(float level, float duration, float time_attack, float time_decay)
{
    uint16_t time = duration - time_attack - time_decay;

    animationStateAdsr[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationStateAdsr[0].EndingColor = stripAdsr.GetPixelColor(0);

    animationsAdsr.StartAnimation(0, time, AnimationUpdateLinear);
    stripAdsr.Show();
}

void TurnOnStripRelease(float level, float time_release)
{
    RgbColor target_1 = HslColor(120 / 360.0f, 1.0f, level * 0.7);
    RgbColor target_2 = HslColor(120 / 360.0f, 1.0f, level * 0.3);
    RgbColor target_3 = RgbColor(0);
    uint16_t time = time_release;

    animationStateAdsr[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationStateAdsr[0].ToneColor = target_1;
    animationStateAdsr[0].TwoColor = target_2;
    animationStateAdsr[0].EndingColor = target_3;

    animationsAdsr.StartAnimation(0, time, AnimationUpdateUnlinear);
    stripAdsr.Show();
}

void TurnOffMasterStripAdsr()
{
    for (byte Led = 0; Led <= PixelCount; Led++)
    {
        RgbColor black(0);
        stripAdsr.SetPixelColor(Led, black);
    }
    stripAdsr.Show();
}

void LedStripAdsrUpdate(float intensity, float time_attack, float time_decay, float level, float time_release, float duration)
{
    // the normal loop just needs these two to run the active animations
    if (animationsAdsr.IsAnimating())
    {
        animationsAdsr.UpdateAnimations();
        stripAdsr.Show();
    }

    else
    {
        switch (expression)
        {
        case 1:
            TurnOnStripdecay(time_decay, level, intensity);
            expression = 2;
            break;

        case 2:
            TurnOnStripSubstain(level, duration, time_attack, time_decay);
            expression = 3;
            break;

        case 3:
            TurnOnStripRelease(level, time_release);
            expression = 4;
            break;
        case 4:
            TurnOffMasterStripAdsr();
            expression = 0;
            break;
        }
    }
}