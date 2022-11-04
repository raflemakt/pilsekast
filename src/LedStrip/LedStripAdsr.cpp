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

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
    RgbColor ToneColor;
    RgbColor TwoColor;
};

uint8_t u(uint8_t t, uint8_t x)
{
    return (t >= x);
}

MyAnimationState animationState[AnimationChannels];

void AnimationUpdateLinear(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
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
        animationState[param.index].StartingColor,
        animationState[param.index].ToneColor,
        animationState[param.index].TwoColor,
        animationState[param.index].EndingColor,
        param.progress, progress_y);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        stripAdsr.SetPixelColor(pixel, updatedColor);
    }
}

void TurnOnStripAttack(float intensity, float longevity)
{
    RgbColor target = HslColor(120 / 360.0f, 1.0f, intensity);
    uint16_t time = longevity;

    animationState[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationState[0].EndingColor = target;

    animationsAdsr.StartAnimation(0, time, AnimationUpdateLinear);
    stripAdsr.Show();
    expression = 1;
}

void TurnOnStripDuration(float longevity, float level, float intensity)
{
    float luminance_target1 = (intensity - level) * 0.7;
    float luminance_target2 = (intensity - level) * 0.3;
    RgbColor target_1 = HslColor(120 / 360.0f, 1.0f, luminance_target1);
    RgbColor target_2 = HslColor(120 / 360.0f, 1.0f, luminance_target2);
    RgbColor target_3 = HslColor(120 / 360.0f, 1.0f, level);
    uint16_t time = longevity;

    animationState[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationState[0].ToneColor = target_1;
    animationState[0].TwoColor = target_2;
    animationState[0].EndingColor = target_3;

    animationsAdsr.StartAnimation(0, time, AnimationUpdateUnlinear);
    stripAdsr.Show();
}

void TurnOnStripSubstain(float level, float duration, float longevityAttack, float longevityDuration)
{
    uint16_t time = duration - longevityAttack - longevityDuration;

    animationState[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationState[0].EndingColor = stripAdsr.GetPixelColor(0);

    animationsAdsr.StartAnimation(0, time, AnimationUpdateLinear);
    stripAdsr.Show();
}

void TurnOnStripRelease(float luminance, float longevity)
{
    RgbColor target_1 = HslColor(120 / 360.0f, 1.0f, luminance * 0.3);
    RgbColor target_2 = HslColor(120 / 360.0f, 1.0f, luminance * 0.1);
    RgbColor target_3 = RgbColor(0);
    uint16_t time = longevity;

    animationState[0].StartingColor = stripAdsr.GetPixelColor(0);
    animationState[0].ToneColor = target_1;
    animationState[0].TwoColor = target_2;
    animationState[0].EndingColor = target_3;

    animationsAdsr.StartAnimation(0, time, AnimationUpdateUnlinear);
    stripAdsr.Show();
}

void LedStripAdsrUpdate(float light, float time)
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
            TurnOnStripDuration(5, 0.25, 0.5);
            expression = 2;
            break;

        case 2:
            TurnOnStripSubstain(0.25, 15, 5, 5);
            expression = 3;
            break;

        case 3:
            TurnOnStripRelease(0.25, 5);
            expression = 0;
            break;
        }
    }
}